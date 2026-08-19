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

#ifndef __MXL_EPU_REGS_H
#define __MXL_EPU_REGS_H

/******** EPU AON ********/
#define EPU_SYS_PM_CTL			0x0
#define EPU_COMBO_MODE_L_MSK		GENMASK(8, 7)
#define EPU_COMBO_MODE_R_MSK		GENMASK(10, 9)
#define EPU_COMBO_MODE_PCIE		0x0
#define EPU_COMBO_MODE_SATA		0x1
#define EPU_COMBO_MODE_ETH		0x2

/* EPU always on,i.e, EPU_AON top registers */

/* From BBU to S0 */
#define EPU_SYS_PM_S0_REQ		BIT(0)

#define EPU_SYS_PM_S3_REQ		BIT(1)

#define EPU_SYS_PM_BBU_ACT_REQ		BIT(3)
#define EPU_SYS_PM_BBU_IDL_REQ		BIT(4)

#define EPU_SYS_PM_ST			0x4

#define EPU_SYS_PM_S0			BIT(0)

#define EPU_SYS_PM_S0I1			BIT(1)
#define EPU_SYS_PM_S0I2			BIT(2)
#define EPU_SYS_PM_S0I3			BIT(3)
#define EPU_SYS_PM_S3			BIT(4)
#define EPU_SYS_PM_BBU_ACT		BIT(6)
#define EPU_SYS_PM_BBU_IDL		BIT(7)

#define EPU_CORE_PM_CTRL(i)		(0x8 + ((i) << 2))
#define EPU_CORE_C7_REQ			BIT(0)
#define EPU_SOFT_BIA_IDLE_REQ		BIT(1)

#define EPU_CORE_PM_ST			0x18

#define EPU_CORE_PM_S0I1		BIT(0)
#define EPU_CORE_PM_S0I2		BIT(1)
#define EPU_CORE_PM_S0I3		BIT(2)
#define EPU_CORE_PM_S3			BIT(3)

#define EPU_ADP_PM_CTL			0x1c
#define EPU_ADP_HW_DFS_MSK		BIT(0)
#define EPU_ADP_HW_DFS_EN		0x1
#define EPU_ADP_HW_DFS_DIS		0x0
#define EPU_ADP_DFS_LVL_MSK		GENMASK(3, 1)
#define EPU_ADP_DFS_LVL0		0
#define EPU_ADP_DFS_LVL1		1
#define EPU_ADP_DFS_LVL2		2
#define EPU_ADP_DFS_LVL3		4
#define EPU_ADP_DFS_LVL4		5

#define EPU_ADP_PM_ST			0x20
#define EPU_ADP_CUR_DFS_LVL_MSK		GENMASK(2, 0)
#define EPU_ADP_SW_DFS_LVL_MSK		GENMASK(5, 3)
#define EPU_ADP_SW_DFS_LVL_SFT		3
#define EPU_ADP_HW_DFS_LVL_MSK		GENMASK(8, 6)

/* Bits and masks will be defined in C */
#define EPU_DEV_PM_CTL_0		0x24
#define MSK_PUNIT_EN			0x1
#define MSK_PUNIT_MSK			BIT(24)
#define SW_PRIO_EN			0x1
#define SW_PRIO_MSK			BIT(25)
#define EPU_DEV_PM_CTL_1		0x28
#define EPU_DEV_PM_CTL_2		0x2c
#define EPU_DEV_PM_CTL_3		0x30

#define EPU_DEV_PM_ST_0			0x34
#define EPU_DEV_PM_ST_1			0x38
#define EPU_DEV_PM_ST_2			0x3c
#define EPU_DEV_PM_ST_3			0x40

#define EPU_DEV_S0I3_PAT_0		0x44
#define EPU_DEV_S0I3_PAT_1		0x48
#define EPU_DEV_S0I3_PAT_2		0x4c

#define EPU_DEV_PM_GRPS			3

