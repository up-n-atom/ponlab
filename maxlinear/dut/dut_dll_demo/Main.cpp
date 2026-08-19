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

#include "dut/ConnectionImpl.h"
#include "dut/DutImpl.h"
#include "dut/WindowsConsoleLogger.h"

#include <iostream>

std::shared_ptr<dut::Connection> createConnection()
{
    auto connection = std::make_shared<dut::ConnectionImpl>();
    if (!connection->open("192.168.1.1", 22222, std::chrono::milliseconds(3000))) {
        throw std::runtime_error("Timeout!");
    }
    return connection;
}

std::shared_ptr<dut::Logger> createLogger()
{
    return std::make_shared<dut::WindowsConsoleLogger>(true, dut::LogLevel::LOG_LEVEL_TRACE);
}

int main()
{
    try {
        auto connection = createConnection();
        auto logger = createLogger();

        dut::DutImpl dut(0, connection, logger, true);

        dut.driverInit(false, dut::NvMemoryType::MEMORY_TYPE_FLASH, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::Band::BAND_INVALID);

        std::array<int16_t, dut::maxNumRxAntennas> rssi;
        dut.getInbandRssi(rssi);
        for (size_t i = 0; i < rssi.size(); i++) {
            std::cout << "rssi[" << std::to_string(i) << "]: " << std::to_string(rssi[i]) << std::endl;
        }

        dut.driverRelease();
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
