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

#include "Cis.h"

#include "dut/Buffer.h"

#include <gtest/gtest.h>

namespace {

TEST(CisTest, cisConstructorShouldSucceed)
{
    const char* p = "Hello, World!";

    uint8_t code = 1;
    uint16_t length = static_cast<uint16_t>(strlen(p)) + 1;
    auto data = reinterpret_cast<const uint8_t*>(p);

    dut::Cis cis(code, length, data);

    EXPECT_EQ(cis.getCode(), code);
    EXPECT_EQ(cis.getLength(), length);
    EXPECT_EQ(memcmp(cis.getData(), data, length), 0);
    EXPECT_NE(cis.getData(), data);
}

TEST(CisTest, serializeShouldSucceed)
{
    const char* p = "Hello, World!";

    uint8_t code = 1;
    uint16_t length = static_cast<uint16_t>(strlen(p)) + 1;
    auto data = reinterpret_cast<const uint8_t*>(p);

    dut::Cis cis(code, length, data);

    auto buffer = cis.serialize();
    uint8_t* serializedData = buffer->data();
    size_t serializedLength = buffer->length();

    EXPECT_EQ(serializedLength, sizeof(code) + sizeof(length) + length);

    size_t offset = 0;
    EXPECT_EQ(serializedData[offset++], code);
    EXPECT_EQ(serializedData[offset++], length & 0xff);
    EXPECT_EQ(serializedData[offset++], length >> 8);
    EXPECT_EQ(memcmp(&serializedData[offset], data, length), 0);
}

TEST(CisTest, parseCisShouldSucceed)
{
    const char* p = "Hello, World!";

    uint8_t code = 1;
    uint16_t length = static_cast<uint16_t>(strlen(p)) + 1;
    auto data = reinterpret_cast<const uint8_t*>(p);

    dut::Cis cis(code, length, data);

    auto buffer = cis.serialize();
    const uint8_t* serializedData = buffer->data();
    size_t serializedLength = buffer->length();

    size_t offset = 0;
    dut::Cis parsedCis = dut::Cis::parse(serializedData, serializedLength, offset);

    EXPECT_EQ(parsedCis.getCode(), code);
    EXPECT_EQ(parsedCis.getLength(), length);
    EXPECT_EQ(memcmp(parsedCis.getData(), data, length), 0);
    EXPECT_EQ(offset, sizeof(code) + sizeof(length) + length);
}

TEST(CisTest, parseCisShouldFailWithInvalidLength)
{
    const char* p = "Hello, World!";

    uint8_t code = 1;
    uint16_t length = static_cast<uint16_t>(strlen(p)) + 1;
    auto data = reinterpret_cast<const uint8_t*>(p);

    dut::Cis cis(code, length, data);

    auto buffer = cis.serialize();
    const uint8_t* serializedData = buffer->data();
    size_t serializedLength = buffer->length();

    size_t offset = 0;
    size_t invalidLength = serializedLength - 1;
    EXPECT_THROW(dut::Cis::parse(serializedData, invalidLength, offset), std::invalid_argument);
}

}