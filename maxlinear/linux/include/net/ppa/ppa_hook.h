/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PPA_HOOK_H__
#define __PPA_HOOK_H__
/*******************************************************************************
 **
 ** FILE NAME    : ppa_hook.h
 ** PROJECT      : PPA
 ** MODULES      : PPA Protocol Stack Hooks
 **
 ** DATE         : 3 NOV 2008
 ** AUTHOR       : Xu Liang
 ** DESCRIPTION  : PPA Protocol Stack Hook Pointers Header File
 ** Copyright (c) 2020 - 2023 MaxLinear, Inc.
 ** COPYRIGHT    : Copyright (c) 2017 Intel Corporation
 ** Copyright (c) 2010 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 ** HISTORY
 ** $Date        $Author         $Comment
 ** 03 NOV 2008  Xu Liang        Initiate Version
 *******************************************************************************/
/*! \file ppa_hook.h
  \brief This file contains all exported HOOK API to linux Kernel and user space via ioctl API.
 */
#include <net/ppa/ppa_api_common.h>
/** \addtogroup  PPA_HOOK_API */
/*@{*/
/*
 * ####################################
 *             Declaration
 * ####################################
 */
#ifdef __KERNEL__
/*!
  \brief Add a PPA routing session entry
  \param[in] skb Pointer to the packet buffer for which PPA session addition is to be done.
  \param[in] p_session Points to the connection tracking session to which this packet belongs. It maybe passed as NULL in which case PPA will try to determine it using the PPA stack adaptation layer.
  \param[in] flags  Flags as valid for the PPA session Valid \n
  values are one or more of: \n
  - PPA_F_SESSION_BIDIRECTIONAL \n
  - PPA_F_BRIDGED_SESSION \n
  - PPA_F_STATIC_ENTRY \n
  - PPA_F_DROP_PACKET \n
  - PPA_F_SESSION_ORG_DIR \n
  - PPA_F_SESSION_REPLY_DIR \n
  - PPA_F_BEFORE_NAT_TRANSFORM \n
  \return The return value can be any one of the following:  \n
  - PPA_SESSION_NOT_ADDED \n
  - PPA_SESSION_ADDED \n
  - PPA_SESSION_EXISTS \n
  \note   Linux 2.4 Hook-up recommendation \n
  Must be hooked into the stack before the PREROUTING and after the POSTROUTING hooks In ip_conntrack_in() function in file ip_conntrack_core.c, with the flag PPA_F_BEFORE_NAT_TRANSFORM specified. \n
  1) In ip_finish_output2(), the hook should be invoked after NAT transforms are applied and at the very beginning of function call. \n
  Linux 2.6 Hook-up recommendation \n
  Must be in netfilter IPV4 or IPV6 hook PREROUTRING and POSTROUTING hooks. In nf_conntrack_in for function which is common for both IPV4 and V6 connection tracking, PPA_F_BEFORE_NAT_TRANSFORM. In ip_finish_output2(), the hook should be invoked after NAT transforms are applied at netfilter POSTROUTING hooks and at the very beginning of function call.
 */
#ifdef NO_DOXY
extern int32_t (*ppa_hook_session_add_fn)(PPA_BUF *skb, PPA_SESSION *p_session, uint32_t flags);
#else
extern int32_t ppa_hook_session_add_fn(PPA_BUF *skb, PPA_SESSION *p_session, uint32_t flags);
#endif
extern int32_t (*ppa_hook_session_bradd_fn)(PPA_BUF *skb, PPA_SESSION *p_session, uint32_t flags);
/*!
  \brief Del a PPA routing session entry
  \param[in] p_session Points to the connection tracking session to which this packet belongs. It maybe passed as NULL in which case PPA will try to determine it using the PPA stack adaptation layer.
  \param[in] flags Reserved currently.
  \return The return value can be any one of the following:  \n
  - PPA_SUCCESS \n
  - PPA_FAILURE
 */
#ifdef NO_DOXY
extern int32_t (*ppa_hook_session_del_fn)(PPA_SESSION *p_session, uint32_t flags);
#else
extern int32_t ppa_hook_session_del_fn(PPA_SESSION *p_session, uint32_t flags);
#endif
/*!
  \brief It is used to get session priority  of a ppa session
  \param[in] p_session Points to the connection tracking session to which this packet belongs. It maybe passed as NULL in which case PPA will try to determine it using the PPA stack adaptation layer.
  \param[in] flags Reserved currently.
  \return The return value is PPA session priority
 */
#ifdef NO_DOXY
extern int32_t (*ppa_hook_session_prio_fn)(PPA_SESSION *p_session, uint32_t flags);
#else
extern int32_t ppa_hook_session_prio_fn(PPA_SESSION *p_session, uint32_t flags);
#endif
/*!
  \brief Returns session statistics
  \param[in]  p_session   Pointer to the session
  \param[in]  pCtCounter  Pointer to the statistics structure
  \return The return value can be any one of the following:  \n
  - PPA_SUCCESS \n
  - PPA_FAILURE
 */
