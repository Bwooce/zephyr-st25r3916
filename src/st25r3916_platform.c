/*
 * Copyright (c) 2026 Bruce Fitzsimons
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Zephyr implementation of the RFAL platform contract declared in
 * rfal_platform.h, bound to the single devicetree instance of
 * "st,st25r3916".
 *
 * Threading model:
 *  - All RFAL API / worker calls happen in the caller's thread.
 *  - The ST25R3916 IRQ line (edge, active-high) fires a GPIO ISR, which only
 *    submits a work item to a dedicated work queue. The work handler calls
 *    st25r3916Isr(), which reads the interrupt registers over SPI — that is
 *    why it cannot run in real ISR context.
 *  - A recursive comm mutex serialises SPI access between those two threads;
 *    a second mutex protects the shared IRQ-status word.
 */

#define DT_DRV_COMPAT st_st25r3916

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "rfal_platform.h"
#include "st25r3916_zephyr.h"

LOG_MODULE_REGISTER(st25r3916, CONFIG_ST25R3916_LOG_LEVEL);

#if DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT) != 1
#error "RFAL is a singleton: exactly one enabled st,st25r3916 devicetree node is required"
#endif

/*
 * ST25R3916 SPI format: mode 1 (CPOL=0, CPHA=1), MSB first, 8-bit words,
 * up to 10 MHz. The frequency comes from the node's spi-max-frequency.
 * Chip select is driven manually (see rfal_platform.h), so the spi_config
 * carries no cs — Zephyr must not toggle it per transfer.
 */
#define ST25R3916_SPI_OP (SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | \
			  SPI_TRANSFER_MSB | SPI_MODE_CPHA)

static const struct spi_dt_spec bus =
	SPI_DT_SPEC_INST_GET(0, ST25R3916_SPI_OP);
static const struct gpio_dt_spec cs_gpio =
	SPI_CS_GPIOS_DT_SPEC_INST_GET(0);
static const struct gpio_dt_spec irq_gpio =
	GPIO_DT_SPEC_INST_GET(0, irq_gpios);
static const struct gpio_dt_spec en_gpio =
	GPIO_DT_SPEC_INST_GET_OR(0, en_gpios, {0});

/* spi_dt_spec's config includes the devicetree cs-gpios; strip it so the
 * controller never touches CS. Filled in at init from bus.config.
 */
static struct spi_config spi_cfg;

static struct k_mutex comm_mutex;
static struct k_mutex irq_status_mutex;

static void (*irq_callback)(void);

static K_THREAD_STACK_DEFINE(irq_wq_stack, CONFIG_ST25R3916_IRQ_THREAD_STACK_SIZE);
static struct k_work_q irq_wq;
static struct k_work irq_work;
static struct gpio_callback irq_cb_data;

static bool initialised;

/* ---------------------------------------------------------------- IRQ -- */

static void irq_work_handler(struct k_work *work)
{
	ARG_UNUSED(work);

	if (irq_callback != NULL) {
		/* st25r3916Isr() itself loops while the IRQ line is high, so
		 * one invocation per submission is sufficient even if further
		 * edges arrived meanwhile.
		 */
		irq_callback();
	}
}

static void irq_isr(const struct device *port, struct gpio_callback *cb,
		    uint32_t pins)
{
	ARG_UNUSED(port);
	ARG_UNUSED(cb);
	ARG_UNUSED(pins);

	k_work_submit_to_queue(&irq_wq, &irq_work);
}

bool st25r3916_zpf_irq_pin_is_high(void)
{
	return gpio_pin_get_dt(&irq_gpio) > 0;
}

void st25r3916_zpf_irq_set_callback(void (*cb)(void))
{
	irq_callback = cb;
}

/* ---------------------------------------------------------------- SPI -- */

void st25r3916_zpf_spi_select(void)
{
	gpio_pin_set_dt(&cs_gpio, 1);
}

void st25r3916_zpf_spi_deselect(void)
{
	gpio_pin_set_dt(&cs_gpio, 0);
}

void st25r3916_zpf_spi_txrx(const uint8_t *tx, uint8_t *rx, uint16_t len)
{
	const struct spi_buf tx_buf = { .buf = (void *)tx, .len = len };
	const struct spi_buf rx_buf = { .buf = rx, .len = len };
	const struct spi_buf_set tx_set = { .buffers = &tx_buf, .count = 1 };
	const struct spi_buf_set rx_set = { .buffers = &rx_buf, .count = 1 };
	int rc;

	/* RFAL passes tx == NULL for pure reads and rx == NULL for pure
	 * writes; spi_transceive handles NULL buf entries by clocking zeroes
	 * out / discarding input, which matches the ST expectation.
	 */
	rc = spi_transceive(bus.bus, &spi_cfg,
			    (tx != NULL) ? &tx_set : NULL,
			    (rx != NULL) ? &rx_set : NULL);
	if (rc != 0) {
		LOG_ERR("SPI transceive failed (%d)", rc);
	}
}

/* ------------------------------------------------------------- locking -- */

void st25r3916_zpf_comm_lock(void)
{
	k_mutex_lock(&comm_mutex, K_FOREVER);
}

