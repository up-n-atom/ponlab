/******************************************************************************
 *
 * Copyright (c) 2020 MaxLinear, Inc.
 * Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * VLAN Tagging Operation Configuration Data.
 */

#ifndef _pon_adapter_me_vlan_tagging_operation_conf_data
#define _pon_adapter_me_vlan_tagging_operation_conf_data

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_VLAN_TAGGING_CONFIG_DATA\
 *	      VLAN Tagging Operation Configuration Data
 *
 *  This Managed Entity organizes data associated with VLAN tagging.
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/* When the optional association type attribute is 0 or undefined, this
 * attribute value is the same as the ID of the Managed Entity with which
 * this VLAN tagging operation configuration data instance is associated,
 * which may be either a PPTP Ethernet UNI or an IP host config data Managed
 * Entity. Otherwise, the value of the Managed Entity ID is unconstrained
 * except for the need to be unique.
 */

/** This enumerator holds the values for the Association Type attribute of
 *  VLAN Tagging Operation Configuration Data Managed Entity
 */
enum pa_vlan_tag_op_data {
	/** PPTP Ethernet UNI as default */
	PA_VLAN_TAG_OP_CFG_DATA_PPTP_ETH_UNI_DEFAULT = 0,
	/** IP Host or IPv6 Host */
	PA_VLAN_TAG_OP_CFG_DATA_IP_HOST_CONFIG_DATA = 1,
	/** IEEE 802.1p mapper */
	PA_VLAN_TAG_OP_CFG_DATA_P_MAPPER = 2,
	/** Bridge Port */
	PA_VLAN_TAG_OP_CFG_DATA_MAC_BP_CFG_DATA = 3,
	/** xDSL UNI (not supported) */
	PA_VLAN_TAG_OP_CFG_DATA_PPTP_XDSL_UNI = 4,
	/** GEM Port */
	PA_VLAN_TAG_OP_CFG_DATA_GEM_ITP = 5,
	/** Multicast GEM Port */
	PA_VLAN_TAG_OP_CFG_DATA_MC_GEM_ITP = 6,
	/** MOCA UNI (not supported) */
	PA_VLAN_TAG_OP_CFG_DATA_PPTP_MOCA_UNI = 7,
	/** Deprecated code point */
	PA_VLAN_TAG_OP_CFG_DATA_PPTP_802_11_UNI = 8,
	/** Ethernet flow (nor supported) */
	PA_VLAN_TAG_OP_CFG_DATA_ETH_FLOW_TP = 9,
	/** PPTP Ethernet UNI */
	PA_VLAN_TAG_OP_CFG_DATA_PPTP_ETH_UNI = 10,
	/** Virtual Ethernet Interface Point */
	PA_VLAN_TAG_OP_CFG_DATA_VEIP = 11,
	/** Pseudowire (not supported) */
	PA_VLAN_TAG_OP_CFG_DATA_MPLS_PSEUDOWIRE_TP = 12,
	/** EFM Bonding (not supported) */
	PA_VLAN_TAG_OP_CFG_DATA_EFM_BONDING_GROUP = 13
};

/** VLAN Tagging Operation Configuration Data update structure */
struct pa_vlan_tagging_operation_config_data_update_data {
	/** Upstream VLAN tagging operation mode */
	uint8_t us_vlan_tag_oper_mode;
	/** Upstream VLAN tag TCI value */
	uint16_t us_vlan_tag_tci_value;
	/** Downstream VLAN tagging operation mode */
	uint8_t ds_vlan_tag_oper_mode;
	/** Association type */
	uint8_t association_type;
	/** Associated Managed Entity pointer */
	uint16_t association_ptr;
};

/** VLAN Tagging Operation Configuration Data operations structure */
struct pa_vlan_tagging_operation_config_data_ops {
	/** Update VLAN Tagging Operation Configuration Data Managed Entity
	 *  resources
	 *
	 * \remark Enable Managed Entity identifier mapping to driver index and
	 *  initialize corresponding driver structures when it is called first
	 *  time for the given Managed Entity ID
	 *
	 *  \param[in] ll_handle             Lower layer context pointer
	 *  \param[in] me_id                 Managed Entity identifier
	 *  \param[in] update_data           VLAN Tagging Operation
	 *				     Configuration Data update data
	 */
	enum pon_adapter_errno (*update)(
		void *ll_handle,
		const uint16_t me_id,
		const struct pa_vlan_tagging_operation_config_data_update_data
			*update_data);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 * \param[in] ll_handle             Lower layer context pointer
	 * \param[in] me_id                 Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle,
					  const uint16_t me_id);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
