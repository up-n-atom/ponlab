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
// LSD Source          : /home/l1502/lgm_chip/v_leichuan.priv.v-dfv.lgm_chip.leichuan/ipg_lsd/lsd_sys/source/xml/reg_files/CBM_lgm.xml
// Register File Name  : CQEM
// Register File Title : Central Queue Manager Top-level Configuration Register Description
// Register Width      : 32
// Note                : Doxygen compliant comments
//-----------------------------------------------------------------------------

#ifndef _CQEM_H
#define _CQEM_H

//! \defgroup CQEM Register File CQEM - Central Queue Manager Top-level Configuration Register Description
//! @{

//! Base Address of CQM_CONTROL
#define CQM_CONTROL_MODULE_BASE 0xE6610000u
//! Base Address of CQM_CONTROL
#define CQM_CONTROL_MODULE_BASE 0xE6610000u

//! \defgroup CBM_IRNCR_0 Register CBM_IRNCR_0 - CBM IRN Capture Register
//! @{

//! Register Offset (relative)
#define CBM_IRNCR_0 0x0
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNCR_0 0xE6610000u

//! Register Reset Value
#define CBM_IRNCR_0_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_0_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_0_FSQM0I_MASK 0x1u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_0_FSQM0I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_0_FSQM0I_INTACK 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_0_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_0_FSQM0ERRI_MASK 0x2u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_0_FSQM0ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_0_FSQM0ERRI_INTACK 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_0_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_0_FSQM1I_MASK 0x4u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_0_FSQM1I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_0_FSQM1I_INTACK 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_0_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_0_FSQM1ERRI_MASK 0x8u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_0_FSQM1ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_0_FSQM1ERRI_INTACK 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_0_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_0_LSI_MASK 0xFF0u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_0_LSI_NUL 0x00u
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_0_LSI_INTACK 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_0_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_0_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_0_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_0_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_0_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_0_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_0_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_0_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNICR_0 Register CBM_IRNICR_0 - CBM IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define CBM_IRNICR_0 0x4
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNICR_0 0xE6610004u

//! Register Reset Value
#define CBM_IRNICR_0_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_0_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_0_FSQM0I_MASK 0x1u

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_0_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_0_FSQM0ERRI_MASK 0x2u

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_0_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_0_FSQM1I_MASK 0x4u

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_0_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_0_FSQM1ERRI_MASK 0x8u

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_0_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_0_LSI_MASK 0xFF0u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_0_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_0_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_0_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_0_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_0_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_0_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_0_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_0_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNEN_0 Register CBM_IRNEN_0 - CBM IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define CBM_IRNEN_0 0x8
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNEN_0 0xE6610008u

//! Register Reset Value
#define CBM_IRNEN_0_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_0_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_0_FSQM0I_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_0_FSQM0I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_0_FSQM0I_EN 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_0_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_0_FSQM0ERRI_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_0_FSQM0ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_0_FSQM0ERRI_EN 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_0_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_0_FSQM1I_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_0_FSQM1I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_0_FSQM1I_EN 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_0_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_0_FSQM1ERRI_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_0_FSQM1ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_0_FSQM1ERRI_EN 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_0_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_0_LSI_MASK 0xFF0u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_0_LSI_DIS 0x00u
//! Constant EN - EN
#define CONST_CBM_IRNEN_0_LSI_EN 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_0_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_0_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_0_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_0_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_0_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_0_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_0_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_0_CPUPI_MASK 0x100000u

//! @}

//! \defgroup IGP_IRNCR_0 Register IGP_IRNCR_0 - Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IGP_IRNCR_0 0x10
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNCR_0 0xE6610010u

//! Register Reset Value
#define IGP_IRNCR_0_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_0_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_0_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNICR_0 Register IGP_IRNICR_0 - Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IGP_IRNICR_0 0x14
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNICR_0 0xE6610014u

//! Register Reset Value
#define IGP_IRNICR_0_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_0_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_0_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNEN_0 Register IGP_IRNEN_0 - Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IGP_IRNEN_0 0x18
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNEN_0 0xE6610018u

//! Register Reset Value
#define IGP_IRNEN_0_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_0_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_0_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup EGP_IRNCR_0 Register EGP_IRNCR_0 - Egress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define EGP_IRNCR_0 0x20
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNCR_0 0xE6610020u

//! Register Reset Value
#define EGP_IRNCR_0_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_0_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_0_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNICR_0 Register EGP_IRNICR_0 - Egress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define EGP_IRNICR_0 0x24
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNICR_0 0xE6610024u

//! Register Reset Value
#define EGP_IRNICR_0_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_0_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_0_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNEN_0 Register EGP_IRNEN_0 - Egress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define EGP_IRNEN_0 0x28
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNEN_0 0xE6610028u

//! Register Reset Value
#define EGP_IRNEN_0_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_0_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_0_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup CBM_IRNCR_1 Register CBM_IRNCR_1 - CBM IRN Capture Register
//! @{

//! Register Offset (relative)
#define CBM_IRNCR_1 0x40
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNCR_1 0xE6610040u

//! Register Reset Value
#define CBM_IRNCR_1_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_1_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_1_FSQM0I_MASK 0x1u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_1_FSQM0I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_1_FSQM0I_INTACK 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_1_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_1_FSQM0ERRI_MASK 0x2u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_1_FSQM0ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_1_FSQM0ERRI_INTACK 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_1_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_1_FSQM1I_MASK 0x4u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_1_FSQM1I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_1_FSQM1I_INTACK 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_1_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_1_FSQM1ERRI_MASK 0x8u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_1_FSQM1ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_1_FSQM1ERRI_INTACK 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_1_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_1_LSI_MASK 0xFF0u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_1_LSI_NUL 0x00u
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_1_LSI_INTACK 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_1_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_1_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_1_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_1_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_1_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_1_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_1_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_1_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNICR_1 Register CBM_IRNICR_1 - CBM IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define CBM_IRNICR_1 0x44
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNICR_1 0xE6610044u

//! Register Reset Value
#define CBM_IRNICR_1_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_1_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_1_FSQM0I_MASK 0x1u

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_1_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_1_FSQM0ERRI_MASK 0x2u

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_1_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_1_FSQM1I_MASK 0x4u

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_1_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_1_FSQM1ERRI_MASK 0x8u

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_1_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_1_LSI_MASK 0xFF0u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_1_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_1_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_1_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_1_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_1_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_1_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_1_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_1_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNEN_1 Register CBM_IRNEN_1 - CBM IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define CBM_IRNEN_1 0x48
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNEN_1 0xE6610048u

//! Register Reset Value
#define CBM_IRNEN_1_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_1_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_1_FSQM0I_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_1_FSQM0I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_1_FSQM0I_EN 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_1_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_1_FSQM0ERRI_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_1_FSQM0ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_1_FSQM0ERRI_EN 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_1_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_1_FSQM1I_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_1_FSQM1I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_1_FSQM1I_EN 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_1_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_1_FSQM1ERRI_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_1_FSQM1ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_1_FSQM1ERRI_EN 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_1_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_1_LSI_MASK 0xFF0u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_1_LSI_DIS 0x00u
//! Constant EN - EN
#define CONST_CBM_IRNEN_1_LSI_EN 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_1_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_1_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_1_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_1_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_1_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_1_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_1_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_1_CPUPI_MASK 0x100000u

//! @}

//! \defgroup IGP_IRNCR_1 Register IGP_IRNCR_1 - Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IGP_IRNCR_1 0x50
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNCR_1 0xE6610050u

//! Register Reset Value
#define IGP_IRNCR_1_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_1_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_1_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNICR_1 Register IGP_IRNICR_1 - Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IGP_IRNICR_1 0x54
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNICR_1 0xE6610054u

//! Register Reset Value
#define IGP_IRNICR_1_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_1_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_1_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNEN_1 Register IGP_IRNEN_1 - Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IGP_IRNEN_1 0x58
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNEN_1 0xE6610058u

//! Register Reset Value
#define IGP_IRNEN_1_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_1_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_1_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup EGP_IRNCR_1 Register EGP_IRNCR_1 - Egress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define EGP_IRNCR_1 0x60
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNCR_1 0xE6610060u

//! Register Reset Value
#define EGP_IRNCR_1_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_1_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_1_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNICR_1 Register EGP_IRNICR_1 - Egress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define EGP_IRNICR_1 0x64
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNICR_1 0xE6610064u

//! Register Reset Value
#define EGP_IRNICR_1_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_1_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_1_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNEN_1 Register EGP_IRNEN_1 - Egress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define EGP_IRNEN_1 0x68
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNEN_1 0xE6610068u

//! Register Reset Value
#define EGP_IRNEN_1_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_1_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_1_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup CBM_IRNCR_2 Register CBM_IRNCR_2 - CBM IRN Capture Register
//! @{

//! Register Offset (relative)
#define CBM_IRNCR_2 0x80
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNCR_2 0xE6610080u

//! Register Reset Value
#define CBM_IRNCR_2_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_2_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_2_FSQM0I_MASK 0x1u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_2_FSQM0I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_2_FSQM0I_INTACK 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_2_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_2_FSQM0ERRI_MASK 0x2u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_2_FSQM0ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_2_FSQM0ERRI_INTACK 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_2_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_2_FSQM1I_MASK 0x4u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_2_FSQM1I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_2_FSQM1I_INTACK 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_2_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_2_FSQM1ERRI_MASK 0x8u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_2_FSQM1ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_2_FSQM1ERRI_INTACK 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_2_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_2_LSI_MASK 0xFF0u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_2_LSI_NUL 0x00u
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_2_LSI_INTACK 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_2_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_2_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_2_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_2_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_2_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_2_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_2_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_2_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNICR_2 Register CBM_IRNICR_2 - CBM IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define CBM_IRNICR_2 0x84
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNICR_2 0xE6610084u

//! Register Reset Value
#define CBM_IRNICR_2_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_2_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_2_FSQM0I_MASK 0x1u

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_2_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_2_FSQM0ERRI_MASK 0x2u

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_2_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_2_FSQM1I_MASK 0x4u

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_2_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_2_FSQM1ERRI_MASK 0x8u

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_2_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_2_LSI_MASK 0xFF0u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_2_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_2_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_2_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_2_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_2_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_2_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_2_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_2_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNEN_2 Register CBM_IRNEN_2 - CBM IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define CBM_IRNEN_2 0x88
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNEN_2 0xE6610088u

//! Register Reset Value
#define CBM_IRNEN_2_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_2_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_2_FSQM0I_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_2_FSQM0I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_2_FSQM0I_EN 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_2_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_2_FSQM0ERRI_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_2_FSQM0ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_2_FSQM0ERRI_EN 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_2_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_2_FSQM1I_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_2_FSQM1I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_2_FSQM1I_EN 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_2_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_2_FSQM1ERRI_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_2_FSQM1ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_2_FSQM1ERRI_EN 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_2_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_2_LSI_MASK 0xFF0u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_2_LSI_DIS 0x00u
//! Constant EN - EN
#define CONST_CBM_IRNEN_2_LSI_EN 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_2_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_2_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_2_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_2_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_2_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_2_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_2_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_2_CPUPI_MASK 0x100000u

//! @}

//! \defgroup IGP_IRNCR_2 Register IGP_IRNCR_2 - Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IGP_IRNCR_2 0x90
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNCR_2 0xE6610090u

//! Register Reset Value
#define IGP_IRNCR_2_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_2_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_2_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNICR_2 Register IGP_IRNICR_2 - Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IGP_IRNICR_2 0x94
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNICR_2 0xE6610094u

//! Register Reset Value
#define IGP_IRNICR_2_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_2_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_2_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNEN_2 Register IGP_IRNEN_2 - Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IGP_IRNEN_2 0x98
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNEN_2 0xE6610098u

//! Register Reset Value
#define IGP_IRNEN_2_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_2_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_2_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup EGP_IRNCR_2 Register EGP_IRNCR_2 - Egress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define EGP_IRNCR_2 0xA0
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNCR_2 0xE66100A0u

//! Register Reset Value
#define EGP_IRNCR_2_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_2_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_2_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNICR_2 Register EGP_IRNICR_2 - Egress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define EGP_IRNICR_2 0xA4
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNICR_2 0xE66100A4u

//! Register Reset Value
#define EGP_IRNICR_2_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_2_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_2_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNEN_2 Register EGP_IRNEN_2 - Egress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define EGP_IRNEN_2 0xA8
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNEN_2 0xE66100A8u

//! Register Reset Value
#define EGP_IRNEN_2_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_2_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_2_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup CBM_IRNCR_3 Register CBM_IRNCR_3 - CBM IRN Capture Register
//! @{

//! Register Offset (relative)
#define CBM_IRNCR_3 0xC0
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNCR_3 0xE66100C0u

//! Register Reset Value
#define CBM_IRNCR_3_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_3_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_3_FSQM0I_MASK 0x1u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_3_FSQM0I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_3_FSQM0I_INTACK 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_3_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_3_FSQM0ERRI_MASK 0x2u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_3_FSQM0ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_3_FSQM0ERRI_INTACK 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_3_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_3_FSQM1I_MASK 0x4u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_3_FSQM1I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_3_FSQM1I_INTACK 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_3_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_3_FSQM1ERRI_MASK 0x8u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_3_FSQM1ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_3_FSQM1ERRI_INTACK 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_3_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_3_LSI_MASK 0xFF0u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_3_LSI_NUL 0x00u
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_3_LSI_INTACK 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_3_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_3_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_3_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_3_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_3_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_3_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_3_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_3_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNICR_3 Register CBM_IRNICR_3 - CBM IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define CBM_IRNICR_3 0xC4
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNICR_3 0xE66100C4u

//! Register Reset Value
#define CBM_IRNICR_3_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_3_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_3_FSQM0I_MASK 0x1u

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_3_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_3_FSQM0ERRI_MASK 0x2u

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_3_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_3_FSQM1I_MASK 0x4u

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_3_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_3_FSQM1ERRI_MASK 0x8u

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_3_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_3_LSI_MASK 0xFF0u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_3_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_3_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_3_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_3_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_3_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_3_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_3_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_3_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNEN_3 Register CBM_IRNEN_3 - CBM IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define CBM_IRNEN_3 0xC8
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNEN_3 0xE66100C8u

//! Register Reset Value
#define CBM_IRNEN_3_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_3_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_3_FSQM0I_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_3_FSQM0I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_3_FSQM0I_EN 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_3_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_3_FSQM0ERRI_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_3_FSQM0ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_3_FSQM0ERRI_EN 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_3_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_3_FSQM1I_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_3_FSQM1I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_3_FSQM1I_EN 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_3_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_3_FSQM1ERRI_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_3_FSQM1ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_3_FSQM1ERRI_EN 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_3_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_3_LSI_MASK 0xFF0u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_3_LSI_DIS 0x00u
//! Constant EN - EN
#define CONST_CBM_IRNEN_3_LSI_EN 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_3_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_3_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_3_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_3_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_3_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_3_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_3_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_3_CPUPI_MASK 0x100000u

//! @}

//! \defgroup IGP_IRNCR_3 Register IGP_IRNCR_3 - Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IGP_IRNCR_3 0xD0
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNCR_3 0xE66100D0u

//! Register Reset Value
#define IGP_IRNCR_3_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_3_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_3_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNICR_3 Register IGP_IRNICR_3 - Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IGP_IRNICR_3 0xD4
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNICR_3 0xE66100D4u

//! Register Reset Value
#define IGP_IRNICR_3_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_3_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_3_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNEN_3 Register IGP_IRNEN_3 - Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IGP_IRNEN_3 0xD8
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNEN_3 0xE66100D8u

//! Register Reset Value
#define IGP_IRNEN_3_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_3_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_3_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup EGP_IRNCR_3 Register EGP_IRNCR_3 - Egress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define EGP_IRNCR_3 0xE0
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNCR_3 0xE66100E0u

//! Register Reset Value
#define EGP_IRNCR_3_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_3_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_3_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNICR_3 Register EGP_IRNICR_3 - Egress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define EGP_IRNICR_3 0xE4
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNICR_3 0xE66100E4u

//! Register Reset Value
#define EGP_IRNICR_3_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_3_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_3_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNEN_3 Register EGP_IRNEN_3 - Egress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define EGP_IRNEN_3 0xE8
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNEN_3 0xE66100E8u

//! Register Reset Value
#define EGP_IRNEN_3_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_3_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_3_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup CBM_IRNCR_4 Register CBM_IRNCR_4 - CBM IRN Capture Register
//! @{

//! Register Offset (relative)
#define CBM_IRNCR_4 0x100
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNCR_4 0xE6610100u

//! Register Reset Value
#define CBM_IRNCR_4_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_4_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_4_FSQM0I_MASK 0x1u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_4_FSQM0I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_4_FSQM0I_INTACK 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_4_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_4_FSQM0ERRI_MASK 0x2u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_4_FSQM0ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_4_FSQM0ERRI_INTACK 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_4_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_4_FSQM1I_MASK 0x4u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_4_FSQM1I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_4_FSQM1I_INTACK 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_4_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_4_FSQM1ERRI_MASK 0x8u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_4_FSQM1ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_4_FSQM1ERRI_INTACK 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_4_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_4_LSI_MASK 0xFF0u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_4_LSI_NUL 0x00u
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_4_LSI_INTACK 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_4_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_4_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_4_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_4_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_4_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_4_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_4_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_4_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNICR_4 Register CBM_IRNICR_4 - CBM IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define CBM_IRNICR_4 0x104
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNICR_4 0xE6610104u

//! Register Reset Value
#define CBM_IRNICR_4_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_4_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_4_FSQM0I_MASK 0x1u

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_4_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_4_FSQM0ERRI_MASK 0x2u

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_4_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_4_FSQM1I_MASK 0x4u

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_4_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_4_FSQM1ERRI_MASK 0x8u

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_4_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_4_LSI_MASK 0xFF0u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_4_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_4_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_4_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_4_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_4_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_4_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_4_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_4_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNEN_4 Register CBM_IRNEN_4 - CBM IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define CBM_IRNEN_4 0x108
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNEN_4 0xE6610108u

//! Register Reset Value
#define CBM_IRNEN_4_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_4_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_4_FSQM0I_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_4_FSQM0I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_4_FSQM0I_EN 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_4_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_4_FSQM0ERRI_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_4_FSQM0ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_4_FSQM0ERRI_EN 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_4_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_4_FSQM1I_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_4_FSQM1I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_4_FSQM1I_EN 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_4_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_4_FSQM1ERRI_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_4_FSQM1ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_4_FSQM1ERRI_EN 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_4_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_4_LSI_MASK 0xFF0u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_4_LSI_DIS 0x00u
//! Constant EN - EN
#define CONST_CBM_IRNEN_4_LSI_EN 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_4_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_4_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_4_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_4_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_4_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_4_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_4_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_4_CPUPI_MASK 0x100000u

//! @}

//! \defgroup IGP_IRNCR_4 Register IGP_IRNCR_4 - Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IGP_IRNCR_4 0x110
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNCR_4 0xE6610110u

//! Register Reset Value
#define IGP_IRNCR_4_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_4_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_4_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNICR_4 Register IGP_IRNICR_4 - Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IGP_IRNICR_4 0x114
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNICR_4 0xE6610114u

//! Register Reset Value
#define IGP_IRNICR_4_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_4_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_4_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNEN_4 Register IGP_IRNEN_4 - Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IGP_IRNEN_4 0x118
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNEN_4 0xE6610118u

//! Register Reset Value
#define IGP_IRNEN_4_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_4_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_4_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup EGP_IRNCR_4 Register EGP_IRNCR_4 - Egress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define EGP_IRNCR_4 0x120
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNCR_4 0xE6610120u

//! Register Reset Value
#define EGP_IRNCR_4_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_4_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_4_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNICR_4 Register EGP_IRNICR_4 - Egress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define EGP_IRNICR_4 0x124
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNICR_4 0xE6610124u

//! Register Reset Value
#define EGP_IRNICR_4_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_4_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_4_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNEN_4 Register EGP_IRNEN_4 - Egress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define EGP_IRNEN_4 0x128
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNEN_4 0xE6610128u

//! Register Reset Value
#define EGP_IRNEN_4_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_4_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_4_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup CBM_IRNCR_5 Register CBM_IRNCR_5 - CBM IRN Capture Register
//! @{

//! Register Offset (relative)
#define CBM_IRNCR_5 0x140
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNCR_5 0xE6610140u

//! Register Reset Value
#define CBM_IRNCR_5_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_5_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_5_FSQM0I_MASK 0x1u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_5_FSQM0I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_5_FSQM0I_INTACK 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_5_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_5_FSQM0ERRI_MASK 0x2u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_5_FSQM0ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_5_FSQM0ERRI_INTACK 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_5_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_5_FSQM1I_MASK 0x4u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_5_FSQM1I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_5_FSQM1I_INTACK 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_5_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_5_FSQM1ERRI_MASK 0x8u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_5_FSQM1ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_5_FSQM1ERRI_INTACK 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_5_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_5_LSI_MASK 0xFF0u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_5_LSI_NUL 0x00u
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_5_LSI_INTACK 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_5_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_5_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_5_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_5_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_5_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_5_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_5_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_5_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNICR_5 Register CBM_IRNICR_5 - CBM IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define CBM_IRNICR_5 0x144
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNICR_5 0xE6610144u

//! Register Reset Value
#define CBM_IRNICR_5_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_5_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_5_FSQM0I_MASK 0x1u

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_5_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_5_FSQM0ERRI_MASK 0x2u

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_5_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_5_FSQM1I_MASK 0x4u

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_5_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_5_FSQM1ERRI_MASK 0x8u

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_5_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_5_LSI_MASK 0xFF0u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_5_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_5_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_5_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_5_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_5_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_5_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_5_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_5_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNEN_5 Register CBM_IRNEN_5 - CBM IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define CBM_IRNEN_5 0x148
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNEN_5 0xE6610148u

//! Register Reset Value
#define CBM_IRNEN_5_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_5_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_5_FSQM0I_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_5_FSQM0I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_5_FSQM0I_EN 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_5_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_5_FSQM0ERRI_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_5_FSQM0ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_5_FSQM0ERRI_EN 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_5_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_5_FSQM1I_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_5_FSQM1I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_5_FSQM1I_EN 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_5_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_5_FSQM1ERRI_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_5_FSQM1ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_5_FSQM1ERRI_EN 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_5_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_5_LSI_MASK 0xFF0u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_5_LSI_DIS 0x00u
//! Constant EN - EN
#define CONST_CBM_IRNEN_5_LSI_EN 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_5_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_5_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_5_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_5_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_5_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_5_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_5_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_5_CPUPI_MASK 0x100000u

//! @}

//! \defgroup IGP_IRNCR_5 Register IGP_IRNCR_5 - Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IGP_IRNCR_5 0x150
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNCR_5 0xE6610150u

//! Register Reset Value
#define IGP_IRNCR_5_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_5_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_5_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNICR_5 Register IGP_IRNICR_5 - Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IGP_IRNICR_5 0x154
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNICR_5 0xE6610154u

//! Register Reset Value
#define IGP_IRNICR_5_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_5_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_5_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNEN_5 Register IGP_IRNEN_5 - Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IGP_IRNEN_5 0x158
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNEN_5 0xE6610158u

//! Register Reset Value
#define IGP_IRNEN_5_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_5_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_5_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup EGP_IRNCR_5 Register EGP_IRNCR_5 - Egress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define EGP_IRNCR_5 0x160
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNCR_5 0xE6610160u

//! Register Reset Value
#define EGP_IRNCR_5_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_5_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_5_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNICR_5 Register EGP_IRNICR_5 - Egress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define EGP_IRNICR_5 0x164
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNICR_5 0xE6610164u

//! Register Reset Value
#define EGP_IRNICR_5_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_5_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_5_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNEN_5 Register EGP_IRNEN_5 - Egress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define EGP_IRNEN_5 0x168
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNEN_5 0xE6610168u

//! Register Reset Value
#define EGP_IRNEN_5_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_5_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_5_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup CBM_IRNCR_6 Register CBM_IRNCR_6 - CBM IRN Capture Register
//! @{

//! Register Offset (relative)
#define CBM_IRNCR_6 0x180
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNCR_6 0xE6610180u

//! Register Reset Value
#define CBM_IRNCR_6_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_6_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_6_FSQM0I_MASK 0x1u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_6_FSQM0I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_6_FSQM0I_INTACK 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_6_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_6_FSQM0ERRI_MASK 0x2u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_6_FSQM0ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_6_FSQM0ERRI_INTACK 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_6_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_6_FSQM1I_MASK 0x4u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_6_FSQM1I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_6_FSQM1I_INTACK 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_6_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_6_FSQM1ERRI_MASK 0x8u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_6_FSQM1ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_6_FSQM1ERRI_INTACK 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_6_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_6_LSI_MASK 0xFF0u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_6_LSI_NUL 0x00u
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_6_LSI_INTACK 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_6_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_6_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_6_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_6_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_6_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_6_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_6_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_6_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNICR_6 Register CBM_IRNICR_6 - CBM IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define CBM_IRNICR_6 0x184
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNICR_6 0xE6610184u

//! Register Reset Value
#define CBM_IRNICR_6_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_6_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_6_FSQM0I_MASK 0x1u

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_6_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_6_FSQM0ERRI_MASK 0x2u

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_6_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_6_FSQM1I_MASK 0x4u

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_6_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_6_FSQM1ERRI_MASK 0x8u

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_6_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_6_LSI_MASK 0xFF0u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_6_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_6_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_6_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_6_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_6_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_6_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_6_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_6_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNEN_6 Register CBM_IRNEN_6 - CBM IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define CBM_IRNEN_6 0x188
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNEN_6 0xE6610188u

//! Register Reset Value
#define CBM_IRNEN_6_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_6_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_6_FSQM0I_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_6_FSQM0I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_6_FSQM0I_EN 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_6_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_6_FSQM0ERRI_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_6_FSQM0ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_6_FSQM0ERRI_EN 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_6_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_6_FSQM1I_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_6_FSQM1I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_6_FSQM1I_EN 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_6_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_6_FSQM1ERRI_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_6_FSQM1ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_6_FSQM1ERRI_EN 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_6_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_6_LSI_MASK 0xFF0u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_6_LSI_DIS 0x00u
//! Constant EN - EN
#define CONST_CBM_IRNEN_6_LSI_EN 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_6_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_6_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_6_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_6_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_6_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_6_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_6_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_6_CPUPI_MASK 0x100000u

//! @}

//! \defgroup IGP_IRNCR_6 Register IGP_IRNCR_6 - Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IGP_IRNCR_6 0x190
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNCR_6 0xE6610190u

//! Register Reset Value
#define IGP_IRNCR_6_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_6_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_6_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNICR_6 Register IGP_IRNICR_6 - Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IGP_IRNICR_6 0x194
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNICR_6 0xE6610194u

//! Register Reset Value
#define IGP_IRNICR_6_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_6_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_6_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNEN_6 Register IGP_IRNEN_6 - Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IGP_IRNEN_6 0x198
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNEN_6 0xE6610198u

//! Register Reset Value
#define IGP_IRNEN_6_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_6_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_6_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup EGP_IRNCR_6 Register EGP_IRNCR_6 - Egress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define EGP_IRNCR_6 0x1A0
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNCR_6 0xE66101A0u