#define EPU_ADP_DFS_TIMER_L		0x50
#define EPU_ADP_DFS_TIEMR_H		0x54

#define EPU_OS_S3_RESUME_ADDR_L		0x58
#define EPU_OS_S3_RESUME_ADDR_H		0x5c

#define EPU_VMM_S3_RESUME_ADDR_L	0x60
#define EPU_VMM_S3_RESUME_ADDR_H	0x64

#define EPU_SOC_WAKE_MSK		0x68
#define EPU_SOC_WAKE_STAT		0x6c

#define EPU_SW_I2C_REQ			0x70
#define EPU_SW_I2C_REQ_SET		BIT(0)
#define EPU_SW_I2C_REQ_CLR		0x0

#define EPU_HW_I2C_REL			0x74
#define EPU_HW_I2C_REL_SET		BIT(0)

#define EPU_DDR_HASH_0			0x78
#define EPU_DDR_HASH_1			0x7c
#define EPU_DDR_HASH_2			0x80
#define EPU_DDR_HASH_3			0x84
#define EPU_DDR_HASH_4			0x88
#define EPU_DDR_HASH_5			0x8c
#define EPU_DDR_HASH_6			0x90
#define EPU_DDR_HASH_7			0x94

#define EPU_CORE0_C7_CN_L		0x400
#define EPU_CORE0_C7_CN_H		0x404
#define EPU_CORE_C7_CN_L(id)		(0x400 + (id) * 0x8)
#define EPU_CORE_C7_CN_H(id)		(0x404 + (id) * 0x8)

#define EPU_ADP_SW_DVFS_REQ_CNL		0x420
#define EPU_ADP_SW_DVFS_REQ_CNH		0x424
#define EPU_ADP_SW_DVFS_GNT_CNL		0x428
#define EPU_ADP_SW_DVFS_GNT_CNH		0x42C

#define EPU_ADP_HW_DVFS_REQ_CNL		0x430
#define EPU_ADP_HW_DVFS_REQ_CNH		0x434
#define EPU_ADP_HW_DVFS_GNT_CNL		0x438
#define EPU_ADP_HW_DVFS_GNT_CNH		0x43C

#define EPU_ADP_DVFS_LVL_CNL(level)	(0x440 + (level) * 0x8)
#define EPU_ADP_DVFS_LVL_CNH(level)	(0x444 + (level) * 0x8)

#define EPU_S0I3_PAT_MTH_CNL		0x458
#define EPU_S0I3_PAT_MTH_CNH		0x45c

#define EPU_S0I3_PAT_MISMTH_CNL		0x460
#define EPU_S0I3_PAT_MISMTH_CNH		0x464
#define EPU_S0I3_RESIDENCY_CNL		0x468
#define EPU_S0I3_RESIDENCY_CNH		0x46c

#define EPU_S3_RESIDENCY_CNL		0x470
#define EPU_S3_RESIDENCY_CNH		0x474

#define EPU_PMIC_CPU0_TIMEOUT		0x478
#define EPU_PMIC_CPU1_TIMEOUT		0x47c
#define EPU_PMIC_ADP_TIMEOUT		0x480
#define EPU_PMIC_REQ_TIME		0x484
#define EPU_POWERGOOD_TIMEOUT		0x488
#define EPU_PMIC_POWERGOOD_TOGGLE	0x48c
#define EPU_PMIC_POWERGOOD_TOGGLE_SET	BIT(0)
#define EPU_PMIC_POWERGOOD_TOGGLE_CLR	0x0
#define EPU_POWERGOOD_TIMEOUT_2		0x4b0

#define EPU_BOOT_REASON			0x1000

#define EPU_S3_RESUME			BIT(0)

/******** EPU GEN ********/
#define EPU_ICE_IC_CTL			0x0
#define EPU_ICE_IC_CTL_GRP0_EN		BIT_ULL(0)
#define EPU_ICE_IC_CTL_GRP0_RT0		0x0

#define EPU_ICE_IC_PRIO_MSK		0x8
#define EPU_ICE_IC_PRIO_MSK_SFT		0x4

