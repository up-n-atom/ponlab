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

#include "BeamformingUtils.h"
#include "HardwareUtils.h" // For generic hardware operation templates
#include "SharedHeaders.h" // For hardware register bit masks and constants
#include "dut/DutImpl.h" // For beamforming namespace constants
#include "dut/Tools.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <stdexcept>

namespace dut {
namespace beamforming_utils {

    // ========================================
    // ADDRESS PROVIDERS FOR DIFFERENT CHIPSETS
    // ========================================

    struct Wave600AddressProvider {
        uint32_t getHeaderAddress() const { return beamforming::wave600::headerAddress; }
        uint32_t getValuesAddress() const { return beamforming::wave600::valuesBaseAddress; }
    };

    struct Wave700AddressProvider {
        const Wave700BeamformingAddresses& addresses;

        explicit Wave700AddressProvider(const Wave700BeamformingAddresses& addr)
            : addresses(addr)
        {
        }

        uint32_t getHeaderAddress() const { return addresses.bfHeaderAddress; }
        uint32_t getValuesAddress() const { return addresses.bfValuesAddress; }
    };

    // ========================================
    // REGISTER CONFIGURATION CLASSES
    // ========================================

    struct Wave700ControlRegisterConfig {
        uint8_t suPage;

        explicit Wave700ControlRegisterConfig(uint8_t page)
            : suPage(page)
        {
        }

        uint32_t applyModifications(uint32_t currentValue) const
        {
            // Clear and set apb_debug_select
            currentValue &= ~beamforming::wave700::registers::rxfdApbDebugSelectMask;
            currentValue |= beamforming::wave700::registers::rxfdApbDebugSelectEnable;

            // Clear and set su_page
            currentValue &= ~beamforming::wave700::registers::suPageMask;
            currentValue |= (static_cast<uint32_t>(suPage) << beamforming::wave700::registers::suPageShift);

            return currentValue;
        }
    };

    // ========================================
    // BEAMFORMING UTILITY FUNCTIONS
    // ========================================

    bool isBeamformingPhyModeCompatible(PhyMode beamformingPhyMode, PhyMode dutPhyMode)
    {
        return beamformingPhyMode == dutPhyMode || (beamformingPhyMode == PhyMode::PHY_MODE_N_5 && dutPhyMode == PhyMode::PHY_MODE_N_2_4) || (beamformingPhyMode == PhyMode::PHY_MODE_N_2_4 && dutPhyMode == PhyMode::PHY_MODE_N_5);
    }

    ValidationResult validateBeamformingCompatibility(const BeamformingHeaderInfo_t& headerInfo,
        Bandwidth dutBandwidth,
        PhyMode dutPhyMode)
    {
        // Check bandwidth compatibility
        if (headerInfo.bandwidth != dutBandwidth) {
            return { false, "Beamforming header bandwidth (" + toString(headerInfo.bandwidth) + ") does not match DUT spectrum bandwidth (" + toString(dutBandwidth) + ")" };
        }

        // Check PHY mode compatibility
        if (!isBeamformingPhyModeCompatible(headerInfo.phyMode, dutPhyMode)) {
            return { false, "Beamforming header PHY mode (" + toString(headerInfo.phyMode) + ") does not match DUT PHY mode (" + toString(dutPhyMode) + ")" };
        }

        return { true, "" }; // Valid - no error
    }

