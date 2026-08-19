// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2021 - 2023 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/
#include <linux/list.h>
#include <linux/list_sort.h>
#include <linux/netdevice.h>
#include <linux/version.h>
#include <net/pkt_cls.h>
#include <net/flow_dissector.h>
#include <net/tc_act/tc_vlan.h>
#include <net/tc_act/tc_gact.h>
#include <net/tc_act/tc_mirred.h>
#include <net/datapath_api.h>
#include <net/datapath_api_vlan.h>
#include <uapi/linux/tc_act/tc_vlan.h>
#include "qos_tc_flower.h"
#include "qos_tc_vlan_prepare.h"
#include "qos_tc_vlan_storage.h"
#include "qos_tc_vlan_filter.h"
#include "qos_tc_ext_vlan.h"
#include "qos_tc_trace.h"

#define QOS_TC_FILTER_DEF_PRIO_THRESHOLD 64000

static LIST_HEAD(tc_vlan_filter_storage);

static int untagged_add(struct net_device *dev,
			struct flow_cls_offload *f,
			struct qos_tc_vlan_storage_node *node,
			struct dp_tc_vlan *dp_vlan)
{
	struct dp_vlan0 *rule;
	int err = 0;

	rule = kzalloc(sizeof(*rule), GFP_KERNEL);
	if (!rule)
		return -ENOMEM;

	err = qos_tc_vlan_untagged_flt_prepare(dev, f, dp_vlan, rule);
	if (err) {
		kfree(rule);
		return err;
	}

	err = qos_tc_flower_storage_add(dev, f->cookie, TC_TYPE_VLAN_FILTER,
					node, rule);
	if (err < 0) {
		kfree(rule);
		return err;
	}

	if (rule->prio > QOS_TC_FILTER_DEF_PRIO_THRESHOLD)
		rule->def = DP_VLAN_DEF_RULE;

	list_add(&rule->list, &node->rules);

	node->rule_cnt++;

	return 0;
}

static int single_tagged_add(struct net_device *dev,
			     struct flow_cls_offload *f,
			     struct qos_tc_vlan_storage_node *node)
{
	struct dp_vlan1 *rule;
	int err;

	rule = kzalloc(sizeof(*rule), GFP_KERNEL);
	if (!rule)
		return -ENOMEM;

	err = qos_tc_vlan_single_tagged_flt_prepare(dev, f, rule);
	if (err) {
		kfree(rule);
		return err;
	}

	err = qos_tc_flower_storage_add(dev, f->cookie, TC_TYPE_VLAN_FILTER,
					node, rule);
	if (err < 0) {
		kfree(rule);
		return err;
	}

	if (rule->prio > QOS_TC_FILTER_DEF_PRIO_THRESHOLD)
		rule->def = DP_VLAN_DEF_RULE;

	list_add(&rule->list, &node->rules);

	node->rule_cnt++;

	return 0;
}

static int double_tagged_add(struct net_device *dev,
			     struct flow_cls_offload *f,
			     struct qos_tc_vlan_storage_node *node)
{
	struct dp_vlan2 *rule;
	int err;

	rule = kzalloc(sizeof(*rule), GFP_KERNEL);
	if (!rule)
		return -ENOMEM;

	err = qos_tc_vlan_double_tagged_flt_prepare(dev, f, rule);
	if (err) {
		kfree(rule);
		return err;
	}

	err = qos_tc_flower_storage_add(dev, f->cookie, TC_TYPE_VLAN_FILTER,
					node, rule);
	if (err < 0) {
		kfree(rule);
		return err;
	}

	if (rule->prio > QOS_TC_FILTER_DEF_PRIO_THRESHOLD)
		rule->def = DP_VLAN_DEF_RULE;

	list_add(&rule->list, &node->rules);

	node->rule_cnt++;

	return 0;
}

static void dp_vlan_list_update(struct net_device *dev,
				struct dp_tc_vlan *dp_vlan,
				struct qos_tc_vlan_storage_node *node)
{
	switch (node->tag) {
	case TC_VLAN_UNTAGGED:
		dp_vlan->vlan0_head = &node->rules;
		dp_vlan->n_vlan0 = node->rule_cnt;
		break;
	case TC_VLAN_SINGLE_TAGGED:
		dp_vlan->vlan1_head = &node->rules;
		dp_vlan->n_vlan1 = node->rule_cnt;
		break;
	case TC_VLAN_DOUBLE_TAGGED:
		dp_vlan->vlan2_head = &node->rules;
		dp_vlan->n_vlan2 = node->rule_cnt;
		break;
	default:
		break;
	}
}

