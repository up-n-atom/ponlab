// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2024 MaxLinear, Inc.
 *
 *****************************************************************************/
#include <linux/netdevice.h>
#include <linux/types.h>
#include <linux/version.h>
#include <net/pkt_cls.h>
#include <net/tc_act/tc_mirred.h>
#include <net/tc_act/tc_gact.h>
#include <net/tc_act/tc_sample.h>
#include "qos_tc_flower.h"
#include "qos_tc_extract_cfm.h"
#include "../dp_oam/dp_oam.h"

#define MAX_MDL 8

#define CFM_DMM 0x2f
#define CFM_DMR 0x2e

struct cfm_rule {
	struct net_device *dev;
	unsigned long level[MAX_MDL];
	u8 level_num;
	enum cfm_act_type type;
	struct dp_oam_args parm;
	struct list_head list;
};

static LIST_HEAD(cfm_ext_rules_list);
static LIST_HEAD(cfm_drop_rules_list);

struct ts_offset_map {
	u8 opcode;
	u8 types;
	u8 offset;
};

struct ts_offset_map ts_supported_opcodes[] = {
	{ .opcode = CFM_DMM,
	  .types = CFM_ACT_EXT1 | CFM_ACT_EXT2,
	  .offset = 14
	},
	{ .opcode = CFM_DMM,
	  .types = CFM_ACT_TX_TS,
	  .offset = 6,
	},
	{ .opcode = CFM_DMR,
	  .types = CFM_ACT_EXT1 | CFM_ACT_EXT2,
	  .offset = 30,
	},
	{ .opcode = CFM_DMR,
	  .types = CFM_ACT_TX_TS,
	  .offset = 22
	},
};

int (*qos_tc_cfm_offload_fn)(struct dp_oam_args *args) = NULL;
EXPORT_SYMBOL(qos_tc_cfm_offload_fn);
int (*qos_tc_cfm_unoffload_fn)(struct dp_oam_args *args) = NULL;
EXPORT_SYMBOL(qos_tc_cfm_unoffload_fn);

static int check_cfm_proto(struct flow_cls_offload *f)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);

	if (f->common.protocol == htons(ETH_P_CFM))
		return 0;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_BASIC)) {
		struct flow_dissector_key_basic *key =
			skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_BASIC,
						  qos_tc_get_key(f));
		struct flow_dissector_key_basic *mask =
			skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_BASIC,
						  qos_tc_get_mask(f));

		if (mask->n_proto && ntohs(key->n_proto) == ETH_P_CFM)
			return 0;
	}

	pr_debug("%s: invalid cfm rule\n", __func__);
	return -EOPNOTSUPP;
}

static int check_flow_dissector_keys(unsigned int used_keys)
{
	if (used_keys &
	    ~(BIT(FLOW_DISSECTOR_KEY_CONTROL) |
	      BIT(FLOW_DISSECTOR_KEY_BASIC) |
	      BIT(FLOW_DISSECTOR_KEY_VLAN) |
	      BIT(FLOW_DISSECTOR_KEY_CVLAN) |
	      BIT(FLOW_DISSECTOR_KEY_CFM) |
	      BIT(FLOW_DISSECTOR_KEY_ETH_ADDRS))) {
		pr_debug("%s: Unsupported key used: 0x%x\n", __func__,
			 used_keys);
		return -EINVAL;
	}

	return 0;
}

static int set_oam_args_dev(struct net_device *dev,
			    struct dp_oam_args *p)
{
	p->dev_en = true;
	p->ifindex = dev->ifindex;

	return 0;
}

static int set_oam_args_vlan(struct flow_cls_offload *f,
			     struct dp_oam_args *p)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_vlan *key = NULL;
	struct flow_dissector_key_vlan *mask = NULL;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_VLAN)) {
		key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_VLAN,
						qos_tc_get_key(f));
		mask = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_VLAN,
						 qos_tc_get_mask(f));

		if (mask->vlan_id == 0xfff) {
			p->vlan_en = true;
			p->vlan_id = key->vlan_id;
		}
	}

	return 0;
}

static int set_oam_args_cfm_op(struct flow_cls_offload *f,
			       struct dp_oam_args *p)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_cfm *key = NULL;
	struct flow_dissector_key_cfm *mask = NULL;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_CFM)) {
		key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_CFM,
						qos_tc_get_key(f));
		mask = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_CFM,
						 qos_tc_get_mask(f));

		if (mask->opcode) {
			p->opcode_en = true;
			p->opcode = key->opcode;
		}
	}

	return 0;
}

