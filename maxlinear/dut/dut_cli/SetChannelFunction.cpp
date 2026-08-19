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

#include "SetChannelFunction.h"

#include "tclap/CmdLine.h"

#include <set>

namespace dut_cli {

bool SetChannelFunction::parse(int argc, char* argv[])
{
    TCLAP::CmdLine cmd(argv[0], ' ');

    TCLAP::ValueArg<uint32_t> spectrumBandwidthArg(
        "b",
        "bandwidth",
        "Spectrum bandwidth: 20Mhz = 0, 40Mhz = 1, 80Mhz = 2, 160Mhz = 3, 320Mhz = 4. Default value: " + std::to_string(static_cast<uint32_t>(m_spectrumBandwidth)),
        false,
        static_cast<uint32_t>(m_spectrumBandwidth),
        "Bandwidth",
        cmd);

    TCLAP::ValueArg<uint16_t> channelArg(
        "c",
        "channel",
        "Channel number. Default value: " + std::to_string(static_cast<uint16_t>(m_channel)),
        false,
        static_cast<uint16_t>(m_channel),
        "Channel",
        cmd);

    try {
        cmd.setExceptionHandling(false);
        cmd.parse(argc, argv);

        {
            std::set<Bandwidth_e> validValues {
                Bandwidth_e::BANDWIDTH_TWENTY,
                Bandwidth_e::BANDWIDTH_FOURTY,
                Bandwidth_e::BANDWIDTH_EIGHTY,
                Bandwidth_e::BANDWIDTH_ONE_HUNDRED_SIXTY,
                Bandwidth_e::BANDWIDTH_THREE_HUNDRED_TWENTY,
            };
            if (validValues.find(static_cast<Bandwidth_e>(spectrumBandwidthArg.getValue())) == validValues.end()) {
                throw TCLAP::ArgException("Invalid value", spectrumBandwidthArg.longID());
            }
        }
    } catch (const TCLAP::ArgException& e) {
        std::cerr << "PARSE ERROR: " << e.argId() << std::endl
                  << "\t" << e.error() << std::endl;

        TCLAP::StdOutput out;
        out.usage(cmd);

        return false;
    } catch (const TCLAP::ExitException&) {
        return false;
    }

    m_spectrumBandwidth = static_cast<Bandwidth_e>(spectrumBandwidthArg.getValue());
    m_channel = static_cast<uint8_t>(channelArg.getValue());

    return true;
}

bool SetChannelFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setChannel(m_spectrumBandwidth, m_channel, false);
}

}
