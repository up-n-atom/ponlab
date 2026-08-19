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
 * Physical Path Termination Point xDSL UNI.
 */

#ifndef _pon_adapter_pptp_xdsl_uni_h
#define _pon_adapter_pptp_xdsl_uni_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
   @{
*/

/** \defgroup PON_ADAPTER_ME_PPTP_XDSL_UNI PPTP XDSL UNI

    This Managed Entity represents the point where physical paths terminate on
    an xDSL CO modem (xTU-C). The xDSL Managed Entity family is used for ADSL
    and VDSL2 services. A legacy family of VDSL managed entities remains valid
    for [ITU-T G.993.1] VDSL, if needed. It is documented in [ITU-T G.983.2].

    @{
*/

/** PPTP XDSL UNI operations structure */
struct pa_pptp_xdsl_uni_ops {
	/** Enable Managed Entity identifier mapping to driver index
	 *  and initialize corresponding driver structures
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

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
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
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
