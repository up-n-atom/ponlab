// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2023-2024 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
#include <linux/pp_api.h>
#endif /* CONFIG_DPM_DATAPATH_HAL_GSWIP32 */
#ifdef CONFIG_NETWORK_EXTMARK
#include <linux/extmark.h>
#endif
#include <net/directpath_api.h>
#include "directpath.h"

/* If device has these features enabled, they must be disabled for directpath */
#define NETIF_F_DPDP_DISABLES	(NETIF_F_VLAN_FEATURES |		\
				 NETIF_F_CSUM_MASK | NETIF_F_RXCSUM |	\
				 NETIF_F_ALL_TSO | NETIF_F_GRO_HW)

static DEFINE_READ_MOSTLY_HASHTABLE(dev_list, 8);
static DEFINE_MUTEX(dev_list_lock);

static u32 device_key_get(struct dpdp_device *dp_dev)
{
	return dp_dev->dp_subif.gpid;
}

static u32 handle_key_get(struct dpdp_handle *handle)
{
	return handle->gpid;
}

static void dpdp_device_add(struct dpdp_device *dp_dev)
{
	int bkt = device_key_get(dp_dev) % HASH_SIZE(dev_list);

	mutex_lock(&dev_list_lock);
	hlist_add_head_rcu(&dp_dev->hlist, &dev_list[bkt]);
	mutex_unlock(&dev_list_lock);
}

static void dpdp_device_del(struct dpdp_device *dp_dev)
{
	mutex_lock(&dev_list_lock);
	hlist_del_init_rcu(&dp_dev->hlist);
	mutex_unlock(&dev_list_lock);
}

static void dpdp_device_rehash_add(struct dpdp_device *dp_dev)
{
	int bkt = device_key_get(dp_dev) % HASH_SIZE(dev_list);

	mutex_lock(&dev_list_lock);
	hlist_del_init_rcu(&dp_dev->hlist);
	hlist_add_head_rcu(&dp_dev->hlist, &dev_list[bkt]);
	mutex_unlock(&dev_list_lock);
}

static struct dpdp_device *dpdp_device_find_by_dev(struct net_device *dev)
{
	struct dpdp_device *dp_dev;
	int bkt;

	for (bkt = 0; bkt < HASH_SIZE(dev_list); bkt++)
		hlist_for_each_entry_rcu(dp_dev, &dev_list[bkt], hlist)
			if (dp_dev->dev == dev)
				return dp_dev;

	return NULL;
}

static struct dpdp_device *dpdp_device_find_by_port(int port_id)
{
	struct dpdp_device *dp_dev;
	int bkt;

	for (bkt = 0; bkt < HASH_SIZE(dev_list); bkt++)
		hlist_for_each_entry_rcu(dp_dev, &dev_list[bkt], hlist)
			if (dp_dev->pinfo->dp_port == port_id)
				return dp_dev;

	return NULL;
}

static struct dpdp_device *dpdp_device_find_by_key(struct net_device *dev, u32 key)
{
	struct dpdp_device *dp_dev = NULL;
	int bkt = key % HASH_SIZE(dev_list);

	hlist_for_each_entry_rcu(dp_dev, &dev_list[bkt], hlist)
		if (dp_dev->dev == dev)
			return dp_dev;

	return NULL;
}

void dpdp_device_dump(dump_cb_t *cb, void *data)
{
	struct dpdp_device *dp_dev = NULL;
	int bkt;

	rcu_read_lock();
	for (bkt = 0; bkt < HASH_SIZE(dev_list); bkt++)
		hlist_for_each_entry_rcu(dp_dev, &dev_list[bkt], hlist)
			cb(dp_dev, bkt, data);
	rcu_read_unlock();
}

static struct dpdp_device *dpdp_device_alloc(struct module *owner,
					     struct net_device *dev)
{
	struct dpdp_device *dp_dev;

	dp_dev = kzalloc(sizeof(*dp_dev), GFP_KERNEL);
	if (!dp_dev)
		return NULL;

	dp_dev->stats = alloc_percpu(struct dpdp_stats);
	if (!dp_dev->stats) {
		kfree(dp_dev);
		return NULL;
	}

	dp_dev->pinfo = kzalloc(sizeof(*dp_dev->pinfo), GFP_KERNEL);
	if (!dp_dev->pinfo) {
		free_percpu(dp_dev->stats);
		kfree(dp_dev);
		return NULL;
	}

