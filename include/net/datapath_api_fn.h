// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation.
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

#ifndef DATAPATH_API_FN_H
#define DATAPATH_API_FN_H

#include <linux/version.h>
#include <linux/netdevice.h>
struct xfrm_state;

extern int (*dp_rx_enable_fn)(struct net_device *netif, char *ifname, u32 flags);
extern int (*dp_shaper_conf_set_fn)(struct dp_shaper_conf *cfg, int flag);
extern int (*dp_shaper_conf_get_fn)(struct dp_shaper_conf *cfg, int flag);
extern int (*dp_node_link_add_fn)(struct dp_node_link *info, int flag);
extern int (*dp_queue_map_set_fn)(struct dp_queue_map_set *cfg, int flag);
extern int (*dp_queue_map_get_fn)(struct dp_queue_map_get *cfg, int flag);
extern int (*dp_vlan_set_fn)(struct dp_tc_vlan *vlan, int flags);
extern int (*dp_qos_port_conf_set_fn)(struct dp_port_cfg_info *info, int flag);
extern int (*dp_qos_link_prio_set_fn)(struct dp_node_prio *info, int flag);
extern int (*dp_qos_link_prio_get_fn)(struct dp_node_prio *info, int flag);
extern int (*dp_queue_conf_get_fn)(struct dp_queue_conf *cfg, int flag);
extern int (*dp_queue_conf_set_fn)(struct dp_queue_conf *cfg, int flag);
extern int (*dp_ingress_ctp_tc_map_set_fn)(struct dp_tc_cfg *tc, int flag);
extern int (*dp_deq_port_res_get_fn)(struct dp_dequeue_res *res, int flag);
extern int (*dp_node_free_fn)(struct dp_node_alloc *node, int flag);
extern int (*dp_node_alloc_fn)(struct dp_node_alloc *node, int flag);
extern int (*dp_node_unlink_fn)(struct dp_node_link *info, int flag);
extern int (*dp_meter_alloc_fn)(int inst, int *meterid, int flag);
extern int (*dp_meter_add_fn)(struct net_device *dev, struct dp_meter_cfg *meter,
			      int flag);
extern int (*dp_meter_del_fn)(struct net_device *dev, struct dp_meter_cfg *meter,
			      int flag);
extern int (*dp_set_bp_attr_fn)(struct dp_bp_attr *conf, u32 flag);
extern void (*dp_net_dev_get_ss_stat_strings_fn)(struct net_device *dev, u8 *data);
extern int (*dp_net_dev_get_ss_stat_strings_count_fn)(struct net_device *dev);
extern int (*dp_get_mtu_size_fn)(struct net_device *dev, u32 *mtu_size);
extern int32_t (*dp_alloc_port_fn)(struct module *owner, struct net_device *dev,
				   u32 dev_port, int32_t port_id,
				   dp_pmac_cfg_t *pmac_cfg, u32 flags);
extern int32_t (*dp_alloc_port_ext_fn)(int inst, struct module *owner,
				       struct net_device *dev,
				       u32 dev_port, int32_t port_id,
				       dp_pmac_cfg_t *pmac_cfg,
				       struct dp_port_data *data, u32 flags);
extern int32_t (*dp_register_dev_fn)(struct module *owner, u32 port_id,
				     dp_cb_t *dp_cb, u32 flags);
extern int32_t (*dp_register_dev_ext_fn)(int inst, struct module *owner, u32 port_id,
					 dp_cb_t *dp_cb, 
					 struct dp_dev_data *data,
					 u32 flags);
extern int32_t (*dp_register_subif_fn)(struct module *owner, struct net_device *dev,
				       char *subif_name, dp_subif_t *subif_id,
				       u32 flags);
extern int32_t (*dp_register_subif_ext_fn)(int inst, struct module *owner,
					   struct net_device *dev,
					   char *subif_name,
					   dp_subif_t *subif_id,
					   struct dp_subif_data *data,
					   u32 flags);
extern int32_t (*dp_register_subif_spl_dev_fn)(int inst,
					       struct net_device *dev,
					       char *subif_name,
					       dp_subif_t *subif_id,
					       struct dp_subif_data *data,
					       u32 flags);
extern int (*dp_pce_rule_add_fn)(struct dp_pce_blk_info *pce_blk_info,
				 GSW_PCE_rule_t *pce);
extern int (*dp_pce_rule_del_fn)(struct dp_pce_blk_info *pce_blk_info,
				 GSW_PCE_ruleDelete_t *pce);
extern int (*dp_pce_rule_update_fn)(struct dp_pce_blk_info *pce_blk_info,
				    GSW_PCE_rule_t *pce,
				    enum DP_PCE_RULE_UPDATE update);
extern int (*dp_pce_rmon_get_fn)(int *id);
extern int (*dp_pce_rmon_free_fn)(int id);
extern void (*dp_get_dev_ss_stat_strings_fn)(struct net_device *dev, u8 *data);
extern int (*dp_get_dev_stat_strings_count_fn)(struct net_device *dev);
extern int (*dp_register_event_cb_fn)(struct dp_event *info, u32 flag);
extern int32_t (*dp_xmit_fn)(struct net_device *rx_if, dp_subif_t *rx_subif,
			     struct sk_buff *skb, int32_t len, u32 flags);
extern int32_t (*dp_get_netif_subifid_fn)(struct net_device *netif, struct sk_buff *skb,
					  void *subif_data, 
					  u8 dst_mac[DP_MAX_ETH_ALEN],
					  dp_subif_t *subif, u32 flags);
