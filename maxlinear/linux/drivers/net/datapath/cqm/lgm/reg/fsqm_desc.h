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
// LSD Source          : /home/l1502/lgm_chip/v_leichuan.priv.v-dfv.lgm_chip.leichuan/ipg_lsd/lsd_sys/source/xml/reg_files/FSQM_Descriptor.xml
// Register File Name  : FSQM_DESC
// Register File Title : Descriptor Linking Register Description
// Register Width      : 32
// Note                : Doxygen compliant comments
//-----------------------------------------------------------------------------

#ifndef _FSQM_DESC_H
#define _FSQM_DESC_H

//! \defgroup FSQM_DESC Register File FSQM_DESC - Descriptor Linking Register Description
//! @{

//! Base Address of DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_MODULE_BASE 0xE6800000u
//! Base Address of DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_MODULE_BASE 0xE6800000u

//! \defgroup FSQM_DESC_CTRL Register FSQM_DESC_CTRL - Control Register
//! @{

//! Register Offset (relative)
#define FSQM_DESC_CTRL 0x0
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_FSQM_DESC_CTRL 0xE6800000u

//! Register Reset Value
#define FSQM_DESC_CTRL_RST 0x00000000u

//! Field ACT - Activate Core State Machine
#define FSQM_DESC_CTRL_ACT_POS 0
//! Field ACT - Activate Core State Machine
#define FSQM_DESC_CTRL_ACT_MASK 0x1u
//! Constant DIS - DIS
#define CONST_FSQM_DESC_CTRL_ACT_DIS 0x0
//! Constant EN - EN
#define CONST_FSQM_DESC_CTRL_ACT_EN 0x1

//! Field ALLOC_FRZ - Allocation Freeze Mechanism
#define FSQM_DESC_CTRL_ALLOC_FRZ_POS 1
//! Field ALLOC_FRZ - Allocation Freeze Mechanism
#define FSQM_DESC_CTRL_ALLOC_FRZ_MASK 0x2u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_ALLOC_FRZ_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_ALLOC_FRZ_SEL 0x1

//! Field ALLOC_DIS - Disable Allocation Mechanism
#define FSQM_DESC_CTRL_ALLOC_DIS_POS 2
//! Field ALLOC_DIS - Disable Allocation Mechanism
#define FSQM_DESC_CTRL_ALLOC_DIS_MASK 0x4u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_ALLOC_DIS_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_ALLOC_DIS_SEL 0x1

//! Field RST - Soft Reset of the FSQM Module
#define FSQM_DESC_CTRL_RST_POS 4
//! Field RST - Soft Reset of the FSQM Module
#define FSQM_DESC_CTRL_RST_MASK 0x10u
//! Constant DIS - Disable
#define CONST_FSQM_DESC_CTRL_RST_DIS 0x0
//! Constant EN - Enable
#define CONST_FSQM_DESC_CTRL_RST_EN 0x1

//! Field AD0 - Alloc Disable for master 0
#define FSQM_DESC_CTRL_AD0_POS 8
//! Field AD0 - Alloc Disable for master 0
#define FSQM_DESC_CTRL_AD0_MASK 0x100u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD0_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD0_SEL 0x1

//! Field AD1 - Alloc Disable for master 1
#define FSQM_DESC_CTRL_AD1_POS 9
//! Field AD1 - Alloc Disable for master 1
#define FSQM_DESC_CTRL_AD1_MASK 0x200u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD1_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD1_SEL 0x1

//! Field AD2 - Alloc Disable for master 2
#define FSQM_DESC_CTRL_AD2_POS 10
//! Field AD2 - Alloc Disable for master 2
#define FSQM_DESC_CTRL_AD2_MASK 0x400u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD2_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD2_SEL 0x1

//! Field AD3 - Alloc Disable for master 3
#define FSQM_DESC_CTRL_AD3_POS 11
//! Field AD3 - Alloc Disable for master 3
#define FSQM_DESC_CTRL_AD3_MASK 0x800u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD3_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD3_SEL 0x1

//! Field AD4 - Alloc Disable for master 4
#define FSQM_DESC_CTRL_AD4_POS 12
//! Field AD4 - Alloc Disable for master 4
#define FSQM_DESC_CTRL_AD4_MASK 0x1000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD4_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD4_SEL 0x1

