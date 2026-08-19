/*****************************************************************************
 *
 * Copyright (c) 2020 - 2024 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <netlink/route/link.h>
#include <netlink/route/link/gem.h>

#include <pon_adapter.h>
#include <omci/me/pon_adapter_gem_port_network_ctp.h>
#include <omci/me/pon_adapter_mac_bridge_port_config_data.h>
#include <omci/me/pon_adapter_onu_g.h>
#include <omci/me/pon_adapter_priority_queue.h>
#include <omci/me/pon_adapter_traffic_scheduler.h>

#include "../pon_net_debug.h"
#include "../pon_net_netlink.h"
#include "../pon_net_qdisc.h"
#include "../pon_net_uni.h"
#include "../pon_net_dev_db.h"
#include "../pon_net_dev_defs.h"

#include "pon_net_traffic_descriptor.h"
#include "pon_net_tcont.h"
#include "pon_net_onu_g.h"
#include "pon_net_gem_port_network_ctp.h"

/** Check if two queue assignments are identical by comparing all parameters */
static bool assignment_same(const struct queue_assignment *a,
			    const struct queue_assignment *b)
{
	return a->us == b->us &&
	       a->me_id == b->me_id &&
	       a->queue_id == b->queue_id &&
	       a->traffic_scheduler_me_id == b->traffic_scheduler_me_id &&
	       a->priority_queue_index == b->priority_queue_index &&
	       a->traffic_class == b->traffic_class;
}

/* Checks if any parent of given qdisc is of type DRR */
static bool check_weight_drr(struct pon_net_qdiscs *tree,
			     struct pon_net_qdisc *qdisc)
{
	struct pon_net_qdisc *tmp_qdisc, *child_qdisc;

	bool parent_drr = false;
	bool weight = true;

	if (qdisc) {
		tmp_qdisc = qdisc;
		/* iterate until root or parent type drr found */
		do {
			child_qdisc = tmp_qdisc;
			tmp_qdisc = &tree->qdiscs[tmp_qdisc->parent_idx];
			if (tmp_qdisc->type == &pon_net_drr_type) {
				parent_drr = true;
				weight = !!child_qdisc->parent_arg;
			}
		} while (!(parent_drr) &&
			 !(tmp_qdisc->idx == tmp_qdisc->parent_idx));
	}
	return !parent_drr || weight;
}

/*
 * Return qdiscs handle to which GEM port will be assigned. This will be a
 * leaf qdisc from a qdisc 'tree'. The 'allowed_assignment' will be set to true
 * if it makes sense to assign a GEM to the queue.
 */
static uint16_t assignment_qdisc(struct pon_net_qdiscs *tree,
				 const struct queue_assignment *assignment,
				 bool *allowed_assignment)
{
	struct pon_net_qdisc *qdisc;
	uint16_t handle =
		PON_TBF_QDISC_HANDLE(assignment->priority_queue_index);

	*allowed_assignment = false;
	/* try to find the tc-tbf qdisc in the tree */
	qdisc = qdiscs_get_by_handle(tree, handle);
	if (qdisc) {
		*allowed_assignment = check_weight_drr(tree, qdisc);
	} else {
		/* if there is no tc-tbf leaf, then look for tc-red */
		handle = PON_WRED_QDISC_1(assignment->priority_queue_index);
		/* try to find the second tc-red (yellow) qdisc in the tree */
		qdisc = qdiscs_get_by_handle(tree, handle);
		if (qdisc)
			*allowed_assignment = check_weight_drr(tree, qdisc);
	}
	return handle;
}

/* If the most significant bit of Priority Queue ME ID is
   1, then it means that it represents an upstream queue */
static bool is_upstream(uint16_t me_id)
{
	return !!(me_id & 0x8000);
}

/* Initialize assignment from Priority Queue ME parameters */
static void assignment_from_pq(struct queue_assignment *a,
			       const struct pon_net_priority_queue *pq)
{
	struct queue_assignment new = {
		.queue_id = pq->queue,
		.me_id = pq->associated_me_id,
		.traffic_scheduler_me_id = pq->traffic_scheduler_me_id,
		.priority_queue_index = pq->index,
		.us = is_upstream(pq->me_id),
		.traffic_class = -1,
	};

	*a = new;
}

/*
 * Initialize 'queue_assignment', by looking up Priority Queue ME identified by
 * 'priority_queue_me_id' in ctx->me_list. Return 'true' if assignment could
 * be initialized, which means that a GEM can be connected to Priority Queue.
 * Return 'false' otherwise.
 */
static bool
assignment_read(struct pon_net_context *ctx, uint16_t priority_queue_me_id,
		struct queue_assignment *queue_assignment)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_priority_queue pq;

	dbg_in_args("%p, %u, %p", ctx, priority_queue_me_id, queue_assignment);

	ret = pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_PRIORITY_QUEUE,
				   priority_queue_me_id, &pq, sizeof(pq));
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", false);
		return false;
	}

	if (is_upstream(priority_queue_me_id)) {
		/* Assigning a GEM to a T-CONT that doesn't exists won't work */
		if (!pon_net_me_list_exists(&ctx->me_list, PON_CLASS_ID_TCONT,
					    pq.associated_me_id)) {
			dbg_out_ret("%d", false);
			return false;
		}
	}

	assignment_from_pq(queue_assignment, &pq);

	dbg_out_ret("%d", true);
	return true;
}

/* Returns true if two assignment lists are identical */
static bool queue_assignment_list_eq(const struct queue_assignment_list *a,
				     const struct queue_assignment_list *b)
{
	unsigned int i = 0;

	if (a->num_assignments != b->num_assignments)
		return false;

	for (i = 0; i < a->num_assignments; ++i)
		if (!assignment_same(&a->assignments[i], &b->assignments[i]))
			return false;

	return true;
}

/* Retrieve qdisc tree from data structure associated with T-CONT */
static struct pon_net_qdiscs *tcont_tree_get(void *data)
{
	return ((struct tcont *)data)->tree;
}

/* Retrieve qdisc tree from data structure associated with UNI port */
static struct pon_net_qdiscs *uni_tree_get(void *data)
{
	return ((struct uni *)data)->qdiscs;
}

