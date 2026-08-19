/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2019 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <linux/if_ether.h>

#include <netlink/route/link.h>
#include <netlink/route/act/mirred.h>
#include <pon_adapter.h>

#include "pon_net_common.h"
#include "pon_net_debug.h"
#include "pon_net_netlink.h"
#include "pon_net_rtnl.h"
#include "pon_net_dev_db.h"
#include "pon_net_dev_defs.h"
#include "me/pon_net_mac_bridge_port_filter_preassign_table.h"
#include "me/pon_net_mac_bridge_port_config_data.h"
#include <omci/me/pon_adapter_mac_bridge_port_config_data.h>
#include <omci/me/pon_adapter_gem_port_network_ctp.h>
#include <omci/pon_adapter_vlan_flow.h>

static enum pon_adapter_errno
__vlan_flow_add(void *ll_handle, struct vlan_flow *flow, uint32_t handle)
{
	struct pon_net_context *ctx = ll_handle;
	struct netlink_filter filter = {0};
	struct netlink_mirred_data act_data = {0};
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p, %u", ctx, flow, handle);

	netlink_filter_defaults(&filter);

	if (memcpy_s(filter.device, sizeof(filter.device),
		     flow->igdev, sizeof(flow->igdev))) {
		dbg_err_fn(memcpy_s);
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	if (memcpy_s(act_data.dev, sizeof(act_data.dev),
		     flow->egdev, sizeof(flow->egdev))) {
		dbg_err_fn(memcpy_s);
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	act_data.action = TCA_EGRESS_REDIR;

	filter.prio = flow->prio;
	filter.handle = handle;

	if (flow->tci_mask & TCI_MASK_VID)
		filter.vlan_id = TCI_VID(flow->tci);

/* Each forwarding rule with the PCP bits sets and a wildcard for the DEI bit
 * needs 2 PCE rules (one for each position of DEI bit). Do not activate
 * this feature for now to save PCE rules in the hardware.
 */
#if defined(VLAN_FORWARDING_PCP)
	if (flow->tci_mask & TCI_MASK_PCP)
		filter.vlan_prio = TCI_PCP(flow->tci);
#endif

	if (!flow->untagged)
		filter.proto = flow->tpid;

	if (flow->drop)
		act_data.policy = TC_ACT_SHOT;

	filter.dir = NETLINK_FILTER_DIR_INGRESS;
	filter.act = NETLINK_FILTER_ACT_MIRRED;
	filter.act_data = &act_data;

	ret = pon_net_rtnl_bridge_learning_set(ctx, flow->egdev, false);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_rtnl_bridge_learning_set, ret);
		return ret;
	}

	ret = netlink_filter_add(ctx->netlink, &filter);
	if (ret) {
		dbg_err_fn_ret(netlink_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
vlan_flow_add(void *ll_handle, struct vlan_flow *flow)
{
	enum pon_adapter_errno ret;

	ret = __vlan_flow_add(ll_handle, flow, flow->prio);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, vlan_flow_add, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
vlan_flow_del(void *ll_handle, struct vlan_flow *flow)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, flow);

	ret = netlink_filter_clear_one(ctx->netlink, flow->igdev, flow->tpid,
				       flow->prio, flow->prio,
				       NETLINK_FILTER_DIR_INGRESS);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_wrn_fn_ret(netlink_filter_clear_one, ret);
		return ret;
	}

	ret = pon_net_rtnl_bridge_learning_set(ctx, flow->egdev, false);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_rtnl_bridge_learning_set, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

void pon_net_vlan_flow_list_init(struct pon_net_vlan_flow_list *list)
{
	dbg_in_args("%p", list);
	SLIST_INIT(list);
	dbg_out();
}

void pon_net_vlan_flow_list_exit(struct pon_net_context *ctx,
				 struct pon_net_vlan_flow_list *list)
{
	struct vlan_flow_entry *i;

	dbg_in_args("%p", list);

	while (!SLIST_EMPTY(list)) {
		i = SLIST_FIRST(list);
		(void)vlan_flow_del(ctx, i->flow);
		SLIST_REMOVE_HEAD(list, entries);
		free(i->flow);
		free(i);
	}

	dbg_out();
}

static enum pon_net_pool_type
vlan_flow_get_type(struct vlan_flow *flow)
{
	if (!flow->untagged) {
		if (flow->tci_mask & ~TCI_MASK_VID)
			return PON_NET_PRIO_VLAN_FLOW_INGRESS_TCI;
		else if (flow->tci_mask & TCI_MASK_VID)
			return PON_NET_PRIO_VLAN_FLOW_INGRESS_VID;
		else
			return PON_NET_PRIO_VLAN_FLOW_INGRESS_ANY_VLAN;
	}
	return PON_NET_PRIO_VLAN_FLOW_INGRESS_ANY;
}

/* Mark all entries identified by 'me_id' in 'list' for deletion */
static void
pon_net_vlan_flow_list_mark_for_deletion(struct pon_net_vlan_flow_list *list,
					 uint16_t me_id)
{
	struct vlan_flow_entry *i;

	dbg_in_args("%p, %u", list, me_id);

	SLIST_FOREACH(i, list, entries)
		if (i->me_id == me_id)
			i->marked_for_deletion = true;

	dbg_out();
}

/* Return 'true' if two instances of struct vlan_flow are equal */
static bool vlan_flow_eq(const struct vlan_flow *a, const struct vlan_flow *b)
{
	return strcmp(a->igdev, b->igdev) == 0 &&
		strcmp(a->egdev, b->egdev) == 0 &&
		a->tci == b->tci &&
		a->tci_mask == b->tci_mask &&
		a->tpid == b->tpid &&
		a->untagged == b->untagged &&
		a->drop == b->drop;
}

/*
 * Unmark entry marked for deletion, which contains 'flow' and return 'true'
 * if the entry was unmarked
 */
static bool
pon_net_vlan_flow_list_unmark_one(struct pon_net_vlan_flow_list *list,
				  uint16_t me_id, const struct vlan_flow *flow)
{
	struct vlan_flow_entry *i;

	dbg_in_args("%p, %u, %p", list, me_id, flow);

	SLIST_FOREACH(i, list, entries) {
		if (i->me_id != me_id)
			continue;

		if (!vlan_flow_eq(i->flow, flow))
			continue;

		i->marked_for_deletion = false;
		dbg_out_ret("%d", true);
		return true;
	}

	dbg_out_ret("%d", false);
	return false;
}

/*
 * Mark all entries identified by 'me_id' and were marked
 * for deletion with pon_net_vlan_flow_list_mark_for_deletion()
 */
static void
pon_net_vlan_flow_clear_marked(struct pon_net_context *ctx, uint16_t me_id)
{
	struct vlan_flow_entry *i;
	struct vlan_flow_entry *next;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	enum pon_net_pool_type pool_type;

	dbg_in_args("%p, %u", ctx, me_id);

	FOREACH_SAFE(i, next, &ctx->vlan_flow_list, entries) {
		if (i->me_id == me_id && i->marked_for_deletion) {
			uint32_t prio = i->flow->prio;

			ret = vlan_flow_del(ctx, i->flow);
			if (ret != PON_ADAPTER_SUCCESS)
				dbg_err_fn_ret(vlan_flow_del, ret);

			SLIST_REMOVE(&ctx->vlan_flow_list, i,
				     vlan_flow_entry, entries);

			ret = pon_net_dev_db_put(ctx->db, i->flow->igdev,
					   PON_NET_HANDLE_FILTER_INGRESS,
					   &i->handle, 1);
			if (ret != PON_ADAPTER_SUCCESS)
				dbg_err_fn_ret(pon_net_dev_db_put, ret);
			pool_type = vlan_flow_get_type(i->flow);
			ret = pon_net_dev_db_put(ctx->db, i->flow->igdev,
						 pool_type, &prio, 1);
			if (ret != PON_ADAPTER_SUCCESS)
				dbg_err_fn_ret(pon_net_dev_db_put, ret);

			free(i->flow);
			free(i);
		}
	}

	dbg_out();
}

enum pon_adapter_errno
pon_net_vlan_flow_clear_all(struct pon_net_context *ctx,
			    uint16_t me_id)
{
	dbg_in_args("%p, %u", ctx, me_id);

	if (!ctx->cfg.vlan_forwarding) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	/* Mark everything for deletion */
	pon_net_vlan_flow_list_mark_for_deletion(&ctx->vlan_flow_list, me_id);

	/* Execute deletion */
	pon_net_vlan_flow_clear_marked(ctx, me_id);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Get the ME ID of the bridge which services a given VLAN flow. */
static enum pon_adapter_errno
vlan_flow_bridge_get(struct pon_net_context *ctx,
		     uint8_t tp_type,
		     uint16_t tp_ptr,
		     uint16_t *bridge_me_id)
{
	struct pon_net_me_list_item *item;
	struct pon_net_bridge_port_config *bpc;
	uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;

	dbg_in_args("%p, %u, %u, %p", ctx, tp_type, tp_ptr, bridge_me_id);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		bpc = item->data;
		/* Find bridge based on tp data */
		if (tp_ptr != bpc->tp_ptr ||
		    tp_type != bpc->tp_type)
			continue;

		*bridge_me_id = bpc->bridge_me_id;

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	dbg_out_ret("%d", PON_ADAPTER_ERROR);
	return PON_ADAPTER_ERROR;
}

static enum pon_adapter_errno
vlan_flow_create_one(struct pon_net_context *ctx,
		     uint16_t me_id,
		     uint8_t tp_type,
		     uint16_t tp_ptr,
		     uint8_t uni_tp_type,
		     uint16_t uni_tp_ptr,
		     uint16_t tci,
		     uint16_t tci_mask,
		     bool untagged,
		     struct pon_net_vlan_flow_list *list)
{
	struct vlan_flow *flow;
	struct vlan_flow_entry *entry;
	enum pon_adapter_errno ret;
	enum pon_adapter_errno ret2;
	uint32_t handle, prio;
	enum pon_net_pool_type pool_type;
	bool exists;

	dbg_in_args("%p, %u, %u, %u, %u, %u, %u, %u, %d, %p", ctx, me_id,
		    tp_type, tp_ptr, uni_tp_type, uni_tp_ptr, tci,
		    tci_mask, untagged, list);

	flow = malloc(sizeof(struct vlan_flow));
	if (!flow)  {
		dbg_err_fn_ret(malloc, 0);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto cleanup;
	}
	memset(flow, 0, sizeof(struct vlan_flow));

	flow->untagged = untagged;
	if (!untagged) {
		flow->tci = tci;
		flow->tci_mask = tci_mask;
		flow->tpid = ETH_P_8021Q;
	}

	/* Set Ingress net_device to related UNI port */
	ret = pon_net_tp_ifname_get(ctx, uni_tp_type,
				    uni_tp_ptr,
				    flow->igdev,
				    sizeof(flow->igdev));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_tp_ifname_get, ret);
		goto cleanup_flow;
	}

	pool_type = vlan_flow_get_type(flow);

	ret = pon_net_dev_db_gen(ctx->db, flow->igdev, pool_type, &prio, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_dev_db_gen, ret);
		goto cleanup_flow;
	}

	ret = pon_net_dev_db_gen(ctx->db, flow->igdev,
				 PON_NET_HANDLE_FILTER_INGRESS, &handle, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_dev_db_gen, ret);
		goto cleanup_prio;
	}

	flow->prio = (uint16_t)prio;

	/* Set Egress net_device to related PMAPPER ME */
	ret = pon_net_tp_ifname_get(ctx, tp_type, tp_ptr,
				    flow->egdev,
				    sizeof(flow->egdev));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_tp_ifname_get, ret);
		goto cleanup_handle;
	}

	/* If 'flow' already exists, there is no need to create it again
	 * As all flows were previously marked for deletion, we unmark it so
	 * that it is not deleted afterwards
	 */
	exists = pon_net_vlan_flow_list_unmark_one(&ctx->vlan_flow_list, me_id,
						   flow);
	if (exists) {
		/* Entry exists - do not create */
		ret2 = pon_net_dev_db_put(ctx->db, flow->igdev,
					  PON_NET_HANDLE_FILTER_INGRESS,
					  &handle, 1);
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret2);
		ret2 = pon_net_dev_db_put(ctx->db, flow->igdev, pool_type,
					  &prio, 1);
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret2);

		free(flow);

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	entry = malloc(sizeof(struct vlan_flow_entry));
	if (!entry)  {
		dbg_err_fn_ret(malloc, 0);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto cleanup_entry;
	}

	ret = vlan_flow_add(ctx, flow);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(vlan_flow_add, ret);
		goto cleanup_entry;
	}

	entry->me_id = me_id;
	entry->flow = flow;
	entry->handle = handle;
	entry->marked_for_deletion = false;

	SLIST_INSERT_HEAD(list, entry, entries);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

