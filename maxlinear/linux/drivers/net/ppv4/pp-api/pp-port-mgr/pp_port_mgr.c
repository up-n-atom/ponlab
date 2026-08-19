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
 * Description: PP Port Manager Module
 */

#define pr_fmt(fmt) "[PP_PORT_MGR]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/bitops.h>
#include <linux/bug.h>
#include <linux/bitops.h>
#include <linux/pp_api.h>
#include "pp_common.h"
#include "pp_port_mgr.h"
#include "pp_buffer_mgr.h"
#include "port_dist.h"
#include "rx_dma.h"
#include "rpb.h"
#include "parser.h"
#include "checker.h"
#include "pp_desc.h"
#include "pp_port_mgr_internal.h"
#include "uc.h"

/**
 * @define PreL2 is in 16Bytes granularity
 */
#define PP_MIN_PREL2_SZ (16)

/**
 * @define Shortcut macro to check if queue number is a valid
 *         queue number.
 * @param queue queue number
 */
#define PP_IS_QUEUE_VALID(queue) \
	((queue) != PP_QOS_INVALID_ID)

/* Port internal routines */
static s32  __pmgr_port_param_check(u16 pid, struct pp_port_cfg *cfg);
static s32  __pmgr_port_cfg_set(u16 pid, struct pp_port_cfg *cfg);
static bool __pmgr_port_cfg_is_valid_update(u16 pid, struct pp_port_cfg *cfg);
static s32  __pmgr_port_flow_ctr_param_check(u8 flow_ctrl_en);
static s32  __pmgr_port_tx_cfg_set(u16 pid, struct pp_port_tx_info *tx);
static s32  __pmgr_port_rx_cfg_set(u16 pid, struct pp_port_rx_info *rx);
static s32  __pmgr_port_rxdma_param_check(u16 headroom_size, u16 tailroom_size,
					  bool prel2_en, bool seg_en,
					  bool wr_desc);
static s32  __pmgr_port_tx_policies_param_check(u16 base_policy,
						ulong policies_map);
static s32  __pmgr_port_rx_policies_param_check(bool mem_port,
						ulong policies_map);
static void __pmgr_port_db_set(u16 pid, struct pp_port_cfg *cfg);
static void __pmgr_port_db_get(u16 pid, struct pp_port_cfg *cfg);
static void __pmgr_port_db_del(u16 pid);

/* Host interface internal routines */
static s32  __pmgr_hif_cfg_set(struct pp_hif_class *cls,
			       struct pp_hif_datapath *dp,
			       bool check_dflt_map);
static s32  __pmgr_hif_cfg_del(struct pp_hostif_cfg *hif);
static s32  __pmgr_hif_param_check(struct pp_hostif_cfg *cfg);
static s32  __pmgr_hif_cls_param_check(struct pp_hif_class *cls);
static s32  __pmgr_hif_dp_param_check(struct pp_hif_datapath *dp);
static s32 __pmgr_hif_dp_dflt_set(struct pp_hif_datapath *dp, u8 *dflt_idx);
static void __pmgr_hif_dp_clean(u8 *dp_num_arr, u8 arr_sz);
static void __pmgr_hif_dp_eg_cnt_get(struct pp_hif_datapath *dp, u8 *cnt);
static s32  __pmgr_hif_map_set(struct pp_hif_class *cls, u8 *dp_num_arr,
			       u8 arr_sz, bool check_dflt_map);
static s32 __pmgr_hif_dp_set(struct pp_hif_datapath*, u8*, u8);
static bool __pmgr_hif_db_dp_is_equal(struct pp_hif_datapath *dp,
				      u8 eg_idx, u8 dp_num);
static u8 pmgr_hif_db_dp_find(struct pp_hif_datapath *dp, u8 eg_idx);
static s32 __pmgr_packets_drops_get(struct pp_stats *stats);

/* Module internal routines */
static s32 __pmgr_db_init(void);

/**
 * @brief Module database
 */
static struct pmgr_db *db;

/**
 * @brief Get port mapping for specific tc and hash of hashed according
 *        to the port state.
 * @return 0 on success
 */
static s32 pmgr_port_map_get(u16 pid, u8 tc, u8 hoh, u8 *hal_dp)
{
	if (pmgr_port_is_protected(pid)) {
		*hal_dp = db->port[pid].prev_map_id[tc][hoh];
		return 0;
	}

	return chk_exception_session_map_get(pid, tc, hoh, hal_dp);
}

/**
 * @brief set port mapping for specific tc and hash of hashed according
 *        to the port state.
 * @return 0 on success
 */
static s32 pmgr_port_map_set(u16 pid, u8 tc, u8 hoh, u8 hal_dp)
{
	if (pmgr_port_is_protected(pid)) {
		db->port[pid].prev_map_id[tc][hoh] = hal_dp;
		return 0;
	}

	return chk_exception_session_map_set(pid, tc, hoh, hal_dp);
}

s32 pmgr_dflt_hif_id_get(void)
{
	struct pmgr_db *db = pmgr_get_db();
	u8 dflt_id;

	if (ptr_is_null(db))
		return -EPERM;

	spin_lock_bh(&db->lock);
	if (!PMGR_HIF_IS_DP_VALID(db->dflt_dp)) {
		spin_unlock_bh(&db->lock);
		return -EPERM;
	}

	dflt_id = db->dflt_dp;
	spin_unlock_bh(&db->lock);

	return dflt_id;
}

s32 pmgr_ports_stats_get(void *stats, u32 num_stats, void *data)
{
	struct pp_stats *it;
	u32 num = min_t(u32, num_stats, PP_MAX_PORT);
	u16 idx = 0;
	s32 ret;

	if (unlikely(ptr_is_null(stats)))
		return -EINVAL;

	for_each_arr_entry(it, stats, num, idx++) {
		if (!pmgr_port_is_active(idx))
			continue;

		ret = pp_port_stats_get(idx, it);
		if (unlikely(ret)) {
			pr_err("Error getting port %u stats\n", idx);
			return ret;
		}
	}

	return 0;
}

s32 pmgr_ports_stats_diff(void *pre, u32 num_pre, void *post, u32 num_post,
			  void *delta, u32 num_delta, void *data)
{
	struct pp_stats *pre_it, *post_it, *delta_it;
	u32 num = min_t(u32, num_pre, PP_MAX_PORT);
	u16 idx = 0;

	if (unlikely(ptr_is_null(pre) || ptr_is_null(post) ||
		     ptr_is_null(delta)))
		return -EINVAL;

	if (unlikely(num_pre != num_post && num_pre != num_delta))
		return -EINVAL;

	post_it  = post;
	delta_it = delta;
	for_each_arr_entry(pre_it, pre, num, post_it++, delta_it++, idx++) {
		if (!pmgr_port_is_active(idx))
			continue;

		U64_STRUCT_DIFF(pre_it, post_it, delta_it);
	}

	return 0;
}

s32 pmgr_ports_stats_show(char *buf, size_t sz, size_t *n, void *stats,
			  u32 num_stats, void *data)
{
	struct pp_stats *it;
	u32 num = min_t(u32, num_stats, PP_MAX_PORT);

	if (unlikely(ptr_is_null(buf) || ptr_is_null(n) || ptr_is_null(stats)))
		return -EINVAL;

	pr_buf(buf, sz, *n, "\n");
	pr_buf_cat(buf, sz, *n, "+----------+--------------+--------------+----------------+----------------+\n");
	pr_buf_cat(buf, sz, *n, "|   Port   |  Rx Packets  |  Rx Bytes    | ing Drop Bytes | egr Drop Bytes |\n");
	pr_buf_cat(buf, sz, *n, "+----------+--------------+--------------+----------------+----------------+\n");
	pr_debug("stats %p, num_stats %u\n", stats, num_stats);
	for_each_arr_entry_cond(it, stats, num, pp_stats_is_non_zero)
		pr_buf_cat(buf, sz, *n, "| %-7u  | %-12llu | %-12llu |  %-12llu  |  %-12llu  |\n",
			   arr_entry_idx(stats, it), it->packets, it->bytes,
			    it->ing_dropped_bytes, it->egr_dropped_bytes);

	pr_buf_cat(buf, sz, *n, "+----------+--------------+--------------+----------------+----------------+\n");

	return 0;
}

s32 pmgr_port_db_info_get(u16 pid, struct pmgr_db_port *info)
{
	struct pmgr_db *db = pmgr_get_db();

	if (ptr_is_null(db))
		return -EPERM;

	if (ptr_is_null(info) || !__pp_is_port_valid(pid))
		return -EINVAL;

	spin_lock_bh(&db->lock);
	memcpy(info, &db->port[pid], sizeof(*info));
	spin_unlock_bh(&db->lock);

	return 0;
}

