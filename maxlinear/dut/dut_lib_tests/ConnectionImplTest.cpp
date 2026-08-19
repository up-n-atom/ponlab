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

#include <functional>
#include <thread>

#include <gtest/gtest.h>

namespace {
constexpr const char* localhost = "127.0.0.1";
constexpr uint16_t tcpPort = 22222;
constexpr auto connectionTimeout = std::chrono::milliseconds(1000);
constexpr auto receiveTimeout = std::chrono::milliseconds(1000);
constexpr size_t maxBufferSize = 255;
constexpr const char* request = "Hello, World!";

class Socket {
public:
    explicit Socket(SOCKET socket)
        : m_socket(socket)
    {
    }
    ~Socket()
    {
        closesocket(m_socket);
    }

    void send(const std::string& s) const
    {
        int result = ::send(m_socket, s.c_str(), static_cast<int>(s.length()) + 1, 0);
        if (result == SOCKET_ERROR) {
            throw std::runtime_error("Unable to send data, error: " + std::to_string(WSAGetLastError()));
        }
    }

    size_t recv(char* buffer, size_t offset, size_t size) const
    {
        if (size <= offset) {
            throw std::invalid_argument("Argument size must be greater than offset");
        }

        int result = ::recv(m_socket, &buffer[offset], static_cast<int>(size - offset), 0);
        if (result == SOCKET_ERROR) {
            throw std::runtime_error("Unable to receive data, error: " + std::to_string(WSAGetLastError()));
        } else if (result == 0) {
            throw std::runtime_error("No data received");
        }

        return static_cast<size_t>(result);
    }

    std::string receiveString()
    {
        char buffer[maxBufferSize] {};
        size_t bytesReceived = 0;

        while ((bytesReceived == 0) || (buffer[bytesReceived - 1] != 0)) {
            bytesReceived += recv(buffer, bytesReceived, maxBufferSize);
        }

        return std::string(buffer);
    }

private:
    SOCKET m_socket = INVALID_SOCKET;
};

class ServerSocket {
public:
    ServerSocket()
    {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        EXPECT_NE(m_socket, INVALID_SOCKET);

        if (m_socket != INVALID_SOCKET) {
            bool reuseAddr = true;
            EXPECT_NE(setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuseAddr), sizeof(reuseAddr)), SOCKET_ERROR);

            bool dontLinger = false;
            EXPECT_NE(setsockopt(m_socket, SOL_SOCKET, SO_DONTLINGER, reinterpret_cast<const char*>(&dontLinger), sizeof(dontLinger)), SOCKET_ERROR);

            m_address.sin_family = AF_INET;
            m_address.sin_addr.s_addr = INADDR_ANY;
            m_address.sin_port = htons(tcpPort);
            EXPECT_NE(bind(m_socket, reinterpret_cast<const sockaddr*>(&m_address), sizeof(m_address)), SOCKET_ERROR);

            EXPECT_NE(listen(m_socket, 3), SOCKET_ERROR);
        }
    }
    ~ServerSocket()
    {
        closesocket(m_socket);
    }

    std::shared_ptr<Socket> accept()
    {
        if (m_socket == INVALID_SOCKET) {
            return nullptr;
        }

        int addrlen = sizeof(m_address);
        SOCKET s = ::accept(m_socket, reinterpret_cast<sockaddr*>(&m_address), &addrlen);

        if (s == INVALID_SOCKET) {
            return nullptr;
        }

        return std::make_shared<Socket>(s);
    }

private:
    SOCKET m_socket = INVALID_SOCKET;
    sockaddr_in m_address {};
};

class ServerThread {
public:
    explicit ServerThread(std::function<void()> f)
        : m_thread(f)
    {
    }
    ~ServerThread()
    {
        m_thread.join();
    }

private:
    std::thread m_thread;
};

TEST(ConnectionImplTest, openShouldFailWithInvalidAddress)
{
    dut::ConnectionImpl connection;

    ASSERT_THROW(connection.open("this_is_an_invalid_ip_address", tcpPort, connectionTimeout), std::invalid_argument);
}

TEST(ConnectionImplTest, openShouldFailIfTimeout)
{
    dut::ConnectionImpl connection;

    // There's no server to connect to so connection times out
    ASSERT_FALSE(connection.open(localhost, tcpPort, connectionTimeout));
}

TEST(ConnectionImplTest, openShouldSucceed)
{
    ServerThread serverThread([]() {
        ServerSocket serverSocket;

        std::shared_ptr<Socket> socket = serverSocket.accept();
        ASSERT_FALSE(socket == nullptr);
    });

    dut::ConnectionImpl connection;

    ASSERT_TRUE(connection.open(localhost, tcpPort, connectionTimeout));
}

TEST(ConnectionImplTest, sendReceiveShouldSucceed)
{
    ServerThread serverThread([]() {
        ServerSocket serverSocket;

        std::shared_ptr<Socket> socket = serverSocket.accept();
        ASSERT_FALSE(socket == nullptr);

        // Receive data and send back the echo
        std::string data = socket->receiveString();
        socket->send(data);

        // Wait until main thread receives the echo before closing the connection
        std::this_thread::sleep_for(2 * receiveTimeout);
    });

    dut::ConnectionImpl connection;

    ASSERT_TRUE(connection.open(localhost, tcpPort, connectionTimeout));

    connection.send(reinterpret_cast<const uint8_t*>(request), strlen(request) + 1);

    uint8_t response[maxBufferSize];
    size_t length = sizeof(response);

    size_t bytesReceived = 0;
    size_t expectedBytes = strlen(request) + 1;

    while (bytesReceived < expectedBytes) {
        bytesReceived += connection.receive(&response[bytesReceived], length - bytesReceived, receiveTimeout);
    }

    EXPECT_STREQ(reinterpret_cast<const char*>(response), request);
}

TEST(ConnectionImplTest, sendReceiveShouldFailIfTimeout)
{
    ServerThread serverThread([]() {
        ServerSocket serverSocket;

        std::shared_ptr<Socket> socket = serverSocket.accept();
        ASSERT_FALSE(socket == nullptr);

        // Receive request but do not respond
        socket->receiveString();

        // Wait until receive timeout elapses before closing connection
        std::this_thread::sleep_for(2 * receiveTimeout);
    });

    dut::ConnectionImpl connection;

    ASSERT_TRUE(connection.open(localhost, tcpPort, connectionTimeout));

    connection.send(reinterpret_cast<const uint8_t*>(request), strlen(request) + 1);

    uint8_t response[maxBufferSize];
    size_t length = sizeof(response);

    size_t bytesReceived = connection.receive(response, length, receiveTimeout);

    EXPECT_EQ(0, bytesReceived);
}

TEST(ConnectionImplTest, sendReceiveShouldFailIfConnectionClosed)
{
    ServerThread serverThread([]() {
        ServerSocket serverSocket;

        std::shared_ptr<Socket> socket = serverSocket.accept();
        ASSERT_FALSE(socket == nullptr);

        // Receive request but do not respond. Connection closes immediately after the socket object goes out of scope.
        socket->receiveString();
    });

    dut::ConnectionImpl connection;

    ASSERT_TRUE(connection.open(localhost, tcpPort, connectionTimeout));

    connection.send(reinterpret_cast<const uint8_t*>(request), strlen(request) + 1);

    uint8_t response[maxBufferSize];
    size_t length = sizeof(response);

    EXPECT_THROW(connection.receive(response, length, receiveTimeout), std::runtime_error);
}
}
