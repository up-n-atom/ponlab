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

/*
Overview of Marshaling in C++ / CLI
https://docs.microsoft.com/en-us/cpp/dotnet/overview-of-marshaling-in-cpp?view=msvc-160

How to : Extend the Marshaling Library
https://docs.microsoft.com/en-us/cpp/dotnet/how-to-extend-the-marshaling-library?view=msvc-160
*/

#include <msclr/marshal_cppstd.h>

#include "RssiCalibrationData.h"
#include "TssiCalibrationData.h"

#include <array>
#include <chrono>

namespace dut_cli {

template <class T, size_t SIZE>
    cli::array<T> ^ marshal_as_array(const std::array<T, SIZE>& from) {
        auto result = gcnew cli::array<T>(static_cast<int>(from.size()));

        for (int i = 0; i < result->Length; i++) {
            result[i] = from[i];
        }

        return result;
    }

    template <class T>
    cli::array<T> ^ marshal_as_array(const T* from, size_t size) {
        auto result = gcnew cli::array<T>(static_cast<int>(size));

        for (int i = 0; i < result->Length; i++) {
            result[i] = from[i];
        }

        return result;
    }

    template <typename T>
    struct array_deleter {
    void operator()(T const* p)
    {
        delete[] p;
    }
};

template <class T>
std::shared_ptr<T> create_shared_ptr_to_array(size_t size)
{
    T* p = new T[size];

    std::shared_ptr<T> result(p, array_deleter<T>());
    return result;
}

template <class T>
std::shared_ptr<T> marshal_as_shared_ptr_to_array(cli::array<T> ^ from)
{
    auto result = create_shared_ptr_to_array<T>(from->Length);

    T* to = result.get();
    for (int i = 0; i < from->Length; i++) {
        to[i] = from[i];
    }

    return result;
}

template <class T, size_t SIZE>
void copy_array(cli::array<T> ^ from, std::array<T, SIZE>& to)
{
    for (int i = 0; i < min(SIZE, from->Length); i++) {
        to[i] = from[i];
    }
}

}

namespace msclr {
namespace interop {
    template <>
    inline std::chrono::milliseconds marshal_as<std::chrono::milliseconds, uint32_t>(const uint32_t& from)
    {
        return std::chrono::milliseconds(from);
    }

    template <>
    inline dut_cli::RssiAB marshal_as<dut_cli::RssiAB, dut::RssiAB_t>(const dut::RssiAB_t& from)
    {
        return dut_cli::RssiAB { from.a, from.b };
    }

    template <>
    inline dut_cli::TssiAB marshal_as<dut_cli::TssiAB, dut::TssiAB_t>(const dut::TssiAB_t& from)
    {
        return dut_cli::TssiAB { from.a, from.b };
    }

    template <>
    inline dut_cli::RssiS2D marshal_as<dut_cli::RssiS2D, dut::RssiS2D_t>(const dut::RssiS2D_t& from)
    {
        return dut_cli::RssiS2D { from.gain, from.offset };
    }

    template <>
        inline dut_cli::RssiCalibrationDataVer6::AntennaData ^ marshal_as<dut_cli::RssiCalibrationDataVer6::AntennaData ^, dut::RssiAntennaDataVer6_t>(const dut::RssiAntennaDataVer6_t& from)
    {
        auto result = gcnew dut_cli::RssiCalibrationDataVer6::AntennaData();
        result->lnaMidGain = from.lnaMidGain;
        result->lnaGains = dut_cli::marshal_as_array<float>(from.lnaGains, dut::maxNumLnaGainStepsVer6);
        result->midGainDeltaPoints = dut_cli::marshal_as_array<int8_t>(from.midGainDeltaPoints, dut::maxRxRfFlatnessPoints);
        result->rxRfFlatnessDeltaPointsHighGain = dut_cli::marshal_as_array<int8_t>(from.rxRfFlatnessDeltaPointsHighGain, dut::maxRxRfFlatnessPoints);
        result->rxRfFlatnessDeltaPointsBypass = dut_cli::marshal_as_array<int8_t>(from.rxRfFlatnessDeltaPointsBypass, dut::maxRxRfFlatnessPoints);
        result->s2dGainOffset = gcnew cli::array<dut_cli::RssiS2D>(static_cast<int>(dut::maxNumRxRegionsVer6));
        for (int i = 0; i < result->s2dGainOffset->Length; i++) {
            result->s2dGainOffset[i] = msclr::interop::marshal_as<dut_cli::RssiS2D>(from.s2dGainOffset[i]);
        }
        result->pointsAB = gcnew cli::array<dut_cli::RssiAB>(static_cast<int>(dut::maxNumRxRegionsVer6));
        for (int i = 0; i < result->pointsAB->Length; i++) {
            result->pointsAB[i] = msclr::interop::marshal_as<dut_cli::RssiAB>(from.pointsAB[i]);
        }
        result->rxRssiFlatnessDeltaPoints = dut_cli::marshal_as_array<int8_t>(from.rxRssiFlatnessDeltaPoints, dut::maxRxRssiFlatnessPoints);
        result->subBandsCrossingPoints = dut_cli::marshal_as_array<uint16_t>(from.subBandsCrossingPoints, dut::maxNumLnaSubBandsCrossingPointsVer6);
        result->firstCrossingPointIndex = from.firstCrossingPointIndex;

        return result;
    }