//! Register Reset Value
#define EGP_IRNCR_6_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_6_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_6_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNICR_6 Register EGP_IRNICR_6 - Egress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define EGP_IRNICR_6 0x1A4
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNICR_6 0xE66101A4u

//! Register Reset Value
#define EGP_IRNICR_6_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_6_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_6_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNEN_6 Register EGP_IRNEN_6 - Egress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define EGP_IRNEN_6 0x1A8
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNEN_6 0xE66101A8u

//! Register Reset Value
#define EGP_IRNEN_6_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_6_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_6_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup CBM_IRNCR_7 Register CBM_IRNCR_7 - CBM IRN Capture Register
//! @{

//! Register Offset (relative)
#define CBM_IRNCR_7 0x1C0
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNCR_7 0xE66101C0u

//! Register Reset Value
#define CBM_IRNCR_7_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_7_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNCR_7_FSQM0I_MASK 0x1u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_7_FSQM0I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_7_FSQM0I_INTACK 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_7_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNCR_7_FSQM0ERRI_MASK 0x2u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_7_FSQM0ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_7_FSQM0ERRI_INTACK 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_7_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNCR_7_FSQM1I_MASK 0x4u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_7_FSQM1I_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_7_FSQM1I_INTACK 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_7_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNCR_7_FSQM1ERRI_MASK 0x8u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_7_FSQM1ERRI_NUL 0x0
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_7_FSQM1ERRI_INTACK 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_7_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNCR_7_LSI_MASK 0xFF0u
//! Constant NUL - NULL
#define CONST_CBM_IRNCR_7_LSI_NUL 0x00u
//! Constant INTACK - INTACK
#define CONST_CBM_IRNCR_7_LSI_INTACK 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_7_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNCR_7_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_7_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNCR_7_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_7_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNCR_7_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_7_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNCR_7_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNICR_7 Register CBM_IRNICR_7 - CBM IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define CBM_IRNICR_7 0x1C4
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNICR_7 0xE66101C4u

//! Register Reset Value
#define CBM_IRNICR_7_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_7_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNICR_7_FSQM0I_MASK 0x1u

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_7_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNICR_7_FSQM0ERRI_MASK 0x2u

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_7_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNICR_7_FSQM1I_MASK 0x4u

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_7_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNICR_7_FSQM1ERRI_MASK 0x8u

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_7_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNICR_7_LSI_MASK 0xFF0u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_7_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNICR_7_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_7_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNICR_7_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_7_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNICR_7_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_7_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNICR_7_CPUPI_MASK 0x100000u

//! @}

//! \defgroup CBM_IRNEN_7 Register CBM_IRNEN_7 - CBM IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define CBM_IRNEN_7 0x1C8
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_IRNEN_7 0xE66101C8u

//! Register Reset Value
#define CBM_IRNEN_7_RST 0x00000000u

//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_7_FSQM0I_POS 0
//! Field FSQM0I - FSQM0 Interrupt
#define CBM_IRNEN_7_FSQM0I_MASK 0x1u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_7_FSQM0I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_7_FSQM0I_EN 0x1

//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_7_FSQM0ERRI_POS 1
//! Field FSQM0ERRI - FSQM0 Error Interrupt
#define CBM_IRNEN_7_FSQM0ERRI_MASK 0x2u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_7_FSQM0ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_7_FSQM0ERRI_EN 0x1

//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_7_FSQM1I_POS 2
//! Field FSQM1I - FSQM1 Interrupt
#define CBM_IRNEN_7_FSQM1I_MASK 0x4u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_7_FSQM1I_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_7_FSQM1I_EN 0x1

//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_7_FSQM1ERRI_POS 3
//! Field FSQM1ERRI - FSQM1 Error Interrupt
#define CBM_IRNEN_7_FSQM1ERRI_MASK 0x8u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_7_FSQM1ERRI_DIS 0x0
//! Constant EN - EN
#define CONST_CBM_IRNEN_7_FSQM1ERRI_EN 0x1

//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_7_LSI_POS 4
//! Field LSI - Load Spreader Interrupt
#define CBM_IRNEN_7_LSI_MASK 0xFF0u
//! Constant DIS - DIS
#define CONST_CBM_IRNEN_7_LSI_DIS 0x00u
//! Constant EN - EN
#define CONST_CBM_IRNEN_7_LSI_EN 0x01u

//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_7_PIBI_POS 12
//! Field PIBI - PIB Block Interrupt
#define CBM_IRNEN_7_PIBI_MASK 0x3000u

//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_7_UMTI_POS 14
//! Field UMTI - UMT Block Interrupt
#define CBM_IRNEN_7_UMTI_MASK 0x4000u

//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_7_LLI_POS 16
//! Field LLI - Linked List Interrupt
#define CBM_IRNEN_7_LLI_MASK 0x10000u

//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_7_CPUPI_POS 20
//! Field CPUPI - CPU Pool Interrupt
#define CBM_IRNEN_7_CPUPI_MASK 0x100000u

//! @}

//! \defgroup IGP_IRNCR_7 Register IGP_IRNCR_7 - Ingress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define IGP_IRNCR_7 0x1D0
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNCR_7 0xE66101D0u

//! Register Reset Value
#define IGP_IRNCR_7_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_7_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNCR_7_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNICR_7 Register IGP_IRNICR_7 - Ingress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define IGP_IRNICR_7 0x1D4
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNICR_7 0xE66101D4u

//! Register Reset Value
#define IGP_IRNICR_7_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_7_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNICR_7_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup IGP_IRNEN_7 Register IGP_IRNEN_7 - Ingress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define IGP_IRNEN_7 0x1D8
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_IGP_IRNEN_7 0xE66101D8u

//! Register Reset Value
#define IGP_IRNEN_7_RST 0x00000000u

//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_7_IGPI_POS 0
//! Field IGPI - Ingress Port Interrupt
#define IGP_IRNEN_7_IGPI_MASK 0x1FFFFFFu

//! @}

//! \defgroup EGP_IRNCR_7 Register EGP_IRNCR_7 - Egress Port IRN Capture Register
//! @{

//! Register Offset (relative)
#define EGP_IRNCR_7 0x1E0
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNCR_7 0xE66101E0u

//! Register Reset Value
#define EGP_IRNCR_7_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_7_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNCR_7_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNICR_7 Register EGP_IRNICR_7 - Egress Port IRN Interrupt Control Register
//! @{

//! Register Offset (relative)
#define EGP_IRNICR_7 0x1E4
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNICR_7 0xE66101E4u

//! Register Reset Value
#define EGP_IRNICR_7_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_7_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNICR_7_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup EGP_IRNEN_7 Register EGP_IRNEN_7 - Egress Port IRN Interrupt Enable Register
//! @{

//! Register Offset (relative)
#define EGP_IRNEN_7 0x1E8
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_EGP_IRNEN_7 0xE66101E8u

//! Register Reset Value
#define EGP_IRNEN_7_RST 0x00000000u

//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_7_EGPI_POS 0
//! Field EGPI - Egress Port Interrupt
#define EGP_IRNEN_7_EGPI_MASK 0xFFFFFFFu

//! @}

//! \defgroup CBM_VERSION_REG Register CBM_VERSION_REG - CBM/CQEM Version Register
//! @{

//! Register Offset (relative)
#define CBM_VERSION_REG 0x200
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_VERSION_REG 0xE6610200u

//! Register Reset Value
#define CBM_VERSION_REG_RST 0x00000103u

//! Field MAJOR - MAJOR Version Number
#define CBM_VERSION_REG_MAJOR_POS 0
//! Field MAJOR - MAJOR Version Number
#define CBM_VERSION_REG_MAJOR_MASK 0xFu

//! Field MINOR - MINOR Version Number
#define CBM_VERSION_REG_MINOR_POS 8
//! Field MINOR - MINOR Version Number
#define CBM_VERSION_REG_MINOR_MASK 0xF00u

//! @}

//! \defgroup CBM_BUF_SIZE0_7 Register CBM_BUF_SIZE0_7 - Buffer Size Select Register
//! @{

//! Register Offset (relative)
#define CBM_BUF_SIZE0_7 0x204
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_BUF_SIZE0_7 0xE6610204u

//! Register Reset Value
#define CBM_BUF_SIZE0_7_RST 0x44447431u

//! Field SIZE0 - Buffer Size of Pool 0
#define CBM_BUF_SIZE0_7_SIZE0_POS 0
//! Field SIZE0 - Buffer Size of Pool 0
#define CBM_BUF_SIZE0_7_SIZE0_MASK 0x7u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE0_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE0_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE0_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE0_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE0_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE0_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE0_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE0_S10240 0x7

//! Field SIZE1 - Buffer Size of Pool 1
#define CBM_BUF_SIZE0_7_SIZE1_POS 4
//! Field SIZE1 - Buffer Size of Pool 1
#define CBM_BUF_SIZE0_7_SIZE1_MASK 0x70u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE1_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE1_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE1_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE1_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE1_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE1_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE1_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE1_S10240 0x7

//! Field SIZE2 - Buffer Size of Pool 2
#define CBM_BUF_SIZE0_7_SIZE2_POS 8
//! Field SIZE2 - Buffer Size of Pool 2
#define CBM_BUF_SIZE0_7_SIZE2_MASK 0x700u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE2_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE2_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE2_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE2_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE2_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE2_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE2_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE2_S10240 0x7

//! Field SIZE3 - Buffer Size of Pool 3
#define CBM_BUF_SIZE0_7_SIZE3_POS 12
//! Field SIZE3 - Buffer Size of Pool 3
#define CBM_BUF_SIZE0_7_SIZE3_MASK 0x7000u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE3_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE3_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE3_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE3_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE3_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE3_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE3_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE3_S10240 0x7

//! Field SIZE4 - Buffer Size of Pool 4
#define CBM_BUF_SIZE0_7_SIZE4_POS 16
//! Field SIZE4 - Buffer Size of Pool 4
#define CBM_BUF_SIZE0_7_SIZE4_MASK 0x70000u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE4_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE4_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE4_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE4_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE4_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE4_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE4_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE4_S10240 0x7

//! Field SIZE5 - Buffer Size of Pool 5
#define CBM_BUF_SIZE0_7_SIZE5_POS 20
//! Field SIZE5 - Buffer Size of Pool 5
#define CBM_BUF_SIZE0_7_SIZE5_MASK 0x700000u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE5_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE5_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE5_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE5_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE5_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE5_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE5_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE5_S10240 0x7

//! Field SIZE6 - Buffer Size of Pool 6
#define CBM_BUF_SIZE0_7_SIZE6_POS 24
//! Field SIZE6 - Buffer Size of Pool 6
#define CBM_BUF_SIZE0_7_SIZE6_MASK 0x7000000u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE6_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE6_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE6_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE6_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE6_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE6_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE6_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE6_S10240 0x7

//! Field SIZE7 - Buffer Size of Pool 7
#define CBM_BUF_SIZE0_7_SIZE7_POS 28
//! Field SIZE7 - Buffer Size of Pool 7
#define CBM_BUF_SIZE0_7_SIZE7_MASK 0x70000000u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE7_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE7_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE7_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE7_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE7_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE7_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE7_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE0_7_SIZE7_S10240 0x7

//! @}

//! \defgroup CBM_BUF_SIZE8_15 Register CBM_BUF_SIZE8_15 - Buffer Size Select Register
//! @{

//! Register Offset (relative)
#define CBM_BUF_SIZE8_15 0x208
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_BUF_SIZE8_15 0xE6610208u

//! Register Reset Value
#define CBM_BUF_SIZE8_15_RST 0x44444444u

//! Field SIZE8 - Buffer Size of Pool 8
#define CBM_BUF_SIZE8_15_SIZE8_POS 0
//! Field SIZE8 - Buffer Size of Pool 8
#define CBM_BUF_SIZE8_15_SIZE8_MASK 0x7u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE8_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE8_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE8_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE8_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE8_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE8_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE8_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE8_S10240 0x7

//! Field SIZE9 - Buffer Size of Pool 9
#define CBM_BUF_SIZE8_15_SIZE9_POS 4
//! Field SIZE9 - Buffer Size of Pool 9
#define CBM_BUF_SIZE8_15_SIZE9_MASK 0x70u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE9_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE9_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE9_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE9_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE9_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE9_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE9_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE9_S10240 0x7

//! Field SIZE10 - Buffer Size of Pool 10
#define CBM_BUF_SIZE8_15_SIZE10_POS 8
//! Field SIZE10 - Buffer Size of Pool 10
#define CBM_BUF_SIZE8_15_SIZE10_MASK 0x700u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE10_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE10_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE10_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE10_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE10_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE10_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE10_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE10_S10240 0x7

//! Field SIZE11 - Buffer Size of Pool 11
#define CBM_BUF_SIZE8_15_SIZE11_POS 12
//! Field SIZE11 - Buffer Size of Pool 11
#define CBM_BUF_SIZE8_15_SIZE11_MASK 0x7000u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE11_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE11_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE11_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE11_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE11_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE11_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE11_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE11_S10240 0x7

//! Field SIZE12 - Buffer Size of Pool 12
#define CBM_BUF_SIZE8_15_SIZE12_POS 16
//! Field SIZE12 - Buffer Size of Pool 12
#define CBM_BUF_SIZE8_15_SIZE12_MASK 0x70000u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE12_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE12_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE12_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE12_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE12_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE12_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE12_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE12_S10240 0x7

//! Field SIZE13 - Buffer Size of Pool 13
#define CBM_BUF_SIZE8_15_SIZE13_POS 20
//! Field SIZE13 - Buffer Size of Pool 13
#define CBM_BUF_SIZE8_15_SIZE13_MASK 0x700000u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE13_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE13_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE13_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE13_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE13_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE13_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE13_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE13_S10240 0x7

//! Field SIZE14 - Buffer Size of Pool 14
#define CBM_BUF_SIZE8_15_SIZE14_POS 24
//! Field SIZE14 - Buffer Size of Pool 14
#define CBM_BUF_SIZE8_15_SIZE14_MASK 0x7000000u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE14_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE14_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE14_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE14_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE14_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE14_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE14_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE14_S10240 0x7

//! Field SIZE15 - Buffer Size of Pool 15
#define CBM_BUF_SIZE8_15_SIZE15_POS 28
//! Field SIZE15 - Buffer Size of Pool 15
#define CBM_BUF_SIZE8_15_SIZE15_MASK 0x70000000u
//! Constant S128 - 128B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE15_S128 0x0
//! Constant S256 - 256B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE15_S256 0x1
//! Constant S512 - 512B buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE15_S512 0x2
//! Constant S1024 - 1kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE15_S1024 0x3
//! Constant S2048 - 2kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE15_S2048 0x4
//! Constant S4096 - 4kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE15_S4096 0x5
//! Constant S8192 - 8kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE15_S8192 0x6
//! Constant S10240 - 10kB buffers
#define CONST_CBM_BUF_SIZE8_15_SIZE15_S10240 0x7

//! @}

//! \defgroup CBM_PB_BASE Register CBM_PB_BASE - Packet Buffer Base Address
//! @{

//! Register Offset (relative)
#define CBM_PB_BASE 0x20C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_PB_BASE 0xE661020Cu

//! Register Reset Value
#define CBM_PB_BASE_RST 0x00000000u

//! Field BASE - Base Address
#define CBM_PB_BASE_BASE_POS 3
//! Field BASE - Base Address
#define CBM_PB_BASE_BASE_MASK 0xFFFFFFF8u

//! @}

//! \defgroup CBM_CTRL Register CBM_CTRL - CBM Control
//! @{

//! Register Offset (relative)
#define CBM_CTRL 0x210
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_CTRL 0xE6610210u

//! Register Reset Value
#define CBM_CTRL_RST 0x00000000u

//! Field ACT - CBM Activity Status
#define CBM_CTRL_ACT_POS 0
//! Field ACT - CBM Activity Status
#define CBM_CTRL_ACT_MASK 0x1u
//! Constant INACTIVE - Inactive
#define CONST_CBM_CTRL_ACT_INACTIVE 0x0
//! Constant ACTIVE - Active
#define CONST_CBM_CTRL_ACT_ACTIVE 0x1

//! Field UMT_RST - UMT Reset
#define CBM_CTRL_UMT_RST_POS 8
//! Field UMT_RST - UMT Reset
#define CBM_CTRL_UMT_RST_MASK 0x100u
//! Constant NORMAL - Normal Operation
#define CONST_CBM_CTRL_UMT_RST_NORMAL 0x0
//! Constant RESET - Reset the UMT
#define CONST_CBM_CTRL_UMT_RST_RESET 0x1

//! Field LSBYP - Hardware SMS lightspeed mode
#define CBM_CTRL_LSBYP_POS 16
//! Field LSBYP - Hardware SMS lightspeed mode
#define CBM_CTRL_LSBYP_MASK 0x10000u
//! Constant BYP - Lightspeed mode bypass
#define CONST_CBM_CTRL_LSBYP_BYP 0x0
//! Constant LS - Lightspeed mode enable
#define CONST_CBM_CTRL_LSBYP_LS 0x1

//! Field PBSEL - Packet Buffer Select
#define CBM_CTRL_PBSEL_POS 17
//! Field PBSEL - Packet Buffer Select
#define CBM_CTRL_PBSEL_MASK 0x20000u
//! Constant PBSEL0 - size 128 Bytes
#define CONST_CBM_CTRL_PBSEL_PBSEL0 0x0
//! Constant PBSEL1 - size 2 KBytes
#define CONST_CBM_CTRL_PBSEL_PBSEL1 0x1

//! Field FSQM1_RST - FSQM1 Reset
#define CBM_CTRL_FSQM1_RST_POS 23
//! Field FSQM1_RST - FSQM1 Reset
#define CBM_CTRL_FSQM1_RST_MASK 0x800000u
//! Constant NORMAL - Normal Operation
#define CONST_CBM_CTRL_FSQM1_RST_NORMAL 0x0
//! Constant RESET - Reset the FSQM0
#define CONST_CBM_CTRL_FSQM1_RST_RESET 0x1

//! Field LL_DBG - Linked List Debug
#define CBM_CTRL_LL_DBG_POS 24
//! Field LL_DBG - Linked List Debug
#define CBM_CTRL_LL_DBG_MASK 0x1000000u
//! Constant DIS - Disable
#define CONST_CBM_CTRL_LL_DBG_DIS 0x0
//! Constant EN - Enable
#define CONST_CBM_CTRL_LL_DBG_EN 0x1

//! Field DESC64B_RST - 64-bit Descriptor Reset
#define CBM_CTRL_DESC64B_RST_POS 25
//! Field DESC64B_RST - 64-bit Descriptor Reset
#define CBM_CTRL_DESC64B_RST_MASK 0x2000000u
//! Constant NORMAL - Normal Operation
#define CONST_CBM_CTRL_DESC64B_RST_NORMAL 0x0
//! Constant RESET - Reset the 64-bit descriptors
#define CONST_CBM_CTRL_DESC64B_RST_RESET 0x1

//! Field LS_RST - LS Reset
#define CBM_CTRL_LS_RST_POS 26
//! Field LS_RST - LS Reset
#define CBM_CTRL_LS_RST_MASK 0x4000000u
//! Constant NORMAL - Normal Operation
#define CONST_CBM_CTRL_LS_RST_NORMAL 0x0
//! Constant RESET - Reset the LS
#define CONST_CBM_CTRL_LS_RST_RESET 0x1

//! Field DESC128_RST - 128b DMA Desc Reset
#define CBM_CTRL_DESC128_RST_POS 27
//! Field DESC128_RST - 128b DMA Desc Reset
#define CBM_CTRL_DESC128_RST_MASK 0x8000000u
//! Constant NORMAL - Normal Operation
#define CONST_CBM_CTRL_DESC128_RST_NORMAL 0x0
//! Constant RESET - Reset the QOS and BM
#define CONST_CBM_CTRL_DESC128_RST_RESET 0x1

//! Field PIB_RST - PIB Reset
#define CBM_CTRL_PIB_RST_POS 28
//! Field PIB_RST - PIB Reset
#define CBM_CTRL_PIB_RST_MASK 0x10000000u
//! Constant NORMAL - Normal Operation
#define CONST_CBM_CTRL_PIB_RST_NORMAL 0x0
//! Constant RESET - Reset the BM
#define CONST_CBM_CTRL_PIB_RST_RESET 0x1

//! Field FSQM0_RST - FSQM0 Reset
#define CBM_CTRL_FSQM0_RST_POS 29
//! Field FSQM0_RST - FSQM0 Reset
#define CBM_CTRL_FSQM0_RST_MASK 0x20000000u
//! Constant NORMAL - Normal Operation
#define CONST_CBM_CTRL_FSQM0_RST_NORMAL 0x0
//! Constant RESET - Reset the FSQM0
#define CONST_CBM_CTRL_FSQM0_RST_RESET 0x1

//! Field DQM_RST - DQM Reset
#define CBM_CTRL_DQM_RST_POS 30
//! Field DQM_RST - DQM Reset
#define CBM_CTRL_DQM_RST_MASK 0x40000000u
//! Constant NORMAL - Normal Operation
#define CONST_CBM_CTRL_DQM_RST_NORMAL 0x0
//! Constant RESET - Reset the Dequeue Manager
#define CONST_CBM_CTRL_DQM_RST_RESET 0x1

//! Field EQM_RST - EQM Reset
#define CBM_CTRL_EQM_RST_POS 31
//! Field EQM_RST - EQM Reset
#define CBM_CTRL_EQM_RST_MASK 0x80000000u
//! Constant NORMAL - Normal Operation
#define CONST_CBM_CTRL_EQM_RST_NORMAL 0x0
//! Constant RESET - Reset the Enqueue Manager
#define CONST_CBM_CTRL_EQM_RST_RESET 0x1

//! @}

//! \defgroup CBM_LL_DBG Register CBM_LL_DBG - CBM Linked List Debug
//! @{

//! Register Offset (relative)
#define CBM_LL_DBG 0x214
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_LL_DBG 0xE6610214u

//! Register Reset Value
#define CBM_LL_DBG_RST 0x7FFF0000u

//! Field ERR_PORT - Error Port Type
#define CBM_LL_DBG_ERR_PORT_POS 1
//! Field ERR_PORT - Error Port Type
#define CBM_LL_DBG_ERR_PORT_MASK 0x2u
//! Constant IGP - Ingress Port
#define CONST_CBM_LL_DBG_ERR_PORT_IGP 0x0
//! Constant EGP - Egress Port
#define CONST_CBM_LL_DBG_ERR_PORT_EGP 0x1

//! Field ERR_EGPID - Error Egress Port ID
#define CBM_LL_DBG_ERR_EGPID_POS 3
//! Field ERR_EGPID - Error Egress Port ID
#define CBM_LL_DBG_ERR_EGPID_MASK 0x7F8u

//! Field ERR_IGPID - Error Ingress Port ID
#define CBM_LL_DBG_ERR_IGPID_POS 11
//! Field ERR_IGPID - Error Ingress Port ID
#define CBM_LL_DBG_ERR_IGPID_MASK 0xF800u

//! Field ERR_LSA - Error LSA
#define CBM_LL_DBG_ERR_LSA_POS 16
//! Field ERR_LSA - Error LSA
#define CBM_LL_DBG_ERR_LSA_MASK 0x7FFF0000u

//! @}

//! \defgroup CBM_ACA_CTRL Register CBM_ACA_CTRL - CBM ACA Port Control Registers
//! @{

//! Register Offset (relative)
#define CBM_ACA_CTRL 0x21C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_ACA_CTRL 0xE661021Cu

//! Register Reset Value
#define CBM_ACA_CTRL_RST 0x00000000u

//! Field ACA_EP0_EN - ACA EP0 Enable
#define CBM_ACA_CTRL_ACA_EP0_EN_POS 0
//! Field ACA_EP0_EN - ACA EP0 Enable
#define CBM_ACA_CTRL_ACA_EP0_EN_MASK 0x1u

//! Field ACA_EP0_MARK_EN - ACA EP0 Marking Enable
#define CBM_ACA_CTRL_ACA_EP0_MARK_EN_POS 1
//! Field ACA_EP0_MARK_EN - ACA EP0 Marking Enable
#define CBM_ACA_CTRL_ACA_EP0_MARK_EN_MASK 0x2u

//! Field ACA_EP1_EN - ACA EP1 Enable
#define CBM_ACA_CTRL_ACA_EP1_EN_POS 4
//! Field ACA_EP1_EN - ACA EP1 Enable
#define CBM_ACA_CTRL_ACA_EP1_EN_MASK 0x10u

//! Field ACA_EP1_MARK_EN - ACA EP1 Marking Enable
#define CBM_ACA_CTRL_ACA_EP1_MARK_EN_POS 5
//! Field ACA_EP1_MARK_EN - ACA EP1 Marking Enable
#define CBM_ACA_CTRL_ACA_EP1_MARK_EN_MASK 0x20u

//! Field ACA_EP2_EN - ACA EP2 Enable
#define CBM_ACA_CTRL_ACA_EP2_EN_POS 8
//! Field ACA_EP2_EN - ACA EP2 Enable
#define CBM_ACA_CTRL_ACA_EP2_EN_MASK 0x100u

//! Field ACA_EP2_MARK_EN - ACA EP2 Marking Enable
#define CBM_ACA_CTRL_ACA_EP2_MARK_EN_POS 9
//! Field ACA_EP2_MARK_EN - ACA EP2 Marking Enable
#define CBM_ACA_CTRL_ACA_EP2_MARK_EN_MASK 0x200u

//! Field ACA_EP3_EN - ACA EP3 Enable
#define CBM_ACA_CTRL_ACA_EP3_EN_POS 12
//! Field ACA_EP3_EN - ACA EP3 Enable
#define CBM_ACA_CTRL_ACA_EP3_EN_MASK 0x1000u

//! Field ACA_EP3_MARK_EN - ACA EP3 Marking Enable
#define CBM_ACA_CTRL_ACA_EP3_MARK_EN_POS 13
//! Field ACA_EP3_MARK_EN - ACA EP3 Marking Enable
#define CBM_ACA_CTRL_ACA_EP3_MARK_EN_MASK 0x2000u

//! Field ACA_EP4_EN - ACA EP4 Enable
#define CBM_ACA_CTRL_ACA_EP4_EN_POS 16
//! Field ACA_EP4_EN - ACA EP4 Enable
#define CBM_ACA_CTRL_ACA_EP4_EN_MASK 0x10000u

//! Field ACA_EP4_MARK_EN - ACA EP4 Marking Enable
#define CBM_ACA_CTRL_ACA_EP4_MARK_EN_POS 17
//! Field ACA_EP4_MARK_EN - ACA EP4 Marking Enable
#define CBM_ACA_CTRL_ACA_EP4_MARK_EN_MASK 0x20000u

//! Field ACA_EP5_EN - ACA EP5 Enable
#define CBM_ACA_CTRL_ACA_EP5_EN_POS 20
//! Field ACA_EP5_EN - ACA EP5 Enable
#define CBM_ACA_CTRL_ACA_EP5_EN_MASK 0x100000u

//! Field ACA_EP5_MARK_EN - ACA EP5 Marking Enable
#define CBM_ACA_CTRL_ACA_EP5_MARK_EN_POS 21
//! Field ACA_EP5_MARK_EN - ACA EP5 Marking Enable
#define CBM_ACA_CTRL_ACA_EP5_MARK_EN_MASK 0x200000u

//! Field ACA_EP6_EN - ACA EP6 Enable
#define CBM_ACA_CTRL_ACA_EP6_EN_POS 24
//! Field ACA_EP6_EN - ACA EP6 Enable
#define CBM_ACA_CTRL_ACA_EP6_EN_MASK 0x1000000u

//! Field ACA_EP6_MARK_EN - ACA EP6 Marking Enable
#define CBM_ACA_CTRL_ACA_EP6_MARK_EN_POS 25
//! Field ACA_EP6_MARK_EN - ACA EP6 Marking Enable
#define CBM_ACA_CTRL_ACA_EP6_MARK_EN_MASK 0x2000000u

//! Field ACA_EP7_EN - ACA EP7 Enable
#define CBM_ACA_CTRL_ACA_EP7_EN_POS 28
//! Field ACA_EP7_EN - ACA EP7 Enable
#define CBM_ACA_CTRL_ACA_EP7_EN_MASK 0x10000000u

//! Field ACA_EP7_MARK_EN - ACA EP7 Marking Enable
#define CBM_ACA_CTRL_ACA_EP7_MARK_EN_POS 29
//! Field ACA_EP7_MARK_EN - ACA EP7 Marking Enable
#define CBM_ACA_CTRL_ACA_EP7_MARK_EN_MASK 0x20000000u

//! @}

//! \defgroup CBM_BSL_CTRL Register CBM_BSL_CTRL - CBM Buffer Selection Logic Control Register
//! @{

//! Register Offset (relative)
#define CBM_BSL_CTRL 0x220
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_BSL_CTRL 0xE6610220u

//! Register Reset Value
#define CBM_BSL_CTRL_RST 0x00000000u

//! Field BSL1_EN - BSL DMA1Rx Enable
#define CBM_BSL_CTRL_BSL1_EN_POS 0
//! Field BSL1_EN - BSL DMA1Rx Enable
#define CBM_BSL_CTRL_BSL1_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_CBM_BSL_CTRL_BSL1_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_CBM_BSL_CTRL_BSL1_EN_EN 0x1

//! Field BSL1_MODE - BSL1 Mode
#define CBM_BSL_CTRL_BSL1_MODE_POS 4
//! Field BSL1_MODE - BSL1 Mode
#define CBM_BSL_CTRL_BSL1_MODE_MASK 0x10u
//! Constant PRIO - Priority Based Selection
#define CONST_CBM_BSL_CTRL_BSL1_MODE_PRIO 0x0
//! Constant WRR - WRR Based Selection
#define CONST_CBM_BSL_CTRL_BSL1_MODE_WRR 0x1

//! Field BSL1_WRR_RATIO - BSL1 WRR Allocation Ratio
#define CBM_BSL_CTRL_BSL1_WRR_RATIO_POS 8
//! Field BSL1_WRR_RATIO - BSL1 WRR Allocation Ratio
#define CBM_BSL_CTRL_BSL1_WRR_RATIO_MASK 0x300u

//! @}

//! \defgroup CBM_BUFFER_METADATA_CTRL Register CBM_BUFFER_METADATA_CTRL - CBM Buffer Metadata Control Register
//! @{

//! Register Offset (relative)
#define CBM_BUFFER_METADATA_CTRL 0x224
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_BUFFER_METADATA_CTRL 0xE6610224u

//! Register Reset Value
#define CBM_BUFFER_METADATA_CTRL_RST 0x00000000u

//! Field OFFSET - Metadata Offset
#define CBM_BUFFER_METADATA_CTRL_OFFSET_POS 0
//! Field OFFSET - Metadata Offset
#define CBM_BUFFER_METADATA_CTRL_OFFSET_MASK 0x1FFu

//! @}

//! \defgroup CBM_POLICY_SEL0_3 Register CBM_POLICY_SEL0_3 - Buffer Policy Select Register
//! @{

//! Register Offset (relative)
#define CBM_POLICY_SEL0_3 0x228
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POLICY_SEL0_3 0xE6610228u

//! Register Reset Value
#define CBM_POLICY_SEL0_3_RST 0x03020100u

//! Field POLICY0 - Buffer Policy for Size0
#define CBM_POLICY_SEL0_3_POLICY0_POS 0
//! Field POLICY0 - Buffer Policy for Size0
#define CBM_POLICY_SEL0_3_POLICY0_MASK 0xFFu

//! Field POLICY1 - Buffer Policy for Size1
#define CBM_POLICY_SEL0_3_POLICY1_POS 8
//! Field POLICY1 - Buffer Policy for Size1
#define CBM_POLICY_SEL0_3_POLICY1_MASK 0xFF00u

//! Field POLICY2 - Buffer Policy for Size2
#define CBM_POLICY_SEL0_3_POLICY2_POS 16
//! Field POLICY2 - Buffer Policy for Size2
#define CBM_POLICY_SEL0_3_POLICY2_MASK 0xFF0000u

//! Field POLICY3 - Buffer Policy for Size3
#define CBM_POLICY_SEL0_3_POLICY3_POS 24
//! Field POLICY3 - Buffer Policy for Size3
#define CBM_POLICY_SEL0_3_POLICY3_MASK 0xFF000000u

//! @}

//! \defgroup CBM_POLICY_SEL4 Register CBM_POLICY_SEL4 - Buffer Policy Select Register
//! @{

//! Register Offset (relative)
#define CBM_POLICY_SEL4 0x22C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POLICY_SEL4 0xE661022Cu

//! Register Reset Value
#define CBM_POLICY_SEL4_RST 0x00000004u

//! Field POLICY4 - Buffer Policy for Generic Pool
#define CBM_POLICY_SEL4_POLICY4_POS 0
//! Field POLICY4 - Buffer Policy for Generic Pool
#define CBM_POLICY_SEL4_POLICY4_MASK 0xFFu

//! @}

//! \defgroup CBM_QID_MODE_SEL_REG_0 Register CBM_QID_MODE_SEL_REG_0 - CBM QID Mode Selection Register
//! @{

//! Register Offset (relative)
#define CBM_QID_MODE_SEL_REG_0 0x230
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_QID_MODE_SEL_REG_0 0xE6610230u

//! Register Reset Value
#define CBM_QID_MODE_SEL_REG_0_RST 0x00000000u

//! Field MODE0 - Mode0 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE0_POS 0
//! Field MODE0 - Mode0 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE0_MASK 0x7u

//! Field MODE1 - Mode1 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE1_POS 4
//! Field MODE1 - Mode1 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE1_MASK 0x70u

//! Field MODE2 - Mode2 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE2_POS 8
//! Field MODE2 - Mode2 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE2_MASK 0x700u

//! Field MODE3 - Mode3 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE3_POS 12
//! Field MODE3 - Mode3 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE3_MASK 0x7000u

//! Field MODE4 - Mode4 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE4_POS 16
//! Field MODE4 - Mode4 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE4_MASK 0x70000u

//! Field MODE5 - Mode5 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE5_POS 20
//! Field MODE5 - Mode5 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE5_MASK 0x700000u

//! Field MODE6 - Mode6 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE6_POS 24
//! Field MODE6 - Mode6 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE6_MASK 0x7000000u

//! Field MODE7 - Mode7 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE7_POS 28
//! Field MODE7 - Mode7 Bits
#define CBM_QID_MODE_SEL_REG_0_MODE7_MASK 0x70000000u

//! @}

//! \defgroup CBM_QID_MODE_SEL_REG_1 Register CBM_QID_MODE_SEL_REG_1 - CBM QID Mode Selection Register
//! @{

//! Register Offset (relative)
#define CBM_QID_MODE_SEL_REG_1 0x234
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_QID_MODE_SEL_REG_1 0xE6610234u

//! Register Reset Value
#define CBM_QID_MODE_SEL_REG_1_RST 0x00000000u

//! Field MODE0 - Mode0 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE0_POS 0
//! Field MODE0 - Mode0 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE0_MASK 0x7u

//! Field MODE1 - Mode1 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE1_POS 4
//! Field MODE1 - Mode1 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE1_MASK 0x70u

//! Field MODE2 - Mode2 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE2_POS 8
//! Field MODE2 - Mode2 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE2_MASK 0x700u

//! Field MODE3 - Mode3 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE3_POS 12
//! Field MODE3 - Mode3 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE3_MASK 0x7000u

//! Field MODE4 - Mode4 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE4_POS 16
//! Field MODE4 - Mode4 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE4_MASK 0x70000u

//! Field MODE5 - Mode5 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE5_POS 20
//! Field MODE5 - Mode5 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE5_MASK 0x700000u

//! Field MODE6 - Mode6 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE6_POS 24
//! Field MODE6 - Mode6 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE6_MASK 0x7000000u

//! Field MODE7 - Mode7 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE7_POS 28
//! Field MODE7 - Mode7 Bits
#define CBM_QID_MODE_SEL_REG_1_MODE7_MASK 0x70000000u

//! @}

//! \defgroup CBM_UMTRX_MUX_SEL Register CBM_UMTRX_MUX_SEL - UMTRx count mux select register
//! @{

//! Register Offset (relative)
#define CBM_UMTRX_MUX_SEL 0x238
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_UMTRX_MUX_SEL 0xE6610238u

//! Register Reset Value
#define CBM_UMTRX_MUX_SEL_RST 0x00000000u

//! Field MUXSEL - MUXSEL
#define CBM_UMTRX_MUX_SEL_MUXSEL_POS 0
//! Field MUXSEL - MUXSEL
#define CBM_UMTRX_MUX_SEL_MUXSEL_MASK 0xFFu

//! Field RXCNTSEL - RX Counter Select
#define CBM_UMTRX_MUX_SEL_RXCNTSEL_POS 15
//! Field RXCNTSEL - RX Counter Select
#define CBM_UMTRX_MUX_SEL_RXCNTSEL_MASK 0x7F8000u

//! @}

//! \defgroup CBM_BM_BASE Register CBM_BM_BASE - Buffer Manager Base Address
//! @{

//! Register Offset (relative)
#define CBM_BM_BASE 0x240
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_BM_BASE 0xE6610240u

//! Register Reset Value
#define CBM_BM_BASE_RST 0x00000000u

//! Field BASE - Base Address
#define CBM_BM_BASE_BASE_POS 13
//! Field BASE - Base Address
#define CBM_BM_BASE_BASE_MASK 0xFFFFE000u

//! @}

//! \defgroup CBM_WRED_BASE Register CBM_WRED_BASE - WRED Query Base Address
//! @{

//! Register Offset (relative)
#define CBM_WRED_BASE 0x244
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_WRED_BASE 0xE6610244u

//! Register Reset Value
#define CBM_WRED_BASE_RST 0x00000000u

//! Field BASE - Base Address
#define CBM_WRED_BASE_BASE_POS 9
//! Field BASE - Base Address
#define CBM_WRED_BASE_BASE_MASK 0xFFFFFE00u

//! @}

//! \defgroup CBM_QPUSH_BASE Register CBM_QPUSH_BASE - Queue Push Base Address
//! @{

//! Register Offset (relative)
#define CBM_QPUSH_BASE 0x248
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_QPUSH_BASE 0xE6610248u

//! Register Reset Value
#define CBM_QPUSH_BASE_RST 0x00000000u

//! Field BASE - Base Address
#define CBM_QPUSH_BASE_BASE_POS 9
//! Field BASE - Base Address
#define CBM_QPUSH_BASE_BASE_MASK 0xFFFFFE00u

//! @}

//! \defgroup CBM_TX_CREDIT_BASE Register CBM_TX_CREDIT_BASE - TX Credit Base Address
//! @{

//! Register Offset (relative)
#define CBM_TX_CREDIT_BASE 0x24C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_TX_CREDIT_BASE 0xE661024Cu

//! Register Reset Value
#define CBM_TX_CREDIT_BASE_RST 0x00000000u

//! Field BASE - Base Address
#define CBM_TX_CREDIT_BASE_BASE_POS 8
//! Field BASE - Base Address
#define CBM_TX_CREDIT_BASE_BASE_MASK 0xFFFFFF00u

//! @}

//! \defgroup CBM_EPONL_BASE Register CBM_EPONL_BASE - EPON Descriptor Write Lower Base Address
//! @{

//! Register Offset (relative)
#define CBM_EPONL_BASE 0x250
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_EPONL_BASE 0xE6610250u

//! Register Reset Value
#define CBM_EPONL_BASE_RST 0x00000000u

//! Field BASE - Base Address
#define CBM_EPONL_BASE_BASE_POS 0
//! Field BASE - Base Address
#define CBM_EPONL_BASE_BASE_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_TXPUSH_DMA_QOSBYPASS_BASE Register CBM_TXPUSH_DMA_QOSBYPASS_BASE - Tx Push Base address for QoS Bypass Mode
//! @{

//! Register Offset (relative)
#define CBM_TXPUSH_DMA_QOSBYPASS_BASE 0x254
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_TXPUSH_DMA_QOSBYPASS_BASE 0xE6610254u

//! Register Reset Value
#define CBM_TXPUSH_DMA_QOSBYPASS_BASE_RST 0x00000000u

//! Field BASE - Base Address
#define CBM_TXPUSH_DMA_QOSBYPASS_BASE_BASE_POS 4
//! Field BASE - Base Address
#define CBM_TXPUSH_DMA_QOSBYPASS_BASE_BASE_MASK 0xFFFFFFF0u

//! @}

//! \defgroup CBM_EPONH_BASE Register CBM_EPONH_BASE - EPON Descriptor Write High Base Address
//! @{

//! Register Offset (relative)
#define CBM_EPONH_BASE 0x258
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_EPONH_BASE 0xE6610258u

//! Register Reset Value
#define CBM_EPONH_BASE_RST 0x00000000u

