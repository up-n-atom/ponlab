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

#ifndef _DUT_BITS_H_
#define _DUT_BITS_H_

#include <stdint.h>
#ifdef LINUX_HOST
#include <cstddef>
#endif

#include <vector>

#define LOWER_NIBBLE(byte) (byte & 0x0f)
#define UPPER_NIBBLE(byte) ((byte >> 4) & 0x0f)

#define LOWER_BYTE(word) (word & 0x00ff)
#define UPPER_BYTE(word) ((word >> 8) & 0x00ff)

#define MAKE_BYTE(upper_nibble, lower_nibble) (static_cast<uint8_t>((upper_nibble & 0x0F) << 4) | (lower_nibble & 0x0F))
#define MAKE_WORD(upper_byte, lower_byte) (static_cast<uint16_t>((upper_byte & 0x00FF) << 8) | (lower_byte & 0x00FF))

namespace dut {

int8_t clipValueTo4Bit(int8_t value);
std::vector<int8_t> clipValuesTo4Bit(const int8_t* data, size_t length);

std::vector<uint8_t> getNibbleArray(const uint8_t* data, size_t size, size_t& offset, size_t numNibbles);
std::vector<uint8_t> setNibbleArray(const uint8_t* data, size_t length);

void parseField(void* dest, const uint8_t* buffer_, size_t size_, size_t& offset_, size_t length);

std::vector<uint8_t> convertValuesToDeltas(const uint16_t* data, size_t size, uint16_t initialValue, uint16_t valueStep);
std::vector<uint16_t> convertDeltasToValues(const uint8_t* data, size_t size, uint16_t initialValue, uint16_t valueStep);

template <typename Iterator>
void parseDeltaPoints(const uint8_t* buffer, size_t size, size_t& offset, size_t length, Iterator begin, Iterator end)
{
    auto nibbles = getNibbleArray(buffer, size, offset, length);
    for (auto it = begin; it != end; ++it) {
        size_t index = it - begin;
        uint8_t x = (index < length) ? nibbles[index] : 0;
        *it = (x > 0xf / 2) ? (x - 0xf - 1) : x;
    }
};

template <typename Container>
void parseDeltaPoints(const uint8_t* buffer, size_t size, size_t& offset, size_t length, Container& container)
{
    parseDeltaPoints(buffer, size, offset, length, std::begin(container), std::end(container));
};

}

#endif
