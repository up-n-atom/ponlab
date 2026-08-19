/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2016-2020 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2, as published
 * by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. 
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

//-----------------------------------------------------------------------------
// LSD Generator
//-----------------------------------------------------------------------------
// Perl Package        : LSD::generator::targetC (v1.1)
// LSD Source          : /home/l1502/lgm_chip/v_leichuan.priv.v-dfv.lgm_chip.leichuan/ipg_lsd/lsd_sys/source/xml/reg_files/load_spreader.xml
// Register File Name  : CBM_LS
// Register File Title : Load Spreader Register Description
// Register Width      : 32
// Note                : Doxygen compliant comments
//-----------------------------------------------------------------------------

#ifndef _CBM_LS_H
#define _CBM_LS_H

//! \defgroup CBM_LS Register File CBM_LS - Load Spreader Register Description
//! @{

//! Base Address of CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_MODULE_BASE 0xE6660000u
//! Base Address of CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_MODULE_BASE 0xE6660000u

//! \defgroup LS_DESC_DW0_PORT0 Register LS_DESC_DW0_PORT0 - CQM Load Spreader Module Register Description
//! @{

//! Register Offset (relative)
#define LS_DESC_DW0_PORT0 0x0
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW0_PORT0 0xE6660000u

//! Register Reset Value
#define LS_DESC_DW0_PORT0_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT0_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT0_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW1_PORT0 Register LS_DESC_DW1_PORT0 - Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_DESC_DW1_PORT0 0x4
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW1_PORT0 0xE6660004u

//! Register Reset Value
#define LS_DESC_DW1_PORT0_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT0_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT0_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW2_PORT0 Register LS_DESC_DW2_PORT0 - Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_DESC_DW2_PORT0 0x8
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW2_PORT0 0xE6660008u

//! Register Reset Value
#define LS_DESC_DW2_PORT0_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT0_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT0_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW3_PORT0 Register LS_DESC_DW3_PORT0 - Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_DESC_DW3_PORT0 0xC
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW3_PORT0 0xE666000Cu

//! Register Reset Value
#define LS_DESC_DW3_PORT0_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT0_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT0_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_CTRL_PORT0 Register LS_CTRL_PORT0 - Load Spreader Port Control Register
//! @{

//! Register Offset (relative)
#define LS_CTRL_PORT0 0x10
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_CTRL_PORT0 0xE6660010u

//! Register Reset Value
#define LS_CTRL_PORT0_RST 0x00000400u

//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT0_PORT_EN_POS 0
//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT0_PORT_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT0_PORT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT0_PORT_EN_EN 0x1

//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT0_SPR_EN_POS 1
//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT0_SPR_EN_MASK 0x2u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT0_SPR_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT0_SPR_EN_EN 0x1

//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT0_INT_EN_POS 2
//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT0_INT_EN_MASK 0x4u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT0_INT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT0_INT_EN_EN 0x1

//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT0_CNT_EN_POS 3
//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT0_CNT_EN_MASK 0x8u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT0_CNT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT0_CNT_EN_EN 0x1

//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT0_QUEUE_THRSHLD_POS 8
//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT0_QUEUE_THRSHLD_MASK 0x700u

//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT0_CNT_THRSHLD_POS 16
//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT0_CNT_THRSHLD_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_STATUS_PORT0 Register LS_STATUS_PORT0 - Load Spreader Port Status Register
//! @{

//! Register Offset (relative)
#define LS_STATUS_PORT0 0x14
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_STATUS_PORT0 0xE6660014u

//! Register Reset Value
#define LS_STATUS_PORT0_RST 0x00002000u

//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT0_QUEUE_LEN_POS 7
//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT0_QUEUE_LEN_MASK 0x780u

//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT0_QUEUE_FULL_POS 12
//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT0_QUEUE_FULL_MASK 0x1000u

//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT0_QUEUE_EMPTY_POS 13
//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT0_QUEUE_EMPTY_MASK 0x2000u

//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT0_CNT_VAL_POS 16
//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT0_CNT_VAL_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_QDESC_DW0_0_PORT0 Register LS_QDESC_DW0_0_PORT0 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_0_PORT0 0x20
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_0_PORT0 0xE6660020u

//! Register Reset Value
#define LS_QDESC_DW0_0_PORT0_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT0_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT0_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_0_PORT0 Register LS_QDESC_DW1_0_PORT0 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_0_PORT0 0x24
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_0_PORT0 0xE6660024u

//! Register Reset Value
#define LS_QDESC_DW1_0_PORT0_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT0_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT0_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_0_PORT0 Register LS_QDESC_DW2_0_PORT0 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_0_PORT0 0x28
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_0_PORT0 0xE6660028u

//! Register Reset Value
#define LS_QDESC_DW2_0_PORT0_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT0_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT0_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_0_PORT0 Register LS_QDESC_DW3_0_PORT0 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_0_PORT0 0x2C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_0_PORT0 0xE666002Cu

//! Register Reset Value
#define LS_QDESC_DW3_0_PORT0_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT0_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT0_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_1_PORT0 Register LS_QDESC_DW0_1_PORT0 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_1_PORT0 0x30
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_1_PORT0 0xE6660030u

//! Register Reset Value
#define LS_QDESC_DW0_1_PORT0_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT0_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT0_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_1_PORT0 Register LS_QDESC_DW1_1_PORT0 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_1_PORT0 0x34
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_1_PORT0 0xE6660034u

//! Register Reset Value
#define LS_QDESC_DW1_1_PORT0_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT0_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT0_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_1_PORT0 Register LS_QDESC_DW2_1_PORT0 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_1_PORT0 0x38
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_1_PORT0 0xE6660038u

//! Register Reset Value
#define LS_QDESC_DW2_1_PORT0_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT0_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT0_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_1_PORT0 Register LS_QDESC_DW3_1_PORT0 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_1_PORT0 0x3C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_1_PORT0 0xE666003Cu

//! Register Reset Value
#define LS_QDESC_DW3_1_PORT0_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT0_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT0_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_2_PORT0 Register LS_QDESC_DW0_2_PORT0 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_2_PORT0 0x40
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_2_PORT0 0xE6660040u

//! Register Reset Value
#define LS_QDESC_DW0_2_PORT0_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT0_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT0_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_2_PORT0 Register LS_QDESC_DW1_2_PORT0 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_2_PORT0 0x44
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_2_PORT0 0xE6660044u

//! Register Reset Value
#define LS_QDESC_DW1_2_PORT0_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT0_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT0_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_2_PORT0 Register LS_QDESC_DW2_2_PORT0 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_2_PORT0 0x48
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_2_PORT0 0xE6660048u

//! Register Reset Value
#define LS_QDESC_DW2_2_PORT0_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT0_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT0_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_2_PORT0 Register LS_QDESC_DW3_2_PORT0 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_2_PORT0 0x4C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_2_PORT0 0xE666004Cu

//! Register Reset Value
#define LS_QDESC_DW3_2_PORT0_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT0_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT0_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_3_PORT0 Register LS_QDESC_DW0_3_PORT0 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_3_PORT0 0x50
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_3_PORT0 0xE6660050u

//! Register Reset Value
#define LS_QDESC_DW0_3_PORT0_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT0_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT0_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_3_PORT0 Register LS_QDESC_DW1_3_PORT0 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_3_PORT0 0x54
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_3_PORT0 0xE6660054u

//! Register Reset Value
#define LS_QDESC_DW1_3_PORT0_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT0_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT0_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_3_PORT0 Register LS_QDESC_DW2_3_PORT0 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_3_PORT0 0x58
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_3_PORT0 0xE6660058u

//! Register Reset Value
#define LS_QDESC_DW2_3_PORT0_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT0_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT0_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_3_PORT0 Register LS_QDESC_DW3_3_PORT0 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_3_PORT0 0x5C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_3_PORT0 0xE666005Cu

//! Register Reset Value
#define LS_QDESC_DW3_3_PORT0_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT0_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT0_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_4_PORT0 Register LS_QDESC_DW0_4_PORT0 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_4_PORT0 0x60
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_4_PORT0 0xE6660060u

//! Register Reset Value
#define LS_QDESC_DW0_4_PORT0_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT0_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT0_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_4_PORT0 Register LS_QDESC_DW1_4_PORT0 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_4_PORT0 0x64
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_4_PORT0 0xE6660064u

//! Register Reset Value
#define LS_QDESC_DW1_4_PORT0_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT0_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT0_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_4_PORT0 Register LS_QDESC_DW2_4_PORT0 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_4_PORT0 0x68
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_4_PORT0 0xE6660068u

//! Register Reset Value
#define LS_QDESC_DW2_4_PORT0_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT0_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT0_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_4_PORT0 Register LS_QDESC_DW3_4_PORT0 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_4_PORT0 0x6C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_4_PORT0 0xE666006Cu

//! Register Reset Value
#define LS_QDESC_DW3_4_PORT0_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT0_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT0_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_5_PORT0 Register LS_QDESC_DW0_5_PORT0 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_5_PORT0 0x70
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_5_PORT0 0xE6660070u

//! Register Reset Value
#define LS_QDESC_DW0_5_PORT0_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT0_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT0_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_5_PORT0 Register LS_QDESC_DW1_5_PORT0 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_5_PORT0 0x74
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_5_PORT0 0xE6660074u

//! Register Reset Value
#define LS_QDESC_DW1_5_PORT0_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT0_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT0_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_5_PORT0 Register LS_QDESC_DW2_5_PORT0 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_5_PORT0 0x78
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_5_PORT0 0xE6660078u

//! Register Reset Value
#define LS_QDESC_DW2_5_PORT0_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT0_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT0_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_5_PORT0 Register LS_QDESC_DW3_5_PORT0 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_5_PORT0 0x7C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_5_PORT0 0xE666007Cu

//! Register Reset Value
#define LS_QDESC_DW3_5_PORT0_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT0_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT0_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_6_PORT0 Register LS_QDESC_DW0_6_PORT0 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_6_PORT0 0x80
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_6_PORT0 0xE6660080u

//! Register Reset Value
#define LS_QDESC_DW0_6_PORT0_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT0_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT0_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_6_PORT0 Register LS_QDESC_DW1_6_PORT0 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_6_PORT0 0x84
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_6_PORT0 0xE6660084u

//! Register Reset Value
#define LS_QDESC_DW1_6_PORT0_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT0_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT0_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_6_PORT0 Register LS_QDESC_DW2_6_PORT0 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_6_PORT0 0x88
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_6_PORT0 0xE6660088u

//! Register Reset Value
#define LS_QDESC_DW2_6_PORT0_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT0_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT0_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_6_PORT0 Register LS_QDESC_DW3_6_PORT0 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_6_PORT0 0x8C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_6_PORT0 0xE666008Cu

//! Register Reset Value
#define LS_QDESC_DW3_6_PORT0_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT0_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT0_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW0_PORT1 Register LS_DESC_DW0_PORT1 - CQM Load Spreader Module Register Description
//! @{

//! Register Offset (relative)
#define LS_DESC_DW0_PORT1 0x100
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW0_PORT1 0xE6660100u

//! Register Reset Value
#define LS_DESC_DW0_PORT1_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT1_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT1_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW1_PORT1 Register LS_DESC_DW1_PORT1 - Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_DESC_DW1_PORT1 0x104
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW1_PORT1 0xE6660104u

//! Register Reset Value
#define LS_DESC_DW1_PORT1_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT1_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT1_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW2_PORT1 Register LS_DESC_DW2_PORT1 - Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_DESC_DW2_PORT1 0x108
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW2_PORT1 0xE6660108u

//! Register Reset Value
#define LS_DESC_DW2_PORT1_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT1_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT1_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW3_PORT1 Register LS_DESC_DW3_PORT1 - Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_DESC_DW3_PORT1 0x10C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW3_PORT1 0xE666010Cu

//! Register Reset Value
#define LS_DESC_DW3_PORT1_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT1_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT1_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_CTRL_PORT1 Register LS_CTRL_PORT1 - Load Spreader Port Control Register
//! @{

//! Register Offset (relative)
#define LS_CTRL_PORT1 0x110
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_CTRL_PORT1 0xE6660110u

//! Register Reset Value
#define LS_CTRL_PORT1_RST 0x00000400u

//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT1_PORT_EN_POS 0
//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT1_PORT_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT1_PORT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT1_PORT_EN_EN 0x1

//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT1_SPR_EN_POS 1
//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT1_SPR_EN_MASK 0x2u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT1_SPR_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT1_SPR_EN_EN 0x1

//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT1_INT_EN_POS 2
//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT1_INT_EN_MASK 0x4u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT1_INT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT1_INT_EN_EN 0x1

//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT1_CNT_EN_POS 3
//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT1_CNT_EN_MASK 0x8u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT1_CNT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT1_CNT_EN_EN 0x1

//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT1_QUEUE_THRSHLD_POS 8
//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT1_QUEUE_THRSHLD_MASK 0x700u

//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT1_CNT_THRSHLD_POS 16
//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT1_CNT_THRSHLD_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_STATUS_PORT1 Register LS_STATUS_PORT1 - Load Spreader Port Status Register
//! @{

//! Register Offset (relative)
#define LS_STATUS_PORT1 0x114
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_STATUS_PORT1 0xE6660114u

//! Register Reset Value
#define LS_STATUS_PORT1_RST 0x00002000u

//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT1_QUEUE_LEN_POS 7
//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT1_QUEUE_LEN_MASK 0x780u

//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT1_QUEUE_FULL_POS 12
//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT1_QUEUE_FULL_MASK 0x1000u

//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT1_QUEUE_EMPTY_POS 13
//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT1_QUEUE_EMPTY_MASK 0x2000u

//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT1_CNT_VAL_POS 16
//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT1_CNT_VAL_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_QDESC_DW0_0_PORT1 Register LS_QDESC_DW0_0_PORT1 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_0_PORT1 0x120
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_0_PORT1 0xE6660120u

//! Register Reset Value
#define LS_QDESC_DW0_0_PORT1_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT1_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT1_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_0_PORT1 Register LS_QDESC_DW1_0_PORT1 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_0_PORT1 0x124
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_0_PORT1 0xE6660124u

//! Register Reset Value
#define LS_QDESC_DW1_0_PORT1_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT1_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT1_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_0_PORT1 Register LS_QDESC_DW2_0_PORT1 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_0_PORT1 0x128
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_0_PORT1 0xE6660128u

