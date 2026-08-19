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
using dut_cli;
using static DUT_GUI.GUI_Constants;

namespace DUT_GUI
{
    partial class DUT_GUI
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem("RX EVM S1");
            System.Windows.Forms.ListViewItem listViewItem2 = new System.Windows.Forms.ListViewItem("RX EVM S2");
            System.Windows.Forms.ListViewItem listViewItem3 = new System.Windows.Forms.ListViewItem("RX EVM S3");
            System.Windows.Forms.ListViewItem listViewItem4 = new System.Windows.Forms.ListViewItem("RX EVM S4");
            System.Windows.Forms.ListViewItem listViewItem5 = new System.Windows.Forms.ListViewItem("RX EVM S5");
            System.Windows.Forms.ListViewItem listViewItem6 = new System.Windows.Forms.ListViewItem("RSSI RX0");
            System.Windows.Forms.ListViewItem listViewItem7 = new System.Windows.Forms.ListViewItem("RSSI RX1");
            System.Windows.Forms.ListViewItem listViewItem8 = new System.Windows.Forms.ListViewItem("RSSI RX2");
            System.Windows.Forms.ListViewItem listViewItem9 = new System.Windows.Forms.ListViewItem("RSSI RX3");
            System.Windows.Forms.ListViewItem listViewItem10 = new System.Windows.Forms.ListViewItem("RSSI RX4");
            System.Windows.Forms.ListViewItem listViewItem11 = new System.Windows.Forms.ListViewItem("Packets received (phy)");
            System.Windows.Forms.ListViewItem listViewItem12 = new System.Windows.Forms.ListViewItem("Packets w/ CRC error");
            System.Windows.Forms.ListViewItem listViewItem13 = new System.Windows.Forms.ListViewItem("Packets received (mac)");
            System.Windows.Forms.ListViewItem listViewItem14 = new System.Windows.Forms.ListViewItem("PA Voltage TX0");
            System.Windows.Forms.ListViewItem listViewItem15 = new System.Windows.Forms.ListViewItem("PA Voltage TX1");
            System.Windows.Forms.ListViewItem listViewItem16 = new System.Windows.Forms.ListViewItem("PA Voltage TX2");
            System.Windows.Forms.ListViewItem listViewItem17 = new System.Windows.Forms.ListViewItem("PA Voltage TX3");
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DUT_GUI));
            this.dllConsole = new System.Windows.Forms.RichTextBox();
            this.txtBox_nvMemCtrl_dutVersion = new System.Windows.Forms.TextBox();
            this.lbl_nvMemCtrl_dutVersion = new System.Windows.Forms.Label();
            this.table_nvMemCtrl_versionInfo = new System.Windows.Forms.TableLayoutPanel();
            this.txtBox_nvMemCtrl_BBchipVersion = new System.Windows.Forms.TextBox();
            this.txtBox_nvMemCtrl_RFchipVersion = new System.Windows.Forms.TextBox();
            this.lbl_nvMemCtrl_BBchip = new System.Windows.Forms.Label();
            this.lbl_nvMemCtrl_RFchip = new System.Windows.Forms.Label();
            this.txtBox_nvMemCtrl_CVVersion = new System.Windows.Forms.TextBox();
            this.txtBox_nvMemCtrl_MoreCompVersion = new System.Windows.Forms.TextBox();
            this.lbl_nvMemCtrl_CV = new System.Windows.Forms.Label();
            this.combox_MoreComp = new System.Windows.Forms.ComboBox();
            this.table_basicOp_antennas = new System.Windows.Forms.TableLayoutPanel();
            this.cmd_basicOp_txAnt0 = new System.Windows.Forms.Button();
            this.cmd_basicOp_txAnt2 = new System.Windows.Forms.Button();
            this.cmd_basicOp_rxAnt1 = new System.Windows.Forms.Button();
            this.cmd_basicOp_txAnt1 = new System.Windows.Forms.Button();
            this.cmd_basicOp_txAnt3 = new System.Windows.Forms.Button();
            this.cmd_basicOp_rxAnt2 = new System.Windows.Forms.Button();
            this.cmd_basicOp_rxAnt0 = new System.Windows.Forms.Button();
            this.cmd_basicOp_rxAnt3 = new System.Windows.Forms.Button();
            this.cmd_basicOp_rxAnt4 = new System.Windows.Forms.Button();
            this.groupBox_tx = new System.Windows.Forms.GroupBox();
            this.table_basicOp_txALL = new System.Windows.Forms.TableLayoutPanel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel_basicOp_rate_cmd = new System.Windows.Forms.Panel();
            this.cmd_basicOp_setRate = new System.Windows.Forms.Button();
            this.txtBox_basicOp_phyRate = new System.Windows.Forms.TextBox();
            this.lbl_basicOp_phyRate = new System.Windows.Forms.Label();
            this.lbl_basicOp_powerParam = new System.Windows.Forms.Label();
            this.lbl_basicOp_chanParam = new System.Windows.Forms.Label();
            this.panel_basicOp_rate_params = new System.Windows.Forms.Panel();
            this.combox_basicOp_signalBW = new System.Windows.Forms.ComboBox();
            this.combox_basicOp_ltf = new System.Windows.Forms.ComboBox();
            this.lbl_basicOp_LTF = new System.Windows.Forms.Label();
            this.combox_basicOp_GI = new System.Windows.Forms.ComboBox();
            this.lbl_basicOp_signalBW = new System.Windows.Forms.Label();
            this.lbl_basicOp_GI = new System.Windows.Forms.Label();
            this.combox_basicOp_spatialStream = new System.Windows.Forms.ComboBox();
            this.lbl_basicOp_spatialStream = new System.Windows.Forms.Label();
            this.combox_basicOp_MCS = new System.Windows.Forms.ComboBox();
            this.lbl_basicOp_MCS = new System.Windows.Forms.Label();
            this.lbl_basicOp_rateParam = new System.Windows.Forms.Label();
            this.lbl_basicOp_spacing = new System.Windows.Forms.Label();
            this.panel_spacing_params = new System.Windows.Forms.Panel();
            this.txtBox_basicOp_spacingIFS = new System.Windows.Forms.TextBox();
            this.lbl_basicOp_us = new System.Windows.Forms.Label();
            this.check_basicOp_closeRXants = new System.Windows.Forms.CheckBox();
            this.panel_spacing_cmd = new System.Windows.Forms.Panel();
            this.cmd_basicOp_setSpacing = new System.Windows.Forms.Button();
            this.panel_txParams_cmd = new System.Windows.Forms.Panel();
            this.cmd_basicOp_stopTransmission = new System.Windows.Forms.Button();
            this.cmd_basicOp_startTransmitting = new System.Windows.Forms.Button();
            this.lbl_basicOp_txParam = new System.Windows.Forms.Label();
            this.panel_powerParam_cmd = new System.Windows.Forms.Panel();
            this.txtBox_basicOp_powerLimit = new System.Windows.Forms.TextBox();
            this.lbl_basicOp_powerLimit = new System.Windows.Forms.Label();
            this.txtBox_basicOp_powerLevel = new System.Windows.Forms.TextBox();
            this.lbl_basicOp_powerLevel = new System.Windows.Forms.Label();
            this.checkBox_basicOp_autoPower = new System.Windows.Forms.CheckBox();
            this.checkBox_basicOp_closedLoop = new System.Windows.Forms.CheckBox();
            this.panel_loop_cmd = new System.Windows.Forms.Panel();
            this.cmd_basicOp_setPower = new System.Windows.Forms.Button();
            this.panel_basicOp_channel_cmd = new System.Windows.Forms.Panel();
            this.cmd_basicOp_setChannel = new System.Windows.Forms.Button();
            this.lbl_basicOp_band = new System.Windows.Forms.Label();
            this.panel_basicOp_params = new System.Windows.Forms.Panel();
            this.lbl_basicOp_band_choose = new System.Windows.Forms.Label();
            this.combox_basicOp_band = new System.Windows.Forms.ComboBox();
            this.lbl_basicOp_phy = new System.Windows.Forms.Label();
            this.panel_basicOp_phy_param = new System.Windows.Forms.Panel();
            this.combox_basicOp_phyType = new System.Windows.Forms.ComboBox();
            this.lbl_basicOp_phy_choose = new System.Windows.Forms.Label();
            this.panel_basicOp_txParams_params = new System.Windows.Forms.Panel();
            this.radio_basicOp_codingLDPC = new System.Windows.Forms.RadioButton();
            this.radio_basicOp_codingBCC = new System.Windows.Forms.RadioButton();
            this.lbl_basicOp_codingType = new System.Windows.Forms.Label();
            this.check_basicOp_numSymbols = new System.Windows.Forms.CheckBox();
            this.txtBox_basicOp_numSymbols = new System.Windows.Forms.TextBox();
            this.check_basicOp_txBeamforming = new System.Windows.Forms.CheckBox();
            this.check_basicOp_txLongData = new System.Windows.Forms.CheckBox();
            this.txtBox_basicOp_repetitions = new System.Windows.Forms.TextBox();
            this.lbl_basicOp_repetitions = new System.Windows.Forms.Label();
            this.lbl_basicOp_packetLen = new System.Windows.Forms.Label();
            this.txtBox_basicOp_packetLen = new System.Windows.Forms.TextBox();
            this.panel_basicOp_channel_params = new System.Windows.Forms.Panel();
            this.combox_basicOp_regulationType = new System.Windows.Forms.ComboBox();
            this.label_basicOp_regulationType = new System.Windows.Forms.Label();
            this.combox_basicOp_primaryChannelIndex = new System.Windows.Forms.ComboBox();
            this.combox_basicOp_spectrumBW = new System.Windows.Forms.ComboBox();
            this.lbl_basicOp_spectrumBW = new System.Windows.Forms.Label();
            this.combox_basicOp_lowChan = new System.Windows.Forms.ComboBox();
            this.txtBox_basicOp_CenterFreqMHz = new System.Windows.Forms.TextBox();
            this.lbl_basicOp_CenterFreqMHz = new System.Windows.Forms.Label();
            this.lbl_basicOp_lowChan = new System.Windows.Forms.Label();
            this.groupBox_burnEEPROM = new System.Windows.Forms.GroupBox();
            this.cmd_nvMemCtrl_burnToNVmemory = new System.Windows.Forms.Button();
            this.table_nvMemCtrl_burnEEPROM = new System.Windows.Forms.TableLayoutPanel();
            this.lbl_nvMemCtrl_calFile = new System.Windows.Forms.Label();
            this.txtBox_nvMemCtrl_eepromFile = new System.Windows.Forms.TextBox();
            this.cmd_nvMemCtrl_browseEEPROM = new System.Windows.Forms.Button();
            this.cmd_nvMemCtrl_readExtCalFile = new System.Windows.Forms.Button();
            this.lbl_nvMemCtrl_barcode = new System.Windows.Forms.Label();
            this.txtBox_nvMemCtrl_barcode = new System.Windows.Forms.TextBox();
            this.cmd_nvMemCtrl_viewEEPROM = new System.Windows.Forms.Button();
            this.openEEPROMfile = new System.Windows.Forms.OpenFileDialog();
            this.openBeamformingMatrixValuesFile_standard = new System.Windows.Forms.OpenFileDialog();
            this.groupBox_genRisc = new System.Windows.Forms.GroupBox();
            this.check_basicOp_enableRxAggregation = new System.Windows.Forms.CheckBox();
            this.RxEvmList = new System.Windows.Forms.ListView();
            this.rxEvmName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.rxEvmValue = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.cmd_basicOp_updateRXevm = new System.Windows.Forms.Button();
            this.inputsList = new System.Windows.Forms.ListView();
            this.inputName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.inputValue = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.cmd_basicOp_resetCounter = new System.Windows.Forms.Button();
            this.cmd_basicOp_updateInput = new System.Windows.Forms.Button();
            this.pictureBoxLogo = new System.Windows.Forms.PictureBox();
            this.cmd_basicOp_stopCW = new System.Windows.Forms.Button();
            this.cmd_basicOp_transmitCW = new System.Windows.Forms.Button();
            this.lbl_basicOp_amplitude = new System.Windows.Forms.Label();
            this.txtBox_basicOp_cwAmplitude = new System.Windows.Forms.TextBox();
            this.lbl_basicOp_binNum = new System.Windows.Forms.Label();
            this.txtBox_basicOp_cwBinNum = new System.Windows.Forms.TextBox();
            this.groupBox_connection = new System.Windows.Forms.GroupBox();
            this.comboBox_basicOp_memoryType = new System.Windows.Forms.ComboBox();
            this.groupBox_WlanCard = new System.Windows.Forms.GroupBox();
            this.wlan4card = new System.Windows.Forms.RadioButton();
            this.wlan2card = new System.Windows.Forms.RadioButton();
            this.wlan0card = new System.Windows.Forms.RadioButton();
            this.snifferEnable = new System.Windows.Forms.CheckBox();
            this.ipAddress = new System.Windows.Forms.TextBox();
            this.ConnectButton = new System.Windows.Forms.Button();
            this.combox_DebugConsole = new System.Windows.Forms.ComboBox();
            this.combox_nvMemCtrl_printLevel = new System.Windows.Forms.ComboBox();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.basicOperationTab = new System.Windows.Forms.TabPage();
            this.groupBox_RuParameters = new System.Windows.Forms.GroupBox();
            this.cmd_basicOp_setRuParams = new System.Windows.Forms.Button();
            this.txtBox_basicOp_user2 = new System.Windows.Forms.TextBox();
            this.txtBox_basicOp_user1 = new System.Windows.Forms.TextBox();
            this.lbl_basicOp_user2 = new System.Windows.Forms.Label();
            this.lbl_basicOp_user1 = new System.Windows.Forms.Label();
            this.groupBox_basicOp_spaceless = new System.Windows.Forms.GroupBox();
            this.cmd_basicOp_stopSpaceless = new System.Windows.Forms.Button();
            this.cmd_basicOp_startSpaceless = new System.Windows.Forms.Button();
            this.groupBox_cardID = new System.Windows.Forms.GroupBox();
            this.txtBox_basicOp_rxAntennaMask = new System.Windows.Forms.TextBox();
            this.txtBox_basicOp_CalMode = new System.Windows.Forms.TextBox();
            this.btn_basicOp_deleteRegistry = new System.Windows.Forms.Button();
            this.txtBox_basicOp_bandSupport = new System.Windows.Forms.TextBox();
            this.lbl_basicOp_bandSupport = new System.Windows.Forms.Label();
            this.txtBox_basicOp_bandCurrent = new System.Windows.Forms.TextBox();
            this.lbl_basicOp_bandCurrent = new System.Windows.Forms.Label();
            this.txtBox_basicOp_txAntennaMask = new System.Windows.Forms.TextBox();
            this.lbl_basicOp_antMask = new System.Windows.Forms.Label();
            this.lbl_basicOp_wav = new System.Windows.Forms.Label();
            this.txtBox_basicOp_wav = new System.Windows.Forms.TextBox();
            this.groupBox_basicOp_misc = new System.Windows.Forms.GroupBox();
            this.groupBox_antennasOnOff = new System.Windows.Forms.GroupBox();
            this.extendedOperationTab = new System.Windows.Forms.TabPage();
            this.tableLayoutPanel6 = new System.Windows.Forms.TableLayoutPanel();
            this.bf_tabControl = new System.Windows.Forms.TabControl();
            this.bf_tab_default = new System.Windows.Forms.TabPage();
            this.label22 = new System.Windows.Forms.Label();
            this.bf_tab_standard = new System.Windows.Forms.TabPage();
            this.groupBox_WriteBeamformingMatrix = new System.Windows.Forms.GroupBox();
            this.button_writeBeamformingMatrixFile_standard = new System.Windows.Forms.Button();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.lbl_beamformingMatrixValuesFile = new System.Windows.Forms.Label();
            this.lbl_beamformingMatrixHeaderFile = new System.Windows.Forms.Label();
            this.txtBox_beamformingMatrixHeaderFile_standard = new System.Windows.Forms.TextBox();
            this.button_browseBeamformingMatrixHeaderFile_standard = new System.Windows.Forms.Button();
            this.txtBox_beamformingMatrixValuesFile_standard = new System.Windows.Forms.TextBox();
            this.button_browseBeamformingMatrixValuesFile_standard = new System.Windows.Forms.Button();
            this.bf_tab_eht160 = new System.Windows.Forms.TabPage();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.button_writeBeamformingMatrixFile_160mhz = new System.Windows.Forms.Button();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.label15 = new System.Windows.Forms.Label();
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz = new System.Windows.Forms.TextBox();
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.txtBox_beamformingMatrixHeaderFile_160mhz = new System.Windows.Forms.TextBox();
            this.button_browseBeamformingMatrixHeaderFile_160mhz = new System.Windows.Forms.Button();
            this.txtBox_beamformingMatrixValuesFile_160mhz = new System.Windows.Forms.TextBox();
            this.button_browseBeamformingMatrixValuesFile_160mhz = new System.Windows.Forms.Button();
            this.bf_tab_eht320 = new System.Windows.Forms.TabPage();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.label16 = new System.Windows.Forms.Label();
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz = new System.Windows.Forms.TextBox();
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz = new System.Windows.Forms.Button();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.txtBox_beamformingMatrixHeaderFile_lower_320mhz = new System.Windows.Forms.TextBox();
            this.button_browseBeamformingMatrixHeaderFile_lower_320mhz = new System.Windows.Forms.Button();
            this.txtBox_beamformingMatrixValuesFile_lower_320mhz = new System.Windows.Forms.TextBox();
            this.button_browseBeamformingMatrixValuesFile_lower_320mhz = new System.Windows.Forms.Button();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel7 = new System.Windows.Forms.TableLayoutPanel();
            this.label19 = new System.Windows.Forms.Label();
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz = new System.Windows.Forms.TextBox();
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz = new System.Windows.Forms.Button();
            this.label20 = new System.Windows.Forms.Label();
            this.txtBox_beamformingMatrixHeaderFile_upper_320mhz = new System.Windows.Forms.TextBox();
            this.button_browseBeamformingMatrixHeaderFile_upper_320mhz = new System.Windows.Forms.Button();
            this.txtBox_beamformingMatrixValuesFile_upper_320mhz = new System.Windows.Forms.TextBox();
            this.button_browseBeamformingMatrixValuesFile_upper_320mhz = new System.Windows.Forms.Button();
            this.label21 = new System.Windows.Forms.Label();
            this.button_writeBeamformingMatrixFile_320mhz = new System.Windows.Forms.Button();
            this.groupBoxOther = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.textBoxTemperature = new System.Windows.Forms.TextBox();
            this.button_getTemperature = new System.Windows.Forms.Button();
            this.labelTemperature = new System.Windows.Forms.Label();
            this.groupBox_rwRegisters = new System.Windows.Forms.GroupBox();
            this.cmd_nvMemCtrl_writeReg = new System.Windows.Forms.Button();
            this.cmd_nvMemCtrl_readReg = new System.Windows.Forms.Button();
            this.table_nvMemCtrl_rwRegisters = new System.Windows.Forms.TableLayoutPanel();
            this.label13 = new System.Windows.Forms.Label();
            this.txtBox_nvMemCtrl_regModule = new System.Windows.Forms.TextBox();
            this.txtBox_nvMemCtrl_regAddress = new System.Windows.Forms.TextBox();
            this.lbl_nvMemCtrl_address = new System.Windows.Forms.Label();
            this.lbl_nvMemCtrl_val = new System.Windows.Forms.Label();
            this.lbl_nvMemCtrl_mask = new System.Windows.Forms.Label();
            this.txtBox_nvMemCtrl_regValue = new System.Windows.Forms.TextBox();
            this.txtBox_nvMemCtrl_regMask = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.cmd_nvMemCtrl_allRFRW = new System.Windows.Forms.Button();
            this.cmd_nvMemCtrl_writeRF = new System.Windows.Forms.Button();
            this.cmd_nvMemCtrl_readRF = new System.Windows.Forms.Button();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.lbl_nvMemCtrl_val4 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.txtBox_nvMemCtrl_RFval2 = new System.Windows.Forms.TextBox();
            this.txtBox_nvMemCtrl_RFaddress = new System.Windows.Forms.TextBox();
            this.txtBox_nvMemCtrl_RFval0 = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.txtBox_nvMemCtrl_RFval1 = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.txtBox_nvMemCtrl_RFval3 = new System.Windows.Forms.TextBox();
            this.txtBox_nvMemCtrl_RFval4 = new System.Windows.Forms.TextBox();
            this.groupBox_antennaPhase = new System.Windows.Forms.GroupBox();
            this.comboBox_extendedOp_antNum = new System.Windows.Forms.ComboBox();
            this.txtBox_extendedOp_offset2 = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.txtBox_extendedOp_offset3 = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.txtBox_extendedOp_offset1 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.cmd_extendedOp_CDDget = new System.Windows.Forms.Button();
            this.cmd_extendedOp_CDDset = new System.Windows.Forms.Button();
            this.groupBox_xtalCalib = new System.Windows.Forms.GroupBox();
            this.lbl_extendedOp_xtalReg = new System.Windows.Forms.Label();
            this.table_extendedOp_xtalRegister = new System.Windows.Forms.TableLayoutPanel();
            this.lbl_extendedOp_xtalVal_reg = new System.Windows.Forms.Label();
            this.num_extendedOp_xtalRegisterValue = new System.Windows.Forms.NumericUpDown();
            this.num_extendedOp_xtalRegisterBias = new System.Windows.Forms.NumericUpDown();
            this.lbl_extendedOp_xtalBias_reg = new System.Windows.Forms.Label();
            this.lbl_extendedOp_xtalBias = new System.Windows.Forms.Label();
            this.num_extendedOp_xtalCalBias = new System.Windows.Forms.NumericUpDown();
            this.lbl_extendedOp_xtalVal = new System.Windows.Forms.Label();
            this.num_extendedOp_xtalCalValue = new System.Windows.Forms.NumericUpDown();
            this.lbl_extendedOp_xtalRegAutoUpdate = new System.Windows.Forms.CheckBox();
            this.cmd_extendedOp_xtalCalWriteEEPROM = new System.Windows.Forms.Button();
            this.cmd_extendedOp_xtalRegisterRead = new System.Windows.Forms.Button();
            this.cmd_extendedOp_xtalCalReadEEPROM = new System.Windows.Forms.Button();
            this.check_extendedOp_xtalEnable = new System.Windows.Forms.CheckBox();
            this.cmd_extendedOp_xtalRegisterWrite = new System.Windows.Forms.Button();
            this.nvMemControlTab = new System.Windows.Forms.TabPage();
            this.groupBox_versionInfo = new System.Windows.Forms.GroupBox();
            this.groupBox_eepromInfo = new System.Windows.Forms.GroupBox();
            this.groupBox_nvMem_prodInfo = new System.Windows.Forms.GroupBox();
            this.combox_nvMemCtrl_prodValue = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.cmd_nvMemCtrl_burnProdFlag = new System.Windows.Forms.Button();
            this.combox_nvMemCtrl_prodFlag = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox_eepromBarParse = new System.Windows.Forms.GroupBox();
            this.table_nvMemCtrl_EEPROMbarcodeParsing = new System.Windows.Forms.TableLayoutPanel();
            this.lbl_nvMemCtrl_serial = new System.Windows.Forms.Label();
            this.lbl_nvMemCtrl_year = new System.Windows.Forms.Label();
            this.txtBox_nvMemCtrl_year = new System.Windows.Forms.TextBox();
            this.lbl_nvMemCtrl_week = new System.Windows.Forms.Label();
            this.txtBox_nvMemCtrl_week = new System.Windows.Forms.TextBox();
            this.txtBox_nvMemCtrl_serial = new System.Windows.Forms.TextBox();
            this.lbl_nvMemCtrl_country = new System.Windows.Forms.Label();
            this.txtBox_nvMemCtrl_country = new System.Windows.Forms.TextBox();
            this.burnBarcodeAndMAC = new System.Windows.Forms.Button();
            this.cmd_nvMemCtrl_readCalFileInfo = new System.Windows.Forms.Button();
            this.table_nvMemCtrl_currentEEPROM = new System.Windows.Forms.TableLayoutPanel();
            this.txtBox_nvMemCtrl_MACaddr = new System.Windows.Forms.TextBox();
            this.lbl_nvMemCtrl_MACaddr = new System.Windows.Forms.Label();
            this.cmd_nvMemCtrl_burnCountryCode = new System.Windows.Forms.Button();
            this.lbl_nvMemCtrl_countryCurrent = new System.Windows.Forms.Label();
            this.combox_nvMemCtrl_country = new System.Windows.Forms.ComboBox();
            this.check_nvMemCtrl_SNformatBarcode = new System.Windows.Forms.CheckBox();
            this.tpcTab = new System.Windows.Forms.TabPage();
            this.groupBox_calData = new System.Windows.Forms.GroupBox();
            this.cmd_tpc_collapseAll = new System.Windows.Forms.Button();
            this.cmd_tpc_expandAll = new System.Windows.Forms.Button();
            this.tree_tpc_tpc = new System.Windows.Forms.TreeView();
            this.txtBox_tpc_eepromVersion = new System.Windows.Forms.TextBox();
            this.lbl_tpc_eepromVersion = new System.Windows.Forms.Label();
            this.ToolTipIFS = new System.Windows.Forms.ToolTip(this.components);
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.openBeamformingMatrixHeaderFile_standard = new System.Windows.Forms.OpenFileDialog();
            this.openBeamformingMatrixHeaderFile_160mhz = new System.Windows.Forms.OpenFileDialog();
            this.openBeamformingMatrixValuesFile_160mhz = new System.Windows.Forms.OpenFileDialog();
            this.openBeamformingMatrixValuesFile_ehtExtra_160mhz = new System.Windows.Forms.OpenFileDialog();
            this.openBeamformingMatrixHeaderFile_lower_320mhz = new System.Windows.Forms.OpenFileDialog();
            this.openBeamformingMatrixHeaderFile_upper_320mhz = new System.Windows.Forms.OpenFileDialog();
            this.openBeamformingMatrixValuesFile_lower_320mhz = new System.Windows.Forms.OpenFileDialog();
            this.openBeamformingMatrixValuesFile_upper_320mhz = new System.Windows.Forms.OpenFileDialog();
            this.openBeamformingMatrixValuesFile_ehtExtra_upper_320mhz = new System.Windows.Forms.OpenFileDialog();
            this.openBeamformingMatrixValuesFile_ehtExtra_lower_320mhz = new System.Windows.Forms.OpenFileDialog();
            this.radio_basicOp_codingAuto = new System.Windows.Forms.RadioButton();
            this.table_nvMemCtrl_versionInfo.SuspendLayout();
            this.table_basicOp_antennas.SuspendLayout();
            this.groupBox_tx.SuspendLayout();
            this.table_basicOp_txALL.SuspendLayout();
            this.panel_basicOp_rate_cmd.SuspendLayout();
            this.panel_basicOp_rate_params.SuspendLayout();
            this.panel_spacing_params.SuspendLayout();
            this.panel_spacing_cmd.SuspendLayout();
            this.panel_txParams_cmd.SuspendLayout();
            this.panel_powerParam_cmd.SuspendLayout();
            this.panel_loop_cmd.SuspendLayout();
            this.panel_basicOp_channel_cmd.SuspendLayout();
            this.panel_basicOp_params.SuspendLayout();
            this.panel_basicOp_phy_param.SuspendLayout();
            this.panel_basicOp_txParams_params.SuspendLayout();
            this.panel_basicOp_channel_params.SuspendLayout();
            this.groupBox_burnEEPROM.SuspendLayout();
            this.table_nvMemCtrl_burnEEPROM.SuspendLayout();
            this.groupBox_genRisc.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLogo)).BeginInit();
            this.groupBox_connection.SuspendLayout();
            this.groupBox_WlanCard.SuspendLayout();
            this.tabControl.SuspendLayout();
            this.basicOperationTab.SuspendLayout();
            this.groupBox_RuParameters.SuspendLayout();
            this.groupBox_basicOp_spaceless.SuspendLayout();
            this.groupBox_cardID.SuspendLayout();
            this.groupBox_basicOp_misc.SuspendLayout();
            this.groupBox_antennasOnOff.SuspendLayout();
            this.extendedOperationTab.SuspendLayout();
            this.tableLayoutPanel6.SuspendLayout();
            this.bf_tabControl.SuspendLayout();
            this.bf_tab_default.SuspendLayout();
            this.bf_tab_standard.SuspendLayout();
            this.groupBox_WriteBeamformingMatrix.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.bf_tab_eht160.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            this.bf_tab_eht320.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.tableLayoutPanel5.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.tableLayoutPanel7.SuspendLayout();
            this.groupBoxOther.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.groupBox_rwRegisters.SuspendLayout();
            this.table_nvMemCtrl_rwRegisters.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.groupBox_antennaPhase.SuspendLayout();
            this.groupBox_xtalCalib.SuspendLayout();
            this.table_extendedOp_xtalRegister.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.num_extendedOp_xtalRegisterValue)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.num_extendedOp_xtalRegisterBias)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.num_extendedOp_xtalCalBias)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.num_extendedOp_xtalCalValue)).BeginInit();
            this.nvMemControlTab.SuspendLayout();
            this.groupBox_versionInfo.SuspendLayout();
            this.groupBox_eepromInfo.SuspendLayout();
            this.groupBox_nvMem_prodInfo.SuspendLayout();
            this.groupBox_eepromBarParse.SuspendLayout();
            this.table_nvMemCtrl_EEPROMbarcodeParsing.SuspendLayout();
            this.table_nvMemCtrl_currentEEPROM.SuspendLayout();
            this.tpcTab.SuspendLayout();
            this.groupBox_calData.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // dllConsole
            // 
            this.dllConsole.BackColor = System.Drawing.Color.GhostWhite;
            this.dllConsole.Location = new System.Drawing.Point(420, 1159);
            this.dllConsole.Margin = new System.Windows.Forms.Padding(4);
            this.dllConsole.Name = "dllConsole";
            this.dllConsole.ReadOnly = true;
            this.dllConsole.Size = new System.Drawing.Size(829, 246);
            this.dllConsole.TabIndex = 0;
            this.dllConsole.Text = "";
            // 
            // txtBox_nvMemCtrl_dutVersion
            // 
            this.txtBox_nvMemCtrl_dutVersion.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_dutVersion.BackColor = System.Drawing.Color.PaleGoldenrod;
            this.txtBox_nvMemCtrl_dutVersion.Location = new System.Drawing.Point(170, 14);
            this.txtBox_nvMemCtrl_dutVersion.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_dutVersion.Name = "txtBox_nvMemCtrl_dutVersion";
            this.txtBox_nvMemCtrl_dutVersion.ReadOnly = true;
            this.txtBox_nvMemCtrl_dutVersion.Size = new System.Drawing.Size(204, 26);
            this.txtBox_nvMemCtrl_dutVersion.TabIndex = 2;
            // 
            // lbl_nvMemCtrl_dutVersion
            // 
            this.lbl_nvMemCtrl_dutVersion.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_dutVersion.AutoSize = true;
            this.lbl_nvMemCtrl_dutVersion.Location = new System.Drawing.Point(51, 17);
            this.lbl_nvMemCtrl_dutVersion.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_dutVersion.Name = "lbl_nvMemCtrl_dutVersion";
            this.lbl_nvMemCtrl_dutVersion.Size = new System.Drawing.Size(111, 20);
            this.lbl_nvMemCtrl_dutVersion.TabIndex = 3;
            this.lbl_nvMemCtrl_dutVersion.Text = "DUT Version:";
            // 
            // table_nvMemCtrl_versionInfo
            // 
            this.table_nvMemCtrl_versionInfo.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.table_nvMemCtrl_versionInfo.BackColor = System.Drawing.Color.White;
            this.table_nvMemCtrl_versionInfo.ColumnCount = 2;
            this.table_nvMemCtrl_versionInfo.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 44.04762F));
            this.table_nvMemCtrl_versionInfo.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 55.95238F));
            this.table_nvMemCtrl_versionInfo.Controls.Add(this.txtBox_nvMemCtrl_BBchipVersion, 1, 2);
            this.table_nvMemCtrl_versionInfo.Controls.Add(this.txtBox_nvMemCtrl_RFchipVersion, 1, 1);
            this.table_nvMemCtrl_versionInfo.Controls.Add(this.lbl_nvMemCtrl_BBchip, 0, 2);
            this.table_nvMemCtrl_versionInfo.Controls.Add(this.lbl_nvMemCtrl_dutVersion, 0, 0);
            this.table_nvMemCtrl_versionInfo.Controls.Add(this.txtBox_nvMemCtrl_dutVersion, 1, 0);
            this.table_nvMemCtrl_versionInfo.Controls.Add(this.lbl_nvMemCtrl_RFchip, 0, 1);
            this.table_nvMemCtrl_versionInfo.Controls.Add(this.txtBox_nvMemCtrl_CVVersion, 1, 3);
            this.table_nvMemCtrl_versionInfo.Controls.Add(this.txtBox_nvMemCtrl_MoreCompVersion, 1, 4);
            this.table_nvMemCtrl_versionInfo.Controls.Add(this.lbl_nvMemCtrl_CV, 0, 3);
            this.table_nvMemCtrl_versionInfo.Controls.Add(this.combox_MoreComp, 0, 4);
            this.table_nvMemCtrl_versionInfo.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.table_nvMemCtrl_versionInfo.Location = new System.Drawing.Point(10, 28);
            this.table_nvMemCtrl_versionInfo.Margin = new System.Windows.Forms.Padding(4);
            this.table_nvMemCtrl_versionInfo.Name = "table_nvMemCtrl_versionInfo";
            this.table_nvMemCtrl_versionInfo.RowCount = 5;
            this.table_nvMemCtrl_versionInfo.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 35.80247F));
            this.table_nvMemCtrl_versionInfo.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 30.20833F));
            this.table_nvMemCtrl_versionInfo.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 34.375F));
            this.table_nvMemCtrl_versionInfo.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.table_nvMemCtrl_versionInfo.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 48F));
            this.table_nvMemCtrl_versionInfo.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.table_nvMemCtrl_versionInfo.Size = new System.Drawing.Size(378, 252);
            this.table_nvMemCtrl_versionInfo.TabIndex = 4;
            // 
            // txtBox_nvMemCtrl_BBchipVersion
            // 
            this.txtBox_nvMemCtrl_BBchipVersion.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_BBchipVersion.BackColor = System.Drawing.Color.PaleGoldenrod;
            this.txtBox_nvMemCtrl_BBchipVersion.Location = new System.Drawing.Point(170, 113);
            this.txtBox_nvMemCtrl_BBchipVersion.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_BBchipVersion.Name = "txtBox_nvMemCtrl_BBchipVersion";
            this.txtBox_nvMemCtrl_BBchipVersion.ReadOnly = true;
            this.txtBox_nvMemCtrl_BBchipVersion.Size = new System.Drawing.Size(204, 26);
            this.txtBox_nvMemCtrl_BBchipVersion.TabIndex = 7;
            // 
            // txtBox_nvMemCtrl_RFchipVersion
            // 
            this.txtBox_nvMemCtrl_RFchipVersion.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_RFchipVersion.BackColor = System.Drawing.Color.PaleGoldenrod;
            this.txtBox_nvMemCtrl_RFchipVersion.Location = new System.Drawing.Point(170, 64);
            this.txtBox_nvMemCtrl_RFchipVersion.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_RFchipVersion.Name = "txtBox_nvMemCtrl_RFchipVersion";
            this.txtBox_nvMemCtrl_RFchipVersion.ReadOnly = true;
            this.txtBox_nvMemCtrl_RFchipVersion.Size = new System.Drawing.Size(204, 26);
            this.txtBox_nvMemCtrl_RFchipVersion.TabIndex = 5;
            // 
            // lbl_nvMemCtrl_BBchip
            // 
            this.lbl_nvMemCtrl_BBchip.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_BBchip.AutoSize = true;
            this.lbl_nvMemCtrl_BBchip.Location = new System.Drawing.Point(23, 116);
            this.lbl_nvMemCtrl_BBchip.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_BBchip.Name = "lbl_nvMemCtrl_BBchip";
            this.lbl_nvMemCtrl_BBchip.Size = new System.Drawing.Size(139, 20);
            this.lbl_nvMemCtrl_BBchip.TabIndex = 6;
            this.lbl_nvMemCtrl_BBchip.Text = "BB Chip Version:";
            // 
            // lbl_nvMemCtrl_RFchip
            // 
            this.lbl_nvMemCtrl_RFchip.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_RFchip.AutoSize = true;
            this.lbl_nvMemCtrl_RFchip.Location = new System.Drawing.Point(25, 67);
            this.lbl_nvMemCtrl_RFchip.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_RFchip.Name = "lbl_nvMemCtrl_RFchip";
            this.lbl_nvMemCtrl_RFchip.Size = new System.Drawing.Size(137, 20);
            this.lbl_nvMemCtrl_RFchip.TabIndex = 3;
            this.lbl_nvMemCtrl_RFchip.Text = "RF Chip Version:";
            // 
            // txtBox_nvMemCtrl_CVVersion
            // 
            this.txtBox_nvMemCtrl_CVVersion.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_CVVersion.BackColor = System.Drawing.Color.PaleGoldenrod;
            this.txtBox_nvMemCtrl_CVVersion.Location = new System.Drawing.Point(170, 164);
            this.txtBox_nvMemCtrl_CVVersion.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_CVVersion.Name = "txtBox_nvMemCtrl_CVVersion";
            this.txtBox_nvMemCtrl_CVVersion.ReadOnly = true;
            this.txtBox_nvMemCtrl_CVVersion.Size = new System.Drawing.Size(204, 26);
            this.txtBox_nvMemCtrl_CVVersion.TabIndex = 8;
            // 
            // txtBox_nvMemCtrl_MoreCompVersion
            // 
            this.txtBox_nvMemCtrl_MoreCompVersion.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_MoreCompVersion.BackColor = System.Drawing.Color.PaleGoldenrod;
            this.txtBox_nvMemCtrl_MoreCompVersion.Location = new System.Drawing.Point(170, 214);
            this.txtBox_nvMemCtrl_MoreCompVersion.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_MoreCompVersion.Name = "txtBox_nvMemCtrl_MoreCompVersion";
            this.txtBox_nvMemCtrl_MoreCompVersion.ReadOnly = true;
            this.txtBox_nvMemCtrl_MoreCompVersion.Size = new System.Drawing.Size(204, 26);
            this.txtBox_nvMemCtrl_MoreCompVersion.TabIndex = 10;
            // 
            // lbl_nvMemCtrl_CV
            // 
            this.lbl_nvMemCtrl_CV.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_CV.AutoSize = true;
            this.lbl_nvMemCtrl_CV.Location = new System.Drawing.Point(11, 167);
            this.lbl_nvMemCtrl_CV.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_CV.Name = "lbl_nvMemCtrl_CV";
            this.lbl_nvMemCtrl_CV.Size = new System.Drawing.Size(151, 20);
            this.lbl_nvMemCtrl_CV.TabIndex = 13;
            this.lbl_nvMemCtrl_CV.Text = "Combined Version:";
            // 
            // combox_MoreComp
            // 
            this.combox_MoreComp.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.combox_MoreComp.FormattingEnabled = true;
            this.combox_MoreComp.Location = new System.Drawing.Point(8, 213);
            this.combox_MoreComp.Margin = new System.Windows.Forms.Padding(4);
            this.combox_MoreComp.Name = "combox_MoreComp";
            this.combox_MoreComp.Size = new System.Drawing.Size(154, 28);
            this.combox_MoreComp.TabIndex = 14;
            this.combox_MoreComp.Tag = "TAG_COMPVER";
            this.combox_MoreComp.Text = "More Component:";
            this.combox_MoreComp.SelectedIndexChanged += new System.EventHandler(this.combox_MoreComp_SelectedIndexChanged);
            // 
            // table_basicOp_antennas
            // 
            this.table_basicOp_antennas.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.table_basicOp_antennas.ColumnCount = 5;
            this.table_basicOp_antennas.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 20F));
            this.table_basicOp_antennas.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 20F));
            this.table_basicOp_antennas.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 20F));
            this.table_basicOp_antennas.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 20F));
            this.table_basicOp_antennas.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 20F));
            this.table_basicOp_antennas.Controls.Add(this.cmd_basicOp_txAnt0, 0, 0);
            this.table_basicOp_antennas.Controls.Add(this.cmd_basicOp_txAnt2, 2, 0);
            this.table_basicOp_antennas.Controls.Add(this.cmd_basicOp_rxAnt1, 1, 1);
            this.table_basicOp_antennas.Controls.Add(this.cmd_basicOp_txAnt1, 1, 0);
            this.table_basicOp_antennas.Controls.Add(this.cmd_basicOp_txAnt3, 3, 0);
            this.table_basicOp_antennas.Controls.Add(this.cmd_basicOp_rxAnt2, 2, 1);
            this.table_basicOp_antennas.Controls.Add(this.cmd_basicOp_rxAnt0, 0, 1);
            this.table_basicOp_antennas.Controls.Add(this.cmd_basicOp_rxAnt3, 3, 1);
            this.table_basicOp_antennas.Controls.Add(this.cmd_basicOp_rxAnt4, 4, 1);
            this.table_basicOp_antennas.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.table_basicOp_antennas.Location = new System.Drawing.Point(9, 27);
            this.table_basicOp_antennas.Margin = new System.Windows.Forms.Padding(4);
            this.table_basicOp_antennas.Name = "table_basicOp_antennas";
            this.table_basicOp_antennas.RowCount = 2;
            this.table_basicOp_antennas.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50.9434F));
            this.table_basicOp_antennas.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 49.0566F));
            this.table_basicOp_antennas.Size = new System.Drawing.Size(531, 88);
            this.table_basicOp_antennas.TabIndex = 5;
            // 
            // cmd_basicOp_txAnt0
            // 
            this.cmd_basicOp_txAnt0.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_basicOp_txAnt0.BackColor = System.Drawing.Color.CornflowerBlue;
            this.cmd_basicOp_txAnt0.Location = new System.Drawing.Point(4, 4);
            this.cmd_basicOp_txAnt0.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_txAnt0.Name = "cmd_basicOp_txAnt0";
            this.cmd_basicOp_txAnt0.Size = new System.Drawing.Size(98, 36);
            this.cmd_basicOp_txAnt0.TabIndex = 0;
            this.cmd_basicOp_txAnt0.Text = "TX Ant 0";
            this.cmd_basicOp_txAnt0.UseVisualStyleBackColor = false;
            this.cmd_basicOp_txAnt0.Click += new System.EventHandler(this.TXant0_Click);
            // 
            // cmd_basicOp_txAnt2
            // 
            this.cmd_basicOp_txAnt2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_basicOp_txAnt2.BackColor = System.Drawing.Color.CornflowerBlue;
            this.cmd_basicOp_txAnt2.Location = new System.Drawing.Point(216, 4);
            this.cmd_basicOp_txAnt2.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_txAnt2.Name = "cmd_basicOp_txAnt2";
            this.cmd_basicOp_txAnt2.Size = new System.Drawing.Size(98, 36);
            this.cmd_basicOp_txAnt2.TabIndex = 0;
            this.cmd_basicOp_txAnt2.Text = "TX Ant 2";
            this.cmd_basicOp_txAnt2.UseVisualStyleBackColor = false;
            this.cmd_basicOp_txAnt2.Click += new System.EventHandler(this.TXant2_Click);
            // 
            // cmd_basicOp_rxAnt1
            // 
            this.cmd_basicOp_rxAnt1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_basicOp_rxAnt1.BackColor = System.Drawing.Color.CornflowerBlue;
            this.cmd_basicOp_rxAnt1.Location = new System.Drawing.Point(110, 48);
            this.cmd_basicOp_rxAnt1.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_rxAnt1.Name = "cmd_basicOp_rxAnt1";
            this.cmd_basicOp_rxAnt1.Size = new System.Drawing.Size(98, 36);
            this.cmd_basicOp_rxAnt1.TabIndex = 0;
            this.cmd_basicOp_rxAnt1.Text = "RX Ant 1";
            this.cmd_basicOp_rxAnt1.UseVisualStyleBackColor = false;
            this.cmd_basicOp_rxAnt1.Click += new System.EventHandler(this.RXant1_Click);
            // 
            // cmd_basicOp_txAnt1
            // 
            this.cmd_basicOp_txAnt1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_basicOp_txAnt1.BackColor = System.Drawing.Color.CornflowerBlue;
            this.cmd_basicOp_txAnt1.Location = new System.Drawing.Point(110, 4);
            this.cmd_basicOp_txAnt1.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_txAnt1.Name = "cmd_basicOp_txAnt1";
            this.cmd_basicOp_txAnt1.Size = new System.Drawing.Size(98, 36);
            this.cmd_basicOp_txAnt1.TabIndex = 0;
            this.cmd_basicOp_txAnt1.Text = "TX Ant 1";
            this.cmd_basicOp_txAnt1.UseVisualStyleBackColor = false;
            this.cmd_basicOp_txAnt1.Click += new System.EventHandler(this.TXant1_Click);
            // 
            // cmd_basicOp_txAnt3
            // 
            this.cmd_basicOp_txAnt3.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_basicOp_txAnt3.BackColor = System.Drawing.Color.CornflowerBlue;
            this.cmd_basicOp_txAnt3.Location = new System.Drawing.Point(322, 4);
            this.cmd_basicOp_txAnt3.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_txAnt3.Name = "cmd_basicOp_txAnt3";
            this.cmd_basicOp_txAnt3.Size = new System.Drawing.Size(98, 36);
            this.cmd_basicOp_txAnt3.TabIndex = 1;
            this.cmd_basicOp_txAnt3.Text = "TX Ant 3";
            this.cmd_basicOp_txAnt3.UseVisualStyleBackColor = false;
            this.cmd_basicOp_txAnt3.Click += new System.EventHandler(this.TXant3_Click);
            // 
            // cmd_basicOp_rxAnt2
            // 
            this.cmd_basicOp_rxAnt2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_basicOp_rxAnt2.BackColor = System.Drawing.Color.CornflowerBlue;
            this.cmd_basicOp_rxAnt2.Location = new System.Drawing.Point(216, 48);
            this.cmd_basicOp_rxAnt2.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_rxAnt2.Name = "cmd_basicOp_rxAnt2";
            this.cmd_basicOp_rxAnt2.Size = new System.Drawing.Size(98, 36);
            this.cmd_basicOp_rxAnt2.TabIndex = 0;
            this.cmd_basicOp_rxAnt2.Text = "RX Ant 2";
            this.cmd_basicOp_rxAnt2.UseVisualStyleBackColor = false;
            this.cmd_basicOp_rxAnt2.Click += new System.EventHandler(this.RXant2_Click);
            // 
            // cmd_basicOp_rxAnt0
            // 
            this.cmd_basicOp_rxAnt0.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_basicOp_rxAnt0.BackColor = System.Drawing.Color.CornflowerBlue;
            this.cmd_basicOp_rxAnt0.Location = new System.Drawing.Point(4, 48);
            this.cmd_basicOp_rxAnt0.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_rxAnt0.Name = "cmd_basicOp_rxAnt0";
            this.cmd_basicOp_rxAnt0.Size = new System.Drawing.Size(98, 36);
            this.cmd_basicOp_rxAnt0.TabIndex = 0;
            this.cmd_basicOp_rxAnt0.Text = "RX Ant 0";
            this.cmd_basicOp_rxAnt0.UseVisualStyleBackColor = false;
            this.cmd_basicOp_rxAnt0.Click += new System.EventHandler(this.RXant0_Click);
            // 
            // cmd_basicOp_rxAnt3
            // 
            this.cmd_basicOp_rxAnt3.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_basicOp_rxAnt3.BackColor = System.Drawing.Color.CornflowerBlue;
            this.cmd_basicOp_rxAnt3.Location = new System.Drawing.Point(322, 48);
            this.cmd_basicOp_rxAnt3.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_rxAnt3.Name = "cmd_basicOp_rxAnt3";
            this.cmd_basicOp_rxAnt3.Size = new System.Drawing.Size(98, 36);
            this.cmd_basicOp_rxAnt3.TabIndex = 2;
            this.cmd_basicOp_rxAnt3.Text = "RX Ant 3";
            this.cmd_basicOp_rxAnt3.UseVisualStyleBackColor = false;
            this.cmd_basicOp_rxAnt3.Click += new System.EventHandler(this.RXant3_Click);
            // 
            // cmd_basicOp_rxAnt4
            // 
            this.cmd_basicOp_rxAnt4.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_basicOp_rxAnt4.BackColor = System.Drawing.Color.CornflowerBlue;
            this.cmd_basicOp_rxAnt4.Location = new System.Drawing.Point(428, 48);
            this.cmd_basicOp_rxAnt4.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_rxAnt4.Name = "cmd_basicOp_rxAnt4";
            this.cmd_basicOp_rxAnt4.Size = new System.Drawing.Size(99, 36);
            this.cmd_basicOp_rxAnt4.TabIndex = 3;
            this.cmd_basicOp_rxAnt4.Text = "RX Ant 4";
            this.cmd_basicOp_rxAnt4.UseVisualStyleBackColor = false;
            this.cmd_basicOp_rxAnt4.Click += new System.EventHandler(this.RXant4_Click);
            // 
            // groupBox_tx
            // 
            this.groupBox_tx.BackColor = System.Drawing.Color.White;
            this.groupBox_tx.Controls.Add(this.table_basicOp_txALL);
            this.groupBox_tx.Enabled = false;
            this.groupBox_tx.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox_tx.Location = new System.Drawing.Point(10, 9);
            this.groupBox_tx.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_tx.Name = "groupBox_tx";
            this.groupBox_tx.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_tx.Size = new System.Drawing.Size(651, 1040);
            this.groupBox_tx.TabIndex = 7;
            this.groupBox_tx.TabStop = false;
            this.groupBox_tx.Text = "TX";
            // 
            // table_basicOp_txALL
            // 
            this.table_basicOp_txALL.BackColor = System.Drawing.Color.White;
            this.table_basicOp_txALL.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this.table_basicOp_txALL.ColumnCount = 3;
            this.table_basicOp_txALL.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 20F));
            this.table_basicOp_txALL.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 58F));
            this.table_basicOp_txALL.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 22F));
            this.table_basicOp_txALL.Controls.Add(this.panel1, 2, 1);
            this.table_basicOp_txALL.Controls.Add(this.panel_basicOp_rate_cmd, 2, 3);
            this.table_basicOp_txALL.Controls.Add(this.lbl_basicOp_powerParam, 0, 4);
            this.table_basicOp_txALL.Controls.Add(this.lbl_basicOp_chanParam, 0, 2);
            this.table_basicOp_txALL.Controls.Add(this.panel_basicOp_rate_params, 1, 3);
            this.table_basicOp_txALL.Controls.Add(this.lbl_basicOp_rateParam, 0, 3);
            this.table_basicOp_txALL.Controls.Add(this.lbl_basicOp_spacing, 0, 5);
            this.table_basicOp_txALL.Controls.Add(this.panel_spacing_params, 1, 5);
            this.table_basicOp_txALL.Controls.Add(this.panel_spacing_cmd, 2, 5);
            this.table_basicOp_txALL.Controls.Add(this.panel_txParams_cmd, 2, 6);
            this.table_basicOp_txALL.Controls.Add(this.lbl_basicOp_txParam, 0, 6);
            this.table_basicOp_txALL.Controls.Add(this.panel_powerParam_cmd, 1, 4);
            this.table_basicOp_txALL.Controls.Add(this.panel_loop_cmd, 2, 4);
            this.table_basicOp_txALL.Controls.Add(this.panel_basicOp_channel_cmd, 2, 2);
            this.table_basicOp_txALL.Controls.Add(this.lbl_basicOp_band, 0, 0);
            this.table_basicOp_txALL.Controls.Add(this.panel_basicOp_params, 1, 0);
            this.table_basicOp_txALL.Controls.Add(this.lbl_basicOp_phy, 0, 1);
            this.table_basicOp_txALL.Controls.Add(this.panel_basicOp_phy_param, 1, 1);
            this.table_basicOp_txALL.Controls.Add(this.panel_basicOp_txParams_params, 1, 6);
            this.table_basicOp_txALL.Controls.Add(this.panel_basicOp_channel_params, 1, 2);
            this.table_basicOp_txALL.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.table_basicOp_txALL.Location = new System.Drawing.Point(4, 26);
            this.table_basicOp_txALL.Margin = new System.Windows.Forms.Padding(4);
            this.table_basicOp_txALL.Name = "table_basicOp_txALL";
            this.table_basicOp_txALL.RowCount = 7;
            this.table_basicOp_txALL.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 64F));
            this.table_basicOp_txALL.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 63F));
            this.table_basicOp_txALL.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 170F));
            this.table_basicOp_txALL.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.table_basicOp_txALL.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 158F));
            this.table_basicOp_txALL.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 88F));
            this.table_basicOp_txALL.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 122F));
            this.table_basicOp_txALL.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.table_basicOp_txALL.Size = new System.Drawing.Size(646, 1003);
            this.table_basicOp_txALL.TabIndex = 8;
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.Location = new System.Drawing.Point(507, 70);
            this.panel1.Margin = new System.Windows.Forms.Padding(4);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(134, 55);
            this.panel1.TabIndex = 22;
            // 
            // panel_basicOp_rate_cmd
            // 
            this.panel_basicOp_rate_cmd.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_basicOp_rate_cmd.AutoSize = true;
            this.panel_basicOp_rate_cmd.Controls.Add(this.cmd_basicOp_setRate);
            this.panel_basicOp_rate_cmd.Controls.Add(this.txtBox_basicOp_phyRate);
            this.panel_basicOp_rate_cmd.Controls.Add(this.lbl_basicOp_phyRate);
            this.panel_basicOp_rate_cmd.Location = new System.Drawing.Point(507, 305);
            this.panel_basicOp_rate_cmd.Margin = new System.Windows.Forms.Padding(4);
            this.panel_basicOp_rate_cmd.Name = "panel_basicOp_rate_cmd";
            this.panel_basicOp_rate_cmd.Size = new System.Drawing.Size(134, 222);
            this.panel_basicOp_rate_cmd.TabIndex = 21;
            // 
            // cmd_basicOp_setRate
            // 
            this.cmd_basicOp_setRate.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_basicOp_setRate.Location = new System.Drawing.Point(-2, 42);
            this.cmd_basicOp_setRate.Margin = new System.Windows.Forms.Padding(34, 4, 4, 4);
            this.cmd_basicOp_setRate.Name = "cmd_basicOp_setRate";
            this.cmd_basicOp_setRate.Size = new System.Drawing.Size(135, 52);
            this.cmd_basicOp_setRate.TabIndex = 19;
            this.cmd_basicOp_setRate.Text = "Set Rate";
            this.cmd_basicOp_setRate.UseVisualStyleBackColor = true;
            this.cmd_basicOp_setRate.Click += new System.EventHandler(this.SetRatebutton_Click);
            // 
            // txtBox_basicOp_phyRate
            // 
            this.txtBox_basicOp_phyRate.Location = new System.Drawing.Point(3, 138);
            this.txtBox_basicOp_phyRate.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_phyRate.Name = "txtBox_basicOp_phyRate";
            this.txtBox_basicOp_phyRate.Size = new System.Drawing.Size(126, 26);
            this.txtBox_basicOp_phyRate.TabIndex = 8;
            // 
            // lbl_basicOp_phyRate
            // 
            this.lbl_basicOp_phyRate.AutoSize = true;
            this.lbl_basicOp_phyRate.Location = new System.Drawing.Point(-2, 112);
            this.lbl_basicOp_phyRate.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_phyRate.Name = "lbl_basicOp_phyRate";
            this.lbl_basicOp_phyRate.Size = new System.Drawing.Size(146, 20);
            this.lbl_basicOp_phyRate.TabIndex = 7;
            this.lbl_basicOp_phyRate.Text = "PHY Rate (Mbps):";
            // 
            // lbl_basicOp_powerParam
            // 
            this.lbl_basicOp_powerParam.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_basicOp_powerParam.AutoSize = true;
            this.lbl_basicOp_powerParam.ForeColor = System.Drawing.Color.Black;
            this.lbl_basicOp_powerParam.Location = new System.Drawing.Point(5, 591);
            this.lbl_basicOp_powerParam.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_powerParam.Name = "lbl_basicOp_powerParam";
            this.lbl_basicOp_powerParam.Size = new System.Drawing.Size(96, 40);
            this.lbl_basicOp_powerParam.TabIndex = 28;
            this.lbl_basicOp_powerParam.Text = "Power Parameters";
            this.lbl_basicOp_powerParam.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lbl_basicOp_chanParam
            // 
            this.lbl_basicOp_chanParam.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_basicOp_chanParam.AutoSize = true;
            this.lbl_basicOp_chanParam.Location = new System.Drawing.Point(5, 195);
            this.lbl_basicOp_chanParam.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_chanParam.Name = "lbl_basicOp_chanParam";
            this.lbl_basicOp_chanParam.Size = new System.Drawing.Size(96, 40);
            this.lbl_basicOp_chanParam.TabIndex = 10;
            this.lbl_basicOp_chanParam.Text = "Channel Parameters";
            this.lbl_basicOp_chanParam.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // panel_basicOp_rate_params
            // 
            this.panel_basicOp_rate_params.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_basicOp_rate_params.AutoSize = true;
            this.panel_basicOp_rate_params.Controls.Add(this.combox_basicOp_signalBW);
            this.panel_basicOp_rate_params.Controls.Add(this.combox_basicOp_ltf);
            this.panel_basicOp_rate_params.Controls.Add(this.lbl_basicOp_LTF);
            this.panel_basicOp_rate_params.Controls.Add(this.combox_basicOp_GI);
            this.panel_basicOp_rate_params.Controls.Add(this.lbl_basicOp_signalBW);
            this.panel_basicOp_rate_params.Controls.Add(this.lbl_basicOp_GI);
            this.panel_basicOp_rate_params.Controls.Add(this.combox_basicOp_spatialStream);
            this.panel_basicOp_rate_params.Controls.Add(this.lbl_basicOp_spatialStream);
            this.panel_basicOp_rate_params.Controls.Add(this.combox_basicOp_MCS);
            this.panel_basicOp_rate_params.Controls.Add(this.lbl_basicOp_MCS);
            this.panel_basicOp_rate_params.Location = new System.Drawing.Point(134, 305);
            this.panel_basicOp_rate_params.Margin = new System.Windows.Forms.Padding(4);
            this.panel_basicOp_rate_params.Name = "panel_basicOp_rate_params";
            this.panel_basicOp_rate_params.Size = new System.Drawing.Size(364, 222);
            this.panel_basicOp_rate_params.TabIndex = 20;
            // 
            // combox_basicOp_signalBW
            // 
            this.combox_basicOp_signalBW.FormattingEnabled = true;
            this.combox_basicOp_signalBW.Location = new System.Drawing.Point(183, 12);
            this.combox_basicOp_signalBW.Margin = new System.Windows.Forms.Padding(4);
            this.combox_basicOp_signalBW.Name = "combox_basicOp_signalBW";
            this.combox_basicOp_signalBW.Size = new System.Drawing.Size(176, 28);
            this.combox_basicOp_signalBW.TabIndex = 22;
            this.combox_basicOp_signalBW.Tag = "TAG_SIGNALBW";
            this.combox_basicOp_signalBW.SelectedIndexChanged += new System.EventHandler(this.signalBW_SelectedIndexChanged);
            // 
            // combox_basicOp_ltf
            // 
            this.combox_basicOp_ltf.Cursor = System.Windows.Forms.Cursors.Default;
            this.combox_basicOp_ltf.FormattingEnabled = true;
            this.combox_basicOp_ltf.Location = new System.Drawing.Point(183, 176);
            this.combox_basicOp_ltf.Margin = new System.Windows.Forms.Padding(4);
            this.combox_basicOp_ltf.Name = "combox_basicOp_ltf";
            this.combox_basicOp_ltf.Size = new System.Drawing.Size(176, 28);
            this.combox_basicOp_ltf.TabIndex = 20;
            this.combox_basicOp_ltf.Tag = "TAG_LTF";
            this.combox_basicOp_ltf.SelectedIndexChanged += new System.EventHandler(this.basicOp_ltf_SelectedIndexChanged);
            // 
            // lbl_basicOp_LTF
            // 
            this.lbl_basicOp_LTF.AutoSize = true;
            this.lbl_basicOp_LTF.Location = new System.Drawing.Point(3, 180);
            this.lbl_basicOp_LTF.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_LTF.Name = "lbl_basicOp_LTF";
            this.lbl_basicOp_LTF.Size = new System.Drawing.Size(44, 20);
            this.lbl_basicOp_LTF.TabIndex = 12;
            this.lbl_basicOp_LTF.Text = "LTF:";
            // 
            // combox_basicOp_GI
            // 
            this.combox_basicOp_GI.Cursor = System.Windows.Forms.Cursors.Default;
            this.combox_basicOp_GI.FormattingEnabled = true;
            this.combox_basicOp_GI.Location = new System.Drawing.Point(183, 135);
            this.combox_basicOp_GI.Margin = new System.Windows.Forms.Padding(4);
            this.combox_basicOp_GI.Name = "combox_basicOp_GI";
            this.combox_basicOp_GI.Size = new System.Drawing.Size(176, 28);
            this.combox_basicOp_GI.TabIndex = 9;
            this.combox_basicOp_GI.Tag = "TAG_GI";
            this.combox_basicOp_GI.SelectedIndexChanged += new System.EventHandler(this.basicOp_GI_SelectedIndexChanged);
            // 
            // lbl_basicOp_signalBW
            // 
            this.lbl_basicOp_signalBW.Location = new System.Drawing.Point(3, 14);
            this.lbl_basicOp_signalBW.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_signalBW.Name = "lbl_basicOp_signalBW";
            this.lbl_basicOp_signalBW.Size = new System.Drawing.Size(110, 28);
            this.lbl_basicOp_signalBW.TabIndex = 7;
            this.lbl_basicOp_signalBW.Text = "Signal BW:\r\n";
            this.lbl_basicOp_signalBW.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lbl_basicOp_GI
            // 
            this.lbl_basicOp_GI.AutoSize = true;
            this.lbl_basicOp_GI.Location = new System.Drawing.Point(3, 140);
            this.lbl_basicOp_GI.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_GI.Name = "lbl_basicOp_GI";
            this.lbl_basicOp_GI.Size = new System.Drawing.Size(31, 20);
            this.lbl_basicOp_GI.TabIndex = 5;
            this.lbl_basicOp_GI.Text = "GI:";
            // 
            // combox_basicOp_spatialStream
            // 
            this.combox_basicOp_spatialStream.FormattingEnabled = true;
            this.combox_basicOp_spatialStream.Location = new System.Drawing.Point(183, 94);
            this.combox_basicOp_spatialStream.Margin = new System.Windows.Forms.Padding(4);
            this.combox_basicOp_spatialStream.Name = "combox_basicOp_spatialStream";
            this.combox_basicOp_spatialStream.Size = new System.Drawing.Size(176, 28);
            this.combox_basicOp_spatialStream.TabIndex = 1;
            this.combox_basicOp_spatialStream.Tag = "TAG_SPATIALSTREAM";
            this.combox_basicOp_spatialStream.SelectedIndexChanged += new System.EventHandler(this.ss_Changed);
            // 
            // lbl_basicOp_spatialStream
            // 
            this.lbl_basicOp_spatialStream.AutoSize = true;
            this.lbl_basicOp_spatialStream.Location = new System.Drawing.Point(3, 99);
            this.lbl_basicOp_spatialStream.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_spatialStream.Name = "lbl_basicOp_spatialStream";
            this.lbl_basicOp_spatialStream.Size = new System.Drawing.Size(133, 20);
            this.lbl_basicOp_spatialStream.TabIndex = 4;
            this.lbl_basicOp_spatialStream.Text = "Spatial Streams:";
            // 
            // combox_basicOp_MCS
            // 
            this.combox_basicOp_MCS.FormattingEnabled = true;
            this.combox_basicOp_MCS.Location = new System.Drawing.Point(183, 54);
            this.combox_basicOp_MCS.Margin = new System.Windows.Forms.Padding(4);
            this.combox_basicOp_MCS.Name = "combox_basicOp_MCS";
            this.combox_basicOp_MCS.Size = new System.Drawing.Size(176, 28);
            this.combox_basicOp_MCS.TabIndex = 3;
            this.combox_basicOp_MCS.Tag = "TAG_MCS";
            this.combox_basicOp_MCS.SelectedIndexChanged += new System.EventHandler(this.mcs_Changed);
            // 
            // lbl_basicOp_MCS
            // 
            this.lbl_basicOp_MCS.AutoSize = true;
            this.lbl_basicOp_MCS.Location = new System.Drawing.Point(3, 58);
            this.lbl_basicOp_MCS.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_MCS.Name = "lbl_basicOp_MCS";
            this.lbl_basicOp_MCS.Size = new System.Drawing.Size(51, 20);
            this.lbl_basicOp_MCS.TabIndex = 2;
            this.lbl_basicOp_MCS.Text = "MCS:";
            // 
            // lbl_basicOp_rateParam
            // 
            this.lbl_basicOp_rateParam.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_basicOp_rateParam.AutoSize = true;
            this.lbl_basicOp_rateParam.Location = new System.Drawing.Point(5, 396);
            this.lbl_basicOp_rateParam.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_rateParam.Name = "lbl_basicOp_rateParam";
            this.lbl_basicOp_rateParam.Size = new System.Drawing.Size(96, 40);
            this.lbl_basicOp_rateParam.TabIndex = 10;
            this.lbl_basicOp_rateParam.Text = "Rate Parameters";
            this.lbl_basicOp_rateParam.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lbl_basicOp_spacing
            // 
            this.lbl_basicOp_spacing.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_basicOp_spacing.AutoSize = true;
            this.lbl_basicOp_spacing.ForeColor = System.Drawing.Color.Black;
            this.lbl_basicOp_spacing.Location = new System.Drawing.Point(5, 725);
            this.lbl_basicOp_spacing.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_spacing.Name = "lbl_basicOp_spacing";
            this.lbl_basicOp_spacing.Size = new System.Drawing.Size(69, 20);
            this.lbl_basicOp_spacing.TabIndex = 22;
            this.lbl_basicOp_spacing.Text = "Spacing";
            this.lbl_basicOp_spacing.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // panel_spacing_params
            // 
            this.panel_spacing_params.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_spacing_params.Controls.Add(this.txtBox_basicOp_spacingIFS);
            this.panel_spacing_params.Controls.Add(this.lbl_basicOp_us);
            this.panel_spacing_params.Controls.Add(this.check_basicOp_closeRXants);
            this.panel_spacing_params.Location = new System.Drawing.Point(133, 694);
            this.panel_spacing_params.Name = "panel_spacing_params";
            this.panel_spacing_params.Size = new System.Drawing.Size(366, 82);
            this.panel_spacing_params.TabIndex = 23;
            // 
            // txtBox_basicOp_spacingIFS
            // 
            this.txtBox_basicOp_spacingIFS.BackColor = System.Drawing.Color.White;
            this.txtBox_basicOp_spacingIFS.Location = new System.Drawing.Point(10, 4);
            this.txtBox_basicOp_spacingIFS.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_spacingIFS.Name = "txtBox_basicOp_spacingIFS";
            this.txtBox_basicOp_spacingIFS.Size = new System.Drawing.Size(52, 26);
            this.txtBox_basicOp_spacingIFS.TabIndex = 7;
            this.ToolTipIFS.SetToolTip(this.txtBox_basicOp_spacingIFS, "Integer >=16");
            this.txtBox_basicOp_spacingIFS.Validating += new System.ComponentModel.CancelEventHandler(this.IFS_Validating);
            // 
            // lbl_basicOp_us
            // 
            this.lbl_basicOp_us.AutoSize = true;
            this.lbl_basicOp_us.Location = new System.Drawing.Point(69, 9);
            this.lbl_basicOp_us.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_us.Name = "lbl_basicOp_us";
            this.lbl_basicOp_us.Size = new System.Drawing.Size(38, 20);
            this.lbl_basicOp_us.TabIndex = 6;
            this.lbl_basicOp_us.Text = "[�s]";
            // 
            // check_basicOp_closeRXants
            // 
            this.check_basicOp_closeRXants.AutoSize = true;
            this.check_basicOp_closeRXants.Checked = true;
            this.check_basicOp_closeRXants.CheckState = System.Windows.Forms.CheckState.Checked;
            this.check_basicOp_closeRXants.Location = new System.Drawing.Point(10, 46);
            this.check_basicOp_closeRXants.Name = "check_basicOp_closeRXants";
            this.check_basicOp_closeRXants.Size = new System.Drawing.Size(181, 24);
            this.check_basicOp_closeRXants.TabIndex = 14;
            this.check_basicOp_closeRXants.Text = "Close RX Antennas";
            this.check_basicOp_closeRXants.UseVisualStyleBackColor = true;
            this.check_basicOp_closeRXants.CheckStateChanged += new System.EventHandler(this.check_basicOp_closeRXants_CheckStateChanged);
            // 
            // panel_spacing_cmd
            // 
            this.panel_spacing_cmd.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_spacing_cmd.Controls.Add(this.cmd_basicOp_setSpacing);
            this.panel_spacing_cmd.Location = new System.Drawing.Point(506, 694);
            this.panel_spacing_cmd.Name = "panel_spacing_cmd";
            this.panel_spacing_cmd.Size = new System.Drawing.Size(136, 82);
            this.panel_spacing_cmd.TabIndex = 21;
            // 
            // cmd_basicOp_setSpacing
            // 
            this.cmd_basicOp_setSpacing.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_basicOp_setSpacing.Location = new System.Drawing.Point(2, 12);
            this.cmd_basicOp_setSpacing.Margin = new System.Windows.Forms.Padding(34, 30, 4, 4);
            this.cmd_basicOp_setSpacing.Name = "cmd_basicOp_setSpacing";
            this.cmd_basicOp_setSpacing.Size = new System.Drawing.Size(135, 57);
            this.cmd_basicOp_setSpacing.TabIndex = 8;
            this.cmd_basicOp_setSpacing.Text = "Set Spacing";
            this.cmd_basicOp_setSpacing.UseVisualStyleBackColor = true;
            this.cmd_basicOp_setSpacing.Click += new System.EventHandler(this.setIFSbtn_Click);
            // 
            // panel_txParams_cmd
            // 
            this.panel_txParams_cmd.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_txParams_cmd.Controls.Add(this.cmd_basicOp_stopTransmission);
            this.panel_txParams_cmd.Controls.Add(this.cmd_basicOp_startTransmitting);
            this.panel_txParams_cmd.Location = new System.Drawing.Point(506, 783);
            this.panel_txParams_cmd.Name = "panel_txParams_cmd";
            this.panel_txParams_cmd.Size = new System.Drawing.Size(136, 216);
            this.panel_txParams_cmd.TabIndex = 21;
            // 
            // cmd_basicOp_stopTransmission
            // 
            this.cmd_basicOp_stopTransmission.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cmd_basicOp_stopTransmission.Enabled = false;
            this.cmd_basicOp_stopTransmission.Location = new System.Drawing.Point(1, 101);
            this.cmd_basicOp_stopTransmission.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_stopTransmission.Name = "cmd_basicOp_stopTransmission";
            this.cmd_basicOp_stopTransmission.Size = new System.Drawing.Size(135, 51);
            this.cmd_basicOp_stopTransmission.TabIndex = 9;
            this.cmd_basicOp_stopTransmission.Text = "Stop Tx";
            this.cmd_basicOp_stopTransmission.UseVisualStyleBackColor = true;
            this.cmd_basicOp_stopTransmission.Click += new System.EventHandler(this.stopTransmission_Click);
            // 
            // cmd_basicOp_startTransmitting
            // 
            this.cmd_basicOp_startTransmitting.Location = new System.Drawing.Point(1, 42);
            this.cmd_basicOp_startTransmitting.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_startTransmitting.Name = "cmd_basicOp_startTransmitting";
            this.cmd_basicOp_startTransmitting.Size = new System.Drawing.Size(135, 51);
            this.cmd_basicOp_startTransmitting.TabIndex = 9;
            this.cmd_basicOp_startTransmitting.Text = "Start Tx";
            this.cmd_basicOp_startTransmitting.UseVisualStyleBackColor = true;
            this.cmd_basicOp_startTransmitting.Click += new System.EventHandler(this.transmit_Click);
            // 
            // lbl_basicOp_txParam
            // 
            this.lbl_basicOp_txParam.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_basicOp_txParam.AutoSize = true;
            this.lbl_basicOp_txParam.Location = new System.Drawing.Point(5, 881);
            this.lbl_basicOp_txParam.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_txParam.Name = "lbl_basicOp_txParam";
            this.lbl_basicOp_txParam.Size = new System.Drawing.Size(119, 20);
            this.lbl_basicOp_txParam.TabIndex = 24;
            this.lbl_basicOp_txParam.Text = "Tx Parameters";
            this.lbl_basicOp_txParam.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // panel_powerParam_cmd
            // 
            this.panel_powerParam_cmd.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_powerParam_cmd.Controls.Add(this.txtBox_basicOp_powerLimit);
            this.panel_powerParam_cmd.Controls.Add(this.lbl_basicOp_powerLimit);
            this.panel_powerParam_cmd.Controls.Add(this.txtBox_basicOp_powerLevel);
            this.panel_powerParam_cmd.Controls.Add(this.lbl_basicOp_powerLevel);
            this.panel_powerParam_cmd.Controls.Add(this.checkBox_basicOp_autoPower);
            this.panel_powerParam_cmd.Controls.Add(this.checkBox_basicOp_closedLoop);
            this.panel_powerParam_cmd.Enabled = false;
            this.panel_powerParam_cmd.Location = new System.Drawing.Point(133, 535);
            this.panel_powerParam_cmd.Name = "panel_powerParam_cmd";
            this.panel_powerParam_cmd.Size = new System.Drawing.Size(366, 152);
            this.panel_powerParam_cmd.TabIndex = 26;
            // 
            // txtBox_basicOp_powerLimit
            // 
            this.txtBox_basicOp_powerLimit.Enabled = false;
            this.txtBox_basicOp_powerLimit.Location = new System.Drawing.Point(274, 76);
            this.txtBox_basicOp_powerLimit.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_powerLimit.Name = "txtBox_basicOp_powerLimit";
            this.txtBox_basicOp_powerLimit.Size = new System.Drawing.Size(84, 26);
            this.txtBox_basicOp_powerLimit.TabIndex = 9;
            this.txtBox_basicOp_powerLimit.TextChanged += new System.EventHandler(this.txtBox_basicOp_powerLimit_TextChanged);
            // 
            // lbl_basicOp_powerLimit
            // 
            this.lbl_basicOp_powerLimit.AutoSize = true;
            this.lbl_basicOp_powerLimit.Location = new System.Drawing.Point(6, 84);
            this.lbl_basicOp_powerLimit.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_powerLimit.Name = "lbl_basicOp_powerLimit";
            this.lbl_basicOp_powerLimit.Size = new System.Drawing.Size(210, 20);
            this.lbl_basicOp_powerLimit.TabIndex = 8;
            this.lbl_basicOp_powerLimit.Text = "Power Limit (eighth of dB):";
            // 
            // txtBox_basicOp_powerLevel
            // 
            this.txtBox_basicOp_powerLevel.Location = new System.Drawing.Point(274, 116);
            this.txtBox_basicOp_powerLevel.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_powerLevel.Name = "txtBox_basicOp_powerLevel";
            this.txtBox_basicOp_powerLevel.Size = new System.Drawing.Size(84, 26);
            this.txtBox_basicOp_powerLevel.TabIndex = 7;
            this.txtBox_basicOp_powerLevel.TextChanged += new System.EventHandler(this.txtBox_basicOp_powerLevel_TextChanged);
            // 
            // lbl_basicOp_powerLevel
            // 
            this.lbl_basicOp_powerLevel.AutoSize = true;
            this.lbl_basicOp_powerLevel.Location = new System.Drawing.Point(6, 123);
            this.lbl_basicOp_powerLevel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_powerLevel.Name = "lbl_basicOp_powerLevel";
            this.lbl_basicOp_powerLevel.Size = new System.Drawing.Size(289, 20);
            this.lbl_basicOp_powerLevel.TabIndex = 6;
            this.lbl_basicOp_powerLevel.Text = "Power Level Value (-10 to 31.5 dBm):";
            // 
            // checkBox_basicOp_autoPower
            // 
            this.checkBox_basicOp_autoPower.AutoSize = true;
            this.checkBox_basicOp_autoPower.Location = new System.Drawing.Point(10, 46);
            this.checkBox_basicOp_autoPower.Name = "checkBox_basicOp_autoPower";
            this.checkBox_basicOp_autoPower.Size = new System.Drawing.Size(120, 24);
            this.checkBox_basicOp_autoPower.TabIndex = 5;
            this.checkBox_basicOp_autoPower.Text = "Auto-power";
            this.checkBox_basicOp_autoPower.UseVisualStyleBackColor = true;
            this.checkBox_basicOp_autoPower.CheckedChanged += new System.EventHandler(this.checkBox_basicOp_autoPower_CheckedChanged);
            // 
            // checkBox_basicOp_closedLoop
            // 
            this.checkBox_basicOp_closedLoop.AutoSize = true;
            this.checkBox_basicOp_closedLoop.Location = new System.Drawing.Point(10, 14);
            this.checkBox_basicOp_closedLoop.Name = "checkBox_basicOp_closedLoop";
            this.checkBox_basicOp_closedLoop.Size = new System.Drawing.Size(129, 24);
            this.checkBox_basicOp_closedLoop.TabIndex = 4;
            this.checkBox_basicOp_closedLoop.Text = "Closed Loop";
            this.checkBox_basicOp_closedLoop.UseVisualStyleBackColor = true;
            this.checkBox_basicOp_closedLoop.CheckedChanged += new System.EventHandler(this.checkBox_basicOp_closedLoop_CheckedChanged);
            // 
            // panel_loop_cmd
            // 
            this.panel_loop_cmd.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_loop_cmd.Controls.Add(this.cmd_basicOp_setPower);
            this.panel_loop_cmd.Enabled = false;
            this.panel_loop_cmd.Location = new System.Drawing.Point(507, 536);
            this.panel_loop_cmd.Margin = new System.Windows.Forms.Padding(4);
            this.panel_loop_cmd.Name = "panel_loop_cmd";
            this.panel_loop_cmd.Size = new System.Drawing.Size(134, 150);
            this.panel_loop_cmd.TabIndex = 27;
            // 
            // cmd_basicOp_setPower
            // 
            this.cmd_basicOp_setPower.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.cmd_basicOp_setPower.Location = new System.Drawing.Point(0, 48);
            this.cmd_basicOp_setPower.Margin = new System.Windows.Forms.Padding(34, 30, 4, 4);
            this.cmd_basicOp_setPower.Name = "cmd_basicOp_setPower";
            this.cmd_basicOp_setPower.Size = new System.Drawing.Size(135, 51);
            this.cmd_basicOp_setPower.TabIndex = 26;
            this.cmd_basicOp_setPower.Text = "Set Power";
            this.cmd_basicOp_setPower.UseVisualStyleBackColor = true;
            this.cmd_basicOp_setPower.Click += new System.EventHandler(this.setPower_Click);
            // 
            // panel_basicOp_channel_cmd
            // 
            this.panel_basicOp_channel_cmd.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_basicOp_channel_cmd.Controls.Add(this.cmd_basicOp_setChannel);
            this.panel_basicOp_channel_cmd.Location = new System.Drawing.Point(507, 134);
            this.panel_basicOp_channel_cmd.Margin = new System.Windows.Forms.Padding(4);
            this.panel_basicOp_channel_cmd.Name = "panel_basicOp_channel_cmd";
            this.panel_basicOp_channel_cmd.Size = new System.Drawing.Size(134, 162);
            this.panel_basicOp_channel_cmd.TabIndex = 21;
            // 
            // cmd_basicOp_setChannel
            // 
            this.cmd_basicOp_setChannel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_basicOp_setChannel.ForeColor = System.Drawing.SystemColors.ControlText;
            this.cmd_basicOp_setChannel.Location = new System.Drawing.Point(-2, 49);
            this.cmd_basicOp_setChannel.Margin = new System.Windows.Forms.Padding(34, 4, 4, 4);
            this.cmd_basicOp_setChannel.Name = "cmd_basicOp_setChannel";
            this.cmd_basicOp_setChannel.Size = new System.Drawing.Size(135, 51);
            this.cmd_basicOp_setChannel.TabIndex = 9;
            this.cmd_basicOp_setChannel.Text = "Set Channel";
            this.cmd_basicOp_setChannel.UseVisualStyleBackColor = true;
            this.cmd_basicOp_setChannel.Click += new System.EventHandler(this.setChannel_Click);
            // 
            // lbl_basicOp_band
            // 
            this.lbl_basicOp_band.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_basicOp_band.AutoSize = true;
            this.lbl_basicOp_band.Location = new System.Drawing.Point(5, 23);
            this.lbl_basicOp_band.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_band.Name = "lbl_basicOp_band";
            this.lbl_basicOp_band.Size = new System.Drawing.Size(48, 20);
            this.lbl_basicOp_band.TabIndex = 29;
            this.lbl_basicOp_band.Text = "Band";
            // 
            // panel_basicOp_params
            // 
            this.panel_basicOp_params.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_basicOp_params.Controls.Add(this.lbl_basicOp_band_choose);
            this.panel_basicOp_params.Controls.Add(this.combox_basicOp_band);
            this.panel_basicOp_params.Location = new System.Drawing.Point(134, 5);
            this.panel_basicOp_params.Margin = new System.Windows.Forms.Padding(4);
            this.panel_basicOp_params.Name = "panel_basicOp_params";
            this.panel_basicOp_params.Size = new System.Drawing.Size(364, 56);
            this.panel_basicOp_params.TabIndex = 32;
            // 
            // lbl_basicOp_band_choose
            // 
            this.lbl_basicOp_band_choose.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_basicOp_band_choose.AutoSize = true;
            this.lbl_basicOp_band_choose.Location = new System.Drawing.Point(3, 15);
            this.lbl_basicOp_band_choose.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_band_choose.Name = "lbl_basicOp_band_choose";
            this.lbl_basicOp_band_choose.Size = new System.Drawing.Size(53, 20);
            this.lbl_basicOp_band_choose.TabIndex = 32;
            this.lbl_basicOp_band_choose.Text = "Band:";
            // 
            // combox_basicOp_band
            // 
            this.combox_basicOp_band.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.combox_basicOp_band.FormattingEnabled = true;
            this.combox_basicOp_band.Location = new System.Drawing.Point(183, 10);
            this.combox_basicOp_band.Margin = new System.Windows.Forms.Padding(4);
            this.combox_basicOp_band.Name = "combox_basicOp_band";
            this.combox_basicOp_band.Size = new System.Drawing.Size(176, 28);
            this.combox_basicOp_band.TabIndex = 31;
            this.combox_basicOp_band.Tag = "TAG_BAND";
            // 
            // lbl_basicOp_phy
            // 
            this.lbl_basicOp_phy.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_basicOp_phy.AutoSize = true;
            this.lbl_basicOp_phy.Location = new System.Drawing.Point(5, 87);
            this.lbl_basicOp_phy.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_phy.Name = "lbl_basicOp_phy";
            this.lbl_basicOp_phy.Size = new System.Drawing.Size(37, 20);
            this.lbl_basicOp_phy.TabIndex = 34;
            this.lbl_basicOp_phy.Text = "Phy";
            // 
            // panel_basicOp_phy_param
            // 
            this.panel_basicOp_phy_param.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_basicOp_phy_param.Controls.Add(this.combox_basicOp_phyType);
            this.panel_basicOp_phy_param.Controls.Add(this.lbl_basicOp_phy_choose);
            this.panel_basicOp_phy_param.Location = new System.Drawing.Point(134, 70);
            this.panel_basicOp_phy_param.Margin = new System.Windows.Forms.Padding(4);
            this.panel_basicOp_phy_param.Name = "panel_basicOp_phy_param";
            this.panel_basicOp_phy_param.Size = new System.Drawing.Size(364, 55);
            this.panel_basicOp_phy_param.TabIndex = 35;
            // 
            // combox_basicOp_phyType
            // 
            this.combox_basicOp_phyType.FormattingEnabled = true;
            this.combox_basicOp_phyType.Location = new System.Drawing.Point(183, 12);
            this.combox_basicOp_phyType.Margin = new System.Windows.Forms.Padding(4);
            this.combox_basicOp_phyType.Name = "combox_basicOp_phyType";
            this.combox_basicOp_phyType.Size = new System.Drawing.Size(176, 28);
            this.combox_basicOp_phyType.TabIndex = 23;
            this.combox_basicOp_phyType.Tag = "TAG_PHYTYPE";
            this.combox_basicOp_phyType.SelectedIndexChanged += new System.EventHandler(this.combox_basicOp_phyType_SelectedIndexChanged);
            // 
            // lbl_basicOp_phy_choose
            // 
            this.lbl_basicOp_phy_choose.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_basicOp_phy_choose.AutoSize = true;
            this.lbl_basicOp_phy_choose.Location = new System.Drawing.Point(3, 18);
            this.lbl_basicOp_phy_choose.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_phy_choose.Name = "lbl_basicOp_phy_choose";
            this.lbl_basicOp_phy_choose.Size = new System.Drawing.Size(83, 20);
            this.lbl_basicOp_phy_choose.TabIndex = 22;
            this.lbl_basicOp_phy_choose.Text = "Phy Type:";
            this.lbl_basicOp_phy_choose.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // panel_basicOp_txParams_params
            // 
            this.panel_basicOp_txParams_params.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_basicOp_txParams_params.Controls.Add(this.radio_basicOp_codingAuto);
            this.panel_basicOp_txParams_params.Controls.Add(this.radio_basicOp_codingLDPC);
            this.panel_basicOp_txParams_params.Controls.Add(this.radio_basicOp_codingBCC);
            this.panel_basicOp_txParams_params.Controls.Add(this.lbl_basicOp_codingType);
            this.panel_basicOp_txParams_params.Controls.Add(this.check_basicOp_numSymbols);
            this.panel_basicOp_txParams_params.Controls.Add(this.txtBox_basicOp_numSymbols);
            this.panel_basicOp_txParams_params.Controls.Add(this.check_basicOp_txBeamforming);
            this.panel_basicOp_txParams_params.Controls.Add(this.check_basicOp_txLongData);
            this.panel_basicOp_txParams_params.Controls.Add(this.txtBox_basicOp_repetitions);
            this.panel_basicOp_txParams_params.Controls.Add(this.lbl_basicOp_repetitions);
            this.panel_basicOp_txParams_params.Controls.Add(this.lbl_basicOp_packetLen);
            this.panel_basicOp_txParams_params.Controls.Add(this.txtBox_basicOp_packetLen);
            this.panel_basicOp_txParams_params.Location = new System.Drawing.Point(133, 783);
            this.panel_basicOp_txParams_params.Name = "panel_basicOp_txParams_params";
            this.panel_basicOp_txParams_params.Size = new System.Drawing.Size(366, 216);
            this.panel_basicOp_txParams_params.TabIndex = 15;
            // 
            // radio_basicOp_codingLDPC
            // 
            this.radio_basicOp_codingLDPC.AutoSize = true;
            this.radio_basicOp_codingLDPC.Location = new System.Drawing.Point(274, 169);
            this.radio_basicOp_codingLDPC.Name = "radio_basicOp_codingLDPC";
            this.radio_basicOp_codingLDPC.Size = new System.Drawing.Size(80, 24);
            this.radio_basicOp_codingLDPC.TabIndex = 21;
            this.radio_basicOp_codingLDPC.TabStop = true;
            this.radio_basicOp_codingLDPC.Text = "LDPC";
            this.radio_basicOp_codingLDPC.UseVisualStyleBackColor = true;
            // 
            // radio_basicOp_codingBCC
            // 
            this.radio_basicOp_codingBCC.AutoSize = true;
            this.radio_basicOp_codingBCC.Location = new System.Drawing.Point(193, 169);
            this.radio_basicOp_codingBCC.Name = "radio_basicOp_codingBCC";
            this.radio_basicOp_codingBCC.Size = new System.Drawing.Size(70, 24);
            this.radio_basicOp_codingBCC.TabIndex = 20;
            this.radio_basicOp_codingBCC.TabStop = true;
            this.radio_basicOp_codingBCC.Text = "BCC";
            this.radio_basicOp_codingBCC.UseVisualStyleBackColor = true;
            // 
            // lbl_basicOp_codingType
            // 
            this.lbl_basicOp_codingType.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_basicOp_codingType.AutoSize = true;
            this.lbl_basicOp_codingType.Location = new System.Drawing.Point(9, 171);
            this.lbl_basicOp_codingType.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_codingType.Name = "lbl_basicOp_codingType";
            this.lbl_basicOp_codingType.Size = new System.Drawing.Size(107, 20);
            this.lbl_basicOp_codingType.TabIndex = 19;
            this.lbl_basicOp_codingType.Text = "Coding Type:";
            this.lbl_basicOp_codingType.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // check_basicOp_numSymbols
            // 
            this.check_basicOp_numSymbols.AutoSize = true;
            this.check_basicOp_numSymbols.Location = new System.Drawing.Point(10, 128);
            this.check_basicOp_numSymbols.Name = "check_basicOp_numSymbols";
            this.check_basicOp_numSymbols.Size = new System.Drawing.Size(194, 24);
            this.check_basicOp_numSymbols.TabIndex = 18;
            this.check_basicOp_numSymbols.Text = "OFDM data symbols:";
            this.check_basicOp_numSymbols.UseVisualStyleBackColor = true;
            this.check_basicOp_numSymbols.CheckedChanged += new System.EventHandler(this.check_basicOp_numSymbols_CheckedChanged);
            // 
            // txtBox_basicOp_numSymbols
            // 
            this.txtBox_basicOp_numSymbols.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_basicOp_numSymbols.BackColor = System.Drawing.SystemColors.Window;
            this.txtBox_basicOp_numSymbols.Location = new System.Drawing.Point(210, 126);
            this.txtBox_basicOp_numSymbols.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_numSymbols.Name = "txtBox_basicOp_numSymbols";
            this.txtBox_basicOp_numSymbols.Size = new System.Drawing.Size(156, 26);
            this.txtBox_basicOp_numSymbols.TabIndex = 17;
            this.txtBox_basicOp_numSymbols.Leave += new System.EventHandler(this.txtBox_basicOp_numSymbols_Leave);
            // 
            // check_basicOp_txBeamforming
            // 
            this.check_basicOp_txBeamforming.AutoSize = true;
            this.check_basicOp_txBeamforming.Location = new System.Drawing.Point(132, 14);
            this.check_basicOp_txBeamforming.Margin = new System.Windows.Forms.Padding(4);
            this.check_basicOp_txBeamforming.Name = "check_basicOp_txBeamforming";
            this.check_basicOp_txBeamforming.Size = new System.Drawing.Size(135, 24);
            this.check_basicOp_txBeamforming.TabIndex = 15;
            this.check_basicOp_txBeamforming.Text = "Beamforming";
            this.check_basicOp_txBeamforming.UseVisualStyleBackColor = true;
            // 
            // check_basicOp_txLongData
            // 
            this.check_basicOp_txLongData.AutoSize = true;
            this.check_basicOp_txLongData.Location = new System.Drawing.Point(10, 14);
            this.check_basicOp_txLongData.Margin = new System.Windows.Forms.Padding(4);
            this.check_basicOp_txLongData.Name = "check_basicOp_txLongData";
            this.check_basicOp_txLongData.Size = new System.Drawing.Size(109, 24);
            this.check_basicOp_txLongData.TabIndex = 14;
            this.check_basicOp_txLongData.Text = "Long data";
            this.check_basicOp_txLongData.UseVisualStyleBackColor = true;
            this.check_basicOp_txLongData.CheckedChanged += new System.EventHandler(this.check_basicOp_txLongData_CheckedChanged);
            // 
            // txtBox_basicOp_repetitions
            // 
            this.txtBox_basicOp_repetitions.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_basicOp_repetitions.BackColor = System.Drawing.SystemColors.Window;
            this.txtBox_basicOp_repetitions.Location = new System.Drawing.Point(210, 46);
            this.txtBox_basicOp_repetitions.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_repetitions.Name = "txtBox_basicOp_repetitions";
            this.txtBox_basicOp_repetitions.Size = new System.Drawing.Size(156, 26);
            this.txtBox_basicOp_repetitions.TabIndex = 13;
            this.txtBox_basicOp_repetitions.TextChanged += new System.EventHandler(this.repetitions_TextChanged);
            this.txtBox_basicOp_repetitions.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtBoxRepetitions_keypress);
            // 
            // lbl_basicOp_repetitions
            // 
            this.lbl_basicOp_repetitions.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_basicOp_repetitions.AutoSize = true;
            this.lbl_basicOp_repetitions.Location = new System.Drawing.Point(6, 50);
            this.lbl_basicOp_repetitions.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_repetitions.Name = "lbl_basicOp_repetitions";
            this.lbl_basicOp_repetitions.Size = new System.Drawing.Size(98, 20);
            this.lbl_basicOp_repetitions.TabIndex = 12;
            this.lbl_basicOp_repetitions.Text = "Repetitions:";
            this.lbl_basicOp_repetitions.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lbl_basicOp_packetLen
            // 
            this.lbl_basicOp_packetLen.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_basicOp_packetLen.AutoSize = true;
            this.lbl_basicOp_packetLen.Location = new System.Drawing.Point(6, 90);
            this.lbl_basicOp_packetLen.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_packetLen.Name = "lbl_basicOp_packetLen";
            this.lbl_basicOp_packetLen.Size = new System.Drawing.Size(172, 20);
            this.lbl_basicOp_packetLen.TabIndex = 8;
            this.lbl_basicOp_packetLen.Text = "Packet length (bytes):";
            this.lbl_basicOp_packetLen.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // txtBox_basicOp_packetLen
            // 
            this.txtBox_basicOp_packetLen.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_basicOp_packetLen.BackColor = System.Drawing.SystemColors.Window;
            this.txtBox_basicOp_packetLen.Location = new System.Drawing.Point(210, 86);
            this.txtBox_basicOp_packetLen.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_packetLen.Name = "txtBox_basicOp_packetLen";
            this.txtBox_basicOp_packetLen.Size = new System.Drawing.Size(156, 26);
            this.txtBox_basicOp_packetLen.TabIndex = 9;
            this.txtBox_basicOp_packetLen.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtBoxPacketLen_keypress);
            // 
            // panel_basicOp_channel_params
            // 
            this.panel_basicOp_channel_params.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_basicOp_channel_params.BackColor = System.Drawing.Color.White;
            this.panel_basicOp_channel_params.Controls.Add(this.combox_basicOp_regulationType);
            this.panel_basicOp_channel_params.Controls.Add(this.label_basicOp_regulationType);
            this.panel_basicOp_channel_params.Controls.Add(this.combox_basicOp_primaryChannelIndex);
            this.panel_basicOp_channel_params.Controls.Add(this.combox_basicOp_spectrumBW);
            this.panel_basicOp_channel_params.Controls.Add(this.lbl_basicOp_spectrumBW);
            this.panel_basicOp_channel_params.Controls.Add(this.combox_basicOp_lowChan);
            this.panel_basicOp_channel_params.Controls.Add(this.txtBox_basicOp_CenterFreqMHz);
            this.panel_basicOp_channel_params.Controls.Add(this.lbl_basicOp_CenterFreqMHz);
            this.panel_basicOp_channel_params.Controls.Add(this.lbl_basicOp_lowChan);
            this.panel_basicOp_channel_params.Location = new System.Drawing.Point(133, 133);
            this.panel_basicOp_channel_params.Name = "panel_basicOp_channel_params";
            this.panel_basicOp_channel_params.Size = new System.Drawing.Size(366, 164);
            this.panel_basicOp_channel_params.TabIndex = 17;
            // 
            // combox_basicOp_regulationType
            // 
            this.combox_basicOp_regulationType.FormattingEnabled = true;
            this.combox_basicOp_regulationType.Location = new System.Drawing.Point(184, 126);
            this.combox_basicOp_regulationType.Name = "combox_basicOp_regulationType";
            this.combox_basicOp_regulationType.Size = new System.Drawing.Size(176, 28);
            this.combox_basicOp_regulationType.TabIndex = 24;
            this.combox_basicOp_regulationType.Tag = "TAG_REGULATION_TYPE";
            // 
            // label_basicOp_regulationType
            // 
            this.label_basicOp_regulationType.AutoSize = true;
            this.label_basicOp_regulationType.Location = new System.Drawing.Point(3, 132);
            this.label_basicOp_regulationType.Name = "label_basicOp_regulationType";
            this.label_basicOp_regulationType.Size = new System.Drawing.Size(134, 20);
            this.label_basicOp_regulationType.TabIndex = 23;
            this.label_basicOp_regulationType.Text = "Regulation Type:";
            // 
            // combox_basicOp_primaryChannelIndex
            // 
            this.combox_basicOp_primaryChannelIndex.BackColor = System.Drawing.SystemColors.Window;
            this.combox_basicOp_primaryChannelIndex.FormattingEnabled = true;
            this.combox_basicOp_primaryChannelIndex.Items.AddRange(new object[] {
            "0",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15"});
            this.combox_basicOp_primaryChannelIndex.Location = new System.Drawing.Point(296, 6);
            this.combox_basicOp_primaryChannelIndex.Name = "combox_basicOp_primaryChannelIndex";
            this.combox_basicOp_primaryChannelIndex.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.combox_basicOp_primaryChannelIndex.Size = new System.Drawing.Size(66, 28);
            this.combox_basicOp_primaryChannelIndex.TabIndex = 22;
            this.combox_basicOp_primaryChannelIndex.SelectedIndexChanged += new System.EventHandler(this.combox_basicOp_primaryChannelIndex_SelectedIndexChanged);
            this.combox_basicOp_primaryChannelIndex.Leave += new System.EventHandler(this.combox_basicOp_primaryChannelIndex_Leave);
            // 
            // combox_basicOp_spectrumBW
            // 
            this.combox_basicOp_spectrumBW.FormattingEnabled = true;
            this.combox_basicOp_spectrumBW.Location = new System.Drawing.Point(184, 86);
            this.combox_basicOp_spectrumBW.Margin = new System.Windows.Forms.Padding(4);
            this.combox_basicOp_spectrumBW.Name = "combox_basicOp_spectrumBW";
            this.combox_basicOp_spectrumBW.Size = new System.Drawing.Size(176, 28);
            this.combox_basicOp_spectrumBW.TabIndex = 10;
            this.combox_basicOp_spectrumBW.Tag = "TAG_SPECTRUMBW";
            this.combox_basicOp_spectrumBW.SelectedIndexChanged += new System.EventHandler(this.comboSpectrumBW_SelectedIndexChanged);
            // 
            // lbl_basicOp_spectrumBW
            // 
            this.lbl_basicOp_spectrumBW.Location = new System.Drawing.Point(3, 88);
            this.lbl_basicOp_spectrumBW.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_spectrumBW.Name = "lbl_basicOp_spectrumBW";
            this.lbl_basicOp_spectrumBW.Size = new System.Drawing.Size(118, 20);
            this.lbl_basicOp_spectrumBW.TabIndex = 7;
            this.lbl_basicOp_spectrumBW.Text = "Spectrum BW:";
            this.lbl_basicOp_spectrumBW.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // combox_basicOp_lowChan
            // 
            this.combox_basicOp_lowChan.BackColor = System.Drawing.SystemColors.Window;
            this.combox_basicOp_lowChan.FormattingEnabled = true;
            this.combox_basicOp_lowChan.Location = new System.Drawing.Point(184, 6);
            this.combox_basicOp_lowChan.Name = "combox_basicOp_lowChan";
            this.combox_basicOp_lowChan.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.combox_basicOp_lowChan.Size = new System.Drawing.Size(102, 28);
            this.combox_basicOp_lowChan.TabIndex = 3;
            this.combox_basicOp_lowChan.SelectedIndexChanged += new System.EventHandler(this.channel_TextChanged);
            this.combox_basicOp_lowChan.TextChanged += new System.EventHandler(this.channel_TextChanged);
            // 
            // txtBox_basicOp_CenterFreqMHz
            // 
            this.txtBox_basicOp_CenterFreqMHz.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_basicOp_CenterFreqMHz.BackColor = System.Drawing.SystemColors.Window;
            this.txtBox_basicOp_CenterFreqMHz.Location = new System.Drawing.Point(184, 45);
            this.txtBox_basicOp_CenterFreqMHz.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_CenterFreqMHz.Name = "txtBox_basicOp_CenterFreqMHz";
            this.txtBox_basicOp_CenterFreqMHz.Size = new System.Drawing.Size(178, 26);
            this.txtBox_basicOp_CenterFreqMHz.TabIndex = 2;
            this.txtBox_basicOp_CenterFreqMHz.TextChanged += new System.EventHandler(this.CenterFreqMHz_TextChanged);
            // 
            // lbl_basicOp_CenterFreqMHz
            // 
            this.lbl_basicOp_CenterFreqMHz.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_basicOp_CenterFreqMHz.AutoSize = true;
            this.lbl_basicOp_CenterFreqMHz.Location = new System.Drawing.Point(3, 48);
            this.lbl_basicOp_CenterFreqMHz.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_CenterFreqMHz.Name = "lbl_basicOp_CenterFreqMHz";
            this.lbl_basicOp_CenterFreqMHz.Size = new System.Drawing.Size(103, 20);
            this.lbl_basicOp_CenterFreqMHz.TabIndex = 0;
            this.lbl_basicOp_CenterFreqMHz.Text = "Center Freq:";
            this.lbl_basicOp_CenterFreqMHz.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lbl_basicOp_lowChan
            // 
            this.lbl_basicOp_lowChan.AutoSize = true;
            this.lbl_basicOp_lowChan.Location = new System.Drawing.Point(3, 12);
            this.lbl_basicOp_lowChan.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_lowChan.Name = "lbl_basicOp_lowChan";
            this.lbl_basicOp_lowChan.Size = new System.Drawing.Size(198, 20);
            this.lbl_basicOp_lowChan.TabIndex = 21;
            this.lbl_basicOp_lowChan.Text = "Channel (Low+Pri index):";
            this.lbl_basicOp_lowChan.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // groupBox_burnEEPROM
            // 
            this.groupBox_burnEEPROM.BackColor = System.Drawing.Color.White;
            this.groupBox_burnEEPROM.Controls.Add(this.cmd_nvMemCtrl_burnToNVmemory);
            this.groupBox_burnEEPROM.Controls.Add(this.table_nvMemCtrl_burnEEPROM);
            this.groupBox_burnEEPROM.Location = new System.Drawing.Point(3, 9);
            this.groupBox_burnEEPROM.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_burnEEPROM.Name = "groupBox_burnEEPROM";
            this.groupBox_burnEEPROM.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_burnEEPROM.Size = new System.Drawing.Size(724, 172);
            this.groupBox_burnEEPROM.TabIndex = 8;
            this.groupBox_burnEEPROM.TabStop = false;
            this.groupBox_burnEEPROM.Text = "Burn EEPROM File";
            // 
            // cmd_nvMemCtrl_burnToNVmemory
            // 
            this.cmd_nvMemCtrl_burnToNVmemory.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_nvMemCtrl_burnToNVmemory.Location = new System.Drawing.Point(531, 114);
            this.cmd_nvMemCtrl_burnToNVmemory.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_nvMemCtrl_burnToNVmemory.Name = "cmd_nvMemCtrl_burnToNVmemory";
            this.cmd_nvMemCtrl_burnToNVmemory.Size = new System.Drawing.Size(176, 39);
            this.cmd_nvMemCtrl_burnToNVmemory.TabIndex = 0;
            this.cmd_nvMemCtrl_burnToNVmemory.Text = "Burn To NV Memory";
            this.cmd_nvMemCtrl_burnToNVmemory.UseVisualStyleBackColor = true;
            this.cmd_nvMemCtrl_burnToNVmemory.Click += new System.EventHandler(this.burnInfoBtn_Click);
            // 
            // table_nvMemCtrl_burnEEPROM
            // 
            this.table_nvMemCtrl_burnEEPROM.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)));
            this.table_nvMemCtrl_burnEEPROM.BackColor = System.Drawing.Color.White;
            this.table_nvMemCtrl_burnEEPROM.ColumnCount = 3;
            this.table_nvMemCtrl_burnEEPROM.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 19.02748F));
            this.table_nvMemCtrl_burnEEPROM.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 54.79744F));
            this.table_nvMemCtrl_burnEEPROM.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 26.22601F));
            this.table_nvMemCtrl_burnEEPROM.Controls.Add(this.lbl_nvMemCtrl_calFile, 0, 0);
            this.table_nvMemCtrl_burnEEPROM.Controls.Add(this.txtBox_nvMemCtrl_eepromFile, 1, 0);
            this.table_nvMemCtrl_burnEEPROM.Controls.Add(this.cmd_nvMemCtrl_browseEEPROM, 2, 0);
            this.table_nvMemCtrl_burnEEPROM.Location = new System.Drawing.Point(9, 32);
            this.table_nvMemCtrl_burnEEPROM.Margin = new System.Windows.Forms.Padding(4);
            this.table_nvMemCtrl_burnEEPROM.Name = "table_nvMemCtrl_burnEEPROM";
            this.table_nvMemCtrl_burnEEPROM.RowCount = 1;
            this.table_nvMemCtrl_burnEEPROM.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.table_nvMemCtrl_burnEEPROM.Size = new System.Drawing.Size(704, 70);
            this.table_nvMemCtrl_burnEEPROM.TabIndex = 0;
            // 
            // lbl_nvMemCtrl_calFile
            // 
            this.lbl_nvMemCtrl_calFile.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_calFile.AutoSize = true;
            this.lbl_nvMemCtrl_calFile.Location = new System.Drawing.Point(12, 25);
            this.lbl_nvMemCtrl_calFile.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_calFile.Name = "lbl_nvMemCtrl_calFile";
            this.lbl_nvMemCtrl_calFile.Size = new System.Drawing.Size(117, 20);
            this.lbl_nvMemCtrl_calFile.TabIndex = 0;
            this.lbl_nvMemCtrl_calFile.Text = "Calibration File:";
            // 
            // txtBox_nvMemCtrl_eepromFile
            // 
            this.txtBox_nvMemCtrl_eepromFile.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_nvMemCtrl_eepromFile.BackColor = System.Drawing.Color.LavenderBlush;
            this.txtBox_nvMemCtrl_eepromFile.ForeColor = System.Drawing.Color.Black;
            this.txtBox_nvMemCtrl_eepromFile.Location = new System.Drawing.Point(137, 22);
            this.txtBox_nvMemCtrl_eepromFile.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_eepromFile.Name = "txtBox_nvMemCtrl_eepromFile";
            this.txtBox_nvMemCtrl_eepromFile.Size = new System.Drawing.Size(373, 26);
            this.txtBox_nvMemCtrl_eepromFile.TabIndex = 3;
            // 
            // cmd_nvMemCtrl_browseEEPROM
            // 
            this.cmd_nvMemCtrl_browseEEPROM.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.cmd_nvMemCtrl_browseEEPROM.Location = new System.Drawing.Point(523, 15);
            this.cmd_nvMemCtrl_browseEEPROM.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_nvMemCtrl_browseEEPROM.Name = "cmd_nvMemCtrl_browseEEPROM";
            this.cmd_nvMemCtrl_browseEEPROM.Size = new System.Drawing.Size(176, 39);
            this.cmd_nvMemCtrl_browseEEPROM.TabIndex = 4;
            this.cmd_nvMemCtrl_browseEEPROM.Text = "Browse...";
            this.cmd_nvMemCtrl_browseEEPROM.UseVisualStyleBackColor = true;
            this.cmd_nvMemCtrl_browseEEPROM.Click += new System.EventHandler(this.browseEEPROMfile_Click);
            // 
            // cmd_nvMemCtrl_readExtCalFile
            // 
            this.cmd_nvMemCtrl_readExtCalFile.Location = new System.Drawing.Point(14, 482);
            this.cmd_nvMemCtrl_readExtCalFile.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_nvMemCtrl_readExtCalFile.Name = "cmd_nvMemCtrl_readExtCalFile";
            this.cmd_nvMemCtrl_readExtCalFile.Size = new System.Drawing.Size(294, 57);
            this.cmd_nvMemCtrl_readExtCalFile.TabIndex = 1;
            this.cmd_nvMemCtrl_readExtCalFile.Text = "Read External Calibration File";
            this.cmd_nvMemCtrl_readExtCalFile.UseVisualStyleBackColor = true;
            this.cmd_nvMemCtrl_readExtCalFile.Click += new System.EventHandler(this.ParseFile_Click);
            // 
            // lbl_nvMemCtrl_barcode
            // 
            this.lbl_nvMemCtrl_barcode.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_barcode.AutoSize = true;
            this.lbl_nvMemCtrl_barcode.Location = new System.Drawing.Point(37, 11);
            this.lbl_nvMemCtrl_barcode.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_barcode.Name = "lbl_nvMemCtrl_barcode";
            this.lbl_nvMemCtrl_barcode.Size = new System.Drawing.Size(73, 20);
            this.lbl_nvMemCtrl_barcode.TabIndex = 0;
            this.lbl_nvMemCtrl_barcode.Text = "Barcode:";
            // 
            // txtBox_nvMemCtrl_barcode
            // 
            this.txtBox_nvMemCtrl_barcode.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_barcode.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(245)))), ((int)(((byte)(252)))), ((int)(((byte)(199)))));
            this.txtBox_nvMemCtrl_barcode.ForeColor = System.Drawing.Color.Black;
            this.txtBox_nvMemCtrl_barcode.Location = new System.Drawing.Point(118, 8);
            this.txtBox_nvMemCtrl_barcode.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_barcode.Name = "txtBox_nvMemCtrl_barcode";
            this.txtBox_nvMemCtrl_barcode.Size = new System.Drawing.Size(292, 26);
            this.txtBox_nvMemCtrl_barcode.TabIndex = 0;
            // 
            // cmd_nvMemCtrl_viewEEPROM
            // 
            this.cmd_nvMemCtrl_viewEEPROM.Location = new System.Drawing.Point(339, 482);
            this.cmd_nvMemCtrl_viewEEPROM.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_nvMemCtrl_viewEEPROM.Name = "cmd_nvMemCtrl_viewEEPROM";
            this.cmd_nvMemCtrl_viewEEPROM.Size = new System.Drawing.Size(363, 57);
            this.cmd_nvMemCtrl_viewEEPROM.TabIndex = 0;
            this.cmd_nvMemCtrl_viewEEPROM.Text = "View/Change/Save Calibration File";
            this.cmd_nvMemCtrl_viewEEPROM.UseVisualStyleBackColor = true;
            this.cmd_nvMemCtrl_viewEEPROM.Click += new System.EventHandler(this.ViewEEPROM_Click);
            // 
            // openEEPROMfile
            // 
            this.openEEPROMfile.DefaultExt = "txt";
            // 
            // openBeamformingMatrixValuesFile_standard
            // 
            this.openBeamformingMatrixValuesFile_standard.DefaultExt = "txt";
            // 
            // groupBox_genRisc
            // 
            this.groupBox_genRisc.BackColor = System.Drawing.Color.White;
            this.groupBox_genRisc.Controls.Add(this.check_basicOp_enableRxAggregation);
            this.groupBox_genRisc.Controls.Add(this.RxEvmList);
            this.groupBox_genRisc.Controls.Add(this.cmd_basicOp_updateRXevm);
            this.groupBox_genRisc.Controls.Add(this.inputsList);
            this.groupBox_genRisc.Controls.Add(this.cmd_basicOp_resetCounter);
            this.groupBox_genRisc.Controls.Add(this.cmd_basicOp_updateInput);
            this.groupBox_genRisc.Enabled = false;
            this.groupBox_genRisc.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox_genRisc.ForeColor = System.Drawing.Color.Black;
            this.groupBox_genRisc.Location = new System.Drawing.Point(670, 9);
            this.groupBox_genRisc.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_genRisc.Name = "groupBox_genRisc";
            this.groupBox_genRisc.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_genRisc.Size = new System.Drawing.Size(549, 456);
            this.groupBox_genRisc.TabIndex = 0;
            this.groupBox_genRisc.TabStop = false;
            this.groupBox_genRisc.Text = "GenRisc";
            // 
            // check_basicOp_enableRxAggregation
            // 
            this.check_basicOp_enableRxAggregation.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.check_basicOp_enableRxAggregation.Location = new System.Drawing.Point(322, 141);
            this.check_basicOp_enableRxAggregation.Name = "check_basicOp_enableRxAggregation";
            this.check_basicOp_enableRxAggregation.Size = new System.Drawing.Size(213, 26);
            this.check_basicOp_enableRxAggregation.TabIndex = 14;
            this.check_basicOp_enableRxAggregation.Text = "Enable RX Aggregation";
            this.check_basicOp_enableRxAggregation.UseVisualStyleBackColor = true;
            this.check_basicOp_enableRxAggregation.CheckedChanged += new System.EventHandler(this.enableRxAggregation_CheckedChanged);
            // 
            // RxEvmList
            // 
            this.RxEvmList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.rxEvmName,
            this.rxEvmValue});
            this.RxEvmList.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.RxEvmList.GridLines = true;
            this.RxEvmList.HideSelection = false;
            listViewItem1.Tag = "";
            this.RxEvmList.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem1,
            listViewItem2,
            listViewItem3,
            listViewItem4,
            listViewItem5});
            this.RxEvmList.Location = new System.Drawing.Point(316, 194);
            this.RxEvmList.Name = "RxEvmList";
            this.RxEvmList.Size = new System.Drawing.Size(218, 169);
            this.RxEvmList.TabIndex = 13;
            this.RxEvmList.UseCompatibleStateImageBehavior = false;
            this.RxEvmList.View = System.Windows.Forms.View.Details;
            // 
            // rxEvmName
            // 
            this.rxEvmName.Text = "Name";
            this.rxEvmName.Width = 71;
            // 
            // rxEvmValue
            // 
            this.rxEvmValue.Text = "Value";
            this.rxEvmValue.Width = 70;
            // 
            // cmd_basicOp_updateRXevm
            // 
            this.cmd_basicOp_updateRXevm.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_basicOp_updateRXevm.Location = new System.Drawing.Point(328, 380);
            this.cmd_basicOp_updateRXevm.Margin = new System.Windows.Forms.Padding(0);
            this.cmd_basicOp_updateRXevm.Name = "cmd_basicOp_updateRXevm";
            this.cmd_basicOp_updateRXevm.Size = new System.Drawing.Size(192, 39);
            this.cmd_basicOp_updateRXevm.TabIndex = 12;
            this.cmd_basicOp_updateRXevm.Text = "Update RX EVM";
            this.cmd_basicOp_updateRXevm.UseVisualStyleBackColor = true;
            this.cmd_basicOp_updateRXevm.Click += new System.EventHandler(this.RxEvmInputsBtn_Click);
            // 
            // inputsList
            // 
            this.inputsList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.inputName,
            this.inputValue});
            this.inputsList.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.inputsList.GridLines = true;
            this.inputsList.HideSelection = false;
            this.inputsList.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem6,
            listViewItem7,
            listViewItem8,
            listViewItem9,
            listViewItem10,
            listViewItem11,
            listViewItem12,
            listViewItem13,
            listViewItem14,
            listViewItem15,
            listViewItem16,
            listViewItem17});
            this.inputsList.Location = new System.Drawing.Point(9, 27);
            this.inputsList.Name = "inputsList";
            this.inputsList.Size = new System.Drawing.Size(300, 390);
            this.inputsList.TabIndex = 7;
            this.inputsList.UseCompatibleStateImageBehavior = false;
            this.inputsList.View = System.Windows.Forms.View.Details;
            // 
            // inputName
            // 
            this.inputName.Text = "Name";
            this.inputName.Width = 127;
            // 
            // inputValue
            // 
            this.inputValue.Text = "Value";
            this.inputValue.Width = 135;
            // 
            // cmd_basicOp_resetCounter
            // 
            this.cmd_basicOp_resetCounter.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_basicOp_resetCounter.Location = new System.Drawing.Point(328, 82);
            this.cmd_basicOp_resetCounter.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_resetCounter.Name = "cmd_basicOp_resetCounter";
            this.cmd_basicOp_resetCounter.Size = new System.Drawing.Size(192, 39);
            this.cmd_basicOp_resetCounter.TabIndex = 3;
            this.cmd_basicOp_resetCounter.Text = "Reset Counters";
            this.cmd_basicOp_resetCounter.UseVisualStyleBackColor = true;
            this.cmd_basicOp_resetCounter.Click += new System.EventHandler(this.resetCountersBtn_Click);
            // 
            // cmd_basicOp_updateInput
            // 
            this.cmd_basicOp_updateInput.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_basicOp_updateInput.Location = new System.Drawing.Point(328, 32);
            this.cmd_basicOp_updateInput.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_updateInput.Name = "cmd_basicOp_updateInput";
            this.cmd_basicOp_updateInput.Size = new System.Drawing.Size(192, 39);
            this.cmd_basicOp_updateInput.TabIndex = 5;
            this.cmd_basicOp_updateInput.Text = "Update";
            this.cmd_basicOp_updateInput.UseVisualStyleBackColor = true;
            this.cmd_basicOp_updateInput.Click += new System.EventHandler(this.UpdateInputs_Click);
            // 
            // pictureBoxLogo
            // 
            this.pictureBoxLogo.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxLogo.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxLogo.Image")));
            this.pictureBoxLogo.Location = new System.Drawing.Point(1018, 8);
            this.pictureBoxLogo.Margin = new System.Windows.Forms.Padding(4);
            this.pictureBoxLogo.Name = "pictureBoxLogo";
            this.pictureBoxLogo.Size = new System.Drawing.Size(165, 86);
            this.pictureBoxLogo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBoxLogo.TabIndex = 20;
            this.pictureBoxLogo.TabStop = false;
            // 
            // cmd_basicOp_stopCW
            // 
            this.cmd_basicOp_stopCW.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_basicOp_stopCW.Location = new System.Drawing.Point(333, 34);
            this.cmd_basicOp_stopCW.Margin = new System.Windows.Forms.Padding(34, 4, 4, 4);
            this.cmd_basicOp_stopCW.Name = "cmd_basicOp_stopCW";
            this.cmd_basicOp_stopCW.Size = new System.Drawing.Size(147, 34);
            this.cmd_basicOp_stopCW.TabIndex = 8;
            this.cmd_basicOp_stopCW.Text = "Stop CW";
            this.cmd_basicOp_stopCW.UseVisualStyleBackColor = true;
            this.cmd_basicOp_stopCW.Click += new System.EventHandler(this.stopCW_Click);
            // 
            // cmd_basicOp_transmitCW
            // 
            this.cmd_basicOp_transmitCW.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_basicOp_transmitCW.Location = new System.Drawing.Point(207, 34);
            this.cmd_basicOp_transmitCW.Margin = new System.Windows.Forms.Padding(34, 4, 4, 4);
            this.cmd_basicOp_transmitCW.Name = "cmd_basicOp_transmitCW";
            this.cmd_basicOp_transmitCW.Size = new System.Drawing.Size(122, 34);
            this.cmd_basicOp_transmitCW.TabIndex = 8;
            this.cmd_basicOp_transmitCW.Text = "Transmit CW";
            this.cmd_basicOp_transmitCW.UseVisualStyleBackColor = true;
            this.cmd_basicOp_transmitCW.Click += new System.EventHandler(this.TransmitCWbtn_Click);
            // 
            // lbl_basicOp_amplitude
            // 
            this.lbl_basicOp_amplitude.AutoSize = true;
            this.lbl_basicOp_amplitude.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.lbl_basicOp_amplitude.Location = new System.Drawing.Point(22, 21);
            this.lbl_basicOp_amplitude.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_amplitude.Name = "lbl_basicOp_amplitude";
            this.lbl_basicOp_amplitude.Size = new System.Drawing.Size(88, 20);
            this.lbl_basicOp_amplitude.TabIndex = 6;
            this.lbl_basicOp_amplitude.Text = "Amplitude:";
            // 
            // txtBox_basicOp_cwAmplitude
            // 
            this.txtBox_basicOp_cwAmplitude.BackColor = System.Drawing.Color.White;
            this.txtBox_basicOp_cwAmplitude.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_basicOp_cwAmplitude.Location = new System.Drawing.Point(114, 16);
            this.txtBox_basicOp_cwAmplitude.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_cwAmplitude.Name = "txtBox_basicOp_cwAmplitude";
            this.txtBox_basicOp_cwAmplitude.Size = new System.Drawing.Size(84, 26);
            this.txtBox_basicOp_cwAmplitude.TabIndex = 7;
            // 
            // lbl_basicOp_binNum
            // 
            this.lbl_basicOp_binNum.AutoSize = true;
            this.lbl_basicOp_binNum.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.lbl_basicOp_binNum.Location = new System.Drawing.Point(22, 54);
            this.lbl_basicOp_binNum.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_binNum.Name = "lbl_basicOp_binNum";
            this.lbl_basicOp_binNum.Size = new System.Drawing.Size(71, 20);
            this.lbl_basicOp_binNum.TabIndex = 6;
            this.lbl_basicOp_binNum.Text = "Bin num";
            // 
            // txtBox_basicOp_cwBinNum
            // 
            this.txtBox_basicOp_cwBinNum.BackColor = System.Drawing.Color.White;
            this.txtBox_basicOp_cwBinNum.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_basicOp_cwBinNum.Location = new System.Drawing.Point(114, 50);
            this.txtBox_basicOp_cwBinNum.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_cwBinNum.Name = "txtBox_basicOp_cwBinNum";
            this.txtBox_basicOp_cwBinNum.Size = new System.Drawing.Size(84, 26);
            this.txtBox_basicOp_cwBinNum.TabIndex = 7;
            // 
            // groupBox_connection
            // 
            this.groupBox_connection.BackColor = System.Drawing.Color.White;
            this.groupBox_connection.Controls.Add(this.comboBox_basicOp_memoryType);
            this.groupBox_connection.Controls.Add(this.groupBox_WlanCard);
            this.groupBox_connection.Controls.Add(this.snifferEnable);
            this.groupBox_connection.Controls.Add(this.ipAddress);
            this.groupBox_connection.Controls.Add(this.ConnectButton);
            this.groupBox_connection.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox_connection.Location = new System.Drawing.Point(12, 1159);
            this.groupBox_connection.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_connection.Name = "groupBox_connection";
            this.groupBox_connection.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_connection.Size = new System.Drawing.Size(399, 170);
            this.groupBox_connection.TabIndex = 14;
            this.groupBox_connection.TabStop = false;
            this.groupBox_connection.Text = "Connection";
            // 
            // comboBox_basicOp_memoryType
            // 
            this.comboBox_basicOp_memoryType.Cursor = System.Windows.Forms.Cursors.Default;
            this.comboBox_basicOp_memoryType.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.comboBox_basicOp_memoryType.FormattingEnabled = true;
            this.comboBox_basicOp_memoryType.Location = new System.Drawing.Point(144, 120);
            this.comboBox_basicOp_memoryType.Margin = new System.Windows.Forms.Padding(4);
            this.comboBox_basicOp_memoryType.Name = "comboBox_basicOp_memoryType";
            this.comboBox_basicOp_memoryType.Size = new System.Drawing.Size(110, 28);
            this.comboBox_basicOp_memoryType.TabIndex = 21;
            this.comboBox_basicOp_memoryType.Tag = "TAG_MEMTYPE";
            this.comboBox_basicOp_memoryType.Text = "flash";
            this.comboBox_basicOp_memoryType.SelectedIndexChanged += new System.EventHandler(this.comboBox_basicOp_memoryType_SelectedIndexChanged);
            // 
            // groupBox_WlanCard
            // 
            this.groupBox_WlanCard.Controls.Add(this.wlan4card);
            this.groupBox_WlanCard.Controls.Add(this.wlan2card);
            this.groupBox_WlanCard.Controls.Add(this.wlan0card);
            this.groupBox_WlanCard.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.groupBox_WlanCard.Location = new System.Drawing.Point(267, 24);
            this.groupBox_WlanCard.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_WlanCard.Name = "groupBox_WlanCard";
            this.groupBox_WlanCard.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_WlanCard.Size = new System.Drawing.Size(123, 141);
            this.groupBox_WlanCard.TabIndex = 19;
            this.groupBox_WlanCard.TabStop = false;
            this.groupBox_WlanCard.Text = "WLAN Card";
            // 
            // wlan4card
            // 
            this.wlan4card.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.wlan4card.Location = new System.Drawing.Point(9, 99);
            this.wlan4card.Margin = new System.Windows.Forms.Padding(4);
            this.wlan4card.Name = "wlan4card";
            this.wlan4card.Size = new System.Drawing.Size(104, 26);
            this.wlan4card.TabIndex = 1;
            this.wlan4card.Text = "WLAN 4";
            this.wlan4card.UseVisualStyleBackColor = true;
            this.wlan4card.CheckedChanged += new System.EventHandler(this.wlan4card_CheckedChanged);
            // 
            // wlan2card
            // 
            this.wlan2card.Checked = true;
            this.wlan2card.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.wlan2card.Location = new System.Drawing.Point(9, 64);
            this.wlan2card.Margin = new System.Windows.Forms.Padding(4);
            this.wlan2card.Name = "wlan2card";
            this.wlan2card.Size = new System.Drawing.Size(104, 26);
            this.wlan2card.TabIndex = 0;
            this.wlan2card.TabStop = true;
            this.wlan2card.Text = "WLAN 2";
            this.wlan2card.UseVisualStyleBackColor = true;
            this.wlan2card.CheckedChanged += new System.EventHandler(this.wlan2card_CheckedChanged);
            // 
            // wlan0card
            // 
            this.wlan0card.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.wlan0card.Location = new System.Drawing.Point(9, 30);
            this.wlan0card.Margin = new System.Windows.Forms.Padding(4);
            this.wlan0card.Name = "wlan0card";
            this.wlan0card.Size = new System.Drawing.Size(104, 26);
            this.wlan0card.TabIndex = 0;
            this.wlan0card.Text = "WLAN 0";
            this.wlan0card.UseVisualStyleBackColor = true;
            this.wlan0card.CheckedChanged += new System.EventHandler(this.wlan0card_CheckedChanged);
            // 
            // snifferEnable
            // 
            this.snifferEnable.AutoSize = true;
            this.snifferEnable.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.snifferEnable.Location = new System.Drawing.Point(24, 123);
            this.snifferEnable.Margin = new System.Windows.Forms.Padding(4);
            this.snifferEnable.Name = "snifferEnable";
            this.snifferEnable.Size = new System.Drawing.Size(84, 24);
            this.snifferEnable.TabIndex = 16;
            this.snifferEnable.Text = "Sniffer";
            this.snifferEnable.UseVisualStyleBackColor = true;
            // 
            // ipAddress
            // 
            this.ipAddress.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.ipAddress.BackColor = System.Drawing.Color.MistyRose;
            this.ipAddress.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.ipAddress.Location = new System.Drawing.Point(146, 74);
            this.ipAddress.Margin = new System.Windows.Forms.Padding(4);
            this.ipAddress.Name = "ipAddress";
            this.ipAddress.Size = new System.Drawing.Size(110, 26);
            this.ipAddress.TabIndex = 13;
            // 
            // ConnectButton
            // 
            this.ConnectButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.ConnectButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.ConnectButton.Location = new System.Drawing.Point(144, 28);
            this.ConnectButton.Margin = new System.Windows.Forms.Padding(4);
            this.ConnectButton.Name = "ConnectButton";
            this.ConnectButton.Size = new System.Drawing.Size(112, 36);
            this.ConnectButton.TabIndex = 12;
            this.ConnectButton.Text = "Connect";
            this.ConnectButton.UseVisualStyleBackColor = true;
            this.ConnectButton.Click += new System.EventHandler(this.ConnectButton_Click);
            // 
            // combox_DebugConsole
            // 
            this.combox_DebugConsole.Cursor = System.Windows.Forms.Cursors.Default;
            this.combox_DebugConsole.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.combox_DebugConsole.FormattingEnabled = true;
            this.combox_DebugConsole.Location = new System.Drawing.Point(20, 28);
            this.combox_DebugConsole.Margin = new System.Windows.Forms.Padding(4);
            this.combox_DebugConsole.Name = "combox_DebugConsole";
            this.combox_DebugConsole.Size = new System.Drawing.Size(110, 28);
            this.combox_DebugConsole.TabIndex = 37;
            this.combox_DebugConsole.Tag = "TAG_DBGOUT";
            this.combox_DebugConsole.Text = "PrintOutput";
            this.combox_DebugConsole.SelectedIndexChanged += new System.EventHandler(this.DebugConsole_SelectedIndexChanged);
            // 
            // combox_nvMemCtrl_printLevel
            // 
            this.combox_nvMemCtrl_printLevel.Cursor = System.Windows.Forms.Cursors.Default;
            this.combox_nvMemCtrl_printLevel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.combox_nvMemCtrl_printLevel.FormattingEnabled = true;
            this.combox_nvMemCtrl_printLevel.Location = new System.Drawing.Point(144, 28);
            this.combox_nvMemCtrl_printLevel.Margin = new System.Windows.Forms.Padding(4);
            this.combox_nvMemCtrl_printLevel.Name = "combox_nvMemCtrl_printLevel";
            this.combox_nvMemCtrl_printLevel.Size = new System.Drawing.Size(110, 28);
            this.combox_nvMemCtrl_printLevel.TabIndex = 35;
            this.combox_nvMemCtrl_printLevel.Tag = "TAG_PRINTLEVEL";
            this.combox_nvMemCtrl_printLevel.Text = "PrintLevel";
            this.combox_nvMemCtrl_printLevel.SelectedIndexChanged += new System.EventHandler(this.PrintingLevel_SelectedIndexChanged);
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.basicOperationTab);
            this.tabControl.Controls.Add(this.extendedOperationTab);
            this.tabControl.Controls.Add(this.nvMemControlTab);
            this.tabControl.Controls.Add(this.tpcTab);
            this.tabControl.DrawMode = System.Windows.Forms.TabDrawMode.OwnerDrawFixed;
            this.tabControl.Location = new System.Drawing.Point(12, 57);
            this.tabControl.Margin = new System.Windows.Forms.Padding(4);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(1233, 1096);
            this.tabControl.SizeMode = System.Windows.Forms.TabSizeMode.FillToRight;
            this.tabControl.TabIndex = 15;
            this.tabControl.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.tabControl_DrawGray);
            this.tabControl.SelectedIndexChanged += new System.EventHandler(this.tabControl_SelectedIndexChanged);
            this.tabControl.Selecting += new System.Windows.Forms.TabControlCancelEventHandler(this.tabControl_Unclickable);
            // 
            // basicOperationTab
            // 
            this.basicOperationTab.BackColor = System.Drawing.Color.White;
            this.basicOperationTab.Controls.Add(this.groupBox_RuParameters);
            this.basicOperationTab.Controls.Add(this.groupBox_basicOp_spaceless);
            this.basicOperationTab.Controls.Add(this.groupBox_cardID);
            this.basicOperationTab.Controls.Add(this.groupBox_basicOp_misc);
            this.basicOperationTab.Controls.Add(this.groupBox_antennasOnOff);
            this.basicOperationTab.Controls.Add(this.groupBox_tx);
            this.basicOperationTab.Controls.Add(this.groupBox_genRisc);
            this.basicOperationTab.Location = new System.Drawing.Point(4, 29);
            this.basicOperationTab.Margin = new System.Windows.Forms.Padding(4);
            this.basicOperationTab.Name = "basicOperationTab";
            this.basicOperationTab.Padding = new System.Windows.Forms.Padding(4);
            this.basicOperationTab.Size = new System.Drawing.Size(1225, 1063);
            this.basicOperationTab.TabIndex = 0;
            this.basicOperationTab.Text = "Basic Operation";
            // 
            // groupBox_RuParameters
            // 
            this.groupBox_RuParameters.Controls.Add(this.cmd_basicOp_setRuParams);
            this.groupBox_RuParameters.Controls.Add(this.txtBox_basicOp_user2);
            this.groupBox_RuParameters.Controls.Add(this.txtBox_basicOp_user1);
            this.groupBox_RuParameters.Controls.Add(this.lbl_basicOp_user2);
            this.groupBox_RuParameters.Controls.Add(this.lbl_basicOp_user1);
            this.groupBox_RuParameters.Location = new System.Drawing.Point(670, 474);
            this.groupBox_RuParameters.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_RuParameters.Name = "groupBox_RuParameters";
            this.groupBox_RuParameters.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_RuParameters.Size = new System.Drawing.Size(549, 88);
            this.groupBox_RuParameters.TabIndex = 22;
            this.groupBox_RuParameters.TabStop = false;
            this.groupBox_RuParameters.Text = "RU Parameters (Not used if beamforming on)";
            // 
            // cmd_basicOp_setRuParams
            // 
            this.cmd_basicOp_setRuParams.Location = new System.Drawing.Point(386, 34);
            this.cmd_basicOp_setRuParams.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_setRuParams.Name = "cmd_basicOp_setRuParams";
            this.cmd_basicOp_setRuParams.Size = new System.Drawing.Size(150, 34);
            this.cmd_basicOp_setRuParams.TabIndex = 10;
            this.cmd_basicOp_setRuParams.Text = "Set RU Params";
            this.cmd_basicOp_setRuParams.UseVisualStyleBackColor = true;
            this.cmd_basicOp_setRuParams.Click += new System.EventHandler(this.cmd_basicOp_setRuParams_Click);
            // 
            // txtBox_basicOp_user2
            // 
            this.txtBox_basicOp_user2.Location = new System.Drawing.Point(255, 36);
            this.txtBox_basicOp_user2.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_user2.Name = "txtBox_basicOp_user2";
            this.txtBox_basicOp_user2.Size = new System.Drawing.Size(104, 26);
            this.txtBox_basicOp_user2.TabIndex = 9;
            // 
            // txtBox_basicOp_user1
            // 
            this.txtBox_basicOp_user1.Location = new System.Drawing.Point(72, 36);
            this.txtBox_basicOp_user1.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_user1.Name = "txtBox_basicOp_user1";
            this.txtBox_basicOp_user1.Size = new System.Drawing.Size(104, 26);
            this.txtBox_basicOp_user1.TabIndex = 8;
            // 
            // lbl_basicOp_user2
            // 
            this.lbl_basicOp_user2.AutoSize = true;
            this.lbl_basicOp_user2.Location = new System.Drawing.Point(189, 42);
            this.lbl_basicOp_user2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_user2.Name = "lbl_basicOp_user2";
            this.lbl_basicOp_user2.Size = new System.Drawing.Size(56, 20);
            this.lbl_basicOp_user2.TabIndex = 3;
            this.lbl_basicOp_user2.Text = "User 2";
            // 
            // lbl_basicOp_user1
            // 
            this.lbl_basicOp_user1.AutoSize = true;
            this.lbl_basicOp_user1.Location = new System.Drawing.Point(6, 42);
            this.lbl_basicOp_user1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_user1.Name = "lbl_basicOp_user1";
            this.lbl_basicOp_user1.Size = new System.Drawing.Size(56, 20);
            this.lbl_basicOp_user1.TabIndex = 2;
            this.lbl_basicOp_user1.Text = "User 1";
            // 
            // groupBox_basicOp_spaceless
            // 
            this.groupBox_basicOp_spaceless.BackColor = System.Drawing.Color.White;
            this.groupBox_basicOp_spaceless.Controls.Add(this.cmd_basicOp_stopSpaceless);
            this.groupBox_basicOp_spaceless.Controls.Add(this.cmd_basicOp_startSpaceless);
            this.groupBox_basicOp_spaceless.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox_basicOp_spaceless.Location = new System.Drawing.Point(670, 794);
            this.groupBox_basicOp_spaceless.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_basicOp_spaceless.Name = "groupBox_basicOp_spaceless";
            this.groupBox_basicOp_spaceless.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_basicOp_spaceless.Size = new System.Drawing.Size(549, 72);
            this.groupBox_basicOp_spaceless.TabIndex = 21;
            this.groupBox_basicOp_spaceless.TabStop = false;
            this.groupBox_basicOp_spaceless.Text = "Tx Spaceless";
            // 
            // cmd_basicOp_stopSpaceless
            // 
            this.cmd_basicOp_stopSpaceless.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.cmd_basicOp_stopSpaceless.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_basicOp_stopSpaceless.Location = new System.Drawing.Point(244, 30);
            this.cmd_basicOp_stopSpaceless.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_stopSpaceless.Name = "cmd_basicOp_stopSpaceless";
            this.cmd_basicOp_stopSpaceless.Size = new System.Drawing.Size(186, 34);
            this.cmd_basicOp_stopSpaceless.TabIndex = 38;
            this.cmd_basicOp_stopSpaceless.Text = "Stop Tx";
            this.cmd_basicOp_stopSpaceless.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.cmd_basicOp_stopSpaceless.UseVisualStyleBackColor = true;
            this.cmd_basicOp_stopSpaceless.Click += new System.EventHandler(this.cmd_basicOp_stopSpaceless_Click);
            // 
            // cmd_basicOp_startSpaceless
            // 
            this.cmd_basicOp_startSpaceless.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.cmd_basicOp_startSpaceless.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_basicOp_startSpaceless.Location = new System.Drawing.Point(50, 30);
            this.cmd_basicOp_startSpaceless.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_basicOp_startSpaceless.Name = "cmd_basicOp_startSpaceless";
            this.cmd_basicOp_startSpaceless.Size = new System.Drawing.Size(186, 34);
            this.cmd_basicOp_startSpaceless.TabIndex = 37;
            this.cmd_basicOp_startSpaceless.Text = "Start Tx Spaceless";
            this.cmd_basicOp_startSpaceless.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.cmd_basicOp_startSpaceless.UseVisualStyleBackColor = true;
            this.cmd_basicOp_startSpaceless.Click += new System.EventHandler(this.cmd_basicOp_startSpaceless_Click);
            // 
            // groupBox_cardID
            // 
            this.groupBox_cardID.BackColor = System.Drawing.Color.White;
            this.groupBox_cardID.Controls.Add(this.txtBox_basicOp_rxAntennaMask);
            this.groupBox_cardID.Controls.Add(this.txtBox_basicOp_CalMode);
            this.groupBox_cardID.Controls.Add(this.btn_basicOp_deleteRegistry);
            this.groupBox_cardID.Controls.Add(this.txtBox_basicOp_bandSupport);
            this.groupBox_cardID.Controls.Add(this.lbl_basicOp_bandSupport);
            this.groupBox_cardID.Controls.Add(this.txtBox_basicOp_bandCurrent);
            this.groupBox_cardID.Controls.Add(this.lbl_basicOp_bandCurrent);
            this.groupBox_cardID.Controls.Add(this.txtBox_basicOp_txAntennaMask);
            this.groupBox_cardID.Controls.Add(this.lbl_basicOp_antMask);
            this.groupBox_cardID.Controls.Add(this.lbl_basicOp_wav);
            this.groupBox_cardID.Controls.Add(this.txtBox_basicOp_wav);
            this.groupBox_cardID.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox_cardID.Location = new System.Drawing.Point(670, 868);
            this.groupBox_cardID.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_cardID.Name = "groupBox_cardID";
            this.groupBox_cardID.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_cardID.Size = new System.Drawing.Size(549, 141);
            this.groupBox_cardID.TabIndex = 20;
            this.groupBox_cardID.TabStop = false;
            this.groupBox_cardID.Text = "Card ID";
            // 
            // txtBox_basicOp_rxAntennaMask
            // 
            this.txtBox_basicOp_rxAntennaMask.BackColor = System.Drawing.Color.White;
            this.txtBox_basicOp_rxAntennaMask.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_basicOp_rxAntennaMask.Location = new System.Drawing.Point(321, 45);
            this.txtBox_basicOp_rxAntennaMask.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_rxAntennaMask.Name = "txtBox_basicOp_rxAntennaMask";
            this.txtBox_basicOp_rxAntennaMask.ReadOnly = true;
            this.txtBox_basicOp_rxAntennaMask.Size = new System.Drawing.Size(92, 26);
            this.txtBox_basicOp_rxAntennaMask.TabIndex = 38;
            // 
            // txtBox_basicOp_CalMode
            // 
            this.txtBox_basicOp_CalMode.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_basicOp_CalMode.BackColor = System.Drawing.Color.PaleGoldenrod;
            this.txtBox_basicOp_CalMode.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_basicOp_CalMode.Location = new System.Drawing.Point(321, 14);
            this.txtBox_basicOp_CalMode.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_CalMode.Name = "txtBox_basicOp_CalMode";
            this.txtBox_basicOp_CalMode.ReadOnly = true;
            this.txtBox_basicOp_CalMode.Size = new System.Drawing.Size(212, 26);
            this.txtBox_basicOp_CalMode.TabIndex = 37;
            // 
            // btn_basicOp_deleteRegistry
            // 
            this.btn_basicOp_deleteRegistry.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.btn_basicOp_deleteRegistry.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.btn_basicOp_deleteRegistry.Location = new System.Drawing.Point(352, 94);
            this.btn_basicOp_deleteRegistry.Margin = new System.Windows.Forms.Padding(34, 30, 4, 4);
            this.btn_basicOp_deleteRegistry.Name = "btn_basicOp_deleteRegistry";
            this.btn_basicOp_deleteRegistry.Size = new System.Drawing.Size(150, 34);
            this.btn_basicOp_deleteRegistry.TabIndex = 35;
            this.btn_basicOp_deleteRegistry.Text = "Delete Registry";
            this.btn_basicOp_deleteRegistry.UseVisualStyleBackColor = true;
            this.btn_basicOp_deleteRegistry.Click += new System.EventHandler(this.btn_basicOp_deleteRegistry_Click);
            // 
            // txtBox_basicOp_bandSupport
            // 
            this.txtBox_basicOp_bandSupport.BackColor = System.Drawing.Color.White;
            this.txtBox_basicOp_bandSupport.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_basicOp_bandSupport.Location = new System.Drawing.Point(150, 76);
            this.txtBox_basicOp_bandSupport.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_bandSupport.Name = "txtBox_basicOp_bandSupport";
            this.txtBox_basicOp_bandSupport.ReadOnly = true;
            this.txtBox_basicOp_bandSupport.Size = new System.Drawing.Size(150, 26);
            this.txtBox_basicOp_bandSupport.TabIndex = 11;
            // 
            // lbl_basicOp_bandSupport
            // 
            this.lbl_basicOp_bandSupport.AutoSize = true;
            this.lbl_basicOp_bandSupport.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.lbl_basicOp_bandSupport.Location = new System.Drawing.Point(21, 80);
            this.lbl_basicOp_bandSupport.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_bandSupport.Name = "lbl_basicOp_bandSupport";
            this.lbl_basicOp_bandSupport.Size = new System.Drawing.Size(111, 20);
            this.lbl_basicOp_bandSupport.TabIndex = 10;
            this.lbl_basicOp_bandSupport.Text = "Band Support";
            // 
            // txtBox_basicOp_bandCurrent
            // 
            this.txtBox_basicOp_bandCurrent.BackColor = System.Drawing.Color.White;
            this.txtBox_basicOp_bandCurrent.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_basicOp_bandCurrent.Location = new System.Drawing.Point(150, 108);
            this.txtBox_basicOp_bandCurrent.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_bandCurrent.Name = "txtBox_basicOp_bandCurrent";
            this.txtBox_basicOp_bandCurrent.ReadOnly = true;
            this.txtBox_basicOp_bandCurrent.Size = new System.Drawing.Size(150, 26);
            this.txtBox_basicOp_bandCurrent.TabIndex = 11;
            // 
            // lbl_basicOp_bandCurrent
            // 
            this.lbl_basicOp_bandCurrent.AutoSize = true;
            this.lbl_basicOp_bandCurrent.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.lbl_basicOp_bandCurrent.Location = new System.Drawing.Point(21, 110);
            this.lbl_basicOp_bandCurrent.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_bandCurrent.Name = "lbl_basicOp_bandCurrent";
            this.lbl_basicOp_bandCurrent.Size = new System.Drawing.Size(109, 20);
            this.lbl_basicOp_bandCurrent.TabIndex = 10;
            this.lbl_basicOp_bandCurrent.Text = "Current Band";
            // 
            // txtBox_basicOp_txAntennaMask
            // 
            this.txtBox_basicOp_txAntennaMask.BackColor = System.Drawing.Color.White;
            this.txtBox_basicOp_txAntennaMask.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_basicOp_txAntennaMask.Location = new System.Drawing.Point(207, 45);
            this.txtBox_basicOp_txAntennaMask.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_txAntennaMask.Name = "txtBox_basicOp_txAntennaMask";
            this.txtBox_basicOp_txAntennaMask.ReadOnly = true;
            this.txtBox_basicOp_txAntennaMask.Size = new System.Drawing.Size(92, 26);
            this.txtBox_basicOp_txAntennaMask.TabIndex = 9;
            // 
            // lbl_basicOp_antMask
            // 
            this.lbl_basicOp_antMask.AutoSize = true;
            this.lbl_basicOp_antMask.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.lbl_basicOp_antMask.Location = new System.Drawing.Point(21, 54);
            this.lbl_basicOp_antMask.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_antMask.Name = "lbl_basicOp_antMask";
            this.lbl_basicOp_antMask.Size = new System.Drawing.Size(181, 20);
            this.lbl_basicOp_antMask.TabIndex = 8;
            this.lbl_basicOp_antMask.Text = "Antenna Mask (TX/RX)";
            // 
            // lbl_basicOp_wav
            // 
            this.lbl_basicOp_wav.AutoSize = true;
            this.lbl_basicOp_wav.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.lbl_basicOp_wav.Location = new System.Drawing.Point(21, 26);
            this.lbl_basicOp_wav.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_basicOp_wav.Name = "lbl_basicOp_wav";
            this.lbl_basicOp_wav.Size = new System.Drawing.Size(91, 20);
            this.lbl_basicOp_wav.TabIndex = 6;
            this.lbl_basicOp_wav.Text = "Generation";
            // 
            // txtBox_basicOp_wav
            // 
            this.txtBox_basicOp_wav.BackColor = System.Drawing.Color.White;
            this.txtBox_basicOp_wav.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_basicOp_wav.Location = new System.Drawing.Point(150, 14);
            this.txtBox_basicOp_wav.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_basicOp_wav.Name = "txtBox_basicOp_wav";
            this.txtBox_basicOp_wav.ReadOnly = true;
            this.txtBox_basicOp_wav.Size = new System.Drawing.Size(150, 26);
            this.txtBox_basicOp_wav.TabIndex = 7;
            // 
            // groupBox_basicOp_misc
            // 
            this.groupBox_basicOp_misc.BackColor = System.Drawing.Color.White;
            this.groupBox_basicOp_misc.Controls.Add(this.lbl_basicOp_amplitude);
            this.groupBox_basicOp_misc.Controls.Add(this.lbl_basicOp_binNum);
            this.groupBox_basicOp_misc.Controls.Add(this.cmd_basicOp_stopCW);
            this.groupBox_basicOp_misc.Controls.Add(this.txtBox_basicOp_cwAmplitude);
            this.groupBox_basicOp_misc.Controls.Add(this.txtBox_basicOp_cwBinNum);
            this.groupBox_basicOp_misc.Controls.Add(this.cmd_basicOp_transmitCW);
            this.groupBox_basicOp_misc.Enabled = false;
            this.groupBox_basicOp_misc.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox_basicOp_misc.Location = new System.Drawing.Point(670, 706);
            this.groupBox_basicOp_misc.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_basicOp_misc.Name = "groupBox_basicOp_misc";
            this.groupBox_basicOp_misc.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_basicOp_misc.Size = new System.Drawing.Size(549, 84);
            this.groupBox_basicOp_misc.TabIndex = 16;
            this.groupBox_basicOp_misc.TabStop = false;
            this.groupBox_basicOp_misc.Text = "Tx CW";
            // 
            // groupBox_antennasOnOff
            // 
            this.groupBox_antennasOnOff.BackColor = System.Drawing.Color.White;
            this.groupBox_antennasOnOff.Controls.Add(this.table_basicOp_antennas);
            this.groupBox_antennasOnOff.Enabled = false;
            this.groupBox_antennasOnOff.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox_antennasOnOff.Location = new System.Drawing.Point(670, 579);
            this.groupBox_antennasOnOff.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_antennasOnOff.Name = "groupBox_antennasOnOff";
            this.groupBox_antennasOnOff.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_antennasOnOff.Size = new System.Drawing.Size(549, 124);
            this.groupBox_antennasOnOff.TabIndex = 14;
            this.groupBox_antennasOnOff.TabStop = false;
            this.groupBox_antennasOnOff.Text = "Antennas On / Off";
            // 
            // extendedOperationTab
            // 
            this.extendedOperationTab.BackColor = System.Drawing.Color.White;
            this.extendedOperationTab.Controls.Add(this.tableLayoutPanel6);
            this.extendedOperationTab.Controls.Add(this.groupBox_rwRegisters);
            this.extendedOperationTab.Controls.Add(this.groupBox2);
            this.extendedOperationTab.Controls.Add(this.groupBox_antennaPhase);
            this.extendedOperationTab.Controls.Add(this.groupBox_xtalCalib);
            this.extendedOperationTab.Location = new System.Drawing.Point(4, 29);
            this.extendedOperationTab.Margin = new System.Windows.Forms.Padding(4);
            this.extendedOperationTab.Name = "extendedOperationTab";
            this.extendedOperationTab.Padding = new System.Windows.Forms.Padding(4);
            this.extendedOperationTab.Size = new System.Drawing.Size(1225, 1063);
            this.extendedOperationTab.TabIndex = 4;
            this.extendedOperationTab.Text = "Extended Operation";
            // 
            // tableLayoutPanel6
            // 
            this.tableLayoutPanel6.AutoSize = true;
            this.tableLayoutPanel6.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.tableLayoutPanel6.ColumnCount = 1;
            this.tableLayoutPanel6.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel6.Controls.Add(this.bf_tabControl, 0, 0);
            this.tableLayoutPanel6.Controls.Add(this.groupBoxOther, 0, 1);
            this.tableLayoutPanel6.Location = new System.Drawing.Point(7, 211);
            this.tableLayoutPanel6.MinimumSize = new System.Drawing.Size(100, 100);
            this.tableLayoutPanel6.Name = "tableLayoutPanel6";
            this.tableLayoutPanel6.RowCount = 2;
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel6.Size = new System.Drawing.Size(606, 1138);
            this.tableLayoutPanel6.TabIndex = 39;
            this.tableLayoutPanel6.Paint += new System.Windows.Forms.PaintEventHandler(this.tableLayoutPanel6_Paint);
            // 
            // bf_tabControl
            // 
            this.bf_tabControl.Appearance = System.Windows.Forms.TabAppearance.FlatButtons;
            this.bf_tabControl.Controls.Add(this.bf_tab_default);
            this.bf_tabControl.Controls.Add(this.bf_tab_standard);
            this.bf_tabControl.Controls.Add(this.bf_tab_eht160);
            this.bf_tabControl.Controls.Add(this.bf_tab_eht320);
            this.bf_tabControl.Dock = System.Windows.Forms.DockStyle.Top;
            this.bf_tabControl.ItemSize = new System.Drawing.Size(0, 1);
            this.bf_tabControl.Location = new System.Drawing.Point(3, 3);
            this.bf_tabControl.Name = "bf_tabControl";
            this.bf_tabControl.Padding = new System.Drawing.Point(0, 0);
            this.bf_tabControl.SelectedIndex = 0;
            this.bf_tabControl.Size = new System.Drawing.Size(600, 563);
            this.bf_tabControl.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.bf_tabControl.TabIndex = 38;
            // 
            // bf_tab_default
            // 
            this.bf_tab_default.BackColor = System.Drawing.SystemColors.Window;
            this.bf_tab_default.Controls.Add(this.label22);
            this.bf_tab_default.Location = new System.Drawing.Point(4, 5);
            this.bf_tab_default.Margin = new System.Windows.Forms.Padding(0);
            this.bf_tab_default.Name = "bf_tab_default";
            this.bf_tab_default.Size = new System.Drawing.Size(592, 554);
            this.bf_tab_default.TabIndex = 0;
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label22.Location = new System.Drawing.Point(88, 0);
            this.label22.Name = "label22";
            this.label22.Padding = new System.Windows.Forms.Padding(0, 20, 0, 20);
            this.label22.Size = new System.Drawing.Size(396, 65);
            this.label22.TabIndex = 0;
            this.label22.Text = "To write beamforming matrix first set channel";
            this.label22.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // bf_tab_standard
            // 
            this.bf_tab_standard.Controls.Add(this.groupBox_WriteBeamformingMatrix);
            this.bf_tab_standard.Location = new System.Drawing.Point(4, 5);
            this.bf_tab_standard.Margin = new System.Windows.Forms.Padding(0);
            this.bf_tab_standard.Name = "bf_tab_standard";
            this.bf_tab_standard.Padding = new System.Windows.Forms.Padding(3);
            this.bf_tab_standard.Size = new System.Drawing.Size(592, 554);
            this.bf_tab_standard.TabIndex = 1;
            this.bf_tab_standard.Text = "tabPage2";
            this.bf_tab_standard.UseVisualStyleBackColor = true;
            // 
            // groupBox_WriteBeamformingMatrix
            // 
            this.groupBox_WriteBeamformingMatrix.BackColor = System.Drawing.Color.White;
            this.groupBox_WriteBeamformingMatrix.Controls.Add(this.button_writeBeamformingMatrixFile_standard);
            this.groupBox_WriteBeamformingMatrix.Controls.Add(this.tableLayoutPanel2);
            this.groupBox_WriteBeamformingMatrix.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox_WriteBeamformingMatrix.Location = new System.Drawing.Point(0, 0);
            this.groupBox_WriteBeamformingMatrix.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_WriteBeamformingMatrix.Name = "groupBox_WriteBeamformingMatrix";
            this.groupBox_WriteBeamformingMatrix.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_WriteBeamformingMatrix.Size = new System.Drawing.Size(598, 276);
            this.groupBox_WriteBeamformingMatrix.TabIndex = 36;
            this.groupBox_WriteBeamformingMatrix.TabStop = false;
            this.groupBox_WriteBeamformingMatrix.Text = "Write Beamforming Matrix to Memory";
            // 
            // button_writeBeamformingMatrixFile_standard
            // 
            this.button_writeBeamformingMatrixFile_standard.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.button_writeBeamformingMatrixFile_standard.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_writeBeamformingMatrixFile_standard.Location = new System.Drawing.Point(436, 218);
            this.button_writeBeamformingMatrixFile_standard.Margin = new System.Windows.Forms.Padding(4);
            this.button_writeBeamformingMatrixFile_standard.Name = "button_writeBeamformingMatrixFile_standard";
            this.button_writeBeamformingMatrixFile_standard.Size = new System.Drawing.Size(148, 39);
            this.button_writeBeamformingMatrixFile_standard.TabIndex = 0;
            this.button_writeBeamformingMatrixFile_standard.Text = "Write To Memory";
            this.button_writeBeamformingMatrixFile_standard.UseVisualStyleBackColor = true;
            this.button_writeBeamformingMatrixFile_standard.Click += new System.EventHandler(this.button_writeBeamformingMatrixFile_Click);
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)));
            this.tableLayoutPanel2.BackColor = System.Drawing.Color.White;
            this.tableLayoutPanel2.ColumnCount = 3;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 19.01779F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 54.76954F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 26.21266F));
            this.tableLayoutPanel2.Controls.Add(this.lbl_beamformingMatrixValuesFile, 0, 1);
            this.tableLayoutPanel2.Controls.Add(this.lbl_beamformingMatrixHeaderFile, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.txtBox_beamformingMatrixHeaderFile_standard, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.button_browseBeamformingMatrixHeaderFile_standard, 2, 0);
            this.tableLayoutPanel2.Controls.Add(this.txtBox_beamformingMatrixValuesFile_standard, 1, 1);
            this.tableLayoutPanel2.Controls.Add(this.button_browseBeamformingMatrixValuesFile_standard, 2, 1);
            this.tableLayoutPanel2.Location = new System.Drawing.Point(9, 32);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(4);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 2;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(580, 174);
            this.tableLayoutPanel2.TabIndex = 0;
            // 
            // lbl_beamformingMatrixValuesFile
            // 
            this.lbl_beamformingMatrixValuesFile.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_beamformingMatrixValuesFile.AutoSize = true;
            this.lbl_beamformingMatrixValuesFile.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.lbl_beamformingMatrixValuesFile.Location = new System.Drawing.Point(9, 110);
            this.lbl_beamformingMatrixValuesFile.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_beamformingMatrixValuesFile.Name = "lbl_beamformingMatrixValuesFile";
            this.lbl_beamformingMatrixValuesFile.Size = new System.Drawing.Size(97, 40);
            this.lbl_beamformingMatrixValuesFile.TabIndex = 5;
            this.lbl_beamformingMatrixValuesFile.Text = "Matrix Values File:";
            // 
            // lbl_beamformingMatrixHeaderFile
            // 
            this.lbl_beamformingMatrixHeaderFile.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_beamformingMatrixHeaderFile.AutoSize = true;
            this.lbl_beamformingMatrixHeaderFile.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.lbl_beamformingMatrixHeaderFile.Location = new System.Drawing.Point(5, 23);
            this.lbl_beamformingMatrixHeaderFile.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_beamformingMatrixHeaderFile.Name = "lbl_beamformingMatrixHeaderFile";
            this.lbl_beamformingMatrixHeaderFile.Size = new System.Drawing.Size(101, 40);
            this.lbl_beamformingMatrixHeaderFile.TabIndex = 0;
            this.lbl_beamformingMatrixHeaderFile.Text = "Matrix Header File:";
            // 
            // txtBox_beamformingMatrixHeaderFile_standard
            // 
            this.txtBox_beamformingMatrixHeaderFile_standard.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_beamformingMatrixHeaderFile_standard.BackColor = System.Drawing.Color.LavenderBlush;
            this.txtBox_beamformingMatrixHeaderFile_standard.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_beamformingMatrixHeaderFile_standard.ForeColor = System.Drawing.Color.Black;
            this.txtBox_beamformingMatrixHeaderFile_standard.Location = new System.Drawing.Point(114, 30);
            this.txtBox_beamformingMatrixHeaderFile_standard.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_beamformingMatrixHeaderFile_standard.Name = "txtBox_beamformingMatrixHeaderFile_standard";
            this.txtBox_beamformingMatrixHeaderFile_standard.Size = new System.Drawing.Size(306, 26);
            this.txtBox_beamformingMatrixHeaderFile_standard.TabIndex = 3;
            // 
            // button_browseBeamformingMatrixHeaderFile_standard
            // 
            this.button_browseBeamformingMatrixHeaderFile_standard.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.button_browseBeamformingMatrixHeaderFile_standard.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_browseBeamformingMatrixHeaderFile_standard.Location = new System.Drawing.Point(432, 24);
            this.button_browseBeamformingMatrixHeaderFile_standard.Margin = new System.Windows.Forms.Padding(4);
            this.button_browseBeamformingMatrixHeaderFile_standard.Name = "button_browseBeamformingMatrixHeaderFile_standard";
            this.button_browseBeamformingMatrixHeaderFile_standard.Size = new System.Drawing.Size(142, 39);
            this.button_browseBeamformingMatrixHeaderFile_standard.TabIndex = 4;
            this.button_browseBeamformingMatrixHeaderFile_standard.Text = "Browse...";
            this.button_browseBeamformingMatrixHeaderFile_standard.UseVisualStyleBackColor = true;
            this.button_browseBeamformingMatrixHeaderFile_standard.Click += new System.EventHandler(this.button_browseBeamformingMatrixHeaderFile_Click);
            // 
            // txtBox_beamformingMatrixValuesFile_standard
            // 
            this.txtBox_beamformingMatrixValuesFile_standard.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_beamformingMatrixValuesFile_standard.BackColor = System.Drawing.Color.LavenderBlush;
            this.txtBox_beamformingMatrixValuesFile_standard.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_beamformingMatrixValuesFile_standard.ForeColor = System.Drawing.Color.Black;
            this.txtBox_beamformingMatrixValuesFile_standard.Location = new System.Drawing.Point(114, 117);
            this.txtBox_beamformingMatrixValuesFile_standard.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_beamformingMatrixValuesFile_standard.Name = "txtBox_beamformingMatrixValuesFile_standard";
            this.txtBox_beamformingMatrixValuesFile_standard.Size = new System.Drawing.Size(306, 26);
            this.txtBox_beamformingMatrixValuesFile_standard.TabIndex = 6;
            // 
            // button_browseBeamformingMatrixValuesFile_standard
            // 
            this.button_browseBeamformingMatrixValuesFile_standard.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.button_browseBeamformingMatrixValuesFile_standard.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_browseBeamformingMatrixValuesFile_standard.Location = new System.Drawing.Point(432, 111);
            this.button_browseBeamformingMatrixValuesFile_standard.Margin = new System.Windows.Forms.Padding(4);
            this.button_browseBeamformingMatrixValuesFile_standard.Name = "button_browseBeamformingMatrixValuesFile_standard";
            this.button_browseBeamformingMatrixValuesFile_standard.Size = new System.Drawing.Size(142, 39);
            this.button_browseBeamformingMatrixValuesFile_standard.TabIndex = 7;
            this.button_browseBeamformingMatrixValuesFile_standard.Text = "Browse...";
            this.button_browseBeamformingMatrixValuesFile_standard.UseVisualStyleBackColor = true;
            this.button_browseBeamformingMatrixValuesFile_standard.Click += new System.EventHandler(this.button_browseBeamformingMatrixValuesFile_Click);
            // 
            // bf_tab_eht160
            // 
            this.bf_tab_eht160.Controls.Add(this.groupBox3);
            this.bf_tab_eht160.Location = new System.Drawing.Point(4, 5);
            this.bf_tab_eht160.Margin = new System.Windows.Forms.Padding(0);
            this.bf_tab_eht160.Name = "bf_tab_eht160";
            this.bf_tab_eht160.Size = new System.Drawing.Size(592, 554);
            this.bf_tab_eht160.TabIndex = 2;
            this.bf_tab_eht160.Text = "tabPage1";
            this.bf_tab_eht160.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.BackColor = System.Drawing.Color.White;
            this.groupBox3.Controls.Add(this.button_writeBeamformingMatrixFile_160mhz);
            this.groupBox3.Controls.Add(this.tableLayoutPanel4);
            this.groupBox3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox3.Location = new System.Drawing.Point(0, 0);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox3.Size = new System.Drawing.Size(598, 299);
            this.groupBox3.TabIndex = 37;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Write Beamforming Matrix to Memory";
            // 
            // button_writeBeamformingMatrixFile_160mhz
            // 
            this.button_writeBeamformingMatrixFile_160mhz.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.button_writeBeamformingMatrixFile_160mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_writeBeamformingMatrixFile_160mhz.Location = new System.Drawing.Point(435, 252);
            this.button_writeBeamformingMatrixFile_160mhz.Margin = new System.Windows.Forms.Padding(4);
            this.button_writeBeamformingMatrixFile_160mhz.Name = "button_writeBeamformingMatrixFile_160mhz";
            this.button_writeBeamformingMatrixFile_160mhz.Size = new System.Drawing.Size(148, 39);
            this.button_writeBeamformingMatrixFile_160mhz.TabIndex = 0;
            this.button_writeBeamformingMatrixFile_160mhz.Text = "Write To Memory";
            this.button_writeBeamformingMatrixFile_160mhz.UseVisualStyleBackColor = true;
            this.button_writeBeamformingMatrixFile_160mhz.Click += new System.EventHandler(this.button_writeBeamformingMatrixFile_160mhz_Click);
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)));
            this.tableLayoutPanel4.BackColor = System.Drawing.Color.White;
            this.tableLayoutPanel4.ColumnCount = 3;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 21.89655F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 51.72414F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 26.21266F));
            this.tableLayoutPanel4.Controls.Add(this.label15, 0, 2);
            this.tableLayoutPanel4.Controls.Add(this.txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz, 0, 2);
            this.tableLayoutPanel4.Controls.Add(this.button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz, 0, 2);
            this.tableLayoutPanel4.Controls.Add(this.label10, 0, 1);
            this.tableLayoutPanel4.Controls.Add(this.label14, 0, 0);
            this.tableLayoutPanel4.Controls.Add(this.txtBox_beamformingMatrixHeaderFile_160mhz, 1, 0);
            this.tableLayoutPanel4.Controls.Add(this.button_browseBeamformingMatrixHeaderFile_160mhz, 2, 0);
            this.tableLayoutPanel4.Controls.Add(this.txtBox_beamformingMatrixValuesFile_160mhz, 1, 1);
            this.tableLayoutPanel4.Controls.Add(this.button_browseBeamformingMatrixValuesFile_160mhz, 2, 1);
            this.tableLayoutPanel4.Location = new System.Drawing.Point(9, 32);
            this.tableLayoutPanel4.Margin = new System.Windows.Forms.Padding(4);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 3;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(580, 209);
            this.tableLayoutPanel4.TabIndex = 0;
            // 
            // label15
            // 
            this.label15.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label15.AutoSize = true;
            this.label15.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.label15.Location = new System.Drawing.Point(7, 143);
            this.label15.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(116, 60);
            this.label15.TabIndex = 8;
            this.label15.Text = "Extended Matrix Values File:";
            this.label15.Click += new System.EventHandler(this.label15_Click);
            // 
            // txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz
            // 
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz.BackColor = System.Drawing.Color.LavenderBlush;
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz.ForeColor = System.Drawing.Color.Black;
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz.Location = new System.Drawing.Point(131, 160);
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz.Name = "txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz";
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz.Size = new System.Drawing.Size(292, 26);
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz.TabIndex = 9;
            // 
            // button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz
            // 
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz.Location = new System.Drawing.Point(432, 154);
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz.Margin = new System.Windows.Forms.Padding(4);
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz.Name = "button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz";
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz.Size = new System.Drawing.Size(142, 39);
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz.TabIndex = 10;
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz.Text = "Browse...";
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz.UseVisualStyleBackColor = true;
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz.Click += new System.EventHandler(this.button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz_Click);
            // 
            // label10
            // 
            this.label10.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.label10.Location = new System.Drawing.Point(7, 83);
            this.label10.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(116, 40);
            this.label10.TabIndex = 5;
            this.label10.Text = "Matrix Values File:";
            // 
            // label14
            // 
            this.label14.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label14.AutoSize = true;
            this.label14.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.label14.Location = new System.Drawing.Point(8, 14);
            this.label14.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(115, 40);
            this.label14.TabIndex = 0;
            this.label14.Text = "Matrix Header File:";
            // 
            // txtBox_beamformingMatrixHeaderFile_160mhz
            // 
            this.txtBox_beamformingMatrixHeaderFile_160mhz.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_beamformingMatrixHeaderFile_160mhz.BackColor = System.Drawing.Color.LavenderBlush;
            this.txtBox_beamformingMatrixHeaderFile_160mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_beamformingMatrixHeaderFile_160mhz.ForeColor = System.Drawing.Color.Black;
            this.txtBox_beamformingMatrixHeaderFile_160mhz.Location = new System.Drawing.Point(131, 21);
            this.txtBox_beamformingMatrixHeaderFile_160mhz.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_beamformingMatrixHeaderFile_160mhz.Name = "txtBox_beamformingMatrixHeaderFile_160mhz";
            this.txtBox_beamformingMatrixHeaderFile_160mhz.Size = new System.Drawing.Size(292, 26);
            this.txtBox_beamformingMatrixHeaderFile_160mhz.TabIndex = 3;
            // 
            // button_browseBeamformingMatrixHeaderFile_160mhz
            // 
            this.button_browseBeamformingMatrixHeaderFile_160mhz.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.button_browseBeamformingMatrixHeaderFile_160mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_browseBeamformingMatrixHeaderFile_160mhz.Location = new System.Drawing.Point(432, 15);
            this.button_browseBeamformingMatrixHeaderFile_160mhz.Margin = new System.Windows.Forms.Padding(4);
            this.button_browseBeamformingMatrixHeaderFile_160mhz.Name = "button_browseBeamformingMatrixHeaderFile_160mhz";
            this.button_browseBeamformingMatrixHeaderFile_160mhz.Size = new System.Drawing.Size(142, 39);
            this.button_browseBeamformingMatrixHeaderFile_160mhz.TabIndex = 4;
            this.button_browseBeamformingMatrixHeaderFile_160mhz.Text = "Browse...";
            this.button_browseBeamformingMatrixHeaderFile_160mhz.UseVisualStyleBackColor = true;
            this.button_browseBeamformingMatrixHeaderFile_160mhz.Click += new System.EventHandler(this.button_browseBeamformingMatrixHeaderFile_160mhz_Click);
            // 
            // txtBox_beamformingMatrixValuesFile_160mhz
            // 
            this.txtBox_beamformingMatrixValuesFile_160mhz.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_beamformingMatrixValuesFile_160mhz.BackColor = System.Drawing.Color.LavenderBlush;
            this.txtBox_beamformingMatrixValuesFile_160mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_beamformingMatrixValuesFile_160mhz.ForeColor = System.Drawing.Color.Black;
            this.txtBox_beamformingMatrixValuesFile_160mhz.Location = new System.Drawing.Point(131, 90);
            this.txtBox_beamformingMatrixValuesFile_160mhz.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_beamformingMatrixValuesFile_160mhz.Name = "txtBox_beamformingMatrixValuesFile_160mhz";
            this.txtBox_beamformingMatrixValuesFile_160mhz.Size = new System.Drawing.Size(292, 26);
            this.txtBox_beamformingMatrixValuesFile_160mhz.TabIndex = 6;
            // 
            // button_browseBeamformingMatrixValuesFile_160mhz
            // 
            this.button_browseBeamformingMatrixValuesFile_160mhz.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.button_browseBeamformingMatrixValuesFile_160mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_browseBeamformingMatrixValuesFile_160mhz.Location = new System.Drawing.Point(432, 84);
            this.button_browseBeamformingMatrixValuesFile_160mhz.Margin = new System.Windows.Forms.Padding(4);
            this.button_browseBeamformingMatrixValuesFile_160mhz.Name = "button_browseBeamformingMatrixValuesFile_160mhz";
            this.button_browseBeamformingMatrixValuesFile_160mhz.Size = new System.Drawing.Size(142, 39);
            this.button_browseBeamformingMatrixValuesFile_160mhz.TabIndex = 7;
            this.button_browseBeamformingMatrixValuesFile_160mhz.Text = "Browse...";
            this.button_browseBeamformingMatrixValuesFile_160mhz.UseVisualStyleBackColor = true;
            this.button_browseBeamformingMatrixValuesFile_160mhz.Click += new System.EventHandler(this.button_browseBeamformingMatrixValuesFile_160mhz_Click);
            // 
            // bf_tab_eht320
            // 
            this.bf_tab_eht320.Controls.Add(this.groupBox4);
            this.bf_tab_eht320.Location = new System.Drawing.Point(4, 5);
            this.bf_tab_eht320.Margin = new System.Windows.Forms.Padding(0);
            this.bf_tab_eht320.Name = "bf_tab_eht320";
            this.bf_tab_eht320.Size = new System.Drawing.Size(592, 554);
            this.bf_tab_eht320.TabIndex = 3;
            this.bf_tab_eht320.Text = "tabPage1";
            this.bf_tab_eht320.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.BackColor = System.Drawing.Color.White;
            this.groupBox4.Controls.Add(this.groupBox5);
            this.groupBox4.Controls.Add(this.groupBox6);
            this.groupBox4.Controls.Add(this.button_writeBeamformingMatrixFile_320mhz);
            this.groupBox4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox4.Location = new System.Drawing.Point(0, 0);
            this.groupBox4.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox4.Size = new System.Drawing.Size(598, 536);
            this.groupBox4.TabIndex = 38;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Write Beamforming Matrix to Memory";
            // 
            // groupBox5
            // 
            this.groupBox5.AutoSize = true;
            this.groupBox5.Controls.Add(this.tableLayoutPanel5);
            this.groupBox5.Location = new System.Drawing.Point(4, 21);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(589, 223);
            this.groupBox5.TabIndex = 39;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Lower band";
            // 
            // tableLayoutPanel5
            // 
            this.tableLayoutPanel5.BackColor = System.Drawing.Color.White;
            this.tableLayoutPanel5.ColumnCount = 3;
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 21.89655F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 51.72414F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 26.21266F));
            this.tableLayoutPanel5.Controls.Add(this.label16, 0, 2);
            this.tableLayoutPanel5.Controls.Add(this.txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz, 0, 2);
            this.tableLayoutPanel5.Controls.Add(this.button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz, 0, 2);
            this.tableLayoutPanel5.Controls.Add(this.label17, 0, 1);
            this.tableLayoutPanel5.Controls.Add(this.label18, 0, 0);
            this.tableLayoutPanel5.Controls.Add(this.txtBox_beamformingMatrixHeaderFile_lower_320mhz, 1, 0);
            this.tableLayoutPanel5.Controls.Add(this.button_browseBeamformingMatrixHeaderFile_lower_320mhz, 2, 0);
            this.tableLayoutPanel5.Controls.Add(this.txtBox_beamformingMatrixValuesFile_lower_320mhz, 1, 1);
            this.tableLayoutPanel5.Controls.Add(this.button_browseBeamformingMatrixValuesFile_lower_320mhz, 2, 1);
            this.tableLayoutPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel5.Location = new System.Drawing.Point(3, 22);
            this.tableLayoutPanel5.Margin = new System.Windows.Forms.Padding(4);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 3;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(583, 198);
            this.tableLayoutPanel5.TabIndex = 0;
            // 
            // label16
            // 
            this.label16.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label16.AutoSize = true;
            this.label16.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.label16.Location = new System.Drawing.Point(7, 135);
            this.label16.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(116, 60);
            this.label16.TabIndex = 8;
            this.label16.Text = "Extended Matrix Values File:";
            // 
            // txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz
            // 
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz.BackColor = System.Drawing.Color.LavenderBlush;
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz.ForeColor = System.Drawing.Color.Black;
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz.Location = new System.Drawing.Point(131, 152);
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz.Name = "txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz";
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz.Size = new System.Drawing.Size(292, 26);
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz.TabIndex = 9;
            // 
            // button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz
            // 
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz.Location = new System.Drawing.Point(435, 145);
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz.Margin = new System.Windows.Forms.Padding(4);
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz.Name = "button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz";
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz.Size = new System.Drawing.Size(142, 39);
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz.TabIndex = 10;
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz.Text = "Browse...";
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz.UseVisualStyleBackColor = true;
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz.Click += new System.EventHandler(this.button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz_Click);
            // 
            // label17
            // 
            this.label17.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label17.AutoSize = true;
            this.label17.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.label17.Location = new System.Drawing.Point(7, 79);
            this.label17.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(116, 40);
            this.label17.TabIndex = 5;
            this.label17.Text = "Matrix Values File:";
            // 
            // label18
            // 
            this.label18.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label18.AutoSize = true;
            this.label18.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.label18.Location = new System.Drawing.Point(8, 13);
            this.label18.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(115, 40);
            this.label18.TabIndex = 0;
            this.label18.Text = "Matrix Header File:";
            // 
            // txtBox_beamformingMatrixHeaderFile_lower_320mhz
            // 
            this.txtBox_beamformingMatrixHeaderFile_lower_320mhz.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_beamformingMatrixHeaderFile_lower_320mhz.BackColor = System.Drawing.Color.LavenderBlush;
            this.txtBox_beamformingMatrixHeaderFile_lower_320mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_beamformingMatrixHeaderFile_lower_320mhz.ForeColor = System.Drawing.Color.Black;
            this.txtBox_beamformingMatrixHeaderFile_lower_320mhz.Location = new System.Drawing.Point(131, 20);
            this.txtBox_beamformingMatrixHeaderFile_lower_320mhz.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_beamformingMatrixHeaderFile_lower_320mhz.Name = "txtBox_beamformingMatrixHeaderFile_lower_320mhz";
            this.txtBox_beamformingMatrixHeaderFile_lower_320mhz.Size = new System.Drawing.Size(292, 26);
            this.txtBox_beamformingMatrixHeaderFile_lower_320mhz.TabIndex = 3;
            // 
            // button_browseBeamformingMatrixHeaderFile_lower_320mhz
            // 
            this.button_browseBeamformingMatrixHeaderFile_lower_320mhz.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.button_browseBeamformingMatrixHeaderFile_lower_320mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_browseBeamformingMatrixHeaderFile_lower_320mhz.Location = new System.Drawing.Point(435, 13);
            this.button_browseBeamformingMatrixHeaderFile_lower_320mhz.Margin = new System.Windows.Forms.Padding(4);
            this.button_browseBeamformingMatrixHeaderFile_lower_320mhz.Name = "button_browseBeamformingMatrixHeaderFile_lower_320mhz";
            this.button_browseBeamformingMatrixHeaderFile_lower_320mhz.Size = new System.Drawing.Size(142, 39);
            this.button_browseBeamformingMatrixHeaderFile_lower_320mhz.TabIndex = 4;
            this.button_browseBeamformingMatrixHeaderFile_lower_320mhz.Text = "Browse...";
            this.button_browseBeamformingMatrixHeaderFile_lower_320mhz.UseVisualStyleBackColor = true;
            this.button_browseBeamformingMatrixHeaderFile_lower_320mhz.Click += new System.EventHandler(this.button_browseBeamformingMatrixHeaderFile_lower_320mhz_Click);
            // 
            // txtBox_beamformingMatrixValuesFile_lower_320mhz
            // 
            this.txtBox_beamformingMatrixValuesFile_lower_320mhz.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_beamformingMatrixValuesFile_lower_320mhz.BackColor = System.Drawing.Color.LavenderBlush;
            this.txtBox_beamformingMatrixValuesFile_lower_320mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_beamformingMatrixValuesFile_lower_320mhz.ForeColor = System.Drawing.Color.Black;
            this.txtBox_beamformingMatrixValuesFile_lower_320mhz.Location = new System.Drawing.Point(131, 86);
            this.txtBox_beamformingMatrixValuesFile_lower_320mhz.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_beamformingMatrixValuesFile_lower_320mhz.Name = "txtBox_beamformingMatrixValuesFile_lower_320mhz";
            this.txtBox_beamformingMatrixValuesFile_lower_320mhz.Size = new System.Drawing.Size(292, 26);
            this.txtBox_beamformingMatrixValuesFile_lower_320mhz.TabIndex = 6;
            // 
            // button_browseBeamformingMatrixValuesFile_lower_320mhz
            // 
            this.button_browseBeamformingMatrixValuesFile_lower_320mhz.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.button_browseBeamformingMatrixValuesFile_lower_320mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_browseBeamformingMatrixValuesFile_lower_320mhz.Location = new System.Drawing.Point(435, 79);
            this.button_browseBeamformingMatrixValuesFile_lower_320mhz.Margin = new System.Windows.Forms.Padding(4);
            this.button_browseBeamformingMatrixValuesFile_lower_320mhz.Name = "button_browseBeamformingMatrixValuesFile_lower_320mhz";
            this.button_browseBeamformingMatrixValuesFile_lower_320mhz.Size = new System.Drawing.Size(142, 39);
            this.button_browseBeamformingMatrixValuesFile_lower_320mhz.TabIndex = 7;
            this.button_browseBeamformingMatrixValuesFile_lower_320mhz.Text = "Browse...";
            this.button_browseBeamformingMatrixValuesFile_lower_320mhz.UseVisualStyleBackColor = true;
            this.button_browseBeamformingMatrixValuesFile_lower_320mhz.Click += new System.EventHandler(this.button_browseBeamformingMatrixValuesFile_lower_320mhz_Click);
            // 
            // groupBox6
            // 
            this.groupBox6.AutoSize = true;
            this.groupBox6.Controls.Add(this.tableLayoutPanel7);
            this.groupBox6.Location = new System.Drawing.Point(6, 250);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(585, 223);
            this.groupBox6.TabIndex = 40;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Upper band";
            // 
            // tableLayoutPanel7
            // 
            this.tableLayoutPanel7.BackColor = System.Drawing.Color.White;
            this.tableLayoutPanel7.ColumnCount = 3;
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 21.89655F));
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 51.72414F));
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 26.21266F));
            this.tableLayoutPanel7.Controls.Add(this.label19, 0, 2);
            this.tableLayoutPanel7.Controls.Add(this.txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz, 0, 2);
            this.tableLayoutPanel7.Controls.Add(this.button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz, 0, 2);
            this.tableLayoutPanel7.Controls.Add(this.label20, 0, 1);
            this.tableLayoutPanel7.Controls.Add(this.txtBox_beamformingMatrixHeaderFile_upper_320mhz, 1, 0);
            this.tableLayoutPanel7.Controls.Add(this.button_browseBeamformingMatrixHeaderFile_upper_320mhz, 2, 0);
            this.tableLayoutPanel7.Controls.Add(this.txtBox_beamformingMatrixValuesFile_upper_320mhz, 1, 1);
            this.tableLayoutPanel7.Controls.Add(this.button_browseBeamformingMatrixValuesFile_upper_320mhz, 2, 1);
            this.tableLayoutPanel7.Controls.Add(this.label21, 0, 0);
            this.tableLayoutPanel7.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel7.Location = new System.Drawing.Point(3, 22);
            this.tableLayoutPanel7.Margin = new System.Windows.Forms.Padding(4);
            this.tableLayoutPanel7.Name = "tableLayoutPanel7";
            this.tableLayoutPanel7.RowCount = 3;
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            this.tableLayoutPanel7.Size = new System.Drawing.Size(579, 198);
            this.tableLayoutPanel7.TabIndex = 0;
            // 
            // label19
            // 
            this.label19.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label19.AutoSize = true;
            this.label19.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.label19.Location = new System.Drawing.Point(6, 135);
            this.label19.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(116, 60);
            this.label19.TabIndex = 8;
            this.label19.Text = "Extended Matrix Values File:";
            // 
            // txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz
            // 
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz.BackColor = System.Drawing.Color.LavenderBlush;
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz.ForeColor = System.Drawing.Color.Black;
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz.Location = new System.Drawing.Point(130, 152);
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz.Name = "txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz";
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz.Size = new System.Drawing.Size(291, 26);
            this.txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz.TabIndex = 9;
            // 
            // button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz
            // 
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz.Location = new System.Drawing.Point(431, 145);
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz.Margin = new System.Windows.Forms.Padding(4);
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz.Name = "button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz";
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz.Size = new System.Drawing.Size(142, 39);
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz.TabIndex = 10;
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz.Text = "Browse...";
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz.UseVisualStyleBackColor = true;
            this.button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz.Click += new System.EventHandler(this.button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz_Click);
            // 
            // label20
            // 
            this.label20.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label20.AutoSize = true;
            this.label20.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.label20.Location = new System.Drawing.Point(6, 79);
            this.label20.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(116, 40);
            this.label20.TabIndex = 5;
            this.label20.Text = "Matrix Values File:";
            // 
            // txtBox_beamformingMatrixHeaderFile_upper_320mhz
            // 
            this.txtBox_beamformingMatrixHeaderFile_upper_320mhz.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_beamformingMatrixHeaderFile_upper_320mhz.BackColor = System.Drawing.Color.LavenderBlush;
            this.txtBox_beamformingMatrixHeaderFile_upper_320mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_beamformingMatrixHeaderFile_upper_320mhz.ForeColor = System.Drawing.Color.Black;
            this.txtBox_beamformingMatrixHeaderFile_upper_320mhz.Location = new System.Drawing.Point(130, 20);
            this.txtBox_beamformingMatrixHeaderFile_upper_320mhz.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_beamformingMatrixHeaderFile_upper_320mhz.Name = "txtBox_beamformingMatrixHeaderFile_upper_320mhz";
            this.txtBox_beamformingMatrixHeaderFile_upper_320mhz.Size = new System.Drawing.Size(291, 26);
            this.txtBox_beamformingMatrixHeaderFile_upper_320mhz.TabIndex = 3;
            // 
            // button_browseBeamformingMatrixHeaderFile_upper_320mhz
            // 
            this.button_browseBeamformingMatrixHeaderFile_upper_320mhz.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.button_browseBeamformingMatrixHeaderFile_upper_320mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_browseBeamformingMatrixHeaderFile_upper_320mhz.Location = new System.Drawing.Point(431, 13);
            this.button_browseBeamformingMatrixHeaderFile_upper_320mhz.Margin = new System.Windows.Forms.Padding(4);
            this.button_browseBeamformingMatrixHeaderFile_upper_320mhz.Name = "button_browseBeamformingMatrixHeaderFile_upper_320mhz";
            this.button_browseBeamformingMatrixHeaderFile_upper_320mhz.Size = new System.Drawing.Size(142, 39);
            this.button_browseBeamformingMatrixHeaderFile_upper_320mhz.TabIndex = 4;
            this.button_browseBeamformingMatrixHeaderFile_upper_320mhz.Text = "Browse...";
            this.button_browseBeamformingMatrixHeaderFile_upper_320mhz.UseVisualStyleBackColor = true;
            this.button_browseBeamformingMatrixHeaderFile_upper_320mhz.Click += new System.EventHandler(this.button_browseBeamformingMatrixHeaderFile_upper_320mhz_Click);
            // 
            // txtBox_beamformingMatrixValuesFile_upper_320mhz
            // 
            this.txtBox_beamformingMatrixValuesFile_upper_320mhz.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_beamformingMatrixValuesFile_upper_320mhz.BackColor = System.Drawing.Color.LavenderBlush;
            this.txtBox_beamformingMatrixValuesFile_upper_320mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_beamformingMatrixValuesFile_upper_320mhz.ForeColor = System.Drawing.Color.Black;
            this.txtBox_beamformingMatrixValuesFile_upper_320mhz.Location = new System.Drawing.Point(130, 86);
            this.txtBox_beamformingMatrixValuesFile_upper_320mhz.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_beamformingMatrixValuesFile_upper_320mhz.Name = "txtBox_beamformingMatrixValuesFile_upper_320mhz";
            this.txtBox_beamformingMatrixValuesFile_upper_320mhz.Size = new System.Drawing.Size(291, 26);
            this.txtBox_beamformingMatrixValuesFile_upper_320mhz.TabIndex = 6;
            // 
            // button_browseBeamformingMatrixValuesFile_upper_320mhz
            // 
            this.button_browseBeamformingMatrixValuesFile_upper_320mhz.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.button_browseBeamformingMatrixValuesFile_upper_320mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_browseBeamformingMatrixValuesFile_upper_320mhz.Location = new System.Drawing.Point(431, 79);
            this.button_browseBeamformingMatrixValuesFile_upper_320mhz.Margin = new System.Windows.Forms.Padding(4);
            this.button_browseBeamformingMatrixValuesFile_upper_320mhz.Name = "button_browseBeamformingMatrixValuesFile_upper_320mhz";
            this.button_browseBeamformingMatrixValuesFile_upper_320mhz.Size = new System.Drawing.Size(142, 39);
            this.button_browseBeamformingMatrixValuesFile_upper_320mhz.TabIndex = 7;
            this.button_browseBeamformingMatrixValuesFile_upper_320mhz.Text = "Browse...";
            this.button_browseBeamformingMatrixValuesFile_upper_320mhz.UseVisualStyleBackColor = true;
            this.button_browseBeamformingMatrixValuesFile_upper_320mhz.Click += new System.EventHandler(this.button_browseBeamformingMatrixValuesFile_upper_320mhz_Click);
            // 
            // label21
            // 
            this.label21.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label21.AutoSize = true;
            this.label21.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.label21.Location = new System.Drawing.Point(7, 13);
            this.label21.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(115, 40);
            this.label21.TabIndex = 0;
            this.label21.Text = "Matrix Header File:";
            // 
            // button_writeBeamformingMatrixFile_320mhz
            // 
            this.button_writeBeamformingMatrixFile_320mhz.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.button_writeBeamformingMatrixFile_320mhz.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_writeBeamformingMatrixFile_320mhz.Location = new System.Drawing.Point(436, 489);
            this.button_writeBeamformingMatrixFile_320mhz.Margin = new System.Windows.Forms.Padding(4);
            this.button_writeBeamformingMatrixFile_320mhz.Name = "button_writeBeamformingMatrixFile_320mhz";
            this.button_writeBeamformingMatrixFile_320mhz.Size = new System.Drawing.Size(148, 39);
            this.button_writeBeamformingMatrixFile_320mhz.TabIndex = 0;
            this.button_writeBeamformingMatrixFile_320mhz.Text = "Write To Memory";
            this.button_writeBeamformingMatrixFile_320mhz.UseVisualStyleBackColor = true;
            this.button_writeBeamformingMatrixFile_320mhz.Click += new System.EventHandler(this.button_writeBeamformingMatrixFile_320mhz_Click);
            // 
            // groupBoxOther
            // 
            this.groupBoxOther.BackColor = System.Drawing.Color.White;
            this.groupBoxOther.Controls.Add(this.tableLayoutPanel3);
            this.groupBoxOther.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBoxOther.Location = new System.Drawing.Point(4, 573);
            this.groupBoxOther.Margin = new System.Windows.Forms.Padding(4);
            this.groupBoxOther.Name = "groupBoxOther";
            this.groupBoxOther.Padding = new System.Windows.Forms.Padding(4);
            this.groupBoxOther.Size = new System.Drawing.Size(598, 174);
            this.groupBoxOther.TabIndex = 37;
            this.groupBoxOther.TabStop = false;
            this.groupBoxOther.Text = "Other";
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)));
            this.tableLayoutPanel3.BackColor = System.Drawing.Color.White;
            this.tableLayoutPanel3.ColumnCount = 3;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 23.80952F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 38.09524F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 38.09524F));
            this.tableLayoutPanel3.Controls.Add(this.textBoxTemperature, 1, 0);
            this.tableLayoutPanel3.Controls.Add(this.button_getTemperature, 2, 0);
            this.tableLayoutPanel3.Controls.Add(this.labelTemperature, 0, 0);
            this.tableLayoutPanel3.Location = new System.Drawing.Point(9, 28);
            this.tableLayoutPanel3.Margin = new System.Windows.Forms.Padding(4);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 1;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(580, 70);
            this.tableLayoutPanel3.TabIndex = 1;
            // 
            // textBoxTemperature
            // 
            this.textBoxTemperature.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.textBoxTemperature.BackColor = System.Drawing.SystemColors.Window;
            this.textBoxTemperature.Enabled = false;
            this.textBoxTemperature.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.textBoxTemperature.ForeColor = System.Drawing.Color.Black;
            this.textBoxTemperature.Location = new System.Drawing.Point(142, 22);
            this.textBoxTemperature.Margin = new System.Windows.Forms.Padding(4);
            this.textBoxTemperature.Name = "textBoxTemperature";
            this.textBoxTemperature.Size = new System.Drawing.Size(210, 26);
            this.textBoxTemperature.TabIndex = 3;
            // 
            // button_getTemperature
            // 
            this.button_getTemperature.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.button_getTemperature.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.button_getTemperature.Location = new System.Drawing.Point(367, 15);
            this.button_getTemperature.Margin = new System.Windows.Forms.Padding(4);
            this.button_getTemperature.Name = "button_getTemperature";
            this.button_getTemperature.Size = new System.Drawing.Size(204, 39);
            this.button_getTemperature.TabIndex = 4;
            this.button_getTemperature.Text = "Get temperature";
            this.button_getTemperature.UseVisualStyleBackColor = true;
            this.button_getTemperature.Click += new System.EventHandler(this.button_getTemperature_Click);
            // 
            // labelTemperature
            // 
            this.labelTemperature.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.labelTemperature.AutoSize = true;
            this.labelTemperature.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.labelTemperature.Location = new System.Drawing.Point(4, 15);
            this.labelTemperature.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelTemperature.Name = "labelTemperature";
            this.labelTemperature.Size = new System.Drawing.Size(109, 40);
            this.labelTemperature.TabIndex = 0;
            this.labelTemperature.Text = "Temperature (�C):";
            // 
            // groupBox_rwRegisters
            // 
            this.groupBox_rwRegisters.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox_rwRegisters.BackColor = System.Drawing.Color.White;
            this.groupBox_rwRegisters.Controls.Add(this.cmd_nvMemCtrl_writeReg);
            this.groupBox_rwRegisters.Controls.Add(this.cmd_nvMemCtrl_readReg);
            this.groupBox_rwRegisters.Controls.Add(this.table_nvMemCtrl_rwRegisters);
            this.groupBox_rwRegisters.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox_rwRegisters.Location = new System.Drawing.Point(616, 742);
            this.groupBox_rwRegisters.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_rwRegisters.Name = "groupBox_rwRegisters";
            this.groupBox_rwRegisters.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_rwRegisters.Size = new System.Drawing.Size(531, 216);
            this.groupBox_rwRegisters.TabIndex = 23;
            this.groupBox_rwRegisters.TabStop = false;
            this.groupBox_rwRegisters.Text = "Read/Write Registers";
            // 
            // cmd_nvMemCtrl_writeReg
            // 
            this.cmd_nvMemCtrl_writeReg.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_nvMemCtrl_writeReg.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_nvMemCtrl_writeReg.Location = new System.Drawing.Point(336, 124);
            this.cmd_nvMemCtrl_writeReg.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_nvMemCtrl_writeReg.Name = "cmd_nvMemCtrl_writeReg";
            this.cmd_nvMemCtrl_writeReg.Size = new System.Drawing.Size(159, 60);
            this.cmd_nvMemCtrl_writeReg.TabIndex = 5;
            this.cmd_nvMemCtrl_writeReg.Text = "Write";
            this.cmd_nvMemCtrl_writeReg.UseVisualStyleBackColor = true;
            this.cmd_nvMemCtrl_writeReg.Click += new System.EventHandler(this.cmd_nvMemCtrl_writeReg_Click);
            // 
            // cmd_nvMemCtrl_readReg
            // 
            this.cmd_nvMemCtrl_readReg.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_nvMemCtrl_readReg.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_nvMemCtrl_readReg.Location = new System.Drawing.Point(336, 40);
            this.cmd_nvMemCtrl_readReg.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_nvMemCtrl_readReg.Name = "cmd_nvMemCtrl_readReg";
            this.cmd_nvMemCtrl_readReg.Size = new System.Drawing.Size(158, 62);
            this.cmd_nvMemCtrl_readReg.TabIndex = 4;
            this.cmd_nvMemCtrl_readReg.Text = "Read";
            this.cmd_nvMemCtrl_readReg.UseVisualStyleBackColor = true;
            this.cmd_nvMemCtrl_readReg.Click += new System.EventHandler(this.cmd_nvMemCtrl_readReg_Click);
            // 
            // table_nvMemCtrl_rwRegisters
            // 
            this.table_nvMemCtrl_rwRegisters.ColumnCount = 2;
            this.table_nvMemCtrl_rwRegisters.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 31.93717F));
            this.table_nvMemCtrl_rwRegisters.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 68.06283F));
            this.table_nvMemCtrl_rwRegisters.Controls.Add(this.label13, 0, 3);
            this.table_nvMemCtrl_rwRegisters.Controls.Add(this.txtBox_nvMemCtrl_regModule, 1, 0);
            this.table_nvMemCtrl_rwRegisters.Controls.Add(this.txtBox_nvMemCtrl_regAddress, 1, 1);
            this.table_nvMemCtrl_rwRegisters.Controls.Add(this.lbl_nvMemCtrl_address, 0, 0);
            this.table_nvMemCtrl_rwRegisters.Controls.Add(this.lbl_nvMemCtrl_val, 0, 1);
            this.table_nvMemCtrl_rwRegisters.Controls.Add(this.lbl_nvMemCtrl_mask, 0, 2);
            this.table_nvMemCtrl_rwRegisters.Controls.Add(this.txtBox_nvMemCtrl_regValue, 1, 2);
            this.table_nvMemCtrl_rwRegisters.Controls.Add(this.txtBox_nvMemCtrl_regMask, 1, 3);
            this.table_nvMemCtrl_rwRegisters.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.table_nvMemCtrl_rwRegisters.Location = new System.Drawing.Point(10, 30);
            this.table_nvMemCtrl_rwRegisters.Margin = new System.Windows.Forms.Padding(4);
            this.table_nvMemCtrl_rwRegisters.Name = "table_nvMemCtrl_rwRegisters";
            this.table_nvMemCtrl_rwRegisters.RowCount = 4;
            this.table_nvMemCtrl_rwRegisters.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.table_nvMemCtrl_rwRegisters.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.table_nvMemCtrl_rwRegisters.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.table_nvMemCtrl_rwRegisters.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 38F));
            this.table_nvMemCtrl_rwRegisters.Size = new System.Drawing.Size(272, 165);
            this.table_nvMemCtrl_rwRegisters.TabIndex = 0;
            // 
            // label13
            // 
            this.label13.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(28, 135);
            this.label13.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(54, 20);
            this.label13.TabIndex = 4;
            this.label13.Text = "Mask:";
            // 
            // txtBox_nvMemCtrl_regModule
            // 
            this.txtBox_nvMemCtrl_regModule.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_regModule.Location = new System.Drawing.Point(90, 8);
            this.txtBox_nvMemCtrl_regModule.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_regModule.Name = "txtBox_nvMemCtrl_regModule";
            this.txtBox_nvMemCtrl_regModule.Size = new System.Drawing.Size(178, 26);
            this.txtBox_nvMemCtrl_regModule.TabIndex = 0;
            // 
            // txtBox_nvMemCtrl_regAddress
            // 
            this.txtBox_nvMemCtrl_regAddress.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_regAddress.Location = new System.Drawing.Point(90, 50);
            this.txtBox_nvMemCtrl_regAddress.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_regAddress.Name = "txtBox_nvMemCtrl_regAddress";
            this.txtBox_nvMemCtrl_regAddress.Size = new System.Drawing.Size(178, 26);
            this.txtBox_nvMemCtrl_regAddress.TabIndex = 1;
            // 
            // lbl_nvMemCtrl_address
            // 
            this.lbl_nvMemCtrl_address.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_address.AutoSize = true;
            this.lbl_nvMemCtrl_address.Location = new System.Drawing.Point(9, 11);
            this.lbl_nvMemCtrl_address.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_address.Name = "lbl_nvMemCtrl_address";
            this.lbl_nvMemCtrl_address.Size = new System.Drawing.Size(73, 20);
            this.lbl_nvMemCtrl_address.TabIndex = 1;
            this.lbl_nvMemCtrl_address.Text = "Module: ";
            // 
            // lbl_nvMemCtrl_val
            // 
            this.lbl_nvMemCtrl_val.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_val.AutoSize = true;
            this.lbl_nvMemCtrl_val.Location = new System.Drawing.Point(6, 53);
            this.lbl_nvMemCtrl_val.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_val.Name = "lbl_nvMemCtrl_val";
            this.lbl_nvMemCtrl_val.Size = new System.Drawing.Size(76, 20);
            this.lbl_nvMemCtrl_val.TabIndex = 1;
            this.lbl_nvMemCtrl_val.Text = "Address: ";
            // 
            // lbl_nvMemCtrl_mask
            // 
            this.lbl_nvMemCtrl_mask.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_mask.AutoSize = true;
            this.lbl_nvMemCtrl_mask.Location = new System.Drawing.Point(21, 95);
            this.lbl_nvMemCtrl_mask.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_mask.Name = "lbl_nvMemCtrl_mask";
            this.lbl_nvMemCtrl_mask.Size = new System.Drawing.Size(61, 20);
            this.lbl_nvMemCtrl_mask.TabIndex = 1;
            this.lbl_nvMemCtrl_mask.Text = "Value: ";
            // 
            // txtBox_nvMemCtrl_regValue
            // 
            this.txtBox_nvMemCtrl_regValue.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_regValue.Location = new System.Drawing.Point(90, 92);
            this.txtBox_nvMemCtrl_regValue.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_regValue.Name = "txtBox_nvMemCtrl_regValue";
            this.txtBox_nvMemCtrl_regValue.Size = new System.Drawing.Size(178, 26);
            this.txtBox_nvMemCtrl_regValue.TabIndex = 2;
            // 
            // txtBox_nvMemCtrl_regMask
            // 
            this.txtBox_nvMemCtrl_regMask.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_regMask.Location = new System.Drawing.Point(90, 132);
            this.txtBox_nvMemCtrl_regMask.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_regMask.Name = "txtBox_nvMemCtrl_regMask";
            this.txtBox_nvMemCtrl_regMask.Size = new System.Drawing.Size(178, 26);
            this.txtBox_nvMemCtrl_regMask.TabIndex = 3;
            // 
            // groupBox2
            // 
            this.groupBox2.BackColor = System.Drawing.Color.White;
            this.groupBox2.Controls.Add(this.cmd_nvMemCtrl_allRFRW);
            this.groupBox2.Controls.Add(this.cmd_nvMemCtrl_writeRF);
            this.groupBox2.Controls.Add(this.cmd_nvMemCtrl_readRF);
            this.groupBox2.Controls.Add(this.tableLayoutPanel1);
            this.groupBox2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox2.Location = new System.Drawing.Point(616, 428);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox2.Size = new System.Drawing.Size(531, 306);
            this.groupBox2.TabIndex = 35;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Read/Write RF Registers";
            // 
            // cmd_nvMemCtrl_allRFRW
            // 
            this.cmd_nvMemCtrl_allRFRW.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_nvMemCtrl_allRFRW.Location = new System.Drawing.Point(336, 194);
            this.cmd_nvMemCtrl_allRFRW.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_nvMemCtrl_allRFRW.Name = "cmd_nvMemCtrl_allRFRW";
            this.cmd_nvMemCtrl_allRFRW.Size = new System.Drawing.Size(159, 80);
            this.cmd_nvMemCtrl_allRFRW.TabIndex = 4;
            this.cmd_nvMemCtrl_allRFRW.Text = "All Register View/Edit";
            this.cmd_nvMemCtrl_allRFRW.UseVisualStyleBackColor = true;
            this.cmd_nvMemCtrl_allRFRW.Click += new System.EventHandler(this.cmd_nvMemCtrl_allRFRW_Click);
            // 
            // cmd_nvMemCtrl_writeRF
            // 
            this.cmd_nvMemCtrl_writeRF.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_nvMemCtrl_writeRF.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_nvMemCtrl_writeRF.Location = new System.Drawing.Point(336, 111);
            this.cmd_nvMemCtrl_writeRF.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_nvMemCtrl_writeRF.Name = "cmd_nvMemCtrl_writeRF";
            this.cmd_nvMemCtrl_writeRF.Size = new System.Drawing.Size(159, 51);
            this.cmd_nvMemCtrl_writeRF.TabIndex = 3;
            this.cmd_nvMemCtrl_writeRF.Text = "Write";
            this.cmd_nvMemCtrl_writeRF.UseVisualStyleBackColor = true;
            this.cmd_nvMemCtrl_writeRF.Click += new System.EventHandler(this.cmd_nvMemCtrl_writeRF_Click);
            // 
            // cmd_nvMemCtrl_readRF
            // 
            this.cmd_nvMemCtrl_readRF.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_nvMemCtrl_readRF.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_nvMemCtrl_readRF.Location = new System.Drawing.Point(336, 39);
            this.cmd_nvMemCtrl_readRF.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_nvMemCtrl_readRF.Name = "cmd_nvMemCtrl_readRF";
            this.cmd_nvMemCtrl_readRF.Size = new System.Drawing.Size(159, 52);
            this.cmd_nvMemCtrl_readRF.TabIndex = 2;
            this.cmd_nvMemCtrl_readRF.Text = "Read";
            this.cmd_nvMemCtrl_readRF.UseVisualStyleBackColor = true;
            this.cmd_nvMemCtrl_readRF.Click += new System.EventHandler(this.cmd_nvMemCtrl_readRF_Click);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 31.93717F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 68.06283F));
            this.tableLayoutPanel1.Controls.Add(this.lbl_nvMemCtrl_val4, 0, 5);
            this.tableLayoutPanel1.Controls.Add(this.label7, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.txtBox_nvMemCtrl_RFval2, 1, 3);
            this.tableLayoutPanel1.Controls.Add(this.txtBox_nvMemCtrl_RFaddress, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.txtBox_nvMemCtrl_RFval0, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.label8, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.label9, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.label11, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.txtBox_nvMemCtrl_RFval1, 1, 2);
            this.tableLayoutPanel1.Controls.Add(this.label12, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.txtBox_nvMemCtrl_RFval3, 1, 4);
            this.tableLayoutPanel1.Controls.Add(this.txtBox_nvMemCtrl_RFval4, 1, 5);
            this.tableLayoutPanel1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(10, 32);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(4);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 6;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(272, 232);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // lbl_nvMemCtrl_val4
            // 
            this.lbl_nvMemCtrl_val4.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_val4.AutoSize = true;
            this.lbl_nvMemCtrl_val4.Location = new System.Drawing.Point(12, 191);
            this.lbl_nvMemCtrl_val4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_val4.Name = "lbl_nvMemCtrl_val4";
            this.lbl_nvMemCtrl_val4.Size = new System.Drawing.Size(70, 20);
            this.lbl_nvMemCtrl_val4.TabIndex = 7;
            this.lbl_nvMemCtrl_val4.Text = "Value 4:";
            // 
            // label7
            // 
            this.label7.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(12, 109);
            this.label7.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(70, 20);
            this.label7.TabIndex = 4;
            this.label7.Text = "Value 2:";
            // 
            // txtBox_nvMemCtrl_RFval2
            // 
            this.txtBox_nvMemCtrl_RFval2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_RFval2.Location = new System.Drawing.Point(90, 106);
            this.txtBox_nvMemCtrl_RFval2.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_RFval2.Name = "txtBox_nvMemCtrl_RFval2";
            this.txtBox_nvMemCtrl_RFval2.Size = new System.Drawing.Size(178, 26);
            this.txtBox_nvMemCtrl_RFval2.TabIndex = 3;
            // 
            // txtBox_nvMemCtrl_RFaddress
            // 
            this.txtBox_nvMemCtrl_RFaddress.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_RFaddress.Location = new System.Drawing.Point(90, 4);
            this.txtBox_nvMemCtrl_RFaddress.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_RFaddress.Name = "txtBox_nvMemCtrl_RFaddress";
            this.txtBox_nvMemCtrl_RFaddress.Size = new System.Drawing.Size(178, 26);
            this.txtBox_nvMemCtrl_RFaddress.TabIndex = 0;
            // 
            // txtBox_nvMemCtrl_RFval0
            // 
            this.txtBox_nvMemCtrl_RFval0.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_RFval0.Location = new System.Drawing.Point(90, 38);
            this.txtBox_nvMemCtrl_RFval0.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_RFval0.Name = "txtBox_nvMemCtrl_RFval0";
            this.txtBox_nvMemCtrl_RFval0.Size = new System.Drawing.Size(178, 26);
            this.txtBox_nvMemCtrl_RFval0.TabIndex = 1;
            // 
            // label8
            // 
            this.label8.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 7);
            this.label8.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(76, 20);
            this.label8.TabIndex = 1;
            this.label8.Text = "Address:";
            // 
            // label9
            // 
            this.label9.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(12, 41);
            this.label9.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(70, 20);
            this.label9.TabIndex = 1;
            this.label9.Text = "Value 0:";
            // 
            // label11
            // 
            this.label11.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(12, 75);
            this.label11.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(70, 20);
            this.label11.TabIndex = 1;
            this.label11.Text = "Value 1:";
            // 
            // txtBox_nvMemCtrl_RFval1
            // 
            this.txtBox_nvMemCtrl_RFval1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_RFval1.Location = new System.Drawing.Point(90, 72);
            this.txtBox_nvMemCtrl_RFval1.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_RFval1.Name = "txtBox_nvMemCtrl_RFval1";
            this.txtBox_nvMemCtrl_RFval1.Size = new System.Drawing.Size(178, 26);
            this.txtBox_nvMemCtrl_RFval1.TabIndex = 2;
            // 
            // label12
            // 
            this.label12.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(12, 143);
            this.label12.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(70, 20);
            this.label12.TabIndex = 5;
            this.label12.Text = "Value 3:";
            // 
            // txtBox_nvMemCtrl_RFval3
            // 
            this.txtBox_nvMemCtrl_RFval3.Location = new System.Drawing.Point(90, 140);
            this.txtBox_nvMemCtrl_RFval3.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_RFval3.Name = "txtBox_nvMemCtrl_RFval3";
            this.txtBox_nvMemCtrl_RFval3.Size = new System.Drawing.Size(175, 26);
            this.txtBox_nvMemCtrl_RFval3.TabIndex = 6;
            // 
            // txtBox_nvMemCtrl_RFval4
            // 
            this.txtBox_nvMemCtrl_RFval4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_RFval4.Location = new System.Drawing.Point(93, 202);
            this.txtBox_nvMemCtrl_RFval4.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_RFval4.Name = "txtBox_nvMemCtrl_RFval4";
            this.txtBox_nvMemCtrl_RFval4.Size = new System.Drawing.Size(175, 26);
            this.txtBox_nvMemCtrl_RFval4.TabIndex = 8;
            // 
            // groupBox_antennaPhase
            // 
            this.groupBox_antennaPhase.BackColor = System.Drawing.Color.White;
            this.groupBox_antennaPhase.Controls.Add(this.comboBox_extendedOp_antNum);
            this.groupBox_antennaPhase.Controls.Add(this.txtBox_extendedOp_offset2);
            this.groupBox_antennaPhase.Controls.Add(this.label6);
            this.groupBox_antennaPhase.Controls.Add(this.txtBox_extendedOp_offset3);
            this.groupBox_antennaPhase.Controls.Add(this.label5);
            this.groupBox_antennaPhase.Controls.Add(this.txtBox_extendedOp_offset1);
            this.groupBox_antennaPhase.Controls.Add(this.label3);
            this.groupBox_antennaPhase.Controls.Add(this.label2);
            this.groupBox_antennaPhase.Controls.Add(this.cmd_extendedOp_CDDget);
            this.groupBox_antennaPhase.Controls.Add(this.cmd_extendedOp_CDDset);
            this.groupBox_antennaPhase.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox_antennaPhase.Location = new System.Drawing.Point(9, 10);
            this.groupBox_antennaPhase.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_antennaPhase.Name = "groupBox_antennaPhase";
            this.groupBox_antennaPhase.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_antennaPhase.Size = new System.Drawing.Size(598, 194);
            this.groupBox_antennaPhase.TabIndex = 8;
            this.groupBox_antennaPhase.TabStop = false;
            this.groupBox_antennaPhase.Text = "BBIC CDD";
            // 
            // comboBox_extendedOp_antNum
            // 
            this.comboBox_extendedOp_antNum.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.comboBox_extendedOp_antNum.FormattingEnabled = true;
            this.comboBox_extendedOp_antNum.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4"});
            this.comboBox_extendedOp_antNum.Location = new System.Drawing.Point(160, 36);
            this.comboBox_extendedOp_antNum.Margin = new System.Windows.Forms.Padding(4);
            this.comboBox_extendedOp_antNum.Name = "comboBox_extendedOp_antNum";
            this.comboBox_extendedOp_antNum.Size = new System.Drawing.Size(92, 28);
            this.comboBox_extendedOp_antNum.TabIndex = 12;
            this.comboBox_extendedOp_antNum.SelectedIndexChanged += new System.EventHandler(this.comboBox_extendedOp_antNum_SelectedIndexChanged);
            // 
            // txtBox_extendedOp_offset2
            // 
            this.txtBox_extendedOp_offset2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_extendedOp_offset2.Location = new System.Drawing.Point(160, 114);
            this.txtBox_extendedOp_offset2.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_extendedOp_offset2.Name = "txtBox_extendedOp_offset2";
            this.txtBox_extendedOp_offset2.Size = new System.Drawing.Size(92, 26);
            this.txtBox_extendedOp_offset2.TabIndex = 11;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.label6.Location = new System.Drawing.Point(76, 118);
            this.label6.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(74, 20);
            this.label6.TabIndex = 10;
            this.label6.Text = "Offset 2:";
            // 
            // txtBox_extendedOp_offset3
            // 
            this.txtBox_extendedOp_offset3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_extendedOp_offset3.Location = new System.Drawing.Point(160, 152);
            this.txtBox_extendedOp_offset3.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_extendedOp_offset3.Name = "txtBox_extendedOp_offset3";
            this.txtBox_extendedOp_offset3.Size = new System.Drawing.Size(92, 26);
            this.txtBox_extendedOp_offset3.TabIndex = 9;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.label5.Location = new System.Drawing.Point(76, 154);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(74, 20);
            this.label5.TabIndex = 8;
            this.label5.Text = "Offset 3:";
            // 
            // txtBox_extendedOp_offset1
            // 
            this.txtBox_extendedOp_offset1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_extendedOp_offset1.Location = new System.Drawing.Point(160, 75);
            this.txtBox_extendedOp_offset1.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_extendedOp_offset1.Name = "txtBox_extendedOp_offset1";
            this.txtBox_extendedOp_offset1.Size = new System.Drawing.Size(92, 26);
            this.txtBox_extendedOp_offset1.TabIndex = 7;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.label3.Location = new System.Drawing.Point(76, 86);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(74, 20);
            this.label3.TabIndex = 6;
            this.label3.Text = "Offset 1:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.label2.Location = new System.Drawing.Point(39, 39);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(115, 20);
            this.label2.TabIndex = 4;
            this.label2.Text = "Antenna Num:";
            // 
            // cmd_extendedOp_CDDget
            // 
            this.cmd_extendedOp_CDDget.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_extendedOp_CDDget.Location = new System.Drawing.Point(309, 118);
            this.cmd_extendedOp_CDDget.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_extendedOp_CDDget.Name = "cmd_extendedOp_CDDget";
            this.cmd_extendedOp_CDDget.Size = new System.Drawing.Size(180, 51);
            this.cmd_extendedOp_CDDget.TabIndex = 3;
            this.cmd_extendedOp_CDDget.Text = "Get CDD";
            this.cmd_extendedOp_CDDget.UseVisualStyleBackColor = true;
            this.cmd_extendedOp_CDDget.Click += new System.EventHandler(this.cmd_extendedOp_CDDget_Click);
            // 
            // cmd_extendedOp_CDDset
            // 
            this.cmd_extendedOp_CDDset.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_extendedOp_CDDset.Location = new System.Drawing.Point(309, 42);
            this.cmd_extendedOp_CDDset.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_extendedOp_CDDset.Name = "cmd_extendedOp_CDDset";
            this.cmd_extendedOp_CDDset.Size = new System.Drawing.Size(180, 56);
            this.cmd_extendedOp_CDDset.TabIndex = 2;
            this.cmd_extendedOp_CDDset.Text = "Set CDD";
            this.cmd_extendedOp_CDDset.UseVisualStyleBackColor = true;
            this.cmd_extendedOp_CDDset.Click += new System.EventHandler(this.CddSetIndexButton_Click);
            // 
            // groupBox_xtalCalib
            // 
            this.groupBox_xtalCalib.BackColor = System.Drawing.Color.White;
            this.groupBox_xtalCalib.Controls.Add(this.lbl_extendedOp_xtalReg);
            this.groupBox_xtalCalib.Controls.Add(this.table_extendedOp_xtalRegister);
            this.groupBox_xtalCalib.Controls.Add(this.lbl_extendedOp_xtalBias);
            this.groupBox_xtalCalib.Controls.Add(this.num_extendedOp_xtalCalBias);
            this.groupBox_xtalCalib.Controls.Add(this.lbl_extendedOp_xtalVal);
            this.groupBox_xtalCalib.Controls.Add(this.num_extendedOp_xtalCalValue);
            this.groupBox_xtalCalib.Controls.Add(this.lbl_extendedOp_xtalRegAutoUpdate);
            this.groupBox_xtalCalib.Controls.Add(this.cmd_extendedOp_xtalCalWriteEEPROM);
            this.groupBox_xtalCalib.Controls.Add(this.cmd_extendedOp_xtalRegisterRead);
            this.groupBox_xtalCalib.Controls.Add(this.cmd_extendedOp_xtalCalReadEEPROM);
            this.groupBox_xtalCalib.Controls.Add(this.check_extendedOp_xtalEnable);
            this.groupBox_xtalCalib.Controls.Add(this.cmd_extendedOp_xtalRegisterWrite);
            this.groupBox_xtalCalib.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox_xtalCalib.Location = new System.Drawing.Point(616, 10);
            this.groupBox_xtalCalib.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_xtalCalib.Name = "groupBox_xtalCalib";
            this.groupBox_xtalCalib.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_xtalCalib.Size = new System.Drawing.Size(531, 410);
            this.groupBox_xtalCalib.TabIndex = 7;
            this.groupBox_xtalCalib.TabStop = false;
            this.groupBox_xtalCalib.Text = "Xtal Calibration";
            // 
            // lbl_extendedOp_xtalReg
            // 
            this.lbl_extendedOp_xtalReg.AutoSize = true;
            this.lbl_extendedOp_xtalReg.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.lbl_extendedOp_xtalReg.Location = new System.Drawing.Point(57, 142);
            this.lbl_extendedOp_xtalReg.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_extendedOp_xtalReg.Name = "lbl_extendedOp_xtalReg";
            this.lbl_extendedOp_xtalReg.Size = new System.Drawing.Size(132, 20);
            this.lbl_extendedOp_xtalReg.TabIndex = 26;
            this.lbl_extendedOp_xtalReg.Text = "XTAL Register";
            // 
            // table_extendedOp_xtalRegister
            // 
            this.table_extendedOp_xtalRegister.ColumnCount = 2;
            this.table_extendedOp_xtalRegister.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 48.55491F));
            this.table_extendedOp_xtalRegister.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 51.44509F));
            this.table_extendedOp_xtalRegister.Controls.Add(this.lbl_extendedOp_xtalVal_reg, 1, 0);
            this.table_extendedOp_xtalRegister.Controls.Add(this.num_extendedOp_xtalRegisterValue, 1, 1);
            this.table_extendedOp_xtalRegister.Controls.Add(this.num_extendedOp_xtalRegisterBias, 0, 1);
            this.table_extendedOp_xtalRegister.Controls.Add(this.lbl_extendedOp_xtalBias_reg, 0, 0);
            this.table_extendedOp_xtalRegister.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.table_extendedOp_xtalRegister.Location = new System.Drawing.Point(57, 188);
            this.table_extendedOp_xtalRegister.Margin = new System.Windows.Forms.Padding(4);
            this.table_extendedOp_xtalRegister.Name = "table_extendedOp_xtalRegister";
            this.table_extendedOp_xtalRegister.RowCount = 2;
            this.table_extendedOp_xtalRegister.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.table_extendedOp_xtalRegister.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.table_extendedOp_xtalRegister.Size = new System.Drawing.Size(260, 93);
            this.table_extendedOp_xtalRegister.TabIndex = 25;
            // 
            // lbl_extendedOp_xtalVal_reg
            // 
            this.lbl_extendedOp_xtalVal_reg.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_extendedOp_xtalVal_reg.AutoSize = true;
            this.lbl_extendedOp_xtalVal_reg.Location = new System.Drawing.Point(130, 13);
            this.lbl_extendedOp_xtalVal_reg.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_extendedOp_xtalVal_reg.Name = "lbl_extendedOp_xtalVal_reg";
            this.lbl_extendedOp_xtalVal_reg.Size = new System.Drawing.Size(98, 20);
            this.lbl_extendedOp_xtalVal_reg.TabIndex = 22;
            this.lbl_extendedOp_xtalVal_reg.Text = "XTAL Value";
            // 
            // num_extendedOp_xtalRegisterValue
            // 
            this.num_extendedOp_xtalRegisterValue.Hexadecimal = true;
            this.num_extendedOp_xtalRegisterValue.Location = new System.Drawing.Point(130, 50);
            this.num_extendedOp_xtalRegisterValue.Margin = new System.Windows.Forms.Padding(4);
            this.num_extendedOp_xtalRegisterValue.Maximum = new decimal(new int[] {
            511,
            0,
            0,
            0});
            this.num_extendedOp_xtalRegisterValue.Name = "num_extendedOp_xtalRegisterValue";
            this.num_extendedOp_xtalRegisterValue.Size = new System.Drawing.Size(114, 26);
            this.num_extendedOp_xtalRegisterValue.TabIndex = 19;
            this.num_extendedOp_xtalRegisterValue.Value = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.num_extendedOp_xtalRegisterValue.ValueChanged += new System.EventHandler(this.xtal_value_hex_ValueChanged);
            // 
            // num_extendedOp_xtalRegisterBias
            // 
            this.num_extendedOp_xtalRegisterBias.Hexadecimal = true;
            this.num_extendedOp_xtalRegisterBias.Location = new System.Drawing.Point(4, 50);
            this.num_extendedOp_xtalRegisterBias.Margin = new System.Windows.Forms.Padding(4);
            this.num_extendedOp_xtalRegisterBias.Maximum = new decimal(new int[] {
            127,
            0,
            0,
            0});
            this.num_extendedOp_xtalRegisterBias.Name = "num_extendedOp_xtalRegisterBias";
            this.num_extendedOp_xtalRegisterBias.Size = new System.Drawing.Size(116, 26);
            this.num_extendedOp_xtalRegisterBias.TabIndex = 20;
            this.num_extendedOp_xtalRegisterBias.Value = new decimal(new int[] {
            63,
            0,
            0,
            0});
            this.num_extendedOp_xtalRegisterBias.ValueChanged += new System.EventHandler(this.xtal_reg_bias_ValueChanged);
            // 
            // lbl_extendedOp_xtalBias_reg
            // 
            this.lbl_extendedOp_xtalBias_reg.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.lbl_extendedOp_xtalBias_reg.AutoSize = true;
            this.lbl_extendedOp_xtalBias_reg.Location = new System.Drawing.Point(4, 13);
            this.lbl_extendedOp_xtalBias_reg.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_extendedOp_xtalBias_reg.Name = "lbl_extendedOp_xtalBias_reg";
            this.lbl_extendedOp_xtalBias_reg.Size = new System.Drawing.Size(94, 20);
            this.lbl_extendedOp_xtalBias_reg.TabIndex = 21;
            this.lbl_extendedOp_xtalBias_reg.Text = "XTAL BIAS";
            // 
            // lbl_extendedOp_xtalBias
            // 
            this.lbl_extendedOp_xtalBias.AutoSize = true;
            this.lbl_extendedOp_xtalBias.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.lbl_extendedOp_xtalBias.Location = new System.Drawing.Point(186, 102);
            this.lbl_extendedOp_xtalBias.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_extendedOp_xtalBias.Name = "lbl_extendedOp_xtalBias";
            this.lbl_extendedOp_xtalBias.Size = new System.Drawing.Size(90, 20);
            this.lbl_extendedOp_xtalBias.TabIndex = 24;
            this.lbl_extendedOp_xtalBias.Text = "XTAL Bias";
            // 
            // num_extendedOp_xtalCalBias
            // 
            this.num_extendedOp_xtalCalBias.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.num_extendedOp_xtalCalBias.Location = new System.Drawing.Point(298, 96);
            this.num_extendedOp_xtalCalBias.Margin = new System.Windows.Forms.Padding(4);
            this.num_extendedOp_xtalCalBias.Maximum = new decimal(new int[] {
            127,
            0,
            0,
            0});
            this.num_extendedOp_xtalCalBias.Name = "num_extendedOp_xtalCalBias";
            this.num_extendedOp_xtalCalBias.Size = new System.Drawing.Size(90, 26);
            this.num_extendedOp_xtalCalBias.TabIndex = 23;
            this.num_extendedOp_xtalCalBias.ValueChanged += new System.EventHandler(this.xtal_bias_dec_ValueChanged);
            // 
            // lbl_extendedOp_xtalVal
            // 
            this.lbl_extendedOp_xtalVal.AutoSize = true;
            this.lbl_extendedOp_xtalVal.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.lbl_extendedOp_xtalVal.Location = new System.Drawing.Point(182, 52);
            this.lbl_extendedOp_xtalVal.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_extendedOp_xtalVal.Name = "lbl_extendedOp_xtalVal";
            this.lbl_extendedOp_xtalVal.Size = new System.Drawing.Size(98, 20);
            this.lbl_extendedOp_xtalVal.TabIndex = 21;
            this.lbl_extendedOp_xtalVal.Text = "XTAL Value";
            // 
            // num_extendedOp_xtalCalValue
            // 
            this.num_extendedOp_xtalCalValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.num_extendedOp_xtalCalValue.Location = new System.Drawing.Point(298, 45);
            this.num_extendedOp_xtalCalValue.Margin = new System.Windows.Forms.Padding(4);
            this.num_extendedOp_xtalCalValue.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.num_extendedOp_xtalCalValue.Minimum = new decimal(new int[] {
            256,
            0,
            0,
            -2147483648});
            this.num_extendedOp_xtalCalValue.Name = "num_extendedOp_xtalCalValue";
            this.num_extendedOp_xtalCalValue.Size = new System.Drawing.Size(92, 26);
            this.num_extendedOp_xtalCalValue.TabIndex = 18;
            this.num_extendedOp_xtalCalValue.Value = new decimal(new int[] {
            255,
            0,
            0,
            -2147483648});
            this.num_extendedOp_xtalCalValue.ValueChanged += new System.EventHandler(this.xtal_value_dec_ValueChanged);
            // 
            // lbl_extendedOp_xtalRegAutoUpdate
            // 
            this.lbl_extendedOp_xtalRegAutoUpdate.AutoSize = true;
            this.lbl_extendedOp_xtalRegAutoUpdate.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.lbl_extendedOp_xtalRegAutoUpdate.Location = new System.Drawing.Point(57, 303);
            this.lbl_extendedOp_xtalRegAutoUpdate.Margin = new System.Windows.Forms.Padding(4);
            this.lbl_extendedOp_xtalRegAutoUpdate.Name = "lbl_extendedOp_xtalRegAutoUpdate";
            this.lbl_extendedOp_xtalRegAutoUpdate.Size = new System.Drawing.Size(230, 24);
            this.lbl_extendedOp_xtalRegAutoUpdate.TabIndex = 16;
            this.lbl_extendedOp_xtalRegAutoUpdate.Text = "Xtal Register Auto-Update";
            this.lbl_extendedOp_xtalRegAutoUpdate.UseVisualStyleBackColor = true;
            // 
            // cmd_extendedOp_xtalCalWriteEEPROM
            // 
            this.cmd_extendedOp_xtalCalWriteEEPROM.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_extendedOp_xtalCalWriteEEPROM.Location = new System.Drawing.Point(284, 354);
            this.cmd_extendedOp_xtalCalWriteEEPROM.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_extendedOp_xtalCalWriteEEPROM.Name = "cmd_extendedOp_xtalCalWriteEEPROM";
            this.cmd_extendedOp_xtalCalWriteEEPROM.Size = new System.Drawing.Size(180, 38);
            this.cmd_extendedOp_xtalCalWriteEEPROM.TabIndex = 9;
            this.cmd_extendedOp_xtalCalWriteEEPROM.Text = "Write EEPROM";
            this.cmd_extendedOp_xtalCalWriteEEPROM.UseVisualStyleBackColor = true;
            this.cmd_extendedOp_xtalCalWriteEEPROM.Click += new System.EventHandler(this.xtal_write_eeprom_Click);
            // 
            // cmd_extendedOp_xtalRegisterRead
            // 
            this.cmd_extendedOp_xtalRegisterRead.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_extendedOp_xtalRegisterRead.Location = new System.Drawing.Point(351, 246);
            this.cmd_extendedOp_xtalRegisterRead.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_extendedOp_xtalRegisterRead.Name = "cmd_extendedOp_xtalRegisterRead";
            this.cmd_extendedOp_xtalRegisterRead.Size = new System.Drawing.Size(112, 34);
            this.cmd_extendedOp_xtalRegisterRead.TabIndex = 5;
            this.cmd_extendedOp_xtalRegisterRead.Text = "Read Reg";
            this.cmd_extendedOp_xtalRegisterRead.UseVisualStyleBackColor = true;
            this.cmd_extendedOp_xtalRegisterRead.Click += new System.EventHandler(this.read_xtal_Click);
            // 
            // cmd_extendedOp_xtalCalReadEEPROM
            // 
            this.cmd_extendedOp_xtalCalReadEEPROM.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_extendedOp_xtalCalReadEEPROM.Location = new System.Drawing.Point(57, 354);
            this.cmd_extendedOp_xtalCalReadEEPROM.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_extendedOp_xtalCalReadEEPROM.Name = "cmd_extendedOp_xtalCalReadEEPROM";
            this.cmd_extendedOp_xtalCalReadEEPROM.Size = new System.Drawing.Size(180, 38);
            this.cmd_extendedOp_xtalCalReadEEPROM.TabIndex = 8;
            this.cmd_extendedOp_xtalCalReadEEPROM.Text = "Read EEPROM";
            this.cmd_extendedOp_xtalCalReadEEPROM.UseVisualStyleBackColor = true;
            this.cmd_extendedOp_xtalCalReadEEPROM.Click += new System.EventHandler(this.xtal_read_eeprom_Click);
            // 
            // check_extendedOp_xtalEnable
            // 
            this.check_extendedOp_xtalEnable.AutoSize = true;
            this.check_extendedOp_xtalEnable.Checked = true;
            this.check_extendedOp_xtalEnable.CheckState = System.Windows.Forms.CheckState.Checked;
            this.check_extendedOp_xtalEnable.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.check_extendedOp_xtalEnable.Location = new System.Drawing.Point(57, 46);
            this.check_extendedOp_xtalEnable.Margin = new System.Windows.Forms.Padding(4);
            this.check_extendedOp_xtalEnable.Name = "check_extendedOp_xtalEnable";
            this.check_extendedOp_xtalEnable.Size = new System.Drawing.Size(86, 24);
            this.check_extendedOp_xtalEnable.TabIndex = 6;
            this.check_extendedOp_xtalEnable.Text = "Enable";
            this.check_extendedOp_xtalEnable.UseVisualStyleBackColor = true;
            this.check_extendedOp_xtalEnable.CheckedChanged += new System.EventHandler(this.xtal_enable_CheckedChanged);
            // 
            // cmd_extendedOp_xtalRegisterWrite
            // 
            this.cmd_extendedOp_xtalRegisterWrite.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_extendedOp_xtalRegisterWrite.Location = new System.Drawing.Point(351, 183);
            this.cmd_extendedOp_xtalRegisterWrite.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_extendedOp_xtalRegisterWrite.Name = "cmd_extendedOp_xtalRegisterWrite";
            this.cmd_extendedOp_xtalRegisterWrite.Size = new System.Drawing.Size(112, 34);
            this.cmd_extendedOp_xtalRegisterWrite.TabIndex = 2;
            this.cmd_extendedOp_xtalRegisterWrite.Text = "Write Reg";
            this.cmd_extendedOp_xtalRegisterWrite.UseVisualStyleBackColor = true;
            this.cmd_extendedOp_xtalRegisterWrite.Click += new System.EventHandler(this.send_xtal_Click);
            // 
            // nvMemControlTab
            // 
            this.nvMemControlTab.BackColor = System.Drawing.Color.White;
            this.nvMemControlTab.Controls.Add(this.groupBox_versionInfo);
            this.nvMemControlTab.Controls.Add(this.groupBox_burnEEPROM);
            this.nvMemControlTab.Controls.Add(this.groupBox_eepromInfo);
            this.nvMemControlTab.Enabled = false;
            this.nvMemControlTab.Location = new System.Drawing.Point(4, 29);
            this.nvMemControlTab.Margin = new System.Windows.Forms.Padding(4);
            this.nvMemControlTab.Name = "nvMemControlTab";
            this.nvMemControlTab.Padding = new System.Windows.Forms.Padding(4);
            this.nvMemControlTab.Size = new System.Drawing.Size(1225, 1063);
            this.nvMemControlTab.TabIndex = 1;
            this.nvMemControlTab.Text = "NV Memory Control & DUT Version";
            // 
            // groupBox_versionInfo
            // 
            this.groupBox_versionInfo.BackColor = System.Drawing.Color.White;
            this.groupBox_versionInfo.Controls.Add(this.table_nvMemCtrl_versionInfo);
            this.groupBox_versionInfo.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox_versionInfo.Location = new System.Drawing.Point(741, 9);
            this.groupBox_versionInfo.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_versionInfo.Name = "groupBox_versionInfo";
            this.groupBox_versionInfo.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_versionInfo.Size = new System.Drawing.Size(402, 350);
            this.groupBox_versionInfo.TabIndex = 12;
            this.groupBox_versionInfo.TabStop = false;
            this.groupBox_versionInfo.Text = "Version Information";
            // 
            // groupBox_eepromInfo
            // 
            this.groupBox_eepromInfo.BackColor = System.Drawing.Color.White;
            this.groupBox_eepromInfo.Controls.Add(this.groupBox_nvMem_prodInfo);
            this.groupBox_eepromInfo.Controls.Add(this.cmd_nvMemCtrl_readExtCalFile);
            this.groupBox_eepromInfo.Controls.Add(this.groupBox_eepromBarParse);
            this.groupBox_eepromInfo.Controls.Add(this.burnBarcodeAndMAC);
            this.groupBox_eepromInfo.Controls.Add(this.cmd_nvMemCtrl_readCalFileInfo);
            this.groupBox_eepromInfo.Controls.Add(this.cmd_nvMemCtrl_viewEEPROM);
            this.groupBox_eepromInfo.Controls.Add(this.table_nvMemCtrl_currentEEPROM);
            this.groupBox_eepromInfo.Location = new System.Drawing.Point(3, 192);
            this.groupBox_eepromInfo.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_eepromInfo.Name = "groupBox_eepromInfo";
            this.groupBox_eepromInfo.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_eepromInfo.Size = new System.Drawing.Size(724, 618);
            this.groupBox_eepromInfo.TabIndex = 13;
            this.groupBox_eepromInfo.TabStop = false;
            this.groupBox_eepromInfo.Text = "Current EEPROM Information";
            // 
            // groupBox_nvMem_prodInfo
            // 
            this.groupBox_nvMem_prodInfo.Controls.Add(this.combox_nvMemCtrl_prodValue);
            this.groupBox_nvMem_prodInfo.Controls.Add(this.label4);
            this.groupBox_nvMem_prodInfo.Controls.Add(this.cmd_nvMemCtrl_burnProdFlag);
            this.groupBox_nvMem_prodInfo.Controls.Add(this.combox_nvMemCtrl_prodFlag);
            this.groupBox_nvMem_prodInfo.Controls.Add(this.label1);
            this.groupBox_nvMem_prodInfo.Location = new System.Drawing.Point(9, 399);
            this.groupBox_nvMem_prodInfo.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_nvMem_prodInfo.Name = "groupBox_nvMem_prodInfo";
            this.groupBox_nvMem_prodInfo.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_nvMem_prodInfo.Size = new System.Drawing.Size(710, 75);
            this.groupBox_nvMem_prodInfo.TabIndex = 18;
            this.groupBox_nvMem_prodInfo.TabStop = false;
            this.groupBox_nvMem_prodInfo.Text = "Production info";
            // 
            // combox_nvMemCtrl_prodValue
            // 
            this.combox_nvMemCtrl_prodValue.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.combox_nvMemCtrl_prodValue.BackColor = System.Drawing.Color.PaleGoldenrod;
            this.combox_nvMemCtrl_prodValue.Location = new System.Drawing.Point(324, 33);
            this.combox_nvMemCtrl_prodValue.Margin = new System.Windows.Forms.Padding(4);
            this.combox_nvMemCtrl_prodValue.Name = "combox_nvMemCtrl_prodValue";
            this.combox_nvMemCtrl_prodValue.ReadOnly = true;
            this.combox_nvMemCtrl_prodValue.Size = new System.Drawing.Size(50, 26);
            this.combox_nvMemCtrl_prodValue.TabIndex = 7;
            // 
            // label4
            // 
            this.label4.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(228, 38);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(86, 20);
            this.label4.TabIndex = 6;
            this.label4.Text = "Byte value:";
            // 
            // cmd_nvMemCtrl_burnProdFlag
            // 
            this.cmd_nvMemCtrl_burnProdFlag.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_nvMemCtrl_burnProdFlag.Location = new System.Drawing.Point(402, 27);
            this.cmd_nvMemCtrl_burnProdFlag.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_nvMemCtrl_burnProdFlag.Name = "cmd_nvMemCtrl_burnProdFlag";
            this.cmd_nvMemCtrl_burnProdFlag.Size = new System.Drawing.Size(290, 39);
            this.cmd_nvMemCtrl_burnProdFlag.TabIndex = 5;
            this.cmd_nvMemCtrl_burnProdFlag.Text = "Burn Production Flag";
            this.cmd_nvMemCtrl_burnProdFlag.UseVisualStyleBackColor = true;
            this.cmd_nvMemCtrl_burnProdFlag.Click += new System.EventHandler(this.cmd_nvMemCtrl_burnProdFlag_Click);
            // 
            // combox_nvMemCtrl_prodFlag
            // 
            this.combox_nvMemCtrl_prodFlag.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.combox_nvMemCtrl_prodFlag.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(245)))), ((int)(((byte)(252)))), ((int)(((byte)(199)))));
            this.combox_nvMemCtrl_prodFlag.FormattingEnabled = true;
            this.combox_nvMemCtrl_prodFlag.Items.AddRange(new object[] {
            "Production",
            "Operational"});
            this.combox_nvMemCtrl_prodFlag.Location = new System.Drawing.Point(78, 32);
            this.combox_nvMemCtrl_prodFlag.Margin = new System.Windows.Forms.Padding(4);
            this.combox_nvMemCtrl_prodFlag.Name = "combox_nvMemCtrl_prodFlag";
            this.combox_nvMemCtrl_prodFlag.Size = new System.Drawing.Size(139, 28);
            this.combox_nvMemCtrl_prodFlag.TabIndex = 4;
            this.combox_nvMemCtrl_prodFlag.SelectedIndexChanged += new System.EventHandler(this.combox_nvMemCtrl_prodFlag_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 36);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 20);
            this.label1.TabIndex = 1;
            this.label1.Text = "Mode:";
            // 
            // groupBox_eepromBarParse
            // 
            this.groupBox_eepromBarParse.Controls.Add(this.table_nvMemCtrl_EEPROMbarcodeParsing);
            this.groupBox_eepromBarParse.Location = new System.Drawing.Point(4, 201);
            this.groupBox_eepromBarParse.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox_eepromBarParse.Name = "groupBox_eepromBarParse";
            this.groupBox_eepromBarParse.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox_eepromBarParse.Size = new System.Drawing.Size(714, 192);
            this.groupBox_eepromBarParse.TabIndex = 15;
            this.groupBox_eepromBarParse.TabStop = false;
            this.groupBox_eepromBarParse.Text = "EEPROM Barcode Parsing";
            // 
            // table_nvMemCtrl_EEPROMbarcodeParsing
            // 
            this.table_nvMemCtrl_EEPROMbarcodeParsing.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.table_nvMemCtrl_EEPROMbarcodeParsing.BackColor = System.Drawing.Color.White;
            this.table_nvMemCtrl_EEPROMbarcodeParsing.ColumnCount = 2;
            this.table_nvMemCtrl_EEPROMbarcodeParsing.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 18.49462F));
            this.table_nvMemCtrl_EEPROMbarcodeParsing.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 81.50538F));
            this.table_nvMemCtrl_EEPROMbarcodeParsing.Controls.Add(this.lbl_nvMemCtrl_serial, 0, 0);
            this.table_nvMemCtrl_EEPROMbarcodeParsing.Controls.Add(this.lbl_nvMemCtrl_year, 0, 2);
            this.table_nvMemCtrl_EEPROMbarcodeParsing.Controls.Add(this.txtBox_nvMemCtrl_year, 1, 2);
            this.table_nvMemCtrl_EEPROMbarcodeParsing.Controls.Add(this.lbl_nvMemCtrl_week, 0, 1);
            this.table_nvMemCtrl_EEPROMbarcodeParsing.Controls.Add(this.txtBox_nvMemCtrl_week, 1, 1);
            this.table_nvMemCtrl_EEPROMbarcodeParsing.Controls.Add(this.txtBox_nvMemCtrl_serial, 1, 0);
            this.table_nvMemCtrl_EEPROMbarcodeParsing.Controls.Add(this.lbl_nvMemCtrl_country, 0, 3);
            this.table_nvMemCtrl_EEPROMbarcodeParsing.Controls.Add(this.txtBox_nvMemCtrl_country, 1, 3);
            this.table_nvMemCtrl_EEPROMbarcodeParsing.Location = new System.Drawing.Point(9, 28);
            this.table_nvMemCtrl_EEPROMbarcodeParsing.Margin = new System.Windows.Forms.Padding(4);
            this.table_nvMemCtrl_EEPROMbarcodeParsing.Name = "table_nvMemCtrl_EEPROMbarcodeParsing";
            this.table_nvMemCtrl_EEPROMbarcodeParsing.RowCount = 4;
            this.table_nvMemCtrl_EEPROMbarcodeParsing.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 48.97959F));
            this.table_nvMemCtrl_EEPROMbarcodeParsing.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 51.02041F));
            this.table_nvMemCtrl_EEPROMbarcodeParsing.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 39F));
            this.table_nvMemCtrl_EEPROMbarcodeParsing.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
            this.table_nvMemCtrl_EEPROMbarcodeParsing.Size = new System.Drawing.Size(698, 154);
            this.table_nvMemCtrl_EEPROMbarcodeParsing.TabIndex = 0;
            // 
            // lbl_nvMemCtrl_serial
            // 
            this.lbl_nvMemCtrl_serial.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_serial.AutoSize = true;
            this.lbl_nvMemCtrl_serial.Location = new System.Drawing.Point(12, 8);
            this.lbl_nvMemCtrl_serial.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_serial.Name = "lbl_nvMemCtrl_serial";
            this.lbl_nvMemCtrl_serial.Size = new System.Drawing.Size(113, 20);
            this.lbl_nvMemCtrl_serial.TabIndex = 2;
            this.lbl_nvMemCtrl_serial.Text = "Serial Number:";
            // 
            // lbl_nvMemCtrl_year
            // 
            this.lbl_nvMemCtrl_year.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_year.AutoSize = true;
            this.lbl_nvMemCtrl_year.Location = new System.Drawing.Point(78, 83);
            this.lbl_nvMemCtrl_year.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_year.Name = "lbl_nvMemCtrl_year";
            this.lbl_nvMemCtrl_year.Size = new System.Drawing.Size(47, 20);
            this.lbl_nvMemCtrl_year.TabIndex = 6;
            this.lbl_nvMemCtrl_year.Text = "Year:";
            // 
            // txtBox_nvMemCtrl_year
            // 
            this.txtBox_nvMemCtrl_year.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_nvMemCtrl_year.BackColor = System.Drawing.Color.PaleGoldenrod;
            this.txtBox_nvMemCtrl_year.Location = new System.Drawing.Point(133, 80);
            this.txtBox_nvMemCtrl_year.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_year.Name = "txtBox_nvMemCtrl_year";
            this.txtBox_nvMemCtrl_year.ReadOnly = true;
            this.txtBox_nvMemCtrl_year.Size = new System.Drawing.Size(61, 26);
            this.txtBox_nvMemCtrl_year.TabIndex = 7;
            // 
            // lbl_nvMemCtrl_week
            // 
            this.lbl_nvMemCtrl_week.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_week.AutoSize = true;
            this.lbl_nvMemCtrl_week.Location = new System.Drawing.Point(71, 45);
            this.lbl_nvMemCtrl_week.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_week.Name = "lbl_nvMemCtrl_week";
            this.lbl_nvMemCtrl_week.Size = new System.Drawing.Size(54, 20);
            this.lbl_nvMemCtrl_week.TabIndex = 4;
            this.lbl_nvMemCtrl_week.Text = "Week:";
            // 
            // txtBox_nvMemCtrl_week
            // 
            this.txtBox_nvMemCtrl_week.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.txtBox_nvMemCtrl_week.BackColor = System.Drawing.Color.PaleGoldenrod;
            this.txtBox_nvMemCtrl_week.Location = new System.Drawing.Point(133, 42);
            this.txtBox_nvMemCtrl_week.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_week.Name = "txtBox_nvMemCtrl_week";
            this.txtBox_nvMemCtrl_week.ReadOnly = true;
            this.txtBox_nvMemCtrl_week.Size = new System.Drawing.Size(61, 26);
            this.txtBox_nvMemCtrl_week.TabIndex = 5;
            // 
            // txtBox_nvMemCtrl_serial
            // 
            this.txtBox_nvMemCtrl_serial.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_serial.BackColor = System.Drawing.Color.PaleGoldenrod;
            this.txtBox_nvMemCtrl_serial.Location = new System.Drawing.Point(133, 5);
            this.txtBox_nvMemCtrl_serial.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_serial.Name = "txtBox_nvMemCtrl_serial";
            this.txtBox_nvMemCtrl_serial.ReadOnly = true;
            this.txtBox_nvMemCtrl_serial.Size = new System.Drawing.Size(561, 26);
            this.txtBox_nvMemCtrl_serial.TabIndex = 3;
            // 
            // lbl_nvMemCtrl_country
            // 
            this.lbl_nvMemCtrl_country.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_country.AutoSize = true;
            this.lbl_nvMemCtrl_country.Location = new System.Drawing.Point(57, 123);
            this.lbl_nvMemCtrl_country.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_country.Name = "lbl_nvMemCtrl_country";
            this.lbl_nvMemCtrl_country.Size = new System.Drawing.Size(68, 20);
            this.lbl_nvMemCtrl_country.TabIndex = 6;
            this.lbl_nvMemCtrl_country.Text = "Country:";
            // 
            // txtBox_nvMemCtrl_country
            // 
            this.txtBox_nvMemCtrl_country.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_country.BackColor = System.Drawing.Color.PaleGoldenrod;
            this.txtBox_nvMemCtrl_country.Location = new System.Drawing.Point(133, 120);
            this.txtBox_nvMemCtrl_country.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_country.Name = "txtBox_nvMemCtrl_country";
            this.txtBox_nvMemCtrl_country.ReadOnly = true;
            this.txtBox_nvMemCtrl_country.Size = new System.Drawing.Size(561, 26);
            this.txtBox_nvMemCtrl_country.TabIndex = 7;
            // 
            // burnBarcodeAndMAC
            // 
            this.burnBarcodeAndMAC.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.burnBarcodeAndMAC.Location = new System.Drawing.Point(52, 324);
            this.burnBarcodeAndMAC.Margin = new System.Windows.Forms.Padding(4);
            this.burnBarcodeAndMAC.Name = "burnBarcodeAndMAC";
            this.burnBarcodeAndMAC.Size = new System.Drawing.Size(627, 38);
            this.burnBarcodeAndMAC.TabIndex = 0;
            this.burnBarcodeAndMAC.Text = " Burn Barcode and MAC Address";
            this.burnBarcodeAndMAC.UseVisualStyleBackColor = true;
            this.burnBarcodeAndMAC.Click += new System.EventHandler(this.burnBarcodeAndMAC_Click);
            // 
            // cmd_nvMemCtrl_readCalFileInfo
            // 
            this.cmd_nvMemCtrl_readCalFileInfo.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_nvMemCtrl_readCalFileInfo.Location = new System.Drawing.Point(14, 549);
            this.cmd_nvMemCtrl_readCalFileInfo.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_nvMemCtrl_readCalFileInfo.Name = "cmd_nvMemCtrl_readCalFileInfo";
            this.cmd_nvMemCtrl_readCalFileInfo.Size = new System.Drawing.Size(294, 57);
            this.cmd_nvMemCtrl_readCalFileInfo.TabIndex = 0;
            this.cmd_nvMemCtrl_readCalFileInfo.Text = "Read Calibration File Information";
            this.cmd_nvMemCtrl_readCalFileInfo.UseVisualStyleBackColor = true;
            this.cmd_nvMemCtrl_readCalFileInfo.Click += new System.EventHandler(this.UpdateCurEEPROMInfo_Click);
            // 
            // table_nvMemCtrl_currentEEPROM
            // 
            this.table_nvMemCtrl_currentEEPROM.ColumnCount = 3;
            this.table_nvMemCtrl_currentEEPROM.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 16.19912F));
            this.table_nvMemCtrl_currentEEPROM.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 42.62441F));
            this.table_nvMemCtrl_currentEEPROM.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 41.17647F));
            this.table_nvMemCtrl_currentEEPROM.Controls.Add(this.txtBox_nvMemCtrl_barcode, 1, 0);
            this.table_nvMemCtrl_currentEEPROM.Controls.Add(this.txtBox_nvMemCtrl_MACaddr, 1, 1);
            this.table_nvMemCtrl_currentEEPROM.Controls.Add(this.lbl_nvMemCtrl_MACaddr, 0, 1);
            this.table_nvMemCtrl_currentEEPROM.Controls.Add(this.cmd_nvMemCtrl_burnCountryCode, 2, 2);
            this.table_nvMemCtrl_currentEEPROM.Controls.Add(this.lbl_nvMemCtrl_countryCurrent, 0, 2);
            this.table_nvMemCtrl_currentEEPROM.Controls.Add(this.combox_nvMemCtrl_country, 1, 2);
            this.table_nvMemCtrl_currentEEPROM.Controls.Add(this.check_nvMemCtrl_SNformatBarcode, 2, 0);
            this.table_nvMemCtrl_currentEEPROM.Controls.Add(this.lbl_nvMemCtrl_barcode, 0, 0);
            this.table_nvMemCtrl_currentEEPROM.Location = new System.Drawing.Point(9, 28);
            this.table_nvMemCtrl_currentEEPROM.Margin = new System.Windows.Forms.Padding(4);
            this.table_nvMemCtrl_currentEEPROM.Name = "table_nvMemCtrl_currentEEPROM";
            this.table_nvMemCtrl_currentEEPROM.RowCount = 3;
            this.table_nvMemCtrl_currentEEPROM.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 45.90164F));
            this.table_nvMemCtrl_currentEEPROM.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 54.09836F));
            this.table_nvMemCtrl_currentEEPROM.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 46F));
            this.table_nvMemCtrl_currentEEPROM.Size = new System.Drawing.Size(704, 138);
            this.table_nvMemCtrl_currentEEPROM.TabIndex = 2;
            // 
            // txtBox_nvMemCtrl_MACaddr
            // 
            this.txtBox_nvMemCtrl_MACaddr.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBox_nvMemCtrl_MACaddr.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(245)))), ((int)(((byte)(252)))), ((int)(((byte)(199)))));
            this.txtBox_nvMemCtrl_MACaddr.Location = new System.Drawing.Point(118, 53);
            this.txtBox_nvMemCtrl_MACaddr.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_nvMemCtrl_MACaddr.Name = "txtBox_nvMemCtrl_MACaddr";
            this.txtBox_nvMemCtrl_MACaddr.Size = new System.Drawing.Size(292, 26);
            this.txtBox_nvMemCtrl_MACaddr.TabIndex = 1;
            // 
            // lbl_nvMemCtrl_MACaddr
            // 
            this.lbl_nvMemCtrl_MACaddr.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_MACaddr.AutoSize = true;
            this.lbl_nvMemCtrl_MACaddr.Location = new System.Drawing.Point(38, 46);
            this.lbl_nvMemCtrl_MACaddr.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_MACaddr.Name = "lbl_nvMemCtrl_MACaddr";
            this.lbl_nvMemCtrl_MACaddr.Size = new System.Drawing.Size(72, 40);
            this.lbl_nvMemCtrl_MACaddr.TabIndex = 0;
            this.lbl_nvMemCtrl_MACaddr.Text = "MAC Address:";
            // 
            // cmd_nvMemCtrl_burnCountryCode
            // 
            this.cmd_nvMemCtrl_burnCountryCode.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmd_nvMemCtrl_burnCountryCode.Location = new System.Drawing.Point(418, 95);
            this.cmd_nvMemCtrl_burnCountryCode.Margin = new System.Windows.Forms.Padding(4);
            this.cmd_nvMemCtrl_burnCountryCode.Name = "cmd_nvMemCtrl_burnCountryCode";
            this.cmd_nvMemCtrl_burnCountryCode.Size = new System.Drawing.Size(282, 39);
            this.cmd_nvMemCtrl_burnCountryCode.TabIndex = 2;
            this.cmd_nvMemCtrl_burnCountryCode.Text = "Burn Country Code";
            this.cmd_nvMemCtrl_burnCountryCode.UseVisualStyleBackColor = true;
            this.cmd_nvMemCtrl_burnCountryCode.Click += new System.EventHandler(this.burnCountryCode_Click);
            // 
            // lbl_nvMemCtrl_countryCurrent
            // 
            this.lbl_nvMemCtrl_countryCurrent.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lbl_nvMemCtrl_countryCurrent.AutoSize = true;
            this.lbl_nvMemCtrl_countryCurrent.Location = new System.Drawing.Point(42, 104);
            this.lbl_nvMemCtrl_countryCurrent.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nvMemCtrl_countryCurrent.Name = "lbl_nvMemCtrl_countryCurrent";
            this.lbl_nvMemCtrl_countryCurrent.Size = new System.Drawing.Size(68, 20);
            this.lbl_nvMemCtrl_countryCurrent.TabIndex = 0;
            this.lbl_nvMemCtrl_countryCurrent.Text = "Country:";
            // 
            // combox_nvMemCtrl_country
            // 
            this.combox_nvMemCtrl_country.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.combox_nvMemCtrl_country.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(245)))), ((int)(((byte)(252)))), ((int)(((byte)(199)))));
            this.combox_nvMemCtrl_country.FormattingEnabled = true;
            this.combox_nvMemCtrl_country.Location = new System.Drawing.Point(118, 100);
            this.combox_nvMemCtrl_country.Margin = new System.Windows.Forms.Padding(4);
            this.combox_nvMemCtrl_country.Name = "combox_nvMemCtrl_country";
            this.combox_nvMemCtrl_country.Size = new System.Drawing.Size(292, 28);
            this.combox_nvMemCtrl_country.TabIndex = 3;
            // 
            // check_nvMemCtrl_SNformatBarcode
            // 
            this.check_nvMemCtrl_SNformatBarcode.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.check_nvMemCtrl_SNformatBarcode.AutoSize = true;
            this.check_nvMemCtrl_SNformatBarcode.Location = new System.Drawing.Point(418, 4);
            this.check_nvMemCtrl_SNformatBarcode.Margin = new System.Windows.Forms.Padding(4);
            this.check_nvMemCtrl_SNformatBarcode.Name = "check_nvMemCtrl_SNformatBarcode";
            this.check_nvMemCtrl_SNformatBarcode.Size = new System.Drawing.Size(282, 34);
            this.check_nvMemCtrl_SNformatBarcode.TabIndex = 4;
            this.check_nvMemCtrl_SNformatBarcode.Text = "SN only Barcode";
            this.check_nvMemCtrl_SNformatBarcode.UseVisualStyleBackColor = true;
            // 
            // tpcTab
            // 
            this.tpcTab.BackColor = System.Drawing.Color.White;
            this.tpcTab.Controls.Add(this.groupBox_calData);
            this.tpcTab.Location = new System.Drawing.Point(4, 29);
            this.tpcTab.Name = "tpcTab";
            this.tpcTab.Size = new System.Drawing.Size(1225, 1063);
            this.tpcTab.TabIndex = 2;
            this.tpcTab.Text = "EEPROM";
            // 
            // groupBox_calData
            // 
            this.groupBox_calData.BackColor = System.Drawing.Color.White;
            this.groupBox_calData.Controls.Add(this.cmd_tpc_collapseAll);
            this.groupBox_calData.Controls.Add(this.cmd_tpc_expandAll);
            this.groupBox_calData.Controls.Add(this.tree_tpc_tpc);
            this.groupBox_calData.Controls.Add(this.txtBox_tpc_eepromVersion);
            this.groupBox_calData.Controls.Add(this.lbl_tpc_eepromVersion);
            this.groupBox_calData.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox_calData.Location = new System.Drawing.Point(0, 9);
            this.groupBox_calData.Name = "groupBox_calData";
            this.groupBox_calData.Size = new System.Drawing.Size(1161, 1089);
            this.groupBox_calData.TabIndex = 0;
            this.groupBox_calData.TabStop = false;
            this.groupBox_calData.Text = "Calibration Data";
            // 
            // cmd_tpc_collapseAll
            // 
            this.cmd_tpc_collapseAll.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(210)))), ((int)(((byte)(210)))), ((int)(((byte)(210)))));
            this.cmd_tpc_collapseAll.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_tpc_collapseAll.Image = ((System.Drawing.Image)(resources.GetObject("cmd_tpc_collapseAll.Image")));
            this.cmd_tpc_collapseAll.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.cmd_tpc_collapseAll.Location = new System.Drawing.Point(938, 238);
            this.cmd_tpc_collapseAll.Name = "cmd_tpc_collapseAll";
            this.cmd_tpc_collapseAll.Size = new System.Drawing.Size(138, 162);
            this.cmd_tpc_collapseAll.TabIndex = 5;
            this.cmd_tpc_collapseAll.Text = "Collapse All";
            this.cmd_tpc_collapseAll.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.cmd_tpc_collapseAll.UseVisualStyleBackColor = false;
            this.cmd_tpc_collapseAll.Click += new System.EventHandler(this.TPC_CollapseAll_Click);
            // 
            // cmd_tpc_expandAll
            // 
            this.cmd_tpc_expandAll.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(210)))), ((int)(((byte)(210)))), ((int)(((byte)(210)))));
            this.cmd_tpc_expandAll.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.cmd_tpc_expandAll.Image = ((System.Drawing.Image)(resources.GetObject("cmd_tpc_expandAll.Image")));
            this.cmd_tpc_expandAll.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.cmd_tpc_expandAll.Location = new System.Drawing.Point(938, 70);
            this.cmd_tpc_expandAll.Name = "cmd_tpc_expandAll";
            this.cmd_tpc_expandAll.Size = new System.Drawing.Size(138, 162);
            this.cmd_tpc_expandAll.TabIndex = 5;
            this.cmd_tpc_expandAll.Text = "Expand All";
            this.cmd_tpc_expandAll.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.cmd_tpc_expandAll.UseVisualStyleBackColor = false;
            this.cmd_tpc_expandAll.Click += new System.EventHandler(this.TPC_ExpandAll_Click);
            // 
            // tree_tpc_tpc
            // 
            this.tree_tpc_tpc.BackColor = System.Drawing.Color.GhostWhite;
            this.tree_tpc_tpc.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.tree_tpc_tpc.Location = new System.Drawing.Point(22, 70);
            this.tree_tpc_tpc.Name = "tree_tpc_tpc";
            this.tree_tpc_tpc.Size = new System.Drawing.Size(907, 1010);
            this.tree_tpc_tpc.TabIndex = 4;
            // 
            // txtBox_tpc_eepromVersion
            // 
            this.txtBox_tpc_eepromVersion.BackColor = System.Drawing.Color.GhostWhite;
            this.txtBox_tpc_eepromVersion.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.txtBox_tpc_eepromVersion.Location = new System.Drawing.Point(166, 30);
            this.txtBox_tpc_eepromVersion.Margin = new System.Windows.Forms.Padding(4);
            this.txtBox_tpc_eepromVersion.Name = "txtBox_tpc_eepromVersion";
            this.txtBox_tpc_eepromVersion.ReadOnly = true;
            this.txtBox_tpc_eepromVersion.Size = new System.Drawing.Size(49, 26);
            this.txtBox_tpc_eepromVersion.TabIndex = 3;
            // 
            // lbl_tpc_eepromVersion
            // 
            this.lbl_tpc_eepromVersion.AutoSize = true;
            this.lbl_tpc_eepromVersion.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.lbl_tpc_eepromVersion.Location = new System.Drawing.Point(18, 34);
            this.lbl_tpc_eepromVersion.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_tpc_eepromVersion.Name = "lbl_tpc_eepromVersion";
            this.lbl_tpc_eepromVersion.Size = new System.Drawing.Size(148, 20);
            this.lbl_tpc_eepromVersion.TabIndex = 2;
            this.lbl_tpc_eepromVersion.Text = "EEPROM Version:";
            // 
            // groupBox1
            // 
            this.groupBox1.BackColor = System.Drawing.Color.White;
            this.groupBox1.Controls.Add(this.combox_DebugConsole);
            this.groupBox1.Controls.Add(this.combox_nvMemCtrl_printLevel);
            this.groupBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox1.Location = new System.Drawing.Point(12, 1338);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox1.Size = new System.Drawing.Size(399, 68);
            this.groupBox1.TabIndex = 22;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Debug";
            // 
            // openBeamformingMatrixHeaderFile_standard
            // 
            this.openBeamformingMatrixHeaderFile_standard.DefaultExt = "txt";
            // 
            // openBeamformingMatrixHeaderFile_160mhz
            // 
            this.openBeamformingMatrixHeaderFile_160mhz.FileName = "openFileDialog1";
            // 
            // openBeamformingMatrixValuesFile_160mhz
            // 
            this.openBeamformingMatrixValuesFile_160mhz.DefaultExt = "txt";
            // 
            // openBeamformingMatrixValuesFile_ehtExtra_160mhz
            // 
            this.openBeamformingMatrixValuesFile_ehtExtra_160mhz.DefaultExt = "txt";
            // 
            // openBeamformingMatrixHeaderFile_lower_320mhz
            // 
            this.openBeamformingMatrixHeaderFile_lower_320mhz.DefaultExt = "txt";
            // 
            // openBeamformingMatrixHeaderFile_upper_320mhz
            // 
            this.openBeamformingMatrixHeaderFile_upper_320mhz.DefaultExt = "txt";
            // 
            // openBeamformingMatrixValuesFile_lower_320mhz
            // 
            this.openBeamformingMatrixValuesFile_lower_320mhz.DefaultExt = "txt";
            // 
            // openBeamformingMatrixValuesFile_upper_320mhz
            // 
            this.openBeamformingMatrixValuesFile_upper_320mhz.DefaultExt = "txt";
            // 
            // openBeamformingMatrixValuesFile_ehtExtra_upper_320mhz
            // 
            this.openBeamformingMatrixValuesFile_ehtExtra_upper_320mhz.DefaultExt = "txt";
            // 
            // openBeamformingMatrixValuesFile_ehtExtra_lower_320mhz
            // 
            this.openBeamformingMatrixValuesFile_ehtExtra_lower_320mhz.DefaultExt = "txt";
            // 
            // radio_basicOp_codingAuto
            // 
            this.radio_basicOp_codingAuto.AutoSize = true;
            this.radio_basicOp_codingAuto.Location = new System.Drawing.Point(119, 169);
            this.radio_basicOp_codingAuto.Name = "radio_basicOp_codingAuto";
            this.radio_basicOp_codingAuto.Size = new System.Drawing.Size(68, 24);
            this.radio_basicOp_codingAuto.TabIndex = 22;
            this.radio_basicOp_codingAuto.TabStop = true;
            this.radio_basicOp_codingAuto.Text = "Auto";
            this.radio_basicOp_codingAuto.UseVisualStyleBackColor = true;
            // 
            // DUT_GUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(144F, 144F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.BackColor = System.Drawing.Color.White;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ClientSize = new System.Drawing.Size(1260, 1126);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.pictureBoxLogo);
            this.Controls.Add(this.tabControl);
            this.Controls.Add(this.groupBox_connection);
            this.Controls.Add(this.dllConsole);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MaximizeBox = false;
            this.Name = "DUT_GUI";
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "DUT GUI";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.DUT_GUI_FormClosing);
            this.Load += new System.EventHandler(this.DUT_GUI_Load);
            this.table_nvMemCtrl_versionInfo.ResumeLayout(false);
            this.table_nvMemCtrl_versionInfo.PerformLayout();
            this.table_basicOp_antennas.ResumeLayout(false);
            this.groupBox_tx.ResumeLayout(false);
            this.table_basicOp_txALL.ResumeLayout(false);
            this.table_basicOp_txALL.PerformLayout();
            this.panel_basicOp_rate_cmd.ResumeLayout(false);
            this.panel_basicOp_rate_cmd.PerformLayout();
            this.panel_basicOp_rate_params.ResumeLayout(false);
            this.panel_basicOp_rate_params.PerformLayout();
            this.panel_spacing_params.ResumeLayout(false);
            this.panel_spacing_params.PerformLayout();
            this.panel_spacing_cmd.ResumeLayout(false);
            this.panel_txParams_cmd.ResumeLayout(false);
            this.panel_powerParam_cmd.ResumeLayout(false);
            this.panel_powerParam_cmd.PerformLayout();
            this.panel_loop_cmd.ResumeLayout(false);
            this.panel_basicOp_channel_cmd.ResumeLayout(false);
            this.panel_basicOp_params.ResumeLayout(false);
            this.panel_basicOp_params.PerformLayout();
            this.panel_basicOp_phy_param.ResumeLayout(false);
            this.panel_basicOp_phy_param.PerformLayout();
            this.panel_basicOp_txParams_params.ResumeLayout(false);
            this.panel_basicOp_txParams_params.PerformLayout();
            this.panel_basicOp_channel_params.ResumeLayout(false);
            this.panel_basicOp_channel_params.PerformLayout();
            this.groupBox_burnEEPROM.ResumeLayout(false);
            this.table_nvMemCtrl_burnEEPROM.ResumeLayout(false);
            this.table_nvMemCtrl_burnEEPROM.PerformLayout();
            this.groupBox_genRisc.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLogo)).EndInit();
            this.groupBox_connection.ResumeLayout(false);
            this.groupBox_connection.PerformLayout();
            this.groupBox_WlanCard.ResumeLayout(false);
            this.tabControl.ResumeLayout(false);
            this.basicOperationTab.ResumeLayout(false);
            this.groupBox_RuParameters.ResumeLayout(false);
            this.groupBox_RuParameters.PerformLayout();
            this.groupBox_basicOp_spaceless.ResumeLayout(false);
            this.groupBox_cardID.ResumeLayout(false);
            this.groupBox_cardID.PerformLayout();
            this.groupBox_basicOp_misc.ResumeLayout(false);
            this.groupBox_basicOp_misc.PerformLayout();
            this.groupBox_antennasOnOff.ResumeLayout(false);
            this.extendedOperationTab.ResumeLayout(false);
            this.extendedOperationTab.PerformLayout();
            this.tableLayoutPanel6.ResumeLayout(false);
            this.bf_tabControl.ResumeLayout(false);
            this.bf_tab_default.ResumeLayout(false);
            this.bf_tab_default.PerformLayout();
            this.bf_tab_standard.ResumeLayout(false);
            this.groupBox_WriteBeamformingMatrix.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.bf_tab_eht160.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.tableLayoutPanel4.ResumeLayout(false);
            this.tableLayoutPanel4.PerformLayout();
            this.bf_tab_eht320.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.tableLayoutPanel5.ResumeLayout(false);
            this.tableLayoutPanel5.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.tableLayoutPanel7.ResumeLayout(false);
            this.tableLayoutPanel7.PerformLayout();
            this.groupBoxOther.ResumeLayout(false);
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            this.groupBox_rwRegisters.ResumeLayout(false);
            this.table_nvMemCtrl_rwRegisters.ResumeLayout(false);
            this.table_nvMemCtrl_rwRegisters.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.groupBox_antennaPhase.ResumeLayout(false);
            this.groupBox_antennaPhase.PerformLayout();
            this.groupBox_xtalCalib.ResumeLayout(false);
            this.groupBox_xtalCalib.PerformLayout();
            this.table_extendedOp_xtalRegister.ResumeLayout(false);
            this.table_extendedOp_xtalRegister.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.num_extendedOp_xtalRegisterValue)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.num_extendedOp_xtalRegisterBias)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.num_extendedOp_xtalCalBias)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.num_extendedOp_xtalCalValue)).EndInit();
            this.nvMemControlTab.ResumeLayout(false);
            this.groupBox_versionInfo.ResumeLayout(false);
            this.groupBox_eepromInfo.ResumeLayout(false);
            this.groupBox_nvMem_prodInfo.ResumeLayout(false);
            this.groupBox_nvMem_prodInfo.PerformLayout();
            this.groupBox_eepromBarParse.ResumeLayout(false);
            this.table_nvMemCtrl_EEPROMbarcodeParsing.ResumeLayout(false);
            this.table_nvMemCtrl_EEPROMbarcodeParsing.PerformLayout();
            this.table_nvMemCtrl_currentEEPROM.ResumeLayout(false);
            this.table_nvMemCtrl_currentEEPROM.PerformLayout();
            this.tpcTab.ResumeLayout(false);
            this.groupBox_calData.ResumeLayout(false);
            this.groupBox_calData.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RichTextBox dllConsole;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_dutVersion;
        private System.Windows.Forms.Label lbl_nvMemCtrl_dutVersion;
        private System.Windows.Forms.TableLayoutPanel table_nvMemCtrl_versionInfo;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_RFchipVersion;
        private System.Windows.Forms.Label lbl_nvMemCtrl_RFchip;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_BBchipVersion;
        private System.Windows.Forms.Label lbl_nvMemCtrl_BBchip;
        private System.Windows.Forms.TableLayoutPanel table_basicOp_antennas;
        private System.Windows.Forms.Button cmd_basicOp_txAnt0;
        private System.Windows.Forms.Button cmd_basicOp_txAnt2;
        private System.Windows.Forms.Button cmd_basicOp_rxAnt0;
        private System.Windows.Forms.Button cmd_basicOp_rxAnt1;
        private System.Windows.Forms.Button cmd_basicOp_rxAnt2;
        private System.Windows.Forms.GroupBox groupBox_tx;
        private System.Windows.Forms.Label lbl_basicOp_spectrumBW;
        private System.Windows.Forms.TableLayoutPanel table_basicOp_txALL;
        private System.Windows.Forms.Button cmd_basicOp_setChannel;
        private System.Windows.Forms.GroupBox groupBox_burnEEPROM;
        private System.Windows.Forms.TableLayoutPanel table_nvMemCtrl_burnEEPROM;
        private System.Windows.Forms.Label lbl_nvMemCtrl_barcode;
        private System.Windows.Forms.Label lbl_nvMemCtrl_calFile;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_barcode;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_eepromFile;
        private System.Windows.Forms.Button cmd_nvMemCtrl_browseEEPROM;
        private System.Windows.Forms.OpenFileDialog openEEPROMfile;
        private System.Windows.Forms.OpenFileDialog openBeamformingMatrixValuesFile_standard;
        private System.Windows.Forms.Button cmd_nvMemCtrl_burnToNVmemory;
        private System.Windows.Forms.GroupBox groupBox_genRisc;
        private System.Windows.Forms.Button cmd_basicOp_resetCounter;
        private System.Windows.Forms.Button cmd_nvMemCtrl_viewEEPROM;
        private System.Windows.Forms.Button cmd_basicOp_updateInput;
        private System.Windows.Forms.GroupBox groupBox_connection;
        private System.Windows.Forms.Button ConnectButton;
        private System.Windows.Forms.Label lbl_basicOp_signalBW;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage basicOperationTab;
        private System.Windows.Forms.TabPage nvMemControlTab;
        private System.Windows.Forms.GroupBox groupBox_versionInfo;
        private System.Windows.Forms.GroupBox groupBox_antennasOnOff;
        private System.Windows.Forms.GroupBox groupBox_eepromInfo;
        private System.Windows.Forms.TableLayoutPanel table_nvMemCtrl_currentEEPROM;
        private System.Windows.Forms.Label lbl_nvMemCtrl_MACaddr;
        private System.Windows.Forms.Label lbl_nvMemCtrl_week;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_week;
        private System.Windows.Forms.Label lbl_nvMemCtrl_serial;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_serial;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_MACaddr;
        private System.Windows.Forms.Label lbl_nvMemCtrl_year;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_year;
        private System.Windows.Forms.Button cmd_nvMemCtrl_readCalFileInfo;
        private System.Windows.Forms.TextBox ipAddress;
        private System.Windows.Forms.ListView inputsList;
        private System.Windows.Forms.ColumnHeader inputName;
        private System.Windows.Forms.ColumnHeader inputValue;
        private System.Windows.Forms.Panel panel_basicOp_channel_params;
        private System.Windows.Forms.Label lbl_basicOp_CenterFreqMHz;
        private System.Windows.Forms.ComboBox combox_basicOp_lowChan;
        private System.Windows.Forms.Button cmd_basicOp_setSpacing;
        private System.Windows.Forms.TextBox txtBox_basicOp_spacingIFS;
        private System.Windows.Forms.Button cmd_basicOp_transmitCW;
        private System.Windows.Forms.Label lbl_basicOp_amplitude;
        private System.Windows.Forms.TextBox txtBox_basicOp_cwAmplitude;
        private System.Windows.Forms.Label lbl_basicOp_binNum;
        private System.Windows.Forms.TextBox txtBox_basicOp_cwBinNum;
        private System.Windows.Forms.Label lbl_basicOp_us;
        private System.Windows.Forms.CheckBox check_basicOp_closeRXants;
        private System.Windows.Forms.Button cmd_basicOp_stopCW;
        private System.Windows.Forms.TabPage tpcTab;
        private System.Windows.Forms.GroupBox groupBox_calData;
        private System.Windows.Forms.TextBox txtBox_tpc_eepromVersion;
        private System.Windows.Forms.Label lbl_tpc_eepromVersion;
        private System.Windows.Forms.TreeView tree_tpc_tpc;
        private System.Windows.Forms.Button cmd_tpc_expandAll;
        private System.Windows.Forms.Button cmd_tpc_collapseAll;
        private System.Windows.Forms.Button burnBarcodeAndMAC;
        private System.Windows.Forms.GroupBox groupBox_eepromBarParse;
        private System.Windows.Forms.TableLayoutPanel table_nvMemCtrl_EEPROMbarcodeParsing;
        private System.Windows.Forms.GroupBox groupBox_basicOp_misc;
        private System.Windows.Forms.Label lbl_nvMemCtrl_country;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_country;
        private System.Windows.Forms.Button cmd_nvMemCtrl_burnCountryCode;
        private System.Windows.Forms.Label lbl_nvMemCtrl_countryCurrent;
        private System.Windows.Forms.ComboBox combox_nvMemCtrl_country;
        private System.Windows.Forms.GroupBox groupBox_WlanCard;
        private System.Windows.Forms.RadioButton wlan2card;
        private System.Windows.Forms.RadioButton wlan0card;
        private System.Windows.Forms.Button cmd_basicOp_txAnt1;
        private System.Windows.Forms.TabPage extendedOperationTab;
        private System.Windows.Forms.CheckBox check_nvMemCtrl_SNformatBarcode;
        private System.Windows.Forms.Button cmd_extendedOp_xtalRegisterWrite;
        private System.Windows.Forms.Button cmd_extendedOp_xtalRegisterRead;
        private System.Windows.Forms.GroupBox groupBox_xtalCalib;
        private System.Windows.Forms.CheckBox check_extendedOp_xtalEnable;
        private System.Windows.Forms.Button cmd_extendedOp_xtalCalReadEEPROM;
        private System.Windows.Forms.Button cmd_extendedOp_xtalCalWriteEEPROM;
        private System.Windows.Forms.CheckBox lbl_extendedOp_xtalRegAutoUpdate;
        private System.Windows.Forms.NumericUpDown num_extendedOp_xtalCalValue;
        private System.Windows.Forms.Label lbl_extendedOp_xtalVal;
        private System.Windows.Forms.NumericUpDown num_extendedOp_xtalRegisterValue;
        private System.Windows.Forms.ToolTip ToolTipIFS;
        private System.Windows.Forms.Label lbl_extendedOp_xtalBias;
        private System.Windows.Forms.NumericUpDown num_extendedOp_xtalCalBias;
        private System.Windows.Forms.TableLayoutPanel table_extendedOp_xtalRegister;
        private System.Windows.Forms.Label lbl_extendedOp_xtalVal_reg;
        private System.Windows.Forms.NumericUpDown num_extendedOp_xtalRegisterBias;
        private System.Windows.Forms.Label lbl_extendedOp_xtalBias_reg;
        private System.Windows.Forms.Label lbl_extendedOp_xtalReg;
        private System.Windows.Forms.GroupBox groupBox_antennaPhase;
        private System.Windows.Forms.Button cmd_extendedOp_CDDset;
        private System.Windows.Forms.Button cmd_nvMemCtrl_readExtCalFile;
        private System.Windows.Forms.Button cmd_basicOp_txAnt3;
        private System.Windows.Forms.Button cmd_basicOp_rxAnt3;
        private System.Windows.Forms.Panel panel_basicOp_rate_params;
        private System.Windows.Forms.Label lbl_basicOp_MCS;
        private System.Windows.Forms.ComboBox combox_basicOp_MCS;
        private System.Windows.Forms.ComboBox combox_basicOp_spatialStream;
        private System.Windows.Forms.Label lbl_basicOp_spatialStream;
        private System.Windows.Forms.Label lbl_basicOp_GI;
        private System.Windows.Forms.Label lbl_basicOp_phyRate;
        private System.Windows.Forms.TextBox txtBox_basicOp_phyRate;
        private System.Windows.Forms.Button cmd_basicOp_setRate;
        private System.Windows.Forms.CheckBox snifferEnable;
        private System.Windows.Forms.Label lbl_basicOp_lowChan;
        private System.Windows.Forms.Label lbl_basicOp_rateParam;
        private System.Windows.Forms.Label lbl_basicOp_chanParam;
        public System.Windows.Forms.TextBox txtBox_basicOp_CenterFreqMHz;
        private System.Windows.Forms.Label lbl_basicOp_spacing;
        private System.Windows.Forms.Panel panel_spacing_params;
        private System.Windows.Forms.Panel panel_spacing_cmd;
        private System.Windows.Forms.RadioButton wlan4card;
        private System.Windows.Forms.ListView RxEvmList;
        private System.Windows.Forms.ColumnHeader rxEvmName;
        private System.Windows.Forms.ColumnHeader rxEvmValue;
        private System.Windows.Forms.Button cmd_basicOp_updateRXevm;
        private System.Windows.Forms.PictureBox pictureBoxLogo;
        private System.Windows.Forms.Panel panel_powerParam_cmd;
        private System.Windows.Forms.Panel panel_loop_cmd;
        private System.Windows.Forms.Button cmd_basicOp_setPower;
        private System.Windows.Forms.Label lbl_basicOp_powerParam;
        private System.Windows.Forms.Panel panel_basicOp_rate_cmd;
        private System.Windows.Forms.Panel panel_basicOp_channel_cmd;
        private System.Windows.Forms.Label lbl_basicOp_band;
        private System.Windows.Forms.Panel panel_basicOp_params;
        private System.Windows.Forms.Label lbl_basicOp_band_choose;
        private System.Windows.Forms.ComboBox combox_basicOp_band;
        private System.Windows.Forms.ComboBox combox_basicOp_GI;
        private System.Windows.Forms.GroupBox groupBox_cardID;
        private System.Windows.Forms.Label lbl_basicOp_wav;
        private System.Windows.Forms.TextBox txtBox_basicOp_wav;
        private System.Windows.Forms.TextBox txtBox_basicOp_bandSupport;
        private System.Windows.Forms.Label lbl_basicOp_bandSupport;
        private System.Windows.Forms.TextBox txtBox_basicOp_bandCurrent;
        private System.Windows.Forms.Label lbl_basicOp_bandCurrent;
        private System.Windows.Forms.TextBox txtBox_basicOp_txAntennaMask;
        private System.Windows.Forms.Label lbl_basicOp_antMask;
        private System.Windows.Forms.ComboBox combox_basicOp_ltf;
        private System.Windows.Forms.Label lbl_basicOp_LTF;
        private System.Windows.Forms.ComboBox combox_basicOp_spectrumBW;
        private System.Windows.Forms.ComboBox combox_basicOp_signalBW;
        private System.Windows.Forms.Panel panel_txParams_cmd;
        private System.Windows.Forms.Button cmd_basicOp_stopTransmission;
        private System.Windows.Forms.Button cmd_basicOp_startTransmitting;
        private System.Windows.Forms.Panel panel_basicOp_txParams_params;
        private System.Windows.Forms.TextBox txtBox_basicOp_repetitions;
        private System.Windows.Forms.Label lbl_basicOp_repetitions;
        private System.Windows.Forms.Label lbl_basicOp_packetLen;
        private System.Windows.Forms.TextBox txtBox_basicOp_packetLen;
        private System.Windows.Forms.Label lbl_basicOp_txParam;
        private System.Windows.Forms.Label lbl_basicOp_phy;
        private System.Windows.Forms.Panel panel_basicOp_phy_param;
        private System.Windows.Forms.ComboBox combox_basicOp_phyType;
        private System.Windows.Forms.Label lbl_basicOp_phy_choose;
        private System.Windows.Forms.Button cmd_basicOp_stopSpaceless;
        private System.Windows.Forms.Button cmd_basicOp_startSpaceless;
        private System.Windows.Forms.GroupBox groupBox_basicOp_spaceless;
        private System.Windows.Forms.ComboBox comboBox_basicOp_memoryType;
        private System.Windows.Forms.Button btn_basicOp_deleteRegistry;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TextBox txtBox_basicOp_CalMode;
        private System.Windows.Forms.GroupBox groupBox_nvMem_prodInfo;
        private System.Windows.Forms.TextBox combox_nvMemCtrl_prodValue;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button cmd_nvMemCtrl_burnProdFlag;
        private System.Windows.Forms.ComboBox combox_nvMemCtrl_prodFlag;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox check_basicOp_txLongData;
        private System.Windows.Forms.CheckBox check_basicOp_enableRxAggregation;
        private System.Windows.Forms.ComboBox combox_nvMemCtrl_printLevel;
        private System.Windows.Forms.ComboBox combox_DebugConsole;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox txtBox_extendedOp_offset2;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox txtBox_extendedOp_offset3;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txtBox_extendedOp_offset1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button cmd_extendedOp_CDDget;
        private System.Windows.Forms.ComboBox comboBox_extendedOp_antNum;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_CVVersion;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_MoreCompVersion;
        private System.Windows.Forms.Label lbl_nvMemCtrl_CV;
        private System.Windows.Forms.ComboBox combox_MoreComp;
        private System.Windows.Forms.GroupBox groupBox_rwRegisters;
        private System.Windows.Forms.Button cmd_nvMemCtrl_writeReg;
        private System.Windows.Forms.Button cmd_nvMemCtrl_readReg;
        private System.Windows.Forms.TableLayoutPanel table_nvMemCtrl_rwRegisters;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_regModule;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_regAddress;
        private System.Windows.Forms.Label lbl_nvMemCtrl_address;
        private System.Windows.Forms.Label lbl_nvMemCtrl_val;
        private System.Windows.Forms.Label lbl_nvMemCtrl_mask;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_regValue;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_regMask;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button cmd_nvMemCtrl_allRFRW;
        private System.Windows.Forms.Button cmd_nvMemCtrl_writeRF;
        private System.Windows.Forms.Button cmd_nvMemCtrl_readRF;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label lbl_nvMemCtrl_val4;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_RFval2;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_RFaddress;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_RFval0;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_RFval1;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_RFval3;
        private System.Windows.Forms.TextBox txtBox_nvMemCtrl_RFval4;
        private System.Windows.Forms.Button cmd_basicOp_rxAnt4;
        private System.Windows.Forms.TextBox txtBox_basicOp_rxAntennaMask;
        private System.Windows.Forms.CheckBox check_basicOp_txBeamforming;
        private System.Windows.Forms.GroupBox groupBox_WriteBeamformingMatrix;
        private System.Windows.Forms.Button button_writeBeamformingMatrixFile_standard;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.ComboBox combox_basicOp_primaryChannelIndex;
        private System.Windows.Forms.GroupBox groupBoxOther;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.TextBox textBoxTemperature;
        private System.Windows.Forms.Button button_getTemperature;
        private System.Windows.Forms.Label labelTemperature;
        private System.Windows.Forms.CheckBox check_basicOp_numSymbols;
        private System.Windows.Forms.TextBox txtBox_basicOp_numSymbols;
        private System.Windows.Forms.Label lbl_beamformingMatrixValuesFile;
        private System.Windows.Forms.Label lbl_beamformingMatrixHeaderFile;
        private System.Windows.Forms.TextBox txtBox_beamformingMatrixHeaderFile_standard;
        private System.Windows.Forms.Button button_browseBeamformingMatrixHeaderFile_standard;
        private System.Windows.Forms.TextBox txtBox_beamformingMatrixValuesFile_standard;
        private System.Windows.Forms.Button button_browseBeamformingMatrixValuesFile_standard;
        private System.Windows.Forms.OpenFileDialog openBeamformingMatrixHeaderFile_standard;
        private System.Windows.Forms.ComboBox combox_basicOp_regulationType;
        private System.Windows.Forms.Label label_basicOp_regulationType;
        private System.Windows.Forms.TextBox txtBox_basicOp_powerLimit;
        private System.Windows.Forms.Label lbl_basicOp_powerLimit;
        private System.Windows.Forms.TextBox txtBox_basicOp_powerLevel;
        private System.Windows.Forms.Label lbl_basicOp_powerLevel;
        private System.Windows.Forms.CheckBox checkBox_basicOp_autoPower;
        private System.Windows.Forms.CheckBox checkBox_basicOp_closedLoop;
        private System.Windows.Forms.GroupBox groupBox_RuParameters;
        private System.Windows.Forms.Label lbl_basicOp_user2;
        private System.Windows.Forms.Label lbl_basicOp_user1;
        private System.Windows.Forms.TextBox txtBox_basicOp_user2;
        private System.Windows.Forms.TextBox txtBox_basicOp_user1;
        private System.Windows.Forms.Button cmd_basicOp_setRuParams;
        private System.Windows.Forms.TabControl bf_tabControl;
        private System.Windows.Forms.TabPage bf_tab_default;
        private System.Windows.Forms.TabPage bf_tab_standard;
        private System.Windows.Forms.TabPage bf_tab_eht160;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button button_writeBeamformingMatrixFile_160mhz;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox txtBox_beamformingMatrixValuesFile_ehtExtra_160mhz;
        private System.Windows.Forms.Button button_browseBeamformingMatrixValuesFile_ehtExtra_160mhz;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox txtBox_beamformingMatrixHeaderFile_160mhz;
        private System.Windows.Forms.Button button_browseBeamformingMatrixHeaderFile_160mhz;
        private System.Windows.Forms.TextBox txtBox_beamformingMatrixValuesFile_160mhz;
        private System.Windows.Forms.Button button_browseBeamformingMatrixValuesFile_160mhz;
        private System.Windows.Forms.TabPage bf_tab_eht320;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel6;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button button_writeBeamformingMatrixFile_320mhz;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.TextBox txtBox_beamformingMatrixValuesFile_ehtExtra_lower_320mhz;
        private System.Windows.Forms.Button button_browseBeamformingMatrixValuesFile_ehtExtra_lower_320mhz;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.TextBox txtBox_beamformingMatrixHeaderFile_lower_320mhz;
        private System.Windows.Forms.Button button_browseBeamformingMatrixHeaderFile_lower_320mhz;
        private System.Windows.Forms.TextBox txtBox_beamformingMatrixValuesFile_lower_320mhz;
        private System.Windows.Forms.Button button_browseBeamformingMatrixValuesFile_lower_320mhz;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel7;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.TextBox txtBox_beamformingMatrixValuesFile_ehtExtra_upper_320mhz;
        private System.Windows.Forms.Button button_browseBeamformingMatrixValuesFile_ehtExtra_upper_320mhz;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.TextBox txtBox_beamformingMatrixHeaderFile_upper_320mhz;
        private System.Windows.Forms.Button button_browseBeamformingMatrixHeaderFile_upper_320mhz;
        private System.Windows.Forms.TextBox txtBox_beamformingMatrixValuesFile_upper_320mhz;
        private System.Windows.Forms.Button button_browseBeamformingMatrixValuesFile_upper_320mhz;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.OpenFileDialog openBeamformingMatrixHeaderFile_160mhz;
        private System.Windows.Forms.OpenFileDialog openBeamformingMatrixValuesFile_160mhz;
        private System.Windows.Forms.OpenFileDialog openBeamformingMatrixValuesFile_ehtExtra_160mhz;
        private System.Windows.Forms.OpenFileDialog openBeamformingMatrixHeaderFile_lower_320mhz;
        private System.Windows.Forms.OpenFileDialog openBeamformingMatrixHeaderFile_upper_320mhz;
        private System.Windows.Forms.OpenFileDialog openBeamformingMatrixValuesFile_lower_320mhz;
        private System.Windows.Forms.OpenFileDialog openBeamformingMatrixValuesFile_upper_320mhz;
        private System.Windows.Forms.OpenFileDialog openBeamformingMatrixValuesFile_ehtExtra_upper_320mhz;
        private System.Windows.Forms.OpenFileDialog openBeamformingMatrixValuesFile_ehtExtra_lower_320mhz;
        private System.Windows.Forms.RadioButton radio_basicOp_codingLDPC;
        private System.Windows.Forms.RadioButton radio_basicOp_codingBCC;
        private System.Windows.Forms.Label lbl_basicOp_codingType;
        private System.Windows.Forms.RadioButton radio_basicOp_codingAuto;
    }
}

