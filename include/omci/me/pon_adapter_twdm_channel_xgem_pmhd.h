/******************************************************************************
 *
 *  Copyright (c) 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * TWDM Channel XGEM Performance Monitoring History Data managed entity.
 */

#ifndef _pon_adapter_twdm_channel_xgem_h
#define _pon_adapter_twdm_channel_xgem_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_TWDM_CHANNEL_XGEM_PMHD TWDM Channel XGEM PMHD
 *
 * This ME collects certain XGEM-related PM data associated with the
 * slot/circuit pack, hosting one or more ANI-G MEs, for a specific TWDM
 * channel. Instances of this ME are created and deleted by the OLT.
 *
 * An instance of this ME is associated with an instance of TWDM channel ME.
 *
 *  @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
 * instance of the TWDM Channel Entity.
 */

/** Structure to collect counters related to XGEM-related PM data.
 *  These values are used by the TWDM Channel XGEM PMHD managed entity.
 */
struct pa_twdm_channel_xgem_pmhd {
	/** Total transmitted XGEM frames
	 *
	 *  The counter aggregated across all XGEM ports of the given ONU.
	 *  This attribute is read-only by the OLT.
	 */
	uint64_t t_tx_frames;

	/** Transmitted XGEM frames with LF bit not set
	 *
	 *  The counter aggregated across all XGEM ports of the given ONU
	 *  identifies the number of fragmentation operations.
	 *  This attribute is read-only by the OLT.
	 */
	uint64_t t_tx_fragments;

	/** Total received XGEM frames
	 *
	 *  The counter aggregated across all XGEM ports of the given ONU.
	 *  This attribute is read-only by the OLT.
	 */
	uint64_t t_rx_frames;

	/** Received XGEM frames with XGEM header HEC errors
	 *
	 *  The counter aggregated across all XGEM ports of the given ONU
	 *  identifies the number of loss XGEM frame delineation events.
	 *  This attribute is read-only by the OLT.
	 */
	uint64_t rx_frames_hdr_hec_err;

	/** FS words lost to XGEM header HEC errors
	 *
	 *  The counter of the FS frame words lost due to XGEM frame header
	 *  errors that cause loss of XGEM frame delineation.
	 *  This attribute is read-only by the OLT.
	 */
	uint64_t fs_words_lost_hdr_hec_err;

	/** XGEM encryption key errors
	 *
	 *  The counter aggregated across all XGEM ports of the given ONU
	 *  identifies the number of received XGEM frames that have to be
	 *  discarded because of unknown or invalid encryption key. The number
	 *  is included into the Total received XGEM frame count above.
	 *  This attribute is read-only by the OLT.
	 */
	uint64_t encrypt_key_err;

	/** Total transmitted bytes in non-idle XGEM frames
	 *
	 *  The counter aggregated across all XGEM ports of the given.
	 *  This attribute is read-only by the OLT.
	 */
	uint64_t t_tx_bytes_non_idle_frames;

	/** Total received bytes in non-idle XGEM frames
	 *
	 *  The counter aggregated across all XGEM ports of the given ONU.
	 *  This attribute is read-only by the OLT.
	 */
	uint64_t t_rx_bytes_non_idle_frames;
};

struct pa_twdm_channel_xgem_pmhd_ops {
	/**
	 * Get XGEM TC counters
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[in] me_id Managed Entity ID
	 * \param[out] param Pointer to a structure as defined
	 * by \ref pa_twdm_channel_xgem_pmhd.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*cnt_get)(void *ll_handle, uint16_t me_id,
			    struct pa_twdm_channel_xgem_pmhd *props);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
