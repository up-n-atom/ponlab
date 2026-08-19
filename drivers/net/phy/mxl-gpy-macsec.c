// SPDX-License-Identifier: GPL-2.0
/*
 * Driver for MACsec Engine inside Maxlinear GPY2XX PHYs
 * Copyright 2020 - 2024 Maxlinear, Inc.
 */

#include <linux/version.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/types.h>
#include <linux/phy.h>
#include <linux/mdio.h>
#include <uapi/linux/if_macsec.h>

#if IS_ENABLED(CONFIG_EIP160S_MACSEC)
#include "mxl-gpy.h"
#include "mxl-gpy-macsec.h"
#include "../../crypto/mxl/eip160s_struct.h"

#define GPY2XX_REG_RW		0
#define DEBUG_MSEC_PRINT	0

/*  Field MACSEC_CAP - MACSEC Capability in the product */
#define VSPEC1_SGMII_STAT_MACSEC_CAP_MASK 0x8000u

/*  Base Address of PM_PDI for P31G */
#define PM_PDI_MODULE_BASE 0x00D38000u
/*  Base Address of PM_PDI for P34X */
#define P34X_PM_PDI_MODULE_BASE 0x00A38000u

/*  Register Offset (relative) */
#define PM_PDI_CFG 0x0

/*  Field GMAC_BYP - GMAC Bypass */
#define PM_PDI_CFG_GMAC_BYP_POS 0
/*  Field GMAC_BYP - GMAC Bypass */
#define PM_PDI_CFG_GMAC_BYP_MASK 0x1u

/*  Field GMACL_BYP - GMAC-Lite Bypass */
#define PM_PDI_CFG_GMACL_BYP_POS 1
/*  Field GMACL_BYP - GMAC-Lite Bypass */
#define PM_PDI_CFG_GMACL_BYP_MASK 0x2u

/*  Field MACSECE_BYP - MACsec Egress Bypass - Chip Static Configuration */
#define PM_PDI_CFG_MACSECE_BYP_POS 2
/*  Field MACSECE_BYP - MACsec Egress Bypass - Chip Static Configuration */
#define PM_PDI_CFG_MACSECE_BYP_MASK 0x4u

/*  Field MACSECI_BYP - MACsec Ingress Bypass - Chip Static Configuration */
#define PM_PDI_CFG_MACSECI_BYP_POS 3
/*  Field MACSECI_BYP - MACsec Ingress Bypass - Chip Static Configuration */
#define PM_PDI_CFG_MACSECI_BYP_MASK 0x8u

/*  Field MACSEC_RST - MACsec Reset */
#define PM_PDI_CFG_MACSEC_RST_POS 4
/*  Field MACSEC_RST - MACsec Reset */
#define PM_PDI_CFG_MACSEC_RST_MASK 0x10u

/*  Field CRC_ERR - Drop packet on CRC error */
#define PM_PDI_CFG_CRC_ERR_POS 5
/*  Field CRC_ERR - Drop packet on CRC error */
#define PM_PDI_CFG_CRC_ERR_MASK 0x20u

/*  Field PKT_ERR - Drop packet on PKT error */
#define PM_PDI_CFG_PKT_ERR_POS 6
/*  Field PKT_ERR - Drop packet on PKT error */
#define PM_PDI_CFG_PKT_ERR_MASK 0x40u

/*  Field SEC_FAIL - Drop packet on Security Fail */
#define PM_PDI_CFG_SEC_FAIL_POS 7
/*  Field SEC_FAIL - Drop packet on Security Fail */
#define PM_PDI_CFG_SEC_FAIL_MASK 0x80u

/*  Field CLASS_DROP - Drop packet on Classification Drop */
#define PM_PDI_CFG_CLASS_DROP_POS 8
/*  Field CLASS_DROP - Drop packet on Classification Drop */
#define PM_PDI_CFG_CLASS_DROP_MASK 0x100u

/*  Register Offset (relative) */
#define PM_PDI_GMAC_CFG 0x4

/*  Field GMAC_CRC_PAD - GMAC CRC and Pad Control */
#define PM_PDI_GMAC_CFG_GMAC_CRC_PAD_POS 0
/*  Field GMAC_CRC_PAD - GMAC CRC and Pad Control */
#define PM_PDI_GMAC_CFG_GMAC_CRC_PAD_MASK 0x3u

/*  Base Address of GMACF_EQOS_MAC for P31G */
#define GMACF_EQOS_MAC_MODULE_BASE 0x00D30000u
/*  Base Address of GMACF_EQOS_MAC for P34X */
#define P34X_GMACF_EQOS_MAC_MODULE_BASE 0x00A32000u

/*  Base Address of CHIP ID CFG for P34X */
#define P34X_CHIP_ID_CFG_ADDR 0x00D288C0
/*  Base Address of CHIP ID CFG for P31G */
#define P31G_CHIP_ID_CFG_ADDR 0x00D28884

#define CHIP_ID_CFG_GPY215    0x9113
#define CHIP_ID_CFG_MXL86249  0x8001
#define CHIP_ID_CFG_MXL86249C 0x8401

/*  Register Offset (relative) */
#define GMACF_MAC_CONFIGURATION_H 0x4

/*  Field ACS - Automatic Pad or CRC Stripping */
#define GMACF_MAC_CONFIGURATION_H_ACS_POS 4
/*  Field ACS - Automatic Pad or CRC Stripping */
#define GMACF_MAC_CONFIGURATION_H_ACS_MASK 0x10u

/*  Field CST - CRC stripping for Type packets */
#define GMACF_MAC_CONFIGURATION_H_CST_POS 5
/*  Field CST - CRC stripping for Type packets */
#define GMACF_MAC_CONFIGURATION_H_CST_MASK 0x20u

/*  Field IPG - Inter-Packet Gap */
#define GMACF_MAC_CONFIGURATION_H_IPG_POS 8
/*  Field IPG - Inter-Packet Gap */
#define GMACF_MAC_CONFIGURATION_H_IPG_MASK 0x700u

int gpy24x_macsec_init(struct phy_device *phydev);
int gpy24x_macsec_uninit(struct phy_device *phydev);

static u32 gpy2xx_sc_idx_max(const enum macsec_sc_sa_map sc_sa)
{
	u32 result = 0;

	switch (sc_sa) {
	case MACSEC_SA_SC_4SA_8SC:
		result = 8;
		break;

	case MACSEC_SA_SC_2SA_16SC:
		result = 16;
		break;

	case MACSEC_SA_SC_1SA_32SC:
		result = 32;
		break;

	default:
		break;
	};

	return result;
}

static u32 gpy2xx_to_hw_sc_idx(const u32 sc_idx,
			       const enum macsec_sc_sa_map sc_sa)
{
	switch (sc_sa) {
	case MACSEC_SA_SC_4SA_8SC:
		return sc_idx << 2;

	case MACSEC_SA_SC_2SA_16SC:
		return sc_idx << 1;

	case MACSEC_SA_SC_1SA_32SC:
		return sc_idx;

	default:
		WARN_ONCE(true, "Invalid sc_sa");
	};

	return sc_idx;
}

static enum macsec_sc_sa_map gpy24x_sc_sa_from_num_an(const int num_an)
{
	enum macsec_sc_sa_map sc_sa = MACSEC_SA_SC_NOT_USED;

	switch (num_an) {
	case 4:
		sc_sa = MACSEC_SA_SC_4SA_8SC;
		break;

	case 2:
		sc_sa = MACSEC_SA_SC_2SA_16SC;
		break;

	case 1:
		sc_sa = MACSEC_SA_SC_1SA_32SC;
		break;

	default:
		break;
	}

	return sc_sa;
}

static int gpy2xx_get_rxsc_idx_from_rxsc(struct gpy2xx_macsec_cfg *macsec_cfg,
					 const struct macsec_rx_sc *rxsc)
{
	int i;

	if (unlikely(!rxsc))
		return -1;

	for (i = 0; i < MACSEC_MAX_SC; i++) {
		if (macsec_cfg->rx_sc[i].sw_rxsc == rxsc)
			return i;
	}

	return -1;
}

/* There's a 1:1 mapping between SecY and Tx SC */
static int gpy2xx_get_txsc_idx_from_secy(struct gpy2xx_macsec_cfg *macsec_cfg,
					 const struct macsec_secy *secy)
{
	int i;

	if (unlikely(!secy))
		return -1;

	for (i = 0; i < MACSEC_MAX_SC; i++) {
		if (macsec_cfg->tx_sc[i].sw_secy == secy)
			return i;
	}

	return -1;
}

static int gpy2xx_clear_rxsa(struct gpy2xx_macsec_cfg *phy_msec,
			     struct gpy2xx_macsec_rxsc *rx_sc, const int sa_num,
			     enum macsec_clr_type clear_type)
{
	int sa_idx = rx_sc->hw_sc_idx | sa_num;
	struct clr_trans_rec pkt_xform_parms = { 0 };
	int ret = 0;

	if (clear_type & MACSEC_CLEAR_SW)
		clear_bit(sa_num, &rx_sc->rx_sa_idx_busy);

	if (clear_type & MACSEC_CLEAR_HW) {
		pkt_xform_parms.sa_index = sa_idx;
		ret = eip160_clr_ing_sa_parms(&phy_msec->dev, &pkt_xform_parms);
		if (ret)
			return ret;
	}

	return ret;
}

