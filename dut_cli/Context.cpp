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

#include "Context.h"

namespace dut_cli {

Context::Context(
    Config& config,
    Console& console,
    dut::DutFactory& dutFactory,
    std::shared_ptr<dut::Connection> connection,
    std::shared_ptr<dut::Logger> logger)
    : m_config(config)
    , m_console(console)
    , m_dutFactory(dutFactory)
    , m_connection(connection)
    , m_logger(logger)
{
}

Config& Context::getConfig()
{
    return m_config;
}

Console& Context::getConsole()
{
    return m_console;
}

std::shared_ptr<dut::Connection> Context::getConnection()
{
    return m_connection;
}

std::shared_ptr<dut::Logger> Context::getLogger()
{
    return m_logger;
}

std::shared_ptr<dut::Dut> Context::getDut(uint8_t wlanIndex)
{
    if (m_dutMap.count(wlanIndex) == 0) {
        m_dutMap.insert(std::make_pair(wlanIndex, m_dutFactory.createInstance(wlanIndex, m_connection, m_logger, true)));
    }

    return m_dutMap[wlanIndex];
}

}
