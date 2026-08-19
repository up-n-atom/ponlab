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

#include "dut_c/Dut.h"

#include "dut/DutImpl.h"

#include <memory>

DutHandle DUT_C_API_ENTRY DUT_Create(uint8_t wlanIndex, ConnectionHandle connection, LoggerHandle logger)
{
    /** 
     * It's the responsability of the calling party to delete the connection and logger objects.
     * Since the DutImpl constructor requires shared_ptrs, create them with an empty custom deleter.
     */
    std::shared_ptr<dut::Connection> _connection(connection, [](const dut::Connection*) {
        // Object is deleted by calling party
    });
    std::shared_ptr<dut::Logger> _logger(logger, [](const dut::Logger*) {
        // Object is deleted by calling party
    });

    return new dut::DutImpl(wlanIndex, _connection, _logger, false);
}

void DUT_C_API_ENTRY DUT_Destroy(DutHandle* handle)
{
    if (handle) {
        delete *handle;
        *handle = nullptr;
    }
}

DUT_C_API const char* DUT_C_API_ENTRY DUT_getVersion()
{
    static std::string version = dut::Dut::getVersion();

    return version.c_str();
}

DUT_C_API const char* DUT_C_API_ENTRY DUT_getBuildTag()
{
    static std::string buildTag = dut::Dut::getBuildTag();

    return buildTag.c_str();
}

bool DUT_C_API_ENTRY DUT_getLastError(const DutHandle handle, char* buffer, size_t* size)
{
    if (size == nullptr) {
        return false;
    }

    std::string lastError = handle->getLastError();

    bool ok = true;
    if (buffer) {
        if (*size > lastError.length()) {
            if (0 != strcpy_s(buffer, *size, lastError.c_str())) {
                ok = false;
            }
        } else {
            ok = false;
        }
    }

    *size = lastError.length() + 1;

    return ok;
}

bool DUT_C_API_ENTRY DUT_calibrateTssiS2d(const DutHandle handle, uint8_t lowerPowerLimit, uint8_t upperPowerLimit, uint32_t tssiLow, uint32_t tssiHigh, uint8_t region, uint8_t powerThreshold, uint8_t* gain, uint8_t* offset)
{
    if ((gain == nullptr) || (offset == nullptr)) {
        return false;
    }

    return handle->calibrateTssiS2d(lowerPowerLimit, upperPowerLimit, tssiLow, tssiHigh, region, powerThreshold, *gain, *offset);
}

