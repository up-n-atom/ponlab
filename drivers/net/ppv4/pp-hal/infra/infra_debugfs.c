/*
 * Copyright (C) 2020-2023 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation
 *
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
 * Description: PP Infrastructure Driver Debug FS Interface
 */

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/bitops.h>
#include <linux/debugfs.h>
#include <linux/pp_api.h>
#include <linux/parser.h>
#include "pp_logger.h"
#include "pp_common.h"
#include "pp_debugfs_common.h"
#include "infra.h"
#include "infra_internal.h"

/* 5 sec */
#define ASYNC_CYCLE_DELAY_M_SEC 5000000

/* Infra Debug FS directory */
static struct dentry *dbgfs;

struct service_log_db {
	struct workqueue_struct *workq;
	struct delayed_work dwork;
	bool print_srvs_log[SERVICE_LOG_NUM];
};

enum service_log_opts{
	service_log_opt_help = 1,
	service_log_opt_srvs_log_id,
	service_log_opt_enable,
	service_log_opt_clear,
	service_log_opt_print,
	service_log_opt_async_print,
};

static struct service_log_db *srvs_log_db;

/**
 * @brief Prints into seq_file PP boot and config relevant
 *        registers information
 * @param f seq_file
 * @note Used ONLY for debugfs prints
 */
static void __infra_bootcfg_show(struct seq_file *f)
{
	u32 version;
	bool sess_cache, cntrs_cache;
	bool rpb_done, port_dist_done;
	bool eg_uc_timers, ing_uc_timers;

	version        = infra_version_get();
	sess_cache     = infra_cls_sess_cache_en_get();
	cntrs_cache    = infra_cls_cntrs_cache_en_get();
	rpb_done       = infra_rpb_init_done_get();
	port_dist_done = infra_port_dist_init_done_get();
	eg_uc_timers   = infra_eg_uc_timers_clk_en_get();
	ing_uc_timers  = infra_ing_uc_timers_clk_en_get();

	seq_printf(f, "  %-25s: \t0x%x\n", "Boot config version", version);
	seq_printf(f, "  %-25s: \t%s\n", "Session cache", BOOL2EN(sess_cache));
	seq_printf(f, "  %-25s: \t%s\n", "Counters cache",
		   BOOL2EN(cntrs_cache));
	seq_printf(f, "  %-25s: \t%s\n", "Port Dist Memory Init",
		   port_dist_done ? "Done" : "No done");
	seq_printf(f, "  %-25s: \t%s\n", "RPB Memory Init",
		   rpb_done ? "Done" : "No done");
	seq_printf(f, "  %-25s: \t%s\n", "Egress UC Timers",
		   BOOL2EN(eg_uc_timers));
	seq_printf(f, "  %-25s: \t%s\n", "Ingress UC Timers",
		   BOOL2EN(ing_uc_timers));
}

/**
 * @brief Prints into buffer PP clock control relevant registers
 *        information
 * @param f seq_file
 * @param offset
 * @note Used ONLY for debugfs prints
 */
static void __infra_clock_ctrl_show(struct seq_file *f)
{
	u32 state, m;

	for_each_infra_cru_module(m) {
		infra_cru_mod_state_get(m, &state);
		seq_printf(f, "  %-13s CRU State  : \t%s(%u)\n",
			   infra_cru_module_str(m), infra_cru_state_str(state),
			   state);
	}
}

static void __infra_dynamic_clk_show(struct seq_file *f)
{
	u32 m, grace;
	bool en, on;

	infra_dclk_grace_get(&grace);
	seq_printf(f, "  %-25s: \t%u\n", "Grace", grace);

	for_each_infra_dclk_mod(m) {
		infra_mod_dclk_get(m, &en);
		infra_mod_dclk_status_get(m, &on);
		seq_printf(f, "  %-10s Dynamic Clock : \t%-7s, clock status: %s\n",
			   infra_dclk_module_str(m), BOOL2EN(en),
			   on ? "on" : "off");
	}
}

/**
 * @brief Debugfs status attribute show API, prints driver
 *        status including relevant registers information.
 * @param f seq_file
 * @param offset
 * @note Used ONLY for debugfs prints
 */
