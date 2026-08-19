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
#include "compat.h"
#include "driver_api.h"
#include "dut_host_if.h"
#include "dut_msg_clbk.h"
#include "sockets.h"
#ifdef MTLK_DEBUG
  #include "dut_utest_nv.h"
#endif /* MTLK_DEBUG */

#define LOG_LOCAL_GID   GID_DUTSERVER
#define LOG_LOCAL_FID   1

#define MT_SERVER_IFACE_NAME   "br-lan" 
#define MT_SERVER_PORT_STREAM  (22222)  /* DUT server/client port */

typedef struct DutContext_t
{
  int epoll_fd;
  int signal_fd;
  int server_fd;
  int client_fd;
  uint32_t client_ip_address;
  uint8_t buffer[DUT_MSG_MAX_MESSAGE_LENGTH];
  size_t bufferLength;
} DutContext_t;

void handle_client_disconnected(struct DutContext_t *ctx)
{
  remove_fd_from_epoll(ctx->epoll_fd, ctx->client_fd);
  close_fd(&ctx->client_fd);
  ctx->bufferLength = 0;
}

void handle_connection_request(struct DutContext_t *ctx)
{
  int temp_fd = INVALID_SOCKET;
  struct sockaddr_in address = {};
  if (accept_connection(ctx->server_fd, &temp_fd, &address))
  {
    // Get the IP address of the remote peer
    uint32_t ip = address.sin_addr.s_addr;

    // Reject request if there is already a connection from a different host.
    // Replace existing connection if new connection was placed from the same host.
    BOOL reject = (ctx->client_fd != INVALID_SOCKET) && (ctx->client_ip_address != ip);
    if (reject)
    {
      ILOG0_DS("Rejecting connection on fd %d from %s", temp_fd, inet_ntoa(address.sin_addr));
      close_fd(&temp_fd);
    }
    else if (add_fd_to_epoll(ctx->epoll_fd, temp_fd, EPOLLRDHUP | EPOLLHUP | EPOLLERR | EPOLLIN))
    {
      // Set socket options
      set_linger_sockopt(temp_fd);
      set_tcp_nodelay_sockopt(temp_fd);
      set_keepalive_sockopt(temp_fd);

      // Close current connection, if any
      if (ctx->client_fd != INVALID_SOCKET)
      {
        WLOG_D("Closing current connection with fd %d", ctx->client_fd);
        handle_client_disconnected(ctx);
      }
      
      // Save current connection attributes
      ILOG0_DS("Accepting new connection on fd %d from %s", temp_fd, inet_ntoa(address.sin_addr));
      ctx->client_fd = temp_fd;
      ctx->client_ip_address = ip;
    }
  }
}

BOOL handle_response(const uint8_t* buffer, size_t length, void *arg)
{
  return send_data(*(int*)arg, buffer, length);
}

/**
 * In a stream-oriented socket, the read() system call will read as much data as is available, 
 * which might not be the entire message (message boundaries are not preserved in stream-oriented 
 * sockets).
 * The communication between the DUT client and the DUT server uses a framing protocol that includes
 * the message size into the header. This field is used by the server to know if the message has 
 * been fully received and thus can be processed. On the contrary, if the message has been partially 
 * received, the server must wait for the rest of it.
 */
void handle_incoming_data(struct DutContext_t *ctx)
{
  // Receive data and append it to existing data (if any)
  int bytesReceived = receive_data(ctx->client_fd, &ctx->buffer[ctx->bufferLength], sizeof(ctx->buffer) - ctx->bufferLength);

  if (bytesReceived < 0)
  {
    ILOG0_V("Unable to receive data from socket: closing connection");
    handle_client_disconnected(ctx);
  }
  else if (bytesReceived == 0)
  {
    ILOG0_V("Client socket disconnected: closing connection");
    handle_client_disconnected(ctx);
  }
  else
  {
    // Discard existing buffer contents if incoming data does not containg the remaining of the 
    // message but a new message.
    uint8_t *request = ctx->buffer;
    if ((ctx->bufferLength > 0) && dut_hostif_request_contains_valid_header(&ctx->buffer[ctx->bufferLength], bytesReceived))
    {
      ILOG1_D("Discarding incomplete staled message with %d bytes", ctx->bufferLength);
      request = &ctx->buffer[ctx->bufferLength];
      ctx->bufferLength = 0;
    }

    // Handle received request and send response back to client.
    // If request message is complete, the handling function will empty the buffer. Otherwise, the
    // buffer will keep received data until the rest of the message is received or a new message 
    // arrives. 
    ctx->bufferLength += bytesReceived;
    dut_hostif_handle_request(request, &ctx->bufferLength, handle_response, &ctx->client_fd);
  }
}