static int gpy2xx_clear_rxsc(struct gpy2xx_macsec_cfg *phy_msec,
			     const int rxsc_idx, enum macsec_clr_type clear_type)
{
	struct gpy2xx_macsec_rxsc *rx_sc = &phy_msec->rx_sc[rxsc_idx];
	struct clr_sam_par pkt_clas_parms = { 0 };
	struct sam_enable_ctrl sam_ena_ctrl = { 0 };
	struct clr_sam_flow_ctrl pkt_match_actn = { 0 };

	int ret = 0;
	int sa_num;

	for_each_set_bit(sa_num, &rx_sc->rx_sa_idx_busy, MACSEC_MAX_SA) {
		ret = gpy2xx_clear_rxsa(phy_msec, rx_sc, sa_num, clear_type);
		if (ret)
			return ret;
	}

	if (clear_type & MACSEC_CLEAR_HW) {
		sam_ena_ctrl.clear_enable = 1;
		sam_ena_ctrl.sam_index_clear = rx_sc->hw_sc_idx;

		ret = eip160_cfg_ing_sam_ena_ctrl(&phy_msec->dev, &sam_ena_ctrl);
		if (ret)
			return ret;

		pkt_clas_parms.rule_index = rxsc_idx;
		ret = eip160_clr_ing_sam_rule(&phy_msec->dev, &pkt_clas_parms);
		if (ret)
			return ret;

		pkt_match_actn.flow_index = rx_sc->hw_sc_idx;
		ret = eip160_clr_ing_sam_flow_ctrl(&phy_msec->dev, &pkt_match_actn);
		if (ret)
			return ret;
	}

	if (clear_type & MACSEC_CLEAR_SW) {
		clear_bit(rxsc_idx, &phy_msec->rxsc_idx_busy);
		rx_sc->sw_secy = NULL;
		rx_sc->sw_rxsc = NULL;
	}

	return ret;
}

static int gpy2xx_clear_txsa(struct gpy2xx_macsec_cfg *phy_msec,
			     struct gpy2xx_macsec_txsc *tx_sc, const int sa_num,
			     enum macsec_clr_type clear_type)
{
	const int sa_idx = tx_sc->hw_sc_idx | sa_num;
	struct clr_trans_rec pkt_xform_parms = { 0 };
	int ret = 0;

	if (clear_type & MACSEC_CLEAR_SW)
		clear_bit(sa_num, &tx_sc->tx_sa_idx_busy);

	if (clear_type & MACSEC_CLEAR_HW) {
		pkt_xform_parms.sa_index = sa_idx;
		ret = eip160_clr_egr_sa_parms(&phy_msec->dev, &pkt_xform_parms);
		if (ret)
			return ret;
	}

	return 0;
}

static int gpy2xx_clear_txsc(struct gpy2xx_macsec_cfg *phy_msec,
			     const int txsc_idx, enum macsec_clr_type clear_type)
{
	struct gpy2xx_macsec_txsc *tx_sc = &phy_msec->tx_sc[txsc_idx];
	struct clr_sam_par pkt_clas_parms = { 0 };
	struct sam_enable_ctrl sam_ena_ctrl = { 0 };
	struct clr_sam_flow_ctrl pkt_match_actn = { 0 };

	int ret = 0;
	int sa_num;

	for_each_set_bit(sa_num, &tx_sc->tx_sa_idx_busy, MACSEC_MAX_SA) {
		ret = gpy2xx_clear_txsa(phy_msec, tx_sc, sa_num, clear_type);
		if (ret)
			return ret;
	}

	if (clear_type & MACSEC_CLEAR_HW) {
		sam_ena_ctrl.clear_enable = 1;
		sam_ena_ctrl.sam_index_clear = tx_sc->hw_sc_idx;

		ret = eip160_cfg_egr_sam_ena_ctrl(&phy_msec->dev, &sam_ena_ctrl);
		if (ret)
			return ret;

		pkt_clas_parms.rule_index = txsc_idx;
		ret = eip160_clr_egr_sam_rule(&phy_msec->dev, &pkt_clas_parms);
		if (ret)
			return ret;

		pkt_match_actn.flow_index = tx_sc->hw_sc_idx;
		ret = eip160_clr_egr_sam_flow_ctrl(&phy_msec->dev, &pkt_match_actn);
		if (ret)
			return ret;
	}

	if (clear_type & MACSEC_CLEAR_SW) {
		clear_bit(txsc_idx, &phy_msec->txsc_idx_busy);
		phy_msec->tx_sc[txsc_idx].sw_secy = NULL;
	}

	return ret;
}

static int gpy2xx_clear_secy_cfg(struct gpy2xx_macsec_cfg *phy_msec,
				 const struct macsec_secy *secy, enum macsec_clr_type clear_type)
{
	struct macsec_rx_sc *rx_sc;
	int txsc_idx;
	int rxsc_idx;
	int ret = 0;

	txsc_idx = gpy2xx_get_txsc_idx_from_secy(phy_msec, secy);
	if (txsc_idx >= 0) {
		ret = gpy2xx_clear_txsc(phy_msec, txsc_idx, clear_type);
		if (ret)
			return ret;
	}

	for (rx_sc = rcu_dereference_bh(secy->rx_sc); rx_sc;
	     rx_sc = rcu_dereference_bh(rx_sc->next)) {
		rxsc_idx = gpy2xx_get_rxsc_idx_from_rxsc(phy_msec, rx_sc);
		if (rxsc_idx < 0)
			continue;

		ret = gpy2xx_clear_rxsc(phy_msec, rxsc_idx, clear_type);
		if (ret)
			return ret;
	}

	return ret;
}

static int gpy2xx_update_rxsa(struct gpy2xx_macsec_cfg *phy_msec,
			      const unsigned int sc_idx, const struct macsec_secy *secy,
			      const struct macsec_rx_sa *rx_sa, const unsigned char *key,
			      const unsigned char an)
{
	const u64 next_pn = rx_sa->next_pn;
	struct transform_rec pkt_xform_parms = { 0 };
	const int sa_idx = sc_idx | an;
	int ret = 0;
#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	struct transform_rec _pkt_xform_parms = { 0 };
#endif
	memset(&pkt_xform_parms, 0, sizeof(pkt_xform_parms));

	if (!key)
		return ret;

	pkt_xform_parms.ccw.an = secy->tx_sc.encoding_sa;

	if (secy->xpn)
		pkt_xform_parms.ccw.sn_type = SN_64_BIT;
	else
		pkt_xform_parms.ccw.sn_type = SN_32_BIT;

	switch (secy->key_len) {
	case MACSEC_KEY_LEN_128_BIT:
		pkt_xform_parms.ccw.ca_type = CA_AES_CTR_128;
		break;

	case MACSEC_KEY_LEN_256_BIT:
		pkt_xform_parms.ccw.ca_type = CA_AES_CTR_256;
		break;

	default:
		WARN_ONCE(true, "Invalid sc_sa");
		return -EINVAL;
	}

	memcpy(pkt_xform_parms.cp.key, key, secy->key_len);
	memcpy((char *)&pkt_xform_parms.cp.scid, (char *)&secy->sci, MACSEC_SCI_LEN_BYTE);

	if (secy->xpn) {
		memcpy(pkt_xform_parms.cp.c_salt.salt, (char *)&rx_sa->key.salt,
		       MACSEC_SALT_LEN_BYTE);
		memcpy(pkt_xform_parms.cp.c_salt.s_sci, (char *)&rx_sa->ssci,
		       MACSEC_SSCI_LEN_BYTE);
	}

	pkt_xform_parms.pn_rc.seq_num = next_pn;
	pkt_xform_parms.sa_index = sa_idx;

	if (secy->replay_protect)
		pkt_xform_parms.pn_rc.mask = secy->replay_window;

	ret = eip160_cfg_ing_sa_parms(&phy_msec->dev, &pkt_xform_parms);
	if (ret)
		return ret;

#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	memset(&_pkt_xform_parms, 0, sizeof(_pkt_xform_parms));
	return eip160_get_ing_sa_parms(&phy_msec->dev, &_pkt_xform_parms);
#endif

	return ret;
}

static int gpy2xx_set_rxsc(struct gpy2xx_macsec_cfg *phy_msec,
			   const u32 rxsc_idx)
{
	const struct gpy2xx_macsec_rxsc *rx_sc = &phy_msec->rx_sc[rxsc_idx];
	const struct macsec_secy *secy = rx_sc->sw_secy;
	const u32 hw_sc_idx = rx_sc->hw_sc_idx;
	struct sam_flow_ctrl pkt_match_actn = { 0 };
	struct sa_match_parm pkt_clas_parms = { 0 };
	struct sam_enable_ctrl sam_ena_ctrl = { 0 };
	int ret = 0;
#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	struct sam_flow_ctrl _pkt_match_actn = { 0 };
	struct sa_match_parm _pkt_clas_parms = { 0 };
	struct sam_enable_flags _sam_ena_ctrl = { 0 };
#endif

	memset(&sam_ena_ctrl, 0, sizeof(sam_ena_ctrl));
	memset(&pkt_match_actn, 0, sizeof(pkt_match_actn));
	memset(&pkt_clas_parms, 0, sizeof(pkt_clas_parms));

	pkt_match_actn.flow_type = SAM_FCA_FLOW_INGRESS;
	pkt_match_actn.dest_port = SAM_FCA_DPT_CONTROL;
	pkt_match_actn.drop_non_reserved = 0;
	pkt_match_actn.flow_crypt_auth = 0;
	pkt_match_actn.drop_action = SAM_FCA_DROP_AS_CRC;
	pkt_match_actn.replay_protect = secy->replay_protect;
	pkt_match_actn.sa_in_use = 1;
	pkt_match_actn.validate_frames = secy->validate_frames;
	pkt_match_actn.sa_index_update = 0;

	pkt_match_actn.sa_index = hw_sc_idx;
	pkt_match_actn.flow_index = hw_sc_idx;

	ret = eip160_cfg_ing_sam_flow_ctrl(&phy_msec->dev, &pkt_match_actn);
	if (ret)
		return ret;

	/* SA match params */
	memcpy(pkt_clas_parms.sa_mac, (char *)&rx_sc->sw_rxsc->sci, ETH_ALEN);
	pkt_clas_parms.eth_type = ETH_P_MACSEC;
	memcpy((char *)&pkt_clas_parms.scid, (char *)&rx_sc->sw_rxsc->sci, MACSEC_SCI_LEN_BYTE);
	/* Misc match params */
	pkt_clas_parms.misc_par.control_packet = 1;
	pkt_clas_parms.misc_par.untagged = 0;
	pkt_clas_parms.misc_par.tagged = 1;
	pkt_clas_parms.misc_par.bad_tag = 0;
	pkt_clas_parms.misc_par.kay_tag = 1;
	pkt_clas_parms.misc_par.source_port = 0x0;
	pkt_clas_parms.misc_par.match_priority = 0x0;
	pkt_clas_parms.misc_par.macsec_tci_an = 0x00;
	/* Mask params */
	pkt_clas_parms.mask_par.mac_sa_mask = 0x3f;
	pkt_clas_parms.mask_par.mac_da_mask = 0x00;
	pkt_clas_parms.mask_par.mac_etype_mask = 0;
	pkt_clas_parms.mask_par.source_port_mask = 0;
	pkt_clas_parms.mask_par.ctrl_packet_mask = 0;
	pkt_clas_parms.mask_par.macsec_sci_mask = 0;
	pkt_clas_parms.mask_par.tci_an_mask = 0x00;
	/* Index params */
	pkt_clas_parms.rule_index = hw_sc_idx;
	pkt_clas_parms.flow_index = hw_sc_idx;

	ret = eip160_cfg_ing_sam_rule(&phy_msec->dev, &pkt_clas_parms);
	if (ret)
		return ret;

	sam_ena_ctrl.set_enable = 1;
	sam_ena_ctrl.sam_index_set = hw_sc_idx;

	ret = eip160_cfg_ing_sam_ena_ctrl(&phy_msec->dev, &sam_ena_ctrl);
	if (ret)
		return ret;

#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	memset(&_pkt_clas_parms, 0, sizeof(_pkt_clas_parms));
	memset(&_pkt_match_actn, 0, sizeof(_pkt_match_actn));
	memset(&_sam_ena_ctrl, 0, sizeof(_sam_ena_ctrl));

	ret = eip160_get_ing_sam_rule(&phy_msec->dev, &_pkt_clas_parms);
	if (ret)
		return ret;

	ret = eip160_get_ing_sam_flow_ctrl(&phy_msec->dev, &_pkt_match_actn);
	if (ret)
		return ret;

	return eip160_get_ing_sam_ena_flags(&phy_msec->dev, &_sam_ena_ctrl);
#endif

	return ret;
}

