/*****************************************************************************
 *
 * Copyright (c) 2020 - 2021 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 * Copyright (c) 2016 Lantiq Beteiligungs-GmbH & Co. KG
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <errno.h>

#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>

#include <pon/pon_mbox.h>

/** Timeout after 2 seconds when no answer to a FW message was received. */
#define ANSWER_TIMEOUT	2

static struct nla_policy pon_mbox_genl_policy[PON_MBOX_A_MAX + 1] = {
	[PON_MBOX_A_READ_WRITE] = { .type = NLA_U8 },
	[PON_MBOX_A_COMMAND] = { .type = NLA_U16 },
	[PON_MBOX_A_ACK] = { .type = NLA_U8 },
	[PON_MBOX_A_DATA] = { .type = NLA_UNSPEC },
	[PON_MBOX_A_REG] = { .type = NLA_U8 },
	[PON_MBOX_A_REG_VAL] = { .type = NLA_U32 },
};

static int print_msg(struct nl_msg *msg, void *arg)
{
	int i;
	int ret;
	struct nlattr *attrs[PON_MBOX_A_MAX + 1];
	uint32_t *buf;
	int buf_len;
	int buf_bytes;
	struct genlmsghdr *header;

	ret = genlmsg_parse(nlmsg_hdr(msg), 0, attrs, PON_MBOX_A_MAX,
			    pon_mbox_genl_policy);
	if (ret < 0)
		return ret;

	header = nlmsg_data(nlmsg_hdr(msg));

	switch (header->cmd) {
	case PON_MBOX_C_MSG:
		if (!attrs[PON_MBOX_A_DATA])
			return -EINVAL;

		buf = nla_data(attrs[PON_MBOX_A_DATA]);
		buf_bytes = nla_len(attrs[PON_MBOX_A_DATA]);
		buf_len = buf_bytes / sizeof(uint32_t);

		printf("errorcode=0 recv_msg_length=%i ", buf_len);
		printf("message=");
		for (i = 0; i < buf_len; i++)
			printf("%08x ", buf[i]);
		printf("\n");
		break;
	case PON_MBOX_C_RESET:
		printf("mailbox reset was requested\n");
		break;
	default:
		fprintf(stderr, "got unknown command: 0x%x\n", header->cmd);
		return -EINVAL;
	}

	return 0;
}

static int send_msg(struct nl_sock *nls, int family, bool write, int command,
		    int ack, void *data, size_t data_len, uint32_t *seq)
{
	struct nl_msg *msg;
	struct nlmsghdr *nlh;
	int ret;
	void *nl_hdr;

	msg = nlmsg_alloc();
	if (!msg) {
		fprintf(stderr, "can not alloc netlink msg\n");
		return -ENOMEM;
	}

	nl_hdr = genlmsg_put(msg, 0, NL_AUTO_SEQ, family, 0, 0,
			     PON_MBOX_C_MSG, 0);
	if (!nl_hdr) {
		fprintf(stderr, "can not generate message\n");
		ret = -ENOMEM;
		goto free_msg;
	}

	ret = nla_put_u8(msg, PON_MBOX_A_READ_WRITE, write ? 0 : 1);
	if (ret) {
		fprintf(stderr, "can not add nl attribute\n");
		goto free_msg;
	}

	ret = nla_put_u16(msg, PON_MBOX_A_COMMAND, command);
	if (ret) {
		fprintf(stderr, "can not add nl attribute\n");
		goto free_msg;
	}

	ret = nla_put_u8(msg, PON_MBOX_A_ACK, ack);
	if (ret) {
		fprintf(stderr, "can not add nl attribute\n");
		goto free_msg;
	}

	if (data_len > 0) {
		ret = nla_put(msg, PON_MBOX_A_DATA, data_len, data);
		if (ret) {
			fprintf(stderr, "can not add nl attribute\n");
			goto free_msg;
		}
	}

	ret = nl_send_auto_complete(nls, msg);
	if (ret < 0)
		fprintf(stderr, "can not send netlink msg: %i\n", ret);

	nlh = nlmsg_hdr(msg);
	if (nlh)
		*seq = nlh->nlmsg_seq;

free_msg:
	nlmsg_free(msg);
	return ret;
}

