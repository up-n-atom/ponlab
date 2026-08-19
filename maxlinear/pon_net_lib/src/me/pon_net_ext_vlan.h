/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef _PON_NET_EXT_VLAN_H
#define _PON_NET_EXT_VLAN_H

#include <stdint.h>

struct pon_net_context;
struct pon_net_vlan_forwarding;

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/** Special function to convert DSCP from OMCI format (24 byte array containing
 *  3bit PCP values) to PON Net Lib format (64 DSCP to PCP array).
 *
 * \param[in] src Input OMCI format array
 * \param[in] dst Output DSCP to PCP array
 */
void pon_net_dscp_convert(uint8_t *src, uint8_t *dst);

/** Get Extended VLAN Configuration Data ME ID for given termination point
 *
 * \param[in]  ctx		PON NET context pointer
 * \param[in]  association_type	Extended VLAN Configuration Data association
 *				type
 * \param[in]  associated_ptr	Extended VLAN Configuration Data associated
 *				pointer
 * \param[out] me_id		Extended VLAN Configuration Data ME ID
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_ext_vlan_me_id_get(struct pon_net_context *ctx,
						  uint16_t association_type,
						  uint16_t associated_ptr,
						  uint16_t *me_id);

/** Cleanup and then update Extended VLAN Tagging Configuration Data
 *
 * \param[in] ctx                PON Network Library context pointer
 * \param[in] me_id              Extended VLAN Tagging Configuration Data ME ID
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_ext_vlan_reapply(struct pon_net_context *ctx,
						uint16_t me_id);

/** Get VLAN forwarding configuration from Extended VLAN Configuration Data ME
 *
 * \param[in]  ctx	PON NET context pointer
 * \param[in]  me_id	Extended VLAN Configuration Data ME ID
 * \param[out] fwd	VLAN Forwarding Configuration
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_ext_vlan_vlan_forwarding_get(struct pon_net_context *ctx,
				     uint16_t me_id,
				     struct pon_net_vlan_forwarding *fwd);

/** @} */ /* PON_NET_LIB */
#endif /* _PON_NET_EXT_VLAN_H */
