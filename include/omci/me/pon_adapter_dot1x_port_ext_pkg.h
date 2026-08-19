/******************************************************************************
 *
 * Copyright (c) 2020 MaxLinear, Inc.
 * Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * IEEE 802.1x Port Extension Package.
 */

#ifndef _pon_adapter_dot1x_port_ext_pkg_h
#define _pon_adapter_dot1x_port_ext_pkg_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 * @{
 */

/** \defgroup PON_ADAPTER_ME_DOT1X_PORT_EXTENSION_PACKAGE\
 *	      Dot1x Port Extension Package
 *
 * This Managed Entity represents a set of attributes that control a port's
 * IEEE 802.1x operation.
 *
 * @{
 */

enum pon_adapter_dot1x_port_ext_pkg_action_register {
	/** Action Register: No Action */
	PA_DOT1X_PORT_EXT_PKG_ACTION_REGISTER_FORCE_REAUTH = 0,
	/** Action Register: Force Re-authentication */
	PA_DOT1X_PORT_EXT_PKG_ACTION_REGISTER_FORCE_UNAUTH = 1,
	/** Action Register: Force Authenticated */
	PA_DOT1X_PORT_EXT_PKG_ACTION_REGISTER_FORCE_AUTH = 2
};

enum pon_adapter_dot1x_port_ext_pkg_ctrled_port_status {
	/** Controlled Port Status: Authorized */
	PA_DOT1X_PORT_EXT_PKG_CTRLED_PORT_STATUS_AUTHORIZED = 1,
	/** Controlled Port Status: Unauthorized */
	PA_DOT1X_PORT_EXT_PKG_CTRLED_PORT_STATUS_UNAUTHORIZED = 2
};

enum pon_adapter_dot1x_port_ext_pkg_auth_pae_state {
	/** Authenticator PAE State: Initializing */
	PA_DOT1X_PORT_EXT_PKG_AUTH_PAE_STATE_INITIALIZING = 0,
	/** Authenticator PAE State: Disconnected */
	PA_DOT1X_PORT_EXT_PKG_AUTH_PAE_STATE_DISCONNECTED = 1,
	/** Authenticator PAE State: Connecting */
	PA_DOT1X_PORT_EXT_PKG_AUTH_PAE_STATE_CONNECTING = 2,
	/** Authenticator PAE State: Authenticating */
	PA_DOT1X_PORT_EXT_PKG_AUTH_PAE_STATE_AUTHENTICATING = 3,
	/** Authenticator PAE State: Authenticated */
	PA_DOT1X_PORT_EXT_PKG_AUTH_PAE_STATE_AUTHENTICATED = 4,
	/** Authenticator PAE State: Aborting */
	PA_DOT1X_PORT_EXT_PKG_AUTH_PAE_STATE_ABORTING = 5,
	/** Authenticator PAE State: Held */
	PA_DOT1X_PORT_EXT_PKG_AUTH_PAE_STATE_HELD = 6,
	/** Authenticator PAE State: Force Authorized */
	PA_DOT1X_PORT_EXT_PKG_AUTH_PAE_STATE_FORCE_AUTHORIZED = 7,
	/** Authenticator PAE State: Force Unauthorized */
	PA_DOT1X_PORT_EXT_PKG_AUTH_PAE_STATE_FORCE_UNAUTHORIZED = 8,
	/** Authenticator PAE State: Restart */
	PA_DOT1X_PORT_EXT_PKG_AUTH_PAE_STATE_RESTART = 9
};
enum pon_adapter_dot1x_port_ext_pkg_backend_auth_state {
	/** Backend Authentication State: Request */
	PA_DOT1X_PORT_EXT_PKG_BACKEND_AUTH_STATE_REQUEST = 0,
	/** Backend Authentication State: Response */
	PA_DOT1X_PORT_EXT_PKG_BACKEND_AUTH_STATE_RESPONSE = 1,
	/** Backend Authentication State: Success */
	PA_DOT1X_PORT_EXT_PKG_BACKEND_AUTH_STATE_SUCCESS = 2,
	/** Backend Authentication State: Fail */
	PA_DOT1X_PORT_EXT_PKG_BACKEND_AUTH_STATE_FAIL = 3,
	/** Backend Authentication State: Timeout */
	PA_DOT1X_PORT_EXT_PKG_BACKEND_AUTH_STATE_TIMEOUT = 4,
	/** Backend Authentication State: Idle */
	PA_DOT1X_PORT_EXT_PKG_BACKEND_AUTH_STATE_IDLE = 5,
	/** Backend Authentication State: Initialize */
	PA_DOT1X_PORT_EXT_PKG_BACKEND_AUTH_STATE_INITIALIZE = 6,
	/** Backend Authentication State: Ignore */
	PA_DOT1X_PORT_EXT_PKG_BACKEND_AUTH_STATE_IGNORE = 7
};

