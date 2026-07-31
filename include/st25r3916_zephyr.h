/*
 * Copyright (c) 2026 Bruce Fitzsimons
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Public entry point of the zephyr-st25r3916 module.
 *
 * Call st25r3916_zephyr_init() once, before any RFAL or st25r3916_* call.
 * It binds the platform layer (SPI, CS, IRQ, EN) to the devicetree instance
 * of "st,st25r3916" and starts the IRQ bottom-half work queue. After it
 * returns 0, ST's own APIs are usable directly:
 *
 *   - chip layer:  #include "st25r3916.h", "st25r3916_com.h", ...
 *   - RFAL:        #include "rfal_nfc.h", "rfal_rf.h", ...
 *
 * Deliberately not a SYS_INIT hook: this module is in bring-up, and an
 * explicit, fallible init in the application keeps failure modes visible.
 */

#ifndef ST25R3916_ZEPHYR_H
#define ST25R3916_ZEPHYR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Bind the RFAL platform layer to the st,st25r3916 devicetree node.
 *
 * Idempotent; safe to call again after a failure once the cause (wiring,
 * power, overlay) is fixed.
 *
 * @retval 0        success
 * @retval -ENODEV  SPI bus or GPIO controller not ready
 * @retval -ENOTSUP the irq-gpios pin cannot generate interrupts on this SoC
 *                  (on nRF54L15, any P2 pin: no GPIOTE instance serves P2)
 * @retval other    negative errno from GPIO configuration
 */
int st25r3916_zephyr_init(void);

/**
 * @brief Register a callback invoked AFTER each IRQ bottom-half completes.
 *
 * RFAL observes chip interrupts only through the status word that
 * st25r3916Isr() latches over SPI in the bottom-half work queue, and only
 * from a subsequent rfalNfcWorker()/rfalWorker() pass. An application that
 * blocks indefinitely between worker passes must schedule the next pass from
 * THIS hook: a wake taken at the raw GPIO edge can run before the SPI read
 * has latched anything, observe no change, and sleep through the event.
 *
 * Runs in the bottom-half work queue thread (not ISR context). One callback;
 * NULL clears it.
 */
void st25r3916_zephyr_set_irq_notify(void (*cb)(void));

/**
 * @brief Milliseconds until the soonest pending RFAL software timer.
 *
 * The platform layer shadows every platformTimerCreate() deadline (RFAL
 * never destroys timers through the platform — platformTimerDestroy is a
 * no-op — so tracked deadlines are dropped once expired and reported).
 *
 * @retval -1  no RFAL timer pending: nothing time-driven can happen, an
 *             indefinite IRQ-only wait is safe from RFAL's point of view
 * @retval 0   a timer has expired since the last call (each expiry is
 *             reported exactly once): run the worker now
 * @retval >0  ms until the soonest pending timer: sleeping that long cannot
 *             skip any RFAL deadline
 *
 * Single consumer assumed (the thread that pumps the RFAL worker): the
 * report-once semantics of 0 make concurrent callers race for the report.
 */
int32_t st25r3916_zephyr_next_timer_ms(void);

#ifdef __cplusplus
}
#endif

#endif /* ST25R3916_ZEPHYR_H */
