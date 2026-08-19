/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file
 * This file contains functions for Multicast GEM Interworking TP ME
 */

#ifndef _PON_NET_MULTICAST_GEM_INTERWORKING_TP_
#define _PON_NET_MULTICAST_GEM_INTERWORKING_TP_

#include <stdint.h>
#include <stdbool.h>
#include <pon_adapter_errno.h>

struct pon_net_context;

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/**
 * Get GEM Port Network CTP ME ID for Multicast GEM Interworking TP
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] me_id		Multicast GEM Interworking TP ME ID
 * \param[out] gem_ctp_me_id	GEM Port Network CTP ME ID
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_multicast_gem_interworking_tp_ctp_get(struct pon_net_context *ctx,
					      uint16_t me_id,
					      uint16_t *gem_ctp_me_id);

/** @} */ /* PON_NET_LIB */
#endif
