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

#ifdef __cplusplus
}
#endif

#endif /* ST25R3916_ZEPHYR_H */
