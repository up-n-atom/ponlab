/*****************************************************************************
 *
 * Copyright (c) 2021 - 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "pon_net_pool.h"
#include "pon_net_dev_db.h"
#include "pon_net_dev_defs.h"
#include "pon_net_common.h"
#include "pon_net_debug.h"

static const struct pon_net_dev_pool_cfg pools[] = {
	/* Handle ingress start */
	{ .id = PON_NET_HANDLE_COLOR_MARKING_INGRESS,
		.min = 1, .max = 1, .is_static = true },
	{ .id = PON_NET_HANDLE_POLICE_INGRESS,
		.min = 2, .max = 2, .is_static = true },
	{ .id = PON_NET_HANDLE_LCT_INGRESS,
		.min = 2, .max = 100, .is_static = true },
	{ .id = PON_NET_HANDLE_MCC_DROP_INGRESS,
		.min = 101, .max = 110, .is_static = true },
	{ .id = PON_NET_HANDLE_FILTER_INGRESS,
		.min = 111, .max = 0xFFFFFFFE },
	/* Handle ingress end */

	/* Handle egress start */
	{ .id = PON_NET_HANDLE_POLICE_EGRESS,
		.min = 1, .max = 1, .is_static = true },
	{ .id = PON_NET_HANDLE_FILTER_EGRESS,
		.min = 2, .max = 0xFFFFFFFE },
	/* Handle egress end */

	/* Priority ingress start */
	{ .id = PON_NET_PRIO_VLAN_FILTERING_INGRESS,
		.min = 1, .max = 250 },
	{ .id = PON_NET_PRIO_TC_TO_QUEUE_ASSIGNMENT,
		.min = 251, .max = 750 },
	{ .id = PON_NET_PRIO_GEM_TO_QUEUE_ASSIGNMENT,
		.min = 751, .max = 4397 },
	{ .id = PON_NET_PRIO_LCT_INGRESS,
		.min = 4398, .max = 5000, .is_static = true },
	{ .id = PON_NET_PRIO_MC_INGRESS,
		.min = 5001, .max = 5060 },
	{ .id = PON_NET_PRIO_MCC_DROP_INGRESS,
		.min = 5061, .max = 5100, .is_static = true },
	{ .id = PON_NET_PRIO_EXT_VLAN_INGRESS_FORWARD,
		.min = 5101, .max = 5500 },
	{ .id = PON_NET_PRIO_EXT_VLAN_INGRESS,
		.min = 5501, .max = 59999 },
	{ .id = PON_NET_PRIO_PREASSIGN_VLAN_INGRESS,
		.min = 60000, .max = 60999 },
	{ .id = PON_NET_PRIO_PREASSIGN_INGRESS,
		.min = 61000, .max = 61999 },
	{ .id = PON_NET_PRIO_VLAN_FLOW_INGRESS_TCI,
		.min = 62000, .max = 62099 },
	{ .id = PON_NET_PRIO_VLAN_FLOW_INGRESS_VID,
		.min = 62100, .max = 62199 },
	{ .id = PON_NET_PRIO_VLAN_FLOW_INGRESS_ANY_VLAN,
		.min = 62200, .max = 62299 },
	{ .id = PON_NET_PRIO_VLAN_FLOW_INGRESS_ANY,
		.min = 62300, .max = 62399 },
	{ .id = PON_NET_PRIO_VLAN_FLOW_INGRESS_ALL,
		.min = 62400, .max = 62499 },
	/* Range is reserved for PON Adapter vlan flow API */
	{ .id = PON_NET_PRIO_VLAN_FLOW_INGRESS_RESERVED,
		.min = 63000, .max = 64000, .is_static = true },
	{ .id = PON_NET_PRIO_EXT_VLAN_INGRESS_DEFAULT,
		.min = 64001, .max = 64900 },
	{ .id = PON_NET_PRIO_EXT_VLAN_INGRESS_DISCARD,
		.min = 64901, .max = 65000 },
	{ .id = PON_NET_PRIO_VLAN_FILTERING_INGRESS_DEFAULT,
		.min = 65001, .max = 65250 },
	{ .id = PON_NET_PRIO_OMCI_TRAP_INGRESS,
		.min = 65252, .max = 65252 },
	{ .id = PON_NET_PRIO_COLOR_MARKING_INGRESS,
		.min = 65253, .max = 65253, .is_static = true },
	{ .id = PON_NET_PRIO_POLICE_INGRESS,
		.min = 65254, .max = 65254, .is_static = true },
	/* Priority ingress end */

	/* Priority egress start */
	{ .id = PON_NET_PRIO_VLAN_FILTERING_EGRESS,
		.min = 1, .max = 250 },
	{ .id = PON_NET_PRIO_MC_EGRESS,
		.min = 251, .max = 301 },
	{ .id = PON_NET_PRIO_EXT_VLAN_EGRESS_FORWARD,
		.min = 301, .max = 360 },
	{ .id = PON_NET_PRIO_EXT_VLAN_EGRESS,
		.min = 361, .max = 62999 },
	/* Range is reserved for PON Adapter vlan flow API */
	{ .id = PON_NET_PRIO_VLAN_FLOW_EGRESS_RESERVED,
		.min = 63000, .max = 64000, .is_static = true },
	{ .id = PON_NET_PRIO_EXT_VLAN_EGRESS_DEFAULT,
		.min = 64001, .max = 64900 },
	{ .id = PON_NET_PRIO_EXT_VLAN_EGRESS_DISCARD,
		.min = 64901, .max = 65000 },
	{ .id = PON_NET_PRIO_VLAN_FILTERING_EGRESS_DEFAULT,
		.min = 65001, .max = 65250 },
	{ .id = PON_NET_PRIO_POLICE_EGRESS,
		.min = 65254, .max = 65254, .is_static = true },
	/* Priority egress end */
};

