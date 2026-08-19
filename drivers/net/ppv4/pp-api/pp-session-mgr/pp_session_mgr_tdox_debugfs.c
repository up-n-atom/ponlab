/*
 * Copyright (C) 2023-2024 MaxLinear, Inc.
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
 * Description: PP session manager TDOX debugfs interface
 */

#define pr_fmt(fmt) "[PP_SESS_MGR_TDOX_DBG]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/types.h>
#include <linux/notifier.h>
#include <linux/bitops.h>
#include <linux/parser.h>
#include <linux/string.h>
#include <linux/jiffies.h>
#include <linux/pp_api.h>

#include "pp_common.h"
#include "pp_debugfs_common.h"
#include "pp_session_mgr.h"
#include "pp_session_mgr_internal.h"

char *dash_line_str = "|----------------------------------------|";

enum tdox_conf_update_ops {
	tdox_update_help = 1,
	tdox_update_tout,
	tdox_update_max_reach_target,
	tdox_update_max_supp_ratio,
	tdox_update_max_supp_bytes,
};

static const match_table_t tdox_conf_update_tkns = {
	{tdox_update_help,             "help"},
	{tdox_update_tout,             "timeout=%u"},
	{tdox_update_max_reach_target, "max_reach_target=%u"},
	{tdox_update_max_supp_ratio,   "max_supp_ratio=%u"},
	{tdox_update_max_supp_bytes,   "max_supp_bytes=%u"},
	{ 0 },
};

const char *tdox_flds[] = {
	"timeout",
	"max_reach_target",
	"max_supp_ratio",
	"max_supp_bytes"
};

static void __smgr_dbg_tdox_stats_diff(struct smgr_tdox_stats *hs,
					struct smgr_tdox_stats *hs2,
					struct smgr_tdox_uc_stats *ucs,
					struct smgr_tdox_uc_stats *ucs2)
{
	ucs->stage1 = ucs2->stage1 - ucs->stage1;
	ucs->stage2 = ucs2->stage2 - ucs->stage2;
	ucs->stored = ucs2->stored - ucs->stored;
	ucs->forward = ucs2->forward - ucs->forward;
	ucs->aggressive = ucs2->aggressive - ucs->aggressive;
	ucs->low_timers = ucs2->low_timers - ucs->low_timers;
	ucs->high_timers = ucs2->high_timers - ucs->high_timers;
	ucs->expired = ucs2->expired - ucs->expired;
	ucs->create = ucs2->create - ucs->create;
	ucs->remove = ucs2->remove - ucs->remove;
	ucs->recycle = ucs2->recycle - ucs->recycle;
	ucs->recycle_next_op = ucs2->recycle_next_op - ucs->recycle_next_op;
	ucs->errors = ucs2->errors - ucs->errors;
	ucs->max_ratio = ucs2->max_ratio - ucs->max_ratio;
	ucs->max_supp = ucs2->max_supp - ucs->max_supp;
	ucs->max_reach_target = ucs2->max_reach_target - ucs->max_reach_target;
	ucs->timer_cancel = ucs2->timer_cancel - ucs->timer_cancel;
	ucs->s2_fwd1 = ucs2->s2_fwd1 - ucs->s2_fwd1;
	ucs->s2_fwd2 = ucs2->s2_fwd2 - ucs->s2_fwd2;
	ucs->set_timer = ucs2->set_timer - ucs->set_timer;

	hs->free_list_cnt = hs2->free_list_cnt; /* not a pps counter */
	hs->init_list_cnt = hs2->init_list_cnt; /* not a pps counter */
	hs->supp_list_cnt = hs2->supp_list_cnt; /* not a pps counter */
	hs->prio_list_cnt = hs2->prio_list_cnt; /* not a pps counter */
	hs->cand_list_cnt = hs2->cand_list_cnt; /* not a pps counter */
	hs->lro_list_cnt = hs2->lro_list_cnt; /* not a pps counter */
	hs->supp_ids_cnt = hs2->supp_ids_cnt; /* not a pps counter */
	hs->dbg.supp_list_full = hs2->dbg.supp_list_full -
				 hs->dbg.supp_list_full;
	hs->dbg.prio_list_full = hs2->dbg.prio_list_full -
				 hs->dbg.prio_list_full;
	hs->dbg.cand_list_full = hs2->dbg.cand_list_full -
				 hs->dbg.cand_list_full;
	hs->dbg.supp_full_state = hs2->dbg.supp_full_state -
				 hs->dbg.supp_full_state;
	hs->dbg.switch_q = hs2->dbg.switch_q - hs->dbg.switch_q;
}