//! Field AD5 - Alloc Disable for master 5
#define FSQM_DESC_CTRL_AD5_POS 13
//! Field AD5 - Alloc Disable for master 5
#define FSQM_DESC_CTRL_AD5_MASK 0x2000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD5_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD5_SEL 0x1

//! Field AD6 - Alloc Disable for master 6
#define FSQM_DESC_CTRL_AD6_POS 14
//! Field AD6 - Alloc Disable for master 6
#define FSQM_DESC_CTRL_AD6_MASK 0x4000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD6_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD6_SEL 0x1

//! Field AD7 - Alloc Disable for master 7
#define FSQM_DESC_CTRL_AD7_POS 15
//! Field AD7 - Alloc Disable for master 7
#define FSQM_DESC_CTRL_AD7_MASK 0x8000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD7_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD7_SEL 0x1

//! Field AD8 - Alloc Disable for master 8
#define FSQM_DESC_CTRL_AD8_POS 16
//! Field AD8 - Alloc Disable for master 8
#define FSQM_DESC_CTRL_AD8_MASK 0x10000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD8_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD8_SEL 0x1

//! Field AD9 - Alloc Disable for master 9
#define FSQM_DESC_CTRL_AD9_POS 17
//! Field AD9 - Alloc Disable for master 9
#define FSQM_DESC_CTRL_AD9_MASK 0x20000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD9_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD9_SEL 0x1

//! Field AD10 - Alloc Disable for master 10
#define FSQM_DESC_CTRL_AD10_POS 18
//! Field AD10 - Alloc Disable for master 10
#define FSQM_DESC_CTRL_AD10_MASK 0x40000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD10_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD10_SEL 0x1

//! Field AD11 - Alloc Disable for master 11
#define FSQM_DESC_CTRL_AD11_POS 19
//! Field AD11 - Alloc Disable for master 11
#define FSQM_DESC_CTRL_AD11_MASK 0x80000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD11_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD11_SEL 0x1

//! Field AD12 - Alloc Disable for master 12
#define FSQM_DESC_CTRL_AD12_POS 20
//! Field AD12 - Alloc Disable for master 12
#define FSQM_DESC_CTRL_AD12_MASK 0x100000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD12_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD12_SEL 0x1

//! Field AD13 - Alloc Disable for master 13
#define FSQM_DESC_CTRL_AD13_POS 21
//! Field AD13 - Alloc Disable for master 13
#define FSQM_DESC_CTRL_AD13_MASK 0x200000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD13_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD13_SEL 0x1

//! Field AD14 - Alloc Disable for master 14
#define FSQM_DESC_CTRL_AD14_POS 22
//! Field AD14 - Alloc Disable for master 14
#define FSQM_DESC_CTRL_AD14_MASK 0x400000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD14_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD14_SEL 0x1

//! Field AD15 - Alloc Disable for master 15
#define FSQM_DESC_CTRL_AD15_POS 23
//! Field AD15 - Alloc Disable for master 15
#define FSQM_DESC_CTRL_AD15_MASK 0x800000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD15_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD15_SEL 0x1

//! Field AD16 - Alloc Disable for master 16
#define FSQM_DESC_CTRL_AD16_POS 24
//! Field AD16 - Alloc Disable for master 16
#define FSQM_DESC_CTRL_AD16_MASK 0x1000000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD16_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD16_SEL 0x1

//! Field AD17 - Alloc Disable for master 17
#define FSQM_DESC_CTRL_AD17_POS 25
//! Field AD17 - Alloc Disable for master 17
#define FSQM_DESC_CTRL_AD17_MASK 0x2000000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD17_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD17_SEL 0x1

//! Field AD18 - Alloc Disable for master 18
#define FSQM_DESC_CTRL_AD18_POS 26
//! Field AD18 - Alloc Disable for master 18
#define FSQM_DESC_CTRL_AD18_MASK 0x4000000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD18_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD18_SEL 0x1

//! Field AD19 - Alloc Disable for master 19
#define FSQM_DESC_CTRL_AD19_POS 27
//! Field AD19 - Alloc Disable for master 19
#define FSQM_DESC_CTRL_AD19_MASK 0x8000000u
//! Constant NSEL - NSEL
#define CONST_FSQM_DESC_CTRL_AD19_NSEL 0x0
//! Constant SEL - SEL
#define CONST_FSQM_DESC_CTRL_AD19_SEL 0x1

