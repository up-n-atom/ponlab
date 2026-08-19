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

namespace DUT_GUI
{
	partial class EEPROM
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
      this.dataGrid = new System.Windows.Forms.DataGridView();
      this.viewOnly = new System.Windows.Forms.CheckBox();
      this.exportEEPROM = new System.Windows.Forms.Button();
      this.saveEEPROMDialog = new System.Windows.Forms.SaveFileDialog();
      ((System.ComponentModel.ISupportInitialize)(this.dataGrid)).BeginInit();
      this.SuspendLayout();
      // 
      // dataGrid
      // 
      this.dataGrid.AllowDrop = true;
      this.dataGrid.AllowUserToAddRows = false;
      this.dataGrid.AllowUserToDeleteRows = false;
      this.dataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.dataGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.dataGrid.Location = new System.Drawing.Point(12, 12);
      this.dataGrid.Name = "dataGrid";
      this.dataGrid.ReadOnly = true;
      this.dataGrid.Size = new System.Drawing.Size(540, 439);
      this.dataGrid.TabIndex = 0;
      this.dataGrid.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGrid_CellEndEdit);
      // 
      // viewOnly
      // 
      this.viewOnly.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this.viewOnly.AutoSize = true;
      this.viewOnly.Checked = true;
      this.viewOnly.CheckState = System.Windows.Forms.CheckState.Checked;
      this.viewOnly.Location = new System.Drawing.Point(14, 466);
      this.viewOnly.Name = "viewOnly";
      this.viewOnly.Size = new System.Drawing.Size(103, 17);
      this.viewOnly.TabIndex = 1;
      this.viewOnly.Text = "View Only Mode";
      this.viewOnly.UseVisualStyleBackColor = true;
      this.viewOnly.CheckedChanged += new System.EventHandler(this.viewOnly_CheckedChanged);
      // 
      // exportEEPROM
      // 
      this.exportEEPROM.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
      this.exportEEPROM.Location = new System.Drawing.Point(415, 456);
      this.exportEEPROM.Margin = new System.Windows.Forms.Padding(2);
      this.exportEEPROM.Name = "exportEEPROM";
      this.exportEEPROM.Size = new System.Drawing.Size(138, 27);
      this.exportEEPROM.TabIndex = 2;
      this.exportEEPROM.Text = "Save to text/binary file...";
      this.exportEEPROM.UseVisualStyleBackColor = true;
      this.exportEEPROM.Click += new System.EventHandler(this.exportEEPROM_Click);
      // 
      // saveEEPROMDialog
      // 
      this.saveEEPROMDialog.DefaultExt = "txt";
      this.saveEEPROMDialog.Filter = "Text files (*.txt)|*.txt|Binary fies (*.bin)|*.bin|All Files|*.*";
      // 
      // EEPROM
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
      this.ClientSize = new System.Drawing.Size(564, 493);
      this.Controls.Add(this.exportEEPROM);
      this.Controls.Add(this.viewOnly);
      this.Controls.Add(this.dataGrid);
      this.Name = "EEPROM";
      this.Text = "EEPROM";
      this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
      this.Resize += new System.EventHandler(this.EEPROM_Resize);
      ((System.ComponentModel.ISupportInitialize)(this.dataGrid)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.DataGridView dataGrid;
		private System.Windows.Forms.CheckBox viewOnly;
		private System.Windows.Forms.Button exportEEPROM;
    private System.Windows.Forms.SaveFileDialog saveEEPROMDialog;
	}
}