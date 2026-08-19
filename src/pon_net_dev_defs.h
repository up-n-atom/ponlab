/*****************************************************************************
 *
 * Copyright (c) 2021 - 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file
 * Definition of specific kinds of network devices, which are stored in
 * pon_net_dev_db. We need different kinds of device definitions, because each
 * device can have different requirements for handle and priority generations
 */

#ifndef _PON_NET_DEV_DB_DEFS_H_
#define _PON_NET_DEV_DB_DEFS_H_

/** \addtogroup PON_NET_LIB PON Network Library Functions
* @{
*/

/**
 * Types of handle and priority pools
 *
 * There are just types - the actual ranges are device dependent and are
 * defined by pon_net_dev_def definitions.
 */
enum pon_net_pool_type {
	/** Handle pool for TC filters in ingress direction */
	PON_NET_HANDLE_FILTER_INGRESS = 1,
	/** Handle pool for TC filters in egress direction */
	PON_NET_HANDLE_FILTER_EGRESS,
	/** Handle pool for IPHOST vlan trap filters */
	PON_NET_HANDLE_IPHOST_VLAN_TRAP,
	/** Handle pool for color marking TC filters in ingress direction */
	PON_NET_HANDLE_COLOR_MARKING_INGRESS,
	/** Handle pool for TC filters with action police in ingress direction
	 */
	PON_NET_HANDLE_POLICE_INGRESS,
	/** Handle pool for TC filters with action police in egress direction */
	PON_NET_HANDLE_POLICE_EGRESS,
	/** MCC trap handles */
	PON_NET_HANDLE_MCC_TRAP_INGRESS,
	/** Handles for filters on LCT */
	PON_NET_HANDLE_LCT_INGRESS,
	/** MCC drop filter handles */
	PON_NET_HANDLE_MCC_DROP_INGRESS,
	/** Priority pool for Extended VLAN ingress TC filters */
	PON_NET_PRIO_EXT_VLAN_INGRESS,
	/** Priority pool for Extended VLAN downstream TC filters */
	PON_NET_PRIO_EXT_VLAN_EGRESS,
	/** Priority pool for Extended VLAN upstream default TC filters */
	PON_NET_PRIO_EXT_VLAN_INGRESS_DEFAULT,
	/** Priority pool for Extended VLAN downstream default TC filters */
	PON_NET_PRIO_EXT_VLAN_EGRESS_DEFAULT,
	/** Priority pool for ext vlan forward filters in egress */
	PON_NET_PRIO_EXT_VLAN_EGRESS_FORWARD,
	/** Priority pool for ext vlan forward filters in ingress*/
	PON_NET_PRIO_EXT_VLAN_INGRESS_FORWARD,
	/** Priority pool for ext vlan discard filters in egress */
	PON_NET_PRIO_EXT_VLAN_EGRESS_DISCARD,
	/** Priority pool for ext vlan discard filters in ingress*/
	PON_NET_PRIO_EXT_VLAN_INGRESS_DISCARD,
	/** Priority pool for IGMP trap filters */
	PON_NET_PRIO_IGMP_TRAP,
	/** Priority pool for OMCI trap */
	PON_NET_PRIO_OMCI_TRAP_INGRESS,
	/** Priority pool for IP Host MAC trap filters */
	PON_NET_PRIO_IPHOST_MAC_TRAP_INGRESS,
	/** Priority pool for IP Host VLAN trap filters */
	PON_NET_PRIO_IPHOST_VLAN_TRAP,
	/** Priority pool for color marking filters */
	PON_NET_PRIO_COLOR_MARKING_INGRESS,
	/** Priority pool for filters with TC action police */
	PON_NET_PRIO_POLICE_INGRESS,
	/** Priority pool for filters with TC action police */
	PON_NET_PRIO_POLICE_EGRESS,
	/** Priority pool for multicast VLAN operations in upstream */
	PON_NET_PRIO_MC_INGRESS,
	/** Priority pool for multicast VLAN operations in downstream */
	PON_NET_PRIO_MC_EGRESS,
	/** Multicast flow forwarding */
	PON_NET_PRIO_MC_FLOW_FORWARD_INGRESS,
	/** MAC Bridge Port Preassign Table filters */
	PON_NET_PRIO_PREASSIGN_INGRESS,
	/** MAC Bridge Port Preassign Table filters + VLAN */
	PON_NET_PRIO_PREASSIGN_VLAN_INGRESS,
	/** VLAN flow forwarding: VLAN + anything */
	PON_NET_PRIO_VLAN_FLOW_INGRESS_TCI,
	/** VLAN flow forwarding: VLAN ID only */
	PON_NET_PRIO_VLAN_FLOW_INGRESS_VID,
	/** VLAN flow forwarding: Any VLAN ID */
	PON_NET_PRIO_VLAN_FLOW_INGRESS_ANY_VLAN,
	/** VLAN flow forwarding: Any packet */
	PON_NET_PRIO_VLAN_FLOW_INGRESS_ANY,
	/** VLAN flow forwarding: all */
	PON_NET_PRIO_VLAN_FLOW_INGRESS_ALL,
	/**
	 * Vlan flow forwarding ingress priorities reserved for
	 * PON Adapter pa_vlan_flow_ops API
	 */
	PON_NET_PRIO_VLAN_FLOW_INGRESS_RESERVED,
	/**
	 * Vlan flow forwarding ingress priorities reserved for
	 * PON Adapter pa_vlan_flow_ops API
	 */
	PON_NET_PRIO_VLAN_FLOW_EGRESS_RESERVED,
	/** Vlan filtering on ingress */
	PON_NET_PRIO_VLAN_FILTERING_INGRESS,
	/** Vlan filtering on ingress (default rules) */
	PON_NET_PRIO_VLAN_FILTERING_INGRESS_DEFAULT,
	/** Vlan filtering on egress */
	PON_NET_PRIO_VLAN_FILTERING_EGRESS,
	/** Vlan filtering on egress (default rules) */
	PON_NET_PRIO_VLAN_FILTERING_EGRESS_DEFAULT,
	/** GEM to queue assignment filters */
	PON_NET_PRIO_GEM_TO_QUEUE_ASSIGNMENT,
	/** TC to queue assignment filters */
	PON_NET_PRIO_TC_TO_QUEUE_ASSIGNMENT,
	/** MCC trap priorities */
	PON_NET_PRIO_MCC_TRAP_INGRESS,
	/** LCT filter priorities */
	PON_NET_PRIO_LCT_INGRESS,
	/** MCC drop filter priorities */
	PON_NET_PRIO_MCC_DROP_INGRESS,
};

/** Device definition for the GEM port */
extern const struct pon_net_dev_def pon_net_gem_def;
/** Device definition for the PMAPPER */
extern const struct pon_net_dev_def pon_net_pmapper_def;
/** Device definition for the PON port */
extern const struct pon_net_dev_def pon_net_pon_def;
/** Device definition for the T-CONT */
extern const struct pon_net_dev_def pon_net_tcont_def;
/** Device definition for the UNI broadcast interface */
extern const struct pon_net_dev_def pon_net_uni_bc_def;
/** Device definition for the UNI interface */
extern const struct pon_net_dev_def pon_net_uni_def;
/** Device definition for the UNI multicast interface */
extern const struct pon_net_dev_def pon_net_uni_mc_def;
/** Device definition for the IP Host interface */
extern const struct pon_net_dev_def pon_net_iphost_def;

/** @} */ /* PON_NET_LIB */

#endif