//! @}

//! \defgroup DESC_IO_BUF_RD Register DESC_IO_BUF_RD - IO-Buffer Control Register for Read (ALLOC)
//! @{

//! Register Offset (relative)
#define DESC_IO_BUF_RD 0x8
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_IO_BUF_RD 0xE6800008u

//! Register Reset Value
#define DESC_IO_BUF_RD_RST 0x00000000u

//! Field BUF_EN - Enable/disable specific IO-Buffer(s)
#define DESC_IO_BUF_RD_BUF_EN_POS 0
//! Field BUF_EN - Enable/disable specific IO-Buffer(s)
#define DESC_IO_BUF_RD_BUF_EN_MASK 0xFFFFFu

//! @}

//! \defgroup DESC_IO_BUF_WR Register DESC_IO_BUF_WR - IO-Buffer Control Register for Write (FREE)
//! @{

//! Register Offset (relative)
#define DESC_IO_BUF_WR 0xC
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_IO_BUF_WR 0xE680000Cu

//! Register Reset Value
#define DESC_IO_BUF_WR_RST 0x00000000u

//! Field BUF_EN - Enable/disable specific IO-Buffer(s)
#define DESC_IO_BUF_WR_BUF_EN_POS 0
//! Field BUF_EN - Enable/disable specific IO-Buffer(s)
#define DESC_IO_BUF_WR_BUF_EN_MASK 0xFFFFFu

//! @}

//! \defgroup FSQM_DESC_IRNCR Register FSQM_DESC_IRNCR - IRN Capture Register
//! @{

//! Register Offset (relative)
#define FSQM_DESC_IRNCR 0x10
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_FSQM_DESC_IRNCR 0xE6800010u

//! Register Reset Value
#define FSQM_DESC_IRNCR_RST 0x00000000u

//! Field T0U - Threshold 0 Underflow
#define FSQM_DESC_IRNCR_T0U_POS 0
//! Field T0U - Threshold 0 Underflow
#define FSQM_DESC_IRNCR_T0U_MASK 0x1u
//! Constant NUL - NULL
#define CONST_FSQM_DESC_IRNCR_T0U_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_FSQM_DESC_IRNCR_T0U_INTACK 0x1

//! Field T1U - Threshold 1 Underflow
#define FSQM_DESC_IRNCR_T1U_POS 1
//! Field T1U - Threshold 1 Underflow
#define FSQM_DESC_IRNCR_T1U_MASK 0x2u
//! Constant NUL - NULL
#define CONST_FSQM_DESC_IRNCR_T1U_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_FSQM_DESC_IRNCR_T1U_INTACK 0x1

//! Field T2U - Threshold 2 Underflow
#define FSQM_DESC_IRNCR_T2U_POS 2
//! Field T2U - Threshold 2 Underflow
#define FSQM_DESC_IRNCR_T2U_MASK 0x4u
//! Constant NUL - NULL
#define CONST_FSQM_DESC_IRNCR_T2U_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_FSQM_DESC_IRNCR_T2U_INTACK 0x1

//! Field T3U - Threshold 3 Underflow
#define FSQM_DESC_IRNCR_T3U_POS 3
//! Field T3U - Threshold 3 Underflow
#define FSQM_DESC_IRNCR_T3U_MASK 0x8u
//! Constant NUL - NULL
#define CONST_FSQM_DESC_IRNCR_T3U_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_FSQM_DESC_IRNCR_T3U_INTACK 0x1

//! Field T4U - Threshold 4 Underflow
#define FSQM_DESC_IRNCR_T4U_POS 4
//! Field T4U - Threshold 4 Underflow
#define FSQM_DESC_IRNCR_T4U_MASK 0x10u
//! Constant NUL - NULL
#define CONST_FSQM_DESC_IRNCR_T4U_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_FSQM_DESC_IRNCR_T4U_INTACK 0x1

//! Field ACCMEM - RAM Access Violation
#define FSQM_DESC_IRNCR_ACCMEM_POS 12
//! Field ACCMEM - RAM Access Violation
#define FSQM_DESC_IRNCR_ACCMEM_MASK 0x1000u
//! Constant NUL - NULL
#define CONST_FSQM_DESC_IRNCR_ACCMEM_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_FSQM_DESC_IRNCR_ACCMEM_INTACK 0x1

