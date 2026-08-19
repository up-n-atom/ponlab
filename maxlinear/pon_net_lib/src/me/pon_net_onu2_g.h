/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef _PON_NET_ONU2_G_H
#define _PON_NET_ONU2_G_H

#include <stdint.h>

struct pon_net_context;
struct pon_net_vlan_forwarding;

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/**
 * Get Priority queue scale factor set on ONU2-G
 *
 * \param[in] ctx		PON NET context pointer
 *
 * \return returns Priority Queue scale factor
 */
uint16_t pon_net_priority_queue_scale_factor_get(struct pon_net_context *ctx);

/** @} */ /* PON_NET_LIB */
#endif /* _PON_NET_ONU2_G_H */