#ifdef NO_DOXY
extern int32_t (*ppa_hook_get_ct_stats_fn)(PPA_SESSION *p_session, PPA_CT_COUNTER *pCtCounter);
#else
extern int32_t ppa_hook_get_ct_stats_fn(PPA_SESSION *p_session, PPA_CT_COUNTER *pCtCounter);
#endif
/*!
  \brief Checks if the "accelerated" PPA session should be timed out due to inactivity.
  \param[in]  p_session Pointer to PPA unicast or multicast session.
  \return The return value can be any one of the following:  \n
  - PPA_TIMEOUT if the PPA session inactivity timer has expired \n
  - PPA_HIT if the PPA session has been active
 */
#ifdef NO_DOXY
extern int32_t (*ppa_hook_inactivity_status_fn)(PPA_U_SESSION *p_session);
#else
extern int32_t ppa_hook_inactivity_status_fn(PPA_U_SESSION *p_session);
#endif
/*!
  \brief Update the session inactivity timeout for a PPA session as per the session inactivity configuration in the protocol stack.
  \param[in] p_session Pointer to PPA unicast or multicast session.
  \param[in] timeout  Timeout value for session inactivity in  seconds.
  \return The return value can be any one of the following:  \n
  - PPA_SUCCESS \n
  - PPA_FAILURE \n
 */
#ifdef NO_DOXY
extern int32_t (*ppa_hook_set_inactivity_fn)(PPA_U_SESSION *p_session, int32_t timeout);
#else
extern int32_t ppa_hook_set_inactivity_fn(PPA_U_SESSION *p_session, int32_t timeout);
#endif
/*!
  \brief Add or update a MAC entry and its source ethernet port information in the PPA bridge table.
  \param[in]  mac_addr Pointer to MAC address to add to PPA bridge table.
  \param[in]  brif pointer to PPA net interface sttructure of the bridge.
  \param[in]  netif   Pointer to PPA net interface which is the source of the MAC address.
  \param[in]  flags Valid values are:
  - PPA_F_BRIDGE_LOCAL - traffic is destined for local termination.
  - PPA_F_STATIC_ENTRY - static MAC address entry in the PPA bridge table. It will not be aged out.
  - PPA_F_DROP_PACKET - firewall action. Always drop packets with this MAC destination.
  \return The return value can be any one of the following:  \n
  - PPA_SUCCESS \n
  - PPA_FAILURE \n
  \note Static MAC entry updates and MAC address drop filters can be configured from userspace.  For dynamic entries, the function must be hooked from bridging code where new entries are inserted into bridge mac table (or forwarding database, fdb).
  Linux 2.4 Hook-up recommendation \n
  Hook in kernel function br_fdb_insert() in net/bridge/br_fdb.c.  For Linux bridging code, the netif is given by fdb->dst->dev field where fdb points to a MAC entry.
 */
#ifdef NO_DOXY
extern int32_t (*ppa_hook_bridge_entry_add_fn)(uint8_t *mac_addr, PPA_NETIF *brif, PPA_NETIF *netif, uint32_t flags);
#else
extern int32_t ppa_hook_bridge_entry_add_fn(uint8_t *mac_addr, PPA_NETIF *brif, PPA_NETIF *netif, uint32_t flags);
#endif
/*!
  \brief Delete a MAC entry from PPA Bridge table since the MAC entry is aged out or administratively triggered.
  \param[in]  mac_addr Pointer to MAC address to delete from PPA bridge table.
  \param[in]  brif pointer to PPA net interface sttructure of the bridge.
  \param[in]  flags Reserved
  \return The return value can be any one of the following:  \n
  - PPA_SUCCESS \n
  - PPA_FAILURE
 */
#ifdef NO_DOXY
extern int32_t (*ppa_hook_bridge_entry_delete_fn)(uint8_t *mac_addr, PPA_NETIF *brif, uint32_t flags);
#else
extern int32_t ppa_hook_bridge_entry_delete_fn(uint8_t *mac_addr, PPA_NETIF *brif, uint32_t flags);
#endif
/*!
  \brief Get latest packet arriving time for the specified MAC address entry. This is used for aging out decisions for the MAC entry.
  \param[in]  mac_addr Pointer to MAC address whose entry hit time is being queried
  \param[in]  brif pointer to PPA net interface sttructure of the bridge.
  \param[out] p_hit_time Provides the latest packet arriving time in seconds from system bootup.
  \return The return value can be any one of the following:  \n
  - PPA_SUCCESS \n
  - PPA_FAILURE
  \note Linux 2.4 Hook-up recommendation \n
  This API can be hooked in function br_fdb_cleanup(). In Linux, there is a kernel thread (br_fdb_cleanup) polling each entry in the MAC table and removes entries without traffic for a long time.
 */
