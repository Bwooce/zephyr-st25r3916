/*
 * Copyright (c) 2026 Bruce Fitzsimons
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * First-light test for an ST25R3916/ST25R3916B on SPI: read the IC identity
 * register and report what answered. This deliberately proves the wiring
 * layers one at a time, before any RF/protocol work:
 *
 *   1. raw register read   — proves power, SPI signals and chip select
 *   2. chip initialisation — proves the IRQ line too (oscillator-stable is
 *                            signalled by interrupt)
 *
 * With nothing wired, both stages fail cleanly and the loop retries — so it
 * doubles as a live wiring-debug tool: watch the log while plugging leads.
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "st25r3916_zephyr.h"
#include "st25r3916.h"
#include "st25r3916_com.h"
#include "rfal_nfc.h"

LOG_MODULE_REGISTER(identity, LOG_LEVEL_INF);

static const char *ic_type_name(uint8_t identity)
{
	switch (identity & ST25R3916_REG_IC_IDENTITY_ic_type_mask) {
	case ST25R3916_REG_IC_IDENTITY_ic_type_st25r3916:
		return "ST25R3916";
	case ST25R3916_REG_IC_IDENTITY_ic_type_st25r3916B:
		return "ST25R3916B";
	default:
		return NULL;
	}
}

int main(void)
{
	LOG_INF("zephyr-st25r3916 identity sample (WIP driver, %s variant build)",
		IS_ENABLED(CONFIG_ST25R3916_VARIANT_B) ? "ST25R3916B" : "ST25R3916");

	int rc = st25r3916_zephyr_init();

	if (rc != 0) {
		LOG_ERR("platform init failed (%d) — check the board overlay", rc);
		return 0;
	}

	while (true) {
		uint8_t identity = 0;

		/* Stage 1: raw identity register over SPI. An absent, unpowered
		 * or miswired chip reads as 0x00 or 0xFF (bus idle/floating).
		 */
		st25r3916ReadRegister(ST25R3916_REG_IC_IDENTITY, &identity);

		const char *name = ic_type_name(identity);

		if (name == NULL) {
			LOG_ERR("no ST25R3916/B on the bus: IC_IDENTITY read 0x%02x "
				"(0x00/0xFF = no chip answering — check power, "
				"wiring, CS polarity)", identity);
			k_sleep(K_SECONDS(3));
			continue;
		}

		LOG_INF("stage 1 PASS: %s answered (IC_IDENTITY=0x%02x, silicon rev %u)",
			name, identity,
			identity & ST25R3916_REG_IC_IDENTITY_ic_rev_mask);

		/* Stage 2: full RFAL initialisation. This runs the chip init
		 * internally (which only accepts the variant this image was
		 * compiled for — CONFIG_ST25R3916_VARIANT_*), starts the
		 * oscillator (completion is signalled by interrupt, so success
		 * proves the IRQ path end-to-end), and loads the analog
		 * configuration table.
		 */
		rc = (int)rfalNfcInitialize();
		if (rc != 0) {
			LOG_ERR("stage 2 FAIL: rfalNfcInitialize() = %d — SPI works "
				"but chip/RFAL bring-up failed (IRQ line wired? "
				"variant mismatch between build and silicon?)", rc);
			k_sleep(K_SECONDS(3));
			continue;
		}

		LOG_INF("stage 2 PASS: RFAL initialised, oscillator stable, IRQ line live");
		LOG_INF("identity check complete — this hardware is ready for RFAL bring-up");
		break;
	}

	/* Park quietly; re-run by resetting. */
	return 0;
}
