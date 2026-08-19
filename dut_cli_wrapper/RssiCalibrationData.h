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
value struct RssiAB {
    float a;
    float b;
};

public
value struct RssiS2D {
    uint8_t gain;
    uint8_t offset;
};

public
ref class RssiCalibrationData : public CalibrationData {
};

public
ref class RssiCalibrationDataVer6 : public RssiCalibrationData {
public:
    static constexpr int32_t maxNumRegions = 3;
    static constexpr int32_t maxRxRfFlatnessPoints = 11;
    static constexpr int32_t maxRxRssiFlatnessPoints = 7;

    ref class AntennaData {
    public:
        uint8_t lnaMidGain = 0;
        array<float> ^ lnaGains = gcnew array<float>(dut::maxNumLnaGainStepsVer6);
        array<int8_t> ^ midGainDeltaPoints = gcnew array<int8_t>(maxRxRfFlatnessPoints);
        array<int8_t> ^ rxRfFlatnessDeltaPointsHighGain = gcnew array<int8_t>(maxRxRfFlatnessPoints);
        array<int8_t> ^ rxRfFlatnessDeltaPointsBypass = gcnew array<int8_t>(maxRxRfFlatnessPoints);
        array<RssiS2D> ^ s2dGainOffset = gcnew array<RssiS2D>(maxNumRegions);
        array<RssiAB> ^ pointsAB = gcnew array<RssiAB>(maxNumRegions);
        array<int8_t> ^ rxRssiFlatnessDeltaPoints = gcnew array<int8_t>(maxRxRssiFlatnessPoints);
        array<uint16_t> ^ subBandsCrossingPoints = gcnew array<uint16_t>(dut::maxNumLnaSubBandsCrossingPointsVer6);
        uint8_t firstCrossingPointIndex = 0;
    };

    AntennaMask ^ antennaMask = gcnew AntennaMask();
    uint16_t startFreq = 0;
    uint16_t stopFreq = 0;
    uint16_t calibrationFreq = 0;
    uint8_t chipTemperature = 0;
    uint8_t numRxRfFlatnessPoints = 0; // M
    uint8_t numRxRssiFlatnessPoints = 0; // L
    array<uint16_t> ^ rxRfFlatnessFreqs = gcnew array<uint16_t>(maxRxRfFlatnessPoints);
    array<uint16_t> ^ rxRssiFlatnessFreqs = gcnew array<uint16_t>(maxRxRssiFlatnessPoints);

    array<AntennaData ^> ^ antennas = gcnew array<AntennaData ^>(dut::maxNumRxAntennas);
};

public
ref class RssiCalibrationDataVer7 : public RssiCalibrationData {
public:
    static constexpr int32_t maxNumRegions = 2;
    static constexpr int32_t maxRxRfFlatnessPoints = 11;
    static constexpr int32_t maxRxRssiFlatnessPoints = 7;

    ref class AntennaData {
    public:
        array<float> ^ lnaGains = gcnew array<float>(dut::maxNumLnaGainStepsVer7);
        array<int8_t> ^ rxRfFlatnessDeltaPointsHighGain = gcnew array<int8_t>(maxRxRfFlatnessPoints);
        array<int8_t> ^ rxRfFlatnessDeltaPointsLowGain = gcnew array<int8_t>(maxRxRfFlatnessPoints);
        array<int8_t> ^ rxRfFlatnessDeltaPointsBypass = gcnew array<int8_t>(maxRxRfFlatnessPoints);
        array<RssiS2D> ^ s2dGainOffset = gcnew array<RssiS2D>(maxNumRegions);
        array<RssiAB> ^ pointsAB = gcnew array<RssiAB>(maxNumRegions);
        array<int8_t> ^ rxRssiFlatnessDeltaPoints = gcnew array<int8_t>(maxRxRssiFlatnessPoints);
        array<uint16_t> ^ subBandsCrossingPoints = gcnew array<uint16_t>(dut::maxNumLnaSubBandsCrossingPointsVer7);
        uint8_t firstCrossingPointIndex = 0;
    };

    AntennaMask ^ antennaMask = gcnew AntennaMask();
    uint16_t startFreq = 0;
    uint16_t stopFreq = 0;
    uint16_t calibrationFreq = 0;
    uint8_t chipTemperature = 0;
    uint8_t numRxRfFlatnessPoints = 0; // M
    uint8_t numRxRssiFlatnessPoints = 0; // L
    array<uint16_t> ^ rxRfFlatnessFreqs = gcnew array<uint16_t>(maxRxRfFlatnessPoints);
    array<uint16_t> ^ rxRssiFlatnessFreqs = gcnew array<uint16_t>(maxRxRssiFlatnessPoints);

    array<AntennaData ^> ^ antennas = gcnew array<AntennaData ^>(dut::maxNumRxAntennas);
};

}