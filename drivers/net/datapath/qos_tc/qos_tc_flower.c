// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2020 - 2024 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/
#include <net/pkt_cls.h>
#include <net/tc_act/tc_mirred.h>
#include <net/tc_act/tc_colmark.h>
#include <net/tc_act/tc_police.h>
#include <net/tc_act/tc_gact.h>
#include <net/tc_act/tc_vlan.h>
#include <net/tc_act/tc_skbedit.h>
#include <net/tc_act/tc_sample.h>
#include <net/flow_dissector.h>
#include <linux/version.h>
#include "qos_tc_flower.h"
#include "qos_tc_ext_vlan.h"
#include "qos_tc_vlan_filter.h"
#include "qos_tc_qmap.h"
#include "qos_tc_mirred.h"
#include "qos_tc_police.h"
#include "qos_tc_trap.h"
#include "qos_tc_ip_drop.h"
#include "qos_tc_skbedit.h"
#include "qos_tc_extract_cfm.h"
#include "qos_tc_trace.h"

struct qos_tc_storage_node {
	/** Unique key for TC flower - flower cookie */
	unsigned long key;
	/** Type of TC flower */
	enum qos_tc_flower_type type;
	/** Pointer to structure of specific type */
	void *arg1;
	/** Pointer to structure of specific type */
	void *arg2;
	/** Hash table identifier */
	struct hlist_node node;
};

static DEFINE_HASHTABLE(qos_tc_storage, 16);

static char *flower_type_str_get(enum qos_tc_flower_type type)
{
	switch (type) {
	case TC_TYPE_EXT_VLAN:
		return "TC_TYPE_EXT_VLAN";
	case TC_TYPE_VLAN_FILTER:
		return "TC_TYPE_VLAN_FILTER";
	case TC_TYPE_QUEUE:
		return "TC_TYPE_QUEUE";
	case TC_TYPE_MIRRED:
		return "TC_TYPE_MIRRED";
	case TC_TYPE_POLICE:
		return "TC_TYPE_POLICE";
	case TC_TYPE_COLMARK:
		return "TC_TYPE_COLMARK";
	case TC_TYPE_TRAP:
		return "TC_TYPE_TRAP";
	case TC_TYPE_IP_DROP:
		return "TC_TYPE_DROP";
	case TC_TYPE_SKBEDIT:
		return "TC_TYPE_SKBEDIT";
	case TC_TYPE_EXTRACT_CFM:
		return "TC_TYPE_EXTRACT_CFM";
	default:
		return "TC_TYPE_UNKNOWN";
	}
}

int qos_tc_flower_storage_add(struct net_device *dev,
			      unsigned long cookie,
			      enum qos_tc_flower_type type,
			      void *arg1, void *arg2)
{
	struct qos_tc_storage_node *entry;

	entry = kzalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		return -ENOMEM;

	entry->key = cookie;
	entry->type = type;
	entry->arg1 = arg1;
	entry->arg2 = arg2;

	hash_add(qos_tc_storage, &entry->node, entry->key);
	netdev_dbg(dev, "QoS TC flower storage add, cookie: %lx, type: %s, arg1: %p, arg2: %p\n",
		   entry->key, flower_type_str_get(entry->type),
		   entry->arg1, entry->arg2);

	return 0;
}

static int flower_remove(struct net_device *dev,
			 unsigned long cookie,
			 struct flow_cls_offload *f,
			 enum qos_tc_flower_type type,
			 void *arg1, void *arg2,
			 const struct qos_tc_params *tc_params)
{
	switch (type) {
	case TC_TYPE_EXT_VLAN:
		return qos_tc_ext_vlan_del(dev, arg1, arg2);
	case TC_TYPE_VLAN_FILTER:
		return qos_tc_vlan_filter_del(dev, arg1, arg2);
	case TC_TYPE_QUEUE:
		return qos_tc_unmap(dev, arg1, tc_params);
	case TC_TYPE_MIRRED:
		return qos_tc_mirred_unoffload(dev, f, cookie);
	case TC_TYPE_IP_DROP:
		return qos_tc_ip_drop_unoffload(dev, f, cookie);
	case TC_TYPE_POLICE:
	case TC_TYPE_COLMARK:
		return qos_tc_police_unoffload(dev, arg1, arg2);
	case TC_TYPE_TRAP:
		return qos_tc_trap_unoffload(dev, f, cookie);
	case TC_TYPE_SKBEDIT:
		return qos_tc_skbedit_unoffload(dev, arg1, arg2);
	case TC_TYPE_EXTRACT_CFM:
		return qos_tc_cfm_unoffload(dev, f, cookie, arg1, arg2);
	default:
		return -EOPNOTSUPP;
	}

	return -EOPNOTSUPP;
}

