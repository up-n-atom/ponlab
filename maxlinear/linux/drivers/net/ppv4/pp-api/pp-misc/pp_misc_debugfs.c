/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation
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
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PP Misc Driver Debug FS Interface
 */

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/debugfs.h>
#include <linux/atomic.h>
#include <linux/pp_api.h>
#include <linux/jiffies.h>
#include <linux/parser.h>
#include "pp_common.h"
#include "pp_debugfs_common.h"
#include "pp_buffer_mgr.h"
#include "pp_dev.h"
#include "pp_misc.h"
#include "pp_misc_internal.h"
#include "rpb.h"
#include "bm.h"
#include "pp_desc.h"
#include "pp_session_mgr.h"
#include "pp_port_mgr.h"

/* PP infra Debug FS directory */
static struct dentry *dbgfs;

#if IS_ENABLED(CONFIG_OF)

struct misc_dbgfs_db {
	unsigned long addr;
	u32 num_regs;
	u32 regs_per_line;
};

struct misc_dbgfs_db *gdb;

#define REG_INVALID (0xFFFFFFFF)

static phys_addr_t ppv4_reg_base;

void regs_dump_rd(struct seq_file *f)
{
	seq_puts(f,
		 "<physical address> <num regs (optional)> <regs per line>\n");
}

void regs_dump_wr(char *cmd_buf, void *data)
{
	unsigned long addr;
	u32 num_regs = 1;
	u32 regs_per_line = 4;
	u32 reg, i, n = 0;
	char kbuf[128];
	void *virt;
	s32 ret;

	ret = sscanf(cmd_buf, "%lx %u %u", &addr, &num_regs, &regs_per_line);
	if (ret < 2) {
		ret = kstrtoul(cmd_buf, 0, &addr);
		if (ret) {
			pr_err("kstrtou32 err (%d)\n", ret);
			return;
		}
	}

	/* limit number of bytes to print */
	num_regs = num_regs > 4096 ? 4096 : num_regs;

	kbuf[0] = '\0';
	for (i = 0; i < num_regs; i++, addr += 4) {
		if (!(i % regs_per_line)) {
			if (i)
				pr_info("%s\n", kbuf);
			n = scnprintf(kbuf, sizeof(kbuf), "0x%08lx:\t", addr);
		}

		if (addr >= ppv4_reg_base) {
			/* For PPv4 registers use offset from virtual base */
			virt = pp_phys_to_virt(addr);
			if (virt) {
				reg = PP_REG_RD32(virt);
			} else {
				virt = ioremap(addr, 4);
				if (virt) {
					reg = PP_REG_RD32(virt);
					iounmap(virt);
				} else {
					reg = REG_INVALID;
				}
			}
		} else {
			/* For DDR use phys_to_virt */
			reg = *(volatile u32 *)phys_to_virt(addr);
		}

		n += scnprintf(kbuf + n, sizeof(kbuf) - n, "%08X ", reg);
	}
	pr_info("%s\n\n", kbuf);
}

void reg_write_32_rd(struct seq_file *f)
{
	seq_puts(f, "<physical address> <value>\n");
}

void reg_write_32_wr(char *cmd_buf, void *data)
{
	unsigned long addr;
	void *virt;
	u32 val;

	if (sscanf(cmd_buf, "%lx %x", &addr, &val) != 2) {
		pr_err("sscanf err\n");
		return;
	}

	if (addr >= ppv4_reg_base) {
		/* For PPv4 registers use offset from virtual base */
		virt = pp_phys_to_virt(addr);
		if (virt) {
			PP_REG_WR32(virt, val);
		} else {
			virt = ioremap(addr, 4);
			if (virt) {
				PP_REG_WR32(virt, val);
				iounmap(virt);
			}
		}
	} else {
		/* For DDR use phys_to_virt */
		*((u32 *)phys_to_virt(addr)) = val;
	}
}

PP_DEFINE_DEBUGFS(regs_rd, regs_dump_rd, regs_dump_wr);
PP_DEFINE_DEBUGFS(regs_wr, reg_write_32_rd, reg_write_32_wr);

void reg_dbg_rd(struct seq_file *f)
{
	unsigned long addr;
	u32 num_regs = 1;
	u32 regs_per_line = 4;
	u32 reg, i, n = 0;
	struct misc_dbgfs_db *db = f->private;
	char kbuf[128];
	void *virt;
	kbuf[0] = '\0';

	addr = db->addr;
	if (db->regs_per_line)
		regs_per_line = db->regs_per_line;
	if (db->num_regs)
		num_regs = db->num_regs;

	for (i = 0; i < num_regs; i++, addr += 4) {
		if (!(i % regs_per_line)) {
			if (i)
				seq_printf(f, "%s\n", kbuf);
			n = scnprintf(kbuf, sizeof(kbuf), "0x%08lx:\t", addr);
		}

		if (addr >= ppv4_reg_base) {
			/* For PPv4 registers use offset from virtual base */
			virt = pp_phys_to_virt(addr);
			if (virt) {
				reg = PP_REG_RD32(virt);
			} else {
				virt = ioremap(addr, 4);
				if (virt) {
					reg = PP_REG_RD32(virt);
					iounmap(virt);
				} else {
					reg = REG_INVALID;
				}
			}
		} else {
			/* For DDR use phys_to_virt */
			reg = *(volatile u32 *)phys_to_virt(addr);
		}

		n += scnprintf(kbuf + n, sizeof(kbuf) - n, "%08X ", reg);
	}
	seq_printf(f, "%s\n\n", kbuf);
}

void regs_dbg_wr(char *cmd_buf, void *data)
{
	struct misc_dbgfs_db *db = data;
	s32 ret;

	if (ptr_is_null(db))
		return;

	ret = sscanf(cmd_buf, "%lx %u %u", &db->addr, &db->num_regs, &db->regs_per_line);
	if (ret < 2) {
		ret = kstrtoul(cmd_buf, 0, &db->addr);
		if (ret) {
			pr_err("kstrtou32 err (%d)\n", ret);
			return;
		}
	}

	/* limit number of bytes to print */
	db->num_regs = db->num_regs > 4096 ? 4096 : db->num_regs;
}

PP_DEFINE_DEBUGFS(regs_dbg_rd, reg_dbg_rd, regs_dbg_wr);

void buf_dump_rd(struct seq_file *f)
{
	seq_puts(f, "<physical address> <len>\n");
}

void buf_dump_wr(char *cmd_buf, void *data)
{
	unsigned long addr, len = 1024;
	char *tok, *args;
	void *virt;
	bool unmap = false;

	args = strim(cmd_buf);
	/* get physical address */
	tok = strsep(&args, " \t\n");
	if (!tok || kstrtoul(tok, 0, &addr))
		goto parse_fail;
	/* get length to print */
	tok = strsep(&args, " \t\n");
	if (!tok)
		goto print;
	if (kstrtoul(tok, 0, &len))
		goto parse_fail;

print:
	/* limit number of bytes to print */
	len = len > 4096 ? 4096 : len;

	/* map the address if necessary */
	if (addr >= ppv4_reg_base) {
		virt = pp_phys_to_virt(addr);
		if (!virt) {
			virt = ioremap(addr, len);
			if (!virt)
				goto map_fail;
			unmap = true;
		}
	} else {
		/* For DDR use phys_to_virt */
		virt = phys_to_virt(addr);
	}

	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_ADDRESS, 32, 1, virt, len,
		       false);

	if (unmap)
		iounmap(virt);
	return;

parse_fail:
	pr_err("failed to parse args %s\n", cmd_buf);
	return;
map_fail:
	pr_err("failed to map address %#lx, len %lu\n", addr, len);
	return;
}

PP_DEFINE_DEBUGFS(buf_rd, buf_dump_rd, buf_dump_wr);

#endif

/**
 * @brief Set brief max lines
 */
static void __brief_max_cnt_set(char *cmd_buf, void *data)
{
	u32 cnt;

	if (kstrtou32(cmd_buf, 10, &cnt))
		pr_info("failed to parse '%s'\n", cmd_buf);

	pr_info("Brief max lines: %u\n", cnt);
	pp_misc_set_brief_cnt(cnt);
}

/**
 * @brief Reset all PP HW (HAL) statistics
 */
void __pp_global_reset(char *cmd_buf, void *data)
{
	if (strncmp(cmd_buf, "0", strlen("0")))
		return;

	pp_global_stats_reset();
}

static void __pp_pps_stats_show(struct seq_file *f)
{
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(ptr_is_null(qdev)))
		return;

	pp_pps_show_seq(f, sizeof(struct pp_global_stats), 1,
			pp_global_stats_get, pp_global_stats_diff,
			pp_global_stats_show, qdev);
}

PP_DEFINE_DEBUGFS(pps, __pp_pps_stats_show, __pp_global_reset);

/**
 * @brief Debugfs stats attribute show API, prints global PP
 *        statistics
 */
static void __pp_global_show(struct seq_file *f)
{
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(ptr_is_null(qdev)))
		return;

	pp_stats_show_seq(f, sizeof(struct pp_global_stats), 1,
			  pp_global_stats_get, pp_global_stats_show, qdev);
}

PP_DEFINE_DEBUGFS(global, __pp_global_show, __pp_global_reset);

