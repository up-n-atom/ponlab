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
// LSD Source          : /home/l1502/lgm_chip/v_leichuan.priv.v-dfv.lgm_chip.leichuan/ipg_lsd/lsd_sys/source/xml/reg_files/cbm_pon_ip_if.xml
// Register File Name  : CQEM_PON_IP_IF
// Register File Title : WAN Interface Block (WIB) Register Description
// Register Width      : 32
// Note                : Doxygen compliant comments
//-----------------------------------------------------------------------------

#ifndef _CQEM_PON_IP_IF_H
#define _CQEM_PON_IP_IF_H

//! \defgroup CQEM_PON_IP_IF Register File CQEM_PON_IP_IF - WAN Interface Block (WIB) Register Description
//! @{

//! Base Address of CQM_WIB
#define CQM_WIB_MODULE_BASE 0xE6400000u
//! Base Address of CQM_WIB
#define CQM_WIB_MODULE_BASE 0xE6400000u

//! \defgroup PIB_PON_IP_CMD Register PIB_PON_IP_CMD - PON IP Command Register
//! @{

//! Register Offset (relative)
#define PIB_PON_IP_CMD 0x100
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_PON_IP_CMD 0xE6400100u

//! Register Reset Value
#define PIB_PON_IP_CMD_RST 0x00000000u

//! Field NUM_BYTES - Number of Bytes to be dequeued from the PORT_ID
#define PIB_PON_IP_CMD_NUM_BYTES_POS 0
//! Field NUM_BYTES - Number of Bytes to be dequeued from the PORT_ID
#define PIB_PON_IP_CMD_NUM_BYTES_MASK 0xFFFFFFu

//! Field PORT_ID - PORT ID of the port from which the descriptor are to be dequeued
#define PIB_PON_IP_CMD_PORT_ID_POS 24
//! Field PORT_ID - PORT ID of the port from which the descriptor are to be dequeued
#define PIB_PON_IP_CMD_PORT_ID_MASK 0xFF000000u

//! @}

//! \defgroup PIB_OVERSHOOT_BYTES Register PIB_OVERSHOOT_BYTES - PIB overshoot bytes register
//! @{

//! Register Offset (relative)
#define PIB_OVERSHOOT_BYTES 0x108
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_OVERSHOOT_BYTES 0xE6400108u

//! Register Reset Value
#define PIB_OVERSHOOT_BYTES_RST 0x00000000u

//! Field OVERSHOOT - Overshoot Bytes
#define PIB_OVERSHOOT_BYTES_OVERSHOOT_POS 0
//! Field OVERSHOOT - Overshoot Bytes
#define PIB_OVERSHOOT_BYTES_OVERSHOOT_MASK 0xFFFFFFu

//! @}

//! \defgroup PIB_CTRL Register PIB_CTRL - PON IP IF Block Control Register
//! @{

//! Register Offset (relative)
#define PIB_CTRL 0x110
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_CTRL 0xE6400110u

//! Register Reset Value
#define PIB_CTRL_RST 0x06000000u

//! Field PIB_EN - Enable the PIB
#define PIB_CTRL_PIB_EN_POS 0
//! Field PIB_EN - Enable the PIB
#define PIB_CTRL_PIB_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_PIB_CTRL_PIB_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_PIB_CTRL_PIB_EN_EN 0x1

//! Field PIB_PAUSE - PON IP IF Block Pause enable
#define PIB_CTRL_PIB_PAUSE_POS 1
//! Field PIB_PAUSE - PON IP IF Block Pause enable
#define PIB_CTRL_PIB_PAUSE_MASK 0x2u
//! Constant RUN - Run
#define CONST_PIB_CTRL_PIB_PAUSE_RUN 0x0
//! Constant PAUSE - Pause
#define CONST_PIB_CTRL_PIB_PAUSE_PAUSE 0x1

//! Field DQRDPTRDIS - Disable the Auto Read pointer increment
#define PIB_CTRL_DQRDPTRDIS_POS 2
//! Field DQRDPTRDIS - Disable the Auto Read pointer increment
#define PIB_CTRL_DQRDPTRDIS_MASK 0x4u
//! Constant EN - EN
#define CONST_PIB_CTRL_DQRDPTRDIS_EN 0x0
//! Constant DIS - DIS
#define CONST_PIB_CTRL_DQRDPTRDIS_DIS 0x1

//! Field PIB_DC_MODE - PON IP IF Block Direct Connect Modr
#define PIB_CTRL_PIB_DC_MODE_POS 4
//! Field PIB_DC_MODE - PON IP IF Block Direct Connect Modr
#define PIB_CTRL_PIB_DC_MODE_MASK 0x10u
//! Constant DMA - PIB operate in DMA mode
#define CONST_PIB_CTRL_PIB_DC_MODE_DMA 0x0
//! Constant DC - PIB operate in DC mode
#define CONST_PIB_CTRL_PIB_DC_MODE_DC 0x1

//! Field OVRFLW_INT_EN - Enable PIB command FIFO overflow Interrupt
#define PIB_CTRL_OVRFLW_INT_EN_POS 8
//! Field OVRFLW_INT_EN - Enable PIB command FIFO overflow Interrupt
#define PIB_CTRL_OVRFLW_INT_EN_MASK 0x100u
//! Constant DIS - Disable Interrupt
#define CONST_PIB_CTRL_OVRFLW_INT_EN_DIS 0x0
//! Constant EN - Enable Interrupt
#define CONST_PIB_CTRL_OVRFLW_INT_EN_EN 0x1

//! Field ILLEGAL_PORT_INT_EN - Enable Illegal Port Command Interrupt
#define PIB_CTRL_ILLEGAL_PORT_INT_EN_POS 9
//! Field ILLEGAL_PORT_INT_EN - Enable Illegal Port Command Interrupt
#define PIB_CTRL_ILLEGAL_PORT_INT_EN_MASK 0x200u
//! Constant DIS - Disable Interrupt
#define CONST_PIB_CTRL_ILLEGAL_PORT_INT_EN_DIS 0x0
//! Constant EN - Enable Interrupt
#define CONST_PIB_CTRL_ILLEGAL_PORT_INT_EN_EN 0x1

//! Field WAKEUP_INT_EN - Enable Wakeup Interrupt
#define PIB_CTRL_WAKEUP_INT_EN_POS 10
//! Field WAKEUP_INT_EN - Enable Wakeup Interrupt
#define PIB_CTRL_WAKEUP_INT_EN_MASK 0x400u
//! Constant DIS - Disable Interrupt
#define CONST_PIB_CTRL_WAKEUP_INT_EN_DIS 0x0
//! Constant EN - Enable Interrupt
#define CONST_PIB_CTRL_WAKEUP_INT_EN_EN 0x1