static int set_oam_args_cfm_mdl(struct flow_cls_offload *f,
				struct dp_oam_args *p)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	struct flow_dissector_key_cfm *key = NULL;
	struct flow_dissector_key_cfm *mask = NULL;

	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_CFM)) {
		key = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_CFM,
						qos_tc_get_key(f));
		mask = skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_CFM,
						 qos_tc_get_mask(f));

		if (mask->mdl) {
			p->level_en = true;
			p->level = key->mdl;
		}
	}

	return 0;
}

static struct dp_oam_args convert_flower_to_oam_args(struct net_device *dev,
						     struct flow_cls_offload *f)
{
	struct dp_oam_args p = {0};

	set_oam_args_dev(dev, &p);

	set_oam_args_vlan(f, &p);

	set_oam_args_cfm_op(f, &p);

	set_oam_args_cfm_mdl(f, &p);

	p.prio = f->common.prio >> 16;

	return p;
}

static int is_cfm_offload_supported(struct net_device *dev,
				    struct flow_cls_offload *f)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);
	int ret = 0;

	ret = check_cfm_proto(f);
	if (ret < 0) {
		netdev_err(dev, "%s: proto not supported %d\n", __func__, ret);
		return ret;
	}

	ret = check_flow_dissector_keys(d->used_keys);
	if (ret < 0) {
		netdev_err(dev, "%s: attr. not supported %d\n", __func__, ret);
		return ret;
	}

	ret = qos_tc_get_cfm_act(dev, f);
	if (ret < 0) {
		netdev_err(dev, "%s: action not supported %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

static bool cmp_oam_args(struct dp_oam_args *p1, struct dp_oam_args *p2)
{
	if (p1->dev_en == p2->dev_en && p1->ifindex != p2->ifindex)
		return false;

	if (p1->vlan_en == p2->vlan_en && p1->vlan_id != p2->vlan_id)
		return false;

	if (p1->opcode_en == p2->opcode_en && p1->opcode != p2->opcode)
		return false;

	return true;
}

static int get_max_mdl(struct cfm_rule *entry)
{
	int i, max_mdl = 0;

	for (i = 0; i < MAX_MDL; i++)
		if (entry->level[i])
			max_mdl = i;

	return max_mdl;
}

static int offload_rule_to_hw(struct net_device *dev, struct dp_oam_args *args)
{
	int ret;

	/* dp_oam uses ranges to reduce hw rules usage, so it expectes one
	 * level higher to drop all levels including max mdl, e.g. if all
	 * levels <= 5 need to be dropped then level = 6 should be used.
	 */
	if (test_bit(DP_OAM_ACT_DROP, args->act) && args->level_en)
		args->level++;

	ret = qos_tc_cfm_offload_fn ? qos_tc_cfm_offload_fn(args) : -1;
	if (ret) {
		netdev_err(dev, "dp_oam_act_add failed: %d\n", ret);
		return -EINVAL;
	}

	if (test_bit(DP_OAM_ACT_DROP, args->act) && args->level_en)
		args->level--;

	return ret;
}

static int unoffload_rule_from_hw(struct net_device *dev,
				  struct dp_oam_args *args)
{
	int ret;

	ret = qos_tc_cfm_unoffload_fn ? qos_tc_cfm_unoffload_fn(args) : -1;
	if (ret) {
		netdev_err(dev, "dp_oam_act_del failed: %d\n", ret);
		return -EINVAL;
	}

	return ret;
}

static int update_filter_in_hw(u8 old_level, struct cfm_rule *entry)
{
	int ret;

	netdev_dbg(entry->dev, "%s: level %d->%d\n",
		   __func__, old_level, entry->parm.level);
	if (old_level == entry->parm.level)
		return 0;

	/* dp_oam_act_del needs only index+ifindex */
	ret = unoffload_rule_from_hw(entry->dev, &entry->parm);
	if (ret)
		return ret;

	return offload_rule_to_hw(entry->dev, &entry->parm);
}

static int update_mdl_filter_in_hw(struct cfm_rule *entry,
				   struct dp_oam_args *p,
				   unsigned long cookie)
{
	u8 old_level = entry->parm.level;
	int ret = 0;

	entry->level[p->level] = cookie;
	entry->parm.level = get_max_mdl(entry);
	entry->level_num++;

	ret = update_filter_in_hw(old_level, entry);
	if (ret)
		return ret;

	ret = qos_tc_flower_storage_add(entry->dev, cookie,
					TC_TYPE_EXTRACT_CFM,
					entry, &entry->level[p->level]);

	netdev_dbg(entry->dev, "%s: entry %pK type: %d level %d cookie: %#lx\n",
		   __func__, entry, entry->type, p->level, cookie);

	return ret;
}

static int handle_drop_rule_exists(struct dp_oam_args *args,
				   unsigned long cookie)
{
	struct cfm_rule *entry;

	list_for_each_entry(entry, &cfm_drop_rules_list, list) {
		struct dp_oam_args *p = &entry->parm;

		if (cmp_oam_args(p, args))
			return update_mdl_filter_in_hw(entry, args, cookie);
	}

	return -ENOENT;
}

static void set_ext_point(struct dp_oam_args *args, enum cfm_act_type type)
{
	args->ext_pt = (type == CFM_ACT_EXT1) ?
			DP_OAM_EXT_VLAN : DP_OAM_EXT_Q_VLAN;
}

static void set_ts_act(struct dp_oam_args *args, enum cfm_act_type type)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(ts_supported_opcodes); i++) {
		struct ts_offset_map *p = &ts_supported_opcodes[i];

		if (p->types & type && args->opcode == p->opcode) {
			set_bit(DP_OAM_ACT_TIMESTAMP, args->act);
			args->ts_format = DP_OAM_TS_DIGITAL_8B;
			args->ts_offset = p->offset;
			pr_debug("%s: ts @ offset: %d\n", __func__, p->offset);
			break;
		}
	}
}

