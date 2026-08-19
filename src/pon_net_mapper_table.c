/******************************************************************************
 *
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

#include "pon_adapter_mapper.h"
#include "pon_net_mapper_table.h"
#include "pon_net_common.h"
#include "pon_net_debug.h"

/** Mapper items description */
static const struct mapper_entry m_entry[] = {
	{
		.type = MAPPER_MACBRIDGE_MEID_TO_IDX,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.max_key[KEY_TYPE_INDEX] = 0xffffffff,
		.name = "MAC Bridge Configuration ME ID -> index"
	},
	{
		.type = MAPPER_MACBRIDGEPORT_MEID_TO_IDX,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.max_key[KEY_TYPE_INDEX] = 0xffffffff,
		.name = "MAC Bridge Port Configuration Data ME ID -> IDX"
	},
	{
		.type = MAPPER_TCONT_MEID_TO_ALLOC_ID,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.max_key[KEY_TYPE_INDEX] = 0xffff,
		.name = "T-CONT -> ALLOC ID"
	},
	{
		.type = MAPPER_GEMPORTCTP_MEID_TO_ID,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.max_key[KEY_TYPE_INDEX] = 0xffffffff,
		.name = "GEM Port NW CTP -> MEID | GPID"
	},
	{
		.type = MAPPER_PPTPETHERNETUNI_MEID_TO_IDX,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.max_key[KEY_TYPE_INDEX] = 0xffffffff,
		.name = "PPTP Ethernet Uni ME ID -> IDX"
	},
	{
		.type = MAPPER_MACBRIDGEPORT_MEID_TO_TP,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.max_key[KEY_TYPE_INDEX] = 0xffffffff,
		.name = "Mac Bridge Port Configuration Data ME ID -> TP"
	},
	{
		.type = MAPPER_GEMPORTITP_MEID_TO_ID,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.max_key[KEY_TYPE_INDEX] = 0xffffffff,
		.name = "GEM Port ITP -> MEID | GPID"
	},
	{
		.type = MAPPER_MCGEMPORTITP_MEID_TO_CTP_MEID,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.max_key[KEY_TYPE_INDEX] = 0xffffffff,
		.name = "MC GEM Port ITP -> MEID | GEM Port NW CTP -> MEID"
	},
	{
		.type = MAPPER_TCONT_MEID_TO_TS_MEID,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.max_key[KEY_TYPE_INDEX] = 0xffffffff,
		.name = "Traffic Scheduler MEID -> T-CONT MEID"
	},
	{
		.type = MAPPER_PMAPPER_MEID_TO_IDX,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.max_key[KEY_TYPE_INDEX] = 0xffffffff,
		.name = "802.1p Mapper Service Profile ME ID -> TP"
	},
	{
		.type = MAPPER_TCONT_PRI_QUEUE,
		.max_key[KEY_TYPE_ID] = 0xffffffff,
		.max_key[KEY_TYPE_INDEX] = 0xffff,
		.name = "T-CONT+Queue number -> GEM Port Id"
	},
	{
		.type = MAPPER_PPTPETHERNETUNI_PRI_QUEUE,
		.max_key[KEY_TYPE_ID] = 0xffffffff,
		.max_key[KEY_TYPE_INDEX] = 0xffff,
		.name = "PPTP Ethernet Uni+Queue number -> GEM Port Id"
	},
	{
		.type = MAPPER_EXTVLAN_MEID_TO_IDX,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.max_key[KEY_TYPE_INDEX] = 0xff,
		.name = "Extended VLAN Tagging Operation Config Data -> IDX"
	},
	{
		.type = MAPPER_VEIP_MEID_TO_IDX,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.max_key[KEY_TYPE_INDEX] = 0xffffffff,
		.name = "Virtual Ethernet Interface Point ME ID -> IDX"
	},
	{
		.type = MAPPER_PRIORITYQUEUE_MEID_TO_IDX,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.max_key[KEY_TYPE_INDEX] = 0xfff,
		.name = "Priority Queue ME ID -> IDX"
	},
	{
		.type = MAPPER_IPHOST_MEID_TO_IDX,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.min_key[KEY_TYPE_INDEX] = 0x1,
		.max_key[KEY_TYPE_INDEX] = 0xff,
		.name = "IP Host ME ID -> IDX"
	},
	{
		.type = MAPPER_IPV6HOST_MEID_TO_IDX,
		.max_key[KEY_TYPE_ID] = 0xffff,
		.min_key[KEY_TYPE_INDEX] = 0x1,
		.max_key[KEY_TYPE_INDEX] = 0xff,
		.name = "IPv6 Host ME ID -> IDX"
	},
};

enum pon_adapter_errno pon_net_pa_mapper_init(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ctx);

	PON_NET_STATIC_ASSERT(ARRAY_SIZE(ctx->mapper) ==
		ARRAY_SIZE(m_entry),
		mapper_array_matches_number_of_entries);

	ret = pa_mapper_init(ctx->mapper, ARRAY_SIZE(m_entry), m_entry);
	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_net_pa_mapper_reset(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ctx);
	ret = pa_mapper_reset(ctx->mapper, ARRAY_SIZE(m_entry));
	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_net_pa_mapper_shutdown(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ctx);
	ret = pa_mapper_shutdown(ctx->mapper, ARRAY_SIZE(m_entry));
	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_net_pa_mapper_dump(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ctx);
	if (ctx == NULL) {
		dbg_err("pon_net_context not found");
		dbg_out_ret("%d", PON_ADAPTER_ERR_PTR_INVALID);
		return PON_ADAPTER_ERR_PTR_INVALID;
	}

	ret = pa_mapper_dump(ctx->mapper, ARRAY_SIZE(m_entry), m_entry);
	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_net_pa_mapper_index_get_uint16(struct mapper *m,
							  uint32_t id,
							  uint16_t *idx)
{
	enum pon_adapter_errno ret;
	uint32_t idx_uint32;

	dbg_in_args("%p, %u, %p", m, id, idx);
	ret = mapper_index_get(m, id, &idx_uint32);
	if (ret == PON_ADAPTER_SUCCESS)
		*idx = (uint16_t) idx_uint32;
	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_net_pa_mapper_id_get_uint16(struct mapper *m,
						       uint16_t *id,
						       uint32_t idx)
{
	enum pon_adapter_errno ret;
	uint32_t id_uint32;

	dbg_in_args("%p, %p, %u", m, id, idx);
	ret = mapper_id_get(m, &id_uint32, idx);
	if (ret == PON_ADAPTER_SUCCESS)
		*id = (uint16_t) id_uint32;
	dbg_out_ret("%d", ret);
	return ret;
}
