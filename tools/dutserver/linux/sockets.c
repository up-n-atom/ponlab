/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2009 - 2014 Lantiq Deutschland GmbH
         Copyright 2007 - 2008 Infineon Technologies AG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#include "mtlkinc.h"
#include "sockets.h"
#include "compat.h"

#include <signal.h>
#include <sys/signalfd.h>
#include <netinet/tcp.h> // For TCP_NODELAY
#include <sys/ioctl.h>

#define LOG_LOCAL_GID   GID_DUT_SOCKETS
#define LOG_LOCAL_FID   1

uint32_t get_ip_address(const char* ifname) 
{
  uint32_t ip_address = INADDR_ANY;
  
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == INVALID_SOCKET)
  {
    ELOG_S("Failed to create socket: %s", strerror(errno));
  }
  else 
  {
    struct ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);

    int rc = ioctl(fd, SIOCGIFADDR, &ifr);
    if (rc == -1)
    {
      ELOG_S("Failed to get interface address: %s", strerror(errno));
    }
    else
    {
      struct sockaddr_in* address = (struct sockaddr_in*)&ifr.ifr_addr;
      ILOG0_SS("The IP address of interface '%s' is %s", ifname, inet_ntoa(address->sin_addr));
      ip_address = ntohl(address->sin_addr.s_addr);
    }

    close(fd);
  }

  return ip_address;
}

void close_fd(int *fd)
{
  if ((fd != NULL) && (*fd != INVALID_SOCKET))
  {
    int rc = close(*fd);
    if (rc == -1)
    {
      ELOG_S("Failed to close descriptor: %s", strerror(errno));
    }
    *fd = INVALID_SOCKET;
  }
}

BOOL set_reuseaddr_sockopt(int fd)
{
  int enable = 1;
  
  int rc = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&enable, sizeof(enable));
  if (rc == -1)
  {
      ELOG_S("Failed to allow reuse of local addresses: %s", strerror(errno));
      return FALSE;
  }

  return TRUE;
}

BOOL set_linger_sockopt(int fd)
{
  struct linger params = 
  {
    .l_onoff = 1,
    .l_linger = 0,
  };
  
  int rc = setsockopt(fd, SOL_SOCKET, SO_LINGER, (const void*)&params, sizeof(params));
  if (rc == -1)
  {
      ELOG_S("Failed to set the linger socket option: %s", strerror(errno));
      return FALSE;
  }

  return TRUE;
}

BOOL set_tcp_nodelay_sockopt(int fd)
{
  int enable = 1;
  
  int rc = setsockopt(fd, SOL_SOCKET, TCP_NODELAY, (const void*)&enable, sizeof(enable));
  if (rc == -1)
  {
      ELOG_S("Failed to force explicit flush of pending output: %s", strerror(errno));
      return FALSE;
  }

  return TRUE;
}

BOOL set_keepalive_sockopt(int fd)
{
  int enable = 1;
  
  int rc = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (const void*)&enable, sizeof(enable));
  if (rc == -1)
  {
      ELOG_S("Failed to enable sending of keep-alive messages: %s", strerror(errno));
      return FALSE;
  }

  return TRUE;
}

BOOL create_server(int *server_fd)
{
  if (server_fd == NULL)
  {
    ELOG_V("Invalid parameter: pointer to file descriptor cannot be NULL");
    return FALSE;
  }

  *server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (*server_fd == INVALID_SOCKET)
  {
    ELOG_S("Failed to create server socket: %s", strerror(errno));
    return FALSE;
  }

  ILOG0_D("Server socket created on fd %d", *server_fd);

  return TRUE;
}

