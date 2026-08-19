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
 * Description: PP port manager debugfs interface
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/types.h>
#include <linux/parser.h>
#include "pp_common.h"
#include "pp_debugfs_common.h"
#include "pp_buffer_mgr.h"
#include "pp_port_mgr.h"
#include "pp_port_mgr_internal.h"
#include "rx_dma.h"
#include "uc.h"

/**
 * @brief main port manager debugfs dir
 */
static struct dentry *dbgfs;

/**
 * @brief ports debugfs dir
 */
static struct dentry *ports_dbgfs;

/**
 * @brief ports debugfs dir
 */
static struct dentry *groups_dir;

/**
 * @brief datapaths debugfs dir
 */
static struct dentry *dps_dbgfs;

/**
 * @brief Debugfs stats show API, prints module statistics
 */
void __pmgr_stats_show(struct seq_file *f)
{
	struct pmgr_stats stats;
	u8 dflt_dp;

	if (unlikely(pmgr_stats_get(&stats)))
		return;
	dflt_dp = pmgr_dflt_hif_id_get();

	seq_puts(f, "\n");
	seq_puts(f, "+---------------------------------------------+\n");
	seq_puts(f, "|          Port Manager Statistics            |\n");
	seq_puts(f, "+---------------------------------------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "Default DP", dflt_dp);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "Active Ports",
		   stats.port_count);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "Active Host IF DPs",
		   stats.hif_dps_count);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "NULL Error",
		   stats.null_err);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "HAL Error",
		   stats.hal_err);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "Port Invalid Error",
		   stats.port_invalid_err);
	seq_printf(f, "| %-30s | %-10u |\n", "Port Invalid Headroom",
		   stats.port_invalid_headroom);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "Port Active Error",
		   stats.port_act_err);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "Port Inactive Error",
		   stats.port_inact_err);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "Port Update Error",
		   stats.port_update_no_changes_err);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "Port Classification Error",
		   stats.port_cls_param_err);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "Port Policy Error",
		   stats.port_policy_param_err);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "Port TX cfg Error",
		   stats.port_tx_cfg_err);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "Port RX cfg Error",
		   stats.port_rx_cfg_err);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "Host IF datapath Error",
		   stats.hif_dp_param_err);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "Host IF classification Error",
		   stats.hif_cls_param_err);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "Host IF datapath Port Error",
		   stats.hif_dp_port_err);
	seq_puts(f, "+--------------------------------+------------+\n");
}

PP_DEFINE_DEBUGFS(pmgr_stats, __pmgr_stats_show, NULL);

/**
 * @brief Debugfs stats show API, prints module statistics
 */
void __pmgr_ports_stats_show(struct seq_file *f)
{
	pp_stats_show_seq(f, sizeof(struct pp_stats), PP_MAX_PORT,
			  pmgr_ports_stats_get, pmgr_ports_stats_show, NULL);
}

PP_DEFINE_DEBUGFS(ports_stats, __pmgr_ports_stats_show, NULL);

/**
 * @brief Debugfs stats show API, prints module statistics
 */
void __pmgr_ports_pps_show(struct seq_file *f)
{
	pp_pps_show_seq(f, sizeof(struct pp_stats), PP_MAX_PORT,
			pmgr_ports_stats_get, pmgr_ports_stats_diff,
			pmgr_ports_stats_show, NULL);
}

PP_DEFINE_DEBUGFS(ports_pps, __pmgr_ports_pps_show, NULL);

/**
 * @brief Debugfs port add API, adding new pp port
 */
static void __pmgr_dbg_port_add_set(char *cmd_buf, void *data)
{
	u16 id;
	struct pp_port_cfg cfg;

	if (sscanf(cmd_buf,
		   "%hu %hu %u %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu",
		   &id, &cfg.tx.base_policy, &cfg.rx.parse_type,
		   &cfg.rx.cls.n_flds, &cfg.rx.cls.cp[0].stw_off,
		   &cfg.rx.cls.cp[0].copy_size, &cfg.rx.cls.cp[1].stw_off,
		   &cfg.rx.cls.cp[1].copy_size, &cfg.rx.cls.cp[2].stw_off,
		   &cfg.rx.cls.cp[2].copy_size, &cfg.rx.cls.cp[3].stw_off,
		   &cfg.rx.cls.cp[3].copy_size) != 12) {
		pr_err("sscanf error\n");
		return;
	}

	cfg.rx.flow_ctrl_en = true;
	cfg.rx.mem_port_en = false;
	cfg.rx.policies_map = 0;
	cfg.tx.headroom_size = 64;
	cfg.tx.tailroom_size = 32;
	cfg.tx.max_pkt_size = 4000;
	cfg.tx.min_pkt_len = 0;
	cfg.tx.pkt_only_en = false;
	cfg.tx.policies_map = 0xF;
	cfg.tx.seg_en = false;
	cfg.tx.wr_desc = true;

	if (pp_port_add(id, &cfg)) {
		pr_err("failed to add port %hu\n", id);
		return;
	}

	pr_info("PORT[%hu] ADDED\n", id);
}

static void __pmgr_dbg_port_add_help(struct seq_file *f)
{
	seq_puts(f, " <port-id>\n");
	seq_puts(f, " <base-policy>\n");
	seq_puts(f, " <parsing [0=L2_PARSE, 1=IP_PARSE, 2=IP_PARSE]>\n");
	seq_puts(f, " <cls-number-of-fields ,[up to 4]>\n");
	seq_puts(f, " <cls-field0 stw-off    [bits]\n");
	seq_puts(f, " <cls-field0 szie       [bits]\n");
	seq_puts(f, " <cls-field1 stw-off    [bits]\n");
	seq_puts(f, " <cls-field1 szie       [bits]\n");
	seq_puts(f, " <cls-field2 stw-off    [bits]\n");
	seq_puts(f, " <cls-field2 szie       [bits]\n");
	seq_puts(f, " <cls-field3 stw-off    [bits]\n");
	seq_puts(f, " <cls-field3 szie       [bits]\n");
}

PP_DEFINE_DEBUGFS(port_add, __pmgr_dbg_port_add_help, __pmgr_dbg_port_add_set);

/**
 * @brief Debugfs stats show API, prints module statistics
 */
void __pmgr_dp_stats_show(struct seq_file *f)
{
	struct pp_stats stats;
	u32 dp_idx = (unsigned long)f->private;
	s32 ret;

	seq_puts(f, "\n");
	seq_puts(f, "+----------+--------------+--------------+\n");
	seq_puts(f, "|    DP    |   Packets    |    Bytes     |\n");
	seq_puts(f, "+----------+--------------+--------------+\n");
	memset(&stats, 0, sizeof(stats));
	ret = chk_exception_stats_get(dp_idx, &stats);
	if (ret) {
		pr_err("Error fetching dp %u stats\n", dp_idx);
		return;
	}

	seq_printf(f, "| %-7u  | %-12llu | %-12llu |\n",
		   dp_idx, stats.packets, stats.bytes);
	seq_puts(f, "+----------+--------------+--------------+\n");
}

PP_DEFINE_DEBUGFS(dp_stats, __pmgr_dp_stats_show, NULL);

