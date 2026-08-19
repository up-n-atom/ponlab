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
 * Description: dpl monitor driver
 */

#define pr_fmt(fmt) "dpl_mon: %s:%d: " fmt, __func__, __LINE__

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/timer.h>

#include "dpl_monitor.h"

#define MON_FREQ_SEC 1

/**
 * @enum dpl_cross_type
 * @brief dpl crossing type
 */
enum dpl_cross_type {
	/*! thresholds not crossed */
	NOT_CROSSED,

	/*! thresholds for ON crossed */
	CROSSED_ON,

	/*! thresholds for OFF crossed */
	CROSSED_OFF,
};

/**
 * @struct dpl_act_status
 * @brief dpl action status
 */
struct dpl_act_status {
	/*! true if action is currently enabled */
	bool is_enabled;

	/*! true if attribute crossed the thr for the required iterations */
	enum dpl_cross_type crossed[DPL_ATTR_MAX];

	/*! number of continues iterations above the threshold */
	unsigned int num_seq_iter[DPL_ATTR_MAX];

	/*! last DPL_HISTORY_SIZE iterations, 1 if was above ON threshold */
	unsigned long history_bmap[DPL_ATTR_MAX][BITS_TO_LONGS(DPL_HISTORY_SIZE)];
};

/**
 * @struct dpl_mon
 * @brief dpl monitor information
 */
struct dpl_mon {
	struct dpl_monitor_param *param;
	struct dpl_act_status status[DPL_ACTION_MAX];
	unsigned long long cur_val[DPL_ATTR_MAX];
	unsigned long long prev_val[DPL_ATTR_MAX];
};

/**
 * @struct dpl_mon_db
 * @brief dpl monitor database
 */
struct dpl_mon_db {
	struct dpl_mon mon;
	struct timer_list timer;
	bool active;
	unsigned int tick_time_ms;
	unsigned long prev_tick_jiffies;
};

#define foreach_dpl_attr(i) for (i = 0; i < DPL_ATTR_MAX;   i++)
#define foreach_dpl_act(i)  for (i = 0; i < DPL_ACTION_MAX; i++)

static struct dpl_mon_db *db;

/**
 * @brief do the action
 * @param mon dpl monitor
 * @param act action
 */
static void mon_act_do(struct dpl_mon *mon, unsigned int act)
{
	enum dpl_attr_type attr;
	struct dpl_thr *thr;
	enum dpl_attr_type attr_dep;
	enum dpl_cross_type cross_type;
	enum dpl_cross_type dep_cross_type;

	if (mon->status[act].is_enabled)
		thr = mon->param->act[act].thr[THR_OFF];
	else
		thr = mon->param->act[act].thr[THR_ON];

	foreach_dpl_attr(attr) {
		cross_type = mon->status[act].crossed[attr];
		if (cross_type == NOT_CROSSED)
			continue;

		pr_debug("act %u attr %u crossed %s\n", act, attr,
			  cross_type == CROSSED_ON ? "ON" : "OFF");
		attr_dep = thr[attr].dependency;
		while (attr_dep != DPL_ATTR_MAX) {
			if (attr_dep == attr) {
				/* loop */
				pr_debug("act %u attr %u dependency loop\n",
					 act, attr);
				break;
			}
			dep_cross_type = mon->status[act].crossed[attr_dep];
			if (dep_cross_type != cross_type) {
				pr_debug("act %u attr %u dependency %u not crossed (cross_type = %s)\n",
					 act, attr, attr_dep,
					 dep_cross_type == NOT_CROSSED ? "NOT_CROSSED" : 
					 dep_cross_type == CROSSED_ON ? "ON" : "OFF");
				return;
			}
			/* check next dependency
			 * dependency of the attribute dependency
			 */
			attr_dep = thr[attr_dep].dependency;
		}

		if (mon->status[act].is_enabled) {
			if (cross_type == CROSSED_ON) {
				if (mon->param->act[act].dynamic) {
					mon->param->act[act].dynamic();
					pr_debug("#### act %u dynamic enable ####\n",
						  act);
				}
			} else {
				mon->param->act[act].disable();
				mon->status[act].is_enabled = false;
				pr_debug("#### act %u disabled ####\n", act);
			}
		} else {
			mon->param->act[act].enable();
			mon->status[act].is_enabled = true;
			pr_debug("#### act %u enabled ####\n", act);
		}
		mon->status[act].num_seq_iter[attr] = 0;
		mon->status[act].crossed[attr] = NOT_CROSSED;
		return;
	}
}

