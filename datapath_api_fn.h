/*******************************************************************************
 * Copyright (c) 2021 - 2024, MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_API_FN_H_
#define DATAPATH_API_FN_H_

#include <linux/version.h>
#include <linux/netdevice.h>

int32_t dp_xmit2(struct net_device *rx_if, dp_subif_t *rx_subif,
		 struct sk_buff *skb, int32_t len, u32 flags);
void dp_replace_ch2(char *p, int len, char orig_ch, char new_ch);
int dp_rx_enable2(struct net_device *netif, char *ifname, u32 flags);
int dp_shaper_conf_get2(struct dp_shaper_conf *cfg, int flag);
int dp_node_link_add2(struct dp_node_link *info, int flag);
int dp_queue_map_set2(struct dp_queue_map_set *cfg, int flag);
int dp_queue_map_get2(struct dp_queue_map_get *cfg, int flag);
int dp_vlan_set2(struct dp_tc_vlan *vlan, int flags);
int dp_qos_port_conf_set2(struct dp_port_cfg_info *info, int flag);
int dp_shaper_conf_set2(struct dp_shaper_conf *cfg, int flag);
int dp_qos_link_prio_set2(struct dp_node_prio *info, int flag);
int dp_qos_link_prio_get2(struct dp_node_prio *info, int flag);
int dp_get_reinsert_cnt2(int inst, int dp_port, int vap, int flag,
			struct dp_reinsert_count *dp_reins_count);
int dp_get_lct_cnt2(int inst, int dp_port, int flag,
		    u32 *lct_idx, struct dp_lct_rx_cnt *dp_lct_rx_count);
int dp_strncmpi2(const char *s1, const char *s2, size_t n);
int dp_atoi2(unsigned char *str);
int dp_split_buffer2(char *buffer, char *array[], int max_param_num);

int dp_qos_get_q_mib2(struct dp_qos_queue_info *info, int flag);
int32_t dp_rx2(struct sk_buff *skb, u32 flags);
int dp_register_ops2(int inst, enum DP_OPS_TYPE type, void *ops);
int32_t dp_get_netif_subifid2(struct net_device *netif, struct sk_buff *skb,
			     void *subif_data, u8 dst_mac[DP_MAX_ETH_ALEN],
			     dp_subif_t *subif, u32 flags);
u32 dp_get_tx_cbm_pkt2(int inst, int port_id, int subif_id_grp);
bool dp_is_pmapper_check2(struct net_device *dev);
u32 dp_get_tx_cbm_pkt2(int inst, int port_id, int subif_id_grp);
int dp_register_event_cb2(struct dp_event *info, u32 flag);
int dp_pce_rule_del2(struct dp_pce_blk_info *pce_blk_info,
		    GSW_PCE_ruleDelete_t *pce);
int dp_pce_rule_add2(struct dp_pce_blk_info *pce_blk_info,
		    GSW_PCE_rule_t *pce);
int dp_pce_rule_update2(struct dp_pce_blk_info *pce_blk_info,
			GSW_PCE_rule_t *pce,
			enum DP_PCE_RULE_UPDATE update);
int dp_pce_rmon_get2(int *id);
int dp_pce_rmon_free2(int id);
int32_t dp_register_subif2(struct module *owner, struct net_device *dev,
			  char *subif_name, dp_subif_t *subif_id,
			  u32 flags);
int32_t dp_register_dev_ext2(int inst, struct module *owner, u32 port_id,
			    dp_cb_t *dp_cb, struct dp_dev_data *data,
			    u32 flags);
int32_t dp_register_dev2(struct module *owner, u32 port_id,
			dp_cb_t *dp_cb, u32 flags);

int32_t dp_alloc_port2(struct module *owner, struct net_device *dev,
		      u32 dev_port, int32_t port_id,
		      dp_pmac_cfg_t *pmac_cfg, u32 flags);
int dp_get_mtu_size2(struct net_device *dev, u32 *mtu_size);
int dp_set_mtu_size2(struct net_device *dev, u32 mtu_size);
int32_t dp_register_subif_ext2(int inst, struct module *owner,
			      struct net_device *dev,
			      char *subif_name, dp_subif_t *subif_id,
			      /*device related info*/
			      struct dp_subif_data *data, u32 flags);
int32_t dp_alloc_port_ext2(int inst, struct module *owner,
			  struct net_device *dev,
			  u32 dev_port, int32_t port_id,
			  dp_pmac_cfg_t *pmac_cfg,
			  struct dp_port_data *data, u32 flags);
int32_t dp_register_subif_spl_dev2(int inst, struct net_device *dev,
				   char *subif_name, dp_subif_t *subif_id,
				   struct dp_subif_data *data, u32 flags);