BOOL bind_server(uint32_t ip_address, uint16_t port, int server_fd)
{
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = htonl(ip_address);

  int rc = bind(server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
  if (rc == -1)
  {
      ELOG_S("Failed to bind address to socket: %s", strerror(errno));
      return FALSE;
  }

  return TRUE;
}

BOOL listen_server(int server_fd)
{
  int rc = listen(server_fd, 5);
  if (rc == -1)
  {
      ELOG_S("Failed to listen for connection requests: %s", strerror(errno));
      return FALSE;
  }

  return TRUE;
}

BOOL accept_connection(int server_fd, int *socket_fd, struct sockaddr_in *address)
{
  if (socket_fd == NULL)
  {
    ELOG_V("Invalid parameter: pointer to file descriptor cannot be NULL");
    return FALSE;
  }

  if (address == NULL)
  {
    ELOG_V("Invalid parameter: pointer to peer socket address cannot be NULL");
    return FALSE;
  }

  socklen_t addrlen =  sizeof(struct sockaddr_in);
  *socket_fd = accept(server_fd, (struct sockaddr *)address, &addrlen);
  if (*socket_fd == INVALID_SOCKET)
  {
      ELOG_S("Failed to accept connection request: %s", strerror(errno));
      return FALSE;
  }

  return TRUE;
}

int receive_data(int fd, uint8_t *buffer, size_t size)
{
  int bytesRead = read(fd, buffer, size);
  if (bytesRead == -1)
  {
    ELOG_DS("Failed to read data on fd %d: %s", fd, strerror(errno));
  }

  return bytesRead;
}

BOOL send_data(int fd, const uint8_t *buffer, size_t length)
{
  int bytesWritten = write(fd, buffer, length);
  if (bytesWritten == -1)
  {
    ELOG_DS("Failed to write data on fd %d: %s", fd, strerror(errno));
    return FALSE;
  }

  return TRUE;
}

BOOL create_epoll(int *epoll_fd)
{
  if (epoll_fd == NULL)
  {
    ELOG_V("Invalid parameter: pointer to file descriptor cannot be NULL");
    return FALSE;
  }

  *epoll_fd = epoll_create1(0);
  if (*epoll_fd == INVALID_SOCKET)
  {
    ELOG_S("Failed to create epoll: %s", strerror(errno));
    return FALSE;
  }

  ILOG0_D("I/O event notification facility created on fd %d", *epoll_fd);

  return TRUE;
}

BOOL add_fd_to_epoll(int epoll_fd, int fd, uint32_t events) 
{
  struct epoll_event event;
  event.data.fd = fd;
  event.events = events;
  int rc = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
  if (rc == -1)
  {
    ELOG_S("Failed to add descriptor to epoll instance: %s", strerror(errno));
    return FALSE;
  }

  return TRUE;
}

BOOL remove_fd_from_epoll(int epoll_fd, int fd) 
{
  int rc = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
  if (rc == -1)
  {
    ELOG_S("Failed to remove descriptor from epoll instance: %s", strerror(errno));
    return FALSE;
  }

  return TRUE;
}

BOOL wait_epoll(int epoll_fd, int *fd, uint32_t *events)
{
  if (fd == NULL)
  {
    ELOG_V("Invalid parameter: pointer to file descriptor cannot be NULL");
    return FALSE;
  }

  if (events == NULL)
  {
    ELOG_V("Invalid parameter: pointer to events cannot be NULL");
    return FALSE;
  }

  int num_events;
  struct epoll_event event = {};
  do 
  {
      num_events = epoll_wait(epoll_fd, &event, 1, -1);
  } 
  while ((num_events < 0) && (EINTR == errno));

  if (num_events == -1) 
  {
    ELOG_S("Failed to wait for event: %s", strerror(errno));
    return FALSE;
  }

  if (num_events == 0) 
  {
    ELOG_V("Timeout waiting for event");
    return FALSE;
  }

  *fd = event.data.fd;
  *events = event.events;

  return TRUE;
}

BOOL setup_termination_signals(int *signal_fd) 
{
  if (signal_fd == NULL)
  {
    ELOG_V("Invalid parameter: pointer to file descriptor cannot be NULL");
    return FALSE;
  }

  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGTERM);
  sigaddset(&mask, SIGINT);
  *signal_fd = signalfd(-1, &mask, 0);
  if (*signal_fd == INVALID_SOCKET)
  {
    ELOG_S("Failed to create descriptor for signals: %s", strerror(errno));
    return FALSE;
  }

  ILOG0_D("Termination signals will be delivered via fd %d", *signal_fd);

  return TRUE;
}

BOOL ignore_sigpipe()
{
  struct sigaction act = 
  {
    .sa_handler = SIG_IGN,
    .sa_flags = 0,
  };

  int rc = sigaction(SIGPIPE, &act, NULL);
  if (rc == -1)
  {
      ELOG_S("Failed to set signal action: %s", strerror(errno));
      return FALSE;
  }

  return TRUE;
}

