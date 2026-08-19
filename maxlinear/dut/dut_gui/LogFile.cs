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
using System.IO;

namespace DUT_GUI
{
    public class LogFile
    {
        private readonly StreamWriter output;

        private static string GetLogFileName(int i)
        {
            string filePath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            string filePart = "DUT_GUI_console";
            if (i > 0)
            {
                filePart += "_" + Convert.ToString(i);
            }

            return Path.Combine(filePath, filePart + ".txt");
        }

        public LogFile()
        {
            int i = 0;
            while (true)
            {
                try
                {
                    output = new StreamWriter(File.OpenWrite(GetLogFileName(i)));
                    break;
                }
                catch
                {
                    i++;
                }
            }
        }

        public void Write(string s)
        {
            lock (output)
            {
                output.Write(s);
            }
        }

        public void Close()
        {
            output.Flush();
            output.Close();
        }
    }
}
