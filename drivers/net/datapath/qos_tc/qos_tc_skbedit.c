// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2023 MaxLinear, Inc.
 *
 *****************************************************************************/

#include <linux/list.h>
#include <linux/version.h>
#include <net/tc_act/tc_skbedit.h>
#include <net/tc_act/tc_vlan.h>
#include <net/datapath_api.h>
#include <net/flow_dissector.h>
#include "qos_tc_flower.h"
#include "qos_tc_vlan_prepare.h"
#include "qos_tc_trace.h"

/* skbedit reassignment action info */
struct skbedit {
	int proto;
	u32 pref;
	enum tc_flower_vlan_tag tag;
	int prio;
	int tc;
};

/* vlan action info */
struct vlan_act {
	u8 action;                      /* action: PUSH or POP */
	struct dp_pattern_vlan outer;	/*outer VLAN match pattern */
	struct dp_pattern_vlan inner;	/*inner VLAN match pattern */
	struct dp_act_vlan act;
};

struct replic_key {
	u64 hi;
	u64 lo;
};

struct flower_skbedit_rule {
	struct skbedit skbedit_act;
	struct vlan_act vlan;
	struct list_head list;
};

/* flower storage */
struct flower_cls_skbedit {
	struct net_device *dev;
	struct replic_key cookie;	/* cookie used to replicate rules */
	struct net_device *rep_dev;	/* netdev to replicate from */
	bool ingress;
	struct list_head rules;
	int rule_cnt;
	struct list_head list;
};

static LIST_HEAD(tc_skbedit_list);

#define qos_tc_vlan_free_rules(l, r)				\
	({							\
		typeof (*r) *p, *n;				\
		list_for_each_entry_safe(p, n, l, list) {	\
			list_del(&p->list);			\
			kfree(p);				\
		}						\
		kfree(l);					\
	})

static bool cookie_is_eq(const struct replic_key *a,
			 const struct replic_key *b)
{
	return a->hi == b->hi && a->lo == b->lo;
}

static int qos_tc_get_ctp_conf(struct net_device *dev,
			       struct core_ops *ops,
			       GSW_CTP_portConfig_t *ctp)
{
	dp_subif_t *dp_subif;
	struct ctp_ops *cops = &ops->gsw_ctp_ops;
	int ret = 0;

	if (!dev || !ops || !ctp)
		return -EINVAL;

	memset(ctp, 0, sizeof(*ctp));

	dp_subif = kzalloc(sizeof(*dp_subif), GFP_KERNEL);
	if (!dp_subif) {
		netdev_err(dev, "%s: failed to allocate memory for subif\n",
			   __func__);
		return -ENOMEM;
	}
	ret = dp_get_netif_subifid(dev, NULL, NULL, NULL, dp_subif, 0);
	if (ret != DP_SUCCESS) {
		kfree(dp_subif);
		return -ENODEV;
	}

	ctp->nLogicalPortId = dp_subif->port_id;
	ctp->nSubIfIdGroup = dp_subif->subif_groupid;
	kfree(dp_subif);

	ctp->eMask |= GSW_CTP_PORT_CONFIG_MASK_EGRESS_VLAN;
	ctp->eMask |= GSW_CTP_PORT_CONFIG_MASK_EGRESS_VLAN_IGMP;
	ret = cops->CTP_PortConfigGet(ops, ctp);
	if (ret != GSW_statusOk) {
		netdev_err(dev, "%s: failed: %d\n", __func__, ret);
		return -EIO;
	}

	return 0;
}

static int qos_tc_ctp_assign_blk(struct net_device *dev, int blk)
{
	struct core_ops *gswops;
	GSW_CTP_portConfig_t ctp = {0}, ctp2 = {0};
	int ret;

	gswops = gsw_get_swcore_ops(0);
	if (!gswops)
		return -EINVAL;

	ret = qos_tc_get_ctp_conf(dev, gswops, &ctp);
	if (ret < 0)
		return ret;

	ctp2.nLogicalPortId = ctp.nLogicalPortId;
	ctp2.nSubIfIdGroup = ctp.nSubIfIdGroup;

	ctp2.eMask |= GSW_CTP_PORT_CONFIG_MASK_EGRESS_VLAN;
	ctp2.eMask |= GSW_CTP_PORT_CONFIG_MASK_EGRESS_VLAN_IGMP;
	if (blk >= 0) {
		ctp2.bEgressExtendedVlanEnable = 1;
		ctp2.bEgressExtendedVlanIgmpEnable = 1;
		ctp2.nEgressExtendedVlanBlockId = blk;
		ctp2.nEgressExtendedVlanBlockSize = 0;
		ctp2.nEgressExtendedVlanBlockIdIgmp = blk;
		ctp2.nEgressExtendedVlanBlockSizeIgmp = 0;
	} else {
		ctp2.bEgressExtendedVlanEnable = 0;
		ctp2.bEgressExtendedVlanIgmpEnable = 0;
		ctp2.nEgressExtendedVlanBlockId = 0;
		ctp2.nEgressExtendedVlanBlockSize = 0;
		ctp2.nEgressExtendedVlanBlockIdIgmp = 0;
		ctp2.nEgressExtendedVlanBlockSizeIgmp = 0;
	}
	ret = gswops->gsw_ctp_ops.CTP_PortConfigSet(gswops, &ctp2);
	if (ret != GSW_statusOk) {
		netdev_err(dev, "%s: failed: %d\n", __func__, ret);
		return -EIO;
	}

	return 0;
}

static int qos_tc_evblk_ctpdev_set(struct net_device *dev, int nblk)
{
	int ret = 0;

		ret = qos_tc_ctp_assign_blk(dev, nblk);
		if (ret < 0)
			netdev_err(dev, "%s:evblk assign failed\n",
				   __func__);

	return ret;
}

#define QOS_TC_EVBLK_INV	-1

static int qos_tc_get_ev_blk(struct net_device *dev)
{
	struct core_ops *gswops;
	GSW_CTP_portConfig_t ctp = {0};
	int blk, blk_igmp, ret = 0;

	gswops = gsw_get_swcore_ops(0);
	if (!gswops)
		return -EINVAL;

	ret = qos_tc_get_ctp_conf(dev, gswops, &ctp);
	if (ret < 0)
		return ret;

	blk = ctp.nEgressExtendedVlanBlockId;
	blk_igmp = ctp.nEgressExtendedVlanBlockIdIgmp;
	if (ctp.bEgressExtendedVlanIgmpEnable &&
	    ctp.bEgressExtendedVlanEnable && blk == blk_igmp)
		return blk;

	return QOS_TC_EVBLK_INV;
}

static void qos_tc_pattern_init(struct dp_pattern_vlan *p)
{
	if (!p)
		return;

	p->vid = DP_VLAN_PATTERN_NOT_CARE;
	p->tpid = DP_VLAN_PATTERN_NOT_CARE;
	p->dei = DP_VLAN_PATTERN_NOT_CARE;
	p->proto = DP_VLAN_PATTERN_NOT_CARE;
	p->prio = DP_VLAN_PATTERN_NOT_CARE;
}

static struct flower_cls_skbedit
*qos_tc_skbedit_storage_get(struct net_device *dev,
			    bool ingress, struct replic_key *cookie,
			    struct list_head *head)
{
	struct flower_cls_skbedit *p;

	list_for_each_entry(p, head, list) {
		if (dev == p->dev &&
		    cookie_is_eq(cookie, &p->cookie) &&
		    ingress == p->ingress) {
			netdev_dbg(dev, "skbedit storage found: %p\n", p);
			return p;
		}
	}

	netdev_dbg(dev, "skbedit storage not found\n");
	return NULL;
}

static struct flower_cls_skbedit
*qos_tc_skbedit_match_get(struct net_device *dev,
			    bool ingress, struct replic_key *cookie,
			    struct list_head *head)
{
	struct flower_cls_skbedit *p;

