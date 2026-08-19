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
 * GAL Ethernet Profile.
 */

#ifndef _pon_adapter_me_gal_ethernet_profile
#define _pon_adapter_me_gal_ethernet_profile

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_GAL_ETHERNET_PROFILE GAL Ethernet Profile
 *
 *  This Managed Entity organizes data that describes the GTC adaptation layer
 *  processing functions of the ONU for Ethernet services. It is used with the
 *  GEM interworking termination point Managed Entity.
 *
 *  Instances of this Managed Entity are created and deleted on request of the
 *  OLT.
 *
 *  @{
 */

/** GAL Ethernet Profile operations structure */
struct pa_gal_eth_profile_ops {
	/** Update GAL Ethernet Profile resources
	 *
	 *  \remark Enable Managed Entity identifier mapping to driver index
	 *          and initialize corresponding driver structures when it is
	 *          called first time for the given Managed Entity ID.
	 *
	 *  \param[in] ll_handle            Lower layer context pointer
	 *  \param[in] me_id                Managed Entity identifier
	 *  \param[in] gem_payload_size_max Maximum GEM payload size
	 *  \param[in] gem_port_id	   GEM Port ID
	 */
	enum pon_adapter_errno (*update)(void *ll_handle,
					 uint16_t me_id,
					 uint16_t gem_payload_size_max,
					 uint16_t gem_port_id);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures.
	 *
	 *  \param[in] ll_handle            Lower layer context pointer
	 *  \param[in] me_id                Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle,
					  uint16_t me_id);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
