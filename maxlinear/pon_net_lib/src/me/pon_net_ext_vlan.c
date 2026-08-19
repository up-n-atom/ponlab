/*****************************************************************************
 *
 * Copyright (c) 2020 - 2024 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/** \addtogroup PON_NET_EXT_VLAN
 *
 *   @{
 */

/**
 * \file
 * ## Overview
 *
 * This file contains the implementation of Extended VLAN Configuration Data ME.
 *
 * Adding an Extended VLAN rule works the following way:
 *
 * 1) Higher layer calls `struct pa_ext_vlan_ops->rules_add()` to replace
 *    currently configured rules with new rules.
 *
 * 2) Array of rules passed to `struct pa_ext_vlan_ops->rules_add()` is compared
 *    with currently configured rules (`struct pon_net_ext_vlan->cache`).
 *    \ref pon_net_ext_vlan_rules_pair_walk().
 *
 * 3) Every new rule is matched against the internal \ref omci_rules table to
 *    classify it as specific case \ref pon_net_ext_vlan_table.h.
 *    The matching determines rule major and minor number.
 *
 * 4) Having determined major and minor number, the ext_vlan_rules table is
 *    searched (\ref pon_net_ext_vlan_table.h). This is done by
 *    \ref pon_net_ext_vlan_filters_get().
 *
 * 5) The pon_net_ext_vlan_filters_get() obtains a rule from `ext_vlan_rules`
 *    table that matches major and minor number and it expands it to
 *    multiple TC filters that represent the Extended VLAN rule. The expansion
 *    is needed because a single OMCI Extended VLAN rule is usually represented
 *    by multiple TC filters.
 *
 * 6) Such expanded array of TC filters is sent through RT netlink
 *    interface by passing it to \ref netlink_filter_add() function.
 *
 * ### Example
 *
 * Assuming we have these rules configured:
 *
 *     (struct pon_adapter_ext_vlan_filter[]) {
 *         {15, 4096, 0, 15, 4096, 0, 0, 0, 15, 0, 0, 15, 0, 0}
 *         {15, 4096, 0, 14, 4096, 0, 0, 0, 15, 0, 0, 15, 0, 0}
 *         {14, 4096, 0, 14, 4096, 0, 0, 0, 15, 0, 0, 15, 0, 0}
 *     }
 *
 * And the OLT wants to set the following rule:
 *
 *     {15, 4096, 0, 15, 4096, 0, 0, 0, 15, 0, 0, 5, 1000, 6}
 *
 * Because the filtering part of the rule is also a key, which uniquely
 * identifies a given rule, this new rule will overwrite the Rule 0.1.
 * The higher layer will call `struct pa_ext_vlan_ops->rules_add()` with the
 * following array of rules:
 *
 *     (struct pon_adapter_ext_vlan_filter[]) {
 *         {15, 4096, 0, 15, 4096, 0, 0, 0, 15, 0, 0, 5, 1000, 6}  <--- new rule
 *         {15, 4096, 0, 14, 4096, 0, 0, 0, 15, 0, 0, 15, 0, 0}
 *         {14, 4096, 0, 14, 4096, 0, 0, 0, 15, 0, 0, 15, 0, 0}
 *     }
 *
 * This will be compared with the existing rules using \ref
 * pon_net_ext_vlan_rules_pair_walk(). As a result:
 *
 * This will be deleted:
 *
 *     {15, 4096, 0, 15, 4096, 0, 0, 0, 15, 0, 0, 15, 0, 0}
 *
 * And this rule will be added:
 *
 *     {15, 4096, 0, 15, 4096, 0, 0, 0, 15, 0, 0, 5, 1000, 6}
 *
 * First step when adding a rule is to match it against the \ref omci_rules
 * table to determine its major and minor number. The rule in this example
 * will match to the Rule 0.5, which is "Insert one full tag X with fixed
 * prio (default flow)". The major number is 0 and minor number is 5.
 *
 * Next step is to generate TC filters. Having a major and minor number, the
 * function \ref pon_net_ext_vlan_filters_get() is called and it returns an
 * array of TC filters that are later applied with netlink_filter_add().
 * For the Rule 0.5 it will create the following TC filters:
 *
 *     tc filter add dev eth0_0 ingress handle 0x77 protocol all prio 64001 \
 *           flower skip_sw action vlan push id 1000 priority 5 \
 *           protocol 0x88a8 cookie f8000000f8000000000f000000051f46
 *     tc filter add dev eth0_0 egress handle 0x9 protocol 802.1ad \
 *           prio 64001 flower skip_sw vlan_id 1000
 *           action vlan pop cookie f8000000f80005c0000f000000151f46
 *
 * The information about configured rules will be saved in
 * `struct pon_net_ext_vlan->cache`, and will be used for the comparison
 * on the next update.
 *
 * Additionally the filters for the multicast (`eth0_0_2`) and broadcast devices
 * (`eth0_0_3`) will be replicated, to ensure that multicast traffic is handled
 * according to the same Extended VLAN configuration.
 *
 *     tc filter add dev eth0_0_2 egress handle 0x2 protocol 802.1ad \
 *         prio 64001 flower skip_sw vlan_id 1000 vlan_prio 5 \
 *         action vlan pop cookie f8000000f80005c0000f000000151f46
 *     tc filter add dev eth0_0_3 egress handle 0x2 protocol 802.1ad \
 *         prio 64001 flower skip_sw vlan_id 1000 \
 *         action vlan pop cookie f8000000f80005c0000f000000151f46
 */

#include <stdlib.h>
#include <string.h>
#include <linux/if_ether.h>

#include <netlink/route/act/mirred.h>

#include <pon_adapter.h>
#include <omci/me/pon_adapter_mac_bridge_port_config_data.h>

#include "../pon_net_debug.h"
#include "../pon_net_common.h"
#include "../pon_net_netlink.h"
#include "../pon_net_ext_vlan_filter_def.h"
#include "../pon_net_ext_vlan_rules.h"
#include "../pon_net_pool.h"
#include "../pon_net_dev_db.h"
#include "../pon_net_dev_defs.h"
#include "../pon_net_tc_filter.h"
#include "../pon_net_uni.h"
#include "pon_net_mac_bridge_port_config_data.h"
#include "pon_net_ext_vlan.h"

/** Size of intermediate array for DSCP conversion */
#define DSCP_CONVERSION_GROUPS 8

/** Extended VLAN rule restoration list item definition
 *
 * When applying multiple rules, the operations are recorded on this list.
 * If rule creation or deletion fails during this procedure, then this list is
 * used to revert the operations in order to restore original state.
 */
struct rule_restoration_item {
	SLIST_ENTRY(rule_restoration_item) entries;
	/** If true, then rules_add_restoration() will delete on r_new rule */
	bool del_new;
	/** If true, then rules_add_restoration() will add on r_old rule */
	bool add_old;
	/**
	 * Old rule to be restored. This is not owning pointer.
	 * It will be valid as long as corresponding
	 * struct pon_net_ext_vlan_rules collection is valid.
	 */
	struct pon_net_ext_vlan_rule *r_old;
	/**
	 * New rule to be deleted. This is not owning pointer.
	 * It will be valid as long as corresponding
	 * struct pon_net_ext_vlan_rules collection is valid.
	 */
	struct pon_net_ext_vlan_rule *r_new;
};
/** Extended VLAN rules restoration list definition */
SLIST_HEAD(rule_restoration_list, rule_restoration_item);

/* OMCI bit DSCP to DSCP table converter */
void pon_net_dscp_convert(uint8_t *src, uint8_t *dst)
{
	int i, j, k;
	uint32_t groups[DSCP_CONVERSION_GROUPS];

	dbg_in_args("%p, %p", src, dst);

	/* Group bits into 8 x 24 bit groups */
	j = 0;
	for (i = 0; i < DSCP_CONVERSION_GROUPS; ++i) {
		groups[i] = (uint32_t)src[j++] << 16;
		groups[i] |= (uint32_t)src[j++] << 8;
		groups[i] |= src[j++];
	}

	/* Convert prepared groups into 64 dscp */
	k = 0;
	for (i = 0; i < DSCP_CONVERSION_GROUPS; ++i) {
		for (j = k + 7; j >= k; --j) {
			dst[j] = groups[i] & 0x7;
			groups[i] >>= 3;
		}
		k += 8;
	}

	dbg_out();
}

enum pon_adapter_errno
pon_net_ext_vlan_vlan_forwarding_get(struct pon_net_context *ctx,
				     uint16_t me_id,
				     struct pon_net_vlan_forwarding *fwd)
{
	const struct pon_net_ext_vlan *ext_vlan;
	const struct pon_net_ext_vlan_rule *rule;
	unsigned int i;
	bool has_vlan;
	uint16_t vlan;
	int ret;

	dbg_in_args("%p, %u, %p", ctx, me_id, fwd);

