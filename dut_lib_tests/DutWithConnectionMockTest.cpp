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

#include "dut/DutImpl.h"

#include "CmdLineArgs.h"
#include "EmbeddedResource.h"
#include "NonVolatileMemory.h"
#include "TemporaryFile.h"
#include "dut/ConnectionMock.h"
#include "dut/Tools.h"
#include "dut/WindowsConsoleLogger.h"
#include "resource.h"

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::StrictMock;

namespace {

constexpr uint8_t wlanIndex = 0x02;
constexpr dut::NvMemoryType memoryType = dut::NvMemoryType::MEMORY_TYPE_FLASH;
constexpr dut::NvMemorySize memorySize = dut::NvMemorySize::MEMORY_SIZE_BYTES_1K;
constexpr size_t bufferSize = 1024;

size_t toByteArray(const std::string& data, uint8_t* buffer, size_t size, const std::string& separator = " ")
{
    size_t length = 0;
    for (size_t i = 0; (i < data.length()) && (length < size); i += (2 + separator.length())) {
        auto byte = data.substr(i, 2);
        buffer[length++] = uint8_t(strtol(byte.c_str(), nullptr, 16));
    }

    return length;
}

#define SET_REQUEST_RESPONSE_EXPECTATION(request, response)                                                                                 \
    EXPECT_CALL(*m_connection, send).WillOnce(Invoke([&](const uint8_t* buffer, size_t length) {                                            \
        m_length = toByteArray(request, m_buffer.data(), m_buffer.size());                                                                  \
        EXPECT_EQ(length, m_length);                                                                                                        \
        EXPECT_EQ(memcmp(buffer, m_buffer.data(), length), 0);                                                                              \
    }));                                                                                                                                    \
    if (strlen(response) > 0) {                                                                                                             \
        EXPECT_CALL(*m_connection, receive).WillOnce(Invoke([&](uint8_t* buffer, size_t length, const std::chrono::milliseconds& timeout) { \
            return toByteArray(response, buffer, length);                                                                                   \
        }));                                                                                                                                \
    }

#define SET_REQUEST_RESPONSE_EXPECTATION_DEBUG(request, response)                                                                           \
    EXPECT_CALL(*m_connection, send).WillOnce(Invoke([&](const uint8_t* buffer, size_t length) {                                            \
        m_length = toByteArray(request, m_buffer.data(), m_buffer.size());                                                                  \
        std::cout << dut::toString(buffer, length, "actual  ") << std::endl;                                                                \
        std::cout << dut::toString(m_buffer.data(), m_length, "expected") << std::endl;                                                     \
        EXPECT_EQ(length, m_length);                                                                                                        \
        EXPECT_EQ(memcmp(buffer, m_buffer.data(), length), 0);                                                                              \
    }));                                                                                                                                    \
    if (strlen(response) > 0) {                                                                                                             \
        EXPECT_CALL(*m_connection, receive).WillOnce(Invoke([&](uint8_t* buffer, size_t length, const std::chrono::milliseconds& timeout) { \
            return toByteArray(response, buffer, length);                                                                                   \
        }));                                                                                                                                \
    }

class DutWithConnectionMockTest : public ::testing::Test {
public:
    DutWithConnectionMockTest()
        : m_dut(wlanIndex, m_connection, m_logger)
    {
    }

    void SetUp() override
    {
        constexpr auto resetMacRequest = "4d 54 01 21 0c 00 00 00 02 00 00 00 00 04 00 00 01 00 00 00";
        constexpr auto resetMacResponse = "4d 54 01 81 00 00 00 00";

        constexpr auto getFwInfoRequest = "4d 54 01 23 18 00 00 00 10 00 00 00 02 00 68 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
        constexpr auto getFwInfoResponse = "4d 54 01 83 18 00 00 00 10 00 01 00 02 00 69 00 1f 01 00 00 00 00 00 0f 0f 01 03 00 00 00 0f 0f";

        constexpr auto readNvm1Request = "4d 54 01 25 00 02 00 00 f8 01 00 00 02 00 02 00 02 00 00 00 00 00 00 00 00 04 00 00 e4 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
        constexpr auto readNvm1Response = "4d 54 01 85 00 02 00 00 f8 01 01 00 02 00 03 00 02 00 00 00 00 00 00 00 00 08 00 00 e4 01 00 00 00 00 00 00 fc 1b 00 00 00 00 00 00 30 1a 80 09 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff ff ff ff ff ff ff ff ff ff ff 06 60 10 00 61 56 41 00 43 ac 9a 96 f1 bb 20 db 07 00 29 0f 64 02 00 3c 00 ff 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

        constexpr auto readNvm2Request = "4d 54 01 25 00 02 00 00 f8 01 00 00 02 00 02 00 02 00 00 00 e4 01 00 00 00 04 00 00 e4 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
        constexpr auto readNvm2Response = "4d 54 01 85 00 02 00 00 f8 01 01 00 02 00 03 00 02 00 00 00 e4 01 00 00 00 08 00 00 e4 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

        constexpr auto readNvm3Request = "4d 54 01 25 54 00 00 00 4c 00 00 00 02 00 02 00 02 00 00 00 c8 03 00 00 00 04 00 00 38 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
        constexpr auto readNvm3Response = "4d 54 01 85 54 00 00 00 4c 00 01 00 02 00 03 00 02 00 00 00 c8 03 00 00 00 08 00 00 38 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff";

        constexpr auto readChipVersionRequest = "4d 54 01 23 10 00 00 00 08 00 00 00 02 00 20 00 00 00 00 00 00 00 00 00";
        constexpr auto readChipVersionResponse = "4d 54 01 83 10 00 00 00 08 00 01 00 02 00 21 00 80 09 00 00 00 00 00 00";

        constexpr auto getCombinedVersionRequest = "4d 54 01 2d 00 02 00 00 f8 01 00 00 02 00 02 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
        constexpr auto getCombinedVersionResponse = "4d 54 01 8d 00 02 00 00 f8 01 01 00 02 00 03 00 77 61 76 65 5f 72 65 6c 65 61 73 65 5f 6d 69 6e 6f 72 3d 22 30 36 2e 30 31 2e 30 30 2e 31 31 30 34 22 0a 77 61 76 65 5f 70 73 64 5f 76 65 72 3d 22 32 30 32 31 2d 30 38 2d 32 33 5f 31 35 2d 33 30 2d 30 36 5f 52 45 56 5f 33 35 37 38 63 30 37 66 38 37 31 63 22 0a 77 61 76 65 5f 72 65 67 75 6c 61 74 6f 72 79 5f 76 65 72 3d 22 32 30 31 38 2e 30 34 2e 31 37 5f 57 41 56 45 31 39 2d 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

        constexpr auto setHdkConfigRequest = "4d 54 01 2b 2c 00 00 00 36 04 00 00 1c 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff ff 00 00 00 00 00 00 00 00 00 00";
        constexpr auto setHdkConfigResponse = "4d 54 01 8b 2c 00 00 00 36 04 00 00 1c 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff ff 00 00 00 00 00 00 00 00 00 00";

        constexpr auto setRxAggregationEnabledRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 8e 00 00 00 00 00";
        constexpr auto setRxAggregationEnabledResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 8f 00 00 00 00 00";

        constexpr auto stopTxRequest = "4d 54 01 23 08 00 00 00 00 00 00 00 02 00 02 02";
        constexpr auto stopTxResponse = "4d 54 01 83 08 00 00 00 00 00 01 00 02 00 03 02";

        constexpr auto stopCwRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 2a 00 00 00 00 00";
        constexpr auto stopCwResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 2b 00 00 00 00 00";

        constexpr auto setSpacelessTxDisabledRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 2c 00 00";
        constexpr auto setSpacelessTxDisabledResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 2d 00 00";

        constexpr auto stopRxCalibrationRequest = "4d 54 01 23 08 00 00 00 00 00 00 00 02 00 82 00";
        constexpr auto stopRxCalibrationResponse = "4d 54 01 83 08 00 00 00 00 00 01 00 02 00 83 00";

        constexpr auto setRiscModeEnabledRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 3a 00 01 00 00 00";
        constexpr auto setRiscModeEnabledResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 3b 00 01 00 00 00";

        constexpr auto setIfsRequest = "4d 54 01 23 0C 00 00 00 04 00 00 00 02 00 30 00 10 00 00 00";
        constexpr auto setIfsResponse = "4d 54 01 83 0C 00 00 00 04 00 01 00 02 00 31 00 10 00 00 00";

        constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
        constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

        constexpr auto setEnabledTxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 24 00 0f";
        constexpr auto setEnabledTxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 25 00 0f";

        constexpr auto setTransmitPowerLevelAntenna1Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 00 00 22 44";
        constexpr auto setTransmitPowerLevelAntenna1Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 00 00 22 44";

        constexpr auto setTransmitPowerLevelAntenna2Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 01 00 22 44";
        constexpr auto setTransmitPowerLevelAntenna2Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 01 00 22 44";

        constexpr auto setTransmitPowerLevelAntenna3Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 02 00 22 44";
        constexpr auto setTransmitPowerLevelAntenna3Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 02 00 22 44";

        constexpr auto setTransmitPowerLevelAntenna4Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 03 00 22 44";
        constexpr auto setTransmitPowerLevelAntenna4Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 03 00 22 44";

        {
            InSequence sequence;

            SET_REQUEST_RESPONSE_EXPECTATION(resetMacRequest, resetMacResponse)
            SET_REQUEST_RESPONSE_EXPECTATION(getFwInfoRequest, getFwInfoResponse)
            SET_REQUEST_RESPONSE_EXPECTATION(readNvm1Request, readNvm1Response)
            SET_REQUEST_RESPONSE_EXPECTATION(readNvm2Request, readNvm2Response)
            SET_REQUEST_RESPONSE_EXPECTATION(readNvm3Request, readNvm3Response)
            SET_REQUEST_RESPONSE_EXPECTATION(readChipVersionRequest, readChipVersionResponse)
            SET_REQUEST_RESPONSE_EXPECTATION(getCombinedVersionRequest, getCombinedVersionResponse)
            SET_REQUEST_RESPONSE_EXPECTATION(setHdkConfigRequest, setHdkConfigResponse)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxAggregationEnabledRequest, setRxAggregationEnabledResponse)
            SET_REQUEST_RESPONSE_EXPECTATION(stopTxRequest, stopTxResponse)
            SET_REQUEST_RESPONSE_EXPECTATION(stopCwRequest, stopCwResponse)
            SET_REQUEST_RESPONSE_EXPECTATION(setSpacelessTxDisabledRequest, setSpacelessTxDisabledResponse)
            SET_REQUEST_RESPONSE_EXPECTATION(stopRxCalibrationRequest, stopRxCalibrationResponse)
            SET_REQUEST_RESPONSE_EXPECTATION(setRiscModeEnabledRequest, setRiscModeEnabledResponse)
            SET_REQUEST_RESPONSE_EXPECTATION(setIfsRequest, setIfsResponse)
            SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
            SET_REQUEST_RESPONSE_EXPECTATION(setEnabledTxAntennaMaskRequest, setEnabledTxAntennaMaskResponse)
            SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna1Request, setTransmitPowerLevelAntenna1Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna2Request, setTransmitPowerLevelAntenna2Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna3Request, setTransmitPowerLevelAntenna3Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna4Request, setTransmitPowerLevelAntenna4Response)
        }

        ASSERT_TRUE(m_dut.driverInit(false, memoryType, memorySize, dut::Band::BAND_INVALID));
    }

    void TearDown() override
    {
        m_logger->setLogLevel(dut::LogLevel::LOG_LEVEL_NONE);

        if (m_tearDown) {
            constexpr auto stopVapTrafficRequest = "4d 54 01 2b 14 00 00 00 0f 04 00 00 04 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
            constexpr auto stopVapTrafficResponse = "4d 54 01 8b 14 00 00 00 0f 04 00 00 04 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

            constexpr auto removeVapRequest = "4d 54 01 2b 14 00 00 00 11 04 00 00 04 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
            constexpr auto removeVapResponse = "4d 54 01 8b 14 00 00 00 11 04 00 00 04 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

            {
                InSequence sequence;

                SET_REQUEST_RESPONSE_EXPECTATION(stopVapTrafficRequest, stopVapTrafficResponse)
                SET_REQUEST_RESPONSE_EXPECTATION(removeVapRequest, removeVapResponse)
            }

            ASSERT_TRUE(m_dut.driverRelease());

            m_tearDown = false;
        }
    }

    std::shared_ptr<StrictMock<dut::ConnectionMock>> m_connection = std::make_shared<StrictMock<dut::ConnectionMock>>();
    std::shared_ptr<dut::Logger> m_logger = std::make_shared<dut::WindowsConsoleLogger>(true, g_cmdLineArgs.getLogLevel());
    dut::DutImpl m_dut;

    std::array<uint8_t, bufferSize> m_buffer {};
    size_t m_length = 0;

    bool m_tearDown = true;
};

TEST_F(DutWithConnectionMockTest, driverInitShouldFailIfAlreadyInitialized)
{
    EXPECT_FALSE(m_dut.driverInit(false, memoryType, memorySize, dut::Band::BAND_INVALID));
    EXPECT_EQ(m_dut.getLastError(), "Already initialized");
}

TEST_F(DutWithConnectionMockTest, driverReleaseShouldFailIfNotInitialized)
{
    TearDown();

    EXPECT_FALSE(m_dut.driverRelease());
    EXPECT_EQ(m_dut.getLastError(), "Not initialized");
}

