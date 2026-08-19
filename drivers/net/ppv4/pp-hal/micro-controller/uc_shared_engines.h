/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2019-2020 Intel Corporation
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR APARTICULARPURPOSE.See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public
 * License along with this program; if not,see
 * <http://www.gnu.org/licenses/>.
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PP micro-comtrollers shared accelerators definitions
 */

#ifndef __PP_UC_SHARED_ENGINES_H__
#define __PP_UC_SHARED_ENGINES_H__

/* this file defines the <AID,RID> of the HW entities in the
 * uC Cluster  (aka Shared engines)
 *  +-------------------------------------------------------+
 *  | Important: the <AID,RID> must be aligned with the RDL |
 *  +-------------------------------------------------------+
 * Comments:
 *(1): internal, for debug usage
 *(2): host to program/use
 */

/* Shared Engines AID: */
/**
 * @define Writer
 */
#define WRITER_B_AID           28
/**
 * @define Reader
 */
#define READER_B_AID            29
/**
 * @define Cluster control unit
 */
#define CCU_B_AID               0

/**
 * @define Cluster Bridge
 */
#define CLUSTERBRIDGE_B_AID     4

/* Writer_QoS Egress: */
#define WRITER_B_PORT_SPEC      18
#define WRITER_B_STATUS_SPEC    19

#define WRITER_B_PORT_ENABLE    16

#define WRITER_B_MAP            1
#define WRITER_B_QUEUE_CREDIT   2

/* Writer_MBOX Egress: */

/* each bit in WRITER_MB_FLAGS is a flag: */
#define WR_FLAG_INTERRUPT       0
#define WR_FLAG_IS_DATA         1

/* Reader: */
#define READER_B_IRR            5

#define READER_B_WRED_BASE      0

#define READER_B_QMNGR_BASE     1

#define READER_B_BMNGR_BASE     4

#define READER_B_CLIENT_BASE    2

#define READER_B_PORT_SPEC      8
#define READER_B_COUNTER        9

/* CCU: */
/**
 * @define interrupt status register, bit per source
 */
#define CCU_B_ISR         0
/**
 * @define uC_error Interrupt Enable, bit per uC
 */
#define CCU_B_IER         1
/**
 * @define uC_error Interrupt Request, bit per uC
 */
#define CCU_B_IRR         2
/**
 * @define deassert reset of a uC, array, index per uC
 */
#define CCU_B_UC_UNRESET  3
/**
 * @define clk_en a uC, array, index per uC
 */
#define CCU_B_UC_CLK_EN   4
/**
 * @define array of 8 GP registers. can be used at init
 */
#define CCU_B_GPREG       5
/**
 * @define number of uC's in the cluster
 */
#define CCU_B_MAXCPU      6

/* each bit in ISR indicates interrupt source: */
#define INT_SRC_UCERROR     0
#define INT_SRC_HWEVENTA    1
#define INT_SRC_MBOX        2
#define INT_SRC_READER      3
#define INT_SRC_BRIDGE      4

/* CLUSTERBRIDGE: */

/**
 * @define filter and address translation table[48]
 */
#define CLUSTERBRIDGE_B_FAT       1
#define CLUSTERBRIDGE_B_IRR       3
#define CLUSTERBRIDGE_B_BASE      6
#define CLUSTERBRIDGE_B_VALID     7

#endif /* __PP_UC_SHARED_ENGINES_H__ */
