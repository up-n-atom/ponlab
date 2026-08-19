/*
 * Copyright (C) 2024 MaxLinear, Inc.
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public
 * License along with this program; if not,see
 * <http://www.gnu.org/licenses/>.
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PP session manager LRO debugfs interface
 */

#define pr_fmt(fmt) "[PP_SESS_MGR_LRO_DBG]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/parser.h>

#include "pp_common.h"
#include "pp_debugfs_common.h"
#include "pp_session_mgr.h"
#include "pp_session_mgr_internal.h"

char *dash_line = "|----------------------------------------|";

enum lro_conf_update_ops {
	lro_update_help = 1,
	lro_update_queue,
	lro_update_max_pkts,
	lro_update_max_bytes,
	lro_update_pool,
	lro_update_policy,
};

static const match_table_t lro_conf_update_tkns = {
	{lro_update_help,              "help"},
	{lro_update_queue,         "queue=%u"},
	{lro_update_max_pkts,   "max_pkts=%u"},
	{lro_update_max_bytes, "max_bytes=%u"},
	{lro_update_pool,           "pool=%u"},
	{lro_update_policy,       "policy=%u"},
	{ 0 },
};

const char *lro_flds[] = {
	"queue",
	"max_packets",
	"max_bytes",
	"pool",
	"policy"
};

void lro_conf_update_help(void)
{
	u32 idx;

	pr_info("echo [fld]=[value]... > config\n");
	pr_info("Supported fields:\n");
	pr_info("=================\n");
	for (idx = 0; idx < ARRAY_SIZE(lro_flds); idx++)
		pr_info("%s\n", lro_flds[idx]);
}

static void __smgr_dbg_lro_conf_get(struct seq_file *f)
{
	struct smgr_lro_conf conf;

	if (smgr_lro_conf_get(&conf)) {
		pr_err("failed to get lro conf parameters \n");
		return;
	}

	seq_puts(f, "\n");
	seq_puts(f, "|========================================|\n");
	seq_puts(f, "|              LRO Parameters            |\n");
	seq_puts(f, "|========================================|\n");

	seq_printf(f, "%s\n", dash_line);
	seq_printf(f, "| %-25s ", " TX queue");
	seq_printf(f, "| %10u |\n", conf.tx_q);
	seq_printf(f, "| %-25s ", " Max Agg Bytes ");
	seq_printf(f, "| %10u |\n", conf.max_agg_bytes);
	seq_printf(f, "| %-25s ", " Max Agg Packets ");
	seq_printf(f, "| %10u |\n", conf.max_agg_pkts);
	seq_printf(f, "| %-25s ", " Buffer Pool ");
	seq_printf(f, "| %10u |\n", conf.pool);
	seq_printf(f, "| %-25s ", " Pool Policy ");
	seq_printf(f, "| %10u |\n", conf.policy);
	seq_printf(f, "%s\n", dash_line);
}

static void __smgr_dbg_lro_conf_set(char *args, void *data)
{
	struct smgr_lro_conf conf;
	enum lro_conf_update_ops opt;
	substring_t substr[MAX_OPT_ARGS];
	u32 val;
	char *tok;

	if (smgr_lro_conf_get(&conf)) {
		pr_err("failed to get lro conf parameters \n");
		return;
	}

	args = strim(args);
	while ((tok = strsep(&args, " \t\n,")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, lro_conf_update_tkns, substr);
		switch (opt) {
		case lro_update_help:
			lro_conf_update_help();
			return;
		case lro_update_queue:
			if (match_uint(substr, &val))
				goto opt_parse_err;
			conf.tx_q = val;
			break;
		case lro_update_max_pkts:
			if (match_uint(substr, &val))
				goto opt_parse_err;
			conf.max_agg_pkts = val;
			break;
		case lro_update_max_bytes:
			if (match_uint(substr, &val))
				goto opt_parse_err;
			conf.max_agg_bytes = val;
			break;
		case lro_update_pool:
			if (match_uint(substr, &val))
				goto opt_parse_err;
			conf.pool = val;
			break;
		case lro_update_policy:
			if (match_uint(substr, &val))
				goto opt_parse_err;
			conf.policy = val;
			break;
		default:
			goto opt_parse_err;
		}
	}

	if (smgr_lro_conf_set(&conf))
		pr_err("failed to set lro configurations\n");

	return;

opt_parse_err:
	lro_conf_update_help();
}

PP_DEFINE_DEBUGFS(lro_conf, __smgr_dbg_lro_conf_get, __smgr_dbg_lro_conf_set);

struct debugfs_file debugfs_lro_files[] = {
	{ "config", &PP_DEBUGFS_FOPS(lro_conf) },
};

s32 smgr_lro_create_debugfs(struct dentry *dbgfs)
{
	return pp_debugfs_create(dbgfs, "lro", NULL, debugfs_lro_files,
				 ARRAY_SIZE(debugfs_lro_files), NULL);
}