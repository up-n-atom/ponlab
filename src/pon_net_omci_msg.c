/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <pon_adapter_errno.h>
#include <omci/pon_adapter_msg.h>

#include "pon_net_common.h"
#include "pon_net_debug.h"

#include <linux/if_ether.h>
#include <sys/socket.h>

#ifndef __PACKED__
#define __PACKED__ __attribute__((packed))
#endif

#ifdef LINUX
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/if_packet.h>
#endif

/* OUI Extended Ethertype (IEEE Std 802) */
#define ETH_P_OUI		0x88B7

#define OMCI_MAX_LENGTH		1980

/* broadcast mac address */
static uint8_t bc_mac[ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

struct oui_protocol {
	unsigned char val[5];
	unsigned short length;
} __PACKED__;

static struct oui_protocol oui_prot_omci = {
	.val = { 0x00, 0x19, 0xA7, 0x00, 0x02 }
};

/**
 * PON Net Lib OMCC socket handle
 */
struct pon_net_omcc_socket {
	/** network interface name for this socket */
	char if_name[IF_NAMESIZE];
	/** interface index */
	int if_index;
	/** MAC address of the interface */
	uint8_t if_mac[ETH_ALEN];
	/** file descriptor for the socket */
	int sock_fd;
};

static struct pon_net_omcc_socket *omcc_socket_create(const char *if_name)
{
#ifdef LINUX
	struct pon_net_omcc_socket *omcc_socket;
	struct ifreq if_temp = { { {0} }, { {0} } };

	dbg_in_args("%s", if_name);

	omcc_socket = malloc(sizeof(*omcc_socket));
	if (!omcc_socket)
		goto exit;

	memset(omcc_socket, 0, sizeof(*omcc_socket));

	omcc_socket->sock_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_OUI));
	if (omcc_socket->sock_fd < 0) {
		dbg_err("socket(): %s\n", strerror(errno));
		goto exit;
	}

	snprintf(if_temp.ifr_name, sizeof(if_temp.ifr_name), "%s", if_name);

	/* set if_temp.ifr_index */
	if (ioctl(omcc_socket->sock_fd, SIOCGIFINDEX, &if_temp) < 0) {
		dbg_err("ioctl SIOCGIFINDEX: %s\n",
			strerror(errno));
		goto exit;
	}

	snprintf(omcc_socket->if_name, sizeof(omcc_socket->if_name), "%s",
		 if_name);

	omcc_socket->if_index = if_temp.ifr_ifindex;

	if (ioctl(omcc_socket->sock_fd, SIOCGIFFLAGS, &if_temp) < 0) {
		dbg_err("ioctl SIOCGIFFLAGS: %s\n",
			strerror(errno));
		goto exit;
	}

	if_temp.ifr_flags |= IFF_PROMISC;
	if (ioctl(omcc_socket->sock_fd, SIOCSIFFLAGS, &if_temp) < 0) {
		dbg_err("ioctl SIOCSIFFLAGS: %s\n",
			strerror(errno));
		goto exit;
	}

	/* Bind to device */
	if (setsockopt(omcc_socket->sock_fd, SOL_SOCKET, SO_BINDTODEVICE,
		       if_temp.ifr_name, IFNAMSIZ - 1) < 0) {
		dbg_err("setsockopt SO_BINDTODEVICE: %s\n",
			strerror(errno));
		goto exit;
	}

	if (ioctl(omcc_socket->sock_fd, SIOCGIFHWADDR, &if_temp) < 0) {
		dbg_err("ioctl SIOCGIFHWADDR: %s\n",
			strerror(errno));
		goto exit;
	}

	if (memcpy_s(omcc_socket->if_mac, ETH_ALEN,
				if_temp.ifr_hwaddr.sa_data, ETH_ALEN)) {
		dbg_err_fn(memcpy_s);
		goto exit;
	}

	dbg_out_ret("%p", omcc_socket);
	return omcc_socket;

exit:
	/* this will be reached only in error case */
	if (omcc_socket) {
		if (omcc_socket->sock_fd >= 0) {
			shutdown(omcc_socket->sock_fd, SHUT_RDWR);
			close(omcc_socket->sock_fd);
		}
		free(omcc_socket);
	}
	dbg_out_ret("%p", NULL);
#else
	UNUSED(if_name);