//! Field BASE - Base Address
#define CBM_EPONH_BASE_BASE_POS 0
//! Field BASE - Base Address
#define CBM_EPONH_BASE_BASE_MASK 0xFu

//! @}

//! \defgroup CBM_WRED_RESP_BASE Register CBM_WRED_RESP_BASE - WRED Response Base address
//! @{

//! Register Offset (relative)
#define CBM_WRED_RESP_BASE 0x25C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_WRED_RESP_BASE 0xE661025Cu

//! Register Reset Value
#define CBM_WRED_RESP_BASE_RST 0x00000000u

//! Field BASE - Base Address
#define CBM_WRED_RESP_BASE_BASE_POS 7
//! Field BASE - Base Address
#define CBM_WRED_RESP_BASE_BASE_MASK 0xFFFFFF80u

//! @}

//! \defgroup CBM_VM_POOL_CHK_REG_0 Register CBM_VM_POOL_CHK_REG_0 - CBM VM Ports Pool Check Register
//! @{

//! Register Offset (relative)
#define CBM_VM_POOL_CHK_REG_0 0x260
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_VM_POOL_CHK_REG_0 0xE6610260u

//! Register Reset Value
#define CBM_VM_POOL_CHK_REG_0_RST 0x00000000u

//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_0_POLICYID_POS 0
//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_0_POLICYID_MASK 0xFFu

//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_0_POOLID_POS 8
//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_0_POOLID_MASK 0xF00u

//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_0_CHKEN_POS 15
//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_0_CHKEN_MASK 0x8000u

//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_0_VMMODEN_POS 24
//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_0_VMMODEN_MASK 0x1000000u

//! @}

//! \defgroup CBM_VM_POOL_CHK_REG_1 Register CBM_VM_POOL_CHK_REG_1 - CBM VM Ports Pool Check Register
//! @{

//! Register Offset (relative)
#define CBM_VM_POOL_CHK_REG_1 0x264
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_VM_POOL_CHK_REG_1 0xE6610264u

//! Register Reset Value
#define CBM_VM_POOL_CHK_REG_1_RST 0x00000000u

//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_1_POLICYID_POS 0
//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_1_POLICYID_MASK 0xFFu

//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_1_POOLID_POS 8
//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_1_POOLID_MASK 0xF00u

//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_1_CHKEN_POS 15
//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_1_CHKEN_MASK 0x8000u

//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_1_VMMODEN_POS 24
//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_1_VMMODEN_MASK 0x1000000u

//! @}

//! \defgroup CBM_VM_POOL_CHK_REG_2 Register CBM_VM_POOL_CHK_REG_2 - CBM VM Ports Pool Check Register
//! @{

//! Register Offset (relative)
#define CBM_VM_POOL_CHK_REG_2 0x268
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_VM_POOL_CHK_REG_2 0xE6610268u

//! Register Reset Value
#define CBM_VM_POOL_CHK_REG_2_RST 0x00000000u

//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_2_POLICYID_POS 0
//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_2_POLICYID_MASK 0xFFu

//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_2_POOLID_POS 8
//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_2_POOLID_MASK 0xF00u

//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_2_CHKEN_POS 15
//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_2_CHKEN_MASK 0x8000u

//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_2_VMMODEN_POS 24
//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_2_VMMODEN_MASK 0x1000000u

//! @}

//! \defgroup CBM_VM_POOL_CHK_REG_3 Register CBM_VM_POOL_CHK_REG_3 - CBM VM Ports Pool Check Register
//! @{

//! Register Offset (relative)
#define CBM_VM_POOL_CHK_REG_3 0x26C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_VM_POOL_CHK_REG_3 0xE661026Cu

//! Register Reset Value
#define CBM_VM_POOL_CHK_REG_3_RST 0x00000000u

//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_3_POLICYID_POS 0
//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_3_POLICYID_MASK 0xFFu

//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_3_POOLID_POS 8
//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_3_POOLID_MASK 0xF00u

//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_3_CHKEN_POS 15
//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_3_CHKEN_MASK 0x8000u

//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_3_VMMODEN_POS 24
//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_3_VMMODEN_MASK 0x1000000u

//! @}

//! \defgroup CBM_VM_POOL_CHK_REG_4 Register CBM_VM_POOL_CHK_REG_4 - CBM VM Ports Pool Check Register
//! @{

//! Register Offset (relative)
#define CBM_VM_POOL_CHK_REG_4 0x270
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_VM_POOL_CHK_REG_4 0xE6610270u

//! Register Reset Value
#define CBM_VM_POOL_CHK_REG_4_RST 0x00000000u

//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_4_POLICYID_POS 0
//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_4_POLICYID_MASK 0xFFu

//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_4_POOLID_POS 8
//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_4_POOLID_MASK 0xF00u

//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_4_CHKEN_POS 15
//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_4_CHKEN_MASK 0x8000u

//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_4_VMMODEN_POS 24
//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_4_VMMODEN_MASK 0x1000000u

//! @}

//! \defgroup CBM_VM_POOL_CHK_REG_5 Register CBM_VM_POOL_CHK_REG_5 - CBM VM Ports Pool Check Register
//! @{

//! Register Offset (relative)
#define CBM_VM_POOL_CHK_REG_5 0x274
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_VM_POOL_CHK_REG_5 0xE6610274u

//! Register Reset Value
#define CBM_VM_POOL_CHK_REG_5_RST 0x00000000u

//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_5_POLICYID_POS 0
//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_5_POLICYID_MASK 0xFFu

//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_5_POOLID_POS 8
//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_5_POOLID_MASK 0xF00u

//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_5_CHKEN_POS 15
//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_5_CHKEN_MASK 0x8000u

//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_5_VMMODEN_POS 24
//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_5_VMMODEN_MASK 0x1000000u

//! @}

//! \defgroup CBM_VM_POOL_CHK_REG_6 Register CBM_VM_POOL_CHK_REG_6 - CBM VM Ports Pool Check Register
//! @{

//! Register Offset (relative)
#define CBM_VM_POOL_CHK_REG_6 0x278
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_VM_POOL_CHK_REG_6 0xE6610278u

//! Register Reset Value
#define CBM_VM_POOL_CHK_REG_6_RST 0x00000000u

//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_6_POLICYID_POS 0
//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_6_POLICYID_MASK 0xFFu

//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_6_POOLID_POS 8
//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_6_POOLID_MASK 0xF00u

//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_6_CHKEN_POS 15
//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_6_CHKEN_MASK 0x8000u

//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_6_VMMODEN_POS 24
//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_6_VMMODEN_MASK 0x1000000u

//! @}

//! \defgroup CBM_VM_POOL_CHK_REG_7 Register CBM_VM_POOL_CHK_REG_7 - CBM VM Ports Pool Check Register
//! @{

//! Register Offset (relative)
#define CBM_VM_POOL_CHK_REG_7 0x27C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_VM_POOL_CHK_REG_7 0xE661027Cu

//! Register Reset Value
#define CBM_VM_POOL_CHK_REG_7_RST 0x00000000u

//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_7_POLICYID_POS 0
//! Field POLICYID - Policy ID for the VM
#define CBM_VM_POOL_CHK_REG_7_POLICYID_MASK 0xFFu

//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_7_POOLID_POS 8
//! Field POOLID - Pool ID for the VM
#define CBM_VM_POOL_CHK_REG_7_POOLID_MASK 0xF00u

//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_7_CHKEN_POS 15
//! Field CHKEN - Check Enable
#define CBM_VM_POOL_CHK_REG_7_CHKEN_MASK 0x8000u

//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_7_VMMODEN_POS 24
//! Field VMMODEN - VM Mode enable
#define CBM_VM_POOL_CHK_REG_7_VMMODEN_MASK 0x1000000u

//! @}

//! \defgroup CBM_CPU_POOL_BUF_RTRN_START_ADDR Register CBM_CPU_POOL_BUF_RTRN_START_ADDR - CPU Pool Buffer Return Start Address
//! @{

//! Register Offset (relative)
#define CBM_CPU_POOL_BUF_RTRN_START_ADDR 0x280
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_CPU_POOL_BUF_RTRN_START_ADDR 0xE6610280u

//! Register Reset Value
#define CBM_CPU_POOL_BUF_RTRN_START_ADDR_RST 0x00000000u

//! Field ADDR - Start Address
#define CBM_CPU_POOL_BUF_RTRN_START_ADDR_ADDR_POS 0
//! Field ADDR - Start Address
#define CBM_CPU_POOL_BUF_RTRN_START_ADDR_ADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_CPU_POOL_BUF_ALW_NUM Register CBM_CPU_POOL_BUF_ALW_NUM - CPU Pool Buffer Allowed Number
//! @{

//! Register Offset (relative)
#define CBM_CPU_POOL_BUF_ALW_NUM 0x284
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_CPU_POOL_BUF_ALW_NUM 0xE6610284u

//! Register Reset Value
#define CBM_CPU_POOL_BUF_ALW_NUM_RST 0x00000000u

//! Field NUMBER - Number
#define CBM_CPU_POOL_BUF_ALW_NUM_NUMBER_POS 0
//! Field NUMBER - Number
#define CBM_CPU_POOL_BUF_ALW_NUM_NUMBER_MASK 0xFFFFu

//! @}

//! \defgroup CBM_CPU_POOL_ENQ_CNT Register CBM_CPU_POOL_ENQ_CNT - CBM CPU Pool Enqueue Count
//! @{

//! Register Offset (relative)
#define CBM_CPU_POOL_ENQ_CNT 0x288
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_CPU_POOL_ENQ_CNT 0xE6610288u

//! Register Reset Value
#define CBM_CPU_POOL_ENQ_CNT_RST 0x00000000u

//! Field COUNT - Count
#define CBM_CPU_POOL_ENQ_CNT_COUNT_POS 0
//! Field COUNT - Count
#define CBM_CPU_POOL_ENQ_CNT_COUNT_MASK 0xFFFFu

//! Field STS - Status
#define CBM_CPU_POOL_ENQ_CNT_STS_POS 16
//! Field STS - Status
#define CBM_CPU_POOL_ENQ_CNT_STS_MASK 0x10000u
//! Constant NA - Not Allowed
#define CONST_CBM_CPU_POOL_ENQ_CNT_STS_NA 0x0
//! Constant ALLOW - Allowed
#define CONST_CBM_CPU_POOL_ENQ_CNT_STS_ALLOW 0x1

//! @}

//! \defgroup CBM_CPU_POOL_BUF_RTRN_CNT Register CBM_CPU_POOL_BUF_RTRN_CNT - CBM CPU Pool Buffer Return Count
//! @{

//! Register Offset (relative)
#define CBM_CPU_POOL_BUF_RTRN_CNT 0x28C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_CPU_POOL_BUF_RTRN_CNT 0xE661028Cu

//! Register Reset Value
#define CBM_CPU_POOL_BUF_RTRN_CNT_RST 0x00000000u

//! Field COUNT - Count
#define CBM_CPU_POOL_BUF_RTRN_CNT_COUNT_POS 0
//! Field COUNT - Count
#define CBM_CPU_POOL_BUF_RTRN_CNT_COUNT_MASK 0xFFFFu

//! @}

//! \defgroup CBM_CPU_POOL_ENQ_DEC Register CBM_CPU_POOL_ENQ_DEC - CPU POOL ENQUEUE DECrement
//! @{

//! Register Offset (relative)
#define CBM_CPU_POOL_ENQ_DEC 0x290
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_CPU_POOL_ENQ_DEC 0xE6610290u

//! Register Reset Value
#define CBM_CPU_POOL_ENQ_DEC_RST 0x00000000u

//! Field DEC - Count Decrement
#define CBM_CPU_POOL_ENQ_DEC_DEC_POS 0
//! Field DEC - Count Decrement
#define CBM_CPU_POOL_ENQ_DEC_DEC_MASK 0xFFFFu

//! @}

//! \defgroup CBM_FSQM_QUEUE_REG_0 Register CBM_FSQM_QUEUE_REG_0 - FSQM Queue Configuration Register
//! @{

//! Register Offset (relative)
#define CBM_FSQM_QUEUE_REG_0 0x2A0
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_FSQM_QUEUE_REG_0 0xE66102A0u

//! Register Reset Value
#define CBM_FSQM_QUEUE_REG_0_RST 0x0000FFFFu

//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_0_QSTARTLSA_POS 0
//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_0_QSTARTLSA_MASK 0xFFFFu

//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_0_QINITDONE_POS 31
//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_0_QINITDONE_MASK 0x80000000u

//! @}

//! \defgroup CBM_FSQM_QUEUE_REG_1 Register CBM_FSQM_QUEUE_REG_1 - FSQM Queue Configuration Register
//! @{

//! Register Offset (relative)
#define CBM_FSQM_QUEUE_REG_1 0x2A4
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_FSQM_QUEUE_REG_1 0xE66102A4u

//! Register Reset Value
#define CBM_FSQM_QUEUE_REG_1_RST 0x0000FFFFu

//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_1_QSTARTLSA_POS 0
//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_1_QSTARTLSA_MASK 0xFFFFu

//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_1_QINITDONE_POS 31
//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_1_QINITDONE_MASK 0x80000000u

//! @}

//! \defgroup CBM_FSQM_QUEUE_REG_2 Register CBM_FSQM_QUEUE_REG_2 - FSQM Queue Configuration Register
//! @{

//! Register Offset (relative)
#define CBM_FSQM_QUEUE_REG_2 0x2A8
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_FSQM_QUEUE_REG_2 0xE66102A8u

//! Register Reset Value
#define CBM_FSQM_QUEUE_REG_2_RST 0x0000FFFFu

//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_2_QSTARTLSA_POS 0
//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_2_QSTARTLSA_MASK 0xFFFFu