/**
 * @brief Debugfs stats show API, prints module statistics
 */
void __pmgr_dps_stats_show(struct seq_file *f)
{
	struct pp_stats stats;
	u32 dp_idx;
	s32 ret;

	seq_puts(f, "\n");
	seq_puts(f, "+----------+--------------+--------------+\n");
	seq_puts(f, "|    DP    |   Packets    |    Bytes     |\n");
	seq_puts(f, "+----------+--------------+--------------+\n");

	for (dp_idx = 0; dp_idx < CHK_NUM_EXCEPTION_SESSIONS; dp_idx++) {
		memset(&stats, 0, sizeof(stats));
		ret = chk_exception_stats_get(dp_idx, &stats);
		if (ret) {
			pr_err("Error fetching dp %u stats\n", dp_idx);
			return;
		}

		if (!stats.packets)
			continue;

		seq_printf(f, "| %-7u  | %-12llu | %-12llu |\n",
			   dp_idx, stats.packets, stats.bytes);
	}
	seq_puts(f, "+----------+--------------+--------------+\n");
}

PP_DEFINE_DEBUGFS(dps_stats, __pmgr_dps_stats_show, NULL);

/**
 * @brief Debugfs port show API, prints port configuration
 */
void __pmgr_dbg_port_cfg_show(struct seq_file *f)
{
	struct pmgr_db_port info;
	u32 i, p = (unsigned long)f->private;
	char buf[64];
	u32 rpb_port = U32_MAX;

	if (!pmgr_port_is_active(p)) {
		seq_printf(f, "Port %-3u is not active\n", p);
		return;
	}

	if (pmgr_port_db_info_get(p, &info))
		return;

	if (pmgr_port_rpb_map_get(p, &rpb_port))
		pr_info("ERROR: Can't get RPB mapping for port %u\n", p);

	/* Print Port Configuration */
	seq_puts(f, "\n");
	seq_puts(f, "+---------------------------------------------+\n");
	seq_printf(f, "|          Port %-3u Configuration             |\n", p);
	seq_puts(f, "+---------------------------------------------+\n");
	seq_printf(f, "| %-30s | %-10s |\n", "Protection",
		   BOOL2EN(pmgr_port_is_protected(p)));
	seq_puts(f, "+--------------------------------+------------+\n");

	if (__uc_gpid_group_id_is_valid(info.grp_id)) {
		seq_printf(f, "| %-30s | %-10u |\n", "GPID Group", info.grp_id);
		seq_puts(f, "+--------------------------------+------------+\n");
		seq_printf(f, "| %-30s | %-10u |\n", "Default GPID group priority",
			   info.dflt_priority);
	} else {
		seq_printf(f, "| %-30s | %-10s |\n", "GPID Group", "Invalid");
		seq_puts(f, "+--------------------------------+------------+\n");
		seq_printf(f, "| %-30s | %-10s |\n", "Default GPID group priority",
			   "NA");
	}
	seq_puts(f, "+--------------------------------+------------+\n");

	seq_printf(f, "| %-30s | %-10hhu |\n", "Classification fields",
		   info.cfg.rx.cls.n_flds);
	seq_puts(f, "+--------------------------------+------------+\n");

	for (i = 0; i < info.cfg.rx.cls.n_flds; i++) {
		scnprintf(buf, sizeof(buf), "fld[%u] Cls stw offset", i);
		seq_printf(f, "| %-30s | %-10hhu |\n",
			   buf, info.cfg.rx.cls.cp[i].stw_off);
		seq_puts(f, "+--------------------------------+------------+\n");
		scnprintf(buf, sizeof(buf), "fld[%u] Cls field size", i);
		seq_printf(f, "| %-30s | %-10hhu |\n",
			   buf, info.cfg.rx.cls.cp[i].copy_size);
		seq_puts(f, "+--------------------------------+------------+\n");
	}

	seq_printf(f, "| %-30s | %-10s |\n", "Port type",
		   info.cfg.rx.mem_port_en ? "MEMORY" : "STREAM");
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10s |\n", "Flow control",
		   BOOL2EN(info.cfg.rx.flow_ctrl_en));
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10s |\n", "Parsing Type",
		   (info.cfg.rx.parse_type == L2_PARSE ? "L2" :
		    (info.cfg.rx.parse_type == IP_PARSE ? "IP" : "None")));
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-#10x |\n", "Rx Policies Map",
		   info.cfg.rx.policies_map);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10hu |\n", "Max packet size",
		   info.cfg.tx.max_pkt_size);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10hu |\n", "Min packet size",
		   (u16)info.cfg.tx.min_pkt_len);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10hu |\n", "Headroom size",
		   info.cfg.tx.headroom_size);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10hu |\n", "Tailroom size",
		   info.cfg.tx.tailroom_size);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "TX Base Policy",
		   info.cfg.tx.base_policy);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-#10x |\n", "TX Policies Map",
		   info.cfg.tx.policies_map);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10s |\n", "Packet only mode",
		   BOOL2STR(info.cfg.tx.pkt_only_en));
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10s |\n", "Segmentation",
		   BOOL2STR(info.cfg.tx.seg_en));
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10s |\n", "PreL2",
		   BOOL2STR(info.cfg.tx.prel2_en));
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10s |\n", "Wr Desc",
		   BOOL2STR(info.cfg.tx.wr_desc));
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10u |\n", "RPB port mapping", rpb_port);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %#-10lx |\n", "Mapped TCs", info.tc_mapped[0]);
	seq_puts(f, "+--------------------------------+------------+\n");
}

PP_DEFINE_DEBUGFS(port_cfg, __pmgr_dbg_port_cfg_show, NULL);

static u16 create_port_props(struct pp_dbg_prop *props, u16 size,
			     u32 *id, struct pp_port_cfg *pconf)
{
	u16 num = 0;

	pp_dbg_add_prop(props, &num, size, "id",
			"id. Must exist as the first property!", id,
			sizeof(*id));
	pp_dbg_add_prop(props, &num, size, "max_pkt", "Max packet size",
			&pconf->tx.max_pkt_size,
			sizeof(pconf->tx.max_pkt_size));
	pp_dbg_add_prop(props, &num, size, "hr", "Headroom size",
			&pconf->tx.headroom_size,
			sizeof(pconf->tx.headroom_size));
	pp_dbg_add_prop(props, &num, size, "tr", "Tailroom size",
			&pconf->tx.tailroom_size,
			sizeof(pconf->tx.tailroom_size));
	pp_dbg_add_prop(props, &num, size, "min_pkt",
			"min pkt len (0 - None, 1 - 60B , 2 - 64B, 3 - 128B)",
			&pconf->tx.min_pkt_len,
			sizeof(pconf->tx.min_pkt_len));
	pp_dbg_add_prop(props, &num, size, "base_policy", "Base policy",
			&pconf->tx.base_policy,
			sizeof(pconf->tx.base_policy));
	pp_dbg_add_prop(props, &num, size, "policies_map", "Policies map",
			&pconf->tx.policies_map,
			sizeof(pconf->tx.policies_map));
	pp_dbg_add_prop(props, &num, size, "pkt_only_en", "Pkt only en",
			&pconf->tx.pkt_only_en,
			sizeof(pconf->tx.pkt_only_en));
	pp_dbg_add_prop(props, &num, size, "seg_en", "Segmentation en",
			&pconf->tx.seg_en,
			sizeof(pconf->tx.seg_en));
	pp_dbg_add_prop(props, &num, size, "prel2_en", "Prel2 en",
			&pconf->tx.prel2_en,
			sizeof(pconf->tx.prel2_en));
	pp_dbg_add_prop(props, &num, size, "wr_desc", "Write Descriptor",
			&pconf->tx.wr_desc,
			sizeof(pconf->tx.wr_desc));

	return num;
}

