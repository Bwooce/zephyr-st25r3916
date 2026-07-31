# Copyright (c) 2026 Bruce Fitzsimons
# SPDX-License-Identifier: Apache-2.0
#
# zephyr-st25r3916 — Zephyr module build.
#
# Layout:
#   third_party/st/rfal/        ST RFAL v3.0.1 (STSW-ST25RFAL002, SLA0052):
#     include/                    RFAL core API headers
#     source/                     RFAL core, unmodified
#     source/st25r3916/           ST25R3916/B chip layer, unmodified
#   third_party/elechouse/      board-specific analog table (ST-derived)
#   src/                        the Zephyr platform shim (Apache-2.0)
#
# The ST sources are compiled verbatim; everything platform-specific enters
# through src/rfal_platform.h.

if(CONFIG_ST25R3916)

zephyr_library_named(st25r3916)

set(ST_RFAL ${CMAKE_CURRENT_SOURCE_DIR}/third_party/st/rfal)

# Include order matters for the analog-configuration table:
# rfal_analogConfig.c does `#include "rfal_analogConfigTbl.h"`, which exists
# both in the ST chip layer (generic defaults) and, when selected, in
# third_party/elechouse/ (ElecHouse ST25R3916B Mini antenna tuning). Placing
# the elechouse directory FIRST shadows the generic table.
if(CONFIG_ST25R3916_ANALOG_CONFIG_ELECHOUSE)
  zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/elechouse)
endif()

zephyr_include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${ST_RFAL}/include
  # A few headers (rfal_crc.h, rfal_iso15693_2.h) live beside the sources.
  ${ST_RFAL}/source
  ${ST_RFAL}/source/st25r3916
)

set(ST_SOURCES
  # ST25R3916/B chip layer.
  ${ST_RFAL}/source/st25r3916/st25r3916.c
  ${ST_RFAL}/source/st25r3916/st25r3916_com.c
  ${ST_RFAL}/source/st25r3916/st25r3916_irq.c
  ${ST_RFAL}/source/st25r3916/st25r3916_aat.c
  ${ST_RFAL}/source/st25r3916/st25r3916_led.c
  ${ST_RFAL}/source/st25r3916/rfal_rfst25r3916.c

  # RFAL core.
  ${ST_RFAL}/source/rfal_analogConfig.c
  ${ST_RFAL}/source/rfal_cd.c
  ${ST_RFAL}/source/rfal_crc.c
  ${ST_RFAL}/source/rfal_dpo.c
  ${ST_RFAL}/source/rfal_iso15693_2.c
  ${ST_RFAL}/source/rfal_isoDep.c
  ${ST_RFAL}/source/rfal_nfc.c
  ${ST_RFAL}/source/rfal_nfca.c
  ${ST_RFAL}/source/rfal_nfcb.c
  ${ST_RFAL}/source/rfal_nfcDep.c
  ${ST_RFAL}/source/rfal_nfcf.c
  ${ST_RFAL}/source/rfal_nfcv.c
  ${ST_RFAL}/source/rfal_st25tb.c
  ${ST_RFAL}/source/rfal_st25xv.c
  ${ST_RFAL}/source/rfal_t1t.c
  ${ST_RFAL}/source/rfal_t2t.c
  ${ST_RFAL}/source/rfal_t4t.c
)

zephyr_library_sources(
  src/st25r3916_platform.c
  ${ST_SOURCES}
)

# ST's sources are not warning-clean under Zephyr's default flags; keep the
# vendored code compiling verbatim rather than patching it. Our own shim in
# src/ builds with the project's normal warnings.
set_source_files_properties(${ST_SOURCES}
  PROPERTIES COMPILE_OPTIONS "-Wno-unused-parameter"
)

endif()