//! Field FRMEM - Free Command RAM Access Violation
#define FSQM_DESC_IRNCR_FRMEM_POS 16
//! Field FRMEM - Free Command RAM Access Violation
#define FSQM_DESC_IRNCR_FRMEM_MASK 0x10000u
//! Constant NUL - NULL
#define CONST_FSQM_DESC_IRNCR_FRMEM_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_FSQM_DESC_IRNCR_FRMEM_INTACK 0x1

//! Field FROVLO - Free Command Overflow on OFSQ
#define FSQM_DESC_IRNCR_FROVLO_POS 20
//! Field FROVLO - Free Command Overflow on OFSQ
#define FSQM_DESC_IRNCR_FROVLO_MASK 0x100000u
//! Constant NUL - NULL
#define CONST_FSQM_DESC_IRNCR_FROVLO_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_FSQM_DESC_IRNCR_FROVLO_INTACK 0x1

//! Field ALLNIL - Alloc Command NIL Response
#define FSQM_DESC_IRNCR_ALLNIL_POS 24
//! Field ALLNIL - Alloc Command NIL Response
#define FSQM_DESC_IRNCR_ALLNIL_MASK 0x1000000u
//! Constant NUL - NULL
#define CONST_FSQM_DESC_IRNCR_ALLNIL_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_FSQM_DESC_IRNCR_ALLNIL_INTACK 0x1

//! @}

//! \defgroup FSQM_DESC_IRNICR Register FSQM_DESC_IRNICR - IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define FSQM_DESC_IRNICR 0x14
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_FSQM_DESC_IRNICR 0xE6800014u

//! Register Reset Value
#define FSQM_DESC_IRNICR_RST 0x00000000u

//! Field T0U - Threshold 0 Underflow
#define FSQM_DESC_IRNICR_T0U_POS 0
//! Field T0U - Threshold 0 Underflow
#define FSQM_DESC_IRNICR_T0U_MASK 0x1u

//! Field T1U - Threshold 1 Underflow
#define FSQM_DESC_IRNICR_T1U_POS 1
//! Field T1U - Threshold 1 Underflow
#define FSQM_DESC_IRNICR_T1U_MASK 0x2u

//! Field T2U - Threshold 2 Underflow
#define FSQM_DESC_IRNICR_T2U_POS 2
//! Field T2U - Threshold 2 Underflow
#define FSQM_DESC_IRNICR_T2U_MASK 0x4u

//! Field T3U - Threshold 3 Underflow
#define FSQM_DESC_IRNICR_T3U_POS 3
//! Field T3U - Threshold 3 Underflow
#define FSQM_DESC_IRNICR_T3U_MASK 0x8u

//! Field T4U - Threshold 4 Underflow
#define FSQM_DESC_IRNICR_T4U_POS 4
//! Field T4U - Threshold 4 Underflow
#define FSQM_DESC_IRNICR_T4U_MASK 0x10u

//! Field ACCMEM - RAM Access Violation
#define FSQM_DESC_IRNICR_ACCMEM_POS 12
//! Field ACCMEM - RAM Access Violation
#define FSQM_DESC_IRNICR_ACCMEM_MASK 0x1000u

//! Field FRMEM - Free Command RAM Access Violation
#define FSQM_DESC_IRNICR_FRMEM_POS 16
//! Field FRMEM - Free Command RAM Access Violation
#define FSQM_DESC_IRNICR_FRMEM_MASK 0x10000u

//! Field FROVLO - Free Command Overflow on OFSQ
#define FSQM_DESC_IRNICR_FROVLO_POS 20
//! Field FROVLO - Free Command Overflow on OFSQ
#define FSQM_DESC_IRNICR_FROVLO_MASK 0x100000u

//! Field ALLNIL - Alloc Command NIL Response
#define FSQM_DESC_IRNICR_ALLNIL_POS 24
//! Field ALLNIL - Alloc Command NIL Response
#define FSQM_DESC_IRNICR_ALLNIL_MASK 0x1000000u

//! @}

//! \defgroup FSQM_DESC_IRNEN Register FSQM_DESC_IRNEN - IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define FSQM_DESC_IRNEN 0x18
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_FSQM_DESC_IRNEN 0xE6800018u

