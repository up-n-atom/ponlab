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

namespace dut {

CyclicPrefixMode getCyclicPrefixMode(PhyMode phyMode, Gi gi, Ltf ltf)
{
    if ((phyMode == PhyMode::PHY_MODE_AX) || (phyMode == PhyMode::PHY_MODE_BE)) {
        switch (ltf) {
        case Ltf::LTF_MEDIUM:
            if (gi == Gi::GI_0_8_US) {
                return CyclicPrefixMode::CP_MODE_SHORT_CP_MED_LTF;
            } else if (gi == Gi::GI_1_6_US) {
                return CyclicPrefixMode::CP_MODE_MED_CP_MED_LTF;
            }
            break;
        case Ltf::LTF_LONG:
            if (gi == Gi::GI_3_2_US) {
                return CyclicPrefixMode::CP_MODE_LONG_CP_LONG_LTF;
            } else if (gi == Gi::GI_0_8_US && phyMode == PhyMode::PHY_MODE_BE) {
                return CyclicPrefixMode::CP_MODE_SHORT_CP_LONG_LTF;
            }
            break;
        }
    } else {
        if (gi == Gi::GI_0_8_US) {
            return CyclicPrefixMode::CP_MODE_MED_CP_SHORT_LTF;
        } else if (gi == Gi::GI_0_4_US) {
            return CyclicPrefixMode::CP_MODE_SHORT_CP_SHORT_LTF;
        }
    }

    throw std::invalid_argument("Invalid GI (" + toString(gi) + ") and LTF (" + toString(ltf) + ") combination for PHY mode " + toString(phyMode));
}

}