//! Register Reset Value
#define LS_QDESC_DW2_0_PORT1_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT1_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT1_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_0_PORT1 Register LS_QDESC_DW3_0_PORT1 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_0_PORT1 0x12C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_0_PORT1 0xE666012Cu

//! Register Reset Value
#define LS_QDESC_DW3_0_PORT1_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT1_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT1_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_1_PORT1 Register LS_QDESC_DW0_1_PORT1 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_1_PORT1 0x130
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_1_PORT1 0xE6660130u

//! Register Reset Value
#define LS_QDESC_DW0_1_PORT1_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT1_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT1_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_1_PORT1 Register LS_QDESC_DW1_1_PORT1 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_1_PORT1 0x134
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_1_PORT1 0xE6660134u

//! Register Reset Value
#define LS_QDESC_DW1_1_PORT1_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT1_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT1_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_1_PORT1 Register LS_QDESC_DW2_1_PORT1 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_1_PORT1 0x138
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_1_PORT1 0xE6660138u

//! Register Reset Value
#define LS_QDESC_DW2_1_PORT1_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT1_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT1_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_1_PORT1 Register LS_QDESC_DW3_1_PORT1 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_1_PORT1 0x13C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_1_PORT1 0xE666013Cu

//! Register Reset Value
#define LS_QDESC_DW3_1_PORT1_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT1_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT1_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_2_PORT1 Register LS_QDESC_DW0_2_PORT1 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_2_PORT1 0x140
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_2_PORT1 0xE6660140u

//! Register Reset Value
#define LS_QDESC_DW0_2_PORT1_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT1_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT1_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_2_PORT1 Register LS_QDESC_DW1_2_PORT1 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_2_PORT1 0x144
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_2_PORT1 0xE6660144u

//! Register Reset Value
#define LS_QDESC_DW1_2_PORT1_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT1_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT1_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_2_PORT1 Register LS_QDESC_DW2_2_PORT1 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_2_PORT1 0x148
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_2_PORT1 0xE6660148u

//! Register Reset Value
#define LS_QDESC_DW2_2_PORT1_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT1_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT1_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_2_PORT1 Register LS_QDESC_DW3_2_PORT1 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_2_PORT1 0x14C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_2_PORT1 0xE666014Cu

//! Register Reset Value
#define LS_QDESC_DW3_2_PORT1_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT1_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT1_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_3_PORT1 Register LS_QDESC_DW0_3_PORT1 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_3_PORT1 0x150
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_3_PORT1 0xE6660150u

//! Register Reset Value
#define LS_QDESC_DW0_3_PORT1_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT1_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT1_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_3_PORT1 Register LS_QDESC_DW1_3_PORT1 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_3_PORT1 0x154
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_3_PORT1 0xE6660154u

//! Register Reset Value
#define LS_QDESC_DW1_3_PORT1_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT1_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT1_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_3_PORT1 Register LS_QDESC_DW2_3_PORT1 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_3_PORT1 0x158
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_3_PORT1 0xE6660158u

//! Register Reset Value
#define LS_QDESC_DW2_3_PORT1_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT1_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT1_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_3_PORT1 Register LS_QDESC_DW3_3_PORT1 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_3_PORT1 0x15C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_3_PORT1 0xE666015Cu

//! Register Reset Value
#define LS_QDESC_DW3_3_PORT1_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT1_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT1_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_4_PORT1 Register LS_QDESC_DW0_4_PORT1 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_4_PORT1 0x160
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_4_PORT1 0xE6660160u

//! Register Reset Value
#define LS_QDESC_DW0_4_PORT1_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT1_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT1_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_4_PORT1 Register LS_QDESC_DW1_4_PORT1 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_4_PORT1 0x164
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_4_PORT1 0xE6660164u

//! Register Reset Value
#define LS_QDESC_DW1_4_PORT1_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT1_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT1_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_4_PORT1 Register LS_QDESC_DW2_4_PORT1 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_4_PORT1 0x168
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_4_PORT1 0xE6660168u

//! Register Reset Value
#define LS_QDESC_DW2_4_PORT1_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT1_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT1_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_4_PORT1 Register LS_QDESC_DW3_4_PORT1 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_4_PORT1 0x16C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_4_PORT1 0xE666016Cu

//! Register Reset Value
#define LS_QDESC_DW3_4_PORT1_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT1_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT1_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_5_PORT1 Register LS_QDESC_DW0_5_PORT1 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_5_PORT1 0x170
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_5_PORT1 0xE6660170u

//! Register Reset Value
#define LS_QDESC_DW0_5_PORT1_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT1_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT1_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_5_PORT1 Register LS_QDESC_DW1_5_PORT1 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_5_PORT1 0x174
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_5_PORT1 0xE6660174u

//! Register Reset Value
#define LS_QDESC_DW1_5_PORT1_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT1_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT1_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_5_PORT1 Register LS_QDESC_DW2_5_PORT1 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_5_PORT1 0x178
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_5_PORT1 0xE6660178u

//! Register Reset Value
#define LS_QDESC_DW2_5_PORT1_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT1_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT1_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_5_PORT1 Register LS_QDESC_DW3_5_PORT1 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_5_PORT1 0x17C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_5_PORT1 0xE666017Cu

//! Register Reset Value
#define LS_QDESC_DW3_5_PORT1_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT1_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT1_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_6_PORT1 Register LS_QDESC_DW0_6_PORT1 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_6_PORT1 0x180
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_6_PORT1 0xE6660180u

//! Register Reset Value
#define LS_QDESC_DW0_6_PORT1_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT1_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT1_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_6_PORT1 Register LS_QDESC_DW1_6_PORT1 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_6_PORT1 0x184
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_6_PORT1 0xE6660184u

//! Register Reset Value
#define LS_QDESC_DW1_6_PORT1_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT1_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT1_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_6_PORT1 Register LS_QDESC_DW2_6_PORT1 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_6_PORT1 0x188
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_6_PORT1 0xE6660188u

//! Register Reset Value
#define LS_QDESC_DW2_6_PORT1_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT1_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT1_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_6_PORT1 Register LS_QDESC_DW3_6_PORT1 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_6_PORT1 0x18C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_6_PORT1 0xE666018Cu

//! Register Reset Value
#define LS_QDESC_DW3_6_PORT1_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT1_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT1_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW0_PORT2 Register LS_DESC_DW0_PORT2 - CQM Load Spreader Module Register Description
//! @{

//! Register Offset (relative)
#define LS_DESC_DW0_PORT2 0x200
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW0_PORT2 0xE6660200u

//! Register Reset Value
#define LS_DESC_DW0_PORT2_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT2_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT2_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW1_PORT2 Register LS_DESC_DW1_PORT2 - Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_DESC_DW1_PORT2 0x204
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW1_PORT2 0xE6660204u

//! Register Reset Value
#define LS_DESC_DW1_PORT2_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT2_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT2_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW2_PORT2 Register LS_DESC_DW2_PORT2 - Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_DESC_DW2_PORT2 0x208
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW2_PORT2 0xE6660208u

//! Register Reset Value
#define LS_DESC_DW2_PORT2_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT2_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT2_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW3_PORT2 Register LS_DESC_DW3_PORT2 - Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_DESC_DW3_PORT2 0x20C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW3_PORT2 0xE666020Cu

//! Register Reset Value
#define LS_DESC_DW3_PORT2_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT2_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT2_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_CTRL_PORT2 Register LS_CTRL_PORT2 - Load Spreader Port Control Register
//! @{

//! Register Offset (relative)
#define LS_CTRL_PORT2 0x210
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_CTRL_PORT2 0xE6660210u

//! Register Reset Value
#define LS_CTRL_PORT2_RST 0x00000400u

//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT2_PORT_EN_POS 0
//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT2_PORT_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT2_PORT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT2_PORT_EN_EN 0x1

//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT2_SPR_EN_POS 1
//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT2_SPR_EN_MASK 0x2u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT2_SPR_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT2_SPR_EN_EN 0x1

//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT2_INT_EN_POS 2
//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT2_INT_EN_MASK 0x4u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT2_INT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT2_INT_EN_EN 0x1

//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT2_CNT_EN_POS 3
//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT2_CNT_EN_MASK 0x8u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT2_CNT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT2_CNT_EN_EN 0x1

//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT2_QUEUE_THRSHLD_POS 8
//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT2_QUEUE_THRSHLD_MASK 0x700u

//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT2_CNT_THRSHLD_POS 16
//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT2_CNT_THRSHLD_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_STATUS_PORT2 Register LS_STATUS_PORT2 - Load Spreader Port Status Register
//! @{

//! Register Offset (relative)
#define LS_STATUS_PORT2 0x214
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_STATUS_PORT2 0xE6660214u

//! Register Reset Value
#define LS_STATUS_PORT2_RST 0x00002000u

//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT2_QUEUE_LEN_POS 7
//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT2_QUEUE_LEN_MASK 0x780u

//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT2_QUEUE_FULL_POS 12
//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT2_QUEUE_FULL_MASK 0x1000u

//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT2_QUEUE_EMPTY_POS 13
//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT2_QUEUE_EMPTY_MASK 0x2000u

//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT2_CNT_VAL_POS 16
//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT2_CNT_VAL_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_QDESC_DW0_0_PORT2 Register LS_QDESC_DW0_0_PORT2 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_0_PORT2 0x220
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_0_PORT2 0xE6660220u

//! Register Reset Value
#define LS_QDESC_DW0_0_PORT2_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT2_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT2_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_0_PORT2 Register LS_QDESC_DW1_0_PORT2 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_0_PORT2 0x224
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_0_PORT2 0xE6660224u

//! Register Reset Value
#define LS_QDESC_DW1_0_PORT2_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT2_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT2_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_0_PORT2 Register LS_QDESC_DW2_0_PORT2 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_0_PORT2 0x228
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_0_PORT2 0xE6660228u

//! Register Reset Value
#define LS_QDESC_DW2_0_PORT2_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT2_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT2_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_0_PORT2 Register LS_QDESC_DW3_0_PORT2 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_0_PORT2 0x22C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_0_PORT2 0xE666022Cu

//! Register Reset Value
#define LS_QDESC_DW3_0_PORT2_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT2_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT2_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_1_PORT2 Register LS_QDESC_DW0_1_PORT2 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_1_PORT2 0x230
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_1_PORT2 0xE6660230u

//! Register Reset Value
#define LS_QDESC_DW0_1_PORT2_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT2_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT2_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_1_PORT2 Register LS_QDESC_DW1_1_PORT2 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_1_PORT2 0x234
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_1_PORT2 0xE6660234u

//! Register Reset Value
#define LS_QDESC_DW1_1_PORT2_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT2_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT2_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_1_PORT2 Register LS_QDESC_DW2_1_PORT2 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_1_PORT2 0x238
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_1_PORT2 0xE6660238u

//! Register Reset Value
#define LS_QDESC_DW2_1_PORT2_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT2_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT2_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_1_PORT2 Register LS_QDESC_DW3_1_PORT2 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_1_PORT2 0x23C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_1_PORT2 0xE666023Cu

//! Register Reset Value
#define LS_QDESC_DW3_1_PORT2_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT2_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT2_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_2_PORT2 Register LS_QDESC_DW0_2_PORT2 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_2_PORT2 0x240
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_2_PORT2 0xE6660240u

//! Register Reset Value
#define LS_QDESC_DW0_2_PORT2_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT2_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT2_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_2_PORT2 Register LS_QDESC_DW1_2_PORT2 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_2_PORT2 0x244
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_2_PORT2 0xE6660244u

//! Register Reset Value
#define LS_QDESC_DW1_2_PORT2_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT2_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT2_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_2_PORT2 Register LS_QDESC_DW2_2_PORT2 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_2_PORT2 0x248
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_2_PORT2 0xE6660248u

//! Register Reset Value
#define LS_QDESC_DW2_2_PORT2_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT2_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT2_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_2_PORT2 Register LS_QDESC_DW3_2_PORT2 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_2_PORT2 0x24C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_2_PORT2 0xE666024Cu

//! Register Reset Value
#define LS_QDESC_DW3_2_PORT2_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT2_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT2_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_3_PORT2 Register LS_QDESC_DW0_3_PORT2 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_3_PORT2 0x250
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_3_PORT2 0xE6660250u

//! Register Reset Value
#define LS_QDESC_DW0_3_PORT2_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT2_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT2_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_3_PORT2 Register LS_QDESC_DW1_3_PORT2 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_3_PORT2 0x254
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_3_PORT2 0xE6660254u

//! Register Reset Value
#define LS_QDESC_DW1_3_PORT2_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT2_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT2_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_3_PORT2 Register LS_QDESC_DW2_3_PORT2 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_3_PORT2 0x258
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_3_PORT2 0xE6660258u

//! Register Reset Value
#define LS_QDESC_DW2_3_PORT2_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT2_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT2_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_3_PORT2 Register LS_QDESC_DW3_3_PORT2 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_3_PORT2 0x25C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_3_PORT2 0xE666025Cu

//! Register Reset Value
#define LS_QDESC_DW3_3_PORT2_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT2_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT2_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_4_PORT2 Register LS_QDESC_DW0_4_PORT2 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_4_PORT2 0x260
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_4_PORT2 0xE6660260u

//! Register Reset Value
#define LS_QDESC_DW0_4_PORT2_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT2_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT2_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_4_PORT2 Register LS_QDESC_DW1_4_PORT2 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_4_PORT2 0x264
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_4_PORT2 0xE6660264u

//! Register Reset Value
#define LS_QDESC_DW1_4_PORT2_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT2_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT2_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_4_PORT2 Register LS_QDESC_DW2_4_PORT2 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_4_PORT2 0x268
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_4_PORT2 0xE6660268u

//! Register Reset Value
#define LS_QDESC_DW2_4_PORT2_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT2_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT2_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_4_PORT2 Register LS_QDESC_DW3_4_PORT2 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_4_PORT2 0x26C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_4_PORT2 0xE666026Cu

//! Register Reset Value
#define LS_QDESC_DW3_4_PORT2_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT2_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT2_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_5_PORT2 Register LS_QDESC_DW0_5_PORT2 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_5_PORT2 0x270
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_5_PORT2 0xE6660270u

