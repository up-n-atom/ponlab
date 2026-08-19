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

#include "CalibrationData.h"
#include "Status.h"
#include "dut/Client.h"
#include "dut/Tools.h"

#ifdef LINUX_HOST
#include <cmath>
#endif

namespace dut {

static void checkRegion(uint8_t region, uint8_t maxNumRegions)
{
    if ((region == 0) || (region > maxNumRegions)) {
        throw std::invalid_argument("Invalid region value (" + toString(region) + "). Valid values range from 1 to " + toString(maxNumRegions));
    }
}

static void checkRxRegion(uint8_t region, ChipID chipId)
{
    if (chipId >= ChipID::CHIP_ID_GEN7) {
        checkRegion(region, maxNumRxRegionsVer7);
    } else {
        checkRegion(region, maxNumRxRegionsVer6);
    }
}

static float getAverageLnaGain(const std::array<float, maxNumRxAntennas>& lnaGains, const AntennaMask& antennaMask)
{
    float total = 0.0f;
    size_t count = lnaGains.size();

    for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
        if (antennaMask.isBitSet(antenna)) {
            total += lnaGains[antenna];
        } else {
            count--;
        }
    }

    return total / static_cast<float>(count);
}

static RssiAB_t getCalibrationSlopes(const std::array<Point_t, numPowerPoints>& points, float& maxError)
{
    float sumX = 0.0f;
    float sumY = 0.0f;
    float sumXsqr = 0.0f;
    float sumXY = 0.0f;

    for (size_t i = 0; i < points.size(); ++i) {
        float ylin = points[i].y; //already linear
        sumX += points[i].x;
        sumY += ylin;
        sumXsqr += points[i].x * points[i].x;
        sumXY += points[i].x * points[i].y;
    }

    float a = (points.size() * sumXY - sumX * sumY) / (points.size() * sumXsqr - sumX * sumX);
    float b = (sumY - a * sumX) / points.size();

    // Error handling
    maxError = 0;
    for (size_t i = 0; i < points.size(); ++i) {
        float ylin = points[i].y;
        float err = ylin - (a * points[i].x + b);
        maxError = std::max(std::abs(err), maxError);
    }

    b = b / a;

    return RssiAB_t { a, b };
}

bool DutImpl::calculateLinearRegression(const std::array<float, numPowerPoints>& powerVector, const std::array<std::array<uint16_t, numPowerPoints>, maxNumRxAntennas>& voltages, const std::array<uint16_t, maxNumRxAntennas>& offsets, const std::array<float, maxNumRxAntennas>& lnaGains, std::array<RssiAB_t, maxNumRxAntennas>& result, std::array<float, maxNumRxAntennas>& maxErrors)
{
    return execute("calculateLinearRegression", [this, powerVector, voltages, offsets, lnaGains, &result, &maxErrors]() {
        logInput({ { "powerVector", toString(powerVector.begin(), powerVector.end()) }, { "v[0][0]", toString(voltages[0][0]) }, { "c[0]", toString(offsets[0]) }, { "lnaGains[0]", toString(lnaGains[0]) } });

        AntennaMask antennaMask = m_status->enabledRxAntennaMask.getValue();

        for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
            if (antennaMask.isBitSet(antenna)) {
                std::array<Point_t, numPowerPoints> points;
                for (size_t i = 0; i < points.size(); i++) {
                    if (voltages[antenna][i] < offsets[antenna]) {
                        throw std::invalid_argument("Voltage must be larger than DC offset (antenna index: " + toString(antenna) + ", vector index: " + toString(i) + ")");
                    }

                    auto vdb = 20.0f * static_cast<float>(std::log10(16 * (voltages[antenna][i] - offsets[antenna]))); //y linear

                    Point_t point;
                    point.x = vdb / 2.0f;
                    point.y = (powerVector[i] + lnaGains[antenna]);

                    points[i] = point;
                }

                result[antenna] = getCalibrationSlopes(points, maxErrors[antenna]);
            } else {
                maxErrors[antenna] = -1.0f;
            }
        }

        logOutput({ { "result", toString(result.begin(), result.end()) }, { "maxErrors", toString(maxErrors.begin(), maxErrors.end()) } });
    });
}

