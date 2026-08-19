/******************************************************************************
 *
 *  Copyright (c) 2021 - 2024 MaxLinear, Inc.
 *  Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
#  include "pon_config.h"
#endif

#include "pon_ip_msg.h"
#include "fapi_pon.h"
#include "fapi_pon_os.h"
#include "fapi_pon_core.h"
#include "fapi_pon_error.h"
#include "fapi_pon_debug.h"

enum pon_debug_level pon_dbg_lvl = PON_DBG_WRN;

/*
 * NetLink policy, this is used to check if the received data has the correct
 * types.
 */
static struct nla_policy pon_mbox_genl_policy[PON_MBOX_A_MAX + 1] = {
	[PON_MBOX_A_READ_WRITE] = { .type = NLA_U8 },
	[PON_MBOX_A_COMMAND] = { .type = NLA_U16 },
	[PON_MBOX_A_ACK] = { .type = NLA_U8 },
	[PON_MBOX_A_DATA] = { .type = NLA_UNSPEC },
	[PON_MBOX_A_MODE] = { .type = NLA_U8 },
	[PON_MBOX_A_CNT] = { .type = NLA_NESTED },
	[PON_MBOX_A_SRDS_READ] = { .type = NLA_NESTED },
	[PON_MBOX_A_DP_CONFIG] = { .type = NLA_NESTED },
};

/*
 * Netlink callback handler which gets called in case a Netlink error message
 * is received. This mostly indicates an internal error in the mbox driver.
 */
static int cb_error_handler(struct sockaddr_nl *nla,
			    struct nlmsgerr *nlerr, void *arg)
{
	struct read_cmd_cb *cb_data = arg;

	UNUSED(nla);

	cb_data->running = 0;
	switch (nlerr->error) {
	case -ENOMEM:
		cb_data->err = PON_STATUS_MBOX_ENOMEM;
		break;
	case -EINVAL:
		cb_data->err = PON_STATUS_MBOX_EINVAL;
		break;
	case -ENODEV:
		cb_data->err = PON_STATUS_MBOX_ENODEV;
		break;
	default:
		cb_data->err = PON_STATUS_MBOX_ERR;
		break;
	}
	return NL_OK;
}

/*
 * Netlink callback handler which skips all messages except the one with the
 * correct sequence number. We normally search for the sequence number of the
 * answer.
 */
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

enum fapi_pon_errorcode pon_handle_error_default(uint8_t ack)
{
	switch (ack) {
	case PONFW_NACK:
		PON_DEBUG_MSG("Got NACK from firmware");
		return PON_STATUS_FW_NACK;
	case PONFW_NACK_DUP:
		PON_DEBUG_MSG("Got NACK from firmware, duplicate entry");
		return PON_STATUS_FW_DUP_ERR;
	case PONFW_NACK_DBG:
		PON_DEBUG_WRN("Got NACK from firmware, debug deactivated");
		return PON_STATUS_FW_DBG;
	case PONFW_NACK_STATE:
		PON_DEBUG_MSG("Got NACK from firmware, wrong state");
		return PON_STATUS_FW_STATE;
	case PONFW_CMDERR:
		PON_DEBUG_ERR("Got CMD Error from firmware");
		return PON_STATUS_FW_CMDERR;
	default:
		PON_DEBUG_ERR("Got unknown error code from firmware: %i", ack);
		return PON_STATUS_FW_CMDERR;
	}
}

/*
 * Netlink callback handler which gets called for a valid message. This handler
 * checks if we really have a message from the PON MBOX driver and then calls
 * the callback functions to copy the data. In case of an error received from
 * the FW it either calls a specific callback handler to handle this or the
 * generic handler.
 */
