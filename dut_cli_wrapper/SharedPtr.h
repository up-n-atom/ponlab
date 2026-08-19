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

#pragma once

#include <memory>

namespace dut_cli {

ref class Lock {
    Object ^ m_pObject;

public:
    Lock(Object ^ pObject)
        : m_pObject(pObject)
    {
        System::Threading::Monitor::Enter(m_pObject);
    }
    ~Lock()
    {
        System::Threading::Monitor::Exit(m_pObject);
    }
};

template <typename T>
ref class SharedPtr {
public:
    SharedPtr()
        : m_sharedPtr(__nullptr)
    {
    }

    ~SharedPtr()
    {
        Reset();
    }

    inline bool IsValid()
    {
        return m_sharedPtr;
    }

    inline T* operator->()
    {
        return m_sharedPtr->get();
    }

    !SharedPtr()
    {
        Reset();
    }

    std::shared_ptr<T> Get()
    {
        return std::shared_ptr<T>(*m_sharedPtr);
    }

    std::shared_ptr<T>& GetRef()
    {
        return *m_sharedPtr;
    }

    void Reset()
    {
        Lock lock(this);
        if (m_sharedPtr) {
            delete m_sharedPtr;
            m_sharedPtr = __nullptr;
        }
    }

    void Reset(std::shared_ptr<T>& ptr)
    {
        Lock lock(this);
        if (!m_sharedPtr || ptr != *m_sharedPtr) {
            if (m_sharedPtr) {
                delete m_sharedPtr;
                m_sharedPtr = __nullptr;
            }
            m_sharedPtr = new std::shared_ptr<T>(ptr);
        }
    }

private:
    std::shared_ptr<T>* m_sharedPtr;
};
}
