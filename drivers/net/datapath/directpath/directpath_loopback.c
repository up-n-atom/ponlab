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

/*
 * Usage:
 *
 * # Directpath link setup
 * ip link add dplo0 type directpath
 * ip link add link dplo0 name dplo0.1 type directpath
 *
 * # Directpath loopback setup
 * ifconfig dplo0 192.168.2.1 netmask 255.255.255.0 up
 * ip neigh add 192.168.2.2 dev dplo0 lladdr 00:11:22:33:44:55
 *
 * # Firewall settings (optional)
 * iptables -I OUTPUT -j ACCEPT
 * iptables -I INPUT -j ACCEPT
 * iptables -I FORWARD -j ACCEPT
 *
 * # Ping loopback test
 * ping 192.168.2.2
 *
 * # Iperf UDP loopback test
 * iperf -s -u -i 1
 * iperf -c 192.168.2.2 -u -b 100m -i 1
 *
 * # Directpath link teardown
 * ip link del dplo0.1
 * ip link del dplo0
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/if_arp.h>
#include <linux/ip.h>
#include <linux/rtnetlink.h>
#include <net/rtnetlink.h>
#include <linux/u64_stats_sync.h>
#include <net/ip_tunnels.h>
#include <net/gro_cells.h>
#include <net/directpath_api.h>

#define DRV_NAME	"directpath_loopback"
#define DRV_VERSION	"1.0"

#define DPDP_MAX_SUBIF_PER_PORT	16

struct directpath_loopback {
	u32 speed;
	struct dpdp_handle handle;
	struct gro_cells gro_cells;
};

static struct module owner;

static bool dev_type;
module_param(dev_type, bool, 0644);
MODULE_PARM_DESC(dev_type, "Override default device type (0:L2, 1:L3)");

static void directpath_loopback_get_stats64(struct net_device *dev,
					    struct rtnl_link_stats64 *stats)
{
	int i;

	for_each_possible_cpu(i) {
		const struct pcpu_sw_netstats *tstats;
		u64 tbytes, tpackets;
		u64 rbytes, rpackets;
		unsigned int start;

		tstats = per_cpu_ptr(dev->tstats, i);
		do {
			start = u64_stats_fetch_begin_irq(&tstats->syncp);
			tbytes = tstats->tx_bytes;
			tpackets = tstats->tx_packets;
			rbytes = tstats->rx_bytes;
			rpackets = tstats->rx_packets;
		} while (u64_stats_fetch_retry_irq(&tstats->syncp, start));
		stats->tx_bytes += tbytes;
		stats->tx_packets += tpackets;
		stats->rx_bytes += rbytes;
		stats->rx_packets += rpackets;
	}
}

static void loopback_swap_mac_ip_addr(struct sk_buff *skb)
{
	u8 tmp_mac[ETH_ALEN];
	union nf_inet_addr tmp_ip;

	/* swap mac addr */
	if (skb->dev->type != ARPHRD_NONE) {
		ether_addr_copy(tmp_mac, eth_hdr(skb)->h_dest);
		ether_addr_copy(eth_hdr(skb)->h_dest, eth_hdr(skb)->h_source);
		ether_addr_copy(eth_hdr(skb)->h_source, tmp_mac);
	}

	/* swap ip addr */
	if (skb->protocol == htons(ETH_P_IPV6)) {
		memcpy(&tmp_ip.ip6, &ipv6_hdr(skb)->saddr, 16);
		memcpy(&ipv6_hdr(skb)->saddr, &ipv6_hdr(skb)->daddr, 16);
		memcpy(&ipv6_hdr(skb)->daddr, &tmp_ip.ip6, 16);
	} else {
		tmp_ip.ip = ip_hdr(skb)->saddr;
		ip_hdr(skb)->saddr = ip_hdr(skb)->daddr;
		ip_hdr(skb)->daddr = tmp_ip.ip;
	}
}

static netdev_tx_t directpath_loopback_xmit(struct sk_buff *skb,
					    struct net_device *dev)
{
	struct directpath_loopback *priv = netdev_priv(dev);
	struct pcpu_sw_netstats *tstats = this_cpu_ptr(dev->tstats);
	int err;

