/*
 * Copyright (C) 2022-2025 MaxLinear, Inc.
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
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PP micro-controllers gpid grouping module
 */

#define pr_fmt(fmt) "[PP_UC_GRP]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/pp_api.h>

#include "pp_regs.h"
#include "pp_common.h"
#include "pp_dma.h"
#include "infra.h"
#include "classifier.h"
#include "checker.h"
#include "rx_dma.h"
#include "uc.h"
#include "uc_regs.h"
#include "uc_internal.h"
#include <soc/mxl/mxl_sec.h>

struct rule_type_info {
	/* number of rules available for the type of rule */
	u16 max_rules;
	/*! rule size */
	u16 sz;
	/*! rules offset within struct ing_wl_rules */
	u16 off;
	/*! description */
	const char *name;
	/* rules enable bitmap */
	ulong en_bmap[BITS_TO_LONGS(ING_WL_RULE_MAX_RULES)];
	/* the rules */
	union ing_wl_rule_union rule[ING_WL_RULE_MAX_RULES];
};

struct uc_group_info {
	/*! bitmap specifying which rule types are enabled for the group */
	ulong rule_type_bmap_en[BITS_TO_LONGS(ING_WL_RULE_TYPE_COUNT)];
	/*! per rule type bitmap specifying which rule is enable for
	 *  the group
	 */
	ulong rule_idx_bmap_en[ING_WL_RULE_TYPE_COUNT]
			      [BITS_TO_LONGS(ING_WL_RULE_MAX_RULES)];
};

struct wl_rules_db {
	/*! per gpid group info */
	struct uc_group_info grp_info[UC_GPID_GRP_CNT];
	/*! per rule type info */
	struct rule_type_info rule_info[ING_WL_RULE_TYPE_COUNT];
	/*! bitmap specifying which types has active rules */
	ulong type_en_bmap[BITS_TO_LONGS(ING_WL_RULE_TYPE_COUNT)];
};

const char *wl_rules_name[ING_WL_RULE_TYPE_COUNT] = {
	[ING_WL_RULE_TYPE_INVALID] = "Invalid",
	[ING_WL_RULE_TYPE_IP_MAC] = "Ip mac",
	[ING_WL_RULE_TYPE_MAC] = "Mac",
	[ING_WL_RULE_TYPE_ETH_TYPE] = "Eth",
	[ING_WL_RULE_TYPE_IP_PROTO_PORTS] = "ip_proto L4 ports",
	[ING_WL_RULE_TYPE_ICMP] = "Icmp",
	[ING_WL_RULE_TYPE_IPV4_SRC_OR_DST_AND_L4_PORTS] = "Ipv4 l4 ports",
	[ING_WL_RULE_TYPE_IPV4_SRC_AND_DST_AND_L4_PORTS] = "Two ipv4 l4 ports",
	[ING_WL_RULE_TYPE_IPV6_SRC_OR_DST_AND_L4_PORTS] = "Ipv6 l4 ports",
	[ING_WL_RULE_TYPE_BITHASH] = "Bithash",
};

/* white list db, used only to avoid allocating
 * DDR memory every time we add a new rule
 */
struct wl_rules_db *wl_rules_db;

bool __uc_gpid_group_id_is_valid(u32 id)
{
	return id < UC_GPID_GRP_CNT;
}

bool uc_gpid_group_id_is_valid(u32 id)
{
	if (__uc_gpid_group_id_is_valid(id))
		return true;

	pr_err("GPID Group id %u is invalid\n", id);
	return false;
}

bool uc_gpid_group_priority_is_valid(u32 prio)
{
	if (prio < UC_WHITELIST_NUM_PRIO)
		return true;

	pr_err("GPID Group priority %u is invalid\n", prio);
	return false;
}

s32 uc_ing_gpid_group_create(u32 grp_id,
			     struct ing_host_cpu_info cpu[PP_MAX_HOST_CPUS])
{
	struct uc_gpid_group *grp;
	u32 i;

	if (!uc_gpid_group_id_is_valid(grp_id))
		return -EINVAL;

