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

#include "Application.h"
#include "ConsoleMock.h"
#include "dut/ConnectionMock.h"
#include "dut/DutMock.h"

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::StrictMock;

namespace {

class ApplicationTest : public ::testing::Test {
public:
    ApplicationTest()
    {
        ON_CALL(m_console, eof()).WillByDefault(Return(false));
    }

    std::shared_ptr<StrictMock<dut::ConnectionMock>> m_connection = std::make_shared<StrictMock<dut::ConnectionMock>>();
    std::shared_ptr<dut::Logger> m_logger = std::make_shared<dut::NullLogger>();

    dut_cli::Config m_config;
    dut_cli::ConsoleMock m_console;
    dut::DutMockFactory m_dutFactory;

    dut_cli::Context m_context = dut_cli::Context(m_config, m_console, m_dutFactory, m_connection, m_logger);
    dut_cli::Application m_application = dut_cli::Application(m_context);
};

TEST_F(ApplicationTest, applicationShouldSucceedWithEof)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_console, cout("> ")).Times(1);
        EXPECT_CALL(m_console, cin()).WillOnce(Return(""));
        EXPECT_CALL(m_console, eof()).WillOnce(Return(true));
    }

    ASSERT_TRUE(m_application.run());
}

TEST_F(ApplicationTest, applicationShouldSucceedWithExitCommand)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_console, cout("> ")).Times(1);
        EXPECT_CALL(m_console, cin()).WillOnce(Return("exit"));
    }

    ASSERT_TRUE(m_application.run());
}

TEST_F(ApplicationTest, applicationShouldIgnoreSpacesInCommands)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_console, cout("> ")).Times(1);
        EXPECT_CALL(m_console, cin()).WillOnce(Return(" exit "));
    }

    ASSERT_TRUE(m_application.run());
}

TEST_F(ApplicationTest, applicationShouldIgnoreEmptyLines)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_console, cout("> ")).Times(1);
        EXPECT_CALL(m_console, cin()).WillOnce(Return(""));
        EXPECT_CALL(m_console, cout("> ")).Times(1);
        EXPECT_CALL(m_console, cin()).WillOnce(Return("exit"));
    }

    ASSERT_TRUE(m_application.run());
}

TEST_F(ApplicationTest, applicationShouldIgnoreComments)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_console, cout("> ")).Times(1);
        EXPECT_CALL(m_console, cin()).WillOnce(Return("# This is a comment"));
        EXPECT_CALL(m_console, cout("> ")).Times(1);
        EXPECT_CALL(m_console, cin()).WillOnce(Return("exit"));
    }

    ASSERT_TRUE(m_application.run());
}

TEST_F(ApplicationTest, applicationShouldFailOnException)
{
    {
        InSequence sequence;

        EXPECT_CALL(m_console, cout("> ")).Times(1);
        EXPECT_CALL(m_console, cin()).WillOnce(Return("invalid-command"));
        EXPECT_CALL(m_console, cerr("Exception: Invalid command\n")).Times(1);
    }

    m_config.failOnException = true;

    dut_cli::Application application(m_context);

    ASSERT_FALSE(application.run());
}

} // namespace
