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

#include "dut/WindowsConsoleLogger.h"

#include <Windows.h>
#include <iostream>

namespace dut {

#define MAKE_COLOR(foreground, background) (foreground + background * 16)

constexpr WORD blackColor = 0;
constexpr WORD blueColor = 1;
constexpr WORD greenColor = 2;
constexpr WORD aquaColor = 3;
constexpr WORD redColor = 4;
constexpr WORD purpleColor = 5;
constexpr WORD yellowColor = 6;
constexpr WORD whiteColor = 7;
constexpr WORD greyColor = 8;
constexpr WORD lightBlueColor = 9;
constexpr WORD lightGreenColor = 10;
constexpr WORD lightAquaColor = 11;
constexpr WORD lightRedColor = 12;
constexpr WORD lightPurpleColor = 13;
constexpr WORD lightYellowColor = 14;
constexpr WORD brightWhiteColor = 15;

constexpr WORD defaultColor = whiteColor;
constexpr WORD badColor = static_cast<WORD>(-256);

static WORD getTextColor()
{
    CONSOLE_SCREEN_BUFFER_INFO i;
    return GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &i) ? i.wAttributes : badColor;
}

static void setTextColor(WORD c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

static WORD toColor(const LogLevel& logLevel)
{
    switch (logLevel) {
    case LogLevel::LOG_LEVEL_ERROR:
        return MAKE_COLOR(redColor, blackColor);
    case LogLevel::LOG_LEVEL_WARNING:
        return MAKE_COLOR(yellowColor, blackColor);
    case LogLevel::LOG_LEVEL_INFO:
        return MAKE_COLOR(lightGreenColor, blackColor);
    case LogLevel::LOG_LEVEL_DEBUG:
        return MAKE_COLOR(lightAquaColor, blackColor);
    case LogLevel::LOG_LEVEL_TRACE:
        return MAKE_COLOR(defaultColor, blackColor);
    default:
        return getTextColor();
    }
}

WindowsConsoleLogger::WindowsConsoleLogger(bool useColors, LogLevel logLevel)
    : StreamLogger(std::cout, logLevel)
    , m_useColors(useColors)
{
}

void WindowsConsoleLogger::writeMessageToLog(LogLevel level, const std::string& message)
{
    WORD currentColor = badColor;
    if (m_useColors) {
        currentColor = getTextColor();
        setTextColor(toColor(level));
    }

    StreamLogger::writeMessageToLog(level, message);

    if (m_useColors) {
        setTextColor(currentColor);
    }
}

}
