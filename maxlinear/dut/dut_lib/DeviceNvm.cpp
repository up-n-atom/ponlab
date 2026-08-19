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

#include "Exceptions.h"
#include "Protocol.h"
#include "dut/Client.h"
#include "dut/Logger.h"
#include "dut/Tools.h"

#include <fstream>
#include <stdexcept>
#include <string>

#ifdef LINUX_HOST
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

namespace dut {

DeviceNvm::DeviceNvm(std::shared_ptr<Client> client, std::shared_ptr<Logger> logger)
    : m_client(client)
    , m_logger(logger)
{
}

NvMemoryType DeviceNvm::getType() const
{
    return m_memoryType;
}

bool DeviceNvm::setType(NvMemoryType memoryType)
{
    if ((memoryType == NvMemoryType::MEMORY_TYPE_FLASH) || (memoryType == NvMemoryType::MEMORY_TYPE_EEPROM)) {
        m_memoryType = memoryType;
        return true;
    }

    return false;
}

size_t DeviceNvm::getSize() const
{
    return static_cast<size_t>(m_memorySize);
}

bool DeviceNvm::setSize(NvMemorySize memorySize)
{
    if ((memorySize == NvMemorySize::MEMORY_SIZE_BYTES_1K) || (memorySize == NvMemorySize::MEMORY_SIZE_BYTES_2K) || (memorySize == NvMemorySize::MEMORY_SIZE_BYTES_3K)) {
        m_memorySize = memorySize;
        m_buffer.length() = static_cast<size_t>(memorySize);
        return true;
    }

    return false;
}

const uint8_t* DeviceNvm::getData() const
{
    return m_buffer.data();
}

void DeviceNvm::load()
{
    if (NvMemorySize::MEMORY_SIZE_BYTES_1K != m_memorySize) {
        setSize(getFlashMemorySize());
    }

    read(0, m_buffer.data(), getSize(), false);
}

void DeviceNvm::loadFromFile(const std::string& fileName)
{
    struct stat stat_buf;
    int rc = stat(fileName.c_str(), &stat_buf);
    if (rc != 0) {
        throw std::invalid_argument("Unable to get file size for file '" + fileName + "'");
    }

    size_t fileSize = stat_buf.st_size;
    if (!setSize(static_cast<NvMemorySize>(fileSize))) {
        throw std::invalid_argument("Invalid file size for file '" + fileName + "'");
    }

    std::ifstream f(fileName, std::ios::in | std::ios::binary);
    if (!f.is_open()) {
        throw std::invalid_argument("Unable to open file '" + fileName + "' for reading");
    }

    f.read(reinterpret_cast<char*>(m_buffer.data()), fileSize);
    f.close();

    if (f.fail()) {
        throw std::invalid_argument("Unable to read file '" + fileName + "'");
    }
}

uint8_t DeviceNvm::getVersion() const
{
    return m_buffer.data()[nvmVersionAddress];
}

void DeviceNvm::read(size_t address, uint8_t* data, size_t length, bool useCache)
{
    if (address >= getSize()) {
        throw std::invalid_argument("The specified address (" + toString(address) + ") is too large (memory size is " + toString(getSize()) + ")");
    }

    if (!data) {
        throw std::invalid_argument("Parameter 'data' cannot be a null pointer");
    }

    if (length == 0) {
        throw std::invalid_argument("Parameter 'length' cannot be 0");
    }

    size_t maxLength = getSize() - address;
    if (length > maxLength) {
        throw std::invalid_argument("The specified length (" + toString(length) + ") is too large (only " + toString(maxLength) + " bytes allowed starting at address " + toString(address) + ")");
    }

    if (useCache) {
        memcpy(data, &m_buffer.data()[address], length);
    } else {
        size_t bytesRead = 0;
        while (bytesRead < length) {
            size_t bytesToRead = std::min(dut::maxNvMemoryAccessLength, (length - bytesRead));

            m_client->readNvMemory(address + bytesRead, &data[bytesRead], bytesToRead, m_memoryType, m_memorySize, FileType::FILE_TYPE_CALIBRATION);

            bytesRead += bytesToRead;
        }
    }
}

void DeviceNvm::saveToFile(const std::string& fileName)
{
    std::ofstream f(fileName, std::ios::out | std::ios::binary);
    if (!f.is_open()) {
        throw std::invalid_argument("Unable to open file '" + fileName + "' for writing");
    }

    f.write(reinterpret_cast<const char*>(getData()), getSize());
    f.close();

    if (f.fail()) {
        throw std::invalid_argument("Unable to write file '" + fileName + "'");
    }
}

void DeviceNvm::write(size_t address, const uint8_t* data, size_t length)
{
    if (address >= getSize()) {
        throw std::invalid_argument("The specified address (" + toString(address) + ") is too large (memory size is " + toString(getSize()) + ")");
    }

    if (!data) {
        throw std::invalid_argument("Parameter 'data' cannot be a null pointer");
    }

    if (length == 0) {
        throw std::invalid_argument("Parameter 'length' cannot be 0");
    }

    size_t maxLength = getSize() - address;
    if (length > maxLength) {
        throw std::invalid_argument("The specified length (" + toString(length) + ") is too large (only " + toString(maxLength) + " bytes allowed starting at address " + toString(address) + ")");
    }

    size_t bytesWritten = 0;
    while (bytesWritten < length) {
        size_t bytesToWrite = std::min(dut::maxNvMemoryAccessLength, (length - bytesWritten));

        m_client->writeNvMemory(address + bytesWritten, &data[bytesWritten], bytesToWrite, m_memoryType, m_memorySize, FileType::FILE_TYPE_CALIBRATION);

        bytesWritten += bytesToWrite;
    }

    read(address, &m_buffer.data()[address], length, false);

    if (memcmp(&m_buffer.data()[address], data, length) != 0) {
        throw std::runtime_error("NVM write failed validation check!");
    }
}

NvMemorySize DeviceNvm::getFlashMemorySize() const
{
    auto tryReadFlash = [&](NvMemorySize memorySize) noexcept {
        try {
            std::array<uint8_t, 1> dummy;
            size_t address = static_cast<size_t>(memorySize) - sizeof(dummy);
            m_client->readNvMemory(address, dummy.data(), dummy.size(), NvMemoryType::MEMORY_TYPE_FLASH, memorySize, FileType::FILE_TYPE_CALIBRATION);
            return true;
        } catch (...) {
            m_logger->log(LogLevel::LOG_LEVEL_WARNING, "Unable to read last byte from a " + toString(memorySize) + " bytes flash memory");
        }
        return false;
    };

    std::vector<NvMemorySize> validSizes { NvMemorySize::MEMORY_SIZE_BYTES_3K, NvMemorySize::MEMORY_SIZE_BYTES_2K, NvMemorySize::MEMORY_SIZE_BYTES_1K };

    for (const auto& size : validSizes) {
        if (tryReadFlash(size)) {
            return size;
        }
    }

    return NvMemorySize::MEMORY_SIZE_BYTES_1K;
}

}