static int dp_update(struct net_device *dev,
		     struct dp_tc_vlan *dp_vlan,
		     bool ingress)
{
	struct qos_tc_vlan_storage_node *p;

	list_for_each_entry(p, &tc_vlan_filter_storage, list) {
		if (dev == p->dev &&
		    ingress == p->ingress) {
			dp_vlan_list_update(dev, dp_vlan, p);
		}
	}

	if (dp_vlan_set(dp_vlan, 0) == DP_FAILURE) {
		netdev_err(dev, "dp_vlan_set failed!\n");
		return -EPERM;
	}

	return 0;
}

int qos_tc_vlan_filter_add(struct net_device *dev,
			   struct flow_cls_offload *f,
			   bool ingress)
{
	struct dp_tc_vlan dp_vlan = {0};
	struct qos_tc_vlan_storage_node *node;
	enum tc_flower_vlan_tag tag;
	int err = 0;

	netdev_dbg(dev, "VLAN filter add, flower: %p\n", f);

	trace_vf_add_enter(dev, NULL, -1, -1, ingress);

	tag = qos_tc_vlan_tag_get(dev, f);
	if (tag == TC_VLAN_UNKNOWN)
		return -EIO;

	node = qos_tc_vlan_storage_get(dev, ingress, NULL, tag, false,
				       &tc_vlan_filter_storage);
	if (!node) {
		err = qos_tc_vlan_storage_crt(dev, ingress, tag, false, &node);
		if (err)
			return err;
		list_add(&node->list, &tc_vlan_filter_storage);
	}

	qos_tc_dp_vlan_prepare(dev, &dp_vlan, ingress);

	switch (tag) {
	case TC_VLAN_UNTAGGED:
		err = untagged_add(dev, f, node, &dp_vlan);
		if (err)
			goto err;
		break;
	case TC_VLAN_SINGLE_TAGGED:
		err = single_tagged_add(dev, f, node);
		if (err)
			goto err;
		break;
	case TC_VLAN_DOUBLE_TAGGED:
		err = double_tagged_add(dev, f, node);
		if (err)
			goto err;
		break;
	default:
		err = -EIO;
		goto err;
	}

	err = dp_update(dev, &dp_vlan, ingress);

	trace_vf_add_exit(dev, node, node->tag, node->rule_cnt, ingress);

	return err;
err:
	qos_tc_vlan_storage_del(dev, node);
	return err;
}

int qos_tc_vlan_filter_del(struct net_device *dev,
			   void *vlan_storage,
			   void *rule)
{
	struct dp_tc_vlan dp_vlan = {0};
	struct qos_tc_vlan_storage_node *node;
	struct dp_vlan0 *rule0;
	struct dp_vlan1 *rule1;
	struct dp_vlan2 *rule2;
	bool ingress;
	int ret = 0;

	netdev_dbg(dev, "VLAN filter del, node: %p, rule: %p\n",
		   vlan_storage, rule);

	if (!vlan_storage || !rule)
		return -EIO;

	node = vlan_storage;
	ingress = node->ingress;

	trace_vf_del_enter(dev, node, node->tag, node->rule_cnt, ingress);

	qos_tc_dp_vlan_prepare(dev, &dp_vlan, ingress);

	switch (node->tag) {
	case TC_VLAN_UNTAGGED:
		rule0 = rule;
		list_del(&rule0->list);
		break;
	case TC_VLAN_SINGLE_TAGGED:
		rule1 = rule;
		list_del(&rule1->list);
		break;
	case TC_VLAN_DOUBLE_TAGGED:
		rule2 = rule;
		list_del(&rule2->list);
		break;
	default:
		return -EIO;
	}

	kfree(rule);
	node->rule_cnt--;
	qos_tc_vlan_storage_del(dev, node);

	ret = dp_update(dev, &dp_vlan, ingress);

	trace_vf_del_exit(dev, NULL, -1, -1, ingress);

	return ret;
}

static bool qos_tc_lookup_by_cvid(struct qos_tc_vlan_storage_node *node,
				  int cvid)
{
	struct dp_vlan1 *p1;
	struct dp_vlan2 *p2;

	switch (node->tag) {
	case TC_VLAN_UNTAGGED:
		break;
	case TC_VLAN_SINGLE_TAGGED:
		list_for_each_entry(p1, &node->rules, list) {
			if (p1->outer.vid == cvid)
				return true;
		}
		break;
	case TC_VLAN_DOUBLE_TAGGED:
		list_for_each_entry(p2, &node->rules, list) {
			if (p2->inner.vid == cvid)
				return true;
		}
		break;
	default:
		break;
	}

	return false;
}

