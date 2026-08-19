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
 * GAL Ethernet PMHD.
 */

#ifndef _pon_adapter_me_gal_ethernet_pmhd
#define _pon_adapter_me_gal_ethernet_pmhd

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_GAL_ETHERNET_PMHD GAL Ethernet PMHD
 *
 *  This Managed Entity collects performance monitoring data associated with a
 *  GEM interworking termination point when the GEM layer provides Ethernet
 *  service.
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
   instance of the GEM interworking TP. */

/** GAL Ethernet PMHD operations structure */
struct pon_adapter_gal_ethernet_pmhd_ops {
	/** Retrieve total counters for GAL Ethernet PMHD Managed Entity
	 *
	 * \param[in]  ll_handle            Lower layer context pointer
	 * \param[in]  me_id                Managed Entity identifier
	 * \param[out] cnt_discarded_frames Discarded frames
	 */
	enum pon_adapter_errno (*total_cnt_get)(void *ll_handle,
						uint16_t me_id,
						uint64_t *cnt_discarded_frames);

	/** Retrieve current values of GAL Ethernet PMHD Managed Entity counters
	 *
	 * \param[in]  ll_handle            Lower layer context pointer
	 * \param[in]  me_id                Managed Entity identifier
	 * \param[in]  get_curr             Get current value or last interval
	 * \param[in]  reset_cnt            Reset counters
	 * \param[out] cnt_discarded_frames Discarded frames
	 */
	enum pon_adapter_errno (*cnt_get)(void *ll_handle,
					  uint16_t me_id,
					  bool get_curr,
					  bool reset_cnt,
					  uint64_t *cnt_discarded_frames);

	/** Set GAL Ethernet PMHD counters thresholds
	 *
	 * \param[in] ll_handle             Lower layer context pointer
	 * \param[in] me_id                 Managed Entity identifier
	 * \param[in] cnt_discarded_frames  Discarded frames
	 */
	enum pon_adapter_errno (*thr_set)(void *ll_handle,
					  uint16_t me_id,
					  uint32_t cnt_discarded_frames);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