static int qos_tc_flower_storage_del(struct net_device *dev,
				     unsigned long cookie,
				     struct flow_cls_offload *f,
				     const struct qos_tc_params *tc_params)
{
	struct qos_tc_storage_node *entry;
	struct hlist_node *tmp;

	hash_for_each_possible_safe(qos_tc_storage, entry, tmp, node,
				    f->cookie) {
		if (f->cookie != entry->key)
			continue;
		flower_remove(dev, cookie, f, entry->type, entry->arg1,
			      entry->arg2, tc_params);
		netdev_dbg(dev, "QoS TC flower storage del, cookie: %lx, type: %s\n",
			   entry->key, flower_type_str_get(entry->type));
		hash_del(&entry->node);
		kfree(entry);
	}

	return 0;
}

bool has_action_id(struct flow_cls_offload *f, enum flow_action_id id)
{
	const struct flow_action_entry *a;
	int i;

	flow_action_for_each(i, a, &f->rule->action)
		if (a->id == id)
			return true;

	return false;
}

static bool is_type_mirred(struct net_device *dev,
			   struct flow_cls_offload *f)
{
	return has_action_id(f, FLOW_ACTION_REDIRECT) ||
	       has_action_id(f, FLOW_ACTION_MIRRED) ||
	       has_action_id(f, FLOW_ACTION_REDIRECT_INGRESS) ||
	       has_action_id(f, FLOW_ACTION_MIRRED_INGRESS);
}

static bool is_type_ip_drop(struct net_device *dev,
			    struct flow_cls_offload *f)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);

	if (!dissector_uses_key(d, FLOW_DISSECTOR_KEY_BASIC) ||
	    !dissector_uses_key(d, FLOW_DISSECTOR_KEY_CONTROL))
		return false;

	if (!dissector_uses_key(d, FLOW_DISSECTOR_KEY_IPV4_ADDRS) &&
	    !dissector_uses_key(d, FLOW_DISSECTOR_KEY_IPV6_ADDRS))
		return false;

	return has_action_id(f, FLOW_ACTION_DROP);
}

static bool is_type_colmark(struct net_device *dev,
			    struct flow_cls_offload *f)
{
	/* This driver supports only single actions for now */
	return has_action_id(f, FLOW_ACTION_COLMARK);
}

static bool is_type_police(struct net_device *dev,
			   struct flow_cls_offload *f)
{
	/* This driver supports only single actions for now */
	return has_action_id(f, FLOW_ACTION_POLICE);
}


static bool is_type_trap(struct net_device *dev,
			 struct flow_cls_offload *f)
{
	return has_action_id(f, FLOW_ACTION_TRAP);
}

static bool is_type_drop(struct net_device *dev,
			 struct flow_cls_offload *f)
{
	return has_action_id(f, FLOW_ACTION_DROP);
}

static bool is_type_queue(struct net_device *dev,
			  struct flow_cls_offload *f)
{
	return !!f->classid;
}

static bool is_type_vlan_filter(struct net_device *dev,
				struct flow_cls_offload *f)
{
	struct net_device *indev;

	indev = qos_tc_get_indev(dev, f);
	if (!indev)
		return false;

	if (!strncmp(dev->name, indev->name, sizeof(dev->name)))
		return true;

	return false;
}

