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

#include "DutMessage.h"

#include <stdexcept>
#include <string>

#ifdef LINUX_HOST
#include <cstring>
#endif

namespace dut {
DutMessage::DutMessage(uint8_t wlanIndex, const Payload& payload)
    : Message(payload)
{
    if (payload.length() > m_payloadSize) {
        throw std::invalid_argument("The specified payload is too large (" + std::to_string(payload.length()) + " bytes do not fit into message)");
    }

    m_message.msgLength = static_cast<uint16_t>(payload.length());
    m_message.status = 0;
    m_message.wlanIndex = wlanIndex;
    m_message.msgId = 0;

    m_length = m_headerLength + payload.length();
    memcpy(m_message.data, payload.data(), payload.length());
}

DutMessage::~DutMessage()
{
    memcpy(m_payload.data(), m_message.data, m_payload.length());
}

const uint8_t* DutMessage::data() const
{
    return reinterpret_cast<const uint8_t*>(&m_message);
}

size_t DutMessage::size() const
{
    return sizeof(m_message);
}

uint32_t DutMessage::getMsgId()
{
    return static_cast<uint32_t>(m_message.msgId);
}

void DutMessage::setMsgId(uint32_t msgId)
{
    m_message.msgId = static_cast<uint16_t>(msgId);
}

size_t DutMessage::getHeaderLength()
{
    return m_headerLength;
}

size_t DutMessage::getPayloadLength()
{
    return m_message.msgLength;
}

size_t DutMessage::getPayloadSize()
{
    return m_payloadSize;
}

}