	dp_dev->pinfo->owner = owner;
	dp_dev->pinfo->dev = dev;
	dp_dev->pinfo->dp_port = -1;
	refcount_set(&dp_dev->pinfo->users, 1);

	INIT_HLIST_NODE(&dp_dev->hlist);
	dp_dev->dev = dev;
	dp_dev->dp_subif.port_id = -1;
	dp_dev->dp_subif.subif = -1;
	refcount_set(&dp_dev->users, 1);

	dpdp_device_add(dp_dev);

	netdev_dbg(dev, "%s: device allocated\n", __func__);
	return dp_dev;
}

static struct dpdp_device *dpdp_device_clone(struct net_device *dev,
					     struct dpdp_handle *handle)
{
	struct dpdp_device *dp_dev, *n;

	dp_dev = dpdp_device_find_by_dev(dev);
	if (dp_dev) {
		refcount_inc(&dp_dev->users);
		return dp_dev;
	}

	dp_dev = dpdp_device_find_by_port(handle->port_id);
	if (!dp_dev) {
		netdev_err(dev, "%s: not allowed, dp_dev NULL\n", __func__);
		return NULL;
	}

	n = kzalloc(sizeof(*dp_dev), GFP_KERNEL);
	if (!n)
		return NULL;

	n->stats = alloc_percpu(struct dpdp_stats);
	if (!n->stats) {
		kfree(n);
		return NULL;
	}

	n->pinfo = dp_dev->pinfo;
	refcount_inc(&n->pinfo->users);

	INIT_HLIST_NODE(&n->hlist);
	n->dev = dev;
	n->dp_subif.port_id = n->pinfo->dp_port;
	n->dp_subif.subif = -1;
	refcount_set(&n->users, 1);

	dpdp_device_add(n);

	netdev_dbg(dev, "%s: device cloned\n", __func__);
	return n;
}

static inline bool dpdp_device_running(const struct dpdp_device *dp_dev)
{
	return (dp_dev && dp_dev->subif_active);
}

static void dpdp_device_free(struct dpdp_device *dp_dev)
{
	if (!dp_dev)
		return;

	if (!refcount_dec_and_test(&dp_dev->users))
		return;

	if (dp_dev->subif_active)
		netdev_warn(dp_dev->dev,
			    "%s: port %d subif 0x%04x still active\n",
			    __func__, dp_dev->dp_subif.port_id,
			    dp_dev->dp_subif.subif);

	if (refcount_dec_and_test(&dp_dev->pinfo->users)) {
		if (dp_dev->pinfo->port_active)
			netdev_warn(dp_dev->dev, "%s: port %d still active\n",
				    __func__, dp_dev->pinfo->dp_port);
		kfree(dp_dev->pinfo);
	}

	dpdp_device_del(dp_dev);

	netdev_dbg(dp_dev->dev, "%s: device freed\n", __func__);
	free_percpu(dp_dev->stats);
	kfree(dp_dev);
}

static struct dp_pmac_cfg *dpdp_pmac_cfg_get(struct dp_pmac_cfg *pmac_cfg)
{
	return NULL;
}

static void dpdp_pkt_parse_early(struct sk_buff *skb, struct net_device *dev)
{
#ifdef CONFIG_PPA_EXT_PKT_LEARNING
	struct packet_type *ptype;

	rcu_read_lock();
	list_for_each_entry_rcu(ptype, &dev->ptype_all, list) {
		if (!ptype->af_packet_priv) {
			/* deliver to learning driver's parse handler */
			skb->pkt_type = PKTPRS_ETH_RX;
			ptype->func(skb, skb->dev, NULL, dev);
			break;
		}
	}
	rcu_read_unlock();
#endif
}

static int dpdp_eth_header_push(struct sk_buff *skb,
				const u8 *dest, const u8 *source, __be16 proto)
{
	if (skb_headroom(skb) < ETH_HLEN)
		return -EPERM;

	skb_reset_network_header(skb);
	skb_push(skb, ETH_HLEN);
	skb_reset_mac_header(skb);
	ether_addr_copy(eth_hdr(skb)->h_dest, dest);
	ether_addr_copy(eth_hdr(skb)->h_source, source);
	eth_hdr(skb)->h_proto = proto;
	return 0;
}

static void dpdp_eth_header_pop_rx_slow(struct sk_buff *skb, struct net_device *dev)
{
	skb->protocol = eth_type_trans(skb, dev);
	dpdp_pkt_parse_early(skb, dev);
	skb->pkt_type = PACKET_HOST;
	skb_reset_mac_header(skb);
	skb_reset_network_header(skb);
}

