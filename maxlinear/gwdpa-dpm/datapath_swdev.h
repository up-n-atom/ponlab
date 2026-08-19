// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_SWDEV_H
#define DATAPATH_SWDEV_H

#include <linux/if.h>
#include <linux/etherdevice.h>
#include <net/datapath_api.h>
#include <net/switchdev.h>
#include <net/switch_api/lantiq_gsw_api.h> /*Switch related structures */
#include <net/switch_api/lantiq_gsw.h>
#include <linux/version.h>

#define BRIDGE_ID_ENTRY_HASH_LENGTH 6
#define BR_ID_ENTRY_HASH_TABLE_SIZE BIT(BRIDGE_ID_ENTRY_HASH_LENGTH)

#define ADD_BRENTRY		BIT(0)
#define DEL_BRENTRY		BIT(1)
#define BRIDGE_NO_ACTION	BIT(2)
#define LOGIC_DEV_REGISTER	BIT(3)
#define CPU_PORT_DISABLE	BIT(4)

/* This flag DP_SWDEV_DUMMY_DEL_IF is used to simulate switchdev call
 * (SWITCHDEV_ATTR_ID_PORT_STP_STATE with dp_swdev_port_attr_set)
 * from dp_deregister_subif or netowrk notifier in case this dev still in
 * bridge during de_register
 */
#define DP_SWDEV_DUMMY_DEL_IF 1
#define DP_DEF_MAX_VLAN_LIMIT 8

#define DP_BR_NOCHANGE 0  /* no change or don't know */
#define DP_BR_JOIN 1  /* brctl addif */
#define DP_BR_LEAVE 2 /* brctl delif */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
struct switchdev_trans_item {
	struct list_head list;
	void *data;
	void (*destructor)(const void *data);
};
#else
struct switchdev_brport_flags {
	unsigned long val;
	unsigned long mask;
};
#endif  /* LINUX_VERSION_CODE */

struct bridge_id_entry_item {
	struct net_device *dev;
	s16 fid;
	u32 flags;
	s16 bportid; /*Bridge port*/
	s16 dp_port; /*DP port*/
	s16 inst;
	u32 priv_flag; /* to store bp dev priv flags (like IFF_NO_QUEUE) */
	struct switchdev_trans_item tritem;
};

struct br_info {
	struct hlist_node br_hlist;
	struct net_device *dev; /* bridge dev */
	s16 fid;
	s16 last_fid; /* save last fid when fid is freed */
	u32 flag;
	s16 inst;
	s16 dp_port;
	struct list_head bp_list; /* point to struct bridge_member_port */
	bool br_vlan_en; 	/* VLAN Enabled for this bridge, valid only on PRX */
	u32 max_vlan_limit;	/* Max Vlan allowed for this bridge */
	u32 max_brid_limit;	/* Max Brid allowed for this bridge */
	int num_vlan;		/* Num of vlans supported in br */
	int num_fid;		/* Num of vlan aware fid in br */
	int num_dev; 		/* Num of device under this bridge including
				 * those not register to dpm yet
				 */
	struct list_head br_vlan_list;  /* List pointing to the vlan entry */
	const struct net_device_ops *old_dev_ops;
	struct net_device_ops new_devops;
	int netdev_ops_num; /* number of ndo_xx hacked in net_device_ops */
	int all_ops_cnt; /* all ndo_xx hacked in all kinds of ops */

	bool f_mac_add; /* indicate mac address is added to GSWIP or not */
	u8 br_mac[ETH_ALEN]; /* bridge mac address added to GSWIP */
};

/* member dev info */
struct bridge_member_port {
	struct list_head list;
	s16 bportid; /* bridge port */
	struct net_device *dev; /* bridge port dev for debug */
	u32 dev_reg_flag;
	u32 dev_priv_flag; /* to store bp dev priv flags (like IFF_NO_QUEUE) */
	s16 dp_port;
	bool isolate;
	bool hairpin;
	struct list_head bport_vlan_list; /* List pointing to the vlan entry */
	/* added for linux 5.x for fast access */
	struct br_info *br_info;
	struct dp_dev *dp_dev;
};

