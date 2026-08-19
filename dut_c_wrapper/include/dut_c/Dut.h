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

#ifndef _DUT_C_DUT_H_
#define _DUT_C_DUT_H_

#include "dut_c/C_API.h"

#include "dut_c/Connection.h"
#include "dut_c/Logger.h"

#include <stdint.h>

#include "dut/Dut.h"
#ifdef __cplusplus
using DutHandle = dut::Dut*;
#else
typedef dut::Dut* DutHandle;
#endif

#ifdef __cplusplus
extern "C" {
#endif

DUT_C_API DutHandle DUT_C_API_ENTRY DUT_Create(uint8_t wlanIndex, ConnectionHandle connection, LoggerHandle logger);
DUT_C_API void DUT_C_API_ENTRY DUT_Destroy(DutHandle* handle);

DUT_C_API const char* DUT_C_API_ENTRY DUT_getVersion();
DUT_C_API const char* DUT_C_API_ENTRY DUT_getBuildTag();

DUT_C_API bool DUT_C_API_ENTRY DUT_getLastError(const DutHandle handle, char* buffer, size_t* size);

DUT_C_API bool DUT_C_API_ENTRY DUT_calibrateTssiS2d(const DutHandle handle, uint8_t lowerPowerLimit, uint8_t upperPowerLimit, uint32_t tssiLow, uint32_t tssiHigh, uint8_t region, uint8_t powerThreshold, uint8_t* gain, uint8_t* offset);
DUT_C_API bool DUT_C_API_ENTRY DUT_calculateLinearRegression(const DutHandle handle, const float powerVector[dut::numPowerPoints], const uint16_t voltages[dut::maxNumRxAntennas][dut::numPowerPoints], const uint16_t offsets[dut::maxNumRxAntennas], const float lnaGains[dut::maxNumRxAntennas], dut::RssiAB_t result[dut::maxNumRxAntennas], float maxErrors[dut::maxNumRxAntennas]);
DUT_C_API bool DUT_C_API_ENTRY DUT_calculateOptimalS2dGains(const DutHandle handle, const uint16_t rssiVoltage1[dut::maxNumRxAntennas], const uint16_t rssiVoltage2[dut::maxNumRxAntennas], int32_t regionS2dGain, int32_t optimalS2dGains[dut::maxNumRxAntennas]);
DUT_C_API bool DUT_C_API_ENTRY DUT_calculateOptimalS2dOffsets(const DutHandle handle, const uint16_t rssiVoltage1[dut::maxNumRxAntennas], const uint16_t rssiVoltage2[dut::maxNumRxAntennas], int32_t regionS2dGain, uint8_t regionS2dOffset, uint8_t optimalS2dOffsets[dut::maxNumRxAntennas]);
DUT_C_API bool DUT_C_API_ENTRY DUT_calculateMaxPacketLength(const DutHandle handle, dut::PhyMode phyMode, uint32_t* maxPacketLength);
DUT_C_API bool DUT_C_API_ENTRY DUT_calculatePacketLength(const DutHandle handle, dut::PhyMode phyMode, dut::Bandwidth signalBandwidth, dut::Mcs mcs, uint8_t nss, uint32_t numSymbols, uint32_t* packetLength);
DUT_C_API bool DUT_C_API_ENTRY DUT_calculatePhyDataRate(const DutHandle handle, dut::PhyMode phyMode, dut::Bandwidth signalBandwidth, dut::Mcs mcs, uint8_t nss, dut::Gi gi, float* rateMbps);
DUT_C_API bool DUT_C_API_ENTRY DUT_calibrateRxLnaMidGains(const DutHandle handle, const float targetGains[dut::maxNumRxAntennas], const float prxin[dut::maxNumRxAntennas], uint16_t numSamples, int8_t pgc1, int8_t pgc2, float calcGain[dut::maxNumRxAntennas], uint8_t calcGainCtrl[dut::maxNumRxAntennas]);
DUT_C_API bool DUT_C_API_ENTRY DUT_calculateRxLnaGains(const DutHandle handle, const dut::CorrelationResults_t correlationResults[dut::maxNumRxAntennas], const float prxin[dut::maxNumRxAntennas], int8_t pgc1, int8_t pgc2, int8_t pgc3, float gains[dut::maxNumRxAntennas]);
DUT_C_API bool DUT_C_API_ENTRY DUT_calculateRxLnaSubBandGains(const DutHandle handle, uint16_t freqResultTable[dut::maxNumRxAntennas][dut::maxNumLnaSubBands]);
DUT_C_API bool DUT_C_API_ENTRY DUT_calculateRxPowerVector(const DutHandle handle, int32_t maxPower, const float lnaGains[dut::maxNumRxAntennas], int32_t lowerPowerBoundary, int32_t upperPowerBoundary, float powerVector[dut::numPowerPoints]);
DUT_C_API bool DUT_C_API_ENTRY DUT_calculateRxRfFlatness(const DutHandle handle, const int32_t P_Adc_array[dut::maxNumRxAntennas][dut::maxNumLnaSubBands], const float refGain[dut::maxNumRxAntennas], int8_t pgc1, int8_t pgc2, int8_t pgc3, const float pin[dut::maxNumRxAntennas][dut::maxNumLnaSubBands], float results[dut::maxNumRxAntennas][dut::maxNumLnaSubBands]);
DUT_C_API bool DUT_C_API_ENTRY DUT_calculateTxABCoefficients(const DutHandle handle, dut::AntennaMask_t antennaMask, uint8_t numRegions, const dut::Point_t* pointsPerAntennaAndRegion[dut::maxNumTxAntennas][dut::maxNumTxRegions], const size_t numPointsPerAntennaAndRegion[dut::maxNumTxAntennas][dut::maxNumTxRegions], float nLog, dut::TssiAB_t coefficientsPerAntennaAndRegion[dut::maxNumTxAntennas][dut::maxNumTxRegions], float errorsPerAntennaAndRegion[dut::maxNumTxAntennas][dut::maxNumTxRegions]);
DUT_C_API bool DUT_C_API_ENTRY DUT_driverInit(const DutHandle handle, bool snifferMode, dut::NvMemoryType memoryType, dut::NvMemorySize memorySize, dut::Band band);
DUT_C_API bool DUT_C_API_ENTRY DUT_driverRelease(const DutHandle handle);
DUT_C_API bool DUT_C_API_ENTRY DUT_flushNvm(const DutHandle handle);
DUT_C_API bool DUT_C_API_ENTRY DUT_getAvailableRxAntennaMask(const DutHandle handle, dut::AntennaMask_t* antennaMask);
DUT_C_API bool DUT_C_API_ENTRY DUT_getAvailableTxAntennaMask(const DutHandle handle, dut::AntennaMask_t* antennaMask);
DUT_C_API bool DUT_C_API_ENTRY DUT_getBand(const DutHandle handle, dut::Band* band);
DUT_C_API bool DUT_C_API_ENTRY DUT_getSupportedBands(const DutHandle handle, dut::Band supportedBands[], size_t* size);
DUT_C_API bool DUT_C_API_ENTRY DUT_getBbicCddValues(const DutHandle handle, uint8_t numTxAntennas, uint32_t* offset1, uint32_t* offset2, uint32_t* offset3);
DUT_C_API bool DUT_C_API_ENTRY DUT_getCalibrationFileVersion(const DutHandle handle, dut::CalibrationFileVersion* version, dut::CalibrationFileSubversion* subversion);
DUT_C_API bool DUT_C_API_ENTRY DUT_getCardInfo(const DutHandle handle, uint8_t* countryCode, uint8_t macAddress[dut::cardInfoMacAddressSize], uint8_t serialNumber[dut::cardInfoSerialNumberSize], uint8_t* week, uint8_t* year);
DUT_C_API bool DUT_C_API_ENTRY DUT_getChipId(const DutHandle handle, dut::ChipID* chipId);
DUT_C_API bool DUT_C_API_ENTRY DUT_getComponentVersion(const DutHandle handle, dut::VersionedComponent component, char* version, size_t* size);
DUT_C_API bool DUT_C_API_ENTRY DUT_getEnabledRxAntennaMask(const DutHandle handle, dut::AntennaMask_t* antennaMask);
DUT_C_API bool DUT_C_API_ENTRY DUT_getEnabledTxAntennaMask(const DutHandle handle, dut::AntennaMask_t* antennaMask);
DUT_C_API bool DUT_C_API_ENTRY DUT_getFemType(const DutHandle handle, dut::FemType* femType);
DUT_C_API bool DUT_C_API_ENTRY DUT_getHardwareType(const DutHandle handle, dut::HardwareType* hardwareType);
DUT_C_API bool DUT_C_API_ENTRY DUT_getInbandRssi(const DutHandle handle, int16_t rssi[dut::maxNumRxAntennas]);
DUT_C_API bool DUT_C_API_ENTRY DUT_getMacPacketCounters(const DutHandle handle, uint32_t* receivedPackets);
DUT_C_API bool DUT_C_API_ENTRY DUT_getNmseValues(const DutHandle handle, int32_t values[dut::maxNumTxAntennas][dut::dpdTotalCalibrationPoints]);
DUT_C_API bool DUT_C_API_ENTRY DUT_getNvmSize(const DutHandle handle, size_t* size);
DUT_C_API bool DUT_C_API_ENTRY DUT_getNvmType(const DutHandle handle, dut::NvMemoryType* type);
DUT_C_API bool DUT_C_API_ENTRY DUT_getNvmVersion(const DutHandle handle, uint8_t* version);
DUT_C_API bool DUT_C_API_ENTRY DUT_getPhyMode(const DutHandle handle, dut::PhyMode* phyMode);
DUT_C_API bool DUT_C_API_ENTRY DUT_getPhyPacketCounters(const DutHandle handle, uint32_t* receivedPackets, uint32_t* crcErrors, uint32_t* forwardedPackets);
DUT_C_API bool DUT_C_API_ENTRY DUT_getProductionFlag(const DutHandle handle, bool* productionFlag);
DUT_C_API bool DUT_C_API_ENTRY DUT_getRfRssiPower(const DutHandle handle, int8_t power[dut::maxNumRxAntennas]);
DUT_C_API bool DUT_C_API_ENTRY DUT_getRssi(const DutHandle handle, uint8_t method, uint16_t numSamples, uint16_t rssi[dut::maxNumRxAntennas]);
DUT_C_API bool DUT_C_API_ENTRY DUT_getRssiCalibrationData(const DutHandle handle, dut::CalibrationFileVersion* version, dut::RssiCalibrationData_t data[], size_t* size);
DUT_C_API bool DUT_C_API_ENTRY DUT_getRxCwPower(const DutHandle handle, uint8_t method, int32_t freqOffset, uint16_t numSamples, dut::CorrelationResults_t correlationResults[dut::maxNumRxAntennas]);
DUT_C_API bool DUT_C_API_ENTRY DUT_getRxEvm(const DutHandle handle, uint8_t rxEvm[dut::maxNumRxAntennas]);
DUT_C_API bool DUT_C_API_ENTRY DUT_getRxRateInfo(const DutHandle handle, uint8_t* mcs, uint8_t* nss);
DUT_C_API bool DUT_C_API_ENTRY DUT_getTemperature(const DutHandle handle, float* temperature);
DUT_C_API bool DUT_C_API_ENTRY DUT_getTransmitPowerTableOffset(const DutHandle handle, uint8_t antenna, dut::Bandwidth bandwidth, int16_t* offset);
DUT_C_API bool DUT_C_API_ENTRY DUT_getTransmitVoltages(const DutHandle handle, uint32_t voltages[dut::maxNumTxAntennas]);
DUT_C_API bool DUT_C_API_ENTRY DUT_getTssiCalibrationData(const DutHandle handle, dut::CalibrationFileVersion* version, dut::TssiCalibrationData_t data[], size_t* size);
DUT_C_API bool DUT_C_API_ENTRY DUT_getXtalCalValue(const DutHandle handle, uint16_t* xtalValue);
DUT_C_API bool DUT_C_API_ENTRY DUT_getXtalRegValue(const DutHandle handle, uint16_t* xtalValue);
DUT_C_API bool DUT_C_API_ENTRY DUT_getZwdfsStatus(const DutHandle handle, dut::AntennaMask_t* antennaMask, bool* enabled);
DUT_C_API bool DUT_C_API_ENTRY DUT_loadBeamformingMatrixFromFileSet(const DutHandle handle, const char* primaryHeaderFile, const char* primaryValuesFile, const char* primaryExtValuesEhtFile, const char* secondaryHeaderFile, const char* secondaryValuesFile, const char* secondaryExtValuesEhtFile);
DUT_C_API bool DUT_C_API_ENTRY DUT_loadNvmFromFile(const DutHandle handle, const char* fileName);
DUT_C_API bool DUT_C_API_ENTRY DUT_measureRxLnaSubBandGains(const DutHandle handle);
DUT_C_API bool DUT_C_API_ENTRY DUT_readMemory(const DutHandle handle, dut::ChipModule chipModule, size_t address, uint8_t* data, size_t length);
DUT_C_API bool DUT_C_API_ENTRY DUT_readNvm(const DutHandle handle, size_t address, uint8_t* data, size_t length, bool useCache);
DUT_C_API bool DUT_C_API_ENTRY DUT_readRegister(const DutHandle handle, dut::ChipModule chipModule, size_t address, uint32_t mask, uint32_t* value);
DUT_C_API bool DUT_C_API_ENTRY DUT_resetMacPacketCounters(const DutHandle handle);
DUT_C_API bool DUT_C_API_ENTRY DUT_resetPhyPacketCounters(const DutHandle handle);
DUT_C_API bool DUT_C_API_ENTRY DUT_rxMeasure(const DutHandle handle, uint32_t numCaptures, uint32_t captureInterval, bool disabled);
DUT_C_API bool DUT_C_API_ENTRY DUT_saveNvmToFile(const DutHandle handle, const char* fileName);
DUT_C_API bool DUT_C_API_ENTRY DUT_setBbicCddValues(const DutHandle handle, uint8_t numTxAntennas, uint32_t offset1, uint32_t offset2, uint32_t offset3);
DUT_C_API bool DUT_C_API_ENTRY DUT_setCalibrationFileVersion(const DutHandle handle, dut::CalibrationFileVersion version, dut::CalibrationFileSubversion subversion);
DUT_C_API bool DUT_C_API_ENTRY DUT_setCardInfo(const DutHandle handle, uint8_t countryCode, const uint8_t macAddress[dut::cardInfoMacAddressSize], const uint8_t serialNumber[dut::cardInfoSerialNumberSize], uint8_t week, uint8_t year);
DUT_C_API bool DUT_C_API_ENTRY DUT_setChannel(const DutHandle handle, dut::PhyMode phyMode, dut::Bandwidth spectrumBandwidth, uint8_t lowestChannel, uint8_t primaryChannelIndex, dut::RegulationType regulationType);
DUT_C_API bool DUT_C_API_ENTRY DUT_setClipper(const DutHandle handle, bool enabled);
DUT_C_API bool DUT_C_API_ENTRY DUT_setEnabledRxAntennaMask(const DutHandle handle, dut::AntennaMask_t antennaMask);
DUT_C_API bool DUT_C_API_ENTRY DUT_setEnabledTxAntennaMask(const DutHandle handle, dut::AntennaMask_t antennaMask);
DUT_C_API bool DUT_C_API_ENTRY DUT_setHdkConfig(const DutHandle handle, uint32_t offlineCalMask, uint32_t onlineCalMask);
DUT_C_API bool DUT_C_API_ENTRY DUT_setIfs(const DutHandle handle, uint32_t ifs);
DUT_C_API bool DUT_C_API_ENTRY DUT_setProductionFlag(const DutHandle handle, bool productionFlag, bool writeToNvm);
DUT_C_API bool DUT_C_API_ENTRY DUT_setRate(const DutHandle handle, dut::Bandwidth signalBandwidth, dut::Mcs mcs, uint8_t nss, dut::Gi gi, dut::Ltf ltf, float* rateMbps);
DUT_C_API bool DUT_C_API_ENTRY DUT_setRssiCalibrationData(const DutHandle handle, const dut::RssiCalibrationData_t data[], size_t size);
DUT_C_API bool DUT_C_API_ENTRY DUT_setRxGainBlocks(const DutHandle handle, uint8_t lnaIndex, int8_t pgc1, int8_t pgc2, int8_t pgc3);
DUT_C_API bool DUT_C_API_ENTRY DUT_setRssiS2dInitials(const DutHandle handle, uint8_t auxAdcRes, float ioffsStep);
DUT_C_API bool DUT_C_API_ENTRY DUT_setRssiS2dParams(const DutHandle handle, dut::AntennaMask_t antennaMask, uint8_t region, uint8_t gain, uint8_t offset);
DUT_C_API bool DUT_C_API_ENTRY DUT_setRxAggregationEnabled(const DutHandle handle, bool enabled);
DUT_C_API bool DUT_C_API_ENTRY DUT_setRxBandLut(const DutHandle handle, dut::AntennaMask_t antennaMask, const uint16_t rxBandLut[dut::maxNumRxAntennas][dut::maxNumLnaSubBandsCrossingPoints], const uint8_t firstTunningIndex[dut::maxNumRxAntennas]);
DUT_C_API bool DUT_C_API_ENTRY DUT_setSpacelessTxEnabled(const DutHandle handle, bool enabled);
DUT_C_API bool DUT_C_API_ENTRY DUT_setTransmitPowerControl(const DutHandle handle, bool closedLoop, uint8_t powerLimit);
DUT_C_API bool DUT_C_API_ENTRY DUT_setTransmitPowerLevel(const DutHandle handle, uint8_t powerLevel);
DUT_C_API bool DUT_C_API_ENTRY DUT_setTransmitPowerTableOffset(const DutHandle handle, uint8_t antenna, dut::Bandwidth bandwidth, int16_t offset);
DUT_C_API bool DUT_C_API_ENTRY DUT_setTransmitPowerVector(const DutHandle handle, dut::TransmitPowerVector_t* transmitPowerVector);
DUT_C_API bool DUT_C_API_ENTRY DUT_setTssiCalibrationData(const DutHandle handle, const dut::TssiCalibrationData_t data[], size_t size);
DUT_C_API bool DUT_C_API_ENTRY DUT_setTssiS2dParams(const DutHandle handle, uint8_t antenna, uint8_t region, uint8_t powerThreshold, uint8_t gain, uint8_t offset);
DUT_C_API bool DUT_C_API_ENTRY DUT_setXtalCalValue(const DutHandle handle, uint16_t xtalValue);
DUT_C_API bool DUT_C_API_ENTRY DUT_setXtalRegValue(const DutHandle handle, uint16_t xtalValue);
DUT_C_API bool DUT_C_API_ENTRY DUT_setZwdfsConfiguration(const DutHandle handle, uint8_t zwdfsChannel, dut::Bandwidth zwdfsBandwidth, dut::Bandwidth radarDetectionBandwidth);
DUT_C_API bool DUT_C_API_ENTRY DUT_startCalibration(const DutHandle handle, const dut::StartCalibrationParams_t* params, uint8_t* status);
DUT_C_API bool DUT_C_API_ENTRY DUT_startCw(const DutHandle handle, int8_t amplitude, int16_t tone);
DUT_C_API bool DUT_C_API_ENTRY DUT_startRxCalibration(const DutHandle handle);
DUT_C_API bool DUT_C_API_ENTRY DUT_startRxPer(const DutHandle handle, uint32_t packetLimit);
DUT_C_API bool DUT_C_API_ENTRY DUT_startTx(const DutHandle handle, uint16_t repetitions, uint32_t packetLength, bool longData, bool beamforming, dut::CodingType codingType);
DUT_C_API bool DUT_C_API_ENTRY DUT_stopCw(const DutHandle handle);
DUT_C_API bool DUT_C_API_ENTRY DUT_stopRxCalibration(const DutHandle handle);
DUT_C_API bool DUT_C_API_ENTRY DUT_stopRxPer(const DutHandle handle, bool calcRxPer);
DUT_C_API bool DUT_C_API_ENTRY DUT_stopTx(const DutHandle handle);
DUT_C_API bool DUT_C_API_ENTRY DUT_validateBeamformingHeaderRegister(const DutHandle handle, dut::PhyMode expectedPhyMode, dut::Bandwidth expectedBandwidth);
DUT_C_API bool DUT_C_API_ENTRY DUT_writeCalibrationFile(const DutHandle handle, dut::NvMemoryType memoryType, dut::NvMemorySize memorySize);
DUT_C_API bool DUT_C_API_ENTRY DUT_writeMemory(const DutHandle handle, dut::ChipModule chipModule, size_t address, const uint8_t* data, size_t length);
DUT_C_API bool DUT_C_API_ENTRY DUT_writeNvm(const DutHandle handle, size_t address, const uint8_t* data, size_t length);
DUT_C_API bool DUT_C_API_ENTRY DUT_writeRegister(const DutHandle handle, dut::ChipModule chipModule, size_t address, uint32_t mask, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif
