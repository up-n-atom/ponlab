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
// LSD Source          : /home/l1502/lgm_chip/v_leichuan.priv.v-dfv.lgm_chip.leichuan/ipg_lsd/lsd_sys/source/xml/reg_files/CBM_EQM.xml
// Register File Name  : CQEM_ENQ
// Register File Title : CPU En-queue & EQM Configuration Register Description
// Register Width      : 32
// Note                : Doxygen compliant comments
//-----------------------------------------------------------------------------

#ifndef _CQEM_ENQ_H
#define _CQEM_ENQ_H

//! \defgroup CQEM_ENQ Register File CQEM_ENQ - CPU En-queue & EQM Configuration Register Description
//! @{

//! Base Address of CQM_ENQ
#define CQM_ENQ_MODULE_BASE 0xE6680000u
//! Base Address of CQM_ENQ
#define CQM_ENQ_MODULE_BASE 0xE6680000u

//! \defgroup CBM_EQM_CTRL Register CBM_EQM_CTRL - CBM Enqueue Manager Control Register
//! @{

//! Register Offset (relative)
#define CBM_EQM_CTRL 0x0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CBM_EQM_CTRL 0xE6680000u

//! Register Reset Value
#define CBM_EQM_CTRL_RST 0x00001E00u

//! Field EQM_EN - Enable Enqueue Manager Core State Machine
#define CBM_EQM_CTRL_EQM_EN_POS 0
//! Field EQM_EN - Enable Enqueue Manager Core State Machine
#define CBM_EQM_CTRL_EQM_EN_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CBM_EQM_CTRL_EQM_EN_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_EQM_CTRL_EQM_EN_EN 0x1

//! Field EQM_FRZ - Freeze Enqueue Manager Core State Machine
#define CBM_EQM_CTRL_EQM_FRZ_POS 1
//! Field EQM_FRZ - Freeze Enqueue Manager Core State Machine
#define CBM_EQM_CTRL_EQM_FRZ_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CBM_EQM_CTRL_EQM_FRZ_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_EQM_CTRL_EQM_FRZ_EN 0x1

//! Field EQM_ACT - Enqueue Manager Activity Status
#define CBM_EQM_CTRL_EQM_ACT_POS 2
//! Field EQM_ACT - Enqueue Manager Activity Status
#define CBM_EQM_CTRL_EQM_ACT_MASK 0x4u
//! Constant INACTIVE - Inactive
#define CONST_CBM_EQM_CTRL_EQM_ACT_INACTIVE 0x0
//! Constant ACTIVE - Active
#define CONST_CBM_EQM_CTRL_EQM_ACT_ACTIVE 0x1

//! Field ACTEN - Port Activity Logic Enable
#define CBM_EQM_CTRL_ACTEN_POS 6
//! Field ACTEN - Port Activity Logic Enable
#define CBM_EQM_CTRL_ACTEN_MASK 0x40u
//! Constant DIS - DIS
#define CONST_CBM_EQM_CTRL_ACTEN_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_EQM_CTRL_ACTEN_EN 0x1

//! Field PDEN - Programmable Delay Enable
#define CBM_EQM_CTRL_PDEN_POS 8
//! Field PDEN - Programmable Delay Enable
#define CBM_EQM_CTRL_PDEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CBM_EQM_CTRL_PDEN_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_EQM_CTRL_PDEN_EN 0x1

//! Field L - Queue Length Threshold
#define CBM_EQM_CTRL_L_POS 9
//! Field L - Queue Length Threshold
#define CBM_EQM_CTRL_L_MASK 0x1FE00u

//! Field DICDROPDIS - DIC Drop Disable
#define CBM_EQM_CTRL_DICDROPDIS_POS 17
//! Field DICDROPDIS - DIC Drop Disable
#define CBM_EQM_CTRL_DICDROPDIS_MASK 0x20000u
//! Constant EN - EN
#define CONST_CBM_EQM_CTRL_DICDROPDIS_EN 0x0
//! Constant DIS - DIS
#define CONST_CBM_EQM_CTRL_DICDROPDIS_DIS 0x1

//! Field NODICPORT - No DIC bit discard for Port
#define CBM_EQM_CTRL_NODICPORT_POS 20
//! Field NODICPORT - No DIC bit discard for Port
#define CBM_EQM_CTRL_NODICPORT_MASK 0xF00000u

//! @}

//! \defgroup DBG_EQM_0 Register DBG_EQM_0 - Hardware Debug Register
//! @{

//! Register Offset (relative)
#define DBG_EQM_0 0x10
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DBG_EQM_0 0xE6680010u

//! Register Reset Value
#define DBG_EQM_0_RST 0x00000000u

//! Field DBG - Debug
#define DBG_EQM_0_DBG_POS 0
//! Field DBG - Debug
#define DBG_EQM_0_DBG_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DBG_EQM_1 Register DBG_EQM_1 - Hardware Debug Register
//! @{

//! Register Offset (relative)
#define DBG_EQM_1 0x14
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DBG_EQM_1 0xE6680014u

//! Register Reset Value
#define DBG_EQM_1_RST 0xE00004A0u

//! Field DBG - Debug
#define DBG_EQM_1_DBG_POS 0
//! Field DBG - Debug
#define DBG_EQM_1_DBG_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DBG_EQM_2 Register DBG_EQM_2 - Hardware Debug Register
//! @{

//! Register Offset (relative)
#define DBG_EQM_2 0x18
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DBG_EQM_2 0xE6680018u

//! Register Reset Value
#define DBG_EQM_2_RST 0x00000000u

//! Field DBG - Debug
#define DBG_EQM_2_DBG_POS 0
//! Field DBG - Debug
#define DBG_EQM_2_DBG_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TEST_EQM_0 Register TEST_EQM_0 - Hardware Test Register
//! @{

//! Register Offset (relative)
#define TEST_EQM_0 0x20
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_TEST_EQM_0 0xE6680020u

//! Register Reset Value
#define TEST_EQM_0_RST 0x00000000u

//! Field TEST - Test
#define TEST_EQM_0_TEST_POS 0
//! Field TEST - Test
#define TEST_EQM_0_TEST_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TEST_EQM_1 Register TEST_EQM_1 - Hardware Test Register
//! @{

//! Register Offset (relative)
#define TEST_EQM_1 0x24
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_TEST_EQM_1 0xE6680024u

//! Register Reset Value
#define TEST_EQM_1_RST 0x00000000u

//! Field TEST - Test
#define TEST_EQM_1_TEST_POS 0
//! Field TEST - Test
#define TEST_EQM_1_TEST_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DBG_EQM_3 Register DBG_EQM_3 - Hardware Debug Register
//! @{

//! Register Offset (relative)
#define DBG_EQM_3 0x28
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DBG_EQM_3 0xE6680028u

//! Register Reset Value
#define DBG_EQM_3_RST 0xE00004A0u

//! Field DBG - Debug
#define DBG_EQM_3_DBG_POS 0
//! Field DBG - Debug
#define DBG_EQM_3_DBG_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DBG_EQM_4 Register DBG_EQM_4 - Hardware Debug Register
//! @{

//! Register Offset (relative)
#define DBG_EQM_4 0x2C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DBG_EQM_4 0xE668002Cu

//! Register Reset Value
#define DBG_EQM_4_RST 0xE00004A0u

//! Field DBG - Debug
#define DBG_EQM_4_DBG_POS 0
//! Field DBG - Debug
#define DBG_EQM_4_DBG_MASK 0xFFFFFFFFu

//! @}

//! \defgroup OVH_0 Register OVH_0 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_0 0x50
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_0 0xE6680050u

//! Register Reset Value
#define OVH_0_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_0_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_0_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_1 Register OVH_1 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_1 0x54
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_1 0xE6680054u

//! Register Reset Value
#define OVH_1_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_1_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_1_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_2 Register OVH_2 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_2 0x58
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_2 0xE6680058u

//! Register Reset Value
#define OVH_2_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_2_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_2_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_3 Register OVH_3 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_3 0x5C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_3 0xE668005Cu

//! Register Reset Value
#define OVH_3_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_3_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_3_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_4 Register OVH_4 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_4 0x60
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_4 0xE6680060u

//! Register Reset Value
#define OVH_4_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_4_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_4_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_5 Register OVH_5 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_5 0x64
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_5 0xE6680064u

//! Register Reset Value
#define OVH_5_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_5_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_5_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_6 Register OVH_6 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_6 0x68
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_6 0xE6680068u

//! Register Reset Value
#define OVH_6_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_6_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_6_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_7 Register OVH_7 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_7 0x6C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_7 0xE668006Cu

//! Register Reset Value
#define OVH_7_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_7_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_7_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_8 Register OVH_8 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_8 0x70
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_8 0xE6680070u

//! Register Reset Value
#define OVH_8_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_8_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_8_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_9 Register OVH_9 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_9 0x74
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_9 0xE6680074u

//! Register Reset Value
#define OVH_9_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_9_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_9_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_10 Register OVH_10 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_10 0x78
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_10 0xE6680078u

//! Register Reset Value
#define OVH_10_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_10_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_10_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_11 Register OVH_11 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_11 0x7C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_11 0xE668007Cu

//! Register Reset Value
#define OVH_11_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_11_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_11_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_12 Register OVH_12 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_12 0x80
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_12 0xE6680080u

//! Register Reset Value
#define OVH_12_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_12_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_12_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_13 Register OVH_13 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_13 0x84
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_13 0xE6680084u

//! Register Reset Value
#define OVH_13_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_13_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_13_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_14 Register OVH_14 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_14 0x88
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_14 0xE6680088u

//! Register Reset Value
#define OVH_14_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_14_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_14_OVH_MASK 0xFFu

//! @}

//! \defgroup OVH_15 Register OVH_15 - Overhead Byte
//! @{

//! Register Offset (relative)
#define OVH_15 0x8C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_OVH_15 0xE668008Cu

//! Register Reset Value
#define OVH_15_RST 0x00000000u

//! Field OVH - Overhead Bytes
#define OVH_15_OVH_POS 0
//! Field OVH - Overhead Bytes
#define OVH_15_OVH_MASK 0xFFu

//! @}

//! \defgroup DMA_RDY_EN Register DMA_RDY_EN - DMA Ready Enable
//! @{

//! Register Offset (relative)
#define DMA_RDY_EN 0x90
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DMA_RDY_EN 0xE6680090u

//! Register Reset Value
#define DMA_RDY_EN_RST 0x0000003Fu

//! Field RDY_EN0 - Channel PB Ready Enable
#define DMA_RDY_EN_RDY_EN0_POS 0
//! Field RDY_EN0 - Channel PB Ready Enable
#define DMA_RDY_EN_RDY_EN0_MASK 0x1u
//! Constant DIS - DIS
#define CONST_DMA_RDY_EN_RDY_EN0_DIS 0x0
//! Constant EN - EN
#define CONST_DMA_RDY_EN_RDY_EN0_EN 0x1

//! Field RDY_EN1 - Channel Size0 Ready Enable
#define DMA_RDY_EN_RDY_EN1_POS 1
//! Field RDY_EN1 - Channel Size0 Ready Enable
#define DMA_RDY_EN_RDY_EN1_MASK 0x2u
//! Constant DIS - DIS
#define CONST_DMA_RDY_EN_RDY_EN1_DIS 0x0
//! Constant EN - EN
#define CONST_DMA_RDY_EN_RDY_EN1_EN 0x1

//! Field RDY_EN2 - Channel Size1 Ready Enable
#define DMA_RDY_EN_RDY_EN2_POS 2
//! Field RDY_EN2 - Channel Size1 Ready Enable
#define DMA_RDY_EN_RDY_EN2_MASK 0x4u
//! Constant DIS - DIS
#define CONST_DMA_RDY_EN_RDY_EN2_DIS 0x0
//! Constant EN - EN
#define CONST_DMA_RDY_EN_RDY_EN2_EN 0x1

//! Field RDY_EN3 - Channel Size2 Ready Enable
#define DMA_RDY_EN_RDY_EN3_POS 3
//! Field RDY_EN3 - Channel Size2 Ready Enable
#define DMA_RDY_EN_RDY_EN3_MASK 0x8u
//! Constant DIS - DIS
#define CONST_DMA_RDY_EN_RDY_EN3_DIS 0x0
//! Constant EN - EN
#define CONST_DMA_RDY_EN_RDY_EN3_EN 0x1

//! Field RDY_EN4 - Channel Size3 Ready Enable
#define DMA_RDY_EN_RDY_EN4_POS 4
//! Field RDY_EN4 - Channel Size3 Ready Enable
#define DMA_RDY_EN_RDY_EN4_MASK 0x10u
//! Constant DIS - DIS
#define CONST_DMA_RDY_EN_RDY_EN4_DIS 0x0
//! Constant EN - EN
#define CONST_DMA_RDY_EN_RDY_EN4_EN 0x1

//! Field RDY_EN5 - Channel H_mode Ready Enable
#define DMA_RDY_EN_RDY_EN5_POS 5
//! Field RDY_EN5 - Channel H_mode Ready Enable
#define DMA_RDY_EN_RDY_EN5_MASK 0x20u
//! Constant DIS - DIS
#define CONST_DMA_RDY_EN_RDY_EN5_DIS 0x0
//! Constant EN - EN
#define CONST_DMA_RDY_EN_RDY_EN5_EN 0x1

//! Field RDY_EN6 - Channel Size2 Ready Enable
#define DMA_RDY_EN_RDY_EN6_POS 6
//! Field RDY_EN6 - Channel Size2 Ready Enable
#define DMA_RDY_EN_RDY_EN6_MASK 0x40u
//! Constant DIS - DIS
#define CONST_DMA_RDY_EN_RDY_EN6_DIS 0x0
//! Constant EN - EN
#define CONST_DMA_RDY_EN_RDY_EN6_EN 0x1

//! Field RDY_EN7 - Channel Size2 Ready Enable
#define DMA_RDY_EN_RDY_EN7_POS 7
//! Field RDY_EN7 - Channel Size2 Ready Enable
#define DMA_RDY_EN_RDY_EN7_MASK 0x80u
//! Constant DIS - DIS
#define CONST_DMA_RDY_EN_RDY_EN7_DIS 0x0
//! Constant EN - EN
#define CONST_DMA_RDY_EN_RDY_EN7_EN 0x1

//! Field RDY_EN8 - Channel Size2 Ready Enable
#define DMA_RDY_EN_RDY_EN8_POS 8
//! Field RDY_EN8 - Channel Size2 Ready Enable
#define DMA_RDY_EN_RDY_EN8_MASK 0x100u
//! Constant DIS - DIS
#define CONST_DMA_RDY_EN_RDY_EN8_DIS 0x0
//! Constant EN - EN
#define CONST_DMA_RDY_EN_RDY_EN8_EN 0x1

//! Field RDY_EN9 - Channel Size2 Ready Enable
#define DMA_RDY_EN_RDY_EN9_POS 9
//! Field RDY_EN9 - Channel Size2 Ready Enable
#define DMA_RDY_EN_RDY_EN9_MASK 0x200u
//! Constant DIS - DIS
#define CONST_DMA_RDY_EN_RDY_EN9_DIS 0x0
//! Constant EN - EN
#define CONST_DMA_RDY_EN_RDY_EN9_EN 0x1

//! @}

//! \defgroup EP_THRSD_EN_0 Register EP_THRSD_EN_0 - Egress Port Threshold Enable
//! @{

//! Register Offset (relative)
#define EP_THRSD_EN_0 0x100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EP_THRSD_EN_0 0xE6680100u

//! Register Reset Value
#define EP_THRSD_EN_0_RST 0xFFFFFFFFu

//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_0_EPEN0_POS 0
//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_0_EPEN0_MASK 0x1u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN0_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN0_EN 0x1

//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_0_EPEN1_POS 1
//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_0_EPEN1_MASK 0x2u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN1_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN1_EN 0x1

//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_0_EPEN2_POS 2
//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_0_EPEN2_MASK 0x4u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN2_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN2_EN 0x1

//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_0_EPEN3_POS 3
//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_0_EPEN3_MASK 0x8u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN3_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN3_EN 0x1

//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_0_EPEN4_POS 4
//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_0_EPEN4_MASK 0x10u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN4_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN4_EN 0x1

//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_0_EPEN5_POS 5
//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_0_EPEN5_MASK 0x20u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN5_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN5_EN 0x1

//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_0_EPEN6_POS 6
//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_0_EPEN6_MASK 0x40u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN6_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN6_EN 0x1

//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_0_EPEN7_POS 7
//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_0_EPEN7_MASK 0x80u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN7_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN7_EN 0x1

//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_0_EPEN8_POS 8
//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_0_EPEN8_MASK 0x100u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN8_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN8_EN 0x1

//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_0_EPEN9_POS 9
//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_0_EPEN9_MASK 0x200u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN9_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN9_EN 0x1

//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_0_EPEN10_POS 10
//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_0_EPEN10_MASK 0x400u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN10_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN10_EN 0x1

//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_0_EPEN11_POS 11
//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_0_EPEN11_MASK 0x800u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN11_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN11_EN 0x1

//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_0_EPEN12_POS 12
//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_0_EPEN12_MASK 0x1000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN12_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN12_EN 0x1

//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_0_EPEN13_POS 13
//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_0_EPEN13_MASK 0x2000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN13_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN13_EN 0x1

//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_0_EPEN14_POS 14
//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_0_EPEN14_MASK 0x4000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN14_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN14_EN 0x1

//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_0_EPEN15_POS 15
//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_0_EPEN15_MASK 0x8000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN15_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN15_EN 0x1

//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_0_EPEN16_POS 16
//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_0_EPEN16_MASK 0x10000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN16_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN16_EN 0x1

//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_0_EPEN17_POS 17
//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_0_EPEN17_MASK 0x20000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN17_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN17_EN 0x1

//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_0_EPEN18_POS 18
//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_0_EPEN18_MASK 0x40000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN18_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN18_EN 0x1

//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_0_EPEN19_POS 19
//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_0_EPEN19_MASK 0x80000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN19_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN19_EN 0x1

//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_0_EPEN20_POS 20
//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_0_EPEN20_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN20_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN20_EN 0x1

//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_0_EPEN21_POS 21
//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_0_EPEN21_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN21_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN21_EN 0x1

//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_0_EPEN22_POS 22
//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_0_EPEN22_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN22_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN22_EN 0x1

//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_0_EPEN23_POS 23
//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_0_EPEN23_MASK 0x800000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN23_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN23_EN 0x1

//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_0_EPEN24_POS 24
//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_0_EPEN24_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN24_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN24_EN 0x1

//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_0_EPEN25_POS 25
//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_0_EPEN25_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN25_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN25_EN 0x1

//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_0_EPEN26_POS 26
//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_0_EPEN26_MASK 0x4000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN26_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN26_EN 0x1

//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_0_EPEN27_POS 27
//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_0_EPEN27_MASK 0x8000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN27_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN27_EN 0x1

//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_0_EPEN28_POS 28
//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_0_EPEN28_MASK 0x10000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN28_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN28_EN 0x1

//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_0_EPEN29_POS 29
//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_0_EPEN29_MASK 0x20000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN29_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN29_EN 0x1

//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_0_EPEN30_POS 30
//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_0_EPEN30_MASK 0x40000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN30_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN30_EN 0x1

//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_0_EPEN31_POS 31
//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_0_EPEN31_MASK 0x80000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_0_EPEN31_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_0_EPEN31_EN 0x1

//! @}

//! \defgroup EP_THRSD_EN_1 Register EP_THRSD_EN_1 - Egress Port Threshold Enable
//! @{

//! Register Offset (relative)
#define EP_THRSD_EN_1 0x104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EP_THRSD_EN_1 0xE6680104u

//! Register Reset Value
#define EP_THRSD_EN_1_RST 0xFFFFFFFFu

//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_1_EPEN0_POS 0
//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_1_EPEN0_MASK 0x1u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN0_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN0_EN 0x1

//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_1_EPEN1_POS 1
//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_1_EPEN1_MASK 0x2u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN1_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN1_EN 0x1

//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_1_EPEN2_POS 2
//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_1_EPEN2_MASK 0x4u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN2_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN2_EN 0x1

//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_1_EPEN3_POS 3
//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_1_EPEN3_MASK 0x8u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN3_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN3_EN 0x1

//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_1_EPEN4_POS 4
//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_1_EPEN4_MASK 0x10u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN4_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN4_EN 0x1

//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_1_EPEN5_POS 5
//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_1_EPEN5_MASK 0x20u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN5_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN5_EN 0x1

//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_1_EPEN6_POS 6
//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_1_EPEN6_MASK 0x40u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN6_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN6_EN 0x1

//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_1_EPEN7_POS 7
//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_1_EPEN7_MASK 0x80u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN7_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN7_EN 0x1

//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_1_EPEN8_POS 8
//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_1_EPEN8_MASK 0x100u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN8_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN8_EN 0x1

//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_1_EPEN9_POS 9
//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_1_EPEN9_MASK 0x200u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN9_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN9_EN 0x1

//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_1_EPEN10_POS 10
//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_1_EPEN10_MASK 0x400u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN10_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN10_EN 0x1

//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_1_EPEN11_POS 11
//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_1_EPEN11_MASK 0x800u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN11_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN11_EN 0x1

//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_1_EPEN12_POS 12
//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_1_EPEN12_MASK 0x1000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN12_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN12_EN 0x1

//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_1_EPEN13_POS 13
//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_1_EPEN13_MASK 0x2000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN13_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN13_EN 0x1

//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_1_EPEN14_POS 14
//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_1_EPEN14_MASK 0x4000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN14_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN14_EN 0x1

//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_1_EPEN15_POS 15
//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_1_EPEN15_MASK 0x8000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN15_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN15_EN 0x1

//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_1_EPEN16_POS 16
//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_1_EPEN16_MASK 0x10000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN16_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN16_EN 0x1

//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_1_EPEN17_POS 17
//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_1_EPEN17_MASK 0x20000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN17_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN17_EN 0x1

//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_1_EPEN18_POS 18
//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_1_EPEN18_MASK 0x40000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN18_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN18_EN 0x1

//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_1_EPEN19_POS 19
//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_1_EPEN19_MASK 0x80000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN19_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN19_EN 0x1

//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_1_EPEN20_POS 20
//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_1_EPEN20_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN20_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN20_EN 0x1

//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_1_EPEN21_POS 21
//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_1_EPEN21_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN21_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN21_EN 0x1

//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_1_EPEN22_POS 22
//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_1_EPEN22_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN22_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN22_EN 0x1

//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_1_EPEN23_POS 23
//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_1_EPEN23_MASK 0x800000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN23_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN23_EN 0x1

//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_1_EPEN24_POS 24
//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_1_EPEN24_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN24_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN24_EN 0x1

//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_1_EPEN25_POS 25
//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_1_EPEN25_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN25_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN25_EN 0x1

//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_1_EPEN26_POS 26
//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_1_EPEN26_MASK 0x4000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN26_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN26_EN 0x1

//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_1_EPEN27_POS 27
//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_1_EPEN27_MASK 0x8000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN27_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN27_EN 0x1

//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_1_EPEN28_POS 28
//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_1_EPEN28_MASK 0x10000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN28_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN28_EN 0x1

//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_1_EPEN29_POS 29
//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_1_EPEN29_MASK 0x20000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN29_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN29_EN 0x1

//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_1_EPEN30_POS 30
//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_1_EPEN30_MASK 0x40000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN30_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN30_EN 0x1

//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_1_EPEN31_POS 31
//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_1_EPEN31_MASK 0x80000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_1_EPEN31_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_1_EPEN31_EN 0x1

//! @}

//! \defgroup EP_THRSD_EN_2 Register EP_THRSD_EN_2 - Egress Port Threshold Enable
//! @{

//! Register Offset (relative)
#define EP_THRSD_EN_2 0x108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EP_THRSD_EN_2 0xE6680108u

//! Register Reset Value
#define EP_THRSD_EN_2_RST 0xFFFFFFFFu

//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_2_EPEN0_POS 0
//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_2_EPEN0_MASK 0x1u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN0_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN0_EN 0x1

//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_2_EPEN1_POS 1
//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_2_EPEN1_MASK 0x2u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN1_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN1_EN 0x1

//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_2_EPEN2_POS 2
//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_2_EPEN2_MASK 0x4u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN2_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN2_EN 0x1

//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_2_EPEN3_POS 3
//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_2_EPEN3_MASK 0x8u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN3_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN3_EN 0x1

//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_2_EPEN4_POS 4
//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_2_EPEN4_MASK 0x10u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN4_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN4_EN 0x1

//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_2_EPEN5_POS 5
//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_2_EPEN5_MASK 0x20u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN5_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN5_EN 0x1

//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_2_EPEN6_POS 6
//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_2_EPEN6_MASK 0x40u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN6_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN6_EN 0x1

//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_2_EPEN7_POS 7
//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_2_EPEN7_MASK 0x80u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN7_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN7_EN 0x1

//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_2_EPEN8_POS 8
//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_2_EPEN8_MASK 0x100u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN8_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN8_EN 0x1

//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_2_EPEN9_POS 9
//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_2_EPEN9_MASK 0x200u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN9_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN9_EN 0x1

//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_2_EPEN10_POS 10
//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_2_EPEN10_MASK 0x400u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN10_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN10_EN 0x1

//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_2_EPEN11_POS 11
//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_2_EPEN11_MASK 0x800u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN11_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN11_EN 0x1

//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_2_EPEN12_POS 12
//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_2_EPEN12_MASK 0x1000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN12_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN12_EN 0x1

//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_2_EPEN13_POS 13
//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_2_EPEN13_MASK 0x2000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN13_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN13_EN 0x1

//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_2_EPEN14_POS 14
//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_2_EPEN14_MASK 0x4000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN14_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN14_EN 0x1

//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_2_EPEN15_POS 15
//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_2_EPEN15_MASK 0x8000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN15_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN15_EN 0x1

//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_2_EPEN16_POS 16
//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_2_EPEN16_MASK 0x10000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN16_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN16_EN 0x1

//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_2_EPEN17_POS 17
//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_2_EPEN17_MASK 0x20000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN17_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN17_EN 0x1

//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_2_EPEN18_POS 18
//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_2_EPEN18_MASK 0x40000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN18_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN18_EN 0x1

//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_2_EPEN19_POS 19
//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_2_EPEN19_MASK 0x80000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN19_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN19_EN 0x1

//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_2_EPEN20_POS 20
//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_2_EPEN20_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN20_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN20_EN 0x1

//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_2_EPEN21_POS 21
//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_2_EPEN21_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN21_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN21_EN 0x1

//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_2_EPEN22_POS 22
//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_2_EPEN22_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN22_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN22_EN 0x1

//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_2_EPEN23_POS 23
//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_2_EPEN23_MASK 0x800000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN23_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN23_EN 0x1

//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_2_EPEN24_POS 24
//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_2_EPEN24_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN24_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN24_EN 0x1

//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_2_EPEN25_POS 25
//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_2_EPEN25_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN25_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN25_EN 0x1

//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_2_EPEN26_POS 26
//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_2_EPEN26_MASK 0x4000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN26_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN26_EN 0x1

//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_2_EPEN27_POS 27
//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_2_EPEN27_MASK 0x8000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN27_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN27_EN 0x1

//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_2_EPEN28_POS 28
//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_2_EPEN28_MASK 0x10000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN28_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN28_EN 0x1

//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_2_EPEN29_POS 29
//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_2_EPEN29_MASK 0x20000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN29_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN29_EN 0x1

//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_2_EPEN30_POS 30
//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_2_EPEN30_MASK 0x40000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN30_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN30_EN 0x1

//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_2_EPEN31_POS 31
//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_2_EPEN31_MASK 0x80000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_2_EPEN31_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_2_EPEN31_EN 0x1

//! @}

//! \defgroup EP_THRSD_EN_3 Register EP_THRSD_EN_3 - Egress Port Threshold Enable
//! @{

//! Register Offset (relative)
#define EP_THRSD_EN_3 0x10C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EP_THRSD_EN_3 0xE668010Cu

//! Register Reset Value
#define EP_THRSD_EN_3_RST 0xFFFFFFFFu

//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_3_EPEN0_POS 0
//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_3_EPEN0_MASK 0x1u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN0_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN0_EN 0x1

//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_3_EPEN1_POS 1
//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_3_EPEN1_MASK 0x2u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN1_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN1_EN 0x1

//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_3_EPEN2_POS 2
//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_3_EPEN2_MASK 0x4u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN2_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN2_EN 0x1

//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_3_EPEN3_POS 3
//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_3_EPEN3_MASK 0x8u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN3_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN3_EN 0x1

//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_3_EPEN4_POS 4
//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_3_EPEN4_MASK 0x10u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN4_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN4_EN 0x1

//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_3_EPEN5_POS 5
//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_3_EPEN5_MASK 0x20u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN5_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN5_EN 0x1

//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_3_EPEN6_POS 6
//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_3_EPEN6_MASK 0x40u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN6_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN6_EN 0x1

//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_3_EPEN7_POS 7
//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_3_EPEN7_MASK 0x80u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN7_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN7_EN 0x1

//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_3_EPEN8_POS 8
//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_3_EPEN8_MASK 0x100u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN8_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN8_EN 0x1

//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_3_EPEN9_POS 9
//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_3_EPEN9_MASK 0x200u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN9_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN9_EN 0x1

//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_3_EPEN10_POS 10
//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_3_EPEN10_MASK 0x400u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN10_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN10_EN 0x1

//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_3_EPEN11_POS 11
//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_3_EPEN11_MASK 0x800u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN11_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN11_EN 0x1

//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_3_EPEN12_POS 12
//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_3_EPEN12_MASK 0x1000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN12_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN12_EN 0x1

//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_3_EPEN13_POS 13
//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_3_EPEN13_MASK 0x2000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN13_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN13_EN 0x1

//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_3_EPEN14_POS 14
//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_3_EPEN14_MASK 0x4000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN14_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN14_EN 0x1

//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_3_EPEN15_POS 15
//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_3_EPEN15_MASK 0x8000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN15_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN15_EN 0x1

//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_3_EPEN16_POS 16
//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_3_EPEN16_MASK 0x10000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN16_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN16_EN 0x1

//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_3_EPEN17_POS 17
//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_3_EPEN17_MASK 0x20000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN17_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN17_EN 0x1

//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_3_EPEN18_POS 18
//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_3_EPEN18_MASK 0x40000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN18_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN18_EN 0x1

//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_3_EPEN19_POS 19
//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_3_EPEN19_MASK 0x80000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN19_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN19_EN 0x1

//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_3_EPEN20_POS 20
//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_3_EPEN20_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN20_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN20_EN 0x1

//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_3_EPEN21_POS 21
//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_3_EPEN21_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN21_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN21_EN 0x1

//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_3_EPEN22_POS 22
//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_3_EPEN22_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN22_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN22_EN 0x1

//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_3_EPEN23_POS 23
//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_3_EPEN23_MASK 0x800000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN23_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN23_EN 0x1

//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_3_EPEN24_POS 24
//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_3_EPEN24_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN24_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN24_EN 0x1

//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_3_EPEN25_POS 25
//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_3_EPEN25_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN25_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN25_EN 0x1

//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_3_EPEN26_POS 26
//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_3_EPEN26_MASK 0x4000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN26_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN26_EN 0x1

//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_3_EPEN27_POS 27
//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_3_EPEN27_MASK 0x8000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN27_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN27_EN 0x1

//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_3_EPEN28_POS 28
//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_3_EPEN28_MASK 0x10000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN28_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN28_EN 0x1

//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_3_EPEN29_POS 29
//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_3_EPEN29_MASK 0x20000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN29_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN29_EN 0x1

//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_3_EPEN30_POS 30
//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_3_EPEN30_MASK 0x40000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN30_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN30_EN 0x1

//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_3_EPEN31_POS 31
//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_3_EPEN31_MASK 0x80000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_3_EPEN31_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_3_EPEN31_EN 0x1

//! @}

//! \defgroup EP_THRSD_EN_4 Register EP_THRSD_EN_4 - Egress Port Threshold Enable
//! @{

//! Register Offset (relative)
#define EP_THRSD_EN_4 0x110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EP_THRSD_EN_4 0xE6680110u

//! Register Reset Value
#define EP_THRSD_EN_4_RST 0xFFFFFFFFu

//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_4_EPEN0_POS 0
//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_4_EPEN0_MASK 0x1u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN0_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN0_EN 0x1

//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_4_EPEN1_POS 1
//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_4_EPEN1_MASK 0x2u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN1_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN1_EN 0x1

//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_4_EPEN2_POS 2
//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_4_EPEN2_MASK 0x4u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN2_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN2_EN 0x1

//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_4_EPEN3_POS 3
//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_4_EPEN3_MASK 0x8u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN3_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN3_EN 0x1

//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_4_EPEN4_POS 4
//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_4_EPEN4_MASK 0x10u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN4_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN4_EN 0x1

//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_4_EPEN5_POS 5
//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_4_EPEN5_MASK 0x20u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN5_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN5_EN 0x1

//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_4_EPEN6_POS 6
//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_4_EPEN6_MASK 0x40u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN6_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN6_EN 0x1

//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_4_EPEN7_POS 7
//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_4_EPEN7_MASK 0x80u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN7_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN7_EN 0x1

//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_4_EPEN8_POS 8
//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_4_EPEN8_MASK 0x100u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN8_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN8_EN 0x1

//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_4_EPEN9_POS 9
//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_4_EPEN9_MASK 0x200u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN9_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN9_EN 0x1

//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_4_EPEN10_POS 10
//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_4_EPEN10_MASK 0x400u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN10_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN10_EN 0x1

//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_4_EPEN11_POS 11
//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_4_EPEN11_MASK 0x800u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN11_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN11_EN 0x1

//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_4_EPEN12_POS 12
//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_4_EPEN12_MASK 0x1000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN12_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN12_EN 0x1

//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_4_EPEN13_POS 13
//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_4_EPEN13_MASK 0x2000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN13_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN13_EN 0x1

//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_4_EPEN14_POS 14
//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_4_EPEN14_MASK 0x4000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN14_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN14_EN 0x1

//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_4_EPEN15_POS 15
//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_4_EPEN15_MASK 0x8000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN15_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN15_EN 0x1

//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_4_EPEN16_POS 16
//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_4_EPEN16_MASK 0x10000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN16_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN16_EN 0x1

//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_4_EPEN17_POS 17
//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_4_EPEN17_MASK 0x20000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN17_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN17_EN 0x1

//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_4_EPEN18_POS 18
//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_4_EPEN18_MASK 0x40000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN18_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN18_EN 0x1

//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_4_EPEN19_POS 19
//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_4_EPEN19_MASK 0x80000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN19_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN19_EN 0x1

//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_4_EPEN20_POS 20
//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_4_EPEN20_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN20_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN20_EN 0x1

//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_4_EPEN21_POS 21
//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_4_EPEN21_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN21_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN21_EN 0x1

//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_4_EPEN22_POS 22
//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_4_EPEN22_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN22_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN22_EN 0x1

//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_4_EPEN23_POS 23
//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_4_EPEN23_MASK 0x800000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN23_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN23_EN 0x1

//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_4_EPEN24_POS 24
//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_4_EPEN24_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN24_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN24_EN 0x1

//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_4_EPEN25_POS 25
//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_4_EPEN25_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN25_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN25_EN 0x1

//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_4_EPEN26_POS 26
//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_4_EPEN26_MASK 0x4000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN26_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN26_EN 0x1

//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_4_EPEN27_POS 27
//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_4_EPEN27_MASK 0x8000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN27_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN27_EN 0x1

//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_4_EPEN28_POS 28
//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_4_EPEN28_MASK 0x10000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN28_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN28_EN 0x1

//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_4_EPEN29_POS 29
//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_4_EPEN29_MASK 0x20000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN29_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN29_EN 0x1

//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_4_EPEN30_POS 30
//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_4_EPEN30_MASK 0x40000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN30_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN30_EN 0x1

//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_4_EPEN31_POS 31
//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_4_EPEN31_MASK 0x80000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_4_EPEN31_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_4_EPEN31_EN 0x1

//! @}

//! \defgroup EP_THRSD_EN_5 Register EP_THRSD_EN_5 - Egress Port Threshold Enable
//! @{

//! Register Offset (relative)
#define EP_THRSD_EN_5 0x114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EP_THRSD_EN_5 0xE6680114u

//! Register Reset Value
#define EP_THRSD_EN_5_RST 0xFFFFFFFFu

