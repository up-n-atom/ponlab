/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2009 - 2014 Lantiq Deutschland GmbH
         Copyright 2007 - 2008 Infineon Technologies AG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#ifndef __SOCKETS_H__
#define __SOCKETS_H__

#include <stdint.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>

uint32_t get_ip_address(const char* ifname);

void close_fd(int *fd);

BOOL set_reuseaddr_sockopt(int fd);
BOOL set_linger_sockopt(int fd);
BOOL set_tcp_nodelay_sockopt(int fd);
BOOL set_keepalive_sockopt(int fd);

BOOL create_server(int *server_fd);
BOOL bind_server(uint32_t ip_address, uint16_t port, int server_fd);
BOOL listen_server(int server_fd);
BOOL accept_connection(int server_fd, int *socket_fd, struct sockaddr_in *address);
int receive_data(int fd, uint8_t *buffer, size_t size);
BOOL send_data(int fd, const uint8_t *buffer, size_t length);

BOOL create_epoll(int *epoll_fd);
BOOL add_fd_to_epoll(int epoll_fd, int fd, uint32_t events);
BOOL remove_fd_from_epoll(int epoll_fd, int fd);
BOOL wait_epoll(int epoll_fd, int *fd, uint32_t *events);

BOOL setup_termination_signals(int *signal_fd);

BOOL ignore_sigpipe();

#endif /* !__SOCKETS_H__ */