//! Field PIB_BYPASS - Enable PIB Bypass Mode
#define PIB_CTRL_PIB_BYPASS_POS 16
//! Field PIB_BYPASS - Enable PIB Bypass Mode
#define PIB_CTRL_PIB_BYPASS_MASK 0x10000u
//! Constant DIS - Disable Bypass
#define CONST_PIB_CTRL_PIB_BYPASS_DIS 0x0
//! Constant EN - Enable Bypass
#define CONST_PIB_CTRL_PIB_BYPASS_EN 0x1

//! Field PKT_LEN_ADJ_EN - Enable Packet Length Adjustment
#define PIB_CTRL_PKT_LEN_ADJ_EN_POS 20
//! Field PKT_LEN_ADJ_EN - Enable Packet Length Adjustment
#define PIB_CTRL_PKT_LEN_ADJ_EN_MASK 0x100000
//! Constant DIS - Disable adj
#define CONST_PIB_CTRL_PKT_LEN_ADJ_DIS 0x0
//! Constant EN - Enable Adj
#define CONST_PIB_CTRL_PIB_PKT_LEN_ADJ_EN 0x1

//! Field DQ_DLY - Dequeue Delay
#define PIB_CTRL_DQ_DLY_POS 24
//! Field DQ_DLY - Dequeue Delay
#define PIB_CTRL_DQ_DLY_MASK 0xF000000u

//! Field PORTRDPTR - Port Internal Read pointer value
#define PIB_CTRL_PORTRDPTR_POS 28
//! Field PORTRDPTR - Port Internal Read pointer value
#define PIB_CTRL_PORTRDPTR_MASK 0xF0000000u

//! @}

//! \defgroup PIB_STATUS Register PIB_STATUS - PON IP Status Register
//! @{

//! Register Offset (relative)
#define PIB_STATUS 0x114
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_STATUS 0xE6400114u

//! Register Reset Value
#define PIB_STATUS_RST 0x00020000u

//! Field PIB_EN_STS - PIB Block is enabled
#define PIB_STATUS_PIB_EN_STS_POS 0
//! Field PIB_EN_STS - PIB Block is enabled
#define PIB_STATUS_PIB_EN_STS_MASK 0x1u

//! Field PIB_PAUSE_STS - PIB Block is paused
#define PIB_STATUS_PIB_PAUSE_STS_POS 1
//! Field PIB_PAUSE_STS - PIB Block is paused
#define PIB_STATUS_PIB_PAUSE_STS_MASK 0x2u

//! Field FIFO_SIZE - Current PON IP FIFO Size
#define PIB_STATUS_FIFO_SIZE_POS 7
//! Field FIFO_SIZE - Current PON IP FIFO Size
#define PIB_STATUS_FIFO_SIZE_MASK 0xFF80u

//! Field FIFO_FULL - FIFO Full
#define PIB_STATUS_FIFO_FULL_POS 16
//! Field FIFO_FULL - FIFO Full
#define PIB_STATUS_FIFO_FULL_MASK 0x10000u

//! Field FIFO_EMPTY - FIFO Empty
#define PIB_STATUS_FIFO_EMPTY_POS 17
//! Field FIFO_EMPTY - FIFO Empty
#define PIB_STATUS_FIFO_EMPTY_MASK 0x20000u

//! Field FIFO_OVFL - FIFO Overflow
#define PIB_STATUS_FIFO_OVFL_POS 24
//! Field FIFO_OVFL - FIFO Overflow
#define PIB_STATUS_FIFO_OVFL_MASK 0x1000000u
//! Constant NOR - Normal
#define CONST_PIB_STATUS_FIFO_OVFL_NOR 0x0
//! Constant OVFL - Overflow
#define CONST_PIB_STATUS_FIFO_OVFL_OVFL 0x1

//! Field ILLEGAL_PORT - Illegal port command
#define PIB_STATUS_ILLEGAL_PORT_POS 25
//! Field ILLEGAL_PORT - Illegal port command
#define PIB_STATUS_ILLEGAL_PORT_MASK 0x2000000u
//! Constant NOR - Normal
#define CONST_PIB_STATUS_ILLEGAL_PORT_NOR 0x0
//! Constant ILL - Illegal
#define CONST_PIB_STATUS_ILLEGAL_PORT_ILL 0x1

//! Field WAKEUP_INT - Wake Up Interrupt
#define PIB_STATUS_WAKEUP_INT_POS 26
//! Field WAKEUP_INT - Wake Up Interrupt
#define PIB_STATUS_WAKEUP_INT_MASK 0x4000000u
//! Constant NOR - Normal
#define CONST_PIB_STATUS_WAKEUP_INT_NOR 0x0
//! Constant ILL - Illegal
#define CONST_PIB_STATUS_WAKEUP_INT_ILL 0x1

//! @}

//! \defgroup PIB_FIFO_OVFL_CMD_REG Register PIB_FIFO_OVFL_CMD_REG - FIFO Overflow command register
//! @{

//! Register Offset (relative)
#define PIB_FIFO_OVFL_CMD_REG 0x118
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_FIFO_OVFL_CMD_REG 0xE6400118u

//! Register Reset Value
#define PIB_FIFO_OVFL_CMD_REG_RST 0x00000000u

//! Field OVFL_CMD - Overflow Command
#define PIB_FIFO_OVFL_CMD_REG_OVFL_CMD_POS 0
//! Field OVFL_CMD - Overflow Command
#define PIB_FIFO_OVFL_CMD_REG_OVFL_CMD_MASK 0xFFFFFFFFu

//! @}

//! \defgroup PIB_ILLEGAL_CMD_REG Register PIB_ILLEGAL_CMD_REG - PIB Illeagal Command Register
//! @{

//! Register Offset (relative)
#define PIB_ILLEGAL_CMD_REG 0x11C
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_ILLEGAL_CMD_REG 0xE640011Cu

//! Register Reset Value
#define PIB_ILLEGAL_CMD_REG_RST 0x00000000u

//! Field ILLEGAL_PORT_CMD - Illegal port command
#define PIB_ILLEGAL_CMD_REG_ILLEGAL_PORT_CMD_POS 0
//! Field ILLEGAL_PORT_CMD - Illegal port command
#define PIB_ILLEGAL_CMD_REG_ILLEGAL_PORT_CMD_MASK 0xFFFFFFFFu

//! @}

//! \defgroup PIB_PORT_EN_31_0 Register PIB_PORT_EN_31_0 - PON IP IF Port Enable Register Port 31-0
//! @{

//! Register Offset (relative)
#define PIB_PORT_EN_31_0 0x120
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_PORT_EN_31_0 0xE6400120u

//! Register Reset Value
#define PIB_PORT_EN_31_0_RST 0x00000000u

//! Field EN_PORT0 - PORT0 Enable
#define PIB_PORT_EN_31_0_EN_PORT0_POS 0
//! Field EN_PORT0 - PORT0 Enable
#define PIB_PORT_EN_31_0_EN_PORT0_MASK 0x1u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT0_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT0_EN 0x1

