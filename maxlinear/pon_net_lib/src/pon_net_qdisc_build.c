/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#include <omci/me/pon_adapter_priority_queue.h>
#include <omci/me/pon_adapter_gem_port_network_ctp.h>

#include "me/pon_net_gem_port_network_ctp.h"
#include "pon_net_qdisc_build.h"
#include "pon_net_common.h"
#include "pon_net_debug.h"
#include "pon_net_qdisc.h"
#include "pon_net_uni.h"

static double to_double(uint8_t probability)
{
	return (probability + 1.0) / 256.0;
}

static bool is_upstream(uint16_t me_id)
{
	return !!(me_id & 0x8000);
}

/*
 * Add two consecutive tc-red qdiscs as children of parent qdisc
 * to implement WRED
 */
static struct pon_net_qdisc *
add_two_red_qdiscs(struct pon_net_qdiscs *qdiscs,
		   const struct pon_net_priority_queue *pq,
		   struct pon_net_qdisc *parent_qdisc)
{
	struct pon_net_qdisc *qdisc = NULL;
	struct pon_net_qdisc *pq_qdisc = NULL;
	uint32_t first_red_handle = PON_WRED_QDISC_0(pq->index);
	uint32_t second_red_handle = PON_WRED_QDISC_1(pq->index);
	const struct pon_net_red_type_data first_params = {
		.min = pq->pkt_drop_q_thr_green_min,
		.max = pq->pkt_drop_q_thr_green_max,
		.probability = to_double(pq->pkt_drop_probability_green),
	};
	const struct pon_net_red_type_data second_params = {
		.min = pq->pkt_drop_q_thr_yellow_min,
		.max = pq->pkt_drop_q_thr_yellow_max,
		.probability = to_double(pq->pkt_drop_probability_yellow),
	};

	dbg_in_args("%p, %p, %p", qdiscs, pq, parent_qdisc);

	if (first_red_handle > 0xFFFF) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return NULL;
	}
	pq_qdisc = pon_net_qdiscs_get_or_add(qdiscs, (uint16_t)first_red_handle,
					     &pon_net_red_type,
					     &first_params,
					     parent_qdisc);
	if (!pq_qdisc) {
		FN_ERR_RET(0, pon_net_qdiscs_get_or_add, 0);
		return NULL;
	}

	if (second_red_handle > 0xFFFF) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return NULL;
	}
	qdisc = pon_net_qdiscs_get_or_add(qdiscs, (uint16_t)second_red_handle,
					  &pon_net_red_type,
					  &second_params,
					  pq_qdisc);
	if (!qdisc) {
		FN_ERR_RET(0, pon_net_qdiscs_get_or_add, 0);
		return NULL;
	}

	dbg_out_ret("%p", pq_qdisc);
	return pq_qdisc;
}