	/* copy cpu info to all CPUs memory */
	for_each_ing_active_cpu(i) {
		grp = (struct uc_gpid_group *)
			DCCM_INGRESS_ADDR(i, ING_GRP_INFO_BASE(grp_id));

		memcpy_toio(grp->cpu, cpu, sizeof(grp->cpu));
	}

	return 0;
}

s32 uc_ing_gpid_group_delete(u32 grp_id)
{
	struct uc_gpid_group *grp;
	u32 i;

	if (!uc_gpid_group_id_is_valid(grp_id))
		return -EINVAL;

	/* copy cpu info to all CPUs memory */
	for_each_ing_active_cpu(i) {
		grp = (struct uc_gpid_group *)
			DCCM_INGRESS_ADDR(i, ING_GRP_INFO_BASE(grp_id));
		memset_io(grp, 0, sizeof(*grp));
	}

	return 0;
}

s32 uc_ing_gpid_group_dflt_prio_port_set(u32 id, u32 prio)
{
	u64 *port_addr;
	u8 i, _prio = prio;

	if (!__pp_is_port_valid(id) ||
	    !uc_gpid_group_priority_is_valid(prio))
		return -EINVAL;

	/* copy cpu info to all CPUs memory */
	for_each_ing_active_cpu(i) {
		port_addr = (u64 *)DCCM_INGRESS_ADDR(
			i, ING_DFLT_PORT_PRIORITY_OFF(id));
		memcpy_toio(port_addr, &_prio, sizeof(_prio));
	}
	return 0;
}

s32 uc_ing_gpid_group_info_get(u32 grp_id, struct uc_gpid_group *grp)
{
	struct uc_gpid_group *db_grp;
	u32 cid;

	if (!uc_gpid_group_id_is_valid(grp_id) || ptr_is_null(grp))
		return -EINVAL;

	if (!in_cpu_act_bmap) {
		pr_err("no active ingress cpu found\n");
		return -EPERM;
	}

	cid = find_first_bit(&in_cpu_act_bmap, UC_CPUS_MAX);
	db_grp = (struct uc_gpid_group *)
		DCCM_INGRESS_ADDR(cid, ING_GRP_INFO_BASE(grp_id));

	memcpy_fromio(grp, db_grp, sizeof(*db_grp));
	return 0;

}

s32 uc_ing_gpid_group_gpid_set(u32 grp_id, u32 host_cid, u32 prio, u32 gpid)
{
	struct uc_gpid_group *grp;
	u32 uc_cid;

	if (!uc_gpid_group_id_is_valid(grp_id) ||
	    !uc_gpid_group_priority_is_valid(prio))
		return -EINVAL;

	if (host_cid >= UC_ING_MAX_HOST_CPU) {
		pr_err("Invalid host cpu id %u\n", host_cid);
		return -EINVAL;
	}

	for_each_ing_active_cpu(uc_cid) {
		grp = (struct uc_gpid_group *)
			DCCM_INGRESS_ADDR(uc_cid, ING_GRP_INFO_BASE(grp_id));
		PP_REG_WR8(&grp->cpu[host_cid].gpid[prio], gpid);
	}

	return 0;
}

s32 uc_ing_gpid_group_queue_set(u32 grp_id, u32 host_cid, u32 prio, u32 phy_qid)
{
	struct uc_gpid_group *grp;
	u32 uc_cid;

	if (!uc_gpid_group_id_is_valid(grp_id) ||
	    !uc_gpid_group_priority_is_valid(prio))
		return -EINVAL;

	if (host_cid >= UC_ING_MAX_HOST_CPU) {
		pr_err("Invalid host cpu id %u\n", host_cid);
		return -EINVAL;
	}

	for_each_ing_active_cpu(uc_cid) {
		grp = (struct uc_gpid_group *)
			DCCM_INGRESS_ADDR(uc_cid, ING_GRP_INFO_BASE(grp_id));
		PP_REG_WR16(&grp->cpu[host_cid].qid[prio], phy_qid);
	}

	return 0;

}

