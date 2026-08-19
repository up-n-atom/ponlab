// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/
#include <net/pkt_cls.h>
#include <linux/list.h>
#include <linux/version.h>
#include <net/datapath_api.h>
#include <net/flow_dissector.h>
#include <net/tc_act/tc_mirred.h>
#include <net/tc_act/tc_gact.h>
#include <net/qos_tc.h>
#include "qos_tc_qos.h"
#include "qos_tc_flower.h"

struct flower_cls_map {
	__be16 proto;
	u32 pref;
	u32 classid;
	u32 qid;
	char tc_cookie;
	bool ingress;
	struct flow_dissector_key_vlan key;
	struct flow_dissector_key_vlan mask;
	struct net_device *dev;
	int in_ifi;
	int tc;
	int subif;
	struct list_head list;
};

static LIST_HEAD(tc_class_list);

static int qos_tc_parse_flower_action(struct flow_cls_offload *f,
				      char *tc_cookie)
{
	const struct flow_action_entry *a;
	int i;
	int act_nr = 0;
	int act_ok_nr = 0;
	int offset;

	if (f->rule->action.num_entries == 0)
		return -EINVAL;

	flow_action_for_each(i, a, &f->rule->action) {
		if (a->id == FLOW_ACTION_ACCEPT)
			act_ok_nr++;
		act_nr++;

		if (a->cookie) {
			/* Maximum supported value is 15, so there is not need
			 * to store whole act_cookie->data. Copying whole
			 * unsigned char to signed char with such low values
			 * won't cause any conversion problem, and in addition
			 * will reset sign bit, which later will be used to
			 * check if cookie was set or not.
			 */
			offset = a->cookie->cookie_len - sizeof(*tc_cookie);
			memcpy(tc_cookie, a->cookie->cookie + offset,
			       sizeof(*tc_cookie));
		}
	}

	if (act_nr != 1 && act_ok_nr != 1)
		return -EINVAL;

	return 0;
}

static int qos_tc_parse_tc_flower(struct flow_cls_offload *f,
				  struct flower_cls_map **flt,
				  bool ingress)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);

	*flt = kzalloc(sizeof(**flt), GFP_KERNEL);
	if (!*flt)
		return -ENOMEM;

	if (d->used_keys &
			~(BIT(FLOW_DISSECTOR_KEY_CONTROL) |
				BIT(FLOW_DISSECTOR_KEY_BASIC) |
				BIT(FLOW_DISSECTOR_KEY_ETH_ADDRS) |
				BIT(FLOW_DISSECTOR_KEY_VLAN) |
				BIT(FLOW_DISSECTOR_KEY_IPV4_ADDRS) |
				BIT(FLOW_DISSECTOR_KEY_IPV6_ADDRS) |
				BIT(FLOW_DISSECTOR_KEY_IP) |
				BIT(FLOW_DISSECTOR_KEY_META) |
				BIT(FLOW_DISSECTOR_KEY_PORTS))) {
		pr_debug("%s: Unsupported key used: 0x%x\n", __func__,
			 d->used_keys);
		kfree(*flt);
		return -EOPNOTSUPP;
	}
	pr_debug("%s: Supported key used: 0x%x\n", __func__, d->used_keys);

	(*flt)->pref = f->common.prio >> 16;
	(*flt)->proto = f->common.protocol;
	(*flt)->classid = f->classid;
	(*flt)->ingress = ingress;

	/* Classification/Matching arguments parsing */
	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_VLAN)) {
		struct flow_dissector_key_vlan *key =
			skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_VLAN,
						  qos_tc_get_key(f));
		struct flow_dissector_key_vlan *mask =
			skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_VLAN,
						  qos_tc_get_mask(f));
		pr_debug("%s: match vid: %#x/%#x pcp: %#x\n", __func__,
			 key->vlan_id,
			 key->vlan_priority,
			 mask->vlan_id);
		(*flt)->key = *key;
		(*flt)->mask = *mask;
	}

	return 0;
}

static int qos_tc_get_subif_idx(struct net_device *dev, int *subif)
{
	dp_subif_t *dp_subif;
	int ret;

	if (!dev)
		return -EINVAL;

	if (qos_tc_is_netdev_cpu_port(dev)) {
		*subif = 0;
		return 0;
	}

	dp_subif = kzalloc(sizeof(*dp_subif), GFP_KERNEL);
	if (!dp_subif) {
		netdev_err(dev, "%s: failed to allocate memory for subif\n",
			   __func__);
		return -ENOMEM;
	}

	ret = dp_get_netif_subifid(dev, NULL, NULL, 0, dp_subif, 0);
	if (ret < 0) {
		netdev_err(dev, "%s: subif idx get failed\n", __func__);
		kfree(dp_subif);
		return -ENODEV;
	}

	*subif = dp_subif->subif;
	kfree(dp_subif);

	return 0;
}

static int __qos_tc_map(struct net_device *dev,
			struct flower_cls_map *map,
			struct net_device *indev,
			int tc, bool en,
			const struct qos_tc_params *tc_params)
{
	struct qos_tc_qmap_tc qmap = { 0 };
	int ret;

	qmap.tc = TC_VAL_INV;
	qmap.subif = TC_VAL_INV;

	qmap.indev = indev;

	if (qmap.indev) {
		ret = qos_tc_get_subif_idx(qmap.indev, &qmap.subif);
		if (ret < 0)
			return -ENODEV;
	}

	if (map->mask.vlan_priority)
		qmap.tc = map->key.vlan_priority;

	qmap.handle = map->classid;
	qmap.tc_cookie = map->tc_cookie;
	qmap.ingress = map->ingress;
	ret = qos_tc_update_qmap(dev, &qmap, true, &map->qid, tc_params);
	if (ret < 0) {
		netdev_err(dev, "%s: queue map fail (%d)\n", __func__, ret);
		return -EINVAL;
	}

	map->tc = qmap.tc;
	map->subif = qmap.subif;

	return 0;
}

