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

#ifndef _DUT_C_API_H_
#define _DUT_C_API_H_

/*
 * HowTo: Export C++ classes from a DLL
 * https://www.codeproject.com/Articles/28969/HowTo-Export-C-classes-from-a-DLL
 * 
 * Create and Consume C++ Class DLL on Windows
 * http://neutrofoton.github.io/blog/2017/09/14/create-and-consume-c-plus-plus-class-dll-on-windows/
 */

/**
 * The following ifdef block is the standard way of creating macros which make 
 * exporting from a DLL simpler. All files within this DLL are compiled with 
 * the DUTCWRAPPER_EXPORTS symbol defined. This symbol should not be defined on 
 * any project that uses this DLL. This way any other project whose source 
 * files include this file see DUT_C_API functions as being imported from a 
 * DLL, whereas this DLL sees symbols defined with this macro as being 
 * exported.
 */

#ifdef DUTCWRAPPER_EXPORTS
#define DUT_C_API __declspec(dllexport)
#else
#define DUT_C_API __declspec(dllimport)
#endif

/* Calling convention */
#define DUT_C_API_ENTRY __stdcall

#endif
