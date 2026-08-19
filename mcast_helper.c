/******************************************************************************
 **
 ** FILE NAME   : mcast_helper.c
 ** AUTHOR      :
 ** DESCRIPTION : Multicast Helper module
 ** COPYRIGHT   : Copyright (c) 2020-2024, MaxLinear, Inc.
 **               Copyright (c) 2014 2015 Lantiq Beteiligungs-GmbH & Co. KG
 **
 **    This program is free software; you can redistribute it and/or modify
 **    it under the terms of the GNU General Public License as published by
 **    the Free Software Foundation; either version 2 of the License, or
 **    (at your option) any later version.
 **
 *******************************************************************************/

/** Header files */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#if IS_ENABLED(CONFIG_BRIDGE_IGMP_SNOOPING)
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/skbuff.h>
#include <linux/errno.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/ipv6.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/inet.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <net/checksum.h>

#ifdef CONFIG_PROC_FS
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#endif

#include <linux/spinlock.h>
#include <linux/if_bridge.h>
#include <linux/if_vlan.h>

#include "mcast_helper.h"

/** Defines **/

/* Maximum GID supported by helper.
 * FIXME: Due to WAVE FW limitation, multicast streams are limited to 32.
 */
#define MCGID_MAX_SIZE          32

#define MCH_MAC_STR_LEN         20

#define UDP_HDR_LEN             (sizeof(struct udphdr))
#define TOT_HDR_LEN             (sizeof(struct iphdr) + UDP_HDR_LEN)
#define IP6_HDR_LEN             (sizeof(struct ipv6hdr))
#define TOT6_HDR_LEN            (IP6_HDR_LEN + UDP_HDR_LEN)

#define MCH_UPDATE_TIMER        10

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0)
#define setup_timer(t, cb, d)	timer_setup(t, cb, 0)
#endif

/* Log level for debug prints */
static int mcast_debug;

#define mch_debug(fmt, ...)							\
do {										\
	if (mcast_debug)							\
		printk(KERN_DEBUG "MCH(%d): "fmt, __LINE__, ##__VA_ARGS__);	\
} while (0)

/** Mcast helper MCGID table */
LIST_HEAD(mch_mcgid_table_g);
LIST_HEAD(mch_mcgid_table6_g);

/* Pointer to multicast table entries */
mcast_table_t *g_mc_table[MCGID_MAX_SIZE];

/* Mcast helper MCGID lock */
static DEFINE_SPINLOCK(mch_mcgid_lock);

/** Mcast helper global variables */
DECLARE_BITMAP(g_mcgid_bitmap, MCGID_MAX_SIZE);
#ifdef CONFIG_MCAST_HELPER_ACL
static struct sk_buff *skb_buff;
static struct sk_buff *skb_buff6;
static char mch_captured_skb = 1;
static char mch_captured_skb6 = 1;
static const char mch_signature[] = "mcast1234";
#endif

int mch_timerstarted;
int mch_timermod;
struct timer_list mch_helper_timer;
#ifdef CONFIG_MCAST_HELPER_ACL
static void mcast_helper_start_helper_timer(void);
#endif

int mch_iptype;

int mch_accl_enabled = 1;
int mch_acl_enabled = 0;

/* Mcast device global variables */
static int mch_major = -1;
static struct cdev mcast_cdev;
static struct class *mcast_class;
static bool device_created;

#if defined(CONFIG_SYSCTL) && defined(CONFIG_PROC_FS)
#ifdef CONFIG_MCAST_HELPER_ACL
static struct ctl_table_header *mcast_acl_sysctl_header;
#endif
static struct ctl_table_header *mcast_accl_sysctl_header;
#endif

/** Mcast helper function prototype */
static mcast_table_t *mcast_helper_get_inactive_table_entry(void);
static void mcast_helper_cleanup_inactive_table_entry(mcast_table_t *mc_tbl);
static void mcast_helper_inactive_timer_handler(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 15, 0)
		unsigned long data
#else
		struct timer_list *timer
#endif
		);

/*=============================================================================
 *Function Name: mch_cb_str
 *Description  : Function to returns callback option ADD/DEL/UPDATE/DEL_UPDATE
 *===========================================================================*/
static inline char *mch_cb_str(u32 flag)
{
	switch (flag) {
	case MCH_CB_F_ADD:
		return "ADD";
	case MCH_CB_F_DEL:
		return "DEL";
	case MCH_CB_F_UPD:
		return "UPDATE";
	case MCH_CB_F_DEL_UPD:
		return "DEL_UPD";
	case MCH_CB_F_DROP:
		return "DROP";
	case (MCH_CB_F_DEL | MCH_CB_F_DROP):
		return "DEL_DROP";
	default:
		return "?";
	}
}

/*=============================================================================
 *Function Name: mcast_helper_copy_ipaddr
 *Description  : Wrapper function to copy ip address
 *===========================================================================*/
static void mcast_helper_copy_ipaddr(ip_addr_t *to, ip_addr_t *from)
{
	if (!to || !from) {
		mch_debug("failed to copy IP, pointer is NULL\n");
		return;
	}

	memcpy(to, from, sizeof(ip_addr_t));
}

/*=============================================================================
 *Function Name: mcast_helper_is_same_ipaddr
 *Description  : Wrapper function to compare IP address
 *===========================================================================*/
static int mcast_helper_is_same_ipaddr(ip_addr_t *addr1, ip_addr_t *addr2)
{
	if (!addr1 || !addr2) {
		mch_debug("failed to compare IP, pointer is NULL\n");
		return 0;
	}

	if (addr1->ip_type == MCH_IPV4 && addr2->ip_type == MCH_IPV4)
		return addr1->addr.ip4.s_addr == addr2->addr.ip4.s_addr;
	else if (addr1->ip_type == MCH_IPV6 && addr2->ip_type == MCH_IPV6)
		return ipv6_addr_equal(&addr1->addr.ip6, &addr2->addr.ip6);

	return 0;
}

/*=============================================================================
 *Function Name: mcast_helper_init_ipaddr
 *Description  : Wrapper function to initialize IP address
 *===========================================================================*/
static void mcast_helper_init_ipaddr(ip_addr_t *addr, mch_ptype_t type, void *addrp)
{
	if (!addr) {
		mch_debug("failed to initialize IP, pointer is NULL\n");
		return;
	}

	addr->ip_type = type;
	if (type == MCH_IPV4) {
		if (addrp)
			addr->addr.ip4.s_addr = *((u32 *)addrp);
		else
			addr->addr.ip4.s_addr = 0;
	} else if (type == MCH_IPV6) {
		struct in6_addr *in6 = (struct in6_addr *)addrp;
		if (addrp)
			memcpy(&(addr->addr.ip6), in6, sizeof(struct in6_addr));
		else
			memset(&(addr->addr.ip6), 0, sizeof(struct in6_addr));
	}
}

/*=============================================================================
 *Function Name: mcast_helper_is_addr_unspecified
 *Description  : Wrapper function to IP address is specified
 *===========================================================================*/
static int mcast_helper_is_addr_unspecified(ip_addr_t *addr)
{
	if (!addr) {
		mch_debug("failed to verify IP, pointer is NULL\n");
		return 0;
	}

	if (addr->ip_type == MCH_IPV4)
		return (addr->addr.ip4.s_addr == 0);
	else if (addr->ip_type == MCH_IPV6)
		return ipv6_addr_any(&addr->addr.ip6);

	return 0;
}

/*=============================================================================
 *Function Name: mcast_helper_list_p
 *Description  : Wrapper function to get the membership list pointer
 *===========================================================================*/
static struct list_head *mcast_helper_list_p(mch_ptype_t type)
{
	if (type == MCH_IPV4)
		return &mch_mcgid_table_g;
	else if (type == MCH_IPV6)
		return &mch_mcgid_table6_g;

	mch_debug("MCGID table list not found - IP type wrong\n");
	return NULL;
}

/*=============================================================================
 *Function Name: mch_get_netif
 *Description  : Wrapper function to get netif from interface name
 *===========================================================================*/
static struct net_device *mch_get_netif(char *ifname)
{
	struct net_device *netif;

	if (!ifname) {
		mch_debug("failed to get device, pointer is NULL\n");
		return NULL;
	}

	netif = dev_get_by_name(&init_net, ifname);
	if (netif) {
		dev_put(netif);
		return netif;
	} else {
		return NULL;
	}
}

/*=============================================================================
 *Function Name: mch_get_netif_by_idx
 *Description  : Wrapper function to get netif from interface index
 *===========================================================================*/
static struct net_device *mch_get_netif_by_idx(int ifindex)
{
	struct net_device *netif;

	netif = dev_get_by_index(&init_net, ifindex);
	if (netif) {
		dev_put(netif);
		return netif;
	} else {
		return NULL;
	}
}

/*=============================================================================
 *Function Name: mcast_helper_ipv6_link_local_word3
 *Description  : Function to check link local multicast last word
 *===========================================================================*/
static bool mcast_helper_ipv6_link_local_word3(__be32 addr)
{
	if (addr == 0)
		return false;

	/* Validate ranges */
	if (addr <= htonl(0x00000013))
		return true;
	if ((addr >= htonl(0x0000006a)) && (addr <= htonl(0x0000006f)))
		return true;
	if ((addr >= htonl(0x00010001)) && (addr <= htonl(0x00010007)))
		return true;

	/* Validate specific values */
	switch (ntohl(addr)) {
	case 0x00000016: /* All MLDv2-capable routers */
	case 0x0000001a: /* all-RPL-nodes */
	case 0x000000fb: /* mDNSv6 */
		return true;
	}

	return false;
}

/*=============================================================================
 *Function Name: mcast_helper_is_ll_multicast
 *Description  : Function to check link local multicast address to be skipped
 *===========================================================================*/
static bool mcast_helper_is_ll_multicast(ip_addr_t *gaddr)
{
	if (gaddr->ip_type == MCH_IPV4) {
		/* IPv4 link-local multicast space 224.0.0.0/24
		 * and IPv4 Service Discovery Protocol address 239.255.255.250
		 */
		if (ipv4_is_local_multicast(gaddr->addr.ip4.s_addr) ||
		    (gaddr->addr.ip4.s_addr == htonl(0xEFFFFFFAU))) {
			return true;
		}
	} else if (gaddr->ip_type == MCH_IPV6) {
		struct in6_addr *addr = &gaddr->addr.ip6;

		/* Validate all link local address - ref IANA */
		if ((((addr->s6_addr32[0] ^ htonl(0xff020000)) |
		    addr->s6_addr32[1] | addr->s6_addr32[2]) == 0) &&
		    mcast_helper_ipv6_link_local_word3(addr->s6_addr32[3])) {
			return true;
		}
	}

	return false;
}

/* XXX: Below utility APIs must be called under mch_mcgid_lock */

/*=============================================================================
 *Function Name: mch_list_count_nodes
 *Description  : Function to count number of nodes in a list
 *===========================================================================*/
static inline int mch_list_count_nodes(struct list_head *head)
{
	struct list_head *tmp;
	int count = 0;

	if (!head)
		return count;

	list_for_each(tmp, head)
		count++;

	return count;
}

/*=============================================================================
 * Function Name: mcast_helper_is_mcgid_full
 * Description  : Function to check all GID are in use or not
 *===========================================================================*/
static inline bool mcast_helper_is_mcgid_full(void)
{
	return (find_first_zero_bit(g_mcgid_bitmap,
		MCGID_MAX_SIZE) >= MCGID_MAX_SIZE);
}

/*=============================================================================
 * Function Name: mcast_helper_allocate_gid
 * Description  : Function to allocate group index
 *===========================================================================*/
static inline int mcast_helper_allocate_gid(void)
{
	int gid = find_first_zero_bit(g_mcgid_bitmap, MCGID_MAX_SIZE);

	if (gid >= MCGID_MAX_SIZE)
		return MCH_GID_ERR;

	set_bit(gid, g_mcgid_bitmap);
	return gid;
}

/*=============================================================================
 *Function Name: mcast_helper_release_gid
 *Description  : Function to release allocated group index
 *===========================================================================*/
static inline void mcast_helper_release_gid(u32 gid)
{
	clear_bit(gid, g_mcgid_bitmap);
}

/*=============================================================================
 * function name: mcast_helper_update_mac_list
 * description  : Function updates the MAC list which will be passed to
 *                registered subscribers
 *===========================================================================*/