s32 uc_ing_hash_bit_db_get(struct hash_bit_db *dst, u32 cnt)
{
	u32 cid, i = 0;
	u64 addr;

	if (ptr_is_null(dst))
		return -EINVAL;

	/* get the hash bit db from each CPU */
	for_each_ing_active_cpu(cid) {
		if (i >= cnt)
			break;
		addr = DCCM_INGRESS_ADDR(cid, ING_HASH_BIT_DB_BASE);
		memcpy_fromio(&dst[i], (void *)addr, sizeof(*dst));
		i++;
	}

	return 0;
}

s32 uc_ing_hash_bit_enable(u32 index)
{
	u16 bit = index % 32;
	u16 word = index / 32;
	u32 cid, word_val;
	u64 *word_addr;

	if (index >= PP_DPL_HASH_BIT_MAX_ENTRIES) {
		pr_err("%s : index %d not supported\n", __func__, index);
		return -EINVAL;
	}

	/* get the val from the first CPU */
	cid = find_first_bit(&in_cpu_act_bmap, UC_CPUS_MAX);
	word_addr = (u64 *)DCCM_INGRESS_ADDR(cid, ING_HASH_BIT_DB_OFF(word));

	memcpy_fromio(&word_val, word_addr, sizeof(word_val));

	if (test_bit(bit, (unsigned long *)&word_val)) {
		/* already enabled */
		pr_err("%s : index %u already enabled\n", __func__, index);
		return -EINVAL;
	}

	set_bit(bit, (unsigned long *)&word_val);

	/* lastly, enable the id in the enable bitmap on all CPUs */
	for_each_ing_active_cpu(cid) {
		word_addr = (u64 *)DCCM_INGRESS_ADDR(cid,
						     ING_HASH_BIT_DB_OFF(word));
		memcpy_toio(word_addr, &word_val, sizeof(word_val));
	}

	return 0;
}

s32 uc_ing_hash_bit_disable(u32 index)
{
	u16 bit = index % 32;
	u16 word = index / 32;
	u32 cid, word_val;
	u64 *word_addr;

	if (index >= PP_DPL_HASH_BIT_MAX_ENTRIES) {
		pr_err("%s : index %d not supported\n", __func__, index);
		return -EINVAL;
	}

	/* get the val from the first CPU */
	cid = find_first_bit(&in_cpu_act_bmap, UC_CPUS_MAX);
	word_addr = (u64 *)DCCM_INGRESS_ADDR(cid, ING_HASH_BIT_DB_OFF(word));

	memcpy_fromio(&word_val, word_addr, sizeof(word_val));

	if (test_bit(bit, (unsigned long *)&word_val)) {
		clear_bit(bit, (unsigned long *)&word_val);
	} else {
		/* already disabled */
		pr_err("%s : index %u already disabled\n", __func__, index);
		return -EINVAL;
	}

	/* lastly, update the id in the bitmap */
	for_each_ing_active_cpu(cid) {
		word_addr = (u64 *)DCCM_INGRESS_ADDR(cid,
						     ING_HASH_BIT_DB_OFF(word));
		memcpy_toio(word_addr, &word_val, sizeof(word_val));
	}

	return 0;
}

s32 uc_ing_hash_bit_reset(void)
{
	u64 addr;
	u32 cid;
	struct hash_bit_db *hash_bit_db;

	for_each_ing_active_cpu(cid) {
		addr = DCCM_INGRESS_ADDR(cid, ING_HASH_BIT_DB_OFF(0));
		memset_io((void *)addr, 0, sizeof(hash_bit_db->hash_bit));
	}
	return 0;
}

s32 uc_ing_whitelist_db_get(struct ing_wl_rules *dst)
{
	u64 addr;
	u32 cid;

	if (ptr_is_null(dst))
		return -EINVAL;

	if (!in_cpu_act_bmap) {
		pr_err("no active cid found\n");
		return -EPERM;
	}

	cid = find_first_bit(&in_cpu_act_bmap, UC_CPUS_MAX);
	addr = DCCM_INGRESS_ADDR(cid, ING_WL_OFFSET);
	memcpy_fromio(dst, (void *)addr, sizeof(*dst));

	return 0;
}

