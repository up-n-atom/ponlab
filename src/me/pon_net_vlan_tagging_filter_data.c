/*****************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>

#include <linux/if_ether.h>
#include <netlink/route/link.h>

#ifdef LINUX
#include <netinet/in.h>
#endif

#include <pon_adapter.h>

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "../pon_net_netlink.h"
#include "../pon_net_dev_db.h"
#include "../pon_net_dev_defs.h"
#include "../pon_net_vlan_flow.h"
#include "../pon_net_vlan_aware_bridging.h"
#include "../pon_net_tc_filter.h"
#include "pon_net_mac_bridge_port_config_data.h"
#include "pon_net_vlan_tagging_filter_data.h"
#include "pon_net_mac_bridge_port_filter_preassign_table.h"
#include <omci/me/pon_adapter_vlan_tagging_filter_data.h>
#include <omci/me/pon_adapter_mac_bridge_port_config_data.h>

/* This is an entry from VLAN Forwarding Operation Table */
struct vops {
	/* Index. This also known as forward_operation */
	uint32_t index:8;
	/*
	 * Mask which selects whether we are creating
	 * filter for VID or PCP
	 */
	uint16_t tci_mask;
	/* Enabled if all VLAN tagged frames should be passed */
	uint32_t tagged_pass_enable:1;
	/* Enabled if all VLAN tagged frames should be dropped */
	uint32_t tagged_drop_enable:1;
	/*
	 * Enabled if all VLAN tagged frames whose VLAN TCIs match
	 * the tci_list should pass. If disabled they will be dropped
	 * This takes tci_mask into account (if mask is only for PCP then
	 * only filters matching PCP are created)
	 */
	uint32_t pass_on_match_enable:1;
	/*
	 * Enabled if untagged frames should be passed, disabled if they
	 * should be dropped
	 */
	uint32_t untagged_bridging:1;
	/* Which kind of traffic this rule should be created for? */
	enum netlink_filter_dir direction;
};

/*
 * The prio of default rules should start from 65521
 * This is required by the driver
 */
#define DEFAULT_PRIO_START 65521

/*
 * Macro for building a filter handle from me_id and i
 * See PON_FILTER_HANDLE() macro definition for more details
 */
#define VLAN_FILTERING_HANDLE(me_id, i) \
	PON_FILTER_HANDLE(PON_FILTER_VLAN_FILTERING, \
			  (((uint32_t)(me_id)) << 10) | ((i) & 0x3FF))
#define MAX_FILTERS 1024

/*
 * Table of VLAN forwarding operations
 * Based on ITU G.984.4 Amendment 1, chapter 9.3.11 VLAN tagging filter data
 */
static const struct vops vops_g984[] = {
	{0x00, 0x0000, 1, 0, 1, 1, NETLINK_FILTER_DIR_NONE},
	{0x01, 0x0000, 0, 1, 0, 1, NETLINK_FILTER_DIR_BOTH},
	{0x02, 0x0000, 1, 0, 0, 0, NETLINK_FILTER_DIR_BOTH},
	{0x03, 0x0fff, 0, 0, 1, 1, NETLINK_FILTER_DIR_EGRESS},
	{0x04, 0x0fff, 0, 0, 1, 0, NETLINK_FILTER_DIR_EGRESS},
	{0x05, 0x0fff, 0, 0, 0, 1, NETLINK_FILTER_DIR_INGRESS},
	{0x06, 0x0fff, 0, 0, 0, 0, NETLINK_FILTER_DIR_INGRESS},
	{0x07, 0xe000, 0, 0, 1, 1, NETLINK_FILTER_DIR_EGRESS},
	{0x08, 0xe000, 0, 0, 1, 0, NETLINK_FILTER_DIR_EGRESS},
	{0x09, 0xe000, 0, 0, 0, 1, NETLINK_FILTER_DIR_INGRESS},
	{0x0a, 0xe000, 0, 0, 0, 0, NETLINK_FILTER_DIR_INGRESS},
	{0x0b, 0xffff, 0, 0, 1, 1, NETLINK_FILTER_DIR_EGRESS},
	{0x0c, 0xffff, 0, 0, 1, 0, NETLINK_FILTER_DIR_EGRESS},
	{0x0d, 0xffff, 0, 0, 0, 1, NETLINK_FILTER_DIR_INGRESS},
	{0x0e, 0xffff, 0, 0, 0, 0, NETLINK_FILTER_DIR_INGRESS},
	{0x0f, 0x0fff, 0, 0, 1, 1, NETLINK_FILTER_DIR_EGRESS},
	{0x10, 0x0fff, 0, 0, 1, 0, NETLINK_FILTER_DIR_EGRESS},
	{0x11, 0xe000, 0, 0, 1, 1, NETLINK_FILTER_DIR_EGRESS},
	{0x12, 0xe000, 0, 0, 1, 0, NETLINK_FILTER_DIR_EGRESS},
	{0x13, 0xffff, 0, 0, 1, 1, NETLINK_FILTER_DIR_EGRESS},
	{0x14, 0xffff, 0, 0, 1, 0, NETLINK_FILTER_DIR_EGRESS},
	{0x15, 0x0000, 1, 0, 1, 0, NETLINK_FILTER_DIR_EGRESS},
	{0x16, 0x0fff, 0, 0, 1, 1, NETLINK_FILTER_DIR_EGRESS},
	{0x17, 0x0fff, 0, 0, 1, 0, NETLINK_FILTER_DIR_EGRESS},
	{0x18, 0xe000, 0, 0, 1, 1, NETLINK_FILTER_DIR_EGRESS},
	{0x19, 0xe000, 0, 0, 1, 0, NETLINK_FILTER_DIR_EGRESS},
	{0x1a, 0xffff, 0, 0, 1, 1, NETLINK_FILTER_DIR_EGRESS},
	{0x1b, 0xffff, 0, 0, 1, 0, NETLINK_FILTER_DIR_EGRESS},
	{0x1c, 0x0fff, 0, 0, 1, 1, NETLINK_FILTER_DIR_BOTH},
	{0x1d, 0x0fff, 0, 0, 1, 0, NETLINK_FILTER_DIR_BOTH},
	{0x1e, 0xe000, 0, 0, 1, 1, NETLINK_FILTER_DIR_BOTH},
	{0x1f, 0xe000, 0, 0, 1, 0, NETLINK_FILTER_DIR_BOTH},
	{0x20, 0xffff, 0, 0, 1, 1, NETLINK_FILTER_DIR_BOTH},
	{0x21, 0xffff, 0, 0, 1, 0, NETLINK_FILTER_DIR_BOTH}
};

