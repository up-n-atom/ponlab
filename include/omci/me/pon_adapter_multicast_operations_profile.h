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
 * Multicast Operations Profile.
 */

#ifndef _pon_adapter_me_multicast_operations_profile_h
#define _pon_adapter_me_multicast_operations_profile_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_MULTICAST_OPERATIONS_PROFILE\
 *	      Multicast Operations Profile
 *
 *  This Managed Entity configures the multicast policy. A an ONU
 *  may have several such policies, which are linked to subscribers as required.
 *  Some of the attributes configure IGMP snooping and proxy parameters, in case
 *  the defaults do not suffice, as described in [b-IETF RFC 2236],
 *  [b-IETF RFC 3376], [b-IETF RFC 3810] and [b-IETF RFC 2933].
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/** Multicast Operations Profile Managed Entity Class Identifier*/
#define PON_ADAPTER_ME_MOP	309

/**
 * Pass the downstream IGMP/MLD and multicast traffic transparently, neither
 * stripping nor modifying tags that may be present.
 */
#define PA_MOP_DS_PASS 0

/**
 * Strip the outer VLAN tag (including P bits) from the downstream IGMP/MLD and
 * multicast traffic.
 */

#define PA_MOP_DS_STRIP 1

/**
 * Add a tag on to the downstream IGMP/MLD and multicast traffic. The new tag is
 * specified by the second and third bytes of this attribute.
 */
#define PA_MOP_DS_ADD 2

/**
 * Replace the tag on the downstream IGMP/MLD and multicast traffic. The new tag
 * is specified by the second and third bytes of this attribute.
 */
#define PA_MOP_DS_REPLACE 3

/**
 * Replace only the VLAN ID on the downstream IGMP/MLD and multicast traffic,
 * retaining the original DEI and P bits. The new VLAN ID is specified by the
 * VLAN ID field of the second and third bytes of this attribute.
 */
#define PA_MOP_DS_REPLACE_ONLY_VLAN 4

/**
 * Add a tag on to the downstream IGMP/MLD and multicast traffic. The new tag is
 * specified by the VID (UNI) field of the multicast service package table row
 * of the multicast subscriber config info ME that is associated with this
 * profile. If the VID (UNI) field is unspecified (0xFFFF) or specifies untagged
 * traffic, the new tag is specified by the second and third bytes of this
 * attribute.
 */
#define PA_MOP_DS_ADD_MSC 5

/**
 * Replace the tag on the downstream IGMP/MLD and multicast traffic. The new tag
 * is specified by the VID (UNI) field of the multicast service package table
 * row of the multicast subscriber config info ME that is associated with this
 * profile. If the VID (UNI) field specifies untagged traffic, the outer VLAN
 * tag (including P bits) is stripped from the downstream IGMP/MLD and multicast
 * traffic. If the value of the VID (UNI) is unspecified (0xFFFF), the new tag
 * is specified by the second and third bytes of this attribute.
 */
#define PA_MOP_DS_REPLACE_MSC 6

/**
 * Replace only the VID on the downstream IGMP/MLD and multicast traffic,
 * retaining the original DEI and P bits. The new VLAN ID is specified by the
 * VID (UNI) field of the multicast service package table row of the multicast
 * subscriber config info ME that is associated with this profile. If the VID
 * (UNI) field specifies untagged traffic, the outer VLAN tag (including P bits)
 * is stripped from the downstream IGMP/MLD and multicast traffic. If the value
 * of the VID (UNI) is unspecified (0xFFFF), the new tag is specified by the
 * second and third bytes of this attribute.
 */
#define PA_MOP_DS_REPLACE_ONLY_VLAN_MSC 7

/**
 * Pass upstream IGMP/MLD traffic transparently, neither adding, stripping nor
 * modifying tags that may be present.
 */
#define PA_MOP_US_PASS 0

/**
 * Add a VLAN tag (including P bits) to upstream IGMP/MLD traffic. The tag is
 * specified by the upstream IGMP TCI attribute.
 */
#define PA_MOP_US_ADD 1

/**
 * Replace the entire TCI (VLAN ID plus P bits) on upstream IGMP/MLD traffic.
 * The new tag is specified by the upstream IGMP/MLD TCI attribute. If the
 * received IGMP/MLD traffic is untagged, an add operation is performed.
 */
#define PA_MOP_US_REPLACE 2

/**
 * Replace only the VLAN ID on upstream IGMP/MLD traffic, retaining the original
 * DEI and P bits. The new VLAN ID is specified by the VLAN ID field of the
 * upstream IGMP TCI attribute. If the received IGMP/MLD traffic is untagged, an
 * add operation is performed, with DEI and P bits also taken from the upstream
 * IGMP TCI attribute.
 */
