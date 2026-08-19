/******************************************************************************
 * Copyright (c) 2021 - 2024, MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <net/datapath_api.h>
#include <net/datapath_proc_api.h>
#include <net/datapath_api_fn.h>
#include "datapath.h"
#include "datapath_tx.h"
#include "datapath_api_fn.h"
#include "datapath_instance.h"
#include "datapath_swdev.h"

struct fn_list{
	char *name;
	void *f1;
	void *f2;
};

struct fn_list fn_list[] = {
	{"dp_rx_enable_fn", &dp_rx_enable_fn, &dp_rx_enable2},
	{"dp_shaper_conf_set_fn", &dp_shaper_conf_set_fn, &dp_shaper_conf_set2},
	{"dp_shaper_conf_get_fn", &dp_shaper_conf_get_fn, &dp_shaper_conf_get2},
	{"dp_node_link_add_fn", &dp_node_link_add_fn, &dp_node_link_add2},
	{"dp_queue_map_set_fn", &dp_queue_map_set_fn, &dp_queue_map_set2},
	{"dp_queue_map_get_fn", &dp_queue_map_get_fn, &dp_queue_map_get2},
	{"dp_vlan_set_fn", &dp_vlan_set_fn, &dp_vlan_set2},
	{"dp_qos_port_conf_set_fn", &dp_qos_port_conf_set_fn, &dp_qos_port_conf_set2},
	{"dp_qos_link_prio_set_fn", &dp_qos_link_prio_set_fn, &dp_qos_link_prio_set2},
	{"dp_qos_link_prio_get_fn", &dp_qos_link_prio_get_fn, &dp_qos_link_prio_get2},
	{"dp_queue_conf_get_fn", &dp_queue_conf_get_fn, &dp_queue_conf_get2},
	{"dp_queue_conf_set_fn", &dp_queue_conf_set_fn, &dp_queue_conf_set2},
	{"dp_ingress_ctp_tc_map_set_fn", &dp_ingress_ctp_tc_map_set_fn, &dp_ingress_ctp_tc_map_set2},
	{"dp_deq_port_res_get_fn", &dp_deq_port_res_get_fn, &dp_deq_port_res_get2},
	{"dp_node_free_fn", &dp_node_free_fn, &dp_node_free2},
	{"dp_node_alloc_fn", &dp_node_alloc_fn, &dp_node_alloc2},
	{"dp_node_unlink_fn", &dp_node_unlink_fn, &dp_node_unlink2},
	{"dp_meter_alloc_fn", &dp_meter_alloc_fn, &dp_meter_alloc2},
	{"dp_meter_add_fn", &dp_meter_add_fn, &dp_meter_add2},
	{"dp_meter_del_fn", &dp_meter_del_fn, &dp_meter_del2},
	{"dp_set_bp_attr_fn", &dp_set_bp_attr_fn, &dp_set_bp_attr2},
	{"dp_get_mtu_size_fn", &dp_get_mtu_size_fn, &dp_get_mtu_size2},
	{"dp_alloc_port_fn", &dp_alloc_port_fn, &dp_alloc_port2},
	{"dp_alloc_port_ext_fn", &dp_alloc_port_ext_fn, &dp_alloc_port_ext2},
	{"dp_register_dev_fn", &dp_register_dev_fn, &dp_register_dev2},
	{"dp_register_dev_ext_fn", &dp_register_dev_ext_fn, &dp_register_dev_ext2},
	{"dp_register_subif_fn", &dp_register_subif_fn, &dp_register_subif2},
	{"dp_register_subif_ext_fn", &dp_register_subif_ext_fn, &dp_register_subif_ext2},
	{"dp_register_subif_spl_dev_fn", &dp_register_subif_spl_dev_fn, &dp_register_subif_spl_dev2},
	{"dp_pce_rule_add_fn", &dp_pce_rule_add_fn, &dp_pce_rule_add2},
	{"dp_pce_rule_del_fn", &dp_pce_rule_del_fn, &dp_pce_rule_del2},
	{"dp_pce_rule_update_fn", &dp_pce_rule_update_fn, &dp_pce_rule_update2},
	{"dp_pce_rmon_get_fn", &dp_pce_rmon_get_fn, &dp_pce_rmon_get2},
	{"dp_pce_rmon_free_fn", &dp_pce_rmon_free_fn, &dp_pce_rmon_free2},
	{"dp_register_event_cb_fn", &dp_register_event_cb_fn, &dp_register_event_cb2},
	{"dp_xmit_fn", &dp_xmit_fn, &dp_xmit2},
	{"dp_get_netif_subifid_fn", &dp_get_netif_subifid_fn, &dp_get_netif_subifid2},
	{"dp_is_pmapper_check_fn", &dp_is_pmapper_check_fn, &dp_is_pmapper_check2},
	{"dp_get_tx_cbm_pkt_fn", &dp_get_tx_cbm_pkt_fn, &dp_get_tx_cbm_pkt2},
	{"dp_register_ops_fn", &dp_register_ops_fn, &dp_register_ops2},
	{"dp_rx_fn", &dp_rx_fn, &dp_rx2},
	{"dp_qos_get_q_mib_fn", &dp_qos_get_q_mib_fn, &dp_qos_get_q_mib2},
	{"dp_split_buffer_fn", &dp_split_buffer_fn, &dp_split_buffer2},
	{"dp_replace_ch_fn", &dp_replace_ch_fn, &dp_replace_ch2},
	{"dp_atoi_fn", &dp_atoi_fn, &dp_atoi2},
	{"dp_strncmpi_fn", &dp_strncmpi_fn, &dp_strncmpi2},
	{"dp_get_reinsert_cnt_fn", &dp_get_reinsert_cnt_fn, &dp_get_reinsert_cnt2},
	{"dp_get_lct_cnt_fn", &dp_get_lct_cnt_fn, &dp_get_lct_cnt2},
	{"dp_get_port_prop_fn", &dp_get_port_prop_fn, &dp_get_port_prop2},
	{"dp_get_subif_prop_fn", &dp_get_subif_prop_fn, &dp_get_subif_prop2},
	{"dp_is_ready_fn", &dp_is_ready_fn, &dp_is_ready2},
	{"dp_get_ops_fn", &dp_get_ops_fn, &dp_get_ops2},
	{"dp_set_mtu_size_fn", &dp_set_mtu_size_fn, &dp_set_mtu_size2},
	{"dp_spl_conn_fn", &dp_spl_conn_fn, &dp_spl_conn2},
	{"dp_spl_conn_get_fn", &dp_spl_conn_get_fn, &dp_spl_conn_get2},
	{"dp_qos_get_q_logic_fn", &dp_qos_get_q_logic_fn, &dp_qos_get_q_logic2},
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	{"dp_hostif_update_fn", &dp_hostif_update_fn, &dp_update_hostif2},
	{"dp_get_netif_stats_fn", &dp_get_netif_stats_fn, &dp_get_netif_stats2},
#endif
	{"dp_qos_codel_cfg_set_fn", &dp_qos_codel_cfg_set_fn, &dp_qos_codel_cfg_set2},
	{"dp_qos_codel_cfg_get_fn", &dp_qos_codel_cfg_get_fn, &dp_qos_codel_cfg_get2},
	{"dp_register_tx_fn", &dp_register_tx_fn, &dp_register_tx2},
	{"dp_register_rx_hook_fn", &dp_register_rx_hook_fn, &dp_register_rx_hook2},
	{"dp_deregister_rx_hook_fn", &dp_deregister_rx_hook_fn, &dp_deregister_rx_hook2},
	{"dp_ndo_setup_tc", &dp_ndo_setup_tc_fn, &dp_ndo_setup_tc2},
	{"dp_dev_update_tc", &dp_dev_update_tc_fn, &dp_dev_update_tc2},
	{"dp_bp_ndo_br_setlink", &dp_bp_ndo_br_setlink_fn, &dp_bp_ndo_br_setlink2},
	{"dp_bp_ndo_br_dellink", &dp_bp_ndo_br_dellink_fn, &dp_bp_ndo_br_dellink2},
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
	/*eg for LGM 5.15, PRX 5.15*/
	{"dp_ndo_get_port_parent_id", &dp_ndo_get_port_parent_id_fn, &dp_ndo_get_port_parent_id2},
	{"dp_ndo_get_phys_port_id", &dp_ndo_get_phys_port_id_fn, &dp_ndo_get_phys_port_id2},
	{"dp_ndo_get_phys_port_name", &dp_ndo_get_phys_port_name_fn, &dp_ndo_get_phys_port_name2},
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
	/*eg: for PRX 4.9*/
	{"dp_swdev_port_obj_add", &dp_swdev_port_obj_add_fn, &dp_swdev_port_obj_add2},
	{"dp_swdev_port_obj_del", &dp_swdev_port_obj_del_fn, &dp_swdev_port_obj_del2},
	{"dp_swdev_port_attr_get", &dp_swdev_port_attr_get_fn, &dp_swdev_port_attr_get2},
	{"dp_swdev_port_attr_set", &dp_swdev_port_attr_set_fn, &dp_swdev_port_attr_set2},
#endif
#endif /*CONFIG_DPM_DATAPATH_SWITCHDEV*/
	{"dp_dev_update_toe", &dp_dev_update_toe_fn, &dp_dev_update_toe2},
#if IS_ENABLED(CONFIG_MXL_VPN)
	{"dp_xdo_dev_state_add", &dp_xdo_dev_state_add_fn, &dp_xdo_dev_state_add2},
	{"dp_xdo_dev_state_delete", &dp_xdo_dev_state_delete_fn, &dp_xdo_dev_state_delete2},
	{"dp_xdo_dev_offload_ok", &dp_xdo_dev_offload_ok_fn, &dp_xdo_dev_offload_ok2},
	{"dp_xdo_dev_state_advance_esn", &dp_xdo_dev_state_advance_esn_fn, &dp_xdo_dev_state_advance_esn2},
	{"dp_dev_update_xfrm", &dp_dev_update_xfrm_fn, &dp_dev_update_xfrm2},
#endif
	{"dp_get_ts_info", &dp_get_ts_info_fn, &dp_get_ts_info2},
	{"dp_ndo_do_ioctl", &dp_ndo_do_ioctl_fn, &dp_ndo_do_ioctl2},
	{"dp_qos_get_q_global_parms", &dp_qos_get_q_global_parms_fn, &dp_qos_get_q_global_parms2},
};


void proc_fn_read(struct seq_file *s)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(fn_list); i++)
		dp_sprintf(s, "%s=&%lx: %lx\n",
			   fn_list[i].name,
			   (unsigned long)fn_list[i].f1,
			   *(unsigned long *)fn_list[i].f1);
}

void dp_init_fn(u32 flags)
{
	int i;
	unsigned long tmp;

	for (i = 0; i < ARRAY_SIZE(fn_list); i++) {
		if (flags & DP_PLATFORM_DE_INIT)
			tmp = 0;
		else
			tmp = (unsigned long)fn_list[i].f2;
		DP_DEBUG(DP_DBG_FLAG_REG, "fn[%d] for %s 0x%px 0x%px -> 0x%lx\n",
			i,
			fn_list[i].name,
			fn_list[i].f1,
			(void *)*(unsigned long *)fn_list[i].f1,
			tmp);
		*(unsigned long *)fn_list[i].f1 = (unsigned long)fn_list[i].f2;
	}
}

