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

#include "dut/DutImpl.h"

#include "CmdLineArgs.h"
#include "CombinedVersion.h"
#include "NonVolatileMemory.h"
#include "dut/ConnectionImpl.h"
#include "dut/Tools.h"
#include "dut/WindowsConsoleLogger.h"

#include <gtest/gtest.h>
#include <thread>

namespace {
constexpr auto g_connectionTimeout = std::chrono::milliseconds(1000);
constexpr uint8_t g_wlanIndex = 0x02;

constexpr dut::NvMemoryType g_memoryType = dut::NvMemoryType::MEMORY_TYPE_FLASH;
constexpr dut::NvMemorySize g_memorySize = dut::NvMemorySize::MEMORY_SIZE_BYTES_1K;

constexpr std::chrono::milliseconds g_recoveryTimeAfterNvmFlush { 10000 };

class _IntegrationTest : public ::testing::Test {
public:
    _IntegrationTest()
        : m_dut(g_wlanIndex, m_connection, m_logger, true)
    {
    }

    void SetUp() override
    {
        m_connected = m_connection->open(g_cmdLineArgs.getIpAddress(), g_cmdLineArgs.getTcpPort(), g_connectionTimeout);
        ASSERT_TRUE(m_connected);

        constexpr dut::NvMemoryType memoryType = dut::NvMemoryType::MEMORY_TYPE_FLASH;
        constexpr dut::NvMemorySize memorySize = dut::NvMemorySize::MEMORY_SIZE_BYTES_1K;
        m_dut.driverInit(false, g_memoryType, g_memorySize, dut::Band::BAND_INVALID);
    }

    void TearDown() override
    {
        if (m_connected) {
            m_dut.driverRelease();

            m_connection->close();
            m_connected = false;
        }
    }

