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
 * RTP PMHD.
 */

#ifndef _pon_adapter_rtp_pmhd
#define _pon_adapter_rtp_pmhd

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_RTP_PMHD RTP PMHD
 *
 *  This Managed Entity collects performance monitoring data related to an RTP
 *  session.
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
 * instance of the PPTP POTS UNI Managed Entity.
 */

/** RTP PMHD counters structure */
struct pa_rtp_cnt {
	/** RTP errors */
	uint32_t rtp_errors;
	/** Packet loss */
	uint32_t packet_loss;
	/** Maximum jitter */
	uint32_t max_jitter;
	/** Max time between RTCP packets */
	uint32_t max_time_between_rtcp_packets;
	/** Buffer underflows */
	uint32_t buffer_underflows;
	/** Buffer overflows */
	uint32_t buffer_overflows;
};

/** RTP PMHD operations structure */
struct pa_rtp_pmhd_ops {
	/** Retrieve the current values of RTP PMHD counters
	 *
	 * \param[in]  ll_handle  Lower layer context pointer
	 * \param[in]  me_id      Managed Entity identifier
	 * \param[out]  rtp_cnt    Pointer to RTP PMHD counters structure
	 */
	enum pon_adapter_errno (*cnt_get)(
			void *ll_handle,
			uint16_t me_id,
			struct pa_rtp_cnt *rtp_cnt);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