/** Dot1x Port Extension Package data */
struct pa_dot1x_port_ext_pkg {
	/** Dot1x Enable */
	uint8_t dot1x_enable;
	/** Action Register */
	uint8_t action_register;
	/** Admin Controlled Directions */
	uint8_t adm_ctrled_dirs;
	/** Quiet Period */
	uint16_t quiet_period;
	/** Server Timeout Period */
	uint16_t server_timeout_period;
	/** Re-authentication Period */
	uint16_t re_auth_period;
	/** Re-authentication Enabled */
	uint8_t re_auth_enabled;
	/** Key Transmission Enabled */
	uint8_t key_tx_enabled;
};

/** Dot1x Port Extension Package operations structure */
struct pa_dot1x_port_ext_pkg_ops {
	/** Update Dot1x Port Extension Package Managed Entity resources
	 *  associated with PPTP ETH.
	 *
	 * \param[in] ll_handle             Lower layer context pointer
	 * \param[in] me_id                 Managed Entity identifier
	 * \param[in] update_data           Update data
	 */
	enum pon_adapter_errno (*update)(
		void *ll_handle,
		const uint16_t me_id,
		const struct pa_dot1x_port_ext_pkg *update_data);

	/** Retrieve current Authenticator PAE State
	 * \param[in]  ll_handle      Lower layer context pointer
	 * \param[in]  me_id          Managed Entity identifier
	 * \param[out] auth_pae_state Authenticator PAE State
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*auth_pae_state_get)(
		void *ll_handle,
		uint16_t me_id,
		uint8_t *auth_pae_state);

	/** Retrieve current Backend Authentication State
	 * \param[in]  ll_handle           Lower layer context pointer
	 * \param[in]  me_id               Managed Entity identifier
	 * \param[out] backend_auth_state  Backend Authentication State
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*backend_auth_state_get)(
		void *ll_handle,
		uint16_t me_id,
		uint8_t *backend_auth_state);

	/** Retrieve current Authenticator Controlled Port Status
	 * \param[in]  ll_handle                 Lower layer context pointer
	 * \param[in]  me_id                     Managed Entity identifier
	 * \param[out] auth_ctrled_port_status   Authenticator Controlled Port Status
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*auth_ctrled_port_status_get)(
		void *ll_handle,
		uint16_t me_id,
		uint8_t *auth_ctrled_port_status);

	/** Retrieve current Re-authentication Period
	 * \param[in]  ll_handle       Lower layer context pointer
	 * \param[in]  me_id           Managed Entity identifier
	 * \param[out] re_auth_period  Re-authentication Period
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*re_auth_period_get)(
		void *ll_handle,
		uint16_t me_id,
		uint16_t *re_auth_period);

	/** Retrieve current Re-authentication Enabled status
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[out] re_auth_enabled  Re-authentication Enabled status
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*re_auth_enabled_get)(
		void *ll_handle,
		uint16_t me_id,
		uint8_t *re_auth_enabled);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