static void __smgr_dbg_tdox_stats_read(struct seq_file *f, bool reset, bool pps)
{
	struct smgr_tdox_stats hs;
	struct smgr_tdox_stats hs2;
	struct smgr_tdox_uc_stats ucs;
	struct smgr_tdox_uc_stats ucs2;
	s32 ret;
	u64 start, end;
	u32 time;

	if (pps)
		start = get_jiffies_64();
	memset(&ucs, 0, sizeof(ucs));
	ret = smgr_tdox_stats_get(&hs, &ucs, reset);
	if (ret)
		return;

	if (reset)
		return;

	if (pps) {
		end = get_jiffies_64();
		time = jiffies_to_msecs(end - start);

		if (time < 1000)
			msleep(1000 - time);
		else
			pr_warn("Reading counters take too long\n");

		memset(&ucs2, 0, sizeof(ucs2));
		ret = smgr_tdox_stats_get(&hs2, &ucs2, reset);
		if (ret)
			return;

		__smgr_dbg_tdox_stats_diff(&hs, &hs2, &ucs, &ucs2);
	}

	seq_puts(f, "\n");
	seq_puts(f, "|========================================|\n");
	seq_puts(f, "|             TDOX Statistics            |\n");
	seq_puts(f, "|========================================|\n");

	/* priority counters */
	seq_printf(f, "%s\n", dash_line_str);
	seq_printf(f, "| %-25s ", " free list ");
	seq_printf(f, "| %10u |\n", hs.free_list_cnt);
	seq_printf(f, "| %-25s ", " init list ");
	seq_printf(f, "| %10u |\n", hs.init_list_cnt);
	seq_printf(f, "| %-25s ", " supp list ");
	seq_printf(f, "| %10u |\n", hs.supp_list_cnt);
	seq_printf(f, "| %-25s ", " prio list ");
	seq_printf(f, "| %10u |\n", hs.prio_list_cnt);
	seq_printf(f, "| %-25s ", " cand list ");
	seq_printf(f, "| %10u |\n", hs.cand_list_cnt);
	seq_printf(f, "| %-25s ", " lro list ");
	seq_printf(f, "| %10u |\n", hs.lro_list_cnt);
	seq_printf(f, "| %-25s ", " supp ids ");
	seq_printf(f, "| %10u |\n", hs.supp_ids_cnt);

	seq_printf(f, "%s\n", dash_line_str);
	seq_printf(f, "| %-25s ", " supp list full event ");
	seq_printf(f, "| %10u |\n", hs.dbg.supp_list_full);
	seq_printf(f, "| %-25s ", " prio list full event ");
	seq_printf(f, "| %10u |\n", hs.dbg.prio_list_full);
	seq_printf(f, "| %-25s ", " cand list full event ");
	seq_printf(f, "| %10u |\n", hs.dbg.cand_list_full);
	seq_printf(f, "| %-25s ", " supp full state event ");
	seq_printf(f, "| %10u |\n", hs.dbg.supp_full_state);
	seq_printf(f, "| %-25s ", " switch Q ");
	seq_printf(f, "| %10u |\n", hs.dbg.switch_q);
	seq_printf(f, "%s\n", dash_line_str);

	/* UC counters */
	seq_puts(f, "\n");
	seq_puts(f, "|========================================|\n");
	seq_puts(f, "|             UC   Statistics            |\n");
	seq_puts(f, "|========================================|\n");

	seq_printf(f, "%s\n", dash_line_str);
	seq_printf(f, "| %-25s ", " phase1 ");
	seq_printf(f, "| %10u |\n", ucs.stage1);
	seq_printf(f, "| %-25s ", " phase2 ");
	seq_printf(f, "| %10u |\n", ucs.stage2);
	seq_printf(f, "| %-25s ", " stored ");
	seq_printf(f, "| %10u |\n", ucs.stored);
	seq_printf(f, "| %-25s ", " forward ");
	seq_printf(f, "| %10u |\n", ucs.forward);
	seq_printf(f, "| %-25s ", " aggressive ");
	seq_printf(f, "| %10u |\n", ucs.aggressive);
	seq_printf(f, "| %-25s ", " low_timers");
	seq_printf(f, "| %10u |\n", ucs.low_timers);
	seq_printf(f, "| %-25s ", " high_timers ");
	seq_printf(f, "| %10u |\n", ucs.high_timers);
	seq_printf(f, "| %-25s ", " expired ");
	seq_printf(f, "| %10u |\n", ucs.expired);
	seq_printf(f, "| %-25s ", " create ");
	seq_printf(f, "| %10u |\n", ucs.create);
	seq_printf(f, "| %-25s ", " remove ");
	seq_printf(f, "| %10u |\n", ucs.remove);
	seq_printf(f, "| %-25s ", " recycle ");
	seq_printf(f, "| %10u |\n", ucs.recycle);
	seq_printf(f, "| %-25s ", " recycle_next_op ");
	seq_printf(f, "| %10u |\n", ucs.recycle_next_op);
	seq_printf(f, "| %-25s ", " errors ");
	seq_printf(f, "| %10u |\n", ucs.errors);
	seq_printf(f, "| %-25s ", " max_ratio ");
	seq_printf(f, "| %10u |\n", ucs.max_ratio);
	seq_printf(f, "| %-25s ", " max_supp ");
	seq_printf(f, "| %10u |\n", ucs.max_supp);
	seq_printf(f, "| %-25s ", " max_reach_target ");
	seq_printf(f, "| %10u |\n", ucs.max_reach_target);
	seq_printf(f, "| %-25s ", " timer_cancel ");
	seq_printf(f, "| %10u |\n", ucs.timer_cancel);
	seq_printf(f, "| %-25s ", " s2_fwd1 ");
	seq_printf(f, "| %10u |\n", ucs.s2_fwd1);
	seq_printf(f, "| %-25s ", " s2_fwd2 ");
	seq_printf(f, "| %10u |\n", ucs.s2_fwd2);
	seq_printf(f, "| %-25s ", " set_timer ");
	seq_printf(f, "| %10u |\n", ucs.set_timer);
	seq_printf(f, "%s\n", dash_line_str);
}

