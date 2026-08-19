/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef _PON_NET_VLAN_TAGGING_FILTER_DATA_H
#define _PON_NET_VLAN_TAGGING_FILTER_DATA_H

#include <stdint.h>

struct pon_net_context;
struct pon_net_vlan_forwarding;

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/** Update VLAN Tagging Filter Data filters
 *
 * \param[in] ctx                PON Network Library context pointer
 * \param[in] vlan_tagging_me_id VLAN Tagging Filter Data Managed Entity Id
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_vlan_filtering_update(struct pon_net_context *ctx,
			      uint16_t vlan_tagging_me_id);

/** Clean up VLAN filtering
 *
 * \param[in] ctx                PON Network Library context pointer
 * \param[in] me_id              VLAN Tagging Filter Data Managed Entity Id
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_vlan_filtering_cleanup(struct pon_net_context *ctx, uint16_t me_id);

/** Get VLAN forwarding configuration from VLAN Tagging Filter Data ME
 *
 * \param[in]  ctx	PON NET context pointer
 * \param[in]  me_id	VLAN Taggin Filter Data ME
 * \param[out] fwd	VLAN Forwarding Configuration
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_vlan_tagging_filter_data_vlan_forwarding_get(
					   struct pon_net_context *ctx,
					   uint16_t me_id,
					   struct pon_net_vlan_forwarding *fwd);

/** @} */ /* PON_NET_LIB */
#endif /* _PON_NET_VLAN_TAGGING_FILTER_DATA_H */
