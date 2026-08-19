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

#ifndef _DUT_CLIENT_IMPL_H_
#define _DUT_CLIENT_IMPL_H_

#include "dut/Client.h"

#include "Messenger.h"

typedef struct packetCountersParams packetCountersParams_t;

namespace dut {

class Message;
class Logger;

class DUT_API ClientImpl : public Client {
public:
    ClientImpl(uint8_t wlanIndex, std::shared_ptr<Connection> connection, std::shared_ptr<Logger> logger);

    uint8_t getWlanIndex() override
    {
        return m_wlanIndex;
    }

    void addVap(uint8_t vapIndex, bool snifferMode) override;
    void flushNvMemory(ChipID chipId, NvMemoryType memoryType, FileType fileType) override;
    void getBbicCddValues(uint8_t numTxAntennas, uint32_t& offset1, uint32_t& offset2, uint32_t& offset3) override;
    void getCombinedVersion(std::array<uint8_t, combinedVersionSize>& buffer) override;
    void getFemType(FemType& femType) override;
    void getFwInfo(struct dutGetFwInfo_tag& fwInfo) override;
    void getInbandRssi(const AntennaMask& antennaMask, std::array<int16_t, maxNumRxAntennas>& rssi, uint16_t& numSamples) override;
    void getMacPacketCounters(uint32_t& receivedPackets) override;
    void getMpduPacketCounters(uint32_t& receivedPackets, uint32_t& errorPackets) override;
    void getNmseValues(std::array<std::array<int32_t, dut::dpdTotalCalibrationPoints>, dut::maxNumTxAntennas>& values) override;
    void getPhyPacketCounters(uint32_t& receivedPackets, uint32_t& crcErrors, uint32_t& forwardedPackets) override;
    void getRfRssiPower(const AntennaMask& antennaMask, std::array<int8_t, maxNumRxAntennas>& power) override;
    void getRssi(uint8_t method, uint16_t numSamples, const AntennaMask& antennaMask, std::array<uint16_t, maxNumRxAntennas>& rssi) override;
    void getRxCwPower(uint8_t method, int32_t freqOffset, uint16_t numSamples, const AntennaMask& antennaMask, std::array<CorrelationResults_t, maxNumRxAntennas>& results) override;
    void getRxEvm(std::array<uint8_t, maxNumRxAntennas>& rxEvm) override;
    void getRxRateInfo(uint8_t& mcs, uint8_t& nss) override;
    void getTemperature(float& temperature) override;
    void getTransmitPowerTableOffset(uint8_t antenna, Bandwidth bandwidth, int16_t& offset) override;
    void getTransmitVoltages(const AntennaMask& antennaMask, std::array<uint32_t, maxNumTxAntennas>& voltages) override;
    void getXtalValue(uint16_t& xtalValue) override;
    void readChipId(ChipID& chipId) override;
    void readMemory(ChipModule chipModule, size_t address, uint8_t* data, size_t length) override;
    void readNvMemory(size_t address, uint8_t* data, size_t length, NvMemoryType memoryType, NvMemorySize memorySize, FileType fileType) override;
    void removeVap(uint8_t vapIndex) override;
    void resetMac(NvMemoryType nvType, NvMemorySize memorySize, bool reset) override;
    void resetMacPacketCounters() override;
    void resetMpduPacketCounters() override;
    void resetPhyPacketCounters() override;
    void rxMeasure(uint32_t numCaptures, uint32_t captureInterval, bool disabled, PhyMode phyMode) override;
    void setBand(Band band) override;
    void setBbicCddValues(uint8_t numTxAntennas, uint32_t offset1, uint32_t offset2, uint32_t offset3) override;
    void setChannel(uint8_t lowestChannel, uint8_t primaryChannelIndex, Bandwidth spectrumBandwidth, const AntennaMask& txAntennaMask, RegulationType regulationType) override;
    void setClipper(bool enabled) override;
    void setEnabledRxAntennas(const AntennaMask& antennaMask) override;
    void setEnabledTxAntennas(const AntennaMask& antennaMask) override;
    void setHdkConfig(uint32_t offlineCalMask, uint32_t onlineCalMask, Band band) override;
    void setIfs(uint32_t ifs) override;
    void setLnaMidGains(const AntennaMask& antennaMask, const std::array<uint8_t, maxNumRxAntennas>& midGains) override;
    void setRate(PhyMode phyMode, Bandwidth signalBandwidth, uint8_t mcsIndex, uint8_t nss, Gi gi, Ltf ltf) override;
    void setRiscModeEnabled(bool enabled) override;
    void setRssiCalData(const uint8_t* data, size_t length) override;
    void setRssiS2dParams(const AntennaMask& antennaMask, uint8_t region, uint8_t gain, uint8_t offset) override;
    void setRuParams(uint32_t userOne, uint32_t userTwo) override;
    void setRxAggregationEnabled(bool enabled) override;
    void setRxBand(uint8_t rxBand) override;
    void setRxBandLut(const AntennaMask& antennaMask, const std::array<std::array<uint16_t, maxNumLnaSubBandsCrossingPoints>, maxNumRxAntennas>& rxBandLut, const std::array<uint8_t, maxNumRxAntennas>& firstTunningIndex) override;
    void setRxGainBlocks(const AntennaMask& antennaMask, uint8_t lnaIndex, int8_t pgc1, int8_t pgc2, int8_t pgc3) override;
    void setSpacelessTxEnabled(bool enabled) override;
    void setS2dParams(uint8_t antenna, Bandwidth signalBandwidth, uint8_t region, uint8_t powerThreshold, uint8_t gain, uint8_t offset) override;
    void setTransmitPowerControlAntennaParams(uint8_t antenna, uint8_t tpcFreqLen, const uint8_t* data, size_t length) override;
    void setTransmitPowerControl(CalibrationType calibrationType, bool closedLoop, uint8_t powerLimit) override;
    void setTransmitPowerLevel(uint8_t antenna, Bandwidth bandwidth, uint8_t powerLevel) override;
    void setTransmitPowerTableOffset(uint8_t antenna, Bandwidth bandwidth, int16_t offset) override;
    void setTransmitPowerVector(const AntennaMask& antennaMask, TransmitPowerVector_t& transmitPowerVector) override;
    void setXtalValue(uint16_t xtalValue) override;
    void setZwdfsEnabled(bool zwdfsEnabled, uint8_t zwdfsChannel, Bandwidth zwdfsBandwidth, Bandwidth radarDetectionBandwidth, RegulationType regulationType) override;
    void startCalibration(const StartCalibrationParams_t& params, uint8_t& status) override;
    void startCw(int8_t amplitude, int16_t tone) override;
    void stopCw() override;
    void startRxCalibration() override;
    void stopRxCalibration() override;
    void startTx(uint16_t repetitions, uint32_t packetLength, bool longData, bool beamforming, bool ldpc) override;
    void startRxPer(uint32_t packetLimit, PhyMode phyMode) override;
    void stopTx() override;
    void stopRxPer(PhyMode phyMode, uint32_t& totalPacketsReceived, uint32_t& packetCounter) override;
    void writeMemory(ChipModule chipModule, size_t address, const uint8_t* data, size_t length) override;
    void writeNvMemory(size_t address, const uint8_t* data, size_t length, NvMemoryType memoryType, NvMemorySize memorySize, FileType fileType) override;

protected:
    void exchangeMessage(uint8_t wlanIndex, uint8_t msgId, uint32_t requestId, uint32_t responseId, Message& message, const std::chrono::milliseconds& receiveTimeout) const;

private:
#pragma warning(disable : 4251) // class needs to have dll-interface to be used by clients of class 'dut::ClientImpl'
    uint8_t m_wlanIndex;
    Messenger m_messenger;
    std::shared_ptr<Logger> m_logger;
#pragma warning(default : 4251)

    void getTransmitPowerVectorForAntenna(uint8_t antenna, TransmitPowerVector_t& transmitPowerVector) const;
    void exchangeMessage(packetCountersParams_t& packetCountersParams);
};
}

#endif
