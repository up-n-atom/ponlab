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
#include <iostream>
#include <stdexcept>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/sockios.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/un.h>
#include <cstdint>
#include <vector>


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

    struct sockaddr_in target;

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == INVALID_SOCKET) {
        throw std::runtime_error("Unable to create socket. " + std::string(strerror(errno)));
    }

    target.sin_family = AF_INET;
    target.sin_port = htons(tcpPort);
    target.sin_addr.s_addr = INADDR_ANY;

    // get host address
    if (inet_pton(target.sin_family, ipAddress.c_str(), &target.sin_addr) != 1) {
        // check if can resolve name //
        sockaddr addr_;
        if (!getnameinfo(&addr_, sizeof(addr_), (char *)ipAddress.c_str(), ipAddress.length(), NULL, 0, 0)) {
            close();
            throw std::runtime_error("No such host. " + std::string(strerror(errno)));
        }
        target.sin_addr = ((struct sockaddr_in *)&addr_)->sin_addr;
    }

    // check connection for none block connect //
    int flags = fcntl(m_socket, F_GETFL, 0);
    if (flags < 0) {
        close();
        throw std::runtime_error("Can't read socket flags. " + std::string(strerror(errno)));
    }

    flags = flags | O_NONBLOCK;

    if (fcntl(m_socket, F_SETFL, flags) != 0) {
        close();
        throw std::runtime_error("Set O_NONBLOCK failed. " + std::string(strerror(errno)));
    }

    ::connect(m_socket, (sockaddr *)&target, sizeof(target));

    fd_set set;
    FD_ZERO(&set);
    FD_SET(m_socket, &set);
    auto msec = static_cast<long>(timeout.count());
    struct timeval tv;
    tv.tv_sec = (msec / 1000);
    tv.tv_usec = (1000 * (msec % 1000));

    int max_s       = int(m_socket) + 1;
    int ret         = select(max_s, NULL, &set, NULL, &tv);
    if (ret != 1) {
        close();
        throw std::runtime_error("Connection timeout. " + std::string(strerror(errno)));
    }

    flags = (flags & ~O_NONBLOCK);
    fcntl(m_socket, F_SETFL, flags);
    return true;
}

void ConnectionImpl::close()
{
    if (m_socket == INVALID_SOCKET) {
        return;
    }

    ::close(m_socket);
    m_socket = INVALID_SOCKET;
}

void ConnectionImpl::send(const uint8_t* buffer, size_t length)
{
    size_t bytesSent = 0;

    while (bytesSent < length) {
        auto ptr = reinterpret_cast<const char*>(&buffer[bytesSent]);

        int result = ::send(m_socket, ptr, static_cast<int>(length), 0);
        if (result == SOCKET_ERROR) {
            throw std::runtime_error("Unable to send data. " + std::string(strerror(errno)));
        }

        bytesSent += static_cast<size_t>(result);
    }
}

size_t ConnectionImpl::receive(uint8_t* buffer, size_t length, const std::chrono::milliseconds& timeout)
{
    auto* ptr = reinterpret_cast<char*>(buffer);

    if (m_socket == INVALID_SOCKET) {
        return 0;
    }

    // Get received data
    int result = ::recv(m_socket, ptr, static_cast<int>(length), 0);

    if (result < 0) {
        throw std::runtime_error("Error reading from socket. " + std::string(strerror(errno)));
    }
    // Return number of bytes received
    return static_cast<size_t>(result);

}

}