#ifdef NO_DOXY
extern int32_t (*ppa_hook_bridge_entry_hit_time_fn)(uint8_t *mac_addr, PPA_NETIF *brif, uint32_t *p_hit_time);
#else
extern int32_t ppa_hook_bridge_entry_hit_time_fn(uint8_t *mac_addr, PPA_NETIF *brif, uint32_t *p_hit_time);
#endif
#if defined(PPA_IF_MIB) && PPA_IF_MIB
/*!
  \brief Returns per interface statistics kept by the PPA which is a superset of the per Interface statistics above. This provides, for example, fastpath routed and bridged statistics.
  \param[in] ifname Pointer to the interface name
  \param[out] p_stats  Pointer to the Statistics data structure of the interface.
  \param[in] flags Reserved.
  \return The return value can be any one of the following:  \n
  - PPA_SUCCESS on sucess \n
  - PPA_FAILURE on error
  \note  This function is only implemented for D4 firmware.
 */
#ifdef NO_DOXY
extern int32_t (*ppa_hook_get_netif_accel_stats_fn)(PPA_IFNAME *ifname, PPA_NETIF_ACCEL_STATS *p_stats, uint32_t flags);
#else
extern int32_t ppa_hook_get_netif_accel_stats_fn(PPA_IFNAME *ifname, PPA_NETIF_ACCEL_STATS *p_stats, uint32_t flags);
#endif
#endif
#if defined(CONFIG_PPA_API_DIRECTCONNECT) && CONFIG_PPA_API_DIRECTCONNECT
/*!
  \brief Check if network interafce like ACA is a fastpath interface
  \param[in] netif Pointer to the network interface structure in the protocol stack. For eg. pointer to a struct net_device
  \param[in] ifname Interface name
  \param[in] flags  Reserved for future use
  \return The return value can be any one of the following:  \n
  - 1 if ACA or WLAN fastpath interface \n
  - 0 otherwise
 */
#ifdef NO_DOXY
extern int32_t (*ppa_check_if_netif_fastpath_fn)(PPA_NETIF *netif, char *ifname, uint32_t flags);
#else
extern int32_t ppa_check_if_netif_fastpath_fn(PPA_NETIF *netif, char *ifname, uint32_t flags);
#endif
/*!
  \brief Deletes an WAVE500 STA session information from the PPA session list.
  \param[in] dev Pointer to the network device structure in the protocol stack. For eg. pointer to a struct netdevice
  \param[in] subif Pointer to the sub-interface structure in the datapath library.
  \param[in] mac station mac address
  \param[in] flags  Reserved for future use
  \return The return value can be any one of the following:  \n
  - PPA_SUCCESS on sucess \n
  - PPA_FAILURE on error
 */
#ifdef NO_DOXY
extern int32_t (*ppa_hook_disconn_if_fn)(PPA_NETIF *dev, PPA_DP_SUBIF *subif, uint8_t *mac, uint32_t flags);
#else
extern int32_t ppa_hook_disconn_if_fn(PPA_NETIF *dev, PPA_DP_SUBIF *subif, uint8_t *mac, uint32_t flags);
#endif
#endif
/*#ifdef CONFIG_PPA_GRE*/
extern uint32_t (*ppa_is_ipv4_gretap_fn)(struct net_device *dev);
extern uint32_t (*ppa_is_ipv6_gretap_fn)(struct net_device *dev);
/*#endif*/
extern int32_t (*ppa_ppp_get_chan_info_fn)(struct net_device *ppp_dev, struct ppp_channel **chan);
extern int32_t (*ppa_check_pppoe_addr_valid_fn)(struct net_device *dev, struct pppoe_addr *pa);
extern int32_t (*ppa_get_pppoa_info_fn)(struct net_device *dev, void *pvcc, uint32_t pppoa_id, void *value);
extern int32_t (*ppa_get_pppol2tp_info_fn)(struct net_device *dev, void *po, uint32_t pppol2tp_id, void *value);
extern int32_t (*ppa_if_is_ipoa_fn)(struct net_device *netdev, char *ifname);
extern int32_t (*ppa_if_is_br2684_fn)(struct net_device *netdev, char *ifname);
extern int32_t (*ppa_br2684_get_vcc_fn)(struct net_device *netdev, struct atm_vcc **pvcc);
extern int32_t (*ppa_if_ops_veth_xmit_fn)(struct net_device *dev);
/*!
  \brief This is to set the lan separation flag
  \param[in] flag Lan separation flag
  \return int32_t, The return value can be 0 or 1
 */
