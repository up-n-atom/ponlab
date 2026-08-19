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

#include "dut/AntennaMask.h"

#include "dut/Tools.h"

#include <gtest/gtest.h>

namespace {

TEST(AntennaMaskTest, isBitSetTest)
{
    dut::AntennaMask mask(0);
    for (int j = 0; j < dut::AntennaMask::totalBits(); j++) {
        EXPECT_FALSE(mask.isBitSet(j));
    }

    for (int i = 0; i < dut::AntennaMask::totalBits(); i++) {
        mask = (1 << i);
        for (int j = 0; j < dut::AntennaMask::totalBits(); j++) {
            if (i == j) {
                EXPECT_TRUE(mask.isBitSet(j));
            } else {
                EXPECT_FALSE(mask.isBitSet(j));
            }
        }
    }
}

TEST(AntennaMaskTest, bitCountTest)
{
    dut::AntennaMask mask(0);
    EXPECT_EQ(0, mask.bitCount());

    for (int i = 0; i < dut::AntennaMask::totalBits(); i++) {
        mask = mask + (1 << i);
        EXPECT_EQ(i + 1, mask.bitCount());
    }

    for (int i = 0; i < dut::AntennaMask::totalBits(); i++) {
        mask = (1 << i);
        EXPECT_EQ(1, mask.bitCount());
    }
}

TEST(AntennaMaskTest, lengthTest)
{
    dut::AntennaMask mask(0);
    EXPECT_EQ(0, mask.length());

    for (int i = 0; i < dut::AntennaMask::totalBits(); i++) {
        mask = mask + (1 << i);
        EXPECT_EQ(i + 1, mask.length());
    }

    for (int i = 0; i < dut::AntennaMask::totalBits(); i++) {
        mask = (1 << i);
        EXPECT_EQ(i + 1, mask.length());
    }
}

TEST(AntennaMaskTest, isValidTest)
{
    dut::AntennaMask validityMask(0xb);

    std::vector<dut::AntennaMask> validMasks { 0x1, 0x2, 0x8, 0xb };
    std::vector<dut::AntennaMask> invalidMasks { 0x4, 0x5, 0x10 };

    for (const auto& mask : validMasks) {
        EXPECT_TRUE(mask.isValid(validityMask)) << "Mask " << dut::toHexString(mask) << " is not valid";
    }

    for (const auto& mask : invalidMasks) {
        EXPECT_FALSE(mask.isValid(validityMask)) << "Mask " << dut::toHexString(mask) << " is valid";
    }
}

TEST(AntennaMaskTest, sizeTest)
{
    dut::AntennaMask mask(0);
    EXPECT_EQ(1, mask.size());
}
}