	list_for_each_entry(p, head, list) {
		if (dev != p->dev &&
		    !p->rep_dev &&
		    cookie_is_eq(cookie, &p->cookie) &&
		    ingress == p->ingress) {
			netdev_dbg(dev, "skbedit match found: %p\n", p);
			return p;
		}
	}

	netdev_dbg(dev, "skbedit match not found\n");
	return NULL;
}

static struct flower_skbedit_rule
*qos_tc_skbedit_rule_get(struct flower_cls_skbedit *skbedit,
			 struct flower_skbedit_rule *rule)
{
	struct flower_skbedit_rule *p, *tmp;
	struct list_head *head = &skbedit->rules;

	list_for_each_entry_safe(p, tmp, head, list) {
		if (memcmp(&rule->skbedit_act, &p->skbedit_act,
			   sizeof(rule->skbedit_act)) == 0 &&
		    memcmp(&rule->vlan, &p->vlan,
			   sizeof(rule->vlan)) == 0) {
			netdev_dbg(skbedit->dev, "skbedit rule found: %p\n", p);
			return p;
		}
	}

	netdev_dbg(skbedit->dev, "skbedit rule not found\n");
	return NULL;
}

static int qos_tc_skbedit_rule_del(struct flower_cls_skbedit *skbedit,
				   struct flower_skbedit_rule *rule)
{
	struct flower_skbedit_rule *p;

	p = qos_tc_skbedit_rule_get(skbedit, rule);
	if (p) {
		netdev_dbg(skbedit->dev, "%s skbedit rule deleted %p\n",
			   __func__, p);
		list_del(&p->list);
		kfree(p);
		skbedit->rule_cnt--;
		return 0;
	}

	netdev_dbg(skbedit->dev, "%s skbedit rule not found\n", __func__);
	return -EINVAL;
}

static void qos_tc_skbedit_rule_add(struct flower_cls_skbedit *skbedit,
				    struct flower_skbedit_rule *rule)
{
	list_add_tail(&rule->list, &skbedit->rules);
	skbedit->rule_cnt++;
}

int qos_tc_skbedit_storage_crt(struct net_device *dev, bool ingress,
			       struct replic_key *cookie,
			       struct flower_cls_skbedit **node)
{
	*node = kzalloc(sizeof(**node), GFP_KERNEL);
	if (!*node) {
		netdev_err(dev, "%s: Out of memory!\n", __func__);
		return -ENOMEM;
	}

	(*node)->dev = dev;
	(*node)->ingress = ingress;
	memcpy(&((*node)->cookie), cookie, sizeof(*cookie));
	INIT_LIST_HEAD(&(*node)->rules);

	netdev_dbg(dev, "skbedit storage created: %p\n", *node);
	return 0;
}

void qos_tc_skbedit_storage_del(struct net_device *dev,
				struct flower_cls_skbedit *node)
{
	if (node->rule_cnt > 0)
		return;

	if (!list_empty(&node->rules))
		netdev_warn(dev, "%s: rules list not empty!\n", __func__);

	netdev_dbg(dev, "skbedit storage deleted: %p\n", node);
	list_del(&node->list);
	kfree(node);
}

int qos_tc_skbedit_action_check(struct flow_cls_offload *f,
				int *tc, u8 *vlan_act,
				struct replic_key *cookie)
{
	const struct flow_action_entry *a;
	int i;
	int act_nr = 0;
	int act_ok_nr = 0;

	if (f->rule->action.num_entries == 0)
		return -EINVAL;

	flow_action_for_each(i, a, &f->rule->action) {
		if (a->id == FLOW_ACTION_PRIORITY) {
			act_ok_nr++;
			if (tc)
				*tc = a->priority;

			if (a->cookie) {
				u8 *p = a->cookie->cookie;
				u32 len = a->cookie->cookie_len;

				if (len < 16) {
					pr_warn("%s: cookie too short!\n",
						__func__);
					continue;
				}

				cookie->hi = get_unaligned_be64(p);
				cookie->lo = get_unaligned_be64(p + 8);
			}
		}

		if (a->id == FLOW_ACTION_VLAN_POP ||
		    a->id == FLOW_ACTION_VLAN_PUSH) {
			act_ok_nr++;
			*vlan_act = (a->id == FLOW_ACTION_VLAN_PUSH ?
				TCA_VLAN_ACT_PUSH : TCA_VLAN_ACT_POP);
		}

		act_nr++;
	}