/*
 * Table of VLAN forwarding operations
 * Based on ITU-T G.988, chapter 9.3.11, VLAN tagging filter data,
 * Table 9.3.11-1
 */
static const struct vops vops_g988[] = {
	{0x00, 0x0000, 1, 0, 1, 1, NETLINK_FILTER_DIR_NONE},
	{0x01, 0x0000, 0, 1, 0, 1, NETLINK_FILTER_DIR_BOTH},
	{0x02, 0x0000, 1, 0, 0, 0, NETLINK_FILTER_DIR_BOTH},
	{0x03, 0x0fff, 0, 0, 1, 1, NETLINK_FILTER_DIR_BOTH},
	{0x04, 0x0fff, 0, 0, 1, 0, NETLINK_FILTER_DIR_BOTH},
	{0x05, 0x0fff, 0, 0, 0, 1, NETLINK_FILTER_DIR_INGRESS},
	{0x06, 0x0fff, 0, 0, 0, 0, NETLINK_FILTER_DIR_INGRESS},
	{0x07, 0xe000, 0, 0, 1, 1, NETLINK_FILTER_DIR_BOTH},
	{0x08, 0xe000, 0, 0, 1, 0, NETLINK_FILTER_DIR_BOTH},
	{0x09, 0xe000, 0, 0, 0, 1, NETLINK_FILTER_DIR_INGRESS},
	{0x0a, 0xe000, 0, 0, 0, 0, NETLINK_FILTER_DIR_INGRESS},
	{0x0b, 0xffff, 0, 0, 1, 1, NETLINK_FILTER_DIR_BOTH},
	{0x0c, 0xffff, 0, 0, 1, 0, NETLINK_FILTER_DIR_BOTH},
	{0x0d, 0xffff, 0, 0, 0, 1, NETLINK_FILTER_DIR_INGRESS},
	{0x0e, 0xffff, 0, 0, 0, 0, NETLINK_FILTER_DIR_INGRESS},
	{0x0f, 0x0fff, 0, 0, 1, 1, NETLINK_FILTER_DIR_BOTH},
	{0x10, 0x0fff, 0, 0, 1, 0, NETLINK_FILTER_DIR_BOTH},
	{0x11, 0xe000, 0, 0, 1, 1, NETLINK_FILTER_DIR_BOTH},
	{0x12, 0xe000, 0, 0, 1, 0, NETLINK_FILTER_DIR_BOTH},
	{0x13, 0xffff, 0, 0, 1, 1, NETLINK_FILTER_DIR_BOTH},
	{0x14, 0xffff, 0, 0, 1, 0, NETLINK_FILTER_DIR_BOTH},
	{0x15, 0x0000, 1, 0, 1, 0, NETLINK_FILTER_DIR_BOTH},
	{0x16, 0x0fff, 0, 0, 1, 1, NETLINK_FILTER_DIR_EGRESS},
	{0x17, 0x0fff, 0, 0, 1, 0, NETLINK_FILTER_DIR_EGRESS},
	{0x18, 0xe000, 0, 0, 1, 1, NETLINK_FILTER_DIR_EGRESS},
	{0x19, 0xe000, 0, 0, 1, 0, NETLINK_FILTER_DIR_EGRESS},
	{0x1a, 0xffff, 0, 0, 1, 1, NETLINK_FILTER_DIR_EGRESS},
	{0x1b, 0xffff, 0, 0, 1, 0, NETLINK_FILTER_DIR_EGRESS},
	{0x1c, 0x0fff, 0, 0, 1, 1, NETLINK_FILTER_DIR_BOTH},
	{0x1d, 0x0fff, 0, 0, 1, 0, NETLINK_FILTER_DIR_BOTH},
	{0x1e, 0xe000, 0, 0, 1, 1, NETLINK_FILTER_DIR_BOTH},
	{0x1f, 0xe000, 0, 0, 1, 0, NETLINK_FILTER_DIR_BOTH},
	{0x20, 0xffff, 0, 0, 1, 1, NETLINK_FILTER_DIR_BOTH},
	{0x21, 0xffff, 0, 0, 1, 0, NETLINK_FILTER_DIR_BOTH}
};

/*
 * ANI termination point types for which the meaning of ingress and egress
 * is inverted
 */
static const uint8_t ani_tp_types[] = {
	PA_BP_TP_TYPE_PMAP,
	PA_BP_TP_TYPE_GEM,
	PA_BP_TP_TYPE_MC_GEM
};