extern bool (*dp_is_pmapper_check_fn)(struct net_device *dev);
extern u32 (*dp_get_tx_cbm_pkt_fn)(int inst, int port_id, int subif_id_grp);
extern int (*dp_register_ops_fn)(int inst, enum DP_OPS_TYPE type, void *ops);
extern int32_t (*dp_rx_fn)(struct sk_buff *skb, u32 flags);
extern int (*dp_qos_get_q_mib_fn)(struct dp_qos_queue_info *info, int flag);
extern int (*dp_split_buffer_fn)(char *buffer, char *array[], int max_param_num);
extern void (*dp_replace_ch_fn)(char *p, int len, char orig_ch, char new_ch);
extern int (*dp_atoi_fn)(unsigned char *str);
extern int (*dp_strncmpi_fn)(const char *s1, const char *s2, size_t n);
extern int (*dp_get_reinsert_cnt_fn)(int inst, int dp_port, int vap, int flag,
				     struct dp_reinsert_count *dp_reins_count);
extern int (*dp_get_lct_cnt_fn)(int inst, int dp_port, int flag,
		   		u32 *lct_idx,
				struct dp_lct_rx_cnt *dp_lct_rx_count);
extern bool (*dp_is_ready_fn)(void);
extern int (*dp_get_port_prop_fn)(int inst, int port_id, struct dp_port_prop *prop);
extern int (*dp_get_subif_prop_fn)(int inst, int port_id, int vap, struct dp_subif_prop *prop);
extern void *(*dp_get_ops_fn)(int inst, enum DP_OPS_TYPE type);
extern int (*dp_set_mtu_size_fn)(struct net_device *dev, u32 mtu_size);
extern int (*dp_spl_conn_fn)(int inst, struct dp_spl_cfg *conn);
extern int (*dp_spl_conn_get_fn)(int inst, enum DP_SPL_TYPE type,
				 struct dp_spl_cfg *conns, u8 cnt);
extern int (*dp_qos_get_q_logic_fn)(struct dp_qos_q_logic *cfg, int flag);
extern int (*dp_register_tx_fn)(enum DP_TX_PRIORITY priority, tx_fn fn, void *priv);
extern int (*dp_register_rx_hook_fn)(int priority, dp_rx_hook_fn_t fn, void *priv);
extern int (*dp_deregister_rx_hook_fn)(int priority, dp_rx_hook_fn_t fn);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
extern int32_t (*qos_mgr_hook_setup_tc)(struct net_device *dev, u32 handle,
					__be16 protocol,
					struct tc_to_netdev *tc);
#else
extern int32_t (*qos_mgr_hook_setup_tc)(struct net_device *dev,
					enum tc_setup_type type,
					void *type_data);
#endif
extern int (*dp_hostif_update_fn)(int inst, int dpid, int vap, struct dp_hif_datapath *new_dp);
extern int (*dp_qos_codel_cfg_set_fn)(struct dp_qos_codel_cfg *cfg, int flag);
extern int (*dp_qos_codel_cfg_get_fn)(struct dp_qos_codel_cfg *cfg, int flag);

/* tc callback */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
extern int dp_ndo_setup_tc_fn(struct net_device *dev, u32 handle,
			      __be16 protocol, struct tc_to_netdev *tc);
#else
extern int (*dp_ndo_setup_tc_fn)(struct net_device *dev,
	    enum tc_setup_type type, void *type_data);
#endif
/* tc dev capability update */
extern int (*dp_dev_update_tc_fn)(struct net_device *dev);

/* swtichdev callback */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
extern int (*dp_bp_ndo_br_setlink_fn)(struct net_device *dev, struct nlmsghdr *nlh,
				      u16 flags);
#else
extern int (*dp_bp_ndo_br_setlink_fn)(struct net_device *dev,
	    struct nlmsghdr *nlh, u16 flags, struct netlink_ext_ack *extack);
#endif
extern int (*dp_bp_ndo_br_dellink_fn)(struct net_device *dev,
	    struct nlmsghdr *nlh, u16 flags);
extern int (*dp_ndo_get_port_parent_id_fn)(struct net_device *dev,
	    struct netdev_phys_item_id *ppid);
extern int (*dp_ndo_get_phys_port_id_fn)(struct net_device *dev,
	    struct netdev_phys_item_id *ppid);
extern int (*dp_ndo_get_phys_port_name_fn)(struct net_device *dev,
	    char *name, size_t len);

/* toe dev capability update */
extern int (*dp_dev_update_toe_fn)(struct net_device *dev);

/* xfrm callback */
extern int (*dp_xdo_dev_state_add_fn)(struct xfrm_state *x);
extern void (*dp_xdo_dev_state_delete_fn)(struct xfrm_state *x);
extern bool (*dp_xdo_dev_offload_ok_fn)(struct sk_buff *skb,
	     struct xfrm_state *x);
/* xfrm dev capability update */
extern int (*dp_dev_update_xfrm_fn)(struct net_device *dev);
/* ptp */
extern int dp_get_ts_info_fn(struct net_device *dev,
			  struct ethtool_ts_info *ts_info);
extern int dp_ndo_do_ioctl_fn(struct net_device *dev, struct ifreq *ifr, int cmd);
extern int dp_get_netif_stats_fn(struct net_device *dev, dp_subif_t *subif_id,
				 struct rtnl_link_stats64 *path_stats, uint32_t flags);

int dp_late_register_ops(void);
int dp_late_register_event_cb(void);

#endif