static s32 port_first_prop_cb(char *field, u32 val, void *user_data)
{
	/* Make sure first property is the port id */
	if (strncmp(field, "id", strlen("id"))) {
		pr_err("First prop (%s) must be id\n", field);
		return -EINVAL;
	}

	if (pp_port_get(val, (struct pp_port_cfg *)user_data) != 0) {
		pr_err("pp_port_get failed (id %u)", val);
		return -EINVAL;
	}

	return 0;
}

static s32 port_done_props_cb(u32 val, void *user_data)
{
	if (pp_port_update(val, (struct pp_port_cfg *)user_data) != 0) {
		pr_err("pp_port_update failed (id %u)", val);
		return -EINVAL;
	}

	return 0;
}

void ports_cfg_set(char *cmd_buf, void *data)
{
	struct pp_port_cfg conf;
	struct pp_dbg_props_cbs cbs = {port_first_prop_cb, port_done_props_cb};
	u32 id = UINT_MAX;
	u16 num_props;
	struct pp_dbg_prop *props;

	props = kmalloc_array(DBG_MAX_PROPS, sizeof(struct pp_dbg_prop),
			      GFP_KERNEL | __GFP_ZERO);
	if (!props)
		return;

	num_props = create_port_props(props, DBG_MAX_PROPS, &id, &conf);
	pp_dbg_props_set(cmd_buf, &cbs, props, num_props, &conf);
	kfree(props);
}

void ports_cfg_help(struct seq_file *f)
{
	struct pp_port_cfg conf;
	const char *name = "set port cfg";
	const char *format = "echo id=[port_id] [field]=[value]... > config";
	struct pp_dbg_prop *props;
	u16 num_props;
	u32 id = UINT_MAX;

	props = kmalloc_array(DBG_MAX_PROPS, sizeof(struct pp_dbg_prop),
			      GFP_KERNEL);
	if (!props)
		return;

	num_props = create_port_props(props, DBG_MAX_PROPS, &id, &conf);
	pp_dbg_props_help(f, name, format, props, num_props);
	kfree(props);
}

PP_DEFINE_DEBUGFS(ports_cfg, ports_cfg_help, ports_cfg_set);

/**
 * @brief Debugfs port stat show API, prints port stats
 */
void __pmgr_dbg_port_stats_show(struct seq_file *f)
{
	u32 p = (unsigned long)f->private;
	s32 ret;
	struct pp_stats stats = {0};

	if (!pmgr_port_is_active(p)) {
		seq_printf(f, "Port %-3u is not active\n", p);
		return;
	}
	/* Print Port Statistics */
	seq_puts(f, "\n");

	ret = pp_port_stats_get(p, &stats);
	if (unlikely(ret)) {
		seq_printf(f, "Port %u RX stats not available\n", p);
		return;
	}

	seq_puts(f, "+---------------------------------------------+\n");
	seq_printf(f, "|          Port %-3u RX Statistics             |\n", p);
	seq_puts(f, "+---------------------------------------------+\n");
	seq_printf(f, "| %-30s | %-10llu |\n", "packets", stats.packets);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10llu |\n", "rx bytes", stats.bytes);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10llu |\n", "rx dropped bytes", stats.ing_dropped_bytes);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10llu |\n", "tx dropped bytes", stats.egr_dropped_bytes);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10llu |\n", "rx dropped packets", stats.ing_dropped_packets);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10llu |\n", "tx dropped packets", stats.egr_dropped_packets);
	seq_puts(f, "+--------------------------------+------------+\n");

	seq_puts(f, " For TX port statistics use QoS dbgfs\n\n");
}

PP_DEFINE_DEBUGFS(port_stats, __pmgr_dbg_port_stats_show, NULL);

/**
 * @brief Debugfs port hostif show API, prints port hostif
 */
void __pmgr_dbg_port_hostif_show(struct seq_file *f)
{
	struct pmgr_db_port info;
	u32 p = (unsigned long)f->private;
	u8 tc, hoh, db_dp, hw_dp, prev_dp, dflt_dp;

	if (!pmgr_port_is_active(p)) {
		seq_printf(f, "Port %-3u is not active\n", p);
		return;
	}

	if (pmgr_port_db_info_get(p, &info))
		return;

	dflt_dp = pmgr_dflt_hif_id_get();

	/* Print Port host interface mapping */
	seq_puts(f, "\n");
	seq_puts(f, "+-------------------------------------------------+\n");
	seq_printf(f, "|  Port %-3u Host Interface Map %-11s        |\n", p,
		   pmgr_port_is_protected(p) ? "(Protected)" : "");
	seq_puts(f, "+-------------------------------------------------+\n");
	seq_puts(f, "| TC        | HoH       | DB DP | PREV DP | HW DP |\n");
	seq_puts(f, "+-----------+-----------+-------------------------+\n");

	for (tc = 0; tc < PP_MAX_TC; tc++) {
		PMGR_HIF_FOR_EACH_DP_HOH(hoh) {
			db_dp = pmgr_port_hif_map_get(p, tc, hoh);
			chk_exception_session_map_get(p, tc, hoh, &hw_dp);
			prev_dp = info.prev_map_id[tc][hoh];

			seq_printf(f, "| %-9u | %-9u ", tc, hoh);

			if (db_dp == dflt_dp)
				seq_printf(f, "| %2u*   ", db_dp);
			else
				seq_printf(f, "| %-5u ", db_dp);

			if (prev_dp == dflt_dp)
				seq_printf(f, "| %2u*     ", prev_dp);
			else
				seq_printf(f, "| %-7u ", prev_dp);

			if (hw_dp == dflt_dp)
				seq_printf(f, "| %2u*   ", hw_dp);
			else
				seq_printf(f, "| %-5u ", hw_dp);

			seq_puts(f, "|\n");
		}
		seq_puts(f, "+-----------+-----------+-------------------------+\n");
	}
}

PP_DEFINE_DEBUGFS(port_hostif, __pmgr_dbg_port_hostif_show, NULL);

/**
 * @brief Debugfs datapath show API, prints datapath config
 * @note Used ONLY for debugfs prints
 * @param f seq_file
 * @param unused
 * @return s32 0 on success, non-zero value otherwise
 */