TEST_F(DutWithConnectionMockTest, calculateLinearRegressionShouldSucceed)
{
    int32_t powerMax = -12;
    const std::array<float, dut::maxNumRxAntennas> lnaGains { 24.1345f, 24.7555f, 23.7855f, 23.2155f, 0.0f };
    int32_t lowerPowerBoundary = 0;
    int32_t upperPowerBoundary = 1;
    std::array<float, dut::numPowerPoints> powerVector {};

    ASSERT_TRUE(m_dut.calculateRxPowerVector(powerMax, lnaGains, lowerPowerBoundary, upperPowerBoundary, powerVector));

    const std::vector<float> expectedPowerVector { -37.0f, -37.0f, -37.0f, -36.0f, -36.0f, -36.0f };
    for (uint8_t i = 0; i < powerVector.size(); i++) {
        EXPECT_EQ(expectedPowerVector.at(i), powerVector[i]);
    }

    const std::array<std::array<uint16_t, dut::numPowerPoints>, dut::maxNumRxAntennas> voltages {
        {
            //
            { 143, 157, 172, 184, 197, 214 },
            { 138, 153, 167, 178, 192, 208 },
            { 260, 272, 281, 286, 295, 307 },
            { 130, 140, 151, 158, 169, 181 },
            { 0, 0, 0, 0, 0, 0 } //
        }
    };
    const std::array<uint16_t, dut::maxNumRxAntennas> offsets { 131, 126, 254, 122, 0 };
    std::array<dut::RssiAB_t, dut::maxNumRxAntennas> result {};
    std::array<float, dut::maxNumRxAntennas> maxErrors {};

    ASSERT_TRUE(m_dut.calculateLinearRegression(powerVector, voltages, offsets, lnaGains, result, maxErrors));

    const std::array<dut::RssiAB_t, dut::maxNumRxAntennas> expectedResult { {
        //
        { 0.28665f / 2, -113.8824f },
        { 0.29375f / 2, -109.4515f },
        { 0.24943f / 2, -130.8720f },
        { 0.28053f / 2, -122.7129f },
        { 0.0f, 0.0f } //
    } };
    for (uint8_t i = 0; i < dut::maxNumRxAntennas; i++) {
        EXPECT_NEAR(expectedResult[i].a, result[i].a, 0.01f);
        EXPECT_NEAR(expectedResult[i].b, result[i].b, 0.01f);
    }

    const std::array<float, dut::maxNumRxAntennas> expectedMaxErrors { 1.05128f / 2, 1.04999f / 2, 1.11351f / 2, 1.07098f / 2, -1.0f };
    for (uint8_t i = 0; i < dut::maxNumRxAntennas; i++) {
        EXPECT_NEAR(expectedMaxErrors[i], maxErrors[i], 0.00001f);
    }
}

TEST_F(DutWithConnectionMockTest, calculateOptimalS2dGainsShouldSucceed)
{
    uint8_t auxAdcRes = 10;
    float ioffsStep = 0.0125f;

    ASSERT_TRUE(m_dut.setRssiS2dInitials(auxAdcRes, ioffsStep));

    /* Example code to create the DB
    int32_t s2d_gain_init_db[] { -6, -6, -10 };
    int32_t s2d_gain_boost[] { 12, 6, 0 };
    int32_t s2d_gain_db[] { 0, 0, 0 };

    for (int i = 0; i < sizeof(s2d_gain_db) / sizeof(s2d_gain_db[0]); i++) {
        s2d_gain_db[i] = s2d_gain_init_db[i] + s2d_gain_boost[i];
    }

    uint8_t region = 0;
    int32_t regionS2dGain = s2d_gain_db[region];
    */
    int32_t regionS2dGain = 2;

    const std::array<uint16_t, dut::maxNumRxAntennas> rssiVoltage1 { 177, 140, 142, 152, 0 };
    const std::array<uint16_t, dut::maxNumRxAntennas> rssiVoltage2 { 1023, 1023, 1023, 1023, 0 };

    std::array<int32_t, dut::maxNumRxAntennas> optimalS2dGains {};

    ASSERT_TRUE(m_dut.calculateOptimalS2dGains(rssiVoltage1, rssiVoltage2, regionS2dGain, optimalS2dGains));

    const std::array<int32_t, dut::maxNumRxAntennas> expectedoptimalS2dGains { 1, 1, 1, 1, 0 };
    for (uint8_t i = 0; i < dut::maxNumRxAntennas; i++) {
        EXPECT_EQ(expectedoptimalS2dGains[i], optimalS2dGains[i]);
    }
}

TEST_F(DutWithConnectionMockTest, calculateOptimalS2dOffsetsShouldSucceed)
{
    uint8_t auxAdcRes = 9;
    float ioffsStep = 0.0625f;

    ASSERT_TRUE(m_dut.setRssiS2dInitials(auxAdcRes, ioffsStep));

    /* Example code to create the DB
    int32_t s2d_gain_init_db[] { -6, -6, -10 };
    uint8_t s2d_offset_init_db[] = { 8, 8, 16 };

    uint8_t region = 0;
    int32_t regionS2dGain = s2d_gain_init_db[region];
    uint8_t regionS2dOffset = s2d_offset_init_db[region];
    */
    int32_t regionS2dGain = -1;
    uint8_t regionS2dOffset = 8;

    const std::array<uint16_t, dut::maxNumRxAntennas> rssiVoltage1 { 294, 308, 302, 305, 0 };
    const std::array<uint16_t, dut::maxNumRxAntennas> rssiVoltage2 { 316, 332, 324, 328, 0 };

    std::array<uint8_t, dut::maxNumRxAntennas> optimalS2dOffsets {};

    ASSERT_TRUE(m_dut.calculateOptimalS2dOffsets(rssiVoltage1, rssiVoltage2, regionS2dGain, regionS2dOffset, optimalS2dOffsets));

    const std::array<uint8_t, dut::maxNumRxAntennas> expectedoptimalS2dOffsets { 11, 12, 12, 12, 0 };
    for (uint8_t i = 0; i < dut::maxNumRxAntennas; i++) {
        EXPECT_EQ(expectedoptimalS2dOffsets[i], optimalS2dOffsets[i]);
    }
}

TEST_F(DutWithConnectionMockTest, calculateRxLnaSubBandGainsShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto setRiscModeDisabledRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 3a 00 00 00 00 00";
    constexpr auto setRiscModeDisabledResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 3b 00 00 00 00 00";

    constexpr auto startRxCalibrationRequest = "4d 54 01 23 08 00 00 00 00 00 00 00 02 00 80 00";
    constexpr auto startRxCalibrationResponse = "4d 54 01 83 08 00 00 00 00 00 01 00 02 00 81 00";

    constexpr auto stopRxCalibrationRequest = "4d 54 01 23 08 00 00 00 00 00 00 00 02 00 82 00";
    constexpr auto stopRxCalibrationResponse = "4d 54 01 83 08 00 00 00 00 00 01 00 02 00 83 00";

    constexpr auto setRiscModeEnabledRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 3a 00 01 00 00 00";
    constexpr auto setRiscModeEnabledResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 3b 00 01 00 00 00";

    constexpr auto setRxBand0Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 00";
    constexpr auto setRxBand0Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 00";
    constexpr auto setRxBand1Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 01";
    constexpr auto setRxBand1Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 01";
    constexpr auto setRxBand2Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 02";
    constexpr auto setRxBand2Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 02";
    constexpr auto setRxBand3Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 03";
    constexpr auto setRxBand3Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 03";
    constexpr auto setRxBand4Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 04";
    constexpr auto setRxBand4Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 04";
    constexpr auto setRxBand5Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 05";
    constexpr auto setRxBand5Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 05";
    constexpr auto setRxBand6Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 06";
    constexpr auto setRxBand6Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 06";
    constexpr auto setRxBand7Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 07";
    constexpr auto setRxBand7Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 07";
    constexpr auto setRxBand8Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 08";
    constexpr auto setRxBand8Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 08";
    constexpr auto setRxBand9Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 09";
    constexpr auto setRxBand9Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 09";
    constexpr auto setRxBand10Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 0a";
    constexpr auto setRxBand10Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 0a";
    constexpr auto setRxBand11Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 0b";
    constexpr auto setRxBand11Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 0b";
    constexpr auto setRxBand12Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 0c";
    constexpr auto setRxBand12Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 0c";
    constexpr auto setRxBand13Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 0d";
    constexpr auto setRxBand13Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 0d";
    constexpr auto setRxBand14Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 0e";
    constexpr auto setRxBand14Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 0e";
    constexpr auto setRxBand15Request = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 86 00 0f";
    constexpr auto setRxBand15Response = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 87 00 0f";

    constexpr auto getRxCwPowerRequest = "4d 54 01 23 54 00 00 00 4c 00 00 00 02 00 72 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";

    constexpr auto getRxCwPower0Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 75 FF FF FF 75 FF FF FF 8B 40 00 00 48 FF FF FF 48 FF FF FF 1E F9 FF FF 33 FF FF FF 39 FF FF FF 4B E9 FF FF 5D FF FF FF 5D FF FF FF 4F 05 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower1Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 93 FF FF FF 99 FF FF FF F8 59 00 00 66 FF FF FF 66 FF FF FF 84 EC FF FF 54 FF FF FF 5A FF FF FF BA D0 FF FF 7E FF FF FF 7E FF FF FF 46 F5 FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower2Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 BA FF FF FF BD FF FF FF 69 1E 00 00 8A FF FF FF 8D FF FF FF D7 AA FF FF 7B FF FF FF 7B FF FF FF 11 CF FF FF A5 FF FF FF A2 FF FF FF E2 F8 FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower3Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 E1 FF FF FF E1 FF FF FF E7 47 00 00 B4 FF FF FF B4 FF FF FF BE B5 FF FF A2 FF FF FF A2 FF FF FF 5D A1 FF FF C9 FF FF FF C9 FF FF FF FF C0 FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower4Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 0E 00 00 00 0E 00 00 00 12 BB 00 00 DE FF FF FF DE FF FF FF E4 DD FF FF C9 FF FF FF CF FF FF FF 1C AF FF FF F0 FF FF FF F3 FF FF FF E2 DF FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower5Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 32 00 00 00 32 00 00 00 1A DA FF FF 05 00 00 00 05 00 00 00 C8 1E FF FF ED FF FF FF F0 FF FF FF 69 8E FF FF 1A 00 00 00 1A 00 00 00 51 E5 FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower6Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 50 00 00 00 50 00 00 00 0A C1 00 00 23 00 00 00 23 00 00 00 96 ED FF FF 0B 00 00 00 0E 00 00 00 92 70 FF FF 3B 00 00 00 3B 00 00 00 E8 8F FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower7Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 5C 00 00 00 5C 00 00 00 15 20 01 00 2F 00 00 00 2F 00 00 00 4A E2 FF FF 14 00 00 00 17 00 00 00 05 21 FF FF 47 00 00 00 47 00 00 00 30 40 FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower8Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 5C 00 00 00 5C 00 00 00 15 20 01 00 2F 00 00 00 2F 00 00 00 4A E2 FF FF 14 00 00 00 17 00 00 00 05 21 FF FF 47 00 00 00 47 00 00 00 30 40 FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower9Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 5C 00 00 00 5C 00 00 00 15 20 01 00 2F 00 00 00 2F 00 00 00 4A E2 FF FF 14 00 00 00 17 00 00 00 05 21 FF FF 47 00 00 00 47 00 00 00 30 40 FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower10Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 5C 00 00 00 5C 00 00 00 15 20 01 00 2F 00 00 00 2F 00 00 00 4A E2 FF FF 14 00 00 00 17 00 00 00 05 21 FF FF 47 00 00 00 47 00 00 00 30 40 FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower11Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 5C 00 00 00 5C 00 00 00 15 20 01 00 2F 00 00 00 2F 00 00 00 4A E2 FF FF 14 00 00 00 17 00 00 00 05 21 FF FF 47 00 00 00 47 00 00 00 30 40 FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower12Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 5C 00 00 00 5C 00 00 00 15 20 01 00 2F 00 00 00 2F 00 00 00 4A E2 FF FF 14 00 00 00 17 00 00 00 05 21 FF FF 47 00 00 00 47 00 00 00 30 40 FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower13Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 5C 00 00 00 5C 00 00 00 15 20 01 00 2F 00 00 00 2F 00 00 00 4A E2 FF FF 14 00 00 00 17 00 00 00 05 21 FF FF 47 00 00 00 47 00 00 00 30 40 FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower14Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 5C 00 00 00 5C 00 00 00 15 20 01 00 2F 00 00 00 2F 00 00 00 4A E2 FF FF 14 00 00 00 17 00 00 00 05 21 FF FF 47 00 00 00 47 00 00 00 30 40 FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPower15Response = "4d 54 01 83 54 00 00 00 4C 00 01 00 02 00 73 00 5C 00 00 00 5C 00 00 00 15 20 01 00 2F 00 00 00 2F 00 00 00 4A E2 FF FF 14 00 00 00 17 00 00 00 05 21 FF FF 47 00 00 00 47 00 00 00 30 40 FF FF 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";

    constexpr size_t numFrequencies = 3;

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)

        SET_REQUEST_RESPONSE_EXPECTATION(setRiscModeDisabledRequest, setRiscModeDisabledResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(startRxCalibrationRequest, startRxCalibrationResponse)

        for (size_t i = 0; i < numFrequencies; i++) {
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand0Request, setRxBand0Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower0Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand1Request, setRxBand1Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower1Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand2Request, setRxBand2Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower2Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand3Request, setRxBand3Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower3Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand4Request, setRxBand4Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower4Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand5Request, setRxBand5Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower5Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand6Request, setRxBand6Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower6Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand7Request, setRxBand7Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower7Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand8Request, setRxBand8Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower8Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand9Request, setRxBand9Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower9Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand10Request, setRxBand10Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower10Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand11Request, setRxBand11Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower11Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand12Request, setRxBand12Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower12Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand13Request, setRxBand13Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower13Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand14Request, setRxBand14Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower14Response)
            SET_REQUEST_RESPONSE_EXPECTATION(setRxBand15Request, setRxBand15Response)
            SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower15Response)
        }

        SET_REQUEST_RESPONSE_EXPECTATION(stopRxCalibrationRequest, stopRxCalibrationResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setRiscModeEnabledRequest, setRiscModeEnabledResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    ASSERT_TRUE(m_dut.startRxCalibration());

    for (size_t i = 0; i < numFrequencies; i++) {
        ASSERT_TRUE(m_dut.measureRxLnaSubBandGains());
    }

    ASSERT_TRUE(m_dut.stopRxCalibration());

    std::array<std::array<uint16_t, dut::maxNumLnaSubBands>, dut::maxNumRxAntennas> freqResultTable;

    ASSERT_TRUE(m_dut.calculateRxLnaSubBandGains(freqResultTable));

    uint16_t expectedFreq = 4900;
    EXPECT_EQ(expectedFreq, freqResultTable[0][0]);
}