#define PA_MOP_US_REPLACE_ONLY_VLAN 3

/**
 * If the value of the VID (UNI) is unspecified (0xFFFF) in struct
 * pa_mc_profile_ext_vlan_data->msp_vid, then the new tag is specified by struct
 * pa_mc_profile_ext_vlan_update_data->ds_igmp_mc_tci
 */
#define PA_MOP_UNSPECIFIED_VID 0xFFFF

/** Multicast Operations Profile Extended VLAN update structure */
struct pa_mc_profile_ext_vlan_update_data {
	/** LAN port index (0 to 3) */
	uint16_t lan_idx;
	/** Upstream multicast VLAN tag control */
	uint8_t us_igmp_tag_ctrl;
	/** Upstream multicast TCI value */
	uint16_t us_igmp_tci;
	/** Downstream multicast VLAN tag control */
	uint8_t ds_igmp_mc_tag_ctrl;
	/** Downstream multicast TCI value */
	uint16_t ds_igmp_mc_tci;
	/** VID from Multicast subscriber package */
	uint16_t msp_vid;
};

/** Static Access Control List Table of Multicast Operations Profile add
 *  structure
 */
struct pa_mc_profile_static_acl_table_entry_add_data {
	/** Table index */
	uint16_t table_idx;
	/** GEM port ID */
	uint16_t gem_port_id;
	/** VLAN ID */
	uint16_t vlan_id;
	/** Source IP address */
	uint8_t src_ip_addr[4];
	/** Destination IP address of the start of the multicast range */
	uint8_t dest_ip_addr_mc_range_start[4];
	/** Destination IP address of the end of the multicast range */
	uint8_t dest_ip_addr_mc_range_end[4];
	/** Imputed group bandwidth */
	uint32_t imputed_group_bandwidth;
};

/** Multicast Operations Profile operations structure */
struct pa_mc_profile_ops {
	/** Initialize required Managed Entity resources.
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] me_id		Managed Entity identifier
	 * \param[in] igmp_version	OMCI IGMP version
	 */
	enum pon_adapter_errno (*create)(
		void *ll_handle,
		const uint16_t me_id,
		const uint8_t igmp_version);

	/** Destroy Managed Entity identifier resources.
	 *
	 * \param[in] ll_handle  Lower layer context pointer
	 * \param[in] me_id      Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(
		void *ll_handle,
		const uint16_t me_id);

	/** Update Multicast Operations Profiles(mop) Extended VLAN entries.
	 *
	 *  \param[in] ll_handle	    Lower layer context pointer
	 *  \param[in] me_id		    Managed Entity identifier
	 *  \param[in] update_data	    Multicast Operations Profile
	 *				    Extended VLAN update data
	 */
	enum pon_adapter_errno (*mc_ext_vlan_update)(
		void *ll_handle,
		const uint16_t me_id,
		const struct pa_mc_profile_ext_vlan_update_data *update_data);

	/** Clear Multicast Operations Profiles(mop) Extended VLAN entries
	 *
	 *  \param[in] ll_handle  Lower layer context pointer
	 *  \param[in] me_id	  Managed Entity identifier
	 *  \param[in] lan_idx	  LAN port index (0 to 3)
	 */
	enum pon_adapter_errno (*mc_ext_vlan_clear)(
		void *ll_handle,
		const uint16_t me_id,
		const uint16_t lan_idx);

	/** Add entry to Static Access Control List Table of Multicast
	 *  Operations Profile(mop) Managed Entity
	 *
	 *  \param[in] ll_handle		    Lower layer context pointer
	 *  \param[in] me_id			    Managed Entity identifier
	 *  \param[in] add_data			    Static Access Control List
	 *					    Table of Multicast
	 *					    Operations Profile add data
	 */
	enum pon_adapter_errno (*static_acl_table_entry_add)(
		void *ll_handle,
		uint16_t me_id,
		const struct pa_mc_profile_static_acl_table_entry_add_data
			*add_data);

	/** Remove entry from Static Access Control List Table of Multicast
	 *  Operations Profile(mop) Managed Entity
	 *
	 *  \param[in] ll_handle  Lower layer context pointer
	 *  \param[in] me_id	  Managed Entity identifier
	 *  \param[in] table_idx  Table index
	 */
	enum pon_adapter_errno (*static_acl_table_entry_rm)(
		void *ll_handle,
		uint16_t me_id,
		uint16_t table_idx);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
