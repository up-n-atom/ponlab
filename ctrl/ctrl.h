/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include "list.h"

#define DEFAULT_CTRL_WAIT_MSG_TIMEOUT 90
#define REPLY_LEN 4096
#define MAX_BIND_RETRY_CNT 5

struct afc_ctrl
{
	int soc;
	struct sockaddr_un local;
	struct sockaddr_un dest;
};

struct ctrl_client {
	struct dl_list list;
	struct sockaddr_storage addr;
	socklen_t addrlen;
	unsigned char errors;
};

int afc_ctrl_request(struct afc_ctrl *ctrl, const char *cmd, size_t cmd_len,
		     char *reply, size_t *reply_len,
		     void (*msg_cb)(char *msg, size_t len));
struct afc_ctrl *afc_ctrl_connect(char *src_path, char *dest_path);
void afc_ctrl_disconnect(struct afc_ctrl *ctrl);
int afc_ctrl_iface_init(int *cli_sock, struct sockaddr_un *cli_addr, char *src_path);
void afc_ctrl_iface_deinit(int *cli_sock, char *src_path);