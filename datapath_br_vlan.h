// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2022, MaxLinear, Inc.
 * Copyright 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_BR_VLAN_H
#define DATAPATH_BR_VLAN_H

#include <../net/bridge/br_private.h>

static inline bool dp_br_vlan_enabled(struct net_device *br_dev)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
	struct net_bridge *br;
	br = netdev_priv(br_dev);
	if (br_vlan_enabled(br))
#else
	if (br_vlan_enabled(br_dev))
#endif
		return true;
	return false;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 15, 15)
#define CALL_SETLINK(x) x(dev, nlh, flags)
typedef int (*ndo_bridge_setlink)(struct net_device *dev,
				      struct nlmsghdr *nlh, u16 flags);

#else
#define CALL_SETLINK(x) x(dev, nlh, flags, extack)
typedef int (*ndo_bridge_setlink)(struct net_device *dev,
				      struct nlmsghdr *nlh, u16 flags,
				      struct netlink_ext_ack *extack);

#endif

#endif
