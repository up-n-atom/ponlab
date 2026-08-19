// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 * Copyright (c) 2021 - 2024, MaxLinear, Inc.
 *
 ******************************************************************************/

#include <net/datapath_api.h>
#include <linux/version.h>

int (*dp_rx_enable_fn)(struct net_device *netif, char *ifname, u32 flags) = NULL;
EXPORT_SYMBOL(dp_rx_enable_fn);

int (*dp_shaper_conf_set_fn)(struct dp_shaper_conf *cfg, int flag) = NULL;
EXPORT_SYMBOL(dp_shaper_conf_set_fn);

int (*dp_shaper_conf_get_fn)(struct dp_shaper_conf *cfg, int flag) = NULL;
EXPORT_SYMBOL(dp_shaper_conf_get_fn);

int (*dp_node_link_add_fn)(struct dp_node_link *info, int flag) = NULL;
EXPORT_SYMBOL(dp_node_link_add_fn);

int (*dp_queue_map_set_fn)(struct dp_queue_map_set *cfg, int flag) = NULL;
EXPORT_SYMBOL(dp_queue_map_set_fn);

int (*dp_queue_map_get_fn)(struct dp_queue_map_get *cfg, int flag) = NULL;
EXPORT_SYMBOL(dp_queue_map_get_fn);

int (*dp_vlan_set_fn)(struct dp_tc_vlan *vlan, int flags) = NULL;
EXPORT_SYMBOL(dp_vlan_set_fn);

int (*dp_qos_port_conf_set_fn)(struct dp_port_cfg_info *info, int flag) = NULL;
EXPORT_SYMBOL(dp_qos_port_conf_set_fn);

int (*dp_qos_link_prio_set_fn)(struct dp_node_prio *info, int flag) = NULL;
EXPORT_SYMBOL(dp_qos_link_prio_set_fn);

int (*dp_qos_link_prio_get_fn)(struct dp_node_prio *info, int flag) = NULL;
EXPORT_SYMBOL(dp_qos_link_prio_get_fn);

int (*dp_queue_conf_get_fn)(struct dp_queue_conf *cfg, int flag) = NULL;
EXPORT_SYMBOL(dp_queue_conf_get_fn);

int (*dp_queue_conf_set_fn)(struct dp_queue_conf *cfg, int flag) = NULL;
EXPORT_SYMBOL(dp_queue_conf_set_fn);

int (*dp_ingress_ctp_tc_map_set_fn)(struct dp_tc_cfg *tc, int flag) = NULL;
EXPORT_SYMBOL(dp_ingress_ctp_tc_map_set_fn);

int (*dp_deq_port_res_get_fn)(struct dp_dequeue_res *res, int flag) = NULL;
EXPORT_SYMBOL(dp_deq_port_res_get_fn);

int (*dp_node_free_fn)(struct dp_node_alloc *node, int flag) = NULL;
EXPORT_SYMBOL(dp_node_free_fn);

int (*dp_node_alloc_fn)(struct dp_node_alloc *node, int flag) = NULL;
EXPORT_SYMBOL(dp_node_alloc_fn);

int (*dp_node_unlink_fn)(struct dp_node_link *info, int flag) = NULL;
EXPORT_SYMBOL(dp_node_unlink_fn);

int (*dp_meter_alloc_fn)(int inst, int *meterid, int flag) = NULL;
EXPORT_SYMBOL(dp_meter_alloc_fn);

int (*dp_meter_add_fn)(struct net_device *dev, struct dp_meter_cfg *meter,
		       int flag) = NULL;
EXPORT_SYMBOL(dp_meter_add_fn);

int (*dp_meter_del_fn)(struct net_device *dev, struct dp_meter_cfg *meter,
		       int flag) = NULL;
EXPORT_SYMBOL(dp_meter_del_fn);

int (*dp_set_bp_attr_fn)(struct dp_bp_attr *conf, u32 flag) = NULL;
EXPORT_SYMBOL(dp_set_bp_attr_fn);

int (*dp_get_mtu_size_fn)(struct net_device *dev, u32 *mtu_size) = NULL;
EXPORT_SYMBOL(dp_get_mtu_size_fn);

int32_t (*dp_alloc_port_fn)(struct module *owner, struct net_device *dev,
			 u32 dev_port, int32_t port_id,
			 dp_pmac_cfg_t *pmac_cfg, u32 flags) = NULL;
EXPORT_SYMBOL(dp_alloc_port_fn);

int32_t (*dp_alloc_port_ext_fn)(int inst, struct module *owner,
				struct net_device *dev,
				u32 dev_port, int32_t port_id,
				dp_pmac_cfg_t *pmac_cfg,
				struct dp_port_data *data, u32 flags) = NULL;
EXPORT_SYMBOL(dp_alloc_port_ext_fn);

int32_t (*dp_register_dev_fn)(struct module *owner, u32 port_id,
			      dp_cb_t *dp_cb, u32 flags) = NULL;
EXPORT_SYMBOL(dp_register_dev_fn);

int32_t (*dp_register_dev_ext_fn)(int inst, struct module *owner, u32 port_id,
			       dp_cb_t *dp_cb, struct dp_dev_data *data,
			       u32 flags) = NULL;
EXPORT_SYMBOL(dp_register_dev_ext_fn);

int32_t (*dp_register_subif_fn)(struct module *owner, struct net_device *dev,
				char *subif_name, dp_subif_t *subif_id,
				u32 flags) = NULL;
EXPORT_SYMBOL(dp_register_subif_fn);

