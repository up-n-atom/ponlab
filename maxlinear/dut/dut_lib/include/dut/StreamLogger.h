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

#ifndef _DUT_STREAM_LOGGER_H_
#define _DUT_STREAM_LOGGER_H_

#include "dut/Logger.h"

#include <ostream>

namespace dut {

/**
 * @brief Logger implementation class in which the logging method writes the log message to an 
 * output stream.
 */
class DUT_API StreamLogger : public Logger {
public:
    /**
     * @brief Class constructor.
     * 
     * @param stream Output stream to write log messages to.
     * @param logLevel Optional log level threshold.
     */
    explicit StreamLogger(std::ostream& stream, LogLevel logLevel = LogLevel::LOG_LEVEL_NONE);

    /**
     * @brief Copy constructor
     *
     * Delete copy constructor to avoid having multiple copies of the same instance.
     */
    StreamLogger(const StreamLogger&) = delete;

    /**
     * @brief Assignment operator
     *
     * Delete assignment operator to avoid having multiple copies of the same instance.
     */
    StreamLogger& operator=(const StreamLogger&) = delete;

protected:
    /**
     * Output stream to write log messages to.
     */
    std::ostream& m_stream;

    /**
     * @see Logger::writeMessageToLog()
     */
    void writeMessageToLog(LogLevel level, const std::string& message) override;
};
}

#endif