TEST_F(DutWithConnectionMockTest, calculateRxRfFlatnessShouldSucceed)
{
    const std::array<std::array<int32_t, dut::maxNumLnaSubBands>, dut::maxNumRxAntennas> P_Adc_array {
        {
            //
            { 107, 17, 161, 251, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 149, 125, 245, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 170, 155, 176, 185, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 161, 137, 203, 239, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } //
        }
    };
    const std::array<float, dut::maxNumRxAntennas> refGain { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    int8_t pgc1 = 12;
    int8_t pgc2 = 0;
    int8_t pgc3 = 0;
    const std::array<std::array<float, dut::maxNumLnaSubBands>, dut::maxNumRxAntennas> pin {
        {
            //
            { -45.1672554f, -45.0204544f, -44.9429283f, -44.7631493f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
            { -44.8112564f, -44.6304550f, -44.5169296f, -44.5531502f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
            { -45.0912552f, -45.4904556f, -45.7849312f, -46.0831490f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
            { -44.9212532f, -44.8104553f, -44.6389313f, -44.3831482f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } //
        }
    };
    std::array<std::array<float, dut::maxNumLnaSubBands>, dut::maxNumRxAntennas> results;

    ASSERT_TRUE(m_dut.calculateRxRfFlatness(P_Adc_array, refGain, pgc1, pgc2, pgc3, pin, results));

    const std::array<std::array<float, dut::maxNumLnaSubBands>, dut::maxNumRxAntennas> expectedResults {
        {
            //
            { 36.51101f, 33.55170f, 37.97418f, 40.60690f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f },
            { 37.46751f, 36.53671f, 40.17318f, 39.92815f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f },
            { 38.40376f, 38.33421f, 39.28493f, 39.86440f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f },
            { 37.95250f, 37.09171f, 38.98268f, 39.85190f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f },
            { -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f, -12.0f } //
        }
    };
    for (uint8_t i = 0; i < dut::maxNumRxAntennas; i++) {
        for (uint8_t j = 0; j < dut::maxNumLnaSubBands; j++) {
            const uint8_t precision = 5;
            EXPECT_EQ(dut::toString(expectedResults[i][j], precision), dut::toString(results[i][j], precision));
        }
    }
}

TEST_F(DutWithConnectionMockTest, calibrateRxLnaMidGainsShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto setRiscModeDisabledRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 3a 00 00 00 00 00";
    constexpr auto setRiscModeDisabledResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 3b 00 00 00 00 00";

    constexpr auto startRxCalibrationRequest = "4d 54 01 23 08 00 00 00 00 00 00 00 02 00 80 00";
    constexpr auto startRxCalibrationResponse = "4d 54 01 83 08 00 00 00 00 00 01 00 02 00 81 00";

    constexpr auto stopRxCalibrationRequest = "4d 54 01 23 08 00 00 00 00 00 00 00 02 00 82 00";
    constexpr auto stopRxCalibrationResponse = "4d 54 01 83 08 00 00 00 00 00 01 00 02 00 83 00";

    constexpr auto setRiscModeEnabledRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 3a 00 01 00 00 00";
    constexpr auto setRiscModeEnabledResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 3b 00 01 00 00 00";

    constexpr auto setLnaMidGains0Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 00 00 00 00 00";
    constexpr auto setLnaMidGains0Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 00 00 00 00 00";
    constexpr auto setLnaMidGains1Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 01 01 01 01 00";
    constexpr auto setLnaMidGains1Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 01 01 01 01 00";
    constexpr auto setLnaMidGains2Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 02 02 02 02 00";
    constexpr auto setLnaMidGains2Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 02 02 02 02 00";
    constexpr auto setLnaMidGains3Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 03 03 03 03 00";
    constexpr auto setLnaMidGains3Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 03 03 03 03 00";
    constexpr auto setLnaMidGains4Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 04 04 04 04 00";
    constexpr auto setLnaMidGains4Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 04 04 04 04 00";
    constexpr auto setLnaMidGains5Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 05 05 05 05 00";
    constexpr auto setLnaMidGains5Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 05 05 05 05 00";
    constexpr auto setLnaMidGains6Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 06 06 06 06 00";
    constexpr auto setLnaMidGains6Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 06 06 06 06 00";
    constexpr auto setLnaMidGains7Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 07 07 07 07 00";
    constexpr auto setLnaMidGains7Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 07 07 07 07 00";
    constexpr auto setLnaMidGains8Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 08 08 08 08 00";
    constexpr auto setLnaMidGains8Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 08 08 08 08 00";
    constexpr auto setLnaMidGains9Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 09 09 09 09 00";
    constexpr auto setLnaMidGains9Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 09 09 09 09 00";
    constexpr auto setLnaMidGains10Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 0a 0a 0a 0a 00";
    constexpr auto setLnaMidGains10Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 0a 0a 0a 0a 00";
    constexpr auto setLnaMidGains11Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 0b 0b 0b 0b 00";
    constexpr auto setLnaMidGains11Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 0b 0b 0b 0b 00";
    constexpr auto setLnaMidGains12Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 0c 0c 0c 0c 00";
    constexpr auto setLnaMidGains12Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 0c 0c 0c 0c 00";
    constexpr auto setLnaMidGains13Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 0d 0d 0d 0d 00";
    constexpr auto setLnaMidGains13Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 0d 0d 0d 0d 00";
    constexpr auto setLnaMidGains14Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 0e 0e 0e 0e 00";
    constexpr auto setLnaMidGains14Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 0e 0e 0e 0e 00";
    constexpr auto setLnaMidGains15Request = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 0f 0f 0f 0f 00";
    constexpr auto setLnaMidGains15Response = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 0f 0f 0f 0f 00";

    constexpr auto getRxCwPowerRequest = "4d 54 01 23 54 00 00 00 4c 00 00 00 02 00 72 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";

    constexpr auto getRxCwPower0Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 a0 fc ff ff d3 fc ff ff 82 c1 00 00 a3 fc ff ff ec fb ff ff f0 47 00 00 8e fc ff ff e9 fb ff ff 37 3c 00 00 e2 fc ff ff 8d fd ff ff 2a ff 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower1Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 bb fc ff ff df fc ff ff cc e4 00 00 94 fc ff ff ce fb ff ff 0f 30 00 00 94 fc ff ff f2 fb ff ff 44 43 00 00 d3 fc ff ff 87 fd ff ff af d9 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower2Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 be fc ff ff 2b fc ff ff 7e 6f 00 00 70 fc ff ff 16 fc ff ff 9d 45 00 00 7c fc ff ff 70 fc ff ff 4e 70 00 00 06 fd ff ff 60 fd ff ff 52 ee 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower3Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 cd fc ff ff 34 fc ff ff 33 7a 00 00 67 fc ff ff 55 fc ff ff 39 5c 00 00 67 fc ff ff 4f fc ff ff fc 58 00 00 15 fd ff ff 6c fd ff ff d9 1d 02 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower4Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 a3 fc ff ff d0 fc ff ff 84 c3 00 00 a0 fc ff ff ef fb ff ff 6c 45 00 00 7f fc ff ff fb fb ff ff 43 42 00 00 eb fc ff ff 8d fd ff ff 76 0e 02 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower5Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 a3 fc ff ff d0 fc ff ff 2c c4 00 00 94 fc ff ff d1 fb ff ff 84 35 00 00 6a fc ff ff 04 fc ff ff c8 3d 00 00 ee fc ff ff 8d fd ff ff 79 16 02 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower6Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 d6 fc ff ff 4c fc ff ff 48 8d 00 00 7c fc ff ff 1f fc ff ff 87 4f 00 00 67 fc ff ff 64 fc ff ff 7f 5f 00 00 12 fd ff ff 72 fd ff ff 21 28 02 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower7Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 ca fc ff ff 46 fc ff ff a0 83 00 00 7f fc ff ff 4c fc ff ff 85 60 00 00 55 fc ff ff 61 fc ff ff 76 57 00 00 0c fd ff ff 72 fd ff ff 10 1a 02 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower8Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 af fc ff ff d3 fc ff ff c9 ce 00 00 a6 fc ff ff dd fb ff ff 7d 3f 00 00 8e fc ff ff e6 fb ff ff 20 3a 00 00 e8 fc ff ff 93 fd ff ff 4e 14 02 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower9Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 a9 fc ff ff d3 fc ff ff f9 cb 00 00 a0 fc ff ff d4 fb ff ff c3 3b 00 00 94 fc ff ff d4 fb ff ff 0b 37 00 00 ee fc ff ff 87 fd ff ff 52 0b 02 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower10Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 d0 fc ff ff 46 fc ff ff 9d 81 00 00 6a fc ff ff 55 fc ff ff 64 5b 00 00 6a fc ff ff 58 fc ff ff a8 5e 00 00 1b fd ff ff 5a fd ff ff f6 04 02 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower11Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 d0 fc ff ff 34 fc ff ff 14 7c 00 00 76 fc ff ff 34 fc ff ff 02 54 00 00 7c fc ff ff 64 fc ff ff 74 6c 00 00 1e fd ff ff 5d fd ff ff 4a 13 02 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower12Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 be fc ff ff d3 fc ff ff 88 dd 00 00 a3 fc ff ff ce fb ff ff 02 3b 00 00 82 fc ff ff ef fb ff ff 02 38 00 00 dc fc ff ff 90 fd ff ff e0 f3 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower13Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 af fc ff ff d0 fc ff ff 41 cb 00 00 9d fc ff ff a1 fb ff ff b4 24 00 00 91 fc ff ff f2 fb ff ff a1 3f 00 00 dc fc ff ff 87 fd ff ff 9e e3 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower14Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 cd fc ff ff 55 fc ff ff 1c 8c 00 00 5b fc ff ff 43 fc ff ff 7e 4d 00 00 6a fc ff ff 67 fc ff ff 8d 62 00 00 09 fd ff ff 6f fd ff ff 9a 11 02 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";
    constexpr auto getRxCwPower15Response = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 d0 fc ff ff 46 fc ff ff 41 87 00 00 64 fc ff ff 40 fc ff ff 78 51 00 00 55 fc ff ff 40 fc ff ff f1 4a 00 00 06 fd ff ff 69 fd ff ff 9c 00 02 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 88 13 00 00 00 04 00 00";

    constexpr auto setLnaMidGainsRequest = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 84 00 0d 0f 0c 0f 00";
    constexpr auto setLnaMidGainsResponse = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 85 00 0d 0f 0c 0f 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)

        SET_REQUEST_RESPONSE_EXPECTATION(setRiscModeDisabledRequest, setRiscModeDisabledResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(startRxCalibrationRequest, startRxCalibrationResponse)

        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains0Request, setLnaMidGains0Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower0Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains1Request, setLnaMidGains1Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower1Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains2Request, setLnaMidGains2Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower2Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains3Request, setLnaMidGains3Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower3Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains4Request, setLnaMidGains4Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower4Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains5Request, setLnaMidGains5Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower5Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains6Request, setLnaMidGains6Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower6Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains7Request, setLnaMidGains7Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower7Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains8Request, setLnaMidGains8Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower8Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains9Request, setLnaMidGains9Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower9Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains10Request, setLnaMidGains10Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower10Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains11Request, setLnaMidGains11Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower11Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains12Request, setLnaMidGains12Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower12Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains13Request, setLnaMidGains13Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower13Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains14Request, setLnaMidGains14Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower14Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGains15Request, setLnaMidGains15Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPower15Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setLnaMidGainsRequest, setLnaMidGainsResponse)

        SET_REQUEST_RESPONSE_EXPECTATION(stopRxCalibrationRequest, stopRxCalibrationResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setRiscModeEnabledRequest, setRiscModeEnabledResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    ASSERT_TRUE(m_dut.startRxCalibration());

    std::array<float, dut::maxNumRxAntennas> targetGains { 20.0f, 20.0f, 20.0f, 20.0f, 0.0f };
    std::array<float, dut::maxNumRxAntennas> prxin { -60.0f, -60.0f, -60.0f, -60.0f, 0.0f };
    uint16_t numSamples = 1024;
    int8_t pgc1 = 0;
    int8_t pgc2 = 12;
    std::array<float, dut::maxNumRxAntennas> calcGain {};
    std::array<uint8_t, dut::maxNumRxAntennas> calcGainCtrl {};

    ASSERT_TRUE(m_dut.calibrateRxLnaMidGains(targetGains, prxin, numSamples, pgc1, pgc2, calcGain, calcGainCtrl));

    std::array<float, dut::maxNumRxAntennas> expectedCalcGain = { 21.46875f, 19.125f, 20.0625f, 24.1875f, 0.0f };
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        EXPECT_EQ(expectedCalcGain[i], calcGain[i]);
    }

    std::array<uint8_t, dut::maxNumRxAntennas> expectedCalcGainCtrl = { 13, 15, 12, 15, 0 };
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        EXPECT_EQ(expectedCalcGainCtrl[i], calcGainCtrl[i]);
    }

    ASSERT_TRUE(m_dut.stopRxCalibration());
}

TEST_F(DutWithConnectionMockTest, flushNvmShouldSucceed)
{
    constexpr auto flushNvmRequest = "4d 54 01 25 14 00 00 00 0c 00 00 00 02 00 06 00 02 00 00 00 00 00 00 00 01 00 00 00";
    constexpr auto flushNvmResponse = "4d 54 01 85 14 00 00 00 0c 00 01 00 02 00 07 00 02 00 00 00 00 00 00 00 01 00 00 00";

    constexpr auto recoveryRequest = "4d 54 01 25 14 00 00 00 0c 00 00 00 02 00 0c 00 02 00 00 00 00 00 00 00 01 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(flushNvmRequest, flushNvmResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(recoveryRequest, "")
    }

    ASSERT_TRUE(m_dut.flushNvm());

    ASSERT_TRUE(m_dut.driverRelease());

    m_tearDown = false;
}

TEST_F(DutWithConnectionMockTest, getAvailableRxAntennaMaskShouldSucceed)
{
    dut::AntennaMask antennaMask;

    ASSERT_TRUE(m_dut.getAvailableRxAntennaMask(antennaMask));

    EXPECT_EQ(antennaMask, 0x0f);
}

TEST_F(DutWithConnectionMockTest, getAvailableTxAntennaMaskShouldSucceed)
{
    dut::AntennaMask antennaMask;

    ASSERT_TRUE(m_dut.getAvailableTxAntennaMask(antennaMask));

    EXPECT_EQ(antennaMask, 0x0f);
}

TEST_F(DutWithConnectionMockTest, getBandShouldSucceed)
{
    dut::Band band;

    ASSERT_TRUE(m_dut.getBand(band));

    EXPECT_EQ(band, dut::Band::BAND_5000MHZ);
}

TEST_F(DutWithConnectionMockTest, getSupportedBandsShouldSucceed)
{
    std::vector<dut::Band> supportedBands;

    ASSERT_TRUE(m_dut.getSupportedBands(supportedBands));

    EXPECT_EQ(supportedBands.size(), 2);
    EXPECT_EQ(supportedBands.at(0), dut::Band::BAND_2400MHZ);
    EXPECT_EQ(supportedBands.at(1), dut::Band::BAND_5000MHZ);
}

TEST_F(DutWithConnectionMockTest, getBbicCddValuesShouldSucceed)
{
    constexpr auto getBbicCddValuesRequest = "4d 54 01 23 18 00 00 00 10 00 00 00 02 00 94 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto getBbicCddValuesResponse = "4d 54 01 83 18 00 00 00 10 00 01 00 02 00 95 00 01 00 00 00 01 00 00 00 02 00 00 00 03 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getBbicCddValuesRequest, getBbicCddValuesResponse)
    }

    uint8_t numTxAntennas = 1;
    uint32_t offset1 = 0;
    uint32_t offset2 = 0;
    uint32_t offset3 = 0;

    ASSERT_TRUE(m_dut.getBbicCddValues(numTxAntennas, offset1, offset2, offset3));

    EXPECT_EQ(offset1, 1);
    EXPECT_EQ(offset2, 2);
    EXPECT_EQ(offset3, 3);
}

