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

#include "CommandLine.h"

#include "Config.h"
#include "ConsoleMock.h"
#include "Context.h"
#include "TempFile.h"
#include "dut/ConnectionMock.h"
#include "dut/DutMock.h"

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;

namespace {

class CommandsTest : public ::testing::Test {
protected:
    CommandsTest()
    {
        m_config.failOnException = true;
    }

    std::shared_ptr<dut::Connection> m_connection = std::make_shared<dut::ConnectionMock>();
    std::shared_ptr<dut::Logger> m_logger = std::make_shared<dut::NullLogger>();

    dut_cli::Config m_config;
    StrictMock<dut_cli::ConsoleMock> m_console;
    StrictMock<dut::DutMock>* m_dut0 = new StrictMock<dut::DutMock>();
    StrictMock<dut::DutMock>* m_dut2 = new StrictMock<dut::DutMock>();
    StrictMock<dut::DutMock>* m_dut4 = new StrictMock<dut::DutMock>();
    dut::DutMockFactory m_dutFactory;

    dut_cli::Context m_context = dut_cli::Context(m_config, m_console, m_dutFactory, m_connection, m_logger);
};

TEST_F(CommandsTest, runShouldFailWithInvalidCommand)
{
    EXPECT_CALL(m_console, cerr("Exception: Invalid command\n")).Times(1);

    ASSERT_FALSE(dut_cli::CommandLine::run("invalid-command", m_context));
}

TEST_F(CommandsTest, runShouldSucceedWithApiCommand)
{
    EXPECT_CALL(m_console, cout(_)).WillRepeatedly(Return());

    ASSERT_TRUE(dut_cli::CommandLine::run("api", m_context));
}

TEST_F(CommandsTest, runShouldSucceedWithHelpCommand)
{
    std::string expectedHelp = "help\n\tShow this message.\n";

    EXPECT_CALL(m_console, cout(::testing::HasSubstr(expectedHelp))).Times(1);

    ASSERT_TRUE(dut_cli::CommandLine::run("help", m_context));
}

TEST_F(CommandsTest, runShouldSucceedWithSleepCommand)
{
    ASSERT_TRUE(dut_cli::CommandLine::run("sleep 1", m_context));
}

TEST_F(CommandsTest, runShouldFailWithSleepCommandWithNoSleepTime)
{
    EXPECT_CALL(m_console, cerr("Exception: Sleep time not specified\n")).Times(1);

    ASSERT_FALSE(dut_cli::CommandLine::run("sleep", m_context));
}

TEST_F(CommandsTest, runShouldFailWithSleepCommandWithInvalidSleepTime)
{
    EXPECT_CALL(m_console, cerr("Exception: Invalid sleep time\n")).Times(1);

    ASSERT_FALSE(dut_cli::CommandLine::run("sleep abc", m_context));
}

TEST_F(CommandsTest, runShouldSucceedWithVersionCommand)
{
    std::string expectedVersion = dut::Dut::getVersion();

    EXPECT_CALL(m_console, cout(::testing::StartsWith(expectedVersion))).Times(1);

    ASSERT_TRUE(dut_cli::CommandLine::run("version", m_context));
}

TEST_F(CommandsTest, runShouldSucceedWithExecCommand)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, driverInit(false, dut::NvMemoryType::MEMORY_TYPE_FLASH, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::Band::BAND_INVALID)).WillOnce(Return(true));
        EXPECT_CALL(*m_dut0, driverRelease()).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 driverInit -t 2 -s 1024", m_context));
    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 driverRelease", m_context));
}

TEST_F(CommandsTest, runShouldSucceedWithExecCommandAndDifferentWlanIndex)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, driverRelease()).WillOnce(Return(true));
        EXPECT_CALL(m_dutFactory, createInstanceProxy(2, m_connection, m_logger, true)).WillOnce(Return(m_dut2));
        EXPECT_CALL(*m_dut2, driverRelease()).WillOnce(Return(true));
        EXPECT_CALL(m_dutFactory, createInstanceProxy(4, m_connection, m_logger, true)).WillOnce(Return(m_dut4));
        EXPECT_CALL(*m_dut4, driverRelease()).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("exec 0 driverRelease", m_context));
    ASSERT_TRUE(dut_cli::CommandLine::run("exec 2 driverRelease", m_context));
    ASSERT_TRUE(dut_cli::CommandLine::run("exec 4 driverRelease", m_context));
}