#ifdef NO_DOXY
extern int32_t(*ppa_hook_set_lan_seperate_flag_fn)(uint32_t flag);
#else
extern int32_t ppa_hook_set_lan_seperate_flag_fn(uint32_t flag);
#endif
/*!
  \brief This is to set the wan separation flag
  \param[in] flag Wan separation flag
  \return int32_t, The return value can be 0 or 1
 */
#ifdef NO_DOXY
extern int32_t(*ppa_hook_set_wan_seperate_flag_fn)(uint32_t flag);
#else
extern int32_t ppa_hook_set_wan_seperate_flag_fn (uint32_t flag);
#endif
/*!
  \brief This is to get the lan separation flag
  \param[in] flag Lan separation flag
  \return int32_t, The return value can be 0 or 1
 */
#ifdef NO_DOXY
extern uint32_t (*ppa_hook_get_lan_seperate_flag_fn)(uint32_t flag);
#else
extern uint32_t ppa_hook_get_lan_seperate_flag_fn(uint32_t flag);
#endif
/*!
  \brief This is to get the wan separation flag
  \param[in] flag Wan separation flag
  \return int32_t, The return value can be 0 or 1
 */
#ifdef NO_DOXY
extern uint32_t (*ppa_hook_get_wan_seperate_flag_fn)(uint32_t flag);
#else
extern uint32_t ppa_hook_get_wan_seperate_flag_fn(uint32_t flag);
#endif
#if defined(CONFIG_PPA_API_SW_FASTPATH)
/*!
  \brief This is to send skb through software fastpath
  \param[in] skb skb buffer
  \return int32_t, The return value can be 0 or 1
 */
#ifdef NO_DOXY
extern int32_t (*ppa_hook_sw_fastpath_send_fn)(struct sk_buff *skb);
#else
extern int32_t ppa_hook_sw_fastpath_send_fn(struct sk_buff *skb);
#endif
/*!
  \brief This is to enable software fastpath
  \param[in] flags for software fp enable disable
  \return int32_t, The return value can be 0 or 1
 */
#ifdef NO_DOXY
extern int32_t (*ppa_hook_set_sw_fastpath_enable_fn)(uint32_t flags);
#else
extern int32_t ppa_hook_set_sw_fastpath_enable_fn(uint32_t *flags);
#endif
/*!
  \brief This is to get software fastpath status
  \param[in] flags for software fp enable disable
  \return int32_t, The return value can be 0 or 1
 */
#ifdef NO_DOXY
extern int32_t (*ppa_hook_get_sw_fastpath_status_fn)(uint32_t flags);
#else
extern int32_t ppa_hook_get_sw_fastpath_status_fn(uint32_t *flags);
#endif
#endif
#if defined(CONFIG_L2NAT_MODULE) || defined(CONFIG_L2NAT)
/*!
  \brief Check if network interafce is a part of l2nat
  \param[in] netif Pointer to the network interface structure in the protocol stack. For eg. pointer to a struct net_device
  \param[in] ifname Interface name
  \param[in] flags  Reserved for future use
  \return The return value can be any one of the following:  \n
  - 1 if network interafce is a part of l2nat \n
  - 0 otherwise
 */
#ifdef NO_DOXY
extern int32_t (*ppa_check_if_netif_l2nat_fn)(PPA_NETIF *netif, char *ifname, uint32_t flags);
#else
extern int32_t ppa_check_if_netif_l2nat_fn(PPA_NETIF *netif, char *ifname, uint32_t flags);
#endif
#endif /* CONFIG_L2NAT*/
#if IS_ENABLED(CONFIG_MXL_VPN)
/*!
 \brief PPA based VPN acceleration learning hook to be called by VPNA
 \param[in] : sk_buff ingress/egress netdevice and the tunnel id
 \return : 0 if the session learning call succeeded
	-1 if the session learning call failed
 **************************************************************************************************/
extern int32_t (*ppa_vpn_ig_lrn_hook)(struct sk_buff *, struct net_device *, uint16_t);
extern int32_t (*ppa_vpn_eg_lrn_hook)(struct sk_buff *, struct net_device *, uint16_t);
extern int32_t (*ppa_vpn_tunn_del_hook)(uint16_t tunnel_id);
#endif /*IS_ENABLED(CONFIG_MXL_VPN)*/
/*!
  \brief Update packet device qos priority from peripheral driver
  \param[in] netif Linux network device pointer
  \param[in] subif Datapath sub-interface pointer
  \param[in,out] skb Linux packet buffer pointer, where skb->priority to be updated
  \return 0 on success, <0 otherwise
 */
extern int32_t (*ppa_update_pkt_devqos_priority_hook)(
	struct net_device *netif,
	struct dp_subif *subif,
	struct sk_buff *skb);
#endif /*end of __KERNEL__*/
/* @} */
#endif  /*  __PPA_HOOK_H__ */
