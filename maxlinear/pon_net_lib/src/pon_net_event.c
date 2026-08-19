/*****************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifdef LINUX
#include <asm/types.h>
#include <sys/socket.h>
#endif

#include <pon_adapter.h>
#include <pon_adapter_event_handlers.h>
#include <omci/me/pon_adapter_pptp_ethernet_uni.h>
#include <omci/me/pon_adapter_mac_bridge_port_config_data.h>

#include "pon_net_common.h"
#include "pon_net_debug.h"
#include "pon_net_ethtool.h"
#include "pon_net_event.h"

#include <netlink/netlink.h>
#include <netlink/msg.h>
#include <netlink/socket.h>

#include <linux/rtnetlink.h>

#include <pthread.h>

/* Instead of including whole linux/if.h, we just define what we use.
 * This avoids compilation conflicts with net/if.h in old kernel versions.
 */
#ifndef IFF_LOWER_UP
#define IFF_LOWER_UP 1 << 16
#endif

static struct {
	enum pa_pptp_eth_uni_cfg_ind conf_ind;
	uint32_t speed;
	uint8_t duplex;
} configuration_ind[] = {
	{PA_PPTPETHUNI_CFG_IND_10_FULL, SPEED_10, DUPLEX_FULL},
	{PA_PPTPETHUNI_CFG_IND_100_FULL, SPEED_100, DUPLEX_FULL},
	{PA_PPTPETHUNI_CFG_IND_1000_FULL, SPEED_1000, DUPLEX_FULL},
	{PA_PPTPETHUNI_CFG_IND_2500_FULL, SPEED_2500, DUPLEX_FULL},
	{PA_PPTPETHUNI_CFG_IND_5000_FULL, SPEED_5000, DUPLEX_FULL},
	{PA_PPTPETHUNI_CFG_IND_10000_FULL, SPEED_10000, DUPLEX_FULL},
	{PA_PPTPETHUNI_CFG_IND_25000_FULL, SPEED_25000, DUPLEX_FULL},
	{PA_PPTPETHUNI_CFG_IND_40000_FULL, SPEED_40000, DUPLEX_FULL},
	{PA_PPTPETHUNI_CFG_IND_10_HALF, SPEED_10, DUPLEX_HALF},
	{PA_PPTPETHUNI_CFG_IND_100_HALF, SPEED_100, DUPLEX_HALF},
	{PA_PPTPETHUNI_CFG_IND_1000_HALF, SPEED_1000, DUPLEX_HALF}
};

static uint8_t interpret_configuration_ind(uint32_t speed, uint8_t duplex)
{
	enum pa_pptp_eth_uni_cfg_ind ret = PA_PPTPETHUNI_CFG_IND_AUTO;
	unsigned int i;

	dbg_in_args("%u, %u", speed, duplex);

	for (i = 0; i < ARRAY_SIZE(configuration_ind); ++i) {
		if (speed != configuration_ind[i].speed)
			continue;

		if (configuration_ind[i].duplex != duplex)
			continue;

		ret = configuration_ind[i].conf_ind;
		break;
	}

	dbg_out_ret("%u", (uint8_t)ret);
	return (uint8_t)ret;
}

#define CFG_UNI_NAME_SIZE_MAX 100000
static int
pon_net_get_idx_for_ifname(struct pon_net_context *ctx, char *ifname)
{
	int i;
	int size_max;
	unsigned int array_size = ARRAY_SIZE(ctx->cfg.uni_name);

	dbg_in_args("%p, %s", ctx, ifname);

	size_max = (array_size > CFG_UNI_NAME_SIZE_MAX) ?
				CFG_UNI_NAME_SIZE_MAX : (int)array_size;

	for (i = 0; i < size_max; i++) {
		if (strncmp(ctx->cfg.uni_name[i], ifname, IF_NAMESIZE) == 0) {
			dbg_out_ret("%u", i);
			return i;
		}
	}

	dbg_out_ret("%u", -ENOENT);
	return -ENOENT;
}

