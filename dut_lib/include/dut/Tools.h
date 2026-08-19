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

#ifndef _DUT_TOOLS_H_
#define _DUT_TOOLS_H_

#include "dut/Types.h"

#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace dut {

std::ostream& operator<<(std::ostream& out, const Band& value);

std::ostream& operator<<(std::ostream& out, const Bandwidth& value);

std::ostream& operator<<(std::ostream& out, const CalibrationFileVersion& value);

std::ostream& operator<<(std::ostream& out, const CalibrationFileSubversion& value);

std::ostream& operator<<(std::ostream& out, const ChipModule& value);

std::ostream& operator<<(std::ostream& out, const FemType& value);

std::ostream& operator<<(std::ostream& out, const Gi& value);

std::ostream& operator<<(std::ostream& out, const Ltf& value);

std::ostream& operator<<(std::ostream& out, const Mcs& value);

std::ostream& operator<<(std::ostream& out, const NvMemorySize& value);

std::ostream& operator<<(std::ostream& out, const NvMemoryType& value);

std::ostream& operator<<(std::ostream& out, const PhyMode& value);

std::ostream& operator<<(std::ostream& out, const RegulationType& value);

std::ostream& operator<<(std::ostream& out, const VersionedComponent& value);

std::ostream& operator<<(std::ostream& out, const RssiAB_t& value);

std::ostream& operator<<(std::ostream& out, const CorrelationResults_t& value);

std::ostream& operator<<(std::ostream& out, const BeamformingHeaderInfo_t& value);

std::ostream& operator<<(std::ostream& out, const BeamformingFilePathSet_t& value);

std::ostream& operator<<(std::ostream& stream, uint8_t byte);

std::vector<uint8_t> toBytes(const std::string& hex);

template <typename T>
std::string toHexString(const T& value)
{
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2)
           << std::hex << value;
    return stream.str();
}

std::string toHexString(uint8_t byte);

template <typename T>
std::string toString(const T& value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

std::string toString(bool value);

std::string toString(int8_t byte);

std::string toString(uint8_t byte);

std::string toString(float value, uint8_t precision = 4);

std::string toString(const uint8_t* data, size_t length, const char* name = nullptr);

std::string toString(const std::vector<std::pair<std::string, std::string>>& pairs, const std::string& pairsSeparator = ";", const std::string& valuesSeparator = ":");

std::string toString(const ChipID& value);

template <typename It>
std::string toString(It begin, It end)
{
    std::ostringstream os;
    std::stringstream stream;

    It current = begin;
    while (current != end) {
        if (!stream.str().empty()) {
            stream << ",";
        }
        stream << toString(*current);

        current++;
    }

    os << "[" << stream.str() << "]";
    return os.str();
}

uint16_t lq_round(double num);

}

#endif