	ext_vlan = pon_net_me_list_get_data(&ctx->me_list,
					    PON_CLASS_ID_EXT_VLAN_CONF_DATA,
					    me_id);
	if (!ext_vlan) {
		FN_ERR_RET(0, "%d", PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	ret = memset_s(fwd, sizeof(*fwd), 0, sizeof(*fwd));
	if (ret) {
		FN_ERR_RET(ret, "%d", PON_ADAPTER_ERR_MEM_ACCESS);
		return PON_ADAPTER_ERR_MEM_ACCESS;
	}

	/* Go over every Ext. VLAN rule */
	for (i = 0; i < ext_vlan->cache->num_rules; ++i) {
		rule = &ext_vlan->cache->rules[i];

		/* Get outer VLAN for this rule */
		has_vlan = pon_net_ext_vlan_bridge_port_vlan(&rule->filter,
							     &vlan);
		if (has_vlan) {
			/* If VLAN is there, add it to the list */
			if (fwd->tci_list_len < ARRAY_SIZE(fwd->tci_list))
				fwd->tci_list[fwd->tci_list_len++] = vlan;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pon_net_ext_vlan_me_id_get(struct pon_net_context *ctx,
						  uint16_t association_type,
						  uint16_t associated_ptr,
						  uint16_t *me_id)
{
	struct pon_net_me_list_item *item;
	struct pon_net_ext_vlan *ext_vlan;

	dbg_in_args("%p, %u, %u, %p", ctx, association_type, associated_ptr,
		    me_id);

	pon_net_me_list_foreach_class(&ctx->me_list,
				      PON_CLASS_ID_EXT_VLAN_CONF_DATA, item)
	{
		ext_vlan = item->data;
		if (ext_vlan->association_type == association_type &&
		    ext_vlan->associated_ptr == associated_ptr) {
			if (me_id)
				*me_id = item->me_id;
			dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
			return PON_ADAPTER_SUCCESS;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
	return PON_ADAPTER_ERR_NOT_FOUND;
}

/*
 * Convert struct netlink_filter into struct pon_net_tc_filter, which contains
 * only information that is necessary to delete a rule. The
 * struct pon_net_tc_filter is stored in cache and used for rule deletion
 */
static void into_tcf(const struct netlink_filter *filter,
		     struct pon_net_tc_filter *tcf,
		     unsigned int handle_pool,
		     unsigned int prio_pool)
{

	tcf->dir = filter->dir,
	tcf->handle = filter->handle,
	tcf->prio = (uint16_t)filter->prio,
	tcf->vlan_id = filter->vlan_id,
	tcf->proto = filter->proto != NETLINK_FILTER_UNUSED ?
			(uint16_t)filter->proto : ETH_P_ALL;
	tcf->handle_pool_id = handle_pool;
	tcf->prio_pool_id = prio_pool;
	snprintf(tcf->ifname, sizeof(tcf->ifname), "%s", filter->device);
}

static enum pon_adapter_errno
generic_filter_add(struct pon_net_ext_vlan *ext_vlan,
		   const struct netlink_filter *orig,
		   unsigned int handle_pool, unsigned int prio_pool,
		   struct pon_net_tc_filter_array *info)
{
	uint32_t handle, prio;
	struct netlink_filter filter = *orig;
	enum pon_adapter_errno ret;
	struct pon_net_tc_filter tcf;
	struct pon_net_context *ctx = ext_vlan->ctx;

	dbg_in_args("%p, %p, %u, %u, %p", ext_vlan, orig, handle_pool,
		    prio_pool, info);

	ret = pon_net_dev_db_gen(ctx->db, filter.device, handle_pool, &handle,
				 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
		return ret;
	}

	ret = pon_net_dev_db_gen(ctx->db, filter.device, prio_pool, &prio, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		enum pon_adapter_errno ret2;

		ret2 = pon_net_dev_db_put(ctx->db, filter.device, handle_pool,
					  &handle, 1);
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret2);

		FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
		return ret;
	}

	filter.handle = handle;
	filter.prio = (int)prio;

	into_tcf(&filter, &tcf, handle_pool, prio_pool);

	ret = pon_net_tc_filter_array_add(info, &tcf);
	if (ret != PON_ADAPTER_SUCCESS) {
		enum pon_adapter_errno ret2;

		ret2 = pon_net_dev_db_put(ctx->db, filter.device, prio_pool,
					  &prio, 1);
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret2);
		ret2 = pon_net_dev_db_put(ctx->db, filter.device, handle_pool,
					  &handle, 1);
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret2);
		FN_ERR_RET(ret, pon_net_tc_filter_array_add, ret);
		return ret;
	}

	ret = netlink_filter_add(ctx->netlink, &filter);
	if (ret != PON_ADAPTER_SUCCESS) {
		enum pon_adapter_errno ret2;

		pon_net_tc_filter_array_pop(info);
		ret2 = pon_net_dev_db_put(ctx->db, filter.device, prio_pool,
					  &prio, 1);
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret2);
		ret2 = pon_net_dev_db_put(ctx->db, filter.device, handle_pool,
					  &handle, 1);
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret2);
		FN_ERR_RET(ret, netlink_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ext_vlan_rules_apply(struct pon_net_ext_vlan *ext_vlan);

static enum pon_adapter_errno
ext_vlan_rules_reapply(struct pon_net_ext_vlan *ext_vlan);

static enum pon_adapter_errno
ext_vlan_rules_clear(struct pon_net_ext_vlan *ext_vlan);

static enum pon_adapter_errno
iphost_filter_add(struct pon_net_ext_vlan *ext_vlan,
		  const struct netlink_filter *orig, unsigned int handle_pool,
		  unsigned int prio_pool, struct pon_net_tc_filter_array *info)
{
	enum pon_adapter_errno ret;
	struct netlink_filter filter = *orig;
	uint32_t handle, prio;
	struct pon_net_context *ctx = ext_vlan->ctx;

	dbg_in_args("%p, %p", ext_vlan, orig);

	filter.vlan_prio = NETLINK_FILTER_UNUSED;

	if (filter.vlan_id != NETLINK_FILTER_UNUSED) {
		uint32_t vid = (uint32_t)filter.vlan_id;
		struct pon_net_tc_filter tcf = {0};
		struct netlink_filter flt;
		struct netlink_mirred_data flt_data = {0};

		snprintf(flt_data.dev, IF_NAMESIZE, "%s", filter.device);
		flt_data.action = TCA_EGRESS_REDIR;

		ret = pon_net_dev_db_map(ctx->db, PON_MASTER_DEVICE,
					 PON_NET_HANDLE_IPHOST_VLAN_TRAP,
					 vid, &handle);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_dev_db_map, ret);
			return ret;
		}
		ret = pon_net_dev_db_map(ctx->db, PON_MASTER_DEVICE,
					 PON_NET_PRIO_IPHOST_VLAN_TRAP,
					 vid, &prio);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_dev_db_map, ret);
			return ret;
		}

		/* Generate system filter for IP Host vlan */
		ret = netlink_filter_vlan(&flt, PON_MASTER_DEVICE,
					  IPHOST_VLAN_HW_TC, filter.vlan_id,
					  NETLINK_FILTER_ACT_MIRRED, &flt_data);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_filter_vlan, ret);
			return ret;
		}

		flt.handle = handle;
		flt.prio = (int)prio;

		into_tcf(&flt, &tcf, 0, 0);

		ret = pon_net_tc_filter_array_add(info, &tcf);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_tc_filter_array_add, ret);
			return ret;
		}

		netlink_filter_del(ctx->netlink, &flt);
		ret = netlink_filter_add(ctx->netlink, &flt);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_filter_add, ret);
			return ret;
		}
	}

	ret = generic_filter_add(ext_vlan, &filter, handle_pool, prio_pool,
				 info);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, generic_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* Get MAC Bridge Port Configuration Data ME ID to given association */
static enum pon_adapter_errno bp_meid_get(struct pon_net_context *ctx,
					  uint16_t association_type,
					  uint16_t associated_ptr,
					  uint16_t *bp_meid)
{
	enum pon_adapter_errno ret;
	uint8_t tp_type;

	dbg_in_args("%p, %u, %u, %p", ctx, association_type, associated_ptr,
		    bp_meid);

	if (association_type == PA_EXT_VLAN_MAC_BP_CFG_DATA) {
		/*
		 * If Extended VLAN points to a bridge port, then we
		 * already have MAC Bridge Port Configuration Data ME ID
		 */
		*bp_meid = associated_ptr;
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	/*
	 * Otherwise we must find the MAC Bridge Port Configuration Data
	 * ME ID
	 */
	ret = pon_net_association_type_to_tp_type(association_type,
						  &tp_type);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_association_type_to_tp_type,
			   ret);
		return ret;
	}

	ret = pon_net_bp_me_id_get(ctx, tp_type, associated_ptr,
				   bp_meid);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", ret);
		return ret;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bp_me_id_get, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Get the interface name of the device that is connected to the bridge for
 * given associated ME
 */
static enum pon_adapter_errno bp_ifname_get(struct pon_net_context *ctx,
					    uint16_t association_type,
					    uint16_t associated_ptr,
					    char *ifname,
					    unsigned int capacity)
{
	enum pon_adapter_errno ret;
	uint16_t bp_meid;

	dbg_in_args("%p, %u, %u, %p, %u", ctx, association_type, associated_ptr,
		    ifname, capacity);

	ret = bp_meid_get(ctx, association_type, associated_ptr, &bp_meid);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", ret);
		return ret;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, bp_meid_get, ret);
		return ret;
	}

	ret = pon_net_bp_ifname_get(ctx, bp_meid, ifname, capacity);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", ret);
		return ret;
	}
	if (!pon_net_dev_db_get(ctx->db, ifname)) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bp_ifname_get, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return PON_ADAPTER_SUCCESS;
}

/*
 * This implementation is used to mirror the corresponding Ext. Vlan TC filters
 * on multicast devices, related to UNI
 *
 * So for example: we get the same filters on egress on eth0_0 and on eth0_0_2
 */