	if (dev->type != ARPHRD_NONE) {
		if (is_multicast_ether_addr(eth_hdr(skb)->h_dest) ||
		    ((eth_hdr(skb)->h_proto != htons(ETH_P_IP) &&
		     (eth_hdr(skb)->h_proto != htons(ETH_P_IPV6))))) {
			dev_kfree_skb(skb);
			return NETDEV_TX_OK;
		}
	}

	if (dpdp_qos_offload_hook) {
		err = dpdp_qos_offload_hook(dev, &priv->handle, skb, 0);
		if (err != NETDEV_TX_CONTINUE) {
			net_dbg_ratelimited("%s(%s) dpdp_qos_offload status %d\n",
					    __func__, netdev_name(dev), err);
			return err;
		}
	}

	u64_stats_update_begin(&tstats->syncp);
	tstats->tx_packets++;
	tstats->tx_bytes += skb->len;
	u64_stats_update_end(&tstats->syncp);

	skb_scrub_packet(skb, true);
#if IS_ENABLED(CONFIG_MXL_SKB_EXT)
	skb_ext_del(skb, SKB_EXT_ARCH);
#endif

	loopback_swap_mac_ip_addr(skb);

	u64_stats_update_begin(&tstats->syncp);
	tstats->rx_packets++;
	tstats->rx_bytes += skb->len;
	u64_stats_update_end(&tstats->syncp);

	if (dpdp_fwd_offload_hook) {
		err = dpdp_fwd_offload_hook(dev, &priv->handle, skb, 0);
		if (likely(err == RX_HANDLER_CONSUMED)) {
			net_dbg_ratelimited("%s(%s) dpdp_fwd_offload status %d\n",
					    __func__, netdev_name(dev), err);
			return NETDEV_TX_OK;
		}
	}

	if (dev->type == ARPHRD_NONE) {
		skb->dev = dev;
		if (ip_hdr(skb)->version == 6)
			skb->protocol = htons(ETH_P_IPV6);
		else
			skb->protocol = htons(ETH_P_IP);
		err = gro_cells_receive(&priv->gro_cells, skb);
	} else {
		skb->protocol = eth_type_trans(skb, dev);
		err = netif_rx(skb);
	}
	net_dbg_ratelimited("%s(%s) netif_rx status %d\n",
			    __func__, netdev_name(dev), err);

	return NETDEV_TX_OK;
}

static int directpath_loopback_dev_init(struct net_device *dev)
{
	dev->tstats = netdev_alloc_pcpu_stats(struct pcpu_sw_netstats);
	if (!dev->tstats)
		return -ENOMEM;

	return 0;
}

static void directpath_loopback_dev_uninit(struct net_device *dev)
{
	free_percpu(dev->tstats);
}

static const struct net_device_ops directpath_loopback_netdev_ops = {
	.ndo_init		= directpath_loopback_dev_init,
	.ndo_uninit		= directpath_loopback_dev_uninit,
	.ndo_start_xmit		= directpath_loopback_xmit,
	.ndo_get_stats64	= directpath_loopback_get_stats64,
};

static void directpath_loopback_get_drvinfo(struct net_device *dev,
					    struct ethtool_drvinfo *info)
{
	strlcpy(info->driver, DRV_NAME, sizeof(info->driver));
	strlcpy(info->version, DRV_VERSION, sizeof(info->version));
}

static
int directpath_loopback_get_link_ksettings(struct net_device *dev,
					   struct ethtool_link_ksettings *cmd)
{
	struct directpath_loopback *priv = netdev_priv(dev);

	cmd->base.speed = priv->speed;
	return 0;
}

static int
directpath_loopback_set_link_ksettings(struct net_device *dev,
				       const struct ethtool_link_ksettings *cmd)
{
	struct directpath_loopback *priv = netdev_priv(dev);

	priv->speed = cmd->base.speed;
	return 0;
}

static const struct ethtool_ops directpath_loopback_ethtool_ops = {
	.get_drvinfo		= directpath_loopback_get_drvinfo,
	.get_link_ksettings	= directpath_loopback_get_link_ksettings,
	.set_link_ksettings	= directpath_loopback_set_link_ksettings,
};