struct read_cmd_cb {
	int err;
	size_t len;
	unsigned char **out;
};

static int cb_error_handler(struct sockaddr_nl *nla, struct nlmsgerr *nlerr,
			    void *arg)
{
	struct read_cmd_cb *cb_data = arg;

	cb_data->err = nlerr->error;
	return NL_STOP;
}

static int cb_valid_handler(struct nl_msg *msg, void *arg)
{
	struct read_cmd_cb *cb_data = arg;
	struct nlattr *attrs[PON_MBOX_A_MAX + 1];
	char *buf;
	int buf_len;
	struct genlmsghdr *header;

	cb_data->err = genlmsg_parse(nlmsg_hdr(msg), 0, attrs, PON_MBOX_A_MAX,
				     pon_mbox_genl_policy);
	if (cb_data->err < 0)
		return NL_STOP;

	cb_data->err = 0;

	header = nlmsg_data(nlmsg_hdr(msg));

	if (header->cmd != PON_MBOX_C_MSG) {
		cb_data->err = -EINVAL;
		fprintf(stderr, "got unknown command: 0x%x\n", header->cmd);
		return NL_STOP;
	}

	/* in case no data should be read */
	if (!cb_data->out)
		return NL_STOP;

	if (!attrs[PON_MBOX_A_DATA]) {
		cb_data->out = NULL;
		return NL_STOP;
	}

	buf = nla_data(attrs[PON_MBOX_A_DATA]);
	buf_len = nla_len(attrs[PON_MBOX_A_DATA]);

	*cb_data->out = malloc(buf_len);
	if (!*cb_data->out) {
		cb_data->err = -ENOMEM;
		return NL_SKIP;
	}

	memcpy(*cb_data->out, buf, buf_len);
	cb_data->len = buf_len;

	return NL_STOP;
}

static int pon_seq_check(struct nl_msg *msg, void *arg)
{
	uint32_t *seq_expect = arg;
	struct nlmsghdr *nlh;

	nlh = nlmsg_hdr(msg);

	if (!seq_expect || !nlh)
		return NL_SKIP;

	if (*seq_expect == nlh->nlmsg_seq)
		return NL_OK;

	return NL_SKIP;
}

/**
 * Netlink callback handler which overwrites the internal nl_recv() function.
 * We use this to forward an error code in case the socket read timed out.
 */
static int pon_nl_ow_recv(struct nl_sock *sk, struct sockaddr_nl *nla,
			       unsigned char **buf, struct ucred **creds)
{
	int ret;

	ret = nl_recv(sk, nla, buf, creds);
	if (!ret)
		return -EAGAIN;
	return ret;
}

static int issue_cmd(struct nl_sock *nls, int family, bool write, int command,
		     void *in, size_t in_len, unsigned char **out)
{
	int ret;
	struct nl_cb *cb;
	struct read_cmd_cb cb_data = {
		.err = 1,
		.len = 0,
		.out = out,
	};
	uint32_t seq = 0;
	int nl_sock;
	struct timeval timeout = {
		.tv_sec = ANSWER_TIMEOUT,
		.tv_usec = 0,
	};

	ret = send_msg(nls, family, write, command, 0, in, in_len, &seq);
	if (ret < 0)
		return ret;

	cb = nl_socket_get_cb(nls);
	if (!cb) {
		fprintf(stderr, "can not clone existing callback handle\n");
		return -ENOMEM;
	}

	nl_cb_set(cb, NL_CB_SEQ_CHECK, NL_CB_CUSTOM, pon_seq_check, &seq);
	nl_cb_err(cb, NL_CB_CUSTOM, cb_error_handler, &cb_data);
	nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, cb_valid_handler, &cb_data);

	/* We set a socket timeout of 2 seconds here. We assume that the FW can
	 * answer to all request within 2 seconds. The default nl_recv()
	 * function just retries the reading in case it ran into a timeout,
	 * we have to overwrite it.
	 */
	nl_sock = nl_socket_get_fd(nls);

	ret = setsockopt(nl_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout,
			 sizeof(timeout));
	if (ret) {
		fprintf(stderr, "setsockopt() failed with %d\n", ret);
		nl_cb_put(cb);
		return ret;
	}

	/*
	 * Overwrite the nl_recv() function. This functions returns 0 in case
	 * of an error and the return value is forward in the next functions.
	 * To get to know if everything is fine or if we ran into an error
	 * return a negative value in case of an error instead. An error is
	 * for example the timeout of 2 seconds was reached.
	 */
	nl_cb_overwrite_recv(cb, pon_nl_ow_recv);

	while (cb_data.err == 1) {
		ret = nl_recvmsgs(nls, cb);
		if (ret) {
			cb_data.err = -1;
			break;
		}
	}

	nl_cb_put(cb);

	return cb_data.err ? cb_data.err : cb_data.len;
}

