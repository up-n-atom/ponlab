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

#include "RssiCalibrationDataVer6.h"

#include "Bits.h"
#include "ResizableBuffer.h"
#include "dut/Tools.h"
#ifdef LINUX_HOST
#include <cmath>
#endif

namespace dut {

void RssiCalibrationDataVer6::serialize(std::shared_ptr<ResizableBuffer> buffer)
{
    auto getSubBandsCrossingPoints = [this]() {
        for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
            AntennaMask antennaMask = data().antennaMask;
            if (!antennaMask.isBitSet(antenna)) {
                continue;
            }

            const auto& antennaData = data().antennaData.antennasVer6[antenna];
            for (const auto& value : antennaData.subBandsCrossingPoints) {
                if (value) {
                    return true;
                }
            }
        }

        return false;
    };

    data().subBandsCrossingPoints = getSubBandsCrossingPoints();

    RssiCalibrationDataVer6_7::serialize(buffer);
}

void RssiCalibrationDataVer6::serializeAntennaParameters(uint8_t antenna, std::shared_ptr<ResizableBuffer> buffer)
{
    uint8_t value;
    std::vector<uint8_t> tmpByteBuf;
    std::vector<int8_t> clippedFlatnessPoints;

    const auto& data = this->data();
    const auto& antennaData = data.antennaData.antennasVer6[antenna];

    if (data.subBandsCrossingPoints) {
        tmpByteBuf = convertValuesToDeltas(antennaData.subBandsCrossingPoints, maxNumLnaSubBandsCrossingPointsVer6, data.startFreq, rxSubBandsFreqStep);
        tmpByteBuf = setNibbleArray(tmpByteBuf.data(), tmpByteBuf.size());
        tmpByteBuf[tmpByteBuf.size() - 1] |= (antennaData.firstCrossingPointIndex << 4);
        buffer->append(tmpByteBuf.data(), tmpByteBuf.size());
    }

    value = antennaData.lnaMidGain;
    buffer->append(value);

    for (size_t index = 0; index < maxNumLnaGainStepsVer6; index++) {
        value = static_cast<uint8_t>(lq_round(antennaData.lnaGains[index] * 2.0));
        buffer->append(value);
    }

    if (data.numRxRfFlatnessPoints > 0) {
        tmpByteBuf = setNibbleArray(reinterpret_cast<const uint8_t*>(antennaData.midGainDeltaPoints), data.numRxRfFlatnessPoints);
        buffer->append(tmpByteBuf.data(), tmpByteBuf.size());

        clippedFlatnessPoints = clipValuesTo4Bit(antennaData.rxRfFlatnessDeltaPointsHighGain, data.numRxRfFlatnessPoints);
        tmpByteBuf = setNibbleArray(reinterpret_cast<const uint8_t*>(clippedFlatnessPoints.data()), clippedFlatnessPoints.size());
        buffer->append(tmpByteBuf.data(), tmpByteBuf.size());

        clippedFlatnessPoints = clipValuesTo4Bit(antennaData.rxRfFlatnessDeltaPointsBypass, data.numRxRfFlatnessPoints);
        tmpByteBuf = setNibbleArray(reinterpret_cast<const uint8_t*>(clippedFlatnessPoints.data()), clippedFlatnessPoints.size());
        buffer->append(tmpByteBuf.data(), tmpByteBuf.size());
    }

    for (uint8_t region = 0; region < maxNumRxRegionsVer6; region++) {
        value = antennaData.s2dGainOffset[region].offset;
        buffer->append(value);

        value = static_cast<uint8_t>(std::round((double)antennaData.s2dGainOffset[region].gain - (-10)) / 2);
        buffer->append(value);
    }

    for (uint8_t region = 0; region < maxNumRxRegionsVer6; region++) {
        value = static_cast<uint8_t>(std::round((antennaData.pointsAB[region].a - 1.5f) * 256));
        buffer->append(value);

        value = static_cast<uint8_t>(std::round(antennaData.pointsAB[region].b * -4));
        buffer->append(value);
    }

    if (data.numRxRssiFlatnessPoints > 0) {
        clippedFlatnessPoints = clipValuesTo4Bit(antennaData.rxRssiFlatnessDeltaPoints, data.numRxRssiFlatnessPoints);
        tmpByteBuf = setNibbleArray(reinterpret_cast<const uint8_t*>(clippedFlatnessPoints.data()), clippedFlatnessPoints.size());
        buffer->append(tmpByteBuf.data(), tmpByteBuf.size());
    }
}

