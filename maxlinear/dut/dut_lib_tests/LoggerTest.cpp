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

#include <gtest/gtest.h>

namespace {

class MyLogger : public dut::Logger {
public:
    size_t count = 0;
    dut::LogLevel lastLogLevel = dut::LogLevel::LOG_LEVEL_NONE;
    std::string lastLogMessage;

protected:
    void writeMessageToLog(dut::LogLevel level, const std::string& message) override
    {
        count++;

        lastLogLevel = level;
        lastLogMessage = message;
    }
};

TEST(LoggerTest, logLevelNoneToStringShouldSucceed)
{
    EXPECT_EQ("", dut::toString(dut::LogLevel::LOG_LEVEL_NONE));
}

TEST(LoggerTest, logLevelErrorToStringShouldSucceed)
{
    EXPECT_EQ("ERROR", dut::toString(dut::LogLevel::LOG_LEVEL_ERROR));
}

TEST(LoggerTest, logLevelWarningToStringShouldSucceed)
{
    EXPECT_EQ("WARNING", dut::toString(dut::LogLevel::LOG_LEVEL_WARNING));
}

TEST(LoggerTest, logLevelInfoToStringShouldSucceed)
{
    EXPECT_EQ("INFO", dut::toString(dut::LogLevel::LOG_LEVEL_INFO));
}

TEST(LoggerTest, logLevelDebugToStringShouldSucceed)
{
    EXPECT_EQ("DEBUG", dut::toString(dut::LogLevel::LOG_LEVEL_DEBUG));
}

TEST(LoggerTest, logLevelTraceToStringShouldSucceed)
{
    EXPECT_EQ("TRACE", dut::toString(dut::LogLevel::LOG_LEVEL_TRACE));
}

TEST(LoggerTest, getDefaultLogLevelShouldSucceed)
{
    MyLogger logger;

    dut::LogLevel logLevel = logger.getLogLevel();
    EXPECT_EQ(dut::LogLevel::LOG_LEVEL_NONE, logLevel);
}

TEST(LoggerTest, setLogLevelShouldSucceed)
{
    MyLogger logger;

    dut::LogLevel logLevel = dut::LogLevel::LOG_LEVEL_DEBUG;
    logger.setLogLevel(logLevel);

    EXPECT_EQ(logger.getLogLevel(), logLevel);
}

TEST(LoggerTest, logShouldSucceed)
{
    MyLogger logger;

    dut::LogLevel logLevel = dut::LogLevel::LOG_LEVEL_DEBUG;
    logger.setLogLevel(logLevel);

    const std::string message = "Hello, World!";

    // Only DEBUG messages are logged, TRACE messages are filtered out
    logger.log(dut::LogLevel::LOG_LEVEL_DEBUG, message);
    logger.log(dut::LogLevel::LOG_LEVEL_TRACE, message);

    EXPECT_EQ(1, logger.count);
    EXPECT_EQ(logLevel, logger.lastLogLevel);
    EXPECT_EQ(message, logger.lastLogMessage);
}

} // namespace
