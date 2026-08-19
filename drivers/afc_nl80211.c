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
#include "afc_nl80211.h"
#include "afc.h"
#include "afc_reg_rule.h"

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

struct nl80211_state state;

void afc_nl80211_cleanup()
{
	nl_socket_free(state.nl_sock);
}

int afc_nl80211_init(void)
{
	int err = 1;

	state.nl_sock = nl_socket_alloc();
	if (!state.nl_sock) {
		afc_printf(MSG_ERROR, "failed to allocate netlink socket.");
		return AFC_STATUS_FAILURE;
	}

	if (genl_connect(state.nl_sock)) {
		afc_printf(MSG_ERROR, "failed to connect to generic netlink.");
		goto out_handle_destroy;
	}

	/* try to set NETLINK_EXT_ACK to 1, ignoring errors */
	if (setsockopt(nl_socket_get_fd(state.nl_sock), SOL_NETLINK,
				   NETLINK_EXT_ACK, &err, sizeof(err)) < 0) {
		afc_printf(MSG_ERROR, "setsockopt failed");
		goto out_handle_destroy;
	}

	state.nl80211_id = genl_ctrl_resolve(state.nl_sock, "nl80211");
	if (state.nl80211_id < 0) {
		afc_printf(MSG_ERROR, "nl80211 not found.");
		goto out_handle_destroy;
	}

	return AFC_STATUS_SUCCESS;

out_handle_destroy:
	nl_socket_free(state.nl_sock);
	return AFC_STATUS_FAILURE;
}

int afc_nl80211_send_afc_info_to_drv(const uint8_t *data, size_t length)
{
	int ret;
	int ifidx;
	char master_ifname[IFNAMSIZ] = VAP_NAME_6GHZ; /* TODO: Need to fetch the ifname */
	struct nl_msg *msg;
	size_t nlmsg_sz;

	if (!state.nl_sock)
		return AFC_STATUS_FAILURE;

	nlmsg_sz = nlmsg_total_size(length + SIZE_OF_NLMSG_HDR);
	msg = nlmsg_alloc_size(nlmsg_sz);
	if (!msg) {
		afc_printf(MSG_ERROR, "failed to allocate netlink message");
		return AFC_STATUS_FAILURE;
	}

	if (!genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, state.nl80211_id, 0, 0,
					 NL80211_CMD_VENDOR, 0)) {
		afc_printf(MSG_ERROR, "error constructing the nl hdr");
		goto nla_put_failure;
	}

	ifidx = if_nametoindex(master_ifname);
	afc_printf(MSG_INFO, "ifindex : %d", ifidx);
	if (!ifidx)
		goto nla_put_failure;

	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, ifidx);
	NLA_PUT_U32(msg, NL80211_ATTR_VENDOR_ID, OUI_LTQ);
	NLA_PUT_U32(msg, NL80211_ATTR_VENDOR_SUBCMD, LTQ_NL80211_VENDOR_SUBCMD_UPDATE_AFC_INFO);

	afc_printf(MSG_INFO, "size of NL data: %zu, total size of NL msg : %zu", length, nlmsg_sz);

	if (nla_put(msg, NL80211_ATTR_VENDOR_DATA, length, data) < 0) {
		afc_printf(MSG_ERROR, "failed to add data_buffer as attribute");
		goto nla_put_failure;
	}

	ret = nl_send_auto_complete(state.nl_sock, msg);
	if (ret < 0) {
		afc_printf(MSG_ERROR, "failed to send NL msg : %d", ret);
		goto nla_put_failure;
	}

	ret = nl_recvmsgs_default(state.nl_sock);
	if (ret < 0) {
		afc_printf(MSG_ERROR, "failed to receive NL: %d", ret);
		goto nla_put_failure;
	}

	nlmsg_free(msg);
	return AFC_STATUS_SUCCESS;

nla_put_failure:
	nlmsg_free(msg);
	return AFC_STATUS_FAILURE;
}
