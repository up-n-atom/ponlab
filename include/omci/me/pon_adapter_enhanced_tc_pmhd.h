/******************************************************************************
 *
 *  Copyright (c) 2022 - 2025 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * Enhanced TC PMHD.
 */

#ifndef _pon_adapter_enhanced_tc_pmhd
#define _pon_adapter_enhanced_tc_pmhd

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_ENHANCED_TC_PMHD Enhanced TC PMHD
 *
 *  This Managed Entity collects performance monitoring data related to an the
 *  XGEM TC layer.
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
 * instance of the ANI-G Managed Entity.
 */

/** Structure to collect counters related to XGEM TC reception and
 *  transmission.
 *  These values are used by the Enhanced TC PMHD managed entity.
 */
struct pa_enhanced_tc_counters {
	/** Uncorrected PSBd HEC errors */
	uint64_t psbd_hec_err_uncorr;
	/** Uncorrected FS/XGTC HEC errors */
	uint64_t fs_hec_err_uncorr;
	/** Unknown burst profiles
	 *  The firmware does not support this counter due to
	 *  performance limitations and always reports the value 0.
	 */
	uint64_t burst_profile_err;
	/** Transmitted GEM frames. */
	uint64_t tx_frames;
	/** Transmitted GEM frame fragments. */
	uint64_t tx_fragments;
	/** Lost words due to uncorrectable HEC errors.
	 *  This is the number of four-byte words lost because of an
	 *  XGEM frame HEC error. In general, all XGTC payload following
	 *  the error it lost, until the next PSBd event.
	 */
	uint64_t lost_words;
	/** PLOAM MIC errors.
	 *  This is the number of received PLOAM messages with an invalid
	 *  Message Integrity Check (MIC) field.
	 */
	uint64_t ploam_mic_err;
	/** XGEM key errors.
	 *  The number of received key errors is provided for XG-PON,
	 *  XGS-PON, and NG-PON2 only. The value is set to 0 for GPON
	 *  implementations.
	 */
	uint64_t key_errors;
	/** Discarded XGEM frames */
	uint64_t xgem_hec_err_uncorr;
	/** Transmitted bytes in GEM frames.
	 *  This value reports user payload bytes only, not including
	 *  header bytes.
	 */
	uint64_t tx_bytes;
	/** Received bytes in valid GEM frames.
	 *  This value reports user payload bytes only, not including
	 *  header bytes.
	 */
	uint64_t rx_bytes;
	/** Number of LODS events,
	 *  resulting in state transitions from O5.1 to O6.
	 */
	uint64_t lods;
	/** Number of LODS restore events,
	 *  resulting in a direct state transition from O6 to O5.1.
	 */
	uint64_t lods_rest;
	/** Number of LODS reactivation events,
	 *  resulting in a state transition from O6 to O1.1.
	 */
	uint64_t lods_react;
	/** Received GEM frames.
	 *  This value reports the number of valid GEM frames that have
	 *  been received without uncorrectable errors and a valid HEC.
	 */
	uint64_t rx_frames;
	/** Received GEM frame fragments. */
	uint64_t rx_fragments;
};

struct pa_enhanced_tc_pmhd_ops {
	/**
	 * Get XGEM TC counters
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[out] param Pointer to a structure as defined
	 * by \ref pa_enhanced_tc_counters.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*enhanced_tc_counters_get)(void *ll_handle,
				    struct pa_enhanced_tc_counters *props);

	/**
	 * Get XG-PON LODS counters.
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[out] param Pointer to a structure as defined
	 * by \ref pa_enhanced_tc_counters.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*lods_counters_get)(void *ll_handle,
			     struct pa_enhanced_tc_counters *props);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
