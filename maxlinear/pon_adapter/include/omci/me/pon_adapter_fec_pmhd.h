/******************************************************************************
 *
 *  Copyright (c) 2021 MaxLinear, Inc.
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
 * FEC PMHD.
 */

#ifndef _pon_adapter_me_gal_fec_pmhd
#define _pon_adapter_me_gal_fec_pmhd

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_FEC_PMHD FEC PMHD
 *
 *  This Managed Entity collects performance monitoring data associated with FEC
 *  counters. Instances of this Managed Entity are created and deleted by the
 *  OLT.
 *  The interface provides 64bit values to allow to use it also for
 *  the Managed Entity "Enhanced FEC performance monitoring history data"
 *
 *  @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
   instance of the ANI-G. */

/** FEC PMHD attribute handling operations structure */
struct pa_fec_pmhd_ops {
	/** Retrieve total counters for the FEC PMHD
	 *
	 * \param[in]  ll_handle		   Lower layer context pointer
	 * \param[in]  me_id			   Managed Entity identifier
	 * \param[out] cnt_corrected_bytes	   Corrected bytes
	 * \param[out] cnt_corrected_code_words	   Corrected code words
	 * \param[out] cnt_uncorrected_code_words  Uncorrectable code words
	 * \param[out] cnt_total_code_words	   Total code words
	 * \param[out] cnt_fec_seconds		   FEC seconds
	 */
	enum pon_adapter_errno (*cnt_get)(
		void *ll_handle,
		uint16_t me_id,
		uint64_t *cnt_corrected_bytes,
		uint64_t *cnt_corrected_code_words,
		uint64_t *cnt_uncorrected_code_words,
		uint64_t *cnt_total_code_words,
		uint16_t *cnt_fec_seconds);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
