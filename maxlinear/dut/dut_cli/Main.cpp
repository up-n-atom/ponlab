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

#include <iostream>

#include "Application.h"
#include "ConsoleImpl.h"

#include "dut/ConnectionImpl.h"
#include "dut/DutImpl.h"
#ifndef LINUX_HOST
#include "dut/WindowsConsoleLogger.h"

#include "dut/WinSockInit.h"

// Static variable to ensure that winsock is initialized before main, and
// therefore before any other threads can get started.
const WinSockInit g_winSockInitInstance;
#else
#include "dut/StreamLogger.h"
#endif

static std::shared_ptr<dut::Connection> createConnection(const dut_cli::Config& config)
{
    std::shared_ptr<dut::ConnectionImpl> connection(new dut::ConnectionImpl(), [](dut::ConnectionImpl* _connection) { _connection->close(); });
    if (!connection->open(config.ipAddress, config.tcpPort, config.connectionTimeout)) {
        throw std::runtime_error("Connection timeout! Make sure the IP address is correct and DUT Server is running on the AP.");
    }

    return connection;
}

#ifndef LINUX_HOST
static std::shared_ptr<dut::Logger> createLogger(const dut_cli::Config& config)
{
    return std::make_shared<dut::WindowsConsoleLogger>(true, config.logLevel);
}
#else
static std::shared_ptr<dut::Logger> createLogger(const dut_cli::Config& config)
{
    return std::make_shared<dut::StreamLogger>(std::cout, config.logLevel);
}
#endif

int main(int argc, char* argv[])
{
    int exitCode = EXIT_SUCCESS;

    dut_cli::Config config(dut::Dut::getVersion() + " (" + dut::Dut::getBuildTag() + ")");

    if (config.parse(argc, argv, exitCode)) {
        try {
            dut_cli::ConsoleImpl console;
            dut::DutImplFactory dutFactory;
            auto connection = createConnection(config);
            auto logger = createLogger(config);
            dut_cli::Context context(config, console, dutFactory, connection, logger);

            dut_cli::Application application(context);
            if (!application.run()) {
                exitCode = EXIT_FAILURE;
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << std::string(e.what()) << std::endl;
            exitCode = EXIT_FAILURE;
        }
    }

    return exitCode;
}
