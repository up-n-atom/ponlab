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

#ifndef _DUT_CLI_CONTEXT_H_
#define _DUT_CLI_CONTEXT_H_

#include "Config.h"
#include "Console.h"
#include "dut/Dut.h"

#include <memory>
#include <unordered_map>

namespace dut_cli {

class Context {
public:
    Context(
        Config& config,
        Console& console,
        dut::DutFactory& dutFactory,
        std::shared_ptr<dut::Connection> connection,
        std::shared_ptr<dut::Logger> logger);

    Config& getConfig();
    Console& getConsole();

    std::shared_ptr<dut::Connection> getConnection();
    std::shared_ptr<dut::Logger> getLogger();
    std::shared_ptr<dut::Dut> getDut(uint8_t wlanIndex);

private:
    Config& m_config;
    Console& m_console;
    dut::DutFactory& m_dutFactory;

    std::shared_ptr<dut::Connection> m_connection;
    std::shared_ptr<dut::Logger> m_logger;

    std::unordered_map<uint8_t, std::shared_ptr<dut::Dut>> m_dutMap;
};

}

#endif
