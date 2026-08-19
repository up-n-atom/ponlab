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

#include "dut/ClientImpl.h"

#include "SharedHeaders.h"
#include "dut/AntennaMask.h"
#include "dut/ConnectionMock.h"
#include "dut/Logger.h"
#include "dut/Tools.h"

#include <gtest/gtest.h>

using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::ThrowsMessage;

namespace {

constexpr uint8_t wlanIndex = 0x02;
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

class ClientImplTest : public ::testing::Test {
public:
    ClientImplTest()
        : m_client(wlanIndex, m_connection, m_logger)
    {
    }

    std::shared_ptr<StrictMock<dut::ConnectionMock>> m_connection = std::make_shared<StrictMock<dut::ConnectionMock>>();
    std::shared_ptr<dut::Logger> m_logger = std::make_shared<dut::NullLogger>();
    dut::ClientImpl m_client;

    std::array<uint8_t, bufferSize> m_buffer {};
    size_t m_length = 0;
};

TEST_F(ClientImplTest, exchangeMessageShouldFailIfResponseLengthIsTooSmall)
{
    constexpr auto getFwInfoRequest = "4d 54 01 23 18 00 00 00 10 00 00 00 02 00 68 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto getFwInfoResponse = "4d 54 01 83 17 00 00 00 10 00 01 00 02 00 69 00 1f 01 00 00 00 00 00 0f 0f 01 00 00 00 00 0f";
    // Invalid length (should be 0x18) -------------^
    // We expect a response message with the same length as the request.
    // In this test, the connection mock returns a response message with one byte less than what is expected.

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getFwInfoRequest, getFwInfoResponse)
    }

    dutGetFwInfo_t fwInfo {};

    const char* expectedMessage = "Invalid response received: response length (23) is too small (expected length is 24)";
    EXPECT_THAT([&]() { m_client.getFwInfo(fwInfo); }, ThrowsMessage<std::runtime_error>(expectedMessage));
}

TEST_F(ClientImplTest, exchangeMessageShouldFailIfInvalidResponseIdIsReceived)
{
    constexpr auto getFwInfoRequest = "4d 54 01 23 18 00 00 00 10 00 00 00 02 00 68 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto getFwInfoResponse = "4d 54 01 83 18 00 00 00 10 00 01 00 02 00 68 00 1f 01 00 00 00 00 00 0f 0f 01 00 00 00 00 0f 1f";
    // Invalid response ID (should be 0x69) --------------------------------------^
    // In this test, the connection mock returns a response message with an unexpected response ID.

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getFwInfoRequest, getFwInfoResponse)
    }

    dutGetFwInfo_t fwInfo {};

    const char* expectedMessage = "Invalid response received for message ID 0x03 and request ID 0x00000068: expected response ID is 0x00000069, received response ID is 0x00000068";
    EXPECT_THAT([&]() { m_client.getFwInfo(fwInfo); }, ThrowsMessage<std::runtime_error>(expectedMessage));
}

TEST_F(ClientImplTest, addVapShouldSucceed)
{
    constexpr auto addVapRequest = "4d 54 01 2b 60 00 00 00 10 04 00 00 50 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto addVapResponse = "4d 54 01 8b 60 00 00 00 10 04 00 00 50 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setBssRequest = "4d 54 01 2b 68 00 00 00 12 04 00 00 58 00 00 00 00 00 00 00 00 00 00 00 00 00 01 02 04 0b 16 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 04 ff ff 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setBssResponse = "4d 54 01 8b 68 00 00 00 12 04 00 00 58 00 00 00 00 00 00 00 00 00 00 00 00 00 01 02 04 0b 16 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 04 ff ff 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    constexpr auto setWmmParametersRequest = "4d 54 01 2b 34 00 00 00 05 04 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01 00 00 00 00 00";
    constexpr auto setWmmParametersResponse = "4d 54 01 8b 34 00 00 00 05 04 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01 00 00 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(addVapRequest, addVapResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setBssRequest, setBssResponse)
        SET_REQUEST_RESPONSE_EXPECTATION(setWmmParametersRequest, setWmmParametersResponse)
    }

    uint8_t vapIndex = 0;
    bool snifferMode = false;

    EXPECT_NO_THROW(m_client.addVap(vapIndex, snifferMode));
}