#endif
	return NULL;
}

static void omcc_socket_clear(struct pon_net_omcc_socket *omcc_socket)
{
	if (!omcc_socket)
		return;

	if (omcc_socket->sock_fd >= 0) {
#ifdef LINUX
		shutdown(omcc_socket->sock_fd, SHUT_RDWR);
		close(omcc_socket->sock_fd);
#else
		closesocket(omcc_socket->sock_fd);
#endif
	}
	free(omcc_socket);
}

static int omcc_socket_wait(struct pon_net_omcc_socket *omcc_socket,
			    const unsigned int timeout_msec)
{
	int ret = -1;
#ifdef LINUX
	fd_set rfds;
	struct timeval tv;

	dbg_in_args("%p, %d", omcc_socket, timeout_msec);

	tv.tv_sec = (time_t)(timeout_msec / 1000);
	tv.tv_usec = ((suseconds_t)(timeout_msec % 1000)) * 1000;
	FD_ZERO(&rfds);
	FD_SET(omcc_socket->sock_fd, &rfds);

	ret = select(omcc_socket->sock_fd + 1, &rfds, NULL, NULL, &tv);

	dbg_out_ret("%d", ret);
#else
	UNUSED(omcc_socket);
	UNUSED(timeout_msec);
#endif
	return ret;
}

#ifdef LINUX
/**
 *  Filter packets to be handled by this module.
 *
 *  \param eh		Ethernet header reference
 *  \param src_addr	Physical-layer address reference
 *  \param op		Oui_protocol reference
 *  \param src_mac	Source mac address
 *  \param dst_mac	Destination mac address
 *
 *  \return		1 if data is correct or 0 otherwise
 */
static int recv_packet_match(const struct ethhdr *eh,
			     const struct sockaddr_ll *src_addr,
			     const struct oui_protocol *op,
			     const unsigned char *src_mac,
			     const unsigned char *dst_mac)
{
	int ret = 1;

	dbg_in_args("%p, %p, %p, %p, %p", eh, src_addr, op, src_mac, dst_mac);

	/* is this a broadcast packet? */
	if (!memcmp(eh->h_dest, bc_mac, ETH_ALEN)) {
		/* ignore broadcasts sent by ourself */
		if (src_mac && memcmp(eh->h_source, src_mac, ETH_ALEN))
			ret = 0;
	} else {
		/* non matching dst address */
		if (dst_mac && memcmp(eh->h_dest, dst_mac, ETH_ALEN))
			ret = 0;
	}

	/* non matching src address */
	if (ret && (src_mac && memcmp(src_addr->sll_addr, src_mac,
				      src_addr->sll_halen)))
		ret = 0;

	/* wrong ethertype */
	if (ret && (eh->h_proto != htons(ETH_P_OUI)))
		ret = 0;

	/* not an oui proto packet */
	if (ret && (memcmp(&oui_prot_omci, op, sizeof(oui_prot_omci.val))))
		ret = 0;

	dbg_out_ret("%d", ret);
	return ret;
}
#endif

/**
 *  Extract omci_msg and mic/crc from message buffer.
 *
 *  \param buf		Buffer containing entire message
 *  \param maxlen	Maximum length of omci_msg
 *  \param op		Oui_protocol reference containing omci_msg length
 *  \param omci_msg	Pointer to buffer for omci_msg
 *  \param mic		Pointer to buffer for mic/crc
 *
 *  \return		Length of omci_msg
 */
static int omci_msg_mic_extract(const unsigned char *buf,
				const uint16_t maxlen,
				const struct oui_protocol *op,
				void *omci_msg,
				uint32_t *mic)
{
	uint16_t len = 0;

	dbg_in_args("%p, %d, %p, %p, %p", buf, maxlen, op, omci_msg, mic);

	if (!op) {
		len = 0;
		goto exit;
	}

	/* length without mic, as it is handled separately */
	if (ntohs(op->length) <= sizeof(*mic))
		len = 0;
	else
		len = (uint16_t)(ntohs(op->length) - sizeof(*mic));

	if (omci_msg && len < maxlen) {
		if (memcpy_s(omci_msg, maxlen, buf, len)) {
			dbg_err_fn(memcpy_s);
			len = 0;
			goto exit;
		}
		buf += len;
		if (mic)
			if (memcpy_s(mic, sizeof(*mic), buf, sizeof(*mic))) {
				dbg_err_fn(memcpy_s);
				len = 0;
				goto exit;
			}

	} else {
		len = 0;
	}

exit:
	dbg_out_ret("%d", len);
	return len;
}

