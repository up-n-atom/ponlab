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

#include "Messenger.h"

#include "Exceptions.h"
#include "dut/ConnectionMock.h"
#include "dut/WindowsConsoleLogger.h"

#include <gtest/gtest.h>

using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::StrictMock;

namespace {

constexpr uint8_t wlanIndex = 0x02;
constexpr uint8_t testMsgId = 0x03;
constexpr uint8_t headerLength = 8;
constexpr size_t bufferSize = 4 * 1024 - headerLength;
constexpr uint8_t setChannelRequest[] { 'M', 'T', 0x01, 0x23, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x24, 0x00, 0x0F };
constexpr uint8_t setChannelResponse[] { 'M', 'T', 0x01, 0x83, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x25, 0x00, 0x0F };

constexpr auto timeout = std::chrono::milliseconds(1000);

class MessengerTest : public ::testing::Test {
public:
    MessengerTest()
        : m_connection(std::make_shared<StrictMock<dut::ConnectionMock>>())
        , m_messenger(m_connection, m_logger)
    {
    }

protected:
    uint8_t m_request[bufferSize] {};
    size_t m_requestLength = sizeof(m_request);

    uint8_t m_response[bufferSize] {};
    size_t m_responseLength = sizeof(m_response);

    std::shared_ptr<StrictMock<dut::ConnectionMock>> m_connection;
    std::shared_ptr<dut::Logger> m_logger = std::make_shared<dut::NullLogger>();
    dut::Messenger m_messenger;
};

TEST_F(MessengerTest, executeShouldFailWithRequestTooLarge)
{
    // Request is too large to fit into frame to send
    m_requestLength++;

    EXPECT_THROW(m_messenger.sendReceive(wlanIndex, testMsgId, m_request, m_requestLength, m_response, m_responseLength, timeout), std::invalid_argument);
}

TEST_F(MessengerTest, executeShouldFailWithTimeout)
{
    {
        InSequence sequence;

        EXPECT_CALL(*m_connection, send);
        EXPECT_CALL(*m_connection, receive).WillOnce(Return(0));
    }

    EXPECT_THROW(m_messenger.sendReceive(wlanIndex, testMsgId, m_request, m_requestLength, m_response, m_responseLength, timeout), dut::timeout_exception);
}

TEST_F(MessengerTest, executeShouldSucceed)
{
    m_requestLength = sizeof(setChannelRequest) - headerLength;
    memcpy(m_request, &setChannelRequest[headerLength], m_requestLength);

    {
        InSequence sequence;

        EXPECT_CALL(*m_connection, send).WillOnce(Invoke([&](const uint8_t* buffer, size_t length) {
            EXPECT_EQ(length, sizeof(setChannelRequest));
            EXPECT_EQ(memcmp(buffer, setChannelRequest, length), 0);
        }));
        EXPECT_CALL(*m_connection, receive).WillOnce(Invoke([&](uint8_t* buffer, size_t length, const std::chrono::milliseconds& timeout) {
            size_t frameLength = sizeof(setChannelResponse);
            memcpy(buffer, setChannelResponse, frameLength);

            return frameLength;
        }));
    }

    EXPECT_NO_THROW(m_messenger.sendReceive(wlanIndex, testMsgId, m_request, m_requestLength, m_response, m_responseLength, timeout));
    EXPECT_EQ(m_responseLength, sizeof(setChannelResponse) - headerLength);
    EXPECT_EQ(memcmp(m_response, &setChannelResponse[headerLength], m_responseLength), 0);
}

TEST_F(MessengerTest, executeShouldSucceedIfResponseReceivedInChunks)
{
    m_requestLength = sizeof(setChannelRequest) - headerLength;
    memcpy(m_request, &setChannelRequest[headerLength], m_requestLength);

    // Response is received in two chunks
    constexpr size_t bytesInSecondChunk = 1;

    {
        InSequence sequence;

        EXPECT_CALL(*m_connection, send).WillOnce(Invoke([&](const uint8_t* buffer, size_t length) {
            EXPECT_EQ(length, sizeof(setChannelRequest));
            EXPECT_EQ(memcmp(buffer, setChannelRequest, length), 0);
        }));
        EXPECT_CALL(*m_connection, receive).WillOnce(Invoke([&](uint8_t* buffer, size_t length, const std::chrono::milliseconds& timeout) {
            size_t frameLength = sizeof(setChannelResponse) - bytesInSecondChunk;
            memcpy(buffer, setChannelResponse, frameLength);

            return frameLength;
        }));
        EXPECT_CALL(*m_connection, receive).WillOnce(Invoke([&](uint8_t* buffer, size_t length, const std::chrono::milliseconds& timeout) {
            size_t frameLength = bytesInSecondChunk;
            memcpy(buffer, &setChannelResponse[sizeof(setChannelResponse) - bytesInSecondChunk], frameLength);

            return frameLength;
        }));
    }

    EXPECT_NO_THROW(m_messenger.sendReceive(wlanIndex, testMsgId, m_request, m_requestLength, m_response, m_responseLength, timeout));
    EXPECT_EQ(m_responseLength, sizeof(setChannelResponse) - headerLength);
    EXPECT_EQ(memcmp(m_response, &setChannelResponse[headerLength], m_responseLength), 0);
}

TEST_F(MessengerTest, executeShouldFailWithResponseTooSmall)
{
    m_requestLength = sizeof(setChannelRequest) - headerLength;
    memcpy(m_request, &setChannelRequest[headerLength], m_requestLength);

    // Response buffer is too small
    m_responseLength = 1;

    {
        InSequence sequence;

        EXPECT_CALL(*m_connection, send).WillOnce(Invoke([&](const uint8_t* buffer, size_t length) {
            EXPECT_EQ(length, sizeof(setChannelRequest));
            EXPECT_EQ(memcmp(buffer, setChannelRequest, length), 0);
        }));
        EXPECT_CALL(*m_connection, receive).WillOnce(Invoke([&](uint8_t* buffer, size_t length, const std::chrono::milliseconds& timeout) {
            size_t frameLength = sizeof(setChannelResponse);
            memcpy(buffer, setChannelResponse, frameLength);

            return frameLength;
        }));
    }

    EXPECT_THROW(m_messenger.sendReceive(wlanIndex, testMsgId, m_request, m_requestLength, m_response, m_responseLength, timeout), std::invalid_argument);
}

TEST_F(MessengerTest, executeShouldFailWithInvalidResponse)
{
    m_requestLength = sizeof(setChannelRequest) - headerLength;
    memcpy(m_request, &setChannelRequest[headerLength], m_requestLength);

    {
        InSequence sequence;

        EXPECT_CALL(*m_connection, send).WillOnce(Invoke([&](const uint8_t* buffer, size_t length) {
            EXPECT_EQ(length, sizeof(setChannelRequest));
            EXPECT_EQ(memcmp(buffer, setChannelRequest, length), 0);
        }));
        EXPECT_CALL(*m_connection, receive).WillOnce(Invoke([&](uint8_t* buffer, size_t length, const std::chrono::milliseconds& timeout) {
            size_t frameLength = sizeof(setChannelResponse);
            memcpy(buffer, setChannelResponse, frameLength);

            // Invalid message ID in response
            buffer[3]++;

            return frameLength;
        }));
    }

    EXPECT_THROW(m_messenger.sendReceive(wlanIndex, testMsgId, m_request, m_requestLength, m_response, m_responseLength, timeout), std::runtime_error);
}
}
