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

#include "CommandLine.h"

#include "Config.h"
#include "ConsoleMock.h"
#include "Context.h"
#include "TempFile.h"
#include "dut/ConnectionMock.h"
#include "dut/DutMock.h"

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::StrictMock;

namespace {

MATCHER_P(startCalibrationParamsMatcher, expectedParams, "") { return (arg.type == expectedParams.type) && (arg.maskType == expectedParams.maskType) && (arg.mask == expectedParams.mask); }

MATCHER_P3(beamformingFilePathSetMatcher, expectedHeaderFile, expectedValuesFile, expectedExtValuesEhtFile, "")
{
    bool headerMatch = (expectedHeaderFile == nullptr && arg.headerFile == nullptr) || (expectedHeaderFile != nullptr && arg.headerFile != nullptr && std::string(arg.headerFile) == std::string(expectedHeaderFile));
    bool valuesMatch = (expectedValuesFile == nullptr && arg.valuesFile == nullptr) || (expectedValuesFile != nullptr && arg.valuesFile != nullptr && std::string(arg.valuesFile) == std::string(expectedValuesFile));
    bool extMatch = (expectedExtValuesEhtFile == nullptr && arg.extValuesEhtFile == nullptr) || (expectedExtValuesEhtFile != nullptr && arg.extValuesEhtFile != nullptr && std::string(arg.extValuesEhtFile) == std::string(expectedExtValuesEhtFile));
    return headerMatch && valuesMatch && extMatch;
}

class FunctionsTest : public ::testing::Test {
public:
    FunctionsTest()
    {
        m_config.failOnException = true;
    }

    std::shared_ptr<dut::Connection> m_connection = std::make_shared<dut::ConnectionMock>();
    std::shared_ptr<dut::Logger> m_logger = std::make_shared<dut::NullLogger>();

    dut_cli::Config m_config;
    StrictMock<dut_cli::ConsoleMock> m_console;
    StrictMock<dut::DutMock>* m_dut0 = new StrictMock<dut::DutMock>();
    dut::DutMockFactory m_dutFactory;

    dut_cli::Context m_context = dut_cli::Context(m_config, m_console, m_dutFactory, m_connection, m_logger);
};

TEST_F(FunctionsTest, driverInitShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, driverInit(false, dut::NvMemoryType::MEMORY_TYPE_FLASH, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::Band::BAND_2400MHZ)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 driverInit --memory-type 2 --memory-size 1024 --band 1", m_context));
}

TEST_F(FunctionsTest, driverInitShouldFailWithInvalidMemoryType)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 driverInit --memory-type 999 --memory-size 1024", m_context));
}

TEST_F(FunctionsTest, driverInitShouldFailWithInvalidMemorySize)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 driverInit --memory-type 2 --memory-size 999", m_context));
}

TEST_F(FunctionsTest, driverInitShouldFailWithInvalidBand)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 driverInit --memory-type 2 --memory-size 1024 --band 999", m_context));
}

TEST_F(FunctionsTest, driverReleaseShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, driverRelease()).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 driverRelease", m_context));
}