static inline bool rule_type_valid(enum ing_wl_rule_type t)
{
	if ((t < ING_WL_RULE_TYPE_COUNT) || (t > ING_WL_RULE_TYPE_INVALID))
		return true;

	pr_err("invalid rule type %u\n", t);
	return false;
}

static inline bool rule_index_valid(enum ing_wl_rule_type t, u32 idx)
{
	struct rule_type_info *info;

	if (!rule_type_valid(t))
		return false;

	if (!wl_rules_db)
		return false;

	info = &wl_rules_db->rule_info[t];
	if (idx < info->max_rules)
		return true;

	pr_err("invalid rule index %u for rule type %s\n", idx, info->name);
	return false;
}

bool uc_is_whitelist_rule_exist(union ing_wl_rule_union *rule,
				enum ing_wl_rule_type type, u32 *idx)
{
	struct rule_type_info *info;
	u32 i;

	if (!rule_type_valid(type))
		return false;

	if (!wl_rules_db)
		return false;

	info = &wl_rules_db->rule_info[type];

	for_each_set_bit(i, info->en_bmap, info->max_rules) {
		if (memcmp(&info->rule[i], rule, info->sz))
			continue;

		if (idx)
			*idx = i;

		pr_debug("rule type %u, exist in index %u\n", type, i);
		return true;
	}

	return false;
}

bool rule_index_active(enum ing_wl_rule_type type, u32 idx)
{
	struct rule_type_info *info;

	if (!rule_index_valid(type, idx))
		return false;

	if (!wl_rules_db)
		return -EINVAL;

	info = &wl_rules_db->rule_info[type];
	if (test_bit(type, wl_rules_db->type_en_bmap) &&
	    test_bit(idx, info->en_bmap))
		return true;

	pr_err("%s rule%u isn't active\n", info->name, idx);
	return false;
}

s32 uc_ing_whitelist_rule_del(enum ing_wl_rule_type type,
				  u32 idx)
{
	struct rule_type_info *info;
	struct uc_ing_cmd msg = { 0 };

	if (!rule_index_active(type, idx))
		return -EINVAL;

	if (!wl_rules_db)
		return -EINVAL;

	info = &wl_rules_db->rule_info[type];
	msg.msg_type = ING_MBOX_WL_DEL_RULE;
	msg.ing_cmd.del_rule.rule_index = idx;
	msg.ing_cmd.del_rule.rule_type = type;
	if (uc_ing_host_mbox_cmd_send(&msg, true)) {
		pr_err("Failed to send delete rule command\n");
		return -EPERM;
	}

	clear_bit(idx, info->en_bmap);
	if (bitmap_empty(info->en_bmap, info->max_rules))
		clear_bit(type, wl_rules_db->type_en_bmap);

	memset(&info->rule[idx], 0, sizeof(info->sz));

	return 0;
}

s32 uc_ing_whitelist_rule_add(union ing_wl_rule_union *rule,
			      enum ing_wl_rule_type type, u32 *rule_idx)
{
	struct uc_ing_cmd msg = { 0 };
	struct rule_type_info *type_info;
	u32 idx;
	s32 ret;

	if (ptr_is_null(rule) || !rule_type_valid(type))
		return -EINVAL;

	if (!wl_rules_db)
		return -EINVAL;

	type_info = &wl_rules_db->rule_info[type];
	idx = find_first_zero_bit(type_info->en_bmap, type_info->max_rules);
	if (idx >= type_info->max_rules) {
		pr_err("[rule type %s] : no free entry in table\n",
		       type_info->name);
		return -ENOSPC;
	}

	msg.ing_cmd.add_rule.rule_type = type;
	msg.ing_cmd.add_rule.rule_index = idx;
	msg.msg_type = ING_MBOX_WL_ADD_RULE;
	memcpy(&msg.ing_cmd.add_rule.rule_data, rule, sizeof(*rule));

