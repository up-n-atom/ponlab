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

#include "dut/Logger.h"

namespace dut {

std::string toString(LogLevel logLevel)
{
    switch (logLevel) {
    case LogLevel::LOG_LEVEL_ERROR:
        return "ERROR";
    case LogLevel::LOG_LEVEL_WARNING:
        return "WARNING";
    case LogLevel::LOG_LEVEL_INFO:
        return "INFO";
    case LogLevel::LOG_LEVEL_DEBUG:
        return "DEBUG";
    case LogLevel::LOG_LEVEL_TRACE:
        return "TRACE";
    default:
        return "";
    }
}

Logger::Logger(LogLevel logLevel)
    : m_logLevel(logLevel)
{
}

LogLevel Logger::getLogLevel() const
{
    return m_logLevel;
}

void Logger::setLogLevel(LogLevel logLevel)
{
    m_logLevel = logLevel;
}

void Logger::log(LogLevel level, const std::string& message)
{
    if ((m_logLevel > LogLevel::LOG_LEVEL_NONE) && (level <= m_logLevel)) {
        writeMessageToLog(level, message);
    }
}

}
