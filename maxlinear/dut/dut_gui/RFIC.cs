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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using dut_cli;

namespace DUT_GUI
{

    public partial class RFIC : Form
    {
        readonly DUT_t DUT;
        readonly bool isInit = false;
        UInt16[][] RFICdata;
        public const int RFIC_NUM_OF_SHORT_REGS = (int)4;
        public const int RFIC_NUM_OF_LONG_REGS = (int)64;
        public const int RFIC_FIRST_SHORT_REG_ADDR = (int)(0x0);
        public const int RFIC_FIRST_LONG_REG_ADDR = (int)(0x40);
        public const int FCSI_TOTAL_CHANNELS = (int)5; //antenna0-4, central
        public const int FCSI_REG_BYTE_LENGTH = (int)FCSI_TOTAL_CHANNELS * 2; //2 bytes for each channel
        public const int RFIC_NUM_OF_REGS = RFIC_NUM_OF_SHORT_REGS + RFIC_NUM_OF_LONG_REGS;
        private readonly RficRfRegistersDB_t[] RficDriverRegistersLocalDB;
        const int RFIC_COL_NUMBER = FCSI_TOTAL_CHANNELS;

        public RFIC(DUT_t DUT)
        {
            this.DUT = DUT;
            InitializeComponent();

            isInit = true;
            for (int col = 0; col < 16; ++col)
            {
                dataGrid.Columns.Add(new DataGridViewTextBoxColumn());
                dataGrid.Columns[col].HeaderText = col.ToString("x");
            }

            RficDriverRegistersLocalDB = new RficRfRegistersDB_t[RFIC_NUM_OF_REGS];
            for (int dbIndex = 0; dbIndex < RficDriverRegistersLocalDB.Length; dbIndex++)
            {
                RficDriverRegistersLocalDB[dbIndex] = new RficRfRegistersDB_t();
            }
            rficStoreRegDB();

            int rowsNumber = RFIC_NUM_OF_REGS;
            RFICdata = new UInt16[rowsNumber][];
            int colNumber = RFIC_COL_NUMBER;
            for (uint row = 0; row < rowsNumber; ++row)
            {
                string[] myRow = new string[colNumber];

                RFICdata[row] = new UInt16[colNumber];
                for (int col = 0; col < colNumber; ++col)
                {
                    RFICdata[row][col] = RficDriverRegistersLocalDB[row].regData[col];
                    myRow[col] = RFICdata[row][col].ToString("X4") + "h";
                }
                dataGrid.Rows.Add(myRow);
                dataGrid.Rows[(int)row].HeaderCell.Value = RficDriverRegistersLocalDB[row].regAddr.ToString("X4");
            }
            dataGrid.TopLeftHeaderCell.Value = "Address";
            dataGrid.Columns[0].HeaderCell.Value = "Ant0";
            dataGrid.Columns[1].HeaderCell.Value = "Ant1";
            dataGrid.Columns[2].HeaderCell.Value = "Ant2";
            dataGrid.Columns[3].HeaderCell.Value = "Ant3";
            dataGrid.Columns[4].HeaderCell.Value = "Central";
            ResizeDataGrid();
        }

        void ResizeDataGrid()
        {
            if (!isInit) return; // too early !
            int fullWidth = dataGrid.Width - 65;
            double cellWidth = fullWidth / (double)RFIC_COL_NUMBER;
            int totalWidth = 0;
            for (int col = 0; col < RFIC_COL_NUMBER; ++col)
            {
                int width = (col < RFIC_COL_NUMBER - 1) ? (int)cellWidth : (fullWidth - totalWidth);
                totalWidth += width;
                dataGrid.Columns[col].Width = width;
            }
            dataGrid.RowHeadersWidth = 60;
        }

        private void RFIC_Resize(object sender, EventArgs e)
        {
            ResizeDataGrid();
        }

