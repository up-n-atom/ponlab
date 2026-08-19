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

#include <exception>
#ifdef LINUX_HOST
#include <cmath>
#include <cstring>
#endif

namespace dut {

// Restrict the value to lie within the minimum and maximum range of a 4-bit value.
int8_t clipValueTo4Bit(int8_t value)
{
    if (value < -8) {
        return -8;
    } else if (value > 7) {
        return 7;
    } else {
        return value;
    }
}

std::vector<int8_t> clipValuesTo4Bit(const int8_t* data, size_t length)
{
    std::vector<int8_t> result(length);

    for (size_t i = 0; i < length; i++) {
        result[i] = clipValueTo4Bit(data[i]);
    }

    return result;
}

std::vector<uint8_t> getNibbleArray(const uint8_t* data, size_t size, size_t& offset, size_t numNibbles)
{
    size_t length = (numNibbles + (numNibbles % 2)) / 2;
    if (size < offset + length) {
        throw std::invalid_argument("Invalid data (cannot parse " + toString(length) + " bytes starting at offset " + toString(offset) + " from buffer with size " + toString(size) + " bytes)");
    }

    std::vector<uint8_t> result(numNibbles);

    for (size_t nibble = 0; nibble < numNibbles; nibble++) {
        uint8_t shift = 4 * (nibble % 2);
        result[nibble] = (data[offset] >> shift) & 0xf;
        offset += (nibble % 2);
    }
    offset += (numNibbles % 2); // When number of points is odd - need to skip to next byte

    return result;
}

// Each data item is stored in half a byte
std::vector<uint8_t> setNibbleArray(const uint8_t* data, size_t length)
{
    std::vector<uint8_t> result((length + 1) / 2);

    size_t offset = 0;

    for (size_t i = 0; i < length; i++) {
        uint8_t shift = 4 * (i % 2);
        result[offset] |= ((data[i] & 0xf) << shift);
        offset += (i % 2);
    }

    return result;
}

void parseField(void* dest, const uint8_t* buffer_, size_t size_, size_t& offset_, size_t length)
{
    if (!dest) {
        throw std::invalid_argument("Parameter 'dest' cannot be a null pointer");
    }

    if (size_ < offset_ + length) {
        throw std::invalid_argument("Invalid data (cannot parse " + toString(length) + " bytes starting at offset " + toString(offset_) + " from buffer with size " + toString(size_) + " bytes)");
    }

    memcpy(dest, &buffer_[offset_], length);
    offset_ += length;
}

std::vector<uint8_t> convertValuesToDeltas(const uint16_t* data, size_t size, uint16_t initialValue, uint16_t valueStep)
{
    // Each 2 consecutive items will later be written as the 2 nibbles of a byte, hence the
    // maximum value for each output value is 0xf.
    // We assume that the maximum delta between two input values (i.e.: calibrated frequencies)
    // will be less than 0xf * valueStep (20 or 40 MHz) so no overflow will occur.
    constexpr uint8_t maxDeltaValue = 0xf;

    std::vector<uint8_t> result(size);
    uint16_t previousValue = initialValue;
    uint16_t previousSteps = 0;
    uint8_t overflow = 0;

    for (size_t i = 0; i < size; i++) {

        uint16_t currentValue = data[i];

        if (currentValue < 16) {
            result[i] = static_cast<uint8_t>(currentValue);
            break;
        }

        if (currentValue < previousValue) {
            currentValue = previousValue;
        }

        auto currentSteps = static_cast<uint16_t>(std::round(((double)currentValue - initialValue) / valueStep));

        uint8_t currentDeltaValue = static_cast<uint8_t>(currentSteps - previousSteps) + overflow;

        if (currentDeltaValue > maxDeltaValue) {
            overflow = currentDeltaValue - maxDeltaValue;
            result[i] = maxDeltaValue;
        } else {
            result[i] = currentDeltaValue;
            overflow = 0;
        }

        previousSteps = currentSteps;
        previousValue = currentValue;
    }

    return result;
}

std::vector<uint16_t> convertDeltasToValues(const uint8_t* data, size_t size, uint16_t initialValue, uint16_t valueStep)
{
    std::vector<uint16_t> result(size);
    uint16_t previousValue = initialValue;

    for (size_t i = 0; i < size; i++) {
        uint16_t currentValue = previousValue + valueStep * data[i];
        result[i] = currentValue;
        previousValue = currentValue;
    }

    return result;
}

}
