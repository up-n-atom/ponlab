/*****************************************************************************
 *
 * Copyright (c) 2020 - 2021 MaxLinear, Inc.
 * Copyright (c) 2019 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file
 * This file holds the definition of the uint32_t pool
 */

#ifndef _PON_NET_POOL_H_
#define _PON_NET_POOL_H_

#include <stdint.h>

struct pon_net_pool;

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/**
 * Create index pool
 *
 * \param[in]  min        Minimum integer value returned from the pool
 * \param[in]  max        Maximum integer value returned from the pool
 *
 * \return returns pool or NULL in case of failure
 */
struct pon_net_pool *pon_net_pool_create(uint32_t min, uint32_t max);

/**
 * Obtains 'count' integers from the pool
 *
 * This will obtain exactly 'count' number of integers from the pool and store
 * them in the 'values' array. The 'values' array must be large enough
 * to store 'count' numbers. The numbers stored in 'values' array are
 * guaranteed to be sorted.
 *
 * \param[in] pool        Pool
 * \param[in] values	  Returned integers
 * \param[in] count	  Number of integers to be obtained
 */
enum pon_adapter_errno pon_net_pool_get_many(struct pon_net_pool *pool,
					     uint32_t *values,
					     unsigned int count);

/**
 * Puts 'count' integers back to the pool
 *
 * The 'values' array does not have to be sorted
 *
 * \param[in] pool        Pool
 * \param[in] values	  Integers to be freed
 * \param[in] count	  Number of integers in the 'values' array
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_pool_put_many(struct pon_net_pool *pool,
					     const uint32_t *values,
					     unsigned int count);

/**
 * Return number of available integers in the pool
 *
 * \param[in] pool        Pool
 *
 * \return returns number of available integers in the pool
 */
unsigned int pon_net_pool_available(const struct pon_net_pool *pool);

/**
 * Clear pool
 *
 * Acts like destroy but does not deallocate the pool
 *
 * \param[in] pool        Pool to clear
 */
void pon_net_pool_clear(struct pon_net_pool *pool);

/**
 * Destroy index pool
 *
 * \param[in] pool        Pool to destroy
 */
void pon_net_pool_destroy(struct pon_net_pool *pool);

/** @} */ /* PON_NET_LIB */

#endif