static void status_update_history(struct dpl_mon *mon, unsigned int act,
				  enum dpl_attr_type attr, bool is_above)
{
	unsigned long *bmap = mon->status[act].history_bmap[attr];

	if (!bmap) {
		pr_debug("Invalid bmap: NULL pointer\n");
		return;
	}

	bitmap_shift_left(bmap, bmap, 1, DPL_HISTORY_SIZE);

	if (is_above)
		set_bit(0, bmap);

	pr_debug("act %u attr %u: history_bmap: 0x%lx\n", act, attr, *bmap);
}

static bool is_crossed_history_thr(struct dpl_act_status *act,
				   enum dpl_attr_type attr,
				   struct dpl_thr *thr)
{
	/* number of above ON thr from last DPL_HISTORY_SIZE iterations */
	unsigned int num_above =
			bitmap_weight(act->history_bmap[attr], DPL_HISTORY_SIZE);

	/* for enable state need to count all the OFF in history_bmap */
	if (act->is_enabled)
		num_above = DPL_HISTORY_SIZE - num_above;

	return num_above >= thr->num_iter;
}

static bool is_crossed_seq_iter_thr(struct dpl_act_status *act,
				    enum dpl_attr_type attr,
				    struct dpl_thr *thr)
{
	return act->num_seq_iter[attr] >= thr->num_seq_iter;
}

/**
 * @brief update the monitor status for specific action for specific attribute
 * when the action is disable.
 */
static void status_update_case_disable(struct dpl_mon *mon, unsigned int act,
				       enum dpl_attr_type attr)
{
	struct dpl_thr *thr;
	bool is_above_thr_on;

	/* check ON thr for enable action */
	thr = mon->param->act[act].thr[THR_ON];
	/* if the attribute is inactive for the action, nothing to do */
	if (!thr[attr].is_active)
		return;

	is_above_thr_on = mon->cur_val[attr] > thr[attr].val;
	
	status_update_history(mon, act, attr, is_above_thr_on);

	if (!is_above_thr_on) {
		pr_debug("act %u attr %u below threshold %llu[%u]\n", act, attr,
			  mon->cur_val[attr], thr[attr].val);
		mon->status[act].num_seq_iter[attr] = 0;
		mon->status[act].crossed[attr] = NOT_CROSSED;
		return;
	}

	mon->status[act].num_seq_iter[attr]++;
	pr_debug("act %u attr %u above threshold %llu[%u] for %u iterations\n",
		 act, attr, mon->cur_val[attr], thr[attr].val,
		 mon->status[act].num_seq_iter[attr]);

	if (is_crossed_history_thr(&mon->status[act], attr, thr) ||
	    is_crossed_seq_iter_thr(&mon->status[act], attr, thr)) {
		pr_debug("act %u attr %u crossed ON\n", act, attr);
		mon->status[act].crossed[attr] = CROSSED_ON;
	}
}

/**
 * @brief update the monitor status for specific action for specific attribute
 * when the action is enable.
 */
static void status_update_case_enable(struct dpl_mon *mon, unsigned int act,
				      enum dpl_attr_type attr)
{
	struct dpl_thr *thr;
	bool is_above_thr_on;

	/* check ON thr for dynamic action */
	thr = mon->param->act[act].thr[THR_ON];
	/* if the attribute is inactive for the action, skip to OFF thr */
	if (!thr[attr].is_active)
		goto check_off;
	
	is_above_thr_on = mon->cur_val[attr] > thr[attr].val;
	status_update_history(mon, act, attr, is_above_thr_on);