static void __infra_status_show(struct seq_file *f)
{
	seq_puts(f, "\n");
	seq_puts(f, " Infrastructure\n");
	seq_puts(f, " ===============================\n");
	seq_puts(f, "  Boot and Config Info\n");
	seq_puts(f, "  ------------------------------\n");
	__infra_bootcfg_show(f);
	seq_puts(f, "  ------------------------------\n");
	seq_puts(f, "  Clock Control Info\n");
	seq_puts(f, "  ------------------------------\n");
	__infra_clock_ctrl_show(f);
	seq_puts(f, "  ------------------------------\n");
	seq_puts(f, "  Dynamic Clock Info\n");
	seq_puts(f, "  ------------------------------\n");
	__infra_dynamic_clk_show(f);
	seq_puts(f, "  ------------------------------\n");
	seq_puts(f, "\n\n");
}

PP_DEFINE_DEBUGFS(status, __infra_status_show, NULL);

enum mod_cru_rst_opts {
	mod_cru_rst_opt_help = 1,
	mod_cru_rst_opt_mod,
};

static const match_table_t mod_cru_rst_tokens = {
	{mod_cru_rst_opt_help, "help"},
	{mod_cru_rst_opt_mod,  "mod=%u"},
	{ 0 },
};

/**
 * @brief Session delete help
 */
static void __infra_mod_cru_rst_help(void)
{
	u32 m;

	pr_info("\n");
	pr_info(" Usage: echo <option>[=value] > cru\n");
	pr_info(" Options:\n");
	pr_info("   help - print this help function\n");
	pr_info("   mod  - module's id\n");
	for_each_infra_cru_module(m)
		pr_info("          %u - %s\n", m, infra_cru_module_str(m));
	pr_info(" Examples:\n");
	pr_info("   reset module 3 cru:\n");
	pr_info("     echo mod=3 > mod_reset\n\n");
}

/**
 * @brief Set modules dynamic clock
 */
void __infra_mod_cru_reset(char *cmd_buf, void *data)
{
	substring_t substr[MAX_OPT_ARGS];
	enum mod_cru_rst_opts opt;
	char *tok, *args;
	int val, ret;
	enum cru_module m;

	m    = CRU_MOD_NUM;
	args = cmd_buf;
	args = strim(args);
	/* iterate over user arguments */
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, mod_cru_rst_tokens, substr);
		switch (opt) {
		case mod_cru_rst_opt_help:
			__infra_mod_cru_rst_help();
			return;
		case mod_cru_rst_opt_mod:
			if (match_int(&substr[0], &val))
				goto parse_err;
			m = (enum cru_module)val;
			break;
		default:
			goto parse_err;
		}
	}

	ret = infra_reset_hw(m);
	if (!ret)
		pr_info("%s CRU was reset\n", infra_cru_module_str(m));
	else
		pr_info("failed to reset %s CRU\n", infra_cru_module_str(m));
	return;

parse_err:
	pr_err("failed to parse %s\n", tok);
}

PP_DEFINE_DEBUGFS(reset_mod, NULL, __infra_mod_cru_reset);

enum mod_dclk_opts {
	mod_dclk_opt_help = 1,
	mod_dclk_opt_mod,
	mod_dclk_opt_en,
	mod_dclk_opt_grace
};

static const match_table_t mod_dclk_tokens = {
	{mod_dclk_opt_help,  "help"},
	{mod_dclk_opt_mod,   "mod=%u"},
	{mod_dclk_opt_en,    "en=%u"},
	{mod_dclk_opt_grace, "grace=%u"},
	{ 0 },
};

/**
 * @brief Session delete help
 */
static void __infra_mod_dclk_set_help(void)
{
	u32 m;

	pr_info("\n");
	pr_info(" Usage: echo <option>[=value] > dclock\n");
	pr_info(" Options:\n");
	pr_info("   help  - print this help function\n");
	pr_info("   en    - enable/disable\n");
	pr_info("   grace - grace cycles\n");
	pr_info("   mod   - module's id\n");
	for_each_infra_dclk_mod(m)
		pr_info("          %u - %s\n", m, infra_dclk_module_str(m));
	pr_info(" Examples:\n");
	pr_info("   enable module 5 dynamic clock:\n");
	pr_info("     echo mod=5 en=1 > dclock\n\n");
	pr_info("   change grace cycles:\n");
	pr_info("     echo grace=30 > dclock\n");
}

/**
 * @brief Set modules dynamic clock
 */
void __infra_mod_dclk_set(char *cmd_buf, void *data)
{
	substring_t substr[MAX_OPT_ARGS];
	enum mod_dclk_opts opt;
	char *tok, *args;
	int val, ret;
	enum dclk_module m;
	bool en;

	en    = false;
	m     = DCLK_MOD_NUM;
	args  = cmd_buf;
	args  = strim(args);
	/* iterate over user arguments */
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, mod_dclk_tokens, substr);
		switch (opt) {
		case mod_dclk_opt_help:
			__infra_mod_dclk_set_help();
			return;
		case mod_dclk_opt_mod:
			if (match_int(&substr[0], &val))
				goto parse_err;
			m = (enum dclk_module)val;
			break;
		case mod_dclk_opt_en:
			if (match_int(&substr[0], &val))
				goto parse_err;
			en = !!val;
			break;
		case mod_dclk_opt_grace:
			if (match_int(&substr[0], &val))
				goto parse_err;
			infra_dclk_grace_set((u32)val);
			return;
		default:
			goto parse_err;
		}
	}

	ret = infra_mod_dclk_set(m, en);
	if (!ret)
		pr_info("%s dynamic clock %s\n", infra_dclk_module_str(m),
			BOOL2EN(en));
	else
		pr_info("failed to %s %s dynamic clock\n", BOOL2EN(en),
			infra_dclk_module_str(m));
	return;

parse_err:
	pr_err("failed to parse %s\n", tok);
}

PP_DEFINE_DEBUGFS(mod_dclk, NULL, __infra_mod_dclk_set);

static const char *srvs_log_module_str[] = {
	[SERVICE_INGA]		= "INGA",
	[SERVICE_INGB]		= "INGB",
	[SERVICE_QOS]		= "QOS",
	[SERVICE_LOG_NUM]	= "ALL",
};

static const char *srvs_log_init_flow_str[] = {
	[BMGR_MAIN]		= "BMGR_MAIN",
	[CHK]			= "CHK",
	[CLS_HT]		= "CLS_HT",
	[CLS_SI]		= "CLS_SI",
	[CQM_MAIN_NSP]		= "CQM_MAIN_NSP",
	[EGRESS_UC_MAIN]	= "EGRESS_UC_MAIN",
	[INGRESS_UC_MAIN]	= "INGRESS_UC_MAIN",
	[PPV4_HOST_NSP]		= "PPV4_HOST_NSP",
	[QM_MAIN]		= "QM_MAIN",
	[QOS_RX_DMA_QUARY_RESP]	= "QOS_RX_DMA_QUARY_RESP",
	[QOS_UC_INST]		= "QOS_UC_INST",
	[QOS_UC_MAIN]		= "QOS_UC_MAIN",
	[RPB_MST]		= "RPB_MST",
	[RX_DMA_MAIN]		= "RX_DMA_MAIN",
	[RX_DMA_WRED_REQ]	= "RX_DMA_WRED_REQ",
	[TXMNG_MAIN]		= "TXMNG_MAIN",
	[INIT_FLOW_NONE]	= "NONE",
};

static const char *srvs_log_trgt_flow_str[] = {
	[BMGR_CFG]		= "BMGR_CFG",
	[BMGR_POP_PUSH]		= "BMGR_POP_PUSH",
	[BOOT_CFG]		= "BOOT_CFG",
	[CLK_CTRL]		= "CLK_CTRL",
	[CLS_CFG]		= "CLS_CFG",
	[CQM_CFG_NSP]		= "CQM_CFG_NSP",
	[DISTRIBUTOR_CFG]	= "DISTRIBUTOR_CFG",
	[EGRESS_UC_CFG]		= "EGRESS_UC_CFG",
	[EGRESS_UC_CL]		= "EGRESS_UC_CL",
	[INGRESS_ILA_CFG]	= "INGRESS_ILA_CFG",
	[INGRESS_UC_CL]		= "INGRESS_UC_CL",
	[IRC_CFG]		= "IRC_CFG",
	[MOD_CFG]		= "MOD_CFG",
	[PARSER_CFG]		= "PARSER_CFG",
	[PPV4_DDR_NSP]		= "PPV4_DDR_NSP",
	[QM_PUSH]		= "QM_PUSH",
	[QOS_HOST_IF]		= "QOS_HOST_IF",
	[QOS_RX_DMA_QUARY_REQ]  = "QOS_RX_DMA_QUARY_REQ",
	[RPB2_CFG]		= "RPB2_CFG",
	[RPB_CFG]		= "RPB_CFG",
	[RX_DMA_CFG]		= "RX_DMA_CFG",
	[RX_DMA_WRED_RESP]	= "RX_DMA_WRED_RESP",
	[TARGET_FLOW_NONE]	= "NONE",
};