static void dpdp_eth_header_pop_tx(struct sk_buff *skb, struct net_device *dev)
{
	skb->protocol = eth_type_trans(skb, dev);
	skb->pkt_type = PACKET_OUTGOING;
	skb_reset_mac_header(skb);
	skb_reset_network_header(skb);
}

static int dpdp_rx_slow(struct sk_buff *skb, struct net_device *dev)
{
	int err;

	if (!netif_running(dev)) {
		net_dbg_ratelimited("%s(%s): drop\n",
				    __func__, netdev_name(dev));
		dev_kfree_skb_any(skb);
		return -1;
	}

	if (!dev_is_mac_header_xmit(dev))
		dpdp_eth_header_pop_rx_slow(skb, dev);
	else
		skb->protocol = eth_type_trans(skb, dev);
	err = netif_rx(skb);
	net_dbg_ratelimited("%s(%s): netif_rx status %d\n",
			    __func__, netdev_name(dev), err);
	return err;
}

static int dpdp_start_xmit(struct sk_buff *skb,
			   struct net_device *dev)
{
	int err;

	if (!dev_is_mac_header_xmit(dev)) {
		dpdp_eth_header_pop_tx(skb, dev);
	} else {
		skb->dev = dev;
		skb_reset_mac_header(skb);
		skb->protocol = eth_hdr(skb)->h_proto;
		skb_set_network_header(skb, ETH_HLEN);
	}
#ifdef CONFIG_NETWORK_EXTMARK
	skb_extmark_set(skb, FILTERTAP_MASK, FILTERTAP_MASK);
#endif
	err = dev_queue_xmit(skb);
	net_dbg_ratelimited("%s(%s): dev_queue_xmit status %d\n",
			    __func__, netdev_name(dev), err);
	return err;
}

static int dpdp_rxtx_handler_dflt(struct net_device *rxdev,
				  struct net_device *txdev,
				  struct sk_buff *skb, int len)
{
	if (txdev)
		return dpdp_start_xmit(skb, txdev);
	else
		return dpdp_rx_slow(skb, rxdev);
}

static void dpdp_rxtx_handler_check(struct sk_buff *skb,
				    struct net_device **rxdev,
				    struct net_device **txdev,
				    struct dpdp_handle *handle)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	struct dma_tx_desc_1 *desc_1 = (struct dma_tx_desc_1 *)&skb->DW1;

	handle->gpid = desc_1->field.ep;

	/* NOTE: hack to adjust egflag reset in slowpath egress qos flow */
	if (!desc_1->field.redir) {
		struct pp_desc *pp_desc;

		pp_desc = pp_pkt_desc_get(skb);
		if (!pp_desc) {
			desc_1->field.redir = 1; /* egflag=1 */
			*txdev = *rxdev;
			*rxdev = NULL;
		} else {
			handle->gpid = pp_desc->ud.rx_port;
		}
	}
#endif /* CONFIG_DPM_DATAPATH_HAL_GSWIP32 */
}

static dp_rx_fn_t dpdp_rx_fn_get(struct dpdp_device *dp_dev)
{
	return dp_dev->pinfo->dp_ops.rx_fn;
}

static int dpdp_rxtx_handler(struct net_device *rxdev,
			     struct net_device *txdev,
			     struct sk_buff *skb, int len)
{
	int err;
	struct dpdp_device *dp_dev;
	struct net_device *dev = txdev ? txdev : rxdev;
	dp_rx_fn_t rx_fn;
	struct dpdp_handle handle = {0};

	dpdp_rxtx_handler_check(skb, &rxdev, &txdev, &handle);

	dp_dev = dpdp_device_find_by_key(dev, handle_key_get(&handle));
	if (!dp_dev)
		goto drop;

	rx_fn = dpdp_rx_fn_get(dp_dev);
	if (rx_fn) {
		if (!dev_is_mac_header_xmit(dev)) {
			if (txdev)
				dpdp_eth_header_pop_tx(skb, txdev);
			else if (rxdev)
				dpdp_eth_header_pop_rx_slow(skb, rxdev);
		}

		err = rx_fn(rxdev, txdev, skb, skb->len);
	} else {
		err = dpdp_rxtx_handler_dflt(rxdev, txdev, skb, skb->len);
	}

