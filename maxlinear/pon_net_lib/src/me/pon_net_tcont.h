/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef _PON_NET_TCOUNT_H
#define _PON_NET_TCOUNT_H

#include <stdint.h>

struct pon_net_context;
struct pon_net_vlan_forwarding;

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/**
 * Destroys qdiscs tree from the T-CONT
 *
 * \param[in] ctx PON NET context pointer
 * \param[in] me_ids T-CONT Managed Entity IDs
 * \param[in] len Length of the array
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_tcont_qdiscs_cleanup(struct pon_net_context *ctx,
						    const uint16_t *me_ids,
						    unsigned int len);

/**
 * Function sets the T-CONT into mixed mode
 *
 * \param[in] ctx PON NET context pointer
 * \param[in] me_ids T-CONT Managed Entity IDs
 * \param[in] len Length of the array
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_tcont_qdiscs_mixed_mode_set(struct pon_net_context *ctx,
				    const uint16_t *me_ids, unsigned int len);

/**
 * Updates the qdisc tree on the T-CONT
 *
 * \param[in] ctx PON NET context pointer
 * \param[in] me_ids T-CONT Managed Entity IDs
 * \param[in] len Length of the array
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_tcont_qdiscs_update(struct pon_net_context *ctx,
						   const uint16_t *me_ids,
						   unsigned int len);

/** @} */ /* PON_NET_LIB */
#endif /* _PON_NET_TCOUNT_H */
