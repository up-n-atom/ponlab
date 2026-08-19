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

#include "Rates.h"

#include "dut/Tools.h"

#include <map>
#include <math.h>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef LINUX_HOST
#include <algorithm>
#endif

namespace dut {
namespace Rates {

    /**
     * @brief Returns the number of data subcarriers that transmit modulated data.
     * 
     * @param bandwidth Channel width.
     * @param mode IEEE Standard (that is, PHY mode: HT, VHT, HE).
     * @return Number of data subcarriers.
     */
    uint32_t getNumBins(PhyMode mode, Bandwidth bandwidth)
    {
        if ((mode == PhyMode::PHY_MODE_AX) || (mode == PhyMode::PHY_MODE_BE)) {
            switch (bandwidth) {
            case Bandwidth::BANDWIDTH_TWENTY:
                return 234;
            case Bandwidth::BANDWIDTH_FOURTY:
                return 468;
            case Bandwidth::BANDWIDTH_EIGHTY:
                return 980;
            case Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY:
                return 1960;
            case Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY:
                if (mode == PhyMode::PHY_MODE_BE) {
                    return 3920;
                }
                break;
            default:
                break;
            }
        } else {
            switch (bandwidth) {
            case Bandwidth::BANDWIDTH_TWENTY:
                // For 20 MHz bandwidth signals, Legacy Mode uses 48 data subcarriers and 4 pilot
                // subcarriers, while HT modes use 52 data subcarriers and 4 pilot subcarriers.
                // This gives 20 MHz HT mode slightly more throughput than Legacy Mode.
                if ((mode == PhyMode::PHY_MODE_A) || (mode == PhyMode::PHY_MODE_G)) {
                    return 48;
                } else {
                    return 52;
                }

            case Bandwidth::BANDWIDTH_FOURTY:
                if (mode > PhyMode::PHY_MODE_G) {
                    return 108;
                }
                break;

            case Bandwidth::BANDWIDTH_EIGHTY:
                if (mode == PhyMode::PHY_MODE_AC) {
                    return 234;
                }
                break;

            case Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY:
                if (mode == PhyMode::PHY_MODE_AC) {
                    return 468;
                }
                break;

            default:
                break;
            }
        }

        throw std::invalid_argument("Unsupported PHY mode and bandwidth combination");
    }

    /**
     * @brief Gets coding rate (how many bits transfer information and how many are used for error correction).
     * 
     * @param mcs Modulation Coding Scheme. 
     * @return coding rate
     */
    float getCoding(Mcs mcs)
    {
        switch (mcs) {
        case Mcs::MCS_BPSK_12:
        case Mcs::MCS_QPSK_12:
        case Mcs::MCS_16QAM_12:
            return 1.0f / 2.0f;
        case Mcs::MCS_64QAM_23:
            return 2.0f / 3.0f;
        case Mcs::MCS_BPSK_34:
        case Mcs::MCS_QPSK_34:
        case Mcs::MCS_16QAM_34:
        case Mcs::MCS_64QAM_34:
        case Mcs::MCS_256QAM_34:
        case Mcs::MCS_1024QAM_34:
        case Mcs::MCS_4096QAM_34:
            return 3.0f / 4.0f;
        case Mcs::MCS_64QAM_56:
        case Mcs::MCS_256QAM_56:
        case Mcs::MCS_1024QAM_56:
        case Mcs::MCS_4096QAM_56:
            return 5.0f / 6.0f;
        default:
            throw std::invalid_argument("Invalid MCS (" + toString(mcs) + ")");
        }
    }

    uint8_t getCodingFor11b(Mcs mcs)
    {
        switch (mcs) {
        case Mcs::MCS_80211b_1MBPS_SHORT: // DSSS-1
        case Mcs::MCS_80211b_2MBPS_SHORT: // DSSS-2
        case Mcs::MCS_80211b_5MBPS_SHORT: // CCK-5_5
        case Mcs::MCS_80211b_11MBPS_SHORT: // CCK-11
        case Mcs::MCS_80211b_1MBPS_LONG:
        case Mcs::MCS_80211b_2MBPS_LONG:
        case Mcs::MCS_80211b_5MBPS_LONG:
        case Mcs::MCS_80211b_11MBPS_LONG:
            return static_cast<uint8_t>(mcs) - static_cast<uint8_t>(Mcs::MCS_80211b_1MBPS_SHORT);
        default:
            throw std::invalid_argument("Invalid MCS (" + toString(mcs) + ")");
        }
    }