	if (is_above_thr_on) {
		pr_debug("act %u attr %u above ON threshold %llu[%u]\n", act, attr,
			  mon->cur_val[attr], thr[attr].val);
		mon->status[act].num_seq_iter[attr] = 0;
		mon->status[act].crossed[attr] = CROSSED_ON;
		return;
	}
check_off:
	/* check OFF thr for disable the action */
	thr = mon->param->act[act].thr[THR_OFF];
	/* if the attribute is inactive for the action, nothing to do */
	if (!thr[attr].is_active)
		return;
	if (mon->cur_val[attr] >= thr[attr].val) {
		pr_debug("act %u attr %u below threshold %llu[%u]\n", act, attr,
			  mon->cur_val[attr], thr[attr].val);
		mon->status[act].num_seq_iter[attr] = 0;
		mon->status[act].crossed[attr] = NOT_CROSSED;
		return;
	}

	mon->status[act].num_seq_iter[attr]++;
	pr_debug("act %u attr %u above threshold %llu[%u] for %u iterations\n",
		  act, attr, mon->cur_val[attr], thr[attr].val,
		  mon->status[act].num_seq_iter[attr]);

	if (is_crossed_history_thr(&mon->status[act], attr, thr) &&
	    is_crossed_seq_iter_thr(&mon->status[act], attr, thr)) {
		pr_debug("act %u attr %u crossed OFF\n", act, attr);
		mon->status[act].crossed[attr] = CROSSED_OFF;
	}
}

/**
 * @brief update the monitor action status
 * @param mon dpl monitor
 * @param act action
 */
static void mon_status_update(struct dpl_mon *mon, unsigned int act)
{
	enum dpl_attr_type attr;
	bool is_en = mon->status[act].is_enabled;

	pr_debug("act %u status %s\n", act, is_en ? "enabled" : "disabled");

	foreach_dpl_attr(attr) {
		if (is_en)
			status_update_case_enable(mon, act, attr);
		else
			status_update_case_disable(mon, act, attr);
	}
}

/**
 * @brief collect the attributes values by the get callbacks
 * @param mon dpl monitor
 */
static void mon_attr_get(struct dpl_mon *mon)
{
	enum dpl_attr_type attr;
	unsigned long long val;

	/* collect the information (get_cb) */
	foreach_dpl_attr(attr) {
		if (!mon->param->attr_get[attr].get_cb)
			continue;
		val = 0;
		mon->param->attr_get[attr].get_cb(&val);
		switch (attr) {
		case DPL_PPS:
		case DPL_DPS:
		case DPL_SPS:
			/* value diff */
			mon->cur_val[attr] = val - mon->prev_val[attr];
			/* value per MON_FREQ_SEC seconds */
			mon->cur_val[attr] =
				(mon->cur_val[attr] * MON_FREQ_SEC * 1000) /
				db->tick_time_ms;
			break;
		case DPL_SUT:
			mon->cur_val[attr] = val;
			break;
		case DPL_CPU:
			mon->cur_val[attr] = val;
			break;
		default:
			pr_err("Unknown attr %d\n", attr);
			break;
		}
		pr_debug("attr %u iteration read %llu prev %llu value %llu\n",
			 attr, val, mon->prev_val[attr], mon->cur_val[attr]);
		mon->prev_val[attr] = val;
	}
}

/**
 * @brief monitor iteration
 * @param mon dpl monitor
 */
static void mon_iteration(struct dpl_mon *mon)
{
	unsigned int act;
	unsigned long tstmp = jiffies;

	/* save the iteration actual time */
	db->tick_time_ms = jiffies_to_msecs(tstmp - db->prev_tick_jiffies);
	db->prev_tick_jiffies = tstmp;
	if (!db->tick_time_ms)
		return;

	/* read all attributes */
	mon_attr_get(mon);

	/* update the monitor status */
	foreach_dpl_act(act) {
		if (!mon->param->act[act].enable ||
		    !mon->param->act[act].disable)
			continue;
		mon_status_update(mon, act);
	}

	/* take actions according to current status */
	foreach_dpl_act(act) {
		if (!mon->param->act[act].enable ||
		    !mon->param->act[act].disable)
			continue;
		mon_act_do(mon, act);
	}
}

