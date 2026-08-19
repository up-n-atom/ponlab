/*
 * Copyright (C) 2022-2024 MaxLinear, Inc.
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
 * Description: PP port manager GPID grouping module
 */

#define pr_fmt(fmt) "[PP_GRP]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/bitops.h>
#include <linux/bitops.h>
#include <linux/pp_api.h>
#include "pp_common.h"
#include "pp_port_mgr.h"
#include "checker.h"
#include "pp_port_mgr_internal.h"
#include "uc.h"

static s32 __pp_rule_ip_mac(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index);
static s32 __pp_rule_mac(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index);
static s32 __pp_rule_eth_type(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index);
static s32 __pp_rule_ip_proto_ports(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index);
static s32 __pp_rule_icmp(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index);
static s32 __pp_rule_ipv4_ports(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index);
static s32 __pp_rule_two_ipv4_ports(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index);
static s32 __pp_rule_ipv6_ports(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index);
typedef s32 (*pp_uc_rule_func)(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index);

pp_uc_rule_func pp_to_uc_rule[ING_WL_RULE_TYPE_COUNT] = {
	NULL, __pp_rule_ip_mac, __pp_rule_mac, __pp_rule_eth_type,
	__pp_rule_ip_proto_ports, __pp_rule_icmp, __pp_rule_ipv4_ports,
	__pp_rule_two_ipv4_ports, __pp_rule_ipv6_ports};

static inline bool group_is_active(struct pmgr_db *db, u32 id)
{
	if (uc_gpid_group_id_is_valid(id) && test_bit(id, db->grp_bmap))
		return true;

	pr_err("GPID Group %u is inactive\n", id);
	return false;
}

static inline bool group_params_check(const char *name, struct pp_cpu_info *cpu,
				      unsigned int num_cpus)
{
	int i = 0;

	if (ptr_is_null(cpu))
		return false;

	if (num_cpus < 1 || PP_MAX_HOST_CPUS < num_cpus) {
		pr_err("CPUs number should be in range 1-%u\n",
		       PP_MAX_HOST_CPUS);
		return false;
	}

	for (i = 0; i < num_cpus; i++) {
		if (!cpu[i].num_q) {
			pr_err("CPU[%u] MUST have at least 1 queue\n", i);
			return false;
		}
	}
	return true;
}

static s32 gpid_group_create(struct pmgr_db *db, const char *name)
{
	struct pp_hif_datapath dp;
	struct pmgr_db_hif *dflt_dp;
	struct pmgr_db_grp *grp;
	u32 grp_id;
	s32 ret;

	pp_hif_init(&dp);

	/* allocate group id */
	grp_id = find_first_zero_bit(db->grp_bmap, UC_GPID_GRP_CNT);
	if (grp_id >= UC_GPID_GRP_CNT) {
		pr_err("No free group exist\n");
		return -ENOSPC;
	}
	set_bit(grp_id, db->grp_bmap);
	grp = &db->grp[grp_id];

	/* create group DP */
	dflt_dp = &db->hif_dp[db->dflt_dp];
	dp.color = PP_COLOR_GREEN;
	dp.eg[0].pid = dflt_dp->eg.pid;
	dp.eg[0].qos_q = dflt_dp->eg.qos_q;
	ret = pmgr_hif_dp_add(&dp, grp_id, 0, BIT(SI_CHCK_FLAG_SEND2FW));
	if (ret) {
		pr_err("Failed to create group %u dp, ret %d\n", grp_id, ret);
		return ret;
	}

	if (name && strnlen(name, sizeof(grp->name)))
		strncpy(grp->name, name, sizeof(grp->name) - 1);

	return grp_id;
}