static void
mcast_helper_update_mac_list(mcast_member_t *mc_mem, mcast_stream_t *stream)
{
	mcast_mac_t *mc_mac = NULL, *tmp = NULL;

	if (!mc_mem || !stream) {
		mch_debug("failed to update MAC list, pointer is NULL\n");
		return;
	}

	memset(stream->macaddr, 0, MAX_MAC * ETH_ALEN);
	stream->num_joined_macs = 0;

	list_for_each_entry_safe(mc_mac, tmp, &mc_mem->macaddr_list, list) {
		if (stream->num_joined_macs < MAX_MAC) {
			memcpy(stream->macaddr[stream->num_joined_macs],
			       mc_mac->macaddr, ETH_ALEN);
			stream->num_joined_macs++;
		}
	}
}

/*=============================================================================
 *function name: mcast_helper_search_mac_record
 *description  : function to search MAC record in a member record
 *===========================================================================*/
static mcast_mac_t *
mcast_helper_search_mac_record(mcast_member_t *mc_mem, u8 *mac)
{
	mcast_mac_t *mc_mac = NULL, *tmp = NULL;

	if (!mc_mem || !mac) {
		mch_debug("failed to find MAC record, pointer is NULL\n");
		return NULL;
	}

	list_for_each_entry_safe(mc_mac, tmp, &mc_mem->macaddr_list, list) {
		if (!memcmp(mc_mac->macaddr, mac, ETH_ALEN))
			return mc_mac;
	}

	return NULL;
}

/*=============================================================================
 *function name: mcast_helper_add_mac_record
 *description  : function to add MAC address record into member record
 *===========================================================================*/
static mcast_mac_t *
mcast_helper_add_mac_record(mcast_member_t *mc_mem, u8 *macaddr)
{
	mcast_mac_t *mc_mac;

	if (!mc_mem || !macaddr) {
		mch_debug("failed to add MAC record, pointer is NULL\n");
		return NULL;
	}

	mc_mac = kzalloc(sizeof(mcast_mac_t), GFP_ATOMIC);
	if (!mc_mac) {
		mch_debug("failed to allocate memory for MAC record\n");
		return NULL;
	}
	INIT_LIST_HEAD(&mc_mac->list);
	memcpy(mc_mac->macaddr, macaddr, ETH_ALEN);
	mc_mem->macaddr_count++;

	/* Back pointer to member record */
	mc_mac->mc_mem = mc_mem;
	list_add_tail_rcu(&mc_mac->list, &mc_mem->macaddr_list);
	mch_debug("adding MAC=%pM into member=%s, MAC count=%d\n",
		  macaddr, mc_mem->mem_dev->name, mc_mem->macaddr_count);

	return mc_mac;
}

/*=============================================================================
 *Function Name: mcast_helper_delete_mac_record
 *Description  : Function which delete an entry from MAC record
 *===========================================================================*/
static void
mcast_helper_delete_mac_record(mcast_mac_t *mc_mac)
{
	mcast_member_t *mc_mem;

	if (!mc_mac || !mc_mac->mc_mem) {
		mch_debug("failed to delete MAC record, pointer is NULL\n");
		return;
	}

	mc_mem = mc_mac->mc_mem;
	mc_mem->macaddr_count--;
	mch_debug("deleting MAC=%pM from member=%s, MAC remains=%d\n",
		  mc_mac->macaddr, mc_mem->mem_dev->name, mc_mem->macaddr_count);
	list_del_rcu(&mc_mac->list);
	kfree_rcu(mc_mac, rcu);
}

/*=============================================================================
 *Function Name: mcast_helper_search_mem_record
 *Description  : Function to search and get the member record based on device
 *===========================================================================*/
static mcast_member_t *
mcast_helper_search_mem_record(mcast_gid_t *mc_gid, struct net_device *dev)
{
	mcast_member_t *mc_mem = NULL, *tmp = NULL;

	if (!mc_gid || !dev) {
		mch_debug("failed to find member record, pointer is NULL\n");
		return NULL;
	}

	list_for_each_entry_safe(mc_mem, tmp, &mc_gid->member_list, list) {
		if (mc_mem->mem_dev == dev)
			return mc_mem;
	}

	return NULL;
}

/*=============================================================================
 *Function Name: mcast_helper_add_mem_record
 *Description  : Function which add entry in member record.
 *===========================================================================*/
static mcast_member_t *
mcast_helper_add_mem_record(mcast_gid_t *mc_gid, struct net_device *mem_dev)
{
	mcast_member_t *mc_mem;

	if (!mc_gid || !mem_dev) {
		mch_debug("failed to add member record, pointer is NULL\n");
		return NULL;
	}

	mc_mem = kzalloc(sizeof(mcast_member_t), GFP_ATOMIC);
	if (!mc_mem) {
		mch_debug("failed to allocate memory for add member\n");
		return NULL;
	}
	INIT_LIST_HEAD(&mc_mem->list);
	INIT_LIST_HEAD(&mc_mem->macaddr_list);

	mc_mem->mem_dev = mem_dev;
	mc_mem->macaddr_count = 0;
#ifdef CONFIG_MCAST_HELPER_ACL
	if (mch_acl_enabled)
		mc_mem->acl_blocked = 0;
#endif

	/* Back pointer to gid record */
	mc_mem->mc_gid = mc_gid;
	list_add_tail_rcu(&mc_mem->list, &mc_gid->member_list);
	mch_debug("adding member=%s into GID record=%p with GID=%u\n",
		  mem_dev->name, mc_gid, mc_gid->gid);

	return mc_mem;
}

/*=============================================================================
 *Function Name: mcast_helper_delete_mem_record
 *Description  : Function which deletes an entry from member record
 *===========================================================================*/
static void
mcast_helper_delete_mem_record(mcast_member_t *mc_mem)
{
	mcast_gid_t *mc_gid;

	if (!mc_mem || !mc_mem->mc_gid) {
		mch_debug("failed to delete member record, pointer is NULL\n");
		return;
	}

	if (!list_empty(&mc_mem->macaddr_list))
		return;

	mc_gid = mc_mem->mc_gid;
	mch_debug("deleting member=%s from GID record=%p with GID=%u\n",
		  mc_mem->mem_dev->name, mc_gid, mc_gid->gid);
	list_del_rcu(&mc_mem->list);
	kfree_rcu(mc_mem, rcu);
}

#ifdef CONFIG_MCAST_HELPER_ACL
/*=============================================================================
 *function name: mcast_helper_get_gid_record_by_gid_num
 *description  : function to get the GID record from GID number
 *===========================================================================*/
static mcast_gid_t *
mcast_helper_get_gid_record_by_gid_num(struct list_head *head, u32 gid)
{
	mcast_table_t *mc_tbl = NULL, *tmp_tbl = NULL;
	mcast_gid_t *mc_gid = NULL, *tmp = NULL;

	if (!head) {
		mch_debug("failed to get GID record, pointer is NULL\n");
		return NULL;
	}

	if (gid >= MCGID_MAX_SIZE)
		return NULL;

	list_for_each_entry_safe(mc_tbl, tmp_tbl, head, list) {
		list_for_each_entry_safe(mc_gid, tmp, &mc_tbl->gid_list, list) {
			if (mc_gid->gid == gid)
				return mc_gid;
		}
	}

	return NULL;
}
#endif

/*=============================================================================
 *Function Name: mcast_helper_get_gid_record_by_mem_dev
 *Description  : Function to get GID record from member device (VLAN supported)
 *===========================================================================*/
static mcast_gid_t *
mcast_helper_get_gid_record_by_mem_dev(mcast_table_t *mc_tbl,
				       struct net_device *mem_dev)
{
	mcast_gid_t *mc_gid = NULL, *tmp = NULL;
	u16 vid = VLAN_VID_MASK;

	if (!mc_tbl || !mem_dev) {
		mch_debug("failed to find GID record, pointer is NULL\n");
		return NULL;
	}
	if (is_vlan_dev(mem_dev))
		vid = vlan_dev_vlan_id(mem_dev);

	list_for_each_entry_safe(mc_gid, tmp, &mc_tbl->gid_list, list) {
		if (vid == mc_gid->eg_vlan) {
			/* EG VLAN matched (for non VLAN, VLAN_VID_MASK) */
			return mc_gid;
		}
	}

	return NULL;
}

/*=============================================================================
 *Function Name: mcast_helper_get_inactive_gid_record
 *Description  : Function to get inactive GID record
 *===========================================================================*/
static inline mcast_gid_t *
mcast_helper_get_inactive_gid_record(mcast_table_t *mc_tbl)
{
	if (!mc_tbl || !mc_tbl->inactive) {
		mch_debug("failed to find inactive GID record, NULL/active\n");
		return NULL;
	}

	return list_first_entry_or_null(&mc_tbl->gid_list, mcast_gid_t, list);
}

/*=============================================================================
 *Function Name: mcast_helper_add_gid_record
 *Description  : Function to create GID and save VLAN into GID record.
 *===========================================================================*/
static mcast_gid_t *
mcast_helper_add_gid_record(mcast_table_t *mc_tbl, struct net_device *mem_dev)
{
	mcast_table_t *mc_inact_tbl;
	mcast_gid_t *mc_gid;
	u16 vid = VLAN_VID_MASK;
	int gid = MCH_GID_ERR;

	if (!mc_tbl || !mem_dev) {
		mch_debug("failed to add GID record, pointer NULL\n");
		return NULL;
	}

	/* GIDs are exhausted? */
	if (mcast_helper_is_mcgid_full()) {
		mc_inact_tbl = mcast_helper_get_inactive_table_entry();
		if (!mc_inact_tbl) {
			/* If no inactive table entry, all GIDs are
			 * in use - learning failed.
			 */
			return NULL;
		}
		mch_debug("All GID exhausted, reusing inactive entry");
		/* To reuse GID from inactive table entry, cleanup old one */
		mcast_helper_cleanup_inactive_table_entry(mc_inact_tbl);
	}

	gid = mcast_helper_allocate_gid();
	if (gid < 0)
		return NULL;

	mc_gid = kzalloc(sizeof(mcast_gid_t), GFP_ATOMIC);
	if (!mc_gid) {
		mch_debug("failed to allocate memory for add GID\n");
		return NULL;
	}
	INIT_LIST_HEAD(&mc_gid->list);
	INIT_LIST_HEAD(&mc_gid->member_list);

	if (is_vlan_dev(mem_dev))
		vid = vlan_dev_vlan_id(mem_dev);
	mc_gid->gid = (u32)gid;
	mc_gid->eg_vlan = vid;
#ifdef CONFIG_MCAST_HELPER_ACL
	mc_gid->oif_bitmap = 0;
	mc_gid->probe_flag = 0;
#endif

	/* Back pointer to multicast table entry */
	mc_gid->mc_table = mc_tbl;
	list_add_tail_rcu(&mc_gid->list, &mc_tbl->gid_list);
	mch_debug("adding GID record=%p with GID=%d\n", mc_gid, gid);

	return mc_gid;
}

/*=============================================================================
 *Function Name: mcast_helper_delete_gid_record
 *Description  : Function to delete a GID record
 *===========================================================================*/
static void mcast_helper_delete_gid_record(mcast_gid_t *mc_gid)
{
	if (!mc_gid || !mc_gid->mc_table) {
		mch_debug("failed to delete GID record, pointer is NULL\n");
		return;
	}

	if (mc_gid->mc_table->inactive) {
		/* Transition from ACTIVE to DROP: DO NOT remove GID */
		return;
	}

	if (!list_empty(&mc_gid->member_list))
		return;

	mch_debug("deleting GID record=%p with GID=%u\n", mc_gid, mc_gid->gid);
	mcast_helper_release_gid(mc_gid->gid);
	list_del_rcu(&mc_gid->list);
	kfree_rcu(mc_gid, rcu);
}

/*=============================================================================
 *Function Name: mcast_helper_search_table_entry
 *Description  : Function to search mcast table entry from RX dev, saddr and
 *               gaddr. Returns the number of entries found for that stream.
 *               The entries will be saved to global array of pointer which
 *               is used under lock.
 *===========================================================================*/
