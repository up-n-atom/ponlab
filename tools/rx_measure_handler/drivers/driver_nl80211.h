/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <linux/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/family.h>
#include <netlink/genl/ctrl.h>
#include <netlink/msg.h>
#include <netlink/attr.h>
#include <stdint.h>
#include <net/if.h>
#include "nl80211.h"
#include <asm/errno.h>
#include <linux/genetlink.h>


#define SIZE_OF_NLMSG_HDR 64

typedef unsigned long long int u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

#include "vendor_cmds_copy.h"

struct nl80211_state {
	struct nl_sock *nl_sock;
	int nl80211_id;
	void (*event_handler)(struct nlattr **)
};

int nl80211_init(struct nl80211_state *state);
void nl80211_cleanup(struct nl80211_state *state);
uint32_t listen_events(struct nl80211_state *state);
