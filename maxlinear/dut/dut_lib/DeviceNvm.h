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

#ifndef _DUT_DEVICE_NVM_H_
#define _DUT_DEVICE_NVM_H_

#include "NonVolatileMemory.h"

#include "FixedSizeBuffer.h"

#include <memory>
#include <string>

namespace dut {

class Client;
class Logger;

class DeviceNvm : public NonVolatileMemory {
public:
    DeviceNvm(std::shared_ptr<Client> client, std::shared_ptr<Logger> logger);

    NvMemoryType getType() const override;
    bool setType(NvMemoryType memoryType) override;

    size_t getSize() const override;
    bool setSize(NvMemorySize memorySize) override;

    const uint8_t* getData() const override;

    void load() override;
    void loadFromFile(const std::string& fileName) override;
    void saveToFile(const std::string& fileName) override;

    uint8_t getVersion() const override;

    void read(size_t address, uint8_t* data, size_t length, bool useCache = true) override;
    void write(size_t address, const uint8_t* data, size_t length) override;

private:
    NvMemoryType m_memoryType = NvMemoryType::MEMORY_TYPE_FLASH;
    NvMemorySize m_memorySize = NvMemorySize::MEMORY_SIZE_BYTES_1K;
    std::shared_ptr<Client> m_client;
    std::shared_ptr<Logger> m_logger;

    /*
     * Data buffer is sized to contain the biggest EEPROM memory allowed.
     */
    FixedSizeBuffer<static_cast<size_t>(NvMemorySize::MEMORY_SIZE_BYTES_3K)> m_buffer;

    NvMemorySize getFlashMemorySize() const;
};
}

#endif