//! Field EN_PORT1 - PORT1 Enable
#define PIB_PORT_EN_31_0_EN_PORT1_POS 1
//! Field EN_PORT1 - PORT1 Enable
#define PIB_PORT_EN_31_0_EN_PORT1_MASK 0x2u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT1_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT1_EN 0x1

//! Field EN_PORT2 - PORT2 Enable
#define PIB_PORT_EN_31_0_EN_PORT2_POS 2
//! Field EN_PORT2 - PORT2 Enable
#define PIB_PORT_EN_31_0_EN_PORT2_MASK 0x4u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT2_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT2_EN 0x1

//! Field EN_PORT3 - PORT3 Enable
#define PIB_PORT_EN_31_0_EN_PORT3_POS 3
//! Field EN_PORT3 - PORT3 Enable
#define PIB_PORT_EN_31_0_EN_PORT3_MASK 0x8u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT3_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT3_EN 0x1

//! Field EN_PORT4 - PORT4 Enable
#define PIB_PORT_EN_31_0_EN_PORT4_POS 4
//! Field EN_PORT4 - PORT4 Enable
#define PIB_PORT_EN_31_0_EN_PORT4_MASK 0x10u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT4_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT4_EN 0x1

//! Field EN_PORT5 - PORT5 Enable
#define PIB_PORT_EN_31_0_EN_PORT5_POS 5
//! Field EN_PORT5 - PORT5 Enable
#define PIB_PORT_EN_31_0_EN_PORT5_MASK 0x20u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT5_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT5_EN 0x1

//! Field EN_PORT6 - PORT6 Enable
#define PIB_PORT_EN_31_0_EN_PORT6_POS 6
//! Field EN_PORT6 - PORT6 Enable
#define PIB_PORT_EN_31_0_EN_PORT6_MASK 0x40u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT6_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT6_EN 0x1

//! Field EN_PORT7 - PORT7 Enable
#define PIB_PORT_EN_31_0_EN_PORT7_POS 7
//! Field EN_PORT7 - PORT7 Enable
#define PIB_PORT_EN_31_0_EN_PORT7_MASK 0x80u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT7_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT7_EN 0x1

//! Field EN_PORT8 - PORT8 Enable
#define PIB_PORT_EN_31_0_EN_PORT8_POS 8
//! Field EN_PORT8 - PORT8 Enable
#define PIB_PORT_EN_31_0_EN_PORT8_MASK 0x100u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT8_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT8_EN 0x1

//! Field EN_PORT9 - PORT9 Enable
#define PIB_PORT_EN_31_0_EN_PORT9_POS 9
//! Field EN_PORT9 - PORT9 Enable
#define PIB_PORT_EN_31_0_EN_PORT9_MASK 0x200u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT9_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT9_EN 0x1

//! Field EN_PORT10 - PORT10 Enable
#define PIB_PORT_EN_31_0_EN_PORT10_POS 10
//! Field EN_PORT10 - PORT10 Enable
#define PIB_PORT_EN_31_0_EN_PORT10_MASK 0x400u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT10_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT10_EN 0x1

//! Field EN_PORT11 - PORT11 Enable
#define PIB_PORT_EN_31_0_EN_PORT11_POS 11
//! Field EN_PORT11 - PORT11 Enable
#define PIB_PORT_EN_31_0_EN_PORT11_MASK 0x800u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT11_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT11_EN 0x1

//! Field EN_PORT12 - PORT12 Enable
#define PIB_PORT_EN_31_0_EN_PORT12_POS 12
//! Field EN_PORT12 - PORT12 Enable
#define PIB_PORT_EN_31_0_EN_PORT12_MASK 0x1000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT12_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT12_EN 0x1

//! Field EN_PORT13 - PORT13 Enable
#define PIB_PORT_EN_31_0_EN_PORT13_POS 13
//! Field EN_PORT13 - PORT13 Enable
#define PIB_PORT_EN_31_0_EN_PORT13_MASK 0x2000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT13_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT13_EN 0x1

//! Field EN_PORT14 - PORT14 Enable
#define PIB_PORT_EN_31_0_EN_PORT14_POS 14
//! Field EN_PORT14 - PORT14 Enable
#define PIB_PORT_EN_31_0_EN_PORT14_MASK 0x4000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT14_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT14_EN 0x1

//! Field EN_PORT15 - PORT15 Enable
#define PIB_PORT_EN_31_0_EN_PORT15_POS 15
//! Field EN_PORT15 - PORT15 Enable
#define PIB_PORT_EN_31_0_EN_PORT15_MASK 0x8000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT15_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT15_EN 0x1

//! Field EN_PORT16 - PORT16 Enable
#define PIB_PORT_EN_31_0_EN_PORT16_POS 16
//! Field EN_PORT16 - PORT16 Enable
#define PIB_PORT_EN_31_0_EN_PORT16_MASK 0x10000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT16_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT16_EN 0x1

//! Field EN_PORT17 - PORT17 Enable
#define PIB_PORT_EN_31_0_EN_PORT17_POS 17
//! Field EN_PORT17 - PORT17 Enable
#define PIB_PORT_EN_31_0_EN_PORT17_MASK 0x20000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT17_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT17_EN 0x1

//! Field EN_PORT18 - PORT18 Enable
#define PIB_PORT_EN_31_0_EN_PORT18_POS 18
//! Field EN_PORT18 - PORT18 Enable
#define PIB_PORT_EN_31_0_EN_PORT18_MASK 0x40000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT18_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT18_EN 0x1

//! Field EN_PORT19 - PORT19 Enable
#define PIB_PORT_EN_31_0_EN_PORT19_POS 19
//! Field EN_PORT19 - PORT19 Enable
#define PIB_PORT_EN_31_0_EN_PORT19_MASK 0x80000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT19_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT19_EN 0x1

//! Field EN_PORT20 - PORT20 Enable
#define PIB_PORT_EN_31_0_EN_PORT20_POS 20
//! Field EN_PORT20 - PORT20 Enable
#define PIB_PORT_EN_31_0_EN_PORT20_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT20_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT20_EN 0x1

//! Field EN_PORT21 - PORT21 Enable
#define PIB_PORT_EN_31_0_EN_PORT21_POS 21
//! Field EN_PORT21 - PORT21 Enable
#define PIB_PORT_EN_31_0_EN_PORT21_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT21_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT21_EN 0x1

//! Field EN_PORT22 - PORT22 Enable
#define PIB_PORT_EN_31_0_EN_PORT22_POS 22
//! Field EN_PORT22 - PORT22 Enable
#define PIB_PORT_EN_31_0_EN_PORT22_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT22_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT22_EN 0x1

