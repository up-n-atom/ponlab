/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Driver for MACsec Engine inside Intel CHD GSWIP or GPHY
 * Copyright 2020 - 2022 Maxlinear, Inc.
 * Copyright 2019 - 2020 Intel Corporation
 */

#ifndef _LGM_MACSEC_DRIV_H_
#define _LGM_MACSEC_DRIV_H_

#include <linux/skbuff.h>
#include <linux/module.h>
#include <net/genetlink.h>
#include <net/sock.h>
#include <net/gro_cells.h>
#include <uapi/linux/if_macsec.h>
#include <net/macsec.h>
#include <linux/netdevice.h>

#if IS_ENABLED(CONFIG_EIP160S_MACSEC)

#define LGM_MACSEC_MAX_SC		32
#define LGM_MACSEC_MAX_SA		32
#define MACSEC_CONFIG_TX_ID		0
#define MACSEC_CONFIG_RX_ID		1
#define MACSEC_CONFIG_MAX_ID		2
#define LGM_MACSEC_KEY_LEN_128_BIT	16
#define LGM_MACSEC_KEY_LEN_256_BIT	32

/* The MISC_CONTROL register value after MACsec engine initialization, check 
_macsec_cfg_ing_misc_flow_ctrl, _macsec_cfg_egr_misc_flow_ctrl for details */
#define ING_MISC_CTRL_INIT_VAL	0x01000A00
#define EGR_MISC_CTRL_INIT_VAL	0x02000A00

enum lgm_clear_type {
	/* update HW configuration */
	LGM_CLEAR_HW = BIT(0),
	/* update SW configuration (busy bits, pointers) */
	LGM_CLEAR_SW = BIT(1),
	/* update both HW and SW configuration */
	LGM_CLEAR_ALL = LGM_CLEAR_HW | LGM_CLEAR_SW,
};

enum lgm_macsec_sc_sa {
	LGM_MACSEC_SA_SC_4SA_8SC,
	LGM_MACSEC_SA_SC_NOT_USED,
	LGM_MACSEC_SA_SC_2SA_16SC,
	LGM_MACSEC_SA_SC_1SA_32SC, //Not supported?
};

struct lgm_macsec_txsc {
	u32 hw_sc_idx;
	unsigned long tx_sa_idx_busy;
	const struct macsec_secy *sw_secy;
	const struct macsec_rx_sc *sw_rxsc;
	u8 tx_sa_key[MACSEC_NUM_AN][MACSEC_KEYID_LEN];
	struct macsec_tx_sc_stats tx_sc_stats;
	struct macsec_tx_sa_stats tx_sa_stats[MACSEC_NUM_AN];
};

struct lgm_macsec_rxsc {
	u32 hw_sc_idx;
	unsigned long rx_sa_idx_busy;
	const struct macsec_secy *sw_secy;
	const struct macsec_rx_sc *sw_rxsc;
	u8 rx_sa_key[MACSEC_NUM_AN][MACSEC_KEYID_LEN];
	struct macsec_rx_sc_stats rx_sc_stats;
	struct macsec_rx_sa_stats rx_sa_stats[MACSEC_NUM_AN];
};

struct lgm_macsec_cfg {
	bool initialized;
	struct eip160_pdev dev;
	enum lgm_macsec_sc_sa sc_sa;
	/* Egress channel configuration */
	unsigned long txsc_idx_busy;
	struct lgm_macsec_txsc lgm_txsc[LGM_MACSEC_MAX_SC];
	/* Ingress channel configuration */
	unsigned long rxsc_idx_busy;
	struct lgm_macsec_rxsc lgm_rxsc[LGM_MACSEC_MAX_SC];
	/* Statistics / counters */
	struct macsec_dev_stats glb_stats;
};

extern const struct macsec_ops lgm_eth_macsec_ops;

#endif
#endif