TEST_F(DutWithConnectionMockTest, getCardInfoShouldSucceed)
{
    uint8_t countryCode = 0;
    std::array<uint8_t, dut::cardInfoMacAddressSize> macAddress {};
    std::array<uint8_t, dut::cardInfoSerialNumberSize> serialNumber {};
    uint8_t week = 0;
    uint8_t year = 0;

    ASSERT_TRUE(m_dut.getCardInfo(countryCode, macAddress, serialNumber, week, year));

    const uint8_t expectedCountryCode = 65;
    const uint8_t expectedMacAddress[] { 0xac, 0x9a, 0x96, 0xf1, 0xbb, 0x20 };
    const uint8_t expectedSerialNumber[] { 0xdb, 0x07, 0x00 };
    const uint8_t expectedWeek = 41;
    const uint8_t expectedYear = 15;

    EXPECT_EQ(countryCode, expectedCountryCode);
    EXPECT_TRUE(memcmp(macAddress.data(), expectedMacAddress, sizeof(expectedMacAddress)) == 0);
    EXPECT_TRUE(memcmp(serialNumber.data(), expectedSerialNumber, sizeof(expectedSerialNumber)) == 0);
    EXPECT_EQ(week, expectedWeek);
    EXPECT_EQ(year, expectedYear);
}

TEST_F(DutWithConnectionMockTest, getChipIdShouldSucceed)
{
    dut::ChipID chipId = dut::ChipID::CHIP_ID_INVALID;

    ASSERT_TRUE(m_dut.getChipId(chipId));

    const dut::ChipID expectedChipId = dut::ChipID::CHIP_ID_GEN6_D2;

    EXPECT_EQ(chipId, expectedChipId);
}

TEST_F(DutWithConnectionMockTest, getComponentVersionShouldSucceed)
{
    std::string version;

    ASSERT_TRUE(m_dut.getComponentVersion(dut::VersionedComponent::VERSIONED_COMPONENT_CV, version));

    const std::string expectedVersion = "06.01.00.1104";

    EXPECT_EQ(version, expectedVersion);
}

TEST_F(DutWithConnectionMockTest, getEnabledRxAntennaMaskShouldSucceed)
{
    dut::AntennaMask antennaMask;

    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(antennaMask));

    EXPECT_EQ(antennaMask, 0x0f);
}

TEST_F(DutWithConnectionMockTest, getEnabledTxAntennaMaskShouldSucceed)
{
    dut::AntennaMask antennaMask;

    ASSERT_TRUE(m_dut.getEnabledTxAntennaMask(antennaMask));

    EXPECT_EQ(antennaMask, 0x0f);
}

TEST_F(DutWithConnectionMockTest, getFemTypeShouldSucceed)
{
    constexpr auto getFemTypeRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 9e 00 00 00 00 00";
    constexpr auto getFemTypeResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 9f 00 01 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getFemTypeRequest, getFemTypeResponse)
    }

    dut::FemType femType = dut::FemType::FEM_TYPE_INVALID;

    ASSERT_TRUE(m_dut.getFemType(femType));

    EXPECT_EQ(femType, dut::FemType::FEM_TYPE_NON_LINEAR);
}

TEST_F(DutWithConnectionMockTest, getHardwareTypeShouldSucceed)
{
    dut::HardwareType hardwareType = dut::HardwareType::HARDWARE_TYPE_INVALID;

    ASSERT_TRUE(m_dut.getHardwareType(hardwareType));

    EXPECT_EQ(hardwareType, dut::HardwareType::HARDWARE_TYPE_GEN6);
}

TEST_F(DutWithConnectionMockTest, getInbandRssiShouldSucceed)
{
    constexpr auto getInbandRssiRequest = "4d 54 01 23 0e 00 00 00 06 00 00 00 02 00 46 00 00 00 00 00 00 00";
    constexpr auto getInbandRssiResponse = "4d 54 01 83 0e 00 00 00 05 00 01 00 02 00 47 00 9a 9a 9c 9b 01 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getInbandRssiRequest, getInbandRssiResponse)
    }

    std::array<int16_t, dut::maxNumRxAntennas> rssi {};

    ASSERT_TRUE(m_dut.getInbandRssi(rssi));

    const std::array<int16_t, dut::maxNumRxAntennas> expectedRssi { -102, -102, -100, -101, 0 };
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        EXPECT_EQ(rssi[i], expectedRssi[i]);
    }
}

TEST_F(DutWithConnectionMockTest, getMacPacketCountersShouldSucceed)
{
    constexpr auto getMacPacketCountersRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 5c 00 00 00 00 00";
    constexpr auto getMacPacketCountersResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 5d 00 01 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getMacPacketCountersRequest, getMacPacketCountersResponse)
    }

    uint32_t receivedPackets = 0;

    ASSERT_TRUE(m_dut.getMacPacketCounters(receivedPackets));

    EXPECT_EQ(receivedPackets, 1);
}

TEST_F(DutWithConnectionMockTest, getMpduPacketCountersShouldSucceed)
{
    constexpr auto getMpduPacketCountersRequest = "4d 54 01 23 18 00 00 00 10 00 00 00 02 00 36 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00";
    constexpr auto getMpduPacketCountersResponse = "4d 54 01 83 18 00 00 00 10 00 01 00 02 00 37 00 01 00 00 00 02 00 00 00 00 00 00 00 00 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getMpduPacketCountersRequest, getMpduPacketCountersResponse)
    }

    uint32_t receivedPackets = 0;
    uint32_t errorPackets = 0;

    ASSERT_TRUE(m_dut.getMpduPacketCounters(receivedPackets, errorPackets));

    EXPECT_EQ(receivedPackets, 1);
    EXPECT_EQ(errorPackets, 2);
}

TEST_F(DutWithConnectionMockTest, getNmseValuesShouldSucceed)
{
    constexpr auto getNmseValuesRequest = "4d 54 01 23 f8 00 00 00 f0 00 00 00 02 00 a0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto getNmseValuesResponse = "4d 54 01 83 f8 00 00 00 f0 00 01 00 02 00 a1 00 01 00 00 00 02 00 00 00 03 00 00 00 04 00 00 00 05 00 00 00 06 00 00 00 07 00 00 00 08 00 00 00 09 00 00 00 0a 00 00 00 0b 00 00 00 0c 00 00 00 0d 00 00 00 0e 00 00 00 0f 00 00 00 01 00 00 00 02 00 00 00 03 00 00 00 04 00 00 00 05 00 00 00 06 00 00 00 07 00 00 00 08 00 00 00 09 00 00 00 0a 00 00 00 0b 00 00 00 0c 00 00 00 0d 00 00 00 0e 00 00 00 0f 00 00 00 01 00 00 00 02 00 00 00 03 00 00 00 04 00 00 00 05 00 00 00 06 00 00 00 07 00 00 00 08 00 00 00 09 00 00 00 0a 00 00 00 0b 00 00 00 0c 00 00 00 0d 00 00 00 0e 00 00 00 0f 00 00 00 01 00 00 00 02 00 00 00 03 00 00 00 04 00 00 00 05 00 00 00 06 00 00 00 07 00 00 00 08 00 00 00 09 00 00 00 0a 00 00 00 0b 00 00 00 0c 00 00 00 0d 00 00 00 0e 00 00 00 0f 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getNmseValuesRequest, getNmseValuesResponse)
    }

    std::array<std::array<int32_t, dut::dpdTotalCalibrationPoints>, dut::maxNumTxAntennas> values {};

    ASSERT_TRUE(m_dut.getNmseValues(values));

    for (size_t i = 0; i < dut::maxNumTxAntennas; i++) {
        for (size_t j = 0; j < dut::dpdTotalCalibrationPoints; j++) {
            EXPECT_EQ(values[i][j], j + 1)
                << "with antenna " << i << " and calibration point " << j;
        }
    }
}

TEST_F(DutWithConnectionMockTest, getPhyPacketCountersShouldSucceed)
{
    constexpr auto getPhyPacketCountersRequest = "4d 54 01 23 18 00 00 00 10 00 00 00 02 00 36 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto getPhyPacketCountersResponse = "4d 54 01 83 18 00 00 00 10 00 01 00 02 00 37 00 01 00 00 00 02 00 00 00 03 00 00 00 00 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getPhyPacketCountersRequest, getPhyPacketCountersResponse)
    }

    uint32_t receivedPackets = 0;
    uint32_t crcErrors = 0;
    uint32_t forwardedPackets = 0;

    ASSERT_TRUE(m_dut.getPhyPacketCounters(receivedPackets, crcErrors, forwardedPackets));

    EXPECT_EQ(receivedPackets, 1);
    EXPECT_EQ(crcErrors, 2);
    EXPECT_EQ(forwardedPackets, 3);
}

TEST_F(DutWithConnectionMockTest, getProductionFlagShouldSucceed)
{
    bool productionFlag = true;

    ASSERT_TRUE(m_dut.getProductionFlag(productionFlag));

    EXPECT_FALSE(productionFlag);
}

TEST_F(DutWithConnectionMockTest, getRfRssiPowerShouldSucceed)
{
    constexpr auto getRfRssiPowerRequest = "4d 54 01 23 16 00 00 00 0e 00 00 00 02 00 9c 00 0f 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto getRfRssiPowerResponse = "4d 54 01 83 16 00 00 00 0e 00 01 00 02 00 9d 00 0f 00 00 00 10 00 20 00 30 00 00 01 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getRfRssiPowerRequest, getRfRssiPowerResponse)
    }

    std::array<int8_t, dut::maxNumRxAntennas> power {};

    ASSERT_TRUE(m_dut.getRfRssiPower(power));

    const std::array<int8_t, dut::maxNumRxAntennas> expectedPower { 8, 16, 24, -128, 0 };
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        EXPECT_EQ(power[i], expectedPower[i]);
    }
}