//! Field EN_PORT23 - PORT23 Enable
#define PIB_PORT_EN_31_0_EN_PORT23_POS 23
//! Field EN_PORT23 - PORT23 Enable
#define PIB_PORT_EN_31_0_EN_PORT23_MASK 0x800000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT23_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT23_EN 0x1

//! Field EN_PORT24 - PORT24 Enable
#define PIB_PORT_EN_31_0_EN_PORT24_POS 24
//! Field EN_PORT24 - PORT24 Enable
#define PIB_PORT_EN_31_0_EN_PORT24_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT24_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT24_EN 0x1

//! Field EN_PORT25 - PORT25 Enable
#define PIB_PORT_EN_31_0_EN_PORT25_POS 25
//! Field EN_PORT25 - PORT25 Enable
#define PIB_PORT_EN_31_0_EN_PORT25_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT25_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT25_EN 0x1

//! Field EN_PORT26 - PORT26 Enable
#define PIB_PORT_EN_31_0_EN_PORT26_POS 26
//! Field EN_PORT26 - PORT26 Enable
#define PIB_PORT_EN_31_0_EN_PORT26_MASK 0x4000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT26_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT26_EN 0x1

//! Field EN_PORT27 - PORT27 Enable
#define PIB_PORT_EN_31_0_EN_PORT27_POS 27
//! Field EN_PORT27 - PORT27 Enable
#define PIB_PORT_EN_31_0_EN_PORT27_MASK 0x8000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT27_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT27_EN 0x1

//! Field EN_PORT28 - PORT28 Enable
#define PIB_PORT_EN_31_0_EN_PORT28_POS 28
//! Field EN_PORT28 - PORT28 Enable
#define PIB_PORT_EN_31_0_EN_PORT28_MASK 0x10000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT28_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT28_EN 0x1

//! Field EN_PORT29 - PORT29 Enable
#define PIB_PORT_EN_31_0_EN_PORT29_POS 29
//! Field EN_PORT29 - PORT29 Enable
#define PIB_PORT_EN_31_0_EN_PORT29_MASK 0x20000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT29_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT29_EN 0x1

//! Field EN_PORT30 - PORT30 Enable
#define PIB_PORT_EN_31_0_EN_PORT30_POS 30
//! Field EN_PORT30 - PORT30 Enable
#define PIB_PORT_EN_31_0_EN_PORT30_MASK 0x40000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT30_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT30_EN 0x1

//! Field EN_PORT31 - PORT31 Enable
#define PIB_PORT_EN_31_0_EN_PORT31_POS 31
//! Field EN_PORT31 - PORT31 Enable
#define PIB_PORT_EN_31_0_EN_PORT31_MASK 0x80000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_31_0_EN_PORT31_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_31_0_EN_PORT31_EN 0x1

//! @}

//! \defgroup PIB_PORT_EN_64_32 Register PIB_PORT_EN_64_32 - PON IP IF Port Enable Register Port 63-32
//! @{

//! Register Offset (relative)
#define PIB_PORT_EN_64_32 0x124
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_PORT_EN_64_32 0xE6400124u

//! Register Reset Value
#define PIB_PORT_EN_64_32_RST 0x00000000u

//! Field EN_PORT0 - PORT0 Enable
#define PIB_PORT_EN_64_32_EN_PORT0_POS 0
//! Field EN_PORT0 - PORT0 Enable
#define PIB_PORT_EN_64_32_EN_PORT0_MASK 0x1u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT0_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT0_EN 0x1

//! Field EN_PORT1 - PORT1 Enable
#define PIB_PORT_EN_64_32_EN_PORT1_POS 1
//! Field EN_PORT1 - PORT1 Enable
#define PIB_PORT_EN_64_32_EN_PORT1_MASK 0x2u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT1_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT1_EN 0x1

//! Field EN_PORT2 - PORT2 Enable
#define PIB_PORT_EN_64_32_EN_PORT2_POS 2
//! Field EN_PORT2 - PORT2 Enable
#define PIB_PORT_EN_64_32_EN_PORT2_MASK 0x4u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT2_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT2_EN 0x1

//! Field EN_PORT3 - PORT3 Enable
#define PIB_PORT_EN_64_32_EN_PORT3_POS 3
//! Field EN_PORT3 - PORT3 Enable
#define PIB_PORT_EN_64_32_EN_PORT3_MASK 0x8u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT3_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT3_EN 0x1

//! Field EN_PORT4 - PORT4 Enable
#define PIB_PORT_EN_64_32_EN_PORT4_POS 4
//! Field EN_PORT4 - PORT4 Enable
#define PIB_PORT_EN_64_32_EN_PORT4_MASK 0x10u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT4_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT4_EN 0x1

//! Field EN_PORT5 - PORT5 Enable
#define PIB_PORT_EN_64_32_EN_PORT5_POS 5
//! Field EN_PORT5 - PORT5 Enable
#define PIB_PORT_EN_64_32_EN_PORT5_MASK 0x20u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT5_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT5_EN 0x1

//! Field EN_PORT6 - PORT6 Enable
#define PIB_PORT_EN_64_32_EN_PORT6_POS 6
//! Field EN_PORT6 - PORT6 Enable
#define PIB_PORT_EN_64_32_EN_PORT6_MASK 0x40u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT6_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT6_EN 0x1

//! Field EN_PORT7 - PORT7 Enable
#define PIB_PORT_EN_64_32_EN_PORT7_POS 7
//! Field EN_PORT7 - PORT7 Enable
#define PIB_PORT_EN_64_32_EN_PORT7_MASK 0x80u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT7_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT7_EN 0x1

//! Field EN_PORT8 - PORT8 Enable
#define PIB_PORT_EN_64_32_EN_PORT8_POS 8
//! Field EN_PORT8 - PORT8 Enable
#define PIB_PORT_EN_64_32_EN_PORT8_MASK 0x100u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT8_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT8_EN 0x1

//! Field EN_PORT9 - PORT9 Enable
#define PIB_PORT_EN_64_32_EN_PORT9_POS 9
//! Field EN_PORT9 - PORT9 Enable
#define PIB_PORT_EN_64_32_EN_PORT9_MASK 0x200u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT9_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT9_EN 0x1

//! Field EN_PORT10 - PORT10 Enable
#define PIB_PORT_EN_64_32_EN_PORT10_POS 10
//! Field EN_PORT10 - PORT10 Enable
#define PIB_PORT_EN_64_32_EN_PORT10_MASK 0x400u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT10_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT10_EN 0x1

//! Field EN_PORT11 - PORT11 Enable
#define PIB_PORT_EN_64_32_EN_PORT11_POS 11
//! Field EN_PORT11 - PORT11 Enable
#define PIB_PORT_EN_64_32_EN_PORT11_MASK 0x800u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT11_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT11_EN 0x1

