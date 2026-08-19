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

using dut_cli;
using System.Collections.Generic;
using System.Windows.Forms;

namespace DUT_GUI
{
    class GUI_BoxItems
    {
        private string boxItemName;
        private int boxItemValue;

        public enum idx_mcs_e // index value definition in combox_basicOp_mcs
        {
            idx_mcs_bpsk12 = 0,
            idx_mcs_bpsk34 = 1,
            idx_mcs_qpsk12 = 2,
            idx_mcs_qpsk34 = 3,
            idx_mcs_16qam12 = 4,
            idx_mcs_16qam34 = 5,
            idx_mcs_64qam23 = 6,
            idx_mcs_64qam34 = 7,
            idx_mcs_64qam56 = 8,
            idx_mcs_256qam34 = 9,
            idx_mcs_256qam56 = 10,
            idx_mcs_1024qam34 = 11,
            idx_mcs_1024qam56 = 12,
            idx_mcs_4096qam34 = 13,
            idx_mcs_4096qam56 = 14,

            idx_mcs_80211b_baseline = 30,
            idx_mcs_2mbps_short = 31,
            idx_mcs_5mbps_short = 32,
            idx_mcs_11mbps_short = 33,
            idx_mcs_1mbps_long = 34,
            idx_mcs_2mbps_long = 35,
            idx_mcs_5mbps_long = 36,
            idx_mcs_11mbps_long = 37
        }

        public enum idx_spatialStream_e // index value definition in combox_basicOp_spatialStream
        {
            idx_spatialStream1 = 0,
            idx_spatialStream2 = 1,
            idx_spatialStream3 = 2,
            idx_spatialStream4 = 3
        }

        public enum idx_gi_e // index value definition in combox_basicOp_gi
        {
            idx_gi08 = Gi.GI_0_8_US,
            idx_gi04 = Gi.GI_0_4_US,
            idx_gi16 = Gi.GI_1_6_US,
            idx_gi32 = Gi.GI_3_2_US,
        }

        public enum idx_ltf_e // index value definition combox_basicOp_ltf
        {
            idx_ltfx1 = 0,
            idx_ltfx2 = 1,
            idx_ltfx4 = 2
        }

        public enum idx_memType_e
        {
            idx_flash = 0,
            idx_eeprom_1k = 2,
            idx_eeprom_2k = 3,
            idx_eeprom_3k = 4,
        }

        public enum idx_phaseAlignment_e
        {
            idx_antenna1 = 1,
            idx_antenna2 = 2
        }

        public enum idx_dbgout_e
        {
            idx_textbox = 0,
            idx_console = 1,
            idx_file = 2,
            idx_all = 3
        }
        public string Name
        {
            get { return boxItemName; }
            set { boxItemName = value; }
        }

        public int Value
        {
            get { return boxItemValue; }
            set { boxItemValue = value; }
        }

        public GUI_BoxItems() { }
        public GUI_BoxItems(string name, int value) { Name = name; Value = value; }

        public override string ToString() { return boxItemName; }

        /// <summary>
        /// Loads ComboBox values for selected ComboBox.
        /// </summary>

