/******************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2019 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 * This file holds definitions used for multicast control handling.
 */

#ifndef _PON_NET_VLAN_FLOW_H_
#define _PON_NET_VLAN_FLOW_H_

#include <stdbool.h>

#include <omci/pon_adapter_vlan_flow.h>

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/** \defgroup PON_NET_VLAN_FLOW PON Adapter VLAN Flow
 *  This covers network functions used for VLAN Flow.
 *
 * @{
 */

/* Maximum supported number of TCIs */
#define MAX_TCIS 12

/* Forward declaration */
extern const struct pa_vlan_flow_ops pon_net_vlan_flow_ops;

/* An entry in the struct pon_net_vlan_flow_list */
struct vlan_flow_entry {
	/* Next and previous element */
	SLIST_ENTRY(vlan_flow_entry) entries;
	/* MAC Bridge Port Configuration Data ME ID */
	uint16_t me_id;
	/* VLAN flow */
	struct vlan_flow *flow;
	/* TC filter handle */
	uint32_t handle;
	/* Flag used to mark entries for deletion */
	bool marked_for_deletion;
};

SLIST_HEAD(pon_net_vlan_flow_list, vlan_flow_entry);

/**
 * Initialize VLAN flow list
 *
 * \param[in]  list        List of Managed Entity data
 */
void pon_net_vlan_flow_list_init(struct pon_net_vlan_flow_list *list);

/**
 * Attempt to remove all vlan flows and clean VLAN flow list
 *
 * \param[in]	ctx		PON Net Lib context
 * \param[in]	list		List of Managed Entity data
 */
void pon_net_vlan_flow_list_exit(struct pon_net_context *ctx,
				 struct pon_net_vlan_flow_list *list);

/**
 * Clear VLAN flow list by freeing all resources
 *
 * \param[in]	ctx		PON Net Lib context
 * \param[in]	me_id		VLAN tagging filter data ME id
 */
enum pon_adapter_errno
pon_net_vlan_flow_clear_all(struct pon_net_context *ctx,
			    uint16_t me_id);

/**
 *  Special function allowing creation of VLAN forwarding rules based on
 *  VLAN tagging filter data ME.
 *
 *  \param[in]	ctx		PON Net Lib context
 *  \param[in]	me_id		VLAN tagging filter data ME id
 *  \param[in]	tci_list	VLAN ids list
 *  \param[in]	tci_list_len	VLAN ids count
 *  \param[in]	tci_mask	VLAN TCI mask for all IDs
 *  \param[in]	untagged	Untagged frames should get forwarded
 */
enum pon_adapter_errno
pon_net_vlan_forwarding_update(struct pon_net_context *ctx,
			       uint16_t me_id,
			       const uint16_t *tci_list,
			       uint8_t tci_list_len,
			       uint16_t tci_mask,
			       bool untagged);

/** @} */ /* PON_NET_MCC_CORE */

/** @} */ /* PON_NET_LIB */

#endif