int32_t (*dp_register_subif_ext_fn)(int inst, struct module *owner,
				    struct net_device *dev,
				    char *subif_name, dp_subif_t *subif_id,
				struct dp_subif_data *data, u32 flags) = NULL;
EXPORT_SYMBOL(dp_register_subif_ext_fn);

int32_t (*dp_register_subif_spl_dev_fn)(int inst, struct net_device *dev,
					char *subif_name, dp_subif_t *subif_id,
					struct dp_subif_data *data,
					u32 flags) = NULL;
EXPORT_SYMBOL(dp_register_subif_spl_dev_fn);

int (*dp_pce_rule_add_fn)(struct dp_pce_blk_info *pce_blk_info,
			  GSW_PCE_rule_t *pce) = NULL;
EXPORT_SYMBOL(dp_pce_rule_add_fn);

int (*dp_pce_rule_del_fn)(struct dp_pce_blk_info *pce_blk_info,
			  GSW_PCE_ruleDelete_t *pce) = NULL;
EXPORT_SYMBOL(dp_pce_rule_del_fn);

int (*dp_pce_rule_update_fn)(struct dp_pce_blk_info *pce_blk_info,
			     GSW_PCE_rule_t *pce,
			     enum DP_PCE_RULE_UPDATE update) = NULL;
EXPORT_SYMBOL(dp_pce_rule_update_fn);

int (*dp_pce_rmon_get_fn)(int *id) = NULL;
EXPORT_SYMBOL(dp_pce_rmon_get_fn);

int (*dp_pce_rmon_free_fn)(int id) = NULL;
EXPORT_SYMBOL(dp_pce_rmon_free_fn);

int (*dp_register_event_cb_fn)(struct dp_event *info, u32 flag);
EXPORT_SYMBOL(dp_register_event_cb_fn);

int32_t (*dp_xmit_fn)(struct net_device *rx_if, dp_subif_t *rx_subif,
		struct sk_buff *skb, int32_t len, u32 flags) = NULL;
EXPORT_SYMBOL(dp_xmit_fn);

int32_t (*dp_get_netif_subifid_fn)(struct net_device *netif, struct sk_buff *skb,
				   void *subif_data, u8 dst_mac[DP_MAX_ETH_ALEN],
				   dp_subif_t *subif, u32 flags) = NULL;
EXPORT_SYMBOL(dp_get_netif_subifid_fn);

bool (*dp_is_pmapper_check_fn)(struct net_device *dev) = NULL;
EXPORT_SYMBOL(dp_is_pmapper_check_fn);

u32 (*dp_get_tx_cbm_pkt_fn)(int inst, int port_id, int subif_id_grp) = NULL;
EXPORT_SYMBOL(dp_get_tx_cbm_pkt_fn);

int (*dp_register_ops_fn)(int inst, enum DP_OPS_TYPE type, void *ops) = NULL;
EXPORT_SYMBOL(dp_register_ops_fn);

int32_t (*dp_rx_fn)(struct sk_buff *skb, u32 flags) = NULL;
EXPORT_SYMBOL(dp_rx_fn);

int (*dp_qos_get_q_mib_fn)(struct dp_qos_queue_info *info, int flag) = NULL;
EXPORT_SYMBOL(dp_qos_get_q_mib_fn);

int (*dp_split_buffer_fn)(char *buffer, char *array[], int max_param_num) = NULL;
EXPORT_SYMBOL(dp_split_buffer_fn);

void (*dp_replace_ch_fn)(char *p, int len, char orig_ch, char new_ch) = NULL;
EXPORT_SYMBOL(dp_replace_ch_fn);

int (*dp_atoi_fn)(unsigned char *str) = NULL;
EXPORT_SYMBOL(dp_atoi_fn);

int (*dp_strncmpi_fn)(const char *s1, const char *s2, size_t n) = NULL;
EXPORT_SYMBOL(dp_strncmpi_fn);

int (*dp_get_reinsert_cnt_fn)(int inst, int dp_port, int vap, int flag,
			      struct dp_reinsert_count *dp_reins_count) = NULL;
EXPORT_SYMBOL(dp_get_reinsert_cnt_fn);

int (*dp_get_lct_cnt_fn)(int inst, int dp_port, int flag,
                      u32 *lct_idx, struct dp_lct_rx_cnt *dp_lct_rx_count);
EXPORT_SYMBOL(dp_get_lct_cnt_fn);

bool (*dp_is_ready_fn)(void) = NULL;
EXPORT_SYMBOL(dp_is_ready_fn);

int (*dp_get_port_prop_fn)(int inst, int port_id, struct dp_port_prop *prop) = NULL;
EXPORT_SYMBOL(dp_get_port_prop_fn);

int (*dp_get_subif_prop_fn)(int inst, int port_id, int vap, struct dp_subif_prop *prop) = NULL;
EXPORT_SYMBOL(dp_get_subif_prop_fn);

void * (*dp_get_ops_fn)(int inst, enum DP_OPS_TYPE type) = NULL;
EXPORT_SYMBOL(dp_get_ops_fn);

int (*dp_set_mtu_size_fn)(struct net_device *dev, u32 mtu_size) = NULL;
EXPORT_SYMBOL(dp_set_mtu_size_fn);

int (*dp_spl_conn_fn)(int inst, struct dp_spl_cfg *conn) = NULL;
EXPORT_SYMBOL(dp_spl_conn_fn);

int (*dp_spl_conn_get_fn)(int inst, enum DP_SPL_TYPE type,
			  struct dp_spl_cfg *conns, u8 cnt);
EXPORT_SYMBOL(dp_spl_conn_get_fn);