static int
mcast_helper_search_table_entry(struct net_device *rx_dev, ip_addr_t *saddr,
				ip_addr_t *gaddr, struct list_head *head)
{
	mcast_table_t *mc_tbl = NULL, *tmp = NULL;
	mcast_stream_t *stream;
	int count = 0;

	if (!head) {
		mch_debug("failed to find MCGID entry, list is NULL\n");
		return count;
	}

	/* Called under lock, so safe to clear and use global pointer array */
	memset(g_mc_table, 0, sizeof(mcast_table_t *) * MCGID_MAX_SIZE);

	list_for_each_entry_safe(mc_tbl, tmp, head, list) {
		stream = &mc_tbl->mc_stream;
		if (rx_dev && (rx_dev != stream->rx_dev))
			continue;
		/* IG interface matched */
		if (!mcast_helper_is_same_ipaddr(&stream->dst_ip, gaddr))
			continue;
		/* ASM: group IP matched */
		if (!mcast_helper_is_addr_unspecified(saddr) &&
		    !mcast_helper_is_addr_unspecified(&stream->src_ip) &&
		    !mcast_helper_is_same_ipaddr(&stream->src_ip, saddr))
			continue;
		/* SSM: source IP matched (if specified) */
		g_mc_table[count++] = mc_tbl;
	}

	return count;
}

/*=============================================================================
 *Function Name: mcast_helper_add_table_entry
 *Description  : Function to create and add 5 tuple entry into MCGID table.
 *===========================================================================*/
static mcast_table_t *
mcast_helper_add_table_entry(struct net_device *rx_dev, unsigned long port_flags,
			     ip_addr_t *saddr, ip_addr_t *gaddr, u32 proto,
			     u32 sport, u32 dport, u8 *src_mac,
			     struct net_device *mem_dev, u8 *mac,
			     struct list_head *head)
{
	mcast_table_t *mc_tbl = NULL;

	if (!head) {
		mch_debug("failed to add mcast entry, list is NULL\n");
		return NULL;
	}

	mc_tbl = kzalloc(sizeof(mcast_table_t), GFP_ATOMIC);
	if (!mc_tbl) {
		mch_debug("failed to allocate memory for mcast table\n");
		return NULL;
	}
	INIT_LIST_HEAD(&mc_tbl->list);
	INIT_LIST_HEAD(&mc_tbl->gid_list);

	/* Fill stream information */
	mc_tbl->mc_stream.rx_dev = rx_dev;
	mcast_helper_copy_ipaddr(&mc_tbl->mc_stream.src_ip, saddr);
	mcast_helper_copy_ipaddr(&mc_tbl->mc_stream.dst_ip, gaddr);
	mc_tbl->mc_stream.proto = proto;
	mc_tbl->mc_stream.src_port = sport;
	mc_tbl->mc_stream.dst_port = dport;
	/* If RX device is under bridge, pass MAC to HW */
	if (netif_is_bridge_port(rx_dev)) {
		memcpy(mc_tbl->mc_stream.src_mac, src_mac, ETH_ALEN);
		/* Save brport flags for hairpin/fast_leave properties */
		mc_tbl->rx_brport_flags = port_flags;
	}
	/* Setup inactive timer to cleanup inactive entry from MCGID table */
	setup_timer(&mc_tbl->inactive_timer, mcast_helper_inactive_timer_handler,
		    (unsigned long)mc_tbl);

	list_add_tail_rcu(&mc_tbl->list, head);
	mch_debug("adding entry=%p into mcast table\n", mc_tbl);

	return mc_tbl;
}

/*=============================================================================
 *Function Name: mcast_helper_delete_table_entry
 *Description  : Function to delete a multicast table entry
 *===========================================================================*/
static void mcast_helper_delete_table_entry(mcast_table_t *mc_tbl)
{
	if (!mc_tbl) {
		mch_debug("failed to delete mcast table entry, ptr is NULL\n");
		return;
	}

	if (!list_empty(&mc_tbl->gid_list))
		return;

	mch_debug("deleting mcast entry=%p from mcast table\n", mc_tbl);
	del_timer(&mc_tbl->inactive_timer);
	list_del_rcu(&mc_tbl->list);
	kfree_rcu(mc_tbl, rcu);
}

/*=============================================================================
 * Function Name: mcast_helper_get_inactive_table_entry
 * Description  : Function to get inactive entry from MCGID table
 *===========================================================================*/
static mcast_table_t *mcast_helper_get_inactive_table_entry(void)
{
	struct list_head *mcgid_list = mcast_helper_list_p(MCH_IPV4);
	mcast_table_t *mc_tbl = NULL, *tmp = NULL;

	if (!mcgid_list || list_empty(mcgid_list))
		return NULL;

	/* Check IPV4 list */
	list_for_each_entry_safe(mc_tbl, tmp, mcgid_list, list) {
		if (!mc_tbl->inactive)
			continue;
		/* Return first inactive entry from IPV4 table */
		return mc_tbl;
	}

	mcgid_list = mcast_helper_list_p(MCH_IPV6);
	if (!mcgid_list || list_empty(mcgid_list))
		return NULL;

	/* Check IPV6 list */
	list_for_each_entry_safe(mc_tbl, tmp, mcgid_list, list) {
		if (!mc_tbl->inactive)
			continue;
		/* Return first inactive entry from IPV6 table */
		return mc_tbl;
	}

	return NULL;
}

/*=============================================================================
 *Function Name: mcast_helper_cleanup_inactive_table_entry
 *Description  : Function to cleanup HW and inactive entry from MCGID table
 *===========================================================================*/
static void
mcast_helper_cleanup_inactive_table_entry(mcast_table_t *mc_tbl)
{
	mcast_gid_t *mc_gid = NULL;
	mcast_stream_t *stream;

	if (!mc_tbl) {
		mch_debug("failed to cleanup inactive entry, ptr is NULL\n");
		return;
	}

	if (!mc_tbl->inactive)
		return;

	mc_gid = mcast_helper_get_inactive_gid_record(mc_tbl);
	if (!mc_gid) {
		mch_debug("GID record not found in inactive table entry\n");
		return;
	}

	/* Updates the MAC list to zero count before deleting inactive stream */
	stream = &mc_tbl->mc_stream;
	memset(stream->macaddr, 0, MAX_MAC * ETH_ALEN);
	stream->num_joined_macs = 0;

	mcast_helper_invoke_callback(mc_gid->gid, NULL, stream, MCH_CB_F_DEL);
	mch_debug("Transition from DROP in HW to DELETE: remove from HW\n");
	if (stream->src_ip.ip_type == MCH_IPV4) {
		mch_debug("CB_DEL for stream from(%s)# [%pI4]@[%pI4]"
			  " proto=%#x, sPort=%d, dPort=%d\n",
			  stream->rx_dev->name, &stream->src_ip.addr,
			  &stream->dst_ip.addr, stream->proto,
			  stream->src_port, stream->dst_port);
	} else if (stream->src_ip.ip_type == MCH_IPV6) {
		mch_debug("CB_DEL for stream from(%s)# [%pI6c]@[%pI6c]"
			  " proto=%#x, sPort=%d, dPort=%d\n",
			  stream->rx_dev->name, &stream->src_ip.addr,
			  &stream->dst_ip.addr, stream->proto,
			  stream->src_port, stream->dst_port);
	}

	mc_tbl->inactive = false;
	mcast_helper_delete_gid_record(mc_gid);
	mcast_helper_delete_table_entry(mc_tbl);
}

/*=============================================================================
 *Function Name: mcast_helper_invoke_return_callback
 *Description  : Function invokes callback to subscribers like PPA/WLAN ..
 *===========================================================================*/
static int
mcast_helper_invoke_return_callback(mcast_member_t *mc_mem, u32 flag)
{
	int ret = MCH_SUCCESS;
	struct net_device *mem_dev;
	mcast_stream_t *stream;
	mcast_table_t *mc_tbl;
	mcast_gid_t *mc_gid;

	if (mch_accl_enabled == 0)
		goto done;

	if (!mc_mem || !mc_mem->mc_gid || !mc_mem->mc_gid->mc_table) {
		mch_debug("failed to invoke callback, pointer is NULL\n");
		return MCH_FAILURE;
	}

	mc_gid = mc_mem->mc_gid;
	mc_tbl = mc_gid->mc_table;
	mem_dev = mc_mem->mem_dev;
	stream = &mc_tbl->mc_stream;

	/* Updates the MAC list before callback */
	mcast_helper_update_mac_list(mc_mem, stream);
	if (stream->num_joined_macs == 0) {
		mch_debug("no MAC present, so not invoking callback\n");
		goto done;
	}
	ret = mcast_helper_invoke_callback(mc_gid->gid, mem_dev, stream, flag);
	if (ret)
		goto done;

	if (stream->src_ip.ip_type == MCH_IPV4) {
		mch_debug("CB_%s for member(%s): stream from(%s)# [%pI4]@[%pI4]"
			  " proto=%#x, sPort=%d, dPort=%d\n",
			  mch_cb_str(flag), mem_dev->name, stream->rx_dev->name,
			  &stream->src_ip.addr, &stream->dst_ip.addr,
			  stream->proto, stream->src_port, stream->dst_port);
	} else if (stream->src_ip.ip_type == MCH_IPV6) {
		mch_debug("CB_%s for member(%s): stream from(%s)# [%pI6c]@[%pI6c]"
			  " proto=%#x, sPort=%d, dPort=%d\n",
			  mch_cb_str(flag), mem_dev->name, stream->rx_dev->name,
			  &stream->src_ip.addr, &stream->dst_ip.addr,
			  stream->proto, stream->src_port, stream->dst_port);
	}

done:
	return ret;
}

/* XXX: Above utility APIs must be called under mch_mcgid_lock */

/*=============================================================================
 *function name: mcast_helper_inactive_timer_handler
 *description  : function to cleanup inactive entry from table on timer expiry
 *===========================================================================*/
static void mcast_helper_inactive_timer_handler(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 15, 0)
		unsigned long data
#else
		struct timer_list *timer
#endif
		)
{
	mcast_table_t *mc_tbl = NULL;

	mch_debug("helper inactive timer invoked\n");

	spin_lock_bh(&mch_mcgid_lock);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 15, 0)
	mc_tbl = (mcast_table_t *)data;
#else
	mc_tbl = from_timer(mc_tbl, timer, inactive_timer);
#endif

	if (!mc_tbl) {
		mch_debug("No table entry found from timer\n");
		goto unlock;
	}

	if (list_empty(&mc_tbl->gid_list) ||
	    mch_list_count_nodes(&mc_tbl->gid_list) > 1)
		goto unlock;

	/* Remove only inactive stream which is dropping in HW */
	if (!mc_tbl->inactive)
		goto unlock;

	mcast_helper_cleanup_inactive_table_entry(mc_tbl);

unlock:
	spin_unlock_bh(&mch_mcgid_lock);
}

#ifdef CONFIG_MCAST_HELPER_ACL
/*=============================================================================
 *Function Name: mcast_helper_make_skb_writeable
 *Description  : Function to make skb writeable
 *===========================================================================*/
static int mcast_helper_make_skb_writeable(struct sk_buff *skb, int write_len)
{
	if (!pskb_may_pull(skb, write_len))
		return -ENOMEM;

	if (!skb_cloned(skb) || skb_clone_writable(skb, write_len))
		return 0;

	return pskb_expand_head(skb, 0, 0, GFP_ATOMIC);
}

/*=============================================================================
 *Function Name: mcast_helper_set_ip_addr
 *Description  : Function to set IP address in the skb
 *===========================================================================*/
static void
mcast_helper_set_ip_addr(struct sk_buff *skb, struct iphdr *nh,
			 u32 *addr, u32 new_addr)
{
	int transport_len = skb->len - skb_transport_offset(skb);

	if (nh->protocol == IPPROTO_TCP) {
		if (likely(transport_len >= sizeof(struct tcphdr)))
			inet_proto_csum_replace4(&tcp_hdr(skb)->check, skb,
					*addr, new_addr, 1);
	} else if (nh->protocol == IPPROTO_UDP) {
		if (likely(transport_len >= sizeof(struct udphdr))) {
			struct udphdr *udph = udp_hdr(skb);

			if (udph->check || skb->ip_summed == CHECKSUM_PARTIAL) {
				inet_proto_csum_replace4(&udph->check, skb,
						*addr, new_addr, 1);
				if (!udph->check)
					udph->check = CSUM_MANGLED_0;
			}
		}
	}

	csum_replace4(&nh->check, *addr, new_addr);

	*addr = new_addr;
}

/*=============================================================================
 *function name: mcast_helper_update_ipv6_checksum
 *description  : function to update ipv6 checksum in the passed skb
 *===========================================================================*/