	if (!err) {
		if (txdev)
			this_cpu_inc(dp_dev->stats->rx_fn_txif_pkts);
		else
			this_cpu_inc(dp_dev->stats->rx_fn_rxif_pkts);
	} else {
		if (txdev)
			this_cpu_inc(dp_dev->stats->rx_fn_txif_drop);
		else
			this_cpu_inc(dp_dev->stats->rx_fn_rxif_drop);
	}
	return 0;

drop:
	net_err_ratelimited("%s: drop\n", __func__);
	dev_kfree_skb_any(skb);
	return -1;
}

static int dpdp_dev_register(struct module *owner, struct net_device *dev,
			     struct dpdp_handle *handle, struct dp_cb *ops,
			     int max_subif)
{
	int err;
	int dp_port;
	struct dp_pmac_cfg pmac_cfg = {0};
	struct dp_cb dp_ops = {0};
	struct dp_dev_data dp_data = {0};
	struct dpdp_device *dp_dev;

	dp_dev = dpdp_device_find_by_dev(dev);
	if (!dp_dev)
		dp_dev = dpdp_device_alloc(owner, dev);

	if (!dp_dev || !dp_dev->pinfo) {
		pr_err("%s: not allowed, dp_dev or dp_dev->pinfo NULL\n",
		       __func__);
		dpdp_device_free(dp_dev);
		return -EEXIST;
	}

	if (dp_dev->pinfo->port_active) {
		netdev_dbg(dp_dev->dev, "%s: port %d already active\n",
			   __func__, dp_dev->pinfo->dp_port);
		return 0;
	}

	dp_port = dp_alloc_port_ext(0, dp_dev->pinfo->owner,
				    dp_dev->pinfo->dev, 0, 0,
				    dpdp_pmac_cfg_get(&pmac_cfg),
				    NULL, DP_F_DIRECT);
	if (dp_port < 0) {
		netdev_err(dp_dev->dev, "%s: dp_alloc_port_ext status %d\n",
			   __func__, dp_port);
		err = dp_port;
		goto out;
	}

	if (ops)
		memcpy(&dp_ops, ops, sizeof(dp_ops));
	dp_ops.rx_fn = dpdp_rxtx_handler;
	dp_data.max_ctp = max_subif;
	dp_data.max_gpid = max_subif;
	err = dp_register_dev_ext(0, dp_dev->pinfo->owner,
				  dp_port, &dp_ops, &dp_data, 0);
	if (err) {
		netdev_err(dp_dev->dev, "%s: dp_register_dev_ext status %d\n",
			   __func__, err);
		goto dealloc;
	}

	netdev_dbg(dp_dev->dev, "%s: port %d active\n", __func__, dp_port);

	dp_dev->pinfo->dp_port = dp_port;
	dp_dev->pinfo->port_active = 1;
	if (ops)
		memcpy(&dp_dev->pinfo->dp_ops, ops, sizeof(*ops));
	refcount_set(&dp_dev->pinfo->port_users, 0);

	handle->port_id = dp_port;
	return err;

dealloc:
	dp_alloc_port_ext(0, dp_dev->pinfo->owner, dp_dev->pinfo->dev, 0,
			  dp_port, NULL, NULL, DP_F_DEREGISTER);
out:
	dpdp_device_free(dp_dev);
	return err;
}

static int dpdp_dev_unregister(struct module *owner, struct net_device *dev,
			       struct dpdp_handle *handle)
{
	int err;
	struct dpdp_device *dp_dev;

	dp_dev = dpdp_device_find_by_dev(dev);
	if (!dp_dev || !dp_dev->pinfo) {
		pr_err("%s: not allowed, dp_dev or dp_dev->pinfo NULL\n",
		       __func__);
		return -EINVAL;
	}

	if (!dp_dev->pinfo->port_active) {
		netdev_dbg(dp_dev->dev, "%s: port %d not active yet\n",
			   __func__, dp_dev->pinfo->dp_port);
		return 0;
	}

	if (refcount_read(&dp_dev->pinfo->port_users)) {
		netdev_dbg(dp_dev->dev, "%s: port %d remain active still\n",
			   __func__, dp_dev->pinfo->dp_port);
		return 0;
	}

	err = dp_register_dev_ext(0, dp_dev->pinfo->owner,
				  dp_dev->pinfo->dp_port,
				  NULL, NULL, DP_F_DEREGISTER);
	if (err) {
		netdev_err(dp_dev->dev, "%s: dp_register_dev_ext status %d\n",
			   __func__, err);
		return err;
	}