	if (act_nr > 2 || act_ok_nr > 2)
		return -EINVAL;

	return 0;
}

static void qos_tc_skbedit_parse_stag_vlan(struct flow_cls_offload *f,
					   struct flower_skbedit_rule **rule)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_vlan *key =
		skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_VLAN,
					  qos_tc_get_key(f));
	struct flow_dissector_key_vlan *mask =
		skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_VLAN,
					  qos_tc_get_mask(f));
	if (mask && key) {
		if (mask->vlan_id == 0xfff)
			(*rule)->vlan.outer.vid = key->vlan_id;
		if (mask->vlan_priority == 0x7) {
			(*rule)->vlan.outer.prio = key->vlan_priority;
			(*rule)->skbedit_act.prio = key->vlan_priority;
		}
		(*rule)->vlan.outer.tpid = ntohs(key->vlan_tpid);
	}
}

static void qos_tc_skbedit_parse_dtag_vlan(struct flow_cls_offload *f,
					   struct flower_skbedit_rule **rule)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_vlan *key =
		skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_CVLAN,
					  qos_tc_get_key(f));
	struct flow_dissector_key_vlan *mask =
		skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_CVLAN,
					  qos_tc_get_mask(f));

	if (mask && key) {
		if (mask->vlan_id == 0xfff)
			(*rule)->vlan.inner.vid = key->vlan_id;
		if (mask->vlan_priority == 0x7) {
			(*rule)->vlan.inner.prio = key->vlan_priority;
			(*rule)->skbedit_act.prio = key->vlan_priority;
		}
		(*rule)->vlan.inner.tpid = ntohs(key->vlan_tpid);
	}
}

static void qos_tc_skbedit_parse_basic(struct flow_cls_offload *f,
				       struct flower_skbedit_rule **rule)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_basic *key =
		skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_BASIC,
					  qos_tc_get_key(f));
	struct flow_dissector_key_basic *mask =
		skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_BASIC,
					  qos_tc_get_mask(f));

	if (mask->n_proto)
		(*rule)->skbedit_act.proto = ntohs(key->n_proto);
	else
		(*rule)->skbedit_act.proto = DP_VLAN_PATTERN_NOT_CARE;
}

static int qos_tc_parse_skbedit(struct flow_cls_offload *f,
				struct flower_cls_skbedit *skbedit,
				struct flower_skbedit_rule **rule,
				enum tc_flower_vlan_tag tag,
				int tc, u8 vlan_act, bool ingress)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);

	if (d->used_keys &
			~(BIT(FLOW_DISSECTOR_KEY_CONTROL) |
			  BIT(FLOW_DISSECTOR_KEY_BASIC) |
			  BIT(FLOW_DISSECTOR_KEY_META) |
			  BIT(FLOW_DISSECTOR_KEY_VLAN) |
			  BIT(FLOW_DISSECTOR_KEY_CVLAN))) {
		pr_debug("%s: Unsupported key used: 0x%x\n", __func__,
			 d->used_keys);
		return -EOPNOTSUPP;
	}

	*rule = kzalloc(sizeof(**rule), GFP_KERNEL);
	if (!*rule)
		return -ENOMEM;

	netdev_dbg(skbedit->dev, "skbedit rule created: %p\n", *rule);

	(*rule)->skbedit_act.pref = f->common.prio;
	qos_tc_pattern_init(&((*rule)->vlan.outer));
	qos_tc_pattern_init(&((*rule)->vlan.inner));
	(*rule)->skbedit_act.tc = tc;
	(*rule)->skbedit_act.tag = tag;
	(*rule)->vlan.action = vlan_act;

	/* Classification/Matching arguments parsing */
	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_VLAN))
		qos_tc_skbedit_parse_stag_vlan(f, rule);

	if (tag == TC_VLAN_DOUBLE_TAGGED &&
	    dissector_uses_key(d, FLOW_DISSECTOR_KEY_CVLAN))
		qos_tc_skbedit_parse_dtag_vlan(f, rule);

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_BASIC))
		qos_tc_skbedit_parse_basic(f, rule);

	return 0;
}

static int qos_tc_ev_skbedit_clr(struct net_device *dev)
{
	struct dp_tc_vlan dp_vlan = {0};
	int ret = 0;

	if (!dev)
		return -EINVAL;

	/* Remove the full block first to avoid fragmentation */
	dp_vlan.dev = dev;
	dp_vlan.def_apply = DP_VLAN_APPLY_CTP;
	dp_vlan.dir = DP_DIR_EGRESS;
	dp_vlan.n_vlan0 = 0;
	dp_vlan.n_vlan1 = 0;
	dp_vlan.n_vlan2 = 0;

	ret = dp_vlan_set(&dp_vlan, 0);
	if (ret < 0)
		netdev_err(dev, "%s dp_vlan_set failed %d\n", __func__, ret);

	return ret;
}

static int qos_tc_ev_utag_rule_add(struct flower_skbedit_rule *rule,
				   struct list_head *list)
{
	struct dp_vlan0 *r0 = NULL;

	r0 = kzalloc(sizeof(*r0), GFP_KERNEL);
	if (!r0)
		return -ENOMEM;

	qos_tc_pattern_init(&r0->outer);
	memcpy(&r0->outer, &rule->vlan.outer,
	       sizeof(r0->outer));

	if (rule->vlan.action)
		memcpy(&r0->act, &rule->vlan.act,
		       sizeof(r0->act));

	r0->act.ract.new_tc = rule->skbedit_act.tc;
	r0->act.ract.act = DP_TC_REASSIGN;

	list_add_tail(&r0->list, list);

	return 0;
}

static int qos_tc_ev_stag_rule_add(struct flower_skbedit_rule *rule,
				   struct list_head *list)
{
	struct dp_vlan1 *r1 = NULL;

	r1 = kzalloc(sizeof(*r1), GFP_KERNEL);
	if (!r1)
		return -ENOMEM;

	qos_tc_pattern_init(&r1->outer);
	memcpy(&r1->outer, &rule->vlan.outer,
	       sizeof(r1->outer));

	/* Rules will be setup for standard single or double
	 * tagged packets. Overwrite tpid in order to match
	 * here for all possible combinations.
	 */
	r1->outer.tpid = DP_VLAN_PATTERN_NOT_CARE;

	if (rule->vlan.action)
		memcpy(&r1->act, &rule->vlan.act,
		       sizeof(r1->act));

	r1->act.ract.new_tc = rule->skbedit_act.tc;
	r1->act.ract.act = DP_TC_REASSIGN;

	list_add_tail(&r1->list, list);

	return 0;
}

static int qos_tc_ev_dtag_rule_add(struct flower_skbedit_rule *rule,
				   struct list_head *list)
{
	struct dp_vlan2 *r2 = NULL;

	r2 = kzalloc(sizeof(*r2), GFP_KERNEL);
	if (!r2)
		return -ENOMEM;

	qos_tc_pattern_init(&r2->outer);
	qos_tc_pattern_init(&r2->inner);
	memcpy(&r2->outer, &rule->vlan.outer,
	       sizeof(r2->outer));
	memcpy(&r2->inner, &rule->vlan.inner,
	       sizeof(r2->inner));

	/* Rules will be setup for standard single or double
	 * tagged packets. Overwrite tpid in order to match
	 * here for all possible combinations.
	 */
	r2->outer.tpid = DP_VLAN_PATTERN_NOT_CARE;
	r2->inner.tpid = DP_VLAN_PATTERN_NOT_CARE;

