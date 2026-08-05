/*
 * PROVENANCE NOTE (added by the zephyr-st25r3916 module, 2026-07-31; the ST
 * copyright and licence notice below is retained unmodified as required):
 *
 * This analog-configuration table was taken from the ElecHouse ST25R3916B
 * Mini module Arduino library:
 *   https://github.com/wilson-elechouse/ST25R3916
 *   (ST25R3916_ELECHOUSE/src/rfal_rfst25r3916_analogConfigTbl.h)
 * It carries antenna/board-specific tuning for that exact module (antenna
 * tuning registers, field thresholds, RFO resistance, load modulation),
 * which differs materially from ST's generic default table. It is an
 * ST-derived file (see the retained notice below) with ElecHouse's tuning
 * applied, and remains under ST's licence terms — see ../st/SLA0052.txt
 * and the discussion in the repository README.
 *
 * Local changes for this module, beyond this added comment block: the two
 * #include lines were renamed from the Arduino C++ port's file names
 * (rfal_rfst25r3916_analogConfig.h) to the ST C distribution's names
 * (rfal_analogConfig.h), and a compatibility alias is provided after the
 * includes for one register-bit macro this table uses under its older ST
 * name: AUX_MOD bit 7 is ST25R3916_REG_AUX_MOD_am_mode in the headers this
 * table was written against, and ST25R3916_REG_AUX_MOD_dis_reg_am (same
 * bit, same meaning) in the RFAL v3.0.1 headers vendored here. Table data
 * is untouched.
 */
/******************************************************************************
  * \attention
  *
  * <h2><center>&copy; COPYRIGHT 2021 STMicroelectronics</center></h2>
  *
  * Licensed under ST MIX MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        www.st.com/mix_myliberty
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
******************************************************************************/

/*! \file
 *
 *  \author SRA
 *
 *  \brief ST25R3916 Analog Configuration Settings
 *
 */

#ifndef ST25R3916_ANALOGCONFIG_H
#define ST25R3916_ANALOGCONFIG_H

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
#include "rfal_analogConfig.h"
#include "st25r3916_com.h"

/* Compatibility alias — see the provenance note above. */
#ifndef ST25R3916_REG_AUX_MOD_am_mode
#define ST25R3916_REG_AUX_MOD_am_mode  ST25R3916_REG_AUX_MOD_dis_reg_am
#endif

/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */

/*
 ******************************************************************************
 * GLOBAL MACROS
 ******************************************************************************
 */

/*! Macro for Configuration Setting with only one register-mask-value set:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1] */
#define MODE_ENTRY_1_REG(MODE, R0, M0, V0)              \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU), 1, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0)

/*! Macro for Configuration Setting with only two register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1] */
#define MODE_ENTRY_2_REG(MODE, R0, M0, V0, R1, M1, V1)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU), 2, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1)

