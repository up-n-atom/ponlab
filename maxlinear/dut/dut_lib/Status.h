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

#ifndef _DUT_STATUS_H_
#define _DUT_STATUS_H_

#include "dut/AntennaMask.h"
#include "dut/Types.h"

#include <stdexcept>
#include <vector>

namespace dut {

template <class T>
class CachedValue {
public:
    CachedValue() = default;

    T getValue() const
    {
        if (!m_valueIsSet) {
            throw std::runtime_error("Value is not set");
        }
        return m_value;
    }

    void setValue(const T& value)
    {
        m_value = value;
        m_valueIsSet = true;
    }

    bool isValueSet() const
    {
        return m_valueIsSet;
    }

    void reset()
    {
        m_valueIsSet = false;
    }

private:
    T m_value {};
    bool m_valueIsSet = false;
};

class Status {
public:
    CachedValue<bool> snifferMode;
    CachedValue<bool> vapAdded;
    CachedValue<AntennaMask> enabledRxAntennaMask;
    CachedValue<AntennaMask> enabledTxAntennaMask;
    CachedValue<AntennaMask> enabledZwdfsAntennaMask;
    CachedValue<AntennaMask> availableRxAntennaMask;
    CachedValue<AntennaMask> availableTxAntennaMask;
    CachedValue<AntennaMask> availableZwdfsAntennaMask;
    CachedValue<Band> band;
    CachedValue<std::vector<Band>> supportedBands;
    CachedValue<Bandwidth> spectrumBandwidth;
    CachedValue<Bandwidth> signalBandwidth;
    CachedValue<uint8_t> lowestChannel;
    CachedValue<uint8_t> primaryChannelIndex;
    CachedValue<Mcs> mcs;
    CachedValue<uint8_t> nss;
    CachedValue<Gi> gi;
    CachedValue<Ltf> ltf;
    CachedValue<uint32_t> offlineCalMask;
    CachedValue<uint32_t> onlineCalMask;
    CachedValue<uint32_t> ifs;
    CachedValue<PhyMode> phyMode;
    CachedValue<RegulationType> regulationType;
    CachedValue<bool> closedLoop;
    CachedValue<bool> spacelessTxEnabled;
    CachedValue<bool> rxAggregationEnabled;
    CachedValue<bool> rxCalibrationRunning;
    CachedValue<uint8_t> transmitPowerLevel;
    CachedValue<uint8_t> transmitPowerLimit;
    CachedValue<ChipID> chipId;
    CachedValue<uint8_t> zwdfsChannel;
    CachedValue<Bandwidth> zwdfsBandwidth;
    CachedValue<Bandwidth> radarDetectionBandwidth;
    CachedValue<uint32_t> userOne;
    CachedValue<uint32_t> userTwo;
    CachedValue<uint32_t> rxPacketLimit;
    CachedValue<bool> rxMeasureRunning;
    CachedValue<bool> setClipper;

    void reset();
};

}

#endif
