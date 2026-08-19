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
 * MAC Bridge Service Profile.
 */

#ifndef _pon_adapter_me_mac_bridge_service_profile
#define _pon_adapter_me_mac_bridge_service_profile

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_MAC_BRIDGE_SERVICE_PROFILE\
 *	MAC Bridge Service Profile
 *
 *  This Managed Entity models a MAC bridge in its entirety. Any number of ports
 *  may then be associated with the bridge through pointers to the MAC bridge
 *  service profile Managed Entity.
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/** MAC Bridge Service Profile update structure */
struct pa_mac_bridge_service_profile_upd_data {
	/** MAC learning ind */
	uint8_t learning_ind;
	/** Port bridging ind */
	uint8_t port_bridging_ind;
	/** Unknown MAC address discard */
	uint8_t unknown_mac_discard;
	/** MAC learning depth */
	uint8_t mac_learning_depth;
	/** MAC aging time */
	uint32_t mac_aging_time;
};

/** MAC Bridge Service Profile operations structure */
struct pa_mac_bridge_service_profile_ops {
	/** Initialize MAC Bridge Service Profile Managed Entity resources.
	 *
	 *  \remark Enable Managed Entity identifier mapping to driver index and
	 *  initialize corresponding driver structures when it is called first
	 *  time for the given Managed Entity ID
	 *
	 *  \param[in] ll_handle           Lower layer context pointer
	 *  \param[in] me_id               Managed Entity identifier
	 */
	enum pon_adapter_errno (*init)(void *ll_handle,
				       const uint16_t me_id);

	/** Update MAC Bridge Service Profile Managed Entity resources.
	 *
	 * \param[in] ll_handle           Lower layer context pointer
	 * \param[in] me_id               Managed Entity identifier
	 * \param[in] update_data         Bridge service profile update data
	 */
	enum pon_adapter_errno (*update)(void *ll_handle, const uint16_t me_id,
		const struct pa_mac_bridge_service_profile_upd_data
			*update_data);

	/** Disable Managed Entity identifier mapping to driver index
	 *  and clear corresponding driver structures
	 *
	 *  \param[in] ll_handle           Lower layer context pointer
	 *  \param[in] me_id               Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle,
					  const uint16_t me_id);

	/** Retrieve the number of connected ports
	 *
	 *  \param[in] ll_handle          Lower layer context pointer
	 *  \param[in] me_id              Managed Entity identifier
	 *  \param[out] port_count        Bridge port count
	 */
	enum pon_adapter_errno (*port_count_get)(void *ll_handle,
						 const uint16_t me_id,
						 uint8_t *port_count);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