static void set_ext_action(struct dp_oam_args *args, enum cfm_act_type type)
{
	switch (type) {
	case CFM_ACT_EXT1:
	case CFM_ACT_EXT2:
	case CFM_ACT_EXT1_SAMPLE:
	case CFM_ACT_EXT2_SAMPLE:
		set_bit(DP_OAM_ACT_EXTRACT, args->act);
		break;
	case CFM_ACT_TX_TS:
		break;
	case CFM_ACT_TX_SAMPLE:
		break;
	default:
		break;
	}
}

static int entry_cleanup(struct cfm_rule *p)
{
	int ret;

	ret = unoffload_rule_from_hw(p->dev, &p->parm);

	netdev_dbg(p->dev, "%s: entry type:%d @%pK\n", __func__, p->type, p);

	list_del(&p->list);
	kfree(p);

	return ret;
}

static struct cfm_rule *add_rule(struct net_device *dev,
				 struct flow_cls_offload *f,
				 struct dp_oam_args *args)
{
	struct cfm_rule *rule;
	int ret;

	ret = offload_rule_to_hw(dev, args);
	if (ret)
		return ERR_PTR(ret);

	rule = kzalloc(sizeof(*rule), GFP_KERNEL);
	if (!rule)
		return ERR_PTR(-ENOMEM);

	rule->parm = *args;
	if (args->level_en) {
		rule->level[args->level] = f->cookie;
		rule->level_num = 1;
	}
	rule->dev = dev;
	rule->type = qos_tc_get_cfm_act(dev, f);

	ret = qos_tc_flower_storage_add(dev, f->cookie, TC_TYPE_EXTRACT_CFM,
					rule, &rule->level[args->level]);
	if (ret < 0) {
		unoffload_rule_from_hw(dev, args);
		kfree(rule);
		return ERR_PTR(ret);
	}

	if (rule->type == CFM_ACT_DROP)
		list_add_tail(&rule->list, &cfm_drop_rules_list);
	else
		list_add_tail(&rule->list, &cfm_ext_rules_list);

	netdev_dbg(dev, "%s: entry %pK level %d cookie: %#lx\n",
		   __func__, rule, rule->parm.level, f->cookie);

	return rule;
}

static int handle_cfm_level_drop_act(struct net_device *dev,
				     struct flow_cls_offload *f)
{
	struct dp_oam_args p = { 0 };
	struct cfm_rule *rule;
	int ret;

	p = convert_flower_to_oam_args(dev, f);
	if (p.opcode_en) {
		netdev_warn(dev, "cfm drop on opcode not supported\n");
		return -EOPNOTSUPP;
	}
	set_bit(DP_OAM_ACT_DROP, p.act);

	/* check list */
	ret = handle_drop_rule_exists(&p, f->cookie);
	if (ret != -ENOENT)
		return ret;

	rule = add_rule(dev, f, &p);
	if (IS_ERR(rule)) {
		netdev_err(dev, "%s: add drop rule fail\n", __func__);
		return PTR_ERR(rule);
	}

	return 0;
}

