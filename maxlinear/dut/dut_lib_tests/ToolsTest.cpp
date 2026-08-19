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

#include <unordered_map>

#include <gmock/gmock.h>

using ::testing::ThrowsMessage;

namespace {

TEST(ToolsTest, toStringForBandShouldSucceed)
{
    const std::unordered_map<dut::Band, std::string> values {
        { dut::Band::BAND_5000MHZ, "5GHz" },
        { dut::Band::BAND_2400MHZ, "2.4GHz" },
        { dut::Band::BAND_6000MHZ, "6GHz" },
        { dut::Band::BAND_INVALID, "INVALID" }
    };

    for (const auto& entry : values) {
        EXPECT_EQ(dut::toString(entry.first), entry.second);
    }
}

TEST(ToolsTest, toStringForBandwidthShouldSucceed)
{
    const std::unordered_map<dut::Bandwidth, std::string> values {
        { dut::Bandwidth::BANDWIDTH_TWENTY, "20MHz" },
        { dut::Bandwidth::BANDWIDTH_FOURTY, "40MHz" },
        { dut::Bandwidth::BANDWIDTH_EIGHTY, "80MHz" },
        { dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, "160MHz" },
        { dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY, "320MHz" },
        { dut::Bandwidth::BANDWIDTH_INVALID, "INVALID" },
    };

    for (const auto& entry : values) {
        EXPECT_EQ(dut::toString(entry.first), entry.second);
    }
}

TEST(ToolsTest, toStringForCalibrationFileVersionShouldSucceed)
{
    const std::unordered_map<dut::CalibrationFileVersion, std::string> values {
        { dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_4, "4" },
        { dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_5, "5" },
        { dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6, "6" },
        { dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_7, "7" },
        { dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_INVALID, "INVALID" }
    };

    for (const auto& entry : values) {
        EXPECT_EQ(dut::toString(entry.first), entry.second);
    }
}

TEST(ToolsTest, toStringForChipModuleShouldSucceed)
{
    const std::unordered_map<dut::ChipModule, std::string> values {
        { dut::ChipModule::CHIP_MODULE_UMAC_MEM, "UMAC" },
        { dut::ChipModule::CHIP_MODULE_LMAC_MEM, "LMAC" },
        { dut::ChipModule::CHIP_MODULE_PHY, "PHY" },
        { dut::ChipModule::CHIP_MODULE_RF, "RF" },
        { dut::ChipModule::CHIP_MODULE_AFE, "AFE" },
        { dut::ChipModule::CHIP_MODULE_REGISTER, "REGISTER" }
    };

    for (const auto& entry : values) {
        EXPECT_EQ(dut::toString(entry.first), entry.second);
    }
}

TEST(ToolsTest, toStringForFemTypeShouldSucceed)
{
    const std::unordered_map<dut::FemType, std::string> values {
        { dut::FemType::FEM_TYPE_LINEAR, "Linear" },
        { dut::FemType::FEM_TYPE_NON_LINEAR, "Non-Linear" },
        { dut::FemType::FEM_TYPE_INVALID, "Invalid" },
    };

    for (const auto& entry : values) {
        EXPECT_EQ(dut::toString(entry.first), entry.second);
    }
}

TEST(ToolsTest, toStringForGiShouldSucceed)
{
    const std::unordered_map<dut::Gi, std::string> values {
        { dut::Gi::GI_0_4_US, "0.4 us" },
        { dut::Gi::GI_0_8_US, "0.8 us" },
        { dut::Gi::GI_1_6_US, "1.6 us" },
        { dut::Gi::GI_3_2_US, "3.2 us" },
    };

    for (const auto& entry : values) {
        EXPECT_EQ(dut::toString(entry.first), entry.second);
    }
}

TEST(ToolsTest, toStringForLtfShouldSucceed)
{
    const std::unordered_map<dut::Ltf, std::string> values {
        { dut::Ltf::LTF_SHORT, "x1" },
        { dut::Ltf::LTF_MEDIUM, "x2" },
        { dut::Ltf::LTF_LONG, "x4" }
    };

    for (const auto& entry : values) {
        EXPECT_EQ(dut::toString(entry.first), entry.second);
    }
}

TEST(ToolsTest, toStringForMcsShouldSucceed)
{
    const std::unordered_map<dut::Mcs, std::string> values {
        { dut::Mcs::MCS_BPSK_12, "BPSK 1/2" },
        { dut::Mcs::MCS_BPSK_34, "BPSK 3/4" },
        { dut::Mcs::MCS_QPSK_12, "QPSK 1/2" },
        { dut::Mcs::MCS_QPSK_34, "QPSK 3/4" },
        { dut::Mcs::MCS_16QAM_12, "16-QAM 1/2" },
        { dut::Mcs::MCS_16QAM_34, "16-QAM 3/4" },
        { dut::Mcs::MCS_64QAM_23, "64-QAM 2/3" },
        { dut::Mcs::MCS_64QAM_34, "64-QAM 3/4" },
        { dut::Mcs::MCS_64QAM_56, "64-QAM 5/6" },
        { dut::Mcs::MCS_256QAM_34, "256-QAM 3/4" },
        { dut::Mcs::MCS_256QAM_56, "256-QAM 5/6" },
        { dut::Mcs::MCS_1024QAM_34, "1024-QAM 3/4" },
        { dut::Mcs::MCS_1024QAM_56, "1024-QAM 5/6" },
        { dut::Mcs::MCS_4096QAM_34, "4096-QAM 3/4" },
        { dut::Mcs::MCS_4096QAM_56, "4096-QAM 5/6" },
        { dut::Mcs::MCS_80211b_1MBPS_SHORT, "802.11b 1Mbps Short Preamble" },
        { dut::Mcs::MCS_80211b_2MBPS_SHORT, "802.11b 2Mbps Short Preamble" },
        { dut::Mcs::MCS_80211b_5MBPS_SHORT, "802.11b 5Mbps Short Preamble" },
        { dut::Mcs::MCS_80211b_11MBPS_SHORT, "802.11b 11Mbps Short Preamble" },
        { dut::Mcs::MCS_80211b_1MBPS_LONG, "802.11b 1Mbps Long Preamble" },
        { dut::Mcs::MCS_80211b_2MBPS_LONG, "802.11b 2Mbps Long Preamble" },
        { dut::Mcs::MCS_80211b_5MBPS_LONG, "802.11b 5Mbps Long Preamble" },
        { dut::Mcs::MCS_80211b_11MBPS_LONG, "802.11b 11Mbps Long Preamble" }
    };

    for (const auto& entry : values) {
        EXPECT_EQ(dut::toString(entry.first), entry.second);
    }
}

TEST(ToolsTest, toStringForNvMemorySizeShouldSucceed)
{
    const std::unordered_map<dut::NvMemorySize, std::string> values {
        { dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, "1K" },
        { dut::NvMemorySize::MEMORY_SIZE_BYTES_2K, "2K" },
        { dut::NvMemorySize::MEMORY_SIZE_BYTES_3K, "3K" },
        { dut::NvMemorySize::MEMORY_SIZE_INVALID, "INVALID" }
    };

    for (const auto& entry : values) {
        EXPECT_EQ(dut::toString(entry.first), entry.second);
    }
}

TEST(ToolsTest, toStringForNvMemoryTypeeShouldSucceed)
{
    const std::unordered_map<dut::NvMemoryType, std::string> values {
        { dut::NvMemoryType::MEMORY_TYPE_EEPROM, "EEPROM" },
        { dut::NvMemoryType::MEMORY_TYPE_FLASH, "Flash" },
        { dut::NvMemoryType::MEMORY_TYPE_EFUSE, "EFuse" }
    };

    for (const auto& entry : values) {
        EXPECT_EQ(dut::toString(entry.first), entry.second);
    }
}

TEST(ToolsTest, toStringForPhyModeShouldSucceed)
{
    const std::unordered_map<dut::PhyMode, std::string> values {
        { dut::PhyMode::PHY_MODE_A, "A" },
        { dut::PhyMode::PHY_MODE_B, "B" },
        { dut::PhyMode::PHY_MODE_G, "G" },
        { dut::PhyMode::PHY_MODE_N_5, "N_5" },
        { dut::PhyMode::PHY_MODE_N_2_4, "N_2_4" },
        { dut::PhyMode::PHY_MODE_AC, "AC" },
        { dut::PhyMode::PHY_MODE_AX, "AX" },
        { dut::PhyMode::PHY_MODE_BE, "BE" },
    };

    for (const auto& entry : values) {
        EXPECT_EQ(dut::toString(entry.first), entry.second);
    }
}

TEST(ToolsTest, toStringForRegulationTypeShouldSucceed)
{
    const std::unordered_map<dut::RegulationType, std::string> values {
        { dut::RegulationType::REGULATION_TYPE_UNKNOWN, "Unknown" },
        { dut::RegulationType::REGULATION_TYPE_FCC_SP, "FCC_SP" },
        { dut::RegulationType::REGULATION_TYPE_FCC_LPI, "FCC_LPI" },
        { dut::RegulationType::REGULATION_TYPE_DOC, "DOC" },
        { dut::RegulationType::REGULATION_TYPE_ETSI, "ETSI" },
        { dut::RegulationType::REGULATION_TYPE_SPAIN, "Spain" },
        { dut::RegulationType::REGULATION_TYPE_FRANCE, "France" },
        { dut::RegulationType::REGULATION_TYPE_UAE, "UAE" },
        { dut::RegulationType::REGULATION_TYPE_GERMANY, "Germany" },
        { dut::RegulationType::REGULATION_TYPE_MKK, "MKK" },
        { dut::RegulationType::REGULATION_TYPE_ISRAEL, "Israel" },
        { dut::RegulationType::REGULATION_TYPE_SINGAPORE, "Singapore" },
        { dut::RegulationType::REGULATION_TYPE_AUSTRALIA, "Australia" },
        { dut::RegulationType::REGULATION_TYPE_BRAZIL, "Brazil" },
        { dut::RegulationType::REGULATION_TYPE_CHINA, "China" },
        { dut::RegulationType::REGULATION_TYPE_KOREA, "Korea" },
        { dut::RegulationType::REGULATION_TYPE_APAC, "APAC" },
        { dut::RegulationType::REGULATION_TYPE_JAPAN, "Japan" },
    };

    for (const auto& entry : values) {
        EXPECT_EQ(dut::toString(entry.first), entry.second);
    }
}

TEST(ToolsTest, toStringForVersionedComponentShouldSucceed)
{
    const std::unordered_map<dut::VersionedComponent, std::string> values {
        { dut::VersionedComponent::VERSIONED_COMPONENT_CV, "CV" },
        { dut::VersionedComponent::VERSIONED_COMPONENT_PSD, "PSD" },
        { dut::VersionedComponent::VERSIONED_COMPONENT_REGULATORY, "REGULATORY" },
        { dut::VersionedComponent::VERSIONED_COMPONENT_500B_PROGMODEL, "500B_PROGMODEL" },
        { dut::VersionedComponent::VERSIONED_COMPONENT_600_PROGMODEL, "600_PROGMODEL" },
        { dut::VersionedComponent::VERSIONED_COMPONENT_600B_PROGMODEL, "600B_PROGMODEL" },
        { dut::VersionedComponent::VERSIONED_COMPONENT_600D2_PROGMODEL, "600D2_PROGMODEL" },
        { dut::VersionedComponent::VERSIONED_COMPONENT_700_PROGMODEL, "700_PROGMODEL" },
        { dut::VersionedComponent::VERSIONED_COMPONENT_700B_PROGMODEL, "700B_PROGMODEL" }
    };

    for (const auto& entry : values) {
        EXPECT_EQ(dut::toString(entry.first), entry.second);
    }
}

TEST(ToolsTest, stringToBytesShouldSucceed)
{
    std::string hex = " BEEF\nCAFE \nA5A5A5A5A";

    auto bytes = dut::toBytes(hex);

    std::vector<uint8_t> expectedBytes { 0xbe, 0xef, 0xca, 0xfe, 0xa5, 0xa5, 0xa5, 0xa5, 0xA0 };
    ASSERT_EQ(bytes.size(), expectedBytes.size());
    for (size_t i = 0; i < bytes.size(); i++) {
        EXPECT_EQ(bytes[i], expectedBytes[i]) << "Byte at #" + dut::toString(i) << "does not match";
    }
}

TEST(ToolsTest, stringToBytesShouldFail)
{
    std::string hex = "Hello, World!";

    const char* expectedMessage = "Unable to convert string to bytes";
    EXPECT_THAT([&]() { dut::toBytes(hex); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST(ToolsTest, bufferToStringShouldSucceed)
{
    uint8_t buffer[] { 0x00, 0xbe, 0xef };
    size_t length = sizeof(buffer);

    std::string s = dut::toString(buffer, length);

    EXPECT_EQ(s, "00 be ef");
}

TEST(ToolsTest, bufferToStringShouldSucceedWithName)
{
    uint8_t buffer[] { 0x00, 0xbe, 0xef };
    size_t length = sizeof(buffer);
    const char* name = "name";

    std::string s = dut::toString(buffer, length, name);

    EXPECT_EQ(s, "name (3): 00 be ef");
}

TEST(ToolsTest, pairsToStringShouldSucceed)
{
    std::vector<std::pair<std::string, std::string>> pairs { { "a", "A" }, { "b", "B" }, { "c", "C" } };

    std::string s = dut::toString(pairs);

    EXPECT_EQ(s, "a:A;b:B;c:C");
}

} // namespace
