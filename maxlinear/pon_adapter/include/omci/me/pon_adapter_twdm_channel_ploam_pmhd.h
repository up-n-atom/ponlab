/******************************************************************************
 *
 *  Copyright (c) 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the TWDM Channel
 * PLOAM Performance Monitoring History Data managed entity (part 1-3).
 */

#ifndef _pon_adapter_twdm_channel_ploam_pmhd_h
#define _pon_adapter_twdm_channel_ploam_pmhd_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_TWDM_CHANNEL_PLOAM_PMHD TWDM Channel PLOAM PMHD
 *
 * This ME collects certain PLOAM-related PM data associated with the
 * slot/circuit pack, hosting one or more ANI-G MEs, for a specific TWDM
 * channel. Instances of this ME are created and deleted by the OLT.
 *
 * An instance of this ME is associated with an instance of TWDM channel ME.
 *
 *  @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
 * instance of the TWDM Channel Entity.
 */

/** Structure to collect counters related to PLOAM-related PM data.
 *  These values are used by the TWDM Channel PLOAM PMHD1 managed entity.
 */
struct pa_twdm_channel_ploam_pmhd1 {
	/** PLOAM MIC errors
	 *  The counter of received PLOAM messages that remain unparsable due
	 *  to MIC error.
	 */
	uint64_t ploam_mic_errors;
	/** Downstream PLOAM message count
	 *  The counter of received broadcast and unicast PLOAM messages
	 *  pertaining to the given ONU.
	 */
	uint64_t ds_ploam_msg_cnt;
	/** Ranging_Time message count
	 *  The counter of received Ranging_Time PLOAM messages.
	 */
	uint64_t rng_time_msg_cnt;
	/** Protection_Control message count
	 *  The counter of received Protection_Control PLOAM messages.
	 */
	uint64_t prot_ctrl_msg_cnt;
	/** Adjust_Tx_Wavelength message count
	 *  The counter of received Adjust_Tx_Wavelength PLOAM messages.
	 */
	uint64_t adj_tx_wl_msg_cnt;
	/** Adjust_Tx_Wavelength adjustment amplitude
	 *  An estimator of the absolute value of the transmission wavelength
	 *  adjustment.
	 */
	uint64_t adj_tx_wl_adj_amplitude;
};

/** Structure to collect counters related to PLOAM-related PM data.
 *  These values are used by the TWDM Channel PLOAM PMHD2 managed entity.
 */
struct pa_twdm_channel_ploam_pmhd2 {
	/** System_Profile message count
	 *  The counter of received System_Profile PLOAM messages.
	 */
	uint64_t sys_profile_msg_cnt;
	/** Channel_Profile message count
	 *  The counter of received Channel_Profile PLOAM messages.
	 */
	uint64_t ch_profile_msg_cnt;
	/** Burst_Profile message count
	 *  The counter of received Burst_Profile PLOAM messages.
	 */
	uint64_t burst_profile_msg_cnt;
	/** Assign_ONU-ID message count
	 *  The counter of received Assign_ONU-ID PLOAM messages.
	 */
	uint64_t ass_onu_msg_cnt;
	/** Unsatisfied Adjust_Tx_Wavelength requests
	 *  The counter of Adjust_Tx_Wavelength requests not applied or
	 *  partially applied due to target US wavelength being out of Tx
	 *  tuning range.
	 */
	uint64_t uns_adj_tx_wl_req;
	/** Deactivate_ONU-ID message count
	 *  The counter of received Deactivate_ONU-ID PLOAM messages.
	 */
	uint64_t deact_onu_msg_cnt;
	/** Disable_Serial_Number message count
	 *  The counter of received Disable_Serial_Number PLOAM messages.
	 */
	uint64_t dis_serial_msg_cnt;
	/** Request_Registration message count
	 *  The counter of received Request_Registration PLOAM messages.
	 */
	uint64_t req_reg_msg_cnt;
	/** Assign_Alloc-ID message count
	 *  The counter of received Assign_Alloc-ID PLOAM messages.
	 */
	uint64_t ass_alloc_id_msg_cnt;
	/** Key_Control message count
	 *  The counter of received Key_Control PLOAM messages.
	 */
	uint64_t key_ctrl_msg_cnt;
	/** Sleep_Allow message count
	 *  The counter of received Sleep_Allow PLOAM messages.
	 */
	uint64_t slp_allow_msg_cnt;
	/** Tuning_Control/Request message count
	 *  The counter of received Tuning_Control PLOAM messages with
	 *  Request operation code
	 */
	uint64_t tune_req_msg_cnt;
	/** Tuning_Control/Complete_d message count
	 *  The counter of received Tuning_Control PLOAM messages with
	 *  Complete_d operation code.
	 */
	uint64_t tune_compl_msg_cnt;
	/** Calibration_Request message count
	 *  The counter of received Calibration_Request PLOAM messages.
	 */
	uint64_t calib_req_msg_cnt;
};