//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_5_EPEN0_POS 0
//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_5_EPEN0_MASK 0x1u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN0_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN0_EN 0x1

//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_5_EPEN1_POS 1
//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_5_EPEN1_MASK 0x2u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN1_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN1_EN 0x1

//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_5_EPEN2_POS 2
//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_5_EPEN2_MASK 0x4u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN2_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN2_EN 0x1

//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_5_EPEN3_POS 3
//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_5_EPEN3_MASK 0x8u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN3_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN3_EN 0x1

//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_5_EPEN4_POS 4
//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_5_EPEN4_MASK 0x10u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN4_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN4_EN 0x1

//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_5_EPEN5_POS 5
//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_5_EPEN5_MASK 0x20u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN5_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN5_EN 0x1

//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_5_EPEN6_POS 6
//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_5_EPEN6_MASK 0x40u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN6_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN6_EN 0x1

//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_5_EPEN7_POS 7
//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_5_EPEN7_MASK 0x80u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN7_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN7_EN 0x1

//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_5_EPEN8_POS 8
//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_5_EPEN8_MASK 0x100u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN8_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN8_EN 0x1

//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_5_EPEN9_POS 9
//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_5_EPEN9_MASK 0x200u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN9_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN9_EN 0x1

//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_5_EPEN10_POS 10
//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_5_EPEN10_MASK 0x400u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN10_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN10_EN 0x1

//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_5_EPEN11_POS 11
//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_5_EPEN11_MASK 0x800u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN11_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN11_EN 0x1

//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_5_EPEN12_POS 12
//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_5_EPEN12_MASK 0x1000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN12_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN12_EN 0x1

//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_5_EPEN13_POS 13
//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_5_EPEN13_MASK 0x2000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN13_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN13_EN 0x1

//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_5_EPEN14_POS 14
//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_5_EPEN14_MASK 0x4000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN14_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN14_EN 0x1

//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_5_EPEN15_POS 15
//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_5_EPEN15_MASK 0x8000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN15_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN15_EN 0x1

//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_5_EPEN16_POS 16
//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_5_EPEN16_MASK 0x10000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN16_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN16_EN 0x1

//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_5_EPEN17_POS 17
//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_5_EPEN17_MASK 0x20000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN17_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN17_EN 0x1

//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_5_EPEN18_POS 18
//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_5_EPEN18_MASK 0x40000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN18_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN18_EN 0x1

//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_5_EPEN19_POS 19
//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_5_EPEN19_MASK 0x80000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN19_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN19_EN 0x1

//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_5_EPEN20_POS 20
//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_5_EPEN20_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN20_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN20_EN 0x1

//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_5_EPEN21_POS 21
//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_5_EPEN21_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN21_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN21_EN 0x1

//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_5_EPEN22_POS 22
//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_5_EPEN22_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN22_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN22_EN 0x1

//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_5_EPEN23_POS 23
//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_5_EPEN23_MASK 0x800000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN23_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN23_EN 0x1

//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_5_EPEN24_POS 24
//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_5_EPEN24_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN24_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN24_EN 0x1

//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_5_EPEN25_POS 25
//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_5_EPEN25_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN25_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN25_EN 0x1

//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_5_EPEN26_POS 26
//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_5_EPEN26_MASK 0x4000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN26_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN26_EN 0x1

//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_5_EPEN27_POS 27
//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_5_EPEN27_MASK 0x8000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN27_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN27_EN 0x1

//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_5_EPEN28_POS 28
//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_5_EPEN28_MASK 0x10000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN28_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN28_EN 0x1

//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_5_EPEN29_POS 29
//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_5_EPEN29_MASK 0x20000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN29_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN29_EN 0x1

//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_5_EPEN30_POS 30
//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_5_EPEN30_MASK 0x40000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN30_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN30_EN 0x1

//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_5_EPEN31_POS 31
//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_5_EPEN31_MASK 0x80000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_5_EPEN31_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_5_EPEN31_EN 0x1

//! @}

//! \defgroup EP_THRSD_EN_6 Register EP_THRSD_EN_6 - Egress Port Threshold Enable
//! @{

//! Register Offset (relative)
#define EP_THRSD_EN_6 0x118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EP_THRSD_EN_6 0xE6680118u

//! Register Reset Value
#define EP_THRSD_EN_6_RST 0xFFFFFFFFu

//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_6_EPEN0_POS 0
//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_6_EPEN0_MASK 0x1u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN0_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN0_EN 0x1

//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_6_EPEN1_POS 1
//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_6_EPEN1_MASK 0x2u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN1_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN1_EN 0x1

//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_6_EPEN2_POS 2
//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_6_EPEN2_MASK 0x4u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN2_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN2_EN 0x1

//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_6_EPEN3_POS 3
//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_6_EPEN3_MASK 0x8u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN3_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN3_EN 0x1

//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_6_EPEN4_POS 4
//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_6_EPEN4_MASK 0x10u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN4_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN4_EN 0x1

//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_6_EPEN5_POS 5
//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_6_EPEN5_MASK 0x20u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN5_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN5_EN 0x1

//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_6_EPEN6_POS 6
//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_6_EPEN6_MASK 0x40u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN6_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN6_EN 0x1

//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_6_EPEN7_POS 7
//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_6_EPEN7_MASK 0x80u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN7_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN7_EN 0x1

//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_6_EPEN8_POS 8
//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_6_EPEN8_MASK 0x100u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN8_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN8_EN 0x1

//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_6_EPEN9_POS 9
//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_6_EPEN9_MASK 0x200u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN9_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN9_EN 0x1

//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_6_EPEN10_POS 10
//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_6_EPEN10_MASK 0x400u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN10_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN10_EN 0x1

//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_6_EPEN11_POS 11
//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_6_EPEN11_MASK 0x800u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN11_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN11_EN 0x1

//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_6_EPEN12_POS 12
//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_6_EPEN12_MASK 0x1000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN12_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN12_EN 0x1

//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_6_EPEN13_POS 13
//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_6_EPEN13_MASK 0x2000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN13_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN13_EN 0x1

//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_6_EPEN14_POS 14
//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_6_EPEN14_MASK 0x4000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN14_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN14_EN 0x1

//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_6_EPEN15_POS 15
//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_6_EPEN15_MASK 0x8000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN15_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN15_EN 0x1

//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_6_EPEN16_POS 16
//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_6_EPEN16_MASK 0x10000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN16_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN16_EN 0x1

//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_6_EPEN17_POS 17
//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_6_EPEN17_MASK 0x20000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN17_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN17_EN 0x1

//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_6_EPEN18_POS 18
//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_6_EPEN18_MASK 0x40000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN18_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN18_EN 0x1

//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_6_EPEN19_POS 19
//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_6_EPEN19_MASK 0x80000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN19_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN19_EN 0x1

//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_6_EPEN20_POS 20
//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_6_EPEN20_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN20_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN20_EN 0x1

//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_6_EPEN21_POS 21
//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_6_EPEN21_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN21_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN21_EN 0x1

//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_6_EPEN22_POS 22
//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_6_EPEN22_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN22_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN22_EN 0x1

//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_6_EPEN23_POS 23
//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_6_EPEN23_MASK 0x800000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN23_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN23_EN 0x1

//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_6_EPEN24_POS 24
//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_6_EPEN24_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN24_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN24_EN 0x1

//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_6_EPEN25_POS 25
//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_6_EPEN25_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN25_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN25_EN 0x1

//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_6_EPEN26_POS 26
//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_6_EPEN26_MASK 0x4000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN26_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN26_EN 0x1

//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_6_EPEN27_POS 27
//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_6_EPEN27_MASK 0x8000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN27_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN27_EN 0x1

//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_6_EPEN28_POS 28
//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_6_EPEN28_MASK 0x10000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN28_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN28_EN 0x1

//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_6_EPEN29_POS 29
//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_6_EPEN29_MASK 0x20000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN29_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN29_EN 0x1

//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_6_EPEN30_POS 30
//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_6_EPEN30_MASK 0x40000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN30_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN30_EN 0x1

//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_6_EPEN31_POS 31
//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_6_EPEN31_MASK 0x80000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_6_EPEN31_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_6_EPEN31_EN 0x1

//! @}

//! \defgroup EP_THRSD_EN_7 Register EP_THRSD_EN_7 - Egress Port Threshold Enable
//! @{

//! Register Offset (relative)
#define EP_THRSD_EN_7 0x11C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EP_THRSD_EN_7 0xE668011Cu

//! Register Reset Value
#define EP_THRSD_EN_7_RST 0xFFFFFFFFu

//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_7_EPEN0_POS 0
//! Field EPEN0 - EP Enable 0
#define EP_THRSD_EN_7_EPEN0_MASK 0x1u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN0_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN0_EN 0x1

//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_7_EPEN1_POS 1
//! Field EPEN1 - EP Enable 1
#define EP_THRSD_EN_7_EPEN1_MASK 0x2u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN1_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN1_EN 0x1

//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_7_EPEN2_POS 2
//! Field EPEN2 - EP Enable 2
#define EP_THRSD_EN_7_EPEN2_MASK 0x4u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN2_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN2_EN 0x1

//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_7_EPEN3_POS 3
//! Field EPEN3 - EP Enable 3
#define EP_THRSD_EN_7_EPEN3_MASK 0x8u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN3_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN3_EN 0x1

//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_7_EPEN4_POS 4
//! Field EPEN4 - EP Enable 4
#define EP_THRSD_EN_7_EPEN4_MASK 0x10u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN4_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN4_EN 0x1

//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_7_EPEN5_POS 5
//! Field EPEN5 - EP Enable 5
#define EP_THRSD_EN_7_EPEN5_MASK 0x20u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN5_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN5_EN 0x1

//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_7_EPEN6_POS 6
//! Field EPEN6 - EP Enable 6
#define EP_THRSD_EN_7_EPEN6_MASK 0x40u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN6_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN6_EN 0x1

//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_7_EPEN7_POS 7
//! Field EPEN7 - EP Enable 7
#define EP_THRSD_EN_7_EPEN7_MASK 0x80u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN7_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN7_EN 0x1

//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_7_EPEN8_POS 8
//! Field EPEN8 - EP Enable 8
#define EP_THRSD_EN_7_EPEN8_MASK 0x100u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN8_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN8_EN 0x1

//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_7_EPEN9_POS 9
//! Field EPEN9 - EP Enable 9
#define EP_THRSD_EN_7_EPEN9_MASK 0x200u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN9_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN9_EN 0x1

//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_7_EPEN10_POS 10
//! Field EPEN10 - EP Enable 10
#define EP_THRSD_EN_7_EPEN10_MASK 0x400u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN10_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN10_EN 0x1

//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_7_EPEN11_POS 11
//! Field EPEN11 - EP Enable 11
#define EP_THRSD_EN_7_EPEN11_MASK 0x800u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN11_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN11_EN 0x1

//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_7_EPEN12_POS 12
//! Field EPEN12 - EP Enable 12
#define EP_THRSD_EN_7_EPEN12_MASK 0x1000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN12_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN12_EN 0x1

//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_7_EPEN13_POS 13
//! Field EPEN13 - EP Enable 13
#define EP_THRSD_EN_7_EPEN13_MASK 0x2000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN13_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN13_EN 0x1

//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_7_EPEN14_POS 14
//! Field EPEN14 - EP Enable 14
#define EP_THRSD_EN_7_EPEN14_MASK 0x4000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN14_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN14_EN 0x1

//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_7_EPEN15_POS 15
//! Field EPEN15 - EP Enable 15
#define EP_THRSD_EN_7_EPEN15_MASK 0x8000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN15_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN15_EN 0x1

//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_7_EPEN16_POS 16
//! Field EPEN16 - EP Enable 16
#define EP_THRSD_EN_7_EPEN16_MASK 0x10000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN16_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN16_EN 0x1

//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_7_EPEN17_POS 17
//! Field EPEN17 - EP Enable 17
#define EP_THRSD_EN_7_EPEN17_MASK 0x20000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN17_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN17_EN 0x1

//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_7_EPEN18_POS 18
//! Field EPEN18 - EP Enable 18
#define EP_THRSD_EN_7_EPEN18_MASK 0x40000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN18_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN18_EN 0x1

//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_7_EPEN19_POS 19
//! Field EPEN19 - EP Enable 19
#define EP_THRSD_EN_7_EPEN19_MASK 0x80000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN19_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN19_EN 0x1

//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_7_EPEN20_POS 20
//! Field EPEN20 - EP Enable 20
#define EP_THRSD_EN_7_EPEN20_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN20_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN20_EN 0x1

//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_7_EPEN21_POS 21
//! Field EPEN21 - EP Enable 21
#define EP_THRSD_EN_7_EPEN21_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN21_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN21_EN 0x1

//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_7_EPEN22_POS 22
//! Field EPEN22 - EP Enable 22
#define EP_THRSD_EN_7_EPEN22_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN22_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN22_EN 0x1

//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_7_EPEN23_POS 23
//! Field EPEN23 - EP Enable 23
#define EP_THRSD_EN_7_EPEN23_MASK 0x800000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN23_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN23_EN 0x1

//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_7_EPEN24_POS 24
//! Field EPEN24 - EP Enable 24
#define EP_THRSD_EN_7_EPEN24_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN24_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN24_EN 0x1

//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_7_EPEN25_POS 25
//! Field EPEN25 - EP Enable 25
#define EP_THRSD_EN_7_EPEN25_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN25_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN25_EN 0x1

//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_7_EPEN26_POS 26
//! Field EPEN26 - EP Enable 26
#define EP_THRSD_EN_7_EPEN26_MASK 0x4000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN26_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN26_EN 0x1

//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_7_EPEN27_POS 27
//! Field EPEN27 - EP Enable 27
#define EP_THRSD_EN_7_EPEN27_MASK 0x8000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN27_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN27_EN 0x1

//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_7_EPEN28_POS 28
//! Field EPEN28 - EP Enable 28
#define EP_THRSD_EN_7_EPEN28_MASK 0x10000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN28_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN28_EN 0x1

//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_7_EPEN29_POS 29
//! Field EPEN29 - EP Enable 29
#define EP_THRSD_EN_7_EPEN29_MASK 0x20000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN29_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN29_EN 0x1

//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_7_EPEN30_POS 30
//! Field EPEN30 - EP Enable 30
#define EP_THRSD_EN_7_EPEN30_MASK 0x40000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN30_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN30_EN 0x1

//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_7_EPEN31_POS 31
//! Field EPEN31 - EP Enable 31
#define EP_THRSD_EN_7_EPEN31_MASK 0x80000000u
//! Constant DIS - DIS
#define CONST_EP_THRSD_EN_7_EPEN31_DIS 0x0
//! Constant EN - EN
#define CONST_EP_THRSD_EN_7_EPEN31_EN 0x1

//! @}

//! \defgroup EPON_EPON_MODE_REG Register EPON_EPON_MODE_REG - EPON Queue range register
//! @{

//! Register Offset (relative)
#define EPON_EPON_MODE_REG 0x120
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EPON_EPON_MODE_REG 0xE6680120u

//! Register Reset Value
#define EPON_EPON_MODE_REG_RST 0x00000000u

//! Field STARTQ - Start Queue
#define EPON_EPON_MODE_REG_STARTQ_POS 0
//! Field STARTQ - Start Queue
#define EPON_EPON_MODE_REG_STARTQ_MASK 0x1FFu

//! Field ENDQ - End Queue
#define EPON_EPON_MODE_REG_ENDQ_POS 10
//! Field ENDQ - End Queue
#define EPON_EPON_MODE_REG_ENDQ_MASK 0x7FC00u

//! Field EPONBASEPORT - EPON Base Port Value
#define EPON_EPON_MODE_REG_EPONBASEPORT_POS 20
//! Field EPONBASEPORT - EPON Base Port Value
#define EPON_EPON_MODE_REG_EPONBASEPORT_MASK 0xFF00000u

//! Field EPONPKTSIZADJ - EPON Packet size Adjustment enable
#define EPON_EPON_MODE_REG_EPONPKTSIZADJ_POS 30
//! Field EPONPKTSIZADJ - EPON Packet size Adjustment enable
#define EPON_EPON_MODE_REG_EPONPKTSIZADJ_MASK 0x40000000u

//! Field EPONCHKEN - EPON Mode Check Enable
#define EPON_EPON_MODE_REG_EPONCHKEN_POS 31
//! Field EPONCHKEN - EPON Mode Check Enable
#define EPON_EPON_MODE_REG_EPONCHKEN_MASK 0x80000000u

//! @}

//! \defgroup EPON_PORTBP_REG_0 Register EPON_PORTBP_REG_0 - EPON Mode backpressure enable register
//! @{

//! Register Offset (relative)
#define EPON_PORTBP_REG_0 0x124
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EPON_PORTBP_REG_0 0xE6680124u

//! Register Reset Value
#define EPON_PORTBP_REG_0_RST 0x00000000u

//! Field PORTBPEN - Port Back Pressure Enable
#define EPON_PORTBP_REG_0_PORTBPEN_POS 0
//! Field PORTBPEN - Port Back Pressure Enable
#define EPON_PORTBP_REG_0_PORTBPEN_MASK 0xFFFFFFFFu

//! @}

//! \defgroup EPON_PORTBP_REG_1 Register EPON_PORTBP_REG_1 - EPON Mode backpressure enable register
//! @{

//! Register Offset (relative)
#define EPON_PORTBP_REG_1 0x128
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EPON_PORTBP_REG_1 0xE6680128u

//! Register Reset Value
#define EPON_PORTBP_REG_1_RST 0x00000000u

//! Field PORTBPEN - Port Back Pressure Enable
#define EPON_PORTBP_REG_1_PORTBPEN_POS 0
//! Field PORTBPEN - Port Back Pressure Enable
#define EPON_PORTBP_REG_1_PORTBPEN_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CFG_CPU_IGP_0 Register CFG_CPU_IGP_0 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_0 0x1000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_0 0xE6681000u

//! Register Reset Value
#define CFG_CPU_IGP_0_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_0_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_0_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_0_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_0_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_0_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_0_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_0_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_0_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_0_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_0_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_0_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_0_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_0_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_0_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_0_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_0_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_0_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_0_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_0_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_0_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_0_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_0_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_0_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_0_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_0_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_0_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_0_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_0_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_0_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_0_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_0_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_0_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_0_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_0_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_0_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_0_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_0_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_0_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_0_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_0_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_0_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_0_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_0_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_0_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_0_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_0_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_0_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_0_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_0_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_0_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_0_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_0_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_0_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_0_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_0_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_0_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_0_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_0_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_0_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_0_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_0_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_0_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_0_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_0_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_0_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_0_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_0_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_0_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_0_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_0_FSQMQEN_EN 0x1

//! Field PSBKEEP - Keep PSB during conversion to QoS descriptor
#define CFG_CPU_IGP_0_PSBKEEP_POS 26
//! Field PSBKEEP - Keep PSB during conversion to QoS descriptor
#define CFG_CPU_IGP_0_PSBKEEP_MASK 0x4000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_0_PSBKEEP_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_0_PSBKEEP_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_0_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_0_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_0 Register EQPC_CPU_IGP_0 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_0 0x100C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_0 0xE668100Cu

//! Register Reset Value
#define EQPC_CPU_IGP_0_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_0_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_0_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_0 Register DISC_DESC0_CPU_IGP_0 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_0 0x1010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_0 0xE6681010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_0_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_0_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_0_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_0 Register DISC_DESC1_CPU_IGP_0 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_0 0x1014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_0 0xE6681014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_0_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_0_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_0_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_0 Register DISC_DESC2_CPU_IGP_0 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_0 0x1018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_0 0xE6681018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_0_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_0_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_0_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_0 Register DISC_DESC3_CPU_IGP_0 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_0 0x101C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_0 0xE668101Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_0_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_0_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_0_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_0 Register IRNCR_CPU_IGP_0 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_0 0x1020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_0 0xE6681020u

//! Register Reset Value
#define IRNCR_CPU_IGP_0_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_0_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_0_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_0_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_0_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_0_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_0_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_0_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_0_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_0_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_0_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_0_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_0_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_0_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_0_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_0_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_0_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_0_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_0_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_0_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_0_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_0_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_0_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_0_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_0_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_0_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_0_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_0_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_0_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_0 Register IRNICR_CPU_IGP_0 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_0 0x1024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_0 0xE6681024u

//! Register Reset Value
#define IRNICR_CPU_IGP_0_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_0_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_0_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_0_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_0_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_0_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_0_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_0_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_0_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_0_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_0_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_0_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_0_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_0_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_0_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_0 Register IRNEN_CPU_IGP_0 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_0 0x1028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_0 0xE6681028u

//! Register Reset Value
#define IRNEN_CPU_IGP_0_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_0_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_0_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_0_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_0_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_0_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_0_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_0_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_0_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_0_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_0_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_0_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_0_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_0_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_0_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_0_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_0_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_0_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_0_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_0_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_0_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_0_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_0_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_0_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_0_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_0_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_0_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_0_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_0_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_0 Register DICC_IGP_0 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_0 0x1038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_0 0xE6681038u

//! Register Reset Value
#define DICC_IGP_0_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_0_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_0_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_0 Register DROP_IGP_0 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_0 0x103C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_0 0xE668103Cu

//! Register Reset Value
#define DROP_IGP_0_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_0_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_0_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NPBPC_CPU_IGP_0 Register NPBPC_CPU_IGP_0 - CPU Ingress Port new packet buffer pointer counter
//! @{

//! Register Offset (relative)
#define NPBPC_CPU_IGP_0 0x1040
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NPBPC_CPU_IGP_0 0xE6681040u

//! Register Reset Value
#define NPBPC_CPU_IGP_0_RST 0x00000000u

//! Field NPBPC - Per CPU New Packet Buffer Pointer Counter
#define NPBPC_CPU_IGP_0_NPBPC_POS 0
//! Field NPBPC - Per CPU New Packet Buffer Pointer Counter
#define NPBPC_CPU_IGP_0_NPBPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS0PC_CPU_IGP_0 Register NS0PC_CPU_IGP_0 - CPU Ingress Port new size0 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS0PC_CPU_IGP_0 0x1044
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS0PC_CPU_IGP_0 0xE6681044u

//! Register Reset Value
#define NS0PC_CPU_IGP_0_RST 0x00000000u

//! Field NS0PC - Per CPU New Size0 Buffer Pointer Counter
#define NS0PC_CPU_IGP_0_NS0PC_POS 0
//! Field NS0PC - Per CPU New Size0 Buffer Pointer Counter
#define NS0PC_CPU_IGP_0_NS0PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS1PC_CPU_IGP_0 Register NS1PC_CPU_IGP_0 - CPU Ingress Port new size1 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS1PC_CPU_IGP_0 0x1048
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS1PC_CPU_IGP_0 0xE6681048u

//! Register Reset Value
#define NS1PC_CPU_IGP_0_RST 0x00000000u

//! Field NS1PC - Per CPU New Size1 Buffer Pointer Counter
#define NS1PC_CPU_IGP_0_NS1PC_POS 0
//! Field NS1PC - Per CPU New Size1 Buffer Pointer Counter
#define NS1PC_CPU_IGP_0_NS1PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_0 Register NS2PC_CPU_IGP_0 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_0 0x104C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_0 0xE668104Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_0_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_0_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_0_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS3PC_CPU_IGP_0 Register NS3PC_CPU_IGP_0 - CPU Ingress Port new size3 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS3PC_CPU_IGP_0 0x1050
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS3PC_CPU_IGP_0 0xE6681050u

//! Register Reset Value
#define NS3PC_CPU_IGP_0_RST 0x00000000u

//! Field NS3PC - Per CPU New Size3 Buffer Pointer Counter
#define NS3PC_CPU_IGP_0_NS3PC_POS 0
//! Field NS3PC - Per CPU New Size3 Buffer Pointer Counter
#define NS3PC_CPU_IGP_0_NS3PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_0 Register DCNTR_IGP_0 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_0 0x1060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_0 0xE6681060u

//! Register Reset Value
#define DCNTR_IGP_0_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_0_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_0_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_0_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_0_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_PBPTR_CPU_IGP_0 Register NEW_PBPTR_CPU_IGP_0 - CPU Ingress Port New Packet Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_PBPTR_CPU_IGP_0 0x1080
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_PBPTR_CPU_IGP_0 0xE6681080u

//! Register Reset Value
#define NEW_PBPTR_CPU_IGP_0_RST 0xFFFFFF80u

//! Field OFFSET - Offset
#define NEW_PBPTR_CPU_IGP_0_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_PBPTR_CPU_IGP_0_OFFSET_MASK 0x7Fu

//! Field SA - Segment Address
#define NEW_PBPTR_CPU_IGP_0_SA_POS 7
//! Field SA - Segment Address
#define NEW_PBPTR_CPU_IGP_0_SA_MASK 0xFFFFFF80u

//! @}

//! \defgroup NEW_PBPTR1_CPU_IGP_0 Register NEW_PBPTR1_CPU_IGP_0 - CPU Ingress Port New Packet Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_PBPTR1_CPU_IGP_0 0x1084
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_PBPTR1_CPU_IGP_0 0xE6681084u

//! Register Reset Value
#define NEW_PBPTR1_CPU_IGP_0_RST 0x07800000u

//! Field SA - Segment Address Upper bits
#define NEW_PBPTR1_CPU_IGP_0_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_PBPTR1_CPU_IGP_0_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S0PTR_CPU_IGP_0 Register NEW_S0PTR_CPU_IGP_0 - CPU Ingress Port New Size0 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S0PTR_CPU_IGP_0 0x1090
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S0PTR_CPU_IGP_0 0xE6681090u

//! Register Reset Value
#define NEW_S0PTR_CPU_IGP_0_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S0PTR_CPU_IGP_0_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S0PTR_CPU_IGP_0_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S0PTR_CPU_IGP_0_SA_POS 9
//! Field SA - Segment Address
#define NEW_S0PTR_CPU_IGP_0_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S0PTR1_CPU_IGP_0 Register NEW_S0PTR1_CPU_IGP_0 - CPU Ingress Port New Size0 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S0PTR1_CPU_IGP_0 0x1094
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S0PTR1_CPU_IGP_0 0xE6681094u

//! Register Reset Value
#define NEW_S0PTR1_CPU_IGP_0_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S0PTR1_CPU_IGP_0_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S0PTR1_CPU_IGP_0_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S0PTR1_CPU_IGP_0_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S0PTR1_CPU_IGP_0_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S1PTR_CPU_IGP_0 Register NEW_S1PTR_CPU_IGP_0 - CPU Ingress Port New Size1 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S1PTR_CPU_IGP_0 0x10A0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S1PTR_CPU_IGP_0 0xE66810A0u

//! Register Reset Value
#define NEW_S1PTR_CPU_IGP_0_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S1PTR_CPU_IGP_0_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S1PTR_CPU_IGP_0_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S1PTR_CPU_IGP_0_SA_POS 9
//! Field SA - Segment Address
#define NEW_S1PTR_CPU_IGP_0_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S1PTR1_CPU_IGP_0 Register NEW_S1PTR1_CPU_IGP_0 - CPU Ingress Port New Size1 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S1PTR1_CPU_IGP_0 0x10A4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S1PTR1_CPU_IGP_0 0xE66810A4u

//! Register Reset Value
#define NEW_S1PTR1_CPU_IGP_0_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S1PTR1_CPU_IGP_0_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S1PTR1_CPU_IGP_0_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S1PTR1_CPU_IGP_0_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S1PTR1_CPU_IGP_0_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_0 Register NEW_S2PTR_CPU_IGP_0 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_0 0x10B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_0 0xE66810B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_0_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_0_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_0_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_0_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_0_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_0 Register NEW_S2PTR1_CPU_IGP_0 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_0 0x10B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_0 0xE66810B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_0_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_0_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_0_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_0_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_0_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S3PTR_CPU_IGP_0 Register NEW_S3PTR_CPU_IGP_0 - CPU Ingress Port New Size3 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S3PTR_CPU_IGP_0 0x10C0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S3PTR_CPU_IGP_0 0xE66810C0u

//! Register Reset Value
#define NEW_S3PTR_CPU_IGP_0_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S3PTR_CPU_IGP_0_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S3PTR_CPU_IGP_0_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S3PTR_CPU_IGP_0_SA_POS 9
//! Field SA - Segment Address
#define NEW_S3PTR_CPU_IGP_0_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S3PTR1_CPU_IGP_0 Register NEW_S3PTR1_CPU_IGP_0 - CPU Ingress Port New Size3 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S3PTR1_CPU_IGP_0 0x10C4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S3PTR1_CPU_IGP_0 0xE66810C4u

//! Register Reset Value
#define NEW_S3PTR1_CPU_IGP_0_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S3PTR1_CPU_IGP_0_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S3PTR1_CPU_IGP_0_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S3PTR1_CPU_IGP_0_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S3PTR1_CPU_IGP_0_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_0 Register DESC0_0_CPU_IGP_0 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_0 0x1100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_0 0xE6681100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_0_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_0_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_0_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_0 Register DESC1_0_CPU_IGP_0 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_0 0x1104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_0 0xE6681104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_0_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_0_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_0_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_0 Register DESC2_0_CPU_IGP_0 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_0 0x1108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_0 0xE6681108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_0_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_0_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_0_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_0 Register DESC3_0_CPU_IGP_0 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_0 0x110C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_0 0xE668110Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_0_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_0_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_0_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_0 Register DESC0_1_CPU_IGP_0 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_0 0x1110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_0 0xE6681110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_0_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_0_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_0_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_0 Register DESC1_1_CPU_IGP_0 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_0 0x1114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_0 0xE6681114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_0_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_0_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_0_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_0 Register DESC2_1_CPU_IGP_0 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_0 0x1118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_0 0xE6681118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_0_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_0_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_0_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_0 Register DESC3_1_CPU_IGP_0 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_0 0x111C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_0 0xE668111Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_0_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_0_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_0_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_0_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_0_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_1 Register CFG_CPU_IGP_1 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_1 0x2000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_1 0xE6682000u

//! Register Reset Value
#define CFG_CPU_IGP_1_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_1_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_1_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_1_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_1_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_1_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_1_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_1_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_1_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_1_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_1_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_1_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_1_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_1_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_1_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_1_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_1_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_1_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_1_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_1_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_1_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_1_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_1_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_1_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_1_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_1_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_1_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_1_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_1_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_1_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_1_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_1_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_1_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_1_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_1_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_1_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_1_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_1_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_1_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_1_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_1_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_1_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_1_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_1_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_1_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_1_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_1_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_1_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_1_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_1_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_1_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_1_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_1_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_1_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_1_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_1_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_1_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_1_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_1_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_1_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_1_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_1_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_1_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_1_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_1_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_1_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_1_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_1_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_1_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_1_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_1_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_1_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_1_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_1 Register EQPC_CPU_IGP_1 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_1 0x200C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_1 0xE668200Cu

//! Register Reset Value
#define EQPC_CPU_IGP_1_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_1_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_1_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_1 Register DISC_DESC0_CPU_IGP_1 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_1 0x2010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_1 0xE6682010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_1_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_1_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_1_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_1 Register DISC_DESC1_CPU_IGP_1 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_1 0x2014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_1 0xE6682014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_1_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_1_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_1_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_1 Register DISC_DESC2_CPU_IGP_1 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_1 0x2018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_1 0xE6682018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_1_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_1_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_1_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_1 Register DISC_DESC3_CPU_IGP_1 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_1 0x201C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_1 0xE668201Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_1_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_1_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_1_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_1 Register IRNCR_CPU_IGP_1 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_1 0x2020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_1 0xE6682020u

//! Register Reset Value
#define IRNCR_CPU_IGP_1_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_1_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_1_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_1_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_1_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_1_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_1_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_1_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_1_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_1_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_1_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_1_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_1_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_1_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_1_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_1_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_1_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_1_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_1_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_1_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_1_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_1_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_1_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_1_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_1_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_1_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_1_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_1_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_1_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_1 Register IRNICR_CPU_IGP_1 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_1 0x2024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_1 0xE6682024u

//! Register Reset Value
#define IRNICR_CPU_IGP_1_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_1_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_1_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_1_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_1_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_1_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_1_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_1_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_1_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_1_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_1_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_1_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_1_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_1_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_1_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_1 Register IRNEN_CPU_IGP_1 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_1 0x2028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_1 0xE6682028u

//! Register Reset Value
#define IRNEN_CPU_IGP_1_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_1_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_1_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_1_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_1_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_1_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_1_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_1_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_1_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_1_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_1_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_1_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_1_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_1_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_1_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_1_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_1_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_1_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_1_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_1_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_1_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_1_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_1_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_1_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_1_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_1_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_1_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_1_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_1_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_1 Register DICC_IGP_1 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_1 0x2038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_1 0xE6682038u

//! Register Reset Value
#define DICC_IGP_1_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_1_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_1_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_1 Register DROP_IGP_1 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_1 0x203C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_1 0xE668203Cu

//! Register Reset Value
#define DROP_IGP_1_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_1_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_1_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NPBPC_CPU_IGP_1 Register NPBPC_CPU_IGP_1 - CPU Ingress Port new packet buffer pointer counter
//! @{

//! Register Offset (relative)
#define NPBPC_CPU_IGP_1 0x2040
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NPBPC_CPU_IGP_1 0xE6682040u

//! Register Reset Value
#define NPBPC_CPU_IGP_1_RST 0x00000000u

//! Field NPBPC - Per CPU New Packet Buffer Pointer Counter
#define NPBPC_CPU_IGP_1_NPBPC_POS 0
//! Field NPBPC - Per CPU New Packet Buffer Pointer Counter
#define NPBPC_CPU_IGP_1_NPBPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS0PC_CPU_IGP_1 Register NS0PC_CPU_IGP_1 - CPU Ingress Port new size0 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS0PC_CPU_IGP_1 0x2044
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS0PC_CPU_IGP_1 0xE6682044u

//! Register Reset Value
#define NS0PC_CPU_IGP_1_RST 0x00000000u

//! Field NS0PC - Per CPU New Size0 Buffer Pointer Counter
#define NS0PC_CPU_IGP_1_NS0PC_POS 0
//! Field NS0PC - Per CPU New Size0 Buffer Pointer Counter
#define NS0PC_CPU_IGP_1_NS0PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS1PC_CPU_IGP_1 Register NS1PC_CPU_IGP_1 - CPU Ingress Port new size1 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS1PC_CPU_IGP_1 0x2048
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS1PC_CPU_IGP_1 0xE6682048u

//! Register Reset Value
#define NS1PC_CPU_IGP_1_RST 0x00000000u

//! Field NS1PC - Per CPU New Size1 Buffer Pointer Counter
#define NS1PC_CPU_IGP_1_NS1PC_POS 0
//! Field NS1PC - Per CPU New Size1 Buffer Pointer Counter
#define NS1PC_CPU_IGP_1_NS1PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_1 Register NS2PC_CPU_IGP_1 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_1 0x204C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_1 0xE668204Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_1_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_1_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_1_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS3PC_CPU_IGP_1 Register NS3PC_CPU_IGP_1 - CPU Ingress Port new size3 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS3PC_CPU_IGP_1 0x2050
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS3PC_CPU_IGP_1 0xE6682050u

//! Register Reset Value
#define NS3PC_CPU_IGP_1_RST 0x00000000u

//! Field NS3PC - Per CPU New Size3 Buffer Pointer Counter
#define NS3PC_CPU_IGP_1_NS3PC_POS 0
//! Field NS3PC - Per CPU New Size3 Buffer Pointer Counter
#define NS3PC_CPU_IGP_1_NS3PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_1 Register DCNTR_IGP_1 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_1 0x2060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_1 0xE6682060u

//! Register Reset Value
#define DCNTR_IGP_1_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_1_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_1_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_1_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_1_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_PBPTR_CPU_IGP_1 Register NEW_PBPTR_CPU_IGP_1 - CPU Ingress Port New Packet Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_PBPTR_CPU_IGP_1 0x2080
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_PBPTR_CPU_IGP_1 0xE6682080u

//! Register Reset Value
#define NEW_PBPTR_CPU_IGP_1_RST 0xFFFFFF80u

//! Field OFFSET - Offset
#define NEW_PBPTR_CPU_IGP_1_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_PBPTR_CPU_IGP_1_OFFSET_MASK 0x7Fu

//! Field SA - Segment Address
#define NEW_PBPTR_CPU_IGP_1_SA_POS 7
//! Field SA - Segment Address
#define NEW_PBPTR_CPU_IGP_1_SA_MASK 0xFFFFFF80u

//! @}

