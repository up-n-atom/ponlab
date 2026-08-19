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

#ifndef __MXL_LGM_CQM_H
#define __MXL_LGM_CQM_H

#define DP_RES_ID_SYS		1
#define DP_RES_ID_WAV		2
#define DP_RES_ID_VOICE0        3
#define DP_RES_ID_VRX		4
#define DP_RES_ID_DOCSIS	5
#define DP_RES_ID_CPU           6
#define DP_RES_ID_TOE		7
#define DP_RES_ID_DPDK		8
#define DP_RES_ID_LROUC		9
#define DP_RES_ID_DOCSIS_MMM	30
#define DP_RES_ID_DOCSIS_VOICE	31
#define DP_RES_ID_DOCSIS_MPEG	32
#define	DP_RES_ID_WAV614	614
#define	DP_RES_ID_WAV624	624
#define	DP_RES_ID_WAV654	654
#define DP_RES_ID_WAV700	700
#define CQM_CPU_ISOLATED	0
#define CQM_NIOC_ISOLATED	1
#define CQM_NIOC_SHARED		2
#define SSB_NIOC_SHARED		3
#define SSB_LROUC_NIOC_SHARED	4
#define SSB_LROUC_NIOC_ISOLATED	5

/* CQM resource direction in bitmap format.
 * Bit 0 is for CQM_RX
 * Bit 1 is for CQM_TX
 * When both bit 0 and 1 are set, it's CQM_TX_RX
 */
#define CQM_RX			0x1
#define CQM_TX			0x2
#define CQM_TX_RX		0x3

/* LPID port mode is a 16-bit bitmap representing port P0-P15.
 * Bit value 1 indicates the port corresponding to the bit
 * is dynamic port. For example, 0xF000 indicates
 * P12-P15 are dynamic ports.
 */
#define LPID_PORT_MODE0		0xf000
#define LPID_PORT_MODE1		0xf600
#define LPID_PORT_INVALID	0x0000

/* LPID wan mode is a 8-bit bitmap representing the WAN mode.
 * Bit 0   - Ethernet
 * Bit 1   - PON
 * Bit 2   - DOCSIS
 * Bit 3   - DSL
 * Bit 4-7 - Reserved
 */
#define LPID_WAN_ETH		0x01
#define LPID_WAN_PON		0x02
#define LPID_WAN_DOCSIS		0x04
#define LPID_WAN_DSL		0x08
#define LPID_WAN_INVALID	0x00

#endif
