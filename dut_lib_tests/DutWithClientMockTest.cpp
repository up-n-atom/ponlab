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

#include "BeamformingTestHelpers.h"
#include "BeamformingUtils.h"
#include "dut/DutImpl.h"

#include "CmdLineArgs.h"
#include "EmbeddedResource.h"
#include "NonVolatileMemory.h"
#include "Rates.h"
#include "ResizableBuffer.h"
#include "SharedHeaders.h"
#include "TemporaryFile.h"
#include "dut/ClientMock.h"
#include "dut/Tools.h"
#include "dut/WindowsConsoleLogger.h"
#include "resource.h"

#include <fstream>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::StrictMock;

namespace {

constexpr bool snifferMode = false;
constexpr dut::NvMemoryType memoryType = dut::NvMemoryType::MEMORY_TYPE_FLASH;
constexpr dut::NvMemorySize memorySize = dut::NvMemorySize::MEMORY_SIZE_BYTES_1K;

const dut::AntennaMask rxAntennaMask = 0x1f;
const dut::AntennaMask rxAntennaMaskGen7 = 0x0f;
const dut::AntennaMask txAntennaMask = 0x0f;
const dut::AntennaMask zwdfsAntennaMask = 0x10;

const uint8_t supportedBandsMaskFor2400MhzAnd5000Mhz = 0x01 | 0x02;

struct GetPacketLengthTestData_t {
    dut::PhyMode phyMode;
    dut::Bandwidth signalBandwidth;
    dut::Mcs mcs;
    uint8_t nss;
    uint32_t numSymbols;
    uint32_t expectedPacketLength;
};

struct GetMaxPacketLengthTestData_t {
    dut::PhyMode phyMode;
    uint32_t expectedMaxPacketLength;
};

struct GetRateTestData_t {
    dut::PhyMode phyMode;
    dut::Bandwidth signalBandwidth;
    dut::Mcs mcs;
    uint8_t nss;
    dut::Gi gi;
    float expectedRateMbps;
};

/*
 * These tests use a client mock instead of a connection mock, so they are easier to implement
 * because there's no need to specify request-response frames in the expectations but just method
 * calls (those of the client mock).
 * These tests are used to test sad paths, corner cases and boundary cases.
 */
class DutWithClientMockTest : public ::testing::Test {
public:
    explicit DutWithClientMockTest(dut::ChipID chipId = dut::ChipID::CHIP_ID_GEN6_B)
        : m_chipId(chipId)
        , m_dut(m_client, m_logger)
    {
    }

    void SetUp() override
    {
        {
            InSequence sequence;

            EXPECT_CALL(*m_client, getWlanIndex()).WillOnce(Return(0));
            EXPECT_CALL(*m_client, resetMac(memoryType, memorySize, true));
            EXPECT_CALL(*m_client, getFwInfo(_)).WillOnce(Invoke([this](dutGetFwInfo_t& fwInfo) {
                fwInfo.operationalRxAntMask = (m_chipId < dut::ChipID::CHIP_ID_GEN7) ? rxAntennaMask : rxAntennaMaskGen7;
                fwInfo.operationalTxAntMask = txAntennaMask;
                fwInfo.psdMaxRxAntMask = (m_chipId < dut::ChipID::CHIP_ID_GEN7) ? rxAntennaMask : rxAntennaMaskGen7;
                fwInfo.psdMaxTxAntMask = txAntennaMask;
                fwInfo.zwdfsAntMask = (m_chipId < dut::ChipID::CHIP_ID_GEN7) ? 0x00 : zwdfsAntennaMask;
                fwInfo.currentRfBandPsd = dutBand_e::DUT_BAND_5000MHZ;
                fwInfo.supportedBandsPsd = supportedBandsMaskFor2400MhzAnd5000Mhz;
            }));
            EXPECT_CALL(*m_client, removeVap(0));
            EXPECT_CALL(*m_client, addVap(0, snifferMode));
            EXPECT_CALL(*m_client, readNvMemory(_, _, _, memoryType, memorySize, dut::FileType::FILE_TYPE_CALIBRATION)).WillRepeatedly(Invoke([this](size_t address, uint8_t* data, size_t length, dut::NvMemoryType, dut::NvMemorySize, dut::FileType) {
                memcpy(data, &m_calibrationFileResource.getData()[address], length);
            }));
            EXPECT_CALL(*m_client, readChipId(_)).WillOnce(Invoke([this](dut::ChipID& chipId) {
                chipId = m_chipId;
            }));
            EXPECT_CALL(*m_client, getCombinedVersion(_)).WillOnce(Invoke([](std::array<uint8_t, dut::Client::combinedVersionSize>& buffer) {
                constexpr auto combinedVersion = "wave_release_minor=\"06.01.00.1097\"\nwave_psd_ver=\"2021-08-23_15-30-06_REV_3578c07f871c\"\nwave_regulatory_ver=\"2018.04.17_WAVE19-";
                memcpy(buffer.data(), combinedVersion, strlen(combinedVersion) + 1);
            }));
            EXPECT_CALL(*m_client, setHdkConfig(0xffff, 0, dut::Band::BAND_5000MHZ));
            EXPECT_CALL(*m_client, setRxAggregationEnabled(false));
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, stopCw());
            EXPECT_CALL(*m_client, setSpacelessTxEnabled(false));
            EXPECT_CALL(*m_client, stopRxCalibration());
            EXPECT_CALL(*m_client, setRiscModeEnabled(true));
            EXPECT_CALL(*m_client, setIfs(dut::defaultIfs));
            EXPECT_CALL(*m_client, setEnabledRxAntennas(_));
            EXPECT_CALL(*m_client, setEnabledTxAntennas(_));
            EXPECT_CALL(*m_client, setTransmitPowerLevel(0, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
            EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
            EXPECT_CALL(*m_client, setTransmitPowerLevel(2, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
            EXPECT_CALL(*m_client, setTransmitPowerLevel(3, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
            if (m_chipId >= dut::ChipID::CHIP_ID_GEN7) {
                EXPECT_CALL(*m_client, setZwdfsEnabled(false, dut::defaultZwdfsChannel, dut::defaultZwdfsBandwidth, dut::defaultRadarDetectionBandwidth, dut::defaultRegulationType));
            }
        }

        EXPECT_TRUE(m_dut.driverInit(snifferMode, memoryType, memorySize, dut::Band::BAND_INVALID));
    }

    void TearDown() override
    {
        m_logger->setLogLevel(dut::LogLevel::LOG_LEVEL_NONE);

        if (m_tearDown) {
            {
                InSequence sequence;

                EXPECT_CALL(*m_client, removeVap(0));
            }

            EXPECT_TRUE(m_dut.driverRelease());
        }
    }

    void setupChannelAndRate(dut::PhyMode phyMode, dut::Bandwidth spectrumBandwidth, dut::Bandwidth signalBandwidth,
        uint8_t channel = 36, uint8_t primaryChannelIndex = 0,
        dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP,
        dut::Mcs mcs = dut::Mcs::MCS_QPSK_34, uint8_t nss = 1,
        dut::Gi gi = dut::Gi::GI_0_8_US, dut::Ltf ltf = dut::Ltf::LTF_MEDIUM)
    {
        uint8_t antenna = 1;
        dut::AntennaMask enabledTxAntennaMask(1 << antenna);

        dut::AntennaMask enabledRxAntennaMask = 0x00;
        ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

        {
            InSequence sequence;

            EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
            EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
            EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
            EXPECT_CALL(*m_client, setRssiCalData(_, _));
            EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
            EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
            EXPECT_CALL(*m_client, setRate(phyMode, signalBandwidth, _, nss, gi, ltf));
            EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, dut::defaultTransmitPowerLevel));
        }

        float rateMbps = 0.0f;
        ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
        ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
        ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));
    }

    dut::ChipID m_chipId;
    std::shared_ptr<StrictMock<dut::ClientMock>> m_client = std::make_shared<StrictMock<dut::ClientMock>>();
    std::shared_ptr<dut::Logger> m_logger = std::make_shared<dut::WindowsConsoleLogger>(true, g_cmdLineArgs.getLogLevel());
    dut::DutImpl m_dut;

    EmbeddedResource m_calibrationFileResource { IDR_CALIBRATIONFILE1, CALIBRATIONFILE };

    bool m_tearDown = true;

    void calculateMaxPacketLengthTest(const std::vector<GetMaxPacketLengthTestData_t>& testData)
    {
        auto dataToString = [](const GetMaxPacketLengthTestData_t& data) {
            return "[" + dut::toString(data.phyMode) + "]";
        };

        for (const auto& data : testData) {
            uint32_t maxPacketLength = 0;
            if (data.expectedMaxPacketLength == 0) {
                EXPECT_FALSE(m_dut.calculateMaxPacketLength(data.phyMode, maxPacketLength));
            } else {
                EXPECT_TRUE(m_dut.calculateMaxPacketLength(data.phyMode, maxPacketLength));
                EXPECT_EQ(data.expectedMaxPacketLength, maxPacketLength) << dataToString(data);
            }
        }
    }

    void calculatePacketLengthTest(const std::vector<GetPacketLengthTestData_t>& testData)
    {
        auto dataToString = [](const GetPacketLengthTestData_t& data) {
            return "[" + dut::toString(data.phyMode) + "," + dut::toString(data.signalBandwidth) + "," + dut::toString(data.mcs) + "," + dut::toString(data.nss) + "," + dut::toString(data.numSymbols) + "]";
        };

        for (const auto& data : testData) {
            uint32_t packetLength = 0;
            if (data.expectedPacketLength == 0) {
                EXPECT_FALSE(m_dut.calculatePacketLength(data.phyMode, data.signalBandwidth, data.mcs, data.nss, data.numSymbols, packetLength));
            } else {
                EXPECT_TRUE(m_dut.calculatePacketLength(data.phyMode, data.signalBandwidth, data.mcs, data.nss, data.numSymbols, packetLength));
                EXPECT_EQ(data.expectedPacketLength, packetLength) << dataToString(data);
            }
        }
    }

    void calculatePhyDataRateTest(const std::vector<GetRateTestData_t>& testData)
    {
        auto dataToString = [](const GetRateTestData_t& data) {
            return "[" + dut::toString(data.phyMode) + "," + dut::toString(data.signalBandwidth) + "," + dut::toString(data.mcs) + "," + dut::toString(data.nss) + "," + dut::toString(data.gi) + "]";
        };

        for (const auto& data : testData) {
            float rateMbps = 0.0f;
            if (data.expectedRateMbps == 0.0f) {
                EXPECT_FALSE(m_dut.calculatePhyDataRate(data.phyMode, data.signalBandwidth, data.mcs, data.nss, data.gi, rateMbps));
            } else {
                EXPECT_TRUE(m_dut.calculatePhyDataRate(data.phyMode, data.signalBandwidth, data.mcs, data.nss, data.gi, rateMbps));
                EXPECT_EQ(data.expectedRateMbps, rateMbps) << dataToString(data);
            }
        }
    }
};

class DutWithClientMockTestGen7 : public DutWithClientMockTest {
public:
    DutWithClientMockTestGen7()
        : DutWithClientMockTest(dut::ChipID::CHIP_ID_GEN7)
    {
    }

protected:
    // Override setupChannelAndRate for Wave700 hardware matching the actual calling sequence
    void setupChannelAndRate(dut::PhyMode phyMode, dut::Bandwidth spectrumBandwidth, dut::Bandwidth signalBandwidth,
        uint8_t channel = 36, uint8_t primaryChannelIndex = 0,
        dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP,
        dut::Mcs mcs = dut::Mcs::MCS_QPSK_34, uint8_t nss = 1,
        dut::Gi gi = dut::Gi::GI_0_8_US, dut::Ltf ltf = dut::Ltf::LTF_MEDIUM)
    {
        uint8_t antenna = 1;
        dut::AntennaMask enabledTxAntennaMask(1 << antenna);
        dut::AntennaMask enabledRxAntennaMask = 0x00;
        ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

        {
            InSequence sequence;

            EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
            EXPECT_CALL(*m_client, setTransmitPowerLevel(_, _, _)).Times(::testing::AtLeast(1)); // Multiple calls with different params
            EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
            EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
            EXPECT_CALL(*m_client, setRate(phyMode, signalBandwidth, _, _, _, _));
            EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, dut::defaultTransmitPowerLevel));
        }

        ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
        ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

        float rateMbps;
        ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));
    }
};

TEST_F(DutWithClientMockTest, calculateMaxPacketLengthShouldSucceed)
{
    std::vector<GetMaxPacketLengthTestData_t> testData = {
        { dut::PhyMode::PHY_MODE_A, 0xfff },
        { dut::PhyMode::PHY_MODE_G, 0xfff },
        { dut::PhyMode::PHY_MODE_N_2_4, 0xffff },
        { dut::PhyMode::PHY_MODE_N_5, 0xffff },
        { dut::PhyMode::PHY_MODE_AC, 0xfffff },
        { dut::PhyMode::PHY_MODE_AX, 0x3fffff },
        { dut::PhyMode::PHY_MODE_BE, 0x3fffff },
    };

    calculateMaxPacketLengthTest(testData);
}

TEST_F(DutWithClientMockTest, calculatePacketLengthShouldSucceed)
{
    std::vector<GetPacketLengthTestData_t> testData = {
        // 802.11b is not OFDM (there is no number of symbols)
        { dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_12, 1, 20, 0 },
        // 802.11a and 802.11g support 20MHz bandwidth only
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_BPSK_12, 1, 20, 0 },
        { dut::PhyMode::PHY_MODE_G, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_BPSK_12, 1, 20, 0 },
        // 802.11a and 802.11g do not support MIMO so cannot specify more than 1 spatial stream
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_12, 2, 20, 0 },
        { dut::PhyMode::PHY_MODE_G, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_12, 2, 20, 0 },
        // Invalid number of ODFM symbols
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_12, 1, dut::minNumOdfmSymbols - 1, 0 },

        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_12, 1, 100, 297 },
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_64QAM_56, 1, 100, 2997 },
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_64QAM_56, 1, 300, 8997 },
        { dut::PhyMode::PHY_MODE_G, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_64QAM_56, 1, 300, 8997 },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_64QAM_56, 1, 300, 9748 },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_64QAM_56, 1, 300, 20248 },
        { dut::PhyMode::PHY_MODE_N_5, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_64QAM_56, 1, 300, 20248 },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_64QAM_56, 1, 300, 20248 },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_64QAM_56, 4, 120, 70198 },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_64QAM_56, 4, 120, 587998 },
        { dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY, dut::Mcs::MCS_64QAM_56, 4, 320, 3135998 },
    };

    calculatePacketLengthTest(testData);
}

TEST_F(DutWithClientMockTest, calculatePhyDataRateShouldSucceedFor11aAnd11g)
{
    // PHY_MODE_A supports 5GHz frequency band only and 20MHz channel width only
    // PHY_MODE_G supports 2.4GHz frequency band only and 20MHz channel width only
    std::vector<GetRateTestData_t> testData = {
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 6.0f },
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_34, 1, dut::Gi::GI_0_8_US, 9.0f },
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_QPSK_12, 1, dut::Gi::GI_0_8_US, 12.0f },
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_8_US, 18.0f },
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_16QAM_12, 1, dut::Gi::GI_0_8_US, 24.0f },
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_16QAM_34, 1, dut::Gi::GI_0_8_US, 36.0f },
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_64QAM_23, 1, dut::Gi::GI_0_8_US, 48.0f },
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_64QAM_34, 1, dut::Gi::GI_0_8_US, 54.0f },

        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_64QAM_34, 1, dut::Gi::GI_0_8_US, 0.0f },
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_64QAM_34, 1, dut::Gi::GI_0_8_US, 0.0f },
        { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_64QAM_34, 1, dut::Gi::GI_0_8_US, 0.0f },
    };

    for (auto phyMode : { dut::PhyMode::PHY_MODE_A, dut::PhyMode::PHY_MODE_G }) {
        for (auto& data : testData) {
            data.phyMode = phyMode;
        }
        calculatePhyDataRateTest(testData);
    }
}

TEST_F(DutWithClientMockTest, calculatePhyDataRateShouldSucceedFor11b)
{
    // PHY_MODE_B supports 2.4GHz frequency band only and 20MHz channel width only
    // Channel width, number of spatial streams and guard interval are all ignored when computing data rate for 11b
    // MCS_80211b_1MBPS_SHORT is not legal
    std::vector<GetRateTestData_t> testData = {
        { dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_80211b_1MBPS_SHORT, 1, dut::Gi::GI_0_8_US, 0.0f },
        { dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_80211b_2MBPS_SHORT, 1, dut::Gi::GI_0_8_US, 16.0f },
        { dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_80211b_5MBPS_SHORT, 1, dut::Gi::GI_0_8_US, 44.0f },
        { dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_80211b_11MBPS_SHORT, 1, dut::Gi::GI_0_8_US, 88.0f },
        { dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_80211b_1MBPS_LONG, 1, dut::Gi::GI_0_8_US, 8.0f },
        { dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_80211b_2MBPS_LONG, 1, dut::Gi::GI_0_8_US, 16.0f },
        { dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_80211b_5MBPS_LONG, 1, dut::Gi::GI_0_8_US, 48.0f },
        { dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_80211b_11MBPS_LONG, 1, dut::Gi::GI_0_8_US, 88.0f },

        { dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_80211b_11MBPS_LONG, 1, dut::Gi::GI_0_8_US, 0.0f },
        { dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_80211b_11MBPS_LONG, 1, dut::Gi::GI_0_8_US, 0.0f },
        { dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_80211b_11MBPS_LONG, 1, dut::Gi::GI_0_8_US, 0.0f },
    };

    calculatePhyDataRateTest(testData);
}

