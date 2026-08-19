/******************************************************************************
 *
 *  Copyright (c) 2017 - 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_mpcp.h
 *
 * This is the PON adapter header file for EPON MPCP related operations.
 */

#ifndef _PON_ADAPTER_MPCP_H_
#define _PON_ADAPTER_MPCP_H_

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 * @{
 */

/** \defgroup PON_ADAPTER_MPCP MPCP Registration
 *  @{
 */

/** EPON low-level link request operations */
struct epon_mpcp_ops {
	/**
	 * \brief MPCP registration operation. Trigger MPCP registration
	 *
	 * \param[in] ll_handle      Lower layer context pointer
	 * \param[in] link_index     Index under which firmware stores LLID
	 *			     details
	 * \param[in] mac_sa	     MAC sources address
	 */
	enum pon_adapter_errno (*link_register)(void *ll_handle,
							uint32_t link_index,
							uint8_t mac_sa[6]);

	/**
	 * \brief MPCP reset to WAIT state
	 *
	 * \param[in] ll_handle      Lower layer context pointer
	 * \param[in] link_index     Index under which firmware stores LLID
	 * \param[in] mac_sa	     MAC sources address
	 */
	enum pon_adapter_errno (*link_reset)(void *ll_handle,
							uint32_t link_index,
							uint8_t mac_sa[6]);

	/**
	 * \brief MPCP link deregistration trigger
	 *
	 * \param[in] ll_handle      Lower layer context pointer
	 * \param[in] link_index     Index under which firmware stores LLID
	 * \param[in] mac_sa	     MAC sources address
	 */
	enum pon_adapter_errno (*link_unregister)(void *ll_handle,
							  uint32_t link_index,
							  uint8_t mac_sa[6]);
};

/** @} */ /* PON_ADAPTER_MPCP */

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif
