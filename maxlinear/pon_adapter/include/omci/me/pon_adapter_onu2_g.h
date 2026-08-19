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
 * ONU2-G.
 */

#ifndef _pon_adapter_me_onu2_g_h
#define _pon_adapter_me_onu2_g_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_ONU2_G ONU2-G
 *
 *  This Managed Entity contains additional attributes associated with the
 *  ONU. The ONU automatically creates an instance of this Managed Entity. Its
 *  attributes are populated according to the data within the ONU itself.
 *
 *  @{
 */

 /** ONU2-G update structure */
struct pa_onu2_g_update {
	/** Security mode */
	uint8_t security_mode;
	/** Current connectivity mode */
	uint8_t current_connectivity_mode;
	/** Priority queue scale factor */
	uint16_t priority_queue_scale_factor;
};

/** ONU2-G operations structure */
struct pa_onu2_g_ops {
	/** Updates ONU-G Managed Entity resources
	 *
	 * \param[in] ll_handle    Lower layer context pointer
	 * \param[in] me_id	   Managed Entity identifier
	 * \param[in] update_data  Pointer to ONU2 update structure
	 */
	enum pon_adapter_errno (*update)(void *ll_handle,
					 uint16_t me_id,
					 struct pa_onu2_g_update *update_data);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 * \param[in] ll_handle  Lower layer context pointer
	 * \param[in] me_id      Managed Entity identifier
	 *
	 * \remark me_id is not used by device (therefore no mapping)
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle,
					  uint16_t me_id);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