static int handle_cfm_act(struct net_device *dev,
			  struct flow_cls_offload *f)
{
	struct dp_oam_args p = { 0 };
	struct cfm_rule *rule;

	p = convert_flower_to_oam_args(dev, f);
	set_ext_action(&p, qos_tc_get_cfm_act(dev, f));
	set_ext_point(&p, qos_tc_get_cfm_act(dev, f));
	set_ts_act(&p, qos_tc_get_cfm_act(dev, f));

	rule = add_rule(dev, f, &p);
	if (IS_ERR(rule)) {
		netdev_err(dev, "%s: add ext rule fail\n", __func__);
		return PTR_ERR(rule);
	}

	netdev_dbg(dev, "%s: added level: %d vlan_id: %d ext_pt: %d\n",
		   __func__, rule->parm.level, rule->parm.vlan_id,
		   rule->parm.ext_pt);
	return 0;
}

static int handle_cfm_offload(struct net_device *dev,
			      struct flow_cls_offload *f)
{
	switch (qos_tc_get_cfm_act(dev, f)) {
	case CFM_ACT_DROP:
		return handle_cfm_level_drop_act(dev, f);
	case CFM_ACT_EXT1:
	case CFM_ACT_EXT2:
	case CFM_ACT_TX_TS:
		return handle_cfm_act(dev, f);
	case CFM_ACT_EXT1_SAMPLE:
		break;
	case CFM_ACT_EXT2_SAMPLE:
		break;
	case CFM_ACT_TX_SAMPLE:
		break;
	default:
		break;
	}

	return -EOPNOTSUPP;
}

int qos_tc_cfm_offload(struct net_device *dev,
		       struct flow_cls_offload *f,
		       bool ingress)
{
	int ret;

	ret = is_cfm_offload_supported(dev, f);
	if (ret < 0) {
		netdev_dbg(dev, "%s: cfm offload unsupported\n", __func__);
		return ret;
	}

	ret = handle_cfm_offload(dev, f);

	return ret;
}

static int drop_entry_free(struct cfm_rule *entry)
{
	u8 old_level = entry->parm.level;

	if (!entry->level_num)
		return entry_cleanup(entry);

	entry->parm.level = get_max_mdl(entry);

	if (entry->level_num)
		return update_filter_in_hw(old_level, entry);

	netdev_dbg(entry->dev, "%s: entry @%pK level[] updated\n",
		   __func__, entry);
	return 0;
}

static int free_drop_list_entry(struct cfm_rule *entry,
				unsigned long *p_cookie,
				unsigned long cookie)
{
	WARN_ON(!p_cookie);

	if (*p_cookie != cookie) {
		netdev_err(entry->dev,
			   "%s:bad entry @ %pK p_cookie:%lx cookie:%lx\n",
			   __func__, entry, *p_cookie, cookie);
		return -EINVAL;
	}

	*p_cookie = 0;
	entry->level_num--;

	WARN_ON(entry->level_num < 0 || entry->level_num > 8);

	return drop_entry_free(entry);
}

int qos_tc_cfm_unoffload(struct net_device *dev,
			 struct flow_cls_offload *f,
			 unsigned long cookie,
			 struct cfm_rule *rule,
			 unsigned long *p_cookie)
{
	int ret = 0;

	if (rule->type == CFM_ACT_DROP)
		ret = free_drop_list_entry(rule, p_cookie, cookie);

	if (rule->type == CFM_ACT_EXT1 || rule->type == CFM_ACT_EXT2)
		ret = entry_cleanup(rule);

	return ret;
}

static void print_cfm_drop_rule(struct seq_file *file, struct cfm_rule *p)
{
	struct dp_oam_args *args = &p->parm;
	int i;

	seq_puts(file, "+---+\n");
	seq_printf(file, "dev: %s (%d)\n", p->dev->name, args->ifindex);
	seq_printf(file, "drop mdl_num: %d mdl: %d\n",
		   p->level_num, p->parm.level);
	for (i = 0; i < MAX_MDL; i++)
		seq_printf(file, "\tmdl=%d = %#lx\n", i, p->level[i]);

	if (args->vlan_en)
		seq_printf(file, "vlan_id: %d\n", args->vlan_id);

	seq_puts(file, "+---+\n");
}

void qos_tc_cfm_debugfs(struct seq_file *file, void *ctx)
{
	struct cfm_rule *p, *n;

	rtnl_lock();
	list_for_each_entry_safe(p, n, &cfm_drop_rules_list, list)
		print_cfm_drop_rule(file, p);
	rtnl_unlock();
}
