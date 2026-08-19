/*****************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file
 * This file holds definitions used for the handling of VLAN tags.
 */

#ifndef _PON_NET_EXT_VLAN_FILTER_H_
#define _PON_NET_EXT_VLAN_FILTER_H_

#include "pon_net_common.h"
#include "pon_net_netlink.h"
#include "omci/me/pon_adapter_ext_vlan.h"

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/** \defgroup PON_NET_EXT_VLAN PON Network Library Extended VLAN Functions
 *  These functions are used to configure Extended VLAN translation functions.
 *  @{
 */

/** Extended VLAN handling filter definition */
struct netlink_filter_definition {
	/** Filter */
	struct netlink_filter filter;
	/** VLAN data */
	struct netlink_vlan_data vlan_data;
	/** Flag to indicate, if the filter is defined */
	uint8_t is_def;
};

/**
 * Structure storing the relevant attributes of the
 * Extended VLAN Configuration Data Managed Entity
 */
struct pon_net_ext_vlan {
	/** PON Net Context*/
	struct pon_net_context *ctx;
	/** Input TPID attribute value */
	uint16_t input_tpid;
	/** Output TPID attribute value */
	uint16_t output_tpid;
	/** Downstream Mode attribute value*/
	uint16_t ds_mode;
	/**
	 * The number representing the type of the Managed Entity
	 * associated with Extended VLAN Managed Entity instance
	 */
	uint16_t association_type;
	/**
	 * The number representing the ME ID of the Managed Entity
	 * associated with Extended VLAN Managed Entity instance
	 */
	uint16_t associated_ptr;

	/** Operations specific to the associated ME */
	const struct association_ops *association_ops;

	/** DSCP to PCP mapping */
	uint8_t dscp[DSCP_MAX];

	/** Rule collection not yet applied to the system */
	struct pon_net_ext_vlan_rules *pending;

	/** Rules cache for computing the difference on updates */
	struct pon_net_ext_vlan_rules *cache;

	/** Configure downstream discard filtering */
	bool ds_discard;
	/** Configure downstream forward filtering */
	bool ds_forward;
	/** Handles for downstream mode filters */
	uint32_t ds_handles[7];
	/** Priorities for downstream mode filters */
	uint32_t ds_discard_priorities[7];
	/** Priorities for downstream mode filters */
	uint32_t ds_forward_priorities[7];
	/** Interface name */
	char ifname[IF_NAMESIZE];
	/**
	 * Whether the Multicast TC filter replication is disabled -
	 * relevant for the UNI
	 */
	bool mc_replication_disabled;

	/*
	 * Multicast interface name, on which the filters are replicated -
	 * relevant for the UNI
	 */
	char mc_ifname[IF_NAMESIZE];
};

/**
 *  Read an Extended VLAN filter rule definition.
 *
 * \param[in] ext_vlan     Extended VLAN context
 * \param[in] filter       Extended VLAN filter rule
 * \param[in] ds           Direction indicator, 0 if upstream, 1 if downstream.
 * \param[in] rule_major   Major rule index, selects between untagged,
 *                         single-tagged, and double-tagged rules.
 * \param[in] rule_minor   Minor rule index, selects a rule within a major
 *                         group of rules.
 * \param[out] out         Rule definition
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
unsigned int
pon_net_ext_vlan_filters_get(struct pon_net_ext_vlan *ext_vlan,
			     const struct pon_adapter_ext_vlan_filter *filter,
			     int ds,
			     int rule_major,
			     int rule_minor,
			     struct netlink_filter_definition **out);

/**
 *  Find the rule index from an Extended VLAN definition for an upstream rule.
 *
 * \param[in]  filter  Extended VLAN filter definition
 * \param[out] major   Major rule index, selects between untagged,
 *                     single-tagged, and double-tagged rules.
 * \param[out] minor   Minor rule index, selects a rule within a major
 *                     group of rules.
 * \param[out] def     Indicates if this is a default rule (1) or not (0).
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
int
pon_net_get_rule_by_filter_us(const struct pon_adapter_ext_vlan_filter *filter,
			      int *major,
			      int *minor,
			      uint8_t *def);

/**
 *  Find the rule index from an Extended VLAN definition for a downstream rule.
 *
 * \param[in]  filter  Extended VLAN filter definition
 * \param[out] major   Major rule index, selects between untagged,
 *                     single-tagged, and double-tagged rules.
 * \param[out] minor   Minor rule index, selects a rule within a major
 *                     group of rules.
 * \param[out] def     Indicates if this is a default rule (1) or not (0).
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
int
pon_net_get_rule_by_filter_ds(const struct pon_adapter_ext_vlan_filter *filter,
			      int *major,
			      int *minor,
			      uint8_t *def);
struct filter_template;
/**
 * Builds a cookie from Ext. VLAN OMCI filter and filter template
 *
 * \param[out] cookie  Cookie
 * \param[in]  f  Extended VLAN rule
 * \param[in]  t  Filter template
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - PON_ADAPTER_ERR_MEM_ACCESS: If failed to copy memory
 */
enum pon_adapter_errno
pon_net_ext_vlan_make_cookie(struct netlink_cookie *cookie,
			     const struct pon_adapter_ext_vlan_filter *f,
			     const struct filter_template *t);


/** Read vlan which would enter the bridge after applying the rule
 *
 * \param[in]  f        Ext. VLAN rule
 * \param[out] vlan_id  VLAN ID
 *
 * \return true if there is any VLAN that would enter the bridge
 */
bool
pon_net_ext_vlan_bridge_port_vlan(const struct pon_adapter_ext_vlan_filter *f,
				  uint16_t *vlan_id);

/** @} */ /* PON_NET_EXT_VLAN */

/** @} */ /* PON_NET_LIB */

#endif
