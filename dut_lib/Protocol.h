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

#ifndef _DUT_PROTOCOL_H_
#define _DUT_PROTOCOL_H_

#include <stdint.h>
#ifdef LINUX_HOST
#include <cstddef>
#endif

namespace dut {

/**
 * @brief DUT protocol serializing and parsing functions. 
 * 
 * The communication between the DUT client and the DUT server uses a very simple proprietary protocol in which messages have this structure: 
 * - There are 2 signature bytes, with values 'M' and 'T' which stand for Metalink
 * - 1 byte for the version, which is set to one and not used by the DUT server at this moment.
 * - A 4-bit field containing the index of the WiFi card the message is intended for.
 * - A 4-bit field containing the message identifier
 * - A 4-byte data length, containing the length of the variable data field (in little endian)
 * - And the payload data, which contents depend on the message id.
 */
namespace Protocol {
    bool buildFrame(uint8_t wlanIndex, uint8_t msgId, const uint8_t* data, size_t dataLength, uint8_t* frame, size_t& frameLength);
    bool parseFrame(const uint8_t* frame, size_t frameLength, uint8_t& msgId, uint8_t* data, size_t& dataLength);
}
}

#endif
