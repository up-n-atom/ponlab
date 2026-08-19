/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef _PON_NET_MAC_BRIDGE_PORT_FILTER_PREASSIGN_TABLE_H
#define _PON_NET_MAC_BRIDGE_PORT_FILTER_PREASSIGN_TABLE_H

#include <stdint.h>

struct pon_net_context;

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/** Update the pre-assign table
 *
 * This function will apply the MAC Bridge Port Filter Pre-assign table
 * to the system
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in]  me_ids		MAC Bridge Port Filter Pre-assign table ME IDs
 * \param[out] me_ids_len	Number of ME IDs in the 'me_ids' array
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_mac_bridge_port_filter_preassign_table_update(
	struct pon_net_context *ctx, uint16_t *me_ids,
	unsigned int me_ids_len);

/** Return true if MAC Bridge Port Filter Pre-assign table of given
 * 'me_id' is configured.
 *
 * This will only return true if TC filters have been created.
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] me_id		MAC Bridge Port Preassign Table ME ID
 *
 */
bool pon_net_mac_bridge_port_filter_preassign_table_configured(
	struct pon_net_context *ctx, uint16_t me_id);

/** @} */ /* PON_NET_LIB */
#endif /* _PON_NET_MAC_BRIDGE_PORT_FILTER_PREASSIGN_TABLE_H */
