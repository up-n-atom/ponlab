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
 * VLAN Aware bridging configuration
 */

#ifndef _PON_NET_VLAN_AWARE_BRIDGING_H_
#define _PON_NET_VLAN_AWARE_BRIDGING_H_

#include <stdint.h>
#include <stdbool.h>

struct pon_net_vlan_aware_bridging;
struct pon_net_context;
struct pon_net_me_list;

/**
 * Create VLAN aware bridging management object
 *
 * \return instance of VLAN aware bridging management object
 */
struct pon_net_vlan_aware_bridging *pon_net_vlan_aware_bridging_create(void);

/**
 * Update VLAN aware bridging
 *
 * \param[in]   vab             VLAN aware bridging management object
 * \param[in]   me_list         List of Managed Entities
 * \param[in]	me_id		MAC Bridge Port Configuration Data ME ID
 * \param[in]	tci_list	VLAN ids list
 * \param[in]	tci_list_len	VLAN ids count
 * \param[in]	ctx             Netlink context for netlink requests
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_vlan_aware_bridging_update(struct pon_net_vlan_aware_bridging *vab,
				   struct pon_net_me_list *me_list,
				   uint16_t me_id,
				   const uint16_t *tci_list,
				   uint8_t tci_list_len,
				   struct pon_net_context *ctx);

/**
 * Update VLAN aware bridging
 *
 * \param[in]   vab             VLAN aware bridging management object
 * \param[in]	me_id		MAC Bridge Port Configuration Data ME ID
 * \param[in]	ctx         Netlink context for netlink requests
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_vlan_aware_bridging_clear(struct pon_net_vlan_aware_bridging *vab,
				  uint16_t me_id,
				  struct pon_net_context *ctx);

/**
 * Destroy VLAN aware bridging management object
 *
 * \param[in] vab      VLAN aware bridging management object
 * \param[in] ctx      Netlink context for netlink requests
 */
void
pon_net_vlan_aware_bridging_destroy(struct pon_net_vlan_aware_bridging *vab,
				    struct pon_net_context *ctx);

#endif