        public static void GUI_fillComboBox(ComboBox fillBox, int argInt = 0)
        {
            string fillBox_Tag = fillBox.Tag.ToString();

            switch (fillBox_Tag) // determine which Combo Box to fill
            {
                case GUI_Constants.TAG_COMBOX_SPECTRUMBW:
                    {
                        GUI_BoxItems item_spectrumBW20 = new GUI_BoxItems();
                        item_spectrumBW20.Name = GUI_Constants.TXT_BW_20MHz;
                        item_spectrumBW20.Value = (int)Bandwidth.BANDWIDTH_TWENTY;
                        fillBox.Items.Add(item_spectrumBW20);

                        GUI_BoxItems item_spectrumBW40 = new GUI_BoxItems();
                        item_spectrumBW40.Name = GUI_Constants.TXT_BW_40MHz;
                        item_spectrumBW40.Value = (int)Bandwidth.BANDWIDTH_FOURTY;
                        fillBox.Items.Add(item_spectrumBW40);

                        GUI_BoxItems item_spectrumBW80 = new GUI_BoxItems();
                        item_spectrumBW80.Name = GUI_Constants.TXT_BW_80MHz;
                        item_spectrumBW80.Value = (int)Bandwidth.BANDWIDTH_EIGHTY;
                        fillBox.Items.Add(item_spectrumBW80);

                        GUI_BoxItems item_spectrumBW160 = new GUI_BoxItems();
                        item_spectrumBW160.Name = GUI_Constants.TXT_BW_160MHz;
                        item_spectrumBW160.Value = (int)Bandwidth.BANDWIDTH_ONE_HUNDRED_SIXTY;
                        fillBox.Items.Add(item_spectrumBW160);

                        GUI_BoxItems item_spectrumBW320 = new GUI_BoxItems();
                        item_spectrumBW320.Name = GUI_Constants.TXT_BW_320MHz;
                        item_spectrumBW320.Value = (int)Bandwidth.BANDWIDTH_THREE_HUNDRED_TWENTY;
                        fillBox.Items.Add(item_spectrumBW320);
                        break;
                    }
                case GUI_Constants.TAG_COMBOX_SIGNALBW:
                    {
                        GUI_BoxItems item_signalBW20 = new GUI_BoxItems();
                        item_signalBW20.Name = GUI_Constants.TXT_BW_20MHz;
                        item_signalBW20.Value = (int)Bandwidth.BANDWIDTH_TWENTY;
                        fillBox.Items.Add(item_signalBW20);

                        GUI_BoxItems item_signalBW40 = new GUI_BoxItems();
                        item_signalBW40.Name = GUI_Constants.TXT_BW_40MHz;
                        item_signalBW40.Value = (int)Bandwidth.BANDWIDTH_FOURTY;
                        fillBox.Items.Add(item_signalBW40);

                        GUI_BoxItems item_signalBW80 = new GUI_BoxItems();
                        item_signalBW80.Name = GUI_Constants.TXT_BW_80MHz;
                        item_signalBW80.Value = (int)Bandwidth.BANDWIDTH_EIGHTY;
                        fillBox.Items.Add(item_signalBW80);

                        GUI_BoxItems item_signalBW160 = new GUI_BoxItems();
                        item_signalBW160.Name = GUI_Constants.TXT_BW_160MHz;
                        item_signalBW160.Value = (int)Bandwidth.BANDWIDTH_ONE_HUNDRED_SIXTY;
                        fillBox.Items.Add(item_signalBW160);

                        GUI_BoxItems item_spectrumBW320 = new GUI_BoxItems();
                        item_spectrumBW320.Name = GUI_Constants.TXT_BW_320MHz;
                        item_spectrumBW320.Value = (int)Bandwidth.BANDWIDTH_THREE_HUNDRED_TWENTY;
                        fillBox.Items.Add(item_spectrumBW320);
                        break;
                    }
                case GUI_Constants.TAG_COMBOX_MCS:
                    {
                        GUI_BoxItems item_mcs_bpsk12 = new GUI_BoxItems();
                        item_mcs_bpsk12.Name = GUI_Constants.TXT_MCS_BPSK12;
                        item_mcs_bpsk12.Value = (int)idx_mcs_e.idx_mcs_bpsk12;
                        fillBox.Items.Add(item_mcs_bpsk12);

                        GUI_BoxItems item_mcs_bpsk34 = new GUI_BoxItems();
                        item_mcs_bpsk34.Name = GUI_Constants.TXT_MCS_BPSK34;
                        item_mcs_bpsk34.Value = (int)idx_mcs_e.idx_mcs_bpsk34;
                        fillBox.Items.Add(item_mcs_bpsk34);

                        GUI_BoxItems item_mcs_qpsk12 = new GUI_BoxItems();
                        item_mcs_qpsk12.Name = GUI_Constants.TXT_MCS_QPSK12;
                        item_mcs_qpsk12.Value = (int)idx_mcs_e.idx_mcs_qpsk12;
                        fillBox.Items.Add(item_mcs_qpsk12);

                        GUI_BoxItems item_mcs_qpsk34 = new GUI_BoxItems();
                        item_mcs_qpsk34.Name = GUI_Constants.TXT_MCS_QPSK34;
                        item_mcs_qpsk34.Value = (int)idx_mcs_e.idx_mcs_qpsk34;
                        fillBox.Items.Add(item_mcs_qpsk34);

                        GUI_BoxItems item_mcs_16qam12 = new GUI_BoxItems();
                        item_mcs_16qam12.Name = GUI_Constants.TXT_MCS_16QAM12;
                        item_mcs_16qam12.Value = (int)idx_mcs_e.idx_mcs_16qam12;
                        fillBox.Items.Add(item_mcs_16qam12);

                        GUI_BoxItems item_mcs_16qam34 = new GUI_BoxItems();
                        item_mcs_16qam34.Name = GUI_Constants.TXT_MCS_16QAM34;
                        item_mcs_16qam34.Value = (int)idx_mcs_e.idx_mcs_16qam34;
                        fillBox.Items.Add(item_mcs_16qam34);

                        GUI_BoxItems item_mcs_64qam23 = new GUI_BoxItems();
                        item_mcs_64qam23.Name = GUI_Constants.TXT_MCS_64QAM23;
                        item_mcs_64qam23.Value = (int)idx_mcs_e.idx_mcs_64qam23;
                        fillBox.Items.Add(item_mcs_64qam23);

                        GUI_BoxItems item_mcs_64qam34 = new GUI_BoxItems();
                        item_mcs_64qam34.Name = GUI_Constants.TXT_MCS_64QAM34;
                        item_mcs_64qam34.Value = (int)idx_mcs_e.idx_mcs_64qam34;
                        fillBox.Items.Add(item_mcs_64qam34);

                        GUI_BoxItems item_mcs_64qam56 = new GUI_BoxItems();
                        item_mcs_64qam56.Name = GUI_Constants.TXT_MCS_64QAM56;
                        item_mcs_64qam56.Value = (int)idx_mcs_e.idx_mcs_64qam56;
                        fillBox.Items.Add(item_mcs_64qam56);

                        GUI_BoxItems item_mcs_256qam34 = new GUI_BoxItems();
                        item_mcs_256qam34.Name = GUI_Constants.TXT_MCS_256QAM34;
                        item_mcs_256qam34.Value = (int)idx_mcs_e.idx_mcs_256qam34;
                        fillBox.Items.Add(item_mcs_256qam34);

                        GUI_BoxItems item_mcs_256qam56 = new GUI_BoxItems();
                        item_mcs_256qam56.Name = GUI_Constants.TXT_MCS_256QAM56;
                        item_mcs_256qam56.Value = (int)idx_mcs_e.idx_mcs_256qam56;
                        fillBox.Items.Add(item_mcs_256qam56);

                        GUI_BoxItems item_mcs_1024qam34 = new GUI_BoxItems();
                        item_mcs_1024qam34.Name = GUI_Constants.TXT_MCS_1024QAM34;
                        item_mcs_1024qam34.Value = (int)idx_mcs_e.idx_mcs_1024qam34;
                        fillBox.Items.Add(item_mcs_1024qam34);

                        GUI_BoxItems item_mcs_1024qam56 = new GUI_BoxItems();
                        item_mcs_1024qam56.Name = GUI_Constants.TXT_MCS_1024QAM56;
                        item_mcs_1024qam56.Value = (int)idx_mcs_e.idx_mcs_1024qam56;
                        fillBox.Items.Add(item_mcs_1024qam56);

                        GUI_BoxItems item_mcs_4096qam34 = new GUI_BoxItems();
                        item_mcs_4096qam34.Name = GUI_Constants.TXT_MCS_4096QAM34;
                        item_mcs_4096qam34.Value = (int)idx_mcs_e.idx_mcs_4096qam34;
                        fillBox.Items.Add(item_mcs_4096qam34);

                        GUI_BoxItems item_mcs_4096qam56 = new GUI_BoxItems();
                        item_mcs_4096qam56.Name = GUI_Constants.TXT_MCS_4096QAM56;
                        item_mcs_4096qam56.Value = (int)idx_mcs_e.idx_mcs_4096qam56;
                        fillBox.Items.Add(item_mcs_4096qam56);

                        // combox_basicOp_MCS name definitions for 80211.b phy setting
                        GUI_BoxItems item_mcs_2mbps_short = new GUI_BoxItems();
                        item_mcs_2mbps_short.Name = GUI_Constants.TXT_MCSB_2MBPS_SHORT;
                        item_mcs_2mbps_short.Value = (int)idx_mcs_e.idx_mcs_2mbps_short;
                        fillBox.Items.Add(item_mcs_2mbps_short);


                        GUI_BoxItems item_mcs_5mbps_short = new GUI_BoxItems();
                        item_mcs_5mbps_short.Name = GUI_Constants.TXT_MCSB_5MBPS_SHORT;
                        item_mcs_5mbps_short.Value = (int)idx_mcs_e.idx_mcs_5mbps_short;
                        fillBox.Items.Add(item_mcs_5mbps_short);

                        GUI_BoxItems item_mcs_11mbps_short = new GUI_BoxItems();
                        item_mcs_11mbps_short.Name = GUI_Constants.TXT_MCSB_11MBPS_SHORT;
                        item_mcs_11mbps_short.Value = (int)idx_mcs_e.idx_mcs_11mbps_short;
                        fillBox.Items.Add(item_mcs_11mbps_short);

                        GUI_BoxItems item_mcs_1mbps_long = new GUI_BoxItems();
                        item_mcs_1mbps_long.Name = GUI_Constants.TXT_MCSB_1MBPS_LONG;
                        item_mcs_1mbps_long.Value = (int)idx_mcs_e.idx_mcs_1mbps_long;
                        fillBox.Items.Add(item_mcs_1mbps_long);

                        GUI_BoxItems item_mcs_2mbps_long = new GUI_BoxItems();
                        item_mcs_2mbps_long.Name = GUI_Constants.TXT_MCSB_2MBPS_LONG;
                        item_mcs_2mbps_long.Value = (int)idx_mcs_e.idx_mcs_2mbps_long;
                        fillBox.Items.Add(item_mcs_2mbps_long);

                        GUI_BoxItems item_mcs_5mbps_long = new GUI_BoxItems();
                        item_mcs_5mbps_long.Name = GUI_Constants.TXT_MCSB_5MBPS_LONG;
                        item_mcs_5mbps_long.Value = (int)idx_mcs_e.idx_mcs_5mbps_long;
                        fillBox.Items.Add(item_mcs_5mbps_long);

                        GUI_BoxItems item_mcs_11mbps_long = new GUI_BoxItems();
                        item_mcs_11mbps_long.Name = GUI_Constants.TXT_MCSB_11MBPS_LONG;
                        item_mcs_11mbps_long.Value = (int)idx_mcs_e.idx_mcs_11mbps_long;
                        fillBox.Items.Add(item_mcs_11mbps_long);

                        break;
                    }
                case GUI_Constants.TAG_COMBOX_SPATIALSTREAM:
                    {
                        var spatialStreamList = new List<GUI_BoxItems>()
                        {
                        new GUI_BoxItems() { Name = GUI_Constants.TXT_SPATIALSTREAM_1, Value = (int)idx_spatialStream_e.idx_spatialStream1 },
                        new GUI_BoxItems() { Name = GUI_Constants.TXT_SPATIALSTREAM_2, Value = (int)idx_spatialStream_e.idx_spatialStream2 },
                        new GUI_BoxItems() { Name = GUI_Constants.TXT_SPATIALSTREAM_3, Value = (int)idx_spatialStream_e.idx_spatialStream3 },
                        new GUI_BoxItems() { Name = GUI_Constants.TXT_SPATIALSTREAM_4, Value = (int)idx_spatialStream_e.idx_spatialStream4 }
                        };

                        int ant_mask = argInt;
                        int num_of_ants = 0;
                        int curr_mask = 0x0001;
                        for (int i = 0; i < 4; i++)
                        {
                            if ((ant_mask & curr_mask) != 0)
                            {
                                num_of_ants++;
                            }
                            curr_mask <<= 1;
                        }

                        if (num_of_ants != 0)
                        {
                            for (int i = 0; i < num_of_ants; ++i) fillBox.Items.Add(spatialStreamList[i]);
                        }
                        else
                        {
                            fillBox.Items.Add(spatialStreamList[0]);
                        }

                        break;
                    }
                case GUI_Constants.TAG_COMBOX_GI:
                    {
                        GUI_BoxItems item_gi08 = new GUI_BoxItems();
                        item_gi08.Name = GUI_Constants.TXT_GI_08;
                        item_gi08.Value = (int)idx_gi_e.idx_gi08;
                        fillBox.Items.Add(item_gi08);

                        GUI_BoxItems item_gi04 = new GUI_BoxItems();
                        item_gi04.Name = GUI_Constants.TXT_GI_04;
                        item_gi04.Value = (int)idx_gi_e.idx_gi04;
                        fillBox.Items.Add(item_gi04);

                        GUI_BoxItems item_gi16 = new GUI_BoxItems();
                        item_gi16.Name = GUI_Constants.TXT_GI_16;
                        item_gi16.Value = (int)idx_gi_e.idx_gi16;
                        fillBox.Items.Add(item_gi16);

                        GUI_BoxItems item_gi32 = new GUI_BoxItems();
                        item_gi32.Name = GUI_Constants.TXT_GI_32;
                        item_gi32.Value = (int)idx_gi_e.idx_gi32;
                        fillBox.Items.Add(item_gi32);
                        break;
                    }
                case GUI_Constants.TAG_COMBOX_LTF:
                    {
                        GUI_BoxItems item_ltfx1 = new GUI_BoxItems();
                        item_ltfx1.Name = GUI_Constants.TXT_LTF_X1;
                        item_ltfx1.Value = (int)idx_ltf_e.idx_ltfx1;
                        fillBox.Items.Add(item_ltfx1);

                        GUI_BoxItems item_ltfx2 = new GUI_BoxItems();
                        item_ltfx2.Name = GUI_Constants.TXT_LTF_X2;
                        item_ltfx2.Value = (int)idx_ltf_e.idx_ltfx2;
                        fillBox.Items.Add(item_ltfx2);

                        GUI_BoxItems item_ltfx4 = new GUI_BoxItems();
                        item_ltfx4.Name = GUI_Constants.TXT_LTF_X4;
                        item_ltfx4.Value = (int)idx_ltf_e.idx_ltfx4;
                        fillBox.Items.Add(item_ltfx4);
                        break;
                    }
                case GUI_Constants.TAG_COMBOX_BAND:
                    {
                        // load Band values
                        GUI_BoxItems item_band_5G = new GUI_BoxItems();
                        item_band_5G.Name = GUI_Constants.TXT_BAND_5GHZ;
                        item_band_5G.Value = (int)Band.BAND_5000MHZ;
                        fillBox.Items.Add(item_band_5G);

                        GUI_BoxItems item_band_24G = new GUI_BoxItems();
                        item_band_24G.Name = GUI_Constants.TXT_BAND_24GHZ;
                        item_band_24G.Value = (int)Band.BAND_2400MHZ;
                        fillBox.Items.Add(item_band_24G);

                        GUI_BoxItems item_band_6G = new GUI_BoxItems();
                        item_band_6G.Name = GUI_Constants.TXT_BAND_6GHZ;
                        item_band_6G.Value = (int)Band.BAND_6000MHZ;
                        fillBox.Items.Add(item_band_6G);
                        break;
                    }
                case GUI_Constants.TAG_COMBOX_PHYTYPE:
                    {
                        GUI_BoxItems item_phy_80211a = new GUI_BoxItems();
                        item_phy_80211a.Name = GUI_Constants.TXT_PHY_80211A;
                        item_phy_80211a.Value = (int)PhyMode.PHY_MODE_A;
                        fillBox.Items.Add(item_phy_80211a);

                        GUI_BoxItems item_phy_80211b = new GUI_BoxItems();
                        item_phy_80211b.Name = GUI_Constants.TXT_PHY_80211B;
                        item_phy_80211b.Value = (int)PhyMode.PHY_MODE_B;
                        fillBox.Items.Add(item_phy_80211b);

                        GUI_BoxItems item_phy_80211g = new GUI_BoxItems();
                        item_phy_80211g.Name = GUI_Constants.TXT_PHY_80211G;
                        item_phy_80211g.Value = (int)PhyMode.PHY_MODE_G;
                        fillBox.Items.Add(item_phy_80211g);

                        GUI_BoxItems item_phy_80211n_5GHz = new GUI_BoxItems();
                        item_phy_80211n_5GHz.Name = GUI_Constants.TXT_PHY_80211N_5GHZ;
                        item_phy_80211n_5GHz.Value = (int)PhyMode.PHY_MODE_N_5;
                        fillBox.Items.Add(item_phy_80211n_5GHz);

                        GUI_BoxItems item_phy_80211n_24GHz = new GUI_BoxItems();
                        item_phy_80211n_24GHz.Name = GUI_Constants.TXT_PHY_80211N_24GHZ;
                        item_phy_80211n_24GHz.Value = (int)PhyMode.PHY_MODE_N_2_4;
                        fillBox.Items.Add(item_phy_80211n_24GHz);

                        GUI_BoxItems item_phy_80211ac = new GUI_BoxItems();
                        item_phy_80211ac.Name = GUI_Constants.TXT_PHY_80211AC;
                        item_phy_80211ac.Value = (int)PhyMode.PHY_MODE_AC;
                        fillBox.Items.Add(item_phy_80211ac);

                        GUI_BoxItems item_phy_80211ax = new GUI_BoxItems();
                        item_phy_80211ax.Name = GUI_Constants.TXT_PHY_80211AX;
                        item_phy_80211ax.Value = (int)PhyMode.PHY_MODE_AX;
                        fillBox.Items.Add(item_phy_80211ax);

                        GUI_BoxItems item_phy_80211be = new GUI_BoxItems();
                        item_phy_80211be.Name = GUI_Constants.TXT_PHY_80211BE;
                        item_phy_80211be.Value = (int)PhyMode.PHY_MODE_BE;
                        fillBox.Items.Add(item_phy_80211be);
                        break;
                    }
                case GUI_Constants.TAG_COMBOX_MEMTYPE:
                    {
                        GUI_BoxItems item_memType_flash = new GUI_BoxItems();
                        item_memType_flash.Name = GUI_Constants.TXT_MEMTYPE_FLASH;
                        item_memType_flash.Value = (int)idx_memType_e.idx_flash;
                        fillBox.Items.Add(item_memType_flash);

                        GUI_BoxItems item_memType_eeprom_1k = new GUI_BoxItems();
                        item_memType_eeprom_1k.Name = GUI_Constants.TXT_MEMTYPE_EEPROM_1K;
                        item_memType_eeprom_1k.Value = (int)idx_memType_e.idx_eeprom_1k;
                        fillBox.Items.Add(item_memType_eeprom_1k);

                        GUI_BoxItems item_memType_eeprom_2k = new GUI_BoxItems();
                        item_memType_eeprom_2k.Name = GUI_Constants.TXT_MEMTYPE_EEPROM_2K;
                        item_memType_eeprom_2k.Value = (int)idx_memType_e.idx_eeprom_2k;
                        fillBox.Items.Add(item_memType_eeprom_2k);

                        GUI_BoxItems item_memType_eeprom_3k = new GUI_BoxItems();
                        item_memType_eeprom_3k.Name = GUI_Constants.TXT_MEMTYPE_EEPROM_3K;
                        item_memType_eeprom_3k.Value = (int)idx_memType_e.idx_eeprom_3k;
                        fillBox.Items.Add(item_memType_eeprom_3k);

                        break;
                    }
                case GUI_Constants.TAG_COMBOX_PHASE_ALIGNMENT:
                    {
                        GUI_BoxItems item_phaseAlignment_a1 = new GUI_BoxItems();
                        item_phaseAlignment_a1.Name = GUI_Constants.TXT_PHASEALIGN_ANTENNA1;
                        item_phaseAlignment_a1.Value = (int)idx_phaseAlignment_e.idx_antenna1;
                        fillBox.Items.Add(item_phaseAlignment_a1);

                        GUI_BoxItems item_phaseAlignment_a2 = new GUI_BoxItems();
                        item_phaseAlignment_a2.Name = GUI_Constants.TXT_PHASEALIGN_ANTENNA2;
                        item_phaseAlignment_a2.Value = (int)idx_phaseAlignment_e.idx_antenna2;
                        fillBox.Items.Add(item_phaseAlignment_a2);
                        break;
                    }
                case GUI_Constants.TAG_COMBOX_PRINTLEVEL:
                    {
                        GUI_BoxItems item_printlevel_none = new GUI_BoxItems();
                        item_printlevel_none.Name = GUI_Constants.TXT_PRINTLEVEL_NONE;
                        item_printlevel_none.Value = (int)LogLevel.LOG_LEVEL_NONE;
                        fillBox.Items.Add(item_printlevel_none);

                        GUI_BoxItems item_printlevel_errors = new GUI_BoxItems();
                        item_printlevel_errors.Name = GUI_Constants.TXT_PRINTLEVEL_ERRORS;
                        item_printlevel_errors.Value = (int)LogLevel.LOG_LEVEL_ERROR;
                        fillBox.Items.Add(item_printlevel_errors);

                        GUI_BoxItems item_printlevel_warnings = new GUI_BoxItems();
                        item_printlevel_warnings.Name = GUI_Constants.TXT_PRINTLEVEL_WARNINGS;
                        item_printlevel_warnings.Value = (int)LogLevel.LOG_LEVEL_WARNING;
                        fillBox.Items.Add(item_printlevel_warnings);

                        GUI_BoxItems item_printlevel_info = new GUI_BoxItems();
                        item_printlevel_info.Name = GUI_Constants.TXT_PRINTLEVEL_INFO;
                        item_printlevel_info.Value = (int)LogLevel.LOG_LEVEL_INFO;
                        fillBox.Items.Add(item_printlevel_info);

                        GUI_BoxItems item_printlevel_debug = new GUI_BoxItems();
                        item_printlevel_debug.Name = GUI_Constants.TXT_PRINTLEVEL_DEBUG;
                        item_printlevel_debug.Value = (int)LogLevel.LOG_LEVEL_DEBUG;
                        fillBox.Items.Add(item_printlevel_debug);

                        GUI_BoxItems item_printlevel_api = new GUI_BoxItems();
                        item_printlevel_api.Name = GUI_Constants.TXT_PRINTLEVEL_TRACE;
                        item_printlevel_api.Value = (int)LogLevel.LOG_LEVEL_TRACE;
                        fillBox.Items.Add(item_printlevel_api);
                        break;
                    }
                case GUI_Constants.TAG_COMBOX_DBGOUT:
                    {
                        GUI_BoxItems item_dbgout_textbox = new GUI_BoxItems();
                        item_dbgout_textbox.Name = GUI_Constants.TXT_DBGOUT_TEXTBOX;
                        item_dbgout_textbox.Value = (int)idx_dbgout_e.idx_textbox;
                        fillBox.Items.Add(item_dbgout_textbox);

                        GUI_BoxItems item_dbgout_console = new GUI_BoxItems();
                        item_dbgout_console.Name = GUI_Constants.TXT_DBGOUT_CONSOLE;
                        item_dbgout_console.Value = (int)idx_dbgout_e.idx_console;
                        fillBox.Items.Add(item_dbgout_console);

                        GUI_BoxItems item_dbgout_file = new GUI_BoxItems();
                        item_dbgout_file.Name = GUI_Constants.TXT_DBGOUT_FILE;
                        item_dbgout_file.Value = (int)idx_dbgout_e.idx_file;
                        fillBox.Items.Add(item_dbgout_file);

                        GUI_BoxItems item_dbgout_all = new GUI_BoxItems();
                        item_dbgout_all.Name = GUI_Constants.TXT_DBGOUT_ALL;
                        item_dbgout_all.Value = (int)idx_dbgout_e.idx_all;
                        fillBox.Items.Add(item_dbgout_all);
                        break;
                    }
                case GUI_Constants.TAG_COMBOX_COMPVER:
                    {
                        GUI_BoxItems item_component_PSD = new GUI_BoxItems();
                        item_component_PSD.Name = GUI_Constants.TXT_COMP_PSD;
                        item_component_PSD.Value = (int)VersionedComponent.VERSIONED_COMPONENT_PSD;
                        fillBox.Items.Add(item_component_PSD);

                        GUI_BoxItems item_component_regulatory = new GUI_BoxItems();
                        item_component_regulatory.Name = GUI_Constants.TXT_COMP_REGULATORY;
                        item_component_regulatory.Value = (int)VersionedComponent.VERSIONED_COMPONENT_REGULATORY;
                        fillBox.Items.Add(item_component_regulatory);

                        GUI_BoxItems item_component_500b_progmodel = new GUI_BoxItems();
                        item_component_500b_progmodel.Name = GUI_Constants.TXT_COMP_500B_PROGMODEL;
                        item_component_500b_progmodel.Value = (int)VersionedComponent.VERSIONED_COMPONENT_500B_PROGMODEL;
                        fillBox.Items.Add(item_component_500b_progmodel);

                        GUI_BoxItems item_component_600_progmodel = new GUI_BoxItems();
                        item_component_600_progmodel.Name = GUI_Constants.TXT_COMP_600_PROGMODEL;
                        item_component_600_progmodel.Value = (int)VersionedComponent.VERSIONED_COMPONENT_600_PROGMODEL;
                        fillBox.Items.Add(item_component_600_progmodel);

                        GUI_BoxItems item_component_600b_progmodel = new GUI_BoxItems();
                        item_component_600b_progmodel.Name = GUI_Constants.TXT_COMP_600B_PROGMODEL;
                        item_component_600b_progmodel.Value = (int)VersionedComponent.VERSIONED_COMPONENT_600B_PROGMODEL;
                        fillBox.Items.Add(item_component_600b_progmodel);

                        GUI_BoxItems item_component_600d2_progmodel = new GUI_BoxItems();
                        item_component_600d2_progmodel.Name = GUI_Constants.TXT_COMP_600D2_PROGMODEL;
                        item_component_600d2_progmodel.Value = (int)VersionedComponent.VERSIONED_COMPONENT_600D2_PROGMODEL;
                        fillBox.Items.Add(item_component_600d2_progmodel);

                        GUI_BoxItems item_component_700_progmodel = new GUI_BoxItems();
                        item_component_700_progmodel.Name = GUI_Constants.TXT_COMP_700_PROGMODEL;
                        item_component_700_progmodel.Value = (int)VersionedComponent.VERSIONED_COMPONENT_700_PROGMODEL;
                        fillBox.Items.Add(item_component_700_progmodel);

                        GUI_BoxItems item_component_700b_progmodel = new GUI_BoxItems();
                        item_component_700b_progmodel.Name = GUI_Constants.TXT_COMP_700B_PROGMODEL;
                        item_component_700b_progmodel.Value = (int)VersionedComponent.VERSIONED_COMPONENT_700B_PROGMODEL;
                        fillBox.Items.Add(item_component_700b_progmodel);
                        break;
                    }
                case GUI_Constants.TAG_COMBOX_REGULATION_TYPE:
                    {
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_UNKNOWN, (int)RegulationType.REGULATION_TYPE_UNKNOWN));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_FCC_SP, (int)RegulationType.REGULATION_TYPE_FCC_SP));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_FCC_LPI, (int)RegulationType.REGULATION_TYPE_FCC_LPI));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_DOC, (int)RegulationType.REGULATION_TYPE_DOC));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_ETSI, (int)RegulationType.REGULATION_TYPE_ETSI));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_SPAIN, (int)RegulationType.REGULATION_TYPE_SPAIN));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_FRANCE, (int)RegulationType.REGULATION_TYPE_FRANCE));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_UAE, (int)RegulationType.REGULATION_TYPE_UAE));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_GERMANY, (int)RegulationType.REGULATION_TYPE_GERMANY));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_MKK, (int)RegulationType.REGULATION_TYPE_MKK));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_ISRAEL, (int)RegulationType.REGULATION_TYPE_ISRAEL));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_SINGAPORE, (int)RegulationType.REGULATION_TYPE_SINGAPORE));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_AUSTRALIA, (int)RegulationType.REGULATION_TYPE_AUSTRALIA));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_BRAZIL, (int)RegulationType.REGULATION_TYPE_BRAZIL));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_CHINA, (int)RegulationType.REGULATION_TYPE_CHINA));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_KOREA, (int)RegulationType.REGULATION_TYPE_KOREA));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_APAC, (int)RegulationType.REGULATION_TYPE_APAC));
                        fillBox.Items.Add(new GUI_BoxItems(GUI_Constants.TXT_REGULATION_TYPE_JAPAN, (int)RegulationType.REGULATION_TYPE_JAPAN));
                        break;
                    }
            }
        }

        /// <summary>
        /// Loads band values and selects operating band.
        /// </summary>


        /// <summary>
        /// Returns int value of selected item in ComboBox if ComboBox is initialized.
        /// </summary>

        public static int GUI_GetSelectedValue(ComboBox combox)
        {
            if (combox.SelectedIndex == GUI_Constants.GUI_COMBOX_ERROR)
            {
                return GUI_Constants.GUI_COMBOX_DEFAULT;
            }

            return ((GUI_BoxItems)combox.SelectedItem).Value;
        }

        /// <summary>
        /// Returns the ComboBox index for the itemValue.
        /// </summary>

        public static int GUI_GetValueAsIndex(ComboBox combox, int itemValue)
        { // Return combox index of the item with value that you want selected
            foreach (GUI_BoxItems boxItem in combox.Items)
            {
                if (boxItem.Value.Equals(itemValue))
                {
                    int index = combox.Items.IndexOf(boxItem);
                    return index;
                }
            }
            return GUI_Constants.GUI_COMBOX_ERROR;
        }

        /// <summary>
        /// <para> Returns index value for selected channel. </para>
        /// <para> combox_basicOp_lowChan is filled with a list of sequential integers due to size, not GUI_BoxItems </para>
        /// </summary>

        public static int GUI_GetChannelAsIndex(ComboBox comboBox, byte channel)
        { // returns channel index value, as combox_basicOp_lowChan is filled with a list of int
            int index = comboBox.Items.IndexOf(channel);
            if (index == -1)
            {
                index = 0;
            }

            return index;
        }

        /// <summary>
        /// Returns combox_basicOp_lowChan channel value based on selected index.
        /// </summary>

        public static int GUI_GetCurrentChannelValue(ComboBox combox)
        {
            return ++combox.SelectedIndex; // combox_basicOp_lowchan is populated with int items beginning from 1 sequentially
        }


        public static void GUI_ReorderBoxItemsAsIndexPrefix(ComboBox combox)
        {
            int index = 0;
            string newName;
            GUI_BoxItems[] tempBoxItems = new GUI_BoxItems[combox.Items.Count];
            foreach (GUI_BoxItems boxItem in combox.Items)
            {
                tempBoxItems[index] = boxItem;
                index++;
            }
            combox.Items.Clear();

            index = 0;
            foreach (GUI_BoxItems boxItem in tempBoxItems)
            {
                newName = index.ToString() + ". " + boxItem.boxItemName;
                boxItem.boxItemName = newName;
                index++;
            }
            combox.Items.AddRange(tempBoxItems);

        }
        /// <summary>
        /// Removes all items from ComboBox that have value bigger than maxValue.
        /// </summary>

        public static void GUI_removeMultipleBoxItems(ComboBox combox, int maxValue)
        { // removes multiple selection choices from Combo Box
            List<int> removeItemWithValue = new List<int>();
            int[] removeItemsArray;
            int arraySize, i;

            foreach (GUI_BoxItems boxItem in combox.Items)
            {
                if (boxItem.Value > maxValue)
                { // removes items with value larger than max allowed
                    removeItemWithValue.Add(boxItem.Value);
                }
            }

            removeItemsArray = removeItemWithValue.ToArray();
            arraySize = removeItemWithValue.Count;

            for (i = 0; i < arraySize; i++)
            {
                GUI_removeSingleBoxItem(combox, removeItemsArray[i]);
            }
        }

        /// <summary>
        /// Removes all items from ComboBox that have value Smaller Than minValue.
        /// </summary>
        public static void GUI_removeMultipleBoxItems_ST(ComboBox combox, int minValue)
        { // removes multiple selection choices from Combo Box
            List<int> removeItemWithValue = new List<int>();
            int[] removeItemsArray;
            int arraySize, i;

            foreach (GUI_BoxItems boxItem in combox.Items)
            {
                if (boxItem.Value < minValue)
                { // removes items with value larger than max allowed
                    removeItemWithValue.Add(boxItem.Value);
                }
            }

            removeItemsArray = removeItemWithValue.ToArray();
            arraySize = removeItemWithValue.Count;

            for (i = 0; i < arraySize; i++)
            {
                GUI_removeSingleBoxItem(combox, removeItemsArray[i]);
            }
        }
        /// <summary>
        /// Removes single item from ComboBox that has the indicated itemValue.
        /// </summary>

        public static void GUI_removeSingleBoxItem(ComboBox combox, int itemValue)
        { // removes single object item from Combo Box
            foreach (GUI_BoxItems boxItem in combox.Items)
            {
                if (boxItem.Value.Equals(itemValue))
                {
                    combox.Items.Remove(boxItem);
                    return;
                }
            }
        } // end of GUI_removeBoxItem
    }
}