	if (rule->vlan.action)
		memcpy(&r2->act, &rule->vlan.act,
		       sizeof(r2->act));

	r2->act.ract.new_tc = rule->skbedit_act.tc;
	r2->act.ract.act = DP_TC_REASSIGN;

	list_add_tail(&r2->list, list);

	return 0;
}

static int qos_tc_ev_skbedit_set(struct net_device *dev,
				 struct flower_cls_skbedit *skbedit,
				 int *nblk)
{
	struct dp_tc_vlan dp_vlan = {0};
	struct list_head *list0 = NULL;
	struct list_head *list1 = NULL;
	struct list_head *list2 = NULL;
	struct dp_vlan0 *r0 = NULL;
	struct dp_vlan1 *r1 = NULL;
	struct dp_vlan2 *r2 = NULL;
	struct flower_skbedit_rule *rule;
	unsigned int idx0 = 0, idx1 = 0, idx2 = 0;
	int ret = 0;

	if (!dev || !nblk || !skbedit)
		return -EINVAL;

	if (skbedit->rule_cnt == 0)
		return 0;

	list0 = kzalloc(sizeof(*list0), GFP_KERNEL);
	if (!list0)
		return -ENOMEM;

	list1 = kzalloc(sizeof(*list1), GFP_KERNEL);
	if (!list1) {
		kfree(list0);
		return -ENOMEM;
	}

	list2 = kzalloc(sizeof(*list2), GFP_KERNEL);
	if (!list2) {
		kfree(list0);
		kfree(list1);
		return -ENOMEM;
	}

	INIT_LIST_HEAD(list0);
	INIT_LIST_HEAD(list1);
	INIT_LIST_HEAD(list2);
	dp_vlan.vlan0_head = list0;
	dp_vlan.vlan1_head = list1;
	dp_vlan.vlan2_head = list2;

	list_for_each_entry(rule, &skbedit->rules, list) {
		if (rule->skbedit_act.tag == TC_VLAN_UNTAGGED) {
			ret = qos_tc_ev_utag_rule_add(rule, list0);
			if (ret < 0)
				goto exit_free;

			idx0++;
		}

		if (rule->skbedit_act.tag == TC_VLAN_SINGLE_TAGGED) {
			ret = qos_tc_ev_stag_rule_add(rule, list1);
			if (ret < 0)
				goto exit_free;

			idx1++;
		}

		if (rule->skbedit_act.tag == TC_VLAN_DOUBLE_TAGGED) {
			ret = qos_tc_ev_dtag_rule_add(rule, list2);
			if (ret < 0)
				goto exit_free;

			idx2++;
		}
	}

	if (!idx0 && !idx1 && !idx2)
		goto exit_free;

	dp_vlan.dev = dev;
	dp_vlan.def_apply = DP_VLAN_APPLY_CTP;
	dp_vlan.dir = DP_DIR_EGRESS;
	dp_vlan.n_vlan0 = idx0;
	dp_vlan.n_vlan1 = idx1;
	dp_vlan.n_vlan2 = idx2;

	ret = dp_vlan_set(&dp_vlan, 0);
	if (ret < 0)
		netdev_err(dev, "%s dp_vlan_set failed %d\n", __func__, ret);

exit_free:
	/* free the lists with tc reassign rules */
	qos_tc_vlan_free_rules(list0, r0);
	qos_tc_vlan_free_rules(list1, r1);
	qos_tc_vlan_free_rules(list2, r2);

	if (!ret && (idx0 || idx1 || idx2)) {
		*nblk = qos_tc_get_ev_blk(dev);
		netdev_dbg(dev, "%s block ID %d\n", __func__, *nblk);
		if (*nblk < 0) {
			netdev_err(dev, "%s failed to get block ID\n",
				   __func__);
			return -ENODEV;
		}
	}

	return ret;
}

