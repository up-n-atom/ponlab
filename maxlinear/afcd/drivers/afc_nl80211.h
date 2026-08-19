/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <stdio.h>
#include <net/if.h>
#include "nl80211.h"

#define VAP_NAME_6GHZ "wlan4"
#define SIZE_OF_NLMSG_HDR 64

struct nl80211_state {
	struct nl_sock *nl_sock;
	int nl80211_id;
};

int afc_nl80211_init(void);
int afc_nl80211_send_afc_info_to_drv(const uint8_t *data, size_t length);
void afc_nl80211_cleanup(void);
