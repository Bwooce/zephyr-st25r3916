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
 *
 * Shared-bus arbitration (the reader may share its SPI controller with other
 * devices — e.g. an onboard NOR flash):
 *
 *  RFAL groups logically-related transfers between platformSpiSelect() and
 *  platformSpiDeselect() and expects CS to stay asserted across the whole
 *  group. Driving CS as a bare GPIO (the obvious port) satisfies RFAL but
 *  BYPASSES Zephyr's per-controller bus lock: another device's driver (e.g.
 *  spi_nor) could win the controller mid-group and clock the bus while the
 *  reader's CS is still low — corrupting both devices' transfers.
 *
 *  Instead, the Select..Deselect window is mapped onto the SPI API's own
 *  arbitration primitives, SPI_LOCK_ON | SPI_HOLD_ON_CS:
 *
 *   - platformSpiSelect() touches no hardware; it only opens the group.
 *   - The first platformSpiTxRx() of the group runs spi_transceive() with a
 *     config that carries the devicetree cs-gpios entry plus LOCK_ON and
 *     HOLD_ON_CS: the driver acquires the controller-wide spi_context lock
 *     (the same lock every other device on the bus contends on), asserts CS
 *     itself, transfers, and — because of those two flags — keeps BOTH the
 *     lock and CS across any further transfers in the group.
 *   - platformSpiDeselect() calls spi_release(), which forces CS inactive
 *     and releases the controller lock.
 *
 *  Consequences, all deliberate:
 *   - CS asserts at the group's first transfer, not at Select. No clocking
 *     happens in between, so the chip cannot observe the difference.
 *   - Another device's transfer can no longer interleave inside a group; it
 *     blocks (K_FOREVER, inside the SPI driver) until Deselect. Worst-case
 *     tap-path latency is therefore bounded by the longest transfer any
 *     bus-mate performs — budget accordingly (e.g. chunk NOR-flash erases).
 *   - On a transfer error the nrfx driver force-clears CS even under
 *     HOLD_ON_CS; Deselect's spi_release() then reconciles the lock state
 *     (checked against the driver source, not assumed).
 *   - Correctness requires every Select..Deselect window to sit inside
 *     platformProtectST25RComm(), which the ST comm layer guarantees
 *     (st25r3916comStart/Stop). The comm mutex is what stops the discovery
 *     thread and the IRQ bottom-half from interleaving their OWN groups; the
 *     bus lock is deliberately not recursive-per-thread and both threads use
 *     the same spi_config, so unprotected concurrent groups would corrupt.
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
 *
 * SPI_LOCK_ON + SPI_HOLD_ON_CS implement RFAL's "keep CS low across a group
 * of transfers" contract THROUGH the Zephyr SPI API rather than around it —
 * see the shared-bus arbitration note in the header comment. The devicetree
 * cs-gpios entry stays in the config so the controller owns the CS pin.
 */
#define ST25R3916_SPI_OP (SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | \
			  SPI_TRANSFER_MSB | SPI_MODE_CPHA | \
			  SPI_LOCK_ON | SPI_HOLD_ON_CS)

static const struct spi_dt_spec bus =
	SPI_DT_SPEC_INST_GET(0, ST25R3916_SPI_OP);
static const struct gpio_dt_spec irq_gpio =
	GPIO_DT_SPEC_INST_GET(0, irq_gpios);
static const struct gpio_dt_spec en_gpio =
	GPIO_DT_SPEC_INST_GET_OR(0, en_gpios, {0});

/*
 * True from the first transfer of a Select..Deselect group until Deselect
 * releases the bus. Only ever touched with the comm mutex held (all RFAL
 * Select/TxRx/Deselect sequences run inside platformProtectST25RComm()).
 */
static bool bus_held;

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
	/*
	 * Deliberately no hardware action. CS is asserted by the SPI driver
	 * at the group's first transfer, AFTER it has won the controller
	 * lock — asserting it here, before the lock, would re-open the exact
	 * shared-bus corruption window this design exists to close. No
	 * clocking happens between Select and the first transfer, so the
	 * chip cannot observe the deferred assertion.
	 */
}

void st25r3916_zpf_spi_deselect(void)
{
	int rc;

	if (!bus_held) {
		/* A group with no transfers (e.g. st25r3916Initialize()'s
		 * defensive bare Deselect): nothing was locked, CS is
		 * already parked inactive by the controller.
		 */
		return;
	}

	/* Forces CS inactive and releases the controller lock (verified in
	 * spi_context_unlock_unconditionally()). -EPERM means the driver
	 * already reconciled ownership after a failed transfer — CS is
	 * inactive in that path too (the nrfx driver force-clears CS on
	 * error even under SPI_HOLD_ON_CS).
	 */
	rc = spi_release(bus.bus, &bus.config);
	if (rc != 0 && rc != -EPERM) {
		LOG_ERR("SPI bus release failed (%d)", rc);
	}
	bus_held = false;
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
	rc = spi_transceive(bus.bus, &bus.config,
			    (tx != NULL) ? &tx_set : NULL,
			    (rx != NULL) ? &rx_set : NULL);

	/* Success or failure, the group now owns (or owes) the controller
	 * lock until Deselect reconciles it via spi_release(). On success
	 * SPI_LOCK_ON kept the lock and SPI_HOLD_ON_CS kept CS asserted for
	 * the rest of the group.
	 */
	bus_held = true;

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

	/* spi_is_ready_dt also checks the cs-gpios controller — CS stays in
	 * the config so the SPI driver owns the pin (parked OUTPUT_INACTIVE
	 * by the controller's own init, asserted/held/released per group;
	 * see the shared-bus arbitration note in the header comment).
	 */
	if (!spi_is_ready_dt(&bus)) {
		LOG_ERR("SPI bus %s not ready", bus.bus->name);
		return -ENODEV;
	}
	if (!bus.config.cs.cs_is_gpio) {
		LOG_ERR("no cs-gpios entry for the st,st25r3916 node — the "
			"grouped-transfer CS hold requires GPIO chip select");
		return -ENODEV;
	}
	if (!gpio_is_ready_dt(&irq_gpio)) {
		LOG_ERR("IRQ GPIO controller not ready");
		return -ENODEV;
	}

	k_mutex_init(&comm_mutex);
	k_mutex_init(&irq_status_mutex);

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
	LOG_INF("platform bound: SPI %s @%u Hz, CS %s pin %u (bus-locked "
		"groups), IRQ %s pin %u",
		bus.bus->name, bus.config.frequency,
		bus.config.cs.gpio.port->name, bus.config.cs.gpio.pin,
		irq_gpio.port->name, irq_gpio.pin);
	return 0;
}