static int cb_valid_reg_read_handler(struct nl_msg *msg, void *arg)
{
	struct read_cmd_cb *cb_data = arg;
	struct nlattr *attrs[PON_MBOX_A_MAX + 1];
	uint32_t buf;
	struct genlmsghdr *header;

	cb_data->err = genlmsg_parse(nlmsg_hdr(msg), 0, attrs, PON_MBOX_A_MAX,
				     pon_mbox_genl_policy);
	if (cb_data->err < 0)
		return NL_STOP;

	cb_data->err = 0;

	header = nlmsg_data(nlmsg_hdr(msg));

	if (header->cmd != PON_MBOX_C_REG_READ) {
		cb_data->err = -EINVAL;
		fprintf(stderr, "got unknown command: 0x%x\n", header->cmd);
		return NL_STOP;
	}

	buf = nla_get_u32(attrs[PON_MBOX_A_REG_VAL]);

	printf("errorcode=0 reg=0x%x\n", buf);

	return NL_STOP;
}

static int reg_read(struct nl_sock *nls, int family, int reg)
{
	int ret;
	struct nl_msg *msg;
	void *nl_hdr;
	struct nl_cb *cb;
	struct read_cmd_cb cb_data = {
		.err = 1,
	};

	msg = nlmsg_alloc();
	if (!msg) {
		fprintf(stderr, "can not alloc netlink msg\n");
		return -ENOMEM;
	}

	nl_hdr = genlmsg_put(msg, 0, NL_AUTO_SEQ, family, 0, 0,
			     PON_MBOX_C_REG_READ, 0);
	if (!nl_hdr) {
		fprintf(stderr, "can not generate message\n");
		nlmsg_free(msg);
		return -ENOMEM;
	}

	ret = nla_put_u8(msg, PON_MBOX_A_REG, reg);
	if (ret) {
		fprintf(stderr, "can not add nl attribute\n");
		nlmsg_free(msg);
		return ret;
	}

	ret = nl_send_auto_complete(nls, msg);
	if (ret < 0)
		fprintf(stderr, "can not send netlink msg: %i\n", ret);

	nlmsg_free(msg);

	cb = nl_socket_get_cb(nls);
	if (!cb) {
		fprintf(stderr, "can not clone existing callback handle\n");
		return -ENOMEM;
	}

	nl_cb_err(cb, NL_CB_CUSTOM, cb_error_handler, &cb_data);
	nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, cb_valid_reg_read_handler,
		  &cb_data);

	while (cb_data.err == 1)
		nl_recvmsgs(nls, cb);

	nl_cb_put(cb);

	return cb_data.err ? cb_data.err : 0;
}