bool DutImpl::calculateOptimalS2dGains(const std::array<uint16_t, maxNumRxAntennas>& rssiVoltage1, const std::array<uint16_t, maxNumRxAntennas>& rssiVoltage2, int32_t regionS2dGain, std::array<int32_t, maxNumRxAntennas>& optimalS2dGains)
{
    return execute("calculateOptimalS2dGains", [this, rssiVoltage1, rssiVoltage2, regionS2dGain, &optimalS2dGains]() {
        logInput({ { "rssiVoltage1", toString(rssiVoltage1.begin(), rssiVoltage1.end()) }, { "rssiVoltage2", toString(rssiVoltage2.begin(), rssiVoltage2.end()) }, { "regionS2dGain", toString(regionS2dGain) } });

        // 1.5[mV/K] - temperature coefficient of RSSI Detector DC voltage
        constexpr float Kt_dc = 1.5F;

        // 20K - temperature change that triggers the online calibration
        constexpr float Temp_Between_Cals = 20.0f;

        // margin for the offset due to the temperature change in [Volts]
        constexpr float MarginT = Temp_Between_Cals * Kt_dc / 1000;

        // s2d gain resolution in [dB]
        constexpr int32_t s2dGainStep = 2;
        int32_t minS2dGain = -10;

        // s2d min gain available [dB]
        if (m_status->chipId.getValue() >= ChipID::CHIP_ID_GEN6_D2) {
            minS2dGain = -4;
        }
        // s2d max gain available [dB]
        constexpr int32_t maxS2dGain = 20;

        AntennaMask antennaMask = m_status->enabledRxAntennaMask.getValue();

        for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
            if (antennaMask.isBitSet(antenna)) {
                float v1peak = std::abs(rssiVoltage1[antenna] - m_rssiS2dParams.vmid);
                float v2peak = std::abs(rssiVoltage2[antenna] - m_rssiS2dParams.vmid);
                float regionS2dGaindB = static_cast<float>((s2dGainStep * regionS2dGain + minS2dGain));
                float tmp = regionS2dGaindB / 20.0f;
                auto k = static_cast<float>(std::pow(10, tmp));
                float marginTemp = MarginT * k * m_rssiS2dParams.vmax;
                tmp = m_rssiS2dParams.vp / (v1peak + marginTemp);
                float dG1max = 20 * std::log10(tmp) /*Margin1Gain*/;
                tmp = m_rssiS2dParams.vp / (v2peak + marginTemp);
                float dG2max = 20 * std::log10(tmp) /*Margin2Gain*/;
                auto dGain = static_cast<int32_t>(s2dGainStep * std::floor(std::min(dG1max, dG2max) / s2dGainStep));

                optimalS2dGains[antenna] = std::min(std::max(static_cast<int32_t>(regionS2dGaindB + dGain), minS2dGain), maxS2dGain);
                //Limit to 4 bits
                optimalS2dGains[antenna] = std::min(15, (optimalS2dGains[antenna] - minS2dGain) / s2dGainStep);
            }
        }

        logOutput({ { "optimalS2dGains", toString(optimalS2dGains.begin(), optimalS2dGains.end()) } });
    });
}

