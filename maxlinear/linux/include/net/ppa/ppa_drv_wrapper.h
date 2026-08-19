/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PPA_DRV_WRAPPER__
#define __PPA_DRV_WRAPPER__
/******************************************************************************
**
** FILE NAME    : ppa_drv_wrapper.h
** PROJECT      : PPA
** MODULES      : PPA Wrapper for PPE Driver API
**
** DATE         : 14 Mar 2011
** AUTHOR       : Shao Guohua
** DESCRIPTION  : PPA Wrapper for PPE Driver API
** Copyright (c) 2020 - 2025 MaxLinear, Inc.
** COPYRIGHT    : Copyright (c) 2017 Intel Corporation
** Copyright (c) 2011 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
** HISTORY
** $Date        $Author         $Comment
** 14 MAR 2011  Shao Guohua       Initiate Version
** 05 JUL 2017  Kamal eradath    merged ppe_drv_wrapper.h and ppa_datapath_wrapper.h
*******************************************************************************/
extern uint32_t ppa_drv_dp_sb_addr_to_fpi_addr_convert(PPA_FPI_ADDR *a, uint32_t flag);
extern uint32_t ppa_drv_get_number_of_phys_port(PPA_COUNT_CFG *count, uint32_t flag);
extern uint32_t ppa_drv_get_phys_port_info(PPE_IFINFO *info, uint32_t flag);
extern uint32_t ppa_drv_get_max_entries(PPA_MAX_ENTRY_INFO *entry, uint32_t flag);
extern uint32_t ppa_drv_set_route_cfg(PPA_ROUTING_CFG *cfg, uint32_t flag);
extern uint32_t ppa_drv_set_bridging_cfg(PPA_BRDG_CFG *cfg, uint32_t flag);
extern uint32_t ppa_drv_get_acc_mode(PPA_ACC_ENABLE *acc_enable, uint32_t flag);
extern uint32_t ppa_drv_set_acc_mode(PPA_ACC_ENABLE *acc_enable, uint32_t flag);
extern uint32_t ppa_drv_set_bridge_if_vlan_config(PPA_BRDG_VLAN_CFG *cfg, uint32_t flag);
extern uint32_t ppa_drv_get_bridge_if_vlan_config(PPA_BRDG_VLAN_CFG *cfg, uint32_t flag);
extern uint32_t ppa_drv_add_vlan_map(PPA_BRDG_VLAN_FILTER_MAP *filter, uint32_t flag);
extern uint32_t ppa_drv_del_vlan_map(PPA_BRDG_VLAN_FILTER_MAP *filter, uint32_t flag);
extern uint32_t ppa_drv_get_vlan_map(PPA_BRDG_VLAN_FILTER_MAP *filter , uint32_t flag);
extern uint32_t ppa_drv_del_all_vlan_map(uint32_t flag);
extern uint32_t ppa_drv_get_max_vfilter_entries(PPA_VFILTER_COUNT_CFG *count, uint32_t flag);
extern uint32_t ppa_drv_is_ipv6_enabled(uint32_t flag);
#if defined(MIB_MODE_ENABLE) && MIB_MODE_ENABLE
extern uint32_t ppa_drv_set_mib_mode(PPA_MIB_MODE_ENABLE *cfg, uint32_t);
extern uint32_t ppa_drv_get_mib_mode(PPA_MIB_MODE_ENABLE *cfg);
#endif
extern uint32_t ppa_drv_add_bridging_entry(PPA_BR_MAC_INFO *entry, uint32_t flag);
extern uint32_t ppa_drv_del_bridging_entry(PPA_BR_MAC_INFO *entry, uint32_t flag);
extern uint32_t ppa_drv_get_ports_mib(PPA_PORT_MIB *mib, uint32_t flag);
extern uint32_t ppa_drv_get_itf_mib(PPE_ITF_MIB_INFO *mib, uint32_t flag);
extern uint32_t ppa_drv_test_and_clear_bridging_hit_stat(PPA_BR_MAC_INFO *entry, uint32_t flag);
extern uint32_t ppa_set_wan_itf(PPA_WANITF_CFG *cfg, uint32_t flag);
extern uint32_t ppa_drv_hal_init(PPA_HAL_INIT_CFG *cfg, uint32_t flag);
extern uint32_t ppa_drv_hal_exit(uint32_t flag);
extern uint32_t ppa_get_session_hash(PPA_SESSION_HASH *cfg, uint32_t flag);
#if defined(MBR_CONFIG) && MBR_CONFIG
extern uint32_t ppa_drv_set_qos_shaper(PPA_QOS_RATE_SHAPING_CFG *cfg , uint32_t flag);
extern uint32_t ppa_drv_get_qos_shaper(PPA_QOS_RATE_SHAPING_CFG *cfg , uint32_t flag);
#endif

extern int ppa_drv_get_dslwan_qid_with_vcc(struct atm_vcc *vcc);
extern int ppa_drv_get_netif_qid_with_pkt(struct sk_buff *skb, void *arg, int is_atm_vcc);
extern int ppa_drv_ppe_clk_change(unsigned int arg, unsigned int flags);
extern int ppa_drv_ppe_pwm_change(unsigned int arg, unsigned int flags);   /*  arg - parameter, flags - 1: clock gating on/off, 2: power gating on/off*/
struct ppa_debugfs_files {
	const char *name;
	umode_t perm;
	const struct file_operations *fops;
	struct dentry *file;
};
extern struct proc_dir_entry *g_ppa_proc_dir;
struct dentry *ppa_debugfs_dir_get(void);
struct dentry *ppa_hal_debugfs_dir_get(void);
int32_t ppa_debugfs_create(struct dentry *root, const char *dname,
	struct dentry **parent, struct ppa_debugfs_files *files, uint32_t count);
void ppa_debugfs_remove(struct dentry *parent,
	struct ppa_debugfs_files *files, uint32_t count);

/* netdev attributes for qos support */
struct netdev_attr {
	int32_t portid;		/* egress port */
	uint32_t dir;		/* qos direction ingress or egress */
	uint32_t tc;		/* skb traffic class or priority */
	uint32_t mark;		/* skb mark or extmark */
	uint32_t flags;		/* reserved */
	int32_t dst_q_low;	/* low prio queue */
	int32_t dst_q_high;	/* high prio queue */
	PPA_SKBUF *skb;		/* associated skb */
};
extern int32_t (*ppa_api_get_mapped_queue)(PPA_NETIF *netdev,
		struct netdev_attr *attr);
#endif /*end of __PPA_DRV_WRAPPER__ */