static void __pp_hal_pps_stats_show(struct seq_file *f)
{
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(ptr_is_null(qdev)))
		return;

	pp_pps_show_seq(f, sizeof(struct pp_global_stats), 1,
			pp_global_stats_get, pp_global_stats_diff,
			pp_hal_global_stats_show, qdev);
}

PP_DEFINE_DEBUGFS(hal_pps, __pp_hal_pps_stats_show, __pp_global_reset);

static void __pp_brief_show(struct seq_file *f)
{
	struct smgr_stats stats;

	if (pp_smgr_stats_get(&stats))
		return;

	pp_pps_show_seq(f, sizeof(struct pp_active_sess_stats),
		atomic_read(&stats.sess_open), pp_brief_stats_get,
		pp_brief_stats_diff, pp_brief_stats_show, NULL);
}

PP_DEFINE_DEBUGFS(brief, __pp_brief_show, __brief_max_cnt_set);

/**
 * @brief Overloop on all services log, if find err indication prints it
 */
static void __pp_srvs_log_err_indication(struct seq_file *f)
{
	int i;
	struct srvs_log_err err = { 0 };

	for (i = SERVICE_INGA; i < SERVICE_LOG_NUM; i++) {
		infra_service_log_get_err((enum service_log)i, &err);
		if (err.is_err) {
			seq_puts(f, "+---------------------------+-------------------------+-------------------------+------------------------+\n");
			seq_puts(f, "***     There is error indication in one of the services log, you can try to clear the service.        ***\n");
			seq_puts(f, "***     For more information run 'cat hal/infra/service_log'                                           ***\n");
			return;
		}
	}
}

/**
 * @brief Debugfs stats attribute show API, prints global PP
 *        statistics
 */
static void __pp_hal_global_show(struct seq_file *f)
{
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(ptr_is_null(qdev)))
		return;

	__pp_srvs_log_err_indication(f);

	pp_stats_show_seq(f, sizeof(struct pp_global_stats), 1,
			  pp_global_stats_get, pp_hal_global_stats_show, qdev);
}

PP_DEFINE_DEBUGFS(hal_global, __pp_hal_global_show, __pp_global_reset);

/**
 * @brief Debugfs stats attribute show API, prints global PP
 *        statistics
 */
static void __pp_driver_stats_show(struct seq_file *f)
{
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(ptr_is_null(qdev)))
		return;

	pp_stats_show_seq(f, sizeof(struct pp_driver_stats), 1,
			  pp_driver_stats_get, pp_driver_stats_show, qdev);
}

void __pp_driver_stats_reset(char *cmd_buf, void *data)
{
	if (strncmp(cmd_buf, "0", strlen("0")))
		return;

	pp_driver_stats_reset();
}

PP_DEFINE_DEBUGFS(dstats, __pp_driver_stats_show, __pp_driver_stats_reset);

static void __pp_resource_stats_show(struct seq_file *f)
{
	pp_resource_stats_show(f->buf, f->size, &f->count);
}

PP_DEFINE_DEBUGFS(resources, __pp_resource_stats_show, NULL);

/**
 * @brief Debugfs version attribute show API, prints pp version
 */
void __pp_ver_show(struct seq_file *f)
{
	struct pp_version drv, fw, hw;
	s32 ret = 0;

	ret |= pp_version_get(&drv, PP_VER_TYPE_DRV);
	ret |= pp_version_get(&fw, PP_VER_TYPE_FW);
	ret |= pp_version_get(&hw, PP_VER_TYPE_HW);

	if (unlikely(ret)) {
		pr_err("Failed to get PP driver version\n");
		return;
	}

	ret = pp_version_show(f->buf, f->size, &f->count, &drv, &fw, &hw);
	if (unlikely(ret))
		pr_err("Failed to show PP driver version\n");
}

PP_DEFINE_DEBUGFS(version, __pp_ver_show, NULL);

/**
 * @brief Print descriptor format from DDR physical address
 * @param data unused
 * @param val DDR physical address of descriptor
 * @return 0 on success
 */
static int __pp_desc_show(void *data, u64 val)
{
	struct pp_desc desc;
	struct pp_hw_desc hw_desc;

	memcpy((void *)&hw_desc, phys_to_virt(val), sizeof(hw_desc));
	pp_desc_decode(&desc, &hw_desc);
	pp_desc_dump(&desc);

	return 0;
}

PP_DEFINE_DBGFS_ATT(desc, NULL, __pp_desc_show);

/**
 * @brief Print start of descriptor format from Debug registers
 *        physical address
 * @return 0 on success
 */
void __pp_rxdma_desc_show(struct seq_file *f)
{
	struct pp_desc desc;
	struct pp_hw_desc hw_desc;

	rx_dma_desc_dbg_get((struct pp_hw_dbg_desc *)(&hw_desc));
	pp_desc_top_decode(&desc, &hw_desc);
	pp_desc_top_dump(&desc);
}

PP_DEFINE_DEBUGFS(rxdma_desc, __pp_rxdma_desc_show, NULL);

void __pp_clk_freq_status_show(struct seq_file *f)
{
	u32 dbg_ignore_updates;

	dbg_ignore_updates = ignore_clk_updates_get();
	if (dbg_ignore_updates)
		seq_puts(f, "Listen to clk freq updates is disabled\n");
	else
		seq_puts(f, "Listen to clk freq updates is enabled\n");
}

void __pp_clk_freq_status_set(char *cmd_buf, void *data)
{
	u32 dbg_ignore_updates;

	if (kstrtou32(cmd_buf, 10, &dbg_ignore_updates))
		pr_info("failed to parse '%s'\n", cmd_buf);

	if (dbg_ignore_updates != 0 && dbg_ignore_updates != 1) {
		pr_info("/sys/kernel/debug/pp/misc/ignore_clk_updates help:\n");
		pr_info("echo 0 > /sys/kernel/debug/pp/misc/ignore_clk_updates : Allow updates to be processed by PPv4 driver\n");
		pr_info("echo 1 > /sys/kernel/debug/pp/misc/ignore_clk_updates : Ignore updates from being processed by PPv4 driver\n");
		return;
	}

	ignore_clk_updates_set(dbg_ignore_updates);
}

PP_DEFINE_DEBUGFS(ignore_clk_updates, __pp_clk_freq_status_show,
		  __pp_clk_freq_status_set);

void __free_running_ctr_show(struct seq_file *f)
{
	unsigned long addr;
	void *virt;
	u32  val = 0;

	addr = 0xF1050438;
	virt = pp_phys_to_virt(addr);
	if (virt) {
		val = PP_REG_RD32(virt);
	} else {
		virt = ioremap(addr, 4);
		if (virt) {
			val = PP_REG_RD32(virt);
			iounmap(virt);
		}
	}

	seq_printf(f, "Free Running Counter: %u\n", val);
}

PP_DEFINE_DEBUGFS(free_running_ctr, __free_running_ctr_show, NULL);

/**
 * @brief Get PP driver hw clock frequency
 */
static int __misc_dbg_clk_get(void *data, u64 *val)
{
	*val = (u64)pp_dev_hw_clk_get();

	return 0;
}

PP_DEFINE_DBGFS_ATT(clock, __misc_dbg_clk_get, NULL);

enum nf_opts {
	nf_opt_help = 1,
	nf_opt_type,
	nf_opt_pid,
	nf_opt_subif,
	nf_opt_dst_q,
	nf_opt_c2_q,
};

static const match_table_t nf_tokens = {
	{nf_opt_help,  "help"},
	{nf_opt_type,  "type=%u"},
	{nf_opt_pid,   "pid=%u"},
	{nf_opt_subif, "subif=%u"},
	{nf_opt_dst_q, "dst_q=%u"},
	{nf_opt_c2_q,  "c2_q=%u"},
	{ 0 },
};

static const char *pp_nf_str[PP_NF_NUM] = {
	[PP_NF_REASSEMBLY] = "Reassembly",
	[PP_NF_TURBODOX] = "TurboDox",
	[PP_NF_FRAGMENTER] = "Fragmenter",
	[PP_NF_MULTICAST] = "Multicast",
	[PP_NF_IPSEC_LLD] = "IPSEC_LLD",
	[PP_NF_REMARKING] = "Remarking",
};

static const char *pp_qos_sf_q_type_str[PP_QOS_SF_QUEUE_TYPE_NUM] = {
	[PP_QOS_SF_QUEUE_TYPE_LOW]  = "low",
	[PP_QOS_SF_QUEUE_TYPE_HIGH] = "high",
	[PP_QOS_SF_QUEUE_TYPE_MGMT] = "management",
};

static const char *pp_qos_aqm_mode_str[PP_QOS_AQM_MODE_NUM] = {
	[PP_QOS_AQM_MODE_NORMAL]  = "normal",
	[PP_QOS_AQM_MODE_NO_DROP] = "no drop",
};

/**
 * @brief SGC file help
 */