static s32 gpid_grp_to_uc(struct pp_cpu_info *cpu, unsigned int num_cpus,
			  struct ing_host_cpu_info uc_cpu[PP_MAX_HOST_CPUS])
{
	struct pp_qos_queue_info q_info;
	struct pp_qos_dev *qdev;
	s32 i, j, ret;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(ptr_is_null(qdev)))
		return -EINVAL;

	for (i = 0; i < num_cpus; i++) {
		for (j = 0; j < cpu[i].num_q; j++) {
			/* get queue's physical id */
			ret = pp_qos_queue_info_get(qdev, cpu[i].queue[j].id,
						    &q_info);
			if (ret) {
				pr_err("failed to find phy id of queue %u, ret %d\n",
				       cpu[i].queue[j].id, ret);
				return ret;
			}
			uc_cpu[i].qid[j] = q_info.physical_id;
			uc_cpu[i].gpid[j] = cpu[i].queue[j].gpid;
		}
		/* in order not have extra cycles in the FW, we configured
		 * all the queues where the lowest priority queue will be
		 */
		if (j > 0) {
			while (j < ARRAY_SIZE(uc_cpu[i].qid)) {
				uc_cpu[i].qid[j] = uc_cpu[i].qid[j - 1];
				uc_cpu[i].gpid[j] = uc_cpu[i].gpid[j - 1];
				j++;
			}
		}
	}

	if (num_cpus == PP_MAX_HOST_CPUS)
		return 0;

	/* to simplify uC work and reduce number of cycles, we configure
	 * all CPUs entries
	 */
	i = 0;
	for (j = num_cpus; j < PP_MAX_HOST_CPUS; j++) {
		memcpy(&uc_cpu[j], &uc_cpu[i], sizeof(uc_cpu[j]));
		if (++i == num_cpus)
			i = 0;
	}

	return 0;
}

s32 pp_gpid_group_create(const char *name, struct pp_cpu_info *cpu,
			 unsigned int num_cpus)
{
	struct ing_host_cpu_info uc_cpu[PP_MAX_HOST_CPUS];
	struct pmgr_db *db = pmgr_get_db();
	s32 ret;
	u32 grp_id;

	if (ptr_is_null(db))
		return -EPERM;

	if (!group_params_check(name, cpu, num_cpus))
		return -EINVAL;

	spin_lock_bh(&db->lock);

	/* create the group in DB */
	ret = gpid_group_create(db, name);
	if (ret < 0)
		goto unlock;
	grp_id = ret;

	/* create the uC group info */
	ret = gpid_grp_to_uc(cpu, num_cpus, uc_cpu);
	if (ret)
		goto unlock;

	/* create group in uC */
	ret = uc_ing_gpid_group_create(grp_id, uc_cpu);

unlock:
	spin_unlock_bh(&db->lock);

	if (!ret) {
		pmgr_gpid_grp_dbg_create(grp_id, name);
		ret = grp_id;
	}
	return ret;
}
EXPORT_SYMBOL(pp_gpid_group_create);

s32 pp_gpid_group_delete(u32 id)
{
	struct pmgr_db *db = pmgr_get_db();
	struct pmgr_db_grp *grp = &db->grp[id];
	s32 ret = 0;

	if (ptr_is_null(db))
		return -EPERM;

	if (!uc_gpid_group_id_is_valid(id))
		return -EINVAL;

	/* delete debugfs */
	pmgr_gpid_grp_dbg_clean(id, grp->name);

	spin_lock_bh(&db->lock);

	if (!group_is_active(db, id)) {
		spin_unlock_bh(&db->lock);
		return -EINVAL;
	}

	/* if ports are still mapped, exit with error */
	if (!bitmap_empty(grp->gpid_bmap, PP_MAX_PORT)) {
		pr_err("can't delete group%u, %u ports still mapped\n", id,
		       bitmap_weight(grp->gpid_bmap, PP_MAX_PORT));
		spin_unlock_bh(&db->lock);
		return -EPERM;
	}

	/* remove group from DB */
	memset(grp, 0, sizeof(*grp));
	clear_bit(id, db->grp_bmap);

	/* remove group from uC */
	spin_unlock_bh(&db->lock);

	if (!ret)
		pr_debug("group%u deleted from DB\n", id);
	return ret;
}
EXPORT_SYMBOL(pp_gpid_group_delete);

s32 pmgr_gpid_groups_bmap_get(ulong *grp_bmap)
{
	struct pmgr_db *db = pmgr_get_db();

	if (ptr_is_null(db))
		return -EPERM;

	if (ptr_is_null(grp_bmap))
		return -EINVAL;

	spin_lock_bh(&db->lock);
	memcpy(grp_bmap, db->grp_bmap, sizeof(db->grp_bmap));
	spin_unlock_bh(&db->lock);

	return 0;
}

