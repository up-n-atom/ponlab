/******************************************************************************
 *
 *  Copyright (c) 2020 - 2022 MaxLinear, Inc.
 *  Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * Extended VLAN Tagging Operation Configuration Data (EVTOCD).
 */

#ifndef _pon_adapter_me_ext_vlan
#define _pon_adapter_me_ext_vlan

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *  @{
 */

/** \defgroup PON_ADAPTER_EXTENDED_VLAN Extended VLAN Tagging Operation
 *  Configuration Data
 *  These structures are used to handle the extended VLAN functionality.
 *  @{
 */

/** This enumerator holds the values that identify the type
 *  of the Managed Entity associated with
 *  the Extended VLAN tagging Operation Configuration (EVTOCD) Managed Entity
 *  - 0: MAC bridge port configuration data
 *  - 1: IEEE 802.1p mapper service profile
 *  - 2: Physical path termination point Ethernet UNI
 *  - 3: IP host config data or IPv6 host config data
 *  - 4: Physical path termination point xDSL UNI
 *  - 5: GEM interworking termination point
 *  - 6: Multicast GEM interworking termination point
 *  - 7: Physical path termination point MoCA UNI
 *  - 8: Reserved (Deprecated PPTP 802.11 UNI)
 *  - 9: Ethernet flow termination point
 *  - 10: Virtual Ethernet interface point
 *  - 11: MPLS pseudowire termination point
 *  - 12: EFM bonding group
 */
enum pon_adapter_ext_vlan_associated_me {
	/** EVTOCD is related to a MAC Bridge Port */
	PA_EXT_VLAN_MAC_BP_CFG_DATA = 0,
	/** EVTOCD is related to an IEEE 802.1p mapper */
	PA_EXT_VLAN_IEEE_P_MAPPER = 1,
	/** EVTOCD is related to a PPTP Ethernet UNI */
	PA_EXT_VLAN_PPTP_ETH_UNI = 2,
	/** EVTOCD is related to an IP Host or IPv6 Host  */
	PA_EXT_VLAN_IP_HOST_CONFIG_DATA = 3,
	/** EVTOCD is related to a PPTP XDSL UNI (not supported) */
	PA_EXT_VLAN_PPTP_XDSL_UNI = 4,
	/** EVTOCD is related to a GEM ITP */
	PA_EXT_VLAN_GEM_ITP = 5,
	/** EVTOCD is related to a multicast GEM ITP */
	PA_EXT_VLAN_MC_GEM_ITP = 6,
	/** EVTOCD is related to a PPTP MOCA UNI (not supported) */
	PA_EXT_VLAN_PPTP_MOCA_UNI = 7,
	/** This code point is reserved (deprecated function). */
	PA_EXT_VLAN_PPTP_802_11_UNI = 8,
	/** EVTOCD is related to an Ethernet Flow (not supported) */
	PA_EXT_VLAN_ETH_FLOW_TP = 9,
	/** EVTOCD is related to a Virtual Ethernet Interface Point */
	PA_EXT_VLAN_VEIP = 10,
	/** EVTOCD is related to a Pseudowire TP (not supported) */
	PA_EXT_VLAN_MPLS_PSEUDOWIRE_TP = 11,
	/** EVTOCD is related to an EFM Bonding Group (not supported) */
	PA_EXT_VLAN_EFM_BONDING_GROUP = 12
};

/** Extended VLAN filter definition. */
struct pon_adapter_ext_vlan_filter {
	/** Outer VLAN priority filter. */
	int filter_outer_priority;
	/** Outer VLAN ID filter. */
	int filter_outer_vid;
	/** Outer VLAN TPID filter. */
	int filter_outer_tpid_de;
	/** Inner VLAN priority filter. */
	int filter_inner_priority;
	/** Inner VLAN ID filter. */
	int filter_inner_vid;
	/** Inner VLAN TPID filter. */
	int filter_inner_tpid_de;
	/** Ethertype filter. */
	int filter_ethertype;
	/** Number of VLAN tags to remove. */
	int treatment_tags_to_remove;
	/** Outer VLAN priority treatment. */
	int treatment_outer_priority;
	/** Outer VLAN ID treatment. */
	int treatment_outer_vid;
	/** Outer VLAN TPID and DE bit treatment. */
	int treatment_outer_tpid_de;
	/** Inner VLAN priority treatment. */
	int treatment_inner_priority;
	/** Inner VLAN ID treatment. */
	int treatment_inner_vid;
	/** Inner VLAN TPID and DE bit treatment. */
	int treatment_inner_tpid_de;
};

/** Extended VLAN update structure */
struct pon_adapter_ext_vlan_update {
	/** Association type */
	uint8_t association_type;
	/** Pointer to the associated Managed Entity */
	uint16_t associated_ptr;
	/** Input TPID */
	uint16_t input_tpid;
	/** Output TPID */
	uint16_t output_tpid;
	/** Pointer to the DSCP mapping table */
	uint8_t *dscp;
	/** Downstream Mode
	 *
	 *  Regardless of its association, the extended VLAN tagging operation
	 *  configuration data Managed Entity pertains to upstream traffic.
	 *  This attribute is readable and writable by the OLT.
	 *  This attribute specifies the mode for downstream mapping:
	 *
	 *  - 0x00: The operation performed in the downstream direction is the
	 *          inverse of that performed in the upstream direction.
	 *  - 0x01: No operation is performed in the downstream direction.
	 *  - 0x02: Filter on VID and p-bit or pass.
	 *  - 0x03: Filter on VID only or pass.
	 *  - 0x04: Filter on p-bit only or pass.
	 *  - 0x05: Filter on VID and p-bit or drop.
	 *  - 0x06: Filter on VID only or drop.
	 *  - 0x07: Filter on p-bit only or drop.
	 *  - 0x08: Drop all.
	 */
	uint8_t ds_mode;
};

/** Extended_vlan operations structure */
struct pa_ext_vlan_ops {
	/** Add ExtVLAN action (rule & treatment)
	 *
	 *  \param[in]  ll_handle      Lower layer context pointer
	 *  \param[in]  id             Managed Entity Class and Instance
	 *                             identifier
	 *                             (16 MSB - class, 16 LSB - instance)
	 *  \param[in] ds_mode         Downstream mode
	 *                             0: Enabled
	 *                             1: Disabled
	 *  \param[in] filter          Filter value
	 *  \param[in] entries_num     Number of entries in Received frame,
	 *                             VLAN tagging operation table
	 */
	enum pon_adapter_errno (*rules_add)(
			void *ll_handle,
			const uint16_t id,
			const uint8_t ds_mode,
			struct pon_adapter_ext_vlan_filter *filter,
			uint16_t entries_num);

	/** Enable/Disable common IP ExtVLAN handling - Treat IPV6 payloads,
	 *  like IPV4 under ExtVLAN.
	 *
	 *  \param[in] ll_handle  Lower layer context pointer
	 *  \param[in] enable     Enable/Disable flag
	 */
	enum pon_adapter_errno (*common_ip_handling_enable)(void *ll_handle,
							const uint8_t enable);

	/** Enable/Disable Ignoring DS Extended VLAN rules priority matching
	 *
	 *  \param[in] ll_handle  Lower layer context pointer
	 *  \param[in] enable     Enable/Disable flag
	 */
	enum pon_adapter_errno (*ignoring_ds_prio_enable)(void *ll_handle,
							  const bool enable);

	/** Remove existing entry from Received frame,
	 *  VLAN tagging operation table.
	 *
	 *  \param[in] ll_handle  Lower layer context pointer
	 *  \param[in] me_id      Managed Entity identifier
	 *  \param[in] filter     Extended VLAN filter
	 *  \param[in] ds_mode    Downstream mode
	 *
	 *  \remark
	 *  The first 7 fields of each entry are guaranteed to be unique,
	 *  and are used to identify table entries.
	 */
	enum pon_adapter_errno (*rule_remove)(void *ll_handle,
			      uint16_t me_id,
			      const struct pon_adapter_ext_vlan_filter *filter,
			      uint8_t ds_mode);

	/** Remove existing entry from Received frame,
	 *  VLAN tagging operation table.
	 *
	 *  \param[in] ll_handle  Lower layer context pointer
	 *  \param[in] me_id      Managed Entity identifier
	 *  \param[in] ds_mode    Downstream mode
	 *
	 *  \remark
	 *  The first 7 fields of each entry are guaranteed to be unique,
	 *  and are used to identify table entries.
	 */
	enum pon_adapter_errno (*rule_clear_all)(void *ll_handle,
					 uint16_t me_id,
					 uint8_t ds_mode);

	/** Update Extended VLAN Tagging Operation Configuration Data,
	 *  Managed Entity resources related to the association type,
	 *  0 (MAC bridge port configuration data).
	 *
	 *  \remark Enable Managed Entity identifier mapping to driver index and
	 *  initialize corresponding driver structures when,
	 *  it is called first time for the given Managed Entity ID.
	 *
	 *  \param[in] ll_handle    Lower layer context pointer
	 *  \param[in] update_data  Ext vlan data update
	 *  \param[in] me_id        Managed Entity identifier
	 */
	enum pon_adapter_errno (*update)(void *ll_handle,
			struct pon_adapter_ext_vlan_update *update_data,
			uint16_t me_id);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 *  \param[in] ll_handle  Lower layer context pointer
	 *  \param[in] me_id      Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle, uint16_t me_id);
};
/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