TEST_F(DutWithClientMockTest, calculatePhyDataRateShouldSucceedFor11n)
{
    // https://mcsindex.com/
    std::vector<GetRateTestData_t> testData = {
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 6.5f },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_4_US, 7.2f },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 13.5f },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_4_US, 15.0f },

        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_8_US, 19.5f },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_4_US, 21.7f },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_8_US, 40.5f },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_4_US, 45.0f },

        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_34, 1, dut::Gi::GI_0_8_US, 0.0f },

        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_4096QAM_34, 1, dut::Gi::GI_0_8_US, 0.0f },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_4096QAM_56, 1, dut::Gi::GI_0_8_US, 0.0f },

        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_256QAM_34, 1, dut::Gi::GI_0_8_US, 0.0f },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_256QAM_34, 1, dut::Gi::GI_0_4_US, 0.0f },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_8_US, 0.0f },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_4_US, 0.0f },

        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_256QAM_34, 1, dut::Gi::GI_0_8_US, 0.0f },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_256QAM_34, 1, dut::Gi::GI_0_4_US, 0.0f },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_8_US, 0.0f },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_4_US, 0.0f },

        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 0.0f },
        { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 0.0f },
    };

    for (auto phyMode : { dut::PhyMode::PHY_MODE_N_2_4, dut::PhyMode::PHY_MODE_N_5 }) {
        for (auto& data : testData) {
            data.phyMode = phyMode;
        }
        calculatePhyDataRateTest(testData);
    }
}

TEST_F(DutWithClientMockTest, calculatePhyDataRateShouldSucceedFor11ac)
{
    // https://mcsindex.com/
    std::vector<GetRateTestData_t> testData = {
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 6.5f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_4_US, 7.2f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 13.5f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_4_US, 15.0f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 29.3f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_4_US, 32.5f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 58.5f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_4_US, 65.0f },

        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_8_US, 19.5f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_4_US, 21.7f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_8_US, 40.5f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_4_US, 45.0f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_8_US, 87.8f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_4_US, 97.5f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_8_US, 175.5f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_4_US, 195.0f },

        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_8_US, 0.0f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_4_US, 0.0f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_8_US, 180.0f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_4_US, 200.0f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_8_US, 390.0f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_4_US, 433.3f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_8_US, 780.0f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_4_US, 866.7f },

        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_34, 1, dut::Gi::GI_0_8_US, 0.0f },

        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_1024QAM_34, 1, dut::Gi::GI_0_8_US, 0.0f },

        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_4096QAM_34, 1, dut::Gi::GI_0_8_US, 0.0f },
        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_4096QAM_56, 1, dut::Gi::GI_0_8_US, 0.0f },

        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_64QAM_34, 3, dut::Gi::GI_0_8_US, 0.0f },

        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_256QAM_56, 3, dut::Gi::GI_0_8_US, 0.0f },

        { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 0.0f },
    };

    calculatePhyDataRateTest(testData);
}

TEST_F(DutWithClientMockTest, calculatePhyDataRateShouldSucceedFor11ax)
{
    // https://mcsindex.com/
    // Note: some of the values provided in MCS index table in section OFDM (802.11ax) are close but do not exactly match computed value
    std::vector<GetRateTestData_t> testData = {
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 8.6f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_1_6_US, 8.1f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_3_2_US, 7.3f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 17.2f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_1_6_US, 16.3f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_3_2_US, 14.6f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 36.0f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_1_6_US, 34.0f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_3_2_US, 30.6f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 72.1f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_1_6_US, 68.1f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_3_2_US, 61.3f },

        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_8_US, 25.8f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_1_6_US, 24.4f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_3_2_US, 21.9f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_8_US, 51.6f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_1_6_US, 48.7f /*48.8f*/ },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_3_2_US, 43.9f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_8_US, 108.1f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_1_6_US, 102.1f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_3_2_US, 91.9f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_8_US, 216.2f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_1_6_US, 204.2f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_3_2_US, 183.8f },

        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_8_US, 114.7f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_1_6_US, 108.3f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_3_2_US, 97.5f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_8_US, 229.4f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_1_6_US, 216.7f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_3_2_US, 195.0f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_8_US, 480.4f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_1_6_US, 453.7f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_3_2_US, 408.3f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_8_US, 960.8f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_1_6_US, 907.4f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_3_2_US, 816.7f },

        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_1024QAM_56, 1, dut::Gi::GI_0_8_US, 143.4f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_1024QAM_56, 1, dut::Gi::GI_1_6_US, 135.4f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_1024QAM_56, 1, dut::Gi::GI_3_2_US, 121.9f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_1024QAM_56, 1, dut::Gi::GI_0_8_US, 286.8f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_1024QAM_56, 1, dut::Gi::GI_1_6_US, 270.8f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_1024QAM_56, 1, dut::Gi::GI_3_2_US, 243.8f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_1024QAM_56, 1, dut::Gi::GI_0_8_US, 600.5f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_1024QAM_56, 1, dut::Gi::GI_1_6_US, 567.1f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_1024QAM_56, 1, dut::Gi::GI_3_2_US, 510.4f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_1024QAM_56, 1, dut::Gi::GI_0_8_US, 1201.0f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_1024QAM_56, 1, dut::Gi::GI_1_6_US, 1134.3f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_1024QAM_56, 1, dut::Gi::GI_3_2_US, 1020.8f },

        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_BPSK_34, 1, dut::Gi::GI_0_8_US, 0.0f },

        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_4096QAM_34, 1, dut::Gi::GI_0_8_US, 0.0f },
        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_4096QAM_56, 1, dut::Gi::GI_0_8_US, 0.0f },

        { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY, dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, 0.0f },
    };

    calculatePhyDataRateTest(testData);
}

TEST_F(DutWithClientMockTest, calculatePhyDataRateShouldSucceedFor11be)
{
    std::vector<GetRateTestData_t> testData = {
        { dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_4096QAM_34, 1, dut::Gi::GI_0_8_US, 154.9f },
        { dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_4096QAM_56, 1, dut::Gi::GI_0_8_US, 172.1f },
        { dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_4096QAM_34, 1, dut::Gi::GI_0_8_US, 309.7f },
        { dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Mcs::MCS_4096QAM_56, 1, dut::Gi::GI_0_8_US, 344.1f },
        { dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_4096QAM_34, 1, dut::Gi::GI_0_8_US, 648.5f },
        { dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Mcs::MCS_4096QAM_56, 1, dut::Gi::GI_0_8_US, 720.6f },
        { dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_4096QAM_34, 1, dut::Gi::GI_0_8_US, 1297.1F },
        { dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Mcs::MCS_4096QAM_56, 1, dut::Gi::GI_0_8_US, 1441.2f },
        { dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY, dut::Mcs::MCS_4096QAM_34, 1, dut::Gi::GI_0_8_US, 2594.1f },
        { dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY, dut::Mcs::MCS_4096QAM_56, 1, dut::Gi::GI_0_8_US, 2882.4f },
    };

    calculatePhyDataRateTest(testData);
}

TEST_F(DutWithClientMockTest, calibrateRxLnaMidGainsShouldFailIfRxCalibrationNotStarted)
{
    std::array<float, dut::maxNumRxAntennas> targetGains { 20.0f, 20.0f, 20.0f, 20.0f, 0.0f };
    std::array<float, dut::maxNumRxAntennas> prxin { -60.0f, -60.0f, -60.0f, -60.0f, 0.0f };
    uint16_t numSamples = 1024;
    int8_t pgc1 = 0;
    int8_t pgc2 = 12;
    std::array<float, dut::maxNumRxAntennas> calcGain {};
    std::array<uint8_t, dut::maxNumRxAntennas> calcGainCtrl {};

    EXPECT_FALSE(m_dut.calibrateRxLnaMidGains(targetGains, prxin, numSamples, pgc1, pgc2, calcGain, calcGainCtrl));
    EXPECT_EQ(m_dut.getLastError(), "Invalid state: RX calibration not started");
}

TEST_F(DutWithClientMockTest, calibrateTssiS2dShouldFailWithInvalidLowerPowerLevel)
{
    uint8_t lowerPowerLevel = dut::maxPowerLevelIndex + 1;
    uint8_t region = 1;
    uint8_t gain;
    uint8_t offset;

    EXPECT_FALSE(m_dut.calibrateTssiS2d(lowerPowerLevel, 0, 0, 0, region, 0, gain, offset));
    EXPECT_EQ(m_dut.getLastError(), "The specified lower power level index (" + dut::toString(lowerPowerLevel) + ") exceeds maximum allowed value (" + dut::toString(dut::maxPowerLevelIndex) + ")");
}

TEST_F(DutWithClientMockTest, calibrateTssiS2dShouldFailWithInvalidUpperPowerLevel)
{
    uint8_t upperPowerLevel = dut::maxPowerLevelIndex + 1;
    uint8_t region = 1;
    uint8_t gain;
    uint8_t offset;

    EXPECT_FALSE(m_dut.calibrateTssiS2d(0, upperPowerLevel, 0, 0, region, 0, gain, offset));
    EXPECT_EQ(m_dut.getLastError(), "The specified upper power level index (" + dut::toString(upperPowerLevel) + ") exceeds maximum allowed value (" + dut::toString(dut::maxPowerLevelIndex) + ")");
}

TEST_F(DutWithClientMockTest, calibrateTssiS2dShouldFailWithLowerPowerLevelGreaterThanUpperPowerLevel)
{
    uint8_t upperPowerLevel = 0;
    uint8_t lowerPowerLevel = upperPowerLevel + 1;
    uint8_t region = 1;
    uint8_t gain;
    uint8_t offset;

    EXPECT_FALSE(m_dut.calibrateTssiS2d(lowerPowerLevel, upperPowerLevel, 0, 0, region, 0, gain, offset));
    EXPECT_EQ(m_dut.getLastError(), "The specified lower power level index (" + dut::toString(lowerPowerLevel) + ") is greater than the specified upper power level index (" + dut::toString(upperPowerLevel) + ")");
}

TEST_F(DutWithClientMockTest, calibrateTssiS2dShouldFailWithInvalidRegion)
{
    uint8_t region = dut::maxNumTxRegions + 1;
    uint8_t gain;
    uint8_t offset;

    EXPECT_FALSE(m_dut.calibrateTssiS2d(0, 0, 0, 0, region, 0, gain, offset));
    EXPECT_EQ(m_dut.getLastError(), "Invalid region value (" + dut::toString(region) + "). Valid values range from 1 to " + dut::toString(dut::maxNumTxRegions));
}

TEST_F(DutWithClientMockTest, calibrateTssiS2dShouldFailWithInvalidPowerThreshold)
{
    uint8_t region = 1;
    uint8_t powerThreshold = dut::maxPowerLevelIndex + 1;
    uint8_t gain;
    uint8_t offset;

    EXPECT_FALSE(m_dut.calibrateTssiS2d(0, 0, 0, 0, region, powerThreshold, gain, offset));
    EXPECT_EQ(m_dut.getLastError(), "The specified power threshold value (" + dut::toString(powerThreshold) + ") exceeds maximum allowed value (" + dut::toString(dut::maxPowerLevelIndex) + ")");
}

TEST_F(DutWithClientMockTest, calibrateTssiS2dShouldFailIfMoreThanOneTxAntennaIsSet)
{
    uint8_t region = 1;
    uint8_t gain;
    uint8_t offset;

    EXPECT_FALSE(m_dut.calibrateTssiS2d(0, 0, 0, 0, region, 0, gain, offset));
    EXPECT_EQ(m_dut.getLastError(), "Illegal antenna selection (only one TX antenna is allowed)");
}

TEST_F(DutWithClientMockTest, calibrateTssiS2dShouldFailIfNoTxAntennaIsSet)
{
    dut::AntennaMask enabledTxAntennaMask = 0x00;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));

    uint8_t region = 1;
    uint8_t gain;
    uint8_t offset;

    EXPECT_FALSE(m_dut.calibrateTssiS2d(0, 0, 0, 0, region, 0, gain, offset));
    EXPECT_EQ(m_dut.getLastError(), "Illegal antenna selection (at least one TX antenna must be selected)");
}

TEST_F(DutWithClientMockTest, calibrateTssiS2dShouldFailIfNotTxStarted)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, setRate(phyMode, signalBandwidth, dut::Rates::getMcsIndex(phyMode, mcs, nss), nss, gi, ltf));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, dut::defaultTransmitPowerLevel));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));

    uint8_t region = 1;
    uint8_t gain;
    uint8_t offset;

    EXPECT_FALSE(m_dut.calibrateTssiS2d(0, 0, 0, 0, region, 0, gain, offset));
    EXPECT_EQ(m_dut.getLastError(), "Packet transmission not started");
}

TEST_F(DutWithClientMockTest, calibrateTssiS2dShouldSucceedIfTransmitVoltageHigherThanUpperLimit)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AC;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    uint16_t repetitions = UINT16_MAX;
    uint32_t packetLength = 4000;
    bool longData = false;
    bool beamforming = false;

    uint8_t lowerPowerLevel = 6;
    uint8_t upperPowerLevel = 56;
    uint8_t gain = 2;
    uint8_t offset = 15;
    uint32_t tssiLow = 2000;
    uint32_t tssiHigh = 14000;

    uint8_t region = 1;
    uint8_t powerThreshold = 62;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, setRate(phyMode, signalBandwidth, dut::Rates::getMcsIndex(phyMode, mcs, nss), nss, gi, ltf));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
        // setTssiS2dParams
        {
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setS2dParams(antenna, signalBandwidth, region, powerThreshold, gain, offset));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
        }
        // transmitPowersGetVoltage
        {
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, lowerPowerLevel));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
            EXPECT_CALL(*m_client, getTransmitVoltages(enabledTxAntennaMask, _)).WillOnce(Invoke([antenna](const dut::AntennaMask&, std::array<uint32_t, dut::maxNumTxAntennas>& voltages) {
                voltages[antenna] = 9243;
            }));
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, upperPowerLevel));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
            EXPECT_CALL(*m_client, getTransmitVoltages(enabledTxAntennaMask, _)).WillOnce(Invoke([antenna](const dut::AntennaMask&, std::array<uint32_t, dut::maxNumTxAntennas>& voltages) {
                voltages[antenna] = 16368;
            }));
        }
        // s2dOffsetlResMeasure
        {
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setS2dParams(antenna, signalBandwidth, region, powerThreshold, _, _));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, lowerPowerLevel));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
            EXPECT_CALL(*m_client, getTransmitVoltages(enabledTxAntennaMask, _)).WillOnce(Invoke([antenna](const dut::AntennaMask&, std::array<uint32_t, dut::maxNumTxAntennas>& voltages) {
                voltages[antenna] = 9435;
            }));
        }
        // setTssiS2dParams
        {
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setS2dParams(antenna, signalBandwidth, region, powerThreshold, _, _));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
        }
        // transmitPowersGetVoltage
        {
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, lowerPowerLevel));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
            EXPECT_CALL(*m_client, getTransmitVoltages(enabledTxAntennaMask, _)).WillOnce(Invoke([antenna](const dut::AntennaMask&, std::array<uint32_t, dut::maxNumTxAntennas>& voltages) {
                voltages[antenna] = 4731;
            }));
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, upperPowerLevel));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
            EXPECT_CALL(*m_client, getTransmitVoltages(enabledTxAntennaMask, _)).WillOnce(Invoke([antenna](const dut::AntennaMask&, std::array<uint32_t, dut::maxNumTxAntennas>& voltages) {
                voltages[antenna] = 18000;
            }));
        }
        // setTssiS2dParams
        {
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setS2dParams(antenna, signalBandwidth, region, powerThreshold, _, _));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
        }
        EXPECT_CALL(*m_client, stopTx());
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));
    ASSERT_TRUE(m_dut.startTx(repetitions, packetLength, longData, beamforming));

    ASSERT_TRUE(m_dut.calibrateTssiS2d(lowerPowerLevel, upperPowerLevel, tssiLow, tssiHigh, region, powerThreshold, gain, offset));

    constexpr uint8_t expectedGain = 0;
    constexpr uint8_t expectedOffset = 39;

    EXPECT_EQ(gain, expectedGain);
    EXPECT_EQ(offset, expectedOffset);

    ASSERT_TRUE(m_dut.stopTx());
}

TEST_F(DutWithClientMockTest, calibrateTssiS2dShouldSucceed)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AC;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    uint16_t repetitions = UINT16_MAX;
    uint32_t packetLength = 4000;
    bool longData = false;
    bool beamforming = false;

    uint8_t lowerPowerLevel = 6;
    uint8_t upperPowerLevel = 56;
    uint8_t gain = 2;
    uint8_t offset = 15;
    uint32_t tssiLow = 2000;
    uint32_t tssiHigh = 14000;

    uint8_t region = 1;
    uint8_t powerThreshold = 62;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, setRate(phyMode, signalBandwidth, dut::Rates::getMcsIndex(phyMode, mcs, nss), nss, gi, ltf));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
        // setTssiS2dParams
        {
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setS2dParams(antenna, signalBandwidth, region, powerThreshold, gain, offset));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
        }
        // transmitPowersGetVoltage
        {
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, lowerPowerLevel));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
            EXPECT_CALL(*m_client, getTransmitVoltages(enabledTxAntennaMask, _)).WillOnce(Invoke([antenna](const dut::AntennaMask&, std::array<uint32_t, dut::maxNumTxAntennas>& voltages) {
                voltages[antenna] = 9243;
            }));
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, upperPowerLevel));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
            EXPECT_CALL(*m_client, getTransmitVoltages(enabledTxAntennaMask, _)).WillOnce(Invoke([antenna](const dut::AntennaMask&, std::array<uint32_t, dut::maxNumTxAntennas>& voltages) {
                voltages[antenna] = 16368;
            }));
        }
        // s2dOffsetlResMeasure
        {
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setS2dParams(antenna, signalBandwidth, region, powerThreshold, _, _));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, lowerPowerLevel));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
            EXPECT_CALL(*m_client, getTransmitVoltages(enabledTxAntennaMask, _)).WillOnce(Invoke([antenna](const dut::AntennaMask&, std::array<uint32_t, dut::maxNumTxAntennas>& voltages) {
                voltages[antenna] = 9435;
            }));
        }
        // setTssiS2dParams
        {
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setS2dParams(antenna, signalBandwidth, region, powerThreshold, _, _));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
        }
        // transmitPowersGetVoltage
        {
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, lowerPowerLevel));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
            EXPECT_CALL(*m_client, getTransmitVoltages(enabledTxAntennaMask, _)).WillOnce(Invoke([antenna](const dut::AntennaMask&, std::array<uint32_t, dut::maxNumTxAntennas>& voltages) {
                voltages[antenna] = 4731;
            }));
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, upperPowerLevel));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
            EXPECT_CALL(*m_client, getTransmitVoltages(enabledTxAntennaMask, _)).WillOnce(Invoke([antenna](const dut::AntennaMask&, std::array<uint32_t, dut::maxNumTxAntennas>& voltages) {
                voltages[antenna] = 14449;
            }));
        }
        // setTssiS2dParams
        {
            EXPECT_CALL(*m_client, stopTx());
            EXPECT_CALL(*m_client, setS2dParams(antenna, signalBandwidth, region, powerThreshold, _, _));
            EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
        }
        EXPECT_CALL(*m_client, stopTx());
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));
    ASSERT_TRUE(m_dut.startTx(repetitions, packetLength, longData, beamforming));

    ASSERT_TRUE(m_dut.calibrateTssiS2d(lowerPowerLevel, upperPowerLevel, tssiLow, tssiHigh, region, powerThreshold, gain, offset));

    uint8_t expectedGain = 1;
    uint8_t expectedOffset = 39;

    EXPECT_EQ(gain, expectedGain);
    EXPECT_EQ(offset, expectedOffset);

    ASSERT_TRUE(m_dut.stopTx());
}