s32 pmgr_gpid_group_info_get(u32 grp_id, struct pmgr_db_grp *pmgr_grp,
			     struct uc_gpid_group *uc_grp, struct pp_stats *st)
{
	struct pmgr_db *db = pmgr_get_db();
	s32 ret = 0;

	if (ptr_is_null(db))
		return -EPERM;

	spin_lock_bh(&db->lock);
	if (!group_is_active(db, grp_id)) {
		ret = -EINVAL;
		goto unlock;
	}

	if (!uc_grp)
		goto copy_stats;

	ret = uc_ing_gpid_group_info_get(grp_id, uc_grp);
	if (ret) {
		pr_err("Failed to get GPID group %u uc info, ret %d\n", grp_id,
		       ret);
		goto unlock;
	}

copy_stats:
	if (!st)
		goto copy_pmgr_info;

	ret = chk_exception_stats_get(grp_id, st);
	if (ret) {
		pr_err("Failed to get GPID group %u stats, ret %d\n", grp_id,
		       ret);
		goto unlock;
	}

copy_pmgr_info:
	if (pmgr_grp)
		memcpy(pmgr_grp, &db->grp[grp_id], sizeof(*pmgr_grp));

unlock:
	spin_unlock_bh(&db->lock);
	return ret;
}

s32 __pp_rule_ip_mac(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index)
{
	struct pp_whitelist_field *field;

	if (bmap & BIT(PP_FLD_SRC_MAC)) {
		field = &fields[type_2_index[PP_FLD_SRC_MAC]];
		memcpy(rule_data->rule_ip_mac.mac,
			field->src_mac,
			sizeof(field->src_mac));
	} else if (bmap & BIT(PP_FLD_DST_MAC))	{
		field = &fields[type_2_index[PP_FLD_DST_MAC]];
		memcpy(rule_data->rule_ip_mac.mac,
			field->dst_mac, sizeof(field->src_mac));
	}

	if (bmap & BIT(PP_FLD_IPV4_SRC_IP)) {
		field = &fields[type_2_index[PP_FLD_IPV4_SRC_IP]];
		rule_data->rule_ip_mac.ip.v4 = field->src_ip.v4;
	} else if (bmap & BIT(PP_FLD_IPV4_DST_IP)) {
		field = &fields[type_2_index[PP_FLD_IPV4_DST_IP]];
		rule_data->rule_ip_mac.ip.v4 = field->dst_ip.v4;
	} else if (bmap & BIT(PP_FLD_IPV6_SRC_IP)) {
		field = &fields[type_2_index[PP_FLD_IPV6_SRC_IP]];
		memcpy(&rule_data->rule_ip_mac.ip.v6,
			&field->src_ip.v6,
			sizeof(field->src_ip.v6));
	} else if (bmap & BIT(PP_FLD_IPV6_DST_IP)) {
		field = &fields[type_2_index[PP_FLD_IPV6_DST_IP]];
		memcpy(&rule_data->rule_ip_mac.ip.v6,
			&field->dst_ip.v6,
			sizeof(field->dst_ip.v6));
	}

	return 0;
}

s32 __pp_rule_mac(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index)
{
	struct pp_whitelist_field *field;

	if (bmap & BIT(PP_FLD_SRC_MAC)) {
		field = &fields[type_2_index[PP_FLD_SRC_MAC]];
		memcpy(rule_data->rule_mac.mac,
			field->src_mac,
			sizeof(field->src_mac));
	} else if (bmap & BIT(PP_FLD_DST_MAC))	{
		field = &fields[type_2_index[PP_FLD_DST_MAC]];
		memcpy(rule_data->rule_mac.mac,
			field->src_mac,
			sizeof(field->src_mac));
	}

	return 0;
}

s32 __pp_rule_eth_type(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index)
{
	struct pp_whitelist_field *field;

	field = &fields[type_2_index[PP_FLD_ETH_TYPE]];
	rule_data->rule_eth.eth_type = field->eth_type;
	return 0;
}

