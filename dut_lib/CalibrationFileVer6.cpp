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

#include "CalibrationFileVer6.h"

#include "ResizableBuffer.h"
#include "RssiCalibrationDataVer6.h"
#include "TssiCalibrationDataVer6.h"

namespace dut {

CalibrationFileVer6::CalibrationFileVer6(const uint8_t* data, size_t length)
    : CalibrationFileImpl(data, length)
{
}

std::unique_ptr<RssiCalibrationData> CalibrationFileVer6::createRssiCalibrationData()
{
    return std::make_unique<RssiCalibrationDataVer6>();
}

std::unique_ptr<TssiCalibrationData> CalibrationFileVer6::createTssiCalibrationData()
{
    return std::make_unique<TssiCalibrationDataVer6>();
}

std::unique_ptr<Buffer> CalibrationFileVer6::getRssiCalibrationDataBytes(const AntennaMask& antennaMask)
{
    std::vector<Cis> cisList = getCisList(cisCodeRssi);

    for (const auto& cis : cisList) {
        if (cis.getLength() > 0) {
            size_t offset = 0;

            RssiCalibrationDataVer6 rssiCalibrationData;
            rssiCalibrationData.parse(cis.getData(), cis.getLength(), offset);

            if (rssiCalibrationData.data().antennaMask == antennaMask) {
                auto buffer = std::make_unique<ResizableBuffer>(cis.getLength());
                buffer->append(cis.getData(), cis.getLength());

                return buffer;
            }
        }
    }

    return nullptr;
}

}