static void
mcast_helper_update_ipv6_checksum(struct sk_buff *skb,
				  u32 addr[4], u32 new_addr[4])
{
	int transport_len = skb->len - skb_transport_offset(skb);

	if (likely(transport_len >= sizeof(struct udphdr))) {
		struct udphdr *udph = udp_hdr(skb);

		if (udph->check || (skb->ip_summed == CHECKSUM_PARTIAL)) {
			inet_proto_csum_replace16(&udph->check, skb,
					addr, new_addr, 1);
			if (!udph->check)
				udph->check = CSUM_MANGLED_0;
		}
	}
}

/*=============================================================================
 *function name: mcast_helper_set_ipv6_addr
 *description  : function to set the IP address in the skb
 *===========================================================================*/
static void
mcast_helper_set_ipv6_addr(struct sk_buff *skb, u32 addr[4],
			   u32 new_addr[4], bool recalculate_csum)
{
	if (recalculate_csum)
		mcast_helper_update_ipv6_checksum(skb, addr, new_addr);

	memcpy(addr, new_addr, sizeof(__be32[4]));
}

/*=============================================================================
 *function name: mcast_helper_set_ipv6
 *description  : function to update the ipv6 address in skb
 *===========================================================================*/
static int mcast_helper_set_ipv6(struct sk_buff *skb,
				 ip_addr_t *new_saddr, ip_addr_t *new_daddr)
{
	struct ipv6hdr *nh;
	int err;
	u32 *saddr, *daddr;

	err = mcast_helper_make_skb_writeable(skb, skb_network_offset(skb) +
			sizeof(struct ipv6hdr));
	if (unlikely(err))
		return err;

	nh = ipv6_hdr(skb);
	saddr = (u32 *)&nh->saddr;
	daddr = (u32 *)&nh->daddr;

	if (memcmp(&(new_saddr->addr.ip6), saddr, sizeof(struct in6_addr)))
		mcast_helper_set_ipv6_addr(skb, saddr,
				(u32 *)&(new_saddr->addr.ip6), true);
	if (memcmp(&(new_daddr->addr.ip6), daddr, sizeof(struct in6_addr)))
		mcast_helper_set_ipv6_addr(skb, daddr,
				(u32 *)&(new_daddr->addr.ip6), true);


	return 0;
}

/*=============================================================================
 *function name: mcast_helper_set_ipv4
 *description  : function to update the ipv4 address in skb
 *===========================================================================*/
static int mcast_helper_set_ipv4(struct sk_buff *skb,
				 ip_addr_t *saddr, ip_addr_t *daddr)
{
	struct iphdr *nh;
	int err;

	err = mcast_helper_make_skb_writeable(skb, skb_network_offset(skb) +
					      sizeof(struct iphdr));
	if (unlikely(err))
		return err;

	nh = ip_hdr(skb);

	if (saddr->addr.ip4.s_addr != nh->saddr)
		mcast_helper_set_ip_addr(skb, nh, &nh->saddr,
					 saddr->addr.ip4.s_addr);

	if (daddr->addr.ip4.s_addr != nh->daddr)
		mcast_helper_set_ip_addr(skb, nh, &nh->daddr,
					 daddr->addr.ip4.s_addr);

	return 0;
}

/*=============================================================================
 *function name: mcast_helper_set_port
 *description  : function to update the port info in skb
 *===========================================================================*/
static void
mcast_helper_set_port(struct sk_buff *skb, unsigned short *port,
		      unsigned short new_port, unsigned short *check)
{
	inet_proto_csum_replace2(check, skb, *port, new_port, 0);
	*port = new_port;
}

/*=============================================================================
 *function name: mcast_helper_set_udp_port
 *description  : function to update the UDP port in skb
 *===========================================================================*/
static void mcast_helper_set_udp_port(struct sk_buff *skb, unsigned short *port,
				      unsigned short new_port)
{
	struct udphdr *udph = udp_hdr(skb);

	if (udph->check && skb->ip_summed != CHECKSUM_PARTIAL) {
		mcast_helper_set_port(skb, port, new_port, &udph->check);

		if (!udph->check)
			udph->check = CSUM_MANGLED_0;
	} else {
		*port = new_port;
	}
}

/*=============================================================================
 *function name: mcast_helper_set_udp
 *description  : function to update the UDP header in skb
 *===========================================================================*/
static void mcast_helper_set_udp(struct sk_buff *skb, unsigned short udp_src,
				 unsigned short udp_dst)
{
	struct udphdr *udph;
	int err;

	err = mcast_helper_make_skb_writeable(skb, skb_transport_offset(skb) +
					      sizeof(struct udphdr));
	if (unlikely(err))
		return;

	udph = udp_hdr(skb);
	if (udp_src != udph->source)
		mcast_helper_set_udp_port(skb, &udph->source, udp_src);

	if (udp_dst != udph->dest)
		mcast_helper_set_udp_port(skb, &udph->dest, udp_dst);

	return;
}

/*=============================================================================
 *function name: mcast_helper_set_sig
 *description  : function to insert the signature in skb
 *===========================================================================*/
static void mcast_helper_set_sig(struct sk_buff *skb, struct net_device *netdev,
				 int gid, int flag)
{
	u8 *data = NULL;
	u32 data_len = 0, extra_data_len = -1;
	int index = 0;

	if (flag == MCH_IPV6)
		data_len = skb->len - TOT6_HDR_LEN;
	else
		data_len = skb->len - TOT_HDR_LEN;
	extra_data_len = sizeof(mch_signature) + 8;
	index = sizeof(mch_signature) -1;
	data = (u8 *)udp_hdr(skb) + UDP_HDR_LEN;
	if (data_len > extra_data_len) {
		memcpy(data, mch_signature, sizeof(mch_signature));
		data[index] = (u8)(gid & 0xFF);
		data[index+1] = (u8)(netdev->ifindex & 0xFF);
	}
}

/*=============================================================================
 *function name: mcast_helper_acl_probe_pckt_send
 *description  : function to send the IPV4 probe packet
 *===========================================================================*/
static u32
mcast_helper_acl_probe_pckt_send(struct net_device *inetdev,
				 struct net_device *onetdev, int gid,
				 ip_addr_t *gaddr, ip_addr_t *saddr,
				 u32 proto, u32 sport, u32 dport)
{
	struct iphdr *iph = NULL;
	struct sk_buff *newskb = NULL;

	if (!skb_buff) {
		mch_debug("failed to send probe, pointer is NULL\n");
		return 0;
	}

	if (ip_hdr(skb_buff)->protocol == IPPROTO_UDP) {
		newskb = skb_copy(skb_buff, GFP_ATOMIC);
		if (newskb != NULL) {
			iph = (struct iphdr *) skb_network_header(newskb);

			mcast_helper_set_ipv4(newskb, saddr, gaddr);
			mcast_helper_set_udp(newskb, sport, dport);
#if 0
			mcast_helper_set_eth_addr(newskb, newskb->dev->dev_addr,
						  eth_hdr(newskb)->h_dest);
#endif
			newskb->dev = inetdev;
			mcast_helper_set_sig(newskb, onetdev, gid, MCH_IPV4);
			/* Insert the skb in to input queue */
			netif_receive_skb(newskb);
			return 1;
		}
	}

	return 0;
}

/*=============================================================================
 *function name: mcast_helper_acl_probe_pckt_send6
 *description  : function to send the IPV6 probe packet
 *===========================================================================*/
static u32
mcast_helper_acl_probe_pckt_send6(struct net_device *inetdev,
				  struct net_device *onetdev, int gid,
				  ip_addr_t *gaddr, ip_addr_t *saddr,
				  u32 proto, u32 sport, u32 dport)

{
	struct sk_buff *newskb = NULL;

	if (!skb_buff6) {
		mch_debug("failed to send probe6, pointer is NULL\n");
		return NF_ACCEPT;
	}

	if (ipv6_hdr(skb_buff6)->nexthdr == IPPROTO_UDP) {
		newskb = skb_copy(skb_buff6, GFP_ATOMIC);
		if (newskb != NULL) {
			mcast_helper_set_ipv6(newskb, saddr, gaddr);
			mcast_helper_set_udp(newskb, sport, dport);
#if 0
			mcast_helper_set_eth_addr(newskb, newskb->dev->dev_addr,
						  eth_hdr(newskb)->h_dest);
#endif
			newskb->dev = inetdev;
			mcast_helper_set_sig(newskb, onetdev, gid, MCH_IPV6);
			/* Insert the skb in to input queue */
			netif_receive_skb(newskb);
		}
	}

	return NF_ACCEPT;
}
#endif /* CONFIG_MCAST_HELPER_ACL */

/*=============================================================================
 *function name: mcast_helper_add_host_to_table
 *description  : function updates the member record and then send the probe
 *               packets by starting the probe packet expiry timer. Must be
 *               called under MCGID lock.
 *===========================================================================*/
static int
mcast_helper_add_host_into_table(mcast_table_t *mc_tbl,
				 struct net_device *mem_dev, u8 *mac)
{
	int ret = MCH_FAILURE;
	mcast_gid_t *mc_gid = NULL, *mc_gid_new = NULL;
	mcast_member_t *mc_mem = NULL, *mc_mem_new = NULL;
	mcast_mac_t *mc_mac = NULL;
	u32 flag = MCH_CB_F_UPD;
	u16 vid = VLAN_VID_MASK;

	if (!mc_tbl || !mem_dev || !mac) {
		mch_debug("failed to update host into table, NULL ptr\n");
		goto done;
	}

	if (mc_tbl->inactive) {
		/* Transition from DROP in HW to ACTIVE: reuse GID */
		mc_gid = mcast_helper_get_inactive_gid_record(mc_tbl);
		if (!mc_gid) {
			mch_debug("GID record not found in inactive entry\n");
			goto done;
		}
		/* Reusing GID, so update VLAN ID and other info */
		if (is_vlan_dev(mem_dev))
			vid = vlan_dev_vlan_id(mem_dev);
		mc_gid->eg_vlan = vid;
#ifdef CONFIG_MCAST_HELPER_ACL
		mc_gid->oif_bitmap = 0;
		mc_gid->probe_flag = 0;
#endif
		flag = MCH_CB_F_ADD;
	} else {
		/* Find GID record from member to add the host */
		mc_gid = mcast_helper_get_gid_record_by_mem_dev(mc_tbl, mem_dev);
	}

	if (!mc_gid) {
		/* Add new GID, so callback ADD need to invoke */
		flag = MCH_CB_F_ADD;
		mc_gid = mcast_helper_add_gid_record(mc_tbl, mem_dev);
		if (!mc_gid)
			goto done;
		mc_gid_new = mc_gid;
	}

	/* Find member record to add the new host */
	mc_mem = mcast_helper_search_mem_record(mc_gid, mem_dev);
	if (!mc_mem) {
		mc_mem = mcast_helper_add_mem_record(mc_gid, mem_dev);
		if (!mc_mem)
			goto done;
		mc_mem_new = mc_mem;
	}

	mc_mac = mcast_helper_search_mac_record(mc_mem, mac);
	if (mc_mac) {
		/* Prevent spamming the modules with callbacks */
		ret = MCH_SUCCESS;
		goto done;
	}
	mc_mac = mcast_helper_add_mac_record(mc_mem, mac);
	if (!mc_mac)
		goto done;

	if (!mch_acl_enabled) {
		ret = mcast_helper_invoke_return_callback(mc_mem, flag);
#ifdef CONFIG_MCAST_HELPER_ACL
	} else {
		/* Start the timer here */
		mcast_helper_start_helper_timer();
		mc_gid->probe_flag = 1;
		/* Send the Skb probe packet on interfaces */
		if (mc_tbl->mc_stream.src_ip.ip_type == MCH_IPV6) {
			mch_iptype = MCH_IPV6;
			mcast_helper_acl_probe_pckt_send6(
					mc_tbl->mc_stream.rx_dev, mem_dev,
					mc_gid->gid,
					&mc_tbl->mc_stream.dst_ip,
					&mc_tbl->mc_stream.src_ip,
					mc_tbl->mc_stream.proto,
					mc_tbl->mc_stream.src_port,
					mc_tbl->mc_stream.dst_port);
		} else {
			mch_iptype = MCH_IPV4;
			mcast_helper_acl_probe_pckt_send(
					mc_tbl->mc_stream.rx_dev,
					mem_dev, mc_gid->gid,
					&mc_tbl->mc_stream.dst_ip,
					&mc_tbl->mc_stream.src_ip,
					mc_tbl->mc_stream.proto,
					mc_tbl->mc_stream.src_port,
					mc_tbl->mc_stream.dst_port);
		}
		ret = MCH_SUCCESS;
#endif /* CONFIG_MCAST_HELPER_ACL */
	}