static int gpy2xx_update_txsa(struct gpy2xx_macsec_cfg *phy_msec,
			      const unsigned int sc_idx, const struct macsec_secy *secy,
			      const struct macsec_tx_sa *tx_sa, const unsigned char *key,
			      const unsigned char an)
{
	const u64 next_pn = tx_sa->next_pn;
	struct transform_rec pkt_xform_parms = { 0 };
	const unsigned int sa_idx = sc_idx | an;
	int ret = 0;
#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	struct transform_rec _pkt_xform_parms;
#endif

	memset(&pkt_xform_parms, 0, sizeof(pkt_xform_parms));

	if (!key)
		return ret;

	pkt_xform_parms.ccw.an = secy->tx_sc.encoding_sa;

	if (secy->xpn)
		pkt_xform_parms.ccw.sn_type = SN_64_BIT;
	else
		pkt_xform_parms.ccw.sn_type = SN_32_BIT;

	switch (secy->key_len) {
	case MACSEC_KEY_LEN_128_BIT:
		pkt_xform_parms.ccw.ca_type = CA_AES_CTR_128;
		break;

	case MACSEC_KEY_LEN_256_BIT:
		pkt_xform_parms.ccw.ca_type = CA_AES_CTR_256;
		break;

	default:
		WARN_ONCE(true, "Invalid sc_sa");
		return -EINVAL;
	}

	memcpy(pkt_xform_parms.cp.key, key, secy->key_len);
	memcpy((char *)&pkt_xform_parms.cp.scid, (char *)&secy->sci, MACSEC_SCI_LEN_BYTE);

	if (secy->xpn) {
		memcpy(pkt_xform_parms.cp.c_salt.salt, (char *)&tx_sa->key.salt,
		       MACSEC_SALT_LEN_BYTE);
		memcpy(pkt_xform_parms.cp.c_salt.s_sci, (char *)&tx_sa->ssci,
		       MACSEC_SSCI_LEN_BYTE);
	}

	pkt_xform_parms.pn_rc.seq_num = next_pn;
	pkt_xform_parms.sa_index = sa_idx;
	pkt_xform_parms.sa_cw.sa_index = sa_idx + 1;
	pkt_xform_parms.sa_cw.sa_expired_irq = 1;
	pkt_xform_parms.sa_cw.sa_index_valid = tx_sa->active;
	pkt_xform_parms.sa_cw.sa_ind_update_en = tx_sa->active;
	pkt_xform_parms.sa_cw.flow_index = sa_idx;

	pkt_xform_parms.sa_index = sa_idx;

	ret = eip160_cfg_egr_sa_parms(&phy_msec->dev, &pkt_xform_parms);
	if (ret)
		return ret;

#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	memset(&_pkt_xform_parms, 0, sizeof(_pkt_xform_parms));
	return eip160_get_egr_sa_parms(&phy_msec->dev, &_pkt_xform_parms);
#endif

	return ret;
}

static int gpy2xx_set_txsc(struct gpy2xx_macsec_cfg *phy_msec, const int txsc_idx)
{
	struct gpy2xx_macsec_txsc *tx_sc = &phy_msec->tx_sc[txsc_idx];
	const struct macsec_secy *secy = tx_sc->sw_secy;
	unsigned int sc_idx = tx_sc->hw_sc_idx;
	struct sam_flow_ctrl pkt_match_actn = { 0 };
	struct sa_match_parm pkt_clas_parms = { 0 };
	struct sam_enable_ctrl sam_ena_ctrl = { 0 };
	int ret = 0;
#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	struct sam_flow_ctrl _pkt_match_actn = { 0 };
	struct sa_match_parm _pkt_clas_parms = { 0 };
	struct sam_enable_flags _sam_ena_ctrl = { 0 };
#endif

	memset(&sam_ena_ctrl, 0, sizeof(sam_ena_ctrl));
	memset(&pkt_match_actn, 0, sizeof(pkt_match_actn));
	memset(&pkt_clas_parms, 0, sizeof(pkt_clas_parms));

	pkt_match_actn.flow_type = SAM_FCA_FLOW_EGRESS;
	pkt_match_actn.dest_port = SAM_FCA_DPT_COMMON;
	pkt_match_actn.drop_non_reserved = 0;
	pkt_match_actn.flow_crypt_auth = 0;
	pkt_match_actn.drop_action = SAM_FCA_DROP_AS_CRC;
	pkt_match_actn.protect_frames = secy->protect_frames;
	pkt_match_actn.sa_in_use = 1;
	pkt_match_actn.sa_index_update = 0;

	if (secy->tx_sc.send_sci)
		pkt_match_actn.include_sci = 1;
	if (secy->tx_sc.encrypt)
		pkt_match_actn.encrypt_frames = 1;
	if (secy->tx_sc.scb)
		pkt_match_actn.use_scb = 1;
	if (secy->tx_sc.end_station)
		pkt_match_actn.use_es = 1;

	pkt_match_actn.sa_index = sc_idx;
	pkt_match_actn.flow_index = sc_idx;

	ret = eip160_cfg_egr_sam_flow_ctrl(&phy_msec->dev, &pkt_match_actn);
	if (ret)
		return ret;

	/* SA match params */
	memcpy(pkt_clas_parms.sa_mac, secy->netdev->dev_addr, ETH_ALEN);
	pkt_clas_parms.eth_type = 0x0000;
	pkt_clas_parms.scid = 0;
	/* Misc match params */
	pkt_clas_parms.misc_par.control_packet = 0;
	pkt_clas_parms.misc_par.untagged = 1;
	pkt_clas_parms.misc_par.tagged = 0;
	pkt_clas_parms.misc_par.bad_tag = 0;
	pkt_clas_parms.misc_par.kay_tag = 0;
	pkt_clas_parms.misc_par.source_port = 0x0;
	pkt_clas_parms.misc_par.match_priority = 0x0;
	pkt_clas_parms.misc_par.macsec_tci_an = 0x00;
	/* Mask params */
	pkt_clas_parms.mask_par.mac_sa_mask = 0x3f;
	pkt_clas_parms.mask_par.mac_da_mask = 0x00;
	pkt_clas_parms.mask_par.mac_etype_mask = 0;
	pkt_clas_parms.mask_par.source_port_mask = 0;
	pkt_clas_parms.mask_par.ctrl_packet_mask = 0;
	pkt_clas_parms.mask_par.macsec_sci_mask = 0;
	pkt_clas_parms.mask_par.tci_an_mask = 0x00;
	/* Index params */
	pkt_clas_parms.flow_index = sc_idx;
	pkt_clas_parms.rule_index = sc_idx;

	ret = eip160_cfg_egr_sam_rule(&phy_msec->dev, &pkt_clas_parms);
	if (ret)
		return ret;

	sam_ena_ctrl.set_enable = 1;
	sam_ena_ctrl.sam_index_set = sc_idx;

	ret = eip160_cfg_egr_sam_ena_ctrl(&phy_msec->dev, &sam_ena_ctrl);
	if (ret)
		return ret;

#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	memset(&_pkt_clas_parms, 0, sizeof(_pkt_clas_parms));
	memset(&_pkt_match_actn, 0, sizeof(_pkt_match_actn));
	memset(&_sam_ena_ctrl, 0, sizeof(_sam_ena_ctrl));

	ret = eip160_get_egr_sam_rule(&phy_msec->dev, &_pkt_clas_parms);
	if (ret)
		return ret;

	ret = eip160_get_egr_sam_flow_ctrl(&phy_msec->dev, &_pkt_match_actn);
	if (ret)
		return ret;

	return eip160_get_egr_sam_ena_flags(&phy_msec->dev, &_sam_ena_ctrl);
#endif

	return ret;
}

static int gpy2xx_apply_rxsc_cfg(struct gpy2xx_macsec_cfg *phy_msec,
				 const int rxsc_idx)
{
	struct gpy2xx_macsec_rxsc *rx_sc = &phy_msec->rx_sc[rxsc_idx];
	const struct macsec_secy *secy = rx_sc->sw_secy;
	struct macsec_rx_sa *rx_sa;
	int ret = 0;
	int i;

	if (!netif_running(secy->netdev))
		return ret;

	for (i = 0; i < MACSEC_NUM_AN; i++) {
		rx_sa = rcu_dereference_bh(rx_sc->sw_rxsc->sa[i]);

		if (rx_sa) {
			ret = gpy2xx_update_rxsa(phy_msec, rx_sc->hw_sc_idx, secy,
						 rx_sa, rx_sc->rx_sa_key[i], i);
			if (ret)
				return ret;
		}
	}

	return gpy2xx_set_rxsc(phy_msec, rxsc_idx);
}