TEST_F(FunctionsTest, calculateMaxPacketLengthShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, calculateMaxPacketLength(dut::PhyMode::PHY_MODE_AX, _)).WillOnce(Invoke([](dut::PhyMode, uint32_t& maxPacketLength) {
            maxPacketLength = 1234;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Maximum packet length: 1234 bytes\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 calculateMaxPacketLength --phy-mode 7", m_context));
}

TEST_F(FunctionsTest, calculatePacketLengthShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, calculatePacketLength(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_QPSK_34, 1, 30, _)).WillOnce(Invoke([](dut::PhyMode, dut::Bandwidth, dut::Mcs, uint8_t, uint32_t, uint32_t& packetLength) {
            packetLength = 1234;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Packet length: 1234 bytes\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 calculatePacketLength --phy-mode 7 --bandwidth 0 --mcs 2 --nss 1 --num-symbols 30", m_context));
}

TEST_F(FunctionsTest, flushNvmShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, flushNvm()).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 flushNvm", m_context));
}

TEST_F(FunctionsTest, getAvailableRxAntennaMaskShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getAvailableRxAntennaMask(_)).WillOnce(Invoke([](dut::AntennaMask& antennaMask) {
            antennaMask = 0x0f;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Available RX antenna mask: 0x0f\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getAvailableRxAntennaMask", m_context));
}

TEST_F(FunctionsTest, getAvailableTxAntennaMaskShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getAvailableTxAntennaMask(_)).WillOnce(Invoke([](dut::AntennaMask& antennaMask) {
            antennaMask = 0x0f;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Available TX antenna mask: 0x0f\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getAvailableTxAntennaMask", m_context));
}

TEST_F(FunctionsTest, getBandShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getBand(_)).WillOnce(Invoke([](dut::Band& band) {
            band = dut::Band::BAND_2400MHZ;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Band: 2.4GHz\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getBand", m_context));
}

TEST_F(FunctionsTest, getSupportedBandsShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getSupportedBands(_)).WillOnce(Invoke([](std::vector<dut::Band>& supportedBands) {
            supportedBands.push_back(dut::Band::BAND_2400MHZ);
            supportedBands.push_back(dut::Band::BAND_5000MHZ);
            return true;
        }));
        EXPECT_CALL(m_console, cout("Supported bands: [2.4GHz,5GHz]\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getSupportedBands", m_context));
}

TEST_F(FunctionsTest, getBbicCddValuesShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getBbicCddValues(1, _, _, _)).WillOnce(Invoke([](uint8_t, uint32_t& offset1, uint32_t& offset2, uint32_t& offset3) {
            offset1 = 1;
            offset2 = 2;
            offset3 = 3;
            return true;
        }));
        EXPECT_CALL(m_console, cout("offset1: 1, offset2: 2, offset3: 3\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getBbicCddValues --num-tx-antennas 1", m_context));
}

TEST_F(FunctionsTest, getCalibrationFileVersionShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getCalibrationFileVersion(_, _)).WillOnce(Invoke([](dut::CalibrationFileVersion& version, dut::CalibrationFileSubversion& subversion) {
            version = dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6;
            subversion = dut::CalibrationFileSubversion::CALIBRATION_FILE_SUBVERSION_0;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Version: 6, Subversion: 0\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getCalibrationFileVersion", m_context));
}

TEST_F(FunctionsTest, getCardInfoShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getCardInfo(_, _, _, _, _)).WillOnce(Invoke([](uint8_t& countryCode, std::array<uint8_t, dut::cardInfoMacAddressSize>& macAddress, std::array<uint8_t, dut::cardInfoSerialNumberSize>& serialNumber, uint8_t& week, uint8_t& year) {
            countryCode = 34;
            macAddress = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
            serialNumber = { 0x0, 0x20, 0x11 };
            week = 41;
            year = 15;
            return true;
        }));
        EXPECT_CALL(m_console, cout("countryCode: 34, macAddress: 01:02:03:04:05:06, serialNumber: 002011, week: 41, year: 15\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getCardInfo", m_context));
}

TEST_F(FunctionsTest, getChipIdShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getChipId(_)).WillOnce(Invoke([](dut::ChipID& chipId) {
            chipId = dut::ChipID::CHIP_ID_GEN6_B;
            return true;
        }));
        EXPECT_CALL(m_console, cout("chipId: 0x0910\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getChipId", m_context));
}

TEST_F(FunctionsTest, getComponentVersionShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getComponentVersion(_, _)).WillOnce(Invoke([](dut::VersionedComponent, std::string& version) {
            version = "610.0.1.1234";
            return true;
        }));
        EXPECT_CALL(m_console, cout("version: 610.0.1.1234\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getComponentVersion --component 0", m_context));
}

TEST_F(FunctionsTest, getEnabledRxAntennaMaskShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getEnabledRxAntennaMask(_)).WillOnce(Invoke([](dut::AntennaMask& antennaMask) {
            antennaMask = 0x0f;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Enabled RX antenna mask: 0x0f\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getEnabledRxAntennaMask", m_context));
}

TEST_F(FunctionsTest, getEnabledTxAntennaMaskShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getEnabledTxAntennaMask(_)).WillOnce(Invoke([](dut::AntennaMask& antennaMask) {
            antennaMask = 0x0f;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Enabled TX antenna mask: 0x0f\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getEnabledTxAntennaMask", m_context));
}

TEST_F(FunctionsTest, getFemTypeShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getFemType(_)).WillOnce(Invoke([](dut::FemType& femType) {
            femType = dut::FemType::FEM_TYPE_NON_LINEAR;
            return true;
        }));
        EXPECT_CALL(m_console, cout("FEM type: Non-Linear\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getFemType", m_context));
}

TEST_F(FunctionsTest, getHardwareTypeShouldSucceedWithGen6)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getHardwareType(_)).WillOnce(Invoke([](dut::HardwareType& hardwareType) {
            hardwareType = dut::HardwareType::HARDWARE_TYPE_GEN6;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Hardware type: GEN6\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getHardwareType", m_context));
}

TEST_F(FunctionsTest, getHardwareTypeShouldSucceedWithGen7)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getHardwareType(_)).WillOnce(Invoke([](dut::HardwareType& hardwareType) {
            hardwareType = dut::HardwareType::HARDWARE_TYPE_GEN7;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Hardware type: GEN7\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getHardwareType", m_context));
}

TEST_F(FunctionsTest, getInbandRssiShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getInbandRssi(_)).WillOnce(Invoke([](std::array<int16_t, dut::maxNumRxAntennas>& rssi) {
            for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
                rssi[i] = static_cast<uint16_t>(i);
            }
            return true;
        }));
        EXPECT_CALL(m_console, cout("RSSI: [0,1,2,3,4]\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getInbandRssi", m_context));
}

TEST_F(FunctionsTest, getMacPacketCountersShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getMacPacketCounters(_)).WillOnce(Invoke([](uint32_t& receivedPackets) {
            receivedPackets = 123;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Received packets: 123\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getMacPacketCounters", m_context));
}

TEST_F(FunctionsTest, getMpduPacketCountersShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getMpduPacketCounters(_, _)).WillOnce(Invoke([](uint32_t& receivedPackets, uint32_t& errorPackets) {
            receivedPackets = 123;
            errorPackets = 456;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Received packets: 123, Error packets: 456\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getMpduPacketCounters", m_context));
}

TEST_F(FunctionsTest, getNmseValuesShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));

        EXPECT_CALL(*m_dut0, getNmseValues(_)).WillOnce(Invoke([](std::array<std::array<int32_t, dut::dpdTotalCalibrationPoints>, dut::maxNumTxAntennas>& values) {
            for (size_t i = 0; i < dut::maxNumTxAntennas; i++) {
                for (size_t j = 0; j < dut::dpdTotalCalibrationPoints; j++) {
                    values[i][j] = static_cast<int32_t>(j);
                }
            }
            return true;
        }));
        EXPECT_CALL(m_console, cout("DPD NMSE Values:\n"
                                    "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14\n"
                                    "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14\n"
                                    "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14\n"
                                    "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14\n"))
            .Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getNmseValues", m_context));
}

TEST_F(FunctionsTest, getNvmSizeShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getNvmSize(_)).WillOnce(Invoke([](size_t& size) {
            size = 1024;
            return true;
        }));
        EXPECT_CALL(m_console, cout("NVM size: 1024\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getNvmSize", m_context));
}

TEST_F(FunctionsTest, getNvmTypeShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getNvmType(_)).WillOnce(Invoke([](dut::NvMemoryType& type) {
            type = dut::NvMemoryType::MEMORY_TYPE_FLASH;
            return true;
        }));
        EXPECT_CALL(m_console, cout("NVM type: Flash\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getNvmType", m_context));
}

TEST_F(FunctionsTest, getNvmVersionShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getNvmVersion(_)).WillOnce(Invoke([](uint8_t& version) {
            version = 6;
            return true;
        }));
        EXPECT_CALL(m_console, cout("NVM version: 6\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getNvmVersion", m_context));
}

TEST_F(FunctionsTest, getPhyModeShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getPhyMode(_)).WillOnce(Invoke([](dut::PhyMode& phyMode) {
            phyMode = dut::PhyMode::PHY_MODE_AX;
            return true;
        }));
        EXPECT_CALL(m_console, cout("PHY mode: AX\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getPhyMode", m_context));
}

TEST_F(FunctionsTest, getPhyPacketCountersShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getPhyPacketCounters(_, _, _)).WillOnce(Invoke([](uint32_t& receivedPackets, uint32_t& crcErrors, uint32_t& forwardedPackets) {
            receivedPackets = 123;
            crcErrors = 456;
            forwardedPackets = 789;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Received packets: 123, CRC errors: 456, forwarded packets: 789\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getPhyPacketCounters", m_context));
}

TEST_F(FunctionsTest, getProductionFlagShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getProductionFlag(_)).WillOnce(Invoke([](bool& productionFlag) {
            productionFlag = true;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Production flag: true\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getProductionFlag", m_context));
}

TEST_F(FunctionsTest, getRxEvmShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getRxEvm(_)).WillOnce(Invoke([](std::array<uint8_t, dut::maxNumRxAntennas>& rxEvm) {
            for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
                rxEvm[i] = static_cast<uint8_t>(i) * 10 + 1;
            }
            return true;
        }));
        EXPECT_CALL(m_console, cout("RX EVM: [0.5,5.5,10.5,15.5,20.5]\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getRxEvm", m_context));
}

TEST_F(FunctionsTest, getRxRateInfoShouldSucceedWithPhyModeAx)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getPhyMode(_)).WillOnce(Invoke([](dut::PhyMode& phyMode) {
            phyMode = dut::PhyMode::PHY_MODE_AX;
            return true;
        }));
        EXPECT_CALL(*m_dut0, getRxRateInfo(_, _)).WillOnce(Invoke([](uint8_t& mcs, uint8_t& nss) {
            mcs = 7;
            nss = 2;
            return true;
        }));
        EXPECT_CALL(m_console, cout("RX Rate Info: MCS=7, NSS=2\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getRxRateInfo", m_context));
}

TEST_F(FunctionsTest, getRxRateInfoShouldSucceedWithPhyModeA)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getPhyMode(_)).WillOnce(Invoke([](dut::PhyMode& phyMode) {
            phyMode = dut::PhyMode::PHY_MODE_A;
            return true;
        }));
        EXPECT_CALL(*m_dut0, getRxRateInfo(_, _)).WillOnce(Invoke([](uint8_t& mcs, uint8_t& nss) {
            mcs = 8;
            nss = 0;
            return true;
        }));
        EXPECT_CALL(m_console, cout("RX Rate Info: 6:64QAM 2/3: 48 Mbps\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getRxRateInfo", m_context));
}

TEST_F(FunctionsTest, getRxRateInfoShouldSucceedWithPhyModeB)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getPhyMode(_)).WillOnce(Invoke([](dut::PhyMode& phyMode) {
            phyMode = dut::PhyMode::PHY_MODE_B;
            return true;
        }));
        EXPECT_CALL(*m_dut0, getRxRateInfo(_, _)).WillOnce(Invoke([](uint8_t& mcs, uint8_t& nss) {
            mcs = 2;
            nss = 0;
            return true;
        }));
        EXPECT_CALL(m_console, cout("RX Rate Info: 5.5 Mbps\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getRxRateInfo", m_context));
}

TEST_F(FunctionsTest, getTemperatureShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getTemperature(_)).WillOnce(Invoke([](float& temperature) {
            temperature = 12.34f;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Temperature: 12.34 degrees Celsius\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getTemperature", m_context));
}

TEST_F(FunctionsTest, getTransmitVoltagesShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getTransmitVoltages(_)).WillOnce(Invoke([](std::array<uint32_t, dut::maxNumTxAntennas>& voltages) {
            for (uint32_t i = 0; i < dut::maxNumTxAntennas; i++) {
                voltages[i] = i;
            }
            return true;
        }));
        EXPECT_CALL(m_console, cout("TX voltages: [0,1,2,3]\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getTransmitVoltages", m_context));
}

TEST_F(FunctionsTest, getXtalCalValueShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getXtalCalValue(_)).WillOnce(Invoke([](uint16_t& value) {
            value = 0xabcd;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Xtal cal value: 0xabcd\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getXtalCalValue", m_context));
}

TEST_F(FunctionsTest, getXtalRegValueShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getXtalRegValue(_)).WillOnce(Invoke([](uint16_t& value) {
            value = 0xabcd;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Xtal reg value: 0xabcd\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getXtalRegValue", m_context));
}

TEST_F(FunctionsTest, getZwdfsStatusShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, getZwdfsStatus(_, _)).WillOnce(Invoke([](dut::AntennaMask& antennaMask, bool& enabled) {
            antennaMask = 0x10;
            enabled = true;
            return true;
        }));
        EXPECT_CALL(m_console, cout("ZWDFS antenna mask: 0x10\n")).Times(1);
        EXPECT_CALL(m_console, cout("ZWDFS enabled: true\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 getZwdfsStatus", m_context));
}

TEST_F(FunctionsTest, loadBeamformingMatrixFromFileShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));

        // Verify the correct arguments are passed to loadBeamformingMatrixFromFileSet
        EXPECT_CALL(*m_dut0, loadBeamformingMatrixFromFileSet(beamformingFilePathSetMatcher("beamforming-matrix-header.txt", "beamforming-matrix-values.txt", nullptr), beamformingFilePathSetMatcher(nullptr, nullptr, nullptr))).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 loadBeamformingMatrixFromFile --header-filename beamforming-matrix-header.txt --values-filename beamforming-matrix-values.txt", m_context));
}

TEST_F(FunctionsTest, loadBeamformingMatrixFromFileShouldSucceedWithOptionalArguments)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));

        // Verify that optional arguments are correctly parsed and passed to loadBeamformingMatrixFromFileSet
        EXPECT_CALL(*m_dut0, loadBeamformingMatrixFromFileSet(beamformingFilePathSetMatcher("beamforming-matrix-header.txt", "beamforming-matrix-values.txt", "beamforming-matrix-ext-eht.txt"), beamformingFilePathSetMatcher("secondary-header.txt", "secondary-values.txt", "secondary-ext-eht.txt"))).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 loadBeamformingMatrixFromFile --header-filename beamforming-matrix-header.txt --values-filename beamforming-matrix-values.txt --ext-values-eht-filename beamforming-matrix-ext-eht.txt --secondary-header-filename secondary-header.txt --secondary-values-filename secondary-values.txt --secondary-ext-values-eht-filename secondary-ext-eht.txt", m_context));
}

TEST_F(FunctionsTest, loadBeamformingMatrixFromFileShouldSucceedWithPartialOptionalArguments)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));

        // Verify that only EHT extension file is parsed correctly (without secondary files)
        EXPECT_CALL(*m_dut0, loadBeamformingMatrixFromFileSet(beamformingFilePathSetMatcher("beamforming-matrix-header.txt", "beamforming-matrix-values.txt", "beamforming-matrix-ext-eht.txt"), beamformingFilePathSetMatcher(nullptr, nullptr, nullptr))).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 loadBeamformingMatrixFromFile --header-filename beamforming-matrix-header.txt --values-filename beamforming-matrix-values.txt --ext-values-eht-filename beamforming-matrix-ext-eht.txt", m_context));
}