static
enum pon_adapter_errno uni_filter_add(struct pon_net_ext_vlan *ext_vlan,
				      const struct netlink_filter *orig,
				      unsigned int handle_pool,
				      unsigned int prio_pool,
				      struct pon_net_tc_filter_array *info,
				      uint8_t uni_mc_type,
				      uint8_t uni_bc_type)
{
	enum pon_adapter_errno ret;
	struct netlink_filter filter = *orig;
	uint8_t lport;
	struct pon_net_context *ctx = ext_vlan->ctx;
	int len;
	struct netlink_mirred_data flt_data = {
		.policy = TC_ACT_OK,
		.action = TCA_INGRESS_REDIR,
	};

	dbg_in_args("%p, %p, %u, %u, %p, %u, %u", ext_vlan, orig, handle_pool,
		    prio_pool, info, uni_mc_type, uni_bc_type);

	lport = ifname_to_lan_idx(ctx, filter.device);
	if (!(lport < LAN_PORT_MAX)) {
		FN_ERR_RET((int)lport, ifname_to_lan_idx, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	/* Change drop rules for LCT.
	 * Packets have to be redirected to LCT device if device is present.
	 */
	if ((strncmp(ctx->cfg.lct_name, "none", IF_NAMESIZE) != 0) &&
	    !pon_net_is_ext_uni(ctx, lport) &&
	    filter.act == NETLINK_FILTER_ACT_DROP &&
	    filter.vlan_id == NETLINK_FILTER_UNUSED &&
	    (filter.proto == ETH_P_ALL ||
	     filter.proto == NETLINK_FILTER_UNUSED)) {
		snprintf(flt_data.dev, IF_NAMESIZE, "%s", ctx->cfg.lct_name);
		filter.act = NETLINK_FILTER_ACT_MIRRED;
		filter.act_data = &flt_data;
	}

	ret = generic_filter_add(ext_vlan, &filter, handle_pool, prio_pool,
				 info);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_add, ret);
		return ret;
	}

	if (orig->dir != NETLINK_FILTER_DIR_EGRESS) {
		/* For multicast devices we need only egress */
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (!ext_vlan->mc_replication_disabled) {
		len = snprintf(filter.device, sizeof(filter.device), "%s",
			       ext_vlan->mc_ifname);
		if (len >= (int)sizeof(filter.device)) {
			FN_ERR_RET(len, snprintf, PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		ret = generic_filter_add(ext_vlan, &filter, handle_pool,
					 prio_pool, info);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_filter_add, ret);
			return ret;
		}
	}

	ret = pon_net_uni_ifname_get(ctx, uni_bc_type, lport, filter.device,
				     sizeof(filter.device));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_uni_ifname_get, ret);
		return ret;
	}

	ret = generic_filter_add(ext_vlan, &filter, handle_pool, prio_pool,
				 info);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
count_mc_gem_ext_vlans(struct pon_net_context *ctx, const char *ifname,
		       unsigned int *count);

/* Apply pending rule collection on the UNI to the system */
static enum pon_adapter_errno
uni_apply(struct pon_net_ext_vlan *ext_vlan, uint8_t uni_mc_type)
{
	enum pon_adapter_errno ret;
	unsigned int count;
	bool mc_replication_disabled;

	dbg_in_args("%p, %u", ext_vlan, uni_mc_type);

	ret = count_mc_gem_ext_vlans(ext_vlan->ctx, ext_vlan->mc_ifname,
				     &count);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, count_mc_gem_ext_vlans, ret);
		return ret;
	}

	/*
	 * During MIB reset, when MEs are deleted, the UNI ME might
	 * already be deleted. If it is so, then just do nothing.
	 */
	if (!pon_net_dev_db_get(ext_vlan->ctx->db, ext_vlan->ifname)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	/*
	 * If there are Multicast GEM ITP extended VLANs configured on
	 * this interface, then we will not perform multicast rules
	 * "replication"
	 */
	mc_replication_disabled = !!count;

	/*
	 * If the "replication" option changed, then recreate the rules -
	 * delete and add them again
	 */
	if (ext_vlan->mc_replication_disabled != mc_replication_disabled) {
		ext_vlan->mc_replication_disabled = mc_replication_disabled;
		ret = ext_vlan_rules_reapply(ext_vlan);
		if (ret != PON_ADAPTER_SUCCESS) {
			ext_vlan->mc_replication_disabled =
			    !mc_replication_disabled;
			FN_ERR_RET(ret, ext_vlan_rules_reapply, ret);
			return ret;
		}

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = ext_vlan_rules_apply(ext_vlan);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, ext_vlan_rules_apply, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
pptp_uni_filter_add(struct pon_net_ext_vlan *ext_vlan,
		    const struct netlink_filter *orig,
		    unsigned int handle_pool,
		    unsigned int prio_pool,
		    struct pon_net_tc_filter_array *info)
{
	return uni_filter_add(ext_vlan, orig, handle_pool, prio_pool, info,
			      UNI_PPTP_MC, UNI_PPTP_BC);
}

static enum pon_adapter_errno
pptp_uni_apply(struct pon_net_ext_vlan *ext_vlan)
{
	return uni_apply(ext_vlan, UNI_PPTP_MC);
}

static enum pon_adapter_errno
veip_filter_add(struct pon_net_ext_vlan *ext_vlan,
		const struct netlink_filter *orig, unsigned int handle_pool,
		unsigned int prio_pool, struct pon_net_tc_filter_array *info)
{
	return uni_filter_add(ext_vlan, orig, handle_pool, prio_pool, info,
			      UNI_VEIP_MC, UNI_VEIP_BC);
}

static enum pon_adapter_errno
veip_apply(struct pon_net_ext_vlan *ext_vlan)
{
	return uni_apply(ext_vlan, UNI_VEIP_MC);
}

static enum pon_adapter_errno apply_on_unis(struct pon_net_context *ctx,
					    const char *ifname);

/* Apply pending rule collection to the system */
static enum pon_adapter_errno
mc_gem_apply(struct pon_net_ext_vlan *ext_vlan)
{
	enum pon_adapter_errno ret;
	char ifname[IF_NAMESIZE] = {0};

	dbg_in_args("%p", ext_vlan);

	ret = bp_ifname_get(ext_vlan->ctx, ext_vlan->association_type,
			    ext_vlan->associated_ptr, ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_ERR_NOT_FOUND &&
	    ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, bp_ifname_get, ret);
		return ret;
	}

	/*
	 * If the interface name is empty, it means that this MC
	 * GEM has not yet been connected to the bridge, or it was
	 * disconnected from the bridge. Nevertheless - we should
	 * clear the Extended VLAN rules that are applied and refresh
	 * Extended VLAN on the UNI - in order to create the
	 * "replicated" multicast filters there
	 */
	if (!strlen(ifname)) {
		ret = ext_vlan_rules_clear(ext_vlan);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, ext_vlan_rules_clear, ret);
			return ret;
		}

		ret = apply_on_unis(ext_vlan->ctx, NULL);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, apply_on_unis, ret);
			return ret;
		}

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = ext_vlan_rules_apply(ext_vlan);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, ext_vlan_rules_apply, ret);
		return ret;
	}

	ret = apply_on_unis(ext_vlan->ctx, ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, apply_on_unis, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Add an Extended VLAN TC filter, but instead of using ext_vlan->ifname, use
 * the name of the device that is connected to the bridge. For the
 * Multicast GEM Port Interworking TP, this will be the one of the UNI Multicast
 * interfaces
 */
static enum pon_adapter_errno
mc_gem_filter_add(struct pon_net_ext_vlan *ext_vlan,
		  const struct netlink_filter *orig, unsigned int handle_pool,
		  unsigned int prio_pool, struct pon_net_tc_filter_array *info)
{
	enum pon_adapter_errno ret;
	struct netlink_filter filter = *orig;

	dbg_in_args("%p, %p, %u, %u, %p", ext_vlan, orig, handle_pool,
		    prio_pool, info);

	ret = bp_ifname_get(ext_vlan->ctx, ext_vlan->association_type,
			    ext_vlan->associated_ptr, filter.device,
			    sizeof(filter.device));
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, bp_ifname_get, ret);
		return ret;
	}

	ret = generic_filter_add(ext_vlan, &filter, handle_pool, prio_pool,
				 info);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* Structure holding implementation details for specific association types */
struct association_ops {
	/* Association type */
	uint16_t association_type;

	/*
	 * If association type is PA_EXT_VLAN_MAC_BP_CFG_DATA
	 * then this is used to select correct association_ops
	 * instead of association_type. See association_ops_get()
	 */
	uint8_t bp_type;

	/** Reverse direction of filters for this association_type */
	bool reverse_direction;

