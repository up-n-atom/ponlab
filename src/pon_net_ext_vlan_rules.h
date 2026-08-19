/******************************************************************************
 * Copyright (c) 2021 - 2022 MaxLinear, Inc.
 * Copyright (c) 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 ******************************************************************************/

#ifndef _PON_NET_EXT_VLAN_RULES_H_
#define _PON_NET_EXT_VLAN_RULES_H_

#include <stdint.h>
#include <pon_adapter_errno.h>
#include "omci/me/pon_adapter_ext_vlan.h"

#include "pon_net_tc_filter.h"

/** \addtogroup PON_NET_LIB

   @{
*/

/** \defgroup PON_NET_EXT_VLAN_RULES Extended VLAN rule container functions

    Contains the definitions of Extended VLAN rule container,
    which is used to cache the configured Extended VLAN rules

    Caching the Extended VLAN rules is needed to improve the performance
    of rule creation and deletion. The PON Adapter interface only allows
    to configure all rules at once - not one by one. Deleting and recreating
    all the rules is not optimal, therefore this cache is used to improve the
    performance.

    Each time caller of the PON Adapter interface configures all rules, a new
    instance of the struct pon_net_ext_vlan_rules is created. Then a difference
    between cached structure and the newly created one is computed using
    pon_net_ext_vlan_rules_pair_walk() and only the difference in Extended VLAN
    configuration is applied.

   @{
*/

/** Ext. Vlan rule */
struct pon_net_ext_vlan_rule {
	/** Ext. Vlan rule */
	struct pon_adapter_ext_vlan_filter filter;
	/** 1 if the filter is a default filter */
	uint8_t is_def;
	/** Rule major number */
	int major;
	/** Rule minor number */
	int minor;
	/** Information about created tc filters for given rule */
	struct pon_net_tc_filter_array tc_info;
};

/** A collection of Ext. Vlan rules */
struct pon_net_ext_vlan_rules {
	/* Rules array */
	struct pon_net_ext_vlan_rule *rules;
	/* Number entries in the rule array */
	unsigned int num_rules;
};

/**
 * Create rule collection
 *
 * \param[in]     filters      Ext. Vlan rules from the higher layer
 * \param[in]     num_rules    Number of rules in collection
 *
 * \return returns rule collection or NULL in case of an error
 */
struct pon_net_ext_vlan_rules *
pon_net_ext_vlan_rules_create(const struct pon_adapter_ext_vlan_filter *filters,
			      unsigned int num_rules);

/**
 * Create identical deep copy of a rule collection
 *
 * \param[in]     rules    Rule collection to destroy
 *
 * \return returns rule collection or NULL in case of an error
 */
struct pon_net_ext_vlan_rules *
pon_net_ext_vlan_rules_clone(const struct pon_net_ext_vlan_rules *rules);

/**
 * Destroy rule collection
 *
 * \param[in]     rules    Rule collection to destroy
 */
void pon_net_ext_vlan_rules_destroy(struct pon_net_ext_vlan_rules *rules);

/**
 * Call fn for pairs of the same rules.
 *
 * Rules are considered to be the same if their filtering part is equal
 *
 * If a rule is present in 'new_rules' but not in 'old_rules', 'fn' will
 * be called with 'old_rule' argument set to NULL
 * If a rule is present in 'old_rules' but not in 'new_rules', 'fn' will
 * be called with 'new_rule' argument set to NULL
 * If a rule is present in both sets, 'fn' will be called with 'old_rule'
 * from the 'old_rules' rule set, and 'new_rule' from the 'new_rules' rule set
 *
 * \param[in]     old_rules  Rule collection
 * \param[in]     new_rules  Rule collection
 * \param[in]     fn         Callback function
 * \param[out]    arg        Argument to callback function
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_ext_vlan_rules_pair_walk(struct pon_net_ext_vlan_rules *old_rules,
				 struct pon_net_ext_vlan_rules *new_rules,
				 enum pon_adapter_errno
				 (*fn)(struct pon_net_ext_vlan_rule *old_rule,
				       struct pon_net_ext_vlan_rule *new_rule,
				       void *arg),
				 void *arg);
/**
 * Remove a rule from the collection
 *
 * \param[in]     rules  Rule collection
 * \param[out]    filter Rule contain this filter is removed
 */
void pon_net_ext_vlan_rules_remove(struct pon_net_ext_vlan_rules *rules,
			const struct pon_adapter_ext_vlan_filter *filter);

/**
 * Remove a all rules from the collection
 *
 * \param[in]     rules  Rule collection
 */
void pon_net_ext_vlan_rules_clear(struct pon_net_ext_vlan_rules *rules);

/**
 * Check if 2 Ext. Vlan rules are equal
 *
 * \param[in]     a Ext. Vlan rule
 * \param[in]     b Ext. Vlan rule
 *
 * \return 1 if rules are equal, 0 if they are not
 */
int pon_net_ext_vlan_filter_eq(const struct pon_adapter_ext_vlan_filter *a,
			       const struct pon_adapter_ext_vlan_filter *b);

/**
 * Swap two sets of rules
 *
 * \param[in]     a   First set of rules
 * \param[in]     b   Second set of rules
 */
void pon_net_ext_vlan_rules_swap(struct pon_net_ext_vlan_rules *a,
				 struct pon_net_ext_vlan_rules *b);

/**
 * Finds rule with contains given filter
 *
 * \param[in] filter         Filter to find
 * \param[in] rules          Rules as a base
 *
 * \return rule with contain given filter
 */
struct pon_net_ext_vlan_rule *pon_net_ext_vlan_rules_find(
	const struct pon_net_ext_vlan_rules *rules,
	const struct pon_adapter_ext_vlan_filter *filter);

/**
 * Copy original rule content into new copy
 *
 * \param[in] copy          Copy rule
 * \param[in] orig          Original rule
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_ext_vlan_rule_copy(struct pon_net_ext_vlan_rule *copy,
			   struct pon_net_ext_vlan_rule *orig);

/** @} */

/** @} */

#endif
