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

#include <map>

namespace dut {

static std::string parseComponentVersion(const std::string& combinedVersion, const std::string& componentName)
{
    std::string searchString = componentName + "=\"";
    std::size_t found = combinedVersion.find(searchString);
    if (found != std::string::npos) {
        std::size_t position = found + searchString.length();
        std::size_t endingQuote = combinedVersion.find("\"", position + 1);
        if (endingQuote != std::string::npos) {
            std::string version = combinedVersion.substr(position, endingQuote - position);
            return version;
        }
    }

    return "";
}

void parseCombinedVersion(const std::string& combinedVersion, std::unordered_map<VersionedComponent, std::string>& componentVersions)
{
    static std::map<VersionedComponent, std::string> components {
        { VersionedComponent::VERSIONED_COMPONENT_CV, "wave_release_minor" },
        { VersionedComponent::VERSIONED_COMPONENT_PSD, "wave_psd_ver" },
        { VersionedComponent::VERSIONED_COMPONENT_REGULATORY, "wave_regulatory_ver" },
        { VersionedComponent::VERSIONED_COMPONENT_500B_PROGMODEL, "wave500B_progmodel_ver" },
        { VersionedComponent::VERSIONED_COMPONENT_600_PROGMODEL, "wave600_progmodel_ver" },
        { VersionedComponent::VERSIONED_COMPONENT_600B_PROGMODEL, "wave600B_progmodel_ver" },
        { VersionedComponent::VERSIONED_COMPONENT_600D2_PROGMODEL, "wave600D2_progmodel_ver" },
        { VersionedComponent::VERSIONED_COMPONENT_700_PROGMODEL, "wave700_progmodel_ver" },
        { VersionedComponent::VERSIONED_COMPONENT_700B_PROGMODEL, "wave700B_progmodel_ver" },
    };

    componentVersions.clear();

    for (const auto& entry : components) {
        VersionedComponent componentId = entry.first;
        std::string componentName = entry.second;

        componentVersions[componentId] = parseComponentVersion(combinedVersion, componentName);
    }
}

}
