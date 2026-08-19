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

#ifndef _DUT_RESIZABLE_BUFFER_H_
#define _DUT_RESIZABLE_BUFFER_H_

#include "dut/Buffer.h"

namespace dut {

/*
 * A resizable buffer is a buffer that can grow in size. 
 * The buffer grows explicitly by calling resize() or automatically when needed after calling the 
 * append() method (i.e. if there is not enough room in the buffer to hold the new data). 
 */
class ResizableBuffer : public Buffer {
public:
    /**
     * @brief Class constructor with optional initial size.
     * 
     * @param initialSize Number of bytes to initially allocate for the buffer. 
     */
    explicit ResizableBuffer(size_t initialSize = 0);

    /**
     * @brief Class destructor
     */
    ~ResizableBuffer() override;

    /**
     * @brief Copy constructor
     *
     * Delete copy constructor to avoid having multiple copies of the resources
     */
    ResizableBuffer(const ResizableBuffer&) = delete;

    /**
     * @brief Move constructor.
     *
     * A move constructor allows the resources owned by an rvalue object to be moved into an
     * lvalue without creating its copy.
     */
    ResizableBuffer(ResizableBuffer&& obj) noexcept
        : Buffer(obj.m_length)
        , m_size(obj.m_size)
        , m_data(obj.m_data)
    {
        obj.m_size = 0;
        obj.m_length = 0;
        obj.m_data = nullptr;
    }

    /**
     * @brief Assignment operator
     *
     * Delete assignment operator to avoid having multiple copies of the resources
     */
    ResizableBuffer& operator=(const ResizableBuffer&) = delete;

    /**
     * @brief Move assignment operator
     *
     * The move assignment operator is used to transfer ownership of resources
     */
    ResizableBuffer& operator=(ResizableBuffer&& obj) noexcept
    {
        // Self-assignment detection
        if (&obj == this) {
            return *this;
        }

        // Transfer ownership
        this->m_size = obj.m_size;
        this->m_length = obj.m_length;
        this->m_data = obj.m_data;

        return *this;
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
     * @brief Returns the size of the buffer.
     *
     * @return size of buffer
     */
    size_t size() const override
    {
        return m_size;
    }

    /**
     * @brief Resizes the buffer to the specified new size.
     * 
     * @param newSize New size of the buffer. 
     */
    void resize(size_t newSize);

    /**
     * @brief Appends another buffer to this buffer.
     * 
     * Buffer is resized to hold the specified buffer data only if there is not enough room for it.
     * 
     * @param buffer Other buffer to append to this buffer. 
     */
    size_t append(const Buffer& buffer);

    /**
     * @brief Appends a new chunk of data to the buffer.
     * 
     * Buffer is resized to hold the specified data only if there is not enough room for it.
     * 
     * @param data Data to append to the buffer. 
     * @param length Length of the data to append to the buffer. 
     */
    size_t append(const void* data, size_t length);

    /**
     * @brief Appends a new byte to the buffer.
     * 
     * Buffer is resized to hold the specified byte only if there is not enough room for it.
     * 
     * @param byte Byte to append to the buffer. 
     */
    size_t append(uint8_t byte);

    /**
     * @brief Clears the buffer.
     * 
     * The buffer is emptied but not resized (no memory is deallocated).
     */
    void clear();

private:
    /**
     * Size of the buffer (number of allocated bytes).
     */
    size_t m_size = 0;

    /**
     * Pointer to buffer data.
     */
    uint8_t* m_data = nullptr;
};
}

#endif