    std::shared_ptr<dut::ConnectionImpl> m_connection = std::make_shared<dut::ConnectionImpl>();
    std::shared_ptr<dut::Logger> m_logger = std::make_shared<dut::WindowsConsoleLogger>(true, g_cmdLineArgs.getLogLevel());
    dut::DutImpl m_dut;
    bool m_connected = false;
};

TEST_F(_IntegrationTest, exampleOfUse)
{
    {
        std::string version = dut::Dut::getVersion();
        EXPECT_FALSE(version.length() == 0);
        std::cout << "version: " + version << std::endl;
        std::cout << std::endl;
    }

    {
        dut::ChipID chipId = dut::ChipID::CHIP_ID_INVALID;
        m_dut.getChipId(chipId);
        std::cout << "chipId: " + dut::toString(chipId) << std::endl;
        std::cout << std::endl;
    }

    {
        std::string version;
        m_dut.getComponentVersion(dut::VersionedComponent::VERSIONED_COMPONENT_CV, version);
        std::cout << "version: " + version << std::endl;
        std::cout << std::endl;
    }

    {
        uint8_t countryCode = 0;
        std::array<uint8_t, dut::cardInfoMacAddressSize> macAddress {};
        std::array<uint8_t, dut::cardInfoSerialNumberSize> serialNumber {};
        uint8_t week = 0;
        uint8_t year = 0;
        m_dut.getCardInfo(countryCode, macAddress, serialNumber, week, year);
        std::cout << "countryCode: " + dut::toHexString(countryCode) << std::endl;
        std::cout << "macAddress: " + dut::toString(macAddress.data(), dut::cardInfoMacAddressSize) << std::endl;
        std::cout << "serialNumber: " + dut::toString(serialNumber.data(), dut::cardInfoSerialNumberSize) << std::endl;
        std::cout << "week: " + dut::toString(week) << std::endl;
        std::cout << "year: " + dut::toString(year) << std::endl;
        std::cout << std::endl;
    }

    {
        uint8_t numTxAntennas = 1;
        uint32_t offset1 = 0;
        uint32_t offset2 = 0;
        uint32_t offset3 = 0;
        m_dut.getBbicCddValues(numTxAntennas, offset1, offset2, offset3);
        std::cout << "offset1: " + dut::toString(offset1) << std::endl;
        std::cout << "offset2: " + dut::toString(offset2) << std::endl;
        std::cout << "offset3: " + dut::toString(offset3) << std::endl;
    }

    {
        uint32_t receivedPackets = 0;
        m_dut.getMacPacketCounters(receivedPackets);
        std::cout << "receivedPackets: " + dut::toString(receivedPackets) << std::endl;
    }

    {
        uint32_t receivedPackets = 0;
        uint32_t crcErrors = 0;
        uint32_t forwardedPackets = 0;

        m_dut.getPhyPacketCounters(receivedPackets, crcErrors, forwardedPackets);

        std::cout << "receivedPackets: " + dut::toString(receivedPackets) << std::endl;
        std::cout << "crcErrors: " + dut::toString(crcErrors) << std::endl;
        std::cout << "forwardedPackets: " + dut::toString(forwardedPackets) << std::endl;
    }

    {
        uint16_t xtalValue = 0;

        m_dut.getXtalRegValue(xtalValue);

        std::cout << "xtalValue: " << std::to_string(xtalValue) << std::endl;

        m_dut.setXtalRegValue(xtalValue);
    }

    {
        dut::ChipModule chipModule = dut::ChipModule::CHIP_MODULE_PHY;
        size_t address = 0xa0b0019c;
        uint32_t mask = 0x1;
        uint32_t value = 0;

        m_dut.readRegister(chipModule, address, mask, value);

        std::cout << "value: " << std::to_string(value) << std::endl;

        m_dut.writeRegister(chipModule, address, mask, value);
    }

    {
        size_t address = 0;
        std::array<uint8_t, dut::NonVolatileMemory::nvmVersionAddress + 1> buffer;
        bool useCache = false;

        m_dut.readNvm(address, buffer.data(), buffer.size(), useCache);

        std::cout << "data: " << dut::toString(buffer.data(), buffer.size()) << std::endl;

        m_dut.writeNvm(address, buffer.data(), buffer.size());
    }

    {
        dut::ChipModule chipModule = dut::ChipModule::CHIP_MODULE_RF;
        size_t address = 0x1;
        std::array<uint8_t, 10> buffer;

        m_dut.readMemory(chipModule, address, buffer.data(), buffer.size());

        std::cout << "data: " << dut::toString(buffer.data(), buffer.size()) << std::endl;

        m_dut.writeMemory(chipModule, address, buffer.data(), buffer.size());

        std::array<uint8_t, 10> expectedBuffer = buffer;

        m_dut.readMemory(chipModule, address, buffer.data(), buffer.size());
        EXPECT_EQ(memcmp(expectedBuffer.data(), buffer.data(), buffer.size()), 0);
    }

    m_dut.resetMacPacketCounters();
    m_dut.resetPhyPacketCounters();
    m_dut.resetMpduPacketCounters();

    {
        dut::AntennaMask antennaMask = 0x0f;
        m_dut.setEnabledRxAntennaMask(antennaMask);
    }

    {
        dut::AntennaMask antennaMask = 0x0f;
        m_dut.setEnabledTxAntennaMask(antennaMask);
    }

    {
        uint32_t offlineCalMask = 0x0788;
        uint32_t onlineCalMask = 0x000f;
        m_dut.setHdkConfig(offlineCalMask, onlineCalMask);
    }

    {
        bool productionFlag = false;
        m_dut.setProductionFlag(productionFlag, true);
    }

    {
        std::array<uint8_t, dut::maxNumRxAntennas> rxEvm;
        m_dut.getRxEvm(rxEvm);

        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            std::cout << "RxEvm[" + dut::toString(i) << "] = " << dut::toString(rxEvm[i]) << std::endl;
        }
    }
}

TEST_F(_IntegrationTest, rssi)
{
    {
        std::array<int16_t, dut::maxNumRxAntennas> rssi;
        m_dut.getInbandRssi(rssi);

        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            std::cout << "inband RSSI[" + dut::toString(i) << "] = " << dut::toString(rssi[i]) << std::endl;
        }
    }

    {
        std::array<uint16_t, dut::maxNumRxAntennas> rssi;
        m_dut.getRssi(0, 1024, rssi);

        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            std::cout << "RSSI[" + dut::toString(i) << "] = " << dut::toString(rssi[i]) << std::endl;
        }
    }

    {
        std::array<int8_t, dut::maxNumRxAntennas> power;
        EXPECT_TRUE(m_dut.getRfRssiPower(power));

        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            std::cout << "POWER[" + dut::toString(i) << "] = " << dut::toString(power[i]) << std::endl;
        }
    }
}