TEST_F(FunctionsTest, loadNvmFromFileShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, loadNvmFromFile("cal_wlan0.bin")).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 loadNvmFromFile --filename cal_wlan0.bin", m_context));
}

TEST_F(FunctionsTest, readMemoryShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, readMemory(dut ::ChipModule::CHIP_MODULE_LMAC_MEM, 0x1, _, 8)).WillOnce(Invoke([](dut::ChipModule, size_t, uint8_t* data, size_t length) {
            for (size_t i = 0; i < length; i++) {
                data[i] = static_cast<uint8_t>(i);
            }
            return true;
        }));
        EXPECT_CALL(m_console, cout("00 01 02 03 04 05 06 07\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 readMemory --chip-module 2 --address 0x1 --length 8", m_context));
}

TEST_F(FunctionsTest, readMemoryShouldFailWithInvalidModule)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 readMemory --chip-module 999 --address 0x1 --length 8", m_context));
}

TEST_F(FunctionsTest, readNvmShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, readNvm(0, _, 33, false)).WillOnce(Invoke([](size_t, uint8_t* data, size_t length, bool) {
            for (size_t i = 0; i < length; i++) {
                data[i] = static_cast<uint8_t>(i);
            }
            return true;
        }));
        EXPECT_CALL(m_console, cout("00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f\n20\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 readNvm --address 0 --length 33", m_context));
}