TEST_F(DutWithClientMockTest, getBbicCddValuesShouldFailWithInvalidNumberOfAntennas)
{
    uint8_t numTxAntennas = dut::maxNumTxAntennas + 1;
    uint32_t offset1;
    uint32_t offset2;
    uint32_t offset3;

    EXPECT_FALSE(m_dut.getBbicCddValues(numTxAntennas, offset1, offset2, offset3));
    EXPECT_EQ(m_dut.getLastError(), "The specified number of antennas (" + dut::toString(numTxAntennas) + ") exceeds maximum allowed value (" + dut::toString(dut::maxNumTxAntennas) + ")");
}

TEST_F(DutWithClientMockTest, getCalibrationFileVersionShouldSucceed)
{
    dut::CalibrationFileVersion version { dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_INVALID };
    dut::CalibrationFileSubversion subversion { dut::CalibrationFileSubversion::CALIBRATION_FILE_SUBVERSION_INVALID };

    ASSERT_TRUE(m_dut.getCalibrationFileVersion(version, subversion));

    EXPECT_EQ(version, dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6);
    EXPECT_EQ(subversion, dut::CalibrationFileSubversion::CALIBRATION_FILE_SUBVERSION_0);
}

TEST_F(DutWithClientMockTestGen7, getEnabledRxAntennaMaskShouldSucceedWithZwdfs)
{
    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));
    ASSERT_EQ(enabledRxAntennaMask, rxAntennaMaskGen7);

    dut::AntennaMask newRxAntennaMask = 0x01;
    dut::AntennaMask newZwdfsAntennaMask = 0x10;
    dut::AntennaMask newEnabledRxAntennaMask = newRxAntennaMask | newZwdfsAntennaMask;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setZwdfsEnabled(true, dut::defaultZwdfsChannel, dut::defaultZwdfsBandwidth, dut::defaultRadarDetectionBandwidth, dut::defaultRegulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(newEnabledRxAntennaMask));
    }

    EXPECT_TRUE(m_dut.setEnabledRxAntennaMask(newEnabledRxAntennaMask));

    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));
    ASSERT_EQ(enabledRxAntennaMask, newEnabledRxAntennaMask);
}

TEST_F(DutWithClientMockTest, getNvmSizeShouldSucceed)
{
    size_t size = 0;

    ASSERT_TRUE(m_dut.getNvmSize(size));

    EXPECT_EQ(size, static_cast<size_t>(memorySize));
}

TEST_F(DutWithClientMockTest, getNvmTypeShouldSucceed)
{
    dut::NvMemoryType type = dut::NvMemoryType::MEMORY_TYPE_EEPROM;

    ASSERT_TRUE(m_dut.getNvmType(type));

    EXPECT_EQ(type, memoryType);
}

TEST_F(DutWithClientMockTest, getNvmVersionShouldSucceed)
{
    uint8_t version = 0;

    ASSERT_TRUE(m_dut.getNvmVersion(version));

    EXPECT_EQ(version, 6);
}

TEST_F(DutWithClientMockTest, getPhyModeShouldSucceed)
{
    dut::PhyMode actualPhyMode = dut::PhyMode::PHY_MODE_A;

    ASSERT_FALSE(m_dut.getPhyMode(actualPhyMode));
    EXPECT_EQ(m_dut.getLastError(), "PhyMode not set");

    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, setRate(phyMode, signalBandwidth, dut::Rates::getMcsIndex(phyMode, mcs, nss), nss, gi, ltf));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, dut::defaultTransmitPowerLevel));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));

    ASSERT_TRUE(m_dut.getPhyMode(actualPhyMode));
    EXPECT_EQ(phyMode, actualPhyMode);
}

TEST_F(DutWithClientMockTest, getRxCwPowerShouldFailIfRxCalibrationNotStarted)
{
    uint8_t method = 0;
    int32_t freqOffset = 0;
    uint16_t numSamples = 1024;
    std::array<dut::CorrelationResults_t, dut::maxNumRxAntennas> correlationResults {};

    EXPECT_FALSE(m_dut.getRxCwPower(method, freqOffset, numSamples, correlationResults));
    EXPECT_EQ(m_dut.getLastError(), "Invalid state: RX calibration not started");
}

TEST_F(DutWithClientMockTest, getRxRateInfoShouldFailWithoutPhymode)
{
    uint8_t mcs;
    uint8_t nss;

    EXPECT_FALSE(m_dut.getRxRateInfo(mcs, nss));
    EXPECT_EQ(m_dut.getLastError(), "PhyMode not set");
}

TEST_F(DutWithClientMockTest, getRxRateInfoShouldSucceed)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);
    dut::AntennaMask enabledRxAntennaMask(1 << antenna);
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    uint8_t actualMcs = 0;
    uint8_t expectedMcs = 7;
    uint8_t actualNss = 0;
    uint8_t expectedNss = 2;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, getRxRateInfo(_, _)).WillOnce(Invoke([expectedMcs, expectedNss](uint8_t& mcs, uint8_t& nss) {
            mcs = expectedMcs;
            nss = expectedNss;
        }));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setEnabledRxAntennaMask(enabledRxAntennaMask));

    ASSERT_TRUE(m_dut.getRxRateInfo(actualMcs, actualNss));

    EXPECT_EQ(actualMcs, expectedMcs);
    EXPECT_EQ(actualNss, expectedNss);
}

TEST_F(DutWithClientMockTest, getTemperatureShouldSucceed)
{
    float actualTemperature = 0.0f;
    float expectedTemperature = 12.34f;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, getTemperature(_)).WillOnce(Invoke([expectedTemperature](float& temperature) {
            temperature = expectedTemperature;
        }));
    }

    ASSERT_TRUE(m_dut.getTemperature(actualTemperature));

    EXPECT_EQ(actualTemperature, expectedTemperature);
}

TEST_F(DutWithClientMockTest, getTransmitPowerTableOffsetShouldFailWithInvalidAntenna)
{
    uint8_t antenna = dut::maxNumTxAntennas;
    dut::Bandwidth bandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    int16_t offset;

    EXPECT_FALSE(m_dut.getTransmitPowerTableOffset(antenna, bandwidth, offset));
    EXPECT_EQ(m_dut.getLastError(), "Invalid antenna index value (" + dut::toString(antenna) + "). Valid values range from 0 to " + dut::toString(dut::maxNumTxAntennas - 1));
}

TEST_F(DutWithClientMockTest, loadNvmFromFileShouldFailIfInvalidContents)
{
    // Buffer with invalid calibration file contents
    dut::ResizableBuffer buffer(static_cast<size_t>(memorySize));
    for (size_t i = 0; i < buffer.size(); i++) {
        buffer.append(0x00);
    }

    TemporaryFile temporaryFile(buffer.data(), buffer.size());

    EXPECT_FALSE(m_dut.loadNvmFromFile(temporaryFile.getFilename()));
    EXPECT_EQ(m_dut.getLastError(), "Invalid calibration file '" + std::string(temporaryFile.getFilename()) + "': Invalid version 0");
}

TEST_F(DutWithClientMockTest, loadNvmFromFileShouldFailIfInvalidSize)
{
    // Buffer with invalid calibration file size
    dut::ResizableBuffer buffer(static_cast<size_t>(memorySize) + 1);
    for (size_t i = 0; i < buffer.size(); i++) {
        buffer.append(0x00);
    }

    TemporaryFile temporaryFile(buffer.data(), buffer.size());

    EXPECT_FALSE(m_dut.loadNvmFromFile(temporaryFile.getFilename()));
    EXPECT_EQ(m_dut.getLastError(), "Invalid file size for file '" + std::string(temporaryFile.getFilename()) + "'");
}

TEST_F(DutWithClientMockTest, measureRxLnaSubBandGainsShouldFailIfRxCalibrationNotStarted)
{
    EXPECT_FALSE(m_dut.measureRxLnaSubBandGains());
    EXPECT_EQ(m_dut.getLastError(), "Invalid state: RX calibration not started");
}

TEST_F(DutWithClientMockTest, rxMeasureShouldFailIfRxPacketLimitRunning)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    uint32_t packetLimit = 10000;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, startRxPer(_, _));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.startRxPer(packetLimit));

    uint32_t numCaptures = 1;
    uint32_t captureInterval = 1000;
    bool disabled = false;
    EXPECT_FALSE(m_dut.rxMeasure(numCaptures, captureInterval, disabled));
    EXPECT_EQ(m_dut.getLastError(), "Rx Packet Limit running");
}

TEST_F(DutWithClientMockTest, rxMeasureShouldFailWithoutPhymode)
{
    uint32_t numCaptures = 1;
    uint32_t captureInterval = 1000;
    bool disabled = false;
    EXPECT_FALSE(m_dut.rxMeasure(numCaptures, captureInterval, disabled));
    EXPECT_EQ(m_dut.getLastError(), "PhyMode not set");
}

TEST_F(DutWithClientMockTest, rxMeasureShouldFailIfRxAntennaMaskZero)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);
    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(_));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setEnabledRxAntennaMask(0x00));

    uint32_t numCaptures = 1;
    uint32_t captureInterval = 1000;
    bool disabled = false;
    EXPECT_FALSE(m_dut.rxMeasure(numCaptures, captureInterval, disabled));
    EXPECT_EQ(m_dut.getLastError(), "Illegal antenna selection (at least one RX antenna must be selected)");
}

TEST_F(DutWithClientMockTest, saveNvmToFileShouldSucceed)
{
    // Get current NVM size and contents
    size_t initialSize = 0;
    ASSERT_TRUE(m_dut.getNvmSize(initialSize));

    std::shared_ptr<uint8_t> initialData(new uint8_t[initialSize], std::default_delete<uint8_t[]>());
    ASSERT_TRUE(m_dut.readNvm(0, initialData.get(), initialSize));

    TemporaryFile temporaryFile;

    // Save NVM to file
    ASSERT_TRUE(m_dut.saveNvmToFile(temporaryFile.getFilename()));

    // Load NVM from that file and check that size and contents are the same as before
    ASSERT_TRUE(m_dut.loadNvmFromFile(temporaryFile.getFilename()));

    size_t size = 0;
    ASSERT_TRUE(m_dut.getNvmSize(size));

    std::shared_ptr<uint8_t> data(new uint8_t[size], std::default_delete<uint8_t[]>());
    ASSERT_TRUE(m_dut.readNvm(0, data.get(), size));

    ASSERT_EQ(memcmp(initialData.get(), data.get(), size), 0);
}

TEST_F(DutWithClientMockTest, saveNvmToFileShouldFailWithInvalidFileName)
{
    EXPECT_FALSE(m_dut.saveNvmToFile(""));
    EXPECT_EQ(m_dut.getLastError(), "Unable to open file '' for writing");
}

TEST_F(DutWithClientMockTest, setBbicCddValuesShouldFailWithInvalidNumberOfAntennas)
{
    uint8_t numTxAntennas = dut::maxNumTxAntennas + 1;
    uint32_t offset1 = 1;
    uint32_t offset2 = 2;
    uint32_t offset3 = 3;

    EXPECT_FALSE(m_dut.setBbicCddValues(numTxAntennas, offset1, offset2, offset3));
    EXPECT_EQ(m_dut.getLastError(), "The specified number of antennas (" + dut::toString(numTxAntennas) + ") exceeds maximum allowed value (" + dut::toString(dut::maxNumTxAntennas) + ")");
}

TEST_F(DutWithClientMockTest, setCalibrationFileVersionShouldSucceed)
{
    dut::CalibrationFileVersion version { dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_7 };
    dut::CalibrationFileSubversion subversion { dut::CalibrationFileSubversion::CALIBRATION_FILE_SUBVERSION_1 };

    ASSERT_TRUE(m_dut.setCalibrationFileVersion(version, subversion));

    version = dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_INVALID;
    subversion = dut::CalibrationFileSubversion::CALIBRATION_FILE_SUBVERSION_INVALID;

    ASSERT_TRUE(m_dut.getCalibrationFileVersion(version, subversion));

    EXPECT_EQ(version, dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_7);
    EXPECT_EQ(subversion, dut::CalibrationFileSubversion::CALIBRATION_FILE_SUBVERSION_1);
}

#ifdef ALLOW_ILLEGAL_CHANNELS
TEST_F(DutWithClientMockTest, setChannelShouldFailIfInvalidPhyModeWith5000MHz)
{
    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_N_2_4;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    ASSERT_FALSE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    EXPECT_EQ(m_dut.getLastError(), "Invalid PHY mode (N_2_4) for 5GHz band. Valid values are A, N-5GHz, AC, AX and BE");
}

TEST_F(DutWithClientMockTest, setChannelShouldFailWithInvalidLowestChannel)
{
    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 0;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    EXPECT_FALSE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    EXPECT_EQ(m_dut.getLastError(), "Invalid channel (0)");
}
#endif

TEST_F(DutWithClientMockTest, setChannelShouldFailWithInvalidPrimaryChannelIndexIn20MHz)
{
    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 1;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    EXPECT_FALSE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    EXPECT_EQ(m_dut.getLastError(), "Invalid primary channel index (1) for " + dut::toString(spectrumBandwidth) + " bandwidth");
}

TEST_F(DutWithClientMockTest, setChannelShouldFailWithInvalidPrimaryChannelIndexIn40MHz)
{
    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_FOURTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 2;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    EXPECT_FALSE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    EXPECT_EQ(m_dut.getLastError(), "Invalid primary channel index (2) for " + dut::toString(spectrumBandwidth) + " bandwidth");
}

TEST_F(DutWithClientMockTest, setChannelShouldFailWithInvalidPrimaryChannelIndexIn80MHz)
{
    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_EIGHTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 4;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    EXPECT_FALSE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    EXPECT_EQ(m_dut.getLastError(), "Invalid primary channel index (4) for " + dut::toString(spectrumBandwidth) + " bandwidth");
}

TEST_F(DutWithClientMockTest, setChannelShouldFailWithInvalidPrimaryChannelIndexIn160MHz)
{
    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 8;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    EXPECT_FALSE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    EXPECT_EQ(m_dut.getLastError(), "Invalid primary channel index (8) for " + dut::toString(spectrumBandwidth) + " bandwidth");
}

TEST_F(DutWithClientMockTest, setChannelShouldFailWithInvalidPrimaryChannelIndexIn320MHz)
{
    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_BE;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY;
    uint8_t channel = 1;
    uint8_t primaryChannelIndex = 16;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    EXPECT_FALSE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    EXPECT_EQ(m_dut.getLastError(), "Invalid primary channel index (16) for " + dut::toString(spectrumBandwidth) + " bandwidth");
}

TEST_F(DutWithClientMockTest, setChannelShouldSucceedAndDoNothingIfNoChangeIsRequired)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
}

TEST_F(DutWithClientMockTest, setClipperShouldSucceedAndDoNothingIfNoChangeIsRequired)
{
    bool enabled = false;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setClipper(enabled));
    }

    ASSERT_TRUE(m_dut.setClipper(enabled));

    ASSERT_TRUE(m_dut.setClipper(enabled));
}

TEST_F(DutWithClientMockTest, setEnabledRxAntennaMaskShouldFailWithInvalidAntennaMask)
{
    dut::AntennaMask availableAntennaMask;
    m_dut.getAvailableRxAntennaMask(availableAntennaMask);

    dut::AntennaMask invalidAntennaMask(availableAntennaMask << 1);

    EXPECT_FALSE(m_dut.setEnabledRxAntennaMask(invalidAntennaMask));
    EXPECT_EQ(m_dut.getLastError(), "Invalid antenna mask " + dut::toHexString(invalidAntennaMask) + ", available RX antenna mask is " + dut::toHexString(availableAntennaMask));
}

TEST_F(DutWithClientMockTest, setEnabledRxAntennaMaskShouldSucceedAndDoNothingIfNoChangeIsRequired)
{
    dut::AntennaMask enabledRxAntennaMask;
    m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask);

    EXPECT_TRUE(m_dut.setEnabledRxAntennaMask(enabledRxAntennaMask));
}

