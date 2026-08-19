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

#ifndef _DUT_CLIENT_H_
#define _DUT_CLIENT_H_

#include "dut/API.h"

#include "dut/Types.h"

#include <array>

struct dutGetFwInfo_tag;

namespace dut {

class AntennaMask;

enum class CalibrationType {
    TSSI_CAL_TYPE_VER4 = 0x00,
    TSSI_CAL_TYPE_VER5 = 0x01,
    TSSI_CAL_TYPE_VER6 = 0x02
};

class DUT_API Client {
public:
    // sizeof(dutMessage_t.data)
    static constexpr size_t combinedVersionSize = 504; // DUT_MSG_DATA_LENGTH

    virtual ~Client() = default;

    virtual uint8_t getWlanIndex() = 0;

    // Wireless Local Area Network (WLAN)
    virtual void addVap(uint8_t vapIndex, bool snifferMode) = 0;
    virtual void getBbicCddValues(uint8_t numTxAntennas, uint32_t& offset1, uint32_t& offset2, uint32_t& offset3) = 0;
    virtual void getFwInfo(struct dutGetFwInfo_tag& fwInfo) = 0;
    virtual void readChipId(ChipID& chipId) = 0;
    virtual void removeVap(uint8_t vapIndex) = 0;
    virtual void setBand(Band band) = 0;
    virtual void setBbicCddValues(uint8_t numTxAntennas, uint32_t offset1, uint32_t offset2, uint32_t offset3) = 0;
    virtual void setChannel(uint8_t lowestChannel, uint8_t primaryChannelIndex, Bandwidth spectrumBandwidth, const AntennaMask& txAntennaMask, RegulationType regulationType) = 0;
    virtual void setEnabledRxAntennas(const AntennaMask& antennaMask) = 0;
    virtual void setEnabledTxAntennas(const AntennaMask& antennaMask) = 0;
    virtual void setHdkConfig(uint32_t offlineCalMask, uint32_t onlineCalMask, Band band) = 0;
    virtual void setIfs(uint32_t ifs) = 0;
    virtual void setZwdfsEnabled(bool zwdfsEnabled, uint8_t zwdfsChannel, Bandwidth zwdfsBandwidth, Bandwidth radarDetectionBandwidth, RegulationType regulationType) = 0;

    // WLAN - RX
    virtual void getMacPacketCounters(uint32_t& receivedPackets) = 0;
    virtual void getMpduPacketCounters(uint32_t& receivedPackets, uint32_t& errorPackets) = 0;
    virtual void getPhyPacketCounters(uint32_t& receivedPackets, uint32_t& crcErrors, uint32_t& forwardedPackets) = 0;
    virtual void resetMacPacketCounters() = 0;
    virtual void resetMpduPacketCounters() = 0;
    virtual void resetPhyPacketCounters() = 0;
    virtual void setRxAggregationEnabled(bool enabled) = 0;

    // WLAN - TX
    virtual void setRate(PhyMode phyMode, Bandwidth signalBandwidth, uint8_t mcsIndex, uint8_t nss, Gi gi, Ltf ltf) = 0;
    virtual void setRuParams(uint32_t userOne, uint32_t userTwo) = 0;
    virtual void setSpacelessTxEnabled(bool enabled) = 0;
    virtual void startCw(int8_t amplitude, int16_t tone) = 0;
    virtual void stopCw() = 0;
    virtual void startTx(uint16_t repetitions, uint32_t packetLength, bool longData, bool beamforming, bool ldpc) = 0;
    virtual void stopTx() = 0;

    // Non Volatile Memory (NVM)
    virtual void flushNvMemory(ChipID chipId, NvMemoryType memoryType, FileType fileType) = 0;
    virtual void readNvMemory(size_t address, uint8_t* data, size_t length, NvMemoryType memoryType, NvMemorySize memorySize, FileType fileType) = 0;
    virtual void writeNvMemory(size_t address, const uint8_t* data, size_t length, NvMemoryType memoryType, NvMemorySize memorySize, FileType fileType) = 0;

    // Hardware access
    virtual void readMemory(ChipModule chipModule, size_t address, uint8_t* data, size_t length) = 0;
    virtual void writeMemory(ChipModule chipModule, size_t address, const uint8_t* data, size_t length) = 0;

    // Platform access
    virtual void getCombinedVersion(std::array<uint8_t, combinedVersionSize>& buffer) = 0;

    // Calibration - XTAL
    virtual void getXtalValue(uint16_t& xtalValue) = 0;
    virtual void setXtalValue(uint16_t xtalValue) = 0;

    // Calibration - TSSI
    virtual void getTransmitPowerTableOffset(uint8_t antenna, Bandwidth bandwidth, int16_t& offset) = 0;
    virtual void getTransmitVoltages(const AntennaMask& antennaMask, std::array<uint32_t, maxNumTxAntennas>& voltages) = 0;
    virtual void setS2dParams(uint8_t antenna, Bandwidth signalBandwidth, uint8_t region, uint8_t powerThreshold, uint8_t gain, uint8_t offset) = 0;
    virtual void setTransmitPowerControlAntennaParams(uint8_t antenna, uint8_t tpcFreqLen, const uint8_t* data, size_t length) = 0;
    virtual void setTransmitPowerControl(CalibrationType calibrationType, bool closedLoop, uint8_t powerLimit) = 0;
    virtual void setTransmitPowerLevel(uint8_t antenna, Bandwidth bandwidth, uint8_t powerLevel) = 0;
    virtual void setTransmitPowerTableOffset(uint8_t antenna, Bandwidth bandwidth, int16_t offset) = 0;
    virtual void setTransmitPowerVector(const AntennaMask& antennaMask, TransmitPowerVector_t& transmitPowerVector) = 0;

    // Calibration - RSSI
    virtual void getInbandRssi(const AntennaMask& antennaMask, std::array<int16_t, maxNumRxAntennas>& rssi, uint16_t& numSamples) = 0;
    virtual void getRfRssiPower(const AntennaMask& antennaMask, std::array<int8_t, maxNumRxAntennas>& power) = 0;
    virtual void getRssi(uint8_t method, uint16_t numSamples, const AntennaMask& antennaMask, std::array<uint16_t, maxNumRxAntennas>& rssi) = 0;
    virtual void getRxCwPower(uint8_t method, int32_t freqOffset, uint16_t numSamples, const AntennaMask& antennaMask, std::array<CorrelationResults_t, maxNumRxAntennas>& results) = 0;
    virtual void getRxEvm(std::array<uint8_t, maxNumRxAntennas>& rxEvm) = 0;
    virtual void setLnaMidGains(const AntennaMask& antennaMask, const std::array<uint8_t, maxNumRxAntennas>& midGains) = 0;
    virtual void setRiscModeEnabled(bool enabled) = 0;
    virtual void setRssiCalData(const uint8_t* data, size_t length) = 0;
    virtual void setRssiS2dParams(const AntennaMask& antennaMask, uint8_t region, uint8_t gain, uint8_t offset) = 0;
    virtual void setRxBand(uint8_t rxBand) = 0;
    virtual void setRxBandLut(const AntennaMask& antennaMask, const std::array<std::array<uint16_t, maxNumLnaSubBandsCrossingPoints>, maxNumRxAntennas>& rxBandLut, const std::array<uint8_t, maxNumRxAntennas>& firstTunningIndex) = 0;
    virtual void setRxGainBlocks(const AntennaMask& antennaMask, uint8_t lnaIndex, int8_t pgc1, int8_t pgc2, int8_t pgc3) = 0;
    virtual void startRxCalibration() = 0;
    virtual void stopRxCalibration() = 0;

    // Other
    virtual void getFemType(FemType& femType) = 0;
    virtual void getNmseValues(std::array<std::array<int32_t, dut::dpdTotalCalibrationPoints>, dut::maxNumTxAntennas>& values) = 0;
    virtual void getRxRateInfo(uint8_t& mcs, uint8_t& nss) = 0;
    virtual void getTemperature(float& temperature) = 0;
    virtual void resetMac(NvMemoryType memoryType, NvMemorySize memorySize, bool reset) = 0;
    virtual void rxMeasure(uint32_t numCaptures, uint32_t captureInterval, bool disabled, PhyMode phyMode) = 0;
    virtual void setClipper(bool enabled) = 0;
    virtual void startCalibration(const StartCalibrationParams_t& params, uint8_t& status) = 0;
    virtual void startRxPer(uint32_t packetLimit, PhyMode phyMode) = 0;
    virtual void stopRxPer(PhyMode phyMode, uint32_t& totalPacketsReceived, uint32_t& packetCounter) = 0;
};
}

#endif