s32 pp_port_add(u16 pid, struct pp_port_cfg *cfg)
{
	if (unlikely(!pp_is_ready()))
		return -EPERM;

	return pmgr_port_add(pid, cfg);
}
EXPORT_SYMBOL(pp_port_add);

s32 pp_port_del(u16 pid)
{
	if (unlikely(!pp_is_ready()))
		return -EPERM;

	return pmgr_port_del(pid);
}
EXPORT_SYMBOL(pp_port_del);

s32 pp_port_update(u16 pid, struct pp_port_cfg *cfg)
{
	s32 ret = 0;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	pr_debug("Update PP port %hu\n", pid);

	/* Check param validity */
	ret = __pmgr_port_param_check(pid, cfg);
	if (unlikely(ret))
		goto out;

	spin_lock_bh(&db->lock);
	/* Check if requested port is not active */
	if (unlikely(!__pmgr_port_is_active(pid))) {
		pr_err("PP Port is not active, %hu\n", pid);
		db->stats.port_inact_err++;
		ret = -ENODEV;
		goto unlock_out;
	}

	/* Check if any configuration changes are required */
	if (unlikely(!__pmgr_port_cfg_is_valid_update(pid, cfg))) {
		pr_err("Port update is not allowed\n");
		db->stats.port_update_no_changes_err++;
		ret = -EBADE;
		goto unlock_out;
	}

	/* Update port configuration */
	ret = __pmgr_port_cfg_set(pid, cfg);
	if (unlikely(ret))
		goto unlock_out;

	pr_debug("PP port %hu was updated successfully\n", pid);

unlock_out:
	spin_unlock_bh(&db->lock);
out:
	return ret;
}
EXPORT_SYMBOL(pp_port_update);

s32 pp_port_get(u16 pid, struct pp_port_cfg *cfg)
{
	s32 ret = 0;

	if (!pp_is_ready())
		return -EPERM;

	pr_debug("Get PP port %hu configuration\n", pid);

	/* Check user param */
	if (!cfg) {
		pr_err("Invalid PP Port configuration parameter\n");
		db->stats.null_err++;
		ret = -EINVAL;
		goto out;
	}

	/* Check if port is valid */
	if (!__pp_is_port_valid(pid)) {
		db->stats.port_invalid_err++;
		ret = -ECHRNG;
		goto out;
	}

	spin_lock_bh(&db->lock);
	/* Check if requested port is active */
	if (unlikely(!__pmgr_port_is_active(pid))) {
		/* Port is not active */
		pr_err("PP Port is not active, %hu\n", pid);
		db->stats.port_inact_err++;
		ret = -ENODEV;
		goto unlock_out;
	}

	/* Get the port configuration from db */
	__pmgr_port_db_get(pid, cfg);

unlock_out:
	spin_unlock_bh(&db->lock);
out:
	return ret;
}
EXPORT_SYMBOL(pp_port_get);

s32 pp_port_stats_get(u16 pid, struct pp_stats *stats)
{
	s32 ret = 0;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	pr_debug("Get PP port %hu statistics\n", pid);

	/* Check user param */
	if (unlikely(!stats)) {
		pr_err("Invalid stats parameter\n");
		db->stats.null_err++;
		ret = -EINVAL;
		goto out;
	}

	/* Check if port is valid */
	if (unlikely(!__pp_is_port_valid(pid))) {
		db->stats.port_invalid_err++;
		ret = -ECHRNG;
		goto out;
	}

	spin_lock_bh(&db->lock);
	/* Check if requested port is active */
	if (unlikely(!__pmgr_port_is_active(pid))) {
		pr_err("PP Port is not active, %hu\n", pid);
		db->stats.port_inact_err++;
		ret = -ENODEV;
		goto unlock_out;
	}

	/* Get statistics from HAL */
	ret = port_dist_port_stat_get(pid, stats);
	if (unlikely(ret)) {
		db->stats.hal_err++;
		goto unlock_out;
	}

	ret = rx_dma_egr_bytes_drops_get(pid, &stats->egr_dropped_bytes);
	if (unlikely(ret)) {
		db->stats.hal_err++;
		goto unlock_out;
	}

	ret = rx_dma_ing_bytes_drops_get(pid, &stats->ing_dropped_bytes);
	if (unlikely(ret)) {
		db->stats.hal_err++;
		goto unlock_out;
	}

	ret = __pmgr_packets_drops_get(stats);

unlock_out:
	spin_unlock_bh(&db->lock);
out:
	return ret;
}
EXPORT_SYMBOL(pp_port_stats_get);

s32 pmgr_protected_grp_host_cpu_info_set(struct pp_cpu_info *cpu,
					 unsigned int num_cpus, s32 *group_id)
{
	struct pmgr_db *db = pmgr_get_db();
	s32 id, ret;

	if (ptr_is_null(db) || ptr_is_null(group_id))
		return -EPERM;

	id = pp_gpid_group_create("pp protected", cpu, num_cpus);
	if (id < 0) {
		pr_err("Failed to create PP protected group, ret %d\n",
		       id);
		return id;
	}

	/* enable bithash rule */
	ret = pmgr_gpid_group_hash_bit_set(id, true);
	if (ret) {
		pr_err("Failed to enable bithash to protected group\n");
		return ret;
	}

	db->protected_grp_id = id;
	*group_id = id;

	return ret;
}

/* pp_port_protect
 * Called from DPL platform implementation.
 * All packets received from the protected port will be
 * diverted to the ingress uc
 */
s32 pp_port_protect(u16 port_id)
{
	struct pmgr_db *db = pmgr_get_db();
	struct pmgr_db_port *port = &db->port[port_id];
	struct pmgr_db_grp *protected_grp;
	u8 tc, hoh, map[PP_MAX_TC][CHK_HOH_MAX];
	s32 ret;

	pr_debug("PP Start port %u protection\n", port_id);
	if (ptr_is_null(db))
		return -EINVAL;

	spin_lock_bh(&db->lock);

	if (!__pmgr_port_is_active(port_id)) {
		spin_unlock_bh(&db->lock);
		return -EINVAL;
	}
	if (pmgr_port_is_protected(port_id)) {
		pr_err("port%u is already protected\n", port_id);
		spin_unlock_bh(&db->lock);
		return -EPERM;
	}
	if (!__uc_gpid_group_id_is_valid(db->protected_grp_id)) {
		pr_err("PP protected group wasn't initialized\n");
		spin_unlock_bh(&db->lock);
		return -EPERM;
	}

	protected_grp = &db->grp[db->protected_grp_id];
	/* we only backup port's current mapping for ports which aren't
	 * mapped to a group
	 */
	if (!__uc_gpid_group_id_is_valid(port->grp_id)) {
		ret = chk_exception_session_port_map_get(port_id,
							 port->prev_map_id);
		if (ret) {
			spin_unlock_bh(&db->lock);
			return ret;
		}
	}

	/* prepare new mapping to group's exception */
	PMGR_HIF_FOR_EACH_DP_TC(tc)
		PMGR_HIF_FOR_EACH_DP_HOH(hoh)
			map[tc][hoh] = db->protected_grp_id;

	/* Configure new mapping */
	ret = chk_exception_session_port_map_set(port_id, map);
	if (ret) {
		spin_unlock_bh(&db->lock);
		return ret;
	}

	/* set un-directional mapping in DB from the group to the port */
	set_bit(port_id, protected_grp->gpid_bmap);
	pmgr_set_port_flag(port_id, PORT_FL_PROTECTED);
	spin_unlock_bh(&db->lock);

	return 0;
}
EXPORT_SYMBOL(pp_port_protect);

