/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
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
 * Description: PP micro-controllers Debug FS Interface
 */

#define pr_fmt(fmt) "[PP_UC_DBG]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/debugfs.h>
#include <linux/dma-mapping.h>
#include <linux/parser.h>
#include <linux/pp_api.h>
#include <linux/delay.h>

#include "pp_common.h"
#include "pp_regs.h"
#include "pp_debugfs_common.h"
#include "infra.h"
#include "uc.h"
#include "uc_dut.h"
#include "uc_regs.h"
#include "uc_internal.h"

/**
 * @brief main uc debugfs dir
 */
static struct dentry *dbgfs;

static u32 gcid, gtid;

static void uc_dbg_nf_cfg_set(char *cmd_buf, void *data)
{
	u16 pid, qos_port, tx_queue, uc_queue, host_q, subif;
	u32 nf;

	if (sscanf(cmd_buf, "%u %hu %hu %hu %hu %hu %hu",
		   &nf, &pid, &subif, &qos_port, &uc_queue, &tx_queue, &host_q)
		!= 6) {
		pr_err("sscanf error\n");
		return;
	}

	if (uc_nf_set(nf, pid, subif, qos_port, tx_queue, uc_queue, host_q, NULL)) {
		pr_err("failed to set nf\n");
		return;
	}

	pr_info("UC_TYPE[EGRESS] NF[%u] PID[%hu] SUBIF[%hu] QOS_RX_PORT[%hu] "
		"QOS_TX_QUEUE[%hu] UC_QUEUE[%hu]\n",
		nf, pid, subif, qos_port, tx_queue, uc_queue);
}

static void uc_dbg_nf_cfg_help(struct seq_file *f)
{
	seq_puts(f, "<nf 0=REASS, 1=TDOX, 2=FRAG, 3=MCAST, 4=IPSEC 5=REMARKING> <pid> <subif> <rx_qos_phy_port> <tx_qos_phy_queue>\n");
}

PP_DEFINE_DEBUGFS(nf_cfg, uc_dbg_nf_cfg_help, uc_dbg_nf_cfg_set);

static void uc_dbg_core_run_halt_set(char *cmd_buf, void *data)
{
	u8 uc_type, cpu_id, enable;

	if (unlikely(sscanf(cmd_buf, "%hhu %hhu %hhu",
			    &uc_type, &cpu_id, &enable) != 3)) {
		pr_err("sscanf error\n");
		return;
	}

	if (unlikely(uc_type > 1)) {
		pr_err("invalid uc cluster %hhu\n", uc_type);
		return;
	}

	uc_run_set(!!uc_type, (u32)cpu_id, !!enable);

	pr_info("UC_TYPE[%s] CPU_ID[%hhu] ACTION[%s]\n",
		uc_type ? "EGRESS" : "INGRESS", cpu_id,
		enable ?  "RUN"    : "HALT");
}

static void uc_dbg_core_run_halt_show(struct seq_file *f)
{
	seq_puts(f, "<uc cluster 0=ING, 1=EGR> <cpu id> <action 0=halt, 1=run>\n");
}

PP_DEFINE_DEBUGFS(core_run_halt, uc_dbg_core_run_halt_show,
		  uc_dbg_core_run_halt_set);

static void uc_dbg_cpu_enable_set(char *cmd_buf, void *data)
{
	u8 uc_type, cpu_id, enable;

	if (unlikely(sscanf(cmd_buf, "%hhu %hhu %hhu",
			    &uc_type, &cpu_id, &enable) != 3)) {
		pr_err("Command format error\n");
		pr_err("<uc cluster 0=ING, 1=EGR> <cpu id> <action 0=disable, 1=enable>\n");
		return;
	}

	if (unlikely(uc_type > 1)) {
		pr_err("invalid uc cluster %hhu\n", uc_type);
		return;
	}

	uc_ccu_enable_set(!!uc_type, (u32)cpu_id, !!enable);

	pr_info("UC_TYPE[%s] CPU_ID[%hhu] ACTION[%s]\n",
		uc_type ? "EGRESS" : "INGRESS", cpu_id,
		BOOL2EN(enable));
}

