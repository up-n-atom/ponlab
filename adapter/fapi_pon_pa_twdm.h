/******************************************************************************
 *
 * Copyright (c) 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 *  \file fapi_pon_pa_twdm.h
 *  This is the PON TWDM WRAPPER header file.
 */

#ifndef _FAPI_PON_PA_TWDM_H_
#define _FAPI_PON_PA_TWDM_H_

#include <stdint.h>
#include "fapi_pon.h"
#include "fapi_pon_error.h"

struct pon_twdm_ops;

/**
 *	Select TWDM functions based on TWDM configuration.
 *
 *	\param[in] twdm_config_method TWDM configuration method.
 */
const struct pon_twdm_ops *pon_twdm_select_ops(uint8_t twdm_config_method);

/**
 *	Configure TWDM Upstream.
 *
 *	\param[in] ctx		PON wrapper context
 *	\param[in] pon_ctx	PON context
 *	\param[in] ch_id	PON channel_id
 */
enum fapi_pon_errorcode pon_twdm_write_us(struct fapi_pon_wrapper_ctx *ctx,
					  struct pon_ctx *pon_ctx,
					  const uint8_t ch_id);

/**
 *	Configure TWDM Downstream.
 *
 *	\param[in] ctx		PON wrapper context
 *	\param[in] pon_ctx	PON context
 *	\param[in] ch_id	PON channel_id
 */
enum fapi_pon_errorcode pon_twdm_write_ds(struct fapi_pon_wrapper_ctx *ctx,
					  struct pon_ctx *pon_ctx,
					  const uint8_t ch_id);

/**
 *	Configure TWDM Tuning.
 *
 *	\param[in] ctx		PON wrapper context
 *	\param[in] pon_ctx	PON context
 *	\param[in] ch_id	PON channel_id
 */
enum fapi_pon_errorcode pon_twdm_tuning(struct fapi_pon_wrapper_ctx *ctx,
					struct pon_ctx *pon_ctx,
					const uint8_t ch_id);

#endif /* _FAPI_PON_PA_TWDM_H_ */