/* Lookup table, describing Managed Entities for which have a qdisc tree */
static const struct {
	/* True if ME represents an upstream direction */
	bool us;
	/* ME Class ID */
	uint16_t class_id;
	/* Function to retrieve a qdisc tree from ME's private data structure */
	struct pon_net_qdiscs *(*tree_get)(void *data);
} classes_to_try[] = {
	{ true, PON_CLASS_ID_TCONT, tcont_tree_get },
	{ false, PON_CLASS_ID_VEIP, uni_tree_get },
	{ false, PON_CLASS_ID_PPTP_ETHERNET_UNI, uni_tree_get },
};

/* Read interface name on which queue assignment tc filters will be created */
static enum pon_adapter_errno
read_assignment_ifname(struct pon_net_context *ctx,
		       const struct queue_assignment *as, char *ifname,
		       unsigned int capacity)
{
	enum pon_adapter_errno ret;
	unsigned int i;
	uint16_t class_id;

	dbg_in_args("%p, %p, %p, %u", ctx, as, ifname, capacity);

	for (i = 0; i < ARRAY_SIZE(classes_to_try); ++i) {
		if (classes_to_try[i].us != as->us)
			continue;

		class_id = classes_to_try[i].class_id;

		if (class_id == PON_CLASS_ID_PPTP_ETHERNET_UNI &&
		    !as->us)
			ret = pon_net_ext_uni_ifname_get(ctx, class_id,
							 as->me_id, ifname,
							 capacity);
		else
			ret = pon_net_ifname_get(ctx, class_id, as->me_id,
						 ifname, capacity);
		if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
			continue;
		} else if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_ifname_get, ret);
			return ret;
		}
		/* We found it */
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
	return PON_ADAPTER_ERR_NOT_FOUND;
}

/* Get the qdiscs tree for given queue_assignment */
static struct pon_net_qdiscs *
read_assignment_tree(struct pon_net_context *ctx,
		     const struct queue_assignment *as)
{
	unsigned int i;
	struct pon_net_qdiscs *tree;
	void *data;
	uint16_t class_id;

	dbg_in_args("%p, %p", ctx, as);

	for (i = 0; i < ARRAY_SIZE(classes_to_try); ++i) {
		if (classes_to_try[i].us != as->us)
			continue;

		class_id = classes_to_try[i].class_id;

		data = pon_net_me_list_get_data(&ctx->me_list, class_id,
						as->me_id);
		if (!data)
			continue;

		/* We found it */
		tree = classes_to_try[i].tree_get(data);

		dbg_out_ret("%p", tree);
		return tree;

	}

	dbg_out_ret("%p", NULL);
	return NULL;
}

/** Compare two queue index values and check if they are equal. */
static int by_pq_index(const void *a, const void *b)
{
	const struct queue_assignment *assignment_a = a;
	const struct queue_assignment *assignment_b = b;

	if (assignment_a->priority_queue_index <
	    assignment_b->priority_queue_index)
		return -1;
	if (assignment_a->priority_queue_index >
	    assignment_b->priority_queue_index)
		return -1;

	return 0;
}

/*
 * Sort assignments within a struct queue_assignment_list according to
 * priority_queue_index
 */
static void sort_by_pq_index(struct queue_assignment_list *assignments)
{
	qsort(assignments->assignments, assignments->num_assignments,
	      sizeof(assignments->assignments[0]), by_pq_index);
}

/*
 * Read 'assignments' collection, by looking up all Priority Queue MEs who
 * are related to the T-CONT specified by 'tcont_me_id'.
 * If no assignments can be read for requested T-CONT, then 'assignments'
 * will contain zero elements.
 */
static void read_tcont_assignments(struct pon_net_context *ctx,
				   uint16_t tcont_me_id,
				   struct queue_assignment_list *assignments)
{
	struct pon_net_priority_queue *pq;
	struct pon_net_me_list_item *item;
	static const struct queue_assignment_list zero = { 0 };

	dbg_in_args("%p, %u, %p", ctx, tcont_me_id, assignments);

	*assignments = zero;

	pon_net_me_list_foreach_class(&ctx->me_list,
				      PON_CLASS_ID_PRIORITY_QUEUE, item) {
		struct queue_assignment *a;

		pq = item->data;
		if (pq->associated_me_id != tcont_me_id)
			continue;

		if (assignments->num_assignments >=
		    ARRAY_SIZE(assignments->assignments))
			break;

		a = &assignments->assignments[assignments->num_assignments++];

		assignment_from_pq(a, pq);

		a->traffic_class = (int)pq->traffic_class;
	}

	sort_by_pq_index(assignments);

	dbg_out();
}

/*
 * Read 'assignments' collection, by looking up Priority Queue ME identified by
 * 'pq_me_id' in ctx->me_list. If no assignment can be found for requested
 * Priority Queue, then 'assignments' will contain zero elements.
 */
static void read_pq_assignments(struct pon_net_context *ctx,
				uint16_t pq_me_id,
				struct queue_assignment_list *assignments)
{
	struct queue_assignment assignment = {0};

	dbg_in_args("%p, %u, %p", ctx, pq_me_id, assignments);

	if (!assignment_read(ctx, pq_me_id, &assignment)) {
		assignments->num_assignments = 0;
		dbg_out();
		return;
	}

	assignments->num_assignments = 1;
	assignments->assignments[0] = assignment;

	dbg_out();
}

/*
 * Read 'assignments' for 'tcont_me_id' or 'pq_me_id', depending on which
 * traffic management option was set for ONU-G
 */
static void read_assignments(struct pon_net_context *ctx, uint16_t tcont_me_id,
			     uint16_t pq_me_id,
			     struct queue_assignment_list *assignments)
{
	uint8_t option;
	struct queue_assignment_list read = { 0 };

	dbg_in_args("%p, %u, %u, %p", ctx, tcont_me_id, pq_me_id, assignments);

	option = pon_net_traffic_management_option_get(ctx);
	switch (option) {
	case PA_TMO_RATE:
		read_tcont_assignments(ctx, tcont_me_id, &read);
		break;
	case PA_TMO_PRIO_RATE:
	case PA_TMO_PRIO:
		read_pq_assignments(ctx, pq_me_id, &read);
		break;
	default:
		break;
	}

	*assignments = read;

	dbg_out();
}

/*
 * Retrieve a color marking value for the Priority Queue
 * identified by 'priority_queue_me_id'.
 */
static uint8_t color_marking_read(struct pon_net_context *ctx,
				  uint16_t priority_queue_me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_priority_queue pq;

	dbg_in_args("%p, %u", ctx, priority_queue_me_id);

	ret = pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_PRIORITY_QUEUE,
				   priority_queue_me_id, &pq, sizeof(pq));
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", 0);
		return 0;
	}

	dbg_out_ret("%d", pq.drop_precedence_color_marking);
	return pq.drop_precedence_color_marking;
}

enum pon_adapter_errno
pon_net_gem_port_net_ctp_pmapper_assign(struct pon_net_context *ctx,
					uint16_t *gem_ctp_me_ids,
					bool status)
{
	struct pon_net_me_list_item *item;
	struct pon_net_gem_port_net_ctp *gem;
	uint16_t class_id = PON_CLASS_ID_GEM_PORT_NET_CTP_DATA;
	uint16_t me_id;
	unsigned int i = 0;

	dbg_in_args("%p, %p, %d", ctx, gem_ctp_me_ids, status);

	if (!gem_ctp_me_ids) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	for (i = 0; i < PON_8021P_PCP_COUNT; i++) {
		/* 0xFFFF is a NULL pointer, as specified in OMCI */
		if (gem_ctp_me_ids[i] == 0xFFFF)
			continue;

		pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
			gem = item->data;
			me_id = item->me_id;

			if (gem_ctp_me_ids[i] == me_id) {
				if (status)
					++gem->has_pmapper;
				else
					--gem->has_pmapper;
			}
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Assigns GEM port to a T-CONT queue, by adding a tc-filter to
 * ingress of clsact qdisc.
 */
static enum pon_adapter_errno
add_queue_filter(struct pon_net_context *ctx, uint16_t gem_ctp_me_id,
		 struct queue_assignment *assignment)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE] = "";
	char indev[IF_NAMESIZE] = "";
	uint16_t qdisc_handle = 0;
	struct pon_net_gem_port_net_ctp gem = {0};
	struct pon_net_qdiscs *tree = NULL;
	int written;
	bool assignment_allowed = false;

	dbg_in_args("%p, %u, %p", ctx, gem_ctp_me_id, assignment);

	ret = read_assignment_ifname(ctx, assignment, ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, read_assignment_ifname, ret);
		return ret;
	}

	written = snprintf(indev, sizeof(indev), PON_IFNAME_GEM, gem_ctp_me_id);
	if (written > (int)sizeof(indev) - 1) {
		FN_ERR_RET(written, snprintf, PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	ret = pon_net_me_list_read(&ctx->me_list,
				   PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
				   gem_ctp_me_id, &gem, sizeof(gem));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_read, ret);
		return ret;
	}

	tree = read_assignment_tree(ctx, assignment);
	if (!tree) {
		FN_ERR_RET(0, read_assignment_tree, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	qdisc_handle = assignment_qdisc(tree, assignment, &assignment_allowed);
	if (assignment_allowed) {
		uint32_t handle;
		uint32_t prio;

		ret = pon_net_dev_db_gen(ctx->db, ifname,
					 PON_NET_PRIO_GEM_TO_QUEUE_ASSIGNMENT,
					 &prio, 1);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
			return ret;
		}

		ret = pon_net_dev_db_gen(
		    ctx->db, ifname, PON_NET_HANDLE_FILTER_INGRESS, &handle, 1);
		if (ret != PON_ADAPTER_SUCCESS) {
			enum pon_adapter_errno ret2;

			ret2 = pon_net_dev_db_put(
			    ctx->db, ifname,
			    PON_NET_PRIO_GEM_TO_QUEUE_ASSIGNMENT, &prio, 1);
			if (ret2 != PON_ADAPTER_SUCCESS)
				dbg_err_fn_ret(pon_net_dev_db_put, ret2);

			FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
			return ret;
		}

		ret = netlink_queue_assign(ctx->netlink, ifname, qdisc_handle,
					   assignment->queue_id,
					   indev, handle,
					   NETLINK_FILTER_UNUSED,
					   assignment->traffic_class,
					   (uint16_t)prio);
		if (ret != PON_ADAPTER_SUCCESS) {
			enum pon_adapter_errno ret2;

			ret2 = pon_net_dev_db_put(ctx->db, ifname,
					   PON_NET_HANDLE_FILTER_INGRESS,
					   &handle, 1);
			if (ret2 != PON_ADAPTER_SUCCESS)
				dbg_err_fn_ret(pon_net_dev_db_put, ret2);

			ret2 = pon_net_dev_db_put(
			    ctx->db, ifname,
			    PON_NET_PRIO_GEM_TO_QUEUE_ASSIGNMENT, &prio, 1);
			if (ret2 != PON_ADAPTER_SUCCESS)
				dbg_err_fn_ret(pon_net_dev_db_put, ret2);

			FN_ERR_RET(ret, netlink_queue_assign, ret);
			return ret;
		}

		assignment->tc_filter.prio = (uint16_t)prio;
		assignment->tc_filter.proto = ETH_P_ALL;
		assignment->tc_filter.handle = handle;
		assignment->tc_filter.dir = NETLINK_FILTER_DIR_INGRESS;
		assignment->tc_filter.handle_pool_id =
		    PON_NET_HANDLE_FILTER_INGRESS;
		assignment->tc_filter.prio_pool_id =
		    PON_NET_PRIO_GEM_TO_QUEUE_ASSIGNMENT;
	}


	dbg_out_ret("%d", ret);
	return ret;
}

/*
 * Unassigns GEM port from T-CONT queue, by removing a tc-filter from
 * ingress of clsact qdisc.
 */
static enum pon_adapter_errno
remove_queue_filter(struct pon_net_context *ctx, uint16_t gem_ctp_me_id,
		    struct queue_assignment *assignment)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE] = "";
	int interface_exists;

	dbg_in_args("%p, %u, %p", ctx, gem_ctp_me_id, assignment);

	ret = read_assignment_ifname(ctx, assignment, ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, read_assignment_ifname, ret);
		return ret;
	}

	interface_exists = netlink_netdevice_ifindex_get(ctx->netlink, ifname);
	if (!interface_exists) {
		/*
		 * Sometimes at this point the network device may already be
		 * removed. This can happen, if for example when T-CONT or
		 * PPTP Ethernet UNI is deleted before GEM Port Network CTP.
		 * In this case it is fine to return success.
		 */
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (assignment->tc_filter.handle_pool_id) {
		uint32_t handle = assignment->tc_filter.handle;

		ret = pon_net_dev_db_put(ctx->db, ifname,
					 assignment->tc_filter.handle_pool_id,
					 &handle, 1);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret);
		assignment->tc_filter.handle_pool_id = 0;
	}

	if (assignment->tc_filter.prio_pool_id) {
		uint32_t prio = assignment->tc_filter.prio;

		ret = pon_net_dev_db_put(ctx->db, ifname,
					 assignment->tc_filter.prio_pool_id,
					 &prio, 1);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret);
		assignment->tc_filter.prio_pool_id = 0;
	}

	/*
	* It is possible to have assignment without tc filter. There is
	* nothing to be removed if handle is 0.
	*/
	if (assignment->tc_filter.handle == 0)
		return PON_ADAPTER_SUCCESS;

	ret = netlink_filter_clear_one(ctx->netlink, ifname,
				       assignment->tc_filter.proto,
				       assignment->tc_filter.prio,
				       assignment->tc_filter.handle,
				       assignment->tc_filter.dir);
	/*
	 * Change return to success if filter couldn't be removed because it
	 * does not exist anymore.
	 */
	if (ret == PON_ADAPTER_ERROR) {
		ret = PON_ADAPTER_SUCCESS;
	} else if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_clear_one, ret);
		return ret;
	}

	memset(&(assignment->tc_filter), 0, sizeof(assignment->tc_filter));

	dbg_out_ret("%d", ret);
	return ret;
}

/* Adds many tc filters */
static enum pon_adapter_errno
add_queue_filters(struct pon_net_context *ctx, uint16_t gem_ctp_me_id,
		  struct queue_assignment_list *assignments)
{
	unsigned int i;
	enum pon_adapter_errno ret;
	struct queue_assignment *as;

	dbg_in_args("%p, %u, %p", ctx, gem_ctp_me_id, assignments);

