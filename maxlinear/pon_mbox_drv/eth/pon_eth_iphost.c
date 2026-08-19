/******************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 ******************************************************************************/

/* We have a main network interface registered to the DP manager as the port
 * which is represented by struct 'ltq_pon_net_iphost_port' usually named ip0.
 * Below that we can have multiple iphost bridge port devices.
 * They are represented by struct 'ltq_pon_net_iphost' with is_bp set to true.
 * They are normally named 'iphostX_bp'. These devices are registered as
 * subinterfaces to DP and we can configure TC rules on them. We can not send
 * or receive data on them. Each iphost bridge port devices has one virtual ip
 * host netdevice for transmit. It is represented by struct 'ltq_pon_net_iphost'
 * with is_bp set to false. They are normally named 'iphostX'.
 * These devices can be added to a bridge to send and receive traffic.
 *
 * This is an example configuration done with the ip command:
 *
 * # Register bridge port netdevice for iphost receive.
 * ip link add link ip0 iphost0_bp type iphost
 *
 * # Register virtual netdevice for iphost transmit.
 * ip link add link iphost0_bp iphost0 type iphost
 */

#include <linux/version.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/if_ether.h>
#include <net/rtnetlink.h>
#include <linux/of.h>

#include <net/datapath_api.h>
#include <net/datapath_br_domain.h>
#include <pon/pon_mbox_ikm.h>

#if defined(CONFIG_SOC_LGM)
#include <net/mxl_pon_hgu_vuni.h>
#endif

#include "pon_eth.h"
#include "pon_eth_iphost.h"

/** DMA RX descriptor dword 0 bit 16 used to mark direction */
#define PON_ETH_DMA_RX_DESC0_DW0BIT16 16

/** This represents the IP Host netdev port private data. */
struct ltq_pon_net_iphost_port {
	struct ltq_pon_net_hw *hw;
};

/** Dummy IP host open for future use */
static int iphost_open(struct net_device *iphost_ndev)
{
	return 0;
}

/** Dummy IP host stop for future use */
static int iphost_stop(struct net_device *iphost_ndev)
{
	return 0;
}

/** Start the transmission on the selected network device. */
static netdev_tx_t iphost_start_xmit(struct sk_buff *skb,
				     struct net_device *iphost_ndev)
{
	struct ltq_pon_net_iphost *iphost = netdev_priv(iphost_ndev);
	int32_t ret;
	uint32_t dp_flags = 0;
	unsigned int len = skb->len;
	dp_subif_t dp_subif;

	if (iphost->is_bp) {
		kfree_skb(skb);
		iphost->stats.tx_dropped++;
		return NETDEV_TX_OK;
	}

	/* Insert the packet at the UNI interface by using the DP manager
	 * reinsert function on the interface. This way the packet
	 * will go through the complete GSWIP processing like when
	 * directly received on this UNI port.
	 */
	if (dp_get_netif_subifid(iphost->link, NULL, NULL, NULL,
				 &dp_subif, 0)) {
		kfree_skb(skb);
		pr_err("%s: dp_get_netif_subifid failed for %s\n", __func__,
		       iphost->link->name);
		iphost->stats.tx_dropped++;
		return NETDEV_TX_BUSY;
	}

	if (skb_put_padto(skb, ETH_ZLEN)) {
		iphost->stats.tx_dropped++;
		return NETDEV_TX_OK;
	}

	ltq_pon_net_set_skd_dw(skb, dp_subif.port_id, dp_subif.subif);

#if defined(CONFIG_SOC_LGM)
	/* mark direction as US */
	skb->DW0 |= (1 << PON_ETH_DMA_RX_DESC0_DW0BIT16);
#endif

	if (iphost_ndev->features & NETIF_F_HW_CSUM)
		dp_flags |= DP_TX_CAL_CHKSUM;

	ret = dp_xmit(iphost_ndev, &dp_subif, skb, skb->len, dp_flags);
	if (!ret) {
		iphost->stats.tx_packets++;
		iphost->stats.tx_bytes += len;
	} else {
		iphost->stats.tx_dropped++;
	}

	return NETDEV_TX_OK;
}

static void iphost_get_stats(struct net_device *iphost_ndev,
			     struct rtnl_link_stats64 *storage)
{
#ifndef CONFIG_DPM_DATAPATH_MIB
	struct ltq_pon_net_iphost *iphost = netdev_priv(iphost_ndev);

	*storage = iphost->stats;
#else
	dp_get_netif_stats(iphost, NULL, storage, 0);
#endif
}

