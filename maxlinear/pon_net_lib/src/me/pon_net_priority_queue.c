/*****************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <pon_adapter.h>

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "../pon_net_uni.h"
#include "pon_net_gem_port_network_ctp.h"
#include "pon_net_priority_queue.h"
#include "pon_net_onu2_g.h"
#include "pon_net_tcont.h"

#include <omci/me/pon_adapter_priority_queue.h>
#include <omci/me/pon_adapter_traffic_scheduler.h>

static const uint16_t uni_class_ids[] = {
	PON_CLASS_ID_PPTP_ETHERNET_UNI,
	PON_CLASS_ID_VEIP,
};

/**
 * There are 3 concepts used to manage traffic priority:
 *
 * - Priority Queue's priority
 * - Traffic Class
 * - Queue number
 *
 * Priority Queue's priority are the two least significant bits of Related
 * Port attribute. The value 0 is the highest priority and 0x0FFF is the
 * lowest priority.
 *
 * Traffic class represents the type of the traffic and by *default* is
 * mapped in a way specified by table 'tc_to_queue'. OLT can override this
 * mapping by mapping GEM ports to the queues directly and mapping certain
 * traffic classes to concrete GEM ports using 802.1p Mapper Service Profile.
 *
 * The queue number corresponds to the actual queue number in hardware.
 * The queue 1 is the highest priority queue and 8 is the lowest.
 *
 * The below tables connect these 3 concepts together.
 */
static const uint16_t priority_to_tc[] = {
	[0] = 7, /* highest priority */
	[1] = 6,
	[2] = 5,
	[3] = 4,
	[4] = 3,
	[5] = 2,
	[6] = 1,
	[7] = 0, /* lowest priority */
};

/*
 * Please keep in mind that values of these array should be
 * from 1 to MAX_QUEUES
 */
static const uint16_t tc_to_queue[] = {
	[7] = 1, /* highest priority */
	[6] = 2,
	[5] = 3,
	[4] = 4,
	[3] = 5,
	[2] = 6,
	[1] = 7,
	[0] = 8, /* lowest priority */
};

static uint16_t pon_net_map_priority_to_tc(uint16_t priority)
{
	return priority_to_tc[priority % ARRAY_SIZE(priority_to_tc)];
}

uint16_t pon_net_map_priority_to_queue(uint16_t priority)
{
	uint16_t tc = pon_net_map_priority_to_tc(priority);

	return tc_to_queue[tc];
}

uint16_t pon_net_map_tc_to_queue(uint16_t tc)
{
	return tc_to_queue[tc % ARRAY_SIZE(tc_to_queue)];
}

/* If the most significant bit of Priority Queue ME ID is
   1, then it means that it represents an upstream queue */
static int is_upstream(uint16_t me_id)
{
	return !!(me_id & 0x8000);
}

static enum pon_adapter_errno
priority_queue_scale_get(void *ll_handle, uint32_t *scale)
{
	enum pon_adapter_errno ret = PON_ADAPTER_ERR_NOT_SUPPORTED;

	dbg_in_args("%p, %p", ll_handle, scale);

	if (!scale) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}
	*scale = 0;

	dbg_out_ret("%d", ret);
	return ret;
}

/* We need this to avoid infinite loop, which would hang higher layer
 * process. */
#define HEIGHT_OF_POSSIBLE_CIRCULAR_REF 1000

#define CHAIN_READ_ERROR	((unsigned int)-1)
static unsigned int chain_read(struct pon_net_me_list *me_list,
			       uint16_t traffic_scheduler_me_id,
			       struct pon_net_traffic_scheduler *chain,
			       unsigned int chain_capacity)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	unsigned int i = 0;
	struct pon_net_traffic_scheduler ts = {0};

	dbg_in_args("%p, %u", me_list, traffic_scheduler_me_id);

	/* Read a chain of traffic scheduler connected to this PQ */
	while (traffic_scheduler_me_id != 0 &&
	       i < HEIGHT_OF_POSSIBLE_CIRCULAR_REF) {
		ret = pon_net_me_list_read(me_list,
					   PON_CLASS_ID_TRAFFIC_SCHEDULER,
					   traffic_scheduler_me_id,
					   &ts,
					   sizeof(ts));
		if (ret != PON_ADAPTER_SUCCESS) {
			/* The traffic_scheduler_me_id is invalid, which
			 * means the tree of Traffic Schedulers is incorrectly
			 * configured - we can't help it. */
			dbg_err_fn_ret(pon_net_me_list_read, ret);
			dbg_out_ret("%d", -1);
			return CHAIN_READ_ERROR;
		}

		traffic_scheduler_me_id = ts.traffic_scheduler_ptr;

		/* We can read up to chain_capacity, but we can count more
		 * traffic schedulers */
		if (chain && i < chain_capacity)
			chain[i] = ts;

		i++;
	}

	if (i >= HEIGHT_OF_POSSIBLE_CIRCULAR_REF) {
		dbg_err("Possible circular reference in Traffic Scheduler ME.\n");
		dbg_out_ret("%d", -1);
		return CHAIN_READ_ERROR;
	}

	dbg_out_ret("%u", i);
	return i;
}

/* We do not fully support hierarchical scheduling, we only
 * support simple "flat" single level hierarchy of Traffic Schedulers.
 *
 * If non-flat multiple level hierarchy is assigned to PQ the following will
 * happen:
 *
 * PQ -- TS ----\
 *              |
 * PQ -- TS ----------------\
 *                          |
 * PQ -- TS ----------\     |
 *                    |     |
 * PQ -- TS -- TS --------- TS --- T-CONT
 *       ^     ^            ^ tcont is taken from the scheduler directly
 *       |     |              connected to it
 *       |     weight/prio is completely ignored here
 *       weight/prio is taken only from schedulers directly connected to PQ
 *
 * For simple flat hierarchy this approach works fine:
 *
 * PQ -- TS ----\
 *              |
 * PQ -- TS ----|
 *              |
 * PQ -- TS ----|
 *              |
 * PQ -- TS --- T-CONT
 *       ^ tcont is taken from the scheduler directly connected to it.
 *       |
 *       weight/prio is taken from scheduler directly connected to PQ
 *
 * The above approach is taken when policy is set to
 * PA_TRAFFICSCHEDULERG_POLICY_SP or PA_TRAFFICSCHEDULERG_POLICY_WRR.
 *
 * If PA_TRAFFICSCHEDULERG_POLICY_NULL is set as a policy on a first traffic
 * scheduler, that is connected to Priority Queue, then we will use the
 * weight and priority from the Priority Queue ME itself. */
