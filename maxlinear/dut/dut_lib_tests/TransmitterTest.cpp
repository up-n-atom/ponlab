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

#include "dut/ClientMock.h"

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::ThrowsMessage;

namespace {

class TransmitterTest : public ::testing::Test {
protected:
    std::shared_ptr<StrictMock<dut::ClientMock>> m_client = std::make_shared<StrictMock<dut::ClientMock>>();
};

TEST_F(TransmitterTest, startTxShouldFailIfZeroRepetitions)
{
    dut::Transmitter transmitter(m_client);

    uint16_t repetitions = 0;
    uint32_t packetLength = 1;
    bool longData = false;
    bool beamforming = false;
    bool ldpc = false;

    const char* expectedMessage = "The number of repetitions cannot be 0";
    EXPECT_THAT([&]() { transmitter.startTx(repetitions, packetLength, longData, beamforming, ldpc); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
}

TEST_F(TransmitterTest, startAndStopTxShouldSucceed)
{
    dut::Transmitter transmitter(m_client);

    uint16_t repetitions = 1;
    uint32_t packetLength = 1;
    bool longData = false;
    bool beamforming = false;
    bool ldpc = false;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, ldpc)).Times(1);
        EXPECT_CALL(*m_client, stopTx()).Times(1);
        EXPECT_CALL(*m_client, startTx(repetitions, packetLength, longData, beamforming, ldpc)).Times(1);
        EXPECT_CALL(*m_client, stopTx()).Times(1);
    }

    EXPECT_NO_THROW(transmitter.startTx(repetitions, packetLength, longData, beamforming, ldpc));
    EXPECT_EQ(transmitter.getState(), dut::Transmitter::State::TX_STARTED);

    EXPECT_NO_THROW(transmitter.pause());
    EXPECT_EQ(transmitter.getState(), dut::Transmitter::State::TX_PAUSED);

    EXPECT_NO_THROW(transmitter.resume());
    EXPECT_EQ(transmitter.getState(), dut::Transmitter::State::TX_STARTED);

    EXPECT_NO_THROW(transmitter.stop());
    EXPECT_EQ(transmitter.getState(), dut::Transmitter::State::IDLE);
}

TEST_F(TransmitterTest, startAndStopCwShouldSucceed)
{
    dut::Transmitter transmitter(m_client);

    int8_t amplitude = 1;
    int16_t tone = 1;

    {
        InSequence sequence;

        EXPECT_CALL(*m_client, startCw(amplitude, tone)).Times(1);
        EXPECT_CALL(*m_client, stopCw()).Times(1);
        EXPECT_CALL(*m_client, startCw(amplitude, tone)).Times(1);
        EXPECT_CALL(*m_client, stopCw()).Times(1);
    }

    EXPECT_NO_THROW(transmitter.startCw(amplitude, tone));
    EXPECT_EQ(transmitter.getState(), dut::Transmitter::State::CW_STARTED);

    EXPECT_NO_THROW(transmitter.pause());
    EXPECT_EQ(transmitter.getState(), dut::Transmitter::State::CW_PAUSED);

    EXPECT_NO_THROW(transmitter.resume());
    EXPECT_EQ(transmitter.getState(), dut::Transmitter::State::CW_STARTED);

    EXPECT_NO_THROW(transmitter.stop());
    EXPECT_EQ(transmitter.getState(), dut::Transmitter::State::IDLE);
}

}