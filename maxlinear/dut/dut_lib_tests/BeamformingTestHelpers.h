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

#ifndef BEAMFORMING_TEST_HELPERS_H
#define BEAMFORMING_TEST_HELPERS_H

#include "BeamformingUtils.h"
#include "TemporaryFile.h"
#include "dut/ClientMock.h"

#include <gmock/gmock.h>
#include <memory>
#include <string>
#include <vector>

namespace beamforming_test_helpers {

/**
 * @brief Set up mock expectations for Wave600 beamforming operations
 *
 * Configures GoogleMock expectations for Wave600 beamforming hardware writes:
 * - Header write (8 bytes total: 2 x 4-byte writes)
 * - Values write (chunked based on maxMemoryAccessLength)
 *
 * @param client Mock client to set expectations on
 * @param valuesSize Size of beamforming values data in bytes
 */
void setupWave600BeamformingExpectations(std::shared_ptr<testing::StrictMock<dut::ClientMock>> client, size_t valuesSize);

/**
 * @brief Set up mock expectations for Wave700 beamforming header operation
 *
 * Configures GoogleMock expectations for Wave700 header writes to a specific segment.
 * Each header operation consists of 2 x 4-byte writes (8 bytes total).
 *
 * @param client Mock client to set expectations on
 * @param isSecondary whether to expect secondary segment addresses (for EHT 320MHz)
 */
void setupWave700HeaderOperation(std::shared_ptr<testing::StrictMock<dut::ClientMock>> client,
    bool isSecondary);

/**
 * @brief Set up mock expectations for Wave700 phase database operation
 *
 * Configures GoogleMock expectations for a single Wave700 phase database write operation.
 * Each operation includes:
 * - Control register read/write (to set apb_debug_select and su_page)
 * - Chunked values write to su_phases_database (based on maxMemoryAccessLength)
 *
 * @param client Mock client to set expectations on
 * @param isSecondary whether to expect secondary segment addresses (for EHT 320MHz)
 * @param valuesSize Size of beamforming values data in bytes
 * @param ehtMultiplier In eht 160MHz and 320MHz modes, there are double the writes (su page 0 and 4)
 */
void setupWave700PhaseDatabaseOperation(std::shared_ptr<testing::StrictMock<dut::ClientMock>> client,
    bool isSecondary,
    size_t valuesSize, int ehtMultiplier = 1);

/**
 * @brief Set up mock expectations for complete Wave700 beamforming operations
 *
 * Configures GoogleMock expectations for Wave700 beamforming based on PHY mode:
 *
 * Standard (VHT/HT/HE):
 *   - 1 header write to primary addresses
 *   - 1 phase database write to primary addresses (su_page 0)
 *
 * EHT 160MHz:
 *   - 1 header write to primary addresses
 *   - 2 phase database writes to primary addresses (su_page 0 and su_page 4)
 *
 * EHT 320MHz:
 *   - 2 header writes (primary and secondary addresses)
 *   - 4 phase database writes (2 to primary, 2 to secondary, each with su_page 0 and 4)
 *
 * @param client Mock client to set expectations on
 * @param valuesSize Size of beamforming values data in bytes
 * @param isEht160 True if EHT 160MHz mode (extra phase database write)
 * @param isEht320 True if EHT 320MHz mode (6GHz only - forces BAND2/BAND2_UPPER operations)
 *
 * @note EHT 320MHz is only supported on 6GHz (BAND2), band parameter is ignored for EHT 320MHz
 */
void setupWave700BeamformingExpectations(
    std::shared_ptr<testing::StrictMock<dut::ClientMock>> client,
    size_t valuesSize,
    bool isEht160 = false,
    bool isEht320 = false);

template <typename T>
std::unique_ptr<TemporaryFile> createTempFileFromSource(const T& source);

template <>
std::unique_ptr<TemporaryFile> createTempFileFromSource<int>(const int& source);

template <>
std::unique_ptr<TemporaryFile> createTempFileFromSource<std::string>(const std::string& source);

/**
 * Helper function to count non-empty lines in a file.
 * Each line represents 32 bytes (1 row) of beamforming data.
 * 
 * @param filename Path to the file to analyze
 * @return Number of non-empty lines in the file
 */
size_t countNonEmptyLines(const std::string& filename);

/**
 * Extended beamforming file set that includes both file paths and their line counts.
 * Each line represents 32 bytes (1 row) of beamforming data.
 * This is used when working out the expected number of chunks to be written.
 */
struct BeamformingFileSetWithSizes {
    dut::BeamformingFilePathSet_t fileSet;
    size_t headerLineCount = 0; // Number of non-empty lines in header file
    size_t valuesLineCount = 0; // Number of non-empty lines in values file
    size_t extValuesEhtLineCount = 0; // Number of non-empty lines in extended EHT values file
};

template <typename T1, typename T2, typename T3 = int>
BeamformingFileSetWithSizes createBeamformingFileSetWithSizes(
    std::vector<std::unique_ptr<TemporaryFile>>& tempFiles,
    const T1& headerSource,
    const T2& valuesSource,
    const T3& extEhtValuesSource = 0);

} // namespace beamforming_test_helpers

#endif // BEAMFORMING_TEST_HELPERS_H
