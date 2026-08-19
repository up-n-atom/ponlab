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

#include "EmbeddedResource.h"

#include <fstream>

EmbeddedResource::EmbeddedResource(int resourceId, int resourceType)
{
    HMODULE hModule = ::GetModuleHandleW(nullptr);
    m_hResource = ::FindResource(hModule, MAKEINTRESOURCEW(resourceId), MAKEINTRESOURCEW(resourceType));
    if (m_hResource == nullptr) {
        throw std::invalid_argument("Resource not found");
    }

    m_size = ::SizeofResource(hModule, m_hResource);

    HGLOBAL hGlobal = ::LoadResource(hModule, m_hResource);
    if (hGlobal != nullptr) {
        m_data = static_cast<const uint8_t*>(::LockResource(hGlobal));
        //LockResource does not actually lock memory; it is just used to obtain a pointer to the memory containing the resource data.
    }
}

EmbeddedResource::~EmbeddedResource()
{
    if (m_hResource != nullptr) {
        FreeResource(m_hResource);
    }
}

size_t EmbeddedResource::getSize() const
{
    return m_size;
}

const uint8_t* EmbeddedResource::getData() const
{
    return m_data;
}
