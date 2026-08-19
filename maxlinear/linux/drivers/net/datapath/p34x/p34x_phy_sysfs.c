// SPDX-License-Identifier: GPL-2.0
/*
 * P34X FW Download Driver
 *
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2019-2020, Intel Corporation.
 * Govindaiah Mudepalli <gmudepalli@maxlinear.com>
 * Xu Liang <lxu@maxlinear.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/platform_device.h>

#include "p34x_phy.h"

static ssize_t linksts_show(struct device *dev, struct device_attribute *attr,
			    char *buf)
{
	struct p34x_priv_data *pdata;
	int off = 0;
	u32 val;

	pdata = dev_get_drvdata(dev);

	off = snprintf(buf + off, PAGE_SIZE - off, "\nXPCS: LINK Status\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	off += snprintf(buf + off, PAGE_SIZE - off, "\nPMA Tx status\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_16G_25G_TX_STS */
	val = p34x_xpcs_rd(pdata, 0x18040);
	off += snprintf(buf + off, PAGE_SIZE - off, "\tTX_ACK_0:                            %s\n",
		       (val & BIT(0)) ? "Tx Acknowledge on Lane 0" :
		       "Tx NOT Acknowledge on Lane 0");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tRx Detection Result on lane 0:       %s\n",
		       (val & BIT(4)) ? "Rx Detected on Lane 0" :
		       "Rx NOT Detected on Lane 0");

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_16G_25G_TX_POWER_STATE_CTRL */
	val = p34x_xpcs_rd(pdata, 0x18035);
	off += snprintf(buf + off, PAGE_SIZE - off,
		       "\tTX0_PSTATE:                          %08x\n",
		       val & 0x03);

	off += snprintf(buf + off, PAGE_SIZE - off, "\nPMA Rx status\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	/* XPCS_SR_PMA_STATUS1 */
	val = p34x_xpcs_rd(pdata, 0x10001);
	off += snprintf(buf + off, PAGE_SIZE - off, "\tPMA_STS:                             %s\n",
		       (val & BIT(2)) ? "LINK UP" : "LINK DOWN");

	/* XPCS_SR_PMA_STATUS2 */
	val = p34x_xpcs_rd(pdata, 0x10008);
	off += snprintf(buf + off, PAGE_SIZE - off, "\tPMA_STATUS2 RF:                      %s\n",
		       (val & BIT(10)) ? "Rx Fault" : "Rx No Fault");

	/* XPCS_SR_PMA_KR_PMD_STS */
	val = p34x_xpcs_rd(pdata, 0x10097);
	off += snprintf(buf + off, PAGE_SIZE - off, "\tPMA_KR_PMD_STS RCV_STS:              %s\n",
		       (val & BIT(0)) ? "Received trained and ready to receive data" :
		       "Rx training");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tPMA_KR_PMD_STS FRM_LCK:              %s\n",
		       (val & BIT(1)) ? "Training frame delineation detected" :
		       "Training frame delineation not detected");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tPMA_KR_PMD_STS SU_PR_DTD:            %s\n",
		       (val & BIT(2)) ? "Start-up protocol in progress" :
		       "Start-up protocol complete");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tPMA_KR_PMD_STS TR_FAIL:              %s\n",
		       (val & BIT(3)) ? "Training is completed with failure" :
		       "Training is not yet complete");

	off += snprintf(buf + off, PAGE_SIZE - off, "\n10GBASE-KR Link Partner coeff Update\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	/* XPCS_SR_PMA_KR_LP_CEU */
	val = p34x_xpcs_rd(pdata, 0x10098);
	off += snprintf(buf + off, PAGE_SIZE - off, "\tPMA_KR_PMD_STS LP_INIT:              %s\n",
		       (val & BIT(12)) ? "Init the local device tx filter coeff" :
		       "Normal operation");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tPMA_KR_PMD_STS LP_PRST:              %s\n",
		       (val & BIT(13)) ? "Local device Pre-initialize coeffs" :
		       "Normal operation");

	off += snprintf(buf + off, PAGE_SIZE - off, "\n10GBASE-KR Link Partner coeff Status\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	/* XPCS_SR_PMA_KR_LP_CESTS */
	val = p34x_xpcs_rd(pdata, 0x10099) & BIT(15);
	off += snprintf(buf + off, PAGE_SIZE - off, "\tPMA_KR_LP_CESTS LP_RR:               %s\n",
		       val ? "The Link Partner rx has determined that the training is "
		       "complete, and it is ready to receive data." :
		       "The Link Partner rx is requesting that the training "
		       "should be continued.");

	off += snprintf(buf + off, PAGE_SIZE - off, "\n10GBASE-KR LD coeff Update Local Device "
		       "requests to Link Partner.\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	/* XPCS_SR_PMA_KR_LD_CEU */
	val = p34x_xpcs_rd(pdata, 0x1009a);
	off += snprintf(buf + off, PAGE_SIZE - off, "\tPMA_KR_LD_CEU LD_INIT:               %s\n",
		       (val & BIT(12)) ? "Init the link partner tx filter coeff" :
		       "Normal operation");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tPMA_KR_LD_CEU PRST:                  %s\n",
		       (val & BIT(13)) ? "LP Pre-initialize coeffs" :
		       "Normal operation");

	off += snprintf(buf + off, PAGE_SIZE - off, "\n10GBASE-KR LD coeff Status\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	/* XPCS_SR_PMA_KR_LD_CESTS */
	val = p34x_xpcs_rd(pdata, 0x1009b) & BIT(15);
	off += snprintf(buf + off, PAGE_SIZE - off, "\tPMA_KR_LD_CESTS RR:                  %s\n",
		       val ? "The local device rx has determined that the training is "
		       "complete, and it is ready to receive data." :
		       "The local device rx is requesting that the training "
		       "should be continued.");

	off += snprintf(buf + off, PAGE_SIZE - off, "\nAutonegotiation Status Register\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	/* XPCS_SR_AN_STS */
	val = p34x_xpcs_rd(pdata, 0x70001);
	off += snprintf(buf + off, PAGE_SIZE - off, "\tLP AN Ability:                       %s\n",
		       (val & BIT(0)) ? "LP able to participate in AN" :
		       "LP unable to participate in AN");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tAN Link Status:                      %s\n",
		       (val & BIT(2)) ? "CL73 AN complete and determined a valid link" :
		       "CL73 not complete and no valid link");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tLD AN Ability:                       %s\n",
		       (val & BIT(3)) ? "LD supports CL73 AN" :
		       "LD does not support CL73 AN");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tAN Remote Fault:                     %s\n",
		       (val & BIT(4)) ? "AN process detected a remote fault" :
		       "No fault detected");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tAN Complete:                         %s\n",
		       (val & BIT(5)) ? "AN completes" :
		       "AN not complete");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tPage Received:                       %s\n",
		       (val & BIT(6)) ? "Page is received and the corresponding "
		       "Link Code Word is stored " : "No page received");


	off += snprintf(buf + off, PAGE_SIZE - off, "\tParallel Detection Fault in CL73 :   %s\n",
		       (val & BIT(9)) ? "Parallel detection fault detected " :
		       "No fault detected");

	off += snprintf(buf + off, PAGE_SIZE - off, "\nPCS Rx Status\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	/* XPCS_SR_XS_PCS_STS1 */
	val = p34x_xpcs_rd(pdata, 0x30001) & BIT(2);
	off += snprintf(buf + off, PAGE_SIZE - off, "\tPCS_STS:                             %s\n",
		       val ? "LINK UP" : "LINK DOWN");

	/* XPCS_SR_XS_PCS_STS2 */
	val = p34x_xpcs_rd(pdata, 0x30008);
	off += snprintf(buf + off, PAGE_SIZE - off, "\tPCS_STATUS2 RF:                      %s\n",
		       (val & BIT(10)) ? "Rx Fault" : "Rx No Fault");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tPCS_STATUS2 TF:                      %s\n",
		       (val & BIT(11)) ? "Tx Fault" : "Tx No Fault");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tCAP_EN:                              %s\n",
		       (val & BIT(0)) ? "10GBASE-R Capable" : "10GBASE-R Not Capable");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tCAP_10_1GC:                          %s\n",
		       (val & BIT(1)) ? "10GBASE-X Capable" : "10GBASE-X Not Capable");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tCAP_10GBW:                           %s\n",
		       (val & BIT(2)) ? "10GBASE-W Capable" : "10GBASE-W Not Capable");

	off += snprintf(buf + off, PAGE_SIZE - off, "\tCAP_10GBT:                           %s\n",
		       (val & BIT(3)) ? "10GBASE-T Capable" : "10GBASE-T Not Capable");

	val = (val >> 14) & 0x3;
	off += snprintf(buf + off, PAGE_SIZE - off, "\tDS:                                  %s\n",
		       (val == 2) ?
		       "MMD is present and responding to this reg addr" :
		       "MMD is not present or not functioning properly");

	/* XPCS_SR_XS_PCS_KR_STS2 */
	val = p34x_xpcs_rd(pdata, 0x30021);
	off += snprintf(buf + off, PAGE_SIZE - off,
		       "\tPCS_KR_STS2 :                        %08X\n", val);

	/* XPCS_SR_XS_PCS_CTRL2 */
	val = p34x_xpcs_rd(pdata, 0x30007) & 0xf;
	switch (val) {
	case 0:
		off += snprintf(buf + off, PAGE_SIZE - off,
			       "\tPCS_TYPE_SEL:                        %s\n",
			       "10GBASE-R PCS Type");
		break;
	case 1:
		off += snprintf(buf + off, PAGE_SIZE - off,
			       "\tPCS_TYPE_SEL:                        %s\n",
			       "10GBASE-X PCS Type");
		break;
	case 2:
		off += snprintf(buf + off, PAGE_SIZE - off,
			       "\tPCS_TYPE_SEL:                        %s\n",
			       "10GBASE-W PCS Type");
		break;
	case 14:
		off += snprintf(buf + off, PAGE_SIZE - off,
			       "\tPCS_TYPE_SEL:                        %s\n",
			       "2.5GBASE-X PCS Type");
		break;
	case 15:
		off += snprintf(buf + off, PAGE_SIZE - off,
			       "\tPCS_TYPE_SEL:                        %s\n",
			       "5GBASE-X PCS Type");
		break;
	default:
		off += snprintf(buf + off, PAGE_SIZE - off,
			       "\tPCS_TYPE_SEL:                        %s\n",
			       "Reserved");
	}

	off += snprintf(buf + off, PAGE_SIZE - off, "\nMII Status\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	/* XPCS_SR_MII_STS */
	val = p34x_xpcs_rd(pdata, 0x1f0001) & BIT(2);
	off += snprintf(buf + off, PAGE_SIZE - off, "\t1000BASE-X mode Rx:                  %s\n",
		       val ? "LINK UP" : "LINK DOWN");

	/* XPCS_VR_MII_AN_CTRL */
	val = p34x_xpcs_rd(pdata, 0x1f8001) & BIT(4);
	off += snprintf(buf + off, PAGE_SIZE - off, "\tSGMII/QSGMII/USXGMII mode Port0 Rx:  %s\n",
		       val ? "LINK UP" : "LINK DOWN");

	return off;
}
static DEVICE_ATTR_RO(linksts);

