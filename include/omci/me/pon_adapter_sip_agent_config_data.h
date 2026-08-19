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
 * SIP Agent Configuration Data.
 */

#ifndef _pon_adapter_me_sip_agent_config_data
#define _pon_adapter_me_sip_agent_config_data

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_SIP_AGENT_CONFIG_DATA SIP Agent Config Data
 *
 *   The SIP agent config data Managed Entity models a SIP signaling agent. It
 *   defines the configuration necessary to establish communication for
 *   signaling between the SIP user agent and a SIP server.
 *
 *   Note: If a non-OMCI interface is used to manage SIP for VoIP, this Managed
 *   Entity is unnecessary.
 *   The non-OMCI interface supplies the necessary data, which may
 *   be read back to the OLT via the SIP config portal Managed Entity.
 *
 *   Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/** SIP Agent Config Data update structure */
struct pa_sip_agent_config_data {
	/** Proxy server address pointer */
	const char *proxy_addr;
	/** Length of string proxy_addr */
	uint16_t proxy_addr_len;
	/** Primary SIP DNS */
	uint32_t primary_dns;
	/** Secondary SIP DNS */
	uint32_t secondary_dns;
	/** SIP agent IP */
	uint32_t agent_ip;
	/** SIP signaling protocol type */
	uint32_t agent_proto;
	/** SIP signaling protocol DSCP value */
	uint8_t agent_dscp;
	/** IP agent port */
	uint32_t agent_port;
	/** SIP registration expiration time [s] */
	uint32_t reg_exp_time;
	/** SIP re-registration head start time [s] */
	uint32_t rereg_head_start;
	/** Host part URI */
	const char *host_part_uri;
	/** Length of string host_part_uri */
	uint16_t host_part_uri_len;
	/** SIP registrar */
	const char *registrar;
	/** Length of string registrar */
	uint16_t registrar_len;
};

/** sip_agent_config_data operations structure */
struct pa_sip_agent_config_data_ops {
	/** Update SIP agent config data Managed Entity resources
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] me_id		Managed Entity identifier
	 * \param[in] update_data	SIP Agent config data
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*update)
		(void *ll_handle,
		 const uint16_t me_id,
		 struct pa_sip_agent_config_data *update_data);

	/** Cleanup SIP agent config data Managed Entity resources
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] me_id		Managed Entity identifier
	 * \param[in] destroy_agent	Destroy SIP Agent instance
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle,
					  uint16_t me_id,
					  bool destroy_agent);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