#define UC_CPU_AUX_RTC_LOW 0x104
static void uc_dbg_cpu_info_show(struct seq_file *f)
{
	u32 i, clk, clk_tmp, usage, sleep[2][UC_MAXCPU];
	u8 hw_max_cpu, uc_type, uc_types;
	bool act;

	if (unlikely(!uc_is_cluster_valid(UC_IS_EGR) &&
		     !uc_is_cluster_valid(UC_IS_ING))) {
		seq_puts(f, "all uc cpus are disabled\n");
		return;
	}

	/* read clock and sleep cycles */
	uc_aux_reg_read(UC_CPU_AUX_RTC_LOW, &clk);
	uc_types = 2;
	while (uc_types--) {
		uc_type = !!uc_types;
		if (!uc_is_cluster_valid(uc_type))
			continue;

		uc_ccu_maxcpus_get(uc_type, &hw_max_cpu);
		for (i = 0; i < hw_max_cpu; i++)
			sleep[uc_type][i] = uc_cpu_sleep_count(uc_type, i);
	}
	msleep(1000);
	uc_aux_reg_read(UC_CPU_AUX_RTC_LOW, &clk_tmp);
	clk = clk_tmp - clk;
	uc_types = 2;
	while (uc_types--) {
		uc_type = !!uc_types;
		if (!uc_is_cluster_valid(uc_type))
			continue;

		uc_ccu_maxcpus_get(uc_type, &hw_max_cpu);
		for (i = 0; i < hw_max_cpu; i++)
			sleep[uc_type][i] = uc_cpu_sleep_count(uc_type, i) -
					    sleep[uc_type][i];
	}

	seq_puts(f, "\n");
	seq_puts(f, " +------------------------------------------------------+\n");
	seq_puts(f, " |                 UC CPU STATUS                        |\n");
	seq_puts(f, " +------------------------------------------------------+\n");
	seq_puts(f, " |  CLUSTER  |  CPU ID  |  STATUS  |  Clock   |  Usage  |\n");
	seq_puts(f, " +-----------+----------+----------+----------+---------+\n");
	uc_types = 2;
	while (uc_types--) {
		uc_type = !!uc_types;
		if (unlikely(!uc_is_cluster_valid(uc_type)))
			continue;

		if (uc_ccu_maxcpus_get(uc_type, &hw_max_cpu)) {
			seq_printf(f, "failed to get the %s max cpus\n",
				   uc_type ? "egress" : "ingress");
			return;
		}

		for (i = 0; i < hw_max_cpu; i++) {
			act = uc_is_cpu_active(uc_type, i);
			seq_printf(f, " | %-9s |  %-6u  | %-8s |",
				   uc_type ? "EGRESS" : "INGRESS", i,
				   act ? "ACTIVE" : "INACTIVE");

			usage = 100 - (((u64)sleep[uc_type][i] * 100) / clk);
			seq_printf(f, "  %-3uMhz  |  %4u%%  |\n", clk / 1000000,
				   usage);
			seq_puts(f, " +-----------+----------+----------+----------+---------+\n");
		}
	}
	seq_puts(f, "\n");
}

PP_DEFINE_DEBUGFS(uc_cpu_info, uc_dbg_cpu_info_show, uc_dbg_cpu_enable_set);

static void uc_dbg_fat_set(char *cmd_buf, void *data)
{
	u8 uc_type;
	u32 ent, val, en;
	s32 ret;

	if (unlikely(sscanf(cmd_buf, "%hhu %x %x %u",
			    &uc_type, &ent, &val, &en) != 4)) {
		pr_err("Command format error\n");
		pr_err("<uc cluster 0=ING, 1=EGR> <fat entry> <val> <action 0=disable, 1=enable>\n");
		return;
	}

	if (unlikely(uc_type > 1)) {
		pr_err("invalid uc cluster %u\n", uc_type);
		return;
	}

	ret = uc_fat_entry_set(!!uc_type, ent, val, !!en);
	if (unlikely(ret)) {
		pr_err("failed to set the cluster bridge fat\n");
		return;
	}

	pr_info("UC_TYPE[%s] FAT_ENT[%#x] VAL[%#x] ACTION[%s]\n",
		uc_type ? "EGRESS" : "INGRESS", ent, val,
		BOOL2EN(en));
}

static void uc_dbg_fat_show(struct seq_file *f)
{
	u32 i, val, from;
	bool enable, cluster = (bool)f->private;
	u64 to;

	if (!uc_is_cluster_valid(cluster))
		return;

	seq_puts(f, " +----------------------------------------------------------+\n");
	if (cluster == UC_IS_EGR)
		seq_printf(f, " |         %-17s                                |\n",
			   "EGRESS FAT Table");
	else
		seq_printf(f, " |         %-17s                                |\n",
			   "INGRESS FAT Table");
	seq_puts(f, " +----------------------------------------------------------+\n");
	seq_puts(f, " | Entry | Enable  | Value      | Translation               |\n");
	seq_puts(f, " +-------+---------+------------+---------------------------+\n");

	for (i = 0; i < UC_FAT_ALL_ENT_CNT; i++) {
		if (uc_fat_entry_get(cluster, i, &val, &enable)) {
			seq_printf(f, "failed to get fat ent %u\n", i);
			break;
		}

		from = PP_FIELD_PREP(GENMASK_ULL(31, 26), i);
		to = uc_fat_addr_trans(cluster, from);

		seq_printf(f,
			   " | %#-5X | %-7s | %#-10X | %#-10x %s %#-11llx |\n",
			   i, BOOL2EN(enable), val, from,
			   (((u64)from) == to) ? "==" : "->", to);
	}
	seq_puts(f, " +-------+---------+------------+---------------------------+\n\n");
}
PP_DEFINE_DEBUGFS(fat, uc_dbg_fat_show, uc_dbg_fat_set);

static void uc_dbg_ccu_gpreg_set(char *cmd_buf, void *data)
{
	u8 uc_type;
	u32 gpreg_id, val;

	if (unlikely(sscanf(cmd_buf, "%hhu %u %u",
			    &uc_type, &gpreg_id, &val) != 3)) {
		pr_err("Command format error\n");
		pr_err("<uc cluster 0=ING, 1=EGR> <gpreg id> <val>\n");
		return;
	}

	if (unlikely(uc_type > 1)) {
		pr_err("invalid uc cluster %hhu\n", uc_type);
		return;
	}

	if (uc_ccu_gpreg_set(!!uc_type, gpreg_id, val)) {
		pr_err("failed to set gpreg %u\n", gpreg_id);
		return;
	}

	pr_info("UC_TYPE[%s] GPREG_ID[%u] VALUE[%u]\n",
		uc_type ? "EGRESS" : "INGRESS", gpreg_id, val);
}