static int gpy2xx_apply_txsc_cfg(struct gpy2xx_macsec_cfg *phy_msec,
				 const int txsc_idx)
{
	struct gpy2xx_macsec_txsc *tx_sc = &phy_msec->tx_sc[txsc_idx];
	const struct macsec_secy *secy = tx_sc->sw_secy;
	struct macsec_tx_sa *tx_sa;
	int ret = 0;
	int i;

	if (!netif_running(secy->netdev))
		return ret;

	for (i = 0; i < MACSEC_NUM_AN; i++) {
		tx_sa = rcu_dereference_bh(tx_sc->sw_txsc->sa[i]);

		if (tx_sa) {
			ret = gpy2xx_update_txsa(phy_msec, tx_sc->hw_sc_idx, secy,
						 tx_sa, tx_sc->tx_sa_key[i], i);
			if (ret)
				return ret;
		}
	}

	return gpy2xx_set_txsc(phy_msec, txsc_idx);
}

static int gpy2xx_apply_secy_cfg(struct gpy2xx_macsec_cfg *phy_msec,
				 const struct macsec_secy *secy)
{
	struct macsec_rx_sc *rx_sc;
	int rxsc_idx, txsc_idx, ret = 0;

	txsc_idx = gpy2xx_get_txsc_idx_from_secy(phy_msec, secy);
	if (txsc_idx >= 0)
		gpy2xx_apply_txsc_cfg(phy_msec, txsc_idx);

	for (rx_sc = rcu_dereference_bh(secy->rx_sc); rx_sc && rx_sc->active;
	     rx_sc = rcu_dereference_bh(rx_sc->next)) {
		rxsc_idx = gpy2xx_get_rxsc_idx_from_rxsc(phy_msec, rx_sc);
		if (unlikely(rxsc_idx < 0))
			continue;

		ret = gpy2xx_apply_rxsc_cfg(phy_msec, rxsc_idx);
		if (ret)
			return ret;
	}

	return ret;
}