/*! Macro for Configuration Setting with only three register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_3_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU), 3, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1) \
                                 , (uint8_t)((R2) >> 8), (uint8_t)((R2) & 0xFFU), (uint8_t)(M2), (uint8_t)(V2)

/*! Macro for Configuration Setting with only four register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_4_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU), 4, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1) \
                                 , (uint8_t)((R2) >> 8), (uint8_t)((R2) & 0xFFU), (uint8_t)(M2), (uint8_t)(V2) \
                                 , (uint8_t)((R3) >> 8), (uint8_t)((R3) & 0xFFU), (uint8_t)(M3), (uint8_t)(V3)

/*! Macro for Configuration Setting with only five register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_5_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU), 5, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1) \
                                 , (uint8_t)((R2) >> 8), (uint8_t)((R2) & 0xFFU), (uint8_t)(M2), (uint8_t)(V2) \
                                 , (uint8_t)((R3) >> 8), (uint8_t)((R3) & 0xFFU), (uint8_t)(M3), (uint8_t)(V3) \
                                 , (uint8_t)((R4) >> 8), (uint8_t)((R4) & 0xFFU), (uint8_t)(M4), (uint8_t)(V4)

/*! Macro for Configuration Setting with only six register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_6_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU), 6, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1) \
                                 , (uint8_t)((R2) >> 8), (uint8_t)((R2) & 0xFFU), (uint8_t)(M2), (uint8_t)(V2) \
                                 , (uint8_t)((R3) >> 8), (uint8_t)((R3) & 0xFFU), (uint8_t)(M3), (uint8_t)(V3) \
                                 , (uint8_t)((R4) >> 8), (uint8_t)((R4) & 0xFFU), (uint8_t)(M4), (uint8_t)(V4) \
                                 , (uint8_t)((R5) >> 8), (uint8_t)((R5) & 0xFFU), (uint8_t)(M5), (uint8_t)(V5)

/*! Macro for Configuration Setting with only seven register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_7_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU), 7, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1) \
                                 , (uint8_t)((R2) >> 8), (uint8_t)((R2) & 0xFFU), (uint8_t)(M2), (uint8_t)(V2) \
                                 , (uint8_t)((R3) >> 8), (uint8_t)((R3) & 0xFFU), (uint8_t)(M3), (uint8_t)(V3) \
                                 , (uint8_t)((R4) >> 8), (uint8_t)((R4) & 0xFFU), (uint8_t)(M4), (uint8_t)(V4) \
                                 , (uint8_t)((R5) >> 8), (uint8_t)((R5) & 0xFFU), (uint8_t)(M5), (uint8_t)(V5) \
                                 , (uint8_t)((R6) >> 8), (uint8_t)((R6) & 0xFFU), (uint8_t)(M6), (uint8_t)(V6)

/*! Macro for Configuration Setting with only eight register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_8_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU), 8, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1) \
                                 , (uint8_t)((R2) >> 8), (uint8_t)((R2) & 0xFFU), (uint8_t)(M2), (uint8_t)(V2) \
                                 , (uint8_t)((R3) >> 8), (uint8_t)((R3) & 0xFFU), (uint8_t)(M3), (uint8_t)(V3) \
                                 , (uint8_t)((R4) >> 8), (uint8_t)((R4) & 0xFFU), (uint8_t)(M4), (uint8_t)(V4) \
                                 , (uint8_t)((R5) >> 8), (uint8_t)((R5) & 0xFFU), (uint8_t)(M5), (uint8_t)(V5) \
                                 , (uint8_t)((R6) >> 8), (uint8_t)((R6) & 0xFFU), (uint8_t)(M6), (uint8_t)(V6) \
                                 , (uint8_t)((R7) >> 8), (uint8_t)((R7) & 0xFFU), (uint8_t)(M7), (uint8_t)(V7)

/*! Macro for Configuration Setting with only nine register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_9_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU), 9, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1) \
                                 , (uint8_t)((R2) >> 8), (uint8_t)((R2) & 0xFFU), (uint8_t)(M2), (uint8_t)(V2) \
                                 , (uint8_t)((R3) >> 8), (uint8_t)((R3) & 0xFFU), (uint8_t)(M3), (uint8_t)(V3) \
                                 , (uint8_t)((R4) >> 8), (uint8_t)((R4) & 0xFFU), (uint8_t)(M4), (uint8_t)(V4) \
                                 , (uint8_t)((R5) >> 8), (uint8_t)((R5) & 0xFFU), (uint8_t)(M5), (uint8_t)(V5) \
                                 , (uint8_t)((R6) >> 8), (uint8_t)((R6) & 0xFFU), (uint8_t)(M6), (uint8_t)(V6) \
                                 , (uint8_t)((R7) >> 8), (uint8_t)((R7) & 0xFFU), (uint8_t)(M7), (uint8_t)(V7) \
                                 , (uint8_t)((R8) >> 8), (uint8_t)((R8) & 0xFFU), (uint8_t)(M8), (uint8_t)(V8)

/*! Macro for Configuration Setting with only ten register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_10_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU),10, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1) \
                                 , (uint8_t)((R2) >> 8), (uint8_t)((R2) & 0xFFU), (uint8_t)(M2), (uint8_t)(V2) \
                                 , (uint8_t)((R3) >> 8), (uint8_t)((R3) & 0xFFU), (uint8_t)(M3), (uint8_t)(V3) \
                                 , (uint8_t)((R4) >> 8), (uint8_t)((R4) & 0xFFU), (uint8_t)(M4), (uint8_t)(V4) \
                                 , (uint8_t)((R5) >> 8), (uint8_t)((R5) & 0xFFU), (uint8_t)(M5), (uint8_t)(V5) \
                                 , (uint8_t)((R6) >> 8), (uint8_t)((R6) & 0xFFU), (uint8_t)(M6), (uint8_t)(V6) \
                                 , (uint8_t)((R7) >> 8), (uint8_t)((R7) & 0xFFU), (uint8_t)(M7), (uint8_t)(V7) \
                                 , (uint8_t)((R8) >> 8), (uint8_t)((R8) & 0xFFU), (uint8_t)(M8), (uint8_t)(V8) \
                                 , (uint8_t)((R9) >> 8), (uint8_t)((R9) & 0xFFU), (uint8_t)(M9), (uint8_t)(V9)

/*! Macro for Configuration Setting with eleven register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_11_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9, R10, M10, V10)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU),11, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1) \
                                 , (uint8_t)((R2) >> 8), (uint8_t)((R2) & 0xFFU), (uint8_t)(M2), (uint8_t)(V2) \
                                 , (uint8_t)((R3) >> 8), (uint8_t)((R3) & 0xFFU), (uint8_t)(M3), (uint8_t)(V3) \
                                 , (uint8_t)((R4) >> 8), (uint8_t)((R4) & 0xFFU), (uint8_t)(M4), (uint8_t)(V4) \
                                 , (uint8_t)((R5) >> 8), (uint8_t)((R5) & 0xFFU), (uint8_t)(M5), (uint8_t)(V5) \
                                 , (uint8_t)((R6) >> 8), (uint8_t)((R6) & 0xFFU), (uint8_t)(M6), (uint8_t)(V6) \
                                 , (uint8_t)((R7) >> 8), (uint8_t)((R7) & 0xFFU), (uint8_t)(M7), (uint8_t)(V7) \
                                 , (uint8_t)((R8) >> 8), (uint8_t)((R8) & 0xFFU), (uint8_t)(M8), (uint8_t)(V8) \
                                 , (uint8_t)((R9) >> 8), (uint8_t)((R9) & 0xFFU), (uint8_t)(M9), (uint8_t)(V9) \
                                 , (uint8_t)((R10) >> 8), (uint8_t)((R10) & 0xFFU), (uint8_t)(M10), (uint8_t)(V10)

/*! Macro for Configuration Setting with twelve register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_12_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9, R10, M10, V10, R11, M11, V11)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU),12, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1) \
                                 , (uint8_t)((R2) >> 8), (uint8_t)((R2) & 0xFFU), (uint8_t)(M2), (uint8_t)(V2) \
                                 , (uint8_t)((R3) >> 8), (uint8_t)((R3) & 0xFFU), (uint8_t)(M3), (uint8_t)(V3) \
                                 , (uint8_t)((R4) >> 8), (uint8_t)((R4) & 0xFFU), (uint8_t)(M4), (uint8_t)(V4) \
                                 , (uint8_t)((R5) >> 8), (uint8_t)((R5) & 0xFFU), (uint8_t)(M5), (uint8_t)(V5) \
                                 , (uint8_t)((R6) >> 8), (uint8_t)((R6) & 0xFFU), (uint8_t)(M6), (uint8_t)(V6) \
                                 , (uint8_t)((R7) >> 8), (uint8_t)((R7) & 0xFFU), (uint8_t)(M7), (uint8_t)(V7) \
                                 , (uint8_t)((R8) >> 8), (uint8_t)((R8) & 0xFFU), (uint8_t)(M8), (uint8_t)(V8) \
                                 , (uint8_t)((R9) >> 8), (uint8_t)((R9) & 0xFFU), (uint8_t)(M9), (uint8_t)(V9) \
                                 , (uint8_t)((R10) >> 8), (uint8_t)((R10) & 0xFFU), (uint8_t)(M10), (uint8_t)(V10) \
                                 , (uint8_t)((R11) >> 8), (uint8_t)((R11) & 0xFFU), (uint8_t)(M11), (uint8_t)(V11)

/*! Macro for Configuration Setting with thirteen register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_13_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9, R10, M10, V10, R11, M11, V11, R12, M12, V12)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU),13, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1) \
                                 , (uint8_t)((R2) >> 8), (uint8_t)((R2) & 0xFFU), (uint8_t)(M2), (uint8_t)(V2) \
                                 , (uint8_t)((R3) >> 8), (uint8_t)((R3) & 0xFFU), (uint8_t)(M3), (uint8_t)(V3) \
                                 , (uint8_t)((R4) >> 8), (uint8_t)((R4) & 0xFFU), (uint8_t)(M4), (uint8_t)(V4) \
                                 , (uint8_t)((R5) >> 8), (uint8_t)((R5) & 0xFFU), (uint8_t)(M5), (uint8_t)(V5) \
                                 , (uint8_t)((R6) >> 8), (uint8_t)((R6) & 0xFFU), (uint8_t)(M6), (uint8_t)(V6) \
                                 , (uint8_t)((R7) >> 8), (uint8_t)((R7) & 0xFFU), (uint8_t)(M7), (uint8_t)(V7) \
                                 , (uint8_t)((R8) >> 8), (uint8_t)((R8) & 0xFFU), (uint8_t)(M8), (uint8_t)(V8) \
                                 , (uint8_t)((R9) >> 8), (uint8_t)((R9) & 0xFFU), (uint8_t)(M9), (uint8_t)(V9) \
                                 , (uint8_t)((R10) >> 8), (uint8_t)((R10) & 0xFFU), (uint8_t)(M10), (uint8_t)(V10) \
                                 , (uint8_t)((R11) >> 8), (uint8_t)((R11) & 0xFFU), (uint8_t)(M11), (uint8_t)(V11) \
                                 , (uint8_t)((R12) >> 8), (uint8_t)((R12) & 0xFFU), (uint8_t)(M12), (uint8_t)(V12)

/*! Macro for Configuration Setting with thirteen register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_14_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9, R10, M10, V10, R11, M11, V11, R12, M12, V12, R13, M13, V13, R14, M14, V14, R15, M15, V15)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU),14, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1) \
                                 , (uint8_t)((R2) >> 8), (uint8_t)((R2) & 0xFFU), (uint8_t)(M2), (uint8_t)(V2) \
                                 , (uint8_t)((R3) >> 8), (uint8_t)((R3) & 0xFFU), (uint8_t)(M3), (uint8_t)(V3) \
                                 , (uint8_t)((R4) >> 8), (uint8_t)((R4) & 0xFFU), (uint8_t)(M4), (uint8_t)(V4) \
                                 , (uint8_t)((R5) >> 8), (uint8_t)((R5) & 0xFFU), (uint8_t)(M5), (uint8_t)(V5) \
                                 , (uint8_t)((R6) >> 8), (uint8_t)((R6) & 0xFFU), (uint8_t)(M6), (uint8_t)(V6) \
                                 , (uint8_t)((R7) >> 8), (uint8_t)((R7) & 0xFFU), (uint8_t)(M7), (uint8_t)(V7) \
                                 , (uint8_t)((R8) >> 8), (uint8_t)((R8) & 0xFFU), (uint8_t)(M8), (uint8_t)(V8) \
                                 , (uint8_t)((R9) >> 8), (uint8_t)((R9) & 0xFFU), (uint8_t)(M9), (uint8_t)(V9) \
                                 , (uint8_t)((R10) >> 8), (uint8_t)((R10) & 0xFFU), (uint8_t)(M10), (uint8_t)(V10) \
                                 , (uint8_t)((R11) >> 8), (uint8_t)((R11) & 0xFFU), (uint8_t)(M11), (uint8_t)(V11) \
                                 , (uint8_t)((R12) >> 8), (uint8_t)((R12) & 0xFFU), (uint8_t)(M12), (uint8_t)(V12) \
                                 , (uint8_t)((R13) >> 8), (uint8_t)((R13) & 0xFFU), (uint8_t)(M13), (uint8_t)(V13)

/*! Macro for Configuration Setting with thirteen register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_15_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9, R10, M10, V10, R11, M11, V11, R12, M12, V12, R13, M13, V13, R14, M14, V14, R15, M15, V15)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU),15, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1) \
                                 , (uint8_t)((R2) >> 8), (uint8_t)((R2) & 0xFFU), (uint8_t)(M2), (uint8_t)(V2) \
                                 , (uint8_t)((R3) >> 8), (uint8_t)((R3) & 0xFFU), (uint8_t)(M3), (uint8_t)(V3) \
                                 , (uint8_t)((R4) >> 8), (uint8_t)((R4) & 0xFFU), (uint8_t)(M4), (uint8_t)(V4) \
                                 , (uint8_t)((R5) >> 8), (uint8_t)((R5) & 0xFFU), (uint8_t)(M5), (uint8_t)(V5) \
                                 , (uint8_t)((R6) >> 8), (uint8_t)((R6) & 0xFFU), (uint8_t)(M6), (uint8_t)(V6) \
                                 , (uint8_t)((R7) >> 8), (uint8_t)((R7) & 0xFFU), (uint8_t)(M7), (uint8_t)(V7) \
                                 , (uint8_t)((R8) >> 8), (uint8_t)((R8) & 0xFFU), (uint8_t)(M8), (uint8_t)(V8) \
                                 , (uint8_t)((R9) >> 8), (uint8_t)((R9) & 0xFFU), (uint8_t)(M9), (uint8_t)(V9) \
                                 , (uint8_t)((R10) >> 8), (uint8_t)((R10) & 0xFFU), (uint8_t)(M10), (uint8_t)(V10) \
                                 , (uint8_t)((R11) >> 8), (uint8_t)((R11) & 0xFFU), (uint8_t)(M11), (uint8_t)(V11) \
                                 , (uint8_t)((R12) >> 8), (uint8_t)((R12) & 0xFFU), (uint8_t)(M12), (uint8_t)(V12) \
                                 , (uint8_t)((R13) >> 8), (uint8_t)((R13) & 0xFFU), (uint8_t)(M13), (uint8_t)(V13) \
                                 , (uint8_t)((R14) >> 8), (uint8_t)((R14) & 0xFFU), (uint8_t)(M14), (uint8_t)(V14)

/*! Macro for Configuration Setting with thirteen register-mask-value sets:
 *  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_16_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9, R10, M10, V10, R11, M11, V11, R12, M12, V12, R13, M13, V13, R14, M14, V14, R15, M15, V15)  \
    (uint8_t)((MODE) >> 8), (uint8_t)((MODE) & 0xFFU),16, (uint8_t)((R0) >> 8), (uint8_t)((R0) & 0xFFU), (uint8_t)(M0), (uint8_t)(V0) \
                                 , (uint8_t)((R1) >> 8), (uint8_t)((R1) & 0xFFU), (uint8_t)(M1), (uint8_t)(V1) \
                                 , (uint8_t)((R2) >> 8), (uint8_t)((R2) & 0xFFU), (uint8_t)(M2), (uint8_t)(V2) \
                                 , (uint8_t)((R3) >> 8), (uint8_t)((R3) & 0xFFU), (uint8_t)(M3), (uint8_t)(V3) \
                                 , (uint8_t)((R4) >> 8), (uint8_t)((R4) & 0xFFU), (uint8_t)(M4), (uint8_t)(V4) \
                                 , (uint8_t)((R5) >> 8), (uint8_t)((R5) & 0xFFU), (uint8_t)(M5), (uint8_t)(V5) \
                                 , (uint8_t)((R6) >> 8), (uint8_t)((R6) & 0xFFU), (uint8_t)(M6), (uint8_t)(V6) \
                                 , (uint8_t)((R7) >> 8), (uint8_t)((R7) & 0xFFU), (uint8_t)(M7), (uint8_t)(V7) \
                                 , (uint8_t)((R8) >> 8), (uint8_t)((R8) & 0xFFU), (uint8_t)(M8), (uint8_t)(V8) \
                                 , (uint8_t)((R9) >> 8), (uint8_t)((R9) & 0xFFU), (uint8_t)(M9), (uint8_t)(V9) \
                                 , (uint8_t)((R10) >> 8), (uint8_t)((R10) & 0xFFU), (uint8_t)(M10), (uint8_t)(V10) \
                                 , (uint8_t)((R11) >> 8), (uint8_t)((R11) & 0xFFU), (uint8_t)(M11), (uint8_t)(V11) \
                                 , (uint8_t)((R12) >> 8), (uint8_t)((R12) & 0xFFU), (uint8_t)(M12), (uint8_t)(V12) \
                                 , (uint8_t)((R13) >> 8), (uint8_t)((R13) & 0xFFU), (uint8_t)(M13), (uint8_t)(V13) \
                                 , (uint8_t)((R14) >> 8), (uint8_t)((R14) & 0xFFU), (uint8_t)(M14), (uint8_t)(V14) \
                                 , (uint8_t)((R15) >> 8), (uint8_t)((R15) & 0xFFU), (uint8_t)(M15), (uint8_t)(V15)


/*
 ******************************************************************************
 * GLOBAL DATA TYPES
 ******************************************************************************
 */
