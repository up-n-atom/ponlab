/******************************************************************************
 *
 *  Copyright (c) 2022 - 2024 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * XG-PON Downstream and Upstream Management PMHD.
 */

#ifndef _pon_adapter_management_pmhd
#define _pon_adapter_management_pmhd

#include "pon_adapter_base.h"
#include "pon_adapter_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** Structure to collect counters related to downstream PLOAM and OMCI message
 *  reception.
 *  These values are used by the XG-PON Downstream Management PMHD managed
 *  entity.
 */
struct pa_management_ds_cnt {
	/* PLOAM counters */
	/** Number of downstream PLOAM messages that have been discarded
	 *  due to PLOAM MIC errors.
	 */
	uint64_t ploam_mic_errors;
	/** Total number of downstream PLOAM messages that have been received
	 *  with a correct PLOAM MIC field.
	 */
	uint64_t all_ds;
	/** Number of "Burst_Profile" messages. */
	uint64_t profile;
	/** Number of "Ranging_Time" messages. */
	uint64_t ranging_time;
	/** Number of "Deactivate_ONU-ID" messages. */
	uint64_t deactivate_onu;
	/** Number of "Disable_Serial_Number" messages. */
	uint64_t disable_serial_number;
	/** Number of "Request_Registration" messages. */
	uint64_t request_registration;
	/** Number of "Assign_Alloc-ID" messages. */
	uint64_t assign_allocid;
	/** Number of "Key_Control" messages. */
	uint64_t key_control;
	/** Number of "Sleep_Allow" messages. */
	uint64_t sleep_allow;
	/** Number of "Assign_Alloc-ID" messages. */
	uint64_t assign_onu_id;
};

/** Structure to collect counters related to upstream PLOAM message
 *  transmission.
 *  These values are used by the XG-PON Upstream Management PMHD managed entity.
 */
struct pa_management_us_cnt {
	/* PLOAM message counters */
	/** Total number of upstream PLOAM messages. */
	uint64_t all_us;
	/** Number of "Serial_number_ONU" messages. */
	uint64_t serial_number;
	/** Number of "Registration" messages. */
	uint64_t registration;
	/** Number of "Key_report" messages. */
	uint64_t key_report;
	/** Number of "Acknowledge" messages. */
	uint64_t acknowledge;
	/** Number of "Sleep_request" messages. */
	uint64_t sleep_request;
};

struct pa_management_cnt_ops {
	/**
	 * Get counters related to downstream PLOAM message reception.
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[out] param Pointer to a structure as defined
	 * by \ref pa_management_ds_cnt.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*management_ds_cnt)(void *ll_handle,
			    struct pa_management_ds_cnt *props);

	/**
	 * Get counters related to upstream PLOAM message sending.
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[out] param Pointer to a structure as defined
	 * by \ref pa_management_us_cnt.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*management_us_cnt)(void *ll_handle,
			    struct pa_management_us_cnt *props);
};

/** @} */

#ifdef __cplusplus
}
#endif

#endif