	/* Transition from DROP in HW to ACTIVE: stop timer, mark active */
	if (mc_tbl->inactive && (ret == MCH_SUCCESS)) {
		mch_debug("Transition from DROP in HW to ACTIVE: mark active\n");
		del_timer(&mc_tbl->inactive_timer);
		mc_tbl->inactive = false;
	}

done:
	if (ret == MCH_FAILURE) {
		/* For update case should not delete existing DB */
		if (mc_mac)
			mcast_helper_delete_mac_record(mc_mac);
		if (mc_mem_new)
			mcast_helper_delete_mem_record(mc_mem_new);
		if (mc_gid_new)
			mcast_helper_delete_gid_record(mc_gid_new);
	}
	return ret;
}

/*=============================================================================
 *function name: mcast_helper_should_deliver
 *description  : function to identify the bridge multicast stream is deliverable
 *               to this member port or not
 *===========================================================================*/
static bool
mcast_helper_should_deliver(mcast_table_t *mc_tbl, struct net_device *mem_dev)
{
	struct net_device *rx_master = NULL, *mem_master = NULL;
	bool ret = false;

	if (!mc_tbl || !mem_dev) {
		mch_debug("failed to check hairpin, NULL ptr\n");
		return false;
	}

	if (mc_tbl->mc_stream.rx_dev == mem_dev) {
		/* For same RX and TX port verify hairpin */
		if ((mc_tbl->rx_brport_flags & BR_HAIRPIN_MODE))
			ret = true;
		return ret;
	}

	rcu_read_lock();
	mem_master = netdev_master_upper_dev_get_rcu(mem_dev);
	rx_master = netdev_master_upper_dev_get_rcu(mc_tbl->mc_stream.rx_dev);
	/* Verify both are under same bridge */
	if (rx_master && netif_is_bridge_master(rx_master) &&
	    mem_master && netif_is_bridge_master(mem_master)) {
		ret = (rx_master == mem_master);
	}
	rcu_read_unlock();

	return ret;
}

/*=============================================================================
 *function name: mcast_helper_update_entry
 *description  : function to identify the MCGID record based on the parameters
 *               from user space and update the host into GID/member record
 *===========================================================================*/
/* Call the function to check if GID exist for this group in MCGID table */
static int mcast_helper_update_entry(mcast_rec_t *mc_rec, u8 op)
{
	int ret = MCH_SUCCESS;
	struct net_device *rx_dev, *mem_dev;
	mcast_table_t *mc_tbl = NULL;
	struct list_head *mcgid_list;
	int num_entry = 0, i;
	u8 *mac = mc_rec->macaddr;

	mem_dev = mch_get_netif(mc_rec->mem_ifname);
	if (!mem_dev)
		return -ENXIO;
	rx_dev = mch_get_netif(mc_rec->rx_ifname);

	if (mc_rec->group_ip.ip_type == MCH_IPV4)
		mch_debug("%s request to [%pI4]@[%pI4]", (op) ? "UPDATE" : "ADD",
			  &mc_rec->src_ip.addr, &mc_rec->group_ip.addr);
	else if (mc_rec->group_ip.ip_type == MCH_IPV6)
		mch_debug("%s request to [%pI6c]@[%pI6c]", (op) ? "UPDATE" : "ADD",
			  &mc_rec->src_ip.addr, &mc_rec->group_ip.addr);

	mcgid_list = mcast_helper_list_p(mc_rec->group_ip.ip_type);
	spin_lock_bh(&mch_mcgid_lock);
	num_entry = mcast_helper_search_table_entry(rx_dev, &mc_rec->src_ip,
			&mc_rec->group_ip, mcgid_list);
	mch_debug("%s(%pM) into(%s)# for %d stream(s) in table",
		  (op) ? "UPDATE" : "ADD", mac, mem_dev->name, num_entry);

	for (i = 0; i < num_entry && i < MCGID_MAX_SIZE; i++) {
		/* There may be multiple stream, need to join all (for ASM) */
		mc_tbl = g_mc_table[i];
		if (!mc_tbl)
			continue;
		/* For bridge multicast, member should be on same bridge */
		if (netif_is_bridge_port(mc_tbl->mc_stream.rx_dev) &&
		    !mcast_helper_should_deliver(mc_tbl, mem_dev))
			continue;
		ret = mcast_helper_add_host_into_table(mc_tbl, mem_dev, mac);
		if (ret)
			break;
	}
	spin_unlock_bh(&mch_mcgid_lock);

	return ret;
}

/*=============================================================================
 *function name: mcast_helper_del_host_from_member
 *description  : function deletes the host from GID/member record and inform
 *               subscribers. Must be called under MCGID lock.
 *===========================================================================*/
static int
mcast_helper_del_host_from_table(mcast_table_t *mc_tbl,
				 struct net_device *mem_dev, mcast_rec_t *mc_rec)
{
	int ret = MCH_SUCCESS;
	mcast_gid_t *mc_gid = NULL;
	mcast_member_t *mc_mem = NULL;
	mcast_mac_t *mc_mac = NULL;
	u32 flag = MCH_CB_F_DEL;

	if (!mc_tbl || !mem_dev || !mc_rec) {
		mch_debug("failed to delete host from member record, NULL ptr\n");
		return MCH_FAILURE;
	}

	/* Find the GID record to delete the host */
	mc_gid = mcast_helper_get_gid_record_by_mem_dev(mc_tbl, mem_dev);
	if (!mc_gid)
		goto done;

	/* Find the member record to delete the host */
	mc_mem = mcast_helper_search_mem_record(mc_gid, mem_dev);
	if (!mc_mem)
		goto done;

	/* Find the MAC record to delete */
	mc_mac = mcast_helper_search_mac_record(mc_mem, mc_rec->macaddr);
	if (!mc_mac)
		goto done;

	if (mc_mem->macaddr_count <= 1) {
		if ((mch_list_count_nodes(&mc_tbl->gid_list) == 1) &&
		    (mch_list_count_nodes(&mc_gid->member_list) == 1)) {
			/* DISCONNECT/LEAVE - start inactive timer. This
			 * timer will expire in multicast membership
			 * timeout (from daemon) + 10 sec.
			 */
			mod_timer(&mc_tbl->inactive_timer, jiffies +
				  (mc_rec->mem_interval + 10) * HZ);
			mch_debug("Transition from ACTIVE to DROP: timeout=%u\n",
				  mc_rec->mem_interval + 10);
			/* Transition from ACTIVE to DROP: mark as inactive */
			mc_tbl->inactive = true;
			flag |= MCH_CB_F_DROP;
		}

		/* MAC count 1 and matching, first inform then remove */
		ret = mcast_helper_invoke_return_callback(mc_mem, flag);
		mcast_helper_delete_mac_record(mc_mac);
		mcast_helper_delete_mem_record(mc_mem);
	} else {
		flag = MCH_CB_F_DEL_UPD;
		mcast_helper_delete_mac_record(mc_mac);
		ret = mcast_helper_invoke_return_callback(mc_mem, flag);
	}

	/* If no member, delete GID record from MCGID table */
	mcast_helper_delete_gid_record(mc_gid);

	/* If no GID, delete MCGID table entry */
	mcast_helper_delete_table_entry(mc_tbl);

done:
	return ret;
}

/*=============================================================================
 *function name: mcast_helper_delete_entry
 *description  : function to identify the MCGID record based on the parameters
 *               from user space and delete the host from GID/member record
 *===========================================================================*/
/* Call the function to check if GID exist for this group in MCGID table */
static int mcast_helper_delete_entry(mcast_rec_t *mc_rec)
{
	int ret = MCH_SUCCESS;
	struct net_device *rx_dev, *mem_dev;
	mcast_table_t *mc_tbl = NULL;
	struct list_head *mcgid_list;
	int num_entry = 0, i;

	mem_dev = mch_get_netif(mc_rec->mem_ifname);
	if (!mem_dev)
		return -ENXIO;
	rx_dev = mch_get_netif(mc_rec->rx_ifname);

	if (mc_rec->group_ip.ip_type == MCH_IPV4)
		mch_debug("delete request from [%pI4]@[%pI4]",
			  &mc_rec->src_ip.addr, &mc_rec->group_ip.addr);
	else if (mc_rec->group_ip.ip_type == MCH_IPV6)
		mch_debug("delete request from [%pI6c]@[%pI6c]",
			  &mc_rec->src_ip.addr, &mc_rec->group_ip.addr);

	mcgid_list = mcast_helper_list_p(mc_rec->group_ip.ip_type);
	spin_lock_bh(&mch_mcgid_lock);
	num_entry = mcast_helper_search_table_entry(rx_dev, &mc_rec->src_ip,
			&mc_rec->group_ip, mcgid_list);
	mch_debug("DELETE(%pM) from(%s)# for %d streams in table\n",
		  mc_rec->macaddr, mem_dev->name, num_entry);

	for (i = 0; i < num_entry && i < MCGID_MAX_SIZE; i++) {
		/* There may be multiple stream, leave from all (for ASM) */
		mc_tbl = g_mc_table[i];
		if (!mc_tbl)
			continue;
		ret = mcast_helper_del_host_from_table(mc_tbl, mem_dev, mc_rec);
		if (ret)
			break;
	}
	spin_unlock_bh(&mch_mcgid_lock);

	return ret;
}

/*=============================================================================
 *function name: mcast_helper_ioctl
 *description  : IOCTL handler functon
 *===========================================================================*/
static long mcast_helper_ioctl(struct file *f, u32 cmd, unsigned long arg)
{
	long ret = 0;
	mcast_rec_t mc_rec;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	switch (cmd) {
	case MCH_MEMBER_ENTRY_ADD:
	case MCH_MEMBER_ENTRY_UPDATE:
	case MCH_MEMBER_ENTRY_REMOVE:
		if (copy_from_user(&mc_rec, (mcast_rec_t *)arg,
		    sizeof(mcast_rec_t))) {
			ret = -EACCES;
			break;
		}
		if (mcast_helper_is_ll_multicast(&mc_rec.group_ip)) {
			ret = -EINVAL;
			break;
		}
		if (is_zero_ether_addr(mc_rec.macaddr)) {
			mch_debug("MAC is not available in request\n");
			return -EINVAL;
		}

		if (cmd == MCH_MEMBER_ENTRY_ADD)
			ret = mcast_helper_update_entry(&mc_rec, 0);
		else if (cmd == MCH_MEMBER_ENTRY_UPDATE)
			ret = mcast_helper_update_entry(&mc_rec, 1);
		else if (cmd == MCH_MEMBER_ENTRY_REMOVE)
			ret = mcast_helper_delete_entry(&mc_rec);
		if (ret)
			break;

		if (copy_to_user((mcast_rec_t *)arg, &mc_rec,
		    sizeof(mcast_rec_t))) {
			ret = -EACCES;
			break;
		}
		break;
	case MCH_SERVER_ENTRY_GET:
	default:
		ret = -ENOTSUPP;
		break;
	}

	return ret;
}

static int mcast_helper_open(struct inode *i, struct file *f)
{
	return 0;
}

static int mcast_helper_close(struct inode *i, struct file *f)
{
	return 0;
}

static struct file_operations mcast_helper_fops = {
	.owner = THIS_MODULE,
	.open = mcast_helper_open,
	.release = mcast_helper_close,
	.unlocked_ioctl = mcast_helper_ioctl
};

/*=============================================================================
 *Function Name: mcast_helper_update_mcgid_table
 *Description  : Function to ADD/UPDATE MCGID entry for a stream and invoke
 *               callback to subscribers
 *===========================================================================*/
static void
mcast_helper_update_mcgid_table(struct sk_buff *skb, unsigned long port_flags,
				ip_addr_t *saddr, ip_addr_t *gaddr, u32 proto,
				u32 sPort, u32 dPort,
				struct net_device *mem_dev, u8 *host_mac)
{
	mcast_table_t *mc_tbl = NULL;
	mcast_gid_t *mc_gid = NULL;
	mcast_member_t *mc_mem = NULL;
	mcast_mac_t *mc_mac = NULL;
	struct net_device *rx_dev = NULL;
	struct list_head *mcgid_list = mcast_helper_list_p(gaddr->ip_type);
	int num_entry = 0, status = MCH_FAILURE;
	/* XXX: GID must be handled by module callback API */
	int gid = MCH_GID_ERR;
	u32 flag = MCH_CB_F_UPD;
	u8 *src_mac = eth_hdr(skb)->h_source;