TEST_F(_IntegrationTest, startCw)
{
    {
        dut::AntennaMask antennaMask = 0x0f;
        m_dut.setEnabledTxAntennaMask(antennaMask);
    }

    {
        dut::AntennaMask antennaMask = 0x0f;
        m_dut.setEnabledRxAntennaMask(antennaMask);
    }

    {
        dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
        dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
        uint8_t channel = 36;
        uint8_t primaryChannelIndex = 0;
        dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;
        m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType);
    }

    {
        int8_t amplitude = 10;
        int16_t tone = 4;

        m_dut.startCw(amplitude, tone);
        m_dut.stopCw();
    }
}

TEST_F(_IntegrationTest, startTx)
{
    {
        dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
        dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
        uint8_t channel = 36;
        uint8_t primaryChannelIndex = 0;
        dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;
        m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType);
    }

    {
        dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
        dut::Mcs mcs = dut::Mcs::MCS_QPSK_12;
        uint8_t nss = 1;
        dut::Gi gi = dut::Gi::GI_0_8_US;
        dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
        float rateMbps = 0.0f;

        m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps);
    }

    {
        uint16_t repetitions = UINT16_MAX;
        uint32_t packetLength = 4000;
        bool longData = false;
        bool beamforming = false;

        m_dut.startTx(repetitions, packetLength, longData, beamforming);
    }

    {
        std::array<uint32_t, dut::maxNumTxAntennas> voltages {};

        m_dut.getTransmitVoltages(voltages);

        for (size_t i = 0; i < dut::maxNumTxAntennas; i++) {
            std::cout << "Voltage[" + dut::toString(i) << "] = " << dut::toString(voltages[i]) << std::endl;
        }
    }

    {
        dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
        dut::Mcs mcs = dut::Mcs::MCS_QPSK_34;
        uint8_t nss = 1;
        dut::Gi gi = dut::Gi::GI_0_8_US;
        dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
        float rateMbps = 0.0f;

        m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps);
    }

    {
        m_dut.stopTx();
    }
}

TEST_F(_IntegrationTest, startSpacelessTx)
{
    {
        dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AX;
        dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
        uint8_t channel = 36;
        uint8_t primaryChannelIndex = 0;
        dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;
        m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType);
    }

    {
        dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
        dut::Mcs mcs = dut::Mcs::MCS_QPSK_12;
        uint8_t nss = 1;
        dut::Gi gi = dut::Gi::GI_0_8_US;
        dut::Ltf ltf = dut::Ltf::LTF_MEDIUM;
        float rateMbps = 0.0f;

        m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps);
    }

    {
        m_dut.setSpacelessTxEnabled(true);
    }

    {
        m_dut.setSpacelessTxEnabled(false);
    }
}

TEST_F(_IntegrationTest, calibrateTssiS2d)
{
    {
        uint8_t antenna = 1;
        dut::AntennaMask enabledTxAntennaMask(1 << antenna);

        uint32_t offlineCalMask = 0xffff;
        uint32_t onlineCalMask = 0x0000;

        dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AC;
        dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
        uint8_t channel = 106;
        uint8_t primaryChannelIndex = 0;
        dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

        dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
        dut::Mcs mcs = dut::Mcs::MCS_64QAM_56;
        uint8_t nss = 1;
        dut::Gi gi = dut::Gi::GI_0_8_US;
        dut::Ltf ltf = dut::Ltf::LTF_SHORT;
        float rateMbps = 0.0f;

        uint32_t ifs = 140;

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

        m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask);
        m_dut.setHdkConfig(offlineCalMask, onlineCalMask);
        m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType);
        m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps);
        m_dut.setIfs(ifs);
        m_dut.startTx(repetitions, packetLength, longData, beamforming);
        m_dut.calibrateTssiS2d(lowerPowerLevel, upperPowerLevel, tssiLow, tssiHigh, region, powerThreshold, gain, offset);

        std::cout << "gain: " + dut::toString(gain) << ", offset: " << dut::toString(offset) << std::endl;
    }
}

