/******************************************************************************
 *
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

#ifndef _PON_NET_EXTERN_H
#define _PON_NET_EXTERN_H

#include <stddef.h>  /* for size_t */

#ifdef PON_NET_EXTERN_IMPL
#ifdef LINUX
#define PON_NET_EXT(type, name, args, def) type(*name) args = def
#else
#define PON_NET_EXT(type, name, args, def) \
	static type fake_##name args { return 0; } \
	type(*name) args = fake_##name
#endif
#else
#define PON_NET_EXT(type, name, args, def) \
	extern type(*name) args
#endif

/* We call function pointers, instead of functions directly, to replace
 * them with fake implementation in unit tests.
 * The idea is that unit tests will replace this function pointers with their
 * fake versions allowing to run tests in isolation from actual platform */

PON_NET_EXT(int, pon_net_socket, (int domain, int type, int protocol),
	    socket);
PON_NET_EXT(unsigned int, pon_net_if_nametoindex, (const char *ifname),
	    if_nametoindex);
#ifdef LINUX
PON_NET_EXT(ssize_t, pon_net_send, (int sockfd, const void *buf, size_t len,
				    int flags),
	    send);
#else
PON_NET_EXT(int, pon_net_send, (int sockfd, const void *buf, size_t len,
				int flags),
	    send);
#endif
PON_NET_EXT(int, pon_net_close, (int fd),
	    close);
/* Glibc uses "unsigned long" which is not compliant to the POSIX standard. */
#ifdef __GLIBC__
PON_NET_EXT(int, pon_net_ioctl, (int fd, unsigned long request, ...),
	    ioctl);
#else
PON_NET_EXT(int, pon_net_ioctl, (int fd, int request, ...),
	    ioctl);
#endif

#endif