bool DutImpl::calculateOptimalS2dOffsets(const std::array<uint16_t, maxNumRxAntennas>& rssiVoltage1, const std::array<uint16_t, maxNumRxAntennas>& rssiVoltage2, int32_t regionS2dGain, uint8_t regionS2dOffset, std::array<uint8_t, maxNumRxAntennas>& optimalS2dOffsets)
{
    return execute("calculateOptimalS2dOffsets", [this, rssiVoltage1, rssiVoltage2, regionS2dGain, regionS2dOffset, &optimalS2dOffsets]() {
        logInput({ { "rssiVoltage1", toString(rssiVoltage1.begin(), rssiVoltage1.end()) }, { "rssiVoltage2", toString(rssiVoltage2.begin(), rssiVoltage2.end()) }, { "regionS2dGain", toString(regionS2dGain) }, { "regionS2dOffset", toString(regionS2dOffset) } });

        // s2d min offset available [dB]
        constexpr uint8_t minS2dOffset = 0;

        // s2d max offset available [dB]
        uint8_t maxS2dOffset = 31;
        constexpr int32_t s2dGainStep = 2;
        int32_t minS2dGain = -10;
        if (m_status->chipId.getValue() >= ChipID::CHIP_ID_GEN6_D2) {
            maxS2dOffset = 63;
            minS2dGain = -4;
        }

        AntennaMask antennaMask = m_status->enabledRxAntennaMask.getValue();

        for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
            if (antennaMask.isBitSet(antenna)) {
                float vMean = (rssiVoltage1[antenna] + rssiVoltage2[antenna]) / 2.0f;
                float regionS2dGaindB = static_cast<float>((s2dGainStep * regionS2dGain + minS2dGain));
                float tmp = -regionS2dGaindB / 20.0f;
                auto k = static_cast<float>(std::pow(10, tmp));
                auto dIOffs = static_cast<uint8_t>(std::round(m_rssiS2dParams.m * (vMean - m_rssiS2dParams.vmid) / m_rssiS2dParams.vmax * k));

                optimalS2dOffsets[antenna] = std::min(std::max(static_cast<uint8_t>(regionS2dOffset + dIOffs), minS2dOffset), maxS2dOffset);
            }
        }

        logOutput({ { "optimalS2dOffsets", toString(optimalS2dOffsets.begin(), optimalS2dOffsets.end()) } });
    });
}

bool DutImpl::calibrateRxLnaMidGains(const std::array<float, maxNumRxAntennas>& targetGains, const std::array<float, maxNumRxAntennas>& prxin, uint16_t numSamples, int8_t pgc1, int8_t pgc2, std::array<float, maxNumRxAntennas>& calcGain, std::array<uint8_t, maxNumRxAntennas>& calcGainCtrl)
{
    return execute("calibrateRxLnaMidGains", [this, targetGains, prxin, numSamples, pgc1, pgc2, &calcGain, &calcGainCtrl]() {
        logInput({ { "targetGains", toString(targetGains.begin(), targetGains.end()) }, { "prxin", toString(prxin.begin(), prxin.end()) }, { "numSymbols", toString(numSamples) }, { "pgc1", toString(pgc1) }, { "pgc2", toString(pgc2) } });

        if (!m_status->rxCalibrationRunning.getValue()) {
            throw std::runtime_error("Invalid state: RX calibration not started");
        }

        AntennaMask antennaMask = m_status->enabledRxAntennaMask.getValue();

        std::array<std::array<float, maxNumRxAntennas>, maxNumLnaSubBands> glna {};

        std::array<float, maxNumRxAntennas> targetGainsQ5 {};
        for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
            targetGainsQ5[antenna] = targetGains[antenna] * 32;
        }

        //Align to SL 5
        int32_t pgc1_ = pgc1 * 32;
        int32_t pgc2_ = pgc2 * 32;

        std::array<float, maxNumRxAntennas> prxin_ {};
        for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
            prxin_[antenna] = prxin[antenna] * 32;
        }

        for (uint8_t midGain = 0; midGain < maxNumLnaSubBands; midGain++) {
            //Set same mid gain value to all antennas:
            std::array<uint8_t, maxNumRxAntennas> midGains {};
            for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
                if (antennaMask.isBitSet(antenna)) {
                    midGains[antenna] = midGain;
                }
            }

            m_client->setLnaMidGains(antennaMask, midGains);

            //Get CW power for point midGain (all antennas, each left shifted by 6):
            constexpr uint8_t method = 0;
            constexpr int32_t freqOffset = 5000;
            std::array<CorrelationResults_t, maxNumRxAntennas> correlationResults {};
            m_client->getRxCwPower(method, freqOffset, numSamples, antennaMask, correlationResults);

            //Save point midGain in the final param, Calculate LNA Mid Gain:
            for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
                if (antennaMask.isBitSet(antenna)) {
                    float grx = correlationResults[antenna].II - prxin_[antenna];
#pragma warning(disable : 28020) // The expression '0<=_Param_(1)&&_Param_(1)<=16-1' is not true at this call.
                    glna[midGain][antenna] = grx - pgc1_ - pgc2_; //Mid Gain is only in G5 (no Pgc3). glna is right shifted below
#pragma warning(default : 28020)
                }
            }
        }

        //We have all the required data, lets find closest entry:
        std::array<float, maxNumRxAntennas> abs_i {};
        std::array<uint8_t, maxNumRxAntennas> optIndex {};
        std::array<float, maxNumRxAntennas> minVal {};

        //Preparing initialize:
        for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
            if (antennaMask.isBitSet(antenna)) {
                minVal[antenna] = std::abs(targetGainsQ5[antenna] - (int)glna[0][antenna]);
                optIndex[antenna] = 0;
            }
        }

        for (uint8_t midGain = 0; midGain < maxNumLnaSubBands; midGain++) {
            for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
                if (antennaMask.isBitSet(antenna)) {
                    abs_i[antenna] = std::abs(targetGainsQ5[antenna] - glna[midGain][antenna]);

                    if (abs_i[antenna] <= minVal[antenna]) {
                        optIndex[antenna] = midGain;
                    }

                    minVal[antenna] = std::abs(targetGainsQ5[antenna] - glna[midGain][antenna]);
                }
            }
        }

        //Return the closest values:
        std::array<uint8_t, maxNumRxAntennas> midGains;
        for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
            if (antennaMask.isBitSet(antenna)) {
                calcGain[antenna] = glna[optIndex[antenna]][antenna] / 32; //LNA Mid Gain
                calcGainCtrl[antenna] = optIndex[antenna]; //tuning control
                midGains[antenna] = optIndex[antenna];
            }
        }

        m_client->setLnaMidGains(antennaMask, midGains);

        logOutput({ { "calcGain", toString(calcGain.begin(), calcGain.end()) }, { "calcGainCtrl", toString(calcGainCtrl.begin(), calcGainCtrl.end()) } });
    });
}

bool DutImpl::calculateRxLnaGains(const std::array<CorrelationResults_t, maxNumRxAntennas>& correlationResults, const std::array<float, maxNumRxAntennas>& prxin, int8_t pgc1, int8_t pgc2, int8_t pgc3, std::array<float, maxNumRxAntennas>& gains)
{
    return execute("calculateRxLnaGains", [this, correlationResults, prxin, pgc1, pgc2, pgc3, &gains]() {
        logInput({ { "correlationResults", toString(correlationResults.begin(), correlationResults.end()) }, { "prxin", toString(prxin.begin(), prxin.end()) }, { "pgc1", toString(pgc1) }, { "pgc2", toString(pgc2) }, { "pgc3", toString(pgc3) } });

        AntennaMask antennaMask = m_status->enabledRxAntennaMask.getValue();

        //gain is left shifted by 5, therefore multiply prxin, pgc1 and pgc2 by 32, calculate and divide result.
        int32_t pgc1_ = pgc1 * 32;
        int32_t pgc2_ = pgc2 * 32;

        std::array<float, maxNumRxAntennas> prxin_ {};
        for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
            prxin_[antenna] = prxin[antenna] * 32;
        }

        //(make sure to have 0.5Db accuracy. TODO- not processing gain before write to cal_wlan. See lnaGainsArray[i] = calData.ants[ant].lnaGains[i])

        for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
            if (antennaMask.isBitSet(antenna)) {
                float grx = correlationResults[antenna].II - prxin_[antenna];
                gains[antenna] = grx - pgc1_ - pgc2_ - pgc3;
                gains[antenna] /= 32; //SR 5
            }
        }

        logOutput({ { "gains", toString(gains.begin(), gains.end()) } });
    });
}

