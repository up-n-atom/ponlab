/*****************************************************************************
 *
 * Copyright (c) 2022 - 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef _PON_NET_MAC_BRIDGE_PORT_CONFIG_DATA_H
#define _PON_NET_MAC_BRIDGE_PORT_CONFIG_DATA_H

#include <stdint.h>

struct pon_net_context;
struct pon_net_vlan_forwarding;

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/** Get the termination point type for an association type
 *
 * \param[in]  association_type Association type
 * \param[out] tp_type          Termination Point Type
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_association_type_to_tp_type(uint16_t association_type,
				    uint8_t *tp_type);

/** Get the termination point for a bridge interface index
 *
 * \param[in]  ctx		PON NET context pointer
 * \param[in]  tp_type		Termination point type
 * \param[in]  tp_ptr		Termination point pointer
 * \param[out] me_id		MAC Bridge Port Configuration Data ME ID
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_bp_me_id_get(struct pon_net_context *ctx,
					    uint8_t tp_type, uint16_t tp_ptr,
					    uint16_t *me_id);

/** Clean up VLAN forwarding from selected bridge ports or all bridge ports
 *
 * If 'me_ids' is NULL, then the cleanup will be executed for all bridge ports.
 * Otherwise only for those bridge ports, whose ME IDs are in the array.
 *
 * \param[in]  ctx		PON NET context pointer
 * \param[in]  me_ids		MAC Bridge Port Configuration Data ME IDs
 * \param[out] me_ids_len	Number of ME IDs in the 'me_ids' array
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_bp_vlan_forwarding_cleanup(struct pon_net_context *ctx,
				   uint16_t *me_ids, unsigned int me_ids_len);

/** Update VLAN aware forwarding for selected bridge ports or all bridge ports
 *
 * If 'me_ids' is NULL, then the update will be executed for all bridge ports.
 * Otherwise only for those bridge ports, whose ME IDs are in the array.
 *
 * \param[in]  ctx		PON NET context pointer
 * \param[in]  me_ids		MAC Bridge Port Configuration Data ME IDs
 * \param[out] me_ids_len	Number of ME IDs in the 'me_ids' array
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_bp_vlan_forwarding_update(struct pon_net_context *ctx, uint16_t *me_ids,
				  unsigned int me_ids_len);

/** Read all bridge ports connected to a given bridge
 *
 * If 'me_ids' is NULL or 'me_ids_capacity' is 0, then the function will return
 * number of bridge ports in the given bridge. If the returned number
 * is larger than 'me_ids_capacity', it means that the 'me_ids' array is not
 * big enough.
 *
 * \param[in]  ctx		PON NET context pointer
 * \param[in]  br_me_id		MAC Bridge Service Profile ME ID
 * \param[in]  me_ids		MAC Bridge Port Configuration Data ME IDs
 * \param[out] me_ids_capacity	Number of ME IDs in the 'me_ids' array
 *
 * \return returns number of bridge ports
 */
unsigned int pon_net_bp_all_get(struct pon_net_context *ctx, uint16_t br_me_id,
				uint16_t *me_ids, unsigned int me_ids_capacity);

/** Get the direction of a given GEM port ()
 *
 * \param[in]  ctx     PON Network Library context pointer
 * \param[in]  tp_ptr  Termination Point Pointer
 *
 * \return returns value as follows:
 * - PA_GEMPORTNETCTP_DIR_DOWNSTREAM
 * - PA_GEMPORTNETCTP_DIR_UPSTREAM
 * - PA_GEMPORTNETCTP_DIR_BOTH
 * - 0: in case of an error
 */
uint8_t pon_net_gem_dir_get(struct pon_net_context *ctx, uint16_t tp_ptr);

/** Get the termination point for a bridge interface index
 *
 * \param[in]  ctx     PON Network Library context pointer
 * \param[in]  me_id   MAC Bridge Port Configuration Managed Entity ID
 * \param[out] tp_type Termination Point Type
 * \param[out] tp_ptr  Termination Point Pointer
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_tp_get(struct pon_net_context *ctx,
				      const uint16_t me_id,
				      uint8_t *tp_type,
				      uint16_t *tp_ptr);

/** Get the interface name of the device that is connected to the bridge
 *
 * \param[in]  ctx     PON Network Library context pointer
 * \param[in]  me_id   MAC Bridge Port Configuration Managed Entity ID
 * \param[out] ifname  Network interface name
 * \param[in]  size    Capacity of the 'ifname' array
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_bp_ifname_get(struct pon_net_context *ctx,
					     uint16_t me_id,
					     char *ifname,
					     size_t size);

enum pon_adapter_errno
pon_net_bp_reconnect_multicast_gems(struct pon_net_context *ctx);

/** @} */ /* PON_NET_LIB */
#endif /* _PON_NET_MAC_BRIDGE_PORT_CONFIG_DATA_H */
