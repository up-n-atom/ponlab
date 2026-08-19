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
 * SIP Agent Call Initiation PMHD.
 */

#ifndef _pon_adapter_sip_call_init_pmhd
#define _pon_adapter_sip_call_init_pmhd

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_SIP_CALL_INIT_PMHD SIP Call Initiation PMHD
 *
 *  This Managed Entity collects performance monitoring data related to a SIP
 *  session.
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
 * instance of the SIP agent config data or the SIP config portal Managed
 * Entity.
 */

/** SIP Call Initiation counters structure */
struct pa_sip_call_init_cnt {
	/** Failed to connect counter */
	uint32_t failed_to_connect;
	/** Failed to validate counter */
	uint32_t failed_to_validate;
	/** Timeout counter */
	uint32_t timeout;
	/** Failure received counter */
	uint32_t failure_received;
	/** Failed to authenticate counter */
	uint32_t failed_to_auth;
};

/** SIP Call Initiation counters operations structure */
struct pa_sip_call_init_pmhd_ops {
	/** Retrieve current value of SIP call initiation PMHD counters
	 *
	 * \param[in]  ll_handle          Lower layer context pointer
	 * \param[in]  me_id              Managed Entity identifier
	 * \param[out]  sip_call_init_cnt  Pointer to pa_sip_call_init_cnt
	 *                                structure
	 */
	enum pon_adapter_errno (*cnt_get)(
		void *ll_handle,
		uint16_t me_id,
		struct pa_sip_call_init_cnt *sip_call_init_cnt);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