bool DUT_C_API_ENTRY DUT_calculateLinearRegression(const DutHandle handle, const float powerVector[dut::numPowerPoints], const uint16_t voltages[dut::maxNumRxAntennas][dut::numPowerPoints], const uint16_t offsets[dut::maxNumRxAntennas], const float lnaGains[dut::maxNumRxAntennas], dut::RssiAB_t result[dut::maxNumRxAntennas], float maxErrors[dut::maxNumRxAntennas])
{
    if ((powerVector == nullptr) || (voltages == nullptr) || (offsets == nullptr) || (lnaGains == nullptr) || (result == nullptr) || (maxErrors == nullptr)) {
        return false;
    }

    std::array<float, dut::numPowerPoints> _powerVector;
    for (size_t i = 0; i < dut::numPowerPoints; i++) {
        _powerVector[i] = powerVector[i];
    }

    std::array<std::array<uint16_t, dut::numPowerPoints>, dut::maxNumRxAntennas> _voltages;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        for (size_t j = 0; j < dut::numPowerPoints; j++) {
            _voltages[i][j] = voltages[i][j];
        }
    }

    std::array<uint16_t, dut::maxNumRxAntennas> _offsets;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        _offsets[i] = offsets[i];
    }

    std::array<float, dut::maxNumRxAntennas> _lnaGains;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        _lnaGains[i] = lnaGains[i];
    }

    std::array<dut::RssiAB_t, dut::maxNumRxAntennas> _result {};
    std::array<float, dut::maxNumRxAntennas> _maxErrors {};

    bool ok = handle->calculateLinearRegression(_powerVector, _voltages, _offsets, _lnaGains, _result, _maxErrors);
    if (ok) {
        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            result[i] = _result.at(i);
            maxErrors[i] = _maxErrors.at(i);
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_calculateOptimalS2dGains(const DutHandle handle, const uint16_t rssiVoltage1[dut::maxNumRxAntennas], const uint16_t rssiVoltage2[dut::maxNumRxAntennas], int32_t regionS2dGain, int32_t optimalS2dGains[dut::maxNumRxAntennas])
{
    if ((rssiVoltage1 == nullptr) || (rssiVoltage2 == nullptr) || (optimalS2dGains == nullptr)) {
        return false;
    }

    std::array<uint16_t, dut::maxNumRxAntennas> _rssiVoltage1;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        _rssiVoltage1[i] = rssiVoltage1[i];
    }

    std::array<uint16_t, dut::maxNumRxAntennas> _rssiVoltage2;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        _rssiVoltage2[i] = rssiVoltage2[i];
    }

    std::array<int32_t, dut::maxNumRxAntennas> _optimalS2dGains {};

    bool ok = handle->calculateOptimalS2dGains(_rssiVoltage1, _rssiVoltage2, regionS2dGain, _optimalS2dGains);
    if (ok) {
        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            optimalS2dGains[i] = _optimalS2dGains.at(i);
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_calculateOptimalS2dOffsets(const DutHandle handle, const uint16_t rssiVoltage1[dut::maxNumRxAntennas], const uint16_t rssiVoltage2[dut::maxNumRxAntennas], int32_t regionS2dGain, uint8_t regionS2dOffset, uint8_t optimalS2dOffsets[dut::maxNumRxAntennas])
{
    if ((rssiVoltage1 == nullptr) || (rssiVoltage2 == nullptr) || (optimalS2dOffsets == nullptr)) {
        return false;
    }

    std::array<uint16_t, dut::maxNumRxAntennas> _rssiVoltage1;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        _rssiVoltage1[i] = rssiVoltage1[i];
    }

    std::array<uint16_t, dut::maxNumRxAntennas> _rssiVoltage2;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        _rssiVoltage2[i] = rssiVoltage2[i];
    }

    std::array<uint8_t, dut::maxNumRxAntennas> _optimalS2dOffsets {};

    bool ok = handle->calculateOptimalS2dOffsets(_rssiVoltage1, _rssiVoltage2, regionS2dGain, regionS2dOffset, _optimalS2dOffsets);
    if (ok) {
        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            optimalS2dOffsets[i] = _optimalS2dOffsets.at(i);
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_calculateMaxPacketLength(const DutHandle handle, dut::PhyMode phyMode, uint32_t* maxPacketLength)
{
    if (maxPacketLength == nullptr) {
        return false;
    }

    return handle->calculateMaxPacketLength(phyMode, *maxPacketLength);
}

bool DUT_C_API_ENTRY DUT_calculatePacketLength(const DutHandle handle, dut::PhyMode phyMode, dut::Bandwidth signalBandwidth, dut::Mcs mcs, uint8_t nss, uint32_t numSymbols, uint32_t* packetLength)
{
    if (packetLength == nullptr) {
        return false;
    }

    return handle->calculatePacketLength(phyMode, signalBandwidth, mcs, nss, numSymbols, *packetLength);
}

bool DUT_C_API_ENTRY DUT_calculatePhyDataRate(const DutHandle handle, dut::PhyMode phyMode, dut::Bandwidth signalBandwidth, dut::Mcs mcs, uint8_t nss, dut::Gi gi, float* rateMbps)
{
    if (rateMbps == nullptr) {
        return false;
    }

    return handle->calculatePhyDataRate(phyMode, signalBandwidth, mcs, nss, gi, *rateMbps);
}

bool DUT_C_API_ENTRY DUT_calibrateRxLnaMidGains(const DutHandle handle, const float targetGains[dut::maxNumRxAntennas], const float prxin[dut::maxNumRxAntennas], uint16_t numSamples, int8_t pgc1, int8_t pgc2, float calcGain[dut::maxNumRxAntennas], uint8_t calcGainCtrl[dut::maxNumRxAntennas])
{
    if ((targetGains == nullptr) || (prxin == nullptr) || (calcGain == nullptr) || (calcGainCtrl == nullptr)) {
        return false;
    }

    std::array<float, dut::maxNumRxAntennas> _targetGains;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        _targetGains[i] = targetGains[i];
    }

    std::array<float, dut::maxNumRxAntennas> _prxin;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        _prxin[i] = prxin[i];
    }

    std::array<float, dut::maxNumRxAntennas> _calcGain {};
    std::array<uint8_t, dut::maxNumRxAntennas> _calcGainCtrl {};

    bool ok = handle->calibrateRxLnaMidGains(_targetGains, _prxin, numSamples, pgc1, pgc2, _calcGain, _calcGainCtrl);
    if (ok) {
        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            calcGain[i] = _calcGain.at(i);
            calcGainCtrl[i] = _calcGainCtrl.at(i);
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_calculateRxLnaGains(const DutHandle handle, const dut::CorrelationResults_t correlationResults[dut::maxNumRxAntennas], const float prxin[dut::maxNumRxAntennas], int8_t pgc1, int8_t pgc2, int8_t pgc3, float gains[dut::maxNumRxAntennas])
{
    if ((correlationResults == nullptr) || (prxin == nullptr) || (gains == nullptr)) {
        return false;
    }

    std::array<dut::CorrelationResults_t, dut::maxNumRxAntennas> _correlationResults;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        _correlationResults[i] = correlationResults[i];
    }

    std::array<float, dut::maxNumRxAntennas> _prxin;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        _prxin[i] = prxin[i];
    }

    std::array<float, dut::maxNumRxAntennas> _gains {};

    bool ok = handle->calculateRxLnaGains(_correlationResults, _prxin, pgc1, pgc2, pgc3, _gains);
    if (ok) {
        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            gains[i] = _gains.at(i);
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_calculateRxLnaSubBandGains(const DutHandle handle, uint16_t freqResultTable[dut::maxNumRxAntennas][dut::maxNumLnaSubBands])
{
    if (freqResultTable == nullptr) {
        return false;
    }

    std::array<std::array<uint16_t, dut::maxNumLnaSubBands>, dut::maxNumRxAntennas> _freqResultTable {};

    bool ok = handle->calculateRxLnaSubBandGains(_freqResultTable);
    if (ok) {
        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            for (size_t j = 0; j < dut::maxNumLnaSubBands; j++) {
                freqResultTable[i][j] = _freqResultTable[i][j];
            }
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_calculateRxPowerVector(const DutHandle handle, int32_t maxPower, const float lnaGains[dut::maxNumRxAntennas], int32_t lowerPowerBoundary, int32_t upperPowerBoundary, float powerVector[dut::numPowerPoints])
{
    if ((lnaGains == nullptr) || (powerVector == nullptr)) {
        return false;
    }

    std::array<float, dut::maxNumRxAntennas> _lnaGains;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        _lnaGains[i] = lnaGains[i];
    }

    std::array<float, dut::numPowerPoints> _powerVector {};

    bool ok = handle->calculateRxPowerVector(maxPower, _lnaGains, lowerPowerBoundary, upperPowerBoundary, _powerVector);
    if (ok) {
        for (size_t i = 0; i < dut::numPowerPoints; i++) {
            powerVector[i] = _powerVector.at(i);
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_calculateRxRfFlatness(const DutHandle handle, const int32_t P_Adc_array[dut::maxNumRxAntennas][dut::maxNumLnaSubBands], const float refGain[dut::maxNumRxAntennas], int8_t pgc1, int8_t pgc2, int8_t pgc3, const float pin[dut::maxNumRxAntennas][dut::maxNumLnaSubBands], float results[dut::maxNumRxAntennas][dut::maxNumLnaSubBands])
{
    if ((P_Adc_array == nullptr) || (refGain == nullptr) || (pin == nullptr) || (results == nullptr)) {
        return false;
    }

    std::array<std::array<int32_t, dut::maxNumLnaSubBands>, dut::maxNumRxAntennas> _P_Adc_array;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        for (size_t j = 0; j < dut::maxNumLnaSubBands; j++) {
            _P_Adc_array[i][j] = P_Adc_array[i][j];
        }
    }

    std::array<float, dut::maxNumRxAntennas> _refGain;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        _refGain[i] = refGain[i];
    }

    std::array<std::array<float, dut::maxNumLnaSubBands>, dut::maxNumRxAntennas> _pin;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        for (size_t j = 0; j < dut::maxNumLnaSubBands; j++) {
            _pin[i][j] = pin[i][j];
        }
    }

    std::array<std::array<float, dut::maxNumLnaSubBands>, dut::maxNumRxAntennas> _results {};

    bool ok = handle->calculateRxRfFlatness(_P_Adc_array, _refGain, pgc1, pgc2, pgc3, _pin, _results);
    if (ok) {
        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            for (size_t j = 0; j < dut::maxNumLnaSubBands; j++) {
                results[i][j] = _results[i][j];
            }
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_calculateTxABCoefficients(const DutHandle handle, dut::AntennaMask_t antennaMask, uint8_t numRegions, const dut::Point_t* pointsPerAntennaAndRegion[dut::maxNumTxAntennas][dut::maxNumTxRegions], const size_t numPointsPerAntennaAndRegion[dut::maxNumTxAntennas][dut::maxNumTxRegions], float nLog, dut::TssiAB_t coefficientsPerAntennaAndRegion[dut::maxNumTxAntennas][dut::maxNumTxRegions], float errorsPerAntennaAndRegion[dut::maxNumTxAntennas][dut::maxNumTxRegions])
{
    if ((pointsPerAntennaAndRegion == nullptr) || (numPointsPerAntennaAndRegion == nullptr) || (coefficientsPerAntennaAndRegion == nullptr) || (errorsPerAntennaAndRegion == nullptr)) {
        return false;
    }

    std::array<std::array<std::vector<dut::Point_t>, dut::maxNumTxRegions>, dut::maxNumTxAntennas> _pointsPerAntennaAndRegion;
    for (size_t i = 0; i < dut::maxNumTxAntennas; i++) {
        for (size_t j = 0; j < dut::maxNumTxRegions; j++) {
            for (size_t k = 0; k < numPointsPerAntennaAndRegion[i][j]; k++) {
                _pointsPerAntennaAndRegion[i][j].push_back(pointsPerAntennaAndRegion[i][j][k]);
            }
        }
    }

    std::array<std::array<dut::TssiAB_t, dut::maxNumTxRegions>, dut::maxNumTxAntennas> _coefficientsPerAntennaAndRegion {};
    std::array<std::array<float, dut::maxNumTxRegions>, dut::maxNumTxAntennas> _errorsPerAntennaAndRegion {};

    bool ok = handle->calculateTxABCoefficients(antennaMask, numRegions, _pointsPerAntennaAndRegion, nLog, _coefficientsPerAntennaAndRegion, _errorsPerAntennaAndRegion);
    if (ok) {
        for (size_t i = 0; i < dut::maxNumTxAntennas; i++) {
            for (size_t j = 0; j < dut::maxNumTxRegions; j++) {
                coefficientsPerAntennaAndRegion[i][j] = _coefficientsPerAntennaAndRegion[i][j];
                errorsPerAntennaAndRegion[i][j] = _errorsPerAntennaAndRegion[i][j];
            }
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_driverInit(const DutHandle handle, bool snifferMode, dut::NvMemoryType memoryType, dut::NvMemorySize memorySize, dut::Band band)
{
    return handle->driverInit(snifferMode, memoryType, memorySize, band);
}

bool DUT_C_API_ENTRY DUT_driverRelease(const DutHandle handle)
{
    return handle->driverRelease();
}

bool DUT_C_API_ENTRY DUT_flushNvm(const DutHandle handle)
{
    return handle->flushNvm();
}

bool DUT_C_API_ENTRY DUT_getAvailableRxAntennaMask(const DutHandle handle, dut::AntennaMask_t* antennaMask)
{
    if (antennaMask == nullptr) {
        return false;
    }

    dut::AntennaMask _antennaMask;

    bool ok = handle->getAvailableRxAntennaMask(_antennaMask);
    if (ok) {
        *antennaMask = _antennaMask;
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getAvailableTxAntennaMask(const DutHandle handle, dut::AntennaMask_t* antennaMask)
{
    if (antennaMask == nullptr) {
        return false;
    }

    dut::AntennaMask _antennaMask;

    bool ok = handle->getAvailableTxAntennaMask(_antennaMask);
    if (ok) {
        *antennaMask = _antennaMask;
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getBand(const DutHandle handle, dut::Band* band)
{
    if (band == nullptr) {
        return false;
    }

    return handle->getBand(*band);
}

bool DUT_C_API_ENTRY DUT_getSupportedBands(const DutHandle handle, dut::Band supportedBands[], size_t* size)
{
    if (size == nullptr) {
        return false;
    }

    std::vector<dut::Band> _supportedBands;

    bool ok = handle->getSupportedBands(_supportedBands);
    if (ok) {
        if (supportedBands) {
            if (*size < _supportedBands.size()) {
                ok = false;
            } else {
                for (size_t i = 0; i < _supportedBands.size(); i++) {
                    supportedBands[i] = _supportedBands.at(i);
                }
            }
        }

        *size = _supportedBands.size();
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getBbicCddValues(const DutHandle handle, uint8_t numTxAntennas, uint32_t* offset1, uint32_t* offset2, uint32_t* offset3)
{
    if ((offset1 == nullptr) || (offset2 == nullptr) || (offset3 == nullptr)) {
        return false;
    }

    return handle->getBbicCddValues(numTxAntennas, *offset1, *offset2, *offset3);
}

bool DUT_C_API_ENTRY DUT_getCalibrationFileVersion(const DutHandle handle, dut::CalibrationFileVersion* version, dut::CalibrationFileSubversion* subversion)
{
    if ((version == nullptr) || (subversion == nullptr)) {
        return false;
    }

    return handle->getCalibrationFileVersion(*version, *subversion);
}

bool DUT_C_API_ENTRY DUT_getCardInfo(const DutHandle handle, uint8_t* countryCode, uint8_t macAddress[dut::cardInfoMacAddressSize], uint8_t serialNumber[dut::cardInfoSerialNumberSize], uint8_t* week, uint8_t* year)
{
    if ((countryCode == nullptr) || (macAddress == nullptr) || (serialNumber == nullptr) || (week == nullptr) || (year == nullptr)) {
        return false;
    }

    std::array<uint8_t, dut::cardInfoMacAddressSize> _macAddress;
    std::array<uint8_t, dut::cardInfoSerialNumberSize> _serialNumber;

    bool ok = handle->getCardInfo(*countryCode, _macAddress, _serialNumber, *week, *year);
    if (ok) {
        memcpy(macAddress, _macAddress.data(), _macAddress.size());
        memcpy(serialNumber, _serialNumber.data(), _serialNumber.size());
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getChipId(const DutHandle handle, dut::ChipID* chipId)
{
    if (chipId == nullptr) {
        return false;
    }

    return handle->getChipId(*chipId);
}

bool DUT_C_API_ENTRY DUT_getComponentVersion(const DutHandle handle, dut::VersionedComponent component, char* version, size_t* size)
{
    if (size == nullptr) {
        return false;
    }

    std::string _version;

    bool ok = handle->getComponentVersion(component, _version);
    if (ok) {
        if (version) {
            if (*size > _version.length()) {
                memcpy(version, _version.c_str(), _version.length() + 1);
            } else {
                ok = false;
            }
        }

        *size = _version.length() + 1;
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getEnabledRxAntennaMask(const DutHandle handle, dut::AntennaMask_t* antennaMask)
{
    if (antennaMask == nullptr) {
        return false;
    }

    dut::AntennaMask _antennaMask;

    bool ok = handle->getEnabledRxAntennaMask(_antennaMask);
    if (ok) {
        *antennaMask = _antennaMask;
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getEnabledTxAntennaMask(const DutHandle handle, dut::AntennaMask_t* antennaMask)
{
    if (antennaMask == nullptr) {
        return false;
    }

    dut::AntennaMask _antennaMask;

    bool ok = handle->getEnabledTxAntennaMask(_antennaMask);
    if (ok) {
        *antennaMask = _antennaMask;
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getFemType(const DutHandle handle, dut::FemType* femType)
{
    if (femType == nullptr) {
        return false;
    }

    return handle->getFemType(*femType);
}

bool DUT_C_API_ENTRY DUT_getHardwareType(const DutHandle handle, dut::HardwareType* hardwareType)
{
    if (hardwareType == nullptr) {
        return false;
    }

    return handle->getHardwareType(*hardwareType);
}

bool DUT_C_API_ENTRY DUT_getInbandRssi(const DutHandle handle, int16_t rssi[dut::maxNumRxAntennas])
{
    if (rssi == nullptr) {
        return false;
    }

    std::array<int16_t, dut::maxNumRxAntennas> _rssi {};

    bool ok = handle->getInbandRssi(_rssi);
    if (ok) {
        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            rssi[i] = _rssi.at(i);
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getMacPacketCounters(const DutHandle handle, uint32_t* receivedPackets)
{
    if (receivedPackets == nullptr) {
        return false;
    }

    return handle->getMacPacketCounters(*receivedPackets);
}

bool DUT_C_API_ENTRY DUT_getMpduPacketCounters(const DutHandle handle, uint32_t* receivedPackets, uint32_t* errorPackets)
{
    if ((receivedPackets == nullptr) || (errorPackets == nullptr)) {
        return false;
    }

    return handle->getMpduPacketCounters(*receivedPackets, *errorPackets);
}

bool DUT_C_API_ENTRY DUT_getNmseValues(const DutHandle handle, int32_t values[dut::maxNumTxAntennas][dut::dpdTotalCalibrationPoints])
{
    if (values == nullptr) {
        return false;
    }

    std::array<std::array<int32_t, dut::dpdTotalCalibrationPoints>, dut::maxNumTxAntennas> _values {};

    bool result = handle->getNmseValues(_values);
    if (result) {
        for (size_t i = 0; i < dut::maxNumTxAntennas; i++) {
            for (size_t j = 0; j < dut::dpdTotalCalibrationPoints; j++) {
                values[i][j] = _values[i][j];
            }
        }
    }

    return result;
}

bool DUT_C_API_ENTRY DUT_getNvmSize(const DutHandle handle, size_t* size)
{
    if (size == nullptr) {
        return false;
    }

    return handle->getNvmSize(*size);
}

bool DUT_C_API_ENTRY DUT_getNvmType(const DutHandle handle, dut::NvMemoryType* type)
{
    if (type == nullptr) {
        return false;
    }

    return handle->getNvmType(*type);
}

bool DUT_C_API_ENTRY DUT_getNvmVersion(const DutHandle handle, uint8_t* version)
{
    if (version == nullptr) {
        return false;
    }

    return handle->getNvmVersion(*version);
}

bool DUT_C_API_ENTRY DUT_getPhyMode(const DutHandle handle, dut::PhyMode* phyMode)
{
    if (phyMode == nullptr) {
        return false;
    }

    return handle->getPhyMode(*phyMode);
}

bool DUT_C_API_ENTRY DUT_getPhyPacketCounters(const DutHandle handle, uint32_t* receivedPackets, uint32_t* crcErrors, uint32_t* forwardedPackets)
{
    if ((receivedPackets == nullptr) || (crcErrors == nullptr) || (forwardedPackets == nullptr)) {
        return false;
    }

    return handle->getPhyPacketCounters(*receivedPackets, *crcErrors, *forwardedPackets);
}

bool DUT_C_API_ENTRY DUT_getProductionFlag(const DutHandle handle, bool* productionFlag)
{
    if (productionFlag == nullptr) {
        return false;
    }

    return handle->getProductionFlag(*productionFlag);
}

bool DUT_C_API_ENTRY DUT_getRfRssiPower(const DutHandle handle, int8_t power[dut::maxNumRxAntennas])
{
    if (power == nullptr) {
        return false;
    }

    std::array<int8_t, dut::maxNumRxAntennas> _power {};

    bool ok = handle->getRfRssiPower(_power);
    if (ok) {
        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            power[i] = _power.at(i);
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getRssi(const DutHandle handle, uint8_t method, uint16_t numSamples, uint16_t rssi[dut::maxNumRxAntennas])
{
    if (rssi == nullptr) {
        return false;
    }

    std::array<uint16_t, dut::maxNumRxAntennas> _rssi {};

    bool ok = handle->getRssi(method, numSamples, _rssi);
    if (ok) {
        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            rssi[i] = _rssi.at(i);
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getRssiCalibrationData(const DutHandle handle, dut::CalibrationFileVersion* version, dut::RssiCalibrationData_t data[], size_t* size)
{
    if ((version == nullptr) || (size == nullptr)) {
        return false;
    }

    std::vector<dut::RssiCalibrationData_t> _data;

    bool ok = handle->getRssiCalibrationData(*version, _data);
    if (ok) {
        if (data) {
            if (*size < _data.size()) {
                ok = false;
            } else {
                for (size_t i = 0; i < _data.size(); i++) {
                    data[i] = _data.at(i);
                }
            }
        }

        *size = _data.size();
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getRxCwPower(const DutHandle handle, uint8_t method, int32_t freqOffset, uint16_t numSamples, dut::CorrelationResults_t correlationResults[dut::maxNumRxAntennas])
{
    if (correlationResults == nullptr) {
        return false;
    }

    std::array<dut::CorrelationResults_t, dut::maxNumRxAntennas> _correlationResults {};

    bool ok = handle->getRxCwPower(method, freqOffset, numSamples, _correlationResults);
    if (ok) {
        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            correlationResults[i] = _correlationResults.at(i);
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getRxEvm(const DutHandle handle, uint8_t rxEvm[dut::maxNumRxAntennas])
{
    if (rxEvm == nullptr) {
        return false;
    }

    std::array<uint8_t, dut::maxNumRxAntennas> _rxEvm {};

    bool ok = handle->getRxEvm(_rxEvm);
    if (ok) {
        for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
            rxEvm[i] = _rxEvm.at(i);
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getRxRateInfo(const DutHandle handle, uint8_t* mcs, uint8_t* nss)
{
    if ((mcs == nullptr) || (nss == nullptr)) {
        return false;
    }

    return handle->getRxRateInfo(*mcs, *nss);
}

bool DUT_C_API_ENTRY DUT_getTemperature(const DutHandle handle, float* temperature)
{
    if (temperature == nullptr) {
        return false;
    }

    return handle->getTemperature(*temperature);
}

bool DUT_C_API_ENTRY DUT_getTransmitPowerTableOffset(const DutHandle handle, uint8_t antenna, dut::Bandwidth bandwidth, int16_t* offset)
{
    if (offset == nullptr) {
        return false;
    }

    return handle->getTransmitPowerTableOffset(antenna, bandwidth, *offset);
}

bool DUT_C_API_ENTRY DUT_getTransmitVoltages(const DutHandle handle, uint32_t voltages[dut::maxNumTxAntennas])
{
    if (voltages == nullptr) {
        return false;
    }

    std::array<uint32_t, dut::maxNumTxAntennas> _voltages {};

    bool ok = handle->getTransmitVoltages(_voltages);
    if (ok) {
        for (size_t i = 0; i < dut::maxNumTxAntennas; i++) {
            voltages[i] = _voltages.at(i);
        }
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getTssiCalibrationData(const DutHandle handle, dut::CalibrationFileVersion* version, dut::TssiCalibrationData_t data[], size_t* size)
{
    if ((version == nullptr) || (size == nullptr)) {
        return false;
    }

    std::vector<dut::TssiCalibrationData_t> _data;

    bool ok = handle->getTssiCalibrationData(*version, _data);
    if (ok) {
        if (data) {
            if (*size < _data.size()) {
                ok = false;
            } else {
                for (size_t i = 0; i < _data.size(); i++) {
                    data[i] = _data.at(i);
                }
            }
        }

        *size = _data.size();
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_getXtalCalValue(const DutHandle handle, uint16_t* xtalValue)
{
    if (xtalValue == nullptr) {
        return false;
    }

    return handle->getXtalCalValue(*xtalValue);
}

bool DUT_C_API_ENTRY DUT_getXtalRegValue(const DutHandle handle, uint16_t* xtalValue)
{
    if (xtalValue == nullptr) {
        return false;
    }

    return handle->getXtalRegValue(*xtalValue);
}

bool DUT_C_API_ENTRY DUT_getZwdfsStatus(const DutHandle handle, dut::AntennaMask_t* antennaMask, bool* enabled)
{
    if ((antennaMask == nullptr) || (enabled == nullptr)) {
        return false;
    }

    dut::AntennaMask _antennaMask;

    bool ok = handle->getZwdfsStatus(_antennaMask, *enabled);
    if (ok) {
        *antennaMask = _antennaMask;
    }

    return ok;
}

bool DUT_C_API_ENTRY DUT_loadBeamformingMatrixFromFileSet(const DutHandle handle,
    const char* primaryHeaderFile, const char* primaryValuesFile, const char* primaryExtValuesEhtFile,
    const char* secondaryHeaderFile, const char* secondaryValuesFile, const char* secondaryExtValuesEhtFile)
{
    if (!handle || !primaryHeaderFile || !primaryValuesFile) {
        return false;
    }

    dut::BeamformingFilePathSet_t primaryFileSet;
    primaryFileSet.headerFile = primaryHeaderFile;
    primaryFileSet.valuesFile = primaryValuesFile;
    primaryFileSet.extValuesEhtFile = primaryExtValuesEhtFile ? primaryExtValuesEhtFile : "";

    dut::BeamformingFilePathSet_t secondaryFileSet {};
    if (secondaryHeaderFile && secondaryValuesFile) {
        secondaryFileSet.headerFile = secondaryHeaderFile;
        secondaryFileSet.valuesFile = secondaryValuesFile;
        secondaryFileSet.extValuesEhtFile = secondaryExtValuesEhtFile ? secondaryExtValuesEhtFile : "";
    }

    return handle->loadBeamformingMatrixFromFileSet(primaryFileSet, secondaryFileSet);
}

bool DUT_C_API_ENTRY DUT_loadNvmFromFile(const DutHandle handle, const char* fileName)
{
    return handle->loadNvmFromFile(fileName);
}

bool DUT_C_API_ENTRY DUT_measureRxLnaSubBandGains(const DutHandle handle)
{
    return handle->measureRxLnaSubBandGains();
}

bool DUT_C_API_ENTRY DUT_readMemory(const DutHandle handle, dut::ChipModule chipModule, size_t address, uint8_t* data, size_t length)
{
    return handle->readMemory(chipModule, address, data, length);
}

bool DUT_C_API_ENTRY DUT_readNvm(const DutHandle handle, size_t address, uint8_t* data, size_t length, bool useCache)
{
    return handle->readNvm(address, data, length, useCache);
}

bool DUT_C_API_ENTRY DUT_readRegister(const DutHandle handle, dut::ChipModule chipModule, size_t address, uint32_t mask, uint32_t* value)
{
    if (value == nullptr) {
        return false;
    }

    return handle->readRegister(chipModule, address, mask, *value);
}

bool DUT_C_API_ENTRY DUT_resetMacPacketCounters(const DutHandle handle)
{
    return handle->resetMacPacketCounters();
}

bool DUT_C_API_ENTRY DUT_resetMpduPacketCounters(const DutHandle handle)
{
    return handle->resetMpduPacketCounters();
}

bool DUT_C_API_ENTRY DUT_resetPhyPacketCounters(const DutHandle handle)
{
    return handle->resetPhyPacketCounters();
}

bool DUT_C_API_ENTRY DUT_rxMeasure(const DutHandle handle, uint32_t numCaptures, uint32_t captureInterval, bool disabled)
{
    return handle->rxMeasure(numCaptures, captureInterval, disabled);
}

bool DUT_C_API_ENTRY DUT_saveNvmToFile(const DutHandle handle, const char* fileName)
{
    if (fileName == nullptr) {
        return false;
    }

    return handle->saveNvmToFile(fileName);
}

bool DUT_C_API_ENTRY DUT_setBbicCddValues(const DutHandle handle, uint8_t numTxAntennas, uint32_t offset1, uint32_t offset2, uint32_t offset3)
{
    return handle->setBbicCddValues(numTxAntennas, offset1, offset2, offset3);
}

bool DUT_C_API_ENTRY DUT_setCalibrationFileVersion(const DutHandle handle, dut::CalibrationFileVersion version, dut::CalibrationFileSubversion subversion)
{
    return handle->setCalibrationFileVersion(version, subversion);
}

bool DUT_C_API_ENTRY DUT_setCardInfo(const DutHandle handle, uint8_t countryCode, const uint8_t macAddress[dut::cardInfoMacAddressSize], const uint8_t serialNumber[dut::cardInfoSerialNumberSize], uint8_t week, uint8_t year)
{
    if ((macAddress == nullptr) || (serialNumber == nullptr)) {
        return false;
    }

    std::array<uint8_t, dut::cardInfoMacAddressSize> _macAddress;
    std::array<uint8_t, dut::cardInfoSerialNumberSize> _serialNumber;

    memcpy(_macAddress.data(), macAddress, _macAddress.size());
    memcpy(_serialNumber.data(), serialNumber, _serialNumber.size());

    return handle->setCardInfo(countryCode, _macAddress, _serialNumber, week, year);
}

bool DUT_C_API_ENTRY DUT_setChannel(const DutHandle handle, dut::PhyMode phyMode, dut::Bandwidth spectrumBandwidth, uint8_t lowestChannel, uint8_t primaryChannelIndex, dut::RegulationType regulationType)
{
    return handle->setChannel(phyMode, spectrumBandwidth, lowestChannel, primaryChannelIndex, regulationType);
}

bool DUT_C_API_ENTRY DUT_setClipper(const DutHandle handle, bool enabled)
{
    return handle->setClipper(enabled);
}

bool DUT_C_API_ENTRY DUT_setEnabledRxAntennaMask(const DutHandle handle, dut::AntennaMask_t antennaMask)
{
    return handle->setEnabledRxAntennaMask(antennaMask);
}

bool DUT_C_API_ENTRY DUT_setEnabledTxAntennaMask(const DutHandle handle, dut::AntennaMask_t antennaMask)
{
    return handle->setEnabledTxAntennaMask(antennaMask);
}

bool DUT_C_API_ENTRY DUT_setHdkConfig(const DutHandle handle, uint32_t offlineCalMask, uint32_t onlineCalMask)
{
    return handle->setHdkConfig(offlineCalMask, onlineCalMask);
}

bool DUT_C_API_ENTRY DUT_setIfs(const DutHandle handle, uint32_t ifs)
{
    return handle->setIfs(ifs);
}

bool DUT_C_API_ENTRY DUT_setProductionFlag(const DutHandle handle, bool productionFlag, bool writeToNvm)
{
    return handle->setProductionFlag(productionFlag, writeToNvm);
}

bool DUT_C_API_ENTRY DUT_setRate(const DutHandle handle, dut::Bandwidth signalBandwidth, dut::Mcs mcs, uint8_t nss, dut::Gi gi, dut::Ltf ltf, float* rateMbps)
{
    return handle->setRate(signalBandwidth, mcs, nss, gi, ltf, *rateMbps);
}

bool DUT_C_API_ENTRY DUT_setRssiCalibrationData(const DutHandle handle, const dut::RssiCalibrationData_t data[], size_t size)
{
    if (data == nullptr) {
        return false;
    }

    std::vector<dut::RssiCalibrationData_t> _data;
    for (size_t i = 0; i < size; i++) {
        _data.push_back(data[i]);
    }

    return handle->setRssiCalibrationData(_data);
}

bool DUT_C_API_ENTRY DUT_setRxGainBlocks(const DutHandle handle, uint8_t lnaIndex, int8_t pgc1, int8_t pgc2, int8_t pgc3)
{
    return handle->setRxGainBlocks(lnaIndex, pgc1, pgc2, pgc3);
}

bool DUT_C_API_ENTRY DUT_setRssiS2dInitials(const DutHandle handle, uint8_t auxAdcRes, float ioffsStep)
{
    return handle->setRssiS2dInitials(auxAdcRes, ioffsStep);
}

bool DUT_C_API_ENTRY DUT_setRssiS2dParams(const DutHandle handle, dut::AntennaMask_t antennaMask, uint8_t region, uint8_t gain, uint8_t offset)
{
    return handle->setRssiS2dParams(antennaMask, region, gain, offset);
}

bool DUT_C_API_ENTRY DUT_setRuParams(const DutHandle handle, uint32_t userOne, uint32_t userTwo)
{
    return handle->setRuParams(userOne, userTwo);
}

bool DUT_C_API_ENTRY DUT_setRxAggregationEnabled(const DutHandle handle, bool enabled)
{
    return handle->setRxAggregationEnabled(enabled);
}

bool DUT_C_API_ENTRY DUT_setRxBandLut(const DutHandle handle, dut::AntennaMask_t antennaMask, const uint16_t rxBandLut[dut::maxNumRxAntennas][dut::maxNumLnaSubBandsCrossingPoints], const uint8_t firstTunningIndex[dut::maxNumRxAntennas])
{
    if ((rxBandLut == nullptr) || (firstTunningIndex == nullptr)) {
        return false;
    }

    std::array<std::array<uint16_t, dut::maxNumLnaSubBandsCrossingPoints>, dut::maxNumRxAntennas> _rxBandLut;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        for (size_t j = 0; j < dut::maxNumLnaSubBandsCrossingPoints; j++) {
            _rxBandLut[i][j] = rxBandLut[i][j];
        }
    }

    std::array<uint8_t, dut::maxNumRxAntennas> _firstTunningIndex;
    for (size_t i = 0; i < dut::maxNumRxAntennas; i++) {
        _firstTunningIndex[i] = firstTunningIndex[i];
    }

    return handle->setRxBandLut(antennaMask, _rxBandLut, _firstTunningIndex);
}

bool DUT_C_API_ENTRY DUT_setSpacelessTxEnabled(const DutHandle handle, bool enabled)
{
    return handle->setSpacelessTxEnabled(enabled);
}

bool DUT_C_API_ENTRY DUT_setTransmitPowerControl(const DutHandle handle, bool closedLoop, uint8_t powerLimit)
{
    return handle->setTransmitPowerControl(closedLoop, powerLimit);
}

bool DUT_C_API_ENTRY DUT_setTransmitPowerLevel(const DutHandle handle, uint8_t powerLevel)
{
    return handle->setTransmitPowerLevel(powerLevel);
}

bool DUT_C_API_ENTRY DUT_setTransmitPowerTableOffset(const DutHandle handle, uint8_t antenna, dut::Bandwidth bandwidth, int16_t offset)
{
    return handle->setTransmitPowerTableOffset(antenna, bandwidth, offset);
}

bool DUT_C_API_ENTRY DUT_setTransmitPowerVector(const DutHandle handle, dut::TransmitPowerVector_t* transmitPowerVector)
{
    if (transmitPowerVector == nullptr) {
        return false;
    }

    return handle->setTransmitPowerVector(*transmitPowerVector);
}

bool DUT_C_API_ENTRY DUT_setTssiCalibrationData(const DutHandle handle, const dut::TssiCalibrationData_t data[], size_t size)
{
    if (data == nullptr) {
        return false;
    }

    std::vector<dut::TssiCalibrationData_t> _data;
    for (size_t i = 0; i < size; i++) {
        _data.push_back(data[i]);
    }

    return handle->setTssiCalibrationData(_data);
}

bool DUT_C_API_ENTRY DUT_setTssiS2dParams(const DutHandle handle, uint8_t antenna, uint8_t region, uint8_t powerThreshold, uint8_t gain, uint8_t offset)
{
    return handle->setTssiS2dParams(antenna, region, powerThreshold, gain, offset);
}

bool DUT_C_API_ENTRY DUT_setXtalCalValue(const DutHandle handle, uint16_t xtalValue)
{
    return handle->setXtalCalValue(xtalValue);
}

bool DUT_C_API_ENTRY DUT_setXtalRegValue(const DutHandle handle, uint16_t xtalValue)
{
    return handle->setXtalRegValue(xtalValue);
}

bool DUT_C_API_ENTRY DUT_setZwdfsConfiguration(const DutHandle handle, uint8_t zwdfsChannel, dut::Bandwidth zwdfsBandwidth, dut::Bandwidth radarDetectionBandwidth)
{
    return handle->setZwdfsConfiguration(zwdfsChannel, zwdfsBandwidth, radarDetectionBandwidth);
}

bool DUT_C_API_ENTRY DUT_startCalibration(const DutHandle handle, const dut::StartCalibrationParams_t* params, uint8_t* status)
{
    dut::StartCalibrationParams_t _params = { 0 };
    uint8_t _status = 0;

    memcpy(&_params, params, sizeof(_params));

    bool result = handle->startCalibration(_params, _status);
    *status = _status;

    return result;
}

bool DUT_C_API_ENTRY DUT_startCw(const DutHandle handle, int8_t amplitude, int16_t tone)
{
    return handle->startCw(amplitude, tone);
}

bool DUT_C_API_ENTRY DUT_startRxCalibration(const DutHandle handle)
{
    return handle->startRxCalibration();
}

bool DUT_C_API_ENTRY DUT_startRxPer(const DutHandle handle, uint32_t packetLimit)
{
    return handle->startRxPer(packetLimit);
}

bool DUT_C_API_ENTRY DUT_startTx(const DutHandle handle, uint16_t repetitions, uint32_t packetLength, bool longData, bool beamforming, dut::CodingType codingType)
{
    return handle->startTx(repetitions, packetLength, longData, beamforming, codingType);
}

bool DUT_C_API_ENTRY DUT_stopCw(const DutHandle handle)
{
    return handle->stopCw();
}

bool DUT_C_API_ENTRY DUT_stopRxCalibration(const DutHandle handle)
{
    return handle->stopRxCalibration();
}

bool DUT_C_API_ENTRY DUT_stopRxPer(const DutHandle handle, bool calcRxPer)
{
    return handle->stopRxPer(calcRxPer);
}

bool DUT_C_API_ENTRY DUT_stopTx(const DutHandle handle)
{
    return handle->stopTx();
}

bool DUT_C_API_ENTRY DUT_validateBeamformingHeaderRegister(const DutHandle handle, dut::PhyMode expectedPhyMode, dut::Bandwidth expectedBandwidth)
{
    return handle->validateBeamformingHeaderRegister(expectedPhyMode, expectedBandwidth);
}

bool DUT_C_API_ENTRY DUT_writeCalibrationFile(const DutHandle handle, dut::NvMemoryType memoryType, dut::NvMemorySize memorySize)
{
    return handle->writeCalibrationFile(memoryType, memorySize);
}

bool DUT_C_API_ENTRY DUT_writeMemory(const DutHandle handle, dut::ChipModule chipModule, size_t address, const uint8_t* data, size_t length)
{
    return handle->writeMemory(chipModule, address, data, length);
}

bool DUT_C_API_ENTRY DUT_writeNvm(const DutHandle handle, size_t address, const uint8_t* data, size_t length)
{
    return handle->writeNvm(address, data, length);
}

bool DUT_C_API_ENTRY DUT_writeRegister(const DutHandle handle, dut::ChipModule chipModule, size_t address, uint32_t mask, uint32_t value)
{
    return handle->writeRegister(chipModule, address, mask, value);
}
