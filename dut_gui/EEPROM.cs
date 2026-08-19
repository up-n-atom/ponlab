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

namespace DUT_GUI
{
    public partial class EEPROM : Form
    {
        readonly DUT_t DUT;
        readonly bool isInit = false;
        readonly Byte[][] EEPROMdata;
        readonly Byte[] allEEPROM;
        bool changed;
        public void SaveOnExit()
        {
            if (changed)
            {
                if (DialogResult.Yes == MessageBox.Show("Would you like to burn your changes to the EEPROM ?", "Save Change to EEPROM", MessageBoxButtons.YesNo))
                {
                    DUT.BurnEEPROM(0, allEEPROM, (uint)allEEPROM.Length);
                }
            }
        }
        public EEPROM(DUT_t DUT)
        {
            this.DUT = DUT;
            changed = false;
            InitializeComponent();
            isInit = true;
            for (int col = 0; col < 16; ++col)
            {
                dataGrid.Columns.Add(new DataGridViewTextBoxColumn());
                dataGrid.Columns[col].HeaderText = col.ToString("x");
            }

            if (DUT.ReadEEPROM(0, out allEEPROM))
            {
                int rowsNumber = allEEPROM.Length / 16;
                EEPROMdata = new Byte[rowsNumber][];

                for (uint row = 0; row < rowsNumber; ++row)
                {
                    string[] myRow = new string[16];

                    EEPROMdata[row] = new Byte[16];
                    for (int col = 0; col < 16; ++col)
                    {
                        EEPROMdata[row][col] = allEEPROM[row * 16 + col];
                        myRow[col] = EEPROMdata[row][col].ToString("X2") + "h";
                    }
                    dataGrid.Rows.Add(myRow);
                    dataGrid.Rows[(int)row].HeaderCell.Value = (row * 16).ToString("X2");
                }
                ResizeDataGrid();
            }
        }

        void ResizeDataGrid()
        {
            if (!isInit) return; // too early !
            int fullWidth = dataGrid.Width - 65;
            double cellWidth = fullWidth / 16.0;
            int totalWidth = 0;
            for (int col = 0; col < 16; ++col)
            {
                int width = (col < 15) ? (int)cellWidth : (fullWidth - totalWidth);
                totalWidth += width;
                dataGrid.Columns[col].Width = width;
            }
            dataGrid.RowHeadersWidth = 60;
        }

        private void EEPROM_Resize(object sender, EventArgs e)
        {
            ResizeDataGrid();
        }

        private void dataGrid_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!viewOnly.Checked)
            {
                byte data = (byte)DUT_GUI.ParseString((string)dataGrid[e.ColumnIndex, e.RowIndex].Value);
                if (data != allEEPROM[e.ColumnIndex + e.RowIndex * 16])
                {
                    changed = true;
                    allEEPROM[e.ColumnIndex + e.RowIndex * 16] = data;
                    EEPROMdata[e.RowIndex][e.ColumnIndex] = data;
                    //DUT.BurnEEPROM((uint)(e.ColumnIndex + e.RowIndex * 16), (byte)DUT_GUI.ParseString((string)dataGrid[e.ColumnIndex, e.RowIndex].Value));
                }
            }
        }

        private void viewOnly_CheckedChanged(object sender, EventArgs e)
        {
            dataGrid.ReadOnly = viewOnly.Checked;
        }

        private void exportEEPROM_Click(object sender, EventArgs e)
        {
            try
            {
                if (saveEEPROMDialog.ShowDialog() != DialogResult.OK) return;
                string filename = saveEEPROMDialog.FileName;
                bool isBinary = Path.GetExtension(filename).ToUpper() == ".BIN";
                if (isBinary)
                {
                    BinaryWriter bw = new BinaryWriter(File.Open(filename, FileMode.Create));
                    for (int row = 0; row < EEPROMdata.Length; ++row) bw.Write(EEPROMdata[row]);
                    bw.Close();
                }
                else // Text file
                {
                    StreamWriter sw = new StreamWriter(filename);
                    sw.WriteLine("// EEPROM exported by DUT GUI V. " + DUT_t.Version);
                    sw.WriteLine("// " + DateTime.Now.ToString("g"));
                    for (int row = 0; row < EEPROMdata.Length; ++row)
                    {
                        int addr = row << 4;
                        sw.Write(addr.ToString("X4") + "\t");
                        for (int col = 0; col < 16; ++col)
                        {
                            sw.Write(EEPROMdata[row][col].ToString("X2"));
                        }
                        sw.WriteLine();
                    }
                    sw.Close();
                }
            }
            catch (System.Exception ex)
            {
                MessageDialog.ShowError("Failed to export: " + ex.Message);
            }
        }
    }
}