TEST_F(FunctionsTest, readRegisterShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, readRegister(dut::ChipModule::CHIP_MODULE_LMAC_MEM, 0, 0x0123, _)).WillOnce(Invoke([](dut::ChipModule, size_t, uint32_t, uint32_t& value) {
            value = 0xbeef;
            return true;
        }));
        EXPECT_CALL(m_console, cout("Register value: 0x0000beef\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 readRegister --chip-module 2 --address 0 --mask 0x0123", m_context));
}

TEST_F(FunctionsTest, readRegisterShouldFailWithInvalidModule)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 readRegister --chip-module 999 --address 0 --mask 0x0123", m_context));
}

TEST_F(FunctionsTest, resetMacPacketCountersShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, resetMacPacketCounters()).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 resetMacPacketCounters", m_context));
}

TEST_F(FunctionsTest, resetMpduPacketCountersShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, resetMpduPacketCounters()).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 resetMpduPacketCounters", m_context));
}

TEST_F(FunctionsTest, resetPhyPacketCountersShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, resetPhyPacketCounters()).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 resetPhyPacketCounters", m_context));
}

TEST_F(FunctionsTest, rxMeasureShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, rxMeasure(1, 1000, false)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 rxMeasure --num-captures 1 --interval 1000", m_context));
}

