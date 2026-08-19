// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2022 - 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_SWITCHDEV_H_
#define DATAPATH_SWITCHDEV_H_
#include <net/datapath_api.h>

#define DP_SWDEV_LOCK_T spinlock_t
#define DP_SWDEV_LOCK_INIT(lock) spin_lock_init(lock)
#define DP_SWDEV_DEFINE_LOCK(lock) DEFINE_SPINLOCK(lock)
#define DP_SWDEV_LOCK spin_lock_bh
#define DP_SWDEV_UNLOCK spin_unlock_bh

extern DP_SWDEV_LOCK_T dp_swdev_lock;

struct dp_swdev_data
{
	struct net_device *dev;
	dp_subif_t subif;
	/* later we can add more: like port_info, subif_info and so on */
};

int dp_gswip_stp_set(struct dp_swdev_data *dev_data,
			     int port, int control,
			     int value);
int dp_gswip_stp_get(struct dp_swdev_data *dev_data,
			     int port, int control);
int dp_gswip_add_ucast(struct dp_swdev_data *dev_data,
				const u8 *addr, int port,
				int flags, u16 vid);
int dp_gswip_del_ucast(struct dp_swdev_data *dev_data,
				const u8 *addr, int port,
				int flags, u16 vid);
int dp_gswip_add_mcast(struct dp_swdev_data *dev_data,
				const u8 *addr, int port_mask,
				int flags, u16 vid, int mcast_state);
int dp_gswip_del_mcast(struct dp_swdev_data *dev_data,
				const u8 *addr, int port_mask,
				int flags, u16 vid);
int dp_gswip_port_attr_bridge_flags_set(struct net_device *dev,
					unsigned long flags);
void dp_gswip_set_unreg_mcast(struct dp_swdev_data *dev_data,
					  int unreg_mcast_mask,
					  bool add);
int dp_gswip_vlan_add_modify(struct dp_swdev_data *dev_data,
					 u16 vid, int port_mask,
					 int untag_mask, int reg_mask,
					 int unreg_mask);
int dp_gswip_del_vlan(struct dp_swdev_data *dev_data,
			      u16 vid, int port_mask);
int get_swdev_port_id(struct dp_swdev_data *dev_data);
char *get_swdev_event_name(enum switchdev_notifier_type type);
char *get_swdev_obj_add_name(enum switchdev_obj_id id);
char *get_stp_stat_str(int stat);
char *get_bport_flags(unsigned long flags);
char *get_swdev_attr_name(enum switchdev_attr_id id);
int auto_register_vlan(dp_subif_t *subif, struct net_device *dev);
int auto_deregister_vlan(struct net_device *dev);
int dp_swdev_stp_forward(struct dp_dev *dp_dev);
int dp_swdev_stp_disable(struct dp_dev *dp_dev);
int dp_gswip_vlan_mac_learning_en(GSW_BRIDGE_portConfig_t *bpcfg, bool status);

#endif