/* Add a tc-tbf qdisc as a child of a yellow (second) tc-red qdisc */
static struct pon_net_qdisc *
add_tbf_qdisc(struct pon_net_qdiscs *qdiscs,
	      const struct pon_net_priority_queue *pq,
	      const struct pon_net_traffic_descriptor *td)
{
	struct pon_net_qdisc *pq_qdisc;
	struct pon_net_qdisc *parent_qdisc;
	uint32_t tbf_handle = PON_TBF_QDISC_HANDLE(pq->index);
	uint16_t parent_handle = PON_WRED_QDISC_1(pq->index);
	const struct pon_net_tbf_type_data tbf_params = {
		.cir = (int)td->cir,
		.pir = (int)td->pir,
		.cbs = (int)td->cbs,
		.pbs = (int)td->pbs
	};

	dbg_in_args("%p, %p, %p", qdiscs, pq, td);

	parent_qdisc = qdiscs_get_by_handle(qdiscs, parent_handle);
	if (!parent_qdisc) {
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	pq_qdisc = pon_net_qdiscs_get_or_add(qdiscs, (uint16_t)tbf_handle,
					     &pon_net_tbf_type,
					     &tbf_params,
					     parent_qdisc);

	dbg_out_ret("%p", pq_qdisc);
	return pq_qdisc;
}

static void remove_drr_classes_with_quantum_0(struct pon_net_qdiscs *qdiscs)
{
	unsigned int i = 0;
	unsigned int j = 0;

	dbg_in_args("%p", qdiscs);

	for (i = 0; i < pon_net_qdiscs_len(qdiscs); ++i) {
		struct pon_net_qdisc *qdisc = pon_net_qdiscs_get(qdiscs, i);

		if (!qdisc)
			continue;

		if (qdisc->type != &pon_net_drr_type)
			continue;

		for (j = 0; j < ARRAY_SIZE(qdisc->classes); ++j) {
			struct qdisc_class *class = &qdisc->classes[j];

			if (class->state == CLASS_USED && class->arg == 0)
				class->state = CLASS_UNUSED;
		}
	}

	dbg_out();
}

/*
 * Handle generator is used for generating qdisc handles for additional
 * qdiscs in partitioning process (see pon_net_qdiscs_partition() for details)
 */
static uint16_t handle_generator(unsigned int i, void *arg)
{
	uint16_t *handle_ptr = arg;
	uint16_t handle = *handle_ptr;

	uint16_t type = PON_QDISC_HANDLE_TYPE(handle);
	uint16_t s = PON_QDISC_TS_DATA_S(handle);
	uint16_t tsid = PON_QDISC_TS_DATA_TSID(handle);
	uint16_t type_data = PON_QDISC_TS_DATA(s, tsid, (uint16_t)i);

	if (type == PON_QDISC_ROOT)
		type = PON_QDISC_ROOT_AUX;
	if (type == PON_QDISC_TS)
		type = PON_QDISC_TS_AUX;

	return PON_QDISC_HANDLE(type, type_data);
}

/*
 * Recursively partition qdiscs, if their number of classes exceeds maximum
 * supported number of queues by the driver.
 */
static enum pon_adapter_errno
represent_many_queues_with_multiple_qdiscs(struct pon_net_qdiscs *qdiscs,
					   unsigned int idx)
{
	struct pon_net_qdisc *qdisc = NULL;
	unsigned int i = 0;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %d", qdiscs, i);

	qdisc = pon_net_qdiscs_get(qdiscs, idx);
	if (!qdisc) {
		FN_ERR_RET(0, pon_net_qdiscs_get, PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	/* Partition "this" qdisc */
	if (pon_net_qdisc_classes_last(qdisc) > MAX_QUEUES) {
		uint16_t handle = (uint16_t)qdisc->handle;

		ret = pon_net_qdiscs_partition(qdiscs, idx, MAX_QUEUES,
					       handle_generator,
					       &handle);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_qdiscs_partition, ret);
			return ret;
		}
	}

	/* Partition children recursively */
	for (i = 0; i < ARRAY_SIZE(qdisc->classes); i++) {
		struct qdisc_class *class = &qdisc->classes[i];

		if (class->state != CLASS_USED)
			continue;

		ret = represent_many_queues_with_multiple_qdiscs(qdiscs,
							      class->child_idx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret,
				   represent_many_queues_with_multiple_qdiscs,
				   ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Iterate over Priority Queues in the ME list, and use those whose Related Port
 * matches me_id to build "qdiscs" tree
 */
static enum pon_adapter_errno
build_from_priority_queues(struct pon_net_qdiscs *qdiscs,
			   struct pon_net_me_list *me_list, uint16_t me_id,
			   bool enable_wrr, bool add_tbfs)
{
	enum pon_adapter_errno ret;
	struct pon_net_me_list_item *item;
	struct pon_net_me_list_item *gem_item;
	uint32_t quantum;
	struct pon_net_traffic_scheduler *ts = NULL;
	struct pon_net_qdisc *qdisc = NULL;
	struct pon_net_qdisc *pq_qdisc = NULL;
	int i;
	struct pon_net_priority_queue *priority_queue;

	dbg_in_args("%p, %u, %p", me_list, me_id, qdiscs);

	/* Create the root qdisc if it doesn't exist */
	qdisc = pon_net_qdiscs_get_or_add(qdiscs, PON_ROOT_QDISC,
					  &pon_net_prio_type, NULL,
					  NULL);
	if (!qdisc) {
		FN_ERR_RET(0, pon_net_qdisc_get_or_add, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	/* Read all priority queues for this T-CONT */
	pon_net_me_list_foreach_class(me_list,
				      PON_CLASS_ID_PRIORITY_QUEUE,
				      item) {
		priority_queue = item->data;

		if (priority_queue->associated_me_id != me_id)
			continue;

		/* Create the root qdisc if it doesn't exist */
		qdisc = pon_net_qdiscs_get_or_add(qdiscs, PON_ROOT_QDISC,
						  &pon_net_prio_type, NULL,
						  NULL);
		if (!qdisc) {
			FN_ERR_RET(0, pon_net_qdisc_get_or_add,
				   PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		if ((int)priority_queue->chain_length < 0) {
			dbg_out_ret("%d", PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		/* We iterate in reversed order over the chain to create
		 * parents before children */
		for (i = (int)priority_queue->chain_length - 1; i >= 0; --i) {
			ts = &priority_queue->chain[i];

			qdisc =
			  pon_net_qdiscs_get_or_add(qdiscs,
						    PON_TS_QDISC(ts->me_id),
						    &pon_net_prio_type,
						    NULL, qdisc);
			if (!qdisc) {
				FN_ERR_RET(0, pon_net_qdisc_get_or_add,
					   PON_ADAPTER_ERROR);
				return PON_ADAPTER_ERROR;
			}
		}

		quantum = pon_net_calculate_quantum(priority_queue->weight);

		/* Here the qdisc represents the Traffic Scheduler
		 * "closest" to the Priority Queue.
		 * The weight of the Priority Queue != 0, so we must switch
		 * this qdisc to drr. */
		if (enable_wrr && quantum) {
			ret = pon_net_qdisc_type_set(qdisc, &pon_net_drr_type,
						     NULL);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, pon_net_qdisc_type_set, ret);
				return ret;
			}
		}

		pq_qdisc = add_two_red_qdiscs(qdiscs, priority_queue, qdisc);
		if (!pq_qdisc) {
			FN_ERR_RET(0, add_two_red_qdiscs, PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		pq_qdisc->parent_queue = priority_queue->queue;

		pq_qdisc->parent_arg = quantum;

		if (add_tbfs) {
			pon_net_me_list_foreach_class(
			    me_list, PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
			    gem_item) {
				struct pon_net_gem_port_net_ctp *gem;
				struct pon_net_traffic_descriptor *td;
				uint16_t td_me_id;

				gem = gem_item->data;
				if (is_upstream(priority_queue->me_id)) {
					if (gem->tcont_me_id !=
					    priority_queue->associated_me_id)
						continue;
					td_me_id = gem->us_td_me_id;
				} else {
					if (gem->ds_priority_queue_me_id !=
					    priority_queue->me_id)
						continue;
					td_me_id = gem->ds_td_me_id;
				}

				td = pon_net_me_list_get_data(
				    me_list, PON_CLASS_ID_TRAFFIC_DESCRIPTOR,
				    td_me_id);
				if (td) {
					pq_qdisc = add_tbf_qdisc(
					    qdiscs, priority_queue, td);
					if (!pq_qdisc) {
						FN_ERR_RET(0, add_tbf_qdisc,
							   PON_ADAPTER_ERROR);
						return PON_ADAPTER_ERROR;
					}
				}
			}
		}
	}

	ret = pon_net_qdiscs_change_drr_parents_to_prio(qdiscs);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_qdiscs_change_drr_parent_to_prio,
			   ret);
		return ret;
	}

	ret = pon_net_qdiscs_assign_to_classes(qdiscs);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_qdiscs_assign_to_classes, ret);
		return ret;
	}

	remove_drr_classes_with_quantum_0(qdiscs);

	ret = represent_many_queues_with_multiple_qdiscs(qdiscs, 0);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, represent_many_queues_with_multiple_qdiscs,
			   ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Iterate over Priority Queues in the ME list, and use those whose Related Port
 * matches me_id to build "qdiscs" tree
 */
static enum pon_adapter_errno
build_qos_tree_ext_uni_ds(struct pon_net_qdiscs *qdiscs,
			  struct pon_net_me_list *me_list,
			  uint16_t me_id)
{
	uint16_t lport;
	struct pon_net_me_list_item *item;
	struct pon_net_priority_queue *priority_queue;
	struct pon_net_qdisc *qdisc = NULL, *sp_qdisc = NULL, *pq_qdisc = NULL;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %p", me_list, me_id, qdiscs);

	ret = pon_net_uni_lport_get(me_id, &lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_uni_lport_get, ret);
		return ret;
	}

	/* Create the root qdisc if it doesn't exist */
	qdisc = pon_net_qdiscs_get_or_add(qdiscs, PON_ROOT_QDISC,
					  &pon_net_drr_type, NULL, NULL);
	if (!qdisc) {
		FN_ERR_RET(0, pon_net_qdisc_get_or_add, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	/* Create Strict Priority Queues */
	sp_qdisc = pon_net_qdiscs_get_or_add(qdiscs,
					PON_QDISC_HANDLE(PON_QDISC_TS, me_id),
					&pon_net_prio_type, NULL, qdisc);
	if (!sp_qdisc) {
		FN_ERR_RET(0, pon_net_qdisc_get_or_add, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	sp_qdisc->parent_queue = PON_ROOT_CLASS_QDISC(lport + 1U);
	sp_qdisc->parent_arg = pon_net_calculate_quantum(DEF_DRR_WEIGHT);

	/* Read all priority queues for this T-CONT */
	pon_net_me_list_foreach_class(me_list,
				      PON_CLASS_ID_PRIORITY_QUEUE,
				      item) {
		priority_queue = item->data;

		if (priority_queue->associated_me_id != me_id)
			continue;

		pq_qdisc = add_two_red_qdiscs(qdiscs, priority_queue, sp_qdisc);
		if (!pq_qdisc) {
			FN_ERR_RET(0, add_two_red_qdiscs, PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
		pq_qdisc->parent_queue = priority_queue->queue;
		pq_qdisc->parent_arg =
			pon_net_calculate_quantum(DEF_DRR_WEIGHT);
	}

	ret = pon_net_qdiscs_assign_to_classes(qdiscs);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_qdiscs_assign_to_classes, ret);
		return ret;
	}

	remove_drr_classes_with_quantum_0(qdiscs);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Builds pon_net_qdiscs for strict priority */
static enum pon_adapter_errno
sp_build(struct pon_net_me_list *me_list, uint16_t me_id,
	 struct pon_net_qdiscs *qdiscs)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %p", me_list, me_id, qdiscs);

	ret = build_from_priority_queues(qdiscs, me_list, me_id, false, false);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, build_from_priority_queues, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pon_net_tree_builder pon_net_sp_builder = {
	.build = sp_build
};

/* Builds pon_net_qdiscs for strict priority + WRR */
static enum pon_adapter_errno mixed_build(struct pon_net_me_list *me_list,
					  uint16_t me_id,
					  struct pon_net_qdiscs *qdiscs)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %p", me_list, me_id, qdiscs);

	ret = build_from_priority_queues(qdiscs, me_list, me_id, true, false);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, build_from_priority_queues, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pon_net_tree_builder pon_net_mixed_builder = {
	.build = mixed_build
};

static enum pon_adapter_errno sp_tbf_build(struct pon_net_me_list *me_list,
					   uint16_t me_id,
					   struct pon_net_qdiscs *qdiscs)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %p", me_list, me_id, qdiscs);

	ret = build_from_priority_queues(qdiscs, me_list, me_id, false, true);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, build_from_priority_queues, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pon_net_tree_builder pon_net_sp_tbf_builder = {
	.build = sp_tbf_build
};

static enum pon_adapter_errno ext_uni_ds_build(struct pon_net_me_list *me_list,
					       uint16_t me_id,
					       struct pon_net_qdiscs *qdiscs)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %p", me_list, me_id, qdiscs);

	ret = build_qos_tree_ext_uni_ds(qdiscs, me_list, me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, build_from_priority_queues, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pon_net_tree_builder pon_net_ext_uni_ds_builder = {
	.build = ext_uni_ds_build
};

static enum pon_adapter_errno mixed_tbf_build(struct pon_net_me_list *me_list,
					      uint16_t me_id,
					      struct pon_net_qdiscs *qdiscs)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %p", me_list, me_id, qdiscs);

	ret = build_from_priority_queues(qdiscs, me_list, me_id, true, true);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, build_from_priority_queues, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pon_net_tree_builder pon_net_mixed_tbf_builder = {
	.build = mixed_tbf_build
};

static enum pon_adapter_errno qdiscs_diff_apply(struct pon_net_qdiscs *new,
						struct pon_net_qdiscs *old,
						uint16_t class_id,
						uint16_t me_id,
						struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE] = {0};

	dbg_in_args("%p, %p, %u, %p", new, old, me_id, ctx);

	ret = pon_net_ext_uni_ifname_get(ctx, class_id, me_id, ifname,
					 sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_ext_uni_ifname_get, ret);
		goto error;
	}

	/* Apply only the difference in configuration */
	ret = pon_net_qdiscs_diff_apply(new, old, ifname, ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_qdiscs_diff_apply, ret);
		goto error;
	}

error:
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
qdiscs_diff_apply_with_reassigning_gems(struct pon_net_qdiscs *new,
					struct pon_net_qdiscs *old,
					uint16_t class_id,
					uint16_t me_id,
					struct pon_net_context *ctx)
{
	/* TODO: Get rid of the GEM assignment/reassignment functions from here
	 *       once the driver starts to support it.
	 *
	 * Specifically: pon_net_gem_queue_unassign(),
	 * pon_net_gem_queue_update()
	 *
	 * The tc filters are added to clsact qdisc and not to the qdiscs
	 * that we are adding/deleting here. So technically there should be
	 * possibility to change qdisc tree independently from the
	 * filters configured on clsact.
	 *
	 * But the driver does not allow us to modify qdisc configuration when
	 * queue related tc filters are present.
	 * We must remove the tc filters before we change the qdiscs and
	 * add them again. This workaround should be removed if the driver
	 * is changed.
	 */

	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p, %u, %p", new, old, me_id, ctx);

	if (pon_net_qdiscs_len(old)) {
		ret = pon_net_queue_unassign(ctx, class_id, me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(pon_net_queue_unassign, ret);
			goto error;
		}
	}

	/* Apply only the difference in configuration */
	ret = qdiscs_diff_apply(new, old, class_id, me_id, ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(qdiscs_diff_apply, ret);
		goto error;
	}

	if (pon_net_qdiscs_len(new)) {
		/* We have deleted old and added new qdiscs. Add TC filters
		 * that we have previously removed */
		ret = pon_net_queue_update(ctx, class_id, me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(pon_net_queue_update, ret);
			goto error;
		}
	}

error:
	dbg_out_ret("%d", ret);
	return ret;
}

/**
 * Calls a tree_builer to get a tree of qdiscs, and then
 * applies the tree of qdiscs
 */
enum pon_adapter_errno
pon_net_qdiscs_build_and_diff_apply(struct pon_net_qdiscs *qdiscs,
				    uint16_t class_id,
				    uint16_t related_me_id,
				    const struct pon_net_tree_builder *builder,
				    bool reassign_gems,
				    struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;
	struct pon_net_qdiscs *new_qdiscs;

	dbg_in_args("%p, %u, %p, %p", qdiscs, related_me_id, builder, ctx);

	new_qdiscs = pon_net_qdiscs_create();
	if (!new_qdiscs) {
		dbg_err_fn_ret(pon_net_qdiscs_create, 0);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto error;
	}

	/* Build qdisc tree using a new builder, if any */
	if (builder) {
		ret = builder->build(&ctx->me_list, related_me_id, new_qdiscs);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(setup, ret);
			goto error;
		}
	}

	if (reassign_gems) {
		ret = qdiscs_diff_apply_with_reassigning_gems(new_qdiscs,
							      qdiscs, class_id,
							      related_me_id,
							      ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(qdiscs_diff_apply_with_reassigning_gems,
				       ret);
			goto error;
		}
	} else {
		ret = qdiscs_diff_apply(new_qdiscs, qdiscs, class_id,
					related_me_id, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(qdiscs_diff_apply, ret);
			goto error;
		}
	}

	pon_net_qdiscs_swap(qdiscs, new_qdiscs);
error:
	if (new_qdiscs)
		pon_net_qdiscs_destroy(new_qdiscs);

	dbg_out_ret("%d", ret);
	return ret;

}