	if (!mem_dev || !host_mac)
		return;

	/*
	 * T&C for stream creating MCGID:
	 * 1. Host tracking must be supported by Kernel
	 * 2. Callback to ADD/UPDATE (for 2nd host onwards - if already joined)
	 */
	rx_dev = mch_get_netif_by_idx(skb->skb_iif);
	if (!rx_dev)
		return;

	spin_lock_bh(&mch_mcgid_lock);
	/* Handle MCGID table entry */
	num_entry = mcast_helper_search_table_entry(rx_dev, saddr, gaddr,
						    mcgid_list);
	if (!num_entry) {
		/* Create MCGID table entry */
		mc_tbl = mcast_helper_add_table_entry(rx_dev, port_flags,
				saddr, gaddr, proto, sPort, dPort, src_mac,
				mem_dev, host_mac, mcgid_list);
	} else {
		/* Stream is unique, so will have single entry */
		mc_tbl = g_mc_table[0];
		/* Stream for drop session: skip learning */
		if (mc_tbl->inactive) {
			printk_ratelimited(KERN_DEBUG
					   "Stream for drop session - skip\n");
			goto done;
		}
	}
	if (!mc_tbl)
		goto done;

	/* Handle GID record */
	mc_gid = mcast_helper_get_gid_record_by_mem_dev(mc_tbl, mem_dev);
	if (!mc_gid) {
		/* Add new GID, so callback ADD need to invoke */
		flag = MCH_CB_F_ADD;
		mc_gid = mcast_helper_add_gid_record(mc_tbl, mem_dev);
		if (!mc_gid)
			goto done;
	}

	/* Handle member record */
	mc_mem = mcast_helper_search_mem_record(mc_gid, mem_dev);
	if (!mc_mem) {
		mc_mem = mcast_helper_add_mem_record(mc_gid, mem_dev);
		if (!mc_mem)
			goto done;
	}

	/* Handle MAC record for host tracking of member device */
	mc_mac = mcast_helper_search_mac_record(mc_mem, host_mac);
	/* Prevent spamming the modules with callbacks */
	if (mc_mac) {
		status = MCH_SUCCESS;
		goto done;
	}
	mc_mac = mcast_helper_add_mac_record(mc_mem, host_mac);
	if (!mc_mac)
		goto done;

	status = mcast_helper_invoke_return_callback(mc_mem, flag);

done:
	if ((status == MCH_FAILURE) && (flag == MCH_CB_F_ADD)) {
		/* For update case should not delete existing DB */
		if (mc_mac)
			mcast_helper_delete_mac_record(mc_mac);
		if (mc_mem)
			mcast_helper_delete_mem_record(mc_mem);
		if (mc_gid)
			mcast_helper_delete_gid_record(mc_gid);
		if (mc_tbl)
			mcast_helper_delete_table_entry(mc_tbl);
	}
	if (mc_gid && (status == MCH_SUCCESS))
		gid = (int)mc_gid->gid;
	spin_unlock_bh(&mch_mcgid_lock);

	/* Attach MCGID into skb */
	mcast_helper_set_skb_gid(skb, gid);

	return;
}

/*=============================================================================
 *Function Name: mcast_helper_bridge_learning
 *Description  : Function to retrieve 5-tuple info from IPV4/IPV6 stream.
 *               ADD/UPDATE callback to subscribers for host already JOINed.
 *===========================================================================*/
static void
mcast_helper_bridge_learning(struct sk_buff *skb, struct net_device *mem_dev,
			     unsigned long port_flags, u8 *host_mac)
{
	ip_addr_t saddr, gaddr;

	spin_lock_bh(&mch_mcgid_lock);
	/* If all GIDs are exhausted and no inactive entry,
	 * do not learn from new stream.
	 */
	if (mcast_helper_is_mcgid_full() &&
	    !mcast_helper_get_inactive_table_entry()) {
		spin_unlock_bh(&mch_mcgid_lock);
		return;
	}
	spin_unlock_bh(&mch_mcgid_lock);

	if (skb->protocol == htons(ETH_P_IP)) {
		struct iphdr *iph = ip_hdr(skb);
		struct udphdr *udph = (struct udphdr *)((u8 *)iph + (iph->ihl << 2));

		mcast_helper_init_ipaddr(&gaddr, MCH_IPV4, &iph->daddr);
		if (mcast_helper_is_ll_multicast(&gaddr))
			return;

		mcast_helper_init_ipaddr(&saddr, MCH_IPV4, &iph->saddr);
#ifdef CONFIG_MCAST_HELPER_ACL
		if (mch_captured_skb && mch_acl_enabled) {
			skb_buff = skb_copy(skb, GFP_ATOMIC);
			mch_captured_skb = 0;
		}
#endif

		mcast_helper_update_mcgid_table(skb, port_flags, &saddr, &gaddr,
						iph->protocol, udph->source,
						udph->dest, mem_dev, host_mac);
	} else if (skb->protocol == htons(ETH_P_IPV6)) {
		struct ipv6hdr *iph6 = ipv6_hdr(skb);
		/*
		 * XXX: IP packet received from bridge hook,
		 * cannot use udp_hdr() directly to get udp details
		 */
		struct udphdr *udph6 = (struct udphdr *)((u8 *)iph6 + IP6_HDR_LEN);

		mcast_helper_init_ipaddr(&gaddr, MCH_IPV6, &iph6->daddr);
		if (mcast_helper_is_ll_multicast(&gaddr))
			return;

		mcast_helper_init_ipaddr(&saddr, MCH_IPV6, &iph6->saddr);
#ifdef CONFIG_MCAST_HELPER_ACL
		if (mch_captured_skb6 && mch_acl_enabled) {
			skb_buff6 = skb_copy(skb, GFP_ATOMIC);
			mch_captured_skb6 = 0;
		}
#endif

		mcast_helper_update_mcgid_table(skb, port_flags, &saddr, &gaddr,
						iph6->nexthdr, udph6->source,
						udph6->dest, mem_dev, host_mac);
	}

	return;
}

#ifdef CONFIG_MCAST_HELPER_ACL
/*=============================================================================
 *function name: mcast_helper_extract_gid
 *description  : function to retrieve the group index from the data buffer
 *===========================================================================*/
static u32 mcast_helper_extract_gid(char *data, int offset)
{
	u32 gid = 0;

	gid = (u32) (data[offset] & 0xFF);

	return gid;
}

/*=============================================================================
 *function name: mcast_helper_extract_intrfidx
 *description  : function to retrieve the interface index from the data buffer
 *===========================================================================*/

static u32 mcast_helper_extract_intrfidx(char *data, int offset)
{
	u32 intrfidx = 0;

	intrfidx = (u8) (data[offset] & 0xFF);

	return intrfidx;
}

/*=============================================================================
 *function name: mcast_helper_sig_check
 *description  : function to check the signature in the received probe packet
 *===========================================================================*/
static inline int mcast_helper_sig_check(u8 *data)
{
	return (memcmp(data, mch_signature, (sizeof(mch_signature) - 1)) == 0);
}

/*=============================================================================
 *function name: mcast_helper_sig_check_update_ip
 *description  : function to check signature and update the member table and
 *               inform subscribers for IPv4 packet
 *===========================================================================*/
static int mcast_helper_sig_check_update_ip(struct sk_buff *skb)
{
	u8 *data;
	u32 gid = 0;
	int intrfid = 0;
	struct list_head *mcgid_list = mcast_helper_list_p(MCH_IPV4);
	mcast_gid_t *mc_gid = NULL;
	mcast_member_t *mc_mem = NULL;

	if (ip_hdr(skb)->protocol == IPPROTO_UDP)
		return 0;

	data = (u8 *)udp_hdr(skb) + UDP_HDR_LEN;

	if (mcast_helper_sig_check(data) == 0)
		return 0;

	/*
	 * Signature matched now extract the grpindex
	 * and the call update member table
	 */
	gid = mcast_helper_extract_gid(data, sizeof(mch_signature) - 1);
	intrfid = mcast_helper_extract_intrfidx(data, sizeof(mch_signature));
	spin_lock_bh(&mch_mcgid_lock);
	mc_gid = mcast_helper_get_gid_record_by_gid_num(mcgid_list, gid);
	if (!mc_gid)
		goto done;

	if (skb->dev->ifindex == intrfid) {
		/*
		 * Update the member table to add the
		 * new interface into the list
		 */
		mc_mem = mcast_helper_search_mem_record(mc_gid, skb->dev);
		if (mc_mem) {
			mc_mem->acl_blocked = 0;
			mcast_helper_invoke_return_callback(mc_mem, MCH_CB_F_ADD);
		}
	}
	/*
	 * Update the oifindex bitmap to be used
	 * for evaluating after timer expires
	 */
	mc_gid->oif_bitmap |= (1ULL << skb->dev->ifindex);

done:
	spin_unlock_bh(&mch_mcgid_lock);
	return 1;
}

/*=============================================================================
 *function name: mcast_helper_sig_check_update_ip6
 *description  : function to check signature and update the member table and
 *               inform subscribers for the IPv6 packet
 *===========================================================================*/
static int mcast_helper_sig_check_update_ip6(struct sk_buff *skb)
{
	u8 *data;
	u32 gid = 0;
	int intrfid = 0;
	struct list_head *mcgid_list = mcast_helper_list_p(MCH_IPV6);
	mcast_gid_t *mc_gid = NULL;
	mcast_member_t *mc_mem = NULL;

	data = (u8 *)udp_hdr(skb) + UDP_HDR_LEN;

	if (mcast_helper_sig_check(data) == 0)
		return 0;

	/*
	 * Signature matched now extract the grpindex
	 * and the call update member table
	 */
	gid = mcast_helper_extract_gid(data, sizeof(mch_signature) - 1);
	intrfid = mcast_helper_extract_intrfidx(data, sizeof(mch_signature));
	spin_lock_bh(&mch_mcgid_lock);
	mc_gid = mcast_helper_get_gid_record_by_gid_num(mcgid_list, gid);
	if (!mc_gid)
		goto done;

	if (skb->dev->ifindex == intrfid) {
		/*
		 * Update the member table to add the
		 * new interface into the list
		 */
		mc_mem = mcast_helper_search_mem_record(mc_gid, skb->dev);
		if (mc_mem) {
			mc_mem->acl_blocked = 0;
			mcast_helper_invoke_return_callback(mc_mem, MCH_CB_F_UPD);
		}
	}
	/*
	 * Update the oifindex bitmap to be used
	 * for evaluating after timer expires
	 */
	mc_gid->oif_bitmap |= (1ULL << skb->dev->ifindex);

done:
	spin_unlock_bh(&mch_mcgid_lock);
	return 1;
}

/*=============================================================================
 *function name: mcast_helper_sig_check_update
 *description  : function to check signature and call corresponding callbacks
 *===========================================================================*/
static int mcast_helper_sig_check_update(struct sk_buff *skb)
{
	const u8 *dest = eth_hdr(skb)->h_dest;

	if (!mch_acl_enabled)
		return 1;

	if (mch_timerstarted && is_multicast_ether_addr(dest)) {
		if (eth_hdr(skb)->h_proto == ETH_P_IP) {
			if (ip_hdr(skb)->protocol == IPPROTO_UDP)
				mcast_helper_sig_check_update_ip(skb);
		} else if (eth_hdr(skb)->h_proto == ETH_P_IPV6) {
			if (ipv6_hdr(skb)->nexthdr == IPPROTO_UDP)
				mcast_helper_sig_check_update_ip6(skb);
		}
	}

	return 1;
}
#endif /* CONFIG_MCAST_HELPER_ACL */

/* XXX: NETDEV operation API */

/*=============================================================================
 * function name: mcast_helper_clear_all_mac_from_mem
 * description  : Function clears all clients and inform subscribers.
 *                Must be called under mch_mcgid_lock.
 *===========================================================================*/
static void
mcast_helper_clear_all_mac_from_mem(mcast_member_t *mc_mem)
{
	mcast_mac_t *mc_mac = NULL, *tmp = NULL;

	if (!mc_mem) {
		mch_debug("failed to delete all MAC, pointer is NULL\n");
		return;
	}

	/* Send delete callback for all MACs */
	mcast_helper_invoke_return_callback(mc_mem, MCH_CB_F_DEL);
	list_for_each_entry_safe(mc_mac, tmp, &mc_mem->macaddr_list, list) {
		mcast_helper_delete_mac_record(mc_mac);
	}
}

