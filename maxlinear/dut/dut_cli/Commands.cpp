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

#include "Commands.h"

#include "CommandLine.h"
#include "Functions.h"
#include "Tools.h"

#include <chrono>
#include <fstream>
#include <functional>
#include <map>
#include <set>
#include <stdexcept>
#include <thread>

namespace dut_cli {

using CommandFactory = std::function<std::unique_ptr<Command>()>;

const std::map<std::string, CommandFactory, std::less<>> g_commands {
    { "api", []() { return std::make_unique<ApiCommand>(); } },
    { "exec", []() { return std::make_unique<ExecCommand>(); } },
    { "help", []() { return std::make_unique<HelpCommand>(); } },
    { "load", []() { return std::make_unique<LoadCommand>(); } },
    { "sleep", []() { return std::make_unique<SleepCommand>(); } },
    { "version", []() { return std::make_unique<VersionCommand>(); } },
};

std::vector<std::string> getCommandNames()
{
    std::vector<std::string> commandNames;
    for (const auto& entry : g_commands) {
        commandNames.push_back(entry.first);
    }
    return commandNames;
}

std::unique_ptr<Command> createCommand(const std::string& name)
{
    if (g_commands.count(name) == 0) {
        throw std::runtime_error("Invalid command");
    }

    return g_commands.at(name)();
}

bool ApiCommand::execute(std::vector<std::string>& args, Context& context)
{
    for (const auto& functionName : getFunctionNames()) {
        context.getConsole().cout(functionName + "\n");
    }
    return true;
}

bool ExecCommand::execute(std::vector<std::string>& args, Context& context)
{
    if (args.size() < 2) {
        throw std::runtime_error("WLAN index not specified");
    }

    const char* nptr = args.at(1).c_str();
    char* endptr = nullptr;
    auto wlanIndex = static_cast<uint8_t>(strtol(nptr, &endptr, 0));
    std::set<uint8_t> validValues { 0, 2, 4 };
    if ((nptr == endptr) || (validValues.find(wlanIndex) == validValues.end())) {
        throw std::runtime_error("Invalid WLAN index. Valid values: 2.4GHz = 0, 5GHz = 2 and 6GHz = 4");
    }

    if (args.size() < 3) {
        throw std::runtime_error("Function not specified. Run the 'api' command to get a list of available functions");
    }

    std::unique_ptr<Function> function = createFunction(args.at(2));

    args.erase(args.begin());
    args.erase(args.begin());
    if (!function->parse(args)) {
        return false;
    }

    return function->execute(context.getDut(wlanIndex), context);
}

bool HelpCommand::execute(std::vector<std::string>& args, Context& context)
{
    constexpr const char* helpMessage = "version\n\tShow DUT version.\n"
                                        "sleep <milliseconds>\n\tPause for the specified number of milliseconds.\n"
                                        "api\n\tList the names of the functions in the DUT library\n"
                                        "exec <wlanIndex> <functionName> [parameters]\n\tExecute the specified function of the DUT library.\n"
                                        "load <fileName>\n\tExecute the functions listed in the specified file.\n"
                                        "help\n\tShow this message.\n"
                                        "exit\n\tExit application.\n";

    context.getConsole().cout(helpMessage);
    return true;
}

bool LoadCommand::execute(std::vector<std::string>& args, Context& context)
{
    if (args.size() < 2) {
        throw std::runtime_error("File not specified");
    }

    std::string fileName { args.at(1) };
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::invalid_argument("Unable to open file '" + fileName + "' for reading");
    }

    std::string command;
    while (std::getline(file, command)) {
        trim(command);

        context.getConsole().cout(command + "\n");
        if (!CommandLine::run(command, context)) {
            return false;
        }
    }

    return true;
}

bool SleepCommand::execute(std::vector<std::string>& args, Context& context)
{
    if (args.size() < 2) {
        throw std::runtime_error("Sleep time not specified");
    }

    const char* nptr = args.at(1).c_str();
    char* endptr = nullptr;
    auto sleepTime = static_cast<uint32_t>(strtol(nptr, &endptr, 0));
    if (nptr == endptr) {
        throw std::runtime_error("Invalid sleep time");
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    return true;
}

bool VersionCommand::execute(std::vector<std::string>& args, Context& context)
{
    context.getConsole().cout(dut::Dut::getVersion() + " (" + dut::Dut::getBuildTag() + ")\n");
    return true;
}

}