int (*dp_qos_get_q_logic_fn)(struct dp_qos_q_logic *cfg, int flag);
EXPORT_SYMBOL(dp_qos_get_q_logic_fn);

int (*dp_register_tx_fn)(enum DP_TX_PRIORITY priority, tx_fn fn, void *priv) = NULL;
EXPORT_SYMBOL(dp_register_tx_fn);

int (*dp_register_rx_hook_fn)(int priority, dp_rx_hook_fn_t fn, void *priv) = NULL;
EXPORT_SYMBOL(dp_register_rx_hook_fn);

int (*dp_deregister_rx_hook_fn)(int priority, dp_rx_hook_fn_t fn) = NULL;
EXPORT_SYMBOL(dp_deregister_rx_hook_fn);

/* tc callback */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
int (*dp_ndo_setup_tc_fn)(struct net_device *dev, u32 handle,
			  __be16 protocol, struct tc_to_netdev *tc) = NULL;
#else
int (*dp_ndo_setup_tc_fn)(struct net_device *dev, enum tc_setup_type type, 
     void *type_data) = NULL;
#endif     
EXPORT_SYMBOL(dp_ndo_setup_tc_fn);

int (*dp_dev_update_tc_fn)(struct net_device *dev) = NULL;
EXPORT_SYMBOL(dp_dev_update_tc_fn);

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
int (*dp_bp_ndo_br_setlink_fn)(struct net_device *dev, struct nlmsghdr *nlh,
			    u16 flags) = NULL;
#else
int (*dp_bp_ndo_br_setlink_fn)(struct net_device *dev, struct nlmsghdr *nlh,
     u16 flags, struct netlink_ext_ack *extack) = NULL;
#endif
EXPORT_SYMBOL(dp_bp_ndo_br_setlink_fn);

int (*dp_bp_ndo_br_dellink_fn)(struct net_device *dev, struct nlmsghdr *nlh,
     u16 flags) = NULL;
EXPORT_SYMBOL(dp_bp_ndo_br_dellink_fn);

int (*dp_ndo_get_port_parent_id_fn)(struct net_device *dev,
     struct netdev_phys_item_id *ppid) = NULL;
EXPORT_SYMBOL(dp_ndo_get_port_parent_id_fn);

int (*dp_ndo_get_phys_port_id_fn)(struct net_device *dev,
     struct netdev_phys_item_id *ppid) = NULL;
EXPORT_SYMBOL(dp_ndo_get_phys_port_id_fn);

int (*dp_ndo_get_phys_port_name_fn)(struct net_device *dev,
     char *name, size_t len) = NULL;
EXPORT_SYMBOL(dp_ndo_get_phys_port_name_fn);

int (*dp_dev_update_toe_fn)(struct net_device *dev) = NULL;
EXPORT_SYMBOL(dp_dev_update_toe_fn);

int (*dp_xdo_dev_state_add_fn)(struct xfrm_state *x) = NULL;
EXPORT_SYMBOL(dp_xdo_dev_state_add_fn);

void (*dp_xdo_dev_state_delete_fn)(struct xfrm_state *x) = NULL;
EXPORT_SYMBOL(dp_xdo_dev_state_delete_fn);

bool (*dp_xdo_dev_offload_ok_fn)(struct sk_buff *skb,
      struct xfrm_state *x) = NULL;
EXPORT_SYMBOL(dp_xdo_dev_offload_ok_fn);

int (*dp_dev_update_xfrm_fn)(struct net_device *dev) = NULL;
EXPORT_SYMBOL(dp_dev_update_xfrm_fn);

int (*dp_get_ts_info_fn)(struct net_device *dev,
     struct ethtool_ts_info *ts_info) =NULL;
EXPORT_SYMBOL(dp_get_ts_info_fn);

int (*dp_ndo_do_ioctl_fn)(struct net_device *dev, struct ifreq *ifr, int cmd);
EXPORT_SYMBOL(dp_ndo_do_ioctl_fn);

int (*dp_get_netif_stats_fn)(struct net_device *dev, dp_subif_t *subif_id,
			     struct rtnl_link_stats64 *path_stats, uint32_t flags);
EXPORT_SYMBOL(dp_get_netif_stats_fn);

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
int (*qos_tc_setup_fn)(struct net_device *dev,
		       u32 handle,
		       __be16 protocol,
		       struct tc_to_netdev *tc,
		       int port_id,
		       int deq_idx) = NULL;
#else
int (*qos_tc_setup_fn)(struct net_device *dev,
		       enum tc_setup_type type,
		       void *type_data,
		       int port_id,
		       int deq_idx) = NULL;
#endif
EXPORT_SYMBOL(qos_tc_setup_fn);

int (*dp_hostif_update_fn)(int inst, int dpid, int vap, struct dp_hif_datapath *new_dp) = NULL;
EXPORT_SYMBOL(dp_hostif_update_fn);

int (*dp_qos_codel_cfg_set_fn)(struct dp_qos_codel_cfg *cfg, int flag) = NULL;
EXPORT_SYMBOL(dp_qos_codel_cfg_set_fn);

int (*dp_qos_codel_cfg_get_fn)(struct dp_qos_codel_cfg *cfg, int flag) = NULL;
EXPORT_SYMBOL(dp_qos_codel_cfg_get_fn);