static void uc_dbg_ccu_gpreg_show(struct seq_file *f)
{
	u32 i, val;
	u8 uc_types = 2;

	if (unlikely(!uc_is_cluster_valid(UC_IS_EGR) &&
		     !uc_is_cluster_valid(UC_IS_ING))) {
		seq_puts(f, "uc clusters are disabled\n");
		return;
	}

	seq_puts(f, "\n");
	seq_puts(f, " +---------------------------------+\n");
	seq_puts(f, " |          UC CCU GPREG           |\n");
	seq_puts(f, " +---------------------------------+\n");
	seq_puts(f, " |  CLUSTER  | GPREG ID |  VALUE   |\n");
	seq_puts(f, " +-----------+----------+----------+\n");
	while (uc_types--) {
		if (unlikely(!uc_is_cluster_valid(!!uc_types)))
			continue;

		for (i = 0; i < UC_CCU_GPREG_MAX; i++) {
			if (unlikely(uc_ccu_gpreg_get(!!uc_types, i, &val)))
				return;
			seq_printf(f, " | %-9s |  %-6u  |0x%08X|\n",
				   !!uc_types ? "EGRESS" : "INGRESS", i, val);
			seq_puts(f, " +-----------+----------+----------+\n");
		}
	}
	seq_puts(f, "\n");
}

PP_DEFINE_DEBUGFS(ccu_gpreg, uc_dbg_ccu_gpreg_show, uc_dbg_ccu_gpreg_set);

static void uc_dbg_ccu_irr_show(struct seq_file *f)
{
	u32 irr;
	u8 uc_types = 2;

	if (unlikely(!uc_is_cluster_valid(UC_IS_EGR) &&
		     !uc_is_cluster_valid(UC_IS_ING))) {
		seq_puts(f, "uc clusters are disabled\n");
		return;
	}

	seq_puts(f, "\n");
	seq_puts(f, " +---------------------------------+\n");
	seq_puts(f, " | UC CCU INTERRUPT REQUEST STATUS |\n");
	seq_puts(f, " +---------------------------------+\n");
	seq_puts(f, " |  CLUSTER  | CCU IRR REG VAL     |\n");
	seq_puts(f, " +-----------+---------------------+\n");
	while (uc_types--) {
		if (unlikely(!uc_is_cluster_valid(!!uc_types)))
			continue;

		if (uc_ccu_irr_get(!!uc_types, &irr)) {
			seq_printf(f, "failed to get the %s irr value\n",
				   !!uc_types ? "egress" : "ingress");
			return;
		}
		seq_printf(f, " | %-9s |     0x%08X      |\n",
			   !!uc_types ? "EGRESS" : "INGRESS", irr);
		seq_puts(f, " +-----------+---------------------+\n");
	}
	seq_puts(f, "\n");
}

PP_DEFINE_DEBUGFS(ccu_irr, uc_dbg_ccu_irr_show, NULL);

static void uc_dbg_reader_irr_show(struct seq_file *f)
{
	u32 irr;
	u8 uc_types = 2;

	if (unlikely(!uc_is_cluster_valid(UC_IS_EGR) &&
		     !uc_is_cluster_valid(UC_IS_ING))) {
		seq_puts(f, "uc clusters are disabled\n");
		return;
	}

	seq_puts(f, "\n");
	seq_puts(f, " +------------------------------------+\n");
	seq_puts(f, " | UC READER INTERRUPT REQUEST STATUS |\n");
	seq_puts(f, " +------------------------------------+\n");
	seq_puts(f, " |  CLUSTER  | READER IRR REG VAL     |\n");
	seq_puts(f, " +-----------+------------------------+\n");
	while (uc_types--) {
		if (unlikely(!uc_is_cluster_valid(!!uc_types)))
			continue;

		if (uc_reader_irr_get(!!uc_types, &irr)) {
			seq_printf(f, "failed to get the %s irr value\n",
				   !!uc_types ? "egress" : "ingress");
			return;
		}
		seq_printf(f, " | %-9s |     0x%08X         |\n",
			   !!uc_types ? "EGRESS" : "INGRESS", irr);
		seq_puts(f, " +-----------+------------------------+\n");
	}
	seq_puts(f, "\n");
}

PP_DEFINE_DEBUGFS(reader_irr, uc_dbg_reader_irr_show, NULL);

static void uc_dbg_version_show(struct seq_file *f)
{
	u32 ver;

	seq_puts(f, "\n");
	if (likely(!uc_ver_get(UC_IS_EGR, &ver))) {
		seq_printf(f, "EGRESS  : MAJOR[%u], MINOR[%u], BUILD[%u]\n",
			   (ver >> 16) & U8_MAX, (ver >> 8) & U8_MAX,
			   ver & U8_MAX);
	}

	if (likely(!uc_ver_get(UC_IS_ING, &ver))) {
		seq_printf(f, "INGRESS : MAJOR[%u], MINOR[%u], BUILD[%u]\n",
			   (ver >> 16) & U8_MAX, (ver >> 8) & U8_MAX,
			   ver & U8_MAX);
	}
}

PP_DEFINE_DEBUGFS(version, uc_dbg_version_show, NULL);

enum uc_log_opts {
	uc_log_opt_help = 1,
	uc_log_opt_reset,
	uc_log_opt_level,
};

static const match_table_t uc_log_tokens = {
	{ uc_log_opt_help,  "help" },
	{ uc_log_opt_reset, "reset" },
	{ uc_log_opt_level, "level=%u" },
	{ 0 },
};

static void __uc_log_help(void)
{
	pr_info("\n");
	pr_info(" Brief: Print/configure UC logger\n");
	pr_info(" Usage: echo <option>[=value] ... > log\n");
	pr_info(" Options:\n");
	pr_info("   help   - print this help\n");
	pr_info("   reset  - delete all logs\n");
	pr_info("   level  - set log level\n");
	pr_info(" Examples:\n");
	pr_info("   print log: cat log\n");
	pr_info("   reset log: echo reset > log\n");
}

static void __uc_log_set(char *args, void *data)
{
	substring_t substr[MAX_OPT_ARGS];
	enum uc_log_opts opt;
	char *tok;
	s32 val;
	bool is_egr = !!data;

	/* iterate over user arguments */
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, uc_log_tokens, substr);
		switch (opt) {
		case uc_log_opt_help:
			__uc_log_help();
			break;
		case uc_log_opt_reset:
			uc_log_reset(is_egr);
			break;
		case uc_log_opt_level:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			uc_log_level_set((enum uc_log_level)val, is_egr);
			break;
		default:
			pr_info("Error: Invalid option '%s'\n", tok);
			break;
		}
	}

	return;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
}

static void uc_dbg_log_show(struct seq_file *f)
{
	static const char *const level_str[] = { "FATAL", "ERROR", "WARN",
						 "INFO", "DEBUG" };
	struct uc_log_msg *msgs, *msg;

	size_t sz = UC_LOGGER_BUFF_SZ;
	bool is_egr = !!(f->private);

	msgs = kzalloc(sz, GFP_KERNEL);
	if (unlikely(!msgs)) {
		pr_err("failed to allocate %zu bytes buffer\n", sz);
		return;
	}
	uc_log_buff_info_get(msgs, sz, is_egr);

	seq_printf(f, "  UC %-7s Log\n", is_egr ? "Egress" : "Ingress");
	seq_puts(f, "===================\n");
	for_each_arr_entry(msg, msgs, sz / sizeof(*msg)) {
		if (strlen(msg->str) == 0)
			continue;
		seq_printf(f, "[%8x]:[%-5s]:[%u.%02u][%s]: %s %u, %#x\n",
			   msg->ts, level_str[msg->level], msg->cid, msg->tid,
			   msg->func, msg->str, msg->val, msg->val);
	}
	seq_puts(f, "\n");

	kfree(msgs);
}

PP_DEFINE_DEBUGFS(uc_egr_log, uc_dbg_log_show, __uc_log_set);
PP_DEFINE_DEBUGFS(uc_ing_log, uc_dbg_log_show, __uc_log_set);

enum uc_aux_reg_opts {
	uc_aux_reg_help = 1,
	uc_aux_reg_rd,
	uc_aux_reg_wr,
	uc_aux_reg_id,
	uc_aux_reg_val,
	uc_aux_reg_max
};

static const match_table_t uc_aux_reg_tokens = {
	{ uc_aux_reg_help, "help" },
	{ uc_aux_reg_rd,   "rd" },
	{ uc_aux_reg_wr,   "wr" },
	{ uc_aux_reg_id,   "id=%u" },
	{ uc_aux_reg_val,  "val=%x" },
	{ 0 },
};

static void __uc_aux_reg_help(void)
{
	pr_info("\n");
	pr_info(" Brief: Read/Write egress UC auxiliary registers\n");
	pr_info("        for full list of auxiliary registers see Figure 3-5\n");
	pr_info("        in ARCv2 Programmer's Reference Manual\n");
	pr_info(" Usage: echo <action> [option][=value] ... > eg_aux_reg\n");
	pr_info(" actions:\n");
	pr_info("   help   - print this help\n");
	pr_info("   rd     - read register\n");
	pr_info("   write  - write register\n");
	pr_info(" options:\n");
	pr_info("   id     - register id\n");
	pr_info("   value  - value to write\n");
	pr_info(" Examples:\n");
	pr_info("   read: echo read id=0x11 > aux_reg\n");
	pr_info("   reset echo write id=0xc5 0x50 > aux_reg\n");
}