s32 pp_port_unprotect(u16 port_id)
{
	struct pmgr_db *db = pmgr_get_db();
	struct pmgr_db_port *port = &db->port[port_id];
	struct pmgr_db_grp *protected_grp;
	u8 tc, hoh, map[PP_MAX_TC][CHK_HOH_MAX];
	s32 ret = 0;

	pr_debug("PP Stop port %u protection\n", port_id);
	if (ptr_is_null(db))
		return -EPERM;

	spin_lock_bh(&db->lock);

	if (!__pmgr_port_is_active(port_id)) {
		spin_unlock_bh(&db->lock);
		return -EINVAL;
	}
	if (!pmgr_port_is_protected(port_id)) {
		pr_err("port%u isn't protected, can't un-protect\n", port_id);
		spin_unlock_bh(&db->lock);
		return -EPERM;
	}
	if (!__uc_gpid_group_id_is_valid(db->protected_grp_id)) {
		pr_err("PP protected group wasn't initialized\n");
		spin_unlock_bh(&db->lock);
		return -EPERM;
	}

	/* in case the port was deleted while being protected, we don't
	 * need to revert the mapping cause the port is now
	 * mapped to the default DP
	 */
	if (!pmgr_test_port_flag(port_id, PORT_FL_ACTIVE))
		goto skip_map;

	/* prepare old mapping */
	if (!__uc_gpid_group_id_is_valid(port->grp_id)) {
		/* if the port isn't a group member, we copy the old mapping
		 * from the previous mapping we keep in DB
		 */
		PMGR_HIF_FOR_EACH_DP_TC(tc)
			PMGR_HIF_FOR_EACH_DP_HOH(hoh)
				map[tc][hoh] = port->prev_map_id[tc][hoh];
	} else {
		/* if the port is a group member, we map it back to the group */
		PMGR_HIF_FOR_EACH_DP_TC(tc)
			PMGR_HIF_FOR_EACH_DP_HOH(hoh)
				map[tc][hoh] = port->grp_id;
	}

	/* restore old mapping in HW */
	ret = chk_exception_session_port_map_set(port_id, map);
	if (ret) {
		spin_unlock_bh(&db->lock);
		return ret;
	}

skip_map:
	/* clear the port bit from the protected group and the protected flag */
	protected_grp = &db->grp[db->protected_grp_id];
	clear_bit(port_id, protected_grp->gpid_bmap);
	pmgr_clear_port_flag(port_id, PORT_FL_PROTECTED);

	spin_unlock_bh(&db->lock);

	return ret;
}
EXPORT_SYMBOL(pp_port_unprotect);

s32 pp_hostif_dflt_set(struct pp_hif_datapath *dp)
{
	if (unlikely(!pp_is_ready()))
		return -EPERM;

	return pmgr_hostif_dflt_set(dp);
}
EXPORT_SYMBOL(pp_hostif_dflt_set);

s32 pp_hostif_add(struct pp_hostif_cfg *hif)
{
	if (unlikely(!pp_is_ready()))
		return -EPERM;

	return pmgr_hostif_add(hif);
}
EXPORT_SYMBOL(pp_hostif_add);

s32 pp_hostif_update(struct pp_hostif_cfg *hif, struct pp_hif_datapath *new_dp)
{
	struct pmgr_db *_db = pmgr_get_db();
	s32 ret;

	if (!pp_is_ready())
		return -EPERM;

	/* Check old hostif to make sure it is ok */
	ret = __pmgr_hif_param_check(hif);
	if (ret)
		return ret;

	/* make sure new dp is valid before changing anything in DB */
	ret = __pmgr_hif_dp_param_check(new_dp);
	if (ret)
		return ret;

	/* doing everything under lock to make sure no other process will
	 * get into the way and will use the deleted DPs in case
	 * context switch will occur in between delete and add operations
	 */
	spin_lock_bh(&_db->lock);
	/* first delete current hostif */
	ret = __pmgr_hif_cfg_del(hif);
	if (ret)
		goto out_unlock;

	/* create the new hostif with the new dp */
	ret = __pmgr_hif_cfg_set(&hif->cls, new_dp, true);

out_unlock:
	spin_unlock_bh(&_db->lock);
	return ret;
}
EXPORT_SYMBOL(pp_hostif_update);

s32 pp_hostif_del(struct pp_hostif_cfg *hif)
{
	if (unlikely(!pp_is_ready()))
		return -EPERM;

	return pmgr_hostif_del(hif);
}
EXPORT_SYMBOL(pp_hostif_del);

s32 pp_hostif_dflt_get(struct pp_hif_datapath *dp)
{
	struct pmgr_db_hif *dflt;
	u32 i;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	if (!PMGR_HIF_IS_DP_VALID(db->dflt_dp)) {
		pr_err("Default host interface wasn't set yet\n");
		return -EPERM;
	}

	pr_debug("Get the default PP host interface\n");
	dflt = &db->hif_dp[db->dflt_dp];
	dp->color = dflt->color;
	memcpy(dp->sgc, dflt->sgc, sizeof(dp->sgc));
	memcpy(dp->tbm, dflt->tbm, sizeof(dp->tbm));
	/* db hif only has 1 eg, we copy it and set all others to invalid */
	memcpy(&dp->eg[0], &dflt->eg, sizeof(dp->eg[0]));
	for (i = 1; i < ARRAY_SIZE(dp->eg); i++) {
		dp->eg[i].pid = PP_PORT_INVALID;
		dp->eg[i].qos_q = PP_QOS_INVALID_ID;
	}

	return 0;
}
EXPORT_SYMBOL(pp_hostif_dflt_get);

s32 pmgr_port_add(u16 pid, struct pp_port_cfg *cfg)
{
	s32 ret = 0;

	pr_debug("Add new PP port %hu\n", pid);

	/* Check param validity */
	ret = __pmgr_port_param_check(pid, cfg);
	if (unlikely(ret))
		goto out;

	spin_lock_bh(&db->lock);
	/* Check if requested port is already active */
	if (unlikely(__pmgr_port_is_active(pid))) {
		pr_err("PP Port is already active, %hu\n", pid);
		db->stats.port_act_err++;
		ret = -ENODEV;
		goto unlock_out;
	}

	/* Configure the new port */
	ret = __pmgr_port_cfg_set(pid, cfg);
	if (unlikely(ret))
		goto unlock_out;

	db->stats.port_count++;
	pr_debug("PP port %hu was added successfully\n", pid);

unlock_out:
	spin_unlock_bh(&db->lock);
out:
	return ret;
}

s32 pmgr_port_del(u16 pid)
{
	s32 ret = 0;

	pr_debug("Delete PP port %hu\n", pid);

	/* Check if port is valid */
	if (unlikely(!__pp_is_port_valid(pid))) {
		db->stats.port_invalid_err++;
		ret = -ECHRNG;
		goto out;
	}

	spin_lock_bh(&db->lock);
	/* Check if requested port is active */
	if (unlikely(!__pmgr_port_is_active(pid))) {
		pr_err("Port%u is not active\n", pid);
		db->stats.port_inact_err++;
		ret = -ENODEV;
		goto unlock_out;
	}
	if (__uc_gpid_group_id_is_valid(db->port[pid].grp_id)) {
		pr_err("port%u is mapped to gpid group%u, cannot delete it\n",
		       pid, db->port[pid].grp_id);
		db->stats.port_inact_err++;
		ret = -ENODEV;
		goto unlock_out;
	}

	/* Unmap port from rpb port */
	rpb_pp_port_unmap(db->port[pid].rpb_map);

	/* Delete port from db */
	__pmgr_port_db_del(pid);

	db->stats.port_count--;
	pr_debug("PP port %hu was deleted successfully\n", pid);

unlock_out:
	spin_unlock_bh(&db->lock);
out:
	return ret;
}

s32 pmgr_hostif_dflt_set(struct pp_hif_datapath *dp)
{
	struct pp_hif_class cls = { 0 };
	s32 ret = -EINVAL;
	u8 dflt_idx;

	pr_debug("Set the default PP host interface\n");

	/* Check user param */
	if (unlikely(!dp)) {
		pr_err("Invalid hostif datapath parameter\n");
		db->stats.null_err++;
		goto out;
	}

	/* Check param validity */
	ret = __pmgr_hif_dp_param_check(dp);
	if (unlikely(ret))
		goto out;

	spin_lock_bh(&db->lock);
	/* Set default hostif dp */
	ret = __pmgr_hif_dp_dflt_set(dp, &dflt_idx);
	if (!PMGR_HIF_IS_DP_VALID(ret))
		goto unlock_out;

	/* if default dp was already created, then this call is only for
	 * update it w/o mapping all ports again
	 */
	if (PMGR_HIF_IS_DP_VALID(db->dflt_dp)) {
		/* __pmgr_hif_dp_dflt_set always increment dps count assuming
		 * its always creating a new one, in this case its not true
		 * so fix the counter
		 */
		db->stats.hif_dps_count--;
		goto unlock_out;
	}

	/* map all ports to the default hostif, create a template
	 * and use it for all ports
	 */
	cls.tc_bitmap = (u8)GENMASK(PP_MAX_TC - 1, 0);
	for (cls.port = 0; cls.port < ARRAY_SIZE(db->port); cls.port++) {
		ret = __pmgr_hif_map_set(&cls, &dflt_idx, 1, false);
		if (unlikely(ret))
			goto unlock_out;
	}

	/* succuss, save the default index in the DB */
	db->dflt_dp = dflt_idx;

unlock_out:
	spin_unlock_bh(&db->lock);
out:
	if (!ret)
		pr_debug("Default PP host interface was configured successfully\n");
	return ret;
}

