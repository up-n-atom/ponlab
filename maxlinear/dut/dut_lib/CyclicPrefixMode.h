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

#ifndef _DUT_CYCLIC_PREFIX_MODE_H_
#define _DUT_CYCLIC_PREFIX_MODE_H_

#include "dut/Types.h"

namespace dut {

enum class CyclicPrefixMode {
    CP_MODE_SHORT_CP_SHORT_LTF = 0x00, // HT/VHT: 0.4µs CP;	HE: 0.8us CP, 1x LTF
    CP_MODE_MED_CP_SHORT_LTF = 0x01, // HT/VHT: 0.8µs CP;	HE: 1.6us CP, 1x LTF
    CP_MODE_SHORT_CP_MED_LTF = 0x02, // HT/VHT: invalid;	   	HE: 0.8us CP, 2x LTF
    CP_MODE_MED_CP_MED_LTF = 0x03, // HT/VHT: invalid;		HE: 1.6us CP, 2x LTF
    CP_MODE_SHORT_CP_LONG_LTF = 0x04, // HT/VHT: invalid;		HE: 0.8us CP, 4x LTF
    CP_MODE_LONG_CP_LONG_LTF = 0x05, // HT/VHT: invalid;		HE: 3.2us CP, 4x LTF
    CP_NUM_OF_MODES = 0x06,
    CP_MODE_INVALID = 0xFF
};

CyclicPrefixMode getCyclicPrefixMode(PhyMode phyMode, Gi gi, Ltf ltf);

}

#endif