const struct pon_net_dev_def pon_net_gem_def = {
	.pool_cfgs = pools,
	.pool_cfgs_count = ARRAY_SIZE(pools),
};

const struct pon_net_dev_def pon_net_pmapper_def = {
	.pool_cfgs = pools,
	.pool_cfgs_count = ARRAY_SIZE(pools),
};

static const struct pon_net_dev_pool_cfg pon_pools[] = {
	/* Handle ingress start */
	{ .id = PON_NET_HANDLE_IPHOST_VLAN_TRAP,
		.min = 1, .max = 5000, .is_static = true },
	{ .id = PON_NET_HANDLE_MCC_TRAP_INGRESS,
		.min = 5001, .max = 5100, .is_static = true },
	{ .id = PON_NET_HANDLE_FILTER_INGRESS,
		.min = 5101, .max = 0xFFFFFFFE },
	/* Handle ingress end */

	/* Handle egress start */
	{ .id = PON_NET_HANDLE_FILTER_EGRESS,
		.min = 1, .max = 0xFFFFFFFE },
	/* Handle egress end */

	/* Prio ingress start */
	{ .id = PON_NET_PRIO_IPHOST_VLAN_TRAP,
		.min = 1, .max = 5000, .is_static = true },
	{ .id = PON_NET_PRIO_MC_FLOW_FORWARD_INGRESS,
		.min = 5002, .max = 5128 },
	{ .id = PON_NET_PRIO_IPHOST_MAC_TRAP_INGRESS,
		.min = 5200, .max = 6000 },
	{ .id = PON_NET_PRIO_MCC_TRAP_INGRESS,
		.min = 5300, .max = 5400, .is_static = true },
	{ .id = PON_NET_PRIO_TC_TO_QUEUE_ASSIGNMENT,
		.min = 5401, .max = 5450 },
	/* Range is reserved for PON Adapter vlan flow API */
	{ .id = PON_NET_PRIO_VLAN_FLOW_INGRESS_RESERVED,
		.min = 63000, .max = 64000, .is_static = true },
	/* Prio ingress end */

	/* Prio egress start */
	/* Range is reserved for PON Adapter vlan flow API */
	{ .id = PON_NET_PRIO_VLAN_FLOW_EGRESS_RESERVED,
		.min = 63000, .max = 64000, .is_static = true },
	/* Prio egress end */
};

const struct pon_net_dev_def pon_net_pon_def = {
	.pool_cfgs = pon_pools,
	.pool_cfgs_count = ARRAY_SIZE(pon_pools),
};

static const struct pon_net_dev_pool_cfg tcont_pools[] = {
	/* Handle ingress start */
	{ .id = PON_NET_HANDLE_FILTER_INGRESS,
		.min = 1, .max = 0xFFFFFFFE },
	/* Handle ingress end */

	/* Handle ingress start */
	{ .id = PON_NET_HANDLE_FILTER_EGRESS,
		.min = 1, .max = 0xFFFFFFFE },
	/* Handle ingress end */

	/* Priority ingress start */
	{ .id = PON_NET_PRIO_GEM_TO_QUEUE_ASSIGNMENT,
		.min = 100, .max = 4096 },
	/* Range is reserved for PON Adapter vlan flow API and its */
	{ .id = PON_NET_PRIO_VLAN_FLOW_INGRESS_RESERVED,
		.min = 63000, .max = 64000, .is_static = true },
	/* Prio ingress end */

	/* Prio egress start */
	/* Range is reserved for PON Adapter vlan flow API and its */
	{ .id = PON_NET_PRIO_VLAN_FLOW_INGRESS_RESERVED,
		.min = 63000, .max = 64000, .is_static = true },
	/* Prio egress end */
};

const struct pon_net_dev_def pon_net_tcont_def = {
	.pool_cfgs = tcont_pools,
	.pool_cfgs_count = ARRAY_SIZE(tcont_pools),
};

const struct pon_net_dev_def pon_net_uni_bc_def = {
	.pool_cfgs = pools,
	.pool_cfgs_count = ARRAY_SIZE(pools),
};

const struct pon_net_dev_def pon_net_uni_def = {
	.pool_cfgs = pools,
	.pool_cfgs_count = ARRAY_SIZE(pools),
};

const struct pon_net_dev_def pon_net_uni_mc_def = {
	.pool_cfgs = pools,
	.pool_cfgs_count = ARRAY_SIZE(pools),
};

const struct pon_net_dev_def pon_net_iphost_def = {
	.pool_cfgs = pools,
	.pool_cfgs_count = ARRAY_SIZE(pools),
};
