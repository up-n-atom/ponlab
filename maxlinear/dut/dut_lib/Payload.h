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

#ifndef _DUT_PAYLOAD_H_
#define _DUT_PAYLOAD_H_

#include "dut/Buffer.h"

namespace dut {

/*
 * A payload buffer is a buffer to hold the payload of a message exchanged with server. 
 * The buffer data must be provided as a pointer to a message structure.
 */
class Payload : public Buffer {
public:
    /**
     * @brief Class constructor.
     * 
     * @param data Pointer to the data (message structure). 
     * @param length Length of the data. 
     */
    Payload(uint8_t* data, size_t length)
        : Buffer(length)
        , m_data(data)
    {
    }

    /**
     * @brief Returns pointer to the raw data.
     *
     * @return address of raw data.
     */
    const uint8_t* data() const override
    {
        return m_data;
    }

    /**
     * @brief  Returns pointer to the raw data.
     *
     * This is the non-const version of the method with the same name.
     *
     * @return address of raw data.
     */
    uint8_t* data() override { return const_cast<uint8_t*>(const_cast<const Payload*>(this)->data()); }

    /**
     * @brief Returns the size of the message.
     *
     * @return size of message
     */
    size_t size() const override
    {
        return length();
    }

private:
    uint8_t* m_data = nullptr;
};
}

#endif
