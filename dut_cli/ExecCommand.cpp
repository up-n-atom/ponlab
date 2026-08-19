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

#include "ExecCommand.h"
#include "Functions.h"

#include <set>
#include <stdexcept>

namespace dut_cli {

bool ExecCommand::execute(int argc, char* argv[], Context& context)
{
    if (argc < 2) {
        throw std::runtime_error("WLAN index not specified");
    }

    const char* nptr = argv[1];
    char* endptr = nullptr;
    uint8_t wlanIndex = static_cast<uint8_t>(strtol(nptr, &endptr, 0));
    std::set<uint8_t> validValues { 0, 2, 4 };
    if ((nptr == endptr) || (validValues.find(wlanIndex) == validValues.end())) {
        throw std::runtime_error("Invalid WLAN index");
    }

    if (argc < 3) {
        throw std::runtime_error("Function not specified");
    }

    std::unique_ptr<Function> function = createFunction(argv[2]);

    if (!function->parse(argc - 2, &argv[2])) {
        return false;
    }

    return function->execute(context.getDut(wlanIndex), context);
}

}