static int qos_tc_skbedit_upd(struct net_device *dev,
			      struct flower_cls_skbedit *skbedit)
{
	int ret = 0;
	int nblk = QOS_TC_EVBLK_INV;

	/* remark: Do not remove the block assignment before below function
	 * call. The CTP allocation is freed automatically but only in case of
	 * valid block assignment.
	 */
	ret = qos_tc_ev_skbedit_clr(dev);
	if (ret < 0)
		return ret;

	ret = qos_tc_ev_skbedit_set(dev, skbedit, &nblk);
	if (ret < 0)
		return ret;

	ret = qos_tc_evblk_ctpdev_set(dev, nblk);

	return ret;
}

static int qos_tc_skbedit_replicate(struct flower_cls_skbedit *new,
				    struct flower_cls_skbedit *replicate)
{
	int ret = 0;
	int nblk = QOS_TC_EVBLK_INV;

	/* Read the block ID from the device to replicate rules from */
	nblk = qos_tc_get_ev_blk(replicate->dev);
	if (nblk < 0) {
		netdev_err(replicate->dev, "%s failed to get block ID\n",
			   __func__);
		return -ENODEV;
	}

	ret = qos_tc_evblk_ctpdev_set(new->dev, nblk);
	if (ret == 0)
		new->rep_dev = replicate->dev;

	return ret;
}

static int qos_tc_skbedit_replicate_del(struct flower_cls_skbedit *skbedit)
{
	int ret = 0;

	/* remove CTP port assignment if replicate dev is set */
	if (skbedit->rep_dev)
		ret = qos_tc_evblk_ctpdev_set(skbedit->dev, QOS_TC_EVBLK_INV);

	return ret;
}

static void qos_tc_skbedit_remove(struct net_device *dev,
				  struct flower_cls_skbedit *skbedit,
				  struct flower_skbedit_rule *rule)
{
	int ret = 0;

	ret = qos_tc_skbedit_rule_del(skbedit, rule);
	if (ret == 0) {
		if (skbedit->rep_dev)
			ret = qos_tc_skbedit_replicate_del(skbedit);
		else
			ret = qos_tc_skbedit_upd(dev, skbedit);

		if (ret < 0)
			netdev_err(dev, "%s: rule deletion failed %d\n",
				   __func__, ret);
	}

	qos_tc_skbedit_storage_del(dev, skbedit);
}

int qos_tc_skbedit_offload(struct net_device *dev,
			   struct flow_cls_offload *f,
			   bool ingress)
{
	struct flower_cls_skbedit *skbedit = NULL;
	struct flower_cls_skbedit *match = NULL;
	struct flower_skbedit_rule *rule = NULL;
	int tc = 0;
	enum tc_flower_vlan_tag tag;
	int ret = 0;
	u8 vlan_act = 0;
	struct replic_key cookie = {0};

	trace_skbedit_offload_enter(dev, NULL, -1, -1, ingress, -1, -1);

	if (ingress)
		return -EOPNOTSUPP;

	ret = qos_tc_skbedit_action_check(f, &tc, &vlan_act, &cookie);
	if (ret < 0)
		return ret;

	tag = qos_tc_vlan_tag_get(dev, f);
	if (tag == TC_VLAN_UNKNOWN)
		return -EIO;

	skbedit = qos_tc_skbedit_storage_get(dev, ingress, &cookie,
					     &tc_skbedit_list);
	if (!skbedit) {
		ret = qos_tc_skbedit_storage_crt(dev, ingress, &cookie,
						 &skbedit);
		if (ret)
			return ret;
		list_add(&skbedit->list, &tc_skbedit_list);
	}

	ret = qos_tc_parse_skbedit(f, skbedit, &rule, tag, tc, vlan_act,
				   ingress);
	if (ret < 0) {
		kfree(rule);
		goto err;
	}

	if (vlan_act) {
		ret = vlan_action_parse(skbedit->dev, f, &rule->vlan.act, tag);
		if (ret < 0) {
			kfree(rule);
			goto err;
		}
	}

	qos_tc_skbedit_rule_add(skbedit, rule);

	/* check for matching cookie to replicate rules from existing netdev */
	match = qos_tc_skbedit_match_get(dev, ingress, &cookie,
					 &tc_skbedit_list);
	if (match)
		ret = qos_tc_skbedit_replicate(skbedit, match);
	else
		ret = qos_tc_skbedit_upd(dev, skbedit);

	if (ret < 0) {
		qos_tc_skbedit_rule_del(skbedit, rule);
		netdev_err(dev, "%s: rule couldn't be applied %d\n",
			   __func__, ret);
		goto err;
	}

	ret = qos_tc_flower_storage_add(dev, f->cookie, TC_TYPE_SKBEDIT,
					skbedit, rule);
	if (ret < 0) {
		qos_tc_skbedit_remove(dev, skbedit, rule);
		return ret;
	}

	trace_skbedit_offload_exit(dev, skbedit, rule->skbedit_act.tag,
				   skbedit->rule_cnt, ingress,
				   rule->skbedit_act.prio,
				   rule->skbedit_act.tc);

	return 0;

err:
	qos_tc_skbedit_storage_del(dev, skbedit);

	return ret;
}

int qos_tc_skbedit_unoffload(struct net_device *dev,
			     void *arg1,
			     void *arg2)
{
	struct flower_cls_skbedit *skbedit;
	struct flower_skbedit_rule *rule;

	if (!arg1 || !arg2)
		return -EIO;

	skbedit = arg1;
	rule = arg2;

	qos_tc_skbedit_remove(dev, skbedit, rule);

	return 0;
}

static void print_skbedit_storage(struct seq_file *file,
				  struct flower_cls_skbedit *s)
{
	struct flower_skbedit_rule *p, *tmp;
	struct list_head *head = &s->rules;
	int i = 0;

	seq_printf(file, "dev: %s\n", s->dev->name);
	seq_printf(file, "dir: %s\n", s->ingress ? "ingress" : "egress");
	seq_printf(file, "rules: %d\n", s->rule_cnt);

	if (s->rep_dev) {
		seq_printf(file, "CTP rules replicated from dev: %s\n",
			   s->rep_dev->name);
		seq_puts(file, "\n");
		return;
	}

	list_for_each_entry_safe(p, tmp, head, list) {
		seq_printf(file, "rule: %d\n", i++);
		seq_printf(file, "\tpref: %d\n", p->skbedit_act.pref);
		seq_printf(file, "\tproto: %#x\n", p->skbedit_act.proto);
		seq_printf(file, "\ttagging: %s\n",
			   p->skbedit_act.tag == TC_VLAN_SINGLE_TAGGED ?
			   "single" :
			   (p->skbedit_act.tag == TC_VLAN_DOUBLE_TAGGED ?
			   "double" : "none"));
		seq_puts(file, "\t\tSKBEDIT reassignment act:\n");
		seq_printf(file, "\t\tprio: %d\n", p->skbedit_act.prio);
		seq_printf(file, "\t\ttc: %d\n", p->skbedit_act.tc);
		if (p->vlan.action) {
			seq_printf(file, "\t\tVLAN act: %s\n",
				   p->vlan.action == TCA_VLAN_ACT_PUSH ?
				   "push" : "pop");
			if (p->vlan.action == TCA_VLAN_ACT_PUSH) {
				seq_printf(file, "\t\tvid: %d\n",
					   p->vlan.act.vid[0]);
				seq_printf(file, "\t\tproto: %#x\n",
					   p->vlan.act.tpid[0]);
				seq_printf(file, "\t\tprio: %d\n",
					   p->vlan.act.prio[0]);
			}
		}
	}

	seq_puts(file, "\n");
}

void qos_tc_skbedit_debugfs(struct seq_file *file, void *ctx)
{
	struct flower_cls_skbedit *p, *n;

	rtnl_lock();
	list_for_each_entry_safe(p, n, &tc_skbedit_list, list) {
		print_skbedit_storage(file, p);
	}
	rtnl_unlock();
}
