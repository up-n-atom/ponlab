/******************************************************************************
 *
 *  Copyright (c) 2017 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * SIP Agent PMHD.
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

/** \defgroup PON_ADAPTER_ME_SIP_AGENT_PMHD SIP Agent PMHD
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

/** SIP Agent counters structure */
struct pa_sip_agent_cnt {
	/** Transactions */
	uint32_t transactions;
	/** RX invite requests */
	uint32_t rx_invite_reqs;
	/** RX invite retransmissions */
	uint32_t rx_invite_retrans;
	/** RX noninvite requests */
	uint32_t rx_noninvite_reqs;
	/** RX noninvite retransmissions */
	uint32_t rx_noninvite_retrans;
	/** RX Response */
	uint32_t rx_response;
	/** RX Response retransmissions */
	uint32_t rx_response_retrans;
	/** TX invite requests */
	uint32_t tx_invite_reqs;
	/** TX invite retransmissions */
	uint32_t tx_invite_retrans;
	/** TX noninvite requests */
	uint32_t tx_noninvite_reqs;
	/** TX noninvite retransmissions */
	uint32_t tx_noninvite_retrans;
	/** TX Response */
	uint32_t tx_response;
	/** TX Response retransmissions */
	uint32_t tx_response_retrans;
};

/** SIP Agent PMHD operations structure */
struct pa_sip_agent_pmhd_ops {
	/** Retrieve current value of SIP call initiation PMHD counters
	 *
	 * \param[in]  ll_handle      Lower layer context pointer
	 * \param[in]  me_id          Managed Entity identifier
	 * \param[out]  sip_agent_cnt  Pointer to Sip Agent counters structure
	 */
	enum pon_adapter_errno (*cnt_get)(
		void *ll_handle,
		uint16_t me_id,
		struct pa_sip_agent_cnt *sip_agent_cnt);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