static bool is_dissector_vlan_compatible(struct flow_cls_offload *f)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_BASIC) ||
	    dissector_uses_key(d, FLOW_DISSECTOR_KEY_VLAN) ||
	    dissector_uses_key(d, FLOW_DISSECTOR_KEY_CVLAN))
		return true;

	return false;
}

static bool is_type_ext_vlan(struct net_device *dev,
			     struct flow_cls_offload *f)
{
	if (is_dissector_vlan_compatible(f))
		return has_action_id(f, FLOW_ACTION_ACCEPT) ||
		       has_action_id(f, FLOW_ACTION_DROP) ||
		       has_action_id(f, FLOW_ACTION_VLAN_PUSH) ||
		       has_action_id(f, FLOW_ACTION_VLAN_POP) ||
		       has_action_id(f, FLOW_ACTION_VLAN_MANGLE);

	return false;
}

static bool is_type_skbedit(struct net_device *dev,
			    struct flow_cls_offload *f)
{
	return has_action_id(f, FLOW_ACTION_MARK) ||
	       has_action_id(f, FLOW_ACTION_PTYPE) ||
	       has_action_id(f, FLOW_ACTION_PRIORITY);
}

static bool is_type_accept(struct flow_cls_offload *f)
{
	return has_action_id(f, FLOW_ACTION_ACCEPT);
}

static bool is_type_sample(struct net_device *dev,
			   struct flow_cls_offload *f)
{
	return has_action_id(f, FLOW_ACTION_SAMPLE);
}

int qos_tc_get_cfm_act(struct net_device *dev, struct flow_cls_offload *f)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);

	netdev_dbg(dev, "%s: nr_actions: %d mirred: %s trap: %s sample: %s ok: %s shot: %s\n",
		   __func__, f->rule->action.num_entries,
		   is_type_mirred(dev, f) ? "true" : "false",
		   is_type_trap(dev, f) ? "true" : "false",
		   is_type_sample(dev, f) ? "true" : "false",
		   is_type_accept(f) ? "true" : "false",
		   is_type_drop(dev, f) ? "true" : "false");

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_CFM) &&
	    is_type_drop(dev, f) && f->rule->action.num_entries == 1)
		return CFM_ACT_DROP;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_CFM) &&
	    is_type_trap(dev, f) && f->rule->action.num_entries == 1)
		return CFM_ACT_EXT1;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_CFM) &&
	    is_type_mirred(dev, f) && is_type_trap(dev, f) &&
	    f->rule->action.num_entries == 3)
		return CFM_ACT_EXT2;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_CFM) &&
	    is_type_mirred(dev, f) && is_type_accept(f) &&
	    f->rule->action.num_entries == 3)
		return CFM_ACT_TX_TS;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_CFM) &&
	    is_type_trap(dev, f) && is_type_sample(dev, f) &&
	    f->rule->action.num_entries == 3)
		return CFM_ACT_EXT1_SAMPLE;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_CFM) &&
	    is_type_mirred(dev, f) && is_type_trap(dev, f) &&
	    is_type_sample(dev, f) && f->rule->action.num_entries == 5)
		return CFM_ACT_EXT2_SAMPLE;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_CFM) &&
	    is_type_mirred(dev, f) && is_type_accept(f) &&
	    is_type_sample(dev, f) && f->rule->action.num_entries == 5)
		return CFM_ACT_TX_SAMPLE;

	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(qos_tc_get_cfm_act);

static bool is_type_extract_cfm(struct net_device *dev,
				struct flow_cls_offload *f)
{
	return qos_tc_get_cfm_act(dev, f) > 0 ? true : false;
}