void __pmgr_dbg_dp_cfg_show(struct seq_file *f)
{
	struct pmgr_db_hif dp;
	u32 refc, i, dp_idx = (unsigned long)f->private;
	char buf[64];

	if (unlikely(pmgr_hif_dp_get(dp_idx, &dp)))
		return;

	/* Print Port Configuration */
	seq_puts(f, "\n");
	seq_puts(f, "+---------------------------------------------+\n");
	seq_printf(f, "|    Host Interface DP %-3u Configuration      |\n",
		   dp_idx);
	seq_puts(f, "+---------------------------------------------+\n");
	seq_printf(f, "| %-30s | %-10hu |\n", "Host PP Port", dp.eg.pid);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10hu |\n", "Host QoS Queue", dp.eg.qos_q);
	seq_puts(f, "+--------------------------------+------------+\n");
	seq_printf(f, "| %-30s | %-10s |\n", "Color",
		   PP_COLOR_TO_STR(dp.color));
	seq_puts(f, "+--------------------------------+------------+\n");

	PMGR_HIF_FOR_EACH_DP_SGC(i) {
		if (dp.sgc[i] != PP_SGC_INVALID) {
			scnprintf(buf, sizeof(buf), "SGC Counter%u", i);
			seq_printf(f, "| %-30s | %-10hu |\n", buf, dp.sgc[i]);
			seq_puts(f, "+--------------------------------+------------+\n");
		}
	}

	PMGR_HIF_FOR_EACH_DP_TBM(i) {
		if (dp.tbm[i] != PP_TBM_INVALID) {
			scnprintf(buf, sizeof(buf), "TBM Meter%u", i);
			seq_printf(f, "| %-30s | %-10hu |\n", buf, dp.tbm[i]);
			seq_puts(f, "+--------------------------------+------------+\n");
		}
	}

	if (unlikely(pmgr_hif_dp_refc_get(dp_idx, &refc)))
		seq_puts(f, "  ERROR: Can't get datapath refc\n");
	else
		seq_printf(f, "| %-30s | %-10u |\n", "DP reference count",
			   refc);
	seq_puts(f, "+---------------------------------------------+\n\n");
}

PP_DEFINE_DEBUGFS(dp_cfg, __pmgr_dbg_dp_cfg_show, NULL);

static void gpid_groups_show(struct seq_file *f)
{
	ulong grp_bmap[BITS_TO_LONGS(UC_GPID_GRP_CNT)];
	struct pmgr_db_grp grp;
	struct uc_gpid_group uc_grp;
	struct pp_stats st;
	u32 grp_id, i;
	ulong bmap;
	char buf[32];
	bool pr_hdr;

	pmgr_gpid_groups_bmap_get(grp_bmap);

	seq_puts(f, "\n");
	seq_puts(f, "+---------------------------------------------------------------------------------+\n");
	seq_puts(f, "|      Port Manager GPID Groups                                                   |\n");
	seq_puts(f, "+---------------------------------------------------------------------------------+\n");
	seq_printf(f, "| %-2s | %-16s | %-10s | %-30s | %-9s |\n",
		   "ID", "Name", "Hits", "Features", "Num GPIDs");
	seq_puts(f, "+----+------------------+------------+--------------------------------+-----------+\n");
	for_each_set_bit(grp_id, grp_bmap, UC_GPID_GRP_CNT) {
		if (pmgr_gpid_group_info_get(grp_id, &grp, &uc_grp, &st))
			continue;

		bmap = uc_grp.wl_rule_type_bmap_en;
		i = find_first_bit(&bmap, ING_WL_RULE_TYPE_COUNT);
		pr_hdr = true;
		do {
			if (pr_hdr)
				seq_printf(f, "| %-2d | %-16s | %-10llu ",
					   grp_id, grp.name, st.packets);
			else
				seq_puts(
					f,
					"|    |                  |            ");

			/* rules */
			if (i < ING_WL_RULE_TYPE_COUNT) {
				bmap = uc_grp.wl_rule_index_bmap_en[i];
				if (i == ING_WL_RULE_TYPE_BITHASH)
					snprintf(buf, sizeof(buf), "%s",
						 uc_wl_rule_type_name(i));
				else
					snprintf(buf, sizeof(buf),
						 "%s rules: %*pbl",
						 uc_wl_rule_type_name(i),
						 ING_WL_RULE_MAX_RULES, &bmap);
				seq_printf(f, "| %-30s ", buf);
			} else {
				seq_puts(f,
					 "|                                ");
			}

			/* GPIDs */
			if (pr_hdr)
				seq_printf(f, "| %-9u |\n",
					   bitmap_weight(grp.gpid_bmap,
							 PP_MAX_PORT));
			else
				seq_puts(f, "|           |\n");

			pr_hdr = false;
			bmap = uc_grp.wl_rule_type_bmap_en;
			i = find_next_bit(&bmap, ING_WL_RULE_TYPE_COUNT, i + 1);
		} while (i < ING_WL_RULE_TYPE_COUNT);
		seq_puts(f, "+----+------------------+------------+--------------------------------+-----------+\n");
	}
}

PP_DEFINE_DEBUGFS(gpid_groups, gpid_groups_show, NULL);

static void create_group_help(struct seq_file *f)
{
	seq_puts(f, "\n");
	seq_puts(f, " Brief: Create a new gpid group\n");
	seq_puts(f, " Usage: echo name=[group name] src=<group id> > create\n");
	seq_puts(f, " \thelp - print this help\n");
	seq_puts(f, " \tname - group name, optional, max size is 16\n");
	seq_puts(f, " \tsrc  - group to copy all queues configuration from\n\n");
}

static void delete_group_help(struct seq_file *f)
{
	seq_puts(f, "\n");
	seq_puts(f, " Brief: Delete a gpid group\n");
	seq_puts(f, " Usage: echo id=<group id> > delete\n");
	seq_puts(f, " \thelp - print this help\n");
	seq_puts(f, " \tid   - group id to delete\n\n");
}

enum grp_act_ops {
	grp_act_help = 1,
	grp_act_cr_name,
	grp_act_cr_src_grp,
	grp_act_del_id,
};

static const match_table_t grp_act_tkns = {
	{grp_act_help, "help"},
	{grp_act_cr_name, "name=%s"},
	{grp_act_cr_src_grp, "src=%u"},
	{grp_act_del_id, "id=%u"},
	{ 0 },
};

enum grp_act {
	GRP_DELETE,
	GRP_CREATE,
};

static void group_action_help(struct seq_file *f)
{
	if ((enum grp_act)f->private == GRP_DELETE)
		delete_group_help(f);
	else if ((enum grp_act)f->private == GRP_CREATE)
		create_group_help(f);
	else
		pr_err("action not supported %#lx\n", (ulong)f->private);
}