TEST_F(FunctionsTest, rxMeasureDisableShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, rxMeasure(1, 1000, true)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 rxMeasure --disable", m_context));
}

TEST_F(FunctionsTest, saveNvmToFileShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, saveNvmToFile("cal_wlan0.bin")).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 saveNvmToFile --filename cal_wlan0.bin", m_context));
}

TEST_F(FunctionsTest, setBbicCddValuesShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setBbicCddValues(3, 1, 2, 0)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setBbicCddValues --num-tx-antennas 3 -1 1 --offset2 2", m_context));
}

TEST_F(FunctionsTest, setCalibrationFileVersionShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setCalibrationFileVersion(dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6, dut::CalibrationFileSubversion::CALIBRATION_FILE_SUBVERSION_0)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setCalibrationFileVersion --cal-file-version 6 --cal-file-subversion 0", m_context));
}

TEST_F(FunctionsTest, setCardInfoShouldSucceed)
{
    const std::array<uint8_t, dut::cardInfoMacAddressSize> expectedMacAddress { 0x1, 0x2, 0x3, 0x4, 0x5, 0x6 };
    const std::array<uint8_t, dut::cardInfoSerialNumberSize> expectedSerialNumber { 0x40, 0xe2, 0x01 };

    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setCardInfo(34, expectedMacAddress, expectedSerialNumber, 41, 15)).WillOnce(Return(true));
        EXPECT_CALL(*m_dut0, setCardInfo(34, expectedMacAddress, expectedSerialNumber, 41, 15)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setCardInfo --country-code 34 --mac-address 01:02:03:04:05:06 --serial-number 123456 --week 41 --year 15", m_context));
    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setCardInfo --country-code 34 --mac-address 01-02-03-04-05-06 --serial-number 123456 --week 41 --year 15", m_context));
}

