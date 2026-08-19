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
 *                                          Copyright (c) 2022, MaxLinear, Inc.
 *  ***************************************************************************************
 *  </legal_notice>
 */

#include "CyclicPrefixMode.h"

#include "dut/Tools.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::ThrowsMessage;

namespace {

struct GetCyclicPrefixModeTestData_t {
    dut::PhyMode phyMode;
    dut::Gi gi;
    dut::Ltf ltf;
    dut::CyclicPrefixMode expectedCyclicPrefixMode;
};

class CyclicPrefixModeTest : public ::testing::Test {
protected:
    void getCyclicPrefixModeTest(const std::vector<GetCyclicPrefixModeTestData_t>& testData) const
    {
        for (const auto& data : testData) {
            if (data.expectedCyclicPrefixMode == dut::CyclicPrefixMode::CP_MODE_INVALID) {
                const std::string expectedMessage = "Invalid GI (" + dut::toString(data.gi) + ") and LTF (" + dut::toString(data.ltf) + ") combination for PHY mode " + dut::toString(data.phyMode);
                EXPECT_THAT([&]() { dut::getCyclicPrefixMode(data.phyMode, data.gi, data.ltf); }, ThrowsMessage<std::invalid_argument>(expectedMessage));
            } else {
                EXPECT_EQ(dut::getCyclicPrefixMode(data.phyMode, data.gi, data.ltf), data.expectedCyclicPrefixMode);
            }
        }
    }
};

TEST_F(CyclicPrefixModeTest, getCyclicPrefixModeShouldSucceed)
{
    std::vector<GetCyclicPrefixModeTestData_t> testData = {
        { dut::PhyMode::PHY_MODE_AX, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT, dut::CyclicPrefixMode::CP_MODE_INVALID },
        { dut::PhyMode::PHY_MODE_AX, dut::Gi::GI_1_6_US, dut::Ltf::LTF_SHORT, dut::CyclicPrefixMode::CP_MODE_INVALID },
        { dut::PhyMode::PHY_MODE_AX, dut::Gi::GI_0_8_US, dut::Ltf::LTF_MEDIUM, dut::CyclicPrefixMode::CP_MODE_SHORT_CP_MED_LTF },
        { dut::PhyMode::PHY_MODE_AX, dut::Gi::GI_1_6_US, dut::Ltf::LTF_MEDIUM, dut::CyclicPrefixMode::CP_MODE_MED_CP_MED_LTF },
        { dut::PhyMode::PHY_MODE_AX, dut::Gi::GI_0_8_US, dut::Ltf::LTF_LONG, dut::CyclicPrefixMode::CP_MODE_INVALID },
        { dut::PhyMode::PHY_MODE_AX, dut::Gi::GI_3_2_US, dut::Ltf::LTF_LONG, dut::CyclicPrefixMode::CP_MODE_LONG_CP_LONG_LTF },

        { dut::PhyMode::PHY_MODE_AX, dut::Gi::GI_3_2_US, dut::Ltf::LTF_SHORT, dut::CyclicPrefixMode::CP_MODE_INVALID },
        { dut::PhyMode::PHY_MODE_AX, dut::Gi::GI_3_2_US, dut::Ltf::LTF_MEDIUM, dut::CyclicPrefixMode::CP_MODE_INVALID },
        { dut::PhyMode::PHY_MODE_AX, dut::Gi::GI_1_6_US, dut::Ltf::LTF_LONG, dut::CyclicPrefixMode::CP_MODE_INVALID },

        { dut::PhyMode::PHY_MODE_AC, dut::Gi::GI_0_8_US, dut::Ltf::LTF_SHORT, dut::CyclicPrefixMode::CP_MODE_MED_CP_SHORT_LTF },
        { dut::PhyMode::PHY_MODE_AC, dut::Gi::GI_0_4_US, dut::Ltf::LTF_SHORT, dut::CyclicPrefixMode::CP_MODE_SHORT_CP_SHORT_LTF },

        { dut::PhyMode::PHY_MODE_AC, dut::Gi::GI_1_6_US, dut::Ltf::LTF_SHORT, dut::CyclicPrefixMode::CP_MODE_INVALID },
        { dut::PhyMode::PHY_MODE_AC, dut::Gi::GI_3_2_US, dut::Ltf::LTF_SHORT, dut::CyclicPrefixMode::CP_MODE_INVALID },

        { dut::PhyMode::PHY_MODE_BE, dut::Gi::GI_0_8_US, dut::Ltf::LTF_LONG, dut::CyclicPrefixMode::CP_MODE_SHORT_CP_LONG_LTF },
        { dut::PhyMode::PHY_MODE_BE, dut::Gi::GI_3_2_US, dut::Ltf::LTF_LONG, dut::CyclicPrefixMode::CP_MODE_LONG_CP_LONG_LTF },
    };

    getCyclicPrefixModeTest(testData);
}

}