cleanup_entry:
	free(entry);

cleanup_handle:
	ret2 = pon_net_dev_db_put(ctx->db, flow->igdev,
				  PON_NET_HANDLE_FILTER_INGRESS, &handle, 1);
	if (ret2 != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret2);
cleanup_prio:
	ret2 = pon_net_dev_db_put(ctx->db, flow->igdev, pool_type, &prio, 1);
	if (ret2 != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret2);
cleanup_flow:
	free(flow);

cleanup:
	dbg_out_ret("%d", ret);
	return ret;
}

/*
 * Termination point types on which VLAN aware forwarding rules will
 * be created
 */
static const uint16_t tp_types[] = {
	PA_BP_TP_TYPE_PPTP_UNI,
	PA_BP_TP_TYPE_VEIP,
	PA_BP_TP_TYPE_IP_HOST,
};

/*
 * Return true if forwarding rule should be created for this termination point
 */
static bool requires_vlan_forwarding(uint16_t tp_type)
{
	dbg_in_args("%u", tp_type);

	for (unsigned int i = 0; i < ARRAY_SIZE(tp_types); ++i) {
		if (tp_types[i] == tp_type) {
			dbg_out_ret("%d", true);
			return true;
		}
	}

	dbg_out_ret("%d", false);
	return false;
}