static void __uc_aux_reg(char *args, void *data)
{
	substring_t substr[MAX_OPT_ARGS];
	enum uc_aux_reg_opts opt;
	enum uc_aux_reg_opts act = uc_aux_reg_max;
	char *tok;
	s32 tmp, ret;
	u32 reg = 0;
	u32 val;
	u64 tmp64;
	bool val_valid = false;

	/* iterate over user arguments */
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, uc_aux_reg_tokens, substr);
		switch (opt) {
		case uc_aux_reg_help:
		case uc_aux_reg_rd:
		case uc_aux_reg_wr:
			act = opt;
			break;
		case uc_aux_reg_id:
			if (match_int(&substr[0], &tmp))
				goto opt_parse_err;
			reg = tmp;
			break;
		case uc_aux_reg_val:
			if (match_u64(&substr[0], &tmp64))
				goto opt_parse_err;
			val = (u32)tmp64;
			val_valid = true;
			break;
		default:
			pr_info("Error: Invalid option '%s'\n", tok);
			break;
		}
	}

	switch (act) {
	case uc_aux_reg_help:
		__uc_aux_reg_help();
		break;
	case uc_aux_reg_rd:
		ret = uc_aux_reg_read(reg, &val);
		if (ret)
			pr_info("Failed to read aux reg %#x\n", reg);
		else
			pr_info("Auxiliary register %#x ==> %#x\n", reg,
				val);
		break;
	case uc_aux_reg_wr:
		if (!val_valid) {
			pr_info("value wasn't specified\n");
			break;
		}
		ret = uc_aux_reg_write(reg, val);
		if (ret)
			pr_info("Failed to write %#x to aux reg %#x\n", val,
				reg);
		else
			pr_info("Auxiliary register %#x <== %#x\n", reg,
				val);
		break;
	default:
		break;
	}

	return;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
}

PP_DEFINE_DEBUGFS(uc_aux_reg, NULL, __uc_aux_reg);

static void uc_tasks_show(struct seq_file *f)
{
	u8 cid, tid, num_cpu, num_tasks;
	u32 pc, status, queue;
	bool ready, active;
	bool uc_is_egr = !!(f->private);

	uc_ccu_maxcpus_get(uc_is_egr, &num_cpu);
	num_tasks = uc_is_egr ? UC_EGR_MAXTASK : UC_ING_MAXTASK;

	seq_puts(f, " +---------------------------------------------------------+\n");
	seq_puts(f, " | Task ID | PC         | STATUS32   | Ready | POPA Active |\n");
	seq_puts(f, " +---------+------------+------------+-------+-------------+\n");
	for (cid = 0; cid < num_cpu; cid++) {
		for (tid = 0; tid < num_tasks; tid++) {
			uc_task_pc_get(uc_is_egr, cid, tid, &pc);
			uc_task_status32_get(uc_is_egr, cid, tid, &status);
			uc_task_ready_get(uc_is_egr, cid, tid, &ready);
			uc_task_popa_active_get(uc_is_egr, cid, tid, &active,
						&queue);

			/** 0xe4000098 is a pc value for tasks which aren't
			 *  any tasks, it may change in the future
			 */
			if (pc == 0xe4000098)
				continue;

			seq_puts(f, " |");
			seq_printf(f, " CPU%u.%02u |", cid, tid);
			seq_printf(f, " %#010x |", pc);
			seq_printf(f, " %#010x |", status);
			seq_printf(f, " %-5s |", BOOL2STR(ready));
			if (active)
				seq_printf(f, " %-8s Q%u |\n", BOOL2STR(active),
					   queue);
			else
				seq_printf(f, " %-11s |\n", BOOL2STR(active));
		}
		seq_puts(f, " +---------+------------+------------+-------+-------------+\n");
	}
	seq_puts(f, " +---------------------------------------------------------+\n");
}

PP_DEFINE_DEBUGFS(egr_tasks, uc_tasks_show, NULL);
PP_DEFINE_DEBUGFS(ing_tasks, uc_tasks_show, NULL);

enum uc_task_set_opts {
	uc_task_set_tok_help = 1,
	uc_task_set_tok_cid,
	uc_task_set_tok_tid,
};

static const match_table_t uc_task_set_tokens = {
	{ uc_task_set_tok_help, "help" },
	{ uc_task_set_tok_cid,  "cid=%u" },
	{ uc_task_set_tok_tid,  "tid=%u" },
	{ 0 },
};

enum uc_msg_type_set_opts {
	uc_mbox_set_tok_help = 1,
	uc_msg_type = 2,
	uc_msg_param = 3,
	uc_msg_wait = 4
};

static const match_table_t uc_ing_msg_set_tokens = {
	{ uc_mbox_set_tok_help, "help" },
	{ uc_msg_type, "msg=%u" },
	{ uc_msg_param, "param=%u" },
	{ uc_msg_wait, "wait" },
	{ 0 },
};

static void uc_task_set_help(void)
{
	pr_info("\n");
	pr_info(" Usage: echo cid=<cpu id> tid=<task id> > egr_task\n");
	pr_info(" Usage: echo cid=<cpu id> tid=<task id> > ing_task\n");
}

static void uc_task_set(char *args, void *data)
{
	substring_t substr[MAX_OPT_ARGS];
	enum uc_task_set_opts opt;
	char *tok;
	s32 tmp;

	/* iterate over user arguments */
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, uc_task_set_tokens, substr);
		switch (opt) {
		case uc_task_set_tok_help:
			uc_task_set_help();
			return;
		case uc_task_set_tok_cid:
			if (match_int(&substr[0], &tmp))
				goto opt_parse_err;
			gcid = (u32)tmp;
			break;
		case uc_task_set_tok_tid:
			if (match_int(&substr[0], &tmp))
				goto opt_parse_err;
			gtid = (u32)tmp;
			break;
		default:
			pr_info("Error: Invalid option '%s'\n", tok);
			break;
		}
	}

	return;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
}

