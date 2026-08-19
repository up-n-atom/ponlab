/******************************************************************************
 *
 * Copyright (c) 2021 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 * This file holds definitions for mapping between OMCI Managed Entity assets.
 */

#ifndef _FAPI_PON_MAPPER_TABLE_H_
#define _FAPI_PON_MAPPER_TABLE_H_

#include "pon_adapter_mapper.h"

 /* Forward declaration */
struct fapi_pon_wrapper_ctx;

/** Mapper ID type definition */
enum mapper_id_type {
	/** Mapper ID type minimum value */
	MAPPER_IDTYPE_MIN = 0,
	/** Mapper from GEM Port Network CTP Managed Entity ID to GEM port ID */
	MAPPER_GEMPORTCTP_MEID_TO_ID = MAPPER_IDTYPE_MIN,
	/** Mapper ID type maximum value */
	MAPPER_IDTYPE_MAX
};

/** Initialize all mappings
 *
 * \param[in] ctx     Wrapper context.
 */
enum pon_adapter_errno pon_pa_mapper_init(struct fapi_pon_wrapper_ctx *ctx);

/** Shutdown all mappings
 *
 * \param[in] ctx     Wrapper context.
 */
enum pon_adapter_errno pon_pa_mapper_shutdown(struct fapi_pon_wrapper_ctx *ctx);

#endif
