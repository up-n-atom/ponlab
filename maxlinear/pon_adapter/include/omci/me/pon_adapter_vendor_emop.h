/******************************************************************************
 *
 *  Copyright (c) 2020 MaxLinear, Inc.
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
 * Extended Multicast Operations Profile (vendor specific).
 */

#ifndef _pon_adapter_me_vendor_emop_h
#define _pon_adapter_me_vendor_emop_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_EMOP Extended Multicast Operations Profile
 *
 *  This Managed Entity configures the VLAN handling of downstream multicast
 *  packets. Once this Managed Entity is present, the downstream VLAN tag
 *  handling attribute "Downstream IGMP and Multicast TCI" of the regular
 *  "Multicast Operations Profile" shall be ignored (if present).
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/** Extended Multicast Operations Profile Managed Entity Class Identifier*/
#define PA_ME_VENDOR_EMOP_ME_ID		65282

/** Maximum number of entries in the VLAN translation table attribute. */
#define PA_VENDOR_EMOP_TRANS_TABLE_ENTRIES_MAX      6

/** No VLAN translation */
#define PA_VENDOR_EMOP_TRANS_NO		0
/** Strip all VLANs (send untagged packets to the UNI) */
#define PA_VENDOR_EMOP_TRANS_STRIP	1
/**  Translate the outer VLAN ID (keep the priority and DE) */
#define PA_VENDOR_EMOP_TRANS_REPLACE	2

/** Structure to define VLAN translation table entry. */
struct pa_vendor_emop_vlan_trans_table {
	/** Filter VLAN ID value to match the original VLAN ID. */
	uint16_t flt_vid;
	/** Treatment VLAN ID value that shall be used for replacement. */
	uint16_t trt_vid;
};

/** Extended Multicast Operations Profile operations */
struct pon_adapter_vendor_emop_ops {
	/** Set MC DS VLAN translation
	 *
	 * \param[in]  ll_handle     Lower layer context pointer
	 * \param[in]  me_id         MC Subscriber Config Managed Entity
	 *                           identifier
	 * \param[in]  lan_idx       LAN port index
	 * \param[in]  control_mode  Control Mode
	 * \param[in]  vlans_num     Number of VLAN table entries
	 * \param[in]  vlan_table    VLAN table
	 */
	enum pon_adapter_errno (*mc_ds_ext_vlan_set)(
		void *ll_handle,
		const uint16_t me_id,
		const uint16_t lan_idx,
		const uint8_t control_mode,
		const uint8_t vlans_num,
		const struct pa_vendor_emop_vlan_trans_table *vlan_table);

	/** Clear MC DS VLAN translation
	 *
	 * \param[in] ll_handle  Lower layer context pointer
	 * \param[in] me_id      MC Subscriber Config Managed Entity identifier
	 * \param[in] lan_idx    LAN port index
	 */
	enum pon_adapter_errno (*mc_ds_ext_vlan_clear)(void *ll_handle,
						const uint16_t me_id,
						const uint16_t lan_idx);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