TEST_F(DutWithClientMockTestGen7, setEnabledRxAntennaMaskShouldSucceedAndDoNothingIfNoChangeIsRequired)
{
    dut::AntennaMask newRxAntennaMask = 0x01;
    dut::AntennaMask newZwdfsAntennaMask = 0x10;
    dut::AntennaMask enabledRxAntennaMask = newRxAntennaMask | newZwdfsAntennaMask;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setZwdfsEnabled(true, dut::defaultZwdfsChannel, dut::defaultZwdfsBandwidth, dut::defaultRadarDetectionBandwidth, dut::defaultRegulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
    }

    EXPECT_TRUE(m_dut.setEnabledRxAntennaMask(enabledRxAntennaMask));

    EXPECT_TRUE(m_dut.setEnabledRxAntennaMask(enabledRxAntennaMask));
}

TEST_F(DutWithClientMockTest, setEnabledTxAntennaMaskShouldFailWithInvalidAntennaMask)
{
    dut::AntennaMask availableTxAntennaMask;
    m_dut.getAvailableTxAntennaMask(availableTxAntennaMask);

    dut::AntennaMask invalidTxAntennaMask(availableTxAntennaMask << 1);

    EXPECT_FALSE(m_dut.setEnabledTxAntennaMask(invalidTxAntennaMask));
    EXPECT_EQ(m_dut.getLastError(), "Invalid antenna mask " + dut::toHexString(invalidTxAntennaMask) + ", available TX antenna mask is " + dut::toHexString(availableTxAntennaMask));
}

TEST_F(DutWithClientMockTest, setEnabledTxAntennaMaskShouldSucceedAndDoNothingIfNoChangeIsRequired)
{
    dut::AntennaMask enabledTxAntennaMask;
    m_dut.getEnabledTxAntennaMask(enabledTxAntennaMask);

    EXPECT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
}

TEST_F(DutWithClientMockTest, setEnabledTxAntennaMaskShouldRestoreTransmitPowerLevelIfChannelSet)
{
    dut::AntennaMask oldEnabledTxAntennaMask;
    m_dut.getEnabledTxAntennaMask(oldEnabledTxAntennaMask);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    uint8_t powerLevel = 6;

    {
        InSequence sequence;

        for (uint8_t i = 0; i < dut::maxNumTxAntennas; i++) {
            if (oldEnabledTxAntennaMask.isBitSet(i)) {
                EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(i, _, _, _));
            }
        }
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, oldEnabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        for (uint8_t i = 0; i < dut::maxNumTxAntennas; i++) {
            if (oldEnabledTxAntennaMask.isBitSet(i)) {
                EXPECT_CALL(*m_client, setTransmitPowerLevel(i, spectrumBandwidth, powerLevel));
            }
        }
        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        // Check that transmit power limit is restored when TX antenna mask is changed
        EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, powerLevel));
    }

    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setTransmitPowerLevel(powerLevel));
    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
}

TEST_F(DutWithClientMockTest, setHdkConfigShouldSucceedAndDoNothingIfNoChangeIsRequired)
{
    uint32_t offlineCalMask = 0xbabe;
    uint32_t onlineCalMask = 0xbeef;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setHdkConfig(offlineCalMask, onlineCalMask, _));
    }

    ASSERT_TRUE(m_dut.setHdkConfig(offlineCalMask, onlineCalMask));

    ASSERT_TRUE(m_dut.setHdkConfig(offlineCalMask, onlineCalMask));
}

TEST_F(DutWithClientMockTest, setIfsShouldFailWithInvalidValue)
{
    uint32_t ifs = dut::minInterframeSpace - 1;

    EXPECT_FALSE(m_dut.setIfs(ifs));
    EXPECT_EQ(m_dut.getLastError(), "Invalid interframe space (minimum allowed value is 10 us)");
}

TEST_F(DutWithClientMockTest, setIfsShouldSucceedAndDoNothingIfNoChangeIsRequired)
{
    uint32_t ifs = 20;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setIfs(ifs));
    }

    ASSERT_TRUE(m_dut.setIfs(ifs));

    ASSERT_TRUE(m_dut.setIfs(ifs));
}

TEST_F(DutWithClientMockTest, setRateShouldSucceedAndDoNothingIfNoChangeIsRequired)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_N_5;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, setRate(phyMode, signalBandwidth, _, nss, gi, ltf));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setRate(phyMode, signalBandwidth, _, nss + 1, gi, ltf));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, dut::defaultTransmitPowerLevel));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));

    // This call is the same as previous so chache hit prevents sending any command to DUT
    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));

    // This call is not the same as previous so chache miss forces sending command to DUT
    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss + 1, gi, ltf, rateMbps));
}

TEST_F(DutWithClientMockTest, setRssiS2dParamsShouldFailWithInvalidAntenna)
{
    dut::AntennaMask availableAntennaMask;
    m_dut.getAvailableRxAntennaMask(availableAntennaMask);

    dut::AntennaMask invalidAntennaMask(availableAntennaMask << 1);
    uint8_t region = 1;
    uint8_t gain = 0;
    uint8_t offset = 15;

    EXPECT_FALSE(m_dut.setRssiS2dParams(invalidAntennaMask, region, gain, offset));
    EXPECT_EQ(m_dut.getLastError(), "Invalid antenna mask " + dut::toHexString(invalidAntennaMask) + ", available RX antenna mask is " + dut::toHexString(availableAntennaMask));
}

TEST_F(DutWithClientMockTest, setRssiS2dParamsShouldFailWithInvalidRegion)
{
    dut::AntennaMask antennaMask = 0x01;
    uint8_t region = dut::maxNumRxRegionsVer6 + 1;
    uint8_t gain = 0;
    uint8_t offset = 15;

    EXPECT_FALSE(m_dut.setRssiS2dParams(antennaMask, region, gain, offset));
    EXPECT_EQ(m_dut.getLastError(), "Invalid region value (" + dut::toString(region) + "). Valid values range from 1 to " + dut::toString(dut::maxNumRxRegionsVer6));
}

TEST_F(DutWithClientMockTest, setRuParamsShouldFailWithoutSetRate)
{
    uint32_t userOne = 100;
    uint32_t userTwo = 100;

    EXPECT_FALSE(m_dut.setRuParams(userOne, userTwo));
    EXPECT_EQ(m_dut.getLastError(), "Rate not set");
}

TEST_F(DutWithClientMockTest, setRuParamsShouldFailWithInvalidUserOneValue)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_N_5;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    uint32_t userOne = dut::maxRuParamsUserOneValue + 1;
    uint32_t userTwo = dut::maxRuParamsUserTwoValue;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, setRate(phyMode, signalBandwidth, _, nss, gi, ltf));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, dut::defaultTransmitPowerLevel));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));

    EXPECT_FALSE(m_dut.setRuParams(userOne, userTwo));
    EXPECT_EQ(m_dut.getLastError(), "Invalid User 1 Value! Maximum allowed value is 511");
}

TEST_F(DutWithClientMockTest, setRuParamsShouldFailWithInvalidUserTwoValue)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_N_5;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    uint32_t userOne = dut::maxRuParamsUserOneValue;
    uint32_t userTwo = dut::maxRuParamsUserTwoValue + 1;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, setRate(phyMode, signalBandwidth, _, nss, gi, ltf));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, dut::defaultTransmitPowerLevel));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));

    EXPECT_FALSE(m_dut.setRuParams(userOne, userTwo));
    EXPECT_EQ(m_dut.getLastError(), "Invalid User 2 Value! Maximum allowed value is 512");
}

TEST_F(DutWithClientMockTest, setRuParamsShouldSucceedAndDoNothingIfNoChangeIsRequired)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_N_5;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    uint32_t userOne = 100;
    uint32_t userTwo = 100;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, setRate(phyMode, signalBandwidth, _, nss, gi, ltf));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setRuParams(userOne, userTwo));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));

    ASSERT_TRUE(m_dut.setRuParams(userOne, userTwo));
    ASSERT_TRUE(m_dut.setRuParams(userOne, userTwo));
}

TEST_F(DutWithClientMockTest, setRxAggregationEnabledShouldSucceedAndDoNothingIfNoChangeIsRequired)
{
    bool rxAggregationEnabled = true;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setRxAggregationEnabled(rxAggregationEnabled));
    }

    ASSERT_TRUE(m_dut.setRxAggregationEnabled(rxAggregationEnabled));

    ASSERT_TRUE(m_dut.setRxAggregationEnabled(rxAggregationEnabled));
}

TEST_F(DutWithClientMockTest, setSpacelessTxEnabledShouldFailIfChannelNotSet)
{
    bool enabled = true;

    EXPECT_FALSE(m_dut.setSpacelessTxEnabled(enabled));
    EXPECT_EQ(m_dut.getLastError(), "Channel not set");
}

TEST_F(DutWithClientMockTest, setTransmitPowerControlShouldSucceedAndDoNothingIfNoChangeIsRequired)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    bool closedLoop = true;
    uint8_t powerLimit = 0xf;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerControl(dut::CalibrationType::TSSI_CAL_TYPE_VER6, closedLoop, powerLimit));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setTransmitPowerControl(closedLoop, powerLimit));

    ASSERT_TRUE(m_dut.setTransmitPowerControl(closedLoop, powerLimit));
}

TEST_F(DutWithClientMockTest, setTransmitPowerControlShouldSucceedIfNotAutoPower)
{
    bool closedLoop = true;
    uint8_t powerLimit = 0xff;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setTransmitPowerControl(dut::CalibrationType::TSSI_CAL_TYPE_VER6, closedLoop, powerLimit));
    }

    ASSERT_TRUE(m_dut.setTransmitPowerControl(closedLoop, powerLimit));
}

TEST_F(DutWithClientMockTest, setTransmitPowerControlShouldSucceedIfChannelNotSet)
{
    bool closedLoop = true;
    uint8_t powerLimit = 0xf;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setTransmitPowerControl(dut::CalibrationType::TSSI_CAL_TYPE_VER6, closedLoop, powerLimit));
    }

    ASSERT_TRUE(m_dut.setTransmitPowerControl(closedLoop, powerLimit));
}

TEST_F(DutWithClientMockTest, setTransmitPowerLevelShouldFailIfChannelNotSet)
{
    uint8_t powerLevel = 0;

    EXPECT_FALSE(m_dut.setTransmitPowerLevel(powerLevel));
    EXPECT_EQ(m_dut.getLastError(), "Channel not set");
}

TEST_F(DutWithClientMockTest, setTransmitPowerLevelShouldFailIfInvalidPowerLevel)
{
    uint8_t powerLevel = dut::maxPowerLevelIndex + 1;

    EXPECT_FALSE(m_dut.setTransmitPowerLevel(powerLevel));
    EXPECT_EQ(m_dut.getLastError(), "The specified power level value (64) exceeds maximum allowed value (63)");
}

TEST_F(DutWithClientMockTest, setTransmitPowerLevelShouldFailIfInvalidLowPowerLevel)
{
    uint8_t powerLevel = static_cast<uint8_t>(dut::minPowerLevelIndex - 1);

    EXPECT_FALSE(m_dut.setTransmitPowerLevel(powerLevel));
    EXPECT_EQ(m_dut.getLastError(), "The specified power level value (-21) exceeds minimum allowed value (-20)");
}

TEST_F(DutWithClientMockTest, setTransmitPowerLevelShouldSucceedAndDoNothingIfNoChangeIsRequired)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    uint8_t powerLevel = 6;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        for (uint8_t i = 0; i < dut::maxNumTxAntennas; i++) {
            if (enabledTxAntennaMask.isBitSet(i)) {
                EXPECT_CALL(*m_client, setTransmitPowerLevel(i, spectrumBandwidth, powerLevel));
            }
        }
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setTransmitPowerLevel(powerLevel));

    ASSERT_TRUE(m_dut.setTransmitPowerLevel(powerLevel));
}

TEST_F(DutWithClientMockTest, setTransmitPowerLevelShouldSucceedWithAutoPower)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    // Auto-power (power is calculated by firmware)
    uint8_t powerLevel = 0xff;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        for (uint8_t i = 0; i < dut::maxNumTxAntennas; i++) {
            if (enabledTxAntennaMask.isBitSet(i)) {
                EXPECT_CALL(*m_client, setTransmitPowerLevel(i, spectrumBandwidth, powerLevel));
            }
        }
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setTransmitPowerLevel(powerLevel));
}

TEST_F(DutWithClientMockTest, setTransmitPowerVectorShouldFailIfTransmissionNotIdle)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AC;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
    uint8_t nss = 1;
    dut::Gi gi = dut::Gi::GI_0_8_US;
    dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
    float rateMbps = 0.0f;

    uint16_t repetitions = UINT16_MAX;
    uint32_t packetLength = 4000;
    bool longData = false;
    bool beamforming = false;

    dut::TransmitPowerVector_t transmitPowerVector {};

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, setRate(phyMode, signalBandwidth, dut::Rates::getMcsIndex(phyMode, mcs, nss), nss, gi, ltf));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(antenna, spectrumBandwidth, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, _));
        EXPECT_CALL(*m_client, stopTx());
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps));
    ASSERT_TRUE(m_dut.startTx(repetitions, packetLength, longData, beamforming));

    ASSERT_FALSE(m_dut.setTransmitPowerVector(transmitPowerVector));
    EXPECT_EQ(m_dut.getLastError(), "Transmission state is not idle");

    ASSERT_TRUE(m_dut.stopTx());
}

TEST_F(DutWithClientMockTest, setTssiS2dParamsShouldFailWithInvalidAntenna)
{
    uint8_t antenna = dut::maxNumTxAntennas;
    uint8_t region = 1;
    uint8_t powerThreshold = 4;
    uint8_t gain = 5;
    uint8_t offset = 7;

    EXPECT_FALSE(m_dut.setTssiS2dParams(antenna, region, powerThreshold, gain, offset));
    EXPECT_EQ(m_dut.getLastError(), "Invalid antenna index value (" + dut::toString(antenna) + "). Valid values range from 0 to " + dut::toString(dut::maxNumTxAntennas - 1));
}

TEST_F(DutWithClientMockTest, setTssiS2dParamsShouldFailWithInvalidRegion)
{
    uint8_t antenna = 1;
    uint8_t region = dut::maxNumTxRegions + 1;
    uint8_t powerThreshold = 4;
    uint8_t gain = 5;
    uint8_t offset = 7;

    EXPECT_FALSE(m_dut.setTssiS2dParams(antenna, region, powerThreshold, gain, offset));
    EXPECT_EQ(m_dut.getLastError(), "Invalid region value (" + dut::toString(region) + "). Valid values range from 1 to " + dut::toString(dut::maxNumTxRegions));
}

TEST_F(DutWithClientMockTest, setTssiS2dParamsShouldFailWithInvalidPowerThreshold)
{
    uint8_t antenna = 1;
    uint8_t region = 1;
    uint8_t powerThreshold = dut::maxPowerLevelIndex + 1;
    uint8_t gain = 5;
    uint8_t offset = 7;

    EXPECT_FALSE(m_dut.setTssiS2dParams(antenna, region, powerThreshold, gain, offset));
    EXPECT_EQ(m_dut.getLastError(), "The specified power threshold value (" + dut::toString(powerThreshold) + ") exceeds maximum allowed value (" + dut::toString(dut::maxPowerLevelIndex) + ")");
}

TEST_F(DutWithClientMockTest, setTssiS2dParamsShouldFailIfRateNotSet)
{
    uint8_t antenna = 1;
    uint8_t region = 1;
    uint8_t powerThreshold = 4;
    uint8_t gain = 5;
    uint8_t offset = 7;

    EXPECT_FALSE(m_dut.setTssiS2dParams(antenna, region, powerThreshold, gain, offset));
    EXPECT_EQ(m_dut.getLastError(), "Rate (and signal bandwidth) not set");
}

TEST_F(DutWithClientMockTest, setTransmitPowerTableOffsetShouldFailWithInvalidAntenna)
{
    uint8_t antenna = dut::maxNumTxAntennas;
    dut::Bandwidth bandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    int16_t offset = 0;

    EXPECT_FALSE(m_dut.setTransmitPowerTableOffset(antenna, bandwidth, offset));
    EXPECT_EQ(m_dut.getLastError(), "Invalid antenna index value (" + dut::toString(antenna) + "). Valid values range from 0 to " + dut::toString(dut::maxNumTxAntennas - 1));
}

TEST_F(DutWithClientMockTest, setTransmitPowerTableOffsetShouldFailWithInvalidOffset)
{
    uint8_t antenna = 0;
    dut::Bandwidth bandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    int16_t offset = dut::transmitGainTableSize + 1;

    EXPECT_FALSE(m_dut.setTransmitPowerTableOffset(antenna, bandwidth, offset));
    EXPECT_EQ(m_dut.getLastError(), "The specified offset value (193) exceeds maximum allowed value (128)");

    offset = -(dut::transmitGainTableSize + 1);

    EXPECT_FALSE(m_dut.setTransmitPowerTableOffset(antenna, bandwidth, offset));
    EXPECT_EQ(m_dut.getLastError(), "The specified offset value (-193) exceeds minimum allowed value (-128)");
}

TEST_F(DutWithClientMockTest, setZwdfsConfigurationShouldFailIfZwdfsNotAvailable)
{
    uint8_t zwdfsChannel = 100;
    dut::Bandwidth zwdfsBandwidth = dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY;
    dut::Bandwidth radarDetectionBandwidth = dut::Bandwidth::BANDWIDTH_INVALID;

    ASSERT_FALSE(m_dut.setZwdfsConfiguration(zwdfsChannel, zwdfsBandwidth, radarDetectionBandwidth));
    EXPECT_EQ(m_dut.getLastError(), "The ZWDFS feature is not available");
}

#ifdef ALLOW_ILLEGAL_CHANNELS
TEST_F(DutWithClientMockTestGen7, setZwdfsConfigurationShouldFailWithInvalidChannel)
{
    uint8_t zwdfsChannel = 120;
    dut::Bandwidth zwdfsBandwidth = dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY;
    dut::Bandwidth radarDetectionBandwidth = dut::Bandwidth::BANDWIDTH_INVALID;

    ASSERT_FALSE(m_dut.setZwdfsConfiguration(zwdfsChannel, zwdfsBandwidth, radarDetectionBandwidth));
    EXPECT_EQ(m_dut.getLastError(), "Invalid ZWDFS channel (120)");
}
#endif

