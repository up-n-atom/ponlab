/******************************************************************************
 *
 * Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 * This file holds definitions for mapping between OMCI Managed Entity assets.
 */

#ifndef _pon_net_mapper_table_h
#define _pon_net_mapper_table_h

#include "pon_adapter_mapper.h"

/* Forward declaration */
struct pon_net_context;

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/** \defgroup PON_NET_PA_MAPPER PON Adapter Low-level Index Mapper
 *  This is a mapping table to handle dependencies between OMCI resources.
 *
 * @{
 */

/** Mapper ID type definition */
enum mapper_id_type {
	/** Mapper ID type minimum value */
	MAPPER_IDTYPE_MIN = 0,
	/** Mapper from MAC Bridge configuration Managed Entity ID to index */
	MAPPER_MACBRIDGE_MEID_TO_IDX = MAPPER_IDTYPE_MIN,
	/** Mapper from MAC Bridge Port Configuration Data Managed Entity ID
	 *  to index
	 */
	MAPPER_MACBRIDGEPORT_MEID_TO_IDX,
	/** Mapper from MAC Bridge Port Configuration Data Managed Entity ID
	 *  to termination point
	 */
	MAPPER_MACBRIDGEPORT_MEID_TO_TP,
	/** Mapper from T-CONT Managed Entity ID to Allocation ID */
	MAPPER_TCONT_MEID_TO_ALLOC_ID,
	/** Mapper from GEM Port Network CTP Managed Entity ID to GEM port ID */
	MAPPER_GEMPORTCTP_MEID_TO_ID,
	/** Mapper from GEM Port Interworking TP Managed Entity ID
	 *  to GEM port ID
	 */
	MAPPER_GEMPORTITP_MEID_TO_ID,
	/** Mapper from Multicast GEM Port Interworking TP Managed Entity ID
	 *  to GEM Port Network CTP Managed Entity ID
	 */
	MAPPER_MCGEMPORTITP_MEID_TO_CTP_MEID,
	/** Mapper from PPTP Ethernet UNI Managed Entity ID to index */
	MAPPER_PPTPETHERNETUNI_MEID_TO_IDX,
	/** Mapper from T-CONT Managed Entity ID to Traffic Scheduler
	 *  Managed Entity ID
	 */
	MAPPER_TCONT_MEID_TO_TS_MEID,
	/** Mapper from IEEE 802.1p-mapper Managed Entity ID to index */
	MAPPER_PMAPPER_MEID_TO_IDX,
	/** Maps T-CONT and Priority Queue to GEM Port ID */
	MAPPER_TCONT_PRI_QUEUE,
	/** Maps PPTP Ethernet UNI and Priority Queue to a GEM Port ID */
	MAPPER_PPTPETHERNETUNI_PRI_QUEUE,
	/** Mapper from Extended VLAN Configuration Data Managed Entity ID to
	    index */
	MAPPER_EXTVLAN_MEID_TO_IDX,
	/** Mapper from Virtual Ethernet Interface Point Managed Entity ID
	 *  to index
	 */
	MAPPER_VEIP_MEID_TO_IDX,
	/** Mapper from Priority Queue Managed Entity ID to index */
	MAPPER_PRIORITYQUEUE_MEID_TO_IDX,
	/** Mapper from IP Host Managed Entity ID to index */
	MAPPER_IPHOST_MEID_TO_IDX,
	/** Mapper from IPv6 Host Managed Entity ID to index */
	MAPPER_IPV6HOST_MEID_TO_IDX,
	/** Mapper ID type maximum value */
	MAPPER_IDTYPE_MAX
};

/** Initialize all mappings
 *
 * \param[in] ctx PON Network Library context pointer
 */
enum pon_adapter_errno pon_net_pa_mapper_init(struct pon_net_context *ctx);

/** Reset mappings
 *
 * \param[in] ctx PON NET context pointer
 */
enum pon_adapter_errno pon_net_pa_mapper_reset(struct pon_net_context *ctx);

/** Shutdown all mappings
 *
 * \param[in] ctx PON Network Library context pointer
 */
enum pon_adapter_errno pon_net_pa_mapper_shutdown(struct pon_net_context *ctx);

/** Dump mappings
 *
 * \param[in] ctx PON Network Library context pointer
 */
enum pon_adapter_errno pon_net_pa_mapper_dump(struct pon_net_context *ctx);

/** Retrieve index for the given ID
 *
 * \param[in]  m       Pointer to mapper
 * \param[in]  id      ID
 * \param[out] idx     Return mapped index
 */
enum pon_adapter_errno pon_net_pa_mapper_index_get_uint16(struct mapper *m,
							  uint32_t id,
							  uint16_t *idx);

/** Retrieve ID for the given index
 *
 * \param[in]  m       Pointer to mapper
 * \param[out] id      Return mapped ID
 * \param[in]  idx     Index
 */
enum pon_adapter_errno pon_net_pa_mapper_id_get_uint16(struct mapper *m,
						       uint16_t *id,
						       uint32_t idx);

/** @} */ /* PON_NET_PA_MAPPER */

/** @} */ /* PON_NET_LIB */

#endif