//! Field EN_PORT12 - PORT12 Enable
#define PIB_PORT_EN_64_32_EN_PORT12_POS 12
//! Field EN_PORT12 - PORT12 Enable
#define PIB_PORT_EN_64_32_EN_PORT12_MASK 0x1000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT12_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT12_EN 0x1

//! Field EN_PORT13 - PORT13 Enable
#define PIB_PORT_EN_64_32_EN_PORT13_POS 13
//! Field EN_PORT13 - PORT13 Enable
#define PIB_PORT_EN_64_32_EN_PORT13_MASK 0x2000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT13_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT13_EN 0x1

//! Field EN_PORT14 - PORT14 Enable
#define PIB_PORT_EN_64_32_EN_PORT14_POS 14
//! Field EN_PORT14 - PORT14 Enable
#define PIB_PORT_EN_64_32_EN_PORT14_MASK 0x4000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT14_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT14_EN 0x1

//! Field EN_PORT15 - PORT15 Enable
#define PIB_PORT_EN_64_32_EN_PORT15_POS 15
//! Field EN_PORT15 - PORT15 Enable
#define PIB_PORT_EN_64_32_EN_PORT15_MASK 0x8000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT15_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT15_EN 0x1

//! Field EN_PORT16 - PORT16 Enable
#define PIB_PORT_EN_64_32_EN_PORT16_POS 16
//! Field EN_PORT16 - PORT16 Enable
#define PIB_PORT_EN_64_32_EN_PORT16_MASK 0x10000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT16_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT16_EN 0x1

//! Field EN_PORT17 - PORT17 Enable
#define PIB_PORT_EN_64_32_EN_PORT17_POS 17
//! Field EN_PORT17 - PORT17 Enable
#define PIB_PORT_EN_64_32_EN_PORT17_MASK 0x20000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT17_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT17_EN 0x1

//! Field EN_PORT18 - PORT18 Enable
#define PIB_PORT_EN_64_32_EN_PORT18_POS 18
//! Field EN_PORT18 - PORT18 Enable
#define PIB_PORT_EN_64_32_EN_PORT18_MASK 0x40000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT18_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT18_EN 0x1

//! Field EN_PORT19 - PORT19 Enable
#define PIB_PORT_EN_64_32_EN_PORT19_POS 19
//! Field EN_PORT19 - PORT19 Enable
#define PIB_PORT_EN_64_32_EN_PORT19_MASK 0x80000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT19_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT19_EN 0x1

//! Field EN_PORT20 - PORT20 Enable
#define PIB_PORT_EN_64_32_EN_PORT20_POS 20
//! Field EN_PORT20 - PORT20 Enable
#define PIB_PORT_EN_64_32_EN_PORT20_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT20_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT20_EN 0x1

//! Field EN_PORT21 - PORT21 Enable
#define PIB_PORT_EN_64_32_EN_PORT21_POS 21
//! Field EN_PORT21 - PORT21 Enable
#define PIB_PORT_EN_64_32_EN_PORT21_MASK 0x200000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT21_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT21_EN 0x1

//! Field EN_PORT22 - PORT22 Enable
#define PIB_PORT_EN_64_32_EN_PORT22_POS 22
//! Field EN_PORT22 - PORT22 Enable
#define PIB_PORT_EN_64_32_EN_PORT22_MASK 0x400000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT22_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT22_EN 0x1

//! Field EN_PORT23 - PORT23 Enable
#define PIB_PORT_EN_64_32_EN_PORT23_POS 23
//! Field EN_PORT23 - PORT23 Enable
#define PIB_PORT_EN_64_32_EN_PORT23_MASK 0x800000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT23_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT23_EN 0x1

//! Field EN_PORT24 - PORT24 Enable
#define PIB_PORT_EN_64_32_EN_PORT24_POS 24
//! Field EN_PORT24 - PORT24 Enable
#define PIB_PORT_EN_64_32_EN_PORT24_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT24_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT24_EN 0x1

//! Field EN_PORT25 - PORT25 Enable
#define PIB_PORT_EN_64_32_EN_PORT25_POS 25
//! Field EN_PORT25 - PORT25 Enable
#define PIB_PORT_EN_64_32_EN_PORT25_MASK 0x2000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT25_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT25_EN 0x1

//! Field EN_PORT26 - PORT26 Enable
#define PIB_PORT_EN_64_32_EN_PORT26_POS 26
//! Field EN_PORT26 - PORT26 Enable
#define PIB_PORT_EN_64_32_EN_PORT26_MASK 0x4000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT26_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT26_EN 0x1

//! Field EN_PORT27 - PORT27 Enable
#define PIB_PORT_EN_64_32_EN_PORT27_POS 27
//! Field EN_PORT27 - PORT27 Enable
#define PIB_PORT_EN_64_32_EN_PORT27_MASK 0x8000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT27_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT27_EN 0x1

//! Field EN_PORT28 - PORT28 Enable
#define PIB_PORT_EN_64_32_EN_PORT28_POS 28
//! Field EN_PORT28 - PORT28 Enable
#define PIB_PORT_EN_64_32_EN_PORT28_MASK 0x10000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT28_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT28_EN 0x1

//! Field EN_PORT29 - PORT29 Enable
#define PIB_PORT_EN_64_32_EN_PORT29_POS 29
//! Field EN_PORT29 - PORT29 Enable
#define PIB_PORT_EN_64_32_EN_PORT29_MASK 0x20000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT29_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT29_EN 0x1

//! Field EN_PORT30 - PORT30 Enable
#define PIB_PORT_EN_64_32_EN_PORT30_POS 30
//! Field EN_PORT30 - PORT30 Enable
#define PIB_PORT_EN_64_32_EN_PORT30_MASK 0x40000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT30_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT30_EN 0x1

//! Field EN_PORT31 - PORT31 Enable
#define PIB_PORT_EN_64_32_EN_PORT31_POS 31
//! Field EN_PORT31 - PORT31 Enable
#define PIB_PORT_EN_64_32_EN_PORT31_MASK 0x80000000u
//! Constant DIS - DIS
#define CONST_PIB_PORT_EN_64_32_EN_PORT31_DIS 0x0
//! Constant EN - EN
#define CONST_PIB_PORT_EN_64_32_EN_PORT31_EN 0x1

//! @}

//! \defgroup PORT_IRNCR_0 Register PORT_IRNCR_0 - PON Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define PORT_IRNCR_0 0x130
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PORT_IRNCR_0 0xE6400130u

//! Register Reset Value
#define PORT_IRNCR_0_RST 0x00000000u

//! Field PONPI - PON Port Interrupt
#define PORT_IRNCR_0_PONPI_POS 0
//! Field PONPI - PON Port Interrupt
#define PORT_IRNCR_0_PONPI_MASK 0xFFFFFFFFu

//! @}

//! \defgroup PORT_IRNICR_0 Register PORT_IRNICR_0 - PON Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define PORT_IRNICR_0 0x134
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PORT_IRNICR_0 0xE6400134u

