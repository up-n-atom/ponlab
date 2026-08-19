/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file
 * This file holds definitions of function that print the iproute commands
 * equivalent to libnl operations.
 */

#ifndef _PON_NET_NETLINK_DEBUG_H_
#define _PON_NET_NETLINK_DEBUG_H_

#include <stdint.h>
#include <stdbool.h>

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/** \defgroup PON_NET_DEBUG PON Network Library Debug Functions
 *  These are debug functions dedicated for the PON Network Library.
 *
 * @{
 */

/* Forward declarations */
struct rtnl_cls;
struct rtnl_qdisc;
struct rtnl_class;
struct nl_cache;
struct netlink_gem;
struct netlink_bridge;

/** PON Network library debug function to check tc flower filter settings.
 *
 * \param[in]  link_cache        Link cache
 * \param[out] cls               Return value
 */
void netlink_debug_tc_flower_filter_add(struct nl_cache *link_cache,
					struct rtnl_cls *cls);

void netlink_debug_tc_flower_filter_del(struct nl_cache *link_cache,
					struct rtnl_cls *cls);

void netlink_debug_tc_qdisc_add(struct nl_cache *link_cache,
				struct rtnl_qdisc *qdisc);

void netlink_debug_tc_qdisc_del(struct nl_cache *link_cache,
				struct rtnl_qdisc *qdisc);

void netlink_debug_tc_class_add(struct nl_cache *link_cache,
				struct rtnl_class *clss);

void netlink_debug_tc_class_del(struct nl_cache *link_cache,
				struct rtnl_class *clss);

void netlink_debug_ip_link_add_pmapper(const char *pon0,
				       const char *name,
				       int pcpdef);

void netlink_debug_ip_link_set_pmapper(const char *name,
				       uint16_t *gem_ctp_me_ids,
				       unsigned int count,
				       uint8_t *dscp_to_pbit);

void netlink_debug_ip_link_set(const char *name, const char *options);

void netlink_debug_ip_link_set_mtu(const char *name, unsigned int mtu);

void netlink_debug_ip_link_set_macaddr(const char *name,
				       const uint8_t if_mac[ETH_ALEN]);

void netlink_debug_ip_link_set_master(const char *master, const char *slave);

void netlink_debug_ip_link_del(const char *device);

void netlink_debug_ip_link_add_tcont(const char *pon0,
				     const char *name,
				     unsigned int tcont_id);

struct netlink_gem;
void netlink_debug_ip_link_gem(const char *ifname, const char *action,
			       const struct netlink_gem *gem);

void netlink_debug_ip_link_add_gem_tcont(const char *pon0,
					 const char *name,
					 unsigned int gem_id,
					 const char *tcont);

void netlink_debug_ip_link_bridge_add(const char *name,
				      const struct netlink_bridge *bridge);

void netlink_debug_bridge_vlan(const char *ifname, uint16_t vlan, bool self,
			       bool del);

void netlink_debug_bridge_link_set(const char *ifname, const char *options);

/** @} */ /* PON_NET_DEBUG */

/** @} */ /* PON_NET_LIB */

#endif
