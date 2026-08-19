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

#ifndef _DUT_API_H_
#define _DUT_API_H_

/**
 * The following ifdef block is the standard way of creating macros which make 
 * exporting from a DLL simpler. All files within this DLL are compiled with 
 * the DUTDLL_EXPORTS symbol defined. This symbol should not be defined on any 
 * project that uses this DLL. This way any other project whose source files 
 * include this file see DUT_API functions as being imported from a DLL, 
 * whereas this DLL sees symbols defined with this macro as being exported.
 */

#ifdef DUTDLL_EXPORTS
#define DUT_API __declspec(dllexport)
#elif DUTDLL_IMPORTS
#define DUT_API __declspec(dllimport)
#else
#define DUT_API
#endif

/*
 * There is no need to explicitly specify a calling convention for exporting 
 * classes or their methods. By default, the C++ compiler uses the __thiscall 
 * calling convention for class methods. However, due to different naming 
 * decoration schemes that are used by different compilers, the exported C++ 
 * class can only be used by the same compiler and by the same version of the 
 * compiler. Only the MS Visual C++ compiler can use this DLL now. Both the 
 * DLL and the client code must be compiled with the same version of MS Visual 
 * C++ in order to ensure that the naming decoration scheme matches between 
 * the caller and the callee.
*/

#endif