	/**
	 * Adds one (or more) TC filters to the system
	 *
	 * \param[in] ext_vlan     Extended VLAN structure
	 * \param[in] filter       TC filter to add
	 * \param[in] handle_pool  Handle pool id from which to generate handle
	 * \param[in] prio_pool    Priority pool id from which to generate prio
	 * \param[in,out] tc_info  Array of pon_net_tc_filter saved for deletion
	 *
	 * \return returns value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*filter_add)(struct pon_net_ext_vlan *ext_vlan,
		      const struct netlink_filter *filter,
		      unsigned int handle_pool,
		      unsigned int prio_pool,
		      struct pon_net_tc_filter_array *tc_info);

	/**
	 * Applies 'pending' rule collection to the system. Saves
	 * applied rule collectoin in 'cache'.
	 *
	 * \param[in]  ext_vlan      Extended VLAN structure
	 *
	 * \return returns value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*apply)(struct pon_net_ext_vlan *ext_vlan);
};

static const struct association_ops iphost_association_ops = {
	.bp_type = PA_BP_TP_TYPE_IP_HOST,
	.association_type = PA_EXT_VLAN_IP_HOST_CONFIG_DATA,
	.reverse_direction = false,
	.filter_add = iphost_filter_add,
	.apply = ext_vlan_rules_apply,
};

static const struct association_ops pptp_uni_association_ops = {
	.bp_type = PA_BP_TP_TYPE_PPTP_UNI,
	.association_type = PA_EXT_VLAN_PPTP_ETH_UNI,
	.filter_add = pptp_uni_filter_add,
	.apply = pptp_uni_apply,
};

static const struct association_ops veip_association_ops = {
	.bp_type = PA_BP_TP_TYPE_VEIP,
	.association_type = PA_EXT_VLAN_VEIP,
	.filter_add = veip_filter_add,
	.apply = veip_apply,
};

static const struct association_ops pmapper_association_ops = {
	.bp_type = PA_BP_TP_TYPE_PMAP,
	.association_type = PA_EXT_VLAN_IEEE_P_MAPPER,
	.reverse_direction = true,
	.filter_add = generic_filter_add,
	.apply = ext_vlan_rules_apply,
};

static const struct association_ops gem_association_ops = {
	.bp_type = PA_BP_TP_TYPE_GEM,
	.association_type = PA_EXT_VLAN_GEM_ITP,
	.reverse_direction = true,
	.filter_add = generic_filter_add,
	.apply = ext_vlan_rules_apply,
};

static const struct association_ops mc_gem_association_ops = {
	.bp_type = PA_BP_TP_TYPE_MC_GEM,
	.association_type = PA_EXT_VLAN_MC_GEM_ITP,
	/*
	 * We apply this on the UNI Multicast interfaces, hence reverse
	 * direction is false
	 */
	.reverse_direction = false,
	.filter_add = mc_gem_filter_add,
	.apply = mc_gem_apply,
};

static const struct association_ops generic_association_ops = {
	.filter_add = generic_filter_add,
	.apply = ext_vlan_rules_apply,
};

static const struct association_ops *association_ops_list[] = {
	&iphost_association_ops,
	&pptp_uni_association_ops,
	&veip_association_ops,
	&gem_association_ops,
	&pmapper_association_ops,
	&mc_gem_association_ops,
	&generic_association_ops, /* Last resort */
};

static const struct association_ops
*association_ops_get(struct pon_net_context *ctx,
		     uint16_t association_type,
		     uint16_t association_ptr)
{
	unsigned int i = 0;
	uint8_t bp_tp_type;
	uint16_t bp_tp_ptr;
	const struct association_ops *op = &generic_association_ops;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %u", ctx, association_type, association_ptr);

	if (association_type == PA_EXT_VLAN_MAC_BP_CFG_DATA) {
		ret = pon_net_tp_get(ctx, association_ptr, &bp_tp_type,
				     &bp_tp_ptr);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_out_ret("%p", op);
			return op;
		}
	}

	for (i = 0; i < ARRAY_SIZE(association_ops_list); ++i) {
		op = association_ops_list[i];

		if (association_type == PA_EXT_VLAN_MAC_BP_CFG_DATA) {
			if (op->bp_type == bp_tp_type)
				break;
			continue;
		}

		if (association_type == op->association_type)
			break;
	}

	dbg_out_ret("%p", op);
	return op;
}

/* This function will determine if a rule should be applied to
   an ingress or an egress traffic */
static enum netlink_filter_dir
association_ops_dir_get(const struct association_ops *op, bool ds)
{
	if (op->reverse_direction)
		return ds ? NETLINK_FILTER_DIR_INGRESS :
			    NETLINK_FILTER_DIR_EGRESS;
	else
		return ds ? NETLINK_FILTER_DIR_EGRESS :
			    NETLINK_FILTER_DIR_INGRESS;
}

static uint8_t association_ops_uni_mc_type(const struct association_ops *op)
{
	if (op->association_type == PA_EXT_VLAN_PPTP_ETH_UNI)
		return UNI_PPTP_MC;
	if (op->association_type == PA_EXT_VLAN_VEIP)
		return UNI_VEIP_MC;
	return UNI_UNSPEC;
}

/*
 * Iterate over every Extended VLAN on the UNI, whose multicast interface name
 * matches 'ifname' and trigger apply() for them. If 'ifname' is NULL, then
 * apply() be called for every UNI Extended VLAN
 *
 * This is needed so that UNI Extended VLAN can create or delete replicated
 * TC filters on multicast interfaces.
 */
static enum pon_adapter_errno apply_on_unis(struct pon_net_context *ctx,
					    const char *ifname)
{
	enum pon_adapter_errno ret;
	struct pon_net_me_list_item *item;
	struct pon_net_ext_vlan *ext_vlan;
	uint8_t uni_mc_type;

	dbg_in_args("%p, \"%s\"", ctx, ifname ? ifname : "(NULL)");

	pon_net_me_list_foreach_class(&ctx->me_list,
				      PON_CLASS_ID_EXT_VLAN_CONF_DATA, item) {
		ext_vlan = item->data;
		uni_mc_type =
		    association_ops_uni_mc_type(ext_vlan->association_ops);
		if (uni_mc_type == UNI_UNSPEC)
			continue;

		if (ifname && strcmp(ifname, ext_vlan->mc_ifname) != 0)
			continue;

		if (ext_vlan->association_ops->apply) {
			ret = ext_vlan->association_ops->apply(ext_vlan);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, apply, ret);
				return ret;
			}
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Count how many Multicast GEM Port ITP Extended VLANs are configured on
 * the 'ifname' interface.
 */
static enum pon_adapter_errno
count_mc_gem_ext_vlans(struct pon_net_context *ctx, const char *ifname,
		       unsigned int *count)
{
	enum pon_adapter_errno ret;
	struct pon_net_me_list_item *item;
	struct pon_net_ext_vlan *ext_vlan;
	char bp_ifname[IF_NAMESIZE];
	unsigned int counter = 0;

	dbg_in_args("%p, %p, %p", ctx, ifname, count);

	pon_net_me_list_foreach_class(&ctx->me_list,
				      PON_CLASS_ID_EXT_VLAN_CONF_DATA, item) {
		ext_vlan = item->data;

		if (ext_vlan->association_ops->association_type !=
		    PA_EXT_VLAN_MC_GEM_ITP)
			continue;

		/*
		 * The rules are not yet applied on this interface, or there
		 * are no rules at all
		 */
		if (!ext_vlan->cache->num_rules)
			continue;

		ret = bp_ifname_get(ext_vlan->ctx, ext_vlan->association_type,
				    ext_vlan->associated_ptr, bp_ifname,
				    sizeof(bp_ifname));
		if (ret == PON_ADAPTER_ERR_NOT_FOUND)
			continue;
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, bp_ifname_get, ret);
			return ret;
		}

		if (strcmp(bp_ifname, ifname) != 0)
			continue;

		counter++;
	}

	if (count)
		*count = counter;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static const uint16_t protos[] = { ETH_P_8021Q, ETH_P_8021AD };

/*
 * Create precedent filters used for implementing Extended VLAN
 * downstream modes
 */
static enum pon_adapter_errno
precedent_filters_create(struct pon_net_context *ctx, const char *ifname,
			 enum netlink_filter_dir dir,
			 enum netlink_filter_act act,
			 uint32_t *priorities,
			 uint32_t *handles)
{
	struct netlink_filter filter;
	unsigned int i, j;
	uint32_t index = 0;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\", %d, %d, %p, %p", ctx, ifname, dir, act,
		    priorities, handles);

	netlink_filter_defaults(&filter);
	filter.act = act;
	filter.dir = dir;
	snprintf(filter.device, sizeof(filter.device), "%s", ifname);

	/* Untagged */
	filter.handle = handles[index];
	filter.prio = (int)priorities[index];
	ret = netlink_filter_add(ctx->netlink, &filter);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_add, ret);
		return ret;
	}

	/* Single tagged */
	for (i = 0; i < ARRAY_SIZE(protos); ++i) {
		index++;
		filter.proto = protos[i];
		filter.handle = handles[index];
		filter.prio = (int)priorities[index];
		ret = netlink_filter_add(ctx->netlink, &filter);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_filter_add, ret);
			return ret;
		}
	}

	/* Double tagged, loop over inner and outer tag */
	for (i = 0; i < ARRAY_SIZE(protos); ++i) {
		filter.proto = protos[i];
		for (j = 0; j < ARRAY_SIZE(protos); ++j) {
			index++;

			filter.handle = handles[index];
			filter.prio = (int)priorities[index];
			filter.vlan_proto = protos[j];

			ret = netlink_filter_add(ctx->netlink, &filter);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, netlink_filter_add, ret);
				return ret;
			}
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Destroy precedent filters used for implementing Extended VLAN
 * downstream modes
 */
static void
precedent_filters_destroy(struct pon_net_context *ctx, const char *ifname,
			  enum netlink_filter_dir dir,
			  uint32_t *priorities,
			  uint32_t *handles)
{
	uint32_t handle, priority;
	unsigned int i, j;
	uint32_t index = 0;

	dbg_in_args("%p, \"%s\", %d, %p, %p", ctx, ifname, dir, priorities,
		    handles);

	/* Untagged */
	handle = handles[index];
	priority = priorities[index];
	netlink_filter_clear_one(ctx->netlink, ifname, ETH_P_ALL,
				 (uint16_t)priority, handle, dir);

	/* Single tagged */
	for (i = 0; i < ARRAY_SIZE(protos); ++i) {
		index++;
		handle = handles[index];
		priority = priorities[index];
		netlink_filter_clear_one(ctx->netlink, ifname, protos[i],
					 (uint16_t)priority, handle, dir);
	}

	/* Double tagged, loop over inner and outer tag */
	for (i = 0; i < ARRAY_SIZE(protos); ++i) {
		for (j = 0; j < ARRAY_SIZE(protos); ++j) {
			index++;
			handle = handles[index];
			priority = priorities[index];
			netlink_filter_clear_one(ctx->netlink, ifname,
						 protos[i], (uint16_t)priority,
						 handle, dir);
		}
	}

	dbg_out();
}

