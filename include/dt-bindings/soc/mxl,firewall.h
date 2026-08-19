/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2021-2022 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MXL_FIREWALL_H
#define _MXL_FIREWALL_H

#include <dt-bindings/soc/mxl,noc_fw_defs.h>
/**
 * SAI definition
 * This definition must be in sync with uboot configuration
 */
/* SAI without device in DT */

/* CPU and MEMAXI */
#define LGM_SAI_CPU             ALL_SAI_IA_VMM_VAL
#define LGM_SAI_MEMAXI          SAI_IA_MEMAXI_VAL
/* DMA */
#define LGM_SAI_DMATX           SAI_IA_DMAT_VAL
#define LGM_SAI_DMARX           SAI_IA_DMAR_VAL
#define LGM_SAI_DMA3            SAI_IA_DMA3_VAL
#define LGM_SAI_DMA0            SAI_IA_DMA0_VAL
#define LGM_SAI_TOEDMA          SAI_IA_TOET_VAL
/* PCIe */
#define LGM_SAI_PCIE10          SAI_IA_PCI_2_VAL
#define LGM_SAI_PCIE11          SAI_IA_PCI_3_VAL
#define LGM_SAI_PCIE20          SAI_IA_PCI_6_VAL
#define LGM_SAI_PCIE21          SAI_IA_PCI_7_VAL
#define LGM_SAI_PCIE30          SAI_IA_PCI_0_VAL
#define LGM_SAI_PCIE31          SAI_IA_PCI_1_VAL
#define LGM_SAI_PCIE40          SAI_IA_PCI_4_VAL
#define LGM_SAI_PCIE41          SAI_IA_PCI_5_VAL
/* USB */
#define LGM_SAI_USB1            SAI_IA_USB_0_VAL
#define LGM_SAI_USB2            SAI_IA_USB_1_VAL
/* SATA */
#define LGM_SAI_SATA0           SAI_IA_SATA_0_VAL
#define LGM_SAI_SATA1           SAI_IA_SATA_1_VAL
#define LGM_SAI_SATA2           SAI_IA_SATA_2_VAL
#define LGM_SAI_SATA3           SAI_IA_SATA_3_VAL
/* Periperhal device */
#define LGM_SAI_V130            SAI_IA_V130_VAL
#define LGM_SAI_CQMDEQ          SAI_IA_DEQ_VAL
#define LGM_SAI_CQMENQ          SAI_IA_ENQ_VAL
#define LGM_SAI_PON             SAI_IA_PON_VAL
#define LGM_SAI_TOE             SAI_IA_TOE_VAL
#define LGM_SAI_MSIGEN          SAI_IA_SOCMSIGEN_VAL
#define LGM_SAI_VOICE           SAI_IA_VOICE_VAL
#define LGM_SAI_EIP197          SAI_IA_E197_VAL
#define LGM_SAI_SDXC            SAI_IA_SDXC_VAL
#define LGM_SAI_EMMC            SAI_IA_EMMC_VAL
#define LGM_SAI_PPV4            SAI_IA_PP_VAL
#define LGM_SAI_EPU             SAI_IA_EPU_VAL
#define LGM_SAI_VPN             SAI_IA_ARCVPN_VAL
/* SAI definition end here */

/* read/write permission for genpool DT */
#define NOC_RD_PERM             0x2
#define NOC_WR_PERM             0x4
#define NOC_RW_PERM             0x8

#define NOC_FW_PSEUDO_ADDR(addr) ((NOC_FW_SPECIAL_REGION << 32)  | (addr))

/* sepcial address definion */
#define NOC_DT_ADDR_SP_REGION		BIT(63)
#define NOC_DT_ADDR_SP_REGION_DYN	BIT(62)
#define NOC_DT_ADDR_SP_NON_COH		BIT(61)
#define NOC_DT_ADDR_SP_PLUS1		BIT(60)
#define NOC_DT_ADDR_SP_MINUS1		BIT(59)

/* extend SP regaion address for the DDR range larger than 3GB */
#define NOC_FW_DDR_UPPER_4GB_START	17
#define NOC_FW_DDR_UPPER_4GB_END	18
#define NOC_FW_SP_ADDR_MAX		(NOC_FW_DDR_UPPER_4GB_END + 1)
#
#endif  /*_MXL_FIREWALL_H*/

