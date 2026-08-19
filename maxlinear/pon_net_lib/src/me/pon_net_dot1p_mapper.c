/*****************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <netlink/route/link.h>
#include <netlink/route/link/pmapper.h>
#include <pon_adapter.h>

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "../pon_net_netlink.h"
#include "../pon_net_uni.h"
#include "../pon_net_dev_db.h"
#include "../pon_net_dev_defs.h"
#include <omci/me/pon_adapter_dot1p_mapper.h>
#include <omci/me/pon_adapter_gem_port_network_ctp.h>
#include "pon_net_ext_vlan.h"
#include "pon_net_gem_port_network_ctp.h"

/** Translates many IDs using mapper
 *
 * \param[in]  mapper      Mapper instance
 * \param[in]  in_ids      Array of input IDs of size count
 * \param[in]  out_ids     Array of output IDs of size count
 * \param[in]  reverse     If 0, use mapper_index_get to translate in_ids
 *                         to out_ids. Otherwise use mapper_id_get
 * \param[in]  name_in     Name describing input IDs used for debugging purposes
 * \param[in]  name_out    Name describing output IDs used for debugging
 *                         purposes
 */
static enum pon_adapter_errno
translate_ids(struct mapper *mapper, const uint16_t *in_ids,
	      uint16_t *out_ids, size_t count,
	      int reverse,
	      const char *name_in,
	      const char *name_out)
{
	enum pon_adapter_errno ret;
	unsigned int i = 0;

	dbg_in_args("%p, %p, %p, %zu, %d, \"%s\", \"%s\"", mapper, in_ids,
		    out_ids, count, reverse, name_in, name_out);

	for (i = 0; i < count; i++) {
		uint32_t out_id;

		/**
		 * 0xFFFF is a NULL pointer, as specified in OMCI.
		 * This will also work for GEM Port IDs since it ranges
		 * from 0 to 4096
		 */
		if (in_ids[i] == 0xFFFF) {
			out_ids[i] = in_ids[i];
			continue;
		}

		ret = !reverse ? mapper_index_get(mapper, in_ids[i], &out_id) :
			mapper_id_get(mapper, &out_id, in_ids[i]);
		if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
			dbg_wrn("Can't find %s for %s with id %u\n",
				name_out, name_in, in_ids[i]);
			out_ids[i] = 0xFFFF;
			continue;
		}
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, mapper_index_get, PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
		out_ids[i] = (uint16_t)out_id;
		dbg_msg("Found %s with id %u for %s with id %u\n", name_out,
			out_ids[i], name_in, in_ids[i]);
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/**
 * This function counts the number of "GEM Interworking TP Managed Entities"
 * which are assigned to the 802.1p Mapper Managed Entity.
 * It checks the "Interwork TP Pointer" attributes if they are other than
 * 0xFFFF (unassigned).
 */
static int count_valid_pcps(uint16_t *pcps, size_t count)
{
	unsigned int i = 0;
	int result = 0;

	dbg_in_args("%p, %zu", pcps, count);

	for (i = 0; i < count; ++i)
		if (pcps[i] != 0xFFFF)
			result++;

	dbg_out_ret("%d", result);
	return result;
}

/* Returns true if a change in PCP mapping needs Pmapper to be reconnected to
 * the bridge (driver limitation) */
static bool needs_reconnect(uint16_t *old_pcps, uint16_t *new_pcps,
			    size_t count)
{
	unsigned int i = 0;
	unsigned int overlapping = 0;

	dbg_in_args("%p, %p %zu", old_pcps, new_pcps, count);

	for (i = 0; i < count; ++i) {
		if (old_pcps[i] != 0xFFFF && new_pcps[i] != 0xFFFF)
			overlapping++;

		/* If two entries overlap, it means that there will be always
		 * one GEM in Pmapper, therefore no need to reconnect */
		if (overlapping >= 2)
			return false;
	}

	dbg_out_ret("%d", true);
	return true;
}

static enum pon_adapter_errno pmapper_create(struct pon_net_context *ctx,
					     uint16_t me_id)
{
	enum pon_adapter_errno ret;
	char ifname[IF_NAMESIZE];
	int written;

	dbg_in_args("%p, %u", ctx, me_id);

	written = snprintf(ifname, sizeof(ifname), PON_IFNAME_PMAPPER, me_id);
	if (written >= (int)sizeof(ifname)) {
		FN_ERR_RET(written, snprintf, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = pon_net_dev_db_add(ctx->db, ifname, &pon_net_pmapper_def);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_add, ret);
		return ret;
	}

	ret = netlink_pmapper_create(ctx->netlink, me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		pon_net_dev_db_del(ctx->db, ifname);
		FN_ERR_RET(ret, netlink_pmapper_create, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno pmapper_change(struct pon_net_context *ctx,
					     uint16_t pmapper_idx,
					     uint8_t default_pcp,
					     uint16_t *new_gem_ctp_me_ids,
					     uint8_t *dscp_to_pbit)
{
	enum pon_adapter_errno ret;
	int bring_up = 0;

	dbg_in_args("%p, %u, %u, %p, %p", ctx, pmapper_idx, default_pcp,
		    new_gem_ctp_me_ids, dscp_to_pbit);

	ret = netlink_pmapper_setup(ctx->netlink, pmapper_idx, default_pcp,
				    new_gem_ctp_me_ids, dscp_to_pbit);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_pmapper_setup, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	bring_up = count_valid_pcps(new_gem_ctp_me_ids,
				    PON_8021P_PCP_COUNT) > 0;
	ret = netlink_pmapper_state_set(ctx->netlink, pmapper_idx, bring_up);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_pmapper_state_set, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno pmapper_destroy(struct pon_net_context *ctx,
					      uint16_t me_id,
					      uint16_t *gem_ctp_me_ids)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE];
	int written;

	dbg_in_args("%p, %u, %p", ctx, me_id, gem_ctp_me_ids);

	written = snprintf(ifname, sizeof(ifname), PON_IFNAME_PMAPPER, me_id);
	if (written >= (int)sizeof(ifname)) {
		FN_ERR_RET(written, snprintf, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = netlink_pmapper_destroy(ctx->netlink, me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_pmapper_destroy, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	pon_net_dev_db_del(ctx->db, ifname);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static bool dscp_all_zeros(uint8_t *dscp)
{
	unsigned int i;

	dbg_in_args("%p", dscp);

	for (i = 0; i < DSCP_MAX; ++i) {
		if (dscp[i])  {
			dbg_out_ret("%d", false);
			return false;
		}
	}

	dbg_out_ret("%d", true);
	return true;
}

/** 802.1p Mapper Service Profile Managed Entity update() implementation */
static enum pon_adapter_errno update(void *ll_handle, const uint16_t me_id,
			  const struct pa_dot1p_mapper_update_data *update_data)
{
	enum pon_adapter_errno ret;
	struct pon_net_context *ctx = ll_handle;
	struct pmapper pmapper = { 0 };
	unsigned int i = 0;
	uint16_t gem_ctp_me_ids[PON_8021P_PCP_COUNT];
	uint16_t gem_ids[PON_8021P_PCP_COUNT];
	uint16_t pmapper_idx;
	uint8_t dscp_to_pbit[DSCP_MAX];
	bool use_dscp = false;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, update_data);

	/* If "Unmarked frame option" indicates that we should use dscp and
	 * if the called has provided the DSCP map, then we will try to
	 * configure DSCP to Pbit mapping */
	if (update_data->unmarked_frame_opt == PA_DOT1P_MAPPER_PBIT_FROM_DSCP &&
	    update_data->dscp_prio_map) {
		use_dscp = true;
		pon_net_dscp_convert(update_data->dscp_prio_map, dscp_to_pbit);

		/* If OLT deos not set any mapping then we will use
		 * PCP only mode. We do this because the HW only supports
		 * DSCP only mode */
		if (dscp_all_zeros(dscp_to_pbit))
			use_dscp = false;
	}

	for (i = 0; i < ARRAY_SIZE(pmapper.gem_ctp_me_ids); ++i)
		pmapper.gem_ctp_me_ids[i] = 0xFFFF;

	/* We have GEM Port Interworking TP ME IDs, but we need
	   GEM Port CTP ME IDs, so first we need to translate
	   GEM Port Interworking TP ME IDs to GEM Port Ids */
	ret = translate_ids(ctx->mapper[MAPPER_GEMPORTITP_MEID_TO_ID],
			    update_data->gem_port_id,
			    gem_ids,
			    PON_8021P_PCP_COUNT,
			    0,
			    "GEM Port Interworking TP",
			    "GEM Port Id");
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, translate_ids, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	/* And then, we need to translate GEM Port Ids to
	   GEM Port CTP ME IDs */
	ret = translate_ids(ctx->mapper[MAPPER_GEMPORTCTP_MEID_TO_ID],
			    gem_ids,
			    gem_ctp_me_ids,
			    PON_8021P_PCP_COUNT,
			    1,
			    "GEM Port Id",
			    "GEM Port CTP");
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, translate_ids, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_PMAPPER, me_id,
			     &pmapper, sizeof(pmapper));

	ret = pon_net_gem_port_net_ctp_pmapper_assign(ctx,
						      pmapper.gem_ctp_me_ids,
						      false);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_gem_port_net_ctp_pmapper_assign, ret);
		return ret;
	}

	ret = pon_net_pa_mapper_index_get_uint16(
				      ctx->mapper[MAPPER_PMAPPER_MEID_TO_IDX],
				      me_id,
				      &pmapper_idx);
	if (ret == PON_ADAPTER_ERROR) {
		FN_ERR_RET(ret, pon_net_pa_mapper_index_get_uint16,
			   PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		/* We could use mapper_id_map here to generate pmapper_idx,
		   but instead we perform (x)->x mapping, so that pmapper0
		   network device would be associated with 802.1p Mapper Service
		   Profile ME ID 0, pmapper1 with ME ID 1, etc */
		pmapper_idx = me_id;
		ret = mapper_explicit_map(
				ctx->mapper[MAPPER_PMAPPER_MEID_TO_IDX],
				me_id,
				pmapper_idx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, mapper_explicit_map, PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		ret = pmapper_create(ctx, me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, create, ret);
			mapper_id_remove(
				ctx->mapper[MAPPER_PMAPPER_MEID_TO_IDX],
				me_id);
			return PON_ADAPTER_ERROR;
		}
	}

	/* If we had 0 or more PCPs mapped, then when driver will apply the
	 * changes, there can be a situation that during application of these
	 * changed PMapper has no gems and it is in a bridge
	 * (which is not supported). This can only
	 * happen if we have 0 or 1 PCPs mapped.
	 * To fix this situation, we need to reconnect the Pmapper.
	 * The MAC Bridge Port Config data implementation will reconnect
	 * the Pmapper if needs_reconnect is true */
	pmapper.needs_reconnect =
	    needs_reconnect(pmapper.gem_ctp_me_ids, gem_ctp_me_ids,
			    ARRAY_SIZE(pmapper.gem_ctp_me_ids));

	ret = pmapper_change(ctx, pmapper_idx,
			     update_data->default_priority_marking,
			     gem_ctp_me_ids, use_dscp ? dscp_to_pbit : NULL);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, change, ret);
		return ret;
	}

	if (memcpy_s(pmapper.gem_ctp_me_ids, sizeof(pmapper.gem_ctp_me_ids),
			gem_ctp_me_ids, sizeof(gem_ctp_me_ids))) {
		dbg_err_fn(memcpy_s);
		dbg_out_ret("%d", PON_ADAPTER_ERR_MEM_ACCESS);
		return PON_ADAPTER_ERR_MEM_ACCESS;
	}

	ret = pon_net_gem_port_net_ctp_pmapper_assign(ctx,
						      pmapper.gem_ctp_me_ids,
						      true);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_gem_port_net_ctp_pmapper_assign, ret);
		return ret;
	}

	/* Save GEM Port Network CTP */
	ret = pon_net_me_list_write(&ctx->me_list, PON_CLASS_ID_PMAPPER,
				    me_id, &pmapper, sizeof(pmapper));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/** 802.1p Mapper Service Profile Managed Entity destroy() implementation */
static enum pon_adapter_errno destroy(void *ll_handle, const uint16_t me_id,
	const struct pa_dot1p_mapper_destroy_data *destroy_data)
{
	struct pon_net_context *ctx = ll_handle;
	struct pmapper pmapper = { 0 };
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, destroy_data);


	ret = pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_PMAPPER,
				   me_id, &pmapper, sizeof(pmapper));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_read, ret);
		return ret;
	}

	mapper_id_remove(ctx->mapper[MAPPER_PMAPPER_MEID_TO_IDX], me_id);

	ret = pmapper_destroy(ctx, me_id, pmapper.gem_ctp_me_ids);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, destroy, ret);
		return ret;
	}

	ret = pon_net_gem_port_net_ctp_pmapper_assign(ctx,
						      pmapper.gem_ctp_me_ids,
						      false);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_gem_port_net_ctp_pmapper_assign, ret);
		return ret;
	}

	pon_net_me_list_remove(&ctx->me_list, PON_CLASS_ID_PMAPPER, me_id);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_dot1p_mapper_ops pon_net_dot1p_mapper_ops = {
	.update = update,
	.destroy = destroy
};
