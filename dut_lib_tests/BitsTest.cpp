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

#include "Bits.h"

#include "dut/Tools.h"

#include <gtest/gtest.h>

namespace {

constexpr uint16_t initialFreq = 1000;
constexpr uint16_t freqStep = 10;

void checkResult(const std::vector<uint8_t>& actualResult, const std::vector<uint8_t>& expectedResult)
{
    for (size_t i = 0; i < actualResult.size(); i++) {
        EXPECT_EQ(actualResult.at(i), expectedResult.at(i)) << "Actual result '" << +actualResult.at(i) << "' does not match expected result '" << +expectedResult.at(i) << "' at index #" << std::to_string(i);
    }
}

void checkResult(const std::vector<uint16_t>& actualResult, const std::vector<uint16_t>& expectedResult)
{
    for (size_t i = 0; i < actualResult.size(); i++) {
        EXPECT_EQ(actualResult.at(i), expectedResult.at(i)) << "Actual result '" << actualResult.at(i) << "' does not match expected result '" << expectedResult.at(i) << "' at index #" << std::to_string(i);
    }
}

TEST(BitsTest, convertValuesToDeltasShouldSucceed)
{
    std::vector<uint16_t> buffer { 1000, 1010, 1030, 1060 };

    std::vector<uint8_t> result = dut::convertValuesToDeltas(buffer.data(), buffer.size(), initialFreq, freqStep);

    const std::vector<uint8_t> expectedResult { 0, 1, 2, 3 };
    checkResult(result, expectedResult);
}

TEST(BitsTest, convertValuesToDeltasShouldSucceedWithValueLessThanInitial)
{
    std::vector<uint16_t> buffer { 999, 1000 };

    std::vector<uint8_t> result = dut::convertValuesToDeltas(buffer.data(), buffer.size(), initialFreq, freqStep);

    const std::vector<uint8_t> expectedResult { 0, 0 };
    checkResult(result, expectedResult);
}

TEST(BitsTest, convertValuesToDeltasShouldSucceedWhenNoOverflow)
{
    std::vector<uint16_t> buffer { 1000, 1000 + 15 * freqStep, 1000 + 17 * freqStep };

    std::vector<uint8_t> result = dut::convertValuesToDeltas(buffer.data(), buffer.size(), initialFreq, freqStep);

    const std::vector<uint8_t> expectedResult { 0, 15, 2 };
    checkResult(result, expectedResult);
}
}

TEST(BitsTest, convertValuesToDeltasShouldSucceedWhenOverflow)
{
    std::vector<uint16_t> buffer { 1000, 1000 + 16 * freqStep, 1000 + 17 * freqStep };

    std::vector<uint8_t> result = dut::convertValuesToDeltas(buffer.data(), buffer.size(), initialFreq, freqStep);

    const std::vector<uint8_t> expectedResult { 0, 15, 2 };
    checkResult(result, expectedResult);
}

TEST(BitsTest, convertDeltasToValuesShouldSucceed)
{
    std::vector<uint8_t> buffer { 0, 1, 2, 3 };

    std::vector<uint16_t> result = dut::convertDeltasToValues(buffer.data(), buffer.size(), initialFreq, freqStep);

    const std::vector<uint16_t> expectedResult { 1000, 1010, 1030, 1060 };
    checkResult(result, expectedResult);
}