void RssiCalibrationDataVer6::parseAntennaParameters(uint8_t antenna, const uint8_t* buffer, size_t size, size_t& offset)
{
    uint8_t value;
    std::vector<uint8_t> tmpByteBuf;
    std::vector<uint16_t> tmpShortBuf;

    auto& data = this->data();
    auto& antennaData = data.antennaData.antennasVer6[antenna];

    if (data.subBandsCrossingPoints) {
        // Parse LNA sub-bands crossing points
        tmpByteBuf = getNibbleArray(buffer, size, offset, maxNumLnaSubBandsCrossingPointsVer6);
        tmpShortBuf = convertDeltasToValues(tmpByteBuf.data(), tmpByteBuf.size(), data.startFreq, rxSubBandsFreqStep);
        for (size_t i = 0; i < maxNumLnaSubBandsCrossingPointsVer6; i++) {
            antennaData.subBandsCrossingPoints[i] = tmpShortBuf[i];
        }
        antennaData.firstCrossingPointIndex = UPPER_NIBBLE(buffer[offset - 1]);
    }

    // Parse LNA mid Gain tuning word
    parseField(&value, buffer, size, offset, sizeof(uint8_t));
    antennaData.lnaMidGain = value;

    // Parse LNA Gain
    for (size_t index = 0; index < maxNumLnaGainStepsVer6; index++) {
        parseField(&value, buffer, size, offset, sizeof(uint8_t));
        auto x = static_cast<float>((value > UINT8_MAX / 2) ? (value - UINT8_MAX - 1) : value);
        antennaData.lnaGains[index] = x / 2.0f;
    }

    if (data.numRxRfFlatnessPoints > 0) {
        // Parse mid gain delta points
        parseDeltaPoints(buffer, size, offset, data.numRxRfFlatnessPoints, antennaData.midGainDeltaPoints);

        // Parse flatness calibration delta points (for eLNA high gain)
        parseDeltaPoints(buffer, size, offset, data.numRxRfFlatnessPoints, antennaData.rxRfFlatnessDeltaPointsHighGain);

        // Parse flatness calibration delta points (for eLNA bypass mode)
        parseDeltaPoints(buffer, size, offset, data.numRxRfFlatnessPoints, antennaData.rxRfFlatnessDeltaPointsBypass);
    }

    // Parse S2D Offset and Gain per regions
    for (int region = 0; region < maxNumRxRegionsVer6; region++) {
        parseField(&value, buffer, size, offset, sizeof(uint8_t));
        antennaData.s2dGainOffset[region].offset = value;

        // TODO: when serializing, gain is added to 10 and divided by 2. Shouldn't we do the reverse here when parsing?
        parseField(&value, buffer, size, offset, sizeof(uint8_t));
        antennaData.s2dGainOffset[region].gain = value;
    }

    // Parse linear fit slope & offset per region
    for (int region = 0; region < maxNumRxRegionsVer6; region++) {
        // TODO: both a and b are of type float and here are serialized as bytes. Is this right? (possible loss of data)
        // TODO: when serializing, some computation is performed around A and B values. Shouldn't we do the reverse here when parsing?
        parseField(&value, buffer, size, offset, sizeof(uint8_t));
        antennaData.pointsAB[region].a = value;

        parseField(&value, buffer, size, offset, sizeof(uint8_t));
        antennaData.pointsAB[region].b = value;
    }

    if (data.numRxRssiFlatnessPoints > 0) {
        // Parse RSSI flatness calibration delta points
        parseDeltaPoints(buffer, size, offset, data.numRxRssiFlatnessPoints, antennaData.rxRssiFlatnessDeltaPoints);
    }
}

}
