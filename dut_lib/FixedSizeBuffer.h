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

#ifndef _DUT_FIXED_SIZE_BUFFER_H_
#define _DUT_FIXED_SIZE_BUFFER_H_

#include "dut/Buffer.h"

#include <array>

namespace dut {

/*
 * A fixed size buffer is a buffer which size is specified at creation time and cannot be changed. 
 */
template <size_t SIZE>
class FixedSizeBuffer : public Buffer {
public:
    /**
     * @brief Returns pointer to the raw data.
     *
     * @return address of raw data.
     */
    const uint8_t* data() const override
    {
        return m_data.data();
    }

    /**
     * @brief  Returns pointer to the raw data.
     *
     * This is the non-const version of the method with the same name.
     *
     * @return address of raw data.
     */
    uint8_t* data() override { return const_cast<uint8_t*>(const_cast<const FixedSizeBuffer*>(this)->data()); }

    /**
     * @brief Returns the size of the buffer.
     *
     * @return size of buffer
     */
    size_t size() const override
    {
        return m_data.size();
    }

    /**
     * @brief Clears the buffer.
     * 
     * The buffer is emptied.
     */
    void clear()
    {
        m_data = {};
        m_length = 0;
    }

private:
    /**
     * Buffer data.
     */
    std::array<uint8_t, SIZE> m_data {};
};
}

#endif