static int omcc_socket_receive(struct pon_net_omcc_socket *omcc_socket,
			       void *omci_msg,
			       const uint16_t maxlen,
			       uint32_t *mic,
			       const unsigned char *src_mac,
			       const unsigned char *dst_mac)
{
	int length = 0;
#ifdef LINUX
	struct sockaddr_ll src_addr;
	unsigned char buffer[OMCI_MAX_LENGTH];
	struct oui_protocol op;
	struct ethhdr eh;
	struct iovec iov[3];
	struct msghdr mh;

	dbg_in_args("%p, %p, %d, %p, %p, %p", omcc_socket, omci_msg, maxlen,
					      mic, src_mac, dst_mac);

	iov[0].iov_base = &eh;
	iov[0].iov_len = sizeof(eh);
	iov[1].iov_base = &op;
	iov[1].iov_len = sizeof(op);
	iov[2].iov_base = &buffer;
	iov[2].iov_len = OMCI_MAX_LENGTH;

	mh.msg_name = &src_addr;
	mh.msg_namelen = sizeof(src_addr);
	mh.msg_iov = iov;
	mh.msg_iovlen = 3;

	length = (int)recvmsg(omcc_socket->sock_fd, &mh, MSG_DONTWAIT);

	if (length <= 0) {
		if (length < 0)
			dbg_err("omcc_socket_recv: recvmsg(): %s\n",
				strerror(errno));
	} else {
		if (!recv_packet_match(&eh, &src_addr, &op,
				       src_mac, dst_mac))
			length = -1;
	}

	if (length > 0)
		length = omci_msg_mic_extract(buffer, maxlen, &op,
					      omci_msg, mic);

	dbg_out_ret("%d", length);
#else
	UNUSED(omcc_socket);
	UNUSED(dst_mac);
	UNUSED(src_mac);
	UNUSED(mic);
	UNUSED(maxlen);
	UNUSED(omci_msg);
#endif
	return length;
}

static int ethhdr_set(struct pon_net_omcc_socket *omcc_socket,
		       struct ethhdr *eh,
		       const unsigned char *src_mac,
		       const unsigned char *dst_mac)
{
	int ret = 0;

	dbg_in_args("%p, %p, %p, %p", omcc_socket, eh, src_mac, dst_mac);

	/* Broadcast message if destination mac is not set */
	if (dst_mac) {
		if (memcpy_s(eh->h_dest, ETH_ALEN, dst_mac, ETH_ALEN)) {
			dbg_err_fn(memcpy_s);
			ret = 1;
			goto exit;
		}
	} else
		if (memcpy_s(eh->h_dest, ETH_ALEN, bc_mac, ETH_ALEN)) {
			dbg_err_fn(memcpy_s);
			ret = 1;
			goto exit;
		}

	/* Use mac from netif if source mac is not set */
	if (src_mac) {
		if (memcpy_s(eh->h_source, ETH_ALEN, src_mac, ETH_ALEN)) {
			dbg_err_fn(memcpy_s);
			ret = 1;
			goto exit;
		}
	} else
		if (memcpy_s(eh->h_source, ETH_ALEN,
					omcc_socket->if_mac, ETH_ALEN)) {
			dbg_err_fn(memcpy_s);
			ret = 1;
			goto exit;
		}

	eh->h_proto = htons(ETH_P_OUI);

exit:
	dbg_out_ret("%d", ret);
	return ret;
}

/**
 *  Message handling thread
 *
 *  \param arg        Thread arguments
 */
