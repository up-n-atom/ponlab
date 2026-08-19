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

#include "DeviceNvm.h"

#include "CmdLineArgs.h"
#include "EmbeddedResource.h"
#include "dut/ClientMock.h"
#include "dut/WindowsConsoleLogger.h"
#include "resource.h"

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::ThrowsMessage;

namespace {

constexpr dut::NvMemoryType memoryType = dut::NvMemoryType::MEMORY_TYPE_FLASH;

class DeviceNvmTest : public ::testing::Test {
public:
    DeviceNvmTest()
        : m_resource(IDR_CALIBRATIONFILE1, CALIBRATIONFILE)
    {
    }

    std::shared_ptr<StrictMock<dut::ClientMock>> m_client = std::make_shared<StrictMock<dut::ClientMock>>();
    std::shared_ptr<dut::Logger> m_logger = std::make_shared<dut::WindowsConsoleLogger>(true, g_cmdLineArgs.getLogLevel());

    EmbeddedResource m_resource;
};

TEST_F(DeviceNvmTest, getTypeShouldSucceed)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    ASSERT_EQ(nvm.getType(), dut::NvMemoryType::MEMORY_TYPE_FLASH);
}

TEST_F(DeviceNvmTest, setTypeShouldSucceed)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    ASSERT_TRUE(nvm.setType(dut::NvMemoryType::MEMORY_TYPE_EEPROM));
    ASSERT_EQ(nvm.getType(), dut::NvMemoryType::MEMORY_TYPE_EEPROM);
}

TEST_F(DeviceNvmTest, setTypeShouldFailWithInvalidType)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    ASSERT_FALSE(nvm.setType(dut::NvMemoryType::MEMORY_TYPE_EFUSE));
}

TEST_F(DeviceNvmTest, getFlashMemorySizeShouldSucceedWith3KFlash)
{
    dut::DeviceNvm nvm(m_client, m_logger);
    nvm.setSize(dut::NvMemorySize::MEMORY_SIZE_BYTES_3K);

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, readNvMemory(_, _, _, memoryType, dut::NvMemorySize::MEMORY_SIZE_BYTES_3K, dut::FileType::FILE_TYPE_CALIBRATION)).WillRepeatedly(Invoke([&](size_t address, uint8_t* data, size_t length, dut::NvMemoryType /*memoryType*/, dut::NvMemorySize /*memorySize*/, dut::FileType /*fileType*/) {
            if ((address == static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_3K) - 1) && (length == 1)) {
                data[length - 1] = 0xff;
            } else {
                memcpy(data, &m_resource.getData()[address], length);
            }
        }));
    }

    nvm.load();
    ASSERT_EQ(nvm.getSize(), static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_3K));
}

TEST_F(DeviceNvmTest, getFlashMemorySizeShouldSucceedWith2KFlash)
{
    dut::DeviceNvm nvm(m_client, m_logger);
    nvm.setSize(dut::NvMemorySize::MEMORY_SIZE_BYTES_3K);

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, readNvMemory(static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_3K) - 1, _, 1, memoryType, dut::NvMemorySize::MEMORY_SIZE_BYTES_3K, dut::FileType::FILE_TYPE_CALIBRATION)).WillRepeatedly(Invoke([](size_t /*address*/, uint8_t* /*data*/, size_t /*length*/, dut::NvMemoryType /*memoryType*/, dut::NvMemorySize /*memorySize*/, dut::FileType /*fileType*/) {
            throw std::runtime_error("Unable to read 3K");
        }));
        EXPECT_CALL(*m_client, readNvMemory(_, _, _, memoryType, dut::NvMemorySize::MEMORY_SIZE_BYTES_2K, dut::FileType::FILE_TYPE_CALIBRATION)).WillRepeatedly(Invoke([&](size_t address, uint8_t* data, size_t length, dut::NvMemoryType /*memoryType*/, dut::NvMemorySize /*memorySize*/, dut::FileType /*fileType*/) {
            if ((address == static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_2K) - 1) && (length == 1)) {
                data[length - 1] = 0xff;
            } else {
                memcpy(data, &m_resource.getData()[address], length);
            }
        }));
    }

    nvm.load();
    ASSERT_EQ(nvm.getSize(), static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_2K));
}

TEST_F(DeviceNvmTest, getFlashMemorySizeShouldSucceedWith1KFlash)
{
    dut::DeviceNvm nvm(m_client, m_logger);
    nvm.setSize(dut::NvMemorySize::MEMORY_SIZE_BYTES_3K);

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, readNvMemory(static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_3K) - 1, _, 1, memoryType, dut::NvMemorySize::MEMORY_SIZE_BYTES_3K, dut::FileType::FILE_TYPE_CALIBRATION)).WillRepeatedly(Invoke([](size_t /*address*/, uint8_t* /*data*/, size_t /*length*/, dut::NvMemoryType /*memoryType*/, dut::NvMemorySize /*memorySize*/, dut::FileType /*fileType*/) {
            throw std::runtime_error("Unable to read 3K");
        }));
        EXPECT_CALL(*m_client, readNvMemory(static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_2K) - 1, _, 1, memoryType, dut::NvMemorySize::MEMORY_SIZE_BYTES_2K, dut::FileType::FILE_TYPE_CALIBRATION)).WillRepeatedly(Invoke([](size_t /*address*/, uint8_t* /*data*/, size_t /*length*/, dut::NvMemoryType /*memoryType*/, dut::NvMemorySize /*memorySize*/, dut::FileType /*fileType*/) {
            throw std::runtime_error("Unable to read 2K");
        }));
        EXPECT_CALL(*m_client, readNvMemory(_, _, _, memoryType, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::FileType::FILE_TYPE_CALIBRATION)).WillRepeatedly(Invoke([&](size_t address, uint8_t* data, size_t length, dut::NvMemoryType /*memoryType*/, dut::NvMemorySize /*memorySize*/, dut::FileType /*fileType*/) {
            if ((address == static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_1K) - 1) && (length == 1)) {
                data[length - 1] = 0xff;
            } else {
                memcpy(data, &m_resource.getData()[address], length);
            }
        }));
    }

    nvm.load();
    ASSERT_EQ(nvm.getSize(), static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_1K));
}