TEST_F(DutWithClientMockTestGen7, setZwdfsConfigurationShouldFailWithInvalidBandwidth)
{
    uint8_t zwdfsChannel = 100;
    dut::Bandwidth zwdfsBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    dut::Bandwidth radarDetectionBandwidth = dut::Bandwidth::BANDWIDTH_INVALID;

    ASSERT_FALSE(m_dut.setZwdfsConfiguration(zwdfsChannel, zwdfsBandwidth, radarDetectionBandwidth));
    EXPECT_EQ(m_dut.getLastError(), "Invalid ZWDFS bandwidth (20MHz)");
}

TEST_F(DutWithClientMockTestGen7, setZwdfsConfigurationShouldFailWithInvalidRadarDetectionBandwidth)
{
    uint8_t zwdfsChannel = 100;
    dut::Bandwidth zwdfsBandwidth = dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY;
    dut::Bandwidth radarDetectionBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;

    ASSERT_FALSE(m_dut.setZwdfsConfiguration(zwdfsChannel, zwdfsBandwidth, radarDetectionBandwidth));
    EXPECT_EQ(m_dut.getLastError(), "Invalid ZWDFS radar detection bandwidth (20MHz)");
}

TEST_F(DutWithClientMockTestGen7, setZwdfsConfigurationShouldSucceedWithZwdfsAntennaEnabledAfterConfiguring)
{
    dut::AntennaMask enabledRxAntennaMask = rxAntennaMaskGen7 | zwdfsAntennaMask;
    uint8_t zwdfsChannel = 116;
    dut::Bandwidth zwdfsBandwidth = dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY;
    dut::Bandwidth radarDetectionBandwidth = dut::Bandwidth::BANDWIDTH_INVALID;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setZwdfsEnabled(true, zwdfsChannel, zwdfsBandwidth, radarDetectionBandwidth, dut::defaultRegulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
    }

    ASSERT_TRUE(m_dut.setZwdfsConfiguration(zwdfsChannel, zwdfsBandwidth, radarDetectionBandwidth));
    ASSERT_TRUE(m_dut.setEnabledRxAntennaMask(enabledRxAntennaMask));
}

TEST_F(DutWithClientMockTestGen7, setZwdfsConfigurationShouldSucceedWithZwdfsAntennaEnabledAndDoNothingIfNoChangeIsRequired)
{
    dut::AntennaMask enabledRxAntennaMask = rxAntennaMaskGen7 | zwdfsAntennaMask;
    uint8_t zwdfsChannel = 116;
    dut::Bandwidth zwdfsBandwidth = dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY;
    dut::Bandwidth radarDetectionBandwidth = dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setZwdfsEnabled(true, dut::defaultZwdfsChannel, dut::defaultZwdfsBandwidth, dut::defaultRadarDetectionBandwidth, dut::defaultRegulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, setZwdfsEnabled(true, zwdfsChannel, zwdfsBandwidth, radarDetectionBandwidth, dut::defaultRegulationType));
    }

    ASSERT_TRUE(m_dut.setEnabledRxAntennaMask(enabledRxAntennaMask));
    ASSERT_TRUE(m_dut.setZwdfsConfiguration(zwdfsChannel, zwdfsBandwidth, radarDetectionBandwidth));

    ASSERT_TRUE(m_dut.setEnabledRxAntennaMask(enabledRxAntennaMask));
    ASSERT_TRUE(m_dut.setZwdfsConfiguration(zwdfsChannel, zwdfsBandwidth, radarDetectionBandwidth));
}

TEST_F(DutWithClientMockTest, startCalibrationShouldFailIfInvalidMaskType)
{
    dut::StartCalibrationParams_t params = {};
    uint8_t status = {};
    params.maskType = 2;

    EXPECT_FALSE(m_dut.startCalibration(params, status));
    EXPECT_EQ(m_dut.getLastError(), "Invalid maskType (2). Maximum allowed value is 1");
}

TEST_F(DutWithClientMockTest, startCalibrationShouldFailIfInvalidCalibrationType)
{
    dut::StartCalibrationParams_t params = {};
    uint8_t status = {};
    params.type = 2;

    EXPECT_FALSE(m_dut.startCalibration(params, status));
    EXPECT_EQ(m_dut.getLastError(), "Invalid type (2). Maximum allowed value is 1");
}

TEST_F(DutWithClientMockTest, startCwShouldFailIfInvalidAmplitude)
{
    int8_t amplitude = -3;
    int16_t tone = 1;

    EXPECT_FALSE(m_dut.startCw(amplitude, tone));
    EXPECT_EQ(m_dut.getLastError(), "Invalid amplitude (-3). Minimum allowed value is -2");

    amplitude = 3;
    EXPECT_FALSE(m_dut.startCw(amplitude, tone));
    EXPECT_EQ(m_dut.getLastError(), "Invalid amplitude (3). Maximum allowed value is 2");
}

TEST_F(DutWithClientMockTest, startCwShouldFailIfInvalidTone)
{
    int8_t amplitude = 1;
    int16_t tone = -513;

    EXPECT_FALSE(m_dut.startCw(amplitude, tone));
    EXPECT_EQ(m_dut.getLastError(), "Invalid tone (-513). Minimum allowed value is -512");

    tone = 512;
    EXPECT_FALSE(m_dut.startCw(amplitude, tone));
    EXPECT_EQ(m_dut.getLastError(), "Invalid tone (512). Maximum allowed value is 511");
}

TEST_F(DutWithClientMockTest, startCwShouldFailIfChannelNotSet)
{
    int8_t amplitude = 1;
    int16_t tone = 1;

    EXPECT_FALSE(m_dut.startCw(amplitude, tone));
    EXPECT_EQ(m_dut.getLastError(), "Channel not set");
}

TEST_F(DutWithClientMockTest, startCwShouldFailIfNoTxAntennaIsSet)
{
    dut::AntennaMask enabledTxAntennaMask = 0x00;

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    int8_t amplitude = 1;
    int16_t tone = 1;

    EXPECT_FALSE(m_dut.startCw(amplitude, tone));
    EXPECT_EQ(m_dut.getLastError(), "Illegal antenna selection (at least one TX antenna must be selected)");
}

TEST_F(DutWithClientMockTest, startTxShouldFailIfChannelNotSet)
{
    uint16_t repetitions = UINT16_MAX;
    uint32_t packetLength = 4000;
    bool longData = false;
    bool beamforming = false;

    EXPECT_FALSE(m_dut.startTx(repetitions, packetLength, longData, beamforming));
    EXPECT_EQ(m_dut.getLastError(), "Channel not set");
}

TEST_F(DutWithClientMockTest, startTxShouldFailIfInvalidPacketLength)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    uint16_t repetitions = UINT16_MAX;
    uint32_t packetLength = dut::maxDefaultPacketLength + 1;
    bool longData = false;
    bool beamforming = false;

    EXPECT_FALSE(m_dut.startTx(repetitions, packetLength, longData, beamforming));
    EXPECT_EQ(m_dut.getLastError(), "Invalid packet length: the specified value (4001) exceeds maximum allowed value (4000) for current PHY mode");
}

TEST_F(DutWithClientMockTest, startTxShouldFailIfInvalidPacketLengthAndLongData)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    uint16_t repetitions = UINT16_MAX;
    uint32_t packetLength = 0x3fffff + 1;
    bool longData = true;
    bool beamforming = false;

    EXPECT_FALSE(m_dut.startTx(repetitions, packetLength, longData, beamforming));
    EXPECT_EQ(m_dut.getLastError(), "Invalid packet length: the specified value (4194304) exceeds maximum allowed value (4194303) for current PHY mode");
}

TEST_F(DutWithClientMockTest, startTxShouldFailIfNoTxAntennaIsSet)
{
    dut::AntennaMask enabledTxAntennaMask = 0x00;

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    uint16_t repetitions = UINT16_MAX;
    uint32_t packetLength = 4000;
    bool longData = false;
    bool beamforming = false;

    EXPECT_FALSE(m_dut.startTx(repetitions, packetLength, longData, beamforming));
    EXPECT_EQ(m_dut.getLastError(), "Illegal antenna selection (at least one TX antenna must be selected)");
}

TEST_F(DutWithClientMockTest, startTxShouldFailIfRateNotSet)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    uint16_t repetitions = UINT16_MAX;
    uint32_t packetLength = 4000;
    bool longData = false;
    bool beamforming = false;

    EXPECT_FALSE(m_dut.startTx(repetitions, packetLength, longData, beamforming));
    EXPECT_EQ(m_dut.getLastError(), "Transmission rate not set");
}

TEST_F(DutWithClientMockTest, startRxCalibrationShouldFailIfChannelNotSet)
{
    EXPECT_FALSE(m_dut.startRxCalibration());
    EXPECT_EQ(m_dut.getLastError(), "Channel not set");
}

TEST_F(DutWithClientMockTestGen7, startTxShouldFailForLdpcWith11bPhyMode)
{
    uint16_t repetitions = 100;
    uint32_t packetLength = 100;
    bool longData = false;
    bool beamforming = false;

    setupChannelAndRate(dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Bandwidth::BANDWIDTH_TWENTY,
        6, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP,
        dut::Mcs::MCS_80211b_1MBPS_LONG, 1, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT);

    EXPECT_FALSE(m_dut.startTx(repetitions, packetLength, longData, beamforming, dut::CodingType::CODING_TYPE_LDPC));
    EXPECT_EQ(m_dut.getLastError(), "LDPC is not supported for 11b PHY mode");
}

TEST_F(DutWithClientMockTestGen7, startTxShouldFailForLdpcWith11aPhyMode)
{
    uint16_t repetitions = 100;
    uint32_t packetLength = 100;
    bool longData = false;
    bool beamforming = false;

    setupChannelAndRate(dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Bandwidth::BANDWIDTH_TWENTY,
        36, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP,
        dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT);

    EXPECT_FALSE(m_dut.startTx(repetitions, packetLength, longData, beamforming, dut::CodingType::CODING_TYPE_LDPC));
    EXPECT_EQ(m_dut.getLastError(), "LDPC is not supported for 11a/g PHY modes (only BCC is supported)");
}

TEST_F(DutWithClientMockTestGen7, startTxShouldFailForLdpcWith11gPhyMode)
{
    uint16_t repetitions = 100;
    uint32_t packetLength = 100;
    bool longData = false;
    bool beamforming = false;

    setupChannelAndRate(dut::PhyMode::PHY_MODE_G, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Bandwidth::BANDWIDTH_TWENTY,
        6, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP,
        dut::Mcs::MCS_BPSK_12, 1, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT);

    EXPECT_FALSE(m_dut.startTx(repetitions, packetLength, longData, beamforming, dut::CodingType::CODING_TYPE_LDPC));
    EXPECT_EQ(m_dut.getLastError(), "LDPC is not supported for 11a/g PHY modes (only BCC is supported)");
}

TEST_F(DutWithClientMockTestGen7, startTxShouldFailForBccWith11axAndHighBandwidth)
{
    uint16_t repetitions = 100;
    uint32_t packetLength = 100;
    bool longData = false;
    bool beamforming = false;

    setupChannelAndRate(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_FOURTY, dut::Bandwidth::BANDWIDTH_FOURTY,
        36, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP,
        dut::Mcs::MCS_QPSK_12, 1, dut::Gi::GI_0_8_US, dut::Ltf::LTF_MEDIUM);

    EXPECT_FALSE(m_dut.startTx(repetitions, packetLength, longData, beamforming, dut::CodingType::CODING_TYPE_BCC));
    EXPECT_EQ(m_dut.getLastError(), "BCC is not supported for 11ax/be with bandwidth greater than 20MHz");
}

TEST_F(DutWithClientMockTestGen7, startTxShouldFailForBccWith11axAndHighMcs)
{
    uint16_t repetitions = 100;
    uint32_t packetLength = 100;
    bool longData = false;
    bool beamforming = false;

    setupChannelAndRate(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Bandwidth::BANDWIDTH_TWENTY,
        36, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP,
        dut::Mcs::MCS_1024QAM_56, 1, dut::Gi::GI_0_8_US, dut::Ltf::LTF_MEDIUM);

    EXPECT_FALSE(m_dut.startTx(repetitions, packetLength, longData, beamforming, dut::CodingType::CODING_TYPE_BCC));
    EXPECT_EQ(m_dut.getLastError(), "BCC is not supported for 11ax/be with MCS greater than 9 (QAM256)");
}

TEST_F(DutWithClientMockTestGen7, startTxShouldFailForBccWith11beAndHighBandwidth)
{
    uint16_t repetitions = 100;
    uint32_t packetLength = 100;
    bool longData = false;
    bool beamforming = false;

    setupChannelAndRate(dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Bandwidth::BANDWIDTH_EIGHTY,
        36, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP,
        dut::Mcs::MCS_QPSK_12, 1, dut::Gi::GI_0_8_US, dut::Ltf::LTF_LONG);

    EXPECT_FALSE(m_dut.startTx(repetitions, packetLength, longData, beamforming, dut::CodingType::CODING_TYPE_BCC));
    EXPECT_EQ(m_dut.getLastError(), "BCC is not supported for 11ax/be with bandwidth greater than 20MHz");
}

TEST_F(DutWithClientMockTestGen7, startTxShouldSucceedWith11bBccCoding)
{
    uint16_t repetitions = 100;
    uint32_t packetLength = 100;
    bool longData = false;
    bool beamforming = false;

    setupChannelAndRate(dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Bandwidth::BANDWIDTH_TWENTY,
        6, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP,
        dut::Mcs::MCS_80211b_1MBPS_LONG, 1, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT);

    EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, false)); // BCC = false for LDPC flag
    EXPECT_CALL(*m_client, stopTx());

    EXPECT_TRUE(m_dut.startTx(repetitions, packetLength, longData, beamforming, dut::CodingType::CODING_TYPE_BCC));
    EXPECT_TRUE(m_dut.stopTx());
}

TEST_F(DutWithClientMockTestGen7, startTxShouldSucceedWith11nLdpcCoding)
{
    uint16_t repetitions = 100;
    uint32_t packetLength = 100;
    bool longData = false;
    bool beamforming = false;

    setupChannelAndRate(dut::PhyMode::PHY_MODE_N_5, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Bandwidth::BANDWIDTH_TWENTY,
        36, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP,
        dut::Mcs::MCS_QPSK_12, 1, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT);

    EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, true)); // LDPC = true
    EXPECT_CALL(*m_client, stopTx());

    EXPECT_TRUE(m_dut.startTx(repetitions, packetLength, longData, beamforming, dut::CodingType::CODING_TYPE_LDPC));
    EXPECT_TRUE(m_dut.stopTx());
}

TEST_F(DutWithClientMockTestGen7, startTxShouldSucceedWith11axDefaultCoding)
{
    uint16_t repetitions = 100;
    uint32_t packetLength = 100;
    bool longData = false;
    bool beamforming = false;

    setupChannelAndRate(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Bandwidth::BANDWIDTH_TWENTY,
        36, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP,
        dut::Mcs::MCS_QPSK_12, 1, dut::Gi::GI_0_8_US, dut::Ltf::LTF_MEDIUM);

    EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, true)); // Default for 11ax is LDPC = true
    EXPECT_CALL(*m_client, stopTx());

    EXPECT_TRUE(m_dut.startTx(repetitions, packetLength, longData, beamforming, dut::CodingType::CODING_TYPE_AUTO));
    EXPECT_TRUE(m_dut.stopTx());
}

TEST_F(DutWithClientMockTestGen7, startTxShouldSucceedWith11axBccCodingLowMcs)
{
    uint16_t repetitions = 100;
    uint32_t packetLength = 100;
    bool longData = false;
    bool beamforming = false;

    setupChannelAndRate(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Bandwidth::BANDWIDTH_TWENTY,
        36, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP,
        dut::Mcs::MCS_256QAM_56, 1, dut::Gi::GI_0_8_US, dut::Ltf::LTF_MEDIUM); // MCS 9 (valid for BCC)

    EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, false)); // BCC = false for LDPC flag
    EXPECT_CALL(*m_client, stopTx());

    EXPECT_TRUE(m_dut.startTx(repetitions, packetLength, longData, beamforming, dut::CodingType::CODING_TYPE_BCC));
    EXPECT_TRUE(m_dut.stopTx());
}

// PARAMETERIZED TEST FOR ALL VALID PHY MODE AND CODING TYPE COMBINATIONS

struct PhyModeAndCodingTestParams {
    dut::PhyMode phyMode;
    dut::Bandwidth bandwidth;
    uint8_t channel;
    dut::Mcs mcs;
    dut::Gi gi;
    dut::Ltf ltf;
    dut::CodingType codingType;
    bool expectedLdpcFlag;
    const char* description;
};

class DutStartTxPhyModeAndCodingTest : public DutWithClientMockTestGen7,
                                       public ::testing::WithParamInterface<PhyModeAndCodingTestParams> {
};

TEST_P(DutStartTxPhyModeAndCodingTest, startTxShouldSucceedWithValidPhyModeAndCodingCombinations)
{
    uint16_t repetitions = 100;
    uint32_t packetLength = 100;
    bool longData = false;
    bool beamforming = false;

    auto params = GetParam();

    setupChannelAndRate(params.phyMode, params.bandwidth, params.bandwidth,
        params.channel, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP,
        params.mcs, 1, params.gi, params.ltf);

    EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, params.expectedLdpcFlag));
    EXPECT_CALL(*m_client, stopTx());

    EXPECT_TRUE(m_dut.startTx(repetitions, packetLength, longData, beamforming, params.codingType));
    EXPECT_TRUE(m_dut.stopTx());
}