#define EPU_ICE_IC_IRQ_ACK		0x10
#define EPU_ICE_IC_IRQ_ACK_MSK		GENMASK_ULL(15, 0)

#define EPU_ICE_IC_END_OF_IRQ		0x18

#define EPU_ICE_IC_GRP0			0x80

#define EPU_ICE_IC_SET_EN0		0x180

#define EPU_ICE_IC_PRIO5		0x328
#define EPU_ICE_IC_PRIO7		0x338

#define EPU_PON_PDC_PWR_UP_TMR1		0x1480

#define EPU_SOC_OPC_CONTROL(n)		(0x6000 + (n) * 0x2000)
#define EPU_SOC_OPC_STATUS(n)		(0x6008 + (n) * 0x2000)
#define EPU_SOC3_OPC_CONTROL		0x20000
#define EPU_SOC3_OPC_STATUS		0x20008
#define EPU_ADP_OPC_CONTROL		0xc000
#define EPU_ADP_OPC_STATUS		0xc008
#define EPU_CPUM_OPC_CONTROL(n)		(0xe000 + (n) * 0x10000)
#define EPU_CPUM_OPC_STATUS(n)		(0xe008 + (n) * 0x10000)
#define EPU_SYSTEM_OPC_CONTROL		0x24000
#define EPU_SYSTEM_OPC_STATUS		0x24008
#define OPC_CONTROL_RUN_MASK		BIT_ULL(0)
#define OPC_CONTROL_RUN			BIT_ULL(0)
#define OPC_CONTROL_HLT_ON_TIMEOUT	BIT_ULL(2)
#define OPC_STATUS_IDLE_MASK		BIT_ULL(0)
#define OPC_STATUS_IDLE			BIT_ULL(0)

#define EPU_ADP_OPC_IRQ_SET_EN		0xc010
#define EPU_ADP_OPC_IRQ_PENDING		0xc020
#define EPU_ADP_OPC_IRQ_CLR_PENDING	0xc028

#define EPU_ADP_OPC_TIMEOUT		0xc030

#define EPU_ADP_OPC_TARGET_VFT0		0xc040
#define EPU_ADP_VFT0_RPI0_MSK		GENMASK_ULL(4, 0)

#define EPU_CPUM0_OPC_IRQ_SET_EN	0xe010
#define EPU_CPUM1_OPC_IRQ_SET_EN	0x1e010
#define EPU_CPUM_OPC_IRQ_PENDING(n)	(0xe020 + (n) * 0x10000)
#define EPU_CPUM_OPC_IRQ_CLR_PENDING(n)	(0xe028 + (n) * 0x10000)
#define EPU_OPC_IRQ_TIMEOUT		BIT_ULL(1)

#define EPU_CPUM0_OPC_TIMEOUT		0xe030
#define EPU_CPUM1_OPC_TIMEOUT		0x1e030
#define EPU_OPC_TIMEOUT_MSK		GENMASK_ULL(15, 8)
#define EPU_OPC_TIMEOUT_PRE_MSK		GENMASK_ULL(1, 0)
#define EPU_OPC_TIMEOUT_PRE_4096	0x3

#define EPU_CPUM_OPC_TARGET_VFT0(n)	(0xe040 + (n) * 0x10000)
#define EPU_CPUM_OPC_CUR_VFT0(n)	(0xe060 + (n) * 0x10000)
#define EPU_CPUM_VFT0_RPI0_MSK		GENMASK_ULL(4, 0)

#define PASEQ0_PASEQ_STATUS		0x10008
#define PASEQ_STATUS_IDLE_MSK		BIT_ULL(0)
#define PASEQ0_PASEQ_GP_REG0		0x10080

#define PASEQ1_PASEQ_STATUS		0x12008
#define PASEQ1_PASEQ_GP_REG0		0x12080

#define PASEQ2_PASEQ_STATUS		0x13008
#define PASEQ2_PASEQ_GP_REG0		0x13080