//! Register Reset Value
#define LS_QDESC_DW0_5_PORT2_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT2_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT2_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_5_PORT2 Register LS_QDESC_DW1_5_PORT2 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_5_PORT2 0x274
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_5_PORT2 0xE6660274u

//! Register Reset Value
#define LS_QDESC_DW1_5_PORT2_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT2_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT2_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_5_PORT2 Register LS_QDESC_DW2_5_PORT2 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_5_PORT2 0x278
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_5_PORT2 0xE6660278u

//! Register Reset Value
#define LS_QDESC_DW2_5_PORT2_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT2_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT2_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_5_PORT2 Register LS_QDESC_DW3_5_PORT2 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_5_PORT2 0x27C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_5_PORT2 0xE666027Cu

//! Register Reset Value
#define LS_QDESC_DW3_5_PORT2_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT2_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT2_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_6_PORT2 Register LS_QDESC_DW0_6_PORT2 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_6_PORT2 0x280
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_6_PORT2 0xE6660280u

//! Register Reset Value
#define LS_QDESC_DW0_6_PORT2_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT2_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT2_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_6_PORT2 Register LS_QDESC_DW1_6_PORT2 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_6_PORT2 0x284
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_6_PORT2 0xE6660284u

//! Register Reset Value
#define LS_QDESC_DW1_6_PORT2_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT2_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT2_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_6_PORT2 Register LS_QDESC_DW2_6_PORT2 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_6_PORT2 0x288
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_6_PORT2 0xE6660288u

//! Register Reset Value
#define LS_QDESC_DW2_6_PORT2_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT2_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT2_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_6_PORT2 Register LS_QDESC_DW3_6_PORT2 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_6_PORT2 0x28C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_6_PORT2 0xE666028Cu

//! Register Reset Value
#define LS_QDESC_DW3_6_PORT2_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT2_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT2_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW0_PORT3 Register LS_DESC_DW0_PORT3 - CQM Load Spreader Module Register Description
//! @{

//! Register Offset (relative)
#define LS_DESC_DW0_PORT3 0x300
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW0_PORT3 0xE6660300u

//! Register Reset Value
#define LS_DESC_DW0_PORT3_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT3_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT3_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW1_PORT3 Register LS_DESC_DW1_PORT3 - Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_DESC_DW1_PORT3 0x304
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW1_PORT3 0xE6660304u

//! Register Reset Value
#define LS_DESC_DW1_PORT3_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT3_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT3_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW2_PORT3 Register LS_DESC_DW2_PORT3 - Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_DESC_DW2_PORT3 0x308
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW2_PORT3 0xE6660308u

//! Register Reset Value
#define LS_DESC_DW2_PORT3_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT3_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT3_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW3_PORT3 Register LS_DESC_DW3_PORT3 - Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_DESC_DW3_PORT3 0x30C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW3_PORT3 0xE666030Cu

//! Register Reset Value
#define LS_DESC_DW3_PORT3_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT3_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT3_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_CTRL_PORT3 Register LS_CTRL_PORT3 - Load Spreader Port Control Register
//! @{

//! Register Offset (relative)
#define LS_CTRL_PORT3 0x310
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_CTRL_PORT3 0xE6660310u

//! Register Reset Value
#define LS_CTRL_PORT3_RST 0x00000400u

//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT3_PORT_EN_POS 0
//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT3_PORT_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT3_PORT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT3_PORT_EN_EN 0x1

//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT3_SPR_EN_POS 1
//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT3_SPR_EN_MASK 0x2u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT3_SPR_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT3_SPR_EN_EN 0x1

//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT3_INT_EN_POS 2
//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT3_INT_EN_MASK 0x4u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT3_INT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT3_INT_EN_EN 0x1

//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT3_CNT_EN_POS 3
//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT3_CNT_EN_MASK 0x8u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT3_CNT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT3_CNT_EN_EN 0x1

//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT3_QUEUE_THRSHLD_POS 8
//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT3_QUEUE_THRSHLD_MASK 0x700u

//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT3_CNT_THRSHLD_POS 16
//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT3_CNT_THRSHLD_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_STATUS_PORT3 Register LS_STATUS_PORT3 - Load Spreader Port Status Register
//! @{

//! Register Offset (relative)
#define LS_STATUS_PORT3 0x314
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_STATUS_PORT3 0xE6660314u

//! Register Reset Value
#define LS_STATUS_PORT3_RST 0x00002000u

//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT3_QUEUE_LEN_POS 7
//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT3_QUEUE_LEN_MASK 0x780u

//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT3_QUEUE_FULL_POS 12
//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT3_QUEUE_FULL_MASK 0x1000u

//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT3_QUEUE_EMPTY_POS 13
//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT3_QUEUE_EMPTY_MASK 0x2000u

//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT3_CNT_VAL_POS 16
//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT3_CNT_VAL_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_QDESC_DW0_0_PORT3 Register LS_QDESC_DW0_0_PORT3 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_0_PORT3 0x320
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_0_PORT3 0xE6660320u

//! Register Reset Value
#define LS_QDESC_DW0_0_PORT3_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT3_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT3_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_0_PORT3 Register LS_QDESC_DW1_0_PORT3 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_0_PORT3 0x324
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_0_PORT3 0xE6660324u

//! Register Reset Value
#define LS_QDESC_DW1_0_PORT3_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT3_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT3_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_0_PORT3 Register LS_QDESC_DW2_0_PORT3 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_0_PORT3 0x328
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_0_PORT3 0xE6660328u

//! Register Reset Value
#define LS_QDESC_DW2_0_PORT3_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT3_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT3_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_0_PORT3 Register LS_QDESC_DW3_0_PORT3 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_0_PORT3 0x32C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_0_PORT3 0xE666032Cu

//! Register Reset Value
#define LS_QDESC_DW3_0_PORT3_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT3_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT3_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_1_PORT3 Register LS_QDESC_DW0_1_PORT3 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_1_PORT3 0x330
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_1_PORT3 0xE6660330u

//! Register Reset Value
#define LS_QDESC_DW0_1_PORT3_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT3_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT3_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_1_PORT3 Register LS_QDESC_DW1_1_PORT3 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_1_PORT3 0x334
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_1_PORT3 0xE6660334u

//! Register Reset Value
#define LS_QDESC_DW1_1_PORT3_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT3_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT3_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_1_PORT3 Register LS_QDESC_DW2_1_PORT3 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_1_PORT3 0x338
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_1_PORT3 0xE6660338u

//! Register Reset Value
#define LS_QDESC_DW2_1_PORT3_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT3_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT3_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_1_PORT3 Register LS_QDESC_DW3_1_PORT3 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_1_PORT3 0x33C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_1_PORT3 0xE666033Cu

//! Register Reset Value
#define LS_QDESC_DW3_1_PORT3_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT3_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT3_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_2_PORT3 Register LS_QDESC_DW0_2_PORT3 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_2_PORT3 0x340
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_2_PORT3 0xE6660340u

//! Register Reset Value
#define LS_QDESC_DW0_2_PORT3_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT3_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT3_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_2_PORT3 Register LS_QDESC_DW1_2_PORT3 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_2_PORT3 0x344
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_2_PORT3 0xE6660344u

//! Register Reset Value
#define LS_QDESC_DW1_2_PORT3_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT3_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT3_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_2_PORT3 Register LS_QDESC_DW2_2_PORT3 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_2_PORT3 0x348
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_2_PORT3 0xE6660348u

//! Register Reset Value
#define LS_QDESC_DW2_2_PORT3_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT3_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT3_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_2_PORT3 Register LS_QDESC_DW3_2_PORT3 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_2_PORT3 0x34C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_2_PORT3 0xE666034Cu

//! Register Reset Value
#define LS_QDESC_DW3_2_PORT3_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT3_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT3_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_3_PORT3 Register LS_QDESC_DW0_3_PORT3 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_3_PORT3 0x350
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_3_PORT3 0xE6660350u

//! Register Reset Value
#define LS_QDESC_DW0_3_PORT3_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT3_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT3_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_3_PORT3 Register LS_QDESC_DW1_3_PORT3 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_3_PORT3 0x354
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_3_PORT3 0xE6660354u

//! Register Reset Value
#define LS_QDESC_DW1_3_PORT3_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT3_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT3_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_3_PORT3 Register LS_QDESC_DW2_3_PORT3 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_3_PORT3 0x358
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_3_PORT3 0xE6660358u

//! Register Reset Value
#define LS_QDESC_DW2_3_PORT3_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT3_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT3_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_3_PORT3 Register LS_QDESC_DW3_3_PORT3 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_3_PORT3 0x35C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_3_PORT3 0xE666035Cu

//! Register Reset Value
#define LS_QDESC_DW3_3_PORT3_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT3_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT3_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_4_PORT3 Register LS_QDESC_DW0_4_PORT3 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_4_PORT3 0x360
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_4_PORT3 0xE6660360u

//! Register Reset Value
#define LS_QDESC_DW0_4_PORT3_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT3_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT3_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_4_PORT3 Register LS_QDESC_DW1_4_PORT3 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_4_PORT3 0x364
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_4_PORT3 0xE6660364u

//! Register Reset Value
#define LS_QDESC_DW1_4_PORT3_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT3_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT3_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_4_PORT3 Register LS_QDESC_DW2_4_PORT3 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_4_PORT3 0x368
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_4_PORT3 0xE6660368u

//! Register Reset Value
#define LS_QDESC_DW2_4_PORT3_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT3_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT3_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_4_PORT3 Register LS_QDESC_DW3_4_PORT3 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_4_PORT3 0x36C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_4_PORT3 0xE666036Cu

//! Register Reset Value
#define LS_QDESC_DW3_4_PORT3_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT3_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT3_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_5_PORT3 Register LS_QDESC_DW0_5_PORT3 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_5_PORT3 0x370
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_5_PORT3 0xE6660370u

//! Register Reset Value
#define LS_QDESC_DW0_5_PORT3_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT3_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT3_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_5_PORT3 Register LS_QDESC_DW1_5_PORT3 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_5_PORT3 0x374
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_5_PORT3 0xE6660374u

//! Register Reset Value
#define LS_QDESC_DW1_5_PORT3_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT3_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT3_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_5_PORT3 Register LS_QDESC_DW2_5_PORT3 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_5_PORT3 0x378
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_5_PORT3 0xE6660378u

//! Register Reset Value
#define LS_QDESC_DW2_5_PORT3_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT3_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT3_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_5_PORT3 Register LS_QDESC_DW3_5_PORT3 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_5_PORT3 0x37C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_5_PORT3 0xE666037Cu

//! Register Reset Value
#define LS_QDESC_DW3_5_PORT3_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT3_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT3_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_6_PORT3 Register LS_QDESC_DW0_6_PORT3 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_6_PORT3 0x380
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_6_PORT3 0xE6660380u

//! Register Reset Value
#define LS_QDESC_DW0_6_PORT3_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT3_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT3_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_6_PORT3 Register LS_QDESC_DW1_6_PORT3 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_6_PORT3 0x384
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_6_PORT3 0xE6660384u

//! Register Reset Value
#define LS_QDESC_DW1_6_PORT3_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT3_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT3_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_6_PORT3 Register LS_QDESC_DW2_6_PORT3 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_6_PORT3 0x388
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_6_PORT3 0xE6660388u

//! Register Reset Value
#define LS_QDESC_DW2_6_PORT3_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT3_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT3_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_6_PORT3 Register LS_QDESC_DW3_6_PORT3 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_6_PORT3 0x38C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_6_PORT3 0xE666038Cu

//! Register Reset Value
#define LS_QDESC_DW3_6_PORT3_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT3_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT3_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW0_PORT4 Register LS_DESC_DW0_PORT4 - CQM Load Spreader Module Register Description
//! @{

//! Register Offset (relative)
#define LS_DESC_DW0_PORT4 0x400
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW0_PORT4 0xE6660400u

//! Register Reset Value
#define LS_DESC_DW0_PORT4_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT4_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT4_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW1_PORT4 Register LS_DESC_DW1_PORT4 - Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_DESC_DW1_PORT4 0x404
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW1_PORT4 0xE6660404u

//! Register Reset Value
#define LS_DESC_DW1_PORT4_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT4_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT4_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW2_PORT4 Register LS_DESC_DW2_PORT4 - Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_DESC_DW2_PORT4 0x408
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW2_PORT4 0xE6660408u

//! Register Reset Value
#define LS_DESC_DW2_PORT4_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT4_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT4_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW3_PORT4 Register LS_DESC_DW3_PORT4 - Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_DESC_DW3_PORT4 0x40C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW3_PORT4 0xE666040Cu

//! Register Reset Value
#define LS_DESC_DW3_PORT4_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT4_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT4_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_CTRL_PORT4 Register LS_CTRL_PORT4 - Load Spreader Port Control Register
//! @{

//! Register Offset (relative)
#define LS_CTRL_PORT4 0x410
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_CTRL_PORT4 0xE6660410u

//! Register Reset Value
#define LS_CTRL_PORT4_RST 0x00000400u

//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT4_PORT_EN_POS 0
//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT4_PORT_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT4_PORT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT4_PORT_EN_EN 0x1

//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT4_SPR_EN_POS 1
//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT4_SPR_EN_MASK 0x2u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT4_SPR_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT4_SPR_EN_EN 0x1

//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT4_INT_EN_POS 2
//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT4_INT_EN_MASK 0x4u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT4_INT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT4_INT_EN_EN 0x1

//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT4_CNT_EN_POS 3
//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT4_CNT_EN_MASK 0x8u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT4_CNT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT4_CNT_EN_EN 0x1

//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT4_QUEUE_THRSHLD_POS 8
//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT4_QUEUE_THRSHLD_MASK 0x700u

//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT4_CNT_THRSHLD_POS 16
//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT4_CNT_THRSHLD_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_STATUS_PORT4 Register LS_STATUS_PORT4 - Load Spreader Port Status Register
//! @{

//! Register Offset (relative)
#define LS_STATUS_PORT4 0x414
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_STATUS_PORT4 0xE6660414u

//! Register Reset Value
#define LS_STATUS_PORT4_RST 0x00002000u

//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT4_QUEUE_LEN_POS 7
//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT4_QUEUE_LEN_MASK 0x780u

//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT4_QUEUE_FULL_POS 12
//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT4_QUEUE_FULL_MASK 0x1000u

