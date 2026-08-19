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

#include "CalibrationFile.h"
#include "Exceptions.h"
#include "NonVolatileMemory.h"
#include "Status.h"
#include "Transmitter.h"
#include "TssiCalibrationDataVer6.h"
#include "dut/Client.h"
#include "dut/Tools.h"

#include <algorithm>

#ifdef LINUX_HOST
#include <cmath>
#endif

namespace dut {

constexpr uint8_t logForS2d = 14;

constexpr size_t powerVectorMinIndex = 0;
constexpr size_t powerVectorMaxIndex = 1;
constexpr size_t powerVectorSize = 2;

constexpr uint8_t s2dOffsetMaxCtrl = 62;
constexpr uint8_t s2dOffsetMinCtrl = 0;
constexpr uint8_t s2dOffsetResCtrl = 1; //5; Dov 3.3

static void checkRegion(uint8_t region, uint8_t maxNumRegions)
{
    if ((region == 0) || (region > maxNumRegions)) {
        throw std::invalid_argument("Invalid region value (" + toString(region) + "). Valid values range from 1 to " + toString(maxNumRegions));
    }
}

static void checkTxRegion(uint8_t region)
{
    checkRegion(region, maxNumTxRegions);
}

static void calculateLinearRegression(CalibrationFileVersion version, std::vector<Point_t>& points, float nLog, TssiAB_t& coefficients, float& error)
{
    float sumX = 0;
    float sumY = 0;
    float sumXsqr = 0;
    float sumXY = 0;

    float div_val_A = (nLog > 0) ? static_cast<float>(std::pow(2, 19)) : static_cast<float>(std::pow(2, 18)); //%2^14 - fix point of coef, 2^4 - as we get tssi after average, 2 - as we work with 0.5 db resolution.
    float div_val_B; //%1/1024 - we multiple in implementation, 2^11 - fix point of coef, 2 - as we work with 0.5 db resolution.
    if (version == CalibrationFileVersion::CALIBRATION_FILE_VERSION_6) {
        div_val_B = (nLog > 0) ? static_cast<float>(std::pow(2, 2)) : static_cast<float>(std::pow(2, 1));
    } else if (version == CalibrationFileVersion::CALIBRATION_FILE_VERSION_7) {
        div_val_B = (nLog > 0) ? static_cast<float>(std::pow(2, 10)) : static_cast<float>(std::pow(2, 9));
    } else {
        throw std::invalid_argument("Invalid version " + toString(static_cast<int>(version)));
    }

    size_t n = points.size();
    for (size_t i = 0; i < n; ++i) {
        //transform power vector from logarithmic to linear domain
        float ylin = (nLog > 0) ? static_cast<float>(std::pow(10, (points[i].y / std::round(nLog)))) : points[i].y;
        sumX += points[i].x;
        sumY += ylin;
        sumXsqr += points[i].x * points[i].x;
        sumXY += points[i].x * ylin;
    }
    float a = (static_cast<float>(n) * sumXY - sumX * sumY) / (static_cast<float>(n) * sumXsqr - sumX * sumX);
    float b = (sumY - a * sumX) / static_cast<float>(n);

    //use fixed points
    auto tempA = static_cast<int>(std::round(a * div_val_A));
    auto tempB = static_cast<int>(std::round(b * div_val_B));
    // Now we have a line ax+b
    if (tempB > 32767) //protected overflow of 15bits
        tempB = 32767;
    else if (tempB < -32767) //protected overflow of 15bits
        tempB = -32767;
    if (tempA > 32767) //protected overflow of 15bits
        tempA = 32767;
    else if (tempA < -32767) //protected overflow of 15bits
        tempA = -32767;
    coefficients.a = static_cast<short>(tempA);
    coefficients.b = static_cast<short>(tempB);

    error = 0;
    //Calc error with fixed points
    a = static_cast<float>(coefficients.a) / div_val_A;
    b = static_cast<float>(coefficients.b) / div_val_B;
    for (size_t i = 0; i < n; ++i) {
        float err;
        if (nLog > 0) {
            err = std::abs(nLog * std::log10(a * points[i].x + b) - points[i].y);
        } else {
            err = std::abs((a * points[i].x + b) - points[i].y);
        }
        error = std::max(err, error);
    }
}

static void calculateTxABCoefficients(CalibrationFileVersion version, const std::vector<Point_t>& points, float nLog, TssiAB_t& coefficients, float& error)
{
    // Remove points with negative Y
    std::vector<Point_t> _points = points;
    _points.erase(std::remove_if(_points.begin(), _points.end(), [](const Point_t& point) { return point.y < 0; }), _points.end());

    if (_points.empty()) {
        throw std::invalid_argument("No valid points with y>=0");
    }

    // In old algo they use A+B+error, if 2 points return them + error=0.
    // In new algo we calc A(slope)+B(offset) also for 2 points.
    // Let's do Linear Regression:
    calculateLinearRegression(version, _points, nLog, coefficients, error);
}

static void calculateTxABCoefficients(CalibrationFileVersion version, uint8_t numRegions, const std::array<std::vector<Point_t>, maxNumTxRegions>& pointsPerRegion, float nLog, std::array<TssiAB_t, maxNumTxRegions>& coefficientsPerRegion, std::array<float, maxNumTxRegions>& errorsPerRegion)
{
    for (uint8_t region = 0; region < numRegions; region++) {
        calculateTxABCoefficients(version, pointsPerRegion[region], nLog, coefficientsPerRegion[region], errorsPerRegion[region]);
    }
}

bool DutImpl::calculateTxABCoefficients(const AntennaMask& antennaMask, uint8_t numRegions, const std::array<std::array<std::vector<Point_t>, maxNumTxRegions>, maxNumTxAntennas>& pointsPerAntennaAndRegion, float nLog, std::array<std::array<TssiAB_t, maxNumTxRegions>, maxNumTxAntennas>& coefficientsPerAntennaAndRegion, std::array<std::array<float, maxNumTxRegions>, maxNumTxAntennas>& errorsPerAntennaAndRegion)
{
    return execute("calculateTxABCoefficients", [this, antennaMask, numRegions, &pointsPerAntennaAndRegion, nLog, &coefficientsPerAntennaAndRegion, &errorsPerAntennaAndRegion]() {
        logInput({ { "antennaMask", toString(antennaMask) }, { "numRegions", toString(numRegions) }, { "nLog", toString(nLog) } });

        CalibrationFileVersion version = m_calibrationFile->getVersion();

        for (uint8_t antenna = 0; antenna < maxNumTxAntennas; antenna++) {
            if (!antennaMask.isBitSet(antenna)) {
                continue;
            }

            dut::calculateTxABCoefficients(version, numRegions, pointsPerAntennaAndRegion[antenna], nLog, coefficientsPerAntennaAndRegion[antenna], errorsPerAntennaAndRegion[antenna]);
        }
    });
}

bool DutImpl::calibrateTssiS2d(uint8_t lowerPowerLevel, uint8_t upperPowerLevel, uint32_t tssiLow, uint32_t tssiHigh, uint8_t region, uint8_t powerThreshold, uint8_t& gain, uint8_t& offset)
{
    return execute("calibrateTssiS2d", [this, lowerPowerLevel, upperPowerLevel, tssiLow, tssiHigh, region, powerThreshold, &gain, &offset]() {
        logInput({ { "lowerPowerLevel", toString(lowerPowerLevel) }, { "upperPowerLevel", toString(upperPowerLevel) }, { "tssiLow", toString(tssiLow) }, { "tssiHigh", toString(tssiHigh) }, { "region", toString(region) }, { "powerThreshold", toString(powerThreshold) }, { "gain", toString(gain) }, { "offset", toString(offset) } });

        if (lowerPowerLevel > maxPowerLevelIndex) {
            throw std::invalid_argument("The specified lower power level index (" + toString(lowerPowerLevel) + ") exceeds maximum allowed value (" + toString(maxPowerLevelIndex) + ")");
        }

        if (upperPowerLevel > maxPowerLevelIndex) {
            throw std::invalid_argument("The specified upper power level index (" + toString(upperPowerLevel) + ") exceeds maximum allowed value (" + toString(maxPowerLevelIndex) + ")");
        }

        if (lowerPowerLevel > upperPowerLevel) {
            throw std::invalid_argument("The specified lower power level index (" + toString(lowerPowerLevel) + ") is greater than the specified upper power level index (" + toString(upperPowerLevel) + ")");
        }

        checkTxRegion(region);

        if (powerThreshold > maxPowerLevelIndex) {
            throw std::invalid_argument("The specified power threshold value (" + toString(powerThreshold) + ") exceeds maximum allowed value (" + toString(maxPowerLevelIndex) + ")");
        }

        auto getAntennaIndex = [](const AntennaMask& antennaMask) {
            if (antennaMask.bitCount() > 1) {
                throw std::logic_error("Illegal antenna selection (only one TX antenna is allowed)");
            }

            for (uint8_t antenna = 0; antenna < maxNumTxAntennas; antenna++) {
                if (antennaMask.isBitSet(antenna)) {
                    return antenna;
                }
            }

            throw std::logic_error("Illegal antenna selection (at least one TX antenna must be selected)");
        };

        uint8_t antenna = getAntennaIndex(m_status->enabledTxAntennaMask.getValue());

        if (!((m_transmitter->getState() == Transmitter::State::TX_STARTED) && (!m_status->spacelessTxEnabled.getValue()))) {
            throw std::logic_error("Packet transmission not started");
        }

        auto setTssiS2dParams = [this](uint8_t antenna, uint8_t region, uint8_t powerThreshold, uint8_t gain, uint8_t offset) {
            m_transmitter->pause();

            m_client->setS2dParams(antenna, m_status->signalBandwidth.getValue(), region, powerThreshold, gain, offset);

            m_transmitter->resume();
        };

        auto transmitPowersGetVoltage = [this](uint8_t lowerPowerLevel, uint8_t upperPowerLevel, uint8_t antenna, std::array<uint32_t, powerVectorSize>& voltages, size_t length = powerVectorSize) {
            std::array<uint8_t, powerVectorSize> powerVector { lowerPowerLevel, upperPowerLevel };

            for (size_t i = 0; i < length; i++) {

                m_transmitter->pause();

                m_client->setTransmitPowerLevel(antenna, m_status->spectrumBandwidth.getValue(), powerVector[i]);

                m_transmitter->resume();

                std::array<uint32_t, maxNumTxAntennas> transmitVoltages {};
                m_client->getTransmitVoltages(m_status->enabledTxAntennaMask.getValue(), transmitVoltages);

                voltages[i] = transmitVoltages[antenna];
            }
        };

        auto s2dOffsetlResMeasure = [&](uint8_t lowerPowerLevel, uint8_t upperPowerLevel, uint32_t tssiLow, uint8_t antenna, uint8_t region, uint8_t powerThreshold, uint8_t gain, uint8_t offset) {
            auto tssiFullScale = static_cast<float>(std::pow(2, logForS2d));

            //calculated tssi step once 2D_OFFSET_RES_CNTRL is applied
            float upperBound = s2dOffsetResCtrl * (tssiFullScale / (s2dOffsetMaxCtrl + 1));

            //if  tssiLow and applied upper bound will exceed tssi range: calculation will change towards the lower limit
            offset = (tssiLow + upperBound < tssiFullScale) ? offset - s2dOffsetResCtrl : offset + s2dOffsetResCtrl;

            //setting the new offset +/- s2dOffsetResCtrl with the default gain
            setTssiS2dParams(antenna, region, powerThreshold, gain, offset);

            std::array<uint32_t, powerVectorSize> voltages {};
            transmitPowersGetVoltage(lowerPowerLevel, upperPowerLevel, antenna, voltages, 1);

            uint32_t tssiHigh = voltages[powerVectorMinIndex];

            //calculating the s2doffset Cntrl tssi Resolution step
            return std::abs(static_cast<int>((tssiHigh - tssiLow) / s2dOffsetResCtrl));
        };

        setTssiS2dParams(antenna, region, powerThreshold, gain, offset);

        std::array<uint32_t, powerVectorSize> voltages;
        transmitPowersGetVoltage(lowerPowerLevel, upperPowerLevel, antenna, voltages);

        // Calculate average TSSI voltage
        float tssiAvg = (voltages[powerVectorMinIndex] + voltages[powerVectorMaxIndex]) / 2.0f;

        uint32_t s2dOffsetlRes = s2dOffsetlResMeasure(lowerPowerLevel, upperPowerLevel, voltages[powerVectorMinIndex], antenna, region, powerThreshold, gain, offset);

        // Calculate offset required change

        auto nLogMinus1 = static_cast<float>(std::pow(2, (logForS2d - 1)));
        auto dOffs = static_cast<uint8_t>(lq_round((tssiAvg - nLogMinus1) / s2dOffsetlRes)); //Math.Pow(2, m_nLogForS2D) / m_vOffset);

        //update S2D offset for current region and protect allowed control configuration

        offset += dOffs;
        offset = std::min(s2dOffsetMaxCtrl, offset);
        offset = std::max(s2dOffsetMinCtrl, offset);

        setTssiS2dParams(antenna, region, powerThreshold, gain, offset);

        transmitPowersGetVoltage(lowerPowerLevel, upperPowerLevel, antenna, voltages);

        //find optimum gain by calculating tssi voltage std (standard deviation)
        //     float tssiStd = Math.Max(Math.Abs(tssiVoltage[powerVectorMinIndex] - nLogMinus1), Math.Abs(tssiVoltage[powerVectorMaxIndex] - nLogMinus1));
        //  float tssiStd = Math.Min(Math.Abs(tssiVoltage[powerVectorMinIndex] - nLogMinus1), Math.Abs(tssiVoltage[powerVectorMaxIndex] - nLogMinus1));

        float V1peak = std::abs(voltages[powerVectorMinIndex] - nLogMinus1);
        float V2peak = std::abs(voltages[powerVectorMaxIndex] - nLogMinus1);

        //     K = 10.^ (Gain_dB / 20);

        float Vpl = nLogMinus1 - tssiLow;
        float Vph = nLogMinus1 - (2 * nLogMinus1 - tssiHigh);
        double dG1max = 20.0 * std::log10(Vpl / V1peak);
        double dG2max = 20.0 * std::log10(Vph / V2peak);

        double dGopt = 2.0 * std::floor(std::min(dG1max, dG2max) / 2.0);

        auto wantedGain = static_cast<int>(dGopt);
        int wantedGain_dB = (gain * 2) - 4 + wantedGain;

        wantedGain = (wantedGain_dB + 4) / 2;

        //Ensure that wantedGain index is not negative
        wantedGain = std::max(wantedGain, 0);

        //Limit to 4 bits
        if (m_status->chipId.getValue() >= ChipID::CHIP_ID_GEN6_D2) {
            wantedGain = std::min(11, wantedGain);
        } else {
            wantedGain = std::min(15, wantedGain);
        }

        //update S2D gain for current region
        gain = static_cast<uint8_t>(wantedGain);

        setTssiS2dParams(antenna, region, powerThreshold, gain, offset);

        logOutput({ { "gain", toString(gain) }, { "offset", toString(offset) } });
    });
}

bool DutImpl::getTransmitPowerTableOffset(uint8_t antenna, Bandwidth bandwidth, int16_t& offset)
{
    return execute("getTransmitPowerTableOffset", [this, antenna, bandwidth, &offset]() {
        logInput({ { "antenna", toString(antenna) }, { "bandwidth", toString(bandwidth) } });

        if (antenna >= maxNumTxAntennas) {
            throw std::invalid_argument("Invalid antenna index value (" + toString(antenna) + "). Valid values range from 0 to " + toString(maxNumTxAntennas - 1));
        }

        m_client->getTransmitPowerTableOffset(antenna, bandwidth, offset);

        logOutput({ { "offset", toString(offset) } });
    });
}

bool DutImpl::getTransmitVoltages(std::array<uint32_t, maxNumTxAntennas>& voltages)
{
    return execute("getTransmitVoltages", [this, &voltages]() {
        voltages = {};
        if ((m_transmitter->getState() == Transmitter::State::TX_STARTED) && (!m_status->spacelessTxEnabled.getValue())) {
            m_client->getTransmitVoltages(m_status->enabledTxAntennaMask.getValue(), voltages);
        }

        std::string s;
        for (const auto& voltage : voltages) {
            if (s.empty()) {
                s += "[";
            } else {
                s += ",";
            }
            s += toString(voltage);
        }
        s += "]";
        logOutput({ { "voltages", s } });
    });
}

bool DutImpl::setTssiS2dParams(uint8_t antenna, uint8_t region, uint8_t powerThreshold, uint8_t gain, uint8_t offset)
{
    return execute("setTssiS2dParams", [this, antenna, region, powerThreshold, gain, offset]() {
        logInput({ { "antenna", toString(antenna) }, { "region", toString(region) }, { "powerThreshold", toString(powerThreshold) }, { "gain", toString(gain) }, { "offset", toString(offset) } });

        if (antenna >= maxNumTxAntennas) {
            throw std::invalid_argument("Invalid antenna index value (" + toString(antenna) + "). Valid values range from 0 to " + toString(maxNumTxAntennas - 1));
        }

        checkTxRegion(region);

        if (powerThreshold > maxPowerLevelIndex) {
            throw std::invalid_argument("The specified power threshold value (" + toString(powerThreshold) + ") exceeds maximum allowed value (" + toString(maxPowerLevelIndex) + ")");
        }

        if (!m_status->signalBandwidth.isValueSet()) {
            throw std::logic_error("Rate (and signal bandwidth) not set");
        }

        m_transmitter->pause();

        m_client->setS2dParams(antenna, m_status->signalBandwidth.getValue(), region, powerThreshold, gain, offset);

        m_transmitter->resume();
    });
}

bool DutImpl::setTransmitPowerLevel(uint8_t powerLevel)
{
    return execute("setTransmitPowerLevel", [this, powerLevel]() {
        if (powerLevel != 0xDD) {
            logInput({ { "powerLevel", toString(static_cast<int8_t>(powerLevel)) } });
        } else {
            logInput({ { "powerLevel", toString(powerLevel) } });
        }

        if (powerLevel != 0xDD) {
            if (static_cast<int8_t>(powerLevel) > maxPowerLevelIndex) {
                throw std::invalid_argument("The specified power level value (" + toString(static_cast<int8_t>(powerLevel)) + ") exceeds maximum allowed value (" + toString(maxPowerLevelIndex) + ")");
            } else if (static_cast<int8_t>(powerLevel) < minPowerLevelIndex) {
                throw std::invalid_argument("The specified power level value (" + toString(static_cast<int8_t>(powerLevel)) + ") exceeds minimum allowed value (" + toString(minPowerLevelIndex) + ")");
            }
        }

        if (!m_status->lowestChannel.isValueSet()) {
            throw std::logic_error("Channel not set");
        }

#ifdef STATUS_CACHE_ENABLED
        if (m_status->transmitPowerLevel.isValueSet() && (powerLevel == m_status->transmitPowerLevel.getValue())) {
            return;
        }
#endif

        m_transmitter->pause();

        for (uint8_t antenna = 0; antenna < maxNumTxAntennas; antenna++) {
            if (m_status->enabledTxAntennaMask.getValue().isBitSet(antenna)) {
                m_client->setTransmitPowerLevel(antenna, m_status->spectrumBandwidth.getValue(), powerLevel);
            }
        }

        m_transmitter->resume();

        m_status->transmitPowerLevel.setValue(powerLevel);
    });
}

bool DutImpl::setTransmitPowerTableOffset(uint8_t antenna, Bandwidth bandwidth, int16_t offset)
{
    return execute("setTransmitPowerTableOffset", [this, antenna, bandwidth, offset]() {
        logInput({ { "antenna", toString(antenna) }, { "bandwidth", toString(bandwidth) }, { "offset", toString(offset) } });

        if (antenna >= maxNumTxAntennas) {
            throw std::invalid_argument("Invalid antenna index value (" + toString(antenna) + "). Valid values range from 0 to " + toString(maxNumTxAntennas - 1));
        }

        const int16_t maxAbsoluteOffsetValue = transmitGainTableSize - transmitPowerTableSize;
        if (offset > maxAbsoluteOffsetValue) {
            throw std::invalid_argument("The specified offset value (" + toString(offset) + ") exceeds maximum allowed value (" + toString(maxAbsoluteOffsetValue) + ")");
        } else if (offset < -maxAbsoluteOffsetValue) {
            throw std::invalid_argument("The specified offset value (" + toString(offset) + ") exceeds minimum allowed value (" + toString(-maxAbsoluteOffsetValue) + ")");
        }

        m_client->setTransmitPowerTableOffset(antenna, bandwidth, offset);
    });
}

bool DutImpl::setTransmitPowerVector(TransmitPowerVector_t& transmitPowerVector)
{
    return execute("setTransmitPowerVector", [this, &transmitPowerVector]() {
        logInput({ { "lengthOfPowerVector", toString(transmitPowerVector.lengthOfPowerVector) }, { "startingPacketLength", toString(transmitPowerVector.startingPacketLength) }, { "packetLengthIncrementPerIndex", toString(transmitPowerVector.packetLengthIncrementPerIndex) } });

        if (m_transmitter->getState() != Transmitter::State::IDLE) {
            throw std::logic_error("Transmission state is not idle");
        }

        m_client->setTransmitPowerVector(m_status->enabledTxAntennaMask.getValue(), transmitPowerVector);
        m_client->stopTx();

        logOutput({ { "voltages[0][0]", toString(transmitPowerVector.measuredVoltagePerPower[0][0]) } });
    });
}

}