static struct net_device *qos_tc_search_pmapper_on_list(int cvid,
						      bool ingress,
						      struct list_head *storage)
{
	struct qos_tc_vlan_storage_node *p;

	list_for_each_entry(p, storage, list) {
		rtnl_lock();
		if (p->ingress == ingress &&
		    dp_is_pmapper_check(p->dev)) {
			if (qos_tc_lookup_by_cvid(p, cvid)) {
				rtnl_unlock();
				return p->dev;
			}
		}
		rtnl_unlock();
	}

	return NULL;
}

struct net_device *qos_tc_find_pmapper_via_cvid(int cvid, bool ingress)
{
	struct net_device *dev = NULL;

	dev = qos_tc_search_pmapper_on_list(cvid, ingress,
					    &tc_vlan_filter_storage);

	if (dev == NULL)
		dev = qos_tc_search_pmapper_on_list(cvid, ingress,
					qos_tc_get_ext_vlan_storage());
	return dev;
}

static bool is_dscp(struct dp_act_vlan *act)
{
	unsigned int i;

	if (act->push_n == 0)
		return false;

	for (i = 0; i < act->push_n; i++)
		if (act->prio[i] != DERIVE_FROM_DSCP)
			return false;

	return true;
}

static void print_rule(struct seq_file *file, struct dp_pattern_vlan *outer,
		       struct dp_pattern_vlan *inner, struct dp_act_vlan *act)
{
	unsigned int i = 0;

	seq_printf(file, "%016llx filter", act->sort_key);

	if (outer) {
		seq_printf(file, " outer prio %d tpid 0x%x vid %d dei %d proto %d",
			   outer->prio, outer->tpid, outer->vid, outer->dei,
			   outer->proto);
	}

	if (inner) {
		seq_printf(file, " inner prio %d tpid 0x%x vid %d dei %d proto %d",
			   inner->prio, inner->tpid, inner->vid, inner->dei,
			   inner->proto);
	}

	seq_puts(file, "\n");

	seq_printf(file, "treatment act %d pop_n %d push_n %d", act->act,
		   act->pop_n, act->push_n);
	for (i = 0; i < act->push_n; ++i) {
		seq_printf(file, " tpid[%d] 0x%x", i, act->tpid[i]);
		seq_printf(file, " vid[%d] %d", i, act->vid[i]);
		seq_printf(file, " dei[%d] %d", i, act->dei[i]);
		seq_printf(file, " prio[%d] %d", i, act->prio[i]);
	}

	seq_puts(file, "\n");
	if (is_dscp(act)) {
		seq_puts(file, "dscp");
		for (i = 0; i < ARRAY_SIZE(act->dscp_pcp_map); ++i) {
			if (act->dscp_pcp_map_valid[i])
				seq_printf(file, " %2x", act->dscp_pcp_map[i]);
			else
				seq_puts(file, " ..");
		}
		seq_puts(file, "\n");
	}
	seq_puts(file, "\n");
}

static void print_node(struct seq_file *file,
		       struct qos_tc_vlan_storage_node *p)
{
	seq_printf(file, "%s", p->dev->name);
	if (p->ingress)
		seq_puts(file, " ingress");
	else
		seq_puts(file, " egress");

	switch (p->tag) {
	case TC_VLAN_UNTAGGED:
		seq_puts(file, " untagged");
		break;
	case TC_VLAN_SINGLE_TAGGED:
		seq_puts(file, " single");
		break;
	case TC_VLAN_DOUBLE_TAGGED:
		seq_puts(file, " double");
		break;
	default:
		seq_puts(file, " unknown");
		break;
	}

	if (p->mcc)
		seq_puts(file, " mcc");

	seq_puts(file, "\n");

	if (p->tag == TC_VLAN_UNTAGGED) {
		struct dp_vlan0 *i;

		list_for_each_entry(i, &p->rules, list)
			print_rule(file, &i->outer, NULL, &i->act);
	}
	if (p->tag == TC_VLAN_SINGLE_TAGGED) {
		struct dp_vlan1 *i;

		list_for_each_entry(i, &p->rules, list)
			print_rule(file, &i->outer, NULL, &i->act);
	}
	if (p->tag == TC_VLAN_DOUBLE_TAGGED) {
		struct dp_vlan2 *i;

		list_for_each_entry(i, &p->rules, list)
			print_rule(file, &i->outer, &i->inner, &i->act);
	}
}

void qos_tc_vlan_filter_storage_debugfs(struct seq_file *file, void *ctx)
{
	struct qos_tc_vlan_storage_node *p;

	rtnl_lock();
	list_for_each_entry(p, &tc_vlan_filter_storage, list) {
		print_node(file, p);
	}
	rtnl_unlock();
}