/*=============================================================================
 * function name: mcast_helper_clear_mem_from_gid
 * description  : Function searches and clears all clients and inform
 *                subscribers. Delete the member record and if member list
 *                empty delete GID record. Also, if GID list is empty, delete
 *                the table entry. Must be called under mch_mcgid_lock.
 *===========================================================================*/
static void
mcast_helper_clear_mem_from_gid(mcast_table_t *mc_tbl, struct net_device *netif)
{
	mcast_gid_t *mc_gid = NULL, *tmp;
	mcast_member_t *mc_mem = NULL;

	if (!mc_tbl || !netif)
		return;

	/* Loop the GID record to clear member and MAC */
	list_for_each_entry_safe(mc_gid, tmp, &mc_tbl->gid_list, list) {
		/* Search group membership for the device */
		mc_mem = mcast_helper_search_mem_record(mc_gid, netif);
		if (!mc_mem)
			continue;

		/* Delete all of its associated clients */
		mcast_helper_clear_all_mac_from_mem(mc_mem);

		/* Delete member from GID record */
		mcast_helper_delete_mem_record(mc_mem);

		/* If no member, delete GID record from MCGID table */
		mcast_helper_delete_gid_record(mc_gid);
	}
}

/*=============================================================================
 * function name: mcast_helper_clear_table_entry
 * description  : Function clears all GID, member and client record from DB and
 *                inform subscribers when RX stream interface went DOWN.
 *===========================================================================*/
static void
mcast_helper_clear_table_entry(mch_ptype_t type, struct net_device *rx_dev)
{
	mcast_table_t *mc_tbl = NULL, *tmp_tbl = NULL;
	mcast_gid_t *mc_gid = NULL, *tmp_gid = NULL;
	mcast_member_t *mc_mem = NULL, *tmp_mem = NULL;
	struct list_head *mcgid_list = mcast_helper_list_p(type);

	if (!mcgid_list || !rx_dev)
		return;

	/* Loop the MCGID table entry (IPv4/IPv6) */
	spin_lock_bh(&mch_mcgid_lock);
	list_for_each_entry_safe(mc_tbl, tmp_tbl, mcgid_list, list) {
		/* If RX interface DOWN then only clear entire list */
		if (rx_dev != mc_tbl->mc_stream.rx_dev)
			continue;

		/* If inactive, Transition from DROP in HW to DELETE */
		if (mc_tbl->inactive) {
			mcast_helper_cleanup_inactive_table_entry(mc_tbl);
			continue;
		}

		list_for_each_entry_safe(mc_gid, tmp_gid,
					 &mc_tbl->gid_list, list) {
			list_for_each_entry_safe(mc_mem, tmp_mem,
						 &mc_gid->member_list, list) {
				mcast_helper_clear_all_mac_from_mem(mc_mem);
				/* If no MAC, delete member from GID record */
				mcast_helper_delete_mem_record(mc_mem);
			}
			/* If no member, delete GID record from MCGID table */
			mcast_helper_delete_gid_record(mc_gid);
		}
		/* If no GID, delete MCGID table entry */
		mcast_helper_delete_table_entry(mc_tbl);
	}
	spin_unlock_bh(&mch_mcgid_lock);
}

/*=============================================================================
 * function name: mcast_helper_clear_mem_and_mac
 * description  : Function clears client record from DB and inform subscribers
 *===========================================================================*/
static void
mcast_helper_clear_mem_and_mac(mch_ptype_t type, struct net_device *netif)
{
	mcast_table_t *mc_tbl = NULL, *tmp = NULL;
	struct list_head *mcgid_list = mcast_helper_list_p(type);

	if (!mcgid_list || !netif)
		return;

	/* Loop the MCGID table entry (IPv4/IPv6) */
	spin_lock_bh(&mch_mcgid_lock);
	list_for_each_entry_safe(mc_tbl, tmp, mcgid_list, list) {
		/* If inactive, Transition from DROP in HW to DELETE */
		if (mc_tbl->inactive) {
			mcast_helper_cleanup_inactive_table_entry(mc_tbl);
			continue;
		}

		/* Search and clear member and it's clients */
		mcast_helper_clear_mem_from_gid(mc_tbl, netif);

		/* If no GID, delete MCGID table entry */
		mcast_helper_delete_table_entry(mc_tbl);
	}
	spin_unlock_bh(&mch_mcgid_lock);
}

/* Member entry deletion from netdev unregister event */
static int mcast_helper_netdevice_event(struct notifier_block *nb,
					unsigned long action, void *ptr)
{
	struct net_device *netif = NULL;

	netif = netdev_notifier_info_to_dev((struct netdev_notifier_info *)ptr);

	switch (action) {
	case NETDEV_DOWN:
		/* Delete all entries when RX stream interface DOWN.
		 * When member interface down, handled by daemon. */
		mcast_helper_clear_table_entry(MCH_IPV4, netif);
		mcast_helper_clear_table_entry(MCH_IPV6, netif);
		break;
	case NETDEV_UNREGISTER:
		/* Search and delete member and all associated clients */
		mcast_helper_clear_mem_and_mac(MCH_IPV4, netif);
		mcast_helper_clear_mem_and_mac(MCH_IPV6, netif);
		break;
	}

	return 0;
}

struct notifier_block mcast_helper_netdevice_notifier = {
	.notifier_call = mcast_helper_netdevice_event
};

#ifdef CONFIG_MCAST_HELPER_ACL
static void mcast_helper_timer_handler_probe(mcast_gid_t *mc_gid)
{
	mcast_member_t *mc_mem = NULL, *tmp = NULL;
	u32 i = 0, delflag = 1;
	u64 oif_bitmap = 0;

	if (!mc_gid) {
		mch_debug("failed to handle probe, pointer is NULL\n");
		return;
	}

	list_for_each_entry_safe(mc_mem, tmp, &mc_gid->member_list, list) {
		oif_bitmap = mc_gid->oif_bitmap;
		i = 0;
		delflag = 1;
		do {
			if (!(oif_bitmap & 0x1)) {
				i++;
				continue;
			}
			if (mc_mem->mem_dev->ifindex == i) {
				if (mc_mem->acl_blocked == 1) {
					mcast_helper_invoke_return_callback(
							mc_mem, MCH_CB_F_ADD);
					mc_mem->acl_blocked = 0;
				}
				delflag = 0;
				break;
			}
			i++;
		} while (oif_bitmap >>= 1);

		if (delflag == 1) {
			/* Delete this interface from the member list
			 * and inform subscribers for this if any.
			 */
			mcast_helper_invoke_return_callback(mc_mem, MCH_CB_F_DEL);
			mc_mem->acl_blocked = 1;
		}
	}
	mc_gid->oif_bitmap = 0;
	mc_gid->probe_flag = 0;
}
#endif /* CONFIG_MCAST_HELPER_ACL */

/*=============================================================================
 *function name: mcast_helper_timer_handler
 *description  : function handling mcast helper timer expiry
 *===========================================================================*/
static void mcast_helper_timer_handler(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 15, 0)
		unsigned long data
#else
		struct timer_list *timer
#endif
		)
{
	mcast_gid_t *mc_gid = NULL, *tmp = NULL;
	struct list_head *mcgid_list = NULL;

	mch_debug("helper timer invoked\n");
	if (mch_timermod) {
		mch_timermod = 0;
		return;
	}

	if (mch_iptype == MCH_IPV6)
		mcgid_list = mcast_helper_list_p(MCH_IPV6);
	else
		mcgid_list = mcast_helper_list_p(MCH_IPV4);

	if (!mcgid_list)
		goto done;

	spin_lock_bh(&mch_mcgid_lock);
	list_for_each_entry_safe(mc_gid, tmp, mcgid_list, list) {
#ifdef CONFIG_MCAST_HELPER_ACL
		if (mc_gid->probe_flag == 1)
			mcast_helper_timer_handler_probe(mc_gid);
#endif
		/* TODO: check if no member then free it */
	}
	spin_unlock_bh(&mch_mcgid_lock);

done:
	mch_iptype = 0;
	mch_timerstarted = 0;
}

#ifdef CONFIG_MCAST_HELPER_ACL
/*=============================================================================
 *function name: mcast_helper_start_helper_timer
 *description  : function starting/modifying helper timer
 *===========================================================================*/
static void mcast_helper_start_helper_timer(void)
{
	if (!mch_timerstarted) {
		mch_debug("starting helper timer\n");
		mch_helper_timer.expires = jiffies + (MCH_UPDATE_TIMER * HZ);
		add_timer(&mch_helper_timer);
		mch_timerstarted = 1;
	} else {
		mch_debug("modifying helper timer\n");
		mch_timermod = 1;
		mod_timer(&mch_helper_timer, jiffies + MCH_UPDATE_TIMER * HZ);
		mch_timermod = 0;
	}
}
#endif /* CONFIG_MCAST_HELPER_ACL */

#ifdef CONFIG_PROC_FS
/*=============================================================================
 *function name: mcast_helper_show_ipv4
 *description  : proc support to read and output the IPv4 table entries
 *===========================================================================*/
static void mcast_helper_show_ipv4(struct seq_file *seq, mcast_gid_t *mc_gid)
{
	char src_mac[MCH_MAC_STR_LEN] = "(NA)       ";
	mcast_stream_t *stream;

	if (!mc_gid || !mc_gid->mc_table)
		return;

	stream = &mc_gid->mc_table->mc_stream;
	if (!is_zero_ether_addr(stream->src_mac))
		snprintf(src_mac, sizeof(src_mac), "(%pM)", stream->src_mac);
	seq_printf(seq, "%3d %15s %15pI4 %15pI4 %5d %5d %5d %19s",
		   mc_gid->gid, stream->rx_dev->name, &stream->src_ip.addr,
		   &stream->dst_ip.addr, stream->proto, ntohs(stream->src_port),
		   ntohs(stream->dst_port), src_mac);
}

/*=============================================================================
 *function name: mcast_helper_show_ipv6
 *description  : proc support to read and output the IPv6 table entries
 *===========================================================================*/
static void
mcast_helper_show_ipv6(struct seq_file *seq, mcast_gid_t *mc_gid)
{
	char src_mac[MCH_MAC_STR_LEN] = "(NA)       ";
	mcast_stream_t *stream;

	if (!mc_gid || !mc_gid->mc_table)
		return;

	stream = &mc_gid->mc_table->mc_stream;
	if (!is_zero_ether_addr(stream->src_mac))
		snprintf(src_mac, sizeof(src_mac), "(%pM)", stream->src_mac);
	seq_printf(seq, "%3d %15s %39pI6 %39pI6 %5d %5d %5d %19s",
		   mc_gid->gid, stream->rx_dev->name, &stream->src_ip.addr,
		   &stream->dst_ip.addr, stream->proto, ntohs(stream->src_port),
		   ntohs(stream->dst_port), src_mac);
}

/*=============================================================================
 *function name: mcast_helper_show
 *description  : Proc support to read and output the MCGID table entries
 *===========================================================================*/
static int
mcast_helper_show_entry(struct seq_file *seq, struct list_head *mcgid_list)
{
	mcast_table_t *mc_tbl = NULL;
	mcast_gid_t *mc_gid = NULL;
	mcast_member_t *mc_mem = NULL;
	mcast_mac_t *mc_mac = NULL;

	rcu_read_lock();
	list_for_each_entry_rcu(mc_tbl, mcgid_list, list) {
		list_for_each_entry_rcu(mc_gid, &mc_tbl->gid_list, list) {
			if (mc_tbl->mc_stream.src_ip.ip_type == MCH_IPV4)
				mcast_helper_show_ipv4(seq, mc_gid);
			else if (mc_tbl->mc_stream.src_ip.ip_type == MCH_IPV6)
				mcast_helper_show_ipv6(seq, mc_gid);
			else
				continue;
			if (mc_tbl->inactive)
				seq_printf(seq, " NA");
			list_for_each_entry_rcu(mc_mem,
						&mc_gid->member_list, list) {
				list_for_each_entry_rcu(mc_mac,
						&mc_mem->macaddr_list, list) {
					seq_printf(seq, " %s(%pM)",
						   mc_mem->mem_dev->name,
						   mc_mac->macaddr);
#ifdef CONFIG_MCAST_HELPER_ACL
					if (mch_acl_enabled) {
						seq_printf(seq, "(%d)",
							   mc_mem->acl_blocked);
					}
#endif
				}
			}
			seq_printf(seq, "\n");
		}
	}
	rcu_read_unlock();

	return 0;
}