static int cb_valid_handler(struct nl_msg *msg, void *arg)
{
	struct read_cmd_cb *cb_data = arg;
	struct nlattr *attrs[PON_MBOX_A_MAX + 1];
	struct genlmsghdr *header;
	void *buf = NULL;
	size_t buf_len = 0;
	int ret;
	uint8_t ack;

	ret = genlmsg_parse(nlmsg_hdr(msg), 0, attrs, PON_MBOX_A_MAX,
			    pon_mbox_genl_policy);
	if (ret < 0) {
		PON_DEBUG_ERR("can not parse netlink message: %i", ret);
		cb_data->err = PON_STATUS_NL_ERR;
		cb_data->running = 0;
		return NL_STOP;
	}

	header = nlmsg_data(nlmsg_hdr(msg));

	if (header->cmd != PON_MBOX_C_MSG) {
		PON_DEBUG_ERR("Got unknown netlink message: 0x%x", header->cmd);
		cb_data->err = PON_STATUS_NL_MSG;
		cb_data->running = 0;
		return NL_STOP;
	}

	ack = nla_get_u8(attrs[PON_MBOX_A_ACK]);
	if (ack == PONFW_ACK) {
		cb_data->err = PON_STATUS_OK;
	} else {
		if (cb_data->error_cb)
			cb_data->err = cb_data->error_cb(cb_data->ctx,
							 ack, cb_data->priv);
		else
			cb_data->err = pon_handle_error_default(ack);
		cb_data->running = 0;
		return NL_STOP;
	}

	if (cb_data->copy) {
		if (attrs[PON_MBOX_A_DATA]) {
			buf = nla_data(attrs[PON_MBOX_A_DATA]);
			buf_len = nla_len(attrs[PON_MBOX_A_DATA]);
		}
		cb_data->err = cb_data->copy(cb_data->ctx, buf, buf_len,
					     cb_data->priv);
	} else if (cb_data->decode) {
		cb_data->err = cb_data->decode(cb_data->ctx, attrs,
					       cb_data->priv);
	}
	cb_data->running = 0;

	return NL_OK;
}

/*
 * Netlink callback handler which overwrites the internal nl_recv() function.
 * We use this to forward an error code in case the socket read timed out.
 */
static int fapi_pon_nl_ow_recv(struct nl_sock *sk, struct sockaddr_nl *nla,
			       unsigned char **buf, struct ucred **creds)
{
	int ret;

	ret = nl_recv(sk, nla, buf, creds);
	if (!ret)
		return -EAGAIN;
	return ret;
}

/*
 * Create and send a message to the mailbox driver which contains a message
 * for the FW. The in_buf is optional if we have a message without a payload
 * set it to NULL. 'flags' attribute allows to distinguish whether called event
 * is fake or not.
 */
