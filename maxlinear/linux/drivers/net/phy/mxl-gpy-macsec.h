/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Driver for MACsec Engine inside Maxlinear GPY2XX PHYs
 * Copyright 2020 - 2024 MaxLinear, Inc.
 */

#ifndef _MXL_GPHY_MACSEC_H_
#define _MXL_GPHY_MACSEC_H_

#include <linux/skbuff.h>
#include <linux/module.h>
#include <net/genetlink.h>
#include <net/sock.h>
#include <net/gro_cells.h>
#include <uapi/linux/if_macsec.h>
#include <net/macsec.h>
#include <net/eip160.h>

#if IS_ENABLED(CONFIG_EIP160S_MACSEC)

#define MACSEC_MAX_SC			32
#define MACSEC_MAX_SA			32
#define MACSEC_KEY_LEN_128_BIT	16
#define MACSEC_KEY_LEN_256_BIT	32

/* The MISC_CONTROL register value after MACsec engine initialization, check
_macsec_cfg_ing_misc_flow_ctrl, _macsec_cfg_egr_misc_flow_ctrl for details */
#define ING_MISC_CTRL_INIT_VAL	0x01000A00
#define EGR_MISC_CTRL_INIT_VAL	0x02000A00

#define REG_FIELD_SET(reg, field) ((reg << field ## _POS) & field ## _MASK)
#define REG_FIELD_REPL(src, reg, field) (src = ((src & (~field ## _MASK)) | \
					 REG_FIELD_SET(reg, field)))

/* Erase/clear type of MACsec config */
enum macsec_clr_type {
	/* Clear HW configuration */
	MACSEC_CLEAR_HW = BIT(0),
	/* Clear SW configuration (busy bits, pointers) */
	MACSEC_CLEAR_SW = BIT(1),
	/* Cleat both HW and SW configuration */
	LGM_CLEAR_ALL = MACSEC_CLEAR_HW | MACSEC_CLEAR_SW,
};

/* Max SA (Secure Association) and SC (Secure Channel) mapping */
enum macsec_sc_sa_map {
	/* 4 SAs and 8 SCs */
	MACSEC_SA_SC_4SA_8SC,
	MACSEC_SA_SC_NOT_USED,
	/* 2 SAs and 16 SCs */
	MACSEC_SA_SC_2SA_16SC,
	/* 1 SAs amandd 32 SCs */
	MACSEC_SA_SC_1SA_32SC,
};

struct gpy2xx_macsec_txsc {
	u32 hw_sc_idx;
	unsigned long tx_sa_idx_busy;
	const struct macsec_secy *sw_secy;
	const struct macsec_tx_sc *sw_txsc;
	u8 tx_sa_key[MACSEC_NUM_AN][MACSEC_KEYID_LEN];
	struct macsec_tx_sc_stats tx_sc_stats;
	struct macsec_tx_sa_stats tx_sa_stats[MACSEC_NUM_AN];
};

struct gpy2xx_macsec_rxsc {
	u32 hw_sc_idx;
	unsigned long rx_sa_idx_busy;
	const struct macsec_secy *sw_secy;
	const struct macsec_rx_sc *sw_rxsc;
	u8 rx_sa_key[MACSEC_NUM_AN][MACSEC_KEYID_LEN];
	struct macsec_rx_sc_stats rx_sc_stats;
	struct macsec_rx_sa_stats rx_sa_stats[MACSEC_NUM_AN];
};

struct gpy2xx_macsec_cfg {
	bool initialized;
	u32 macsec_support;
	struct eip160_pdev dev;
	enum macsec_sc_sa_map sc_sa;
	/* Egress channel configuration */
	unsigned long txsc_idx_busy;
	struct gpy2xx_macsec_txsc tx_sc[MACSEC_MAX_SC];
	/* Ingress channel configuration */
	unsigned long rxsc_idx_busy;
	struct gpy2xx_macsec_rxsc rx_sc[MACSEC_MAX_SC];
	/* Statistics / counters */
	struct macsec_dev_stats glb_stats;
	u32 pdi_module_base;
	u32	gmac_module_base;
	int (*mbox_saz_rd)(struct phy_device *phydev, u32 ahbaddr, u16 *data);
	int (*mbox_saz_wr)(struct phy_device *phydev, u32 ahbaddr, u16 data);
};

extern const struct macsec_ops lgm_eth_macsec_ops;
int gpy24x_reg_macsec_ops(struct phy_device *phydev);

#endif
#endif

