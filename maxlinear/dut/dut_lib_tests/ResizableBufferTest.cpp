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

#include "ResizableBuffer.h"

#include "Bits.h"

#include <gmock/gmock.h>

namespace {

TEST(ResizableBufferTest, appendShouldSucceedWithUint8)
{
    dut::ResizableBuffer buffer;

    uint16_t value = 0xdead;
    buffer.append(UPPER_BYTE(value));
    buffer.append(LOWER_BYTE(value));

    ASSERT_EQ(buffer.length(), sizeof(value));
    ASSERT_EQ(buffer.data()[0], 0xde);
    ASSERT_EQ(buffer.data()[1], 0xad);
}

TEST(ResizableBufferTest, appendShouldSucceedWithUint16)
{
    dut::ResizableBuffer buffer;

    uint16_t value = 0xdead;
    buffer.append(&value, sizeof(value));

    ASSERT_EQ(buffer.length(), sizeof(value));
    ASSERT_EQ(buffer.data()[0], 0xad);
    ASSERT_EQ(buffer.data()[1], 0xde);
}

} // namespace