//! \defgroup NEW_PBPTR1_CPU_IGP_1 Register NEW_PBPTR1_CPU_IGP_1 - CPU Ingress Port New Packet Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_PBPTR1_CPU_IGP_1 0x2084
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_PBPTR1_CPU_IGP_1 0xE6682084u

//! Register Reset Value
#define NEW_PBPTR1_CPU_IGP_1_RST 0x07800000u

//! Field SA - Segment Address Upper bits
#define NEW_PBPTR1_CPU_IGP_1_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_PBPTR1_CPU_IGP_1_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S0PTR_CPU_IGP_1 Register NEW_S0PTR_CPU_IGP_1 - CPU Ingress Port New Size0 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S0PTR_CPU_IGP_1 0x2090
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S0PTR_CPU_IGP_1 0xE6682090u

//! Register Reset Value
#define NEW_S0PTR_CPU_IGP_1_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S0PTR_CPU_IGP_1_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S0PTR_CPU_IGP_1_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S0PTR_CPU_IGP_1_SA_POS 9
//! Field SA - Segment Address
#define NEW_S0PTR_CPU_IGP_1_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S0PTR1_CPU_IGP_1 Register NEW_S0PTR1_CPU_IGP_1 - CPU Ingress Port New Size0 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S0PTR1_CPU_IGP_1 0x2094
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S0PTR1_CPU_IGP_1 0xE6682094u

//! Register Reset Value
#define NEW_S0PTR1_CPU_IGP_1_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S0PTR1_CPU_IGP_1_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S0PTR1_CPU_IGP_1_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S0PTR1_CPU_IGP_1_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S0PTR1_CPU_IGP_1_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S1PTR_CPU_IGP_1 Register NEW_S1PTR_CPU_IGP_1 - CPU Ingress Port New Size1 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S1PTR_CPU_IGP_1 0x20A0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S1PTR_CPU_IGP_1 0xE66820A0u

//! Register Reset Value
#define NEW_S1PTR_CPU_IGP_1_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S1PTR_CPU_IGP_1_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S1PTR_CPU_IGP_1_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S1PTR_CPU_IGP_1_SA_POS 9
//! Field SA - Segment Address
#define NEW_S1PTR_CPU_IGP_1_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S1PTR1_CPU_IGP_1 Register NEW_S1PTR1_CPU_IGP_1 - CPU Ingress Port New Size1 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S1PTR1_CPU_IGP_1 0x20A4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S1PTR1_CPU_IGP_1 0xE66820A4u

//! Register Reset Value
#define NEW_S1PTR1_CPU_IGP_1_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S1PTR1_CPU_IGP_1_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S1PTR1_CPU_IGP_1_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S1PTR1_CPU_IGP_1_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S1PTR1_CPU_IGP_1_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_1 Register NEW_S2PTR_CPU_IGP_1 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_1 0x20B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_1 0xE66820B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_1_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_1_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_1_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_1_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_1_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_1 Register NEW_S2PTR1_CPU_IGP_1 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_1 0x20B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_1 0xE66820B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_1_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_1_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_1_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_1_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_1_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S3PTR_CPU_IGP_1 Register NEW_S3PTR_CPU_IGP_1 - CPU Ingress Port New Size3 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S3PTR_CPU_IGP_1 0x20C0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S3PTR_CPU_IGP_1 0xE66820C0u

//! Register Reset Value
#define NEW_S3PTR_CPU_IGP_1_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S3PTR_CPU_IGP_1_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S3PTR_CPU_IGP_1_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S3PTR_CPU_IGP_1_SA_POS 9
//! Field SA - Segment Address
#define NEW_S3PTR_CPU_IGP_1_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S3PTR1_CPU_IGP_1 Register NEW_S3PTR1_CPU_IGP_1 - CPU Ingress Port New Size3 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S3PTR1_CPU_IGP_1 0x20C4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S3PTR1_CPU_IGP_1 0xE66820C4u

//! Register Reset Value
#define NEW_S3PTR1_CPU_IGP_1_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S3PTR1_CPU_IGP_1_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S3PTR1_CPU_IGP_1_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S3PTR1_CPU_IGP_1_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S3PTR1_CPU_IGP_1_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_1 Register DESC0_0_CPU_IGP_1 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_1 0x2100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_1 0xE6682100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_1_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_1_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_1_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_1 Register DESC1_0_CPU_IGP_1 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_1 0x2104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_1 0xE6682104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_1_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_1_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_1_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_1 Register DESC2_0_CPU_IGP_1 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_1 0x2108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_1 0xE6682108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_1_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_1_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_1_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_1 Register DESC3_0_CPU_IGP_1 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_1 0x210C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_1 0xE668210Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_1_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_1_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_1_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_1 Register DESC0_1_CPU_IGP_1 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_1 0x2110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_1 0xE6682110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_1_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_1_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_1_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_1 Register DESC1_1_CPU_IGP_1 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_1 0x2114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_1 0xE6682114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_1_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_1_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_1_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_1 Register DESC2_1_CPU_IGP_1 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_1 0x2118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_1 0xE6682118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_1_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_1_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_1_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_1 Register DESC3_1_CPU_IGP_1 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_1 0x211C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_1 0xE668211Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_1_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_1_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_1_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_1_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_1_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_2 Register CFG_CPU_IGP_2 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_2 0x3000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_2 0xE6683000u

//! Register Reset Value
#define CFG_CPU_IGP_2_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_2_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_2_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_2_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_2_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_2_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_2_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_2_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_2_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_2_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_2_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_2_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_2_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_2_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_2_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_2_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_2_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_2_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_2_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_2_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_2_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_2_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_2_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_2_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_2_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_2_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_2_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_2_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_2_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_2_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_2_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_2_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_2_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_2_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_2_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_2_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_2_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_2_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_2_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_2_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_2_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_2_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_2_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_2_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_2_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_2_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_2_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_2_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_2_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_2_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_2_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_2_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_2_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_2_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_2_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_2_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_2_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_2_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_2_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_2_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_2_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_2_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_2_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_2_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_2_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_2_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_2_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_2_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_2_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_2_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_2_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_2_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_2_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_2 Register EQPC_CPU_IGP_2 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_2 0x300C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_2 0xE668300Cu

//! Register Reset Value
#define EQPC_CPU_IGP_2_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_2_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_2_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_2 Register DISC_DESC0_CPU_IGP_2 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_2 0x3010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_2 0xE6683010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_2_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_2_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_2_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_2 Register DISC_DESC1_CPU_IGP_2 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_2 0x3014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_2 0xE6683014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_2_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_2_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_2_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_2 Register DISC_DESC2_CPU_IGP_2 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_2 0x3018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_2 0xE6683018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_2_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_2_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_2_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_2 Register DISC_DESC3_CPU_IGP_2 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_2 0x301C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_2 0xE668301Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_2_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_2_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_2_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_2 Register IRNCR_CPU_IGP_2 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_2 0x3020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_2 0xE6683020u

//! Register Reset Value
#define IRNCR_CPU_IGP_2_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_2_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_2_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_2_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_2_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_2_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_2_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_2_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_2_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_2_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_2_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_2_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_2_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_2_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_2_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_2_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_2_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_2_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_2_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_2_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_2_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_2_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_2_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_2_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_2_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_2_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_2_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_2_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_2_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_2 Register IRNICR_CPU_IGP_2 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_2 0x3024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_2 0xE6683024u

//! Register Reset Value
#define IRNICR_CPU_IGP_2_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_2_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_2_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_2_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_2_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_2_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_2_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_2_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_2_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_2_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_2_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_2_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_2_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_2_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_2_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_2 Register IRNEN_CPU_IGP_2 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_2 0x3028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_2 0xE6683028u

//! Register Reset Value
#define IRNEN_CPU_IGP_2_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_2_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_2_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_2_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_2_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_2_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_2_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_2_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_2_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_2_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_2_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_2_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_2_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_2_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_2_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_2_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_2_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_2_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_2_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_2_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_2_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_2_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_2_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_2_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_2_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_2_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_2_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_2_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_2_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_2 Register DICC_IGP_2 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_2 0x3038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_2 0xE6683038u

//! Register Reset Value
#define DICC_IGP_2_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_2_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_2_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_2 Register DROP_IGP_2 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_2 0x303C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_2 0xE668303Cu

//! Register Reset Value
#define DROP_IGP_2_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_2_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_2_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NPBPC_CPU_IGP_2 Register NPBPC_CPU_IGP_2 - CPU Ingress Port new packet buffer pointer counter
//! @{

//! Register Offset (relative)
#define NPBPC_CPU_IGP_2 0x3040
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NPBPC_CPU_IGP_2 0xE6683040u

//! Register Reset Value
#define NPBPC_CPU_IGP_2_RST 0x00000000u

//! Field NPBPC - Per CPU New Packet Buffer Pointer Counter
#define NPBPC_CPU_IGP_2_NPBPC_POS 0
//! Field NPBPC - Per CPU New Packet Buffer Pointer Counter
#define NPBPC_CPU_IGP_2_NPBPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS0PC_CPU_IGP_2 Register NS0PC_CPU_IGP_2 - CPU Ingress Port new size0 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS0PC_CPU_IGP_2 0x3044
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS0PC_CPU_IGP_2 0xE6683044u

//! Register Reset Value
#define NS0PC_CPU_IGP_2_RST 0x00000000u

//! Field NS0PC - Per CPU New Size0 Buffer Pointer Counter
#define NS0PC_CPU_IGP_2_NS0PC_POS 0
//! Field NS0PC - Per CPU New Size0 Buffer Pointer Counter
#define NS0PC_CPU_IGP_2_NS0PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS1PC_CPU_IGP_2 Register NS1PC_CPU_IGP_2 - CPU Ingress Port new size1 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS1PC_CPU_IGP_2 0x3048
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS1PC_CPU_IGP_2 0xE6683048u

//! Register Reset Value
#define NS1PC_CPU_IGP_2_RST 0x00000000u

//! Field NS1PC - Per CPU New Size1 Buffer Pointer Counter
#define NS1PC_CPU_IGP_2_NS1PC_POS 0
//! Field NS1PC - Per CPU New Size1 Buffer Pointer Counter
#define NS1PC_CPU_IGP_2_NS1PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_2 Register NS2PC_CPU_IGP_2 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_2 0x304C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_2 0xE668304Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_2_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_2_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_2_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS3PC_CPU_IGP_2 Register NS3PC_CPU_IGP_2 - CPU Ingress Port new size3 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS3PC_CPU_IGP_2 0x3050
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS3PC_CPU_IGP_2 0xE6683050u

//! Register Reset Value
#define NS3PC_CPU_IGP_2_RST 0x00000000u

//! Field NS3PC - Per CPU New Size3 Buffer Pointer Counter
#define NS3PC_CPU_IGP_2_NS3PC_POS 0
//! Field NS3PC - Per CPU New Size3 Buffer Pointer Counter
#define NS3PC_CPU_IGP_2_NS3PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_2 Register DCNTR_IGP_2 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_2 0x3060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_2 0xE6683060u

//! Register Reset Value
#define DCNTR_IGP_2_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_2_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_2_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_2_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_2_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_PBPTR_CPU_IGP_2 Register NEW_PBPTR_CPU_IGP_2 - CPU Ingress Port New Packet Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_PBPTR_CPU_IGP_2 0x3080
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_PBPTR_CPU_IGP_2 0xE6683080u

//! Register Reset Value
#define NEW_PBPTR_CPU_IGP_2_RST 0xFFFFFF80u

//! Field OFFSET - Offset
#define NEW_PBPTR_CPU_IGP_2_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_PBPTR_CPU_IGP_2_OFFSET_MASK 0x7Fu

//! Field SA - Segment Address
#define NEW_PBPTR_CPU_IGP_2_SA_POS 7
//! Field SA - Segment Address
#define NEW_PBPTR_CPU_IGP_2_SA_MASK 0xFFFFFF80u

//! @}

//! \defgroup NEW_PBPTR1_CPU_IGP_2 Register NEW_PBPTR1_CPU_IGP_2 - CPU Ingress Port New Packet Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_PBPTR1_CPU_IGP_2 0x3084
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_PBPTR1_CPU_IGP_2 0xE6683084u

//! Register Reset Value
#define NEW_PBPTR1_CPU_IGP_2_RST 0x07800000u

//! Field SA - Segment Address Upper bits
#define NEW_PBPTR1_CPU_IGP_2_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_PBPTR1_CPU_IGP_2_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S0PTR_CPU_IGP_2 Register NEW_S0PTR_CPU_IGP_2 - CPU Ingress Port New Size0 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S0PTR_CPU_IGP_2 0x3090
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S0PTR_CPU_IGP_2 0xE6683090u

//! Register Reset Value
#define NEW_S0PTR_CPU_IGP_2_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S0PTR_CPU_IGP_2_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S0PTR_CPU_IGP_2_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S0PTR_CPU_IGP_2_SA_POS 9
//! Field SA - Segment Address
#define NEW_S0PTR_CPU_IGP_2_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S0PTR1_CPU_IGP_2 Register NEW_S0PTR1_CPU_IGP_2 - CPU Ingress Port New Size0 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S0PTR1_CPU_IGP_2 0x3094
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S0PTR1_CPU_IGP_2 0xE6683094u

//! Register Reset Value
#define NEW_S0PTR1_CPU_IGP_2_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S0PTR1_CPU_IGP_2_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S0PTR1_CPU_IGP_2_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S0PTR1_CPU_IGP_2_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S0PTR1_CPU_IGP_2_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S1PTR_CPU_IGP_2 Register NEW_S1PTR_CPU_IGP_2 - CPU Ingress Port New Size1 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S1PTR_CPU_IGP_2 0x30A0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S1PTR_CPU_IGP_2 0xE66830A0u

//! Register Reset Value
#define NEW_S1PTR_CPU_IGP_2_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S1PTR_CPU_IGP_2_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S1PTR_CPU_IGP_2_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S1PTR_CPU_IGP_2_SA_POS 9
//! Field SA - Segment Address
#define NEW_S1PTR_CPU_IGP_2_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S1PTR1_CPU_IGP_2 Register NEW_S1PTR1_CPU_IGP_2 - CPU Ingress Port New Size1 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S1PTR1_CPU_IGP_2 0x30A4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S1PTR1_CPU_IGP_2 0xE66830A4u

//! Register Reset Value
#define NEW_S1PTR1_CPU_IGP_2_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S1PTR1_CPU_IGP_2_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S1PTR1_CPU_IGP_2_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S1PTR1_CPU_IGP_2_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S1PTR1_CPU_IGP_2_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_2 Register NEW_S2PTR_CPU_IGP_2 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_2 0x30B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_2 0xE66830B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_2_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_2_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_2_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_2_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_2_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_2 Register NEW_S2PTR1_CPU_IGP_2 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_2 0x30B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_2 0xE66830B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_2_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_2_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_2_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_2_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_2_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S3PTR_CPU_IGP_2 Register NEW_S3PTR_CPU_IGP_2 - CPU Ingress Port New Size3 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S3PTR_CPU_IGP_2 0x30C0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S3PTR_CPU_IGP_2 0xE66830C0u

//! Register Reset Value
#define NEW_S3PTR_CPU_IGP_2_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S3PTR_CPU_IGP_2_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S3PTR_CPU_IGP_2_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S3PTR_CPU_IGP_2_SA_POS 9
//! Field SA - Segment Address
#define NEW_S3PTR_CPU_IGP_2_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S3PTR1_CPU_IGP_2 Register NEW_S3PTR1_CPU_IGP_2 - CPU Ingress Port New Size3 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S3PTR1_CPU_IGP_2 0x30C4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S3PTR1_CPU_IGP_2 0xE66830C4u

//! Register Reset Value
#define NEW_S3PTR1_CPU_IGP_2_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S3PTR1_CPU_IGP_2_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S3PTR1_CPU_IGP_2_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S3PTR1_CPU_IGP_2_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S3PTR1_CPU_IGP_2_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_2 Register DESC0_0_CPU_IGP_2 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_2 0x3100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_2 0xE6683100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_2_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_2_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_2_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_2 Register DESC1_0_CPU_IGP_2 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_2 0x3104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_2 0xE6683104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_2_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_2_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_2_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_2 Register DESC2_0_CPU_IGP_2 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_2 0x3108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_2 0xE6683108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_2_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_2_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_2_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_2 Register DESC3_0_CPU_IGP_2 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_2 0x310C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_2 0xE668310Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_2_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_2_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_2_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_2 Register DESC0_1_CPU_IGP_2 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_2 0x3110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_2 0xE6683110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_2_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_2_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_2_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_2 Register DESC1_1_CPU_IGP_2 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_2 0x3114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_2 0xE6683114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_2_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_2_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_2_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_2 Register DESC2_1_CPU_IGP_2 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_2 0x3118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_2 0xE6683118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_2_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_2_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_2_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_2 Register DESC3_1_CPU_IGP_2 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_2 0x311C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_2 0xE668311Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_2_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_2_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_2_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_2_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_2_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_3 Register CFG_CPU_IGP_3 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_3 0x4000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_3 0xE6684000u

//! Register Reset Value
#define CFG_CPU_IGP_3_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_3_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_3_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_3_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_3_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_3_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_3_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_3_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_3_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_3_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_3_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_3_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_3_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_3_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_3_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_3_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_3_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_3_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_3_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_3_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_3_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_3_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_3_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_3_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_3_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_3_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_3_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_3_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_3_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_3_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_3_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_3_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_3_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_3_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_3_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_3_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_3_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_3_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_3_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_3_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_3_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_3_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_3_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_3_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_3_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_3_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_3_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_3_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_3_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_3_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_3_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_3_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_3_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_3_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_3_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_3_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_3_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_3_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_3_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_3_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_3_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_3_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_3_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_3_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_3_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_3_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_3_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_3_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_3_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_3_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_3_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_3_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_3_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_3 Register EQPC_CPU_IGP_3 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_3 0x400C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_3 0xE668400Cu

//! Register Reset Value
#define EQPC_CPU_IGP_3_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_3_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_3_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_3 Register DISC_DESC0_CPU_IGP_3 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_3 0x4010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_3 0xE6684010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_3_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_3_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_3_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_3 Register DISC_DESC1_CPU_IGP_3 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_3 0x4014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_3 0xE6684014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_3_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_3_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_3_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_3 Register DISC_DESC2_CPU_IGP_3 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_3 0x4018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_3 0xE6684018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_3_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_3_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_3_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_3 Register DISC_DESC3_CPU_IGP_3 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_3 0x401C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_3 0xE668401Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_3_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_3_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_3_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_3 Register IRNCR_CPU_IGP_3 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_3 0x4020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_3 0xE6684020u

//! Register Reset Value
#define IRNCR_CPU_IGP_3_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_3_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_3_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_3_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_3_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_3_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_3_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_3_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_3_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_3_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_3_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_3_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_3_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_3_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_3_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_3_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_3_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_3_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_3_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_3_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_3_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_3_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_3_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_3_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_3_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_3_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_3_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_3_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_3_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_3 Register IRNICR_CPU_IGP_3 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_3 0x4024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_3 0xE6684024u

//! Register Reset Value
#define IRNICR_CPU_IGP_3_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_3_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_3_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_3_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_3_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_3_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_3_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_3_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_3_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_3_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_3_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_3_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_3_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_3_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_3_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_3 Register IRNEN_CPU_IGP_3 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_3 0x4028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_3 0xE6684028u

//! Register Reset Value
#define IRNEN_CPU_IGP_3_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_3_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_3_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_3_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_3_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_3_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_3_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_3_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_3_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_3_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_3_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_3_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_3_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_3_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_3_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_3_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_3_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_3_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_3_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_3_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_3_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_3_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_3_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_3_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_3_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_3_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_3_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_3_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_3_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_3 Register DICC_IGP_3 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_3 0x4038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_3 0xE6684038u

//! Register Reset Value
#define DICC_IGP_3_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_3_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_3_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_3 Register DROP_IGP_3 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_3 0x403C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_3 0xE668403Cu

//! Register Reset Value
#define DROP_IGP_3_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_3_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_3_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NPBPC_CPU_IGP_3 Register NPBPC_CPU_IGP_3 - CPU Ingress Port new packet buffer pointer counter
//! @{

//! Register Offset (relative)
#define NPBPC_CPU_IGP_3 0x4040
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NPBPC_CPU_IGP_3 0xE6684040u

//! Register Reset Value
#define NPBPC_CPU_IGP_3_RST 0x00000000u

//! Field NPBPC - Per CPU New Packet Buffer Pointer Counter
#define NPBPC_CPU_IGP_3_NPBPC_POS 0
//! Field NPBPC - Per CPU New Packet Buffer Pointer Counter
#define NPBPC_CPU_IGP_3_NPBPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS0PC_CPU_IGP_3 Register NS0PC_CPU_IGP_3 - CPU Ingress Port new size0 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS0PC_CPU_IGP_3 0x4044
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS0PC_CPU_IGP_3 0xE6684044u

//! Register Reset Value
#define NS0PC_CPU_IGP_3_RST 0x00000000u

//! Field NS0PC - Per CPU New Size0 Buffer Pointer Counter
#define NS0PC_CPU_IGP_3_NS0PC_POS 0
//! Field NS0PC - Per CPU New Size0 Buffer Pointer Counter
#define NS0PC_CPU_IGP_3_NS0PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS1PC_CPU_IGP_3 Register NS1PC_CPU_IGP_3 - CPU Ingress Port new size1 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS1PC_CPU_IGP_3 0x4048
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS1PC_CPU_IGP_3 0xE6684048u

//! Register Reset Value
#define NS1PC_CPU_IGP_3_RST 0x00000000u

//! Field NS1PC - Per CPU New Size1 Buffer Pointer Counter
#define NS1PC_CPU_IGP_3_NS1PC_POS 0
//! Field NS1PC - Per CPU New Size1 Buffer Pointer Counter
#define NS1PC_CPU_IGP_3_NS1PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_3 Register NS2PC_CPU_IGP_3 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_3 0x404C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_3 0xE668404Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_3_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_3_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_3_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS3PC_CPU_IGP_3 Register NS3PC_CPU_IGP_3 - CPU Ingress Port new size3 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS3PC_CPU_IGP_3 0x4050
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS3PC_CPU_IGP_3 0xE6684050u

//! Register Reset Value
#define NS3PC_CPU_IGP_3_RST 0x00000000u

//! Field NS3PC - Per CPU New Size3 Buffer Pointer Counter
#define NS3PC_CPU_IGP_3_NS3PC_POS 0
//! Field NS3PC - Per CPU New Size3 Buffer Pointer Counter
#define NS3PC_CPU_IGP_3_NS3PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_3 Register DCNTR_IGP_3 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_3 0x4060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_3 0xE6684060u

//! Register Reset Value
#define DCNTR_IGP_3_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_3_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_3_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_3_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_3_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_PBPTR_CPU_IGP_3 Register NEW_PBPTR_CPU_IGP_3 - CPU Ingress Port New Packet Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_PBPTR_CPU_IGP_3 0x4080
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_PBPTR_CPU_IGP_3 0xE6684080u

//! Register Reset Value
#define NEW_PBPTR_CPU_IGP_3_RST 0xFFFFFF80u

//! Field OFFSET - Offset
#define NEW_PBPTR_CPU_IGP_3_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_PBPTR_CPU_IGP_3_OFFSET_MASK 0x7Fu

//! Field SA - Segment Address
#define NEW_PBPTR_CPU_IGP_3_SA_POS 7
//! Field SA - Segment Address
#define NEW_PBPTR_CPU_IGP_3_SA_MASK 0xFFFFFF80u

//! @}

//! \defgroup NEW_PBPTR1_CPU_IGP_3 Register NEW_PBPTR1_CPU_IGP_3 - CPU Ingress Port New Packet Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_PBPTR1_CPU_IGP_3 0x4084
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_PBPTR1_CPU_IGP_3 0xE6684084u

//! Register Reset Value
#define NEW_PBPTR1_CPU_IGP_3_RST 0x07800000u

//! Field SA - Segment Address Upper bits
#define NEW_PBPTR1_CPU_IGP_3_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_PBPTR1_CPU_IGP_3_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S0PTR_CPU_IGP_3 Register NEW_S0PTR_CPU_IGP_3 - CPU Ingress Port New Size0 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S0PTR_CPU_IGP_3 0x4090
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S0PTR_CPU_IGP_3 0xE6684090u

//! Register Reset Value
#define NEW_S0PTR_CPU_IGP_3_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S0PTR_CPU_IGP_3_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S0PTR_CPU_IGP_3_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S0PTR_CPU_IGP_3_SA_POS 9
//! Field SA - Segment Address
#define NEW_S0PTR_CPU_IGP_3_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S0PTR1_CPU_IGP_3 Register NEW_S0PTR1_CPU_IGP_3 - CPU Ingress Port New Size0 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S0PTR1_CPU_IGP_3 0x4094
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S0PTR1_CPU_IGP_3 0xE6684094u

//! Register Reset Value
#define NEW_S0PTR1_CPU_IGP_3_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S0PTR1_CPU_IGP_3_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S0PTR1_CPU_IGP_3_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S0PTR1_CPU_IGP_3_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S0PTR1_CPU_IGP_3_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S1PTR_CPU_IGP_3 Register NEW_S1PTR_CPU_IGP_3 - CPU Ingress Port New Size1 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S1PTR_CPU_IGP_3 0x40A0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S1PTR_CPU_IGP_3 0xE66840A0u

//! Register Reset Value
#define NEW_S1PTR_CPU_IGP_3_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S1PTR_CPU_IGP_3_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S1PTR_CPU_IGP_3_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S1PTR_CPU_IGP_3_SA_POS 9
//! Field SA - Segment Address
#define NEW_S1PTR_CPU_IGP_3_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S1PTR1_CPU_IGP_3 Register NEW_S1PTR1_CPU_IGP_3 - CPU Ingress Port New Size1 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S1PTR1_CPU_IGP_3 0x40A4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S1PTR1_CPU_IGP_3 0xE66840A4u

//! Register Reset Value
#define NEW_S1PTR1_CPU_IGP_3_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S1PTR1_CPU_IGP_3_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S1PTR1_CPU_IGP_3_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S1PTR1_CPU_IGP_3_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S1PTR1_CPU_IGP_3_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_3 Register NEW_S2PTR_CPU_IGP_3 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_3 0x40B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_3 0xE66840B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_3_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_3_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_3_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_3_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_3_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_3 Register NEW_S2PTR1_CPU_IGP_3 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_3 0x40B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_3 0xE66840B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_3_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_3_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_3_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_3_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_3_SA_MASK 0x7800000u

//! @}

//! \defgroup NEW_S3PTR_CPU_IGP_3 Register NEW_S3PTR_CPU_IGP_3 - CPU Ingress Port New Size3 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S3PTR_CPU_IGP_3 0x40C0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S3PTR_CPU_IGP_3 0xE66840C0u

//! Register Reset Value
#define NEW_S3PTR_CPU_IGP_3_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S3PTR_CPU_IGP_3_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S3PTR_CPU_IGP_3_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S3PTR_CPU_IGP_3_SA_POS 9
//! Field SA - Segment Address
#define NEW_S3PTR_CPU_IGP_3_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S3PTR1_CPU_IGP_3 Register NEW_S3PTR1_CPU_IGP_3 - CPU Ingress Port New Size3 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S3PTR1_CPU_IGP_3 0x40C4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S3PTR1_CPU_IGP_3 0xE66840C4u

//! Register Reset Value
#define NEW_S3PTR1_CPU_IGP_3_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S3PTR1_CPU_IGP_3_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S3PTR1_CPU_IGP_3_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S3PTR1_CPU_IGP_3_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S3PTR1_CPU_IGP_3_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_3 Register DESC0_0_CPU_IGP_3 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_3 0x4100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_3 0xE6684100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_3_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_3_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_3_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_3 Register DESC1_0_CPU_IGP_3 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_3 0x4104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_3 0xE6684104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_3_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_3_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_3_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_3 Register DESC2_0_CPU_IGP_3 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_3 0x4108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_3 0xE6684108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_3_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_3_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_3_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_3 Register DESC3_0_CPU_IGP_3 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_3 0x410C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_3 0xE668410Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_3_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_3_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_3_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_3 Register DESC0_1_CPU_IGP_3 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_3 0x4110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_3 0xE6684110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_3_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_3_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_3_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_3 Register DESC1_1_CPU_IGP_3 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_3 0x4114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_3 0xE6684114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_3_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_3_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_3_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_3 Register DESC2_1_CPU_IGP_3 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_3 0x4118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_3 0xE6684118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_3_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_3_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_3_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_3 Register DESC3_1_CPU_IGP_3 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_3 0x411C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_3 0xE668411Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_3_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_3_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_3_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_3_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_3_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_4 Register CFG_CPU_IGP_4 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_4 0x5000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_4 0xE6685000u

//! Register Reset Value
#define CFG_CPU_IGP_4_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_4_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_4_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_4_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_4_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_4_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_4_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_4_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_4_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_4_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_4_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_4_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_4_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_4_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_4_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_4_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_4_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_4_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_4_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_4_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_4_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_4_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_4_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_4_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_4_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_4_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_4_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_4_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_4_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_4_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_4_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_4_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_4_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_4_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_4_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_4_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_4_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_4_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_4_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_4_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_4_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_4_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_4_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_4_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_4_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_4_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_4_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_4_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_4_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_4_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_4_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_4_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_4_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_4_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_4_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_4_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_4_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_4_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_4_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_4_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_4_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_4_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_4_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_4_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_4_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_4_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_4_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_4_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_4_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_4_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_4_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_4_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_4_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_4 Register EQPC_CPU_IGP_4 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_4 0x500C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_4 0xE668500Cu

//! Register Reset Value
#define EQPC_CPU_IGP_4_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_4_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_4_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_4 Register DISC_DESC0_CPU_IGP_4 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_4 0x5010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_4 0xE6685010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_4_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_4_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_4_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_4 Register DISC_DESC1_CPU_IGP_4 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_4 0x5014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_4 0xE6685014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_4_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_4_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_4_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_4 Register DISC_DESC2_CPU_IGP_4 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_4 0x5018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_4 0xE6685018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_4_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_4_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_4_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_4 Register DISC_DESC3_CPU_IGP_4 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_4 0x501C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_4 0xE668501Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_4_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_4_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_4_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_4 Register IRNCR_CPU_IGP_4 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_4 0x5020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_4 0xE6685020u

//! Register Reset Value
#define IRNCR_CPU_IGP_4_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_4_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_4_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_4_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_4_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_4_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_4_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_4_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_4_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_4_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_4_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_4_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_4_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_4_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_4_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_4_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_4_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_4_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_4_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_4_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_4_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_4_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_4_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_4_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_4_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_4_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_4_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_4_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_4_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_4 Register IRNICR_CPU_IGP_4 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_4 0x5024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_4 0xE6685024u

//! Register Reset Value
#define IRNICR_CPU_IGP_4_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_4_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_4_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_4_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_4_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_4_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_4_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_4_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_4_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_4_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_4_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_4_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_4_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_4_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_4_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_4 Register IRNEN_CPU_IGP_4 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_4 0x5028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_4 0xE6685028u

//! Register Reset Value
#define IRNEN_CPU_IGP_4_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_4_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_4_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_4_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_4_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_4_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_4_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_4_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_4_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_4_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_4_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_4_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_4_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_4_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_4_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_4_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_4_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_4_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_4_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_4_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_4_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_4_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_4_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_4_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_4_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_4_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_4_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_4_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_4_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_4 Register DICC_IGP_4 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_4 0x5038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_4 0xE6685038u

//! Register Reset Value
#define DICC_IGP_4_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_4_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_4_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_4 Register DROP_IGP_4 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_4 0x503C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_4 0xE668503Cu

//! Register Reset Value
#define DROP_IGP_4_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_4_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_4_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_4 Register NS2PC_CPU_IGP_4 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_4 0x504C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_4 0xE668504Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_4_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_4_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_4_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_4 Register DCNTR_IGP_4 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_4 0x5060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_4 0xE6685060u

//! Register Reset Value
#define DCNTR_IGP_4_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_4_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_4_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_4_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_4_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_4 Register NEW_S2PTR_CPU_IGP_4 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_4 0x50B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_4 0xE66850B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_4_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_4_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_4_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_4_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_4_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_4 Register NEW_S2PTR1_CPU_IGP_4 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_4 0x50B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_4 0xE66850B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_4_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_4_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_4_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_4_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_4_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_4 Register DESC0_0_CPU_IGP_4 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_4 0x5100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_4 0xE6685100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_4_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_4_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_4_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_4 Register DESC1_0_CPU_IGP_4 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_4 0x5104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_4 0xE6685104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_4_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_4_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_4_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_4 Register DESC2_0_CPU_IGP_4 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_4 0x5108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_4 0xE6685108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_4_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_4_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_4_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_4 Register DESC3_0_CPU_IGP_4 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_4 0x510C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_4 0xE668510Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_4_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_4_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_4_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_4 Register DESC0_1_CPU_IGP_4 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_4 0x5110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_4 0xE6685110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_4_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_4_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_4_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_4 Register DESC1_1_CPU_IGP_4 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_4 0x5114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_4 0xE6685114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_4_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_4_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_4_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_4 Register DESC2_1_CPU_IGP_4 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_4 0x5118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_4 0xE6685118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_4_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_4_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_4_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_4 Register DESC3_1_CPU_IGP_4 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_4 0x511C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_4 0xE668511Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_4_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_4_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_4_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_4_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_4_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_5 Register CFG_CPU_IGP_5 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_5 0x6000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_5 0xE6686000u

//! Register Reset Value
#define CFG_CPU_IGP_5_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_5_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_5_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_5_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_5_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_5_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_5_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_5_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_5_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_5_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_5_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_5_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_5_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_5_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_5_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_5_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_5_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_5_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_5_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_5_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_5_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_5_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_5_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_5_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_5_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_5_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_5_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_5_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_5_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_5_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_5_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_5_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_5_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_5_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_5_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_5_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_5_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_5_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_5_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_5_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_5_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_5_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_5_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_5_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_5_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_5_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_5_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_5_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_5_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_5_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_5_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_5_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_5_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_5_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_5_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_5_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_5_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_5_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_5_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_5_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_5_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_5_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_5_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_5_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_5_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_5_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_5_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_5_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_5_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_5_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_5_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_5_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_5_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_5 Register EQPC_CPU_IGP_5 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_5 0x600C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_5 0xE668600Cu

//! Register Reset Value
#define EQPC_CPU_IGP_5_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_5_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_5_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_5 Register DISC_DESC0_CPU_IGP_5 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_5 0x6010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_5 0xE6686010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_5_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_5_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_5_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_5 Register DISC_DESC1_CPU_IGP_5 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_5 0x6014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_5 0xE6686014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_5_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_5_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_5_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_5 Register DISC_DESC2_CPU_IGP_5 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_5 0x6018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_5 0xE6686018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_5_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_5_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_5_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_5 Register DISC_DESC3_CPU_IGP_5 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_5 0x601C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_5 0xE668601Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_5_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_5_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_5_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_5 Register IRNCR_CPU_IGP_5 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_5 0x6020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_5 0xE6686020u

//! Register Reset Value
#define IRNCR_CPU_IGP_5_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_5_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_5_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_5_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_5_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_5_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_5_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_5_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_5_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_5_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_5_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_5_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_5_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_5_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_5_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_5_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_5_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_5_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_5_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_5_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_5_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_5_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_5_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_5_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_5_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_5_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_5_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_5_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_5_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_5 Register IRNICR_CPU_IGP_5 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_5 0x6024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_5 0xE6686024u

//! Register Reset Value
#define IRNICR_CPU_IGP_5_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_5_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_5_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_5_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_5_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_5_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_5_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_5_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_5_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_5_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_5_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_5_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_5_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_5_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_5_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_5 Register IRNEN_CPU_IGP_5 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_5 0x6028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_5 0xE6686028u

//! Register Reset Value
#define IRNEN_CPU_IGP_5_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_5_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_5_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_5_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_5_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_5_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_5_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_5_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_5_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_5_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_5_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_5_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_5_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_5_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_5_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_5_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_5_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_5_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_5_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_5_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_5_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_5_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_5_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_5_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_5_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_5_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_5_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_5_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_5_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_5 Register DICC_IGP_5 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_5 0x6038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_5 0xE6686038u

//! Register Reset Value
#define DICC_IGP_5_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_5_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_5_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_5 Register DROP_IGP_5 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_5 0x603C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_5 0xE668603Cu