static void print_observers(void)
{
	int i;

	for (i = SERVICE_INGA; i <= SERVICE_LOG_NUM; i++)
		pr_info("\t%d.\t%s\n", i, srvs_log_module_str[i]);
}

static void service_log_help(void)
{
	pr_info("Brief: Set service log arguments:\n");
	pr_info("Usage: echo [id] [en] [pr] [clr] > service_log\n");
	pr_info("All fields are optional, id must come first\n");
	pr_info("\thelp  - print this help\n");
	pr_info("\ten    - set 0 OR 1 to enable/diasble timeout errors\n");
	pr_info("\tpr    - set 0 OR 1 to print in cat service_log command\n");
	pr_info("\tclr   - clear the err register\n");
	pr_info("\tasync - set 0 OR 1 to checks in the background if there is an error,\n");
	pr_info("\t\tif any - prints it to the screen. NOT associated with given [id]\n");
	pr_info("[id]:");
	print_observers();
	pr_info("Example: echo id=2 en=0 clr > service_log\n");
	pr_info("***NOTE async is overloop on IDs that set to pr=1***\n");
}

static void is_err_callback_func(struct work_struct *work)
{
	int i;
	struct srvs_log_err err;

	if (ptr_is_null(srvs_log_db) || ptr_is_null(work))
		return;

	for (i = SERVICE_INGA; i < SERVICE_LOG_NUM; i++) {
		if (!srvs_log_db->print_srvs_log[i])
			continue;
		infra_service_log_get_err((enum service_log)i, &err);
		if (err.is_err) {
			pr_err("***ERROR in service log %s. init flow: %s, target flow: %s***\n",
				srvs_log_module_str[i],
				srvs_log_init_flow_str[err.init_flow],
				srvs_log_trgt_flow_str[err.target_flow]);
		}
	}

	queue_delayed_work(srvs_log_db->workq, &srvs_log_db->dwork,
			   usecs_to_jiffies(ASYNC_CYCLE_DELAY_M_SEC));
}

static struct workqueue_struct *__create_workqueue(void)
{
	u32 flags = WQ_UNBOUND;
	struct workqueue_struct *workq = NULL;

	/* Create work queue */
	workq = alloc_workqueue("%s", flags, 1, "service log");

	if (!workq)
		pr_err("Failed to create work queue");

	return workq;
}

static void service_log_en_set(enum service_log srvs_log, bool set)
{
	int i;

	if (srvs_log == SERVICE_LOG_NUM) {
		for (i = SERVICE_INGA; i < SERVICE_LOG_NUM; i++)
			service_log_en_set(i, set);
	} else {
		infra_service_log_fault_en_set((enum service_log)srvs_log, set);
	}
}

static void service_log_err_clr(enum service_log srvs_log)
{
	int i;

	if (srvs_log == SERVICE_LOG_NUM) {
		for (i = SERVICE_INGA; i < SERVICE_LOG_NUM; i++)
			service_log_err_clr(i);
	} else {
		infra_service_log_err_clear((enum service_log)srvs_log);
	}
}

static void service_log_print_set(enum service_log srvs_log, bool set)
{
	int i;

	if (srvs_log == SERVICE_LOG_NUM) {
		for (i = SERVICE_INGA; i < SERVICE_LOG_NUM; i++)
			service_log_print_set(i, set);
	} else {
		if (is_service_log_valid(srvs_log))
			srvs_log_db->print_srvs_log[srvs_log] = set;
	}
}

static void enable_async_print(void)
{
	if (!srvs_log_db->workq) {
		srvs_log_db->workq = __create_workqueue();
		if (srvs_log_db->workq) {
			INIT_DELAYED_WORK(&srvs_log_db->dwork,
					  is_err_callback_func);
			queue_delayed_work(
				srvs_log_db->workq, &srvs_log_db->dwork,
				usecs_to_jiffies(ASYNC_CYCLE_DELAY_M_SEC));
		}
	}
}

static void disable_async_print(void)
{
	if (srvs_log_db->workq) {
		cancel_delayed_work_sync(&srvs_log_db->dwork);
		destroy_workqueue(srvs_log_db->workq);
		srvs_log_db->workq = NULL;
	}
}

