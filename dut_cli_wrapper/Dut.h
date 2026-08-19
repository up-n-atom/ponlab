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

#pragma once

#include "AntennaMask.h"
#include "Connection.h"
#include "Logger.h"
#include "ManagedObject.h"
#include "RssiCalibrationData.h"
#include "TssiCalibrationData.h"
#include "Types.h"
#include "dut/Dut.h"

namespace dut_cli {

public
ref class Dut : public ManagedObject<dut::Dut> {
public:
    Dut(uint8_t wlanIndex, Connection ^ connection, Logger ^ logger);

    static System::String ^ getVersion();
    static System::String ^ getBuildTag();

    System::String ^ getLastError();

    bool driverInit(bool snifferMode, NvMemoryType memoryType, NvMemorySize memorySize, Band band);
    bool driverRelease();
    bool calculateMaxPacketLength(PhyMode phyMode, [System::Runtime::InteropServices::OutAttribute] uint32_t % maxPacketLength);
    bool calculatePacketLength(PhyMode phyMode, Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, uint32_t numSymbols, [System::Runtime::InteropServices::OutAttribute] uint32_t % packetLength);
    bool calculatePhyDataRate(PhyMode phyMode, Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, Gi gi, [System::Runtime::InteropServices::OutAttribute] float % rateMbps);
    bool flushNvm();
    bool getAvailableRxAntennaMask([System::Runtime::InteropServices::OutAttribute] AntennaMask ^ % antennaMask);
    bool getAvailableTxAntennaMask([System::Runtime::InteropServices::OutAttribute] AntennaMask ^ % antennaMask);
    bool getBand([System::Runtime::InteropServices::OutAttribute] Band % band);
    bool getSupportedBands([System::Runtime::InteropServices::OutAttribute] cli::array<Band> ^ % supportedBands);
    bool getBbicCddValues(uint8_t numTxAntennas, [System::Runtime::InteropServices::OutAttribute] uint32_t % offset1, [System::Runtime::InteropServices::OutAttribute] uint32_t % offset2, [System::Runtime::InteropServices::OutAttribute] uint32_t % offset3);
    bool getCalibrationFileVersion([System::Runtime::InteropServices::OutAttribute] CalibrationFileVersion % version, [System::Runtime::InteropServices::OutAttribute] CalibrationFileSubversion % subversion);
    bool getCardInfo([System::Runtime::InteropServices::OutAttribute] uint8_t % countryCode, [System::Runtime::InteropServices::OutAttribute] cli::array<uint8_t> ^ % macAddress, [System::Runtime::InteropServices::OutAttribute] cli::array<uint8_t> ^ % serialNumber, [System::Runtime::InteropServices::OutAttribute] uint8_t % week, [System::Runtime::InteropServices::OutAttribute] uint8_t % year);
    bool getChipId([System::Runtime::InteropServices::OutAttribute] uint16_t % chipId);
    bool getComponentVersion(VersionedComponent component, [System::Runtime::InteropServices::OutAttribute] System::String ^ % version);
    bool getEnabledRxAntennaMask([System::Runtime::InteropServices::OutAttribute] AntennaMask ^ % antennaMask);
    bool getEnabledTxAntennaMask([System::Runtime::InteropServices::OutAttribute] AntennaMask ^ % antennaMask);
    bool getFemType([System::Runtime::InteropServices::OutAttribute] FemType % femType);
    bool getHardwareType([System::Runtime::InteropServices::OutAttribute] HardwareType % hardwareType);
    bool getInbandRssi([System::Runtime::InteropServices::OutAttribute] cli::array<int16_t> ^ % rssi);
    bool getMacPacketCounters([System::Runtime::InteropServices::OutAttribute] uint32_t % receivedPackets);
    bool getMpduPacketCounters([System::Runtime::InteropServices::OutAttribute] uint32_t % receivedPackets, [System::Runtime::InteropServices::OutAttribute] uint32_t % errorPackets);
    bool getNvmSize([System::Runtime::InteropServices::OutAttribute] size_t % size);
    bool getNvmType([System::Runtime::InteropServices::OutAttribute] NvMemoryType % type);
    bool getNvmVersion([System::Runtime::InteropServices::OutAttribute] uint8_t % version);
    bool getPhyMode([System::Runtime::InteropServices::OutAttribute] PhyMode % phyMode);
    bool getPhyPacketCounters([System::Runtime::InteropServices::OutAttribute] uint32_t % receivedPackets, [System::Runtime::InteropServices::OutAttribute] uint32_t % crcErrors, [System::Runtime::InteropServices::OutAttribute] uint32_t % forwardedPackets);
    bool getProductionFlag([System::Runtime::InteropServices::OutAttribute] bool % productionFlag);
    bool getRssiCalibrationData([System::Runtime::InteropServices::OutAttribute] CalibrationFileVersion % version, [System::Runtime::InteropServices::OutAttribute] cli::array<RssiCalibrationData ^> ^ % data);
    bool getRxEvm([System::Runtime::InteropServices::OutAttribute] cli::array<uint8_t> ^ % rxEvm);
    bool getRxRateInfo([System::Runtime::InteropServices::OutAttribute] uint8_t % mcs, uint8_t % nss);
    bool getTemperature([System::Runtime::InteropServices::OutAttribute] float % temperature);
    bool getTransmitVoltages([System::Runtime::InteropServices::OutAttribute] cli::array<uint32_t> ^ % voltages);
    bool getTssiCalibrationData([System::Runtime::InteropServices::OutAttribute] CalibrationFileVersion % version, [System::Runtime::InteropServices::OutAttribute] cli::array<TssiCalibrationData ^> ^ % data);
    bool getXtalCalValue([System::Runtime::InteropServices::OutAttribute] uint16_t % xtalValue);
    bool getXtalRegValue([System::Runtime::InteropServices::OutAttribute] uint16_t % xtalValue);
    bool getZwdfsStatus([System::Runtime::InteropServices::OutAttribute] AntennaMask ^ % antennaMask, [System::Runtime::InteropServices::OutAttribute] bool % enabled);
    bool loadBeamformingMatrixFromFileSet(System::String ^ primaryHeaderFile, System::String ^ primaryValuesFile, System::String ^ primaryExtValuesEhtFile, System::String ^ secondaryHeaderFile, System::String ^ secondaryValuesFile, System::String ^ secondaryExtValuesEhtFile);
    bool loadNvmFromFile(System::String ^ fileName);
    bool readMemory(ChipModule chipModule, size_t address, [System::Runtime::InteropServices::OutAttribute] cli::array<uint8_t> ^ % data, size_t length);
    bool readNvm(size_t address, [System::Runtime::InteropServices::OutAttribute] cli::array<uint8_t> ^ % data, size_t length, bool useCache);
    bool readRegister(ChipModule chipModule, size_t address, uint32_t mask, [System::Runtime::InteropServices::OutAttribute] uint32_t % value);
    bool resetMacPacketCounters();
    bool resetMpduPacketCounters();
    bool resetPhyPacketCounters();
    bool rxMeasure(uint32_t numCaptures, uint32_t captureInterval, bool disabled);
    bool saveNvmToFile(System::String ^ fileName);
    bool setBbicCddValues(uint8_t numTxAntennas, uint32_t offset1, uint32_t offset2, uint32_t offset3);
    bool setCalibrationFileVersion(CalibrationFileVersion version, CalibrationFileSubversion subversion);
    bool setCardInfo(uint8_t countryCode, cli::array<uint8_t> ^ macAddress, cli::array<uint8_t> ^ serialNumber, uint8_t week, uint8_t year);
    bool setChannel(PhyMode phyMode, Bandwidth spectrumBandwidth, uint8_t lowestChannel, uint8_t primaryChannelIndex, RegulationType regulationType);
    bool setClipper(bool enabled);
    bool setEnabledRxAntennaMask(AntennaMask ^ antennaMask);
    bool setEnabledTxAntennaMask(AntennaMask ^ antennaMask);
    bool setIfs(uint32_t ifs);
    bool setProductionFlag(bool productionFlag, bool writeToNvm);
    bool setRate(Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, Gi gi, Ltf ltf, [System::Runtime::InteropServices::OutAttribute] float % rateMbps);
    bool setRuParams(uint32_t userOne, uint32_t userTwo);
    bool setRxAggregationEnabled(bool enabled);
    bool setSpacelessTxEnabled(bool enabled);
    bool setTransmitPowerControl(bool closedLoop, uint8_t powerLimit);
    bool setTransmitPowerLevel(uint8_t powerLevel);
    bool setXtalCalValue(uint16_t xtalValue);
    bool setXtalRegValue(uint16_t xtalValue);
    bool setZwdfsConfiguration(uint8_t zwdfsChannel, Bandwidth zwdfsBandwidth, Bandwidth radarDetectionBandwidth);
    bool startCw(int8_t amplitude, int16_t tone);
    bool startTx(uint16_t repetitions, uint32_t packetLength, bool longData, bool beamforming, CodingType codingType);
    bool startRxPer(uint32_t packetLimit);
    bool stopCw();
    bool stopTx();
    bool stopRxPer(bool calcRxPer);
    bool validateBeamformingHeaderRegister(PhyMode expectedPhyMode, Bandwidth expectedBandwidth);
    bool writeCalibrationFile(NvMemoryType memoryType, NvMemorySize memorySize);
    bool writeMemory(ChipModule chipModule, size_t address, cli::array<uint8_t> ^ data, size_t length);
    bool writeNvm(size_t address, cli::array<uint8_t> ^ data, size_t length);
    bool writeRegister(ChipModule chipModule, size_t address, uint32_t mask, uint32_t value);
};
}
