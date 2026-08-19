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

#include <windows.h>

#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "../dut_lib/VersionInfo.h"

std::string createGuid()
{
    std::ostringstream os;

    GUID guid;
    HRESULT result = CoCreateGuid(&guid);

    if (result == S_OK) {
        os << std::hex << std::setw(8) << std::setfill('0') << guid.Data1;
        os << '-';
        os << std::hex << std::setw(4) << std::setfill('0') << guid.Data2;
        os << '-';
        os << std::hex << std::setw(4) << std::setfill('0') << guid.Data3;
        os << '-';
        os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[0]);
        os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[1]);
        os << '-';
        os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[2]);
        os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[3]);
        os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[4]);
        os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[5]);
        os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[6]);
        os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[7]);
    }

    return os.str();
}

std::string toUpper(const std::string& s)
{
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

void insertString(const std::string& str, const std::string& prefix, const std::string& suffix, const std::string& separator, std::string& text)
{
    size_t startPosition = text.find(prefix);
    if (startPosition != std::string::npos) {
        size_t endPosition = text.find(suffix, startPosition + prefix.length());
        if (endPosition != std::string::npos) {
            text = text.substr(0, startPosition) + prefix + separator + str + suffix + text.substr(endPosition + suffix.length());
        }
    }
}

std::string loadFile(const std::string& fileName)
{
    std::ifstream inputFile(fileName);
    if (!inputFile) {
        throw std::runtime_error("Unable to read file '" + fileName + "'");
    }

    std::string contents;
    std::string line;

    while (getline(inputFile, line)) {
        contents += line + '\n';
    }
    inputFile.close();

    return contents;
}

void saveFile(const std::string& fileName, const std::string& contents)
{
    std::ofstream outputFile(fileName);
    if (!outputFile) {
        throw std::runtime_error("Unable to write file '" + fileName + "'");
    }

    outputFile << contents;
    outputFile.close();
}

void setVersionIntoDutAutomationAssemblyInfo(const std::string& version, const std::string& copyright, const std::string& tag, std::string& contents)
{
    insertString(version, "\n[assembly: AssemblyVersion(\"", "\")]", "", contents);
    insertString(version, "AssemblyFileVersion(\"", "\")]", "", contents);
    insertString(copyright, "AssemblyCopyright(\"", "\")]", "", contents);
    insertString("dut_automation (" + tag + ")", "AssemblyTitle(\"", "\")]", "", contents);
}

void setVersionIntoDutGuiAssemblyInfo(const std::string& version, const std::string& copyright, const std::string& tag, std::string& contents)
{
    insertString(version, "\n[assembly: AssemblyVersion(\"", "\")]", "", contents);
    insertString(version, "AssemblyFileVersion(\"", "\")]", "", contents);
    insertString(copyright, "AssemblyCopyright(\"", "\")]", "", contents);
    insertString("dut_gui (" + tag + ")", "AssemblyTitle(\"", "\")]", "", contents);
}

/// <summary>
/// Sets version and copyright information in the dut_gui project file.
/// This is necessary only if using .NET5. For .NET Framework v.4.7.2, version information is stored in file AssemblyInfo.cs and in the resource file.
/// </summary>
/// <param name="version"></param>
/// <param name="copyright"></param>
/// <param name="contents"></param>
void setVersionIntoDutGuiProject(const std::string& version, const std::string& copyright, const std::string& tag, std::string& contents)
{
    // Set package version which in turn sets assembly version and assembly file version
    insertString(version, "<Version>", "</Version>", "", contents);

    // Set the copyright
    insertString(copyright, "<Copyright>", "</Copyright>", "", contents);
}

void setVersionIntoDutCliWrapperResource(const std::string& version, const std::string& copyright, const std::string& tag, std::string& contents)
{
    // How do you set the version of a C++/CLI project in Visual Studio?
    // The 'Details' page in Windows Explorer is pulling the information from unmanaged structures
    // in the file, so you'll need to create an unmanaged version resource in order to fill that in.
    insertString(version, "VALUE \"ProductVersion\", \"", "\"", "", contents);
    insertString(copyright, "VALUE \"LegalCopyright\", \"", "\"", "", contents);
    insertString("DUT C++/CLI wrapper for DUT library (" + tag + ")", "VALUE \"FileDescription\", \"", "\"", "", contents);
}

void setVersionIntoDutCliWrapperAssemblyInfo(const std::string& version, const std::string& copyright, const std::string& tag, std::string& contents)
{
    // It is recommended to maintain the various assembly attributes in the AssemblyInfo.cpp file as well as in the resource file.
    // This way, if you use reflection to get information about the assembly, it will use the stuff in AssemblyInfo.cpp, not in the version resource.
    insertString(version, "AssemblyVersionAttribute(\"", "\")];", "", contents);
    insertString(copyright, "AssemblyCopyrightAttribute(L\"", "\")];", "", contents);
    insertString("dut_cli_wrapper (" + tag + ")", "AssemblyTitleAttribute(L\"", "\")];", "", contents);
}

void setVersionIntoDutGuiSetupProject(const std::string& version, const std::string& copyright, const std::string& tag, std::string& contents)
{
    // Create copy of existing value to later check if version has changed
    std::string oldContents = contents;

    // Append version to product name so it is used as part of the suggested installation folder
    insertString(version + "_" + tag, "\"ProductName\" = \"8:DUT_GUI", "\"", "_", contents);

    // Append version to setup file name so two different setups can be distinguished from each other.
    insertString(version + "_" + tag, "Debug\\\\dut_gui_setup", ".msi\"", "_", contents);
    insertString(version + "_" + tag, "Release\\\\dut_gui_setup", ".msi\"", "_", contents);

    // Cannot set product version because it must be of format '##.##.####'
    //insertString(version, "\"ProductVersion\" = \"8:", "\"", "", contents);

    if (oldContents != contents) {
        // Set a new ProductCode and UpgradeCode so two different versions can be installed on the same computer.
        insertString(toUpper(createGuid()), "\"ProductCode\" = \"8:{", "}\"", "", contents);
        insertString(toUpper(createGuid()), "\"UpgradeCode\" = \"8:{", "}\"", "", contents);
    }
}

void setVersionIntoDutCliProject(const std::string& version, const std::string& copyright, const std::string& tag, std::string& contents)
{
    insertString(version, "VALUE \"ProductVersion\", \"", "\"", "", contents);
    insertString(copyright, "VALUE \"LegalCopyright\", \"", "\"", "", contents);
    insertString("DUT Command Line Interface (" + tag + ")", "VALUE \"FileDescription\", \"", "\"", "", contents);
}

void setVersionIntoDutDllProject(const std::string& version, const std::string& copyright, const std::string& tag, std::string& contents)
{
    insertString(version, "VALUE \"ProductVersion\", \"", "\"", "", contents);
    insertString(copyright, "VALUE \"LegalCopyright\", \"", "\"", "", contents);
    insertString("DUT Dynamic Link Library (" + tag + ")", "VALUE \"FileDescription\", \"", "\"", "", contents);
}

using VersionInfoSetter = std::function<void(const std::string& version, const std::string& copyright, const std::string& tag, std::string& contents)>;

void setVersionIntoProjectFile(const std::string& version, const std::string& copyright, const std::string& tag, const std::string& fileName, VersionInfoSetter setter)
{
    std::string contents = loadFile(fileName);
    std::string oldContents = contents;

    setter(version, copyright, tag, contents);

    if (oldContents != contents) {
        saveFile(fileName, contents);
    }
}

std::string getCurrentDirectory()
{
    CHAR path[MAX_PATH];
    if (!GetCurrentDirectoryA(MAX_PATH, path)) {
        throw std::runtime_error("Unable to get current directory");
    }
    return path;
}

int main(int argc, const char* argv[])
{
    try {
        std::string solutionDir;

        if (argc > 1) {
            solutionDir = argv[1];
        } else {
            solutionDir = getCurrentDirectory();
        }

        std::cout << "Solution directory: " << solutionDir << std::endl;
        std::cout << "Version: " << STR_PROD_VER << std::endl;
        std::cout << "Copyright: " << STR_COPYRIGHT_INFO << std::endl;
        std::cout << "Version Tag: " << STR_BUILD_TAG << std::endl;

        setVersionIntoProjectFile(STR_PROD_VER, STR_COPYRIGHT_INFO, STR_BUILD_TAG, solutionDir + "/dut_automation/Properties/AssemblyInfo.cs", setVersionIntoDutAutomationAssemblyInfo);
        setVersionIntoProjectFile(STR_PROD_VER, STR_COPYRIGHT_INFO, STR_BUILD_TAG, solutionDir + "/dut_gui/Properties/AssemblyInfo.cs", setVersionIntoDutGuiAssemblyInfo);
        setVersionIntoProjectFile(STR_PROD_VER, STR_COPYRIGHT_INFO, STR_BUILD_TAG, solutionDir + "/dut_gui/dut_gui.csproj", setVersionIntoDutGuiProject);
        setVersionIntoProjectFile(STR_PROD_VER, STR_COPYRIGHT_INFO, STR_BUILD_TAG, solutionDir + "/dut_cli_wrapper/Resource.rc", setVersionIntoDutCliWrapperResource);
        setVersionIntoProjectFile(STR_PROD_VER, STR_COPYRIGHT_INFO, STR_BUILD_TAG, solutionDir + "/dut_cli_wrapper/AssemblyInfo.cpp", setVersionIntoDutCliWrapperAssemblyInfo);
        setVersionIntoProjectFile(STR_PROD_VER, STR_COPYRIGHT_INFO, STR_BUILD_TAG, solutionDir + "/dut_gui_setup/dut_gui_setup.vdproj", setVersionIntoDutGuiSetupProject);
        setVersionIntoProjectFile(STR_PROD_VER, STR_COPYRIGHT_INFO, STR_BUILD_TAG, solutionDir + "/dut_cli/Resource.rc", setVersionIntoDutCliProject);
        setVersionIntoProjectFile(STR_PROD_VER, STR_COPYRIGHT_INFO, STR_BUILD_TAG, solutionDir + "/dut_dll/Resource.rc", setVersionIntoDutDllProject);

    } catch (std::exception const& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
