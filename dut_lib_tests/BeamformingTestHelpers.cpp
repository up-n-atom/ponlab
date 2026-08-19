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

#include "BeamformingTestHelpers.h"
#include "EmbeddedResource.h"
#include "dut/DutImpl.h"
#include "resource.h"

#include <fstream>

// ====================================================================================================
// HELPER FUNCTIONS FOR BEAMFORMING TESTS
// ====================================================================================================
// These helper functions are shared between BeamformingUtilsTest and DutWithClientMockTest files.

namespace beamforming_test_helpers {

void setupWave600BeamformingExpectations(std::shared_ptr<testing::StrictMock<dut::ClientMock>> client, size_t valuesSize)
{
    using ::testing::_;

    // Wave600 header writes: 2 writes to exact header address
    EXPECT_CALL(*client, writeMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave600::headerAddress, _, 4));
    EXPECT_CALL(*client, writeMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave600::headerAddress + 4, _, 4));

    // Wave600 values writes: chunked based on maxMemoryAccessLength
    if (valuesSize == 0) {
        // If no values, expect >= 1 writeMemory calls, this is used in 160MHz mode and
        // 320MHz mode where there are lots of different value sizes so not tested.
        EXPECT_CALL(*client, writeMemory(dut::ChipModule::CHIP_MODULE_LMAC_MEM, ::testing::Ge(dut::beamforming_utils::beamforming::wave600::valuesBaseAddress), _, ::testing::Le(dut::maxMemoryAccessLength))).Times(::testing::AtLeast(1));
    } else {
        // valuesSize is the number of 32-bit chunks; convert to bytes for chunking
        size_t numChunks = (valuesSize * 4 + dut::maxMemoryAccessLength - 1) / dut::maxMemoryAccessLength;
        EXPECT_CALL(*client, writeMemory(dut::ChipModule::CHIP_MODULE_LMAC_MEM, ::testing::Ge(dut::beamforming_utils::beamforming::wave600::valuesBaseAddress), _, ::testing::Le(dut::maxMemoryAccessLength))).Times((int)numChunks);
    }
}

void setupWave700HeaderOperation(std::shared_ptr<testing::StrictMock<dut::ClientMock>> client, bool isSecondary)
{
    using ::testing::_;

    auto addr = isSecondary ? dut::beamforming_utils::getWave700SecondaryAddresses() : dut::beamforming_utils::getWave700PrimaryAddresses();

    // Wave700 header writes: 2 writes to exact band header address (2x4 bytes)
    EXPECT_CALL(*client, writeMemory(dut::ChipModule::CHIP_MODULE_REGISTER, addr.bfHeaderAddress, _, 4));
    EXPECT_CALL(*client, writeMemory(dut::ChipModule::CHIP_MODULE_REGISTER, addr.bfHeaderAddress + 4, _, 4));
}

void setupWave700PhaseDatabaseOperation(std::shared_ptr<testing::StrictMock<dut::ClientMock>> client,
    bool isSecondary,
    size_t valuesSize, int ehtMultiplier)
{
    using ::testing::_;

    auto addr = isSecondary ? dut::beamforming_utils::getWave700SecondaryAddresses() : dut::beamforming_utils::getWave700PrimaryAddresses();

    // Wave700 control register operations (sets apb_debug_select and su_page)
    EXPECT_CALL(*client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, addr.apbControlAddress, _, 4)).Times(ehtMultiplier);
    EXPECT_CALL(*client, writeMemory(dut::ChipModule::CHIP_MODULE_REGISTER, addr.apbControlAddress, _, 4)).Times(ehtMultiplier);

    // Wave700 values write to su_phases_database (chunked)
    if (valuesSize == 0) {
        // If no values, expect >= 1 writeMemory calls, this is used in 160MHz mode and
        // 320MHz mode where there are lots of different value sizes so not tested.
        EXPECT_CALL(*client, writeMemory(dut::ChipModule::CHIP_MODULE_LMAC_MEM, ::testing::Ge(addr.bfValuesAddress), _, ::testing::Le(dut::maxMemoryAccessLength))).Times(::testing::AtLeast(ehtMultiplier));
    } else {
        size_t numChunks = (valuesSize * 4 + dut::maxMemoryAccessLength - 1) / dut::maxMemoryAccessLength;
        EXPECT_CALL(*client, writeMemory(dut::ChipModule::CHIP_MODULE_LMAC_MEM, ::testing::Ge(addr.bfValuesAddress), _, ::testing::Le(dut::maxMemoryAccessLength))).Times((int)numChunks * ehtMultiplier);
    }
}

