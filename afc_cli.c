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
#include "ctrl.h"
#include "process.h"
#include "eloop.h"
#include "afc.h"

#define AFC_CLI_SOCKET_PATH "/tmp/afc_cli_ctrl_"
#define AFC_CLI_MAX_ARGUMENT_LIMIT 5

struct afc_cli_cmd {
	const char *cmd;
	int (*handler)(struct afc_ctrl *ctrl, int argc, char *argv[]);
	const char *usage;
};

static const struct afc_cli_cmd cli_cmds[];
static char req_confidential_reply;
static int interactive;

static void afc_cli_msg_cb(char *msg, size_t len)
{
	printf("len:%zu, msg:%s\n", len, msg);
}

static int afc_cli_ctrl_cmd(struct afc_ctrl *ctrl, char *cmd, int clen)
{
	char buf[512], cmd_t[256];
	size_t len;
	int ret;

	if (!ctrl) {
		printf("not connected to afcd - command dropped.\n");
		return -1;
	}

	if (req_confidential_reply) {
		clen = snprintf(cmd_t, sizeof(cmd_t), "confidential_reply %s", cmd);
		cmd = cmd_t;
	}

	len = sizeof(buf) - 1;

	ret = afc_ctrl_request(ctrl, cmd, clen, buf, &len,
			       afc_cli_msg_cb);
	if (ret == -2) {
		printf("'%s' command timed out.\n", cmd);
		return -2;
	} else if (ret < 0) {
		printf("'%s' command failed.\n", cmd);
		return -1;
	}

	buf[len] = '\0';
	printf("%s\n", buf);

	return 0;
}

void cmd_usage(const char *cmd)
{
	int n;

	printf("commands:\n");
	for (n = 0; cli_cmds[n].cmd; n++) {
		if (!strcmp(cli_cmds[n].usage, ""))
			continue;
		if (!cmd || !strncasecmp(cli_cmds[n].cmd, cmd, strlen(cli_cmds[n].cmd)))
			printf("  %s %s\n", cli_cmds[n].cmd, cli_cmds[n].usage);
	}
}

static int afc_cli_help(struct afc_ctrl *ctrl, int argc, char *argv[])
{
	UNUSED_PARAM(ctrl);

	cmd_usage(argc > 0 ? argv[0] : NULL);
	return 0;
}

static int afc_cli_send_spectrum_req(struct afc_ctrl *ctrl, int argc, char *argv[])
{
	char cmd[64] = {0};
	int ret, clen = 0;

	UNUSED_PARAM(argc);
	UNUSED_PARAM(argv);

	clen = snprintf(cmd, sizeof(cmd), "AFC_SEND_SPECTRUM_REQUEST");
	ret = afc_cli_ctrl_cmd(ctrl, cmd, clen);
	if (ret < 0) {
		printf("unable to send spectrum_request\n");
		return ret;
	}

	return 0;
}

static int afc_cli_quit(struct afc_ctrl *ctrl, int argc, char *argv[])
{
	UNUSED_PARAM(ctrl);
	UNUSED_PARAM(argc);
	UNUSED_PARAM(argv);

	if (interactive)
		eloop_terminate();

	return 0;
}

static const struct afc_cli_cmd cli_cmds[] = {
	{ "help", afc_cli_help, "= show command usage" },
	{ "afc_send_spectrum_request", afc_cli_send_spectrum_req, "= send spectrum request to afc server" },
	{ "quit", afc_cli_quit, "= exit from afcd_cli interactive session" },
	{ NULL, NULL, NULL }
};

int afc_exe_cli_cmd(struct afc_ctrl *ctrl, int argc, char *argv[])
{
	const struct afc_cli_cmd *cmd = cli_cmds;

	while (cmd->cmd) {
		if (strcasecmp(cmd->cmd, argv[0]) == 0) {
			return cmd->handler(ctrl, argc - 1, &argv[1]);
		}
		cmd++;
	}

	printf("unknown command '%s', try 'help'\n", argv[0]);
	return -1;
}

