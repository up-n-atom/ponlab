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

#define DUT_USE_ANT_MASK
#define USE_RESTART_IN_INIT_2
#define DUT_USE_DIALOG_FOR_CONNECT
//#define DUT_SET_W500_ID
//#define DUT_USE_3_ANT_WAV500
//#define DUT_S2D_UNITEST
#define DUT_USE_TPC
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Globalization;
using Microsoft.Win32;
using System.Threading;
using System.Diagnostics;
using dut_cli;
using static DUT_GUI.GUI_BoxItems;
using static DUT_GUI.GUI_Constants;
using System.Collections;
using System.Linq;

namespace DUT_GUI
{
    public partial class DUT_GUI : Form
    {
        private readonly Button[] m_txAntButtons;
        private readonly Button[] m_rxAntButtons;

        static string LogLevelToString(LogLevel level)
        {
            switch (level)
            {
                case LogLevel.LOG_LEVEL_NONE:
                    return "NONE";
                case LogLevel.LOG_LEVEL_ERROR:
                    return "ERROR";
                case LogLevel.LOG_LEVEL_WARNING:
                    return "WARNING";
                case LogLevel.LOG_LEVEL_INFO:
                    return "INFO";
                case LogLevel.LOG_LEVEL_DEBUG:
                    return "DEBUG";
                case LogLevel.LOG_LEVEL_TRACE:
                    return "TRACE";
                default:
                    return "";
            }
        }

        static Color LogLevelToColor(LogLevel level)
        {
            switch (level)
            {
                case LogLevel.LOG_LEVEL_NONE:
                    return Color.Black;
                case LogLevel.LOG_LEVEL_ERROR:
                    return Color.Red;
                case LogLevel.LOG_LEVEL_WARNING:
                    return Color.Orange;
                case LogLevel.LOG_LEVEL_INFO:
                    return Color.Green;
                case LogLevel.LOG_LEVEL_DEBUG:
                    return Color.Blue;
                case LogLevel.LOG_LEVEL_TRACE:
                    return Color.Black;
                default:
                    return Color.Black;
            }
        }

        static void WriteLogMessageToConsole(LogLevel level, string message)
        {
            ConsoleOutput.ForegroundColor = LogLevelToColor(level);
            Console.WriteLine("[" + LogLevelToString(level) + "] " + message);
        }

        public static int UnsafeParseString(string str)
        {
            if (String.IsNullOrEmpty(str)) return 0;
            if (str.Contains("0x") || str.Contains("h"))
            {
                str = str.Replace("0x", "");
                str = str.Replace("h", "");
                return int.Parse(str, NumberStyles.AllowHexSpecifier);
            }
            else return int.Parse(str);

        }

        public static int ParseString(string str)
        {
            if (String.IsNullOrEmpty(str)) return 0;
            try
            {
                return UnsafeParseString(str);
            }
            catch (Exception) { return 0; }
        }

        private enum TpcCisStructType
        {
            TPC_CIS_FREQ_XY_CAL = 0,
            TPC_CIS_FREQ_1S2D_NO_CALIB = 1,
            TPC_CIS_FREQ_2S2D_NO_CALIB = 2,
            TPC_CIS_FREQ_3S2D_NO_CALIB = 3,
            TPC_CIS_FREQ_1S2D = 4,
            TPC_CIS_FREQ_2S2D = 5,
            TPC_CIS_FREQ_3S2D = 6,
        }

        readonly DUT_t DUT;

        readonly LogFile logFile = new LogFile();
        readonly ConsoleOutput dutConsoleWriter;
        readonly ConsoleInput dutConsoleReader;
        readonly LogDelegate logDelegate;
        readonly DelegatedLogger logger;
        Rectangle initialRectDutGUI;
        const string DUTkey = @"Software\MaxLinear\DUT";

        bool toShowExceptionMsg = true;
        int prevChannel = -1;
        int prevPrimaryChannelIndex = -1;
        ushort prevCenterFreqMHz = MIN_FREQUENCY;
        bool isSpacelessTransmission = false;
        bool isCwTransmission = false;
        bool isDbgConsoleLaunched = false;

        public bool ToShowExceptionMsg { get { return toShowExceptionMsg; } }

        static public void WriteToConsole(string str, Color color)
        {
            ConsoleOutput.ForegroundColor = color;
            Console.WriteLine("[DUTGUI]" + str);
        }

        static public void WriteLine(string str)
        {
            ConsoleOutput.ForegroundColor = Color.Black;
            Console.WriteLine("[DUTGUI]" + str);
        }

        public DUT_GUI()
        {
            try
            {
                logDelegate = new LogDelegate(WriteLogMessageToConsole);
                logger = new DelegatedLogger(logDelegate);

                DUT = new DUT_t(logger);
                InitializeComponent();

                m_txAntButtons = new Button[]
                {
                    cmd_basicOp_txAnt0,
                    cmd_basicOp_txAnt1,
                    cmd_basicOp_txAnt2,
                    cmd_basicOp_txAnt3
                };
                m_rxAntButtons = new Button[]
                {
                    cmd_basicOp_rxAnt0,
                    cmd_basicOp_rxAnt1,
                    cmd_basicOp_rxAnt2,
                    cmd_basicOp_rxAnt3,
                    cmd_basicOp_rxAnt4
                };

                combox_basicOp_primaryChannelIndex.SelectedIndex = 0;

                GUI_fillComboBox(combox_basicOp_band);
                GUI_fillComboBox(comboBox_basicOp_memoryType);
                GUI_fillComboBox(combox_basicOp_phyType);
                GUI_fillComboBox(combox_DebugConsole);
                GUI_fillComboBox(combox_nvMemCtrl_printLevel);
                GUI_fillComboBox(combox_MoreComp);

                dutConsoleWriter = new ConsoleOutput(Console.Out, logFile, dllConsole);
                dutConsoleReader = new ConsoleInput(Console.In);
                Console.SetOut(dutConsoleWriter);
                Console.SetIn(dutConsoleReader);

                // By default, log to textbox and log errors only
                combox_DebugConsole.SelectedIndex = GUI_GetValueAsIndex(combox_DebugConsole, (int)OutputDestination.OUT_TEXTBOX);
                combox_nvMemCtrl_printLevel.SelectedIndex = GUI_GetValueAsIndex(combox_nvMemCtrl_printLevel, (int)LogLevel.LOG_LEVEL_ERROR);

                this.Left = 0;
                this.Top = 0;

                Rectangle screenRectangle = RectangleToScreen(ClientRectangle);
                int titleHeight = screenRectangle.Top - this.Top;
                this.Height = titleHeight + dllConsole.Location.Y + dllConsole.Height + 2 * dllConsole.Margin.Vertical;

                initialRectDutGUI = this.RestoreBounds;
            }
            catch (Exception ex)
            {
                MessageDialog.ShowError(ex.Message);
                toShowExceptionMsg = false;
                Close();
            }
        }

        public void Init(bool startingUp = false)
        {
            try
            {
                if (startingUp)
                {
                    txtBox_nvMemCtrl_dutVersion.Text = DUT_t.Version;
                    this.Text = "DUT GUI - Version " + DUT_t.Version + " (" + DUT_t.BuildTag + ")";

                    if ((!RestoreIpAddress()) || (!RestoreIpSpecificData()))
                    {
                        LoadDefaultData();
                    }

                    // Filling combox_nvMemCtrl_country with countries defined in DUT dll                                
                    ulong countryCount = Country.getCountryCount();

                    // Country codes combo-box
                    for (ulong i = 0; i < countryCount; ++i)
                    {
                        CountryInfo countryInfo = Country.getCountryByIndex(i);
                        combox_nvMemCtrl_country.Items.Add(countryInfo);
                    }

                    foreach (ListViewItem item in inputsList.Items)
                    {
                        item.SubItems.Add(new ListViewItem.ListViewSubItem());
                    }

                    foreach (ListViewItem item in RxEvmList.Items)
                    {
                        item.SubItems.Add(new ListViewItem.ListViewSubItem());
                    }

                    WriteLine("Welcome to DUT (Device Under Test) GUI");
                }

                DUT.HardwareTypeInit();

                if (DUT.IsInitialized)
                {
                    CardIdUpdate();
                    ProductionInfoUpdate();

                    if (DUT.HardwareType < HardwareType.HARDWARE_TYPE_GEN6)
                    {
                        txtBox_basicOp_CalMode.Hide();
                        groupBox_nvMem_prodInfo.Hide();
                    }

                    updateAnts();
                }

                GUI_setColor_cmd(cmd_basicOp_setPower, Color.Red);

                enableDisableCW();

                check_basicOp_txLongData_CheckedChanged(null, null);
                check_basicOp_numSymbols_CheckedChanged(null, null);

                if (DUT.IsInitialized)
                {
                    tabControl.Invalidate();
                }
            }
            catch (Exception ex)
            {
                MessageDialog.ShowError(ex.Message);
                toShowExceptionMsg = false;
                Close();
            }
        }

        static private void SetAntButtonColor(Button button, bool isOn)
        {
            button.BackColor = isOn ? Color.LightGreen : Color.CornflowerBlue;
        }

        private void TXAntClick(Button antBtn, int antIndex)
        {
            if ((antIndex >= 0) && (antIndex < DUT.TXants.Length))
            {
                bool wasON = DUT.TXants[antIndex];
                using (new WaitCursorBlock(this))
                {
                    if (SetTXant(antBtn, antIndex, !wasON))
                    {
                        if (!DUT.IsTransmitting)
                        {
                            UpdateGUI_ForTXStop();
                        }
                    }
                }
            }
        }

        private void RXAntClick(Button antBtn, int antIndex)
        {
            if ((antIndex >= 0) && (antIndex < DUT.RXants.Length))
            {
                bool wasON = DUT.RXants[antIndex];
                using (new WaitCursorBlock(this))
                {
                    SetRXant(antBtn, antIndex, !wasON);
                }
            }
        }

        private bool SetTXant(Button antBtn, int antIndex, bool isON)
        {
            if (DUT.SetTXant(antIndex, isON))
            {
                SetAntButtonColor(antBtn, isON);
                return true;
            }

            return false;
        }

        private bool SetRXant(Button antBtn, int antIndex, bool isON)
        {
            if (DUT.SetRXant(antIndex, isON))
            {
                SetAntButtonColor(antBtn, isON);
                return true;
            }

            return false;
        }

        private void TXant0_Click(object sender, EventArgs e)
        {
            TXAntClick(cmd_basicOp_txAnt0, 0);
        }

        private void TXant1_Click(object sender, EventArgs e)
        {
            TXAntClick(cmd_basicOp_txAnt1, 1);
        }

        private void TXant2_Click(object sender, EventArgs e)
        {
            TXAntClick(cmd_basicOp_txAnt2, 2);
        }

        private void TXant3_Click(object sender, EventArgs e)
        {
            TXAntClick(cmd_basicOp_txAnt3, 3);
        }

        private void RXant0_Click(object sender, EventArgs e)
        {
            RXAntClick(cmd_basicOp_rxAnt0, 0);
        }

        private void RXant1_Click(object sender, EventArgs e)
        {
            RXAntClick(cmd_basicOp_rxAnt1, 1);
        }

        private void RXant2_Click(object sender, EventArgs e)
        {
            RXAntClick(cmd_basicOp_rxAnt2, 2);
        }

        private void RXant3_Click(object sender, EventArgs e)
        {
            RXAntClick(cmd_basicOp_rxAnt3, 3);
        }

        private void RXant4_Click(object sender, EventArgs e)
        {
            RXAntClick(cmd_basicOp_rxAnt4, 4);
        }

        static private int ParseControl(Control ctrl)
        {
            return ParseString(ctrl.Text);
        }

        private void enableDisableCW()
        {
            bool toEnable = (GUI_GetSelectedPhyMode() != PhyMode.PHY_MODE_B); // not in 11b
            cmd_basicOp_transmitCW.Enabled = cmd_basicOp_stopCW.Enabled = toEnable;
            updateStopTXbutton();
        }

        private PhyMode GUI_GetSelectedPhyMode()
        {
            return (PhyMode)GUI_GetSelectedValue(combox_basicOp_phyType);
        }

        private RegulationType GUI_GetSelectedRegulationType()
        {
            return (RegulationType)GUI_GetSelectedValue(combox_basicOp_regulationType);
        }

        private Band GUI_GetSelectedBand()
        {
            return (Band)GUI_GetSelectedValue(combox_basicOp_band);
        }

        private Bandwidth GUI_GetSelectedSignalBandwidth()
        {
            return (Bandwidth)GUI_GetSelectedValue(combox_basicOp_signalBW);
        }

        private Bandwidth GUI_GetSelectedSpectrumBandwidth()
        {
            return (Bandwidth)GUI_GetSelectedValue(combox_basicOp_spectrumBW);
        }

        private CodingType GUI_GetSelectedCodingType()
        {
            CodingType codingType;
            if (radio_basicOp_codingAuto.Checked)
            {
                codingType = CodingType.CODING_TYPE_AUTO;
            }
            else if (radio_basicOp_codingLDPC.Checked)
            {
                codingType = CodingType.CODING_TYPE_LDPC;
            }
            else
            {
                codingType = CodingType.CODING_TYPE_BCC;
            }

            return codingType;
        }

        private void setChannel_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                PhyMode phyMode = GUI_GetSelectedPhyMode();
                Bandwidth _spectrumBW = GUI_GetSelectedSpectrumBandwidth();
                byte lowestChannel = (byte)ParseControl(combox_basicOp_lowChan); //Lowest channel
                byte primaryChannelIndex = (byte)ParseControl(combox_basicOp_primaryChannelIndex);
                RegulationType regulationType = GUI_GetSelectedRegulationType();

                if (DUT.SetChannel(phyMode, _spectrumBW, lowestChannel, primaryChannelIndex, regulationType))
                {
                    cmd_basicOp_setChannel.ForeColor = Color.Black;
                    cmd_basicOp_setSpacing.ForeColor = Color.Red;
                }
            }
        }

        private bool isGen5Hardware()
        {
            return DUT.HardwareType == HardwareType.HARDWARE_TYPE_GEN5;
        }

        private bool isGen2Hardware()
        {
            return DUT.HardwareType == HardwareType.HARDWARE_TYPE_GEN2_PCI;
        }

        private void updateControlsState()
        {
            if (isGen2Hardware())
            {
                cmd_basicOp_txAnt1.Enabled = false;
                inputsList.Items[(int)InputsListItems.PA_TX1].BackColor = Color.LightGray;
            }
        }

        /// <summary>
        /// transmitCheckConditions - used by tx and txSpaceless. spacless len = 68 & repeate =1, so no len check and no repeate check
        /// </summary>
        private bool transmitCheckConditions()
        {
            //must be after set channel.and repeats !0. 
            if (DUT.GetActiveTxAntennaMask().Value == 0)
            {
                MessageDialog.ShowWarning("transmit_Click: can't transmit when all antennas are off");
                return false;
            }
            if (isSpacelessTransmission)
            {
                MessageDialog.ShowWarning("transmit_Click: can't transmit when spaceless tx on");
                return false;
            }
            if (isCwTransmission)
            {
                MessageDialog.ShowWarning("transmit_Click: can't transmit when CW tx on");
                return false;
            }
            return true;
        }

        private void transmit_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                if (!transmitCheckConditions())
                {
                    return;
                }
                Bandwidth _signalBW = GUI_GetSelectedSignalBandwidth();
                bool _isDataLong = check_basicOp_txLongData.Checked;
                bool _beamforming = check_basicOp_txBeamforming.Checked;
                uint _length = (uint)ParseControl(txtBox_basicOp_packetLen);
                ushort _repetitions = (ushort)ParseControl(txtBox_basicOp_repetitions);
                CodingType _codingType = GUI_GetSelectedCodingType();
                PhyMode _phyType = GUI_GetSelectedPhyMode();

                if (0 == _length || 0 == _repetitions)
                {
                    MessageDialog.ShowWarning("Length and Repetitions must be decimal or hexadecimal and greater than 0");
                    return;
                }
                if (_isDataLong)
                {
                    Mcs _MCS = GetMcs();
                    byte _SS = (byte)ParseControl(combox_basicOp_spatialStream);
                    Gi _GI = (Gi)GUI_GetSelectedValue(combox_basicOp_GI);

                    if (DUT.GetRate(_phyType, _signalBW, _MCS, _SS, _GI, out float _Rate))
                    {
                        float _DurationTimePacket = (_length * 8) / _Rate;
                        WriteToConsole("Calculated duration time = " + _DurationTimePacket.ToString("n2") + " uSec", Color.Blue);
                    }
                }

                if (DUT.StartTxPackets(_repetitions, _length, _isDataLong, _beamforming, _codingType))
                {
                    updateStopTXbutton();
                    updateStartTxButton();

                    GUI_setColor_cmd(cmd_basicOp_startTransmitting, Color.Black);
                    GUI_setColor_cmd(cmd_basicOp_stopTransmission, Color.Red);
                    GUI_setColor_cmd(cmd_basicOp_startSpaceless, Color.Black);
                    groupBox_basicOp_spaceless.Enabled = false;
                    cmd_basicOp_startSpaceless.Enabled = false;
                    cmd_basicOp_stopSpaceless.Enabled = false;
                    cmd_basicOp_transmitCW.Enabled = false;
                    cmd_basicOp_stopCW.Enabled = false;
                    cmd_enableAll(false); //merav
                }
            }
        }
        private void txtBoxRepetitions_keypress(object sender, KeyPressEventArgs e)
        {
            if ((e.KeyChar == (char)8)
                || (Char.IsLetterOrDigit(e.KeyChar)))
            {
                e.Handled = false;
            }
            else
            {
                e.Handled = true;
                WriteToConsole("Input value limited to letter and digit", Color.Red);
            }

        }
        private void txtBoxPacketLen_keypress(object sender, KeyPressEventArgs e)
        {
            if ((e.KeyChar == (char)8)
                || (Char.IsLetterOrDigit(e.KeyChar)))
            {
                e.Handled = false;
            }
            else
            {
                e.Handled = true;
                WriteToConsole("Input value limited to letter and digit", Color.Red);
            }

        }

        private void repetitions_TextChanged(object sender, EventArgs e)
        {
            int _repetitions = ParseControl(txtBox_basicOp_repetitions);
            this.txtBox_basicOp_repetitions.TextChanged -= new System.EventHandler(this.repetitions_TextChanged);
            if (_repetitions < 0 || _repetitions > 0xffff)
            {
                txtBox_basicOp_repetitions.Text = "0xffff";
                WriteToConsole("Repetitions limit to (0 ~ 0xffff), 0xffff means endless", Color.Red);
            }


            this.txtBox_basicOp_repetitions.TextChanged += new System.EventHandler(this.repetitions_TextChanged);
        }

        private void signalBW_SelectedIndexChanged(object sender, EventArgs e)
        {
            rateChanged();
            UpdatePacketLength();
        }

        private void basicOp_ltf_SelectedIndexChanged(object sender, EventArgs e)
        {
            GUI_setColor_cmd(cmd_basicOp_setRate, Color.Red);
        }

        private void basicOp_GI_SelectedIndexChanged(object sender, EventArgs e)
        {
            GUI_setColor_cmd(cmd_basicOp_setRate, Color.Red);
            rateChanged();
        }

        private void mcs_Changed(object sender, EventArgs e)
        {
            GUI_setColor_cmd(cmd_basicOp_setRate, Color.Red);
            rateChanged();
            UpdatePacketLength();
        }

        private void ss_Changed(object sender, EventArgs e)
        {
            GUI_setColor_cmd(cmd_basicOp_setRate, Color.Red);
            rateChanged();
            UpdatePacketLength();
        }

        private Mcs GetMcs()
        {
            int _MCS = GUI_GetSelectedValue(combox_basicOp_MCS);

            if (_MCS == (int)idx_mcs_e.idx_mcs_bpsk12)
            {
                return Mcs.MCS_BPSK_12;
            }
            if (_MCS == (int)idx_mcs_e.idx_mcs_bpsk34)
            {
                return Mcs.MCS_BPSK_34;
            }
            if ((_MCS >= (int)idx_mcs_e.idx_mcs_qpsk12) && (_MCS <= (int)idx_mcs_e.idx_mcs_4096qam56))
            {
                return Mcs.MCS_QPSK_12 + _MCS - (int)idx_mcs_e.idx_mcs_qpsk12;
            }
            if ((_MCS >= (int)idx_mcs_e.idx_mcs_80211b_baseline) && (_MCS <= (int)idx_mcs_e.idx_mcs_11mbps_long))
            {
                return Mcs.MCS_80211b_1MBPS_SHORT + _MCS - (int)idx_mcs_e.idx_mcs_80211b_baseline;
            }

            throw new ApplicationException("Invalid MCS value");
        }

        void txRateFixMe(PhyMode _phyType, int _SS, Bandwidth signalBW)
        {
            if (_phyType != PhyMode.PHY_MODE_AC && _phyType != PhyMode.PHY_MODE_N_2_4
                && (GUI_GetSelectedValue(combox_basicOp_MCS) == (int)idx_mcs_e.idx_mcs_64qam56 || GUI_GetSelectedValue(combox_basicOp_MCS) == (int)idx_mcs_e.idx_mcs_256qam34))
                combox_basicOp_MCS.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_64qam34);
            else if ((_phyType == PhyMode.PHY_MODE_AC) || (_phyType == PhyMode.PHY_MODE_N_2_4))
            {
                if (signalBW == Bandwidth.BANDWIDTH_TWENTY && (GUI_GetSelectedValue(combox_basicOp_MCS) == (int)idx_mcs_e.idx_mcs_256qam34)
                    && (_SS == (int)idx_spatialStream_e.idx_spatialStream1 || _SS == (int)idx_spatialStream_e.idx_spatialStream2))
                    combox_basicOp_MCS.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_64qam34);
                else if (signalBW == Bandwidth.BANDWIDTH_EIGHTY && (GUI_GetSelectedValue(combox_basicOp_MCS) == (int)idx_mcs_e.idx_mcs_64qam23) && _SS == (int)idx_spatialStream_e.idx_spatialStream3)
                    combox_basicOp_MCS.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_64qam34);
            }
        }

        private void rateChanged()
        {
            if (!DUT.IsInitialized)
            {
                return;
            }
            if (combox_basicOp_GI.SelectedIndex == GUI_COMBOX_ERROR)
            {
                return;
            }

            PhyMode phyType = GUI_GetSelectedPhyMode();
            Bandwidth _signalBW = GUI_GetSelectedSignalBandwidth();
            Mcs _MCS = GetMcs();
            Gi _GI = (Gi)GUI_GetSelectedValue(combox_basicOp_GI);
            int _SS = ParseControl(combox_basicOp_spatialStream);

            if (phyType < PhyMode.PHY_MODE_A && phyType > PhyMode.PHY_MODE_BE)
            {
                MessageDialog.ShowWarning("Invalid PHY Mode (should be <=8).");
                return;
            }

            if (DUT.GetRate(phyType, _signalBW, _MCS, (byte)_SS, _GI, out float _Rate))
            {
                txtBox_basicOp_phyRate.Enabled = false;
                txtBox_basicOp_phyRate.Text = Convert.ToString(_Rate);
                GUI_setColor_cmd(cmd_basicOp_setRate, Color.Red);
                return;
            }

            txRateFixMe(phyType, _SS, _signalBW);
        }

        private void SetRatebutton_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                if (SetRatebuttonH())
                {
                    GUI_setColor_cmd(cmd_basicOp_setRate, Color.Black);
                }
            }
        }

        private bool SetRatebuttonH()
        {
            PhyMode _phyType = GUI_GetSelectedPhyMode();
            Bandwidth _signalBW = GUI_GetSelectedSignalBandwidth();
            Mcs _MCS = GetMcs();
            byte _SS = (byte)ParseControl(combox_basicOp_spatialStream);
            Gi _GI = (Gi)GUI_GetSelectedValue(combox_basicOp_GI);
            Ltf _LTF = (Ltf)GUI_GetSelectedValue(combox_basicOp_ltf);

            if (DUT.SetRate(_signalBW, _MCS, _SS, _GI, _LTF, out float _Rate))
            {
                /* update textBoxRate */
                txtBox_basicOp_phyRate.Enabled = false;
                txtBox_basicOp_phyRate.Text = Convert.ToString(_Rate);
                /* END update textBoxRate */
                return true;
            }

            txRateFixMe(_phyType, _SS, _signalBW);

            return false;
        }

        void UpdateGUI_ForTXStop()
        {
            updateStopTXbutton();
            updateStartTxButton();
            cmd_enableAll(true); //merav
            groupBox_basicOp_spaceless.Enabled = true;
            cmd_basicOp_startSpaceless.Enabled = true;
            cmd_basicOp_stopSpaceless.Enabled = true;
            cmd_basicOp_transmitCW.Enabled = true;
            cmd_basicOp_stopCW.Enabled = true;
            cmd_basicOp_stopTransmission.ForeColor = Color.Black;
        }
        private void stopTransmission_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                if (isSpacelessTransmission || isCwTransmission)
                    return;

                if (DUT.StopTxPackets())
                {
                    UpdateGUI_ForTXStop();
                }
            }
        }

        private void browseEEPROMfile_Click(object sender, EventArgs e)
        {
            if (openEEPROMfile.ShowDialog() == DialogResult.OK)
            {
                txtBox_nvMemCtrl_eepromFile.Text = openEEPROMfile.FileName;
            }
        }

        static private byte[] convertMACaddrToBytes(string macAddrText)
        {
            UInt64 _macAddr;
            try { _macAddr = UInt64.Parse(macAddrText, NumberStyles.AllowHexSpecifier); }
            catch (System.Exception)
            {
                MessageDialog.ShowWarning("MAC address is not in the correct format.");
                return Array.Empty<byte>();
            }
            byte[] _macBytes = new byte[6];
            for (int i = 5; i >= 0; --i)
            {
                _macBytes[i] = (byte)(_macAddr & 0xFF);
                _macAddr >>= 8;
            }
            return _macBytes;
        }

        static private byte[] getSnBytes(string barcode, int start, int end)
        {
            int snLen = end - start; // calculate the length of the serial number string part
            uint SN = Convert.ToUInt32(barcode.Substring(start, snLen)); // Convert the serial number string to a U32 number
            return BitConverter.GetBytes(SN); // Convert the U32 number to byte array (little endian)
        }

        enum InputsListItems
        {
            RSSI_RX0,
            RSSI_RX1,
            RSSI_RX2,
            PHY_PACKETS,
            PHY_CRC_ERRORS,
            MAC_PACKETS,
            PA_TX0,
            PA_TX1,
            PA_TX2
        }
        enum InputsListItems4ant
        {
            RSSI_RX0,
            RSSI_RX1,
            RSSI_RX2,
            RSSI_RX3,
            RSSI_RX4,
            PHY_PACKETS,
            MPDU_PACKETS = PHY_PACKETS,
            PHY_CRC_ERRORS,
            MPDU_CRC_ERRORS = PHY_CRC_ERRORS,
            MAC_PACKETS,
            PA_TX0,
            PA_TX1,
            PA_TX2,
            PA_TX3
        }
        enum InputsListEvm
        {
            EVM_RX0,
            EVM_RX1,
            EVM_RX2,
            EVM_RX3,
            EVM_RX4
        }

        private void resetCountersBtn_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                DUT.ResetPacketsCounter();
                DUT.ResetMpduPacketsCounter();
                DUT.MacFilterSetCount();
            }
        }

        private void ViewEEPROM_Click(object sender, EventArgs e)
        {
            WriteToConsole("Reading EEPROM, please wait...", Color.RoyalBlue);
            EEPROM eepromDlg = new EEPROM(DUT);
            eepromDlg.ShowDialog();
            eepromDlg.SaveOnExit();
        }

        private void UpdateInputs_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                if (DUT.RSSIVector(out short[] _RSSI))
                {
                    for (int i = 0; i < DUT.RXants.Length; ++i)
                    {
                        if (DUT.RXants[i])
                            inputsList.Items[(int)InputsListItems4ant.RSSI_RX0 + i].SubItems[inputValue.Index].Text = _RSSI[i].ToString("f1") + " dBm";
                        else
                            inputsList.Items[(int)InputsListItems4ant.RSSI_RX0 + i].SubItems[inputValue.Index].Text = "---";
                    }
                }

                // Reading of MPDU counters is only available in W700 when Rx aggregation is enabled
                if ((DUT.HardwareType < HardwareType.HARDWARE_TYPE_GEN7) || (check_basicOp_enableRxAggregation.Checked == false))
                {
                    if (DUT.PhyPacketsCounter(out uint receivedPackets, out uint crcErrors, out uint forwardedPackets))
                    {
                        inputsList.Items[(int)InputsListItems4ant.PHY_PACKETS].SubItems[inputValue.Index].Text = receivedPackets.ToString();
                        inputsList.Items[(int)InputsListItems4ant.PHY_CRC_ERRORS].SubItems[inputValue.Index].Text = crcErrors.ToString();
                        inputsList.Items[(int)InputsListItems4ant.MAC_PACKETS].SubItems[inputValue.Index].Text = forwardedPackets.ToString();
                    }

                    if (DUT.MacFilterGetCount(out uint receivedMacPackets))
                    {
                        inputsList.Items[(int)InputsListItems4ant.MAC_PACKETS].SubItems[inputValue.Index].Text = receivedMacPackets.ToString();
                    }
                }
                else
                {
                    if (DUT.MpduPacketsCounter(out uint receivedPackets, out uint crcErrors))
                    {
                        inputsList.Items[(int)InputsListItems4ant.MPDU_PACKETS].SubItems[inputValue.Index].Text = receivedPackets.ToString();
                        inputsList.Items[(int)InputsListItems4ant.MPDU_CRC_ERRORS].SubItems[inputValue.Index].Text = crcErrors.ToString();
                    }
                }


                if (DUT.ReadVoltageValue(out uint[] PA))
                {
                    for (int i = 0; i < DUT.TXants.Length; ++i)
                    {
                        if (DUT.TXants[i])
                            inputsList.Items[(int)InputsListItems4ant.PA_TX0 + i].SubItems[inputValue.Index].Text = PA[i].ToString("f1");
                        else
                            inputsList.Items[(int)InputsListItems4ant.PA_TX0 + i].SubItems[inputValue.Index].Text = "--";
                    }
                }
            }
        }

        private void LoadDefaultData()
        {
            // Prevent phy changed event from being triggered
            combox_basicOp_phyType.SelectedIndexChanged -= new EventHandler(combox_basicOp_phyType_SelectedIndexChanged);
            // start with 2.4GHz band
            combox_basicOp_band.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_band, (int)Band.BAND_2400MHZ);
            combox_basicOp_phyType.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_phyType, (int)PhyMode.PHY_MODE_N_2_4);

            GUI_basicOp_loadPhyTypeAdjVals();

            comboBox_basicOp_memoryType.SelectedIndex = GUI_GetValueAsIndex(comboBox_basicOp_memoryType, (int)idx_memType_e.idx_flash);

            txtBox_basicOp_repetitions.Text = DEFAULT_TXPARAM_REPETITIONS;
            txtBox_basicOp_packetLen.Text = DEFAULT_TXPARAM_PACKETLENGTH.ToString();
            txtBox_basicOp_numSymbols.Text = DEFAULT_TXPARAM_NUM_OFDM_SYMBOLS.ToString();
            check_basicOp_numSymbols.Checked = false;
            check_basicOp_txLongData.Checked = false;
            check_basicOp_txBeamforming.Enabled = false;
            check_basicOp_txBeamforming.Checked = false;
            groupBox_WriteBeamformingMatrix.Enabled = false;
            txtBox_basicOp_spacingIFS.Text = DEFAULT_IFS.ToString();

            check_basicOp_closeRXants.Checked = false;
            wlan0card.Checked = true;
            wlan2card.Checked = false;
            wlan4card.Checked = false;
            // Restore phy changed event from being triggered
            combox_basicOp_phyType.SelectedIndexChanged += new EventHandler(combox_basicOp_phyType_SelectedIndexChanged);
        }

        private bool RestoreIpAddress()
        {
            try
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey(DUTkey);
                if (key != null)
                {
                    ipAddress.Text = (string)key.GetValue("IP", DUT_t.defaultIpAddress);
                    key.Close();

                    return true;
                }
            }
            catch (Exception ex)
            {
                MessageDialog.ShowError(ex.Message);
            }

            return false;
        }

        private bool RestoreIpSpecificData()
        {
            try
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey(DUTkey);
                if (key != null)
                {
                    RegistryKey ip_rk = key.OpenSubKey(ipAddress.Text);
                    if (ip_rk != null)
                    {
                        if ((int)ip_rk.GetValue("PhyType", 0) < 0)
                        {
                            throw new ApplicationException("Invalid PhyType");
                        }

                        combox_basicOp_phyType.SelectedIndexChanged -= new EventHandler(combox_basicOp_phyType_SelectedIndexChanged);

                        combox_basicOp_band.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_band,
                                                                               (int)ip_rk.GetValue("Band", (int)Band.BAND_2400MHZ));
                        combox_basicOp_phyType.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_phyType,
                                                                                  (int)ip_rk.GetValue("PhyType", PhyMode.PHY_MODE_N_2_4));
                        GUI_basicOp_loadPhyTypeAdjVals();

                        combox_basicOp_spectrumBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spectrumBW,
                                                                                     (int)ip_rk.GetValue("SpectrumBW", Bandwidth.BANDWIDTH_TWENTY));
                        combox_basicOp_signalBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_signalBW,
                                                                                   (int)ip_rk.GetValue("SignalBW", Bandwidth.BANDWIDTH_TWENTY));

                        combox_basicOp_regulationType.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_regulationType,
                                                                                   (int)ip_rk.GetValue("RegulationType", RegulationType.REGULATION_TYPE_FCC_SP));

                        combox_basicOp_MCS.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_MCS,
                                                                              (int)ip_rk.GetValue("MCS", idx_mcs_e.idx_mcs_64qam34));
                        combox_basicOp_spatialStream.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spatialStream,
                                                                                        (int)ip_rk.GetValue("SpatialStream", 0));
                        combox_basicOp_ltf.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_ltf,
                                                                              (int)ip_rk.GetValue("LTF", idx_ltf_e.idx_ltfx1));
                        combox_basicOp_GI.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_GI,
                                                                             (int)ip_rk.GetValue("GI", idx_gi_e.idx_gi08));

                        combox_basicOp_lowChan.SelectedIndex = GUI_GetChannelAsIndex(combox_basicOp_lowChan,
                            (byte)(int)ip_rk.GetValue("Channel", getDefaultChannel(GUI_GetSelectedBand())));

                        checkBox_basicOp_closedLoop.Checked = Convert.ToBoolean(ip_rk.GetValue("ClosedLoop", "False"));
                        checkBox_basicOp_autoPower.Checked = Convert.ToBoolean(ip_rk.GetValue("AutoPower", "False"));
                        txtBox_basicOp_repetitions.Text = (string)ip_rk.GetValue("Repetitions", DEFAULT_TXPARAM_REPETITIONS);
                        check_basicOp_txLongData.Checked = Convert.ToBoolean(ip_rk.GetValue("TxLongData", "False"));
                        check_basicOp_txBeamforming.Checked = Convert.ToBoolean(ip_rk.GetValue("TxBeamforming", "False"));
                        txtBox_basicOp_packetLen.Text = (string)ip_rk.GetValue("packetLength", DEFAULT_TXPARAM_PACKETLENGTH.ToString());
                        txtBox_basicOp_numSymbols.Text = (string)ip_rk.GetValue("NumSymbolsValue", DEFAULT_TXPARAM_NUM_OFDM_SYMBOLS.ToString());
                        check_basicOp_numSymbols.Checked = Convert.ToBoolean(ip_rk.GetValue("NumSymbolsChecked", "False"));
                        txtBox_basicOp_spacingIFS.Text = (string)ip_rk.GetValue("IFS", DEFAULT_IFS.ToString());
                        check_basicOp_closeRXants.Checked = Convert.ToBoolean(ip_rk.GetValue("closeRXantsOnIFS", "False"));
                        txtBox_nvMemCtrl_MACaddr.Text = (string)ip_rk.GetValue("MacAddress", "");
                        txtBox_nvMemCtrl_barcode.Text = (string)ip_rk.GetValue("Barcode", "");
                        txtBox_nvMemCtrl_regModule.Text = (string)ip_rk.GetValue("RegAddr", "");
                        txtBox_nvMemCtrl_regAddress.Text = (string)ip_rk.GetValue("RegValue", "");
                        txtBox_nvMemCtrl_regValue.Text = (string)ip_rk.GetValue("RegMask", "");
                        txtBox_basicOp_powerLevel.Text = (string)ip_rk.GetValue("PowerLevel", "");
                        txtBox_basicOp_powerLimit.Text = (string)ip_rk.GetValue("PowerLimit", "");
                        txtBox_basicOp_cwAmplitude.Text = (string)ip_rk.GetValue("CWamplitude", "");
                        txtBox_basicOp_cwBinNum.Text = (string)ip_rk.GetValue("CWbinNum", "");

                        wlan0card.Checked = Convert.ToBoolean(ip_rk.GetValue("Wlan0Checked", "True"));
                        wlan2card.Checked = Convert.ToBoolean(ip_rk.GetValue("Wlan2Checked", "False"));
                        wlan4card.Checked = Convert.ToBoolean(ip_rk.GetValue("Wlan4Checked", "False"));

                        txtBox_nvMemCtrl_eepromFile.Text = (string)ip_rk.GetValue("EepromFile", "EEPROM.txt");

                        txtBox_beamformingMatrixHeaderFile_standard.Text = (string)ip_rk.GetValue("BeamformingMatrixHeaderFile", "");
                        txtBox_beamformingMatrixValuesFile_standard.Text = (string)ip_rk.GetValue("BeamformingMatrixValuesFile", "");

                        combox_basicOp_phyType.SelectedIndexChanged += new EventHandler(combox_basicOp_phyType_SelectedIndexChanged);

                        return true;
                    }
                }
            }
            catch (Exception ex)
            {
                WriteToConsole("Failed to restore data from registry. Registry values for IP address " + ipAddress.Text + " must be deleted!", Color.Blue);
                MessageDialog.ShowError(ex.Message);
            }

            return false;
        }

        private bool SaveIpAddress()
        {
            try
            {
                RegistryKey key = Registry.CurrentUser.CreateSubKey(DUTkey);
                if (key != null)
                {
                    key.SetValue("IP", DUT.IpAddress);
                    key.Close();

                    return true;
                }
            }
            catch (Exception ex)
            {
                MessageDialog.ShowError(ex.Message);
            }

            return false;
        }

        private void SaveIpSpecificData()
        {
            try
            {
                RegistryKey key = Registry.CurrentUser.CreateSubKey(DUTkey);
                if (key != null)
                {
                    RegistryKey ip_rk = key.CreateSubKey(DUT.IpAddress);
                    if (ip_rk != null)
                    {
                        ip_rk.SetValue("Band", GUI_GetSelectedValue(combox_basicOp_band));
                        ip_rk.SetValue("PhyType", GUI_GetSelectedValue(combox_basicOp_phyType));
                        ip_rk.SetValue("SpectrumBW", GUI_GetSelectedValue(combox_basicOp_spectrumBW));
                        ip_rk.SetValue("SignalBW", GUI_GetSelectedValue(combox_basicOp_signalBW));
                        ip_rk.SetValue("RegulationType", GUI_GetSelectedValue(combox_basicOp_regulationType));
                        ip_rk.SetValue("MCS", GUI_GetSelectedValue(combox_basicOp_MCS));
                        ip_rk.SetValue("LTF", GUI_GetSelectedValue(combox_basicOp_ltf));
                        ip_rk.SetValue("SpatialStream", GUI_GetSelectedValue(combox_basicOp_spatialStream));
                        ip_rk.SetValue("GI", GUI_GetSelectedValue(combox_basicOp_GI));
                        if (combox_basicOp_lowChan.SelectedItem != null)
                        {
                            ip_rk.SetValue("Channel", (int)(byte)combox_basicOp_lowChan.SelectedItem);
                        }
                        ip_rk.SetValue("PacketLength", txtBox_basicOp_packetLen.Text);
                        ip_rk.SetValue("NumSymbolsValue", txtBox_basicOp_numSymbols.Text);
                        ip_rk.SetValue("NumSymbolsChecked", check_basicOp_numSymbols.Checked);
                        ip_rk.SetValue("ClosedLoop", checkBox_basicOp_closedLoop.Checked);
                        ip_rk.SetValue("AutoPower", checkBox_basicOp_autoPower.Checked);
                        ip_rk.SetValue("Repetitions", txtBox_basicOp_repetitions.Text);
                        ip_rk.SetValue("TxLongData", check_basicOp_txLongData.Checked);
                        ip_rk.SetValue("TxBeamforming", check_basicOp_txBeamforming.Checked);
                        ip_rk.SetValue("MacAddress", txtBox_nvMemCtrl_MACaddr.Text);
                        ip_rk.SetValue("Barcode", txtBox_nvMemCtrl_barcode.Text);
                        ip_rk.SetValue("RegAddr", txtBox_nvMemCtrl_regModule.Text);
                        ip_rk.SetValue("RegValue", txtBox_nvMemCtrl_regAddress.Text);
                        ip_rk.SetValue("RegMask", txtBox_nvMemCtrl_regValue.Text);
                        ip_rk.SetValue("PowerLevel", txtBox_basicOp_powerLevel.Text);
                        ip_rk.SetValue("PowerLimit", txtBox_basicOp_powerLimit.Text);
                        ip_rk.SetValue("IFS", txtBox_basicOp_spacingIFS.Text);
                        ip_rk.SetValue("CWamplitude", txtBox_basicOp_cwAmplitude.Text);
                        ip_rk.SetValue("CWbinNum", txtBox_basicOp_cwBinNum.Text);

                        ip_rk.SetValue("Wlan0Checked", wlan0card.Checked);
                        ip_rk.SetValue("Wlan2Checked", wlan2card.Checked);
                        ip_rk.SetValue("Wlan4Checked", wlan4card.Checked);

                        ip_rk.SetValue("EepromFile", txtBox_nvMemCtrl_eepromFile.Text);

                        ip_rk.SetValue("BeamformingMatrixHeaderFile", txtBox_beamformingMatrixHeaderFile_standard.Text);
                        ip_rk.SetValue("BeamformingMatrixValuesFile", txtBox_beamformingMatrixValuesFile_standard.Text);

                        ip_rk.Close();
                    }

                    key.Close();
                }
            }
            catch (Exception ex)
            {
                MessageDialog.ShowError(ex.Message);
            }
        }

        private void DUT_GUI_FormClosing(object sender, FormClosingEventArgs e)
        {
            WriteToConsole("------ DUT GUI Closing... @ " + ConsoleOutput.GetTimeStamp() + " ------", Color.Black);
            Terminate();
            NativeMethods.FreeConsole();
            IntPtr handle = NativeMethods.GetConsoleWindow();
            _ = NativeMethods.SetStdHandle(-11, handle); // -11 is STD_OUTPUT_HANDLE
            logFile.Close();
        }

        private void Terminate()
        {
            if (DUT.IsInitialized)
            {
                if (SaveIpAddress())
                {
                    SaveIpSpecificData();
                }

                // Release old connection.
                DUT.DriverRelease();
            }
        }

        private void Reconnect()
        {
            if (DUT.IsInitialized)
            {
                WriteToConsole("Reconnecting", Color.DarkCyan);
                Terminate();
            }

            try
            {
                if (!DUT.Connect(ipAddress.Text, DUT_t.defaultTcpPort, DUT_t.defaultConnectionTimeout))
                {
                    throw new ApplicationException("Connection timeout! Make sure the IP address is correct and DUT Server is running on the AP.");
                }
            }
            catch (Exception)
            {
                extendedOperationTab.Enabled = false;
                nvMemControlTab.Enabled = false;
                groupBox_tx.Enabled = false;
                groupBox_genRisc.Enabled = false;
                groupBox_antennasOnOff.Enabled = false;
                groupBox_basicOp_misc.Enabled = false;
                txtBox_basicOp_CalMode.Enabled = false;
                panel_powerParam_cmd.Enabled = false;
                panel_loop_cmd.Enabled = false;
                groupBox_cardID.Enabled = false;
                groupBox_basicOp_spaceless.Enabled = false;

                ipAddress.BackColor = Color.MistyRose;

                throw;
            }

            byte wlanIndex = getWlanIndex();
            bool snifferMode = snifferEnable.Checked;
            getMemCfg(GUI_GetSelectedValue(comboBox_basicOp_memoryType), out NvMemoryType memoryType, out NvMemorySize memorySize);

            if (DUT.Initialize(wlanIndex, snifferMode, memoryType, memorySize))
            {
                updateUIControlForConnection(false);

                tree_tpc_tpc.Nodes.Clear();

                if (DUT.ReadChipId(out string chipId))
                {
                    txtBox_nvMemCtrl_RFchipVersion.Text = "0x0";
                    txtBox_nvMemCtrl_BBchipVersion.Text = chipId;
                }

                // Init will set all
                //
                Init();


                updateStartTxButton(); //Abraham
                updateStopTXbutton(); //Abraham

                /////////////////////////////////////////////////////////

                ReadCalibrationData();
                updateControlsState();

                updateUIControlForConnection(true);

                if (tabControl.SelectedIndex == 1)
                {
                    this.check_extendedOp_xtalEnable.Checked = true;
                    xtal_enable_CheckedChanged(null, null);
                    read_xtal_Click(null, null);
                }

                updateBandWhenConnected();
                extendedOperationTab.Enabled = true;
                nvMemControlTab.Enabled = true;
                groupBox_tx.Enabled = true;
                groupBox_genRisc.Enabled = true;
                groupBox_antennasOnOff.Enabled = true;
                groupBox_basicOp_misc.Enabled = true;
                txtBox_basicOp_CalMode.Enabled = true;
                panel_powerParam_cmd.Enabled = true;
                panel_loop_cmd.Enabled = true;
                groupBox_cardID.Enabled = true;
                groupBox_basicOp_spaceless.Enabled = true;
                check_basicOp_enableRxAggregation.Checked = false;

                GUI_setColor_cmd(cmd_basicOp_setRate, Color.Red);
                GUI_setColor_cmd(cmd_basicOp_setPower, Color.Red);
                GUI_setColor_cmd(cmd_basicOp_setSpacing, Color.Red);
                GUI_setColor_cmd(cmd_basicOp_startTransmitting, Color.Red);
                GUI_setColor_cmd(cmd_basicOp_stopTransmission, Color.Red);
                GUI_setColor_cmd(cmd_basicOp_startSpaceless, Color.Red);

                if (snifferEnable.Checked && DUT.GetTxAntennasMask(out AntennaMask antennaMask))
                {
                    for (byte i = 0; i < DUT.TXants.Length; ++i)
                    {
                        bool isOn = antennaMask.isBitSet(i);

                        if (isOn)
                        {
                            TXAntClick(m_txAntButtons[i], i);
                        }
                    }
                }

                WriteToConsole("Ready.", Color.DarkCyan);
            }

            ipAddress.BackColor = Color.PaleGreen;
            ConnectButton.ForeColor = Color.Black;
        }

        private void updateUIControlForConnection(bool enabled)
        {
            tabControl.Enabled = enabled;
            groupBox_connection.Enabled = enabled;
        }
        private void ConnectButton_Click(object sender, EventArgs e)
        {
            WriteToConsole("------ ConnectButton Click @ " + ConsoleOutput.GetTimeStamp() + " ------", Color.Black);

            // Set cursor as hourglass until connection completes
            using (new WaitCursorBlock(this))
            {
                try
                {
                    Reconnect();
                }
                catch (Exception ex)
                {
                    WriteToConsole(ex.Message, Color.Red);
                }
            }
        }

        void updateBandWhenConnected()
        {
            Band bandCurrent = DUT.RfBandCurrent;//5/2.4 should be ready here to get
            Band[] supportedBands = DUT.RfSupportedBands;
            int old_bandIndex = combox_basicOp_band.SelectedIndex;
            if (bandCurrent != Band.BAND_INVALID)
            {
                if (supportedBands.Length < 2)
                {
                    combox_basicOp_band.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_band, (int)bandCurrent);//band is the same like index index0=5GHz=0
                    combox_basicOp_band.Enabled = false;
                }
                else
                {
                    if (!supportedBands.Contains(Band.BAND_6000MHZ))
                    {
                        GUI_removeSingleBoxItem(combox_basicOp_band, (int)Band.BAND_6000MHZ);
                    }
                    if (!supportedBands.Contains(Band.BAND_5000MHZ))
                    {
                        GUI_removeSingleBoxItem(combox_basicOp_band, (int)Band.BAND_5000MHZ);
                    }
                    if (!supportedBands.Contains(Band.BAND_2400MHZ))
                    {
                        GUI_removeSingleBoxItem(combox_basicOp_band, (int)Band.BAND_2400MHZ);
                    }

                    combox_basicOp_band.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_band, (int)bandCurrent);
                    combox_basicOp_band.Enabled = false;
                }
                if (GUI_GetSelectedBand() != bandCurrent)
                {//if both support 2.4g&5g,then we should change band index according to bandcurrent. (here is the place after restore value from registry)
                    combox_basicOp_band.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_band, (int)bandCurrent);
                }
                if ((old_bandIndex >= 0) && (old_bandIndex != combox_basicOp_band.SelectedIndex))
                {
                    align_phyTypeAdjVals_to_band();
                }
            }
        }

        private void comboSpectrumBW_SelectedIndexChanged(object sender, EventArgs e)
        {
            Bandwidth selSpectrumBW = GUI_GetSelectedSpectrumBandwidth();

            combox_basicOp_primaryChannelIndex.Items.Clear();
            for (int i = 0; i < Math.Pow(2, (int)selSpectrumBW); i++)
            {
                combox_basicOp_primaryChannelIndex.Items.Add(i.ToString());
            }
            combox_basicOp_primaryChannelIndex.SelectedIndex = 0;

            combox_basicOp_signalBW.Items.Clear();
            GUI_fillComboBox(combox_basicOp_signalBW);
            GUI_removeMultipleBoxItems(combox_basicOp_signalBW, (int)selSpectrumBW);
            combox_basicOp_signalBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);

            GUI_setColor_cmd(cmd_basicOp_setChannel, Color.Red);
            updateFreq();
            rateChanged();
        }

        private void UpdateCurEEPROMInfo_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                if (DUT.GetEEPROMInfo(out byte countryCode, out byte[] macAddress, out byte[] serialNumber, out byte week, out byte year))
                {
                    txtBox_nvMemCtrl_MACaddr.Text = "";
                    for (int i = 0; i < macAddress.Length; ++i)
                    {
                        txtBox_nvMemCtrl_MACaddr.Text += macAddress[i].ToString("X2");
                    }

                    int SN = serialNumber[0] + (serialNumber[1] << 8) + (serialNumber[2] << 16);
                    if (check_nvMemCtrl_SNformatBarcode.Checked)
                    {
                        if (Char.IsDigit((char)year))
                        {
                            if (Char.IsDigit((char)week))
                            {
                                /* SN = ######### */
                                txtBox_nvMemCtrl_serial.Text = ((char)week).ToString()
                                    + ((char)year).ToString() + SN.ToString("d7");
                            }
                            else
                            { /* SN = X######## */
                                txtBox_nvMemCtrl_serial.Text = ((char)year).ToString() + SN.ToString("d7");
                            }
                        }
                        else
                        { /* SN = XX####### */
                            txtBox_nvMemCtrl_serial.Text = SN.ToString("d7");
                        }

                        txtBox_nvMemCtrl_week.Text = week.ToString("d2");
                        txtBox_nvMemCtrl_year.Text = year.ToString("d2");
                        txtBox_nvMemCtrl_barcode.Text = ((char)week).ToString() + ((char)year).ToString() + SN.ToString("d7");
                    }
                    else
                    {
                        txtBox_nvMemCtrl_serial.Text = SN.ToString("d6");
                        txtBox_nvMemCtrl_week.Text = week.ToString("d2");
                        txtBox_nvMemCtrl_year.Text = year.ToString("d2");
                        txtBox_nvMemCtrl_barcode.Text = year.ToString("d2") + week.ToString("d2") + "??" + SN.ToString("d5");
                    }

                    try
                    {
                        CountryInfo countryInfo = Country.getCountryByCode(countryCode);

                        combox_nvMemCtrl_country.SelectedItem = countryInfo;
                        txtBox_nvMemCtrl_country.Text = countryInfo.fullName + " (" + countryInfo.code.ToString() + ")";
                    }
                    catch (Exception)
                    {
                        combox_nvMemCtrl_country.SelectedItem = null;
                        txtBox_nvMemCtrl_country.Text = "";
                    }
                }

                if (DUT.ReadProdFlag(out bool productionFlag))
                {
                    combox_nvMemCtrl_prodFlag.SelectedItem = getProductionFlagAsString(productionFlag);
                    combox_nvMemCtrl_prodValue.Text = getProductionFlagAsValue(productionFlag);
                }
            }
        }

        private byte getWlanIndex()
        {
            if (wlan0card.Checked)
            {
                return 0;
            }
            else if (wlan2card.Checked)
            {
                return 2;
            }
            else
            {
                return 4;
            }
        }

        private void updateStopTXbutton()
        {
            cmd_basicOp_stopTransmission.Enabled = DUT.IsTransmitting;

            if (!DUT.IsTransmitting)
            {
                cmd_basicOp_startTransmitting.Enabled = true;
                cmd_basicOp_setPower.Enabled = true;
            }
        }

        private void updateStartTxButton()
        {
            cmd_basicOp_startTransmitting.Enabled = !DUT.IsTransmitting;
        }

        void updateFreq()
        {
            try
            {
                int ch = ParseControl(combox_basicOp_lowChan);
                PhyMode phyMode = GUI_GetSelectedPhyMode();
                Band band = GUI_GetSelectedBand();
                Bandwidth bandwidth = GUI_GetSelectedSpectrumBandwidth();
                ushort centerFrequency = Channels.getCenterFrequency((byte)ch, phyMode, band, bandwidth);
                txtBox_basicOp_CenterFreqMHz.TextChanged -= new System.EventHandler(CenterFreqMHz_TextChanged);
                txtBox_basicOp_CenterFreqMHz.Text = Convert.ToString(centerFrequency);
                txtBox_basicOp_CenterFreqMHz.TextChanged += new System.EventHandler(CenterFreqMHz_TextChanged);
            }
            catch (Exception)
            {
                txtBox_basicOp_CenterFreqMHz.Text = "INVALID";
            }
        }

        private void CenterFreqMHz_TextChanged(object sender, EventArgs e)
        {
            ushort newCenterFreqMHz = (ushort)ParseControl(txtBox_basicOp_CenterFreqMHz);
            if ((prevCenterFreqMHz != newCenterFreqMHz) && (newCenterFreqMHz > MIN_FREQUENCY))
            {
                PhyMode phyMode = GUI_GetSelectedPhyMode();
                Band band = GUI_GetSelectedBand();
                Bandwidth bandwidth = GUI_GetSelectedSpectrumBandwidth();
                try
                {
                    uint newChannel = Channels.getLowestChannel(newCenterFreqMHz, phyMode, band, bandwidth);
                    if (newChannel != INVALID_CHANNEL)
                    {
                        combox_basicOp_lowChan.Text = Convert.ToString(newChannel);
                        channel_TextChanged(sender, e);
                    }

                    prevCenterFreqMHz = newCenterFreqMHz;
                    GUI_setColor_cmd(cmd_basicOp_setChannel, Color.Red);
                }
                catch (Exception ex)
                {
                    WriteToConsole(ex.Message, Color.Red);
                    txtBox_basicOp_CenterFreqMHz.Text = Convert.ToString(prevCenterFreqMHz);
                }
            }
        }

        private void channel_TextChanged(object sender, EventArgs e)
        {
            int newChannel = ParseControl(combox_basicOp_lowChan);
            if (prevChannel != newChannel)
            {
                prevChannel = newChannel;
                cmd_basicOp_setChannel.ForeColor = Color.Red;
            }
            updateFreq();
        }

        private void UpdateChannelsList()
        {
            combox_basicOp_lowChan.Items.Clear();

            PhyMode phyMode = GUI_GetSelectedPhyMode();
            Band band = GUI_GetSelectedBand();
            Bandwidth bandwidth = GUI_GetSelectedSpectrumBandwidth();

            Channels.getChannels(phyMode, band, bandwidth, out byte[] channels);
            foreach (byte channel in channels)
            {
                combox_basicOp_lowChan.Items.Add(channel);
            }

            combox_basicOp_lowChan.SelectedIndex = 0;
        }

        private void TransmitCWbtn_Click(object sender, EventArgs e)
        {
            if (!transmitCheckConditions())
            {
                return;
            }
            int amp = ParseControl(txtBox_basicOp_cwAmplitude);
            int tone = ParseControl(txtBox_basicOp_cwBinNum);

            using (new WaitCursorBlock(this))
            {
                if (DUT.StartCW((sbyte)amp, (Int16)tone))
                {
                    isCwTransmission = true;
                    cmd_basicOp_transmitCW.Enabled = false;
                    cmd_basicOp_stopCW.Enabled = true;
                    cmd_basicOp_stopCW.ForeColor = Color.Red;
                    cmd_basicOp_startTransmitting.Enabled = false;
                    cmd_basicOp_stopTransmission.Enabled = false;
                    cmd_basicOp_startSpaceless.Enabled = false;
                    cmd_basicOp_stopSpaceless.Enabled = false;
                    cmd_enableAll(false); //merav
                }
            }
        }

        private void setIFSbtn_Click(object sender, EventArgs e)
        {
            int IFSval = ParseControl(txtBox_basicOp_spacingIFS);

            if (IFSval <= 0)
            {
                MessageDialog.ShowWarning("Please specify a valid IFS.");
                return;
            }

            using (new WaitCursorBlock(this))
            {
                if (check_basicOp_closeRXants.Checked)
                {
                    for (byte i = 0; i < DUT.RXants.Length; ++i)
                    {
                        bool isOn = DUT.RXants[i];

                        if (isOn && (!SetRXant(m_rxAntButtons[i], i, false)))
                        {
                            return;
                        }
                    }
                }

                if (DUT.SetIFS((uint)IFSval))
                {
                    cmd_basicOp_setSpacing.ForeColor = Color.Black;
                }
            }
        }

        private string BandToFreqString(Band band)
        {
            string s = "2.4";
            if (band == Band.BAND_5000MHZ) { s = "5"; }
            if (band == Band.BAND_6000MHZ) { s = "6"; }
            s += " GHz";

            return s;
        }

        private string getChannelAndFreq(byte ch, Band band)
        {
            try
            {
                PhyMode phyMode = GUI_GetSelectedPhyMode();
                Bandwidth bandwidth = GUI_GetSelectedSpectrumBandwidth();

                return "Channel " + ch + " (" + Channels.getCenterFrequency(ch, phyMode, band, bandwidth) + " MHz)";
            }
            catch (System.Exception)
            {
                return "Channel " + ch + " (on " + BandToFreqString(band) + ")";
            }
        }

        private string getChannelAndFreqForRSSI(ushort freq)
        {
            PhyMode phyMode = GUI_GetSelectedPhyMode();
            Band band = semiOpModeManager.getBandbyFreq(freq);
            Bandwidth bandwidth = GUI_GetSelectedSpectrumBandwidth();

            try
            {
                return "Channel=" + Channels.getLowestChannel(freq, phyMode, band, bandwidth) + " (" + freq + " MHz)";
            }
            catch (System.Exception)
            {
                return "Freq=" + freq + " MHz (on " + BandToFreqString(band) + ")";
            }
        }

        // Fill the TPC tree
        private void ReadCalibrationDataW600_W700()
        {
            txtBox_tpc_eepromVersion.Text = DUT.EEPROMversion.ToString();
            tree_tpc_tpc.Nodes.Clear();

            if (!DUT.GetTxAntennasMask(out AntennaMask txAntennaMask))
            {
                return;
            }
            if (!DUT.GetRxAntennasMask(out AntennaMask rxAntennaMask))
            {
                return;
            }

            DUT.GetCalibrationFileVersion(out CalibrationFileVersion version, out CalibrationFileSubversion subversion);

            if (DUT.GetTssiCalData(out version, out TssiCalibrationData[] tssiCalibrationData) && (tssiCalibrationData.Length > 0))
            {
                if ((version == CalibrationFileVersion.CALIBRATION_FILE_VERSION_6) || (version == CalibrationFileVersion.CALIBRATION_FILE_VERSION_7))
                {
                    WriteLine("ReadCalibrationData, Loading TX data start");
                    TreeNode TSSInode = new TreeNode("TSSI");
                    TreeNode MACnode = new TreeNode("MAC");

                    TssiCalibrationDataVer6[] tssiCalibrationDataVer6 = new TssiCalibrationDataVer6[tssiCalibrationData.Length];
                    for (int i = 0; i < tssiCalibrationData.Length; i++)
                    {
                        tssiCalibrationDataVer6[i] = (TssiCalibrationDataVer6)tssiCalibrationData[i];
                    }

                    byte numRegions = tssiCalibrationDataVer6[0].numRegions;
                    TpcCisStructType CisStructType = TpcCisStructType.TPC_CIS_FREQ_1S2D + numRegions - 1;

                    foreach (TssiCalibrationDataVer6 calData in tssiCalibrationDataVer6)
                    {
                        string baseName = getChannelAndFreq((byte)calData.channel, calData.band);
                        if (calData.bw == Bandwidth.BANDWIDTH_TWENTY)
                            baseName += ", 20MHz";
                        else if (calData.bw == Bandwidth.BANDWIDTH_FOURTY)
                            baseName += ", 40MHz";
                        else if (calData.bw == Bandwidth.BANDWIDTH_EIGHTY)
                            baseName += ", 80MHz";
                        else if (calData.bw == Bandwidth.BANDWIDTH_ONE_HUNDRED_SIXTY)
                        {
                            if (calData.band == Band.BAND_2400MHZ)
                            {
                                baseName += ", 20MHz (11b)";
                            }
                            else
                            {
                                baseName += ", 160MHz";
                            }
                        }
                        else if (calData.bw == Bandwidth.BANDWIDTH_THREE_HUNDRED_TWENTY)
                        {
                            baseName += ", 320MHz";
                        }

                        TreeNode baseNode = new TreeNode(baseName);
                        TssiCalibrationDataVer6.AntennaData[] ants = calData.antennas;
                        for (byte ant = 0; ant < ants.Length; ++ant)
                        {
                            if (!calData.antennaMask.isBitSet(ant))
                            {
                                continue;
                            }
                            if (ants[ant] == null)
                            {
                                continue;
                            }

                            // --------------------------------------------------------------------
                            // Due to a bug in IQDVT, TPC power in Wave700 calibration files is in
                            // dB units for subversion 0.
                            // However, firmware expects it to be in half-dB units.
                            // This problem did not occur in Wave600 calibration files and has been
                            // fixed for Wave700 in subversion 1 and subsequent.
                            // See WLANRTSYS-55425
                            byte maxPower = ants[ant].maxPower;
                            byte uEvm = ants[ant].uEvm;
                            if
                            (
                                (version == CalibrationFileVersion.CALIBRATION_FILE_VERSION_7) &&
                                (subversion == CalibrationFileSubversion.CALIBRATION_FILE_SUBVERSION_0)
                            )
                            {
                                maxPower *= 2;
                                uEvm *= 2;
                            }
                            // --------------------------------------------------------------------

                            int ant1based = ant + 1;
                            TreeNode antNode = new TreeNode("Antenna " + ant1based.ToString());
                            string powerStr = "Max Power=" + maxPower.ToString() + ", uEVM=" + uEvm.ToString() + ", pOutOffset=" + ants[ant].uEvmGain.ToString();
                            antNode.Nodes.Add(powerStr);
                            for (int region = 0; region < calData.numRegions; region++)
                            {
                                // Add A+B:
                                TssiAB point = ants[ant].pointsAB[region];
                                TreeNode regionNode = new TreeNode("Region. " + region.ToString());
                                string pointStr = "A=" + point.a.ToString("f2") + ", B=" + point.b.ToString("f2");
                                if ((CisStructType == TpcCisStructType.TPC_CIS_FREQ_1S2D) || (CisStructType == TpcCisStructType.TPC_CIS_FREQ_2S2D) || (CisStructType == TpcCisStructType.TPC_CIS_FREQ_3S2D))
                                {
                                    // Add S2Ds:
                                    string s2dStr = "S2D Gain=" + ants[ant].s2dGain[region].ToString() + ", S2D Offset=" + ants[ant].s2dOffset[region].ToString();
                                    regionNode.Nodes.Add(s2dStr);
                                }
                                regionNode.Nodes.Add(pointStr);
                                antNode.Nodes.Add(regionNode);
                            }
                            baseNode.Nodes.Add(antNode);
                        }
                        TSSInode.Nodes.Add(baseNode);
                    }

                    byte _channel = (byte)ParseControl(combox_basicOp_lowChan);
                    Band _band = GUI_GetSelectedBand();

                    Bandwidth[] MACbw =
                    {
                        Bandwidth.BANDWIDTH_ONE_HUNDRED_SIXTY,
                        Bandwidth.BANDWIDTH_EIGHTY,
                        Bandwidth.BANDWIDTH_FOURTY,
                        Bandwidth.BANDWIDTH_TWENTY,
                        Bandwidth.BANDWIDTH_ONE_HUNDRED_SIXTY,
                        Bandwidth.BANDWIDTH_EIGHTY,
                        Bandwidth.BANDWIDTH_FOURTY,
                        Bandwidth.BANDWIDTH_TWENTY
                    };

                    string[] MACnames = { "1st BW. 160", "2nd BW. 80", "3rd BW. 40", "4th BW. 20", "1st BW. 160", "2nd BW. 80", "3rd BW. 40", "4th BW. 20" };
                    for (int i = 0; i < MACbw.Length; i++)
                    {
                        DUT_t.GetTpcFreqs(tssiCalibrationDataVer6, _channel, MACbw[i], _band, out List<TpcFreqVer6> tpcFreqs);
                        byte channel = 0;
                        uint distance = uint.MaxValue;
                        int tssiFreqIndex = i / (MACbw.Length / 2);
                        TssiCalibrationDataVer6.AntennaData[] ants = null;

                        if (tpcFreqs.Count > tssiFreqIndex)
                        {
                            channel = (byte)tpcFreqs[tssiFreqIndex].data.channel;
                            distance = tpcFreqs[tssiFreqIndex].distance;
                            ants = tpcFreqs[tssiFreqIndex].data.antennas;
                        }

                        string nodeStr = MACnames[i] + " - " + getChannelAndFreq(channel, GUI_GetSelectedBand());
                        if (distance == 0)
                        {
                            nodeStr += " (!)";
                        }

                        if ((CisStructType == TpcCisStructType.TPC_CIS_FREQ_1S2D) || (CisStructType == TpcCisStructType.TPC_CIS_FREQ_2S2D) || (CisStructType == TpcCisStructType.TPC_CIS_FREQ_3S2D))
                        {
                            TreeNode baseNode = new TreeNode(nodeStr);

                            for (byte ant = 0; (ants != null) && (ant < DUT.TXants.Length); ++ant)
                            {
                                if (!txAntennaMask.isBitSet(ant))
                                {
                                    continue;
                                }
                                if (ants[ant] == null)
                                {
                                    continue;
                                }

                                TreeNode antNode = new TreeNode("Ant " + (ant + 1) + " - Channel " + channel);
                                string Power = "Max Power=" + ants[ant].maxPower.ToString() + ", uEVM=" + ants[ant].uEvm.ToString() + ", pOutOffset=" + ants[ant].uEvmGain.ToString();
                                antNode.Nodes.Add(Power);
                                for (int region = 0; region < numRegions; region++)
                                {
                                    TssiAB point = ants[ant].pointsAB[region];
                                    int Offset = ants[ant].s2dOffset[region];
                                    int Gain = ants[ant].s2dGain[region];

                                    string S2D = "Region" + region.ToString() + ": S2D Gain=" + Gain.ToString() + ", S2D Offset=" + Offset.ToString();
                                    antNode.Nodes.Add(S2D);
                                    string pointStr = "Region" + region.ToString() + ": A=" + point.a.ToString() + ", B=" + point.b.ToString();
                                    antNode.Nodes.Add(pointStr);
                                }
                                baseNode.Nodes.Add(antNode);
                            }
                            MACnode.Nodes.Add(baseNode);
                        }
                    }

                    tree_tpc_tpc.Nodes.Add(TSSInode);
                    tree_tpc_tpc.Nodes.Add(MACnode);
                    WriteLine("ReadCalibrationData, Loading TX data end");
                }
                else
                {
                    WriteLine("Unexpected calibration file version " + version.ToString());
                }
            }

            if (DUT.GetRssiCalData(out version, out RssiCalibrationData[] rssiCalibrationData) && (rssiCalibrationData.Length > 0))
            {
                if ((version == CalibrationFileVersion.CALIBRATION_FILE_VERSION_6) || (version == CalibrationFileVersion.CALIBRATION_FILE_VERSION_7))
                {
                    WriteLine("ReadCalibrationData, Loading RX data start");
                    TreeNode RSSInode = new TreeNode("RSSI");

                    if (version == CalibrationFileVersion.CALIBRATION_FILE_VERSION_6)
                    {
                        foreach (RssiCalibrationDataVer6 calData in rssiCalibrationData)
                        {
                            string baseName = getChannelAndFreqForRSSI(calData.calibrationFreq);
                            TreeNode baseNode = new TreeNode(baseName);

                            string startFreqName = getChannelAndFreqForRSSI(calData.startFreq);
                            TreeNode startFreqNode = new TreeNode("Start " + startFreqName);
                            string stopFreqName = getChannelAndFreqForRSSI(calData.stopFreq);
                            TreeNode stopFreqNode = new TreeNode("Stop " + stopFreqName);
                            TreeNode chipTempNode = new TreeNode("Chip Temperature=" + calData.chipTemperature.ToString() + " �C");
                            baseNode.Nodes.Add(startFreqNode);
                            baseNode.Nodes.Add(stopFreqNode);
                            baseNode.Nodes.Add(chipTempNode);
                            RssiCalibrationDataVer6.AntennaData[] ants = calData.antennas;
                            for (byte rx_ant = 0; rx_ant < ants.Length; ++rx_ant)
                            {
                                if (!calData.antennaMask.isBitSet(rx_ant))
                                {
                                    continue;
                                }
                                if (ants[rx_ant] == null)
                                {
                                    continue;
                                }
                                int ant1based = rx_ant + 1;
                                TreeNode antNode = new TreeNode("Antenna " + ant1based.ToString());
                                /*LNA Mid Gain*/
                                string lnaMidGainStr = "LNA Mid Gain=" + ants[rx_ant].lnaMidGain.ToString();
                                antNode.Nodes.Add(lnaMidGainStr);
                                /*LNA Gain*/
                                TreeNode lnaGainNode = new TreeNode("LNA Gain");
                                for (int lnaGainIdx = 0; lnaGainIdx < Defines.maxNumLnaGainStepsVer6; lnaGainIdx++)
                                {
                                    TreeNode lnaGainValueNode = new TreeNode(ants[rx_ant].lnaGains[lnaGainIdx].ToString("f1"));
                                    lnaGainNode.Nodes.Add(lnaGainValueNode);
                                }
                                antNode.Nodes.Add(lnaGainNode);
                                /*Region*/
                                for (int region = 0; region < RssiCalibrationDataVer6.maxNumRegions; region++)
                                {
                                    // Add A+B:
                                    RssiAB point = ants[rx_ant].pointsAB[region];
                                    TreeNode regionNode = new TreeNode("Region " + region.ToString());
                                    string pointStr = "A=" + point.a.ToString("f2") + ", B=" + point.b.ToString("f2");

                                    // Add S2Ds:
                                    string s2dStr = "S2D Gain=" + ants[rx_ant].s2dGainOffset[region].gain.ToString() + ", S2D Offset=" + ants[rx_ant].s2dGainOffset[region].offset.ToString();
                                    regionNode.Nodes.Add(s2dStr);
                                    regionNode.Nodes.Add(pointStr);
                                    antNode.Nodes.Add(regionNode);
                                }
                                /*RF Flatness*/
                                TreeNode RFFlatnessNode = new TreeNode("RF Flatness");
                                for (int RFfreqPointIdx = 0; RFfreqPointIdx < calData.numRxRfFlatnessPoints; RFfreqPointIdx++)
                                {
                                    string frequency = calData.rxRfFlatnessFreqs[RFfreqPointIdx].ToString() + " MHz";
                                    TreeNode RFFreqPointNode = new TreeNode("Freq Point[" + RFfreqPointIdx.ToString() + "]=" + frequency);
                                    TreeNode PerGainValueNode = new TreeNode("MidGainDelta=" + ants[rx_ant].midGainDeltaPoints[RFfreqPointIdx].ToString() +
                                        ";RXGainDelta=" + ants[rx_ant].rxRfFlatnessDeltaPointsHighGain[RFfreqPointIdx].ToString() +
                                        ";Bypass=" + ants[rx_ant].rxRfFlatnessDeltaPointsBypass[RFfreqPointIdx].ToString());
                                    RFFreqPointNode.Nodes.Add(PerGainValueNode);

                                    RFFlatnessNode.Nodes.Add(RFFreqPointNode);
                                }
                                if (0 == calData.numRxRfFlatnessPoints)
                                {
                                    TreeNode RFNullNode = new TreeNode("Null");
                                    RFFlatnessNode.Nodes.Add(RFNullNode);
                                }
                                antNode.Nodes.Add(RFFlatnessNode);

                                /*RSSI Flatness*/
                                TreeNode RSSIFlatnessNode = new TreeNode("RSSI Flatness");
                                for (int RSSIFreqPointIdx = 0; RSSIFreqPointIdx < calData.numRxRssiFlatnessPoints; RSSIFreqPointIdx++)
                                {
                                    string frequency = calData.rxRssiFlatnessFreqs[RSSIFreqPointIdx].ToString() + " MHz";
                                    TreeNode RSSIFreqPointNode = new TreeNode("Freq Point[" + RSSIFreqPointIdx.ToString() + "]=" + frequency);
                                    TreeNode RSSIDeltaNode = new TreeNode("RSSI Delta=" + ants[rx_ant].rxRssiFlatnessDeltaPoints[RSSIFreqPointIdx].ToString());
                                    RSSIFreqPointNode.Nodes.Add(RSSIDeltaNode);

                                    RSSIFlatnessNode.Nodes.Add(RSSIFreqPointNode);
                                }
                                if (0 == calData.numRxRssiFlatnessPoints)
                                {
                                    TreeNode RSSINullNode = new TreeNode("Null");
                                    RSSIFlatnessNode.Nodes.Add(RSSINullNode);
                                }
                                antNode.Nodes.Add(RSSIFlatnessNode);
                                /*Sub Band Cross Points*/
                                TreeNode subBandCP = new TreeNode("Sub Bands Crossing Points");
                                for (int subBandIdx = 0; subBandIdx < Defines.maxNumLnaSubBandsCrossingPointsVer6; subBandIdx++)
                                {
                                    int crossPointIndex = ants[rx_ant].firstCrossingPointIndex + subBandIdx;
                                    string frequency = ants[rx_ant].subBandsCrossingPoints[subBandIdx].ToString() + " MHz";
                                    TreeNode subBandFreqPointNode = new TreeNode("Freq Point[" + crossPointIndex.ToString() + "]=" + frequency);
                                    subBandCP.Nodes.Add(subBandFreqPointNode);
                                }
                                antNode.Nodes.Add(subBandCP);

                                baseNode.Nodes.Add(antNode);
                            }
                            RSSInode.Nodes.Add(baseNode);
                        }
                    }
                    else if (version == CalibrationFileVersion.CALIBRATION_FILE_VERSION_7)
                    {
                        foreach (RssiCalibrationDataVer7 calData in rssiCalibrationData)
                        {
                            string baseName = getChannelAndFreqForRSSI(calData.calibrationFreq);
                            TreeNode baseNode = new TreeNode(baseName);

                            string startFreqName = getChannelAndFreqForRSSI(calData.startFreq);
                            TreeNode startFreqNode = new TreeNode("Start " + startFreqName);
                            string stopFreqName = getChannelAndFreqForRSSI(calData.stopFreq);
                            TreeNode stopFreqNode = new TreeNode("Stop " + stopFreqName);
                            TreeNode chipTempNode = new TreeNode("Chip Temperature=" + calData.chipTemperature.ToString() + " �C");
                            baseNode.Nodes.Add(startFreqNode);
                            baseNode.Nodes.Add(stopFreqNode);
                            baseNode.Nodes.Add(chipTempNode);
                            RssiCalibrationDataVer7.AntennaData[] ants = calData.antennas;
                            for (byte rx_ant = 0; rx_ant < ants.Length; ++rx_ant)
                            {
                                if (!calData.antennaMask.isBitSet(rx_ant))
                                {
                                    continue;
                                }
                                if (ants[rx_ant] == null)
                                {
                                    continue;
                                }
                                int ant1based = rx_ant + 1;
                                TreeNode antNode = new TreeNode("Antenna " + ant1based.ToString());
                                /*LNA Gain*/
                                TreeNode lnaGainNode = new TreeNode("LNA Gain");
                                for (int lnaGainIdx = 0; lnaGainIdx < Defines.maxNumLnaGainStepsVer7; lnaGainIdx++)
                                {
                                    TreeNode lnaGainValueNode = new TreeNode(ants[rx_ant].lnaGains[lnaGainIdx].ToString("f1"));
                                    lnaGainNode.Nodes.Add(lnaGainValueNode);
                                }
                                antNode.Nodes.Add(lnaGainNode);
                                /*Region*/
                                for (int region = 0; region < RssiCalibrationDataVer7.maxNumRegions; region++)
                                {
                                    // Add A+B:
                                    RssiAB point = ants[rx_ant].pointsAB[region];
                                    TreeNode regionNode = new TreeNode("Region " + region.ToString());
                                    string pointStr = "A=" + point.a.ToString("f2") + ", B=" + point.b.ToString("f2");

                                    // Add S2Ds:
                                    string s2dStr = "S2D Gain=" + ants[rx_ant].s2dGainOffset[region].gain.ToString() + ", S2D Offset=" + ants[rx_ant].s2dGainOffset[region].offset.ToString();
                                    regionNode.Nodes.Add(s2dStr);
                                    regionNode.Nodes.Add(pointStr);
                                    antNode.Nodes.Add(regionNode);
                                }
                                /*RF Flatness*/
                                TreeNode RFFlatnessNode = new TreeNode("RF Flatness");
                                for (int RFfreqPointIdx = 0; RFfreqPointIdx < calData.numRxRfFlatnessPoints; RFfreqPointIdx++)
                                {
                                    string frequency = calData.rxRfFlatnessFreqs[RFfreqPointIdx].ToString() + " MHz";
                                    TreeNode RFFreqPointNode = new TreeNode("Freq Point[" + RFfreqPointIdx.ToString() + "]=" + frequency);
                                    TreeNode PerGainValueNode = new TreeNode("RXHighGainDelta=" + ants[rx_ant].rxRfFlatnessDeltaPointsHighGain[RFfreqPointIdx].ToString() +
                                        ";RXLowGainDelta=" + ants[rx_ant].rxRfFlatnessDeltaPointsLowGain[RFfreqPointIdx].ToString() +
                                        ";Bypass=" + ants[rx_ant].rxRfFlatnessDeltaPointsBypass[RFfreqPointIdx].ToString());
                                    RFFreqPointNode.Nodes.Add(PerGainValueNode);

                                    RFFlatnessNode.Nodes.Add(RFFreqPointNode);
                                }
                                if (0 == calData.numRxRfFlatnessPoints)
                                {
                                    TreeNode RFNullNode = new TreeNode("Null");
                                    RFFlatnessNode.Nodes.Add(RFNullNode);
                                }
                                antNode.Nodes.Add(RFFlatnessNode);

                                /*RSSI Flatness*/
                                TreeNode RSSIFlatnessNode = new TreeNode("RSSI Flatness");
                                for (int RSSIFreqPointIdx = 0; RSSIFreqPointIdx < calData.numRxRssiFlatnessPoints; RSSIFreqPointIdx++)
                                {
                                    string frequency = calData.rxRssiFlatnessFreqs[RSSIFreqPointIdx].ToString() + " MHz";
                                    TreeNode RSSIFreqPointNode = new TreeNode("Freq Point[" + RSSIFreqPointIdx.ToString() + "]=" + frequency);
                                    TreeNode RSSIDeltaNode = new TreeNode("RSSI Delta=" + ants[rx_ant].rxRssiFlatnessDeltaPoints[RSSIFreqPointIdx].ToString());
                                    RSSIFreqPointNode.Nodes.Add(RSSIDeltaNode);

                                    RSSIFlatnessNode.Nodes.Add(RSSIFreqPointNode);
                                }
                                if (0 == calData.numRxRssiFlatnessPoints)
                                {
                                    TreeNode RSSINullNode = new TreeNode("Null");
                                    RSSIFlatnessNode.Nodes.Add(RSSINullNode);
                                }
                                antNode.Nodes.Add(RSSIFlatnessNode);
                                /*Sub Band Cross Points*/
                                TreeNode subBandCP = new TreeNode("Sub Bands Crossing Points");
                                for (int subBandIdx = 0; subBandIdx < Defines.maxNumLnaSubBandsCrossingPointsVer7; subBandIdx++)
                                {
                                    string frequency = ants[rx_ant].subBandsCrossingPoints[subBandIdx].ToString() + " MHz";
                                    TreeNode subBandValueNode = new TreeNode(frequency);
                                    subBandCP.Nodes.Add(subBandValueNode);
                                }
                                antNode.Nodes.Add(subBandCP);

                                baseNode.Nodes.Add(antNode);
                            }
                            RSSInode.Nodes.Add(baseNode);
                        }
                    }

                    tree_tpc_tpc.Nodes.Add(RSSInode);
                    WriteLine("ReadCalibrationData, Loading RX data end");
                }
                else
                {
                    WriteLine("Unexpected calibration file version " + version.ToString());
                }
            }

            CollapseTPC();

            if (tree_tpc_tpc.Nodes.Count == 0)
            {
                WriteToConsole("Read calibration data failed! ", Color.DarkViolet);
            }
        }

        private bool ReadCalibrationData()
        {
            bool retVal = false;

            try
            {
                switch (DUT.EEPROMversion)
                {
                    case 6:
                    case 7:
                        ReadCalibrationDataW600_W700();
                        retVal = true;
                        break;
                }
            }
            catch (Exception ex)
            {
                WriteToConsole(ex.Message + ", Read calibration data failed! ", Color.DarkViolet);
            }

            return retVal;
        }

        private void CollapseTPC()
        {
            tree_tpc_tpc.CollapseAll();
            foreach (TreeNode node in tree_tpc_tpc.Nodes) node.Expand();
        }

        private void TPC_CollapseAll_Click(object sender, EventArgs e)
        {
            CollapseTPC();
        }

        private void TPC_ExpandAll_Click(object sender, EventArgs e)
        {

            tree_tpc_tpc.ExpandAll();
        }

        private void burnInfoBtn_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                if (DUT.ParseFile(txtBox_nvMemCtrl_eepromFile.Text))
                {
                    getMemCfg(GUI_GetSelectedValue(comboBox_basicOp_memoryType), out NvMemoryType memoryType, out NvMemorySize memorySize);
                    if (ReadCalibrationData())
                    {
                        DUT.BurnFile(memoryType, memorySize);
                    }
                }
            }
        }

        private void burnBarcodeAndMAC_Click(object sender, EventArgs e)
        {
            byte[] _macBytes = convertMACaddrToBytes(txtBox_nvMemCtrl_MACaddr.Text);
            if (_macBytes.Length > 0)
            {
                using (new WaitCursorBlock(this))
                {
                    if (DUT.GetEEPROMInfo(out byte countryCode, out byte[] macAddress, out byte[] serialNumber, out byte week, out byte year))
                    {
                        byte[] _serialNumber;

                        string barcode = txtBox_nvMemCtrl_barcode.Text;

                        // 1: New barcode format, 0: Original barcode format
                        int barcodeMode = check_nvMemCtrl_SNformatBarcode.Checked ? 1 : 0;
                        if (barcodeMode == 1)
                        {
                            const int BARCODE_MODE_1_WEEK_OFFSET = 0;
                            const int BARCODE_MODE_1_YEAR_OFFSET = 1;
                            const int BARCODE_MODE_1_SN_OFFSET = 2;

                            // Update SN and date "as-is" in the barcode string
                            // Country code is not changed
                            _serialNumber = getSnBytes(barcode, BARCODE_MODE_1_SN_OFFSET, barcode.Length);
                            week = (byte)barcode[BARCODE_MODE_1_WEEK_OFFSET];
                            year = (byte)barcode[BARCODE_MODE_1_YEAR_OFFSET];
                        }
                        else
                        {
                            int yearIndex, weekIndex, manufacturerIndex;
                            int SNstart, SNend;
                            int countryIndex = -1;
                            int dateBase = 10; // by default, there is week with 2-digits. in 15-digits barcode, there's month in hex

                            switch (barcode.Length)
                            {
                                case 7:
                                    //Special case when given is serial number only.
                                    //Year and week to be obtained from current date
                                    yearIndex = -1;
                                    weekIndex = -1;
                                    manufacturerIndex = 0;
                                    SNstart = 2;
                                    break;

                                case 9:
                                    //Special case when given is serial number and country
                                    //code only. Year and week to be obtained from current date
                                    yearIndex = -1;
                                    weekIndex = -1;
                                    manufacturerIndex = 0;
                                    SNstart = 2;
                                    countryIndex = 7;
                                    break;

                                case 10:
                                    yearIndex = 0;
                                    weekIndex = 1;
                                    manufacturerIndex = 3;
                                    SNstart = 5;
                                    break;

                                case 11:
                                    yearIndex = 0;
                                    weekIndex = 2;
                                    manufacturerIndex = 4;
                                    SNstart = 6;
                                    break;

                                case 13:
                                    yearIndex = 0;
                                    weekIndex = 2;
                                    manufacturerIndex = 4;
                                    SNstart = 6;
                                    countryIndex = 11;
                                    break;

                                case 15:
                                    yearIndex = 5;
                                    weekIndex = 6;
                                    manufacturerIndex = 7;
                                    SNstart = 9;
                                    dateBase = 16; // Date is provided in hex for this case
                                    break;

                                default:
                                    throw new ApplicationException("Barcode is invalid (wrong barcode length).");
                            }

                            // Update Country Code - Check if barcode includes a country code
                            if (countryIndex == -1)
                            {
                                SNend = barcode.Length; // No country code, serial number ends at the end of barcode
                            }
                            else
                            {
                                SNend = countryIndex; // country code exists, serial number ends where country code begins
                                                      // Country code is the last part of the barcode starting from countryIndex position 
                                countryCode = Country.getCountryByName(barcode.Substring(countryIndex)).code;
                            }

                            // Update serial number
                            _serialNumber = getSnBytes(barcode, SNstart, SNend);

                            // Update Year - Check if barcode includes the year
                            int _year;
                            if (yearIndex == -1)
                            {
                                _year = DateTime.Now.Year; // Year not included, use current year
                            }
                            else
                            {
                                _year = Convert.ToInt32(barcode.Substring(yearIndex, weekIndex - yearIndex), dateBase); // Year included, convert year substring to int
                            }
                            year = (byte)(_year % 100);

                            // Update Week - Check if barcode includes the year
                            if (weekIndex == -1)
                            {
                                week = (byte)(DateTime.Now.DayOfYear / 7); // Week not included, use current week calculated by day number since Jan 1st divided by 7 (days a week)
                            }
                            else
                            {
                                week = (byte)Convert.ToInt32(barcode.Substring(weekIndex, manufacturerIndex - weekIndex), dateBase); // Week included, convert week substring to int
                            }
                        }

                        getMemCfg(GUI_GetSelectedValue(comboBox_basicOp_memoryType), out NvMemoryType memoryType, out NvMemorySize memorySize);

                        DUT.BurnInfo(countryCode, _macBytes, _serialNumber, week, year, memoryType, memorySize);
                    }
                }
            }
        }

        private void stopCW_Click(object sender, EventArgs e)
        {
            if (!isCwTransmission)
            {
                return;
            }

            using (new WaitCursorBlock(this))
            {
                if (DUT.StopCW())
                {
                    isCwTransmission = false;
                    cmd_basicOp_transmitCW.Enabled = true;
                    cmd_basicOp_stopCW.ForeColor = Color.Black;
                    cmd_basicOp_startTransmitting.Enabled = true;
                    cmd_basicOp_stopTransmission.Enabled = true;
                    cmd_basicOp_startSpaceless.Enabled = true;
                    cmd_basicOp_stopSpaceless.Enabled = true;
                    cmd_enableAll(true); //merav
                }
            }
        }

        private void wlan0card_CheckedChanged(object sender, EventArgs e)
        {
            ConnectButton.ForeColor = Color.Red;
        }

        private void wlan2card_CheckedChanged(object sender, EventArgs e)
        {
            ConnectButton.ForeColor = Color.Red;
        }

        private void wlan4card_CheckedChanged(object sender, EventArgs e)
        {
            ConnectButton.ForeColor = Color.Red;
        }

        private void burnCountryCode_Click(object sender, EventArgs e)
        {
            if (combox_nvMemCtrl_country.SelectedItem == null)
            {
                MessageDialog.ShowWarning("Please choose a valid country from the list");
                return;
            }

            CountryInfo countryInfo = (CountryInfo)combox_nvMemCtrl_country.SelectedItem;
            getMemCfg(GUI_GetSelectedValue(comboBox_basicOp_memoryType), out NvMemoryType memoryType, out NvMemorySize memorySize);
            using (new WaitCursorBlock(this))
            {
                DUT.BurnCountryCode(countryInfo.code, memoryType, memorySize);
            }
        }

        private void cmd_enableAll(bool status)
        {
            panel_basicOp_rate_cmd.Enabled = status;
            panel_powerParam_cmd.Enabled = status;
            panel_spacing_cmd.Enabled = status;
            panel_loop_cmd.Enabled = status;
            panel_basicOp_channel_cmd.Enabled = status;
            groupBox_WlanCard.Enabled = status;
        }

        private void ResizeBFTabControlToFitContent()
        {
            if (bf_tabControl.SelectedTab != null)
            {
                // Get the height of the selected tab's content
                Control content = bf_tabControl.SelectedTab.Controls[0];
                bf_tabControl.Height = content.Height + bf_tabControl.ItemSize.Height + 10; // Add padding
            }
        }

        private void tabControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (DUT.IsInitialized)
            {
                if (tabControl.SelectedIndex == 4)
                {
                    ReadCalibrationData();
                }
                else if (tabControl.SelectedIndex == 1)
                {
                    this.check_extendedOp_xtalEnable.Checked = true;
                    xtal_enable_CheckedChanged(null, null);
                    read_xtal_Click(null, null);

                    // Update bf_tabControl based on operation type
                    Bandwidth bandwidth = GUI_GetSelectedSignalBandwidth();
                    if (bandwidth == Bandwidth.BANDWIDTH_ONE_HUNDRED_SIXTY)
                    {
                        // EHT 160 operation
                        bf_tabControl.SelectedIndex = 2; // bf_tab_eht160
                    }
                    else if (bandwidth == Bandwidth.BANDWIDTH_THREE_HUNDRED_TWENTY)
                    {
                        // EHT 320 operation
                        bf_tabControl.SelectedIndex = 3; // bf_tab_eht320
                    }
                    else
                    {
                        // Standard operation
                        bf_tabControl.SelectedIndex = 1; // bf_tab_standard
                    }
                    ResizeBFTabControlToFitContent();
                }
                else if (tabControl.SelectedIndex == 2)
                {
                    if (DUT.GetComponentVersion(VersionedComponent.VERSIONED_COMPONENT_CV, out string combinedVersion))
                    {
                        txtBox_nvMemCtrl_CVVersion.Text = combinedVersion;
                    }
                }
            }
        }

        private void tabControl_Unclickable(object sender, TabControlCancelEventArgs e)
        {
            if (!((Control)e.TabPage).Enabled) /* if the tab is set to disabled, don't allow clicks */
            {
                e.Cancel = true;
            }
        }

        private void tabControl_DrawGray(object sender, DrawItemEventArgs e)
        {
            int offset_x, offset_y;
            TabPage tab = tabControl.TabPages[e.Index];
            Rectangle paddedBounds = e.Bounds;

            offset_x = (e.State == DrawItemState.Selected) ? -2 : 1;
            offset_y = (e.State == DrawItemState.Selected) ? -2 : 1;

            paddedBounds.Offset(offset_x, offset_y); // makes text pop if selected along x and y axis

            if (tab.Enabled)
            {
                TextRenderer.DrawText(e.Graphics, tab.Text, Font, paddedBounds, Color.Black);
            }
            else
            {
                TextRenderer.DrawText(e.Graphics, tab.Text, Font, paddedBounds, Color.DarkGray);
            }
        }

        private void send_xtal_Click(object sender, EventArgs e)
        {
            ushort value;
            try
            {
                value = getXtal();
            }
            catch (FormatException)
            {
                MessageDialog.ShowWarning("Error parsing XTAL Value: not an hex value");
                return;
            }

            using (new WaitCursorBlock(this))
            {
                DUT.WriteXtal(value);
            }
        }

        private void read_xtal_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                if (DUT.ReadXtal(out ushort data))
                {
                    setXtal(data);
                }
            }
        }

        private void xtal_enable_CheckedChanged(object sender, EventArgs e)
        {
            if (check_extendedOp_xtalEnable.Checked)
            {
                num_extendedOp_xtalCalValue.Enabled = true;
                num_extendedOp_xtalRegisterValue.Enabled = true;
                cmd_extendedOp_xtalRegisterWrite.Enabled = true;
                cmd_extendedOp_xtalRegisterRead.Enabled = true;
                cmd_extendedOp_xtalCalReadEEPROM.Enabled = true;
                cmd_extendedOp_xtalCalWriteEEPROM.Enabled = true;
                lbl_extendedOp_xtalRegAutoUpdate.Enabled = true;
                num_extendedOp_xtalRegisterBias.Enabled = true;
                num_extendedOp_xtalCalValue.Enabled = true;
            }
            else
            {
                num_extendedOp_xtalCalValue.Enabled = false;
                num_extendedOp_xtalRegisterValue.Enabled = false;
                cmd_extendedOp_xtalRegisterWrite.Enabled = false;
                cmd_extendedOp_xtalRegisterRead.Enabled = false;
                cmd_extendedOp_xtalCalReadEEPROM.Enabled = false;
                cmd_extendedOp_xtalCalWriteEEPROM.Enabled = false;
                lbl_extendedOp_xtalRegAutoUpdate.Enabled = false;
                num_extendedOp_xtalRegisterBias.Enabled = false;
                num_extendedOp_xtalCalValue.Enabled = false;
            }
        }

        private ushort getXtal()
        {
            return (ushort)(num_extendedOp_xtalRegisterValue.Value + ((ushort)num_extendedOp_xtalRegisterBias.Value << 9));
        }

        private void setXtal(uint data)
        {
            num_extendedOp_xtalRegisterValue.Value = data & 0x1FF;
            num_extendedOp_xtalCalBias.Value = num_extendedOp_xtalRegisterBias.Value = (data >> 9) & 0x7F;
        }

        private void xtal_write_eeprom_Click(object sender, EventArgs e)
        {
            ushort value;
            try
            {
                value = getXtal();
            }
            catch (FormatException)
            {
                MessageDialog.ShowWarning("Error parsing XTAL Value: not an hex value");
                return;
            }

            getMemCfg(GUI_GetSelectedValue(comboBox_basicOp_memoryType), out NvMemoryType memoryType, out NvMemorySize memorySize);
            using (new WaitCursorBlock(this))
            {
                DUT.WriteXtalCalDataToEEPROM(value, memoryType, memorySize);
            }
        }


        private void xtal_read_eeprom_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                if (DUT.ReadXtalCalDataFromEEPROM(out ushort data))
                {
                    setXtal(data);
                }
            }
        }

        private void dut_xtal_value_changed(int new_value)
        {
            if (num_extendedOp_xtalRegisterValue.Value != new_value)
            {
                num_extendedOp_xtalRegisterValue.Value = new_value;
            }

            if ((num_extendedOp_xtalCalValue.Value != new_value) && (num_extendedOp_xtalCalValue.Value != (512 - new_value) * -1))
            {
                if (new_value > 255) num_extendedOp_xtalCalValue.Value = (512 - new_value) * -1;
                else num_extendedOp_xtalCalValue.Value = new_value;
            }
        }

        private void xtal_value_dec_ValueChanged(object sender, EventArgs e)
        {
            int new_value = (int)num_extendedOp_xtalCalValue.Value;

            if (new_value < 0) new_value = 512 + new_value;
            dut_xtal_value_changed(new_value);
            if (lbl_extendedOp_xtalRegAutoUpdate.Checked)
                send_xtal_Click(null, null);
        }

        private void xtal_value_hex_ValueChanged(object sender, EventArgs e)
        {
            dut_xtal_value_changed((int)num_extendedOp_xtalRegisterValue.Value);
        }

        private void xtal_reg_bias_ValueChanged(object sender, EventArgs e)
        {
            if (num_extendedOp_xtalCalBias.Value == num_extendedOp_xtalRegisterBias.Value)
                return;
            num_extendedOp_xtalCalBias.Value = num_extendedOp_xtalRegisterBias.Value;
            if (lbl_extendedOp_xtalRegAutoUpdate.Checked)
                send_xtal_Click(null, null);
        }

        private void xtal_bias_dec_ValueChanged(object sender, EventArgs e)
        {
            if (num_extendedOp_xtalCalBias.Value == num_extendedOp_xtalRegisterBias.Value)
                return;
            num_extendedOp_xtalRegisterBias.Value = num_extendedOp_xtalCalBias.Value;
            if (lbl_extendedOp_xtalRegAutoUpdate.Checked)
                send_xtal_Click(null, null);
        }

        private void IFS_Validating(object sender, CancelEventArgs e)
        {
            try
            {
                uint ifs = Convert.ToUInt32(txtBox_basicOp_spacingIFS.Text);
                if (ifs >= MINIMUM_IFS)
                {
                    return; // all is OK
                }
            }
            catch (Exception)
            {

            }
            txtBox_basicOp_spacingIFS.Text = MINIMUM_IFS.ToString();
        }

        private void PrintingLevel_SelectedIndexChanged(object sender, EventArgs e)
        {
            logger.setLogLevel((LogLevel)GUI_GetSelectedValue(combox_nvMemCtrl_printLevel));
        }

        private void ParseFile_Click(object sender, EventArgs e)
        {
            String title = openEEPROMfile.Title;
            openEEPROMfile.Title = "Select external EEPROM file to parse";
            openEEPROMfile.FileName = txtBox_nvMemCtrl_eepromFile.Text;
            DialogResult ret = openEEPROMfile.ShowDialog();
            openEEPROMfile.Title = title;
            if (ret == DialogResult.OK)
            {
                using (new WaitCursorBlock(this))
                {
                    if (DUT.ParseFile(openEEPROMfile.FileName))
                    {
                        ReadCalibrationData();
                        UpdateCurEEPROMInfo_Click(null, null);
                    }
                }
            }
        }

        private void DUT_GUI_Load(object sender, EventArgs e)
        {
        }

        private void RxEvmInputsBtn_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                double rxEvmDbm;
                bool gen5 = false;

                if (DUT.ReadRxEvm(out byte[] rxEvm))
                {
                    gen5 = isGen5Hardware();
                    for (int antIndex = 0; antIndex < rxEvm.Length; antIndex++)
                    {
                        if (gen5)
                        {
                            rxEvmDbm = rxEvm[antIndex] * 0.376; // rxEvm*10*log2/8

                        }
                        else
                        {
                            rxEvmDbm = rxEvm[antIndex] / 2.0;
                        }

                        RxEvmList.Items[(int)InputsListEvm.EVM_RX0 + antIndex].SubItems[1].Text = Convert.ToString(rxEvmDbm);
                    }
                }
            }
        }

        private void setPower_Click(object sender, EventArgs e)
        {
            bool closedLoop = checkBox_basicOp_closedLoop.Checked;
            byte powerLimit = 0xff;
            byte powerLevel = 0xDD;

            if (checkBox_basicOp_autoPower.Checked)
            {
                int tmpPowerLimit = ParseControl(txtBox_basicOp_powerLimit);
                if (tmpPowerLimit > 0xff)
                {
                    MessageDialog.ShowWarning("Invalid power limit!\nPlease specify a valid TX power limit in range [0..255].");
                    return;
                }

                powerLimit = (byte)tmpPowerLimit;
            }
            else
            {
                // Limits are different depending on if OpenLoop is checked or not
                // If open-loop, then power level is an index, if closed-loop, power level value is in 0.5 dBm units
                float upperLimit = checkBox_basicOp_closedLoop.Checked ? Defines.maxPowerLevelValue : Defines.maxPowerLevelIndex;
                float lowerLimit = checkBox_basicOp_closedLoop.Checked ? Defines.minPowerLevelValue : Defines.minPowerLevelIndex;

                try
                {
                    float tmpPowerLevel = float.Parse(txtBox_basicOp_powerLevel.Text);

                    // validate and bound values
                    if (tmpPowerLevel < lowerLimit) throw new ApplicationException("Invalid power level!, power level should be >= " + lowerLimit.ToString());
                    if (tmpPowerLevel > upperLimit) throw new ApplicationException("Invalid power level!, power level should be <= " + upperLimit.ToString());

                    // if in closed-loop, convert dBm to half-dBm (every index in 0.5 dBm)
                    if (checkBox_basicOp_closedLoop.Checked)
                    {
                        tmpPowerLevel = 2 * (int)tmpPowerLevel;
                    }

                    powerLevel = (byte)tmpPowerLevel;
                }
                catch (Exception ex)
                {
                    MessageDialog.ShowWarning(ex.Message + "\nPlease specify a valid TX power level.");
                    txtBox_basicOp_powerLevel.Text = "0";
                    return;
                }
            }

            using (new WaitCursorBlock(this))
            {
                if (DUT.SetTransmitPowerControl(closedLoop, powerLimit) && DUT.SetTransmitPowerLevel(powerLevel))
                {
                    GUI_setColor_cmd(cmd_basicOp_setPower, Color.Black);
                }
            }
        }

        private void checkBox_basicOp_closedLoop_CheckedChanged(object sender, EventArgs e)
        {
            string openLoopRange = $"Power Level Index ({Defines.minPowerLevelIndex} to {Defines.maxPowerLevelIndex}):";
            string closedLoopRange = $"Power Level Value dBm({Defines.minPowerLevelValue} to {Defines.maxPowerLevelValue}):";
            lbl_basicOp_powerLevel.Text = checkBox_basicOp_closedLoop.Checked ? closedLoopRange : openLoopRange;
            txtBox_basicOp_powerLevel.Text = "0";
            GUI_setColor_cmd(cmd_basicOp_setPower, Color.Red);
        }

        static private void getMemCfg(int memTypeValue, out NvMemoryType memoryType, out NvMemorySize memorySize)
        {
            switch (memTypeValue)
            {
                case (int)idx_memType_e.idx_eeprom_1k: //eeprom_1k
                    memoryType = NvMemoryType.MEMORY_TYPE_EEPROM;
                    memorySize = NvMemorySize.MEMORY_SIZE_BYTES_1K;
                    break;
                case (int)idx_memType_e.idx_eeprom_2k: //eeprom_2k
                    memoryType = NvMemoryType.MEMORY_TYPE_EEPROM;
                    memorySize = NvMemorySize.MEMORY_SIZE_BYTES_2K;
                    break;
                case (int)idx_memType_e.idx_eeprom_3k: //eeprom_3k
                    memoryType = NvMemoryType.MEMORY_TYPE_EEPROM;
                    memorySize = NvMemorySize.MEMORY_SIZE_BYTES_3K;
                    break;
                default:
                    memoryType = NvMemoryType.MEMORY_TYPE_FLASH;
                    memorySize = NvMemorySize.MEMORY_SIZE_BYTES_3K;
                    break;
            }
        }

        static private string getBandStr(Band band)
        {
            if (band == Band.BAND_2400MHZ)
                return "2.4 GHz";
            if (band == Band.BAND_5000MHZ)
                return "5 GHz";
            if (band == Band.BAND_6000MHZ)
                return "6 GHz";
            else return "undef";
        }

        static private string getSupportedBandsStr(Band[] supportedBands)
        {
            StringBuilder sb = new StringBuilder();
            foreach (Band band in supportedBands)
            {
                if (sb.Length > 0)
                {
                    sb.Append("+");
                }
                if (band == Band.BAND_2400MHZ)
                    sb.Append("2.4");
                if (band == Band.BAND_5000MHZ)
                    sb.Append("5");
                if (band == Band.BAND_6000MHZ)
                    sb.Append("6");
            }
            sb.Append(" GHz");

            return sb.ToString();
        }

        static private string GetHardwareType(HardwareType hardwareType)
        {
            switch (hardwareType)
            {
                case HardwareType.HARDWARE_TYPE_GEN2_PCI:
                    return "2 PCI";
                case HardwareType.HARDWARE_TYPE_GEN3_PCI:
                    return "3 PCI";
                case HardwareType.HARDWARE_TYPE_GEN3_PCIE:
                    return "3 PCI Express";
                case HardwareType.HARDWARE_TYPE_GEN4:
                    return "AR10";
                case HardwareType.HARDWARE_TYPE_GEN5:
                    return "Wav500";
                case HardwareType.HARDWARE_TYPE_GEN6:
                    return "Wav600";
                case HardwareType.HARDWARE_TYPE_GEN7:
                    return "Wav700";
                case HardwareType.HARDWARE_TYPE_INVALID:
                    return "";
                default:
                    throw new ApplicationException("The hardware type is unknown");
            }
        }

        private void CardIdUpdate()
        {
            txtBox_basicOp_wav.Text = GetHardwareType(DUT.HardwareType);
            if (DUT.GetMaxTxAntennasMask(out AntennaMask maxTxAntennaMask))
            {
                txtBox_basicOp_txAntennaMask.Text = Convert.ToString(maxTxAntennaMask.Value, 2) + "b";
            }
            if (DUT.GetMaxRxAntennasMask(out AntennaMask maxRxAntennaMask))
            {
                txtBox_basicOp_rxAntennaMask.Text = Convert.ToString(maxRxAntennaMask.Value, 2) + "b";
            }
            txtBox_basicOp_bandSupport.Text = getSupportedBandsStr(DUT.RfSupportedBands);
            txtBox_basicOp_bandCurrent.Text = getBandStr(DUT.RfBandCurrent);
        }

        public void updateAnts()
        {
            if (DUT.GetTxAntennasMask(out AntennaMask txAntennaMask) && DUT.GetMaxTxAntennasMask(out AntennaMask maxTxAntennaMask))
            {
                for (byte i = 0; i < m_txAntButtons.Length; ++i)
                {
                    bool isAvailable = maxTxAntennaMask.isBitSet(i);
                    bool isOn = isAvailable && txAntennaMask.isBitSet(i);

                    var button = m_txAntButtons[i];
                    button.Enabled = isAvailable;

                    SetAntButtonColor(button, isOn);
                    DUT.SetTXantLocal(i, isOn);
                }
            }

            if (DUT.GetRxAntennasMask(out AntennaMask rxAntennaMask) && DUT.GetMaxRxAntennasMask(out AntennaMask maxRxAntennaMask))
            {
                for (byte i = 0; i < m_rxAntButtons.Length; ++i)
                {
                    bool isAvailable = maxRxAntennaMask.isBitSet(i);
                    bool isOn = isAvailable && rxAntennaMask.isBitSet(i);

                    var button = m_rxAntButtons[i];
                    button.Enabled = isAvailable;

                    SetAntButtonColor(button, isOn);
                    DUT.SetRXantLocal(i, isOn);
                }
            }

            DUT.UpdateAnts(false);
        }

        /*Make phyType/channel/....aligned in case the band be changed not by user*/
        private void align_phyTypeAdjVals_to_band()
        {
            if (!GUI_basicOp_PhyTypeChange_check())
            {//Have to set phytype to be default since band change
                Band selectedBand = GUI_GetSelectedBand();
                if (selectedBand == Band.BAND_2400MHZ)
                {
                    combox_basicOp_phyType.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_phyType, (int)PhyMode.PHY_MODE_N_2_4);
                }
                else if (selectedBand == Band.BAND_5000MHZ)
                {
                    combox_basicOp_phyType.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_phyType, (int)PhyMode.PHY_MODE_N_5);
                }
                else if (selectedBand == Band.BAND_6000MHZ)
                {
                    combox_basicOp_phyType.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_phyType, (int)PhyMode.PHY_MODE_AX);
                }
                else
                {
                    WriteToConsole("Error in phyType: must be one of 2.4G/5G/6G \r\n", Color.Red);
                }
            }

            GUI_basicOp_loadPhyTypeAdjVals();

        }

        private void cmd_basicOp_startSpaceless_Click(object sender, EventArgs e)
        {
            if (!transmitCheckConditions())
            {
                return;
            }

            using (new WaitCursorBlock(this))
            {
                ushort repetitions;
                uint packetLength = (uint)ParseControl(txtBox_basicOp_packetLen);
                bool longData = check_basicOp_txLongData.Checked;
                bool beamforming = check_basicOp_txBeamforming.Checked;
                bool closedLoop = checkBox_basicOp_closedLoop.Checked;
                CodingType codingType = GUI_GetSelectedCodingType();

                // Transmit 3 packets with close loop, with the parameters in the GUI -to allow
                // for power settling.
                bool ok = DUT.SetTransmitPowerControl(true, 0xff);
                ok = ok && DUT.StartTxPackets(3, packetLength, longData, beamforming, codingType);
                ok = ok && DUT.StopTxPackets();
                ok = ok && DUT.SetTransmitPowerControl(closedLoop, 0xff);

                // Set spaceless transmission mode
                ok = ok && DUT.SetSpacelessTransmission(true);

                PhyMode phyMode = GUI_GetSelectedPhyMode();
                if (phyMode == PhyMode.PHY_MODE_B)
                {
                    // For 11B, transmit endless packet(max repetition), with the parameters from
                    // the GUI, with packet length max (4000), reduce SIFS to minimum
                    uint ifs = MINIMUM_IFS;
                    ok = ok && DUT.SetIFS(ifs);
                    if (ok)
                    {
                        txtBox_basicOp_spacingIFS.Text = ifs.ToString();
                    }
                    repetitions = 0xffff;
                    packetLength = 4000;
                    ok = ok && DUT.StartTxPackets(repetitions, packetLength, longData, beamforming, codingType);
                    if (ok)
                    {
                        txtBox_basicOp_repetitions.Text = "0x" + repetitions.ToString("x4");
                        txtBox_basicOp_packetLen.Text = packetLength.ToString();
                    }
                }
                else
                {
                    // For OFDM, transmit 1 spaceless packet, with the parameters from the GUI,
                    // including packet length.
                    repetitions = 1;
                    ok = ok && DUT.StartTxPackets(repetitions, packetLength, longData, beamforming, codingType);
                    if (ok)
                    {
                        txtBox_basicOp_repetitions.Text = repetitions.ToString();
                    }
                }

                if (ok)
                {
                    isSpacelessTransmission = true;
                    cmd_basicOp_startSpaceless.Enabled = false;
                    cmd_basicOp_stopSpaceless.Enabled = true;
                    GUI_setColor_cmd(cmd_basicOp_stopSpaceless, Color.Red);
                    cmd_basicOp_startTransmitting.Enabled = false;
                    cmd_basicOp_stopTransmission.Enabled = false;
                    cmd_basicOp_transmitCW.Enabled = false;
                    cmd_basicOp_stopCW.Enabled = false;
                    cmd_enableAll(false); //merav
                }
            }
        }

        private void cmd_basicOp_stopSpaceless_Click(object sender, EventArgs e)
        {
            if (!isSpacelessTransmission)
            {
                return;
            }

            using (new WaitCursorBlock(this))
            {
                if (DUT.StopTxPackets() && DUT.SetSpacelessTransmission(false))
                {
                    isSpacelessTransmission = false;
                    cmd_basicOp_startSpaceless.Enabled = true;
                    GUI_setColor_cmd(cmd_basicOp_stopSpaceless, Color.Black);
                    cmd_basicOp_startTransmitting.Enabled = true;
                    cmd_basicOp_stopTransmission.Enabled = true;
                    cmd_basicOp_transmitCW.Enabled = true;
                    cmd_basicOp_stopCW.Enabled = true;
                    cmd_enableAll(true); //merav
                }
            }
        }

        private void comboBox_basicOp_memoryType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (DUT.IsInitialized)
            {
                WriteLine("Memory config change! Please Reconnect!!");
            }

            ConnectButton.ForeColor = Color.Red;
            /*disable some control*/

            extendedOperationTab.Enabled = false;
            if (tabControl.SelectedIndex != 2)
            {
                nvMemControlTab.Enabled = false;
            }

            groupBox_tx.Enabled = false;
            groupBox_genRisc.Enabled = false;
            groupBox_antennasOnOff.Enabled = false;
            groupBox_basicOp_misc.Enabled = false;
            panel_powerParam_cmd.Enabled = false;
            panel_loop_cmd.Enabled = false;
            groupBox_basicOp_spaceless.Enabled = false;
            txtBox_basicOp_CalMode.Enabled = false;
        }

        private void btn_basicOp_deleteRegistry_Click(object sender, EventArgs e)
        {
            try
            {
                Registry.CurrentUser.DeleteSubKeyTree(DUTkey);
            }
            catch (Exception ex)
            {
                MessageDialog.ShowError(ex.Message);
            }

            RegistryKey rk = Registry.CurrentUser.OpenSubKey(DUTkey);
            if (rk == null)
            {
                WriteLine("Registry has already been deleted!");
            }
        }

        private void check_basicOp_closeRXants_CheckStateChanged(object sender, EventArgs e)
        {
            GUI_setColor_cmd(cmd_basicOp_setSpacing, Color.Red);
        }

        private void combox_basicOp_phyType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!GUI_basicOp_PhyTypeChange_check())
            {
                MessageDialog.ShowWarning("Selected PHY Mode is not supported for " + ((GUI_BoxItems)combox_basicOp_band.SelectedItem).Name);
                //Have to set phytype to be default
                Band selectedBand = GUI_GetSelectedBand();
                if (selectedBand == Band.BAND_2400MHZ)
                {
                    combox_basicOp_phyType.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_phyType, (int)PhyMode.PHY_MODE_N_2_4);
                }
                else if (selectedBand == Band.BAND_5000MHZ)
                {
                    combox_basicOp_phyType.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_phyType, (int)PhyMode.PHY_MODE_N_5);
                }
                else if (selectedBand == Band.BAND_6000MHZ)
                {
                    combox_basicOp_phyType.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_phyType, (int)PhyMode.PHY_MODE_AX);
                }
                else
                {
                    WriteToConsole("Error in phyType: must be one of 2.4G/5G/6G \r\n", Color.Red);
                }
                return;
            }


            GUI_basicOp_loadPhyTypeAdjVals();
            rateChanged();
        }

        /// <summary>
        /// <para>Clears then loads all values for:</para>
        /// Spectrum BW, Signal BW, MCS, Spatial Stream, GI, LTF, Memory Type, Memory Size
        /// </summary>

        private void GUI_basicOp_loadAllComboxValues()
        {
            // clear all values before loading to avoid duplicates
            combox_basicOp_spectrumBW.Items.Clear();
            combox_basicOp_signalBW.Items.Clear();
            combox_basicOp_MCS.Items.Clear();
            combox_basicOp_spatialStream.Items.Clear();
            combox_basicOp_GI.Items.Clear();
            combox_basicOp_ltf.Items.Clear();

            // fill Combo Boxes
            GUI_fillComboBox(combox_basicOp_spectrumBW);
            GUI_fillComboBox(combox_basicOp_signalBW);
            GUI_fillComboBox(combox_basicOp_regulationType);
            GUI_fillComboBox(combox_basicOp_MCS);
            int antennaMask = 0;
            for (int i = 0; i < Defines.maxNumTxAntennas; i++)
            {
                antennaMask |= 1 << i;
            }
            GUI_fillComboBox(combox_basicOp_spatialStream, antennaMask);
            GUI_fillComboBox(combox_basicOp_GI);
            GUI_fillComboBox(combox_basicOp_ltf);
        }

        /// <summary>
        /// Loads values in ComboBoxes, combinations adjusted depending on selected Phy type.
        /// </summary>
        private bool GUI_basicOp_PhyTypeChange_check()
        {
            PhyMode selectedPhy = GUI_GetSelectedPhyMode();
            Band selectedBand = GUI_GetSelectedBand();
            bool phyType_change_allow = true;

            switch (selectedPhy)
            {
                case PhyMode.PHY_MODE_A: // 802.11a
                    if ((selectedBand == Band.BAND_2400MHZ) || (selectedBand == Band.BAND_6000MHZ))
                    {
                        phyType_change_allow = false;
                    }
                    break;
                case PhyMode.PHY_MODE_B: // 802.11b
                    if ((selectedBand == Band.BAND_5000MHZ) || (selectedBand == Band.BAND_6000MHZ))
                    {
                        phyType_change_allow = false;
                    }
                    break;
                case PhyMode.PHY_MODE_G: // 802.11g
                    if ((selectedBand == Band.BAND_5000MHZ) || (selectedBand == Band.BAND_6000MHZ))
                    {
                        phyType_change_allow = false;
                    }
                    break;
                case PhyMode.PHY_MODE_N_5: // 80211n 5GHz
                    if ((selectedBand == Band.BAND_2400MHZ) || (selectedBand == Band.BAND_6000MHZ))
                    {
                        phyType_change_allow = false;
                    }
                    break;
                case PhyMode.PHY_MODE_N_2_4: // 802.11n 2.4GHz
                    if ((selectedBand == Band.BAND_5000MHZ) || (selectedBand == Band.BAND_6000MHZ))
                    {
                        phyType_change_allow = false;
                    }
                    break;
                case PhyMode.PHY_MODE_AC: // 802.11ac
                    if (selectedBand == Band.BAND_6000MHZ)
                    {
                        phyType_change_allow = false;
                    }
                    break;
            }

            return phyType_change_allow;
        }

        private void GUI_basicOp_loadPhyTypeAdjVals()
        {
            GUI_basicOp_loadAllComboxValues(); // load all combox values to avoid IndexOutOfBounds
            PhyMode selectedPhy = GUI_GetSelectedPhyMode();

            UpdateChannelsList();
            UpdateNumOfdmSymbolsCheckBox();

            switch (selectedPhy)
            {
                case PhyMode.PHY_MODE_A: // 802.11a
                    {
                        // Spectrum Bandwidth
                        GUI_removeMultipleBoxItems(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_TWENTY);
                        combox_basicOp_spectrumBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_TWENTY);


                        // Signal Bandwidth
                        GUI_removeMultipleBoxItems(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);
                        combox_basicOp_signalBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);

                        // Set Channel
                        combox_basicOp_lowChan.SelectedIndex = GUI_GetChannelAsIndex(combox_basicOp_lowChan, DEFAULT_CHANNEL_5000MHZ);

                        // MCS
                        GUI_removeMultipleBoxItems(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_64qam34);
                        GUI_ReorderBoxItemsAsIndexPrefix(combox_basicOp_MCS);
                        combox_basicOp_MCS.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_qpsk12);


                        // Spatial Stream
                        GUI_removeMultipleBoxItems(combox_basicOp_spatialStream, (int)idx_spatialStream_e.idx_spatialStream1);
                        combox_basicOp_spatialStream.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spatialStream, (int)idx_spatialStream_e.idx_spatialStream1);
                        combox_basicOp_spatialStream.Enabled = false;

                        // GI
                        GUI_removeMultipleBoxItems(combox_basicOp_GI, (int)idx_gi_e.idx_gi08);
                        GUI_removeSingleBoxItem(combox_basicOp_GI, (int)idx_gi_e.idx_gi04);
                        combox_basicOp_GI.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_GI, (int)idx_gi_e.idx_gi08);
                        combox_basicOp_GI.Enabled = false;

                        // LTF
                        GUI_removeMultipleBoxItems(combox_basicOp_ltf, (int)idx_ltf_e.idx_ltfx1);
                        combox_basicOp_ltf.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_ltf, (int)idx_ltf_e.idx_ltfx1);
                        combox_basicOp_ltf.Enabled = false;

                        check_basicOp_txLongData.Checked = false;
                        check_basicOp_txLongData.Enabled = false;
                        check_basicOp_txBeamforming.Checked = false;
                        check_basicOp_txBeamforming.Enabled = false;
                        groupBox_WriteBeamformingMatrix.Enabled = false;

                        // Coding Type
                        radio_basicOp_codingAuto.Checked = true;
                        //  radio_basicOp_codingBCC.Checked = false;
                        radio_basicOp_codingLDPC.Enabled = false;

                        break;
                    }
                case PhyMode.PHY_MODE_B: // 802.11b
                    {
                        // Spectrum Bandwidth
                        GUI_removeMultipleBoxItems(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_TWENTY);
                        combox_basicOp_spectrumBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_TWENTY);

                        // Signal Bandwidth
                        GUI_removeMultipleBoxItems(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);
                        combox_basicOp_signalBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);

                        // Set Channel
                        combox_basicOp_lowChan.SelectedIndex = GUI_GetChannelAsIndex(combox_basicOp_lowChan, DEFAULT_CHANNEL_2400MHZ);

                        //MCS - special values for this type
                        GUI_removeMultipleBoxItems_ST(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_2mbps_short);
                        GUI_ReorderBoxItemsAsIndexPrefix(combox_basicOp_MCS);
                        combox_basicOp_MCS.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_11mbps_short);

                        // Spatial Stream
                        GUI_removeMultipleBoxItems(combox_basicOp_spatialStream, (int)idx_spatialStream_e.idx_spatialStream1);
                        combox_basicOp_spatialStream.Enabled = false;
                        combox_basicOp_spatialStream.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spatialStream, (int)idx_spatialStream_e.idx_spatialStream1);


                        // GI
                        GUI_removeMultipleBoxItems(combox_basicOp_GI, (int)idx_gi_e.idx_gi08);
                        combox_basicOp_GI.Enabled = false;
                        combox_basicOp_GI.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_GI, (int)idx_gi_e.idx_gi08);


                        // LTF
                        combox_basicOp_ltf.Enabled = false;
                        combox_basicOp_ltf.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_ltf, (int)idx_ltf_e.idx_ltfx1);

                        check_basicOp_txLongData.Enabled = true;
                        check_basicOp_txBeamforming.Checked = false;
                        check_basicOp_txBeamforming.Enabled = false;
                        groupBox_WriteBeamformingMatrix.Enabled = false;

                        // Coding Type
                        radio_basicOp_codingAuto.Checked = true;
                        //  radio_basicOp_codingBCC.Checked = false;
                        radio_basicOp_codingLDPC.Enabled = false;

                        break;
                    }
                case PhyMode.PHY_MODE_G: // 802.11g
                    {
                        // Spectrum Bandwidth
                        GUI_removeMultipleBoxItems(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_TWENTY);
                        combox_basicOp_spectrumBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_TWENTY);


                        // Signal Bandwidth
                        GUI_removeMultipleBoxItems(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);
                        combox_basicOp_signalBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);


                        // Set Channel
                        combox_basicOp_lowChan.SelectedIndex = GUI_GetChannelAsIndex(combox_basicOp_lowChan, DEFAULT_CHANNEL_2400MHZ);

                        // MCS
                        GUI_removeMultipleBoxItems(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_64qam34);
                        GUI_ReorderBoxItemsAsIndexPrefix(combox_basicOp_MCS);
                        combox_basicOp_MCS.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_qpsk12);

                        // Spatial Stream
                        GUI_removeMultipleBoxItems(combox_basicOp_spatialStream, (int)idx_spatialStream_e.idx_spatialStream1);
                        combox_basicOp_spatialStream.Enabled = false;
                        combox_basicOp_spatialStream.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spatialStream, (int)idx_spatialStream_e.idx_spatialStream1);


                        // GI
                        GUI_removeMultipleBoxItems(combox_basicOp_GI, (int)idx_gi_e.idx_gi08);
                        combox_basicOp_GI.Enabled = false;
                        combox_basicOp_GI.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_GI, (int)idx_gi_e.idx_gi08);


                        // LTF
                        GUI_removeMultipleBoxItems(combox_basicOp_ltf, (int)idx_ltf_e.idx_ltfx1);
                        combox_basicOp_ltf.Enabled = false;
                        combox_basicOp_ltf.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_ltf, (int)idx_ltf_e.idx_ltfx1);

                        check_basicOp_txLongData.Checked = false;
                        check_basicOp_txLongData.Enabled = false;
                        check_basicOp_txBeamforming.Checked = false;
                        check_basicOp_txBeamforming.Enabled = false;
                        groupBox_WriteBeamformingMatrix.Enabled = false;

                        // Coding Type
                        radio_basicOp_codingAuto.Checked = true;
                        // radio_basicOp_codingBCC.Checked = false;
                        radio_basicOp_codingLDPC.Enabled = false;

                        break;
                    }
                case PhyMode.PHY_MODE_N_5: // 802.11n 5GHz
                    {
                        // Spectrum Bandwidth
                        GUI_removeMultipleBoxItems(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_FOURTY);
                        combox_basicOp_spectrumBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_TWENTY);


                        // Signal Bandwidth
                        GUI_removeMultipleBoxItems(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);
                        combox_basicOp_signalBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);

                        // Set Channel
                        combox_basicOp_lowChan.SelectedIndex = GUI_GetChannelAsIndex(combox_basicOp_lowChan, DEFAULT_CHANNEL_5000MHZ);

                        // MCS
                        GUI_removeMultipleBoxItems(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_64qam56);
                        GUI_removeSingleBoxItem(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_bpsk34);
                        GUI_ReorderBoxItemsAsIndexPrefix(combox_basicOp_MCS);
                        combox_basicOp_MCS.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_qpsk34);

                        // Spatial Stream - set to most commonly used value
                        combox_basicOp_spatialStream.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spatialStream, (int)idx_spatialStream_e.idx_spatialStream1);
                        combox_basicOp_spatialStream.Enabled = true;

                        // GI
                        GUI_removeMultipleBoxItems(combox_basicOp_GI, (int)idx_gi_e.idx_gi04);
                        combox_basicOp_GI.Enabled = true;
                        combox_basicOp_GI.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_GI, (int)idx_gi_e.idx_gi08);

                        // LTF
                        GUI_removeMultipleBoxItems(combox_basicOp_ltf, (int)idx_ltf_e.idx_ltfx1);
                        combox_basicOp_ltf.Enabled = true;
                        combox_basicOp_ltf.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_ltf, (int)idx_ltf_e.idx_ltfx1);

                        check_basicOp_txLongData.Enabled = true;
                        check_basicOp_txBeamforming.Checked = false;
                        check_basicOp_txBeamforming.Enabled = false;
                        groupBox_WriteBeamformingMatrix.Enabled = false;

                        // Coding Type
                        radio_basicOp_codingAuto.Checked = true;
                        //  radio_basicOp_codingBCC.Checked = false;
                        radio_basicOp_codingLDPC.Enabled = true;


                        break;
                    }
                case PhyMode.PHY_MODE_N_2_4: // 802.11n 2.4GHz
                    {
                        // Spectrum Bandwidth
                        GUI_removeMultipleBoxItems(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_FOURTY);
                        combox_basicOp_spectrumBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_TWENTY);

                        // Signal Bandwidth
                        GUI_removeMultipleBoxItems(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);
                        combox_basicOp_signalBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);

                        // Set Channel
                        combox_basicOp_lowChan.SelectedIndex = GUI_GetChannelAsIndex(combox_basicOp_lowChan, DEFAULT_CHANNEL_2400MHZ);

                        // MCS
                        GUI_removeMultipleBoxItems(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_64qam56);
                        GUI_removeSingleBoxItem(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_bpsk34);
                        GUI_ReorderBoxItemsAsIndexPrefix(combox_basicOp_MCS);
                        combox_basicOp_MCS.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_qpsk34);


                        // Spatial Stream - set to most commonly used value
                        combox_basicOp_spatialStream.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spatialStream, (int)idx_spatialStream_e.idx_spatialStream1);
                        combox_basicOp_spatialStream.Enabled = true;

                        // GI
                        GUI_removeMultipleBoxItems(combox_basicOp_GI, (int)idx_gi_e.idx_gi04);
                        combox_basicOp_GI.Enabled = true;
                        combox_basicOp_GI.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_GI, (int)idx_gi_e.idx_gi08);

                        // LTF
                        GUI_removeMultipleBoxItems(combox_basicOp_ltf, (int)idx_ltf_e.idx_ltfx1);
                        combox_basicOp_ltf.Enabled = true;
                        combox_basicOp_ltf.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_ltf, (int)idx_ltf_e.idx_ltfx1);

                        check_basicOp_txLongData.Enabled = true;
                        check_basicOp_txBeamforming.Checked = false;
                        check_basicOp_txBeamforming.Enabled = false;
                        groupBox_WriteBeamformingMatrix.Enabled = false;

                        // Coding Type
                        radio_basicOp_codingAuto.Checked = true;
                        //radio_basicOp_codingBCC.Checked = false;
                        radio_basicOp_codingLDPC.Enabled = true;

                        break;
                    }
                case PhyMode.PHY_MODE_AC: // 802.11ac
                    {
                        // Spectrum Bandwidth
                        GUI_removeMultipleBoxItems(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_ONE_HUNDRED_SIXTY);
                        combox_basicOp_spectrumBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_TWENTY);
                        //
                        // Signal Bandwidth
                        GUI_removeMultipleBoxItems(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);
                        combox_basicOp_signalBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);

                        // Set Channel
                        combox_basicOp_lowChan.SelectedIndex = GUI_GetChannelAsIndex(combox_basicOp_lowChan, DEFAULT_CHANNEL_5000MHZ);

                        // MCS
                        GUI_removeMultipleBoxItems(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_256qam56);
                        GUI_removeSingleBoxItem(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_bpsk34);
                        GUI_ReorderBoxItemsAsIndexPrefix(combox_basicOp_MCS);
                        combox_basicOp_MCS.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_qpsk34);

                        // Spatial Stream - set to most commonly used value
                        combox_basicOp_spatialStream.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spatialStream, (int)idx_spatialStream_e.idx_spatialStream1);
                        combox_basicOp_spatialStream.Enabled = true;

                        // GI
                        GUI_removeMultipleBoxItems(combox_basicOp_GI, (int)idx_gi_e.idx_gi04);
                        combox_basicOp_GI.Enabled = true;
                        combox_basicOp_GI.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_GI, (int)idx_gi_e.idx_gi08);

                        // LTF
                        GUI_removeMultipleBoxItems(combox_basicOp_ltf, (int)idx_ltf_e.idx_ltfx1);
                        combox_basicOp_ltf.Enabled = true;
                        combox_basicOp_ltf.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_ltf, (int)idx_ltf_e.idx_ltfx1);

                        check_basicOp_txLongData.Enabled = true;
                        check_basicOp_txBeamforming.Enabled = true;
                        groupBox_WriteBeamformingMatrix.Enabled = true;

                        // Coding Type
                        radio_basicOp_codingAuto.Checked = true;
                        // radio_basicOp_codingBCC.Checked = false;
                        radio_basicOp_codingLDPC.Enabled = true;

                        break;
                    }
                case PhyMode.PHY_MODE_AX: // 802.11ax
                    {
                        Band selectedBand = GUI_GetSelectedBand();
                        if (selectedBand == Band.BAND_2400MHZ)
                        {
                            // Spectrum Bandwidth
                            GUI_removeMultipleBoxItems(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_FOURTY);
                            combox_basicOp_spectrumBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_FOURTY);

                            // Signal Bandwidth
                            GUI_removeMultipleBoxItems(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_FOURTY);
                            combox_basicOp_signalBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);

                            // Set Channel
                            combox_basicOp_lowChan.SelectedIndex = GUI_GetChannelAsIndex(combox_basicOp_lowChan, DEFAULT_CHANNEL_2400MHZ);
                        }
                        else if ((selectedBand == Band.BAND_5000MHZ) || (selectedBand == Band.BAND_6000MHZ))
                        {
                            // Spectrum Bandwidth
                            GUI_removeMultipleBoxItems(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_ONE_HUNDRED_SIXTY);
                            combox_basicOp_spectrumBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_TWENTY);

                            // Signal Bandwidth
                            GUI_removeMultipleBoxItems(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);
                            combox_basicOp_signalBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);

                            // Set Channel
                            combox_basicOp_lowChan.SelectedIndex = GUI_GetChannelAsIndex(combox_basicOp_lowChan, getDefaultChannel(selectedBand));
                        }

                        // MCS
                        GUI_removeSingleBoxItem(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_bpsk34);
                        GUI_removeMultipleBoxItems(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_1024qam56);
                        GUI_ReorderBoxItemsAsIndexPrefix(combox_basicOp_MCS);
                        combox_basicOp_MCS.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_qpsk34);

                        // Spatial Stream - set to most commonly used value
                        combox_basicOp_spatialStream.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spatialStream, (int)idx_spatialStream_e.idx_spatialStream1);
                        combox_basicOp_spatialStream.Enabled = true;

                        // GI
                        GUI_removeSingleBoxItem(combox_basicOp_GI, (int)idx_gi_e.idx_gi04);
                        combox_basicOp_GI.Enabled = true;
                        combox_basicOp_GI.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_GI, (int)idx_gi_e.idx_gi08);

                        // LTF
                        combox_basicOp_ltf.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_ltf, (int)idx_ltf_e.idx_ltfx2);
                        combox_basicOp_ltf.Enabled = true;

                        check_basicOp_txLongData.Enabled = true;
                        check_basicOp_txBeamforming.Enabled = true;
                        groupBox_WriteBeamformingMatrix.Enabled = true;

                        // Coding Type
                        radio_basicOp_codingAuto.Checked = true;
                        radio_basicOp_codingBCC.Enabled = true;
                        // radio_basicOp_codingLDPC.Checked = false;

                        break;
                    }
                case PhyMode.PHY_MODE_BE: // 802.11be
                    {
                        Band selectedBand = GUI_GetSelectedBand();
                        if (selectedBand == Band.BAND_2400MHZ)
                        {
                            // Spectrum Bandwidth
                            GUI_removeMultipleBoxItems(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_FOURTY);
                            combox_basicOp_spectrumBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_FOURTY);

                            // Signal Bandwidth
                            GUI_removeMultipleBoxItems(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_FOURTY);
                            combox_basicOp_signalBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);

                            // Set Channel
                            combox_basicOp_lowChan.SelectedIndex = GUI_GetChannelAsIndex(combox_basicOp_lowChan, DEFAULT_CHANNEL_2400MHZ);
                        }
                        else if ((selectedBand == Band.BAND_5000MHZ) || (selectedBand == Band.BAND_6000MHZ))
                        {
                            // Spectrum Bandwidth
                            if (selectedBand == Band.BAND_5000MHZ)
                            {
                                GUI_removeMultipleBoxItems(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_ONE_HUNDRED_SIXTY);
                            }
                            else
                            {
                                GUI_removeMultipleBoxItems(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_THREE_HUNDRED_TWENTY);
                            }
                            combox_basicOp_spectrumBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spectrumBW, (int)Bandwidth.BANDWIDTH_TWENTY);

                            // Signal Bandwidth
                            GUI_removeMultipleBoxItems(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);
                            combox_basicOp_signalBW.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_signalBW, (int)Bandwidth.BANDWIDTH_TWENTY);

                            // Set Channel
                            combox_basicOp_lowChan.SelectedIndex = GUI_GetChannelAsIndex(combox_basicOp_lowChan, getDefaultChannel(selectedBand));
                        }

                        // MCS
                        GUI_removeSingleBoxItem(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_bpsk34);
                        GUI_removeMultipleBoxItems(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_4096qam56);
                        GUI_ReorderBoxItemsAsIndexPrefix(combox_basicOp_MCS);
                        combox_basicOp_MCS.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_MCS, (int)idx_mcs_e.idx_mcs_qpsk34);

                        // Spatial Stream - set to most commonly used value
                        combox_basicOp_spatialStream.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_spatialStream, (int)idx_spatialStream_e.idx_spatialStream1);
                        combox_basicOp_spatialStream.Enabled = true;

                        // GI
                        GUI_removeSingleBoxItem(combox_basicOp_GI, (int)idx_gi_e.idx_gi04);
                        combox_basicOp_GI.Enabled = true;
                        combox_basicOp_GI.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_GI, (int)idx_gi_e.idx_gi08);

                        // LTF
                        combox_basicOp_ltf.SelectedIndex = GUI_GetValueAsIndex(combox_basicOp_ltf, (int)idx_ltf_e.idx_ltfx2);
                        combox_basicOp_ltf.Enabled = true;

                        check_basicOp_txLongData.Enabled = true;
                        check_basicOp_txBeamforming.Enabled = true;
                        groupBox_WriteBeamformingMatrix.Enabled = true;

                        // Coding Type
                        radio_basicOp_codingAuto.Checked = true;
                        radio_basicOp_codingBCC.Enabled = true;
                        //radio_basicOp_codingLDPC.Checked = false;

                        break;
                    }
            }
        } // end GUI_basicOp_loadPhyTypeAdjVals()

        /// <summary>
        /// Sets the text color for command button.
        /// </summary>

        static private void GUI_setColor_cmd(Button cmd_button, Color cmd_color)
        {
            cmd_button.ForeColor = cmd_color;
        }

        static private string getProductionFlagAsString(bool productionFlag)
        {
            return productionFlag ? "Production" : "Operational";
        }
        static private String getProductionFlagAsValue(bool productionFlag)
        {
            return "0x" + (productionFlag ? 0x72 : 0x30).ToString("x");
        }
        static private void getProductionCfg(string selected_str, out bool prodFlag)
        {
            switch (selected_str)
            {
                case "Operational": //flash
                    prodFlag = false;
                    break;
                case "Production": //eeprom_1k
                    prodFlag = true;
                    break;
                default:
                    prodFlag = false;
                    break;
            }
        }
        private void cmd_nvMemCtrl_burnProdFlag_Click(object sender, EventArgs e)
        {
            if (combox_nvMemCtrl_prodFlag.SelectedItem != null)
            {
                getProductionCfg((string)combox_nvMemCtrl_prodFlag.SelectedItem, out bool prodFlag);
                bool ok;
                using (new WaitCursorBlock(this))
                {
                    ok = DUT.BurnProdFlag(prodFlag);
                }
                if (ok)
                {
                    MessageDialog.ShowInformation("Complete Burn! Please reboot wlan!");
                }
            }
            else
            {
                MessageDialog.ShowWarning("Please select mode!");
            }
        }

        private void combox_nvMemCtrl_prodFlag_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch ((string)combox_nvMemCtrl_prodFlag.SelectedItem)
            {
                case "Production":
                    combox_nvMemCtrl_prodValue.Text = getProductionFlagAsValue(true);
                    break;
                case "Operational":
                    combox_nvMemCtrl_prodValue.Text = getProductionFlagAsValue(false);
                    break;
                default:
                    combox_nvMemCtrl_prodValue.Text = "0x00";
                    break;
            }
        }

        private void ProductionInfoUpdate()
        {
            if (DUT.ReadProdFlag(out bool productionFlag))
            {
                txtBox_basicOp_CalMode.Text = getProductionFlagAsString(productionFlag);
            }
        }

        private void enableRxAggregation_CheckedChanged(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                bool enabled = check_basicOp_enableRxAggregation.Checked;
                if (!DUT.EnableRxAggregation(enabled))
                {
                    this.check_basicOp_enableRxAggregation.CheckStateChanged -= new System.EventHandler(this.enableRxAggregation_CheckedChanged);
                    check_basicOp_enableRxAggregation.Checked = !enabled;
                    this.check_basicOp_enableRxAggregation.CheckStateChanged += new System.EventHandler(this.enableRxAggregation_CheckedChanged);
                }
                if (enabled)
                {
                    inputsList.Items[(int)InputsListItems4ant.PHY_PACKETS].Text = "Packets received (mpdu)";
                }
                else
                {
                    inputsList.Items[(int)InputsListItems4ant.PHY_PACKETS].Text = "Packets received (phy)";
                }
            }
        }
        private void DebugConsole_SelectedIndexChanged(object sender, EventArgs e)
        {
            dutConsoleWriter.OutputDestination = (OutputDestination)GUI_GetSelectedValue(combox_DebugConsole);
            if ((dutConsoleWriter.OutputDestination == OutputDestination.OUT_CONSOLE) || (dutConsoleWriter.OutputDestination == OutputDestination.OUT_ALL))
            {
                if (!isDbgConsoleLaunched)
                {
                    NativeMethods.AllocConsole();
                    isDbgConsoleLaunched = true;
                    StreamWriter standardOutput = new StreamWriter(Console.OpenStandardOutput());
                    standardOutput.AutoFlush = true;
                    StreamReader standardInput = new StreamReader(Console.OpenStandardInput());
                    dutConsoleWriter.UpdateConsoleOutput(standardOutput);
                    dutConsoleReader.UpdateConsoleInput(standardInput);
                    IntPtr hWin = NativeMethods.GetConsoleWindow();
                    Rectangle WorkingArea = SystemInformation.WorkingArea;

                    this.Left = initialRectDutGUI.Left;
                    this.Top = initialRectDutGUI.Top;
                    int x = initialRectDutGUI.Right;
                    int y = this.Top;
                    int w = WorkingArea.Width - initialRectDutGUI.Width;
                    int h = this.Height;
                    NativeMethods.MoveWindow(hWin, x, y, w, h, true);
                    NativeMethods.ShowWindow(hWin, NativeMethods.SW_SHOW);
                }
                else
                {
                    IntPtr hWin = NativeMethods.GetConsoleWindow();
                    NativeMethods.ShowWindow(hWin, NativeMethods.SW_SHOW);
                }
            }
            else
            {
                IntPtr hWin = NativeMethods.GetConsoleWindow();
                NativeMethods.ShowWindow(hWin, NativeMethods.SW_HIDE);
            }
        }

        static private byte getDefaultChannel(Band band)
        {
            switch (band)
            {
                case Band.BAND_2400MHZ:
                    return DEFAULT_CHANNEL_2400MHZ;
                case Band.BAND_5000MHZ:
                    return DEFAULT_CHANNEL_5000MHZ;
                case Band.BAND_6000MHZ:
                    return DEFAULT_CHANNEL_6000MHZ;
                default:
                    WriteToConsole("Failed to get default channel by band: the band must be one of 2.4G/5G/6G \r\n", Color.Red);
                    return INVALID_CHANNEL;
            }
        }

        private void CddSetIndexButton_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                int antenna = ParseControl(comboBox_extendedOp_antNum);
                int offset1 = ParseControl(txtBox_extendedOp_offset1);
                int offset2 = ParseControl(txtBox_extendedOp_offset2);
                int offset3 = ParseControl(txtBox_extendedOp_offset3);
                // TODO: add this check to library
                //when writing set cdd ,need always put 4 parameters 1)number of antenna 2)offset1 3)offset2 4)offset3 
                //if used less than 4 antenna for example 3 need to write 0 to offset number 3

                DUT.SetBbicCddValues((byte)antenna, (uint)offset1, (uint)offset2, (uint)offset3);
            }
        }

        private void cmd_extendedOp_CDDget_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                byte antenna = (byte)ParseControl(comboBox_extendedOp_antNum);
                if (DUT.GetBbicCddValues(antenna, out uint offset1, out uint offset2, out uint offset3))
                {
                    txtBox_extendedOp_offset1.Text = offset1.ToString();
                    txtBox_extendedOp_offset2.Text = offset2.ToString();
                    txtBox_extendedOp_offset3.Text = offset3.ToString();
                }
            }
        }

        private void comboBox_extendedOp_antNum_SelectedIndexChanged(object sender, EventArgs e)
        {
            int antNum = ParseControl(comboBox_extendedOp_antNum);
            switch (antNum)
            {
                case 4:
                    txtBox_extendedOp_offset1.Enabled = true;
                    txtBox_extendedOp_offset2.Enabled = true;
                    txtBox_extendedOp_offset3.Enabled = true;
                    break;
                case 3:
                    txtBox_extendedOp_offset1.Enabled = true;
                    txtBox_extendedOp_offset2.Enabled = true;
                    txtBox_extendedOp_offset3.Text = "0";
                    txtBox_extendedOp_offset3.Enabled = false;
                    break;
                case 2:
                    txtBox_extendedOp_offset1.Enabled = true;
                    txtBox_extendedOp_offset2.Text = "0";
                    txtBox_extendedOp_offset2.Enabled = false;
                    txtBox_extendedOp_offset3.Text = "0";
                    txtBox_extendedOp_offset3.Enabled = false;
                    break;
                case 1:
                    txtBox_extendedOp_offset1.Text = "0";
                    txtBox_extendedOp_offset1.Enabled = false;
                    txtBox_extendedOp_offset2.Text = "0";
                    txtBox_extendedOp_offset2.Enabled = false;
                    txtBox_extendedOp_offset3.Text = "0";
                    txtBox_extendedOp_offset3.Enabled = false;
                    break;
            }
        }

        private void combox_MoreComp_SelectedIndexChanged(object sender, EventArgs e)
        {
            VersionedComponent compType = (VersionedComponent)GUI_GetSelectedValue(combox_MoreComp);
            try
            {
                if (DUT.GetComponentVersion(compType, out string version))
                {
                    txtBox_nvMemCtrl_MoreCompVersion.Text = version;
                }
            }
            catch (Exception ex)
            {
                WriteToConsole("could not GetComponentVersion: " + ex.Message, Color.DarkViolet);
            }
        }

        private void cmd_nvMemCtrl_readRF_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                uint address = (uint)ParseControl(txtBox_nvMemCtrl_RFaddress);
                if (DUT.ReadMemory(ChipModule.CHIP_MODULE_RF, address, out byte[] data, RFIC.FCSI_REG_BYTE_LENGTH))
                {
                    UInt16[] regData = new UInt16[RFIC.FCSI_TOTAL_CHANNELS];
                    for (int i = 0; i < RFIC.FCSI_TOTAL_CHANNELS; i++)
                    {
                        regData[i] = BitConverter.ToUInt16(data, i * 2);
                    }

                    txtBox_nvMemCtrl_RFval0.Text = "0x" + regData[0].ToString("x");
                    txtBox_nvMemCtrl_RFval1.Text = "0x" + regData[1].ToString("x");
                    txtBox_nvMemCtrl_RFval2.Text = "0x" + regData[2].ToString("x");
                    txtBox_nvMemCtrl_RFval3.Text = "0x" + regData[3].ToString("x");
                    txtBox_nvMemCtrl_RFval4.Text = "0x" + regData[4].ToString("x");
                }
            }
        }

        private void cmd_nvMemCtrl_writeRF_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                UInt16[] regData = new UInt16[RFIC.FCSI_TOTAL_CHANNELS];
                byte[] data = new byte[RFIC.FCSI_TOTAL_CHANNELS * 2];

                uint _address = (uint)ParseControl(txtBox_nvMemCtrl_RFaddress);
                regData[0] = (byte)ParseControl(txtBox_nvMemCtrl_RFval0);
                regData[1] = (byte)ParseControl(txtBox_nvMemCtrl_RFval1);
                regData[2] = (byte)ParseControl(txtBox_nvMemCtrl_RFval2);
                regData[3] = (byte)ParseControl(txtBox_nvMemCtrl_RFval3);
                regData[4] = (byte)ParseControl(txtBox_nvMemCtrl_RFval4);
                for (int i = 0; i < RFIC.FCSI_TOTAL_CHANNELS; i++)
                {
                    byte[] byteArray = BitConverter.GetBytes(regData[i]);
                    data[i * 2] = byteArray[0];
                    data[i * 2 + 1] = byteArray[1];
                }
                DUT.WriteMemory(ChipModule.CHIP_MODULE_RF, _address, data, (uint)data.Length);
            }
        }

        private void cmd_nvMemCtrl_allRFRW_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                WriteToConsole("Reading All RFIC registers, please wait...", Color.RoyalBlue);
                RFIC rficDlg = new RFIC(DUT);
                rficDlg.ShowDialog();
            }
        }

        private void cmd_nvMemCtrl_readReg_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                ChipModule chipModule = (ChipModule)ParseControl(txtBox_nvMemCtrl_regModule);
                uint address = (uint)ParseControl(txtBox_nvMemCtrl_regAddress);
                uint mask = (uint)ParseControl(txtBox_nvMemCtrl_regMask);
                if (DUT.ReadRegister(chipModule, address, mask, out uint value))
                {
                    txtBox_nvMemCtrl_regValue.Text = value.ToString();
                }
            }
        }

        private void cmd_nvMemCtrl_writeReg_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                ChipModule chipModule = (ChipModule)ParseControl(txtBox_nvMemCtrl_regModule);
                uint address = (uint)ParseControl(txtBox_nvMemCtrl_regAddress);
                uint mask = (uint)ParseControl(txtBox_nvMemCtrl_regMask);
                uint value = (uint)ParseControl(txtBox_nvMemCtrl_regValue);

                DUT.WriteRegister(chipModule, address, mask, value);
            }
        }

        private void button_browseBeamformingMatrixHeaderFile_Click(object sender, EventArgs e)
        {
            openBeamformingMatrixHeaderFile_standard.FileName = txtBox_beamformingMatrixHeaderFile_standard.Text;
            if (openBeamformingMatrixHeaderFile_standard.ShowDialog() == DialogResult.OK)
            {
                txtBox_beamformingMatrixHeaderFile_standard.Text = openBeamformingMatrixHeaderFile_standard.FileName;
            }
        }

        private void button_browseBeamformingMatrixValuesFile_Click(object sender, EventArgs e)
        {
            openBeamformingMatrixValuesFile_standard.FileName = txtBox_beamformingMatrixValuesFile_standard.Text;
            if (openBeamformingMatrixValuesFile_standard.ShowDialog() == DialogResult.OK)
            {
                txtBox_beamformingMatrixValuesFile_standard.Text = openBeamformingMatrixValuesFile_standard.FileName;
            }
        }

        // 160MHz tab browse button handlers
        private void button_browseBeamformingMatrixHeaderFile_160mhz_Click(object sender, EventArgs e)
        {
            openBeamformingMatrixHeaderFile_160mhz.FileName = txtBox_beamformingMatrixHeaderFile_160mhz.Text;
            if (openBeamformingMatrixHeaderFile_160mhz.ShowDialog() == DialogResult.OK)
            {
                txtBox_beamformingMatrixHeaderFile_160mhz.Text = openBeamformingMatrixHeaderFile_160mhz.FileName;
            }
        }

        private void button_browseBeamformingMatrixValuesFile_160mhz_Click(object sender, EventArgs e)
        {
            openBeamformingMatrixValuesFile_160mhz.FileName = txtBox_beamformingMatrixValuesFile_160mhz.Text;
            if (openBeamformingMatrixValuesFile_160mhz.ShowDialog() == DialogResult.OK)
            {
                txtBox_beamformingMatrixValuesFile_160mhz.Text = openBeamformingMatrixValuesFile_160mhz.FileName;
            }
        }

        private void button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz_Click(object sender, EventArgs e)
        {
            openBeamformingMatrixValuesFile_ehtExtra_160mhz.FileName = txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz.Text;
            if (openBeamformingMatrixValuesFile_ehtExtra_160mhz.ShowDialog() == DialogResult.OK)
            {
                txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz.Text = openBeamformingMatrixValuesFile_ehtExtra_160mhz.FileName;
            }
        }

        // 320MHz lower tab browse button handlers
        private void button_browseBeamformingMatrixHeaderFile_lower_320mhz_Click(object sender, EventArgs e)
        {
            openBeamformingMatrixHeaderFile_lower_320mhz.FileName = txtBox_beamformingMatrixHeaderFile_lower_320mhz.Text;
            if (openBeamformingMatrixHeaderFile_lower_320mhz.ShowDialog() == DialogResult.OK)
            {
                txtBox_beamformingMatrixHeaderFile_lower_320mhz.Text = openBeamformingMatrixHeaderFile_lower_320mhz.FileName;
            }
        }

        private void button_browseBeamformingMatrixValuesFile_lower_320mhz_Click(object sender, EventArgs e)
        {
            openBeamformingMatrixValuesFile_lower_320mhz.FileName = txtBox_beamformingMatrixValuesFile_lower_320mhz.Text;
            if (openBeamformingMatrixValuesFile_lower_320mhz.ShowDialog() == DialogResult.OK)
            {
                txtBox_beamformingMatrixValuesFile_lower_320mhz.Text = openBeamformingMatrixValuesFile_lower_320mhz.FileName;
            }
        }

        private void button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz_Click(object sender, EventArgs e)
        {
            openBeamformingMatrixValuesFile_ehtExtra_lower_320mhz.FileName = txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz.Text;
            if (openBeamformingMatrixValuesFile_ehtExtra_lower_320mhz.ShowDialog() == DialogResult.OK)
            {
                txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz.Text = openBeamformingMatrixValuesFile_ehtExtra_lower_320mhz.FileName;
            }
        }

        // 320MHz upper tab browse button handlers
        private void button_browseBeamformingMatrixHeaderFile_upper_320mhz_Click(object sender, EventArgs e)
        {
            openBeamformingMatrixHeaderFile_upper_320mhz.FileName = txtBox_beamformingMatrixHeaderFile_upper_320mhz.Text;
            if (openBeamformingMatrixHeaderFile_upper_320mhz.ShowDialog() == DialogResult.OK)
            {
                txtBox_beamformingMatrixHeaderFile_upper_320mhz.Text = openBeamformingMatrixHeaderFile_upper_320mhz.FileName;
            }
        }

        private void button_browseBeamformingMatrixValuesFile_upper_320mhz_Click(object sender, EventArgs e)
        {
            openBeamformingMatrixValuesFile_upper_320mhz.FileName = txtBox_beamformingMatrixValuesFile_upper_320mhz.Text;
            if (openBeamformingMatrixValuesFile_upper_320mhz.ShowDialog() == DialogResult.OK)
            {
                txtBox_beamformingMatrixValuesFile_upper_320mhz.Text = openBeamformingMatrixValuesFile_upper_320mhz.FileName;
            }
        }

        private void button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz_Click(object sender, EventArgs e)
        {
            openBeamformingMatrixValuesFile_ehtExtra_upper_320mhz.FileName = txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz.Text;
            if (openBeamformingMatrixValuesFile_ehtExtra_upper_320mhz.ShowDialog() == DialogResult.OK)
            {
                txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz.Text = openBeamformingMatrixValuesFile_ehtExtra_upper_320mhz.FileName;
            }
        }

        private void button_writeBeamformingMatrixFile_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                // Use standard tab file paths
                string primaryHeaderFile = txtBox_beamformingMatrixHeaderFile_standard.Text;
                string primaryValuesFile = txtBox_beamformingMatrixValuesFile_standard.Text;
                string primaryExtValuesEhtFile = null;

                // No secondary files for standard bandwidth
                string secondaryHeaderFile = null;
                string secondaryValuesFile = null;
                string secondaryExtValuesEhtFile = null;

                // Load beamforming matrix using the new API
                DUT.LoadBeamformingMatrixFromFileSet(
                    primaryHeaderFile, primaryValuesFile, primaryExtValuesEhtFile,
                    secondaryHeaderFile, secondaryValuesFile, secondaryExtValuesEhtFile);
            }
        }

        private void button_writeBeamformingMatrixFile_160mhz_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                // Use 160MHz specific file paths
                string primaryHeaderFile = txtBox_beamformingMatrixHeaderFile_160mhz.Text;
                string primaryValuesFile = txtBox_beamformingMatrixValuesFile_160mhz.Text;
                string primaryExtValuesEhtFile = string.IsNullOrEmpty(txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz.Text) ? null : txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz.Text;

                // No secondary files for 160MHz
                string secondaryHeaderFile = null;
                string secondaryValuesFile = null;
                string secondaryExtValuesEhtFile = null;

                // Load beamforming matrix using the new API
                DUT.LoadBeamformingMatrixFromFileSet(
                    primaryHeaderFile, primaryValuesFile, primaryExtValuesEhtFile,
                    secondaryHeaderFile, secondaryValuesFile, secondaryExtValuesEhtFile);
            }
        }

        private void button_writeBeamformingMatrixFile_320mhz_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                // Use 320MHz specific file paths (primary = lower, secondary = upper)
                string primaryHeaderFile = txtBox_beamformingMatrixHeaderFile_lower_320mhz.Text;
                string primaryValuesFile = txtBox_beamformingMatrixValuesFile_lower_320mhz.Text;
                string primaryExtValuesEhtFile = string.IsNullOrEmpty(txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz.Text) ? null : txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz.Text;

                // Secondary files for 320MHz upper segment
                string secondaryHeaderFile = string.IsNullOrEmpty(txtBox_beamformingMatrixHeaderFile_upper_320mhz.Text) ? null : txtBox_beamformingMatrixHeaderFile_upper_320mhz.Text;
                string secondaryValuesFile = string.IsNullOrEmpty(txtBox_beamformingMatrixValuesFile_upper_320mhz.Text) ? null : txtBox_beamformingMatrixValuesFile_upper_320mhz.Text;
                string secondaryExtValuesEhtFile = string.IsNullOrEmpty(txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz.Text) ? null : txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz.Text;

                DUT.LoadBeamformingMatrixFromFileSet(
                    primaryHeaderFile, primaryValuesFile, primaryExtValuesEhtFile,
                    secondaryHeaderFile, secondaryValuesFile, secondaryExtValuesEhtFile);
            }
        }

        private void combox_basicOp_primaryChannelIndex_Leave(object sender, EventArgs e)
        {
            if (!combox_basicOp_primaryChannelIndex.Items.Contains(combox_basicOp_primaryChannelIndex.Text))
            {
                combox_basicOp_primaryChannelIndex.SelectedIndex = 0;
            }
        }

        private void combox_basicOp_primaryChannelIndex_SelectedIndexChanged(object sender, EventArgs e)
        {
            int newPrimaryChannelIndex = ParseControl(combox_basicOp_primaryChannelIndex);
            if (prevPrimaryChannelIndex != newPrimaryChannelIndex)
            {
                prevPrimaryChannelIndex = newPrimaryChannelIndex;
                cmd_basicOp_setChannel.ForeColor = Color.Red;
            }
        }

        private void button_getTemperature_Click(object sender, EventArgs e)
        {
            using (new WaitCursorBlock(this))
            {
                float temperature = DUT.GetTemperature();
                textBoxTemperature.Text = temperature.ToString("f2");
            }
        }

        private void UpdatePacketLength()
        {
            if (!check_basicOp_numSymbols.Checked)
            {
                return;
            }

            uint numSymbols = (uint)ParseControl(txtBox_basicOp_numSymbols);
            if (numSymbols == 0)
            {
                return;
            }

            using (new WaitCursorBlock(this))
            {
                try
                {
                    PhyMode phyMode = GUI_GetSelectedPhyMode();
                    Bandwidth signalBandwidth = GUI_GetSelectedSignalBandwidth();
                    Mcs mcs = GetMcs();
                    byte nss = (byte)ParseControl(combox_basicOp_spatialStream);

                    if (DUT.GetMaxPacketLength(phyMode, out uint maxPacketLength) && DUT.GetPacketLength(phyMode, signalBandwidth, mcs, nss, numSymbols, out uint packetLength))
                    {
                        packetLength = Math.Min(maxPacketLength, packetLength);
                        txtBox_basicOp_packetLen.Text = Convert.ToString(packetLength);
                    }
                }
                catch (Exception ex)
                {
                    WriteToConsole(ex.Message, Color.Red);
                }
            }
        }

        private void check_basicOp_numSymbols_CheckedChanged(object sender, EventArgs e)
        {
            txtBox_basicOp_numSymbols.Enabled = check_basicOp_numSymbols.Checked;
            txtBox_basicOp_packetLen.Enabled = !txtBox_basicOp_numSymbols.Enabled;
            if (txtBox_basicOp_numSymbols.Enabled)
            {
                UpdatePacketLength();
            }
        }

        private void txtBox_basicOp_numSymbols_Leave(object sender, EventArgs e)
        {
            UpdatePacketLength();
        }

        private void UpdateNumOfdmSymbolsCheckBox()
        {
            bool numSymbolsEnabled = check_basicOp_txLongData.Checked && (GUI_GetSelectedPhyMode() != PhyMode.PHY_MODE_B);
            check_basicOp_numSymbols.Enabled = numSymbolsEnabled;
            if (!numSymbolsEnabled)
            {
                check_basicOp_numSymbols.Checked = false;
            }
        }

        private void check_basicOp_txLongData_CheckedChanged(object sender, EventArgs e)
        {
            UpdateNumOfdmSymbolsCheckBox();
        }

        private void checkBox_basicOp_autoPower_CheckedChanged(object sender, EventArgs e)
        {
            txtBox_basicOp_powerLimit.Enabled = checkBox_basicOp_autoPower.Checked;
            txtBox_basicOp_powerLevel.Enabled = !checkBox_basicOp_autoPower.Checked;
            GUI_setColor_cmd(cmd_basicOp_setPower, Color.Red);
        }

        private void txtBox_basicOp_powerLimit_TextChanged(object sender, EventArgs e)
        {
            GUI_setColor_cmd(cmd_basicOp_setPower, Color.Red);
        }

        private void txtBox_basicOp_powerLevel_TextChanged(object sender, EventArgs e)
        {
            GUI_setColor_cmd(cmd_basicOp_setPower, Color.Red);
        }

        private void cmd_basicOp_setRuParams_Click(object sender, EventArgs e)
        {
            uint lowerLimit = 0;
            uint userOneUpperLimit = 0x1ff; //9-bit Input
            uint userTwoUpperLimit = 0x200; // one additional value to denote if User 2 is valid (SU)
            int userOne = ParseControl(txtBox_basicOp_user1);
            int userTwo;

            if (String.IsNullOrEmpty(txtBox_basicOp_user2.Text))
            {
                // If userTwo is empty treat it as SU
                userTwo = (int)userTwoUpperLimit;
            }
            else
            {
                userTwo = ParseControl(txtBox_basicOp_user2);
            }

            try
            {
                bool userOneEmpty = String.IsNullOrEmpty(txtBox_basicOp_user1.Text);

                if (userOneEmpty) throw new ApplicationException("User 1 value is required\nRange " + lowerLimit.ToString() + "-" + userOneUpperLimit.ToString() + "\nINFO: Bits 0-7 : RU Allocation, Bit 8 - PS 160");

                bool userOneWithinRange = (userOne >= lowerLimit) && (userOne <= userOneUpperLimit);
                bool userTwoWithinRange = (userTwo >= lowerLimit) && (userTwo <= userTwoUpperLimit);

                if (!userOneWithinRange) throw new ApplicationException("User 1 value of " + userOne.ToString() + " is INVALID!\nRange " + lowerLimit.ToString() + "-" + userOneUpperLimit.ToString());
                if (!userTwoWithinRange) throw new ApplicationException("User 2 value of " + userTwo.ToString() + " is INVALID!\nRange " + lowerLimit.ToString() + "-" + userTwoUpperLimit.ToString() + "\nLeave User 2 empty or enter 512 for SU");

            }

            catch (Exception ex)
            {
                MessageDialog.ShowWarning(ex.Message);
                return;
            }

            using (new WaitCursorBlock(this))
            {
                DUT.SetRUParams((uint)userOne, (uint)userTwo);
            }
        }

        private void label15_Click(object sender, EventArgs e)
        {

        }

        private void tableLayoutPanel6_Paint(object sender, PaintEventArgs e)
        {

        }


    }
}
