/******************************************************************************

                 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/

#include <sys/stat.h>
#include "eloop.h"
#include "afc_debug.h"
#include "ctrl.h"

int afc_ctrl_request(struct afc_ctrl *ctrl, const char *cmd, size_t cmd_len,
		     char *reply, size_t *reply_len,
		     void (*msg_cb)(char *msg, size_t len))
{
	struct reltime started_at;
	struct timeval tv;
	fd_set rfds;
	int res;

	errno = 0;
	started_at.sec = 0;
        started_at.usec = 0;

retry_send:
	if (send(ctrl->soc, cmd, cmd_len, 0) < 0) {
		if (errno == EAGAIN || errno == EBUSY || errno == EWOULDBLOCK)
		{
			/*
			 * Must be a non-blocking socket... Try for a bit
			 * longer before giving up.
			 */
			if (started_at.sec == 0)
				get_reltime(&started_at);
			else {
				struct reltime n;
				get_reltime(&n);
				/* Try for a few seconds. */
				if (reltime_expired(&n, &started_at, 5))
					goto send_err;
			}
			sleep(1);
			goto retry_send;
		}
		if (errno == ENOTCONN || errno == ECONNREFUSED) {
			return errno;
		}
	send_err:
		return -1;
	}

	for (;;) {
		tv.tv_sec = 15;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ctrl->soc, &rfds);
		res = select(ctrl->soc + 1, &rfds, NULL, NULL, &tv);
		if (res < 0 && errno == EINTR)
			continue;
		if (res < 0)
			return res;
		if (FD_ISSET(ctrl->soc, &rfds)) {
			res = recv(ctrl->soc, reply, *reply_len, 0);
			if (res < 0)
				return res;
			if (res > 0 && reply[0] == '<') {
				/* This is an unsolicited message from
				 * afcd, not the reply to the request.
				 * Use msg_cb to report this to the
				 * caller. */
				if (msg_cb) {
					/* Make sure the message is null
					 * terminated. */
					if ((size_t) res == *reply_len)
						res = (*reply_len) - 1;
					reply[res] = '\0';
					msg_cb(reply, res);
				}
				continue;
			}
			*reply_len = res;
			break;
		} else {
			return -2;
		}
	}

	return 0;
}

struct afc_ctrl *afc_ctrl_connect(char *src_path, char *dest_path)
{
	struct afc_ctrl *ctrl;
	int flags, try_cnt = 0;

	if ((strlen(src_path) >= UNIX_PATH_MAX) ||
	    (strlen(dest_path) >= UNIX_PATH_MAX))
		return NULL;

	ctrl = zalloc(sizeof(*ctrl));
	if (!ctrl)
		return NULL;

	ctrl->local.sun_family = AF_UNIX;
	snprintf(ctrl->local.sun_path, UNIX_PATH_MAX - 1, "%s", src_path);

	ctrl->soc = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (ctrl->soc < 0) {
		free(ctrl);
		return NULL;
	}

try_again:
	if (bind(ctrl->soc, (struct sockaddr *)&ctrl->local, sizeof(ctrl->local)) < 0) {
		printf("control interface bind failed: %s\n", strerror(errno));
		/*
		 * The existing socket reference still lying around
		 * and must have been left by unclean termination
		 * of an earlier run. Remove the file and try again.
		 */
		unlink(ctrl->local.sun_path);
		if (try_cnt++ > MAX_BIND_RETRY_CNT)
			goto fail;

		goto try_again;
	}

	ctrl->dest.sun_family = AF_UNIX;
	snprintf(ctrl->dest.sun_path, UNIX_PATH_MAX - 1, "%s", dest_path);

	if (connect(ctrl->soc, (struct sockaddr *)&ctrl->dest, sizeof(ctrl->dest)) < 0) {
		printf("control interface connect failed: %s\n", strerror(errno));
		goto fail;
	}

	/* Make socket non-blocking */
	flags = fcntl(ctrl->soc, F_GETFL);
	if (flags >= 0) {
		flags |= O_NONBLOCK;
		if (fcntl(ctrl->soc, F_SETFL, flags) < 0) {
			perror("fcntl(soc, O_NONBLOCK)");
			/* Not fatal, continue on.*/
		}
	}

	return ctrl;
fail:
	unlink(ctrl->local.sun_path);
	close(ctrl->soc);
	free(ctrl);
	return NULL;
}

void afc_ctrl_disconnect(struct afc_ctrl *ctrl)
{
	if (!ctrl)
		return;

        unlink(ctrl->local.sun_path);
        if (ctrl->soc >= 0)
                close(ctrl->soc);
        free(ctrl);
}

int afc_ctrl_iface_init(int *cli_sock, struct sockaddr_un *cli_addr, char *src_path)
{
	int len, try_cnt = 0;

	*cli_sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (*cli_sock < 0) {
		afc_printf(MSG_ERROR, "failed to create control socket");
		return -1;
	}

	memset(cli_addr, 0, sizeof(*cli_addr));
	cli_addr->sun_family = AF_UNIX;
	len = snprintf(cli_addr->sun_path, sizeof(cli_addr->sun_path), "%s", src_path);
	if (len < 0 || len >= (int)sizeof(cli_addr->sun_path))
		goto fail;;

try_again:
	if (bind(*cli_sock, (struct sockaddr *)cli_addr, sizeof(*cli_addr)) < 0) {
		afc_printf(MSG_ERROR, "control interface bind failed: %s", strerror(errno));

		unlink(cli_addr->sun_path);

		if (try_cnt++ > MAX_BIND_RETRY_CNT)
			goto fail;

		goto try_again;
	}

	afc_printf(MSG_ERROR, "control interface bind success");

	if (chmod(cli_addr->sun_path, S_IRWXU | S_IRWXG) < 0) {
		afc_printf(MSG_ERROR, "chmod ctrl_iface failed: %s", strerror(errno));
		goto fail;
	}

	return 0;
fail:
	unlink(cli_addr->sun_path);
	if (*cli_sock >= 0)
		close(*cli_sock);
	*cli_sock = -1;

	return -1;
}

void afc_ctrl_iface_deinit(int *cli_sock, char *src_path)
{
	if (*cli_sock < 0)
		return;

	unlink(src_path);
	close(*cli_sock);
	*cli_sock = 0;
}