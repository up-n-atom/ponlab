/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef _PON_NET_PRIORITY_QUEUE_H
#define _PON_NET_PRIORITY_QUEUE_H

#include <stdint.h>

struct pon_net_context;
struct pon_net_vlan_forwarding;

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/**
 * Map Priority Queue's ME priority value to hardware queue number
 *
 * \param[in] priority       Priority
 *
 * \return queue number
 */
uint16_t pon_net_map_priority_to_queue(uint16_t priority);

/**
 * Map traffic class to hardware queue number
 *
 * \param[in] tc             Traffic class
 *
 * \return queue number
 */
uint16_t pon_net_map_tc_to_queue(uint16_t tc);

#endif /* _PON_NET_PRIORITY_QUEUE_H */
