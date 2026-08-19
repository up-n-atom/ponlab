// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/

#include <linux/list.h>
#include <net/switch_api/gsw_dev.h>
#include <net/datapath_api.h>
#include "qos_tc_flower.h"
#include "qos_tc_pce.h"

/** Mark PCE rule index as reserved */
#define QOS_TC_PCE_RESERVED 1
/** Mark PCE rule index as unreserved */
#define QOS_TC_PCE_UNRESERVED 0

/** Maximum numbers of entries per PCE block */
#define QOS_TC_PCE_BLOCK_ENTRIES_MAX 64
/** Special value used for common block */
#define QOS_TC_PCE_BLOCK_COMMON -1
/** PCE sub-block size for global table */
#define TC_PCE_GLB_SUBBLK_SIZE 8
/** PCE sub-block size for ctp table */
#define TC_PCE_CTP_SUBBLK_SIZE 16

/* This contains the information for one configured PCE rule. */
struct pce_entry {
	/** PCE rule index used in hardware */
	u32 index;
	struct dp_pce_blk_info blk_info;
	/* PCE rule used */
	GSW_PCE_rule_t pce_rule;
	/* List of pce_node */
	struct list_head node_list;
};

/** This structure represent list element storing relation between
 *  TC command and PCE rule.
 */
struct pce_node {
	/** TC command cookie used as dual identificator */
	unsigned long cookie;
	/** TC command preference used as dual identificator */
	int pref;
	/* bridge ports used for this mapping */
	u16 nForwardPortMap[16];
	/* Common data entry for PCE rule, if they have the same pattern
	 * multiple struct pce_node point to the same struct pce_entry
	 */
	struct pce_entry *entry;
	struct list_head list;
	/* List of pce_entry */
	struct list_head pce_list;
};

static LIST_HEAD(tc_pce_list);

static int qos_tc_pce_rule_delete_node(struct pce_entry *p, u32 index);

static int get_subif_data(struct net_device *dev,
			  enum pce_type *type,
			  int *portid,
			  int *subif)
{
	dp_subif_t *dp_subif;
	s32 ret;

	*portid = QOS_TC_PCE_BLOCK_COMMON;
	*subif = QOS_TC_PCE_BLOCK_COMMON;

	switch (*type) {
	case PCE_COMMON:
		pr_debug("%s: portid: %d, subif: %d\n", __func__,
			 QOS_TC_PCE_BLOCK_COMMON, QOS_TC_PCE_BLOCK_COMMON);
		break;
	case PCE_UNCOMMON:
	case PCE_MIRR_VLAN_FWD:
	case PCE_MIRR_VLAN_DROP:
	case PCE_MIRR_VLAN_WILD:
	case PCE_MIRR_UNTAG_FWD:
	case PCE_MIRR_UNTAG_DROP:
		dp_subif = kzalloc(sizeof(*dp_subif), GFP_KERNEL);
		if (!dp_subif) {
			netdev_err(dev, "%s: failed to allocate memory for subif\n",
				   __func__);
			return -ENOMEM;
		}
		ret = dp_get_netif_subifid(dev, NULL, NULL, NULL, dp_subif, 0);
		if (ret != DP_SUCCESS) {
			*type = PCE_COMMON;
			pr_debug("%s: dp_netdev not found, set global mirred\n",
				 __func__);
			kfree(dp_subif);
			return 0;
		}

		pr_debug("%s: portid: %u, subif_groupid: %u\n", __func__,
			 dp_subif->port_id, dp_subif->subif_groupid);
		*portid = dp_subif->port_id;
		*subif = dp_subif->subif_groupid;
		kfree(dp_subif);
		break;
	default:
		pr_err("%s: PCE type unknown\n", __func__);
		return -EINVAL;
	}

	return 0;
}

static int
subblk_id_find(struct net_device *dev,
	       GSW_PCE_RuleRegion_t region,
	       int portid, int subif)
{
	struct pce_node *p;

	list_for_each_entry(p, &tc_pce_list, list) {
		struct pce_entry *entry = p->entry;
		struct dp_subblk_info *info = &entry->blk_info.info;

		if (entry->blk_info.region == GSW_PCE_RULE_COMMMON &&
		    entry->blk_info.region == region) {
			netdev_dbg(dev, "%s: subblk_id found in COMMON region: %d\n",
				   __func__, info->subblk_id);
			return info->subblk_id;
		}
		if (entry->blk_info.region == GSW_PCE_RULE_CTP &&
		    entry->blk_info.region == region &&
		    info->portid == portid &&
		    info->subif == subif) {
			netdev_dbg(dev, "%s: subblk_id found in CTP region(%d|%d): %d\n",
				   __func__, info->portid, info->subif,
				   info->subblk_id);
			return info->subblk_id;
		}
	}

	netdev_dbg(dev, "%s: subblk_id not found\n",
		   __func__);
	return 0;
}