TEST_F(DutWithConnectionMockTest, getRssiShouldSucceed)
{
    constexpr auto getRssiRequest = "4d 54 01 23 1c 00 00 00 14 00 00 00 02 00 76 00 0f 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 04";
    constexpr auto getRssiResponse = "4d 54 01 83 1c 00 00 00 14 00 01 00 02 00 77 00 0f 00 00 00 00 00 00 00 13 02 37 02 45 02 e6 01 00 00 00 04";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getRssiRequest, getRssiResponse)
    }

    uint8_t method = 0;
    uint16_t numSamples = 1024;
    std::array<uint16_t, dut::maxNumRxAntennas> rssi {};

    ASSERT_TRUE(m_dut.getRssi(method, numSamples, rssi));

    const std::array<uint16_t, dut::maxNumRxAntennas> expectedRssi { 531, 567, 581, 486, 0 };
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        EXPECT_EQ(rssi[i], expectedRssi[i]);
    }
}

TEST_F(DutWithConnectionMockTest, getRxCwPowerShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto setRiscModeDisabledRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 3a 00 00 00 00 00";
    constexpr auto setRiscModeDisabledResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 3b 00 00 00 00 00";

    constexpr auto startRxCalibrationRequest = "4d 54 01 23 08 00 00 00 00 00 00 00 02 00 80 00";
    constexpr auto startRxCalibrationResponse = "4d 54 01 83 08 00 00 00 00 00 01 00 02 00 81 00";

    constexpr auto stopRxCalibrationRequest = "4d 54 01 23 08 00 00 00 00 00 00 00 02 00 82 00";
    constexpr auto stopRxCalibrationResponse = "4d 54 01 83 08 00 00 00 00 00 01 00 02 00 83 00";

    constexpr auto setRiscModeEnabledRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 3a 00 01 00 00 00";
    constexpr auto setRiscModeEnabledResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 3b 00 01 00 00 00";

    constexpr auto getRxCwPowerRequest = "4d 54 01 23 54 00 00 00 4c 00 00 00 02 00 72 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";
    constexpr auto getRxCwPowerResponse = "4d 54 01 83 54 00 00 00 4c 00 01 00 02 00 73 00 7b ff ff ff 7e ff ff ff a2 68 00 00 d2 ff ff ff cf ff ff ff cA 46 00 00 90 ff ff ff 93 ff ff ff 9b 2d 00 00 a8 ff ff ff a8 ff ff ff c8 51 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)

        SET_REQUEST_RESPONSE_EXPECTATION(setRiscModeDisabledRequest, setRiscModeDisabledResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(startRxCalibrationRequest, startRxCalibrationResponse)

        SET_REQUEST_RESPONSE_EXPECTATION(getRxCwPowerRequest, getRxCwPowerResponse)

        SET_REQUEST_RESPONSE_EXPECTATION(stopRxCalibrationRequest, stopRxCalibrationResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setRiscModeEnabledRequest, setRiscModeEnabledResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    ASSERT_TRUE(m_dut.startRxCalibration());

    uint8_t method = 0;
    int32_t freqOffset = 0;
    uint16_t numSamples = 1024;
    std::array<dut::CorrelationResults_t, dut::maxNumRxAntennas> correlationResults {};

    ASSERT_TRUE(m_dut.getRxCwPower(method, freqOffset, numSamples, correlationResults));

    std::array<dut::CorrelationResults_t, dut::maxNumRxAntennas> expectedCorrelationResults { { { -133, -130, 26786 }, { -46, -49, 18122 }, { -112, -109, 11675 }, { -88, -88, 20936 }, { 0, 0, 0 } } };

    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        EXPECT_EQ(correlationResults[i].II, expectedCorrelationResults[i].II);
        EXPECT_EQ(correlationResults[i].QQ, expectedCorrelationResults[i].QQ);
        EXPECT_EQ(correlationResults[i].IQ, expectedCorrelationResults[i].IQ);
    }

    ASSERT_TRUE(m_dut.stopRxCalibration());

    const std::array<float, dut::maxNumRxAntennas> prxin { -40, 0, 0, 0, 0 };
    int8_t pgc1 = 0;
    int8_t pgc2 = 0;
    int8_t pgc3 = 0;
    std::array<float, dut::maxNumRxAntennas> gains;
    ASSERT_TRUE(m_dut.calculateRxLnaGains(correlationResults, prxin, pgc1, pgc2, pgc3, gains));

    float expectedGain = 35.84375f;
    EXPECT_EQ(expectedGain, gains[0]);
}

TEST_F(DutWithConnectionMockTest, getRxEvmShouldSucceed)
{
    constexpr auto getRxEvmRequest = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 3c 00 00 00 00 00 00";
    constexpr auto getRxEvmResponse = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 3d 00 01 02 03 04 05";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getRxEvmRequest, getRxEvmResponse)
    }

    std::array<uint8_t, dut::maxNumRxAntennas> rxEvm;

    ASSERT_TRUE(m_dut.getRxEvm(rxEvm));

    const std::array<uint8_t, dut::maxNumRxAntennas> expectedRxEvm { 1, 2, 3, 4, 5 };
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        EXPECT_EQ(rxEvm[i], expectedRxEvm[i]);
    }
}

TEST_F(DutWithConnectionMockTest, getRxRateInfoShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto getRxRateInfoRequest = "4d 54 01 23 0a 00 00 00 02 00 00 00 02 00 aa 00 00 00";
    constexpr auto getRxRateInfoResponse = "4d 54 01 83 0a 00 00 00 02 00 01 00 02 00 ab 00 07 02";
    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(getRxRateInfoRequest, getRxRateInfoResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;

    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    uint8_t mcs = 0;
    uint8_t nss = 0;

    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    ASSERT_TRUE(m_dut.getRxRateInfo(mcs, nss));

    EXPECT_EQ(mcs, 7);
    EXPECT_EQ(nss, 2);
}

TEST_F(DutWithConnectionMockTest, getTransmitPowerTableOffsetShouldSucceed)
{
    constexpr auto getTransmitPowerTableOffsetRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 8a 00 01 00 00 00";
    constexpr auto getTransmitPowerTableOffsetResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 8b 00 01 00 02 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getTransmitPowerTableOffsetRequest, getTransmitPowerTableOffsetResponse)
    }

    uint8_t antenna = 1;
    dut::Bandwidth bandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    int16_t offset;

    ASSERT_TRUE(m_dut.getTransmitPowerTableOffset(antenna, bandwidth, offset));

    EXPECT_EQ(offset, 2);
}

TEST_F(DutWithConnectionMockTest, getTransmitVoltagesShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto setRateRequest = "4d 54 01 23 0e 00 00 00 06 00 00 00 02 00 66 00 02 00 04 02 00 00";
    constexpr auto setRateResponse = "4d 54 01 83 0e 00 00 00 06 00 01 00 02 00 67 00 02 00 04 02 00 00";

    constexpr auto setTransmitPowerLevelAntenna1Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 00 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna1Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 00 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna2Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 01 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna2Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 01 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna3Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 02 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna3Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 02 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna4Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 03 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna4Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 03 00 22 44";

    constexpr auto startTxRequest = "4d 54 01 23 14 00 00 00 0c 00 00 00 02 00 00 02 a0 0f 00 00 ff ff 01 00 00 01 00 00";
    constexpr auto startTxResponse = "4d 54 01 83 14 00 00 00 0c 00 01 00 02 00 01 02 a0 0f 00 00 ff ff 01 00 00 01 00 00";

    constexpr auto getTransmitVoltagesRequest = "4d 54 01 23 24 00 00 00 1c 00 00 00 02 00 3e 00 05 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto getTransmitVoltagesResponse = "4d 54 01 83 24 00 00 00 1c 00 01 00 02 00 3f 00 05 00 00 00 05 00 00 00 00 00 00 00 4b 18 01 00 a6 16 01 00 5e 17 01 00 71 13 01 00";

    constexpr auto stopTxRequest = "4d 54 01 23 08 00 00 00 00 00 00 00 02 00 02 02";
    constexpr auto stopTxResponse = "4d 54 01 83 08 00 00 00 00 00 01 00 02 00 03 02";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setRateRequest, setRateResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna1Request, setTransmitPowerLevelAntenna1Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna2Request, setTransmitPowerLevelAntenna2Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna3Request, setTransmitPowerLevelAntenna3Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna4Request, setTransmitPowerLevelAntenna4Response)
        SET_REQUEST_RESPONSE_EXPECTATION(startTxRequest, startTxResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(getTransmitVoltagesRequest, getTransmitVoltagesResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(stopTxRequest, stopTxResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));

    uint16_t repetitions = UINT16_MAX;
    uint32_t packetLength = 4000;
    bool longData = false;
    bool beamforming = false;

    ASSERT_TRUE(m_dut.startTx(repetitions, packetLength, longData, beamforming));

    std::array<uint32_t, dut::maxNumTxAntennas> voltages;

    ASSERT_TRUE(m_dut.getTransmitVoltages(voltages));

    const std::array<double, dut::maxNumTxAntennas> expectedVoltages { 14351, 14266, 14303, 14102 };
    for (size_t i = 0; i < dut::maxNumTxAntennas; i++) {
        EXPECT_EQ(voltages[i], expectedVoltages[i]);
    }

    ASSERT_TRUE(m_dut.stopTx());
}

TEST_F(DutWithConnectionMockTest, getXtalCalValueShouldSucceed)
{
    uint16_t xtalValue = 0;

    ASSERT_TRUE(m_dut.getXtalCalValue(xtalValue));

    EXPECT_EQ(xtalValue, 0x3c);
}

TEST_F(DutWithConnectionMockTest, getXtalRegValueShouldSucceed)
{
    constexpr auto getXtalValueRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 42 00 00 00 00 00";
    constexpr auto getXtalValueResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 43 00 3c 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getXtalValueRequest, getXtalValueResponse)
    }

    uint16_t xtalValue = 0;

    ASSERT_TRUE(m_dut.getXtalRegValue(xtalValue));

    EXPECT_EQ(xtalValue, 0x3c);
}

TEST_F(DutWithConnectionMockTest, getZwdfsStatusShouldSucceed)
{
    dut::AntennaMask antennaMask;
    bool enabled;

    ASSERT_TRUE(m_dut.getZwdfsStatus(antennaMask, enabled));

    EXPECT_EQ(antennaMask, 0x00);
    EXPECT_FALSE(enabled);
}

TEST_F(DutWithConnectionMockTest, loadNvmFromFileShouldSucceed)
{
    EmbeddedResource resource(IDR_CALIBRATIONFILE1, CALIBRATIONFILE);
    TemporaryFile temporaryFile(resource.getData(), resource.getSize());

    std::string fileName = temporaryFile.getFilename();

    uint16_t xtalValue = 0;

    ASSERT_TRUE(m_dut.getXtalCalValue(xtalValue));
    EXPECT_EQ(xtalValue, 0x3c);

    dut::CalibrationFileVersion version;
    std::vector<dut::RssiCalibrationData_t> rssiCalibrationData {};
    std::vector<dut::TssiCalibrationData_t> tssiCalibrationData {};

    ASSERT_TRUE(m_dut.getRssiCalibrationData(version, rssiCalibrationData));
    EXPECT_EQ(version, dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6);
    EXPECT_EQ(rssiCalibrationData.size(), 0);

    ASSERT_TRUE(m_dut.getTssiCalibrationData(version, tssiCalibrationData));
    EXPECT_EQ(version, dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6);
    EXPECT_EQ(tssiCalibrationData.size(), 0);

    ASSERT_TRUE(m_dut.loadNvmFromFile(fileName));

    ASSERT_TRUE(m_dut.getXtalCalValue(xtalValue));
    EXPECT_EQ(xtalValue, 0x3f);

    ASSERT_TRUE(m_dut.getRssiCalibrationData(version, rssiCalibrationData));
    EXPECT_EQ(version, dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6);
    EXPECT_EQ(rssiCalibrationData.size(), 1);

    ASSERT_TRUE(m_dut.getTssiCalibrationData(version, tssiCalibrationData));
    EXPECT_EQ(version, dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6);
    EXPECT_EQ(tssiCalibrationData.size(), 1);
}

TEST_F(DutWithConnectionMockTest, readMemoryShouldSucceed)
{
    constexpr auto readMemoryRequest = "4d 54 01 23 1a 00 00 00 12 00 00 00 02 00 0e 00 04 00 0a 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto readMemoryResponse = "4d 54 01 83 1a 00 00 00 12 00 01 00 02 00 0f 00 04 00 0a 00 01 00 00 00 0d dd 0d dd 0d dd 0d dd 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(readMemoryRequest, readMemoryResponse)
    }

    dut::ChipModule chipModule = dut::ChipModule::CHIP_MODULE_RF;
    size_t address = 0x1;
    std::array<uint8_t, 10> buffer;
    size_t length = buffer.size();

    ASSERT_TRUE(m_dut.readMemory(chipModule, address, buffer.data(), length));

    std::vector<uint8_t> expectedData { 0x0d, 0xdd, 0x0d, 0xdd, 0x0d, 0xdd, 0x0d, 0xdd, 0x00, 0x00 };
    EXPECT_EQ(memcmp(expectedData.data(), buffer.data(), length), 0);
}