/** Returns ifindex */
static int iphost_get_iflink(const struct net_device *iphost_ndev)
{
	struct ltq_pon_net_iphost *iphost = netdev_priv(iphost_ndev);

	return iphost->link->ifindex;
}

/** IP host netdev ops. */
static const struct net_device_ops iphost_ops = {
	.ndo_open		= iphost_open,
	.ndo_stop		= iphost_stop,
	.ndo_start_xmit		= iphost_start_xmit,
	.ndo_set_mac_address	= eth_mac_addr,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_get_stats64	= iphost_get_stats,
	.ndo_get_iflink		= iphost_get_iflink,
};

static int iphost_mdev_open(struct net_device *hw_ndev)
{
	return -EBUSY;
}

static netdev_tx_t iphost_mdev_start_xmit(struct sk_buff *skb,
					  struct net_device *ndev)
{
	kfree_skb(skb);
	return NETDEV_TX_OK;
}

/** IP host netdev ops for hardware port interface. */
static const struct net_device_ops iphost_mdev_ops = {
	.ndo_open		= iphost_mdev_open,
	.ndo_start_xmit		= iphost_mdev_start_xmit,
};

static int32_t iphost_dp_rx(struct net_device *ndev,
			    struct net_device *txif,
			    struct sk_buff *skb, int32_t len);

static int iphost_dp_register(struct ltq_pon_net_iphost *iphost,
			      struct ltq_pon_net_hw *hw)
{
	struct device *dev = hw->dev;
#if !defined(CONFIG_SOC_LGM)
	struct module *owner = dev->driver->owner;
#else
	struct module *owner = hw->ipp_owner;
#endif
	dp_subif_t dp_subif = {0,};
	struct dp_subif_data dp_data = {0,};
	int err;

	/* DP will assign subif dynamically when negative value is passed */
	dp_subif.subif = -1;
	dp_subif.port_id = hw->ipp_id;
	dp_data.flag_ops |= DP_SUBIF_BP_CPU_DISABLE;
	dp_data.flag_ops |= DP_SUBIF_NON_CPU_MAC;
	dp_data.flag_ops |= DP_SUBIF_BR_DOMAIN;

	dp_data.domain_id = DP_BR_DM_IPH;
	dp_data.domain_members = DP_BR_DM_MEMBER(dp_data.domain_id) |
				 DP_BR_DM_MEMBER(DP_BR_DM_UCA);
	dp_data.rx_en_flag = netif_running(iphost->ndev);
	dp_data.mac_learn_disable = DP_MAC_LEARNING_DIS;
#if defined(CONFIG_SOC_LGM)
	dp_data.rx_fn = iphost_dp_rx;
#endif
	err = dp_register_subif_ext(dev->id, owner,
				    iphost->ndev, iphost->ndev->name, &dp_subif,
				    &dp_data, 0);
	if (err != DP_SUCCESS) {
		netdev_err(iphost->ndev, "%s: dp_register_subif_ext failed\n",
			   __func__);
		return -ENODEV;
	}
	iphost->dp_subif = dp_subif.subif;

	return 0;
}

static int iphost_dp_deregister(struct ltq_pon_net_iphost *iphost,
				struct ltq_pon_net_hw *hw)
{
	struct device *dev = hw->dev;
#if !defined(CONFIG_SOC_LGM)
	struct module *owner = dev->driver->owner;
#else
	struct module *owner = hw->ipp_owner;
#endif
	dp_subif_t dp_subif = {0,};
	struct dp_subif_data dp_data = {0,};
	int err;

	dp_subif.subif = iphost->dp_subif;
	dp_subif.port_id = hw->ipp_id;

	dp_data.flag_ops |= DP_SUBIF_BP_CPU_DISABLE;
	dp_data.flag_ops |= DP_SUBIF_NON_CPU_MAC;

	err = dp_register_subif_ext(dev->id, owner, iphost->ndev,
				    iphost->ndev->name, &dp_subif, &dp_data,
				    DP_F_DEREGISTER);
	if (err != DP_SUCCESS) {
		netdev_err(iphost->ndev, "%s: dp_register_subif_ext failed\n",
			   __func__);
		return -ENODEV;
	}

	return 0;
}

/** Creates a new IP host on the interface given in IFLA_LINK. */
static int iphost_newlink(struct net *src_net,
			  struct net_device *iphost_ndev,
			  struct nlattr *tb[],
			  struct nlattr *data[],
			  struct netlink_ext_ack *extack)
{
	struct ltq_pon_net_iphost *iphost = netdev_priv(iphost_ndev);
	struct net_device *link_ndev;
	struct ltq_pon_net_iphost_port *port_priv;
	struct ltq_pon_net_iphost *bp_priv;
	struct ltq_pon_net_hw *hw;
	int err = -ENODEV;

