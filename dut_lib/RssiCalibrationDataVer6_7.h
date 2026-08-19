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

#ifndef _DUT_RSSI_CALIBRATION_DATA_VER6_7_H_
#define _DUT_RSSI_CALIBRATION_DATA_VER6_7_H_

#include "RssiCalibrationData.h"

#include "dut/AntennaMask.h"
#include "dut/Types.h"

#include <array>
#include <vector>

namespace dut {
class DUT_API RssiCalibrationDataVer6_7 : public RssiCalibrationData {
public:
    static constexpr size_t rxFlatnessFreqStep = 40;
    static constexpr size_t rxSubBandsFreqStep = 20;

    using RssiCalibrationData::RssiCalibrationData;

    void serialize(std::shared_ptr<ResizableBuffer> buffer) override;
    void parse(const uint8_t* buffer, size_t size, size_t& offset) override;

protected:
    void serializeCommonParameters(std::shared_ptr<ResizableBuffer> buffer);
    void parseCommonParameters(const uint8_t* buffer, size_t size, size_t& offset);

    virtual void serializeAntennaParameters(uint8_t antenna, std::shared_ptr<ResizableBuffer> buffer) = 0;
    virtual void parseAntennaParameters(uint8_t antenna, const uint8_t* buffer, size_t size, size_t& offset) = 0;
};
}

#endif
