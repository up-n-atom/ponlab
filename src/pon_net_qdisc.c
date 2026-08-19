/*****************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#include <math.h>

#include "pon_net_common.h"
#include "pon_net_debug.h"
#include "pon_net_qdisc.h"
#include "pon_net_netlink.h"

/**
 * The quanta value is a internal parameter of the PPv4 QoS engine,
 * which initialized during initialization. Currently it is set to 4K.
 * This value is read-only and later will be exported by the QoS drivers
 * to the user space. For this first implementation we have to hard-code
 * this value (4K) to enable the validation of the WRR.
 */
#define QUANTA 4096

uint32_t pon_net_calculate_quantum(uint8_t weight)
{
	return (uint32_t)QUANTA * weight;
}

static int qdisc_class_eq(const struct qdisc_class *a,
			   const struct qdisc_class *b)
{
	return a->arg == b->arg && a->state == b->state;
}

static int qdisc_type_eq(const struct pon_net_qdisc *a,
			 const struct pon_net_qdisc *b)
{
	if (a->type != b->type)
		return 0;

	if (!a->type->data_eq)
		return 1;

	return a->type->data_eq(a->type_data, b->type_data);
}

static int qdisc_eq(const struct pon_net_qdisc *a,
		    const struct pon_net_qdisc *b)
{
	return qdisc_type_eq(a, b) &&
		a->handle == b->handle &&
		a->parent == b->parent &&
		a->parent_queue == b->parent_queue;
}

unsigned int pon_net_qdisc_classes_len(const struct pon_net_qdisc *qdisc)
{
	unsigned int count = 0;
	unsigned int i = 0;

	dbg_in_args("%p", qdisc);

	for (i = 0; i < ARRAY_SIZE(qdisc->classes); ++i)
		if (qdisc->classes[i].state == CLASS_USED)
			count++;

	dbg_out_ret("%d", count);
	return count;
}

unsigned int pon_net_qdisc_classes_last(const struct pon_net_qdisc *qdisc)
{
	unsigned int last = 0;
	unsigned int i = 0;

	dbg_in_args("%p", qdisc);

	for (i = 0; i < ARRAY_SIZE(qdisc->classes); ++i)
		if (qdisc->classes[i].state == CLASS_USED)
			last = i + 1;

	dbg_out_ret("%d", last);
	return last;
}

#define ANY_CLASS 0xFFFFFFFFFFFFFFFFULL
/* When configuring the mixed mode (SP + WRR), the WRR scheduler has to be
 * connected to next free input on the SP scheduler. */
static enum pon_adapter_errno
qdisc_class_take_next_free(struct pon_net_qdisc *qdisc,
			   struct pon_net_qdisc *child, uint32_t arg,
			   uint64_t acceptable_mask)
{
	unsigned int i;
	uint32_t class_id;

	dbg_in_args("%p, %p, %u", qdisc, child, arg);

	PON_NET_STATIC_ASSERT(
		ARRAY_SIZE(qdisc->classes) <= sizeof(acceptable_mask) * 8,
		mask_must_be_big_enough_to_cover_all_classes);

	for (i = 0; i < ARRAY_SIZE(qdisc->classes); ++i) {
		if (!((0x1ULL << i) & acceptable_mask))
			continue;

		if (qdisc->classes[i].state == CLASS_UNUSED) {
			qdisc->classes[i].state = CLASS_USED;
			qdisc->classes[i].arg = arg;
			qdisc->classes[i].child_idx = CHILD_PRIORITY_QUEUE;
			if (child)
				qdisc->classes[i].child_idx = child->idx;
			break;
		}
	}

	if (!child) {
		dbg_out_ret("%u", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	class_id = i + 1;
	if (class_id > ARRAY_SIZE(child->classes)) {
		dbg_out_ret("%u", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	child->parent_queue = class_id;

	/* No free class found - limit exceeded */
	dbg_out_ret("%u", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static void qdisc_clear_classes(struct pon_net_qdisc *qdisc)
{
	unsigned int i;

	dbg_in_args("%p", qdisc);

	for (i = 0; i < ARRAY_SIZE(qdisc->classes); ++i)
		qdisc->classes[i].state = CLASS_UNUSED;

	dbg_out();
}

enum pon_adapter_errno
pon_net_qdisc_type_set(struct pon_net_qdisc *qdisc,
		       const struct pon_net_qdisc_type *type,
		       const void *params)
{
	void *type_data = NULL;
	int err;

	dbg_in_args("%p, %p", qdisc, type);

	if (type && type->data_len) {
		type_data = calloc(1, type->data_len);
		if (!type_data) {
			FN_ERR_RET(0, malloc, PON_ADAPTER_ERR_NO_MEMORY);
			return PON_ADAPTER_ERR_NO_MEMORY;
		}

		if (params) {
			err = memcpy_s(type_data, type->data_len, params,
				       type->data_len);
			if (err) {
				free(type_data);
				FN_ERR_RET(err, memcpy_s,
					   PON_ADAPTER_ERR_MEM_ACCESS);
				return PON_ADAPTER_ERR_MEM_ACCESS;
			}
		}
	}

	if (qdisc->type_data)
		free(qdisc->type_data);

	qdisc->type = type;
	qdisc->type_data = type_data;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static void qdisc_init(struct pon_net_qdisc *qdisc,
		       unsigned int idx,
		       uint32_t handle,
		       const struct pon_net_qdisc *parent)
{
	const struct pon_net_qdisc new = {
		.handle = handle,
		.parent = parent ? parent->handle : 0,
		.parent_idx = parent ? parent->idx : idx,
		.idx = idx
	};

	dbg_in_args("%p, %d, %u, %p", qdisc, idx, handle, parent);

	*qdisc = new;

	dbg_out();
}

static void qdisc_swap(struct pon_net_qdisc *a, struct pon_net_qdisc *b)
{
	struct pon_net_qdisc tmp = *a;

	dbg_in_args("%p, %p", a, b);

	*a = *b;
	*b = tmp;

	dbg_out();
}

static void qdisc_exit(struct pon_net_qdisc *qdisc)
{
	dbg_in_args("%p", qdisc);

	if (qdisc->type_data)
		free(qdisc->type_data);

	dbg_out();
}

struct pon_net_qdiscs *pon_net_qdiscs_create(void)
{
	struct pon_net_qdiscs *qdiscs;

	dbg_in();

	qdiscs = calloc(1, sizeof(*qdiscs));
	if (!qdiscs) {
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	dbg_out_ret("%p", qdiscs);
	return qdiscs;
}

void pon_net_qdiscs_destroy(struct pon_net_qdiscs *qdiscs)
{
	unsigned int i = 0;

	dbg_in_args("%p", qdiscs);

	for (i = 0; i < qdiscs->length; ++i)
		qdisc_exit(&qdiscs->qdiscs[i]);

	free(qdiscs);

	dbg_out();
}

void pon_net_qdiscs_swap(struct pon_net_qdiscs *dst,
			 struct pon_net_qdiscs *src)
{
	unsigned int i = 0;
	unsigned int length;

	dbg_in_args("%p, %p", dst, src);

	for (i = 0; i < ARRAY_SIZE(src->qdiscs); ++i)
		qdisc_swap(&src->qdiscs[i], &dst->qdiscs[i]);

	length = src->length;
	src->length = dst->length;
	dst->length = length;

	dbg_out();
}

unsigned int pon_net_qdiscs_len(const struct pon_net_qdiscs *qdiscs)
{
	return qdiscs->length;
}

struct pon_net_qdisc *qdiscs_get_by_handle(struct pon_net_qdiscs *tree,
					   uint16_t qdisc_handle)
{
	unsigned int i = 0;

	dbg_in_args("%p, %u", tree, qdisc_handle);

	for (i = 0; i < tree->length; ++i) {
		if (tree->qdiscs[i].handle == qdisc_handle) {
			dbg_out_ret("%p", &tree->qdiscs[i]);
			return &tree->qdiscs[i];
		}
	}

	dbg_out_ret("%p", NULL);
	return NULL;
}

struct pon_net_qdisc *pon_net_qdiscs_get(struct pon_net_qdiscs *qdiscs,
					 unsigned int qdisc_idx)
{
	struct pon_net_qdisc *qdisc;

	dbg_in_args("%p, %d", qdiscs, qdisc_idx);

	if (qdisc_idx >= ARRAY_SIZE(qdiscs->qdiscs)) {
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	qdisc = &qdiscs->qdiscs[qdisc_idx];

	dbg_out_ret("%p", qdisc);
	return qdisc;
}

static struct pon_net_qdisc *
qdiscs_add(struct pon_net_qdiscs *qdiscs, uint16_t handle,
	   const struct pon_net_qdisc_type *type_of_created,
	   const void *data,
	   struct pon_net_qdisc *parent_qdisc)
{
	enum pon_adapter_errno ret;
	struct pon_net_qdisc *new_qdisc;

	dbg_in_args("%p, %u, %p, %p, %p", qdiscs, handle, type_of_created,
		    data, parent_qdisc);

	if (qdiscs->length == ARRAY_SIZE(qdiscs->qdiscs)) {
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	new_qdisc = &qdiscs->qdiscs[qdiscs->length];

	qdisc_init(new_qdisc, qdiscs->length, handle, parent_qdisc);

	ret = pon_net_qdisc_type_set(new_qdisc, type_of_created, data);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_qdisc_type_set, 0);
		qdisc_exit(new_qdisc);
		return NULL;
	}

	qdiscs->length++;

	dbg_out_ret("%p", new_qdisc);
	return new_qdisc;
}

struct pon_net_qdisc *
pon_net_qdiscs_get_or_add(struct pon_net_qdiscs *qdiscs, uint16_t handle,
			  const struct pon_net_qdisc_type *type_of_created,
			  const void *data_of_created,
			  struct pon_net_qdisc *parent_of_created)
{
	struct pon_net_qdisc *qdisc = NULL;

	dbg_in_args("%p, %u, %p, %p, %p", qdiscs, handle, type_of_created,
		    data_of_created, parent_of_created);

	qdisc = qdiscs_get_by_handle(qdiscs, handle);
	if (qdisc) {
		dbg_out_ret("%p", qdisc);
		return qdisc;
	}

	qdisc = qdiscs_add(qdiscs, handle, type_of_created, data_of_created,
			   parent_of_created);
	if (!qdisc) {
		FN_ERR_RET(0, qdisc, 0);
		return NULL;
	}

	dbg_out_ret("%p", qdisc);
	return qdisc;
}

static enum pon_adapter_errno
qdiscs_subtree_apply(const struct pon_net_qdiscs *qdiscs, unsigned int idx,
		     const char *ifname, struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;
	unsigned int i = 0;
	unsigned int child_idx = 0;
	const struct pon_net_qdisc *qdisc;

	dbg_in_args("%p, %u, \"%s\", %p", qdiscs, idx, ifname, ctx);

	qdisc = &qdiscs->qdiscs[idx];

	/* We support only MAX_QUEUES queues - this subtree can't be attached
	 * to the parent anyway so we just quit. */
	if (qdisc->parent_queue > MAX_QUEUES) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = qdisc->type->create(ctx, ifname, qdisc);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, create, ret);
		return ret;
	}

	for (i = 0; i < ARRAY_SIZE(qdisc->classes); i++) {
		if (qdisc->classes[i].state != CLASS_USED)
			continue;

		if (qdisc->type->class_create) {
			ret = qdisc->type->class_create(ctx, ifname, qdisc, i);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, class_create, ret);
				return ret;
			}
		}

		child_idx = qdisc->classes[i].child_idx;
		if (child_idx == CHILD_PRIORITY_QUEUE)
			continue;

		ret = qdiscs_subtree_apply(qdiscs, child_idx, ifname, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, qdiscs_subtree_apply, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
qdiscs_subtree_cleanup(const struct pon_net_qdiscs *qdiscs, unsigned int idx,
		       const char *ifname, struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;
	const struct pon_net_qdisc *qdisc;

	dbg_in_args("%p, %u, \"%s\", %p", qdiscs, idx, ifname, ctx);

	qdisc = &qdiscs->qdiscs[idx];

	/* We support only MAX_QUEUES queues - this subtree can't be attached
	 * to the parent anyway so we just quit, because there is
	 * nothing to be cleaned up. */
	if (qdisc->parent_queue > MAX_QUEUES) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = qdisc->type->destroy(ctx, ifname, qdisc);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, destroy, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Change only the part of qdisc tree that changed */
static enum pon_adapter_errno
qdiscs_subtree_cleanup_changed(const struct pon_net_qdiscs *qdiscs,
			       unsigned int idx,
			       const struct pon_net_qdiscs *old_qdiscs,
			       unsigned int old_idx, const char *ifname,
			       struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;
	unsigned int i = 0;
	const struct pon_net_qdisc *qdisc;
	const struct pon_net_qdisc *old_qdisc;
	const struct qdisc_class *class;
	const struct qdisc_class *old_class;

	dbg_in_args("%p, %d, \"%s\", %p", qdiscs, idx, ifname, ctx);

	if ((qdiscs->length == 0 && old_qdiscs->length == 0) ||
	    (old_idx == CHILD_PRIORITY_QUEUE && idx == CHILD_PRIORITY_QUEUE)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if ((qdiscs->length && old_qdiscs->length == 0) ||
	    (old_idx == CHILD_PRIORITY_QUEUE && idx != CHILD_PRIORITY_QUEUE)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if ((qdiscs->length == 0 && old_qdiscs->length) ||
	    (old_idx != CHILD_PRIORITY_QUEUE && idx == CHILD_PRIORITY_QUEUE)) {
		ret = qdiscs_subtree_cleanup(old_qdiscs, old_idx, ifname, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, qdiscs_subtree_cleanup, ret);
			return ret;
		}
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	qdisc = &qdiscs->qdiscs[idx];
	old_qdisc = &old_qdiscs->qdiscs[old_idx];

	if (!qdisc_eq(old_qdisc, qdisc)) {
		/* Qdisc are different - recreate the entire subtree */
		ret = qdiscs_subtree_cleanup(old_qdiscs, old_idx, ifname, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, qdiscs_subtree_cleanup, ret);
			return ret;
		}
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	/* Delete old */
	for (i = 0; i < ARRAY_SIZE(qdisc->classes); i++) {
		old_class = &old_qdisc->classes[i];
		class = &qdisc->classes[i];

		if (qdisc_class_eq(old_class, class))
			continue;

		if (old_class->state != CLASS_USED)
			continue;

		/* Classes are different - drop the entire subtree */
		if (old_class->child_idx != CHILD_PRIORITY_QUEUE &&
		    old_class->child_idx < MAX_QDISCS) {
			ret = qdiscs_subtree_cleanup(old_qdiscs,
						     old_class->child_idx,
						     ifname,
						     ctx);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, qdiscs_subtree_cleanup, ret);
				return ret;
			}
		}

		/* Destroy the class */
		if (!qdisc->type->class_destroy)
			continue;

		ret = qdisc->type->class_destroy(ctx, ifname,
						 old_qdisc, i);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, class_destroy, ret);
			return ret;
		}
	}

	/* Traverse down */
	for (i = 0; i < ARRAY_SIZE(qdisc->classes); i++) {
		old_class = &old_qdisc->classes[i];
		class = &qdisc->classes[i];

		if (!qdisc_class_eq(old_class, class))
			continue;

		if (class->state != CLASS_USED)
			continue;

		ret = qdiscs_subtree_cleanup_changed(qdiscs,
						     class->child_idx,
						     old_qdiscs,
						     old_class->child_idx,
						     ifname,
						     ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, qdiscs_subtree_cleanup_changed, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Change only the part of qdisc tree that changed */
static enum pon_adapter_errno
qdiscs_subtree_apply_changed(const struct pon_net_qdiscs *qdiscs,
			     unsigned int idx,
			     const struct pon_net_qdiscs *old_qdiscs,
			     unsigned int old_idx, const char *ifname,
			     struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;
	unsigned int i = 0;
	const struct pon_net_qdisc *qdisc;
	const struct pon_net_qdisc *old_qdisc;
	const struct qdisc_class *class;
	const struct qdisc_class *old_class;

	dbg_in_args("%p, %d, \"%s\", %p", qdiscs, idx, ifname, ctx);

	if ((qdiscs->length == 0 && old_qdiscs->length == 0) ||
	    (old_idx == CHILD_PRIORITY_QUEUE && idx == CHILD_PRIORITY_QUEUE)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if ((qdiscs->length && old_qdiscs->length == 0) ||
	    (old_idx == CHILD_PRIORITY_QUEUE && idx != CHILD_PRIORITY_QUEUE)) {
		ret = qdiscs_subtree_apply(qdiscs, idx, ifname, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, qdiscs_subtree_apply, ret);
			return ret;
		}
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if ((qdiscs->length == 0 && old_qdiscs->length) ||
	    (old_idx != CHILD_PRIORITY_QUEUE && idx == CHILD_PRIORITY_QUEUE)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	qdisc = &qdiscs->qdiscs[idx];
	old_qdisc = &old_qdiscs->qdiscs[old_idx];

	if (!qdisc_eq(old_qdisc, qdisc)) {
		ret = qdiscs_subtree_apply(qdiscs, idx, ifname, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, qdiscs_subtree_apply, ret);
			return ret;
		}
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	/* Create new */
	for (i = 0; i < ARRAY_SIZE(qdisc->classes); i++) {
		old_class = &old_qdisc->classes[i];
		class = &qdisc->classes[i];

		if (qdisc_class_eq(old_class, class))
			continue;

		if (class->state != CLASS_USED)
			continue;

		/* Create the class */
		if (qdisc->type->class_create) {
			ret = qdisc->type->class_create(ctx, ifname,
							qdisc, i);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, class_create, ret);
				return ret;
			}
		}

		if (class->child_idx != CHILD_PRIORITY_QUEUE &&
		    class->child_idx < MAX_QDISCS) {
			/* Create the subtree */
			ret = qdiscs_subtree_apply(
			    qdiscs, class->child_idx, ifname, ctx);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, qdiscs_subtree_apply,
					   ret);
				return ret;
			}
		}
	}

	/* Traverse down */
	for (i = 0; i < ARRAY_SIZE(qdisc->classes); i++) {
		old_class = &old_qdisc->classes[i];
		class = &qdisc->classes[i];

		if (!qdisc_class_eq(old_class, class))
			continue;

		if (class->state != CLASS_USED)
			continue;

		ret = qdiscs_subtree_apply_changed(
		    qdiscs, class->child_idx, old_qdiscs, old_class->child_idx,
		    ifname, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, qdiscs_subtree_apply_changed, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_qdiscs_diff_apply(const struct pon_net_qdiscs *qdiscs,
			  const struct pon_net_qdiscs *old_qdiscs,
			  const char *ifname, struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p, \"%s\", %p", qdiscs, old_qdiscs, ifname, ctx);

	ret = qdiscs_subtree_cleanup_changed(qdiscs, 0, old_qdiscs, 0,
					     ifname, ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, qdiscs_subtree_cleanup_changed, ret);
		return ret;
	}
	ret = qdiscs_subtree_apply_changed(qdiscs, 0, old_qdiscs, 0,
					   ifname, ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, qdiscs_subtree_apply_changed, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static unsigned int num_of_used_classes(struct pon_net_qdisc *qdisc,
					unsigned int start,
					unsigned int count,
					unsigned int increment)
{
	unsigned int i = 0;
	unsigned int cnt = 0;

	dbg_in_args("%p, %u, %u, %u", qdisc, start, count, increment);

	for (i = 0; i < count; ++i) {
		unsigned int idx = start + i * increment;

		if (idx >= ARRAY_SIZE(qdisc->classes))
			break;

		if (qdisc->classes[idx].state == CLASS_USED)
			cnt++;
	}

	dbg_out_ret("%u", cnt);
	return cnt;
}

static enum pon_adapter_errno
transfer_children(struct pon_net_qdiscs *qdiscs, unsigned int src_idx,
		  unsigned int first_child, unsigned int count,
		  unsigned int increment, unsigned int dst_idx)
{
	unsigned int i = 0;
	struct pon_net_qdisc *qdisc = NULL;
	struct pon_net_qdisc *child = NULL;
	struct pon_net_qdisc *transferred_child = NULL;
	unsigned int child_class_number = 0;

	dbg_in_args("%p, %u, %u, %u, %u, %u", qdiscs, src_idx, first_child,
		    count, increment, dst_idx);

	qdisc = pon_net_qdiscs_get(qdiscs, src_idx);
	if (!qdisc) {
		FN_ERR_RET(0, pon_net_qdiscs_get, PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	child = pon_net_qdiscs_get(qdiscs, dst_idx);
	if (!child) {
		FN_ERR_RET(0, pon_net_qdiscs_get, PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	for (i = 0; i < count; ++i) {
		struct qdisc_class *class = NULL;
		unsigned int idx = first_child + i * increment;

		if (idx >= ARRAY_SIZE(qdisc->classes)) {
			dbg_err("%u <= %zu is false", idx,
				ARRAY_SIZE(qdisc->classes));
			dbg_out_ret("%d", PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}


		class = &qdisc->classes[idx];

		/* Transfer class down into the hierarchy */
		if (class->state == CLASS_USED &&
		    class->child_idx != CHILD_PRIORITY_QUEUE &&
		    class->child_idx < MAX_QDISCS) {
			transferred_child = pon_net_qdiscs_get(qdiscs,
						(unsigned int)class->child_idx);
			if (!transferred_child) {
				/* I this happens it means that the tree is in
				 * inconsistent state */
				FN_ERR_RET(0, pon_net_qdisc_get_or_add,
					   PON_ADAPTER_ERROR);
				return PON_ADAPTER_ERROR;
			}
			transferred_child->parent_idx = child->idx;
			transferred_child->parent = child->handle;
			transferred_child->parent_queue =
			    child_class_number + 1;
		}
		child->classes[child_class_number].child_idx = class->child_idx;
		child->classes[child_class_number].state = class->state;
		child->classes[child_class_number].arg = class->arg;

		/* Cleanup transferred class */
		class->state = CLASS_UNUSED;
		class->arg = 0;
		class->child_idx = CHILD_PRIORITY_QUEUE;

		child_class_number++;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_qdiscs_partition(struct pon_net_qdiscs *qdiscs,
			 unsigned int idx,
			 unsigned int threshold,
			 uint16_t (*handle_gen)(unsigned int child_number,
						void *ctx),
			 void *ctx)
{
	/* TODO: Weight scaling? */
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	unsigned int i = 0;
	struct pon_net_qdisc *qdisc = NULL;
	struct pon_net_qdisc *child = NULL;

	dbg_in_args("%p, %u, %u, %p, %p", qdiscs, idx, threshold, handle_gen,
		    ctx);

	qdisc = pon_net_qdiscs_get(qdiscs, idx);
	if (!qdisc) {
		FN_ERR_RET(0, pon_net_qdiscs_get, PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	if (threshold < 1) {
		dbg_err("partitioning threshold must be a positive number\n");
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	for (i = 0; i < (ARRAY_SIZE(qdisc->classes) - 1) / threshold + 1; ++i) {
		unsigned int first_class = i;
		uint16_t handle = handle_gen(i, ctx);

		if (num_of_used_classes(qdisc, first_class, threshold, 8) <= 1)
			continue;

		child = pon_net_qdiscs_get_or_add(qdiscs, handle, qdisc->type,
						  qdisc->type_data, qdisc);
		if (!child) {
			FN_ERR_RET(0, pon_net_qdisc_get_or_add,
				   PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		ret = transfer_children(qdiscs, idx, first_class, threshold, 8,
					child->idx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, transfer_children, ret);
			return ret;
		}

		qdisc->classes[i].child_idx = child->idx;
		qdisc->classes[i].state = CLASS_USED;
		child->parent_queue = i + 1;
	}

	if (qdisc->type->after_partition)
		qdisc->type->after_partition(qdiscs, qdisc->idx);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Allowing drr qdiscs to have drr parent causes errors in the driver during
 * BW reconfiguration. Temporarily we work around the problem by making
 * every parent of drr an prio qdisc. */
static enum pon_adapter_errno
workaround_drr_parent_must_be_prio(struct pon_net_qdisc *parent,
				     const struct pon_net_qdisc *qdisc)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p", parent, qdisc);

	/* Limitation: every drr qdisc parent must be prio qdisc */
	if (qdisc->type == &pon_net_drr_type) {
		ret = pon_net_qdisc_type_set(parent, &pon_net_prio_type,
					     NULL);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_qdisc_type_set, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_qdiscs_change_drr_parents_to_prio(struct pon_net_qdiscs *qdiscs)
{
	unsigned int i = 0;
	struct pon_net_qdisc *parent = NULL;
	struct pon_net_qdisc *qdisc = NULL;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p", qdiscs);

	for (i = 0; i < pon_net_qdiscs_len(qdiscs); ++i) {
		qdisc = pon_net_qdiscs_get(qdiscs, i);
		if (!qdisc) {
			FN_ERR_RET(0, pon_net_qdiscs_get,
				   PON_ADAPTER_ERR_NOT_FOUND);
			return PON_ADAPTER_ERR_NOT_FOUND;
		}

		if (!qdisc->parent)
			continue;

		if (qdisc->parent_idx == qdisc->idx)
			continue;

		parent = pon_net_qdiscs_get(qdiscs, qdisc->parent_idx);
		if (!parent) {
			FN_ERR_RET(0, parent, PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		ret = workaround_drr_parent_must_be_prio(parent, qdisc);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, workaround_drr_parent_must_be_prio,
				   ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static uint64_t acceptable_queues(unsigned int parent_queue)
{
	uint64_t queues = 0;
	int num_classes = ARRAY_SIZE(((struct pon_net_qdisc *)(0))->classes);
	unsigned int q = (parent_queue - 1) % MAX_QUEUES;

	dbg_in_args("%u", parent_queue);

	while (num_classes > 0) {
		queues <<= MAX_QUEUES;
		queues |= 1ULL << q;
		num_classes -= MAX_QUEUES;
	}

	dbg_out_ret("%" PRIu64, queues);
	return queues;
}

enum pon_adapter_errno
pon_net_qdiscs_assign_to_classes(struct pon_net_qdiscs *qdiscs)
{
	unsigned int i = 0;
	struct pon_net_qdisc *parent = NULL;
	struct pon_net_qdisc *qdisc = NULL;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p", qdiscs);

	/* Cleanup classes */
	for (i = 0; i < pon_net_qdiscs_len(qdiscs); ++i) {
		qdisc = pon_net_qdiscs_get(qdiscs, i);
		if (!qdisc) {
			FN_ERR_RET(0, pon_net_qdiscs_get,
				   PON_ADAPTER_ERR_NOT_FOUND);
			return PON_ADAPTER_ERR_NOT_FOUND;
		}

		qdisc_clear_classes(qdisc);
	}

	/* First - reserve the classes for qdiscs that have specified
	 * parent_queue */
	for (i = 0; i < pon_net_qdiscs_len(qdiscs); ++i) {
		uint64_t queues_to_try;

		qdisc = pon_net_qdiscs_get(qdiscs, i);
		if (!qdisc) {
			FN_ERR_RET(0, pon_net_qdiscs_get,
				   PON_ADAPTER_ERR_NOT_FOUND);
			return PON_ADAPTER_ERR_NOT_FOUND;
		}

		if (!qdisc->parent)
			continue;

		if (qdisc->parent_idx == qdisc->idx)
			continue;

		parent = pon_net_qdiscs_get(qdiscs, qdisc->parent_idx);
		if (!parent) {
			FN_ERR_RET(0, parent, PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		if (!qdisc->parent_queue)
			continue;

		queues_to_try = acceptable_queues(qdisc->parent_queue);

		ret = qdisc_class_take_next_free(parent, qdisc,
						 qdisc->parent_arg,
						 queues_to_try);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, qdisc_class_take_next_free, ret);
			return PON_ADAPTER_ERROR;
		}
	}

	/* Reserve the remaining classes for qdiscs that have not specified
	 * parent_queue */
	for (i = 0; i < pon_net_qdiscs_len(qdiscs); ++i) {
		qdisc = pon_net_qdiscs_get(qdiscs, i);
		if (!qdisc) {
			FN_ERR_RET(0, pon_net_qdiscs_get,
				   PON_ADAPTER_ERR_NOT_FOUND);
			return PON_ADAPTER_ERR_NOT_FOUND;
		}

		if (!qdisc->parent)
			continue;

		if (qdisc->parent_idx == qdisc->idx)
			continue;

		parent = pon_net_qdiscs_get(qdiscs, qdisc->parent_idx);
		if (!parent) {
			FN_ERR_RET(0, parent, PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		if (qdisc->parent_queue)
			continue;

		ret = qdisc_class_take_next_free(parent, qdisc,
						 qdisc->parent_arg,
						 ANY_CLASS);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, qdisc_class_take_net_free, ret);
			return PON_ADAPTER_ERROR;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno generic_destroy(struct pon_net_context *ctx,
					      const char *ifname,
					      const struct pon_net_qdisc *qdisc)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\", %p", ctx, ifname, qdisc);

	if (qdisc->handle > 0xFFFF) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = netlink_qdisc_destroy(ctx->netlink, ifname,
				    (uint16_t)qdisc->handle,
				    (uint16_t)qdisc->parent,
				    (uint16_t)qdisc->parent_queue);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_qdisc_destroy, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
prio_create(struct pon_net_context *ctx, const char *ifname,
	    const struct pon_net_qdisc *qdisc)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\", %p", ctx, ifname, qdisc);

	if (qdisc->handle > 0xFFFF || qdisc->parent > 0xFFFF ||
	    qdisc->parent_queue > 0xFFFF) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	ret = netlink_qdisc_prio_create(ctx->netlink, ifname,
					(uint16_t)qdisc->handle,
					(uint16_t)qdisc->parent,
					(uint16_t)qdisc->parent_queue);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_qdisc_prio_create, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

#define DRR_WEIGHT 4
static void prio_after_partition(struct pon_net_qdiscs *qdiscs,
				   unsigned int idx)
{
	struct pon_net_qdisc *qdisc;
	unsigned int i;
	unsigned int j;

	dbg_in_args("%p, %d", qdiscs, idx);

	qdisc = pon_net_qdiscs_get(qdiscs, idx);
	if (!qdisc) {
		dbg_out();
		return;
	}

	for (i = 0; i < ARRAY_SIZE(qdisc->classes); ++i) {
		struct qdisc_class *class = &qdisc->classes[i];
		struct pon_net_qdisc *child;

		if (class->state != CLASS_USED)
			continue;

		if (class->child_idx == CHILD_PRIORITY_QUEUE)
			continue;

		child = pon_net_qdiscs_get(qdiscs, class->child_idx);
		if (!child) {
			dbg_out();
			return;
		}

		if (PON_QDISC_HANDLE_TYPE(child->handle) != PON_QDISC_TS_AUX &&
		    PON_QDISC_HANDLE_TYPE(child->handle) != PON_QDISC_ROOT_AUX)
			continue;

		pon_net_qdisc_type_set(child, &pon_net_drr_type, NULL);

		for (j = 0; j < ARRAY_SIZE(child->classes); ++j) {
			struct qdisc_class *drr_class = &child->classes[j];
			struct pon_net_qdisc *grand_child;

			if (drr_class->state != CLASS_USED)
				continue;

			drr_class->arg = pon_net_calculate_quantum(DRR_WEIGHT);

			if (drr_class->child_idx != CHILD_PRIORITY_QUEUE) {
				/*
				 * Synchronize the quantum value of the child
				 * with the quantum value in the parent child
				 * array
				 */
				grand_child =
				pon_net_qdiscs_get(qdiscs,
						   drr_class->child_idx);
				if (!grand_child)
					continue;

				grand_child->parent_arg = drr_class->arg;
			}
		}
	}

	dbg_out();
}

const struct pon_net_qdisc_type pon_net_prio_type = {
	.create = prio_create,
	.destroy = generic_destroy,
	.after_partition = prio_after_partition
};

static enum pon_adapter_errno
drr_create(struct pon_net_context *ctx, const char *ifname,
	   const struct pon_net_qdisc *qdisc)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\", %p", ctx, ifname, qdisc);

	if (qdisc->handle > 0xFFFF || qdisc->parent > 0xFFFF ||
	    qdisc->parent_queue > 0xFFFF) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	ret = netlink_qdisc_drr_create(ctx->netlink, ifname,
				       (uint16_t)qdisc->handle,
				       (uint16_t)qdisc->parent,
				       (uint16_t)qdisc->parent_queue);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_qdisc_drr_create, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

#define NUM_SUPPORTED_DRR_CLASSES 8
static enum pon_adapter_errno
drr_class_create(struct pon_net_context *ctx, const char *ifname,
		 const struct pon_net_qdisc *qdisc, unsigned int i)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\", %p, %d", ctx, ifname, qdisc, i);

	if (qdisc->classes[i].state != CLASS_USED) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	/* DRR quantum cannot be zero. Linux kernel will return
	   -NLE_INVAL. This means that for those queues that resulting
	   quantum is zero we do not create a drr class */
	if (!qdisc->classes[i].arg) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (i >= NUM_SUPPORTED_DRR_CLASSES) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (qdisc->handle > 0xFFFF || i + 1 > 0xFFFF) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	ret = netlink_class_drr_create(ctx->netlink,
				       ifname,
				       (uint16_t)qdisc->handle,
				       (uint16_t)(i + 1),
				       qdisc->classes[i].arg);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_class_drr_create, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
drr_class_destroy(struct pon_net_context *ctx, const char *ifname,
		  const struct pon_net_qdisc *qdisc, unsigned int i)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\", %p", ctx, ifname, qdisc);

	if (qdisc->classes[i].state != CLASS_USED) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (i >= NUM_SUPPORTED_DRR_CLASSES) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (!qdisc->classes[i].arg) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (qdisc->handle > 0xFFFF || i + 1 > 0xFFFF) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	ret = netlink_class_clear_one(ctx->netlink, ifname,
				      (uint16_t)qdisc->handle,
				      (uint16_t)(i + 1));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_class_clear_all, ret);
		return PON_ADAPTER_SUCCESS;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

#define PARTITION_DRR_WEIGHT 4
static void drr_after_partition(struct pon_net_qdiscs *qdiscs,
				unsigned int idx)
{
	struct pon_net_qdisc *qdisc;
	unsigned int i;

	dbg_in_args("%p, %d", qdiscs, idx);

	qdisc = pon_net_qdiscs_get(qdiscs, idx);
	if (!qdisc) {
		dbg_out();
		return;
	}

	pon_net_qdisc_type_set(qdisc, &pon_net_drr_type, NULL);

	for (i = 0; i < ARRAY_SIZE(qdisc->classes); ++i) {
		struct qdisc_class *class = &qdisc->classes[i];
		struct pon_net_qdisc *grand_child;

		if (class->state != CLASS_USED)
			continue;

		class->arg = pon_net_calculate_quantum(PARTITION_DRR_WEIGHT);

		if (class->child_idx != CHILD_PRIORITY_QUEUE) {
			/*
			 * Synchronize the quantum value of the child
			 * with the quantum value in the parent child
			 * array
			 */
			grand_child =
			    pon_net_qdiscs_get(qdiscs, class->child_idx);
			if (!grand_child)
				continue;

			grand_child->parent_arg = class->arg;
		}
	}

	dbg_out();
}

const struct pon_net_qdisc_type pon_net_drr_type = {
	.create = drr_create,
	.destroy = generic_destroy,
	.class_create = drr_class_create,
	.class_destroy = drr_class_destroy,
	.after_partition = drr_after_partition
};

static enum pon_adapter_errno
red_create(struct pon_net_context *ctx, const char *ifname,
	   const struct pon_net_qdisc *qdisc)
{
	enum pon_adapter_errno ret;
	struct pon_net_red_type_data *data = qdisc->type_data;
	struct netlink_qdisc_red red;

	dbg_in_args("%p, \"%s\", %p", ctx, ifname, qdisc);

	netlink_qdisc_red_defaults(&red);

	if ((int)data->max < 0 || (int)data->min < 0) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	netlink_qdisc_red_p_and_thr_set(&red, (int)data->min, (int)data->max,
					data->probability, (int)data->max,
					0, 0, 0);

	if (qdisc->handle > 0xFFFF || qdisc->parent > 0xFFFF ||
	    qdisc->parent_queue > 0xFFFF) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	ret = netlink_qdisc_red_create(ctx->netlink, ifname,
				       (uint16_t)qdisc->handle,
				       (uint16_t)qdisc->parent,
				       (uint16_t)qdisc->parent_queue,
				       &red);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_qdisc_red_create, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static int red_eq(void *aa, void *bb)
{
	struct pon_net_red_type_data *a = aa;
	struct pon_net_red_type_data *b = bb;

	return a->min == b->min &&
	       a->max == b->max &&
	       a->probability == b->probability;
}

const struct pon_net_qdisc_type pon_net_red_type = {
	.create = red_create,
	.destroy = generic_destroy,
	.data_len = sizeof(struct pon_net_red_type_data),
	.data_eq = red_eq
};

static enum pon_adapter_errno
tbf_create(struct pon_net_context *ctx, const char *ifname,
	   const struct pon_net_qdisc *qdisc)
{
	enum pon_adapter_errno ret;
	struct pon_net_tbf_type_data *data = qdisc->type_data;
	struct netlink_qdisc_tbf tbf;

	dbg_in_args("%p, \"%s\", %p", ctx, ifname, qdisc);

	netlink_qdisc_tbf_defaults(&tbf);

	/* PIR is mandatory.
	 * If CBS and PBS are not set then use default values
	 */
	if (data->pir == 0) {
		dbg_err("Can't create tbf with pir = 0\n");
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	netlink_qdisc_tbf_params_set(&tbf, data->cir, data->pir, data->cbs,
				     data->pbs, 0);

	if (qdisc->handle > 0xFFFF || qdisc->parent > 0xFFFF ||
	    qdisc->parent_queue > 0xFFFF) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	ret = netlink_qdisc_tbf_create(ctx->netlink, ifname,
				       (uint16_t)qdisc->handle,
				       (uint16_t)qdisc->parent,
				       (uint16_t)qdisc->parent_queue,
				       &tbf);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_qdisc_tbf_create, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static int tbf_eq(void *aa, void *bb)
{
	struct pon_net_tbf_type_data *a = aa;
	struct pon_net_tbf_type_data *b = bb;

	return a->cir == b->cir &&
	       a->pir == b->pir &&
	       a->cbs == b->cbs &&
	       a->pbs == b->pbs;
}

const struct pon_net_qdisc_type pon_net_tbf_type = {
	.create = tbf_create,
	.destroy = generic_destroy,
	.data_len = sizeof(struct pon_net_tbf_type_data),
	.data_eq = tbf_eq
};
