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
 * This is a PON Adapter OMCI Managed Entity header file for the
 * TWDM System Profile.
 */

#ifndef _pon_adapter_me_twdm_system_profile_h
#define _pon_adapter_me_twdm_system_profile_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_TWDM_SYSTEM_PROFILE TWDM SYSTEM PROFILE
 *
 *  This Managed Entity contains central information related to the handling of
 *  a TWDM system. The ONU automatically creates an instance of this Managed
 *  Entity. Its attributes are populated according to the data within the ONU
 *  itself.
 *
 *  @{
 */

/** Structure of the data needed by the TWDM System Profile from the lower
  *  layer to initialize or update the attributes values
  */
struct pa_twdm_system_profile_data {
	/** Total TWDM Channel Number */
	uint8_t total_twdm_ch_num;

	/** Channel Partition Index */
	uint8_t ch_part_index;

	/** Channel Partition Waiver Timer */
	uint32_t ch_part_waiv_timer;

	/** LODS Re-initialization Timer (TO2) */
	uint32_t lods_reinit_timer;

	/** LODS Protection Timer (TO3) */
	uint32_t lods_prot_timer;

	/** Downstream Tuning Timer (TO4) */
	uint32_t ds_tuning_timer;

	/** Upstream Tuning Timer (TO5) */
	uint32_t us_tuning_timer;
};

/** TWDM System Profile operations structure */
struct pa_twdm_system_profile_ops {
	/** Updates TWDM System Profile Managed Entity resources
	 *
	 * \param[in] ll_handle    Lower layer context pointer
	 * \param[in] me_id        Managed Entity identifier
	 * \param[in] update_data  Pointer to TWDM Sys Profile update structure
	 */
	enum pon_adapter_errno (*update)(void *ll_handle,
		uint16_t me_id,
		const struct pa_twdm_system_profile_data *update_data);

	/** Gets TWDM System Profile Managed Entity current values
	 *
	 * \param[in] ll_handle     Lower layer context pointer
	 * \param[out] current_data Pointer to TWDM System Profile current data
	 * structure
	 */
	enum pon_adapter_errno (*current_data_get)(void *ll_handle,
		struct pa_twdm_system_profile_data *current_data);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
