/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __DT_BINDINGS_POWER_LGM_POWER_H__
#define __DT_BINDINGS_POWER_LGM_POWER_H__

/* V080_CPU_M0 */
#define LGM_EPU_PD_CPU_M0	0
/* V080_CPU_M1 */
#define LGM_EPU_PD_CPU_M1	1

/* V080_ADP */
#define LGM_EPU_PD_PPV4		2
#define LGM_EPU_PD_GSW		3
#define LGM_EPU_PD_CQM		4

/* V080_ROC */
#define LGM_EPU_PD_VOICE	5
#define LGM_EPU_PD_PON		6
#define LGM_EPU_PD_HSIO1	7
#define LGM_EPU_PD_HSIO2	8
#define LGM_EPU_PD_HSIO3	9
#define LGM_EPU_PD_HSIO4	10
#define LGM_EPU_PD_TEP		11
#define LGM_EPU_PD_EIP197	12
#define LGM_EPU_PD_VAULT130	13
#define LGM_EPU_PD_TOE		14
#define LGM_EPU_PD_SDXC		15
#define LGM_EPU_PD_EMMC		16
/* 1 */
#define LGM_EPU_PD_DDR_CTL0	17
#define LGM_EPU_PD_RTIT		18
#define LGM_EPU_PD_QSPI		19
#define LGM_EPU_PD_ROM		20
#define LGM_EPU_PD_SSB		21
#define LGM_EPU_PD_USB1		22
#define LGM_EPU_PD_USB2		23
#define LGM_EPU_PD_ASC0		24
#define LGM_EPU_PD_ASC1		25
#define LGM_EPU_PD_ASC2		26
#define LGM_EPU_PD_SSC0		27
#define LGM_EPU_PD_SSC1		28
#define LGM_EPU_PD_SSC2		29
#define LGM_EPU_PD_SSC3		30
#define LGM_EPU_PD_I2C1		31
#define LGM_EPU_PD_I2C2		32
/* 2 */
#define LGM_EPU_PD_I2C3		33
#define LGM_EPU_PD_LEDC0	34
#define LGM_EPU_PD_PCM0		35
#define LGM_EPU_PD_EBU		36
#define LGM_EPU_PD_I2S0		37
#define LGM_EPU_PD_I2S1		38
#define LGM_EPU_PD_DDR_CTL1	39
#define LGM_EPU_PD_EM4		40
#define LGM_EPU_PD_PCM1		41
#define LGM_EPU_PD_PCM2		42
#define LGM_EPU_PD_XPCS5	43
#define LGM_EPU_PD_LEDC1	44
#define LGM_EPU_PD_PCIE30	45
#define LGM_EPU_PD_PCIE40	46
#define LGM_EPU_PD_SATA0	47
#define LGM_EPU_PD_SATA1	48

/* 3 */
#define LGM_EPU_PD_PCIE31      49
#define LGM_EPU_PD_PCIE41      50
#define LGM_EPU_PD_SATA2       51
#define LGM_EPU_PD_SATA3       52
#define LGM_EPU_PD_PCIE10      53
#define LGM_EPU_PD_PCIE20      54
#define LGM_EPU_PD_PCIE11      55
#define LGM_EPU_PD_PCIE21      56

#define LGM_EPU_PD_XPCS10	57
#define LGM_EPU_PD_XPCS11	58
#define LGM_EPU_PD_XPCS20	59
#define LGM_EPU_PD_XPCS21	60

#endif