int tokenize_cmd(char *cmd, char *argv[])
{
	int argc = 0;
	char *temp = strtok(cmd, " ");

	while (temp != NULL) {
		if (argc == AFC_CLI_MAX_ARGUMENT_LIMIT)
			return -1;
		argv[argc] = temp;
		temp = strtok(NULL, " ");
		argc++;
	}
	return argc;
}

static void afc_cli_eof_cb(int sig, void *ctx)
{
	UNUSED_PARAM(sig);
	UNUSED_PARAM(ctx);

	eloop_terminate();
}

static void afc_cli_cmd_cb(void *ctx, char *cmd, struct afc_ctrl *ctrl)
{
	char *argv[AFC_CLI_MAX_ARGUMENT_LIMIT];
	int argc;

	UNUSED_PARAM(ctx);

	argc = tokenize_cmd(cmd, argv);
	if (argc == -1) {
		printf("maximum argument limit reached\n");
		return;
	}

	if (argc)
		afc_exe_cli_cmd(ctrl, argc, &argv[0]);
}

static void cli_ctrl_iface_receive(int sock, void *priv, void *user_data)
{
	struct sockaddr_storage from;
	socklen_t fromlen = sizeof(from);
	char buffer[4096], *buf = buffer;
	int res = 0;

	UNUSED_PARAM(priv);
	UNUSED_PARAM(user_data);

	res = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
		       (struct sockaddr *)&from, &fromlen);

	if (res < 0) {
		printf("receive from ctrl_iface failed\n");
		return;
	}

	buf[res] = '\0';
	printf("%s\n", buf);
}

int afc_attach_interface(struct afc_ctrl *ctrl)
{
	char buf[8];
	int ret, len;

	len = snprintf(buf, sizeof(buf), "ATTACH");

	ret = afc_cli_ctrl_cmd(ctrl, buf, len);
	if (ret < 0) {
		printf("unable to attach with afcd\n");
		return ret;
	}

	eloop_register_read_sock(ctrl->soc, cli_ctrl_iface_receive, ctrl, NULL);
	return 0;
}

int afc_detach_interface(struct afc_ctrl *ctrl)
{
	char buf[8];
	int ret, len;

	len = snprintf(buf, sizeof(buf), "DETACH");

	ret = afc_cli_ctrl_cmd(ctrl, buf, len);
	if (ret < 0) {
		printf("unable to detach from afcd\n");
		return ret;
	}

	eloop_unregister_read_sock(ctrl->soc);

	return 0;
}

int main(int argc, char *argv[])
{
	struct afc_ctrl *ctrl;
	char local_path[UNIX_PATH_MAX];
	char dest_path[UNIX_PATH_MAX];

	if (argc == 1)
		interactive = 1;

	snprintf(local_path, sizeof(local_path), AFC_CLI_SOCKET_PATH "%d", (int) getpid());
	snprintf(dest_path, sizeof(dest_path), AFCD_SOCKET_PATH);

	ctrl = afc_ctrl_connect(local_path, dest_path);
	if (!ctrl) {
		printf("failed to connect with afcd\n");
		return -1;
	}

	if (interactive) {
		eloop_init();
		if (afc_attach_interface(ctrl)) {
			printf("failed to attach with afcd\n");
			afc_ctrl_disconnect(ctrl);
			return -1;
		}

		eloop_register_signal_terminate(afc_cli_eof_cb, NULL);
		command_init(afc_cli_cmd_cb, afc_cli_eof_cb, ctrl, NULL);

		eloop_run();

		afc_detach_interface(ctrl);
		command_deinit();
		eloop_destroy();
	} else {
		req_confidential_reply = 1;
		afc_exe_cli_cmd(ctrl, argc - optind, &argv[optind]);
	}

	afc_ctrl_disconnect(ctrl);

	return 0;
}