/*=============================================================================
 *function name: mcast_helper_seq_show
 *description  : proc support to read and output IPV4 mcast helper record
 *===========================================================================*/
static int mcast_helper_seq_show(struct seq_file *seq, void *v)
{
	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	seq_printf(seq, "%3s %11s %12s %15s %11s %5s %5s %12s %24s%s\n",
		   "GIdx", "RxIntrf", "SA", "GA", "proto", "sPort", "dPort", "sMAC",
		   "memIntrf(MacAddr)", (mch_acl_enabled) ? "(AclFlag)" : "");

	mcast_helper_show_entry(seq, mcast_helper_list_p(MCH_IPV4));

	return 0;
}

/*=============================================================================
 *function name: mcast_helper_seq_show6
 *description  : proc support to read and output IPV6 mcast helper record
 *===========================================================================*/
static int mcast_helper_seq_show6(struct seq_file *seq, void *v)
{
	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	seq_printf(seq, "%3s %11s %24s %38s %24s %5s %5s %12s %24s%s\n",
		   "GIdx", "RxIntrf", "SA", "GA", "proto", "sPort", "dPort", "sMAC",
		   "memIntrf(MacAddr)", (mch_acl_enabled) ? "(AclFlag)" : "");

	mcast_helper_show_entry(seq, mcast_helper_list_p(MCH_IPV6));

	return 0;
}

/*=============================================================================
 *function name: mcast_debug_seq_show
 *description  : proc support to read and output debug status
 *===========================================================================*/
static int mcast_debug_seq_show(struct seq_file *m, void *v)
{
	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	seq_printf(m, "MCH: debug is %sabled\n", (mcast_debug) ? "en" : "dis");

	return 0;
}

/*=============================================================================
 *function name: mcast_debug_proc_write
 *description  : proc support to enable/disable (1/0) debug level
 *===========================================================================*/
static ssize_t mcast_debug_proc_write(struct file *file, const char *buffer,
				size_t length, loff_t *offset)
{
	int ret = 0;
	char *wbuf = NULL;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	wbuf = kzalloc(length, GFP_KERNEL);
	if (!wbuf) {
		printk(KERN_ERR "MCH: Cannot allocate memory!\n");
		return -EFAULT;
	}

	if (copy_from_user(wbuf, buffer, length)) {
		printk(KERN_ERR "MCH: Cannot copy buffer from user space!\n");
		kfree(wbuf);
		return -EFAULT;
	}
	wbuf[length - 1] = '\0';

	if ((wbuf[0] == '0') || (wbuf[0] == '1')) {
		mcast_debug = wbuf[0] - '0';
		printk(KERN_INFO "MCH: Debug level is now %sabled\n",
		       (mcast_debug) ? "en" : "dis");
	} else {
		printk(KERN_ERR "MCH: invalid value - 1/0 (enable/disable)\n");
	}

	ret = length;

	kfree(wbuf);
	*offset += ret;
	return ret;
}

/*=============================================================================
 *function name: helper_proc_open
 *description  : function to open helper proc for ipv4 table entries
 *===========================================================================*/
int helper_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, mcast_helper_seq_show, NULL);
}

/*=============================================================================
 *function name: helper_proc_open6
 *description  : function to open helper proc for ipv6 table entries
 *===========================================================================*/
int helper_proc_open6(struct inode *inode, struct file *file)
{
	return single_open(file, mcast_helper_seq_show6, NULL);
}

/*=============================================================================
 *function name: mcast_debug_proc_open
 *description  : function to open debug proc for debug prints
 *===========================================================================*/
static int mcast_debug_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, mcast_debug_seq_show, NULL);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static const struct proc_ops mcast_helper_seq_fops = {
	.proc_open	= helper_proc_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
};

static const struct proc_ops mcast_helper_seq_fops6 = {
	.proc_open	= helper_proc_open6,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
};

static const struct proc_ops mcast_debug_fops = {
	.proc_open	= mcast_debug_proc_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
	.proc_write	= mcast_debug_proc_write,
};
#else
static const struct file_operations mcast_helper_seq_fops = {
	.owner		= THIS_MODULE,
	.open		= helper_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static const struct file_operations mcast_helper_seq_fops6 = {
	.owner		= THIS_MODULE,
	.open		= helper_proc_open6,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static const struct file_operations mcast_debug_fops = {
	.owner		= THIS_MODULE,
	.open		= mcast_debug_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= mcast_debug_proc_write,
};
#endif

/*=============================================================================
 *function name: mcast_helper_net_init
 *description  : function to create mcast helper proc entry
 *===========================================================================*/
static int mcast_helper_net_init(void)
{
	struct proc_dir_entry *pdh = NULL, *pdh6 = NULL;
	struct proc_dir_entry *pdebug = NULL;

	pdh = proc_create("mcast_helper", 0, NULL, &mcast_helper_seq_fops);
	if (!pdh) {
		goto out_mcast;
	}
	pdh6 = proc_create("mcast_helper6", 0, NULL, &mcast_helper_seq_fops6);
	if (!pdh6)
		goto out_mcast;

	pdebug = proc_create("mcast_debug", 0, NULL, &mcast_debug_fops);
	if (!pdebug)
		goto out_mcast;

	return 0;

out_mcast:
	if (pdh)
		remove_proc_entry("mcast_helper", NULL);
	if (pdh6)
		remove_proc_entry("mcast_helper6", NULL);

	return -ENOMEM;
}

/*=============================================================================
 *function name: mcast_helper_net_exit
 *description  : function to remove mcast helper proc entry
 *===========================================================================*/

#ifdef CONFIG_SYSCTL
#ifdef CONFIG_MCAST_HELPER_ACL
/**Functions to create a proc for ACL enable/disable support **/
static int
mcast_helper_acl_sysctl_call_tables(struct ctl_table *ctl, int write,
				    void __user *buffer, size_t *lenp, loff_t *ppos)
{
	int ret;

	ret = proc_dointvec(ctl, write, buffer, lenp, ppos);

	if (write && *(int *)(ctl->data))
		*(int *)(ctl->data) = 1;

	return ret;
}

static struct ctl_table mcast_helper_acl_table[] = {
	{
		.procname	= "multicast-acl",
		.data		= &mch_acl_enabled,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= mcast_helper_acl_sysctl_call_tables,
	},
	{ }

};
#endif /* CONFIG_MCAST_HELPER_ACL */

/*=============================================================================
 *function name: mcast_helper_accl_sysctl_call_tables
 *description  : Functions to create a proc to enable/disable multicast
 *               acceleration for WLAN
 *===========================================================================*/
static int
mcast_helper_accl_sysctl_call_tables(struct ctl_table *ctl, int write,
				     void __user *buffer, size_t *lenp,
				     loff_t *ppos)
{
	int ret;

	ret = proc_dointvec(ctl, write, buffer, lenp, ppos);

	if (write && *(int *)(ctl->data))
		*(int *)(ctl->data) = 1;

	return ret;
}

static struct ctl_table mcast_helper_accl_table[] = {
	{
		.procname	= "multicast-acceleration",
		.data		= &mch_accl_enabled,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= mcast_helper_accl_sysctl_call_tables,
	},
	{ }

};
#endif /* CONFIG_SYSCTL */

int __init mcast_helper_proc_init(void)
{
	int ret;

#ifdef CONFIG_SYSCTL
#ifdef CONFIG_MCAST_HELPER_ACL
	mcast_acl_sysctl_header = register_net_sysctl(&init_net, "net/",
						      mcast_helper_acl_table);
	if (!mcast_acl_sysctl_header) {
		printk(KERN_WARNING "Failed to register mcast acl sysctl table.\n");
		return 0;
	}
#endif

	/* Proc to disable multicast acceleration for WLAN */
	mcast_accl_sysctl_header = register_net_sysctl(&init_net, "net/",
						       mcast_helper_accl_table);
	if (!mcast_accl_sysctl_header) {
		printk(KERN_WARNING "Failed to register mcast accl sysctl table.\n");
		return 0;
	}

#endif

	ret = mcast_helper_net_init();

	return ret;
}
#endif /* CONFIG_PROC_FS */
#endif /* CONFIG_BRIDGE_IGMP_SNOOPING */

/*=============================================================================
 *function name: mcast_helper_init_module
 *description  : Multicast helper module initialization
 *===========================================================================*/
static int __init mcast_helper_init_module(void)
{
#if IS_ENABLED(CONFIG_BRIDGE_IGMP_SNOOPING)
	int ret_val;

	/* Reserve BIT(0) which is used for broadcast */
	set_bit(0, g_mcgid_bitmap);

	/*
	 * Allocate the chrdev region for mcast helper
	 */
	ret_val = alloc_chrdev_region(&mch_major, 0, 1, DEVICE_NAME);

	/*
	 * Negative values signify an error
	 */
	if (ret_val < 0) {
		printk(KERN_ALERT "%s failed with %d\n",
		       "Sorry, alloc_chrdev_region failed for the mcast device",
		       ret_val);
		return ret_val;
	}

	printk(KERN_INFO "%s The major device number is %d.\n",
	       "Registration is a success", MAJOR(mch_major));

	/* Create device class (before allocation of the array of devices) */
	mcast_class = class_create(THIS_MODULE, DEVICE_NAME);
	if (IS_ERR(mcast_class)) {
		ret_val = PTR_ERR(mcast_class);
		goto fail;
	}
	if (!device_create(mcast_class, NULL, mch_major, NULL, "mcast")) {
		printk(KERN_ERR "MCH: failed to create device!\n");
		goto fail;
	}
	device_created = 1;
	cdev_init(&mcast_cdev, &mcast_helper_fops);
	if (cdev_add(&mcast_cdev, mch_major, 1) == -1)
		goto fail;

	register_netdevice_notifier(&mcast_helper_netdevice_notifier);
	/* Initialize all timers */
	setup_timer(&mch_helper_timer, mcast_helper_timer_handler, 0);
#ifdef CONFIG_PROC_FS
	mcast_helper_proc_init();
#endif
	mcast_helper_br_learning_hook = mcast_helper_bridge_learning;
#ifdef CONFIG_MCAST_HELPER_ACL
	mcast_helper_sig_check_update_ptr = mcast_helper_sig_check_update;
	mch_acl_enabled = 1;
#endif

	return 0;

fail:
	if (device_created) {
		device_destroy(mcast_class, mch_major);
		cdev_del(&mcast_cdev);
	}
	if (mcast_class)
		class_destroy(mcast_class);
	if (mch_major != -1)
		unregister_chrdev_region(mch_major, 1);

	return -1;
#else
	return 0;
#endif
}

 /*=============================================================================
 * function name : mcast_helper_exit_module
 * description   : Mcast helper module exit handler
 *===========================================================================*/

static void __exit mcast_helper_exit_module(void)
{
#if IS_ENABLED(CONFIG_BRIDGE_IGMP_SNOOPING)
	/* Cancel all timers */
	del_timer(&mch_helper_timer);

	mch_acl_enabled = 0;
	mcast_helper_br_learning_hook = NULL;
#ifdef CONFIG_MCAST_HELPER_ACL
	mcast_helper_sig_check_update_ptr = NULL;

	if (skb_buff)
		kfree_skb(skb_buff);
	if (skb_buff6)
		kfree_skb(skb_buff6);
#endif

	unregister_netdevice_notifier(&mcast_helper_netdevice_notifier);
#ifdef CONFIG_PROC_FS
#ifdef CONFIG_SYSCTL
#ifdef CONFIG_MCAST_HELPER_ACL
	unregister_net_sysctl_table(mcast_acl_sysctl_header);
#endif
	unregister_net_sysctl_table(mcast_accl_sysctl_header);
#endif
	remove_proc_entry("mcast_helper", NULL);
	remove_proc_entry("mcast_helper6", NULL);
	remove_proc_entry("mcast_debug", NULL);
#endif

	if (device_created) {
		device_destroy(mcast_class, mch_major);
		cdev_del(&mcast_cdev);
	}
	if (mcast_class)
		class_destroy(mcast_class);
	if (mch_major != -1)
		unregister_chrdev_region(mch_major, 1);
#endif
}

module_init(mcast_helper_init_module);
module_exit(mcast_helper_exit_module);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Multicast helper");
MODULE_AUTHOR("Srikanth");
MODULE_AUTHOR("Ujjal Roy");
module_param(mcast_debug, int, 0);
MODULE_PARM_DESC(mcast_debug, " MCH debug level - 0: disable, 1: enable");