static int reg_write(struct nl_sock *nls, int family, int reg, uint32_t value)
{
	int ret;
	struct nl_msg *msg;
	void *nl_hdr;

	msg = nlmsg_alloc();
	if (!msg) {
		fprintf(stderr, "can not alloc netlink msg\n");
		return -ENOMEM;
	}

	nl_hdr = genlmsg_put(msg, 0, NL_AUTO_SEQ, family, 0, 0,
			     PON_MBOX_C_REG_WRITE, 0);
	if (!nl_hdr) {
		fprintf(stderr, "can not generate message\n");
		nlmsg_free(msg);
		return -ENOMEM;
	}

	ret = nla_put_u8(msg, PON_MBOX_A_REG, reg);
	if (ret) {
		fprintf(stderr, "can not add nl attribute\n");
		nlmsg_free(msg);
		return ret;
	}

	ret = nla_put_u32(msg, PON_MBOX_A_REG_VAL, value);
	if (ret) {
		fprintf(stderr, "can not add nl attribute\n");
		nlmsg_free(msg);
		return ret;
	}

	ret = nl_send_auto_complete(nls, msg);
	if (ret < 0)
		fprintf(stderr, "can not send netlink msg: %i\n", ret);

	nlmsg_free(msg);

	return 0;
}

static const struct option long_options[] = {
	{"listen",	no_argument,		0, 'l'},
	{"command",	required_argument,	0, 'c'},
	{"write",	no_argument,		0, 'w'},
	{"data",	required_argument,	0, 'd'},
	{"reset",	no_argument,		0, 'r'},
	{"reset_full",	no_argument,		0, 'f'},
	{"reg_set",	required_argument,	0, 's'},
	{"reg_get",	required_argument,	0, 'g'},
	{"help",	no_argument,		0, 'h'},
	{NULL,		0,			0,  0 },
};

static void print_help(char *prog)
{
	int i;

	printf("%s <options> <data>: Pon Mailbox command line interface\n\n",
		prog);
	printf("<data> is an optional list of 32 bit words (hex or dec) containing the payload.\n");
	for (i = 0; long_options[i].name != NULL; i++) {
		printf("--%-10s  -%c  %s\n",
		       long_options[i].name,
		       long_options[i].val,
		       (long_options[i].has_arg == required_argument)
		       ? "<value>" : "");
	}
	printf("Example: \"ponmbox -c 96 -w 0x00015430 0x00000005\"\n");
}

