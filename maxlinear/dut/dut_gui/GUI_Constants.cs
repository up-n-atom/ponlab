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

namespace DUT_GUI
{
    public static class GUI_Constants
    {
        // band
        // spectrumBW
        // signalBW
        // channel
        // mcs
        // spatialStream
        // gi
        // ltf
        // phyType
        // band

        public const string TXT_DEFAULT = "DEFAULT";
        public const string DEFAULT_TXPARAM_REPETITIONS = "0xFFFF";

        public const int INVALID_CHANNEL = 0;
        public const ushort MIN_FREQUENCY = 0;

        public const int MAX_CHANNEL_2400MHZ = 13;
        public const int MAX_CHANNEL_5000MHZ = 200;
        public const int MAX_CHANNEL_6000MHZ = 240;

        public const int DEFAULT_CHANNEL_2400MHZ = 1;
        public const int DEFAULT_CHANNEL_5000MHZ = 36;
        public const int DEFAULT_CHANNEL_6000MHZ = 1;
        public const int DEFAULT_CHANNEL_START = 1;
        public const int DEFAULT_CHANNEL_5G_START = 36;
        public const int DEFAULT_CHANNEL_6G_START = 1;

        public const int DEFAULT_TXPARAM_PACKETLENGTH = 1000;
        public const int MINIMUM_IFS = 10; // Minimum SIFS is 16us in 802.11a and 10us in 802.11b
        public const int DEFAULT_IFS = MINIMUM_IFS;

        public const int DEFAULT_TXPARAM_NUM_OFDM_SYMBOLS = 20;

        public const int MAX_CDDSET = 15;

        public const int GUI_COMBOX_ERROR = -1;
        public const int GUI_COMBOX_DEFAULT = 0; // return in case of uninitialized values, do not change

        // Combo Box tag definitions
        public const string TAG_COMBOX_SPECTRUMBW = "TAG_SPECTRUMBW";
        public const string TAG_COMBOX_SIGNALBW = "TAG_SIGNALBW";
        public const string TAG_COMBOX_MCS = "TAG_MCS";
        public const string TAG_COMBOX_SPATIALSTREAM = "TAG_SPATIALSTREAM";
        public const string TAG_COMBOX_GI = "TAG_GI";
        public const string TAG_COMBOX_LTF = "TAG_LTF";
        public const string TAG_COMBOX_PHYTYPE = "TAG_PHYTYPE";
        public const string TAG_COMBOX_BAND = "TAG_BAND";
        public const string TAG_COMBOX_MEMTYPE = "TAG_MEMTYPE";
        public const string TAG_COMBOX_PHASE_ALIGNMENT = "TAG_PHASE_ALIGNMENT";
        public const string TAG_COMBOX_DBGOUT = "TAG_DBGOUT";
        public const string TAG_COMBOX_PRINTLEVEL = "TAG_PRINTLEVEL";
        public const string TAG_COMBOX_COMPVER = "TAG_COMPVER";
        public const string TAG_COMBOX_REGULATION_TYPE = "TAG_REGULATION_TYPE";

        // combox_basicOp_band name definitions
        public const string TXT_BAND_5GHZ = "5 GHz";
        public const string TXT_BAND_24GHZ = "2.4 GHz";
        public const string TXT_BAND_6GHZ = "6 GHz";
        // combox_basicOp_spectrumBW and combox_basicOp_signalBW name definitions
        public const string TXT_BW_20MHz = "20 MHz";
        public const string TXT_BW_40MHz = "40 MHz";
        public const string TXT_BW_80MHz = "80 MHz";
        public const string TXT_BW_160MHz = "160 MHz";
        public const string TXT_BW_320MHz = "320 MHz";

        // combox_basicOp_MCS name definitions
        public const string TXT_MCS_BPSK12 = "BPSK 1/2";
        public const string TXT_MCS_BPSK34 = "BPSK 3/4";
        public const string TXT_MCS_QPSK12 = "QPSK 1/2";
        public const string TXT_MCS_QPSK34 = "QPSK 3/4";
        public const string TXT_MCS_16QAM12 = "16-QAM 1/2";
        public const string TXT_MCS_16QAM34 = "16-QAM 3/4";
        public const string TXT_MCS_64QAM23 = "64-QAM 2/3";
        public const string TXT_MCS_64QAM34 = "64-QAM 3/4";
        public const string TXT_MCS_64QAM56 = "64-QAM 5/6";
        public const string TXT_MCS_256QAM34 = "256-QAM 3/4";
        public const string TXT_MCS_256QAM56 = "256-QAM 5/6";
        public const string TXT_MCS_1024QAM34 = "1024-QAM 3/4";
        public const string TXT_MCS_1024QAM56 = "1024-QAM 5/6";
        public const string TXT_MCS_4096QAM34 = "4096-QAM 3/4";
        public const string TXT_MCS_4096QAM56 = "4096-QAM 5/6";