TEST_F(FunctionsTest, setCardInfoShouldFailWithInvalidMacAddress)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 setCardInfo --country-code 34 --mac-address 999 --serial-number 123456 --week 41 --year 15", m_context));
}

TEST_F(FunctionsTest, setCardInfoShouldFailWithInvalidSerialNumber)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 setCardInfo --country-code 34 --mac-address 01:02:03:04:05:06 --serial-number 1 --week 41 --year 15", m_context));
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 setCardInfo --country-code 34 --mac-address 01:02:03:04:05:06 --serial-number abcdef --week 41 --year 15", m_context));
}

TEST_F(FunctionsTest, setChannelShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setChannel(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_TWENTY, 36, 0, dut::RegulationType::REGULATION_TYPE_FCC_SP)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setChannel --phy-mode 7 --bandwidth 0 --channel 36 --index 0 --regulation_type 0x10", m_context));
}

TEST_F(FunctionsTest, setChannelShouldFailWithInvalidPhyMode)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 setChannel --phy-mode 999 --bandwidth 0 --channel 36 --index 0 --regulation_type 0x10", m_context));
}

TEST_F(FunctionsTest, setChannelShouldFailWithInvalidBandwidth)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 setChannel --phy-mode 7 --bandwidth 999 --channel 36 --index 0 --regulation_type 0x10", m_context));
}

TEST_F(FunctionsTest, setChannelShouldFailWithInvalidPrimaryChannelIndex)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 setChannel --phy-mode 7 --bandwidth 0 --channel 36 --index 999 --regulation_type 0x10", m_context));
}

TEST_F(FunctionsTest, setChannelShouldFailWithInvalidRegulationType)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 setChannel --phy-mode 7 --bandwidth 0 --channel 36 --index 0 --regulation_type 0xff", m_context));
}

TEST_F(FunctionsTest, setClipperShouldSucceedWithArgSet)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setClipper(true)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setClipper --enabled", m_context));
}

TEST_F(FunctionsTest, setClipperShouldSucceedWithArgNotSet)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setClipper(false)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setClipper", m_context));
}

TEST_F(FunctionsTest, setEnabledRxAntennaMaskShouldSucceed)
{
    const dut::AntennaMask expectedAntennaMask(0xf);

    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setEnabledRxAntennaMask(expectedAntennaMask)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setEnabledRxAntennaMask --antenna-mask 15", m_context));
}

TEST_F(FunctionsTest, setEnabledTxAntennaMaskShouldSucceed)
{
    const dut::AntennaMask expectedAntennaMask(0xf);

    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setEnabledTxAntennaMask(expectedAntennaMask)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setEnabledTxAntennaMask --antenna-mask 0xf", m_context));
}

TEST_F(FunctionsTest, setIfsShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setIfs(16)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setIfs --ifs 16", m_context));
}

TEST_F(FunctionsTest, setProductionFlagShouldSucceedIfEnabled)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setProductionFlag(true, false)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setProductionFlag --production-flag", m_context));
}

