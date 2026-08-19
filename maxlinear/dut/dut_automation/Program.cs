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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Automation;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Windows.Forms;
using DUT_GUI;
using dut;

namespace dut_automation
{
    class Program
    {
        enum testcasesDef
        {
            TC_SHORT = 0,
            TC_SHORT2G = 1,
            TC_SHORT5G = 2,
            TC_SHORT2G5G = 3,
            TC_LONG2G11B = 4,
            TC_LONG2G11G = 5,
            TC_LONG2G11N = 6,
            TC_LONG2G11AX = 7,
            TC_LONG5G11A = 8,
            TC_LONG5G11N = 9,
            TC_LONG5G11AC = 10,
            TC_LONG5G11AX = 11,
            TC_LONG2G = 12,
            TC_LONG5G = 13,
            TC_LONG2G5G = 14,
            TC_SHORT6G = 15,
            TC_LONG6G = 16,
        }

        const string version = "0.01";

        const testcasesDef defaultCase = testcasesDef.TC_SHORT;
        const string defaultPath = "C:\\Program Files\\MaxLinear\\DUT\\dut_gui.exe";
        const string defaultIP = "192.168.1.1";
        const int defaultWlan = 0;
        static Program mTest;
        static Process ps;
        static bool isPsActive = false;
        static DirectoryInfo logDir;
        static string logBaseDir = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "\\dut_automation_log";
        static string dutLogFile = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "\\DUT_GUI_console.txt";
        public static string dutAutomationLogFile = logBaseDir + "\\dut_automation_log.txt";

        AutomationElement combox_basicOp_band;
        AutomationElement combox_basicOp_phyType;
        AutomationElement combox_basicOp_lowChan;
        AutomationElement combox_basicOp_MCS;
        AutomationElement combox_basicOp_spectrumBW;
        AutomationElement combox_basicOp_signalBW;
        AutomationElement combox_basicOp_spatialStream;
        AutomationElement combox_basicOp_ltf;
        AutomationElement combox_basicOp_GI;

        AutomationElement cmd_basicOp_setChannel;
        AutomationElement cmd_basicOp_setRate;
        AutomationElement cmd_basicOp_setLoop;
        AutomationElement cmd_basicOp_setPower;
        AutomationElement cmd_basicOp_setSpacing;
        AutomationElement cmd_basicOp_startTransmitting;
        AutomationElement cmd_basicOp_stopTransmission;
        AutomationElement cmd_basicOp_updateInput;
        AutomationElement cmd_basicOp_resetCounter;
        AutomationElement cmd_basicOp_updateRXevm;
        AutomationElement cmd_basicOp_transmitCW;
        AutomationElement cmd_basicOp_stopCW;
        AutomationElement cmd_basicOp_startSpaceless;
        AutomationElement cmd_basicOp_stopSpaceless;
        AutomationElement[] cmd_basicOp_txAnt;
        AutomationElement[] cmd_basicOp_rxAnt;