s32 pmgr_hostif_add(struct pp_hostif_cfg *hif)
{
	s32 ret = -EINVAL;

	pr_debug("Add new PP host interface\n");

	/* Check user param */
	if (unlikely(!hif)) {
		pr_err("Invalid hostif configuration parameter\n");
		db->stats.null_err++;
		goto out;
	}

	/* Check param validity */
	ret = __pmgr_hif_param_check(hif);
	if (unlikely(ret))
		goto out;

	spin_lock_bh(&db->lock);
	/* Set new host interface configuration */
	ret = __pmgr_hif_cfg_set(&hif->cls, &hif->dp, true);
	if (unlikely(ret))
		goto unlock_out;

unlock_out:
	spin_unlock_bh(&db->lock);
out:
	if (!ret)
		pr_debug("PP host interface was added successfully\n");
	return ret;
}

s32 pmgr_hostif_del(struct pp_hostif_cfg *hif)
{
	s32 ret = -EINVAL;

	pr_debug("Delete PP host interface\n");

	/* Check user param */
	if (unlikely(!hif)) {
		pr_err("Invalid hostif configuration parameter\n");
		db->stats.null_err++;
		goto out;
	}

	/* Check param validity */
	ret = __pmgr_hif_param_check(hif);
	if (unlikely(ret))
		goto out;

	spin_lock_bh(&db->lock);
	/* Delete host interface configuration */
	ret = __pmgr_hif_cfg_del(hif);
	if (unlikely(ret))
		goto unlock_out;

	pr_debug("PP host interface was deleted successfully\n");

unlock_out:
	spin_unlock_bh(&db->lock);
out:
	return ret;
}

s32 pmgr_stats_get(struct pmgr_stats *stats)
{
	pr_debug("Get port manager module statistics\n");

	if (unlikely(!stats)) {
		pr_err("Invalid module stats param\n");
		return -EINVAL;
	}

	/* Copy module statistics from database */
	memcpy(stats, &db->stats, sizeof(*stats));

	return 0;
}

s32 pmgr_stats_reset(void)
{
	pr_debug("Reset port manager module statistics\n");

	memset(&db->stats, 0, sizeof(db->stats));
	return 0;
}

s32 pmgr_hif_dp_get(u8 dp_idx, struct pmgr_db_hif *dp)
{
	pr_debug("Get host interface datapath cfg\n");

	if (unlikely(!PMGR_HIF_IS_DP_VALID(dp_idx))) {
		pr_err("Invalid hostif index, %hhu\n", dp_idx);
		return -EINVAL;
	}

	if (unlikely(!dp)) {
		pr_err("Invalid hostif datapath param\n");
		db->stats.null_err++;
		return -EINVAL;
	}

	spin_lock_bh(&db->lock);
	memcpy(dp, &db->hif_dp[dp_idx], sizeof(*dp));
	spin_unlock_bh(&db->lock);

	return 0;
}

s32 pmgr_hif_dp_refc_get(u8 dp_idx, u32 *refc)
{
	if (unlikely(!PMGR_HIF_IS_DP_VALID(dp_idx))) {
		pr_err("Invalid hostif index, %hhu\n", dp_idx);
		return -EINVAL;
	}

	if (unlikely(!refc)) {
		pr_err("Invalid hostif datapath refc param\n");
		return -EINVAL;
	}

	spin_lock_bh(&db->lock);
	/* Get from database */
	*refc = db->hif_dp[dp_idx].refc;
	spin_unlock_bh(&db->lock);

	return 0;
}

u8 pmgr_port_hif_map_get(u16 pid, u8 tc, u8 hoh)
{
	u8 hal_dp = PMGR_HIF_DP_INVALID;
	s32 ret;

	pr_debug("Get port host interface mapping\n");

	/* Check port value validity */
	if (unlikely(!__pp_is_port_valid(pid)))
		goto out;

	/* Check TC value validity */
	if (unlikely(!PP_IS_TC_VALID(tc)))
		goto out;

	/* Check Hash Of Hash value validity */
	if (unlikely(hoh >= CHK_HOH_MAX))
		goto out;

	spin_lock_bh(&db->lock);

	/* Get the exception session ID (DP) */
	ret = pmgr_port_map_get(pid, tc, hoh, &hal_dp);
	if (ret) {
		pr_err("Exception map get failed for port %u, tc %u\n",
		       pid, tc);
	}

	spin_unlock_bh(&db->lock);

out:
	return hal_dp;
}

s32 pmgr_port_rpb_map_get(u16 pid, u32 *rpb_port)
{
	pr_debug("Get port RPB mapping\n");

	if (unlikely(!rpb_port)) {
		pr_err("Invalid rpb port param\n");
		return -EINVAL;
	}

	/* Check if port is valid */
	if (unlikely(!__pp_is_port_valid(pid)))
		return -EINVAL;

	spin_lock_bh(&db->lock);
	/* Get from database */
	*rpb_port = db->port[pid].rpb_map;
	spin_unlock_bh(&db->lock);

	return 0;
}

bool __pmgr_port_is_active(u16 pid)
{
	return __pp_is_port_valid(pid) &&
	       pmgr_test_port_flag(pid, PORT_FL_ACTIVE);
}

bool pmgr_port_is_active(u16 pid)
{
	bool active;

	spin_lock_bh(&db->lock);
	active = __pmgr_port_is_active(pid);
	spin_unlock_bh(&db->lock);

	return active;
}

/**
 * @brief Check port configuration data structure validity
 * @param pid port index
 * @param cfg pp port configuration
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_port_param_check(u16 pid, struct pp_port_cfg *cfg)
{
	s32 ret = 0;

	pr_debug("Check port %hu param\n", pid);

	/* Check user param */
	if (!cfg) {
		pr_err("Invalid PP Port configuration parameter\n");
		db->stats.null_err++;
		ret = -EINVAL;
		goto out;
	}

	/* Check port validity */
	if (!__pp_is_port_valid(pid)) {
		db->stats.port_invalid_err++;
		ret = -ECHRNG;
		goto out;
	}

	ret = __pmgr_port_rxdma_param_check(cfg->tx.headroom_size,
					    cfg->tx.tailroom_size,
					    cfg->tx.prel2_en, cfg->tx.seg_en,
					    cfg->tx.wr_desc);
	if (unlikely(ret))
		goto out;

	/* Check port bm policies param */
	ret = __pmgr_port_tx_policies_param_check(cfg->tx.base_policy,
						  (ulong)cfg->tx.policies_map);
	if (unlikely(ret))
		goto out;

	ret = __pmgr_port_rx_policies_param_check(cfg->rx.mem_port_en,
						  (ulong)cfg->rx.policies_map);
	if (unlikely(ret))
		goto out;

	/* Check port flow control param */
	ret = __pmgr_port_flow_ctr_param_check(cfg->rx.flow_ctrl_en);
	if (unlikely(ret))
		goto out;

	pr_debug("Port %hu param is valid\n", pid);

out:
	return ret;
}

/**
 * @brief Set pp port new configuration
 * @note called under lock
 * @param pid port index
 * @param cfg required pp port configuration
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_port_cfg_set(u16 pid, struct pp_port_cfg *cfg)
{
	s32 ret = 0;

	pr_debug("Set port %hu configuration\n", pid);

	/* Set TX configuration */
	ret = __pmgr_port_tx_cfg_set(pid, &cfg->tx);
	if (unlikely(ret))
		goto out;

	/* Set RX configuration */
	ret = __pmgr_port_rx_cfg_set(pid, &cfg->rx);
	if (unlikely(ret))
		goto out;

	/* Set port configuration in db */
	__pmgr_port_db_set(pid, cfg);

out:
	return ret;
}

/**
 * @brief Check if the port configuration update is valid
 * @note called under lock
 * @param pid port index
 * @param cfg new pp port configuration
 * @return bool true if cfg update is valid, false otherwise
 */