static uint32_t
pon_net_get_meid_for_ifname(struct pon_net_context *ctx, char *ifname)
{
	int32_t idx;
	uint32_t id;
	int32_t ret;

	dbg_in_args("%p, %s", ctx, ifname);

	idx = pon_net_get_idx_for_ifname(ctx, ifname);
	if (idx < 0) {
		dbg_out_ret("%u", idx);
		return 0xFFFFFFFF;
	}

	ret = mapper_id_get(ctx->mapper[MAPPER_PPTPETHERNETUNI_MEID_TO_IDX],
			       &id, (uint32_t)idx);
	if (ret < 0) {
		dbg_out_ret("%u", ret);
		return 0xFFFFFFFF;
	}

	dbg_out_ret("%u", id);
	return id;
}

static void pon_net_link_state_notify(struct pon_net_context *ctx,
				      bool carrier_up, char *ifname)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_ethtool_link_settings link_settings = { 0 };
	uint8_t config_ind = 0;
	uint32_t me_id_get;
	uint16_t me_id;

	dbg_in_args("%p, %i, %s", ctx, carrier_up, ifname);

	if (!ifname)  {
		dbg_out();
		return;
	}

	if (!ctx->event_handlers || !ctx->event_handlers->link_state)  {
		dbg_out();
		return;
	}

	/* We are only interested in events on the PPTP UNI interface */
	me_id_get = pon_net_get_meid_for_ifname(ctx, ifname);
	if (me_id_get > 0xFFFF) {
		dbg_out();
		return;
	}
	me_id = (uint16_t)me_id_get;

	/* Set config_ind to 0 when carrier is not up. */
	if (carrier_up) {
		ret = pon_net_ethtool_link_settings_get(ctx, ifname,
							&link_settings);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_ethtool_link_settings_get, ret);
			dbg_out();
			return;
		}

		config_ind =
			interpret_configuration_ind(link_settings.req.speed,
						    link_settings.req.duplex);
	}

	ctx->event_handlers->link_state(ctx->hl_handle,
					me_id,
					carrier_up,
					config_ind);

	dbg_out();
}

static int nl_valid_callback(struct nl_msg *msg, void *priv)
{
	struct pon_net_context *ctx = priv;
	struct nlmsghdr *nlh = nlmsg_hdr(msg);
	struct ifinfomsg *ifhdr = NLMSG_DATA(nlh);
	size_t remaining = nlh->nlmsg_len - NLMSG_LENGTH(sizeof(*ifhdr));
	struct rtattr *ifattr = IFLA_RTA(ifhdr);
	char *ifname = NULL;
	/* The carrier flags indicates whether the cable was plugged in */
	bool carrier_up = ifhdr->ifi_flags & IFF_LOWER_UP;

	dbg_in_args("%p %p", msg, priv);

	/* Search for the ifname in the attribute list */
	while (RTA_OK(ifattr, remaining)) {
		if (ifattr->rta_type == IFLA_IFNAME) {
			ifname = (char *)RTA_DATA(ifattr);
			break;
		}
		ifattr = RTA_NEXT(ifattr, remaining);
	}

	/* We are only interested in events with an interface name */
	if (ifname)
		pon_net_link_state_notify(ctx, carrier_up, ifname);

	dbg_out_ret("%d", 0);

	return NL_OK;
}

static void cleanup_nl_socket(void *sock)
{
	struct nl_sock *nl_route_sock = sock;

	if (nl_route_sock)
		nl_socket_free(nl_route_sock);
}

/*
 * This has been experimentally chosen. Increase it if you get NLE_NOMEM errors
 * from libnl functions that receive messages from sockets.
 */
#define NL_SOCK_RX_BUF_SIZE 65536