TEST_F(FunctionsTest, setRateShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0,
            setRate(dut::Bandwidth::BANDWIDTH_TWENTY, dut::Mcs::MCS_QPSK_34, 1, dut::Gi::GI_0_8_US, dut::Ltf::LTF_MEDIUM, _))
            .WillOnce(Invoke([](dut::Bandwidth, dut::Mcs, uint8_t, dut::Gi, dut::Ltf, float& rateMbps) {
                rateMbps = 25.8f;
                return true;
            }));
        EXPECT_CALL(m_console, cout("PHY rate: 25.8 Mbps\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setRate --bandwidth 0 --mcs 2 --nss 1 --gi 0 --ltf 1", m_context));
}

TEST_F(FunctionsTest, setRuParamsShouldSucceedWithBothUsers)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setRuParams(100, 200)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setRuParams --user-one 100 --user-two 200", m_context));
}

TEST_F(FunctionsTest, setRuParamsShouldSucceedWithOnlyUserOne)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setRuParams(100, 512)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setRuParams --user-one 100", m_context));
}

TEST_F(FunctionsTest, setRxAggregationEnabledShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setRxAggregationEnabled(true)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setRxAggregationEnabled --enabled", m_context));
}

TEST_F(FunctionsTest, setSpacelessTxEnabledShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setSpacelessTxEnabled(true)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setSpacelessTxEnabled --enabled", m_context));
}

TEST_F(FunctionsTest, setTransmitPowerControlShouldSucceedIfClosedLoop)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setTransmitPowerControl(true, 123)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setTransmitPowerControl --closed-loop --power-limit 123", m_context));
}

TEST_F(FunctionsTest, setTransmitPowerControlShouldSucceedIfOpenLoop)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setTransmitPowerControl(false, 123)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setTransmitPowerControl --power-limit 123", m_context));
}

TEST_F(FunctionsTest, setTransmitPowerLevelShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setTransmitPowerLevel(123)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setTransmitPowerLevel --power-level 123", m_context));
}

TEST_F(FunctionsTest, setTransmitNegativePowerLevelShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setTransmitPowerLevel(-18)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setTransmitPowerLevel --power-level -18", m_context));
}

TEST_F(FunctionsTest, setXtalCalValueShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setXtalCalValue(123)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setXtalCalValue --xtal-value 123", m_context));
}

TEST_F(FunctionsTest, setXtalRegValueShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setXtalRegValue(123)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setXtalRegValue --xtal-value 123", m_context));
}

TEST_F(FunctionsTest, setZwdfsConfigurationShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, setZwdfsConfiguration(52, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, dut::Bandwidth::BANDWIDTH_EIGHTY)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 setZwdfsConfiguration --channel 52 --radarDetectionBandwidth 2", m_context));
}

TEST_F(FunctionsTest, setZwdfsConfigurationShouldFailWithInvalidZwdfsBandwidth)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 setZwdfsConfiguration --channel 52 --bandwidth 255", m_context));
}

TEST_F(FunctionsTest, setZwdfsConfigurationShouldFailWithInvalidRadarDetectionBandwidth)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 setZwdfsConfiguration --channel 52 --radarDetectionBandwidth 0", m_context));
}

TEST_F(FunctionsTest, startCalibrationSucceed)
{
    {
        InSequence sequence;

        dut::StartCalibrationParams_t expectedParams {};
        expectedParams.type = 1;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, startCalibration(startCalibrationParamsMatcher(expectedParams), _)).WillOnce(Invoke([](const dut::StartCalibrationParams_t& params, uint8_t& status) {
            status = 0;

            return true;
        }));
        EXPECT_CALL(m_console, cout("Start Calibration: status 0\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 startCalibration -c 1", m_context));
}

TEST_F(FunctionsTest, startCalibrationWithHexMaskShouldSucceed)
{
    {
        InSequence sequence;

        dut::StartCalibrationParams_t expectedParams {};
        expectedParams.type = 1;
        expectedParams.maskType = 1;
        expectedParams.mask = 0x2708;

        uint8_t status {};

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, startCalibration(startCalibrationParamsMatcher(expectedParams), _)).WillOnce(Invoke([](const dut::StartCalibrationParams_t& params, uint8_t& status) {
            status = 0;

            return true;
        }));
        EXPECT_CALL(m_console, cout("Start Calibration: status 0\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 startCalibration -t 1 -m 0x2708", m_context));
}

TEST_F(FunctionsTest, startCwShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, startCw(10, 4)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 startCw --amplitude 10 --tone 4", m_context));
}

TEST_F(FunctionsTest, startTxShouldSucceedDefaultCoding)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, startTx(65535, 1234, true, true, dut::CodingType::CODING_TYPE_AUTO)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 startTx --repetitions 65535 --packet-length 1234 --long-data --beamforming", m_context));
}