TEST_F(DutWithConnectionMockTest, readNvmShouldSucceed)
{
    constexpr auto readNvmRequest = "4d 54 01 25 45 00 00 00 3d 00 00 00 02 00 02 00 02 00 00 00 00 00 00 00 00 04 00 00 29 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto readNvmResponse = "4d 54 01 85 45 00 00 00 3d 00 01 00 02 00 03 00 02 00 00 00 00 00 00 00 00 08 00 00 29 00 00 00 00 00 00 00 fc 1b 30 00 00 00 00 00 30 1a 80 09 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff ff ff ff ff ff ff ff ff ff ff 06";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(readNvmRequest, readNvmResponse)
    }

    size_t address = 0;
    std::array<uint8_t, dut::NonVolatileMemory::nvmVersionAddress + 1> buffer;
    bool useCache = false;

    ASSERT_TRUE(m_dut.readNvm(address, buffer.data(), buffer.size(), useCache));
}

TEST_F(DutWithConnectionMockTest, readNvmShouldSucceedUsingCache)
{
    EmbeddedResource resource(IDR_CALIBRATIONFILE1, CALIBRATIONFILE);
    TemporaryFile temporaryFile(resource.getData(), resource.getSize());

    std::string fileName = temporaryFile.getFilename();

    ASSERT_TRUE(m_dut.loadNvmFromFile(fileName));

    size_t address = 0;
    std::array<uint8_t, 32> buffer;
    bool useCache = true;

    ASSERT_TRUE(m_dut.readNvm(address, buffer.data(), buffer.size(), useCache));

    EXPECT_EQ(memcmp(resource.getData(), buffer.data(), buffer.size()), 0);
}

TEST_F(DutWithConnectionMockTest, readRegisterShouldSucceed)
{
    constexpr auto readRegisterRequest = "4d 54 01 23 14 00 00 00 0c 00 00 00 02 00 0e 00 03 00 04 00 9c 01 b0 a0 00 00 00 00";
    constexpr auto readRegisterResponse = "4d 54 01 83 14 00 00 00 0c 00 01 00 02 00 0f 00 03 00 04 00 9c 01 b0 a0 01 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(readRegisterRequest, readRegisterResponse)
    }

    dut::ChipModule chipModule = dut::ChipModule::CHIP_MODULE_PHY;
    size_t address = 0xa0b0019c;
    uint32_t mask = 0x1;
    uint32_t value;

    ASSERT_TRUE(m_dut.readRegister(chipModule, address, mask, value));

    EXPECT_EQ(value, 0x1);
}

TEST_F(DutWithConnectionMockTest, resetMacPacketCountersShouldSucceed)
{
    constexpr auto resetMacPacketCountersRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 5a 00 00 00 00 00";
    constexpr auto resetMacPacketCountersResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 5b 00 00 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(resetMacPacketCountersRequest, resetMacPacketCountersResponse)
    }

    ASSERT_TRUE(m_dut.resetMacPacketCounters());
}

TEST_F(DutWithConnectionMockTest, resetMpduPacketCountersShouldSucceed)
{
    constexpr auto resetMpduPacketCountersRequest = "4d 54 01 23 18 00 00 00 10 00 00 00 02 00 36 00 00 00 00 00 00 00 00 00 01 00 00 00 01 00 00 00";
    constexpr auto resetMpduPacketCountersResponse = "4d 54 01 83 18 00 00 00 10 00 01 00 02 00 37 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(resetMpduPacketCountersRequest, resetMpduPacketCountersResponse)
    }

    ASSERT_TRUE(m_dut.resetMpduPacketCounters());
}

TEST_F(DutWithConnectionMockTest, resetPhyPacketCountersShouldSucceed)
{
    constexpr auto resetPhyPacketCountersRequest = "4d 54 01 23 18 00 00 00 10 00 00 00 02 00 36 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00";
    constexpr auto resetPhyPacketCountersResponse = "4d 54 01 83 18 00 00 00 10 00 01 00 02 00 37 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(resetPhyPacketCountersRequest, resetPhyPacketCountersResponse)
    }

    ASSERT_TRUE(m_dut.resetPhyPacketCounters());
}

TEST_F(DutWithConnectionMockTest, rxMeasureReqShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto rxMeasureRequest = "4d 54 01 23 14 00 00 00 0c 00 00 00 02 00 a8 00 0a 00 00 00 10 27 00 00 04 01 00 00";
    constexpr auto rxMeasureResponse = "4d 54 01 83 14 00 00 00 0c 00 01 00 02 00 a9 00 0a 00 00 00 10 27 00 00 04 01 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(rxMeasureRequest, rxMeasureResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    uint32_t numCapture = 10;
    uint32_t interval = 10;
    bool disabled = false;

    ASSERT_TRUE(m_dut.rxMeasure(numCapture, interval, disabled));
}

TEST_F(DutWithConnectionMockTest, setBbicCddValuesShouldSucceed)
{
    constexpr auto setBbicCddValuesRequest = "4d 54 01 23 18 00 00 00 10 00 00 00 02 00 96 00 01 00 00 00 01 00 00 00 02 00 00 00 03 00 00 00";
    constexpr auto setBbicCddValuesResponse = "4d 54 01 83 18 00 00 00 10 00 01 00 02 00 97 00 01 00 00 00 01 00 00 00 02 00 00 00 03 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setBbicCddValuesRequest, setBbicCddValuesResponse)
    }

    uint8_t numTxAntennas = 1;
    uint32_t offset1 = 1;
    uint32_t offset2 = 2;
    uint32_t offset3 = 3;

    ASSERT_TRUE(m_dut.setBbicCddValues(numTxAntennas, offset1, offset2, offset3));
}

TEST_F(DutWithConnectionMockTest, setCardInfoShouldSucceed)
{
    const uint8_t expectedCountryCode = 65;
    std::array<uint8_t, dut::cardInfoMacAddressSize> expectedMacAddress { 0xfe, 0xed, 0xca, 0xfe, 0xba, 0xbe };
    std::array<uint8_t, dut::cardInfoSerialNumberSize> expectedSerialNumber { 0xbe, 0xef, 0x00 };
    const uint8_t expectedWeek = 42;
    const uint8_t expectedYear = 16;

    ASSERT_TRUE(m_dut.setCardInfo(expectedCountryCode, expectedMacAddress, expectedSerialNumber, expectedWeek, expectedYear));

    uint8_t countryCode = 0;
    std::array<uint8_t, dut::cardInfoMacAddressSize> macAddress {};
    std::array<uint8_t, dut::cardInfoSerialNumberSize> serialNumber {};
    uint8_t week = 0;
    uint8_t year = 0;

    ASSERT_TRUE(m_dut.getCardInfo(countryCode, macAddress, serialNumber, week, year));

    EXPECT_EQ(countryCode, expectedCountryCode);
    EXPECT_TRUE(memcmp(macAddress.data(), expectedMacAddress.data(), sizeof(expectedMacAddress)) == 0);
    EXPECT_TRUE(memcmp(serialNumber.data(), expectedSerialNumber.data(), sizeof(expectedSerialNumber)) == 0);
    EXPECT_EQ(week, expectedWeek);
    EXPECT_EQ(year, expectedYear);
}

TEST_F(DutWithConnectionMockTest, setChannelShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
}

TEST_F(DutWithConnectionMockTest, setChannelShouldSucceedWithPrimaryChannelIndex)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 01 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 01 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_FOURTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 1;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
}

TEST_F(DutWithConnectionMockTest, setClipperShouldSucceed)
{
    constexpr auto setClipperRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 ac 00 00";
    constexpr auto setClipperResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 ad 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setClipperRequest, setClipperResponse)
    }

    uint32_t enabled = false;

    ASSERT_TRUE(m_dut.setClipper(enabled));
}

TEST_F(DutWithConnectionMockTest, setEnabledRxAntennaMaskShouldSucceed)
{
    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 01";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 01";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
    }

    dut::AntennaMask antennaMask = 0x01;

    ASSERT_TRUE(m_dut.setEnabledRxAntennaMask(antennaMask));
}

TEST_F(DutWithConnectionMockTest, setEnabledTxAntennaMaskShouldSucceed)
{
    constexpr auto setEnabledTxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 24 00 01";
    constexpr auto setEnabledTxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 25 00 01";

    constexpr auto setTransmitPowerLevelAntenna1Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 00 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna1Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 00 00 22 44";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledTxAntennaMaskRequest, setEnabledTxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna1Request, setTransmitPowerLevelAntenna1Response)
    }

    dut::AntennaMask antennaMask = 0x01;

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(antennaMask));
}

TEST_F(DutWithConnectionMockTest, setHdkConfigShouldSucceed)
{
    constexpr auto setHdkConfigRequest = "4d 54 01 2b 2c 00 00 00 36 04 00 00 1c 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 88 07 00 00 0f 00 00 00 00 00 00 00";
    constexpr auto setHdkConfigResponse = "4d 54 01 8b 2c 00 00 00 36 04 00 00 1c 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 88 07 00 00 0f 00 00 00 00 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setHdkConfigRequest, setHdkConfigResponse)
    }

    uint32_t offlineCalMask = 0x0788;
    uint32_t onlineCalMask = 0x000f;

    ASSERT_TRUE(m_dut.setHdkConfig(offlineCalMask, onlineCalMask));
}

TEST_F(DutWithConnectionMockTest, setIfsShouldSucceed)
{
    constexpr auto setIfsRequest = "4d 54 01 23 0C 00 00 00 04 00 00 00 02 00 30 00 14 00 00 00";
    constexpr auto setIfsResponse = "4d 54 01 83 0C 00 00 00 04 00 01 00 02 00 31 00 14 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setIfsRequest, setIfsResponse)
    }

    uint32_t ifs = 20;

    ASSERT_TRUE(m_dut.setIfs(ifs));
}

TEST_F(DutWithConnectionMockTest, setProductionFlagShouldSucceed)
{
    constexpr auto setProductionFlagWriteRequest = "4d 54 01 25 1d 00 00 00 15 00 00 00 02 00 04 00 02 00 00 00 02 00 00 00 00 04 00 00 01 00 00 00 00 00 00 00 30";
    constexpr auto setProductionFlagWriteResponse = "4d 54 01 85 1d 00 00 00 15 00 01 00 02 00 05 00 02 00 00 00 02 00 00 00 00 08 00 00 01 00 00 00 00 00 00 00 30";

    constexpr auto setProductionFlagReadRequest = "4d 54 01 25 1d 00 00 00 15 00 00 00 02 00 02 00 02 00 00 00 02 00 00 00 00 04 00 00 01 00 00 00 00 00 00 00 00";
    constexpr auto setProductionFlagReadResponse = "4d 54 01 85 1d 00 00 00 15 00 01 00 02 00 03 00 02 00 00 00 02 00 00 00 00 08 00 00 01 00 00 00 00 00 00 00 30";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setProductionFlagWriteRequest, setProductionFlagWriteResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setProductionFlagReadRequest, setProductionFlagReadResponse)
    }

    bool productionFlag = false;
    bool writeToNvm = true;

    ASSERT_TRUE(m_dut.setProductionFlag(productionFlag, writeToNvm));
}

TEST_F(DutWithConnectionMockTest, setRateShouldFailIfChannelNotSet)
{
    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    EXPECT_FALSE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));
    EXPECT_EQ(m_dut.getLastError(), "Channel not set");
}

TEST_F(DutWithConnectionMockTest, setRateShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto setRateRequest = "4d 54 01 23 0e 00 00 00 06 00 00 00 02 00 66 00 02 00 04 02 00 00";
    constexpr auto setRateResponse = "4d 54 01 83 0e 00 00 00 06 00 01 00 02 00 67 00 02 00 04 02 00 00";

    constexpr auto setTransmitPowerLevelAntenna1Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 00 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna1Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 00 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna2Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 01 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna2Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 01 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna3Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 02 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna3Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 02 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna4Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 03 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna4Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 03 00 22 44";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setRateRequest, setRateResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna1Request, setTransmitPowerLevelAntenna1Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna2Request, setTransmitPowerLevelAntenna2Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna3Request, setTransmitPowerLevelAntenna3Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna4Request, setTransmitPowerLevelAntenna4Response)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));
}

TEST_F(DutWithConnectionMockTest, setRssiS2dParamsShouldSucceed)
{
    constexpr auto setRssiS2dParamsRequest = "4d 54 01 23 18 00 00 00 10 00 00 00 02 00 74 00 01 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00";
    constexpr auto setRssiS2dParamsResponse = "4d 54 01 83 18 00 00 00 10 00 01 00 02 00 75 00 01 00 00 00 00 00 00 00 0f 00 00 00 00 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setRssiS2dParamsRequest, setRssiS2dParamsResponse)
    }

    dut::AntennaMask antennaMask = 0x01;
    uint8_t region = 1;
    uint8_t gain = 0;
    uint8_t offset = 15;

    ASSERT_TRUE(m_dut.setRssiS2dParams(antennaMask, region, gain, offset));
}

TEST_F(DutWithConnectionMockTest, setRuParamsShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto setRateRequest = "4d 54 01 23 0e 00 00 00 06 00 00 00 02 00 66 00 02 00 04 02 00 00";
    constexpr auto setRateResponse = "4d 54 01 83 0e 00 00 00 06 00 01 00 02 00 67 00 02 00 04 02 00 00";

    constexpr auto setTransmitPowerLevelAntenna1Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 00 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna1Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 00 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna2Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 01 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna2Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 01 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna3Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 02 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna3Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 02 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna4Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 03 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna4Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 03 00 22 44";

    constexpr auto setRuParamsRequest = "4d 54 01 23 10 00 00 00 08 00 00 00 02 00 A2 00 64 00 00 00 C8 00 00 00";
    constexpr auto setruParamsResponse = "4d 54 01 83 10 00 00 00 08 00 01 00 02 00 A3 00 64 00 00 00 C8 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setRateRequest, setRateResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna1Request, setTransmitPowerLevelAntenna1Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna2Request, setTransmitPowerLevelAntenna2Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna3Request, setTransmitPowerLevelAntenna3Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna4Request, setTransmitPowerLevelAntenna4Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setRuParamsRequest, setruParamsResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));

    uint32_t userOne = 100;
    uint32_t userTwo = 200;

    ASSERT_TRUE(m_dut.setRuParams(userOne, userTwo));
}

