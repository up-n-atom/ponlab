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
 *                                          Copyright (c) 2025, MaxLinear, Inc.
 *  ***************************************************************************************
 *  </legal_notice>
 */

#ifndef _DUT_HARDWARE_UTILS_H_
#define _DUT_HARDWARE_UTILS_H_

#include "dut/Client.h"
#include "dut/Types.h"

#include <memory>
#include <stdexcept> // For std::invalid_argument
#include <vector>

namespace dut {
namespace hardware {

    /**
     * @brief Template for chunked memory writes
     * 
     * Generic utility for writing large data arrays in chunks based on maxMemoryAccessLength.
     * Useful for any hardware component that needs to write large amounts of data.
     * Each value is treated as a 32-bit word (4 bytes).
     *
     * @tparam MODULE The chip module to write to (e.g., CHIP_MODULE_LMAC_MEM)
     * @param client Hardware client for memory operations
     * @param values Vector of 32-bit values to write
     * @param baseAddress Base memory address to start writing from
     *
     * Example usage:
     * @code
     * std::vector<uint32_t> data = {0x12345678, 0xABCDEF00, ...};
     * hardware::writeChunkedMemory<ChipModule::CHIP_MODULE_LMAC_MEM>(client, data, 0xA0BA0000);
     * @endcode
     */
    template <ChipModule MODULE>
    void writeChunkedMemory(std::shared_ptr<Client> client,
        const std::vector<uint32_t>& values,
        uint32_t baseAddress)
    {
        // Calculate maximum number of 32-bit values per chunk
        const size_t maxValuesPerChunk = dut::maxMemoryAccessLength / sizeof(uint32_t);

        size_t valuesWritten = 0;
        while (valuesWritten < values.size()) {
            size_t remaining = values.size() - valuesWritten;
            size_t valuesToWrite = (remaining < maxValuesPerChunk) ? remaining : maxValuesPerChunk;
            size_t bytesToWrite = valuesToWrite * sizeof(uint32_t);

            client->writeMemory(MODULE,
                baseAddress + (valuesWritten * sizeof(uint32_t)),
                reinterpret_cast<const uint8_t*>(&values[valuesWritten]),
                bytesToWrite);
            valuesWritten += valuesToWrite;
        }
    }

    /**
     * @brief Template for register read-modify-write operations
     * 
     * Generic utility for reading a register, modifying specific bits, and writing back.
     * The RegisterConfig type must provide an applyModifications(uint32_t currentValue) method
     * that takes the current register value and returns the modified value.
     *
     * @tparam RegisterConfig Configuration class that defines how to modify the register
     * @param client Hardware client for memory operations
     * @param registerAddress Address of the register to modify
     * @param config Configuration object with modification logic
     *
     * Example RegisterConfig implementation:
     * @code
     * struct MyRegisterConfig {
     *     uint8_t newValue;
     *     uint32_t applyModifications(uint32_t currentValue) const {
     *         currentValue &= ~0xFF;  // Clear lower 8 bits
     *         currentValue |= newValue;  // Set new value
     *         return currentValue;
     *     }
     * };
     * 
     * MyRegisterConfig config{0x42};
     * hardware::modifyRegister(client, 0xA1234567, config);
     * @endcode
     */
    template <typename RegisterConfig>
    void modifyRegister(std::shared_ptr<Client> client,
        uint32_t registerAddress,
        const RegisterConfig& config)
    {
        // Read current register value
        uint32_t currentValue = 0;
        client->readMemory(ChipModule::CHIP_MODULE_REGISTER, registerAddress,
            reinterpret_cast<uint8_t*>(&currentValue), sizeof(currentValue));

        // Apply modifications using config
        uint32_t newValue = config.applyModifications(currentValue);

        // Write back modified value
        client->writeMemory(ChipModule::CHIP_MODULE_REGISTER, registerAddress,
            reinterpret_cast<uint8_t*>(&newValue), sizeof(newValue));
    }

    /**
     * @brief Template for writing fixed-size data to sequential register addresses
     * 
     * Generic utility for writing a fixed number of 32-bit values to sequential
     * register addresses. Useful for any hardware component that needs to write
     * structured data like headers, configuration blocks, etc.
     *
     * @tparam EXPECTED_COUNT Expected number of 32-bit values (compile-time validation)
     * @tparam AddressProvider Type that provides getHeaderAddress() method
     * @param client Hardware client for memory operations
     * @param values Vector of 32-bit values to write
     * @param addresses Address provider object
     *
     * Example usage:
     * @code
     * struct MyAddressProvider {
     *     uint32_t getHeaderAddress() const { return 0xA1234567; }
     * };
     * 
     * std::vector<uint32_t> header = {0x12345678, 0xABCDEF00};
     * MyAddressProvider addresses;
     * hardware::writeSequentialRegisters<2>(client, header, addresses);
     * @endcode
     */
    template <size_t EXPECTED_COUNT, typename AddressProvider>
    void writeSequentialRegisters(std::shared_ptr<Client> client,
        const std::vector<uint32_t>& values,
        const AddressProvider& addresses)
    {
        if (values.size() != EXPECTED_COUNT) {
            throw std::invalid_argument("Expected exactly " + std::to_string(EXPECTED_COUNT) + " x 32-bit values, got " + std::to_string(values.size()));
        }

        for (size_t i = 0; i < values.size(); i++) {
            constexpr size_t bytesToWrite = 4;
            client->writeMemory(ChipModule::CHIP_MODULE_REGISTER,
                addresses.getHeaderAddress() + (i * 4),
                reinterpret_cast<const uint8_t*>(&values[i]),
                bytesToWrite);
        }
    }

} // namespace hardware
} // namespace dut

#endif // _DUT_HARDWARE_UTILS_H_
