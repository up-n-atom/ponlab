/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include "eloop.h"
#include "afc.h"
#include "afc_nl80211.h"
#include "ctrl.h"
#include "list.h"

extern int afc_debug_level;

void afc_ctrl_iface_free(struct dl_list *ctrl_dst)
{
	struct ctrl_client *dst, *next;

	dl_list_for_each_safe(dst, next, ctrl_dst, struct ctrl_client, list) {
		dl_list_del(&dst->list);
		free(dst);
	}
}

struct ctrl_client *afc_ctrl_find_dst(struct dl_list *ctrl_dst, struct sockaddr_storage *from,
				      socklen_t fromlen)
{
	struct ctrl_client *dst, *next;
	struct sockaddr_un *a, *b;

	a = (struct sockaddr_un *)from;
	dl_list_for_each_safe(dst, next, ctrl_dst, struct ctrl_client, list) {
		b = (struct sockaddr_un *)&dst->addr;
		if ((fromlen == dst->addrlen) &&
		    !memcmp(a->sun_path, b->sun_path, fromlen - offsetof(struct sockaddr_un, sun_path)))
			return dst;
	}
	return NULL;
}

int afc_ctrl_iface_attach(struct dl_list *ctrl_dst, struct sockaddr_storage *from,
			  socklen_t fromlen)
{
	struct ctrl_client *dst;

	dst = afc_ctrl_find_dst(ctrl_dst, from, fromlen);
	if (dst == NULL) {
		dst = zalloc(sizeof(*dst));
		if (dst == NULL)
			return -1;
		memcpy(&dst->addr, from, fromlen);
		dst->addrlen = fromlen;
		dl_list_add(ctrl_dst, &dst->list);
	}

	return AFC_STATUS_SUCCESS;
}

void afc_ctrl_iface_detach(struct dl_list *ctrl_dst, struct sockaddr_storage *from,
			   socklen_t fromlen)
{
	struct ctrl_client *dst;

	dst = afc_ctrl_find_dst(ctrl_dst, from, fromlen);
	if (dst) {
		dl_list_del(&dst->list);
		free(dst);
	}
}

void afc_ctrl_iface_send(struct dl_list *ctrl_dst, int cli_sock,
			 const char *buf, size_t len)
{
	struct ctrl_client *dst, *next;

	dl_list_for_each_safe(dst, next, ctrl_dst, struct ctrl_client, list) {
		if (sendto(cli_sock, buf, len, 0, (struct sockaddr *)&dst->addr,
			   dst->addrlen) < 0) {
			afc_printf(MSG_DEBUG, "CTRL_IFACE sendto failed: %s",
				   strerror(errno));
			dst->errors++;
			if (dst->errors > 10 || errno == ENOENT)
				afc_ctrl_iface_detach(ctrl_dst,
						      &dst->addr, dst->addrlen);
		} else {
			dst->errors = 0;
		}
	}
}

static void afc_ctrl_iface_receive(int sock, void *eloop_ctx, void *sock_ctx)
{
	int status;
	int ret, reply_len = 0, confidential_reply = 0;
	char buffer[256], *buf = buffer, reply[256];
	struct sockaddr_storage from;
	socklen_t fromlen = sizeof(from);
	struct dl_list *ctrl_dst = (struct dl_list *)eloop_ctx;

	UNUSED_PARAM(sock_ctx);

	ret = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
		       (struct sockaddr *)&from, &fromlen);
	if (ret < 0) {
		afc_printf(MSG_ERROR, "recvfrom error : %d", ret);
		return;
	}

	buf[ret] = '\0';

	if (!strncmp(buf, "confidential_reply ", 19)) {
		buf += 19;
		confidential_reply = 1;
	}

	afc_printf(MSG_ERROR, "received data from client: %s", buf);

	if (!strcmp(buf, "AFC_SEND_SPECTRUM_REQUEST")) {
		status = afc_query_server();
		if (status)
			reply_len = snprintf(reply, sizeof(reply), "FAILURE");
		else
			reply_len = snprintf(reply, sizeof(reply), "SUCCESS");
	} else if (!strcmp(buf, "ATTACH")) {
		reply_len = afc_ctrl_iface_attach(ctrl_dst, &from, fromlen);
		if (!reply_len)
			reply_len = snprintf(reply, sizeof(reply), "OK");
		else
			reply_len = snprintf(reply, sizeof(reply), "RETRY");
		sendto(sock, reply, reply_len, 0, (struct sockaddr *)&from, fromlen);
		return;
	} else if (!strcmp(buf, "DETACH")) {
		afc_ctrl_iface_detach(ctrl_dst, &from, fromlen);
		reply_len = snprintf(reply, sizeof(reply), "OK");
		sendto(sock, reply, reply_len, 0, (struct sockaddr *)&from, fromlen);
		return;
	} else {
		reply_len = snprintf(reply, sizeof(reply), "Invalid command");
		sendto(sock, reply, reply_len, 0, (struct sockaddr *)&from, fromlen);
		return;
	}

	if (confidential_reply) {
		sendto(sock, reply, reply_len, 0, (struct sockaddr *)&from, fromlen);
	} else {
		afc_ctrl_iface_send(ctrl_dst, sock, reply, reply_len);
	}
}

enum afc_status afc_cli_ctrl_iface_init(int *cli_sock, struct sockaddr_un *cli_addr,
					struct dl_list *ctrliface_dst_list)
{
	afc_printf(MSG_INFO, "initializing control interface");
	if (afc_ctrl_iface_init(cli_sock, cli_addr, AFCD_SOCKET_PATH) < 0)
		return AFC_STATUS_FAILURE;

	if (eloop_register_read_sock(*cli_sock, afc_ctrl_iface_receive, ctrliface_dst_list, NULL) < 0) {
		afc_printf(MSG_ERROR, "eloop register read sock failed");
		return AFC_STATUS_FAILURE;
	}

	return AFC_STATUS_SUCCESS;
}

void afc_cli_ctrl_iface_deinit(int *cli_sock, struct sockaddr_un *cli_addr)
{
	if (*cli_sock > 0)
		eloop_unregister_read_sock(*cli_sock);

	afc_ctrl_iface_deinit(cli_sock, cli_addr->sun_path);
}

int main(int argc, char *argv[])
{
	int cli_sock, c;
	struct sockaddr_un cli_addr;
	struct dl_list ctrliface_dst_list;

	for (;;) {
		c = getopt(argc, argv, "d:");
		if (c < 0)
			break;
		switch (c) {
		case 'd':
			afc_debug_level = atoi(optarg);
		default:
			break;
		}
	}

	if (afc_nl80211_init()) {
		afc_printf(MSG_ERROR, "netlink init failed");
		return AFC_STATUS_FAILURE;
	}

	if (eloop_init() != 0) {
		afc_printf(MSG_ERROR, "failed to initialize eloop");
		return AFC_STATUS_FAILURE;
	}

	dl_list_init(&ctrliface_dst_list);

	if (afc_cli_ctrl_iface_init(&cli_sock, &cli_addr, &ctrliface_dst_list)) {
		afc_printf(MSG_ERROR, "AFC ctrl interface init failed");
		afc_cli_ctrl_iface_deinit(&cli_sock, &cli_addr);
	}

	afc_query_server();
	eloop_run();

	afc_ctrl_iface_free(&ctrliface_dst_list);
	afc_cli_ctrl_iface_deinit(&cli_sock, &cli_addr);
	afc_nl80211_cleanup();
	eloop_destroy();

	return AFC_STATUS_SUCCESS;
}