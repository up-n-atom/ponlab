// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2023 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2, as published
 * by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MCAST_HELPER_API_H
#define _MCAST_HELPER_API_H

#include <net/addrconf.h>
#include <net/ipv6.h>

/* Reg/dereg flag definition */
#define MCH_F_REGISTER		0x01
#define MCH_F_DEREGISTER	0x02
#define MCH_F_DIRECTPATH	0x04
#define MCH_F_UPDATE_MAC_ADDR	0x08
#define MCH_F_FW_RESET		0x10
#define MCH_F_NEW_STA		0x20
#define MCH_F_DISCONN_MAC	0x40

/* Callback flag definition */
#define MCH_CB_F_ADD		0x01
#define MCH_CB_F_DEL		0x02
#define MCH_CB_F_UPD		0x03
#define MCH_CB_F_DEL_UPD	0x08
#define MCH_CB_F_DROP		0x10

#define MAX_MAC			64

#define MCH_GID_BCAST		0
#define MCH_GID_ERR		-1

/** Protocol type */
typedef enum {
	MCH_IPV4 = 0,
	MCH_IPV6 = 1,
	MCH_INVALID
} mch_ptype_t;

typedef struct _ip_addr_t {
	/* Protocol type IPV4 or IPV6 */
	mch_ptype_t ip_type;
	union {
		struct in_addr ip4;
		struct in6_addr ip6;
	} addr;
} ip_addr_t;

typedef struct _mcast_stream_t {
	/* Rx netdevice */
	struct net_device *rx_dev;
	/* Source IP - IPV4 or IPV6 */
	ip_addr_t src_ip;
	/* Destination IP (GA) - IPV4 or IPV6 */
	ip_addr_t dst_ip;
	/* Protocol type */
	uint32_t proto;
	/* Source port */
	uint32_t src_port;
	/* Destination port */
	uint32_t dst_port;
	/* Stream source MAC address */
	unsigned char src_mac[ETH_ALEN];
	/* Number of joined MACs */
	uint32_t num_joined_macs;
	/* Joined host MAC address array */
	unsigned char macaddr[MAX_MAC][ETH_ALEN];
} mcast_stream_t;

typedef int32_t (*Mcast_module_callback_t)(unsigned int grpidx,
		struct net_device *netdev, void *mc_stream, unsigned int flag);

extern void mcast_helper_register_module(
	struct net_device *dev,		/* Registered netdev e.g. wlan0.1 */
	struct module *mod_name,	/* Kernel module name */
	char *addl_name,		/* Optional additional name */
	Mcast_module_callback_t *cb,	/* Callback function */
	void *data,			/* Variable input data */
	unsigned int flags		/* Flag - MCH_F_* */
);

#if IS_ENABLED(CONFIG_BRIDGE_IGMP_SNOOPING) && IS_ENABLED(CONFIG_MCAST_HELPER)
static inline int mcast_helper_get_skb_gid(struct sk_buff *skb)
{
	/* TODO: use CONFIG_SKB_EXTENSIONS to get mc_gid */
	return (skb->mc_gid > 0) ? skb->mc_gid : -1;
}

static inline void mcast_helper_set_skb_gid(struct sk_buff *skb, int gid)
{
	skb->mc_gid = gid;
}
#else
static inline int mcast_helper_get_skb_gid(struct sk_buff *skb)
{
	return -1;
}

static inline void mcast_helper_set_skb_gid(struct sk_buff *skb, int gid)
{
}
#endif /* !CONFIG_BRIDGE_IGMP_SNOOPING */
#endif /* _MCAST_HELPER_API_H */