bool DutImpl::calculateRxLnaSubBandGains(std::array<std::array<uint16_t, maxNumLnaSubBands>, maxNumRxAntennas>& freqResultTable)
{
    return execute("calculateRxLnaSubBandGains", [this, &freqResultTable]() {
        uint16_t point1x;
        uint16_t point2x;
        int32_t point1y;
        int32_t point2y;
        int32_t point3y;
        int32_t point4y;

        AntennaMask antennaMask = m_status->enabledRxAntennaMask.getValue();

        if (m_lnaFrequencyIndex == ULONG_MAX) {
            throw std::logic_error("Invalid state: no data available yet");
        }

        if (m_lnaFrequencyIndex == 0) {
            throw std::logic_error("Invalid state: not enough data available yet");
        }

        std::array<std::array<uint8_t, maxNumRxAntennas>, maxNumLnaSubBands> maxValueIndex {};
        std::array<std::array<int32_t, maxNumRxAntennas>, maxNumLnaSubBands> maxValue {};
        // TODO: what is the value for 6G? 5760?
        const uint16_t DEFAULT_FREQ_RESULT = 4900;

        for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
            if (!antennaMask.isBitSet(antenna)) {
                continue;
            }
            for (size_t xp = 0; xp < maxNumLnaSubBands; xp++) {
                freqResultTable[antenna][xp] = DEFAULT_FREQ_RESULT;
            }
        }

        /*findMaxPoints*/
        for (size_t freqIndex = 0; freqIndex < m_lnaFrequencyIndex; freqIndex++) {
            for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
                maxValue[freqIndex][antenna] = -2000;
            }

            for (uint8_t subBandIndex = 0; subBandIndex < m_lastSubBandIndex; subBandIndex++) {
                for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
                    if (!antennaMask.isBitSet(antenna)) {
                        continue;
                    }
                    int32_t tempValue = m_lnaMeasureResult[freqIndex][subBandIndex][antenna];
                    if (tempValue > maxValue[freqIndex][antenna]) {
                        maxValue[freqIndex][antenna] = tempValue;
                        maxValueIndex[freqIndex][antenna] = subBandIndex;
                    }
                }
            }
        }

        /*interpolation calculation*/
        for (size_t freqIndex = 0; freqIndex < m_lnaFrequencyIndex; freqIndex++) {
            point1x = m_freqTableForLnaSubBand[freqIndex];
            point2x = m_freqTableForLnaSubBand[freqIndex + 1];

            for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
                uint8_t subBandIndex = maxValueIndex[freqIndex][antenna] + 1;
                if (subBandIndex < m_lastSubBandIndex) {
                    if (!antennaMask.isBitSet(antenna)) {
                        continue;
                    }
                    point1y = maxValue[freqIndex][antenna];
                    point2y = m_lnaMeasureResult[freqIndex][subBandIndex][antenna];
                    point3y = maxValue[(freqIndex + 1)][antenna];
                    point4y = m_lnaMeasureResult[(freqIndex + 1)][subBandIndex][antenna];

                    uint16_t frequency;
                    if ((point1y == point2y) && (point3y == point4y)) {
                        frequency = (point1x + point2x) / 2;
                    } else {
                        int32_t temp1 = point1y - point2y;
                        int32_t temp2 = point2x - point1x;
                        int32_t temp3 = point3y - point4y + point1y - point2y;

                        frequency = static_cast<uint16_t>((temp1 * temp2) / temp3 + point1x);
                    }

                    freqResultTable[antenna][maxValueIndex[freqIndex][antenna]] = frequency;
                }
            }
        }

        for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
            if (!antennaMask.isBitSet(antenna)) {
                continue;
            }
            for (size_t freqIndex = 0; freqIndex < m_lnaFrequencyIndex; freqIndex++) {
                /*in case the frequencies we chose don't give result in a certain sub-band*/
                if ((freqResultTable[antenna][freqIndex] == DEFAULT_FREQ_RESULT) && (freqIndex > 0) && (freqResultTable[antenna][(freqIndex - 1)] > DEFAULT_FREQ_RESULT) && (freqIndex < maxNumLnaFrequencies - 1)) {
                    freqResultTable[antenna][freqIndex] = (freqResultTable[antenna][(freqIndex - 1)] + freqResultTable[antenna][(freqIndex + 1)]) / 2;
                }
            }
        }

        /*extrapolation calculation*/
        point1x = m_freqTableForLnaSubBand[0];
        point2x = m_freqTableForLnaSubBand[1];

        for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
            if (!antennaMask.isBitSet(antenna)) {
                continue;
            }
            point1y = maxValue[0][antenna];
            point2y = m_lnaMeasureResult[0][(maxValueIndex[0][antenna] - 1)][antenna];
            point3y = m_lnaMeasureResult[1][maxValueIndex[0][antenna]][antenna];
            point4y = m_lnaMeasureResult[1][(maxValueIndex[0][antenna] - 1)][antenna];

            int32_t temp1 = point2y - point1y;
            int32_t temp2 = point2x - point1x;
            int32_t temp3 = point3y - point1y - point4y + point2y;

            uint16_t frequency;
            if (temp3 == 0) {
                frequency = DEFAULT_FREQ_RESULT;
            } else {
                frequency = static_cast<uint16_t>((temp1 * temp2) / temp3 + point1x);
            }

            if (frequency < DEFAULT_FREQ_RESULT) {
                frequency = DEFAULT_FREQ_RESULT;
            }
            if (frequency > point1x) {
                frequency = DEFAULT_FREQ_RESULT;
            }

            freqResultTable[antenna][maxValueIndex[0][antenna]] = frequency;
        }

        m_lnaFrequencyIndex = ULONG_MAX;

        logOutput({ { "freqResultTable[0][0]", toString(freqResultTable[0][0]) } });
    });
}