static enum pon_adapter_errno
use_scheduler_properties_if_any(struct pon_net_context *ctx,
				uint16_t me_id,
				struct pon_net_priority_queue *pq)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_traffic_scheduler ts = {0};
	unsigned int chain_length = 0;

	dbg_in_args("%p, %u, %p", ctx, me_id, pq);

	if (!is_upstream(me_id)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (pq->traffic_scheduler_me_id == 0) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	chain_length = chain_read(&ctx->me_list,
				  pq->traffic_scheduler_me_id,
				  pq->chain, ARRAY_SIZE(pq->chain));
	if (chain_length == CHAIN_READ_ERROR ||
	    chain_length > TS_HIERARCHY_MAX_HEIGHT) {
		FN_ERR_RET(chain_length, chain_read,
			   PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}
	pq->chain_length = chain_length;
	pq->associated_me_id = pq->chain[pq->chain_length - 1].tcont_ptr;

	ret = pon_net_me_list_read(&ctx->me_list,
				   PON_CLASS_ID_TRAFFIC_SCHEDULER,
				   pq->traffic_scheduler_me_id,
				   &ts,
				   sizeof(ts));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_read, ret);
		return ret;
	}

	switch (ts.policy) {
	case PA_TRAFFICSCHEDULERG_POLICY_WRR:
		pq->weight = ts.priority_weight;
		break;
	case PA_TRAFFICSCHEDULERG_POLICY_SP:
		pq->queue = pon_net_map_priority_to_queue(ts.priority_weight);
		pq->weight = 0;
		break;
	case PA_TRAFFICSCHEDULERG_POLICY_NULL:
	default:
		/* Use weight and priority from Priority Queue ME */
		break;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static void
workaround_wrong_weight_and_traffic_scheduler(struct pon_net_priority_queue *pq)
{
	dbg_in_args("%p", pq);

	/* If we allow Priority Queue to be connected to a Traffic Scheduler
	 * with no weight, and no other Priority Queue's are connected to that
	 * Traffic Scheduler, then the Traffic Scheduler will be configured
	 * as SP scheduler. It is not possible to connect SP scheduler to the
	 * SP scheduler on the T-CONT. Therefore we ignore Traffic Scheduler
	 * in such situation. */
	if (pq->traffic_scheduler_me_id && pq->weight == 0) {
		dbg_wrn("Traffic Scheduler can only be configured on Priority Queue with non-zero weight.\n");
		dbg_wrn("Ignoring the Traffic Scheduler attribute\n");
		pq->traffic_scheduler_me_id = 0;
	}

	dbg_out();
}

static int thresholds_changed(const struct pon_net_priority_queue *old,
			      const struct pon_net_priority_queue *new)
{
	return old->pkt_drop_q_thr_green_min != new->pkt_drop_q_thr_green_min ||
	       old->pkt_drop_q_thr_green_max != new->pkt_drop_q_thr_green_max ||
	       old->pkt_drop_q_thr_yellow_min !=
		   new->pkt_drop_q_thr_yellow_min ||
	       old->pkt_drop_q_thr_yellow_max != new->pkt_drop_q_thr_yellow_max;
}

static int probabilities_changed(const struct pon_net_priority_queue *old,
				 const struct pon_net_priority_queue *new)
{
	return old->pkt_drop_probability_green !=
		   new->pkt_drop_probability_green ||
	       old->pkt_drop_probability_yellow !=
		   new->pkt_drop_probability_yellow;
}

static enum pon_adapter_errno mode_update(struct pon_net_priority_queue *pq,
					  struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", pq, ctx);

	if (is_upstream(pq->me_id)) {
		ret = pon_net_tcont_qdiscs_mixed_mode_set(ctx,
							  &pq->associated_me_id,
							  1);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_tcont_qdiscs_mixed_mode_set,
				   ret);
			return ret;
		}
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno qdiscs_update(uint16_t me_id,
					    uint16_t *associated_me_ids,
					    unsigned int associated_me_ids_len,
					    struct pon_net_context *ctx)
{
	unsigned int i = 0;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%u, %p, %u, %p", me_id, associated_me_ids,
		    associated_me_ids_len, ctx);

	if (is_upstream(me_id)) {
		ret = pon_net_tcont_qdiscs_update(ctx, associated_me_ids,
						  associated_me_ids_len);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_tcont_qdiscs_update, ret);
			return ret;
		}

		dbg_out_ret("%d", ret);
		return ret;
	}

	for (i = 0; i < ARRAY_SIZE(uni_class_ids); ++i) {
		uint16_t class_id = uni_class_ids[i];

		ret = pon_net_uni_qdiscs_update(ctx, class_id,
						associated_me_ids,
						associated_me_ids_len);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_uni_qdiscs_update, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
handle_pq_change(struct pon_net_context *ctx,
		 struct pon_net_priority_queue *old,
		 struct pon_net_priority_queue *new)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint16_t associated_me_ids[2];
	unsigned int len = 0;
	bool update_queues = false;
	struct pon_net_me_list_item *item;
	uint16_t me_id;
	uint32_t i;

	if (old != NULL &&
	    new->associated_me_id != old->associated_me_id) {
		ret = mode_update(old, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, mode_update, ret);
			return ret;
		}

		associated_me_ids[len] = old->associated_me_id;
		len++;
	}

	if (old == NULL ||
	    new->weight != old->weight ||
	    thresholds_changed(old, new) ||
	    probabilities_changed(old, new) ||
	    new->associated_me_id != old->associated_me_id ||
	    new->traffic_scheduler_me_id != old->traffic_scheduler_me_id ||
	    new->drop_precedence_color_marking !=
	    old->drop_precedence_color_marking) {
		ret = mode_update(new, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, mode_update, ret);
			return ret;
		}

		associated_me_ids[len] = new->associated_me_id;
		len++;

		update_queues = true;
	}

	if (old == NULL ||
	    new->queue != old->queue ||
	    new->drop_precedence_color_marking !=
	    old->drop_precedence_color_marking)
		update_queues = true;

	if ((old == NULL ||
	     new->drop_precedence_color_marking !=
	     old->drop_precedence_color_marking) &&
	    is_upstream(new->me_id)) {
		/* update color_marking for all UNI (PPTP and VEIP) */
		for (i = 0; i < ARRAY_SIZE(uni_class_ids); ++i) {
			uint16_t class_id = uni_class_ids[i];

			pon_net_me_list_foreach_class(&ctx->me_list, class_id,
						      item) {
				me_id = item->me_id;
				ret = pon_net_color_marking_set(ctx, class_id,
								me_id,
					new->drop_precedence_color_marking);
				if (ret != PON_ADAPTER_SUCCESS) {
					FN_ERR_RET(ret,
						   pon_net_color_marking_set,
						   ret);
					return ret;
				}
			}
		}
	}

	ret = qdiscs_update(new->me_id, associated_me_ids, len, ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, qdiscs_update, ret);
		return ret;
	}

	if (update_queues) {
		ret = pon_net_gem_queue_update(ctx, NETLINK_FILTER_DIR_BOTH,
					       NULL, 0);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_gem_queue_update, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static void
priority_queue_from_update_data(struct pon_net_priority_queue *pq,
				uint16_t me_id,
				const struct pa_priority_queue_update_data *u)
{
	uint16_t associated_prio =
		(uint16_t)PA_PQ_RELATED_PRIO(u->related_port);
	uint16_t associated_me_id =
		(uint16_t)PA_PQ_RELATED_MEID(u->related_port);

	dbg_in_args("%p, %p", pq, u);

	pq->me_id = me_id;
	pq->queue = pon_net_map_priority_to_queue(associated_prio);
	pq->traffic_class = pon_net_map_priority_to_tc(associated_prio);
	pq->associated_me_id = associated_me_id;
	pq->weight = u->weight;
	pq->pkt_drop_q_thr_green_min = u->pkt_drop_q_thr_green_min;
	pq->pkt_drop_q_thr_green_max = u->pkt_drop_q_thr_green_max;
	pq->pkt_drop_q_thr_yellow_min = u->pkt_drop_q_thr_yellow_min;
	pq->pkt_drop_q_thr_yellow_max = u->pkt_drop_q_thr_yellow_max;
	pq->pkt_drop_probability_green = u->pkt_drop_probability_green;
	pq->pkt_drop_probability_yellow = u->pkt_drop_probability_yellow;
	pq->drop_precedence_color_marking = u->drop_precedence_color_marking;

	if (is_upstream(me_id))
		pq->traffic_scheduler_me_id = u->traffic_scheduler_ptr;

	dbg_out();
}

static void apply_scale_factor(struct pon_net_priority_queue *pq,
			       struct pon_net_context *ctx)
{
	uint16_t scale;

	dbg_in_args("%p, %p", pq, ctx);

	scale = pon_net_priority_queue_scale_factor_get(ctx);

	pq->pkt_drop_q_thr_green_min *= scale;
	pq->pkt_drop_q_thr_green_max *= scale;
	pq->pkt_drop_q_thr_yellow_min *= scale;
	pq->pkt_drop_q_thr_yellow_max *= scale;

	dbg_out();
}

static enum pon_adapter_errno
priority_queue_create(struct pon_net_context *ctx,
		      const uint16_t me_id,
		      struct pa_priority_queue_update_data *update_data)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_priority_queue priority_queue = { 0 };

	dbg_in_args("%p, %u, %p", ctx, me_id, update_data);

	priority_queue_from_update_data(&priority_queue, me_id, update_data);

	apply_scale_factor(&priority_queue, ctx);

	workaround_wrong_weight_and_traffic_scheduler(&priority_queue);

	ret = use_scheduler_properties_if_any(ctx, me_id, &priority_queue);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, use_scheduler_properties_if_any, ret);
		return ret;
	}

	ret = mapper_id_map(ctx->mapper[MAPPER_PRIORITYQUEUE_MEID_TO_IDX],
			    me_id,
			    &priority_queue.index);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, mapper_id_map, ret);
		return ret;
	}

	ret = pon_net_me_list_write(&ctx->me_list,
				    PON_CLASS_ID_PRIORITY_QUEUE,
				    me_id,
				    &priority_queue,
				    sizeof(priority_queue));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	ret = handle_pq_change(ctx, NULL, &priority_queue);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, change, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
