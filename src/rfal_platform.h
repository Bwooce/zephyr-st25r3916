/*
 * Copyright (c) 2026 Bruce Fitzsimons
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Zephyr platform layer for ST's RFAL (RF Abstraction Layer).
 *
 * RFAL's portable sources (third_party/st/) include this header by name and
 * expect it to supply the platform contract: SPI transfer, chip select, the
 * IRQ line, millisecond time, delays, locking, and logging. Every macro here
 * maps onto a small set of functions implemented in st25r3916_platform.c
 * against Zephyr APIs (spi_dt_spec, gpio_dt_spec, k_uptime_get_32, k_mutex,
 * a dedicated work queue for the IRQ bottom half, and LOG_MODULE logging).
 *
 * This header deliberately includes no Zephyr headers: it is pulled into
 * every ST translation unit, and the contract below is plain C99. The Zephyr
 * types stay confined to st25r3916_platform.c.
 *
 * NOTE: RFAL is architecturally a singleton (its state lives in file-scope
 * statics inside the ST sources), so this platform layer binds to exactly one
 * devicetree instance of "st,st25r3916". A second instance is a build error
 * in st25r3916_platform.c, not a silent misbehaviour.
 */

#ifndef RFAL_PLATFORM_H
#define RFAL_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

/*
 * ------------------------------------------------------------------------
 * Chip variant selection.
 *
 * ST's sources require exactly one of ST25R3916 / ST25R3916B to be defined
 * (st25r3916.c errors out otherwise); the two variants differ in register
 * layout details, AAT handling and oscillator bring-up. Selected via
 * Kconfig: CONFIG_ST25R3916_VARIANT_B.
 * ------------------------------------------------------------------------
 */
#if defined(CONFIG_ST25R3916_VARIANT_B)
#define ST25R3916B
#else
#define ST25R3916
#endif

/*
 * Single-buffer SPI strategy: the ST comm layer (st25r3916_com.c) coalesces
 * each register/FIFO operation into one full-duplex SPI transfer instead of
 * several back-to-back partial transfers. One Zephyr spi_transceive() per
 * chip-select window — cheaper, and far friendlier to SPI controllers with
 * per-transfer overhead.
 */
#define ST25R_COM_SINGLETXRX

/*
 * ------------------------------------------------------------------------
 * RFAL feature configuration.
 *
 * The platform header is where ST expects these to live (rfal_defConfig.h
 * only backfills a subset, and not every translation unit includes it).
 * Defaults chosen for a poller/reader: all poll technologies on, listen
 * mode / P2P off, wake-up mode on (low-power card detection is the whole
 * point of this IC in a battery-powered reader).
 * ------------------------------------------------------------------------
 */
#define RFAL_FEATURE_LISTEN_MODE               false  /*!< Listen (card emulation) mode      */
#define RFAL_FEATURE_WAKEUP_MODE               true   /*!< Wake-Up (low-power detect) mode   */
#define RFAL_FEATURE_LOWPOWER_MODE             false  /*!< Low-power chip mode               */
#define RFAL_FEATURE_NFCA                      true   /*!< NFC-A (ISO14443A)                 */
#define RFAL_FEATURE_NFCB                      true   /*!< NFC-B (ISO14443B)                 */
#define RFAL_FEATURE_NFCF                      true   /*!< NFC-F (FeliCa)                    */
#define RFAL_FEATURE_NFCV                      true   /*!< NFC-V (ISO15693)                  */
#define RFAL_FEATURE_T1T                       true   /*!< T1T (Topaz)                       */
#define RFAL_FEATURE_T2T                       true   /*!< T2T                               */
#define RFAL_FEATURE_T4T                       true   /*!< T4T                               */
#define RFAL_FEATURE_ST25TB                    true   /*!< ST25TB                            */
#define RFAL_FEATURE_ST25xV                    true   /*!< ST25V RF extensions               */
#define RFAL_FEATURE_DYNAMIC_ANALOG_CONFIG     false  /*!< Analog table updatable at runtime */
#define RFAL_FEATURE_DPO                       false  /*!< Dynamic power output              */
#define RFAL_FEATURE_ISO_DEP                   true   /*!< ISO-DEP (ISO14443-4)              */
#define RFAL_FEATURE_ISO_DEP_POLL              true   /*!< ISO-DEP poller (PCD)              */
#define RFAL_FEATURE_ISO_DEP_LISTEN            false  /*!< ISO-DEP listener (PICC)           */
#define RFAL_FEATURE_NFC_DEP                   false  /*!< NFC-DEP (NFCIP1/P2P)              */

