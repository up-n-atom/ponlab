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
 * Physical Path Termination Point LCT UNI.
 */

#ifndef _pon_adapter_me_pptp_lct_uni
#define _pon_adapter_me_pptp_lct_uni

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 * @{
 */

/** \defgroup PON_ADAPTER_ME_PPTP_LCT_UNI LCT UNI
 *
 * This Managed Entity models debug access to the ONU from any physical or
 * logical port, for example via a dedicated local craft terminal UNI, via
 * ordinary subscriber UNIs, or via the IP host config Managed Entity.
 *
 * @{
 */

/** PPTP LCT UNI operations structure */
struct pa_pptp_lct_uni_ops {
	/** Create PPTP LCT UNI Managed Entity resources
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*create)(void *ll_handle,
					 uint16_t me_id);

	/** Destroy PPTP LCT UNI Managed Entity resources
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle,
					  uint16_t me_id);

	/** Update PPTP LCT UNI Managed Entity resources
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 * \param[in] admin_state          Administrative state
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*update)(void *ll_handle,
					 uint16_t me_id,
					 const uint8_t admin_state);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