//! Register Reset Value
#define DROP_IGP_5_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_5_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_5_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_5 Register NS2PC_CPU_IGP_5 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_5 0x604C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_5 0xE668604Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_5_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_5_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_5_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_5 Register DCNTR_IGP_5 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_5 0x6060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_5 0xE6686060u

//! Register Reset Value
#define DCNTR_IGP_5_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_5_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_5_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_5_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_5_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_5 Register NEW_S2PTR_CPU_IGP_5 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_5 0x60B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_5 0xE66860B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_5_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_5_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_5_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_5_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_5_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_5 Register NEW_S2PTR1_CPU_IGP_5 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_5 0x60B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_5 0xE66860B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_5_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_5_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_5_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_5_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_5_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_5 Register DESC0_0_CPU_IGP_5 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_5 0x6100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_5 0xE6686100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_5_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_5_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_5_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_5 Register DESC1_0_CPU_IGP_5 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_5 0x6104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_5 0xE6686104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_5_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_5_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_5_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_5 Register DESC2_0_CPU_IGP_5 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_5 0x6108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_5 0xE6686108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_5_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_5_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_5_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_5 Register DESC3_0_CPU_IGP_5 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_5 0x610C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_5 0xE668610Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_5_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_5_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_5_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_5 Register DESC0_1_CPU_IGP_5 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_5 0x6110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_5 0xE6686110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_5_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_5_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_5_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_5 Register DESC1_1_CPU_IGP_5 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_5 0x6114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_5 0xE6686114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_5_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_5_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_5_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_5 Register DESC2_1_CPU_IGP_5 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_5 0x6118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_5 0xE6686118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_5_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_5_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_5_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_5 Register DESC3_1_CPU_IGP_5 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_5 0x611C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_5 0xE668611Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_5_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_5_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_5_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_5_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_5_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_6 Register CFG_CPU_IGP_6 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_6 0x7000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_6 0xE6687000u

//! Register Reset Value
#define CFG_CPU_IGP_6_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_6_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_6_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_6_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_6_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_6_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_6_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_6_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_6_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_6_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_6_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_6_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_6_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_6_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_6_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_6_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_6_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_6_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_6_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_6_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_6_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_6_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_6_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_6_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_6_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_6_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_6_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_6_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_6_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_6_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_6_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_6_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_6_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_6_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_6_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_6_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_6_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_6_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_6_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_6_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_6_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_6_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_6_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_6_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_6_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_6_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_6_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_6_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_6_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_6_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_6_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_6_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_6_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_6_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_6_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_6_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_6_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_6_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_6_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_6_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_6_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_6_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_6_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_6_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_6_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_6_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_6_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_6_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_6_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_6_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_6_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_6_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_6_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_6 Register EQPC_CPU_IGP_6 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_6 0x700C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_6 0xE668700Cu

//! Register Reset Value
#define EQPC_CPU_IGP_6_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_6_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_6_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_6 Register DISC_DESC0_CPU_IGP_6 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_6 0x7010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_6 0xE6687010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_6_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_6_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_6_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_6 Register DISC_DESC1_CPU_IGP_6 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_6 0x7014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_6 0xE6687014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_6_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_6_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_6_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_6 Register DISC_DESC2_CPU_IGP_6 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_6 0x7018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_6 0xE6687018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_6_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_6_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_6_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_6 Register DISC_DESC3_CPU_IGP_6 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_6 0x701C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_6 0xE668701Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_6_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_6_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_6_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_6 Register IRNCR_CPU_IGP_6 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_6 0x7020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_6 0xE6687020u

//! Register Reset Value
#define IRNCR_CPU_IGP_6_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_6_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_6_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_6_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_6_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_6_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_6_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_6_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_6_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_6_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_6_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_6_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_6_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_6_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_6_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_6_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_6_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_6_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_6_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_6_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_6_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_6_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_6_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_6_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_6_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_6_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_6_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_6_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_6_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_6 Register IRNICR_CPU_IGP_6 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_6 0x7024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_6 0xE6687024u

//! Register Reset Value
#define IRNICR_CPU_IGP_6_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_6_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_6_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_6_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_6_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_6_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_6_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_6_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_6_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_6_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_6_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_6_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_6_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_6_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_6_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_6 Register IRNEN_CPU_IGP_6 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_6 0x7028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_6 0xE6687028u

//! Register Reset Value
#define IRNEN_CPU_IGP_6_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_6_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_6_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_6_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_6_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_6_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_6_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_6_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_6_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_6_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_6_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_6_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_6_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_6_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_6_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_6_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_6_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_6_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_6_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_6_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_6_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_6_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_6_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_6_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_6_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_6_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_6_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_6_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_6_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_6 Register DICC_IGP_6 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_6 0x7038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_6 0xE6687038u

//! Register Reset Value
#define DICC_IGP_6_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_6_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_6_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_6 Register DROP_IGP_6 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_6 0x703C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_6 0xE668703Cu

//! Register Reset Value
#define DROP_IGP_6_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_6_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_6_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_6 Register NS2PC_CPU_IGP_6 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_6 0x704C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_6 0xE668704Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_6_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_6_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_6_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_6 Register DCNTR_IGP_6 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_6 0x7060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_6 0xE6687060u

//! Register Reset Value
#define DCNTR_IGP_6_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_6_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_6_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_6_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_6_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_6 Register NEW_S2PTR_CPU_IGP_6 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_6 0x70B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_6 0xE66870B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_6_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_6_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_6_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_6_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_6_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_6 Register NEW_S2PTR1_CPU_IGP_6 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_6 0x70B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_6 0xE66870B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_6_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_6_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_6_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_6_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_6_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_6 Register DESC0_0_CPU_IGP_6 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_6 0x7100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_6 0xE6687100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_6_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_6_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_6_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_6 Register DESC1_0_CPU_IGP_6 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_6 0x7104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_6 0xE6687104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_6_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_6_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_6_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_6 Register DESC2_0_CPU_IGP_6 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_6 0x7108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_6 0xE6687108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_6_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_6_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_6_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_6 Register DESC3_0_CPU_IGP_6 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_6 0x710C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_6 0xE668710Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_6_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_6_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_6_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_6 Register DESC0_1_CPU_IGP_6 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_6 0x7110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_6 0xE6687110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_6_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_6_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_6_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_6 Register DESC1_1_CPU_IGP_6 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_6 0x7114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_6 0xE6687114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_6_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_6_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_6_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_6 Register DESC2_1_CPU_IGP_6 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_6 0x7118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_6 0xE6687118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_6_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_6_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_6_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_6 Register DESC3_1_CPU_IGP_6 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_6 0x711C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_6 0xE668711Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_6_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_6_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_6_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_6_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_6_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_7 Register CFG_CPU_IGP_7 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_7 0x8000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_7 0xE6688000u

//! Register Reset Value
#define CFG_CPU_IGP_7_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_7_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_7_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_7_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_7_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_7_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_7_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_7_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_7_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_7_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_7_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_7_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_7_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_7_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_7_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_7_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_7_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_7_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_7_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_7_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_7_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_7_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_7_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_7_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_7_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_7_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_7_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_7_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_7_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_7_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_7_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_7_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_7_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_7_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_7_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_7_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_7_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_7_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_7_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_7_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_7_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_7_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_7_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_7_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_7_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_7_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_7_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_7_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_7_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_7_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_7_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_7_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_7_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_7_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_7_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_7_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_7_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_7_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_7_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_7_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_7_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_7_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_7_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_7_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_7_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_7_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_7_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_7_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_7_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_7_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_7_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_7_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_7_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_7 Register EQPC_CPU_IGP_7 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_7 0x800C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_7 0xE668800Cu

//! Register Reset Value
#define EQPC_CPU_IGP_7_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_7_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_7_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_7 Register DISC_DESC0_CPU_IGP_7 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_7 0x8010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_7 0xE6688010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_7_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_7_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_7_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_7 Register DISC_DESC1_CPU_IGP_7 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_7 0x8014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_7 0xE6688014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_7_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_7_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_7_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_7 Register DISC_DESC2_CPU_IGP_7 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_7 0x8018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_7 0xE6688018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_7_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_7_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_7_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_7 Register DISC_DESC3_CPU_IGP_7 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_7 0x801C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_7 0xE668801Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_7_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_7_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_7_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_7 Register IRNCR_CPU_IGP_7 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_7 0x8020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_7 0xE6688020u

//! Register Reset Value
#define IRNCR_CPU_IGP_7_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_7_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_7_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_7_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_7_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_7_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_7_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_7_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_7_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_7_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_7_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_7_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_7_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_7_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_7_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_7_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_7_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_7_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_7_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_7_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_7_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_7_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_7_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_7_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_7_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_7_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_7_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_7_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_7_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_7 Register IRNICR_CPU_IGP_7 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_7 0x8024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_7 0xE6688024u

//! Register Reset Value
#define IRNICR_CPU_IGP_7_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_7_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_7_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_7_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_7_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_7_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_7_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_7_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_7_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_7_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_7_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_7_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_7_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_7_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_7_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_7 Register IRNEN_CPU_IGP_7 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_7 0x8028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_7 0xE6688028u

//! Register Reset Value
#define IRNEN_CPU_IGP_7_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_7_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_7_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_7_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_7_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_7_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_7_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_7_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_7_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_7_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_7_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_7_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_7_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_7_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_7_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_7_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_7_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_7_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_7_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_7_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_7_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_7_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_7_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_7_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_7_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_7_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_7_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_7_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_7_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_7 Register DICC_IGP_7 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_7 0x8038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_7 0xE6688038u

//! Register Reset Value
#define DICC_IGP_7_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_7_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_7_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_7 Register DROP_IGP_7 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_7 0x803C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_7 0xE668803Cu

//! Register Reset Value
#define DROP_IGP_7_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_7_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_7_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_7 Register NS2PC_CPU_IGP_7 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_7 0x804C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_7 0xE668804Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_7_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_7_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_7_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_7 Register DCNTR_IGP_7 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_7 0x8060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_7 0xE6688060u

//! Register Reset Value
#define DCNTR_IGP_7_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_7_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_7_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_7_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_7_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_7 Register NEW_S2PTR_CPU_IGP_7 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_7 0x80B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_7 0xE66880B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_7_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_7_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_7_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_7_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_7_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_7 Register NEW_S2PTR1_CPU_IGP_7 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_7 0x80B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_7 0xE66880B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_7_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_7_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_7_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_7_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_7_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_7 Register DESC0_0_CPU_IGP_7 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_7 0x8100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_7 0xE6688100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_7_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_7_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_7_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_7 Register DESC1_0_CPU_IGP_7 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_7 0x8104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_7 0xE6688104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_7_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_7_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_7_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_7 Register DESC2_0_CPU_IGP_7 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_7 0x8108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_7 0xE6688108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_7_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_7_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_7_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_7 Register DESC3_0_CPU_IGP_7 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_7 0x810C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_7 0xE668810Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_7_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_7_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_7_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_7 Register DESC0_1_CPU_IGP_7 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_7 0x8110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_7 0xE6688110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_7_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_7_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_7_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_7 Register DESC1_1_CPU_IGP_7 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_7 0x8114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_7 0xE6688114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_7_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_7_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_7_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_7 Register DESC2_1_CPU_IGP_7 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_7 0x8118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_7 0xE6688118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_7_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_7_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_7_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_7 Register DESC3_1_CPU_IGP_7 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_7 0x811C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_7 0xE668811Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_7_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_7_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_7_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_7_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_7_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_8 Register CFG_CPU_IGP_8 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_8 0x9000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_8 0xE6689000u

//! Register Reset Value
#define CFG_CPU_IGP_8_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_8_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_8_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_8_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_8_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_8_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_8_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_8_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_8_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_8_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_8_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_8_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_8_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_8_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_8_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_8_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_8_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_8_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_8_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_8_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_8_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_8_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_8_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_8_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_8_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_8_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_8_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_8_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_8_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_8_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_8_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_8_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_8_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_8_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_8_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_8_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_8_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_8_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_8_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_8_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_8_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_8_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_8_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_8_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_8_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_8_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_8_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_8_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_8_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_8_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_8_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_8_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_8_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_8_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_8_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_8_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_8_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_8_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_8_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_8_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_8_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_8_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_8_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_8_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_8_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_8_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_8_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_8_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_8_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_8_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_8_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_8_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_8_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_8 Register EQPC_CPU_IGP_8 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_8 0x900C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_8 0xE668900Cu

//! Register Reset Value
#define EQPC_CPU_IGP_8_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_8_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_8_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_8 Register DISC_DESC0_CPU_IGP_8 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_8 0x9010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_8 0xE6689010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_8_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_8_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_8_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_8 Register DISC_DESC1_CPU_IGP_8 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_8 0x9014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_8 0xE6689014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_8_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_8_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_8_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_8 Register DISC_DESC2_CPU_IGP_8 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_8 0x9018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_8 0xE6689018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_8_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_8_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_8_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_8 Register DISC_DESC3_CPU_IGP_8 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_8 0x901C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_8 0xE668901Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_8_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_8_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_8_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_8 Register IRNCR_CPU_IGP_8 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_8 0x9020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_8 0xE6689020u

//! Register Reset Value
#define IRNCR_CPU_IGP_8_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_8_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_8_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_8_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_8_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_8_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_8_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_8_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_8_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_8_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_8_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_8_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_8_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_8_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_8_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_8_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_8_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_8_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_8_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_8_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_8_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_8_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_8_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_8_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_8_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_8_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_8_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_8_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_8_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_8 Register IRNICR_CPU_IGP_8 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_8 0x9024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_8 0xE6689024u

//! Register Reset Value
#define IRNICR_CPU_IGP_8_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_8_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_8_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_8_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_8_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_8_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_8_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_8_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_8_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_8_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_8_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_8_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_8_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_8_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_8_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_8 Register IRNEN_CPU_IGP_8 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_8 0x9028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_8 0xE6689028u

//! Register Reset Value
#define IRNEN_CPU_IGP_8_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_8_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_8_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_8_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_8_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_8_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_8_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_8_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_8_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_8_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_8_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_8_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_8_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_8_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_8_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_8_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_8_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_8_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_8_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_8_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_8_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_8_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_8_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_8_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_8_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_8_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_8_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_8_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_8_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_8 Register DICC_IGP_8 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_8 0x9038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_8 0xE6689038u

//! Register Reset Value
#define DICC_IGP_8_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_8_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_8_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_8 Register DROP_IGP_8 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_8 0x903C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_8 0xE668903Cu

//! Register Reset Value
#define DROP_IGP_8_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_8_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_8_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_8 Register NS2PC_CPU_IGP_8 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_8 0x904C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_8 0xE668904Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_8_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_8_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_8_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_8 Register DCNTR_IGP_8 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_8 0x9060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_8 0xE6689060u

//! Register Reset Value
#define DCNTR_IGP_8_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_8_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_8_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_8_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_8_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_8 Register NEW_S2PTR_CPU_IGP_8 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_8 0x90B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_8 0xE66890B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_8_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_8_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_8_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_8_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_8_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_8 Register NEW_S2PTR1_CPU_IGP_8 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_8 0x90B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_8 0xE66890B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_8_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_8_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_8_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_8_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_8_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_8 Register DESC0_0_CPU_IGP_8 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_8 0x9100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_8 0xE6689100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_8_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_8_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_8_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_8 Register DESC1_0_CPU_IGP_8 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_8 0x9104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_8 0xE6689104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_8_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_8_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_8_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_8 Register DESC2_0_CPU_IGP_8 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_8 0x9108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_8 0xE6689108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_8_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_8_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_8_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_8 Register DESC3_0_CPU_IGP_8 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_8 0x910C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_8 0xE668910Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_8_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_8_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_8_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_8 Register DESC0_1_CPU_IGP_8 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_8 0x9110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_8 0xE6689110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_8_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_8_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_8_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_8 Register DESC1_1_CPU_IGP_8 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_8 0x9114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_8 0xE6689114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_8_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_8_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_8_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_8 Register DESC2_1_CPU_IGP_8 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_8 0x9118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_8 0xE6689118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_8_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_8_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_8_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_8 Register DESC3_1_CPU_IGP_8 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_8 0x911C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_8 0xE668911Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_8_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_8_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_8_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_8_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_8_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_9 Register CFG_CPU_IGP_9 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_9 0xA000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_9 0xE668A000u

//! Register Reset Value
#define CFG_CPU_IGP_9_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_9_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_9_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_9_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_9_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_9_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_9_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_9_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_9_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_9_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_9_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_9_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_9_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_9_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_9_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_9_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_9_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_9_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_9_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_9_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_9_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_9_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_9_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_9_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_9_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_9_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_9_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_9_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_9_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_9_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_9_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_9_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_9_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_9_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_9_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_9_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_9_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_9_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_9_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_9_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_9_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_9_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_9_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_9_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_9_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_9_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_9_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_9_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_9_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_9_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_9_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_9_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_9_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_9_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_9_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_9_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_9_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_9_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_9_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_9_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_9_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_9_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_9_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_9_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_9_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_9_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_9_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_9_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_9_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_9_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_9_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_9_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_9_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_9 Register EQPC_CPU_IGP_9 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_9 0xA00C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_9 0xE668A00Cu

//! Register Reset Value
#define EQPC_CPU_IGP_9_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_9_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_9_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_9 Register DISC_DESC0_CPU_IGP_9 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_9 0xA010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_9 0xE668A010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_9_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_9_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_9_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_9 Register DISC_DESC1_CPU_IGP_9 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_9 0xA014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_9 0xE668A014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_9_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_9_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_9_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_9 Register DISC_DESC2_CPU_IGP_9 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_9 0xA018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_9 0xE668A018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_9_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_9_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_9_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_9 Register DISC_DESC3_CPU_IGP_9 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_9 0xA01C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_9 0xE668A01Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_9_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_9_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_9_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_9 Register IRNCR_CPU_IGP_9 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_9 0xA020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_9 0xE668A020u

//! Register Reset Value
#define IRNCR_CPU_IGP_9_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_9_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_9_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_9_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_9_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_9_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_9_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_9_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_9_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_9_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_9_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_9_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_9_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_9_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_9_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_9_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_9_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_9_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_9_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_9_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_9_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_9_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_9_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_9_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_9_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_9_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_9_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_9_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_9_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_9 Register IRNICR_CPU_IGP_9 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_9 0xA024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_9 0xE668A024u

//! Register Reset Value
#define IRNICR_CPU_IGP_9_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_9_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_9_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_9_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_9_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_9_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_9_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_9_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_9_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_9_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_9_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_9_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_9_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_9_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_9_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_9 Register IRNEN_CPU_IGP_9 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_9 0xA028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_9 0xE668A028u

//! Register Reset Value
#define IRNEN_CPU_IGP_9_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_9_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_9_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_9_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_9_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_9_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_9_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_9_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_9_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_9_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_9_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_9_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_9_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_9_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_9_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_9_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_9_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_9_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_9_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_9_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_9_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_9_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_9_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_9_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_9_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_9_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_9_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_9_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_9_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_9 Register DICC_IGP_9 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_9 0xA038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_9 0xE668A038u

//! Register Reset Value
#define DICC_IGP_9_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_9_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_9_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_9 Register DROP_IGP_9 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_9 0xA03C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_9 0xE668A03Cu

//! Register Reset Value
#define DROP_IGP_9_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_9_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_9_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_9 Register NS2PC_CPU_IGP_9 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_9 0xA04C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_9 0xE668A04Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_9_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_9_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_9_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_9 Register DCNTR_IGP_9 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_9 0xA060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_9 0xE668A060u

//! Register Reset Value
#define DCNTR_IGP_9_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_9_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_9_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_9_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_9_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_9 Register NEW_S2PTR_CPU_IGP_9 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_9 0xA0B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_9 0xE668A0B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_9_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_9_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_9_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_9_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_9_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_9 Register NEW_S2PTR1_CPU_IGP_9 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_9 0xA0B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_9 0xE668A0B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_9_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_9_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_9_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_9_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_9_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_9 Register DESC0_0_CPU_IGP_9 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_9 0xA100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_9 0xE668A100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_9_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_9_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_9_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_9 Register DESC1_0_CPU_IGP_9 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_9 0xA104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_9 0xE668A104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_9_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_9_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_9_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_9 Register DESC2_0_CPU_IGP_9 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_9 0xA108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_9 0xE668A108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_9_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_9_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_9_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_9 Register DESC3_0_CPU_IGP_9 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_9 0xA10C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_9 0xE668A10Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_9_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_9_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_9_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_9 Register DESC0_1_CPU_IGP_9 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_9 0xA110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_9 0xE668A110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_9_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_9_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_9_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_9 Register DESC1_1_CPU_IGP_9 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_9 0xA114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_9 0xE668A114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_9_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_9_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_9_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_9 Register DESC2_1_CPU_IGP_9 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_9 0xA118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_9 0xE668A118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_9_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_9_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_9_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_9 Register DESC3_1_CPU_IGP_9 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_9 0xA11C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_9 0xE668A11Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_9_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_9_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_9_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_9_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_9_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_10 Register CFG_CPU_IGP_10 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_10 0xB000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_10 0xE668B000u

//! Register Reset Value
#define CFG_CPU_IGP_10_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_10_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_10_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_10_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_10_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_10_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_10_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_10_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_10_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_10_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_10_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_10_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_10_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_10_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_10_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_10_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_10_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_10_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_10_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_10_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_10_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_10_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_10_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_10_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_10_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_10_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_10_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_10_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_10_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_10_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_10_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_10_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_10_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_10_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_10_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_10_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_10_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_10_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_10_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_10_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_10_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_10_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_10_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_10_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_10_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_10_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_10_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_10_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_10_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_10_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_10_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_10_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_10_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_10_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_10_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_10_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_10_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_10_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_10_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_10_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_10_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_10_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_10_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_10_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_10_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_10_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_10_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_10_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_10_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_10_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_10_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_10_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_10_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_10 Register EQPC_CPU_IGP_10 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_10 0xB00C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_10 0xE668B00Cu

//! Register Reset Value
#define EQPC_CPU_IGP_10_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_10_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_10_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_10 Register DISC_DESC0_CPU_IGP_10 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_10 0xB010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_10 0xE668B010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_10_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_10_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_10_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_10 Register DISC_DESC1_CPU_IGP_10 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_10 0xB014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_10 0xE668B014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_10_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_10_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_10_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_10 Register DISC_DESC2_CPU_IGP_10 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_10 0xB018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_10 0xE668B018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_10_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_10_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_10_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_10 Register DISC_DESC3_CPU_IGP_10 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_10 0xB01C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_10 0xE668B01Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_10_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_10_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_10_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_10 Register IRNCR_CPU_IGP_10 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_10 0xB020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_10 0xE668B020u

//! Register Reset Value
#define IRNCR_CPU_IGP_10_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_10_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_10_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_10_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_10_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_10_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_10_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_10_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_10_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_10_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_10_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_10_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_10_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_10_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_10_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_10_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_10_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_10_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_10_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_10_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_10_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_10_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_10_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_10_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_10_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_10_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_10_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_10_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_10_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_10 Register IRNICR_CPU_IGP_10 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_10 0xB024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_10 0xE668B024u

//! Register Reset Value
#define IRNICR_CPU_IGP_10_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_10_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_10_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_10_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_10_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_10_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_10_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_10_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_10_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_10_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_10_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_10_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_10_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_10_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_10_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_10 Register IRNEN_CPU_IGP_10 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_10 0xB028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_10 0xE668B028u

//! Register Reset Value
#define IRNEN_CPU_IGP_10_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_10_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_10_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_10_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_10_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_10_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_10_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_10_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_10_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_10_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_10_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_10_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_10_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_10_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_10_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_10_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_10_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_10_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_10_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_10_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_10_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_10_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_10_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_10_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_10_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_10_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_10_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_10_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_10_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_10 Register DICC_IGP_10 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_10 0xB038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_10 0xE668B038u

//! Register Reset Value
#define DICC_IGP_10_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_10_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_10_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_10 Register DROP_IGP_10 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_10 0xB03C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_10 0xE668B03Cu

//! Register Reset Value
#define DROP_IGP_10_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_10_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_10_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_10 Register NS2PC_CPU_IGP_10 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_10 0xB04C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_10 0xE668B04Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_10_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_10_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_10_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_10 Register DCNTR_IGP_10 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_10 0xB060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_10 0xE668B060u

//! Register Reset Value
#define DCNTR_IGP_10_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_10_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_10_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_10_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_10_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_10 Register NEW_S2PTR_CPU_IGP_10 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_10 0xB0B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_10 0xE668B0B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_10_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_10_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_10_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_10_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_10_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_10 Register NEW_S2PTR1_CPU_IGP_10 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_10 0xB0B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_10 0xE668B0B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_10_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_10_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_10_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_10_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_10_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_10 Register DESC0_0_CPU_IGP_10 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_10 0xB100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_10 0xE668B100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_10_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_10_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_10_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_10 Register DESC1_0_CPU_IGP_10 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_10 0xB104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_10 0xE668B104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_10_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_10_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_10_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_10 Register DESC2_0_CPU_IGP_10 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_10 0xB108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_10 0xE668B108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_10_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_10_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_10_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_10 Register DESC3_0_CPU_IGP_10 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_10 0xB10C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_10 0xE668B10Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_10_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_10_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_10_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_10 Register DESC0_1_CPU_IGP_10 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_10 0xB110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_10 0xE668B110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_10_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_10_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_10_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_10 Register DESC1_1_CPU_IGP_10 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_10 0xB114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_10 0xE668B114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_10_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_10_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_10_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_10 Register DESC2_1_CPU_IGP_10 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_10 0xB118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_10 0xE668B118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_10_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_10_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_10_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_10 Register DESC3_1_CPU_IGP_10 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_10 0xB11C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_10 0xE668B11Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_10_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_10_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_10_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_10_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_10_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_11 Register CFG_CPU_IGP_11 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_11 0xC000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_11 0xE668C000u

//! Register Reset Value
#define CFG_CPU_IGP_11_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_11_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_11_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_11_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_11_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_11_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_11_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_11_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_11_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_11_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_11_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_11_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_11_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_11_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_11_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_11_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_11_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_11_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_11_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_11_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_11_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_11_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_11_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_11_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_11_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_11_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_11_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_11_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_11_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_11_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_11_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_11_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_11_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_11_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_11_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_11_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_11_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_11_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_11_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_11_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_11_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_11_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_11_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_11_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_11_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_11_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_11_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_11_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_11_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_11_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_11_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_11_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_11_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_11_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_11_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_11_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_11_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_11_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_11_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_11_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_11_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_11_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_11_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_11_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_11_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_11_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_11_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_11_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_11_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_11_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_11_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_11_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_11_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_11 Register EQPC_CPU_IGP_11 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_11 0xC00C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_11 0xE668C00Cu

//! Register Reset Value
#define EQPC_CPU_IGP_11_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_11_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_11_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_11 Register DISC_DESC0_CPU_IGP_11 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_11 0xC010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_11 0xE668C010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_11_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_11_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_11_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_11 Register DISC_DESC1_CPU_IGP_11 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_11 0xC014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_11 0xE668C014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_11_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_11_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_11_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_11 Register DISC_DESC2_CPU_IGP_11 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_11 0xC018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_11 0xE668C018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_11_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_11_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_11_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_11 Register DISC_DESC3_CPU_IGP_11 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_11 0xC01C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_11 0xE668C01Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_11_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_11_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_11_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_11 Register IRNCR_CPU_IGP_11 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_11 0xC020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_11 0xE668C020u

//! Register Reset Value
#define IRNCR_CPU_IGP_11_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_11_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_11_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_11_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_11_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_11_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_11_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_11_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_11_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_11_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_11_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_11_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_11_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_11_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_11_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_11_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_11_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_11_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_11_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_11_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_11_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_11_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_11_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_11_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_11_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_11_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_11_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_11_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_11_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_11 Register IRNICR_CPU_IGP_11 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_11 0xC024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_11 0xE668C024u

//! Register Reset Value
#define IRNICR_CPU_IGP_11_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_11_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_11_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_11_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_11_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_11_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_11_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_11_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_11_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_11_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_11_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_11_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_11_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_11_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_11_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_11 Register IRNEN_CPU_IGP_11 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_11 0xC028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_11 0xE668C028u

//! Register Reset Value
#define IRNEN_CPU_IGP_11_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_11_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_11_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_11_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_11_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_11_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_11_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_11_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_11_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_11_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_11_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_11_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_11_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_11_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_11_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_11_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_11_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_11_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_11_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_11_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_11_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_11_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_11_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_11_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_11_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_11_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_11_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_11_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_11_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_11 Register DICC_IGP_11 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_11 0xC038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_11 0xE668C038u

//! Register Reset Value
#define DICC_IGP_11_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_11_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_11_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_11 Register DROP_IGP_11 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_11 0xC03C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_11 0xE668C03Cu

//! Register Reset Value
#define DROP_IGP_11_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_11_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_11_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_11 Register NS2PC_CPU_IGP_11 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_11 0xC04C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_11 0xE668C04Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_11_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_11_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_11_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_11 Register DCNTR_IGP_11 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_11 0xC060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_11 0xE668C060u

//! Register Reset Value
#define DCNTR_IGP_11_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_11_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_11_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_11_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_11_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_11 Register NEW_S2PTR_CPU_IGP_11 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_11 0xC0B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_11 0xE668C0B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_11_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_11_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_11_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_11_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_11_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_11 Register NEW_S2PTR1_CPU_IGP_11 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_11 0xC0B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_11 0xE668C0B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_11_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_11_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_11_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_11_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_11_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_11 Register DESC0_0_CPU_IGP_11 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_11 0xC100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_11 0xE668C100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_11_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_11_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_11_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_11 Register DESC1_0_CPU_IGP_11 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_11 0xC104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_11 0xE668C104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_11_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_11_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_11_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_11 Register DESC2_0_CPU_IGP_11 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_11 0xC108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_11 0xE668C108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_11_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_11_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_11_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_11 Register DESC3_0_CPU_IGP_11 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_11 0xC10C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_11 0xE668C10Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_11_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_11_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_11_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_11 Register DESC0_1_CPU_IGP_11 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_11 0xC110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_11 0xE668C110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_11_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_11_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_11_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_11 Register DESC1_1_CPU_IGP_11 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_11 0xC114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_11 0xE668C114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_11_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_11_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_11_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_11 Register DESC2_1_CPU_IGP_11 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_11 0xC118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_11 0xE668C118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_11_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_11_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_11_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_11 Register DESC3_1_CPU_IGP_11 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_11 0xC11C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_11 0xE668C11Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_11_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_11_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_11_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_11_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_11_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_12 Register CFG_CPU_IGP_12 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_12 0xD000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_12 0xE668D000u

//! Register Reset Value
#define CFG_CPU_IGP_12_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_12_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_12_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_12_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_12_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_12_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_12_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_12_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_12_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_12_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_12_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_12_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_12_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_12_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_12_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_12_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_12_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_12_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_12_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_12_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_12_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_12_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_12_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_12_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_12_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_12_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_12_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_12_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_12_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_12_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_12_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_12_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_12_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_12_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_12_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_12_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_12_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_12_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_12_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_12_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_12_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_12_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_12_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_12_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_12_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_12_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_12_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_12_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_12_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_12_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_12_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_12_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_12_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_12_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_12_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_12_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_12_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_12_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_12_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_12_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_12_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_12_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_12_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_12_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_12_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_12_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_12_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_12_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_12_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_12_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_12_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_12_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_12_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_12 Register EQPC_CPU_IGP_12 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_12 0xD00C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_12 0xE668D00Cu

//! Register Reset Value
#define EQPC_CPU_IGP_12_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_12_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_12_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_12 Register DISC_DESC0_CPU_IGP_12 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_12 0xD010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_12 0xE668D010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_12_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_12_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_12_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_12 Register DISC_DESC1_CPU_IGP_12 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_12 0xD014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_12 0xE668D014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_12_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_12_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_12_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_12 Register DISC_DESC2_CPU_IGP_12 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_12 0xD018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_12 0xE668D018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_12_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_12_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_12_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_12 Register DISC_DESC3_CPU_IGP_12 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_12 0xD01C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_12 0xE668D01Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_12_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_12_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_12_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_12 Register IRNCR_CPU_IGP_12 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_12 0xD020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_12 0xE668D020u

//! Register Reset Value
#define IRNCR_CPU_IGP_12_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_12_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_12_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_12_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_12_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_12_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_12_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_12_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_12_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_12_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_12_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_12_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_12_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_12_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_12_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_12_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_12_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_12_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_12_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_12_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_12_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_12_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_12_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_12_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_12_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_12_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_12_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_12_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_12_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_12 Register IRNICR_CPU_IGP_12 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_12 0xD024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_12 0xE668D024u

//! Register Reset Value
#define IRNICR_CPU_IGP_12_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_12_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_12_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_12_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_12_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_12_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_12_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_12_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_12_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_12_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_12_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_12_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_12_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_12_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_12_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_12 Register IRNEN_CPU_IGP_12 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_12 0xD028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_12 0xE668D028u

//! Register Reset Value
#define IRNEN_CPU_IGP_12_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_12_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_12_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_12_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_12_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_12_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_12_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_12_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_12_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_12_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_12_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_12_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_12_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_12_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_12_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_12_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_12_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_12_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_12_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_12_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_12_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_12_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_12_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_12_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_12_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_12_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_12_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_12_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_12_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_12 Register DICC_IGP_12 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_12 0xD038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_12 0xE668D038u

//! Register Reset Value
#define DICC_IGP_12_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_12_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_12_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_12 Register DROP_IGP_12 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_12 0xD03C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_12 0xE668D03Cu

//! Register Reset Value
#define DROP_IGP_12_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_12_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_12_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_12 Register NS2PC_CPU_IGP_12 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_12 0xD04C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_12 0xE668D04Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_12_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_12_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_12_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_12 Register DCNTR_IGP_12 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_12 0xD060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_12 0xE668D060u

//! Register Reset Value
#define DCNTR_IGP_12_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_12_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_12_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_12_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_12_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_12 Register NEW_S2PTR_CPU_IGP_12 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_12 0xD0B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_12 0xE668D0B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_12_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_12_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_12_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_12_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_12_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_12 Register NEW_S2PTR1_CPU_IGP_12 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_12 0xD0B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_12 0xE668D0B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_12_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_12_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_12_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_12_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_12_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_12 Register DESC0_0_CPU_IGP_12 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_12 0xD100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_12 0xE668D100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_12_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_12_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_12_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_12 Register DESC1_0_CPU_IGP_12 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_12 0xD104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_12 0xE668D104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_12_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_12_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_12_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_12 Register DESC2_0_CPU_IGP_12 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_12 0xD108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_12 0xE668D108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_12_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_12_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_12_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_12 Register DESC3_0_CPU_IGP_12 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_12 0xD10C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_12 0xE668D10Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_12_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_12_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_12_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_12 Register DESC0_1_CPU_IGP_12 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_12 0xD110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_12 0xE668D110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_12_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_12_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_12_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_12 Register DESC1_1_CPU_IGP_12 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_12 0xD114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_12 0xE668D114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_12_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_12_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_12_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_12 Register DESC2_1_CPU_IGP_12 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_12 0xD118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_12 0xE668D118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_12_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_12_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_12_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_12 Register DESC3_1_CPU_IGP_12 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_12 0xD11C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_12 0xE668D11Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_12_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_12_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_12_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_12_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_12_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_13 Register CFG_CPU_IGP_13 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_13 0xE000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_13 0xE668E000u

//! Register Reset Value
#define CFG_CPU_IGP_13_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_13_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_13_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_13_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_13_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_13_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_13_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_13_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_13_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_13_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_13_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_13_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_13_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_13_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_13_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_13_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_13_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_13_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_13_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_13_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_13_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_13_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_13_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_13_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_13_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_13_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_13_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_13_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_13_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_13_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_13_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_13_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_13_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_13_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_13_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_13_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_13_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_13_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_13_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_13_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_13_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_13_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_13_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_13_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_13_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_13_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_13_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_13_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_13_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_13_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_13_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_13_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_13_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_13_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_13_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_13_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_13_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_13_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_13_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_13_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_13_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_13_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_13_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_13_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_13_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_13_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_13_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_13_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_13_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_13_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_13_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_13_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_13_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_13 Register EQPC_CPU_IGP_13 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_13 0xE00C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_13 0xE668E00Cu