static enum fapi_pon_errorcode
fapi_pon_send_msg_int(struct nl_sock *nls, int family, uint32_t *seq,
		      uint32_t read, uint32_t command, uint32_t ack,
		      const void *in_buf, size_t in_size, uint8_t msg_type,
		      uint32_t flags)
{
	struct nl_msg *msg;
	struct nlmsghdr *nlh;
	void *nl_hdr;
	int ret;

	msg = nlmsg_alloc();
	if (!msg) {
		PON_DEBUG_ERR("Can't alloc netlink message");
		return PON_STATUS_NL_ERR;
	}

	nl_hdr = genlmsg_put(msg, 0, *seq, family, 0, 0, msg_type, 0);
	if (!nl_hdr) {
		PON_DEBUG_ERR("Can't generate message");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	ret = nla_put_u8(msg, PON_MBOX_A_READ_WRITE, read ? 1 : 0);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	ret = nla_put_u16(msg, PON_MBOX_A_COMMAND, command);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	ret = nla_put_u8(msg, PON_MBOX_A_ACK, ack);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	if (flags) {
		ret = nla_put_u32(msg, PON_MBOX_A_FLAGS, flags);
		if (ret) {
			PON_DEBUG_ERR("Can't add netlink attribute");
			nlmsg_free(msg);
			return PON_STATUS_NL_ERR;
		}
	}

	if (in_buf) {
		ret = nla_put(msg, PON_MBOX_A_DATA, in_size, in_buf);
		if (ret) {
			PON_DEBUG_ERR("Can't add netlink attribute");
			nlmsg_free(msg);
			return PON_STATUS_NL_ERR;
		}
	}

	ret = nl_send_auto_complete(nls, msg);
	if (ret < 0) {
		PON_DEBUG_ERR("Can't send netlink message: %i", ret);
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}
	nlh = nlmsg_hdr(msg);
	if (nlh)
		*seq = nlh->nlmsg_seq;

	nlmsg_free(msg);

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_open(struct pon_ctx **param)
{
	struct pon_ctx *ctx;
	int ret;
	int i;
#if defined(LINUX) && !defined(PON_LIB_SIMULATOR)
	int nl_sock;
	struct timeval timeout = {
		.tv_sec = 2,
		.tv_usec = 0,
	};
#endif

	if (!param)
		return PON_STATUS_INPUT_ERR;

	ctx = calloc(1, sizeof(*ctx));
	if (!ctx)
		return PON_STATUS_MEM_ERR;

	ctx->nls = nl_socket_alloc();
	if (!ctx->nls) {
		PON_DEBUG_ERR("Can't alloc netlink socket");
		fapi_pon_close(ctx);
		return PON_STATUS_NL_ERR;
	}

	ret = genl_connect(ctx->nls);
	if (ret) {
		PON_DEBUG_ERR("Can't connect to netlink socket: %i", ret);
		fapi_pon_close(ctx);
		return PON_STATUS_NL_ERR;
	}

	ctx->family = genl_ctrl_resolve(ctx->nls, PON_MBOX_FAMILY);
	if (ctx->family < 0) {
		PON_DEBUG_ERR("No pon mbox netlink interface found: %i",
				ctx->family);
		fapi_pon_close(ctx);
		return PON_STATUS_NL_NAME_ERR;
	}

#if defined(LINUX) && !defined(PON_LIB_SIMULATOR)
	/* We set a socket timeout of 2 seconds here. We assume that the FW can
	 * answer to all request within 2 seconds. The default nl_recv()
	 * function just retries the reading in case it ran into a timeout,
	 * we have to overwrite it in \ref fapi_pon_send_msg.
	 */
	nl_sock = nl_socket_get_fd(ctx->nls);

	ret = setsockopt(nl_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout,
			 sizeof(timeout));
	if (ret) {
		PON_DEBUG_ERR("setsockopt failed with: %i", errno);
		fapi_pon_close(ctx);
		return PON_STATUS_ERR;
	}
#endif

	for (i = 0; i < PON_DDMI_MAX; i++)
		ctx->eeprom_fd[i] = -1;

	*param = ctx;
	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_close(struct pon_ctx *ctx)
{
	int i;

	for (i = 0; i < PON_DDMI_MAX; i++) {
		if (ctx->eeprom_fd[i] >= 0)
			pon_close(ctx->eeprom_fd[i]);
	}

	nl_socket_free(ctx->nls);
	nl_socket_free(ctx->nls_event);
	free(ctx);

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_nl_msg_prepare_decode(struct pon_ctx *ctx,
			       struct nl_msg **msg,
			       struct read_cmd_cb *cb_data,
			       uint32_t *seq,
			       fapi_pon_decode decode,
			       fapi_pon_error error_cb,
			       void *copy_priv,
			       uint8_t msg_type)
{
	void *nl_hdr;
	struct pon_ctx *context = ctx;

	if (!ctx || !cb_data)
		return PON_STATUS_INPUT_ERR;

	cb_data->running = 1;
	cb_data->copy = NULL;
	cb_data->error_cb = error_cb;
	cb_data->priv = copy_priv;
	cb_data->ctx = ctx;
	cb_data->decode = decode;

	*msg = nlmsg_alloc();
	if (!(*msg)) {
		PON_DEBUG_ERR("Can't alloc netlink message");
		return PON_STATUS_NL_ERR;
	}

	nl_hdr = genlmsg_put(*msg, 0, *seq, context->family, 0, 0, msg_type, 0);
	if (!nl_hdr) {
		PON_DEBUG_ERR("Can't generate message");
		nlmsg_free(*msg);
		return PON_STATUS_NL_ERR;
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_nl_msg_prepare(struct pon_ctx *ctx,
						struct nl_msg **msg,
						struct read_cmd_cb *cb_data,
						uint32_t *seq,
						fapi_pon_copy copy,
						fapi_pon_error error_cb,
						void *copy_priv,
						uint8_t msg_type)
{
	void *nl_hdr;
	struct pon_ctx *context = ctx;

	cb_data->running = 1;
	cb_data->copy = copy;
	cb_data->error_cb = error_cb;
	cb_data->priv = copy_priv;
	cb_data->ctx = ctx;
	cb_data->decode = NULL;

	*msg = nlmsg_alloc();
	if (!(*msg)) {
		PON_DEBUG_ERR("Can't alloc netlink message");
		return PON_STATUS_NL_ERR;
	}

	nl_hdr = genlmsg_put(*msg, 0, *seq, context->family, 0, 0, msg_type, 0);
	if (!nl_hdr) {
		PON_DEBUG_ERR("Can't generate message");
		nlmsg_free(*msg);
		return PON_STATUS_NL_ERR;
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_nl_msg_send(struct pon_ctx *ctx,
					     struct nl_msg **msg,
					     struct read_cmd_cb *cb_data,
					     uint32_t *seq)
{
	struct nlmsghdr *nlh;
	struct nl_cb *cb;
	struct nl_cb *orig;
	struct pon_ctx *context = ctx;
	int ret;

	orig = nl_socket_get_cb(context->nls);
	cb = nl_cb_clone(orig);
	nl_cb_put(orig);
	if (!cb) {
		PON_DEBUG_ERR("Can't allocate new callback struct");
		nlmsg_free(*msg);
		return PON_STATUS_NL_ERR;
	}

	ret = nl_send_auto_complete(context->nls, *msg);
	if (ret < 0) {
		PON_DEBUG_ERR("Can't send netlink message: %i", ret);
		nl_cb_put(cb);
		nlmsg_free(*msg);
		return PON_STATUS_NL_ERR;
	}

	nlh = nlmsg_hdr(*msg);
	if (nlh)
		*seq = nlh->nlmsg_seq;

	nlmsg_free(*msg);

	nl_cb_set(cb, NL_CB_SEQ_CHECK, NL_CB_CUSTOM, pon_seq_check, seq);
	nl_cb_err(cb, NL_CB_CUSTOM, cb_error_handler, cb_data);
	nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, cb_valid_handler, cb_data);

	/*
	 * Overwrite the nl_recv() function. This functions returns 0 in case
	 * of an error and the return value is forward in the next functions.
	 * To get to know if everything is fine or if we ran into an error
	 * return a negative value in case of an error instead. An error is
	 * for example the timeout of 2 seconds was reached.
	 */
	nl_cb_overwrite_recv(cb, fapi_pon_nl_ow_recv);

	while (cb_data->running == 1) {
		ret = nl_recvmsgs(context->nls, cb);
		if (ret) {
			cb_data->err = PON_STATUS_TIMEOUT;
			break;
		}
	}

	nl_cb_put(cb);

	return (*cb_data).err;
}

/*
 * This sends a NetLink message to the mbox driver and waits for the answer.
 * \ref fapi_pon_send_msg_int is used to create the message and send it. This
 * function also registers all the callback handlers needed to handle the
 * answer. If no pon_ctx is given this function will create a context on its
 * own and deletes it again after using it.
 */
static enum fapi_pon_errorcode fapi_pon_send_msg(struct pon_ctx *ctx,
				     uint32_t read, uint32_t command,
				     uint32_t ack, const void *in_buf,
				     size_t in_size, fapi_pon_copy copy,
				     fapi_pon_error error_cb,
				     void *copy_priv, uint8_t msg_type)
{
	int ret;
	uint32_t seq = NL_AUTO_SEQ;
	enum fapi_pon_errorcode err;
	struct nl_cb *cb;
	struct nl_cb *orig;
	struct read_cmd_cb cb_data = {
		.running = 1,
		.copy = copy,
		.error_cb = error_cb,
		.priv = copy_priv,
		.ctx = ctx,
	};

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	err = fapi_pon_send_msg_int(ctx->nls, ctx->family, &seq, read, command,
				    ack, in_buf, in_size, msg_type, 0);
	if (err != PON_STATUS_OK)
		return err;

	orig = nl_socket_get_cb(ctx->nls);
	cb = nl_cb_clone(orig);
	nl_cb_put(orig);
	if (!cb) {
		PON_DEBUG_ERR("Can't allocate new callback struct");
		return PON_STATUS_NL_ERR;
	}

	nl_cb_set(cb, NL_CB_SEQ_CHECK, NL_CB_CUSTOM, pon_seq_check, &seq);
	nl_cb_err(cb, NL_CB_CUSTOM, cb_error_handler, &cb_data);
	nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, cb_valid_handler, &cb_data);

	/*
	 * Overwrite the nl_recv() function. This functions returns 0 in case
	 * of an error and the return value is forward in the next functions.
	 * To get to know if everything is fine or if we ran into an error
	 * return a negative value in case of an error instead. An error is
	 * for example the timeout of 2 seconds was reached.
	 */
	nl_cb_overwrite_recv(cb, fapi_pon_nl_ow_recv);

	while (cb_data.running == 1) {
		ret = nl_recvmsgs(ctx->nls, cb);
		if (ret) {
			cb_data.err = PON_STATUS_TIMEOUT;
			break;
		}
	}

	nl_cb_put(cb);
	return cb_data.err;
}

/*
 * This sends an answer to a previous NetLink message. This is used to send
 * an ACK or NACK to an event received from the firmware.
 */
enum fapi_pon_errorcode
fapi_pon_send_msg_answer(struct pon_ctx *ctx, struct nl_msg *msg,
			 struct nlattr **attrs, uint32_t ack, void *buf,
			 size_t size, uint8_t msg_type)
{
	struct nlmsghdr *hdr = nlmsg_hdr(msg);
	uint16_t command;
	uint8_t read;
	uint32_t flags = 0;

	command = nla_get_u16(attrs[PON_MBOX_A_COMMAND]);
	read = nla_get_u8(attrs[PON_MBOX_A_READ_WRITE]);

	if (attrs[PON_MBOX_A_FLAGS])
		flags = nla_get_u32(attrs[PON_MBOX_A_FLAGS]);

	/*
	 * Send it on normal socket and not the multicast socket. In case
	 * there is a problem the answer would be revived by the event handler
	 * which does not handle them, send them to the normal socket which
	 * will ignore it. The normal netlink socket should ignores the
	 * answers as it ignores all messages with unexpected sequence numbers.
	 */
	return fapi_pon_send_msg_int(ctx->nls, ctx->family,
				     &hdr->nlmsg_seq, read, command, ack,
				     buf, size, msg_type, flags);
}

#if defined(INCLUDE_DEBUG_SUPPORT)
uint32_t pon_debug_print(const enum pon_debug_level level,
			 const char *format, ...)
{
	uint32_t ret = 0;
	va_list ap;

	if ((level < PON_DBG_OFF) && (level >= pon_dbg_lvl)) {
		va_start(ap, format);
		ret = vprintf(format, ap);
		if (ret > 0)
			ret = printf(PON_CRLF);
		va_end(ap);
	}
	return ret;
}
#endif

enum fapi_pon_errorcode fapi_pon_generic_error_get(struct pon_ctx *ctx,
						   uint32_t command,
						   const void *in_buf,
						   size_t in_size,
						   fapi_pon_copy copy,
						   fapi_pon_error error_cb,
						   void *copy_priv,
						   uint8_t msg_type)
{
	return fapi_pon_send_msg(ctx, PONFW_READ, command, PONFW_CMD, in_buf,
				 in_size, copy, error_cb, copy_priv, msg_type);
}

enum fapi_pon_errorcode fapi_pon_generic_error_set(struct pon_ctx *ctx,
						   uint32_t command,
						   const void *param,
						   uint32_t sizeof_param,
						   fapi_pon_error error_cb,
						   void *copy_priv,
						   uint8_t msg_type)
{
	return fapi_pon_send_msg(ctx, PONFW_WRITE, command, PONFW_CMD, param,
				 sizeof_param, NULL, error_cb, copy_priv,
				 msg_type);
}

/*
 * This function gets called whenever a new NetLink message is received on the
 * event group. based on the received message this function then calls
 * different functions to handle the specific message.
 */
static int fapi_pon_listener_rec(struct nl_msg *msg, void *arg)
{
	int ret;
	struct nlattr *attrs[PON_MBOX_A_MAX + 1];
	struct genlmsghdr *header;
	struct pon_ctx *ctx = arg;
	uint16_t command;

	ret = genlmsg_parse(nlmsg_hdr(msg), 0, attrs, PON_MBOX_A_MAX,
			    pon_mbox_genl_policy);
	if (ret < 0)
		return ret;

	header = nlmsg_data(nlmsg_hdr(msg));

	switch (header->cmd) {
	case PON_MBOX_C_MSG:
		if (!attrs[PON_MBOX_A_COMMAND])
			return -EINVAL;

		command = nla_get_u16(attrs[PON_MBOX_A_COMMAND]);
		fapi_pon_listener_msg(command, ctx, msg, attrs);
		break;
	case PON_MBOX_C_RESET:
		PON_DEBUG_ERR("mailbox reset was requested");
		break;
	case PON_MBOX_C_FW_INIT_COMPLETE:
		fapi_pon_fw_init_complete_msg(ctx, msg, attrs);
		break;
	default:
		PON_DEBUG_ERR("got unknown command: 0x%x", header->cmd);
		return -EINVAL;
	}

	return 0;
}

/*
 * This function overwrites the nl_recv() function like
 * \ref fapi_pon_nl_ow_recv does. This function should terminate in case the
 * program using it receives a signal. This makes it possible to gracefully
 * shutdown the application.
 */
static int fapi_pon_listener_recv(struct nl_sock *sk, struct sockaddr_nl *nla,
				  unsigned char **buf, struct ucred **creds)
{
	fd_set rfds;
	int ret;
	int s_fd = nl_socket_get_fd(sk);

	if (s_fd < 0)
		return -NLE_BAD_SOCK;

	FD_ZERO(&rfds);
	FD_SET(s_fd, &rfds);

	ret = select(s_fd + 1, &rfds, NULL, NULL, NULL);
	if (ret  == -1)
		return nl_syserr2nlerr(errno);
	if (ret == 0)
		return -NLE_INTR;

	return nl_recv(sk, nla, buf, creds);
}

enum fapi_pon_errorcode fapi_pon_listener_connect(struct pon_ctx *ctx,
						  void *priv)
{
	enum fapi_pon_errorcode err;
	int msg_grp;
	int ret;
	struct nl_cb *s_cb;

	ctx->priv = priv;

	ctx->nls_event = nl_socket_alloc();
	if (!ctx->nls_event) {
		PON_DEBUG_ERR("can not alloc netlink socket");
		return PON_STATUS_NL_ERR;
	}

	ret = genl_connect(ctx->nls_event);
	if (ret) {
		PON_DEBUG_ERR("can not connect to netlink socket: %i", ret);
		err = PON_STATUS_NL_ERR;
		goto out_nl_socket_free;
	}

	nl_socket_disable_seq_check(ctx->nls_event);

	ret = nl_socket_modify_cb(ctx->nls_event, NL_CB_VALID, NL_CB_CUSTOM,
				  fapi_pon_listener_rec, ctx);
	if (ret) {
		PON_DEBUG_ERR("can not add netlink callback: %i", ret);
		err = PON_STATUS_NL_ERR;
		goto out_nl_socket_free;
	}

	msg_grp = genl_ctrl_resolve_grp(ctx->nls_event, PON_MBOX_FAMILY, "msg");
	if (msg_grp < 0) {
		PON_DEBUG_ERR("cannot find netlink group: %i", msg_grp);
		err = PON_STATUS_NL_NAME_ERR;
		goto out_nl_socket_free;
	}

	ret = nl_socket_add_membership(ctx->nls_event, msg_grp);
	if (ret < 0) {
		PON_DEBUG_ERR("can not join netlink group: %i", ret);
		err = PON_STATUS_NL_ERR;
		goto out_nl_socket_free;
	}

	s_cb = nl_socket_get_cb(ctx->nls_event);
	nl_cb_overwrite_recv(s_cb, fapi_pon_listener_recv);
	nl_cb_put(s_cb);

	return PON_STATUS_OK;

out_nl_socket_free:
	nl_socket_free(ctx->nls_event);
	ctx->nls_event = NULL;
	return err;
}

enum fapi_pon_errorcode fapi_pon_listener_run(struct pon_ctx *ctx)
{
	int ret;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	ret = nl_recvmsgs_default(ctx->nls_event);
	if (ret == -NLE_INTR)
		return PON_STATUS_OK;

	if (ret == -NLE_NOMEM) {
		/*
		 * When the user space is not fast enough with receiving
		 * NetLink messages the kernel will return an -ENOBUFS in the
		 * recvmsg syscall, libnl translates this to -NLE_NOMEM. This
		 * behavior is also described in the NetLink man page. We will
		 * ignore this error and will let the calling application just
		 * try it again.
		 */
		PON_DEBUG_WRN("NetLink ENOMEM, some FW events are lost");
		return PON_STATUS_OK;
	}
	if (ret < 0) {
		PON_DEBUG_ERR("problem receiving message: %i (%s)", ret,
			      nl_geterror(ret));
		return PON_STATUS_NL_ERR;
	}
	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_msg_prepare(struct pon_ctx **ctx,
					     struct nl_msg **msg,
					     uint8_t cmd)
{
	void *nl_hdr;

	*msg = nlmsg_alloc();
	if (!(*msg)) {
		PON_DEBUG_ERR("Can't alloc netlink message");
		return PON_STATUS_NL_ERR;
	}

	nl_hdr = genlmsg_put(*msg, 0, NL_AUTO_SEQ, (*ctx)->family, 0, 0,
			     cmd, 0);
	if (!nl_hdr) {
		PON_DEBUG_ERR("Can't generate message");
		nlmsg_free(*msg);
		return PON_STATUS_NL_ERR;
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_reset(struct pon_ctx *ctx, enum pon_mode mode)
{
	int ret;
	enum fapi_pon_errorcode err;
	struct nl_msg *msg;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	err = fapi_pon_msg_prepare(&ctx, &msg, PON_MBOX_C_RESET_FULL);
	if (err != PON_STATUS_OK)
		return err;

	if (mode != PON_MODE_UNKNOWN) {
		ret = nla_put_u8(msg, PON_MBOX_A_MODE, mode);
		if (ret) {
			PON_DEBUG_ERR("Can't add netlink attribute");
			nlmsg_free(msg);
			return PON_STATUS_NL_ERR;
		}
	}

	ret = nl_send_auto_complete(ctx->nls, msg);
	nlmsg_free(msg);
	if (ret < 0) {
		PON_DEBUG_ERR("Can't send netlink message: %i", ret);
		return PON_STATUS_NL_ERR;
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_link_disable(struct pon_ctx *ctx)
{
	int ret;
	enum fapi_pon_errorcode err;
	struct nl_msg *msg;
	uint8_t pon_mode = 0;
	uint8_t cmd = PON_MBOX_C_LINK_DISABLE;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	err = fapi_pon_mode_get(ctx, &pon_mode);
	if (err != PON_STATUS_OK)
		return err;
	if (pon_mode == PON_MODE_AON)
		return PON_STATUS_SUPPORT;

	err = fapi_pon_msg_prepare(&ctx, &msg, cmd);
	if (err != PON_STATUS_OK)
		return err;

	ret = nl_send_auto_complete(ctx->nls, msg);
	nlmsg_free(msg);
	if (ret < 0) {
		PON_DEBUG_ERR("Can't send netlink message: %i", ret);
		return PON_STATUS_NL_ERR;
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_eeprom_open(struct pon_ctx *ctx,
					     const enum pon_ddmi_page ddmi_page,
					     const char *filename)
{
	if (!ctx || !filename)
		return PON_STATUS_INPUT_ERR;

	if (ddmi_page != PON_DDMI_A0 && ddmi_page != PON_DDMI_A2)
		return PON_STATUS_INPUT_ERR;

	if (ctx->eeprom_fd[ddmi_page] >= 0) {
		pon_close(ctx->eeprom_fd[ddmi_page]);
		ctx->eeprom_fd[ddmi_page] = -1;
	}

#ifdef HAVE_SOPEN_S
	_sopen_s(&ctx->eeprom_fd[ddmi_page], filename, PON_RDONLY, 0, 0);
#else
	ctx->eeprom_fd[ddmi_page] = pon_open(filename, PON_RDWR);
#endif
	if (ctx->eeprom_fd[ddmi_page] < 0) {
		PON_DEBUG_ERR("Couldn't open EEPROM file (file: %s, errno: %u)",
			      filename, errno);
		return PON_STATUS_EEPROM_OPEN_ERR;
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
	fapi_pon_eeprom_data_get(struct pon_ctx *ctx,
				 const enum pon_ddmi_page ddmi_page,
				 unsigned char *data,
				 long offset,
				 size_t data_size)
{
	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (ddmi_page != PON_DDMI_A0 && ddmi_page != PON_DDMI_A2)
		return PON_STATUS_INPUT_ERR;

	if (ctx->eeprom_fd[ddmi_page] < 0)
		return PON_STATUS_INPUT_ERR;

	if (pon_pread(ctx->eeprom_fd[ddmi_page], data, data_size, offset)
	    < (int)data_size) {
		return PON_STATUS_EEPROM_READ_ERR;
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
	fapi_pon_eeprom_data_set(struct pon_ctx *ctx,
				 const enum pon_ddmi_page ddmi_page,
				 unsigned char *data,
				 long offset,
				 size_t data_size)
{
	char buf[64];

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (ddmi_page != PON_DDMI_A0 && ddmi_page != PON_DDMI_A2)
		return PON_STATUS_INPUT_ERR;

	if (ctx->eeprom_fd[ddmi_page] < 0)
		return PON_STATUS_INPUT_ERR;

	if (pon_pwrite(ctx->eeprom_fd[ddmi_page], data, data_size, offset)
	    < (int)data_size) {
		pon_strerr(errno, buf, sizeof(buf));
		PON_DEBUG_ERR(
			"Couldn't write data to requested EEPROM file: %s",
			buf);
		return PON_STATUS_EEPROM_WRITE_ERR;
	}

	return PON_STATUS_OK;
}

void fapi_pon_dbg_level_set(const uint8_t level)
{
	pon_dbg_lvl = level;
}

uint8_t fapi_pon_dbg_level_get(void)
{
	return pon_dbg_lvl;
}

int get_clock_cycle_from_caps(struct pon_cap *caps)
{
	if (!caps)
		return 0;
	return GPON_CLOCK_CYCLE;
}