    bool extractBeamformingHeaderInfo(uint32_t headerLSB, HardwareType hwType, BeamformingHeaderInfo_t& headerInfo)
    {
        // Extract PHY mode (packet format) from bits 2:0 - same for all chips
        uint8_t rawPhyMode = static_cast<uint8_t>(headerLSB & BF_HEADER_PACKET_FORMAT_MASK);

        // Validate PHY mode (1 is reserved/invalid, max is 4 for EHT)
        if (rawPhyMode > 4 || rawPhyMode == 1) {
            return false; // Invalid PHY mode
        }

        // Convert raw PHY mode to DUT PhyMode enum
        switch (rawPhyMode) {
        case 0: // HT
            headerInfo.phyMode = PhyMode::PHY_MODE_N_5; // Default to 5GHz N mode
            break;
        case 2: // VHT
            headerInfo.phyMode = PhyMode::PHY_MODE_AC;
            break;
        case 3: // HE
            headerInfo.phyMode = PhyMode::PHY_MODE_AX;
            break;
        case 4: // EHT
            headerInfo.phyMode = PhyMode::PHY_MODE_BE;
            break;
        default:
            return false;
        }

        // Extract raw bandwidth and convert to DUT Bandwidth enum based on hardware type
        uint8_t rawBandwidth;

        switch (hwType) {
        case HardwareType::HARDWARE_TYPE_GEN6: // Wave600
            // Wave600: Use different BW masks based on PHY mode (HT vs VHT/HE)
            if (rawPhyMode == 0) { // HT mode
                rawBandwidth = static_cast<uint8_t>((headerLSB & WAVE600_BF_HEADER_HT_BW_MASK) >> 20);
                // HT mode: 0=20MHz, 1=40MHz
                switch (rawBandwidth) {
                case 0:
                    headerInfo.bandwidth = Bandwidth::BANDWIDTH_TWENTY;
                    break;
                case 1:
                    headerInfo.bandwidth = Bandwidth::BANDWIDTH_FOURTY;
                    break;
                default:
                    return false;
                }
            } else { // VHT/HE modes
                rawBandwidth = static_cast<uint8_t>((headerLSB & WAVE600_BF_HEADER_VHT_HE_BW_MASK) >> 22);
                // VHT/HE modes: 0=20MHz, 1=40MHz, 2=80MHz, 3=160MHz
                switch (rawBandwidth) {
                case 0:
                    headerInfo.bandwidth = Bandwidth::BANDWIDTH_TWENTY;
                    break;
                case 1:
                    headerInfo.bandwidth = Bandwidth::BANDWIDTH_FOURTY;
                    break;
                case 2:
                    headerInfo.bandwidth = Bandwidth::BANDWIDTH_EIGHTY;
                    break;
                case 3:
                    headerInfo.bandwidth = Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY;
                    break;
                default:
                    return false;
                }
            }
            break;

        case HardwareType::HARDWARE_TYPE_GEN7: // Wave700
            // Wave700: Use BW/RU field from bits 24:21
            rawBandwidth = static_cast<uint8_t>((headerLSB & WAVE700_BF_HEADER_BW_RU_MASK) >> 21);

            switch (rawBandwidth) {
            case 3:
                headerInfo.bandwidth = Bandwidth::BANDWIDTH_TWENTY;
                break;
            case 4:
                headerInfo.bandwidth = Bandwidth::BANDWIDTH_FOURTY;
                break;
            case 5:
                headerInfo.bandwidth = Bandwidth::BANDWIDTH_EIGHTY;
                break;
            case 6:
                headerInfo.bandwidth = Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY;
                break;
            case 7:
                headerInfo.bandwidth = Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY;
                break;
            default:
                return false; // Invalid or unsupported RU value
            }
            break;

        default:
            return false; // Unsupported hardware type
        }

        return true;
    }

    std::vector<uint32_t> readHexStringFile(const std::string& fileName)
    {
        std::ifstream f(fileName, std::ios::in);
        if (!f.is_open()) {
            throw std::invalid_argument("Unable to open file '" + fileName + "' for reading");
        }

        std::vector<uint32_t> values;
        std::string line;

        try {
            while (std::getline(f, line)) {
                // Skip empty lines
                if (line.empty()) {
                    continue;
                }

                // Remove any whitespace
                line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

                // Skip empty lines after whitespace removal
                if (line.empty()) {
                    continue;
                }

                // Each line should contain exactly 8 hex characters (32 bits)
                if (line.length() != 8) {
                    throw std::invalid_argument("Each line must contain exactly 8 hex characters (32 bits)");
                }

                // Validate that all characters are valid hex digits
                if (!std::all_of(line.begin(), line.end(), [](char c) {
                        return std::isxdigit(static_cast<unsigned char>(c));
                    })) {
                    throw std::invalid_argument("Line contains non-hex character: '" + line + "'");
                }

                // Convert hex string directly to uint32_t
                uint32_t value = static_cast<uint32_t>(std::stoul(line, nullptr, 16));
                values.push_back(value);
            }
        } catch (const std::exception& e) {
            f.close();
            throw std::invalid_argument("Unable to parse file '" + fileName + "': " + std::string(e.what()));
        }

        f.close();

        if (f.fail() && (!f.eof())) {
            throw std::invalid_argument("Unable to read file '" + fileName + "'");
        }

        return values;
    }

    BeamformingBandData readBeamformingFilePathSet(const BeamformingFilePathSet_t& fileSet)
    {
        BeamformingBandData data;

        // Validate file set
        if (!isValid(fileSet)) {
            throw std::invalid_argument("Invalid file set: missing required header or values file");
        }

        try {
            // Read required files
            data.header = readHexStringFile(fileSet.headerFile);
            data.values = readHexStringFile(fileSet.valuesFile);

            // Validate header size
            if (data.header.size() != 2) {
                throw std::invalid_argument("Header file '" + std::string(fileSet.headerFile) + "' must contain exactly 2 32-bit values, got " + std::to_string(data.header.size()) + " values");
            }

            // Read optional extended EHT values file
            if (hasExtendedEhtValues(fileSet)) {
                data.extValues = readHexStringFile(fileSet.extValuesEhtFile);
            }

            return data;
        } catch (const std::exception& e) {
            throw std::invalid_argument("Failed to read beamforming file set: " + std::string(e.what()));
        }
    }

    BeamformingData readBeamformingFiles(const BeamformingFilePathSet_t& primarySet,
        const BeamformingFilePathSet_t& secondarySet)
    {
        BeamformingData dataSet;

        try {
            // Read primary file set (always required)
            dataSet.primary = readBeamformingFilePathSet(primarySet);

            // Read secondary file set if provided and valid
            if (isValid(secondarySet)) {
                dataSet.secondary = readBeamformingFilePathSet(secondarySet);
            }

            return dataSet;
        } catch (const std::exception& e) {
            throw std::invalid_argument("Failed to read beamforming files: " + std::string(e.what()));
        }
    }

    bool hasExtendedEhtValues(const BeamformingFilePathSet_t& fileSet)
    {
        return fileSet.extValuesEhtFile != nullptr && fileSet.extValuesEhtFile[0] != '\0';
    }

    bool isValid(const BeamformingFilePathSet_t& fileSet)
    {
        return fileSet.headerFile != nullptr && fileSet.headerFile[0] != '\0' && fileSet.valuesFile != nullptr && fileSet.valuesFile[0] != '\0';
    }

    void writeWave600BeamformingHeader(std::shared_ptr<Client> client, const std::vector<uint32_t>& header)
    {
        Wave600AddressProvider addresses;
        hardware::writeSequentialRegisters<2>(client, header, addresses);
    }

    void writeWave600BeamformingValues(std::shared_ptr<Client> client, const std::vector<uint32_t>& values)
    {
        hardware::writeChunkedMemory<ChipModule::CHIP_MODULE_LMAC_MEM>(
            client, values, beamforming::wave600::valuesBaseAddress);
    }

    void writeWave700BeamformingHeader(std::shared_ptr<Client> client, const std::vector<uint32_t>& header, const Wave700BeamformingAddresses& addresses)
    {
        Wave700AddressProvider addressProvider(addresses);
        hardware::writeSequentialRegisters<2>(client, header, addressProvider);
    }

    void writeWave700BeamformingValues(std::shared_ptr<Client> client, const std::vector<uint32_t>& values, const Wave700BeamformingAddresses& addresses, uint8_t suPage)
    {
        // Configure and modify the control register using generic hardware template
        Wave700ControlRegisterConfig config(suPage);
        hardware::modifyRegister(client, addresses.apbControlAddress, config);

        // Write values using generic chunked memory template
        Wave700AddressProvider addressProvider(addresses);
        hardware::writeChunkedMemory<ChipModule::CHIP_MODULE_LMAC_MEM>(
            client, values, addressProvider.getValuesAddress());
    }

    // ========================================
    // DEBUGGING/LOGGING SUPPORT
    // ========================================

    std::ostream& operator<<(std::ostream& out, const ValidationResult& value)
    {
        out << "{isValid=" << (value.isValid ? "true" : "false") << ",errorMessage=\"" << value.errorMessage << "\"}";
        return out;
    }

} // namespace beamforming_utils
} // namespace dut