struct status32 {
	u32 halt:1,
	    irq_prio:4,
	    exception:1,
	    delay_branch:1,
	    user_mode:1,
	    overflow:1,
	    carry:1,
	    negative:1,
	    zero:1,
	    zero_oh:1,
	    zero_div_exp_en:1,
	    stack_check:1,
	    table_pend:1,
	    reg_bank:3,
	    align_check:1,
	    user_sleep_en:1,
	    secure_mode:1,
	    rsrv:9,
	    irq_en:1;
};

static void uc_task_show(struct seq_file *f)
{
	struct status32 status;
	u32 pc, queue, regs[32], i;
	bool ready, active;
	s32 ret = 0;
	bool uc_is_egr = !!(f->private);

	seq_puts(f, " +---------------------------------------------------------+\n");
	seq_puts(f, " | Task ID | PC         | STATUS32   | Ready | POPA Active |\n");
	seq_puts(f, " +---------+------------+------------+-------+-------------+\n");
	ret |= uc_task_pc_get(uc_is_egr, gcid, gtid, &pc);
	ret |= uc_task_status32_get(uc_is_egr, gcid, gtid, (u32 *)&status);
	ret |= uc_task_ready_get(uc_is_egr, gcid, gtid, &ready);
	ret |= uc_task_popa_active_get(uc_is_egr, gcid, gtid, &active, &queue);
	ret |= uc_task_regs_get(uc_is_egr, gcid, gtid, regs, ARRAY_SIZE(regs));

	if (ret)
		return;

	seq_puts(f, " |");
	seq_printf(f, " CPU%u.%02u |", gcid, gtid);
	seq_printf(f, " %#010x |", pc);
	seq_printf(f, " %#010x |", *((u32 *)&status));
	seq_printf(f, " %-5s |", BOOL2STR(ready));
	if (active)
		seq_printf(f, " %-8s Q%u |\n", BOOL2STR(active),
				queue);
	else
		seq_printf(f, " %-11s |\n", BOOL2STR(active));

	seq_puts(f, " +---------+------------+------------+-------+-------------+\n");
	seq_puts(f, " |    Status 32           |\n");
	seq_puts(f, " +--------------+---------+\n");
	seq_printf(f, " | %-12s | %-7s |\n", "Halt", BOOL2STR(status.halt));
	seq_printf(f, " | %-12s | %-7s |\n", "Overflow",
		   BOOL2STR(status.overflow));
	seq_printf(f, " | %-12s | %-7s |\n", "Exception",
		   BOOL2STR(status.exception));
	seq_printf(f, " | %-12s | %-7s |\n", "Zero", BOOL2STR(status.zero));
	seq_printf(f, " | %-12s | %-7s |\n", "Zero_div_exp",
		   BOOL2EN(status.zero_div_exp_en));
	seq_printf(f, " | %-12s | %-7s |\n", "Stack_check",
		   BOOL2EN(status.stack_check));
	seq_puts(f, " +--------------+---------+\n");
	seq_puts(f, " |    Registers           |\n");
	seq_puts(f, " +-----+------------------+\n");
	for (i = 0; i < ARRAY_SIZE(regs); i++)
		seq_printf(f, " | R%02u | %#010x       |\n", i, regs[i]);
	seq_puts(f, " +-----+------------------+\n");
}

PP_DEFINE_DEBUGFS(egr_task, uc_task_show, uc_task_set);
PP_DEFINE_DEBUGFS(ing_task, uc_task_show, uc_task_set);

static void uc_egr_writer_show(struct seq_file *f)
{
	u32 port, cid, queue, credits;
	bool en;
	u8 num_cpu;
	static const char *const port_str[UC_EGR_MAX_PORT] = {
		"MULTICAST", "REASSEMBLY", "FRAGMENTATION",
		"TURBODOX",  "IPSEC_LLD",  "REMARK",
	};

	uc_ccu_maxcpus_get(UC_IS_EGR, &num_cpu);

	seq_puts(f,
		 " +---------------------------------------------------------------+\n");
	seq_printf(f, " | %-16s", "Port");
	seq_printf(f, " | %-7s", "Enable");
	seq_printf(f, " | %-6s", "Credit");
	seq_printf(f, " | %-8s", "Mapping");
	seq_printf(f, " | %12s |\n", "Queue Credit");
	seq_puts(f,
		 " +------------------+---------+--------+----------+--------------+\n");

	for (port = 0; port < UC_EGR_MAX_PORT; port++) {
		uc_egr_writer_port_en_get(port, &en);
		uc_egr_writer_port_credit_get(port, &credits);
		seq_puts(f, " |");
		seq_printf(f, " %-2u-%-13s |", port, port_str[port]);
		seq_printf(f, " %-7s |", BOOL2EN(en));
		seq_printf(f, " %-6u |", credits);

		for (cid = 0; cid < (u32)num_cpu; cid++) {
			if (cid != 0)
				seq_puts(f,
					 " |                  |         |        |");
			uc_egr_writer_map_get(cid, port, &queue);
			uc_egr_writer_queue_credit_get(cid, port, &credits);
			seq_printf(f, " CPU%u: Q%u |", cid, queue);
			seq_printf(f, " %-12u |\n", credits);
		}
		seq_puts(f,
			 " |------------------+---------+--------+----------+--------------|\n");
	}
}

