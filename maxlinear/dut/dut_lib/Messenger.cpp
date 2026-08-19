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

#include "Messenger.h"

#include "Exceptions.h"
#include "Protocol.h"
#include "dut/Connection.h"
#include "dut/Logger.h"
#include "dut/Tools.h"

#include <array>
#include <stdexcept>
#include <string>

namespace dut {

constexpr uint8_t txVoltageNumSamples = 5;

Messenger::Messenger(std::shared_ptr<Connection> connection, std::shared_ptr<Logger> logger)
    : m_connection(connection)
    , m_logger(logger)
{
}

void Messenger::sendReceive(uint8_t wlanIndex, uint8_t msgId, const uint8_t* request, size_t requestLength, uint8_t* response, size_t& responseLength, const std::chrono::milliseconds& timeout) const
{
    // Size of the buffer used to serialize and parse protocol frames.
    constexpr size_t frameSize = 4 * 1024;

    // Buffer used to serialize and parse protocol frames.
    std::array<uint8_t, frameSize> frame;

    // Length of the frame
    size_t frameLength;

    // Build and send frame
    {
        frameLength = frame.size();

        if (!Protocol::buildFrame(wlanIndex, msgId, request, requestLength, frame.data(), frameLength)) {
            throw std::invalid_argument("The specified request length (" + toString(requestLength) + ") is too large (" + toString(frameLength) + " bytes allowed)");
        }

        m_logger->log(LogLevel::LOG_LEVEL_TRACE, toString({ { "wlanIndex", toString(wlanIndex) }, { "msgId", toHexString(msgId) } }));
        m_logger->log(LogLevel::LOG_LEVEL_TRACE, toString(frame.data(), frameLength, ">>"));

        m_connection->send(frame.data(), frameLength);
    }

    // Do not wait for response frame if no timeout was specified
    if (timeout == std::chrono::milliseconds::zero()) {
        return;
    }

    // Receive and parse frame
    uint8_t responseId = 0x00;
    auto startTime = std::chrono::steady_clock::now();

    frameLength = 0;
    while (responseId == 0x00) {
        size_t bytesReceived = 0;

        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime);
        if (elapsedTime < timeout) {
            auto receiveTimeout = timeout - elapsedTime;
            bytesReceived = m_connection->receive(&frame[frameLength], frame.size() - frameLength, receiveTimeout);
        }

        if (bytesReceived == 0) {
            throw dut::timeout_exception();
        }

        frameLength += bytesReceived;

        if ((!Protocol::parseFrame(frame.data(), frameLength, responseId, response, responseLength)) && (responseId != 0x00)) {
            throw std::invalid_argument("The specified response length (" + toString(responseLength) + ") is too small (" + toString(frameLength) + " bytes received)");
        }
    }

    m_logger->log(LogLevel::LOG_LEVEL_TRACE, toString({ { "responseId", toHexString(responseId) } }));
    m_logger->log(LogLevel::LOG_LEVEL_TRACE, toString(frame.data(), frameLength, "<<"));

    if (msgId != responseId) {
        throw std::runtime_error("Invalid response received: expected message ID is " + toHexString(msgId) + ", received message ID is " + toHexString(responseId));
    }
}

}