	ret = uc_ing_host_mbox_cmd_send(&msg, true);
	if (ret) {
		pr_err("Failed to add rule to ingress uC, %d\n", ret);
		return -EPERM;
	}

	memcpy(&type_info->rule[idx], rule, type_info->sz);
	set_bit(idx, type_info->en_bmap);
	set_bit(type, wl_rules_db->type_en_bmap);
	if (rule_idx)
		*rule_idx = idx;

	return 0;
}

void uc_ing_whitelist_reset_all_rules(void)
{
	struct uc_ing_cmd msg = { 0 };
	struct rule_type_info *info;

	if (!wl_rules_db)
		return;

	msg.msg_type = ING_MBOX_WL_RESET_RULES;
	if (uc_ing_host_mbox_cmd_send(&msg, true))
		pr_err("Failed to send reset whitelist rules command\n");

	for_each_arr_entry(info, wl_rules_db->rule_info,
			   ARRAY_SIZE(wl_rules_db->rule_info)) {
		bitmap_zero(info->en_bmap, info->max_rules);
		memset(info->rule, 0, sizeof(info->rule));
	}
}

void uc_ing_whitelist_rules_test(void)
{
	struct uc_ing_cmd msg = {0};

	msg.msg_type = ING_MBOX_WL_TEST_RULES;
	if (uc_ing_host_mbox_cmd_send(&msg, true))
		pr_err("Failed to send TEST rules command\n");
}

bool uc_ing_gpid_group_rule_is_en(u32 grp_id, enum ing_wl_rule_type type,
				  u32 idx)
{
	struct uc_group_info *grp;

	if (!wl_rules_db)
		return false;

	grp = &wl_rules_db->grp_info[grp_id];
	if (!uc_gpid_group_id_is_valid(grp_id) || !rule_index_active(type, idx))
		return false;

	return test_bit(type, grp->rule_type_bmap_en) &&
	       test_bit(idx, grp->rule_idx_bmap_en[type]);
}

s32 uc_ing_gpid_group_rule_set(u32 grp_id, enum ing_wl_rule_type type, u32 idx,
			       bool en)
{
	struct rule_type_info *info;
	struct uc_group_info *grp;
	struct uc_ing_cmd msg = { 0 };
	s32 ret;

	if (!uc_gpid_group_id_is_valid(grp_id) || !rule_index_active(type, idx))
		return -EINVAL;

	if (!wl_rules_db)
		return -EINVAL;

	info = &wl_rules_db->rule_info[type];
	grp = &wl_rules_db->grp_info[grp_id];

	msg.ing_cmd.grp_rule.grp_id = grp_id;
	msg.ing_cmd.grp_rule.rule_index = idx;
	msg.ing_cmd.grp_rule.rule_type = type;
	msg.msg_type = en ? ING_MBOX_WL_GRP_RULE_EN : ING_MBOX_WL_GRP_RULE_DIS;
	ret = uc_ing_host_mbox_cmd_send(&msg, true);
	if (ret) {
		pr_err("Failed to %s %s rule%u for group %u in ingress uC, %d\n",
		       BOOL2EN(en), info->name, idx, grp_id, ret);
		return ret;
	}

	if (en) {
		set_bit(idx, grp->rule_idx_bmap_en[type]);
		set_bit(type, grp->rule_type_bmap_en);
	} else {
		clear_bit(idx, grp->rule_idx_bmap_en[type]);
		if (bitmap_empty(grp->rule_idx_bmap_en[type], info->max_rules))
			clear_bit(type, grp->rule_type_bmap_en);
	}

	return 0;
}

