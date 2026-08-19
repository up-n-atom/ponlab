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

#ifndef _DUT_LOGGER_H_
#define _DUT_LOGGER_H_

#include "dut/API.h"

#include <string>

namespace dut {

/**
 * @brief Log levels, used to filter out log messages by importance.
 */
enum class LogLevel {
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE
};

/**
 * @brief Converts log level value to string.
 */
std::string toString(LogLevel logLevel);

/**
 * @brief Logger abstract class.
 * 
 * Derived classes must implement pure virtual method writeMessageToLog() which is called whenever 
 * a new message has to be logged.
 */
class DUT_API Logger {
public:
    /**
     * @brief Class destructor
     */
    virtual ~Logger() = default;

    /**
     * @brief Gets current log level threshold.
     * 
     * @return Current log level threshold.
     */
    LogLevel getLogLevel() const;

    /**
     * @brief Sets new log level threshold.
     * 
     * @param logLevel New log level threshold.
     */
    void setLogLevel(LogLevel logLevel);

    /**
     * @brief Writes a new message to the log.
     * 
     * Message is written to the log only if the specified level is less than or equal than the 
     * current threshold (i.e. is message is important enough).
     * 
     * @param level Log level of the message to write to the log. 
     * @param message Message to write to the log.
     */
    void log(LogLevel level, const std::string& message);

protected:
    /**
     * @brief Class constructor.
     * 
     * Constructor is protected so only derived classes can call it.
     */
    explicit Logger(LogLevel logLevel = LogLevel::LOG_LEVEL_NONE);

    /**
     * @brief Actually writes a new message to the log.
     * 
     * This method must be implemented by derived classes to actually write to the log.
     * 
     * @param level Log level of the message to write to the log. 
     * @param message Message to write to the log.
     */
    virtual void writeMessageToLog(LogLevel level, const std::string& message) = 0;

private:
    /**
     * Log level or threshold. 
     * Only log messages with a log level less than or equal to this level are actually logged. 
     */
    LogLevel m_logLevel = LogLevel::LOG_LEVEL_NONE;
};

/**
 * @brief Logger implementation class that does nothing with log messages.
 * 
 * To be used when no logging is needed.
 */
class DUT_API NullLogger : public Logger {
public:
    /**
     * @brief Class constructor
     */
    NullLogger() = default;

    /**
     * @brief Copy constructor
     *
     * Delete copy constructor to avoid having multiple copies of the same instance.
     */
    NullLogger(const NullLogger&) = delete;

    /**
     * @brief Assignment operator
     *
     * Delete assignment operator to avoid having multiple copies of the same instance.
     */
    NullLogger& operator=(const NullLogger&) = delete;

protected:
    /**
     * @see Logger::writeMessageToLog()
     */
    void writeMessageToLog(LogLevel level, const std::string& message) override
    {
        (void)level;
        (void)message;
    }
};

/**
 * @brief Logger implementation class in which the logging method calls a C function.
 */
class DUT_API FunctionPtrLogger : public Logger {
public:
    using LogFunction = void (*)(LogLevel, const char*);

    /**
     * @brief Class constructor.
     * 
     * @param logFunction Pointer to C function to be called to log a message.
     * @param logLevel Optional log level threshold.
     */
    explicit FunctionPtrLogger(LogFunction logFunction, LogLevel logLevel = LogLevel::LOG_LEVEL_NONE)
        : Logger(logLevel)
        , m_logFunction(logFunction)
    {
    }

    /**
     * @brief Copy constructor
     *
     * Delete copy constructor to avoid having multiple copies of the same instance.
     */
    FunctionPtrLogger(const FunctionPtrLogger&) = delete;

    /**
     * @brief Assignment operator
     *
     * Delete assignment operator to avoid having multiple copies of the same instance.
     */
    FunctionPtrLogger& operator=(const FunctionPtrLogger&) = delete;

protected:
    /**
     * @see Logger::writeMessageToLog()
     */
    void writeMessageToLog(dut::LogLevel level, const std::string& message) override
    {
        if (m_logFunction) {
            m_logFunction(level, message.c_str());
        }
    }

private:
    /**
     * @brief Pointer to C function to be called to log a message.
     */
    LogFunction m_logFunction;
};

}

#endif