int main(int argc, char **argv)
{
	int opt;
	int option_index;
	bool listen = false;
	int command = 0; /* 0 is an invalid cmd */
	bool write = false; /* default to read */
	uint32_t *data = NULL;
	size_t data_len = 0;
	bool reg_get = false;
	bool reg_set = false;
	int reg = 0;
	int i;
	char *endptr;

	struct nl_sock *nls;
	int ret;
	int family;

	int msg_grp;

	while ((opt = getopt_long(argc, argv, "lc:wd:rfs:g:h",
				  long_options, &option_index)) != -1) {
		switch (opt) {
		case 'l':
			listen = true;
			break;
		case 'c':
			command = strtoll(optarg, &endptr, 0);
			if (errno != 0) {
				ret = errno;
				perror("command");
				goto out_data_free;
			}
			if (endptr == optarg) {
				ret = -EINVAL;
				fprintf(stderr,
					"No digits were found in command\n");
				goto out_data_free;
			}
			break;
		case 'w':
			write = true;
			break;
		case 'd':
			printf("deprecated - just append the data\n");
			if (data)
				free(data);

			data = malloc(4);
			if (!data)
				return -ENOMEM;

			*data = strtoll(optarg, &endptr, 0);
			if (errno != 0) {
				ret = errno;
				perror("data");
				goto out_data_free;
			}
			if (endptr == optarg) {
				ret = -EINVAL;
				fprintf(stderr,
					"No digits were found in data\n");
				goto out_data_free;
			}
			data_len = 4;
			break;
		case 'r':
			fprintf(stderr, "This command has been deleted. To reset the PON mailbox use higher layer functions.\n");
			break;
		case 'f':
			fprintf(stderr, "This command has been deleted. To reset the PON IP hardware use higher layer functions.\n");
			break;
		case 'g':
			reg_get = true;
			reg = strtoll(optarg, &endptr, 0);
			if (errno != 0) {
				ret = errno;
				perror("reg_get");
				goto out_data_free;
			}
			if (endptr == optarg) {
				ret = -EINVAL;
				fprintf(stderr,
					"No digits were found in reg_get\n");
				goto out_data_free;
			}
			break;
		case 's':
			reg_set = true;
			reg = strtoll(optarg, &endptr, 0);
			if (errno != 0) {
				ret = errno;
				perror("reg_set");
				goto out_data_free;
			}
			if (endptr == optarg) {
				ret = -EINVAL;
				fprintf(stderr,
					"No digits were found in reg_set\n");
				goto out_data_free;
			}
			break;
		case 'h':
			print_help(argv[0]);
			ret = 0;
			goto out_data_free;
		default:
			fprintf(stderr, "invalid parameters\n");
			print_help(argv[0]);
			ret = -EINVAL;
			goto out_data_free;
		}
	}

	if (optind < argc) {
		int optremaining = argc - optind;

		if (data)
			free(data);

		data = malloc(4 * optremaining);
		if (!data)
			return -ENOMEM;

		for (i = 0; i < optremaining; i++) {
			data[i] = strtoll(argv[optind + i], &endptr, 0);
			if (errno != 0) {
				ret = errno;
				perror("data");
				goto out_data_free;
			}
		}

		data_len = 4 * optremaining;
	}

	nls = nl_socket_alloc();
	if (!nls) {
		fprintf(stderr, "can not alloc netlink socket\n");
		ret = -ENOMEM;
		goto out_data_free;
	}

	ret = genl_connect(nls);
	if (ret) {
		fprintf(stderr,
			"can not connect to netlink socket: %i\n", ret);
		goto out_nl_socket_free;
	}

	family = genl_ctrl_resolve(nls, PON_MBOX_FAMILY);
	if (family < 0) {
		fprintf(stderr, "no pon_mbox netlink interface found: %i\n",
			family);
		goto out_nl_socket_free;
	}

	if (reg_get) {
		ret = reg_read(nls, family, reg);
		goto out_nl_socket_free;
	}

	if (reg_set && data) {
		ret = reg_write(nls, family, reg, *data);
		goto out_nl_socket_free;
	}

	if (command) {
		uint32_t *out = NULL;
		int out_len;

		ret = issue_cmd(nls, family, write, command, data, data_len,
				(unsigned char **)&out);
		if (ret < 0) {
			printf("errorcode=%d\n", ret);
			goto out_nl_socket_free;
		}

		if (!out) {
			printf("errorcode=0\n");
			goto out_nl_socket_free;
		}
		out_len = ret / sizeof(uint32_t);

		printf("errorcode=0 recv_msg_length=%i ", ret);
		printf("message=");
		for (i = 0; i < out_len; i++)
			printf("%08x ", out[i]);
		printf("\n");
	}

	if (listen) {
		nl_socket_disable_seq_check(nls);

		ret = nl_socket_modify_cb(nls, NL_CB_VALID, NL_CB_CUSTOM,
					  print_msg, NULL);
		if (ret) {
			fprintf(stderr, "can not add netlink callback: %i\n",
				ret);
			goto out_nl_socket_free;
		}

		msg_grp = genl_ctrl_resolve_grp(nls, PON_MBOX_FAMILY, "msg");
		if (msg_grp < 0) {
			fprintf(stderr, "cannot find netlink group: %i\n",
				msg_grp);
			goto out_nl_socket_free;
		}

		ret = nl_socket_add_membership(nls, msg_grp);
		if (ret < 0) {
			fprintf(stderr, "can  not join netlink group: %i\n",
				ret);
			goto out_nl_socket_free;
		}

		while (1) {
			ret = nl_recvmsgs_default(nls);
			if (ret < 0) {
				fprintf(stderr,
					"problem receiving message: %i\n",
					ret);
				goto out_nl_socket_free;
			}
		}
	}

out_nl_socket_free:
	nl_socket_free(nls);
out_data_free:
	free(data);
	return (ret < 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