/* This function returns a pce_entry which has the same pattern. This is used
 * to configure merge entries with the same pattern. Only one mirred action
 * for example is used, but if we want to send the traffic to two bridge ports
 * we need one rule with two destinations.
 */
struct pce_entry *
qos_tc_pce_find_similar(const GSW_PCE_rule_t *pce_rule)
{
	struct pce_node *p;

	list_for_each_entry(p, &tc_pce_list, list) {
		GSW_PCE_rule_t *rule = &p->entry->pce_rule;
		GSW_PCE_action_t *action = &rule->action;
		const GSW_PCE_action_t *pce_action = &pce_rule->action;

		if (memcmp(&rule->pattern, &pce_rule->pattern,
			   sizeof(pce_rule->pattern)) == 0 &&
		    rule->logicalportid == pce_rule->logicalportid &&
		    rule->subifidgroup == pce_rule->subifidgroup &&
		    action->ePortMapAction == pce_action->ePortMapAction &&
		    action->ePortFilterType_Action ==
					pce_action->ePortFilterType_Action) {
			return p->entry;
		}
	}
	return  NULL;
}

static int qos_tc_merge_port_map(struct net_device *dev,
				 GSW_PCE_rule_t *pce_rule1,
				 const GSW_PCE_rule_t *pce_rule2)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(pce_rule1->action.nForwardPortMap) ; i++)
		pce_rule1->action.nForwardPortMap[i] |= pce_rule2->action.nForwardPortMap[i];

	return 0;
}

static struct pce_entry *create_pce_entry(struct net_device *dev,
					  enum pce_type type,
					  const GSW_PCE_rule_t *pce_rule,
					  int pref)
{
	struct pce_entry *tc_pce_entry;
	struct dp_pce_blk_info *pce_blk_info;
	int portid, subif;
	int ret;

	ret = get_subif_data(dev, &type, &portid, &subif);
	if (ret != 0)
		return NULL;

	tc_pce_entry = kzalloc(sizeof(*tc_pce_entry), GFP_KERNEL);
	if (!tc_pce_entry)
		return NULL;

	pce_blk_info = &tc_pce_entry->blk_info;

	INIT_LIST_HEAD(&tc_pce_entry->node_list);

	memcpy(&tc_pce_entry->pce_rule, pce_rule,
	       sizeof(tc_pce_entry->pce_rule));

	if (type != PCE_COMMON) {
		tc_pce_entry->pce_rule.logicalportid = portid;
		tc_pce_entry->pce_rule.subifidgroup = subif;
		tc_pce_entry->pce_rule.region = GSW_PCE_RULE_CTP;

		pce_blk_info->info.portid = portid;
		pce_blk_info->info.subif = subif;
		pce_blk_info->region = GSW_PCE_RULE_CTP;
		strlcpy(pce_blk_info->info.subblk_name, "MIRRED",
			sizeof(pce_blk_info->info.subblk_name));
	}

	/* All PCE rules for each region should be put in the same subblock,
	 * take subblk_id from the first rule on the list. If there are no
	 * rules in storage, leave subblk_id equal to 0, to make DP create new
	 * subblock. */
	pce_blk_info->info.subblk_id = subblk_id_find(dev,
						tc_pce_entry->pce_rule.region,
						portid, subif);

	if (type == PCE_COMMON)
		pce_blk_info->info.subblk_size = TC_PCE_GLB_SUBBLK_SIZE;
	else
		pce_blk_info->info.subblk_size = TC_PCE_CTP_SUBBLK_SIZE;

	pce_blk_info->info.prio = pref;

	return tc_pce_entry;
}

