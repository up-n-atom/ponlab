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

#include "ResizableBuffer.h"
#include "dut/Tools.h"

#ifdef LINUX_HOST
#include <cstring>
#endif

namespace dut {

Cis::Cis(uint8_t code, uint16_t length, const uint8_t* data)
    : m_code(code)
    , m_length(length)
{
    if (length > 0) {
        void* p = malloc(length);
        if (p) {
            memcpy(p, data, length);
        }
        m_data.reset(static_cast<uint8_t*>(p), free);
    }
}

uint8_t Cis::getCode() const
{
    return m_code;
}

uint16_t Cis::getLength() const
{
    return m_length;
}

uint8_t* Cis::getData() const
{
    return m_data.get();
}

std::unique_ptr<Buffer> Cis::serialize() const
{
    size_t size = sizeof(m_code) + sizeof(m_length) + m_length;

    auto buffer = std::make_unique<ResizableBuffer>(size);

    buffer->append(&m_code, sizeof(m_code));
    buffer->append(&m_length, sizeof(m_length));
    if (m_length > 0) {
        buffer->append(m_data.get(), m_length);
    }

    return buffer;
}

Cis Cis::parse(const uint8_t* buffer, size_t size, size_t& offset)
{
    if (!buffer) {
        throw std::invalid_argument("Parameter 'buffer' cannot be a null pointer");
    }

    uint8_t code = cisCodeInvalid;
    uint16_t length = 0;
    const uint8_t* data = nullptr;

    if (size < offset + sizeof(code)) {
        throw std::invalid_argument("Invalid data (CIS code not found)");
    }

    code = buffer[offset];
    offset++;

    if ((code != cisCodeInvalid) && (code != cisCodeEof)) {

        if (size < offset + sizeof(length)) {
            throw std::invalid_argument("Invalid data (CIS length not found)");
        }

        memmove(&length, &buffer[offset], sizeof(length));
        offset += sizeof(length);

        if (size < offset + length) {
            throw std::invalid_argument("Invalid data (CIS data not found)");
        }

        data = &buffer[offset];
        offset += length;
    }

    return Cis(code, length, data);
}

}