/* Most of the rules are created for both of these protocols */
static const uint16_t vlan_protocols[] = {
	ETH_P_8021Q,
	ETH_P_8021AD
};

/*
 * Number of filters that can be stored in the cache
 *
 * We can have two filters per tci, two filters for default treatment of
 * single tagged frames and one filter for default treatment of untagged frames,
 * and these filters can be in both directions
 */
#define CACHE_SIZE \
	(2 * (MAX_TCIS * ARRAY_SIZE(vlan_protocols) + \
	 ARRAY_SIZE(vlan_protocols) + 1))

/*
 * Filter cache stores information about filters that is necessary to delete
 * them without querying the kernel
 */
struct filter_cache {
	struct pon_net_tc_filter tc_filters[CACHE_SIZE];
	unsigned int count;
};

/* Structure representing a VLAN operation */
struct pon_net_vlan_tagging_operation {
	/* VLAN operation that is to be applied */
	struct vops vops;
	/* List of VLAN TCIs to be applied */
	uint16_t tci_list[MAX_TCIS];
	/* Length of 'tci_list' array */
	uint8_t tci_list_len;
};

/*
 * Data saved on the ctx->me_list, storing the information
 * about VLAN Tagging Filter Data ME
 */
struct pon_net_vlan_tagging {
	/* Pending VLAN tagging operation - to be applied to the system */
	struct pon_net_vlan_tagging_operation pending;
	/* Applied VLAN tagging operation - applied to the system */
	struct pon_net_vlan_tagging_operation applied;
	/* Filter cache - stores information about currently applied filters */
	struct filter_cache filter_cache;
	/* Interface name */
	char ifname[IF_NAMESIZE];
};

/* Compare two vops structures */
static bool vops_eq(const struct vops *a, const struct vops *b)
{
	return a->index == b->index &&
		a->tci_mask == b->tci_mask &&
		a->tagged_pass_enable == b->tagged_pass_enable &&
		a->tagged_drop_enable == b->tagged_drop_enable &&
		a->pass_on_match_enable == b->pass_on_match_enable &&
		a->untagged_bridging == b->untagged_bridging &&
		a->direction == b->direction;
}

/* Return prios and handles to the pool */
static void filter_cache_pool_return(struct filter_cache *cache,
				     struct pon_net_dev_db *db,
				     const char *ifname)
{
	unsigned int i = 0;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %p", cache, db, ifname);

	if (pon_net_dev_db_get(db, ifname) == NULL)
		goto out;

	for (i = 0; i < cache->count; ++i) {
		struct pon_net_tc_filter *filter = &cache->tc_filters[i];
		uint32_t prio = filter->prio;
		uint32_t handle = filter->handle;

		ret = pon_net_dev_db_put(db, ifname, filter->prio_pool_id,
					 &prio, 1);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ifname_ret(pon_net_dev_db_put, ifname, ret);

		ret = pon_net_dev_db_put(db, ifname,
					 filter->handle_pool_id, &handle, 1);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ifname_ret(pon_net_dev_db_put, ifname, ret);
	}

out:
	dbg_out();
}

/* Delete filters from the kernel and clear the cache */
static void filter_cache_delete_filters(struct filter_cache *cache,
					struct pon_net_context *ctx,
					const char *ifname)
{
	unsigned int i = 0;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %p", cache, ctx, ifname);

	filter_cache_pool_return(cache, ctx->db, ifname);

	for (i = 0; i < cache->count; ++i) {
		struct pon_net_tc_filter *filter = &cache->tc_filters[i];

		ret = netlink_filter_clear_one(ctx->netlink, ifname,
					       filter->proto, filter->prio,
					       filter->handle, filter->dir);
		if (ret != PON_ADAPTER_SUCCESS &&
		    ret != PON_ADAPTER_ERR_NOT_FOUND) {
			dbg_err_fn_ifname_ret(netlink_filter_clear_one, ifname,
					      ret);
		}
	}

	memset(cache, 0, sizeof(*cache));

	dbg_out();
}