static void _service_log_dump(struct seq_file *f)
{
	int i;
	bool en;
	struct srvs_log_err err;

	if (ptr_is_null(srvs_log_db))
		return;

	for (i = SERVICE_INGA; i < SERVICE_LOG_NUM; i++) {
		if (!srvs_log_db->print_srvs_log[i])
			continue;
		if (infra_service_log_fault_en_get((enum service_log)i, &en))
			goto out;
		if (infra_service_log_get_err((enum service_log)i, &err))
			goto out;

		seq_printf(f, "| %-4s | %-17s | %-8s | %-21s | %-20s | %-15u | %-7u |\n",
			 srvs_log_module_str[i],
			 BOOL2EN(en), BOOL2EN(err.is_err),
			 srvs_log_init_flow_str[err.init_flow],
			 srvs_log_trgt_flow_str[err.target_flow],
			 err.subrange, err.seq_id);

		seq_puts(f, "+------+-------------------+----------+-----------------------+----------------------+-----------------+---------+\n");
	}

	return;

out:
	pr_err("error in service_log_dump\n");
}

static void service_log_dump(struct seq_file *f)
{
	seq_puts(f, "+------+-------------------+----------+-----------------------+----------------------+-----------------+---------+\n");
	seq_puts(f, "| name | is timeout enable | is error | init flow             | target flow          | target subrange | seq id  |\n");
	seq_puts(f, "+------+-------------------+----------+-----------------------+----------------------+-----------------+---------+\n");
	_service_log_dump(f);
}

static const match_table_t dbfs_tokens = {
	{service_log_opt_help,		"help"},
	{service_log_opt_srvs_log_id,	"id=%u"},
	{service_log_opt_enable,	"en=%u"},
	{service_log_opt_clear,		"clr"},
	{service_log_opt_print,		"pr=%u"},
	{service_log_opt_async_print,	"async=%u"},
	{ 0 },
};

static void service_log_set(char *cmd_buf, void *data)
{
	substring_t substr[MAX_OPT_ARGS];
	enum service_log_opts opt;
	enum service_log srvs_log = SERVICE_LOG_INVALID;
	char *tok;
	s32 val;

	if (ptr_is_null(srvs_log_db))
		return;

	cmd_buf = strim(cmd_buf);
	/* iterate over user arguments */
	while ((tok = strsep(&cmd_buf, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, dbfs_tokens, substr);
		switch (opt) {
		case service_log_opt_help:
			service_log_help();
			break;
		case service_log_opt_srvs_log_id:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			srvs_log = (enum service_log)val;
			break;
		case service_log_opt_enable:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			service_log_en_set(srvs_log, val);
			break;
		case service_log_opt_clear:
			service_log_err_clr(srvs_log);
			break;
		case service_log_opt_print:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			service_log_print_set(srvs_log, val);
			break;
		case service_log_opt_async_print:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			if (val)
				enable_async_print();
			else
				disable_async_print();
			break;
		}
	}
	return;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
}

PP_DEFINE_DEBUGFS(service_log, service_log_dump, service_log_set);

static struct debugfs_file debugfs_files[] = {
	{ "status",       &PP_DEBUGFS_FOPS(status) },
	{ "reset_module", &PP_DEBUGFS_FOPS(reset_mod) },
	{ "dclock",       &PP_DEBUGFS_FOPS(mod_dclk) },
	{ "service_log",  &PP_DEBUGFS_FOPS(service_log) }
};

static struct service_log_db *service_log_init_db(void)
{
	int i;
	struct service_log_db *db = kzalloc(sizeof(*db), GFP_KERNEL);

	if (ptr_is_null(db)) {
		pr_err("Failed to allocate service log db memory\n");
		return NULL;
	}

	for (i = SERVICE_INGA; i < SERVICE_LOG_NUM; i++)
		db->print_srvs_log[i] = true;

	return db;
}

s32 infra_dbg_init(struct dentry *parent)
{
	s32 res;

	if (unlikely(!debugfs_initialized())) {
		pr_err("Debugfs not initialized yet\n");
		return -EPERM;
	}

	/* infra debugfs dir */
	res = pp_debugfs_create(parent, "infra", &dbgfs, debugfs_files,
				 ARRAY_SIZE(debugfs_files), NULL);

	srvs_log_db = service_log_init_db();
	if (!srvs_log_db)
		pr_err("service log db is not initialized\n");

	return res;
}

s32 infra_dbg_clean(void)
{
	debugfs_remove_recursive(dbgfs);
	dbgfs = NULL;

	kfree(srvs_log_db);
	srvs_log_db = NULL;

	return 0;
}