s32 uc_ing_gpid_group_hash_bit_set(u32 grp_id, bool en)
{
	struct uc_group_info *grp;
	struct uc_ing_cmd msg = { 0 };
	enum ing_wl_rule_type type = ING_WL_RULE_TYPE_BITHASH;
	s32 ret;

	if (!uc_gpid_group_id_is_valid(grp_id))
		return -EINVAL;

	if (!wl_rules_db)
		return -EINVAL;

	if (en == uc_ing_gpid_group_hash_bit_is_en(grp_id)) {
		pr_err("group %u already has bit hash %s\n", grp_id,
		       BOOL2EN(en));
		return -EPERM;
	}

	grp = &wl_rules_db->grp_info[grp_id];
	msg.ing_cmd.grp_rule.grp_id = grp_id;
	msg.ing_cmd.grp_rule.rule_type = type;
	msg.msg_type =
		en ? ING_MBOX_WL_GRP_BHASH_EN : ING_MBOX_WL_GRP_BHASH_DIS;
	ret = uc_ing_host_mbox_cmd_send(&msg, true);
	if (ret) {
		pr_err("Failed to %s bithash for group %u in ingress uC, %d\n",
		       BOOL2EN(en), grp_id, ret);
		return ret;
	}

	if (en)
		set_bit(type, grp->rule_type_bmap_en);
	else
		clear_bit(type, grp->rule_type_bmap_en);

	return 0;
}

bool uc_ing_gpid_group_hash_bit_is_en(u32 grp_id)
{
	struct uc_group_info *grp;

	if (!uc_gpid_group_id_is_valid(grp_id))
		return false;

	if (!wl_rules_db)
		return false;

	grp = &wl_rules_db->grp_info[grp_id];

	return test_bit(ING_WL_RULE_TYPE_BITHASH, grp->rule_type_bmap_en);
}

u32 _uc_ing_calc_hits_offset(enum ing_wl_rule_type rule_type,
			     unsigned int rule_index)
{
	struct rule_type_info *info;

	if (!wl_rules_db)
		return 0;

	if (!rule_type_valid(rule_type))
		return 0;

	info = &wl_rules_db->rule_info[rule_type];
	return ING_WL_OFFSET + info->off + info->sz * rule_index +
	       offsetof(union ing_wl_rule_union, rule_common.hits);
}

s32 uc_ing_whitelist_rule_hits_reset(enum ing_wl_rule_type rule_type,
				     unsigned int rule_index)
{
	u64 addr;
	u32 cid;
	u32 rule_hits_offset;

	if (!rule_index_valid(rule_type, rule_index))
		return -EINVAL;

	rule_hits_offset = _uc_ing_calc_hits_offset(rule_type, rule_index);

	for_each_set_bit(cid, &in_cpu_act_bmap, UC_CPUS_MAX) {
		addr = DCCM_INGRESS_ADDR(cid, rule_hits_offset);
		PP_REG_WR32(addr, 0);
	}

	return 0;
}

s32 uc_ing_whitelist_rule_hits_get(enum ing_wl_rule_type rule_type,
				   unsigned int rule_index,
				   unsigned int *hits_val)
{
	u64 addr;
	u32 cid, total = 0;
	u32 rule_hits_offset;

	if (!rule_index_valid(rule_type, rule_index))
		return -EINVAL;
	if (ptr_is_null(hits_val))
		return -EINVAL;

	rule_hits_offset = _uc_ing_calc_hits_offset(rule_type, rule_index);

	for_each_set_bit(cid, &in_cpu_act_bmap, UC_CPUS_MAX) {
		addr = DCCM_INGRESS_ADDR(cid, rule_hits_offset);
		total += PP_REG_RD32(addr);
	}
	*hits_val = total;

	return 0;
}

const char *uc_wl_rule_type_name(enum ing_wl_rule_type type)
{
	if (!rule_type_valid(type))
		return NULL;

	return wl_rules_name[type];
}

static inline void rule_type_info_init(struct rule_type_info *info, u32 max,
				       size_t sz, u16 off, const char *name)
{
	info->max_rules = max;
	info->sz = sz;
	info->off = off;
	info->name = name;
}

