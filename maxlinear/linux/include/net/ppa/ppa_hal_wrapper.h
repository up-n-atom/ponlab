/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PPA_HAL_WRAPPER__
#define __PPA_HAL_WRAPPER__
/*******************************************************************************
 **
 ** FILE NAME	: ppa_hal_wrapper.h
 ** PROJECT	: PPA
 ** MODULES	: PPA Wrapper for various HAL drivers
 **
 ** DATE	: 27 Feb 2014
 ** AUTHOR	: Kamal Eradath
 ** DESCRIPTION	: PPA Wrapper for HAL Driver API
 ** Copyright (c) 2020 - 2025 MaxLinear, Inc.
 ** COPYRIGHT	: Copyright (c) 2017 Intel Corporation
 ** Copyright (c) 2014 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 ** HISTORY
 ** $Date		$Author		 $Comment
 ** 27 Feb 2014		Kamal Eradath	 Initiate Version
 *******************************************************************************/
#include <net/ppa/ppa_hal_api.h>
/* HAL selector table node;*/
#ifndef MAX_TUNNEL_ENTRIES
#define MAX_TUNNEL_ENTRIES			16
#endif
extern uint8_t ppa_drv_get_num_tunnel_entries(void);
extern uint8_t ppa_drv_get_num_registred_hals(void);
extern uint32_t ppa_drv_generic_hal_register(uint32_t hal_id, ppa_generic_hook_t generic_hook);
extern void ppa_drv_generic_hal_deregister(uint32_t hal_id);
extern uint32_t ppa_hsel_get_hal_stats(PPA_HAL_STATS *stat, uint32_t flag, uint32_t hal_id);
/* All the HAL layers initialized must get registered with the HAL selectors regarding the "capabilities" it supports*/
/* This funcion must be invoked by the all HALs while hal_init is called to register all the supported capabilities*/
extern uint32_t ppa_drv_register_cap(PPA_API_CAPS cap, uint8_t wt, PPA_HAL_ID hal_id);
extern uint32_t ppa_drv_deregister_cap(PPA_API_CAPS cap, PPA_HAL_ID hal_id);
/* helper functions*/
extern int32_t ppa_select_hals_from_caplist(uint8_t start, uint8_t num_entries, PPA_HSEL_CAP_NODE *caps_list);
extern int32_t ppa_group_hals_in_capslist(uint8_t start, uint8_t num_entries, PPA_HSEL_CAP_NODE *caps_list);
extern uint32_t ppa_hsel_set_acc_mode(PPA_ACC_ENABLE *cfg, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_get_acc_mode(PPA_ACC_ENABLE *cfg, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_hal_init(PPA_HAL_INIT_CFG *cfg, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_hal_exit(uint32_t flag, uint32_t hal_id);
/* Function to be called for adding additional switch port configuration for bridge acceleration*/
extern uint32_t ppa_drv_add_br_port(PPA_BR_PORT_INFO *entry, uint32_t flag);
extern uint32_t ppa_drv_del_br_port(PPA_BR_PORT_INFO *entry, uint32_t flag);
extern uint32_t ppa_hsel_get_generic_itf_mib( PPA_ITF_MIB_INFO *mib, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_add_if(struct netif_info *ifinfo, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_del_if(struct netif_info *ifinfo, uint32_t flag, uint32_t hal_id);
/******************************************************************************************************/
/* functions to be used in ppa_api_hal_selector.c*/
/******************************************************************************************************/
extern uint32_t ppa_hsel_add_complement(PPA_ROUTING_INFO *entry , uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_del_complement(PPA_ROUTING_INFO *entry , uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_add_routing_entry(PPA_ROUTING_INFO *entry , uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_del_routing_entry(PPA_ROUTING_INFO *entry, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_add_sess_meta(PPA_ROUTING_INFO *entry, PPA_BUF *skb, void *txifinfo, uint32_t hal_id);
extern uint32_t ppa_hsel_del_sess_meta(PPA_ROUTING_INFO *entry, uint32_t hal_id);
extern uint32_t ppa_hsel_update_routing_entry(PPA_ROUTING_INFO *entry , uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_add_wan_mc_entry(PPA_MC_INFO *entry, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_del_wan_mc_entry(PPA_MC_INFO *entry, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_update_wan_mc_entry(PPA_MC_INFO *entry, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_add_tunnel_entry(PPA_TUNNEL_INFO *entry, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_del_tunnel_entry(PPA_TUNNEL_INFO *entry, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_get_routing_entry_bytes(PPA_ROUTING_INFO *entry, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_get_mc_entry_bytes(PPA_MC_INFO *entry, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_test_and_clear_hit_stat(PPA_ROUTING_INFO *entry, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_test_and_clear_mc_hit_stat(PPA_MC_INFO *entry, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_add_outer_vlan_entry(PPA_OUT_VLAN_INFO *entry, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_del_outer_vlan_entry(PPA_OUT_VLAN_INFO *entry, uint32_t flag, uint32_t hal_id);
extern uint32_t ppa_hsel_get_outer_vlan_entry(PPA_OUT_VLAN_INFO *entry, uint32_t flag, uint32_t hal_id);
#if defined(RTP_SAMPLING_ENABLE) && RTP_SAMPLING_ENABLE
extern uint32_t ppa_hsel_set_wan_mc_rtp(PPA_MC_INFO *entry, uint32_t hal_id);
extern uint32_t ppa_hsel_get_mc_rtp_sampling_cnt(PPA_MC_INFO *entry, uint32_t hal_id);
#endif
/******************************************************************************************************/
#endif /*end of __PPA_HAL_WRAPPER__ */