/* Store filter in the cache - for later deletion */
static enum pon_adapter_errno
filter_cache_add(struct filter_cache *cache,
		 const struct netlink_filter *filter, unsigned int handle_pool,
		 unsigned int prio_pool)
{
	struct pon_net_tc_filter *tc_filter;

	dbg_in_args("%p, %p, %u, %u", cache, filter, handle_pool,
		    prio_pool);

	if (cache->count >= ARRAY_SIZE(cache->tc_filters)) {
		dbg_err("Filter cache is full (%zu)\n",
			ARRAY_SIZE(cache->tc_filters));
		return PON_ADAPTER_ERR_OUT_OF_BOUNDS;
	}

	tc_filter = &cache->tc_filters[cache->count];

	tc_filter->prio = (uint16_t)filter->prio;
	tc_filter->proto = (uint16_t)filter->proto;
	tc_filter->handle = filter->handle;
	tc_filter->dir = filter->dir;
	tc_filter->handle_pool_id = handle_pool;
	tc_filter->prio_pool_id = prio_pool;

	cache->count++;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static bool
pon_net_vlan_tagging_operation_eq(
				 const struct pon_net_vlan_tagging_operation *a,
				 const struct pon_net_vlan_tagging_operation *b)
{
	bool result;

	dbg_in_args("%p, %p", a, b);

	result = vops_eq(&a->vops, &b->vops) &&
		 pon_net_array_eq_u16(a->tci_list, a->tci_list_len, b->tci_list,
				      b->tci_list_len);

	dbg_out_ret("%d", result);
	return result;
}

static enum pon_adapter_errno
pon_net_vlan_tagging_operation_set(struct pon_net_vlan_tagging_operation *op,
				   struct vops *vops, const uint16_t *tci_list,
				   uint8_t tci_list_len)
{
	dbg_in_args("%p, %p, %p, %u", op, vops, tci_list,
		    tci_list_len);

	op->vops = *vops;
	if (memcpy_s(op->tci_list, sizeof(op->tci_list), tci_list,
		     tci_list_len * sizeof(*tci_list))) {
		dbg_err_fn(memcpy_s);
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	op->tci_list_len = tci_list_len;
	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Find vops for given forward_operation */
static const struct vops *find_vops(const struct vops *vops,
				    unsigned int num_vops,
				    uint8_t forward_operation)
{
	unsigned int i = 0;

	dbg_in_args("%p, %u, %u", vops, num_vops, forward_operation);

	for (i = 0; i < num_vops; ++i) {
		if (vops[i].index == forward_operation) {
			dbg_out_ret("%p", &vops[i]);
			return &vops[i];
		}
	}

	dbg_out_ret("%p", NULL);
	return NULL;
}

/* Just a wrapper for find_vops */
static enum pon_adapter_errno
forward_operation_to_vops(bool use_g988,
			  uint8_t forward_operation,
			  struct vops *vops_out)
{
	const struct vops *vops = NULL;

	dbg_in_args("%d, %u, %p", use_g988, forward_operation, vops_out);

	if (use_g988)
		vops = find_vops(vops_g988,
				 ARRAY_SIZE(vops_g988),
				 forward_operation);
	else
		vops = find_vops(vops_g984,
				 ARRAY_SIZE(vops_g984),
				 forward_operation);

	if (!vops) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_SUPPORTED);
		return PON_ADAPTER_ERR_NOT_SUPPORTED;
	}

	*vops_out = *vops;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Inverts direction */
static enum netlink_filter_dir invert_dir(enum netlink_filter_dir dir)
{
	dbg_in_args("%d", dir);

	switch (dir) {
	case NETLINK_FILTER_DIR_EGRESS:
		dbg_out_ret("%d", NETLINK_FILTER_DIR_INGRESS);
		return NETLINK_FILTER_DIR_INGRESS;
	case NETLINK_FILTER_DIR_INGRESS:
		dbg_out_ret("%d", NETLINK_FILTER_DIR_EGRESS);
		return NETLINK_FILTER_DIR_EGRESS;
	default:
		dbg_out_ret("%d", dir);
		return dir;
	}
}

/*
 * The meaning of ingress and egress is inverted if a bridge port is located
 * on the ANI side of a bridge. There is no difference if a bridge port is
 * located on the UNI side of a bridge
 */
static void
interpret_dir(struct vops *vops, uint8_t tp_type)
{
	unsigned int i;

	dbg_in_args("%p, %u", vops, tp_type);

	for (i = 0; i < ARRAY_SIZE(ani_tp_types); i++) {
		if (ani_tp_types[i] == tp_type) {
			vops->direction = invert_dir(vops->direction);
			break;
		}
	}

	dbg_out();
}

/* Sets filter to filter all single tagged frames */
static void
single_tagged_frames(struct netlink_filter *filter, uint16_t proto)
{
	dbg_in_args("%p, %u", filter, proto);

	filter->vlan_id = NETLINK_FILTER_UNUSED;
	filter->vlan_prio = NETLINK_FILTER_UNUSED;
	filter->vlan_proto = NETLINK_FILTER_UNUSED;
	filter->proto = proto;
	filter->act_data = NULL;

	dbg_out();
}

/* Sets filter to drop frame that matches tci masked by tci_mask */
static void matched_frame(struct netlink_filter *filter,
			  struct netlink_cookie *cookie,
			  uint16_t proto,
			  uint16_t tci_mask,
			  uint16_t tci)
{
	int ret;

	dbg_in_args("%p, %p, %u, %u, %u", filter, cookie, proto, tci_mask, tci);

	filter->vlan_id = (tci_mask & TCI_MASK_VID) ?
		TCI_VID(tci) : NETLINK_FILTER_UNUSED;
	filter->vlan_prio = (tci_mask & TCI_MASK_PCP) ?
		TCI_PCP(tci) : NETLINK_FILTER_UNUSED;
	filter->vlan_proto = NETLINK_FILTER_UNUSED;
	filter->proto = proto;

	/* The cookie for VLAN filtering is optional, it contains the full
	 * VLAN TCI value in the first 2 bytes followed by the full mask
	 * value in the byte 2 and 3.
	 */
	if (tci_mask & TCI_MASK_DEI) {
		uint16_t tci_mask_be = htons(tci_mask);
		uint16_t tci_be = htons(tci);

		ret = memcpy_s(cookie->cookie, sizeof(cookie->cookie),
			       &tci_be, sizeof(tci_be));
		if (ret) {
			dbg_err_fn_ret(memcpy_s, ret);
			dbg_out();
			return;
		}
		ret = memcpy_s(cookie->cookie + sizeof(tci),
			      sizeof(cookie->cookie) - sizeof(tci),
			      &tci_mask_be, sizeof(tci_mask_be));
		if (ret) {
			dbg_err_fn_ret(memcpy_s, ret);
			dbg_out();
			return;
		}
		filter->act_data = cookie;
	}

	dbg_out();
}

static void cleanup_unused_prios(struct pon_net_dev_db *db,
				 const char *ifname,
				 unsigned int pool,
				 unsigned int current_prio,
				 uint32_t *prios,
				 unsigned int num_prios)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\", %u, %u, %p, %u", db, ifname, pool,
		    current_prio, prios, num_prios);

	if (current_prio < num_prios) {
		unsigned int remaining = num_prios - current_prio;
		/* Put back unused priorities */
		ret = pon_net_dev_db_put(db, ifname, pool, prios + current_prio,
					 remaining);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ifname_ret(pon_net_dev_db_put, ifname, ret);
	}

	dbg_out();
}

