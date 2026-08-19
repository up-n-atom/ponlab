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

#include "DutDriverFwGeneralMsg.h"

#include <stdexcept>
#include <string>
#ifdef LINUX_HOST
#include <cstring>
#endif

namespace dut {
DutDriverFwGeneralMsg::DutDriverFwGeneralMsg(const Payload& payload)
    : Message(payload)
{
    if (payload.length() > m_payloadSize) {
        throw std::invalid_argument("The specified payload is too large (" + std::to_string(payload.length()) + " bytes do not fit into message)");
    }

    m_message.umiReqId = 0;
    m_message.umiLen = static_cast<uint16_t>(payload.length());

    m_length = m_headerLength + payload.length();
    memcpy(m_message.param, payload.data(), payload.length());
}

DutDriverFwGeneralMsg::~DutDriverFwGeneralMsg()
{
    memcpy(m_payload.data(), m_message.param, m_payload.length());
}

const uint8_t* DutDriverFwGeneralMsg::data() const
{
    return reinterpret_cast<const uint8_t*>(&m_message);
}

size_t DutDriverFwGeneralMsg::size() const
{
    return sizeof(m_message);
}

uint32_t DutDriverFwGeneralMsg::getMsgId()
{
    return m_message.umiReqId;
}

void DutDriverFwGeneralMsg::setMsgId(uint32_t msgId)
{
    m_message.umiReqId = msgId;
}

size_t DutDriverFwGeneralMsg::getHeaderLength()
{
    return m_headerLength;
}

size_t DutDriverFwGeneralMsg::getPayloadLength()
{
    return m_message.umiLen;
}

size_t DutDriverFwGeneralMsg::getPayloadSize()
{
    return m_payloadSize;
}

}