int dp_set_bp_attr2(struct dp_bp_attr *conf, u32 flag);
int dp_meter_del2(struct net_device *dev, struct dp_meter_cfg *meter, int flag);
int dp_meter_alloc2(int inst, int *meterid, int flag);
int dp_meter_add2(struct net_device *dev, struct dp_meter_cfg *meter, int flag);
int dp_node_unlink2(struct dp_node_link *info, int flag);
int dp_node_alloc2(struct dp_node_alloc *node, int flag);
int dp_node_free2(struct dp_node_alloc *node, int flag);
int dp_deq_port_res_get2(struct dp_dequeue_res *res, int flag);
int dp_ingress_ctp_tc_map_set2(struct dp_tc_cfg *tc, int flag);
int dp_queue_conf_set2(struct dp_queue_conf *cfg, int flag);
int dp_queue_conf_get2(struct dp_queue_conf *cfg, int flag);
int dp_get_port_prop2(int inst, int port_id, struct dp_port_prop *prop);
int dp_get_subif_prop2(int inst, int port_id, int vap, struct dp_subif_prop *prop);
int dp_register_tx2(enum DP_TX_PRIORITY priority, tx_fn fn, void *p);
int dp_register_rx_hook2(int priority, dp_rx_hook_fn_t fn, void *p);
int dp_deregister_rx_hook2(int priority, dp_rx_hook_fn_t fn);
int dp_spl_conn2(int inst, struct dp_spl_cfg *conn);
int dp_spl_conn_get2(int inst, enum DP_SPL_TYPE type,
		     struct dp_spl_cfg *conns, u8 cnt);
int dp_qos_get_q_logic2(struct dp_qos_q_logic *cfg, int flag);
void *dp_get_ops2(int inst, enum DP_OPS_TYPE type);
bool dp_is_ready2(void);

void proc_fn_read(struct seq_file *s);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
int dp_update_hostif2(int inst, int dpid, int vap, struct dp_hif_datapath *new_dp);
#endif
int dp_qos_codel_cfg_set2(struct dp_qos_codel_cfg *cfg, int flag);
int dp_qos_codel_cfg_get2(struct dp_qos_codel_cfg *cfg, int flag);

/* tc callback */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
int dp_ndo_setup_tc2(struct net_device *dev, u32 handle,
		     __be16 protocol, struct tc_to_netdev *tc);
#else
int dp_ndo_setup_tc2(struct net_device *dev, enum tc_setup_type type, 
		     void *type_data);
#endif
/* tc dev capability update */
int dp_dev_update_tc2(struct net_device *dev);

/* swtichdev callback */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
int dp_bp_ndo_br_setlink2(struct net_device *dev, struct nlmsghdr *nlh,
			  u16 flags);
int dp_swdev_port_obj_add2(struct net_device *dev,
		const struct switchdev_obj *obj,
		struct switchdev_trans *trans);
int dp_swdev_port_obj_del2(struct net_device *dev,
			const struct switchdev_obj *obj);
int dp_swdev_port_attr_get2(struct net_device *dev,
			struct switchdev_attr *attr);
int dp_swdev_port_attr_set2(struct net_device *dev,
		struct switchdev_attr *attr,
		struct switchdev_trans *trans);
#else
int dp_bp_ndo_br_setlink2(struct net_device *dev, struct nlmsghdr *nlh,
			 u16 flags, struct netlink_ext_ack *extack);
int dp_ndo_get_port_parent_id2(struct net_device *dev,
			      struct netdev_phys_item_id *ppid);
int dp_ndo_get_phys_port_id2(struct net_device *dev,
			    struct netdev_phys_item_id *ppid);
int dp_ndo_get_phys_port_name2(struct net_device *dev,
			      char *name, size_t len);

#endif
int dp_bp_ndo_br_dellink2(struct net_device *dev, struct nlmsghdr *nlh,
			 u16 flags);

/* toe dev capability update */
int dp_dev_update_toe2(struct net_device *dev);

/* xfrm callback */
struct xfrm_state;
int dp_xdo_dev_state_add2(struct xfrm_state *x);
void dp_xdo_dev_state_delete2(struct xfrm_state *x);
bool dp_xdo_dev_offload_ok2(struct sk_buff *skb, struct xfrm_state *x);
void dp_xdo_dev_state_advance_esn2(struct xfrm_state *x);

/* xfrm dev capability update */
int dp_dev_update_xfrm2(struct net_device *dev);

/* ptp */
struct ifreq;
int dp_get_ts_info2(struct net_device *dev,
		   struct ethtool_ts_info *ts_info);
int dp_ndo_do_ioctl2(struct net_device *dev, struct ifreq *ifr, int cmd);
int dp_get_netif_stats2(struct net_device *dev, dp_subif_t *subif_id,
			struct rtnl_link_stats64 *stats, u32 flags);

int dp_qos_get_q_global_parms2(int inst, int dp_port, int alloc_flag,
			       u32 qos_id, struct dp_qos_q_parms *parms);

#endif