#define MAX_EVENTS 10
static int num_event;
struct event_late_init_list {
	struct dp_event info;
	u32 flag; /*callback 2nd parameter */
};
struct event_late_init_list late_event_list[MAX_EVENTS];
int dp_register_event_cb(struct dp_event *info, u32 flag)
{
	if (!dp_is_ready()) {
		if (flag & DP_F_DEREGISTER) {
			pr_err("Not support de-register before DPM ready\n");
			return DP_FAILURE;
		}
		if (!info) {
			pr_err("info NULL:%s\n", __func__);
			return DP_FAILURE;
		}

		/* workaround in case some module build-in kernel but DPM in ko */
		if (num_event >= MAX_EVENTS) {
			pr_err("dpm event > capability of %d\n", MAX_EVENTS);
			return DP_FAILURE;
		}
		late_event_list[num_event].info = *info;
		late_event_list[num_event].flag = flag;
		num_event++;
		/* return one dummy id since DPM not ready yet */
		*(unsigned long *)&info->id = (unsigned long)num_event;
		return DP_SUCCESS;
	} else if (dp_register_event_cb_fn) {
		if (flag & DP_F_DEREGISTER) { /*retrieve real info->id */
			unsigned long id = (unsigned long) info->id;
			if (id == 0) {
				pr_err("wrong zero ID:de-register_event_cb\n");
				return DP_FAILURE;
			} 
			if ((id > 0) && (id < MAX_EVENTS)) {
				info->id = late_event_list[id - 1].info.id;
				late_event_list[id - 1].info.id = NULL;
			}
		}
		return dp_register_event_cb_fn(info, flag);
	}
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_register_event_cb);


int32_t dp_xmit(struct net_device *rx_if, dp_subif_t *rx_subif,
		struct sk_buff *skb, int32_t len, u32 flags)
{
	if (dp_xmit_fn)
		return dp_xmit_fn(rx_if, rx_subif, skb, len, flags);
	dev_kfree_skb_any(skb);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_xmit);

int32_t dp_get_netif_subifid(struct net_device *netif, struct sk_buff *skb,
			     void *subif_data, u8 dst_mac[DP_MAX_ETH_ALEN],
			     dp_subif_t *subif, u32 flags)
{
	if (dp_get_netif_subifid_fn)
		return dp_get_netif_subifid_fn(netif, skb, subif_data, dst_mac,
					       subif, flags);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_get_netif_subifid);

bool dp_is_pmapper_check(struct net_device *dev)
{
	if (dp_is_pmapper_check_fn)
		return dp_is_pmapper_check_fn(dev);
	return false;
}
EXPORT_SYMBOL(dp_is_pmapper_check);

int dp_get_port_prop(int inst, int port_id, struct dp_port_prop *prop)
{
	if (dp_get_port_prop_fn)
		return dp_get_port_prop_fn(inst, port_id, prop);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_get_port_prop);

int dp_get_subif_prop(int inst, int port_id, int vap,
		      struct dp_subif_prop *prop)
{
	if (dp_get_subif_prop_fn)
		return dp_get_subif_prop_fn(inst, port_id, vap, prop);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_get_subif_prop);

u32 dp_get_tx_cbm_pkt(int inst, int port_id, int subif_id_grp)
{
	if (dp_get_tx_cbm_pkt_fn)
		return dp_get_tx_cbm_pkt_fn(inst, port_id, subif_id_grp);
	return 0;
}
EXPORT_SYMBOL(dp_get_tx_cbm_pkt);

int32_t dp_rx(struct sk_buff *skb, u32 flags)
{
	if (dp_rx_fn)
		return dp_rx_fn(skb, flags);
	dev_kfree_skb_any(skb);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_rx);

#define DP_MAX_OPS  10
struct ops_late_init_list {
	int inst;
	enum DP_OPS_TYPE type;
	void *ops;
};
static struct ops_late_init_list ops_late_init[DP_MAX_INST][DP_MAX_OPS];
static int ops_num[DP_MAX_INST];
int dp_register_ops(int inst, enum DP_OPS_TYPE type, void *ops)
{
	int idx;

	if (!dp_is_ready()) {
		idx = ops_num[inst];
		/* workaround in case some module build-in kernel but DPM in ko */
		if (idx >= DP_MAX_OPS) {
			pr_err("ops more than capability of %d\n", DP_MAX_OPS);
			return DP_FAILURE;
		}

		ops_late_init[inst][idx].ops = ops;
		ops_late_init[inst][idx].type = type;
		ops_num[inst]++;
		pr_debug("dp_register_ops num[%d]=%d\n", inst, ops_num[inst]);
		return DP_SUCCESS;
	} else if (dp_register_ops_fn)
		return dp_register_ops_fn(inst, type, ops);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_register_ops);