static bool __pmgr_port_cfg_is_valid_update(u16 pid, struct pp_port_cfg *cfg)
{
	pr_debug("Check if port %hu update operation is valid\n", pid);

	if ((db->port[pid].cfg.rx.mem_port_en != cfg->rx.mem_port_en) ||
	    (db->port[pid].cfg.rx.flow_ctrl_en != cfg->rx.flow_ctrl_en)) {
		/* If port type was changed, user must delete the old port
		 * so the rpb will be able to map the port correctly
		 */
		return false;
	}
	return true;
}

/**
 * @brief Set the port tx configuration
 * @note called under lock
 * @param pid port index
 * @param tx pp port tx configuration
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_port_tx_cfg_set(u16 pid, struct pp_port_tx_info *tx)
{
	struct rx_dma_port_cfg rxdma_port;
	s32 ret = 0;
	struct fsqm_cfg fsqm_config;

	pr_debug("Set port TX %hu configuration\n", pid);

	/* Check if fsqm is enabled */
	ret = rx_dma_fsqm_config_get(&fsqm_config);
	if (unlikely(ret)) {
		pr_err("NULL argument\n");
		return -EINVAL;
	}

	/* Enable FSQM upon the first port */
	if (!fsqm_config.enable && tx->seg_en) {
		fsqm_config.enable = 1;
		fsqm_config.segmentation_size = RX_DMA_FSQM_DFLT_SEG_SIZE;
		ret = rx_dma_fsqm_config_set(&fsqm_config);
		if (unlikely(ret)) {
			pr_err("NULL argument\n");
			return -EINVAL;
		}
	}

	/* Prepare the rx-dma param */
	rxdma_port.port_id       = pid;
	rxdma_port.headroom_size = tx->headroom_size;
	rxdma_port.tailroom_size = tx->tailroom_size;
	rxdma_port.wr_desc       = tx->wr_desc;
	rxdma_port.pkt_only      = tx->pkt_only_en;
	rxdma_port.min_pkt_size  = tx->min_pkt_len;

	/* Set port TX configuration in RX-DMA */
	ret = rx_dma_port_set(&rxdma_port);
	if (unlikely(ret)) {
		pr_err("PP Port rx-dma configuration failed, port %hu\n", pid);
		db->stats.hal_err++;
		db->stats.port_tx_cfg_err++;
	}

	ret = chk_mtu_set(pid, tx->max_pkt_size);
	if (unlikely(ret)) {
		pr_err("PP Port mtu set failed, port %hu\n", pid);
		db->stats.hal_err++;
		db->stats.port_tx_cfg_err++;
	}

	return ret;
}

static s32 __pmgr_port_rx_rbp_set(u16 pid, struct pp_port_rx_info *rx)
{
	struct port_dist_map pmap;
	s32 ret;

	ret = rpb_pp_port_map(rx->mem_port_en,
			      rx->flow_ctrl_en,
			      &pmap.rpb_port);
	if (unlikely(ret)) {
		pr_err("PP rpb mapping failed, port %hu\n", pid);
		db->stats.hal_err++;
		goto out;
	}

	/* Map the port in port-dist */
	pmap.src_port = pid;
	pmap.tc = PP_MAX_TC; /* Support tc from STW only */
	ret = port_dist_port_map_set(&pmap);
	if (unlikely(ret)) {
		pr_err("PP Port distributor mapping failed, port %hu\n", pid);
		db->stats.hal_err++;
		rpb_pp_port_unmap(pmap.rpb_port);
		goto out;
	}

	/* Save rpb port for port deletion in db */
	db->port[pid].rpb_map = pmap.rpb_port;
	db->port[pid].is_rbp_mapped = true;

out:
	return ret;
}

/**
 * @brief Set the port rx configuration
 * @note called under lock
 * @param pid port index
 * @param rx pp port rx configuration
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_port_rx_cfg_set(u16 pid, struct pp_port_rx_info *rx)
{
	s32 ret = 0;

	pr_debug("Set port RX %hu configuration\n", pid);

	/* Set the RPB port if not mapped */
	if (!db->port[pid].is_rbp_mapped) {
		ret = __pmgr_port_rx_rbp_set(pid, rx);
		if (ret)
			goto out;
	}

	ret = prsr_port_cfg_set(pid, &rx->cls, rx->parse_type);
	if (unlikely(ret)) {
		pr_err("PP parser port cfg failed, port %hu\n", pid);
		db->stats.hal_err++;
	}
out:
	if (unlikely(ret))
		db->stats.port_rx_cfg_err++;

	return ret;
}

static inline bool __pmgr_is_policies_map_valid(ulong policies_map)
{
	ulong max_valid_map = GENMASK(PP_POLICY_PER_PORT - 1, 0);

	if (likely(policies_map <= max_valid_map))
		return true;

	pr_err("invalid policies map %#lx, max allowed map is %#lx\n",
	       policies_map, max_valid_map);
	return false;
}

/**
 * @brief Check port's tx buffer manager policies validity
 * @param base_policy port's base policy
 * @param policies_map bitmap specifying which policies start at
 *                     base_pol are enabled for the port
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_port_tx_policies_param_check(u16 base_policy,
					       ulong policies_map)
{
	u32 i;

	pr_debug("Check port tx policies params, base_pol %u, policies_map %#lx\n",
		 base_policy, policies_map);

	if (unlikely(!policies_map)) {
		pr_err("egress policies map cannot be zero bitmap\n");
		goto error;
	}

	if (unlikely(!__pmgr_is_policies_map_valid(policies_map)))
		goto error;

	/* Check that all policies are valid and active */
	for_each_set_bit(i, &policies_map, PP_POLICY_PER_PORT) {
		if (unlikely(!pp_bmgr_is_policy_active(base_policy))) {
			pr_err("policy %u is either invalid or inactive\n",
			       base_policy);
			goto error;
		}
		base_policy++;
	}

	return 0;

error:
	pr_err("Invalid port tx policies\n");
	db->stats.port_policy_param_err++;
	return -EINVAL;
}

/**
 * @brief Check port's rx buffer manager policies validity
 * @param mem_port specify if the port is streaming or no
 * @param policies_map bitmap specifying which policies enabled for
 *                     the port
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_port_rx_policies_param_check(bool mem_port,
					       ulong policies_map)
{
	pr_debug("Check port rx policies params, mem_port %s, policies_map %#lx\n",
		 BOOL2STR(mem_port), policies_map);

	if (unlikely(!__pmgr_is_policies_map_valid(policies_map)))
		goto error;

	if (unlikely(!mem_port && policies_map)) {
		pr_err("streaming port cannot have rx policies\n");
		goto error;
	}

	if (unlikely(mem_port && !policies_map))
		pr_warn("memory port with no rx policies can derive unnecessary buffer exchange\n");

	return 0;

error:
	pr_err("Invalid port rx policies\n");
	db->stats.port_policy_param_err++;
	return -EINVAL;
}

static s32 __pmgr_port_flow_ctr_param_check(u8 flow_ctrl_en)
{
	if (likely(flow_ctrl_en))
		return 0;

	pr_err("port flow control must be enabled\n");
	return -EINVAL;
}

/**
 * @brief Check port's tx parameters validity
 * @param headroom_size port's headroom size
 * @param tailroom_size port's tailroom size
 * @param prel2_en If set, port support pre l2
 * @param seg_en If set, port support segmentation
 * @param wr_desc If set, port support wr descriptor to buffer
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_port_rxdma_param_check(u16 headroom_size, u16 tailroom_size,
					 bool prel2_en, bool seg_en,
					 bool wr_desc)
{
	s32 ret = 0;
	u32 headroom_min_sz = 0;

	pr_debug("Check port headroom, headroom size %u, prel2 %u, seg_en %u\n",
		 headroom_size, prel2_en, seg_en);

	if (seg_en) {
		if (headroom_size) {
			pr_err("Headroom must be 0 if seg_en is enabled\n");
			ret = -EINVAL;
			goto out;
		}
		if (tailroom_size) {
			pr_err("Tailroom must be 0 if seg_en is enabled\n");
			ret = -EINVAL;
			goto out;
		}
		if (prel2_en) {
			pr_err("PreL2 cannot co-exist with segmentation\n");
			ret = -EINVAL;
			goto out;
		}
		if (wr_desc) {
			pr_err("Wr_desc cannot co-exist with segmentation\n");
			ret = -EINVAL;
			goto out;
		}
	} else {
		/* Verify enough headroom (descriptor + prel2) */
		headroom_min_sz = PP_MAX_DESC_UD_SZ;

		if (prel2_en)
			headroom_min_sz += PP_MIN_PREL2_SZ;

		if (headroom_size < headroom_min_sz) {
			pr_err("Invalid headroom size %u (minimum is %u)\n",
			       headroom_size, headroom_min_sz);
			db->stats.port_invalid_headroom++;
			ret = -EINVAL;
		}
	}

