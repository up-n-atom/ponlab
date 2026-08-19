/******************************************************************************
 *
 *  Copyright (c) 2022 MaxLinear, Inc.
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
 * VoIP Line Status.
 */

#ifndef _pon_adapter_me_voip_line_status
#define _pon_adapter_me_voip_line_status

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_VOIP_LINE_STATUS VoIP Line Status
 *
 *   The VoIP line status Managed Entity contains line status information for
 *   POTS ports using VoIP service. An ONU that supports VoIP automatically
 *   creates or deletes an instance of this Managed Entity upon creation or
 *   deletion of a PPTP POTS UNI.
 *
 *   An instance of this Managed Entity is associated with a PPTP POTS UNI.
 *
 *  @{
 */

/** VoIP Line Status operations structure */
struct pa_voip_line_status_ops {
	/** Get the VoIP line state
	 *
	 * \param[in]  ll_handle	Lower layer context pointer
	 * \param[in]  me_id		Managed Entity identifier
	 * \param[out] state		VoIP line state
	 */
	enum pon_adapter_errno (*line_state_get)(void *ll_handle,
						 uint16_t me_id,
						 uint8_t *state);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
