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
 * GEM Port PMHD.
 */

#ifndef _pon_adapter_me_gem_port_pmhd
#define _pon_adapter_me_gem_port_pmhd

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_GEM_PORT_PMHD GEM Port PMHD
 *
 *  This Managed Entity collects performance monitoring data associated with a
 *  GEM port network CTP.
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
   instance of the GEM port network CTP. */

/** GEM port PMHD attribute handling operations structure */
struct pa_gem_port_pmhd_ops {
	/** Retrieve total counters for GEM Port PMHD
	 *
	 * \param[in]  ll_handle	 Lower layer context pointer
	 * \param[in]  me_id		 Managed Entity identifier
	 * \param[out] tx_gem_frames	 Transmitted GEM frames
	 * \param[out] rx_gem_frames	 Received GEM frames (packets)
	 * \param[out] rx_payload_bytes  Received payload bytes (GEM blocks)
	 * \param[out] tx_payload_bytes  Transmitted GEM frames (GEM blocks)
	 * \param[out] lost_packets	 Number of dropped RX frames
	 */
	enum pon_adapter_errno (*cnt_get)(void *ll_handle,
					  uint16_t me_id,
					  uint64_t *tx_gem_frames,
					  uint64_t *rx_gem_frames,
					  uint64_t *rx_payload_bytes,
					  uint64_t *tx_payload_bytes,
					  uint32_t *lost_packets);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