out:
	return ret;
}

/**
 * @brief Set the port configuration in database
 * @note called under lock
 * @param pid port index
 * @param cfg pp port configuration
 */
static void __pmgr_port_db_set(u16 pid, struct pp_port_cfg *cfg)
{
	/* Copy port configuration to databse */
	pr_debug("Set port %hu configuration in db\n", pid);
	memcpy(&db->port[pid].cfg, cfg, sizeof(struct pp_port_cfg));
	pmgr_set_port_flag(pid, PORT_FL_ACTIVE);
}

/**
 * @brief Get the port configuration from database
 * @note called under lock
 * @param pid port index
 * @param cfg pp port configuration
 */
static void __pmgr_port_db_get(u16 pid, struct pp_port_cfg *cfg)
{
	/* Copy port configuration from database */
	memcpy(cfg, &db->port[pid].cfg, sizeof(struct pp_port_cfg));
}

/**
 * @brief Delete port configuration from database
 * @note called under lock
 * @param pid port index
 */
static void __pmgr_port_db_del(u16 pid)
{
	struct pmgr_db_port *port = &db->port[pid];
	bool protected;

	/* Remove port configuration from database, in case the port
	 * is protected we keep only protected flag
	 */
	pr_debug("Delete port %hu configuration from db\n", pid);
	protected = pmgr_port_is_protected(pid);
	memset(port, 0, sizeof(*port));
	port->grp_id = PMGR_GPID_GRP_INVALID;
	if (protected)
		pmgr_set_port_flag(pid, PORT_FL_PROTECTED);
}

/**
 * @brief Set host interface configuration
 * @note called under lock
 * @param hif host interface configuration
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_hif_cfg_set(struct pp_hif_class *cls,
			      struct pp_hif_datapath *dp,
			      bool check_dflt_map)
{
	s32 ret = 0;
	u8  dp_num[CHK_HOH_MAX] = { PMGR_HIF_DP_INVALID, PMGR_HIF_DP_INVALID,
		PMGR_HIF_DP_INVALID, PMGR_HIF_DP_INVALID };

	if (__uc_gpid_group_id_is_valid(db->port[cls->port].grp_id)) {
		pr_err("port%u is mapped to gpid group%u, cannot create hif\n",
		       cls->port, db->port[cls->port].grp_id);
		ret = -EPERM;
		goto error;
	}

	/* Set the hostif datapath */
	ret = __pmgr_hif_dp_set(dp, dp_num, ARRAY_SIZE(dp_num));
	if (unlikely(ret))
		goto error;

	/* Map classification to datapath */
	ret = __pmgr_hif_map_set(cls, dp_num, ARRAY_SIZE(dp_num),
				 check_dflt_map);
	if (unlikely(ret))
		goto error;

	return 0;

error:
	pr_err("Failed to set hostif configuration\n");
	__pmgr_hif_dp_clean(dp_num, ARRAY_SIZE(dp_num));
	return ret;
}

/**
 * @brief Delete host interface configuration
 * @note called under lock
 * @param hif host interface configuration
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_hif_cfg_del(struct pp_hostif_cfg *hif)
{
	struct pmgr_db_port *port = &db->port[hif->cls.port];
	u8 hal_dp = PMGR_HIF_DP_INVALID;
	u8 dp, i, tc, cnt = 0;
	unsigned long bmap = (unsigned long)hif->cls.tc_bitmap;
	u8 dp_num[CHK_HOH_MAX] = { PMGR_HIF_DP_INVALID, PMGR_HIF_DP_INVALID,
		PMGR_HIF_DP_INVALID, PMGR_HIF_DP_INVALID };

	if (__uc_gpid_group_id_is_valid(port->grp_id)) {
		pr_err("port%u is mapped to gpid group%u, cannot delete hif\n",
		       hif->cls.port, port->grp_id);
		goto abort;
	}

	/* Validate requested datapaths are exist in DP DB */
	PMGR_HIF_FOR_EACH_DP_EGRESS(i) {
		/* Check queue number */
		if (!PP_IS_QUEUE_VALID(hif->dp.eg[i].qos_q))
			continue;
		if (!PP_IS_PORT_VALID(hif->dp.eg[i].pid))
			continue;
		/* Find the entry in database */
		dp_num[cnt] = pmgr_hif_db_dp_find(&hif->dp, i);
		if (!PMGR_HIF_IS_DP_VALID(dp_num[cnt])) {
			pr_err("gpid%u: dp%u[gpid%u,q%u] doesn't exist in db\n",
			       i, hif->cls.port, hif->dp.eg[i].pid,
			       hif->dp.eg[i].qos_q);
			goto abort; /* Not found in DB, abort request */
		}

		cnt++;
	}

	/* Validate requested datapaths are exist in HAL mapping */
	for_each_set_bit(tc, &bmap, PP_MAX_TC) {
		PMGR_HIF_FOR_EACH_DP_HOH(i) {
			dp = dp_num[PMGR_HOH_TO_DP_IDX(i, cnt)];
			pmgr_port_map_get(hif->cls.port, tc, i, &hal_dp);
			if (hal_dp != dp) {
				pr_err("gpid%u.tc%u.hoh%u is mapped to dp%u and not dp%u\n",
				       hif->cls.port, tc, i, hal_dp, dp);
				/* Not match to user request, abort */
				goto abort;
			}
		}
	}

	/* Valid request, map back to default */
	for_each_set_bit(tc, &bmap, PP_MAX_TC) {
		PMGR_HIF_FOR_EACH_DP_HOH(i) {
			pmgr_port_map_set(hif->cls.port, tc, i, db->dflt_dp);
			db->hif_dp[db->dflt_dp].refc++;
			dp = dp_num[PMGR_HOH_TO_DP_IDX(i, cnt)];
			db->hif_dp[dp].refc--;
		}
	}

	/* Clean removed datapaths */
	__pmgr_hif_dp_clean(dp_num, ARRAY_SIZE(dp_num));

	return 0;

abort:
	pr_err("Host interface deletion request failed\n");
	return -EPERM;
}

/**
 * @brief Check hostif data structure validity
 * @param cfg pp hostif configuration
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_hif_param_check(struct pp_hostif_cfg *cfg)
{
	s32 ret = 0;

	pr_debug("Check hostif configuration param\n");

	if (ptr_is_null(cfg)) {
		db->stats.null_err++;
		return -EINVAL;
	}

	/* Check host interface classification param */
	ret = __pmgr_hif_cls_param_check(&cfg->cls);
	if (unlikely(ret))
		goto out;

	/* Check host interface datapath param */
	ret = __pmgr_hif_dp_param_check(&cfg->dp);
	if (unlikely(ret))
		goto out;

	pr_debug("Hostif configuration param is valid\n");

out:
	return ret;
}

/**
 * @brief Check host interface classification parameter
 * @param cls host interface classification parameter
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_hif_cls_param_check(struct pp_hif_class *cls)
{
	s32 ret = 0;

	pr_debug("Check hostif classification param\n");

	/* Check port value validity */
	if (unlikely(!__pp_is_port_valid(cls->port))) {
		pr_err("Hostif classification invalid port\n");
		ret = -EINVAL;
		goto out;
	}

	/* Check tc bitmap value validity */
	if (unlikely(!cls->tc_bitmap)) {
		pr_err("Hostif classification empty tc bitmap\n");
		ret = -EINVAL;
		goto out;
	}

	pr_debug("Hostif classification paramr is valid\n");

out:
	if (unlikely(ret))
		db->stats.hif_cls_param_err++;

	return ret;
}

/**
 * @brief Check host interface datapath parameter
 * @param dp host interface datapath parameter
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_hif_dp_param_check(struct pp_hif_datapath *dp)
{
	s32 ret = 0;
	u8 cnt = 0;

	pr_debug("Check hostif datapath param\n");

	/* Check color value validity */
	if (unlikely(!__pp_is_color_valid(dp->color))) {
		pr_err("Hostif datapath invalid color\n");
		ret = -EINVAL;
		goto out;
	}

	/* Get the number of host interface egress */
	__pmgr_hif_dp_eg_cnt_get(dp, &cnt);

	/* Only 1/2/4 are allowed */
	if (unlikely((cnt != PP_HOSTIF_1EG) &&
		     (cnt != PP_HOSTIF_2EG) &&
		     (cnt != PP_HOSTIF_4EG))) {
		pr_err("Invalid num of hostif queues, %hhu\n", cnt);
		ret = -EINVAL;
		goto out;
	}

	pr_debug("Hostif datapath param is valid\n");