/*  PRQA S 3406 1 # MISRA 8.6 - Externally generated table included by the library */   /*  PRQA S 1514 1 # MISRA 8.9 - Externally generated table included by the library */
const uint8_t rfalAnalogConfigDefaultSettings[] = {

  /****** Default Analog Configuration for Chip-Specific Reset ******/
  MODE_ENTRY_16_REG((RFAL_ANALOG_CONFIG_TECH_CHIP | RFAL_ANALOG_CONFIG_CHIP_INIT)
                    , ST25R3916_REG_IO_CONF1, (ST25R3916_REG_IO_CONF1_out_cl_mask | ST25R3916_REG_IO_CONF1_lf_clk_off), 0x07                               /* Disable MCU_CLK */
                    , ST25R3916_REG_IO_CONF2, (ST25R3916_REG_IO_CONF2_miso_pd1 | ST25R3916_REG_IO_CONF2_miso_pd2), 0x18                                    /* SPI Pull downs */
                    /* AAT restored to enabled (5 Aug 2026) — the AAT-off experiment neither
                     * fixed nor broke wake-up detection (SW Tag Detect and genuine hardware
                     * wake both still saw amplitude pegged at 255 with AAT off). Fable-agent
                     * research against the actual ST25R3916B datasheet (DS13541 Rev 11) found
                     * the real root cause instead: Measure Amplitude self-drives the TX field
                     * (§4.4.8 p65), and this ElecHouse module runs the chip at 5V (VDD_RF
                     * ~4.5V measured, matches datasheet's 5V-mode regulator behavior, §4.4.10
                     * p65) — driving RFI well past the documented 3 Vpp max (which should read
                     * only 0xE6/230, not our observed 255/0xFF ≈ ≥3.3 Vpp). AAT itself is a
                     * red herring on this specific module: the ElecHouse schematic has no
                     * varactors, so AAT_A/B route nowhere — it's copy-inherited boilerplate,
                     * confirmed harmless either way. The real fix is reducing TX_DRIVER.d_res
                     * (RFO driver resistance) specifically during wake-up's own measurement —
                     * see the CHIP_WAKEUP_ON entry below. */
                    , ST25R3916_REG_IO_CONF2,  ST25R3916_REG_IO_CONF2_aat_en, ST25R3916_REG_IO_CONF2_aat_en                                                /* Enable AAT (restored) */
                    , ST25R3916_REG_TX_DRIVER, ST25R3916_REG_TX_DRIVER_d_res_mask, 0x00                                                                    /* Set RFO resistance Active Tx */
                    , ST25R3916_REG_RES_AM_MOD, 0xFF, 0x80                                                                                          /* Use minimum non-overlap */
                    , ST25R3916_REG_FIELD_THRESHOLD_ACTV,   ST25R3916_REG_FIELD_THRESHOLD_ACTV_trg_mask, ST25R3916_REG_FIELD_THRESHOLD_ACTV_trg_105mV      /* Lower activation threshold (higher than deactivation)*/
                    , ST25R3916_REG_FIELD_THRESHOLD_ACTV,   ST25R3916_REG_FIELD_THRESHOLD_ACTV_rfe_mask, ST25R3916_REG_FIELD_THRESHOLD_ACTV_rfe_105mV      /* Lower activation threshold (higher than deactivation)*/
                    , ST25R3916_REG_FIELD_THRESHOLD_DEACTV, ST25R3916_REG_FIELD_THRESHOLD_DEACTV_trg_mask, ST25R3916_REG_FIELD_THRESHOLD_DEACTV_trg_75mV   /* Lower deactivation threshold */
                    , ST25R3916_REG_FIELD_THRESHOLD_DEACTV, ST25R3916_REG_FIELD_THRESHOLD_DEACTV_rfe_mask, ST25R3916_REG_FIELD_THRESHOLD_DEACTV_rfe_75mV   /* Lower deactivation threshold */
                    , ST25R3916_REG_AUX_MOD, ST25R3916_REG_AUX_MOD_lm_ext, 0x00                                                                            /* Disable Internal Modulation */
                    , ST25R3916_REG_AUX_MOD, ST25R3916_REG_AUX_MOD_lm_dri, ST25R3916_REG_AUX_MOD_lm_dri                                                    /* Use external Load Modulation */
                    , ST25R3916_REG_PASSIVE_TARGET, ST25R3916_REG_PASSIVE_TARGET_fdel_mask, (5U << ST25R3916_REG_PASSIVE_TARGET_fdel_shift)                /* Adjust the FDT to be aligned with the bitgrid  */
                    , ST25R3916_REG_PT_MOD, (ST25R3916_REG_PT_MOD_ptm_res_mask | ST25R3916_REG_PT_MOD_pt_res_mask), 0x51                                   /* Reduce RFO resistance in Modulated state */
                    , ST25R3916_REG_EMD_SUP_CONF, ST25R3916_REG_EMD_SUP_CONF_rx_start_emv, ST25R3916_REG_EMD_SUP_CONF_rx_start_emv_on                      /* Enable start on first 4 bits */
                    , ST25R3916_REG_ANT_TUNE_A, 0xFF, 0x80                                                                                                 /* Set Antenna Tuning (Poller): ANTL */
                    , ST25R3916_REG_ANT_TUNE_B, 0xFF, 0x40                                                                                                 /* Set Antenna Tuning (Poller): ANTL */
                   )

  /****** Default Analog Configuration for Chip-Specific Poll Common ******/
  , MODE_ENTRY_9_REG((RFAL_ANALOG_CONFIG_TECH_CHIP | RFAL_ANALOG_CONFIG_CHIP_POLL_COMMON)
                     , ST25R3916_REG_MODE, ST25R3916_REG_MODE_tr_am, ST25R3916_REG_MODE_tr_am_am                                             /* Use AM modulation */
                     , ST25R3916_REG_TX_DRIVER, ST25R3916_REG_TX_DRIVER_am_mod_mask, ST25R3916_REG_TX_DRIVER_am_mod_12percent                /* Set Modulation index */
                     , ST25R3916_REG_AUX_MOD, (ST25R3916_REG_AUX_MOD_am_mode | ST25R3916_REG_AUX_MOD_res_am), 0x00                           /* Use AM via regulator */
                     , ST25R3916_REG_ANT_TUNE_A, 0xFF, 0x80                                                                                  /* Set Antenna Tuning (Poller): ANTL */
                     , ST25R3916_REG_ANT_TUNE_B, 0xFF, 0x40                                                                                  /* Set Antenna Tuning (Poller): ANTL */
                     , ST25R3916_REG_OVERSHOOT_CONF1,  0xFF, 0x00                                                                            /* Disable Overshoot Protection  */
                     , ST25R3916_REG_OVERSHOOT_CONF2,  0xFF, 0x00                                                                            /* Disable Overshoot Protection  */
                     , ST25R3916_REG_UNDERSHOOT_CONF1, 0xFF, 0x00                                                                            /* Disable Undershoot Protection */
                     , ST25R3916_REG_UNDERSHOOT_CONF2, 0xFF, 0x00                                                                            /* Disable Undershoot Protection */
                    )

  /****** Default Analog Configuration for Wake-up On ******/
  /* MISSING from this board's own table originally (5 Aug 2026 bench finding,
   * OffGridGate): rfalWakeUpModeStart() unconditionally calls
   * rfalSetAnalogConfig(CHIP_WAKEUP_ON), but this ElecHouse-derived table had zero
   * entries for it — rfalSetAnalogConfig hits RFAL_ANALOG_CONFIG_LUT_NOT_FOUND on
   * the first search and silently returns RFAL_ERR_NONE without applying anything
   * (rfal_analogConfig.c). The AWS-disable entry (byte-exact from ST's own
   * reference table) alone did NOT fix detection — raw amplitude stayed pegged at
   * 255 regardless.
   *
   * REAL FIX (5 Aug 2026, later same day, Fable-agent research against the actual
   * ST25R3916B datasheet DS13541 Rev 11): Measure Amplitude self-drives the TX
   * field for its measurement (§4.4.8 p65), and this ElecHouse module runs the
   * chip at 5V (not 3.3V as this project's own docs assumed elsewhere — worth
   * correcting), so the field this measurement briefly generates overdrives the
   * amplitude detector's RFI input past its documented 3 Vpp maximum (which
   * should read only 0xE6/230 — our observed 255/0xFF means ≥~3.3 Vpp). Both SW
   * Tag Detect AND genuine hardware wake-up failed identically because neither
   * touches TX drive strength — AWS just shapes the modulation envelope, it
   * doesn't reduce overall field power.
   *
   * d_res (TX_DRIVER RFO driver resistance, 4-bit field, ST25R3916_REG_TX_DRIVER)
   * is what the datasheet explicitly names for this (§4.2.4 p31: measurement
   * power "can be reduced by changing the RFO driver resistance"). CHIP_INIT sets
   * d_res=0x0 (lowest resistance = strongest drive) for normal active polling —
   * correct there, wrong for wake-up's own measurement. Default 13 picked per the
   * local datasheet's own Table 78 (DS13541 Rev 11, p111): d_res=13 -> 36.6x
   * normalized driver resistance, matching an ST community 3916B thread's
   * reported reliable-wake-up value exactly (confirmed against the table
   * directly, not the forum post's own paraphrase, which read ambiguously as
   * "36.6%" — d_res=14 would be 51.2x, a different, unverified step) — bench-
   * verified 5 Aug 2026: the first real card tap under this exact value produced
   * a genuine hardware WAM interrupt (raw=39) and a full ALLOW.
   *
   * Exposed as CONFIG_ST25R3916_WAKEUP_D_RES (Kconfig.reader — this ElecHouse
   * bench module is NOT the final product antenna, and repeat-tap testing
   * showed some real taps waking correctly but not completing full activation,
   * suggesting the wake-detection range and full-activation range may need
   * independent per-antenna tuning) — tune via prj.conf, not by editing this
   * table, using reader.c's live WAM/WUM diagnostic logging as feedback. */
  , MODE_ENTRY_2_REG((RFAL_ANALOG_CONFIG_TECH_CHIP | RFAL_ANALOG_CONFIG_CHIP_WAKEUP_ON)
                     , ST25R3916_REG_AUX_MOD, ST25R3916_REG_AUX_MOD_rgs_am, 0x00                                                            /* Disable AWS during WU  */
                     , ST25R3916_REG_TX_DRIVER, ST25R3916_REG_TX_DRIVER_d_res_mask, CONFIG_ST25R3916_WAKEUP_D_RES                          /* Reduce field power for WU measurement — tune via CONFIG_ST25R3916_WAKEUP_D_RES */
                    )

  /****** Default Analog Configuration for Wake-up Off ******/
  , MODE_ENTRY_2_REG((RFAL_ANALOG_CONFIG_TECH_CHIP | RFAL_ANALOG_CONFIG_CHIP_WAKEUP_OFF)
                     , ST25R3916_REG_AUX_MOD, ST25R3916_REG_AUX_MOD_rgs_am, ST25R3916_REG_AUX_MOD_rgs_am                                    /* Re-enable AWS after WU */
                     , ST25R3916_REG_TX_DRIVER, ST25R3916_REG_TX_DRIVER_d_res_mask, 0x00                                                    /* Restore full drive for active polling */
                    )

  /****** Default Analog Configuration for Poll NFC-A Rx Common ******/
  , MODE_ENTRY_1_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCA | RFAL_ANALOG_CONFIG_BITRATE_COMMON | RFAL_ANALOG_CONFIG_RX)
                     , ST25R3916_REG_AUX,  ST25R3916_REG_AUX_dis_corr, ST25R3916_REG_AUX_dis_corr_correlator                                   /* Use Correlator Receiver */
                    )

  /****** Default Analog Configuration for Poll NFC-A Tx 106 ******/
  , MODE_ENTRY_5_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCA | RFAL_ANALOG_CONFIG_BITRATE_106 | RFAL_ANALOG_CONFIG_TX)
                     , ST25R3916_REG_MODE, ST25R3916_REG_MODE_tr_am, ST25R3916_REG_MODE_tr_am_ook                                              /* Use OOK */
                     , ST25R3916_REG_OVERSHOOT_CONF1,  0xFF, 0x40                                                                              /* Set default Overshoot Protection */
                     , ST25R3916_REG_OVERSHOOT_CONF2,  0xFF, 0x03                                                                              /* Set default Overshoot Protection */
                     , ST25R3916_REG_UNDERSHOOT_CONF1, 0xFF, 0x40                                                                              /* Set default Undershoot Protection */
                     , ST25R3916_REG_UNDERSHOOT_CONF2, 0xFF, 0x03                                                                              /* Set default Undershoot Protection */
                    )

  /****** Default Analog Configuration for Poll NFC-A Rx 106 ******/
  , MODE_ENTRY_6_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCA | RFAL_ANALOG_CONFIG_BITRATE_106 | RFAL_ANALOG_CONFIG_RX)
                     , ST25R3916_REG_RX_CONF1,   0xFF, 0x08
                     , ST25R3916_REG_RX_CONF2,   0xFF, 0x2D
                     , ST25R3916_REG_RX_CONF3,   0xFF, 0x00
                     , ST25R3916_REG_RX_CONF4,   0xFF, 0x00
                     , ST25R3916_REG_CORR_CONF1, 0xFF, 0x51
                     , ST25R3916_REG_CORR_CONF2, 0xFF, 0x00
                    )

  /****** Default Analog Configuration for Poll NFC-A Tx 212 ******/
  , MODE_ENTRY_7_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCA | RFAL_ANALOG_CONFIG_BITRATE_212 | RFAL_ANALOG_CONFIG_TX)
                     , ST25R3916_REG_MODE, ST25R3916_REG_MODE_tr_am, ST25R3916_REG_MODE_tr_am_am                                               /* Use AM modulation */
                     , ST25R3916_REG_AUX_MOD, (ST25R3916_REG_AUX_MOD_am_mode | ST25R3916_REG_AUX_MOD_res_am), 0x88                             /* Use Resistive AM */
                     , ST25R3916_REG_RES_AM_MOD, ST25R3916_REG_RES_AM_MOD_md_res_mask, 0x7F                                      /* Set Resistive modulation */
                     , ST25R3916_REG_OVERSHOOT_CONF1,  0xFF, 0x40                                                                              /* Set default Overshoot Protection  */
                     , ST25R3916_REG_OVERSHOOT_CONF2,  0xFF, 0x03                                                                              /* Set default Overshoot Protection  */
                     , ST25R3916_REG_UNDERSHOOT_CONF1, 0xFF, 0x40                                                                              /* Set default Undershoot Protection */
                     , ST25R3916_REG_UNDERSHOOT_CONF2, 0xFF, 0x03                                                                              /* Set default Undershoot Protection */
                    )

  /****** Default Analog Configuration for Poll NFC-A Rx 212 ******/
  , MODE_ENTRY_6_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCA | RFAL_ANALOG_CONFIG_BITRATE_212 | RFAL_ANALOG_CONFIG_RX)
                     , ST25R3916_REG_RX_CONF1,   0xFF, 0x02
                     , ST25R3916_REG_RX_CONF2,   0xFF, 0x3D
                     , ST25R3916_REG_RX_CONF3,   0xFF, 0x00
                     , ST25R3916_REG_RX_CONF4,   0xFF, 0x00
                     , ST25R3916_REG_CORR_CONF1, 0xFF, 0x14
                     , ST25R3916_REG_CORR_CONF2, 0xFF, 0x00
                    )

  /****** Default Analog Configuration for Poll NFC-A Tx 424 ******/
  , MODE_ENTRY_7_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCA | RFAL_ANALOG_CONFIG_BITRATE_424 | RFAL_ANALOG_CONFIG_TX)
                     , ST25R3916_REG_MODE, ST25R3916_REG_MODE_tr_am, ST25R3916_REG_MODE_tr_am_am                                               /* Use AM modulation */
                     , ST25R3916_REG_AUX_MOD, (ST25R3916_REG_AUX_MOD_am_mode | ST25R3916_REG_AUX_MOD_res_am), 0x88                             /* Use Resistive AM */
                     , ST25R3916_REG_RES_AM_MOD, ST25R3916_REG_RES_AM_MOD_md_res_mask, 0x7F                                                    /* Set Resistive modulation */
                     , ST25R3916_REG_OVERSHOOT_CONF1,  0xFF, 0x40                                                                              /* Set default Overshoot Protection  */
                     , ST25R3916_REG_OVERSHOOT_CONF2,  0xFF, 0x03                                                                              /* Set default Overshoot Protection  */
                     , ST25R3916_REG_UNDERSHOOT_CONF1, 0xFF, 0x40                                                                              /* Set default Undershoot Protection */
                     , ST25R3916_REG_UNDERSHOOT_CONF2, 0xFF, 0x03                                                                              /* Set default Undershoot Protection */
                    )

  /****** Default Analog Configuration for Poll NFC-A Rx 424 ******/
  , MODE_ENTRY_6_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCA | RFAL_ANALOG_CONFIG_BITRATE_424 | RFAL_ANALOG_CONFIG_RX)
                     , ST25R3916_REG_RX_CONF1,   0xFF, 0x42
                     , ST25R3916_REG_RX_CONF2,   0xFF, 0x3D
                     , ST25R3916_REG_RX_CONF3,   0xFF, 0x00
                     , ST25R3916_REG_RX_CONF4,   0xFF, 0x00
                     , ST25R3916_REG_CORR_CONF1, 0xFF, 0x54
                     , ST25R3916_REG_CORR_CONF2, 0xFF, 0x00
                    )

  /****** Default Analog Configuration for Poll NFC-A Tx 848 ******/
  , MODE_ENTRY_7_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCA | RFAL_ANALOG_CONFIG_BITRATE_848 | RFAL_ANALOG_CONFIG_TX)
                     , ST25R3916_REG_MODE, ST25R3916_REG_MODE_tr_am, ST25R3916_REG_MODE_tr_am_am                                               /* Use AM modulation */
                     , ST25R3916_REG_TX_DRIVER, ST25R3916_REG_TX_DRIVER_am_mod_mask, ST25R3916_REG_TX_DRIVER_am_mod_40percent                  /* Set Modulation index */
                     , ST25R3916_REG_AUX_MOD, (ST25R3916_REG_AUX_MOD_am_mode | ST25R3916_REG_AUX_MOD_res_am), 0x00                             /* Use AM via regulator */
                     , ST25R3916_REG_OVERSHOOT_CONF1,  0xFF, 0x00                                                                              /* Disable Overshoot Protection  */
                     , ST25R3916_REG_OVERSHOOT_CONF2,  0xFF, 0x00                                                                              /* Disable Overshoot Protection  */
                     , ST25R3916_REG_UNDERSHOOT_CONF1, 0xFF, 0x00                                                                              /* Disable Undershoot Protection */
                     , ST25R3916_REG_UNDERSHOOT_CONF2, 0xFF, 0x00                                                                              /* Disable Undershoot Protection */
                    )

  /****** Default Analog Configuration for Poll NFC-A Rx 848 ******/
  , MODE_ENTRY_6_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCA | RFAL_ANALOG_CONFIG_BITRATE_848 | RFAL_ANALOG_CONFIG_RX)
                     , ST25R3916_REG_RX_CONF1,   0xFF, 0x42
                     , ST25R3916_REG_RX_CONF2,   0xFF, 0x3D
                     , ST25R3916_REG_RX_CONF3,   0xFF, 0x00
                     , ST25R3916_REG_RX_CONF4,   0xFF, 0x00
                     , ST25R3916_REG_CORR_CONF1, 0xFF, 0x44
                     , ST25R3916_REG_CORR_CONF2, 0xFF, 0x00
                    )

  /****** Default Analog Configuration for Poll NFC-A Anticolision setting ******/
  , MODE_ENTRY_1_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCA | RFAL_ANALOG_CONFIG_BITRATE_COMMON | RFAL_ANALOG_CONFIG_ANTICOL)
                     , ST25R3916_REG_CORR_CONF1, ST25R3916_REG_CORR_CONF1_corr_s6, 0x00                                                            /* Set collision detection level different from data */
                    )

  /****** Default Analog Configuration for Poll NFC-B Rx Common ******/
  , MODE_ENTRY_1_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCB | RFAL_ANALOG_CONFIG_BITRATE_COMMON | RFAL_ANALOG_CONFIG_RX)
                     , ST25R3916_REG_AUX,  ST25R3916_REG_AUX_dis_corr, ST25R3916_REG_AUX_dis_corr_correlator                                 /* Use Correlator Receiver */
                    )

  /****** Default Analog Configuration for Poll NFC-B Rx 106 ******/
  , MODE_ENTRY_6_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCB | RFAL_ANALOG_CONFIG_BITRATE_106 | RFAL_ANALOG_CONFIG_RX)
                     , ST25R3916_REG_RX_CONF1,   0xFF, 0x04
                     , ST25R3916_REG_RX_CONF2,   0xFF, 0x3D
                     , ST25R3916_REG_RX_CONF3,   0xFF, 0x00
                     , ST25R3916_REG_RX_CONF4,   0xFF, 0x00
                     , ST25R3916_REG_CORR_CONF1, 0xFF, 0x1B
                     , ST25R3916_REG_CORR_CONF2, 0xFF, 0x00
                    )

  /****** Default Analog Configuration for Poll NFC-B Rx 212 ******/
  , MODE_ENTRY_6_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCB | RFAL_ANALOG_CONFIG_BITRATE_212 | RFAL_ANALOG_CONFIG_RX)
                     , ST25R3916_REG_RX_CONF1,   0xFF, 0x02
                     , ST25R3916_REG_RX_CONF2,   0xFF, 0x3D
                     , ST25R3916_REG_RX_CONF3,   0xFF, 0x00
                     , ST25R3916_REG_RX_CONF4,   0xFF, 0x00
                     , ST25R3916_REG_CORR_CONF1, 0xFF, 0x14
                     , ST25R3916_REG_CORR_CONF2, 0xFF, 0x00
                    )

  /****** Default Analog Configuration for Poll NFC-B Rx 424 ******/
  , MODE_ENTRY_6_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCB | RFAL_ANALOG_CONFIG_BITRATE_424 | RFAL_ANALOG_CONFIG_RX)
                     , ST25R3916_REG_RX_CONF1, 0xFF, 0x42
                     , ST25R3916_REG_RX_CONF2, 0xFF, 0x3D
                     , ST25R3916_REG_RX_CONF3, 0xFF, 0x00
                     , ST25R3916_REG_RX_CONF4, 0xFF, 0x00
                     , ST25R3916_REG_CORR_CONF1, 0xFF, 0x54
                     , ST25R3916_REG_CORR_CONF2, 0xFF, 0x00
                    )

  /****** Default Analog Configuration for Poll NFC-B Rx 848 ******/
  , MODE_ENTRY_6_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCB | RFAL_ANALOG_CONFIG_BITRATE_848 | RFAL_ANALOG_CONFIG_RX)
                     , ST25R3916_REG_RX_CONF1,   0xFF, 0x42
                     , ST25R3916_REG_RX_CONF2,   0xFF, 0x3D
                     , ST25R3916_REG_RX_CONF3,   0xFF, 0x00
                     , ST25R3916_REG_RX_CONF4,   0xFF, 0x00
                     , ST25R3916_REG_CORR_CONF1, 0xFF, 0x44
                     , ST25R3916_REG_CORR_CONF2, 0xFF, 0x00
                    )

  /****** Default Analog Configuration for Poll NFC-F Rx Common ******/
  , MODE_ENTRY_7_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCF | RFAL_ANALOG_CONFIG_BITRATE_COMMON | RFAL_ANALOG_CONFIG_RX)
                     , ST25R3916_REG_AUX,  ST25R3916_REG_AUX_dis_corr, ST25R3916_REG_AUX_dis_corr_correlator                                   /* Use Correlator Receiver */
                     , ST25R3916_REG_RX_CONF1,   0xFF, 0x13
                     , ST25R3916_REG_RX_CONF2,   0xFF, 0x3D
                     , ST25R3916_REG_RX_CONF3,   0xFF, 0x00
                     , ST25R3916_REG_RX_CONF4,   0xFF, 0x00
                     , ST25R3916_REG_CORR_CONF1, 0xFF, 0x54
                     , ST25R3916_REG_CORR_CONF2, 0xFF, 0x00
                    )

  /****** Default Analog Configuration for Poll NFC-V Rx Common ******/
  , MODE_ENTRY_7_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_NFCV | RFAL_ANALOG_CONFIG_BITRATE_COMMON | RFAL_ANALOG_CONFIG_RX)
                     , ST25R3916_REG_AUX,  ST25R3916_REG_AUX_dis_corr, ST25R3916_REG_AUX_dis_corr_correlator                                   /* Use Correlator Receiver */
                     , ST25R3916_REG_RX_CONF1,   0xFF, 0x13
                     , ST25R3916_REG_RX_CONF2,   0xFF, 0x2D
                     , ST25R3916_REG_RX_CONF3,   0xFF, 0x00
                     , ST25R3916_REG_RX_CONF4,   0xFF, 0x00
                     , ST25R3916_REG_CORR_CONF1, 0xFF, 0x13
                     , ST25R3916_REG_CORR_CONF2, 0xFF, 0x01
                    )

  /****** Default Analog Configuration for Poll AP2P Rx Common ******/
  , MODE_ENTRY_3_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_AP2P | RFAL_ANALOG_CONFIG_BITRATE_COMMON | RFAL_ANALOG_CONFIG_RX)
                     , ST25R3916_REG_RX_CONF1, ST25R3916_REG_RX_CONF1_lp_mask, ST25R3916_REG_RX_CONF1_lp_1200khz                               /* Set Rx filter configuration */
                     , ST25R3916_REG_RX_CONF1, ST25R3916_REG_RX_CONF1_hz_mask, ST25R3916_REG_RX_CONF1_hz_12_200khz                             /* Set Rx filter configuration */
                     , ST25R3916_REG_RX_CONF2, ST25R3916_REG_RX_CONF2_amd_sel, ST25R3916_REG_RX_CONF2_amd_sel_mixer                            /* AM demodulator: mixer */
                    )

  /****** Default Analog Configuration for Poll AP2P Tx 106 ******/
  , MODE_ENTRY_5_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_AP2P | RFAL_ANALOG_CONFIG_BITRATE_106 | RFAL_ANALOG_CONFIG_TX)
                     , ST25R3916_REG_MODE, ST25R3916_REG_MODE_tr_am, ST25R3916_REG_MODE_tr_am_ook                                              /* Use OOK modulation */
                     , ST25R3916_REG_OVERSHOOT_CONF1,  0xFF, 0x40                                                                              /* Set default Overshoot Protection  */
                     , ST25R3916_REG_OVERSHOOT_CONF2,  0xFF, 0x03                                                                              /* Set default Overshoot Protection  */
                     , ST25R3916_REG_UNDERSHOOT_CONF1, 0xFF, 0x40                                                                              /* Set default Undershoot Protection */
                     , ST25R3916_REG_UNDERSHOOT_CONF2, 0xFF, 0x03                                                                              /* Set default Undershoot Protection */
                    )

  /****** Default Analog Configuration for Poll AP2P Tx 212 ******/
  , MODE_ENTRY_1_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_AP2P | RFAL_ANALOG_CONFIG_BITRATE_212 | RFAL_ANALOG_CONFIG_TX)
                     , ST25R3916_REG_MODE, ST25R3916_REG_MODE_tr_am, ST25R3916_REG_MODE_tr_am_am                                               /* Use AM modulation */
                    )

  /****** Default Analog Configuration for Poll AP2P Tx 424 ******/
  , MODE_ENTRY_1_REG((RFAL_ANALOG_CONFIG_POLL | RFAL_ANALOG_CONFIG_TECH_AP2P | RFAL_ANALOG_CONFIG_BITRATE_424 | RFAL_ANALOG_CONFIG_TX)
                     , ST25R3916_REG_MODE, ST25R3916_REG_MODE_tr_am, ST25R3916_REG_MODE_tr_am_am                                               /* Use AM modulation */
                    )


  /****** Default Analog Configuration for Chip-Specific Listen On ******/
  , MODE_ENTRY_9_REG((RFAL_ANALOG_CONFIG_TECH_CHIP | RFAL_ANALOG_CONFIG_CHIP_LISTEN_ON)
                     , ST25R3916_REG_ANT_TUNE_A, 0xFF, 0x00                                                                                    /* Set Antenna Tuning (Listener): ANTL */
                     , ST25R3916_REG_ANT_TUNE_B, 0xFF, 0xE0                                                                                    /* Set Antenna Tuning (Listener): ANTL */
                     , ST25R3916_REG_RX_CONF1, ST25R3916_REG_RX_CONF1_lp_mask, ST25R3916_REG_RX_CONF1_lp_1200khz                               /* Set Rx filter configuration */
                     , ST25R3916_REG_RX_CONF1, ST25R3916_REG_RX_CONF1_hz_mask, ST25R3916_REG_RX_CONF1_hz_12_200khz                             /* Set Rx filter configuration */
                     , ST25R3916_REG_RX_CONF2, ST25R3916_REG_RX_CONF2_amd_sel, ST25R3916_REG_RX_CONF2_amd_sel_mixer                            /* AM demodulator: mixer */
                     , ST25R3916_REG_OVERSHOOT_CONF1,  0xFF, 0x00                                                                              /* Disable Overshoot Protection  */
                     , ST25R3916_REG_OVERSHOOT_CONF2,  0xFF, 0x00                                                                              /* Disable Overshoot Protection  */
                     , ST25R3916_REG_UNDERSHOOT_CONF1, 0xFF, 0x00                                                                              /* Disable Undershoot Protection */
                     , ST25R3916_REG_UNDERSHOOT_CONF2, 0xFF, 0x00                                                                              /* Disable Undershoot Protection */
                    )


  /****** Default Analog Configuration for Listen AP2P Tx Common ******/
  , MODE_ENTRY_7_REG((RFAL_ANALOG_CONFIG_LISTEN | RFAL_ANALOG_CONFIG_TECH_AP2P | RFAL_ANALOG_CONFIG_BITRATE_COMMON | RFAL_ANALOG_CONFIG_TX)
                     , ST25R3916_REG_ANT_TUNE_A, 0xFF, 0x80                                                                                    /* Set Antenna Tuning (Poller): ANTL */
                     , ST25R3916_REG_ANT_TUNE_B, 0xFF, 0x40                                                                                    /* Set Antenna Tuning (Poller): ANTL */
                     , ST25R3916_REG_TX_DRIVER, ST25R3916_REG_TX_DRIVER_am_mod_mask, ST25R3916_REG_TX_DRIVER_am_mod_12percent                  /* Set Modulation index */
                     , ST25R3916_REG_OVERSHOOT_CONF1,  0xFF, 0x00                                                                              /* Disable Overshoot Protection  */
                     , ST25R3916_REG_OVERSHOOT_CONF2,  0xFF, 0x00                                                                              /* Disable Overshoot Protection  */
                     , ST25R3916_REG_UNDERSHOOT_CONF1, 0xFF, 0x00                                                                              /* Disable Undershoot Protection */
                     , ST25R3916_REG_UNDERSHOOT_CONF2, 0xFF, 0x00                                                                              /* Disable Undershoot Protection */
                    )


  /****** Default Analog Configuration for Listen AP2P Rx Common ******/
  , MODE_ENTRY_3_REG((RFAL_ANALOG_CONFIG_LISTEN | RFAL_ANALOG_CONFIG_TECH_AP2P | RFAL_ANALOG_CONFIG_BITRATE_COMMON | RFAL_ANALOG_CONFIG_RX)
                     , ST25R3916_REG_RX_CONF1, ST25R3916_REG_RX_CONF1_lp_mask, ST25R3916_REG_RX_CONF1_lp_1200khz                               /* Set Rx filter configuration */
                     , ST25R3916_REG_RX_CONF1, ST25R3916_REG_RX_CONF1_hz_mask, ST25R3916_REG_RX_CONF1_hz_12_200khz                             /* Set Rx filter configuration */
                     , ST25R3916_REG_RX_CONF2, ST25R3916_REG_RX_CONF2_amd_sel, ST25R3916_REG_RX_CONF2_amd_sel_mixer                            /* AM demodulator: mixer */
                    )

  /****** Default Analog Configuration for Listen AP2P Tx 106 ******/
  , MODE_ENTRY_5_REG((RFAL_ANALOG_CONFIG_LISTEN | RFAL_ANALOG_CONFIG_TECH_AP2P | RFAL_ANALOG_CONFIG_BITRATE_106 | RFAL_ANALOG_CONFIG_TX)
                     , ST25R3916_REG_MODE, ST25R3916_REG_MODE_tr_am, ST25R3916_REG_MODE_tr_am_ook                                              /* Use OOK modulation */
                     , ST25R3916_REG_OVERSHOOT_CONF1,  0xFF, 0x40                                                                              /* Set default Overshoot Protection  */
                     , ST25R3916_REG_OVERSHOOT_CONF2,  0xFF, 0x03                                                                              /* Set default Overshoot Protection  */
                     , ST25R3916_REG_UNDERSHOOT_CONF1, 0xFF, 0x40                                                                              /* Set default Undershoot Protection */
                     , ST25R3916_REG_UNDERSHOOT_CONF2, 0xFF, 0x03                                                                              /* Set default Undershoot Protection */
                    )

  /****** Default Analog Configuration for Listen AP2P Tx 212 ******/
  , MODE_ENTRY_1_REG((RFAL_ANALOG_CONFIG_LISTEN | RFAL_ANALOG_CONFIG_TECH_AP2P | RFAL_ANALOG_CONFIG_BITRATE_212 | RFAL_ANALOG_CONFIG_TX)
                     , ST25R3916_REG_MODE, ST25R3916_REG_MODE_tr_am, ST25R3916_REG_MODE_tr_am_am                                                /* Use AM modulation */
                    )

  /****** Default Analog Configuration for Listen AP2P Tx 424 ******/
  , MODE_ENTRY_1_REG((RFAL_ANALOG_CONFIG_LISTEN | RFAL_ANALOG_CONFIG_TECH_AP2P | RFAL_ANALOG_CONFIG_BITRATE_424 | RFAL_ANALOG_CONFIG_TX)
                     , ST25R3916_REG_MODE, ST25R3916_REG_MODE_tr_am, ST25R3916_REG_MODE_tr_am_am                                                /* Use AM modulation */
                    )

};

#endif /* ST25R3916_ANALOGCONFIG_H */