//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_2_QINITDONE_POS 31
//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_2_QINITDONE_MASK 0x80000000u

//! @}

//! \defgroup CBM_FSQM_QUEUE_REG_3 Register CBM_FSQM_QUEUE_REG_3 - FSQM Queue Configuration Register
//! @{

//! Register Offset (relative)
#define CBM_FSQM_QUEUE_REG_3 0x2AC
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_FSQM_QUEUE_REG_3 0xE66102ACu

//! Register Reset Value
#define CBM_FSQM_QUEUE_REG_3_RST 0x0000FFFFu

//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_3_QSTARTLSA_POS 0
//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_3_QSTARTLSA_MASK 0xFFFFu

//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_3_QINITDONE_POS 31
//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_3_QINITDONE_MASK 0x80000000u

//! @}

//! \defgroup CBM_FSQM_QUEUE_REG_4 Register CBM_FSQM_QUEUE_REG_4 - FSQM Queue Configuration Register
//! @{

//! Register Offset (relative)
#define CBM_FSQM_QUEUE_REG_4 0x2B0
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_FSQM_QUEUE_REG_4 0xE66102B0u

//! Register Reset Value
#define CBM_FSQM_QUEUE_REG_4_RST 0x0000FFFFu

//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_4_QSTARTLSA_POS 0
//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_4_QSTARTLSA_MASK 0xFFFFu

//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_4_QINITDONE_POS 31
//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_4_QINITDONE_MASK 0x80000000u

//! @}

//! \defgroup CBM_FSQM_QUEUE_REG_5 Register CBM_FSQM_QUEUE_REG_5 - FSQM Queue Configuration Register
//! @{

//! Register Offset (relative)
#define CBM_FSQM_QUEUE_REG_5 0x2B4
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_FSQM_QUEUE_REG_5 0xE66102B4u

//! Register Reset Value
#define CBM_FSQM_QUEUE_REG_5_RST 0x0000FFFFu

//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_5_QSTARTLSA_POS 0
//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_5_QSTARTLSA_MASK 0xFFFFu

//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_5_QINITDONE_POS 31
//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_5_QINITDONE_MASK 0x80000000u

//! @}

//! \defgroup CBM_FSQM_QUEUE_REG_6 Register CBM_FSQM_QUEUE_REG_6 - FSQM Queue Configuration Register
//! @{

//! Register Offset (relative)
#define CBM_FSQM_QUEUE_REG_6 0x2B8
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_FSQM_QUEUE_REG_6 0xE66102B8u

//! Register Reset Value
#define CBM_FSQM_QUEUE_REG_6_RST 0x0000FFFFu

//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_6_QSTARTLSA_POS 0
//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_6_QSTARTLSA_MASK 0xFFFFu

//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_6_QINITDONE_POS 31
//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_6_QINITDONE_MASK 0x80000000u

//! @}

//! \defgroup CBM_FSQM_QUEUE_REG_7 Register CBM_FSQM_QUEUE_REG_7 - FSQM Queue Configuration Register
//! @{

//! Register Offset (relative)
#define CBM_FSQM_QUEUE_REG_7 0x2BC
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_FSQM_QUEUE_REG_7 0xE66102BCu

//! Register Reset Value
#define CBM_FSQM_QUEUE_REG_7_RST 0x0000FFFFu

//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_7_QSTARTLSA_POS 0
//! Field QSTARTLSA - Queue Start LSA
#define CBM_FSQM_QUEUE_REG_7_QSTARTLSA_MASK 0xFFFFu

//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_7_QINITDONE_POS 31
//! Field QINITDONE - Queue Initialization Status
#define CBM_FSQM_QUEUE_REG_7_QINITDONE_MASK 0x80000000u

//! @}

//! \defgroup CBM_POOL_START_ADDR_0 Register CBM_POOL_START_ADDR_0 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_0 0x300
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_0 0xE6610300u

//! Register Reset Value
#define CBM_POOL_START_ADDR_0_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_0_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_0_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_1 Register CBM_POOL_START_ADDR_1 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_1 0x304
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_1 0xE6610304u

//! Register Reset Value
#define CBM_POOL_START_ADDR_1_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_1_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_1_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_2 Register CBM_POOL_START_ADDR_2 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_2 0x308
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_2 0xE6610308u

//! Register Reset Value
#define CBM_POOL_START_ADDR_2_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_2_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_2_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_3 Register CBM_POOL_START_ADDR_3 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_3 0x30C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_3 0xE661030Cu

//! Register Reset Value
#define CBM_POOL_START_ADDR_3_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_3_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_3_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_4 Register CBM_POOL_START_ADDR_4 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_4 0x310
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_4 0xE6610310u

//! Register Reset Value
#define CBM_POOL_START_ADDR_4_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_4_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_4_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_5 Register CBM_POOL_START_ADDR_5 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_5 0x314
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_5 0xE6610314u

//! Register Reset Value
#define CBM_POOL_START_ADDR_5_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_5_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_5_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_6 Register CBM_POOL_START_ADDR_6 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_6 0x318
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_6 0xE6610318u

//! Register Reset Value
#define CBM_POOL_START_ADDR_6_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_6_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_6_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_7 Register CBM_POOL_START_ADDR_7 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_7 0x31C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_7 0xE661031Cu

//! Register Reset Value
#define CBM_POOL_START_ADDR_7_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_7_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_7_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_8 Register CBM_POOL_START_ADDR_8 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_8 0x320
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_8 0xE6610320u

//! Register Reset Value
#define CBM_POOL_START_ADDR_8_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_8_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_8_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_9 Register CBM_POOL_START_ADDR_9 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_9 0x324
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_9 0xE6610324u

//! Register Reset Value
#define CBM_POOL_START_ADDR_9_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_9_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_9_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_10 Register CBM_POOL_START_ADDR_10 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_10 0x328
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_10 0xE6610328u

//! Register Reset Value
#define CBM_POOL_START_ADDR_10_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_10_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_10_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_11 Register CBM_POOL_START_ADDR_11 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_11 0x32C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_11 0xE661032Cu

//! Register Reset Value
#define CBM_POOL_START_ADDR_11_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_11_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_11_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_12 Register CBM_POOL_START_ADDR_12 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_12 0x330
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_12 0xE6610330u

//! Register Reset Value
#define CBM_POOL_START_ADDR_12_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_12_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_12_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_13 Register CBM_POOL_START_ADDR_13 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_13 0x334
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_13 0xE6610334u

//! Register Reset Value
#define CBM_POOL_START_ADDR_13_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_13_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_13_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_14 Register CBM_POOL_START_ADDR_14 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_14 0x338
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_14 0xE6610338u

//! Register Reset Value
#define CBM_POOL_START_ADDR_14_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_14_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_14_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_START_ADDR_15 Register CBM_POOL_START_ADDR_15 - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_START_ADDR_15 0x33C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_START_ADDR_15 0xE661033Cu

//! Register Reset Value
#define CBM_POOL_START_ADDR_15_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_15_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_POOL_START_ADDR_15_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_0 Register CBM_POOL_END_ADDR_0 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_0 0x340
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_0 0xE6610340u

//! Register Reset Value
#define CBM_POOL_END_ADDR_0_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_0_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_0_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_1 Register CBM_POOL_END_ADDR_1 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_1 0x344
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_1 0xE6610344u

//! Register Reset Value
#define CBM_POOL_END_ADDR_1_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_1_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_1_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_2 Register CBM_POOL_END_ADDR_2 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_2 0x348
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_2 0xE6610348u

//! Register Reset Value
#define CBM_POOL_END_ADDR_2_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_2_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_2_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_3 Register CBM_POOL_END_ADDR_3 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_3 0x34C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_3 0xE661034Cu

//! Register Reset Value
#define CBM_POOL_END_ADDR_3_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_3_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_3_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_4 Register CBM_POOL_END_ADDR_4 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_4 0x350
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_4 0xE6610350u

//! Register Reset Value
#define CBM_POOL_END_ADDR_4_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_4_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_4_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_5 Register CBM_POOL_END_ADDR_5 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_5 0x354
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_5 0xE6610354u

//! Register Reset Value
#define CBM_POOL_END_ADDR_5_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_5_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_5_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_6 Register CBM_POOL_END_ADDR_6 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_6 0x358
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_6 0xE6610358u

//! Register Reset Value
#define CBM_POOL_END_ADDR_6_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_6_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_6_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_7 Register CBM_POOL_END_ADDR_7 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_7 0x35C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_7 0xE661035Cu

//! Register Reset Value
#define CBM_POOL_END_ADDR_7_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_7_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_7_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_8 Register CBM_POOL_END_ADDR_8 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_8 0x360
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_8 0xE6610360u

//! Register Reset Value
#define CBM_POOL_END_ADDR_8_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_8_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_8_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_9 Register CBM_POOL_END_ADDR_9 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_9 0x364
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_9 0xE6610364u

//! Register Reset Value
#define CBM_POOL_END_ADDR_9_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_9_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_9_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_10 Register CBM_POOL_END_ADDR_10 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_10 0x368
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_10 0xE6610368u

//! Register Reset Value
#define CBM_POOL_END_ADDR_10_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_10_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_10_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_11 Register CBM_POOL_END_ADDR_11 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_11 0x36C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_11 0xE661036Cu

//! Register Reset Value
#define CBM_POOL_END_ADDR_11_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_11_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_11_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_12 Register CBM_POOL_END_ADDR_12 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_12 0x370
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_12 0xE6610370u

//! Register Reset Value
#define CBM_POOL_END_ADDR_12_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_12_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_12_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_13 Register CBM_POOL_END_ADDR_13 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_13 0x374
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_13 0xE6610374u

//! Register Reset Value
#define CBM_POOL_END_ADDR_13_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_13_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_13_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_14 Register CBM_POOL_END_ADDR_14 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_14 0x378
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_14 0xE6610378u

//! Register Reset Value
#define CBM_POOL_END_ADDR_14_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_14_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_14_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_POOL_END_ADDR_15 Register CBM_POOL_END_ADDR_15 - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_POOL_END_ADDR_15 0x37C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_POOL_END_ADDR_15 0xE661037Cu

//! Register Reset Value
#define CBM_POOL_END_ADDR_15_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_15_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_POOL_END_ADDR_15_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_BM_BUFFER_START_ADDR Register CBM_BM_BUFFER_START_ADDR - Pool Start Address
//! @{

//! Register Offset (relative)
#define CBM_BM_BUFFER_START_ADDR 0x380
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_BM_BUFFER_START_ADDR 0xE6610380u

//! Register Reset Value
#define CBM_BM_BUFFER_START_ADDR_RST 0x00000000u

//! Field STARTADDR - Start Address
#define CBM_BM_BUFFER_START_ADDR_STARTADDR_POS 0
//! Field STARTADDR - Start Address
#define CBM_BM_BUFFER_START_ADDR_STARTADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_BM_BUFFER_END_ADDR Register CBM_BM_BUFFER_END_ADDR - Pool End Address
//! @{

//! Register Offset (relative)
#define CBM_BM_BUFFER_END_ADDR 0x384
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_BM_BUFFER_END_ADDR 0xE6610384u

//! Register Reset Value
#define CBM_BM_BUFFER_END_ADDR_RST 0x00000000u

//! Field ENDADDR - End Address
#define CBM_BM_BUFFER_END_ADDR_ENDADDR_POS 0
//! Field ENDADDR - End Address
#define CBM_BM_BUFFER_END_ADDR_ENDADDR_MASK 0xFFFFFFFFu

//! @}

//! \defgroup CBM_GPID_LPID_MAP_0 Register CBM_GPID_LPID_MAP_0 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_0 0x400
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_0 0xE6610400u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_0_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_0_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_0_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_0_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_0_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_0_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_0_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_0_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_0_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_0_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_0_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_0_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_0_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_0_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_0_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_0_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_0_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_1 Register CBM_GPID_LPID_MAP_1 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_1 0x404
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_1 0xE6610404u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_1_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_1_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_1_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_1_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_1_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_1_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_1_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_1_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_1_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_1_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_1_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_1_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_1_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_1_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_1_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_1_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_1_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_2 Register CBM_GPID_LPID_MAP_2 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_2 0x408
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_2 0xE6610408u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_2_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_2_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_2_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_2_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_2_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_2_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_2_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_2_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_2_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_2_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_2_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_2_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_2_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_2_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_2_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_2_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_2_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_3 Register CBM_GPID_LPID_MAP_3 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_3 0x40C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_3 0xE661040Cu

//! Register Reset Value
#define CBM_GPID_LPID_MAP_3_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_3_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_3_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_3_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_3_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_3_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_3_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_3_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_3_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_3_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_3_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_3_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_3_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_3_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_3_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_3_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_3_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_4 Register CBM_GPID_LPID_MAP_4 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_4 0x410
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_4 0xE6610410u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_4_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_4_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_4_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_4_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_4_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_4_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_4_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_4_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_4_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_4_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_4_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_4_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_4_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_4_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_4_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_4_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_4_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_5 Register CBM_GPID_LPID_MAP_5 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_5 0x414
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_5 0xE6610414u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_5_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_5_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_5_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_5_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_5_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_5_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_5_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_5_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_5_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_5_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_5_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_5_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_5_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_5_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_5_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_5_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_5_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_6 Register CBM_GPID_LPID_MAP_6 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_6 0x418
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_6 0xE6610418u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_6_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_6_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_6_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_6_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_6_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_6_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_6_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_6_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_6_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_6_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_6_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_6_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_6_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_6_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_6_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_6_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_6_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_7 Register CBM_GPID_LPID_MAP_7 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_7 0x41C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_7 0xE661041Cu

