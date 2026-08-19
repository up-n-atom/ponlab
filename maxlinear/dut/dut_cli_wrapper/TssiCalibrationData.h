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

#pragma once

#include "CalibrationData.h"

#include "AntennaMask.h"
#include "Types.h"

namespace dut_cli {

public
value struct TssiAB {
    short a;
    short b;
};

public
ref class TssiCalibrationData : public CalibrationData {
public:
    static constexpr int32_t maxNumRegions = 3;
};

public
ref class TssiCalibrationDataVer6 : public TssiCalibrationData {
public:
    ref class AntennaData {
    public:
        array<TssiAB> ^ pointsAB = gcnew array<TssiAB>(maxNumRegions);
        array<int32_t> ^ s2dGain = gcnew array<int32_t>(maxNumRegions);
        array<int32_t> ^ s2dOffset = gcnew array<int32_t>(maxNumRegions);
        uint8_t maxPower;
        uint8_t uEvm;
        uint8_t uEvmGain;
    };

    uint16_t channel;
    Band band;

    array<AntennaData ^> ^ antennas = gcnew array<AntennaData ^>(dut::maxNumTxAntennas);

    AntennaMask ^ antennaMask = gcnew AntennaMask();
    uint8_t numRegions = 0;
    Bandwidth bw = Bandwidth::BANDWIDTH_INVALID;
};

public
ref class TssiCalibrationDataVer7 : public TssiCalibrationDataVer6 {
};

public
value struct TpcFreqVer6 {
    uint32_t distance;
    TssiCalibrationDataVer6 ^ data;
};

}