void st25r3916_zpf_comm_unlock(void)
{
	k_mutex_unlock(&comm_mutex);
}

void st25r3916_zpf_irq_status_lock(void)
{
	k_mutex_lock(&irq_status_mutex, K_FOREVER);
}

void st25r3916_zpf_irq_status_unlock(void)
{
	k_mutex_unlock(&irq_status_mutex);
}

/* ---------------------------------------------------------- time & log -- */

void st25r3916_zpf_delay_ms(uint32_t ms)
{
	k_msleep((int32_t)ms);
}

uint32_t st25r3916_zpf_uptime_ms(void)
{
	return k_uptime_get_32();
}

uint32_t st25r3916_zpf_timer_create(uint16_t ms)
{
	return k_uptime_get_32() + ms;
}

bool st25r3916_zpf_timer_is_expired(uint32_t timer)
{
	/* Wrap-safe: signed distance from the deadline. */
	return (int32_t)(k_uptime_get_32() - timer) >= 0;
}

void st25r3916_zpf_log(const char *fmt, ...)
{
	char buf[CONFIG_ST25R3916_LOG_BUF_SIZE];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	/* ST logs already carry their own framing/newlines; strip a trailing
	 * newline so Zephyr's per-message framing doesn't double-space.
	 */
	size_t n = strlen(buf);

	while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r')) {
		buf[--n] = '\0';
	}
	if (n > 0) {
		LOG_INF("%s", buf);
	}
}

void st25r3916_zpf_error_handle(const char *file, int line)
{
	/* ST calls this on unrecoverable comm faults. Log loudly but do not
	 * take the system down: an unwired/unpowered reader must degrade to
	 * error returns, not a hang or reboot.
	 */
	LOG_ERR("RFAL platform error trap at %s:%d", file, line);
}

/* ----------------------------------------------------------------- init -- */

int st25r3916_zephyr_init(void)
{
	int rc;

	if (initialised) {
		return 0;
	}

	if (!spi_is_ready_dt(&bus)) {
		LOG_ERR("SPI bus %s not ready", bus.bus->name);
		return -ENODEV;
	}
	if (!gpio_is_ready_dt(&cs_gpio) || !gpio_is_ready_dt(&irq_gpio)) {
		LOG_ERR("CS/IRQ GPIO controller not ready");
		return -ENODEV;
	}

	/* Manual chip select: copy the devicetree-derived config, then drop
	 * its cs so the SPI driver never drives the pin itself.
	 */
	spi_cfg = bus.config;
	spi_cfg.cs = (struct spi_cs_control){0};

	k_mutex_init(&comm_mutex);
	k_mutex_init(&irq_status_mutex);

	rc = gpio_pin_configure_dt(&cs_gpio, GPIO_OUTPUT_INACTIVE);
	if (rc != 0) {
		LOG_ERR("CS configure failed (%d)", rc);
		return rc;
	}

	if (en_gpio.port != NULL) {
		rc = gpio_pin_configure_dt(&en_gpio, GPIO_OUTPUT_ACTIVE);
		if (rc != 0) {
			LOG_ERR("EN configure failed (%d)", rc);
			return rc;
		}
		/* Give the module's supply/oscillator a moment. */
		k_msleep(5);
	}

	rc = gpio_pin_configure_dt(&irq_gpio, GPIO_INPUT);
	if (rc != 0) {
		LOG_ERR("IRQ pin configure failed (%d)", rc);
		return rc;
	}

	const struct k_work_queue_config wq_cfg = {
		.name = "st25r3916_irq",
	};

	k_work_queue_init(&irq_wq);
	k_work_queue_start(&irq_wq, irq_wq_stack,
			   K_THREAD_STACK_SIZEOF(irq_wq_stack),
			   CONFIG_ST25R3916_IRQ_THREAD_PRIORITY, &wq_cfg);
	k_work_init(&irq_work, irq_work_handler);

	gpio_init_callback(&irq_cb_data, irq_isr, BIT(irq_gpio.pin));
	rc = gpio_add_callback(irq_gpio.port, &irq_cb_data);
	if (rc != 0) {
		LOG_ERR("IRQ callback add failed (%d)", rc);
		return rc;
	}

	rc = gpio_pin_interrupt_configure_dt(&irq_gpio, GPIO_INT_EDGE_TO_ACTIVE);
	if (rc != 0) {
		/* On nRF54L15, port P2 has no GPIOTE instance at all: this
		 * call returns -ENOTSUP and the reader could never wake the
		 * system. Name the trap rather than letting it pass.
		 */
		LOG_ERR("IRQ interrupt configure failed (%d)%s", rc,
			(rc == -ENOTSUP) ?
			" — the irq-gpios pin cannot generate interrupts on "
			"this SoC (nRF54L15: P2 has no GPIOTE; use P0/P1)" : "");
		return rc;
	}

	initialised = true;
	LOG_INF("platform bound: SPI %s @%u Hz, CS %s pin %u, IRQ %s pin %u",
		bus.bus->name, spi_cfg.frequency,
		cs_gpio.port->name, cs_gpio.pin,
		irq_gpio.port->name, irq_gpio.pin);
	return 0;
}