s32 __pp_rule_ip_proto_ports(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index)
{
	struct pp_whitelist_field *field;

	field = &fields[type_2_index[PP_FLD_IP_PROTO]];
	rule_data->rule_ip_proto_ports.ip_proto = field->ip_proto;

	if (bmap & BIT(PP_FLD_L4_SRC_PORT)) {
		field = &fields[type_2_index[PP_FLD_L4_SRC_PORT]];
		rule_data->rule_ip_proto_ports.src_port = field->src_port;
	}
	if (bmap & BIT(PP_FLD_L4_DST_PORT)) {
		field = &fields[type_2_index[PP_FLD_L4_DST_PORT]];
		rule_data->rule_ip_proto_ports.dst_port = field->dst_port;
	}

	return 0;
}

s32 __pp_rule_icmp(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index)
{
	struct pp_whitelist_field *field;

	field = &fields[type_2_index[PP_FLD_ICMP_TYPE_CODE]];
	rule_data->rule_icmp.icmp_type = (u8)field->icmp_type_code;
	rule_data->rule_icmp.icmp_code = (u8)(field->icmp_type_code >> 8);

	return 0;
}

s32 __pp_rule_ipv4_ports(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index)
{
	struct pp_whitelist_field *field;

	field = &fields[type_2_index[PP_FLD_L4_SRC_PORT]];
	rule_data->rule_ipv4_ports.src_port = field->src_port;
	field = &fields[type_2_index[PP_FLD_L4_DST_PORT]];
	rule_data->rule_ipv4_ports.dst_port = field->dst_port;
	field = &fields[type_2_index[PP_FLD_IP_PROTO]];
	rule_data->rule_ipv4_ports.ip_proto = field->ip_proto;

	if (bmap & BIT(PP_FLD_IPV4_SRC_IP)) {
		field = &fields[type_2_index[PP_FLD_IPV4_SRC_IP]];
		rule_data->rule_ipv4_ports.v4 =	field->src_ip.v4;
	} else if (bmap & BIT(PP_FLD_IPV4_DST_IP)) {
		field = &fields[type_2_index[PP_FLD_IPV4_DST_IP]];
		rule_data->rule_ipv4_ports.v4 = field->dst_ip.v4;
	}

	return 0;
}

s32 __pp_rule_two_ipv4_ports(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index)
{
	struct pp_whitelist_field *field;

	field = &fields[type_2_index[PP_FLD_L4_SRC_PORT]];
	rule_data->rule_double_ipv4_ports.src_port = field->src_port;
	field = &fields[type_2_index[PP_FLD_L4_DST_PORT]];
	rule_data->rule_double_ipv4_ports.dst_port = field->dst_port;
	field = &fields[type_2_index[PP_FLD_IP_PROTO]];
	rule_data->rule_double_ipv4_ports.ip_proto = field->ip_proto;
	field = &fields[type_2_index[PP_FLD_IPV4_SRC_IP]];
	rule_data->rule_double_ipv4_ports.src = field->src_ip.v4;
	field = &fields[type_2_index[PP_FLD_IPV4_DST_IP]];
	rule_data->rule_double_ipv4_ports.dst =	field->dst_ip.v4;

	return 0;
}

s32 __pp_rule_ipv6_ports(u32 bmap, struct pp_whitelist_field *fields,
			union ing_wl_rule_union *rule_data, u8 *type_2_index)
{
	struct pp_whitelist_field *field;

	field = &fields[type_2_index[PP_FLD_L4_SRC_PORT]];
	rule_data->rule_ipv6_ports.src_port = field->src_port;
	field = &fields[type_2_index[PP_FLD_L4_DST_PORT]];
	rule_data->rule_ipv6_ports.dst_port = field->dst_port;
	field = &fields[type_2_index[PP_FLD_IP_PROTO]];
	rule_data->rule_ipv6_ports.ip_proto = field->ip_proto;

	if (bmap & BIT(PP_FLD_IPV6_SRC_IP)) {
		field = &fields[type_2_index[PP_FLD_IPV6_SRC_IP]];
		memcpy(&rule_data->rule_ipv6_ports.v6,
			&field->src_ip.v6,
			sizeof(field->src_ip.v6));
	} else if (bmap & BIT(PP_FLD_IPV6_DST_IP)) {
		field = &fields[type_2_index[PP_FLD_IPV6_DST_IP]];
		memcpy(&rule_data->rule_ipv6_ports.v6,
			&field->dst_ip.v6,
			sizeof(field->dst_ip.v6));
	}

