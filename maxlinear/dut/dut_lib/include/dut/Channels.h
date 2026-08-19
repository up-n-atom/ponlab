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

#ifndef _DUT_CHANNELS_H_
#define _DUT_CHANNELS_H_

#include "dut/API.h"
#include "dut/Types.h"

#include <vector>

// Define macro to allow setting illegal channels/frequencies.
// Example: channel 86 (5500 MHz) in 160 MHz
#define ALLOW_ILLEGAL_CHANNELS

namespace dut {
namespace Channels {

    constexpr uint8_t minChannel2400MHz = 1;
    constexpr uint8_t minChannel5000MHz = 36;
    constexpr uint8_t minChannel6000MHz = 1;

    constexpr uint8_t maxChannel2400MHz = 14;
    constexpr uint8_t maxChannel5000MHz = 177;
    constexpr uint8_t maxChannel6000MHz = 233;

    constexpr uint16_t startFrequency2400MHz = 2401;
    constexpr uint16_t startFrequency5000MHz = 4995;
    constexpr uint16_t startFrequency6000MHz = 5945;

    DUT_API std::vector<uint8_t> getChannels(PhyMode phyMode, Band band, Bandwidth bandwidth);

    DUT_API uint8_t getLowestChannel(uint16_t centerFrequency, PhyMode phyMode, Band band, Bandwidth bandwidth);
    DUT_API uint16_t getCenterFrequency(uint8_t lowestChannel, PhyMode phyMode, Band band, Bandwidth bandwidth);
};

}

#endif