void setupWave700BeamformingExpectations(std::shared_ptr<testing::StrictMock<dut::ClientMock>> client, size_t valuesSize, bool isEht160, bool isEht320)
{
    using ::testing::_;

    if (isEht160) {
        // EHT 160MHz: 1 header + 2 phase database writes (su_page 0 and su_page 4) to specified band
        setupWave700HeaderOperation(client, false);

        // Single call with ehtMultiplier=2 for both vhtHeEht (su_page 0) and ehtExtra (su_page 4) operations
        setupWave700PhaseDatabaseOperation(client, false, valuesSize, 2);
    } else if (isEht320) {
        // EHT 320MHz: 6GHz only - always uses band2 and band2_upper regardless of band parameter
        // 2 headers + 4 phase database writes
        setupWave700HeaderOperation(client, false); // band2 (6GHz)
        setupWave700HeaderOperation(client, true); // band2_upper (6GHz upper)

        // Phase database operations: 2 operations per band (vhtHeEht and ehtExtra for each band)
        setupWave700PhaseDatabaseOperation(client, false, valuesSize, 2); // band2 (6GHz) - both su_page 0 and 4
        setupWave700PhaseDatabaseOperation(client, true, valuesSize, 2); // band2_upper (6GHz upper) - both su_page 0 and 4
    } else {
        // Standard Wave700 operation: 1 header + 1 phase database write to specified band
        setupWave700HeaderOperation(client, false);

        // One phase database operation: vhtHeEht (su_page 0) only
        setupWave700PhaseDatabaseOperation(client, false, valuesSize, 1);
    }
}

template <>
std::unique_ptr<TemporaryFile> createTempFileFromSource<int>(const int& source)
{
    if (source == 0)
        return nullptr;
    EmbeddedResource resource(source, BEAMFORMINGMATRIX);
    return std::unique_ptr<TemporaryFile>(new TemporaryFile(resource.getData(), resource.getSize()));
}

template <>
std::unique_ptr<TemporaryFile> createTempFileFromSource<std::string>(const std::string& source)
{
    if (source.empty())
        return nullptr;
    return std::unique_ptr<TemporaryFile>(
        new TemporaryFile(reinterpret_cast<const uint8_t*>(source.c_str()), source.length()));
}

size_t countNonEmptyLines(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        return 0;
    }

    size_t count = 0;
    std::string line;
    while (std::getline(file, line)) {
        // Check if line contains any non-whitespace characters
        if (line.find_first_not_of(" \t\r\n") != std::string::npos) {
            ++count;
        }
    }
    return count;
}

template <typename T1, typename T2, typename T3>
BeamformingFileSetWithSizes createBeamformingFileSetWithSizes(
    std::vector<std::unique_ptr<TemporaryFile>>& tempFiles,
    const T1& headerSource,
    const T2& valuesSource,
    const T3& extEhtValuesSource)
{

    auto headerFile = createTempFileFromSource(headerSource);
    auto valuesFile = createTempFileFromSource(valuesSource);
    auto extEhtFile = createTempFileFromSource(extEhtValuesSource);

    BeamformingFileSetWithSizes result;

    if (headerFile) {
        result.fileSet.headerFile = headerFile->getFilename();
        result.headerLineCount = countNonEmptyLines(std::string(headerFile->getFilename()));
        tempFiles.push_back(std::move(headerFile));
    }

    if (valuesFile) {
        result.fileSet.valuesFile = valuesFile->getFilename();
        result.valuesLineCount = countNonEmptyLines(std::string(valuesFile->getFilename()));
        tempFiles.push_back(std::move(valuesFile));
    }

    if (extEhtFile) {
        result.fileSet.extValuesEhtFile = extEhtFile->getFilename();
        result.extValuesEhtLineCount = countNonEmptyLines(std::string(extEhtFile->getFilename()));
        tempFiles.push_back(std::move(extEhtFile));
    }

    return result;
}

// Explicit template instantiations for the common cases
template BeamformingFileSetWithSizes createBeamformingFileSetWithSizes<int, int, int>(
    std::vector<std::unique_ptr<TemporaryFile>>& tempFiles,
    const int& headerSource,
    const int& valuesSource,
    const int& extEhtValuesSource);

template BeamformingFileSetWithSizes createBeamformingFileSetWithSizes<std::string, std::string, int>(
    std::vector<std::unique_ptr<TemporaryFile>>& tempFiles,
    const std::string& headerSource,
    const std::string& valuesSource,
    const int& extEhtValuesSource);

} // namespace beamforming_test_helpers