	return 0;
}

static s32 pp_bmap_to_rule_type(u32 bmap, u32 *rule_type)
{
	switch (bmap) {
	case (BIT(PP_FLD_SRC_MAC) | BIT(PP_FLD_IPV4_SRC_IP)):
	case (BIT(PP_FLD_SRC_MAC) | BIT(PP_FLD_IPV4_DST_IP)):
	case (BIT(PP_FLD_SRC_MAC) | BIT(PP_FLD_IPV6_SRC_IP)):
	case (BIT(PP_FLD_SRC_MAC) | BIT(PP_FLD_IPV6_DST_IP)):
	case (BIT(PP_FLD_DST_MAC) | BIT(PP_FLD_IPV4_SRC_IP)):
	case (BIT(PP_FLD_DST_MAC) | BIT(PP_FLD_IPV4_DST_IP)):
	case (BIT(PP_FLD_DST_MAC) | BIT(PP_FLD_IPV6_SRC_IP)):
	case (BIT(PP_FLD_DST_MAC) | BIT(PP_FLD_IPV6_DST_IP)):
		*rule_type = ING_WL_RULE_TYPE_IP_MAC;
		break;
	case (BIT(PP_FLD_SRC_MAC)):
	case (BIT(PP_FLD_DST_MAC)):
		*rule_type = ING_WL_RULE_TYPE_MAC;
		break;
	case (BIT(PP_FLD_ETH_TYPE)):
		*rule_type = ING_WL_RULE_TYPE_ETH_TYPE;
		break;
	case (BIT(PP_FLD_IP_PROTO)):
	case (BIT(PP_FLD_L4_SRC_PORT) | BIT(PP_FLD_IP_PROTO)):
	case (BIT(PP_FLD_L4_DST_PORT) | BIT(PP_FLD_IP_PROTO)):
	case (BIT(PP_FLD_L4_SRC_PORT) | BIT(PP_FLD_L4_DST_PORT) |
	      BIT(PP_FLD_IP_PROTO)):
		*rule_type = ING_WL_RULE_TYPE_IP_PROTO_PORTS;
		break;
	case (BIT(PP_FLD_ICMP_TYPE_CODE)):
		*rule_type = ING_WL_RULE_TYPE_ICMP;
		break;
	case (BIT(PP_FLD_L4_SRC_PORT) | BIT(PP_FLD_L4_DST_PORT) |
	      BIT(PP_FLD_IPV4_SRC_IP) | BIT(PP_FLD_IP_PROTO)):
	case (BIT(PP_FLD_L4_SRC_PORT) | BIT(PP_FLD_L4_DST_PORT) |
	      BIT(PP_FLD_IPV4_DST_IP) | BIT(PP_FLD_IP_PROTO)):
		*rule_type = ING_WL_RULE_TYPE_IPV4_SRC_OR_DST_AND_L4_PORTS;
		break;
	case (BIT(PP_FLD_L4_SRC_PORT) | BIT(PP_FLD_L4_DST_PORT) |
	      BIT(PP_FLD_IPV4_DST_IP) | BIT(PP_FLD_IPV4_SRC_IP) |
	      BIT(PP_FLD_IP_PROTO)):
		*rule_type = ING_WL_RULE_TYPE_IPV4_SRC_AND_DST_AND_L4_PORTS;
		break;
	case (BIT(PP_FLD_L4_SRC_PORT) | BIT(PP_FLD_L4_DST_PORT) |
	      BIT(PP_FLD_IPV6_SRC_IP) | BIT(PP_FLD_IP_PROTO)):
	case (BIT(PP_FLD_L4_SRC_PORT) | BIT(PP_FLD_L4_DST_PORT) |
	      BIT(PP_FLD_IPV6_DST_IP) | BIT(PP_FLD_IP_PROTO)):
		*rule_type = ING_WL_RULE_TYPE_IPV6_SRC_OR_DST_AND_L4_PORTS;
		break;
	default:
		pr_err("unsupported rule fields combination 0x%x\n", bmap);
		return -EINVAL;
	}

	pr_debug("rule_type %u\n", *rule_type);

	return 0;
}

