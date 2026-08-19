/*****************************************************************************
 *
 * Copyright (c) 2021 - 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/**
 * \file
 * Network device "database". Allows to cache information related to network
 * devices.
 *
 * ## Overview
 *
 * We have to manage TC filter handles and priorities per network device,
 * therefore we need some "storage" per network device, to know which handle
 * and priority numbers we use and which are still available - this is the
 * purpose of struct \ref pon_net_dev_db.
 *
 * The code adds a device to the database when it is created and removes it
 * when it is deleted using \ref pon_net_dev_db_add() and
 * \ref pon_net_dev_db_del() functions.
 *
 * Every device type in the database has its own configuration of handle and
 * priority allocation. This configuration is represented by
 * struct \ref pon_net_dev_def. Definitions for given interface types are
 * available in pon_net_dev_defs.c.
 *
 * To generate handle or priority \ref pon_net_dev_db_gen() is used and
 * \ref pon_net_dev_db_put() is used to free it. Freed handles and priorities
 * go back to the pool and can be used again.
 *
 * Some handle ranges are not managed dynamically
 * (by \ref pon_net_dev_db_gen() and \ref pon_net_dev_db_put()), but rather
 * they are preallocated per device. To use these handles
 * \ref pon_net_dev_db_map() is used, which maps consecutive numbers to a
 * number configured in pon_net_dev_devs.c.
 *
 * Such handle and priority ranges are defined using `.is_static = true` option
 * and they do not need to be freed.
 *
 * ### Examples
 *
 * To generate a handle on ingress qdisc, call:
 *
 *     uint32_t handle;
 *     pon_net_dev_db_gen(ctx->db, ifname, PON_NET_HANDLE_FILTER_INGRESS,
 *                        &handle, 1);
 *
 * Such handle must be freed:
 *
 *     pon_net_dev_db_put(ctx->db, ifname, PON_NET_HANDLE_FILTER_INGRESS,
 *                        &handle, 1);
 *
 * Another example - generate priority for Extended VLAN:
 *
 *     uint32_t prio;
 *     pon_net_dev_db_gen(ctx->db, ifname, PON_NET_PRIO_EXT_VLAN_US, &prio, 1);
 *
 * ## Generating multiple priorities
 *
 * Another use case is when you need to generate a range of priorities, because
 * for example some TC filters must be placed in order (for example:
 * VLAN Filtering, Multicast VLAN Operations).
 * Then you need to grab multiple priorities from the pool:
 *
 *     uint32_t prio[16];
 *     pon_net_dev_db_gen(ctx->db, ifname, PON_NET_PRIO_VLAN_FILTERING, prio,
 *                        ARRAY_SIZE(prio));
 *
 * The returned priorities in the `prio` will be sorted.
 *
 * Freeing is analogous
 *
 *     pon_net_dev_db_put(ctx->db, ifname, PON_NET_PRIO_VLAN_FILTERING, prio,
 *                        ARRAY_SIZE(prio));
 *
 * ## Using a preallocated handle or priority
 *
 * Some handle and priority ranges are "preallocated" on a device.
 * These are the ones defined as `.is_static = true`. Such handles and
 * priorities are used in places where we create a fixed number of possible
 * filters per netdevice, and ownership of such filters is taken by the device
 * and not the ME implementation. (for example: color marking, police,
 * IP Host vlan trap filters)
 *
 * ### Examples
 *
 * Get first prio from the preallocated range:
 *
 *     uint32_t prio1;
 *     pon_net_dev_db_map(ctx->db, ifname, PON_NET_PRIO_LCT_INGRESS, 0, &prio1);
 *
 * Get second prio from the preallocated range:
 *
 *     uint32_t prio2;
 *     pon_net_dev_db_map(ctx->db, ifname, PON_NET_PRIO_LCT_INGRESS, 1, &prio2);
 *
 * Get third prio from the preallocated range:
 *
 *      uint32_t prio2;
 *      pon_net_dev_db_map(ctx->db, ifname, PON_NET_PRIO_LCT_INGRESS, 2,
 *                         &prio3);
 *
 */

#ifndef _PON_NET_DEV_DB_H_
#define _PON_NET_DEV_DB_H_

#include <stdint.h>
#include <stdbool.h>
#include "pon_net_common.h"

struct pon_net_dev_db;
struct pon_net_pool;

/** Structure used to create pool */
struct pon_net_dev_pool_cfg {
	/* Index of the pool in the handle_pools array */
	unsigned int id;
	/* Minimum value returned by pool */
	uint32_t min;
	/* Maximum value returned by pool */
	uint32_t max;
	/*
	 * If true, then the range is not dynamically managed by pon_net_pool.
	 * The function pon_net_dev_db_gen() will not work. Such handles are
	 * considered to be "preallocated" per device and should be managed
	 * using pon_net_dev_db_map().
	 */
	bool is_static;
};