PP_DEFINE_DEBUGFS(egr_writer, uc_egr_writer_show, NULL);

static void uc_ingress_db_show(struct seq_file *f)
{
	struct ingress_uc_gdb *db;
	unsigned int major, minor, build;
	int ret;
	phys_addr_t db_base;

	db = kzalloc(sizeof(*db), GFP_KERNEL);
	if (!db)
		return;

	ret = uc_ing_db_get(db);
	if (ret) {
		seq_printf(f, "Failed to read ingress DB, ret %d\n", ret);
		return;
	}

	major = PP_FIELD_GET(0xFF0000, db->ver);
	minor = PP_FIELD_GET(0xFF00, db->ver);
	build = PP_FIELD_GET(0xFF, db->ver);
	db_base = pp_virt_to_phys(
		(void *)(long)UC_SSRAM(INGRESS, 0));

	seq_puts(f, "\n");
	seq_puts(f, " ==================================================\n");
	seq_puts(f, " |             Ingress uC Database                |\n");
	seq_puts(f, " ==================================================\n");
	seq_printf(f, " | %-25s | %pa |\n", "Base Address", &db_base);
	seq_printf(f, " | %-25s | %18zu |\n", "Size(Bytes)", sizeof(*db));
	seq_printf(f, " | %-25s | %10s%02u.%02u.%02u |\n", "Version", "", major,
		   minor, build);
	seq_printf(f, " | %-25s | %18u |\n", "Default Host Port",
		   db->host_port);
	seq_printf(f, " | %-25s | %18u |\n", "Default Phy Queue ID",
		   db->host_q);
	seq_printf(f, " | %-25s | %18u |\n", "Default Base Policy",
		   db->host_base_policy);
	seq_printf(f, " | %-25s | %#18x |\n", "Default Policies Bitmap",
		   db->host_policies_bmap);
	seq_puts(f, " ==================================================\n");
}

PP_DEFINE_DEBUGFS(ingress_db, uc_ingress_db_show, NULL);

static void uc_mbox_msg_set_help(void)
{
	pr_info("\n");
	pr_info(" Usage: echo [msg=<msgType>] [param=<paramVal>] [help] > ingress_mbox_msg\n");
	return;
}

static void uc_ingress_mbox_msg_set(char *args, void *data)
{
	struct uc_ing_cmd msg = {0};
	substring_t substr[MAX_OPT_ARGS];
	enum uc_msg_type_set_opts opt;
	char *tok;
	s32 tmp;
	u8 num = 0;
	bool wait = false;

	/* iterate over user arguments */
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, uc_ing_msg_set_tokens, substr);
		switch (opt) {
		case uc_msg_type:
			if (match_int(&substr[0], &tmp))
				goto opt_parse_err;
			msg.msg_type = (u32)tmp;
			break;
		case uc_msg_param:
			if (match_int(&substr[0], &tmp))
				goto opt_parse_err;
			msg.ing_cmd.cmd_1.param[num++] = (u32)tmp;
			break;
		case uc_msg_wait:
			wait = true;
			break;
		case uc_mbox_set_tok_help:
			uc_mbox_msg_set_help();
			return;
		default:
			uc_mbox_msg_set_help();
			break;
		}
	}

	if ((msg.msg_type <= ING_MBOX_INVALID) ||
	    (msg.msg_type >= ING_MBOX_MSG_MAX)) {
		pr_err("invalid msg type, mbox msg wasnt sent\n");
		return;
	}

	if (uc_ing_host_mbox_cmd_send(&msg, wait)) {
		pr_err("Failed to send msg to ingress mbox\n");
		return;
	}

	return;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
}

static void uc_ingress_mbox_msg_show(struct seq_file *f)
{
	struct ing_host_mbox_stat stat;
	u32 i;

	memset(&stat, 0, sizeof(stat));
	if (uc_ing_host_mbox_cmd_stats(&stat)) {
		seq_printf(f, "Failed to send msg to ingress mbox\n");
		seq_puts(f, "\n");
		return;
	}

	seq_printf(f, "===========================\n");
	for (i = ING_MBOX_MSG_1; i < ING_MBOX_MSG_MAX; i++)
		seq_printf(f, "msg type %d |  %u\n", i,
			   (unsigned int)stat.msg_recv_type[i]);
	seq_printf(f, "===========================\n");
	seq_printf(f, "Total msg received by UC  %u\n",
		   (unsigned int)stat.msg_recv_cnt);
	seq_printf(f, "Errors received by UC  %u\n",
		   (unsigned int)stat.msg_recv_err);
	seq_puts(f, "\n");
}

PP_DEFINE_DEBUGFS(ingress_mbox_msg, uc_ingress_mbox_msg_show,
		  uc_ingress_mbox_msg_set);

static void uc_ingress_soft_event_set(char *args, void *data)
{
	unsigned int id;
	int ret;
	ret = kstrtou32(args, 0, &id);
	if (ret) {
		pr_err("Failed to parse '%s', ret %d\n", args, ret);
		return;
	}
	if (id < ING_SOFT_EVENT_MAX)
		uc_ing_issue_soft_event_to_uc(id);
	else
		pr_err(" invalid idx %d\n", id);
}