static void __smgr_dbg_tdox_stats(struct seq_file *f)
{
	__smgr_dbg_tdox_stats_read(f, false, false);
}

static void __smgr_dbg_tdox_clr_stats(struct seq_file *f)
{
	__smgr_dbg_tdox_stats_read(f, true, false);
}

static void __smgr_dbg_tdox_pps_stats(struct seq_file *f)
{
	__smgr_dbg_tdox_stats_read(f, false, true);
}

PP_DEFINE_DEBUGFS(tdox_stats, __smgr_dbg_tdox_stats, NULL);
PP_DEFINE_DEBUGFS(tdox_clr_stats, __smgr_dbg_tdox_clr_stats, NULL);
PP_DEFINE_DEBUGFS(tdox_pps_stats, __smgr_dbg_tdox_pps_stats, NULL);

void tdox_conf_update_help(void)
{
	u32 idx;

	pr_info("echo [fld]=[value]... > config\n");
	pr_info("Supported fields:\n");
	pr_info("=================\n");
	for (idx = 0; idx < ARRAY_SIZE(tdox_flds); idx++)
		pr_info("%s\n", tdox_flds[idx]);
}

static void __smgr_dbg_tdox_conf_get(struct seq_file *f)
{
	struct smgr_tdox_conf conf;

	if (smgr_tdox_conf_get(&conf)) {
		pr_info("failed to get tdox conf parameters \n");
		return;
	}

	seq_puts(f, "\n");
	seq_puts(f, "|========================================|\n");
	seq_puts(f, "|             TDOX Parameters            |\n");
	seq_puts(f, "|========================================|\n");

	seq_printf(f, "%s\n", dash_line_str);
	seq_printf(f, "| %-25s ", " Timeout (usec)");
	seq_printf(f, "| %10u |\n", conf.timeout);
	seq_printf(f, "| %-25s ", " Max Reach Target ");
	seq_printf(f, "| %10u |\n", conf.max_reach_target);
	seq_printf(f, "| %-25s ", " Max Supp Ratio ");
	seq_printf(f, "| %10u |\n", conf.max_supp_ratio);
	seq_printf(f, "| %-25s ", " Max Supp Bytes ");
	seq_printf(f, "| %10u |\n", conf.max_supp_bytes);
	seq_printf(f, "| %-25s ", " Max Suppressed Sessions ");
	seq_printf(f, "| %10u |\n", UC_MAX_TDOX_SESSIONS);
	seq_printf(f, "| %-25s ", " Max Non Aggres Suppress ");
	seq_printf(f, "| %10u |\n", TDOX_MAX_NON_AGGRESSIVE_SESSIONS);
	seq_printf(f, "| %-25s ", " Max Aggressive Suppress ");
	seq_printf(f, "| %10u |\n", UC_MAX_TDOX_SESSIONS - TDOX_MAX_NON_AGGRESSIVE_SESSIONS);
	seq_printf(f, "%s\n", dash_line_str);
}