//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT4_QUEUE_EMPTY_POS 13
//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT4_QUEUE_EMPTY_MASK 0x2000u

//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT4_CNT_VAL_POS 16
//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT4_CNT_VAL_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_QDESC_DW0_0_PORT4 Register LS_QDESC_DW0_0_PORT4 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_0_PORT4 0x420
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_0_PORT4 0xE6660420u

//! Register Reset Value
#define LS_QDESC_DW0_0_PORT4_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT4_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT4_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_0_PORT4 Register LS_QDESC_DW1_0_PORT4 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_0_PORT4 0x424
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_0_PORT4 0xE6660424u

//! Register Reset Value
#define LS_QDESC_DW1_0_PORT4_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT4_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT4_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_0_PORT4 Register LS_QDESC_DW2_0_PORT4 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_0_PORT4 0x428
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_0_PORT4 0xE6660428u

//! Register Reset Value
#define LS_QDESC_DW2_0_PORT4_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT4_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT4_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_0_PORT4 Register LS_QDESC_DW3_0_PORT4 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_0_PORT4 0x42C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_0_PORT4 0xE666042Cu

//! Register Reset Value
#define LS_QDESC_DW3_0_PORT4_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT4_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT4_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_1_PORT4 Register LS_QDESC_DW0_1_PORT4 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_1_PORT4 0x430
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_1_PORT4 0xE6660430u

//! Register Reset Value
#define LS_QDESC_DW0_1_PORT4_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT4_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT4_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_1_PORT4 Register LS_QDESC_DW1_1_PORT4 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_1_PORT4 0x434
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_1_PORT4 0xE6660434u

//! Register Reset Value
#define LS_QDESC_DW1_1_PORT4_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT4_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT4_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_1_PORT4 Register LS_QDESC_DW2_1_PORT4 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_1_PORT4 0x438
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_1_PORT4 0xE6660438u

//! Register Reset Value
#define LS_QDESC_DW2_1_PORT4_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT4_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT4_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_1_PORT4 Register LS_QDESC_DW3_1_PORT4 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_1_PORT4 0x43C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_1_PORT4 0xE666043Cu

//! Register Reset Value
#define LS_QDESC_DW3_1_PORT4_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT4_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT4_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_2_PORT4 Register LS_QDESC_DW0_2_PORT4 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_2_PORT4 0x440
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_2_PORT4 0xE6660440u

//! Register Reset Value
#define LS_QDESC_DW0_2_PORT4_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT4_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT4_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_2_PORT4 Register LS_QDESC_DW1_2_PORT4 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_2_PORT4 0x444
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_2_PORT4 0xE6660444u

//! Register Reset Value
#define LS_QDESC_DW1_2_PORT4_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT4_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT4_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_2_PORT4 Register LS_QDESC_DW2_2_PORT4 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_2_PORT4 0x448
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_2_PORT4 0xE6660448u

//! Register Reset Value
#define LS_QDESC_DW2_2_PORT4_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT4_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT4_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_2_PORT4 Register LS_QDESC_DW3_2_PORT4 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_2_PORT4 0x44C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_2_PORT4 0xE666044Cu

//! Register Reset Value
#define LS_QDESC_DW3_2_PORT4_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT4_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT4_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_3_PORT4 Register LS_QDESC_DW0_3_PORT4 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_3_PORT4 0x450
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_3_PORT4 0xE6660450u

//! Register Reset Value
#define LS_QDESC_DW0_3_PORT4_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT4_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT4_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_3_PORT4 Register LS_QDESC_DW1_3_PORT4 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_3_PORT4 0x454
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_3_PORT4 0xE6660454u

//! Register Reset Value
#define LS_QDESC_DW1_3_PORT4_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT4_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT4_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_3_PORT4 Register LS_QDESC_DW2_3_PORT4 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_3_PORT4 0x458
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_3_PORT4 0xE6660458u

//! Register Reset Value
#define LS_QDESC_DW2_3_PORT4_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT4_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT4_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_3_PORT4 Register LS_QDESC_DW3_3_PORT4 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_3_PORT4 0x45C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_3_PORT4 0xE666045Cu

//! Register Reset Value
#define LS_QDESC_DW3_3_PORT4_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT4_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT4_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_4_PORT4 Register LS_QDESC_DW0_4_PORT4 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_4_PORT4 0x460
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_4_PORT4 0xE6660460u

//! Register Reset Value
#define LS_QDESC_DW0_4_PORT4_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT4_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT4_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_4_PORT4 Register LS_QDESC_DW1_4_PORT4 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_4_PORT4 0x464
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_4_PORT4 0xE6660464u

//! Register Reset Value
#define LS_QDESC_DW1_4_PORT4_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT4_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT4_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_4_PORT4 Register LS_QDESC_DW2_4_PORT4 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_4_PORT4 0x468
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_4_PORT4 0xE6660468u

//! Register Reset Value
#define LS_QDESC_DW2_4_PORT4_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT4_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT4_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_4_PORT4 Register LS_QDESC_DW3_4_PORT4 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_4_PORT4 0x46C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_4_PORT4 0xE666046Cu

//! Register Reset Value
#define LS_QDESC_DW3_4_PORT4_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT4_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT4_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_5_PORT4 Register LS_QDESC_DW0_5_PORT4 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_5_PORT4 0x470
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_5_PORT4 0xE6660470u

//! Register Reset Value
#define LS_QDESC_DW0_5_PORT4_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT4_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT4_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_5_PORT4 Register LS_QDESC_DW1_5_PORT4 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_5_PORT4 0x474
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_5_PORT4 0xE6660474u

//! Register Reset Value
#define LS_QDESC_DW1_5_PORT4_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT4_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT4_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_5_PORT4 Register LS_QDESC_DW2_5_PORT4 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_5_PORT4 0x478
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_5_PORT4 0xE6660478u

//! Register Reset Value
#define LS_QDESC_DW2_5_PORT4_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT4_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT4_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_5_PORT4 Register LS_QDESC_DW3_5_PORT4 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_5_PORT4 0x47C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_5_PORT4 0xE666047Cu

//! Register Reset Value
#define LS_QDESC_DW3_5_PORT4_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT4_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT4_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_6_PORT4 Register LS_QDESC_DW0_6_PORT4 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_6_PORT4 0x480
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_6_PORT4 0xE6660480u

//! Register Reset Value
#define LS_QDESC_DW0_6_PORT4_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT4_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT4_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_6_PORT4 Register LS_QDESC_DW1_6_PORT4 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_6_PORT4 0x484
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_6_PORT4 0xE6660484u

//! Register Reset Value
#define LS_QDESC_DW1_6_PORT4_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT4_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT4_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_6_PORT4 Register LS_QDESC_DW2_6_PORT4 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_6_PORT4 0x488
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_6_PORT4 0xE6660488u

//! Register Reset Value
#define LS_QDESC_DW2_6_PORT4_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT4_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT4_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_6_PORT4 Register LS_QDESC_DW3_6_PORT4 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_6_PORT4 0x48C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_6_PORT4 0xE666048Cu

//! Register Reset Value
#define LS_QDESC_DW3_6_PORT4_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT4_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT4_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW0_PORT5 Register LS_DESC_DW0_PORT5 - CQM Load Spreader Module Register Description
//! @{

//! Register Offset (relative)
#define LS_DESC_DW0_PORT5 0x500
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW0_PORT5 0xE6660500u

//! Register Reset Value
#define LS_DESC_DW0_PORT5_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT5_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT5_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW1_PORT5 Register LS_DESC_DW1_PORT5 - Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_DESC_DW1_PORT5 0x504
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW1_PORT5 0xE6660504u

//! Register Reset Value
#define LS_DESC_DW1_PORT5_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT5_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT5_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW2_PORT5 Register LS_DESC_DW2_PORT5 - Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_DESC_DW2_PORT5 0x508
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW2_PORT5 0xE6660508u

//! Register Reset Value
#define LS_DESC_DW2_PORT5_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT5_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT5_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW3_PORT5 Register LS_DESC_DW3_PORT5 - Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_DESC_DW3_PORT5 0x50C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW3_PORT5 0xE666050Cu

//! Register Reset Value
#define LS_DESC_DW3_PORT5_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT5_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT5_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_CTRL_PORT5 Register LS_CTRL_PORT5 - Load Spreader Port Control Register
//! @{

//! Register Offset (relative)
#define LS_CTRL_PORT5 0x510
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_CTRL_PORT5 0xE6660510u

//! Register Reset Value
#define LS_CTRL_PORT5_RST 0x00000400u

//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT5_PORT_EN_POS 0
//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT5_PORT_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT5_PORT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT5_PORT_EN_EN 0x1

//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT5_SPR_EN_POS 1
//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT5_SPR_EN_MASK 0x2u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT5_SPR_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT5_SPR_EN_EN 0x1

//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT5_INT_EN_POS 2
//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT5_INT_EN_MASK 0x4u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT5_INT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT5_INT_EN_EN 0x1

//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT5_CNT_EN_POS 3
//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT5_CNT_EN_MASK 0x8u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT5_CNT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT5_CNT_EN_EN 0x1

//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT5_QUEUE_THRSHLD_POS 8
//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT5_QUEUE_THRSHLD_MASK 0x700u

//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT5_CNT_THRSHLD_POS 16
//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT5_CNT_THRSHLD_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_STATUS_PORT5 Register LS_STATUS_PORT5 - Load Spreader Port Status Register
//! @{

//! Register Offset (relative)
#define LS_STATUS_PORT5 0x514
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_STATUS_PORT5 0xE6660514u

//! Register Reset Value
#define LS_STATUS_PORT5_RST 0x00002000u

//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT5_QUEUE_LEN_POS 7
//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT5_QUEUE_LEN_MASK 0x780u

//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT5_QUEUE_FULL_POS 12
//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT5_QUEUE_FULL_MASK 0x1000u

//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT5_QUEUE_EMPTY_POS 13
//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT5_QUEUE_EMPTY_MASK 0x2000u

//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT5_CNT_VAL_POS 16
//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT5_CNT_VAL_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_QDESC_DW0_0_PORT5 Register LS_QDESC_DW0_0_PORT5 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_0_PORT5 0x520
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_0_PORT5 0xE6660520u

//! Register Reset Value
#define LS_QDESC_DW0_0_PORT5_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT5_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT5_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_0_PORT5 Register LS_QDESC_DW1_0_PORT5 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_0_PORT5 0x524
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_0_PORT5 0xE6660524u

//! Register Reset Value
#define LS_QDESC_DW1_0_PORT5_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT5_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT5_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_0_PORT5 Register LS_QDESC_DW2_0_PORT5 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_0_PORT5 0x528
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_0_PORT5 0xE6660528u

//! Register Reset Value
#define LS_QDESC_DW2_0_PORT5_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT5_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT5_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_0_PORT5 Register LS_QDESC_DW3_0_PORT5 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_0_PORT5 0x52C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_0_PORT5 0xE666052Cu

//! Register Reset Value
#define LS_QDESC_DW3_0_PORT5_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT5_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT5_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_1_PORT5 Register LS_QDESC_DW0_1_PORT5 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_1_PORT5 0x530
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_1_PORT5 0xE6660530u

//! Register Reset Value
#define LS_QDESC_DW0_1_PORT5_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT5_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT5_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_1_PORT5 Register LS_QDESC_DW1_1_PORT5 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_1_PORT5 0x534
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_1_PORT5 0xE6660534u

//! Register Reset Value
#define LS_QDESC_DW1_1_PORT5_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT5_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT5_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_1_PORT5 Register LS_QDESC_DW2_1_PORT5 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_1_PORT5 0x538
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_1_PORT5 0xE6660538u

//! Register Reset Value
#define LS_QDESC_DW2_1_PORT5_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT5_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT5_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_1_PORT5 Register LS_QDESC_DW3_1_PORT5 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_1_PORT5 0x53C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_1_PORT5 0xE666053Cu

//! Register Reset Value
#define LS_QDESC_DW3_1_PORT5_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT5_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT5_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_2_PORT5 Register LS_QDESC_DW0_2_PORT5 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_2_PORT5 0x540
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_2_PORT5 0xE6660540u

//! Register Reset Value
#define LS_QDESC_DW0_2_PORT5_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT5_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT5_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_2_PORT5 Register LS_QDESC_DW1_2_PORT5 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_2_PORT5 0x544
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_2_PORT5 0xE6660544u

//! Register Reset Value
#define LS_QDESC_DW1_2_PORT5_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT5_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT5_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_2_PORT5 Register LS_QDESC_DW2_2_PORT5 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_2_PORT5 0x548
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_2_PORT5 0xE6660548u

//! Register Reset Value
#define LS_QDESC_DW2_2_PORT5_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT5_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT5_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_2_PORT5 Register LS_QDESC_DW3_2_PORT5 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_2_PORT5 0x54C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_2_PORT5 0xE666054Cu

//! Register Reset Value
#define LS_QDESC_DW3_2_PORT5_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT5_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT5_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_3_PORT5 Register LS_QDESC_DW0_3_PORT5 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_3_PORT5 0x550
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_3_PORT5 0xE6660550u

//! Register Reset Value
#define LS_QDESC_DW0_3_PORT5_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT5_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT5_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_3_PORT5 Register LS_QDESC_DW1_3_PORT5 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_3_PORT5 0x554
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_3_PORT5 0xE6660554u

//! Register Reset Value
#define LS_QDESC_DW1_3_PORT5_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT5_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT5_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_3_PORT5 Register LS_QDESC_DW2_3_PORT5 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_3_PORT5 0x558
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_3_PORT5 0xE6660558u

//! Register Reset Value
#define LS_QDESC_DW2_3_PORT5_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT5_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT5_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_3_PORT5 Register LS_QDESC_DW3_3_PORT5 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_3_PORT5 0x55C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_3_PORT5 0xE666055Cu

//! Register Reset Value
#define LS_QDESC_DW3_3_PORT5_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT5_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT5_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_4_PORT5 Register LS_QDESC_DW0_4_PORT5 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_4_PORT5 0x560
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_4_PORT5 0xE6660560u

//! Register Reset Value
#define LS_QDESC_DW0_4_PORT5_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT5_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT5_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_4_PORT5 Register LS_QDESC_DW1_4_PORT5 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_4_PORT5 0x564
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_4_PORT5 0xE6660564u

