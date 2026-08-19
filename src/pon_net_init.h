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
 * This file contains definitions and prototypes of functions
 * used by pon_net_register.c
 */

#ifndef _PON_NET_INIT_H_
#define _PON_NET_INIT_H_

#include <pon_adapter_errno.h>

struct pon_net_context;

/** Inform system that a master device is used and create qdisc for it.
 *
 * \param[in]  ctx		PON Network Library Context
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_master_dev_init(struct pon_net_context *ctx);

/** Destroy the master device qdisc.
 *
 * \param[in]  ctx		PON Network Library Context
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_master_dev_exit(struct pon_net_context *ctx);

/** Inform system that a IPHOST device is used and create qdisc for it.
 *
 * \param[in]  ctx		PON Network Library Context
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_iphost_init(struct pon_net_context *ctx);

/** Cleanup the IPHOST device
 *
 * \param[in]  ctx		PON Network Library Context
 *
 */
void pon_net_iphost_exit(struct pon_net_context *ctx);

/** Create OMCI channel, set up tcont-omci and gem-omci for
 *  OMCI communication
 *
 * \param[in]  ctx		PON Network Library Context
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_omci_channel_init(struct pon_net_context *ctx);

/** Cleanup the OMCI channel
 *
 *  \param[in]  ctx		PON Network Library Context
 */
void pon_net_omci_channel_exit(struct pon_net_context *ctx);

/** Create CPU qdiscs and filters.
 *
 * \param[in]  ctx		PON Network Library Context
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_tc_cpu_queues_add(struct pon_net_context *ctx);

/** Cleanup CPU qdiscs
 *
 * \param[in]  ctx		PON Network Library Context
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_tc_queues_exit(struct pon_net_context *ctx);

/* Allocate CPU handles and prios from the pool
 *
 * \param[in]  ctx		PON Network Library Context
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_pon_handles_alloc(struct pon_net_context *ctx);

/* Release and return handles and prios to the pool
 *
 *  \param[in]  ctx		PON Network Library Context
 */
void pon_net_pon_handles_free(struct pon_net_context *ctx);

/** Set up External Switch netdevice if configured.
 *
 * \param[in]  ctx		PON Network Library Context
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_ext_switch_init(struct pon_net_context *ctx);

/** Cleanup External Switch netdevice if configured.
 *
 *  \param[in]  ctx		PON Network Library Context
 */
void pon_net_ext_switch_exit(struct pon_net_context *ctx);

#endif /* _PON_NET_INIT_H_ */
