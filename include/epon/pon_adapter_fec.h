/******************************************************************************
 *
 *  Copyright (c) 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_fec.h
 *
 * This is the PON adapter header file for FEC attribute related functions.
 */

#ifndef _PON_ADAPTER_FEC_H
#define _PON_ADAPTER_FEC_H

#include "pon_adapter_base.h"
#include "pon_adapter_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 * @{
 */

/** \defgroup PON_ADAPTER_FEC FEC control attribute related functions
 *  @{
 */

/** Enumeration for FEC mode values. */
enum epon_fec_values {
	/** FEC is disabled (get) or to be disabled (set). */
	EPON_FEC_DISABLED = 0,
	/** FEC is enabled (get) or to be enabled (set). */
	EPON_FEC_ENABLED = 1
};

/** EPON low-level FEC key operations */
struct epon_fec_ops {
	/**
	 * \brief Set EPON FEC mode
	 *
	 * \param[in] ll_handle      Lower layer context pointer
	 * \param[in] FEC_mode       FEC mode to be set
	 * - 0: disable FEC
	 * - 1: enable FEC
	 */
	enum pon_adapter_errno (*fec_mode_set)(void *ll_handle,
					     const uint8_t fec_mode);

	/**
	 * \brief Get EPON FEC mode
	 *
	 * \param[in] ll_handle      Lower layer context pointer
	 * \param[out] fec_mode       FEC mode to be get
	 * - 0: FEC is disabled
	 * - 1: FEC is enabled
	 */
	enum pon_adapter_errno (*fec_mode_get)(void *ll_handle,
					     uint8_t *fec_mode);
};

/** @} */ /* PON_ADAPTER_FEC */

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif
