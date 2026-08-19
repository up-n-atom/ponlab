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

#ifndef _DUT_BEAMFORMING_UTILS_H_
#define _DUT_BEAMFORMING_UTILS_H_

#include "dut/Client.h"
#include "dut/Types.h"

#include <memory>
#include <string>
#include <vector>

namespace dut {
namespace beamforming_utils {

    /**
     * @brief Beamforming hardware configuration constants
     *
     * These constants define memory addresses, register masks, and control values
     * for beamforming operations on Wave600 and Wave700 chipsets.
     */
    namespace beamforming {

        // Wave600 chipset addresses (See WLANRTSYS-38009)
        namespace wave600 {
            constexpr uint32_t headerAddress = 0xA0B83000;
            constexpr uint32_t valuesBaseAddress = 0xA0BA0000;
        }

        // Wave700 chipset configuration (See WLANRTSYS-53879)
        namespace wave700 {
            // Wave700 beamforming addresses
            constexpr uint32_t primaryBfHeaderAddress = 0xA1723000;
            constexpr uint32_t primaryBfValuesAddress = 0xA172C000;
            constexpr uint32_t primaryApbControlAddress = 0xA17203C0;

            constexpr uint32_t secondaryBfHeaderAddress = 0xA1743000;
            constexpr uint32_t secondaryBfValuesAddress = 0xA174C000;
            constexpr uint32_t secondaryApbControlAddress = 0xA17403C0;

            // Register field definitions
            namespace registers {
                // Bit masks
                constexpr uint32_t rxfdApbDebugSelectMask = 0x3;
                constexpr uint32_t suPageMask = 0x700;

                // Bit field positions
                constexpr uint32_t suPageShift = 8; // bits 10:8

                // Control values
                constexpr uint32_t rxfdApbDebugSelectEnable = 1; // Enable access to SU phases database
            }

            // SU page configurations
            namespace suPage {
                constexpr uint32_t vhtHeEht = 0; // VHT/HE/EHT phases
                constexpr uint32_t ehtExtra = 4; // EHT extra phases (BW=160,320MHz)
            }
        }
    }

    /**
     * @brief Wave700 beamforming address set
     */
    struct Wave700BeamformingAddresses {
        uint32_t bfHeaderAddress;
        uint32_t apbControlAddress;
        uint32_t bfValuesAddress;
    };

    /**
     * @brief Get Wave700 primary addresses (primary segment)
     * @return Wave700BeamformingAddresses structure with primary addresses
     */
    inline Wave700BeamformingAddresses getWave700PrimaryAddresses()
    {
        return {
            beamforming::wave700::primaryBfHeaderAddress,
            beamforming::wave700::primaryApbControlAddress,
            beamforming::wave700::primaryBfValuesAddress
        };
    }

    /**
     * @brief Get Wave700 secondary addresses (secondary segment for EHT 320MHz)
     * @return Wave700BeamformingAddresses structure with secondary addresses
     */
    inline Wave700BeamformingAddresses getWave700SecondaryAddresses()
    {
        return {
            beamforming::wave700::secondaryBfHeaderAddress,
            beamforming::wave700::secondaryApbControlAddress,
            beamforming::wave700::secondaryBfValuesAddress
        };
    }

    /**
     * @brief Structure containing beamforming data read from a single file set
     */
    struct BeamformingBandData {
        std::vector<uint32_t> header; // Header data (2 x 32-bit values)
        std::vector<uint32_t> values; // Standard values data
        std::vector<uint32_t> extValues; // Extended EHT values (empty if not used)

        // Essential validation methods
        bool isValid() const { return !header.empty() && !values.empty(); }
        bool hasExtendedValues() const { return !extValues.empty(); }

        // Clear all data
        void clear()
        {
            header.clear();
            values.clear();
            extValues.clear();
        }
    };

    /**
     * @brief Structure containing beamforming data read from primary and optional secondary file sets
     *
     * - Primary: Beamforming data for primary segment (Used by all configurations)
     * - Secondary: Beamforming data for secondary segment (optional, empty if not used - used for EHT 320MHz only)
     */
    struct BeamformingData {
        BeamformingBandData primary; // Primary data set (always required)
        BeamformingBandData secondary; // Secondary data set (optional, for EHT 320MHz)

        // Essential validation methods
        bool hasSecondary() const { return secondary.isValid(); }

        // Clear all data
        void clear()
        {
            primary.clear();
            secondary.clear();
        }
    };

    /**
 * @brief Validation result structure
 */
    struct ValidationResult {
        bool isValid;
        std::string errorMessage;
    };

    /**
     * @brief Check if beamforming PHY mode is compatible with DUT PHY mode
     * @param beamformingPhyMode PHY mode from beamforming header
     * @param dutPhyMode PHY mode from DUT settings
     * @return true if the PHY modes are compatible, false otherwise
     */
    bool isBeamformingPhyModeCompatible(PhyMode beamformingPhyMode, PhyMode dutPhyMode);

    /**
     * @brief Validate beamforming header compatibility with DUT configuration
     * @param headerInfo Parsed beamforming header information using standard DUT types
     * @param dutBandwidth Current DUT bandwidth setting
     * @param dutPhyMode Current DUT PHY mode setting
     * @return ValidationResult with success status and error message if invalid
     */
    ValidationResult validateBeamformingCompatibility(const BeamformingHeaderInfo_t& headerInfo,
        Bandwidth dutBandwidth,
        PhyMode dutPhyMode);

    /**
     * @brief Extracts PHY mode and bandwidth information from beamforming matrix header LSB
     *
     * This function parses the beamforming matrix header LSB to extract the PHY mode (packet format)
     * and bandwidth information in MHz.
     *
     * @param headerLSB The first 32 bits (LSB) from the beamforming header data
     * @param hwType The hardware type (HARDWARE_TYPE_GEN6 for Wave600 or HARDWARE_TYPE_GEN7 for Wave700)
     * @param headerInfo Output parameter for the extracted beamforming header information
     * @return true if extraction was successful, false if invalid header format
     *
     * Example usage:
     * @code
     * BeamformingHeaderInfo_t headerInfo;
     * if (extractBeamformingHeaderInfo(headerLSB, HARDWARE_TYPE_GEN7, headerInfo)) {
     *     if (headerInfo.phyMode == PhyMode::PHY_MODE_BE && headerInfo.bandwidth == Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY) {
     *         // Handle EHT 320MHz case
     *     }
     * }
     * @endcode
     */
    bool extractBeamformingHeaderInfo(uint32_t headerLSB, HardwareType hwType, BeamformingHeaderInfo_t& headerInfo);

    /**
     * @brief Read and parse a hex string file into a vector of 32-bit values
     * @param fileName Name of the file to read
     * @return Vector of 32-bit values representing the file content
     * @throws std::invalid_argument if file cannot be opened, read, or parsed
     */
    std::vector<uint32_t> readHexStringFile(const std::string& fileName);

    /**
     * @brief Read and validate a single beamforming file set
     * @param fileSet File set containing header, values, and optional extended EHT values
     * @return BeamformingBandData structure with read data
     * @throws std::invalid_argument if files cannot be opened, read, or parsed
     */
    BeamformingBandData readBeamformingFilePathSet(const dut::BeamformingFilePathSet_t& fileSet);

    /**
     * @brief Read and validate beamforming files from one or two file sets
     *
     * Supports Wave700 beamforming configurations:
     * - Single set: Standard operations (header + values + optional extValues)
     * - Dual set: EHT 320MHz operations (two complete file sets for dual segments)
     *
     * @param primarySet Primary file set (always required)
     * @param secondarySet Secondary file set (optional, empty dut::BeamformingFilePathSet_t if not used)
     * @return BeamformingData structure with all read data
     * @throws std::invalid_argument if files cannot be opened, read, or parsed
     */
    BeamformingData readBeamformingFiles(const dut::BeamformingFilePathSet_t& primarySet,
        const dut::BeamformingFilePathSet_t& secondarySet = dut::BeamformingFilePathSet_t {});

    /**
     * @brief Check if a BeamformingFilePathSet_t has extended EHT values
     * @param fileSet The file set to check
     * @return true if extValuesEhtFile is specified and not NULL/empty
     */
    bool hasExtendedEhtValues(const dut::BeamformingFilePathSet_t& fileSet);

    /**
     * @brief Check if a BeamformingFilePathSet_t is valid (has required files)
     * @param fileSet The file set to check
     * @return true if both headerFile and valuesFile are specified and not NULL/empty
     */
    bool isValid(const dut::BeamformingFilePathSet_t& fileSet);

    /**
     * @brief Write beamforming header to Wave600 hardware
     *
     * @param client Hardware client for memory operations
     * @param header 2 x 32-bit beamforming header data
     */
    void writeWave600BeamformingHeader(std::shared_ptr<Client> client, const std::vector<uint32_t>& header);

    /**
     * @brief Write beamforming values to Wave600 hardware
     *
     * Performs chunked write of beamforming values to Wave600 hardware
     *
     * @param client Hardware client for memory operations
     * @param values Beamforming values data to write (32-bit values)
     */
    void writeWave600BeamformingValues(std::shared_ptr<Client> client, const std::vector<uint32_t>& values);

    /**
     * @brief Write beamforming header to Wave700 hardware
     *
     * @param client Hardware client for memory operations
     * @param header 2 x 32-bit beamforming header data
     * @param addresses Wave700 addresses structure (use getWave700PrimaryAddresses() or getWave700SecondaryAddresses())
     */
    void writeWave700BeamformingHeader(std::shared_ptr<Client> client, const std::vector<uint32_t>& header, const Wave700BeamformingAddresses& addresses);

    /**
     * @brief Write beamforming values to Wave700 phase database
     *
     * Performs chunked write of beamforming values to Wave700 hardware
     *
     * @param client Hardware client for memory operations
     * @param values Beamforming values data to write (32-bit values)
     * @param addresses Wave700 addresses structure (use getWave700PrimaryAddresses() or getWave700SecondaryAddresses())
     * @param suPage SU page value (0=vhtHeEht, 4=ehtExtra)
     */
    void writeWave700BeamformingValues(std::shared_ptr<Client> client, const std::vector<uint32_t>& values, const Wave700BeamformingAddresses& addresses, uint8_t suPage);

    // ========================================
    // DEBUGGING/LOGGING SUPPORT
    // ========================================

    /**
     * @brief Stream output operator for ValidationResult
     * @param out Output stream
     * @param value ValidationResult to output
     * @return Reference to output stream
     */
    std::ostream& operator<<(std::ostream& out, const ValidationResult& value);

} // namespace beamforming_utils
} // namespace dut

#endif // _DUT_BEAMFORMING_UTILS_H_
