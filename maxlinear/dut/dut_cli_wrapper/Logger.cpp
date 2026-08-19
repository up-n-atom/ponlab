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

#include "Logger.h"

#include "marshal_dut.h"

using namespace System;

namespace dut_cli {

Logger::Logger(std::shared_ptr<dut::Logger> logger)
    : ManagedObject(logger)
{
}

LogLevel Logger::getLogLevel()
{
    auto logLevel = GetInstance()->getLogLevel();
    return static_cast<LogLevel>(logLevel);
}

void Logger::setLogLevel(LogLevel logLevel)
{
    auto _logLevel = static_cast<dut::LogLevel>(logLevel);
    GetInstance()->setLogLevel(_logLevel);
}

void Logger::log(LogLevel level, System::String ^ message)
{
    auto _level = static_cast<dut::LogLevel>(level);
    auto _message = msclr::interop::marshal_as<std::string>(message);
    GetInstance()->log(_level, _message);
}

NullLogger::NullLogger()
    : Logger(std::make_shared<dut::NullLogger>())
{
}

WindowsConsoleLogger::WindowsConsoleLogger(bool useColors, LogLevel logLevel)
    : Logger(std::make_shared<dut::WindowsConsoleLogger>(useColors, static_cast<dut::LogLevel>(logLevel)))
{
}

static IntPtr getIntPtrForDelegate(LogDelegate ^ logDelegate)
{
    return Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(logDelegate);
}

static void* getVoidPtrForDelegate(LogDelegate ^ logDelegate)
{
    return getIntPtrForDelegate(logDelegate).ToPointer();
}

static dut::FunctionPtrLogger::LogFunction getFunctionPtrForDelegate(LogDelegate ^ logDelegate)
{
    return static_cast<dut::FunctionPtrLogger::LogFunction>(getVoidPtrForDelegate(logDelegate));
}

/// <summary>
/// You must be sure to keep around the managed delegate object forever. As soon as the managed delegate is garbage collected, the raw function pointer stops working.
/// </summary>
/// <param name="logDelegate"></param>
DelegatedLogger::DelegatedLogger(LogDelegate ^ logDelegate)
    : Logger(std::make_shared<dut::FunctionPtrLogger>(getFunctionPtrForDelegate(logDelegate)))
{
}

}