static enum pon_adapter_errno
vlan_flow_uni_foreach(struct pon_net_context *ctx,
		      uint16_t me_id,
		      uint8_t tp_type,
		      uint16_t tp_ptr,
		      uint16_t tci,
		      uint16_t tci_mask,
		      bool untagged,
		      struct pon_net_vlan_flow_list *list)
{
	uint16_t bridge_me_id;
	struct pon_net_me_list_item *item;
	struct pon_net_bridge_port_config *bpc;
	uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;
	enum pon_adapter_errno ret;
	char ifname[IF_NAMESIZE];

	dbg_in_args("%p, %u, %u, %u, %u, %u, %d, %p", ctx, me_id, tp_type,
		    tp_ptr, tci, tci_mask, untagged, list);

	ret = vlan_flow_bridge_get(ctx, tp_type, tp_ptr, &bridge_me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		/* Skip if pmapper is not connected */
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		bpc = item->data;
		/* Skip bridge port connected to different bridge */
		if (bpc->bridge_me_id != bridge_me_id)
			continue;

		/* Skip UNI types which do not need VLAN forwarding */
		if (!requires_vlan_forwarding(bpc->tp_type))
			continue;

		ret = pon_net_tp_ifname_get(ctx, bpc->tp_type,
					    bpc->tp_ptr,
					    ifname,
					    sizeof(ifname));
		if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
			/* If MAC Bridge Port Configuration Data ME exist,
			 * but the related ME pointer by tp_ptr doesn't,
			 * then skip setting up filters on this port.
			 * This can happen during cleanup, when for example
			 * PPTP Ethernet UNI ME can be deleted before
			 * MAC Bridge Port Configuration Data ME
			 */
			continue;
		} else if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_tp_ifname_get, ret);
			return ret;
		}

		ret = vlan_flow_create_one(ctx, me_id, tp_type, tp_ptr,
					   bpc->tp_type, bpc->tp_ptr, tci,
					   tci_mask, untagged, list);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(vlan_flow_create_one, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Checks if newest element added to list is duplicate of another element */
static bool is_duplicate(const uint16_t *list,
			 int newest_element_idx)
{
	int i;

	dbg_in_args("%p, %u", list, newest_element_idx);

	for (i = 0; i < newest_element_idx; i++) {
		if (list[i] == list[newest_element_idx])
			return true;
	}

	return false;
}

/*
 * This function creates the forwarding rules from (v)UNI ports to GEM ports
 * and pmappers using the mirred tc action which are in the same bridge.
 * These rules are needed to forward the traffic when learning is deactivated.
 * We create one rule for each passing VLAN filter rule.
 *
 * It is called in the following conditions.
 * 1. A new rule is created
 * 2. A new (v)UNI port is added to the bridge.
 * 3. A pmapper tc configuration is reloaded.
 */
static enum pon_adapter_errno
vlan_flow_create_all(struct pon_net_context *ctx,
		     uint16_t me_id,
		     const uint16_t *tci_list,
		     uint8_t tci_list_len,
		     uint16_t tci_mask,
		     bool untagged,
		     struct pon_net_vlan_flow_list *list)
{
	uint8_t tp_type;
	uint16_t tp_ptr;
	int i;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint16_t vlan_id[MAX_TCIS] = { 0 };

	dbg_in_args("%p, %u, %p, %u, %u, %d, %p", ctx, me_id, tci_list,
		    tci_list_len, tci_mask, untagged, list);

	ret = pon_net_tp_get(ctx, me_id, &tp_type, &tp_ptr);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_tp_get, ret);
		return ret;
	}

	/* Create VLAN flow only for PMAPPER and normal GEM net_devices
	 * (not broadcast only GEM ports)
	 */
	if (tp_type != PA_BP_TP_TYPE_PMAP &&
	    !(tp_type == PA_BP_TP_TYPE_GEM &&
	 pon_net_gem_dir_get(ctx, tp_ptr) != PA_GEMPORTNETCTP_DIR_DOWNSTREAM)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (tci_list_len > MAX_TCIS) {
		dbg_err("tci_list_len is too big, received %u, maximum is %u\n",
			tci_list_len, MAX_TCIS);
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	for (i = 0; i < tci_list_len; ++i) {
#if defined(VLAN_FORWARDING_PCP)
		vlan_id[i] = tci_list[i] & (TCI_MASK_VID | TCI_MASK_PCP);
#else
		vlan_id[i] = TCI_VID(tci_list[i]);
#endif

		/* To avoid duplicate rules check if vlan id calculated based on
		 * current tci is the same as previously used
		 */
		if (is_duplicate(vlan_id, i))
			continue;

		ret = vlan_flow_uni_foreach(ctx, me_id, tp_type, tp_ptr,
					    tci_list[i], tci_mask, false, list);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, vlan_flow_uni_foreach, ret);
			return ret;
		}
	}

	if (untagged) {
		ret = vlan_flow_uni_foreach(ctx, me_id, tp_type, tp_ptr,
					    0, 0, true, list);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, vlan_flow_uni_foreach, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
enable_bridge_learning(struct pon_net_context *ctx,
		       uint16_t me_id)
{
	uint8_t tp_type;
	uint16_t tp_ptr;
	char dev[IF_NAMESIZE];
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u", ctx, me_id);

	ret = pon_net_tp_get(ctx, me_id, &tp_type, &tp_ptr);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_tp_get, ret);
		return ret;
	}

	ret = pon_net_tp_ifname_get(ctx, tp_type, tp_ptr,
				    dev, sizeof(dev));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_tp_ifname_get, ret);
		return ret;
	}

	ret = pon_net_rtnl_bridge_learning_set(ctx, dev, true);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_rtnl_bridge_learning_set, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_vlan_forwarding_update(struct pon_net_context *ctx,
				 uint16_t me_id,
				 const uint16_t *tci_list,
				 uint8_t tci_list_len,
				 uint16_t tci_mask,
				 bool untagged)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %p, %u, %u, %d", ctx, me_id, tci_list,
		    tci_list_len, tci_mask, untagged);

	if (!ctx->cfg.vlan_forwarding) {
		ret = enable_bridge_learning(ctx, me_id);
		dbg_out_ret("%d", ret);
		return ret;
	}

	/* Mark every entry for this 'me_id' for deletion */
	pon_net_vlan_flow_list_mark_for_deletion(&ctx->vlan_flow_list, me_id);

	if (!pon_net_mac_bridge_port_filter_preassign_table_configured(ctx,
								       me_id)) {
		/* Create new entries - duplicate entries will be unmarked */
		ret = vlan_flow_create_all(ctx, me_id, tci_list, tci_list_len,
					   tci_mask, untagged,
					   &ctx->vlan_flow_list);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, vlan_flow_create_all, ret);
			return ret;
		}
	}

	/* Delete those entries, which were not unmarked */
	pon_net_vlan_flow_clear_marked(ctx, me_id);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_vlan_flow_ops pon_net_vlan_flow_ops = {
	.flow_add = vlan_flow_add,
	.flow_del = vlan_flow_del,
};
