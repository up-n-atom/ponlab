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

﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace DUT_GUI
{
    public class NativeMethods
    {
        internal const int SW_HIDE = 0;
        internal const int SW_SHOW = 5;

#pragma warning disable CS0649 // Field 'NativeMethods.RECT.left' is never assigned to, and will always have its default value 0
        internal struct RECT { public int left, top, right, bottom; }
#pragma warning restore CS0649

        [DllImport("kernel32.dll")]
        internal static extern bool AllocConsole();

        [DllImport("kernel32.dll")]
        internal static extern bool FreeConsole();

        [DllImport("kernel32.dll")]
        internal static extern IntPtr GetConsoleWindow();

        [DllImport("user32.dll", SetLastError = true)]
        internal static extern bool GetWindowRect(IntPtr hWnd, out RECT rc);

        [DllImport("user32.dll", SetLastError = true)]
        internal static extern bool MoveWindow(IntPtr hWnd, int x, int y, int w, int h, bool repaint);

        [DllImport("kernel32.dll")]
        internal static extern int SetStdHandle(int device, IntPtr handle);

        [DllImport("user32.dll")]
        internal static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);
    }
}