//! Register Reset Value
#define FSQM_DESC_IRNEN_RST 0x00000000u

//! Field T0U - Threshold 0 Underflow
#define FSQM_DESC_IRNEN_T0U_POS 0
//! Field T0U - Threshold 0 Underflow
#define FSQM_DESC_IRNEN_T0U_MASK 0x1u
//! Constant DIS - DIS
#define CONST_FSQM_DESC_IRNEN_T0U_DIS 0x0
//! Constant EN - EN
#define CONST_FSQM_DESC_IRNEN_T0U_EN 0x1

//! Field T1U - Threshold 1 Underflow
#define FSQM_DESC_IRNEN_T1U_POS 1
//! Field T1U - Threshold 1 Underflow
#define FSQM_DESC_IRNEN_T1U_MASK 0x2u
//! Constant DIS - DIS
#define CONST_FSQM_DESC_IRNEN_T1U_DIS 0x0
//! Constant EN - EN
#define CONST_FSQM_DESC_IRNEN_T1U_EN 0x1

//! Field T2U - Threshold 2 Underflow
#define FSQM_DESC_IRNEN_T2U_POS 2
//! Field T2U - Threshold 2 Underflow
#define FSQM_DESC_IRNEN_T2U_MASK 0x4u
//! Constant DIS - DIS
#define CONST_FSQM_DESC_IRNEN_T2U_DIS 0x0
//! Constant EN - EN
#define CONST_FSQM_DESC_IRNEN_T2U_EN 0x1

//! Field T3U - Threshold 3 Underflow
#define FSQM_DESC_IRNEN_T3U_POS 3
//! Field T3U - Threshold 3 Underflow
#define FSQM_DESC_IRNEN_T3U_MASK 0x8u
//! Constant DIS - DIS
#define CONST_FSQM_DESC_IRNEN_T3U_DIS 0x0
//! Constant EN - EN
#define CONST_FSQM_DESC_IRNEN_T3U_EN 0x1

//! Field T4U - Threshold 4 Underflow
#define FSQM_DESC_IRNEN_T4U_POS 4
//! Field T4U - Threshold 4 Underflow
#define FSQM_DESC_IRNEN_T4U_MASK 0x10u
//! Constant DIS - DIS
#define CONST_FSQM_DESC_IRNEN_T4U_DIS 0x0
//! Constant EN - EN
#define CONST_FSQM_DESC_IRNEN_T4U_EN 0x1

//! Field ACCMEM - RAM Access Violation
#define FSQM_DESC_IRNEN_ACCMEM_POS 12
//! Field ACCMEM - RAM Access Violation
#define FSQM_DESC_IRNEN_ACCMEM_MASK 0x1000u
//! Constant DIS - DIS
#define CONST_FSQM_DESC_IRNEN_ACCMEM_DIS 0x0
//! Constant EN - EN
#define CONST_FSQM_DESC_IRNEN_ACCMEM_EN 0x1

//! Field FRMEM - Free Command RAM Access Violation
#define FSQM_DESC_IRNEN_FRMEM_POS 16
//! Field FRMEM - Free Command RAM Access Violation
#define FSQM_DESC_IRNEN_FRMEM_MASK 0x10000u
//! Constant DIS - DIS
#define CONST_FSQM_DESC_IRNEN_FRMEM_DIS 0x0
//! Constant EN - EN
#define CONST_FSQM_DESC_IRNEN_FRMEM_EN 0x1

//! Field FROVLO - Free Command Overflow on OFSQ
#define FSQM_DESC_IRNEN_FROVLO_POS 20
//! Field FROVLO - Free Command Overflow on OFSQ
#define FSQM_DESC_IRNEN_FROVLO_MASK 0x100000u
//! Constant DIS - DIS
#define CONST_FSQM_DESC_IRNEN_FROVLO_DIS 0x0
//! Constant EN - EN
#define CONST_FSQM_DESC_IRNEN_FROVLO_EN 0x1

//! Field ALLNIL - Alloc Command NIL Response
#define FSQM_DESC_IRNEN_ALLNIL_POS 24
//! Field ALLNIL - Alloc Command NIL Response
#define FSQM_DESC_IRNEN_ALLNIL_MASK 0x1000000u
//! Constant DIS - DIS
#define CONST_FSQM_DESC_IRNEN_ALLNIL_DIS 0x0
//! Constant EN - EN
#define CONST_FSQM_DESC_IRNEN_ALLNIL_EN 0x1

