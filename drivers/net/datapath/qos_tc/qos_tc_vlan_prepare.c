// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/
#include <net/pkt_cls.h>
#include <net/flow_dissector.h>
#include <net/tc_act/tc_vlan.h>
#include <net/tc_act/tc_gact.h>
#include <net/tc_act/tc_mirred.h>
#include <net/datapath_api.h>
#include <net/datapath_api_vlan.h>
#include <asm/unaligned.h>
#include <linux/version.h>
#include <uapi/linux/tc_act/tc_vlan.h>
#include "qos_tc_flower.h"
#include "qos_tc_vlan_storage.h"
#include "qos_tc_vlan_prepare.h"
#include "qos_tc_qos.h"

static bool should_use_ctp(struct net_device *dev, bool ingress)
{
	if (!ingress)
		return false;

	/* For lan ports only the 1st subif receives traffic */
	if (qos_tc_is_lan_dev(dev) && qos_tc_is_first_subif(dev))
		return true;

	if (qos_tc_is_vuni_dev(dev))
		return true;

	if (qos_tc_is_gpon_dev(dev) && !dp_is_pmapper_check(dev))
		return true;

	return false;
}

void qos_tc_dp_vlan_prepare(struct net_device *dev,
			    struct dp_tc_vlan *dp_vlan,
			    bool ingress)
{
	bool use_ctp;

	dp_vlan->dev = dev;
	dp_vlan->dir = ingress ? DP_DIR_INGRESS : DP_DIR_EGRESS;
	use_ctp = should_use_ctp(dev, ingress);
	if (!dp_is_pmapper_check(dev) && use_ctp)
		dp_vlan->def_apply = DP_VLAN_APPLY_CTP;
}

static enum tc_flower_vlan_tag
__qos_tc_vlan_tag_get(struct net_device *dev,
		      struct flow_cls_offload *f)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_CVLAN))
		return TC_VLAN_DOUBLE_TAGGED;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_VLAN))
		return TC_VLAN_SINGLE_TAGGED;

	return TC_VLAN_UNKNOWN;
}

enum tc_flower_vlan_tag
qos_tc_vlan_tag_get(struct net_device *dev,
		    struct flow_cls_offload *f)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_basic *key = NULL;
	struct flow_dissector_key_basic *mask = NULL;

	if (eth_type_vlan(f->common.protocol))
		return __qos_tc_vlan_tag_get(dev, f);

	if (ntohs(f->common.protocol) == ETH_P_ALL)
		return TC_VLAN_UNTAGGED;

	if (ntohs(f->common.protocol == ETH_P_PPP_SES))
		return TC_VLAN_UNTAGGED;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_BASIC)) {
		key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_BASIC,
						qos_tc_get_key(f));
		mask = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_BASIC,
						 qos_tc_get_mask(f));

		if (mask->n_proto && !eth_type_vlan(key->n_proto)) {
			netdev_dbg(dev, "%s: 0-tag rule\n", __func__);
			return TC_VLAN_UNTAGGED;
		}
	}

	netdev_dbg(dev, "%s: unknown tag\n", __func__);
	return TC_VLAN_UNKNOWN;
}

static int get_pppoe(struct net_device *dev, struct flow_cls_offload *f)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_pppoe *key = NULL;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_PPPOE)) {
		key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_PPPOE,
						qos_tc_get_key(f));

		netdev_dbg(dev, "%s:etype %#x\n",
			   __func__, ntohs(key->type));
		return ntohs(key->type);
	}

	return DP_VLAN_PATTERN_NOT_CARE;
}

static int qos_tc_get_ethtype(struct net_device *dev,
			      struct flow_cls_offload *f)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_basic *key = NULL;
	struct flow_dissector_key_basic *mask = NULL;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_BASIC)) {
		key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_BASIC,
						qos_tc_get_key(f));
		mask = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_BASIC,
						 qos_tc_get_mask(f));

		if (mask->n_proto) {
			netdev_dbg(dev, "%s:etype %#x\n",
				   __func__, ntohs(key->n_proto));
			return ntohs(key->n_proto);
		}
	}

	return DP_VLAN_PATTERN_NOT_CARE;
}

static void vlan_pattern_default_set(struct dp_pattern_vlan *patt)
{
	patt->proto = DP_VLAN_PATTERN_NOT_CARE;
	patt->prio = DP_VLAN_PATTERN_NOT_CARE;
	patt->vid = DP_VLAN_PATTERN_NOT_CARE;
	patt->tpid = DP_VLAN_PATTERN_NOT_CARE;
	patt->dei = DP_VLAN_PATTERN_NOT_CARE;
}

static void vlan_action_default_set(struct dp_act_vlan *act)
{
	act->act = 0;
	act->pop_n = 0;
	act->push_n = 0;
}

static void flt_set_dei(const u8 *cookie, u32 cookie_len, int *dei)
{
	u16 tci_val;
	u16 tci_mask;

	if (!dei || !cookie || cookie_len < 4)
		return;

	tci_val = get_unaligned_be16(cookie);
	tci_mask = get_unaligned_be16(&cookie[2]);
	if (tci_mask & VLAN_CFI_MASK) {
		*dei = (tci_val & VLAN_CFI_MASK) ? 1 : 0;
	} else {
		*dei = DP_VLAN_PATTERN_NOT_CARE;
	}
}

static bool fwd_drop_action_parse(struct net_device *dev,
				  struct flow_cls_offload *f,
				  struct dp_act_vlan *act,
				  int *dei)
{
	const struct flow_action_entry *a;
	int i;

	flow_action_for_each(i, a, &f->rule->action) {
		if (dei && a->cookie)
			flt_set_dei(a->cookie->cookie, a->cookie->cookie_len,
				    dei);

		/* DROP and PASS make no sense together */
		if (a->id == FLOW_ACTION_DROP) {
			act->act = DP_VLAN_ACT_DROP;
			netdev_dbg(dev, "DROP action detected, act: %x\n",
				   act->act);
			return true;
		}
		if (a->id == FLOW_ACTION_ACCEPT) {
			act->act = DP_VLAN_ACT_FWD;
			netdev_dbg(dev, "PASS action detected, act: %x\n",
				   act->act);
			return true;
		}
	}

	return false;
}

static void qos_tc_vlan_bp_reassign(struct net_device *dev,
				    struct dp_act_vlan *act,
				    struct net_device *bp_dev)
{
	if (bp_dev) {
		act->ract.act = DP_BP_REASSIGN;
		act->ract.bp_dev = bp_dev;
		netdev_dbg(dev, "Bridge port device for reassign action: %s\n",
			   bp_dev->name);
	}
}

static int __vlan_parse(struct net_device *dev,
			struct flow_cls_offload *f,
			struct dp_pattern_vlan *patt,
			enum flow_dissector_key_id key_id)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_vlan *key;
	struct flow_dissector_key_vlan *mask;

	if (!dissector_uses_key(d, key_id))
		return -EIO;

	key = skb_flow_dissector_target(d, key_id, qos_tc_get_key(f));
	mask = skb_flow_dissector_target(d, key_id, qos_tc_get_mask(f));

	if (mask && key) {
		if (mask->vlan_id == 0xfff)
			patt->vid = key->vlan_id;
		if (mask->vlan_priority == 0x7)
			patt->prio = key->vlan_priority;
		patt->tpid = ntohs(key->vlan_tpid);
	}

	netdev_dbg(dev, "VLAN parsed, vid: %d, tpid: %x, prio: %d\n",
		   patt->vid, patt->tpid, patt->prio);

	return 0;
}

static int vlan_parse(struct net_device *dev,
		      struct flow_cls_offload *f,
		      struct dp_pattern_vlan *patt)
{
	return __vlan_parse(dev, f, patt, FLOW_DISSECTOR_KEY_VLAN);
}

static int cvlan_parse(struct net_device *dev,
		       struct flow_cls_offload *f,
		       struct dp_pattern_vlan *patt)
{
	return __vlan_parse(dev, f, patt, FLOW_DISSECTOR_KEY_CVLAN);
}

static void cookie_parse(struct net_device *dev,
			 struct flow_cls_offload *f,
			 struct dp_act_vlan *act)
{
	const struct flow_action_entry *a;
	int i;

	flow_action_for_each(i, a, &f->rule->action) {
		if (a->cookie) {
			u8 *p = a->cookie->cookie;
			u32 len = a->cookie->cookie_len;
			u64 hi = 0;
			u64 lo = 0;

			if (len < 16) {
				netdev_warn(dev, "%s: cookie is too short!\n",
					    __func__);
				continue;
			}
			hi = get_unaligned_be64(p);
			lo = get_unaligned_be64(&p[8]);

			act->sort_key = hi & FILTER_SORT_MASK;
			act->merge_key.hi = hi;
			act->merge_key.lo = lo;

			netdev_dbg(dev, "Cookie parsed: %llx\n",
				   act->sort_key);
			return;
		}
	}
}