//! Register Reset Value
#define EQPC_CPU_IGP_13_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_13_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_13_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_13 Register DISC_DESC0_CPU_IGP_13 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_13 0xE010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_13 0xE668E010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_13_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_13_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_13_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_13 Register DISC_DESC1_CPU_IGP_13 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_13 0xE014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_13 0xE668E014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_13_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_13_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_13_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_13 Register DISC_DESC2_CPU_IGP_13 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_13 0xE018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_13 0xE668E018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_13_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_13_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_13_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_13 Register DISC_DESC3_CPU_IGP_13 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_13 0xE01C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_13 0xE668E01Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_13_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_13_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_13_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_13 Register IRNCR_CPU_IGP_13 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_13 0xE020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_13 0xE668E020u

//! Register Reset Value
#define IRNCR_CPU_IGP_13_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_13_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_13_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_13_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_13_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_13_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_13_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_13_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_13_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_13_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_13_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_13_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_13_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_13_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_13_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_13_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_13_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_13_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_13_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_13_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_13_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_13_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_13_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_13_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_13_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_13_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_13_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_13_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_13_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_13 Register IRNICR_CPU_IGP_13 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_13 0xE024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_13 0xE668E024u

//! Register Reset Value
#define IRNICR_CPU_IGP_13_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_13_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_13_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_13_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_13_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_13_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_13_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_13_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_13_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_13_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_13_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_13_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_13_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_13_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_13_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_13 Register IRNEN_CPU_IGP_13 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_13 0xE028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_13 0xE668E028u

//! Register Reset Value
#define IRNEN_CPU_IGP_13_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_13_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_13_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_13_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_13_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_13_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_13_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_13_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_13_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_13_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_13_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_13_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_13_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_13_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_13_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_13_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_13_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_13_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_13_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_13_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_13_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_13_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_13_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_13_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_13_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_13_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_13_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_13_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_13_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_13 Register DICC_IGP_13 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_13 0xE038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_13 0xE668E038u

//! Register Reset Value
#define DICC_IGP_13_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_13_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_13_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_13 Register DROP_IGP_13 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_13 0xE03C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_13 0xE668E03Cu

//! Register Reset Value
#define DROP_IGP_13_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_13_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_13_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_13 Register NS2PC_CPU_IGP_13 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_13 0xE04C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_13 0xE668E04Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_13_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_13_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_13_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_13 Register DCNTR_IGP_13 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_13 0xE060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_13 0xE668E060u

//! Register Reset Value
#define DCNTR_IGP_13_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_13_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_13_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_13_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_13_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_13 Register NEW_S2PTR_CPU_IGP_13 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_13 0xE0B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_13 0xE668E0B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_13_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_13_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_13_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_13_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_13_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_13 Register NEW_S2PTR1_CPU_IGP_13 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_13 0xE0B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_13 0xE668E0B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_13_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_13_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_13_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_13_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_13_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_13 Register DESC0_0_CPU_IGP_13 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_13 0xE100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_13 0xE668E100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_13_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_13_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_13_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_13 Register DESC1_0_CPU_IGP_13 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_13 0xE104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_13 0xE668E104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_13_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_13_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_13_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_13 Register DESC2_0_CPU_IGP_13 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_13 0xE108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_13 0xE668E108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_13_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_13_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_13_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_13 Register DESC3_0_CPU_IGP_13 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_13 0xE10C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_13 0xE668E10Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_13_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_13_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_13_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_13 Register DESC0_1_CPU_IGP_13 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_13 0xE110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_13 0xE668E110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_13_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_13_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_13_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_13 Register DESC1_1_CPU_IGP_13 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_13 0xE114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_13 0xE668E114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_13_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_13_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_13_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_13 Register DESC2_1_CPU_IGP_13 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_13 0xE118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_13 0xE668E118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_13_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_13_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_13_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_13 Register DESC3_1_CPU_IGP_13 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_13 0xE11C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_13 0xE668E11Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_13_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_13_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_13_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_13_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_13_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_14 Register CFG_CPU_IGP_14 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_14 0xF000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_14 0xE668F000u

//! Register Reset Value
#define CFG_CPU_IGP_14_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_14_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_14_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_14_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_14_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_14_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_14_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_14_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_14_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_14_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_14_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_14_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_14_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_14_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_14_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_14_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_14_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_14_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_14_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_14_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_14_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_14_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_14_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_14_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_14_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_14_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_14_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_14_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_14_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_14_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_14_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_14_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_14_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_14_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_14_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_14_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_14_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_14_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_14_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_14_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_14_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_14_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_14_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_14_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_14_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_14_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_14_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_14_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_14_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_14_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_14_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_14_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_14_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_14_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_14_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_14_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_14_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_14_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_14_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_14_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_14_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_14_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_14_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_14_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_14_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_14_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_14_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_14_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_14_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_14_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_14_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_14_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_14_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_14 Register EQPC_CPU_IGP_14 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_14 0xF00C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_14 0xE668F00Cu

//! Register Reset Value
#define EQPC_CPU_IGP_14_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_14_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_14_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_14 Register DISC_DESC0_CPU_IGP_14 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_14 0xF010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_14 0xE668F010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_14_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_14_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_14_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_14 Register DISC_DESC1_CPU_IGP_14 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_14 0xF014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_14 0xE668F014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_14_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_14_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_14_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_14 Register DISC_DESC2_CPU_IGP_14 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_14 0xF018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_14 0xE668F018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_14_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_14_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_14_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_14 Register DISC_DESC3_CPU_IGP_14 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_14 0xF01C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_14 0xE668F01Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_14_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_14_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_14_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_14 Register IRNCR_CPU_IGP_14 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_14 0xF020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_14 0xE668F020u

//! Register Reset Value
#define IRNCR_CPU_IGP_14_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_14_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_14_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_14_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_14_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_14_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_14_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_14_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_14_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_14_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_14_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_14_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_14_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_14_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_14_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_14_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_14_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_14_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_14_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_14_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_14_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_14_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_14_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_14_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_14_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_14_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_14_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_14_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_14_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_14 Register IRNICR_CPU_IGP_14 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_14 0xF024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_14 0xE668F024u

//! Register Reset Value
#define IRNICR_CPU_IGP_14_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_14_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_14_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_14_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_14_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_14_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_14_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_14_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_14_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_14_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_14_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_14_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_14_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_14_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_14_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_14 Register IRNEN_CPU_IGP_14 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_14 0xF028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_14 0xE668F028u

//! Register Reset Value
#define IRNEN_CPU_IGP_14_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_14_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_14_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_14_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_14_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_14_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_14_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_14_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_14_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_14_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_14_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_14_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_14_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_14_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_14_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_14_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_14_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_14_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_14_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_14_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_14_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_14_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_14_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_14_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_14_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_14_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_14_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_14_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_14_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_14 Register DICC_IGP_14 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_14 0xF038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_14 0xE668F038u

//! Register Reset Value
#define DICC_IGP_14_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_14_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_14_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_14 Register DROP_IGP_14 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_14 0xF03C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_14 0xE668F03Cu

//! Register Reset Value
#define DROP_IGP_14_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_14_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_14_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_14 Register NS2PC_CPU_IGP_14 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_14 0xF04C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_14 0xE668F04Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_14_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_14_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_14_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_14 Register DCNTR_IGP_14 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_14 0xF060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_14 0xE668F060u

//! Register Reset Value
#define DCNTR_IGP_14_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_14_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_14_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_14_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_14_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_14 Register NEW_S2PTR_CPU_IGP_14 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_14 0xF0B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_14 0xE668F0B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_14_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_14_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_14_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_14_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_14_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_14 Register NEW_S2PTR1_CPU_IGP_14 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_14 0xF0B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_14 0xE668F0B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_14_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_14_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_14_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_14_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_14_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_14 Register DESC0_0_CPU_IGP_14 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_14 0xF100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_14 0xE668F100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_14_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_14_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_14_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_14 Register DESC1_0_CPU_IGP_14 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_14 0xF104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_14 0xE668F104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_14_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_14_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_14_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_14 Register DESC2_0_CPU_IGP_14 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_14 0xF108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_14 0xE668F108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_14_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_14_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_14_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_14 Register DESC3_0_CPU_IGP_14 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_14 0xF10C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_14 0xE668F10Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_14_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_14_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_14_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_14 Register DESC0_1_CPU_IGP_14 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_14 0xF110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_14 0xE668F110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_14_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_14_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_14_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_14 Register DESC1_1_CPU_IGP_14 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_14 0xF114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_14 0xE668F114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_14_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_14_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_14_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_14 Register DESC2_1_CPU_IGP_14 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_14 0xF118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_14 0xE668F118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_14_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_14_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_14_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_14 Register DESC3_1_CPU_IGP_14 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_14 0xF11C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_14 0xE668F11Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_14_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_14_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_14_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_14_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_14_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_CPU_IGP_15 Register CFG_CPU_IGP_15 - CPU Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_CPU_IGP_15 0x10000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_CPU_IGP_15 0xE6690000u

//! Register Reset Value
#define CFG_CPU_IGP_15_RST 0x00000000u

//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_15_EQREQ_POS 0
//! Field EQREQ - Enable CPU Enqueue Request
#define CFG_CPU_IGP_15_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_15_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_15_EQREQ_EN 0x1

//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_15_BUFREQ0_POS 1
//! Field BUFREQ0 - Enable CPU Buffer Request For Internal Packet Buffer Pointers
#define CFG_CPU_IGP_15_BUFREQ0_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_15_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_15_BUFREQ0_EN 0x1

//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_15_BUFREQ1_POS 2
//! Field BUFREQ1 - Enable CPU Buffer Request For Size0 Packet
#define CFG_CPU_IGP_15_BUFREQ1_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_15_BUFREQ1_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_15_BUFREQ1_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_15_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_CPU_IGP_15_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_15_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_15_DISCHALT_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_15_BRBPEN_POS 5
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_CPU_IGP_15_BRBPEN_MASK 0x20u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_15_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_15_BRBPEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_15_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_CPU_IGP_15_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_15_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_15_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_15_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_CPU_IGP_15_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_15_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_15_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_CPU_IGP_15_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_CPU_IGP_15_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_CPU_IGP_15_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_CPU_IGP_15_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_CPU_IGP_15_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_CPU_IGP_15_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_CPU_IGP_15_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_CPU_IGP_15_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_CPU_IGP_15_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_CPU_IGP_15_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_CPU_IGP_15_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_CPU_IGP_15_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_CPU_IGP_15_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_CPU_IGP_15_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_CPU_IGP_15_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_CPU_IGP_15_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_CPU_IGP_15_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_CPU_IGP_15_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_CPU_IGP_15_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_CPU_IGP_15_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_CPU_IGP_15_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_CPU_IGP_15_DISC_REASON15 0xF

//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_15_BUFREQ2_POS 20
//! Field BUFREQ2 - Enable CPU Buffer Request For Size1 Packet
#define CFG_CPU_IGP_15_BUFREQ2_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_15_BUFREQ2_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_15_BUFREQ2_EN 0x1

//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_15_BUFREQ3_POS 21
//! Field BUFREQ3 - Enable CPU Buffer Request For Size2 Packet
#define CFG_CPU_IGP_15_BUFREQ3_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_15_BUFREQ3_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_15_BUFREQ3_EN 0x1

//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_15_BUFREQ4_POS 22
//! Field BUFREQ4 - Enable CPU Buffer Request For Size3 Packet
#define CFG_CPU_IGP_15_BUFREQ4_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_15_BUFREQ4_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_15_BUFREQ4_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_15_QOSBYPSEN_POS 24
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_CPU_IGP_15_QOSBYPSEN_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_15_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_15_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_15_FSQMQEN_POS 25
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_CPU_IGP_15_FSQMQEN_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_CFG_CPU_IGP_15_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_CPU_IGP_15_FSQMQEN_EN 0x1

//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_15_DQMDMAQ_POS 28
//! Field DQMDMAQ - DQM DMA Queue to be used in QoS Bypass and Non-FSQMQEN Mode
#define CFG_CPU_IGP_15_DQMDMAQ_MASK 0x70000000u

//! @}

//! \defgroup EQPC_CPU_IGP_15 Register EQPC_CPU_IGP_15 - CPU Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_CPU_IGP_15 0x1000C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_CPU_IGP_15 0xE669000Cu

//! Register Reset Value
#define EQPC_CPU_IGP_15_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_15_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_CPU_IGP_15_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_CPU_IGP_15 Register DISC_DESC0_CPU_IGP_15 - CPU Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_CPU_IGP_15 0x10010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_CPU_IGP_15 0xE6690010u

//! Register Reset Value
#define DISC_DESC0_CPU_IGP_15_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_15_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_CPU_IGP_15_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_CPU_IGP_15 Register DISC_DESC1_CPU_IGP_15 - CPU Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_CPU_IGP_15 0x10014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_CPU_IGP_15 0xE6690014u

//! Register Reset Value
#define DISC_DESC1_CPU_IGP_15_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_15_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_CPU_IGP_15_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_CPU_IGP_15 Register DISC_DESC2_CPU_IGP_15 - CPU Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_CPU_IGP_15 0x10018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_CPU_IGP_15 0xE6690018u

//! Register Reset Value
#define DISC_DESC2_CPU_IGP_15_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_15_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_CPU_IGP_15_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_CPU_IGP_15 Register DISC_DESC3_CPU_IGP_15 - CPU Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_CPU_IGP_15 0x1001C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_CPU_IGP_15 0xE669001Cu

//! Register Reset Value
#define DISC_DESC3_CPU_IGP_15_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_15_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_CPU_IGP_15_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_CPU_IGP_15 Register IRNCR_CPU_IGP_15 - CPU Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_CPU_IGP_15 0x10020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_CPU_IGP_15 0xE6690020u

//! Register Reset Value
#define IRNCR_CPU_IGP_15_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_15_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_CPU_IGP_15_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_15_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_CPU_IGP_15_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_15_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_CPU_IGP_15_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_15_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_15_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_15_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_CPU_IGP_15_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_15_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_15_PBR_INTOCC 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_15_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNCR_CPU_IGP_15_S0PR_MASK 0x20u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_15_S0PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_15_S0PR_INTOCC 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_15_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNCR_CPU_IGP_15_S1PR_MASK 0x40u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_15_S1PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_15_S1PR_INTOCC 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_15_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNCR_CPU_IGP_15_S2PR_MASK 0x80u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_15_S2PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_15_S2PR_INTOCC 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_15_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNCR_CPU_IGP_15_S3PR_MASK 0x100u
//! Constant NUL - NULL
#define CONST_IRNCR_CPU_IGP_15_S3PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_CPU_IGP_15_S3PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_CPU_IGP_15 Register IRNICR_CPU_IGP_15 - CPU Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_CPU_IGP_15 0x10024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_CPU_IGP_15 0xE6690024u

//! Register Reset Value
#define IRNICR_CPU_IGP_15_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_15_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_CPU_IGP_15_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_15_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_CPU_IGP_15_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_15_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_CPU_IGP_15_PBR_MASK 0x10u

//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_15_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNICR_CPU_IGP_15_S0PR_MASK 0x20u

//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_15_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNICR_CPU_IGP_15_S1PR_MASK 0x40u

//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_15_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNICR_CPU_IGP_15_S2PR_MASK 0x80u

//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_15_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNICR_CPU_IGP_15_S3PR_MASK 0x100u

//! @}

//! \defgroup IRNEN_CPU_IGP_15 Register IRNEN_CPU_IGP_15 - CPU Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_CPU_IGP_15 0x10028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_CPU_IGP_15 0xE6690028u

//! Register Reset Value
#define IRNEN_CPU_IGP_15_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_15_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_CPU_IGP_15_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_15_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_15_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_15_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_CPU_IGP_15_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_15_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_15_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_15_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_CPU_IGP_15_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_15_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_15_PBR_EN 0x1

//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_15_S0PR_POS 5
//! Field S0PR - Size0 Pointer Ready
#define IRNEN_CPU_IGP_15_S0PR_MASK 0x20u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_15_S0PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_15_S0PR_EN 0x1

//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_15_S1PR_POS 6
//! Field S1PR - Size1 Pointer Ready
#define IRNEN_CPU_IGP_15_S1PR_MASK 0x40u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_15_S1PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_15_S1PR_EN 0x1

//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_15_S2PR_POS 7
//! Field S2PR - Size2 Pointer Ready
#define IRNEN_CPU_IGP_15_S2PR_MASK 0x80u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_15_S2PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_15_S2PR_EN 0x1

//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_15_S3PR_POS 8
//! Field S3PR - Size3 Pointer Ready
#define IRNEN_CPU_IGP_15_S3PR_MASK 0x100u
//! Constant DIS - DIS
#define CONST_IRNEN_CPU_IGP_15_S3PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_CPU_IGP_15_S3PR_EN 0x1

//! @}

//! \defgroup DICC_IGP_15 Register DICC_IGP_15 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_15 0x10038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_15 0xE6690038u

//! Register Reset Value
#define DICC_IGP_15_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_15_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_15_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_15 Register DROP_IGP_15 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_15 0x1003C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_15 0xE669003Cu

//! Register Reset Value
#define DROP_IGP_15_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_15_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_15_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NS2PC_CPU_IGP_15 Register NS2PC_CPU_IGP_15 - CPU Ingress Port new size2 buffer pointer counter
//! @{

//! Register Offset (relative)
#define NS2PC_CPU_IGP_15 0x1004C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NS2PC_CPU_IGP_15 0xE669004Cu

//! Register Reset Value
#define NS2PC_CPU_IGP_15_RST 0x00000000u

//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_15_NS2PC_POS 0
//! Field NS2PC - Per CPU New Size1 Buffer Pointer Counter
#define NS2PC_CPU_IGP_15_NS2PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_15 Register DCNTR_IGP_15 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_15 0x10060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_15 0xE6690060u

//! Register Reset Value
#define DCNTR_IGP_15_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_15_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_15_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_15_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_15_DCNT_MASK 0x3F00u

//! @}

//! \defgroup NEW_S2PTR_CPU_IGP_15 Register NEW_S2PTR_CPU_IGP_15 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR_CPU_IGP_15 0x100B0
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR_CPU_IGP_15 0xE66900B0u

//! Register Reset Value
#define NEW_S2PTR_CPU_IGP_15_RST 0xFFFFFE00u

//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_15_OFFSET_POS 0
//! Field OFFSET - Offset
#define NEW_S2PTR_CPU_IGP_15_OFFSET_MASK 0x1FFu

//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_15_SA_POS 9
//! Field SA - Segment Address
#define NEW_S2PTR_CPU_IGP_15_SA_MASK 0xFFFFFE00u

//! @}

//! \defgroup NEW_S2PTR1_CPU_IGP_15 Register NEW_S2PTR1_CPU_IGP_15 - CPU Ingress Port New Size2 Buffer Pointer
//! @{

//! Register Offset (relative)
#define NEW_S2PTR1_CPU_IGP_15 0x100B4
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NEW_S2PTR1_CPU_IGP_15 0xE66900B4u

//! Register Reset Value
#define NEW_S2PTR1_CPU_IGP_15_RST 0x07800000u

//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_15_SRC_POOL_POS 0
//! Field SRC_POOL - Source Pool
#define NEW_S2PTR1_CPU_IGP_15_SRC_POOL_MASK 0xFu

//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_15_SA_POS 23
//! Field SA - Segment Address Upper bits
#define NEW_S2PTR1_CPU_IGP_15_SA_MASK 0x7800000u

//! @}

//! \defgroup DESC0_0_CPU_IGP_15 Register DESC0_0_CPU_IGP_15 - CPU Ingress Port Descriptor 0 DW0
//! @{

//! Register Offset (relative)
#define DESC0_0_CPU_IGP_15 0x10100
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_0_CPU_IGP_15 0xE6690100u

//! Register Reset Value
#define DESC0_0_CPU_IGP_15_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_15_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_0_CPU_IGP_15_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_0_CPU_IGP_15 Register DESC1_0_CPU_IGP_15 - CPU Ingress Port Descriptor 0 DW1
//! @{

//! Register Offset (relative)
#define DESC1_0_CPU_IGP_15 0x10104
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_0_CPU_IGP_15 0xE6690104u

//! Register Reset Value
#define DESC1_0_CPU_IGP_15_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_15_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_0_CPU_IGP_15_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_0_CPU_IGP_15 Register DESC2_0_CPU_IGP_15 - CPU Ingress Port Descriptor 0 DW2
//! @{

//! Register Offset (relative)
#define DESC2_0_CPU_IGP_15 0x10108
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_0_CPU_IGP_15 0xE6690108u

//! Register Reset Value
#define DESC2_0_CPU_IGP_15_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_15_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_0_CPU_IGP_15_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_0_CPU_IGP_15 Register DESC3_0_CPU_IGP_15 - CPU Ingress Port Descriptor 0 DW3
//! @{

//! Register Offset (relative)
#define DESC3_0_CPU_IGP_15 0x1010C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_0_CPU_IGP_15 0xE669010Cu

//! Register Reset Value
#define DESC3_0_CPU_IGP_15_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_15_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_0_CPU_IGP_15_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC0_1_CPU_IGP_15 Register DESC0_1_CPU_IGP_15 - CPU Ingress Port Descriptor 1 DW0
//! @{

//! Register Offset (relative)
#define DESC0_1_CPU_IGP_15 0x10110
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC0_1_CPU_IGP_15 0xE6690110u

//! Register Reset Value
#define DESC0_1_CPU_IGP_15_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_15_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DESC0_1_CPU_IGP_15_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC1_1_CPU_IGP_15 Register DESC1_1_CPU_IGP_15 - CPU Ingress Port Descriptor 1 DW1
//! @{

//! Register Offset (relative)
#define DESC1_1_CPU_IGP_15 0x10114
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC1_1_CPU_IGP_15 0xE6690114u

//! Register Reset Value
#define DESC1_1_CPU_IGP_15_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_15_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DESC1_1_CPU_IGP_15_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC2_1_CPU_IGP_15 Register DESC2_1_CPU_IGP_15 - CPU Ingress Port Descriptor 1 DW2
//! @{

//! Register Offset (relative)
#define DESC2_1_CPU_IGP_15 0x10118
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC2_1_CPU_IGP_15 0xE6690118u

//! Register Reset Value
#define DESC2_1_CPU_IGP_15_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_15_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DESC2_1_CPU_IGP_15_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DESC3_1_CPU_IGP_15 Register DESC3_1_CPU_IGP_15 - CPU Ingress Port Descriptor 1 DW3
//! @{

//! Register Offset (relative)
#define DESC3_1_CPU_IGP_15 0x1011C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC3_1_CPU_IGP_15 0xE669011Cu

//! Register Reset Value
#define DESC3_1_CPU_IGP_15_RST 0x80000000u

//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_15_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DESC3_1_CPU_IGP_15_DESC_MASK 0x7FFFFFFFu

//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_15_OWN_POS 31
//! Field OWN - OWN bit
#define DESC3_1_CPU_IGP_15_OWN_MASK 0x80000000u

//! @}

//! \defgroup CFG_DC_IGP_16 Register CFG_DC_IGP_16 - DC Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DC_IGP_16 0x18000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DC_IGP_16 0xE6698000u

//! Register Reset Value
#define CFG_DC_IGP_16_RST 0x00000000u

//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_16_EQREQ_POS 0
//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_16_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_16_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_16_EQREQ_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_16_BRBPEN_POS 1
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_16_BRBPEN_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_16_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_16_BRBPEN_EN 0x1

//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_16_BUFREQ0_POS 2
//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_16_BUFREQ0_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_16_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_16_BUFREQ0_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_16_QOSBYPSEN_POS 6
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_16_QOSBYPSEN_MASK 0x40u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_16_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_16_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_16_FSQMQEN_POS 7
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_16_FSQMQEN_MASK 0x80u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_16_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_16_FSQMQEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_16_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_16_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_16_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_16_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_16_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_16_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_16_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_16_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DC_IGP_16_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DC_IGP_16_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_16_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_16_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DC_IGP_16_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DC_IGP_16_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DC_IGP_16_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DC_IGP_16_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DC_IGP_16_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DC_IGP_16_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DC_IGP_16_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DC_IGP_16_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DC_IGP_16_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DC_IGP_16_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DC_IGP_16_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DC_IGP_16_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DC_IGP_16_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DC_IGP_16_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DC_IGP_16_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DC_IGP_16_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DC_IGP_16_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DC_IGP_16_DISC_REASON15 0xF

//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_16_BASEPOLICY_POS 20
//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_16_BASEPOLICY_MASK 0xFF00000u

//! @}

//! \defgroup DESC_CONV_DC_IGP_16 Register DESC_CONV_DC_IGP_16 - DC Ingress Port descriptor Conversion config
//! @{

//! Register Offset (relative)
#define DESC_CONV_DC_IGP_16 0x18004
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC_CONV_DC_IGP_16 0xE6698004u

//! Register Reset Value
#define DESC_CONV_DC_IGP_16_RST 0x00000000u

//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_16_TS_POS 0
//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_16_TS_MASK 0x1u

//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_16_FCS_POS 1
//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_16_FCS_MASK 0x2u

//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_16_CLASS_EN_POS 2
//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_16_CLASS_EN_MASK 0x4u

//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_16_PKT_TYPE_POS 3
//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_16_PKT_TYPE_MASK 0x18u

//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_16_PMAC_EN_POS 8
//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_16_PMAC_EN_MASK 0x100u

//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_16_PMAC_POS 9
//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_16_PMAC_MASK 0x200u

//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_16_PREL2_EN_POS 12
//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_16_PREL2_EN_MASK 0x1000u

//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_16_PREL2_POS 13
//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_16_PREL2_MASK 0x6000u

//! @}

//! \defgroup EQPC_DC_IGP_16 Register EQPC_DC_IGP_16 - DC Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DC_IGP_16 0x1800C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DC_IGP_16 0xE669800Cu

//! Register Reset Value
#define EQPC_DC_IGP_16_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_16_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_16_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DC_IGP_16 Register DISC_DESC0_DC_IGP_16 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DC_IGP_16 0x18010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DC_IGP_16 0xE6698010u

//! Register Reset Value
#define DISC_DESC0_DC_IGP_16_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_16_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_16_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DC_IGP_16 Register DISC_DESC1_DC_IGP_16 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DC_IGP_16 0x18014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DC_IGP_16 0xE6698014u

//! Register Reset Value
#define DISC_DESC1_DC_IGP_16_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_16_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_16_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DC_IGP_16 Register DISC_DESC2_DC_IGP_16 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DC_IGP_16 0x18018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DC_IGP_16 0xE6698018u

//! Register Reset Value
#define DISC_DESC2_DC_IGP_16_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_16_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_16_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DC_IGP_16 Register DISC_DESC3_DC_IGP_16 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DC_IGP_16 0x1801C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DC_IGP_16 0xE669801Cu

//! Register Reset Value
#define DISC_DESC3_DC_IGP_16_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_16_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_16_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DC_IGP_16 Register IRNCR_DC_IGP_16 - DC Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DC_IGP_16 0x18020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DC_IGP_16 0xE6698020u

//! Register Reset Value
#define IRNCR_DC_IGP_16_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_16_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_16_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_16_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DC_IGP_16_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_16_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_16_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_16_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_16_DR_INTOCC 0x1

//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_16_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_16_PR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_16_PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_16_PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DC_IGP_16 Register IRNICR_DC_IGP_16 - DC Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DC_IGP_16 0x18024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DC_IGP_16 0xE6698024u

//! Register Reset Value
#define IRNICR_DC_IGP_16_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_16_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_16_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_16_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_16_DR_MASK 0x8u

//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_16_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_16_PR_MASK 0x10u

//! @}

//! \defgroup IRNEN_DC_IGP_16 Register IRNEN_DC_IGP_16 - DC Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DC_IGP_16 0x18028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DC_IGP_16 0xE6698028u

//! Register Reset Value
#define IRNEN_DC_IGP_16_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_16_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_16_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_16_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_16_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_16_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_16_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_16_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_16_DR_EN 0x1

//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_16_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_16_PR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_16_PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_16_PR_EN 0x1

//! @}

//! \defgroup DPTR_DC_IGP_16 Register DPTR_DC_IGP_16 - DC Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DC_IGP_16 0x18030
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DC_IGP_16 0xE6698030u

//! Register Reset Value
#define DPTR_DC_IGP_16_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_16_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_16_ND_MASK 0x1Fu

//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_16_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_16_DPTR_MASK 0x1F0000u

//! @}

//! \defgroup DROP_IGP_16 Register DROP_IGP_16 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_16 0x1803C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_16 0xE669803Cu

//! Register Reset Value
#define DROP_IGP_16_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_16_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_16_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NBPC_DC_IGP_16 Register NBPC_DC_IGP_16 - DC Ingress Port buffer pointer counter
//! @{

//! Register Offset (relative)
#define NBPC_DC_IGP_16 0x18040
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NBPC_DC_IGP_16 0xE6698040u

//! Register Reset Value
#define NBPC_DC_IGP_16_RST 0x00000000u

//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_16_NS0PC_POS 0
//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_16_NS0PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DEVQOS_TO_QUEUE0_7_DC_IGP_16 Register DEVQOS_TO_QUEUE0_7_DC_IGP_16 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16 0x18050
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE0_7_DC_IGP_16 0xE6698050u

//! Register Reset Value
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_16_QID7_MASK 0x70000000u

//! @}

//! \defgroup DEVQOS_TO_QUEUE8_15_DC_IGP_16 Register DEVQOS_TO_QUEUE8_15_DC_IGP_16 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16 0x18054
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE8_15_DC_IGP_16 0xE6698054u

//! Register Reset Value
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_16_QID7_MASK 0x70000000u

//! @}

//! \defgroup DCNTR_IGP_16 Register DCNTR_IGP_16 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_16 0x18060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_16 0xE6698060u

//! Register Reset Value
#define DCNTR_IGP_16_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_16_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_16_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_16_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_16_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DC_IGP_17 Register CFG_DC_IGP_17 - DC Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DC_IGP_17 0x18400
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DC_IGP_17 0xE6698400u

//! Register Reset Value
#define CFG_DC_IGP_17_RST 0x00000000u

//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_17_EQREQ_POS 0
//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_17_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_17_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_17_EQREQ_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_17_BRBPEN_POS 1
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_17_BRBPEN_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_17_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_17_BRBPEN_EN 0x1

//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_17_BUFREQ0_POS 2
//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_17_BUFREQ0_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_17_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_17_BUFREQ0_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_17_QOSBYPSEN_POS 6
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_17_QOSBYPSEN_MASK 0x40u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_17_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_17_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_17_FSQMQEN_POS 7
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_17_FSQMQEN_MASK 0x80u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_17_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_17_FSQMQEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_17_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_17_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_17_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_17_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_17_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_17_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_17_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_17_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DC_IGP_17_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DC_IGP_17_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_17_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_17_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DC_IGP_17_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DC_IGP_17_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DC_IGP_17_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DC_IGP_17_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DC_IGP_17_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DC_IGP_17_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DC_IGP_17_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DC_IGP_17_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DC_IGP_17_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DC_IGP_17_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DC_IGP_17_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DC_IGP_17_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DC_IGP_17_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DC_IGP_17_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DC_IGP_17_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DC_IGP_17_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DC_IGP_17_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DC_IGP_17_DISC_REASON15 0xF

//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_17_BASEPOLICY_POS 20
//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_17_BASEPOLICY_MASK 0xFF00000u

//! @}

//! \defgroup DESC_CONV_DC_IGP_17 Register DESC_CONV_DC_IGP_17 - DC Ingress Port descriptor Conversion config
//! @{

//! Register Offset (relative)
#define DESC_CONV_DC_IGP_17 0x18404
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC_CONV_DC_IGP_17 0xE6698404u

//! Register Reset Value
#define DESC_CONV_DC_IGP_17_RST 0x00000000u

//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_17_TS_POS 0
//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_17_TS_MASK 0x1u

//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_17_FCS_POS 1
//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_17_FCS_MASK 0x2u

//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_17_CLASS_EN_POS 2
//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_17_CLASS_EN_MASK 0x4u

//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_17_PKT_TYPE_POS 3
//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_17_PKT_TYPE_MASK 0x18u

//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_17_PMAC_EN_POS 8
//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_17_PMAC_EN_MASK 0x100u

//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_17_PMAC_POS 9
//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_17_PMAC_MASK 0x200u

//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_17_PREL2_EN_POS 12
//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_17_PREL2_EN_MASK 0x1000u

//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_17_PREL2_POS 13
//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_17_PREL2_MASK 0x6000u

//! @}

//! \defgroup EQPC_DC_IGP_17 Register EQPC_DC_IGP_17 - DC Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DC_IGP_17 0x1840C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DC_IGP_17 0xE669840Cu

//! Register Reset Value
#define EQPC_DC_IGP_17_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_17_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_17_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DC_IGP_17 Register DISC_DESC0_DC_IGP_17 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DC_IGP_17 0x18410
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DC_IGP_17 0xE6698410u

//! Register Reset Value
#define DISC_DESC0_DC_IGP_17_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_17_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_17_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DC_IGP_17 Register DISC_DESC1_DC_IGP_17 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DC_IGP_17 0x18414
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DC_IGP_17 0xE6698414u

//! Register Reset Value
#define DISC_DESC1_DC_IGP_17_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_17_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_17_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DC_IGP_17 Register DISC_DESC2_DC_IGP_17 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DC_IGP_17 0x18418
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DC_IGP_17 0xE6698418u

//! Register Reset Value
#define DISC_DESC2_DC_IGP_17_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_17_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_17_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DC_IGP_17 Register DISC_DESC3_DC_IGP_17 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DC_IGP_17 0x1841C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DC_IGP_17 0xE669841Cu

//! Register Reset Value
#define DISC_DESC3_DC_IGP_17_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_17_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_17_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DC_IGP_17 Register IRNCR_DC_IGP_17 - DC Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DC_IGP_17 0x18420
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DC_IGP_17 0xE6698420u

//! Register Reset Value
#define IRNCR_DC_IGP_17_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_17_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_17_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_17_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DC_IGP_17_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_17_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_17_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_17_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_17_DR_INTOCC 0x1

//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_17_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_17_PR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_17_PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_17_PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DC_IGP_17 Register IRNICR_DC_IGP_17 - DC Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DC_IGP_17 0x18424
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DC_IGP_17 0xE6698424u

//! Register Reset Value
#define IRNICR_DC_IGP_17_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_17_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_17_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_17_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_17_DR_MASK 0x8u

//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_17_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_17_PR_MASK 0x10u

//! @}

//! \defgroup IRNEN_DC_IGP_17 Register IRNEN_DC_IGP_17 - DC Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DC_IGP_17 0x18428
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DC_IGP_17 0xE6698428u

//! Register Reset Value
#define IRNEN_DC_IGP_17_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_17_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_17_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_17_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_17_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_17_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_17_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_17_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_17_DR_EN 0x1

//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_17_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_17_PR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_17_PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_17_PR_EN 0x1

//! @}

//! \defgroup DPTR_DC_IGP_17 Register DPTR_DC_IGP_17 - DC Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DC_IGP_17 0x18430
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DC_IGP_17 0xE6698430u

//! Register Reset Value
#define DPTR_DC_IGP_17_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_17_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_17_ND_MASK 0x1Fu

//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_17_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_17_DPTR_MASK 0x1F0000u

//! @}

//! \defgroup DROP_IGP_17 Register DROP_IGP_17 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_17 0x1843C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_17 0xE669843Cu

//! Register Reset Value
#define DROP_IGP_17_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_17_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_17_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NBPC_DC_IGP_17 Register NBPC_DC_IGP_17 - DC Ingress Port buffer pointer counter
//! @{

//! Register Offset (relative)
#define NBPC_DC_IGP_17 0x18440
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NBPC_DC_IGP_17 0xE6698440u

//! Register Reset Value
#define NBPC_DC_IGP_17_RST 0x00000000u

//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_17_NS0PC_POS 0
//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_17_NS0PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DEVQOS_TO_QUEUE0_7_DC_IGP_17 Register DEVQOS_TO_QUEUE0_7_DC_IGP_17 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17 0x18450
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE0_7_DC_IGP_17 0xE6698450u

//! Register Reset Value
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_17_QID7_MASK 0x70000000u

//! @}

//! \defgroup DEVQOS_TO_QUEUE8_15_DC_IGP_17 Register DEVQOS_TO_QUEUE8_15_DC_IGP_17 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17 0x18454
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE8_15_DC_IGP_17 0xE6698454u

//! Register Reset Value
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_17_QID7_MASK 0x70000000u

//! @}

//! \defgroup DCNTR_IGP_17 Register DCNTR_IGP_17 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_17 0x18460
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_17 0xE6698460u

//! Register Reset Value
#define DCNTR_IGP_17_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_17_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_17_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_17_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_17_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DC_IGP_18 Register CFG_DC_IGP_18 - DC Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DC_IGP_18 0x18800
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DC_IGP_18 0xE6698800u

//! Register Reset Value
#define CFG_DC_IGP_18_RST 0x00000000u

//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_18_EQREQ_POS 0
//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_18_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_18_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_18_EQREQ_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_18_BRBPEN_POS 1
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_18_BRBPEN_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_18_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_18_BRBPEN_EN 0x1

//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_18_BUFREQ0_POS 2
//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_18_BUFREQ0_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_18_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_18_BUFREQ0_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_18_QOSBYPSEN_POS 6
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_18_QOSBYPSEN_MASK 0x40u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_18_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_18_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_18_FSQMQEN_POS 7
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_18_FSQMQEN_MASK 0x80u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_18_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_18_FSQMQEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_18_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_18_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_18_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_18_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_18_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_18_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_18_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_18_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DC_IGP_18_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DC_IGP_18_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_18_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_18_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DC_IGP_18_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DC_IGP_18_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DC_IGP_18_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DC_IGP_18_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DC_IGP_18_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DC_IGP_18_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DC_IGP_18_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DC_IGP_18_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DC_IGP_18_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DC_IGP_18_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DC_IGP_18_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DC_IGP_18_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DC_IGP_18_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DC_IGP_18_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DC_IGP_18_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DC_IGP_18_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DC_IGP_18_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DC_IGP_18_DISC_REASON15 0xF

//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_18_BASEPOLICY_POS 20
//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_18_BASEPOLICY_MASK 0xFF00000u

//! @}

//! \defgroup DESC_CONV_DC_IGP_18 Register DESC_CONV_DC_IGP_18 - DC Ingress Port descriptor Conversion config
//! @{

//! Register Offset (relative)
#define DESC_CONV_DC_IGP_18 0x18804
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC_CONV_DC_IGP_18 0xE6698804u

//! Register Reset Value
#define DESC_CONV_DC_IGP_18_RST 0x00000000u

//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_18_TS_POS 0
//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_18_TS_MASK 0x1u

//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_18_FCS_POS 1
//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_18_FCS_MASK 0x2u

//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_18_CLASS_EN_POS 2
//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_18_CLASS_EN_MASK 0x4u

//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_18_PKT_TYPE_POS 3
//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_18_PKT_TYPE_MASK 0x18u

//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_18_PMAC_EN_POS 8
//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_18_PMAC_EN_MASK 0x100u

//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_18_PMAC_POS 9
//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_18_PMAC_MASK 0x200u

//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_18_PREL2_EN_POS 12
//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_18_PREL2_EN_MASK 0x1000u

//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_18_PREL2_POS 13
//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_18_PREL2_MASK 0x6000u

//! @}

//! \defgroup EQPC_DC_IGP_18 Register EQPC_DC_IGP_18 - DC Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DC_IGP_18 0x1880C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DC_IGP_18 0xE669880Cu

//! Register Reset Value
#define EQPC_DC_IGP_18_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_18_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_18_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DC_IGP_18 Register DISC_DESC0_DC_IGP_18 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DC_IGP_18 0x18810
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DC_IGP_18 0xE6698810u

//! Register Reset Value
#define DISC_DESC0_DC_IGP_18_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_18_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_18_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DC_IGP_18 Register DISC_DESC1_DC_IGP_18 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DC_IGP_18 0x18814
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DC_IGP_18 0xE6698814u

//! Register Reset Value
#define DISC_DESC1_DC_IGP_18_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_18_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_18_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DC_IGP_18 Register DISC_DESC2_DC_IGP_18 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DC_IGP_18 0x18818
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DC_IGP_18 0xE6698818u

//! Register Reset Value
#define DISC_DESC2_DC_IGP_18_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_18_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_18_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DC_IGP_18 Register DISC_DESC3_DC_IGP_18 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DC_IGP_18 0x1881C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DC_IGP_18 0xE669881Cu

//! Register Reset Value
#define DISC_DESC3_DC_IGP_18_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_18_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_18_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DC_IGP_18 Register IRNCR_DC_IGP_18 - DC Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DC_IGP_18 0x18820
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DC_IGP_18 0xE6698820u

//! Register Reset Value
#define IRNCR_DC_IGP_18_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_18_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_18_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_18_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DC_IGP_18_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_18_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_18_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_18_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_18_DR_INTOCC 0x1

//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_18_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_18_PR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_18_PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_18_PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DC_IGP_18 Register IRNICR_DC_IGP_18 - DC Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DC_IGP_18 0x18824
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DC_IGP_18 0xE6698824u

//! Register Reset Value
#define IRNICR_DC_IGP_18_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_18_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_18_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_18_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_18_DR_MASK 0x8u

//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_18_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_18_PR_MASK 0x10u

//! @}

//! \defgroup IRNEN_DC_IGP_18 Register IRNEN_DC_IGP_18 - DC Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DC_IGP_18 0x18828
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DC_IGP_18 0xE6698828u

//! Register Reset Value
#define IRNEN_DC_IGP_18_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_18_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_18_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_18_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_18_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_18_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_18_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_18_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_18_DR_EN 0x1

//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_18_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_18_PR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_18_PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_18_PR_EN 0x1

//! @}

//! \defgroup DPTR_DC_IGP_18 Register DPTR_DC_IGP_18 - DC Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DC_IGP_18 0x18830
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DC_IGP_18 0xE6698830u

//! Register Reset Value
#define DPTR_DC_IGP_18_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_18_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_18_ND_MASK 0x1Fu

//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_18_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_18_DPTR_MASK 0x1F0000u

//! @}

//! \defgroup DROP_IGP_18 Register DROP_IGP_18 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_18 0x1883C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_18 0xE669883Cu

//! Register Reset Value
#define DROP_IGP_18_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_18_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_18_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NBPC_DC_IGP_18 Register NBPC_DC_IGP_18 - DC Ingress Port buffer pointer counter
//! @{

//! Register Offset (relative)
#define NBPC_DC_IGP_18 0x18840
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NBPC_DC_IGP_18 0xE6698840u

//! Register Reset Value
#define NBPC_DC_IGP_18_RST 0x00000000u

//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_18_NS0PC_POS 0
//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_18_NS0PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DEVQOS_TO_QUEUE0_7_DC_IGP_18 Register DEVQOS_TO_QUEUE0_7_DC_IGP_18 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18 0x18850
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE0_7_DC_IGP_18 0xE6698850u

//! Register Reset Value
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_18_QID7_MASK 0x70000000u

//! @}

//! \defgroup DEVQOS_TO_QUEUE8_15_DC_IGP_18 Register DEVQOS_TO_QUEUE8_15_DC_IGP_18 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18 0x18854
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE8_15_DC_IGP_18 0xE6698854u

//! Register Reset Value
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_18_QID7_MASK 0x70000000u

//! @}

//! \defgroup DCNTR_IGP_18 Register DCNTR_IGP_18 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_18 0x18860
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_18 0xE6698860u

//! Register Reset Value
#define DCNTR_IGP_18_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_18_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_18_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_18_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_18_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DC_IGP_19 Register CFG_DC_IGP_19 - DC Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DC_IGP_19 0x18C00
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DC_IGP_19 0xE6698C00u

//! Register Reset Value
#define CFG_DC_IGP_19_RST 0x00000000u

//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_19_EQREQ_POS 0
//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_19_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_19_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_19_EQREQ_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_19_BRBPEN_POS 1
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_19_BRBPEN_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_19_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_19_BRBPEN_EN 0x1

//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_19_BUFREQ0_POS 2
//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_19_BUFREQ0_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_19_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_19_BUFREQ0_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_19_QOSBYPSEN_POS 6
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_19_QOSBYPSEN_MASK 0x40u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_19_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_19_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_19_FSQMQEN_POS 7
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_19_FSQMQEN_MASK 0x80u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_19_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_19_FSQMQEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_19_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_19_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_19_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_19_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_19_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_19_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_19_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_19_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DC_IGP_19_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DC_IGP_19_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_19_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_19_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DC_IGP_19_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DC_IGP_19_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DC_IGP_19_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DC_IGP_19_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DC_IGP_19_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DC_IGP_19_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DC_IGP_19_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DC_IGP_19_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DC_IGP_19_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DC_IGP_19_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DC_IGP_19_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DC_IGP_19_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DC_IGP_19_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DC_IGP_19_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DC_IGP_19_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DC_IGP_19_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DC_IGP_19_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DC_IGP_19_DISC_REASON15 0xF

//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_19_BASEPOLICY_POS 20
//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_19_BASEPOLICY_MASK 0xFF00000u

//! @}

//! \defgroup DESC_CONV_DC_IGP_19 Register DESC_CONV_DC_IGP_19 - DC Ingress Port descriptor Conversion config
//! @{

//! Register Offset (relative)
#define DESC_CONV_DC_IGP_19 0x18C04
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC_CONV_DC_IGP_19 0xE6698C04u

//! Register Reset Value
#define DESC_CONV_DC_IGP_19_RST 0x00000000u

//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_19_TS_POS 0
//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_19_TS_MASK 0x1u

//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_19_FCS_POS 1
//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_19_FCS_MASK 0x2u

//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_19_CLASS_EN_POS 2
//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_19_CLASS_EN_MASK 0x4u

//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_19_PKT_TYPE_POS 3
//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_19_PKT_TYPE_MASK 0x18u

//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_19_PMAC_EN_POS 8
//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_19_PMAC_EN_MASK 0x100u

//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_19_PMAC_POS 9
//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_19_PMAC_MASK 0x200u

//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_19_PREL2_EN_POS 12
//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_19_PREL2_EN_MASK 0x1000u

//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_19_PREL2_POS 13
//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_19_PREL2_MASK 0x6000u

//! @}

//! \defgroup EQPC_DC_IGP_19 Register EQPC_DC_IGP_19 - DC Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DC_IGP_19 0x18C0C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DC_IGP_19 0xE6698C0Cu

//! Register Reset Value
#define EQPC_DC_IGP_19_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_19_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_19_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DC_IGP_19 Register DISC_DESC0_DC_IGP_19 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DC_IGP_19 0x18C10
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DC_IGP_19 0xE6698C10u

//! Register Reset Value
#define DISC_DESC0_DC_IGP_19_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_19_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_19_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DC_IGP_19 Register DISC_DESC1_DC_IGP_19 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DC_IGP_19 0x18C14
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DC_IGP_19 0xE6698C14u

//! Register Reset Value
#define DISC_DESC1_DC_IGP_19_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_19_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_19_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DC_IGP_19 Register DISC_DESC2_DC_IGP_19 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DC_IGP_19 0x18C18
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DC_IGP_19 0xE6698C18u

//! Register Reset Value
#define DISC_DESC2_DC_IGP_19_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_19_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_19_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DC_IGP_19 Register DISC_DESC3_DC_IGP_19 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DC_IGP_19 0x18C1C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DC_IGP_19 0xE6698C1Cu

//! Register Reset Value
#define DISC_DESC3_DC_IGP_19_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_19_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_19_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DC_IGP_19 Register IRNCR_DC_IGP_19 - DC Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DC_IGP_19 0x18C20
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DC_IGP_19 0xE6698C20u

//! Register Reset Value
#define IRNCR_DC_IGP_19_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_19_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_19_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_19_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DC_IGP_19_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_19_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_19_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_19_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_19_DR_INTOCC 0x1

//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_19_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_19_PR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_19_PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_19_PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DC_IGP_19 Register IRNICR_DC_IGP_19 - DC Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DC_IGP_19 0x18C24
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DC_IGP_19 0xE6698C24u

//! Register Reset Value
#define IRNICR_DC_IGP_19_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_19_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_19_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_19_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_19_DR_MASK 0x8u

//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_19_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_19_PR_MASK 0x10u

//! @}

//! \defgroup IRNEN_DC_IGP_19 Register IRNEN_DC_IGP_19 - DC Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DC_IGP_19 0x18C28
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DC_IGP_19 0xE6698C28u

//! Register Reset Value
#define IRNEN_DC_IGP_19_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_19_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_19_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_19_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_19_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_19_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_19_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_19_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_19_DR_EN 0x1

//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_19_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_19_PR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_19_PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_19_PR_EN 0x1

//! @}

//! \defgroup DPTR_DC_IGP_19 Register DPTR_DC_IGP_19 - DC Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DC_IGP_19 0x18C30
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DC_IGP_19 0xE6698C30u

//! Register Reset Value
#define DPTR_DC_IGP_19_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_19_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_19_ND_MASK 0x1Fu

//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_19_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_19_DPTR_MASK 0x1F0000u

//! @}

//! \defgroup DROP_IGP_19 Register DROP_IGP_19 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_19 0x18C3C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_19 0xE6698C3Cu

//! Register Reset Value
#define DROP_IGP_19_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_19_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_19_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NBPC_DC_IGP_19 Register NBPC_DC_IGP_19 - DC Ingress Port buffer pointer counter
//! @{

//! Register Offset (relative)
#define NBPC_DC_IGP_19 0x18C40
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NBPC_DC_IGP_19 0xE6698C40u

//! Register Reset Value
#define NBPC_DC_IGP_19_RST 0x00000000u

//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_19_NS0PC_POS 0
//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_19_NS0PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DEVQOS_TO_QUEUE0_7_DC_IGP_19 Register DEVQOS_TO_QUEUE0_7_DC_IGP_19 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19 0x18C50
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE0_7_DC_IGP_19 0xE6698C50u

//! Register Reset Value
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_19_QID7_MASK 0x70000000u

//! @}

//! \defgroup DEVQOS_TO_QUEUE8_15_DC_IGP_19 Register DEVQOS_TO_QUEUE8_15_DC_IGP_19 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19 0x18C54
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE8_15_DC_IGP_19 0xE6698C54u

//! Register Reset Value
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_19_QID7_MASK 0x70000000u

//! @}

//! \defgroup DCNTR_IGP_19 Register DCNTR_IGP_19 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_19 0x18C60
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_19 0xE6698C60u

//! Register Reset Value
#define DCNTR_IGP_19_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_19_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_19_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_19_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_19_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DC_IGP_20 Register CFG_DC_IGP_20 - DC Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DC_IGP_20 0x19000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DC_IGP_20 0xE6699000u

//! Register Reset Value
#define CFG_DC_IGP_20_RST 0x00000000u

//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_20_EQREQ_POS 0
//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_20_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_20_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_20_EQREQ_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_20_BRBPEN_POS 1
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_20_BRBPEN_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_20_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_20_BRBPEN_EN 0x1

//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_20_BUFREQ0_POS 2
//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_20_BUFREQ0_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_20_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_20_BUFREQ0_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_20_QOSBYPSEN_POS 6
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_20_QOSBYPSEN_MASK 0x40u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_20_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_20_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_20_FSQMQEN_POS 7
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_20_FSQMQEN_MASK 0x80u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_20_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_20_FSQMQEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_20_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_20_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_20_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_20_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_20_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_20_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_20_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_20_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DC_IGP_20_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DC_IGP_20_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_20_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_20_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DC_IGP_20_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DC_IGP_20_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DC_IGP_20_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DC_IGP_20_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DC_IGP_20_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DC_IGP_20_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DC_IGP_20_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DC_IGP_20_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DC_IGP_20_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DC_IGP_20_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DC_IGP_20_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DC_IGP_20_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DC_IGP_20_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DC_IGP_20_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DC_IGP_20_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DC_IGP_20_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DC_IGP_20_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DC_IGP_20_DISC_REASON15 0xF

//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_20_BASEPOLICY_POS 20
//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_20_BASEPOLICY_MASK 0xFF00000u

//! @}

//! \defgroup DESC_CONV_DC_IGP_20 Register DESC_CONV_DC_IGP_20 - DC Ingress Port descriptor Conversion config
//! @{

//! Register Offset (relative)
#define DESC_CONV_DC_IGP_20 0x19004
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC_CONV_DC_IGP_20 0xE6699004u

//! Register Reset Value
#define DESC_CONV_DC_IGP_20_RST 0x00000000u

//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_20_TS_POS 0
//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_20_TS_MASK 0x1u

//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_20_FCS_POS 1
//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_20_FCS_MASK 0x2u

//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_20_CLASS_EN_POS 2
//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_20_CLASS_EN_MASK 0x4u

//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_20_PKT_TYPE_POS 3
//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_20_PKT_TYPE_MASK 0x18u

//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_20_PMAC_EN_POS 8
//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_20_PMAC_EN_MASK 0x100u

//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_20_PMAC_POS 9
//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_20_PMAC_MASK 0x200u

//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_20_PREL2_EN_POS 12
//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_20_PREL2_EN_MASK 0x1000u

//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_20_PREL2_POS 13
//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_20_PREL2_MASK 0x6000u

//! @}

//! \defgroup EQPC_DC_IGP_20 Register EQPC_DC_IGP_20 - DC Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DC_IGP_20 0x1900C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DC_IGP_20 0xE669900Cu

//! Register Reset Value
#define EQPC_DC_IGP_20_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_20_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_20_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DC_IGP_20 Register DISC_DESC0_DC_IGP_20 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DC_IGP_20 0x19010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DC_IGP_20 0xE6699010u

//! Register Reset Value
#define DISC_DESC0_DC_IGP_20_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_20_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_20_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DC_IGP_20 Register DISC_DESC1_DC_IGP_20 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DC_IGP_20 0x19014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DC_IGP_20 0xE6699014u

//! Register Reset Value
#define DISC_DESC1_DC_IGP_20_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_20_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_20_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DC_IGP_20 Register DISC_DESC2_DC_IGP_20 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DC_IGP_20 0x19018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DC_IGP_20 0xE6699018u

//! Register Reset Value
#define DISC_DESC2_DC_IGP_20_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_20_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_20_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DC_IGP_20 Register DISC_DESC3_DC_IGP_20 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DC_IGP_20 0x1901C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DC_IGP_20 0xE669901Cu

//! Register Reset Value
#define DISC_DESC3_DC_IGP_20_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_20_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_20_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DC_IGP_20 Register IRNCR_DC_IGP_20 - DC Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DC_IGP_20 0x19020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DC_IGP_20 0xE6699020u

//! Register Reset Value
#define IRNCR_DC_IGP_20_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_20_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_20_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_20_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DC_IGP_20_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_20_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_20_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_20_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_20_DR_INTOCC 0x1

//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_20_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_20_PR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_20_PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_20_PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DC_IGP_20 Register IRNICR_DC_IGP_20 - DC Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DC_IGP_20 0x19024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DC_IGP_20 0xE6699024u

//! Register Reset Value
#define IRNICR_DC_IGP_20_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_20_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_20_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_20_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_20_DR_MASK 0x8u

//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_20_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_20_PR_MASK 0x10u

//! @}

//! \defgroup IRNEN_DC_IGP_20 Register IRNEN_DC_IGP_20 - DC Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DC_IGP_20 0x19028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DC_IGP_20 0xE6699028u

//! Register Reset Value
#define IRNEN_DC_IGP_20_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_20_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_20_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_20_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_20_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_20_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_20_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_20_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_20_DR_EN 0x1

//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_20_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_20_PR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_20_PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_20_PR_EN 0x1

//! @}

//! \defgroup DPTR_DC_IGP_20 Register DPTR_DC_IGP_20 - DC Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DC_IGP_20 0x19030
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DC_IGP_20 0xE6699030u

//! Register Reset Value
#define DPTR_DC_IGP_20_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_20_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_20_ND_MASK 0x1Fu

//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_20_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_20_DPTR_MASK 0x1F0000u

//! @}

//! \defgroup DROP_IGP_20 Register DROP_IGP_20 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_20 0x1903C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_20 0xE669903Cu

//! Register Reset Value
#define DROP_IGP_20_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_20_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_20_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NBPC_DC_IGP_20 Register NBPC_DC_IGP_20 - DC Ingress Port buffer pointer counter
//! @{

//! Register Offset (relative)
#define NBPC_DC_IGP_20 0x19040
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NBPC_DC_IGP_20 0xE6699040u

//! Register Reset Value
#define NBPC_DC_IGP_20_RST 0x00000000u

//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_20_NS0PC_POS 0
//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_20_NS0PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DEVQOS_TO_QUEUE0_7_DC_IGP_20 Register DEVQOS_TO_QUEUE0_7_DC_IGP_20 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20 0x19050
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE0_7_DC_IGP_20 0xE6699050u

//! Register Reset Value
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_20_QID7_MASK 0x70000000u

//! @}

//! \defgroup DEVQOS_TO_QUEUE8_15_DC_IGP_20 Register DEVQOS_TO_QUEUE8_15_DC_IGP_20 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20 0x19054
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE8_15_DC_IGP_20 0xE6699054u

//! Register Reset Value
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_20_QID7_MASK 0x70000000u

//! @}

//! \defgroup DCNTR_IGP_20 Register DCNTR_IGP_20 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_20 0x19060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_20 0xE6699060u

//! Register Reset Value
#define DCNTR_IGP_20_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_20_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_20_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_20_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_20_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DC_IGP_21 Register CFG_DC_IGP_21 - DC Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DC_IGP_21 0x19400
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DC_IGP_21 0xE6699400u

//! Register Reset Value
#define CFG_DC_IGP_21_RST 0x00000000u

//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_21_EQREQ_POS 0
//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_21_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_21_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_21_EQREQ_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_21_BRBPEN_POS 1
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_21_BRBPEN_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_21_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_21_BRBPEN_EN 0x1

//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_21_BUFREQ0_POS 2
//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_21_BUFREQ0_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_21_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_21_BUFREQ0_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_21_QOSBYPSEN_POS 6
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_21_QOSBYPSEN_MASK 0x40u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_21_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_21_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_21_FSQMQEN_POS 7
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_21_FSQMQEN_MASK 0x80u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_21_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_21_FSQMQEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_21_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_21_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_21_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_21_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_21_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_21_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_21_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_21_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DC_IGP_21_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DC_IGP_21_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_21_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_21_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DC_IGP_21_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DC_IGP_21_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DC_IGP_21_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DC_IGP_21_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DC_IGP_21_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DC_IGP_21_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DC_IGP_21_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DC_IGP_21_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DC_IGP_21_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DC_IGP_21_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DC_IGP_21_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DC_IGP_21_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DC_IGP_21_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DC_IGP_21_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DC_IGP_21_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DC_IGP_21_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DC_IGP_21_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DC_IGP_21_DISC_REASON15 0xF

//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_21_BASEPOLICY_POS 20
//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_21_BASEPOLICY_MASK 0xFF00000u

//! @}

//! \defgroup DESC_CONV_DC_IGP_21 Register DESC_CONV_DC_IGP_21 - DC Ingress Port descriptor Conversion config
//! @{

//! Register Offset (relative)
#define DESC_CONV_DC_IGP_21 0x19404
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC_CONV_DC_IGP_21 0xE6699404u

//! Register Reset Value
#define DESC_CONV_DC_IGP_21_RST 0x00000000u

//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_21_TS_POS 0
//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_21_TS_MASK 0x1u

//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_21_FCS_POS 1
//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_21_FCS_MASK 0x2u

//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_21_CLASS_EN_POS 2
//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_21_CLASS_EN_MASK 0x4u

//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_21_PKT_TYPE_POS 3
//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_21_PKT_TYPE_MASK 0x18u

//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_21_PMAC_EN_POS 8
//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_21_PMAC_EN_MASK 0x100u

//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_21_PMAC_POS 9
//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_21_PMAC_MASK 0x200u

//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_21_PREL2_EN_POS 12
//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_21_PREL2_EN_MASK 0x1000u

//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_21_PREL2_POS 13
//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_21_PREL2_MASK 0x6000u

//! @}

//! \defgroup EQPC_DC_IGP_21 Register EQPC_DC_IGP_21 - DC Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DC_IGP_21 0x1940C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DC_IGP_21 0xE669940Cu

//! Register Reset Value
#define EQPC_DC_IGP_21_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_21_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_21_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DC_IGP_21 Register DISC_DESC0_DC_IGP_21 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DC_IGP_21 0x19410
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DC_IGP_21 0xE6699410u

//! Register Reset Value
#define DISC_DESC0_DC_IGP_21_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_21_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_21_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DC_IGP_21 Register DISC_DESC1_DC_IGP_21 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DC_IGP_21 0x19414
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DC_IGP_21 0xE6699414u

//! Register Reset Value
#define DISC_DESC1_DC_IGP_21_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_21_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_21_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DC_IGP_21 Register DISC_DESC2_DC_IGP_21 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DC_IGP_21 0x19418
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DC_IGP_21 0xE6699418u

//! Register Reset Value
#define DISC_DESC2_DC_IGP_21_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_21_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_21_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DC_IGP_21 Register DISC_DESC3_DC_IGP_21 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DC_IGP_21 0x1941C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DC_IGP_21 0xE669941Cu

//! Register Reset Value
#define DISC_DESC3_DC_IGP_21_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_21_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_21_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DC_IGP_21 Register IRNCR_DC_IGP_21 - DC Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DC_IGP_21 0x19420
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DC_IGP_21 0xE6699420u

//! Register Reset Value
#define IRNCR_DC_IGP_21_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_21_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_21_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_21_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DC_IGP_21_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_21_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_21_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_21_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_21_DR_INTOCC 0x1

//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_21_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_21_PR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_21_PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_21_PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DC_IGP_21 Register IRNICR_DC_IGP_21 - DC Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DC_IGP_21 0x19424
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DC_IGP_21 0xE6699424u

//! Register Reset Value
#define IRNICR_DC_IGP_21_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_21_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_21_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_21_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_21_DR_MASK 0x8u

//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_21_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_21_PR_MASK 0x10u

//! @}

//! \defgroup IRNEN_DC_IGP_21 Register IRNEN_DC_IGP_21 - DC Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DC_IGP_21 0x19428
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DC_IGP_21 0xE6699428u

//! Register Reset Value
#define IRNEN_DC_IGP_21_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_21_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_21_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_21_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_21_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_21_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_21_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_21_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_21_DR_EN 0x1

//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_21_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_21_PR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_21_PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_21_PR_EN 0x1

//! @}

//! \defgroup DPTR_DC_IGP_21 Register DPTR_DC_IGP_21 - DC Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DC_IGP_21 0x19430
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DC_IGP_21 0xE6699430u

//! Register Reset Value
#define DPTR_DC_IGP_21_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_21_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_21_ND_MASK 0x1Fu

//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_21_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_21_DPTR_MASK 0x1F0000u

//! @}

//! \defgroup DROP_IGP_21 Register DROP_IGP_21 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_21 0x1943C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_21 0xE669943Cu

//! Register Reset Value
#define DROP_IGP_21_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_21_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_21_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NBPC_DC_IGP_21 Register NBPC_DC_IGP_21 - DC Ingress Port buffer pointer counter
//! @{

//! Register Offset (relative)
#define NBPC_DC_IGP_21 0x19440
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NBPC_DC_IGP_21 0xE6699440u

//! Register Reset Value
#define NBPC_DC_IGP_21_RST 0x00000000u

//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_21_NS0PC_POS 0
//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_21_NS0PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DEVQOS_TO_QUEUE0_7_DC_IGP_21 Register DEVQOS_TO_QUEUE0_7_DC_IGP_21 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21 0x19450
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE0_7_DC_IGP_21 0xE6699450u

//! Register Reset Value
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_21_QID7_MASK 0x70000000u

//! @}

//! \defgroup DEVQOS_TO_QUEUE8_15_DC_IGP_21 Register DEVQOS_TO_QUEUE8_15_DC_IGP_21 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21 0x19454
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE8_15_DC_IGP_21 0xE6699454u

//! Register Reset Value
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_21_QID7_MASK 0x70000000u

//! @}

//! \defgroup DCNTR_IGP_21 Register DCNTR_IGP_21 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_21 0x19460
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_21 0xE6699460u

//! Register Reset Value
#define DCNTR_IGP_21_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_21_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_21_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_21_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_21_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DC_IGP_22 Register CFG_DC_IGP_22 - DC Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DC_IGP_22 0x19800
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DC_IGP_22 0xE6699800u

//! Register Reset Value
#define CFG_DC_IGP_22_RST 0x00000000u

//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_22_EQREQ_POS 0
//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_22_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_22_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_22_EQREQ_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_22_BRBPEN_POS 1
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_22_BRBPEN_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_22_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_22_BRBPEN_EN 0x1

//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_22_BUFREQ0_POS 2
//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_22_BUFREQ0_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_22_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_22_BUFREQ0_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_22_QOSBYPSEN_POS 6
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_22_QOSBYPSEN_MASK 0x40u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_22_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_22_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_22_FSQMQEN_POS 7
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_22_FSQMQEN_MASK 0x80u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_22_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_22_FSQMQEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_22_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_22_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_22_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_22_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_22_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_22_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_22_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_22_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DC_IGP_22_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DC_IGP_22_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_22_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_22_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DC_IGP_22_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DC_IGP_22_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DC_IGP_22_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DC_IGP_22_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DC_IGP_22_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DC_IGP_22_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DC_IGP_22_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DC_IGP_22_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DC_IGP_22_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DC_IGP_22_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DC_IGP_22_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DC_IGP_22_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DC_IGP_22_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DC_IGP_22_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DC_IGP_22_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DC_IGP_22_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DC_IGP_22_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DC_IGP_22_DISC_REASON15 0xF

//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_22_BASEPOLICY_POS 20
//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_22_BASEPOLICY_MASK 0xFF00000u

//! @}

//! \defgroup DESC_CONV_DC_IGP_22 Register DESC_CONV_DC_IGP_22 - DC Ingress Port descriptor Conversion config
//! @{

//! Register Offset (relative)
#define DESC_CONV_DC_IGP_22 0x19804
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC_CONV_DC_IGP_22 0xE6699804u

//! Register Reset Value
#define DESC_CONV_DC_IGP_22_RST 0x00000000u

//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_22_TS_POS 0
//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_22_TS_MASK 0x1u

//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_22_FCS_POS 1
//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_22_FCS_MASK 0x2u

//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_22_CLASS_EN_POS 2
//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_22_CLASS_EN_MASK 0x4u

//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_22_PKT_TYPE_POS 3
//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_22_PKT_TYPE_MASK 0x18u

//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_22_PMAC_EN_POS 8
//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_22_PMAC_EN_MASK 0x100u

//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_22_PMAC_POS 9
//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_22_PMAC_MASK 0x200u

//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_22_PREL2_EN_POS 12
//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_22_PREL2_EN_MASK 0x1000u

//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_22_PREL2_POS 13
//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_22_PREL2_MASK 0x6000u

//! @}

//! \defgroup EQPC_DC_IGP_22 Register EQPC_DC_IGP_22 - DC Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DC_IGP_22 0x1980C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DC_IGP_22 0xE669980Cu

//! Register Reset Value
#define EQPC_DC_IGP_22_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_22_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_22_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DC_IGP_22 Register DISC_DESC0_DC_IGP_22 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DC_IGP_22 0x19810
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DC_IGP_22 0xE6699810u

//! Register Reset Value
#define DISC_DESC0_DC_IGP_22_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_22_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_22_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DC_IGP_22 Register DISC_DESC1_DC_IGP_22 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DC_IGP_22 0x19814
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DC_IGP_22 0xE6699814u

//! Register Reset Value
#define DISC_DESC1_DC_IGP_22_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_22_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_22_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DC_IGP_22 Register DISC_DESC2_DC_IGP_22 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DC_IGP_22 0x19818
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DC_IGP_22 0xE6699818u

//! Register Reset Value
#define DISC_DESC2_DC_IGP_22_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_22_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_22_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DC_IGP_22 Register DISC_DESC3_DC_IGP_22 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DC_IGP_22 0x1981C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DC_IGP_22 0xE669981Cu

//! Register Reset Value
#define DISC_DESC3_DC_IGP_22_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_22_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_22_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DC_IGP_22 Register IRNCR_DC_IGP_22 - DC Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DC_IGP_22 0x19820
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DC_IGP_22 0xE6699820u

//! Register Reset Value
#define IRNCR_DC_IGP_22_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_22_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_22_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_22_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DC_IGP_22_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_22_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_22_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_22_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_22_DR_INTOCC 0x1

//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_22_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_22_PR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_22_PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_22_PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DC_IGP_22 Register IRNICR_DC_IGP_22 - DC Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DC_IGP_22 0x19824
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DC_IGP_22 0xE6699824u

//! Register Reset Value
#define IRNICR_DC_IGP_22_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_22_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_22_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_22_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_22_DR_MASK 0x8u

//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_22_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_22_PR_MASK 0x10u

//! @}

//! \defgroup IRNEN_DC_IGP_22 Register IRNEN_DC_IGP_22 - DC Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DC_IGP_22 0x19828
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DC_IGP_22 0xE6699828u

//! Register Reset Value
#define IRNEN_DC_IGP_22_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_22_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_22_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_22_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_22_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_22_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_22_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_22_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_22_DR_EN 0x1

//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_22_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_22_PR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_22_PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_22_PR_EN 0x1

//! @}

//! \defgroup DPTR_DC_IGP_22 Register DPTR_DC_IGP_22 - DC Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DC_IGP_22 0x19830
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DC_IGP_22 0xE6699830u

//! Register Reset Value
#define DPTR_DC_IGP_22_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_22_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_22_ND_MASK 0x1Fu

//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_22_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_22_DPTR_MASK 0x1F0000u

//! @}

//! \defgroup DROP_IGP_22 Register DROP_IGP_22 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_22 0x1983C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_22 0xE669983Cu

//! Register Reset Value
#define DROP_IGP_22_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_22_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_22_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NBPC_DC_IGP_22 Register NBPC_DC_IGP_22 - DC Ingress Port buffer pointer counter
//! @{

//! Register Offset (relative)
#define NBPC_DC_IGP_22 0x19840
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NBPC_DC_IGP_22 0xE6699840u

//! Register Reset Value
#define NBPC_DC_IGP_22_RST 0x00000000u

//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_22_NS0PC_POS 0
//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_22_NS0PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DEVQOS_TO_QUEUE0_7_DC_IGP_22 Register DEVQOS_TO_QUEUE0_7_DC_IGP_22 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22 0x19850
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE0_7_DC_IGP_22 0xE6699850u

//! Register Reset Value
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_22_QID7_MASK 0x70000000u

//! @}

//! \defgroup DEVQOS_TO_QUEUE8_15_DC_IGP_22 Register DEVQOS_TO_QUEUE8_15_DC_IGP_22 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22 0x19854
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE8_15_DC_IGP_22 0xE6699854u

//! Register Reset Value
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_22_QID7_MASK 0x70000000u

//! @}

//! \defgroup DCNTR_IGP_22 Register DCNTR_IGP_22 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_22 0x19860
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_22 0xE6699860u

//! Register Reset Value
#define DCNTR_IGP_22_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_22_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_22_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_22_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_22_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DC_IGP_23 Register CFG_DC_IGP_23 - DC Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DC_IGP_23 0x19C00
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DC_IGP_23 0xE6699C00u

//! Register Reset Value
#define CFG_DC_IGP_23_RST 0x00000000u

//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_23_EQREQ_POS 0
//! Field EQREQ - Enable DC Enqueue Request
#define CFG_DC_IGP_23_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_23_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_23_EQREQ_EN 0x1

//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_23_BRBPEN_POS 1
//! Field BRBPEN - Buffer Request Back Pressure Enable
#define CFG_DC_IGP_23_BRBPEN_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_23_BRBPEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_23_BRBPEN_EN 0x1

//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_23_BUFREQ0_POS 2
//! Field BUFREQ0 - Enable DC Buffer Request For Base Policy Packet
#define CFG_DC_IGP_23_BUFREQ0_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_23_BUFREQ0_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_23_BUFREQ0_EN 0x1

//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_23_QOSBYPSEN_POS 6
//! Field QOSBYPSEN - Enable QoS Bypass Enqueue Request
#define CFG_DC_IGP_23_QOSBYPSEN_MASK 0x40u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_23_QOSBYPSEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_23_QOSBYPSEN_EN 0x1

//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_23_FSQMQEN_POS 7
//! Field FSQMQEN - Enable FSQM Queue Enqueue Request
#define CFG_DC_IGP_23_FSQMQEN_MASK 0x80u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_23_FSQMQEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_23_FSQMQEN_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_23_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DC_IGP_23_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DC_IGP_23_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DC_IGP_23_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_23_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DC_IGP_23_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_23_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_23_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DC_IGP_23_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DC_IGP_23_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DC_IGP_23_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DC_IGP_23_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DC_IGP_23_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DC_IGP_23_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DC_IGP_23_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DC_IGP_23_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DC_IGP_23_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DC_IGP_23_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DC_IGP_23_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DC_IGP_23_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DC_IGP_23_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DC_IGP_23_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DC_IGP_23_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DC_IGP_23_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DC_IGP_23_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DC_IGP_23_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DC_IGP_23_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DC_IGP_23_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DC_IGP_23_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DC_IGP_23_DISC_REASON15 0xF

//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_23_BASEPOLICY_POS 20
//! Field BASEPOLICY - Base Policy Mapping for ACA/DC Port
#define CFG_DC_IGP_23_BASEPOLICY_MASK 0xFF00000u

//! @}

//! \defgroup DESC_CONV_DC_IGP_23 Register DESC_CONV_DC_IGP_23 - DC Ingress Port descriptor Conversion config
//! @{

//! Register Offset (relative)
#define DESC_CONV_DC_IGP_23 0x19C04
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DESC_CONV_DC_IGP_23 0xE6699C04u

//! Register Reset Value
#define DESC_CONV_DC_IGP_23_RST 0x00000000u

//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_23_TS_POS 0
//! Field TS - TS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_23_TS_MASK 0x1u

//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_23_FCS_POS 1
//! Field FCS - FCS Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_23_FCS_MASK 0x2u

//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_23_CLASS_EN_POS 2
//! Field CLASS_EN - CLASS_EN Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_23_CLASS_EN_MASK 0x4u

//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_23_PKT_TYPE_POS 3
//! Field PKT_TYPE - PKT_TYPE Field in QoS Descriptor for the Port
#define DESC_CONV_DC_IGP_23_PKT_TYPE_MASK 0x18u

//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_23_PMAC_EN_POS 8
//! Field PMAC_EN - PMAC_EN
#define DESC_CONV_DC_IGP_23_PMAC_EN_MASK 0x100u

//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_23_PMAC_POS 9
//! Field PMAC - PMAC
#define DESC_CONV_DC_IGP_23_PMAC_MASK 0x200u

//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_23_PREL2_EN_POS 12
//! Field PREL2_EN - PREL2_EN
#define DESC_CONV_DC_IGP_23_PREL2_EN_MASK 0x1000u

//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_23_PREL2_POS 13
//! Field PREL2 - PREL2
#define DESC_CONV_DC_IGP_23_PREL2_MASK 0x6000u

//! @}

//! \defgroup EQPC_DC_IGP_23 Register EQPC_DC_IGP_23 - DC Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DC_IGP_23 0x19C0C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DC_IGP_23 0xE6699C0Cu

//! Register Reset Value
#define EQPC_DC_IGP_23_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_23_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DC_IGP_23_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DC_IGP_23 Register DISC_DESC0_DC_IGP_23 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DC_IGP_23 0x19C10
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DC_IGP_23 0xE6699C10u

//! Register Reset Value
#define DISC_DESC0_DC_IGP_23_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_23_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DC_IGP_23_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DC_IGP_23 Register DISC_DESC1_DC_IGP_23 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DC_IGP_23 0x19C14
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DC_IGP_23 0xE6699C14u

//! Register Reset Value
#define DISC_DESC1_DC_IGP_23_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_23_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DC_IGP_23_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DC_IGP_23 Register DISC_DESC2_DC_IGP_23 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DC_IGP_23 0x19C18
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DC_IGP_23 0xE6699C18u

//! Register Reset Value
#define DISC_DESC2_DC_IGP_23_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_23_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DC_IGP_23_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DC_IGP_23 Register DISC_DESC3_DC_IGP_23 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DC_IGP_23 0x19C1C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DC_IGP_23 0xE6699C1Cu

//! Register Reset Value
#define DISC_DESC3_DC_IGP_23_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_23_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DC_IGP_23_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DC_IGP_23 Register IRNCR_DC_IGP_23 - DC Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DC_IGP_23 0x19C20
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DC_IGP_23 0xE6699C20u

//! Register Reset Value
#define IRNCR_DC_IGP_23_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_23_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DC_IGP_23_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_23_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DC_IGP_23_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_23_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DC_IGP_23_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_23_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_23_DR_INTOCC 0x1

//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_23_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNCR_DC_IGP_23_PR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_DC_IGP_23_PR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DC_IGP_23_PR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DC_IGP_23 Register IRNICR_DC_IGP_23 - DC Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DC_IGP_23 0x19C24
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DC_IGP_23 0xE6699C24u

//! Register Reset Value
#define IRNICR_DC_IGP_23_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_23_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DC_IGP_23_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_23_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DC_IGP_23_DR_MASK 0x8u

//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_23_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNICR_DC_IGP_23_PR_MASK 0x10u

//! @}

//! \defgroup IRNEN_DC_IGP_23 Register IRNEN_DC_IGP_23 - DC Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DC_IGP_23 0x19C28
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DC_IGP_23 0xE6699C28u

//! Register Reset Value
#define IRNEN_DC_IGP_23_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_23_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DC_IGP_23_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_23_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_23_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_23_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DC_IGP_23_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_23_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_23_DR_EN 0x1

//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_23_PR_POS 4
//! Field PR - Buffer Pointer Ready
#define IRNEN_DC_IGP_23_PR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_DC_IGP_23_PR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DC_IGP_23_PR_EN 0x1

//! @}

//! \defgroup DPTR_DC_IGP_23 Register DPTR_DC_IGP_23 - DC Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DC_IGP_23 0x19C30
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DC_IGP_23 0xE6699C30u

//! Register Reset Value
#define DPTR_DC_IGP_23_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_23_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DC_IGP_23_ND_MASK 0x1Fu

//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_23_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DC_IGP_23_DPTR_MASK 0x1F0000u

//! @}

//! \defgroup DROP_IGP_23 Register DROP_IGP_23 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_23 0x19C3C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_23 0xE6699C3Cu

//! Register Reset Value
#define DROP_IGP_23_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_23_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_23_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NBPC_DC_IGP_23 Register NBPC_DC_IGP_23 - DC Ingress Port buffer pointer counter
//! @{

//! Register Offset (relative)
#define NBPC_DC_IGP_23 0x19C40
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NBPC_DC_IGP_23 0xE6699C40u

//! Register Reset Value
#define NBPC_DC_IGP_23_RST 0x00000000u

//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_23_NS0PC_POS 0
//! Field NS0PC - Per DC Buffer Pointer Counter
#define NBPC_DC_IGP_23_NS0PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DEVQOS_TO_QUEUE0_7_DC_IGP_23 Register DEVQOS_TO_QUEUE0_7_DC_IGP_23 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23 0x19C50
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE0_7_DC_IGP_23 0xE6699C50u

//! Register Reset Value
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE0_7_DC_IGP_23_QID7_MASK 0x70000000u

//! @}

//! \defgroup DEVQOS_TO_QUEUE8_15_DC_IGP_23 Register DEVQOS_TO_QUEUE8_15_DC_IGP_23 - DeVQoS to Queue mapping register
//! @{

//! Register Offset (relative)
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23 0x19C54
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DEVQOS_TO_QUEUE8_15_DC_IGP_23 0xE6699C54u

//! Register Reset Value
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_RST 0x76543210u

//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID0_POS 0
//! Field QID0 - Queue for DEVQoS0
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID0_MASK 0x7u

//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID1_POS 4
//! Field QID1 - Queue for DEVQoS1
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID1_MASK 0x70u

//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID2_POS 8
//! Field QID2 - Queue for DEVQoS2
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID2_MASK 0x700u

//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID3_POS 12
//! Field QID3 - Queue for DEVQoS3
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID3_MASK 0x7000u

//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID4_POS 16
//! Field QID4 - Queue for DEVQoS4
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID4_MASK 0x70000u

//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID5_POS 20
//! Field QID5 - Queue for DEVQoS5
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID5_MASK 0x700000u

//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID6_POS 24
//! Field QID6 - Queue for DEVQoS6
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID6_MASK 0x7000000u

//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID7_POS 28
//! Field QID7 - Queue for DEVQoS7
#define DEVQOS_TO_QUEUE8_15_DC_IGP_23_QID7_MASK 0x70000000u

//! @}

//! \defgroup DCNTR_IGP_23 Register DCNTR_IGP_23 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_23 0x19C60
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_23 0xE6699C60u

//! Register Reset Value
#define DCNTR_IGP_23_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_23_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_23_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_23_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_23_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DMA_IGP_24 Register CFG_DMA_IGP_24 - DMA Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DMA_IGP_24 0x1A000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DMA_IGP_24 0xE669A000u

//! Register Reset Value
#define CFG_DMA_IGP_24_RST 0x00000000u

//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_24_EQREQ_POS 0
//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_24_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_24_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_24_EQREQ_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_24_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_24_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_24_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_24_DISCHALT_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_24_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_24_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_24_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_24_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_24_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_24_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_24_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_24_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DMA_IGP_24_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DMA_IGP_24_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_24_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_24_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DMA_IGP_24_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DMA_IGP_24_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DMA_IGP_24_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DMA_IGP_24_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DMA_IGP_24_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DMA_IGP_24_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DMA_IGP_24_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DMA_IGP_24_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DMA_IGP_24_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DMA_IGP_24_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DMA_IGP_24_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DMA_IGP_24_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DMA_IGP_24_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DMA_IGP_24_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DMA_IGP_24_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DMA_IGP_24_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DMA_IGP_24_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DMA_IGP_24_DISC_REASON15 0xF

//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_24_DIC_DROP_POS 20
//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_24_DIC_DROP_MASK 0x100000u

//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_24_H_MODE_POS 21
//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_24_H_MODE_MASK 0x200000u

//! @}

//! \defgroup EQPC_DMA_IGP_24 Register EQPC_DMA_IGP_24 - DMA Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DMA_IGP_24 0x1A00C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DMA_IGP_24 0xE669A00Cu

//! Register Reset Value
#define EQPC_DMA_IGP_24_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_24_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_24_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DMA_IGP_24 Register DISC_DESC0_DMA_IGP_24 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DMA_IGP_24 0x1A010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DMA_IGP_24 0xE669A010u

//! Register Reset Value
#define DISC_DESC0_DMA_IGP_24_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_24_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_24_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DMA_IGP_24 Register DISC_DESC1_DMA_IGP_24 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DMA_IGP_24 0x1A014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DMA_IGP_24 0xE669A014u

//! Register Reset Value
#define DISC_DESC1_DMA_IGP_24_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_24_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_24_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DMA_IGP_24 Register DISC_DESC2_DMA_IGP_24 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DMA_IGP_24 0x1A018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DMA_IGP_24 0xE669A018u

//! Register Reset Value
#define DISC_DESC2_DMA_IGP_24_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_24_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_24_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DMA_IGP_24 Register DISC_DESC3_DMA_IGP_24 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DMA_IGP_24 0x1A01C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DMA_IGP_24 0xE669A01Cu

//! Register Reset Value
#define DISC_DESC3_DMA_IGP_24_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_24_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_24_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DMA_IGP_24 Register IRNCR_DMA_IGP_24 - DMA Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DMA_IGP_24 0x1A020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DMA_IGP_24 0xE669A020u

//! Register Reset Value
#define IRNCR_DMA_IGP_24_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_24_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_24_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_24_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DMA_IGP_24_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_24_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_24_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_24_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DMA_IGP_24_DR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DMA_IGP_24 Register IRNICR_DMA_IGP_24 - DMA Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DMA_IGP_24 0x1A024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DMA_IGP_24 0xE669A024u

//! Register Reset Value
#define IRNICR_DMA_IGP_24_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_24_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_24_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_24_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_24_DR_MASK 0x8u

//! @}

//! \defgroup IRNEN_DMA_IGP_24 Register IRNEN_DMA_IGP_24 - DMA Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DMA_IGP_24 0x1A028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DMA_IGP_24 0xE669A028u

//! Register Reset Value
#define IRNEN_DMA_IGP_24_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_24_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_24_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_24_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_24_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_24_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_24_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_24_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_24_DR_EN 0x1

//! @}

//! \defgroup DPTR_DMA_IGP_24 Register DPTR_DMA_IGP_24 - DMA Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DMA_IGP_24 0x1A030
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DMA_IGP_24 0xE669A030u

//! Register Reset Value
#define DPTR_DMA_IGP_24_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_24_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_24_ND_MASK 0x7u

//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_24_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_24_DPTR_MASK 0x70000u

//! @}

//! \defgroup DICC_IGP_24 Register DICC_IGP_24 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_24 0x1A038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_24 0xE669A038u

//! Register Reset Value
#define DICC_IGP_24_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_24_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_24_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_24 Register DROP_IGP_24 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_24 0x1A03C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_24 0xE669A03Cu

//! Register Reset Value
#define DROP_IGP_24_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_24_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_24_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC0_DMA_IGP_24 Register DROP_DESC0_DMA_IGP_24 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DROP_DESC0_DMA_IGP_24 0x1A040
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC0_DMA_IGP_24 0xE669A040u

//! Register Reset Value
#define DROP_DESC0_DMA_IGP_24_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_24_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_24_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC1_DMA_IGP_24 Register DROP_DESC1_DMA_IGP_24 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DROP_DESC1_DMA_IGP_24 0x1A044
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC1_DMA_IGP_24 0xE669A044u

//! Register Reset Value
#define DROP_DESC1_DMA_IGP_24_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_24_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_24_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC2_DMA_IGP_24 Register DROP_DESC2_DMA_IGP_24 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DROP_DESC2_DMA_IGP_24 0x1A048
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC2_DMA_IGP_24 0xE669A048u

//! Register Reset Value
#define DROP_DESC2_DMA_IGP_24_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_24_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_24_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC3_DMA_IGP_24 Register DROP_DESC3_DMA_IGP_24 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DROP_DESC3_DMA_IGP_24 0x1A04C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC3_DMA_IGP_24 0xE669A04Cu

//! Register Reset Value
#define DROP_DESC3_DMA_IGP_24_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_24_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_24_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_24 Register DCNTR_IGP_24 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_24 0x1A060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_24 0xE669A060u

//! Register Reset Value
#define DCNTR_IGP_24_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_24_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_24_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_24_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_24_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DMA_IGP_25 Register CFG_DMA_IGP_25 - DMA Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DMA_IGP_25 0x1A400
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DMA_IGP_25 0xE669A400u

//! Register Reset Value
#define CFG_DMA_IGP_25_RST 0x00000000u

//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_25_EQREQ_POS 0
//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_25_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_25_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_25_EQREQ_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_25_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_25_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_25_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_25_DISCHALT_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_25_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_25_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_25_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_25_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_25_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_25_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_25_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_25_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DMA_IGP_25_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DMA_IGP_25_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_25_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_25_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DMA_IGP_25_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DMA_IGP_25_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DMA_IGP_25_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DMA_IGP_25_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DMA_IGP_25_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DMA_IGP_25_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DMA_IGP_25_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DMA_IGP_25_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DMA_IGP_25_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DMA_IGP_25_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DMA_IGP_25_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DMA_IGP_25_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DMA_IGP_25_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DMA_IGP_25_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DMA_IGP_25_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DMA_IGP_25_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DMA_IGP_25_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DMA_IGP_25_DISC_REASON15 0xF

//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_25_DIC_DROP_POS 20
//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_25_DIC_DROP_MASK 0x100000u

//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_25_H_MODE_POS 21
//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_25_H_MODE_MASK 0x200000u

//! @}

//! \defgroup EQPC_DMA_IGP_25 Register EQPC_DMA_IGP_25 - DMA Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DMA_IGP_25 0x1A40C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DMA_IGP_25 0xE669A40Cu

//! Register Reset Value
#define EQPC_DMA_IGP_25_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_25_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_25_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DMA_IGP_25 Register DISC_DESC0_DMA_IGP_25 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DMA_IGP_25 0x1A410
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DMA_IGP_25 0xE669A410u

//! Register Reset Value
#define DISC_DESC0_DMA_IGP_25_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_25_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_25_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DMA_IGP_25 Register DISC_DESC1_DMA_IGP_25 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DMA_IGP_25 0x1A414
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DMA_IGP_25 0xE669A414u

//! Register Reset Value
#define DISC_DESC1_DMA_IGP_25_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_25_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_25_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DMA_IGP_25 Register DISC_DESC2_DMA_IGP_25 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DMA_IGP_25 0x1A418
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DMA_IGP_25 0xE669A418u

//! Register Reset Value
#define DISC_DESC2_DMA_IGP_25_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_25_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_25_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DMA_IGP_25 Register DISC_DESC3_DMA_IGP_25 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DMA_IGP_25 0x1A41C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DMA_IGP_25 0xE669A41Cu

//! Register Reset Value
#define DISC_DESC3_DMA_IGP_25_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_25_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_25_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DMA_IGP_25 Register IRNCR_DMA_IGP_25 - DMA Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DMA_IGP_25 0x1A420
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DMA_IGP_25 0xE669A420u

//! Register Reset Value
#define IRNCR_DMA_IGP_25_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_25_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_25_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_25_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DMA_IGP_25_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_25_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_25_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_25_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DMA_IGP_25_DR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DMA_IGP_25 Register IRNICR_DMA_IGP_25 - DMA Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DMA_IGP_25 0x1A424
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DMA_IGP_25 0xE669A424u

//! Register Reset Value
#define IRNICR_DMA_IGP_25_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_25_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_25_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_25_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_25_DR_MASK 0x8u

//! @}

//! \defgroup IRNEN_DMA_IGP_25 Register IRNEN_DMA_IGP_25 - DMA Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DMA_IGP_25 0x1A428
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DMA_IGP_25 0xE669A428u

//! Register Reset Value
#define IRNEN_DMA_IGP_25_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_25_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_25_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_25_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_25_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_25_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_25_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_25_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_25_DR_EN 0x1

//! @}

//! \defgroup DPTR_DMA_IGP_25 Register DPTR_DMA_IGP_25 - DMA Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DMA_IGP_25 0x1A430
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DMA_IGP_25 0xE669A430u

//! Register Reset Value
#define DPTR_DMA_IGP_25_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_25_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_25_ND_MASK 0x7u

//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_25_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_25_DPTR_MASK 0x70000u

//! @}

//! \defgroup DICC_IGP_25 Register DICC_IGP_25 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_25 0x1A438
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_25 0xE669A438u

//! Register Reset Value
#define DICC_IGP_25_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_25_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_25_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_25 Register DROP_IGP_25 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_25 0x1A43C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_25 0xE669A43Cu

//! Register Reset Value
#define DROP_IGP_25_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_25_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_25_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC0_DMA_IGP_25 Register DROP_DESC0_DMA_IGP_25 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DROP_DESC0_DMA_IGP_25 0x1A440
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC0_DMA_IGP_25 0xE669A440u

//! Register Reset Value
#define DROP_DESC0_DMA_IGP_25_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_25_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_25_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC1_DMA_IGP_25 Register DROP_DESC1_DMA_IGP_25 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DROP_DESC1_DMA_IGP_25 0x1A444
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC1_DMA_IGP_25 0xE669A444u

//! Register Reset Value
#define DROP_DESC1_DMA_IGP_25_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_25_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_25_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC2_DMA_IGP_25 Register DROP_DESC2_DMA_IGP_25 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DROP_DESC2_DMA_IGP_25 0x1A448
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC2_DMA_IGP_25 0xE669A448u

//! Register Reset Value
#define DROP_DESC2_DMA_IGP_25_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_25_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_25_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC3_DMA_IGP_25 Register DROP_DESC3_DMA_IGP_25 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DROP_DESC3_DMA_IGP_25 0x1A44C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC3_DMA_IGP_25 0xE669A44Cu

//! Register Reset Value
#define DROP_DESC3_DMA_IGP_25_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_25_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_25_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_25 Register DCNTR_IGP_25 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_25 0x1A460
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_25 0xE669A460u

//! Register Reset Value
#define DCNTR_IGP_25_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_25_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_25_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_25_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_25_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DMA_IGP_26 Register CFG_DMA_IGP_26 - DMA Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DMA_IGP_26 0x1A800
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DMA_IGP_26 0xE669A800u

//! Register Reset Value
#define CFG_DMA_IGP_26_RST 0x00000000u

//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_26_EQREQ_POS 0
//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_26_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_26_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_26_EQREQ_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_26_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_26_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_26_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_26_DISCHALT_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_26_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_26_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_26_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_26_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_26_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_26_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_26_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_26_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DMA_IGP_26_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DMA_IGP_26_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_26_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_26_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DMA_IGP_26_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DMA_IGP_26_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DMA_IGP_26_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DMA_IGP_26_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DMA_IGP_26_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DMA_IGP_26_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DMA_IGP_26_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DMA_IGP_26_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DMA_IGP_26_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DMA_IGP_26_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DMA_IGP_26_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DMA_IGP_26_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DMA_IGP_26_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DMA_IGP_26_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DMA_IGP_26_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DMA_IGP_26_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DMA_IGP_26_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DMA_IGP_26_DISC_REASON15 0xF

//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_26_DIC_DROP_POS 20
//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_26_DIC_DROP_MASK 0x100000u

//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_26_H_MODE_POS 21
//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_26_H_MODE_MASK 0x200000u

//! @}

//! \defgroup EQPC_DMA_IGP_26 Register EQPC_DMA_IGP_26 - DMA Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DMA_IGP_26 0x1A80C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DMA_IGP_26 0xE669A80Cu

//! Register Reset Value
#define EQPC_DMA_IGP_26_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_26_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_26_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DMA_IGP_26 Register DISC_DESC0_DMA_IGP_26 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DMA_IGP_26 0x1A810
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DMA_IGP_26 0xE669A810u

//! Register Reset Value
#define DISC_DESC0_DMA_IGP_26_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_26_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_26_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DMA_IGP_26 Register DISC_DESC1_DMA_IGP_26 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DMA_IGP_26 0x1A814
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DMA_IGP_26 0xE669A814u

//! Register Reset Value
#define DISC_DESC1_DMA_IGP_26_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_26_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_26_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DMA_IGP_26 Register DISC_DESC2_DMA_IGP_26 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DMA_IGP_26 0x1A818
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DMA_IGP_26 0xE669A818u

//! Register Reset Value
#define DISC_DESC2_DMA_IGP_26_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_26_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_26_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DMA_IGP_26 Register DISC_DESC3_DMA_IGP_26 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DMA_IGP_26 0x1A81C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DMA_IGP_26 0xE669A81Cu

//! Register Reset Value
#define DISC_DESC3_DMA_IGP_26_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_26_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_26_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DMA_IGP_26 Register IRNCR_DMA_IGP_26 - DMA Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DMA_IGP_26 0x1A820
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DMA_IGP_26 0xE669A820u

//! Register Reset Value
#define IRNCR_DMA_IGP_26_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_26_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_26_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_26_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DMA_IGP_26_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_26_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_26_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_26_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DMA_IGP_26_DR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DMA_IGP_26 Register IRNICR_DMA_IGP_26 - DMA Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DMA_IGP_26 0x1A824
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DMA_IGP_26 0xE669A824u

//! Register Reset Value
#define IRNICR_DMA_IGP_26_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_26_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_26_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_26_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_26_DR_MASK 0x8u

//! @}

//! \defgroup IRNEN_DMA_IGP_26 Register IRNEN_DMA_IGP_26 - DMA Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DMA_IGP_26 0x1A828
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DMA_IGP_26 0xE669A828u

//! Register Reset Value
#define IRNEN_DMA_IGP_26_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_26_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_26_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_26_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_26_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_26_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_26_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_26_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_26_DR_EN 0x1

//! @}

//! \defgroup DPTR_DMA_IGP_26 Register DPTR_DMA_IGP_26 - DMA Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DMA_IGP_26 0x1A830
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DMA_IGP_26 0xE669A830u

//! Register Reset Value
#define DPTR_DMA_IGP_26_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_26_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_26_ND_MASK 0x7u

//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_26_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_26_DPTR_MASK 0x70000u

//! @}

//! \defgroup DICC_IGP_26 Register DICC_IGP_26 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_26 0x1A838
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_26 0xE669A838u

//! Register Reset Value
#define DICC_IGP_26_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_26_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_26_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_26 Register DROP_IGP_26 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_26 0x1A83C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_26 0xE669A83Cu

//! Register Reset Value
#define DROP_IGP_26_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_26_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_26_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC0_DMA_IGP_26 Register DROP_DESC0_DMA_IGP_26 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DROP_DESC0_DMA_IGP_26 0x1A840
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC0_DMA_IGP_26 0xE669A840u

//! Register Reset Value
#define DROP_DESC0_DMA_IGP_26_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_26_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_26_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC1_DMA_IGP_26 Register DROP_DESC1_DMA_IGP_26 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DROP_DESC1_DMA_IGP_26 0x1A844
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC1_DMA_IGP_26 0xE669A844u

//! Register Reset Value
#define DROP_DESC1_DMA_IGP_26_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_26_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_26_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC2_DMA_IGP_26 Register DROP_DESC2_DMA_IGP_26 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DROP_DESC2_DMA_IGP_26 0x1A848
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC2_DMA_IGP_26 0xE669A848u

//! Register Reset Value
#define DROP_DESC2_DMA_IGP_26_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_26_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_26_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC3_DMA_IGP_26 Register DROP_DESC3_DMA_IGP_26 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DROP_DESC3_DMA_IGP_26 0x1A84C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC3_DMA_IGP_26 0xE669A84Cu

//! Register Reset Value
#define DROP_DESC3_DMA_IGP_26_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_26_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_26_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_26 Register DCNTR_IGP_26 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_26 0x1A860
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_26 0xE669A860u

//! Register Reset Value
#define DCNTR_IGP_26_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_26_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_26_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_26_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_26_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DMA_IGP_27 Register CFG_DMA_IGP_27 - DMA Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DMA_IGP_27 0x1AC00
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DMA_IGP_27 0xE669AC00u

//! Register Reset Value
#define CFG_DMA_IGP_27_RST 0x00000000u

//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_27_EQREQ_POS 0
//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_27_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_27_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_27_EQREQ_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_27_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_27_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_27_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_27_DISCHALT_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_27_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_27_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_27_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_27_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_27_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_27_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_27_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_27_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DMA_IGP_27_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DMA_IGP_27_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_27_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_27_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DMA_IGP_27_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DMA_IGP_27_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DMA_IGP_27_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DMA_IGP_27_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DMA_IGP_27_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DMA_IGP_27_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DMA_IGP_27_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DMA_IGP_27_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DMA_IGP_27_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DMA_IGP_27_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DMA_IGP_27_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DMA_IGP_27_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DMA_IGP_27_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DMA_IGP_27_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DMA_IGP_27_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DMA_IGP_27_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DMA_IGP_27_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DMA_IGP_27_DISC_REASON15 0xF

//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_27_DIC_DROP_POS 20
//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_27_DIC_DROP_MASK 0x100000u

//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_27_H_MODE_POS 21
//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_27_H_MODE_MASK 0x200000u

//! @}

//! \defgroup EQPC_DMA_IGP_27 Register EQPC_DMA_IGP_27 - DMA Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DMA_IGP_27 0x1AC0C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DMA_IGP_27 0xE669AC0Cu

//! Register Reset Value
#define EQPC_DMA_IGP_27_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_27_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_27_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DMA_IGP_27 Register DISC_DESC0_DMA_IGP_27 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DMA_IGP_27 0x1AC10
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DMA_IGP_27 0xE669AC10u

//! Register Reset Value
#define DISC_DESC0_DMA_IGP_27_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_27_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_27_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DMA_IGP_27 Register DISC_DESC1_DMA_IGP_27 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DMA_IGP_27 0x1AC14
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DMA_IGP_27 0xE669AC14u

//! Register Reset Value
#define DISC_DESC1_DMA_IGP_27_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_27_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_27_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DMA_IGP_27 Register DISC_DESC2_DMA_IGP_27 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DMA_IGP_27 0x1AC18
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DMA_IGP_27 0xE669AC18u

//! Register Reset Value
#define DISC_DESC2_DMA_IGP_27_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_27_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_27_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DMA_IGP_27 Register DISC_DESC3_DMA_IGP_27 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DMA_IGP_27 0x1AC1C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DMA_IGP_27 0xE669AC1Cu

//! Register Reset Value
#define DISC_DESC3_DMA_IGP_27_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_27_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_27_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DMA_IGP_27 Register IRNCR_DMA_IGP_27 - DMA Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DMA_IGP_27 0x1AC20
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DMA_IGP_27 0xE669AC20u

//! Register Reset Value
#define IRNCR_DMA_IGP_27_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_27_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_27_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_27_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DMA_IGP_27_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_27_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_27_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_27_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DMA_IGP_27_DR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DMA_IGP_27 Register IRNICR_DMA_IGP_27 - DMA Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DMA_IGP_27 0x1AC24
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DMA_IGP_27 0xE669AC24u

//! Register Reset Value
#define IRNICR_DMA_IGP_27_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_27_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_27_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_27_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_27_DR_MASK 0x8u

//! @}

//! \defgroup IRNEN_DMA_IGP_27 Register IRNEN_DMA_IGP_27 - DMA Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DMA_IGP_27 0x1AC28
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DMA_IGP_27 0xE669AC28u

//! Register Reset Value
#define IRNEN_DMA_IGP_27_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_27_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_27_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_27_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_27_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_27_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_27_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_27_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_27_DR_EN 0x1

//! @}

//! \defgroup DPTR_DMA_IGP_27 Register DPTR_DMA_IGP_27 - DMA Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DMA_IGP_27 0x1AC30
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DMA_IGP_27 0xE669AC30u

//! Register Reset Value
#define DPTR_DMA_IGP_27_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_27_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_27_ND_MASK 0x7u

//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_27_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_27_DPTR_MASK 0x70000u

//! @}

//! \defgroup DICC_IGP_27 Register DICC_IGP_27 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_27 0x1AC38
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_27 0xE669AC38u

//! Register Reset Value
#define DICC_IGP_27_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_27_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_27_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_27 Register DROP_IGP_27 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_27 0x1AC3C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_27 0xE669AC3Cu

//! Register Reset Value
#define DROP_IGP_27_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_27_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_27_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC0_DMA_IGP_27 Register DROP_DESC0_DMA_IGP_27 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DROP_DESC0_DMA_IGP_27 0x1AC40
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC0_DMA_IGP_27 0xE669AC40u

//! Register Reset Value
#define DROP_DESC0_DMA_IGP_27_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_27_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_27_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC1_DMA_IGP_27 Register DROP_DESC1_DMA_IGP_27 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DROP_DESC1_DMA_IGP_27 0x1AC44
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC1_DMA_IGP_27 0xE669AC44u

//! Register Reset Value
#define DROP_DESC1_DMA_IGP_27_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_27_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_27_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC2_DMA_IGP_27 Register DROP_DESC2_DMA_IGP_27 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DROP_DESC2_DMA_IGP_27 0x1AC48
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC2_DMA_IGP_27 0xE669AC48u

//! Register Reset Value
#define DROP_DESC2_DMA_IGP_27_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_27_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_27_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC3_DMA_IGP_27 Register DROP_DESC3_DMA_IGP_27 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DROP_DESC3_DMA_IGP_27 0x1AC4C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC3_DMA_IGP_27 0xE669AC4Cu

//! Register Reset Value
#define DROP_DESC3_DMA_IGP_27_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_27_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_27_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_27 Register DCNTR_IGP_27 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_27 0x1AC60
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_27 0xE669AC60u

//! Register Reset Value
#define DCNTR_IGP_27_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_27_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_27_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_27_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_27_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DMA_IGP_28 Register CFG_DMA_IGP_28 - DMA Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DMA_IGP_28 0x1B000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DMA_IGP_28 0xE669B000u

//! Register Reset Value
#define CFG_DMA_IGP_28_RST 0x00000000u

//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_28_EQREQ_POS 0
//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_28_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_28_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_28_EQREQ_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_28_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_28_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_28_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_28_DISCHALT_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_28_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_28_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_28_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_28_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_28_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_28_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_28_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_28_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DMA_IGP_28_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DMA_IGP_28_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_28_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_28_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DMA_IGP_28_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DMA_IGP_28_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DMA_IGP_28_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DMA_IGP_28_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DMA_IGP_28_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DMA_IGP_28_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DMA_IGP_28_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DMA_IGP_28_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DMA_IGP_28_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DMA_IGP_28_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DMA_IGP_28_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DMA_IGP_28_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DMA_IGP_28_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DMA_IGP_28_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DMA_IGP_28_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DMA_IGP_28_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DMA_IGP_28_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DMA_IGP_28_DISC_REASON15 0xF

//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_28_DIC_DROP_POS 20
//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_28_DIC_DROP_MASK 0x100000u

//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_28_H_MODE_POS 21
//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_28_H_MODE_MASK 0x200000u

//! @}

//! \defgroup EQPC_DMA_IGP_28 Register EQPC_DMA_IGP_28 - DMA Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DMA_IGP_28 0x1B00C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DMA_IGP_28 0xE669B00Cu

//! Register Reset Value
#define EQPC_DMA_IGP_28_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_28_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_28_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DMA_IGP_28 Register DISC_DESC0_DMA_IGP_28 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DMA_IGP_28 0x1B010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DMA_IGP_28 0xE669B010u

//! Register Reset Value
#define DISC_DESC0_DMA_IGP_28_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_28_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_28_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DMA_IGP_28 Register DISC_DESC1_DMA_IGP_28 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DMA_IGP_28 0x1B014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DMA_IGP_28 0xE669B014u

//! Register Reset Value
#define DISC_DESC1_DMA_IGP_28_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_28_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_28_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DMA_IGP_28 Register DISC_DESC2_DMA_IGP_28 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DMA_IGP_28 0x1B018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DMA_IGP_28 0xE669B018u

//! Register Reset Value
#define DISC_DESC2_DMA_IGP_28_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_28_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_28_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DMA_IGP_28 Register DISC_DESC3_DMA_IGP_28 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DMA_IGP_28 0x1B01C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DMA_IGP_28 0xE669B01Cu

//! Register Reset Value
#define DISC_DESC3_DMA_IGP_28_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_28_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_28_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DMA_IGP_28 Register IRNCR_DMA_IGP_28 - DMA Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DMA_IGP_28 0x1B020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DMA_IGP_28 0xE669B020u

//! Register Reset Value
#define IRNCR_DMA_IGP_28_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_28_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_28_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_28_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DMA_IGP_28_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_28_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_28_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_28_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DMA_IGP_28_DR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DMA_IGP_28 Register IRNICR_DMA_IGP_28 - DMA Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DMA_IGP_28 0x1B024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DMA_IGP_28 0xE669B024u

//! Register Reset Value
#define IRNICR_DMA_IGP_28_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_28_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_28_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_28_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_28_DR_MASK 0x8u

//! @}

//! \defgroup IRNEN_DMA_IGP_28 Register IRNEN_DMA_IGP_28 - DMA Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DMA_IGP_28 0x1B028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DMA_IGP_28 0xE669B028u

//! Register Reset Value
#define IRNEN_DMA_IGP_28_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_28_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_28_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_28_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_28_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_28_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_28_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_28_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_28_DR_EN 0x1

//! @}

//! \defgroup DPTR_DMA_IGP_28 Register DPTR_DMA_IGP_28 - DMA Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DMA_IGP_28 0x1B030
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DMA_IGP_28 0xE669B030u

//! Register Reset Value
#define DPTR_DMA_IGP_28_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_28_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_28_ND_MASK 0x7u

//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_28_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_28_DPTR_MASK 0x70000u

//! @}

//! \defgroup DICC_IGP_28 Register DICC_IGP_28 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_28 0x1B038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_28 0xE669B038u

//! Register Reset Value
#define DICC_IGP_28_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_28_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_28_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_28 Register DROP_IGP_28 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_28 0x1B03C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_28 0xE669B03Cu

//! Register Reset Value
#define DROP_IGP_28_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_28_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_28_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC0_DMA_IGP_28 Register DROP_DESC0_DMA_IGP_28 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DROP_DESC0_DMA_IGP_28 0x1B040
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC0_DMA_IGP_28 0xE669B040u

//! Register Reset Value
#define DROP_DESC0_DMA_IGP_28_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_28_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_28_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC1_DMA_IGP_28 Register DROP_DESC1_DMA_IGP_28 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DROP_DESC1_DMA_IGP_28 0x1B044
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC1_DMA_IGP_28 0xE669B044u

//! Register Reset Value
#define DROP_DESC1_DMA_IGP_28_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_28_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_28_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC2_DMA_IGP_28 Register DROP_DESC2_DMA_IGP_28 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DROP_DESC2_DMA_IGP_28 0x1B048
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC2_DMA_IGP_28 0xE669B048u

//! Register Reset Value
#define DROP_DESC2_DMA_IGP_28_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_28_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_28_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC3_DMA_IGP_28 Register DROP_DESC3_DMA_IGP_28 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DROP_DESC3_DMA_IGP_28 0x1B04C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC3_DMA_IGP_28 0xE669B04Cu

