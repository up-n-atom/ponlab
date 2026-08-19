/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file
 * This file contains common code for implementation of the UNI ports
 */

#ifndef _PON_NET_UNI_H_
#define _PON_NET_UNI_H_

#include "pon_net_qdisc.h"

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

struct pon_net_context;

/** Number of traffic classes per netdevice */
#define PON_TRAFFIC_CLASS_COUNT 8
/** Number of maximum traffic classes */
#define PON_TRAFFIC_CLASS_CNT_MAX 24
/** Number of egress reassignment rules for UNI */
#define PON_UNI_REASSIGN_CNT 16
/** Number of external switch ingress rules */
#define PON_EXT_INGRESS_CNT 6
/** Number of external switch egress rules:
 * One rule per traffic class for single tagged packets +
 * one rule per traffic class for double tagged packets +
 * one rule for untagged packets +
 * two special rules in order to replicate existing rules
 * to MC and BC netdev
 */
#define PON_EXT_EGRESS_CNT (8 + 8 + 1 + 2)

struct uni {
	/* Qdiscs that are configured at a given moment */
	struct pon_net_qdiscs *qdiscs;
	/* Priorities of TC filters for TC to queue mappings */
	uint32_t prios[PON_TRAFFIC_CLASS_CNT_MAX];
	/* Handles of TC filters for TC to queue mappings */
	uint32_t handles[PON_TRAFFIC_CLASS_CNT_MAX];
	/* Priorities of TC filters for TC reassignment */
	uint32_t prios_reassign[PON_UNI_REASSIGN_CNT];
	/* Handles of TC filters for TC reassignment */
	uint32_t handles_reassign[PON_UNI_REASSIGN_CNT];
	/* Priority of TC filter for external switch ingress */
	uint32_t ext_prio_ingress[PON_EXT_INGRESS_CNT];
	/* Priority of TC filter for external switch egress */
	uint32_t ext_prio_egress[PON_EXT_EGRESS_CNT];
	/* Handle of TC filter for external switch ingress */
	uint32_t ext_handle_ingress[PON_EXT_INGRESS_CNT];
	/* Handle of TC filter for external switch egress */
	uint32_t ext_handle_egress[PON_EXT_EGRESS_CNT];
};

/** Creates resources for the UNI port
 *
 *  \param[in]  ctx			PON Net Lib context
 *  \param[in]  class_id		Class ID
 *  \param[in]  me_id			Managed Entity ID
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_uni_create(struct pon_net_context *ctx,
					  uint16_t class_id, uint16_t me_id);

/** Destroys resources for the UNI port
 *
 *  \param[in]  ctx			PON Net Lib context
 *  \param[in]  class_id		Class ID
 *  \param[in]  me_id			Managed Entity ID
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_uni_destroy(struct pon_net_context *ctx,
					   uint16_t class_id, uint16_t me_id);

/** Updates qdiscs on the UNI ports
 *
 *  \param[in]  ctx			PON Net Lib context
 *  \param[in]  class_id		Class ID
 *  \param[in]  me_ids			Array of Managed Entity Ids
 *  \param[in]  len			Length of array
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_uni_qdiscs_update(struct pon_net_context *ctx,
						 const uint16_t class_id,
						 const uint16_t *me_ids,
						 unsigned int len);

/** Get UNI Me ID by lport
 *
 *  \param[in]  ctx			PON Net Lib context
 *  \param[in]  class_id		Class ID
 *  \param[in]  lport			Lan port index
 *  \param[in]  me_id			Managed Entity Id
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_uni_me_id_by_lport_get(struct pon_net_context *ctx,
			       const uint16_t class_id, uint8_t lport,
			       uint16_t *me_id);

/** Clean up qdiscs on the UNI port
 *
 *  \param[in]  ctx			PON Net Lib context
 *  \param[in]  class_id		Class ID
 *  \param[in]  me_ids			Array of Managed Entity Ids
 *  \param[in]  len			Length of array
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_uni_qdiscs_cleanup(struct pon_net_context *ctx,
						  uint16_t class_id,
						  const uint16_t *me_ids,
						  unsigned int len);

/** Configure UNI reassignment rules
 *
 *  \param[in]  ctx			PON Net Lib context
 *  \param[in]  class_id		Class ID
 *  \param[in]  me_id			Managed Entity ID
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_uni_eg_tc_reassign(struct pon_net_context *ctx,
						  uint16_t class_id,
						  uint16_t me_id);

/** Clear UNI reassignment rules
 *
 *  \param[in]  ctx			PON Net Lib context
 *  \param[in]  class_id		Class ID
 *  \param[in]  me_id			Managed Entity ID
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_uni_eg_tc_reassign_clr(struct pon_net_context *ctx,
			       uint16_t class_id,
			       uint16_t me_id);

/** Configure external switch UNI port
 *
 *  \param[in]  ctx			PON Net Lib context
 *  \param[in]  class_id		Class ID
 *  \param[in]  me_id			Managed Entity ID
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_extern_uni_cfg(struct pon_net_context *ctx,
		       uint16_t class_id,
		       uint16_t me_id);

/** Clear external switch UNI port configuration
 *
 *  \param[in]  ctx			PON Net Lib context
 *  \param[in]  class_id		Class ID
 *  \param[in]  me_id			Managed Entity ID
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_extern_uni_clr(struct pon_net_context *ctx,
		       uint16_t class_id,
		       uint16_t me_id);

/** Check if interface is external switch UNI port
 *
 *  \param[in]  ctx			PON Net Lib context
 *  \param[in]  lport			LAN port index
 */
bool pon_net_is_ext_uni(struct pon_net_context *ctx,
			uint16_t lport);

/** Get if interface is configured external switch UNI port
 *
 *  \param[in]  ctx			PON Net Lib context
 *  \param[in]  lport			LAN port index
 */
char *pon_net_ext_uni_master_get(struct pon_net_context *ctx,
				 uint16_t lport);

/** Get UNI network device name for Managed Entity
 *
 * For external UNI interfaces the external switch master netdevice name
 * is returned if existing or regular interface name otherwise.
 *
 * \param[in]  ctx      PON Network Library context pointer
 * \param[in]  class_id Managed Entity Class ID
 * \param[in]  me_id    Managed Entity ID
 * \param[out] ifname   Network device name
 * \param[out] size     ifname size
 */
enum pon_adapter_errno pon_net_ext_uni_ifname_get(struct pon_net_context *ctx,
						  uint16_t class_id,
						  uint16_t me_id,
						  char *ifname, size_t size);

/** @} */ /* PON_NET_LIB */
#endif
