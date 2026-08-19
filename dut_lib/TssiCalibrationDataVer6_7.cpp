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

#include "TssiCalibrationDataVer6_7.h"

#include "Bits.h"
#include "ResizableBuffer.h"
#include "dut/Channels.h"
#include "dut/Tools.h"

namespace dut {

constexpr uint8_t antennaMaskBitPosition = 0;
constexpr uint8_t antennaMaskBitMask = 0x1f;

constexpr uint8_t numRegionsBitPosition = 6;
constexpr uint8_t numRegionsBitMask = 0x03;

constexpr uint8_t channelNumberMsbBitMask = 0x07;
constexpr uint8_t channelNumberMsbBandBitOffset = 0x08;

constexpr uint8_t bandwidthBitPosition = 4;
constexpr uint8_t bandwidthBitMask = 0x0F;

static Band getTssiBand(uint8_t channelMSB, int channel)
{
    Band band;
    bool is_band_6G = ((channelMSB & channelNumberMsbBandBitOffset) == channelNumberMsbBandBitOffset);
    if (is_band_6G) {
        band = Band::BAND_6000MHZ;
    } else /*band 2.4G or 5G*/
    {
        if (channel <= Channels::maxChannel2400MHz) {
            band = Band::BAND_2400MHZ;
        } else {
            band = Band::BAND_5000MHZ;
        }
    }

    return band;
}

void TssiCalibrationDataVer6_7::serializeAntennaData(uint8_t antenna, std::shared_ptr<ResizableBuffer> buffer)
{
    auto& data = this->data();

    buffer->append(data.antennas[antenna].maxPower);
    buffer->append(data.antennas[antenna].uEvm);
    buffer->append(data.antennas[antenna].uEvmGain);

    if (data.numRegions > 0) {
        uint8_t value;

        if (data.numRegions == 1) {
            value = MAKE_BYTE(0, data.antennas[antenna].s2dGain[0]);
        } else {
            value = MAKE_BYTE(data.antennas[antenna].s2dGain[1], data.antennas[antenna].s2dGain[0]);
        }

        buffer->append(value);

        if (data.numRegions == 3) {
            value = MAKE_BYTE(0, data.antennas[antenna].s2dGain[2]);
            buffer->append(value);
        }

        for (uint8_t region = 0; region < data.numRegions; region++) {
            buffer->append(static_cast<uint8_t>(data.antennas[antenna].s2dOffset[region]));
        }

        for (uint8_t region = 0; region < data.numRegions; region++) {
            buffer->append(LOWER_BYTE(data.antennas[antenna].pointsAB[region].a));
            buffer->append(UPPER_BYTE(data.antennas[antenna].pointsAB[region].a));
            buffer->append(LOWER_BYTE(data.antennas[antenna].pointsAB[region].b));
            buffer->append(UPPER_BYTE(data.antennas[antenna].pointsAB[region].b));
        }
    }
}

void TssiCalibrationDataVer6_7::serializeAntennaParams(uint8_t antenna, std::shared_ptr<ResizableBuffer> buffer)
{
    uint8_t value;

    const auto& data = this->data();

    buffer->append(data.channel & 0xff);

    value = ((data.channel >> 8) & channelNumberMsbBitMask);
    value |= ((static_cast<uint8_t>(data.bw) & bandwidthBitMask) << bandwidthBitPosition);
    buffer->append(value);

    serializeAntennaData(antenna, buffer);
}

void TssiCalibrationDataVer6_7::serialize(std::shared_ptr<ResizableBuffer> buffer)
{
    uint8_t value;

    const auto& data = this->data();

    if (data.antennaMask > antennaMaskBitMask) {
        throw std::invalid_argument("Antenna mask cannot be larger than " + toHexString(antennaMaskBitMask));
    }

    if (data.numRegions > maxNumTxRegions) {
        throw std::invalid_argument("Number of regions cannot be larger than " + toString(maxNumTxRegions));
    }

    if (data.channel == 0) {
        throw std::invalid_argument("Channel number cannot be 0");
    }

    if (buffer->length() == 0) {
        // Warning: this layout allows only for a maximum of 5 TX antennas (5 bits) and 3 regions (3 bits)
        value = (data.antennaMask & antennaMaskBitMask) << antennaMaskBitPosition;
        value |= (data.numRegions & numRegionsBitMask) << numRegionsBitPosition;
        buffer->append(value);
    }

    buffer->append(data.channel & 0xff);

    value = ((data.channel >> 8) & channelNumberMsbBitMask);
    value |= ((static_cast<uint8_t>(data.bw) & bandwidthBitMask) << bandwidthBitPosition);
    if (data.band == Band::BAND_6000MHZ) {
        value |= channelNumberMsbBandBitOffset;
    }
    buffer->append(value);

    // Add each antenna calibration data
    for (uint8_t antenna = 0; antenna < maxNumTxAntennas; antenna++) {
        AntennaMask antennaMask = data.antennaMask;
        if (antennaMask.isBitSet(antenna)) {
            serializeAntennaData(antenna, buffer);
        }
    }
}

void TssiCalibrationDataVer6_7::parse(const uint8_t* buffer, size_t size, size_t& offset)
{
    if (!buffer) {
        throw std::invalid_argument("Parameter 'buffer' cannot be a null pointer");
    }

    uint8_t value;
    size_t commonDataOffset = 0;

    auto& data = this->data();

    parseField(&value, buffer, size, commonDataOffset, sizeof(uint8_t));
    data.antennaMask = (value >> antennaMaskBitPosition) & antennaMaskBitMask;
    data.numRegions = (value >> numRegionsBitPosition) & numRegionsBitMask;

    parseField(&value, buffer, size, offset, sizeof(uint8_t));
    data.channel = value;

    parseField(&value, buffer, size, offset, sizeof(uint8_t));
    data.channel |= (value & channelNumberMsbBitMask) << 8;
    data.bw = static_cast<Bandwidth>((value >> bandwidthBitPosition) & bandwidthBitMask);
    data.band = getTssiBand(value, data.channel);

    for (uint8_t antenna = 0; antenna < maxNumTxAntennas; antenna++) {
        AntennaMask antennaMask = data.antennaMask;
        if (antennaMask.isBitSet(antenna)) {
            parseField(&data.antennas[antenna].maxPower, buffer, size, offset, sizeof(uint8_t));
            parseField(&data.antennas[antenna].uEvm, buffer, size, offset, sizeof(uint8_t));
            parseField(&data.antennas[antenna].uEvmGain, buffer, size, offset, sizeof(uint8_t));

            if (data.numRegions > 0) {
                parseField(&value, buffer, size, offset, sizeof(uint8_t));
                data.antennas[antenna].s2dGain[0] = LOWER_NIBBLE(value);
                if (data.numRegions > 1) {
                    data.antennas[antenna].s2dGain[1] = UPPER_NIBBLE(value);
                }

                if (data.numRegions == 3) {
                    parseField(&value, buffer, size, offset, sizeof(uint8_t));
                    data.antennas[antenna].s2dGain[2] = LOWER_NIBBLE(value);
                }

                for (uint8_t region = 0; region < data.numRegions; region++) {
                    parseField(&data.antennas[antenna].s2dOffset[region], buffer, size, offset, sizeof(uint8_t));
                }

                for (uint8_t region = 0; region < data.numRegions; region++) {
                    parseField(&data.antennas[antenna].pointsAB[region].a, buffer, size, offset, sizeof(uint16_t));
                    parseField(&data.antennas[antenna].pointsAB[region].b, buffer, size, offset, sizeof(uint16_t));
                }
            }
        }
    }
}

}