static enum pon_adapter_errno
precedent_filters_set(struct pon_net_ext_vlan *ext_vlan,
		      enum netlink_filter_act act, uint32_t *priorities,
		      uint32_t *handles,
		      const bool create)
{
	enum netlink_filter_dir dir;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %d, %p, %p, %d", ext_vlan, act, priorities, handles,
		    create);

	dir = association_ops_dir_get(ext_vlan->association_ops, true);

	if (create) {
		ret = precedent_filters_create(ext_vlan->ctx, ext_vlan->ifname,
					       dir, act, priorities, handles);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, precedent_filters_create, ret);
			return ret;
		}
		dbg_out_ret("%d", ret);
		return ret;
	}

	precedent_filters_destroy(ext_vlan->ctx, ext_vlan->ifname, dir,
				  priorities, handles);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static bool no_ds_rules(uint16_t ds_mode)
{
	return ds_mode == DS_MODE_FORWARD || ds_mode == DS_MODE_DISCARD;
}

/*
 * Apply precedent TC filters according to ds_mode option, and
 * cache the information about applied precedent filter in ext_vlan
 * structure
 */
static enum pon_adapter_errno
ds_mode_filters_update(struct pon_net_ext_vlan *ext_vlan, uint16_t ds_mode)
{
	bool forward = false;
	bool discard = false;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u", ext_vlan, ds_mode);

	switch (ds_mode) {
	case DS_MODE_FORWARD:
		forward = true;
		break;
	case DS_MODE_VID_PBIT_OR_DISCARD:
	case DS_MODE_VID_OR_DISCARD:
	case DS_MODE_PBIT_OR_DISCARD:
	case DS_MODE_DISCARD:
		discard = true;
		break;
	}

	if (ext_vlan->ds_forward != forward) {
		ret = precedent_filters_set(ext_vlan,
					    NETLINK_FILTER_ACT_OK,
					    ext_vlan->ds_forward_priorities,
					    ext_vlan->ds_handles, forward);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, precedent_filters_set, ret);
			return ret;
		}
		ext_vlan->ds_forward = forward;
	}

	if (ext_vlan->ds_discard != discard) {
		ret = precedent_filters_set(ext_vlan,
					    NETLINK_FILTER_ACT_DROP,
					    ext_vlan->ds_discard_priorities,
					    ext_vlan->ds_handles, discard);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, precedent_filters_set, ret);
			return ret;
		}
		ext_vlan->ds_discard = discard;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Clear precedent filters for ds_mode
 */
static enum pon_adapter_errno
ds_mode_filters_clear(struct pon_net_ext_vlan *ext_vlan)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ext_vlan);

	ret = ds_mode_filters_update(ext_vlan, DS_MODE_REVERSE);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, ds_mode_filters_update, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

 /*
  * This function will produce a series of tc filters for given 'rule' and
  * it will send via netlink to the TC using the corresponding function
  * filter_add() for given association_type.
  */
static enum pon_adapter_errno add_filters(struct pon_net_ext_vlan *ext_vlan,
					  struct pon_net_ext_vlan_rule *rule,
					  const bool ds)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	unsigned int i;
	struct netlink_filter_definition *filter_defs = NULL;
	unsigned int num_filter_defs = 0;
	unsigned int handle_pool, prio_pool;
	enum netlink_filter_dir dir;

	dbg_in_args("%p, %p, %u", ext_vlan, rule, ds);

	/* Special handling for DS modes */
	if (ds) {
		ret = ds_mode_filters_update(ext_vlan, ext_vlan->ds_mode);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, ds_mode_filtering_set, ret);
			return ret;
		}
		/* Do not create additional rules for generic DS modes */
		if (no_ds_rules(ext_vlan->ds_mode)) {
			dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
			return PON_ADAPTER_SUCCESS;
		}
	}

	num_filter_defs = pon_net_ext_vlan_filters_get(ext_vlan,
						       &rule->filter,
						       ds,
						       rule->major,
						       rule->minor,
						       &filter_defs);
	if (!num_filter_defs) {
		dbg_wrn("Rule %u.%u didn't create any tc-flower filters\n",
			rule->major,
			rule->minor);
		return PON_ADAPTER_SUCCESS;
	}

	dir = association_ops_dir_get(ext_vlan->association_ops, ds);

	if (dir == NETLINK_FILTER_DIR_INGRESS) {
		if (rule->is_def)
			prio_pool = PON_NET_PRIO_EXT_VLAN_INGRESS_DEFAULT;
		else
			prio_pool = PON_NET_PRIO_EXT_VLAN_INGRESS;
	} else {
		if (rule->is_def)
			prio_pool = PON_NET_PRIO_EXT_VLAN_EGRESS_DEFAULT;
		else
			prio_pool = PON_NET_PRIO_EXT_VLAN_EGRESS;

	}

	if (dir == NETLINK_FILTER_DIR_INGRESS)
		handle_pool = PON_NET_HANDLE_FILTER_INGRESS;
	else
		handle_pool = PON_NET_HANDLE_FILTER_EGRESS;

	for (i = 0; i < num_filter_defs; ++i) {
		if (strncpy_s(filter_defs[i].filter.device,
					sizeof(filter_defs[i].filter.device),
					ext_vlan->ifname, IF_NAMESIZE)) {
			free(filter_defs);
			dbg_err_fn(strncpy_s);
			dbg_out_ret("%d", PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		filter_defs[i].filter.dir =
			association_ops_dir_get(ext_vlan->association_ops, ds);

		filter_defs[i].is_def = rule->is_def;
	}

	for (i = 0; i < num_filter_defs; ++i) {
		ret = ext_vlan->association_ops->filter_add(ext_vlan,
						  &filter_defs[i].filter,
						  handle_pool, prio_pool,
						  &rule->tc_info);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(filter_add, ret);
			break;
		}
	}

	free(filter_defs);

	dbg_out_ret("%d", ret);
	return ret;
}

/*
 * This function will remove TC filters from the system, by calling
 * netlink_filter_clear_one(), with information stored in rule->tc_info.
 * Also it will return handle and priority numbers to handle and priority pool.
 * The rule->tc_info will be cleared.
 */
static enum pon_adapter_errno
tc_filters_remove(const struct pon_net_ext_vlan *ext_vlan,
		  struct pon_net_ext_vlan_rule *rule)
{
	char ifname[IF_NAMESIZE];
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	unsigned int i = 0;
	struct pon_net_tc_filter_array *tc_info = &rule->tc_info;
	struct pon_net_context *ctx = ext_vlan->ctx;

	dbg_in_args("%p, %p", ext_vlan, rule);

	for (i = 0; i < tc_info->count; i++) {
		struct pon_net_tc_filter *tc_filter = &tc_info->filters[i];
		uint32_t handle = tc_filter->handle;
		uint32_t prio = tc_filter->prio;

		/* If the device that Extended VLAN is pointing to was removed
		 * then there is no need to print errors */
		ret = pon_net_dev_db_put(ctx->db, tc_filter->ifname,
					 tc_filter->prio_pool_id, &prio, 1);
		if (ret != PON_ADAPTER_ERR_NOT_FOUND &&
		    ret != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret);
		ret = pon_net_dev_db_put(ctx->db, tc_filter->ifname,
					 tc_filter->handle_pool_id, &handle, 1);
		if (ret != PON_ADAPTER_ERR_NOT_FOUND &&
		    ret != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret);
	}

	ret = pon_net_associated_ifname_get(ctx, ext_vlan->association_type,
					    ext_vlan->associated_ptr, ifname,
					    sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		/* The related ME might have already been deleted */
		dbg_prn_fn_ret(pon_net_associated_ifname_get, ret);
		dbg_prn("The associated ME (type=%u, id=%u) doesn't exist\n",
			ext_vlan->association_type, ext_vlan->associated_ptr);
		goto out;
	}

	for (i = 0; i < tc_info->count; i++) {
		struct pon_net_tc_filter *tc_filter = &tc_info->filters[i];

		ret = netlink_filter_clear_one(ctx->netlink,
					       tc_filter->ifname,
					       tc_filter->proto,
					       tc_filter->prio,
					       tc_filter->handle,
					       tc_filter->dir);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_wrn_fn_ret(netlink_filter_clear_one, ret);
	}

out:
	pon_net_tc_filter_array_clear(tc_info);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Creates a series of TC filters for given 'rule' for downstream and upstream
 * directions.
 * The rule->tc_info will be filled with information to delete the rule with
 * tc_filters_delete()
 */
static enum pon_adapter_errno
tc_filters_add(struct pon_net_ext_vlan *ext_vlan,
	       struct pon_net_ext_vlan_rule *rule)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS, ret2;

	dbg_in_args("%p, %p", ext_vlan, rule);

	/* Create upstream tc filters */
	ret = add_filters(ext_vlan, rule, 0);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(add_filters, ret);

