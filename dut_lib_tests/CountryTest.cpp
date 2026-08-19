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

#include "dut/Country.h"

#include "dut/Tools.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::ThrowsMessage;

namespace {

TEST(CountryTest, getCountryByNameShouldSucceed)
{
    std::string shortName = "ES";
    dut::CountryInfo countryInfo;

    EXPECT_NO_THROW(countryInfo = dut::getCountryByName(shortName));
    EXPECT_STREQ(countryInfo.fullName, "SPAIN");
}

TEST(CountryTest, getCountryByNameShouldFailIfCountryNotFound)
{
    std::string shortName = "?";

    const std::string expectedMessage = "Country '" + shortName + "' not found";
    EXPECT_THAT([&]() { dut::getCountryByName(shortName); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST(CountryTest, getCountryByCodeShouldSucceed)
{
    uint8_t code = 207;
    dut::CountryInfo countryInfo;

    EXPECT_NO_THROW(countryInfo = dut::getCountryByCode(code));
    EXPECT_STREQ(countryInfo.fullName, "SPAIN");
}

TEST(CountryTest, getCountryByCodeShouldFailIfCountryNotFound)
{
    uint8_t code = UINT8_MAX;

    const std::string expectedMessage = "Country '" + dut::toString(code) + "' not found";
    EXPECT_THAT([&]() { dut::getCountryByCode(code); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST(CountryTest, getCountryByIndexShouldSucceed)
{
    size_t count = dut::getCountryCount();
    size_t index = 0;
    while (index < count) {
        EXPECT_NO_THROW(dut::getCountryByIndex(index));
        index++;
    }
}

TEST(CountryTest, getCountryByIndexShouldFailIfInvalidIndex)
{
    size_t count = dut::getCountryCount();

    EXPECT_THROW(dut::getCountryByIndex(count), std::out_of_range);
}

} // namespace
