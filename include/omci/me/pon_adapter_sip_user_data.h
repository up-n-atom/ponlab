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
 * SIP User Data.
 */

#ifndef _pon_adapter_me_sip_user_data
#define _pon_adapter_me_sip_user_data

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_SIP_USER_DATA SIP User Data
 *
 *   The SIP user data defines the user specific configuration attributes
 *   associated with a specific VoIP CTP. This Managed Entity is conditionally
 *   required for ONUs that offer VoIP SIP services. If a non-OMCI interface is
 *   used to manage SIP for VoIP, this Managed Entity is unnecessary.
 *   The non-OMCI interface supplies the necessary data, which may be read back
 *   to the OLT via the SIP Config Portal Managed Entity.
 *
 *   An instance of this Managed Entity is created and deleted by the OLT. A SIP
 *   user data instance is required for each POTS UNI port using SIP protocol
 *   and configured by OMCI.
 *
 *  @{
 */

/** Cannot authenticate registration session */
#define PON_ADAPTER_ME_SIP_USER_DATA_ALARM_REGISTER_AUTH		0
/** Timeout waiting for response from registration server */
#define PON_ADAPTER_ME_SIP_USER_DATA_ALARM_REGISTER_TIMEOUT		1
/** Failure response received from registration server */
#define PON_ADAPTER_ME_SIP_USER_DATA_ALARM_REGISTER_FAIL		2
/** Missing dial plan */
#define PON_ADAPTER_ME_SIP_USER_DATA_ALARM_MISSING_DIAL_PLAN		3
/** Invalid dial plan */
#define PON_ADAPTER_ME_SIP_USER_DATA_ALARM_INVALID_DIAL_PLAN		4

/** SIP User Data update structure */
struct pa_sip_user_data {
	/** SIP agent Managed Entity pointer */
	uint16_t agent_ptr;
	/** User identification part of the address */
	const char *user_part_aor;
	/** Length of string user_part_aor */
	uint16_t user_part_aor_len;
	/** SIP user name */
	const char *username;
	/** Length of string username */
	uint16_t username_len;
	/** SIP password */
	const char *password;
	/** Length of string password */
	uint16_t password_len;
	/** Authentication security method Managed Entity realm */
	const char *realm;
	/** Length of string realm */
	uint16_t realm_len;
	/** Authentication security method Managed Entity validation scheme */
	uint8_t validation_scheme;
	/** PPTP POTS UNI pointer */
	uint16_t pptp_ptr;
	/** IP host Config Data Managed Entity IP address */
	uint32_t ip_addr;
	/** CID features */
	uint8_t cid_features;
};

/** SIP User Data operations structure */
struct pa_sip_user_data_ops {
	/** Update SIP user data Managed Entity resources
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] me_id		Managed Entity identifier
	 * \param[in] update_data	SIP User Data update data
	 */
	enum pon_adapter_errno(*update)
		(void *ll_handle,
		 uint16_t me_id,
		 struct pa_sip_user_data *update_data);

	/** Cleanup SIP user data Managed Entity resources
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] me_id		Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle, uint16_t me_id);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