static void group_action_wr(char *args, void *data)
{
	struct pp_qos_dev *qdev;
	substring_t substr[MAX_OPT_ARGS];
	struct uc_gpid_group uc_grp;
	struct pp_cpu_info cpu[PP_MAX_HOST_CPUS];
	enum grp_act_ops opt;
	enum grp_act action = (enum grp_act)data;
	char *tok, *name = NULL;
	int ret, i, j, grp_id = PMGR_GPID_GRP_INVALID;
	char buf[512];
	struct seq_file f = {
		.buf = buf,
		.size = sizeof(buf),
		.count = 0,
		.private = data,
	};

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (ptr_is_null(qdev))
		return;

	args = strim(args);
	while ((tok = strsep(&args, " \t\n,")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, grp_act_tkns, substr);
		switch (opt) {
		case grp_act_help:
			group_action_help(&f);
			pr_info("%s", f.buf);
			return;
		case grp_act_cr_name:
			if (action == GRP_DELETE)
				goto opt_parse_err;
			match_strlcpy(buf, substr, sizeof(buf));
			name = buf;
			break;
		case grp_act_cr_src_grp:
			if (action == GRP_DELETE)
				goto opt_parse_err;
			if (match_int(substr, &grp_id))
				goto opt_parse_err;
			break;
		case grp_act_del_id:
			if (action == GRP_CREATE)
				goto opt_parse_err;
			if (match_int(substr, &grp_id))
				goto opt_parse_err;
			break;
		default:
			goto opt_parse_err;
		}
	}

	if (grp_id == PMGR_GPID_GRP_INVALID) {
		pr_err("group id must be provided\n");
		return;
	}

	if (action == GRP_DELETE) {
		ret = pp_gpid_group_delete(grp_id);
		if (ret)
			pr_err("fail to delete group%u, ret %d\n", grp_id, ret);
		else
			pr_info("group%u deleted\n", grp_id);
		return;
	}

	/* group create */
	if (pmgr_gpid_group_info_get(grp_id, NULL, &uc_grp, NULL))
		return;

	for (i = 0; i < ARRAY_SIZE(cpu); i++) {
		cpu[i].num_q = ARRAY_SIZE(uc_grp.cpu[i].gpid);
		for (j = 0; j < cpu[i].num_q; j++) {
			cpu[i].queue[j].gpid = uc_grp.cpu[i].gpid[j];
			/* convert physical id to logical */
			cpu[i].queue[j].id =
				pp_qos_queue_id_get(qdev, uc_grp.cpu[i].qid[j]);
		}
	}

	ret = pp_gpid_group_create(name, cpu, ARRAY_SIZE(cpu));
	if (ret < 0) {
		pr_err("failed to create group, ret %d\n", ret);
		return;
	}

	pr_info("group %s created at index %d\n", name, ret);
	return;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
}

PP_DEFINE_DEBUGFS(group_action, group_action_help, group_action_wr);

static void group_queues_show(struct seq_file *f)
{
	struct pp_qos_dev *qdev;
	u32 i, j, log_qid, grp_id = (ulong)f->private;
	struct pmgr_db_grp pmgr_grp;
	struct uc_gpid_group uc_grp;
	char buf[128];

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (ptr_is_null(qdev))
		return;

	if (pmgr_gpid_group_info_get(grp_id, &pmgr_grp, &uc_grp, NULL))
		return;

	seq_puts(f, "\n");
	seq_puts(f, "+-------------");
	for (i = 0; i < ARRAY_SIZE(uc_grp.cpu); i++)
		seq_puts(f, "-----------");
	seq_puts(f, "+\n");

	snprintf(buf, sizeof(buf), "Group%u %s Queues", grp_id, pmgr_grp.name);
	seq_printf(f, "|   %-53s |\n", buf);

	seq_puts(f, "+-------------");
	for (i = 0; i < ARRAY_SIZE(uc_grp.cpu); i++)
		seq_puts(f, "-----------");
	seq_puts(f, "+\n");

	seq_printf(f, "| %-11s ", "Priority");
	for (i = 0; i < ARRAY_SIZE(uc_grp.cpu); i++)
		seq_printf(f, "| CPU%u     ", i);
	seq_puts(f, "|\n");

	seq_puts(f, "|-------------");
	for (i = 0; i < ARRAY_SIZE(uc_grp.cpu); i++)
		seq_puts(f, "+----------");
	seq_puts(f, "|\n");

	for (i = 0; i < UC_WHITELIST_NUM_PRIO; i++) {
		seq_printf(f, "| P%u Qid(phy) ", i);
		for (j = 0; j < ARRAY_SIZE(uc_grp.cpu); j++) {
			log_qid =
				pp_qos_queue_id_get(qdev, uc_grp.cpu[j].qid[i]);
			snprintf(buf, sizeof(buf), "%u(%u)", log_qid,
				 uc_grp.cpu[j].qid[i]);
			seq_printf(f, "| %-8s ", buf);
		}
		seq_puts(f, "|\n");

		seq_printf(f, "| P%u GPID     ", i);
		for (j = 0; j < ARRAY_SIZE(uc_grp.cpu); j++)
			seq_printf(f, "| %-8u ", uc_grp.cpu[j].gpid[i]);
		seq_puts(f, "|\n");

		seq_puts(f, "+-------------");
		for (j = 0; j < ARRAY_SIZE(uc_grp.cpu); j++)
			seq_puts(f, "+----------");
		seq_puts(f, "+\n");
	}
	seq_puts(f, "\n");
}

static void group_queues_help(void *data)
{
	pr_info("\n");
	pr_info(" Brief: Modify gpid group%lu queues configuration\n",
		(ulong)data);
	pr_info(" Usage:\n");
	pr_info(" \techo cid=<host cpu id> prio=<priority> qid=<qid> gpid=<gpid> > queues\n");
	pr_info(" Options:\n");
	pr_info(" \thelp - print this help\n");
	pr_info(" \tcid  - cpu id (mandatory)\n");
	pr_info(" \tprio - priority(mandatory)\n");
	pr_info(" \tqid  - physical queue id\n");
	pr_info(" \tgpid - gpid\n\n");
}

enum grp_queues_ops {
	grp_q_help = 1,
	grp_q_cid,
	grp_q_prio,
	grp_q_qid,
	grp_q_gpid,
};

static const match_table_t grp_queues_tkns = {
	{grp_q_help, "help"},
	{grp_q_cid, "cid=%u"},
	{grp_q_prio, "prio=%u"},
	{grp_q_qid, "qid=%u"},
	{grp_q_gpid, "gpid=%u"},
	{ 0 },
};

static void group_queues_set(char *args, void *data)
{
	substring_t substr[MAX_OPT_ARGS];
	enum grp_queues_ops opt;
	u32 grp_id = (ulong)data;
	char *tok;
	int cid = UC_ING_MAX_HOST_CPU;
	int prio = UC_WHITELIST_NUM_PRIO;
	int gpid = PP_PORT_INVALID;
	int qid = PP_QOS_INVALID_ID;
	int ret;

	args = strim(args);
	while ((tok = strsep(&args, " \t\n,")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, grp_queues_tkns, substr);
		switch (opt) {
		case grp_q_help:
			group_queues_help(data);
			return;
		case grp_q_cid:
			if (match_int(substr, &cid))
				goto opt_parse_err;
			break;
		case grp_q_prio:
			if (match_int(substr, &prio))
				goto opt_parse_err;
			break;
		case grp_q_gpid:
			if (match_int(substr, &gpid))
				goto opt_parse_err;
			break;
		case grp_q_qid:
			if (match_int(substr, &qid))
				goto opt_parse_err;
			break;
		default:
			goto opt_parse_err;
		}
	}

	if (cid == UC_ING_MAX_HOST_CPU || prio == UC_WHITELIST_NUM_PRIO) {
		pr_err("invalid arguments\n");
		return;
	}

	if (gpid == PP_PORT_INVALID)
		goto set_queue;

	ret = uc_ing_gpid_group_gpid_set(grp_id, cid, prio, gpid);
	if (ret < 0) {
		pr_err("fail to set group%u gpid to %u at cpu%u[%u] , ret %d\n",
		       grp_id, gpid, cid, prio, ret);
		return;
	}

set_queue:
	if (qid == PP_QOS_INVALID_ID)
		return;

	ret = uc_ing_gpid_group_queue_set(grp_id, cid, prio, qid);
	if (ret < 0)
		pr_err("fail to set group%u queue to %u at cpu%u[%u] , ret %d\n",
		       grp_id, qid, cid, prio, ret);

	return;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
}

