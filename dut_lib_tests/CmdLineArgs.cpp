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

#include "CmdLineArgs.h"

constexpr const char* logLevelArg = "--log_level";
constexpr const char* ipAddressArg = "--ip_address";
constexpr const char* tcpPortArg = "--tcp_port";

CmdLineArgs g_cmdLineArgs;

static const char* getCmdLineArg(int argc, char* argv[], const char* arg)
{
    for (int i = 0; i < argc; i++) {
        if (memcmp(argv[i], arg, strlen(arg)) == 0) {
            if (argv[i][strlen(arg)] == '=') {
                return &argv[i][strlen(arg) + 1];
            }
        }
    }

    return nullptr;
}

void CmdLineArgs::parse(int argc, char* argv[])
{
    const char* logLevel = getCmdLineArg(argc, argv, logLevelArg);
    if (logLevel) {
        m_logLevel = static_cast<dut::LogLevel>(strtol(logLevel, nullptr, 0));
    }

    const char* ipAddress = getCmdLineArg(argc, argv, ipAddressArg);
    if (ipAddress) {
        m_ipAddress = ipAddress;
    }

    const char* tcpPort = getCmdLineArg(argc, argv, tcpPortArg);
    if (tcpPort) {
        m_tcpPort = static_cast<uint16_t>(strtol(tcpPort, nullptr, 0));
    }
}

dut::LogLevel CmdLineArgs::getLogLevel() const
{
    return m_logLevel;
}

std::string CmdLineArgs::getIpAddress() const
{
    return m_ipAddress;
}

uint16_t CmdLineArgs::getTcpPort() const
{
    return m_tcpPort;
}