INSTANTIATE_TEST_SUITE_P(ValidPhyModeAndCodingCombinations, DutStartTxPhyModeAndCodingTest,
    ::testing::Values(
        // 11b - only BCC supported
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_TWENTY, 6,
            dut::Mcs::MCS_80211b_1MBPS_LONG, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_AUTO, false, "Mode11b_Default_BCC" },
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_B, dut::Bandwidth::BANDWIDTH_TWENTY, 6,
            dut::Mcs::MCS_80211b_2MBPS_LONG, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_BCC, false, "Mode11b_Explicit_BCC" },

        // 11a - only BCC supported
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, 36,
            dut::Mcs::MCS_BPSK_12, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_AUTO, false, "Mode11a_Default_BCC" },
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_A, dut::Bandwidth::BANDWIDTH_TWENTY, 36,
            dut::Mcs::MCS_QPSK_12, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_BCC, false, "Mode11a_Explicit_BCC" },

        // 11g - only BCC supported
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_G, dut::Bandwidth::BANDWIDTH_TWENTY, 6,
            dut::Mcs::MCS_BPSK_12, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_AUTO, false, "Mode11g_Default_BCC" },
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_G, dut::Bandwidth::BANDWIDTH_TWENTY, 6,
            dut::Mcs::MCS_QPSK_12, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_BCC, false, "Mode11g_Explicit_BCC" },

        // 11n 5GHz - both BCC and LDPC supported, defaults to BCC
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_N_5, dut::Bandwidth::BANDWIDTH_TWENTY, 36,
            dut::Mcs::MCS_QPSK_12, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_AUTO, false, "Mode11n5GHz_Default_BCC" },
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_N_5, dut::Bandwidth::BANDWIDTH_TWENTY, 36,
            dut::Mcs::MCS_16QAM_12, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_BCC, false, "Mode11n5GHz_Explicit_BCC" },
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_N_5, dut::Bandwidth::BANDWIDTH_TWENTY, 36,
            dut::Mcs::MCS_64QAM_23, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_LDPC, true, "Mode11n5GHz_Explicit_LDPC" },

        // 11n 2.4GHz - both BCC and LDPC supported, defaults to BCC
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, 6,
            dut::Mcs::MCS_QPSK_12, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_AUTO, false, "Mode11n24GHz_Default_BCC" },
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, 6,
            dut::Mcs::MCS_16QAM_12, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_BCC, false, "Mode11n24GHz_Explicit_BCC" },
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_N_2_4, dut::Bandwidth::BANDWIDTH_TWENTY, 6,
            dut::Mcs::MCS_64QAM_23, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_LDPC, true, "Mode11n24GHz_Explicit_LDPC" },

        // 11ac - both BCC and LDPC supported, defaults to BCC
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_TWENTY, 36,
            dut::Mcs::MCS_QPSK_12, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_AUTO, false, "Mode11ac_Default_BCC" },
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_TWENTY, 36,
            dut::Mcs::MCS_16QAM_12, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_BCC, false, "Mode11ac_Explicit_BCC" },
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_TWENTY, 36,
            dut::Mcs::MCS_64QAM_23, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT,
            dut::CodingType::CODING_TYPE_LDPC, true, "Mode11ac_Explicit_LDPC" },

        // 11ax - defaults to LDPC, BCC only for 20MHz and MCS <= 9
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, 36,
            dut::Mcs::MCS_QPSK_12, dut::Gi::GI_0_8_US, dut::Ltf::LTF_MEDIUM,
            dut::CodingType::CODING_TYPE_AUTO, true, "Mode11ax_Default_LDPC" },
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, 36,
            dut::Mcs::MCS_256QAM_56, dut::Gi::GI_0_8_US, dut::Ltf::LTF_MEDIUM,
            dut::CodingType::CODING_TYPE_BCC, false, "Mode11ax_Explicit_BCC_MCS9" },
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, 36,
            dut::Mcs::MCS_1024QAM_56, dut::Gi::GI_0_8_US, dut::Ltf::LTF_MEDIUM,
            dut::CodingType::CODING_TYPE_LDPC, true, "Mode11ax_Explicit_LDPC" },

        // 11be - defaults to LDPC, BCC only for 20MHz and MCS <= 9
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_TWENTY, 36,
            dut::Mcs::MCS_QPSK_12, dut::Gi::GI_0_8_US, dut::Ltf::LTF_LONG,
            dut::CodingType::CODING_TYPE_AUTO, true, "Mode11be_Default_LDPC" },
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_TWENTY, 36,
            dut::Mcs::MCS_256QAM_56, dut::Gi::GI_0_8_US, dut::Ltf::LTF_LONG,
            dut::CodingType::CODING_TYPE_BCC, false, "Mode11be_Explicit_BCC_MCS9" },
        PhyModeAndCodingTestParams { dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_TWENTY, 36,
            dut::Mcs::MCS_1024QAM_56, dut::Gi::GI_0_8_US, dut::Ltf::LTF_LONG,
            dut::CodingType::CODING_TYPE_LDPC, true, "Mode11be_Explicit_LDPC" }),
    [](const ::testing::TestParamInfo<PhyModeAndCodingTestParams>& info) {
        return info.param.description;
    });

TEST_F(DutWithClientMockTestGen7, startTxShouldSucceedWithValidBeamformingHeader)
{
    uint16_t repetitions = 100;
    uint32_t packetLength = 100;
    bool longData = false;
    bool beamforming = true;

    // Configure DUT for HE 80MHz
    setupChannelAndRate(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Bandwidth::BANDWIDTH_EIGHTY,
        36, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP,
        dut::Mcs::MCS_QPSK_12, 1, dut::Gi::GI_0_8_US, dut::Ltf::LTF_MEDIUM);

    // Extract valid 80MHz beamforming header from resource
    EmbeddedResource headerResource(IDR_BEAMFORMINGMATRIX_WAVE700_HE_SU_80MHZ_HEADER, BEAMFORMINGMATRIX);
    TemporaryFile tempHeaderFile(headerResource.getData(), headerResource.getSize());
    auto headerValues = dut::beamforming_utils::readHexStringFile(tempHeaderFile.getFilename());
    ASSERT_EQ(headerValues.size(), 2);
    uint32_t validHeader = headerValues[0];

    // Mock hardware to return matching 80MHz header
    EXPECT_CALL(*m_client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave700::primaryBfHeaderAddress, _, sizeof(uint32_t)))
        .WillOnce(Invoke([validHeader](dut::ChipModule, size_t, uint8_t* data, size_t) {
            *reinterpret_cast<uint32_t*>(data) = validHeader;
        }));

    // Expect startTx to succeed with valid header
    EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, true));
    EXPECT_CALL(*m_client, stopTx());

    EXPECT_TRUE(m_dut.startTx(repetitions, packetLength, longData, beamforming));
    EXPECT_TRUE(m_dut.stopTx());
}

TEST_F(DutWithClientMockTestGen7, startTxShouldFailWithInvalidBeamformingHeader)
{
    uint16_t repetitions = 100;
    uint32_t packetLength = 100;
    bool longData = false;
    bool beamforming = true;

    // Configure DUT for HE 80MHz
    setupChannelAndRate(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Bandwidth::BANDWIDTH_EIGHTY,
        36, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP,
        dut::Mcs::MCS_QPSK_12, 1, dut::Gi::GI_0_8_US, dut::Ltf::LTF_MEDIUM);

    // Extract 20MHz beamforming header from resource (invalid for 80MHz rate)
    EmbeddedResource invalidHeaderResource(IDR_BEAMFORMINGMATRIX_WAVE700_HE_SU_20MHZ_HEADER, BEAMFORMINGMATRIX);
    TemporaryFile tempInvalidHeaderFile(invalidHeaderResource.getData(), invalidHeaderResource.getSize());
    auto invalidHeaderValues = dut::beamforming_utils::readHexStringFile(tempInvalidHeaderFile.getFilename());
    ASSERT_EQ(invalidHeaderValues.size(), 2);
    uint32_t invalidHeader = invalidHeaderValues[0];

    // Mock hardware to return mismatched 20MHz header
    EXPECT_CALL(*m_client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave700::primaryBfHeaderAddress, _, sizeof(uint32_t)))
        .WillOnce(Invoke([invalidHeader](dut::ChipModule, size_t, uint8_t* data, size_t) {
            *reinterpret_cast<uint32_t*>(data) = invalidHeader;
        }));

    // Expect startTx to fail due to beamforming header validation
    EXPECT_FALSE(m_dut.startTx(repetitions, packetLength, longData, beamforming));

    // Verify the error message contains beamforming validation details
    std::string errorMsg = m_dut.getLastError();
    EXPECT_TRUE(
        (errorMsg.find("Bandwidth mismatch") != std::string::npos) || (errorMsg.find("PHY mode mismatch") != std::string::npos) || (errorMsg.find("validation failed") != std::string::npos));
}

TEST_F(DutWithClientMockTest, startRxCalibrationShouldFailIfNoRxAntennaIsSet)
{
    dut::AntennaMask enabledTxAntennaMask = 0x00;

    dut::AntennaMask enabledRxAntennaMask = 0x00;

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setEnabledRxAntennaMask(enabledRxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    EXPECT_FALSE(m_dut.startRxCalibration());
    EXPECT_EQ(m_dut.getLastError(), "Illegal antenna selection (at least one RX antenna must be selected)");
}

TEST_F(DutWithClientMockTest, startRxCalibrationShouldSucceedAndDoNothingIfNoChangeIsRequired)
{
    dut::AntennaMask enabledTxAntennaMask = 0x00;

    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));

        EXPECT_CALL(*m_client, setRiscModeEnabled(false));
        EXPECT_CALL(*m_client, startRxCalibration());
        EXPECT_CALL(*m_client, stopRxCalibration());
        EXPECT_CALL(*m_client, setRiscModeEnabled(true));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    ASSERT_TRUE(m_dut.startRxCalibration());
    ASSERT_TRUE(m_dut.startRxCalibration());
    ASSERT_TRUE(m_dut.stopRxCalibration());
}

TEST_F(DutWithClientMockTest, stopRxCalibrationShouldSucceedAndDoNothingIfNoChangeIsRequired)
{
    ASSERT_TRUE(m_dut.stopRxCalibration());
}

TEST_F(DutWithClientMockTest, startRxPerShouldFailIfChannelNotSet)
{
    uint32_t packetLimit = 10000;

    ASSERT_FALSE(m_dut.startRxPer(packetLimit));
    EXPECT_EQ(m_dut.getLastError(), "Channel not set");
}

TEST_F(DutWithClientMockTest, startRxPerShouldFailIfRunTwice)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);
    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    uint32_t packetLimit = 10000;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, startRxPer(_, _));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.startRxPer(packetLimit));

    EXPECT_FALSE(m_dut.startRxPer(packetLimit));
    EXPECT_EQ(m_dut.getLastError(), "Rx PER measure already running");
}

TEST_F(DutWithClientMockTest, startRxPerShouldFailIfRxMeasureRunning)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);
    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    uint32_t numCaptures = 1;
    uint32_t captureInterval = 1000;
    bool disabled = false;

    uint32_t packetLimit = 10000;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, rxMeasure(_, _, _, phyMode));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.rxMeasure(numCaptures, captureInterval, disabled));

    EXPECT_FALSE(m_dut.startRxPer(packetLimit));
    EXPECT_EQ(m_dut.getLastError(), "Rx Measure running");
}

TEST_F(DutWithClientMockTest, startRxPerShouldFailIfRxAntennaMaskZero)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);
    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    uint32_t packetLimit = 10000;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(_));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.setEnabledRxAntennaMask(0x00));

    EXPECT_FALSE(m_dut.startRxPer(packetLimit));
    EXPECT_EQ(m_dut.getLastError(), "Illegal antenna selection (at least one RX antenna must be selected)");
}

TEST_F(DutWithClientMockTest, stopRxPerShouldFailIfRxAntennaMaskZero)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);
    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    bool calcRxPer = false;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    uint32_t packetLimit = 10000;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
        EXPECT_CALL(*m_client, startRxPer(_, _));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(_));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));
    ASSERT_TRUE(m_dut.startRxPer(packetLimit));
    ASSERT_TRUE(m_dut.setEnabledRxAntennaMask(0x00));

    EXPECT_FALSE(m_dut.stopRxPer(calcRxPer));
    EXPECT_EQ(m_dut.getLastError(), "Illegal antenna selection (at least one RX antenna must be selected)");
}

TEST_F(DutWithClientMockTest, stopRxPerShouldFailIfRxPerPacketLimitNotSet)
{
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);
    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
    uint8_t channel = 36;
    uint8_t primaryChannelIndex = 0;
    dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

    bool calcRxPer = false;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(channel, primaryChannelIndex, spectrumBandwidth, enabledTxAntennaMask, regulationType));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType));

    EXPECT_FALSE(m_dut.stopRxPer(calcRxPer));
    EXPECT_EQ(m_dut.getLastError(), "Rx PER measure not running");
}

TEST_F(DutWithClientMockTest, writeCalibrationFileShouldFailWithInvalidSize)
{
    dut::NvMemoryType type = dut::NvMemoryType::MEMORY_TYPE_FLASH;
    size_t size = static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_2K) + 1;

    EXPECT_FALSE(m_dut.writeCalibrationFile(type, static_cast<dut::NvMemorySize>(size)));
    EXPECT_EQ(m_dut.getLastError(), "Invalid NVM size (2049)");
}

TEST_F(DutWithClientMockTest, writeCalibrationFileShouldFailWithInvalidType)
{
    dut::NvMemoryType type = dut::NvMemoryType::MEMORY_TYPE_EFUSE;
    dut::NvMemorySize size = dut::NvMemorySize::MEMORY_SIZE_BYTES_1K;

    EXPECT_FALSE(m_dut.writeCalibrationFile(type, size));
    EXPECT_EQ(m_dut.getLastError(), "Invalid NVM type (EFuse)");
}

// ====================================================================================================
// BEAMFORMING MATRIX LOADING TESTS
// ====================================================================================================
// These integration tests verify the loadBeamformingMatrixFromFileSet() function behavior including:
// - File I/O operations and error handling with detailed error messages
// - Integration with BeamformingUtils functions for parsing and validation
// - Hardware routing and memory write operations for Wave600 and Wave700 chipsets
// - Support for different PHY modes (HT, VHT, HE, EHT) and bandwidths
// - Comprehensive validation functions including validateBeamformingHeaderRegister()
// - Parameterized test suites covering all WiFi standard/bandwidth combinations
// - Extensive embedded resource files for thorough test coverage
// - Robust error handling with specific error messages for various failure scenarios
//
// For detailed unit tests of beamforming utilities (parsing, validation, etc.),
// see BeamformingUtilsTest.cpp and HardwareUtilsTest.cpp

TEST_F(DutWithClientMockTest, loadBeamformingMatrixFromFileSetShouldFailIfHeaderFileNotFound)
{
    EmbeddedResource beamformingMatrixResource(IDR_BEAMFORMINGMATRIX_VHT, BEAMFORMINGMATRIX);
    TemporaryFile temporaryValuesFile(beamformingMatrixResource.getData(), beamformingMatrixResource.getSize());

    std::string headerFileName = "nonexistent-file-name";
    std::string valuesFileName = temporaryValuesFile.getFilename();

    dut::BeamformingFilePathSet_t fileSet;
    fileSet.headerFile = headerFileName.c_str();
    fileSet.valuesFile = valuesFileName.c_str();

    // Setup channel and rate
    setupChannelAndRate(dut::PhyMode::PHY_MODE_N_5, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Bandwidth::BANDWIDTH_TWENTY);

    EXPECT_FALSE(m_dut.loadBeamformingMatrixFromFileSet(fileSet));
    EXPECT_THAT(m_dut.getLastError(), ::testing::HasSubstr("Unable to open file '" + headerFileName + "' for reading"));
}

TEST_F(DutWithClientMockTest, loadBeamformingMatrixFromFileSetShouldFailIfValuesFileNotFound)
{
    EmbeddedResource headerResource(IDR_BEAMFORMINGMATRIX_WAVE600_HE_HEADER, BEAMFORMINGMATRIX);

    TemporaryFile temporaryHeaderFile(headerResource.getData(), headerResource.getSize());

    std::string headerFileName = temporaryHeaderFile.getFilename();
    std::string valuesFileName = "nonexistent-file-name";

    dut::BeamformingFilePathSet_t fileSet;
    fileSet.headerFile = headerFileName.c_str();
    fileSet.valuesFile = valuesFileName.c_str();

    // Setup channel and rate
    setupChannelAndRate(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Bandwidth::BANDWIDTH_EIGHTY);

    EXPECT_FALSE(m_dut.loadBeamformingMatrixFromFileSet(fileSet));
    EXPECT_THAT(m_dut.getLastError(), ::testing::HasSubstr("Unable to open file '" + valuesFileName + "' for reading"));
}

TEST_F(DutWithClientMockTest, loadBeamformingMatrixFromFileSetShouldFailIfInvalidHeaderLength)
{
    std::string header = "00"; // Too short
    EmbeddedResource valuesResource(IDR_BEAMFORMINGMATRIX_WAVE600_HE_PHASES, BEAMFORMINGMATRIX);

    TemporaryFile temporaryHeaderFile(reinterpret_cast<const uint8_t*>(header.c_str()), header.length());
    TemporaryFile temporaryValuesFile(valuesResource.getData(), valuesResource.getSize());

    std::string headerFileName = temporaryHeaderFile.getFilename();
    std::string valuesFileName = temporaryValuesFile.getFilename();

    dut::BeamformingFilePathSet_t fileSet;
    fileSet.headerFile = headerFileName.c_str();
    fileSet.valuesFile = valuesFileName.c_str();

    // Setup channel and rate
    setupChannelAndRate(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Bandwidth::BANDWIDTH_EIGHTY);

    EXPECT_FALSE(m_dut.loadBeamformingMatrixFromFileSet(fileSet));
    EXPECT_THAT(m_dut.getLastError(), ::testing::HasSubstr("Each line must contain exactly 8 hex characters"));
}