	err = dp_alloc_port_ext(0, dp_dev->pinfo->owner, dp_dev->pinfo->dev, 0,
				dp_dev->pinfo->dp_port,
				NULL, NULL, DP_F_DEREGISTER);
	if (err)
		netdev_err(dp_dev->dev, "%s: dp_alloc_port_ext status %d\n",
			   __func__, err);

	netdev_dbg(dp_dev->dev, "%s: port %d inactivated\n", __func__,
		   dp_dev->pinfo->dp_port);

	dp_dev->pinfo->port_active = 0;
	dp_dev->pinfo->dp_port = -1;
	dpdp_device_free(dp_dev);

	return err;
}

static void dpdp_dev_features_disable(struct dpdp_device *dp_dev)
{
	bool locked = false;
	netdev_features_t changed_features;

	dp_dev->delta_features = dp_dev->dev->features & NETIF_F_DPDP_DISABLES;
	if (!dp_dev->delta_features)
		return;

	if (rtnl_trylock())
		locked = true;

	dp_dev->dev->wanted_features &= ~dp_dev->delta_features;
	netdev_update_features(dp_dev->dev);

	if (locked)
		rtnl_unlock();

	changed_features = dp_dev->dev->features & dp_dev->delta_features;
	if (unlikely(changed_features)) {
		netdev_warn(dp_dev->dev,
			    "%s: failed to disable netdev_features %pNF!\n",
			    __func__, &changed_features);
	} else {
		netdev_notice(dp_dev->dev,
			      "%s: disabled netdev_features %pNF\n",
			      __func__, &dp_dev->delta_features);
	}
}

static void dpdp_dev_features_restore(struct dpdp_device *dp_dev)
{
	bool locked = false;
	netdev_features_t changed_features;

	if (!dp_dev->delta_features)
		return;

	if (rtnl_trylock())
		locked = true;

	dp_dev->dev->wanted_features |= dp_dev->delta_features;
	netdev_update_features(dp_dev->dev);

	if (locked)
		rtnl_unlock();

	changed_features = ~dp_dev->dev->features & dp_dev->delta_features;
	if (unlikely(changed_features)) {
		netdev_warn(dp_dev->dev,
			    "%s: failed to enable netdev_features %pNF!\n",
			    __func__, &changed_features);
	} else {
		netdev_notice(dp_dev->dev, "%s: enabled netdev_features %pNF\n",
			      __func__, &dp_dev->delta_features);
	}
	dp_dev->delta_features = 0;
}

static void dpdp_subif_connect(struct dpdp_device *dp_dev)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	int err;
	struct dp_subif *dp_subif;
	struct dp_queue_map_set qmap = {0};

	if (dp_dev->dev->needed_headroom < sizeof(struct pmac_tx_hdr))
		dp_dev->dev->needed_headroom = sizeof(struct pmac_tx_hdr);

	dp_subif = kzalloc(sizeof(*dp_subif), GFP_KERNEL);
	if (!dp_subif) {
		netdev_err(dp_dev->dev, "%s: failed to allocate memory for subif\n",
			   __func__);
		return;
	}
	err = dp_get_netif_subifid(dp_dev->dev, NULL, NULL, NULL, dp_subif, 0);
	if (err) {
		netdev_err(dp_dev->dev, "%s: dp_get_netif_subifid status %d\n",
			   __func__, err);
		kfree(dp_subif);
		return;
	}

	dp_dev->dp_subif.gpid = dp_subif->gpid;

	/* set CQM default qmap for slowpath egress qos flow */
	qmap.q_id = dp_subif->def_qid;
	memset(&qmap.mask, 0xff, sizeof(qmap.mask));
	qmap.map.dp_port = dp_dev->pinfo->dp_port;
	qmap.mask.dp_port = 0;
	qmap.map.subif = dp_dev->dp_subif.subif;
	qmap.mask.subif = 0;
	qmap.map.egflag = 1;
	qmap.mask.egflag = 0;
	err = dp_queue_map_set(&qmap, 0);
	if (err) {
		netdev_err(dp_dev->dev, "%s: dp_queue_map_set status %d\n",
			   __func__, err);
	}
#endif /* CONFIG_DPM_DATAPATH_HAL_GSWIP32 */
	kfree(dp_subif);
}

static void dpdp_subif_disconnect(struct dpdp_device *dp_dev)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	dp_dev->dp_subif.gpid = 0;
#endif
}