	for (i = 0; i < assignments->num_assignments; ++i) {
		as = &assignments->assignments[i];

		ret = add_queue_filter(ctx, gem_ctp_me_id, as);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(add_queue_filter, ret);
			goto rollback;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

rollback:
	while (i) {
		enum pon_adapter_errno err;

		i--;

		as = &assignments->assignments[i];

		err = remove_queue_filter(ctx, gem_ctp_me_id, as);
		if (err != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(remove_queue_filter, err);
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* Removes many tc filters */
static enum pon_adapter_errno
remove_queue_filters(struct pon_net_context *ctx, uint16_t gem_ctp_me_id,
		     struct queue_assignment_list *assignments)
{
	unsigned int i;
	enum pon_adapter_errno ret;
	struct queue_assignment *as;

	dbg_in_args("%p, %u, %p", ctx, gem_ctp_me_id, assignments);

	for (i = 0; i < assignments->num_assignments; ++i) {
		as = &assignments->assignments[i];

		ret = remove_queue_filter(ctx, gem_ctp_me_id, as);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(remove_queue_filter, ret);
			goto rollback;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

rollback:
	while (i) {
		enum pon_adapter_errno err;

		i--;

		as = &assignments->assignments[i];

		err = add_queue_filter(ctx, gem_ctp_me_id, as);
		if (err != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(add_queue_filter, err);
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/*
 * Changes tc-filter, removes 'old' assignment and adds filters 'new'
 * to reflect new assignment
 */
static enum pon_adapter_errno
change_queue_filters(struct pon_net_context *ctx, uint16_t me_id,
		     struct queue_assignment_list *old,
		     struct queue_assignment_list *new)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %p, %p", ctx, me_id, old, new);

	if (queue_assignment_list_eq(old, new)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	/* Remove tc filters for old queue assignment */
	ret = remove_queue_filters(ctx, me_id, old);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, remove_queue_filters, ret);
		return ret;
	}

	/* Add tc filters for new queue assignment */
	ret = add_queue_filters(ctx, me_id, new);
	if (ret != PON_ADAPTER_SUCCESS) {
		/* Restore old */
		enum pon_adapter_errno err;

		dbg_err_fn_ret(add_queue_filters, ret);

		err = add_queue_filters(ctx, me_id, old);
		if (err != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(add_queue_filters, err);

		dbg_out_ret("%d", ret);
		return ret;
	}
	*old = *new;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Change color marking value from 'old' to 'new' for GEM Port Network CTP
 * identified by 'me_id'
 */
static enum pon_adapter_errno
tc_ds_queue_color_marking_change(struct pon_net_context *ctx, uint16_t me_id,
				 uint8_t old, uint8_t new)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %u, %u", ctx, me_id, old, new);

	if (old == new) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_color_marking_set(ctx, PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
					me_id, new);
	if (ret != PON_ADAPTER_SUCCESS) {
		/* Restore old */
		enum pon_adapter_errno err;

		dbg_err_fn_ret(pon_net_color_marking_set, ret);

		err = pon_net_color_marking_set(ctx,
					     PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
					     me_id, old);
		if (err != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_color_marking_set, err);

		dbg_out_ret("%d", ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Add ME 'id' to the 'array', but only if it is not present in the 'array'
 * already
 */
static void me_id_array_add_unique(uint16_t *array, unsigned int *len,
				   unsigned int capacity, uint16_t id)
{
	unsigned int i = 0;

	dbg_in_args("%p, %p, %u, %u", array, len, capacity, id);

	for (i = 0; i < *len; ++i) {
		if (id == array[i]) {
			dbg_out();
			return;
		}
	}

	if (*len < capacity)
		array[*len] = id;

	*len += 1;

	dbg_out();
}

static int direction_matches(uint8_t dir1, uint8_t dir2)
{
	switch (dir1) {
	case PA_GEMPORTNETCTP_DIR_DOWNSTREAM:
		return dir2 == PA_GEMPORTNETCTP_DIR_DOWNSTREAM ||
			dir2 == PA_GEMPORTNETCTP_DIR_BOTH;
	case PA_GEMPORTNETCTP_DIR_UPSTREAM:
		return dir2 == PA_GEMPORTNETCTP_DIR_UPSTREAM ||
			dir2 == PA_GEMPORTNETCTP_DIR_BOTH;
	case PA_GEMPORTNETCTP_DIR_BOTH:
		return 1;
	}

	return 0;
}

static int direction_us(uint8_t dir)
{
	return direction_matches(dir, PA_GEMPORTNETCTP_DIR_UPSTREAM);
}

static int direction_ds(uint8_t dir)
{
	return direction_matches(dir, PA_GEMPORTNETCTP_DIR_DOWNSTREAM);
}

static int direction_to_netlink_direction(uint8_t dir)
{
	switch (dir) {
	case PA_GEMPORTNETCTP_DIR_DOWNSTREAM:
		return NETLINK_GEM_DIR_DS;
	case PA_GEMPORTNETCTP_DIR_UPSTREAM:
		return NETLINK_GEM_DIR_US;
	case PA_GEMPORTNETCTP_DIR_BOTH:
		return NETLINK_GEM_DIR_BI;
	default:
		return NETLINK_GEM_UNUSED;
	}
}

static int enc_to_netlink_enc(uint8_t enc)
{
	switch (enc) {
	case PA_GEMPORTNETCTP_ENC_NO:
		return NETLINK_GEM_ENC_DIS;
	case PA_GEMPORTNETCTP_ENC_UC:
		return NETLINK_GEM_ENC_UC;
	case PA_GEMPORTNETCTP_ENC_BC:
		return NETLINK_GEM_ENC_BC;
	case PA_GEMPORTNETCTP_ENC_UCDS:
		return NETLINK_GEM_ENC_DSEN;
	default:
		return NETLINK_GEM_UNUSED;
	}
}

enum pon_adapter_errno
pon_net_gem_queue_unassign(struct pon_net_context *ctx,
			   enum netlink_filter_dir dir,
			   const uint16_t *gem_ctp_me_ids,
			   size_t capacity)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_me_list_item *item;
	struct pon_net_gem_port_net_ctp *gem;
	uint16_t class_id = PON_CLASS_ID_GEM_PORT_NET_CTP_DATA;

	dbg_in_args("%p, %d, %p, %zu", ctx, dir, gem_ctp_me_ids, capacity);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		gem = item->data;
		if (gem_ctp_me_ids &&
		    !pon_net_me_id_in_array(item->me_id, gem_ctp_me_ids,
					    capacity))
			continue;

		/*
		 * We should only unassign GEM ports whose direction matches the
		 * direction specified by the caller, to avoid unnecessary
		 * unassignment
		 */
		if (!direction_matches(gem->dir, dir))
			continue;

		if (direction_us(dir)) {
			remove_queue_filters(ctx, item->me_id, &gem->us);
			gem->us.num_assignments = 0;
		}

		if (direction_ds(dir)) {
			remove_queue_filters(ctx, item->me_id, &gem->ds);
			gem->ds.num_assignments = 0;
		}
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/*
 * Update color marking values for GEM Port Network CTP's whose IDs are provided
 * in 'gem_ctp_me_ids' array.
 */
static enum pon_adapter_errno
color_marking_update(struct pon_net_context *ctx,
		     const uint16_t *gem_ctp_me_ids, size_t capacity)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_me_list_item *item;
	uint16_t me_id;
	struct pon_net_gem_port_net_ctp *gem;
	uint16_t class_id = PON_CLASS_ID_GEM_PORT_NET_CTP_DATA;
	uint8_t new;

	dbg_in_args("%p, %p, %zu", ctx, gem_ctp_me_ids, capacity);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		gem = item->data;
		me_id = item->me_id;
		if (gem_ctp_me_ids &&
		    !pon_net_me_id_in_array(item->me_id, gem_ctp_me_ids,
					    capacity))
			continue;

		if (direction_ds(gem->dir)) {
			new = color_marking_read(ctx,
						 gem->ds_priority_queue_me_id);

			ret =
			    tc_ds_queue_color_marking_change(ctx, me_id,
							     gem->color_marking,
							     new);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret,
					   tc_ds_queue_color_marking_change,
					   ret);
				return ret;
			}
			gem->color_marking = new;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pon_net_gem_queue_update(struct pon_net_context *ctx,
						enum netlink_filter_dir dir,
						const uint16_t *gem_ctp_me_ids,
						size_t capacity)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_me_list_item *item;
	uint16_t me_id;
	struct pon_net_gem_port_net_ctp *gem;
	struct queue_assignment_list as;
	uint16_t class_id = PON_CLASS_ID_GEM_PORT_NET_CTP_DATA;

	dbg_in_args("%p, %d, %p, %zu", ctx, dir, gem_ctp_me_ids, capacity);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		gem = item->data;
		me_id = item->me_id;
		if (gem_ctp_me_ids &&
		    !pon_net_me_id_in_array(item->me_id, gem_ctp_me_ids,
					    capacity))
			continue;

		/* We should only update GEM ports whose direction matches the
		 * direction specified by the caller, to avoid unnecessary
		 * unassignment */
		if (!direction_matches(gem->dir, dir))
			continue;

		if (direction_ds(dir))
			read_pq_assignments(ctx, gem->ds_priority_queue_me_id,
					    &as);
		else
			as.num_assignments = 0;

		ret = change_queue_filters(ctx, me_id, &gem->ds, &as);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, change_queue_filters, ret);
			return ret;
		}

		if (direction_us(dir))
			read_assignments(ctx, gem->tcont_me_id,
					 gem->us_priority_queue_me_id, &as);
		else
			as.num_assignments = 0;

		ret = change_queue_filters(ctx, me_id, &gem->us, &as);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, change_queue_filters, ret);
			return ret;
		}
	}

	ret = color_marking_update(ctx, gem_ctp_me_ids, capacity);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, color_marking_update, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static int
max_gem_payload_size_to_netlink_max_size(uint16_t max_gem_payload_size)
{
	if (!max_gem_payload_size)
		return PON_NET_GEM_PORT_MAX_FRAME_SIZE;

	if (max_gem_payload_size < PON_NET_GEM_PORT_MIN_FRAME_SIZE)
		return PON_NET_GEM_PORT_MIN_FRAME_SIZE;

	return max_gem_payload_size;
}

enum pon_adapter_errno
pon_net_gem_max_payload_size_set(struct pon_net_context *ctx,
				 uint16_t gem_port_me_id,
				 uint16_t max_gem_payload_size)
{
	struct netlink_gem gem;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %u", ctx, gem_port_me_id, max_gem_payload_size);

	netlink_gem_defaults(&gem);

	gem.max_size =
		max_gem_payload_size_to_netlink_max_size(max_gem_payload_size);
	ret = netlink_gem_update(ctx->netlink, gem_port_me_id, &gem);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_gem_update, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

unsigned int
pon_net_gem_tcont_me_ids_get(struct pon_net_context *ctx,
			     const uint16_t *gem_ctp_me_ids,
			     unsigned int gem_ctp_me_ids_len,
			     uint16_t *tcont_me_id,
			     unsigned int tcont_me_ids_capacity)
{
	struct pon_net_me_list_item *item;
	struct pon_net_gem_port_net_ctp *gem;
	uint16_t class_id = PON_CLASS_ID_GEM_PORT_NET_CTP_DATA;
	unsigned int tcont_me_ids_len = 0;

	dbg_in_args("%p, %p, %u, %p, %u", ctx, gem_ctp_me_ids,
		    gem_ctp_me_ids_len, tcont_me_id, tcont_me_ids_capacity);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		gem = item->data;

		if (gem_ctp_me_ids &&
		    !pon_net_me_id_in_array(item->me_id, gem_ctp_me_ids,
					    gem_ctp_me_ids_len))
			continue;

		me_id_array_add_unique(tcont_me_id, &tcont_me_ids_len,
				       tcont_me_ids_capacity, gem->tcont_me_id);
	};

	dbg_out_ret("%d", tcont_me_ids_len);
	return tcont_me_ids_len;
}

/*
 * Return true if GEM Port Netowork CTP is associated with ME
 * identified by 'class_id' and 'me_id'.
 */
static bool is_related_to_me(struct pon_net_context *ctx,
			     const struct pon_net_gem_port_net_ctp *gem,
			     uint16_t class_id,
			     uint16_t me_id)
{
	unsigned int i;
	struct queue_assignment_list as = {0};

	switch (class_id) {
	case PON_CLASS_ID_TCONT:
		return gem->tcont_me_id == me_id;
	case PON_CLASS_ID_VEIP:
	case PON_CLASS_ID_PPTP_ETHERNET_UNI:
		read_pq_assignments(ctx, gem->ds_priority_queue_me_id, &as);
		for (i = 0; i < as.num_assignments; ++i)
			if (as.assignments[i].me_id == me_id)
				return true;
		return false;
	default:
		return false;
	}

	return false;
}

size_t pon_net_gem_me_ids_for_class_get(struct pon_net_context *ctx,
					uint16_t class_id,
					uint16_t me_id,
					uint16_t *gem_ctp_me_ids,
					unsigned int capacity)
{
	struct pon_net_me_list_item *item;
	size_t i = 0;

	dbg_in_args("%p, %u, %p, %u", ctx, me_id, gem_ctp_me_ids, capacity);

	for (i = 0; i < capacity; ++i)
		gem_ctp_me_ids[i] = 0xFFFF;

	i = 0;
	pon_net_me_list_foreach_class(&ctx->me_list,
				      PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
				      item) {
		struct pon_net_gem_port_net_ctp *gem_ctp;

		gem_ctp = item->data;
		if (!is_related_to_me(ctx, gem_ctp, class_id, me_id))
			continue;

		if (gem_ctp_me_ids && i < capacity)
			gem_ctp_me_ids[i] = item->me_id;
		i++;
	}

	dbg_out_ret("%zu", i);
	return i;
}

/* Assigning a GEM to a queue from a different T-CONT is not
 * supported by the QoS Manager */
static int
unsupported_us_queue_assignment(const struct queue_assignment_list *as,
				uint16_t tcont_me_id)
{
	unsigned int i = 0;

	if (tcont_me_id == 0xFFFF)
		return 0;

	if (!as->num_assignments)
		return 0;

	for (i = 0; i < as->num_assignments; ++i) {
		if (!as->assignments[i].us)
			continue;
		if (as->assignments[i].me_id != tcont_me_id)
			return 1;
	}

	return 0;
}

/*
 * Downstream GEM Ports have no T-CONT this is why we return
 * 0xFFFF - this means no T-CONT is assigned
 */
static uint16_t
tcont_ptr_get(const struct pa_gem_port_net_ctp_update_data *upd_data)
{
	return (upd_data->direction == PA_GEMPORTNETCTP_DIR_DOWNSTREAM)
		   ? 0xFFFF
		   : upd_data->tcont_ptr;
}

static bool is_gem_port_id_valid(struct pon_net_context *ctx, uint16_t gem_id)
{
	enum pa_pon_op_mode pon_mode = ctx->cfg.mode;

	switch (pon_mode) {
	case PA_PON_MODE_G984:
	case PA_PON_MODE_G989:
		if (gem_id > 4095)
			return false;
		return true;
	case PA_PON_MODE_G987:
	case PA_PON_MODE_G9807:
		if (gem_id < 1021 || gem_id > 65534)
			return false;
		return true;
	case PA_PON_MODE_UNKNOWN:
	default:
		dbg_err("system in unsupported pon mode\n");
		return false;
	}
}

static void
gem_port_net_ctp_from_update_data(struct pon_net_gem_port_net_ctp *gem_ctp,
			 const struct pa_gem_port_net_ctp_update_data *upd_data)
{
	dbg_in_args("%p, %p", gem_ctp, upd_data);

	gem_ctp->port_id = upd_data->gem_port_id;
	gem_ctp->tcont_me_id = tcont_ptr_get(upd_data);
	gem_ctp->us_priority_queue_me_id = upd_data->us_priority_queue_ptr;
	gem_ctp->ds_priority_queue_me_id = upd_data->ds_priority_queue_ptr;
	gem_ctp->us_td_me_id = upd_data->us_traffic_descriptor_profile_ptr;
	gem_ctp->ds_td_me_id = upd_data->ds_traffic_descriptor_profile_ptr;
	gem_ctp->color_marking = 0;
	gem_ctp->dir = upd_data->direction;

	dbg_out();
}

/*
 * Create tc-clsact qdisc on GEM Port network device. We need tc-clsact to
 * add tc-filters to GEM Port network device.
 */
static enum pon_adapter_errno create_clsact(struct pon_net_context *ctx,
					    uint16_t me_id)
{
	enum pon_adapter_errno ret;
	char ifname[IF_NAMESIZE] = {0};

	dbg_in_args("%p, %u", ctx, me_id);

	ret = pon_net_ifname_get(ctx, PON_CLASS_ID_GEM_PORT_NET_CTP_DATA, me_id,
				 ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_ifname_get, ret);
		return ret;
	}

	ret = netlink_qdisc_clsact_create(ctx->netlink, ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_qdisc_clsact_create, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Update qdisc tree on a device related to given GEM Port Network CTP.
 * When traffic management option on ONU-G is set to "rate", then qdisc
 * trees will have tc-tbf qdiscs as leaves. But these leaves should only exist
 * if the GEM port is connected to them. We need this function to refresh the
 * qdisc tree on the related device, which will result in creating or deleting
 * tc-tbfs, depending on whether the GEM port exists.
 */
static enum pon_adapter_errno
qdiscs_update(struct pon_net_context *ctx,
	      struct pon_net_gem_port_net_ctp *gem_ctp)
{
	uint8_t traffic_management_option = 0;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p", ctx, gem_ctp);

	traffic_management_option = pon_net_traffic_management_option_get(ctx);
	if (traffic_management_option != PA_TMO_RATE) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (direction_ds(gem_ctp->dir)) {
		ret =
		    pon_net_uni_qdiscs_update(ctx, PON_CLASS_ID_VEIP, NULL, 0);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_uni_qdiscs_update, ret);
			return ret;
		}

		ret = pon_net_uni_qdiscs_update(
		    ctx, PON_CLASS_ID_PPTP_ETHERNET_UNI, NULL, 0);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_uni_qdiscs_update, ret);
			return ret;
		}
	}

	if (direction_us(gem_ctp->dir)) {
		ret =
		    pon_net_tcont_qdiscs_update(ctx, &gem_ctp->tcont_me_id, 1);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_tcont_qdiscs_update, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Creates GEM port netdevice and adds it to ctx->db */
static enum pon_adapter_errno create_device(struct pon_net_context *ctx,
					    const uint16_t me_id,
					    const struct netlink_gem *gem_data)
{
	char ifname[IF_NAMESIZE];
	enum pon_adapter_errno ret;
	int written;

	dbg_in_args("%p, %u, %p", ctx, me_id, gem_data);

	written = snprintf(ifname, sizeof(ifname), PON_IFNAME_GEM, me_id);
	if (written >= (int)sizeof(ifname)) {
		FN_ERR_RET(written, snprintf, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = pon_net_dev_db_add(ctx->db, ifname, &pon_net_gem_def);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_add, ret);
		return ret;
	}

	ret = netlink_gem_create(ctx->netlink, me_id, gem_data);
	if (ret != PON_ADAPTER_SUCCESS) {
		pon_net_dev_db_del(ctx->db, ifname);
		FN_ERR_RET(ret, netlink_gem_create, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Destroy the GEM device and remove it from ctx->db */
static enum pon_adapter_errno destroy_device(struct pon_net_context *ctx,
					     const uint16_t me_id)
{
	char ifname[IF_NAMESIZE];
	enum pon_adapter_errno ret;
	int written;

	dbg_in_args("%p, %u", ctx, me_id);

	written = snprintf(ifname, sizeof(ifname), PON_IFNAME_GEM, me_id);
	if (written >= (int)sizeof(ifname)) {
		FN_ERR_RET(written, snprintf, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	pon_net_dev_db_del(ctx->db, ifname);

	ret = netlink_gem_destroy(ctx->netlink, me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_gem_destroy, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno update(void *ll_handle, const uint16_t me_id,
	const struct pa_gem_port_net_ctp_update_data *upd_data)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_gem_port_net_ctp gem_ctp = {0};
	struct pon_net_gem_port_net_ctp old_gem_ctp = {0};
	struct queue_assignment_list as = {0};
	struct netlink_gem gem_data = {0};
	uint32_t existing_me_id;
	bool update = false;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, upd_data);

	netlink_gem_defaults(&gem_data);

	gem_port_net_ctp_from_update_data(&gem_ctp, upd_data);

	if (direction_us(upd_data->direction) &&
	    gem_ctp.tcont_me_id == 0xFFFF) {
		/* The bidirectional direction and upstream direction
		 * require a T-CONT */
		dbg_err("Upstream and bidirectional GEMs require a T-CONT\n");
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	if (direction_us(upd_data->direction)) {
		read_assignments(ctx, gem_ctp.tcont_me_id,
				 gem_ctp.us_priority_queue_me_id, &as);
		if (unsupported_us_queue_assignment(&as, gem_ctp.tcont_me_id))
			dbg_wrn("Assigning a GEM port to a queue from a different T-CONT is not supported.\n");
	}

	if (gem_ctp.tcont_me_id != 0xFFFF &&
	    !pon_net_me_list_exists(&ctx->me_list, PON_CLASS_ID_TCONT,
		    gem_ctp.tcont_me_id)) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	if (!is_gem_port_id_valid(ctx, gem_ctp.port_id)) {
		dbg_err("Invalid GEM port id\n");
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	ret = mapper_id_get(ctx->mapper[MAPPER_GEMPORTCTP_MEID_TO_ID],
			    &existing_me_id, upd_data->gem_port_id);
	if (ret == PON_ADAPTER_SUCCESS) {
		if (existing_me_id != me_id) {
			dbg_err("GEM port %d already in use\n",
				upd_data->gem_port_id);
			dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
			return PON_ADAPTER_ERR_INVALID_VAL;
		}
	}

	ret = pon_net_me_list_read(&ctx->me_list,
				   PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
				   me_id,
				   &old_gem_ctp,
				   sizeof(old_gem_ctp));
	if (ret != PON_ADAPTER_SUCCESS) {
		ret = pon_net_me_list_write(&ctx->me_list,
					PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
					me_id,
					&gem_ctp,
					sizeof(gem_ctp));
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_me_list_write, ret);
			return ret;
		}
	} else if (gem_ctp.port_id != old_gem_ctp.port_id) {
		/* port_id changed, need to re-create GEM port */
		if (old_gem_ctp.has_pmapper) {
			/* re-creating GEM if it is connected to pmapper will
			 * not work
			 */
			dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_SUPPORTED);
			return PON_ADAPTER_ERR_NOT_SUPPORTED;
		}
		mapper_id_remove(ctx->mapper[MAPPER_GEMPORTCTP_MEID_TO_ID],
				 me_id);

		remove_queue_filters(ctx, me_id, &old_gem_ctp.ds);
		old_gem_ctp.ds.num_assignments = 0;
		remove_queue_filters(ctx, me_id, &old_gem_ctp.us);
		old_gem_ctp.us.num_assignments = 0;

		ret = destroy_device(ctx, me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_out_ret("%d", ret);
			return ret;
		}

		old_gem_ctp.port_id = gem_ctp.port_id;

		ret = pon_net_me_list_write(&ctx->me_list,
					PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
					me_id,
					&old_gem_ctp,
					sizeof(old_gem_ctp));
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_me_list_write, ret);
			return ret;
		}
	} else {
		if (direction_us(old_gem_ctp.dir) && !direction_us(gem_ctp.dir))
			gem_ctp.us_priority_queue_me_id = 0;
		if (direction_ds(old_gem_ctp.dir) && !direction_ds(gem_ctp.dir))
			gem_ctp.ds_priority_queue_me_id = 0;
		gem_ctp.us = old_gem_ctp.us;
		gem_ctp.ds = old_gem_ctp.ds;
		gem_ctp.has_pmapper = old_gem_ctp.has_pmapper;
		update = true;
	}

	gem_data.port_id = upd_data->gem_port_id;
	gem_data.dir = direction_to_netlink_direction(upd_data->direction);
	gem_data.tcont_meid = gem_ctp.tcont_me_id;
	gem_data.enc = enc_to_netlink_enc(upd_data->encryption_key_ring);
	gem_data.max_size = PON_NET_GEM_PORT_MAX_FRAME_SIZE;

	if (update) {
		ret = netlink_gem_update(ctx->netlink, me_id, &gem_data);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_gem_update, ret);
			return ret;
		}

		ret = pon_net_me_list_write(&ctx->me_list,
					PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
					me_id,
					&gem_ctp,
					sizeof(gem_ctp));
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_me_list_write, ret);
			return ret;
		}
	} else {
		ret = create_device(ctx, me_id, &gem_data);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, create_device, ret);
			return ret;
		}

