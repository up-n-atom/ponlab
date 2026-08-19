/******************************************************************************
 **
 ** FILE NAME   : mcast_helper.h
 ** AUTHOR      :
 ** DESCRIPTION : Multicast Helper module header file
 ** COPYRIGHT   : Copyright (c) 2020-2023, MaxLinear, Inc.
 **               Copyright (c) 2014 2015 Lantiq Beteiligungs-GmbH & Co. KG
 **
 **    This program is free software; you can redistribute it and/or modify
 **    it under the terms of the GNU General Public License as published by
 **    the Free Software Foundation; either version 2 of the License, or
 **    (at your option) any later version.
 **
 *******************************************************************************/

/*
 * mcast_helper.h - the header file with the ioctl definitions.
 *
 */

#ifndef _MCAST_HELPER_H
#define _MCAST_HELPER_H
#include <uapi/linux/in.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/module.h>
#include <net/mcast_helper_api.h>

/* Device information for IOCTL */
#define DEVICE_NAME		"mcast"
#define FIRST_MINOR		0
#define MINOR_CNT		1

#define MCH_MAGIC		'M'
#define MCH_MEMBER_ENTRY_ADD	_IOR(MCH_MAGIC, 0, char *)
#define MCH_MEMBER_ENTRY_UPDATE	_IOR(MCH_MAGIC, 1, char *)
#define MCH_MEMBER_ENTRY_REMOVE	_IOR(MCH_MAGIC, 2, char *)
#define MCH_SERVER_ENTRY_GET	_IOR(MCH_MAGIC, 3, char *)

#define MCH_SUCCESS		(0)
#define MCH_FAILURE		(-1)

typedef struct _mcast_table_t {
	/* Five tuple stream info */
	mcast_stream_t mc_stream;
	/* Inactive timer to cleanup the stream */
	struct timer_list inactive_timer;
	/* Stream is active or inactive */
	bool inactive;
	/* RX device brport flags (if bridge port) */
	unsigned long rx_brport_flags;
	/* mcgid_table map list */
	struct list_head list;
	/* RCU head */
	struct rcu_head rcu;
	/* GID list */
	struct list_head gid_list;
} mcast_table_t;

typedef struct _mcast_gid_t {
	/* Back pointer to mcast_table_t */
	mcast_table_t *mc_table;
	/* MultiCast Group Index */
	u32 gid;
	/* Member VLAN */
	u16 eg_vlan;
#ifdef CONFIG_MCAST_HELPER_ACL
	/* Output interface bitmap */
	u64 oif_bitmap;
	/* Probe Packet generate flag enable(1)/disable(0) */
	u32 probe_flag;
#endif
	/* mcast_gid map list */
	struct list_head list;
	/* RCU head */
	struct rcu_head rcu;
	/* DS member interface list */
	struct list_head member_list;
} mcast_gid_t;

typedef struct _mcast_member_t {
	/* Back pointer to mcast_gid_t */
	mcast_gid_t *mc_gid;
	/* Member netdevice */
	struct net_device *mem_dev;
#ifdef CONFIG_MCAST_HELPER_ACL
	/* Flag indicates acl status blocked(1)/unblocked(0) */
	u32 acl_blocked;
#endif
	/* MAC address count */
	u32 macaddr_count;
	/* mcast_member interface map list */
	struct list_head list;
	/* RCU head */
	struct rcu_head rcu;
	/* Host MAC address list */
	struct list_head macaddr_list;
} mcast_member_t;

typedef struct _mcast_mac_t {
	/* Back pointer to mcast_member_t */
	mcast_member_t *mc_mem;
	/* Host MAC address */
	u8 macaddr[ETH_ALEN];
	/* mcast_mac map list */
	struct list_head list;
	/* RCU head */
	struct rcu_head rcu;
} mcast_mac_t;

/* Bit definitions for IOCTL flags */

/* Call from event path */
#define MCH_IOC_EVENT		(1UL << 0)

typedef struct _mcast_rec_t {
	/* Member netdevice name */
	char mem_ifname[IFNAMSIZ];
	/* RX member netdevice name */
	char rx_ifname[IFNAMSIZ];
	/* Destination IP (GA) - IPV4 or IPV6 */
	ip_addr_t group_ip;
	/* Source IP - IPV4 or IPV6 (optional) */
	ip_addr_t src_ip;
	/* Protocol number */
	u32 proto;
	/* Source port */
	u32 src_port;
	/* Destination port */
	u32 dst_port;
	/* Host MAC address */
	u8 macaddr[ETH_ALEN];
	/* Multi-purpose flag for a single call */
	u32 flags;
	/* Multicast membership interval */
	u32 mem_interval;
} mcast_rec_t;

/* External APIs or callback pointers */
extern int mcast_helper_invoke_callback(u32 grpidx, struct net_device *netdev,
					void *mc_stream, u32 flag);
extern void (*mcast_helper_br_learning_hook)(struct sk_buff *skb,
					     struct net_device *dev,
					     unsigned long flags, u8 *host_mac);
#ifdef CONFIG_MCAST_HELPER_ACL
/* It should be defined in kernel and called from dev_queue_xmit() */
extern int (*mcast_helper_sig_check_update_ptr)(struct sk_buff *skb);
#endif
#endif /* _MCAST_HELPER_H */
