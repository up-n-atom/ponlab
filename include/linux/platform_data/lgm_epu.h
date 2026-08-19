/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
 * Copyright (C) 2019-2020 Intel Corporation.
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

#ifndef __LGM_EPU_H
#define __LGM_EPU_H

#include <linux/notifier.h>

enum epu_state {
	EPU_D0 = 0,
	EPU_D1,
	EPU_D2,
	EPU_D3,
};

/**
 * CPU EVENT ID format
 * 23-16: CPU EVENT ID
 * 15-8: CPU EVENT SUB ID
 * 7-0:  CPU index ID
 */
#define CPU_EVENT_ID	0x0
#define CPU_REQ	0x0
#define CPU_REL	0x1
#define CPU_EVENT_C7_REQ_CORE(i) ((CPU_EVENT_ID << 16) | (CPU_REQ << 8) | (i))
#define CPU_EVENT_C7_REL_CORE(i) ((CPU_EVENT_ID << 16) | (CPU_REL << 8) | (i))

#define COMBO_EVENT_ID	0x1
#define COMBO_EVENT_PCIE(i)	((COMBO_EVENT_ID << 16) | ((i) << 8))
#define COMBO_EVENT_SATA(i)	((COMBO_EVENT_ID << 16) | ((i) << 8) | 1)
#define COMBO_EVENT_ETH(i)	((COMBO_EVENT_ID << 16) | ((i) << 8) | 2)

#define I2C_SEM_EVENT_ID	0x2
#define I2C_SEM_EVENT_REQUEST	((I2C_SEM_EVENT_ID << 16) | 0x0)
#define I2C_SEM_EVENT_RELEASE	((I2C_SEM_EVENT_ID << 16) | 0x1)

#define PMIC_EVENT_ID	0x3
#define PMIC_EVENT_REG	((PMIC_EVENT_ID << 16) | 0x0)

#define ADP_EVENT_ID	0x4
#define EPU_ADP_DFS_LVL_H	0
#define EPU_ADP_DFS_LVL_M	1
#define EPU_ADP_DFS_LVL_L	2

#define ADP_EVENT_LOW	((ADP_EVENT_ID << 16) | EPU_ADP_DFS_LVL_L)
#define ADP_EVENT_MED	((ADP_EVENT_ID << 16) | EPU_ADP_DFS_LVL_M)
#define ADP_EVENT_HIGH	((ADP_EVENT_ID << 16) | EPU_ADP_DFS_LVL_H)

#define RCU_EVENT_ID	0x5
#define EPU_GET		0x0
#define EPU_SET		0x1
#define RCU_EVENT_SET_D0(id)	((RCU_EVENT_ID << 16) | (EPU_SET << 12) | (EPU_D0 << 8) | (id))
#define RCU_EVENT_GET_D0(id)	((RCU_EVENT_ID << 16) | (EPU_GET << 12) | (EPU_D0 << 8) | (id))
#define RCU_EVENT_SET_D1(id)	((RCU_EVENT_ID << 16) | (EPU_SET << 12) | (EPU_D1 << 8) | (id))

#define SOC_EVENT_ID	0x6
#define SOC_PD_ID(PD)	((SOC_EVENT_ID) << 16 | (PD))

#define XPCS_EVENT_ID	0x7
#define XPCS_EVENT_SET	((XPCS_EVENT_ID << 16) | 0x1)

#if IS_ENABLED(CONFIG_LGM_EPU)
int epu_notifier_raw_chain(unsigned long val, void *v);
int epu_notifier_blocking_chain(unsigned long val, void *v);
int epu_adp_lvl_notify_register(struct notifier_block *nb);
int epu_adp_lvl_notify_unregister(struct notifier_block *nb);
int epu_rcu_notify_register(struct notifier_block *nb);
int epu_rcu_notify_unregister(struct notifier_block *nb);
raw_spinlock_t *epu_i2c_raw_spinlock(void);
#else
static inline int epu_notifier_raw_chain(unsigned long val, void *v)
{
	return 0;
}

static inline int epu_notifier_blocking_chain(unsigned long val, void *v)
{
	return 0;
}

static inline int epu_adp_lvl_notify_register(struct notifier_block *nb)
{
	return 0;
}

static inline int epu_adp_lvl_notify_unregister(struct notifier_block *nb)
{
	return 0;
}

static inline int epu_rcu_notify_register(struct notifier_block *nb)
{
        return 0;
}

static inline int epu_rcu_notify_unregister(struct notifier_block *nb)
{
        return 0;
}

static raw_spinlock_t *epu_i2c_raw_spinlock(void)
{
	static DEFINE_RAW_SPINLOCK(pseudo_i2c_lock);

	return &pseudo_i2c_lock;
}
#endif /* CONFIG_LGM_EPU */

#endif