	if (!tb[IFLA_LINK]) {
		netdev_err(iphost_ndev, "Missing IFLA_LINK!\n");
		return -EINVAL;
	}
	link_ndev = dev_get_by_index(src_net, nla_get_u32(tb[IFLA_LINK]));
	if (!link_ndev) {
		netdev_err(iphost_ndev, "Invalid IFLA_LINK\n");
		return err;
	}

	iphost->link = link_ndev;
	/* check if master link is port, register netdev in hardware */
	if (link_ndev->netdev_ops->ndo_open == &iphost_mdev_open) {
		iphost->is_bp = true;
		port_priv = netdev_priv(link_ndev);
		hw = port_priv->hw;
	/* check if master link is bp, register netdev in software only */
	} else if (link_ndev->netdev_ops->ndo_open == &iphost_open) {
		iphost->is_bp = false;
		bp_priv = netdev_priv(link_ndev);
		hw = bp_priv->hw;
	} else {
		netdev_err(iphost_ndev, "Wrong IFLA_LINK\n");
		goto err_put_dev;
	}

	err = register_netdevice(iphost_ndev);
	if (err)
		goto err_put_dev;

	iphost->hw = hw;
	iphost->ndev = iphost_ndev;

	if (iphost->is_bp) {
		err = iphost_dp_register(iphost, hw);
		if (err)
			goto err_netdev_remove;
	}

	spin_lock_bh(&hw->iphost_lock);
	list_add(&iphost->node, &hw->iphost_list);
	spin_unlock_bh(&hw->iphost_lock);

	return 0;

err_netdev_remove:
	unregister_netdevice(iphost_ndev);
err_put_dev:
	dev_put(link_ndev);

	return err;
}

/** Removes IP host local data. */
void iphost_dellink(struct net_device *iphost_ndev,
		    struct list_head *head)
{
	struct ltq_pon_net_iphost *iphost = netdev_priv(iphost_ndev);
	struct ltq_pon_net_hw *hw = iphost->hw;
	bool is_bp = iphost->is_bp;

	if (is_bp)
		iphost_dp_deregister(iphost, hw);

	spin_lock_bh(&hw->iphost_lock);
	list_del(&iphost->node);
	spin_unlock_bh(&hw->iphost_lock);

	unregister_netdevice_queue(iphost_ndev, head);
	dev_put(iphost->link);
}

/** This is called to configure the newly created IP host interface. The
 * interface itself is created by the Linux stack this just does the IP host
 * specific initial configuration.
 */
static void iphost_setup(struct net_device *iphost_ndev)
{
	struct ltq_pon_net_iphost *iphost = netdev_priv(iphost_ndev);

	ether_setup(iphost_ndev);

	iphost_ndev->netdev_ops = &iphost_ops;
	iphost_ndev->priv_destructor = free_netdev;

	eth_hw_addr_random(iphost_ndev);

	iphost_ndev->features = NETIF_F_SG;
	iphost_ndev->hw_features = iphost_ndev->features;
	iphost_ndev->vlan_features = iphost_ndev->features;

	INIT_LIST_HEAD(&iphost->node);
}

/** Rtnl ops to create and delete IP host netdev. */
static struct rtnl_link_ops iphost_rtnl __read_mostly = {
	.kind		= "iphost",
	.priv_size	= sizeof(struct ltq_pon_net_iphost),

	.newlink	= iphost_newlink,
	.dellink	= iphost_dellink,
	.setup		= iphost_setup,
};

/** Register NetLink interface */
int pon_eth_iphost_rtnl_link_register(void)
{
	return rtnl_link_register(&iphost_rtnl);
}

/** Unregister NetLink interface */
void pon_eth_iphost_rtnl_link_unregister(void)
{
	rtnl_link_unregister(&iphost_rtnl);
}

static int skb_send_copy(struct sk_buff *skb,
			 struct ltq_pon_net_iphost *iphost,
			 int32_t len)
{
	struct rtnl_link_stats64 *stats = &iphost->stats;
	struct net_device *ndev = iphost->ndev;
	struct sk_buff *iphost_skb;

	iphost_skb = skb_clone(skb, GFP_ATOMIC);
	if (!iphost_skb) {
		pr_err("IPHOST mcast/bcast skb clone fail\n");
		return -1;
	}

	iphost_skb->dev = iphost->ndev;
	iphost_skb->protocol = eth_type_trans(iphost_skb, ndev);

	if (ndev->features & NETIF_F_RXCSUM)
		iphost_skb->ip_summed = CHECKSUM_UNNECESSARY;

	netif_rx(iphost_skb);
	stats->rx_packets++;
	stats->rx_bytes += len;

	return 0;
}