/**
 * @brief restart the timer
 */
static void mon_timer_mod(void)
{
	pr_debug("restart the timer\n");
	mod_timer(&db->timer, jiffies + msecs_to_jiffies(MON_FREQ_SEC * 1000));
}

/**
 * @brief monitor timer handler
 * @param timer the monitor timer
 */
static void mon_timer_handler(struct timer_list *timer)
{
	pr_debug("timer handler\n");

	mon_iteration(&db->mon);

	if (db->active)
		mon_timer_mod();
}

/**
 * @brief setup the timer
 */
static void mon_timer_setup(void)
{
	pr_debug("setup the timer\n");
	timer_setup(&db->timer, mon_timer_handler, 0);
}

/**
 * @brief delete the timer
 */
static void mon_timer_del(void)
{
	pr_debug("delete the timer\n");

	del_timer_sync(&db->timer);
}

void dpl_mon_param_dump(struct dpl_monitor_param *param)
{
	enum dpl_attr_type attr;
	unsigned int act;

	if (!param)
		return;

	for (act = 0; act < DPL_ACTION_MAX; act++) {
		pr_info("\n\n");
		pr_info("ACTION %u:\n", act);
		for (attr = 0; attr < DPL_ATTR_MAX; attr++) {
			pr_info("         ATTR %u:\n", attr);
			pr_info("         ON : is_active=%u val=%u num_iter=%u num_seq_iter=%u dependency=%u\n",
				param->act[act].thr[THR_ON][attr].is_active,
				param->act[act].thr[THR_ON][attr].val,
				param->act[act].thr[THR_ON][attr].num_iter,
				param->act[act].thr[THR_ON][attr].num_seq_iter,
				param->act[act].thr[THR_ON][attr].dependency);
			pr_info("         OFF: is_active=%u val=%u num_iter=%u num_seq_iter=%u dependency=%u\n",
				param->act[act].thr[THR_OFF][attr].is_active,
				param->act[act].thr[THR_OFF][attr].val,
				param->act[act].thr[THR_OFF][attr].num_iter,
				param->act[act].thr[THR_ON][attr].num_seq_iter,
				param->act[act].thr[THR_OFF][attr].dependency);
		}
	}
}

void dpl_mon_set_enable(bool val)
{
	if (!db) {
		pr_err("monitor DB is not initialized\n");
		return;
	}

	if (val == db->active)
		return;
	if (!val)
		goto set;

	/* case the enable is ON and db->active is OFF */
	if (!db->mon.param) {
		pr_err("mon.param is not set!\n");
		return;
	}
	mon_timer_mod();
set:
	db->active = val;
}

bool dpl_mon_is_enable(void)
{
	if (!db) {
		pr_err("monitor DB is not initialized\n");
		return false;
	}
	return db->active;
}

int dpl_mon_register(struct dpl_monitor_param *param)
{
	if (!param)
		return -EINVAL;

	if (db->mon.param) {
		pr_err("monitor already running\n");
		return -EEXIST;
	}

	memset(db, 0, sizeof(*db));
	db->mon.param = param;

	/* start timer */
	mon_timer_setup();

	db->active = true;
	mon_timer_mod();

	return 0;
}

int dpl_mon_unregister(void)
{
	if (!db->mon.param) {
		pr_err("monitor is not running\n");
		return -ENOENT;
	}

	db->active = false;

	/* stop timer */
	mon_timer_del();

	db->mon.param = NULL;
	memset(db, 0, sizeof(*db));

	return 0;
}

int __init dpl_mon_init(struct dentry *parent_dir)
{
	db = kzalloc(sizeof(*db), GFP_KERNEL);
	if (!db)
		return -ENOMEM;

	mon_dbg_init(parent_dir);
	pr_debug("done\n");

	return 0;
}

void __exit dpl_mon_exit(void)
{
	if (!db)
		return;

	/* if timer is runing - delete the timer */
	if (db->mon.param)
		mon_timer_del();
	mon_dbg_clean();
	kfree(db);
	db = NULL;
	pr_debug("done\n");
}