//! Register Reset Value
#define LS_QDESC_DW1_4_PORT5_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT5_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT5_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_4_PORT5 Register LS_QDESC_DW2_4_PORT5 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_4_PORT5 0x568
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_4_PORT5 0xE6660568u

//! Register Reset Value
#define LS_QDESC_DW2_4_PORT5_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT5_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT5_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_4_PORT5 Register LS_QDESC_DW3_4_PORT5 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_4_PORT5 0x56C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_4_PORT5 0xE666056Cu

//! Register Reset Value
#define LS_QDESC_DW3_4_PORT5_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT5_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT5_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_5_PORT5 Register LS_QDESC_DW0_5_PORT5 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_5_PORT5 0x570
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_5_PORT5 0xE6660570u

//! Register Reset Value
#define LS_QDESC_DW0_5_PORT5_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT5_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT5_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_5_PORT5 Register LS_QDESC_DW1_5_PORT5 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_5_PORT5 0x574
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_5_PORT5 0xE6660574u

//! Register Reset Value
#define LS_QDESC_DW1_5_PORT5_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT5_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT5_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_5_PORT5 Register LS_QDESC_DW2_5_PORT5 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_5_PORT5 0x578
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_5_PORT5 0xE6660578u

//! Register Reset Value
#define LS_QDESC_DW2_5_PORT5_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT5_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT5_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_5_PORT5 Register LS_QDESC_DW3_5_PORT5 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_5_PORT5 0x57C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_5_PORT5 0xE666057Cu

//! Register Reset Value
#define LS_QDESC_DW3_5_PORT5_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT5_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT5_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_6_PORT5 Register LS_QDESC_DW0_6_PORT5 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_6_PORT5 0x580
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_6_PORT5 0xE6660580u

//! Register Reset Value
#define LS_QDESC_DW0_6_PORT5_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT5_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT5_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_6_PORT5 Register LS_QDESC_DW1_6_PORT5 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_6_PORT5 0x584
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_6_PORT5 0xE6660584u

//! Register Reset Value
#define LS_QDESC_DW1_6_PORT5_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT5_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT5_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_6_PORT5 Register LS_QDESC_DW2_6_PORT5 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_6_PORT5 0x588
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_6_PORT5 0xE6660588u

//! Register Reset Value
#define LS_QDESC_DW2_6_PORT5_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT5_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT5_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_6_PORT5 Register LS_QDESC_DW3_6_PORT5 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_6_PORT5 0x58C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_6_PORT5 0xE666058Cu

//! Register Reset Value
#define LS_QDESC_DW3_6_PORT5_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT5_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT5_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW0_PORT6 Register LS_DESC_DW0_PORT6 - CQM Load Spreader Module Register Description
//! @{

//! Register Offset (relative)
#define LS_DESC_DW0_PORT6 0x600
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW0_PORT6 0xE6660600u

//! Register Reset Value
#define LS_DESC_DW0_PORT6_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT6_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT6_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW1_PORT6 Register LS_DESC_DW1_PORT6 - Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_DESC_DW1_PORT6 0x604
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW1_PORT6 0xE6660604u

//! Register Reset Value
#define LS_DESC_DW1_PORT6_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT6_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT6_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW2_PORT6 Register LS_DESC_DW2_PORT6 - Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_DESC_DW2_PORT6 0x608
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW2_PORT6 0xE6660608u

//! Register Reset Value
#define LS_DESC_DW2_PORT6_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT6_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT6_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW3_PORT6 Register LS_DESC_DW3_PORT6 - Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_DESC_DW3_PORT6 0x60C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW3_PORT6 0xE666060Cu

//! Register Reset Value
#define LS_DESC_DW3_PORT6_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT6_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT6_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_CTRL_PORT6 Register LS_CTRL_PORT6 - Load Spreader Port Control Register
//! @{

//! Register Offset (relative)
#define LS_CTRL_PORT6 0x610
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_CTRL_PORT6 0xE6660610u

//! Register Reset Value
#define LS_CTRL_PORT6_RST 0x00000400u

//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT6_PORT_EN_POS 0
//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT6_PORT_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT6_PORT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT6_PORT_EN_EN 0x1

//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT6_SPR_EN_POS 1
//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT6_SPR_EN_MASK 0x2u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT6_SPR_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT6_SPR_EN_EN 0x1

//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT6_INT_EN_POS 2
//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT6_INT_EN_MASK 0x4u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT6_INT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT6_INT_EN_EN 0x1

//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT6_CNT_EN_POS 3
//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT6_CNT_EN_MASK 0x8u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT6_CNT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT6_CNT_EN_EN 0x1

//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT6_QUEUE_THRSHLD_POS 8
//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT6_QUEUE_THRSHLD_MASK 0x700u

//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT6_CNT_THRSHLD_POS 16
//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT6_CNT_THRSHLD_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_STATUS_PORT6 Register LS_STATUS_PORT6 - Load Spreader Port Status Register
//! @{

//! Register Offset (relative)
#define LS_STATUS_PORT6 0x614
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_STATUS_PORT6 0xE6660614u

//! Register Reset Value
#define LS_STATUS_PORT6_RST 0x00002000u

//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT6_QUEUE_LEN_POS 7
//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT6_QUEUE_LEN_MASK 0x780u

//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT6_QUEUE_FULL_POS 12
//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT6_QUEUE_FULL_MASK 0x1000u

//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT6_QUEUE_EMPTY_POS 13
//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT6_QUEUE_EMPTY_MASK 0x2000u

//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT6_CNT_VAL_POS 16
//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT6_CNT_VAL_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_QDESC_DW0_0_PORT6 Register LS_QDESC_DW0_0_PORT6 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_0_PORT6 0x620
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_0_PORT6 0xE6660620u

//! Register Reset Value
#define LS_QDESC_DW0_0_PORT6_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT6_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT6_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_0_PORT6 Register LS_QDESC_DW1_0_PORT6 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_0_PORT6 0x624
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_0_PORT6 0xE6660624u

//! Register Reset Value
#define LS_QDESC_DW1_0_PORT6_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT6_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT6_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_0_PORT6 Register LS_QDESC_DW2_0_PORT6 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_0_PORT6 0x628
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_0_PORT6 0xE6660628u

//! Register Reset Value
#define LS_QDESC_DW2_0_PORT6_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT6_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT6_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_0_PORT6 Register LS_QDESC_DW3_0_PORT6 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_0_PORT6 0x62C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_0_PORT6 0xE666062Cu

//! Register Reset Value
#define LS_QDESC_DW3_0_PORT6_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT6_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT6_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_1_PORT6 Register LS_QDESC_DW0_1_PORT6 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_1_PORT6 0x630
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_1_PORT6 0xE6660630u

//! Register Reset Value
#define LS_QDESC_DW0_1_PORT6_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT6_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT6_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_1_PORT6 Register LS_QDESC_DW1_1_PORT6 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_1_PORT6 0x634
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_1_PORT6 0xE6660634u

//! Register Reset Value
#define LS_QDESC_DW1_1_PORT6_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT6_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT6_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_1_PORT6 Register LS_QDESC_DW2_1_PORT6 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_1_PORT6 0x638
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_1_PORT6 0xE6660638u

//! Register Reset Value
#define LS_QDESC_DW2_1_PORT6_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT6_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT6_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_1_PORT6 Register LS_QDESC_DW3_1_PORT6 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_1_PORT6 0x63C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_1_PORT6 0xE666063Cu

//! Register Reset Value
#define LS_QDESC_DW3_1_PORT6_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT6_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT6_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_2_PORT6 Register LS_QDESC_DW0_2_PORT6 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_2_PORT6 0x640
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_2_PORT6 0xE6660640u

//! Register Reset Value
#define LS_QDESC_DW0_2_PORT6_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT6_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT6_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_2_PORT6 Register LS_QDESC_DW1_2_PORT6 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_2_PORT6 0x644
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_2_PORT6 0xE6660644u

//! Register Reset Value
#define LS_QDESC_DW1_2_PORT6_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT6_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT6_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_2_PORT6 Register LS_QDESC_DW2_2_PORT6 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_2_PORT6 0x648
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_2_PORT6 0xE6660648u

//! Register Reset Value
#define LS_QDESC_DW2_2_PORT6_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT6_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT6_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_2_PORT6 Register LS_QDESC_DW3_2_PORT6 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_2_PORT6 0x64C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_2_PORT6 0xE666064Cu

//! Register Reset Value
#define LS_QDESC_DW3_2_PORT6_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT6_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT6_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_3_PORT6 Register LS_QDESC_DW0_3_PORT6 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_3_PORT6 0x650
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_3_PORT6 0xE6660650u

//! Register Reset Value
#define LS_QDESC_DW0_3_PORT6_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT6_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT6_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_3_PORT6 Register LS_QDESC_DW1_3_PORT6 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_3_PORT6 0x654
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_3_PORT6 0xE6660654u

//! Register Reset Value
#define LS_QDESC_DW1_3_PORT6_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT6_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT6_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_3_PORT6 Register LS_QDESC_DW2_3_PORT6 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_3_PORT6 0x658
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_3_PORT6 0xE6660658u

//! Register Reset Value
#define LS_QDESC_DW2_3_PORT6_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT6_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT6_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_3_PORT6 Register LS_QDESC_DW3_3_PORT6 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_3_PORT6 0x65C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_3_PORT6 0xE666065Cu

//! Register Reset Value
#define LS_QDESC_DW3_3_PORT6_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT6_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT6_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_4_PORT6 Register LS_QDESC_DW0_4_PORT6 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_4_PORT6 0x660
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_4_PORT6 0xE6660660u

//! Register Reset Value
#define LS_QDESC_DW0_4_PORT6_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT6_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT6_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_4_PORT6 Register LS_QDESC_DW1_4_PORT6 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_4_PORT6 0x664
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_4_PORT6 0xE6660664u

//! Register Reset Value
#define LS_QDESC_DW1_4_PORT6_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT6_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT6_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_4_PORT6 Register LS_QDESC_DW2_4_PORT6 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_4_PORT6 0x668
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_4_PORT6 0xE6660668u

//! Register Reset Value
#define LS_QDESC_DW2_4_PORT6_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT6_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT6_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_4_PORT6 Register LS_QDESC_DW3_4_PORT6 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_4_PORT6 0x66C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_4_PORT6 0xE666066Cu

//! Register Reset Value
#define LS_QDESC_DW3_4_PORT6_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT6_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT6_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_5_PORT6 Register LS_QDESC_DW0_5_PORT6 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_5_PORT6 0x670
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_5_PORT6 0xE6660670u

//! Register Reset Value
#define LS_QDESC_DW0_5_PORT6_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT6_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT6_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_5_PORT6 Register LS_QDESC_DW1_5_PORT6 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_5_PORT6 0x674
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_5_PORT6 0xE6660674u

//! Register Reset Value
#define LS_QDESC_DW1_5_PORT6_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT6_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT6_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_5_PORT6 Register LS_QDESC_DW2_5_PORT6 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_5_PORT6 0x678
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_5_PORT6 0xE6660678u

//! Register Reset Value
#define LS_QDESC_DW2_5_PORT6_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT6_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT6_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_5_PORT6 Register LS_QDESC_DW3_5_PORT6 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_5_PORT6 0x67C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_5_PORT6 0xE666067Cu

//! Register Reset Value
#define LS_QDESC_DW3_5_PORT6_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT6_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT6_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_6_PORT6 Register LS_QDESC_DW0_6_PORT6 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_6_PORT6 0x680
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_6_PORT6 0xE6660680u

//! Register Reset Value
#define LS_QDESC_DW0_6_PORT6_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT6_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT6_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_6_PORT6 Register LS_QDESC_DW1_6_PORT6 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_6_PORT6 0x684
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_6_PORT6 0xE6660684u

//! Register Reset Value
#define LS_QDESC_DW1_6_PORT6_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT6_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT6_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_6_PORT6 Register LS_QDESC_DW2_6_PORT6 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_6_PORT6 0x688
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_6_PORT6 0xE6660688u

//! Register Reset Value
#define LS_QDESC_DW2_6_PORT6_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT6_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT6_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_6_PORT6 Register LS_QDESC_DW3_6_PORT6 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_6_PORT6 0x68C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_6_PORT6 0xE666068Cu

//! Register Reset Value
#define LS_QDESC_DW3_6_PORT6_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT6_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT6_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW0_PORT7 Register LS_DESC_DW0_PORT7 - CQM Load Spreader Module Register Description
//! @{

//! Register Offset (relative)
#define LS_DESC_DW0_PORT7 0x700
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW0_PORT7 0xE6660700u

//! Register Reset Value
#define LS_DESC_DW0_PORT7_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT7_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_DESC_DW0_PORT7_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW1_PORT7 Register LS_DESC_DW1_PORT7 - Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_DESC_DW1_PORT7 0x704
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW1_PORT7 0xE6660704u

//! Register Reset Value
#define LS_DESC_DW1_PORT7_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT7_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_DESC_DW1_PORT7_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW2_PORT7 Register LS_DESC_DW2_PORT7 - Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_DESC_DW2_PORT7 0x708
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW2_PORT7 0xE6660708u

//! Register Reset Value
#define LS_DESC_DW2_PORT7_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT7_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_DESC_DW2_PORT7_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_DESC_DW3_PORT7 Register LS_DESC_DW3_PORT7 - Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_DESC_DW3_PORT7 0x70C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_DESC_DW3_PORT7 0xE666070Cu

//! Register Reset Value
#define LS_DESC_DW3_PORT7_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT7_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_DESC_DW3_PORT7_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_CTRL_PORT7 Register LS_CTRL_PORT7 - Load Spreader Port Control Register
//! @{

//! Register Offset (relative)
#define LS_CTRL_PORT7 0x710
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_CTRL_PORT7 0xE6660710u

//! Register Reset Value
#define LS_CTRL_PORT7_RST 0x00000400u

//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT7_PORT_EN_POS 0
//! Field PORT_EN - Enable the Port on the Load Spreader
#define LS_CTRL_PORT7_PORT_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT7_PORT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT7_PORT_EN_EN 0x1

//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT7_SPR_EN_POS 1
//! Field SPR_EN - Spreading Enable
#define LS_CTRL_PORT7_SPR_EN_MASK 0x2u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT7_SPR_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT7_SPR_EN_EN 0x1