//! Register Reset Value
#define CBM_GPID_LPID_MAP_7_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_7_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_7_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_7_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_7_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_7_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_7_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_7_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_7_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_7_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_7_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_7_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_7_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_7_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_7_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_7_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_7_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_8 Register CBM_GPID_LPID_MAP_8 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_8 0x420
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_8 0xE6610420u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_8_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_8_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_8_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_8_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_8_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_8_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_8_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_8_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_8_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_8_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_8_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_8_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_8_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_8_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_8_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_8_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_8_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_9 Register CBM_GPID_LPID_MAP_9 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_9 0x424
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_9 0xE6610424u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_9_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_9_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_9_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_9_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_9_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_9_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_9_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_9_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_9_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_9_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_9_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_9_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_9_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_9_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_9_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_9_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_9_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_10 Register CBM_GPID_LPID_MAP_10 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_10 0x428
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_10 0xE6610428u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_10_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_10_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_10_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_10_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_10_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_10_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_10_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_10_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_10_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_10_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_10_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_10_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_10_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_10_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_10_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_10_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_10_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_11 Register CBM_GPID_LPID_MAP_11 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_11 0x42C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_11 0xE661042Cu

//! Register Reset Value
#define CBM_GPID_LPID_MAP_11_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_11_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_11_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_11_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_11_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_11_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_11_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_11_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_11_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_11_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_11_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_11_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_11_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_11_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_11_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_11_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_11_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_12 Register CBM_GPID_LPID_MAP_12 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_12 0x430
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_12 0xE6610430u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_12_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_12_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_12_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_12_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_12_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_12_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_12_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_12_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_12_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_12_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_12_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_12_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_12_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_12_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_12_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_12_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_12_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_13 Register CBM_GPID_LPID_MAP_13 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_13 0x434
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_13 0xE6610434u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_13_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_13_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_13_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_13_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_13_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_13_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_13_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_13_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_13_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_13_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_13_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_13_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_13_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_13_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_13_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_13_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_13_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_14 Register CBM_GPID_LPID_MAP_14 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_14 0x438
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_14 0xE6610438u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_14_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_14_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_14_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_14_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_14_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_14_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_14_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_14_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_14_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_14_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_14_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_14_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_14_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_14_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_14_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_14_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_14_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_15 Register CBM_GPID_LPID_MAP_15 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_15 0x43C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_15 0xE661043Cu

//! Register Reset Value
#define CBM_GPID_LPID_MAP_15_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_15_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_15_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_15_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_15_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_15_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_15_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_15_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_15_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_15_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_15_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_15_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_15_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_15_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_15_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_15_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_15_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_16 Register CBM_GPID_LPID_MAP_16 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_16 0x440
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_16 0xE6610440u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_16_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_16_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_16_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_16_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_16_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_16_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_16_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_16_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_16_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_16_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_16_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_16_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_16_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_16_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_16_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_16_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_16_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_17 Register CBM_GPID_LPID_MAP_17 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_17 0x444
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_17 0xE6610444u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_17_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_17_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_17_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_17_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_17_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_17_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_17_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_17_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_17_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_17_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_17_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_17_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_17_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_17_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_17_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_17_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_17_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_18 Register CBM_GPID_LPID_MAP_18 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_18 0x448
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_18 0xE6610448u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_18_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_18_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_18_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_18_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_18_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_18_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_18_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_18_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_18_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_18_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_18_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_18_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_18_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_18_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_18_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_18_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_18_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_19 Register CBM_GPID_LPID_MAP_19 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_19 0x44C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_19 0xE661044Cu

//! Register Reset Value
#define CBM_GPID_LPID_MAP_19_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_19_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_19_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_19_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_19_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_19_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_19_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_19_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_19_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_19_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_19_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_19_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_19_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_19_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_19_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_19_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_19_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_20 Register CBM_GPID_LPID_MAP_20 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_20 0x450
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_20 0xE6610450u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_20_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_20_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_20_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_20_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_20_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_20_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_20_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_20_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_20_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_20_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_20_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_20_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_20_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_20_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_20_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_20_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_20_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_21 Register CBM_GPID_LPID_MAP_21 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_21 0x454
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_21 0xE6610454u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_21_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_21_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_21_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_21_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_21_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_21_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_21_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_21_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_21_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_21_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_21_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_21_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_21_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_21_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_21_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_21_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_21_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_22 Register CBM_GPID_LPID_MAP_22 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_22 0x458
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_22 0xE6610458u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_22_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_22_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_22_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_22_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_22_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_22_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_22_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_22_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_22_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_22_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_22_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_22_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_22_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_22_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_22_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_22_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_22_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_23 Register CBM_GPID_LPID_MAP_23 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_23 0x45C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_23 0xE661045Cu

//! Register Reset Value
#define CBM_GPID_LPID_MAP_23_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_23_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_23_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_23_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_23_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_23_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_23_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_23_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_23_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_23_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_23_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_23_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_23_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_23_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_23_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_23_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_23_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_24 Register CBM_GPID_LPID_MAP_24 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_24 0x460
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_24 0xE6610460u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_24_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_24_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_24_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_24_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_24_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_24_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_24_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_24_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_24_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_24_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_24_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_24_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_24_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_24_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_24_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_24_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_24_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_25 Register CBM_GPID_LPID_MAP_25 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_25 0x464
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_25 0xE6610464u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_25_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_25_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_25_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_25_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_25_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_25_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_25_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_25_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_25_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_25_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_25_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_25_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_25_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_25_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_25_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_25_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_25_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_26 Register CBM_GPID_LPID_MAP_26 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_26 0x468
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_26 0xE6610468u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_26_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_26_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_26_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_26_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_26_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_26_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_26_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_26_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_26_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_26_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_26_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_26_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_26_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_26_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_26_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_26_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_26_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_27 Register CBM_GPID_LPID_MAP_27 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_27 0x46C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_27 0xE661046Cu

//! Register Reset Value
#define CBM_GPID_LPID_MAP_27_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_27_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_27_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_27_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_27_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_27_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_27_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_27_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_27_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_27_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_27_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_27_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_27_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_27_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_27_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_27_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_27_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_28 Register CBM_GPID_LPID_MAP_28 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_28 0x470
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_28 0xE6610470u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_28_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_28_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_28_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_28_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_28_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_28_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_28_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_28_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_28_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_28_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_28_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_28_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_28_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_28_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_28_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_28_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_28_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_29 Register CBM_GPID_LPID_MAP_29 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_29 0x474
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_29 0xE6610474u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_29_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_29_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_29_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_29_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_29_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_29_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_29_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_29_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_29_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_29_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_29_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_29_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_29_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_29_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_29_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_29_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_29_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_30 Register CBM_GPID_LPID_MAP_30 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_30 0x478
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_30 0xE6610478u

//! Register Reset Value
#define CBM_GPID_LPID_MAP_30_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_30_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_30_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_30_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_30_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_30_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_30_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_30_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_30_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_30_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_30_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_30_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_30_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_30_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_30_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_30_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_30_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_GPID_LPID_MAP_31 Register CBM_GPID_LPID_MAP_31 - GPID LPID Mapping Table
//! @{

//! Register Offset (relative)
#define CBM_GPID_LPID_MAP_31 0x47C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_GPID_LPID_MAP_31 0xE661047Cu

//! Register Reset Value
#define CBM_GPID_LPID_MAP_31_RST 0x00000000u

//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_31_PORTID0_POS 0
//! Field PORTID0 - GPID2LPID0
#define CBM_GPID_LPID_MAP_31_PORTID0_MASK 0xFu

//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_31_PORTID1_POS 4
//! Field PORTID1 - GPID2LPID1
#define CBM_GPID_LPID_MAP_31_PORTID1_MASK 0xF0u

//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_31_PORTID2_POS 8
//! Field PORTID2 - GPID2LPID2
#define CBM_GPID_LPID_MAP_31_PORTID2_MASK 0xF00u

//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_31_PORTID3_POS 12
//! Field PORTID3 - GPID2LPID3
#define CBM_GPID_LPID_MAP_31_PORTID3_MASK 0xF000u

//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_31_PORTID4_POS 16
//! Field PORTID4 - GPID2LPID4
#define CBM_GPID_LPID_MAP_31_PORTID4_MASK 0xF0000u

//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_31_PORTID5_POS 20
//! Field PORTID5 - GPID2LPID5
#define CBM_GPID_LPID_MAP_31_PORTID5_MASK 0xF00000u

//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_31_PORTID6_POS 24
//! Field PORTID6 - GPID2LPID6
#define CBM_GPID_LPID_MAP_31_PORTID6_MASK 0xF000000u

//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_31_PORTID7_POS 28
//! Field PORTID7 - GPID2LPID7
#define CBM_GPID_LPID_MAP_31_PORTID7_MASK 0xF0000000u

//! @}

//! \defgroup CBM_ACTIVITY_CTRL_REG Register CBM_ACTIVITY_CTRL_REG - CBM Activity Control Register
//! @{

//! Register Offset (relative)
#define CBM_ACTIVITY_CTRL_REG 0x480
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_ACTIVITY_CTRL_REG 0xE6610480u

//! Register Reset Value
#define CBM_ACTIVITY_CTRL_REG_RST 0x00000000u

//! Field LOWACTEN - Low Activity Enable
#define CBM_ACTIVITY_CTRL_REG_LOWACTEN_POS 0
//! Field LOWACTEN - Low Activity Enable
#define CBM_ACTIVITY_CTRL_REG_LOWACTEN_MASK 0x1u

//! Field HIGHACTEN - High Activity Enable
#define CBM_ACTIVITY_CTRL_REG_HIGHACTEN_POS 1
//! Field HIGHACTEN - High Activity Enable
#define CBM_ACTIVITY_CTRL_REG_HIGHACTEN_MASK 0x2u

//! Field ACTLEVIN - Activity Level Input
#define CBM_ACTIVITY_CTRL_REG_ACTLEVIN_POS 4
//! Field ACTLEVIN - Activity Level Input
#define CBM_ACTIVITY_CTRL_REG_ACTLEVIN_MASK 0x70u

//! Field ACTLEVOUT - Activity Level Output
#define CBM_ACTIVITY_CTRL_REG_ACTLEVOUT_POS 8
//! Field ACTLEVOUT - Activity Level Output
#define CBM_ACTIVITY_CTRL_REG_ACTLEVOUT_MASK 0x700u

//! @}

//! \defgroup CBM_ACTIVITY_LEVEL_REG Register CBM_ACTIVITY_LEVEL_REG - CBM Activity Level configuration register
//! @{

//! Register Offset (relative)
#define CBM_ACTIVITY_LEVEL_REG 0x484
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CBM_ACTIVITY_LEVEL_REG 0xE6610484u

//! Register Reset Value
#define CBM_ACTIVITY_LEVEL_REG_RST 0x00000000u

//! Field LEV0 - Level 0 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV0_POS 0
//! Field LEV0 - Level 0 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV0_MASK 0xFu

//! Field LEV1 - Level 1 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV1_POS 4
//! Field LEV1 - Level 1 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV1_MASK 0xF0u

//! Field LEV2 - Level 2 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV2_POS 8
//! Field LEV2 - Level 2 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV2_MASK 0xF00u

//! Field LEV3 - Level 3 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV3_POS 12
//! Field LEV3 - Level 3 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV3_MASK 0xF000u

//! Field LEV4 - Level 4 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV4_POS 16
//! Field LEV4 - Level 4 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV4_MASK 0xF0000u

//! Field LEV5 - Level 5 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV5_POS 20
//! Field LEV5 - Level 5 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV5_MASK 0xF00000u

//! Field LEV6 - Level 6 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV6_POS 24
//! Field LEV6 - Level 6 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV6_MASK 0xF000000u

//! Field LEV7 - Level 7 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV7_POS 28
//! Field LEV7 - Level 7 Frequency Ratio
#define CBM_ACTIVITY_LEVEL_REG_LEV7_MASK 0xF0000000u

//! @}

//! \defgroup CQM_LOW_ACT_COUNTER Register CQM_LOW_ACT_COUNTER - CQM Activity Counter
//! @{

//! Register Offset (relative)
#define CQM_LOW_ACT_COUNTER 0x488
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CQM_LOW_ACT_COUNTER 0xE6610488u

//! Register Reset Value
#define CQM_LOW_ACT_COUNTER_RST 0x00000000u

//! Field COUNT - Activity counter
#define CQM_LOW_ACT_COUNTER_COUNT_POS 0
//! Field COUNT - Activity counter
#define CQM_LOW_ACT_COUNTER_COUNT_MASK 0xFFFFu

//! Field THRSD - Activity counter threshold
#define CQM_LOW_ACT_COUNTER_THRSD_POS 16
//! Field THRSD - Activity counter threshold
#define CQM_LOW_ACT_COUNTER_THRSD_MASK 0xFFFF0000u

//! @}

//! \defgroup CQM_HIGH_ACT_COUNTER Register CQM_HIGH_ACT_COUNTER - CQM Activity Counter
//! @{

//! Register Offset (relative)
#define CQM_HIGH_ACT_COUNTER 0x48C
//! Register Offset (absolute) for 1st Instance CQM_CONTROL
#define CQM_CONTROL_CQM_HIGH_ACT_COUNTER 0xE661048Cu

//! Register Reset Value
#define CQM_HIGH_ACT_COUNTER_RST 0x00000000u

//! Field COUNT - Activity counter
#define CQM_HIGH_ACT_COUNTER_COUNT_POS 0
//! Field COUNT - Activity counter
#define CQM_HIGH_ACT_COUNTER_COUNT_MASK 0xFFFFu

//! Field THRSD - Activity counter threshold
#define CQM_HIGH_ACT_COUNTER_THRSD_POS 16
//! Field THRSD - Activity counter threshold
#define CQM_HIGH_ACT_COUNTER_THRSD_MASK 0xFFFF0000u

//! @}

//! @}

#endif
