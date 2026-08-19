/*
 *  <legal_notice>
 *   MaxLinear, Inc. retains all right, title and interest (including all intellectual
 *   property rights) in and to this computer program, which is protected by applicable
 *   intellectual property laws.  Unless you have obtained a separate written license from
 *   MaxLinear, Inc. or an authorized licensee of MaxLinear, Inc., you are not authorized
 *   to utilize all or a part of this computer program for any purpose (including
 *   reproduction, distribution, modification, and compilation into object code), and you
 *   must immediately destroy or return all copies of this computer program.  If you are
 *   licensed by MaxLinear, Inc. or an authorized licensee of MaxLinear, Inc., your rights
 *   to utilize this computer program are limited by the terms of that license.
 *  
 *   This computer program contains trade secrets owned by MaxLinear, Inc. and, unless
 *   authorized by MaxLinear, Inc. in writing, you agree to maintain the confidentiality
 *   of this computer program and related information and to not disclose this computer
 *   program and related information to any other person or entity.
 *  
 *   Misuse of this computer program or any information contained in it may results in
 *   violations of applicable law.  MaxLinear, Inc. vigorously enforces its copyright,
 *   trade secret, patent, contractual, and other legal rights.
 *  
 *   THIS COMPUTER PROGRAM IS PROVIDED "AS IS" WITHOUT ANY WARRANTIES, AND MAXLINEAR, INC.
 *   EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING THE WARRANTIES OF
 *   MERCHANTIBILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE, AND NONINFRINGEMENT.
 *  
 *  ***************************************************************************************
 *                                          Copyright (c) 2021/2022, MaxLinear, Inc.
 *  ***************************************************************************************
 *  </legal_notice>
 */

#include "dut/ConnectionImpl.h"

#include "dut/Tools.h"

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <mstcpip.h>
#include <stdexcept>

namespace dut {

ConnectionImpl::ConnectionImpl(ConnectionImpl&& obj) noexcept
    : m_socket(obj.m_socket)
{
    obj.m_socket = INVALID_SOCKET;
}

ConnectionImpl& ConnectionImpl::operator=(ConnectionImpl&& obj) noexcept
{
    // Self-assignment detection
    if (&obj == this) {
        return *this;
    }

    // Transfer ownership of resources
    this->m_socket = obj.m_socket;
    obj.m_socket = INVALID_SOCKET;

    return *this;
}

ConnectionImpl::~ConnectionImpl()
{
    close();
}

bool ConnectionImpl::open(const std::string& ipAddress, uint16_t tcpPort, const std::chrono::milliseconds& timeout)
{
    close();

    SOCKADDR_IN target;

    target.sin_family = AF_INET;
    target.sin_port = htons(tcpPort);
    if (inet_pton(target.sin_family, ipAddress.c_str(), &target.sin_addr) != 1) {
        throw std::invalid_argument("Invalid IPv4 network address: " + ipAddress);
    }

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET) {
        throw std::runtime_error("Unable to create socket, error: " + toString(WSAGetLastError()));
    }

    auto handleError = [&](const std::string& functionName, int result, int ignoreError = NO_ERROR) {
        if ((result == SOCKET_ERROR) && (ignoreError != WSAGetLastError())) {
            close();
            throw std::runtime_error("Unable to connect to " + ipAddress + ":" + toString(tcpPort) + ": " + functionName + "() failed, error: " + toString(WSAGetLastError()));
        }

        return result;
    };

    // A close() returns immediately. The underlying stack discards any unsent data and sends a RST (reset) to the peer (this is termed a hard or abortive close).
    bool dontLinger = false;
    handleError("setsockopt", setsockopt(m_socket, SOL_SOCKET, SO_DONTLINGER, reinterpret_cast<const char*>(&dontLinger), sizeof(dontLinger)));

    // Put the socket into non-blocking mode until connected
    u_long nonBlockingMode = 1;
    handleError("ioctlsocket", ioctlsocket(m_socket, FIONBIO, &nonBlockingMode));

    // With a non-blocking socket, the connection attempt cannot be completed immediately.
    // In this case, connect returns SOCKET_ERROR, and WSAGetLastError returns WSAEWOULDBLOCK.
    handleError("connect", connect(m_socket, reinterpret_cast<const sockaddr*>(&target), sizeof(target)), WSAEWOULDBLOCK);

    // The connection request is complete when the socket is writable.
    fd_set write;
    fd_set except;

    FD_ZERO(&write);
    FD_SET(m_socket, &write);

    FD_ZERO(&except);
    FD_SET(m_socket, &except);

    auto msec = static_cast<long>(timeout.count());
    struct timeval tv;
    tv.tv_sec = (msec / 1000);
    tv.tv_usec = (1000 * (msec % 1000));

    int rc = handleError("select", select(0, nullptr, &write, &except, &tv));

    if (rc == NO_ERROR) {

        WSASetLastError(WSAETIMEDOUT);

    } else if (rc > NO_ERROR) {
        if (FD_ISSET(m_socket, &except)) {

            int error = 0;
            auto size = static_cast<int>(sizeof(error));
            getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (char*)&error, &size);

            WSASetLastError(error);

        } else if (FD_ISSET(m_socket, &write)) {

            // Restore the socket into blocking mode
            u_long blockingMode = 0;
            handleError("ioctlsocket", ioctlsocket(m_socket, FIONBIO, &blockingMode));

            return true;
        }
    }

    close();
    return false;
}

void ConnectionImpl::close()
{
    if (m_socket == INVALID_SOCKET) {
        return;
    }

    closesocket(m_socket);
    m_socket = INVALID_SOCKET;
}

void ConnectionImpl::send(const uint8_t* buffer, size_t length)
{
    size_t bytesSent = 0;

    while (bytesSent < length) {
        auto ptr = reinterpret_cast<const char*>(&buffer[bytesSent]);

        int result = ::send(m_socket, ptr, static_cast<int>(length), 0);
        if (result == SOCKET_ERROR) {
            throw std::runtime_error("Unable to send data, error: " + toString(WSAGetLastError()));
        }

        bytesSent += static_cast<size_t>(result);
    }
}

size_t ConnectionImpl::receive(uint8_t* buffer, size_t length, const std::chrono::milliseconds& timeout)
{
    auto* ptr = reinterpret_cast<char*>(buffer);

    auto handleError = [&](const std::string& functionName, int result) {
        if (result == SOCKET_ERROR) {
            throw std::runtime_error("Unable to receive data: " + functionName + "() failed, error: " + toString(WSAGetLastError()));
        }
    };

    // Block until some data is received or timeout elapses or connection is closed by remote peer
    WSAPOLLFD fdArray[] { { 0 } };
    fdArray[0].fd = m_socket;
    fdArray[0].events = POLLIN;
    handleError("WSAPoll", WSAPoll(&fdArray[0], 1, static_cast<int>(timeout.count())));

    // Get the results of the status query
    short events = fdArray[0].revents;

    // Error condition.This bit is also set for a file descriptor referring to the write end of a
    // pipe when the read end has been closed.
    if (events & POLLERR) {
        throw std::runtime_error("Unable to receive data: error condition");
    }

    // Remote peer closed connection
    if (events & POLLHUP) {
        throw std::runtime_error("Unable to receive data: connection closed by remote peer");
    }

    // Timeout receiving data
    if (!(events & POLLIN)) {
        return 0;
    }

    // Get received data
    int result = ::recv(m_socket, ptr, static_cast<int>(length), 0);
    handleError("recv", result);

    // Return number of bytes received
    return static_cast<size_t>(result);
}

}
