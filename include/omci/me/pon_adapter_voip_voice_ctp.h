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
 * VoIP Voice Connection Termination Point (CTP).
 */

#ifndef _pon_adapter_me_voip_voice_ctp
#define _pon_adapter_me_voip_voice_ctp

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_VOIP_VOICE_CTP VoIP Voice CTP
 *
 *   The VoIP voice CTP defines the attributes necessary to associate a
 *   specified VoIP service (SIP, H.248) with a POTS UNI. This entity is
 *   conditionally required for ONUs that offer VoIP services. If a non-OMCI
 *   interface is used to manage VoIP signaling, this Managed Entity is
 *   unnecessary.
 *
 *   An instance of this Managed Entity is created and deleted by the OLT.
 *   A VoIP voice CTP Managed Entity is needed for each PPTP POTS UNI
 *   served by VoIP.
 *
 *  @{
 */

/** VoIP Voice CTP media update structure */
struct pa_voip_voice_ctp_media_data {
	/** SIP user Managed Entity pointer */
	uint16_t sip_user_me_id;
	/** Code selection 1 */
	uint8_t code_selection_1;
	/** Code selection 2 */
	uint8_t code_selection_2;
	/** Code selection 3 */
	uint8_t code_selection_3;
	/** Code selection 4 */
	uint8_t code_selection_4;
};

/** VoIP Voice CTP service update structure */
struct pa_voip_voice_ctp_service_data {
	/** PPTP POTS UNI Managed Entity pointer */
	uint16_t pptp_pots_me_id;
	/** Announcement type */
	uint8_t announcement_type;
	/** Jitter target */
	uint16_t jitter_target;
	/** Jitter maximum buffer */
	uint16_t jitter_buffer_max;
	/** Echo cancellation off/on */
	uint8_t echo_cancel_ind;
	/** POTS signaling protocol variant */
	uint16_t pstn_protocol_variant;
	/** Power level of DTMF digits */
	uint16_t dtmf_digit_levels;
	/** Duration of DTMF digits */
	uint16_t dtmf_digit_duration;
	/** Minimum switchhook flash duration */
	uint16_t hook_flash_minimum_time;
	/** Maximum switchhook flash duration */
	uint16_t hook_flash_maximum_time;
};

/** VoIP Voice CTP RTP update structure */
struct pa_voip_voice_ctp_rtp_data {
	/** Minimum traffic RTP port */
	uint16_t local_port_min;
	/** Highest traffic RTP port */
	uint16_t local_port_max;
	/** DSCP mark */
	uint8_t dscp_mark;
	/** Enable/disable RTP piggyback events */
	uint8_t piggyback_events;
	/** Enable/disable RTP tone events */
	uint8_t tone_events;
	/** Enable/disable RTP DTMF events */
	uint8_t dtmf_events;
	/** Enable/disable RTP CAS events */
	uint8_t cas_events;
};

/** VoIP Voice CTP operations structure */
struct pa_voip_voice_ctp_ops {
	/** Update VoIP Voice CTP Managed Entity media resources
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] me_id		Managed Entity identifier
	 * \param[in] update_data	VoIP Voice CTP media update data
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno(*media_update)
		(void *ll_handle,
		 uint16_t me_id,
		 struct pa_voip_voice_ctp_media_data *update_data);

	/** Update VoIP Voice CTP Managed Entity service resources
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] me_id		Managed Entity identifier
	 * \param[in] update_dat	VoIP Voice CTP service update data
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno(*service_update)
		(void *ll_handle,
		 uint16_t me_id,
		 struct pa_voip_voice_ctp_service_data *update_data);

	/** Update VoIP Voice CTP Managed Entity RTP resources
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] me_id		Managed Entity identifier
	 * \param[in] update_data	VoIP Voice CTP RTP update data
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno(*rtp_update)
		(void *ll_handle,
		 uint16_t me_id,
		 struct pa_voip_voice_ctp_rtp_data *update_data);

	/** Cleanup VoIP Voice CTP Managed Entity resources
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] me_id		Managed Entity identifier
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