#define IP_TOS_TO_DSCP 0xFC
static void dscp_parse(struct net_device *dev,
		       struct flow_cls_offload *f,
		       struct dp_act_vlan *act,
		       u8 vlan_prio)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_ip *key;
	u32 dscp_value;

	if (!dissector_uses_key(d, FLOW_DISSECTOR_KEY_IP))
		return;

	key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_IP,
					qos_tc_get_key(f));

	dscp_value = (key->tos & IP_TOS_TO_DSCP) >> 2;
	act->dscp_pcp_map[dscp_value] = vlan_prio;
	act->dscp_pcp_map_valid[dscp_value] = true;
	act->prio[act->push_n] = DERIVE_FROM_DSCP;
	netdev_dbg(dev, "DSCP detected, value: %u, prio: %u\n",
		   dscp_value, act->dscp_pcp_map[dscp_value]);
}

static int vlan_action_source(enum tc_flower_vlan_tag tag)
{
	if (tag == TC_VLAN_SINGLE_TAGGED)
		return CP_FROM_INNER;
	else if (tag == TC_VLAN_DOUBLE_TAGGED)
		return CP_FROM_OUTER;
	return 0;
}

int vlan_action_parse(struct net_device *dev,
		      struct flow_cls_offload *f,
		      struct dp_act_vlan *act,
		      enum tc_flower_vlan_tag tag)
{
	const struct flow_action_entry *a;
	int i;

	if (f->rule->action.num_entries == 0) {
		netdev_dbg(dev, "TC no actions!\n");
		return -EIO;
	}
	netdev_dbg(dev, "TC actions count: %d\n",
		   f->rule->action.num_entries);

	flow_action_for_each(i, a, &f->rule->action) {
		u8 flags;

		if (!(a->id == FLOW_ACTION_VLAN_POP ||
		      a->id == FLOW_ACTION_VLAN_PUSH ||
		      a->id == FLOW_ACTION_VLAN_MANGLE))
			continue;

		flags = a->vlan.flags;

		if (a->id == FLOW_ACTION_VLAN_POP) {
			act->act |= DP_VLAN_ACT_POP;
			act->pop_n++;
			netdev_dbg(dev, "POP action detected, pop_n: %d, act: %x\n",
				   act->pop_n, act->act);
			continue;
		}

		if (act->push_n >= DP_VLAN_NUM) {
			netdev_warn(dev, "%s: push action overlod\n", __func__);
			continue;
		}

		if (a->id == FLOW_ACTION_VLAN_PUSH) {
			act->act |= DP_VLAN_ACT_PUSH;

			act->prio[act->push_n] = a->vlan.prio;
			act->tpid[act->push_n] = ntohs(a->vlan.proto);
			netdev_dbg(dev, "PUSH action detected, push_n: %d, act: %x\n",
				   act->push_n + 1, act->act);
		} else if (a->id == FLOW_ACTION_VLAN_MANGLE) {
			act->act = DP_VLAN_ACT_POP | DP_VLAN_ACT_PUSH;
			act->pop_n++;

			if (flags & ACTVLAN_PUSH_F_PRIO)
				act->prio[act->push_n] = a->vlan.prio;
			else
				act->prio[act->push_n] = vlan_action_source(tag);

			if (flags & ACTVLAN_PUSH_F_PROTO)
				act->tpid[act->push_n] = ntohs(a->vlan.proto);
			else
				act->tpid[act->push_n] = vlan_action_source(tag);
			netdev_dbg(dev, "MODIFY action detected, push_n: %d, pop_n: %d, act: %x\n",
				   act->push_n + 1, act->pop_n, act->act);
		}

		if (flags & ACTVLAN_PUSH_F_ID)
			act->vid[act->push_n] = a->vlan.vid;
		else
			act->vid[act->push_n] = vlan_action_source(tag);
		act->dei[act->push_n] = 0;

		netdev_dbg(dev, "PUSH action parsed, vid: %d, tpid: %x, prio: %d, dei: %d\n",
			   act->vid[act->push_n],
			   act->tpid[act->push_n],
			   act->prio[act->push_n],
			   act->dei[act->push_n]);

		dscp_parse(dev, f, act, a->vlan.prio);

		act->push_n++;
	}

	return 0;
}

static void default_prio_parse(struct net_device *dev,
			       struct flow_cls_offload *f,
			       int *prio)
{
	*prio = f->common.prio;
	netdev_dbg(dev, "Default prio: %d\n", *prio);
}

int qos_tc_vlan_untagged_prepare(struct net_device *dev,
				 struct flow_cls_offload *f,
				 struct net_device *bp_dev,
				 struct dp_vlan0 *rule)
{
	u32 proto = ntohs(f->common.protocol);
	int err;

	vlan_pattern_default_set(&rule->outer);
	vlan_action_default_set(&rule->act);

	if (proto != ETH_P_ALL)
		rule->outer.proto = proto;

	if (!fwd_drop_action_parse(dev, f, &rule->act, NULL)) {
		qos_tc_vlan_bp_reassign(dev, &rule->act, bp_dev);

		err = vlan_action_parse(dev, f, &rule->act,
					TC_VLAN_UNTAGGED);
		if (err)
			return err;
	}