		/*
		 * Delete every TC filter that was created, to leave
		 * no side effects after failed add_filters()
		 */
		ret2 = tc_filters_remove(ext_vlan, rule);
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(tc_filters_remove, ret2);

		dbg_out_ret("%d", ret);
		return ret;
	}

	/* Create downstream tc filters */
	ret = add_filters(ext_vlan, rule, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(add_filters, ret);

		/*
		 * Delete every TC filter that was created, to leave
		 * no side effects after failed add_filters()
		 */
		ret2 = tc_filters_remove(ext_vlan, rule);
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(tc_filters_remove, ret2);

		dbg_out_ret("%d", ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/** Arguments for \ref rule_walker() callback */
struct rule_walker_args {
	/** Pointer to Extended VLAN context */
	struct pon_net_ext_vlan *ext_vlan;
	/**
	 * List on which add and remove operations are recorded
	 * to roll back failed rule add and remove operations.
	 */
	struct rule_restoration_list restoration_list;
};

/*
 * Executed for every compared rule pair, when old set of rules is compared
 * with a new set of rules
 */
static enum pon_adapter_errno
rule_walker(struct pon_net_ext_vlan_rule *old,
	    struct pon_net_ext_vlan_rule *new,
	    void *arg)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rule_walker_args *walker_args = arg;
	struct pon_net_ext_vlan *ext_vlan = walker_args->ext_vlan;
	int eq = old && new && pon_net_ext_vlan_filter_eq(&old->filter,
							  &new->filter);
	int filtering_part_equals = old && new && !eq;
	int delete_old = (old && !new) || filtering_part_equals;
	int add_new = (!old && new) || filtering_part_equals;
	int is_same = old && new && eq;
	struct rule_restoration_item *item;

	dbg_in_args("%p, %p, %p", old, new, arg);

	if (delete_old) {
		ret = tc_filters_remove(ext_vlan, old);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, tc_filters_remove, ret);
			return ret;
		}
	}

	if (add_new) {
		ret = tc_filters_add(ext_vlan, new);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, tc_filters_add, ret);
			dbg_out_ret("%d", ret);
			return ret;
		}
	}

	if (is_same) {
		ret =
		    pon_net_tc_filter_array_copy(&new->tc_info, &old->tc_info);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_tc_filter_array_copy, ret);
			return ret;
		}
	}

	/* Save state for rule restoration in case of failure */
	item = calloc(1, sizeof(*item));
	if (!item) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_MEMORY);
		return PON_ADAPTER_ERR_NO_MEMORY;
	}
	item->del_new = add_new;
	item->add_old = delete_old;

	item->r_old = old;
	item->r_new = new;

	SLIST_INSERT_HEAD(&walker_args->restoration_list, item, entries);

	dbg_out_ret("%d", ret);
	return ret;
}

/* Called to restore a single rule */
static enum pon_adapter_errno
rule_restoration(struct pon_net_ext_vlan *ext_vlan,
		 struct rule_restoration_item *i)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ext_vlan, i);

	if (i->del_new) {
		ret = tc_filters_remove(ext_vlan, i->r_new);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, tc_filters_remove, ret);
			return ret;
		}
	}

	if (i->add_old) {
		ret = tc_filters_add(ext_vlan, i->r_old);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, tc_filters_add, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* Called to restore rules after failed pon_net_ext_vlan_rules_pair_walk() */
static enum pon_adapter_errno
rules_add_restoration(struct rule_walker_args *args)
{
	struct rule_restoration_item *i, *next;
	enum pon_adapter_errno ret;

	dbg_in_args("%p", args);

	FOREACH_SAFE(i, next, &args->restoration_list, entries) {
		ret = rule_restoration(args->ext_vlan, i);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_out_ret("%d", ret);
			return ret;
		}
		SLIST_REMOVE(&args->restoration_list, i,
			     rule_restoration_item, entries);
		free(i);
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Clean up rule restoration list */
static void
rules_add_restoration_clear(struct rule_restoration_list *restoration_list)
{
	struct rule_restoration_item *i, *next;

	dbg_in_args("%p", restoration_list);

	FOREACH_SAFE(i, next, restoration_list, entries) {
		SLIST_REMOVE(restoration_list, i,
			     rule_restoration_item, entries);
		free(i);
	}

	dbg_out();
}

/* Refresh VLAN forwarding on related bridge port */
static enum pon_adapter_errno
ext_vlan_refresh_vlan_forwarding(struct pon_net_context *ctx,
				 uint16_t association_type,
				 uint16_t associated_ptr)
{
	enum pon_adapter_errno ret;
	uint8_t tp_type;
	uint16_t bp_meid;

	dbg_in_args("%p, %u, %u", ctx, association_type, associated_ptr);

	if (association_type == PA_EXT_VLAN_MAC_BP_CFG_DATA) {
		/*
		 * If Extended VLAN points to a bridge port, then we
		 * already have MAC Bridge Port Configuration Data ME ID
		 */
		bp_meid = associated_ptr;
	} else {
		/*
		 * Otherwise we must find the MAC Bridge Port Configuration Data
		 * ME ID
		 */
		ret = pon_net_association_type_to_tp_type(association_type,
							  &tp_type);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_association_type_to_tp_type,
				   ret);
			return ret;
		}

		ret = pon_net_bp_me_id_get(ctx, tp_type, associated_ptr,
					   &bp_meid);
		if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
			/* If there is no bridge port yet, then we skip
			 * VLAN forwarding refresh */
			dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
			return PON_ADAPTER_SUCCESS;
		}
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_bp_me_id_get, ret);
			return ret;
		}
	}

	ret = pon_net_bp_vlan_forwarding_update(ctx, &bp_meid, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bp_vlan_forwarding_update, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Replace the currently applied rules with a new set of rules, creating
 * and deleting TC filters.
 */
static enum pon_adapter_errno
ext_vlan_rules_swap(struct pon_net_ext_vlan *ext_vlan,
		    struct pon_net_ext_vlan_rules *rules)
{
	struct rule_walker_args walker_args = {0};
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	enum pon_adapter_errno ret2;

	dbg_in_args("%p %p", ext_vlan, rules);

	walker_args.ext_vlan = ext_vlan;
	SLIST_INIT(&walker_args.restoration_list);

	/*
	 * If we set an empty rule set, then there is no need to
	 * have ds_mode filters, so we clear them
	 */
	if (rules->num_rules == 0) {
		ret = ds_mode_filters_clear(ext_vlan);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(ds_mode_filters_clear, ret);
			goto err_out;
		}
	}

	/* Add new rules to TC */
	ret = pon_net_ext_vlan_rules_pair_walk(ext_vlan->cache, rules,
					       rule_walker, &walker_args);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_ext_vlan_rules_pair_walk, ret);
		ret2 = rules_add_restoration(&walker_args);
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(rules_add_restoration, ret2);
		rules_add_restoration_clear(&walker_args.restoration_list);

		ret2 = ds_mode_filters_update(ext_vlan, ext_vlan->ds_mode);
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(ds_mode_filters_update, ret);

		goto err_out;
	}
	rules_add_restoration_clear(&walker_args.restoration_list);

	/* Store the rules so that we can later compute the difference */
	pon_net_ext_vlan_rules_swap(ext_vlan->cache, rules);

err_out:
	dbg_out_ret("%d", ret);
	return ret;
}

/* Apply pending rules */
static enum pon_adapter_errno
ext_vlan_rules_apply(struct pon_net_ext_vlan *ext_vlan)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_ext_vlan_rules *rules;

	dbg_in_args("%p", ext_vlan);

	rules = pon_net_ext_vlan_rules_clone(ext_vlan->pending);
	if (!rules) {
		dbg_err_fn_ret(pon_net_ext_vlan_rules_clone, 0);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto err_out;
	}

	ret = ext_vlan_rules_swap(ext_vlan, rules);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(ext_vlan_rules_swap, ret);
		goto err_destroy_rules;
	}

err_destroy_rules:
	pon_net_ext_vlan_rules_destroy(rules);
err_out:
	dbg_out_ret("%d", ret);
	return ret;
}

/* Same as ext_vlan_rules_apply(), but will clear rules before applying them */
static enum pon_adapter_errno
ext_vlan_rules_reapply(struct pon_net_ext_vlan *ext_vlan)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	enum pon_adapter_errno ret2 = PON_ADAPTER_SUCCESS;
	struct pon_net_ext_vlan_rules *rules;

	dbg_in_args("%p", ext_vlan);

	rules = pon_net_ext_vlan_rules_create(NULL, 0);
	if (!rules) {
		dbg_err_fn_ret(pon_net_ext_vlan_rules_create, ret);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto err_out;
	}

	ret = ext_vlan_rules_swap(ext_vlan, rules);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(ext_vlan_rules_swap, ret);
		goto err_destroy_rules;
	}

	ret = ext_vlan_rules_apply(ext_vlan);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(ext_vlan_rules_apply, ret);
		goto err_revert;
	}

	pon_net_ext_vlan_rules_destroy(rules);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

err_revert:
	ret2 = ext_vlan_rules_swap(ext_vlan, rules);
	if (ret2 != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(ext_vlan_rules_swap, ret2);
err_destroy_rules:
	pon_net_ext_vlan_rules_destroy(rules);
err_out:
	dbg_out_ret("%d", ret);
	return ret;
}

