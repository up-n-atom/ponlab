/*
 * Copyright (C) 2020-2023 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation
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
 * Description: dpl monitor h file
 */

#ifndef _DPL_MONITOR_H_
#define _DPL_MONITOR_H_

#include <linux/types.h>
#include <linux/dcache.h>

/* number of last iterations that are stored */
#define DPL_HISTORY_SIZE 32
#define DPL_ACTION_MAX 4

/**
 * @enum dpl_attr_type
 * @brief dpl attribute type
 */
enum dpl_attr_type {
	/*! packets per second */
	DPL_PPS,

	/*! drop per second */
	DPL_DPS,

	/*! sessions per second */
	DPL_SPS,

	/*! sessions utilization */
	DPL_SUT,

	/*! CPU load */
	DPL_CPU,

	DPL_ATTR_MAX,
};

/**
 * @enum dpl_thr_type
 * @brief dpl threshold type
 */
enum dpl_thr_type {
	/*! threshold type ON */
	THR_ON,

	/*! threshold type OFF */
	THR_OFF,

	/*! number of threshold types */
	DPL_THR_TYPE_MAX,
};

/**
 * @struct dpl_attr_cb
 * @brief dpl attribute read callback
 */
struct dpl_attr_cb {
	void (*get_cb)(unsigned long long *);
};

/**
 * @struct dpl_thr
 * @brief dpl attribute threshold
 */
struct dpl_thr {
	/*! if the action using this attribute */
	unsigned int is_active;

	/*! threshold value */
	unsigned int val;

	/*! number of continuous iterations for ON/OFF states */
	unsigned int num_seq_iter;

	/*! number of iterations from last DPL_HISTORY_SIZE iterations */
	unsigned int num_iter;

	/*! attribute dependency */
	enum dpl_attr_type dependency;
};

/**
 * @struct dpl_action
 * @brief dpl action information
 */
struct dpl_action {
	/*! enable action callback 
	   called in disable state after threshold are above for at least 
	   num_seq_iter continuous iterations OR when the threshold are above 
	   for num_iter iterations from last DPL_HISTORY_SIZE iteration */
	void (*enable)(void);

	/*! dynamic action callback 
	   called in enable state after threshold still above for 1 iteration */
	void (*dynamic)(void);

	/*! disable action callback 
	   called in disable state after threshold are above for at least 
	   num_seq_iter continuous iterations AND when the threshold are above 
	   for num_iter iterations from last DPL_HISTORY_SIZE iteration */
	void (*disable)(void);

	/*! threshold system (per type per attribute) */
	struct dpl_thr thr[DPL_THR_TYPE_MAX][DPL_ATTR_MAX];
};

/**
 * @struct dpl_monitor_param
 * @brief dpl monitor parameter
 */
struct dpl_monitor_param {
	/*! DPL attributes read callback */
	struct dpl_attr_cb attr_get[DPL_ATTR_MAX];

	/*! DPL actions */
	struct dpl_action  act[DPL_ACTION_MAX];
};

/**
 * @brief register to the dpl monitor
 * @param param dpl monitor parameter
 */
int dpl_mon_register(struct dpl_monitor_param *param);

/**
 * @brief unregister from dpl monitor
 */
int dpl_mon_unregister(void);

/**
 * @brief print the attributes and thresholds for each action
 */
void dpl_mon_param_dump(struct dpl_monitor_param *param);

/**
 * @brief set enable/disable to the monitor
 * @param val enable/disable 
 */
void dpl_mon_set_enable(bool val);

/**
 * @brief get the monitor status
 * @return true if it is active, false otherwise
 */
bool dpl_mon_is_enable(void);

#ifdef CONFIG_DEBUG_FS
int mon_dbg_init(struct dentry *parent_dir);
int mon_dbg_clean(void);
#else /* !CONFIG_DEBUG_FS */
inline int mon_dbg_init(struct dentry *parent_dir)
{
	return 0;
}
inline int mon_dbg_clean(void)
{
	return 0;
}
#endif /* CONFIG_DEBUG_FS */

/**
 * @brief init monitor module
 */
int __init dpl_mon_init(struct dentry *parent_dir);

/**
 * @brief exit monitor module
 */
void __exit dpl_mon_exit(void);

#endif /* _DPL_MONITOR_H_ */
