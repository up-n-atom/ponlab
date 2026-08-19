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

#include "Channels.h"
#include "dut/Channels.h"

#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace dut_cli {

void Channels::getChannels(PhyMode phyMode, Band band, Bandwidth bandwidth, [Out] cli::array<uint8_t> ^ % channels)
{
    try {
        auto _phyMode = static_cast<dut::PhyMode>(phyMode);
        auto _band = static_cast<dut::Band>(band);
        auto _bandwidth = static_cast<dut::Bandwidth>(bandwidth);

        auto _channels = dut::Channels::getChannels(_phyMode, _band, _bandwidth);

        channels = gcnew cli::array<uint8_t>(static_cast<int>(_channels.size()));
        for (int i = 0; i < _channels.size(); i++) {
            channels[i] = _channels.at(i);
        }

    } catch (std::exception const& e) {
        auto _message = msclr::interop::marshal_as<System::String ^>(e.what());
        throw gcnew System::Exception(_message);
    }
}

uint8_t Channels::getLowestChannel(uint16_t centerFrequency, PhyMode phyMode, Band band, Bandwidth bandwidth)
{
    auto _phyMode = static_cast<dut::PhyMode>(phyMode);
    auto _band = static_cast<dut::Band>(band);
    auto _bandwidth = static_cast<dut::Bandwidth>(bandwidth);

    try {
        return dut::Channels::getLowestChannel(centerFrequency, _phyMode, _band, _bandwidth);
    } catch (std::exception const& e) {
        auto _message = msclr::interop::marshal_as<System::String ^>(e.what());
        throw gcnew System::Exception(_message);
    }
}

uint16_t Channels::getCenterFrequency(uint8_t lowestChannel, PhyMode phyMode, Band band, Bandwidth bandwidth)
{
    auto _phyMode = static_cast<dut::PhyMode>(phyMode);
    auto _band = static_cast<dut::Band>(band);
    auto _bandwidth = static_cast<dut::Bandwidth>(bandwidth);

    try {
        return dut::Channels::getCenterFrequency(lowestChannel, _phyMode, _band, _bandwidth);
    } catch (std::exception const& e) {
        auto _message = msclr::interop::marshal_as<System::String ^>(e.what());
        throw gcnew System::Exception(_message);
    }
}

}