        // combox_basicOp_MCS name definitions for 80211.b phy setting
        //public const string TXT_MCSB_1MBPS_LONGX = "1Mbps Long(!) Preamble";
        public const string TXT_MCSB_2MBPS_SHORT = "2Mbps Short Preamble";
        public const string TXT_MCSB_5MBPS_SHORT = "5.5Mbps Short Preamble";
        public const string TXT_MCSB_11MBPS_SHORT = "11Mbps Short Preamble";
        public const string TXT_MCSB_1MBPS_LONG = "1Mbps Long Preamble";
        public const string TXT_MCSB_2MBPS_LONG = "2Mbps Long Preamble";
        public const string TXT_MCSB_5MBPS_LONG = "5.5Mbps Long Preamble";
        public const string TXT_MCSB_11MBPS_LONG = "11Mbps Long Preamble";

        // combox_basicOp_spatialStream name definitions
        public const string TXT_SPATIALSTREAM_1 = "1";
        public const string TXT_SPATIALSTREAM_2 = "2";
        public const string TXT_SPATIALSTREAM_3 = "3";
        public const string TXT_SPATIALSTREAM_4 = "4";

        // combox_basicOp_GI name definition
        public const string TXT_GI_04 = "0.4";
        public const string TXT_GI_08 = "0.8";
        public const string TXT_GI_16 = "1.6";
        public const string TXT_GI_32 = "3.2";

        // combox_basicOp_ltf name definitions
        public const string TXT_LTF_X1 = "x1";
        public const string TXT_LTF_X2 = "x2";
        public const string TXT_LTF_X4 = "x4";

        // combox_basicOp_phyType name definition
        public const string TXT_PHY_80211A = "802.11a";
        public const string TXT_PHY_80211B = "802.11b";
        public const string TXT_PHY_80211G = "802.11g";
        public const string TXT_PHY_80211N_5GHZ = "802.11n 5GHz";
        public const string TXT_PHY_80211N_24GHZ = "802.11n 2.4GHz";
        public const string TXT_PHY_80211AC = "802.11ac";
        public const string TXT_PHY_80211AX = "802.11ax";
        public const string TXT_PHY_80211BE = "802.11be";

        // combox_config_memType name definition
        public const string TXT_MEMTYPE_FLASH = "flash";
        public const string TXT_MEMTYPE_EEPROM_1K = "eeprom_1k";
        public const string TXT_MEMTYPE_EEPROM_2K = "eeprom_2k";
        public const string TXT_MEMTYPE_EEPROM_3K = "eeprom_3k";

        // combox_extendedOp_phaseAlignment name definition
        public const string TXT_PHASEALIGN_ANTENNA1 = "Antenna 1";
        public const string TXT_PHASEALIGN_ANTENNA2 = "Antenna 2";

        // combox_nvMemCtrl_printLevel name definition
        public const string TXT_PRINTLEVEL_NONE = "None";
        public const string TXT_PRINTLEVEL_ERRORS = "Errors";
        public const string TXT_PRINTLEVEL_WARNINGS = "Warnings";
        public const string TXT_PRINTLEVEL_INFO = "Info";
        public const string TXT_PRINTLEVEL_DEBUG = "Debug";
        public const string TXT_PRINTLEVEL_TRACE = "Trace";

        // combox_DebugConsole name definition
        public const string TXT_DBGOUT_TEXTBOX = "TextBox";
        public const string TXT_DBGOUT_CONSOLE = "Console";
        public const string TXT_DBGOUT_FILE = "File";
        public const string TXT_DBGOUT_ALL = "All";

        // combox_MoreComp name definition
        //public const string TXT_COMP_DRIVER = "Driver";
        public const string TXT_COMP_PSD = "PSD";
        public const string TXT_COMP_REGULATORY = "regulatory";
        public const string TXT_COMP_500B_PROGMODEL = "500B_progmodel";
        public const string TXT_COMP_600_PROGMODEL = "600_progmodel";
        public const string TXT_COMP_600B_PROGMODEL = "600B_progmodel";
        public const string TXT_COMP_600D2_PROGMODEL = "600D2_progmodel";
        public const string TXT_COMP_700_PROGMODEL = "700_progmodel";
        public const string TXT_COMP_700B_PROGMODEL = "700B_progmodel";

        // combox_basicOp_regulationType name definition
        public const string TXT_REGULATION_TYPE_UNKNOWN = "Unknown";
        public const string TXT_REGULATION_TYPE_FCC_SP = "FCC SP";
        public const string TXT_REGULATION_TYPE_FCC_LPI = "FCC LPI";
        public const string TXT_REGULATION_TYPE_DOC = "DOC";
        public const string TXT_REGULATION_TYPE_ETSI = "ETSI";
        public const string TXT_REGULATION_TYPE_SPAIN = "Spain";
        public const string TXT_REGULATION_TYPE_FRANCE = "France";
        public const string TXT_REGULATION_TYPE_UAE = "UAE";
        public const string TXT_REGULATION_TYPE_GERMANY = "Germany";
        public const string TXT_REGULATION_TYPE_MKK = "MKK";
        public const string TXT_REGULATION_TYPE_ISRAEL = "Israel";
        public const string TXT_REGULATION_TYPE_SINGAPORE = "Singapore";
        public const string TXT_REGULATION_TYPE_AUSTRALIA = "Australia";
        public const string TXT_REGULATION_TYPE_BRAZIL = "Brazil";
        public const string TXT_REGULATION_TYPE_CHINA = "China";
        public const string TXT_REGULATION_TYPE_KOREA = "Korea";
        public const string TXT_REGULATION_TYPE_APAC = "APAC";
        public const string TXT_REGULATION_TYPE_JAPAN = "Japan";
    }
}
