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

#include <netlink/route/link.h>
#include <netlink/route/link/tcont.h>
#include <netlink/route/tc.h>
#include <netlink/route/qdisc.h>
#include <pon_adapter.h>

#include "../pon_net_netlink.h"
#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "../pon_net_qdisc.h"
#include "../pon_net_qdisc_build.h"
#include "../pon_net_dev_db.h"
#include "../pon_net_dev_defs.h"
#include "pon_net_tcont.h"
#include "pon_net_onu_g.h"
#include <omci/me/pon_adapter_tcont.h>
#include <omci/me/pon_adapter_priority_queue.h>
#include <omci/me/pon_adapter_gem_port_network_ctp.h>
#include <omci/me/pon_adapter_onu_g.h>

enum pon_adapter_errno
pon_net_tcont_qdiscs_mixed_mode_set(struct pon_net_context *ctx,
				    const uint16_t *me_ids, unsigned int len)
{
	struct tcont *tcont;
	struct pon_net_me_list_item *item;
	uint16_t class_id = PON_CLASS_ID_TCONT;

	dbg_in_args("%p, %p, %u", ctx, me_ids, len);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		tcont = item->data;

		if (me_ids && !pon_net_me_id_in_array(item->me_id, me_ids, len))
			continue;

		if (pon_net_traffic_management_option_get(ctx) == PA_TMO_RATE)
			tcont->tree_builder = &pon_net_mixed_tbf_builder;
		else
			tcont->tree_builder = &pon_net_mixed_builder;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pon_net_tcont_qdiscs_update(struct pon_net_context *ctx,
						   const uint16_t *me_ids,
						   unsigned int len)
{
	enum pon_adapter_errno ret;
	struct tcont *tcont;
	struct pon_net_me_list_item *item;
	uint16_t class_id = PON_CLASS_ID_TCONT;

	dbg_in_args("%p, %p, %u", ctx, me_ids, len);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		if (me_ids && !pon_net_me_id_in_array(item->me_id, me_ids, len))
			continue;

		ret = pon_net_queue_unassign(ctx, PON_CLASS_ID_TCONT,
					     item->me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_queue_unassign, ret);
			return ret;
		}
	}

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		tcont = item->data;

		if (me_ids && !pon_net_me_id_in_array(item->me_id, me_ids, len))
			continue;

		ret = pon_net_qdiscs_build_and_diff_apply(tcont->tree,
							  PON_CLASS_ID_TCONT,
							  item->me_id,
							  tcont->tree_builder,
							  false, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_qdiscs_build_and_diff_apply,
				   ret);
			return ret;
		}
	}

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		if (me_ids && !pon_net_me_id_in_array(item->me_id, me_ids, len))
			continue;

		ret =
		    pon_net_queue_update(ctx, PON_CLASS_ID_TCONT, item->me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_queue_update, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pon_net_tcont_qdiscs_cleanup(struct pon_net_context *ctx,
						    const uint16_t *me_ids,
						    unsigned int len)
{
	enum pon_adapter_errno ret;
	struct tcont *tcont;
	struct pon_net_me_list_item *item;
	uint16_t class_id = PON_CLASS_ID_TCONT;

	dbg_in_args("%p, %p, %u", ctx, me_ids, len);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		if (me_ids && !pon_net_me_id_in_array(item->me_id, me_ids, len))
			continue;

		ret = pon_net_queue_unassign(ctx, PON_CLASS_ID_TCONT,
					     item->me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_queue_unassign, ret);
			return ret;
		}
	}

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		tcont = item->data;

		if (me_ids && !pon_net_me_id_in_array(item->me_id, me_ids, len))
			continue;

		ret = pon_net_qdiscs_build_and_diff_apply(tcont->tree,
							  PON_CLASS_ID_TCONT,
							  item->me_id,
							  NULL,
							  0, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_qdiscs_build_and_diff_apply,
				   ret);
			return ret;
		}
	}

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		if (me_ids && !pon_net_me_id_in_array(item->me_id, me_ids, len))
			continue;

		ret =
		    pon_net_queue_update(ctx, PON_CLASS_ID_TCONT, item->me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_queue_update, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* It will create T-CONT entry in ME list if it doesn't exist.
   This way whenever tcont_update() is called, the entry will
   always be available */
static enum pon_adapter_errno
me_list_tcont_read_or_create(struct pon_net_me_list *list, uint16_t me_id,
			     struct tcont *tcont)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %d, %p", list, me_id, tcont);

	ret = pon_net_me_list_read(list,
				   PON_CLASS_ID_TCONT,
				   me_id,
				   tcont,
				   sizeof(*tcont));
	if (ret == PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	tcont->tree = pon_net_qdiscs_create();
	if (!tcont->tree) {
		dbg_err_fn_ret(pon_net_qdiscs_create, 0);
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_MEMORY);
		return PON_ADAPTER_ERR_NO_MEMORY;
	}

	tcont->me_id = me_id;

	ret = pon_net_me_list_write(list,
				    PON_CLASS_ID_TCONT,
				    me_id,
				    tcont,
				    sizeof(*tcont));
	if (ret != PON_ADAPTER_SUCCESS) {
		pon_net_qdiscs_destroy(tcont->tree);
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Creates T-CONT netdevice and adds it to ctx->db */
static enum pon_adapter_errno create_device(struct pon_net_context *ctx,
					    const uint16_t me_id,
					    uint16_t alloc_id)
{
	char ifname[IF_NAMESIZE];
	enum pon_adapter_errno ret;
	int written;

	dbg_in_args("%p, %u, %u", ctx, me_id, alloc_id);

	written = snprintf(ifname, sizeof(ifname), PON_IFNAME_TCONT, me_id);
	if (written >= (int)sizeof(ifname)) {
		FN_ERR_RET(written, snprintf, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = pon_net_dev_db_add(ctx->db, ifname, &pon_net_tcont_def);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_add, ret);
		return ret;
	}

	ret = netlink_tcont_create(ctx->netlink, me_id, alloc_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		pon_net_dev_db_del(ctx->db, ifname);
		FN_ERR_RET(ret, netlink_tcont_create, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Destroy the T-CONT device and remove it from ctx->db */
static enum pon_adapter_errno destroy_device(struct pon_net_context *ctx,
					     const uint16_t me_id)
{
	char ifname[IF_NAMESIZE];
	enum pon_adapter_errno ret;
	int written;

	dbg_in_args("%p, %u", ctx, me_id);

	written = snprintf(ifname, sizeof(ifname), PON_IFNAME_TCONT, me_id);
	if (written >= (int)sizeof(ifname)) {
		FN_ERR_RET(written, snprintf, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	pon_net_dev_db_del(ctx->db, ifname);

	ret = netlink_tcont_destroy(ctx->netlink, me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_tcont_destroy, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
tcont_update(void *ll_handle,
	     uint16_t me_id,
	     uint16_t policy,
	     uint16_t alloc_id,
	     uint8_t create_only)
{
	struct pon_net_context *ctx = ll_handle;
	char ifname[IF_NAMESIZE];
	struct tcont tcont = {0};
	uint32_t alloc_id_old;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int cleanup_netdev = 0;
	uint32_t tmp;
	/* The default policy that we will setup is strict priority,
	   later it may be changed to wrr, when Priority Queue's weight is
	   updated */
	const struct pon_net_tree_builder *builder_to_setup =
	    pon_net_get_default_qdisc_tree_builder(ctx);

	dbg_in_args("%p, %u, %u, %u, %u", ll_handle, me_id, policy,
		    alloc_id, create_only);

	if (create_only) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	snprintf(ifname, sizeof(ifname), PON_IFNAME_TCONT, me_id);

	ret = me_list_tcont_read_or_create(&ctx->me_list, me_id, &tcont);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, create_me_list_entry_if_not_created, ret);
		return ret;
	}

	ret = mapper_index_get(ctx->mapper[MAPPER_TCONT_MEID_TO_ALLOC_ID],
			       me_id, &alloc_id_old);
	if (ret == PON_ADAPTER_SUCCESS && alloc_id_old == alloc_id) {
		/* Nothing changed */
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}
	if (ret == PON_ADAPTER_SUCCESS && alloc_id_old != alloc_id) {
		ret = mapper_id_get(ctx->mapper[MAPPER_TCONT_MEID_TO_ALLOC_ID],
				    &tmp, alloc_id);
		if (ret != PON_ADAPTER_ERR_NOT_FOUND) {
			dbg_out_ret("%d", PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		/* Keep the current builder in builder_to_setup -
		 * in order to restore later */
		builder_to_setup = tcont.tree_builder;
		tcont.tree_builder = NULL;
		ret = pon_net_qdiscs_build_and_diff_apply(tcont.tree,
							  PON_CLASS_ID_TCONT,
							  me_id,
							  NULL,
							  1, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_qdiscs_build_and_diff_apply,
				   ret);
			return ret;
		}

		/* The alloc ID for this ME changed - remove link */
		ret = destroy_device(ctx, me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_out_ret("%d", ret);
			return ret;
		}

		/* Remove old me_id/alloc_id map */
		ret = mapper_id_remove(
			ctx->mapper[MAPPER_TCONT_MEID_TO_ALLOC_ID],
			me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_out_ret("%d", ret);
			return ret;
		}
	}

	ret = mapper_explicit_map(ctx->mapper[MAPPER_TCONT_MEID_TO_ALLOC_ID],
				  me_id, alloc_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, mapper_explicit_map, ret);
		return ret;
	}

	ret = create_device(ctx, me_id, alloc_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(create_device, ret);
		goto cleanup;
	}
	cleanup_netdev = 1;

	/* If clsact qdisc already exist, remove it to clear it from all
	 * filters and add it again. It is faster than removing filters
	 * manually.
	 */
	ret = netlink_qdisc_clsact_destroy(ctx->netlink, ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_clsact_destroy, ret);
		goto cleanup;
	}

	/* Create clsact qdisc with no filters */
	ret = netlink_qdisc_clsact_create(ctx->netlink, ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_clsact_create, ret);
		goto cleanup;
	}

	tcont.tree_builder = builder_to_setup;

	ret = pon_net_qdiscs_build_and_diff_apply(tcont.tree,
						  PON_CLASS_ID_TCONT,
						  me_id,
						  tcont.tree_builder,
						  1, ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_qdiscs_build_and_diff_apply,
			       ret);
		goto cleanup;
	}

	ret = pon_net_me_list_write(&ctx->me_list, PON_CLASS_ID_TCONT, me_id,
				    &tcont, sizeof(tcont));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_me_list_write, ret);
		goto cleanup;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

cleanup:
	tcont.tree_builder = NULL;
	pon_net_qdiscs_build_and_diff_apply(tcont.tree, PON_CLASS_ID_TCONT,
					    me_id, NULL, 1, ctx);
	if (tcont.tree)
		pon_net_qdiscs_destroy(tcont.tree);
	pon_net_me_list_remove(&ctx->me_list, PON_CLASS_ID_TCONT, me_id);
	if (cleanup_netdev)
		destroy_device(ctx, me_id);
	mapper_id_remove(ctx->mapper[MAPPER_TCONT_MEID_TO_ALLOC_ID], me_id);
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tcont_destroy(void *ll_handle,
	      uint16_t me_id,
	      uint16_t alloc_id,
	      uint8_t deact_mode)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret;
	uint32_t alloc_id_old;
	struct tcont tcont = {0};

	dbg_in_args("%p, %u, %u, %u", ll_handle, me_id, alloc_id, deact_mode);

	ret = mapper_index_get(ctx->mapper[MAPPER_TCONT_MEID_TO_ALLOC_ID],
			       me_id, &alloc_id_old);
	if (ret == PON_ADAPTER_SUCCESS) {
		/* Destroying netdevice, destroys tc filters and qdiscs related
		 * to the tcont network device */
		ret = destroy_device(ctx, me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_out_ret("%d", ret);
			return ret;
		}

		/* Remove cached queue assignments on the GEMs */
		pon_net_queue_unassign(ctx, PON_CLASS_ID_TCONT, me_id);

		ret = pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_TCONT,
					   me_id, &tcont, sizeof(tcont));
		if (ret == PON_ADAPTER_SUCCESS)
			pon_net_qdiscs_destroy(tcont.tree);

		ret = mapper_id_remove(
				ctx->mapper[MAPPER_TCONT_MEID_TO_ALLOC_ID],
				me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_out_ret("%d", ret);
			return ret;
		}

		pon_net_me_list_remove(&ctx->me_list,
				       PON_CLASS_ID_TCONT, me_id);
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_tcont_ops tcont_ops = {
	.update = tcont_update,
	.destroy = tcont_destroy,
};