struct vlist_entry {
	struct list_head list;
	struct vlan_entry *vlan_entry;  /* Pointer to the VLAN Aware Entry */
};

struct vlan_entry {
	u16 fid;	/* New FiD used for this bridge port */
	u16 vlan_id;	/* Vlan ID supported in this bridge port */
	u32 ref_cnt;	/* Reference count for this VLAN */
	struct list_head pce_list; /* List pointing to PCE */
};

struct pce_entry {
	struct list_head list;
	struct dp_pce_blk_info *blk_info;
	u16 idx; /* pce index */
	bool disable; /* PCE rule index enabled or disabled */
};

struct fdb_tbl {
	struct list_head fdb_list;
	struct net_device *port_dev;
	u8 addr[ETH_ALEN];
	__be16 vid;
};

extern struct list_head fdb_tbl_list;
extern struct hlist_head g_bridge_id_entry_hash_table[BR_ID_ENTRY_HASH_TABLE_SIZE];
int dp_swdev_bridge_id_entry_init(void);
int dp_swdev_bridge_id_entry_de_init(void);
struct br_info *dp_swdev_bridge_entry_lookup(struct net_device *dev);
struct br_info *dp_br_in_free_lookup(struct net_device *dev);
int dp_swdev_chk_bport_in_br(struct net_device *bp_dev, int bport,
			     int inst);
int dp_swdev_get_dp_port_from_bp(struct br_info *br_item, int bport,
				 u32 *priv_flag);
int dp_register_bport_vlan(struct br_info *br_info,
			   struct net_device *dev, int bport, int flags);
int dp_register_br_vlan_ops(struct br_info *br_info, int flags);
struct pce_entry *get_pce_entry_from_ventry(struct vlan_entry *ventry,
		int portid, int subifid);
void dp_switchdev_exit(void);
int dp_switchdev_init(void);
int dp_del_br_if(struct net_device *dev, struct net_device *br_dev,
		 int inst, int bport);
int dp_register_switchdev_ops(struct net_device *dev, int reset);

int dp_notif_br_alloc(struct net_device *br_dev);

int _dp_free_fid(struct dp_dev *dp_dev, struct br_info *br_info);
int _dp_alloc_fid_misc(struct dp_dev *dp_dev, struct br_info *br_info);
int _dp_port_attr_br_flags_get(
	struct net_device *dev,
	struct switchdev_brport_flags *flags);

int _dp_port_attr_br_flags_set(
	struct net_device *dev,
	unsigned long flags);
int _dp_swdev_port_attr_learning_limit_set(
	struct net_device *dev,
	int learning_limit);

int _dp_swdev_bridge_attr_mcast_flood(
	struct net_device *bridge,
	enum br_mcast_flood_mode mcast_flood);
int dp_swdev_stp_forward(struct dp_dev *dp_dev);
int dp_swdev_stp_disable(struct dp_dev *dp_dev);
void dump_bp_flag(struct switchdev_brport_flags flags);
char *get_stp_stat_str(int stat);
char *get_bport_flags(unsigned long flags);

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
int dp_ndo_bridge_setlink(struct net_device *dev, struct nlmsghdr *nlh,
			  u16 flags);
int dp_bp_ndo_br_setlink2(struct net_device *dev, struct nlmsghdr *nlh,
			  u16 flags);
#else
int dp_ndo_bridge_setlink(struct net_device *dev, struct nlmsghdr *nlh,
				    u16 flags, struct netlink_ext_ack *extack);
int dp_bp_ndo_br_setlink2(struct net_device *dev, struct nlmsghdr *nlh,
				   u16 flags, struct netlink_ext_ack *extack);
#endif
int dp_ndo_bridge_dellink2(struct net_device *dev, struct nlmsghdr *nlh,
				    u16 flags);
int dp_bp_ndo_br_dellink2(struct net_device *dev, struct nlmsghdr *nlh,
				   u16 flags);
int dp_add_br(struct net_device *br_dev);
int dp_del_br(struct net_device *br_dev);

#endif /*DATAPATH_SWDEV_H*/