static void *socket_thread_main(void *arg)
{
	const struct pon_net_omcc_context *omcc_ctx = arg;
	struct pon_net_omcc_socket *omcc_socket = omcc_ctx->omcc_socket;
	uint8_t msg[OMCI_MAX_LENGTH];
	uint32_t crc_mic = 0;
	int ret, length;
	enum pon_adapter_errno error;
	int err;

	dbg_in_args("%p", arg);
#ifdef LINUX
	dbg_msg("SocketThread - Started (pid %d)\n", getpid());
#endif

	err = pthread_setname_np(pthread_self(), "omccsocket");
	if (err)
		dbg_err("%s: Can't set <%s> name for a thread\n",
			__func__, "omccsocket");

	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	/* while thread is running */
	for (;;) {
		dbg_msg("SocketThread - Waiting for message...\n");

		pthread_testcancel();

		/* wait for OMCI msgs to receive */
		ret = omcc_socket_wait(omcc_socket,
				       SOCKET_THREAD_SHUTDOWN_WAIT / 2);
		if (ret <= 0)
			continue;

		/* receive the OMCI msg */
		length = omcc_socket_receive(omcc_socket, msg, sizeof(msg),
					     &crc_mic, omcc_ctx->p_mac,
					     NULL);
		/* no OMCI message received */
		if (length <= 0)
			continue;

		if (!omcc_ctx->receive_callback) {
			dbg_err("omci msg receive cb routines not defined\n");
			continue;
		}

		pthread_testcancel();

		error = omcc_ctx->receive_callback(omcc_ctx->hl_handle,
						   msg, (uint16_t)length,
						   &crc_mic);

		if (error != PON_ADAPTER_SUCCESS) {
			dbg_err("omci msg receive cb routines returned %d\n",
				error);
			continue;
		}
	}
	dbg_msg("SocketThread - Bye\n");

	dbg_out_ret("%d", 0);
	return 0;
}

static enum pon_adapter_errno
pon_net_omci_socket_thread_stop(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	static const char *th_name = "omccsocket";

	dbg_in_args("%p", ctx);

#ifndef WIN32
	if (!ctx->omcc_ctx.socket_thread) {
		dbg_out_ret("%d", ret);
		return ret;
	}
#endif
	ret = pon_net_thread_stop(&ctx->omcc_ctx.socket_thread, th_name,
				  SOCKET_THREAD_SHUTDOWN_WAIT / 1000);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err("attempt to cancel thread <%s> failed\n", th_name);

	dbg_out_ret("%d", ret);
	return ret;
}

/**
 *  Register the OMCI msg receive callback
 *
 *  \param ll_handle         low level handle.
 *  \param receive_callback  callback routine for OMCI msg receiving.
 *  \param hl_handle         high level handle to call callback routine.
 */
static enum pon_adapter_errno msg_rx_cb_register(void *ll_handle,
		enum pon_adapter_errno (*receive_callback)(void *hl_handle,
							   const uint8_t *msg,
							   const uint16_t len,
							   const uint32_t *crc),
		void *hl_handle)
{
	enum pon_adapter_errno error = PON_ADAPTER_SUCCESS;
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_omcc_context *omcc_ctx;
	int err;

	dbg_in_args("%p, %p, %p", ll_handle, receive_callback, hl_handle);

	if (!ctx) {
		error = PON_ADAPTER_ERROR;
		goto exit;
	}

	omcc_ctx = &ctx->omcc_ctx;
	omcc_ctx->p_mac = ctx->cfg.omcc_dev_mac;

	if (hl_handle)
		omcc_ctx->hl_handle = hl_handle;

	if (receive_callback)
		omcc_ctx->receive_callback = receive_callback;

	omcc_socket_clear(omcc_ctx->omcc_socket);

	omcc_ctx->omcc_socket = omcc_socket_create(GEM_OMCI_DEVICE);

	if (!omcc_ctx->omcc_socket) {
		dbg_err("OMCC socket create failed\n");
		error =  PON_ADAPTER_ERROR;
		goto exit;
	}

	if (error == PON_ADAPTER_SUCCESS) {
		err = pthread_create(&omcc_ctx->socket_thread,
				     NULL, socket_thread_main, omcc_ctx);
		if (err) {
			dbg_err("Can't create <%s> thread - error: %d\n",
				"omccsocket", err);
			error = PON_ADAPTER_ERROR;
			goto exit;
		}
	}

exit:
	dbg_out_ret("%d", error);
	return error;
}

/**
 *  Clear the OMCI msg receive callback
 *
 *  \param ll_handle         low level handle.
 *  \param hl_handle         high level handle to call callback routine.
 */
