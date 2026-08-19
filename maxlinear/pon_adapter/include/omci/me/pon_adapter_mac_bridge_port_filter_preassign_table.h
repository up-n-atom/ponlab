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
 * MAC Bridge Port Filter Preassign Table.
 */

#ifndef _pon_adapter_me_mac_bridge_port_filter_preassign_table
#define _pon_adapter_me_mac_bridge_port_filter_preassign_table

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 * @{
 */

/** \defgroup PON_ADAPTER_ME_MAC_BRIDGE_PORT_FILTER_PREASSIGN_TABLE\
 *	      MAC Bridge Port Filter Preassign Table
 *
 * This Managed Entity provides an alternate approach to address filtering from
 * that supported through MAC bridge port filter table data. This alternate
 * approach is useful when all groups of addresses are stored beforehand in the
 * ONU, and this Managed Entity designates which groups are valid or invalid
 * for filtering. On a circuit pack in which all groups of addresses are
 * preassigned and stored locally, the ONU creates or deletes an instance of
 * this Managed Entity automatically upon creation or deletion of a MAC bridge
 * port configuration data Managed Entity.
 *
 * @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
   instance of the MAC bridge port configuration data Managed Entity. */

/** Supported values of parameters in functions from
 * \ref pa_mac_bp_filter_preassign_table_ops
 */
enum pa_mac_bp_filter_preassign_table_operation {
	/** Forward packets */
	PA_MAC_BP_FILTER_PREASSIGN_TABLE_OPERATION_FWD = 0,
	/** Filter packets */
	PA_MAC_BP_FILTER_PREASSIGN_TABLE_OPERATION_FILTER = 1
};

/** MAC Bridge Port Filter Preassign Table update structure */
struct pa_mac_bp_filter_preassign_table_upd_data {
	/** IPv4 multicast filtering */
	uint8_t ipv4_multicast_filtering;
	/** IPv6 multicast filtering */
	uint8_t ipv6_multicast_filtering;
	/** IPv4 broadcast filtering */
	uint8_t ipv4_broadcast_filtering;
	/** RARP filtering */
	uint8_t rarp_filtering;
	/** IPX filtering */
	uint8_t ipx_filtering;
	/** NetBEUI filtering */
	uint8_t net_beui_filtering;
	/** AppleTalk filtering */
	uint8_t apple_talk_filtering;
	/** Bridge management information filtering */
	uint8_t bridge_management_inf_filtering;
	/** ARP filtering */
	uint8_t arp_filtering;
	/** PPPoE filtering */
	uint8_t pppoe_filtering;
};

/** MAC Bridge Port Filter Preassign Table operations structure */
struct pa_mac_bp_filter_preassign_table_ops {
	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 * \param[in] ll_handle                 Lower layer context pointer
	 * \param[in] me_id                     Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(
		void *ll_handle,
		uint16_t me_id);

	/** Update MAC Bridge Port Filter Preassign Table Managed Entity
	 *  resources
	 *
	 * \param[in] ll_handle                      Lower layer context
	 *                                           pointer
	 * \param[in] me_id                          Managed Entity identifier
	 * \param[in] upd_data                       Pointer to update data
	 */
	enum pon_adapter_errno (*update)(
	      void *ll_handle,
	      uint16_t me_id,
	      const struct pa_mac_bp_filter_preassign_table_upd_data *upd_data);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