static int dpdp_subif_register(struct net_device *dev,
			       struct dpdp_handle *handle)
{
	int err;
	struct dpdp_device *dp_dev;
	struct dp_subif_data dp_data = {0};

	dp_dev = dpdp_device_clone(dev, handle);
	if (!dp_dev || !dp_dev->pinfo) {
		netdev_err(dev, "%s: not allowed, dp_dev or dp_dev->pinfo NULL\n",
			   __func__);
		dpdp_device_free(dp_dev);
		return -EEXIST;
	}

	if (!dp_dev->pinfo->port_active) {
		netdev_err(dp_dev->dev, "%s: port %d inactive\n",
			   __func__, dp_dev->pinfo->dp_port);
		dpdp_device_free(dp_dev);
		return -EINVAL;
	}

	if (dp_dev->subif_active) {
		netdev_warn(dp_dev->dev, "%s: port %d subif 0x%04x active\n",
			    __func__, dp_dev->dp_subif.port_id,
			    dp_dev->dp_subif.subif);
		return 0;
	}

	/* disable platform TOE engine for the 3rd party device,
	 * which can have internal TOE engine.
	 */
	dp_data.flag_ops = DP_SUBIF_TOE;
	dp_data.toe_disable = 1;

	dp_dev->dp_subif.port_id = dp_dev->pinfo->dp_port;
	dp_dev->dp_subif.subif = handle->subif;
	err = dp_register_subif_ext(0, dp_dev->pinfo->owner, dp_dev->dev,
				    dp_dev->dev->name, &dp_dev->dp_subif,
				    &dp_data, 0);
	if (err) {
		netdev_err(dp_dev->dev, "%s: dp_register_subif_ext status %d\n",
			   __func__, err);
		dpdp_device_free(dp_dev);
		return err;
	}

	dpdp_subif_connect(dp_dev);
	dpdp_device_rehash_add(dp_dev);
	dpdp_dev_features_disable(dp_dev);

	/* create dummy ether address for L3 only type device */
	if (!dev_is_mac_header_xmit(dev)) {
		eth_random_addr(dp_dev->dummy_laddr);
		eth_random_addr(dp_dev->dummy_raddr);
	}

	netdev_dbg(dp_dev->dev, "%s: port %d subif 0x%04x active\n", __func__,
		   dp_dev->dp_subif.port_id, dp_dev->dp_subif.subif);

	dp_dev->subif_active = 1;
	if (refcount_read(&dp_dev->pinfo->port_users) == 0)
		refcount_set(&dp_dev->pinfo->port_users, 1);
	else
		refcount_inc(&dp_dev->pinfo->port_users);

	handle->subif = dp_dev->dp_subif.subif;
	handle->gpid = dp_dev->dp_subif.gpid;
	return 0;
}

static int dpdp_subif_unregister(struct net_device *dev,
				 struct dpdp_handle *handle)
{
	int err;
	struct dpdp_device *dp_dev;

	dp_dev = dpdp_device_find_by_dev(dev);
	if (!dp_dev || !dp_dev->pinfo) {
		pr_err("%s: not allowed, dp_dev or dp_dev->pinfo NULL\n",
		       __func__);
		return -EEXIST;
	}

	if (!dp_dev->subif_active) {
		netdev_dbg(dp_dev->dev, "%s: port %d subif 0x%04x inactive\n",
			   __func__, dp_dev->dp_subif.port_id,
			   dp_dev->dp_subif.subif);
		return 0;
	}

	dpdp_dev_features_restore(dp_dev);
	dpdp_subif_disconnect(dp_dev);
	dpdp_device_rehash_add(dp_dev);
	err = dp_register_subif_ext(0, dp_dev->pinfo->owner, dp_dev->dev,
				    dp_dev->dev->name, &dp_dev->dp_subif,
				    NULL, DP_F_DEREGISTER);
	if (err) {
		netdev_err(dp_dev->dev, "%s: dp_register_subif_ext status %d\n",
			   __func__, err);
		return err;
	}

	netdev_dbg(dp_dev->dev, "%s: port %d subif 0x%04x inactive\n", __func__,
		   dp_dev->dp_subif.port_id, dp_dev->dp_subif.subif);

	dp_dev->subif_active = 0;
	dp_dev->dp_subif.subif = -1;
	if (refcount_read(&dp_dev->pinfo->port_users) == 1)
		refcount_set(&dp_dev->pinfo->port_users, 0);
	else
		refcount_dec(&dp_dev->pinfo->port_users);
	dpdp_device_free(dp_dev);
	return err;
}