static int gpy24x_macsec_get_rx_sa_stats(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	struct gpy2xx_macsec_rxsc *rx_sc;
	struct macsec_rx_sa_stats *ig_sa_stats;
	struct macsec_rx_sa *rx_sa;
	struct sa_next_pn next_pn;
	unsigned int sa_idx;
	int rxsc_idx, ret = 0;

	if (!phy_msec)
		return -EINVAL;

	rxsc_idx = gpy2xx_get_rxsc_idx_from_rxsc(phy_msec, ctx->rx_sc);
	if (rxsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	rx_sc = &phy_msec->rx_sc[rxsc_idx];
	ig_sa_stats = &rx_sc->rx_sa_stats[ctx->sa.assoc_num];

	ctx->stats.rx_sa_stats->InPktsOK = ig_sa_stats->InPktsOK;
	ctx->stats.rx_sa_stats->InPktsInvalid = ig_sa_stats->InPktsInvalid;
	ctx->stats.rx_sa_stats->InPktsNotValid = ig_sa_stats->InPktsNotValid;
	ctx->stats.rx_sa_stats->InPktsNotUsingSA = ig_sa_stats->InPktsNotUsingSA;
	ctx->stats.rx_sa_stats->InPktsUnusedSA = ig_sa_stats->InPktsUnusedSA;

#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	pr_err("\t%40s:\t%u\n", "InPktsOK", ctx->stats.rx_sa_stats->InPktsOK);
	pr_err("\t%40s:\t%u\n", "InPktsInvalid", ctx->stats.rx_sa_stats->InPktsInvalid);
	pr_err("\t%40s:\t%u\n", "InPktsNotValid", ctx->stats.rx_sa_stats->InPktsNotValid);
	pr_err("\t%40s:\t%u\n", "InPktsNotUsingSA", ctx->stats.rx_sa_stats->InPktsNotUsingSA);
	pr_err("\t%40s:\t%u\n", "InPktsUnusedSA", ctx->stats.rx_sa_stats->InPktsUnusedSA);
#endif

	rx_sa = rcu_dereference_bh(rx_sc->sw_rxsc->sa[ctx->sa.assoc_num]);
	sa_idx = rx_sc->hw_sc_idx | ctx->sa.assoc_num;
	next_pn.sa_index = sa_idx;
	ret = eip160_get_ing_sa_next_pn(&phy_msec->dev, &next_pn);
	if (ret == 0) {
		spin_lock_bh(&rx_sa->lock);
		rx_sa->next_pn = next_pn.next_pn;
		spin_unlock_bh(&rx_sa->lock);
	}

	return ret;
}

static int gpy24x_macsec_get_rx_sc_stats(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	struct gpy2xx_macsec_rxsc *rx_sc;
	struct macsec_rx_sa_stats *ig_sa_stats;
	struct macsec_rx_sc_stats *rx_sc_stats;
	struct ing_sa_stats _ig_sa_stats;
	unsigned int sa_idx;
	int rxsc_idx, i;

	if (!phy_msec)
		return -EINVAL;

	rxsc_idx = gpy2xx_get_rxsc_idx_from_rxsc(phy_msec, ctx->rx_sc);
	if (rxsc_idx < 0)
		return -ENOENT;

	if (ctx->prepare)
		return 0;

	memset(&_ig_sa_stats, 0, sizeof(struct ing_sa_stats));
	rx_sc = &phy_msec->rx_sc[rxsc_idx];
	rx_sc_stats = &rx_sc->rx_sc_stats;

	for (i = 0; i < MACSEC_NUM_AN; i++) {
		if (!test_bit(i, &rx_sc->rx_sa_idx_busy))
			continue;

		sa_idx = rx_sc->hw_sc_idx | i;
		_ig_sa_stats.rule_index = sa_idx;

		if (eip160_get_ing_sa_stats(&phy_msec->dev, &_ig_sa_stats))
			return -EIO;

		ig_sa_stats = &rx_sc->rx_sa_stats[i];

		if (ctx->secy->protect_frames)
			rx_sc_stats->InOctetsValidated += _ig_sa_stats.InOctetsValidated;
		else if (ctx->secy->tx_sc.encrypt)
			rx_sc_stats->InOctetsDecrypted += _ig_sa_stats.InOctetsDecrypted;

		rx_sc_stats->InPktsUnchecked += _ig_sa_stats.InPktsUnchecked;
		rx_sc_stats->InPktsDelayed += _ig_sa_stats.InPktsDelayed;
		rx_sc_stats->InPktsLate += _ig_sa_stats.InPktsLate;

		ig_sa_stats->InPktsOK += _ig_sa_stats.InPktsOK;
		ig_sa_stats->InPktsInvalid += _ig_sa_stats.InPktsInvalid;
		ig_sa_stats->InPktsNotValid += _ig_sa_stats.InPktsNotValid;
		ig_sa_stats->InPktsNotUsingSA += _ig_sa_stats.InPktsNotUsingSA;
		ig_sa_stats->InPktsUnusedSA += _ig_sa_stats.InPktsUnusedSA;

		if (ctx->secy->tx_sc.encrypt)
			ctx->stats.rx_sc_stats->InOctetsDecrypted += rx_sc_stats->InOctetsDecrypted;
		else if (ctx->secy->protect_frames)
			ctx->stats.rx_sc_stats->InOctetsValidated += rx_sc_stats->InOctetsValidated;

		ctx->stats.rx_sc_stats->InPktsUnchecked += rx_sc_stats->InPktsUnchecked;
		ctx->stats.rx_sc_stats->InPktsDelayed += rx_sc_stats->InPktsDelayed;
		ctx->stats.rx_sc_stats->InPktsLate += rx_sc_stats->InPktsLate;

		ctx->stats.rx_sc_stats->InPktsOK += ig_sa_stats->InPktsOK;
		ctx->stats.rx_sc_stats->InPktsInvalid += ig_sa_stats->InPktsInvalid;
		ctx->stats.rx_sc_stats->InPktsNotValid += ig_sa_stats->InPktsNotValid;
		ctx->stats.rx_sc_stats->InPktsNotUsingSA += ig_sa_stats->InPktsNotUsingSA;
		ctx->stats.rx_sc_stats->InPktsUnusedSA += ig_sa_stats->InPktsUnusedSA;
	}

#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	pr_err("\t%40s:\t%llu\n", "InOctetsValidated", ctx->stats.rx_sc_stats->InOctetsValidated);
	pr_err("\t%40s:\t%llu\n", "InOctetsDecrypted", ctx->stats.rx_sc_stats->InOctetsDecrypted);
	pr_err("\t%40s:\t%llu\n", "InPktsUnchecked", ctx->stats.rx_sc_stats->InPktsUnchecked);
	pr_err("\t%40s:\t%llu\n", "InPktsDelayed", ctx->stats.rx_sc_stats->InPktsDelayed);
	pr_err("\t%40s:\t%llu\n", "InPktsOK", ctx->stats.rx_sc_stats->InPktsOK);
	pr_err("\t%40s:\t%llu\n", "InPktsInvalid", ctx->stats.rx_sc_stats->InPktsInvalid);
	pr_err("\t%40s:\t%llu\n", "InPktsLate", ctx->stats.rx_sc_stats->InPktsLate);
	pr_err("\t%40s:\t%llu\n", "InPktsNotValid", ctx->stats.rx_sc_stats->InPktsNotValid);
	pr_err("\t%40s:\t%llu\n", "InPktsNotUsingSA", ctx->stats.rx_sc_stats->InPktsNotUsingSA);
	pr_err("\t%40s:\t%llu\n", "InPktsUnusedSA", ctx->stats.rx_sc_stats->InPktsUnusedSA);
#endif

	return 0;
}

static int gpy24x_macsec_get_tx_sa_stats(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	const struct macsec_secy *secy;
	struct gpy2xx_macsec_txsc *tx_sc;
	struct macsec_tx_sa_stats *eg_sa_stats;
	struct macsec_tx_sa *tx_sa;
	struct sa_next_pn next_pn;
	unsigned int sa_idx;
	int txsc_idx, ret = 0;

	if (!phy_msec)
		return -EINVAL;

	txsc_idx = gpy2xx_get_txsc_idx_from_secy(phy_msec, ctx->secy);

	if (txsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	tx_sc = &phy_msec->tx_sc[txsc_idx];
	eg_sa_stats = &tx_sc->tx_sa_stats[ctx->sa.assoc_num];

	if (ctx->secy->tx_sc.encrypt)
		ctx->stats.tx_sa_stats->OutPktsEncrypted = eg_sa_stats->OutPktsEncrypted;
	else if (ctx->secy->protect_frames)
		ctx->stats.tx_sa_stats->OutPktsProtected = eg_sa_stats->OutPktsProtected;

#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	pr_err("\t%40s:\t%u\n", "OutPktsProtected", ctx->stats.tx_sa_stats->OutPktsProtected);
	pr_err("\t%40s:\t%u\n", "OutPktsEncrypted", ctx->stats.tx_sa_stats->OutPktsEncrypted);
#endif

	secy = tx_sc->sw_secy;
	tx_sa = rcu_dereference_bh(tx_sc->sw_txsc->sa[ctx->sa.assoc_num]);
	sa_idx = tx_sc->hw_sc_idx | ctx->sa.assoc_num;
	next_pn.sa_index = sa_idx;
	ret = eip160_get_egr_sa_next_pn(&phy_msec->dev, &next_pn);
	if (ret == 0) {
		spin_lock_bh(&tx_sa->lock);
		tx_sa->next_pn = next_pn.next_pn;
		spin_unlock_bh(&tx_sa->lock);
	}

	return ret;
}

static int gpy24x_macsec_get_tx_sc_stats(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	struct gpy2xx_macsec_txsc *tx_sc;
	struct macsec_tx_sa_stats *eg_sa_stats;
	struct macsec_tx_sc_stats *tx_sc_stats;
	struct egr_sa_stats _eg_sa_stats;
	int txsc_idx;

	if (!phy_msec)
		return -EINVAL;

	txsc_idx = gpy2xx_get_txsc_idx_from_secy(phy_msec, ctx->secy);
	if (txsc_idx < 0)
		return -ENOENT;

	if (ctx->prepare)
		return 0;

	memset(&_eg_sa_stats, 0, sizeof(struct egr_sa_stats));
	_eg_sa_stats.rule_index = txsc_idx;

	if (eip160_get_egr_sa_stats(&phy_msec->dev, &_eg_sa_stats))
		return -EIO;

	tx_sc = &phy_msec->tx_sc[txsc_idx];
	tx_sc_stats =  &tx_sc->tx_sc_stats;
	eg_sa_stats = &tx_sc->tx_sa_stats[ctx->sa.assoc_num];

	tx_sc_stats->OutOctetsProtected += _eg_sa_stats.OutOctetsProtected;
	tx_sc_stats->OutOctetsEncrypted += _eg_sa_stats.OutOctetsEncrypted;

	eg_sa_stats->OutPktsProtected += _eg_sa_stats.OutPktsProtected;
	eg_sa_stats->OutPktsEncrypted += _eg_sa_stats.OutPktsEncrypted;

	if (ctx->secy->tx_sc.encrypt) {
		ctx->stats.tx_sc_stats->OutPktsEncrypted = eg_sa_stats->OutPktsEncrypted;
		ctx->stats.tx_sc_stats->OutOctetsEncrypted = tx_sc_stats->OutOctetsEncrypted;
	} else if (ctx->secy->protect_frames) {
		ctx->stats.tx_sc_stats->OutPktsProtected = eg_sa_stats->OutPktsProtected;
		ctx->stats.tx_sc_stats->OutOctetsProtected = tx_sc_stats->OutOctetsProtected;
	}

#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	pr_err("\t%40s:\t%llu\n", "OutPktsProtected", ctx->stats.tx_sc_stats->OutPktsProtected);
	pr_err("\t%40s:\t%llu\n", "OutPktsEncrypted", ctx->stats.tx_sc_stats->OutPktsEncrypted);
	pr_err("\t%40s:\t%llu\n", "OutOctetsProtected", ctx->stats.tx_sc_stats->OutOctetsProtected);
	pr_err("\t%40s:\t%llu\n", "OutOctetsEncrypted", ctx->stats.tx_sc_stats->OutOctetsEncrypted);
#endif

	return 0;
}

static int gpy24x_macsec_get_dev_stats(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	struct macsec_dev_stats *glb_stats = &phy_msec->glb_stats;
	struct ing_global_stats _ig_glb_stats;
	struct egr_global_stats _eg_glb_stats;

	if (!phy_msec)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	memset(&_ig_glb_stats, 0, sizeof(struct ing_global_stats));
	memset(&_eg_glb_stats, 0, sizeof(struct egr_global_stats));

	if (eip160_get_ing_global_stats(&phy_msec->dev, &_ig_glb_stats))
		return -EIO;

	if (eip160_get_egr_global_stats(&phy_msec->dev, &_eg_glb_stats))
		return -EIO;

#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	pr_err("\nRX GLOBAL Counters:\n----------------\n");
	pr_err("\t%40s:\t%llu\n", "TransformErrorPkts", _ig_glb_stats.TransformErrorPkts);
	pr_err("\t%40s:\t%llu\n", "InPktsCtrl", _ig_glb_stats.InPktsCtrl);
	pr_err("\t%40s:\t%llu\n", "InPktsTagged", _ig_glb_stats.InPktsTagged);
	pr_err("\t%40s:\t%llu\n", "InPktsUntaggedMiss", _ig_glb_stats.InPktsUntaggedMiss);
	pr_err("\t%40s:\t%llu\n", "InOverSizePkts", _ig_glb_stats.InOverSizePkts);
	pr_err("\nTX GLOBAL Counters:\n----------------\n");
	pr_err("\t%40s:\t%llu\n", "TransformErrorPkts", _eg_glb_stats.TransformErrorPkts);
	pr_err("\t%40s:\t%llu\n", "OutPktsCtrl", _eg_glb_stats.OutPktsCtrl);
	pr_err("\t%40s:\t%llu\n", "OutPktsUnknownSA", _eg_glb_stats.OutPktsUnknownSA);
#endif

	glb_stats->OutPktsUntagged += _eg_glb_stats.OutPktsUntagged;
	glb_stats->OutPktsTooLong += _eg_glb_stats.OutOverSizePkts;
	glb_stats->InPktsUntagged += _ig_glb_stats.InPktsUntagged;
	glb_stats->InPktsNoTag += _ig_glb_stats.InPktsNoTag;
	glb_stats->InPktsBadTag += _ig_glb_stats.InPktsBadTag;
	glb_stats->InPktsUnknownSCI += _ig_glb_stats.InPktsUnknownSCI;
	glb_stats->InPktsNoSCI += _ig_glb_stats.InPktsNoSCI;

	ctx->stats.dev_stats->OutPktsUntagged = glb_stats->OutPktsUntagged;
	ctx->stats.dev_stats->OutPktsTooLong = glb_stats->OutPktsTooLong;
	ctx->stats.dev_stats->InPktsUntagged = glb_stats->InPktsUntagged;
	ctx->stats.dev_stats->InPktsNoTag = glb_stats->InPktsNoTag;
	ctx->stats.dev_stats->InPktsBadTag = glb_stats->InPktsBadTag;
	ctx->stats.dev_stats->InPktsUnknownSCI = glb_stats->InPktsUnknownSCI;
	ctx->stats.dev_stats->InPktsNoSCI = glb_stats->InPktsNoSCI;
	ctx->stats.dev_stats->InPktsOverrun = 0; /* This condition cannot occur */

#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	pr_err("\t%40s:\t%llu\n", "OutPktsUntagged", ctx->stats.dev_stats->OutPktsUntagged);
	pr_err("\t%40s:\t%llu\n", "OutPktsTooLong", ctx->stats.dev_stats->OutPktsTooLong);
	pr_err("\t%40s:\t%llu\n", "InPktsUntagged", ctx->stats.dev_stats->InPktsUntagged);
	pr_err("\t%40s:\t%llu\n", "InPktsNoTag", ctx->stats.dev_stats->InPktsNoTag);
	pr_err("\t%40s:\t%llu\n", "InPktsBadTag", ctx->stats.dev_stats->InPktsBadTag);
	pr_err("\t%40s:\t%llu\n", "InPktsUnknownSCI", ctx->stats.dev_stats->InPktsUnknownSCI);
	pr_err("\t%40s:\t%llu\n", "InPktsNoSCI", ctx->stats.dev_stats->InPktsNoSCI);
	pr_err("\t%40s:\t%llu\n", "InPktsOverrun", ctx->stats.dev_stats->InPktsOverrun);
#endif

	return 0;
}

static int gpy24x_macsec_del_txsa(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	int txsc_idx, ret = 0;

	if (!phy_msec)
		return -EINVAL;

	txsc_idx = gpy2xx_get_txsc_idx_from_secy(phy_msec, ctx->secy);
	if (txsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	ret = gpy2xx_clear_txsa(phy_msec, &phy_msec->tx_sc[txsc_idx],
				ctx->sa.assoc_num, LGM_CLEAR_ALL);
	if (ret < 0)
		return ret;

	return ret;
}

static int gpy24x_macsec_upd_txsa(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	const struct macsec_secy *secy = ctx->secy;
	struct gpy2xx_macsec_txsc *tx_sc;
	int txsc_idx, ret = 0;

	if (!phy_msec)
		return -EINVAL;

	txsc_idx = gpy2xx_get_txsc_idx_from_secy(phy_msec, secy);
	if (txsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	tx_sc = &phy_msec->tx_sc[txsc_idx];

	ret = gpy2xx_update_txsa(phy_msec, tx_sc->hw_sc_idx, secy,
				 ctx->sa.tx_sa, NULL, ctx->sa.assoc_num);
	if (ret < 0)
		return ret;

	return ret;
}

static int gpy24x_macsec_add_txsa(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	const struct macsec_secy *secy = ctx->secy;
	struct gpy2xx_macsec_txsc *tx_sc;
	int txsc_idx, ret = 0;

	if (!phy_msec)
		return -EINVAL;

	txsc_idx = gpy2xx_get_txsc_idx_from_secy(phy_msec, secy);
	if (txsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	tx_sc = &phy_msec->tx_sc[txsc_idx];
	phy_msec->tx_sc[txsc_idx].sw_txsc = &secy->tx_sc;
	set_bit(ctx->sa.assoc_num, &tx_sc->tx_sa_idx_busy);
	memcpy(tx_sc->tx_sa_key[ctx->sa.assoc_num], ctx->sa.key, secy->key_len);

	ret = gpy2xx_update_txsa(phy_msec, tx_sc->hw_sc_idx, secy,
				 ctx->sa.tx_sa, ctx->sa.key, ctx->sa.assoc_num);
	if (ret < 0)
		return ret;

	return ret;
}

static int gpy24x_macsec_del_rxsa(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	const struct macsec_rx_sc *rx_sc = ctx->sa.rx_sa->sc;
	int rxsc_idx, ret = 0;

	if (!phy_msec)
		return -EINVAL;

	rxsc_idx = gpy2xx_get_rxsc_idx_from_rxsc(phy_msec, rx_sc);
	if (rxsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	ret = gpy2xx_clear_rxsa(phy_msec, &phy_msec->rx_sc[rxsc_idx],
				ctx->sa.assoc_num, LGM_CLEAR_ALL);
	if (ret < 0)
		return ret;

	return ret;
}

static int gpy24x_macsec_upd_rxsa(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	const struct macsec_rx_sc *rx_sc = ctx->sa.rx_sa->sc;
	const struct macsec_secy *secy = ctx->secy;
	int rxsc_idx, ret = 0;

	if (!phy_msec)
		return -EINVAL;

	rxsc_idx = gpy2xx_get_rxsc_idx_from_rxsc(phy_msec, rx_sc);
	if (rxsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	ret = gpy2xx_update_rxsa(phy_msec, phy_msec->rx_sc[rxsc_idx].hw_sc_idx,
				 secy, ctx->sa.rx_sa, NULL, ctx->sa.assoc_num);
	if (ret < 0)
		return ret;

	return ret;
}

static int gpy24x_macsec_add_rxsa(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	const struct macsec_rx_sc *rx_sc = ctx->sa.rx_sa->sc;
	const struct macsec_secy *secy = ctx->secy;
	struct gpy2xx_macsec_rxsc *gpy24x_rxsc;
	int rxsc_idx, ret = 0;

	if (!phy_msec)
		return -EINVAL;

	rxsc_idx = gpy2xx_get_rxsc_idx_from_rxsc(phy_msec, rx_sc);
	if (rxsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	gpy24x_rxsc = &phy_msec->rx_sc[rxsc_idx];
	set_bit(ctx->sa.assoc_num, &gpy24x_rxsc->rx_sa_idx_busy);
	memcpy(gpy24x_rxsc->rx_sa_key[ctx->sa.assoc_num], ctx->sa.key, secy->key_len);

	ret = gpy2xx_update_rxsa(phy_msec, gpy24x_rxsc->hw_sc_idx, secy,
				 ctx->sa.rx_sa, ctx->sa.key, ctx->sa.assoc_num);
	if (ret < 0)
		return ret;

	return ret;
}

static int gpy24x_macsec_del_rxsc(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	enum macsec_clr_type clear_type = MACSEC_CLEAR_SW;
	int rxsc_idx, ret = 0;

	if (!phy_msec)
		return -EINVAL;

	rxsc_idx = gpy2xx_get_rxsc_idx_from_rxsc(phy_msec, ctx->rx_sc);
	if (rxsc_idx < 0)
		return -ENOENT;

	if (ctx->prepare)
		return 0;

	if (netif_carrier_ok(ctx->netdev))
		clear_type = LGM_CLEAR_ALL;

	ret = gpy2xx_clear_rxsc(phy_msec, rxsc_idx, clear_type);
	if (ret < 0)
		return ret;

	return ret;
}

static int gpy24x_macsec_upd_rxsc(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	int rxsc_idx, ret = 0;

	if (!phy_msec)
		return -EINVAL;

	rxsc_idx = gpy2xx_get_rxsc_idx_from_rxsc(phy_msec, ctx->rx_sc);
	if (rxsc_idx < 0)
		return -ENOENT;

	if (ctx->prepare)
		return 0;

	ret = gpy2xx_set_rxsc(phy_msec, rxsc_idx);
	if (ret < 0)
		return ret;

	return ret;
}

static int gpy24x_macsec_add_rxsc(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	const u32 rxsc_idx_max = gpy2xx_sc_idx_max(phy_msec->sc_sa);
	u32 rxsc_idx;
	int ret = 0;

	if (!phy_msec)
		return -EINVAL;

	if (hweight32(phy_msec->rxsc_idx_busy) >= rxsc_idx_max)
		return -ENOSPC;

	rxsc_idx = ffz(phy_msec->rxsc_idx_busy);
	if (rxsc_idx >= rxsc_idx_max)
		return -ENOSPC;

	if (ctx->prepare)
		return 0;

	phy_msec->rx_sc[rxsc_idx].hw_sc_idx =
				gpy2xx_to_hw_sc_idx(rxsc_idx, phy_msec->sc_sa);
	phy_msec->rx_sc[rxsc_idx].sw_secy = ctx->secy;
	phy_msec->rx_sc[rxsc_idx].sw_rxsc = ctx->rx_sc;

	ret = gpy2xx_set_rxsc(phy_msec, rxsc_idx);
	if (ret < 0)
		return ret;

	set_bit(rxsc_idx, &phy_msec->rxsc_idx_busy);

	return ret;
}

static int gpy24x_macsec_del_secy(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	int ret = 0;

	if (!phy_msec)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	ret = gpy2xx_clear_secy_cfg(phy_msec, ctx->secy, LGM_CLEAR_ALL);
	if (ret < 0)
		return ret;

	ret = gpy24x_macsec_uninit(ctx->phydev);
	if (ret < 0)
		return ret;

	return ret;
}

static int gpy24x_macsec_upd_secy(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	const struct macsec_secy *secy = ctx->secy;
	int txsc_idx, ret = 0;

	if (!phy_msec)
		return -EINVAL;

	txsc_idx = gpy2xx_get_txsc_idx_from_secy(phy_msec, secy);
	if (txsc_idx < 0)
		return -ENOENT;

	if (ctx->prepare)
		return 0;

	ret = gpy2xx_set_txsc(phy_msec, txsc_idx);
	if (ret < 0)
		return ret;

	return ret;
}

static int gpy24x_macsec_add_secy(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	const struct macsec_secy *secy = ctx->secy;
	enum macsec_sc_sa_map sc_sa;
	u32 txsc_idx;
	int ret = 0;

	if (!phy_msec)
		return -EINVAL;

	ret = gpy24x_macsec_init(ctx->phydev);
	if (ret < 0)
		return ret;

	sc_sa = gpy24x_sc_sa_from_num_an(MACSEC_NUM_AN);
	if (sc_sa == MACSEC_SA_SC_NOT_USED)
		return -EINVAL;

	if (hweight32(phy_msec->txsc_idx_busy) >= gpy2xx_sc_idx_max(sc_sa))
		return -ENOSPC;

	txsc_idx = ffz(phy_msec->txsc_idx_busy);
	if (txsc_idx == MACSEC_MAX_SC)
		return -ENOSPC;

	if (ctx->prepare)
		return 0;

	phy_msec->sc_sa = sc_sa;
	phy_msec->tx_sc[txsc_idx].hw_sc_idx =
				gpy2xx_to_hw_sc_idx(txsc_idx, sc_sa);
	phy_msec->tx_sc[txsc_idx].sw_secy = secy;
	phy_msec->tx_sc[txsc_idx].sw_txsc = &secy->tx_sc;

#if defined(DEBUG_MSEC_PRINT) && DEBUG_MSEC_PRINT
	pr_err("sc_sa = %d\n", sc_sa);
	pr_err("available first index = %d\n", txsc_idx);
	pr_err("hw_sc_idx = %d\n", phy_msec->tx_sc[txsc_idx].hw_sc_idx);
#endif

	ret = gpy2xx_set_txsc(phy_msec, txsc_idx);
	if (ret < 0)
		return ret;

	set_bit(txsc_idx, &phy_msec->txsc_idx_busy);

	return ret;
}

static int gpy24x_macsec_dev_stop(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	int i, ret = 0;

	if (!phy_msec)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	if (!phy_msec->initialized)
		return 0;

	for (i = 0; i < MACSEC_MAX_SC; i++) {
		if (phy_msec->txsc_idx_busy & BIT(i)) {
			ret = gpy2xx_clear_secy_cfg(phy_msec,
						    phy_msec->tx_sc[i].sw_secy,
						    MACSEC_CLEAR_HW);
			if (ret < 0)
				return ret;
		}
	}

	phy_msec->initialized = false;

	return ret;
}

static int gpy24x_macsec_dev_open(struct macsec_context *ctx)
{
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(ctx->phydev);
	int ret = 0;

	if (!phy_msec)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	if (phy_msec->initialized)
		/* only 1 SecY is supported */
		return -EEXIST;

	ret = gpy2xx_apply_secy_cfg(phy_msec, ctx->secy);
	if (ret < 0)
		return ret;

	phy_msec->initialized = true;

	return ret;
}

/**
 * struct macsec_ops - MACsec offloading operations
 */
const struct macsec_ops gpy24x_macsec_ops = {
	/* Device wide */
	.mdo_dev_open = gpy24x_macsec_dev_open,
	.mdo_dev_stop = gpy24x_macsec_dev_stop,
	/* SecY */
	.mdo_add_secy = gpy24x_macsec_add_secy,
	.mdo_upd_secy = gpy24x_macsec_upd_secy,
	.mdo_del_secy = gpy24x_macsec_del_secy,
	/* Security channels */
	.mdo_add_rxsc = gpy24x_macsec_add_rxsc,
	.mdo_upd_rxsc = gpy24x_macsec_upd_rxsc,
	.mdo_del_rxsc = gpy24x_macsec_del_rxsc,
	/* Rx Security associations */
	.mdo_add_rxsa = gpy24x_macsec_add_rxsa,
	.mdo_upd_rxsa = gpy24x_macsec_upd_rxsa,
	.mdo_del_rxsa = gpy24x_macsec_del_rxsa,
	/* Tx Security associations */
	.mdo_add_txsa = gpy24x_macsec_add_txsa,
	.mdo_upd_txsa = gpy24x_macsec_upd_txsa,
	.mdo_del_txsa = gpy24x_macsec_del_txsa,
	/* Statistics */
	.mdo_get_dev_stats = gpy24x_macsec_get_dev_stats,
	.mdo_get_tx_sc_stats = gpy24x_macsec_get_tx_sc_stats,
	.mdo_get_tx_sa_stats = gpy24x_macsec_get_tx_sa_stats,
	.mdo_get_rx_sc_stats = gpy24x_macsec_get_rx_sc_stats,
	.mdo_get_rx_sa_stats = gpy24x_macsec_get_rx_sa_stats,
};

static inline int is_mbox_rw_done(struct phy_device *phydev)
{
	int ret;
	int timeout = 3000;

	do {
		ret = phy_read_mmd(phydev, MDIO_MMD_VEND1, 7);
		if (ret < 0)
			return ret;
		if (ret & BIT(15))
			return 0;
	} while (timeout-- > 0);
	return -EBUSY;
}

inline int gpy2xx_mbox_saz_rd(struct phy_device *phydev, u32 regaddr,
				u16 *regdata)
{
	int ret;
	u32 cmd;

	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 6, regaddr & 0xFFFF);
	if (ret < 0)
		goto ERROR;

	cmd = ((regaddr >> 16) & 0xFF) | (0 << 8);
	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 7, cmd);
	if (ret < 0)
		goto ERROR;

	ret = is_mbox_rw_done(phydev);
	if (ret)
		goto ERROR;

	ret = phy_read_mmd(phydev, MDIO_MMD_VEND1, 5);
	if (ret < 0)
		goto ERROR;

	*regdata = ret;

	return ret;

ERROR:
	pr_err("Critical error happened during mailbox 32-bit read.\n");
	return ret;
}

inline int gpy2xx_mbox_saz_wr(struct phy_device *phydev, u32 regaddr,
				u16 data)
{
	int ret;
	u32 cmd;

	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 5, data);
	if (ret)
		goto ERROR;

	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 6, regaddr & 0xFFFF);
	if (ret)
		goto ERROR;

	cmd = ((regaddr >> 16) & 0xFF) | (1 << 8);
	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 7, cmd);
	if (ret)
		goto ERROR;

	ret = is_mbox_rw_done(phydev);
	if (ret)
		goto ERROR;

	return 0;

ERROR:
	pr_err("Critical error happened during mailbox 32-bit write.\n");
	return ret;
}

inline int gpy2xx_mbox_ing_rd(struct macsec_context *ctx, u32 regaddr,
				u32 *regdata)
{
	int ret, data_low = 0, data_high = 0;
	u32 cmd;
	struct phy_device *phydev = ctx->phydev;

	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 6, regaddr & 0xFFFF);
	if (ret < 0)
		goto ERROR;

	cmd = (0xB0) | (4 << 8);
	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 7, cmd);
	if (ret < 0)
		goto ERROR;

	ret = is_mbox_rw_done(phydev);
	if (ret)
		goto ERROR;

	data_low = phy_read_mmd(phydev, MDIO_MMD_VEND1, 5);
	if (data_low < 0)
		goto ERROR;

	data_high = phy_read_mmd(phydev, MDIO_MMD_VEND1, 13);
	if (data_high < 0)
		goto ERROR;

	*regdata = (data_high << 16) | data_low;

	return ret;

ERROR:
	pr_err("Critical error happened during mailbox 32-bit read.\n");
	return ret;
}

inline int gpy2xx_mbox_ing_wr(struct macsec_context *ctx, u32 regaddr,
				u32 data)
{
	int ret;
	u32 cmd;
	struct phy_device *phydev = ctx->phydev;

	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 5, data & 0xFFFF);
	if (ret)
		goto ERROR;

	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 13, data >> 16);
	if (ret)
		goto ERROR;

	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 6, regaddr & 0xFFFF);
	if (ret)
		goto ERROR;

	cmd = (0xB0) | (5 << 8);
	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 7, cmd);
	if (ret)
		goto ERROR;

	ret = is_mbox_rw_done(phydev);
	if (ret)
		goto ERROR;

	return 0;