static
enum pon_adapter_errno filter_add_dir(struct pon_net_context *ctx,
				  const char *ifname,
				  uint32_t *prios, unsigned int *current_prio,
				  unsigned int prio_pool,
				  unsigned int handle_pool,
				  struct filter_cache *cache,
				  struct netlink_filter *filter)
{
	enum pon_adapter_errno ret;
	uint32_t handle;

	dbg_in_args("%p, %p, %p, %p, %u, %u, %p, %p", ctx, ifname, prios,
		    current_prio, prio_pool, handle_pool, cache, filter);

	ret = pon_net_dev_db_gen(ctx->db, ifname, handle_pool, &handle, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_dev_db_gen, ret);
		dbg_out_ret("%d", ret);
		return ret;
	}

	filter->handle = handle;
	filter->prio = (int)prios[*current_prio];

	ret = filter_cache_add(cache, filter, handle_pool, prio_pool);
	if (ret) {
		dbg_err_fn_ret(filter_cache_add, ret);
		dbg_out_ret("%d", ret);
		return ret;
	}
	(*current_prio)++;

	ret = netlink_filter_add(ctx->netlink, filter);
	if (ret) {
		dbg_err_fn_ret(netlink_filter_add, ret);
		dbg_out_ret("%d", ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static
enum pon_adapter_errno filter_add(struct pon_net_context *ctx,
				  const char *ifname,
				  enum netlink_filter_dir direction,
				  uint32_t *ingress_prios,
				  unsigned int *current_ingress_prio,
				  unsigned int ingress_pool,
				  uint32_t *egress_prios,
				  unsigned int *current_egress_prio,
				  unsigned int egress_pool,
				  struct filter_cache *cache,
				  struct netlink_filter *filter)
{
	enum pon_adapter_errno ret;

	if (direction == NETLINK_FILTER_DIR_INGRESS ||
	    direction == NETLINK_FILTER_DIR_BOTH) {
		filter->dir = NETLINK_FILTER_DIR_INGRESS;
		ret = filter_add_dir(ctx, ifname, ingress_prios,
			current_ingress_prio, ingress_pool,
			PON_NET_HANDLE_FILTER_INGRESS,
			cache, filter);
		if (ret != PON_ADAPTER_SUCCESS)
			return ret;
	}
	if (direction == NETLINK_FILTER_DIR_EGRESS ||
	    direction == NETLINK_FILTER_DIR_BOTH) {
		filter->dir = NETLINK_FILTER_DIR_EGRESS;
		ret = filter_add_dir(ctx, ifname, egress_prios,
			current_egress_prio, egress_pool,
			PON_NET_HANDLE_FILTER_EGRESS,
			cache, filter);
		if (ret != PON_ADAPTER_SUCCESS)
			return ret;
	}
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
__update_filtering(struct pon_net_context *ctx, struct filter_cache *cache,
		   const struct vops *vops, const uint16_t *tci_list,
		   uint8_t tci_list_len, const char *ifname)
{
	struct netlink_filter filter = {0};
	struct netlink_cookie cookie = {0};
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	unsigned int i;
	unsigned int j;
	uint32_t ingress_prios[2 * MAX_TCIS];
	unsigned int current_ingress_prio = ARRAY_SIZE(ingress_prios);
	uint32_t egress_prios[2 * MAX_TCIS];
	unsigned int current_egress_prio = ARRAY_SIZE(egress_prios);
	unsigned int ingress_pool;
	unsigned int egress_pool;

	dbg_in_args("%p, %p, %p, %p, %u, \"%s\"", ctx, cache, vops, tci_list,
		    tci_list_len, ifname);

	netlink_filter_defaults(&filter);

	ingress_pool = PON_NET_PRIO_VLAN_FILTERING_INGRESS;
	egress_pool = PON_NET_PRIO_VLAN_FILTERING_EGRESS;

	if (strncpy_s(filter.device, sizeof(filter.device),
		  ifname, IF_NAMESIZE)) {
		dbg_err_fn(strncpy_s);
		ret = PON_ADAPTER_ERR_NO_DATA;
		goto err;
	}
	if (memcpy_s(filter.indev, sizeof(filter.indev),
			filter.device, sizeof(filter.indev))) {
		dbg_err_fn(memcpy_s);
		ret = PON_ADAPTER_ERR_NO_DATA;
		goto err;
	}

	/*
	 * We must allocate priorities up front to maintain appropriate
	 * filter ordering
	 */
	if (vops->direction == NETLINK_FILTER_DIR_INGRESS ||
	    vops->direction == NETLINK_FILTER_DIR_BOTH) {
		ret = pon_net_dev_db_gen(ctx->db, ifname, ingress_pool,
					 ingress_prios,
					 ARRAY_SIZE(ingress_prios));
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(pon_net_dev_db_gen, ret);
			goto err;
		}
		current_ingress_prio = 0;
	}
	if (vops->direction == NETLINK_FILTER_DIR_EGRESS ||
	    vops->direction == NETLINK_FILTER_DIR_BOTH) {
		ret = pon_net_dev_db_gen(ctx->db, ifname, egress_pool,
					 egress_prios,
					 ARRAY_SIZE(egress_prios));
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(pon_net_dev_db_gen, ret);
			goto err;
		}
		current_egress_prio = 0;
	}

	for (i = 0; i < tci_list_len; ++i) {
		for (j = 0; j < ARRAY_SIZE(vlan_protocols); ++j) {
			if (vops->pass_on_match_enable)
				filter.act = NETLINK_FILTER_ACT_OK;
			else
				filter.act = NETLINK_FILTER_ACT_DROP;
			matched_frame(&filter, &cookie,
				      vlan_protocols[j],
				      vops->tci_mask,
				      tci_list[i]);
			ret = filter_add(ctx, ifname, vops->direction,
					ingress_prios, &current_ingress_prio,
					ingress_pool,
					egress_prios, &current_egress_prio,
					egress_pool,
					cache, &filter);
			if (ret != PON_ADAPTER_SUCCESS)
				goto err;
		}
	}

	if (vops->tagged_drop_enable) {
		for (i = 0; i < ARRAY_SIZE(vlan_protocols); ++i) {
			single_tagged_frames(&filter, vlan_protocols[i]);
			filter.act = NETLINK_FILTER_ACT_DROP;
			ret = filter_add(ctx, ifname, vops->direction,
					ingress_prios, &current_ingress_prio,
					ingress_pool,
					egress_prios, &current_egress_prio,
					egress_pool,
					cache, &filter);
			if (ret != PON_ADAPTER_SUCCESS)
				goto err;
		}
	}

	cleanup_unused_prios(ctx->db, ifname, ingress_pool,
			     current_ingress_prio, ingress_prios,
			     ARRAY_SIZE(ingress_prios));
	cleanup_unused_prios(ctx->db, ifname, egress_pool,
			     current_egress_prio, egress_prios,
			     ARRAY_SIZE(egress_prios));

	/* There are no available priorities so set it to the end */
	current_ingress_prio = ARRAY_SIZE(ingress_prios);
	current_egress_prio = ARRAY_SIZE(egress_prios);

	ingress_pool = PON_NET_PRIO_VLAN_FILTERING_INGRESS_DEFAULT;
	egress_pool = PON_NET_PRIO_VLAN_FILTERING_EGRESS_DEFAULT;
	if (vops->direction == NETLINK_FILTER_DIR_INGRESS ||
	    vops->direction == NETLINK_FILTER_DIR_BOTH) {
		ret = pon_net_dev_db_gen(ctx->db, ifname, ingress_pool,
					 ingress_prios,
					 ARRAY_SIZE(ingress_prios));
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(pon_net_dev_db_gen, ret);
			goto err;
		}
		current_ingress_prio = 0;
	}
	if (vops->direction == NETLINK_FILTER_DIR_EGRESS ||
	    vops->direction == NETLINK_FILTER_DIR_BOTH) {
		ret = pon_net_dev_db_gen(ctx->db, ifname, egress_pool,
					 egress_prios,
					 ARRAY_SIZE(egress_prios));
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(pon_net_dev_db_gen, ret);
			goto err;
		}
		current_egress_prio = 0;
	}

	/* We have to drop tagged packets when pass_on_match_enable is set
	 * and also when tagged_drop_enable is set. tagged_drop_enable should
	 * drop all tagged packets and pass_on_match_enable all packets which
	 * are not explicitly filtered.
	 */
	if (vops->pass_on_match_enable || vops->tagged_drop_enable)
		/* Only matched frames should pass, drop every other
		 * tagged frame
		 */
		filter.act = NETLINK_FILTER_ACT_DROP;
	else
		/* Only matched frames should be dropped, pass every other
		 * tagged frame
		 */
		filter.act = NETLINK_FILTER_ACT_OK;

	/* Here we setup default rules for single tagged frames
	 * We need 2 default for single tagged rules (802.1q, 802.1ad)
	 */
	for (i = 0; i < ARRAY_SIZE(vlan_protocols); ++i) {
		single_tagged_frames(&filter, vlan_protocols[i]);

		ret = filter_add(ctx, ifname, vops->direction,
				ingress_prios, &current_ingress_prio,
				ingress_pool,
				egress_prios, &current_egress_prio,
				egress_pool,
				cache, &filter);
		if (ret != PON_ADAPTER_SUCCESS)
			goto err;
	}

	/* Finally we setup default rules for untagged frames */
	single_tagged_frames(&filter, ETH_P_ALL);
	if (vops->untagged_bridging)
		filter.act = NETLINK_FILTER_ACT_OK;
	else
		filter.act = NETLINK_FILTER_ACT_DROP;

	ret = filter_add(ctx, ifname, vops->direction,
			ingress_prios, &current_ingress_prio, ingress_pool,
			egress_prios, &current_egress_prio, egress_pool,
			cache, &filter);
	if (ret != PON_ADAPTER_SUCCESS)
		goto err;

err:
	cleanup_unused_prios(ctx->db, ifname, ingress_pool,
			     current_ingress_prio, ingress_prios,
			     ARRAY_SIZE(ingress_prios));
	cleanup_unused_prios(ctx->db, ifname, egress_pool,
			     current_egress_prio, egress_prios,
			     ARRAY_SIZE(egress_prios));

	dbg_out_ret("%d", ret);
	return ret;
}

/*
 * Apply the VLAN tagging configuration, stored in a struct pon_net_vlan_tagging
 * structure to the system.
 */
static enum pon_adapter_errno
pon_net_vlan_tagging_apply(struct pon_net_vlan_tagging *vlan_tagging,
			   struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	enum pon_adapter_errno ret2;

	dbg_in_args("%p, %p", vlan_tagging, ctx);

	if (pon_net_vlan_tagging_operation_eq(&vlan_tagging->pending,
					      &vlan_tagging->applied)) {
		/* Nothing changed - do nothing */
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = netlink_qdisc_clsact_create(ctx->netlink, vlan_tagging->ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_clsact_create, ret);
		goto err;
	}

	filter_cache_delete_filters(&vlan_tagging->filter_cache, ctx,
				    vlan_tagging->ifname);

	/*
	 * There can be up-to 12 filtering in one ME but for BBF the
	 * most common filter is just one: allowing VID1 and dropping anything
	 * else. For the initial implementation we should target to
	 * support this filter
	 *
	 * TODO: Implement the support for the entire vops table
	 *
	 * Currently this only works when:
	 *     tagged_pass_enable == 0
	 */
	if (vlan_tagging->pending.vops.tagged_pass_enable) {
		vlan_tagging->applied = vlan_tagging->pending;

		dbg_wrn("Unsupported forward_operation. All Tagged Pass Enable is not supported\n");
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = __update_filtering(ctx,
				 &vlan_tagging->filter_cache,
				 &vlan_tagging->pending.vops,
				 vlan_tagging->pending.tci_list,
				 vlan_tagging->pending.tci_list_len,
				 vlan_tagging->ifname);
	if (ret != PON_ADAPTER_SUCCESS)
		goto err;

	vlan_tagging->applied = vlan_tagging->pending;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
err:
	filter_cache_delete_filters(&vlan_tagging->filter_cache, ctx,
				    vlan_tagging->ifname);
	/* Try to revert what we did */
	ret2 = __update_filtering(ctx,
				  &vlan_tagging->filter_cache,
				  &vlan_tagging->applied.vops,
				  vlan_tagging->applied.tci_list,
				  vlan_tagging->applied.tci_list_len,
				  vlan_tagging->ifname);
	if (ret2 != PON_ADAPTER_SUCCESS) {
		static const struct pon_net_vlan_tagging_operation zero = {0};

		dbg_err_fn_ret(__update_filtering, ret2);

		/* If we can't recover then remove everything */
		filter_cache_delete_filters(&vlan_tagging->filter_cache, ctx,
					    vlan_tagging->ifname);
		vlan_tagging->applied = zero;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno
pon_net_vlan_filtering_update(struct pon_net_context *ctx, uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_vlan_tagging vlan_tagging = { 0 };

	dbg_in_args("%p, %u", ctx, me_id);

	ret = pon_net_me_list_read(&ctx->me_list,
				   PON_CLASS_ID_VLAN_TAGGING_FILTER_DATA,
				   me_id, &vlan_tagging, sizeof(vlan_tagging));
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_read, ret);
		return ret;
	}

	ret = pon_net_vlan_tagging_apply(&vlan_tagging, ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_vlan_tagging_apply, ret);
		return ret;
	}

	ret = pon_net_me_list_write(&ctx->me_list,
				    PON_CLASS_ID_VLAN_TAGGING_FILTER_DATA,
				    me_id, &vlan_tagging, sizeof(vlan_tagging));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	ret = pon_net_mac_bridge_port_filter_preassign_table_update(ctx, &me_id,
								    1);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(
		    ret, pon_net_mac_bridge_port_filter_preassign_table_update,
		    ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno
pon_net_vlan_filtering_cleanup(struct pon_net_context *ctx, uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_vlan_tagging *vlan_tagging;
	static const struct pon_net_vlan_tagging_operation zero = {0};

	dbg_in_args("%p, %u", ctx, me_id);

	vlan_tagging = pon_net_me_list_get_data(
	    &ctx->me_list, PON_CLASS_ID_VLAN_TAGGING_FILTER_DATA, me_id);
	if (!vlan_tagging) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	filter_cache_delete_filters(&vlan_tagging->filter_cache, ctx,
				    vlan_tagging->ifname);
	vlan_tagging->applied = zero;

	dbg_out_ret("%d", ret);
	return ret;
}

/* Function which tests, whether we can update the filters */
static enum pon_adapter_errno
should_update_filtering(struct pon_net_context *ctx,
			uint32_t tp_type,
			const char *ifname,
			int *should_update)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, \"%s\", %p", ctx, tp_type, ifname, should_update);

	if (tp_type == PA_BP_TP_TYPE_PMAP) {
		/*
		 * We should only update pmapper filters when it is
		 * enslaved to a bridge
		 */
		ret = netlink_is_slave(ctx->netlink, ifname, should_update);
		dbg_out_ret("%d", ret);
		return ret;
	}

	*should_update = 1;

	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno
pon_net_vlan_tagging_filter_data_vlan_forwarding_get(
					    struct pon_net_context *ctx,
					    uint16_t me_id,
					    struct pon_net_vlan_forwarding *fwd)
{
	struct pon_net_vlan_tagging *vt;
	struct pon_net_vlan_tagging_operation *op;
	int err;

	dbg_in_args("%p, %u, %p", ctx, me_id, fwd);

	vt = pon_net_me_list_get_data(&ctx->me_list,
				      PON_CLASS_ID_VLAN_TAGGING_FILTER_DATA,
				      me_id);
	if (!vt) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	op = &vt->applied;

	err = memcpy_s(fwd->tci_list,
		      sizeof(fwd->tci_list),
		      op->tci_list,
		      op->tci_list_len * sizeof(*op->tci_list));
	if (err) {
		dbg_err_fn_ret(memcpy_s, err);
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	fwd->tci_list_len = op->tci_list_len;
	fwd->tci_mask = op->vops.tci_mask;
	fwd->untagged_bridging = op->vops.untagged_bridging;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* This an update handler for VLAN Tagging Filter Data ME */
static enum pon_adapter_errno
vlan_tagging_filter_data_update(void *ll_handle,
				uint16_t me_id,
				const uint16_t *filter_list,
				uint8_t entries_num,
				uint8_t forward_operation,
				bool use_g988)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct vops vops = {0};
	uint8_t tp_type = 0;
	uint16_t tp_ptr = 0;
	char ifname[IF_NAMESIZE] = "";
	struct pon_net_vlan_tagging vlan_tagging = { 0 };
	int should_update = 0;

	dbg_in_args("%p, %u, %p, %u, %u, %d", ll_handle, me_id, filter_list,
		    entries_num, forward_operation, use_g988);

	ret = forward_operation_to_vops(use_g988, forward_operation, &vops);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn(forward_operation_to_vops);
		dbg_out_ret("%d", ret);
		return ret;
	}

	ret = pon_net_tp_get(ctx, me_id, &tp_type, &tp_ptr);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_tp_get, ret);
		return ret;
	}

	ret = pon_net_tp_ifname_get(ctx, tp_type, tp_ptr, ifname,
				    sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_tp_ifname_get, ret);
		return ret;
	}

	ret = pon_net_me_list_read(&ctx->me_list,
			     PON_CLASS_ID_VLAN_TAGGING_FILTER_DATA, me_id,
			     &vlan_tagging, sizeof(vlan_tagging));
	if (ret != PON_ADAPTER_SUCCESS && ret != PON_ADAPTER_ERR_NOT_FOUND) {
		FN_ERR_RET(ret, pon_net_me_list_read, ret);
		return ret;
	}

	interpret_dir(&vops, tp_type);
	ret = pon_net_vlan_tagging_operation_set(&vlan_tagging.pending,
						 &vops,
						 filter_list,
						 entries_num);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_vlan_tagging_operation_set, ret);
		return ret;
	}

	if (strncpy_s(vlan_tagging.ifname, sizeof(vlan_tagging.ifname), ifname,
		      IF_NAMESIZE)) {
		dbg_err_fn(strncpy_s);
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = pon_net_me_list_write(&ctx->me_list,
				    PON_CLASS_ID_VLAN_TAGGING_FILTER_DATA,
				    me_id, &vlan_tagging, sizeof(vlan_tagging));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	ret = should_update_filtering(ctx, tp_type, ifname, &should_update);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, should_update_filtering, ret);
		return ret;
	}

	if (should_update == 0) {
		/*
		 * The filtering will be updated later - by calling
		 * pon_net_vlan_filtering_update()
		 */
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	/* Filters will be updated now */
	ret = pon_net_vlan_tagging_apply(&vlan_tagging, ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_vlan_tagging_apply, ret);
		return ret;
	}

	ret = pon_net_me_list_write(&ctx->me_list,
				    PON_CLASS_ID_VLAN_TAGGING_FILTER_DATA,
				    me_id, &vlan_tagging, sizeof(vlan_tagging));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	/* VLAN Tagging Filter Data ME ID is the ME ID of the bridge port */
	ret = pon_net_bp_vlan_forwarding_update(ctx, &me_id, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bp_vlan_forwarding_update, ret);
		return ret;
	}

	ret = pon_net_mac_bridge_port_filter_preassign_table_update(ctx, &me_id,
								    1);
	if (ret != PON_ADAPTER_SUCCESS && ret != PON_ADAPTER_ERR_NOT_FOUND) {
		FN_ERR_RET(
		    ret, pon_net_mac_bridge_port_filter_preassign_table_update,
		    ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
vlan_tagging_filter_data_destroy(void *ll_handle, uint16_t me_id)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_vlan_tagging vlan_tagging = { 0 };
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u", ctx, me_id);

	ret = pon_net_me_list_read(&ctx->me_list,
				   PON_CLASS_ID_VLAN_TAGGING_FILTER_DATA,
				   me_id, &vlan_tagging, sizeof(vlan_tagging));
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_read, ret);
		return ret;
	}

	filter_cache_pool_return(&vlan_tagging.filter_cache, ctx->db,
				 vlan_tagging.ifname);

	ret = netlink_qdisc_clsact_destroy(ctx->netlink, vlan_tagging.ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_clsact_destroy, ret);
		return ret;
	}

	/* Clean up VLAN forwarding from the associated bridge port */
	ret = pon_net_bp_vlan_forwarding_cleanup(ctx, &me_id, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bp_vlan_forwarding_cleanup, ret);
		return ret;
	}

	pon_net_me_list_remove(&ctx->me_list,
			       PON_CLASS_ID_VLAN_TAGGING_FILTER_DATA,
			       me_id);

	ret = pon_net_mac_bridge_port_filter_preassign_table_update(ctx, &me_id,
								    1);
	if (ret != PON_ADAPTER_SUCCESS && ret != PON_ADAPTER_ERR_NOT_FOUND) {
		FN_ERR_RET(
		    ret, pon_net_mac_bridge_port_filter_preassign_table_update,
		    ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_vlan_tagging_filter_data_ops vlan_tagging_filter_data_ops = {
	.update = vlan_tagging_filter_data_update,
	.destroy = vlan_tagging_filter_data_destroy,
};