//! Register Reset Value
#define PORT_IRNICR_0_RST 0x00000000u

//! Field PONPI - PON Port Interrupt
#define PORT_IRNICR_0_PONPI_POS 0
//! Field PONPI - PON Port Interrupt
#define PORT_IRNICR_0_PONPI_MASK 0xFFFFFFFFu

//! @}

//! \defgroup PORT_IRNEN_0 Register PORT_IRNEN_0 - PON Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define PORT_IRNEN_0 0x138
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PORT_IRNEN_0 0xE6400138u

//! Register Reset Value
#define PORT_IRNEN_0_RST 0x00000000u

//! Field PONPI - PON Port Interrupt
#define PORT_IRNEN_0_PONPI_POS 0
//! Field PONPI - PON Port Interrupt
#define PORT_IRNEN_0_PONPI_MASK 0xFFFFFFFFu

//! @}

//! \defgroup PORT_IRNCR_1 Register PORT_IRNCR_1 - PON Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define PORT_IRNCR_1 0x140
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PORT_IRNCR_1 0xE6400140u

//! Register Reset Value
#define PORT_IRNCR_1_RST 0x00000000u

//! Field PONPI - PON Port Interrupt
#define PORT_IRNCR_1_PONPI_POS 0
//! Field PONPI - PON Port Interrupt
#define PORT_IRNCR_1_PONPI_MASK 0xFFFFFFFFu

//! @}

//! \defgroup PORT_IRNICR_1 Register PORT_IRNICR_1 - PON Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define PORT_IRNICR_1 0x144
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PORT_IRNICR_1 0xE6400144u

//! Register Reset Value
#define PORT_IRNICR_1_RST 0x00000000u

//! Field PONPI - PON Port Interrupt
#define PORT_IRNICR_1_PONPI_POS 0
//! Field PONPI - PON Port Interrupt
#define PORT_IRNICR_1_PONPI_MASK 0xFFFFFFFFu

//! @}

//! \defgroup PORT_IRNEN_1 Register PORT_IRNEN_1 - PON Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define PORT_IRNEN_1 0x148
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PORT_IRNEN_1 0xE6400148u

//! Register Reset Value
#define PORT_IRNEN_1_RST 0x00000000u

//! Field PONPI - PON Port Interrupt
#define PORT_IRNEN_1_PONPI_POS 0
//! Field PONPI - PON Port Interrupt
#define PORT_IRNEN_1_PONPI_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DC_MODE_DESC_WR_BASE Register DC_MODE_DESC_WR_BASE - DC Mode Descriptor Write Base Address
//! @{

//! Register Offset (relative)
#define DC_MODE_DESC_WR_BASE 0x180
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_DC_MODE_DESC_WR_BASE 0xE6400180u

//! Register Reset Value
#define DC_MODE_DESC_WR_BASE_RST 0x00000000u

//! Field DC_BASE - DC Base Address
#define DC_MODE_DESC_WR_BASE_DC_BASE_POS 0
//! Field DC_BASE - DC Base Address
#define DC_MODE_DESC_WR_BASE_DC_BASE_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DC_MODE_TIMER_THRSHD Register DC_MODE_TIMER_THRSHD - DC Mode Descriptor Write Timer expiry threshold
//! @{

//! Register Offset (relative)
#define DC_MODE_TIMER_THRSHD 0x184
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_DC_MODE_TIMER_THRSHD 0xE6400184u

//! Register Reset Value
#define DC_MODE_TIMER_THRSHD_RST 0x00000000u

//! Field THRSD - Threshold
#define DC_MODE_TIMER_THRSHD_THRSD_POS 0
//! Field THRSD - Threshold
#define DC_MODE_TIMER_THRSHD_THRSD_MASK 0xFFFFFFFFu

//! @}

//! \defgroup DC_MODE_TIMER_CNT Register DC_MODE_TIMER_CNT - DC Mode Descriptor Write Timer Count
//! @{

//! Register Offset (relative)
#define DC_MODE_TIMER_CNT 0x188
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_DC_MODE_TIMER_CNT 0xE6400188u

//! Register Reset Value
#define DC_MODE_TIMER_CNT_RST 0x00000000u

//! Field CNT - Count
#define DC_MODE_TIMER_CNT_CNT_POS 0
//! Field CNT - Count
#define DC_MODE_TIMER_CNT_CNT_MASK 0xFFFFFFFFu

//! @}

//! \defgroup PIB_DOCSIS_CMD_0 Register PIB_DOCSIS_CMD_0 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_0 0x200
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_0 0xE6400200u

//! Register Reset Value
#define PIB_DOCSIS_CMD_0_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_0_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_0_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_0_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_0_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_0_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_0_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_0_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_0_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_1 Register PIB_DOCSIS_CMD_1 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_1 0x204
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_1 0xE6400204u

//! Register Reset Value
#define PIB_DOCSIS_CMD_1_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_1_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_1_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_1_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_1_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_1_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_1_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_1_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_1_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_2 Register PIB_DOCSIS_CMD_2 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_2 0x208
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_2 0xE6400208u

//! Register Reset Value
#define PIB_DOCSIS_CMD_2_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_2_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_2_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_2_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_2_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_2_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_2_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_2_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_2_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_3 Register PIB_DOCSIS_CMD_3 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_3 0x20C
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_3 0xE640020Cu

//! Register Reset Value
#define PIB_DOCSIS_CMD_3_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_3_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_3_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_3_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_3_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_3_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_3_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_3_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_3_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_4 Register PIB_DOCSIS_CMD_4 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_4 0x210
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_4 0xE6400210u

//! Register Reset Value
#define PIB_DOCSIS_CMD_4_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_4_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_4_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_4_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_4_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_4_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_4_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_4_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_4_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_5 Register PIB_DOCSIS_CMD_5 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_5 0x214
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_5 0xE6400214u

//! Register Reset Value
#define PIB_DOCSIS_CMD_5_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_5_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_5_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_5_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_5_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_5_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_5_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_5_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_5_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_6 Register PIB_DOCSIS_CMD_6 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_6 0x218
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_6 0xE6400218u

//! Register Reset Value
#define PIB_DOCSIS_CMD_6_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_6_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_6_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_6_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_6_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_6_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_6_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_6_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_6_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_7 Register PIB_DOCSIS_CMD_7 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_7 0x21C
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_7 0xE640021Cu

//! Register Reset Value
#define PIB_DOCSIS_CMD_7_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_7_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_7_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_7_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_7_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_7_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_7_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_7_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_7_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_8 Register PIB_DOCSIS_CMD_8 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_8 0x220
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_8 0xE6400220u

//! Register Reset Value
#define PIB_DOCSIS_CMD_8_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_8_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_8_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_8_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_8_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_8_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_8_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_8_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_8_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_9 Register PIB_DOCSIS_CMD_9 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_9 0x224
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_9 0xE6400224u

//! Register Reset Value
#define PIB_DOCSIS_CMD_9_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_9_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_9_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_9_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_9_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_9_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_9_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_9_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_9_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_10 Register PIB_DOCSIS_CMD_10 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_10 0x228
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_10 0xE6400228u

//! Register Reset Value
#define PIB_DOCSIS_CMD_10_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_10_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_10_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_10_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_10_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_10_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_10_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_10_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_10_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_11 Register PIB_DOCSIS_CMD_11 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_11 0x22C
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_11 0xE640022Cu

//! Register Reset Value
#define PIB_DOCSIS_CMD_11_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_11_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_11_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_11_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_11_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_11_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_11_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_11_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_11_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_12 Register PIB_DOCSIS_CMD_12 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_12 0x230
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_12 0xE6400230u

//! Register Reset Value
#define PIB_DOCSIS_CMD_12_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_12_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_12_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_12_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_12_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_12_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_12_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_12_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_12_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_13 Register PIB_DOCSIS_CMD_13 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_13 0x234
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_13 0xE6400234u

//! Register Reset Value
#define PIB_DOCSIS_CMD_13_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_13_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_13_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_13_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_13_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_13_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_13_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_13_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_13_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_14 Register PIB_DOCSIS_CMD_14 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_14 0x238
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_14 0xE6400238u

//! Register Reset Value
#define PIB_DOCSIS_CMD_14_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_14_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_14_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_14_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_14_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_14_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_14_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_14_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_14_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_15 Register PIB_DOCSIS_CMD_15 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_15 0x23C
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_15 0xE640023Cu

//! Register Reset Value
#define PIB_DOCSIS_CMD_15_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_15_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_15_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_15_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_15_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_15_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_15_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_15_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_15_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_16 Register PIB_DOCSIS_CMD_16 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_16 0x240
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_16 0xE6400240u

//! Register Reset Value
#define PIB_DOCSIS_CMD_16_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_16_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_16_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_16_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_16_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_16_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_16_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_16_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_16_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_17 Register PIB_DOCSIS_CMD_17 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_17 0x244
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_17 0xE6400244u

//! Register Reset Value
#define PIB_DOCSIS_CMD_17_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_17_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_17_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_17_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_17_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_17_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_17_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_17_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_17_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_18 Register PIB_DOCSIS_CMD_18 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_18 0x248
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_18 0xE6400248u

//! Register Reset Value
#define PIB_DOCSIS_CMD_18_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_18_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_18_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_18_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_18_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_18_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_18_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_18_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_18_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_19 Register PIB_DOCSIS_CMD_19 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_19 0x24C
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_19 0xE640024Cu

//! Register Reset Value
#define PIB_DOCSIS_CMD_19_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_19_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_19_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_19_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_19_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_19_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_19_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_19_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_19_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_20 Register PIB_DOCSIS_CMD_20 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_20 0x250
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_20 0xE6400250u

//! Register Reset Value
#define PIB_DOCSIS_CMD_20_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_20_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_20_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_20_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_20_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_20_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_20_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_20_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_20_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_21 Register PIB_DOCSIS_CMD_21 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_21 0x254
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_21 0xE6400254u

//! Register Reset Value
#define PIB_DOCSIS_CMD_21_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_21_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_21_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_21_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_21_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_21_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_21_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_21_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_21_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_22 Register PIB_DOCSIS_CMD_22 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_22 0x258
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_22 0xE6400258u

//! Register Reset Value
#define PIB_DOCSIS_CMD_22_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_22_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_22_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_22_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_22_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_22_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_22_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_22_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_22_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_23 Register PIB_DOCSIS_CMD_23 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_23 0x25C
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_23 0xE640025Cu

//! Register Reset Value
#define PIB_DOCSIS_CMD_23_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_23_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_23_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_23_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_23_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_23_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_23_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_23_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_23_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_24 Register PIB_DOCSIS_CMD_24 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_24 0x260
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_24 0xE6400260u

//! Register Reset Value
#define PIB_DOCSIS_CMD_24_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_24_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_24_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_24_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_24_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_24_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_24_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_24_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_24_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_25 Register PIB_DOCSIS_CMD_25 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_25 0x264
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_25 0xE6400264u

//! Register Reset Value
#define PIB_DOCSIS_CMD_25_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_25_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_25_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_25_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_25_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_25_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_25_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_25_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_25_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_26 Register PIB_DOCSIS_CMD_26 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_26 0x268
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_26 0xE6400268u

//! Register Reset Value
#define PIB_DOCSIS_CMD_26_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_26_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_26_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_26_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_26_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_26_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_26_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_26_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_26_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_27 Register PIB_DOCSIS_CMD_27 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_27 0x26C
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_27 0xE640026Cu

//! Register Reset Value
#define PIB_DOCSIS_CMD_27_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_27_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_27_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_27_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_27_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_27_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_27_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_27_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_27_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_28 Register PIB_DOCSIS_CMD_28 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_28 0x270
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_28 0xE6400270u

//! Register Reset Value
#define PIB_DOCSIS_CMD_28_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_28_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_28_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_28_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_28_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_28_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_28_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_28_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_28_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_29 Register PIB_DOCSIS_CMD_29 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_29 0x274
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_29 0xE6400274u

//! Register Reset Value
#define PIB_DOCSIS_CMD_29_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_29_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_29_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_29_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_29_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_29_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_29_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_29_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_29_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_30 Register PIB_DOCSIS_CMD_30 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_30 0x278
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_30 0xE6400278u

//! Register Reset Value
#define PIB_DOCSIS_CMD_30_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_30_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_30_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_30_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_30_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_30_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_30_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_30_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_30_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_31 Register PIB_DOCSIS_CMD_31 - PIB DOCSIS Command Register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_31 0x27C
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_31 0xE640027Cu

//! Register Reset Value
#define PIB_DOCSIS_CMD_31_RST 0x00000000u

//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_31_QID0_POS 0
//! Field QID0 - Queue ID0 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_31_QID0_MASK 0x3Fu

//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_31_ND0_POS 8
//! Field ND0 - Number of descriptor entries available in QID0
#define PIB_DOCSIS_CMD_31_ND0_MASK 0x1F00u

//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_31_QID1_POS 16
//! Field QID1 - Queue ID1 for which the empty entries are available in the DOCSIS
#define PIB_DOCSIS_CMD_31_QID1_MASK 0x3F0000u

