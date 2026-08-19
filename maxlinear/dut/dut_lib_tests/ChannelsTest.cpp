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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::ThrowsMessage;

namespace {

const std::vector<std::tuple<uint8_t, dut::PhyMode, dut::Band, dut::Bandwidth, uint16_t>> channels {
    // https://en.wikipedia.org/wiki/List_of_WLAN_channels#2.4_GHz_(802.11b/g/n/ax)
    { 1, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 2412 },
    { 2, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 2417 },
    { 3, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 2422 },
    { 4, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 2427 },
    { 5, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 2432 },
    { 6, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 2437 },
    { 7, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 2442 },
    { 8, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 2447 },
    { 9, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 2452 },
    { 10, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 2457 },
    { 11, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 2462 },
    { 12, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 2467 },
    { 13, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 2472 },
    // Channel 14 is valid only for DSSS and CCK modes (Clause 18 a.k.a. 802.11b)
    { 14, dut::PhyMode::PHY_MODE_B, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 2484 },

    // https://en.wikipedia.org/wiki/IEEE_802.11n-2009
    { 1, dut::PhyMode::PHY_MODE_N_2_4, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 2422 },
    { 2, dut::PhyMode::PHY_MODE_N_2_4, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 2427 },
    { 3, dut::PhyMode::PHY_MODE_N_2_4, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 2432 },
    { 4, dut::PhyMode::PHY_MODE_N_2_4, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 2437 },
    { 5, dut::PhyMode::PHY_MODE_N_2_4, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 2442 },
    { 6, dut::PhyMode::PHY_MODE_N_2_4, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 2447 },
    { 7, dut::PhyMode::PHY_MODE_N_2_4, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 2452 },
    { 8, dut::PhyMode::PHY_MODE_N_2_4, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 2457 },
    { 9, dut::PhyMode::PHY_MODE_N_2_4, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 2462 },

    // https://en.wikipedia.org/wiki/List_of_WLAN_channels#5_GHz_(802.11a/h/j/n/ac/ax)
    { 36, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5180 },
    { 38, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5190 },
    { 40, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5200 },
    { 42, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5210 },
    { 44, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5220 },
    { 46, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5230 },
    { 48, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5240 },
    { 50, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5250 },
    { 52, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5260 },
    { 54, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5270 },
    { 56, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5280 },
    { 58, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5290 },
    { 60, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5300 },
    { 62, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5310 },
    { 64, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5320 },
    { 68, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5340 },
    { 96, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5480 },
    { 100, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5500 },
    { 149, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5745 },
    { 157, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5785 },
    { 173, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5865 },
    { 175, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5875 },
    { 177, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5885 },

    { 36, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 5190 },
    { 44, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 5230 },
    { 52, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 5270 },
    { 54, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 5280 },
    { 60, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 5310 },
    { 149, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 5755 },
    { 157, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 5795 },
    { 173, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 5875 },

    { 36, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY, 5210 },
    { 52, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY, 5290 },
    { 100, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY, 5530 },
    { 116, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY, 5610 },
    { 149, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY, 5775 },
    { 165, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY, 5855 },

    { 36, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, 5250 },
    { 100, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, 5570 },
    { 149, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, 5815 },

    // https://en.wikipedia.org/wiki/List_of_WLAN_channels#6_GHz_(802.11ax_and_802.11be)
    { 1, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5955 },
    { 3, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5965 },
    { 5, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5975 },
    { 7, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5985 },
    { 9, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 5995 },
    { 11, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6005 },
    { 13, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6015 },
    { 15, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6025 },
    { 17, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6035 },
    { 19, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6045 },
    { 21, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6055 },
    { 23, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6065 },
    { 25, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6075 },
    { 27, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6085 },
    { 29, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6095 },
    { 31, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6105 },
    { 33, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6115 },
    { 35, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6125 },
    { 37, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6135 },
    { 39, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6145 },
    { 41, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6155 },
    { 43, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6165 },
    { 45, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6175 },
    { 47, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6185 },
    { 49, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6195 },
    { 51, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6205 },
    { 53, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6215 },
    { 55, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6225 },
    { 57, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6235 },
    { 59, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6245 },
    { 61, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6255 },
    { 63, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY, 6265 },

    { 1, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 5965 },
    { 5, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 5985 },
    { 9, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 6005 },
    { 13, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY, 6025 },

    { 1, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY, 5985 },
    { 5, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY, 6005 },

    { 1, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, 6025 },
    { 5, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, 6045 },

    { 1, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY, 6105 },
    { 5, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY, 6125 },
};

const std::vector<std::tuple<uint8_t, dut::PhyMode, dut::Band, dut::Bandwidth>> invalidChannels {
    // Out of range channels are invalid
    { 0, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY },
    { 14, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY },
    { 15, dut::PhyMode::PHY_MODE_B, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY },

    { 34, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY },
    { 179, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY },
    { 66, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY },
    { 98, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY },

    { 0, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY },
    { 234, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY },

    // Odd channels are invalid in 5 GHz
    { 37, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY },

    // Invalid in 6 GHz
    { 2, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY },
    { 4, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY },
    { 6, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY },

    // Channels that extend beyond maximum allowed frequency values are invalid
    { 10, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_FOURTY },
    { 11, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_FOURTY },
    { 12, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_FOURTY },

    { 62, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY },
    { 64, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY },
    { 68, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY },
    { 96, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY },
    { 142, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY },
    { 144, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY },
    { 175, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY },
    { 177, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY },

    { 54, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY },
    { 56, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY },
    { 58, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY },
    { 60, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY },
    { 62, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY },
    { 64, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY },
    { 68, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY },
    { 96, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY },

    { 68, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY },
    { 96, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY },
    { 132, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY },

    { 233, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_FOURTY },
    { 225, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY },
    { 225, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY },
};

std::string lowestChannelToString(uint8_t lowestChannel, dut::PhyMode phyMode, dut::Band band, dut::Bandwidth bandwidth)
{
    return "[lowestChannel " + dut::toString(lowestChannel) + ", phyMode " + dut::toString(phyMode) + ", band " + dut::toString(band) + ", bandwidth " + dut::toString(bandwidth) + "]";
}

std::string centerFrequencyToString(uint16_t centerFrequency, dut::PhyMode phyMode, dut::Band band, dut::Bandwidth bandwidth)
{
    return "[centerFrequency " + dut::toString(centerFrequency) + ", phyMode " + dut::toString(phyMode) + ", band " + dut::toString(band) + ", bandwidth " + dut::toString(bandwidth) + "]";
}

TEST(ChannelsTest, getChannelsShouldSucceed)
{
    std::vector<dut::PhyMode> phyModes {
        dut::PhyMode::PHY_MODE_G,
        dut::PhyMode::PHY_MODE_N_2_4,
        dut::PhyMode::PHY_MODE_AC,
        dut::PhyMode::PHY_MODE_AX,
        dut::PhyMode::PHY_MODE_BE
    };

    for (auto phyMode : phyModes) {
        std::vector<uint8_t> expectedChannels { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
        std::vector<uint8_t> actualChannels = dut::Channels::getChannels(phyMode, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY);

        EXPECT_EQ(expectedChannels.size(), actualChannels.size());
        for (size_t i = 0; i < expectedChannels.size(); i++) {
            EXPECT_EQ(expectedChannels.at(i), actualChannels.at(i));
        }
    }
}

TEST(ChannelsTest, getChannelsShouldSucceedWith11b)
{
    std::vector<uint8_t> expectedChannels { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    std::vector<uint8_t> actualChannels = dut::Channels::getChannels(dut::PhyMode::PHY_MODE_B, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_TWENTY);

    EXPECT_EQ(expectedChannels.size(), actualChannels.size());
    for (size_t i = 0; i < expectedChannels.size(); i++) {
        EXPECT_EQ(expectedChannels.at(i), actualChannels.at(i));
    }
}

TEST(ChannelsTest, getChannelsShouldSucceedWith320MHz)
{
    std::vector<uint8_t> expectedChannels { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63 };
    std::vector<uint8_t> actualChannels = dut::Channels::getChannels(dut::PhyMode::PHY_MODE_BE, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY);

    EXPECT_EQ(106, actualChannels.size());
    for (size_t i = 0; i < expectedChannels.size(); i++) {
        EXPECT_EQ(expectedChannels.at(i), actualChannels.at(i));
    }
}

TEST(ChannelsTest, getChannelsShouldFailWithInvalidBand)
{
    const std::string expectedMessage = "Invalid band (INVALID)";
    EXPECT_THAT([&]() { dut::Channels::getChannels(dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_INVALID, dut::Bandwidth::BANDWIDTH_TWENTY); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST(ChannelsTest, getChannelsShouldFailWithInvalidPhyMode)
{
    std::vector<std::tuple<dut::Band, dut::PhyMode>> invalidPhyModes {
        { dut::Band::BAND_2400MHZ, dut::PhyMode::PHY_MODE_A },
        { dut::Band::BAND_2400MHZ, dut::PhyMode::PHY_MODE_N_5 },

        { dut::Band::BAND_5000MHZ, dut::PhyMode::PHY_MODE_B },
        { dut::Band::BAND_5000MHZ, dut::PhyMode::PHY_MODE_N_2_4 },

        { dut::Band::BAND_6000MHZ, dut::PhyMode::PHY_MODE_A },
        { dut::Band::BAND_6000MHZ, dut::PhyMode::PHY_MODE_B },
        { dut::Band::BAND_6000MHZ, dut::PhyMode::PHY_MODE_G },
        { dut::Band::BAND_6000MHZ, dut::PhyMode::PHY_MODE_N_2_4 },
        { dut::Band::BAND_6000MHZ, dut::PhyMode::PHY_MODE_N_5 },
        { dut::Band::BAND_6000MHZ, dut::PhyMode::PHY_MODE_AC },
    };

    for (const auto& record : invalidPhyModes) {
        dut::Band band = std::get<0>(record);
        dut::PhyMode phyMode = std::get<1>(record);

        EXPECT_THROW(dut::Channels::getChannels(phyMode, band, dut::Bandwidth::BANDWIDTH_TWENTY), std::invalid_argument);
    }
}

TEST(ChannelsTest, getChannelsShouldFailWithInvalidBandwith)
{
    std::vector<std::tuple<dut::PhyMode, dut::Band, dut::Bandwidth>> invalidBandwiths {
        { dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_EIGHTY },
        { dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY },
        { dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY },
        { dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_INVALID },

        { dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY },
        { dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_INVALID },

        { dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_6000MHZ, dut::Bandwidth::BANDWIDTH_INVALID },

        // https://en.wikipedia.org/wiki/IEEE_802.11n-2009
        { dut::PhyMode::PHY_MODE_B, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_FOURTY },
        { dut::PhyMode::PHY_MODE_G, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_FOURTY },
    };

    for (const auto& record : invalidBandwiths) {
        dut::PhyMode phyMode = std::get<0>(record);
        dut::Band band = std::get<1>(record);
        dut::Bandwidth bandwidth = std::get<2>(record);

        const std::string expectedMessage = "Invalid bandwidth (" + toString(bandwidth) + ")";
        EXPECT_THAT([&]() { dut::Channels::getChannels(phyMode, band, bandwidth); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
    }
}

TEST(ChannelsTest, getLowestChannelShouldFailWithInvalidBand)
{
    const std::string expectedMessage = "Invalid band (INVALID)";
    EXPECT_THAT([&]() { dut::Channels::getLowestChannel(5180, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_INVALID, dut::Bandwidth::BANDWIDTH_TWENTY); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST(ChannelsTest, getLowestChannelShouldFailWithInvalidBandwidth)
{
    const std::string expectedMessage = "Invalid bandwidth (INVALID)";
    EXPECT_THAT([&]() { dut::Channels::getLowestChannel(5180, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_INVALID); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

#ifndef ALLOW_ILLEGAL_CHANNELS
TEST(ChannelsTest, getLowestChannelShouldFailWithInvalidCenterFrequency)
{
    const std::string expectedMessage = "Invalid center frequency (5170) for the specified band (5GHz) and bandwidth (20MHz) combination";
    EXPECT_THAT([&]() { dut::Channels::getLowestChannel(5170, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}
#endif

TEST(ChannelsTest, getLowestChannelShouldFailWithIncompleteCenterFrequency)
{
    EXPECT_EQ(dut::Channels::getLowestChannel(518, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_5000MHZ, dut::Bandwidth::BANDWIDTH_TWENTY), 0);
}

TEST(ChannelsTest, getLowestChannelShouldSucceed)
{
    for (const auto& record : channels) {
        uint8_t lowestChannel = std::get<0>(record);
        dut::PhyMode phyMode = std::get<1>(record);
        dut::Band band = std::get<2>(record);
        dut::Bandwidth bandwidth = std::get<3>(record);
        uint16_t centerFrequency = std::get<4>(record);

        try {
            EXPECT_EQ(dut::Channels::getLowestChannel(centerFrequency, phyMode, band, bandwidth), lowestChannel) << centerFrequencyToString(centerFrequency, phyMode, band, bandwidth);
        } catch (...) {
            std::cout << centerFrequencyToString(centerFrequency, phyMode, band, bandwidth) << std::endl;
            throw;
        }
    }
}

#ifndef ALLOW_ILLEGAL_CHANNELS
TEST(ChannelsTest, getCenterFrequencyShouldFailWithInvalidChannel)
{
    for (const auto& record : invalidChannels) {
        uint8_t lowestChannel = std::get<0>(record);
        dut::PhyMode phyMode = std::get<1>(record);
        dut::Band band = std::get<2>(record);
        dut::Bandwidth bandwidth = std::get<3>(record);

        const std::string expectedMessage = "Invalid channel (" + dut::toString(lowestChannel) + ")";
        EXPECT_THAT([&]() { dut::Channels::getCenterFrequency(lowestChannel, phyMode, band, bandwidth); }, ThrowsMessage<std::invalid_argument>(expectedMessage)) << lowestChannelToString(lowestChannel, phyMode, band, bandwidth);
    }
}
#endif

TEST(ChannelsTest, getCenterFrequencyShouldFailWithInvalidBand)
{
    const std::string expectedMessage = "Invalid band (INVALID)";
    EXPECT_THAT([&]() { dut::Channels::getCenterFrequency(1, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_INVALID, dut::Bandwidth::BANDWIDTH_TWENTY); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST(ChannelsTest, getCenterFrequencyShouldFailWithInvalidBandwidth)
{
    const std::string expectedMessage = "Invalid bandwidth (INVALID)";
    EXPECT_THAT([&]() { dut::Channels::getCenterFrequency(1, dut::PhyMode::PHY_MODE_AX, dut::Band::BAND_2400MHZ, dut::Bandwidth::BANDWIDTH_INVALID); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST(ChannelsTest, getCenterFrequencyShouldSucceed)
{
    for (const auto& record : channels) {
        uint8_t lowestChannel = std::get<0>(record);
        dut::PhyMode phyMode = std::get<1>(record);
        dut::Band band = std::get<2>(record);
        dut::Bandwidth bandwidth = std::get<3>(record);
        uint16_t centerFrequency = std::get<4>(record);

        try {
            EXPECT_EQ(dut::Channels::getCenterFrequency(lowestChannel, phyMode, band, bandwidth), centerFrequency) << lowestChannelToString(lowestChannel, phyMode, band, bandwidth);
        } catch (...) {
            std::cout << lowestChannelToString(lowestChannel, phyMode, band, bandwidth) << std::endl;
            throw;
        }
    }
}

} // namespace
