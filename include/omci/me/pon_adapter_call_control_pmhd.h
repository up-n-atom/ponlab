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
 * Call Control PMHD.
 */

#ifndef _pon_adapter_call_control_pmhd
#define _pon_adapter_call_control_pmhd

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_CALL_CONTROL_PMHD Call Control PMHD
 *
 *  This Managed Entity collects performance monitoring data related to the call
 *  control channel.
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/** Call_control_pmhd attribute handling operations structure */
struct pa_call_control_pmhd_ops {
	/** Retrieve current value of Call Control PMHD counters
	 *
	 * \param[in]  ll_handle			Lower layer context pointer
	 * \param[in]  me_id				Managed Entity identifier
	 * \param[out] call_setup_failures		Call setup failures
	 * \param[out] call_setup_timer		Call setup timer
	 * \param[out] call_terminate_failures	Call terminate failures
	 * \param[out] analog_port_releases	Analog port releases
	 * \param[out] analog_port_offhook_timer	Analog port off-hook timer
	 */
	enum pon_adapter_errno (*cnt_get)(
		void *ll_handle,
		uint16_t me_id,
		uint32_t *call_setup_failures,
		uint32_t *call_setup_timer,
		uint32_t *call_terminate_failures,
		uint32_t *analog_port_releases,
		uint32_t *analog_port_offhook_timer);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
