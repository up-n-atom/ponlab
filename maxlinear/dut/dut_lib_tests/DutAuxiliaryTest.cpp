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

#include "dut/DutImpl.h"

#include "dut/ConnectionImpl.h"

#include <gtest/gtest.h>

namespace {
constexpr uint8_t wlanIndex = 0x02;

TEST(DutAuxiliaryTest, constructorShouldFailWithNullConnection)
{
    std::shared_ptr<dut::Connection> connection;
    auto logger = std::make_shared<dut::NullLogger>();

    EXPECT_THROW(dut::DutImpl(wlanIndex, connection, logger), std::invalid_argument);
}

TEST(DutAuxiliaryTest, constructorShouldFailWithNullClient)
{
    std::shared_ptr<dut::Client> client;
    auto logger = std::make_shared<dut::NullLogger>();

    EXPECT_THROW(dut::DutImpl(client, logger), std::invalid_argument);
}

TEST(DutAuxiliaryTest, constructorShouldFailWithNullLogger)
{
    auto connection = std::make_shared<dut::ConnectionImpl>();
    std::shared_ptr<dut::Logger> logger;

    EXPECT_THROW(dut::DutImpl(wlanIndex, connection, logger), std::invalid_argument);
}

TEST(DutAuxiliaryTest, throwExceptionsShouldSucceed)
{
    auto connection = std::make_shared<dut::ConnectionImpl>();
    auto logger = std::make_shared<dut::NullLogger>();
    bool throwExceptions = true;
    dut::DutImpl dut(wlanIndex, connection, logger, throwExceptions);

    EXPECT_THROW(dut.driverRelease(), std::exception);
    EXPECT_EQ(dut.getLastError(), "Not initialized");
}

TEST(DutAuxiliaryTest, setThrowExceptionsShouldSucceed)
{
    auto connection = std::make_shared<dut::ConnectionImpl>();
    auto logger = std::make_shared<dut::NullLogger>();
    dut::DutImpl dut(wlanIndex, connection, logger);

    EXPECT_FALSE(dut.driverRelease());
    EXPECT_EQ(dut.getLastError(), "Not initialized");

    dut.setThrowExceptions(true);

    EXPECT_THROW(dut.driverRelease(), std::exception);
    EXPECT_EQ(dut.getLastError(), "Not initialized");
}

}
