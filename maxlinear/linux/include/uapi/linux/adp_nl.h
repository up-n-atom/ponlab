/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation.
 */

#ifndef _UAPI__ADP_NETLINK_H
#define _UAPI__ADP_NETLINK_H

#include <linux/types.h>
#include <linux/netlink.h>

/* Adding event notification support elements */
#define ADP_GENL_FAMILY_NAME                "adp_event"
#define ADP_GENL_VERSION                     0x01
#define ADP_GENL_MCAST_GROUP_NAME           "adp_mc_grp"

/* Adding commands */
#define ADP_COOLING_DIS  0x1 /* ADP_GENL_CMD_COOLING */
#define ADP_COOLING_ENA  0x2 /* ADP_GENL_CMD_COOLING */
#define ADP_DVFS_LOW     0x3 /* ADP_GENL_CMD_DVFS */
#define ADP_DVFS_MED     0x4 /* ADP_GENL_CMD_DVFS */
#define ADP_DVFS_HIGH    0x5 /* ADP_GENL_CMD_DVFS */
#define ADP_CMD_MASK     0x7
#define ADP_CMD_ACK      0x40000000

#define ADP_STRING_MAX 40

/* attributes of adp_genl_family */
enum {
	ADP_GENL_ATTR_UNSPEC,
	ADP_GENL_ATTR_SRC,     /* ADP, ETH, WiFi, ... */
	ADP_GENL_ATTR_COMMAND, /* cooling, link, .. */
	ADP_GENL_ATTR_LINK, /* port connected */
	ADP_GENL_ATTR_LSPEED,  /* link speed */
	ADP_GENL_ATTR_STATUS_COOL,  /* status of COOLING */
	ADP_GENL_ATTR_STATUS_DVFS,  /* status of DVFS */
	__ADP_GENL_ATTR_MAX,
};
#define ADP_GENL_ATTR_MAX (__ADP_GENL_ATTR_MAX - 1)


/* commands supported by the adp_genl_family */
enum {
	ADP_GENL_CMD_UNSPEC,
	ADP_GENL_CMD_COOLING,
	ADP_GENL_CMD_DVFS,
	ADP_GENL_CMD_STATUS,
	__ADP_GENL_CMD_MAX,
};
#define ADP_GENL_CMD_MAX (__ADP_GENL_CMD_MAX - 1)

int adp_send_nlmc(int nl_cmd, int cmd);
#endif /* _UAPI__ADP_NETLINK */