//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_31_ND1_POS 24
//! Field ND1 - Number of descriptor entries available in QID1
#define PIB_DOCSIS_CMD_31_ND1_MASK 0x1F000000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_0 Register PIB_DOCSIS_CMD_CNT_0 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_0 0x280
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_0 0xE6400280u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_0_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_0_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_0_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_0_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_0_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_1 Register PIB_DOCSIS_CMD_CNT_1 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_1 0x284
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_1 0xE6400284u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_1_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_1_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_1_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_1_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_1_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_2 Register PIB_DOCSIS_CMD_CNT_2 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_2 0x288
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_2 0xE6400288u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_2_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_2_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_2_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_2_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_2_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_3 Register PIB_DOCSIS_CMD_CNT_3 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_3 0x28C
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_3 0xE640028Cu

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_3_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_3_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_3_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_3_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_3_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_4 Register PIB_DOCSIS_CMD_CNT_4 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_4 0x290
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_4 0xE6400290u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_4_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_4_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_4_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_4_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_4_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_5 Register PIB_DOCSIS_CMD_CNT_5 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_5 0x294
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_5 0xE6400294u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_5_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_5_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_5_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_5_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_5_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_6 Register PIB_DOCSIS_CMD_CNT_6 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_6 0x298
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_6 0xE6400298u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_6_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_6_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_6_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_6_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_6_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_7 Register PIB_DOCSIS_CMD_CNT_7 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_7 0x29C
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_7 0xE640029Cu

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_7_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_7_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_7_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_7_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_7_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_8 Register PIB_DOCSIS_CMD_CNT_8 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_8 0x2A0
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_8 0xE64002A0u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_8_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_8_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_8_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_8_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_8_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_9 Register PIB_DOCSIS_CMD_CNT_9 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_9 0x2A4
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_9 0xE64002A4u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_9_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_9_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_9_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_9_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_9_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_10 Register PIB_DOCSIS_CMD_CNT_10 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_10 0x2A8
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_10 0xE64002A8u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_10_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_10_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_10_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_10_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_10_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_11 Register PIB_DOCSIS_CMD_CNT_11 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_11 0x2AC
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_11 0xE64002ACu

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_11_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_11_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_11_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_11_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_11_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_12 Register PIB_DOCSIS_CMD_CNT_12 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_12 0x2B0
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_12 0xE64002B0u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_12_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_12_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_12_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_12_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_12_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_13 Register PIB_DOCSIS_CMD_CNT_13 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_13 0x2B4
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_13 0xE64002B4u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_13_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_13_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_13_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_13_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_13_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_14 Register PIB_DOCSIS_CMD_CNT_14 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_14 0x2B8
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_14 0xE64002B8u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_14_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_14_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_14_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_14_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_14_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_15 Register PIB_DOCSIS_CMD_CNT_15 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_15 0x2BC
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_15 0xE64002BCu

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_15_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_15_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_15_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_15_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_15_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_16 Register PIB_DOCSIS_CMD_CNT_16 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_16 0x2C0
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_16 0xE64002C0u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_16_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_16_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_16_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_16_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_16_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_17 Register PIB_DOCSIS_CMD_CNT_17 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_17 0x2C4
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_17 0xE64002C4u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_17_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_17_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_17_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_17_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_17_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_18 Register PIB_DOCSIS_CMD_CNT_18 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_18 0x2C8
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_18 0xE64002C8u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_18_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_18_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_18_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_18_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_18_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_19 Register PIB_DOCSIS_CMD_CNT_19 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_19 0x2CC
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_19 0xE64002CCu

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_19_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_19_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_19_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_19_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_19_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_20 Register PIB_DOCSIS_CMD_CNT_20 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_20 0x2D0
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_20 0xE64002D0u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_20_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_20_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_20_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_20_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_20_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_21 Register PIB_DOCSIS_CMD_CNT_21 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_21 0x2D4
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_21 0xE64002D4u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_21_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_21_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_21_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_21_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_21_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_22 Register PIB_DOCSIS_CMD_CNT_22 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_22 0x2D8
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_22 0xE64002D8u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_22_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_22_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_22_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_22_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_22_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_23 Register PIB_DOCSIS_CMD_CNT_23 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_23 0x2DC
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_23 0xE64002DCu

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_23_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_23_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_23_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_23_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_23_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_24 Register PIB_DOCSIS_CMD_CNT_24 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_24 0x2E0
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_24 0xE64002E0u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_24_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_24_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_24_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_24_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_24_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_25 Register PIB_DOCSIS_CMD_CNT_25 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_25 0x2E4
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_25 0xE64002E4u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_25_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_25_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_25_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_25_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_25_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_26 Register PIB_DOCSIS_CMD_CNT_26 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_26 0x2E8
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_26 0xE64002E8u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_26_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_26_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_26_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_26_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_26_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_27 Register PIB_DOCSIS_CMD_CNT_27 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_27 0x2EC
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_27 0xE64002ECu

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_27_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_27_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_27_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_27_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_27_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_28 Register PIB_DOCSIS_CMD_CNT_28 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_28 0x2F0
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_28 0xE64002F0u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_28_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_28_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_28_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_28_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_28_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_29 Register PIB_DOCSIS_CMD_CNT_29 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_29 0x2F4
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_29 0xE64002F4u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_29_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_29_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_29_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_29_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_29_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_30 Register PIB_DOCSIS_CMD_CNT_30 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_30 0x2F8
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_30 0xE64002F8u

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_30_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_30_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_30_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_30_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_30_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DOCSIS_CMD_CNT_31 Register PIB_DOCSIS_CMD_CNT_31 - PIB DOCSIS Command accumulative count register
//! @{

//! Register Offset (relative)
#define PIB_DOCSIS_CMD_CNT_31 0x2FC
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DOCSIS_CMD_CNT_31 0xE64002FCu

//! Register Reset Value
#define PIB_DOCSIS_CMD_CNT_31_RST 0x00000000u

//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_31_ND0_POS 0
//! Field ND0 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_31_ND0_MASK 0xFFu

//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_31_ND1_POS 16
//! Field ND1 - Number pending Descriptors
#define PIB_DOCSIS_CMD_CNT_31_ND1_MASK 0xFF0000u

//! @}

//! \defgroup PIB_DBG Register PIB_DBG - Hardware Debug Register
//! @{

//! Register Offset (relative)
#define PIB_DBG 0x920
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_DBG 0xE6400920u

//! Register Reset Value
#define PIB_DBG_RST 0x00000000u

//! Field DBG - Debug
#define PIB_DBG_DBG_POS 0
//! Field DBG - Debug
#define PIB_DBG_DBG_MASK 0xFFFFFFFFu

//! @}

//! \defgroup PIB_TEST Register PIB_TEST - Hardware Test Register
//! @{

//! Register Offset (relative)
#define PIB_TEST 0x930
//! Register Offset (absolute) for 1st Instance CQM_WIB
#define CQM_WIB_PIB_TEST 0xE6400930u

//! Register Reset Value
#define PIB_TEST_RST 0x00000000u

//! Field TEST - Test
#define PIB_TEST_TEST_POS 0
//! Field TEST - Test
#define PIB_TEST_TEST_MASK 0xFFFFFFFFu

//! @}

//! @}

#endif