out:
	if (unlikely(ret))
		db->stats.hif_dp_param_err++;

	return ret;
}

/**
 * @brief Set host interface mapping
 * @note called under lock
 * @param cls host interface classification parameter
 * @param dp_num_arr datapath number array
 * @param arr_sz dp_num_arr array size
 * @param check_dflt_map if to verify that old mapping was
 *        default dp
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_hif_map_set(struct pp_hif_class *cls, u8 *dp_num_arr,
			      u8 arr_sz, bool check_dflt_map)
{
	struct pmgr_db_port *port = &db->port[cls->port];
	u8 tc, dp, i, cnt = 0;
	unsigned long bmap = (unsigned long)cls->tc_bitmap;
	u8 old_dp[PP_MAX_TC * CHK_HOH_MAX];
	u8 old_dp_cnt = 0;

	pr_debug("Set hostif Mapping\n");

	/* Check how many dps are valid */
	for (i = 0; i < arr_sz; i++) {
		if (!PMGR_HIF_IS_DP_VALID(dp_num_arr[i]))
			break;
		cnt++; /* cnt == number of different datapaths */
	}

	/* Make sure number of datapaths is power of 2 */
	if (unlikely(!is_power_of_2(cnt))) {
		pr_err("Number of datapaths is not power of 2\n");
		return -EPERM;
	}

	/* Check old mapping validity */
	for_each_set_bit(tc, &bmap, PP_MAX_TC) {
		/* if a port's tc wasn't mapped yet, no reason to validate
		 * his mapping
		 */
		if (!test_bit(tc, port->tc_mapped))
			continue;

		PMGR_HIF_FOR_EACH_DP_HOH(i) {
			pmgr_port_map_get(cls->port, tc, i, &dp);
			if (!PMGR_HIF_IS_DP_VALID(dp)) {
				pr_err("Invalid Host interface datapath\n");
				return -EPERM;
			}
			if (check_dflt_map && (dp != db->dflt_dp)) {
				pr_err("Host interface classification exist\n");
				return -EPERM;
			}
			old_dp[old_dp_cnt++] = dp;
		}
	}

	/* Decrement old dp refc */
	for (i = 0; i < old_dp_cnt; i++)
		db->hif_dp[old_dp[i]].refc--;

	/* Set the new mapping */
	for_each_set_bit(tc, &bmap, PP_MAX_TC) {
		PMGR_HIF_FOR_EACH_DP_HOH(i) {
			dp = dp_num_arr[PMGR_HOH_TO_DP_IDX(i, cnt)];
			db->hif_dp[dp].refc++;
			pmgr_port_map_set(cls->port, tc, i, dp);
		}
		set_bit(tc, port->tc_mapped);
	}

	pr_debug("Hostif Mapping was set successfully\n");

	return 0;
}

s32 pmgr_hif_dp_add(struct pp_hif_datapath *dp, u8 dp_num, u8 eg_idx,
		    ulong exp_flags)
{
	struct pp_qos_dev *qdev;
	struct pp_qos_queue_info queue_info;
	struct pmgr_db_hif *dp_db = &db->hif_dp[dp_num];
	struct exception_session_cfg exception_cfg;
	struct pmgr_db_port *port_info;
	s32 ret = 0;
	u16 headroom;

	pr_debug("Add new hostif datapath, dp %hhu\n", dp_num);

	/* Check qos queue value validity and get the physical queue id */
	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(ptr_is_null(qdev)))
		return -EINVAL;

	ret = pp_qos_queue_info_get(qdev, dp->eg[eg_idx].qos_q, &queue_info);
	if (unlikely(ret)) {
		pr_err("Failed getting queue %u info\n", dp->eg[eg_idx].qos_q);
		return ret;
	}

	port_info = &db->port[dp->eg[eg_idx].pid];

	/* Get the port headroom size */
	headroom = port_info->cfg.tx.headroom_size;

	/* For host exception sessions, port headroom must
	 * include the descriptor and tamplate ud size
	 */
	if (unlikely(headroom < sizeof(struct pp_hw_desc))) {
		db->stats.hif_dp_port_err++;
		pr_err("Minimum egress port headroom size for exception session is %u. (Port %u, Headroom %u)\n",
		       (u32)sizeof(struct pp_hw_desc),
		       dp->eg[eg_idx].pid,
		       headroom);
		ret = -EINVAL;
		goto out;
	}

	/* Set new exception session (DP) in HAL */
	exception_cfg.id    = dp_num;
	exception_cfg.port  = dp->eg[eg_idx].pid;
	exception_cfg.queue = queue_info.physical_id;
	exception_cfg.color = dp->color;
	exception_cfg.sgc   = dp->sgc;
	exception_cfg.tbm   = dp->tbm;
	set_bit(SI_CHCK_FLAG_UPDT_SESS_CNT, &exp_flags);
	exception_cfg.flags = exp_flags;

	ret = chk_exception_session_set(&exception_cfg, &port_info->cfg);
	if (unlikely(ret)) {
		pr_err("Exception %d set failed (ret %d)\n", dp_num, ret);
		db->stats.hal_err++;
		goto out;
	}

	/* Update database */
	dp_db->color = dp->color;
	memcpy(&dp_db->eg, &dp->eg[eg_idx], sizeof(dp_db->eg));
	memcpy(dp_db->sgc, dp->sgc, sizeof(dp->sgc));
	memcpy(dp_db->tbm, dp->tbm, sizeof(dp->tbm));

	pr_debug("Added hostif dp %hhu, qos_q=%hu, pp_port=%hu\n",
		 dp_num, dp->eg[eg_idx].qos_q, dp->eg[eg_idx].pid);

	db->stats.hif_dps_count++;
	set_bit(dp_num, db->hif_bmap);

	pr_debug("Hostif datapath %hhu was added successfully\n", dp_num);

out:
	return ret;
}

/**
 * @brief set default host interface datapath
 * @note called under lock
 * @param dp host interface datapath parameter
 * @return s32 return 0 for success
 */
static s32 __pmgr_hif_dp_dflt_set(struct pp_hif_datapath *dp,
				  u8 *dflt_idx)
{
	u8 i, idx;
	s32 ret;

	/* For default DP, egress must be set as first */
	if ((!PP_IS_QUEUE_VALID(dp->eg[PMGR_HIF_DB_EGRESS_IDX].qos_q)) ||
	    (!PP_IS_PORT_VALID(dp->eg[PMGR_HIF_DB_EGRESS_IDX].pid))) {
		/* For default dp, eg[0] must be set */
		pr_err("Invalid default hostif datapath egress, queue %hu, port %hu\n",
		       dp->eg[PMGR_HIF_DB_EGRESS_IDX].qos_q,
		       dp->eg[PMGR_HIF_DB_EGRESS_IDX].pid);
		db->stats.hif_dp_param_err++;
		return -EINVAL;
	}

	/* Reset other queues (only single queue allowed) */
	for (i = PMGR_HIF_DB_EGRESS_IDX + 1; i < PP_HOSTIF_EG_MAX; i++) {
		dp->eg[i].qos_q = PP_QOS_INVALID_ID;
		dp->eg[i].pid = PP_PORT_INVALID;
	}

	/* if default dp already configured, not need to allocate new index */
	if (PMGR_HIF_IS_DP_VALID(db->dflt_dp)) {
		idx = db->dflt_dp;
		goto add_dp;
	}

	idx = find_first_zero_bit(db->hif_bmap, PMGR_HIF_DB_DP_MAX);
	if (!PMGR_HIF_IS_DP_VALID(idx)) {
		/* impossible case as the default is the first to allocate */
		pr_alert("NO FREE DP for DEFAULT!!!!!\n");
		return -ENOENT;
	}

add_dp:
	ret = pmgr_hif_dp_add(dp, idx, PMGR_HIF_DB_EGRESS_IDX, 0);
	if (!ret && dflt_idx)
		*dflt_idx = idx;

	return ret;
}

