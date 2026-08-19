#ifndef __PPA_API_CORE_H__20081103_1920__
#define __PPA_API_CORE_H__20081103_1920__

/*******************************************************************************
 **
 ** FILE NAME    : ppa_api_core.h
 ** PROJECT      : PPA
 ** MODULES      : PPA API (Routing/Bridging Acceleration APIs)
 **
 ** DATE         : 3 NOV 2008
 ** AUTHOR       : Xu Liang
 ** DESCRIPTION  : PPA Protocol Stack Hook API Implementation Header File
 ** COPYRIGHT    : Copyright © 2020-2025 MaxLinear, Inc.
 **                Copyright (c) 2017 Intel Corporation
 **
 **   For licensing information, see the file 'LICENSE' in the root folder of
 **   this software module.
 **
 ** HISTORY
 ** $Date        $Author         $Comment
 ** 03 NOV 2008  Xu Liang        Initiate Version
 *******************************************************************************/
/*! \file ppa_api_core.h
  \brief This file contains es.
  provide PPA API.
 */

/** \addtogroup PPA_CORE_API PPA Core API
  \brief  PPA Core API provide PPA core accleration logic and API
  The API is defined in the following two source files
  - ppa_api_core.h: Header file for PPA API
  - ppa_api_core.c: C Implementation file for PPA API
 */
/* @{ */

/*
 * ####################################
 *              Definition
 * ####################################
 */

#define PPA_SESSION_FILTER_PROTO 0x0001
#define PPA_SESSION_FILTER_SPORT 0x0002
#define PPA_SESSION_FILTER_DPORT 0x0004
typedef struct {
	struct list_head 	list;
	struct rcu_head 	rcu;
	uint16_t		ip_proto;			/* IP prorocol */
	uint16_t		src_port;			/* Source port */
	uint16_t		dst_port;			/* Destination port */
	uint16_t		flags;			  	/* Indicates which parameters to be checked */
	uint16_t		hit_cnt;			/* Indicates hit count */
} FILTER_INFO;

enum VERSION_INDEX {
	VERSION_MAJOR = 0, /*!< Major Version Index */
	VERSION_MID, /*!< Mid Version Index */
	VERSION_MINOR, /*!< Minor Version Index */
	VERSION_TAG, /*!< Tag Version Index */
	VERSION_MAX /*!< Tag Version Index */
};

#if IS_ENABLED(CONFIG_MCAST_HELPER)
#include <net/mcast_helper_api.h>

int32_t mcast_module_config(uint32_t grp_idx,
			    struct net_device *member,
			    void *mc_stream,
			    uint32_t flags);
#endif /* CONFIG_MCAST_HELPER */

#if IS_ENABLED(CONFIG_PPA_MPE_IP97)
extern uint32_t ppa_add_ipsec_tunnel_tbl_entry(PPA_XFRM_STATE * entry,
					       sa_direction dir,
					       uint32_t *tunnel_index);
extern uint32_t ppa_get_ipsec_tunnel_tbl_entry(PPA_XFRM_STATE *entry,
					       sa_direction *dir,
					       uint32_t *tunnel_index);
extern uint32_t ppa_add_ipsec_tunnel_tbl_update(sa_direction dir,
						uint32_t tunnel_index);
#endif
extern FILTER_INFO* get_matched_entry (PPA_HAL_ID hal_id, FILTER_INFO *entry);
extern bool add_filter_entry (PPA_HAL_ID hal_id, FILTER_INFO *entry);
extern bool del_filter_entry (PPA_HAL_ID hal_id, FILTER_INFO *entry);
extern void del_routing_session_cb(void *p_item);

void ppa_get_ver_id(uint32_t *p_major,
		uint32_t *p_mid,
		uint32_t *p_minor,
		char *p_tag);
int32_t ppa_init(PPA_INIT_INFO *p_info, uint32_t flags);
int32_t ppa_exit(void);
int32_t ppa_enable(uint32_t lan_rx_ppa_enable, uint32_t wan_rx_ppa_enable, uint32_t flags);
int32_t ppa_get_status(uint32_t *lan_rx_ppa_enable, uint32_t *wan_rx_ppa_enable, uint32_t flags);
#if IS_ENABLED(CONFIG_MIB_MODE_ENABLE)
int32_t ppa_set_mib_mode(uint8_t mib_mode);
int32_t ppa_get_mib_mode(uint8_t *mib_mode);
#endif
int32_t ppa_get_ct_stats(PPA_SESSION * p_session, PPA_CT_COUNTER * pCtCounter);
int32_t ppa_session_add(PPA_BUF *ppa_buf, PPA_SESSION *p_session, uint32_t flags);
int32_t ppa_session_modify(PPA_SESSION *p_session, PPA_SESSION_EXTRA *p_extra, uint32_t flags);
int32_t ppa_session_get(PPA_SESSION ***pp_sessions, PPA_SESSION_EXTRA **pp_extra,
						int32_t *p_num_entries, uint32_t flags);
int32_t ppa_mc_group_update(PPA_MC_GROUP *ppa_mc_entry, uint32_t flags);
int32_t ppa_mc_entry_get(IP_ADDR_C ip_mc_group, IP_ADDR_C src_ip,
						PPA_SESSION_EXTRA *p_extra, uint32_t flags);
#if defined(RTP_SAMPLING_ENABLE) && RTP_SAMPLING_ENABLE
int32_t ppa_mc_entry_rtp_get(IP_ADDR_C ip_mc_group, IP_ADDR_C src_ip, uint8_t *p_RTP_flag);
#endif
int32_t ppa_multicast_pkt_srcif_add(PPA_BUF *pkt_buf, PPA_NETIF * rx_if);
int32_t ppa_inactivity_status(PPA_U_SESSION *p_session);
int32_t ppa_set_session_inactivity(PPA_U_SESSION *p_session, int32_t timeout);

int32_t ppa_bridge_entry_add(uint8_t *mac_addr, PPA_NETIF *brif, PPA_NETIF *netif, uint32_t flags);
int32_t ppa_bridge_entry_delete(uint8_t *mac_addr, PPA_NETIF *brif, uint32_t flags);
int32_t ppa_bridge_entry_hit_time(uint8_t *mac_addr, PPA_NETIF *brif, uint32_t *p_hit_time);

int32_t ppa_get_if_stats(PPA_IFNAME *ifname, PPA_IF_STATS *p_stats, uint32_t flags);
int32_t ppa_get_accel_stats(PPA_IFNAME *ifname, PPA_ACCEL_STATS *p_stats, uint32_t flags);
int32_t ppa_get_if(int32_t *num_ifs, PPA_IFINFO **ifinfo, uint32_t flags);
#if IS_ENABLED(CONFIG_PPA_API_DIRECTCONNECT) && CONFIG_PPA_API_DIRECTCONNECT
int32_t ppa_disconn_if(PPA_NETIF *netif, PPA_DP_SUBIF * subif, uint8_t *mac, uint32_t flags);
#endif
int32_t ppa_get_max_entries(PPA_MAX_ENTRY_INFO * max_entry, uint32_t flags);
int32_t ppa_ip_comare(PPA_IPADDR ip1, PPA_IPADDR ip2, uint32_t flag);
int32_t ppa_zero_ip(PPA_IPADDR ip);
int32_t ppa_ip_sprintf(char *buf, PPA_IPADDR ip, uint32_t flag);

int32_t ppa_set_switch_mode(bool mode);
#endif  /*  __PPA_API_CORE_H__20081103_1920__ */
/* @} */
