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

#include "dut/Channels.h"
#include "dut/Tools.h"

#include <stdexcept>
#include <string>

#ifdef LINUX_HOST
#include <algorithm>
#endif

// See "List of WLAN channels" in Wikipedia: https://en.wikipedia.org/wiki/List_of_WLAN_channels

namespace dut {
namespace Channels {

    // Channel 14 is valid only for DSSS and CCK modes (Clause 18 a.k.a. 802.11b) in Japan.
    // OFDM (i.e., 802.11g) may not be used. (IEEE 802.11-2007 §19.4.2)
    constexpr uint8_t channel14 = 14;
    constexpr uint16_t centerFrequencyFor20MHzChannel14In2400MHz = 2484;

    void checkPhyMode(PhyMode phyMode, Band band)
    {
        switch (band) {
        case Band::BAND_2400MHZ:
            // 802.11ac (VHT) does not support the 2.4 GHz frequency band.
            // However, we enabled this mode long time ago since our competitors use it for higher
            // throughput.
            if ((phyMode != PhyMode::PHY_MODE_B) && (phyMode != PhyMode::PHY_MODE_G) && (phyMode != PhyMode::PHY_MODE_N_2_4) && (phyMode != PhyMode::PHY_MODE_AC) && (phyMode != PhyMode::PHY_MODE_AX) && (phyMode != PhyMode::PHY_MODE_BE)) {
                throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ") for 2.4GHz band. Valid values are B, G, N-2.4GHz, AC, AX and BE");
            }
            break;
        case Band::BAND_5000MHZ:
            if ((phyMode != PhyMode::PHY_MODE_A) && (phyMode != PhyMode::PHY_MODE_N_5) && (phyMode != PhyMode::PHY_MODE_AC) && (phyMode != PhyMode::PHY_MODE_AX) && (phyMode != PhyMode::PHY_MODE_BE)) {
                throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ") for 5GHz band. Valid values are A, N-5GHz, AC, AX and BE");
            }
            break;
        case Band::BAND_6000MHZ:
            if ((phyMode != PhyMode::PHY_MODE_AX) && (phyMode != PhyMode::PHY_MODE_BE)) {
                throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ") for 6GHz band. Valid values are AX and BE only");
            }
            break;
        default:
            break;
        }
    }

    uint16_t getChannelSize(Band band, Bandwidth bandwidth)
    {
        switch (bandwidth) {
        case Bandwidth::BANDWIDTH_TWENTY:
            if (band == Band::BAND_2400MHZ) {
                return 22;
            } else {
                return 20;
            }
        case Bandwidth::BANDWIDTH_FOURTY:
            if (band == Band::BAND_2400MHZ) {
                return 42;
            } else {
                return 40;
            }
        case Bandwidth::BANDWIDTH_EIGHTY:
            return 80;
        case Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY:
            return 160;
        case Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY:
            return 320;
        default:
            throw std::invalid_argument("Invalid bandwidth (" + toString(bandwidth) + ")");
        }
    }

    uint16_t getStartFrequency(Band band)
    {
        switch (band) {
        case Band::BAND_2400MHZ:
            return startFrequency2400MHz;
        case Band::BAND_5000MHZ:
            return startFrequency5000MHz;
        case Band::BAND_6000MHZ:
            return startFrequency6000MHz;
        default:
            throw std::invalid_argument("Invalid band (" + toString(band) + ")");
        }
    }

    std::vector<uint8_t> get20MhzChannelsIn2400MHzBand(PhyMode phyMode)
    {
        std::vector<uint8_t> channels;

        for (uint8_t channel = minChannel2400MHz; channel <= maxChannel2400MHz; channel++) {
            // Channel 14 is valid only for DSSS and CCK modes (Clause 18 a.k.a. 802.11b)
            if ((channel == channel14) && (phyMode != PhyMode::PHY_MODE_B)) {
                continue;
            }
            channels.push_back(channel);
        }

        return channels;
    }

    std::vector<uint8_t> get20MhzChannelsIn5000MHzBand()
    {
        std::vector<uint8_t> channels;

        for (uint8_t channel = minChannel5000MHz; channel <= 144; channel += 2) {
            // Do not add channels in the gap between U-NII-2A and U-NII-2C
            if ((channel > 64) && (channel < 100) && (channel != 68) && (channel != 96)) {
                continue;
            }
            channels.push_back(channel);
        }
        for (uint8_t channel = 149; channel <= maxChannel5000MHz; channel += 2) {
            channels.push_back(channel);
        }

        return channels;
    }

    std::vector<uint8_t> get20MhzChannelsIn6000MHzBand()
    {
        std::vector<uint8_t> channels;

        for (uint8_t channel = minChannel6000MHz; channel <= maxChannel6000MHz; channel += 2) {
            channels.push_back(channel);
        }

        return channels;
    }

    std::vector<uint8_t> get20MhzBondedChannelsIn2400MHzBand(uint8_t lowestChannel, uint16_t channelSize)
    {
        std::vector<uint8_t> channels;

        auto lastBondedChannel = static_cast<uint8_t>(lowestChannel + channelSize / 10);

        for (uint8_t i = lowestChannel + 1; i <= lastBondedChannel; i++) {
            channels.push_back(i);
        }

        return channels;
    }

    std::vector<uint8_t> get20MhzBondedChannelsIn5000MHzBand(uint8_t lowestChannel, uint16_t channelSize)
    {
        std::vector<uint8_t> channels;

        auto lastBondedChannel = static_cast<uint8_t>(lowestChannel + channelSize / 5 - 4);

        for (uint8_t i = lowestChannel + 2; i <= lastBondedChannel; i += 2) {
            channels.push_back(i);
        }

        return channels;
    }

    std::vector<uint8_t> get20MhzBondedChannelsIn6000MHzBand(uint8_t lowestChannel, uint16_t channelSize)
    {
        std::vector<uint8_t> channels;

        auto lastBondedChannel = static_cast<uint8_t>(lowestChannel + channelSize / 5 - 4);

        // The type of the loop variable cannot be uint8_t because i+4 can be greater than
        // UINT8_MAX in this case
        for (uint32_t i = lowestChannel + 4; i <= lastBondedChannel; i += 4) {
            channels.push_back(static_cast<uint8_t>(i));
        }

        return channels;
    }

    std::vector<uint8_t> get20MhzBondedChannels(uint8_t lowestChannel, uint16_t channelSize, Band band)
    {
        switch (band) {
        case Band::BAND_2400MHZ:
            return get20MhzBondedChannelsIn2400MHzBand(lowestChannel, channelSize);
        case Band::BAND_5000MHZ:
            return get20MhzBondedChannelsIn5000MHzBand(lowestChannel, channelSize);
        case Band::BAND_6000MHZ:
            return get20MhzBondedChannelsIn6000MHzBand(lowestChannel, channelSize);
        default:
            throw std::invalid_argument("Invalid band (" + toString(band) + ")");
        }
    }

    void removeInvalidChannels(uint16_t channelSize, Band band, std::vector<uint8_t>& channels)
    {
        auto it = channels.begin();
        while (it != channels.end()) {
            bool invalid = false;

            auto lowestChannel = *it;
            auto bondedChannels = get20MhzBondedChannels(lowestChannel, channelSize, band);

            for (auto requiredChannel : bondedChannels) {
                if (std::count(channels.begin(), channels.end(), requiredChannel) == 0) {
                    invalid = true;
                    break;
                }
            }

            if (invalid) {
                it = channels.erase(it);
            } else {
                ++it;
            }
        }
    }

    std::vector<uint8_t> getChannels(PhyMode phyMode, Band band, Bandwidth bandwidth)
    {
        std::vector<uint8_t> channels;

        checkPhyMode(phyMode, band);

        switch (band) {
        case Band::BAND_2400MHZ:
            // With 802.11n, there is the option to double the bandwidth per channel to 40 MHz
            // https://en.wikipedia.org/wiki/IEEE_802.11n-2009#40_MHz_channels_in_2.4_GHz
            if ((phyMode < PhyMode::PHY_MODE_N_2_4) && (bandwidth == Bandwidth::BANDWIDTH_FOURTY)) {
                channels = {};
            } else if ((bandwidth >= Bandwidth::BANDWIDTH_TWENTY) && (bandwidth <= Bandwidth::BANDWIDTH_FOURTY)) {
                channels = get20MhzChannelsIn2400MHzBand(phyMode);
            }
            break;
        case Band::BAND_5000MHZ: {
            if ((bandwidth >= Bandwidth::BANDWIDTH_TWENTY) && (bandwidth <= Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY)) {
                channels = get20MhzChannelsIn5000MHzBand();
            }
            break;
        }
        case Band::BAND_6000MHZ:
            if ((bandwidth >= Bandwidth::BANDWIDTH_TWENTY) && (bandwidth <= Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY)) {
                channels = get20MhzChannelsIn6000MHzBand();
            }
            break;
        default:
            throw std::invalid_argument("Invalid band (" + toString(band) + ")");
        }

        if (channels.empty()) {
            throw std::invalid_argument("Invalid bandwidth (" + toString(bandwidth) + ")");
        }

        // Bonded channels that extend beyond maximum allowed frequency values are invalid and must be removed from the list
        if (bandwidth > Bandwidth::BANDWIDTH_TWENTY) {
            removeInvalidChannels(getChannelSize(band, bandwidth), band, channels);
        }

        return channels;
    }

    uint8_t getLowestChannel(uint16_t centerFrequency, PhyMode phyMode, Band band, Bandwidth bandwidth)
    {
        uint8_t lowestChannel = 0;

        uint16_t startFrequency = getStartFrequency(band);

        if (centerFrequency > startFrequency) {
            uint16_t channelSize = getChannelSize(band, bandwidth);

            if ((centerFrequency == centerFrequencyFor20MHzChannel14In2400MHz) && (band == Band::BAND_2400MHZ) && (bandwidth == Bandwidth::BANDWIDTH_TWENTY)) {
                lowestChannel = channel14;
            } else {
                lowestChannel = static_cast<uint8_t>((centerFrequency - channelSize / 2 - startFrequency) / 5 + 1);
            }

#ifdef ALLOW_ILLEGAL_CHANNELS
            (void)phyMode;
#else
            std::vector<uint8_t> channels = getChannels(phyMode, band, bandwidth);
            if (std::count(channels.begin(), channels.end(), lowestChannel) == 0) {
                throw std::invalid_argument("Invalid center frequency (" + toString(centerFrequency) + ") for the specified band (" + toString(band) + ") and bandwidth (" + toString(bandwidth) + ") combination");
            }
#endif
        }

        return lowestChannel;
    }

    uint16_t getCenterFrequency(uint8_t lowestChannel, PhyMode phyMode, Band band, Bandwidth bandwidth)
    {
#ifdef ALLOW_ILLEGAL_CHANNELS
        (void)phyMode;
#else
        auto channels = getChannels(phyMode, band, bandwidth);
        if (std::count(channels.begin(), channels.end(), lowestChannel) == 0) {
            throw std::invalid_argument("Invalid channel (" + toString(lowestChannel) + ")");
        }
#endif

        uint16_t centerFrequency;

        if ((lowestChannel == channel14) && (band == Band::BAND_2400MHZ) && (bandwidth == Bandwidth::BANDWIDTH_TWENTY)) {
            centerFrequency = centerFrequencyFor20MHzChannel14In2400MHz;
        } else {
            uint16_t startFrequency = getStartFrequency(band);
            uint16_t channelSize = getChannelSize(band, bandwidth);

            centerFrequency = startFrequency + (lowestChannel - 1) * 5 + channelSize / 2;
        }

        return centerFrequency;
    }
}
}