typedef int desc_set_t(struct dpdp_device *dp_dev,
		       struct sk_buff *skb);
static enum DP_TX_FN_RET dpdp_rxtx(struct dpdp_device *dp_dev,
				   struct sk_buff *skb,
				   desc_set_t *desc_set,
				   u32 flags)
{
	if (desc_set)
		desc_set(dp_dev, skb);

	return dp_xmit(skb->dev, &dp_dev->dp_subif,
		       skb, skb->len, flags);
}

static bool dpdp_tx_slow_active(struct dpdp_device *dp_dev,
				struct sk_buff *skb)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	struct dma_tx_desc_1 *desc_1 = (struct dma_tx_desc_1 *)&skb->DW1;

	return !(desc_1->field.redir &&
		 desc_1->field.ep == dp_dev->dp_subif.gpid);
#else
	return false;
#endif /* CONFIG_DPM_DATAPATH_HAL_GSWIP32 */
}

static int dpdp_desc_egress_set(struct dpdp_device *dp_dev, struct sk_buff *skb)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	struct dma_tx_desc_0 *desc_0 = (struct dma_tx_desc_0 *)&skb->DW0;
	struct dma_tx_desc_1 *desc_1 = (struct dma_tx_desc_1 *)&skb->DW1;

	desc_0->field.dest_sub_if_id = dp_dev->dp_subif.subif;
	desc_1->field.ep = dp_dev->dp_subif.gpid;
	desc_1->field.redir = 1; /* egflag=1 */
	desc_1->field.classid = (skb->priority >= 15) ? 15 : skb->priority;
#endif /* CONFIG_DPM_DATAPATH_HAL_GSWIP32 */

	return 0;
}

static int dpdp_desc_ingress_set(struct dpdp_device *dp_dev,
				 struct sk_buff *skb)
{
	struct dma_tx_desc_0 *desc_0 = (struct dma_tx_desc_0 *)&skb->DW0;
	struct dma_tx_desc_1 *desc_1 = (struct dma_tx_desc_1 *)&skb->DW1;

	desc_0->field.dest_sub_if_id = dp_dev->dp_subif.subif;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	desc_1->field.ep = dp_dev->dp_subif.gpid;
#endif /* CONFIG_DPM_DATAPATH_HAL_GSWIP32 */

	return 0;
}

static int dpdp_register_dev(struct module *owner, struct net_device *dev,
			     struct dpdp_handle *handle, struct dp_cb *ops,
			     int max_subif, u32 flags)
{
	if (!owner || !dev || !handle) {
		netdev_err(dev, "%s: not allowed, owner or handle NULL\n",
			   __func__);
		return -EINVAL;
	}

	if (flags & DP_F_DEREGISTER)
		return dpdp_dev_unregister(owner, dev, handle);
	else
		return dpdp_dev_register(owner, dev, handle, ops, max_subif);
}

static int dpdp_register_subif(struct module *owner, struct net_device *dev,
			       struct dpdp_handle *handle, u32 flags)
{
	if (!owner || !dev || !handle) {
		netdev_err(dev, "%s: not allowed, owner or handle NULL\n",
			   __func__);
		return -EINVAL;
	}

	if (flags & DP_F_DEREGISTER)
		return dpdp_subif_unregister(dev, handle);
	else
		return dpdp_subif_register(dev, handle);
}

static int dpdp_offload_validate(struct net_device *dev, struct sk_buff *skb)
{
	/* push the VLAN tag from @skb->vlan_tci inside to the payload */
	if (skb_vlan_tag_present(skb)) {
		if (__vlan_insert_tag(skb, skb->vlan_proto,
				      skb_vlan_tag_get(skb))) {
			return -EINVAL;
		}
		skb->vlan_tci = 0;
	}

	return 0;
}

static rx_handler_result_t dpdp_fwd_offload(struct net_device *dev,
					    struct dpdp_handle *handle,
					    struct sk_buff *skb, u32 flags)
{
	int err;
	struct dpdp_device *dp_dev;
	u32 dp_flags = 0;

	if (unlikely(!handle)) {
		netdev_dbg(dev, "%s: not allowed, handle NULL\n", __func__);
		return RX_HANDLER_PASS;
	}

	dp_dev = dpdp_device_find_by_key(dev, handle_key_get(handle));
	if (!dp_dev) {
		netdev_dbg(dev, "%s: not allowed, handle NULL\n", __func__);
		return RX_HANDLER_PASS;
	}