/** Handle IP host netdevice */
static int iphost_rx(struct ltq_pon_net_hw *hw,
		     struct sk_buff *skb,
		     int32_t len)
{
	struct ltq_pon_net_iphost *iphost;
	struct list_head *ele;

	/* multicast/broadcast */
	if (is_multicast_ether_addr(skb->data)) {
		spin_lock_bh(&hw->iphost_lock);
		list_for_each(ele, &hw->iphost_list) {
			iphost = list_entry(ele, struct ltq_pon_net_iphost,
					    node);
			if (iphost->is_bp)
				continue;
			/* Attempt to send packet to all IP host interfaces */
			(void)skb_send_copy(skb, iphost, len);
		}
		spin_unlock_bh(&hw->iphost_lock);
		return 1;
	}

	spin_lock_bh(&hw->iphost_lock);
	list_for_each(ele, &hw->iphost_list) {
		iphost = list_entry(ele, struct ltq_pon_net_iphost, node);
		if (iphost->is_bp)
			continue;
		if (ether_addr_equal(skb->data,
				     iphost->ndev->dev_addr)) {
			spin_unlock_bh(&hw->iphost_lock);
			return skb_send_copy(skb, iphost, len);
		}
	}
	spin_unlock_bh(&hw->iphost_lock);

	return 1;
}

/** Receives one package, this is called by the datapath library. */
static int32_t iphost_dp_rx(struct net_device *ndev,
			    struct net_device *txif,
			    struct sk_buff *skb, int32_t len)
{
	struct ltq_pon_net_hw *hw = NULL;
	struct net_device *rdev = ndev ? ndev : txif;
	struct ltq_pon_net_iphost *iphost;
	int ret;

	if (WARN_ON(!skb))
		return -1;

	if (WARN_ON(!rdev)) {
		dev_kfree_skb_any(skb);
		return -1;
	}

	if (!rdev->netdev_ops || rdev->netdev_ops->ndo_open != &iphost_open) {
		netdev_err(rdev, "received data on unknown device\n");
		dev_kfree_skb_any(skb);
		return -1;
	}

	iphost = netdev_priv(rdev);
	hw = iphost->hw;

	if (WARN_ON(!hw)) {
		dev_kfree_skb_any(skb);
		return -1;
	}

#if defined(CONFIG_SOC_LGM)
	/* DW0.b16=1: Ignore US packets here */
	if (skb->DW0 & (1 << PON_ETH_DMA_RX_DESC0_DW0BIT16)) {
		dev_kfree_skb_any(skb);
		return 0;
	}
#endif

	if (hw->soc_data->pmac_remove) {
		/* Remove PMAC to DMA header */
		len -= DP_MAX_PMAC_LEN;
		skb_pull(skb, DP_MAX_PMAC_LEN);
	}

	/* The bridge port devices have the offload_fwd_mark set by dpm.
	 * It needs to be set back to zero as the iphost devices are
	 * not registered to dpm and still can be in a bridge which
	 * may generate switchdev warnings.
	 */
	skb->offload_fwd_mark = 0;
	ret = iphost_rx(hw, skb, len);
	dev_kfree_skb_any(skb);
	if (ret <= 0)
		return ret;

	/* netif_rx(skb) is not called here as the iphost bp device is used
	 * to setup the hardware forwarding. The iphost network devices has
	 * to be used to forward traffic in linux. This makes possible to
	 * make the iphost not accessible from the UNI side as the standard
	 * defines.
	 */
	return 0;
}

dp_cb_t iphost_dp_cb = {
	.rx_fn		= iphost_dp_rx,
};

#if defined(CONFIG_SOC_LGM)
static int dp_port_register(struct ltq_pon_net_hw *hw)
{
	int err;

	err = mxl_vuni_drv_iphost_info_get(0, &hw->ipp_id,
					   &hw->ipp_owner);
	if (err || !hw->ipp_owner) {
		pr_err("%s: mxl_vuni_drv_iphost_info_get returned: %d\n",
		       __func__, err);
		return err;
	}

	pr_debug("%s: assigned port id: %d from owner: %s\n", __func__,
		 hw->ipp_id, hw->ipp_owner->name);

	return 0;
}
#else
static int dp_port_register(struct ltq_pon_net_hw *hw)
{
	struct dp_port_data port_data = {0,};
	struct dp_dev_data dp_data = {0};
	struct module *owner = hw->dev->driver->owner;
	dp_pmac_cfg_t pmac_cfg = {0};
	int err;

	if (!hw->dp_flags.with_tx_fcs)
		port_data.flag_ops |= DP_F_DATA_FCS_DISABLE;
	if (hw->dp_flags.with_rx_fcs)
		port_data.flag_ops |= DP_F_REM_FCS;

	hw->ipp_id = dp_alloc_port_ext(hw->dev->id, owner, NULL,
				       0, 0, NULL, &port_data,
				       DP_F_DIRECT);
	if (hw->ipp_id == DP_FAILURE) {
		pr_err("%s: dp_alloc_port_ext failed\n", __func__);
		return -ENODEV;
	}
	pr_debug("%s: assigned port id: %d\n", __func__, hw->ipp_id);

	err = dp_register_dev_ext(hw->dev->id, owner, hw->ipp_id,
				  &iphost_dp_cb, &dp_data, 0);
	if (err != DP_SUCCESS) {
		dp_alloc_port_ext(hw->dev->id, owner, NULL, hw->dev->id,
				  hw->ipp_id, NULL, NULL, DP_F_DEREGISTER);
		pr_err("%s: dp_register_dev_ext failed\n", __func__);
		return -ENODEV;
	}

	pmac_cfg.eg_pmac_flags = EG_PMAC_F_PMAC | EG_PMAC_F_BSLSEG |
				 EG_PMAC_F_FCS;
	pmac_cfg.ig_pmac_flags = IG_PMAC_F_PRESENT;
	pmac_cfg.eg_pmac.pmac = 1;
	pmac_cfg.eg_pmac.bsl_seg = 1;
	pmac_cfg.eg_pmac.fcs = 1;
	pmac_cfg.ig_pmac.pmac = 1;
	err = dp_pmac_set(hw->dev->id, hw->ipp_id, &pmac_cfg);
	if (err != DP_SUCCESS) {
		dp_alloc_port_ext(hw->dev->id, owner, NULL, hw->dev->id,
				  hw->ipp_id, NULL, NULL, DP_F_DEREGISTER);
		pr_err("%s: dp_pmac_set failed\n", __func__);
		return -ENODEV;
	}

	return 0;
}
#endif /* CONFIG_SOC_LGM */

int ltq_pon_iphost_port_register(struct ltq_pon_net_hw *hw)
{
	struct net_device *ipp_ndev;
	struct ltq_pon_net_iphost_port *priv;
	int err;

	ipp_ndev = alloc_netdev(sizeof(*hw), "ip%d", NET_NAME_UNKNOWN,
				ether_setup);
	if (!ipp_ndev) {
		dev_err(hw->dev, "%s: Cannot allocate netdev memory!\n",
			__func__);
		return -ENOMEM;
	}

	priv = netdev_priv(ipp_ndev);
	priv->hw = hw;

	ipp_ndev->netdev_ops = &iphost_mdev_ops;
	err = register_netdev(ipp_ndev);
	if (err) {
		dev_err(hw->dev, "%s: Cannot register netdev\n", __func__);
		free_netdev(ipp_ndev);
		return err;
	}
	hw->ipp_ndev = ipp_ndev;

	err = dp_port_register(hw);
	if (err) {
		unregister_netdev(hw->ipp_ndev);
		free_netdev(hw->ipp_ndev);
		return err;
	}

	return 0;
}

#if defined(CONFIG_SOC_LGM)
void ltq_pon_iphost_port_deregister(struct ltq_pon_net_hw *hw)
{
	unregister_netdev(hw->ipp_ndev);
	free_netdev(hw->ipp_ndev);
}
#else
void ltq_pon_iphost_port_deregister(struct ltq_pon_net_hw *hw)
{
	struct module *owner = hw->dev->driver->owner;

	dp_register_dev_ext(hw->dev->id, owner, hw->ipp_id, &iphost_dp_cb,
			    NULL, DP_F_DEREGISTER);
	dev_dbg(hw->dev, "%s: dp_register_dev_ext(inst: %i, port_id: %i, DP_F_DEREGISTER)\n",
		__func__, hw->dev->id, hw->ipp_id);
	dp_alloc_port_ext(hw->dev->id, owner, NULL, hw->ipp_id, hw->ipp_id,
			  NULL, NULL, DP_F_DEREGISTER);
	dev_dbg(hw->dev, "%s: dp_alloc_port_ext(inst: %i, dev_port: %i, DP_F_DEREGISTER)\n",
		__func__, hw->dev->id, hw->ipp_id);

	unregister_netdev(hw->ipp_ndev);
	free_netdev(hw->ipp_ndev);
}
#endif /* CONFIG_SOC_LGM */
