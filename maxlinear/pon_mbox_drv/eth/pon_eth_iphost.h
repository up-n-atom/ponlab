/******************************************************************************
 *
 *  Copyright (c) 2020 - 2025 MaxLinear, Inc.
 *  Copyright (C) 2020 Intel Corporation
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 ******************************************************************************/

/**
 *  \file
 *  IP host specific header file, used for internal definitions.
 */

#ifndef __PON_ETH_IPHOST_H
#define __PON_ETH_IPHOST_H

/** \addtogroup PON_ETH_IPHOST IP host Network Interface Configuration
 *  @{
 */

/** This represents the IP Host netdev configuration. */
struct ltq_pon_net_iphost {
	struct list_head node;
	struct net_device *ndev;
	/* IP Host netdevice master device,
	 * - IP Host port if 'is_bp' is true
	 * - IP Host bridge port if 'is_bp' is false
	 */
	struct net_device *link;
	/* IP Host netdevice is registered to datapath as child of port
	 * netdevice and can be registered to bridge and will be used to
	 * transmit traffic,
	 * otherwise it is only registered as linux netdevice for traffic
	 * receive.
	 */
	bool is_bp;
	s32 dp_subif;
	struct ltq_pon_net_hw *hw;
	struct rtnl_link_stats64 stats;
};

/** Register IP host NetLink interface. */
int pon_eth_iphost_rtnl_link_register(void);

/** Unregister IP Host NetLink interface. */
void pon_eth_iphost_rtnl_link_unregister(void);

/** Deletes IP Host interface */
void iphost_dellink(struct net_device *iphost_ndev, struct list_head *head);

/** Register IP Host port */
int ltq_pon_iphost_port_register(struct ltq_pon_net_hw *hw);
/** Deregister IP Host port */
void ltq_pon_iphost_port_deregister(struct ltq_pon_net_hw *hw);
/** @} */

#endif /* __PON_ETH_IPHOST_H */