int dp_qos_get_q_mib(struct dp_qos_queue_info *info, int flag)
{
	if (dp_qos_get_q_mib_fn)
		return dp_qos_get_q_mib_fn(info, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_qos_get_q_mib);

int dp_split_buffer(char *buffer, char *array[], int max_param_num)
{
	if (dp_split_buffer_fn)
		return dp_split_buffer_fn(buffer, array, max_param_num);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_split_buffer);

void dp_replace_ch(char *p, int len, char orig_ch, char new_ch)
{
	if (dp_replace_ch_fn)
		return dp_replace_ch_fn(p, len, orig_ch, new_ch);
}
EXPORT_SYMBOL(dp_replace_ch);

int dp_atoi(unsigned char *str)
{
	if (dp_atoi_fn)
		return dp_atoi_fn(str);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_atoi);

int dp_strncmpi(const char *s1, const char *s2, size_t n)
{
	if (dp_strncmpi_fn)
		return dp_strncmpi_fn(s1, s2, n);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_strncmpi);

int dp_get_reinsert_cnt(int inst, int dp_port, int vap, int flag,
			struct dp_reinsert_count *dp_reins_count)
{
	if (dp_get_reinsert_cnt_fn)
		return dp_get_reinsert_cnt_fn(inst, dp_port, vap, flag,
					      dp_reins_count);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_get_reinsert_cnt);

int dp_get_lct_cnt(int inst, int dp_port, int flag,
                   u32 *lct_idx, struct dp_lct_rx_cnt *dp_lct_rx_count)
{
	if (dp_get_lct_cnt_fn)
		return dp_get_lct_cnt_fn(inst, dp_port, flag, lct_idx,
					      dp_lct_rx_count);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_get_lct_cnt);

int dp_get_mtu_size(struct net_device *dev, u32 *mtu_size)
{
	if (dp_get_mtu_size_fn)
		return dp_get_mtu_size_fn(dev, mtu_size);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_get_mtu_size);

int32_t dp_alloc_port(struct module *owner, struct net_device *dev,
		      u32 dev_port, int32_t port_id,
		      dp_pmac_cfg_t *pmac_cfg, u32 flags)
{
	if (dp_alloc_port_fn)
		return dp_alloc_port_fn(owner, dev, dev_port, port_id,
					pmac_cfg, flags);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_alloc_port);

int32_t dp_alloc_port_ext(int inst, struct module *owner,
			  struct net_device *dev,
			  u32 dev_port, int32_t port_id,
			  dp_pmac_cfg_t *pmac_cfg,
			  struct dp_port_data *data, u32 flags)
{
	if (dp_alloc_port_ext_fn)
		return dp_alloc_port_ext_fn(inst, owner, dev, dev_port, port_id,
					    pmac_cfg, data, flags);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_alloc_port_ext);

int32_t dp_register_dev(struct module *owner, u32 port_id,
				 dp_cb_t *dp_cb, u32 flags)
{
	if (dp_register_dev_fn)
		return dp_register_dev_fn(owner, port_id, dp_cb, flags);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_register_dev);

int32_t dp_register_dev_ext(int inst, struct module *owner, u32 port_id,
			    dp_cb_t *dp_cb, struct dp_dev_data *data,
			    u32 flags)
{
	if (dp_register_dev_ext_fn)
		return dp_register_dev_ext_fn(inst, owner, port_id, dp_cb, data,
					      flags);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_register_dev_ext);

int32_t dp_register_subif(struct module *owner, struct net_device *dev,
			  char *subif_name, dp_subif_t *subif_id,
			  u32 flags)
{
	if (dp_register_subif_fn)
		return dp_register_subif_fn(owner, dev, subif_name, subif_id,
					    flags);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_register_subif);

int32_t dp_register_subif_ext(int inst, struct module *owner,
			      struct net_device *dev,
			      char *subif_name, dp_subif_t *subif_id,
			      /*device related info*/
			      struct dp_subif_data *data, u32 flags)
{
	if (dp_register_subif_ext_fn)
		return dp_register_subif_ext_fn(inst, owner, dev, subif_name,
						subif_id, data, flags);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_register_subif_ext);

int32_t dp_register_subif_spl_dev(int inst, struct net_device *dev,
				  char *subif_name, dp_subif_t *subif_id,
				  /*device related info*/
				  struct dp_subif_data *data, u32 flags)
{
	if (dp_register_subif_spl_dev_fn)
		return dp_register_subif_spl_dev_fn(inst, dev,
						    subif_name, subif_id,
						    data, flags);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_register_subif_spl_dev);

int dp_set_bp_attr(struct dp_bp_attr *conf, u32 flag)
{
	if (dp_set_bp_attr_fn)
		return dp_set_bp_attr_fn(conf, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_set_bp_attr);

int dp_rx_enable(struct net_device *netif, char *ifname, u32 flags)
{
	if (dp_rx_enable_fn)
		return dp_rx_enable_fn(netif, ifname, flags);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_rx_enable);

int dp_shaper_conf_set(struct dp_shaper_conf *cfg, int flag)
{
	if (dp_shaper_conf_set_fn)
		return dp_shaper_conf_set_fn(cfg, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_shaper_conf_set);

int dp_shaper_conf_get(struct dp_shaper_conf *cfg, int flag)
{
	if (dp_shaper_conf_get_fn)
		return dp_shaper_conf_get_fn(cfg, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_shaper_conf_get);

int dp_node_link_add(struct dp_node_link *info, int flag)
{
	if (dp_node_link_add_fn)
		return dp_node_link_add_fn(info, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_node_link_add);

int dp_hostif_update(int inst, int dpid, int vap, struct dp_hif_datapath *new_dp)
{
	if (dp_hostif_update_fn)
		return dp_hostif_update_fn(inst, dpid, vap, new_dp);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_hostif_update);

int dp_qos_codel_cfg_set(struct dp_qos_codel_cfg *cfg, int flag)
{
	if (dp_qos_codel_cfg_set_fn)
		return dp_qos_codel_cfg_set_fn(cfg, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_qos_codel_cfg_set);

int dp_qos_codel_cfg_get(struct dp_qos_codel_cfg *cfg, int flag)
{
	if (dp_qos_codel_cfg_get_fn)
		return dp_qos_codel_cfg_get_fn(cfg, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_qos_codel_cfg_get);

int dp_queue_map_set(struct dp_queue_map_set *cfg, int flag)
{
	if (dp_queue_map_set_fn)
		return dp_queue_map_set_fn(cfg, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_queue_map_set);

int dp_queue_map_get(struct dp_queue_map_get *cfg, int flag)
{
	if (dp_queue_map_get_fn)
		return dp_queue_map_get_fn(cfg, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_queue_map_get);

int dp_vlan_set(struct dp_tc_vlan *vlan, int flags)
{
	if (dp_vlan_set_fn)
		return dp_vlan_set_fn(vlan, flags);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_vlan_set);

int dp_qos_port_conf_set(struct dp_port_cfg_info *info, int flag)
{
	if (dp_qos_port_conf_set_fn)
		return dp_qos_port_conf_set_fn(info, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_qos_port_conf_set);

int dp_qos_link_prio_set(struct dp_node_prio *info, int flag)
{
	if (dp_qos_link_prio_set_fn)
		return dp_qos_link_prio_set_fn(info, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_qos_link_prio_set);

int dp_queue_conf_get(struct dp_queue_conf *cfg, int flag)
{
	if (dp_queue_conf_get_fn)
		return dp_queue_conf_get_fn(cfg, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_queue_conf_get);

int dp_ingress_ctp_tc_map_set(struct dp_tc_cfg *tc, int flag)
{
	if (dp_ingress_ctp_tc_map_set_fn)
		return dp_ingress_ctp_tc_map_set_fn(tc, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_ingress_ctp_tc_map_set);

int dp_deq_port_res_get(struct dp_dequeue_res *res, int flag)
{
	if (dp_deq_port_res_get_fn)
		return dp_deq_port_res_get_fn(res, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_deq_port_res_get);

int dp_qos_link_prio_get(struct dp_node_prio *info, int flag)
{
	if (dp_qos_link_prio_get_fn)
		return dp_qos_link_prio_get_fn(info, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_qos_link_prio_get);

int dp_node_free(struct dp_node_alloc *node, int flag)
{
	if (dp_node_free_fn)
		return dp_node_free_fn(node, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_node_free);

int dp_node_alloc(struct dp_node_alloc *node, int flag)
{
	if (dp_node_alloc_fn)
		return dp_node_alloc_fn(node, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_node_alloc);

int dp_queue_conf_set(struct dp_queue_conf *cfg, int flag)
{
	if (dp_queue_conf_set_fn)
		return dp_queue_conf_set_fn(cfg, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_queue_conf_set);

int dp_node_unlink(struct dp_node_link *info, int flag)
{
	if (dp_node_unlink_fn)
		return dp_node_unlink_fn(info, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_node_unlink);

int dp_meter_alloc(int inst, int *meterid, int flag)
{
	if (dp_meter_alloc_fn)
		return dp_meter_alloc_fn(inst, meterid, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_meter_alloc);

int dp_meter_add(struct net_device *dev, struct dp_meter_cfg *meter, int flag)
{
	if (dp_meter_add_fn)
		return dp_meter_add_fn(dev, meter, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_meter_add);

int dp_meter_del(struct net_device *dev, struct dp_meter_cfg *meter, int flag)
{
	if (dp_meter_del_fn)
		return dp_meter_del_fn(dev, meter, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_meter_del);

int dp_pce_rule_add(struct dp_pce_blk_info *pce_blk_info,
			     GSW_PCE_rule_t *pce)

{
	if (dp_pce_rule_add_fn)
		return dp_pce_rule_add_fn(pce_blk_info, pce);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_pce_rule_add);

int dp_pce_rule_del(struct dp_pce_blk_info *pce_blk_info,
			     GSW_PCE_ruleDelete_t *pce)
{
	if (dp_pce_rule_del_fn)
		return dp_pce_rule_del_fn(pce_blk_info, pce);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_pce_rule_del);

int dp_pce_rule_update(struct dp_pce_blk_info *pce_blk_info,
		       GSW_PCE_rule_t *pce,
		       enum DP_PCE_RULE_UPDATE update)

{
	if (dp_pce_rule_update_fn)
		return dp_pce_rule_update_fn(pce_blk_info, pce, update);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_pce_rule_update);

int dp_pce_rmon_get(int *id)
{
	if (dp_pce_rmon_get_fn)
		return dp_pce_rmon_get_fn(id);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_pce_rmon_get);

int dp_pce_rmon_free(int id)
{
	if (dp_pce_rmon_free_fn)
		return dp_pce_rmon_free_fn(id);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_pce_rmon_free);

void *dp_get_ops(int inst, enum DP_OPS_TYPE type)
{
	if (dp_get_ops_fn)
		return dp_get_ops_fn(inst, type);
	return NULL;
}
EXPORT_SYMBOL(dp_get_ops);

int dp_set_mtu_size(struct net_device *dev, u32 mtu_size)
{
	if (dp_set_mtu_size_fn)
		return dp_set_mtu_size_fn(dev, mtu_size);
	return 1500;
}
EXPORT_SYMBOL(dp_set_mtu_size);

int dp_spl_conn(int inst, struct dp_spl_cfg *conn)
{
	if (dp_spl_conn_fn)
		return dp_spl_conn_fn(inst, conn);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_spl_conn);

int dp_spl_conn_get(int inst, enum DP_SPL_TYPE type,
			  struct dp_spl_cfg *conns, u8 cnt)
{
	if (dp_spl_conn_get_fn)
		return dp_spl_conn_get_fn(inst, type, conns, cnt);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_spl_conn_get);

int dp_qos_get_q_logic(struct dp_qos_q_logic *cfg, int flag)
{
	if (dp_qos_get_q_logic_fn)
		return dp_qos_get_q_logic_fn(cfg, flag);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_qos_get_q_logic);

#define MAX_TX 10
static int num_tx;
struct tx_late_init_list {
	enum DP_TX_PRIORITY priority;
	tx_fn fn;
	void *priv;
};
struct tx_late_init_list tx_late_init_list[MAX_TX];
int dp_register_tx(enum DP_TX_PRIORITY priority, tx_fn fn, void *priv)
{

	if (!dp_is_ready()) {
		/* workaround in case some module build-in kernel but DPM in ko */
		if (num_tx >= MAX_TX) {
			pr_err("tx cb more than capability of %d\n", MAX_TX);
			return DP_FAILURE;
		}

		tx_late_init_list[num_tx].priority = priority;
		tx_late_init_list[num_tx].fn = fn;
		tx_late_init_list[num_tx].priv = priv;
		num_tx++;
		return DP_SUCCESS;
	} else if (dp_register_tx_fn)
		return dp_register_tx_fn(priority, fn, priv);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_register_tx);

#define MAX_RX 10
static int num_rx;
struct rx_late_init_list {
	int priority;
	dp_rx_hook_fn_t fn;
	void *priv;
};
struct rx_late_init_list rx_late_init_list[MAX_RX];

int dp_register_rx_hook(int priority, dp_rx_hook_fn_t fn, void *priv)
{
	if (!dp_is_ready()) {
		/* workaround in case some module build-in kernel but DPM in ko */
		if (num_rx >= MAX_RX) {
			pr_err("rx cb more than capability of %d\n", MAX_RX);
			return DP_FAILURE;
		}

		rx_late_init_list[num_rx].priority = priority;
		rx_late_init_list[num_rx].fn = fn;
		rx_late_init_list[num_rx].priv = priv;
		num_rx++;
		return DP_SUCCESS;
	} else if (dp_register_rx_hook_fn)
		return dp_register_rx_hook_fn(priority, fn, priv);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_register_rx_hook);

int dp_deregister_rx_hook(int priority, dp_rx_hook_fn_t fn)
{
	if (!dp_is_ready()) {
		pr_err("%s %d, dpm is not ready yet!", __func__, __LINE__);
		return DP_FAILURE;
	} else if (dp_deregister_rx_hook_fn)
		return dp_deregister_rx_hook_fn(priority, fn);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dp_deregister_rx_hook);

/* This is workaround now: for example MIB counter moudle is build-in kernel
 * but DPM is in module
 */
bool dp_is_ready(void)
{
	if (dp_is_ready_fn)
		return dp_is_ready_fn();
	return false;
}
EXPORT_SYMBOL(dp_is_ready);

/* This API will be dpm internally triggered if someone call dp_register_ops
 * before DPM moudle ready
 */
int dp_late_register_ops(void)
{
	int i, j, n = 0;

	for (i = 0; i < DP_MAX_INST; i++) {
		for (j = 0; j < ops_num[i]; j++) {
			dp_register_ops_fn(i, ops_late_init[i][j].type,
					   ops_late_init[i][j].ops);
			n++;
		}
	}
	pr_debug("dp_late_register_ops: %d ops workaround done\n", n);
	return DP_SUCCESS;
}
EXPORT_SYMBOL(dp_late_register_ops);

/* This API will be internally triggered if someone call dp_register_event_cb 
 * before DPM module ready
 */
int dp_late_register_event_cb(void)
{
	int i;

	if (!dp_is_ready()) {
		return DP_FAILURE;
	}
	if (!dp_register_event_cb_fn) {
		return DP_FAILURE;
	}
	
	for (i = 0; i < num_event; i++)
		dp_register_event_cb_fn(&late_event_list[i].info,
					late_event_list[i].flag);
	pr_debug("dp_late_register_event_cb: %d event workaround done\n",
		num_event);

	return DP_SUCCESS;
}
EXPORT_SYMBOL(dp_late_register_event_cb);


/* This API will be internally triggered if someone call dp_register_event_cb 
 * before DPM module ready
 */
int dp_late_register_tx(void)
{
	int i;

	if (!dp_is_ready()) {
		return DP_FAILURE;
	}
	if (!dp_register_tx_fn) {
		return DP_FAILURE;
	}

	for (i = 0; i < num_tx; i++)
		dp_register_tx_fn(tx_late_init_list[i].priority,
					tx_late_init_list[i].fn,
					tx_late_init_list[i].priv);
	pr_debug("dp_late_register_tx: %d tx cb workaround done\n",
		num_tx);

	return DP_SUCCESS;
}
EXPORT_SYMBOL(dp_late_register_tx);

int dp_late_register_rx(void)
{
	int i;

	if (!dp_is_ready())
		return DP_FAILURE;

	if (!dp_register_rx_hook_fn)
		return DP_FAILURE;

	for (i = 0; i < num_rx; i++)
		dp_register_rx_hook_fn(rx_late_init_list[i].priority,
					rx_late_init_list[i].fn,
					rx_late_init_list[i].priv);

	return DP_SUCCESS;
}
EXPORT_SYMBOL(dp_late_register_rx);

/* tc callback */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
int dp_ndo_setup_tc(struct net_device *dev, u32 handle,
		    __be16 protocol, struct tc_to_netdev *tc)
{
	if (dp_ndo_setup_tc_fn)
		return dp_ndo_setup_tc_fn(dev, handle, protocol, tc);
	return -1;
}
#else		    
int dp_ndo_setup_tc(struct net_device *dev, enum tc_setup_type type,
		    void *type_data)
{
	if (dp_ndo_setup_tc_fn)
		return dp_ndo_setup_tc_fn(dev, type, type_data);
	return -1;
}
#endif
EXPORT_SYMBOL(dp_ndo_setup_tc);

/* tc dev capability update */
int dp_dev_update_tc(struct net_device *dev)
{
	if (dp_dev_update_tc_fn)
		return dp_dev_update_tc_fn(dev);
	return -1;
}
EXPORT_SYMBOL(dp_dev_update_tc);

/* swtichdev callback */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
int dp_bp_ndo_br_setlink(struct net_device *dev, struct nlmsghdr *nlh,
			 u16 flags)
{
	if (dp_bp_ndo_br_setlink_fn)
		return dp_bp_ndo_br_setlink_fn(dev, nlh, flags);
	return -1;
}
#else
int dp_bp_ndo_br_setlink(struct net_device *dev, struct nlmsghdr *nlh,
			 u16 flags, struct netlink_ext_ack *extack)
{
	if (dp_bp_ndo_br_setlink_fn)
		return dp_bp_ndo_br_setlink_fn(dev, nlh, flags, extack);
	return -1;
}
#endif
EXPORT_SYMBOL(dp_bp_ndo_br_setlink);

int dp_bp_ndo_br_dellink(struct net_device *dev, struct nlmsghdr *nlh,
			 u16 flags)
{
	if (dp_bp_ndo_br_dellink_fn)
		return dp_bp_ndo_br_dellink_fn(dev, nlh, flags);
	return -1;
}
EXPORT_SYMBOL(dp_bp_ndo_br_dellink);

int dp_ndo_get_port_parent_id(struct net_device *dev,
			      struct netdev_phys_item_id *ppid)
{
	if (dp_ndo_get_port_parent_id_fn)
		return dp_ndo_get_port_parent_id_fn(dev, ppid);
	return -1;
}
EXPORT_SYMBOL(dp_ndo_get_port_parent_id);

int dp_ndo_get_phys_port_id(struct net_device *dev,
			    struct netdev_phys_item_id *ppid)
{
	if (dp_ndo_get_phys_port_id_fn)
		return dp_ndo_get_phys_port_id_fn(dev, ppid);
	return -1;
}
EXPORT_SYMBOL(dp_ndo_get_phys_port_id);

int dp_ndo_get_phys_port_name(struct net_device *dev,
			      char *name, size_t len)
{
	if (dp_ndo_get_phys_port_name_fn)
		return dp_ndo_get_phys_port_name_fn(dev, name, len);
	return -1;
}
EXPORT_SYMBOL(dp_ndo_get_phys_port_name);

/* toe dev capability update */
int dp_dev_update_toe(struct net_device *dev)
{
	if (dp_dev_update_toe_fn)
		return dp_dev_update_toe_fn(dev);
	return -1;
}
EXPORT_SYMBOL(dp_dev_update_toe);

/* xfrm callback */
int dp_xdo_dev_state_add(struct xfrm_state *x)
{
	if (dp_xdo_dev_state_add_fn)
		return dp_xdo_dev_state_add_fn(x);
	return -1;
}
EXPORT_SYMBOL(dp_xdo_dev_state_add);

void dp_xdo_dev_state_delete(struct xfrm_state *x)
{
	if (dp_xdo_dev_state_delete_fn)
		dp_xdo_dev_state_delete_fn(x);
}
EXPORT_SYMBOL(dp_xdo_dev_state_delete);

bool dp_xdo_dev_offload_ok(struct sk_buff *skb, struct xfrm_state *x)
{
	if (dp_xdo_dev_offload_ok_fn)
		return dp_xdo_dev_offload_ok_fn(skb, x);
	return false;
}
EXPORT_SYMBOL(dp_xdo_dev_offload_ok);

/* xfrm dev capability update */
int dp_dev_update_xfrm(struct net_device *dev)
{
	if (dp_dev_update_xfrm_fn)
		return dp_dev_update_xfrm_fn(dev);
	return -1;
}
EXPORT_SYMBOL(dp_dev_update_xfrm);

/* ptp */
int dp_get_ts_info(struct net_device *dev,
		   struct ethtool_ts_info *ts_info)
{
	if (dp_get_ts_info_fn)
		return dp_get_ts_info_fn(dev, ts_info);
	return -1;
}
EXPORT_SYMBOL(dp_get_ts_info);

int dp_ndo_do_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	if (dp_ndo_do_ioctl_fn)
		return dp_ndo_do_ioctl_fn(dev, ifr, cmd);
	return -1;
}
EXPORT_SYMBOL(dp_ndo_do_ioctl);

int dp_get_netif_stats(struct net_device *dev, dp_subif_t *subif_id,
		       struct rtnl_link_stats64 *stats, u32 flags)
{
	if (dp_get_netif_stats_fn)
		return dp_get_netif_stats_fn(dev, subif_id, stats, flags);
	return -1;
}
EXPORT_SYMBOL(dp_get_netif_stats);

static char dp_dbg_cmdline[50];
static int __init dp_dbg_lvl_set(char *str)
{
	if (strlen(str) + 1 >= sizeof(dp_dbg_cmdline))
		pr_info("dp: had better increase dp_dbg_cmdline size to %ld\n",
			strlen(str) + 1);
	strlcpy(dp_dbg_cmdline, str, sizeof(dp_dbg_cmdline));
	return 0;
}
/* uboot pass dp_dbg= to linux built-in kernel module */
early_param("dp_dbg", dp_dbg_lvl_set);

char *dp_get_dbg_cmdline(void)
{
	return dp_dbg_cmdline;
}
EXPORT_SYMBOL(dp_get_dbg_cmdline);

static int __init dp_init(void)
{
	pr_info("dp manager dummy init done\n");
	return 0;
}

static void __exit dp_exit(void)
{
	pr_info("dpm dummy exit\n");
}

module_init(dp_init);
module_exit(dp_exit);

MODULE_LICENSE("GPL");