TEST_F(ClientImplTest, getFwInfoShouldSucceed)
{
    constexpr auto getFwInfoRequest = "4d 54 01 23 18 00 00 00 10 00 00 00 02 00 68 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto getFwInfoResponse = "4d 54 01 83 18 00 00 00 10 00 01 00 02 00 69 00 1f 01 00 00 00 00 00 0f 0f 01 00 00 00 00 0f 1f";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getFwInfoRequest, getFwInfoResponse)
    }

    dutGetFwInfo_t expectedFwInfo {};
    expectedFwInfo.psdMaxTxAntMask = 0x0f;
    expectedFwInfo.psdMaxRxAntMask = 0x1f;

    dutGetFwInfo_t fwInfo {};

    EXPECT_NO_THROW(m_client.getFwInfo(fwInfo));
    EXPECT_EQ(fwInfo.psdMaxTxAntMask, expectedFwInfo.psdMaxTxAntMask);
    EXPECT_EQ(fwInfo.psdMaxRxAntMask, expectedFwInfo.psdMaxRxAntMask);
}

TEST_F(ClientImplTest, getTemperatureShouldSucceed)
{
    constexpr auto getTemperatureRequest = "4d 54 01 23 10 00 00 00 08 00 00 00 02 00 48 00 00 00 00 00 00 00 00 00";
    constexpr auto getTemperatureResponse = "4d 54 01 83 10 00 00 00 08 00 01 00 02 00 49 00 0c 00 00 00 22 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(getTemperatureRequest, getTemperatureResponse)
    }

    float temperature {};

    EXPECT_NO_THROW(m_client.getTemperature(temperature));
    EXPECT_EQ(temperature, 12.34f);
}

TEST_F(ClientImplTest, readChipIdShouldSucceed)
{
    constexpr auto readChipVersionRequest = "4d 54 01 23 10 00 00 00 08 00 00 00 02 00 20 00 00 00 00 00 00 00 00 00";
    constexpr auto readChipVersionResponse = "4d 54 01 83 10 00 00 00 08 00 01 00 02 00 21 00 10 09 00 00 53 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(readChipVersionRequest, readChipVersionResponse)
    }

    dut::ChipID expectedChipId = dut::ChipID::CHIP_ID_GEN6_B;

    dut::ChipID chipId {};

    EXPECT_NO_THROW(m_client.readChipId(chipId));
    EXPECT_EQ(chipId, expectedChipId);
}

