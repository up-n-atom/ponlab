/******************************************************************************
 *
 * Copyright (c) 2021 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

#include "pon_adapter.h"
#include "pon_adapter_debug.h"
#include "pon_adapter_mapper.h"

#include "fapi_pon_os.h"
#include "fapi_pon_pa_common.h"

/** Table of mapper types with details like name and ranges */
static const struct mapper_entry m_entry[] = {
	{
		.type = MAPPER_GEMPORTCTP_MEID_TO_ID,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.max_key[KEY_TYPE_INDEX] = 0xffff,
		.name = "GEM Port NW CTP -> MEID | GPID"
	},
};

enum pon_adapter_errno pon_pa_mapper_init(struct fapi_pon_wrapper_ctx *ctx)
{
	PON_STATIC_ASSERT(ARRAY_SIZE(ctx->mapper) ==
		ARRAY_SIZE(m_entry),
		mapper_array_matches_number_of_entries);

	return pa_mapper_init(ctx->mapper, ARRAY_SIZE(m_entry), m_entry);
}

enum pon_adapter_errno pon_pa_mapper_shutdown(struct fapi_pon_wrapper_ctx *ctx)
{
	return pa_mapper_shutdown(ctx->mapper, ARRAY_SIZE(m_entry));
}