ERROR:
	pr_err("Critical error happened during mailbox 32-bit write.\n");
	return ret;
}

inline int gpy2xx_mbox_egr_rd(struct macsec_context *ctx, u32 regaddr,
				u32 *regdata)
{
	int ret, data_low = 0, data_high = 0;
	u32 cmd;
	struct phy_device *phydev = ctx->phydev;

	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 6, regaddr & 0xFFFF);
	if (ret < 0)
		goto ERROR;

	cmd = (0xB2) | (4 << 8);
	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 7, cmd);
	if (ret < 0)
		goto ERROR;

	ret = is_mbox_rw_done(phydev);
	if (ret)
		goto ERROR;

	data_low = phy_read_mmd(phydev, MDIO_MMD_VEND1, 5);
	if (data_low < 0)
		goto ERROR;

	data_high = phy_read_mmd(phydev, MDIO_MMD_VEND1, 13);
	if (data_high < 0)
		goto ERROR;

	*regdata = (data_high << 16) | data_low;

	return ret;

ERROR:
	pr_err("Critical error happened during mailbox 32-bit read.\n");
	return ret;
}

inline int gpy2xx_mbox_egr_wr(struct macsec_context *ctx, u32 regaddr,
				u32 data)
{
	int ret;
	u32 cmd;
	struct phy_device *phydev = ctx->phydev;

	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 5, data & 0xFFFF);
	if (ret)
		goto ERROR;

	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 13, data >> 16);
	if (ret)
		goto ERROR;

	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 6, regaddr & 0xFFFF);
	if (ret)
		goto ERROR;

	cmd = (0xB2) | (5 << 8);
	ret = phy_write_mmd(phydev, MDIO_MMD_VEND1, 7, cmd);
	if (ret)
		goto ERROR;

	ret = is_mbox_rw_done(phydev);
	if (ret)
		goto ERROR;

	return 0;

ERROR:
	pr_err("Critical error happened during mailbox 32-bit write.\n");
	return ret;
}

int gpy2xx_get_chip_id(struct phy_device *phydev)
{
	int ret;
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(phydev);
	u16 _data;

	if (!phy_msec)
		return -EINVAL;

	ret = phy_msec->mbox_saz_rd(phydev, P31G_CHIP_ID_CFG_ADDR, &_data);
	if (ret < 0) {
		phydev_err(phydev, "ERROR: CHIP_ID_CFG read fails\n");
		return -EINVAL;
	}

	if (_data == CHIP_ID_CFG_GPY215) {
		phy_msec->pdi_module_base = PM_PDI_MODULE_BASE;
		phy_msec->gmac_module_base = GMACF_EQOS_MAC_MODULE_BASE;
		return 0;
	}
	ret = phy_msec->mbox_saz_rd(phydev, P34X_CHIP_ID_CFG_ADDR, &_data);
	if (ret < 0) {
		phydev_err(phydev, "ERROR: CHIP_ID_CFG read fails\n");
		return -EINVAL;
	}

	if (_data == CHIP_ID_CFG_MXL86249 || _data == CHIP_ID_CFG_MXL86249C) {
		phy_msec->pdi_module_base = P34X_PM_PDI_MODULE_BASE;
		phy_msec->gmac_module_base = P34X_GMACF_EQOS_MAC_MODULE_BASE;
		return 0;
	}
	return -EINVAL;
}

int gpy24x_macsec_init(struct phy_device *phydev)
{
	int ret;
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(phydev);
	u16 _data, _saz_bypas;
	u16 pm_pdi_cfg, pm_gmacf_cfg, mac_cfg_h;

	if (!phy_msec)
		return -EINVAL;

#if defined(GPY2XX_REG_RW) && GPY2XX_REG_RW
	int fw_ver, sgmii_stat;

	/* Show GPY PHY FW version in dmesg */
	fw_ver = phy_read(phydev, 0x1e);
	if (fw_ver < 0)
		return fw_ver;

	phydev_info(phydev, "Fw Ver: 0x%04X\n", fw_ver);

	/* Get macsec support */
	sgmii_stat = phy_read_mmd(phydev, MDIO_MMD_VEND1, 9);
	if (sgmii_stat < 0) {
		pr_crit("ERROR: VSPEC1_SGMII_STAT read fails\n");
		return sgmii_stat;
	}

	phydev_info(phydev, "Sgmii stat: 0x%04X\n", sgmii_stat);
#endif

	ret = gpy2xx_get_chip_id(phydev);
	if (ret < 0) {
		return -EINVAL;
	}

	/* Get PM Configuration */
	ret = phy_msec->mbox_saz_rd(phydev,
				phy_msec->pdi_module_base + PM_PDI_CFG, &_data);
	if (ret < 0) {
		pr_crit("ERROR: PM_PDI_CFG read fails\n");
		return ret;
	}

	pm_pdi_cfg = _data;
	_saz_bypas = FIELD_GET(PM_PDI_CFG_GMAC_BYP_MASK, pm_pdi_cfg) |
					FIELD_GET(PM_PDI_CFG_GMACL_BYP_MASK, pm_pdi_cfg);
	if (_saz_bypas == 1) {
		pr_crit("ERROR: Smart-AZ can't be in bypassed state for MACsec\n");
		return ret;
	}

	REG_FIELD_REPL(pm_pdi_cfg, 0, PM_PDI_CFG_MACSECE_BYP);
	REG_FIELD_REPL(pm_pdi_cfg, 0, PM_PDI_CFG_MACSECI_BYP);
	REG_FIELD_REPL(pm_pdi_cfg, 0, PM_PDI_CFG_MACSEC_RST);
	REG_FIELD_REPL(pm_pdi_cfg, 1, PM_PDI_CFG_CRC_ERR);
	REG_FIELD_REPL(pm_pdi_cfg, 1, PM_PDI_CFG_PKT_ERR);
	REG_FIELD_REPL(pm_pdi_cfg, 1, PM_PDI_CFG_SEC_FAIL);
	REG_FIELD_REPL(pm_pdi_cfg, 1, PM_PDI_CFG_CLASS_DROP);

	/* Set PM Configuration */
	ret = phy_msec->mbox_saz_wr(phydev,
				phy_msec->pdi_module_base + PM_PDI_CFG, pm_pdi_cfg);
	if (ret < 0) {
		pr_crit("ERROR: PM_PDI_CFG write fails\n");
		return ret;
	}

	/* Get PM GMAC Configuration */
	ret = phy_msec->mbox_saz_rd(phydev,
				phy_msec->pdi_module_base + PM_PDI_GMAC_CFG, &_data);
	if (ret < 0) {
		pr_crit("ERROR: GMAC_CFG read fails\n");
		return ret;
	}

	pm_gmacf_cfg = _data;
	REG_FIELD_REPL(pm_gmacf_cfg, 0, PM_PDI_GMAC_CFG_GMAC_CRC_PAD);

	/* Set PM GMAC Configuration */
	ret = phy_msec->mbox_saz_wr(phydev,
				phy_msec->pdi_module_base + PM_PDI_GMAC_CFG, pm_gmacf_cfg);
	if (ret < 0) {
		pr_crit("ERROR: GMAC_CFG write fails\n");
		return ret;
	}

	/* Get MAC Pkt Config info */
	ret = phy_msec->mbox_saz_rd(phydev,
				phy_msec->gmac_module_base + GMACF_MAC_CONFIGURATION_H,
				&_data);
	if (ret < 0) {
		pr_crit("ERROR: MAC_CONFIGURATION_H read fails\n");
		return ret;
	}

	mac_cfg_h = _data;
	REG_FIELD_REPL(mac_cfg_h, 1, GMACF_MAC_CONFIGURATION_H_ACS);
	REG_FIELD_REPL(mac_cfg_h, 1, GMACF_MAC_CONFIGURATION_H_CST);
	REG_FIELD_REPL(mac_cfg_h, 0, GMACF_MAC_CONFIGURATION_H_IPG);

	/* Set MAC Pkt Config info */
	ret = phy_msec->mbox_saz_wr(phydev,
				phy_msec->gmac_module_base + GMACF_MAC_CONFIGURATION_H,
				mac_cfg_h);
	if (ret < 0) {
		pr_crit("ERROR: MAC_CONFIGURATION_H write fails\n");
		return ret;
	}

	ret = eip160_init_ing_dev(&phy_msec->dev);
	if (ret < 0) {
		pr_crit("ERROR: MACsec ingress dev init fails\n");
		return ret;
	}

	ret = eip160_init_egr_dev(&phy_msec->dev);
	if (ret < 0) {
		pr_crit("ERROR: MACsec egress dev init fails\n");
		return ret;
	}

	return ret;
}

int gpy24x_macsec_uninit(struct phy_device *phydev)
{
	int ret;
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(phydev);
	u16 _data, _saz_bypas;
	u16 pm_pdi_cfg, pm_gmacf_cfg, mac_cfg_h;

	if (!phy_msec)
		return -EINVAL;

	/* Get PM Configuration */
	ret = phy_msec->mbox_saz_rd(phydev,
				phy_msec->pdi_module_base + PM_PDI_CFG, &_data);
	if (ret < 0) {
		pr_crit("ERROR: PM_PDI_CFG read fails\n");
		return ret;
	}

	pm_pdi_cfg = _data;
	_saz_bypas = FIELD_GET(PM_PDI_CFG_GMAC_BYP_MASK, pm_pdi_cfg) |
					FIELD_GET(PM_PDI_CFG_GMACL_BYP_MASK, pm_pdi_cfg);
	if (_saz_bypas == 1) {
		pr_crit("ERROR: Smart-AZ can't be in bypassed state for MACsec\n");
		return ret;
	}

	REG_FIELD_REPL(pm_pdi_cfg, 1, PM_PDI_CFG_MACSECE_BYP);
	REG_FIELD_REPL(pm_pdi_cfg, 1, PM_PDI_CFG_MACSECI_BYP);
	REG_FIELD_REPL(pm_pdi_cfg, 1, PM_PDI_CFG_MACSEC_RST);

	/* Set PM Configuration */
	ret = phy_msec->mbox_saz_wr(phydev,
				phy_msec->pdi_module_base + PM_PDI_CFG, pm_pdi_cfg);
	if (ret < 0) {
		pr_crit("ERROR: PM_PDI_CFG write fails\n");
		return ret;
	}

	/* Get PM GMAC Configuration */
	ret = phy_msec->mbox_saz_rd(phydev,
				phy_msec->pdi_module_base + PM_PDI_GMAC_CFG, &_data);
	if (ret < 0) {
		pr_crit("ERROR: GMAC_CFG read fails\n");
		return ret;
	}

	pm_gmacf_cfg = _data;

	REG_FIELD_REPL(pm_gmacf_cfg, 2, PM_PDI_GMAC_CFG_GMAC_CRC_PAD);

	/* Set PM GMAC Configuration */
	ret = phy_msec->mbox_saz_wr(phydev,
				phy_msec->pdi_module_base + PM_PDI_GMAC_CFG, pm_gmacf_cfg);
	if (ret < 0) {
		pr_crit("ERROR: GMAC_CFG write fails\n");
		return ret;
	}

	/* Get MAC Pkt Config info */
	ret = phy_msec->mbox_saz_rd(phydev,
				phy_msec->gmac_module_base + GMACF_MAC_CONFIGURATION_H,
				&_data);
	if (ret < 0) {
		pr_crit("ERROR: MAC_CONFIGURATION_H read fails\n");
		return ret;
	}

	mac_cfg_h = _data;

	REG_FIELD_REPL(mac_cfg_h, 0, GMACF_MAC_CONFIGURATION_H_ACS);
	REG_FIELD_REPL(mac_cfg_h, 0, GMACF_MAC_CONFIGURATION_H_CST);
	REG_FIELD_REPL(mac_cfg_h, 0, GMACF_MAC_CONFIGURATION_H_IPG);

	/* Set MAC Pkt Config info */
	ret = phy_msec->mbox_saz_wr(phydev,
				phy_msec->gmac_module_base + GMACF_MAC_CONFIGURATION_H,
				mac_cfg_h);
	if (ret < 0) {
		pr_crit("ERROR: MAC_CONFIGURATION_H write fails\n");
		return ret;
	}

	ret = eip160_init_ing_dev(&phy_msec->dev);
	if (ret < 0) {
		pr_crit("ERROR: MACsec ingress dev init fails\n");
		return ret;
	}

	ret = eip160_init_egr_dev(&phy_msec->dev);
	if (ret < 0) {
		pr_crit("ERROR: MACsec egress dev init fails\n");
		return ret;
	}

	return ret;
}

int gpy24x_reg_macsec_ops(struct phy_device *phydev)
{
	int sgmii_stat;
	struct gpy2xx_macsec_cfg *phy_msec = gpy_get_macsec_cfg(phydev);

	if (!phy_msec)
		return -EINVAL;

#if defined(GPY2XX_REG_RW) && GPY2XX_REG_RW
	int fw_ver, phy_id1, phy_id2;
	int led0, led1, led2;

	/* Show GPY PHY FW version in dmesg */
	fw_ver = phy_read(phydev, 0x1e);
	if (fw_ver < 0)
		return fw_ver;
	phydev_info(phydev, "Fw Ver: 0x%04X\n", fw_ver);

	/* Show GPY PHY ID1 in dmesg */
	phy_id1 = phy_read(phydev, 0x02);
	if (phy_id1 < 0)
		return fw_ver;
	phydev_info(phydev, "Phy ID1: 0x%04X\n", phy_id1);

	/* Show GPY PHY ID2 in dmesg */
	phy_id2 = phy_read(phydev, 0x03);
	if (phy_id2 < 0)
		return phy_id2;
	phydev_info(phydev, "Phy ID2: 0x%04X\n", phy_id2);

	/* Show MMD Led0 in dmesg */
	led0 = phy_read_mmd(phydev, MDIO_MMD_VEND1, 0x01);
	if (led0 < 0)
		return led0;
	phydev_info(phydev, "Led0: 0x%04X\n", led0);

	/* Show MMD Led0 in dmesg */
	led1 = phy_read_mmd(phydev, MDIO_MMD_VEND1, 0x02);
	if (led1 < 0)
		return led1;
	phydev_info(phydev, "Led0: 0x%04X\n", led1);

	/* Show MMD Led0 in dmesg */
	led2 = phy_read_mmd(phydev, MDIO_MMD_VEND1, 0x03);
	if (led2 < 0)
		return led2;
	phydev_info(phydev, "Led0: 0x%04X\n", led2);
#endif

	/* Get macsec support */
	sgmii_stat = phy_read_mmd(phydev, MDIO_MMD_VEND1, 9);
	if (sgmii_stat < 0)
		return sgmii_stat;

	phy_msec->macsec_support =
		FIELD_GET(VSPEC1_SGMII_STAT_MACSEC_CAP_MASK, sgmii_stat);

	if (!phy_msec->macsec_support)
		return -EINVAL;

	phy_msec->mbox_saz_rd = gpy2xx_mbox_saz_rd;
	phy_msec->mbox_saz_wr = gpy2xx_mbox_saz_wr;
	phy_msec->dev.ig_reg_rd = gpy2xx_mbox_ing_rd;
	phy_msec->dev.ig_reg_wr = gpy2xx_mbox_ing_wr;
	phy_msec->dev.eg_reg_rd = gpy2xx_mbox_egr_rd;
	phy_msec->dev.eg_reg_wr = gpy2xx_mbox_egr_wr;
	phy_msec->dev.ctx.phydev = phydev;
	phy_msec->dev.ctx.offload = MACSEC_OFFLOAD_PHY;

	phydev->macsec_ops = &gpy24x_macsec_ops;

	return 0;
}
EXPORT_SYMBOL(gpy24x_reg_macsec_ops);
#endif

MODULE_DESCRIPTION("Maxlinear PHY MACsec Driver");
MODULE_AUTHOR("Govindaiah Mudepalli");
MODULE_LICENSE("GPL");