TEST_F(DutWithConnectionMockTest, setRxAggregationEnabledShouldSucceed)
{
    constexpr auto setRxAggregationEnabledRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 8e 00 01 00 00 00";
    constexpr auto setRxAggregationEnabledResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 8f 00 01 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setRxAggregationEnabledRequest, setRxAggregationEnabledResponse)
    }

    ASSERT_TRUE(m_dut.setRxAggregationEnabled(true));
}

TEST_F(DutWithConnectionMockTest, setRxBandLutShouldSucceed)
{
    constexpr auto setRxBandLutRequest = "4d 54 01 23 56 00 00 00 4e 00 00 00 02 00 88 00 0f 00 00 00 00 00 00 00 24 13 24 13 24 13 24 13 24 13 22 15 c2 15 24 13 24 13 24 13 24 13 82 14 22 15 ea 15 24 13 24 13 24 13 24 13 24 13 8c 14 c2 15 24 13 24 13 24 13 e2 13 82 14 9a 15 c2 15 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setRxBandLutResponse = "4d 54 01 83 56 00 00 00 4e 00 01 00 02 00 89 00 0f 00 00 00 00 00 00 00 24 13 24 13 24 13 24 13 24 13 22 15 c2 15 24 13 24 13 24 13 24 13 82 14 22 15 ea 15 24 13 24 13 24 13 24 13 24 13 8c 14 c2 15 24 13 24 13 24 13 e2 13 82 14 9a 15 c2 15 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setRxBandLutRequest, setRxBandLutResponse)
    }

    dut::AntennaMask antennaMask = 0x0f;
    std::array<std::array<uint16_t, dut::maxNumLnaSubBandsCrossingPoints>, dut::maxNumRxAntennas> rxBandLut { { { 4900, 4900, 4900, 4900, 4900, 5410, 5570 },
        { 4900, 4900, 4900, 4900, 5250, 5410, 5610 },
        { 4900, 4900, 4900, 4900, 4900, 5260, 5570 },
        { 4900, 4900, 4900, 5090, 5250, 5530, 5570 },
        { 0, 0, 0, 0, 0, 0, 0 } } };
    std::array<uint8_t, dut::maxNumRxAntennas> firstTunningIndex { 0, 0, 0, 0, 0 };

    ASSERT_TRUE(m_dut.setRxBandLut(antennaMask, rxBandLut, firstTunningIndex));
}

TEST_F(DutWithConnectionMockTest, setRxGainBlocksShouldSucceed)
{
    constexpr auto setRxGainBlocksRequest = "4d 54 01 23 0d 00 00 00 05 00 00 00 02 00 70 00 0f 01 0c 00 00";
    constexpr auto setRxGainBlocksResponse = "4d 54 01 83 0d 00 00 00 05 00 01 00 02 00 71 00 0f 01 0c 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setRxGainBlocksRequest, setRxGainBlocksResponse)
    }

    uint8_t lnaIndex = 1;
    int8_t pgc1 = 12;
    int8_t pgc2 = 0;
    int8_t pgc3 = 0;

    ASSERT_TRUE(m_dut.setRxGainBlocks(lnaIndex, pgc1, pgc2, pgc3));
}

TEST_F(DutWithConnectionMockTest, setSpacelessTxEnabledShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto setRateRequest = "4d 54 01 23 0e 00 00 00 06 00 00 00 02 00 66 00 02 00 04 02 00 00";
    constexpr auto setRateResponse = "4d 54 01 83 0e 00 00 00 06 00 01 00 02 00 67 00 02 00 04 02 00 00";

    constexpr auto setTransmitPowerLevelAntenna1Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 00 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna1Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 00 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna2Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 01 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna2Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 01 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna3Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 02 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna3Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 02 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna4Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 03 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna4Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 03 00 22 44";

    constexpr auto setSpacelessTxEnabledRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 2c 00 01";
    constexpr auto setSpacelessTxEnabledResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 2d 00 01";

    constexpr auto setSpacelessTxDisabledRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 2c 00 00";
    constexpr auto setSpacelessTxDisabledResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 2d 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setRateRequest, setRateResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna1Request, setTransmitPowerLevelAntenna1Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna2Request, setTransmitPowerLevelAntenna2Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna3Request, setTransmitPowerLevelAntenna3Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna4Request, setTransmitPowerLevelAntenna4Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setSpacelessTxEnabledRequest, setSpacelessTxEnabledResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setSpacelessTxDisabledRequest, setSpacelessTxDisabledResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));

    ASSERT_TRUE(m_dut.setSpacelessTxEnabled(true));

    ASSERT_TRUE(m_dut.setSpacelessTxEnabled(false));
}

TEST_F(DutWithConnectionMockTest, setTransmitPowerControlShouldSucceed)
{
    constexpr auto setEnabledTxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 24 00 01";
    constexpr auto setEnabledTxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 25 00 01";

    constexpr auto setTransmitPowerLevelAntenna1Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 00 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna1Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 00 00 22 44";

    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto setTransmitPowerControlRequest = "4d 54 01 2b 64 00 00 00 35 04 00 00 54 00 00 00 00 00 00 00 00 00 00 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 02 00 ff 00 00 00";
    constexpr auto setTransmitPowerControlResponse = "4d 54 01 8b 64 00 00 00 35 04 00 00 54 00 00 00 00 00 00 00 00 00 00 00 f0 00 f0 00 f0 00 f0 00 f0 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 0f 00 02 00 ff 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledTxAntennaMaskRequest, setEnabledTxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna1Request, setTransmitPowerLevelAntenna1Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerControlRequest, setTransmitPowerControlResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
    }

    dut::AntennaMask antennaMask = 0x01;

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(antennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    bool closedLoop = true;
    uint8_t powerLimit = 0xf;

    ASSERT_TRUE(m_dut.setTransmitPowerControl(closedLoop, powerLimit));
}

TEST_F(DutWithConnectionMockTest, setTransmitPowerLevelShouldSucceed)
{
    constexpr auto setEnabledTxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 24 00 01";
    constexpr auto setEnabledTxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 25 00 01";

    constexpr auto setTransmitPowerLevelAntenna1Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 00 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna1Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 00 00 22 44";

    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto setTransmitPowerLevelAntenna1Request2 = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 00 00 06 0c";
    constexpr auto setTransmitPowerLevelAntenna1Response2 = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 00 00 06 0c";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledTxAntennaMaskRequest, setEnabledTxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna1Request, setTransmitPowerLevelAntenna1Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna1Request2, setTransmitPowerLevelAntenna1Response2)
    }

    dut::AntennaMask antennaMask = 0x01;

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(antennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    uint8_t powerLevel = 6;

    ASSERT_TRUE(m_dut.setTransmitPowerLevel(powerLevel));
}

TEST_F(DutWithConnectionMockTest, setTransmitPowerTableOffsetShouldSucceed)
{
    constexpr auto setTransmitPowerTableOffsetRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 8c 00 01 00 02 00";
    constexpr auto setTransmitPowerTableOffsetResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 8d 00 01 00 02 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerTableOffsetRequest, setTransmitPowerTableOffsetResponse)
    }

    uint8_t antenna = 1;
    dut::Bandwidth bandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    int16_t offset = 2;

    ASSERT_TRUE(m_dut.setTransmitPowerTableOffset(antenna, bandwidth, offset));
}

TEST_F(DutWithConnectionMockTest, setTransmitPowerVectorShouldSucceed)
{
    constexpr auto setTransmitPowerVectorRequest = "4D 54 01 23 8E 00 00 00 86 00 00 00 02 00 04 02 0D 00 58 02 00 00 1E 1C 1A 18 16 14 12 10 0E 0C 0A 08 06 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 01 01 01 01 01 01 01 01 01 01 01 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setTransmitPowerVectorResponse = "4D 54 01 83 8E 00 00 00 86 00 01 00 02 00 05 02 0D 00 58 02 00 00 1E 1C 1A 18 16 14 12 10 0E 0C 0A 08 06 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 01 01 01 01 01 01 01 01 01 01 01 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto getMeasuredVoltagesAntenna1Request = "4D 54 01 23 8C 00 00 00 84 00 00 00 02 00 06 02 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto getMeasuredVoltagesAntenna1Response = "4D 54 01 83 8C 00 00 00 84 00 01 00 02 00 07 02 00 00 00 00 6E 25 E8 23 6A 22 E4 20 DF 1E 3C 1D B5 1B 35 1A B9 17 6D 16 C2 15 B3 14 C2 13 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto getMeasuredVoltagesAntenna2Request = "4D 54 01 23 8C 00 00 00 84 00 00 00 02 00 06 02 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto getMeasuredVoltagesAntenna2Response = "4D 54 01 83 8C 00 00 00 84 00 01 00 02 00 07 02 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto getMeasuredVoltagesAntenna3Request = "4D 54 01 23 8C 00 00 00 84 00 00 00 02 00 06 02 02 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto getMeasuredVoltagesAntenna3Response = "4D 54 01 83 8C 00 00 00 84 00 01 00 02 00 07 02 02 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto getMeasuredVoltagesAntenna4Request = "4D 54 01 23 8C 00 00 00 84 00 00 00 02 00 06 02 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto getMeasuredVoltagesAntenna4Response = "4D 54 01 83 8C 00 00 00 84 00 01 00 02 00 07 02 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto stopTxRequest = "4d 54 01 23 08 00 00 00 00 00 00 00 02 00 02 02";
    constexpr auto stopTxResponse = "4d 54 01 83 08 00 00 00 00 00 01 00 02 00 03 02";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerVectorRequest, setTransmitPowerVectorResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(getMeasuredVoltagesAntenna1Request, getMeasuredVoltagesAntenna1Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getMeasuredVoltagesAntenna2Request, getMeasuredVoltagesAntenna2Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getMeasuredVoltagesAntenna3Request, getMeasuredVoltagesAntenna3Response)
        SET_REQUEST_RESPONSE_EXPECTATION(getMeasuredVoltagesAntenna4Request, getMeasuredVoltagesAntenna4Response)
        SET_REQUEST_RESPONSE_EXPECTATION(stopTxRequest, stopTxResponse)
    }

    dut::TransmitPowerVector_t transmitPowerVector {
        13,
        600,
        0,
        { 0x1e, 0x1c, 0x1a, 0x18, 0x16, 0x14, 0x12, 0x10, 0x0e, 0x0c, 0x0a, 0x08, 0x06 },
        { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
        {}
    };

    ASSERT_TRUE(m_dut.setTransmitPowerVector(transmitPowerVector));

    const std::vector<uint16_t> expectedVoltages { 9582, 9192, 8810, 8420, 7903, 7484, 7093, 6709, 6073, 5741, 5570, 5299, 5058 };
    for (size_t i = 0; i < expectedVoltages.size(); i++) {
        EXPECT_EQ(transmitPowerVector.measuredVoltagePerPower[0][i], expectedVoltages[i]);
    }
    for (size_t i = expectedVoltages.size(); i < dut::transmitPowerTableSize; i++) {
        EXPECT_EQ(transmitPowerVector.measuredVoltagePerPower[0][i], 0);
    }
    for (uint8_t antenna = 1; antenna < dut::maxNumTxAntennas; antenna++) {
        for (size_t i = 0; i < dut::transmitPowerTableSize; i++) {
            EXPECT_EQ(transmitPowerVector.measuredVoltagePerPower[antenna][i], 0);
        }
    }
}

TEST_F(DutWithConnectionMockTest, setTssiS2dParamsShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto setRateRequest = "4d 54 01 23 0e 00 00 00 06 00 00 00 02 00 66 00 02 00 04 02 00 00";
    constexpr auto setRateResponse = "4d 54 01 83 0e 00 00 00 06 00 01 00 02 00 67 00 02 00 04 02 00 00";

    constexpr auto setTransmitPowerLevelAntenna1Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 00 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna1Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 00 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna2Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 01 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna2Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 01 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna3Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 02 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna3Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 02 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna4Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 03 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna4Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 03 00 22 44";

    constexpr auto setTssiS2dParamsRequest = "4d 54 01 23 0e 00 00 00 06 00 00 00 02 00 6c 00 05 07 03 01 00 04";
    constexpr auto setTssiS2dParamsResponse = "4d 54 01 83 0e 00 00 00 06 00 01 00 02 00 6d 00 05 07 03 01 00 04";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setRateRequest, setRateResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna1Request, setTransmitPowerLevelAntenna1Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna2Request, setTransmitPowerLevelAntenna2Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna3Request, setTransmitPowerLevelAntenna3Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna4Request, setTransmitPowerLevelAntenna4Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTssiS2dParamsRequest, setTssiS2dParamsResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));

    uint8_t antenna = 1;
    uint8_t region = 3;
    uint8_t powerThreshold = 4;
    uint8_t gain = 5;
    uint8_t offset = 7;

    ASSERT_TRUE(m_dut.setTssiS2dParams(antenna, region, powerThreshold, gain, offset));
}