static void directpath_loopback_setup(struct net_device *dev)
{
	if (dev_type) {
		/* Point-to-Point Device */
		dev->hard_header_len = 0;
		dev->addr_len = 0;
		dev->mtu = 1500;

		/* Zero header length */
		dev->type = ARPHRD_NONE;
		dev->flags = IFF_POINTOPOINT | IFF_NOARP;
	} else {
		ether_setup(dev);
		eth_hw_addr_random(dev);
	}

	/* Initialize the device structure. */
	dev->netdev_ops = &directpath_loopback_netdev_ops;
	dev->ethtool_ops = &directpath_loopback_ethtool_ops;
	dev->needs_free_netdev = true;
}

static int directpath_loopback_validate(struct nlattr *tb[],
					struct nlattr *data[],
					struct netlink_ext_ack *extack)
{
	if (tb[IFLA_ADDRESS]) {
		if (nla_len(tb[IFLA_ADDRESS]) != ETH_ALEN)
			return -EINVAL;
		if (!is_valid_ether_addr(nla_data(tb[IFLA_ADDRESS])))
			return -EADDRNOTAVAIL;
	}
	return 0;
}

static int loopback_inherit_port_from_parent(struct net_device *parent)
{
	struct directpath_loopback *priv;

	if (!parent)
		return -1;

	priv = netdev_priv(parent);
	return priv->handle.port_id;
}

static int directpath_loopback_newlink(struct net *src_net,
				       struct net_device *dev,
				       struct nlattr *tb[],
				       struct nlattr *data[],
				       struct netlink_ext_ack *extack)
{
	int err;
	struct directpath_loopback *priv = netdev_priv(dev);
	struct net_device *parent;
	struct dp_cb dp_ops = {0};

	priv->speed = SPEED_1000;
	err = register_netdevice(dev);
	if (err < 0)
		goto out;

	if (dev->type == ARPHRD_NONE) {
		err = gro_cells_init(&priv->gro_cells, dev);
		if (err)
			pr_err("gro_cells_init failed for dev %s\n", dev->name);
	}

	if (tb[IFLA_LINK]) {
		parent = __dev_get_by_index(src_net,
					    nla_get_u32(tb[IFLA_LINK]));
		priv->handle.port_id =
			loopback_inherit_port_from_parent(parent);
	} else if (dpdp_register_dev_hook) {
		/* note: use default rx_fn handler */
		dpdp_register_dev_hook(&owner, dev, &priv->handle,
				       &dp_ops, DPDP_MAX_SUBIF_PER_PORT, 0);
	}

	priv->handle.subif = -1;
	if (dpdp_register_subif_hook)
		dpdp_register_subif_hook(&owner, dev, &priv->handle, 0);

out:
	return err;
}

static void directpath_loopback_dellink(struct net_device *dev,
					struct list_head *head)
{
	struct directpath_loopback *priv = netdev_priv(dev);

	if (dpdp_register_subif_hook)
		dpdp_register_subif_hook(&owner, dev, &priv->handle,
					 DP_F_DEREGISTER);
	if (dpdp_register_dev_hook)
		dpdp_register_dev_hook(&owner, dev, &priv->handle, NULL, 1,
				       DP_F_DEREGISTER);
	if (dev->type == ARPHRD_NONE)
		gro_cells_destroy(&priv->gro_cells);
	unregister_netdevice_queue(dev, head);
}

static struct rtnl_link_ops directpath_loopback_link_ops __read_mostly = {
	.kind		= "directpath",
	.priv_size	= sizeof(struct directpath_loopback),
	.setup		= directpath_loopback_setup,
	.validate	= directpath_loopback_validate,
	.newlink	= directpath_loopback_newlink,
	.dellink	= directpath_loopback_dellink,
};

static int __init directpath_loopback_init_module(void)
{
	snprintf(owner.name, MODULE_NAME_LEN, "directpath");

	return rtnl_link_register(&directpath_loopback_link_ops);
}

static void __exit directpath_loopback_cleanup_module(void)
{
	rtnl_link_unregister(&directpath_loopback_link_ops);
}

module_init(directpath_loopback_init_module);
module_exit(directpath_loopback_cleanup_module);
MODULE_LICENSE("GPL");
MODULE_ALIAS_RTNL_LINK(DRV_NAME);
MODULE_VERSION(DRV_VERSION);