	cookie_parse(dev, f, &rule->act);
	default_prio_parse(dev, f, &rule->prio);

	return 0;
}

int qos_tc_vlan_untagged_flt_prepare(struct net_device *dev,
				     struct flow_cls_offload *f,
				     struct dp_tc_vlan *dp_vlan,
				     struct dp_vlan0 *rule)
{
	u32 proto = ntohs(f->common.protocol);
	int err;

	vlan_pattern_default_set(&rule->outer);
	vlan_action_default_set(&rule->act);

	if (proto != ETH_P_ALL)
		rule->outer.proto = proto;

	if (!fwd_drop_action_parse(dev, f, &rule->act, NULL))
		return -EINVAL;

	err = vlan_action_parse(dev, f, &rule->act, TC_VLAN_UNTAGGED);
	if (err)
		return err;

	default_prio_parse(dev, f, &rule->prio);

	return 0;
}

int qos_tc_vlan_single_tagged_prepare(struct net_device *dev,
				      struct flow_cls_offload *f,
				      struct net_device *bp_dev,
				      struct dp_vlan1 *rule)
{
	int err;

	vlan_pattern_default_set(&rule->outer);
	vlan_action_default_set(&rule->act);

	qos_tc_vlan_bp_reassign(dev, &rule->act, bp_dev);

	fwd_drop_action_parse(dev, f, &rule->act, NULL);

	err = vlan_parse(dev, f, &rule->outer);
	if (err)
		return err;

	err = vlan_action_parse(dev, f, &rule->act,
				TC_VLAN_SINGLE_TAGGED);
	if (err)
		return err;

	rule->outer.proto = get_pppoe(dev, f);
	if (rule->outer.proto == DP_VLAN_PATTERN_NOT_CARE)
		rule->outer.proto = qos_tc_get_ethtype(dev, f);

	cookie_parse(dev, f, &rule->act);
	default_prio_parse(dev, f, &rule->prio);

	return 0;
}

int qos_tc_vlan_single_tagged_flt_prepare(struct net_device *dev,
					  struct flow_cls_offload *f,
					  struct dp_vlan1 *rule)
{
	int err, dei = DP_VLAN_PATTERN_NOT_CARE;

	vlan_pattern_default_set(&rule->outer);
	vlan_action_default_set(&rule->act);

	if (!fwd_drop_action_parse(dev, f, &rule->act, &dei))
		return -EINVAL;

	err = vlan_parse(dev, f, &rule->outer);
	if (err)
		return err;

	rule->outer.dei = dei;
	rule->outer.tpid = DP_VLAN_PATTERN_NOT_CARE;

	default_prio_parse(dev, f, &rule->prio);

	return 0;
}

int qos_tc_vlan_double_tagged_prepare(struct net_device *dev,
				      struct flow_cls_offload *f,
				      struct net_device *bp_dev,
				      struct dp_vlan2 *rule)
{
	int err;

	vlan_pattern_default_set(&rule->outer);
	vlan_pattern_default_set(&rule->inner);
	vlan_action_default_set(&rule->act);

	qos_tc_vlan_bp_reassign(dev, &rule->act, bp_dev);

	fwd_drop_action_parse(dev, f, &rule->act, NULL);

	err = vlan_parse(dev, f, &rule->outer);
	if (err)
		return err;

	err = cvlan_parse(dev, f, &rule->inner);
	if (err)
		return err;

	err = vlan_action_parse(dev, f, &rule->act,
				TC_VLAN_DOUBLE_TAGGED);
	if (err)
		return err;

	rule->outer.proto = get_pppoe(dev, f);
	if (rule->outer.proto == DP_VLAN_PATTERN_NOT_CARE)
		rule->outer.proto = qos_tc_get_ethtype(dev, f);

	cookie_parse(dev, f, &rule->act);
	default_prio_parse(dev, f, &rule->prio);

	return 0;
}

int qos_tc_vlan_double_tagged_flt_prepare(struct net_device *dev,
					  struct flow_cls_offload *f,
					  struct dp_vlan2 *rule)
{
	int err, dei = DP_VLAN_PATTERN_NOT_CARE;


	vlan_pattern_default_set(&rule->outer);
	vlan_pattern_default_set(&rule->inner);
	vlan_action_default_set(&rule->act);

	if (!fwd_drop_action_parse(dev, f, &rule->act, &dei))
		return -EINVAL;

	err = vlan_parse(dev, f, &rule->outer);
	if (err)
		return err;

	rule->outer.dei = dei;
	rule->outer.tpid = DP_VLAN_PATTERN_NOT_CARE;

	default_prio_parse(dev, f, &rule->prio);

	return 0;
}