static void __smgr_dbg_tdox_conf_set(char *args, void *data)
{
	struct smgr_tdox_conf conf;
	enum tdox_conf_update_ops opt;
	substring_t substr[MAX_OPT_ARGS];
	u32 val;
	char *tok;

	if (smgr_tdox_conf_get(&conf)) {
		pr_err("failed to get tdox conf parameters \n");
		return;
	}

	args = strim(args);
	while ((tok = strsep(&args, " \t\n,")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, tdox_conf_update_tkns, substr);
		switch (opt) {
		case tdox_update_help:
			tdox_conf_update_help();
			return;
		case tdox_update_tout:
			if (match_uint(substr, &val))
				goto opt_parse_err;
			conf.timeout = val;
			break;
		case tdox_update_max_reach_target:
			if (match_uint(substr, &val))
				goto opt_parse_err;
			conf.max_reach_target = val;
			break;
		case tdox_update_max_supp_ratio:
			if (match_uint(substr, &val))
				goto opt_parse_err;
			conf.max_supp_ratio = val;
			break;
		case tdox_update_max_supp_bytes:
			if (match_uint(substr, &val))
				goto opt_parse_err;
			conf.max_supp_bytes = val;
			break;
		default:
			goto opt_parse_err;
		}
	}

	if (smgr_tdox_conf_set(&conf))
		pr_err("failed to set tdox parameters\n");

	return;

opt_parse_err:
	tdox_conf_update_help();
}

PP_DEFINE_DEBUGFS(tdox_conf, __smgr_dbg_tdox_conf_get,
		  __smgr_dbg_tdox_conf_set);

static void __smgr_dbg_tdox_en_set(char *cmd_buf, void *data)
{
	u32 en_debug;

	if (kstrtou32(cmd_buf, 16, &en_debug)) {
		pr_info("failed to parse '%s'\n", cmd_buf);
		return;
	}

	smgr_tdox_enable_set(!!en_debug);
	return;
}

static void __smgr_dbg_tdox_en_get(struct seq_file *f)
{
	bool en = smgr_tdox_enable_get();

	pr_info("TDOX is %s\n", BOOL2EN(en));
}

PP_DEFINE_DEBUGFS(tdox_en_fops, __smgr_dbg_tdox_en_get, __smgr_dbg_tdox_en_set);

static void __smgr_dbg_tdox_entry_show(struct seq_file *f)
{
	smgr_tdox_debug_read_entry(f, U32_MAX);
}

PP_DEFINE_DEBUGFS(tdox_debug_entry_show, __smgr_dbg_tdox_entry_show, NULL);

struct debugfs_file debugfs_tdox_files[] = {
	{ "stats", &PP_DEBUGFS_FOPS(tdox_stats) },
	{ "clr_stats", &PP_DEBUGFS_FOPS(tdox_clr_stats) },
	{ "pps", &PP_DEBUGFS_FOPS(tdox_pps_stats) },
	{ "enable", &PP_DEBUGFS_FOPS(tdox_en_fops) },
	{ "config", &PP_DEBUGFS_FOPS(tdox_conf) },
	{ "show_entries", &PP_DEBUGFS_FOPS(tdox_debug_entry_show) },
};

s32 smgr_tdox_create_debugfs(struct dentry *dbgfs)
{
	return pp_debugfs_create(dbgfs, "tdox", NULL, debugfs_tdox_files,
				 ARRAY_SIZE(debugfs_tdox_files), NULL);
}