static void __dbg_nf_set_help(void)
{
	u32 type = 0;

	pr_info("\n");
	pr_info(" Brief: Set PP network function\n");
	pr_info(" Usage: echo type=<type> pid=<pid> subif=<subif> dst_q=<dst_q> c2_q=<c2_q> > nf_set\n");
	pr_info("   help  - print this help\n");
	pr_info("   pid   - PP pid\n");
	pr_info("   subif - DP subif\n");
	pr_info("   dst_q - logical destination queue\n");
	pr_info("   c2_q - cycle 2 queue\n");
	pr_info("   type  - nf type\n");

	for (type = 0; type < PP_NF_NUM; type++)
		pr_info("      %u - %s\n", type, pp_nf_str[type]);
}

static s32 __dbg_nf_set_args_parse(char *args_str, enum pp_nf_type *type,
				   struct pp_nf_info *info, bool *help)
{
	substring_t substr[MAX_OPT_ARGS];
	enum nf_opts opt;
	char *tok;
	s32 val;

	args_str = strim(args_str);
	/* iterate over user arguments */
	while ((tok = strsep(&args_str, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, nf_tokens, substr);
		switch (opt) {
		case nf_opt_help:
			*help = true;
			return 0;
		case nf_opt_type:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			*type = (enum pp_nf_type)val;
			break;
		case nf_opt_pid:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			info->pid = (u16)val;
			break;
		case nf_opt_subif:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			info->subif = (u16)val;
			break;
		case nf_opt_dst_q:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			info->q = (u16)val;
			break;
		case nf_opt_c2_q:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			info->cycl2_q = (u16)val;
			break;
		}
	}
	return 0;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
	return -EINVAL;
}

static void __dbg_nf_set(char *args_str, void *data)
{
	enum pp_nf_type type;
	struct pp_nf_info info = { 0 };
	bool help;
	s32 ret = 0;

	help = false;
	type = PP_NF_NUM;
	info.pid = U16_MAX;
	info.q = U16_MAX;

	ret = __dbg_nf_set_args_parse(args_str, &type, &info, &help);
	if (unlikely(ret))
		return;

	if (help) {
		__dbg_nf_set_help();
		return;
	}

	if (unlikely(pp_nf_set(type, &info, NULL)))
		pr_err("failed to set nf, ret %d\n", ret);
}

/**
 * @brief Dump the nf multicast info
 */
static void __dbg_nf_get(struct seq_file *f)
{
	struct pp_nf_info info = { 0 };
	enum pp_nf_type type = 0;
	s32 ret;

	seq_puts(f, "+------------|-------|-------|------------|--------------+\n");
	seq_printf(f, "| %-10s | %-5s | %-5s | %-10s | %-12s |\n",
		   "Type", "PID", "SUBIF", "Dest Queue", "Cycle2 Queue");
	seq_puts(f, "+------------|-------|-------|------------|--------------+\n");

	for (type = 0; type < PP_NF_NUM; type++) {
		ret = pp_nf_get(type, &info);
		if (unlikely(ret)) {
			seq_printf(f, "failed to get %s nf info, ret %d\n",
				   pp_nf_str[type], ret);
			return;
		}
		seq_printf(f, "| %-10s | %-5u | %-5u | %-10u | %-12u |\n",
			   pp_nf_str[type], info.pid, info.subif, info.q,
			   info.cycl2_q);
	}
	seq_puts(f, "+------------|-------|-------|------------|--------------+\n");
}

PP_DEFINE_DEBUGFS(nf, __dbg_nf_get, __dbg_nf_set);

void pp_uc_rule_ip_mac_print(struct seq_file *f, struct ing_wl_rules *db)
{
	ulong rules_index_en;
	u8 rule_index;
	ulong fields_en;
	u32 rule_hits;
	struct wl_rule_ip_mac *rule;

	seq_puts(f, " |====================================================================================================|\n");
	seq_puts(f, " |=======================================    IP MAC  =================================================|\n");
	seq_puts(f, " |====================================================================================================|\n");
	seq_printf(f, " | %-4s", "ID");
	seq_printf(f, " | %-10s", "Hits");
	seq_printf(f, " | %-8s", "Priority");
	seq_printf(f, " | %-22s", "MAC");
	seq_printf(f, " | %-44s|\n", "IP");
	seq_puts(f, " |======|============|==========|========================|============================================|\n");

	rules_index_en = db->wl_rule_index_enabled[ING_WL_RULE_TYPE_IP_MAC];
	for_each_set_bit (rule_index, &rules_index_en,
			  WL_RULE_IP_AND_MAC_MAX_RULES) {
		rule = &db->rule_ip_mac[rule_index];
		fields_en = (ulong)rule->common.fields_en;
		uc_ing_whitelist_rule_hits_get(ING_WL_RULE_TYPE_IP_MAC,
					       rule_index, &rule_hits);
		seq_printf(f, " | %-4u", rule_index);
		seq_printf(f, " | %-10u", rule_hits);
		seq_printf(f, " | %-8u", rule->common.priority);
		if (test_bit(UC_WL_FLD_SRC_MAC, &fields_en))
			seq_printf(f, " | Src: %pM", rule->mac);
		else if (test_bit(UC_WL_FLD_DST_MAC, &fields_en))
			seq_printf(f, " | Dst: %pM", rule->mac);

		if (test_bit(UC_WL_FLD_IPV4_DST_IP, &fields_en))
			seq_printf(f, " | Dst: %-39pI4n", &rule->ip.v4);
		else if (test_bit(UC_WL_FLD_IPV4_SRC_IP, &fields_en))
			seq_printf(f, " | Src: %-39pI4n", &rule->ip.v4);
		else if (test_bit(UC_WL_FLD_IPV6_DST_IP, &fields_en))
			seq_printf(f, " | Dst: %-39pI6c", &rule->ip.v6);
		else if (test_bit(UC_WL_FLD_IPV6_SRC_IP, &fields_en))
			seq_printf(f, " | Src: %-39pI6c", &rule->ip.v6);
		else
			seq_printf(f, " | %-44s", "Dst: N/A");
		seq_puts(f, "|\n");
	}
}

void pp_uc_rule_mac_print(struct seq_file *f, struct ing_wl_rules *db)
{
	ulong rules_index_en;
	u8 rule_index;
	ulong fields_en;
	struct wl_rule_mac *rule;
	u32 rule_hits;

	seq_puts(f, " |=======================================================|\n");
	seq_puts(f, " |================== MAC ================================|\n");
	seq_puts(f, " |=======================================================|\n");
	seq_printf(f, " | %-4s", "ID");
	seq_printf(f, " | %-10s", "Hits");
	seq_printf(f, " | %-8s", "Priority");
	seq_printf(f, " | %-22s |\n", "MAC");
	seq_puts(f, " |======|============|==========|========================|\n");

	rules_index_en = db->wl_rule_index_enabled[ING_WL_RULE_TYPE_MAC];
	for_each_set_bit(rule_index, &rules_index_en, WL_RULE_MAC_MAX_RULES) {
		rule = &db->rule_mac[rule_index];
		fields_en = (ulong)rule->common.fields_en;
		uc_ing_whitelist_rule_hits_get(ING_WL_RULE_TYPE_MAC, rule_index,
					       &rule_hits);
		seq_printf(f, " | %-4u", rule_index);
		seq_printf(f, " | %-10u", rule_hits);
		seq_printf(f, " | %-8u", rule->common.priority);
		if (test_bit(UC_WL_FLD_SRC_MAC, &fields_en))
			seq_printf(f, " | Src: %pM", rule->mac);
		else if (test_bit(UC_WL_FLD_DST_MAC, &fields_en))
			seq_printf(f, " | Dst: %pM", rule->mac);

		seq_puts(f, "|\n");
	}
}

void pp_uc_rule_eth_print(struct seq_file *f, struct ing_wl_rules *db)
{
	ulong rules_index_en;
	u8 rule_index;
	struct wl_rule_eth *rule;
	u32 rule_hits;

	seq_puts(f, " |========================================|\n");
	seq_puts(f, " |=========  ETH_TYPE  ===================|\n");
	seq_puts(f, " |========================================|\n");
	seq_printf(f, " | %-4s", "ID");
	seq_printf(f, " | %-10s", "Hits");
	seq_printf(f, " | %-8s", "Priority");
	seq_printf(f, " | %-8s|\n", "ETH Type");
	seq_puts(f, " |======|============|==========|=========|\n");

	rules_index_en = db->wl_rule_index_enabled[ING_WL_RULE_TYPE_ETH_TYPE];
	for_each_set_bit(rule_index, &rules_index_en, WL_RULE_ETH_MAX_RULES) {
		rule = &db->rule_eth[rule_index];
		uc_ing_whitelist_rule_hits_get(ING_WL_RULE_TYPE_ETH_TYPE,
					       rule_index, &rule_hits);
		seq_printf(f, " | %-4u", rule_index);
		seq_printf(f, " | %-10u", rule_hits);
		seq_printf(f, " | %-8u", rule->common.priority);
		seq_printf(f, " | %#06x  ", ntohs(rule->eth_type));

		seq_puts(f, "|\n");
	}
}

void pp_uc_rule_ip_proto_ports_print(struct seq_file *f, struct ing_wl_rules *db)
{
	ulong rules_index_en;
	u8 rule_index;
	struct wl_rule_ip_proto_ports *rule;
	u32 rule_hits;
	ulong fields_en;

	seq_puts(f, " |======================================================================|\n");
	seq_puts(f, " |================ IP PROTO / PORTS  ===================================|\n");
	seq_puts(f, " |======================================================================|\n");
	seq_printf(f, " | %-4s", "ID");
	seq_printf(f, " | %-10s", "Hits");
	seq_printf(f, " | %-8s", "Priority");
	seq_printf(f, " | %-11s", "IP Protocol");
	seq_printf(f, " | %-10s", "Src: Port");
	seq_printf(f, " | %-10s |\n", "Dst: Port");
	seq_puts(f, " |======|============|==========|=============|============|============|\n");

	rules_index_en =
		db->wl_rule_index_enabled[ING_WL_RULE_TYPE_IP_PROTO_PORTS];
	for_each_set_bit (rule_index, &rules_index_en,
			  WL_RULE_IP_PROTO_PORTS_MAX_RULES) {
		rule = &db->rule_ip_proto_ports[rule_index];
		uc_ing_whitelist_rule_hits_get(ING_WL_RULE_TYPE_IP_PROTO_PORTS,
					       rule_index, &rule_hits);
		fields_en = (ulong)rule->common.fields_en;
		seq_printf(f, " | %-4u", rule_index);
		seq_printf(f, " | %-10u", rule_hits);
		seq_printf(f, " | %-8u", rule->common.priority);
		seq_printf(f, " | %-11u", rule->ip_proto);
		if (test_bit(PP_FLD_L4_SRC_PORT, &fields_en))
			seq_printf(f, " | %-10u", ntohs(rule->src_port));
		else
			seq_printf(f, " | %-10s", "ANY");
		if (test_bit(PP_FLD_L4_DST_PORT, &fields_en))
			seq_printf(f, " | %-10u", ntohs(rule->dst_port));
		else
			seq_printf(f, " | %-10s", "ANY");
		seq_puts(f, " |\n");
	}
}

void pp_uc_rule_icmp_print(struct seq_file *f, struct ing_wl_rules *db)
{
	ulong rules_index_en;
	u8 rule_index;
	struct wl_rule_icmp *rule;
	u32 rule_hits;

	seq_puts(f, " |========================================================|\n");
	seq_puts(f, " |================  ICMP  ================================|\n");
	seq_puts(f, " |========================================================|\n");
	seq_printf(f, " | %-4s", "ID");
	seq_printf(f, " | %-10s", "Hits");
	seq_printf(f, " | %-8s", "Priority");
	seq_printf(f, " | %-10s", "ICMP type");
	seq_printf(f, " | %-10s |\n", "ICMP code");
	seq_puts(f, " |======|============|==========|============|============|\n");

	rules_index_en = db->wl_rule_index_enabled[ING_WL_RULE_TYPE_ICMP];
	for_each_set_bit(rule_index, &rules_index_en, WL_RULE_ICMP_MAX_RULES) {
		rule = &db->rule_icmp[rule_index];
		uc_ing_whitelist_rule_hits_get(ING_WL_RULE_TYPE_ICMP,
					       rule_index, &rule_hits);
		seq_printf(f, " | %-4u", rule_index);
		seq_printf(f, " | %-10u", rule_hits);
		seq_printf(f, " | %-8u", rule->common.priority);
		seq_printf(f, " | %-10u", rule->icmp_type);
		seq_printf(f, " | %-10u", rule->icmp_code);

		seq_puts(f, " |\n");
	}
}

void pp_uc_rule_ipv4_ports_print(struct seq_file *f, struct ing_wl_rules *db)
{
	ulong rules_index_en;
	u8 rule_index;
	ulong fields_en;
	struct wl_rule_ipv4_ports *rule;
	u32 rule_hits;

	seq_puts(f, " |==============================================================================================|\n");
	seq_puts(f, " |============  IPV4 AND PORTS  ================================================================|\n");
	seq_puts(f, " |==============================================================================================|\n");
	seq_printf(f, " | %-4s", "ID");
	seq_printf(f, " | %-10s", "Hits");
	seq_printf(f, " | %-8s", "Priority");
	seq_printf(f, " | %-21s", "IP");
	seq_printf(f, " | %-11s", "IP Protocol");
	seq_printf(f, " | %-10s", "Src: Port");
	seq_printf(f, " | %-10s |\n", "Dst: Port");
	seq_puts(f, " |======|============|==========|=======================|=============|============|============|\n");

	rules_index_en = db->wl_rule_index_enabled
				 [ING_WL_RULE_TYPE_IPV4_SRC_OR_DST_AND_L4_PORTS];
	for_each_set_bit (rule_index, &rules_index_en,
			  WL_RULE_IPV4_SRC_OR_DST_AND_L4_PORTS_MAX_RULES) {
		rule = &db->rule_ipv4_ports[rule_index];
		uc_ing_whitelist_rule_hits_get(
			ING_WL_RULE_TYPE_IPV4_SRC_OR_DST_AND_L4_PORTS,
			rule_index, &rule_hits);
		fields_en = (ulong)rule->common.fields_en;
		seq_printf(f, " | %-4u", rule_index);
		seq_printf(f, " | %-10u", rule_hits);
		seq_printf(f, " | %-8u", rule->common.priority);
		if (test_bit(UC_WL_FLD_IPV4_DST_IP, &fields_en))
			seq_printf(f, " | Dst: %-16pI4n", &rule->v4);
		else if (test_bit(UC_WL_FLD_IPV4_SRC_IP, &fields_en))
			seq_printf(f, " | Src: %-16pI4n", &rule->v4);
		seq_printf(f, " | %-11u", rule->ip_proto);
		seq_printf(f, " | %-10u", ntohs(rule->src_port));
		seq_printf(f, " | %-10u", ntohs(rule->dst_port));
		seq_puts(f, " |\n");
	}
}

void pp_uc_rule_two_ipv4_ports_print(struct seq_file *f,
				     struct ing_wl_rules *db)
{
	ulong rules_index_en;
	u8 rule_index;
	struct wl_rule_double_ipv4_ports *rule;
	u32 rule_hits;