TEST_F(DutWithClientMockTest, loadBeamformingMatrixFromFileSetShouldFailIfInvalidBeamformingHeader)
{
    std::string header = "01000201\n04050607"; // Invalid PHY mode
    EmbeddedResource valuesResource(IDR_BEAMFORMINGMATRIX_WAVE600_HE_PHASES, BEAMFORMINGMATRIX);

    TemporaryFile temporaryHeaderFile(reinterpret_cast<const uint8_t*>(header.c_str()), header.length());
    TemporaryFile temporaryValuesFile(valuesResource.getData(), valuesResource.getSize());

    std::string headerFileName = temporaryHeaderFile.getFilename();
    std::string valuesFileName = temporaryValuesFile.getFilename();

    dut::BeamformingFilePathSet_t fileSet;
    fileSet.headerFile = headerFileName.c_str();
    fileSet.valuesFile = valuesFileName.c_str();

    // Setup channel and rate
    setupChannelAndRate(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Bandwidth::BANDWIDTH_EIGHTY);

    EXPECT_FALSE(m_dut.loadBeamformingMatrixFromFileSet(fileSet));
    EXPECT_THAT(m_dut.getLastError(), ::testing::HasSubstr("Invalid beamforming header in primary file '" + headerFileName + "'"));
}

TEST_F(DutWithClientMockTest, loadBeamformingMatrixFromFileSetShouldFailWithCorruptedHexFile)
{
    std::string corruptedHeader = "0200G203\n04050607"; // Invalid hex character 'G'
    EmbeddedResource beamformingMatrixResource(IDR_BEAMFORMINGMATRIX_WAVE600_HE_PHASES, BEAMFORMINGMATRIX);

    TemporaryFile temporaryHeaderFile(reinterpret_cast<const uint8_t*>(corruptedHeader.c_str()), corruptedHeader.length());
    TemporaryFile temporaryValuesFile(beamformingMatrixResource.getData(), beamformingMatrixResource.getSize());

    std::string headerFileName = temporaryHeaderFile.getFilename();
    std::string valuesFileName = temporaryValuesFile.getFilename();

    dut::BeamformingFilePathSet_t fileSet;
    fileSet.headerFile = headerFileName.c_str();
    fileSet.valuesFile = valuesFileName.c_str();

    // Setup channel and rate
    setupChannelAndRate(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Bandwidth::BANDWIDTH_EIGHTY);

    EXPECT_FALSE(m_dut.loadBeamformingMatrixFromFileSet(fileSet));
    EXPECT_THAT(m_dut.getLastError(), ::testing::HasSubstr("Line contains non-hex character"));
}

TEST_F(DutWithClientMockTestGen7, loadBeamformingMatrixFromFileSetShouldFailIfTransmitting)
{
    // Setup valid beamforming files
    std::vector<std::unique_ptr<TemporaryFile>> tempFiles;
    auto fileSetWithSizes = beamforming_test_helpers::createBeamformingFileSetWithSizes(tempFiles,
        IDR_BEAMFORMINGMATRIX_WAVE700_HE_SU_80MHZ_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_HE_SU_80MHZ_PHASES);

    // Setup channel and rate
    setupChannelAndRate(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Bandwidth::BANDWIDTH_EIGHTY);

    // Start transmission
    EXPECT_CALL(*m_client, startTx(1, 1000, false, false, _));
    ASSERT_TRUE(m_dut.startTx(1, 1000, false, false));

    // Now try to load beamforming matrix while transmitting - should fail
    EXPECT_FALSE(m_dut.loadBeamformingMatrixFromFileSet(fileSetWithSizes.fileSet));
    EXPECT_EQ(m_dut.getLastError(), "Cannot load beamforming matrix while transmitting, stop transmission");

    // Stop transmission and try again - should succeed
    EXPECT_CALL(*m_client, stopTx());
    ASSERT_TRUE(m_dut.stopTx());

    beamforming_test_helpers::setupWave700BeamformingExpectations(m_client, fileSetWithSizes.valuesLineCount);
    ASSERT_TRUE(m_dut.loadBeamformingMatrixFromFileSet(fileSetWithSizes.fileSet));
}

TEST_F(DutWithClientMockTest, loadBeamformingMatrixFromFileSetShouldFailIfChannelNotSet)
{
    // Setup valid beamforming files
    std::vector<std::unique_ptr<TemporaryFile>> tempFiles;
    auto fileSetWithSizes = beamforming_test_helpers::createBeamformingFileSetWithSizes(tempFiles,
        IDR_BEAMFORMINGMATRIX_WAVE600_VHT_HEADER, IDR_BEAMFORMINGMATRIX_WAVE600_VHT_PHASES);

    // Try to load beamforming matrix without setting channel first - should fail
    EXPECT_FALSE(m_dut.loadBeamformingMatrixFromFileSet(fileSetWithSizes.fileSet));
    EXPECT_EQ(m_dut.getLastError(), "Channel not set");
}

TEST_F(DutWithClientMockTest, loadBeamformingMatrixFromFileSetShouldFailIfRateNotSet)
{
    // Setup valid beamforming files
    std::vector<std::unique_ptr<TemporaryFile>> tempFiles;
    auto fileSetWithSizes = beamforming_test_helpers::createBeamformingFileSetWithSizes(tempFiles,
        IDR_BEAMFORMINGMATRIX_WAVE600_VHT_HEADER, IDR_BEAMFORMINGMATRIX_WAVE600_VHT_PHASES);

    // Setup only channel (without rate)
    uint8_t antenna = 1;
    dut::AntennaMask enabledTxAntennaMask(1 << antenna);
    dut::AntennaMask enabledRxAntennaMask = 0x00;
    ASSERT_TRUE(m_dut.getEnabledRxAntennaMask(enabledRxAntennaMask));

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, setEnabledTxAntennas(enabledTxAntennaMask));
        EXPECT_CALL(*m_client, setTransmitPowerLevel(1, dut::Bandwidth::BANDWIDTH_TWENTY, dut::defaultTransmitPowerLevel));
        EXPECT_CALL(*m_client, setTransmitPowerControlAntennaParams(antenna, _, _, _));
        EXPECT_CALL(*m_client, setRssiCalData(_, _));
        EXPECT_CALL(*m_client, setChannel(36, 0, dut::Bandwidth::BANDWIDTH_EIGHTY, enabledTxAntennaMask, dut::RegulationType::REGULATION_TYPE_FCC_SP));
        EXPECT_CALL(*m_client, setEnabledRxAntennas(enabledRxAntennaMask));
    }

    ASSERT_TRUE(m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask));
    ASSERT_TRUE(m_dut.setChannel(dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_EIGHTY, 36, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP));

    // Try to load beamforming matrix without setting rate - should fail
    EXPECT_FALSE(m_dut.loadBeamformingMatrixFromFileSet(fileSetWithSizes.fileSet));
    EXPECT_EQ(m_dut.getLastError(), "Rate not set");
}

TEST_F(DutWithClientMockTest, loadBeamformingMatrixFromFileSetShouldFailIfEmptyHeaderFile)
{
    // Create empty header file
    std::string emptyHeader = "";
    EmbeddedResource valuesResource(IDR_BEAMFORMINGMATRIX_WAVE600_VHT_PHASES, BEAMFORMINGMATRIX);

    TemporaryFile temporaryHeaderFile(reinterpret_cast<const uint8_t*>(emptyHeader.c_str()), emptyHeader.length());
    TemporaryFile temporaryValuesFile(valuesResource.getData(), valuesResource.getSize());

    std::string headerFileName = temporaryHeaderFile.getFilename();
    std::string valuesFileName = temporaryValuesFile.getFilename();

    dut::BeamformingFilePathSet_t fileSet;
    fileSet.headerFile = headerFileName.c_str();
    fileSet.valuesFile = valuesFileName.c_str();

    // Setup channel and rate
    setupChannelAndRate(dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Bandwidth::BANDWIDTH_EIGHTY);

    EXPECT_FALSE(m_dut.loadBeamformingMatrixFromFileSet(fileSet));
    EXPECT_THAT(m_dut.getLastError(), ::testing::HasSubstr("must contain exactly 2 32-bit values, got 0 values"));
}

TEST_F(DutWithClientMockTest, DISABLED_loadBeamformingMatrixFromFileSetShouldSucceedWithWave600VhtResource)
{
    // DISABLED: Wave600 beamforming implementation hasn't been tested and throws an exception
    std::vector<std::unique_ptr<TemporaryFile>> tempFiles;
    auto fileSetWithSizes = beamforming_test_helpers::createBeamformingFileSetWithSizes(tempFiles,
        IDR_BEAMFORMINGMATRIX_WAVE600_VHT_HEADER, IDR_BEAMFORMINGMATRIX_WAVE600_VHT_PHASES);

    // Setup channel and rate
    setupChannelAndRate(dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Bandwidth::BANDWIDTH_EIGHTY);

    beamforming_test_helpers::setupWave600BeamformingExpectations(m_client, fileSetWithSizes.valuesLineCount);
    ASSERT_TRUE(m_dut.loadBeamformingMatrixFromFileSet(fileSetWithSizes.fileSet));

    beamforming_test_helpers::setupWave600BeamformingExpectations(m_client, fileSetWithSizes.valuesLineCount);
    ASSERT_TRUE(m_dut.loadBeamformingMatrixFromFileSet(fileSetWithSizes.fileSet));
}

TEST_F(DutWithClientMockTest, DISABLED_loadBeamformingMatrixFromFileSetShouldSucceedWithWave600HeResource)
{
    // DISABLED: Wave600 beamforming implementation hasn't been tested and throws an exception
    std::vector<std::unique_ptr<TemporaryFile>> tempFiles;
    auto fileSetWithSizes = beamforming_test_helpers::createBeamformingFileSetWithSizes(tempFiles,
        IDR_BEAMFORMINGMATRIX_WAVE600_HE_HEADER, IDR_BEAMFORMINGMATRIX_WAVE600_HE_PHASES);

    // Setup channel and rate
    setupChannelAndRate(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Bandwidth::BANDWIDTH_EIGHTY);

    beamforming_test_helpers::setupWave600BeamformingExpectations(m_client, fileSetWithSizes.valuesLineCount);
    ASSERT_TRUE(m_dut.loadBeamformingMatrixFromFileSet(fileSetWithSizes.fileSet));
}

// ====================================================================================================
// WAVE700 EMBEDDED FILES BEAMFORMING TEST SUITE
// ====================================================================================================
// Parameterized test suite for Wave700 beamforming matrix loading with embedded resources.
// This consolidates all Wave700 beamforming tests into a single, maintainable test suite
// with comprehensive coverage of all supported WiFi standards and bandwidth combinations.
//
// The test suite covers:
// - VHT (802.11ac) Single-User: 20/40/80/160 MHz
// - HE (802.11ax) Single-User: 20/40/80/160 MHz
// - EHT (802.11be) Single-User: 20/40/80/160/320 MHz

// Test parameter structure for Wave700 beamforming tests
struct Wave700BeamformingTestParams {
    std::string testName;
    dut::PhyMode phyMode;
    dut::Bandwidth bandwidth;
    uint8_t channel;
    uint8_t primaryChannelIndex;
    int headerResourceId;
    int phasesResourceId;
    int extPhasesResourceId; // For EHT 160/320MHz tests
    bool hasSecondarySet; // For EHT 320MHz tests
    int secondaryHeaderResourceId;
    int secondaryPhasesResourceId;
    int secondaryExtPhasesResourceId;
    bool useExtendedExpectations; // For EHT 160MHz tests
    bool useSecondaryExpectations; // For EHT 320MHz tests
};

class Wave700BeamformingTestSuite : public DutWithClientMockTestGen7,
                                    public ::testing::WithParamInterface<Wave700BeamformingTestParams> {
};

TEST_P(Wave700BeamformingTestSuite, loadBeamformingMatrixFromEmbeddedResourcesShouldSucceed)
{
    const auto& params = GetParam();
    std::vector<std::unique_ptr<TemporaryFile>> tempFiles;

    setupChannelAndRate(params.phyMode, params.bandwidth, params.bandwidth, params.channel, params.primaryChannelIndex);

    if (params.hasSecondarySet) {
        // EHT 320MHz test with primary and secondary file sets
        auto primarySetWithSizes = beamforming_test_helpers::createBeamformingFileSetWithSizes(tempFiles,
            params.headerResourceId, params.phasesResourceId, params.extPhasesResourceId);
        auto secondarySetWithSizes = beamforming_test_helpers::createBeamformingFileSetWithSizes(tempFiles,
            params.secondaryHeaderResourceId, params.secondaryPhasesResourceId, params.secondaryExtPhasesResourceId);

        beamforming_test_helpers::setupWave700BeamformingExpectations(m_client, 0, params.useExtendedExpectations, params.useSecondaryExpectations);
        ASSERT_TRUE(m_dut.loadBeamformingMatrixFromFileSet(primarySetWithSizes.fileSet, secondarySetWithSizes.fileSet));
    } else {
        // Standard test with single file set
        auto fileSetWithSizes = beamforming_test_helpers::createBeamformingFileSetWithSizes(tempFiles,
            params.headerResourceId, params.phasesResourceId, params.extPhasesResourceId);

        size_t expectedLineCount = params.useExtendedExpectations ? 0 : fileSetWithSizes.valuesLineCount;
        beamforming_test_helpers::setupWave700BeamformingExpectations(m_client, expectedLineCount, params.useExtendedExpectations, params.useSecondaryExpectations);
        ASSERT_TRUE(m_dut.loadBeamformingMatrixFromFileSet(fileSetWithSizes.fileSet));
    }
}

// Test parameters for Wave700 beamforming test suite
INSTANTIATE_TEST_SUITE_P(Wave700BeamformingTests, Wave700BeamformingTestSuite, ::testing::Values(
                                                                                   // VHT (802.11ac) Single-User Tests
                                                                                   Wave700BeamformingTestParams { "VhtSu20MHz", dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_TWENTY, 36, 0, IDR_BEAMFORMINGMATRIX_WAVE700_VHT_SU_20MHZ_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_VHT_SU_20MHZ_PHASES, 0, false, 0, 0, 0, false, false }, Wave700BeamformingTestParams { "VhtSu40MHz", dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_FOURTY, 38, 0, IDR_BEAMFORMINGMATRIX_WAVE700_VHT_SU_40MHZ_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_VHT_SU_40MHZ_PHASES, 0, false, 0, 0, 0, false, false }, Wave700BeamformingTestParams { "VhtSu80MHz", dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_EIGHTY, 36, 0, IDR_BEAMFORMINGMATRIX_WAVE700_VHT_SU_80MHZ_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_VHT_SU_80MHZ_PHASES, 0, false, 0, 0, 0, false, false }, Wave700BeamformingTestParams { "VhtSu160MHz", dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, 36, 0, IDR_BEAMFORMINGMATRIX_WAVE700_VHT_SU_160MHZ_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_VHT_SU_160MHZ_PHASES, 0, false, 0, 0, 0, false, false },

                                                                                   // HE (802.11ax) Single-User Tests
                                                                                   Wave700BeamformingTestParams { "HeSu20MHz", dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, 36, 0, IDR_BEAMFORMINGMATRIX_WAVE700_HE_SU_20MHZ_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_HE_SU_20MHZ_PHASES, 0, false, 0, 0, 0, false, false }, Wave700BeamformingTestParams { "HeSu40MHz", dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_FOURTY, 38, 0, IDR_BEAMFORMINGMATRIX_WAVE700_HE_SU_40MHZ_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_HE_SU_40MHZ_PHASES, 0, false, 0, 0, 0, false, false }, Wave700BeamformingTestParams { "HeSu80MHz", dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY, 36, 0, IDR_BEAMFORMINGMATRIX_WAVE700_HE_SU_80MHZ_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_HE_SU_80MHZ_PHASES, 0, false, 0, 0, 0, false, false }, Wave700BeamformingTestParams { "HeSu160MHz", dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, 36, 0, IDR_BEAMFORMINGMATRIX_WAVE700_HE_SU_160MHZ_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_HE_SU_160MHZ_PHASES, 0, false, 0, 0, 0, false, false },

                                                                                   // EHT (802.11be) Single-User Tests
                                                                                   Wave700BeamformingTestParams { "EhtSu20MHz", dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_TWENTY, 36, 0, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_20MHZ_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_20MHZ_PHASES, 0, false, 0, 0, 0, false, false }, Wave700BeamformingTestParams { "EhtSu40MHz", dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_FOURTY, 38, 0, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_40MHZ_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_40MHZ_PHASES, 0, false, 0, 0, 0, false, false }, Wave700BeamformingTestParams { "EhtSu80MHz", dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_EIGHTY, 36, 0, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_80MHZ_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_80MHZ_PHASES, 0, false, 0, 0, 0, false, false }, Wave700BeamformingTestParams {
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     "EhtSu160MHz", dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, 36, 0, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_160MHZ_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_160MHZ_PHASES, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_160MHZ_PHASES_EHT, false, 0, 0, 0, true, false // Extended expectations for EHT 160MHz
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 },
                                                                                   Wave700BeamformingTestParams {
                                                                                       "EhtSu320MHz", dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY, 1, 0, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_320MHZ_LOWER_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_320MHZ_LOWER_PHASES, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_320MHZ_LOWER_PHASES_EHT, true, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_320MHZ_UPPER_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_320MHZ_UPPER_PHASES, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_320MHZ_UPPER_PHASES_EHT, false, true // Secondary expectations for EHT 320MHz
                                                                                   }));