static enum pon_adapter_errno msg_rx_cb_clear(void *ll_handle,
					      void *hl_handle)
{
	enum pon_adapter_errno error = PON_ADAPTER_SUCCESS;
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_omcc_context *omcc_ctx;

	dbg_in_args("%p, %p", ll_handle, hl_handle);

	if (!ctx) {
		error = PON_ADAPTER_ERROR;
		goto exit;
	}

	error = pon_net_omci_socket_thread_stop(ctx);
	if (error != PON_ADAPTER_SUCCESS)
		dbg_wrn_fn_ret(pon_net_omci_socket_thread_stop, error);

	omcc_ctx = &ctx->omcc_ctx;

	omcc_socket_clear(omcc_ctx->omcc_socket);

exit:
	dbg_out_ret("%d", error);
	return error;
}

/**
 *  Send a OMCI msg via OMCC socket
 *
 *  \param ll_handle      low level handle.
 *  \param omci_msg       OMCI msg to send.
 *  \param length         length of OMCI msg to send.
 *  \param crc            pointer to CRC of OMCI msg to send (or NULL).
 */
static enum pon_adapter_errno msg_send(void *ll_handle,
				       const uint8_t *omci_msg,
				       const uint16_t length,
				       const uint32_t *crc)
{
	enum pon_adapter_errno error = PON_ADAPTER_SUCCESS;
#ifdef LINUX
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_omcc_context *omcc_ctx;
	int ret;
	struct ethhdr eh;
	/* Make copy of global structure containing constant value */
	struct oui_protocol op = oui_prot_omci;
	struct iovec iov[4];
	struct msghdr mh;

	/* Initialize structure containing physical-layer address data */
	struct sockaddr_ll sockaddr = {
		.sll_family = PF_PACKET,
		.sll_halen = ETH_ALEN
	};

	dbg_in_args("%p, %p, %d, %p", ll_handle, omci_msg, length, crc);

	if (!ctx) {
		error = PON_ADAPTER_ERROR;
		goto exit;
	}

	omcc_ctx = &ctx->omcc_ctx;

	/* Calculate length of entire message and store it */
	op.length = htons((uint16_t)(length + sizeof(*crc)));

	memset(&eh, 0, sizeof(eh));
	/* Set ethernet header data */
	if (ethhdr_set(omcc_ctx->omcc_socket, &eh, NULL, omcc_ctx->p_mac)) {
		dbg_err_fn(ethhdr_set);
		error = PON_ADAPTER_ERR_NO_DATA;
		goto exit;
	}

	/* Copy physical layer address from ethernet header */
	if (memcpy_s(&sockaddr.sll_addr, ETH_ALEN, &eh.h_dest, ETH_ALEN)) {
		dbg_err_fn(memcpy_s);
		error = PON_ADAPTER_ERR_NO_DATA;
		goto exit;
	}
	sockaddr.sll_ifindex = omcc_ctx->omcc_socket->if_index;

	memset(&iov, 0, sizeof(iov));
	/* Specify elements of message */
	iov[0].iov_base = &eh;
	iov[0].iov_len = sizeof(eh);
	iov[1].iov_base = &op;
	iov[1].iov_len = sizeof(op);
	/* We need to cast it here, because of const pointer argument */
	iov[2].iov_base = (void *)omci_msg;
	iov[2].iov_len = length;
	iov[3].iov_base = (uint32_t *) crc;
	iov[3].iov_len = sizeof(*crc);

	memset(&mh, 0, sizeof(mh));
	/* Fill message header with parameters for sendmsg */
	mh.msg_name = &sockaddr;
	mh.msg_namelen = sizeof(sockaddr);
	mh.msg_iov = iov;
	mh.msg_iovlen = 4;

	/* Send message on a socket and check correctness of result */
	ret = (int)sendmsg(omcc_ctx->omcc_socket->sock_fd, &mh,
			   MSG_NOSIGNAL);
	if (ret < 0) {
		dbg_err("sendmsg(): %s\n", strerror(errno));
		error = PON_ADAPTER_ERROR;
		goto exit;
	}

exit:
	dbg_out_ret("%d", error);
#else
	UNUSED(ll_handle);
	UNUSED(omci_msg);
	UNUSED(length);
	UNUSED(crc);
#endif
	return error;
}

const struct pa_msg_ops msg_ops = {
	.msg_send = msg_send,
	.msg_rx_cb_register = msg_rx_cb_register,
	.msg_rx_cb_clear = msg_rx_cb_clear
};
