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

#ifndef _DUT_NON_VOLATILE_MEMORY_H_
#define _DUT_NON_VOLATILE_MEMORY_H_

#include "dut/Types.h"

#include <string>

namespace dut {

class NonVolatileMemory {
public:
    static constexpr size_t nvmVersionAddress = 0x28;

    virtual ~NonVolatileMemory() = default;

    virtual NvMemoryType getType() const = 0;
    virtual bool setType(NvMemoryType memoryType) = 0;

    virtual size_t getSize() const = 0;
    virtual bool setSize(NvMemorySize memorySize) = 0;

    virtual const uint8_t* getData() const = 0;

    virtual void load() = 0;
    virtual void loadFromFile(const std::string& fileName) = 0;
    virtual void saveToFile(const std::string& fileName) = 0;

    virtual uint8_t getVersion() const = 0;

    virtual void read(size_t address, uint8_t* data, size_t length, bool useCache = true) = 0;
    virtual void write(size_t address, const uint8_t* data, size_t length) = 0;
};
}

#endif
