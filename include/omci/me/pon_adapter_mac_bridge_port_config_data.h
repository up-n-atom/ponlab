/******************************************************************************
 *
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * MAC Bridge Port Configuration Data.
 */

#ifndef _pon_adapter_me_mac_bridge_port_config_data
#define _pon_adapter_me_mac_bridge_port_config_data

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_MAC_BRIDGE_PORT_CONFIGURATION_DATA\
 *	      MAC Bridge Port Configuration Data
 *
 *  This Managed Entity models a port on a MAC bridge.
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/** Termination point type is a PPTP Ethernet UNI */
#define PA_BP_TP_TYPE_PPTP_UNI	1
/** Termination point type is a IEEE 802.1p mapper */
#define PA_BP_TP_TYPE_PMAP	3
/** Termination point type is an IP host or IPv6 host */
#define PA_BP_TP_TYPE_IP_HOST	4
/** Termination point type is a GEM port */
#define PA_BP_TP_TYPE_GEM	5
/** Termination point type is a multicast GEM port */
#define PA_BP_TP_TYPE_MC_GEM	6
/** Termination point type is a PPTP XDSL UNI */
#define PA_BP_TP_TYPE_XDSL	7
/** Termination point type is a PPTP VDSL UNI */
#define PA_BP_TP_TYPE_VDSL	8
/** Termination point type is a Virtual Ethernet Interface Point */
#define PA_BP_TP_TYPE_VEIP	11
/** Maximum number of bridge ports that can be connected to the bridge */
#define PA_MAC_BP_CONFIG_DATA_MAX	128

#define PA_BRIDGE_DATA_PORT 16

/** This structure contains bridge port configuration information */
struct pa_bridge_port_data {
	/** Managed Entity ID */
	uint16_t me_id;
	/** Termination point type identifier */
	uint8_t tp_type;
	/** Termination point pointer */
	uint16_t tp_ptr;
};

/** This structure contains bridge configuration information */
struct pa_bridge_data {
	/** Managed Entity ID */
	uint16_t me_id;
	/** List of bridge ports connected to the bridge */
	struct pa_bridge_port_data port[PA_BRIDGE_DATA_PORT];
	/** Number of bridge ports connected to the bridge */
	uint16_t count;
};

/** MAC Bridge Port Configuration Data update structure */
struct pa_mac_bp_config_data_upd_data {
	/** Bridge ID pointer */
	uint16_t bridge_id_ptr;
	/** Bridge port number */
	uint8_t port_num;
	/** Termination Point type */
	uint8_t tp_type;
	/** Termination Point pointer */
	uint16_t tp_ptr;
	/** Outbound Traffic Descriptor pointer */
	uint16_t outbound_td_ptr;
	/** Inbound Traffic Descriptor pointer */
	uint16_t inbound_td_ptr;
	/** MAC learning depth */
	uint8_t mac_learning_depth;
	/** IPv4 Multicast Control Enable/Disable flag */
	uint8_t mcc_ipv4_enabled;
	/** IPv6 Multicast Control Enable/Disable flag */
	uint8_t mcc_ipv6_enabled;
};

/** MAC Bridge Port Configuration Data destroy structure */
struct pa_mac_bp_config_data_destroy_data {
	/** Termination point type */
	uint8_t tp_type;
	/** Termination point pointer */
	uint16_t tp_ptr;
	/** Outbound Traffic Descriptor pointer */
	uint16_t outbound_td_ptr;
	/** Inbound Traffic Descriptor pointer */
	uint16_t inbound_td_ptr;
};

/** MAC Bridge Port Configuration Data operations structure */
struct pa_mac_bp_config_data_ops {
	/** Update MAC Bridge Port(bp) Configuration Data Managed Entity
	 *  resources
	 *  \remark Enable Managed Entity identifier mapping to driver index
	 *  and initialize corresponding driver structures, when it is called
	 *  first time for the given Managed Entity ID
	 *
	 *  \param[in] ll_handle          Lower layer context pointer
	 *  \param[in] me_id              Managed Entity identifier
	 *  \param[in] update_data        Bridge port config data update
	 *                                structure
	 */
	enum pon_adapter_errno (*update)(void *ll_handle, const uint16_t me_id,
		const struct pa_mac_bp_config_data_upd_data *update_data);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 *  \param[in] ll_handle          Lower layer context pointer
	 *  \param[in] me_id              Managed Entity identifier
	 *  \param[in] tp_type            Termination Point type
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle, uint16_t me_id,
		const struct pa_mac_bp_config_data_destroy_data *destroy_data);

	/** Connection between bridge port and specified tp type.
	 *
	 *  \remarks Types supported can be pmapper, pptp_eth_uni,
	 *	     pptp_xdsl_uni, veip and ip_host
	 *  \param[in] bridge_me_id       Managed Entity identifier of
	 *				  the bridge
	 *  \param[in] bp_me_id		  Managed Entity identifier
	 *				  of the bridge port
	 *  \param[in] tp_type            Termination Point type
	 *  \param[in] tp_prt             Termination Point pointer or
	 *                                Managed Entity ID
	 */
	enum pon_adapter_errno (*connect)(void *ll_handle,
		uint16_t bridge_me_id, uint16_t bp_me_id,
		uint8_t tp_type, uint16_t tp_ptr);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