TEST_F(_IntegrationTest, getTransmitPowerTableOffset)
{
    {
        std::stringstream stream;

        stream << "Antenna\t20 MHz\t40 MHz\t80 MHz\t160 MHz" << std::endl;

        for (uint8_t antenna = 0; antenna < dut::maxNumTxAntennas; antenna++) {

            stream << dut::toString(antenna);

            for (dut::Bandwidth bandwidth : {
                     dut::Bandwidth::BANDWIDTH_TWENTY,
                     dut::Bandwidth::BANDWIDTH_FOURTY,
                     dut::Bandwidth::BANDWIDTH_EIGHTY,
                     dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY }) {

                int16_t offset = 0;
                m_dut.getTransmitPowerTableOffset(antenna, bandwidth, offset);

                stream << "\t" << dut::toString(offset);
            }

            stream << std::endl;
        }

        std::cout << stream.str();
    }
}

TEST_F(_IntegrationTest, openLoopSweep)
{
    {
        uint32_t offlineCalMask = 0xffff;
        uint32_t onlineCalMask = 0x0000;

        dut::PhyMode phyMode = dut::PhyMode::PHY_MODE_AC;
        dut::Bandwidth spectrumBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
        uint8_t channel = 36;
        uint8_t primaryChannelIndex = 0;
        dut::RegulationType regulationType = dut::RegulationType::REGULATION_TYPE_FCC_SP;

        dut::Bandwidth signalBandwidth = dut::Bandwidth::BANDWIDTH_TWENTY;
        dut::Mcs mcs = dut::Mcs::MCS_256QAM_34;
        uint8_t nss = 1;
        dut::Gi gi = dut::Gi::GI_0_8_US;
        dut::Ltf ltf = dut::Ltf::LTF_SHORT;
        float rateMbps = 0.0f;

        uint32_t ifs = 25;

        uint16_t repetitions = UINT16_MAX;
        uint32_t packetLength = 600;
        bool longData = true;
        bool beamforming = false;

        m_dut.setTransmitPowerControl(false, 0xff);
        m_dut.setHdkConfig(offlineCalMask, onlineCalMask);
        m_dut.setChannel(phyMode, spectrumBandwidth, channel, primaryChannelIndex, regulationType);
        m_dut.setRate(signalBandwidth, mcs, nss, gi, ltf, rateMbps);
        m_dut.setIfs(ifs);
        m_dut.startTx(repetitions, packetLength, longData, beamforming);

        std::stringstream stream;

        stream << "Antenna\tPower\tVoltage" << std::endl;

        for (uint8_t antenna = 0; antenna < dut::maxNumTxAntennas; antenna++) {

            stream << dut::toString(antenna);

            dut::AntennaMask enabledTxAntennaMask(1 << antenna);

            m_dut.setEnabledTxAntennaMask(enabledTxAntennaMask);

            const std::vector<uint8_t> powerLevelValues { 6, 8, 10, 12, 14, 16 };
            for (uint8_t powerLevel : powerLevelValues) {

                m_dut.setTransmitPowerLevel(powerLevel);

                std::array<uint32_t, dut::maxNumTxAntennas> voltages;
                m_dut.getTransmitVoltages(voltages);

                stream << "\t" << dut::toString(powerLevel) << "\t" << dut::toString(voltages[antenna]) << std::endl;
            }

            stream << std::endl;
        }

        std::cout << stream.str();

        m_dut.stopTx();
    }
}

