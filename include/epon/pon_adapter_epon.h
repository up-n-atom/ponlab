/******************************************************************************
 *
 *  Copyright (c) 2017 - 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_epon.h
 *
 * This is the PON adapter header file for structure containing all EPON
 * operations pointers.
 */

#ifndef _PON_ADAPTER_EPON_H_
#define _PON_ADAPTER_EPON_H_

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 *  @{
 */

/** \defgroup PON_ADAPTER_EPON EPON handlers
 *  These functions are used to handle EPON operations.
 *  @{
 */

/** Structure containing all EPON operations pointers */
struct pa_epon_ops {
	const struct epon_mpcp_ops *mpcp;
	const struct epon_crypt_ops *crypt;
	const struct epon_fec_ops *fec;
	const struct epon_qos_ops *qos;
};

/** @} */ /* PON_ADAPTER_EPON */

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif
