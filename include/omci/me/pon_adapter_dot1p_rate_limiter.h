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
 * IEEE 802.1 rate limiter.
 */

#ifndef _pon_adapter_me_dot1_rate_limiter_h
#define _pon_adapter_me_dot1_rate_limiter_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 * @{
 */

/** \defgroup PON_ADAPTER_ME_DOT1_RATE_LIMITER IEEE 802.1 Rate Limiter
 *
 *  This Managed Entity allows rate limits to be defined for various types of
 *  upstream traffic that are processed by 802.1 bridges or related structures.
 *  The Managed Entity short name is "Dot1 Rate Limiter".
 *
 *   @{
 */

/** IEEE 802.1 Rate Limiter update data structure */
struct pa_dot1p_rate_limiter_update {
	/** Parent Managed Entity pointer */
	uint16_t parent_me_ptr;
	/** Parent Managed Entity pointer */
	uint8_t tp_type;
	/** Upstream UC flood rate ptr */
	uint16_t us_unicast_flood_rate_ptr;
	/** Upstream BC rate pointer */
	uint16_t us_broadcast_flood_rate_ptr;
	/** Upstream MC payload rate ptr */
	uint16_t us_multicast_flood_rate_ptr;
};

/** IEEE 802.1 Rate Limiter destroy data structure */
struct pa_dot1p_rate_limiter_destroy {
	/** Parent Managed Entity pointer */
	uint16_t parent_me_ptr;
	/** Parent Managed Entity pointer */
	uint8_t tp_type;
};

/** IEEE 802.1 Rate Limiter operations structure */
struct pa_dot1p_rate_limiter_ops {
	/** Update Dot1 Rate Limiter Managed Entity resources.
	 *
	 *  \remark Enable Managed Entity identifier mapping to driver index and
	 *  initialize corresponding driver structures when it is called first
	 *  time for the given Managed Entity ID.
         *
	 *  \param[in] ll_handle            Lower layer context pointer
	 *  \param[in] me_id                Managed Entity identifier
	 *  \param[in] data                 Update data
	 */
	enum pon_adapter_errno
		(*update)(void *ll_handle,
			  const uint16_t me_id,
			  const struct pa_dot1p_rate_limiter_update *data);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 *  \param[in] ll_handle            Lower layer context pointer
	 *  \param[in] me_id                Managed Entity identifier
	 *  \param[in] data                 Destroy data
	 */
	enum pon_adapter_errno
		(*destroy)(void *ll_handle,
			   const uint16_t me_id,
			   const struct pa_dot1p_rate_limiter_destroy *data);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
