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

#ifndef _DUT_DUT_MOCK_H_
#define _DUT_DUT_MOCK_H_

#include "dut/Dut.h"

#include <array>
#include <memory>
#include <string>

#include <gmock/gmock.h>

// How to use GoogleMock in Visual Studio?
// https://stackoverflow.com/a/60486111

namespace dut {

class DutMock : public Dut {
public:
    MOCK_METHOD(std::string, getLastError, (), (const override));
    MOCK_METHOD(void, setThrowExceptions, (bool throwExceptions), (override));
    MOCK_METHOD(bool, calibrateTssiS2d, (uint8_t lowerPowerLevel, uint8_t upperPowerLevel, uint32_t tssiLow, uint32_t tssiHigh, uint8_t region, uint8_t powerThreshold, uint8_t& gain, uint8_t& offset), (override));
    MOCK_METHOD(bool, calculateLinearRegression, ((const std::array<float, numPowerPoints>&)powerVector, (const std::array<std::array<uint16_t, numPowerPoints>, maxNumRxAntennas>&)voltages, (const std::array<uint16_t, maxNumRxAntennas>&)offsets, (const std::array<float, maxNumRxAntennas>&)lnaGains, (std::array<RssiAB_t, maxNumRxAntennas>&)result, (std::array<float, maxNumRxAntennas>&)maxErrors), (override));
    MOCK_METHOD(bool, calculateOptimalS2dGains, ((const std::array<uint16_t, maxNumRxAntennas>&)rssiVoltage1, (const std::array<uint16_t, maxNumRxAntennas>&)rssiVoltage2, int32_t regionS2dGain, (std::array<int32_t, maxNumRxAntennas>&)optimalS2dGains), (override));
    MOCK_METHOD(bool, calculateOptimalS2dOffsets, ((const std::array<uint16_t, maxNumRxAntennas>&)rssiVoltage1, (const std::array<uint16_t, maxNumRxAntennas>&)rssiVoltage2, int32_t regionS2dGain, uint8_t regionS2dOffset, (std::array<uint8_t, maxNumRxAntennas>&)optimalS2dOffsets), (override));
    MOCK_METHOD(bool, calculateMaxPacketLength, (PhyMode phyMode, uint32_t& maxPacketLength), (override));
    MOCK_METHOD(bool, calculatePacketLength, (PhyMode phyMode, Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, uint32_t numSymbols, uint32_t& packetLength), (override));
    MOCK_METHOD(bool, calculatePhyDataRate, (PhyMode phyMode, Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, Gi gi, float& rateMbps), (override));
    MOCK_METHOD(bool, calibrateRxLnaMidGains, ((const std::array<float, maxNumRxAntennas>&)targetGains, (const std::array<float, maxNumRxAntennas>&)prxin, uint16_t numSamples, int8_t pgc1, int8_t pgc2, (std::array<float, maxNumRxAntennas>&)calcGain, (std::array<uint8_t, maxNumRxAntennas>&)calcGainCtrl), (override));
    MOCK_METHOD(bool, calculateRxLnaGains, ((const std::array<CorrelationResults_t, maxNumRxAntennas>&)correlationResults, (const std::array<float, maxNumRxAntennas>&)prxin, int8_t pgc1, int8_t pgc2, int8_t pgc3, (std::array<float, maxNumRxAntennas>&)gains), (override));
    MOCK_METHOD(bool, calculateRxLnaSubBandGains, ((std::array<std::array<uint16_t, maxNumLnaSubBands>, maxNumRxAntennas>&)freqResultTable), (override));
    MOCK_METHOD(bool, calculateRxPowerVector, (int32_t maxPower, (const std::array<float, maxNumRxAntennas>&)lnaGains, int32_t lowerPowerBoundary, int32_t upperPowerBoundary, (std::array<float, numPowerPoints>&)powerVector), (override));
    MOCK_METHOD(bool, calculateRxRfFlatness, ((const std::array<std::array<int32_t, maxNumLnaSubBands>, maxNumRxAntennas>&)P_Adc_array, (const std::array<float, maxNumRxAntennas>&)refGain, int8_t pgc1, int8_t pgc2, int8_t pgc3, (const std::array<std::array<float, maxNumLnaSubBands>, maxNumRxAntennas>&)pin, (std::array<std::array<float, maxNumLnaSubBands>, maxNumRxAntennas>&)results), (override));
    MOCK_METHOD(bool, calculateTxABCoefficients, (const AntennaMask& antennaMask, uint8_t numRegions, (const std::array<std::array<std::vector<Point_t>, maxNumTxRegions>, maxNumTxAntennas>&)pointsPerAntennaAndRegion, float nLog, (std::array<std::array<TssiAB_t, maxNumTxRegions>, maxNumTxAntennas>&)coefficientsPerAntennaAndRegion, (std::array<std::array<float, maxNumTxRegions>, maxNumTxAntennas>&)errorsPerAntennaAndRegion), (override));
    MOCK_METHOD(bool, driverInit, (bool snifferMode, NvMemoryType memoryType, NvMemorySize memorySize, Band band), (override));
    MOCK_METHOD(bool, driverRelease, (), (override));
    MOCK_METHOD(bool, flushNvm, (), (override));
    MOCK_METHOD(bool, getAvailableRxAntennaMask, (AntennaMask & antennaMask), (override));
    MOCK_METHOD(bool, getAvailableTxAntennaMask, (AntennaMask & antennaMask), (override));
    MOCK_METHOD(bool, getBand, (Band & band), (override));
    MOCK_METHOD(bool, getSupportedBands, (std::vector<Band> & supportedBands), (override));
    MOCK_METHOD(bool, getBbicCddValues, (uint8_t numTxAntennas, uint32_t& offset1, uint32_t& offset2, uint32_t& offset3), (override));
    MOCK_METHOD(bool, getCalibrationFileVersion, (CalibrationFileVersion & version, CalibrationFileSubversion& subversion), (override));
    MOCK_METHOD(bool, getCardInfo, (uint8_t & countryCode, (std::array<uint8_t, cardInfoMacAddressSize>&)macAddress, (std::array<uint8_t, cardInfoSerialNumberSize>&)serialNumber, uint8_t& week, uint8_t& year), (override));
    MOCK_METHOD(bool, getChipId, (ChipID & chipId), (override));
    MOCK_METHOD(bool, getComponentVersion, (VersionedComponent component, std::string& version), (override));
    MOCK_METHOD(bool, getEnabledRxAntennaMask, (AntennaMask & antennaMask), (override));
    MOCK_METHOD(bool, getEnabledTxAntennaMask, (AntennaMask & antennaMask), (override));
    MOCK_METHOD(bool, getFemType, (FemType & femType), (override));
    MOCK_METHOD(bool, getHardwareType, (HardwareType & hardwareType), (override));
    MOCK_METHOD(bool, getInbandRssi, ((std::array<int16_t, maxNumRxAntennas>&)rssi), (override));
    MOCK_METHOD(bool, getMacPacketCounters, (uint32_t & receivedPackets), (override));
    MOCK_METHOD(bool, getMpduPacketCounters, (uint32_t & receivedPackets, uint32_t& errorPackets), (override));
    MOCK_METHOD(bool, getNmseValues, ((std::array<std::array<int32_t, dpdTotalCalibrationPoints>, maxNumTxAntennas>&)values), (override));
    MOCK_METHOD(bool, getNvmSize, (size_t & size), (override));
    MOCK_METHOD(bool, getNvmType, (NvMemoryType & type), (override));
    MOCK_METHOD(bool, getNvmVersion, (uint8_t & version), (override));
    MOCK_METHOD(bool, getPhyMode, (PhyMode & phyMode), (override));
    MOCK_METHOD(bool, getPhyPacketCounters, (uint32_t & receivedPackets, uint32_t& crcErrors, uint32_t& forwardedPackets), (override));
    MOCK_METHOD(bool, getProductionFlag, (bool& productionFlag), (override));
    MOCK_METHOD(bool, getRfRssiPower, ((std::array<int8_t, maxNumRxAntennas>&)power), (override));
    MOCK_METHOD(bool, getRssi, (uint8_t method, uint16_t numSamples, (std::array<uint16_t, maxNumRxAntennas>&)rssi), (override));
    MOCK_METHOD(bool, getRssiCalibrationData, (CalibrationFileVersion & version, std::vector<RssiCalibrationData_t>& data), (override));
    MOCK_METHOD(bool, getRxCwPower, (uint8_t method, int32_t freqOffset, uint16_t numSamples, (std::array<CorrelationResults_t, maxNumRxAntennas>&)correlationResults), (override));
    MOCK_METHOD(bool, getRxEvm, ((std::array<uint8_t, maxNumRxAntennas>&)rxEvm), (override));
    MOCK_METHOD(bool, getRxRateInfo, (uint8_t & mcs, uint8_t& nss), (override));
    MOCK_METHOD(bool, getTemperature, (float& temperature), (override));
    MOCK_METHOD(bool, getTransmitPowerTableOffset, (uint8_t antenna, Bandwidth bandwidth, int16_t& offset), (override));
    MOCK_METHOD(bool, getTransmitVoltages, ((std::array<uint32_t, maxNumTxAntennas>&)voltages), (override));
    MOCK_METHOD(bool, getTssiCalibrationData, (CalibrationFileVersion & version, std::vector<TssiCalibrationData_t>& data), (override));
    MOCK_METHOD(bool, getXtalCalValue, (uint16_t & xtalValue), (override));
    MOCK_METHOD(bool, getXtalRegValue, (uint16_t & xtalValue), (override));
    MOCK_METHOD(bool, getZwdfsStatus, (AntennaMask & antennaMask, bool& enabled), (override));
    MOCK_METHOD(bool, loadBeamformingMatrixFromFileSet, (const BeamformingFilePathSet_t& primarySet, const BeamformingFilePathSet_t& secondarySet), (override));
    MOCK_METHOD(bool, loadNvmFromFile, (const std::string& fileName), (override));
    MOCK_METHOD(bool, measureRxLnaSubBandGains, (), (override));
    MOCK_METHOD(bool, readMemory, (ChipModule chipModule, size_t address, uint8_t* data, size_t length), (override));
    MOCK_METHOD(bool, readNvm, (size_t address, uint8_t* data, size_t length, bool useCache), (override));
    MOCK_METHOD(bool, readRegister, (ChipModule chipModule, size_t address, uint32_t mask, uint32_t& value), (override));
    MOCK_METHOD(bool, resetMacPacketCounters, (), (override));
    MOCK_METHOD(bool, resetMpduPacketCounters, (), (override));
    MOCK_METHOD(bool, resetPhyPacketCounters, (), (override));
    MOCK_METHOD(bool, rxMeasure, (uint32_t numCaptures, uint32_t captureInterval, bool disabled), (override));
    MOCK_METHOD(bool, saveNvmToFile, (const std::string& fileName), (override));
    MOCK_METHOD(bool, setBbicCddValues, (uint8_t numTxAntennas, uint32_t offset1, uint32_t offset2, uint32_t offset3), (override));
    MOCK_METHOD(bool, setCalibrationFileVersion, (CalibrationFileVersion version, CalibrationFileSubversion subversion), (override));
    MOCK_METHOD(bool, setCardInfo, (uint8_t countryCode, (const std::array<uint8_t, cardInfoMacAddressSize>&)macAddress, (const std::array<uint8_t, cardInfoSerialNumberSize>&)serialNumber, uint8_t week, uint8_t year), (override));
    MOCK_METHOD(bool, setChannel, (PhyMode phyMode, Bandwidth spectrumBandwidth, uint8_t lowestChannel, uint8_t primaryChannelIndex, RegulationType regulationType), (override));
    MOCK_METHOD(bool, setEnabledRxAntennaMask, (const AntennaMask& antennaMask), (override));
    MOCK_METHOD(bool, setEnabledTxAntennaMask, (const AntennaMask& antennaMask), (override));
    MOCK_METHOD(bool, setHdkConfig, (uint32_t offlineCalMask, uint32_t onlineCalMask), (override));
    MOCK_METHOD(bool, setIfs, (uint32_t ifs), (override));
    MOCK_METHOD(bool, setProductionFlag, (bool productionFlag, bool writeToNvm), (override));
    MOCK_METHOD(bool, setRate, (Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, Gi gi, Ltf ltf, float& rateMbps), (override));
    MOCK_METHOD(bool, setRssiCalibrationData, (const std::vector<RssiCalibrationData_t>& data), (override));
    MOCK_METHOD(bool, setRssiS2dInitials, (uint8_t auxAdcRes, float ioffsStep), (override));
    MOCK_METHOD(bool, setRssiS2dParams, (const AntennaMask& antennaMask, uint8_t region, uint8_t gain, uint8_t offset), (override));
    MOCK_METHOD(bool, setRuParams, (uint32_t userOne, uint32_t userTwo), (override));
    MOCK_METHOD(bool, setRxAggregationEnabled, (bool enabled), (override));
    MOCK_METHOD(bool, setRxBandLut, (const AntennaMask& antennaMask, (const std::array<std::array<uint16_t, maxNumLnaSubBandsCrossingPoints>, maxNumRxAntennas>&)rxBandLut, (const std::array<uint8_t, maxNumRxAntennas>&)firstTunningIndex), (override));
    MOCK_METHOD(bool, setRxGainBlocks, (uint8_t lnaIndex, int8_t pgc1, int8_t pgc2, int8_t pgc3), (override));
    MOCK_METHOD(bool, setSpacelessTxEnabled, (bool enabled), (override));
    MOCK_METHOD(bool, setTransmitPowerControl, (bool closedLoop, uint8_t powerLimit), (override));
    MOCK_METHOD(bool, setTransmitPowerLevel, (uint8_t powerLevel), (override));
    MOCK_METHOD(bool, setTransmitPowerTableOffset, (uint8_t antenna, Bandwidth bandwidth, int16_t offset), (override));
    MOCK_METHOD(bool, setTransmitPowerVector, (TransmitPowerVector_t & transmitPowerVector), (override));
    MOCK_METHOD(bool, setTssiCalibrationData, (const std::vector<TssiCalibrationData_t>& data), (override));
    MOCK_METHOD(bool, setTssiS2dParams, (uint8_t antenna, uint8_t region, uint8_t powerThreshold, uint8_t gain, uint8_t offset), (override));
    MOCK_METHOD(bool, setXtalCalValue, (uint16_t xtalValue), (override));
    MOCK_METHOD(bool, setXtalRegValue, (uint16_t xtalValue), (override));
    MOCK_METHOD(bool, setZwdfsConfiguration, (uint8_t zwdfsChannel, Bandwidth zwdfsBandwidth, Bandwidth radarDetectionBandwidth), (override));
    MOCK_METHOD(bool, startCalibration, (const StartCalibrationParams_t& params, uint8_t& status), (override));
    MOCK_METHOD(bool, startCw, (int8_t amplitude, int16_t tone), (override));
    MOCK_METHOD(bool, startRxCalibration, (), (override));
    MOCK_METHOD(bool, startTx, (uint16_t repetitions, uint32_t packetLength, bool longData, bool beamforming, CodingType codingType), (override));
    MOCK_METHOD(bool, startRxPer, (uint32_t packetLimit), (override));
    MOCK_METHOD(bool, stopCw, (), (override));
    MOCK_METHOD(bool, stopRxCalibration, (), (override));
    MOCK_METHOD(bool, stopTx, (), (override));
    MOCK_METHOD(bool, stopRxPer, (bool calcRxPer), (override));
    MOCK_METHOD(bool, setClipper, (bool enabled), (override));
    MOCK_METHOD(bool, validateBeamformingHeaderRegister, (PhyMode expectedPhyMode, Bandwidth expectedBandwidth), (override));
    MOCK_METHOD(bool, writeCalibrationFile, (NvMemoryType memoryType, NvMemorySize memorySize), (override));
    MOCK_METHOD(bool, writeMemory, (ChipModule chipModule, size_t address, const uint8_t* data, size_t length), (override));
    MOCK_METHOD(bool, writeNvm, (size_t address, const uint8_t* data, size_t length), (override));
    MOCK_METHOD(bool, writeRegister, (ChipModule chipModule, size_t address, uint32_t mask, uint32_t value), (override));
};

class DutMockFactory : public DutFactory {
public:
    // Google Mock cannot mock a factory method that returns a non copyable return value.
    // To work around this, we add an indirection through a proxy method.
    // Production code will use the overridden method and unit tests will set expectations in the
    // mocked helper method instead.
    std::unique_ptr<Dut> createInstance(uint8_t wlanIndex, std::shared_ptr<Connection> connection, std::shared_ptr<Logger> logger, bool throwExceptions) override
    {
        return std::unique_ptr<Dut>(createInstanceProxy(wlanIndex, connection, logger, throwExceptions));
    };
    MOCK_METHOD(Dut*, createInstanceProxy, (uint8_t wlanIndex, std::shared_ptr<Connection> connection, std::shared_ptr<Logger> logger, bool throwExceptions));
};

}

#endif
