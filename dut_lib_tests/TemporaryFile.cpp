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

#include "TemporaryFile.h"

#include <fcntl.h>
#include <io.h>

#include <gtest/gtest.h>

TemporaryFile::TemporaryFile()
    : TemporaryFile(nullptr, 0)
{
}

TemporaryFile::TemporaryFile(const uint8_t* data, size_t length)
{
    snprintf(m_filename, sizeof(m_filename), "%stmpXXXXXX", testing::TempDir().c_str());

    EXPECT_EQ(_mktemp_s(m_filename, sizeof(m_filename)), 0);

    int fd;
    EXPECT_EQ(_sopen_s(&fd, m_filename, _O_CREAT | _O_BINARY | _O_RDWR, _SH_DENYNO, _S_IREAD | _S_IWRITE), 0);
    EXPECT_EQ(_write(fd, data, static_cast<unsigned int>(length)), length);
    EXPECT_EQ(_close(fd), 0);
}

TemporaryFile::~TemporaryFile()
{
    EXPECT_EQ(_unlink(m_filename), 0);
}

const char* TemporaryFile::getFilename() const
{
    return m_filename;
}