//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT7_INT_EN_POS 2
//! Field INT_EN - Interrupt Enable
#define LS_CTRL_PORT7_INT_EN_MASK 0x4u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT7_INT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT7_INT_EN_EN 0x1

//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT7_CNT_EN_POS 3
//! Field CNT_EN - Counter Enable
#define LS_CTRL_PORT7_CNT_EN_MASK 0x8u
//! Constant DIS - Disable
#define CONST_LS_CTRL_PORT7_CNT_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_CTRL_PORT7_CNT_EN_EN 0x1

//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT7_QUEUE_THRSHLD_POS 8
//! Field QUEUE_THRSHLD - Queue Threshold Level
#define LS_CTRL_PORT7_QUEUE_THRSHLD_MASK 0x700u

//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT7_CNT_THRSHLD_POS 16
//! Field CNT_THRSHLD - Overflow Counter Threshold Level
#define LS_CTRL_PORT7_CNT_THRSHLD_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_STATUS_PORT7 Register LS_STATUS_PORT7 - Load Spreader Port Status Register
//! @{

//! Register Offset (relative)
#define LS_STATUS_PORT7 0x714
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_STATUS_PORT7 0xE6660714u

//! Register Reset Value
#define LS_STATUS_PORT7_RST 0x00002000u

//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT7_QUEUE_LEN_POS 7
//! Field QUEUE_LEN - Current Queue Length
#define LS_STATUS_PORT7_QUEUE_LEN_MASK 0x780u

//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT7_QUEUE_FULL_POS 12
//! Field QUEUE_FULL - Queue Full
#define LS_STATUS_PORT7_QUEUE_FULL_MASK 0x1000u

//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT7_QUEUE_EMPTY_POS 13
//! Field QUEUE_EMPTY - Queue Empty
#define LS_STATUS_PORT7_QUEUE_EMPTY_MASK 0x2000u

//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT7_CNT_VAL_POS 16
//! Field CNT_VAL - Counter Value
#define LS_STATUS_PORT7_CNT_VAL_MASK 0xFFFF0000u

//! @}

//! \defgroup LS_QDESC_DW0_0_PORT7 Register LS_QDESC_DW0_0_PORT7 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_0_PORT7 0x720
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_0_PORT7 0xE6660720u

//! Register Reset Value
#define LS_QDESC_DW0_0_PORT7_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT7_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_0_PORT7_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_0_PORT7 Register LS_QDESC_DW1_0_PORT7 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_0_PORT7 0x724
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_0_PORT7 0xE6660724u

//! Register Reset Value
#define LS_QDESC_DW1_0_PORT7_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT7_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_0_PORT7_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_0_PORT7 Register LS_QDESC_DW2_0_PORT7 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_0_PORT7 0x728
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_0_PORT7 0xE6660728u

//! Register Reset Value
#define LS_QDESC_DW2_0_PORT7_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT7_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_0_PORT7_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_0_PORT7 Register LS_QDESC_DW3_0_PORT7 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_0_PORT7 0x72C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_0_PORT7 0xE666072Cu

//! Register Reset Value
#define LS_QDESC_DW3_0_PORT7_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT7_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_0_PORT7_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_1_PORT7 Register LS_QDESC_DW0_1_PORT7 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_1_PORT7 0x730
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_1_PORT7 0xE6660730u

//! Register Reset Value
#define LS_QDESC_DW0_1_PORT7_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT7_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_1_PORT7_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_1_PORT7 Register LS_QDESC_DW1_1_PORT7 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_1_PORT7 0x734
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_1_PORT7 0xE6660734u

//! Register Reset Value
#define LS_QDESC_DW1_1_PORT7_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT7_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_1_PORT7_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_1_PORT7 Register LS_QDESC_DW2_1_PORT7 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_1_PORT7 0x738
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_1_PORT7 0xE6660738u

//! Register Reset Value
#define LS_QDESC_DW2_1_PORT7_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT7_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_1_PORT7_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_1_PORT7 Register LS_QDESC_DW3_1_PORT7 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_1_PORT7 0x73C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_1_PORT7 0xE666073Cu

//! Register Reset Value
#define LS_QDESC_DW3_1_PORT7_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT7_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_1_PORT7_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_2_PORT7 Register LS_QDESC_DW0_2_PORT7 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_2_PORT7 0x740
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_2_PORT7 0xE6660740u

//! Register Reset Value
#define LS_QDESC_DW0_2_PORT7_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT7_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_2_PORT7_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_2_PORT7 Register LS_QDESC_DW1_2_PORT7 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_2_PORT7 0x744
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_2_PORT7 0xE6660744u

//! Register Reset Value
#define LS_QDESC_DW1_2_PORT7_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT7_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_2_PORT7_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_2_PORT7 Register LS_QDESC_DW2_2_PORT7 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_2_PORT7 0x748
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_2_PORT7 0xE6660748u

//! Register Reset Value
#define LS_QDESC_DW2_2_PORT7_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT7_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_2_PORT7_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_2_PORT7 Register LS_QDESC_DW3_2_PORT7 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_2_PORT7 0x74C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_2_PORT7 0xE666074Cu

//! Register Reset Value
#define LS_QDESC_DW3_2_PORT7_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT7_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_2_PORT7_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_3_PORT7 Register LS_QDESC_DW0_3_PORT7 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_3_PORT7 0x750
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_3_PORT7 0xE6660750u

//! Register Reset Value
#define LS_QDESC_DW0_3_PORT7_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT7_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_3_PORT7_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_3_PORT7 Register LS_QDESC_DW1_3_PORT7 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_3_PORT7 0x754
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_3_PORT7 0xE6660754u

//! Register Reset Value
#define LS_QDESC_DW1_3_PORT7_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT7_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_3_PORT7_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_3_PORT7 Register LS_QDESC_DW2_3_PORT7 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_3_PORT7 0x758
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_3_PORT7 0xE6660758u

//! Register Reset Value
#define LS_QDESC_DW2_3_PORT7_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT7_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_3_PORT7_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_3_PORT7 Register LS_QDESC_DW3_3_PORT7 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_3_PORT7 0x75C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_3_PORT7 0xE666075Cu

//! Register Reset Value
#define LS_QDESC_DW3_3_PORT7_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT7_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_3_PORT7_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_4_PORT7 Register LS_QDESC_DW0_4_PORT7 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_4_PORT7 0x760
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_4_PORT7 0xE6660760u

//! Register Reset Value
#define LS_QDESC_DW0_4_PORT7_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT7_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_4_PORT7_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_4_PORT7 Register LS_QDESC_DW1_4_PORT7 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_4_PORT7 0x764
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_4_PORT7 0xE6660764u

//! Register Reset Value
#define LS_QDESC_DW1_4_PORT7_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT7_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_4_PORT7_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_4_PORT7 Register LS_QDESC_DW2_4_PORT7 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_4_PORT7 0x768
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_4_PORT7 0xE6660768u

//! Register Reset Value
#define LS_QDESC_DW2_4_PORT7_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT7_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_4_PORT7_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_4_PORT7 Register LS_QDESC_DW3_4_PORT7 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_4_PORT7 0x76C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_4_PORT7 0xE666076Cu

//! Register Reset Value
#define LS_QDESC_DW3_4_PORT7_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT7_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_4_PORT7_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_5_PORT7 Register LS_QDESC_DW0_5_PORT7 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_5_PORT7 0x770
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_5_PORT7 0xE6660770u

//! Register Reset Value
#define LS_QDESC_DW0_5_PORT7_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT7_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_5_PORT7_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_5_PORT7 Register LS_QDESC_DW1_5_PORT7 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_5_PORT7 0x774
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_5_PORT7 0xE6660774u

//! Register Reset Value
#define LS_QDESC_DW1_5_PORT7_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT7_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_5_PORT7_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_5_PORT7 Register LS_QDESC_DW2_5_PORT7 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_5_PORT7 0x778
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_5_PORT7 0xE6660778u

//! Register Reset Value
#define LS_QDESC_DW2_5_PORT7_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT7_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_5_PORT7_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_5_PORT7 Register LS_QDESC_DW3_5_PORT7 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_5_PORT7 0x77C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_5_PORT7 0xE666077Cu

//! Register Reset Value
#define LS_QDESC_DW3_5_PORT7_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT7_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_5_PORT7_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW0_6_PORT7 Register LS_QDESC_DW0_6_PORT7 - Queue Descriptor Register DW0
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW0_6_PORT7 0x780
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW0_6_PORT7 0xE6660780u

//! Register Reset Value
#define LS_QDESC_DW0_6_PORT7_RST 0x00000000u

//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT7_DESC_DW0_POS 0
//! Field DESC_DW0 - Descriptor DW0
#define LS_QDESC_DW0_6_PORT7_DESC_DW0_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW1_6_PORT7 Register LS_QDESC_DW1_6_PORT7 - Queue Descriptor Register DW1
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW1_6_PORT7 0x784
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW1_6_PORT7 0xE6660784u

//! Register Reset Value
#define LS_QDESC_DW1_6_PORT7_RST 0x00000000u

//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT7_DESC_DW1_POS 0
//! Field DESC_DW1 - Descriptor DW1
#define LS_QDESC_DW1_6_PORT7_DESC_DW1_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW2_6_PORT7 Register LS_QDESC_DW2_6_PORT7 - Queue Descriptor Register DW2
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW2_6_PORT7 0x788
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW2_6_PORT7 0xE6660788u

//! Register Reset Value
#define LS_QDESC_DW2_6_PORT7_RST 0x00000000u

//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT7_DESC_DW2_POS 0
//! Field DESC_DW2 - Descriptor DW2
#define LS_QDESC_DW2_6_PORT7_DESC_DW2_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_QDESC_DW3_6_PORT7 Register LS_QDESC_DW3_6_PORT7 - Queue Descriptor Register DW3
//! @{

//! Register Offset (relative)
#define LS_QDESC_DW3_6_PORT7 0x78C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_QDESC_DW3_6_PORT7 0xE666078Cu

//! Register Reset Value
#define LS_QDESC_DW3_6_PORT7_RST 0x00000000u

//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT7_DESC_DW3_POS 0
//! Field DESC_DW3 - Descriptor DW3
#define LS_QDESC_DW3_6_PORT7_DESC_DW3_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_GLBL_CTRL Register LS_GLBL_CTRL - Load Spreader Global Control Register
//! @{

//! Register Offset (relative)
#define LS_GLBL_CTRL 0x900
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_GLBL_CTRL 0xE6660900u

//! Register Reset Value
#define LS_GLBL_CTRL_RST 0x00000000u

//! Field EN - Load Spreader Enable
#define LS_GLBL_CTRL_EN_POS 0
//! Field EN - Load Spreader Enable
#define LS_GLBL_CTRL_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_LS_GLBL_CTRL_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LS_GLBL_CTRL_EN_EN 0x1

//! Field CLK_DIV - Overflow counter clock divider
#define LS_GLBL_CTRL_CLK_DIV_POS 12
//! Field CLK_DIV - Overflow counter clock divider
#define LS_GLBL_CTRL_CLK_DIV_MASK 0x3000u
//! Constant DIV1 - No divider used
#define CONST_LS_GLBL_CTRL_CLK_DIV_DIV1 0x0
//! Constant DIV2 - Divide by 2
#define CONST_LS_GLBL_CTRL_CLK_DIV_DIV2 0x1
//! Constant DIV4 - Divide by 4
#define CONST_LS_GLBL_CTRL_CLK_DIV_DIV4 0x2
//! Constant DIV8 - Divide by 8
#define CONST_LS_GLBL_CTRL_CLK_DIV_DIV8 0x3

//! @}

//! \defgroup LS_SPR_CTRL Register LS_SPR_CTRL - Load Spreader Spreading Register
//! @{

//! Register Offset (relative)
#define LS_SPR_CTRL 0x904
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_SPR_CTRL 0xE6660904u

//! Register Reset Value
#define LS_SPR_CTRL_RST 0x00000000u

//! Field SPR_SEL - Spreading Select
#define LS_SPR_CTRL_SPR_SEL_POS 0
//! Field SPR_SEL - Spreading Select
#define LS_SPR_CTRL_SPR_SEL_MASK 0x1u
//! Constant WRR - Weighted Round Robin
#define CONST_LS_SPR_CTRL_SPR_SEL_WRR 0x0
//! Constant FLOWID - Flow ID mapping based spreading
#define CONST_LS_SPR_CTRL_SPR_SEL_FLOWID 0x1

//! Field WP0 - Port0 weight
#define LS_SPR_CTRL_WP0_POS 16
//! Field WP0 - Port0 weight
#define LS_SPR_CTRL_WP0_MASK 0x30000u

//! Field WP1 - Port1 weight
#define LS_SPR_CTRL_WP1_POS 18
//! Field WP1 - Port1 weight
#define LS_SPR_CTRL_WP1_MASK 0xC0000u

//! Field WP2 - Port2 weight
#define LS_SPR_CTRL_WP2_POS 20
//! Field WP2 - Port2 weight
#define LS_SPR_CTRL_WP2_MASK 0x300000u

//! Field WP3 - Port3 weight
#define LS_SPR_CTRL_WP3_POS 22
//! Field WP3 - Port3 weight
#define LS_SPR_CTRL_WP3_MASK 0xC00000u

//! Field WP4 - Port4 weight
#define LS_SPR_CTRL_WP4_POS 24
//! Field WP4 - Port4 weight
#define LS_SPR_CTRL_WP4_MASK 0x3000000u

//! Field WP5 - Port5 weight
#define LS_SPR_CTRL_WP5_POS 26
//! Field WP5 - Port5 weight
#define LS_SPR_CTRL_WP5_MASK 0xC000000u

//! Field WP6 - Port6 weight
#define LS_SPR_CTRL_WP6_POS 28
//! Field WP6 - Port6 weight
#define LS_SPR_CTRL_WP6_MASK 0x30000000u

//! Field WP7 - Port7 weight
#define LS_SPR_CTRL_WP7_POS 30
//! Field WP7 - Port7 weight
#define LS_SPR_CTRL_WP7_MASK 0xC0000000u

//! @}

//! \defgroup IRNCR_LS Register IRNCR_LS - Load Spreader IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_LS 0x910
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_IRNCR_LS 0xE6660910u

//! Register Reset Value
#define IRNCR_LS_RST 0x00000000u