	seq_puts(f, " |======================================================================================================================|\n");
	seq_puts(f, " |============  Two IPV4 AND PORTS  ====================================================================================|\n");
	seq_puts(f, " |======================================================================================================================|\n");
	seq_printf(f, " | %-4s", "ID");
	seq_printf(f, " | %-10s", "Hits");
	seq_printf(f, " | %-8s", "Priority");
	seq_printf(f, " | %-21s", "Dst IP:");
	seq_printf(f, " | %-21s", "Src IP:");
	seq_printf(f, " | %-11s", "IP Protocol");
	seq_printf(f, " | %-10s", "Src: Port");
	seq_printf(f, " | %-10s |\n", "Dst: Port");
	seq_puts(f, " |======|============|==========|=======================|=======================|=============|============|============|\n");

	rules_index_en = db->wl_rule_index_enabled
				 [ING_WL_RULE_TYPE_IPV4_SRC_AND_DST_AND_L4_PORTS];
	for_each_set_bit (rule_index, &rules_index_en,
			  WL_RULE_IPV4_SRC_AND_DST_AND_L4_PORTS_MAX_RULES) {
		rule = &db->rule_double_ipv4_ports[rule_index];
		uc_ing_whitelist_rule_hits_get(
			ING_WL_RULE_TYPE_IPV4_SRC_AND_DST_AND_L4_PORTS,
			rule_index, &rule_hits);
		seq_printf(f, " | %-4u", rule_index);
		seq_printf(f, " | %-10u", rule_hits);
		seq_printf(f, " | %-8u", rule->common.priority);
		seq_printf(f, " | %-21pI4n", &rule->dst);
		seq_printf(f, " | %-21pI4n", &rule->src);
		seq_printf(f, " | %-11u", rule->ip_proto);
		seq_printf(f, " | %-10u", ntohs(rule->src_port));
		seq_printf(f, " | %-10u", ntohs(rule->dst_port));
		seq_puts(f, " |\n");
	}
}

void pp_uc_rule_ipv6_ports_print(struct seq_file *f, struct ing_wl_rules *db)
{
	ulong rules_index_en;
	u8 rule_index;
	ulong fields_en;
	struct wl_rule_ipv6_ports *rule;
	u32 rule_hits;

	seq_puts(f, " |=====================================================================================================================|\n");
	seq_puts(f, " |============  IPV6 AND PORTS  =======================================================================================|\n");
	seq_puts(f, " |=====================================================================================================================|\n");
	seq_printf(f, " | %-4s", "ID");
	seq_printf(f, " | %-10s", "Hits");
	seq_printf(f, " | %-8s", "Priority");
	seq_printf(f, " | %-44s", "IP");
	seq_printf(f, " | %-11s", "IP Protocol");
	seq_printf(f, " | %-10s", "Src: Port");
	seq_printf(f, " | %-10s |\n", "Dst: Port");
	seq_puts(f, " |======|============|==========|==============================================|=============|============|============|\n");

	rules_index_en =
		db->wl_rule_index_enabled
			[ING_WL_RULE_TYPE_IPV6_SRC_OR_DST_AND_L4_PORTS];
	for_each_set_bit (rule_index, &rules_index_en,
			  WL_RULE_IPV6_SRC_OR_DST_AND_L4_PORTS_MAX_RULES) {
		rule = &db->rule_ipv6_ports[rule_index];
		uc_ing_whitelist_rule_hits_get(
			ING_WL_RULE_TYPE_IPV6_SRC_OR_DST_AND_L4_PORTS,
			rule_index, &rule_hits);
		fields_en = (ulong)rule->common.fields_en;
		seq_printf(f, " | %-4u", rule_index);
		seq_printf(f, " | %-10u", rule_hits);
		seq_printf(f, " | %-8u", rule->common.priority);
		if (test_bit(UC_WL_FLD_IPV6_DST_IP, &fields_en))
			seq_printf(f, " | Dst: %-39pI6c", &rule->v6);
		else if (test_bit(UC_WL_FLD_IPV6_SRC_IP, &fields_en))
			seq_printf(f, " | Src: %-39pI6c", &rule->v6);
		seq_printf(f, " | %-11u", rule->ip_proto);
		seq_printf(f, " | %-10u", ntohs(rule->src_port));
		seq_printf(f, " | %-10u", ntohs(rule->dst_port));
		seq_puts(f, " |\n");
	}
}

void pp_misc_rules_show(struct seq_file *f, struct ing_wl_rules *rules)
{
	ulong type_en, type;

	if (!f || !rules)
		return;
	if (!rules->wl_rule_type_bit_enabled)
		return;

	type_en = rules->wl_rule_type_bit_enabled;
#ifdef DEBUG_WL_RULES_TIME
	seq_printf(f, " | RULE  |    time ticks   |\n");
	seq_printf(f, " | TOTAL | %-15d |\n", rules->rule_time[0]);

	for_each_set_bit(type, &type_en, ING_WL_RULE_TYPE_COUNT) {
		seq_printf(f, " | %-4d  | %-15d |\n", type,
			   rules->rule_time[type]);
	}
	seq_puts(f, "\n");
#endif
	for_each_set_bit(type, &type_en, ING_WL_RULE_TYPE_COUNT) {
		switch (type) {
		case ING_WL_RULE_TYPE_IP_MAC:
			pp_uc_rule_ip_mac_print(f, rules);
			break;
		case ING_WL_RULE_TYPE_MAC:
			pp_uc_rule_mac_print(f, rules);
			break;
		case ING_WL_RULE_TYPE_ETH_TYPE:
			pp_uc_rule_eth_print(f, rules);
			break;
		case ING_WL_RULE_TYPE_IP_PROTO_PORTS:
			pp_uc_rule_ip_proto_ports_print(f, rules);
			break;
		case ING_WL_RULE_TYPE_ICMP:
			pp_uc_rule_icmp_print(f, rules);
			break;
		case ING_WL_RULE_TYPE_IPV4_SRC_OR_DST_AND_L4_PORTS:
			pp_uc_rule_ipv4_ports_print(f, rules);
			break;
		case ING_WL_RULE_TYPE_IPV4_SRC_AND_DST_AND_L4_PORTS:
			pp_uc_rule_two_ipv4_ports_print(f, rules);
			break;
		case ING_WL_RULE_TYPE_IPV6_SRC_OR_DST_AND_L4_PORTS:
			pp_uc_rule_ipv6_ports_print(f, rules);
			break;
		case ING_WL_RULE_TYPE_BITHASH:
		case ING_WL_RULE_TYPE_INVALID:
			break;
		default:
			seq_puts(f, "unknown rule type\n");
			return;
		}
	}

	seq_puts(f, "\n");
}

/* print only whitelist group rules */
void pp_uc_whitelist_rules_show(struct seq_file *f)
{
	struct uc_gpid_group *uc_grp = NULL;
	struct ing_wl_rules *rules = NULL;
	int grp_id;

	if (!f)
		return;

	rules = kzalloc(sizeof(*rules), GFP_KERNEL);
	if (!rules)
		return;
	uc_grp = kzalloc(sizeof(*uc_grp), GFP_KERNEL);
	if (!uc_grp)
		goto free_mem;

	grp_id = pp_misc_whitelist_grp_id_get();
	if (uc_ing_whitelist_db_get(rules))
		goto free_mem;
	if (pmgr_gpid_group_info_get(grp_id, NULL, uc_grp, NULL))
		goto free_mem;

	rules->wl_rule_type_bit_enabled = uc_grp->wl_rule_type_bmap_en;
	memcpy(rules->wl_rule_index_enabled, uc_grp->wl_rule_index_bmap_en,
	       sizeof(uc_grp->wl_rule_index_bmap_en));
	pp_misc_rules_show(f, rules);

free_mem:
	kfree(rules);
	kfree(uc_grp);
}
EXPORT_SYMBOL(pp_uc_whitelist_rules_show);

/* prints all whitelist rules */
static void _pp_uc_whitelist_rules_show(struct seq_file *f)
{
	struct ing_wl_rules *rules;
	int ret;

	if (!f)
		return;

	rules = kzalloc(sizeof(*rules), GFP_KERNEL);
	if (!rules)
		return;

	ret = uc_ing_whitelist_db_get(rules);
	if (ret) {
		seq_printf(f, "Fail to read whitelist rules, ret %d\n", ret);
		goto free_db;
	}

	seq_puts(f, " ==================================================\n");
	seq_puts(f, " |        Ingress uC Whitelist Database           |\n");
	seq_puts(f, " ==================================================\n");
	pp_misc_rules_show(f, rules);
	seq_puts(f, "\n");

free_db:
	kfree(rules);
}

PP_DEFINE_DEBUGFS(whitelist_rules, _pp_uc_whitelist_rules_show, NULL);

void __pp_uc_whitelist_rules_reset(char *cmd_buf, void *data)
{
	uc_ing_whitelist_reset_all_rules();
}
PP_DEFINE_DEBUGFS(whitelist_rules_reset, NULL, __pp_uc_whitelist_rules_reset);

void __pp_uc_whitelist_rules_test(char *cmd_buf, void *data)
{
	uc_ing_whitelist_rules_test();
}
PP_DEFINE_DEBUGFS(whitelist_rules_test, NULL, __pp_uc_whitelist_rules_test);

void pp_uc_hashbit_enable_show(struct seq_file *f)
{
	struct hash_bit_db *db;
	ulong bmap[BITS_TO_LONGS(PP_DPL_HASH_BIT_MAX_ENTRIES)] = { 0 };
	int ret, i;
	u8 cpu;

	if (!f)
		return;

	if (uc_ccu_maxcpus_get(UC_IS_ING, &cpu))
		return;

	db = kcalloc(cpu, sizeof(*db), GFP_KERNEL);
	if (!db)
		return;

	ret = uc_ing_hash_bit_db_get(db, cpu);
	if (ret) {
		seq_printf(f, "Failed to read ingress DB, ret %d\n", ret);
		goto free_mem;
	}

	seq_puts(f, " +---------------");
	for (i = 0 ; i < cpu ; i++)
		seq_puts(f, "+---------------");
	seq_puts(f, "-+\n");

	seq_puts(f, " |               ");
	for (i = 0 ; i < cpu ; i++)
		seq_printf(f, "| UC - %u        ", i);
	seq_puts(f, " |\n");

	seq_puts(f, " +---------------");
	for (i = 0 ; i < cpu ; i++)
		seq_puts(f, "+---------------");
	seq_puts(f, "-+\n");

	seq_printf(f, " | %-13s ", "FOUND");
	for (i = 0 ; i < cpu ; i++)
		seq_printf(f, "| %13u ", db[i].bitHashFound);
	seq_puts(f, " |\n");

	seq_printf(f, " | %-13s ", "NOT FOUND");
	for (i = 0 ; i < cpu ; i++)
		seq_printf(f, "| %13u ", db[i].bitHashNotFound);
	seq_puts(f, " |\n");

	seq_printf(f, " | %-13s ", "HIGH PRI");
	for (i = 0 ; i < cpu ; i++)
		seq_printf(f, "| %13u ", db[i].priorityHigherThenBitHash);
	seq_puts(f, " |\n");

	/* dump the hash bit from the first CPU */
	bitmap_from_arr32(bmap, db[0].hash_bit, PP_DPL_HASH_BIT_MAX_ENTRIES);
	seq_printf(f, " | %-13s | %*pbl\n", "Hash Bits Map",
		   PP_DPL_HASH_BIT_MAX_ENTRIES, bmap);
	seq_puts(f, "\n");
free_mem:
	kfree(db);
}

static void uc_hash_bit_enable_show(struct seq_file *f)
{
	seq_puts(f, " ==================================================\n");
	seq_puts(f, " |        Ingress uC hashbit enable Database      |\n");

	pp_uc_hashbit_enable_show(f);
	seq_puts(f, "\n");
}
PP_DEFINE_DEBUGFS(hash_bit, uc_hash_bit_enable_show, NULL);

enum sf_set_opts {
	sf_set_opt_help = 1,
	sf_set_opt_id,
	sf_set_opt_llsf,
	sf_set_opt_coupled,
	sf_set_opt_num_q,
	sf_set_opt_q,
	sf_set_opt_q_type,
	sf_set_opt_target,
	sf_set_opt_peak,
	sf_set_opt_msr,
	sf_set_opt_coupling_factor,
	sf_set_opt_amsr,
	sf_set_opt_msr_l,
	sf_set_opt_weight,
	sf_set_opt_buf_sz,
	sf_set_opt_aqm_mode,
	sf_set_opt_iaqm,
	sf_set_opt_qp,
	sf_set_opt_maxth,
	sf_set_opt_lg_aging,
	sf_set_opt_lg_range,
	sf_set_opt_critical_ql,
	sf_set_opt_critical_ql_score,
	sf_set_opt_vq_interval,
	sf_set_opt_vq_ewma_alpha,
	sf_set_opt_num_hist_bins,
	sf_set_opt_bin_edge,
};

static const match_table_t sf_set_tokens = {
	{sf_set_opt_help,              "help"},
	{sf_set_opt_id,                "id=%u"},
	{sf_set_opt_llsf,              "llsf=%u"},
	{sf_set_opt_coupled,           "coupled=%u"},
	{sf_set_opt_num_q,             "num_q=%u"},
	{sf_set_opt_q,                 "q=%u"},
	{sf_set_opt_q_type,            "q_type=%u"},
	{sf_set_opt_target,            "target=%u"},
	{sf_set_opt_peak,              "peak=%u"},
	{sf_set_opt_msr,               "msr=%u"},
	{sf_set_opt_coupling_factor,   "factor=%u"},
	{sf_set_opt_amsr,              "amsr=%u"},
	{sf_set_opt_msr_l,             "msr_l=%u"},
	{sf_set_opt_weight,            "weight=%u"},
	{sf_set_opt_buf_sz,            "buf_sz=%u"},
	{sf_set_opt_aqm_mode,          "mode=%u"},
	{sf_set_opt_iaqm,              "iaqm=%u"},
	{sf_set_opt_qp,                "qp=%u"},
	{sf_set_opt_maxth,             "maxth=%u"},
	{sf_set_opt_lg_aging,          "aging=%u"},
	{sf_set_opt_lg_range,          "range=%u"},
	{sf_set_opt_critical_ql,       "crit_ql=%u"},
	{sf_set_opt_critical_ql_score, "crit_ql_score=%u"},
	{sf_set_opt_vq_interval,       "interval=%u"},
	{sf_set_opt_vq_ewma_alpha,     "alpha=%u"},
	{sf_set_opt_num_hist_bins,     "num_bins=%u"},
	{sf_set_opt_bin_edge,          "bin=%u"},
	{ 0 },
};

static s32 sf_set_args_parse(char *args_str,
			     struct pp_qos_aqm_lld_sf_config *cfg,
			     u8 *sf_id,
			     bool *help)
{
	substring_t substr[MAX_OPT_ARGS];
	enum sf_set_opts opt;
	char *tok;
	s32 val;
	u16 q_idx = 0;
	u16 q_type_idx = 0;
	u16 bin_idx = 0;

	args_str = strim(args_str);
	/* iterate over user arguments */
	while ((tok = strsep(&args_str, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, sf_set_tokens, substr);
		switch (opt) {
		case sf_set_opt_help:
			*help = true;
			return 0;
		case sf_set_opt_id:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			*sf_id = (u8)val;
			val = pp_misc_sf_conf_get(*sf_id, cfg);
			if (val) {
				pr_err("failed to get sf %u config, ret %d\n",
				       *sf_id, val);
				return -EINVAL;
			}
			break;
		case sf_set_opt_llsf:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			cfg->llsf = (u8)val;
			break;
		case sf_set_opt_coupled:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			cfg->coupled_sf = (u8)val;
			break;
		case sf_set_opt_num_q:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			cfg->num_queues = (u32)val;
			break;
		case sf_set_opt_q:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			cfg->queue[q_idx++].id = (u32)val;
			break;
		case sf_set_opt_q_type:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			cfg->queue[q_type_idx++].type = (u32)val;
			break;
		case sf_set_opt_target:
			if (match_int(&substr[0], &val) || cfg->llsf)
				goto opt_parse_err;
			cfg->cfg.aqm_cfg.latency_target_ms = (u32)val;
			break;
		case sf_set_opt_peak:
			if (match_int(&substr[0], &val) || cfg->llsf)
				goto opt_parse_err;
			cfg->cfg.aqm_cfg.peak_rate = (u32)val;
			break;
		case sf_set_opt_msr:
			if (match_int(&substr[0], &val) || cfg->llsf)
				goto opt_parse_err;
			cfg->cfg.aqm_cfg.msr = (u32)val;
			break;
		case sf_set_opt_coupling_factor:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			cfg->coupling_factor = (u32)val;
			break;
		case sf_set_opt_amsr:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			cfg->amsr = (u32)val;
			break;
		case sf_set_opt_msr_l:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			cfg->msr_l = (u32)val;
			break;
		case sf_set_opt_weight:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			cfg->weight = (u32)val;
			break;
		case sf_set_opt_buf_sz:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			cfg->buffer_size = (u32)val;
			break;
		case sf_set_opt_aqm_mode:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			cfg->aqm_mode = ((u32)val == PP_QOS_AQM_MODE_NO_DROP) ?
				PP_QOS_AQM_MODE_NO_DROP : PP_QOS_AQM_MODE_NORMAL;
			break;
		case sf_set_opt_iaqm:
			if (match_int(&substr[0], &val) || !cfg->llsf)
				goto opt_parse_err;
			cfg->cfg.lld_cfg.iaqm_en = (u8)val;
			break;
		case sf_set_opt_qp:
			if (match_int(&substr[0], &val) || !cfg->llsf)
				goto opt_parse_err;
			cfg->cfg.lld_cfg.qp_en = (u8)val;
			break;
		case sf_set_opt_maxth:
			if (match_int(&substr[0], &val) || !cfg->llsf)
				goto opt_parse_err;
			cfg->cfg.lld_cfg.maxth_us = (u16)val;
			break;
		case sf_set_opt_lg_aging:
			if (match_int(&substr[0], &val) || !cfg->llsf)
				goto opt_parse_err;
			cfg->cfg.lld_cfg.lg_aging = (u8)val;
			break;
		case sf_set_opt_lg_range:
			if (match_int(&substr[0], &val) || !cfg->llsf)
				goto opt_parse_err;
			cfg->cfg.lld_cfg.lg_range = (u8)val;
			break;
		case sf_set_opt_critical_ql:
			if (match_int(&substr[0], &val) || !cfg->llsf)
				goto opt_parse_err;
			cfg->cfg.lld_cfg.critical_ql_us = (u16)val;
			break;
		case sf_set_opt_critical_ql_score:
			if (match_int(&substr[0], &val) || !cfg->llsf)
				goto opt_parse_err;
			cfg->cfg.lld_cfg.critical_ql_score_us = (u16)val;
			break;
		case sf_set_opt_vq_interval:
			if (match_int(&substr[0], &val) || !cfg->llsf)
				goto opt_parse_err;
			cfg->cfg.lld_cfg.vq_interval = (u32)val;
			break;
		case sf_set_opt_vq_ewma_alpha:
			if (match_int(&substr[0], &val) || !cfg->llsf)
				goto opt_parse_err;
			cfg->cfg.lld_cfg.vq_ewma_alpha = (u32)val;
			break;
		case sf_set_opt_num_hist_bins:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			cfg->num_hist_bins = (u8)val;
			break;
		case sf_set_opt_bin_edge:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			cfg->bin_edges[bin_idx++] = (u32)val;
			break;
		default:
			pr_err("Failed to parse %s token\n", tok);
			break;
		}
	}
	return 0;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
	return -EINVAL;
}

static void sf_set_help(void)
{
	u32 i;

	pr_info("\n");
	pr_info(" Brief: Set SF configuration\n");
	pr_info(" Usage: echo id=<id> buf_sz=<sz> ... > sf_set\n");
	pr_info("   help           - print this help\n");
	pr_info("   id             - Context ID (0-15), mandatory, must be first\n");
	pr_info("   llsf           - LL SF. 1 for LLD\n");
	pr_info("   coupled        - Coupled SF. For ASF\n");
	pr_info("   num_q          - Num queues\n");
	pr_info("   q              - add queue\n");
	pr_info("   q_type         - add queue type");
	for (i = 0; i < PP_QOS_SF_QUEUE_TYPE_NUM; i++)
		pr_cont("  %u:%s", i, pp_qos_sf_q_type_str[i]);
	pr_cont("\n");
	pr_info("   target         - AQM Target Latency\n");
	pr_info("   peak           - AQM Peak rate\n");
	pr_info("   msr            - AQM msr\n");
	pr_info("   factor         - Coupling factor for Classic SF in ASF\n");
	pr_info("   buf_sz         - AQM/LLD Config buffer size\n");
	pr_info("   mode           - AQM mode:");
	for (i = 1; i < PP_QOS_AQM_MODE_NUM; i++)
		pr_cont("  %u:%s", i, pp_qos_aqm_mode_str[i]);
	pr_cont("\n");
	pr_info("   iaqm           - LLD iaqm enable\n");
	pr_info("   qp             - LLD Queue Protection enable\n");
	pr_info("   maxth          - LLD max threshold\n");
	pr_info("   aging          - LLD LG Aging\n");
	pr_info("   range          - LLD LG range\n");
	pr_info("   crit_ql        - LLD Critical QL\n");
	pr_info("   crit_ql_score  - LLD Critical QL Score\n");
	pr_info("   interval       - LLD VQ Interval\n");
	pr_info("   alpha          - LLD VQ Alpha\n");
	pr_info("   num_bins       - Num histogram bins\n");
	pr_info("   bin            - Bin edge\n");
}

static void sf_set_write(char *args_str, void *data)
{
	struct pp_qos_aqm_lld_sf_config cfg = {0};
	s32 ret;
	u8 sf_id;
	bool help = false;

	cfg.coupled_sf = PP_QOS_MAX_SERVICE_FLOWS;

	/* set defaults */
	cfg.coupling_factor = 2;
	cfg.weight = 20;

	if (sf_set_args_parse(args_str, &cfg, &sf_id, &help) || help) {
		sf_set_help();
		return;
	}

	/* set defaults in case this is LL */
	if (cfg.llsf) {
		cfg.cfg.lld_cfg.iaqm_en = 1;
		cfg.cfg.lld_cfg.qp_en = 1;
		cfg.cfg.lld_cfg.lg_aging = 19;
		cfg.cfg.lld_cfg.lg_range = 19;
		cfg.cfg.lld_cfg.vq_interval = 500;
		cfg.cfg.lld_cfg.vq_ewma_alpha = 7;
	}

	ret = pp_misc_sf_set(sf_id, &cfg);
	if (ret)
		pr_err("failed to configure SF, ret %d\n", ret);
}
PP_DEFINE_DEBUGFS(sf_set, NULL, sf_set_write);

enum sf_rem_opts {
	sf_rem_opt_help = 1,
	sf_rem_opt_id,
};

static const match_table_t sf_rem_tokens = {
	{sf_rem_opt_help,     "help"},
	{sf_rem_opt_id,       "id=%u"},
	{ 0 },
};

static s32 sf_rem_args_parse(char *args_str, u8 *sf_id, bool *help)
{
	substring_t substr[MAX_OPT_ARGS];
	enum sf_rem_opts opt;
	char *tok;
	s32 val;

	args_str = strim(args_str);
	/* iterate over user arguments */
	while ((tok = strsep(&args_str, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, sf_rem_tokens, substr);
		switch (opt) {
		case sf_rem_opt_help:
			*help = true;
			return 0;
		case sf_rem_opt_id:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			*sf_id = (u8)val;
			break;
		}
	}
	return 0;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
	return -EINVAL;
}

static void sf_rem_help(void)
{
	pr_info("\n");
	pr_info(" Brief: Remove SF configuration\n");
	pr_info(" Usage: echo id=<id> > sf_rem\n");
	pr_info("   help     - print this help\n");
	pr_info("   id       - Context ID (0-15)\n");
}

void sf_rem_write(char *args_str, void *data)
{
	u8 sf_id;
	bool help;
	s32 ret = 0;

	help = false;

	ret = sf_rem_args_parse(args_str, &sf_id, &help);
	if (unlikely(ret))
		return;

	if (help) {
		sf_rem_help();
		return;
	}

	if (unlikely(ret = pp_misc_sf_remove(sf_id)))
		pr_err("failed to remove SF, ret %d\n", ret);
}
PP_DEFINE_DEBUGFS(sf_rem, NULL, sf_rem_write);

enum sf_show_opts {
	sf_show_opt_help = 1,
	sf_show_opt_id,
};

static const match_table_t sf_show_tokens = {
	{sf_show_opt_help,     "help"},
	{sf_show_opt_id,       "id=%u"},
	{ 0 },
};

static s32 sf_show_args_parse(char *args_str, u8 *sf_id, bool *help)
{
	substring_t substr[MAX_OPT_ARGS];
	enum sf_show_opts opt;
	char *tok;
	s32 val;

	args_str = strim(args_str);
	/* iterate over user arguments */
	while ((tok = strsep(&args_str, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, sf_show_tokens, substr);
		switch (opt) {
		case sf_show_opt_help:
			*help = true;
			return 0;
		case sf_show_opt_id:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			*sf_id = (u8)val;
			break;
		}
	}
	return 0;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
	return -EINVAL;
}

static void sf_show_help(void)
{
	pr_info("\n");
	pr_info(" Brief: Show SF configuration\n");
	pr_info(" Usage: echo id=<id> > sf_show\n");
	pr_info("   help     - print this help\n");
	pr_info("   id       - Context ID (0-15)\n");
	pr_info(" For showing all existing SF use: cat sf_show\n");
}

static void sf_conf_dump(u8 sf_id, struct pp_qos_aqm_lld_sf_config *sf_cfg)
{
	u8 queue_idx;
	u8 fw_ctx;

	pr_info("SF %u configuration [%s]:\n", sf_id, sf_cfg->llsf ? "LLD" : "AQM");
	pr_info("==========================\n");

	pr_info("num queues %u\n", sf_cfg->num_queues);
	for (queue_idx = 0; queue_idx < sf_cfg->num_queues; queue_idx++)
		pr_info("\tQ[%u] %u (%s)\n",
		queue_idx, sf_cfg->queue[queue_idx].id,
		pp_qos_sf_q_type_str[sf_cfg->queue[queue_idx].type]);
	pr_info("buffer_size %u\n", sf_cfg->buffer_size);
	pr_info("coupled SF %u\n", sf_cfg->coupled_sf);
	pr_info("amsr %u\n", sf_cfg->amsr);
	pr_info("msr_l %u\n", sf_cfg->msr_l);
	pr_info("couling factor %u\n", sf_cfg->coupling_factor);
	pr_info("weight %u\n", sf_cfg->weight);
	pr_info("num bins %u\n", sf_cfg->num_hist_bins);
	pr_info("aqm_mode %u, %s\n", sf_cfg->aqm_mode,
		pp_qos_aqm_mode_str[sf_cfg->aqm_mode]);

	if (sf_cfg->llsf) {
		if (pp_misc_fw_lld_ctx_get(sf_id, &fw_ctx))
			return;

		pr_info("fw_lld_ctx %u\n", fw_ctx);
		pr_info("iaqm_en %u\n", sf_cfg->cfg.lld_cfg.iaqm_en);
		pr_info("qp_en %u\n", sf_cfg->cfg.lld_cfg.qp_en);
		pr_info("maxth %u\n", sf_cfg->cfg.lld_cfg.maxth_us);
		pr_info("LG Aging %u\n", sf_cfg->cfg.lld_cfg.lg_aging);
		pr_info("LG Range %u\n", sf_cfg->cfg.lld_cfg.lg_range);
		pr_info("Critical QL %u\n", sf_cfg->cfg.lld_cfg.critical_ql_us);
		pr_info("Critical QL Score %u\n",
			sf_cfg->cfg.lld_cfg.critical_ql_score_us);
		pr_info("VQ interval %u\n", sf_cfg->cfg.lld_cfg.vq_interval);
		pr_info("VQ Alpha %u\n", sf_cfg->cfg.lld_cfg.vq_ewma_alpha);
	} else {
		pr_info("latency_target_ms %u\n",
			sf_cfg->cfg.aqm_cfg.latency_target_ms);
		pr_info("peak_rate %u\n", sf_cfg->cfg.aqm_cfg.peak_rate);
		pr_info("msr %u [%u bits]\n", sf_cfg->cfg.aqm_cfg.msr,
			sf_cfg->cfg.aqm_cfg.msr * 8);
		pr_info("\n");
	}
}

static void sf_show_write(char *args_str, void *data)
{
	struct pp_qos_aqm_lld_sf_config sf_cfg;
	bool help = false;
	u8 sf_id;

	if (sf_show_args_parse(args_str, &sf_id, &help))
		return;

	if (help) {
		sf_show_help();
		return;
	}

	if (pp_misc_sf_conf_get(sf_id, &sf_cfg))
		return;
	
	sf_conf_dump(sf_id ,&sf_cfg);
}

static void sf_show_read(struct seq_file *f)
{
	u8 sf_id;
	struct pp_qos_aqm_lld_sf_config sf_cfg;
	char buf[6]; /* 5 chars for "id=xx" + null terminator */

	/* iterate all SF and call sf_show_write() for each active one */
	for (sf_id=0; sf_id < PP_QOS_MAX_SERVICE_FLOWS; sf_id++) {
		if (pp_misc_sf_conf_get(sf_id, &sf_cfg))
			return;

		if (sf_cfg.num_queues == 0)
			continue;

		snprintf(buf, sizeof(buf), "id=%d", sf_id);
		sf_show_write(buf, NULL);

	}
}
PP_DEFINE_DEBUGFS(sf_show, sf_show_read, sf_show_write);

enum sf_hist_show_opts {
	sf_hist_show_opt_help = 1,
	sf_hist_show_opt_id,
	sf_hist_show_opt_reset,
};

static const match_table_t sf_hist_show_tokens = {
	{sf_hist_show_opt_help,     "help"},
	{sf_hist_show_opt_id,       "id=%u"},
	{sf_hist_show_opt_reset,    "reset=%u"},
	{ 0 },
};

static s32 sf_hist_show_args_parse(char *args_str, u8 *sf_id,
	bool *help, u8 *reset)
{
	substring_t substr[MAX_OPT_ARGS];
	enum sf_hist_show_opts opt;
	char *tok;
	s32 val;

	args_str = strim(args_str);
	/* iterate over user arguments */
	while ((tok = strsep(&args_str, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, sf_hist_show_tokens, substr);
		switch (opt) {
		case sf_hist_show_opt_help:
			*help = true;
			return 0;
		case sf_hist_show_opt_id:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			*sf_id = (u8)val;
			break;
		case sf_hist_show_opt_reset:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			*reset = (u8)val;
			break;
		}
	}
	return 0;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
	return -EINVAL;
}

static void sf_hist_show_help(void)
{
	pr_info("\n");
	pr_info(" Brief: Show SF Histogram\n");
	pr_info(" Usage: echo id=<id> > sf_hist_show\n");
	pr_info("   help     - print this help\n");
	pr_info("   id       - Context ID (0-15)\n");
	pr_info("   reset    - reset histogram after retrieving\n");
}

void sf_hist_show_write(char *args_str, void *data)
{
	u8 sf_id;
	bool help;
	u8 reset = 0;
	s32 ret = 0;
	struct pp_sf_hist_stat hist;
	u16 idx;

	help = false;

	ret = sf_hist_show_args_parse(args_str, &sf_id, &help, &reset);
	if (unlikely(ret))
		return;

	if (help) {
		sf_hist_show_help();
		return;
	}

	ret = pp_misc_sf_hist_get(sf_id, &hist, reset);
	if (unlikely(ret))
		return;

	pr_info("SF %u histogram:\n", sf_id);
	pr_info("================\n");

	pr_info("num bins %u\n", hist.num_hist_bins);
	pr_info("num updates %u\n", hist.hist_updates);
	pr_info("num max latency %u\n", hist.max_latency);
	for (idx = 0; idx < PP_QOS_MAX_HIST_BINS; idx++)
		pr_info("\tBin[%u] %u\n", idx, hist.hist_counter[idx]);
}
PP_DEFINE_DEBUGFS(sf_hist_show, NULL, sf_hist_show_write);

static void allowed_aq_set(char *args, void *data)
{
	unsigned int sf_id, allowed_aq;

	if (unlikely(sscanf(args, "%u %u", &sf_id, &allowed_aq) != 2)) {
		pr_err("sscanf error\n");
		return;
	}

	if (unlikely(pp_lld_allowed_aq_set((u8)sf_id, allowed_aq)))
		pr_err("failed to set allowed_aq\n");
}

static void allowed_aq_show(struct seq_file *f)
{
	pr_info("lld_allowed_aq <SF_ID> <allowed>\n");
}

PP_DEFINE_DEBUGFS(allowed_aq, allowed_aq_show, allowed_aq_set);

static void aqm_engine_set(char *args, void *data)
{
	unsigned int aqm_engine;
	s32 ret = 0;

	if (unlikely(sscanf(args, "%u", &aqm_engine) != 1)) {
		pr_err("sscanf error\n");
		return;
	}

	if (aqm_engine != PP_AQM_SW && aqm_engine != PP_AQM_HW) {
	    pr_err("Invalid aqm engine mode, select %u for %s or %u for %s\n",
			   PP_AQM_SW, "PP_AQM_SW", PP_AQM_HW, "PP_AQM_HW");
	    return;
	}

	ret = pp_misc_set_aqm_engine((u8)aqm_engine);
	if (unlikely(ret)) {
		pr_err("Failed to set aqm engine mode\n");
		return;
	}
	pr_info("warning: changing aqm engine requires reinit MAC\n");
}

static void aqm_engine_set_show(struct seq_file *f)
{

	u8 aqm_engine;
	s32 ret = 0;

	ret = pp_misc_get_aqm_engine(&aqm_engine);
	if (unlikely(ret)) {
		pr_err("Failed to get aqm engine mode\n");
		return;
	}

	seq_printf(f, "aqm engine is %u (%s)\n", aqm_engine,
		   aqm_engine == PP_AQM_SW ? "PP_AQM_SW" : "PP_AQM_HW");
}

PP_DEFINE_DEBUGFS(aqm_engine, aqm_engine_set_show, aqm_engine_set);

#ifdef CONFIG_PPV4_HW_MOD_REGS_LOGS

/**
 * @brief Set PP registers write logs on/off
 */
static s32 mod_reg_log_en_set(void *data, u64 val)
{
	ulong bmap = (ulong)val;

	return pp_regs_mod_log_en_set(&bmap, HW_MOD_CNT);
}

/**
 * @brief Get PP registers write logs status
 */
static s32 mod_reg_log_en_get(void *data, u64 *val)
{
	ulong bmap = (ulong)*val;
	s32 ret;

	ret  = pp_regs_mod_log_en_get(&bmap, HW_MOD_CNT);
	*val = (u64)bmap;
	return ret;
}

PP_DEFINE_DBGFS_ATT_FMT(mod_reg_log_en, mod_reg_log_en_get, mod_reg_log_en_set,
			"%llx\n");

#endif /* CONFIG_PPV4_HW_MOD_REGS_LOGS */

/**
 * @brief files to create in the misc folder
 */
static struct debugfs_file files[] = {
	{"clock",     &PP_DEBUGFS_FOPS(clock)},
	{"desc_show", &PP_DEBUGFS_FOPS(desc)},
	{"free_running_ctr", &PP_DEBUGFS_FOPS(free_running_ctr)},
	{"nf",        &PP_DEBUGFS_FOPS(nf)},
	{"ignore_clk_updates", &PP_DEBUGFS_FOPS(ignore_clk_updates)},
	{"wl_rules_show", &PP_DEBUGFS_FOPS(whitelist_rules)},
	{"wl_rules_test", &PP_DEBUGFS_FOPS(whitelist_rules_test)},
	{"wl_rules_reset", &PP_DEBUGFS_FOPS(whitelist_rules_reset)},
	{"hash_bit", &PP_DEBUGFS_FOPS(hash_bit)},
	{"sf_set", &PP_DEBUGFS_FOPS(sf_set)},
	{"sf_rem", &PP_DEBUGFS_FOPS(sf_rem)},
	{"sf_show", &PP_DEBUGFS_FOPS(sf_show)},
	{"sf_hist_show", &PP_DEBUGFS_FOPS(sf_hist_show)},
	{"lld_allowed_aq", &PP_DEBUGFS_FOPS(allowed_aq)},
	{"aqm_engine", &PP_DEBUGFS_FOPS(aqm_engine)},
#if IS_ENABLED(CONFIG_PPV4_HW_MOD_REGS_LOGS)
	{"module_reg_log_en", &PP_DEBUGFS_FOPS(mod_reg_log_en)},
#endif
};

/**
 * @brief files to create in the root folder
 */
static struct debugfs_file drv_files[] = {
	{"brief",        &PP_DEBUGFS_FOPS(brief)},
	{"version",      &PP_DEBUGFS_FOPS(version)},
	{"driver_stats", &PP_DEBUGFS_FOPS(dstats)},
	{"global",       &PP_DEBUGFS_FOPS(global)},
	{"pps",          &PP_DEBUGFS_FOPS(pps)},
	{"resources",    &PP_DEBUGFS_FOPS(resources)},
};

/**
 * @brief files to create in the hal folder
 */
static struct debugfs_file hal_files[] = {
	{"read",    &PP_DEBUGFS_FOPS(regs_rd)},
	{"write32", &PP_DEBUGFS_FOPS(regs_wr)},
	{"read_buf", &PP_DEBUGFS_FOPS(buf_rd)},
	{"read_dbg",    &PP_DEBUGFS_FOPS(regs_dbg_rd)},
	{"global",          &PP_DEBUGFS_FOPS(hal_global)},
	{"pps",             &PP_DEBUGFS_FOPS(hal_pps)},
	{"resources",       &PP_DEBUGFS_FOPS(resources)},
	{"rxdma_desc_show", &PP_DEBUGFS_FOPS(rxdma_desc)},
};

s32 pp_misc_dbg_init(struct pp_misc_init_param *init_param)
{
	s32 ret;

	if (unlikely(!debugfs_initialized())) {
		pr_err("Debugfs not initialized yet\n");
		return -EPERM;
	}

#if IS_ENABLED(CONFIG_OF)
	ppv4_reg_base = init_param->ppv4_base;
#endif

	ret = pp_debugfs_create(init_param->root_dbgfs, "misc", &dbgfs, files,
				ARRAY_SIZE(files), NULL);
	if (unlikely(ret))
		return ret;

	ret = pp_debugfs_create(init_param->root_dbgfs, NULL, NULL, drv_files,
				ARRAY_SIZE(drv_files), NULL);
	if (unlikely(ret))
		return ret;

#if IS_ENABLED(CONFIG_OF)
	gdb = kzalloc(sizeof(*gdb), GFP_KERNEL);
	if (!gdb) {
		pr_err("Failed to allocate smgr debugfs memory of size %zu\n",
			sizeof(*gdb));
		return -ENOMEM;
	}

	return pp_debugfs_create(init_param->hal_dbgfs, NULL, NULL, hal_files,
				 ARRAY_SIZE(hal_files), gdb);
	if (unlikely(ret))
		return ret;
#endif

	return ret;
}

s32 pp_misc_dbg_clean(void)
{
	debugfs_remove_recursive(dbgfs);

	if (gdb)
		kfree(gdb);

	return 0;
}