    template <>
        inline dut_cli::RssiCalibrationDataVer6 ^ marshal_as<dut_cli::RssiCalibrationDataVer6 ^, dut::RssiCalibrationData_t>(const dut::RssiCalibrationData_t& from)
    {
        auto result = gcnew dut_cli::RssiCalibrationDataVer6();
        result->antennaMask = gcnew dut_cli::AntennaMask(from.antennaMask);
        result->startFreq = from.startFreq;
        result->stopFreq = from.stopFreq;
        result->calibrationFreq = from.calibrationFreq;
        result->chipTemperature = from.chipTemperature;
        result->numRxRfFlatnessPoints = from.numRxRfFlatnessPoints;
        result->numRxRssiFlatnessPoints = from.numRxRssiFlatnessPoints;
        result->rxRfFlatnessFreqs = dut_cli::marshal_as_array<uint16_t>(from.rxRfFlatnessFreqs, dut::maxRxRfFlatnessPoints);
        result->rxRssiFlatnessFreqs = dut_cli::marshal_as_array<uint16_t>(from.rxRssiFlatnessFreqs, dut::maxRxRssiFlatnessPoints);

        result->antennas = gcnew cli::array<dut_cli::RssiCalibrationDataVer6::AntennaData ^>(static_cast<int>(dut::maxNumRxAntennas));
        for (int i = 0; i < result->antennas->Length; i++) {
            const auto& antennaData = from.antennaData.antennasVer6[i];
            result->antennas[i] = msclr::interop::marshal_as<dut_cli::RssiCalibrationDataVer6::AntennaData ^>(antennaData);
        }

        return result;
    }

    template <>
        inline cli::array<dut_cli::RssiCalibrationDataVer6 ^> ^ marshal_as<cli::array<dut_cli::RssiCalibrationDataVer6 ^> ^, std::vector<dut::RssiCalibrationData_t>>(const std::vector<dut::RssiCalibrationData_t>& from)
    {
        auto result = gcnew cli::array<dut_cli::RssiCalibrationDataVer6 ^>(static_cast<int>(from.size()));
        for (int i = 0; i < from.size(); i++) {
            result[i] = msclr::interop::marshal_as<dut_cli::RssiCalibrationDataVer6 ^>(from[i]);
        }

        return result;
    }