        private void dataGrid_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!viewOnly.Checked)
            {
                byte data = (byte)DUT_GUI.ParseString((string)dataGrid[e.ColumnIndex, e.RowIndex].Value);
                if (data != RficDriverRegistersLocalDB[e.RowIndex].regData[e.ColumnIndex])
                {
                    RficDriverRegistersLocalDB[e.RowIndex].regData[e.ColumnIndex] = data;
                }
            }
        }

        private void viewOnly_CheckedChanged(object sender, EventArgs e)
        {
            dataGrid.ReadOnly = viewOnly.Checked;
        }


        private bool rficStoreRegDB()
        {
            bool res = true;

            //Store short registers
            for (uint entryInd = 0; entryInd < RFIC_NUM_OF_SHORT_REGS; entryInd++)
            {
                /* Calculate DB index */
                uint dbIndex = entryInd;

                /* Configure the address to read (and also save it to the DB) */
                uint address = entryInd + RFIC_FIRST_SHORT_REG_ADDR;
                RficDriverRegistersLocalDB[dbIndex].regAddr = (UInt16)address;

                /* Read the values from the RF registers into the DB */
                if (DUT.ReadMemory(ChipModule.CHIP_MODULE_RF, address, out byte[] data, FCSI_REG_BYTE_LENGTH))
                {
                    for (int i = 0; i < FCSI_TOTAL_CHANNELS; i++)
                    {
                        RficDriverRegistersLocalDB[dbIndex].regData[i] = BitConverter.ToUInt16(data, i * 2);
                    }
                }
            }

            //Store long registers
            for (uint entryInd = 0; entryInd < RFIC_NUM_OF_LONG_REGS; entryInd++)
            {
                /* Calculate DB index */
                uint dbIndex = entryInd + RFIC_NUM_OF_SHORT_REGS;

                /* Configure the address to read (and also save it to the DB) */
                uint address = entryInd + RFIC_FIRST_LONG_REG_ADDR;
                RficDriverRegistersLocalDB[dbIndex].regAddr = (UInt16)address;

                /* Read the values from the RF registers into the DB */
                if (DUT.ReadMemory(ChipModule.CHIP_MODULE_RF, address, out byte[] data, FCSI_REG_BYTE_LENGTH))
                {
                    for (int i = 0; i < FCSI_TOTAL_CHANNELS; i++)
                    {
                        RficDriverRegistersLocalDB[dbIndex].regData[i] = BitConverter.ToUInt16(data, i * 2);
                    }
                }
            }

            return res;
        }
        private bool rficLoadRegDB()
        {
            byte[] data = new byte[FCSI_TOTAL_CHANNELS * 2];
            bool res = true;
            for (int i = 0; i < RFIC_NUM_OF_REGS; i++)
            {
                for (int j = 0; j < FCSI_TOTAL_CHANNELS; j++)
                {
                    byte[] byteArray = BitConverter.GetBytes(RficDriverRegistersLocalDB[i].regData[j]);
                    data[j * 2] = byteArray[0];
                    data[j * 2 + 1] = byteArray[1];
                }
                res = DUT.WriteMemory(ChipModule.CHIP_MODULE_RF, (uint)RficDriverRegistersLocalDB[i].regAddr, data, (uint)data.Length);
            }
            return res;
        }
        private void readRFIC_Click(object sender, EventArgs e)
        {
            rficStoreRegDB();
            int rowsNumber = RFIC_NUM_OF_REGS;
            RFICdata = new UInt16[rowsNumber][];
            int colNumber = RFIC_COL_NUMBER;
            for (uint row = 0; row < rowsNumber; ++row)
            {
                string[] myRow = new string[colNumber];

                RFICdata[row] = new UInt16[colNumber];
                for (int col = 0; col < colNumber; ++col)
                {
                    RFICdata[row][col] = RficDriverRegistersLocalDB[row].regData[col];
                    myRow[col] = RFICdata[row][col].ToString("X4") + "h";
                }
                dataGrid.Rows[(int)row].SetValues(myRow);
                dataGrid.Rows[(int)row].HeaderCell.Value = RficDriverRegistersLocalDB[row].regAddr.ToString("X4");
            }

        }
        private void writeRFIC_Click(object sender, EventArgs e)
        {
            if (rficLoadRegDB())
            {
                MessageDialog.ShowInformation("Done!");
            }
        }
    }
    public class RficRfRegistersDB_t
    {
        public UInt16 regAddr;
        public UInt16[] regData = new UInt16[RFIC.FCSI_TOTAL_CHANNELS];

    }
}