bool DutImpl::calculateRxPowerVector(int32_t maxPower, const std::array<float, maxNumRxAntennas>& lnaGains, int32_t lowerPowerBoundary, int32_t upperPowerBoundary, std::array<float, numPowerPoints>& powerVector)
{
    return execute("calculateRxPowerVector", [this, maxPower, lnaGains, lowerPowerBoundary, upperPowerBoundary, &powerVector]() {
        logInput({ { "maxPower", toString(maxPower) }, { "lnaGains", toString(lnaGains.begin(), lnaGains.end()) }, { "lowerPowerBoundary", toString(lowerPowerBoundary) }, { "upperPowerBoundary", toString(upperPowerBoundary) } });

        AntennaMask antennaMask = m_status->enabledRxAntennaMask.getValue();

        auto p1 = static_cast<float>(maxPower - upperPowerBoundary);
        auto p2 = static_cast<float>(maxPower - lowerPowerBoundary);

        float lnaGain = getAverageLnaGain(lnaGains, antennaMask);

        float gMin = std::pow(10.0f, p1 / 10);
        float gMax = std::pow(10.0f, p2 / 10);

        for (uint8_t i = 0; i < numPowerPoints; i++) {
            float tmp = std::round(10 * std::log10(gMin + i * (gMax - gMin) / (numPowerPoints - 1)));
            float power = tmp - roundf(lnaGain * 10.0f) / 10.0f;

            powerVector[i] = power;
        }

        logOutput({ { "powerVector", toString(powerVector.begin(), powerVector.end()) } });
    });
}

bool DutImpl::calculateRxRfFlatness(const std::array<std::array<int32_t, maxNumLnaSubBands>, maxNumRxAntennas>& P_Adc_array, const std::array<float, maxNumRxAntennas>& refGain, int8_t pgc1, int8_t pgc2, int8_t pgc3, const std::array<std::array<float, maxNumLnaSubBands>, maxNumRxAntennas>& pin, std::array<std::array<float, maxNumLnaSubBands>, maxNumRxAntennas>& results)
{
    return execute("calculateRxRfFlatness", [this, P_Adc_array, refGain, pgc1, pgc2, pgc3, pin, &results]() {
        logInput({ { "P_Adc_array[0][0]", toString(P_Adc_array[0][0]) }, { "refGain", toString(refGain.begin(), refGain.end()) }, { "pgc1", toString(pgc1) }, { "pgc2", toString(pgc2) }, { "pgc3", toString(pgc3) }, { "pin[0][0]", toString(pin[0][0]) } });

        for (uint8_t subBandIndex = 0; subBandIndex < maxNumLnaSubBands; subBandIndex++) {
            for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
                float pAdcArray = P_Adc_array[antenna][subBandIndex] / 32.0f;
                results[antenna][subBandIndex] = pAdcArray - pin[antenna][subBandIndex] - pgc1 - pgc2 - refGain[antenna];
            }
        }

        logOutput({ { "results[0][0]", toString(results[0][0]) } });
    });
}

bool DutImpl::getInbandRssi(std::array<int16_t, maxNumRxAntennas>& rssi)
{
    return execute("getInbandRssi", [this, &rssi]() {
        AntennaMask antennaMask = m_status->enabledRxAntennaMask.getValue();

        uint16_t numSamples;
        m_client->getInbandRssi(antennaMask, rssi, numSamples);

        logOutput({ { "rssi", toString(rssi.begin(), rssi.end()) }, { "numSamples", toString(numSamples) } });
    });
}

