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
 * MAC Bridge PMHD.
 */

#ifndef _pon_adapter_me_mac_bridge_pmhd
#define _pon_adapter_me_mac_bridge_pmhd

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 * @{
 */

/** \defgroup PON_ADAPTER_ME_MAC_BRIDGE_PMHD MAC Bridge PMHD
 *
 * This Managed Entity collects performance monitoring data associated with a
 * MAC bridge.
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 * @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
   instance of the MAC bridge service profile. */

/** MAC Bridge PMHD operations structure */
struct pa_mac_bridge_pmhd_ops {
	/** Retrieve total counters for MAC Bridge PMHD
	 *
	 * \param[in]  ll_handle                   Lower layer context pointer
	 * \param[in]  me_id                       Managed Entity identifier
	 * \param[out] cnt_bridge_learning_discard Bridge learning entry
	 *                                         discard count
	 */
	enum pon_adapter_errno (*cnt_get)(
			void *ll_handle,
			uint16_t me_id,
			uint32_t *cnt_bridge_learning_discard);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
