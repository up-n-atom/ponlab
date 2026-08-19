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

#include "TempFile.h"

#include <fstream>
#include <windows.h>

TempFile::TempFile(const std::string& tempDir, const std::string& prefix)
{
    DWORD uRetVal = GetTempFileNameA(tempDir.c_str(), // directory for tmp files
        prefix.c_str(), // temp file name prefix
        0, // create unique name
        tempFilename); // buffer for name

    if (uRetVal == 0) {
        throw std::runtime_error("Cannot create temporary file " + std::string(tempFilename));
    }
}

TempFile::~TempFile()
{
    _unlink(tempFilename);
}

std::string TempFile::getName() const
{
    return tempFilename;
}

void TempFile::write(const std::string& s)
{
    std::ofstream f(getName(), std::ios::out);
    if (!f.is_open()) {
        throw std::invalid_argument("Unable to open file '" + getName() + "' for writing");
    }

    f.write(s.c_str(), s.length());
    f.close();

    if (f.fail()) {
        throw std::invalid_argument("Unable to write file '" + getName() + "'");
    }
}
