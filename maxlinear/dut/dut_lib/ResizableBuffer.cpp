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

#include <stdexcept>

#ifdef LINUX_HOST
#include <cstring>
#endif

namespace dut {

ResizableBuffer::ResizableBuffer(size_t initialSize)
{
    resize(initialSize);
}

ResizableBuffer::~ResizableBuffer()
{
    free(m_data);
}

void ResizableBuffer::resize(size_t newSize)
{
    if (newSize > m_size) {
        void* ptr = realloc(m_data, newSize);
        if (!ptr) {
            throw std::bad_alloc();
        }

        m_data = static_cast<uint8_t*>(ptr);
        m_size = newSize;
        std::fill_n(m_data + m_length, m_size - m_length, 0);
    }
}

size_t ResizableBuffer::append(const Buffer& buffer)
{
    return append(buffer.data(), buffer.length());
}

size_t ResizableBuffer::append(const void* data, size_t length)
{
    if (data && (length > 0)) {
        size_t requiredSize = m_length + length;
        if (requiredSize > m_size) {
            size_t newSize = std::max(requiredSize, m_size * 2);
            resize(newSize);
        }

        memcpy(&m_data[m_length], data, length);
        m_length += length;
    }

    return m_length;
}

size_t ResizableBuffer::append(uint8_t byte)
{
    return append(&byte, sizeof(byte));
}

void ResizableBuffer::clear()
{
    std::fill_n(m_data, m_size, 0);
    m_length = 0;
}

}