/** Structure to collect counters related to PLOAM-related PM data.
 *  These values are used by the TWDM Channel PLOAM PMHD3 managed entity.
 */
struct pa_twdm_channel_ploam_pmhd3 {
	/** Upstream PLOAM message count
	 *  The aggregate counter of PLOAM messages, other than AK PLOAM MT,
	 *  transmitted by the given ONU.
	 */
	uint64_t us_ploam_msg_cnt;
	/** Serial_Number_ONU (in-band) message count
	 *  The counter of transmitted in-band Serial_Number_ONU PLOAM
	 *  messages.
	 */
	uint64_t ser_onu_inb_msg_cnt;
	/** Serial_Number_ONU (AMCC) message count
	 *  The counter of transmitted auxiliary management and control
	 *  channel (AMCC) Serial_Number_ONU PLOAM messages.
	 */
	uint64_t ser_onu_amcc_msg_cnt;
	/** Registration message count
	 *  The counter of transmitted Registration PLOAM messages.
	 */
	uint64_t reg_msg_cnt;
	/** Key_Report message count
	 *  The counter of transmitted Key_Report PLOAM messages.
	 */
	uint64_t key_rep_msg_cnt;
	/** Acknowledgment message count
	 *  The counter of transmitted Registration PLOAM messages.
	 */
	uint64_t ack_msg_cnt;
	/** Sleep_Request message count
	 *  The counter of transmitted Sleep_Request PLOAM messages.
	 */
	uint64_t sleep_req_msg_cnt;
	/** Tuning_Response (ACK/NACK) message count
	 *  The counter of transmitted Tuning_Response PLOAM messages with
	 *  ACK/NACK operation code.
	 */
	uint64_t tune_resp_an_msg_cnt;
	/** Tuning_Response (Complete_u/Rollback) message count
	 *  The counter of transmitted Tuning_Response PLOAM messages with
	 *  Complete_u/Rollback operation code.
	 */
	uint64_t tune_resp_crb_msg_cnt;
	/** Power_Consumption_Report message count
	 *  The counter of transmitted Power_Consumption_Report PLOAM messages.
	 */
	uint64_t pwr_cons_msg_cnt;
	/** Change_Power_Level parameter error count
	 *  The counter of transmitted Acknowledgment PLOAM messages with
	 *  Parameter Error completion code in response to Change_Power_Level
	 *  PLOAM message.
	 */
	uint64_t cpl_err_param_err;
};

/** TWDM Channel PLOAM PMHD attribute handling operations structure */
struct pa_twdm_channel_ploam_pmhd_ops {
	/**
	 * Get TWDM Channel PLOAM counters (part 1)
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[in] me_id Managed Entity ID
	 * \param[out] param Pointer to a structure as defined
	 * by \ref pa_twdm_channel_ploam_pmhd1.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*cnt_get_pmhd1)(void *ll_handle, uint16_t me_id,
			    struct pa_twdm_channel_ploam_pmhd1 *props);

	/**
	 * Get TWDM Channel PLOAM counters (part 2)
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[in] me_id Managed Entity ID
	 * \param[out] param Pointer to a structure as defined
	 * by \ref pa_twdm_channel_ploam_pmhd2.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*cnt_get_pmhd2)(void *ll_handle, uint16_t me_id,
			    struct pa_twdm_channel_ploam_pmhd2 *props);

	/**
	 * Get TWDM Channel PLOAM counters (part 3)
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[in] me_id Managed Entity ID
	 * \param[out] param Pointer to a structure as defined
	 * by \ref pa_twdm_channel_ploam_pmhd3.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*cnt_get_pmhd3)(void *ll_handle, uint16_t me_id,
			    struct pa_twdm_channel_ploam_pmhd3 *props);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