static ssize_t cl73_info_show(struct device *dev, struct device_attribute *attr,
			      char *buf)
{
	struct p34x_priv_data *pdata;
	int off = 0;
	u32 val;

	pdata = dev_get_drvdata(dev);

	off = snprintf(buf + off, PAGE_SIZE - off, "\nXPCS: CL 73 Status\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	/* XPCS_VR_AN_INTR */
	val = p34x_xpcs_rd(pdata, 0x78002);
	off += snprintf(buf + off, PAGE_SIZE - off, "AN Complete Interrupt:                 %s\n",
		       (val & BIT(0)) ? "SET" : "NOT_SET");

	off += snprintf(buf + off, PAGE_SIZE - off, "AN Incompatible Link:                  %s\n",
		       (val & BIT(1)) ? "SET" : "NOT_SET");

	off += snprintf(buf + off, PAGE_SIZE - off, "AN Page Received:                      %s\n",
		       (val & BIT(2)) ? "SET" : "NOT_SET");

	/* XPCS_SR_AN_STS */
	val = p34x_xpcs_rd(pdata, 0x70001);
	off += snprintf(buf + off, PAGE_SIZE - off, "AN_STS:                                %s\n",
		       (val & BIT(0)) ? "LP able to participate in AN" :
		       "LP unable to participate in AN");

	off += snprintf(buf + off, PAGE_SIZE - off, "AN_STS:                                %s\n",
		       (val & BIT(2)) ? "CL73 AN complete and determined a valid link" :
		       "CL73 not complete and no valid link");

	off += snprintf(buf + off, PAGE_SIZE - off, "AN_STS:                                %s\n",
		       (val & BIT(3)) ? "LD supports CL73 AN" :
		       "LD does not support CL73 AN");

	off += snprintf(buf + off, PAGE_SIZE - off, "AN_STS:                                %s\n",
		       (val & BIT(4)) ? "AN process detected a remote fault" :
		       "No fault detected");

	off += snprintf(buf + off, PAGE_SIZE - off, "AN_STS:                                %s\n",
		       (val & BIT(5)) ? "AN completes" :
		       "AN not complete");

	off += snprintf(buf + off, PAGE_SIZE - off, "AN_STS:                                %s\n",
		       (val & BIT(6)) ? "Parallel detection fault detected " :
		       "No fault detected");

	off += snprintf(buf + off, PAGE_SIZE - off, "AN_STS:                                %s\n",
		       (val & BIT(9)) ? "Parallel detection fault detected " :
		       "No fault detected");

	return off;
}
static DEVICE_ATTR_RO(cl73_info);

static ssize_t table_show(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	struct p34x_priv_data *pdata;
	int off = 0;
	u32 val;
	u32 mplla_ctrl2 = 0, afe_dfe_ctrl;
	u32 tx_eq0, tx_eq1;

	pdata = dev_get_drvdata(dev);

	off = snprintf(buf + off, PAGE_SIZE - off, "\nXPCS: Table Show\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	off += snprintf(buf + off, PAGE_SIZE - off, "ID:                            USXGMII\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_16G_MPLLA_CTRL0 */
	val = p34x_xpcs_rd(pdata, 0x18071) & 0xff;
	off += snprintf(buf + off, PAGE_SIZE - off, "MPLLA_MULTIPLIER:              %x\n", val);

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_MPLLA_CTRL3 */
	val = p34x_xpcs_rd(pdata, 0x18077) & 0xffff;
	off += snprintf(buf + off, PAGE_SIZE - off, "MPPLA_BANDWIDTH:               %x\n", val);

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_16G_25G_VCO_CAL_LD0 */
	val = p34x_xpcs_rd(pdata, 0x18092) & 0x1fff;
	off += snprintf(buf + off, PAGE_SIZE - off, "VCO_LD_VAL_0:                  %x\n", val);

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_VCO_CAL_REF0 */
	val = p34x_xpcs_rd(pdata, 0x18096) & 0x3f;
	off += snprintf(buf + off, PAGE_SIZE - off, "VCO_REF_LD_0:                  %x\n", val);

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_16G_25G_TX_RATE_CTRL */
	val = p34x_xpcs_rd(pdata, 0x18034) & 0x7;
	off += snprintf(buf + off, PAGE_SIZE - off, "TX_RATE_0:                     %x\n", val);

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_16G_25G_RX_RATE_CTRL */
	val = p34x_xpcs_rd(pdata, 0x18054) & 0x3;
	off += snprintf(buf + off, PAGE_SIZE - off, "RX_RATE_0:                     %x\n", val);

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_16G_TX_GENCTRL2 */
	val = (p34x_xpcs_rd(pdata, 0x18032) >> 8) & 0x3;
	off += snprintf(buf + off, PAGE_SIZE - off, "TX_WIDTH_0:                    %x\n", val);

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_16G_RX_GENCTRL2 */
	val = (p34x_xpcs_rd(pdata, 0x18052) >> 8) & 0x3;
	off += snprintf(buf + off, PAGE_SIZE - off, "RX_WIDTH_0:                    %x\n", val);

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_AFE_DFE_EN_CTRL */
	afe_dfe_ctrl = p34x_xpcs_rd(pdata, 0x1805d);

	val = afe_dfe_ctrl & BIT(0);
	off += snprintf(buf + off, PAGE_SIZE - off, "AFE_EN_0:                      %x\n", val);

	val = (afe_dfe_ctrl >> 4) & BIT(0);
	off += snprintf(buf + off, PAGE_SIZE - off, "DFE_EN_0:                      %x\n", val);

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_16G_25G_RX_EQ_CTRL4 */
	val = p34x_xpcs_rd(pdata, 0x1805c) & BIT(0);
	off += snprintf(buf + off, PAGE_SIZE - off, "CONT_ADAPT_0:                  %x\n", val);

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_16G_MPLLA_CTRL2 */
	mplla_ctrl2 = p34x_xpcs_rd(pdata, 0x18073);

	val = mplla_ctrl2 & BIT(10);
	off += snprintf(buf + off, PAGE_SIZE - off, "MPLLA_DIV16P5_CLK_EN:          %x\n", val);

	val = mplla_ctrl2 & BIT(9);
	off += snprintf(buf + off, PAGE_SIZE - off, "MPLLA_DIV10_CLK_EN:            %x\n", val);

	val = mplla_ctrl2 & BIT(8);
	off += snprintf(buf + off, PAGE_SIZE - off, "MPLLA_DIV8_CLK_EN:             %x\n", val);

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_16G_25G_TX_EQ_CTRL0 */
	tx_eq0 = p34x_xpcs_rd(pdata, 0x18036);

	val = (tx_eq0 >> 8) & 0x3f;
	off += snprintf(buf + off, PAGE_SIZE - off, "TX_EQ_MAIN:                    %x\n", val);

	val = tx_eq0 & 0x3f;
	off += snprintf(buf + off, PAGE_SIZE - off, "TX_EQ_PRE:                     %x\n", val);

	/* XPCS_VR_XS_PMA_SNPS_MP_12G_16G_25G_TX_EQ_CTRL1 */
	tx_eq1 = p34x_xpcs_rd(pdata, 0x18037);

	val = tx_eq1 & 0x3f;
	off += snprintf(buf + off, PAGE_SIZE - off, "TX_EQ_POST:                    %x\n", val);

	val = tx_eq1 & BIT(6);
	off += snprintf(buf + off, PAGE_SIZE - off, "TX_EQ_OVR_RIDE:                %x\n", val);

	return off;
}
static DEVICE_ATTR_RO(table);

static const char *xpcs_status_strings[] = {
	"Wait for Ack High 0",
	"Wait for Ack Low 0",
	"Wait for Ack High 1",
	"Wait for Ack Low 1",
	"Tx/Rx Stable (Power_Good state)",
	"Power Save state",
	"Power Down state",
};

static ssize_t status_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct p34x_priv_data *pdata;
	int off = 0;
	u32 val = 0;

	pdata = dev_get_drvdata(dev);

	off = snprintf(buf + off, PAGE_SIZE - off, "\nXPCS: RX/TX Stable Status\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	/* XPCS_VR_XS_PCS_DIG_STS */
	val = (p34x_xpcs_rd(pdata, 0x38010) >> 2) & 0x07;

	if (val < ARRAY_SIZE(xpcs_status_strings))
		off += snprintf(buf + off, PAGE_SIZE - off, "\tStatus:		      %s\n",
			       xpcs_status_strings[val]);
	else
		off += snprintf(buf + off, PAGE_SIZE - off, "\tStatus:		      %s\n",
			       "Unknown/Invalid Status");

	return off;
}
static DEVICE_ATTR_RO(status);

static ssize_t reg_show(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	struct p34x_priv_data *pdata;
	int off = 0;

	pdata = dev_get_drvdata(dev);

	off += snprintf(buf + off, PAGE_SIZE - off, "Usage:\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    XPCS   read:  echo xpcs <addr> > reg\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    XPCS   write: echo xpcs <addr> <value> > reg\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    SMDIO  read:  echo smdio <addr> > reg\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    SMDIO  write: echo smdio <addr> <value> > reg\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    MDIO   read:  echo mdio <phy idx> <addr> > reg\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    MDIO   write: echo mdio <phy idx> <addr> <value> > reg\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    MMD    read:  echo mmd  <phy idx> <dev type> <addr> > reg\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    MMD    write: echo mmd  <phy idx> <dev type> <addr> <value> > reg\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    MBOX16 read:  echo mb16 <phy idx> <addr> > reg\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    MBOX16 write: echo mb16 <phy idx> <addr> <value> > reg\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    MBOX32 read:  echo mb32 <phy idx> <addr> > reg\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    MBOX32 write: echo mb32 <phy idx> <addr> <value> > reg\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    PM     read:  echo pm   <phy idx> <addr> > reg\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "  notes: all parameters are HEX format\n");

	return off;
}

static ssize_t reg_store(struct device * dev, struct device_attribute * attr,
			 const char * buf, size_t count)
{
	struct p34x_priv_data *pdata = dev_get_drvdata(dev);
	const char *modes[] = {"xpcs", "smdio", "mdio", "mmd", "mb16", "mb32", "pm"};
	u32 param[4] = {0};
	size_t mode;
	size_t i, j = 0;
	char c;
	u32 val;
	u32 addr;

	if (count < 2) {
		pr_err("Not enough inputs. Use command \"cat reg\" to get help.\n");
		return count;
	}

	for (mode = 0; mode < ARRAY_SIZE(modes); mode++) {
		j = strnlen(modes[mode], count - 1);

		if (strncasecmp(buf, modes[mode], j) == 0 && buf[j] <= ' ')
			break;
	}
	if (mode == ARRAY_SIZE(modes)) {
		pr_err("Mode not found. Use command \"cat reg\" to get help.\n");
		return count;
	}

	for (i = 0; i < ARRAY_SIZE(param); i++) {
		for (; j < count && buf[j] <= ' '; j++);
		if (j == count)
			break;
		for (; j < count; j++) {
			c = buf[j];
			if (c >= '0' && c <= '9')
				param[i] = param[i] * 16 + c - '0';
			else if (c >= 'A' && c <= 'F')
				param[i] = param[i] * 16 + c - 'A' + 10;
			else if (c >= 'a' && c <= 'f')
				param[i] = param[i] * 16 + c - 'a' + 10;
			else if (c != 'x' && c != 'X') {
				break;
			}
		}
	}

	switch (mode) {
	case 0:
		if (i == 0)
			pr_err("Not enough parameters. "
				"Use command \"cat reg\" to get help.\n");
		else if (i == 1) {
			val = p34x_xpcs_rd(pdata, param[0]);
			pr_err("XPCS: read off %06x val %08x\n",
				param[0], val);
		} else {
			pr_err("XPCS: write off %06x val %08x\n",
				param[0], param[1]);
			p34x_xpcs_wr(pdata, param[0], param[1]);
			val = p34x_xpcs_rd(pdata, param[0]);
			pr_err("XPCS: read off %06x val %08x\n",
				param[0], val);
		}
		break;
	case 1:
		if (i == 0) {
			pr_err("Not enough parameters. "
				"Use command \"cat reg\" to get help.\n");
			break;
		}
		addr = param[0];
		if (addr & 0xFFFF0000) {
			addr = AHB_TO_PDI(param[0]);
			if (i == 1) {
				val = smdio_read(pdata, addr);
				pr_err("SMDIO: read off %06x (%04x) val %04x\n",
					param[0], addr, val);
			} else {
				pr_err("SMDIO: write off %06x (%04x) val %04x\n",
					param[0], addr, param[1]);
				smdio_write(pdata, addr, param[1]);
				val = smdio_read(pdata, addr);
				pr_err("SMDIO: read off %06x (%04x) val %08x\n",
					param[0], addr, val);
			}
		} else {
			if (i == 1) {
				val = smdio_read(pdata, param[0]);
				pr_err("SMDIO: read off %08x val %08x\n",
					param[0], val);
			} else {
				pr_err("SMDIO: write off %08x val %08x\n",
					param[0], param[1]);
				smdio_write(pdata, param[0], param[1]);
				val = smdio_read(pdata, param[0]);
				pr_err("SMDIO: read off %08x val %08x\n",
					param[0], val);
			}
		}
		break;
	case 2:
		if (i < 2)
			pr_err("Not enough parameters. "
				"Use command \"cat reg\" to get help.\n");
		else if (i == 2) {
			val = mdio_read(pdata, param[0], param[1]);
			pr_err("MDIO: read PHY %u off %08x val %08x\n",
				param[0], param[1], val);
		} else {
			pr_err("MDIO: write PHY %u off %08x val %08x\n",
				param[0], param[1], param[2]);
			mdio_write(pdata, param[0], param[1], param[2]);
			val = mdio_read(pdata, param[0], param[1]);
			pr_err("MDIO: read PHY %u off %08x val %08x\n",
				param[0], param[1], val);
		}
		break;
	case 3:
		if (i < 3)
			pr_err("Not enough parameters. "
				"Use command \"cat reg\" to get help.\n");
		else if (i == 3) {
			val = mmd_read(pdata, param[0], param[1], param[2]);
			pr_err("MMD: read PHY %u MMD %02x off %08x val %08x\n",
				param[0], param[1], param[2], val);
		} else {
			pr_err("MMD: write PHY %u MMD %02x off %08x val %08x\n",
				param[0], param[1], param[2], param[3]);
			mmd_write(pdata, param[0], param[1], param[2], param[3]);
			val = mmd_read(pdata, param[0], param[1], param[2]);
			pr_err("MMD: read PHY %u MMD %02x off %08x val %08x\n",
				param[0], param[1], param[2], val);
		}
		break;
	case 4:
		if (i < 2)
			pr_err("Not enough parameters. "
				"Use command \"cat reg\" to get help.\n");
		else if (i == 2) {
			val = mbox16_read(pdata, param[0], param[1]);
			pr_err("Mbox16: read PHY %u AHB addr %06x val %04x\n",
				param[0], param[1], val);
		} else {
			pr_err("Mbox16: write PHY %u AHB addr %06x val %04x\n",
				param[0], param[1], param[2]);
			mbox16_write(pdata, param[0], param[1], param[2]);
			val = mbox16_read(pdata, param[0], param[1]);
			pr_err("Mbox16: read PHY %u AHB addr %06x val %04x\n",
				param[0], param[1], val);
		}
		break;
	case 5:
		if (i < 2)
			pr_err("Not enough parameters. "
				"Use command \"cat reg\" to get help.\n");
		else if (i == 2) {
			val = mbox32_read(pdata, param[0], param[1]);
			pr_err("Mbox32: read PHY %u AHB addr %06x val %08x\n",
				param[0], param[1], val);
		} else {
			pr_err("Mbox32: write PHY %u AHB addr %06x val %08x\n",
				param[0], param[1], param[2]);
			mbox32_write(pdata, param[0], param[1], param[2]);
			val = mbox32_read(pdata, param[0], param[1]);
			pr_err("Mbox32: read PHY %u AHB addr %06x val %08x\n",
				param[0], param[1], val);
		}
		break;
	case 6:
		if (i < 2)
			pr_err("Not enough parameters. "
				"Use command \"cat reg\" to get help.\n");
		else {
			val = pm_read(pdata, param[0], param[1]);
			pr_err("PM: read PHY %u AHB addr %06x val %08x\n",
				param[0], param[1], val);
		}
		break;
	default:
		pr_err("Mode is not supported yet. Use command \"cat reg\" to get help.\n");
	}

	return count;
}
static DEVICE_ATTR_RW(reg);

static ssize_t phy_control_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	struct p34x_priv_data *pdata = dev_get_drvdata(dev);
	u32 val;
	int off = 0, i;

	off += snprintf(buf + off, PAGE_SIZE - off, "Usage:\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    PHY Power    on/off:  echo phy_power <phy idx> <1/0> > phy_control\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    PHY LP mode  on/off:  echo phy_lpmode <phy idx> <1/0> > phy_control\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    Example: Power-on phyidx 3 (idx:0-3)\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "             echo phy_power 3 1 > phy_control\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "    Example: Low Power Mode-on phyidx 3 (idx:0-3)\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "				echo phy_lpmode 3 1 > phy_control\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "\n\n");

	off += snprintf(buf + off, PAGE_SIZE - off, "Status:\n");
	/* read power down status */
	for (i = 0; i < 4; i++) {
		val = mdio_read(pdata, i, 0x0); /* read pd flag from gphy_i */
		if (val & 0x800)
			off += snprintf(buf + off, PAGE_SIZE - off, "    PowerDown: gphy%d powerdown\n", i);
		else
			off += snprintf(buf + off, PAGE_SIZE - off, "    PowerDown: gphy%d operational\n", i);
	}
	/* read low power status */
	off += snprintf(buf + off, PAGE_SIZE - off, "    LowPower:  gphy0 not-supported\n");
	for (i = 1; i < 4; i++) {
		val = mdio_read(pdata, i, 0x14); /* read lp flag from gphy_i */
		if ((val & 0xFFFF) == 0xFFFF) {
			off += snprintf(buf + off, PAGE_SIZE - off, "    LowPower:  gphy%d active\n", i);
			continue;
		}
		if (val & 0x8)
			off += snprintf(buf + off, PAGE_SIZE - off, "    LowPower:  gphy%d enabled\n", i);
		else
			off += snprintf(buf + off, PAGE_SIZE - off, "    LowPower:  gphy%d disabled\n", i);
	}
	return off;
}

static ssize_t phy_control_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
{
	struct p34x_priv_data *pdata = dev_get_drvdata(dev);
	const char *modes[] = {"phy_power", "phy_lpmode"};
	u32 param[4] = {0};
	size_t mode;
	size_t i, j = 0;
	char c;
	u32 val;

	if (count < 2) {
		pr_err("Not enough inputs. Use command \"cat phy_control\" to get help.\n");
		return count;
	}

	for (mode = 0; mode < ARRAY_SIZE(modes); mode++) {
		j = strnlen(modes[mode], count - 1);

		if (strncasecmp(buf, modes[mode], j) == 0 && buf[j] <= ' ')
			break;
	}
	if (mode == ARRAY_SIZE(modes)) {
		pr_err("Mode not found. Use command \"cat phy_control\" to get help.\n");
		return count;
	}

	for (i = 0; i < ARRAY_SIZE(param); i++) {
		for (; j < count && buf[j] <= ' '; j++);
		if (j == count)
			break;
		for (; j < count; j++) {
			c = buf[j];
			if (c >= '0' && c <= '9')
				param[i] = param[i] * 16 + c - '0';
			else if (c >= 'A' && c <= 'F')
				param[i] = param[i] * 16 + c - 'A' + 10;
			else if (c >= 'a' && c <= 'f')
				param[i] = param[i] * 16 + c - 'a' + 10;
			else if (c != 'x' && c != 'X')
				break;
		}
	}

	switch (mode) {
	case 0:
		if (i == 2) {
			if (param[1] == 1) {
				val = mdio_read(pdata, param[0], 0x0);
				val &= 0xF7FF;
				mdio_write(pdata, param[0], 0x0, val);
			} else if (param[1] == 0) {
				val = mdio_read(pdata, param[0], 0x0);
				val |= 0x0800;
				mdio_write(pdata, param[0], 0x0, val);
			}
		} else {
			pr_err("Too many parameters. "
				"Use command \"cat phy_control\" to get help.\n");
		}
		break;
	case 1:
		if (i == 2) {
			if (param[1] == 1) {
				val = mdio_read(pdata, param[0], 0x14);
				val |= 0x8;
				mdio_write(pdata, param[0], 0x14, val);
			} else if (param[1] == 0) {
				val = mdio_read(pdata, param[0], 0x14);
				val &= 0xFFF7;
				mdio_write(pdata, param[0], 0x14, val);
			}
		} else {
			pr_err("Too many parameters. "
				"Use command \"cat phy_control\" to get help.\n");
		}
		break;
	default:
		pr_err("Mode is not supported yet. Use command \"cat phy_control\" to get help.\n");
	}

	return count;
}
static DEVICE_ATTR_RW(phy_control);

static ssize_t usxgmii_show(struct device *dev, struct device_attribute *attr,
			    char *buf)
{
	struct p34x_priv_data *pdata = dev_get_drvdata(dev);
	int off = 0;

	off += snprintf(buf + off, PAGE_SIZE - off, "### READ CONFIGURATION_USXGMII ####\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "6.1   SR_PMA_CTRL:             %04x\n", p34x_xpcs_rd(pdata, 0x10000));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.55  RX LINK UP:              %04x\n", p34x_xpcs_rd(pdata, 0x30001));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.61  SR_XS_PCS_CTRL2 after:   %04x\n", p34x_xpcs_rd(pdata, 0x30007));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.98  VR_XS_PCS_KR_CTRL:       %04x\n", p34x_xpcs_rd(pdata, 0x38007));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.96  VR_XS_PCS_DEBUG_CTRL:    %04x\n", p34x_xpcs_rd(pdata, 0x38005));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.101 Aligment Timer:          %04x\n", p34x_xpcs_rd(pdata, 0x3800a));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.255 SR_AN_CTRL:              %04x\n", p34x_xpcs_rd(pdata, 0x70000));
	off += snprintf(buf + off, PAGE_SIZE - off, "---------------Cl37----------------\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "6.92  CL37_BP (bit 12):        %04x\n", p34x_xpcs_rd(pdata, 0x38000) & (u32)BIT(12));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.385 VR_MII_AN_CTRL:          %04x\n", p34x_xpcs_rd(pdata, 0x1f8001));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.386 ANEG Status Port 0:      %04x\n", p34x_xpcs_rd(pdata, 0x1f8002));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.295 VR_MII_1_AN_CTRL:        %04x\n", p34x_xpcs_rd(pdata, 0x1a8001));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.296 ANEG Status Port 1:      %04x\n", p34x_xpcs_rd(pdata, 0x1a8002));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.318 VR_MII_2_AN_CTRL:        %04x\n", p34x_xpcs_rd(pdata, 0x1b8001));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.319 ANEG Status Port 2:      %04x\n", p34x_xpcs_rd(pdata, 0x1b8002));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.341 VR_MII_3_AN_CTRL:        %04x\n", p34x_xpcs_rd(pdata, 0x1c8001));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.342 ANEG Status Port 3:      %04x\n", p34x_xpcs_rd(pdata, 0x1c8002));
	off += snprintf(buf + off, PAGE_SIZE - off, "-----------------------------------\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "6.92  VR_XS_PCS_DIG_CTRL1:     %04x\n", p34x_xpcs_rd(pdata, 0x38000));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.384 VR_MII_DIG_CTRL1:        %04x\n", p34x_xpcs_rd(pdata, 0x1f8000));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.103 VR_XS_PCS_DIG_STS:       %04x\n", p34x_xpcs_rd(pdata, 0x38010));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.294 VR_MII_1_DIG_CTRL1:      %04x\n", p34x_xpcs_rd(pdata, 0x1a8000));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.304 VR_MII_1_DIG_STS :       %04x\n", p34x_xpcs_rd(pdata, 0x1a8010));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.317 VR_MII_2_DIG_CTRL1:      %04x\n", p34x_xpcs_rd(pdata, 0x1b8000));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.327 VR_MII_2_DIG_STS :       %04x\n", p34x_xpcs_rd(pdata, 0x1b8010));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.340 VR_MII_3_DIG_CTRL1:      %04x\n", p34x_xpcs_rd(pdata, 0x1c8000));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.350 VR_MII_3_DIG_STS :       %04x\n", p34x_xpcs_rd(pdata, 0x1c8010));
	off += snprintf(buf + off, PAGE_SIZE - off, "-----------Linker Timer------------\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "6.392 Linker Timer:            %04x\n", p34x_xpcs_rd(pdata, 0x1f800a));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.302 VR_MII_1_LINK_TIMER_CTRL:%04x\n", p34x_xpcs_rd(pdata, 0x1a800a));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.325 VR_MII_2_LINK_TIMER_CTRL:%04x\n", p34x_xpcs_rd(pdata, 0x1b800a));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.348 VR_MII_3_LINK_TIMER_CTRL:%04x\n", p34x_xpcs_rd(pdata, 0x1c800a));
	off += snprintf(buf + off, PAGE_SIZE - off, "------------Slice Speed------------\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "6.367 SR_MII_CTRL:             %04x\n", p34x_xpcs_rd(pdata, 0x1f0000));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.286 SR_MII_1_CTRL:           %04x\n", p34x_xpcs_rd(pdata, 0x1a0000));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.309 SR_MII_2_CTRL:           %04x\n", p34x_xpcs_rd(pdata, 0x1b0000));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.332 SR_MII_3_CTRL:           %04x\n", p34x_xpcs_rd(pdata, 0x1c0000));
	off += snprintf(buf + off, PAGE_SIZE - off, "--------------EEE--------------\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "6.97  VR_XS_PCS_EEE_MCTRL0:    %04x\n", p34x_xpcs_rd(pdata, 0x38006));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.102 VR_XS_PCS_EEE_MCTRL1:    %04x\n", p34x_xpcs_rd(pdata, 0x3800b));
	off += snprintf(buf + off, PAGE_SIZE - off, "-----------SERDES Access-----------\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "6.251 VR_XS_PMA_SNPS_CR_CTRL:  %04x\n", p34x_xpcs_rd(pdata, 0x180a0));
	off += snprintf(buf + off, PAGE_SIZE - off, "-----------New Register------------\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "5.1.39  VR_PMA_DIG_CTRL1:          %04x\n", p34x_xpcs_rd(pdata, 0x18000));
	off += snprintf(buf + off, PAGE_SIZE - off, "5.9.132 VR_MII_MP_12G_16G_25G_SRAM:%04x\n", p34x_xpcs_rd(pdata, 0x1809b));
	off += snprintf(buf + off, PAGE_SIZE - off, "-----------Debug------------\n");
	off += snprintf(buf + off, PAGE_SIZE - off, "6.250 VR_XS_PMA_Gen5_10G_MISC_CTRL:%04x\n", p34x_xpcs_rd(pdata, 0x1809f));
	off += snprintf(buf + off, PAGE_SIZE - off, "6.488 VR_MII_Gen5_12G_16G_MISC_CTRL0: %04x\n", p34x_xpcs_rd(pdata, 0x18090));

	return off;
}
static DEVICE_ATTR_RO(usxgmii);

static ssize_t xgmac_rmon_show(struct device *dev, struct device_attribute *attr,
			       char *buf)
{
	struct p34x_priv_data *pdata = dev_get_drvdata(dev);
	int off = 0;
	int idx;
	const char *name[] = {
		"tx packet", "tx bytes",
		"tx good pkt", "tx good bytes",
		"tx pause",
		"rx packet", "rx bytes",
		"rx good bytes",
		"rx CRC error"
	};
	const int size[] = {
		2, 2,
		2, 2,
		2,
		2, 2,
		2,
		2
	};
	const u32 addr[] = {
		0xA3081C, 0xA30814,
		0xA3088C, 0xA30884,
		0xA30894,
		0xA30900, 0xA30908,
		0xA30910,
		0xA30928
	};
	ssize_t i;
	int j;
	int val;
	u64 qw;

	off += snprintf(buf + off, PAGE_SIZE - off, "\n\t\t\tSlice 0\t\tSlice 1\t\t Slice 2\t\t Slice 3\n");
	for (i = 0; i < ARRAY_SIZE(name); i++) {
		off += snprintf(buf + off, PAGE_SIZE - off, "%s:\t", name[i]);
		for (idx = 0; idx < 4; idx++) {
			qw = 0;
			for (j = 0; j < size[i]; j++) {
				//val = mbox32_read(pdata, idx, addr[i] + j * 4);
				val = pm_read(pdata, idx, addr[i] + j * 4);
				qw |= (u64)((val & 0xFFFF) << (j * 32));
			}
			off += snprintf(buf + off, PAGE_SIZE - off, "%16llu", qw);
		}
		off += snprintf(buf + off, PAGE_SIZE - off, "\n");
	}
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	return off;
}
static DEVICE_ATTR_RO(xgmac_rmon);

static ssize_t gmacf_rmon_show(struct device *dev, struct device_attribute *attr,
			       char *buf)
{
	struct p34x_priv_data *pdata = dev_get_drvdata(dev);
	int off = 0;
	int idx;
	const char *name[] = {
		"tx packet",
		"tx good pkt",
		"tx underflow",
		"tx oversize",
		"rx packet",
		"rx CRC error",
		"rx undersize",
		"rx oversize"
	};
	const u32 addr[] = {
		0xA32718,
		0xA32768,
		0xA32748,
		0xA32778,
		0xA32780,
		0xA32794,
		0xA327A4,
		0xA327A8
	};
	ssize_t i;
	u32 dw;

	off += snprintf(buf + off, PAGE_SIZE - off, "\n\t\t\tSlice 0\t\tSlice 1\t\t Slice 2\t\t Slice 3\n");
	for (i = 0; i < ARRAY_SIZE(name); i++) {
		off += snprintf(buf + off, PAGE_SIZE - off, "%s:\t", name[i]);
		for (idx = 0; idx < 4; idx++) {
			//dw = mbox32_read(pdata, idx, addr[i]);
			dw = pm_read(pdata, idx, addr[i]);
			off += snprintf(buf + off, PAGE_SIZE - off, "%16u", dw);
		}
		off += snprintf(buf + off, PAGE_SIZE - off, "\n");
	}
	off += snprintf(buf + off, PAGE_SIZE - off, "\n");

	return off;
}
static DEVICE_ATTR_RO(gmacf_rmon);

#if (IS_ENABLED(CONFIG_HWMON))
static ssize_t trend_tround_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	struct p34x_priv_data *pdata = dev_get_drvdata(dev);
	int ret;

	ret = sprintf(buf, "%d\n", pdata->t_trend.tround);

	return ret;
}

