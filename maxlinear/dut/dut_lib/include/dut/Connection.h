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

#ifndef _DUT_CONNECTION_H_
#define _DUT_CONNECTION_H_

#include "dut/API.h"

#include <chrono>
#include <stdint.h>

namespace dut {

/**
 * Interface to model the connection established between a DUT client and the DUT server.
 * Includes methods to send and receive data which throw an exception on error.
 */
class DUT_API Connection {
public:
    /**
     * @brief Class destructor
     */
    virtual ~Connection() = default;

    /**
     * @brief Sends a message to the server.
     * 
     * @param buffer Array of bytes with data to send.
     * @param length Number of bytes to send.
     */
    virtual void send(const uint8_t* buffer, size_t length) = 0;

    /**
     * @brief Receives a message from the server. 
     * 
     * Throws exception on error or if peer closed the connection.
     * 
     * @param buffer Array of bytes to hold received data.
     * @param length Size of the buffer to hold received data.
     * @param timeout Time in milliseconds to wait before data was received. 
     * @return Number of bytes received or 0 if timeout.
     */
    virtual size_t receive(uint8_t* buffer, size_t length, const std::chrono::milliseconds& timeout) = 0;
};
}

#endif