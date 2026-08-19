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

#include "Protocol.h"

#include <stdexcept>
#include <string>

#ifdef LINUX_HOST
#include <cstring>
#endif

namespace dut {

namespace Protocol {
    constexpr uint8_t version = 1;
    constexpr uint8_t headerLength = 8;
    constexpr uint8_t signatureIndex = 0;
    constexpr uint8_t versionIndex = 2;
    constexpr uint8_t msgIdIndex = 3;
    constexpr uint8_t lengthIndex = 4;
    constexpr uint8_t dataIndex = headerLength;
    constexpr uint8_t lengthSize = 4;

    bool buildFrame(uint8_t wlanIndex, uint8_t msgId, const uint8_t* data, size_t dataLength, uint8_t* frame, size_t& frameLength)
    {
        if ((dataLength > 0) && (!data)) {
            throw std::invalid_argument("Parameter 'data' cannot be a null pointer");
        }

        size_t requiredLength = dataLength + headerLength;
        if (frameLength < requiredLength) {
            frameLength = requiredLength;
            return false;
        }

        if (!frame) {
            throw std::invalid_argument("Parameter 'frame' cannot be a null pointer");
        }

        // Fill in header
        frame[signatureIndex] = 'M';
        frame[signatureIndex + 1] = 'T';
        frame[versionIndex] = version;
        frame[msgIdIndex] = static_cast<uint8_t>(((wlanIndex & 0xF) << 4)) | (msgId & 0xF);

        // Copy message length at offset 4
        memcpy(&frame[lengthIndex], &dataLength, lengthSize);

        // Copy message data at offset 8
        memcpy(&frame[dataIndex], data, dataLength);

        frameLength = requiredLength;
        return true;
    }

    bool parseFrame(const uint8_t* frame, size_t frameLength, uint8_t& msgId, uint8_t* data, size_t& dataLength)
    {
        msgId = 0x00;

        if (!frame) {
            throw std::invalid_argument("Parameter 'frame' cannot be a null pointer");
        }

        if (frameLength == 0) {
            throw std::invalid_argument("Parameter 'frameLength' cannot be 0");
        }

        // Check number of bytes received is larger than the header size (8)
        if (frameLength < headerLength) {
            return false;
        }

        // Check message received starts with proper header 'MT'
        if ((frame[signatureIndex] != 'M') || (frame[signatureIndex + 1] != 'T')) {
            throw std::invalid_argument("The specified frame does not contain a valid signature");
        }

        // Check message received is a response (MSB set)
        if ((frame[msgIdIndex] & 0x80) == 0) {
            throw std::invalid_argument("The specified frame is not a valid response (message ID is " + std::to_string(frame[msgIdIndex]) + ")");
        }

        size_t length = 0;
        memcpy(&length, &frame[lengthIndex], lengthSize);

        // Check if frame is complete (i.e.: all bytes specified in header have been received)
        if (frameLength < headerLength + length) {
            return false;
        }

        msgId = frame[msgIdIndex] & ~0x80;

        if (dataLength < length) {
            dataLength = length;
            return false;
        }

        if (length > 0) {
            if (!data) {
                throw std::invalid_argument("Parameter 'data' cannot be a null pointer");
            }

            dataLength = length;
            memcpy(data, &frame[dataIndex], dataLength);
        }

        return true;
    }

}
}