TEST_F(FunctionsTest, startTxShouldSucceedAutoCoding)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, startTx(65535, 1234, true, true, dut::CodingType::CODING_TYPE_AUTO)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 startTx --repetitions 65535 --packet-length 1234 --long-data --beamforming --coding-type 0", m_context));
}

TEST_F(FunctionsTest, startTxShouldSucceedBccCoding)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, startTx(65535, 1234, true, true, dut::CodingType::CODING_TYPE_BCC)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 startTx --repetitions 65535 --packet-length 1234 --long-data --beamforming --coding-type 1", m_context));
}

TEST_F(FunctionsTest, startTxShouldSucceedLdpcCoding)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, startTx(65535, 1234, true, true, dut::CodingType::CODING_TYPE_LDPC)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 startTx --repetitions 65535 --packet-length 1234 --long-data --beamforming --coding-type 2", m_context));
}

TEST_F(FunctionsTest, startRxPerShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, startRxPer(10000)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 startRxPer --packet-limit 10000", m_context));
}

TEST_F(FunctionsTest, stopRxPerShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, stopRxPer(true)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 stopRxPer --calc-per", m_context));
}

TEST_F(FunctionsTest, stopCwShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, stopCw()).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 stopCw", m_context));
}

TEST_F(FunctionsTest, stopTxShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, stopTx()).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 stopTx", m_context));
}

TEST_F(FunctionsTest, validateBeamformingHeaderRegisterShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, validateBeamformingHeaderRegister(dut::PhyMode::PHY_MODE_AX, dut::Bandwidth::BANDWIDTH_EIGHTY)).WillOnce(Return(true));
        EXPECT_CALL(m_console, cout("Beamforming header register is valid.\n")).Times(1);
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 validateBeamformingHeaderRegister --phy-mode 7 --bandwidth 2", m_context));
}

TEST_F(FunctionsTest, validateBeamformingHeaderRegisterShouldFail)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, validateBeamformingHeaderRegister(dut::PhyMode::PHY_MODE_N_5, dut::Bandwidth::BANDWIDTH_FOURTY)).WillOnce(Return(false));
        EXPECT_CALL(m_console, cout("Beamforming header register is NOT valid.\n")).Times(1);
    }

    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 validateBeamformingHeaderRegister --phy-mode 4 --bandwidth 1", m_context));
}

TEST_F(FunctionsTest, writeCalibrationFileShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, writeCalibrationFile(dut::NvMemoryType::MEMORY_TYPE_FLASH, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 writeCalibrationFile --memory-type 2 --memory-size 1024", m_context));
}

TEST_F(FunctionsTest, writeMemoryShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, writeMemory(dut::ChipModule::CHIP_MODULE_LMAC_MEM, 0x1, _, 8)).WillOnce(Invoke([](dut::ChipModule, size_t, const uint8_t* data, size_t length) {
            const std::vector<uint8_t> expectedData { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef };
            for (size_t i = 0; i < length; i++) {
                if (data[i] != expectedData.at(i)) {
                    return false;
                }
            }
            return true;
        }));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 writeMemory --chip-module 2 --address 0x1 --data 0123456789abcdef", m_context));
}

TEST_F(FunctionsTest, writeMemoryShouldFailWithInvalidModule)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 writeMemory --chip-module 999 --address 0x1 --data 0123456789abcdef", m_context));
}

TEST_F(FunctionsTest, writeMemoryShouldFailWithInvalidData)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 writeMemory --address 0x1 --data 000z00", m_context));
}

TEST_F(FunctionsTest, writeMemoryShouldFailWithOddSizedHexString)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 writeMemory --address 0x1 --data 0", m_context));
}

TEST_F(FunctionsTest, writeNvmShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, writeNvm(0x1, _, 8)).WillOnce(Invoke([](size_t, const uint8_t* data, size_t length) {
            const std::vector<uint8_t> expectedData { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef };
            for (size_t i = 0; i < length; i++) {
                if (data[i] != expectedData.at(i)) {
                    return false;
                }
            }
            return true;
        }));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 writeNvm --address 0x1 --data 0123456789abcdef", m_context));
}

TEST_F(FunctionsTest, writeRegisterShouldSucceed)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, writeRegister(dut::ChipModule::CHIP_MODULE_LMAC_MEM, 0, 0x0123, 0x4567)).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 writeRegister --chip-module 2 --address 0 --mask 0x0123 --value 0x04567", m_context));
}

TEST_F(FunctionsTest, writeRegisterShouldFailWithInvalidModule)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 writeRegister --chip-module 999 --address 0 --mask 0x0123 --value 0x04567", m_context));
}

} // namespace