//! @}

//! \defgroup DESC_TAILCAPR Register DESC_TAILCAPR - TAIL LSA Capture Register
//! @{

//! Register Offset (relative)
#define DESC_TAILCAPR 0x20
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_TAILCAPR 0xE6800020u

//! Register Reset Value
#define DESC_TAILCAPR_RST 0x00000000u

//! Field TAIL_FRZ - Logical Segment Address of captured OFSQ TAIL
#define DESC_TAILCAPR_TAIL_FRZ_POS 0
//! Field TAIL_FRZ - Logical Segment Address of captured OFSQ TAIL
#define DESC_TAILCAPR_TAIL_FRZ_MASK 0x7FFFu

//! @}

//! \defgroup DESC_OFSQ Register DESC_OFSQ - Output Free Segment Queue Register
//! @{

//! Register Offset (relative)
#define DESC_OFSQ 0x24
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_OFSQ 0xE6800024u

//! Register Reset Value
#define DESC_OFSQ_RST 0x00000000u

//! Field HEAD - Head Logical Segment Address
#define DESC_OFSQ_HEAD_POS 0
//! Field HEAD - Head Logical Segment Address
#define DESC_OFSQ_HEAD_MASK 0x7FFFu

//! Field TAIL - Tail Logical Segment Address
#define DESC_OFSQ_TAIL_POS 16
//! Field TAIL - Tail Logical Segment Address
#define DESC_OFSQ_TAIL_MASK 0x7FFF0000u

//! @}

//! \defgroup DESC_OFSC Register DESC_OFSC - Output Free Segment Counter Register
//! @{

//! Register Offset (relative)
#define DESC_OFSC 0x2C
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_OFSC 0xE680002Cu

//! Register Reset Value
#define DESC_OFSC_RST 0x00000000u

//! Field FSC - Free Segment Counter
#define DESC_OFSC_FSC_POS 0
//! Field FSC - Free Segment Counter
#define DESC_OFSC_FSC_MASK 0x7FFFu

//! @}

//! \defgroup DESC_FSQT0 Register DESC_FSQT0 - Free Segment Queue Threshold Register 0
//! @{

//! Register Offset (relative)
#define DESC_FSQT0 0x30
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_FSQT0 0xE6800030u

//! Register Reset Value
#define DESC_FSQT0_RST 0x00000000u

//! Field FSQT - Free Segment Queue Threshold 0
#define DESC_FSQT0_FSQT_POS 0
//! Field FSQT - Free Segment Queue Threshold 0
#define DESC_FSQT0_FSQT_MASK 0x7FFFu

//! @}

//! \defgroup DESC_FSQT1 Register DESC_FSQT1 - Free Segment Queue Threshold Register 1
//! @{

//! Register Offset (relative)
#define DESC_FSQT1 0x70
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_FSQT1 0xE6800070u

//! Register Reset Value
#define DESC_FSQT1_RST 0x00000000u

//! Field FSQT - Free Segment Queue Threshold 1
#define DESC_FSQT1_FSQT_POS 0
//! Field FSQT - Free Segment Queue Threshold 1
#define DESC_FSQT1_FSQT_MASK 0x7FFFu

//! @}

//! \defgroup DESC_FSQT2 Register DESC_FSQT2 - Free Segment Queue Threshold Register 2
//! @{

//! Register Offset (relative)
#define DESC_FSQT2 0xB0
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_FSQT2 0xE68000B0u

//! Register Reset Value
#define DESC_FSQT2_RST 0x00000000u

//! Field FSQT - Free Segment Queue Threshold 2
#define DESC_FSQT2_FSQT_POS 0
//! Field FSQT - Free Segment Queue Threshold 2
#define DESC_FSQT2_FSQT_MASK 0x7FFFu

//! @}

//! \defgroup DESC_FSQT3 Register DESC_FSQT3 - Free Segment Queue Threshold Register 3
//! @{

//! Register Offset (relative)
#define DESC_FSQT3 0xF0
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_FSQT3 0xE68000F0u

//! Register Reset Value
#define DESC_FSQT3_RST 0x00000000u

