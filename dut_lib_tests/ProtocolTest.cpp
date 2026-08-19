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

#include "Protocol.h"

#include <gtest/gtest.h>

namespace {
constexpr uint8_t headerLength = 8;
constexpr uint8_t wlanIndex = 2;
constexpr uint8_t testMsgId = 123;
constexpr size_t dataSize = 255;
constexpr size_t frameSize = headerLength + dataSize;

constexpr uint8_t dummyData[] { 0x00, 0x01, 0x02 };
constexpr size_t dummyDataLength = sizeof(dummyData);

constexpr uint8_t setChannelRequest[] { 'M', 'T', 0x01, 0x23, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x24, 0x00, 0x0F };
constexpr uint8_t setChannelResponse[] { 'M', 'T', 0x01, 0x83, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x25, 0x00, 0x0F };

TEST(ProtocolTest, buildFrameShouldFailWithNullData)
{
    uint8_t frame[frameSize];
    size_t frameLength = sizeof(frame);

    // Parameter 'data' is nullptr
    ASSERT_THROW(dut::Protocol::buildFrame(wlanIndex, testMsgId, nullptr, dummyDataLength, frame, frameLength), std::invalid_argument);
}

TEST(ProtocolTest, buildFrameShouldFailWithFrameTooSmall)
{
    // Provided buffer is not big enough to store even the frame header
    uint8_t frame[1];
    size_t frameLength = sizeof(frame);

    ASSERT_FALSE(dut::Protocol::buildFrame(wlanIndex, testMsgId, dummyData, dummyDataLength, frame, frameLength));
    ASSERT_EQ(headerLength + dummyDataLength, frameLength);
}

TEST(ProtocolTest, buildFrameShouldFailWithNullFrame)
{
    size_t frameLength = frameSize;

    // Parameter 'frame' is nullptr
    ASSERT_THROW(dut::Protocol::buildFrame(wlanIndex, testMsgId, dummyData, dummyDataLength, nullptr, frameLength), std::invalid_argument);
}

TEST(ProtocolTest, buildFrameShouldSucceedWithNoFrame)
{
    size_t frameLength = 0;

    // Do not build frame, just query for required frame length
    ASSERT_FALSE(dut::Protocol::buildFrame(wlanIndex, testMsgId, dummyData, dummyDataLength, nullptr, frameLength));
    ASSERT_EQ(headerLength + dummyDataLength, frameLength);
}

TEST(ProtocolTest, buildFrameShouldSucceed)
{
    uint8_t frame[frameSize];
    size_t frameLength = sizeof(frame);

    ASSERT_TRUE(dut::Protocol::buildFrame(wlanIndex, testMsgId, dummyData, dummyDataLength, frame, frameLength));
    ASSERT_EQ(headerLength + dummyDataLength, frameLength);
    ASSERT_EQ('M', frame[0]);
    ASSERT_EQ('T', frame[1]);
}

TEST(ProtocolTest, parseFrameShouldFailWithNullFrame)
{
    uint8_t msgId = UINT8_MAX;
    uint8_t data[dataSize];
    size_t dataLength = sizeof(data);

    // Parameter 'frame' is nullptr
    ASSERT_THROW(dut::Protocol::parseFrame(nullptr, sizeof(setChannelResponse), msgId, data, dataLength), std::invalid_argument);
    ASSERT_EQ(msgId, 0x00);
}

TEST(ProtocolTest, parseFrameShouldFailWithZeroLengthFrame)
{
    uint8_t msgId = UINT8_MAX;
    uint8_t data[dataSize];
    size_t dataLength = sizeof(data);

    // Parameter 'frameLength' is 0
    ASSERT_THROW(dut::Protocol::parseFrame(setChannelResponse, 0, msgId, data, dataLength), std::invalid_argument);
    ASSERT_EQ(msgId, 0x00);
}

TEST(ProtocolTest, parseFrameShouldFailWithFrameTooSmall)
{
    uint8_t msgId = UINT8_MAX;
    uint8_t data[dataSize];
    size_t dataLength = sizeof(data);

    // Parameter 'frame' does not contain the full header (frame has not been completely received yet)
    ASSERT_FALSE(dut::Protocol::parseFrame(setChannelResponse, headerLength - 1, msgId, data, dataLength));
    ASSERT_EQ(msgId, 0x00);
}

TEST(ProtocolTest, parseFrameShouldFailWithInvalidSignature)
{
    uint8_t frame[sizeof(setChannelResponse)];
    constexpr size_t frameLength = sizeof(setChannelResponse);

    // Frame signature is not valid
    std::memcpy(frame, setChannelResponse, frameLength);
    frame[0] = 0x00;

    uint8_t msgId = UINT8_MAX;
    uint8_t data[dataSize];
    size_t dataLength = sizeof(data);

    ASSERT_THROW(dut::Protocol::parseFrame(frame, frameLength, msgId, data, dataLength), std::invalid_argument);
    ASSERT_EQ(msgId, 0x00);
}

TEST(ProtocolTest, parseFrameShouldFailWithInvalidResponse)
{
    uint8_t frame[sizeof(setChannelResponse)];
    constexpr size_t frameLength = sizeof(setChannelResponse);

    // Frame is not a valid response
    std::memcpy(frame, setChannelResponse, frameLength);
    frame[3] = 0x00;

    uint8_t msgId = UINT8_MAX;
    uint8_t data[dataSize];
    size_t dataLength = sizeof(data);

    ASSERT_THROW(dut::Protocol::parseFrame(frame, frameLength, msgId, data, dataLength), std::invalid_argument);
    ASSERT_EQ(msgId, 0x00);
}

TEST(ProtocolTest, parseFrameShouldFailWithIncompleteResponse)
{
    uint8_t msgId = UINT8_MAX;
    uint8_t data[dataSize];
    size_t dataLength = sizeof(data);

    // Parameter 'frame' does not contain the full frame (not all bytes specified in header have been received)
    ASSERT_FALSE(dut::Protocol::parseFrame(setChannelResponse, sizeof(setChannelResponse) - 1, msgId, data, dataLength));
    ASSERT_EQ(msgId, 0x00);
}

TEST(ProtocolTest, parseFrameShouldSucceedWithNoData)
{
    uint8_t msgId = UINT8_MAX;
    size_t dataLength = 0;

    // Do not parse frame, just query for required data length
    ASSERT_FALSE(dut::Protocol::parseFrame(setChannelResponse, sizeof(setChannelResponse), msgId, nullptr, dataLength));
    ASSERT_EQ(msgId, setChannelResponse[3] & ~0x80);
    ASSERT_EQ(sizeof(setChannelResponse) - headerLength, dataLength);
}

TEST(ProtocolTest, parseFrameShouldFailWithNullData)
{
    uint8_t msgId = UINT8_MAX;
    size_t dataLength = dataSize;

    // Parameter 'data' is nullptr
    ASSERT_THROW(dut::Protocol::parseFrame(setChannelResponse, sizeof(setChannelResponse), msgId, nullptr, dataLength), std::invalid_argument);
    ASSERT_EQ(msgId, setChannelResponse[3] & ~0x80);
}

TEST(ProtocolTest, parseFrameShouldSucceed)
{
    uint8_t msgId = UINT8_MAX;
    uint8_t data[dataSize];
    size_t dataLength = sizeof(data);

    ASSERT_TRUE(dut::Protocol::parseFrame(setChannelResponse, sizeof(setChannelResponse), msgId, data, dataLength));
    ASSERT_EQ(msgId, setChannelResponse[3] & ~0x80);
    ASSERT_EQ(sizeof(setChannelResponse) - headerLength, dataLength);
    ASSERT_EQ(0, std::memcmp(data, &setChannelResponse[headerLength], dataLength));
}

} // namespace