TEST_F(CommandsTest, runShouldFailWithExecCommandIfCommandFails)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, driverRelease()).WillOnce(Return(false));
    }

    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 driverRelease", m_context));
}

TEST_F(CommandsTest, runShouldFailWithExecCommandIfWlanIndexNotSpecified)
{
    EXPECT_CALL(m_console, cerr("Exception: WLAN index not specified\n")).Times(1);

    ASSERT_FALSE(dut_cli::CommandLine::run("exec", m_context));
}

TEST_F(CommandsTest, runShouldFailWithExecCommandIfInvalidWlanIndex)
{
    EXPECT_CALL(m_console, cerr("Exception: Invalid WLAN index. Valid values: 2.4GHz = 0, 5GHz = 2 and 6GHz = 4\n")).Times(1);

    ASSERT_FALSE(dut_cli::CommandLine::run("exec abc", m_context));
}

TEST_F(CommandsTest, runShouldFailWithExecCommandIfFunctionNotSpecified)
{
    EXPECT_CALL(m_console, cerr("Exception: Function not specified. Run the 'api' command to get a list of available functions\n")).Times(1);

    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0", m_context));
}

TEST_F(CommandsTest, runShouldFailWithExecCommandIfInvalidFunction)
{
    EXPECT_CALL(m_console, cerr("Exception: Invalid function. Run the 'api' command to get a list of available functions\n")).Times(1);

    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 invalid-function", m_context));
}

TEST_F(CommandsTest, runShouldFailWithExecCommandIfInvalidFunctionParameters)
{
    ASSERT_FALSE(dut_cli::CommandLine::run("exec 0 driverInit --invalid-param", m_context));
}

TEST_F(CommandsTest, runShouldSucceedWithLoadCommand)
{
    const std::vector<std::string> commands {
        "exec 0 driverInit -t 2 -s 1024",
        "exec 0 driverRelease",
    };

    std::string fileContents;
    for (const auto& command : commands) {
        fileContents += command + "\n";
    }

    TempFile tempFile(testing::TempDir(), "dut");
    tempFile.write(fileContents);

    {
        InSequence sequence;

        EXPECT_CALL(m_console, cout(commands.at(0) + "\n")).Times(1);
        EXPECT_CALL(m_dutFactory, createInstanceProxy(0, m_connection, m_logger, true)).WillOnce(Return(m_dut0));
        EXPECT_CALL(*m_dut0, driverInit(false, dut::NvMemoryType::MEMORY_TYPE_FLASH, dut::NvMemorySize::MEMORY_SIZE_BYTES_1K, dut::Band::BAND_INVALID)).WillOnce(Return(true));
        EXPECT_CALL(m_console, cout(commands.at(1) + "\n")).Times(1);
        EXPECT_CALL(*m_dut0, driverRelease()).WillOnce(Return(true));
    }

    ASSERT_TRUE(dut_cli::CommandLine::run("load " + tempFile.getName(), m_context));
}

TEST_F(CommandsTest, runShouldFailWithLoadCommandIfFileNameNotSpecified)
{
    EXPECT_CALL(m_console, cerr("Exception: File not specified\n")).Times(1);

    ASSERT_FALSE(dut_cli::CommandLine::run("load", m_context));
}

TEST_F(CommandsTest, runShouldFailWithLoadCommandIfFileNotFound)
{
    std::string nonExistentFileName = "non-existent-file-name";

    EXPECT_CALL(m_console, cerr("Exception: Unable to open file '" + nonExistentFileName + "' for reading\n")).Times(1);

    ASSERT_FALSE(dut_cli::CommandLine::run("load " + nonExistentFileName, m_context));
}

TEST_F(CommandsTest, runShouldFailWithLoadCommandIfInvalidCommand)
{
    const std::vector<std::string> commands {
        "invalid-command",
    };

    std::string fileContents;
    for (const auto& command : commands) {
        fileContents += command + "\n";
    }

    TempFile tempFile(testing::TempDir(), "dut");
    tempFile.write(fileContents);

    {
        InSequence sequence;

        EXPECT_CALL(m_console, cout(commands.at(0) + "\n")).Times(1);
        EXPECT_CALL(m_console, cerr("Exception: Invalid command\n")).Times(1);
    }

    ASSERT_FALSE(dut_cli::CommandLine::run("load " + tempFile.getName(), m_context));
}

} // namespace