PP_DEFINE_DEBUGFS(queues, group_queues_show, group_queues_set);

static void group_ports_show(struct seq_file *f)
{
	u32 grp_id = (ulong)f->private;
	struct pmgr_db_grp pmgr_grp;
	char buf[128];

	if (pmgr_gpid_group_info_get(grp_id, &pmgr_grp, NULL, NULL))
		return;

	seq_puts(f, "\n");
	seq_puts(f, "+---------------------------------------------+\n");
	snprintf(buf, sizeof(buf), "Group%u %s Ports", grp_id, pmgr_grp.name);
	seq_printf(f, "|    %-40s |\n", buf);
	seq_puts(f, "+---------------------------------------------+\n");
	seq_printf(f, "%*pbl\n", PP_MAX_PORT, pmgr_grp.gpid_bmap);
	seq_puts(f, "+---------------------------------------------+\n");
}

static void group_ports_help(void *data)
{
	pr_info("\n");
	pr_info(" Brief: Map/Unmap ports to gpid group%lu\n", (ulong)data);
	pr_info(" Usage:\n");
	pr_info(" \techo map=<priority>/unmap id=<gpid> ... [id=<gpid] > ports\n");
	pr_info(" Options:");
	pr_info(" \thelp  - print this help\n");
	pr_info(" \tmap   - map port to group%lu with given priority, priority must be [0-%u] when 0 is highest priority\n",
		(ulong)data, PP_GPID_GRP_LOWEST_PRIO);
	pr_info(" \tunmap - unmap port from group%lu\n", (ulong)data);
	pr_info(" \tmove  - move port to the group%lu\n", (ulong)data);
	pr_info(" \tgpid  - gpid (mandatory), can be more then once\n");
}

enum grp_ports_ops {
	grp_p_invalid,
	grp_p_help,
	grp_p_id,
	grp_p_map,
	grp_p_unmap,
	grp_p_move,
	grp_p_op_max
};

static const match_table_t grp_ports_tkns = {
	{grp_p_help, "help"},
	{grp_p_id, "id=%u"},
	{grp_p_map, "map=%u"},
	{grp_p_unmap, "unmap"},
	{grp_p_move, "move"},
	{grp_p_invalid, NULL},
};

static void group_ports_set(char *args, void *data)
{
	substring_t substr[MAX_OPT_ARGS];
	enum grp_ports_ops opt, act = grp_p_invalid;
	u32 priority, grp_id = (ulong)data;
	ulong gpid_bmap[BITS_TO_LONGS(PP_MAX_PORT)] = { 0 };
	char *tok;
	int id, src_grp, ret;

	args = strim(args);
	while ((tok = strsep(&args, " \t\n,")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, grp_ports_tkns, substr);
		switch (opt) {
		case grp_p_help:
			group_ports_help(data);
			return;
		case grp_p_id:
			if (match_int(substr, &id))
				goto opt_parse_err;

			if (!__pp_is_port_valid(id) && id != PP_MAX_PORT)
				return;
			if (id == PP_MAX_PORT)
				bitmap_fill(gpid_bmap, PP_MAX_PORT);
			else
				set_bit((long)id, gpid_bmap);
			break;
		case grp_p_map:
			if (match_int(substr, &priority))
				goto opt_parse_err;
			act = opt;
			break;
		case grp_p_unmap:
		case grp_p_move:
			act = opt;
			break;
		default:
			goto opt_parse_err;
		}
	}

	if (act == grp_p_invalid) {
		pr_err("Invalid arguments\n");
		return;
	}

	for_each_set_bit(id, gpid_bmap, PP_MAX_PORT) {
		switch (act) {
		case grp_p_map:
			ret = pp_gpid_group_add_port(grp_id, id, priority);
			if (ret) {
				pr_err("fail to map gpid%u to group%u, ret %d\n",
				       id, grp_id, ret);
				return;
			}
			break;
		case grp_p_unmap:
			ret = pp_gpid_group_del_port(grp_id, id);
			if (ret) {
				pr_err("fail to unmap gpid%u from group%u, ret %d\n",
				       id, grp_id, ret);
				return;
			}
			break;
		case grp_p_move:
			ret = pp_gpid_group_get_port_map(id, &src_grp);
			if (ret < 0) {
				pr_err("fail to get gpid%u gpid group map, ret %d\n",
				       id, ret);
				return;
			}

			ret = pmgr_gpid_group_move_port(id, grp_id, src_grp);
			if (ret) {
				pr_err("fail to move gpid%u from group%u to group%u, ret %d\n",
				       id, src_grp, grp_id, ret);
				return;
			}
			break;
		default:
			break;
		}
	}
	return;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
}

PP_DEFINE_DEBUGFS(ports, group_ports_show, group_ports_set);

enum rule_opts {
	rule_invalid_opt,
	rule_help,
	rule_add,
	rule_del,
	rule_priority,
	rule_src_mac,
	rule_dst_mac,
	rule_eth_type,
	rule_src_ip,
	rule_dst_ip,
	rule_src_ipv6,
	rule_dst_ipv6,
	rule_ip_proto,
	rule_src_port,
	rule_dst_port,
	rule_icmp_type,
};

static const match_table_t rule_add_tokens = {
	{rule_help, "help"},
	{rule_add, "add"},
	{rule_del, "del"},
	{rule_priority, "priority=%u"},
	{rule_src_mac, "src_mac=%s"},
	{rule_dst_mac, "dst_mac=%s"},
	{rule_eth_type, "eth_type=%u"},
	{rule_src_ip, "src_ip=%s"},
	{rule_dst_ip, "dst_ip=%s"},
	{rule_src_ipv6, "src_ipv6=%s"},
	{rule_dst_ipv6, "dst_ipv6=%s"},
	{rule_ip_proto, "ip_proto=%u"},
	{rule_src_port, "src_port=%u"},
	{rule_dst_port, "dst_port=%u"},
	{rule_icmp_type, "icmp=%u"},
	{rule_invalid_opt, NULL},
};