/** Arguments used by pon_net_dev_db to create pon_net_dev_info */
struct pon_net_dev_def {
	/* Information on how to create handle pools */
	const struct pon_net_dev_pool_cfg *pool_cfgs;
	/* Number of handle pools */
	unsigned int pool_cfgs_count;
};

/** Network device information stored in pon_net_dev_db */
struct pon_net_dev_info {
	/* Name of the network device */
	char ifname[IF_NAMESIZE];
	/* Array of handle pools */
	struct pon_net_pool **handle_pools;
	/* Number of handle pools */
	unsigned int handle_pools_count;
	/* PON net dev def used to create this info */
	const struct pon_net_dev_def *def;
};

/**
 * Creates new network device database
 *
 * \return instance of network device database
 */
struct pon_net_dev_db *pon_net_dev_db_create(void);

/**
 * Creates new information about network device
 *
 * Note: 'def' must outlive 'db' (it is best that it is static)
 *
 * \param[in] db        Network device database instance
 * \param[in] ifname    Network device name
 * \param[in] def       Device type dependent details
 *
 * \return instance of network device database
 */
enum pon_adapter_errno pon_net_dev_db_add(struct pon_net_dev_db *db,
					  const char *ifname,
					  const struct pon_net_dev_def *def);

/**
 * Retrieve information about network device from the database
 *
 * \param[in] db        Network device database instance
 * \param[in] ifname    Network device name
 *
 * \return instance of network device database
 */
struct pon_net_dev_info *pon_net_dev_db_get(struct pon_net_dev_db *db,
					    const char *ifname);

/**
 * Delete information about network device from the database
 *
 * \param[in] db        Network device database instance
 * \param[in] ifname    Network device name
 */
void pon_net_dev_db_del(struct pon_net_dev_db *db, const char *ifname);

/**
 * Deletes network device database
 *
 * \param[in] db        Network device database instance
 */
void pon_net_dev_db_destroy(struct pon_net_dev_db *db);

/**
 * Allocate handle from the pool
 *
 * \param[in] db        Network device database instance
 * \param[in] ifname    Network device name
 * \param[in] pool_id   Pool identifier
 * \param[out] handles  Array of generated handles
 * \param[in] count     Capacity of array of generated handles
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_dev_db_gen(struct pon_net_dev_db *db, const char *ifname,
		   unsigned int pool_id, uint32_t *handles, unsigned int count);

/**
 * Return handles to the pool
 *
 * \param[in] db        Network device database instance
 * \param[in] ifname    Network device name
 * \param[in] pool_id   Pool identifier
 * \param[in] handles   Array of handles to return
 * \param[in] count     Number of handles
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_dev_db_put(struct pon_net_dev_db *db,
					  const char *ifname,
					  unsigned int pool_id,
					  const uint32_t *handles,
					  unsigned int count);

/**
 * Maps key to val, using static pool config
 *
 * The pool_id must be configured as is_static
 *
 * \param[in] db        Network device database instance
 * \param[in] ifname    Network device name
 * \param[in] pool_id   Pool identifier
 * \param[in] key       Key (from 0 to the pool size define by pool_id)
 * \param[out] value    Mapped value
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_dev_db_map(struct pon_net_dev_db *db,
					  const char *ifname,
					  unsigned int pool_id,
					  uint32_t key,
					  uint32_t *value);

/**
 * Iterates over every dev info in dev db
 *
 * The pool_id must be configured as is_static
 *
 * \param[in] db        Network device database instance
 * \param[in] fn	Callback called for each dev
 * \param[in] arg       Argument passed to callback
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
int pon_net_dev_db_foreach(struct pon_net_dev_db *db,
			   int (*fn)(struct pon_net_dev_info *info, void *arg),
			   void *arg);

/** Allocation configuration */
struct pon_net_dev_db_alloc {
	/** Destination - where allocated handles will be written to */
	uint32_t *dst;
	/** Number of requested handles */
	unsigned int count;
	/** Interface name */
	const char *ifname;
	/** Pool ID */
	unsigned int pool_id;
};

/**
 * Perform multiple allocations - all allocations must succeed
 *
 * This function will perform all requested allocations, or it
 * will fail completely - no need to do cleanup if it fails
 *
 * \param[in] db               Network device database instance
 * \param[in] allocations      Allocation configuration
 * \param[in] num_allocations  Number of allocations
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_dev_db_gen_many(struct pon_net_dev_db *db,
			const struct pon_net_dev_db_alloc *allocations,
			unsigned int num_allocations);

/**
 * Free multiple allocations
 *
 * \param[in] db               Network device database instance
 * \param[in] allocations      Allocation configuration
 * \param[in] num_allocations  Number of allocations to free
 */
void
pon_net_dev_db_put_many(struct pon_net_dev_db *db,
			const struct pon_net_dev_db_alloc *allocations,
			unsigned int num_allocations);

/** @} */ /* PON_NET_LIB */

#endif
