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

#ifndef _DUT_C_LOGGER_H_
#define _DUT_C_LOGGER_H_

#include "dut_c/C_API.h"

#include "dut/Logger.h"
#ifdef __cplusplus
using LoggerHandle = dut::Logger*;
using ConstLoggerHandle = const dut::Logger*;
#else
typedef dut::Logger* LoggerHandle;
typedef const dut::Logger* ConstLoggerHandle;
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum LogLevel {
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE
};

typedef void (*LogFunctionPtr)(LogLevel, const char*);

DUT_C_API LoggerHandle DUT_C_API_ENTRY Logger_CreateNullLogger();
DUT_C_API LoggerHandle DUT_C_API_ENTRY Logger_CreateFunctionPtrLogger(LogFunctionPtr logFunction, LogLevel logLevel);
DUT_C_API LoggerHandle DUT_C_API_ENTRY Logger_CreateWindowsConsoleLogger(bool useColors, LogLevel logLevel);
DUT_C_API void DUT_C_API_ENTRY Logger_Destroy(LoggerHandle* handle);

DUT_C_API LogLevel DUT_C_API_ENTRY Logger_getLogLevel(ConstLoggerHandle handle);
DUT_C_API void DUT_C_API_ENTRY Logger_setLogLevel(LoggerHandle handle, LogLevel logLevel);
DUT_C_API void DUT_C_API_ENTRY Logger_log(LoggerHandle handle, LogLevel level, const char* message);

#ifdef __cplusplus
}
#endif

#endif