#define PASEQ3_PASEQ_STATUS		0x16008
#define PASEQ3_PASEQ_GP_REG0		0x16080

#define PASEQ0_ADP_IDX_TBL_ADR		0x10800
#define PASEQ1_CPU0_IDX_TBL_ADR		0x12800
#define PASEQ2_CPU1_IDX_TBL_ADR		0x13800

#define PASEQ0_IDX_TBL(n)		(0x10800 + ((n) << 3))
#define PASEQM_IDX_TBL(m, n)		(0x12800 + (m) * 0x1000 + ((n) << 3))
#define PASEQ_IDX_TBL_START_MSK		GENMASK_ULL(9, 0)
#define PASEQ_IDX_TBL_BYE0_MSK		GENMASK_ULL(23, 16)
#define PASEQ_IDX_TBL_BYE1_MSK		GENMASK_ULL(31, 24)
#define PASEQ_IDX_TBL_BYE2_MSK		GENMASK_ULL(39, 32)
#define PASEQ_IDX_TBL_BYE3_MSK		GENMASK_ULL(47, 40)
#define PASEQ_IDX_TBL_BYE4_MSK		GENMASK_ULL(55, 48)
#define PASEQ_IDX_TBL_BYE5_MSK		GENMASK_ULL(63, 56)

#define ADP_OPT_VFT_TBL_MEM0		0xdc00
#define CPUM0_OPT_VFT_TBL_MEM0		0xfc00
#define CPUM1_OPT_VFT_TBL_MEM0		0x1fc00

#define OPC_VFT_TBL_MEM_RPI0_MSK	GENMASK_ULL(20, 16)
#define OPC_VFT_TBL_MEM_RPI1_MSK	GENMASK_ULL(26, 24)
#define OPC_VFT_TBL_MEM_INT_MSK		BIT(11)
#define OPC_VFT_TBL_MEM_TEMP_MSK	GENMASK_ULL(7, 0)
#define OPC_VFT_TBL_MEM_TEMP_MSK_L5	GENMASK_ULL(4, 0)
#define OPC_VFT_TBL_MEM_TEMP_MSK_H3	GENMASK_ULL(7, 5)

#define ADP_OPC_TARGET_VFT0		0xc040
#define ADP_OPC_TARGET_VFT0_RPI0_MSK	GENMASK_ULL(4, 0)
#define ADP_OPC_TARGET_VFT0_RPI1_MSK	GENMASK_ULL(10, 8)

#define ADP_OPC_CUR_VFT0		0xc060
#define ADP_OPC_CUR_VFT0_RPI0_MSK	GENMASK_ULL(4, 0)
#define ADP_OPC_CUR_VFT0_RPI1_MSK	GENMASK_ULL(18, 16)
#define ADP_OPC_CUR_VFT0_BUSY0_MSK	BIT(8)
#define ADP_OPC_CUR_VFT0_BUSY1_MSK	BIT(24)

#define CPUM0_OPC_TARGET_VFT0		0xe040
#define CPUM1_OPC_TARGET_VFT0		0x1e040
#define CPUM_OPC_TARGET_VFT0_RPI0_MSK	GENMASK_ULL(4, 0)
#define CPUM_OPC_TARGET_VFT0_RPI1_MSK	GENMASK_ULL(12, 8)

#define CPUM0_OPC_CUR_VFT0		0xe060
#define CPUM1_OPC_CUR_VFT0		0x1e060
#define CPUM_OPC_CUR_VFT0_RPI0_MSK	GENMASK_ULL(4, 0)
#define CPUM_OPC_CUR_VFT0_RPI1_MSK	GENMASK_ULL(20, 16)
#define CPUM_OPC_CUR_VFT0_BUSY0_MSK	BIT(8)
#define CPUM_OPC_CUR_VFT0_BUSY1_MSK	BIT(24)

#define XPCS5_PDC_POWER_DOWN_TIMER0	0x00018268
#endif