//! Register Reset Value
#define DROP_DESC3_DMA_IGP_28_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_28_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_28_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_28 Register DCNTR_IGP_28 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_28 0x1B060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_28 0xE669B060u

//! Register Reset Value
#define DCNTR_IGP_28_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_28_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_28_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_28_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_28_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DMA_IGP_29 Register CFG_DMA_IGP_29 - DMA Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DMA_IGP_29 0x1B400
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DMA_IGP_29 0xE669B400u

//! Register Reset Value
#define CFG_DMA_IGP_29_RST 0x00000000u

//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_29_EQREQ_POS 0
//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_29_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_29_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_29_EQREQ_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_29_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_29_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_29_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_29_DISCHALT_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_29_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_29_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_29_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_29_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_29_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_29_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_29_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_29_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DMA_IGP_29_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DMA_IGP_29_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_29_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_29_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DMA_IGP_29_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DMA_IGP_29_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DMA_IGP_29_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DMA_IGP_29_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DMA_IGP_29_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DMA_IGP_29_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DMA_IGP_29_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DMA_IGP_29_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DMA_IGP_29_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DMA_IGP_29_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DMA_IGP_29_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DMA_IGP_29_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DMA_IGP_29_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DMA_IGP_29_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DMA_IGP_29_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DMA_IGP_29_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DMA_IGP_29_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DMA_IGP_29_DISC_REASON15 0xF

//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_29_DIC_DROP_POS 20
//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_29_DIC_DROP_MASK 0x100000u

//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_29_H_MODE_POS 21
//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_29_H_MODE_MASK 0x200000u

//! @}

//! \defgroup EQPC_DMA_IGP_29 Register EQPC_DMA_IGP_29 - DMA Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DMA_IGP_29 0x1B40C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DMA_IGP_29 0xE669B40Cu

//! Register Reset Value
#define EQPC_DMA_IGP_29_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_29_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_29_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DMA_IGP_29 Register DISC_DESC0_DMA_IGP_29 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DMA_IGP_29 0x1B410
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DMA_IGP_29 0xE669B410u

//! Register Reset Value
#define DISC_DESC0_DMA_IGP_29_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_29_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_29_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DMA_IGP_29 Register DISC_DESC1_DMA_IGP_29 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DMA_IGP_29 0x1B414
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DMA_IGP_29 0xE669B414u

//! Register Reset Value
#define DISC_DESC1_DMA_IGP_29_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_29_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_29_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DMA_IGP_29 Register DISC_DESC2_DMA_IGP_29 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DMA_IGP_29 0x1B418
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DMA_IGP_29 0xE669B418u

//! Register Reset Value
#define DISC_DESC2_DMA_IGP_29_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_29_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_29_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DMA_IGP_29 Register DISC_DESC3_DMA_IGP_29 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DMA_IGP_29 0x1B41C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DMA_IGP_29 0xE669B41Cu

//! Register Reset Value
#define DISC_DESC3_DMA_IGP_29_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_29_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_29_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DMA_IGP_29 Register IRNCR_DMA_IGP_29 - DMA Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DMA_IGP_29 0x1B420
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DMA_IGP_29 0xE669B420u

//! Register Reset Value
#define IRNCR_DMA_IGP_29_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_29_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_29_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_29_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DMA_IGP_29_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_29_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_29_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_29_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DMA_IGP_29_DR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DMA_IGP_29 Register IRNICR_DMA_IGP_29 - DMA Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DMA_IGP_29 0x1B424
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DMA_IGP_29 0xE669B424u

//! Register Reset Value
#define IRNICR_DMA_IGP_29_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_29_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_29_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_29_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_29_DR_MASK 0x8u

//! @}

//! \defgroup IRNEN_DMA_IGP_29 Register IRNEN_DMA_IGP_29 - DMA Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DMA_IGP_29 0x1B428
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DMA_IGP_29 0xE669B428u

//! Register Reset Value
#define IRNEN_DMA_IGP_29_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_29_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_29_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_29_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_29_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_29_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_29_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_29_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_29_DR_EN 0x1

//! @}

//! \defgroup DPTR_DMA_IGP_29 Register DPTR_DMA_IGP_29 - DMA Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DMA_IGP_29 0x1B430
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DMA_IGP_29 0xE669B430u

//! Register Reset Value
#define DPTR_DMA_IGP_29_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_29_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_29_ND_MASK 0x7u

//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_29_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_29_DPTR_MASK 0x70000u

//! @}

//! \defgroup DICC_IGP_29 Register DICC_IGP_29 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_29 0x1B438
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_29 0xE669B438u

//! Register Reset Value
#define DICC_IGP_29_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_29_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_29_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_29 Register DROP_IGP_29 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_29 0x1B43C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_29 0xE669B43Cu

//! Register Reset Value
#define DROP_IGP_29_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_29_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_29_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC0_DMA_IGP_29 Register DROP_DESC0_DMA_IGP_29 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DROP_DESC0_DMA_IGP_29 0x1B440
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC0_DMA_IGP_29 0xE669B440u

//! Register Reset Value
#define DROP_DESC0_DMA_IGP_29_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_29_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_29_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC1_DMA_IGP_29 Register DROP_DESC1_DMA_IGP_29 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DROP_DESC1_DMA_IGP_29 0x1B444
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC1_DMA_IGP_29 0xE669B444u

//! Register Reset Value
#define DROP_DESC1_DMA_IGP_29_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_29_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_29_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC2_DMA_IGP_29 Register DROP_DESC2_DMA_IGP_29 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DROP_DESC2_DMA_IGP_29 0x1B448
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC2_DMA_IGP_29 0xE669B448u

//! Register Reset Value
#define DROP_DESC2_DMA_IGP_29_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_29_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_29_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC3_DMA_IGP_29 Register DROP_DESC3_DMA_IGP_29 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DROP_DESC3_DMA_IGP_29 0x1B44C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC3_DMA_IGP_29 0xE669B44Cu

//! Register Reset Value
#define DROP_DESC3_DMA_IGP_29_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_29_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_29_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_29 Register DCNTR_IGP_29 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_29 0x1B460
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_29 0xE669B460u

//! Register Reset Value
#define DCNTR_IGP_29_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_29_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_29_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_29_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_29_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DMA_IGP_30 Register CFG_DMA_IGP_30 - DMA Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DMA_IGP_30 0x1B800
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DMA_IGP_30 0xE669B800u

//! Register Reset Value
#define CFG_DMA_IGP_30_RST 0x00000000u

//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_30_EQREQ_POS 0
//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_30_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_30_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_30_EQREQ_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_30_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_30_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_30_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_30_DISCHALT_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_30_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_30_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_30_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_30_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_30_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_30_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_30_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_30_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DMA_IGP_30_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DMA_IGP_30_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_30_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_30_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DMA_IGP_30_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DMA_IGP_30_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DMA_IGP_30_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DMA_IGP_30_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DMA_IGP_30_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DMA_IGP_30_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DMA_IGP_30_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DMA_IGP_30_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DMA_IGP_30_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DMA_IGP_30_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DMA_IGP_30_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DMA_IGP_30_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DMA_IGP_30_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DMA_IGP_30_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DMA_IGP_30_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DMA_IGP_30_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DMA_IGP_30_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DMA_IGP_30_DISC_REASON15 0xF

//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_30_DIC_DROP_POS 20
//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_30_DIC_DROP_MASK 0x100000u

//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_30_H_MODE_POS 21
//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_30_H_MODE_MASK 0x200000u

//! @}

//! \defgroup EQPC_DMA_IGP_30 Register EQPC_DMA_IGP_30 - DMA Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DMA_IGP_30 0x1B80C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DMA_IGP_30 0xE669B80Cu

//! Register Reset Value
#define EQPC_DMA_IGP_30_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_30_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_30_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DMA_IGP_30 Register DISC_DESC0_DMA_IGP_30 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DMA_IGP_30 0x1B810
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DMA_IGP_30 0xE669B810u

//! Register Reset Value
#define DISC_DESC0_DMA_IGP_30_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_30_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_30_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DMA_IGP_30 Register DISC_DESC1_DMA_IGP_30 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DMA_IGP_30 0x1B814
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DMA_IGP_30 0xE669B814u

//! Register Reset Value
#define DISC_DESC1_DMA_IGP_30_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_30_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_30_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DMA_IGP_30 Register DISC_DESC2_DMA_IGP_30 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DMA_IGP_30 0x1B818
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DMA_IGP_30 0xE669B818u

//! Register Reset Value
#define DISC_DESC2_DMA_IGP_30_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_30_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_30_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DMA_IGP_30 Register DISC_DESC3_DMA_IGP_30 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DMA_IGP_30 0x1B81C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DMA_IGP_30 0xE669B81Cu

//! Register Reset Value
#define DISC_DESC3_DMA_IGP_30_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_30_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_30_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DMA_IGP_30 Register IRNCR_DMA_IGP_30 - DMA Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DMA_IGP_30 0x1B820
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DMA_IGP_30 0xE669B820u

//! Register Reset Value
#define IRNCR_DMA_IGP_30_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_30_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_30_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_30_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DMA_IGP_30_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_30_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_30_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_30_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DMA_IGP_30_DR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DMA_IGP_30 Register IRNICR_DMA_IGP_30 - DMA Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DMA_IGP_30 0x1B824
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DMA_IGP_30 0xE669B824u

//! Register Reset Value
#define IRNICR_DMA_IGP_30_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_30_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_30_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_30_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_30_DR_MASK 0x8u

//! @}

//! \defgroup IRNEN_DMA_IGP_30 Register IRNEN_DMA_IGP_30 - DMA Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DMA_IGP_30 0x1B828
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DMA_IGP_30 0xE669B828u

//! Register Reset Value
#define IRNEN_DMA_IGP_30_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_30_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_30_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_30_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_30_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_30_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_30_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_30_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_30_DR_EN 0x1

//! @}

//! \defgroup DPTR_DMA_IGP_30 Register DPTR_DMA_IGP_30 - DMA Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DMA_IGP_30 0x1B830
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DMA_IGP_30 0xE669B830u

//! Register Reset Value
#define DPTR_DMA_IGP_30_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_30_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_30_ND_MASK 0x7u

//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_30_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_30_DPTR_MASK 0x70000u

//! @}

//! \defgroup DICC_IGP_30 Register DICC_IGP_30 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_30 0x1B838
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_30 0xE669B838u

//! Register Reset Value
#define DICC_IGP_30_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_30_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_30_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_30 Register DROP_IGP_30 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_30 0x1B83C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_30 0xE669B83Cu

//! Register Reset Value
#define DROP_IGP_30_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_30_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_30_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC0_DMA_IGP_30 Register DROP_DESC0_DMA_IGP_30 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DROP_DESC0_DMA_IGP_30 0x1B840
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC0_DMA_IGP_30 0xE669B840u

//! Register Reset Value
#define DROP_DESC0_DMA_IGP_30_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_30_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_30_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC1_DMA_IGP_30 Register DROP_DESC1_DMA_IGP_30 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DROP_DESC1_DMA_IGP_30 0x1B844
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC1_DMA_IGP_30 0xE669B844u

//! Register Reset Value
#define DROP_DESC1_DMA_IGP_30_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_30_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_30_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC2_DMA_IGP_30 Register DROP_DESC2_DMA_IGP_30 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DROP_DESC2_DMA_IGP_30 0x1B848
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC2_DMA_IGP_30 0xE669B848u

//! Register Reset Value
#define DROP_DESC2_DMA_IGP_30_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_30_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_30_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC3_DMA_IGP_30 Register DROP_DESC3_DMA_IGP_30 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DROP_DESC3_DMA_IGP_30 0x1B84C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC3_DMA_IGP_30 0xE669B84Cu

//! Register Reset Value
#define DROP_DESC3_DMA_IGP_30_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_30_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_30_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_30 Register DCNTR_IGP_30 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_30 0x1B860
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_30 0xE669B860u

//! Register Reset Value
#define DCNTR_IGP_30_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_30_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_30_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_30_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_30_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DMA_IGP_31 Register CFG_DMA_IGP_31 - DMA Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DMA_IGP_31 0x1BC00
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DMA_IGP_31 0xE669BC00u

//! Register Reset Value
#define CFG_DMA_IGP_31_RST 0x00000000u

//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_31_EQREQ_POS 0
//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_31_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_31_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_31_EQREQ_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_31_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_31_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_31_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_31_DISCHALT_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_31_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_31_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_31_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_31_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_31_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_31_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_31_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_31_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DMA_IGP_31_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DMA_IGP_31_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_31_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_31_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DMA_IGP_31_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DMA_IGP_31_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DMA_IGP_31_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DMA_IGP_31_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DMA_IGP_31_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DMA_IGP_31_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DMA_IGP_31_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DMA_IGP_31_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DMA_IGP_31_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DMA_IGP_31_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DMA_IGP_31_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DMA_IGP_31_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DMA_IGP_31_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DMA_IGP_31_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DMA_IGP_31_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DMA_IGP_31_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DMA_IGP_31_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DMA_IGP_31_DISC_REASON15 0xF

//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_31_DIC_DROP_POS 20
//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_31_DIC_DROP_MASK 0x100000u

//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_31_H_MODE_POS 21
//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_31_H_MODE_MASK 0x200000u

//! @}

//! \defgroup EQPC_DMA_IGP_31 Register EQPC_DMA_IGP_31 - DMA Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DMA_IGP_31 0x1BC0C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DMA_IGP_31 0xE669BC0Cu

//! Register Reset Value
#define EQPC_DMA_IGP_31_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_31_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_31_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DMA_IGP_31 Register DISC_DESC0_DMA_IGP_31 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DMA_IGP_31 0x1BC10
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DMA_IGP_31 0xE669BC10u

//! Register Reset Value
#define DISC_DESC0_DMA_IGP_31_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_31_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_31_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DMA_IGP_31 Register DISC_DESC1_DMA_IGP_31 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DMA_IGP_31 0x1BC14
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DMA_IGP_31 0xE669BC14u

//! Register Reset Value
#define DISC_DESC1_DMA_IGP_31_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_31_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_31_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DMA_IGP_31 Register DISC_DESC2_DMA_IGP_31 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DMA_IGP_31 0x1BC18
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DMA_IGP_31 0xE669BC18u

//! Register Reset Value
#define DISC_DESC2_DMA_IGP_31_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_31_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_31_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DMA_IGP_31 Register DISC_DESC3_DMA_IGP_31 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DMA_IGP_31 0x1BC1C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DMA_IGP_31 0xE669BC1Cu

//! Register Reset Value
#define DISC_DESC3_DMA_IGP_31_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_31_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_31_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DMA_IGP_31 Register IRNCR_DMA_IGP_31 - DMA Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DMA_IGP_31 0x1BC20
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DMA_IGP_31 0xE669BC20u

//! Register Reset Value
#define IRNCR_DMA_IGP_31_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_31_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_31_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_31_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DMA_IGP_31_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_31_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_31_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_31_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DMA_IGP_31_DR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DMA_IGP_31 Register IRNICR_DMA_IGP_31 - DMA Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DMA_IGP_31 0x1BC24
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DMA_IGP_31 0xE669BC24u

//! Register Reset Value
#define IRNICR_DMA_IGP_31_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_31_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_31_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_31_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_31_DR_MASK 0x8u

//! @}

//! \defgroup IRNEN_DMA_IGP_31 Register IRNEN_DMA_IGP_31 - DMA Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DMA_IGP_31 0x1BC28
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DMA_IGP_31 0xE669BC28u

//! Register Reset Value
#define IRNEN_DMA_IGP_31_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_31_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_31_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_31_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_31_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_31_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_31_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_31_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_31_DR_EN 0x1

//! @}

//! \defgroup DPTR_DMA_IGP_31 Register DPTR_DMA_IGP_31 - DMA Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DMA_IGP_31 0x1BC30
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DMA_IGP_31 0xE669BC30u

//! Register Reset Value
#define DPTR_DMA_IGP_31_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_31_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_31_ND_MASK 0x7u

//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_31_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_31_DPTR_MASK 0x70000u

//! @}

//! \defgroup DICC_IGP_31 Register DICC_IGP_31 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_31 0x1BC38
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_31 0xE669BC38u

//! Register Reset Value
#define DICC_IGP_31_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_31_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_31_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_31 Register DROP_IGP_31 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_31 0x1BC3C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_31 0xE669BC3Cu

//! Register Reset Value
#define DROP_IGP_31_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_31_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_31_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC0_DMA_IGP_31 Register DROP_DESC0_DMA_IGP_31 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DROP_DESC0_DMA_IGP_31 0x1BC40
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC0_DMA_IGP_31 0xE669BC40u

//! Register Reset Value
#define DROP_DESC0_DMA_IGP_31_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_31_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_31_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC1_DMA_IGP_31 Register DROP_DESC1_DMA_IGP_31 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DROP_DESC1_DMA_IGP_31 0x1BC44
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC1_DMA_IGP_31 0xE669BC44u

//! Register Reset Value
#define DROP_DESC1_DMA_IGP_31_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_31_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_31_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC2_DMA_IGP_31 Register DROP_DESC2_DMA_IGP_31 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DROP_DESC2_DMA_IGP_31 0x1BC48
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC2_DMA_IGP_31 0xE669BC48u

//! Register Reset Value
#define DROP_DESC2_DMA_IGP_31_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_31_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_31_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC3_DMA_IGP_31 Register DROP_DESC3_DMA_IGP_31 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DROP_DESC3_DMA_IGP_31 0x1BC4C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC3_DMA_IGP_31 0xE669BC4Cu

//! Register Reset Value
#define DROP_DESC3_DMA_IGP_31_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_31_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_31_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_31 Register DCNTR_IGP_31 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_31 0x1BC60
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_31 0xE669BC60u

//! Register Reset Value
#define DCNTR_IGP_31_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_31_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_31_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_31_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_31_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_DMA_IGP_32 Register CFG_DMA_IGP_32 - DMA Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_DMA_IGP_32 0x1C000
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_DMA_IGP_32 0xE669C000u

//! Register Reset Value
#define CFG_DMA_IGP_32_RST 0x00000000u

//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_32_EQREQ_POS 0
//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_DMA_IGP_32_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_32_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_32_EQREQ_EN 0x1

//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_32_DISCHALT_POS 3
//! Field DISCHALT - Enable Discard Halt
#define CFG_DMA_IGP_32_DISCHALT_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_32_DISCHALT_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_32_DISCHALT_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_32_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_DMA_IGP_32_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_DMA_IGP_32_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_DMA_IGP_32_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_32_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_DMA_IGP_32_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_32_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_32_BP_EN_EN 0x1

//! Field DEN - Delay enable
#define CFG_DMA_IGP_32_DEN_POS 12
//! Field DEN - Delay enable
#define CFG_DMA_IGP_32_DEN_MASK 0x1000u
//! Constant DIS - Disable
#define CONST_CFG_DMA_IGP_32_DEN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_DMA_IGP_32_DEN_EN 0x1

//! Field DISC - Discard Reason
#define CFG_DMA_IGP_32_DISC_POS 16
//! Field DISC - Discard Reason
#define CFG_DMA_IGP_32_DISC_MASK 0xF0000u
//! Constant ACCEPTED - Accepted
#define CONST_CFG_DMA_IGP_32_DISC_ACCEPTED 0x0
//! Constant REASON1 - Reason 1
#define CONST_CFG_DMA_IGP_32_DISC_REASON1 0x1
//! Constant REASON2 - Reason 2
#define CONST_CFG_DMA_IGP_32_DISC_REASON2 0x2
//! Constant REASON3 - Reason 3
#define CONST_CFG_DMA_IGP_32_DISC_REASON3 0x3
//! Constant REASON4 - Reason 4
#define CONST_CFG_DMA_IGP_32_DISC_REASON4 0x4
//! Constant REASON5 - Reason 5
#define CONST_CFG_DMA_IGP_32_DISC_REASON5 0x5
//! Constant REASON6 - Reason 6
#define CONST_CFG_DMA_IGP_32_DISC_REASON6 0x6
//! Constant REASON7 - Reason 7
#define CONST_CFG_DMA_IGP_32_DISC_REASON7 0x7
//! Constant REASON8 - Reason 8
#define CONST_CFG_DMA_IGP_32_DISC_REASON8 0x8
//! Constant REASON9 - Reason 9
#define CONST_CFG_DMA_IGP_32_DISC_REASON9 0x9
//! Constant REASON10 - Reason 10
#define CONST_CFG_DMA_IGP_32_DISC_REASON10 0xA
//! Constant REASON11 - Reason 11
#define CONST_CFG_DMA_IGP_32_DISC_REASON11 0xB
//! Constant REASON12 - Reason 12
#define CONST_CFG_DMA_IGP_32_DISC_REASON12 0xC
//! Constant REASON13 - Reason 13
#define CONST_CFG_DMA_IGP_32_DISC_REASON13 0xD
//! Constant REASON14 - Reason 14
#define CONST_CFG_DMA_IGP_32_DISC_REASON14 0xE
//! Constant REASON15 - Reason 15
#define CONST_CFG_DMA_IGP_32_DISC_REASON15 0xF

//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_32_DIC_DROP_POS 20
//! Field DIC_DROP - Discard Bit Drop
#define CFG_DMA_IGP_32_DIC_DROP_MASK 0x100000u

//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_32_H_MODE_POS 21
//! Field H_MODE - H_Mode enabled
#define CFG_DMA_IGP_32_H_MODE_MASK 0x200000u

//! @}

//! \defgroup EQPC_DMA_IGP_32 Register EQPC_DMA_IGP_32 - DMA Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DMA_IGP_32 0x1C00C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DMA_IGP_32 0xE669C00Cu

//! Register Reset Value
#define EQPC_DMA_IGP_32_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_32_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_32_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC0_DMA_IGP_32 Register DISC_DESC0_DMA_IGP_32 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DISC_DESC0_DMA_IGP_32 0x1C010
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC0_DMA_IGP_32 0xE669C010u

//! Register Reset Value
#define DISC_DESC0_DMA_IGP_32_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_32_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DISC_DESC0_DMA_IGP_32_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC1_DMA_IGP_32 Register DISC_DESC1_DMA_IGP_32 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DISC_DESC1_DMA_IGP_32 0x1C014
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC1_DMA_IGP_32 0xE669C014u

//! Register Reset Value
#define DISC_DESC1_DMA_IGP_32_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_32_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DISC_DESC1_DMA_IGP_32_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC2_DMA_IGP_32 Register DISC_DESC2_DMA_IGP_32 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DISC_DESC2_DMA_IGP_32 0x1C018
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC2_DMA_IGP_32 0xE669C018u

//! Register Reset Value
#define DISC_DESC2_DMA_IGP_32_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_32_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DISC_DESC2_DMA_IGP_32_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DISC_DESC3_DMA_IGP_32 Register DISC_DESC3_DMA_IGP_32 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DISC_DESC3_DMA_IGP_32 0x1C01C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DISC_DESC3_DMA_IGP_32 0xE669C01Cu

//! Register Reset Value
#define DISC_DESC3_DMA_IGP_32_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_32_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DISC_DESC3_DMA_IGP_32_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_DMA_IGP_32 Register IRNCR_DMA_IGP_32 - DMA Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_DMA_IGP_32 0x1C020
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_DMA_IGP_32 0xE669C020u

//! Register Reset Value
#define IRNCR_DMA_IGP_32_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_32_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_DMA_IGP_32_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_32_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_DMA_IGP_32_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_32_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_DMA_IGP_32_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_DMA_IGP_32_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_DMA_IGP_32_DR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DMA_IGP_32 Register IRNICR_DMA_IGP_32 - DMA Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DMA_IGP_32 0x1C024
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DMA_IGP_32 0xE669C024u

//! Register Reset Value
#define IRNICR_DMA_IGP_32_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_32_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_32_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_32_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_32_DR_MASK 0x8u

//! @}

//! \defgroup IRNEN_DMA_IGP_32 Register IRNEN_DMA_IGP_32 - DMA Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DMA_IGP_32 0x1C028
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DMA_IGP_32 0xE669C028u

//! Register Reset Value
#define IRNEN_DMA_IGP_32_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_32_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_32_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_32_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_32_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_32_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_32_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_32_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_32_DR_EN 0x1

//! @}

//! \defgroup DPTR_DMA_IGP_32 Register DPTR_DMA_IGP_32 - DMA Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DMA_IGP_32 0x1C030
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DMA_IGP_32 0xE669C030u

//! Register Reset Value
#define DPTR_DMA_IGP_32_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_32_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_32_ND_MASK 0x7u

//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_32_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_32_DPTR_MASK 0x70000u

//! @}

//! \defgroup DICC_IGP_32 Register DICC_IGP_32 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_32 0x1C038
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_32 0xE669C038u

//! Register Reset Value
#define DICC_IGP_32_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_32_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_32_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_IGP_32 Register DROP_IGP_32 - Ingress Port Drop counter
//! @{

//! Register Offset (relative)
#define DROP_IGP_32 0x1C03C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_IGP_32 0xE669C03Cu

//! Register Reset Value
#define DROP_IGP_32_RST 0x00000000u

//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_32_DROPC_POS 0
//! Field DROPC - Per Port Drop Counter
#define DROP_IGP_32_DROPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC0_DMA_IGP_32 Register DROP_DESC0_DMA_IGP_32 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DROP_DESC0_DMA_IGP_32 0x1C040
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC0_DMA_IGP_32 0xE669C040u

//! Register Reset Value
#define DROP_DESC0_DMA_IGP_32_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_32_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_32_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC1_DMA_IGP_32 Register DROP_DESC1_DMA_IGP_32 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DROP_DESC1_DMA_IGP_32 0x1C044
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC1_DMA_IGP_32 0xE669C044u

//! Register Reset Value
#define DROP_DESC1_DMA_IGP_32_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_32_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_32_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC2_DMA_IGP_32 Register DROP_DESC2_DMA_IGP_32 - DMA Ingress Port Discard Descriptor 2
//! @{

//! Register Offset (relative)
#define DROP_DESC2_DMA_IGP_32 0x1C048
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC2_DMA_IGP_32 0xE669C048u

//! Register Reset Value
#define DROP_DESC2_DMA_IGP_32_RST 0x00000000u

//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_32_DESC_POS 0
//! Field DESC - Descriptor Double Word 2
#define DROP_DESC2_DMA_IGP_32_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC3_DMA_IGP_32 Register DROP_DESC3_DMA_IGP_32 - DMA Ingress Port Discard Descriptor 3
//! @{

//! Register Offset (relative)
#define DROP_DESC3_DMA_IGP_32 0x1C04C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC3_DMA_IGP_32 0xE669C04Cu

//! Register Reset Value
#define DROP_DESC3_DMA_IGP_32_RST 0x00000000u

//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_32_DESC_POS 0
//! Field DESC - Descriptor Double Word 3
#define DROP_DESC3_DMA_IGP_32_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DCNTR_IGP_32 Register DCNTR_IGP_32 - Ingress Port Delay Counter
//! @{

//! Register Offset (relative)
#define DCNTR_IGP_32 0x1C060
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DCNTR_IGP_32 0xE669C060u

//! Register Reset Value
#define DCNTR_IGP_32_RST 0x00000100u

//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_32_DLY_POS 0
//! Field DLY - Additional Delay before QoS enqueue
#define DCNTR_IGP_32_DLY_MASK 0x3Fu

//! Field DCNT - Counter Value
#define DCNTR_IGP_32_DCNT_POS 8
//! Field DCNT - Counter Value
#define DCNTR_IGP_32_DCNT_MASK 0x3F00u

//! @}

//! \defgroup CFG_RXDMA_IGP_33 Register CFG_RXDMA_IGP_33 - DMA Ingress Port Configuration
//! @{

//! Register Offset (relative)
#define CFG_RXDMA_IGP_33 0x1C400
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_CFG_RXDMA_IGP_33 0xE669C400u

//! Register Reset Value
#define CFG_RXDMA_IGP_33_RST 0x00000000u

//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_RXDMA_IGP_33_EQREQ_POS 0
//! Field EQREQ - Enable DMA Enqueue Request
#define CFG_RXDMA_IGP_33_EQREQ_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CFG_RXDMA_IGP_33_EQREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_RXDMA_IGP_33_EQREQ_EN 0x1

//! Field BUFREQ - Enable Buffer Request For Internal Packet Buffer Pointers
#define CFG_RXDMA_IGP_33_BUFREQ_POS 1
//! Field BUFREQ - Enable Buffer Request For Internal Packet Buffer Pointers
#define CFG_RXDMA_IGP_33_BUFREQ_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CFG_RXDMA_IGP_33_BUFREQ_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_RXDMA_IGP_33_BUFREQ_EN 0x1

//! Field EQPCEN - Enqueue Counter Enable
#define CFG_RXDMA_IGP_33_EQPCEN_POS 8
//! Field EQPCEN - Enqueue Counter Enable
#define CFG_RXDMA_IGP_33_EQPCEN_MASK 0x100u
//! Constant DIS - DIS
#define CONST_CFG_RXDMA_IGP_33_EQPCEN_DIS 0x0
//! Constant EN - EN
#define CONST_CFG_RXDMA_IGP_33_EQPCEN_EN 0x1

//! Field BP_EN - Backpressure enable
#define CFG_RXDMA_IGP_33_BP_EN_POS 10
//! Field BP_EN - Backpressure enable
#define CFG_RXDMA_IGP_33_BP_EN_MASK 0x400u
//! Constant DIS - Disable
#define CONST_CFG_RXDMA_IGP_33_BP_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CFG_RXDMA_IGP_33_BP_EN_EN 0x1

//! Field DIC_DROP - Discard Bit Drop
#define CFG_RXDMA_IGP_33_DIC_DROP_POS 20
//! Field DIC_DROP - Discard Bit Drop
#define CFG_RXDMA_IGP_33_DIC_DROP_MASK 0x100000u

//! @}

//! \defgroup EQPC_DMA_IGP_33 Register EQPC_DMA_IGP_33 - DMA Ingress Port enqueue packet counter
//! @{

//! Register Offset (relative)
#define EQPC_DMA_IGP_33 0x1C40C
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_EQPC_DMA_IGP_33 0xE669C40Cu

//! Register Reset Value
#define EQPC_DMA_IGP_33_RST 0x00000000u

//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_33_PC_POS 0
//! Field PC - Per Port Packet Counter
#define EQPC_DMA_IGP_33_PC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup NPBPC_DMA_IGP_33 Register NPBPC_DMA_IGP_33 - CPU Ingress Port new packet buffer pointer counter
//! @{

//! Register Offset (relative)
#define NPBPC_DMA_IGP_33 0x1C410
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_NPBPC_DMA_IGP_33 0xE669C410u

//! Register Reset Value
#define NPBPC_DMA_IGP_33_RST 0x00000000u

//! Field NPBPC - Per CPU New Packet Buffer Pointer Counter
#define NPBPC_DMA_IGP_33_NPBPC_POS 0
//! Field NPBPC - Per CPU New Packet Buffer Pointer Counter
#define NPBPC_DMA_IGP_33_NPBPC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup IRNCR_RXDMA_IGP_33 Register IRNCR_RXDMA_IGP_33 - DMA Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IRNCR_RXDMA_IGP_33 0x1C420
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNCR_RXDMA_IGP_33 0xE669C420u

//! Register Reset Value
#define IRNCR_RXDMA_IGP_33_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNCR_RXDMA_IGP_33_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNCR_RXDMA_IGP_33_PNA_MASK 0x1u
//! Constant NUL - NULL
#define CONST_IRNCR_RXDMA_IGP_33_PNA_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_IRNCR_RXDMA_IGP_33_PNA_INTACK 0x1

//! Field DR - Descriptor ready
#define IRNCR_RXDMA_IGP_33_DR_POS 3
//! Field DR - Descriptor ready
#define IRNCR_RXDMA_IGP_33_DR_MASK 0x8u
//! Constant NUL - NULL
#define CONST_IRNCR_RXDMA_IGP_33_DR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_RXDMA_IGP_33_DR_INTOCC 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_RXDMA_IGP_33_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNCR_RXDMA_IGP_33_PBR_MASK 0x10u
//! Constant NUL - NULL
#define CONST_IRNCR_RXDMA_IGP_33_PBR_NUL 0x0
//! Constant INTOCC - INTOCC
#define CONST_IRNCR_RXDMA_IGP_33_PBR_INTOCC 0x1

//! @}

//! \defgroup IRNICR_DMA_IGP_33 Register IRNICR_DMA_IGP_33 - DMA Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IRNICR_DMA_IGP_33 0x1C424
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNICR_DMA_IGP_33 0xE669C424u

//! Register Reset Value
#define IRNICR_DMA_IGP_33_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_33_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNICR_DMA_IGP_33_PNA_MASK 0x1u

//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_33_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNICR_DMA_IGP_33_DR_MASK 0x8u

//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_DMA_IGP_33_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNICR_DMA_IGP_33_PBR_MASK 0x10u

//! @}

//! \defgroup IRNEN_DMA_IGP_33 Register IRNEN_DMA_IGP_33 - DMA Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IRNEN_DMA_IGP_33 0x1C428
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_IRNEN_DMA_IGP_33 0xE669C428u

//! Register Reset Value
#define IRNEN_DMA_IGP_33_RST 0x00000000u

//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_33_PNA_POS 0
//! Field PNA - Packet Not Accepted
#define IRNEN_DMA_IGP_33_PNA_MASK 0x1u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_33_PNA_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_33_PNA_EN 0x1

//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_33_DR_POS 3
//! Field DR - Descriptor Ready
#define IRNEN_DMA_IGP_33_DR_MASK 0x8u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_33_DR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_33_DR_EN 0x1

//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_DMA_IGP_33_PBR_POS 4
//! Field PBR - Packet Buffer Pointer Ready
#define IRNEN_DMA_IGP_33_PBR_MASK 0x10u
//! Constant DIS - DIS
#define CONST_IRNEN_DMA_IGP_33_PBR_DIS 0x0
//! Constant EN - EN
#define CONST_IRNEN_DMA_IGP_33_PBR_EN 0x1

//! @}

//! \defgroup DPTR_DMA_IGP_33 Register DPTR_DMA_IGP_33 - DMA Ingress Port Descriptor Pointer
//! @{

//! Register Offset (relative)
#define DPTR_DMA_IGP_33 0x1C430
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DPTR_DMA_IGP_33 0xE669C430u

//! Register Reset Value
#define DPTR_DMA_IGP_33_RST 0x00000001u

//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_33_ND_POS 0
//! Field ND - Number of Descriptors
#define DPTR_DMA_IGP_33_ND_MASK 0xFu

//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_33_DPTR_POS 16
//! Field DPTR - Descriptor Pointer
#define DPTR_DMA_IGP_33_DPTR_MASK 0xF0000u

//! @}

//! \defgroup DICC_IGP_33 Register DICC_IGP_33 - Ingress Port Discard counter
//! @{

//! Register Offset (relative)
#define DICC_IGP_33 0x1C438
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DICC_IGP_33 0xE669C438u

//! Register Reset Value
#define DICC_IGP_33_RST 0x00000000u

//! Field DICC - Per Port Discard Counter
#define DICC_IGP_33_DICC_POS 0
//! Field DICC - Per Port Discard Counter
#define DICC_IGP_33_DICC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC0_DMA_IGP_33 Register DROP_DESC0_DMA_IGP_33 - DMA Ingress Port Discard Descriptor 0
//! @{

//! Register Offset (relative)
#define DROP_DESC0_DMA_IGP_33 0x1C440
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC0_DMA_IGP_33 0xE669C440u

//! Register Reset Value
#define DROP_DESC0_DMA_IGP_33_RST 0x00000000u

//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_33_DESC_POS 0
//! Field DESC - Descriptor Double Word 0
#define DROP_DESC0_DMA_IGP_33_DESC_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DROP_DESC1_DMA_IGP_33 Register DROP_DESC1_DMA_IGP_33 - DMA Ingress Port Discard Descriptor 1
//! @{

//! Register Offset (relative)
#define DROP_DESC1_DMA_IGP_33 0x1C444
//! Register Offset (absolute) for 1st Instance CQM_ENQ
#define CQM_ENQ_DROP_DESC1_DMA_IGP_33 0xE669C444u

//! Register Reset Value
#define DROP_DESC1_DMA_IGP_33_RST 0x00000000u

//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_33_DESC_POS 0
//! Field DESC - Descriptor Double Word 1
#define DROP_DESC1_DMA_IGP_33_DESC_MASK 0xFFFFFFFFu

//! @}

//! @}

#endif
