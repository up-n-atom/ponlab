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

#include "dut/Tools.h"

#include <algorithm>

namespace dut {

std::ostream& operator<<(std::ostream& out, const Band& value)
{
    switch (value) {
    case Band::BAND_5000MHZ:
        out << "5GHz";
        break;
    case Band::BAND_2400MHZ:
        out << "2.4GHz";
        break;
    case Band::BAND_6000MHZ:
        out << "6GHz";
        break;
    case Band::BAND_INVALID:
        out << "INVALID";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Bandwidth& value)
{
    switch (value) {
    case Bandwidth::BANDWIDTH_TWENTY:
        out << "20MHz";
        break;
    case Bandwidth::BANDWIDTH_FOURTY:
        out << "40MHz";
        break;
    case Bandwidth::BANDWIDTH_EIGHTY:
        out << "80MHz";
        break;
    case Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY:
        out << "160MHz";
        break;
    case Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY:
        out << "320MHz";
        break;
    case Bandwidth::BANDWIDTH_INVALID:
        out << "INVALID";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const CalibrationFileVersion& value)
{
    switch (value) {
    case CalibrationFileVersion::CALIBRATION_FILE_VERSION_4:
        out << "4";
        break;
    case CalibrationFileVersion::CALIBRATION_FILE_VERSION_5:
        out << "5";
        break;
    case CalibrationFileVersion::CALIBRATION_FILE_VERSION_6:
        out << "6";
        break;
    case CalibrationFileVersion::CALIBRATION_FILE_VERSION_7:
        out << "7";
        break;
    case CalibrationFileVersion::CALIBRATION_FILE_VERSION_INVALID:
        out << "INVALID";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const CalibrationFileSubversion& value)
{
    out << toString(static_cast<uint8_t>(value));
    return out;
}

std::ostream& operator<<(std::ostream& out, const ChipModule& value)
{
    switch (value) {
    case ChipModule::CHIP_MODULE_UMAC_MEM:
        out << "UMAC";
        break;
    case ChipModule::CHIP_MODULE_LMAC_MEM:
        out << "LMAC";
        break;
    case ChipModule::CHIP_MODULE_PHY:
        out << "PHY";
        break;
    case ChipModule::CHIP_MODULE_RF:
        out << "RF";
        break;
    case ChipModule::CHIP_MODULE_AFE:
        out << "AFE";
        break;
    case ChipModule::CHIP_MODULE_REGISTER:
        out << "REGISTER";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const FemType& value)
{
    switch (value) {
    case FemType::FEM_TYPE_LINEAR:
        out << "Linear";
        break;
    case FemType::FEM_TYPE_NON_LINEAR:
        out << "Non-Linear";
        break;
    case FemType::FEM_TYPE_INVALID:
        out << "Invalid";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Gi& value)
{
    switch (value) {
    case Gi::GI_0_4_US:
        out << "0.4 us";
        break;
    case Gi::GI_0_8_US:
        out << "0.8 us";
        break;
    case Gi::GI_1_6_US:
        out << "1.6 us";
        break;
    case Gi::GI_3_2_US:
        out << "3.2 us";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Ltf& value)
{
    switch (value) {
    case Ltf::LTF_SHORT:
        out << "x1";
        break;
    case Ltf::LTF_MEDIUM:
        out << "x2";
        break;
    case Ltf::LTF_LONG:
        out << "x4";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Mcs& value)
{
    switch (value) {
    case Mcs::MCS_BPSK_12:
        out << "BPSK 1/2";
        break;
    case Mcs::MCS_BPSK_34:
        out << "BPSK 3/4";
        break;
    case Mcs::MCS_QPSK_12:
        out << "QPSK 1/2";
        break;
    case Mcs::MCS_QPSK_34:
        out << "QPSK 3/4";
        break;
    case Mcs::MCS_16QAM_12:
        out << "16-QAM 1/2";
        break;
    case Mcs::MCS_16QAM_34:
        out << "16-QAM 3/4";
        break;
    case Mcs::MCS_64QAM_23:
        out << "64-QAM 2/3";
        break;
    case Mcs::MCS_64QAM_34:
        out << "64-QAM 3/4";
        break;
    case Mcs::MCS_64QAM_56:
        out << "64-QAM 5/6";
        break;
    case Mcs::MCS_256QAM_34:
        out << "256-QAM 3/4";
        break;
    case Mcs::MCS_256QAM_56:
        out << "256-QAM 5/6";
        break;
    case Mcs::MCS_1024QAM_34:
        out << "1024-QAM 3/4";
        break;
    case Mcs::MCS_1024QAM_56:
        out << "1024-QAM 5/6";
        break;

    case Mcs::MCS_4096QAM_34:
        out << "4096-QAM 3/4";
        break;
    case Mcs::MCS_4096QAM_56:
        out << "4096-QAM 5/6";
        break;

    case Mcs::MCS_80211b_1MBPS_SHORT:
        out << "802.11b 1Mbps Short Preamble";
        break;
    case Mcs::MCS_80211b_2MBPS_SHORT:
        out << "802.11b 2Mbps Short Preamble";
        break;
    case Mcs::MCS_80211b_5MBPS_SHORT:
        out << "802.11b 5Mbps Short Preamble";
        break;
    case Mcs::MCS_80211b_11MBPS_SHORT:
        out << "802.11b 11Mbps Short Preamble";
        break;
    case Mcs::MCS_80211b_1MBPS_LONG:
        out << "802.11b 1Mbps Long Preamble";
        break;
    case Mcs::MCS_80211b_2MBPS_LONG:
        out << "802.11b 2Mbps Long Preamble";
        break;
    case Mcs::MCS_80211b_5MBPS_LONG:
        out << "802.11b 5Mbps Long Preamble";
        break;
    case Mcs::MCS_80211b_11MBPS_LONG:
        out << "802.11b 11Mbps Long Preamble";
        break;
    default:
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const NvMemorySize& value)
{
    switch (value) {
    case NvMemorySize::MEMORY_SIZE_BYTES_1K:
        out << "1K";
        break;
    case NvMemorySize::MEMORY_SIZE_BYTES_2K:
        out << "2K";
        break;
    case NvMemorySize::MEMORY_SIZE_BYTES_3K:
        out << "3K";
        break;
    case NvMemorySize::MEMORY_SIZE_INVALID:
        out << "INVALID";
        break;
    default:
        out << std::to_string(static_cast<int32_t>(value));
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const NvMemoryType& value)
{
    switch (value) {
    case NvMemoryType::MEMORY_TYPE_EEPROM:
        out << "EEPROM";
        break;
    case NvMemoryType::MEMORY_TYPE_FLASH:
        out << "Flash";
        break;
    case NvMemoryType::MEMORY_TYPE_EFUSE:
        out << "EFuse";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const PhyMode& value)
{
    switch (value) {
    case PhyMode::PHY_MODE_A:
        out << "A";
        break;
    case PhyMode::PHY_MODE_B:
        out << "B";
        break;
    case PhyMode::PHY_MODE_G:
        out << "G";
        break;
    case PhyMode::PHY_MODE_N_5:
        out << "N_5";
        break;
    case PhyMode::PHY_MODE_N_2_4:
        out << "N_2_4";
        break;
    case PhyMode::PHY_MODE_AC:
        out << "AC";
        break;
    case PhyMode::PHY_MODE_AX:
        out << "AX";
        break;
    case PhyMode::PHY_MODE_BE:
        out << "BE";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const RegulationType& value)
{
    switch (value) {
    case RegulationType::REGULATION_TYPE_UNKNOWN:
        out << "Unknown";
        break;
    case RegulationType::REGULATION_TYPE_FCC_SP:
        out << "FCC_SP";
        break;
    case RegulationType::REGULATION_TYPE_FCC_LPI:
        out << "FCC_LPI";
        break;
    case RegulationType::REGULATION_TYPE_DOC:
        out << "DOC";
        break;
    case RegulationType::REGULATION_TYPE_ETSI:
        out << "ETSI";
        break;
    case RegulationType::REGULATION_TYPE_SPAIN:
        out << "Spain";
        break;
    case RegulationType::REGULATION_TYPE_FRANCE:
        out << "France";
        break;
    case RegulationType::REGULATION_TYPE_UAE:
        out << "UAE";
        break;
    case RegulationType::REGULATION_TYPE_GERMANY:
        out << "Germany";
        break;
    case RegulationType::REGULATION_TYPE_MKK:
        out << "MKK";
        break;
    case RegulationType::REGULATION_TYPE_ISRAEL:
        out << "Israel";
        break;
    case RegulationType::REGULATION_TYPE_SINGAPORE:
        out << "Singapore";
        break;
    case RegulationType::REGULATION_TYPE_AUSTRALIA:
        out << "Australia";
        break;
    case RegulationType::REGULATION_TYPE_BRAZIL:
        out << "Brazil";
        break;
    case RegulationType::REGULATION_TYPE_CHINA:
        out << "China";
        break;
    case RegulationType::REGULATION_TYPE_KOREA:
        out << "Korea";
        break;
    case RegulationType::REGULATION_TYPE_APAC:
        out << "APAC";
        break;
    case RegulationType::REGULATION_TYPE_JAPAN:
        out << "Japan";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const VersionedComponent& value)
{
    switch (value) {
    case VersionedComponent::VERSIONED_COMPONENT_CV:
        out << "CV";
        break;
    case VersionedComponent::VERSIONED_COMPONENT_PSD:
        out << "PSD";
        break;
    case VersionedComponent::VERSIONED_COMPONENT_REGULATORY:
        out << "REGULATORY";
        break;
    case VersionedComponent::VERSIONED_COMPONENT_500B_PROGMODEL:
        out << "500B_PROGMODEL";
        break;
    case VersionedComponent::VERSIONED_COMPONENT_600_PROGMODEL:
        out << "600_PROGMODEL";
        break;
    case VersionedComponent::VERSIONED_COMPONENT_600B_PROGMODEL:
        out << "600B_PROGMODEL";
        break;
    case VersionedComponent::VERSIONED_COMPONENT_600D2_PROGMODEL:
        out << "600D2_PROGMODEL";
        break;
    case VersionedComponent::VERSIONED_COMPONENT_700_PROGMODEL:
        out << "700_PROGMODEL";
        break;
    case VersionedComponent::VERSIONED_COMPONENT_700B_PROGMODEL:
        out << "700B_PROGMODEL";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const RssiAB_t& value)
{
    out << "{a=" << value.a << ",b=" << value.b << "}";
    return out;
}

std::ostream& operator<<(std::ostream& out, const CorrelationResults_t& value)
{
    out << "{II=" << value.II << ",QQ=" << value.QQ << ",IQ=" << value.IQ << "}";
    return out;
}

std::ostream& operator<<(std::ostream& out, const BeamformingHeaderInfo_t& value)
{
    out << "{phyMode=" << value.phyMode << ",bandwidth=" << value.bandwidth << "}";
    return out;
}

std::ostream& operator<<(std::ostream& out, const BeamformingFilePathSet_t& value)
{
    out << "{headerFile=\"" << value.headerFile << "\",valuesFile=\"" << value.valuesFile 
        << "\",extValuesEhtFile=\"" << value.extValuesEhtFile << "\"}";
    return out;
}

std::ostream& operator<<(std::ostream& stream, uint8_t byte)
{
    stream << std::setfill('0') << std::setw(2) << std::hex << +byte;
    return stream;
}

std::vector<uint8_t> toBytes(const std::string& hex)
{
    std::vector<uint8_t> bytes;

    std::string sanitizedString = hex;
    const std::vector<char> charactersToRemove { ' ', '\n', '\r', '\t' };
    for (char characterToRemove : charactersToRemove) {
        sanitizedString.erase(std::remove(sanitizedString.begin(), sanitizedString.end(), characterToRemove), sanitizedString.end());
    }

    if (sanitizedString.length() % 2) {
        sanitizedString += "0";
    }

    std::transform(sanitizedString.begin(), sanitizedString.end(), sanitizedString.begin(), [](unsigned char c) { return std::tolower(c); });

    for (size_t i = 0; i < sanitizedString.length(); i += 2) {
        std::string byteString = sanitizedString.substr(i, 2);
        char* endptr = nullptr;
        long value = strtol(byteString.c_str(), &endptr, 16);
        if (endptr == byteString.c_str()) {
            throw std::invalid_argument("Unable to convert string to bytes");
        }
        bytes.push_back(static_cast<uint8_t>(value));
    }

    return bytes;
}

std::string toHexString(uint8_t byte)
{
    std::stringstream stream;
    stream << "0x" << byte;
    return stream.str();
}

std::string toString(bool value)
{
    std::stringstream stream;
    stream << (value ? "true" : "false");

    return stream.str();
}

std::string toString(int8_t byte)
{
    std::stringstream stream;
    stream << +byte;

    return stream.str();
}

std::string toString(uint8_t byte)
{
    std::stringstream stream;
    stream << +byte;

    return stream.str();
}

std::string toString(float value, uint8_t precision)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;

    return stream.str();
}

std::string toString(const uint8_t* data, size_t length, const char* name)
{
    std::stringstream stream;

    if (name) {
        stream << name << " (" << toString(length) << "): ";
    }

    for (size_t i = 0; i < length; i++) {
        if (i > 0) {
            stream << " ";
        }
        stream << data[i];
    }

    return stream.str();
}

std::string toString(const std::vector<std::pair<std::string, std::string>>& pairs, const std::string& pairsSeparator, const std::string& valuesSeparator)
{
    std::string s;

    for (const auto& pair : pairs) {
        if (!s.empty()) {
            s += pairsSeparator;
        }
        s += pair.first;
        s += valuesSeparator;
        s += pair.second;
    }

    return s;
}

std::string toString(const ChipID& value)
{
    return toHexString(static_cast<uint16_t>(value));
}

// Helper function - round double to 16-bit signed number
uint16_t lq_round(double num)
{
    if (num > 0) {
        return static_cast<uint16_t>(num + 0.5);
    } else {
        return static_cast<uint16_t>(num - 0.5);
    }
}

}