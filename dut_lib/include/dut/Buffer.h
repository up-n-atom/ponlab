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

#ifndef _DUT_BUFFER_H_
#define _DUT_BUFFER_H_

#include <stdint.h>
#ifdef LINUX_HOST
#include <cstddef>
#endif

namespace dut {

/*
 * Interface to model a buffer class including data, size and length properties.
 */
class Buffer {
public:
    /**
     * @brief Class destructor
     */
    virtual ~Buffer() = default;

    /**
     * @brief Returns pointer to the raw data.
     *
     * @return address of raw data.
     */
    virtual const uint8_t* data() const = 0;

    /**
     * @brief  Returns pointer to the raw data.
     *
     * This is the non-const version of the method with the same name.
     *
     * @return address of raw data.
     */
    virtual uint8_t* data() { return const_cast<uint8_t*>(const_cast<const Buffer*>(this)->data()); }

    /**
     * @brief Returns the length of the buffer (number of bytes).
     *
     * @return length of buffer
     */
    const size_t& length() const { return m_length; }

    /**
     * @brief Returns the length of the buffer (number of bytes).
     *
     * This is the non-const version of the method with the same name.
     *
     * @return length of buffer
     */
    size_t& length() { return const_cast<size_t&>(const_cast<const Buffer*>(this)->length()); }

    /**
     * @brief Returns the size of the buffer.
     *
     * @return size of buffer
     */
    virtual size_t size() const = 0;

protected:
    /**
     * @brief Default class constructor
     */
    Buffer() = default;

    /**
     * @brief Class constructor
     */
    explicit Buffer(size_t length)
        : m_length(length)
    {
    }

    /**
     * Length of the buffer (number of bytes).
     */
    size_t m_length = 0;
};
}

#endif