    /**
     * @brief Gets the number of coded bits per subcarrier per stream (BPSK=1, QPSK=2, 16-QAM=4, 64-QAM=6, 256-QAM=8, (log2 Modulation)).
     * 
     * @param mcs Modulation Coding Scheme. 
     * @return number of bits
     */
    uint8_t getNumBits(Mcs mcs)
    {
        switch (mcs) {
        case Mcs::MCS_BPSK_12:
        case Mcs::MCS_BPSK_34:
            return 1;
        case Mcs::MCS_QPSK_12:
        case Mcs::MCS_QPSK_34:
            return 2;
        case Mcs::MCS_16QAM_12:
        case Mcs::MCS_16QAM_34:
            return 4;
        case Mcs::MCS_64QAM_23:
        case Mcs::MCS_64QAM_34:
        case Mcs::MCS_64QAM_56:
            return 6;
        case Mcs::MCS_256QAM_34:
        case Mcs::MCS_256QAM_56:
            return 8;
        case Mcs::MCS_1024QAM_34:
        case Mcs::MCS_1024QAM_56:
            return 10;
        case Mcs::MCS_4096QAM_34:
        case Mcs::MCS_4096QAM_56:
            return 12;
        default:
            throw std::invalid_argument("Invalid MCS (" + toString(mcs) + ")");
        }
    }

    /**
     * @brief Computes the number of bits per ODFM symbol.
     * 
     * @param phyMode IEEE Standard (from 802.11a onwards). 
     * @param bandwidth Channel width.
     * @param mcs Modulation Coding Scheme. 
     * @param nss Number of spatial streams.
     * @return Number of bits per symbol.
     */
    float getBitsPerSymbol(PhyMode phyMode, Bandwidth bandwidth, Mcs mcs, uint8_t nss)
    {
        auto numBins = getNumBins(phyMode, bandwidth);
        auto coding = getCoding(mcs);
        auto numBits = getNumBits(mcs);

        auto bitsPerSymbol = static_cast<float>(numBins) * nss * coding * numBits;

        return bitsPerSymbol;
    }

    /**
     * @brief Gets OFDM symbol duration for the given PHY mode.
     *
     * @param phyMode IEEE Standard (from 802.11a onwards).
     * @return OFDM symbol duration (in microseconds).
     */
    float getOfdmSymbolDuration(PhyMode phyMode)
    {
        if ((phyMode == PhyMode::PHY_MODE_AX) || (phyMode == PhyMode::PHY_MODE_BE)) {
            return 12.8f;
        } else if (phyMode != PhyMode::PHY_MODE_B) {
            return 3.2f;
        }

        throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ")");
    }

    /**
     * @brief Gets guard interval duration for the given PHY mode and guard interval.
     *
     * @param phyMode IEEE Standard (from 802.11a onwards).
     * @param gi Guard interval.
     * @return Guard interval duration (in microseconds).
     */
    float getGuardIntervalDuration(PhyMode phyMode, Gi gi)
    {
        if ((phyMode == PhyMode::PHY_MODE_AX) || (phyMode == PhyMode::PHY_MODE_BE)) {
            if (gi == Gi::GI_0_8_US) {
                return 0.8f;
            } else if (gi == Gi::GI_1_6_US) {
                return 1.6f;
            } else if (gi == Gi::GI_3_2_US) {
                return 3.2f;
            }
        } else if (phyMode != PhyMode::PHY_MODE_B) {
            if (gi == Gi::GI_0_8_US) {
                return 0.8f;
            } else if (gi == Gi::GI_0_4_US) {
                return 0.4f;
            }
        }

        throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ") and guard interval (" + toString(gi) + ") combination");
    }

    //bitsPerSymbol=numBins*SS*Coding*Bits
    //example: BitsSymbol=52*1*0.5*1=26
    //         (52 for N and AC)
    //rate=BitsSymbol/(4-guardInterval*0.4)=26/(4-0*0.4)=6.5
    float getRate(PhyMode phyMode, Bandwidth bandwidth, Mcs mcs, uint8_t nss, Gi gi)
    {
        float rate = 0.0;

        try {
            // Get MCS index to check if input parameters are valid
            uint8_t mcsIndex = getMcsIndex(phyMode, mcs, nss);

            if (phyMode == PhyMode::PHY_MODE_B) {
                if (bandwidth != Bandwidth::BANDWIDTH_TWENTY) {
                    throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ") and channel width (" + toString(bandwidth) + ") combination");
                }
                //0 is not legal index for 11B
                if (mcsIndex == 0) {
                    throw std::invalid_argument("Illegal coding value");
                }

                // Channel width, number of spatial streams and guard interval are all ignored when computing data rate for 11b
                const std::vector<float> phyRatesByMcsIndex { 8, 16, 44, 88, 8, 16, 48, 88 };
                rate = phyRatesByMcsIndex.at(mcsIndex);
            } else {
                if (bandwidth > Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY) {
                    throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ") and channel width (" + toString(bandwidth) + ") combination");
                }

                // Check for some special combinations in VHT, depending on the channel bandwidth and the number of spatial streams
                if (phyMode == PhyMode::PHY_MODE_AC) {
                    if ((mcs == Mcs::MCS_256QAM_56) && (bandwidth == Bandwidth::BANDWIDTH_TWENTY) && (nss != 3) && (nss != 6)) {
                        throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ") and MCS (" + toString(mcs) + ") combination for 20MHz channel width and number of spatial streams (" + toString(nss) + ")");
                    } else if ((mcs == Mcs::MCS_64QAM_34) && (bandwidth == Bandwidth::BANDWIDTH_EIGHTY) && ((nss == 3) || (nss == 7))) {
                        throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ") and MCS (" + toString(mcs) + ") combination for 80MHz channel width and number of spatial streams (" + toString(nss) + ")");
                    } else if ((mcs == Mcs::MCS_256QAM_56) && (bandwidth == Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY) && (nss == 3)) {
                        throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ") and MCS (" + toString(mcs) + ") combination for 160MHz channel width and number of spatial streams (" + toString(nss) + ")");
                    }
                }

                auto bitsPerSymbol = getBitsPerSymbol(phyMode, bandwidth, mcs, nss);
                auto tdft = getOfdmSymbolDuration(phyMode);
                auto tgi = getGuardIntervalDuration(phyMode, gi);

                rate = bitsPerSymbol / (tdft + tgi);

                //one digit accuracy:
                rate = roundf(rate * 10.0f) / 10.0f;
            }
        } catch (std::exception const& e) {
            throw std::invalid_argument("Unable to get PHY data rate: " + std::string(e.what()));
        }

        return rate;
    }

    uint8_t getMcsIndex(PhyMode phyMode, Mcs mcs, uint8_t nss)
    {
        auto getIndexOfMcs = [phyMode, mcs](Mcs value, const std::vector<Mcs>& values) {
            auto it = std::find(values.begin(), values.end(), value);
            if (it == values.end()) {
                throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ") and MCS (" + toString(mcs) + ") combination");
            }
            return static_cast<uint8_t>(std::distance(values.begin(), it));
        };

        if (phyMode == PhyMode::PHY_MODE_B) {
            return getCodingFor11b(mcs);
        } else if ((phyMode == PhyMode::PHY_MODE_A) || (phyMode == PhyMode::PHY_MODE_G)) {
            // BPSK 3/4 is valid in 802.11a and 80211g only
            return getIndexOfMcs(mcs, {
                                          Mcs::MCS_BPSK_12, //
                                          Mcs::MCS_BPSK_34, //
                                          Mcs::MCS_QPSK_12, //
                                          Mcs::MCS_QPSK_34, //
                                          Mcs::MCS_16QAM_12, //
                                          Mcs::MCS_16QAM_34, //
                                          Mcs::MCS_64QAM_23, //
                                          Mcs::MCS_64QAM_34 //
                                      });
        } else if ((phyMode == PhyMode::PHY_MODE_N_2_4) || (phyMode == PhyMode::PHY_MODE_N_5)) {
            uint8_t _nss = nss - 1;
            return static_cast<uint8_t>(getIndexOfMcs(mcs, {
                                                               Mcs::MCS_BPSK_12, //
                                                               Mcs::MCS_QPSK_12, //
                                                               Mcs::MCS_QPSK_34, //
                                                               Mcs::MCS_16QAM_12, //
                                                               Mcs::MCS_16QAM_34, //
                                                               Mcs::MCS_64QAM_23, //
                                                               Mcs::MCS_64QAM_34, //
                                                               Mcs::MCS_64QAM_56, //
                                                           })
                | (_nss << 3));
        } else if (phyMode == PhyMode::PHY_MODE_AC) {
            return getIndexOfMcs(mcs, {
                                          Mcs::MCS_BPSK_12, //
                                          Mcs::MCS_QPSK_12, //
                                          Mcs::MCS_QPSK_34, //
                                          Mcs::MCS_16QAM_12, //
                                          Mcs::MCS_16QAM_34, //
                                          Mcs::MCS_64QAM_23, //
                                          Mcs::MCS_64QAM_34, //
                                          Mcs::MCS_64QAM_56, //
                                          Mcs::MCS_256QAM_34, //
                                          Mcs::MCS_256QAM_56, //
                                      });
        } else if (phyMode == PhyMode::PHY_MODE_AX) {
            return getIndexOfMcs(mcs, {
                                          Mcs::MCS_BPSK_12, //
                                          Mcs::MCS_QPSK_12, //
                                          Mcs::MCS_QPSK_34, //
                                          Mcs::MCS_16QAM_12, //
                                          Mcs::MCS_16QAM_34, //
                                          Mcs::MCS_64QAM_23, //
                                          Mcs::MCS_64QAM_34, //
                                          Mcs::MCS_64QAM_56, //
                                          Mcs::MCS_256QAM_34, //
                                          Mcs::MCS_256QAM_56, //
                                          Mcs::MCS_1024QAM_34, //
                                          Mcs::MCS_1024QAM_56, //
                                      });
        } else if (phyMode == PhyMode::PHY_MODE_BE) {
            return getIndexOfMcs(mcs, {
                                          Mcs::MCS_BPSK_12, //
                                          Mcs::MCS_QPSK_12, //
                                          Mcs::MCS_QPSK_34, //
                                          Mcs::MCS_16QAM_12, //
                                          Mcs::MCS_16QAM_34, //
                                          Mcs::MCS_64QAM_23, //
                                          Mcs::MCS_64QAM_34, //
                                          Mcs::MCS_64QAM_56, //
                                          Mcs::MCS_256QAM_34, //
                                          Mcs::MCS_256QAM_56, //
                                          Mcs::MCS_1024QAM_34, //
                                          Mcs::MCS_1024QAM_56, //
                                          Mcs::MCS_4096QAM_34, //
                                          Mcs::MCS_4096QAM_56, //
                                      });
        }

        throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ")");
    }

    /**
     * @brief Checks if LDPC correction code is used.
     * 
     * @param phyMode IEEE Standard (from 802.11a onwards). 
     * @return true if low-density parity-check (LDPC) encoder used and false if binary convolutional codes (BCC) encoder used.
     */
    bool isLdpcOn(PhyMode phyMode)
    {
        switch (phyMode) {
        case PhyMode::PHY_MODE_A:
        case PhyMode::PHY_MODE_G:
            return false;

        case PhyMode::PHY_MODE_N_5:
        case PhyMode::PHY_MODE_N_2_4:
        case PhyMode::PHY_MODE_AC:
        case PhyMode::PHY_MODE_AX:
        case PhyMode::PHY_MODE_BE:
            return true;

        default:
            break;
        }

        throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ")");
    }

    /**
     * @brief Gets the number of BCC encoders in the transmitter for the Data field.
     * 
     * Don't use this function if a LDPC encoder is used.
     *
     * @param bandwidth Channel width.
     * @param mcs Modulation Coding Scheme. 
     * @param nss Number of spatial streams.
     */
    uint8_t getNumBccEncoders(Bandwidth bandwidth, Mcs mcs, uint8_t nss)
    {
        auto getNesValue = [](Mcs _mcs, uint8_t _nss, const std::map<uint8_t, std::vector<uint8_t>>& nes) {
            auto entry = nes.find(_nss);
            if (entry == nes.end()) {
                throw std::invalid_argument("Invalid NSS (" + toString(_nss) + ")");
            }

            auto values = entry->second;
            if (values.size() < static_cast<size_t>(_mcs)) {
                throw std::invalid_argument("Invalid MCS (" + toString(_mcs) + ")");
            }

            return values.at(static_cast<size_t>(_mcs));
        };

        if (bandwidth == Bandwidth::BANDWIDTH_TWENTY) {
            return getNesValue(mcs, nss, {
                                             { 1, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
                                             { 2, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
                                             { 3, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
                                             { 4, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
                                             { 5, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
                                             { 6, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
                                             { 7, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 } },
                                             { 8, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 } },
                                         });
        } else if (bandwidth == Bandwidth::BANDWIDTH_FOURTY) {
            return getNesValue(mcs, nss, {
                                             { 1, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
                                             { 2, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
                                             { 3, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
                                             { 4, { 1, 1, 1, 1, 1, 1, 1, 1, 2, 2 } },
                                             { 5, { 1, 1, 1, 1, 1, 1, 2, 2, 2, 2 } },
                                             { 6, { 1, 1, 1, 1, 1, 2, 2, 2, 2, 2 } },
                                             { 7, { 1, 1, 1, 1, 2, 2, 2, 2, 3, 3 } },
                                             { 8, { 1, 1, 1, 1, 2, 2, 2, 2, 3, 3 } },
                                         });
        } else if (bandwidth == Bandwidth::BANDWIDTH_EIGHTY) {
            return getNesValue(mcs, nss, {
                                             { 1, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
                                             { 2, { 1, 1, 1, 1, 1, 1, 1, 2, 2, 2 } },
                                             { 3, { 1, 1, 1, 1, 1, 1, 2, 2, 2, 3 } },
                                             { 4, { 1, 1, 1, 1, 2, 2, 2, 3, 3, 3 } },
                                             { 5, { 1, 1, 1, 2, 2, 3, 3, 3, 4, 4 } },
                                             { 6, { 1, 1, 1, 1, 2, 2, 3, 3, 4, 4 } },
                                             { 7, { 1, 1, 3, 2, 3, 4, 6, 6, 6, 6 } },
                                             { 8, { 1, 1, 2, 2, 3, 4, 4, 6, 6, 6 } },
                                         });
        } else if (bandwidth == Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY) {
            return getNesValue(mcs, nss, {
                                             { 1, { 1, 1, 1, 1, 1, 1, 1, 2, 2, 2 } },
                                             { 2, { 1, 1, 1, 1, 2, 2, 2, 3, 3, 3 } },
                                             { 3, { 1, 1, 1, 2, 2, 3, 3, 4, 4, 4 } },
                                             { 4, { 1, 1, 2, 2, 3, 4, 4, 6, 6, 6 } },
                                             { 5, { 1, 2, 2, 3, 4, 5, 5, 6, 8, 8 } },
                                             { 6, { 1, 2, 2, 3, 4, 6, 6, 8, 8, 9 } },
                                             { 7, { 1, 2, 3, 4, 6, 7, 7, 9, 12, 12 } },
                                             { 8, { 1, 2, 3, 4, 6, 8, 8, 9, 12, 12 } },
                                         });
        }

        throw std::invalid_argument("Invalid bandwidth (" + toString(bandwidth) + ")");
    }

    uint32_t getPacketLength(PhyMode phyMode, Bandwidth bandwidth, Mcs mcs, uint8_t nss, uint32_t numSymbols)
    {
        // 802.11b is not OFDM (there is no number of symbols)
        if (phyMode == PhyMode::PHY_MODE_B) {
            throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ")");
        }

        if (numSymbols < minNumOdfmSymbols) {
            throw std::invalid_argument("Invalid number of symbols (" + toString(numSymbols) + "). Minimum allowed value is " + toString(minNumOdfmSymbols));
        }

        auto bitsPerSymbol = getBitsPerSymbol(phyMode, bandwidth, mcs, nss);

        auto packetLength = static_cast<float>(numSymbols) * bitsPerSymbol;
        if (!isLdpcOn(phyMode)) {
            auto nes = getNumBccEncoders(bandwidth, mcs, nss);
            packetLength = packetLength - 6 * nes;
        }
        packetLength = floor(packetLength / 8) - 2;

        return static_cast<uint32_t>(packetLength);
    }

    uint32_t getMaxPacketLength(PhyMode phyMode)
    {
        switch (phyMode) {
        case PhyMode::PHY_MODE_A:
        case PhyMode::PHY_MODE_B:
        case PhyMode::PHY_MODE_G:
            return 0xfff;
        case PhyMode::PHY_MODE_N_2_4:
        case PhyMode::PHY_MODE_N_5:
            return 0xffff;
        case PhyMode::PHY_MODE_AC:
            return 0xfffff;
        case PhyMode::PHY_MODE_AX:
        case PhyMode::PHY_MODE_BE:
            return 0x3fffff;
        }

        throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ")");
    }
}
}