s32 uc_gpid_group_init(void)
{
	struct rule_type_info *rules;

	wl_rules_db =
		devm_kzalloc(pp_dev_get(), sizeof(*wl_rules_db), GFP_KERNEL);
	if (!wl_rules_db) {
		pr_err("Failed to allocate white list rules db memory\n");
		return -ENOMEM;
	}

	rules = wl_rules_db->rule_info;
	/* invalid entry */
	rule_type_info_init(&rules[0], 0, 0, 0, wl_rules_name[0]);
	/* Rule IP MAC */
	rule_type_info_init(
		&rules[ING_WL_RULE_TYPE_IP_MAC],
		WL_RULE_IP_AND_MAC_MAX_RULES,
		sizeof_field(struct ing_wl_rules, rule_ip_mac[0]),
		offsetof(struct ing_wl_rules, rule_ip_mac),
		wl_rules_name[ING_WL_RULE_TYPE_IP_MAC]);

	/* Rule MAC */
	rule_type_info_init(
		&rules[ING_WL_RULE_TYPE_MAC],
		WL_RULE_MAC_MAX_RULES,
		sizeof_field(struct ing_wl_rules, rule_mac[0]),
		offsetof(struct ing_wl_rules, rule_mac),
		wl_rules_name[ING_WL_RULE_TYPE_MAC]);

	/* Rule ETH TYPE */
	rule_type_info_init(
		&rules[ING_WL_RULE_TYPE_ETH_TYPE],
		WL_RULE_ETH_MAX_RULES,
		sizeof_field(struct ing_wl_rules, rule_eth[0]),
		offsetof(struct ing_wl_rules, rule_eth),
		wl_rules_name[ING_WL_RULE_TYPE_ETH_TYPE]);

	/* Rule Source Dest Ports */
	rule_type_info_init(
		&rules[ING_WL_RULE_TYPE_IP_PROTO_PORTS],
		WL_RULE_IP_PROTO_PORTS_MAX_RULES,
		sizeof_field(struct ing_wl_rules, rule_ip_proto_ports[0]),
		offsetof(struct ing_wl_rules, rule_ip_proto_ports),
		wl_rules_name[ING_WL_RULE_TYPE_IP_PROTO_PORTS]);

	/* Rule Source Dest Ports */
	rule_type_info_init(
		&rules[ING_WL_RULE_TYPE_ICMP],
		WL_RULE_ICMP_MAX_RULES,
		sizeof_field(struct ing_wl_rules, rule_icmp[0]),
		offsetof(struct ing_wl_rules, rule_icmp),
		wl_rules_name[ING_WL_RULE_TYPE_ICMP]);

	/* Rule Source or Dest IP and UDP Ports */
	rule_type_info_init(
		&rules[ING_WL_RULE_TYPE_IPV4_SRC_OR_DST_AND_L4_PORTS],
		WL_RULE_IPV4_SRC_OR_DST_AND_L4_PORTS_MAX_RULES,
		sizeof_field(struct ing_wl_rules, rule_ipv4_ports[0]),
		offsetof(struct ing_wl_rules, rule_ipv4_ports),
		wl_rules_name[ING_WL_RULE_TYPE_IPV4_SRC_OR_DST_AND_L4_PORTS]);

	/* Rule Source and Dest IP and UDP Ports */
	rule_type_info_init(
		&rules[ING_WL_RULE_TYPE_IPV4_SRC_AND_DST_AND_L4_PORTS],
		WL_RULE_IPV4_SRC_AND_DST_AND_L4_PORTS_MAX_RULES,
		sizeof_field(struct ing_wl_rules, rule_double_ipv4_ports[0]),
		offsetof(struct ing_wl_rules, rule_double_ipv4_ports),
		wl_rules_name[ING_WL_RULE_TYPE_IPV4_SRC_AND_DST_AND_L4_PORTS]);

	/* Rule Source or Dest IPv6 and UDP Ports */
	rule_type_info_init(
		&rules[ING_WL_RULE_TYPE_IPV6_SRC_OR_DST_AND_L4_PORTS],
		WL_RULE_IPV6_SRC_OR_DST_AND_L4_PORTS_MAX_RULES,
		sizeof_field(struct ing_wl_rules, rule_ipv6_ports[0]),
		offsetof(struct ing_wl_rules, rule_ipv6_ports),
		wl_rules_name[ING_WL_RULE_TYPE_IPV6_SRC_OR_DST_AND_L4_PORTS]);

	return 0;
}
