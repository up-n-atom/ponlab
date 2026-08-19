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

#ifndef _DUT_CONNECTION_IMPL_H_
#define _DUT_CONNECTION_IMPL_H_

#include "dut/Connection.h"

#ifndef LINUX_HOST
#include <WinSock2.h>
#else
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif

#include <string>

namespace dut {

/**
 * Implementation of the connection interface to be used in production.
 * 
 * Implements a TCP/IP socket connection with methods to open and close the network connection.
 */
class DUT_API ConnectionImpl : public Connection {
public:
    /**
     * @brief Class constructor.
     */
    ConnectionImpl() = default;

    /**
     * @brief Copy constructor
     *
     * Delete copy constructor to avoid having multiple copies of the same instance.
     */
    ConnectionImpl(const ConnectionImpl&) = delete;

    /**
     * @brief Move constructor.
     *
     * A move constructor allows the resources owned by an rvalue object to be moved into an
     * lvalue without creating its copy.
     */
    ConnectionImpl(ConnectionImpl&& obj) noexcept;

    /**
     * @brief Assignment operator
     *
     * Delete assignment operator to avoid having multiple copies of the same instance.
     */
    ConnectionImpl& operator=(const ConnectionImpl&) = delete;

    /**
     * @brief Move assignment operator
     *
     * The move assignment operator is used to transfer ownership of an instance.
     */
    ConnectionImpl& operator=(ConnectionImpl&& obj) noexcept;

    /**
     * @brief Class destructor.
     *
     * Closes the socket if it is still open.
     */
    ~ConnectionImpl() override;

    /**
     * @brief Opens a TCP/IP socket connection with the server.
     * 
     * Closes connection if open and then tries to open a new connection with server using given 
     * parameters.
     * Throws exception on error, for example if the specified IP address has an invalid format.
     * 
     * @param ipAddress IP address of the server. 
     * @param tcpPort TCP port number to connect to.
     * @param timeout Connection timeout in milliseconds.
     * @return true on success and false on timeout.
     */
    bool open(const std::string& ipAddress, uint16_t tcpPort, const std::chrono::milliseconds& timeout);

    /**
     * @brief Closes the TCP/IP socket connection with the server.
     */
    void close();

    /**
     * @see Connection::send()
     */
    void send(const uint8_t* buffer, size_t length) override;

    /**
     * @see Connection::receive()
     */
    size_t receive(uint8_t* buffer, size_t length, const std::chrono::milliseconds& timeout) override;

private:
    /**
     * Socket used by the connection
     */
    SOCKET m_socket = INVALID_SOCKET;
};

}

#endif