int qos_tc_pce_rule_create(struct net_device *dev,
			   unsigned long cookie, int pref,
			   enum pce_type type,
			   const GSW_PCE_rule_t *pce_rule)
{
	struct pce_node *tc_pce = NULL;
	struct pce_entry *tc_pce_entry;
	int ret = 0;

	/* Check here if there is already a rule with the same PCE pattern. */
	tc_pce_entry = qos_tc_pce_find_similar(pce_rule);
	if (tc_pce_entry) {
		/* This merges the bridge port forwarding maps. */
		ret = qos_tc_merge_port_map(dev, &tc_pce_entry->pce_rule,
					    pce_rule);
		if (ret)
			return ret;

		tc_pce_entry->pce_rule.pattern.nIndex = tc_pce_entry->index;
		ret = dp_pce_rule_update(&tc_pce_entry->blk_info,
					 &tc_pce_entry->pce_rule,
					 DP_PCE_RULE_REWRITE);
		if (ret < 0) {
			netdev_err(dev, "%s: dp_pce_rule_update failed err %d\n",
				   __func__, ret);
			return ret;
		}
		tc_pce_entry->pce_rule.pattern.nIndex = 0;
	} else {
		tc_pce_entry = create_pce_entry(dev, type, pce_rule, pref);
		if (!tc_pce_entry)
			return -ENOMEM;

		ret = dp_pce_rule_add(&tc_pce_entry->blk_info,
				      &tc_pce_entry->pce_rule);
		if (ret < 0) {
			netdev_err(dev, "%s: dp_pce_rule_add failed err %d\n",
				   __func__, ret);
			kfree(tc_pce_entry);
			return ret;
		}
		tc_pce_entry->index = ret;
		/* Set nIndex to initial value (0) here. We compare the
		 * complete pattern in the qos_tc_pce_find_similar() function
		 * and the pattern also contains the nIndex, the nIndex is the
		 * position of the PCE rule in the hardware, a new rule
		 * normally sets this to 0. To allow to match the rules set it
		 * here to 0 and store the value in our index variable.
		 */
		tc_pce_entry->pce_rule.pattern.nIndex = 0;
	}

	netdev_dbg(dev, "%s: pce rule added with index %d\n", __func__,
		   tc_pce_entry->index);

	tc_pce = kzalloc(sizeof(*tc_pce), GFP_KERNEL);
	if (!tc_pce) {
		if (list_empty(&tc_pce->pce_list)) {
			qos_tc_pce_rule_delete_node(tc_pce_entry,
						    tc_pce_entry->index);
			kfree(tc_pce_entry);
		}
		return -ENOMEM;
	}

	tc_pce->cookie = cookie;
	tc_pce->pref = pref;
	tc_pce->entry = tc_pce_entry;
	list_add(&tc_pce->pce_list, &tc_pce_entry->node_list);
	memcpy(tc_pce->nForwardPortMap, pce_rule->action.nForwardPortMap,
	       sizeof(tc_pce->nForwardPortMap));
	list_add(&tc_pce->list, &tc_pce_list);

	pr_debug("%s: PCE rule created for lpid/gpid\n", __func__);
	return 0;
}

int qos_tc_pce_rule_delete(unsigned long cookie, int pref)
{
	struct pce_node *p, *n;
	struct pce_entry *tc_pce_entry;
	GSW_PCE_action_t *action;
	bool deletion = false;
	int ret, i;

	list_for_each_entry_safe(p, n, &tc_pce_list, list) {
		if (cookie == p->cookie && pref == p->pref) {
			deletion = true;
			break;
		}
	}

	if (!deletion) {
		pr_debug("%s: Nothing to delete\n", __func__);
		return -ENOENT;
	}
	tc_pce_entry = p->entry;
	action = &tc_pce_entry->pce_rule.action;

	list_del(&p->pce_list);
	if (list_empty(&tc_pce_entry->node_list)) {
		ret = qos_tc_pce_rule_delete_node(tc_pce_entry,
						  tc_pce_entry->index);
		if (ret < 0)
			return ret;
		kfree(tc_pce_entry);
	} else {
		/* Create a new nForwardPortMap for the new PCE rule without
		 * the tc rule which should be removed here. This new PCE rule
		 * will have all the bridge ports set which are set in all the
		 * remaining tc rules which use this PCE rule.
		 */
		memset(action->nForwardPortMap, 0x0,
		       sizeof(action->nForwardPortMap));
		list_for_each_entry(n, &tc_pce_entry->node_list, pce_list) {
			for (i = 0; i < ARRAY_SIZE(action->nForwardPortMap);
			     i++)
				action->nForwardPortMap[i] |= n->nForwardPortMap[i];
		}

		tc_pce_entry->pce_rule.pattern.nIndex = tc_pce_entry->index;
		ret = dp_pce_rule_update(&tc_pce_entry->blk_info,
					 &tc_pce_entry->pce_rule,
					 DP_PCE_RULE_REWRITE);
		if (ret < 0) {
			pr_err("%s: dp_pce_rule_update failed err %d\n",
			       __func__, ret);
			return -EINVAL;
		}
		tc_pce_entry->pce_rule.pattern.nIndex = 0;
	}

	list_del(&p->list);
	kfree(p);

	pr_debug("%s: PCE rule deleted\n", __func__);
	return 0;
}

