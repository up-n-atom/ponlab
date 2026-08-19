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

#ifndef _DUT_CLIENT_MOCK_H_
#define _DUT_CLIENT_MOCK_H_

#include "dut/Client.h"

#include <gmock/gmock.h>

namespace dut {

class ClientMock : public Client {
public:
    MOCK_METHOD(uint8_t, getWlanIndex, (), (override));
    MOCK_METHOD(void, addVap, (uint8_t vapIndex, bool snifferMode), (override));
    MOCK_METHOD(void, flushNvMemory, (ChipID chipId, NvMemoryType memoryType, FileType fileType), (override));
    MOCK_METHOD(void, getBbicCddValues, (uint8_t numTxAntennas, uint32_t& offset1, uint32_t& offset2, uint32_t& offset3), (override));
    MOCK_METHOD(void, getCombinedVersion, ((std::array<uint8_t, combinedVersionSize> & buffer)), (override));
    MOCK_METHOD(void, getFemType, (FemType & femType), (override));
    MOCK_METHOD(void, getFwInfo, (struct dutGetFwInfo_tag & fwInfo), (override));
    MOCK_METHOD(void, getInbandRssi, (const AntennaMask& antennaMask, (std::array<int16_t, maxNumRxAntennas> & rssi), uint16_t& numSamples), (override));
    MOCK_METHOD(void, getMacPacketCounters, (uint32_t & receivedPackets), (override));
    MOCK_METHOD(void, getMpduPacketCounters, (uint32_t & receivedPackets, uint32_t& errorPackets), (override));
    MOCK_METHOD(void, getNmseValues, ((std::array<std::array<int32_t, dpdTotalCalibrationPoints>, maxNumTxAntennas> & values)), (override));
    MOCK_METHOD(void, getPhyPacketCounters, (uint32_t & receivedPackets, uint32_t& crcErrors, uint32_t& forwardedPackets), (override));
    MOCK_METHOD(void, getRfRssiPower, (const AntennaMask& antennaMask, (std::array<int8_t, maxNumRxAntennas> & power)), (override));
    MOCK_METHOD(void, getRssi, (uint8_t method, uint16_t numSamples, const AntennaMask& antennaMask, (std::array<uint16_t, maxNumRxAntennas> & rssi)), (override));
    MOCK_METHOD(void, getRxCwPower, (uint8_t method, int32_t freqOffset, uint16_t numSamples, const AntennaMask& antennaMask, (std::array<CorrelationResults_t, maxNumRxAntennas> & results)), (override));
    MOCK_METHOD(void, getRxEvm, ((std::array<uint8_t, maxNumRxAntennas> & rxEvm)), (override));
    MOCK_METHOD(void, getRxRateInfo, (uint8_t & mcs, uint8_t& nss), (override));
    MOCK_METHOD(void, getTemperature, (float& temperature), (override));
    MOCK_METHOD(void, getTransmitPowerTableOffset, (uint8_t antenna, Bandwidth bandwidth, int16_t& offset), (override));
    MOCK_METHOD(void, getTransmitVoltages, (const AntennaMask& antennaMask, (std::array<uint32_t, maxNumTxAntennas> & voltages)), (override));
    MOCK_METHOD(void, getXtalValue, (uint16_t & xtalValue), (override));
    MOCK_METHOD(void, readChipId, (ChipID & chipId), (override));
    MOCK_METHOD(void, readMemory, (ChipModule chipModule, size_t address, uint8_t* data, size_t length), (override));
    MOCK_METHOD(void, readNvMemory, (size_t address, uint8_t* data, size_t length, NvMemoryType memoryType, NvMemorySize memorySize, FileType fileType), (override));
    MOCK_METHOD(void, removeVap, (uint8_t vapIndex), (override));
    MOCK_METHOD(void, resetMac, (NvMemoryType nvType, NvMemorySize memorySize, bool reset), (override));
    MOCK_METHOD(void, resetMacPacketCounters, (), (override));
    MOCK_METHOD(void, resetMpduPacketCounters, (), (override));
    MOCK_METHOD(void, resetPhyPacketCounters, (), (override));
    MOCK_METHOD(void, rxMeasure, (uint32_t numCaptures, uint32_t captureInterval, bool disabled, PhyMode phyMode), (override));
    MOCK_METHOD(void, setBand, (Band band), (override));
    MOCK_METHOD(void, setBbicCddValues, (uint8_t numTxAntennas, uint32_t offset1, uint32_t offset2, uint32_t offset3), (override));
    MOCK_METHOD(void, setChannel, (uint8_t lowestChannel, uint8_t primaryChannelIndex, Bandwidth spectrumBandwidth, const AntennaMask& txAntennaMask, RegulationType regulationType), (override));
    MOCK_METHOD(void, setEnabledRxAntennas, (const AntennaMask& antennaMask), (override));
    MOCK_METHOD(void, setEnabledTxAntennas, (const AntennaMask& antennaMask), (override));
    MOCK_METHOD(void, setHdkConfig, (uint32_t offlineCalMask, uint32_t onlineCalMask, Band band), (override));
    MOCK_METHOD(void, setIfs, (uint32_t ifs), (override));
    MOCK_METHOD(void, setLnaMidGains, (const AntennaMask& antennaMask, (const std::array<uint8_t, maxNumRxAntennas>& midGains)), (override));
    MOCK_METHOD(void, setRate, (PhyMode phyMode, Bandwidth signalBandwidth, uint8_t mcs, uint8_t nss, Gi gi, Ltf ltf), (override));
    MOCK_METHOD(void, setRiscModeEnabled, (bool enabled), (override));
    MOCK_METHOD(void, setRssiCalData, (const uint8_t* data, size_t length), (override));
    MOCK_METHOD(void, setRssiS2dParams, (const AntennaMask& antennaMask, uint8_t region, uint8_t gain, uint8_t offset), (override));
    MOCK_METHOD(void, setRuParams, (uint32_t userOne, uint32_t userTwo), (override));
    MOCK_METHOD(void, setRxAggregationEnabled, (bool enabled), (override));
    MOCK_METHOD(void, setRxBand, (uint8_t rxBand), (override));
    MOCK_METHOD(void, setRxBandLut, (const AntennaMask& antennaMask, (const std::array<std::array<uint16_t, maxNumLnaSubBandsCrossingPoints>, maxNumRxAntennas>& rxBandLut), (const std::array<uint8_t, maxNumRxAntennas>& firstTunningIndex)), (override));
    MOCK_METHOD(void, setRxGainBlocks, (const AntennaMask& antennaMask, uint8_t lnaIndex, int8_t pgc1, int8_t pgc2, int8_t pgc3), (override));
    MOCK_METHOD(void, setSpacelessTxEnabled, (bool enabled), (override));
    MOCK_METHOD(void, setS2dParams, (uint8_t antenna, Bandwidth signalBandwidth, uint8_t region, uint8_t powerThreshold, uint8_t gain, uint8_t offset), (override));
    MOCK_METHOD(void, setTransmitPowerControlAntennaParams, (uint8_t antenna, uint8_t tpcFreqLen, const uint8_t* data, size_t length), (override));
    MOCK_METHOD(void, setTransmitPowerControl, (CalibrationType calibrationType, bool closedLoop, uint8_t powerLimit), (override));
    MOCK_METHOD(void, setTransmitPowerLevel, (uint8_t antenna, Bandwidth bandwidth, uint8_t powerLevel), (override));
    MOCK_METHOD(void, setTransmitPowerTableOffset, (uint8_t antenna, Bandwidth bandwidth, int16_t offset), (override));
    MOCK_METHOD(void, setTransmitPowerVector, (const AntennaMask& antennaMask, TransmitPowerVector_t& transmitPowerVector), (override));
    MOCK_METHOD(void, setXtalValue, (uint16_t xtalValue), (override));
    MOCK_METHOD(void, setZwdfsEnabled, (bool zwdfsEnabled, uint8_t zwdfsChannel, Bandwidth zwdfsBandwidth, Bandwidth radarDetectionBandwidth, RegulationType regulationType), (override));
    MOCK_METHOD(void, startCalibration, (const StartCalibrationParams_t& params, uint8_t& status), (override));
    MOCK_METHOD(void, startCw, (int8_t amplitude, int16_t tone), (override));
    MOCK_METHOD(void, stopCw, (), (override));
    MOCK_METHOD(void, startRxCalibration, (), (override));
    MOCK_METHOD(void, stopRxCalibration, (), (override));
    MOCK_METHOD(void, startTx, (uint16_t repetitions, uint32_t packetLength, bool longData, bool beamforming, bool ldpc), (override));
    MOCK_METHOD(void, stopTx, (), (override));
    MOCK_METHOD(void, startRxPer, (uint32_t packetLimit, PhyMode phyMode), (override));
    MOCK_METHOD(void, stopRxPer, (PhyMode phyMode, uint32_t& totalPacketsReceived, uint32_t& packetCounter), (override));
    MOCK_METHOD(void, setClipper, (bool enabled), (override));
    MOCK_METHOD(void, writeMemory, (ChipModule chipModule, size_t address, const uint8_t* data, size_t length), (override));
    MOCK_METHOD(void, writeNvMemory, (size_t address, const uint8_t* data, size_t length, NvMemoryType memoryType, NvMemorySize memorySize, FileType fileType), (override));
};

}

#endif