TEST_F(_IntegrationTest, setCalibrationData)
{
    // Get RSSI and TSSI calibration data from AP
    // (either from flash or EEPROM, depending on global constant expression `g_memoryType` used as
    // parameter to driverInit() when setting up the test)
    dut::CalibrationFileVersion version;
    std::vector<dut::RssiCalibrationData_t> rssiCalibrationData;
    std::vector<dut::TssiCalibrationData_t> tssiCalibrationData;
    m_dut.getRssiCalibrationData(version, rssiCalibrationData);
    m_dut.getTssiCalibrationData(version, tssiCalibrationData);

#if 1
    if (version == dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6) {
        // Get (or create if it does not exist) the first (and only) RSSI calibration data item
        if (rssiCalibrationData.empty()) {
            rssiCalibrationData.emplace_back();
        }
        //dut::RssiCalibrationData_t* rssiCalibrationDataItemVer6 = &rssiCalibrationData[0];

        // TODO: Set/change RSSI calibration data fields at will
        // ...

    } else if (version == dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_7) {
        // Get (or create if it does not exist) the first (and only) RSSI calibration data item
        if (rssiCalibrationData.empty()) {
            rssiCalibrationData.emplace_back();
        }
        dut::RssiCalibrationData_t* rssiCalibrationDataItemVer7 = &rssiCalibrationData[0];

        // TODO: Set/change RSSI calibration data fields at will
        // Example:

        rssiCalibrationDataItemVer7->antennaMask = 0x1f;
        rssiCalibrationDataItemVer7->startFreq = 4900;
        rssiCalibrationDataItemVer7->stopFreq = 5900;
        rssiCalibrationDataItemVer7->calibrationFreq = 5180;
        rssiCalibrationDataItemVer7->chipTemperature = 38;
        rssiCalibrationDataItemVer7->subBandsCrossingPoints = false;

        rssiCalibrationDataItemVer7->numRxRfFlatnessPoints = 6;
        rssiCalibrationDataItemVer7->numRxRssiFlatnessPoints = 0;
        memset(rssiCalibrationDataItemVer7->rxRfFlatnessFreqs, 0, dut::maxRxRfFlatnessPoints);
        memset(rssiCalibrationDataItemVer7->rxRssiFlatnessFreqs, 0, dut::maxRxRssiFlatnessPoints);

        auto& antennaData = rssiCalibrationDataItemVer7->antennaData.antennasVer7[0];

        const std::vector<uint16_t> subBandsCrossingPoints { 4900, 4920, 4940 };
        std::copy(std::begin(subBandsCrossingPoints), std::end(subBandsCrossingPoints),
            std::begin(antennaData.subBandsCrossingPoints));
        const std::vector<float> lnaGains { 1.0f, 2.0f, 3.0f };
        std::copy(std::begin(lnaGains), std::end(lnaGains),
            std::begin(antennaData.lnaGains));
        const std::vector<int8_t> rxRfFlatnessDeltaPointsHighGain { -2, -1, 0, 1, 2, 3 };
        std::copy(std::begin(rxRfFlatnessDeltaPointsHighGain), std::end(rxRfFlatnessDeltaPointsHighGain),
            std::begin(antennaData.rxRfFlatnessDeltaPointsHighGain));
        const std::vector<int8_t> rxRfFlatnessDeltaPointsLowGain { -3, -2, -2, 0, 1, 2 };
        std::copy(std::begin(rxRfFlatnessDeltaPointsLowGain), std::end(rxRfFlatnessDeltaPointsLowGain),
            std::begin(antennaData.rxRfFlatnessDeltaPointsLowGain));
        const std::vector<int8_t> rxRfFlatnessDeltaPointsBypass { 0, 1, 2, 3, 4, 5 };
        std::copy(std::begin(rxRfFlatnessDeltaPointsBypass), std::end(rxRfFlatnessDeltaPointsBypass),
            std::begin(antennaData.rxRfFlatnessDeltaPointsBypass));
        const std::vector<dut::RssiS2D_t> s2dGainOffset { { 1, 1 }, { 2, 2 } };
        std::copy(std::begin(s2dGainOffset), std::end(s2dGainOffset),
            std::begin(antennaData.s2dGainOffset));
        const std::vector<dut::RssiAB_t> pointsAB { { 1.0f, 1.0f }, { 2.0f, 2.0f } };
        std::copy(std::begin(pointsAB), std::end(pointsAB),
            std::begin(antennaData.pointsAB));
        std::fill(std::begin(antennaData.rxRssiFlatnessDeltaPoints), std::end(antennaData.rxRssiFlatnessDeltaPoints), 0);
    }
#endif

#if 0
    if (version == dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6) {
        // Get (or create if it does not exist) the first TSSI calibration data item
        if (tssiCalibrationData.empty()) {
            tssiCalibrationData.emplace_back();
        }
        dut::TssiCalibrationData_t* tssiCalibrationDataItemVer6 = &tssiCalibrationData[0];

        // TODO: Set/change RSSI calibration data fields at will

    } else if (version == dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_7) {
        // Get (or create if it does not exist) the first TSSI calibration data item
        // (in TSSI there can be more than one item in the array, one for each calibrated channel)
        if (tssiCalibrationData.empty()) {
            tssiCalibrationData.emplace_back();
        }
        dut::TssiCalibrationData_t* tssiCalibrationDataItemVer7 = &tssiCalibrationData[0];

        // TODO: Add/remove/update TSSI calibration data items and fields at will
        // (in TSSI there is no difference between version 6 and 7)
        // Example:

        tssiCalibrationDataItemVer7->antennaMask = 0x0f;
        tssiCalibrationDataItemVer7->numRegions = 1;
        tssiCalibrationDataItemVer7->bw = dut::Bandwidth::BANDWIDTH_TWENTY;
        tssiCalibrationDataItemVer7->channel = 36;
        tssiCalibrationDataItemVer7->band = dut::Band::BAND_5000MHZ;

        {
            auto& antennaData = tssiCalibrationDataItemVer7->antennas[0];

            antennaData.maxPower = 51;
            antennaData.uEvm = 44;
            antennaData.uEvmGain = 16;
            const std::vector<dut::TssiAB_t> pointsAB { { { 0, 0 }, { 0, 0 }, { 0, 0 } } };
            std::copy(std::begin(pointsAB), std::end(pointsAB), std::begin(antennaData.pointsAB));
            const std::vector<int32_t> s2dGain { 2, 0, 0 };
            std::copy(std::begin(s2dGain), std::end(s2dGain), std::begin(antennaData.s2dGain));
            const std::vector<int32_t> s2dOffset { 44, 0, 0 };
            std::copy(std::begin(s2dOffset), std::end(s2dOffset), std::begin(antennaData.s2dOffset));
        }

        {
            auto& antennaData = tssiCalibrationDataItemVer7->antennas[1];

            antennaData.maxPower = 50;
            antennaData.uEvm = 42;
            antennaData.uEvmGain = 14;
            const std::vector<dut::TssiAB_t> pointsAB { { { 0, 0 }, { 0, 0 }, { 0, 0 } } };
            std::copy(std::begin(pointsAB), std::end(pointsAB), std::begin(antennaData.pointsAB));
            const std::vector<int32_t> s2dGain { 3, 0, 0 };
            std::copy(std::begin(s2dGain), std::end(s2dGain), std::begin(antennaData.s2dGain));
            const std::vector<int32_t> s2dOffset { 44, 0, 0 };
            std::copy(std::begin(s2dOffset), std::end(s2dOffset), std::begin(antennaData.s2dOffset));
        }
    }
#endif

    // Set RSSI and TSSI calibration data back to AP
    m_dut.setRssiCalibrationData(rssiCalibrationData);
    m_dut.setTssiCalibrationData(tssiCalibrationData);

    // Due to a bug in old versions of IQDVT, in calibration files for Wave700 the TPC power and
    // UEVM values were set in dB units instead of the expected half-dB units.
    // Once this problem was fixed in IQDVT, we needed a way to distinguish which calibration files
    // contained data in which units and decided to use a "subversion" field.
    // Version was already stored in byte at index 40 (0-based) of the calibration file header.
    // The assigned location for the new subversion field was byte #3 of CardInfo CIS (0x60). This
    // byte used to contain "BB chip version" but was not used anymore and its value was 0x00
    // always, which made it ideal to redefine it as subversion 0.
    // In summary, calibration files for Wave700 with subversion 0 contain TPC power and UEVM in
    // dB units and in files with subversion 1 these values are in half-dB units.
    if (version == dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_7) {
        m_dut.setCalibrationFileVersion(
            dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_7,
            dut::CalibrationFileSubversion::CALIBRATION_FILE_SUBVERSION_1);
    }

    // Write calibration data to non-volatile memory
    // (either to flash or EEPROM depending on value set to parameter `memoryType`)
    dut::NvMemoryType memoryType = dut::NvMemoryType::MEMORY_TYPE_EEPROM;
    m_dut.writeCalibrationFile(memoryType, g_memorySize);

#if 1
    // Save calibration data to a file
    // This file can later be loaded into non-volatile memory using dut_gui.exe or dut_cli.exe
    std::string fileName = "C:\\temp\\cal_wlan" + dut::toString(g_wlanIndex) + ".bin";
    m_dut.saveNvmToFile(fileName);
#endif

#if 0
    // Flush the calibration data to the non-volatile memory.
    m_dut.flushNvm();
    std::this_thread::sleep_for(g_recoveryTimeAfterNvmFlush);
#endif
}

}