bool DutImpl::getRfRssiPower(std::array<int8_t, maxNumRxAntennas>& power)
{
    return execute("getRfRssiPower", [this, &power]() {
        AntennaMask antennaMask = m_status->enabledRxAntennaMask.getValue();

        m_client->getRfRssiPower(antennaMask, power);

        logOutput({ { "power", toString(power.begin(), power.end()) } });
    });
}

bool DutImpl::getRssi(uint8_t method, uint16_t numSamples, std::array<uint16_t, maxNumRxAntennas>& rssi)
{
    return execute("getRssi", [this, method, numSamples, &rssi]() {
        logInput({ { "method", toString(method) }, { "numSamples", toString(numSamples) } });

        AntennaMask antennaMask = m_status->enabledRxAntennaMask.getValue();

        m_client->getRssi(method, numSamples, antennaMask, rssi);

        logOutput({ { "rssi", toString(rssi.begin(), rssi.end()) } });
    });
}

bool DutImpl::getRxCwPower(uint8_t method, int32_t freqOffset, uint16_t numSamples, std::array<CorrelationResults_t, maxNumRxAntennas>& correlationResults)
{
    return execute("getRxCwPower", [this, method, freqOffset, numSamples, &correlationResults]() {
        logInput({ { "method", toString(method) }, { "freqOffset", toString(freqOffset) }, { "numSamples", toString(numSamples) } });

        if (!m_status->rxCalibrationRunning.getValue()) {
            throw std::runtime_error("Invalid state: RX calibration not started");
        }

        AntennaMask antennaMask = m_status->enabledRxAntennaMask.getValue();

        m_client->getRxCwPower(method, freqOffset, numSamples, antennaMask, correlationResults);

        logOutput({ { "correlationResults", toString(correlationResults.begin(), correlationResults.end()) } });
    });
}

bool DutImpl::getRxEvm(std::array<uint8_t, maxNumRxAntennas>& rxEvm)
{
    return execute("getRxEvm", [this, &rxEvm]() {
        m_client->getRxEvm(rxEvm);

        logOutput({ { "rxEvm", toString(rxEvm.begin(), rxEvm.end()) } });
    });
}

bool DutImpl::measureRxLnaSubBandGains()
{
    return execute("measureRxLnaSubBandGains", [this]() {
        if (!m_status->rxCalibrationRunning.getValue()) {
            throw std::runtime_error("Invalid state: RX calibration not started");
        }

        if (m_lnaFrequencyIndex == ULONG_MAX) {
            m_lnaFrequencyIndex = 0;
            m_lnaMeasureResult = {};
        } else {
            m_lnaFrequencyIndex++;
        }

        if (m_lnaFrequencyIndex == maxNumLnaFrequencies) {
            throw std::logic_error("Invalid state: exceeded the maximum number of frequencies allowed (" + toString(maxNumLnaFrequencies) + ")");
        }

        if (Band::BAND_6000MHZ == m_status->band.getValue()) {

            m_freqTableForLnaSubBand[m_lnaFrequencyIndex] = m_status->lowestChannel.getValue() * 5 + 5950;
            m_lastSubBandIndex = maxNumLnaSubBands;

        } else {
            m_freqTableForLnaSubBand[m_lnaFrequencyIndex] = m_status->lowestChannel.getValue() * 5 + 5000;
            m_lastSubBandIndex = 8;

            if (m_status->chipId.getValue() >= ChipID::CHIP_ID_GEN6_D2) {
                m_lastSubBandIndex = maxNumLnaSubBands;
            }
        }

        AntennaMask antennaMask = m_status->enabledRxAntennaMask.getValue();

        for (uint8_t subBandIndex = 0; subBandIndex < m_lastSubBandIndex; subBandIndex++) {
            m_client->setRxBand(subBandIndex);

            constexpr uint8_t method = 0;
            constexpr int32_t freqOffset = 0;
            constexpr uint16_t numSamples = 1024;
            std::array<CorrelationResults_t, maxNumRxAntennas> correlationResults;
            m_client->getRxCwPower(method, freqOffset, numSamples, antennaMask, correlationResults);

            for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
                m_lnaMeasureResult[m_lnaFrequencyIndex][subBandIndex][antenna] = correlationResults[antenna].II;
            }
        }
    });
}

