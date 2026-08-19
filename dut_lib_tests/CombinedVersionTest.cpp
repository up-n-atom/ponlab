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

#include "CombinedVersion.h"

#include <gtest/gtest.h>

namespace {
TEST(CombinedVersionTest, parseCombinedVersionShouldSucceed)
{
    constexpr auto combinedVersion = "wave_release_minor=\"06.01.00.1097\"\nwave_psd_ver=\"2021-08-23_15-30-06_REV_3578c07f871c\"\nwave_regulatory_ver=\"2018.04.17_WAVE19-";
    std::unordered_map<dut::VersionedComponent, std::string> componentVersions;

    dut::parseCombinedVersion(combinedVersion, componentVersions);

    EXPECT_EQ(componentVersions[dut::VersionedComponent::VERSIONED_COMPONENT_CV], "06.01.00.1097");
    EXPECT_EQ(componentVersions[dut::VersionedComponent::VERSIONED_COMPONENT_PSD], "2021-08-23_15-30-06_REV_3578c07f871c");
    EXPECT_EQ(componentVersions[dut::VersionedComponent::VERSIONED_COMPONENT_REGULATORY], "");
}
}