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

#include "Config.h"

#include "tclap/CmdLine.h"

#define COMMAND_DESCRIPTION "DUT Command Line Interface (C) MaxLinear, Inc."

namespace dut_cli {

const std::string validValuesForLogLevelString = "NONE = 0, ERROR = 1, WARNING = 2, INFO = 3, DEBUG = 4 and TRACE = 5";

Config::Config(const std::string& version)
    : m_version(version)
{
}

bool Config::parse(int argc, char* argv[], int& exitCode)
{
    exitCode = EXIT_SUCCESS;

    // Read the Templatized C++ Command Line Parser Library (TCLAP) manual at
    // http://tclap.sourceforge.net/manual.html
    TCLAP::CmdLine cmd(COMMAND_DESCRIPTION, ' ', m_version);

    TCLAP::ValueArg<std::string> ipAddressArg(
        "a",
        "ip-address",
        "Device IP address. Default value: " + ipAddress,
        false,
        ipAddress,
        "IP address",
        cmd);

    TCLAP::ValueArg<uint16_t> tcpPortArg(
        "p",
        "tcp-port",
        "TCP port where device is listening for DUT requests. Default value: " + std::to_string(tcpPort),
        false,
        tcpPort,
        "Port number",
        cmd);

    TCLAP::ValueArg<uint32_t> connectionTimeoutArg(
        "c",
        "connection-timeout",
        "Connection timeout in milliseconds. Default value: " + std::to_string(static_cast<uint32_t>(connectionTimeout.count())),
        false,
        static_cast<uint32_t>(connectionTimeout.count()),
        "Timeout",
        cmd);

    TCLAP::ValueArg<uint16_t> logLevelArg(
        "l",
        "log-level",
        "Log level (threshold for log messages). Valid values: " + validValuesForLogLevelString + ". Default value: " + std::to_string(static_cast<uint16_t>(logLevel)),
        false,
        static_cast<uint16_t>(logLevel),
        "Log level",
        cmd);

    TCLAP::SwitchArg failOnExceptionArg(
        "",
        "fail-on-exception",
        "Exit with failure if an exception is thrown",
        cmd,
        failOnException);

    try {
        cmd.setExceptionHandling(false);
        cmd.parse(argc, argv);

        if (logLevelArg.getValue() > static_cast<uint16_t>(dut::LogLevel::LOG_LEVEL_TRACE)) {
            throw TCLAP::ArgException("Invalid value", logLevelArg.longID());
        }

    } catch (const TCLAP::ArgException& e) {
        std::cerr << "PARSE ERROR: " << e.argId() << std::endl
                  << "\t" << e.error() << std::endl;

        TCLAP::StdOutput out;
        out.usage(cmd);

        exitCode = EXIT_FAILURE;
        return false;
    } catch (const TCLAP::ExitException&) {
        return false;
    }

    ipAddress = ipAddressArg.getValue();
    tcpPort = tcpPortArg.getValue();
    connectionTimeout = std::chrono::milliseconds(connectionTimeoutArg.getValue());
    logLevel = static_cast<dut::LogLevel>(logLevelArg.getValue());
    failOnException = failOnExceptionArg.getValue();

    return true;
}

}