static ssize_t trend_tround_store(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf, size_t count)
{
	struct p34x_priv_data *pdata = dev_get_drvdata(dev);
	u32 t;
	int ret;

	if (count < 1) {
		pr_err("Not enough inputs");
		return -EINVAL;
	}

	ret = kstrtoint(buf, 0, &t);
	if (ret)
		return ret;

	spin_lock(&pdata->t_trend.lock);
	pdata->t_trend.tround = t;
	spin_unlock(&pdata->t_trend.lock);

	return count;
}
static DEVICE_ATTR_RW(trend_tround);

static ssize_t trend_dsize_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct p34x_priv_data *pdata = dev_get_drvdata(dev);
	int ret;

	ret = sprintf(buf, "%d\n", pdata->t_trend.ds_size);

	return ret;
}

static ssize_t trend_dsize_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
{
	struct p34x_priv_data *pdata = dev_get_drvdata(dev);
	int size, ret;

	if (count < 1) {
		pr_err("Not enough inputs");
		return -EINVAL;
	}

	ret = kstrtoint(buf, 0, &size);
	if (ret)
		return ret;

	if (size < 1)
		return -EINVAL;

	spin_lock(&pdata->t_trend.lock);
	pdata->t_trend.ds_size = size;
	spin_unlock(&pdata->t_trend.lock);

	return count;
}
static DEVICE_ATTR_RW(trend_dsize);
#endif

static struct attribute *g241_attrs[] = {
	&dev_attr_phy_control.attr,
	NULL,
};

ATTRIBUTE_GROUPS(g241);

int g241_sysfs_init(struct p34x_priv_data *priv)
{
	return devm_device_add_groups(&priv->pdev->dev, g241_groups);
}

static struct attribute *p34x_attrs[] = {
	&dev_attr_cl73_info.attr,
	&dev_attr_status.attr,
	&dev_attr_table.attr,
	&dev_attr_linksts.attr,
	&dev_attr_reg.attr,
	&dev_attr_phy_control.attr,
	&dev_attr_usxgmii.attr,
	&dev_attr_xgmac_rmon.attr,
	&dev_attr_gmacf_rmon.attr,
#if (IS_ENABLED(CONFIG_HWMON))
	&dev_attr_trend_dsize.attr,
	&dev_attr_trend_tround.attr,
#endif
	NULL,
};

ATTRIBUTE_GROUPS(p34x);

int p34x_sysfs_init(struct p34x_priv_data *priv)
{
	return devm_device_add_groups(&priv->pdev->dev, p34x_groups);
}
