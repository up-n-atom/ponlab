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

#ifndef _DUT_CIS_H_
#define _DUT_CIS_H_

#include <memory>
#include <stdint.h>
#include <vector>

namespace dut {

constexpr uint8_t cisCodeInvalid = 0x00;
constexpr uint8_t cisCodeEof = 0xFF;

class Buffer;

// CIS version 6
class Cis {
public:
    Cis(uint8_t code, uint16_t length, const uint8_t* data);

    uint8_t getCode() const;
    uint16_t getLength() const;
    uint8_t* getData() const;

    friend bool operator<(const Cis& left, const Cis& right)
    {
        return left.getCode() < right.getCode();
    }

    std::unique_ptr<Buffer> serialize() const;
    static Cis parse(const uint8_t* buffer, size_t size, size_t& offset);

    class CodeComparator {
    public:
        explicit CodeComparator(uint8_t code)
            : m_code(code)
        {
        }
        inline bool operator()(const Cis& cis) const { return cis.getCode() == m_code; }

    private:
        uint8_t m_code = cisCodeInvalid;
    };

private:
    uint8_t m_code = cisCodeInvalid;
    uint16_t m_length = 0;
    std::shared_ptr<uint8_t> m_data;
};

}

#endif
