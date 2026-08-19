/******************************************************************************
 *  Copyright (c) 2020 - 2022 MaxLinear, Inc.
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_standard_me.h
 *
 * This is a PON Adapter header file, including other header files for
 * individual Managed Entities.
 */

#ifndef _pon_adapter_standard_me_h
#define _pon_adapter_standard_me_h

#include "me/pon_adapter_ani_g.h"
#include "me/pon_adapter_call_control_pmhd.h"
#include "me/pon_adapter_dot1p_mapper.h"
#include "me/pon_adapter_dot1p_rate_limiter.h"
#include "me/pon_adapter_dot1x_port_ext_pkg.h"
#include "me/pon_adapter_energy_consumption_pmhd.h"
#include "me/pon_adapter_enhanced_security_control.h"
#include "me/pon_adapter_enhanced_tc_pmhd.h"
#include "me/pon_adapter_ethernet_pmhd.h"
#include "me/pon_adapter_ext_vlan.h"
#include "me/pon_adapter_fec_pmhd.h"
#include "me/pon_adapter_gal_ethernet_pmhd.h"
#include "me/pon_adapter_gal_ethernet_profile.h"
#include "me/pon_adapter_gem_interworking_tp.h"
#include "me/pon_adapter_gem_port_network_ctp.h"
#include "me/pon_adapter_gem_port_net_ctp_pmhd.h"
#include "me/pon_adapter_gem_port_pmhd.h"
#include "me/pon_adapter_ip_host.h"
#include "me/pon_adapter_mac_bridge_pmhd.h"
#include "me/pon_adapter_mac_bridge_port_bridge_table_data.h"
#include "me/pon_adapter_mac_bridge_port_config_data.h"
#include "me/pon_adapter_mac_bridge_port_filter_preassign_table.h"
#include "me/pon_adapter_mac_bridge_port_filter_table_data.h"
#include "me/pon_adapter_mac_bridge_port_pmhd.h"
#include "me/pon_adapter_mac_bridge_service_profile.h"
#include "me/pon_adapter_management_pmhd.h"
#include "me/pon_adapter_multicast_gem_interworking_tp.h"
#include "me/pon_adapter_multicast_operations_profile.h"
#include "me/pon_adapter_olt_g.h"
#include "me/pon_adapter_onu2_g.h"
#include "me/pon_adapter_onu_dyn_pwr_mngmt_ctrl.h"
#include "me/pon_adapter_onu_g.h"
#include "me/pon_adapter_onu_loop_detection.h"
#include "me/pon_adapter_onu_remote_debug.h"
#include "me/pon_adapter_pptp_ethernet_uni.h"
#include "me/pon_adapter_pptp_lct_uni.h"
#include "me/pon_adapter_pptp_pots_uni.h"
#include "me/pon_adapter_pptp_xdsl_uni.h"
#include "me/pon_adapter_priority_queue.h"
#include "me/pon_adapter_rtp_pmhd.h"
#include "me/pon_adapter_sip_agent_config_data.h"
#include "me/pon_adapter_sip_agent_pmhd.h"
#include "me/pon_adapter_sip_call_init_pmhd.h"
#include "me/pon_adapter_sip_user_data.h"
#include "me/pon_adapter_sw_image.h"
#include "me/pon_adapter_tcont.h"
#include "me/pon_adapter_traffic_descriptor.h"
#include "me/pon_adapter_traffic_scheduler.h"
#include "me/pon_adapter_twdm_channel.h"
#include "me/pon_adapter_twdm_channel_xgem_pmhd.h"
#include "me/pon_adapter_twdm_channel_ploam_pmhd.h"
#include "me/pon_adapter_twdm_channel_tuning_pmhd.h"
#include "me/pon_adapter_twdm_channel_phy_lods_pmhd.h"
#include "me/pon_adapter_twdm_system_profile.h"
#include "me/pon_adapter_uni_g.h"
#include "me/pon_adapter_virtual_ethernet_interface_point.h"
#include "me/pon_adapter_vlan_tagging_filter_data.h"
#include "me/pon_adapter_vlan_tagging_operation_config_data.h"
#include "me/pon_adapter_voip_line_status.h"
#include "me/pon_adapter_voip_voice_ctp.h"
#include "me/pon_adapter_vendor_emop.h"

/* TODO: global struct */

#endif
