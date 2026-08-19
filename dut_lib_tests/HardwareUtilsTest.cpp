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

#include "HardwareUtils.h"
#include "dut/ClientMock.h"
#include "dut/DutImpl.h" // For maxMemoryAccessLength

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace dut {
namespace hardware {

    class HardwareUtilsTest : public ::testing::Test {
    protected:
        void SetUp() override
        {
            mockClient = std::make_shared<StrictMock<ClientMock>>();
        }

        std::shared_ptr<StrictMock<ClientMock>> mockClient;
    };

    // ========================================
    // writeChunkedMemory Template Tests
    // ========================================

    TEST_F(HardwareUtilsTest, writeChunkedMemoryShouldWriteSmallDataInSingleChunk)
    {
        // Arrange
        const std::vector<uint32_t> values = { 0x12345678, 0xABCDEF00, 0x55AA55AA };
        const uint32_t baseAddress = 0xA0BA0000;
        const size_t expectedBytesToWrite = values.size() * sizeof(uint32_t);

        // Expect single writeMemory call for small data
        EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_LMAC_MEM, baseAddress, _, expectedBytesToWrite))
            .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(values.data()), expectedBytesToWrite)));

        // Act
        writeChunkedMemory<ChipModule::CHIP_MODULE_LMAC_MEM>(mockClient, values, baseAddress);
    }

    TEST_F(HardwareUtilsTest, writeChunkedMemoryShouldWriteLargeDataInMultipleChunks)
    {
        // Arrange - Create data larger than maxMemoryAccessLength
        const size_t maxValuesPerChunk = dut::maxMemoryAccessLength / sizeof(uint32_t);
        const size_t totalValues = maxValuesPerChunk + 10; // Ensure we need multiple chunks

        std::vector<uint32_t> values(totalValues);
        for (size_t i = 0; i < totalValues; ++i) {
            values[i] = static_cast<uint32_t>(i + 1); // Fill with incremental values
        }

        const uint32_t baseAddress = 0xA0BA0000;

        // Expect first chunk (full size)
        const size_t firstChunkBytes = maxValuesPerChunk * sizeof(uint32_t);
        EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_LMAC_MEM, baseAddress, _, firstChunkBytes))
            .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(values.data()), firstChunkBytes)));

        // Expect second chunk (remaining data)
        const size_t remainingValues = totalValues - maxValuesPerChunk;
        const size_t secondChunkBytes = remainingValues * sizeof(uint32_t);
        const uint32_t secondChunkAddress = baseAddress + firstChunkBytes;
        EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_LMAC_MEM, secondChunkAddress, _, secondChunkBytes))
            .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(&values[maxValuesPerChunk]), secondChunkBytes)));

        // Act
        writeChunkedMemory<ChipModule::CHIP_MODULE_LMAC_MEM>(mockClient, values, baseAddress);
    }

    TEST_F(HardwareUtilsTest, writeChunkedMemoryShouldWorkWithDifferentChipModules)
    {
        // Arrange
        const std::vector<uint32_t> values = { 0xDEADBEEF };
        const uint32_t baseAddress = 0xA1234567;
        const size_t expectedBytesToWrite = sizeof(uint32_t);

        // Test with CHIP_MODULE_REGISTER
        EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_REGISTER, baseAddress, _, expectedBytesToWrite))
            .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(values.data()), expectedBytesToWrite)));

        // Act
        writeChunkedMemory<ChipModule::CHIP_MODULE_REGISTER>(mockClient, values, baseAddress);
    }

    TEST_F(HardwareUtilsTest, writeChunkedMemoryShouldHandleEmptyVector)
    {
        // Arrange
        const std::vector<uint32_t> emptyValues;
        const uint32_t baseAddress = 0xA0BA0000;

        // Expect no writeMemory calls for empty data
        EXPECT_CALL(*mockClient, writeMemory(_, _, _, _)).Times(0);

        // Act
        writeChunkedMemory<ChipModule::CHIP_MODULE_LMAC_MEM>(mockClient, emptyValues, baseAddress);
    }

    TEST_F(HardwareUtilsTest, writeChunkedMemoryShouldCalculateAddressesCorrectly)
    {
        // Arrange
        const std::vector<uint32_t> values = { 0x11111111, 0x22222222, 0x33333333 };
        const uint32_t baseAddress = 0xA0BA0000;

        // Each value is 4 bytes, so addresses should increment by 4 for each value
        // But since we're writing all at once, we expect one call with the base address
        const size_t expectedBytesToWrite = values.size() * sizeof(uint32_t);

        EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_PHY, baseAddress, _, expectedBytesToWrite))
            .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(values.data()), expectedBytesToWrite)));

        // Act
        writeChunkedMemory<ChipModule::CHIP_MODULE_PHY>(mockClient, values, baseAddress);
    }

    // ========================================
    // modifyRegister Template Tests
    // ========================================

    // Test register configuration for unit tests
    struct TestRegisterConfig {
        uint8_t bitMask;
        uint8_t newValue;

        TestRegisterConfig(uint8_t mask, uint8_t value)
            : bitMask(mask)
            , newValue(value)
        {
        }

        uint32_t applyModifications(uint32_t currentValue) const
        {
            // Clear bits specified by mask and set new value
            currentValue &= ~static_cast<uint32_t>(bitMask);
            currentValue |= static_cast<uint32_t>(newValue);
            return currentValue;
        }
    };

    TEST_F(HardwareUtilsTest, modifyRegisterShouldReadModifyWrite)
    {
        // Arrange
        const uint32_t registerAddress = 0xA1234567;
        const uint32_t currentRegisterValue = 0x12345678;
        const uint8_t bitMask = 0xFF; // Clear lower 8 bits
        const uint8_t newValue = 0xAB; // Set new lower 8 bits
        const uint32_t expectedNewValue = (currentRegisterValue & ~bitMask) | newValue; // 0x123456AB

        TestRegisterConfig config(bitMask, newValue);

        // Expect read operation
        EXPECT_CALL(*mockClient, readMemory(ChipModule::CHIP_MODULE_REGISTER, registerAddress, _, sizeof(uint32_t)))
            .WillOnce(DoAll(
                SetArrayArgument<2>(reinterpret_cast<const uint8_t*>(&currentRegisterValue),
                    reinterpret_cast<const uint8_t*>(&currentRegisterValue) + sizeof(uint32_t)),
                Return()));

        // Expect write operation with modified value
        EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_REGISTER, registerAddress, _, sizeof(uint32_t)))
            .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(&expectedNewValue), sizeof(uint32_t))));

        // Act
        modifyRegister(mockClient, registerAddress, config);
    }

    TEST_F(HardwareUtilsTest, modifyRegisterShouldHandleNoChangesNeeded)
    {
        // Arrange
        const uint32_t registerAddress = 0xA7654321;
        const uint32_t currentRegisterValue = 0xABCDEF00;

        // Config that doesn't change anything (mask=0, value=0)
        TestRegisterConfig config(0x00, 0x00);

        // Expect read operation
        EXPECT_CALL(*mockClient, readMemory(ChipModule::CHIP_MODULE_REGISTER, registerAddress, _, sizeof(uint32_t)))
            .WillOnce(DoAll(
                SetArrayArgument<2>(reinterpret_cast<const uint8_t*>(&currentRegisterValue),
                    reinterpret_cast<const uint8_t*>(&currentRegisterValue) + sizeof(uint32_t)),
                Return()));

        // Expect write operation with same value (no actual change)
        EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_REGISTER, registerAddress, _, sizeof(uint32_t)))
            .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(&currentRegisterValue), sizeof(uint32_t))));

        // Act
        modifyRegister(mockClient, registerAddress, config);
    }

    // Complex register configuration for more advanced testing
    struct ComplexRegisterConfig {
        uint32_t clearMask;
        uint32_t setMask;

        ComplexRegisterConfig(uint32_t clear, uint32_t set)
            : clearMask(clear)
            , setMask(set)
        {
        }

        uint32_t applyModifications(uint32_t currentValue) const
        {
            currentValue &= ~clearMask; // Clear specified bits
            currentValue |= setMask; // Set specified bits
            return currentValue;
        }
    };

    TEST_F(HardwareUtilsTest, modifyRegisterShouldHandleComplexBitOperations)
    {
        // Arrange
        const uint32_t registerAddress = 0xA1111111;
        const uint32_t currentRegisterValue = 0xFF00FF00; // 11111111 00000000 11111111 00000000
        const uint32_t clearMask = 0xF0F0F0F0; // Clear alternating nibbles
        const uint32_t setMask = 0x0A0A0A0A; // Set specific pattern
        // Expected: (0xFF00FF00 & ~0xF0F0F0F0) | 0x0A0A0A0A
        // Step 1: ~0xF0F0F0F0 = 0x0F0F0F0F
        // Step 2: 0xFF00FF00 & 0x0F0F0F0F = 0x0F000F00
        // Step 3: 0x0F000F00 | 0x0A0A0A0A = 0x0F0A0F0A
        const uint32_t expectedNewValue = 0x0F0A0F0A;

        ComplexRegisterConfig config(clearMask, setMask);

        // Expect read operation
        EXPECT_CALL(*mockClient, readMemory(ChipModule::CHIP_MODULE_REGISTER, registerAddress, _, sizeof(uint32_t)))
            .WillOnce(DoAll(
                SetArrayArgument<2>(reinterpret_cast<const uint8_t*>(&currentRegisterValue),
                    reinterpret_cast<const uint8_t*>(&currentRegisterValue) + sizeof(uint32_t)),
                Return()));

        // Expect write operation with modified value
        EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_REGISTER, registerAddress, _, sizeof(uint32_t)))
            .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(&expectedNewValue), sizeof(uint32_t))));

        // Act
        modifyRegister(mockClient, registerAddress, config);
    }

    TEST_F(HardwareUtilsTest, modifyRegisterShouldAlwaysUseRegisterModule)
    {
        // Arrange
        const uint32_t registerAddress = 0xA9999999;
        const uint32_t currentRegisterValue = 0x12345678;

        TestRegisterConfig config(0x00, 0x00);

        // Verify that both read and write operations use CHIP_MODULE_REGISTER
        EXPECT_CALL(*mockClient, readMemory(ChipModule::CHIP_MODULE_REGISTER, registerAddress, _, sizeof(uint32_t)))
            .WillOnce(DoAll(
                SetArrayArgument<2>(reinterpret_cast<const uint8_t*>(&currentRegisterValue),
                    reinterpret_cast<const uint8_t*>(&currentRegisterValue) + sizeof(uint32_t)),
                Return()));

        EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_REGISTER, registerAddress, _, sizeof(uint32_t)))
            .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(&currentRegisterValue), sizeof(uint32_t))));

        // Act
        modifyRegister(mockClient, registerAddress, config);
    }

    // ==========================================
    // Tests for writeSequentialRegisters
    // ==========================================

    // Test address provider for sequential register tests
    struct TestAddressProvider {
        uint32_t baseAddress;
        explicit TestAddressProvider(uint32_t addr)
            : baseAddress(addr)
        {
        }
        uint32_t getHeaderAddress() const { return baseAddress; }
    };

    TEST_F(HardwareUtilsTest, writeSequentialRegistersShouldWriteCorrectSequence)
    {
        // Arrange
        std::vector<uint32_t> values = { 0x11223344, 0x55667788 };
        TestAddressProvider addresses(0xA0000000);

        // Expect two sequential writes to consecutive addresses
        EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_REGISTER, 0xA0000000, _, 4))
            .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(&values[0]), 4)));

        EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_REGISTER, 0xA0000004, _, 4))
            .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(&values[1]), 4)));

        // Act
        writeSequentialRegisters<2>(mockClient, values, addresses);
    }

    TEST_F(HardwareUtilsTest, writeSequentialRegistersShouldThrowOnWrongCount)
    {
        // Arrange
        std::vector<uint32_t> values = { 0x11223344, 0x55667788, 0x99AABBCC }; // 3 values when expecting 2
        TestAddressProvider addresses(0xA0000000);

        // Act & Assert
        EXPECT_THROW({
            writeSequentialRegisters<2>(mockClient, values, addresses);
        },
            std::invalid_argument);
    }

    TEST_F(HardwareUtilsTest, writeSequentialRegistersShouldHandleSingleValue)
    {
        // Arrange
        std::vector<uint32_t> values = { 0xDEADBEEF };
        TestAddressProvider addresses(0xB0000000);

        // Expect single write
        EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_REGISTER, 0xB0000000, _, 4))
            .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(&values[0]), 4)));

        // Act
        writeSequentialRegisters<1>(mockClient, values, addresses);
    }

    TEST_F(HardwareUtilsTest, writeSequentialRegistersShouldHandleMultipleValues)
    {
        // Arrange
        std::vector<uint32_t> values = { 0x10203040, 0x50607080, 0x90A0B0C0, 0xD0E0F000 };
        TestAddressProvider addresses(0xC0000000);

        // Expect four sequential writes with correct address progression (base + i*4)
        for (uint32_t i = 0; i < static_cast<uint32_t>(values.size()); i++) {
            uint32_t expectedAddress = 0xC0000000 + (i * 4);
            EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_REGISTER, expectedAddress, _, 4))
                .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(&values[i]), 4)));
        }

        // Act
        writeSequentialRegisters<4>(mockClient, values, addresses);
    }

    TEST_F(HardwareUtilsTest, writeSequentialRegistersShouldValidateCountAtCompileTime)
    {
        // This test verifies the template compiles with correct count
        std::vector<uint32_t> threeValues = { 0x11111111, 0x22222222, 0x33333333 };
        TestAddressProvider addresses(0xD0000000);

        // Expect three sequential writes
        for (uint32_t i = 0; i < static_cast<uint32_t>(threeValues.size()); i++) {
            uint32_t expectedAddress = 0xD0000000 + (i * 4);
            EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_REGISTER, expectedAddress, _, 4))
                .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(&threeValues[i]), 4)));
        }

        // Act - This should compile and run successfully
        writeSequentialRegisters<3>(mockClient, threeValues, addresses);
    }

    TEST_F(HardwareUtilsTest, writeSequentialRegistersShouldAlwaysUseRegisterModule)
    {
        // Arrange
        std::vector<uint32_t> values = { 0xAAAABBBB };
        TestAddressProvider addresses(0xE0000000);

        // Verify that write operation always uses CHIP_MODULE_REGISTER
        EXPECT_CALL(*mockClient, writeMemory(ChipModule::CHIP_MODULE_REGISTER, 0xE0000000, _, 4))
            .With(Args<2, 3>(ElementsAreArray(reinterpret_cast<const uint8_t*>(&values[0]), 4)));

        // Act
        writeSequentialRegisters<1>(mockClient, values, addresses);
    }

} // namespace hardware
} // namespace dut
