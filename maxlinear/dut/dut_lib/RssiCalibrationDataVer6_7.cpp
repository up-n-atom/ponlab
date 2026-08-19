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

#include "RssiCalibrationDataVer6_7.h"

#include "Bits.h"
#include "ResizableBuffer.h"
#include "dut/Tools.h"

namespace dut {

void RssiCalibrationDataVer6_7::serializeCommonParameters(std::shared_ptr<ResizableBuffer> buffer)
{
    uint8_t value;
    std::vector<uint8_t> tmpByteBuf;

    RssiCalibrationData_t data = this->data();

    buffer->append(&data.antennaMask, AntennaMask::size());
    buffer->append(&data.startFreq, sizeof(data.startFreq));
    buffer->append(&data.stopFreq, sizeof(data.stopFreq));
    buffer->append(&data.calibrationFreq, sizeof(data.calibrationFreq));

    value = data.chipTemperature & 0x7F;
    if (data.subBandsCrossingPoints) {
        value |= 0x80;
    }
    buffer->append(value);

    value = MAKE_BYTE(data.numRxRssiFlatnessPoints, data.numRxRfFlatnessPoints);
    buffer->append(value);

    if (data.numRxRfFlatnessPoints > maxRxRfFlatnessPoints) {
        throw std::invalid_argument("Value in field 'numRxRfFlatnessPoints' (" + toString(data.numRxRfFlatnessPoints) + ") exceeds maximum allowed value (" + toString(dut::maxRxRfFlatnessPoints) + ")");
    }

    if (data.numRxRfFlatnessPoints > 0) {
        tmpByteBuf = convertValuesToDeltas(data.rxRfFlatnessFreqs, data.numRxRfFlatnessPoints, data.startFreq, rxFlatnessFreqStep);
        tmpByteBuf = setNibbleArray(tmpByteBuf.data(), tmpByteBuf.size());
        buffer->append(tmpByteBuf.data(), tmpByteBuf.size());
    }

    if (data.numRxRssiFlatnessPoints > maxRxRssiFlatnessPoints) {
        throw std::invalid_argument("Value in field 'numRxRfFlatnessPoints' (" + toString(data.numRxRssiFlatnessPoints) + ") exceeds maximum allowed value (" + toString(maxRxRssiFlatnessPoints) + ")");
    }

    if (data.numRxRssiFlatnessPoints > 0) {
        tmpByteBuf = convertValuesToDeltas(data.rxRssiFlatnessFreqs, data.numRxRssiFlatnessPoints, data.startFreq, rxFlatnessFreqStep);
        tmpByteBuf = setNibbleArray(tmpByteBuf.data(), tmpByteBuf.size());
        buffer->append(tmpByteBuf.data(), tmpByteBuf.size());
    }
}

void RssiCalibrationDataVer6_7::serialize(std::shared_ptr<ResizableBuffer> buffer)
{
    serializeCommonParameters(buffer);

    // Add each antenna calibration data
    for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
        AntennaMask antennaMask = data().antennaMask;
        if (antennaMask.isBitSet(antenna)) {
            serializeAntennaParameters(antenna, buffer);
        }
    }
}

void RssiCalibrationDataVer6_7::parseCommonParameters(const uint8_t* buffer, size_t size, size_t& offset)
{
    uint8_t value;
    std::vector<uint8_t> tmpByteBuf;
    std::vector<uint16_t> tmpShortBuf;

    auto& data = this->data();

    parseField(&data.antennaMask, buffer, size, offset, AntennaMask::size());
    parseField(&data.startFreq, buffer, size, offset, sizeof(data.startFreq));
    parseField(&data.stopFreq, buffer, size, offset, sizeof(data.stopFreq));
    parseField(&data.calibrationFreq, buffer, size, offset, sizeof(data.calibrationFreq));

    parseField(&value, buffer, size, offset, sizeof(uint8_t));
    data.chipTemperature = value & 0x7F;
    data.subBandsCrossingPoints = (value & 0x80) > 0;

    parseField(&value, buffer, size, offset, sizeof(uint8_t));
    data.numRxRfFlatnessPoints = LOWER_NIBBLE(value);
    data.numRxRssiFlatnessPoints = UPPER_NIBBLE(value);

    if (data.numRxRfFlatnessPoints > maxRxRfFlatnessPoints) {
        throw std::invalid_argument("Value in field 'numRxRfFlatnessPoints' (" + toString(data.numRxRfFlatnessPoints) + ") exceeds maximum allowed value (" + toString(maxRxRfFlatnessPoints) + ")");
    }

    if (data.numRxRfFlatnessPoints > 0) {
        // Parse Rx RF flatness freq points - 2 cal points in each byte
        tmpByteBuf = getNibbleArray(buffer, size, offset, data.numRxRfFlatnessPoints);
        tmpShortBuf = convertDeltasToValues(tmpByteBuf.data(), tmpByteBuf.size(), data.startFreq, rxFlatnessFreqStep);
        for (size_t i = 0; i < maxRxRfFlatnessPoints; i++) {
            data.rxRfFlatnessFreqs[i] = (i < data.numRxRfFlatnessPoints) ? tmpShortBuf[i] : 0;
        }
    }

    if (data.numRxRssiFlatnessPoints > maxRxRssiFlatnessPoints) {
        throw std::invalid_argument("Value in field 'numRxRfFlatnessPoints' (" + toString(data.numRxRssiFlatnessPoints) + ") exceeds maximum allowed value (" + toString(maxRxRssiFlatnessPoints) + ")");
    }

    if (data.numRxRssiFlatnessPoints > 0) {
        // Parse Rx RSSI flatness freq points - 2 cal points in each byte
        tmpByteBuf = getNibbleArray(buffer, size, offset, data.numRxRssiFlatnessPoints);
        tmpShortBuf = convertDeltasToValues(tmpByteBuf.data(), tmpByteBuf.size(), data.startFreq, rxFlatnessFreqStep);
        for (size_t i = 0; i < maxRxRssiFlatnessPoints; i++) {
            data.rxRssiFlatnessFreqs[i] = (i < data.numRxRssiFlatnessPoints) ? tmpShortBuf[i] : 0;
        }
    }
}

void RssiCalibrationDataVer6_7::parse(const uint8_t* buffer, size_t size, size_t& offset)
{
    if (!buffer) {
        throw std::invalid_argument("Parameter 'buffer' cannot be a null pointer");
    }

    parseCommonParameters(buffer, size, offset);

    // Parse each antenna calibration data
    for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
        AntennaMask antennaMask = data().antennaMask;
        if (antennaMask.isBitSet(antenna)) {
            parseAntennaParameters(antenna, buffer, size, offset);
        }
    }
}

}