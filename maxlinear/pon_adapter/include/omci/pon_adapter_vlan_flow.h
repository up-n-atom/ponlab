/******************************************************************************
 *
 *  Copyright (c) 2022 MaxLinear, Inc.
 *  Copyright (c) 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI header file for the VLAN Flow.
 */

#ifndef _pon_adapter_vlan_flow_data
#define _pon_adapter_vlan_flow_data

#if !defined(__PA_RUST__)
#include <net/if.h>
#endif

#include "pon_adapter_base.h"
#include "pon_adapter_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_VLAN_FLOW VLAN Flow
 *
 *  These function and structures are used to add and delete VLAN flow.
 *
 *  @{
 */

struct vlan_flow {
	/** Priority for VLAN flow. Recommended range 63000-64000 */
	uint16_t prio;
	/** Ingress netdev */
	char igdev[IF_NAMESIZE];
	/** Egress netdev */
	char egdev[IF_NAMESIZE];
	/* VLAN TCI */
	uint16_t tci;
	/* VLAN TCI mask */
	uint16_t tci_mask;
	/* VLAN TPID */
	uint16_t tpid;
	/* Match all - ignore VLAN TPID */
	bool untagged;
	/* Mirred drop action */
	bool drop;
};

/** VLAN Flow operations structure */
struct pa_vlan_flow_ops {
	/** Create VLAN upstream forwarding rule.
	 *
	 *  \param[in] ll_handle	Lower layer context pointer
	 *  \param[in] flow		VLAN forwarding flow
	 */
	enum pon_adapter_errno (*flow_add)(void *ll_handle,
					   struct vlan_flow *flow);

	/** Delete VLAN upstream forwarding rule.
	 *
	 *  \param[in] ll_handle	Lower layer context pointer
	 *  \param[in] flow		VLAN forwarding flow
	 */
	enum pon_adapter_errno (*flow_del)(void *ll_handle,
					   struct vlan_flow *flow);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif
#endif