//! Field FSQT - Free Segment Queue Threshold 3
#define DESC_FSQT3_FSQT_POS 0
//! Field FSQT - Free Segment Queue Threshold 3
#define DESC_FSQT3_FSQT_MASK 0x7FFFu

//! @}

//! \defgroup DESC_FSQT4 Register DESC_FSQT4 - Free Segment Queue Threshold Register 4
//! @{

//! Register Offset (relative)
#define DESC_FSQT4 0x130
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_FSQT4 0xE6800130u

//! Register Reset Value
#define DESC_FSQT4_RST 0x00000000u

//! Field FSQT - Free Segment Queue Threshold 4
#define DESC_FSQT4_FSQT_POS 0
//! Field FSQT - Free Segment Queue Threshold 4
#define DESC_FSQT4_FSQT_MASK 0x7FFFu

//! @}

//! \defgroup DESC_LSARNG Register DESC_LSARNG - Logical Segment Address Range
//! @{

//! Register Offset (relative)
#define DESC_LSARNG 0x180
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_LSARNG 0xE6800180u

//! Register Reset Value
#define DESC_LSARNG_RST 0x03FF0000u

//! Field MINLSA - Minimum Logical Segment Address
#define DESC_LSARNG_MINLSA_POS 0
//! Field MINLSA - Minimum Logical Segment Address
#define DESC_LSARNG_MINLSA_MASK 0x7FFFu

//! Field MAXLSA - Maximum Logical Segment Address
#define DESC_LSARNG_MAXLSA_POS 16
//! Field MAXLSA - Maximum Logical Segment Address
#define DESC_LSARNG_MAXLSA_MASK 0x7FFF0000u

//! @}

//! \defgroup DESC_OMQ Register DESC_OMQ - Output Message Queue Register
//! @{

//! Register Offset (relative)
#define DESC_OMQ 0x60000
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_OMQ 0xE6860000u

//! Register Reset Value
#define DESC_OMQ_RST 0x00000000u

//! Field HLSA - Head Logical Segment Address
#define DESC_OMQ_HLSA_POS 0
//! Field HLSA - Head Logical Segment Address
#define DESC_OMQ_HLSA_MASK 0x7FFFu

//! Field TLSA - Tail Logical Segment Address
#define DESC_OMQ_TLSA_POS 16
//! Field TLSA - Tail Logical Segment Address
#define DESC_OMQ_TLSA_MASK 0x7FFF0000u

//! @}

//! \defgroup DESC_RCNT Register DESC_RCNT - Repeat Counter Addresses
//! @{

//! Register Offset (relative)
#define DESC_RCNT 0x80000
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_RCNT 0xE6880000u

//! Register Reset Value
#define DESC_RCNT_RST 0x00000000u

//! Field RCNT - Repeat Counter Value
#define DESC_RCNT_RCNT_POS 0
//! Field RCNT - Repeat Counter Value
#define DESC_RCNT_RCNT_MASK 0x7u

//! @}

//! \defgroup DESC_RCNT_INCDEC Register DESC_RCNT_INCDEC - Repeat Counter Inc/Dec Addresses
//! @{

//! Register Offset (relative)
#define DESC_RCNT_INCDEC 0xA0000
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_RCNT_INCDEC 0xE68A0000u

//! Register Reset Value
#define DESC_RCNT_INCDEC_RST 0x00000000u

//! Field RCNT_OLD - Repeat Counter Value
#define DESC_RCNT_INCDEC_RCNT_OLD_POS 0
//! Field RCNT_OLD - Repeat Counter Value
#define DESC_RCNT_INCDEC_RCNT_OLD_MASK 0x7u

//! @}

//! \defgroup DESC_RAM Register DESC_RAM - LLT RAM Addresses
//! @{

//! Register Offset (relative)
#define DESC_RAM 0xC0000
//! Register Offset (absolute) for 1st Instance DESC_LINK_MANAGER
#define DESC_LINK_MANAGER_DESC_RAM 0xE68C0000u

//! Register Reset Value
#define DESC_RAM_RST 0x00000000u

//! Field SNEXT - Pointer to the next segment
#define DESC_RAM_SNEXT_POS 0
//! Field SNEXT - Pointer to the next segment
#define DESC_RAM_SNEXT_MASK 0x7FFFu

//! @}

//! @}

#endif