        AutomationElement txtBox_basicOp_cwAmplitude;
        AutomationElement txtBox_basicOp_cwBinNum;
        AutomationElement dllConsole;
        static int Main(string[] args)
        {
            int exitCode = -1;

            DUTConsoleOutput dutConsoleWriter;
            DUTConsoleInput dutConsoleReader;
            if (File.Exists(dutAutomationLogFile))
            {
                File.Delete(dutAutomationLogFile);
            }
            if (File.Exists(dutLogFile))
            {
                File.Delete(dutLogFile);
            }
            logDir = Directory.CreateDirectory(logBaseDir + "\\" + DateTime.Now.ToString("yyyyMMddHHmmssfff"));

            Log dut_log = new Log();
            dutConsoleWriter = new DUTConsoleOutput(Console.Out, dut_log);
            dutConsoleReader = new DUTConsoleInput(Console.In);
            Console.SetOut(dutConsoleWriter);
            Console.SetIn(dutConsoleReader);

            System.Timers.Timer t = new System.Timers.Timer(1000);
            testcasesDef caseIndex = defaultCase;

            string dutGUIPath = defaultPath;
            string ipAddr = defaultIP;
            int wlanIndex = defaultWlan;

            mTest = new Program();
            try
            {
                Console.WriteLine("DUT Automation tool version: " + version);

                if (args.Length > 0)
                {
                    if (args[0] == "-help")
                    {
                        Console.WriteLine("-path [FILEPATH] \n//Set file path of the DUT_GUI.exe application to execute\n\n-case [SHORT][SHORT2G][SHORT5G][SHORT2G5G][SHORT6G][LONG2G11B][LONG2G11G][LONG2G11N][LONG2G11AX][LONG5G11A][LONG5G11N][LONG5G11AC][LONG5G11AX][LONG2G][LONG5G][LONG2G5G]  [LONG6G]\n//Set test cases: \n[SHORT]start TX/stop with the default GUI setting; \n[SHORT2G]start TX/stop with loop all phyType supported 2G and default channel/mcs; \n[SHORT5G]start TX/stop with loop all phyType supported 5G and default channel/mcs; \n[SHORT2G5G]start TX/stop with loop all phyType supported 2G&5G and default channel/mcs; \n[SHORT6G]start TX/stop with loop all phyType supported 6G and default channel/mcs; \n[LONG2G11B]loop all GUI setting for band 2G & phyType 11B;\n[LONG2G11G]loop all GUI setting for band 2G & phyType 11G;\n[LONG2G11N]loop all GUI setting for band 2G & phyType 11N;\n[LONG2G11AX]loop all GUI setting for band 2G & phyType 11AX;\n[LONG5G11A]loop all GUI setting for band 5G & phyType 11A;\n[LONG5G11N]loop all GUI setting for band 5G & phyType 11N;\n[LONG5G11AC]loop all GUI setting for band 5G & phyType 11AC;\n[LONG5G11AX]loop all GUI setting for band 5G & phyType 11AX; \n[LONG2G]loop all GUI setting for 2G band; \n[LONG5G]loop all GUI setting for 5G band; \n[LONG2G5G]loop all GUI setting for 2G&5G band;\n[LONG6G]loop all GUI setting for 6G band; \n\n-ip [IP address]\n//Optional; default is 192.168.1.1 \n\n-wlan [Wlan index:0/2/4]\n//Optional; default is wlan0\n\nExample: \ndut_automation.exe -path dut_gui.exe -case SHORT \ndut_automation.exe -path dut_gui.exe -case SHORT -ip 192.168.1.1 -wlan 0");
                        System.Environment.Exit(-1);
                    }
                    mTest.prepareTestConditions(args, out dutGUIPath, out caseIndex, out ipAddr, out wlanIndex);
                    Console.WriteLine("Customize test, caseIndex: " + caseIndex + ",dutGUIPath: " + dutGUIPath);
                }
                else
                {
                    Console.WriteLine("Default short test, dutGUIPath: " + dutGUIPath);
                }
                Directory.SetCurrentDirectory(logBaseDir);
                DateTime dateBegin = DateTime.Now;
                Console.WriteLine("\nDUT automation test Start! @" + dateBegin.ToLocalTime().ToString());
                ps = Process.Start(dutGUIPath);
                Thread.Sleep(3000);
                isPsActive = true;


                AutomationElement mMainWindow = AutomationElement.FromHandle(ps.MainWindowHandle);
                //Console.WriteLine("Main Window Handle: " + mMainWindow.Current.NativeWindowHandle.ToString());
                Console.WriteLine("[Current DUT version]:" + mMainWindow.Current.Name);
                Console.WriteLine("[Baseline DUT version]:" + dut_cli.Dut.getVersion());

                AutomationElement combox_nvMemCtrl_printLevel = mTest.GetComboBoxHandle(mMainWindow, "combox_nvMemCtrl_printLevel");
                Console.WriteLine("Get print level ComboBox Handle: " + combox_nvMemCtrl_printLevel.Current.NativeWindowHandle.ToString());

                AutomationElement combox_DebugConsole = mTest.GetComboBoxHandle(mMainWindow, "combox_DebugConsole");
                Console.WriteLine("Get debug console ComboBox Handle: " + combox_DebugConsole.Current.NativeWindowHandle.ToString());

                AutomationElement ipAddress = mTest.GetTextBoxHandle(mMainWindow, "ipAddress");
                Console.WriteLine("Get ipAddress TextBox Handle: " + ipAddress.Current.NativeWindowHandle.ToString());

                AutomationElement comboBox_basicOp_memoryType = mTest.GetComboBoxHandle(mMainWindow, "comboBox_basicOp_memoryType");
                Console.WriteLine("Get memory type ComboBox Handle: " + comboBox_basicOp_memoryType.Current.NativeWindowHandle.ToString());

                AutomationElement ConnectButton = mTest.GetButtonHandle(mMainWindow, "ConnectButton");
                Console.WriteLine("Get ConnectButton Button Handle: " + ConnectButton.Current.NativeWindowHandle.ToString());

                AutomationElement wlan0card = mTest.GetRadioButtonHandle(mMainWindow, "wlan0card");
                Console.WriteLine("Get wlan0card Button Handle: " + wlan0card.Current.NativeWindowHandle.ToString());

                AutomationElement wlan2card = mTest.GetRadioButtonHandle(mMainWindow, "wlan2card");
                Console.WriteLine("Get wlan2card Button Handle: " + wlan2card.Current.NativeWindowHandle.ToString());

                AutomationElement wlan4card = mTest.GetRadioButtonHandle(mMainWindow, "wlan4card");
                Console.WriteLine("Get wlan4card Button Handle: " + wlan4card.Current.NativeWindowHandle.ToString());

                Console.WriteLine("Set ipAddress Value: " + ipAddr);
                mTest.SetTextEditData(ipAddress, ipAddr);

                Console.WriteLine("Set combox_nvMemCtrl_printLevel Value TXT_PRINTLEVEL_INFO");
                mTest.SetSelectedComboBoxItem(combox_nvMemCtrl_printLevel, DUT_GUI.GUI_Constants.TXT_PRINTLEVEL_INFO);

                Console.WriteLine("Set combox_DebugConsole Value All");
                mTest.SetSelectedComboBoxItem(combox_DebugConsole, DUT_GUI.GUI_Constants.TXT_DBGOUT_ALL);

                Console.WriteLine("Set comboBox_basicOp_memoryType Value flash");
                mTest.SetComboBoxItemData(comboBox_basicOp_memoryType, DUT_GUI.GUI_Constants.TXT_MEMTYPE_FLASH);

                Console.WriteLine("Push Mouse Left Click-> wlan" + wlanIndex + "card");

                switch (wlanIndex)
                {
                    case 0:
                        mTest.ClickButtonUsingUIAutomation(wlan0card);
                        break;
                    case 2:
                        mTest.ClickButtonUsingUIAutomation(wlan2card);
                        break;
                    case 4:
                        mTest.ClickButtonUsingUIAutomation(wlan4card);
                        break;
                    default:
                        throw new Exception(string.Format("wlanIndex not supported:" + wlanIndex));
                }

                Console.WriteLine("Push Mouse Left Click-> ConnectButton");
                mTest.ButtonLeftClick(ConnectButton);
                DateTime tmpBegin = DateTime.Now;

                //Thread.Sleep(3000);
                Console.WriteLine("\nPrepare control handler->Start");
                mTest.prepareControlHandlerRes(mMainWindow);

                DateTime tmpEnd = DateTime.Now;

                //AutomationElement tabControl = mTest.GetTabHandle(mMainWindow, "tabControl");
                //mTest.SetSelectedTabItem(tabControl, "NV Memory Control & DUT Version");
                //Thread.Sleep(1000);
                //mTest.txtBox_nvMemCtrl_CVVersion = mTest.GetTextBoxHandle(mMainWindow, "txtBox_nvMemCtrl_CVVersion");
                //string CVVersion = mTest.GetEditDataText(mTest.txtBox_nvMemCtrl_CVVersion);
                //mTest.SetSelectedTabItem(tabControl, "Basic Operation");
                //Thread.Sleep(1000);
                //Console.WriteLine("[Combined Version]:" + CVVersion);

                Console.WriteLine("\nPrepare control handler->End, Duration:" + tmpEnd.Subtract(tmpBegin).TotalSeconds);

                t.Elapsed += new System.Timers.ElapsedEventHandler(mTest.TimerCheckException);
                t.AutoReset = true;
                t.Enabled = true;

                Console.WriteLine("\n------Start run testcases-------");

                switch (caseIndex)
                {
                    case testcasesDef.TC_SHORT:
                        mTest.Runcase_TX(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);
                        break;
                    case testcasesDef.TC_SHORT2G:
                        mTest.Runcase_2G_80211B_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_2G_80211G_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_2G_80211N_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_2G_80211AX_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        break;
                    case testcasesDef.TC_SHORT5G:
                        mTest.Runcase_5G_80211A_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_5G_80211N_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_5G_80211AC_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_5G_80211AX_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        break;
                    case testcasesDef.TC_SHORT2G5G:
                        mTest.Runcase_2G_80211B_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 5);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_2G_80211G_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 5);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_2G_80211N_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 5);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_2G_80211AX_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 5);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_5G_80211A_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 10);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_5G_80211N_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 10);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_5G_80211AC_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 10);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_5G_80211AX_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 10);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        break;
                    case testcasesDef.TC_LONG2G11B:
                        mTest.Runcase_2G_80211B(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);
                        break;
                    case testcasesDef.TC_LONG2G11G:
                        mTest.Runcase_2G_80211G(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);
                        break;
                    case testcasesDef.TC_LONG2G11N:
                        mTest.Runcase_2G_80211N(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);
                        break;
                    case testcasesDef.TC_LONG2G11AX:
                        mTest.Runcase_2G_80211AX(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);
                        break;
                    case testcasesDef.TC_LONG5G11A:
                        mTest.Runcase_5G_80211A(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);
                        break;

                    case testcasesDef.TC_LONG5G11N:
                        mTest.Runcase_5G_80211N(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);
                        break;

                    case testcasesDef.TC_LONG5G11AC:
                        mTest.Runcase_5G_80211AC(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);
                        break;

                    case testcasesDef.TC_LONG5G11AX:
                        mTest.Runcase_5G_80211AX(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);
                        break;

                    case testcasesDef.TC_LONG2G:
                        mTest.Runcase_2G_80211B(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_2G_80211G(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_2G_80211N(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_2G_80211AX(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);
                        break;
                    case testcasesDef.TC_LONG5G:
                        mTest.Runcase_5G_80211A(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_5G_80211N(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_5G_80211AC(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_5G_80211AX(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);
                        break;
                    case testcasesDef.TC_LONG2G5G:
                        mTest.Runcase_2G_80211B(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 5);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_2G_80211G(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 5);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_2G_80211N(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 5);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_2G_80211AX(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 5);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_5G_80211A(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 10);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_5G_80211N(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 10);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_5G_80211AC(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 10);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        mTest.Runcase_5G_80211AX(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 10);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);
                        break;
                    case testcasesDef.TC_SHORT6G:
                        mTest.Runcase_6G_80211AX_Once(mMainWindow);
                        mTest.Runcase_Ant(mMainWindow, 15);
                        mTest.Runcase_GenRiscCWSpaceless(mMainWindow);

                        break;
                    case testcasesDef.TC_LONG6G:
                        mTest.Runcase_6G_80211AX(mMainWindow);
                        //mTest.Runcase_Ant(mMainWindow,15);
                        //mTest.Runcase_GenRiscCWSpaceless(mMainWindow);
                        break;
                }
                DateTime dateEnd = DateTime.Now;
                Console.WriteLine("\nDUT automation test End! @" + dateEnd.ToLocalTime().ToString() + "; Total Duration:" + mTest.ExecTimeDiff(dateBegin, dateEnd));
                Console.WriteLine("\nFinal result-> #Complete!#");
                Console.WriteLine("\n Please check all log here: " + logDir.FullName);
                exitCode = 0;
            }
            catch (System.Exception ex)
            {
                Console.WriteLine("\nFinal result-> #Incomplete!#");
                Console.WriteLine("\nException info:" + ex.Message + "@" + DateTime.Now.ToLocalTime().ToString());
            }
            finally
            {
                t.AutoReset = false;
                t.Enabled = false;
                dutConsoleWriter.Close();
                dutConsoleReader.Close();
                dut_log.Close();
                mTest.GetScreenCapture().Save(logDir.FullName + "\\snapshot.png");

                File.Copy(dutAutomationLogFile, logDir.FullName + "\\dut_automation_log.txt");
                if (isPsActive)
                {
                    ps.CloseMainWindow();
                    ps.Close();
                    Thread.Sleep(1000);
                }
                if (File.Exists(dutLogFile))
                {
                    File.Copy(dutLogFile, logDir.FullName + "\\DUT_GUI_console.txt");
                }

                //Console.ReadKey(true);
            }

            return exitCode;
        }

        private void TimerCheckException(object sender, System.Timers.ElapsedEventArgs e)
        {
            string result = GetEditDataText(dllConsole);

            if (result.Contains("FAIL") || result.Contains("Got a wrong message ID") || result.Contains("Dut_CopyLastError"))
            {
                Console.WriteLine("Some error printed in DUT console log, Please check keywords=FAIL/Got a wrong message ID/Dut_CopyLastError");
                GetScreenCapture().Save(logDir.FullName + "\\snapshot-dut-fail.png");
                ps.CloseMainWindow();
                ps.Close();
                isPsActive = false;
            }

        }
        public string ExecTimeDiff(DateTime dateBegin, DateTime dateEnd)
        {
            TimeSpan ts1 = new TimeSpan(dateBegin.Ticks);
            TimeSpan ts2 = new TimeSpan(dateEnd.Ticks);
            TimeSpan ts3 = ts1.Subtract(ts2).Duration();
            return ts3.ToString("c").Substring(0, 8);
        }
        private Bitmap GetScreenCapture()
        {
            Rectangle tScreenRect = new Rectangle(0, 0, SystemInformation.WorkingArea.Width, SystemInformation.WorkingArea.Height);
            Bitmap tSrcBmp = new Bitmap(tScreenRect.Width, tScreenRect.Height);
            Graphics gp = Graphics.FromImage(tSrcBmp);
            gp.CopyFromScreen(0, 0, 0, 0, tScreenRect.Size);
            gp.DrawImage(tSrcBmp, 0, 0, tScreenRect, GraphicsUnit.Pixel);

            return tSrcBmp;
        }
        public void prepareControlHandlerRes(AutomationElement rootElement)
        {
            combox_basicOp_band = GetComboBoxHandle(rootElement, "combox_basicOp_band");
            combox_basicOp_phyType = GetComboBoxHandle(rootElement, "combox_basicOp_phyType");
            combox_basicOp_lowChan = GetComboBoxHandle(rootElement, "combox_basicOp_lowChan");
            combox_basicOp_MCS = GetComboBoxHandle(rootElement, "combox_basicOp_MCS");
            combox_basicOp_spectrumBW = GetComboBoxHandle(rootElement, "combox_basicOp_spectrumBW");
            combox_basicOp_spectrumBW = GetComboBoxHandle(rootElement, "combox_basicOp_spectrumBW");
            combox_basicOp_signalBW = GetComboBoxHandle(rootElement, "combox_basicOp_signalBW");
            combox_basicOp_spatialStream = GetComboBoxHandle(rootElement, "combox_basicOp_spatialStream");
            combox_basicOp_ltf = GetComboBoxHandle(rootElement, "combox_basicOp_ltf");
            combox_basicOp_GI = GetComboBoxHandle(rootElement, "combox_basicOp_GI");

            cmd_basicOp_setChannel = GetButtonHandle(rootElement, "cmd_basicOp_setChannel");
            cmd_basicOp_setRate = GetButtonHandle(rootElement, "cmd_basicOp_setRate");
            cmd_basicOp_setLoop = GetButtonHandle(rootElement, "cmd_basicOp_setLoop");
            cmd_basicOp_setPower = GetButtonHandle(rootElement, "cmd_basicOp_setPower");
            cmd_basicOp_setSpacing = GetButtonHandle(rootElement, "cmd_basicOp_setSpacing");
            cmd_basicOp_startTransmitting = GetButtonHandle(rootElement, "cmd_basicOp_startTransmitting");
            cmd_basicOp_stopTransmission = GetButtonHandle(rootElement, "cmd_basicOp_stopTransmission");
            cmd_basicOp_updateInput = GetButtonHandle(rootElement, "cmd_basicOp_updateInput");
            cmd_basicOp_resetCounter = GetButtonHandle(rootElement, "cmd_basicOp_resetCounter");
            cmd_basicOp_updateRXevm = GetButtonHandle(rootElement, "cmd_basicOp_updateRXevm");
            cmd_basicOp_transmitCW = GetButtonHandle(rootElement, "cmd_basicOp_transmitCW");
            cmd_basicOp_stopCW = GetButtonHandle(rootElement, "cmd_basicOp_stopCW");
            cmd_basicOp_startSpaceless = GetButtonHandle(rootElement, "cmd_basicOp_startSpaceless");
            cmd_basicOp_stopSpaceless = GetButtonHandle(rootElement, "cmd_basicOp_stopSpaceless");
            cmd_basicOp_txAnt = new AutomationElement[4];
            cmd_basicOp_txAnt[0] = GetButtonHandle(rootElement, "cmd_basicOp_txAnt0");
            cmd_basicOp_txAnt[1] = GetButtonHandle(rootElement, "cmd_basicOp_txAnt1");
            cmd_basicOp_txAnt[2] = GetButtonHandle(rootElement, "cmd_basicOp_txAnt2");
            cmd_basicOp_txAnt[3] = GetButtonHandle(rootElement, "cmd_basicOp_txAnt3");

            cmd_basicOp_rxAnt = new AutomationElement[5];
            cmd_basicOp_rxAnt[0] = GetButtonHandle(rootElement, "cmd_basicOp_rxAnt0");
            cmd_basicOp_rxAnt[1] = GetButtonHandle(rootElement, "cmd_basicOp_rxAnt1");
            cmd_basicOp_rxAnt[2] = GetButtonHandle(rootElement, "cmd_basicOp_rxAnt2");
            cmd_basicOp_rxAnt[3] = GetButtonHandle(rootElement, "cmd_basicOp_rxAnt3");
            cmd_basicOp_rxAnt[4] = GetButtonHandle(rootElement, "cmd_basicOp_rxAnt4");

            txtBox_basicOp_cwAmplitude = GetTextBoxHandle(rootElement, "txtBox_basicOp_cwAmplitude");
            txtBox_basicOp_cwBinNum = GetTextBoxHandle(rootElement, "txtBox_basicOp_cwBinNum");
            dllConsole = GetTextBoxHandle(rootElement, "dllConsole");

        }
        private void prepareTestConditions(string[] args, out string dutGUIPath, out testcasesDef caseIndex, out string ipAddr, out int wlanIndex)
        {
            dutGUIPath = defaultPath;
            caseIndex = defaultCase;
            ipAddr = defaultIP;
            wlanIndex = defaultWlan;

            if (args.Length > 0)
            {
                int pIndex = args.ToList().IndexOf("-path");
                if ((pIndex > -1) && ((pIndex + 1) < args.Length))
                {
                    dutGUIPath = args[pIndex + 1];
                    if (!Path.IsPathRooted(dutGUIPath))
                    {
                        dutGUIPath = Path.Combine(Directory.GetCurrentDirectory(), dutGUIPath);
                    }
                }
                else
                {
                    throw new Exception(string.Format("Failed to recognize the -path value: " + args[pIndex + 1]));
                }

                pIndex = args.ToList().IndexOf("-case");
                if ((pIndex > -1) && (pIndex + 1) < args.Length)
                {
                    switch (args[pIndex + 1])
                    {
                        case "SHORT":
                            caseIndex = testcasesDef.TC_SHORT;
                            break;
                        case "SHORT2G":
                            caseIndex = testcasesDef.TC_SHORT2G;
                            break;
                        case "SHORT5G":
                            caseIndex = testcasesDef.TC_SHORT5G;
                            break;
                        case "SHORT6G":
                            caseIndex = testcasesDef.TC_SHORT6G;
                            break;
                        case "SHORT2G5G":
                            caseIndex = testcasesDef.TC_SHORT2G5G;
                            break;
                        case "LONG2G11B":
                            caseIndex = testcasesDef.TC_LONG2G11B;
                            break;
                        case "LONG2G11G":
                            caseIndex = testcasesDef.TC_LONG2G11G;
                            break;
                        case "LONG2G11N":
                            caseIndex = testcasesDef.TC_LONG2G11N;
                            break;
                        case "LONG2G11AX":
                            caseIndex = testcasesDef.TC_LONG2G11AX;
                            break;
                        case "LONG5G11A":
                            caseIndex = testcasesDef.TC_LONG5G11A;
                            break;
                        case "LONG5G11N":
                            caseIndex = testcasesDef.TC_LONG5G11N;
                            break;
                        case "LONG5G11AC":
                            caseIndex = testcasesDef.TC_LONG5G11AC;
                            break;
                        case "LONG5G11AX":
                            caseIndex = testcasesDef.TC_LONG5G11AX;
                            break;
                        case "LONG2G":
                            caseIndex = testcasesDef.TC_LONG2G;
                            break;
                        case "LONG5G":
                            caseIndex = testcasesDef.TC_LONG5G;
                            break;
                        case "LONG2G5G":
                            caseIndex = testcasesDef.TC_LONG2G5G;
                            break;
                        case "LONG6G":
                            caseIndex = testcasesDef.TC_LONG6G;
                            break;
                        default:
                            throw new Exception(string.Format("Failed to recognize the -case value: " + args[pIndex + 1]));
                    }
                }
                else
                {
                    throw new Exception(string.Format("Failed to recognize the -case value: " + args[pIndex + 1]));
                }

                pIndex = args.ToList().IndexOf("-ip");
                if ((pIndex > -1) && (pIndex + 1) < args.Length)
                {
                    ipAddr = args[pIndex + 1];
                }
                pIndex = args.ToList().IndexOf("-wlan");
                if ((pIndex > -1) && (pIndex + 1) < args.Length)
                {
                    wlanIndex = Convert.ToInt32(args[pIndex + 1]);
                }

            }

        }
        public void SetSelectedComboBoxItem(AutomationElement comboBox, string item)
        {
            AutomationElement listItem = comboBox.FindFirst(TreeScope.Subtree, new PropertyCondition(AutomationElement.NameProperty, item));
            if (listItem.Current.IsEnabled)
            {
                AutomationPattern automationPatternFromElement = GetSpecifiedPattern(comboBox, "ExpandCollapsePatternIdentifiers.Pattern");
                ExpandCollapsePattern expandCollapsePattern = comboBox.GetCurrentPattern(automationPatternFromElement) as ExpandCollapsePattern;
                expandCollapsePattern.Expand();
                expandCollapsePattern.Collapse();

                //listItem = comboBox.FindFirst(TreeScope.Subtree, new PropertyCondition(AutomationElement.NameProperty, item));

                automationPatternFromElement = GetSpecifiedPattern(listItem, "SelectionItemPatternIdentifiers.Pattern");

                SelectionItemPattern selectionItemPattern = listItem.GetCurrentPattern(automationPatternFromElement) as SelectionItemPattern;

                selectionItemPattern.Select();
            }
            else
            {
                Console.WriteLine("SetSelectedComboBoxItem - ComboBox Disabled!");
            }
        }
        public void SetSelectedTabItem(AutomationElement tabControl, string item)
        {
            AutomationElement listItem = tabControl.FindFirst(TreeScope.Subtree, new PropertyCondition(AutomationElement.NameProperty, item));
            if (listItem.Current.IsEnabled)
            {
                AutomationPattern automationPatternFromElement = GetSpecifiedPattern(listItem, "SelectionItemPatternIdentifiers.Pattern");

                SelectionItemPattern selectionItemPattern = listItem.GetCurrentPattern(automationPatternFromElement) as SelectionItemPattern;

                selectionItemPattern.Select();
            }
            else
            {
                Console.WriteLine("SetSelectedTabItem - tabControl Disabled!");
            }
        }
        private AutomationPattern GetSpecifiedPattern(AutomationElement element, string patternName)
        {
            AutomationPattern[] supportedPattern = element.GetSupportedPatterns();

            foreach (AutomationPattern pattern in supportedPattern)
            {
                if (pattern.ProgrammaticName == patternName)
                    return pattern;
            }

            return null;
        }
        public SelectionItemPattern GetSelectionItemPattern(AutomationElement element)
        {
            object currentPattern;
            if (!element.TryGetCurrentPattern(SelectionItemPattern.Pattern, out currentPattern))
            {
                throw new Exception(string.Format("Element with AutomationId '{0}' and Name '{1}' does not support the SelectionItemPattern.",
                element.Current.AutomationId, element.Current.Name));
            }
            return currentPattern as SelectionItemPattern;
        }
        public ExpandCollapsePattern GetExpandCollapsePattern(AutomationElement targetControl)
        {
            ExpandCollapsePattern expandCollapsePattern = null;
            try
            {

                expandCollapsePattern =

                targetControl.GetCurrentPattern(

                ExpandCollapsePattern.Pattern)

                as ExpandCollapsePattern;

            }

            // Object doesn't support the ExpandCollapsePattern control pattern.

            catch (InvalidOperationException)
            {
                return null;
            }

            return expandCollapsePattern;
        }

        private string GetEditDataText(AutomationElement txtElement)
        {
            if (txtElement == null)
                return "";
            TextPattern txtText = (TextPattern)txtElement.GetCurrentPattern(TextPattern.Pattern);
            string strText = txtText.DocumentRange.GetText(-1);
            return strText;
        }

        public AutomationElement GetTextBoxHandle(AutomationElement rootElement, string controlName)
        {
            try
            {
                if ((controlName == "") || (rootElement == null))
                {
                    throw new ArgumentException("Argument cannot be null or empty.");
                }
                Condition propCondition = new PropertyCondition(
                AutomationElement.AutomationIdProperty, controlName, PropertyConditionFlags.IgnoreCase);

                return rootElement.FindFirst(TreeScope.Element | TreeScope.Subtree, propCondition);
            }
            catch (System.Exception ex)
            {
                string msg = "Can't find the TextBox name:" + controlName;
                throw new InvalidProgramException(msg, ex);
            }
        }

        public bool SetTextEditData(AutomationElement TextEditHandle, string strData)
        {
            ValuePattern vpTextEdit = null;

            if (!TextEditHandle.Current.IsEnabled)
            {
                throw new InvalidOperationException("The control is not enabled.\n\n");
            }

            if (!TextEditHandle.Current.IsKeyboardFocusable)
            {
                throw new InvalidOperationException("The control is not focusable.\n\n");
            }

            vpTextEdit = TextEditHandle.GetCurrentPattern(ValuePattern.Pattern) as ValuePattern;
            if (null == vpTextEdit)
            {
                return false;
            }

            if (vpTextEdit.Current.IsReadOnly)
            {
                throw new InvalidOperationException("The control is read-only.");
            }

            vpTextEdit.SetValue(strData);

            return true;
        }

        public AutomationElement GetComboBoxHandle(AutomationElement rootElement, string controlName)
        {
            try
            {
                if ((controlName == "") || (rootElement == null))
                {
                    throw new ArgumentException("Argument cannot be null or empty.");
                }

                PropertyCondition mNameCondition = new PropertyCondition(AutomationElement.AutomationIdProperty, controlName);
                PropertyCondition mTypeCondition = new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.ComboBox);

                AutomationElement ComboBoxHandle = rootElement.FindFirst(TreeScope.Subtree, new AndCondition(mNameCondition, mTypeCondition));
                if (null == ComboBoxHandle)
                {
                    return null;
                }

                return ComboBoxHandle;
            }
            catch (System.Exception ex)
            {
                string msg = "Can't find the ComboBox name:" + controlName;
                throw new InvalidProgramException(msg, ex);
            }
        }

        public bool SetComboBoxItemData(AutomationElement ComboBoxHandle, string strData)
        {
            AutomationElement TextEditHandle = null;
            PropertyCondition TypeCondition = null;
            ValuePattern vpTextPattern = null;

            if (null == ComboBoxHandle || null == strData)
            {
                return false;
            }

            TypeCondition = new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Edit);
            TextEditHandle = ComboBoxHandle.FindFirst(TreeScope.Subtree, TypeCondition);
            if (null == TextEditHandle)
            {
                return false;
            }

            if (!TextEditHandle.Current.IsEnabled)
            {
                throw new InvalidOperationException("The control is not enabled.\n\n");
            }

            if (!TextEditHandle.Current.IsKeyboardFocusable)
            {
                throw new InvalidOperationException("The control is not focusable.\n\n");
            }

            vpTextPattern = TextEditHandle.GetCurrentPattern(ValuePattern.Pattern) as ValuePattern;
            if (null == vpTextPattern)
            {
                return false;
            }
            if (vpTextPattern.Current.IsReadOnly)
            {
                throw new InvalidOperationException("The control is read-only.");
            }
            vpTextPattern.SetValue(strData);
            return true;
        }

        public AutomationElement GetRadioButtonHandle(AutomationElement rootElement, string controlName)
        {
            try
            {
                if ((controlName == "") || (rootElement == null))
                {
                    throw new ArgumentException("Argument cannot be null or empty.");
                }

                PropertyCondition mNameCondition = new PropertyCondition(AutomationElement.AutomationIdProperty, controlName);
                PropertyCondition mTypeCondition = new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.RadioButton);

                AutomationElement ComboBoxHandle = rootElement.FindFirst(TreeScope.Subtree, new AndCondition(mNameCondition, mTypeCondition));
                if (null == ComboBoxHandle)
                {
                    return null;
                }

                return ComboBoxHandle;
            }
            catch (System.Exception ex)
            {
                string msg = "Can't find the GroupBox name:" + controlName;
                throw new InvalidProgramException(msg, ex);
            }
        }

        public AutomationElement GetButtonHandle(AutomationElement parentWindowHandle, string sAutomationID)
        {

            try
            {
                if ((sAutomationID == "") || (parentWindowHandle == null))
                {
                    throw new ArgumentException("Argument cannot be null or empty.");
                }

                PropertyCondition mNameCondition = new PropertyCondition(AutomationElement.AutomationIdProperty, sAutomationID);
                PropertyCondition mButtonCondition = new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Button);

                return parentWindowHandle.FindFirst(TreeScope.Subtree, new AndCondition(mNameCondition, mButtonCondition));
            }
            catch (System.Exception ex)
            {
                string msg = "Can't find the button name:" + sAutomationID;
                throw new InvalidProgramException(msg, ex);
            }
        }
        public AutomationElement GetTabHandle(AutomationElement parentWindowHandle, string sAutomationID)
        {

            try
            {
                if ((sAutomationID == "") || (parentWindowHandle == null))
                {
                    throw new ArgumentException("Argument cannot be null or empty.");
                }

                PropertyCondition mNameCondition = new PropertyCondition(AutomationElement.AutomationIdProperty, sAutomationID);
                PropertyCondition mTabCondition = new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Tab);

                return parentWindowHandle.FindFirst(TreeScope.Subtree, new AndCondition(mNameCondition, mTabCondition));
            }
            catch (System.Exception ex)
            {
                string msg = "Can't find the tab name:" + sAutomationID;
                throw new InvalidProgramException(msg, ex);
            }
        }
        public bool ButtonLeftClick(AutomationElement ButtonHandle)
        {
            object objButton = null;
            Console.WriteLine("ButtonLeftClick start");
            try
            {
                if (null == ButtonHandle)
                {
                    throw new ArgumentException("Argument cannot be null or empty.");
                }

                if (ButtonHandle.Current.IsEnabled)
                {
                    if (true == ButtonHandle.TryGetCurrentPattern(InvokePattern.Pattern, out objButton))
                    {
                        InvokePattern btnInvoke = objButton as InvokePattern; ;
                        btnInvoke.Invoke();

                        Console.WriteLine("ButtonLeftClick - OK - Sleep 2s for reponse");
                        Thread.Sleep(2000);
                        return true;
                    }
                    else
                    {
                        Console.WriteLine("ButtonLeftClick - fail!");
                        return false;
                    }
                }
                else
                {
                    Console.WriteLine("ButtonLeftClick - ButtonHandle Disabled!");
                    return false;
                }
            }
            catch (System.Exception ex)
            {
                string msg = "Can't click the button.";
                throw new InvalidProgramException(msg, ex);
            }
        }

        private void ClickButtonUsingUIAutomation(AutomationElement control)
        {
            // Test for the control patterns of interest for this sample. 
            object objPattern;
            try
            {
                if (null == control)
                {
                    throw new ArgumentException("Argument cannot be null or empty.");
                }
                ExpandCollapsePattern expcolPattern;
                if (true == control.TryGetCurrentPattern(ExpandCollapsePattern.Pattern, out objPattern))
                {
                    expcolPattern = objPattern as ExpandCollapsePattern;
                    if (expcolPattern.Current.ExpandCollapseState != ExpandCollapseState.LeafNode)
                    {
                        expcolPattern.Expand();

                    }
                }
                TogglePattern togPattern;
                if (true == control.TryGetCurrentPattern(TogglePattern.Pattern, out objPattern))
                {
                    togPattern = objPattern as TogglePattern;
                    togPattern.Toggle();
                }
                InvokePattern invPattern;
                if (true == control.TryGetCurrentPattern(InvokePattern.Pattern, out objPattern))
                {
                    invPattern = objPattern as InvokePattern;
                    invPattern.Invoke();
                }
                SelectionItemPattern selPattern;
                if (true == control.TryGetCurrentPattern(SelectionItemPattern.Pattern, out objPattern))
                {
                    selPattern = objPattern as SelectionItemPattern;
                    selPattern.Select();
                }
            }
            catch (System.Exception ex)
            {
                string msg = "Can't click the button.";
                throw new InvalidProgramException(msg, ex);
            }
        }
        private void Runcase_TX(AutomationElement rootElement)
        {
            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setChannel");
            ButtonLeftClick(cmd_basicOp_setChannel);

            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setRate");
            ButtonLeftClick(cmd_basicOp_setRate);


            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setLoop");
            ButtonLeftClick(cmd_basicOp_setLoop);


            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setPower");
            ButtonLeftClick(cmd_basicOp_setPower);


            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setSpacing");
            ButtonLeftClick(cmd_basicOp_setSpacing);


            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_startTransmitting -> 1s...");
            ButtonLeftClick(cmd_basicOp_startTransmitting);
            //Thread.Sleep(1000);


            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_stopTransmission");
            ButtonLeftClick(cmd_basicOp_stopTransmission);
        }
        private void Runcase_GenRiscCWSpaceless(AutomationElement rootElement)
        {
            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_updateInput");
            ButtonLeftClick(cmd_basicOp_updateInput);

            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_resetCounter");
            ButtonLeftClick(cmd_basicOp_resetCounter);

            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_updateRXevm");
            ButtonLeftClick(cmd_basicOp_updateRXevm);

            SetTextEditData(txtBox_basicOp_cwAmplitude, "10");
            Console.WriteLine("Set txtBox_basicOp_cwAmplitude: 10");

            Console.WriteLine("Set txtBox_basicOp_cwBinNum: 4");
            SetTextEditData(txtBox_basicOp_cwBinNum, "4");

            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_transmitCW");
            ButtonLeftClick(cmd_basicOp_transmitCW);

            //Thread.Sleep(1000);

            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_stopCW");
            ButtonLeftClick(cmd_basicOp_stopCW);

            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_startSpaceless");
            ButtonLeftClick(cmd_basicOp_startSpaceless);

            //Thread.Sleep(1000);

            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_stopSpaceless");
            ButtonLeftClick(cmd_basicOp_stopSpaceless);
        }

        private void Runcase_Ant(AutomationElement rootElement, int antMask)
        {
            if ((antMask & 1) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_txAnt0");
                ButtonLeftClick(cmd_basicOp_txAnt[0]);
            }
            if ((antMask & 2) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_txAnt1");
                ButtonLeftClick(cmd_basicOp_txAnt[1]);
            }
            if ((antMask & 4) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_txAnt2");
                ButtonLeftClick(cmd_basicOp_txAnt[2]);
            }
            if ((antMask & 8) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_txAnt3");
                ButtonLeftClick(cmd_basicOp_txAnt[3]);
            }

            if ((antMask & 1) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_rxAnt0");
                ButtonLeftClick(cmd_basicOp_rxAnt[0]);
            }
            if ((antMask & 2) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_rxAnt1");
                ButtonLeftClick(cmd_basicOp_rxAnt[1]);
            }
            if ((antMask & 4) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_rxAnt2");
                ButtonLeftClick(cmd_basicOp_rxAnt[2]);
            }
            if ((antMask & 8) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_rxAnt3");
                ButtonLeftClick(cmd_basicOp_rxAnt[3]);
            }
            if ((antMask & 10) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_rxAnt4");
                ButtonLeftClick(cmd_basicOp_rxAnt[4]);
            }

            if ((antMask & 1) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_txAnt0");
                ButtonLeftClick(cmd_basicOp_txAnt[0]);
            }
            if ((antMask & 2) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_txAnt1");
                ButtonLeftClick(cmd_basicOp_txAnt[1]);
            }
            if ((antMask & 4) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_txAnt2");
                ButtonLeftClick(cmd_basicOp_txAnt[2]);
            }
            if ((antMask & 8) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_txAnt3");
                ButtonLeftClick(cmd_basicOp_txAnt[3]);
            }

            if ((antMask & 1) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_rxAnt0");
                ButtonLeftClick(cmd_basicOp_rxAnt[0]);
            }
            if ((antMask & 2) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_rxAnt1");
                ButtonLeftClick(cmd_basicOp_rxAnt[1]);
            }
            if ((antMask & 4) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_rxAnt2");
                ButtonLeftClick(cmd_basicOp_rxAnt[2]);
            }
            if ((antMask & 8) != 0)
            {
                Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_rxAnt3");
                ButtonLeftClick(cmd_basicOp_rxAnt[3]);
            }

        }

        private void Set_TX_Once(AutomationElement rootElement)
        {

            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setChannel");
            ButtonLeftClick(cmd_basicOp_setChannel);


            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setRate");
            ButtonLeftClick(cmd_basicOp_setRate);


            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setLoop");
            ButtonLeftClick(cmd_basicOp_setLoop);


            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setPower");
            ButtonLeftClick(cmd_basicOp_setPower);


            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setSpacing");
            ButtonLeftClick(cmd_basicOp_setSpacing);


            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_startTransmitting -> 1s...");
            ButtonLeftClick(cmd_basicOp_startTransmitting);
            //Thread.Sleep(1000);


            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_stopTransmission");
            ButtonLeftClick(cmd_basicOp_stopTransmission);

        }
        private void Runcase_2G_80211B_Once(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211B");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211B);

            Set_TX_Once(rootElement);
        }
        private void Runcase_2G_80211G_Once(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211G");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211G);

            Set_TX_Once(rootElement);
        }
        private void Runcase_2G_80211N_Once(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211N_24GHZ");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211N_24GHZ);

            Set_TX_Once(rootElement);

        }
        private void Runcase_2G_80211AX_Once(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211AX");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211AX);

            Set_TX_Once(rootElement);
        }
        private void Runcase_5G_80211A_Once(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211A");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211A);

            Set_TX_Once(rootElement);

        }
        private void Runcase_5G_80211N_Once(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211N_5GHZ");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211N_5GHZ);

            Set_TX_Once(rootElement);

        }
        private void Runcase_5G_80211AC_Once(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211AC");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211AC);

            Set_TX_Once(rootElement);
        }
        private void Runcase_5G_80211AX_Once(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211AX");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211AX);

            Set_TX_Once(rootElement);
        }
        private void Runcase_2G_80211B(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211B");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211B);

            /*loop all parameters and do TX*/
            string[] mcs_2G11B = {
                DUT_GUI.GUI_Constants.TXT_MCSB_2MBPS_SHORT,
                DUT_GUI.GUI_Constants.TXT_MCSB_5MBPS_SHORT,
                DUT_GUI.GUI_Constants.TXT_MCSB_11MBPS_SHORT,
                DUT_GUI.GUI_Constants.TXT_MCSB_1MBPS_LONG,
                DUT_GUI.GUI_Constants.TXT_MCSB_2MBPS_LONG,
                DUT_GUI.GUI_Constants.TXT_MCSB_5MBPS_LONG,
                DUT_GUI.GUI_Constants.TXT_MCSB_11MBPS_LONG,
            };
            for (int channel = DUT_GUI.GUI_Constants.DEFAULT_CHANNEL_START; channel <= DUT_GUI.GUI_Constants.MAX_CHANNEL_2400MHZ; channel++)
            {
                foreach (string strMcs in mcs_2G11B)
                {

                    Console.WriteLine("Set combox_basicOp_lowChan Value:" + channel);
                    SetComboBoxItemData(combox_basicOp_lowChan, channel.ToString());


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setChannel");
                    ButtonLeftClick(cmd_basicOp_setChannel);


                    Console.WriteLine("Set combox_basicOp_MCS Value:" + strMcs);
                    SetComboBoxItemData(combox_basicOp_MCS, strMcs);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setRate");
                    ButtonLeftClick(cmd_basicOp_setRate);

                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setLoop");
                    ButtonLeftClick(cmd_basicOp_setLoop);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setPower");
                    ButtonLeftClick(cmd_basicOp_setPower);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setSpacing");
                    ButtonLeftClick(cmd_basicOp_setSpacing);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_startTransmitting -> 1s...");
                    ButtonLeftClick(cmd_basicOp_startTransmitting);
                    //Thread.Sleep(1000);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_stopTransmission");
                    ButtonLeftClick(cmd_basicOp_stopTransmission);

                }
            }


        }
        private void Runcase_2G_80211G(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211G");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211G);

            /*loop all parameters and do TX*/
            string[] mcs_2G11G = {
                DUT_GUI.GUI_Constants.TXT_MCS_BPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_BPSK34,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK34,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM12,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM23,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM34,
            };

            for (int channel = DUT_GUI.GUI_Constants.DEFAULT_CHANNEL_START; channel <= DUT_GUI.GUI_Constants.MAX_CHANNEL_2400MHZ; channel++)
            {
                foreach (string strMcs in mcs_2G11G)
                {

                    Console.WriteLine("Set combox_basicOp_lowChan Value:" + channel);
                    SetComboBoxItemData(combox_basicOp_lowChan, channel.ToString());


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setChannel");
                    ButtonLeftClick(cmd_basicOp_setChannel);


                    Console.WriteLine("Set combox_basicOp_MCS Value:" + strMcs);
                    SetComboBoxItemData(combox_basicOp_MCS, strMcs);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setRate");
                    ButtonLeftClick(cmd_basicOp_setRate);

                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setLoop");
                    ButtonLeftClick(cmd_basicOp_setLoop);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setPower");
                    ButtonLeftClick(cmd_basicOp_setPower);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setSpacing");
                    ButtonLeftClick(cmd_basicOp_setSpacing);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_startTransmitting -> 1s...");
                    ButtonLeftClick(cmd_basicOp_startTransmitting);
                    //Thread.Sleep(1000);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_stopTransmission");
                    ButtonLeftClick(cmd_basicOp_stopTransmission);

                }
            }


        }
        private void Runcase_2G_80211N(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211N_24GHZ");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211N_24GHZ);

            /*loop all parameters and do TX*/
            string[] mcs_2G11N = {
                DUT_GUI.GUI_Constants.TXT_MCS_BPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK34,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM12,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM23,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM56,
            };
            string[] bw_2G11N = {
                DUT_GUI.GUI_Constants.TXT_BW_20MHz,
                DUT_GUI.GUI_Constants.TXT_BW_40MHz
            };
            string[] gi_2G11N = {
                DUT_GUI.GUI_Constants.TXT_GI_08,
                DUT_GUI.GUI_Constants.TXT_GI_04
            };
            string[] ss_2G11N = {
                DUT_GUI.GUI_Constants.TXT_SPATIALSTREAM_1,
                DUT_GUI.GUI_Constants.TXT_SPATIALSTREAM_2
            };
            for (int channel = DUT_GUI.GUI_Constants.DEFAULT_CHANNEL_START; channel <= DUT_GUI.GUI_Constants.MAX_CHANNEL_2400MHZ; channel++)
            {
                foreach (string strMcs in mcs_2G11N)
                {
                    foreach (string strSpectrumBW in bw_2G11N)
                    {
                        foreach (string strSignalBW in bw_2G11N)
                        {
                            if ((strSpectrumBW == DUT_GUI.GUI_Constants.TXT_BW_20MHz) && (strSignalBW == DUT_GUI.GUI_Constants.TXT_BW_40MHz))
                            {
                                break;
                            }
                            foreach (string strGI in gi_2G11N)
                            {
                                foreach (string strSS in ss_2G11N)
                                {

                                    Console.WriteLine("Set combox_basicOp_lowChan Value:" + channel);
                                    SetComboBoxItemData(combox_basicOp_lowChan, channel.ToString());


                                    Console.WriteLine("Set combox_basicOp_spectrumBW Value:" + strSpectrumBW);
                                    SetComboBoxItemData(combox_basicOp_spectrumBW, strSpectrumBW);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setChannel");
                                    ButtonLeftClick(cmd_basicOp_setChannel);


                                    Console.WriteLine("Set combox_basicOp_signalBW Value:" + strSignalBW);
                                    SetComboBoxItemData(combox_basicOp_signalBW, strSignalBW);


                                    Console.WriteLine("Set combox_basicOp_MCS Value:" + strMcs);
                                    SetComboBoxItemData(combox_basicOp_MCS, strMcs);


                                    Console.WriteLine("Set combox_basicOp_spatialStream Value:" + strSS);
                                    SetComboBoxItemData(combox_basicOp_spatialStream, strSS);


                                    Console.WriteLine("Set combox_basicOp_GI Value:" + strGI);
                                    SetComboBoxItemData(combox_basicOp_GI, strGI);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setRate");
                                    ButtonLeftClick(cmd_basicOp_setRate);

                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setLoop");
                                    ButtonLeftClick(cmd_basicOp_setLoop);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setPower");
                                    ButtonLeftClick(cmd_basicOp_setPower);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setSpacing");
                                    ButtonLeftClick(cmd_basicOp_setSpacing);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_startTransmitting -> 1s...");
                                    ButtonLeftClick(cmd_basicOp_startTransmitting);
                                    //Thread.Sleep(1000);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_stopTransmission");
                                    ButtonLeftClick(cmd_basicOp_stopTransmission);
                                }
                            }
                        }
                    }
                }
            }


        }
        private void Runcase_2G_80211AX(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211AX");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211AX);

            /*loop all parameters and do TX*/
            string[] mcs_2G11AX = {
                DUT_GUI.GUI_Constants.TXT_MCS_BPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK34,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM12,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM23,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM56,
                DUT_GUI.GUI_Constants.TXT_MCS_256QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_256QAM56,
                DUT_GUI.GUI_Constants.TXT_MCS_1024QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_1024QAM56,
            };

            string[] bw_2G11AX = {
                DUT_GUI.GUI_Constants.TXT_BW_20MHz,
                DUT_GUI.GUI_Constants.TXT_BW_40MHz
            };
            string[] gi_2G11AX = {
                DUT_GUI.GUI_Constants.TXT_GI_08,
                DUT_GUI.GUI_Constants.TXT_GI_16,
                DUT_GUI.GUI_Constants.TXT_GI_32
            };
            string[] ss_2G11AX = {
                DUT_GUI.GUI_Constants.TXT_SPATIALSTREAM_1,
                DUT_GUI.GUI_Constants.TXT_SPATIALSTREAM_2
            };
            string[] ltf_2G11AX = {
                DUT_GUI.GUI_Constants.TXT_LTF_X1,
                DUT_GUI.GUI_Constants.TXT_LTF_X2,
                DUT_GUI.GUI_Constants.TXT_LTF_X4
            };
            for (int channel = DUT_GUI.GUI_Constants.DEFAULT_CHANNEL_START; channel <= DUT_GUI.GUI_Constants.MAX_CHANNEL_2400MHZ; channel++)
            {
                foreach (string strMcs in mcs_2G11AX)
                {
                    foreach (string strSpectrumBW in bw_2G11AX)
                    {
                        foreach (string strSignalBW in bw_2G11AX)
                        {
                            if ((strSpectrumBW == DUT_GUI.GUI_Constants.TXT_BW_20MHz) && (strSignalBW == DUT_GUI.GUI_Constants.TXT_BW_40MHz))
                            {
                                break;
                            }
                            foreach (string strGI in gi_2G11AX)
                            {
                                foreach (string strSS in ss_2G11AX)
                                {
                                    foreach (string strLTF in ltf_2G11AX)
                                    {

                                        Console.WriteLine("Set combox_basicOp_lowChan Value:" + channel);
                                        SetComboBoxItemData(combox_basicOp_lowChan, channel.ToString());


                                        Console.WriteLine("Set combox_basicOp_spectrumBW Value:" + strSpectrumBW);
                                        SetComboBoxItemData(combox_basicOp_spectrumBW, strSpectrumBW);


                                        Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setChannel");
                                        ButtonLeftClick(cmd_basicOp_setChannel);


                                        Console.WriteLine("Set combox_basicOp_signalBW Value:" + strSignalBW);
                                        SetComboBoxItemData(combox_basicOp_signalBW, strSignalBW);


                                        Console.WriteLine("Set combox_basicOp_MCS Value:" + strMcs);
                                        SetComboBoxItemData(combox_basicOp_MCS, strMcs);


                                        Console.WriteLine("Set combox_basicOp_spatialStream Value:" + strSS);
                                        SetComboBoxItemData(combox_basicOp_spatialStream, strSS);


                                        Console.WriteLine("Set combox_basicOp_GI Value:" + strGI);
                                        SetComboBoxItemData(combox_basicOp_GI, strGI);


                                        Console.WriteLine("Set combox_basicOp_ltf Value:" + strLTF);
                                        SetComboBoxItemData(combox_basicOp_ltf, strLTF);


                                        Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setRate");
                                        ButtonLeftClick(cmd_basicOp_setRate);

                                        Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setLoop");
                                        ButtonLeftClick(cmd_basicOp_setLoop);


                                        Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setPower");
                                        ButtonLeftClick(cmd_basicOp_setPower);


                                        Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setSpacing");
                                        ButtonLeftClick(cmd_basicOp_setSpacing);


                                        Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_startTransmitting -> 1s...");
                                        ButtonLeftClick(cmd_basicOp_startTransmitting);
                                        //Thread.Sleep(1000);


                                        Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_stopTransmission");
                                        ButtonLeftClick(cmd_basicOp_stopTransmission);
                                    }
                                }
                            }
                        }
                    }
                }
            }


        }
        private void Runcase_5G_80211A(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211A");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211A);

            /*loop all parameters and do TX*/
            string[] mcs_5G11A = {
                DUT_GUI.GUI_Constants.TXT_MCS_BPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK34,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM12,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM23,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM56,
            };


            for (int channel = DUT_GUI.GUI_Constants.DEFAULT_CHANNEL_5G_START; channel <= DUT_GUI.GUI_Constants.MAX_CHANNEL_5000MHZ; channel++)
            {
                foreach (string strMcs in mcs_5G11A)
                {

                    Console.WriteLine("Set combox_basicOp_lowChan Value:" + channel);
                    SetComboBoxItemData(combox_basicOp_lowChan, channel.ToString());


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setChannel");
                    ButtonLeftClick(cmd_basicOp_setChannel);


                    Console.WriteLine("Set combox_basicOp_MCS Value:" + strMcs);
                    SetComboBoxItemData(combox_basicOp_MCS, strMcs);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setRate");
                    ButtonLeftClick(cmd_basicOp_setRate);

                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setLoop");
                    ButtonLeftClick(cmd_basicOp_setLoop);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setPower");
                    ButtonLeftClick(cmd_basicOp_setPower);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setSpacing");
                    ButtonLeftClick(cmd_basicOp_setSpacing);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_startTransmitting -> 1s...");
                    ButtonLeftClick(cmd_basicOp_startTransmitting);
                    // Thread.Sleep(1000);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_stopTransmission");
                    ButtonLeftClick(cmd_basicOp_stopTransmission);

                }
            }


        }
        private void Runcase_5G_80211N(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211N_5GHZ");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211N_5GHZ);

            /*loop all parameters and do TX*/
            string[] mcs_5G11N = {
                DUT_GUI.GUI_Constants.TXT_MCS_BPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK34,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM12,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM23,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM56,
                DUT_GUI.GUI_Constants.TXT_MCS_256QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_256QAM56,
                DUT_GUI.GUI_Constants.TXT_MCS_1024QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_1024QAM56,
            };
            string[] bw_5G11N = {
                DUT_GUI.GUI_Constants.TXT_BW_20MHz,
                DUT_GUI.GUI_Constants.TXT_BW_40MHz
            };
            string[] gi_5G11N = {
                DUT_GUI.GUI_Constants.TXT_GI_08,
                DUT_GUI.GUI_Constants.TXT_GI_04
            };
            string[] ss_5G11N = {
                DUT_GUI.GUI_Constants.TXT_SPATIALSTREAM_1,
                DUT_GUI.GUI_Constants.TXT_SPATIALSTREAM_2
            };
            for (int channel = DUT_GUI.GUI_Constants.DEFAULT_CHANNEL_5G_START; channel <= DUT_GUI.GUI_Constants.MAX_CHANNEL_5000MHZ; channel++)
            {
                foreach (string strMcs in mcs_5G11N)
                {
                    foreach (string strSpectrumBW in bw_5G11N)
                    {
                        foreach (string strSignalBW in bw_5G11N)
                        {
                            if ((strSpectrumBW == DUT_GUI.GUI_Constants.TXT_BW_20MHz) && (strSignalBW == DUT_GUI.GUI_Constants.TXT_BW_40MHz))
                            {
                                break;
                            }
                            foreach (string strGI in gi_5G11N)
                            {
                                foreach (string strSS in ss_5G11N)
                                {

                                    Console.WriteLine("Set combox_basicOp_lowChan Value:" + channel);
                                    SetComboBoxItemData(combox_basicOp_lowChan, channel.ToString());


                                    Console.WriteLine("Set combox_basicOp_spectrumBW Value:" + strSpectrumBW);
                                    SetComboBoxItemData(combox_basicOp_spectrumBW, strSpectrumBW);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setChannel");
                                    ButtonLeftClick(cmd_basicOp_setChannel);


                                    Console.WriteLine("Set combox_basicOp_signalBW Value:" + strSignalBW);
                                    SetComboBoxItemData(combox_basicOp_signalBW, strSignalBW);


                                    Console.WriteLine("Set combox_basicOp_MCS Value:" + strMcs);
                                    SetComboBoxItemData(combox_basicOp_MCS, strMcs);


                                    Console.WriteLine("Set combox_basicOp_spatialStream Value:" + strSS);
                                    SetComboBoxItemData(combox_basicOp_spatialStream, strSS);


                                    Console.WriteLine("Set combox_basicOp_GI Value:" + strGI);
                                    SetComboBoxItemData(combox_basicOp_GI, strGI);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setRate");
                                    ButtonLeftClick(cmd_basicOp_setRate);

                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setLoop");
                                    ButtonLeftClick(cmd_basicOp_setLoop);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setPower");
                                    ButtonLeftClick(cmd_basicOp_setPower);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setSpacing");
                                    ButtonLeftClick(cmd_basicOp_setSpacing);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_startTransmitting -> 1s...");
                                    ButtonLeftClick(cmd_basicOp_startTransmitting);
                                    //Thread.Sleep(1000);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_stopTransmission");
                                    ButtonLeftClick(cmd_basicOp_stopTransmission);
                                }
                            }
                        }
                    }
                }
            }


        }
        private void Runcase_5G_80211AC(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211AC");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211AC);

            /*loop all parameters and do TX*/
            string[] mcs_5G11AC = {
                DUT_GUI.GUI_Constants.TXT_MCS_BPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK34,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM12,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM23,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM56,
                DUT_GUI.GUI_Constants.TXT_MCS_256QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_256QAM56,
                DUT_GUI.GUI_Constants.TXT_MCS_1024QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_1024QAM56,
            };
            string[] bw_5G11AC = {
                DUT_GUI.GUI_Constants.TXT_BW_20MHz,
                DUT_GUI.GUI_Constants.TXT_BW_40MHz,
                DUT_GUI.GUI_Constants.TXT_BW_80MHz,
                DUT_GUI.GUI_Constants.TXT_BW_160MHz
            };
            string[] gi_5G11AC = {
                DUT_GUI.GUI_Constants.TXT_GI_08,
                DUT_GUI.GUI_Constants.TXT_GI_04
            };
            string[] ss_5G11AC = {
                DUT_GUI.GUI_Constants.TXT_SPATIALSTREAM_1,
                DUT_GUI.GUI_Constants.TXT_SPATIALSTREAM_2
            };
            int loopIndexSpectrumBW = 0;
            int loopIndexSignalBW = 0;
            for (int channel = DUT_GUI.GUI_Constants.DEFAULT_CHANNEL_5G_START; channel <= DUT_GUI.GUI_Constants.MAX_CHANNEL_5000MHZ; channel++)
            {
                foreach (string strMcs in mcs_5G11AC)
                {
                    loopIndexSpectrumBW = 0;
                    loopIndexSignalBW = 0;
                    foreach (string strSpectrumBW in bw_5G11AC)
                    {
                        loopIndexSpectrumBW++;
                        loopIndexSignalBW = 0;
                        foreach (string strSignalBW in bw_5G11AC)
                        {
                            loopIndexSignalBW++;
                            if (loopIndexSignalBW > loopIndexSpectrumBW)
                            {
                                break;
                            }
                            foreach (string strGI in gi_5G11AC)
                            {
                                foreach (string strSS in ss_5G11AC)
                                {

                                    Console.WriteLine("Set combox_basicOp_lowChan Value:" + channel);
                                    SetComboBoxItemData(combox_basicOp_lowChan, channel.ToString());


                                    Console.WriteLine("Set combox_basicOp_spectrumBW Value:" + strSpectrumBW);
                                    SetComboBoxItemData(combox_basicOp_spectrumBW, strSpectrumBW);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setChannel");
                                    ButtonLeftClick(cmd_basicOp_setChannel);


                                    Console.WriteLine("Set combox_basicOp_signalBW Value:" + strSignalBW);
                                    SetComboBoxItemData(combox_basicOp_signalBW, strSignalBW);


                                    Console.WriteLine("Set combox_basicOp_MCS Value:" + strMcs);
                                    SetComboBoxItemData(combox_basicOp_MCS, strMcs);


                                    Console.WriteLine("Set combox_basicOp_spatialStream Value:" + strSS);
                                    SetComboBoxItemData(combox_basicOp_spatialStream, strSS);


                                    Console.WriteLine("Set combox_basicOp_GI Value:" + strGI);
                                    SetComboBoxItemData(combox_basicOp_GI, strGI);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setRate");
                                    ButtonLeftClick(cmd_basicOp_setRate);

                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setLoop");
                                    ButtonLeftClick(cmd_basicOp_setLoop);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setPower");
                                    ButtonLeftClick(cmd_basicOp_setPower);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setSpacing");
                                    ButtonLeftClick(cmd_basicOp_setSpacing);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_startTransmitting -> 1s...");
                                    ButtonLeftClick(cmd_basicOp_startTransmitting);
                                    //Thread.Sleep(1000);


                                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_stopTransmission");
                                    ButtonLeftClick(cmd_basicOp_stopTransmission);
                                }
                            }
                        }
                    }
                }
            }


        }
        private void Runcase_5G_80211AX(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211AX");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211AX);

            /*loop all parameters and do TX*/
            string[] mcs_5G11AX = {
                DUT_GUI.GUI_Constants.TXT_MCS_BPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK34,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM12,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM23,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM56,
                DUT_GUI.GUI_Constants.TXT_MCS_256QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_256QAM56,
                DUT_GUI.GUI_Constants.TXT_MCS_1024QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_1024QAM56,
            };
            string[] bw_5G11AX = {
                DUT_GUI.GUI_Constants.TXT_BW_20MHz,
                DUT_GUI.GUI_Constants.TXT_BW_40MHz,
                DUT_GUI.GUI_Constants.TXT_BW_80MHz,
                DUT_GUI.GUI_Constants.TXT_BW_160MHz
            };
            string[] gi_5G11AX = {
                DUT_GUI.GUI_Constants.TXT_GI_08,
                DUT_GUI.GUI_Constants.TXT_GI_16,
                DUT_GUI.GUI_Constants.TXT_GI_32
            };
            string[] ss_5G11AX = {
                DUT_GUI.GUI_Constants.TXT_SPATIALSTREAM_1,
                DUT_GUI.GUI_Constants.TXT_SPATIALSTREAM_2
            };
            string[] ltf_5G11AX = {
                DUT_GUI.GUI_Constants.TXT_LTF_X1,
                DUT_GUI.GUI_Constants.TXT_LTF_X2,
                DUT_GUI.GUI_Constants.TXT_LTF_X4
            };
            int loopIndexSpectrumBW = 0;
            int loopIndexSignalBW = 0;
            for (int channel = DUT_GUI.GUI_Constants.DEFAULT_CHANNEL_5G_START; channel <= DUT_GUI.GUI_Constants.MAX_CHANNEL_5000MHZ; channel++)
            {
                foreach (string strMcs in mcs_5G11AX)
                {
                    loopIndexSpectrumBW = 0;
                    loopIndexSignalBW = 0;
                    foreach (string strSpectrumBW in bw_5G11AX)
                    {
                        loopIndexSpectrumBW++;
                        loopIndexSignalBW = 0;
                        foreach (string strSignalBW in bw_5G11AX)
                        {
                            loopIndexSignalBW++;
                            if (loopIndexSignalBW > loopIndexSpectrumBW)
                            {
                                //Console.WriteLine("loopIndexSignalBW > loopIndexSpectrumBW ->" + loopIndexSignalBW +":"+ loopIndexSpectrumBW);
                                break;
                            }
                            foreach (string strGI in gi_5G11AX)
                            {
                                foreach (string strSS in ss_5G11AX)
                                {
                                    foreach (string strLTF in ltf_5G11AX)
                                    {

                                        Console.WriteLine("Set combox_basicOp_lowChan Value:" + channel);
                                        SetComboBoxItemData(combox_basicOp_lowChan, channel.ToString());


                                        Console.WriteLine("Set combox_basicOp_spectrumBW Value:" + strSpectrumBW);
                                        SetComboBoxItemData(combox_basicOp_spectrumBW, strSpectrumBW);


                                        Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setChannel");
                                        ButtonLeftClick(cmd_basicOp_setChannel);


                                        Console.WriteLine("Set combox_basicOp_signalBW Value:" + strSignalBW);
                                        SetComboBoxItemData(combox_basicOp_signalBW, strSignalBW);


                                        Console.WriteLine("Set combox_basicOp_MCS Value:" + strMcs);
                                        SetComboBoxItemData(combox_basicOp_MCS, strMcs);


                                        Console.WriteLine("Set combox_basicOp_spatialStream Value:" + strSS);
                                        SetComboBoxItemData(combox_basicOp_spatialStream, strSS);


                                        Console.WriteLine("Set combox_basicOp_ltf Value:" + strLTF);
                                        SetComboBoxItemData(combox_basicOp_ltf, strLTF);


                                        Console.WriteLine("Set combox_basicOp_GI Value:" + strGI);
                                        SetComboBoxItemData(combox_basicOp_GI, strGI);



                                        Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setRate");
                                        ButtonLeftClick(cmd_basicOp_setRate);

                                        Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setLoop");
                                        ButtonLeftClick(cmd_basicOp_setLoop);


                                        Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setPower");
                                        ButtonLeftClick(cmd_basicOp_setPower);


                                        Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setSpacing");
                                        ButtonLeftClick(cmd_basicOp_setSpacing);


                                        Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_startTransmitting -> 1s...");
                                        ButtonLeftClick(cmd_basicOp_startTransmitting);
                                        //Thread.Sleep(1000);


                                        Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_stopTransmission");
                                        ButtonLeftClick(cmd_basicOp_stopTransmission);

                                    }
                                }
                            }
                        }
                    }
                }
            }


        }
        private void Runcase_6G_80211AX_Once(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211AX");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211AX);

            //Set_TX_Once(rootElement);
            string[] mcs_6G11AX = {
                DUT_GUI.GUI_Constants.TXT_MCS_BPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK34,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM12,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM23,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM56,
                DUT_GUI.GUI_Constants.TXT_MCS_256QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_256QAM56,
                DUT_GUI.GUI_Constants.TXT_MCS_1024QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_1024QAM56,
            };
            string[] bw_6G11AX = {
                DUT_GUI.GUI_Constants.TXT_BW_20MHz,
                DUT_GUI.GUI_Constants.TXT_BW_40MHz,
                DUT_GUI.GUI_Constants.TXT_BW_80MHz,
                DUT_GUI.GUI_Constants.TXT_BW_160MHz
            };
            string[] gi_6G11AX = {
                DUT_GUI.GUI_Constants.TXT_GI_08,
                DUT_GUI.GUI_Constants.TXT_GI_16,
                DUT_GUI.GUI_Constants.TXT_GI_32
            };
            string[] ss_6G11AX = {
                DUT_GUI.GUI_Constants.TXT_SPATIALSTREAM_1,
                DUT_GUI.GUI_Constants.TXT_SPATIALSTREAM_2
            };
            string[] ltf_6G11AX = {
                DUT_GUI.GUI_Constants.TXT_LTF_X1,
                DUT_GUI.GUI_Constants.TXT_LTF_X2,
                DUT_GUI.GUI_Constants.TXT_LTF_X4
            };

            var legal6GBW20Channel = new List<int> { 1, 5, 9, 13, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53, 57, 61, 65, 69, 73, 77, 81, 85, 89, 93, 97, 101, 105, 109, 113, 117, 121, 125, 129, 133, 137, 141, 145, 149, 153, 157, 161, 165, 169, 173, 177, 181, 185, 189, 193, 197, 201, 205, 209, 213, 217, 221, 225, 229, 233 };
            var legal6GBW40Channel = new List<int> { 3, 11, 19, 27, 35, 43, 51, 59, 67, 75, 83, 91, 99, 107, 115, 123, 131, 139, 147, 155, 163, 171, 179, 187, 195, 203, 211, 219, 227 };
            var legal6GBW80Channel = new List<int> { 7, 23, 39, 55, 71, 87, 103, 119, 135, 151, 167, 183, 199, 215 };
            var legal6GBW160Channel = new List<int> { 15, 47, 79, 111, 143, 175, 207 };
            //var valid6GBW160;
            int loopIndexSpectrumBW = 0;
            int loopIndexSignalBW = 0;
            for (int channel = DUT_GUI.GUI_Constants.DEFAULT_CHANNEL_6G_START; channel <= DUT_GUI.GUI_Constants.MAX_CHANNEL_6000MHZ; channel++)
            {
                foreach (string strMcs in mcs_6G11AX)
                {
                    loopIndexSpectrumBW = 0;
                    loopIndexSignalBW = 0;
                    foreach (string strSpectrumBW in bw_6G11AX)
                    {
                        if (strSpectrumBW == DUT_GUI.GUI_Constants.TXT_BW_20MHz)// spectrum BW 20
                        {
                            var valid6GBW20 = legal6GBW20Channel.Contains(channel);
                            if (!valid6GBW20)
                            {
                                continue;
                            }
                        }
                        if (strSpectrumBW == DUT_GUI.GUI_Constants.TXT_BW_40MHz)// spectrum BW 40
                        {
                            var valid6GBW40 = legal6GBW40Channel.Contains(channel);
                            if (!valid6GBW40)
                            {
                                continue;
                            }
                        }
                        if (strSpectrumBW == DUT_GUI.GUI_Constants.TXT_BW_80MHz)// spectrum BW 80
                        {
                            var valid6GBW80 = legal6GBW80Channel.Contains(channel);
                            if (!valid6GBW80)
                            {
                                continue;
                            }
                        }
                        if (strSpectrumBW == DUT_GUI.GUI_Constants.TXT_BW_160MHz)// spectrum BW 160
                        {
                            var valid6GBW160 = legal6GBW160Channel.Contains(channel);
                            if (!valid6GBW160)
                            {
                                continue;
                            }
                        }
                        loopIndexSpectrumBW++;
                        loopIndexSignalBW = 0;
                        foreach (string strSignalBW in bw_6G11AX)
                        {
                            loopIndexSignalBW++;
                            if (loopIndexSignalBW > loopIndexSpectrumBW)
                            {
                                //Console.WriteLine("loopIndexSignalBW > loopIndexSpectrumBW ->" + loopIndexSignalBW +":"+ loopIndexSpectrumBW);
                                //break;
                                continue;
                            }
                            //foreach (string strGI in gi_6G11AX)
                            // {
                            //  foreach (string strSS in ss_6G11AX)
                            //  {
                            //     foreach (string strLTF in ltf_6G11AX)
                            //    {

                            Console.WriteLine("Set combox_basicOp_lowChan Value:" + channel);
                            SetComboBoxItemData(combox_basicOp_lowChan, channel.ToString());


                            Console.WriteLine("Set combox_basicOp_spectrumBW Value:" + strSpectrumBW);
                            SetComboBoxItemData(combox_basicOp_spectrumBW, strSpectrumBW);


                            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setChannel");
                            ButtonLeftClick(cmd_basicOp_setChannel);


                            Console.WriteLine("Set combox_basicOp_signalBW Value:" + strSignalBW);
                            SetComboBoxItemData(combox_basicOp_signalBW, strSignalBW);


                            Console.WriteLine("Set combox_basicOp_MCS Value:" + strMcs);
                            SetComboBoxItemData(combox_basicOp_MCS, strMcs);


                            //Console.WriteLine("Set combox_basicOp_spatialStream Value:" + strSS);
                            //SetComboBoxItemData(combox_basicOp_spatialStream, strSS);


                            // Console.WriteLine("Set combox_basicOp_ltf Value:" + strLTF);
                            // SetComboBoxItemData(combox_basicOp_ltf, strLTF);


                            // Console.WriteLine("Set combox_basicOp_GI Value:" + strGI);
                            //SetComboBoxItemData(combox_basicOp_GI, strGI);



                            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setRate");
                            ButtonLeftClick(cmd_basicOp_setRate);

                            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setLoop");
                            ButtonLeftClick(cmd_basicOp_setLoop);


                            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setPower");
                            ButtonLeftClick(cmd_basicOp_setPower);


                            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setSpacing");
                            ButtonLeftClick(cmd_basicOp_setSpacing);


                            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_startTransmitting -> 1s...");
                            ButtonLeftClick(cmd_basicOp_startTransmitting);
                            //Thread.Sleep(1000);


                            Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_stopTransmission");
                            ButtonLeftClick(cmd_basicOp_stopTransmission);

                        }
                    }
                }
            }
            //    }
        }
        //  }
        // }
        private void Runcase_6G_80211AX(AutomationElement rootElement)
        {
            /*Set phytype*/

            Console.WriteLine("Set combox_basicOp_phyType Value TXT_PHY_80211AX");
            SetSelectedComboBoxItem(combox_basicOp_phyType, DUT_GUI.GUI_Constants.TXT_PHY_80211AX);

            /*loop all parameters and do TX*/
            string[] mcs_6G11AX = {
                DUT_GUI.GUI_Constants.TXT_MCS_BPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK12,
                DUT_GUI.GUI_Constants.TXT_MCS_QPSK34,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM12,
                DUT_GUI.GUI_Constants.TXT_MCS_16QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM23,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_64QAM56,
                DUT_GUI.GUI_Constants.TXT_MCS_256QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_256QAM56,
                DUT_GUI.GUI_Constants.TXT_MCS_1024QAM34,
                DUT_GUI.GUI_Constants.TXT_MCS_1024QAM56,
            };
            string[] bw_6G11AX = {
                DUT_GUI.GUI_Constants.TXT_BW_20MHz,
                DUT_GUI.GUI_Constants.TXT_BW_40MHz,
                DUT_GUI.GUI_Constants.TXT_BW_80MHz,
                DUT_GUI.GUI_Constants.TXT_BW_160MHz
            };
            string[] gi_6G11AX = {
                DUT_GUI.GUI_Constants.TXT_GI_08,
                DUT_GUI.GUI_Constants.TXT_GI_16,
                DUT_GUI.GUI_Constants.TXT_GI_32
            };
            string[] ss_6G11AX = {
                DUT_GUI.GUI_Constants.TXT_SPATIALSTREAM_1,
                DUT_GUI.GUI_Constants.TXT_SPATIALSTREAM_2
            };
            string[] ltf_6G11AX = {
                DUT_GUI.GUI_Constants.TXT_LTF_X1,
                DUT_GUI.GUI_Constants.TXT_LTF_X2,
                DUT_GUI.GUI_Constants.TXT_LTF_X4
            };

            var legal6GBW20Channel = new List<int> { 1, 5, 9, 13, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53, 57, 61, 65, 69, 73, 77, 81, 85, 89, 93, 97, 101, 105, 109, 113, 117, 121, 125, 129, 133, 137, 141, 145, 149, 153, 157, 161, 165, 169, 173, 177, 181, 185, 189, 193, 197, 201, 205, 209, 213, 217, 221, 225, 229, 233 };
            var legal6GBW40Channel = new List<int> { 3, 11, 19, 27, 35, 43, 51, 59, 67, 75, 83, 91, 99, 107, 115, 123, 131, 139, 147, 155, 163, 171, 179, 187, 195, 203, 211, 219, 227 };
            var legal6GBW80Channel = new List<int> { 7, 23, 39, 55, 71, 87, 103, 119, 135, 151, 167, 183, 199, 215 };
            var legal6GBW160Channel = new List<int> { 15, 47, 79, 111, 143, 175, 207 };
            //var valid6GBW160;
            int loopIndexSpectrumBW = 0;
            //for (int channel = DUT_GUI.GUI_Constants.DEFAULT_CHANNEL_6G_START; channel <= DUT_GUI.GUI_Constants.MAX_CHANNEL_6000MHZ; channel++)
            foreach (string strSpectrumBW in bw_6G11AX)
            {
                // Console.WriteLine("testing Channel " + channel);
                //foreach (string strMcs in mcs_6G11AX)
                //  {
                loopIndexSpectrumBW = 0;
                for (int channel = DUT_GUI.GUI_Constants.DEFAULT_CHANNEL_6G_START; channel <= DUT_GUI.GUI_Constants.MAX_CHANNEL_6000MHZ; channel++)
                {
                    // foreach (string strSpectrumBW in bw_6G11AX)
                    Console.WriteLine("testing Channel " + channel);

                    if (strSpectrumBW == DUT_GUI.GUI_Constants.TXT_BW_20MHz)// spectrum BW 20
                    {
                        var valid6GBW20 = legal6GBW20Channel.Contains(channel);
                        if (!valid6GBW20)
                        {
                            continue;
                        }
                    }
                    if (strSpectrumBW == DUT_GUI.GUI_Constants.TXT_BW_40MHz)// spectrum BW 40
                    {
                        var valid6GBW40 = legal6GBW40Channel.Contains(channel);
                        if (!valid6GBW40)
                        {
                            continue;
                        }
                    }
                    if (strSpectrumBW == DUT_GUI.GUI_Constants.TXT_BW_80MHz)// spectrum BW 80
                    {
                        var valid6GBW80 = legal6GBW80Channel.Contains(channel);
                        if (!valid6GBW80)
                        {
                            continue;
                        }
                    }
                    if (strSpectrumBW == DUT_GUI.GUI_Constants.TXT_BW_160MHz)// spectrum BW 160
                    {
                        var valid6GBW160 = legal6GBW160Channel.Contains(channel);
                        if (!valid6GBW160)
                        {
                            continue;
                        }
                    }
                    loopIndexSpectrumBW++;
                    // foreach (string strSignalBW in bw_6G11AX)
                    //   {
                    ////    loopIndexSignalBW++;
                    //    if (loopIndexSignalBW > loopIndexSpectrumBW)
                    //   {
                    //Console.WriteLine("loopIndexSignalBW > loopIndexSpectrumBW ->" + loopIndexSignalBW +":"+ loopIndexSpectrumBW);
                    //       break;
                    //   }
                    //foreach (string strGI in gi_6G11AX)
                    //{
                    //    foreach (string strSS in ss_6G11AX)
                    //    {
                    //        foreach (string strLTF in ltf_6G11AX)
                    //        {

                    Console.WriteLine("Set combox_basicOp_lowChan Value:" + channel);
                    SetComboBoxItemData(combox_basicOp_lowChan, channel.ToString());


                    Console.WriteLine("Set combox_basicOp_spectrumBW Value:" + strSpectrumBW);
                    SetComboBoxItemData(combox_basicOp_spectrumBW, strSpectrumBW);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setChannel");
                    ButtonLeftClick(cmd_basicOp_setChannel);


                    //  Console.WriteLine("Set combox_basicOp_signalBW Value:" + strSignalBW);
                    //   SetComboBoxItemData(combox_basicOp_signalBW, strSignalBW);


                    // Console.WriteLine("Set combox_basicOp_MCS Value:" + strMcs);
                    // SetComboBoxItemData(combox_basicOp_MCS, strMcs);


                    //Console.WriteLine("Set combox_basicOp_spatialStream Value:" + strSS);
                    //SetComboBoxItemData(combox_basicOp_spatialStream, strSS);


                    //Console.WriteLine("Set combox_basicOp_ltf Value:" + strLTF);
                    //SetComboBoxItemData(combox_basicOp_ltf, strLTF);


                    //Console.WriteLine("Set combox_basicOp_GI Value:" + strGI);
                    //SetComboBoxItemData(combox_basicOp_GI, strGI);



                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setRate");
                    ButtonLeftClick(cmd_basicOp_setRate);

                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setLoop");
                    ButtonLeftClick(cmd_basicOp_setLoop);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setPower");
                    ButtonLeftClick(cmd_basicOp_setPower);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_setSpacing");
                    ButtonLeftClick(cmd_basicOp_setSpacing);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_startTransmitting -> 1s...");
                    ButtonLeftClick(cmd_basicOp_startTransmitting);
                    //Thread.Sleep(1000);


                    Console.WriteLine("Push Mouse Left Click-> cmd_basicOp_stopTransmission");
                    ButtonLeftClick(cmd_basicOp_stopTransmission);

                }
            }
        }
    }
    //    }
    //  }
    //  }


    //  }

    //}
    public class Log
    {
        private StreamWriter output;
        string consoleFileName = Program.dutAutomationLogFile;

        public Log()
        {
            output = new StreamWriter(File.OpenWrite(consoleFileName));
        }
        public void Write(char c)
        {
            lock (output)
            {
                output.Write(c);
            }
        }
        public void Write(string c)
        {
            lock (output)
            {
                output.Write(c);
            }
        }
        public void Close()
        {
            output.Flush();
            output.Close();
        }
    }
    public class DUTConsoleOutput : TextWriter
    {
        private TextWriter standard;
        private Log log;

        public DUTConsoleOutput(TextWriter standard, Log log)
        {
            this.standard = standard;
            this.log = log;
        }

        public override void WriteLine(string value)
        {

            standard.WriteLine(value);
            log.Write(value + "\r\n");
        }

        public override Encoding Encoding
        {
            get { return Encoding.Default; }
        }
        protected override void Dispose(bool disposing)
        {
            standard.Dispose();
        }
        public string GetTimeStamp()
        {
            return DateTime.Now.ToLocalTime().ToString();
        }
    }
    public class DUTConsoleInput : TextReader
    {
        private TextReader standard;

        public DUTConsoleInput(TextReader standard)
        {

            this.standard = standard;
        }

        public override int Peek()
        {
            return standard.Peek();
        }
        public override int Read()
        {
            int result = standard.Read();

            return result;
        }
        protected override void Dispose(bool disposing)
        {
            standard.Dispose();
        }
    }
}
