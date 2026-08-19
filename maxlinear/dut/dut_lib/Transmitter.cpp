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

#include "Transmitter.h"

#include "dut/Client.h"
#include "dut/Tools.h"

#include <stdexcept>

namespace dut {

Transmitter::Transmitter(std::shared_ptr<Client> client)
    : m_client(client)
{
}

Transmitter::State Transmitter::getState() const
{
    return m_state;
}

void Transmitter::startTx(uint16_t repetitions, uint32_t packetLength, bool longData, bool beamforming, bool ldpc)
{
    if (repetitions == 0) {
        throw std::invalid_argument("The number of repetitions cannot be 0");
    }

    stop();

    m_client->startTx(repetitions, packetLength, longData, beamforming, ldpc);

    m_txParams.repetitions = repetitions;
    m_txParams.packetLength = packetLength;
    m_txParams.longData = longData;
    m_txParams.beamforming = beamforming;
    m_txParams.ldpc = ldpc;

    m_state = State::TX_STARTED;
}

void Transmitter::startCw(int8_t amplitude, int16_t tone)
{
    stop();

    m_client->startCw(amplitude, tone);

    m_cwParams.amplitude = amplitude;
    m_cwParams.tone = tone;

    m_state = State::CW_STARTED;
}

void Transmitter::stop()
{
    if (m_state == State::TX_STARTED) {
        m_client->stopTx();

        m_state = State::IDLE;
    } else if (m_state == State::CW_STARTED) {
        m_client->stopCw();

        m_state = State::IDLE;
    }
}

void Transmitter::pause()
{
    if (m_state == State::TX_STARTED) {
        m_client->stopTx();

        m_state = State::TX_PAUSED;
    } else if (m_state == State::CW_STARTED) {
        m_client->stopCw();

        m_state = State::CW_PAUSED;
    }
}

void Transmitter::resume()
{
    if (m_state == State::TX_PAUSED) {
        m_client->startTx(m_txParams.repetitions, m_txParams.packetLength, m_txParams.longData, m_txParams.beamforming, m_txParams.ldpc);

        m_state = State::TX_STARTED;
    } else if (m_state == State::CW_PAUSED) {
        m_client->startCw(m_cwParams.amplitude, m_cwParams.tone);

        m_state = State::CW_STARTED;
    }
}

}