static void group_rules_help(void *data)
{
	pr_info("\n");
	pr_info(" Brief: add/delete whitelist rule to gpid group%lu\n",
		(ulong)data);
	pr_info(" Usage:\n");
	pr_info(" \techo add/del op1=<value> ... opN=<value> > rules\n");
	pr_info(" Options:\n");
	pr_info("\thelp - print this help\n");
	pr_info("\tadd - add rule to group%lu\n", (ulong)data);
	pr_info("\tdel - delete rule from group%lu\n", (ulong)data);
	pr_info("\tpriority=<value>\n");
	pr_info("\tsrc_mac=XX:XX:XX:XX:XX:XX\n");
	pr_info("\tdst_mac=XX:XX:XX:XX:XX:XX\n");
	pr_info("\teth_type=<value>\n");
	pr_info("\tsrc_ip=X.X.X.X\n");
	pr_info("\tdst_ip=X.X.X.X\n");
	pr_info("\tsrc_ipv6=X:X:X:X:X:X:X:X\n");
	pr_info("\tdst_ipv6=X:X:X:X:X:X:X:X\n");
	pr_info("\tip_proto=<value>\n");
	pr_info("\tsrc_port=<value>\n");
	pr_info("\tdst_port=<value>\n");
	pr_info("\ticmp type code=<value>\n");
	pr_info(" Notes:\n");
	pr_info("\tIPv6 address short form is not supported\n");
	pr_info("\tadd/del and priority and at least 1 field are mandatory\n");
}

static void group_rules_set(char *args, void *data)
{
	struct pp_whitelist_field *field, flds[PP_FLD_COUNT] = { 0 };
	substring_t substr[MAX_OPT_ARGS];
	enum rule_opts opt, act = rule_invalid_opt;
	u32 grp_id = (ulong)data;
	char *tok, tmp[128];
	int ret, val, flds_cnt = 0;
	int prio = UC_WHITELIST_NUM_PRIO;
	u8 *mac, *ip;
	u16 *ipv6;

	args = strim(args);
	while ((tok = strsep(&args, " \t\n,")) != NULL) {
		if (!*tok)
			continue;
		field = &flds[flds_cnt++];
		opt = match_token(tok, rule_add_tokens, substr);
		switch (opt) {
		case rule_help:
			group_rules_help(data);
			return;
		case rule_add:
		case rule_del:
			act = opt;
			flds_cnt--;
			break;
		case rule_priority:
			if (match_int(substr, &prio))
				goto opt_parse_err;
			flds_cnt--;
			break;
		case rule_src_mac:
		case rule_dst_mac:
			if (opt == rule_src_mac) {
				mac = field->src_mac;
				field->type = PP_FLD_SRC_MAC;
			} else {
				mac = field->dst_mac;
				field->type = PP_FLD_DST_MAC;
			}

			match_strlcpy(tmp, substr, sizeof(tmp));
			ret = sscanf(tmp, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
				     &mac[0], &mac[1], &mac[2], &mac[3],
				     &mac[4], &mac[5]);
			if (ret != 6)
				goto opt_parse_err;
			break;
		case rule_eth_type:
			if (match_int(substr, &val))
				goto opt_parse_err;
			field->eth_type = htons((u16)val);
			field->type = PP_FLD_ETH_TYPE;
			break;
		case rule_src_ip:
		case rule_dst_ip:
			if (opt == rule_src_ip) {
				ip = (u8 *)&field->src_ip.v4;
				field->type = PP_FLD_IPV4_SRC_IP;
			} else {
				ip = (u8 *)&field->dst_ip.v4;
				field->type = PP_FLD_IPV4_DST_IP;
			}

			match_strlcpy(tmp, substr, sizeof(tmp));
			ret = sscanf(tmp, "%hhu.%hhu.%hhu.%hhu",
				     &ip[0], &ip[1], &ip[2], &ip[3]);
			if (ret != 4)
				goto opt_parse_err;
			break;
		case rule_src_ipv6:
		case rule_dst_ipv6:
			if (opt == rule_src_ipv6) {
				ipv6 = (u16 *)&field->src_ip.v6;
				field->type = PP_FLD_IPV6_SRC_IP;
			} else {
				ipv6 = (u16 *)&field->dst_ip.v6;
				field->type = PP_FLD_IPV6_DST_IP;
			}

			match_strlcpy(tmp, substr, sizeof(tmp));
			ret = sscanf(tmp, "%hx:%hx:%hx:%hx:%hx:%hx:%hx:%hx",
				     &ipv6[0], &ipv6[1], &ipv6[2], &ipv6[3],
				     &ipv6[4], &ipv6[5], &ipv6[6], &ipv6[7]);
			if (ret != 8)
				goto opt_parse_err;
			for (val = 0;
			     val < sizeof(struct in6_addr) / sizeof(u16); val++)
				ipv6[val] = htons(ipv6[val]);
			break;
		case rule_ip_proto:
			if (match_int(substr, &val))
				goto opt_parse_err;
			field->ip_proto = val;
			field->type = PP_FLD_IP_PROTO;
			break;
		case rule_src_port:
			if (match_int(substr, &val))
				goto opt_parse_err;
			field->src_port = htons((u16)val);
			field->type = PP_FLD_L4_SRC_PORT;
			break;
		case rule_dst_port:
			if (match_int(substr, &val))
				goto opt_parse_err;
			field->dst_port = htons((u16)val);
			field->type = PP_FLD_L4_DST_PORT;
			break;
		case rule_icmp_type:
			if (match_int(substr, &val))
				goto opt_parse_err;
			field->icmp_type_code = htons((u16)val);
			field->type = PP_FLD_ICMP_TYPE_CODE;
			break;
		default:
			goto opt_parse_err;
		}
	}

	if (act == rule_invalid_opt) {
		pr_err("Invalid arguments\n");
		return;
	}

	if (act == rule_add)
		ret = pp_gpid_group_rule_add(grp_id, prio, flds, flds_cnt);
	else
		ret = pp_gpid_group_rule_del(grp_id, prio, flds, flds_cnt);

	if (ret)
		pr_err("fail to %s rule for group%u, ret %d\n",
		       act == rule_add ? "create" : "delete", grp_id, ret);

	return;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
}
static void group_rules_show(struct seq_file *f)
{
	u32 grp_id = (ulong)f->private;
	struct pmgr_db_grp pmgr_grp;
	struct uc_gpid_group uc_grp;
	struct ing_wl_rules rules;
	char buf[128];

	if (pmgr_gpid_group_info_get(grp_id, &pmgr_grp, &uc_grp, NULL))
		return;
	if (uc_ing_whitelist_db_get(&rules))
		return;

	rules.wl_rule_type_bit_enabled = uc_grp.wl_rule_type_bmap_en;
	memcpy(rules.wl_rule_index_enabled, uc_grp.wl_rule_index_bmap_en,
	       sizeof(uc_grp.wl_rule_index_bmap_en));

	seq_puts(f, "\n");
	seq_puts(f, " +---------------------------------------------+\n");
	snprintf(buf, sizeof(buf), "Group%u %s Rules", grp_id, pmgr_grp.name);
	seq_printf(f, " |    %-40s |\n", buf);
	seq_puts(f, " +---------------------------------------------+\n");
	pp_misc_rules_show(f, &rules);
	seq_puts(f, " +---------------------------------------------+\n");
}
PP_DEFINE_DEBUGFS(rules, group_rules_show, group_rules_set);

static struct debugfs_file port_debugfs_files[] = {
	{ "config", &PP_DEBUGFS_FOPS(port_cfg) },
	{ "stats", &PP_DEBUGFS_FOPS(port_stats) },
	{ "hostif", &PP_DEBUGFS_FOPS(port_hostif) },
};

