/******************************************************************************
 *
 *  Copyright (c) 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the TWDM Channel.
 */

#ifndef _pon_adapter_twdm_channel_h
#define _pon_adapter_twdm_channel_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_TWDM_CHANNEL TWDM Channel
 *
 *  This Managed Entity contains information related to a TWDM channel. The ONU
 *  automatically creates an instance of this Managed Entity. Its attributes are
 *  populated according to the data within the ONU itself.
 *
 *  @{
 */

/** TWDM Channel operations structure */
struct pa_twdm_channel_ops {
	/** Checks if the given wavelength channel is active
	 *
	 * \param[in] ll_handle     Lower layer context pointer
	 * \param[in] wlch_id       Wavelength channel ID
	 * \param[out] is_ch_active Pointer to Active Channel Indication
	 */
	enum pon_adapter_errno (*is_ch_active_get)(void *ll_handle,
						   uint8_t wlch_id,
						   bool *is_ch_active);

	/** Gets operational upstream and operational downstream channel
	 *
	 * \param[in] ll_handle    Lower layer context pointer
	 * \param[out] ds_ch_index Pointer to Downstream Wavelength Channel val
	 * \param[out] us_ch_index Pointer to Upstream Wavelength Channel val
	 */
	enum pon_adapter_errno (*operational_ch_get)(void *ll_handle,
						     uint8_t *ds_ch_index,
						     uint8_t *us_ch_index);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