priority_queue_create_check(void *ll_handle,
			    const uint16_t me_id,
			    struct pa_priority_queue_update_data *update_data)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, update_data);

	if (ctx->cfg.no_ds_prio_queues && !is_upstream(me_id)) {
		dbg_prn("%s: me_id 0x%x ignored\n", __func__, me_id);
		return PON_ADAPTER_SUCCESS;
	}

	ret = priority_queue_create(ctx, me_id, update_data);
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
priority_queue_destroy(void *ll_handle,
		       uint16_t me_id)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u", ll_handle, me_id);

	mapper_id_remove(ctx->mapper[MAPPER_PRIORITYQUEUE_MEID_TO_IDX], me_id);
	pon_net_me_list_remove(&ctx->me_list, PON_CLASS_ID_PRIORITY_QUEUE,
			       me_id);

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
priority_queue_update(struct pon_net_context *ctx,
		      const uint16_t me_id,
		      struct pa_priority_queue_update_data *update_data)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_priority_queue pq = {0};
	struct pon_net_priority_queue old_pq = {0};

	dbg_in_args("%p, %u, %p", ctx, me_id, update_data);

	ret = pon_net_me_list_read(&ctx->me_list,
				   PON_CLASS_ID_PRIORITY_QUEUE,
				   me_id,
				   &old_pq,
				   sizeof(old_pq));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_read, ret);
		return ret;
	}

	priority_queue_from_update_data(&pq, me_id, update_data);

	apply_scale_factor(&pq, ctx);

	pq.index = old_pq.index;

	workaround_wrong_weight_and_traffic_scheduler(&pq);

	ret = use_scheduler_properties_if_any(ctx, me_id, &pq);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, use_scheduler_properties_if_any, ret);
		return ret;
	}

	ret = pon_net_me_list_write(&ctx->me_list, PON_CLASS_ID_PRIORITY_QUEUE,
				    me_id, &pq, sizeof(pq));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	ret = handle_pq_change(ctx, &old_pq, &pq);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, change, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