/**
 * @brief Converts PP white list fields into uc white list rule
 * @param priority rule's priority
 * @param fields fields to convert
 * @param cnt number of fields
 * @return int -EINVAL in case fields type isn't supported, 0 otherwise
 */
static int to_uc_whitelist_rule(unsigned int priority,
				struct pp_whitelist_field *fields, int cnt,
				union ing_wl_rule_union *uc_rule,
				u32 *rule_type)
{
	struct pp_whitelist_field *field;
	unsigned int i;
	unsigned long bmap = 0;
	s32 ret;
	u8 type_2_index[PP_FLD_COUNT];

	if (cnt == 0 || cnt >= PP_FLD_COUNT) {
		pr_err("unsupported number of rule fields %u, valid range is 1-%u\n",
		       cnt, PP_FLD_COUNT);
		return -EINVAL;
	}

	for (i = 0, field = &fields[0]; i < cnt; i++, field++) {
		set_bit(field->type, &bmap);
		type_2_index[field->type] = i;
	}

	ret = pp_bmap_to_rule_type(bmap, rule_type);
	if (ret)
		return ret;

	if (!pp_to_uc_rule[*rule_type]) {
		pr_err("no valid rule function\n");
		return -EPERM;
	}

	ret = pp_to_uc_rule[*rule_type]((u32)bmap, fields, uc_rule,
					type_2_index);

	uc_rule->rule_common.priority = priority;
	uc_rule->rule_common.fields_en = bmap;

	return ret;
}

s32 pp_gpid_group_rule_add(u32 grp_id, u32 prio,
			   struct pp_whitelist_field *fields, u32 cnt)
{
	union ing_wl_rule_union uc_rule = {0};
	struct pmgr_db *db = pmgr_get_db();
	u32 rule_type, rule_idx = 0;
	s32 ret;

	if (ptr_is_null(db))
		return -EPERM;

	if (ptr_is_null(fields) || !uc_gpid_group_priority_is_valid(prio))
		return -EINVAL;

	/* convert rule to uC data-structure */
	ret = to_uc_whitelist_rule(prio, fields, cnt, &uc_rule, &rule_type);
	if (ret)
		return ret;

	spin_lock_bh(&db->lock);
	if (!group_is_active(db, grp_id)) {
		ret = -EINVAL;
		goto done;
	}

	if (uc_is_whitelist_rule_exist(&uc_rule, rule_type, &rule_idx)) {
		if (!uc_ing_gpid_group_rule_is_en(grp_id, rule_type, rule_idx))
			goto map_group;

		pr_err("gpid group %u already mapped to %s rule%u\n", grp_id,
		       uc_wl_rule_type_name(rule_type), rule_idx);
		ret = -EEXIST;
		goto done;
	}

	ret = uc_ing_whitelist_rule_add(&uc_rule, rule_type, &rule_idx);
	if (ret)
		goto done;

map_group:
	db->wl_rule_ref[rule_type][rule_idx]++;
	ret = uc_ing_gpid_group_rule_en(grp_id, rule_type, rule_idx);
	if (ret)
		pr_err("Failed to enable rule%u.%u for group %u in UC, ret %d",
		       rule_type, rule_idx, grp_id, ret);
done:
	spin_unlock_bh(&db->lock);

	if (!ret && db->wl_rule_ref[rule_type][rule_idx] == 1)
		pr_debug("New rule added, type %u, index %u ref %u, group id %u\n",
			 rule_type, rule_idx,
			 db->wl_rule_ref[rule_type][rule_idx], grp_id);

	return ret;
}
EXPORT_SYMBOL(pp_gpid_group_rule_add);

s32 pp_gpid_group_rule_del(u32 grp_id, u32 prio,
			   struct pp_whitelist_field *fields, u32 cnt)
{
	union ing_wl_rule_union rule_data = {0};
	struct pmgr_db *db = pmgr_get_db();
	u32 rule_type;
	s32 ret;
	u32 rule_index;

	if (ptr_is_null(db))
		return -EPERM;

	if (ptr_is_null(fields) || !uc_gpid_group_priority_is_valid(prio))
		return -EINVAL;

	ret = to_uc_whitelist_rule(prio, fields, cnt, &rule_data, &rule_type);
	if (ret)
		return ret;

