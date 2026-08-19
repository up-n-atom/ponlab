/******************************************************************************
 *
 *  Copyright (c) 2020 - 2022 MaxLinear, Inc.
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_omci.h
 *
 * This is a PON Adapter header file, defining OMCI Managed Entity operations.
 */

#ifndef _PON_ADAPTER_OMCI_H_
#define _PON_ADAPTER_OMCI_H_

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 *  @{
 */

/** \defgroup PON_ADAPTER_OMCI_ME OMCI Managed Entity Handlers
 *  These functions are used to create Managed Entity handlers.
 *  @{
 */

/** Structure containing all OMCI Managed Entity operations pointers */
struct pa_omci_me_ops {
	/** Pointer to the ANI-G operations structure */
	const struct pon_adapter_ani_g_ops *ani_g;
	/** Pointer to the call control pmhd ops operations structure */
	const struct pa_call_control_pmhd_ops *call_ctrl_pmhd;
	/** Pointer to 802.1p Mapper Service Profile operations structure */
	const struct pa_dot1p_mapper_ops *dot1p_mapper;
	/** Pointer to 802.1p rate limiter operations structure */
	const struct pa_dot1p_rate_limiter_ops *dot1p_rate_limiter;
	/** Pointer to 802.1x port extension package operations structure */
	const struct pa_dot1x_port_ext_pkg_ops *dot1x_port_ext_pkg;
	/** Pointer to the Energy Consumption PMHD operations structure */
	const struct pa_energy_consumption_pmhd_ops *ener_consum_pmhd;
	/** Pointer to the Enhanced Security Control operations structure */
	const struct pon_adapter_enh_sec_ctrl_ops *enh_sec_ctrl;
	/** Pointer to the Enhanced TC operations structure */
	const struct pa_enhanced_tc_pmhd_ops *enhanced_tc_pmhd;
	/** Pointer to the Ethernet PMHD operations structure */
	const struct pa_eth_pmhd_ops *eth_pmhd;
	/** Pointer to the Extended VLAN operations structure */
	const struct pa_ext_vlan_ops *ext_vlan;
	/** Pointer to the FEC PMHD operations structure */
	const struct pa_fec_pmhd_ops *fec_pmhd;
	/** Pointer to the GAL Ethernet PMHD operations structure */
	const struct pon_adapter_gal_ethernet_pmhd_ops *gal_eth_pmhd;
	/** Pointer to the GAL Ethernet Profile operations structure */
	const struct pa_gal_eth_profile_ops *gal_eth_profile;
	/** Pointer to the GEM Interworking TP operations structure */
	const struct pa_gem_interworking_tp_ops *gem_itp;
	/** Pointer to the GEM Port Network CTP operations structure */
	const struct pa_gem_port_net_ctp_ops *gem_port_net_ctp;
	/** Pointer to the GEM Port PMHD operations structure */
	const struct pa_gem_port_pmhd_ops *gem_port_pmhd;
	/** Pointer to the GEM Port Network CTP PMHD operations structure */
	const struct pa_gem_port_net_ctp_pmhd_ops *gem_port_net_ctp_pmhd;
	/** Pointer to the MAC Bridge PMHD operations structure */
	const struct pa_mac_bridge_pmhd_ops *mac_bridge_pmhd;
	/** Pointer to the MAC Bridge Port Bridge Table Data operations
	 *  structure
	 */
	const struct pa_mac_bp_bridge_table_data_ops *mac_bp_table_data;
	/** Pointer to the MAC Bridge Port Config Data operations structure */
	const struct pa_mac_bp_config_data_ops *mac_bp_config_data;
	/** Pointer to the MAC Bridge Port filter preassign table operations
	 * structure
	 */
	const struct pa_mac_bp_filter_preassign_table_ops
		*mac_bp_filter_preassign_table;
	/** Pointer to the MAC Bridge Port filter table data operations
	 * structure
	 */
	const struct pa_mac_bp_filter_table_data_ops *mac_bp_filter_table_data;
	/** Pointer to the MAC Bridge Port PMHD operations structure */
	const struct pa_mac_bp_pmhd_ops *mac_bp_pmhd;
	/** Pointer to the MAC Bridge Port performance monitoring exceeded
	 * discard operations structure
	 */
	const struct pa_mac_bp_eth_pmhd_mtu_exceeded_discard_ops
		*mac_bp_eth_pmhd_mtu_exceeded_discard;
	/** Pointer to the MAC Bridge Service Profile operations structure */
	const struct pa_mac_bridge_service_profile_ops
		*mac_bridge_service_profile;
	/** Pointer to Counters for PLOAM message transfers. */
	const struct pa_management_cnt_ops *mngmt_cnt;
	/** Pointer to the Multicast GEM Interworking TP operations structure */
	const struct pa_mc_gem_itp_ops *mc_gem_itp;
	/** Pointer to the Multicast Operations Profile operations structure */
	const struct pa_mc_profile_ops *mc_profile;
	/** Pointer to the OLT-G operations structure */
	const struct pa_olt_g_ops *olt_g;
	/** Pointer to the ONU-G operations structure */
	const struct pa_onu_g_ops *onu_g;
	/** Pointer to the ONU Dynamic Power Management operations structure */
	const struct pa_onu_dyn_pwr_mngmt_ctrl_ops
		*onu_dyn_pwr_mngmt_ctrl;
	/** Pointer to the ONU2-G operations structure */
	const struct pa_onu2_g_ops *onu2_g;
	/** Pointer to the onu loop detection operations structure */
	const struct pa_onu_loop_detection_ops *onu_loop_detection;
	/** Pointer to the onu remote debug operations structure */
	const struct pa_onu_remote_debug_ops *onu_remote_debug;
	/** Pointer to the PPTP Ethernet UNI operations structure */
	const struct pa_pptp_eth_uni_ops *pptp_eth_uni;
	/** Pointer to the PPTP local craft terminal UNI operations structure */
	const struct pa_pptp_lct_uni_ops *pptp_lct_uni;
	/** Pointer to the PPTP POTS UNI operations structure */
	const struct pa_pptp_pots_uni_ops *pptp_pots_uni;
	/** Pointer to the PPTP XDSL UNI operations structure */
	const struct pa_pptp_xdsl_uni_ops *pptp_xdsl_uni;
	/** Pointer to RTP PMHD operations structure */
	const struct pa_rtp_pmhd_ops *rtp_pmhd;
	/** Pointer to Sip Agent PMHD operations structure */
	const struct pa_sip_agent_pmhd_ops *sip_agent_pmhd;
	/** Pointer to structure Sip Call Init PMHD */
	const struct pa_sip_call_init_pmhd_ops *sip_call_init_pmhd;
	/** Pointer to the T-CONT operations structure */
	const struct pa_tcont_ops *tcont;
	/** Pointer to the Traffic Descriptor operations structure */
	const struct pa_traffic_descriptor_ops *traffic_descriptor;
	/** Pointer to the Traffic Scheduler operations structure */
	const struct pa_traffic_scheduler_ops *traffic_scheduler;
	/** Pointer to the TWDM Channel operations structure */
	const struct pa_twdm_channel_ops *twdm_channel;
	/** Pointer to the TWDM Channel XGEM PMHD operations structure */
	const struct pa_twdm_channel_xgem_pmhd_ops *twdm_channel_xgem_pmhd;
	/** Pointer to the TWDM Channel PHY_LODS PMHD operations structure */
	const struct pa_twdm_channel_phy_lods_pmhd_ops
		*twdm_channel_phy_lods_pmhd;
	/** Pointer to the TWDM System Profile operations structure */
	const struct pa_twdm_system_profile_ops *twdm_system_profile;
	/** pointer to the TWDM Channel PLOAM opertation structure (Part 1-3)*/
	const struct pa_twdm_channel_ploam_pmhd_ops *twdm_channel_ploam_pmhd;
	/** pointer to the TWDM Channel Tuning opertation structure (Part 1-3)*/
	const struct pa_twdm_channel_tuning_pmhd_ops *twdm_channel_tuning_pmhd;
	/** Pointer to the User Network Interfaces (UNIs) supported by GEM
	 *  operations structure
	 */
	const struct pa_uni_g_ops *uni_g;
	/** Pointer to the virtual Ethernet interface point operations
	 *  structure
	 */
	const struct pa_virtual_ethernet_interface_point_ops
		*virtual_ethernet_interface_point;
	/** Pointer to the VLAN Tagging Filter Data operations structure */
	const struct pa_vlan_tagging_filter_data_ops *vlan_tag_filter_data;
	/** Pointer to the VLAN Tagging Operation Configuration Data operations
	 *  structure
	 */
	const struct pa_vlan_tagging_operation_config_data_ops
		*vlan_tag_oper_cfg_data;
	/** Pointer to the Priority Queue operations structure */
	const struct pa_priority_queue_ops *priority_queue;
	/** Pointer to the vendor emop operations structure */
	const struct pon_adapter_vendor_emop_ops *vendor_emop;
	/** Pointer to the SIP Agent Config Data operations structure */
	const struct pa_sip_agent_config_data_ops *sip_agent_config_data;
	/** Pointer to the SIP User Data operations structure */
	const struct pa_sip_user_data_ops *sip_user_data;
	/** Pointer to the software image download operations structure */
	const struct pa_sw_image_ops *sw_image;
	/** Pointer to the IP Host operations structure */
	const struct pa_ip_host_ops *ip_host;
	/** Pointer to the VoIP Line Status operations structure  */
	const struct pa_voip_line_status_ops *voip_line_status;
	/** Pointer to the VoIP Voice CTP operations structure */
	const struct pa_voip_voice_ctp_ops *voip_voice_ctp;
};

/** @} */ /* PON_ADAPTER_OMCI_ME */

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif
