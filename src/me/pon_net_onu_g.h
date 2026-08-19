/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef _PON_NET_ONU_G_H
#define _PON_NET_ONU_G_H

#include <stdint.h>

struct pon_net_context;
struct pon_net_vlan_forwarding;

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/**
 * Get Traffic management option set on ONU2-G
 *
 * \param[in] ctx		PON NET context pointer
 *
 * \return returns Traffic management option
 */
uint8_t pon_net_traffic_management_option_get(struct pon_net_context *ctx);

/** @} */ /* PON_NET_LIB */
#endif /* _PON_NET_ONU_G_H */
