REM
REM <legal_notice>
REM  MaxLinear, Inc. retains all right, title and interest (including all intellectual
REM  property rights) in and to this computer program, which is protected by applicable
REM  intellectual property laws.  Unless you have obtained a separate written license from
REM  MaxLinear, Inc. or an authorized licensee of MaxLinear, Inc., you are not authorized
REM  to utilize all or a part of this computer program for any purpose (including
REM  reproduction, distribution, modification, and compilation into object code), and you
REM  must immediately destroy or return all copies of this computer program.  If you are
REM  licensed by MaxLinear, Inc. or an authorized licensee of MaxLinear, Inc., your rights
REM  to utilize this computer program are limited by the terms of that license.
REM 
REM  This computer program contains trade secrets owned by MaxLinear, Inc. and, unless
REM  authorized by MaxLinear, Inc. in writing, you agree to maintain the confidentiality
REM  of this computer program and related information and to not disclose this computer
REM  program and related information to any other person or entity.
REM 
REM  Misuse of this computer program or any information contained in it may results in
REM  violations of applicable law.  MaxLinear, Inc. vigorously enforces its copyright,
REM  trade secret, patent, contractual, and other legal rights.
REM 
REM  THIS COMPUTER PROGRAM IS PROVIDED "AS IS" WITHOUT ANY WARRANTIES, AND MAXLINEAR, INC.
REM  EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING THE WARRANTIES OF
REM  MERCHANTIBILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE, AND NONINFRINGEMENT.
REM 
REM ***************************************************************************************
REM                                         Copyright (c) 2022, MaxLinear, Inc.
REM ***************************************************************************************
REM </legal_notice>
REM

@ECHO OFF

SET DUT_SKIP_INCREASE_REVISION=1 & SET DUT_SKIP_AUTOMATION_TESTS=1 & release.bat %1