	if (!dpdp_device_running(dp_dev)) {
		this_cpu_inc(dp_dev->stats->tx_fwd_offload_errs);
		return RX_HANDLER_PASS;
	}

	if (dpdp_offload_validate(dp_dev->dev, skb)) {
		this_cpu_inc(dp_dev->stats->tx_fwd_offload_errs);
		return RX_HANDLER_PASS;
	}

	if (!dev_is_mac_header_xmit(dev) &&
	    dpdp_eth_header_push(skb, dp_dev->dummy_laddr,
				 dp_dev->dummy_raddr, skb->protocol)) {
		this_cpu_inc(dp_dev->stats->tx_fwd_offload_errs);
		return RX_HANDLER_PASS;
	}

	if (IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32))
		dp_flags |= DP_TX_BYPASS_FLOW | DP_TX_BYPASS_QOS;

	err = dpdp_rxtx(dp_dev, skb, dpdp_desc_ingress_set, dp_flags);
	if (likely(!err))
		this_cpu_inc(dp_dev->stats->tx_fwd_offload_pkts);
	else
		this_cpu_inc(dp_dev->stats->tx_fwd_offload_errs);

	return RX_HANDLER_CONSUMED;
}

static netdev_tx_t dpdp_qos_offload(struct net_device *dev,
				    struct dpdp_handle *handle,
				    struct sk_buff *skb, u32 flags)
{
	int err;
	struct dpdp_device *dp_dev;
	u32 dp_flags = 0;

	if (unlikely(!skb || !handle)) {
		netdev_dbg(dev, "%s: not allowed, skb or handle NULL\n", __func__);
		return NETDEV_TX_CONTINUE;
	}

	dp_dev = dpdp_device_find_by_key(dev, handle_key_get(handle));
	if (!dp_dev) {
		netdev_dbg(dev, "%s: not allowed, handle NULL\n", __func__);
		return NETDEV_TX_CONTINUE;
	}

	if (!dpdp_device_running(dp_dev)) {
		this_cpu_inc(dp_dev->stats->tx_qos_offload_errs);
		return NETDEV_TX_CONTINUE;
	}

	if (!dpdp_tx_slow_active(dp_dev, skb))
		return NETDEV_TX_CONTINUE;

	if (dpdp_offload_validate(dp_dev->dev, skb)) {
		this_cpu_inc(dp_dev->stats->tx_qos_offload_errs);
		return NETDEV_TX_CONTINUE;
	}

	if (!dev_is_mac_header_xmit(dev) &&
	    dpdp_eth_header_push(skb, dp_dev->dummy_raddr,
				 dp_dev->dummy_laddr, skb->protocol)) {
		this_cpu_inc(dp_dev->stats->tx_qos_offload_errs);
		return NETDEV_TX_CONTINUE;
	}

	dp_flags |= DP_TX_BYPASS_FLOW | DP_TX_NEWRET;

	err = dpdp_rxtx(dp_dev, skb, dpdp_desc_egress_set, dp_flags);
	if (likely(!err))
		this_cpu_inc(dp_dev->stats->tx_qos_offload_pkts);
	else if (err == DP_TX_FN_DROPPED)
		this_cpu_inc(dp_dev->stats->tx_qos_offload_errs);

	return err;
}

static int __init directpath_dp_init(void)
{
	directpath_debugfs_init();
	rcu_assign_pointer(dpdp_register_dev_hook, dpdp_register_dev);
	rcu_assign_pointer(dpdp_register_subif_hook, dpdp_register_subif);
	rcu_assign_pointer(dpdp_fwd_offload_hook, dpdp_fwd_offload);
	rcu_assign_pointer(dpdp_qos_offload_hook, dpdp_qos_offload);

	pr_info("%s successful\n", __func__);
	return 0;
}

static void __exit directpath_dp_exit(void)
{
	rcu_assign_pointer(dpdp_register_dev_hook, NULL);
	rcu_assign_pointer(dpdp_register_subif_hook, NULL);
	rcu_assign_pointer(dpdp_fwd_offload_hook, NULL);
	rcu_assign_pointer(dpdp_qos_offload_hook, NULL);
	synchronize_net();
	directpath_debugfs_exit();

	pr_info("%s successful\n", __func__);
}

module_init(directpath_dp_init);
module_exit(directpath_dp_exit);
MODULE_LICENSE("GPL");