		ret = mapper_explicit_map(
			ctx->mapper[MAPPER_GEMPORTCTP_MEID_TO_ID],
			me_id, upd_data->gem_port_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			netlink_gem_destroy(ctx->netlink, me_id);
			FN_ERR_RET(ret, mapper_explicit_map, ret);
			return ret;
		}

		ret = create_clsact(ctx, me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, create_clsact, ret);
			return ret;
		}
	}

	ret = qdiscs_update(ctx, &gem_ctp);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, qdiscs_update, ret);
		return ret;
	}

	ret = pon_net_gem_queue_update(ctx, gem_data.dir, &me_id, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		netlink_gem_destroy(ctx->netlink, me_id);
		FN_ERR_RET(ret, pon_net_gem_queue_update, ret);
		return ret;
	}

	ret = pon_net_police_update_gem(ctx, me_id, NETLINK_FILTER_DIR_INGRESS);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_police_update_gem, ret);
		return ret;
	}
	ret = pon_net_police_update_gem(ctx, me_id, NETLINK_FILTER_DIR_EGRESS);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_police_update_gem, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* This checks if the given ME is from the Multicast GEM and deletes the
 * reference in the mc_gem_ifindex attribute if this is the multicast ME.
 * Other code now knows that the Multicast GEM is gone.
 *
 * Errors are only logged to not prevent the removing of the device.
 */