static void uc_ingress_soft_event_show(struct seq_file *f)
{
	u8 i;
	for (i = 0 ; i < ING_SOFT_EVENT_MAX ; i++) {
		if (!uc_ing_is_soft_event_handled_by_uc(i))
			pr_info("soft event %d waiting to be handled\n", i);
	}
}

PP_DEFINE_DEBUGFS(ingress_soft_event, uc_ingress_soft_event_show,
		  uc_ingress_soft_event_set);

static void uc_whitelist_rule_hits_cnt_reset(char *args, void *data)
{
	unsigned int rule_type, rule_index;

	if (unlikely(sscanf(args, "%u %u", &rule_type, &rule_index) != 2)) {
		pr_err("sscanf error\n");
		return;
	}

	uc_ing_whitelist_rule_hits_reset(rule_type, rule_index);
}

static void uc_whitelist_rule_hits(char *args, void *data)
{
	unsigned int rule_type, rule_index, rule_hits;

	if (unlikely(sscanf(args, "%u %u", &rule_type, &rule_index) != 2)) {
		pr_err("sscanf error\n");
		return;
	}

	if (!uc_ing_whitelist_rule_hits_get(rule_type, rule_index, &rule_hits))
		pr_err("Rule type %u, rule index %u hits %u\n", rule_type,
		       rule_index, rule_hits);
}

PP_DEFINE_DEBUGFS(whitelist_rule_reset_cntrs, NULL,
		  uc_whitelist_rule_hits_cnt_reset);


PP_DEFINE_DEBUGFS(whitelist_rule_hits, NULL,
		  uc_whitelist_rule_hits);

static struct debugfs_file uc_dbg_files[] = {
	{"nf_cfg", &PP_DEBUGFS_FOPS(nf_cfg)},
	{"core_run_halt", &PP_DEBUGFS_FOPS(core_run_halt)},
	{"cpu_info", &PP_DEBUGFS_FOPS(uc_cpu_info)},
	{"ccu_gpreg", &PP_DEBUGFS_FOPS(ccu_gpreg)},
	{"ccu_irr", &PP_DEBUGFS_FOPS(ccu_irr)},
	{"reader_irr", &PP_DEBUGFS_FOPS(reader_irr)},
	{"aux_reg", &PP_DEBUGFS_FOPS(uc_aux_reg)},
	{"version", &PP_DEBUGFS_FOPS(version)},
	{"whitelist_reset_hits", &PP_DEBUGFS_FOPS(whitelist_rule_reset_cntrs)},
	{"whitelist_hits", &PP_DEBUGFS_FOPS(whitelist_rule_hits) },
};

static struct debugfs_file uc_egr_dbg_files[] = {
	{"egress_log", &PP_DEBUGFS_FOPS(uc_egr_log)},
	{"egress_tasks", &PP_DEBUGFS_FOPS(egr_tasks)},
	{"egress_task", &PP_DEBUGFS_FOPS(egr_task)},
	{"egress_writer", &PP_DEBUGFS_FOPS(egr_writer)},
	{"egress_fat", &PP_DEBUGFS_FOPS(fat), (void *)UC_IS_EGR},
};

static struct debugfs_file uc_ing_dbg_files[] = {
	{"ingress_tasks", &PP_DEBUGFS_FOPS(ing_tasks)},
	{"ingress_task", &PP_DEBUGFS_FOPS(ing_task)},
	{"ingress_db", &PP_DEBUGFS_FOPS(ingress_db)},
	{"ingress_soft_event", &PP_DEBUGFS_FOPS(ingress_soft_event)},
	{"ingress_mbox_msg", &PP_DEBUGFS_FOPS(ingress_mbox_msg)},
	{"ingress_log", &PP_DEBUGFS_FOPS(uc_ing_log)},
	{"ingress_fat", &PP_DEBUGFS_FOPS(fat), (void *)UC_IS_ING},
};

s32 uc_dbg_init(struct dentry *parent)
{
	int ret;

	if (unlikely(!debugfs_initialized())) {
		pr_err("Debugfs not initialized yet\n");
		return -EPERM;
	}

	/* uc debugfs dir */
	ret = pp_debugfs_create(parent, "uc", &dbgfs, uc_dbg_files,
				ARRAY_SIZE(uc_dbg_files), NULL);
	if (ret)
		return ret;

	ret = pp_debugfs_create(dbgfs, NULL, NULL, uc_ing_dbg_files,
				ARRAY_SIZE(uc_ing_dbg_files),
				(void *)UC_IS_ING);
	if (ret)
		return ret;

	ret = pp_debugfs_create(dbgfs, NULL, NULL, uc_egr_dbg_files,
				 ARRAY_SIZE(uc_egr_dbg_files),
				 (void *)UC_IS_EGR);
	if (ret)
		return ret;
#ifdef UC_DUT_ENABLE
	/* uc dut debugfs */
	if (uc_dut_dbg_init(dbgfs))
		return -ENOMEM;
#endif
	return 0;
}

void uc_dbg_clean(void)
{
	debugfs_remove_recursive(dbgfs);
	dbgfs = NULL;
#ifdef UC_DUT_ENABLE
	uc_dut_dbg_clean();
#endif
}
