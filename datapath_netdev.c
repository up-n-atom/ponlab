// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2025, MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/if_arp.h>
#include <linux/if_link.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>

#include <net/datapath_api.h>

static void dp_get_drvinfo(struct net_device *dev, struct ethtool_drvinfo *info)
{
	strlcpy(info->driver, "dp_dummy_dev", sizeof(info->driver));
}

/* must provide dummy open, otherwise netif_running(...) always return false */
static int dp_open(struct net_device *dev)
{
	netif_tx_start_all_queues(dev);
	return 0;
}

static int dp_stop(struct net_device *dev)
{
	netif_tx_stop_all_queues(dev);
	return 0;
}

static const struct ethtool_ops dp_ethtool_ops = {
	 .get_drvinfo = dp_get_drvinfo,
};

static const struct net_device_ops dp_netdev_ops = {
	.ndo_open     = dp_open,
	.ndo_stop     = dp_stop,
	.ndo_setup_tc = dp_ndo_setup_tc,
};

static void dp_setup(struct net_device *dev)
{
	ether_setup(dev);

	dev->netdev_ops = &dp_netdev_ops;
	dev->ethtool_ops = &dp_ethtool_ops;

	dev->hw_features |= NETIF_F_HW_TC;
	dev->features |= NETIF_F_HW_TC;

	eth_hw_addr_random(dev);
	netif_carrier_on(dev);
}

struct net_device *dp_create_netdev(const char *name)
{
	/* Note: cannot use alloc_netdev if need support multiple tx queue */
	struct net_device *netdev = alloc_netdev_mqs(0, name, NET_NAME_ENUM,
						     dp_setup, 8, 1);

	if (!netdev)
		return NULL;
	if (register_netdev(netdev)) {
		free_netdev(netdev);
		return NULL;
	}
	return netdev;
}

