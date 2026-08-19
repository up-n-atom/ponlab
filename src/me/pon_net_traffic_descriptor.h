/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef _PON_NET_TRAFFIC_DESCRIPTOR_H
#define _PON_NET_TRAFFIC_DESCRIPTOR_H

#include <stdint.h>

struct pon_net_context;
struct pon_net_vlan_forwarding;

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/**
 * Update police settings for given bridge port.
 *
 * The settings will be extracted from the bridge port TD pointer.
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] me_id		Managed Entity ID
 * \param[in] dir		Direction of the police
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_police_update_bp(struct pon_net_context *ctx,
						uint16_t me_id,
						enum netlink_filter_dir dir);

/**
 * Update police settings for given GEM port.
 *
 * The settings will be extracted from the GEM port TD pointer.
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] me_id		Managed Entity ID
 * \param[in] dir		Direction of the police
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_police_update_gem(struct pon_net_context *ctx,
						 uint16_t me_id,
						 enum netlink_filter_dir dir);

/** @} */ /* PON_NET_LIB */
#endif /* _PON_NET_TRAFFIC_DESCRIPTOR_H */
