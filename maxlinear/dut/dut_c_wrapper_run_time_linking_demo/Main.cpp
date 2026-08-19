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

#include <Windows.h>
#include <iostream>

#define try_else_throw(x)                                         \
    {                                                             \
        bool ok = x;                                              \
        if (!ok) {                                                \
            throw std::runtime_error(getLastError(library, dut)); \
        }                                                         \
    }

using Connection_Create_Proc = ConnectionHandle(__cdecl*)(void);
using Connection_Destroy_Proc = void(__cdecl*)(ConnectionHandle* handle);
using Connection_open_Proc = bool(__cdecl*)(ConnectionHandle handle, const char* ipAddress, uint16_t tcpPort, uint32_t timeout);
using Logger_CreateWindowsConsoleLogger_Proc = LoggerHandle(__cdecl*)(bool useColors, LogLevel logLevel);
using Logger_Destroy_Proc = void(__cdecl*)(LoggerHandle* handle);
using DUT_Create_Proc = DutHandle(__cdecl*)(uint8_t wlanIndex, ConnectionHandle connection, LoggerHandle logger);
using DUT_Destroy_Proc = void(__cdecl*)(DutHandle* handle);
using DUT_getLastError_Proc = bool(__cdecl*)(const DutHandle handle, char* buffer, size_t* size);
using DUT_driverInit_Proc = bool(__cdecl*)(const DutHandle handle, bool snifferMode, dut::NvMemoryType memoryType, dut::NvMemorySize memorySize, dut::Band band);
using DUT_driverRelease_Proc = bool(__cdecl*)(const DutHandle handle);
using DUT_getInbandRssi_Proc = bool(__cdecl*)(const DutHandle handle, int16_t rssi[dut::maxNumRxAntennas]);

class Library {
public:
    explicit Library(const char* name)
        : m_hInstance(LoadLibraryA(name))
    {
        if (m_hInstance == nullptr) {
            throw std::runtime_error("Unable to load DLL '" + std::string(name) + "'");
        }
    }

    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;

    virtual ~Library()
    {
        FreeLibrary(m_hInstance);
    }

    FARPROC getFunctionAddress(const char* name) const
    {
        FARPROC procAddress = GetProcAddress(m_hInstance, name);
        if (nullptr == procAddress) {
            throw std::runtime_error("Unable to get address of function '" + std::string(name) + "'");
        }

        return procAddress;
    }

private:
    HINSTANCE m_hInstance = nullptr;
};

class DutLibrary : public Library {
public:
    DutLibrary()
        : Library("dut_c_wrapper.dll")
    {
    }

    Connection_Create_Proc getConnection_Create() const
    {
        return reinterpret_cast<Connection_Create_Proc>(getFunctionAddress("Connection_Create"));
    }
    Connection_Destroy_Proc getConnection_Destroy() const
    {
        return reinterpret_cast<Connection_Destroy_Proc>(getFunctionAddress("Connection_Destroy"));
    }
    Connection_open_Proc getConnection_open() const
    {
        return reinterpret_cast<Connection_open_Proc>(getFunctionAddress("Connection_open"));
    }
    Logger_CreateWindowsConsoleLogger_Proc getLogger_CreateWindowsConsoleLogger() const
    {
        return reinterpret_cast<Logger_CreateWindowsConsoleLogger_Proc>(getFunctionAddress("Logger_CreateWindowsConsoleLogger"));
    }
    Logger_Destroy_Proc getLogger_Destroy() const
    {
        return reinterpret_cast<Logger_Destroy_Proc>(getFunctionAddress("Logger_Destroy"));
    }
    DUT_Create_Proc getDUT_Create() const
    {
        return reinterpret_cast<DUT_Create_Proc>(getFunctionAddress("DUT_Create"));
    }
    DUT_Destroy_Proc getDUT_Destroy() const
    {
        return reinterpret_cast<DUT_Destroy_Proc>(getFunctionAddress("DUT_Destroy"));
    }
    DUT_getLastError_Proc getDUT_getLastError() const
    {
        return reinterpret_cast<DUT_getLastError_Proc>(getFunctionAddress("DUT_getLastError"));
    }
    DUT_driverInit_Proc getDUT_driverInit() const
    {
        return reinterpret_cast<DUT_driverInit_Proc>(getFunctionAddress("DUT_driverInit"));
    }
    DUT_driverRelease_Proc getDUT_driverRelease() const
    {
        return reinterpret_cast<DUT_driverRelease_Proc>(getFunctionAddress("DUT_driverRelease"));
    }
    DUT_getInbandRssi_Proc getDUT_getInbandRssi() const
    {
        return reinterpret_cast<DUT_getInbandRssi_Proc>(getFunctionAddress("DUT_getInbandRssi"));
    }
};

ConnectionHandle createConnection(const DutLibrary& library)
{
    auto connection = (library.getConnection_Create())();

    if (!(library.getConnection_open())(connection, "192.168.1.1", 22222, 3000)) {
        throw std::runtime_error("Timeout!");
    }

    return connection;
}

LoggerHandle createLogger(const DutLibrary& library)
{
    return (library.getLogger_CreateWindowsConsoleLogger())(true, LogLevel::LOG_LEVEL_TRACE);
}

DutHandle createDut(const DutLibrary& library, ConnectionHandle connection, LoggerHandle logger)
{
    return (library.getDUT_Create())(0, connection, logger);
}

std::string getLastError(const DutLibrary& library, const DutHandle dut)
{
    size_t size;
    (library.getDUT_getLastError())(dut, nullptr, &size);

    std::shared_ptr<char> buffer(static_cast<char*>(malloc(size)), free);
    if ((library.getDUT_getLastError())(dut, buffer.get(), &size)) {
        return buffer.get();
    }

    return "Unknown error";
}

int run(const DutLibrary& library)
{
    int exitCode = EXIT_SUCCESS;

    ConnectionHandle connection = nullptr;
    LoggerHandle logger = nullptr;
    DutHandle dut = nullptr;

    try {
        connection = createConnection(library);
        logger = createLogger(library);
        dut = createDut(library, connection, logger);

        try_else_throw((library.getDUT_driverInit())(dut, false, dut::NvMemoryType::MEMORY_TYPE_FLASH, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::Band::BAND_INVALID));

        std::array<int16_t, dut::maxNumRxAntennas> rssi;
        try_else_throw((library.getDUT_getInbandRssi())(dut, rssi.data()));
        for (size_t i = 0; i < rssi.size(); i++) {
            std::cout << "rssi[" << std::to_string(i) << "]: " << std::to_string(rssi[i]) << std::endl;
        }

        try_else_throw((library.getDUT_driverRelease())(dut));
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        exitCode = EXIT_FAILURE;
    }

    (library.getDUT_Destroy())(&dut);
    (library.getLogger_Destroy())(&logger);
    (library.getConnection_Destroy())(&connection);

    return exitCode;
}

int main()
{
    int exitCode = EXIT_SUCCESS;

    try {
        DutLibrary library;

        exitCode = run(library);
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        exitCode = EXIT_FAILURE;
    }

    return exitCode;
}