    template <>
        inline dut_cli::RssiCalibrationDataVer7::AntennaData ^ marshal_as<dut_cli::RssiCalibrationDataVer7::AntennaData ^, dut::RssiAntennaDataVer7_t>(const dut::RssiAntennaDataVer7_t& from)
    {
        auto result = gcnew dut_cli::RssiCalibrationDataVer7::AntennaData();
        result->lnaGains = dut_cli::marshal_as_array<float>(from.lnaGains, dut::maxNumLnaGainStepsVer7);
        result->rxRfFlatnessDeltaPointsHighGain = dut_cli::marshal_as_array<int8_t>(from.rxRfFlatnessDeltaPointsHighGain, dut::maxRxRfFlatnessPoints);
        result->rxRfFlatnessDeltaPointsLowGain = dut_cli::marshal_as_array<int8_t>(from.rxRfFlatnessDeltaPointsLowGain, dut::maxRxRfFlatnessPoints);
        result->rxRfFlatnessDeltaPointsBypass = dut_cli::marshal_as_array<int8_t>(from.rxRfFlatnessDeltaPointsBypass, dut::maxRxRfFlatnessPoints);
        result->s2dGainOffset = gcnew cli::array<dut_cli::RssiS2D>(static_cast<int>(dut::maxNumRxRegionsVer7));
        for (int i = 0; i < result->s2dGainOffset->Length; i++) {
            result->s2dGainOffset[i] = msclr::interop::marshal_as<dut_cli::RssiS2D>(from.s2dGainOffset[i]);
        }
        result->pointsAB = gcnew cli::array<dut_cli::RssiAB>(static_cast<int>(dut::maxNumRxRegionsVer7));
        for (int i = 0; i < result->pointsAB->Length; i++) {
            result->pointsAB[i] = msclr::interop::marshal_as<dut_cli::RssiAB>(from.pointsAB[i]);
        }
        result->rxRssiFlatnessDeltaPoints = dut_cli::marshal_as_array<int8_t>(from.rxRssiFlatnessDeltaPoints, dut::maxRxRssiFlatnessPoints);
        result->subBandsCrossingPoints = dut_cli::marshal_as_array<uint16_t>(from.subBandsCrossingPoints, dut::maxNumLnaSubBandsCrossingPointsVer7);
        result->firstCrossingPointIndex = from.firstCrossingPointIndex;

        return result;
    }

    template <>
        inline dut_cli::RssiCalibrationDataVer7 ^ marshal_as<dut_cli::RssiCalibrationDataVer7 ^, dut::RssiCalibrationData_t>(const dut::RssiCalibrationData_t& from)
    {
        auto result = gcnew dut_cli::RssiCalibrationDataVer7();
        result->antennaMask = gcnew dut_cli::AntennaMask(from.antennaMask);
        result->startFreq = from.startFreq;
        result->stopFreq = from.stopFreq;
        result->calibrationFreq = from.calibrationFreq;
        result->chipTemperature = from.chipTemperature;
        result->numRxRfFlatnessPoints = from.numRxRfFlatnessPoints;
        result->numRxRssiFlatnessPoints = from.numRxRssiFlatnessPoints;
        result->rxRfFlatnessFreqs = dut_cli::marshal_as_array<uint16_t>(from.rxRfFlatnessFreqs, dut::maxRxRfFlatnessPoints);
        result->rxRssiFlatnessFreqs = dut_cli::marshal_as_array<uint16_t>(from.rxRssiFlatnessFreqs, dut::maxRxRssiFlatnessPoints);

        result->antennas = gcnew cli::array<dut_cli::RssiCalibrationDataVer7::AntennaData ^>(static_cast<int>(dut::maxNumRxAntennas));
        for (int i = 0; i < result->antennas->Length; i++) {
            const auto& antennaData = from.antennaData.antennasVer7[i];
            result->antennas[i] = msclr::interop::marshal_as<dut_cli::RssiCalibrationDataVer7::AntennaData ^>(antennaData);
        }

        return result;
    }

    template <>
        inline cli::array<dut_cli::RssiCalibrationDataVer7 ^> ^ marshal_as<cli::array<dut_cli::RssiCalibrationDataVer7 ^> ^, std::vector<dut::RssiCalibrationData_t>>(const std::vector<dut::RssiCalibrationData_t>& from)
    {
        auto result = gcnew cli::array<dut_cli::RssiCalibrationDataVer7 ^>(static_cast<int>(from.size()));
        for (int i = 0; i < from.size(); i++) {
            result[i] = msclr::interop::marshal_as<dut_cli::RssiCalibrationDataVer7 ^>(from[i]);
        }

        return result;
    }

