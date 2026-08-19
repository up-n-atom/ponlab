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
 * MAC Bridge Port PMHD.
 */

#ifndef _pon_adapter_me_mac_bridge_port_pmhd
#define _pon_adapter_me_mac_bridge_port_pmhd

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 * @{
 */

/** \defgroup PON_ADAPTER_ME_MAC_BRIDGE_PORT_PMHD MAC Bridge Port PMHD
 *
 * This Managed Entity collects performance monitoring data associated with a
 * MAC bridge. Instances of this Managed Entity are created and deleted by the
 * OLT.
 *
 * @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
   instance of the MAC bridge port configuration data Managed Entity. */

/** MAC Bridge Port PMHD counters structure */
struct pa_mac_bp_pmhd_cnt {
	/** Forwarded frame counter */
	uint64_t cnt_forwarded_frame;
	/** Delay exceeded discard counter */
	uint64_t cnt_delay_exceed_discard;
	/** Received frame counter */
	uint64_t cnt_received_frame;
	/** Received and discarded counter */
	uint64_t cnt_received_and_discarded;
};

/** MAC Bridge Port PMHD operations structure */
struct pa_mac_bp_pmhd_ops {
	/** Retrieve total counters for MAC Bridge Port PMHD Managed Entity
	 *
	 * \param[in]  ll_handle  Lower layer context pointer
	 * \param[in]  me_id	  Managed Entity identifier
	 * \param[out] cnt_data	  MAC Bridge Port PMHD counters data
	 */
	enum pon_adapter_errno (*cnt_get)(
		void *ll_handle,
		const uint16_t me_id,
		struct pa_mac_bp_pmhd_cnt *cnt_data);

	/** Retrieve MTU Exceed Discard Counter total value of the MAC Bridge
	 *  Port(bp) PMHD Managed Entity connected to PPTP ETH UNI
	 *
	 * \param[in]  ll_handle	   Lower layer context pointer
	 * \param[in]  me_id		   Managed Entity identifier
	 * \param[out] mtu_exceed_discard  MTU Exceed Discard counter
	 */
	enum pon_adapter_errno (*mtu_exceeded_discard_cnt_get)(
		void *ll_handle,
		const uint16_t me_id,
		uint64_t *mtu_exceed_discard);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