TEST_F(DutWithConnectionMockTest, setXtalCalValueShouldSucceed)
{
    uint16_t xtalValue = 0xab;

    ASSERT_TRUE(m_dut.setXtalCalValue(xtalValue));

    ASSERT_TRUE(m_dut.getXtalCalValue(xtalValue));
    EXPECT_EQ(xtalValue, 0xab);
}

TEST_F(DutWithConnectionMockTest, setXtalRegValueShouldSucceed)
{
    constexpr auto setXtalValueRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 44 00 3c 00 00 00";
    constexpr auto setXtalValueResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 45 00 3c 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setXtalValueRequest, setXtalValueResponse)
    }

    uint16_t xtalValue = 0x3c;

    ASSERT_TRUE(m_dut.setXtalRegValue(xtalValue));
}

TEST_F(DutWithConnectionMockTest, startCalibrationShouldSucceed)
{
    constexpr auto startCalibrationRequest = "4d 54 01 23 14 00 00 00 0c 00 00 00 02 00 16 00 80 27 00 00 00 00 00 00 01 00 00 00";
    constexpr auto startCalibrationResponse = "4d 54 01 83 14 00 00 00 0c 00 01 00 02 00 17 00 80 27 00 00 00 00 00 00 00 00 aa 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(startCalibrationRequest, startCalibrationResponse);
    }

    dut::StartCalibrationParams_t params {};
    uint8_t status {};

    params.mask = 0x2780;
    params.maskType = 1;
    params.type = 0;

    ASSERT_TRUE(m_dut.startCalibration(params, status));

    ASSERT_EQ(status, 0xaa);
}

TEST_F(DutWithConnectionMockTest, startCwShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto startCwRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 2a 00 01 0a 04 00";
    constexpr auto startCwResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 2b 00 01 0a 04 00";

    constexpr auto stopCwRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 2a 00 00 00 00 00";
    constexpr auto stopCwResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 2b 00 00 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(startCwRequest, startCwResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(stopCwRequest, stopCwResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    int8_t amplitude = 10;
    int16_t tone = 4;

    ASSERT_TRUE(m_dut.startCw(amplitude, tone));

    ASSERT_TRUE(m_dut.stopCw());
}

TEST_F(DutWithConnectionMockTest, startRxCalibrationShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto setRiscModeDisabledRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 3a 00 00 00 00 00";
    constexpr auto setRiscModeDisabledResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 3b 00 00 00 00 00";

    constexpr auto startRxCalibrationRequest = "4d 54 01 23 08 00 00 00 00 00 00 00 02 00 80 00";
    constexpr auto startRxCalibrationResponse = "4d 54 01 83 08 00 00 00 00 00 01 00 02 00 81 00";

    constexpr auto stopRxCalibrationRequest = "4d 54 01 23 08 00 00 00 00 00 00 00 02 00 82 00";
    constexpr auto stopRxCalibrationResponse = "4d 54 01 83 08 00 00 00 00 00 01 00 02 00 83 00";

    constexpr auto setRiscModeEnabledRequest = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 3a 00 01 00 00 00";
    constexpr auto setRiscModeEnabledResponse = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 3b 00 01 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setRiscModeDisabledRequest, setRiscModeDisabledResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(startRxCalibrationRequest, startRxCalibrationResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(stopRxCalibrationRequest, stopRxCalibrationResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setRiscModeEnabledRequest, setRiscModeEnabledResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    ASSERT_TRUE(m_dut.startRxCalibration());

    ASSERT_TRUE(m_dut.stopRxCalibration());
}

TEST_F(DutWithConnectionMockTest, startTxShouldFailIfRateNotSet)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    uint16_t repetitions = UINT16_MAX;
    uint32_t packetLength = 4000;
    bool longData = false;
    bool beamforming = false;

    EXPECT_FALSE(m_dut.startTx(repetitions, packetLength, longData, beamforming));
    EXPECT_EQ(m_dut.getLastError(), "Transmission rate not set");
}

TEST_F(DutWithConnectionMockTest, startTxShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto setRateRequest = "4d 54 01 23 0e 00 00 00 06 00 00 00 02 00 66 00 02 00 04 02 00 00";
    constexpr auto setRateResponse = "4d 54 01 83 0e 00 00 00 06 00 01 00 02 00 67 00 02 00 04 02 00 00";

    constexpr auto setTransmitPowerLevelAntenna1Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 00 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna1Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 00 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna2Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 01 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna2Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 01 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna3Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 02 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna3Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 02 00 22 44";

    constexpr auto setTransmitPowerLevelAntenna4Request = "4d 54 01 23 0c 00 00 00 04 00 00 00 02 00 32 00 03 00 22 44";
    constexpr auto setTransmitPowerLevelAntenna4Response = "4d 54 01 83 0c 00 00 00 04 00 01 00 02 00 33 00 03 00 22 44";

    constexpr auto startTxRequest = "4d 54 01 23 14 00 00 00 0c 00 00 00 02 00 00 02 a0 0f 00 00 ff ff 01 00 00 01 00 00";
    constexpr auto startTxResponse = "4d 54 01 83 14 00 00 00 0c 00 01 00 02 00 01 02 a0 0f 00 00 ff ff 01 00 00 01 00 00";

    constexpr auto stopTxRequest = "4d 54 01 23 08 00 00 00 00 00 00 00 02 00 02 02";
    constexpr auto stopTxResponse = "4d 54 01 83 08 00 00 00 00 00 01 00 02 00 03 02";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setRateRequest, setRateResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna1Request, setTransmitPowerLevelAntenna1Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna2Request, setTransmitPowerLevelAntenna2Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna3Request, setTransmitPowerLevelAntenna3Response)
        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerLevelAntenna4Request, setTransmitPowerLevelAntenna4Response)
        SET_REQUEST_RESPONSE_EXPECTATION(startTxRequest, startTxResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(stopTxRequest, stopTxResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));

    uint16_t repetitions = UINT16_MAX;
    uint32_t packetLength = 4000;
    bool longData = false;
    bool beamforming = false;

    ASSERT_TRUE(m_dut.startTx(repetitions, packetLength, longData, beamforming));

    ASSERT_TRUE(m_dut.stopTx());
}

TEST_F(DutWithConnectionMockTest, startStopRxPerShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setEnabledRxAntennaMaskRequest = "4d 54 01 23 09 00 00 00 01 00 00 00 02 00 26 00 0f";
    constexpr auto setEnabledRxAntennaMaskResponse = "4d 54 01 83 09 00 00 00 01 00 01 00 02 00 27 00 0f";

    constexpr auto startRxPerRequest = "4d 54 01 23 10 00 00 00 08 00 00 00 02 00 a4 00 20 4e 00 00 04 00 00 00";
    constexpr auto startRxPerResponse = "4d 54 01 83 10 00 00 00 08 00 01 00 02 00 a5 00 20 4e 00 00 04 00 00 00";

    constexpr auto stopRxPerRequest = "4d 54 01 23 14 00 00 00 0c 00 00 00 02 00 a6 00 00 00 00 00 00 00 00 00 04 01 00 00";
    constexpr auto stopRxPerResponse = "4d 54 01 83 14 00 00 00 0c 00 01 00 02 00 a7 00 92 00 00 00 8f 4d 00 00 04 01 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setEnabledRxAntennaMaskRequest, setEnabledRxAntennaMaskResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(startRxPerRequest, startRxPerResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(stopRxPerRequest, stopRxPerResponse)
    }

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    uint32_t packetLimit = 20000;
    bool calcPer = true;

    ASSERT_TRUE(m_dut.startRxPer(packetLimit));

    ASSERT_TRUE(m_dut.stopRxPer(calcPer));
}

TEST_F(DutWithConnectionMockTest, writeCalibrationFileShouldSucceed)
{
    constexpr auto writeNvmRequest = "4d 54 01 25 60 00 00 00 58 00 00 00 02 00 04 00 02 00 00 00 00 00 00 00 00 04 00 00 44 00 00 00 00 00 00 00 fc 1b 00 00 00 00 00 00 30 1a 80 09 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff ff ff ff ff ff ff ff ff ff ff 06 60 10 00 61 56 41 00 43 ac 9a 96 f1 bb 20 db 07 00 29 0f 64 02 00 3c 00 ff 00 00";
    constexpr auto writeNvmResponse = "4d 54 01 85 60 00 00 00 58 00 01 00 02 00 05 00 02 00 00 00 00 00 00 00 00 08 00 00 44 00 00 00 00 00 00 00 fc 1b 00 00 00 00 00 00 30 1a 80 09 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff ff ff ff ff ff ff ff ff ff ff 06 60 10 00 61 56 41 00 43 ac 9a 96 f1 bb 20 db 07 00 29 0f 64 02 00 3c 00 ff 00 00";

    constexpr auto readNvmRequest = "4d 54 01 25 60 00 00 00 58 00 00 00 02 00 02 00 02 00 00 00 00 00 00 00 00 04 00 00 44 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto readNvmResponse = "4d 54 01 85 60 00 00 00 58 00 01 00 02 00 03 00 02 00 00 00 00 00 00 00 00 08 00 00 44 00 00 00 00 00 00 00 fc 1b 00 00 00 00 00 00 30 1a 80 09 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff ff ff ff ff ff ff ff ff ff ff 06 60 10 00 61 56 41 00 43 ac 9a 96 f1 bb 20 db 07 00 29 0f 64 02 00 3c 00 ff 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(writeNvmRequest, writeNvmResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(readNvmRequest, readNvmResponse)
    }

    ASSERT_TRUE(m_dut.writeCalibrationFile(dut::NvMemoryType::MEMORY_TYPE_FLASH, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K));
}

TEST_F(DutWithConnectionMockTest, writeMemoryShouldSucceed)
{
    constexpr auto writeMemoryRequest = "4d 54 01 23 1a 00 00 00 12 00 00 00 02 00 10 00 04 00 0a 00 01 00 00 00 50 ff 50 f3 50 f3 50 ff 00 00";
    constexpr auto writeMemoryResponse = "4d 54 01 83 1a 00 00 00 12 00 01 00 02 00 11 00 04 00 0a 00 01 00 00 00 50 ff 50 f3 50 f3 50 ff 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(writeMemoryRequest, writeMemoryResponse)
    }

    dut::ChipModule chipModule = dut::ChipModule::CHIP_MODULE_RF;
    size_t address = 0x1;
    const uint8_t data[] { 0x50, 0xff, 0x50, 0xf3, 0x50, 0xf3, 0x50, 0xff, 0x00, 0x00 };
    size_t length = sizeof(data);

    ASSERT_TRUE(m_dut.writeMemory(chipModule, address, data, length));
}

TEST_F(DutWithConnectionMockTest, writeNvmShouldSucceed)
{
    constexpr auto writeNvmRequest = "4d 54 01 25 45 00 00 00 3d 00 00 00 02 00 04 00 02 00 00 00 00 00 00 00 00 04 00 00 29 00 00 00 00 00 00 00 fc 1b 30 00 00 00 00 00 30 1a 80 09 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff ff ff ff ff ff ff ff ff ff ff 06";
    constexpr auto writeNvmResponse = "4d 54 01 85 45 00 00 00 3d 00 01 00 02 00 05 00 02 00 00 00 00 00 00 00 00 08 00 00 29 00 00 00 00 00 00 00 fc 1b 30 00 00 00 00 00 30 1a 80 09 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff ff ff ff ff ff ff ff ff ff ff 06";

    constexpr auto readNvmRequest = "4d 54 01 25 45 00 00 00 3d 00 00 00 02 00 02 00 02 00 00 00 00 00 00 00 00 04 00 00 29 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto readNvmResponse = "4d 54 01 85 45 00 00 00 3d 00 01 00 02 00 03 00 02 00 00 00 00 00 00 00 00 08 00 00 29 00 00 00 00 00 00 00 fc 1b 30 00 00 00 00 00 30 1a 80 09 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff ff ff ff ff ff ff ff ff ff ff 06";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(writeNvmRequest, writeNvmResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(readNvmRequest, readNvmResponse)
    }

    size_t address = 0;
    const uint8_t data[] { 0xfc, 0x1b, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x1a, 0x80, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x06 };
    size_t length = sizeof(data);

    ASSERT_TRUE(m_dut.writeNvm(address, data, length));
}

TEST_F(DutWithConnectionMockTest, writeRegisterShouldSucceed)
{
    constexpr auto readRegisterRequest = "4d 54 01 23 14 00 00 00 0c 00 00 00 02 00 0e 00 03 00 04 00 9c 01 b0 a0 00 00 00 00";
    constexpr auto readRegisterResponse = "4d 54 01 83 14 00 00 00 0c 00 01 00 02 00 0f 00 03 00 04 00 9c 01 b0 a0 01 00 00 00";

    constexpr auto writeRegisterRequest = "4d 54 01 23 14 00 00 00 0c 00 00 00 02 00 10 00 03 00 04 00 9c 01 b0 a0 01 00 00 00";
    constexpr auto writeRegisterResponse = "4d 54 01 83 14 00 00 00 0c 00 01 00 02 00 11 00 03 00 04 00 9c 01 b0 a0 01 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(readRegisterRequest, readRegisterResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(writeRegisterRequest, writeRegisterResponse)
    }

    dut::ChipModule chipModule = dut::ChipModule::CHIP_MODULE_PHY;
    size_t address = 0xa0b0019c;
    uint32_t mask = 0x1;
    uint32_t value = 0x1;

    ASSERT_TRUE(m_dut.writeRegister(chipModule, address, mask, value));
}
}