/* Clear Extended VLAN rules */
static enum pon_adapter_errno
ext_vlan_rules_clear(struct pon_net_ext_vlan *ext_vlan)
{
	unsigned int i;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p", ext_vlan);

	for (i = 0; i < ext_vlan->cache->num_rules; ++i) {
		ret = tc_filters_remove(ext_vlan, &ext_vlan->cache->rules[i]);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_wrn_fn_ret(tc_filters_remove, ret);
	}

	pon_net_ext_vlan_rules_clear(ext_vlan->cache);

	ret = ds_mode_filters_clear(ext_vlan);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_wrn_fn_ret(ds_mode_filters_clear, ret);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Swap ext_vlan->pending with a new collection of rules and try to apply
 * them immediately.
 */
static enum pon_adapter_errno
ext_vlan_rules_swap_pending_and_apply(struct pon_net_ext_vlan *ext_vlan,
				      struct pon_net_ext_vlan_rules *rules)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p", ext_vlan, rules);

	pon_net_ext_vlan_rules_swap(ext_vlan->pending, rules);

	if (ext_vlan->association_ops->apply) {
		ret = ext_vlan->association_ops->apply(ext_vlan);
		if (ret != PON_ADAPTER_SUCCESS) {
			pon_net_ext_vlan_rules_swap(rules, ext_vlan->pending);
			FN_ERR_RET(ret, apply, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pon_net_ext_vlan_reapply(struct pon_net_context *ctx,
						uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_ext_vlan *ext_vlan;

	dbg_in_args("%p, %u", ctx, me_id);

	ext_vlan = pon_net_me_list_get_data(&ctx->me_list,
					    PON_CLASS_ID_EXT_VLAN_CONF_DATA,
					    me_id);
	if (!ext_vlan) {
		FN_ERR_RET(ret, pon_net_me_list_get_data,
			   PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	ret = ext_vlan_rules_clear(ext_vlan);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, ext_vlan_rules_clear, ret);
		return ret;
	}

	if (ext_vlan->association_ops->apply) {
		ret = ext_vlan->association_ops->apply(ext_vlan);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, apply, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
rules_add(void *ll_handle, const uint16_t id, const uint8_t ds_mode,
	  struct pon_adapter_ext_vlan_filter *filters, uint16_t entries_num)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_ext_vlan *ext_vlan;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_ext_vlan_rules *rules;

	dbg_in_args("%p, %u, %u, %p, %u", ll_handle, id, ds_mode, filters,
		    entries_num);

	ext_vlan = pon_net_me_list_get_data(
			&ctx->me_list,
			PON_CLASS_ID_EXT_VLAN_CONF_DATA, id);
	if (!ext_vlan) {
		dbg_err_fn_ret(pon_net_me_list_get_data, ret);
		ret = PON_ADAPTER_ERR_NOT_FOUND;
		goto err_out;
	}

	/* Create a next rule set that we will attempt to apply */
	rules = pon_net_ext_vlan_rules_create(filters, entries_num);
	if (!rules) {
		dbg_err_fn_ret(pon_net_ext_vlan_rules_create, 0);
		ret = PON_ADAPTER_ERROR;
		goto err_out;
	}

	ret = ext_vlan_rules_swap_pending_and_apply(ext_vlan, rules);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(ext_vlan_rules_swap_pending_and_apply, ret);
		goto err_destroy_rules;
	}

	/* Refresh bridge port */
	ret = ext_vlan_refresh_vlan_forwarding(ctx, ext_vlan->association_type,
					       ext_vlan->associated_ptr);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(ext_vlan_refresh_vlan_forwarding, ret);
		goto err_destroy_rules;
	}

err_destroy_rules:
	pon_net_ext_vlan_rules_destroy(rules);
err_out:
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
common_ip_handling_enable(void *ll_handle, const uint8_t enable)
{
	dbg_in_args("%p, %u", ll_handle, enable);
	/* TODO: Implement this function */
	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ignoring_ds_prio_enable(void *ll_handle, const bool enable)
{
	struct pon_net_context *ctx = ll_handle;

	dbg_in_args("%p, %u", ll_handle, enable);

	ctx->ignore_ds_rules_prio = enable;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Remove Ext. VLAN rule */
static enum pon_adapter_errno
rule_remove(void *ll_handle,
	    uint16_t me_id,
	    const struct pon_adapter_ext_vlan_filter *filter,
	    uint8_t ds_mode)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_ext_vlan *ext_vlan;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_ext_vlan_rule *res;
	struct pon_net_ext_vlan_rules *rules;

	dbg_in_args("%p, %u, %p, %u", ll_handle, me_id, filter, ds_mode);

	ext_vlan = pon_net_me_list_get_data(
	    &ctx->me_list, PON_CLASS_ID_EXT_VLAN_CONF_DATA, me_id);
	if (!ext_vlan) {
		dbg_err_fn_ret(pon_net_me_list_get_data, 0);
		ret = PON_ADAPTER_ERR_NOT_FOUND;
		goto err_out;
	}

	res = pon_net_ext_vlan_rules_find(ext_vlan->pending,
					  filter);
	if (!res) {
		dbg_prn_fn_ret(pon_net_ext_vlan_rules_find,
			       0);
		ret = PON_ADAPTER_SUCCESS;
		goto err_out;
	}

	rules = pon_net_ext_vlan_rules_clone(ext_vlan->pending);
	if (!rules) {
		dbg_err_fn_ret(pon_net_ext_vlan_rules_clone, 0);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto err_out;
	}

	/* Remove one rule from the rule collection */
	pon_net_ext_vlan_rules_remove(rules, filter);

	ret = ext_vlan_rules_swap_pending_and_apply(ext_vlan, rules);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(ext_vlan_rules_swap_pending_and_apply, ret);
		goto err_destroy_rules;
	}

	/* Refresh bridge port */
	ret = ext_vlan_refresh_vlan_forwarding(ctx, ext_vlan->association_type,
					       ext_vlan->associated_ptr);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(ext_vlan_refresh_vlan_forwarding, ret);
		goto err_destroy_rules;
	}

err_destroy_rules:
	pon_net_ext_vlan_rules_destroy(rules);
err_out:
	dbg_out_ret("%d", ret);
	return ret;
}

/* Clears all tc-flower filter associated with this ME. */
static enum pon_adapter_errno rule_clear_all(void *ll_handle, uint16_t me_id,
					     uint8_t ds_mode)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_ext_vlan *ext_vlan;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_ext_vlan_rules *rules;

	dbg_in_args("%p, %u, %u", ll_handle, me_id, ds_mode);

	ext_vlan = pon_net_me_list_get_data(
	    &ctx->me_list, PON_CLASS_ID_EXT_VLAN_CONF_DATA, me_id);
	if (!ext_vlan) {
		dbg_err_fn_ret(pon_net_me_list_get_data, 0);
		ret = PON_ADAPTER_ERR_NOT_FOUND;
		goto err_out;
	}

	rules = pon_net_ext_vlan_rules_create(NULL, 0);
	if (!rules) {
		dbg_err_fn_ret(pon_net_ext_vlan_rules_clone, 0);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto err_out;
	}

	ret = ext_vlan_rules_swap_pending_and_apply(ext_vlan, rules);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(ext_vlan_rules_swap_pending_and_apply, ret);
		goto err_destroy_rules;
	}

	/* Refresh bridge port */
	ret = ext_vlan_refresh_vlan_forwarding(ctx, ext_vlan->association_type,
					       ext_vlan->associated_ptr);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(ext_vlan_refresh_vlan_forwarding, ret);
		goto err_destroy_rules;
	}

err_destroy_rules:
	pon_net_ext_vlan_rules_destroy(rules);
err_out:
	dbg_out_ret("%d", ret);
	return ret;
}

/* Allocate handles and priorities for DS mode precedent filters up front */
static enum pon_adapter_errno
allocate_ds_handles_and_priorities(struct pon_net_ext_vlan *ext_vlan)
{
	enum pon_adapter_errno ret;
	const struct association_ops *association_ops;
	enum netlink_filter_dir dir;
	unsigned int handle_pool, discard_prio_pool, forward_prio_pool;

	dbg_in_args("%p", ext_vlan);

	association_ops =
	    association_ops_get(ext_vlan->ctx, ext_vlan->association_type,
				ext_vlan->associated_ptr);
	dir = association_ops_dir_get(association_ops, true);

	if (dir == NETLINK_FILTER_DIR_INGRESS) {
		discard_prio_pool = PON_NET_PRIO_EXT_VLAN_INGRESS_DISCARD;
		forward_prio_pool = PON_NET_PRIO_EXT_VLAN_INGRESS_FORWARD;
		handle_pool = PON_NET_HANDLE_FILTER_INGRESS;
	} else {
		discard_prio_pool = PON_NET_PRIO_EXT_VLAN_EGRESS_DISCARD;
		forward_prio_pool = PON_NET_PRIO_EXT_VLAN_EGRESS_FORWARD;
		handle_pool = PON_NET_HANDLE_FILTER_EGRESS;
	}

	ret = pon_net_dev_db_gen(ext_vlan->ctx->db, ext_vlan->ifname,
				 discard_prio_pool,
				 ext_vlan->ds_discard_priorities,
				 ARRAY_SIZE(ext_vlan->ds_discard_priorities));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
		return ret;
	}

	ret = pon_net_dev_db_gen(ext_vlan->ctx->db, ext_vlan->ifname,
				 forward_prio_pool,
				 ext_vlan->ds_forward_priorities,
				 ARRAY_SIZE(ext_vlan->ds_forward_priorities));
	if (ret != PON_ADAPTER_SUCCESS) {
		enum pon_adapter_errno ret2;

		ret2 = pon_net_dev_db_put(
		    ext_vlan->ctx->db, ext_vlan->ifname, discard_prio_pool,
		    ext_vlan->ds_discard_priorities,
		    ARRAY_SIZE(ext_vlan->ds_discard_priorities));
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret2);

		FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
		return ret;
	}

	ret = pon_net_dev_db_gen(ext_vlan->ctx->db, ext_vlan->ifname,
				 handle_pool, ext_vlan->ds_handles,
				 ARRAY_SIZE(ext_vlan->ds_handles));
	if (ret != PON_ADAPTER_SUCCESS) {
		enum pon_adapter_errno ret2;

		ret2 = pon_net_dev_db_put(
		    ext_vlan->ctx->db, ext_vlan->ifname, forward_prio_pool,
		    ext_vlan->ds_forward_priorities,
		    ARRAY_SIZE(ext_vlan->ds_forward_priorities));
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret2);

		ret2 = pon_net_dev_db_put(
		    ext_vlan->ctx->db, ext_vlan->ifname, discard_prio_pool,
		    ext_vlan->ds_discard_priorities,
		    ARRAY_SIZE(ext_vlan->ds_discard_priorities));
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret2);

		FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Deallocate handles and priorities for DS mode precedent filters up front */
