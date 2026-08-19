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

#ifndef _DUT_TRANSMITTER_H_
#define _DUT_TRANSMITTER_H_

#include <memory>

namespace dut {

class Client;

class Transmitter {
public:
    enum class State {
        IDLE,
        TX_STARTED,
        TX_PAUSED,
        CW_STARTED,
        CW_PAUSED,
    };

    explicit Transmitter(std::shared_ptr<Client> client);

    State getState() const;

    void startTx(uint16_t repetitions, uint32_t packetLength, bool longData, bool beamforming, bool ldpc);
    void startCw(int8_t amplitude, int16_t tone);
    void stop();
    void pause();
    void resume();

private:
    std::shared_ptr<Client> m_client;

    struct TxParams {
        uint16_t repetitions = 0;
        uint32_t packetLength = 1000;
        bool longData = false;
        bool beamforming = false;
        bool ldpc = false;
    };

    struct CwParams {
        int8_t amplitude = 0;
        int16_t tone = 0;
    };

    TxParams m_txParams;
    CwParams m_cwParams;

    State m_state = State::IDLE;
};
}

#endif