static void *event_handling_thread(void *arg)
{
	struct pon_net_context *ctx = arg;
	/** NetLink Route socket */
	struct nl_sock *nl_route_sock;
	int ret = PON_ADAPTER_ERROR;
	int err;
	int fd;

	dbg_in_args("%p", arg);

	err = pthread_setname_np(pthread_self(), "pon_net_event");
	if (err)
		dbg_err("%s: Can't set <%s> name for a thread\n",
			__func__, "pon_net_event");

	nl_route_sock = nl_socket_alloc();
	if (!nl_route_sock) {
		dbg_err_fn_ret(nl_socket_alloc, 0);
		goto errout;
	}

	/* Push on the thread stack a function which frees
	 * nl_route_sock socket in case of the thread cancellation.
	 */
	pthread_cleanup_push(cleanup_nl_socket, nl_route_sock);

	ret = nl_connect(nl_route_sock, NETLINK_ROUTE);
	if (ret) {
		dbg_err_fn_ret(nl_connect, ret);
		goto errout;
	}

	fd = nl_socket_get_fd(nl_route_sock);
	if (fd < 0) {
#ifndef UNIT_TESTS
		dbg_err_fn_ret(nl_socket_get_fd, fd);
#endif
		ret = fd;
		goto errout;
	}

	ret = nl_socket_add_membership(nl_route_sock, RTNLGRP_LINK);
	if (ret) {
		dbg_err_fn_ret(nl_socket_add_membership, ret);
		goto errout;
	}

	/*
	 * This is needed for libnl to realloc() iov buffer,
	 * if it is too small to contain entire message received using recvmsg()
	 */
	nl_socket_enable_msg_peek(nl_route_sock);

	/* Set this to avoid -ENOMEM from underlying recvmg() */
	nl_socket_set_buffer_size(nl_route_sock, NL_SOCK_RX_BUF_SIZE, 0);

	nl_socket_disable_seq_check(nl_route_sock);
	ret = nl_socket_modify_cb(nl_route_sock, NL_CB_VALID,
				  NL_CB_CUSTOM, nl_valid_callback, ctx);
	if (ret) {
		dbg_err_fn_ret(nl_socket_modify_cb, ret);
		goto errout;
	}

	ret = nl_socket_set_nonblocking(nl_route_sock);
	if (ret) {
		dbg_err_fn_ret(nl_socket_set_nonblocking, ret);
		goto errout;
	}

	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	for (;;) {
		fd_set rfds;
		struct timeval tv;

		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);

#ifdef UNIT_TESTS
		/* timeout 0.01 s (10 ms) to speed up unit tests */
		tv.tv_sec = 0;
		tv.tv_usec = 10000;
#else
		/* in system the timeout is 1s for better power saving */
		tv.tv_sec = 1;
		tv.tv_usec = 0;
#endif
		ret = select(fd+1, &rfds, NULL, NULL, &tv);

		/* errors are negative and 0 is timeout */
		if (ret <= 0)
			continue;

		ret = nl_recvmsgs_default(nl_route_sock);
		if (ret < 0) {
			dbg_err("problem receiving message: %i (%s)\n", ret,
				nl_geterror(ret));
			break;
		}

		pthread_testcancel();
	}

errout:
	/* Remove the nl_route_sock freeing function from the thread stack */
	pthread_cleanup_pop(false);

	if (nl_route_sock)
		nl_socket_free(nl_route_sock);

	dbg_out_ret("%d", ret);
	return (void *)(long int)ret;
}

enum pon_adapter_errno pon_net_if_event_init(struct pon_net_context *ctx)
{
	int err;

	dbg_in_args("%p", ctx);

	err = pthread_create(&ctx->nl_route_thread,
			     NULL, event_handling_thread, ctx);
	if (err) {
		dbg_err_fn_ret(pthread_create, err);
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Time in [s] to join the thread after cancel call */
#define EVENT_THREAD_CANCEL_TIME 1

enum pon_adapter_errno pon_net_if_event_stop(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	static const char *th_name = "pon_net_event";

	dbg_in_args("%p", ctx);

#ifndef WIN32
	if (!ctx->nl_route_thread) {
		dbg_out_ret("%d", ret);
		return ret;
	}
#endif
	ret = pon_net_thread_stop(&ctx->nl_route_thread, th_name,
				  EVENT_THREAD_CANCEL_TIME);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err("attempt to cancel thread <%s> failed\n", th_name);

	dbg_out_ret("%d", ret);
	return ret;
}