static void
deallocate_ds_handles_and_priorities(struct pon_net_ext_vlan *ext_vlan)
{
	const struct association_ops *association_ops;
	enum netlink_filter_dir dir;
	unsigned int handle_pool;
	unsigned int discard_prio_pool;
	unsigned int forward_prio_pool;
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ext_vlan);

	if (!pon_net_dev_db_get(ext_vlan->ctx->db, ext_vlan->ifname)) {
		/* Device has already been freed */
		dbg_out();
		return;
	}

	association_ops =
	    association_ops_get(ext_vlan->ctx, ext_vlan->association_type,
				ext_vlan->associated_ptr);
	dir = association_ops_dir_get(association_ops, true);

	if (dir == NETLINK_FILTER_DIR_INGRESS) {
		discard_prio_pool = PON_NET_PRIO_EXT_VLAN_INGRESS_DISCARD;
		forward_prio_pool = PON_NET_PRIO_EXT_VLAN_INGRESS_FORWARD;
		handle_pool = PON_NET_HANDLE_FILTER_INGRESS;
	} else {
		discard_prio_pool = PON_NET_PRIO_EXT_VLAN_EGRESS_DISCARD;
		forward_prio_pool = PON_NET_PRIO_EXT_VLAN_EGRESS_FORWARD;
		handle_pool = PON_NET_HANDLE_FILTER_EGRESS;
	}

	ret = pon_net_dev_db_put(ext_vlan->ctx->db, ext_vlan->ifname,
				 handle_pool, ext_vlan->ds_handles,
				 ARRAY_SIZE(ext_vlan->ds_handles));
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	ret = pon_net_dev_db_put(ext_vlan->ctx->db, ext_vlan->ifname,
				 forward_prio_pool,
				 ext_vlan->ds_forward_priorities,
				 ARRAY_SIZE(ext_vlan->ds_forward_priorities));
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	ret = pon_net_dev_db_put(ext_vlan->ctx->db, ext_vlan->ifname,
				 discard_prio_pool,
				 ext_vlan->ds_discard_priorities,
				 ARRAY_SIZE(ext_vlan->ds_discard_priorities));
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	dbg_out();
}

/* Destroys ME resources */
static enum pon_adapter_errno destroy(void *ll_handle, uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_ext_vlan *ext_vlan;
	uint16_t association_type;
	uint16_t associated_ptr;

	dbg_in_args("%p, %u", ll_handle, me_id);

	mapper_id_remove(ctx->mapper[MAPPER_EXTVLAN_MEID_TO_IDX], me_id);

	ext_vlan = pon_net_me_list_get_data(&ctx->me_list,
					    PON_CLASS_ID_EXT_VLAN_CONF_DATA,
					    me_id);
	if (!ext_vlan) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	association_type = ext_vlan->association_type;
	associated_ptr = ext_vlan->associated_ptr;

	rule_clear_all(ll_handle, me_id, 0);

	ret = ext_vlan_rules_clear(ext_vlan);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(ext_vlan_rules_clear, ret);

	pon_net_ext_vlan_rules_destroy(ext_vlan->cache);
	pon_net_ext_vlan_rules_destroy(ext_vlan->pending);

	deallocate_ds_handles_and_priorities(ext_vlan);

	pon_net_me_list_remove(&ctx->me_list, PON_CLASS_ID_EXT_VLAN_CONF_DATA,
			       me_id);

	/* Refresh bridge port */
	ret = ext_vlan_refresh_vlan_forwarding(ctx, association_type,
					       associated_ptr);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(ext_vlan_refresh_vlan_forwarding, ret);

	/* We do not destroy clsact qdisc here, because it may be needed by
	   VLAN tagging filter data */

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* me_update() implementation. */
static enum pon_adapter_errno
update(void *ll_handle, struct pon_adapter_ext_vlan_update *update_data,
	uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_ext_vlan ext_vlan = {0};
	int cleanup_handles = 0, created = 0;
	uint8_t uni_mc_type;
	uint8_t lport;

	dbg_in_args("%p, %p, %u", ll_handle, update_data, me_id);

	ret = pon_net_me_list_read(&ctx->me_list,
				PON_CLASS_ID_EXT_VLAN_CONF_DATA,
				me_id,
				&ext_vlan,
				sizeof(ext_vlan));
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		ext_vlan.ctx = ctx;
		ext_vlan.association_type = update_data->association_type;
		ext_vlan.associated_ptr = update_data->associated_ptr;

		ext_vlan.cache = pon_net_ext_vlan_rules_create(NULL, 0);
		if (!ext_vlan.cache) {
			dbg_err_fn(pon_net_ext_vlan_rules_create);
			ret = PON_ADAPTER_ERR_NO_MEMORY;
			goto cleanup;
		}

		ext_vlan.pending = pon_net_ext_vlan_rules_create(NULL, 0);
		if (!ext_vlan.pending) {
			dbg_err_fn(pon_net_ext_vlan_rules_create);
			ret = PON_ADAPTER_ERR_NO_MEMORY;
			goto cleanup;
		}

		ret = pon_net_associated_ifname_get(ctx,
					update_data->association_type,
					update_data->associated_ptr,
					ext_vlan.ifname,
					sizeof(ext_vlan.ifname));
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(pon_net_associated_ifname_get, ret);
			ret = PON_ADAPTER_ERR_INVALID_VAL;
			goto cleanup;
		}

		ret = netlink_qdisc_clsact_create(ctx->netlink,
						  ext_vlan.ifname);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(netlink_qdisc_clsact_create, ret);
			goto cleanup;
		}

		ret = allocate_ds_handles_and_priorities(&ext_vlan);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(allocate_handles_and_priorities, ret);
			goto cleanup;
		}

		cleanup_handles = 1;

		ext_vlan.association_ops = association_ops_get(
		    ctx, ext_vlan.association_type, ext_vlan.associated_ptr);

		uni_mc_type =
		    association_ops_uni_mc_type(ext_vlan.association_ops);
		if (uni_mc_type != UNI_UNSPEC) {
			lport = ifname_to_lan_idx(ctx, ext_vlan.ifname);
			if (!(lport < LAN_PORT_MAX)) {
				dbg_err_fn_ret(ifname_to_lan_idx, (int)lport);
				ret = PON_ADAPTER_ERR_NOT_FOUND;
				goto cleanup;
			}

			ret = pon_net_uni_ifname_get(
			    ctx, uni_mc_type, lport, ext_vlan.mc_ifname,
			    sizeof(ext_vlan.mc_ifname));
			if (ret != PON_ADAPTER_SUCCESS) {
				dbg_err_fn_ret(pon_net_uni_ifname_get, ret);
				goto cleanup;
			}
		}

		created = 1;
	}

	ext_vlan.input_tpid = update_data->input_tpid;
	ext_vlan.output_tpid = update_data->output_tpid;
	ext_vlan.ds_mode = update_data->ds_mode;
	pon_net_dscp_convert(update_data->dscp, ext_vlan.dscp);

	/* Update not supported at the moment */
	if (update_data->association_type != ext_vlan.association_type)
		dbg_wrn("Association Type change is not supported\n");
	if (update_data->associated_ptr != ext_vlan.associated_ptr)
		dbg_wrn("Associated Ptr change is not supported\n");

	ret = pon_net_me_list_write(&ctx->me_list,
				    PON_CLASS_ID_EXT_VLAN_CONF_DATA,
				    me_id,
				    &ext_vlan,
				    sizeof(ext_vlan));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_me_list_write, ret);
		if (created)
			goto cleanup;
		dbg_out_ret("%d", ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

cleanup:
	if (cleanup_handles)
		deallocate_ds_handles_and_priorities(&ext_vlan);
	if (ext_vlan.cache)
		pon_net_ext_vlan_rules_destroy(ext_vlan.cache);
	if (ext_vlan.pending)
		pon_net_ext_vlan_rules_destroy(ext_vlan.pending);

	dbg_out_ret("%d", ret);
	return ret;
}

const struct pa_ext_vlan_ops ext_vlan_ops = {
	.update = update,
	.destroy = destroy,
	.common_ip_handling_enable = common_ip_handling_enable,
	.ignoring_ds_prio_enable = ignoring_ds_prio_enable,
	.rule_remove = rule_remove,
	.rule_clear_all = rule_clear_all,
	.rules_add = rules_add
};

/** @} */ /* PON_NET_EXT_VLAN */
