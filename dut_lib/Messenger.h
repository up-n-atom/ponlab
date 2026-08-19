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

#ifndef _DUT_MESSENGER_H_
#define _DUT_MESSENGER_H_

#include <chrono>
#include <memory>

namespace dut {

class Connection;
class Logger;

/**
 * @brief Exchanges messages with the DUT server.
 * 
 * The messenger class sends requests to the DUT server and waits for the corresponding response, 
 * using the connection instance provided as parameter to the constructor.
 * Messages exchanged are formatted using the DUT protocol and traced using the provided logger.
 */
class Messenger {
public:
    /**
     * @brief Class constructor
     * 
     * @param connection Connection to send and receive messages.
     * @param logger Logger to trace messages.
     */
    Messenger(std::shared_ptr<Connection> connection, std::shared_ptr<Logger> logger);

    /**
     * @brief Sends a request to the DUT server and waits for the response.
     * 
     * Request and response are formatted using the DUT protocol.
     * Throws timeout exception if no response is received within the time specified. 
     * Throws exception on any other error condition.
     * 
     * @param[in] wlanIndex Index of the WiFi card (0, 2, 4).
     * @param[in] msgId Message identifier (see dutDriverMessagesIdEnum_t).
     * @param[in] request Contents of the request message.
     * @param[in] requestLength Length of the request message.
     * @param[in,out] response Buffer to hold the response message.
     * @param[in,out] responseLength On input, size of the response message. On output, length of the response message.
     * @param[in] timeout Timeout in milliseconds to wait for the response. Set to 0 to not wait for a response.
     */
    void sendReceive(uint8_t wlanIndex, uint8_t msgId, const uint8_t* request, size_t requestLength, uint8_t* response, size_t& responseLength, const std::chrono::milliseconds& timeout) const;

private:
    /**
     * Connection instance to exchange messages with the DUT server.
     */
    std::shared_ptr<Connection> m_connection;

    /**
     * Logger instance to trace exchanged messages.
     */
    std::shared_ptr<Logger> m_logger;
};
}

#endif