#ifdef MCC_DRV_ENABLE
static void remove_mc_gem_idx(struct pon_net_context *ctx, const uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char gem_ifname[IF_NAMESIZE] = {0};
	int gem_ifindex;

	ret = pon_net_ifname_get(ctx, PON_CLASS_ID_GEM_PORT_NET_CTP_DATA, me_id,
				 gem_ifname, sizeof(gem_ifname));
	if (ret != PON_ADAPTER_SUCCESS)
		return;

	gem_ifindex = netlink_netdevice_ifindex_get(ctx->netlink, gem_ifname);
	if (!gem_ifindex) {
		dbg_err_fn(netlink_netdevice_ifindex_get);
		return;
	}
	if (ctx->mc_gem_ifindex == gem_ifindex)
		ctx->mc_gem_ifindex = 0;
}
#else
static void remove_mc_gem_idx(struct pon_net_context *ctx, const uint16_t me_id)
{
	(void)ctx;
	(void)me_id;
}
#endif /* MCC_DRV_ENABLE */

static enum pon_adapter_errno
destroy(void *ll_handle, const uint16_t me_id,
	const struct pa_gem_port_net_ctp_destroy_data *dst_data)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_gem_port_net_ctp gem_ctp = {0};
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, dst_data);

	remove_mc_gem_idx(ctx, me_id);

	ret = pon_net_me_list_read(&ctx->me_list,
				   PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
				   me_id,
				   &gem_ctp,
				   sizeof(gem_ctp));
	if (ret == PON_ADAPTER_SUCCESS)
		pon_net_gem_queue_unassign(ctx, gem_ctp.dir, &me_id, 1);

	destroy_device(ctx, me_id);
	mapper_id_remove(ctx->mapper[MAPPER_GEMPORTCTP_MEID_TO_ID], me_id);

	pon_net_me_list_remove(&ctx->me_list,
			       PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
			       me_id);

	/*
	 * Call qdiscs_update() after GEM Port is removed from the list, because
	 * it will call functions which iterate me_list.
	 */
	ret = qdiscs_update(ctx, &gem_ctp);
	dbg_out_ret("%d", ret);
	return ret;
}

const struct pa_gem_port_net_ctp_ops gem_port_net_ctp_ops = {
	.update = update,
	.destroy = destroy,
};
