/*****************************************************************************
 *
 * Copyright (c) 2020 MaxLinear, Inc.
 * Copyright (c) 2019 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "pon_net_config.h"
#include "pon_net_netlink.h"
#include "pon_net_common.h"
#include "pon_net_debug.h"
#include "pon_net_netlink_debug.h"
#include "pon_net_rtnl.h"
#include "pon_net_extern.h"
#ifdef LINUX
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/in6.h>
#include <linux/if_bridge.h>
#include <unistd.h>
#include <errno.h>

#define NLMSG_TAIL(nmsg) \
	((struct rtattr *) (((void *) (nmsg)) + NLMSG_ALIGN((nmsg)->nlmsg_len)))

static int nested_addattr(struct nlmsghdr *n, unsigned int maxlen,
			  unsigned short type, const void *data,
			  unsigned int alen)
{
	unsigned short len = (unsigned short)RTA_LENGTH(alen);
	struct rtattr *rta;

	if (NLMSG_ALIGN(n->nlmsg_len) + RTA_ALIGN(len) > maxlen) {
		dbg_err("message exceeded bound of %d\n", maxlen);
		return -1;
	}
	rta = NLMSG_TAIL(n);
	rta->rta_type = type;
	rta->rta_len = len;
	if (alen) {
		if (memcpy_s(RTA_DATA(rta), len, data, alen)) {
			dbg_err_fn(memcpy_s);
			return -1;
		}
	}
	n->nlmsg_len = NLMSG_ALIGN(n->nlmsg_len) + RTA_ALIGN(len);
	return 0;
}

static int nested_addattr8(struct nlmsghdr *n, unsigned int maxlen,
			   unsigned short type, __u8 data)
{
	return nested_addattr(n, maxlen, type, &data, sizeof(__u8));
}

static int nested_addattr16(struct nlmsghdr *n, unsigned int maxlen,
			    unsigned short type, __u16 data)
{
	return nested_addattr(n, maxlen, type, &data, sizeof(__u16));
}

static struct rtattr *nesting_start(struct nlmsghdr *n, unsigned int maxlen,
				    unsigned short type)
{
	struct rtattr *nest = NLMSG_TAIL(n);

	nested_addattr(n, maxlen, type, NULL, 0);
	return nest;
}

static unsigned int nesting_finish(struct nlmsghdr *n, struct rtattr *nest)
{
	long len = (void *)NLMSG_TAIL(n) - (void *)nest;

	if (len < 0 || len > 0xFFFFL)
		perror("Invalid length calculation");
	nest->rta_len = (unsigned short)((void *)NLMSG_TAIL(n) - (void *)nest);
	return n->nlmsg_len;
}

static enum pon_adapter_errno
rtnl_bridge_learning_set(int ifindex, uint8_t learning)
{
	struct {
		struct nlmsghdr  n;
		struct ifinfomsg ifm;
		char	     buf[512];
	} req = {
		.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg)),
		.n.nlmsg_flags = NLM_F_REQUEST,
		.n.nlmsg_type = RTM_SETLINK,
		.ifm.ifi_family = PF_BRIDGE,
		.ifm.ifi_index = ifindex,
	};
	__u16 flags = 0;
	struct rtattr *nest;
	int rtnetlink_sk = -1;
	int err;

	dbg_in_args("%d, %u", ifindex, learning);

	rtnetlink_sk = pon_net_socket(AF_NETLINK, SOCK_RAW | SOCK_CLOEXEC,
				      NETLINK_ROUTE);
	if (rtnetlink_sk < 0) {
		perror("Cannot open rtnetlink socket");
		return PON_ADAPTER_ERROR;
	}

	nest = nesting_start(&req.n, sizeof(req),
		    IFLA_PROTINFO | NLA_F_NESTED);
	nested_addattr8(&req.n, sizeof(req), IFLA_BRPORT_LEARNING, learning);
	nesting_finish(&req.n, nest);

	nest = nesting_start(&req.n, sizeof(req), IFLA_AF_SPEC);
	flags |= BRIDGE_FLAGS_MASTER;
	nested_addattr16(&req.n, sizeof(req), IFLA_BRIDGE_FLAGS, flags);
	nesting_finish(&req.n, nest);

	err = (int)pon_net_send(rtnetlink_sk, &req, req.n.nlmsg_len, 0);
	if (err < 0) {
		perror("Cannot send to rtnetlink socket");
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	err = pon_net_close(rtnetlink_sk);
	if (err < 0) {
		perror("Cannot close rtnetlink socket");
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_rtnl_bridge_learning_set(struct pon_net_context *ctx,
				 const char *ifname,
				 uint8_t learning)
{
	enum pon_adapter_errno ret;
	unsigned int ifindex;

	dbg_in_args("%p, \"%s\", %u", ctx, ifname, learning);

	ifindex = pon_net_if_nametoindex(ifname);
	if ((int)ifindex <= 0) {
		perror("Cannot get ifindex");
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	netlink_debug_bridge_link_set(ifname, learning ? "learning on"
						       : "learning off");
	ret = rtnl_bridge_learning_set((int)ifindex, learning);
	dbg_out_ret("%d", ret);
	return ret;
}

#else

enum pon_adapter_errno
pon_net_rtnl_bridge_learning_set(struct pon_net_context *ctx,
				 const char *ifname,
				 uint8_t learning)
{
	return PON_ADAPTER_SUCCESS;
}

#endif