static enum qos_tc_flower_type
flower_type_get(struct net_device *dev, struct flow_cls_offload *f)
{
	/* CFM at the start as it depends on mirred, trap and sample actions*/
	if (is_type_extract_cfm(dev, f))
		return TC_TYPE_EXTRACT_CFM;

	/* must be prio to is_type_ext_vlan() in order
	 * to handle combined ext. VLAN and skbedit actions.
	 */
	if (is_type_skbedit(dev, f))
		return TC_TYPE_SKBEDIT;

	if (is_type_trap(dev, f))
		return TC_TYPE_TRAP;

	if (is_type_ip_drop(dev, f))
		return TC_TYPE_IP_DROP;

	if (is_type_mirred(dev, f))
		return TC_TYPE_MIRRED;

	if (is_type_police(dev, f))
		return TC_TYPE_POLICE;

	if (is_type_colmark(dev, f))
		return TC_TYPE_COLMARK;

	if (is_type_queue(dev, f))
		return TC_TYPE_QUEUE;

	if (is_type_vlan_filter(dev, f))
		return TC_TYPE_VLAN_FILTER;

	if (is_type_ext_vlan(dev, f))
		return TC_TYPE_EXT_VLAN;

	return TC_TYPE_UNKNOWN;
}

static int qos_tc_flower_replace(struct net_device *dev,
				 unsigned long cookie,
				 bool ingress,
				 struct flow_cls_offload *f,
				 const struct qos_tc_params *tc_params)
{
	enum qos_tc_flower_type type;

	type = flower_type_get(dev, f);
	netdev_dbg(dev, "TC TYPE: %s\n", flower_type_str_get(type));

	switch (type) {
	case TC_TYPE_SKBEDIT:
		return qos_tc_skbedit_offload(dev, f, ingress);
	case TC_TYPE_EXT_VLAN:
		return qos_tc_ext_vlan_add(dev, f, ingress, NULL);
	case TC_TYPE_VLAN_FILTER:
		return qos_tc_vlan_filter_add(dev, f, ingress);
	case TC_TYPE_QUEUE:
		return qos_tc_map(dev, f, ingress, tc_params);
	case TC_TYPE_IP_DROP:
		return qos_tc_ip_drop_offload(dev, f, cookie);
	case TC_TYPE_MIRRED:
		return qos_tc_mirred_offload(dev, f, cookie);
	case TC_TYPE_POLICE:
	case TC_TYPE_COLMARK:
		return qos_tc_police_offload(dev, f, ingress);
	case TC_TYPE_TRAP:
		return qos_tc_trap_offload(dev, f, cookie);
	case TC_TYPE_EXTRACT_CFM:
		return qos_tc_cfm_offload(dev, f, ingress);
	default:
		return -EOPNOTSUPP;
	}

	return -EOPNOTSUPP;
}

static int qos_tc_flower_destroy(struct net_device *dev,
				 unsigned long cookie,
				 struct flow_cls_offload *f,
				 const struct qos_tc_params *tc_params)
{
	return qos_tc_flower_storage_del(dev, cookie, f, tc_params);
}

int qos_tc_flower_offload(struct net_device *dev, bool ingress, void *type_data,
		const struct qos_tc_params *tc_params)
{
	int ret = 0;
	struct flow_cls_offload *f = type_data;

	ASSERT_RTNL();
	netdev_dbg(dev, "%s:start %d\n", __func__, ret);

	switch (f->command) {
	case FLOW_CLS_REPLACE:
	{
		netdev_dbg(dev, "%s:replace:\n", __func__);
		trace_qos_tc_flower_enter(dev, f, flower_type_get(dev, f));
		ret = qos_tc_flower_replace(dev, f->cookie, ingress, f, tc_params);
		break;
	}
	case FLOW_CLS_DESTROY:
	{
		netdev_dbg(dev, "%s:destroy:\n", __func__);
		trace_qos_tc_flower_enter(dev, f, -1);
		ret = qos_tc_flower_destroy(dev, f->cookie, f, tc_params);
		break;
	}
	case FLOW_CLS_STATS:
	{
		netdev_dbg(dev, "%s:stats:\n", __func__);
		trace_qos_tc_flower_enter(dev, f, -1);
		return -EOPNOTSUPP;
	}
	default:
		return -EOPNOTSUPP;
	}

	netdev_dbg(dev, "%s:exit %d\n", __func__, ret);
	trace_qos_tc_flower_exit(dev, f, -1);
	return ret;
}