BOOL dut_server_init(int argc, char *argv[])
{
  setsid();

  dut_msg_clbk_init();

  if (MTLK_ERR_OK != dut_api_init(argc, argv))
  {
    ELOG_V("Failed to initialize DUT API");
    return FALSE;
  }

  return TRUE;
}

BOOL dut_server_run()
{
  BOOL ok;
  BOOL done = FALSE;

  struct DutContext_t ctx = 
  {
    .epoll_fd = INVALID_SOCKET,
    .signal_fd = INVALID_SOCKET,
    .server_fd = INVALID_SOCKET,
    .client_fd = INVALID_SOCKET,
    .client_ip_address = 0,
    .bufferLength = 0,
  };

  ILOG0_SD("MaxLinear DUT Server application v%s, pid = %d", MTLK_SOURCE_VERSION, (int)getpid());

  ok = ignore_sigpipe();
  
  ok = ok && setup_termination_signals(&ctx.signal_fd);
  
  ok = ok && create_server(&ctx.server_fd);
  ok = ok && set_reuseaddr_sockopt(ctx.server_fd);
  ok = ok && bind_server(get_ip_address(MT_SERVER_IFACE_NAME), MT_SERVER_PORT_STREAM, ctx.server_fd);
  ok = ok && listen_server(ctx.server_fd);

  ok = ok && create_epoll(&ctx.epoll_fd);
  ok = ok && add_fd_to_epoll(ctx.epoll_fd, ctx.signal_fd, EPOLLIN);
  ok = ok && add_fd_to_epoll(ctx.epoll_fd, ctx.server_fd, EPOLLERR | EPOLLIN);
  
  while (ok && (! done))
  {
    int fd = INVALID_SOCKET;
    uint32_t events = 0;
    ok = wait_epoll(ctx.epoll_fd, &fd, &events);
    if (ok)
    {    
      BOOL processed = FALSE; 
      if (events & EPOLLERR)
      {
        // Handle errors
        ILOG0_D("Error event occurred on fd %d", fd);
        
        if (fd == ctx.server_fd)
        {
          // End server with error
          ELOG_V("Server socket error: exiting application");
          ok = FALSE;
          processed = TRUE;
        }
        else if (fd == ctx.client_fd)
        {
          // Close client socket and continue
          ELOG_V("Client socket error: closing connection");
          handle_client_disconnected(&ctx);
          processed = TRUE;
        }
      }
      else if (((events & EPOLLRDHUP) || (events & EPOLLHUP)) && (fd == ctx.client_fd))
      {
        // Handle disconnected socket (peer socket closed the connection)
        ILOG0_D("Disconnected event occurred on fd %d", fd);

        // Close client socket and continue
        handle_client_disconnected(&ctx);
        processed = TRUE;
      } 
      else if (events & EPOLLIN) 
      {
        if (fd == ctx.signal_fd)
        {
          // End server gracefully
          ILOG0_V("Received termination signal: shutting down");
          done = TRUE;
          processed = TRUE;
        }
        else if (fd == ctx.server_fd)
        {
          handle_connection_request(&ctx);         
          processed = TRUE;
        }
        else if (fd == ctx.client_fd)
        {
          handle_incoming_data(&ctx);
          processed = TRUE;
        }
      }

      if (!processed)
      {
        ELOG_DD("Unexpected event 0x%x occurred on fd %d", events, fd);
        ok = FALSE;
      }
    }
  }
    
  close_fd(&ctx.client_fd);
  close_fd(&ctx.server_fd);
  close_fd(&ctx.signal_fd);
  close_fd(&ctx.epoll_fd);

  ILOG0_V("DUT Server done");

  return ok;
}

void dut_server_destroy()
{
  dut_api_driver_stop(FALSE);
  dut_api_cleanup();
}

int main(int argc, char *argv[])
{
  int exitCode = EXIT_FAILURE;

  if (dut_server_init(argc, argv))
  {
  #ifdef MTLK_DEBUG
    if (NULL != strstr(argv[0], "dut_utest"))
    {
      dut_utest_nv();
      /* dut_utest_***(); */
    }
    else
  #endif /* MTLK_DEBUG */
    {
      if (dut_server_run())
      {
        exitCode = EXIT_SUCCESS;
      }
    }
  }

  dut_server_destroy();

  return exitCode;
}