TEST_F(ClientImplTest, readMemoryShouldFailWithNullData)
{
    const char* expectedMessage = "Parameter 'data' cannot be a null pointer";
    uint8_t data[1] {};
    EXPECT_THAT([&]() { m_client.readMemory(dut::ChipModule::CHIP_MODULE_RF, 0, nullptr, sizeof(data)); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, readMemoryShouldFailWithZeroLength)
{
    const char* expectedMessage = "Parameter 'length' cannot be 0";
    uint8_t data[1] {};
    EXPECT_THAT([&]() { m_client.readMemory(dut::ChipModule::CHIP_MODULE_RF, 0, data, 0); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, readMemoryShouldFailWithInvalidLength)
{
    const char* expectedMessage = "The specified length (257) is too large (256 bytes allowed)";
    uint8_t data[dut::maxMemoryAccessLength + 1] {};
    EXPECT_THAT([&]() { m_client.readMemory(dut::ChipModule::CHIP_MODULE_RF, 0, data, sizeof(data)); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, readNvMemoryShouldFailWithNullData)
{
    const char* expectedMessage = "Parameter 'data' cannot be a null pointer";
    uint8_t data[1] {};
    EXPECT_THAT([&]() { m_client.readNvMemory(0, nullptr, sizeof(data), dut::NvMemoryType::MEMORY_TYPE_FLASH, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::FileType::FILE_TYPE_CALIBRATION); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, readNvMemoryShouldFailWithZeroLength)
{
    const char* expectedMessage = "Parameter 'length' cannot be 0";
    uint8_t data[1] {};
    EXPECT_THAT([&]() { m_client.readNvMemory(0, data, 0, dut::NvMemoryType::MEMORY_TYPE_FLASH, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::FileType::FILE_TYPE_CALIBRATION); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, readNvMemoryShouldFailWithInvalidLength)
{
    const char* expectedMessage = "The specified length (485) is too large (484 bytes allowed)";
    uint8_t data[dut::maxNvMemoryAccessLength + 1] {};
    EXPECT_THAT([&]() { m_client.readNvMemory(0, data, sizeof(data), dut::NvMemoryType::MEMORY_TYPE_FLASH, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::FileType::FILE_TYPE_CALIBRATION); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, setBandShouldSucceed)
{
    constexpr auto setBandRequest = "4d 54 01 25 0c 00 00 00 04 00 00 00 02 00 0a 00 00 00 00 00";
    constexpr auto setBandResponse = "4d 54 01 85 0c 00 00 00 04 00 01 00 02 00 0b 00 00 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setBandRequest, setBandResponse)
    }

    dut::Band band = dut::Band::BAND_2400MHZ;

    EXPECT_NO_THROW(m_client.setBand(band));
}

TEST_F(ClientImplTest, setHdkConfigShouldFailWithInvalidBand)
{
    const char* expectedMessage = "Invalid value for parameter 'band' (INVALID)";
    EXPECT_THAT([&]() { m_client.setHdkConfig(0x0, 0x00, dut::Band::BAND_INVALID); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, setRssiCalDataShouldSucceed)
{
    constexpr auto setRssiCalDataRequest = "4d 54 01 2b e8 00 00 00 41 04 00 00 d8 00 00 00 00 00 00 00 00 00 00 00 05 0f 00 01 02 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setRssiCalDataResponse = "4d 54 01 8b e8 00 00 00 00 00 04 41 00 00 00 d8 00 00 00 00 00 00 00 00 05 0f 00 01 02 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setRssiCalDataRequest, setRssiCalDataResponse)
    }

    uint8_t data[] { 0x0f, 0x00, 0x01, 0x02, 0x03 };

    EXPECT_NO_THROW(m_client.setRssiCalData(data, sizeof(data)));
}

TEST_F(ClientImplTest, setRssiCalDataShouldFailWithNullData)
{
    const char* expectedMessage = "Parameter 'data' cannot be a null pointer";
    uint8_t data[1] {};

    EXPECT_THAT([&]() { m_client.setRssiCalData(nullptr, sizeof(data)); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, setRssiCalDataShouldFailWithWithZeroLength)
{
    const char* expectedMessage = "Parameter 'length' cannot be 0";
    uint8_t data[1] {};

    EXPECT_THAT([&]() { m_client.setRssiCalData(data, 0); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, setRssiCalDataShouldFailWithWithInvalidLength)
{
    const char* expectedMessage = "The specified data length (216) is too large (215 bytes allowed)";
    uint8_t data[dut::maxRssiCalDataLength + 1] {};

    EXPECT_THAT([&]() { m_client.setRssiCalData(data, sizeof(data)); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, setTransmitPowerControlAntennaParamsShouldFailWithNullData)
{
    const char* expectedMessage = "Parameter 'data' cannot be a null pointer";
    uint8_t data[1] {};
    EXPECT_THAT([&]() { m_client.setTransmitPowerControlAntennaParams(0, 0, nullptr, sizeof(data)); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, setTransmitPowerControlAntennaParamsShouldFailWithZeroLength)
{
    const char* expectedMessage = "Parameter 'length' cannot be 0";
    uint8_t data[1] {};
    EXPECT_THAT([&]() { m_client.setTransmitPowerControlAntennaParams(0, 0, data, 0); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, setTransmitPowerControlAntennaParamsShouldSucceed)
{
    constexpr auto setTransmitPowerControlAntennaParamsRequest = "4d 54 01 2b 19 00 00 00 34 04 00 00 09 00 00 00 00 00 00 00 00 00 00 00 01 09 00 00 00 01 02 03 04";
    constexpr auto setTransmitPowerControlAntennaParamsResponse = "4d 54 01 8b 19 00 00 00 00 00 04 34 00 00 00 09 00 00 00 00 00 00 00 00 01 09 00 00 00 01 02 03 04";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setTransmitPowerControlAntennaParamsRequest, setTransmitPowerControlAntennaParamsResponse)
    }

    uint8_t antenna = 1;
    uint8_t tpcFreqLen = 9;
    uint8_t data[] { 0x00, 0x01, 0x02, 0x03, 0x04 };
    size_t length = sizeof(data);

    EXPECT_NO_THROW(m_client.setTransmitPowerControlAntennaParams(antenna, tpcFreqLen, data, length));
}

TEST_F(ClientImplTest, setZwdfsEnabledShouldSucceed)
{
    constexpr auto setChannelRequest = "4d 54 01 2b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 78 00 03 00 00 00 07 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    constexpr auto setChannelResponse = "4d 54 01 8b ec 00 00 00 0a 04 00 00 dc 00 00 00 00 00 00 00 00 00 00 00 78 00 03 00 00 00 07 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    {
        InSequence sequence;

        SET_REQUEST_RESPONSE_EXPECTATION(setChannelRequest, setChannelResponse)
    }

    bool zwdfsEnabled = true;
    uint8_t zwdfsChannel = 120;
    dut::Bandwidth zwdfsBandwidth = dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY;
    dut::Bandwidth radarDetectionBandwidth = dut::Bandwidth::BANDWIDTH_INVALID;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    EXPECT_NO_THROW(m_client.setZwdfsEnabled(zwdfsEnabled, zwdfsChannel, zwdfsBandwidth, radarDetectionBandwidth, regulationType));
}

TEST_F(ClientImplTest, writeMemoryShouldFailWithNullData)
{
    const char* expectedMessage = "Parameter 'data' cannot be a null pointer";
    uint8_t data[1] {};
    EXPECT_THAT([&]() { m_client.writeMemory(dut::ChipModule::CHIP_MODULE_RF, 0, nullptr, sizeof(data)); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, writeMemoryShouldFailWithZeroLength)
{
    const char* expectedMessage = "Parameter 'length' cannot be 0";
    uint8_t data[1] {};
    EXPECT_THAT([&]() { m_client.writeMemory(dut::ChipModule::CHIP_MODULE_RF, 0, data, 0); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, writeMemoryShouldFailWithInvalidLength)
{
    const char* expectedMessage = "The specified length (257) is too large (256 bytes allowed)";
    uint8_t data[dut::maxMemoryAccessLength + 1] {};
    EXPECT_THAT([&]() { m_client.writeMemory(dut::ChipModule::CHIP_MODULE_RF, 0, data, sizeof(data)); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, writeNvMemoryShouldFailWithNullData)
{
    const char* expectedMessage = "Parameter 'data' cannot be a null pointer";
    uint8_t data[1] {};
    EXPECT_THAT([&]() { m_client.writeNvMemory(0, nullptr, sizeof(data), dut::NvMemoryType::MEMORY_TYPE_FLASH, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::FileType::FILE_TYPE_CALIBRATION); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, writeNvMemoryShouldFailWithZeroLength)
{
    const char* expectedMessage = "Parameter 'length' cannot be 0";
    uint8_t data[1] {};
    EXPECT_THAT([&]() { m_client.writeNvMemory(0, data, 0, dut::NvMemoryType::MEMORY_TYPE_FLASH, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::FileType::FILE_TYPE_CALIBRATION); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(ClientImplTest, writeNvMemoryShouldFailWithInvalidLength)
{
    const char* expectedMessage = "The specified length (485) is too large (484 bytes allowed)";
    uint8_t data[dut::maxNvMemoryAccessLength + 1] {};
    EXPECT_THAT([&]() { m_client.writeNvMemory(0, data, sizeof(data), dut::NvMemoryType::MEMORY_TYPE_FLASH, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::FileType::FILE_TYPE_CALIBRATION); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

}