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

#ifndef _DUT_ANTENNA_MASK_H_
#define _DUT_ANTENNA_MASK_H_

#include "dut/API.h"

#include <stdint.h>

namespace dut {

/**
 * Type of the underlying variable that holds the actual value of the bit mask.
 * If more than eight bits are ever required for an antenna mask, just define this type accordingly.
 */
using AntennaMaskType = uint8_t;
typedef uint8_t AntennaMask_t;

class DUT_API AntennaMask {
public:
    /**
     * @brief Default class constructor.
     * 
     * Initializes antenna mask to 0 (all bits clear).
     */
    AntennaMask()
        : AntennaMask(0)
    {
    }

    /**
     * @brief Converting constructor.
     * 
     * This constructor is used when assigning an integral value to an instance of the class.
     * 
     * @param value Value of the antenna mask
     */
    // cppcheck-suppress noExplicitConstructor
    AntennaMask(long int value)
        : m_value(static_cast<AntennaMaskType>(value))
    {
    }

    /**
     * @brief Function call operator () overloaded with no parameters.
     * 
     * Used to assign the value of an antenna mask to an integral type variable. E.g.: uint8_t value = antennaMask;
     * 
     * @return Value of the antenna mask.
     */
    operator AntennaMaskType() const
    {
        return m_value;
    }

    /**
     * @brief Checks if the bit of the antenna mask at the specified position is set or not.
     * 
     * @param position Position of the bit in the mask.
     * @return true if the bit is set and false otherwise.
     */
    bool isBitSet(uint8_t position) const
    {
        return (position < totalBits()) && (m_value & (1 << position)) != 0;
    }

    /**
     * @brief Gets the number of bits that are set to 1 in the antenna mask.
     * 
     * E.g.: bitCount(0x10) = 1
     * 
     * @return Number of bits set to 1 in the mask.
     */
    uint8_t bitCount() const
    {
        // Counting bits set, Brian Kernighan's way
        // http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan

        uint16_t value = m_value;
        uint8_t count = 0;

        while (value != 0) {
            count++;
            value &= value - 1;
        }

        return count;
    }

    /**
     * Gets the length of the antenna mask by computing the position of the highest bit set to 1.
     * 
     * E.g.: length(0x10) = 5
     * 
     * @return Length of the antenna mask.
     */
    uint8_t length() const
    {
        uint8_t length = 0;
        uint8_t count = bitCount();

        while (count != 0) {
            if (isBitSet(length)) {
                count--;
            }
            length++;
        }

        return length;
    }

    /**
     * @brief Checks if this antenna mask is valid against the specified mask.
     * 
     * An antenna mask A is valid against a second mask B if all the bits set in mask A are also
     * set in mask B and invalid otherwise. Put in other words, the bits set to 1 in mask B are
     * the only bits that can be set in mask A.

     * @param validityMask Mask containing valid bits.
     * @return true if this mask is valid against the specified mask.
     */
    bool isValid(const AntennaMask& validityMask) const
    {
        return ((m_value | validityMask) == validityMask);
    }

    /**
     * @brief Gets the size in bytes of the antenna mask.
     * 
     * @return Size in bytes of the underlying data type of the mask.
     */
    static uint8_t size()
    {
        return sizeof(AntennaMaskType);
    }

    /**
     * @brief Gets the size in bits of the antenna mask.
     * 
     * The value returned is equal to 8 times the size of the underlying data type used to hold
     * the mask.
     * 
     * @return Size in bits of the underlying data type of the mask.
     */
    static uint8_t totalBits()
    {
        return size() * 8;
    }

private:
    /**
     * Value of the mask.
     */
    AntennaMaskType m_value;
};

}

#endif