    template <>
        inline dut_cli::TssiCalibrationDataVer6::AntennaData ^ marshal_as<dut_cli::TssiCalibrationDataVer6::AntennaData ^, dut::TssiAntennaData_t>(const dut::TssiAntennaData_t& from)
    {
        auto result = gcnew dut_cli::TssiCalibrationDataVer6::AntennaData();
        result->pointsAB = gcnew cli::array<dut_cli::TssiAB>(static_cast<int>(dut::maxNumTxRegions));
        for (int i = 0; i < result->pointsAB->Length; i++) {
            result->pointsAB[i] = msclr::interop::marshal_as<dut_cli::TssiAB>(from.pointsAB[i]);
        }
        result->s2dGain = dut_cli::marshal_as_array<int32_t>(from.s2dGain, dut::maxNumTxRegions);
        result->s2dOffset = dut_cli::marshal_as_array<int32_t>(from.s2dOffset, dut::maxNumTxRegions);
        result->maxPower = from.maxPower;
        result->uEvm = from.uEvm;
        result->uEvmGain = from.uEvmGain;

        return result;
    }

    template <>
        inline dut_cli::TssiCalibrationDataVer6 ^ marshal_as<dut_cli::TssiCalibrationDataVer6 ^, dut::TssiCalibrationData_t>(const dut::TssiCalibrationData_t& from)
    {
        auto result = gcnew dut_cli::TssiCalibrationDataVer6();
        result->channel = from.channel;
        result->band = static_cast<dut_cli::Band>(from.band);
        result->antennas = gcnew cli::array<dut_cli::TssiCalibrationDataVer6::AntennaData ^>(static_cast<int>(dut::maxNumRxAntennas));
        for (int i = 0; i < result->antennas->Length; i++) {
            result->antennas[i] = msclr::interop::marshal_as<dut_cli::TssiCalibrationDataVer6::AntennaData ^>(from.antennas[i]);
        }
        result->antennaMask = gcnew dut_cli::AntennaMask(from.antennaMask);
        result->numRegions = from.numRegions;
        result->bw = static_cast<dut_cli::Bandwidth>(from.bw);

        return result;
    }

    template <>
        inline cli::array<dut_cli::TssiCalibrationDataVer6 ^> ^ marshal_as<cli::array<dut_cli::TssiCalibrationDataVer6 ^> ^, std::vector<dut::TssiCalibrationData_t>>(const std::vector<dut::TssiCalibrationData_t>& from)
    {
        auto result = gcnew cli::array<dut_cli::TssiCalibrationDataVer6 ^>(static_cast<int>(from.size()));
        for (int i = 0; i < from.size(); i++) {
            result[i] = msclr::interop::marshal_as<dut_cli::TssiCalibrationDataVer6 ^>(from[i]);
        }

        return result;
    }

    template <>
        inline dut_cli::TssiCalibrationDataVer7 ^ marshal_as<dut_cli::TssiCalibrationDataVer7 ^, dut::TssiCalibrationData_t>(const dut::TssiCalibrationData_t& from)
    {
        auto result = gcnew dut_cli::TssiCalibrationDataVer7();
        result->channel = from.channel;
        result->band = static_cast<dut_cli::Band>(from.band);
        result->antennas = gcnew cli::array<dut_cli::TssiCalibrationDataVer7::AntennaData ^>(static_cast<int>(dut::maxNumRxAntennas));
        for (int i = 0; i < result->antennas->Length; i++) {
            result->antennas[i] = msclr::interop::marshal_as<dut_cli::TssiCalibrationDataVer7::AntennaData ^>(from.antennas[i]);
        }
        result->antennaMask = gcnew dut_cli::AntennaMask(from.antennaMask);
        result->numRegions = from.numRegions;
        result->bw = static_cast<dut_cli::Bandwidth>(from.bw);

        return result;
    }

    template <>
        inline cli::array<dut_cli::TssiCalibrationDataVer7 ^> ^ marshal_as<cli::array<dut_cli::TssiCalibrationDataVer7 ^> ^, std::vector<dut::TssiCalibrationData_t>>(const std::vector<dut::TssiCalibrationData_t>& from)
    {
        auto result = gcnew cli::array<dut_cli::TssiCalibrationDataVer7 ^>(static_cast<int>(from.size()));
        for (int i = 0; i < from.size(); i++) {
            result[i] = msclr::interop::marshal_as<dut_cli::TssiCalibrationDataVer7 ^>(from[i]);
        }

        return result;
    }

}
}
