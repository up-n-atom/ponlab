/******************************************************************************
 *
 * Copyright (c) 2020 - 2021 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 * This file holds definitions used for OMCI Managed Entity lists.
 */

#ifndef _PON_NET_STATE_H_
#define _PON_NET_STATE_H_

#include <sys/queue.h>
#include <stdint.h>
#include <stdbool.h>

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/** \defgroup PON_NET_ME_LIST PON Network Library Managed Entity List
 * This is a list of the OMCI Managed Entities which are covered by the PON
 * Adapter and PON Network Library implementation.
 *
 * In the PON Network Library, many PON Adapter Managed Entity interfaces are
 * implemented.
 *
 * For the implementation of one interface function sometimes we need data
 * that is only available in a different interface function.
 *
 * By using this list, we can store a lower layer state of a particular
 * Managed Entity.
 *  @{
 */

/* Class ids used for ME list */
#define PON_CLASS_ID_PPTP_ETHERNET_UNI 11
#define PON_CLASS_ID_BRIDGE_PORT_CONFIG 47
#define PON_CLASS_ID_BRIDGE_PORT_FILTER_PREASSIGN_TABLE 79
#define PON_CLASS_ID_VLAN_TAGGING_FILTER_DATA 84
#define PON_CLASS_ID_PMAPPER 130
#define PON_CLASS_ID_IP_HOST 134
#define PON_CLASS_ID_EXT_VLAN_CONF_DATA 171
#define PON_CLASS_ID_ONU_G 256
#define PON_CLASS_ID_ONU2_G 257
#define PON_CLASS_ID_TCONT 262
#define PON_CLASS_ID_ANI_G 263
#define PON_CLASS_ID_GEM_PORT_NET_CTP_DATA 268
#define PON_CLASS_ID_PRIORITY_QUEUE 277
#define PON_CLASS_ID_TRAFFIC_SCHEDULER 278
#define PON_CLASS_ID_TRAFFIC_DESCRIPTOR 280
#define PON_CLASS_ID_MULTICAST_GEM_ITP 281
#define PON_CLASS_ID_MC_SUBSCRIBER_CFG 310
#define PON_CLASS_ID_VEIP 329

/**
 * This represents a lower layer state of a particular Managed Entity.
 */
struct pon_net_me_list_item {
	/** Next member */
	SLIST_ENTRY(pon_net_me_list_item) entries;
	/** Class ID (type) of a Managed Entity */
	uint16_t class_id;
	/** Managed Entity ID of a specific instance of a Managed Entity */
	uint16_t me_id;
	/** Data stored for this Managed Entity */
	void *data;
};

SLIST_HEAD(pon_net_me_list, pon_net_me_list_item);

/**
 * Macro to ease iteration over each Managed Entity from given class
 *
 * Example:
 * \code
 * struct pon_net_me_list_item *item;
 *
 * pon_net_me_list_foreach(&ctx->me_list, PON_CLASS_ID_TCONT, item) {
 *      struct tcont *tcont = item->data;
 *      //...
 * }
 * \endcode
 */
#define pon_net_me_list_foreach_class(head, cls, i) \
	SLIST_FOREACH(i, head, entries) \
		if ((i)->class_id == (cls))

/**
 * Initialize the Managed Entity data list
 *
 * \param[in]  list        List of Managed Entity data
 */
void pon_net_me_list_init(struct pon_net_me_list *list);

/**
 * Write Managed Entity data
 *
 * \param[in]  list        List of Managed Entity data
 * \param[in]  class_id    Managed Entity class ID
 * \param[in]  me_id       Managed Entity ID
 * \param[in]  data        Data to be saved
 * \param[in]  data_size   Data size
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_me_list_write(struct pon_net_me_list *list,
		      uint16_t class_id,
		      uint16_t me_id,
		      const void *data,
		      unsigned int data_size);

/**
 * Read Managed Entity data
 *
 * \param[in]  list        List of Managed Entity data
 * \param[in]  class_id    Manager Entity class ID
 * \param[in]  me_id       Managed Entity ID
 * \param[out] data        Data to be read
 * \param[in]  data_size   Data size
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_me_list_read(struct pon_net_me_list *list,
		     uint16_t class_id,
		     uint16_t me_id,
		     void *data,
		     unsigned int data_size);

/**
 * Return pointer to Managed Entity data
 *
 * \param[in]  list        List of Managed Entity data
 * \param[in]  class_id    Manager Entity class ID
 * \param[in]  me_id       Managed Entity ID
 *
 * \return returns value as follows:
 * - pointer to Managed Entity data
 * - NULL: An error code in case of error.
 */
void *
pon_net_me_list_get_data(struct pon_net_me_list *list,
			 uint16_t class_id,
			 uint16_t me_id);

/**
 * Check if managed entity data is present on a list
 *
 * \param[in]  list        List of Managed Entity data
 * \param[in]  class_id    Manager Entity class ID
 * \param[in]  me_id       Managed Entity ID
 *
 * \return returns value as follows:
 * - true: if present
 * - false: if not present
 */
bool pon_net_me_list_exists(struct pon_net_me_list *list, uint16_t class_id,
			    uint16_t me_id);

/**
 * Remove Managed Entity data
 *
 * \param[in]  list        List of Managed Entity data
 * \param[in]  class_id    Managed Entity class ID
 * \param[in]  me_id       Managed Entity ID
 *
 */
void pon_net_me_list_remove(struct pon_net_me_list *list,
			    uint16_t class_id,
			    uint16_t me_id);

/**
 * Clear the Managed Entity data list by freeing all resources
 *
 * \param[in]  list        List of Managed Entity data
 */
void pon_net_me_list_exit(struct pon_net_me_list *list);

/** @} */ /* PON_NET_ME_LIST */

/** @} */ /* PON_NET_LIB */
#endif