	spin_lock_bh(&db->lock);
	if (!group_is_active(db, grp_id)) {
		ret = -EINVAL;
		goto done;
	}

	if (!uc_is_whitelist_rule_exist(&rule_data, rule_type, &rule_index)) {
		pr_err("rule doesn't exist\n");
		ret = -ENOENT;
		goto done;
	}

	ret = uc_ing_gpid_group_rule_dis(grp_id, rule_type, rule_index);
	if (ret)
		pr_err("Failed to disable rule%u.%u for group %u in UC, ret %d",
		       rule_type, rule_index, grp_id, ret);

	if (--db->wl_rule_ref[rule_type][rule_index])
		goto done;

	ret = uc_ing_whitelist_rule_del(rule_type, rule_index);
	if (!ret)
		pr_debug("Delete rule type %u, index %u\n", rule_type,
			 rule_index);

done:
	spin_unlock_bh(&db->lock);
	return ret;
}
EXPORT_SYMBOL(pp_gpid_group_rule_del);

s32 pmgr_gpid_group_hash_bit_set(u32 grp_id, bool en)
{
	struct pmgr_db *db = pmgr_get_db();
	s32 ret;

	if (ptr_is_null(db))
		return -EPERM;

	spin_lock_bh(&db->lock);
	if (!group_is_active(db, grp_id)) {
		ret = -EINVAL;
		goto done;
	}

	ret = uc_ing_gpid_group_hash_bit_set(grp_id, en);

done:
	spin_unlock_bh(&db->lock);
	return ret;
}

s32 pp_gpid_group_get_port_map(u32 port_id, u32 *grp_id)
{
	struct pmgr_db *db = pmgr_get_db();
	struct pmgr_db_port *port = &db->port[port_id];;

	if (ptr_is_null(grp_id))
		return -EINVAL;
	if (ptr_is_null(db))
		return -EPERM;

	*grp_id = PMGR_GPID_GRP_INVALID;

	spin_lock_bh(&db->lock);
	if (!__pmgr_port_is_active(port_id)) {
		spin_unlock_bh(&db->lock);
		return -EINVAL;
	}
	if (group_is_active(db, port->grp_id))
		*grp_id = port->grp_id;

	spin_unlock_bh(&db->lock);
	return 0;
}
EXPORT_SYMBOL(pp_gpid_group_get_port_map);

static inline int gpid_grp_add_port(u32 grp_id, u32 port_id, u32 dflt_prio)
{
	struct pmgr_db *db = pmgr_get_db();
	struct pmgr_db_grp *grp = &db->grp[grp_id];
	struct pmgr_db_port *port = &db->port[port_id];
	u8 tc, hoh, map[PP_MAX_TC][CHK_HOH_MAX];
	s32 ret = 0;

	/* if the port is protected, the previous mapping was already saved,
	 * and we skip the actual mapping in HW cause it currently mapped
	 * to the pp protected group
	 */
	if (pmgr_port_is_protected(port_id))
		goto map_in_db;

	/* backup current mapping */
	ret = chk_exception_session_port_map_get(port_id, port->prev_map_id);
	if (ret)
		return ret;

	/* prepare new mapping to group's exception */
	PMGR_HIF_FOR_EACH_DP_TC(tc)
		PMGR_HIF_FOR_EACH_DP_HOH(hoh)
			map[tc][hoh] = grp_id;

	/* Configure new mapping */
	ret = chk_exception_session_port_map_set(port_id, map);
	if (ret)
		return ret;

	ret = uc_ing_gpid_group_dflt_prio_port_set(port_id, dflt_prio);
	if (ret)
		return ret;

	/* set bi-directional mapping in DB */
map_in_db:
	set_bit(port_id, grp->gpid_bmap);
	port->grp_id = grp_id;
	port->dflt_priority = dflt_prio;
	return 0;
}

s32 pp_gpid_group_add_port(u32 grp_id, u32 port_id, u32 dflt_prio)
{
	struct pmgr_db *db = pmgr_get_db();
	struct pmgr_db_port *port = &db->port[port_id];
	s32 ret = 0;

	if (ptr_is_null(db))
		return -EPERM;
	if (!uc_gpid_group_priority_is_valid(dflt_prio))
		return -EINVAL;


	spin_lock_bh(&db->lock);
	if (!group_is_active(db, grp_id) || !__pmgr_port_is_active(port_id)) {
		ret = -EINVAL;
		goto done;
	}

	if (port->grp_id == grp_id) {
		pr_debug("port%u already mapped to group %u\n", port_id,
			 grp_id);
		ret = 0;
		goto done;
	}

	if (__uc_gpid_group_id_is_valid(port->grp_id)) {
		pr_err("port%u already mapped to group %u\n", port_id,
		       port->grp_id);
		ret = -EPERM;
		goto done;
	}

	gpid_grp_add_port(grp_id, port_id, dflt_prio);

done:
	spin_unlock_bh(&db->lock);
	if (!ret)
		pr_debug("Port %u mapped to GPID group %u\n", port_id, grp_id);

	return ret;
}
EXPORT_SYMBOL(pp_gpid_group_add_port);

static inline int gpid_grp_del_port(u32 grp_id, u32 port_id)
{
	struct pmgr_db *db = pmgr_get_db();
	struct pmgr_db_port *port = &db->port[port_id];
	struct pmgr_db_grp *grp = &db->grp[grp_id];
	u32 prio = UC_WHITELIST_LOWEST_PRIO;

	/* clean bi-directional mapping in DB */
	clear_bit(port_id, grp->gpid_bmap);
	port->grp_id = PMGR_GPID_GRP_INVALID;

	uc_ing_gpid_group_dflt_prio_port_set(port_id, prio);
	port->dflt_priority = prio;

	/* if the port is protected, we don't restore the actual mapping in HW
	 * cause it is currently mapped to the pp protected group.
	 * it will be done later when the protection will be stopped.
	 */
	if (pmgr_port_is_protected(port_id))
		return 0;

	/* Restore old mapping */
	return chk_exception_session_port_map_set(port_id, port->prev_map_id);
}

s32 pp_gpid_group_del_port(u32 grp_id, u32 port_id)
{
	struct pmgr_db *db = pmgr_get_db();
	struct pmgr_db_port *port = &db->port[port_id];
	s32 ret;

	if (ptr_is_null(db))
		return -EPERM;

	spin_lock_bh(&db->lock);
	if (!group_is_active(db, grp_id) || !__pmgr_port_is_active(port_id)) {
		ret = -EINVAL;
		goto done;
	}

	if (port->grp_id != grp_id) {
		pr_err("port%u isn't mapped to group %u\n", port_id, grp_id);
		ret = -EINVAL;
		goto done;
	}

	ret = gpid_grp_del_port(grp_id, port_id);

done:
	spin_unlock_bh(&db->lock);
	if (!ret)
		pr_debug("Port %u was unmapped from GPID group %u\n", port_id,
			 grp_id);

	return ret;
}
EXPORT_SYMBOL(pp_gpid_group_del_port);

s32 pmgr_gpid_group_move_port(u16 port_id, u32 dst_grp, u32 src_grp)
{
	struct pmgr_db *db = pmgr_get_db();
	struct pmgr_db_port *port = &db->port[port_id];
	u32 prio;
	int ret = 0;

	if (ptr_is_null(db))
		return -EPERM;

	spin_lock_bh(&db->lock);
	if (!__pmgr_port_is_active(port_id) ||
	    (__uc_gpid_group_id_is_valid(src_grp) &&
	     !group_is_active(db, src_grp)) ||
	    (__uc_gpid_group_id_is_valid(dst_grp) &&
	     !group_is_active(db, dst_grp))) {
		ret = -EINVAL;
		goto done;
	}

	if (port->grp_id != src_grp) {
		pr_err("port%u isn't mapped to group %u\n", port_id, src_grp);
		ret = -EINVAL;
		goto done;
	}

	prio = port->dflt_priority;
	if (__uc_gpid_group_id_is_valid(src_grp)) {
		ret = gpid_grp_del_port(src_grp, port_id);
		if (ret)
			goto done;
	}

	if (__uc_gpid_group_id_is_valid(dst_grp))
		ret = gpid_grp_add_port(dst_grp, port_id, prio);

done:
	spin_unlock_bh(&db->lock);
	if (!ret)
		pr_debug("Port%u was moved from group%u to group%u\n", port_id,
			 src_grp, dst_grp);

	return ret;
}
