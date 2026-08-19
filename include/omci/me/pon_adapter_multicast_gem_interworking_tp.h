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
 * Multicast GEM Interworking Termination Point (TP).
 */

#ifndef _pon_adapter_me_multicast_gem_interworking_tp
#define _pon_adapter_me_multicast_gem_interworking_tp

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_MULTICAST_GEM_INTERWORKING_TP\
 *	      Multicast GEM Interworking Termination Port
 *
 *  An instance of this Managed Entity represents a point in the ONU where a
 *  multicast service interworks with the GEM layer. At this point, a multicast
 *  bit stream is reconstructed from GEM packets.
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/** Multicast GEM Interworking TP Managed Entity operations structure */
struct pa_mc_gem_itp_ops {
	/** Update Multicast GEM Interworking TP Managed Entity resources
	 *
	 *  \remark Enable the Managed Entity identifier mapping to a driver
	 *   index and initialize the corresponding driver structure when it is
	 *   called for the first time for given Managed Entity ID.
	 *
	 *  \param[in] ll_handle      Lower layer context pointer
	 *  \param[in] me_id          Managed Entity identifier
	 *  \param[in] ctp_ptr        CTP pointer
	 */
	enum pon_adapter_errno (*update)(void *ll_handle,
					 uint16_t me_id,
					 uint16_t ctp_ptr);

	/** Disable the Managed Entity identifier mapping to a driver index and
	 *  clear the corresponding driver structures.
	 *
	 *  \param[in] ll_handle      Lower layer context pointer
	 *  \param[in] me_id          Managed Entity identifier
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