TEST_F(DeviceNvmTest, setMemorySizeShouldSucceedWithValidSizes)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    std::vector<dut::NvMemorySize> sizes { dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::NvMemorySize::MEMORY_SIZE_BYTES_2K, dut::NvMemorySize::MEMORY_SIZE_BYTES_3K };
    for (auto size : sizes) {
        ASSERT_TRUE(nvm.setSize(size));
        ASSERT_EQ(nvm.getSize(), static_cast<size_t>(size));
    }
}

TEST_F(DeviceNvmTest, setMemorySizeShouldFailWithInvalidSize)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    size_t size = static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_1K) + 1;

    ASSERT_FALSE(nvm.setSize(static_cast<dut::NvMemorySize>(size)));
}

TEST_F(DeviceNvmTest, loadFromFileShouldFailIfFileNotFound)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    const std::string fileName = "file-not-found";

    const std::string expectedMessage = "Unable to get file size for file '" + fileName + "'";
    EXPECT_THAT([&]() { nvm.loadFromFile(fileName); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(DeviceNvmTest, readShouldFailWithInvalidAddress)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    auto address = static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_1K);
    uint8_t data[1] {};
    size_t length = sizeof(data);

    const char* expectedMessage = "The specified address (1024) is too large (memory size is 1024)";
    EXPECT_THAT([&]() { nvm.read(address, data, length); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(DeviceNvmTest, readShouldFailWithNullData)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    size_t address = 0;
    uint8_t data[1] {};
    size_t length = sizeof(data);

    const char* expectedMessage = "Parameter 'data' cannot be a null pointer";
    EXPECT_THAT([&]() { nvm.read(address, nullptr, length); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(DeviceNvmTest, readShouldFailWithZeroLength)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    size_t address = 0;
    uint8_t data[1] {};
    size_t length = 0;

    const char* expectedMessage = "Parameter 'length' cannot be 0";
    EXPECT_THAT([&]() { nvm.read(address, data, length); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(DeviceNvmTest, readShouldFailWithInvalidLength)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    size_t address = 0;
    uint8_t data[1] {};
    size_t length = static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_1K) + 1;

    const char* expectedMessage = "The specified length (1025) is too large (only 1024 bytes allowed starting at address 0)";
    EXPECT_THAT([&]() { nvm.read(address, data, length); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(DeviceNvmTest, writeShouldFailWithInvalidAddress)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    auto address = static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_1K);
    uint8_t data[1] {};
    size_t length = sizeof(data);

    const char* expectedMessage = "The specified address (1024) is too large (memory size is 1024)";
    EXPECT_THAT([&]() { nvm.write(address, data, length); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(DeviceNvmTest, writeShouldFailWithNullData)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    size_t address = 0;
    uint8_t data[1] {};
    size_t length = sizeof(data);

    const char* expectedMessage = "Parameter 'data' cannot be a null pointer";
    EXPECT_THAT([&]() { nvm.write(address, nullptr, length); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(DeviceNvmTest, writeShouldFailWithZeroLength)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    size_t address = 0;
    uint8_t data[1] {};
    size_t length = 0;

    const char* expectedMessage = "Parameter 'length' cannot be 0";
    EXPECT_THAT([&]() { nvm.write(address, data, length); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(DeviceNvmTest, writeShouldFailWithInvalidLength)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    size_t address = 0;
    uint8_t data[1] {};
    size_t length = static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_1K) + 1;

    const char* expectedMessage = "The specified length (1025) is too large (only 1024 bytes allowed starting at address 0)";
    EXPECT_THAT([&]() { nvm.write(address, data, length); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(DeviceNvmTest, writeShouldFailIfValidationCheckFails)
{
    dut::DeviceNvm nvm(m_client, m_logger);

    const uint8_t byteWritten = 0xab;
    const uint8_t byteRead = 0xba;

    size_t address = 0;
    uint8_t data[1] { byteWritten };
    size_t length = sizeof(data);

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, writeNvMemory(address, _, length, memoryType, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::FileType::FILE_TYPE_CALIBRATION)).Times(1);
        EXPECT_CALL(*m_client, readNvMemory(address, _, length, memoryType, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::FileType::FILE_TYPE_CALIBRATION)).WillOnce(Invoke([byteRead](size_t /*address*/, uint8_t* data_, size_t /*length*/, dut::NvMemoryType /*memoryType*/, dut::NvMemorySize /*memorySize*/, dut::FileType /*fileType*/) {
            data_[0] = byteRead;
        }));
    }

    const char* expectedMessage = "NVM write failed validation check!";
    EXPECT_THAT([&]() { nvm.write(address, data, length); }, ThrowsMessage<std::runtime_error>(expectedMessage));
}
}