bool DutImpl::setRxGainBlocks(uint8_t lnaIndex, int8_t pgc1, int8_t pgc2, int8_t pgc3)
{
    return execute("setRxGainBlocks", [this, lnaIndex, pgc1, pgc2, pgc3]() {
        logInput({ { "lnaIndex", toString(lnaIndex) }, { "pgc1", toString(pgc1) }, { "pgc2", toString(pgc2) }, { "pgc3", toString(pgc3) } });

        AntennaMask antennaMask = m_status->enabledRxAntennaMask.getValue();

        m_client->setRxGainBlocks(antennaMask, lnaIndex, pgc1, pgc2, pgc3);
    });
}

bool DutImpl::setRssiS2dInitials(uint8_t auxAdcRes, float ioffsStep)
{
    return execute("setRssiS2dInitials", [this, auxAdcRes, ioffsStep]() {
        logInput({ { "auxAdcRes", toString(auxAdcRes) }, { "ioffsStep", toString(ioffsStep) } });

        m_rssiS2dParams.vmax = (float)std::pow(2, auxAdcRes); //2^auxAdcRes;       // Vmax = 512
        m_rssiS2dParams.vmid = m_rssiS2dParams.vmax / 2; // Vmid = 256
        m_rssiS2dParams.m = (int)(1 / ioffsStep); // M = 16
        m_rssiS2dParams.vp = (int)(m_rssiS2dParams.vmax / 2); // Vp = 256
    });
}

bool DutImpl::setRssiS2dParams(const AntennaMask& antennaMask, uint8_t region, uint8_t gain, uint8_t offset)
{
    return execute("setRssiS2dParams", [this, antennaMask, region, gain, offset]() {
        logInput({ { "antennaMask", toHexString(antennaMask) }, { "region", toString(region) }, { "gain", toString(gain) }, { "offset", toString(offset) } });

        if (!antennaMask.isValid(m_status->availableRxAntennaMask.getValue())) {
            throw std::invalid_argument("Invalid antenna mask " + toHexString(antennaMask) + ", available RX antenna mask is " + toHexString(m_status->availableRxAntennaMask.getValue()));
        }

        checkRxRegion(region, m_status->chipId.getValue());

        m_client->setRssiS2dParams(antennaMask, region, gain, offset);
    });
}

bool DutImpl::setRxBandLut(const AntennaMask& antennaMask, const std::array<std::array<uint16_t, maxNumLnaSubBandsCrossingPoints>, maxNumRxAntennas>& rxBandLut, const std::array<uint8_t, maxNumRxAntennas>& firstTunningIndex)
{
    return execute("setRxBandLut", [this, antennaMask, rxBandLut, firstTunningIndex]() {
        logInput({ { "antennaMask", toHexString(antennaMask) }, { "rxBandLut[0][0]", toString(rxBandLut[0][0]) }, { "firstTunningIndex", toString(firstTunningIndex.begin(), firstTunningIndex.end()) } });

        m_client->setRxBandLut(antennaMask, rxBandLut, firstTunningIndex);
    });
}

bool DutImpl::startRxCalibration()
{
    return execute("startRxCalibration", [this]() {
        if (!m_status->lowestChannel.isValueSet()) {
            throw std::logic_error("Channel not set");
        }

        AntennaMask antennaMask = m_status->enabledRxAntennaMask.getValue();

        if (antennaMask.bitCount() == 0) {
            throw std::logic_error("Illegal antenna selection (at least one RX antenna must be selected)");
        }

        startRxCalibrationImpl();
    });
}

void DutImpl::startRxCalibrationImpl() const
{
    if ((!m_status->rxCalibrationRunning.isValueSet()) || (!m_status->rxCalibrationRunning.getValue())) {
        m_client->setRiscModeEnabled(false);
        m_client->startRxCalibration();
        m_status->rxCalibrationRunning.setValue(true);
    }
}

bool DutImpl::stopRxCalibration()
{
    return execute("stopRxCalibration", [this]() {
        stopRxCalibrationImpl();
    });
}

void DutImpl::stopRxCalibrationImpl() const
{
    if (m_status->rxCalibrationRunning.isValueSet() && m_status->rxCalibrationRunning.getValue()) {
        m_client->stopRxCalibration();
        m_client->setRiscModeEnabled(true);
        m_status->rxCalibrationRunning.setValue(false);
    }
}

}
