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
 *                                          Copyright (c) 2021/2024, MaxLinear, Inc.
 *  ***************************************************************************************
 *  </legal_notice>
 */

#include "dut_c/Connection.h"
#include "dut_c/Dut.h"
#include "dut_c/Logger.h"

#include <iostream>

#define try_else_throw(x)                                \
    {                                                    \
        bool ok = x;                                     \
        if (!ok) {                                       \
            throw std::runtime_error(getLastError(dut)); \
        }                                                \
    }

ConnectionHandle createConnection()
{
    auto connection = Connection_Create();

    if (!Connection_open(connection, "192.168.1.1", 22222, 3000)) {
        throw std::runtime_error("Timeout!");
    }

    return connection;
}

LoggerHandle createLogger()
{
    return Logger_CreateWindowsConsoleLogger(true, LogLevel::LOG_LEVEL_TRACE);
}

DutHandle createDut(ConnectionHandle connection, LoggerHandle logger)
{
    return DUT_Create(0, connection, logger);
}

std::string getLastError(const DutHandle dut)
{
    size_t size;
    DUT_getLastError(dut, nullptr, &size);

    std::shared_ptr<char> buffer(static_cast<char*>(malloc(size)), free);
    if (DUT_getLastError(dut, buffer.get(), &size)) {
        return buffer.get();
    }

    return "Unknown error";
}

int main()
{
    int exitCode = EXIT_SUCCESS;

    ConnectionHandle connection = nullptr;
    LoggerHandle logger = nullptr;
    DutHandle dut = nullptr;

    try {
        connection = createConnection();
        logger = createLogger();
        dut = createDut(connection, logger);

        try_else_throw(DUT_driverInit(dut, false, dut::NvMemoryType::MEMORY_TYPE_FLASH, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::Band::BAND_INVALID));

        {
            // Declare a variable big enough to contain the version string
            char version[256];
            size_t size = sizeof(version);

            // Function will fail if variable was not big enough
            try_else_throw(DUT_getComponentVersion(dut, dut::VersionedComponent::VERSIONED_COMPONENT_CV, version, &size));

            std::cout << "CV: " << version << std::endl;
        }
        {
            // Since we don't know required size in advance, let's query it first.
            char* version = nullptr;
            size_t size = 0;

            // First call to function is just to get the required buffer size
            try_else_throw(DUT_getComponentVersion(dut, dut::VersionedComponent::VERSIONED_COMPONENT_CV, version, &size));

            // Allocate a buffer with the required size
            version = static_cast<char*>(malloc(size));

            // Second call to function uses a buffer that is exactly as big as needed to contain the version string (no more, no less)
            try_else_throw(DUT_getComponentVersion(dut, dut::VersionedComponent::VERSIONED_COMPONENT_CV, version, &size));

            std::cout << "CV: " << version << std::endl;

            // Release allocated memory
            free(version);
        }

        std::array<int16_t, dut::maxNumRxAntennas> rssi;
        try_else_throw(DUT_getInbandRssi(dut, rssi.data()));
        for (size_t i = 0; i < rssi.size(); i++) {
            std::cout << "rssi[" << std::to_string(i) << "]: " << std::to_string(rssi[i]) << std::endl;
        }

        try_else_throw(DUT_driverRelease(dut));
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        exitCode = EXIT_FAILURE;
    }

    DUT_Destroy(&dut);
    Logger_Destroy(&logger);
    Connection_Destroy(&connection);

    return exitCode;
}
