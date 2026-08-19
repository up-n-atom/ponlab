/*****************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "pon_net_ext_vlan_rules.h"
#include "pon_net_ext_vlan_filter_def.h"
#include "pon_net_tc_filter.h"
#include "pon_net_debug.h"
#include "pon_net_common.h"

int pon_net_ext_vlan_filter_eq(const struct pon_adapter_ext_vlan_filter *a,
			       const struct pon_adapter_ext_vlan_filter *b)
{
	int ret;

	dbg_in_args("%p, %p", a, b);

	ret = a->filter_outer_priority == b->filter_outer_priority &&
	      a->filter_outer_vid == b->filter_outer_vid &&
	      a->filter_outer_tpid_de == b->filter_outer_tpid_de &&
	      a->filter_inner_priority == b->filter_inner_priority &&
	      a->filter_inner_vid == b->filter_inner_vid &&
	      a->filter_inner_tpid_de == b->filter_inner_tpid_de &&
	      a->filter_ethertype == b->filter_ethertype &&
	      a->treatment_tags_to_remove == b->treatment_tags_to_remove &&
	      a->treatment_outer_priority == b->treatment_outer_priority &&
	      a->treatment_outer_vid == b->treatment_outer_vid &&
	      a->treatment_outer_tpid_de == b->treatment_outer_tpid_de &&
	      a->treatment_inner_priority == b->treatment_inner_priority &&
	      a->treatment_inner_vid == b->treatment_inner_vid &&
	      a->treatment_inner_tpid_de == b->treatment_inner_tpid_de;

	dbg_out_ret("%d", ret);
	return ret;
}

/* It is needed for sorting and comparing the filters */
static int filter_cmp(const struct pon_adapter_ext_vlan_filter *a,
		      const struct pon_adapter_ext_vlan_filter *b)
{
	int x[] = {
		a->filter_outer_priority, a->filter_outer_vid,
		a->filter_outer_tpid_de, a->filter_inner_priority,
		a->filter_inner_vid, a->filter_inner_tpid_de,
		a->filter_ethertype,
	};
	int y[] = {
		b->filter_outer_priority, b->filter_outer_vid,
		b->filter_outer_tpid_de, b->filter_inner_priority,
		b->filter_inner_vid, b->filter_inner_tpid_de,
		b->filter_ethertype,
	};
	unsigned int i = 0;
	int ret = 0;

	dbg_in_args("%p, %p", a, b);

	for (i = 0; i < ARRAY_SIZE(x); ++i) {
		if (x[i] != y[i]) {
			ret = x[i] < y[i] ? -1 : 1;
			break;
		}
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* Adapter for qsort */
static int qsort_filter_cmp(const void *a, const void *b)
{
	const struct pon_adapter_ext_vlan_filter *const *fa = a;
	const struct pon_adapter_ext_vlan_filter *const *fb = b;

	return filter_cmp(*fa, *fb);
}

/* Adapter for search filter */
static int rules_search_filter_cmp(const void *a, const void *b)
{
	const struct pon_adapter_ext_vlan_filter *fa = a;
	const struct pon_net_ext_vlan_rule *fb = b;

	return filter_cmp(fa, &fb->filter);
}

struct pon_net_ext_vlan_rule *pon_net_ext_vlan_rules_find(
	const struct pon_net_ext_vlan_rules *rules,
	const struct pon_adapter_ext_vlan_filter *filter)
{
	return bsearch(filter,
		      rules->rules,
		      rules->num_rules,
		      sizeof(rules->rules[0]),
		      rules_search_filter_cmp);
}

static struct pon_net_ext_vlan_rules
*rules_create(unsigned int num_rules)
{
	struct pon_net_ext_vlan_rules *rules;

	dbg_in_args("%u", num_rules);

	rules = malloc(sizeof(*rules));
	if (!rules) {
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	rules->rules = NULL;

	if (num_rules) {
		rules->rules = calloc(num_rules, sizeof(*rules->rules));
		if (!rules->rules) {
			free(rules);
			dbg_out_ret("%p", NULL);
			return NULL;
		}
	}

	rules->num_rules = num_rules;

	dbg_out_ret("%p", rules);
	return rules;
}

struct pon_net_ext_vlan_rules *
pon_net_ext_vlan_rules_create(const struct pon_adapter_ext_vlan_filter *filters,
			      unsigned int num_rules)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_ext_vlan_rules *rules = NULL;
	const struct pon_adapter_ext_vlan_filter **sorted = NULL;
	unsigned int i = 0;

	dbg_in_args("%p, %u", filters, num_rules);

	rules = rules_create(num_rules);
	if (!rules) {
		dbg_err_fn(rules_create);
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	if (!num_rules) {
		dbg_out_ret("%p", rules);
		return rules;
	}

	sorted = calloc(num_rules, sizeof(*sorted));
	if (!sorted) {
		dbg_err_fn(calloc);
		pon_net_ext_vlan_rules_destroy(rules);
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	for (i = 0; i < num_rules; ++i)
		sorted[i] = &filters[i];

	qsort(sorted, num_rules, sizeof(*sorted), qsort_filter_cmp);

	for (i = 0; i < num_rules; ++i) {
		ret = pon_net_get_rule_by_filter_us(sorted[i],
						    &rules->rules[i].major,
						    &rules->rules[i].minor,
						    &rules->rules[i].is_def);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(pon_net_get_rule_by_filter_us, ret);
			free(sorted);
			pon_net_ext_vlan_rules_destroy(rules);
			dbg_out_ret("%p", NULL);
			return NULL;
		}

		rules->rules[i].filter = *sorted[i];
	}

	free(sorted);
	dbg_out_ret("%p", rules);
	return rules;
}

struct pon_net_ext_vlan_rules *
pon_net_ext_vlan_rules_clone(const struct pon_net_ext_vlan_rules *rules)
{
	unsigned int i;
	struct pon_net_ext_vlan_rules *clone;
	enum pon_adapter_errno ret;

	dbg_in_args("%p", rules);

	clone = rules_create(rules->num_rules);
	if (!clone) {
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	for (i = 0; i < rules->num_rules; ++i) {
		ret = pon_net_ext_vlan_rule_copy(&clone->rules[i],
						 &rules->rules[i]);
		if (ret != PON_ADAPTER_SUCCESS) {
			pon_net_ext_vlan_rules_destroy(clone);
			dbg_out_ret("%p", NULL);
			return NULL;
		}
	}

	dbg_out_ret("%p", clone);
	return clone;
}

void
pon_net_ext_vlan_rules_destroy(struct pon_net_ext_vlan_rules *rules)
{
	dbg_in_args("%p", rules);

	if (rules)
		pon_net_ext_vlan_rules_clear(rules);
	free(rules);

	dbg_out();
}

void pon_net_ext_vlan_rules_swap(struct pon_net_ext_vlan_rules *a,
				 struct pon_net_ext_vlan_rules *b)
{
	struct pon_net_ext_vlan_rules tmp = *a;

	dbg_in_args("%p, %p", a, b);

	*a = *b;
	*b = tmp;

	dbg_out();
}

/* Wrapper for filter_cmp(), used for pon_net_pair_walk_args */
static int __filter_cmp(const void *a, const void *b, void *arg)
{
	const struct pon_net_ext_vlan_rule *old_rule = a;
	const struct pon_net_ext_vlan_rule *new_rule = b;

	(void)arg;

	return filter_cmp(&old_rule->filter, &new_rule->filter);
}

/* Arguments to user callback wrapper */
struct ext_vlan_pair_walk_args {
	/* Function pointer provided in pon_net_ext_vlan_rules_pair_walk */
	enum pon_adapter_errno (*fn)(struct pon_net_ext_vlan_rule *old_rule,
				     struct pon_net_ext_vlan_rule *new_rule,
				     void *arg);
	/* Argument provided in pon_net_ext_vlan_rules_pair_walk */
	void *arg;
};

/*
 * Wrapper for user callback provided in pon_net_ext_vlan_rules_pair_walk
 * args
 */
static enum pon_adapter_errno ext_vlan_pair_walk_fn(void *item_a, void *item_b,
						    void *arg)
{
	struct pon_net_ext_vlan_rule *old_rule = item_a;
	struct pon_net_ext_vlan_rule *new_rule = item_b;
	struct ext_vlan_pair_walk_args *args = arg;

	return args->fn(old_rule, new_rule, args->arg);
}

enum pon_adapter_errno
pon_net_ext_vlan_rules_pair_walk(struct pon_net_ext_vlan_rules *old_rules,
				 struct pon_net_ext_vlan_rules *new_rules,
				 enum pon_adapter_errno
				 (*fn)(struct pon_net_ext_vlan_rule *old_rule,
				       struct pon_net_ext_vlan_rule *new_rule,
				       void *arg),
				 void *arg)
{
	struct ext_vlan_pair_walk_args ext_vlan_args = {
		.fn = fn,
		.arg = arg,
	};
	struct pon_net_pair_walk_args args = {
		.first_array = old_rules->rules,
		.first_array_len = old_rules->num_rules,
		.second_array = new_rules->rules,
		.second_array_len = new_rules->num_rules,
		.cmp = __filter_cmp,
		.fn = ext_vlan_pair_walk_fn,
		.arg = &ext_vlan_args,
		.elem_size = sizeof(old_rules->rules[0]),
	};
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %p, %p", old_rules, new_rules, fn, arg);

	ret = pon_net_pair_walk(&args);

	dbg_out_ret("%d", ret);
	return ret;
}

void pon_net_ext_vlan_rules_remove(struct pon_net_ext_vlan_rules *rules,
			const struct pon_adapter_ext_vlan_filter *filter)
{
	unsigned int i = 0;
	struct pon_net_ext_vlan_rule *res;

	dbg_in_args("%p", rules);
	res = pon_net_ext_vlan_rules_find(rules,
					  filter);

	if (res != NULL) {
		pon_net_tc_filter_array_clear(&res->tc_info);
		/*
		 * res is a pointer to element in rules->rules array, result of
		 * this: (res - rules->rules) is a index of res in array.
		 */
		for (i = (unsigned int)(res - rules->rules);
			  i < rules->num_rules - 1; ++i)
			rules->rules[i] = rules->rules[i + 1];

		rules->num_rules--;
	}
	dbg_out();
}

void pon_net_ext_vlan_rules_clear(struct pon_net_ext_vlan_rules *rules)
{
	unsigned int i = 0;

	dbg_in_args("%p", rules);

	for (i = 0; i < rules->num_rules; ++i)
		pon_net_tc_filter_array_clear(&rules->rules[i].tc_info);

	free(rules->rules);
	rules->rules = NULL;
	rules->num_rules = 0;

	dbg_out();
}

enum pon_adapter_errno
pon_net_ext_vlan_rule_copy(struct pon_net_ext_vlan_rule *copy,
			   struct pon_net_ext_vlan_rule *orig)
{
	copy->filter = orig->filter;
	copy->is_def = orig->is_def;
	copy->major = orig->major;
	copy->minor = orig->minor;
	return pon_net_tc_filter_array_copy(&copy->tc_info, &orig->tc_info);
}