/**
 * @brief Set host interface datapath
 * @note called under lock
 * @param dp host interface datapath parameter
 * @param dp_num_arr datapath numbers array
 * @param arr_sz datapath numbers array size
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_hif_dp_set(struct pp_hif_datapath *dp,
			     u8 *dp_num_arr, u8 arr_sz)
{
	s32 ret = 0;
	u8 i, cnt = 0;
	u8 new_dp = PMGR_HIF_DP_INVALID;

	PMGR_HIF_FOR_EACH_DP_EGRESS(i) {
		/* Check queue and port */
		if (!PP_IS_QUEUE_VALID(dp->eg[i].qos_q) ||
		    !PP_IS_PORT_VALID(dp->eg[i].pid))
			continue;

		if (cnt >= arr_sz) {
			pr_err("Invalid number of host interface queues\n");
			return -EPERM;
		}

		/* Check is already exist in database */
		dp_num_arr[cnt] = pmgr_hif_db_dp_find(dp, i);
		if (PMGR_HIF_IS_DP_VALID(dp_num_arr[cnt])) {
			cnt++;
			continue;
		}

		/* Not found in DB */
		new_dp = find_first_zero_bit(db->hif_bmap, PMGR_HIF_DB_DP_MAX);
		if (!PMGR_HIF_IS_DP_VALID(new_dp)) {
			pr_err("No free host-if dp entries\n");
			return -EPERM;
		}

		/* Add in new entry */
		ret = pmgr_hif_dp_add(dp, new_dp, i, 0);
		if (ret)
			goto out;

		dp_num_arr[cnt++] = new_dp;
	}

	/* Make sure number of dps is power of 2 */
	if (unlikely(!is_power_of_2(cnt))) {
		pr_err("Number of dps is not power of 2\n");
		return -EPERM;
	}

out:
	return ret;
}

/**
 * @brief Clean deleted host interface datapath
 * @note called under lock
 * @param dp_num_arr datapath numbers array
 * @param arr_sz datapath numbers array size
 */
static void __pmgr_hif_dp_clean(u8 *dp_num_arr, u8 arr_sz)
{
	u8 i;

	for (i = 0; i < arr_sz; i++) {
		/* Check if DP valid */
		if (!PMGR_HIF_IS_DP_VALID(dp_num_arr[i]))
			return;
		/* IF no entries are mapped, remove DP */
		if (db->hif_dp[dp_num_arr[i]].refc == 1) {
			db->hif_dp[dp_num_arr[i]].refc = 0;
			db->stats.hif_dps_count--;
		}
	}
}

/**
 * @brief Get the number of different queues in host interface
 *        datapath
 * @param dp host interface datapath parameter
 * @param cnt number of different queues
 */
static void __pmgr_hif_dp_eg_cnt_get(struct pp_hif_datapath *dp, u8 *cnt)
{
	u8 i;

	*cnt = 0;
	PMGR_HIF_FOR_EACH_DP_EGRESS(i) {
		if ((PP_IS_QUEUE_VALID(dp->eg[i].qos_q)) &&
		    (PP_IS_PORT_VALID(dp->eg[i].pid)) &&
		    __pmgr_port_is_active(dp->eg[i].pid))
			(*cnt)++;
	}
}

/**
 * @brief Check if given datapath and queue index are equal to
 *        datapath from database
 * @note called under lock
 * @param dp host interface datapath parameter
 * @param eg_idx egress index in dp structure
 * @param dp_num datapath index in database
 * @return bool true if equal, false otherwise
 */
static bool __pmgr_hif_db_dp_is_equal(struct pp_hif_datapath *dp,
				      u8 eg_idx, u8 dp_num)
{
	struct pmgr_db_hif *dp_db = &db->hif_dp[dp_num];

	/* Compare given DP to database */
	if (dp_db->color == dp->color &&
	    !memcmp(&dp_db->eg, &dp->eg[eg_idx], sizeof(dp_db->eg)) &&
	    !memcmp(dp_db->sgc, dp->sgc, sizeof(dp->sgc)) &&
	    !memcmp(dp_db->tbm, dp->tbm, sizeof(dp->tbm)))
		return true;

	return false;
}

/**
 * @brief Find host interface datapath in DB, if found
 *        - increment entry reference count and return the entry
 *        id, if not found - return the next free entry id
 * @note called under lock
 * @param dp datapath structure to find
 * @param eg_idx egress index in dp structure
 * @return u8 dp entry id if found, PMGR_HIF_DP_INVALID
 *         otherwise
 */
static u8 pmgr_hif_db_dp_find(struct pp_hif_datapath *dp, u8 eg_idx)
{
	ulong bmap[BITS_TO_LONGS(PMGR_HIF_DB_DP_MAX)] = { 0 };
	u8 i;

	bitmap_copy(bmap, db->hif_bmap, PMGR_HIF_DB_DP_MAX);
	/* clear all reserved dps so we won't search on them */
	bitmap_clear(bmap, 0, UC_GPID_GRP_CNT);
	clear_bit(db->dflt_dp, bmap);

	/* Search in database not including the groups DPs and default DP */
	for_each_set_bit(i, bmap, PMGR_HIF_DB_DP_MAX) {
		/* If exist in database return DP ID */
		if (__pmgr_hif_db_dp_is_equal(dp, eg_idx, i))
			return i;
	}
	return PMGR_HIF_DP_INVALID;
}

/**
 * @brief Get the port packets drops statistics
 * @param stats port statistics
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_packets_drops_get(struct pp_stats *stats)
{
	u32 avg_pkt_size;

	if (!stats)
		return -EINVAL;

	avg_pkt_size = stats->packets ? stats->bytes / stats->packets : 0;

	if (avg_pkt_size) {
		stats->ing_dropped_packets = stats->ing_dropped_bytes / avg_pkt_size;
		stats->egr_dropped_packets = stats->egr_dropped_bytes / avg_pkt_size;
	}

	return 0;
}

/**
 * @brief Initialized module database
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __pmgr_db_init(void)
{
	struct pp_dev_priv *pp_priv;
	struct pmgr_db_hif *dp;
	struct pmgr_db_port *port;

	pp_priv = pp_priv_get();
	if (ptr_is_null(pp_priv))
		return -EPERM;

	/* Allocate module database */
	db = devm_kzalloc(pp_dev_get(), sizeof(*db), GFP_KERNEL);
	if (unlikely(!db)) {
		pr_err("Failed to allocate module DB\n");
		return -ENOMEM;
	}

	/* Initialize all DPs */
	for_each_arr_entry(dp, db->hif_dp, ARRAY_SIZE(db->hif_dp))
		db_hif_init(dp);

	/* Initialize all ports */
	for_each_arr_entry(port, db->port, ARRAY_SIZE(db->port))
		port->grp_id = PMGR_GPID_GRP_INVALID;

	/* reserve all first DPs to the GPIDs groups,
	 * we do that to get 1 to 1 ID mapping so uC can
	 * use the exception ID as is w/o conversion
	 */
	bitmap_set(db->hif_bmap, 0, UC_GPID_GRP_CNT);
	db->dflt_dp = PMGR_HIF_DP_INVALID;
	db->protected_grp_id = PMGR_GPID_GRP_INVALID;

	spin_lock_init(&db->lock);

	pp_priv->pmgr_db = db;

	return 0;
}

s32 pmgr_init(struct pp_pmgr_init_param *init_param)
{
	s32 ret;

	pr_debug("Init PP port manager module\n");

	/* PP_HOSTIF_EG_MAX must be power of 2 */
	BUILD_BUG_ON_NOT_POWER_OF_2(PP_HOSTIF_EG_MAX);
	/* CHKR_HOH_MAX must be aligned with PP_HOSTIF_EG_MAX */
	BUILD_BUG_ON(CHK_HOH_MAX != PP_HOSTIF_EG_MAX);

	/* Init database */
	ret = __pmgr_db_init();
	if (unlikely(ret)) {
		pr_err("Failed to initialized module db\n");
		return ret;
	}

	/* Init sysfs */
	ret = pmgr_sysfs_init(init_param->sysfs);
	if (unlikely(ret)) {
		pr_err("Failed to initialized module debugfs\n");
		return ret;
	}

	/* Init debug */
	ret = pmgr_dbg_init(init_param->dbgfs);
	if (unlikely(ret)) {
		pr_err("Failed to initialized module debugfs\n");
		return ret;
	}

	return 0;
}

void pmgr_exit(void)
{
	struct pp_dev_priv *pp_priv;

	pr_debug("start\n");

	pp_priv = pp_priv_get();
	if (!pp_priv)
		return;

	/* Clean debug */
	pmgr_dbg_clean();
	/* clean sysfs */
	pmgr_sysfs_clean();

	/* Free database memory */
	kfree(db);
	db = NULL;
	pp_priv->pmgr_db = NULL;

	pr_debug("done\n");
}