int qos_tc_block_cb_ingress(enum tc_setup_type type,
			    void *type_data,
			    void *cb_priv)
{
	int ret = 0;
	struct net_device *dev = cb_priv;

	ASSERT_RTNL();
	netdev_dbg(dev, "%s:start\n", __func__);

	if (!tc_cls_can_offload_and_chain0(dev, type_data))
		return -EOPNOTSUPP;

	if (type == TC_SETUP_CLSFLOWER)
		ret = qos_tc_flower_offload(dev, true, type_data, NULL);

	netdev_dbg(dev, "%s:exit %d\n", __func__, ret);
	return ret;
}

int qos_tc_block_cb_egress(enum tc_setup_type type,
			   void *type_data,
			   void *cb_priv)
{
	int ret = 0;
	struct net_device *dev = cb_priv;

	ASSERT_RTNL();
	netdev_dbg(dev, "%s:start\n", __func__);

	if (!tc_cls_can_offload_and_chain0(dev, type_data))
		return -EOPNOTSUPP;

	if (type == TC_SETUP_CLSFLOWER)
		ret = qos_tc_flower_offload(dev, false, type_data, NULL);

	netdev_dbg(dev, "%s:exit %d\n", __func__, ret);
	return ret;
}

static LIST_HEAD(qos_tc_block_cb_list);

int qos_tc_block_offload(struct net_device *dev,
			 void *type_data)
{
	int ret = 0;
	flow_setup_cb_t *cb;
	struct flow_block_offload *f = type_data;
	struct flow_block_cb *block_cb;

	ASSERT_RTNL();
	netdev_dbg(dev, " tc block offload starting - binder type %d\n",
		   f->binder_type);

	if (f->binder_type == FLOW_BLOCK_BINDER_TYPE_CLSACT_INGRESS)
		cb = qos_tc_block_cb_ingress;
	else if (f->binder_type == FLOW_BLOCK_BINDER_TYPE_CLSACT_EGRESS)
		cb = qos_tc_block_cb_egress;
	else
		return -EOPNOTSUPP;

	f->driver_block_list = &qos_tc_block_cb_list;

	switch (f->command) {
	case FLOW_BLOCK_BIND:
		netdev_dbg(dev, "CLS BIND\n");
		if (flow_block_cb_is_busy(cb, dev_add_offload,
					  &qos_tc_block_cb_list))
			return -EBUSY;

		block_cb = flow_block_cb_alloc(cb, dev, dev, NULL);
		if (IS_ERR(block_cb))
			return PTR_ERR(block_cb);

		flow_block_cb_add(block_cb, f);
		list_add_tail(&block_cb->driver_list, &qos_tc_block_cb_list);
		return 0;
	case FLOW_BLOCK_UNBIND:
		netdev_dbg(dev, "CLS UNBIND\n");
		block_cb = flow_block_cb_lookup(f->block, cb, dev);
		if (!block_cb)
			return -ENOENT;

		flow_block_cb_remove(block_cb, f);
		list_del(&block_cb->driver_list);
		return 0;
	default:
		return -EOPNOTSUPP;
	}

	netdev_dbg(dev, "%s:exit %d\n", __func__, ret);
	return ret;
}

struct net_device *qos_tc_get_indev(struct net_device *dev,
				    struct flow_cls_offload *f)
{
	struct flow_rule *rule = flow_cls_offload_flow_rule(f);
	struct flow_match_meta match;

	if (!flow_rule_match_key(rule, FLOW_DISSECTOR_KEY_META))
		return NULL;

	flow_rule_match_meta(rule, &match);

	if (match.mask->ingress_ifindex != 0xFFFFFFFF) {
		netdev_err(dev, "Unsupported ingress ifindex mask");
		return NULL;
	}

	return __dev_get_by_index(dev_net(dev), match.key->ingress_ifindex);
}

void qos_tc_storage_debugfs(struct seq_file *file, void *ctx)
{
	struct qos_tc_storage_node *entry;
	struct hlist_node *tmp;
	int i;

	rtnl_lock();
	hash_for_each_safe(qos_tc_storage, i, tmp, entry, node)
		seq_printf(file, "key: %ld, tc type: %s\n",
			   entry->key, flower_type_str_get(entry->type));
	rtnl_unlock();
}