//! Field INT_PORT0 - PORT0 Interrupt
#define IRNCR_LS_INT_PORT0_POS 16
//! Field INT_PORT0 - PORT0 Interrupt
#define IRNCR_LS_INT_PORT0_MASK 0x10000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_INT_PORT0_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_INT_PORT0_INTACK 0x1

//! Field CNT_PORT0 - PORT0 Counter Overflow
#define IRNCR_LS_CNT_PORT0_POS 17
//! Field CNT_PORT0 - PORT0 Counter Overflow
#define IRNCR_LS_CNT_PORT0_MASK 0x20000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_CNT_PORT0_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_CNT_PORT0_INTACK 0x1

//! Field INT_PORT1 - PORT1 Interrupt
#define IRNCR_LS_INT_PORT1_POS 18
//! Field INT_PORT1 - PORT1 Interrupt
#define IRNCR_LS_INT_PORT1_MASK 0x40000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_INT_PORT1_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_INT_PORT1_INTACK 0x1

//! Field CNT_PORT1 - PORT1 Counter Overflow
#define IRNCR_LS_CNT_PORT1_POS 19
//! Field CNT_PORT1 - PORT1 Counter Overflow
#define IRNCR_LS_CNT_PORT1_MASK 0x80000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_CNT_PORT1_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_CNT_PORT1_INTACK 0x1

//! Field INT_PORT2 - PORT2 Interrupt
#define IRNCR_LS_INT_PORT2_POS 20
//! Field INT_PORT2 - PORT2 Interrupt
#define IRNCR_LS_INT_PORT2_MASK 0x100000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_INT_PORT2_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_INT_PORT2_INTACK 0x1

//! Field CNT_PORT2 - PORT2 Counter Overflow
#define IRNCR_LS_CNT_PORT2_POS 21
//! Field CNT_PORT2 - PORT2 Counter Overflow
#define IRNCR_LS_CNT_PORT2_MASK 0x200000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_CNT_PORT2_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_CNT_PORT2_INTACK 0x1

//! Field INT_PORT3 - PORT3 Interrupt
#define IRNCR_LS_INT_PORT3_POS 22
//! Field INT_PORT3 - PORT3 Interrupt
#define IRNCR_LS_INT_PORT3_MASK 0x400000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_INT_PORT3_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_INT_PORT3_INTACK 0x1

//! Field CNT_PORT3 - PORT3 Counter Overflow
#define IRNCR_LS_CNT_PORT3_POS 23
//! Field CNT_PORT3 - PORT3 Counter Overflow
#define IRNCR_LS_CNT_PORT3_MASK 0x800000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_CNT_PORT3_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_CNT_PORT3_INTACK 0x1

//! Field INT_PORT4 - PORT4 Interrupt
#define IRNCR_LS_INT_PORT4_POS 24
//! Field INT_PORT4 - PORT4 Interrupt
#define IRNCR_LS_INT_PORT4_MASK 0x1000000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_INT_PORT4_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_INT_PORT4_INTACK 0x1

//! Field CNT_PORT4 - PORT4 Counter Overflow
#define IRNCR_LS_CNT_PORT4_POS 25
//! Field CNT_PORT4 - PORT4 Counter Overflow
#define IRNCR_LS_CNT_PORT4_MASK 0x2000000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_CNT_PORT4_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_CNT_PORT4_INTACK 0x1

//! Field INT_PORT5 - PORT5 Interrupt
#define IRNCR_LS_INT_PORT5_POS 26
//! Field INT_PORT5 - PORT5 Interrupt
#define IRNCR_LS_INT_PORT5_MASK 0x4000000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_INT_PORT5_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_INT_PORT5_INTACK 0x1

//! Field CNT_PORT5 - PORT5 Counter Overflow
#define IRNCR_LS_CNT_PORT5_POS 27
//! Field CNT_PORT5 - PORT5 Counter Overflow
#define IRNCR_LS_CNT_PORT5_MASK 0x8000000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_CNT_PORT5_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_CNT_PORT5_INTACK 0x1

//! Field INT_PORT6 - PORT6 Interrupt
#define IRNCR_LS_INT_PORT6_POS 28
//! Field INT_PORT6 - PORT6 Interrupt
#define IRNCR_LS_INT_PORT6_MASK 0x10000000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_INT_PORT6_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_INT_PORT6_INTACK 0x1

//! Field CNT_PORT6 - PORT6 Counter Overflow
#define IRNCR_LS_CNT_PORT6_POS 29
//! Field CNT_PORT6 - PORT6 Counter Overflow
#define IRNCR_LS_CNT_PORT6_MASK 0x20000000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_CNT_PORT6_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_CNT_PORT6_INTACK 0x1

//! Field INT_PORT7 - PORT7 Interrupt
#define IRNCR_LS_INT_PORT7_POS 30
//! Field INT_PORT7 - PORT7 Interrupt
#define IRNCR_LS_INT_PORT7_MASK 0x40000000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_INT_PORT7_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_INT_PORT7_INTACK 0x1

//! Field CNT_PORT7 - PORT7 Counter Overflow
#define IRNCR_LS_CNT_PORT7_POS 31
//! Field CNT_PORT7 - PORT7 Counter Overflow
#define IRNCR_LS_CNT_PORT7_MASK 0x80000000u
//! Constant NUL - NULL
#define CONST_IRNCR_LS_CNT_PORT7_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_LS_CNT_PORT7_INTACK 0x1

//! @}

//! \defgroup IRNICR_LS Register IRNICR_LS - Load Spreader IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_LS 0x914
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_IRNICR_LS 0xE6660914u

//! Register Reset Value
#define IRNICR_LS_RST 0x00000000u

//! Field INT_PORT0 - PORT0 Interrupt
#define IRNICR_LS_INT_PORT0_POS 16
//! Field INT_PORT0 - PORT0 Interrupt
#define IRNICR_LS_INT_PORT0_MASK 0x10000u

//! Field CNT_PORT0 - PORT0 Counter Overflow
#define IRNICR_LS_CNT_PORT0_POS 17
//! Field CNT_PORT0 - PORT0 Counter Overflow
#define IRNICR_LS_CNT_PORT0_MASK 0x20000u

//! Field INT_PORT1 - PORT1 Interrupt
#define IRNICR_LS_INT_PORT1_POS 18
//! Field INT_PORT1 - PORT1 Interrupt
#define IRNICR_LS_INT_PORT1_MASK 0x40000u

//! Field CNT_PORT1 - PORT1 Counter Overflow
#define IRNICR_LS_CNT_PORT1_POS 19
//! Field CNT_PORT1 - PORT1 Counter Overflow
#define IRNICR_LS_CNT_PORT1_MASK 0x80000u

//! Field INT_PORT2 - PORT2 Interrupt
#define IRNICR_LS_INT_PORT2_POS 20
//! Field INT_PORT2 - PORT2 Interrupt
#define IRNICR_LS_INT_PORT2_MASK 0x100000u

//! Field CNT_PORT2 - PORT2 Counter Overflow
#define IRNICR_LS_CNT_PORT2_POS 21
//! Field CNT_PORT2 - PORT2 Counter Overflow
#define IRNICR_LS_CNT_PORT2_MASK 0x200000u

//! Field INT_PORT3 - PORT3 Interrupt
#define IRNICR_LS_INT_PORT3_POS 22
//! Field INT_PORT3 - PORT3 Interrupt
#define IRNICR_LS_INT_PORT3_MASK 0x400000u

//! Field CNT_PORT3 - PORT3 Counter Overflow
#define IRNICR_LS_CNT_PORT3_POS 23
//! Field CNT_PORT3 - PORT3 Counter Overflow
#define IRNICR_LS_CNT_PORT3_MASK 0x800000u

//! Field INT_PORT4 - PORT4 Interrupt
#define IRNICR_LS_INT_PORT4_POS 24
//! Field INT_PORT4 - PORT4 Interrupt
#define IRNICR_LS_INT_PORT4_MASK 0x1000000u

//! Field CNT_PORT4 - PORT4 Counter Overflow
#define IRNICR_LS_CNT_PORT4_POS 25
//! Field CNT_PORT4 - PORT4 Counter Overflow
#define IRNICR_LS_CNT_PORT4_MASK 0x2000000u

//! Field INT_PORT5 - PORT5 Interrupt
#define IRNICR_LS_INT_PORT5_POS 26
//! Field INT_PORT5 - PORT5 Interrupt
#define IRNICR_LS_INT_PORT5_MASK 0x4000000u

//! Field CNT_PORT5 - PORT5 Counter Overflow
#define IRNICR_LS_CNT_PORT5_POS 27
//! Field CNT_PORT5 - PORT5 Counter Overflow
#define IRNICR_LS_CNT_PORT5_MASK 0x8000000u

//! Field INT_PORT6 - PORT6 Interrupt
#define IRNICR_LS_INT_PORT6_POS 28
//! Field INT_PORT6 - PORT6 Interrupt
#define IRNICR_LS_INT_PORT6_MASK 0x10000000u

//! Field CNT_PORT6 - PORT6 Counter Overflow
#define IRNICR_LS_CNT_PORT6_POS 29
//! Field CNT_PORT6 - PORT6 Counter Overflow
#define IRNICR_LS_CNT_PORT6_MASK 0x20000000u

//! Field INT_PORT7 - PORT7 Interrupt
#define IRNICR_LS_INT_PORT7_POS 30
//! Field INT_PORT7 - PORT7 Interrupt
#define IRNICR_LS_INT_PORT7_MASK 0x40000000u

//! Field CNT_PORT7 - PORT7 Counter Overflow
#define IRNICR_LS_CNT_PORT7_POS 31
//! Field CNT_PORT7 - PORT7 Counter Overflow
#define IRNICR_LS_CNT_PORT7_MASK 0x80000000u

//! @}

//! \defgroup IRNEN_LS Register IRNEN_LS - Load Spreader IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_LS 0x918
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_IRNEN_LS 0xE6660918u

//! Register Reset Value
#define IRNEN_LS_RST 0x00000000u

//! Field INT_PORT0 - PORT0 Interrupt
#define IRNEN_LS_INT_PORT0_POS 16
//! Field INT_PORT0 - PORT0 Interrupt
#define IRNEN_LS_INT_PORT0_MASK 0x10000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_INT_PORT0_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_INT_PORT0_EN 0x1

//! Field CNT_PORT0 - PORT0 Counter Overflow
#define IRNEN_LS_CNT_PORT0_POS 17
//! Field CNT_PORT0 - PORT0 Counter Overflow
#define IRNEN_LS_CNT_PORT0_MASK 0x20000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_CNT_PORT0_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_CNT_PORT0_EN 0x1

//! Field INT_PORT1 - PORT1 Interrupt
#define IRNEN_LS_INT_PORT1_POS 18
//! Field INT_PORT1 - PORT1 Interrupt
#define IRNEN_LS_INT_PORT1_MASK 0x40000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_INT_PORT1_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_INT_PORT1_EN 0x1

//! Field CNT_PORT1 - PORT1 Counter Overflow
#define IRNEN_LS_CNT_PORT1_POS 19
//! Field CNT_PORT1 - PORT1 Counter Overflow
#define IRNEN_LS_CNT_PORT1_MASK 0x80000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_CNT_PORT1_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_CNT_PORT1_EN 0x1

//! Field INT_PORT2 - PORT2 Interrupt
#define IRNEN_LS_INT_PORT2_POS 20
//! Field INT_PORT2 - PORT2 Interrupt
#define IRNEN_LS_INT_PORT2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_INT_PORT2_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_INT_PORT2_EN 0x1

//! Field CNT_PORT2 - PORT2 Counter Overflow
#define IRNEN_LS_CNT_PORT2_POS 21
//! Field CNT_PORT2 - PORT2 Counter Overflow
#define IRNEN_LS_CNT_PORT2_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_CNT_PORT2_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_CNT_PORT2_EN 0x1

//! Field INT_PORT3 - PORT3 Interrupt
#define IRNEN_LS_INT_PORT3_POS 22
//! Field INT_PORT3 - PORT3 Interrupt
#define IRNEN_LS_INT_PORT3_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_INT_PORT3_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_INT_PORT3_EN 0x1

//! Field CNT_PORT3 - PORT3 Counter Overflow
#define IRNEN_LS_CNT_PORT3_POS 23
//! Field CNT_PORT3 - PORT3 Counter Overflow
#define IRNEN_LS_CNT_PORT3_MASK 0x800000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_CNT_PORT3_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_CNT_PORT3_EN 0x1

//! Field INT_PORT4 - PORT4 Interrupt
#define IRNEN_LS_INT_PORT4_POS 24
//! Field INT_PORT4 - PORT4 Interrupt
#define IRNEN_LS_INT_PORT4_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_INT_PORT4_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_INT_PORT4_EN 0x1

//! Field CNT_PORT4 - PORT4 Counter Overflow
#define IRNEN_LS_CNT_PORT4_POS 25
//! Field CNT_PORT4 - PORT4 Counter Overflow
#define IRNEN_LS_CNT_PORT4_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_CNT_PORT4_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_CNT_PORT4_EN 0x1

//! Field INT_PORT5 - PORT5 Interrupt
#define IRNEN_LS_INT_PORT5_POS 26
//! Field INT_PORT5 - PORT5 Interrupt
#define IRNEN_LS_INT_PORT5_MASK 0x4000000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_INT_PORT5_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_INT_PORT5_EN 0x1

//! Field CNT_PORT5 - PORT5 Counter Overflow
#define IRNEN_LS_CNT_PORT5_POS 27
//! Field CNT_PORT5 - PORT5 Counter Overflow
#define IRNEN_LS_CNT_PORT5_MASK 0x8000000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_CNT_PORT5_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_CNT_PORT5_EN 0x1

//! Field INT_PORT6 - PORT6 Interrupt
#define IRNEN_LS_INT_PORT6_POS 28
//! Field INT_PORT6 - PORT6 Interrupt
#define IRNEN_LS_INT_PORT6_MASK 0x10000000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_INT_PORT6_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_INT_PORT6_EN 0x1

//! Field CNT_PORT6 - PORT6 Counter Overflow
#define IRNEN_LS_CNT_PORT6_POS 29
//! Field CNT_PORT6 - PORT6 Counter Overflow
#define IRNEN_LS_CNT_PORT6_MASK 0x20000000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_CNT_PORT6_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_CNT_PORT6_EN 0x1