TEST_F(DutWithClientMockTest, DISABLED_loadBeamformingMatrixFromFileSetShouldFailIfWave600WithSecondarySet)
{
    // DISABLED: Wave600 beamforming implementation hasn't been tested and throws an exception
    // Create valid file sets that have proper headers but we'll provide secondary sets
    // Use Wave600 VHT files which should be valid but don't support secondary sets
    std::vector<std::unique_ptr<TemporaryFile>> tempFiles;
    auto primaryFiles = beamforming_test_helpers::createBeamformingFileSetWithSizes(tempFiles,
        IDR_BEAMFORMINGMATRIX_WAVE600_VHT_HEADER, IDR_BEAMFORMINGMATRIX_WAVE600_VHT_PHASES);
    auto secondaryFiles = beamforming_test_helpers::createBeamformingFileSetWithSizes(tempFiles,
        IDR_BEAMFORMINGMATRIX_WAVE600_VHT_HEADER, IDR_BEAMFORMINGMATRIX_WAVE600_VHT_PHASES);

    // Setup channel and rate for VHT 80MHz (which Wave600 supports)
    setupChannelAndRate(dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Bandwidth::BANDWIDTH_EIGHTY);

    // Try to load beamforming matrix with secondary set on Wave600 - should fail
    EXPECT_FALSE(m_dut.loadBeamformingMatrixFromFileSet(primaryFiles.fileSet, secondaryFiles.fileSet));
    EXPECT_EQ(m_dut.getLastError(), "Wave600 hardware does not support EHT 320MHz beamforming");
}

TEST_F(DutWithClientMockTest, loadBeamformingMatrixFromFileSetShouldFailOnWave600WithNotTestedError)
{
    // Test that Wave600 throws the expected "not tested" exception for beamforming
    std::vector<std::unique_ptr<TemporaryFile>> tempFiles;
    auto fileSetWithSizes = beamforming_test_helpers::createBeamformingFileSetWithSizes(tempFiles,
        IDR_BEAMFORMINGMATRIX_WAVE600_VHT_HEADER, IDR_BEAMFORMINGMATRIX_WAVE600_VHT_PHASES);

    // Setup channel and rate
    setupChannelAndRate(dut::PhyMode::PHY_MODE_AC, dut::Bandwidth::BANDWIDTH_EIGHTY, dut::Bandwidth::BANDWIDTH_EIGHTY);

    // Wave600 should throw an exception indicating the implementation hasn't been tested
    EXPECT_FALSE(m_dut.loadBeamformingMatrixFromFileSet(fileSetWithSizes.fileSet));
    EXPECT_EQ(m_dut.getLastError(), "The Wave600 loadBeamformingMatrixFromFileSet implementation hasn't been tested.");
}

// Test for invalid secondary header error path on Wave700
TEST_F(DutWithClientMockTestGen7, loadBeamformingMatrixFromFileSetShouldFailIfInvalidSecondaryHeader)
{
    // Create valid primary file and invalid secondary header file
    std::vector<std::unique_ptr<TemporaryFile>> tempFiles;
    auto primaryFiles = beamforming_test_helpers::createBeamformingFileSetWithSizes(tempFiles,
        IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_320MHZ_LOWER_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_320MHZ_LOWER_PHASES);

    // Create secondary file with invalid header
    std::string invalidSecondaryHeader = "02eb6ff1\n18405a76"; // Invalid PHY mode bits
    EmbeddedResource secondaryValuesResource(IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_320MHZ_UPPER_PHASES, BEAMFORMINGMATRIX);

    auto secondaryHeaderFile = std::make_unique<TemporaryFile>(
        reinterpret_cast<const uint8_t*>(invalidSecondaryHeader.c_str()), invalidSecondaryHeader.length());
    auto secondaryValuesFile = std::make_unique<TemporaryFile>(
        secondaryValuesResource.getData(), secondaryValuesResource.getSize());

    dut::BeamformingFilePathSet_t secondarySet;
    secondarySet.headerFile = secondaryHeaderFile->getFilename();
    secondarySet.valuesFile = secondaryValuesFile->getFilename();

    tempFiles.push_back(std::move(secondaryHeaderFile));
    tempFiles.push_back(std::move(secondaryValuesFile));

    setupChannelAndRate(dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY, 1, 0);

    // Try to load beamforming matrix with invalid secondary header - should fail
    // No beamforming expectations since it should fail during header validation
    EXPECT_FALSE(m_dut.loadBeamformingMatrixFromFileSet(primaryFiles.fileSet, secondarySet));
    EXPECT_THAT(m_dut.getLastError(), ::testing::HasSubstr("Invalid beamforming header in secondary file"));
    EXPECT_THAT(m_dut.getLastError(), ::testing::HasSubstr(secondarySet.headerFile));
}

// Test for invalid secondary header error path on Wave700
TEST_F(DutWithClientMockTestGen7, loadBeamformingMatrixFromFileSetShouldFailIfSecondaryHeaderDoesNotMatchDutPhyMode)
{
    // Create valid primary file and invalid secondary header file
    std::vector<std::unique_ptr<TemporaryFile>> tempFiles;
    auto primaryFiles = beamforming_test_helpers::createBeamformingFileSetWithSizes(tempFiles,
        IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_320MHZ_LOWER_HEADER, IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_320MHZ_LOWER_PHASES);

    // Create secondary file with invalid header
    std::string invalidSecondaryHeader = "02eb6ffa\n18405a76"; // PhyMode set to VHT
    EmbeddedResource secondaryValuesResource(IDR_BEAMFORMINGMATRIX_WAVE700_EHT_SU_320MHZ_UPPER_PHASES, BEAMFORMINGMATRIX);

    auto secondaryHeaderFile = std::make_unique<TemporaryFile>(
        reinterpret_cast<const uint8_t*>(invalidSecondaryHeader.c_str()), invalidSecondaryHeader.length());
    auto secondaryValuesFile = std::make_unique<TemporaryFile>(
        secondaryValuesResource.getData(), secondaryValuesResource.getSize());

    dut::BeamformingFilePathSet_t secondarySet;
    secondarySet.headerFile = secondaryHeaderFile->getFilename();
    secondarySet.valuesFile = secondaryValuesFile->getFilename();

    tempFiles.push_back(std::move(secondaryHeaderFile));
    tempFiles.push_back(std::move(secondaryValuesFile));

    setupChannelAndRate(dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY, 1, 0);

    // Try to load beamforming matrix with invalid secondary header - should fail
    // No beamforming expectations since it should fail during header validation
    EXPECT_FALSE(m_dut.loadBeamformingMatrixFromFileSet(primaryFiles.fileSet, secondarySet));
    EXPECT_THAT(m_dut.getLastError(), ::testing::HasSubstr("secondary: Beamforming header PHY mode (AC) does not match DUT PHY mode (BE)"));
}

// ====================================================================================================
// BEAMFORMING HEADER VALIDATION TESTS
// ====================================================================================================
// These tests verify the validateBeamformingHeaderRegister() function for both chipset generations:
// - Wave600 (Gen6): Single header validation with bandwidth/PHY mode compatibility
// - Wave700 (Gen7): Single and dual header validation for EHT 320MHz configurations
// - Comprehensive error handling with specific validation failure messages
// - Uninitialized header detection and appropriate error reporting

// Wave600 (Gen6) Tests
TEST_F(DutWithClientMockTest, validateBeamformingHeaderRegisterShouldSucceedForMatchingHeader)
{
    // Create a valid header for Wave600 HT 40MHz (from BeamformingUtilsTest)
    uint32_t validHeader = 0x00100000; // HT 40MHz header pattern

    EXPECT_CALL(*m_client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave600::headerAddress, _, sizeof(uint32_t)))
        .WillOnce(Invoke([validHeader](dut::ChipModule, size_t, uint8_t* data, size_t) {
            *reinterpret_cast<uint32_t*>(data) = validHeader;
        }));

    // This should succeed if the header matches the expected PHY mode and bandwidth
    EXPECT_TRUE(m_dut.validateBeamformingHeaderRegister(dut::PhyMode::PHY_MODE_N_5, dut::Bandwidth::BANDWIDTH_FOURTY));
}

TEST_F(DutWithClientMockTest, validateBeamformingHeaderRegisterShouldFailForPhyModeMismatch)
{
    // Create a header that will parse as VHT mode but we'll expect HT mode
    // Use a definitive VHT pattern that won't be compatible with HT
    uint32_t vhtModeHeader = 0x00800002; // VHT 80MHz header pattern

    EXPECT_CALL(*m_client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave600::headerAddress, _, sizeof(uint32_t)))
        .WillOnce(Invoke([vhtModeHeader](dut::ChipModule, size_t, uint8_t* data, size_t) {
            *reinterpret_cast<uint32_t*>(data) = vhtModeHeader;
        }));

    // Should return false due to PHY mode or bandwidth mismatch
    EXPECT_FALSE(m_dut.validateBeamformingHeaderRegister(dut::PhyMode::PHY_MODE_N_5, dut::Bandwidth::BANDWIDTH_TWENTY));

    // Check that error message contains expected content
    std::string errorMsg = m_dut.getLastError();
    EXPECT_TRUE(
        (errorMsg.find("PHY mode mismatch") != std::string::npos) || (errorMsg.find("Bandwidth mismatch") != std::string::npos));
}

TEST_F(DutWithClientMockTest, validateBeamformingHeaderRegisterShouldFailForBandwidthMismatch)
{
    // Create a header that will parse with 20MHz bandwidth but we'll expect 40MHz
    uint32_t bw20Header = 0x00000000; // HT 20MHz header pattern

    EXPECT_CALL(*m_client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave600::headerAddress, _, sizeof(uint32_t)))
        .WillOnce(Invoke([bw20Header](dut::ChipModule, size_t, uint8_t* data, size_t) {
            *reinterpret_cast<uint32_t*>(data) = bw20Header;
        }));

    // Should return false with bandwidth mismatch message
    EXPECT_FALSE(m_dut.validateBeamformingHeaderRegister(dut::PhyMode::PHY_MODE_N_5, dut::Bandwidth::BANDWIDTH_FOURTY));

    std::string errorMsg = m_dut.getLastError();
    EXPECT_TRUE(errorMsg.find("Bandwidth mismatch") != std::string::npos);
    EXPECT_TRUE(errorMsg.find("Expected: 40MHz") != std::string::npos);
}

// Wave700 (Gen7) Tests
TEST_F(DutWithClientMockTestGen7, validateBeamformingHeaderRegisterShouldSucceedForMatchingHeader)
{
    // Create a valid header for Wave700 HE 80MHz: RU value 5, PHY mode 3
    uint32_t validHeader = (5 << 21) | 3; // HE 80MHz header pattern

    EXPECT_CALL(*m_client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave700::primaryBfHeaderAddress, _, sizeof(uint32_t)))
        .WillOnce(Invoke([validHeader](dut::ChipModule, size_t, uint8_t* data, size_t) {
            *reinterpret_cast<uint32_t*>(data) = validHeader;
        }));

    // This should succeed if the header matches the expected PHY mode and bandwidth
    EXPECT_TRUE(m_dut.validateBeamformingHeaderRegister(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY));
}

TEST_F(DutWithClientMockTestGen7, validateBeamformingHeaderRegisterShouldSucceedForEht320DualHeaders)
{
    // Create valid headers for both primary and secondary headers for EHT 320MHz
    uint32_t validPrimaryHeader = (7 << 21) | 4; // EHT 320MHz: RU value 7, PHY mode 4
    uint32_t validSecondaryHeader = (7 << 21) | 4; // EHT 320MHz: same pattern for secondary

    EXPECT_CALL(*m_client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave700::primaryBfHeaderAddress, _, sizeof(uint32_t)))
        .WillOnce(Invoke([validPrimaryHeader](dut::ChipModule, size_t, uint8_t* data, size_t) {
            *reinterpret_cast<uint32_t*>(data) = validPrimaryHeader;
        }));

    EXPECT_CALL(*m_client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave700::secondaryBfHeaderAddress, _, sizeof(uint32_t)))
        .WillOnce(Invoke([validSecondaryHeader](dut::ChipModule, size_t, uint8_t* data, size_t) {
            *reinterpret_cast<uint32_t*>(data) = validSecondaryHeader;
        }));

    // This should succeed if both headers match the expected PHY mode and bandwidth
    EXPECT_TRUE(m_dut.validateBeamformingHeaderRegister(dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY));
}

TEST_F(DutWithClientMockTestGen7, validateBeamformingHeaderRegisterShouldFailForUninitializedHeader)
{
    // For Wave700, 0x00000000 should be treated as uninitialized since it doesn't match
    // the expected PHY mode/RU value pattern for any valid configuration
    uint32_t uninitializedHeader = 0x00000000;

    EXPECT_CALL(*m_client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave700::primaryBfHeaderAddress, _, sizeof(uint32_t)))
        .WillOnce(Invoke([uninitializedHeader](dut::ChipModule, size_t, uint8_t* data, size_t) {
            *reinterpret_cast<uint32_t*>(data) = uninitializedHeader;
        }));

    // Should return false with specific message about uninitialized header
    EXPECT_FALSE(m_dut.validateBeamformingHeaderRegister(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY));

    std::string errorMsg = m_dut.getLastError();
    EXPECT_TRUE(errorMsg.find("has not been written (contains all zeros)") != std::string::npos);
    EXPECT_TRUE(errorMsg.find("Please configure beamforming before validation") != std::string::npos);
}

TEST_F(DutWithClientMockTestGen7, validateBeamformingHeaderRegisterShouldFailForEht320PartiallyUninitializedHeaders)
{
    // Primary header initialized with EHT 320MHz, secondary header with incompatible data
    uint32_t validPrimaryHeader = (7 << 21) | 4; // EHT 320MHz: RU value 7, PHY mode 4
    uint32_t incompatibleSecondaryHeader = (3 << 21) | 2; // VHT 20MHz: RU value 3, PHY mode 2

    EXPECT_CALL(*m_client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave700::primaryBfHeaderAddress, _, sizeof(uint32_t)))
        .WillOnce(Invoke([validPrimaryHeader](dut::ChipModule, size_t, uint8_t* data, size_t) {
            *reinterpret_cast<uint32_t*>(data) = validPrimaryHeader;
        }));

    EXPECT_CALL(*m_client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave700::secondaryBfHeaderAddress, _, sizeof(uint32_t)))
        .WillOnce(Invoke([incompatibleSecondaryHeader](dut::ChipModule, size_t, uint8_t* data, size_t) {
            *reinterpret_cast<uint32_t*>(data) = incompatibleSecondaryHeader;
        }));

    // Should return false with EHT 320MHz validation failure mentioning secondary header mismatch
    EXPECT_FALSE(m_dut.validateBeamformingHeaderRegister(dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY));

    std::string errorMsg = m_dut.getLastError();
    EXPECT_TRUE(errorMsg.find("EHT 320MHz validation failed") != std::string::npos);
    EXPECT_TRUE(errorMsg.find("secondary header") != std::string::npos);
    // Could be PHY mode or bandwidth mismatch in the secondary header
    EXPECT_TRUE(
        (errorMsg.find("PHY mode mismatch") != std::string::npos) || (errorMsg.find("Bandwidth mismatch") != std::string::npos));
}

TEST_F(DutWithClientMockTestGen7, validateBeamformingHeaderRegisterShouldFailForCorruptedHeader)
{
    // Create a corrupted header that will cause extractBeamformingHeaderInfo to return false
    // Using PHY mode 1 which is reserved/invalid according to extractBeamformingHeaderInfo
    uint32_t corruptedHeader = 0x00600001; // RU value 3 (20MHz), PHY mode 1 (reserved/invalid)

    EXPECT_CALL(*m_client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave700::primaryBfHeaderAddress, _, sizeof(uint32_t)))
        .WillOnce(Invoke([corruptedHeader](dut::ChipModule, size_t, uint8_t* data, size_t) {
            *reinterpret_cast<uint32_t*>(data) = corruptedHeader;
        }));

    // Should return false with specific message about header corruption
    EXPECT_FALSE(m_dut.validateBeamformingHeaderRegister(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY));

    std::string errorMsg = m_dut.getLastError();
    EXPECT_TRUE(errorMsg.find("Failed to extract beamforming header info from header") != std::string::npos);
    EXPECT_TRUE(errorMsg.find("Header may be corrupted") != std::string::npos);
}

TEST_F(DutWithClientMockTestGen7, validateBeamformingHeaderRegisterShouldFailForEht320CorruptedPrimaryHeader)
{
    // Test EHT 320MHz with corrupted primary header - this should trigger the errorMessage += primaryError; path
    uint32_t corruptedPrimaryHeader = 0x00E00001; // RU value 7 (320MHz), PHY mode 1 (reserved/invalid)
    uint32_t validSecondaryHeader = (7 << 21) | 4; // EHT 320MHz: RU value 7, PHY mode 4

    EXPECT_CALL(*m_client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave700::primaryBfHeaderAddress, _, sizeof(uint32_t)))
        .WillOnce(Invoke([corruptedPrimaryHeader](dut::ChipModule, size_t, uint8_t* data, size_t) {
            *reinterpret_cast<uint32_t*>(data) = corruptedPrimaryHeader;
        }));

    EXPECT_CALL(*m_client, readMemory(dut::ChipModule::CHIP_MODULE_REGISTER, dut::beamforming_utils::beamforming::wave700::secondaryBfHeaderAddress, _, sizeof(uint32_t)))
        .WillOnce(Invoke([validSecondaryHeader](dut::ChipModule, size_t, uint8_t* data, size_t) {
            *reinterpret_cast<uint32_t*>(data) = validSecondaryHeader;
        }));

    // Should return false with EHT 320MHz validation failure mentioning primary header corruption
    EXPECT_FALSE(m_dut.validateBeamformingHeaderRegister(dut::PhyMode::PHY_MODE_BE, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY));

    std::string errorMsg = m_dut.getLastError();
    EXPECT_TRUE(errorMsg.find("EHT 320MHz validation failed") != std::string::npos);
    EXPECT_TRUE(errorMsg.find("Failed to extract beamforming header info from primary header") != std::string::npos);
    EXPECT_TRUE(errorMsg.find("Header may be corrupted") != std::string::npos);
}

}