static struct debugfs_file ports_debugfs_files[] = {
	{ "stats", &PP_DEBUGFS_FOPS(ports_stats) },
	{ "pps", &PP_DEBUGFS_FOPS(ports_pps) },
	{ "add", &PP_DEBUGFS_FOPS(port_add) },
	{ "config", &PP_DEBUGFS_FOPS(ports_cfg) },
};

static struct debugfs_file dp_debugfs_files[] = {
	{ "stats", &PP_DEBUGFS_FOPS(dp_stats) },
	{ "config", &PP_DEBUGFS_FOPS(dp_cfg) }
};

static struct debugfs_file dps_debugfs_files[] = {
	{ "stats", &PP_DEBUGFS_FOPS(dps_stats) },
};

static struct debugfs_file debugfs_files[] = {
	{ "stats", &PP_DEBUGFS_FOPS(pmgr_stats) },
};

static struct debugfs_file group_files[] = {
	{ "queues", &PP_DEBUGFS_FOPS(queues), },
	{ "ports", &PP_DEBUGFS_FOPS(ports), },
	{ "rules", &PP_DEBUGFS_FOPS(rules), },
};

static struct debugfs_file groups_files[] = {
	{ "groups", &PP_DEBUGFS_FOPS(gpid_groups) },
	{ "create", &PP_DEBUGFS_FOPS(group_action), (void *)GRP_CREATE },
	{ "delete", &PP_DEBUGFS_FOPS(group_action), (void *)GRP_DELETE },
};

static struct debugfs_file drv_debugfs_files[] = {
	{ "ports_stats", &PP_DEBUGFS_FOPS(ports_stats) },
	{ "ports_pps", &PP_DEBUGFS_FOPS(ports_pps) },
};

s32 pmgr_port_dbg_clean(u16 pid)
{
	struct dentry *dent;
	char dname[32];

	scnprintf(dname, sizeof(dname), "port%hu", pid);
	dent = debugfs_lookup(dname, ports_dbgfs);
	if (!dent) {
		pr_err("failed to find %s\n", dname);
		return -ENOENT;
	}

	debugfs_remove_recursive(dent);
	return 0;
}

s32 pmgr_port_dbg_init(u16 pid)
{
	char buf[16];
	unsigned long id = (unsigned long)pid;

	scnprintf(buf, sizeof(buf), "port%hu", pid);

	return pp_debugfs_create(ports_dbgfs, buf, NULL,
				 port_debugfs_files,
				 ARRAY_SIZE(port_debugfs_files), (void *)id);
}

s32 pmgr_dp_dbg_clean(u8 dp)
{
	struct dentry *dent;
	char dname[32];

	scnprintf(dname, sizeof(dname), "dp%hhu", dp);
	dent = debugfs_lookup(dname, dps_dbgfs);
	if (!dent) {
		pr_err("failed to find %s\n", dname);
		return -ENOENT;
	}

	debugfs_remove_recursive(dent);
	return 0;
}

s32 pmgr_dp_dbg_create(u8 dp)
{
	char buf[16];
	unsigned long id = (unsigned long)dp;

	scnprintf(buf, sizeof(buf), "dp%hhu", dp);

	return pp_debugfs_create(dps_dbgfs, buf, NULL,
				 dp_debugfs_files,
				 ARRAY_SIZE(dp_debugfs_files), (void *)id);
}

s32 pmgr_gpid_grp_dbg_create(u32 grp, const char *name)
{
	char dname[32];
	ulong id = grp;
	u32 n;

	pr_buf(dname, sizeof(dname), n, "group%u", grp);
	if (name && strlen(name))
		pr_buf_cat(dname, sizeof(dname), n, "_%s", name);

	/* remove all spaces and convert to lowercase */
	strreplace(dname, ' ', '_');
	str_tolower(dname);

	return pp_debugfs_create(groups_dir, dname, NULL, group_files,
				 ARRAY_SIZE(group_files), (void *)id);
}

s32 pmgr_gpid_grp_dbg_clean(u32 grp, const char *name)
{
	struct dentry *dent;
	char dname[32];
	u32 n;

	pr_buf(dname, sizeof(dname), n, "group%u", grp);
	if (name && strlen(name))
		pr_buf_cat(dname, sizeof(dname), n, "_%s", name);

	/* remove all spaces and convert to lowercase */
	strreplace(dname, ' ', '_');
	str_tolower(dname);

	dent = debugfs_lookup(dname, groups_dir);
	if (!dent) {
		pr_err("failed to find group%u dir%s\n", grp, name);
		return -ENOENT;
	}

	debugfs_remove_recursive(dent);
	return 0;
}

s32 pmgr_dbg_init(struct dentry *parent)
{
	s32 ret;
	u16 i;

	if (unlikely(!debugfs_initialized())) {
		pr_err("Debugfs not initialized yet\n");
		return -EPERM;
	}

	/* Create ports stats and pps under root directory */
	ret = pp_debugfs_create(parent, NULL, NULL, drv_debugfs_files,
				ARRAY_SIZE(drv_debugfs_files), NULL);
	if (unlikely(ret)) {
		pr_err("Failed to create ports debugfs dir\n");
		return ret;
	}

	/* Port manager debugfs dir */
	ret = pp_debugfs_create(parent, "port_mgr", &dbgfs, debugfs_files,
				ARRAY_SIZE(debugfs_files), NULL);
	if (unlikely(ret)) {
		pr_err("Failed to create the port_mgr debugfs dir\n");
		return ret;
	}

	/* Create main ports dir under port_mgr dir */
	ret = pp_debugfs_create(dbgfs, "ports", &ports_dbgfs,
				ports_debugfs_files,
				ARRAY_SIZE(ports_debugfs_files), NULL);
	if (unlikely(ret)) {
		pr_err("Failed to create ports debugfs dir\n");
		return ret;
	}

	/* Create all ports dir */
	for (i = 0; i < PP_MAX_PORT; i++) {
		ret = pmgr_port_dbg_init(i);
		if (ret) {
			pr_err("Failed to create port %u debugfs dir\n", i);
			return ret;
		}
	}

	/* Create main dps dir under port_mgr dir */
	ret = pp_debugfs_create(dbgfs, "dps", &dps_dbgfs,
				dps_debugfs_files,
				ARRAY_SIZE(dps_debugfs_files), NULL);
	if (unlikely(ret)) {
		pr_err("Failed to create dp's debugfs dir\n");
		return ret;
	}

	/* Create all dps dir */
	for (i = 0; i < PMGR_HIF_DB_DP_MAX; i++) {
		ret = pmgr_dp_dbg_create(i);
		if (ret) {
			pr_err("Failed to create dp %u debugfs dir\n", i);
			return ret;
		}
	}

	/* Create main groups dir under port_mgr dir */
	ret = pp_debugfs_create(dbgfs, "groups", &groups_dir, groups_files,
				ARRAY_SIZE(groups_files), NULL);
	if (unlikely(ret))
		pr_err("Failed to create groups debugfs dir\n");

	return ret;
}

s32 pmgr_dbg_clean(void)
{
	debugfs_remove_recursive(dbgfs);
	dbgfs = NULL;

	return 0;
}