int qos_tc_map(struct net_device *dev, struct flow_cls_offload *f,
	       bool ingress,
	       const struct qos_tc_params *tc_params)
{
	struct flower_cls_map *map = NULL;
	struct net_device *indev;
	char tc_cookie = QOS_TC_COOKIE_EMPTY;
	int ret = 0;

	indev = qos_tc_get_indev(dev, f);

	ret = qos_tc_parse_flower_action(f, &tc_cookie);
	if (ret < 0)
		return ret;

	ret = qos_tc_parse_tc_flower(f, &map, ingress);
	if (ret < 0)
		return ret;

	if (!indev && !map->mask.vlan_priority &&
	    tc_cookie == QOS_TC_COOKIE_EMPTY) {
		netdev_err(dev, "%s: no class specified\n", __func__);
		ret = -EINVAL;
		goto err;
	}

	map->dev = dev;
	if (indev)
		map->in_ifi = indev->ifindex;
	map->tc_cookie = tc_cookie;

	ret = __qos_tc_map(dev, map, indev, 0, true, tc_params);
	if (ret < 0) {
		netdev_err(dev, "%s: queue map fail\n", __func__);
		goto err;
	}

	ret = qos_tc_flower_storage_add(dev, f->cookie, TC_TYPE_QUEUE, map,
					NULL);
	if (ret < 0)
		goto err;

	list_add(&map->list, &tc_class_list);

	return 0;

err:
	kfree(map);

	return ret;
}

static int __qos_tc_unmap(struct net_device *dev,
			  struct flower_cls_map *map,
			  struct net_device *indev,
			  const struct qos_tc_params *tc_params)
{
	struct qos_tc_qmap_tc qmap = { 0 };
	int ret;

	qmap.indev = indev;
	qmap.tc = map->tc;
	qmap.subif = map->subif;

	/* when Q is removed we should unmap the same classid */
	qmap.handle = map->classid;
	qmap.tc_cookie = map->tc_cookie;
	qmap.ingress = map->ingress;
	ret = qos_tc_update_qmap(dev, &qmap, false, &map->qid, tc_params);
	if (ret < 0) {
		netdev_err(dev, "%s: queue unmap fail (%d)\n", __func__, ret);
		return -EINVAL;
	}

	map->tc = qmap.tc;
	map->subif = qmap.subif;

	return 0;
}

int qos_tc_unmap(struct net_device *dev, void *list_node,
		const struct qos_tc_params *tc_params)
{
	struct flower_cls_map *map;
	struct net_device *indev = NULL;
	int ret = 0;

	if (!list_node)
		return -EIO;

	map = list_node;

	/* The interface referenced by in_ifi may already be deleted
	 * from the system here. Unconfigure the system like there would
	 * have been no indev when we cannot find it. The SW flower code
	 * also ignores the case when the indev gets deleted before the
	 * filter is removed. In fl_dump() it would just not add the
	 * indev any more if it was deleted before.
	 */
	if (map->in_ifi)
		indev = __dev_get_by_index(dev_net(dev), map->in_ifi);

	ret = __qos_tc_unmap(dev, map, indev, tc_params);
	if (ret < 0) {
		netdev_err(dev, "%s: queue unmap fail (%d)\n", __func__, ret);
		return ret;
	}

	list_del(&map->list);
	kfree(map);
	return 0;
}

int qos_tc_classid_unmap(u32 classid)
{
	struct flower_cls_map *p, *n;
	int ret;

	list_for_each_entry_safe(p, n, &tc_class_list, list) {
		if (p->classid != classid)
			continue;

		ret = qos_tc_unmap(p->dev, p, NULL);
		if (ret < 0) {
			netdev_err(p->dev, "%s: queue unmap fail (%d)\n",
				   __func__, ret);
			return ret;
		}
	}

	return 0;
}

static void print_flower_cls_map(struct seq_file *file,
				 struct flower_cls_map *p)
{
	seq_printf(file, "dev: %s\n", p->dev->name);

	seq_printf(file, "proto: %d\n", p->proto);
	seq_printf(file, "pref: %u\n", p->pref);
	seq_printf(file, "classid: %#x\n", p->classid);
	seq_printf(file, "tc_cookie: %d\n", p->tc_cookie);
	seq_printf(file, "qid: %u\n", p->qid);
	seq_printf(file, "ingress: %s\n", p->ingress ? "true" : "false");

	seq_printf(file, "match vid: %#x/%#x\npcp: %#x\n",
		   p->key.vlan_id,
		   p->key.vlan_priority,
		   p->mask.vlan_id);

	seq_printf(file, "in_ifi: %d\n", p->in_ifi);
	seq_printf(file, "tc: %d\n", p->tc);

	seq_puts(file, "\n");
}

void qos_tc_class_list_debugfs(struct seq_file *file, void *ctx)
{
	struct flower_cls_map *p, *n;

	rtnl_lock();
	list_for_each_entry_safe(p, n, &tc_class_list, list) {
		print_flower_cls_map(file, p);
	}
	rtnl_unlock();
}
