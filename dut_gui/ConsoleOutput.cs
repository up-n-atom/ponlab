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
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DUT_GUI
{
    public enum OutputDestination { OUT_TEXTBOX = 0, OUT_CONSOLE = 1, OUT_FILE = 2, OUT_ALL = 3 }

    public class ConsoleOutput : TextWriter
    {
        private TextWriter standardOutput;
        private readonly LogFile logFile;
        private readonly RichTextBox textBox;

        public static Color ForegroundColor { get; set; }

        public OutputDestination OutputDestination { get; set; }

        public ConsoleOutput(TextWriter standardOutput, LogFile logFile, RichTextBox textBox)
        {
            ForegroundColor = Color.DarkGreen;

            this.standardOutput = standardOutput;
            this.logFile = logFile;
            this.textBox = textBox;
        }

        public void UpdateConsoleOutput(TextWriter standard)
        {
            this.standardOutput = standard;
        }

        private void WriteTextboxLine(string value)
        {
            textBox.SelectionColor = ((ForegroundColor == Color.White) ? Color.Black : ForegroundColor);
            textBox.AppendText(value + "\r\n");
            textBox.ScrollToCaret();
        }

        private void WriteConsoleLine(string value)
        {
            Console.ForegroundColor = ConvertColor2ConsoleColor(ForegroundColor);
            standardOutput.WriteLine(value);
            Console.ForegroundColor = ConsoleColor.White;
            ForegroundColor = Color.White;
        }

        private void WriteFileLine(string value)
        {
            logFile.Write(value + "\r\n");
        }

        public override void WriteLine(string value)
        {
            switch (OutputDestination)
            {
                case OutputDestination.OUT_TEXTBOX:
                    WriteTextboxLine(value);
                    break;
                case OutputDestination.OUT_CONSOLE:
                    WriteConsoleLine(value);
                    break;
                case OutputDestination.OUT_FILE:
                    WriteFileLine(value);
                    break;
                case OutputDestination.OUT_ALL:
                    WriteTextboxLine(value);
                    WriteConsoleLine(value);
                    WriteFileLine(value);
                    break;
                default:
                    throw new ApplicationException("Invalid output destination for log message");
            }

        }

        private static ConsoleColor ConvertColor2ConsoleColor(Color color)
        {
            switch (color.ToKnownColor())
            {
                case KnownColor.DarkBlue:
                    return ConsoleColor.DarkBlue;
                case KnownColor.Green:
                case KnownColor.DarkGreen:
                    return ConsoleColor.Green;
                case KnownColor.Red:
                    return ConsoleColor.Red;
                case KnownColor.DarkCyan:
                    return ConsoleColor.DarkCyan;
                case KnownColor.RoyalBlue:
                case KnownColor.Yellow:
                case KnownColor.Orange:
                    return ConsoleColor.Yellow;
                case KnownColor.Blue:
                    return ConsoleColor.Blue;
                case KnownColor.Black:
                    return ConsoleColor.White;
                default:
                    return ConsoleColor.White;
            }
        }

        public override Encoding Encoding
        {
            get { return Encoding.Default; }
        }

        protected override void Dispose(bool disposing)
        {
            standardOutput.Dispose();
        }

        public static string GetTimeStamp()
        {
            return DateTime.Now.ToLocalTime().ToString();
        }
    }
}