//! Field INT_PORT7 - PORT7 Interrupt
#define IRNEN_LS_INT_PORT7_POS 30
//! Field INT_PORT7 - PORT7 Interrupt
#define IRNEN_LS_INT_PORT7_MASK 0x40000000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_INT_PORT7_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_INT_PORT7_EN 0x1

//! Field CNT_PORT7 - PORT7 Counter Overflow
#define IRNEN_LS_CNT_PORT7_POS 31
//! Field CNT_PORT7 - PORT7 Counter Overflow
#define IRNEN_LS_CNT_PORT7_MASK 0x80000000u
//! Constant DIS - DIS
#define CONST_IRNEN_LS_CNT_PORT7_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_LS_CNT_PORT7_EN 0x1

//! @}

//! \defgroup DBG_LS Register DBG_LS - Hardware Debug Register
//! @{

//! Register Offset (relative)
#define DBG_LS 0x920
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_DBG_LS 0xE6660920u

//! Register Reset Value
#define DBG_LS_RST 0x00010000u

//! Field DBG - Debug
#define DBG_LS_DBG_POS 0
//! Field DBG - Debug
#define DBG_LS_DBG_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TEST_LS Register TEST_LS - Hardware Test Register
//! @{

//! Register Offset (relative)
#define TEST_LS 0x930
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_TEST_LS 0xE6660930u

//! Register Reset Value
#define TEST_LS_RST 0x00000000u

//! Field TEST - Test
#define TEST_LS_TEST_POS 0
//! Field TEST - Test
#define TEST_LS_TEST_MASK 0xFFFFFFFFu

//! @}

//! \defgroup LS_FLOWID_MAP_COL0 Register LS_FLOWID_MAP_COL0 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL0 0x980
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL0 0xE6660980u

//! Register Reset Value
#define LS_FLOWID_MAP_COL0_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL0_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL0_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL0_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL0_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL0_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL0_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL0_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL0_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL0_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL0_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL0_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL0_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL0_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL0_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL0_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL0_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL1 Register LS_FLOWID_MAP_COL1 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL1 0x984
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL1 0xE6660984u

//! Register Reset Value
#define LS_FLOWID_MAP_COL1_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL1_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL1_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL1_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL1_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL1_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL1_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL1_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL1_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL1_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL1_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL1_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL1_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL1_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL1_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL1_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL1_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL2 Register LS_FLOWID_MAP_COL2 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL2 0x988
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL2 0xE6660988u

//! Register Reset Value
#define LS_FLOWID_MAP_COL2_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL2_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL2_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL2_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL2_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL2_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL2_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL2_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL2_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL2_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL2_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL2_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL2_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL2_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL2_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL2_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL2_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL3 Register LS_FLOWID_MAP_COL3 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL3 0x98C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL3 0xE666098Cu

//! Register Reset Value
#define LS_FLOWID_MAP_COL3_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL3_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL3_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL3_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL3_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL3_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL3_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL3_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL3_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL3_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL3_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL3_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL3_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL3_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL3_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL3_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL3_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL4 Register LS_FLOWID_MAP_COL4 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL4 0x990
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL4 0xE6660990u

//! Register Reset Value
#define LS_FLOWID_MAP_COL4_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL4_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL4_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL4_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL4_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL4_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL4_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL4_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL4_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL4_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL4_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL4_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL4_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL4_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL4_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL4_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL4_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL5 Register LS_FLOWID_MAP_COL5 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL5 0x994
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL5 0xE6660994u

//! Register Reset Value
#define LS_FLOWID_MAP_COL5_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL5_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL5_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL5_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL5_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL5_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL5_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL5_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL5_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL5_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL5_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL5_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL5_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL5_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL5_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL5_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL5_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL6 Register LS_FLOWID_MAP_COL6 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL6 0x998
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL6 0xE6660998u

//! Register Reset Value
#define LS_FLOWID_MAP_COL6_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL6_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL6_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL6_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL6_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL6_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL6_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL6_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL6_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL6_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL6_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL6_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL6_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL6_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL6_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL6_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL6_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL7 Register LS_FLOWID_MAP_COL7 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL7 0x99C
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL7 0xE666099Cu

//! Register Reset Value
#define LS_FLOWID_MAP_COL7_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL7_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL7_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL7_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL7_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL7_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL7_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL7_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL7_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL7_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL7_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL7_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL7_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL7_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL7_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL7_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL7_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL8 Register LS_FLOWID_MAP_COL8 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL8 0x9A0
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL8 0xE66609A0u

//! Register Reset Value
#define LS_FLOWID_MAP_COL8_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL8_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL8_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL8_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL8_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL8_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL8_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL8_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL8_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL8_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL8_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL8_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL8_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL8_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL8_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL8_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL8_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL9 Register LS_FLOWID_MAP_COL9 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL9 0x9A4
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL9 0xE66609A4u

//! Register Reset Value
#define LS_FLOWID_MAP_COL9_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL9_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL9_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL9_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL9_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL9_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL9_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL9_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL9_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL9_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL9_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL9_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL9_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL9_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL9_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL9_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL9_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL10 Register LS_FLOWID_MAP_COL10 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL10 0x9A8
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL10 0xE66609A8u

//! Register Reset Value
#define LS_FLOWID_MAP_COL10_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL10_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL10_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL10_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL10_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL10_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL10_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL10_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL10_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL10_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL10_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL10_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL10_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL10_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL10_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL10_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL10_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL11 Register LS_FLOWID_MAP_COL11 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL11 0x9AC
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL11 0xE66609ACu

//! Register Reset Value
#define LS_FLOWID_MAP_COL11_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL11_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL11_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL11_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL11_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL11_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL11_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL11_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL11_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL11_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL11_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL11_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL11_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL11_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL11_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL11_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL11_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL12 Register LS_FLOWID_MAP_COL12 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL12 0x9B0
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL12 0xE66609B0u

//! Register Reset Value
#define LS_FLOWID_MAP_COL12_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL12_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL12_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL12_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL12_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL12_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL12_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL12_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL12_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL12_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL12_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL12_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL12_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL12_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL12_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL12_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL12_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL13 Register LS_FLOWID_MAP_COL13 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL13 0x9B4
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL13 0xE66609B4u

//! Register Reset Value
#define LS_FLOWID_MAP_COL13_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL13_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL13_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL13_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL13_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL13_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL13_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL13_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL13_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL13_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL13_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL13_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL13_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL13_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL13_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL13_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL13_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL14 Register LS_FLOWID_MAP_COL14 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL14 0x9B8
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL14 0xE66609B8u

//! Register Reset Value
#define LS_FLOWID_MAP_COL14_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL14_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL14_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL14_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL14_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL14_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL14_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL14_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL14_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL14_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL14_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL14_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL14_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL14_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL14_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL14_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL14_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL15 Register LS_FLOWID_MAP_COL15 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL15 0x9BC
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL15 0xE66609BCu

//! Register Reset Value
#define LS_FLOWID_MAP_COL15_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL15_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL15_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL15_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL15_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL15_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL15_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL15_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL15_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL15_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL15_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL15_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL15_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL15_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL15_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL15_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL15_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL16 Register LS_FLOWID_MAP_COL16 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL16 0x9C0
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL16 0xE66609C0u

//! Register Reset Value
#define LS_FLOWID_MAP_COL16_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL16_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL16_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL16_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL16_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL16_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL16_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL16_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL16_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL16_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL16_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL16_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL16_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL16_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL16_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL16_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL16_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL17 Register LS_FLOWID_MAP_COL17 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL17 0x9C4
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL17 0xE66609C4u

//! Register Reset Value
#define LS_FLOWID_MAP_COL17_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL17_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL17_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL17_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL17_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL17_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL17_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL17_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL17_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL17_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL17_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL17_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL17_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL17_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL17_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL17_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL17_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL18 Register LS_FLOWID_MAP_COL18 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL18 0x9C8
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL18 0xE66609C8u

//! Register Reset Value
#define LS_FLOWID_MAP_COL18_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL18_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL18_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL18_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL18_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL18_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL18_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL18_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL18_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL18_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL18_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL18_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL18_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL18_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL18_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL18_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL18_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL19 Register LS_FLOWID_MAP_COL19 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL19 0x9CC
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL19 0xE66609CCu

//! Register Reset Value
#define LS_FLOWID_MAP_COL19_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL19_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL19_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL19_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL19_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL19_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL19_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL19_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL19_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL19_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL19_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL19_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL19_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL19_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL19_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL19_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL19_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL20 Register LS_FLOWID_MAP_COL20 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL20 0x9D0
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL20 0xE66609D0u

//! Register Reset Value
#define LS_FLOWID_MAP_COL20_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL20_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL20_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL20_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL20_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL20_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL20_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL20_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL20_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL20_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL20_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL20_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL20_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL20_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL20_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL20_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL20_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL21 Register LS_FLOWID_MAP_COL21 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL21 0x9D4
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL21 0xE66609D4u

//! Register Reset Value
#define LS_FLOWID_MAP_COL21_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL21_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL21_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL21_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL21_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL21_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL21_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL21_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL21_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL21_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL21_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL21_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL21_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL21_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL21_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL21_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL21_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL22 Register LS_FLOWID_MAP_COL22 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL22 0x9D8
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL22 0xE66609D8u

//! Register Reset Value
#define LS_FLOWID_MAP_COL22_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL22_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL22_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL22_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL22_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL22_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL22_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL22_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL22_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL22_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL22_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL22_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL22_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL22_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL22_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL22_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL22_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL23 Register LS_FLOWID_MAP_COL23 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL23 0x9DC
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL23 0xE66609DCu

//! Register Reset Value
#define LS_FLOWID_MAP_COL23_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL23_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL23_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL23_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL23_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL23_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL23_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL23_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL23_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL23_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL23_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL23_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL23_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL23_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL23_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL23_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL23_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL24 Register LS_FLOWID_MAP_COL24 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL24 0x9E0
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL24 0xE66609E0u

//! Register Reset Value
#define LS_FLOWID_MAP_COL24_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL24_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL24_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL24_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL24_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL24_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL24_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL24_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL24_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL24_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL24_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL24_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL24_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL24_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL24_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL24_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL24_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL25 Register LS_FLOWID_MAP_COL25 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL25 0x9E4
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL25 0xE66609E4u

//! Register Reset Value
#define LS_FLOWID_MAP_COL25_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL25_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL25_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL25_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL25_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL25_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL25_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL25_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL25_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL25_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL25_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL25_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL25_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL25_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL25_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL25_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL25_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL26 Register LS_FLOWID_MAP_COL26 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL26 0x9E8
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL26 0xE66609E8u

//! Register Reset Value
#define LS_FLOWID_MAP_COL26_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL26_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL26_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL26_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL26_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL26_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL26_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL26_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL26_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL26_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL26_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL26_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL26_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL26_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL26_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL26_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL26_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL27 Register LS_FLOWID_MAP_COL27 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL27 0x9EC
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL27 0xE66609ECu

//! Register Reset Value
#define LS_FLOWID_MAP_COL27_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL27_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL27_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL27_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL27_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL27_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL27_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL27_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL27_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL27_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL27_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL27_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL27_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL27_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL27_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL27_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL27_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL28 Register LS_FLOWID_MAP_COL28 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL28 0x9F0
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL28 0xE66609F0u

//! Register Reset Value
#define LS_FLOWID_MAP_COL28_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL28_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL28_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL28_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL28_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL28_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL28_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL28_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL28_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL28_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL28_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL28_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL28_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL28_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL28_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL28_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL28_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL29 Register LS_FLOWID_MAP_COL29 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL29 0x9F4
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL29 0xE66609F4u

//! Register Reset Value
#define LS_FLOWID_MAP_COL29_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL29_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL29_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL29_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL29_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL29_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL29_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL29_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL29_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL29_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL29_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL29_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL29_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL29_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL29_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL29_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL29_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL30 Register LS_FLOWID_MAP_COL30 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL30 0x9F8
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL30 0xE66609F8u

//! Register Reset Value
#define LS_FLOWID_MAP_COL30_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL30_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL30_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL30_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL30_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL30_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL30_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL30_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL30_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL30_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL30_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL30_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL30_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL30_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL30_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL30_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL30_QUEUE7_MASK 0xF0000000u

//! @}

//! \defgroup LS_FLOWID_MAP_COL31 Register LS_FLOWID_MAP_COL31 - Load Spreader Flow ID mapping table
//! @{

//! Register Offset (relative)
#define LS_FLOWID_MAP_COL31 0x9FC
//! Register Offset (absolute) for 1st Instance CQM_LOAD_SPREADER
#define CQM_LOAD_SPREADER_LS_FLOWID_MAP_COL31 0xE66609FCu

//! Register Reset Value
#define LS_FLOWID_MAP_COL31_RST 0x00000000u

//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL31_QUEUE0_POS 0
//! Field QUEUE0 - Queue ID 0
#define LS_FLOWID_MAP_COL31_QUEUE0_MASK 0xFu

//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL31_QUEUE1_POS 4
//! Field QUEUE1 - Queue ID 1
#define LS_FLOWID_MAP_COL31_QUEUE1_MASK 0xF0u

//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL31_QUEUE2_POS 8
//! Field QUEUE2 - Queue ID 2
#define LS_FLOWID_MAP_COL31_QUEUE2_MASK 0xF00u

//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL31_QUEUE3_POS 12
//! Field QUEUE3 - Queue ID 3
#define LS_FLOWID_MAP_COL31_QUEUE3_MASK 0xF000u

//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL31_QUEUE4_POS 16
//! Field QUEUE4 - Queue ID 4
#define LS_FLOWID_MAP_COL31_QUEUE4_MASK 0xF0000u

//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL31_QUEUE5_POS 20
//! Field QUEUE5 - Queue ID 5
#define LS_FLOWID_MAP_COL31_QUEUE5_MASK 0xF00000u

//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL31_QUEUE6_POS 24
//! Field QUEUE6 - Queue ID 6
#define LS_FLOWID_MAP_COL31_QUEUE6_MASK 0xF000000u

//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL31_QUEUE7_POS 28
//! Field QUEUE7 - Queue ID 7
#define LS_FLOWID_MAP_COL31_QUEUE7_MASK 0xF0000000u

//! @}

//! @}

#endif
