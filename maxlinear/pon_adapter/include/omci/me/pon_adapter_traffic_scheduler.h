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
 * Traffic Scheduler.
 */

#ifndef _pon_adapter_me_traffic_scheduler
#define _pon_adapter_me_traffic_scheduler

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_TRAFFIC_SCHEDULER Traffic Scheduler
 *
 *  An instance of this Managed Entity represents a logical object that can
 *  control upstream GEM packets. A traffic scheduler can accommodate GEM
 *  packets after a priority queue or other traffic scheduler and transfer them
 *  toward the next traffic scheduler or T-CONT. Because T-CONTs and traffic
 *  schedulers are created autonomously by the ONU, the ONU vendor predetermines
 *  the most complex traffic handling model it is prepared to support. The OLT
 *  may use less than the ONU's full capabilities, but cannot ask for more.
 *  See Appendix III of ITU-T G.988 for more detail.
 *
 *  After the ONU creates instances of T-CONT Managed Entity, it then
 *  autonomously creates instances of the Traffic Scheduler Managed Entity.
 *
 *  @{
 */

/** Traffic Scheduler policy definitions */
enum pa_traffic_scheduler_policy {
	/** NULL (unspecified) */
	PA_TRAFFICSCHEDULERG_POLICY_NULL = 0,
	/** Head of Line (SP, strict priority) */
	PA_TRAFFICSCHEDULERG_POLICY_SP = 1,
	/** Weighted Round Robin (WRR of DWRR) */
	PA_TRAFFICSCHEDULERG_POLICY_WRR = 2
};

/** Traffic Scheduler update structure */
struct pa_traffic_scheduler_update_data {
	/** T-CONT pointer */
	uint16_t tcont_ptr;
	/** Traffic Scheduler pointer */
	uint16_t traffic_scheduler_ptr;
	/** Policy */
	uint8_t policy;
	/** Priority/weight */
	uint8_t priority_weight;
};

/** Traffic Scheduler operations structure */
struct pa_traffic_scheduler_ops {
	/** Enable Managed Entity identifier mapping to driver index and
	 *  initialize corresponding driver structures.
	 *
	 * \param[in] ll_handle	    Lower layer context pointer
	 * \param[in] me_id	    Managed Entity identifier
	 * \param[in] update_data   Traffic scheduler update data
	 */
	enum pon_adapter_errno (*create)(void *ll_handle,
		uint16_t me_id,
		const struct pa_traffic_scheduler_update_data *update_data);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 * \param[in] ll_handle Lower layer context pointer
	 * \param[in] me_id	Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle, uint16_t me_id);

	/** Update Traffic Scheduler Managed Entity resources
	 *
	 * \param[in] ll_handle	    Lower layer context pointer
	 * \param[in] me_id	    Managed Entity identifier
	 * \param[in] update_data   Traffic scheduler update data
	 */
	enum pon_adapter_errno (*update)(void *ll_handle,
		uint16_t me_id,
		const struct pa_traffic_scheduler_update_data *update_data);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
