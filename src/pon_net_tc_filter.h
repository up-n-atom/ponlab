/******************************************************************************
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 ******************************************************************************/

#ifndef _PON_NET_TC_FILTER_H_
#define _PON_NET_TC_FILTER_H_

#include <stdint.h>
#include <net/if.h>

/** \addtogroup PON_NET_LIB

   @{
*/

/** \defgroup PON_NET_TC_FILTER TC filter storage functions

    Contains the definitions of PON Net TC filter structure
    and a container for items of this structure

    To create TC filters a \ref netlink_filter structure is used. This structure
    is quite big and contains many fields. To delete a TC filter we only
    need a handful of fields like e.g. proto or prio. In order to achieve
    this the \ref pon_net_tc_filter structure is used.

    This structure is used by the PON Net Lib library to cache information
    about created TC filters, so that they can later be deleted.

   @{
*/

/** Information about created tc filter used for filters deletion */
struct pon_net_tc_filter {
	/** TC filter protocol */
	uint16_t proto;
	/** TC priority */
	uint16_t prio;
	/** TC handle */
	uint32_t handle;
	/** TC filter vlan id */
	int vlan_id;
	/** Traffic direction to which the filter/action shall be applied. */
	uint8_t dir;
	/** Interface on which the filter was created */
	char ifname[IF_NAMESIZE];
	/** Pool ID to which the handle should be returned */
	unsigned int handle_pool_id;
	/** Pool ID to which the priority should be returned */
	unsigned int prio_pool_id;
};

/** Information about created TC filters */
struct pon_net_tc_filter_array {
	/** Array of filters */
	struct pon_net_tc_filter *filters;
	/** Number of elements in the array */
	unsigned int count;
	/** Capacity of the array */
	unsigned int capacity;
};

/**
 * Initialize TC filter array
 *
 * \param[in]     array    TC filter array
 */
void pon_net_tc_filter_array_init(struct pon_net_tc_filter_array *array);

/**
 * Remove all entries from TC filter array
 *
 * \param[in]     array TC filter array
 */
void pon_net_tc_filter_array_clear(struct pon_net_tc_filter_array *array);

/**
 * Delete TC filter array
 *
 * \param[in]     array TC filter array
 */
void pon_net_tc_filter_array_exit(struct pon_net_tc_filter_array *array);

/**
 * Add TC filter to array
 *
 * \param[in]     array      TC filter array
 * \param[in]     tc_filter  TC filter
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_tc_filter_array_add(struct pon_net_tc_filter_array *array,
			    const struct pon_net_tc_filter *tc_filter);

/**
 * Removes last element from TC filter array
 *
 * \param[in]     array      TC filter array
 */
void pon_net_tc_filter_array_pop(struct pon_net_tc_filter_array *array);

/**
 * Copies TC filter array
 *
 * \param[out]    dst  Destination
 * \param[in]     src  Source
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_tc_filter_array_copy(struct pon_net_tc_filter_array *dst,
			     const struct pon_net_tc_filter_array *src);

/** @} */

/** @} */

#endif