#define RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN    256U   /*!< ISO-DEP I-Block max length        */
#define RFAL_FEATURE_ISO_DEP_APDU_MAX_LEN      512U   /*!< ISO-DEP APDU max length           */
#define RFAL_FEATURE_NFC_DEP_BLOCK_MAX_LEN     254U   /*!< NFC-DEP block/payload max length  */
#define RFAL_FEATURE_NFC_DEP_PDU_MAX_LEN       512U   /*!< NFC-DEP PDU max length            */
#define RFAL_FEATURE_NFC_RF_BUF_LEN            258U   /*!< RF buffer length (RFAL NFC layer) */

/*
 * ------------------------------------------------------------------------
 * Functions provided by st25r3916_platform.c
 * ------------------------------------------------------------------------
 */
void     st25r3916_zpf_spi_select(void);
void     st25r3916_zpf_spi_deselect(void);
void     st25r3916_zpf_spi_txrx(const uint8_t *tx, uint8_t *rx, uint16_t len);
bool     st25r3916_zpf_irq_pin_is_high(void);
void     st25r3916_zpf_irq_set_callback(void (*cb)(void));
void     st25r3916_zpf_comm_lock(void);
void     st25r3916_zpf_comm_unlock(void);
void     st25r3916_zpf_irq_status_lock(void);
void     st25r3916_zpf_irq_status_unlock(void);
void     st25r3916_zpf_delay_ms(uint32_t ms);
uint32_t st25r3916_zpf_uptime_ms(void);
void     st25r3916_zpf_log(const char *fmt, ...);
void     st25r3916_zpf_error_handle(const char *file, int line);

/* Millisecond one-shot timers (wrap-safe over the 32-bit uptime counter).
 * ST's portable code only ever uses these through the platformTimer* macros
 * below.
 */
uint32_t st25r3916_zpf_timer_create(uint16_t ms);
bool     st25r3916_zpf_timer_is_expired(uint32_t timer);

/*
 * ------------------------------------------------------------------------
 * The macro contract RFAL compiles against.
 * ------------------------------------------------------------------------
 */

/* Serialise access to the SPI channel between the caller's thread and the
 * IRQ bottom-half work item. Implemented as a recursive k_mutex — the ST
 * code nests Protect calls (the STM32 reference keeps a nesting counter for
 * the same reason). Never called from real ISR context: the ST25R interrupt
 * is demoted to a dedicated work queue thread before any RFAL code runs.
 */
#define platformProtectST25RComm()          st25r3916_zpf_comm_lock()
#define platformUnprotectST25RComm()        st25r3916_zpf_comm_unlock()

/* Same, for the interrupt-status word shared between st25r3916Isr() (work
 * queue thread) and RFAL worker calls (application thread).
 */
#define platformProtectST25RIrqStatus()     st25r3916_zpf_irq_status_lock()
#define platformUnprotectST25RIrqStatus()   st25r3916_zpf_irq_status_unlock()

/* RFAL's worker is only ever driven from one thread here; no lock needed. */
#define platformProtectWorker()
#define platformUnprotectWorker()

/* IRQ line plumbing. Pin init happens in st25r3916_zephyr_init() (devicetree
 * owns the pin), so PinInitialize is a no-op; SetCallback registers
 * st25r3916Isr() as the bottom-half handler.
 */
#define platformIrqST25RPinInitialize()
#define platformIrqST25RSetCallback(cb)     st25r3916_zpf_irq_set_callback(cb)

/* st25r3916Isr() re-reads the IRQ registers for as long as the (edge
 * triggered, active-high) IRQ line is still asserted. The port/pin arguments
 * are STM32 idioms; the devicetree gpio_dt_spec is the single source of
 * truth, so they are accepted and ignored.
 */
#define ST25R_INT_PORT                      0
#define ST25R_INT_PIN                       0
#define platformGpioIsHigh(port, pin)       st25r3916_zpf_irq_pin_is_high()

/* SPI. Chip select is driven manually (not by the SPI controller) because
 * the ST comm layer brackets logically-grouped transfers between Select and
 * Deselect itself.
 */
#define platformSpiSelect()                 st25r3916_zpf_spi_select()
#define platformSpiDeselect()               st25r3916_zpf_spi_deselect()
#define platformSpiTxRx(txBuf, rxBuf, len)  st25r3916_zpf_spi_txrx((txBuf), (rxBuf), (len))

/* Millisecond software timers. */
#define platformTimerCreate(t)              st25r3916_zpf_timer_create(t)
#define platformTimerIsExpired(timer)       st25r3916_zpf_timer_is_expired(timer)
#define platformTimerDestroy(timer)
#define platformDelay(t)                    st25r3916_zpf_delay_ms(t)
#define platformGetSysTick()                st25r3916_zpf_uptime_ms()

#define platformErrorHandle()               st25r3916_zpf_error_handle(__FILE__, __LINE__)

/* LEDs: the ST sources poke debug LEDs in a couple of places; not wired. */
#define platformLedsInitialize()
#define platformLedOff(port, pin)
#define platformLedOn(port, pin)

#define platformLog(...)                    st25r3916_zpf_log(__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* RFAL_PLATFORM_H */