static int qos_tc_pce_rule_delete_node(struct pce_entry *p, u32 index)
{
	struct dp_pce_blk_info *pce_blk_info = &p->blk_info;
	GSW_PCE_ruleDelete_t pce_rule = {0};
	GSW_return_t ret;

	if (pce_blk_info->info.subif != QOS_TC_PCE_BLOCK_COMMON &&
	    pce_blk_info->info.portid != QOS_TC_PCE_BLOCK_COMMON) {
		pce_rule.logicalportid = pce_blk_info->info.portid;
		pce_rule.subifidgroup = pce_blk_info->info.subif;
		pce_rule.region = pce_blk_info->region;
	}

	pce_rule.nIndex = index;

	ret = dp_pce_rule_del(pce_blk_info, &pce_rule);
	if (ret < 0) {
		pr_err("%s: dp_pce_rule_del failed err %d\n", __func__, ret);
		return ret;
	}
	pr_debug("%s: deleted pce rule with index %d\n", __func__,
		 pce_rule.nIndex);

	return 0;
}

static void print_dp_subblk_info(struct seq_file *file,
				 struct dp_subblk_info p)
{
	seq_puts(file, "\t\tsubblk_info:\n");

	seq_printf(file, "\t\t\tportid: %d\n", p.portid);
	seq_printf(file, "\t\t\tsubif: %d\n", p.subif);
	seq_printf(file, "\t\t\tbp: %d\n", p.bp);

	seq_printf(file, "\t\t\tsubblk_protected: %s\n",
		   p.subblk_protected ? "true" : "false");

	seq_printf(file, "\t\t\tsubblk_id: %d\n", p.subblk_id);
	seq_printf(file, "\t\t\tsubblk_firstidx: %d\n", p.subblk_firstidx);
	seq_printf(file, "\t\t\tsubblk_size: %d\n", p.subblk_size);

	seq_printf(file, "\t\t\tsubblk_name: %s\n", p.subblk_name);

	seq_printf(file, "\t\t\tprio: %d\n", p.prio);
}

static void print_dp_pce_blk_info(struct seq_file *file,
				  struct dp_pce_blk_info p)
{
	seq_puts(file, "\tblk_info:\n");

	seq_printf(file, "\t\tinst: %d\n", p.inst);

	switch (p.region) {
	case GSW_PCE_RULE_COMMMON:
		seq_puts(file, "\t\tregion: GSW_PCE_RULE_COMMMON\n");
		break;
	case GSW_PCE_RULE_CTP:
		seq_puts(file, "\t\tregion: GSW_PCE_RULE_CTP\n");
		break;
	case GSW_PCE_RULE_DEBUG:
		seq_puts(file, "\t\tregion: GSW_PCE_RULE_DEBUG\n");
		break;
	default:
		seq_puts(file, "\t\tregion: unknown\n");
		break;
	}

	print_dp_subblk_info(file, p.info);
}

static void print_pce_entry(struct seq_file *file,
			    struct pce_entry *p)
{
	seq_puts(file, "pce rule:\n");

	seq_printf(file, "\tindex: %u\n", p->index);

	print_dp_pce_blk_info(file, p->blk_info);
}

static void print_tc_pce_list_node(struct seq_file *file,
				   struct pce_node *p)
{
	int i;

	seq_printf(file, "cookie: %#lx\n", p->cookie);
	seq_printf(file, "pref: %d\n", p->pref);

	seq_puts(file, "nForwardPortMap: ");
	for (i = 0; i < ARRAY_SIZE(p->nForwardPortMap); i++)
		seq_printf(file, "%x", p->nForwardPortMap[i]);

	seq_puts(file, "\n");

	print_pce_entry(file, p->entry);

	seq_puts(file, "\n");
}

void qos_tc_pce_list_debugfs(struct seq_file *file, void *ctx)
{
	struct pce_node *p, *n;

	rtnl_lock();
	list_for_each_entry_safe(p, n, &tc_pce_list, list) {
		print_tc_pce_list_node(file, p);
	}
	rtnl_unlock();
}