priority_queue_update_check(void *ll_handle,
			    const uint16_t me_id,
			    struct pa_priority_queue_update_data *update_data)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, update_data);

	if (ctx->cfg.no_ds_prio_queues && !is_upstream(me_id)) {
		dbg_prn("%s: me_id 0x%x ignored\n", __func__, me_id);
		return PON_ADAPTER_SUCCESS;
	}

	ret = priority_queue_update(ctx, me_id, update_data);
	dbg_out_ret("%d", ret);
	return ret;
}

static uint16_t remove_scale_factor(struct pon_net_context *ctx,
				    uint32_t q_thr)
{
	uint16_t scale;

	scale = pon_net_priority_queue_scale_factor_get(ctx);
	return (uint16_t)(q_thr / scale);
}

static enum pon_adapter_errno
priority_queue_maximum_queue_size_get(void *ll_handle,
				      const uint16_t me_id,
				      uint16_t *data)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_priority_queue pq = {0};
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, data);

	if (!data) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}
	*data = 0xFFFF;

	if (ctx->cfg.no_ds_prio_queues && !is_upstream(me_id)) {
		/* In some systems, downstream priority queues are disabled at
		 * low level, then return default value.
		 */
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_me_list_read(&ctx->me_list,
				   PON_CLASS_ID_PRIORITY_QUEUE,
				   me_id,
				   &pq, sizeof(pq));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_read, ret);
		return ret;
	}
	*data = remove_scale_factor(ctx, pq.pkt_drop_q_thr_green_max);

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
priority_queue_alloc_queue_size_get(void *ll_handle,
				    const uint16_t me_id,
				    uint16_t *data)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_priority_queue pq = {0};
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, data);

	if (!data) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}
	*data = 0xFFFF;

	if (ctx->cfg.no_ds_prio_queues && !is_upstream(me_id)) {
		/* In some systems, downstream priority queues are disabled at
		 * low level, then return default value.
		 */
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_me_list_read(&ctx->me_list,
				   PON_CLASS_ID_PRIORITY_QUEUE,
				   me_id,
				   &pq, sizeof(pq));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_read, ret);
		return ret;
	}
	*data = remove_scale_factor(ctx, pq.pkt_drop_q_thr_green_max);

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
priority_queue_packet_drop_queue_threshold_get(void *ll_handle,
					       const uint16_t me_id,
					       uint8_t *data)
{
	enum pon_adapter_errno ret = PON_ADAPTER_ERR_NOT_SUPPORTED;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, data);

	if (!data) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}
	*data = 0;

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
priority_queue_drop_precedence_color_marking_get(void *ll_handle,
				const enum pa_queue_direction direction,
				const uint8_t lan_port,
				uint8_t *color_marking)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %d, %u, %p", ll_handle,
		    direction, lan_port, color_marking);

	if (!color_marking) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}
	*color_marking = 0;

	dbg_out_ret("%d", ret);
	return ret;
}

const struct pa_priority_queue_ops priority_queue_ops = {
	.scale_get = priority_queue_scale_get,
	.create = priority_queue_create_check,
	.destroy = priority_queue_destroy,
	.update = priority_queue_update_check,
	.maximum_queue_size_get = priority_queue_maximum_queue_size_get,
	.alloc_queue_size_get = priority_queue_alloc_queue_size_get,
	.packet_drop_queue_threshold_get =
		priority_queue_packet_drop_queue_threshold_get,
	.drop_precedence_color_marking_get =
		priority_queue_drop_precedence_color_marking_get
};
