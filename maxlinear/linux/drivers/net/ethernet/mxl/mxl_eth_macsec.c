// SPDX-License-Identifier: GPL-2.0
/*
 * Driver for MACsec Engine inside Intel CHD GSWIP or GPHY
 * Copyright 2020 - 2024 Maxlinear, Inc.
 * Copyright 2019 - 2020 Intel Corporation
 */

#include <linux/version.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/kernel.h> /* printk() */
#include <linux/types.h>  /* size_t */
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/phylink.h>
#include <linux/phy.h>

#include <net/datapath_api.h>
#include <net/switch_api/gsw_dev.h>

#include "../../../crypto/mxl/eip160s_struct.h"
#include "mxl_eth_macsec.h"
#include "mxl_eth_drv.h"

static u32 lgm_sc_idx_max(const enum lgm_macsec_sc_sa sc_sa)
{
	u32 result = 0;

	switch (sc_sa) {
	case LGM_MACSEC_SA_SC_4SA_8SC:
		result = 8;
		break;
	case LGM_MACSEC_SA_SC_2SA_16SC:
		result = 16;
		break;
	case LGM_MACSEC_SA_SC_1SA_32SC:
		result = 32;
		break;
	default:
		break;
	};

	return result;
}

static u32 lgm_to_hw_sc_idx(const u32 sc_idx, const enum lgm_macsec_sc_sa sc_sa)
{
	switch (sc_sa) {
	case LGM_MACSEC_SA_SC_4SA_8SC:
		return sc_idx << 2;
	case LGM_MACSEC_SA_SC_2SA_16SC:
		return sc_idx << 1;
	case LGM_MACSEC_SA_SC_1SA_32SC:
		return sc_idx;
	default:
		WARN_ONCE(true, "Invalid sc_sa");
	};

	return sc_idx;
}

static enum lgm_macsec_sc_sa sc_sa_from_num_an(const int num_an)
{
	enum lgm_macsec_sc_sa sc_sa = LGM_MACSEC_SA_SC_NOT_USED;

	switch (num_an) {
	case 4:
		sc_sa = LGM_MACSEC_SA_SC_4SA_8SC;
		break;
	case 2:
		sc_sa = LGM_MACSEC_SA_SC_2SA_16SC;
		break;
	case 1:
		sc_sa = LGM_MACSEC_SA_SC_1SA_32SC;
		break;
	default:
		break;
	}

	return sc_sa;
}

static int lgm_get_rxsc_idx_from_rxsc(struct lgm_macsec_cfg *macsec_cfg,
				     const struct macsec_rx_sc *rxsc)
{
	int i;

	if (unlikely(!rxsc))
		return -1;

	for (i = 0; i < LGM_MACSEC_MAX_SC; i++) {
		if (macsec_cfg->lgm_rxsc[i].sw_rxsc == rxsc)
			return i;
	}

	return -1;
}

/* There's a 1:1 mapping between SecY and Tx SC */
static int lgm_get_txsc_idx_from_secy(struct lgm_macsec_cfg *macsec_cfg,
				     const struct macsec_secy *secy)
{
	int i;

	if (unlikely(!secy))
		return -1;

	for (i = 0; i < LGM_MACSEC_MAX_SC; i++) {
		if (macsec_cfg->lgm_txsc[i].sw_secy == secy)
			return i;
	}
	return -1;
}

static int lgm_clear_rxsa(struct eth_priv *eth_msec, struct lgm_macsec_rxsc *lgm_rxsc,
			 const int sa_num, enum lgm_clear_type clear_type)
{
	int sa_idx = lgm_rxsc->hw_sc_idx | sa_num;
	struct clr_trans_rec pkt_xform_parms = { 0 };
	int ret = 0;

	if (clear_type & LGM_CLEAR_SW)
		clear_bit(sa_num, &lgm_rxsc->rx_sa_idx_busy);

	if ((clear_type & LGM_CLEAR_HW) && netif_carrier_ok(eth_msec->macsec_cfg->dev.ctx.netdev)) {
		pkt_xform_parms.sa_index = sa_idx;
		ret = eip160_clr_ing_sa_parms(&eth_msec->macsec_cfg->dev, &pkt_xform_parms);
		if (ret)
			return ret;
	}

	return ret;
}

static int lgm_clear_rxsc(struct eth_priv *eth_msec, const int rxsc_idx,
			 enum lgm_clear_type clear_type)
{
	struct lgm_macsec_rxsc *rx_sc = &eth_msec->macsec_cfg->lgm_rxsc[rxsc_idx];
	struct clr_sam_par pkt_clas_parms = { 0 };
        struct sam_enable_ctrl sam_ena_ctrl = { 0 };
        struct clr_sam_flow_ctrl pkt_match_actn = { 0 };
	int ret = 0;
	int sa_num;

	for_each_set_bit (sa_num, &rx_sc->rx_sa_idx_busy, LGM_MACSEC_MAX_SA) {
		ret = lgm_clear_rxsa(eth_msec, rx_sc, sa_num, clear_type);
		if (ret)
			return ret;
	}

	if (clear_type & LGM_CLEAR_HW) {
		sam_ena_ctrl.clear_enable = 1;
		sam_ena_ctrl.sam_index_clear = rx_sc->hw_sc_idx;

		ret = eip160_cfg_ing_sam_ena_ctrl(&eth_msec->macsec_cfg->dev, &sam_ena_ctrl);
		if (ret)
			return ret;

		pkt_clas_parms.rule_index = rxsc_idx;
		ret = eip160_clr_ing_sam_rule(&eth_msec->macsec_cfg->dev, &pkt_clas_parms);
		if (ret)
			return ret;

		pkt_match_actn.flow_index = rx_sc->hw_sc_idx;
		ret = eip160_clr_ing_sam_flow_ctrl(&eth_msec->macsec_cfg->dev, &pkt_match_actn);
		if (ret)
			return ret;
	}

	if (clear_type & LGM_CLEAR_SW) {
		clear_bit(rxsc_idx, &eth_msec->macsec_cfg->rxsc_idx_busy);
		rx_sc->sw_secy = NULL;
		rx_sc->sw_rxsc = NULL;
	}

	return ret;
}

static int lgm_clear_txsa(struct eth_priv *eth_msec, struct lgm_macsec_txsc *lgm_txsc,
			 const int sa_num, enum lgm_clear_type clear_type)
{
	const int sa_idx = lgm_txsc->hw_sc_idx | sa_num;
	int ret = 0;

	if (clear_type & LGM_CLEAR_SW)
		clear_bit(sa_num, &lgm_txsc->tx_sa_idx_busy);

	if ((clear_type & LGM_CLEAR_HW) && netif_carrier_ok(eth_msec->macsec_cfg->dev.ctx.netdev)) {
		struct clr_trans_rec pkt_xform_parms;

		pkt_xform_parms.sa_index = sa_idx;
		ret = eip160_clr_egr_sa_parms(&eth_msec->macsec_cfg->dev, &pkt_xform_parms);
		if (ret)
			return ret;
	}

	return 0;
}

static int lgm_clear_txsc(struct eth_priv *eth_msec, const int txsc_idx,
			 enum lgm_clear_type clear_type)
{
	struct lgm_macsec_txsc *tx_sc = &eth_msec->macsec_cfg->lgm_txsc[txsc_idx];
	struct clr_sam_par pkt_clas_parms = { 0 };
	struct sam_enable_ctrl sam_ena_ctrl = { 0 };
	struct clr_sam_flow_ctrl pkt_match_actn = { 0 };
	int ret = 0;
	int sa_num;

	for_each_set_bit (sa_num, &tx_sc->tx_sa_idx_busy, LGM_MACSEC_MAX_SA) {
		ret = lgm_clear_txsa(eth_msec, tx_sc, sa_num, clear_type);
		if (ret)
			return ret;
	}

	if (clear_type & LGM_CLEAR_HW) {
		sam_ena_ctrl.clear_enable = 1;
		sam_ena_ctrl.sam_index_clear = tx_sc->hw_sc_idx;

		ret = eip160_cfg_egr_sam_ena_ctrl(&eth_msec->macsec_cfg->dev, &sam_ena_ctrl);
		if (ret)
			return ret;

		pkt_clas_parms.rule_index = txsc_idx;
		ret = eip160_clr_egr_sam_rule(&eth_msec->macsec_cfg->dev, &pkt_clas_parms);
		if (ret)
			return ret;

		pkt_match_actn.flow_index = tx_sc->hw_sc_idx;
		ret = eip160_clr_egr_sam_flow_ctrl(&eth_msec->macsec_cfg->dev, &pkt_match_actn);
		if (ret)
			return ret;
	}

	if (clear_type & LGM_CLEAR_SW) {
		clear_bit(txsc_idx, &eth_msec->macsec_cfg->txsc_idx_busy);
		eth_msec->macsec_cfg->lgm_txsc[txsc_idx].sw_secy = NULL;
	}

	return ret;
}

static int lgm_clear_secy_cfg(struct eth_priv *eth_msec, const struct macsec_secy *secy,
			 enum lgm_clear_type clear_type)
{
	struct macsec_rx_sc *rx_sc;
	int txsc_idx;
	int rxsc_idx;
	int ret = 0;

	txsc_idx = lgm_get_txsc_idx_from_secy(eth_msec->macsec_cfg, secy);
	if (txsc_idx >= 0) {
		ret = lgm_clear_txsc(eth_msec, txsc_idx, clear_type);
		if (ret)
			return ret;
	}

	for (rx_sc = rcu_dereference_bh(secy->rx_sc); rx_sc;
	     rx_sc = rcu_dereference_bh(rx_sc->next)) {
		rxsc_idx = lgm_get_rxsc_idx_from_rxsc(eth_msec->macsec_cfg, rx_sc);
		if (rxsc_idx < 0)
			continue;

		ret = lgm_clear_rxsc(eth_msec, rxsc_idx, clear_type);
		if (ret)
			return ret;
	}

	return ret;
}

static int lgm_update_rxsa(struct eth_priv *eth_msec, const unsigned int sc_idx,
			  const struct macsec_secy *secy,
			  const struct macsec_rx_sa *rx_sa,
			  const unsigned char *key, const unsigned char an)
{
	const u64 next_pn = rx_sa->next_pn;
	struct transform_rec pkt_xform_parms = { 0 };
	const int sa_idx = sc_idx | an;
	int ret = 0;

	memset(&pkt_xform_parms, 0, sizeof(pkt_xform_parms));

	if (!key)
		return ret;

	pkt_xform_parms.ccw.an = secy->tx_sc.encoding_sa;
	if(secy->xpn)
		pkt_xform_parms.ccw.sn_type = SN_64_BIT;
	else
		pkt_xform_parms.ccw.sn_type = SN_32_BIT;

	switch (secy->key_len) {
	case LGM_MACSEC_KEY_LEN_128_BIT:
		pkt_xform_parms.ccw.ca_type = CA_AES_CTR_128;
		break;
	case LGM_MACSEC_KEY_LEN_256_BIT:
		pkt_xform_parms.ccw.ca_type = CA_AES_CTR_256;
		break;
	default:
		WARN_ONCE(true, "Invalid sc_sa");
		return -EINVAL;
	}

	memcpy(pkt_xform_parms.cp.key, key, secy->key_len);
	//put_unaligned_be64((__force u64)secy->sci, &pkt_xform_parms.cp.scid);
	memcpy((char *)&pkt_xform_parms.cp.scid, (char *)&secy->sci, MACSEC_SCI_LEN_BYTE);
	if(secy->xpn) {
		memcpy(pkt_xform_parms.cp.c_salt.salt, (char *)&rx_sa->key.salt, MACSEC_SALT_LEN_BYTE);
		memcpy(pkt_xform_parms.cp.c_salt.s_sci, (char *)&rx_sa->ssci, MACSEC_SSCI_LEN_BYTE);
	}
	pkt_xform_parms.pn_rc.seq_num = next_pn;
	if (secy->replay_protect) {
		pkt_xform_parms.pn_rc.mask = secy->replay_window;
	}

	pkt_xform_parms.sa_index = sa_idx;

	ret = eip160_cfg_ing_sa_parms(&eth_msec->macsec_cfg->dev, &pkt_xform_parms);
        if (ret)
                return ret;

#if PRINT_DEBUG
	memset(&pkt_xform_parms, 0, sizeof(pkt_xform_parms));
	ret = eip160_get_ing_sa_parms(&eth_msec->macsec_cfg->dev, &pkt_xform_parms);
	if (ret)
		return ret;
#endif
	return ret;
}

static int lgm_set_rxsc(struct eth_priv *eth_msec, const u32 rxsc_idx)
{
	const struct lgm_macsec_rxsc *lgm_rxsc = &eth_msec->macsec_cfg->lgm_rxsc[rxsc_idx];
	const struct macsec_rx_sc *rx_sc = lgm_rxsc->sw_rxsc;
	const struct macsec_secy *secy = lgm_rxsc->sw_secy;
	const u32 hw_sc_idx = lgm_rxsc->hw_sc_idx;
	struct sam_flow_ctrl pkt_match_actn = { 0 };
	struct sa_match_parm pkt_clas_parms = { 0 };
	struct sam_enable_ctrl sam_ena_ctrl = { 0 };
	int ret = 0;

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

	ret = eip160_cfg_ing_sam_flow_ctrl(&eth_msec->macsec_cfg->dev, &pkt_match_actn);
	if (ret)
		return ret;

	/* SA match params */
	memcpy(pkt_clas_parms.sa_mac, (char *)&rx_sc->sci, ETH_ALEN);
	//pkt_clas_parms.da_mac = { 0 };
	pkt_clas_parms.eth_type = ETH_P_MACSEC;
	//put_unaligned_be64((__force u64)rx_sc->sci, &pkt_clas_parms.scid);
	memcpy((char *)&pkt_clas_parms.scid, (char *)&rx_sc->sci, MACSEC_SCI_LEN_BYTE);
	/* Misc match params */
	pkt_clas_parms.misc_par.control_packet = 0;
	pkt_clas_parms.misc_par.untagged = 1;
	pkt_clas_parms.misc_par.tagged = 1;
	pkt_clas_parms.misc_par.bad_tag = 1;
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

	ret = eip160_cfg_ing_sam_rule(&eth_msec->macsec_cfg->dev, &pkt_clas_parms);
	if (ret)
		return ret;

#if PRINT_DEBUG
	memset(&pkt_match_actn, 0, sizeof(pkt_match_actn));
	memset(&pkt_clas_parms, 0, sizeof(pkt_clas_parms));
	ret = eip160_get_ing_sam_rule(&eth_msec->macsec_cfg->dev, &pkt_clas_parms);
	if (ret)
		return ret;

	ret = eip160_get_ing_sam_flow_ctrl(&eth_msec->macsec_cfg->dev, &pkt_match_actn);
	if (ret)
		return ret;
#endif

	sam_ena_ctrl.set_enable = 1;
	sam_ena_ctrl.sam_index_set = hw_sc_idx;

	ret = eip160_cfg_ing_sam_ena_ctrl(&eth_msec->macsec_cfg->dev, &sam_ena_ctrl);
	if (ret)
		return ret;

	return ret;
}

static int lgm_update_txsa(struct eth_priv *eth_msec, const unsigned int sc_idx,
			  const struct macsec_secy *secy,
			  const struct macsec_tx_sa *tx_sa,
			  const unsigned char *key, const unsigned char an)
{
	const u64 next_pn = tx_sa->next_pn;
	struct transform_rec pkt_xform_parms = { 0 };
	const unsigned int sa_idx = sc_idx | an;
	int ret = 0;

	memset(&pkt_xform_parms, 0, sizeof(pkt_xform_parms));

	if (!key)
		return ret;

	pkt_xform_parms.ccw.an = secy->tx_sc.encoding_sa;
	if(secy->xpn)
		pkt_xform_parms.ccw.sn_type = SN_64_BIT;
	else
		pkt_xform_parms.ccw.sn_type = SN_32_BIT;

	switch (secy->key_len) {
	case LGM_MACSEC_KEY_LEN_128_BIT:
		pkt_xform_parms.ccw.ca_type = CA_AES_CTR_128;
		break;
	case LGM_MACSEC_KEY_LEN_256_BIT:
		pkt_xform_parms.ccw.ca_type = CA_AES_CTR_256;
		break;
	default:
		WARN_ONCE(true, "Invalid sc_sa");
		return -EINVAL;
	}

	memcpy(pkt_xform_parms.cp.key, key, secy->key_len);
	//put_unaligned_be64((__force u64)secy->sci, &pkt_xform_parms.cp.scid);
	memcpy((char *)&pkt_xform_parms.cp.scid, (char *)&secy->sci, MACSEC_SCI_LEN_BYTE);
	if(secy->xpn) {
		memcpy(pkt_xform_parms.cp.c_salt.salt, (char *)&tx_sa->key.salt, MACSEC_SALT_LEN_BYTE);
		memcpy(pkt_xform_parms.cp.c_salt.s_sci, (char *)&tx_sa->ssci, MACSEC_SSCI_LEN_BYTE);
	}
	pkt_xform_parms.pn_rc.seq_num = next_pn;

	pkt_xform_parms.sa_cw.sa_index = sa_idx;
	pkt_xform_parms.sa_cw.next_sa_index = sa_idx + 1;
	pkt_xform_parms.sa_cw.sa_expired_irq = 1;
	pkt_xform_parms.sa_cw.sa_index_valid = pkt_xform_parms.sa_cw.sa_ind_update_en = tx_sa->active;
	pkt_xform_parms.sa_cw.flow_index = sa_idx;

	pkt_xform_parms.sa_index = sa_idx;

	ret = eip160_cfg_egr_sa_parms(&eth_msec->macsec_cfg->dev, &pkt_xform_parms);
        if (ret)
                return ret;

#if PRINT_DEBUG
	memset(&pkt_xform_parms, 0, sizeof(pkt_xform_parms));
	ret = eip160_get_egr_sa_parms(&eth_msec->macsec_cfg->dev, &pkt_xform_parms);
	if (ret)
		return ret;
#endif

	return ret;
}

static int lgm_set_txsc(struct eth_priv *eth_msec, const int txsc_idx)
{
	struct lgm_macsec_txsc *lgm_txsc = &eth_msec->macsec_cfg->lgm_txsc[txsc_idx];
	const struct macsec_secy *secy = lgm_txsc->sw_secy;
	struct sam_flow_ctrl pkt_match_actn = { 0 };
	struct sa_match_parm pkt_clas_parms = { 0 };
	struct sam_enable_ctrl sam_ena_ctrl = { 0 };
	unsigned int sc_idx = lgm_txsc->hw_sc_idx;
	int ret = 0;

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

	ret = eip160_cfg_egr_sam_flow_ctrl(&eth_msec->macsec_cfg->dev, &pkt_match_actn);
	if (ret)
		return ret;

	/* SA match params */
	memcpy(pkt_clas_parms.sa_mac, secy->netdev->dev_addr, ETH_ALEN); 
	//pkt_clas_parms.da_mac = { 0 };
	pkt_clas_parms.eth_type = 0x0000;
	pkt_clas_parms.scid = 0;
	/* Misc match params */
	pkt_clas_parms.misc_par.control_packet = 0;
	pkt_clas_parms.misc_par.untagged = 1;
	pkt_clas_parms.misc_par.tagged = 1;
	pkt_clas_parms.misc_par.bad_tag = 1;
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
	pkt_clas_parms.flow_index = sc_idx;
	pkt_clas_parms.rule_index = sc_idx;

	ret = eip160_cfg_egr_sam_rule(&eth_msec->macsec_cfg->dev, &pkt_clas_parms);
	if (ret)
		return ret;

#if PRINT_DEBUG
	memset(&pkt_match_actn, 0, sizeof(pkt_match_actn));
	memset(&pkt_clas_parms, 0, sizeof(pkt_clas_parms));
	ret = eip160_get_egr_sam_rule(&eth_msec->macsec_cfg->dev, &pkt_clas_parms);
	if (ret)
		return ret;

	ret = eip160_get_egr_sam_flow_ctrl(&eth_msec->macsec_cfg->dev, &pkt_match_actn);
	if (ret)
		return ret;
#endif

	sam_ena_ctrl.set_enable = 1;
	sam_ena_ctrl.sam_index_set = sc_idx;

	ret = eip160_cfg_egr_sam_ena_ctrl(&eth_msec->macsec_cfg->dev, &sam_ena_ctrl);
	if (ret)
		return ret;

	return ret;
}

static int lgm_apply_rxsc_cfg(struct eth_priv *eth_msec, const int rxsc_idx)
{
	struct lgm_macsec_rxsc *lgm_rxsc = &eth_msec->macsec_cfg->lgm_rxsc[rxsc_idx];
	const struct macsec_secy *secy = lgm_rxsc->sw_secy;
	struct macsec_rx_sa *rx_sa;
	int ret = 0;
	int i;

	if (!netif_running(secy->netdev))
		return ret;

	for (i = 0; i < MACSEC_NUM_AN; i++) {
		rx_sa = rcu_dereference_bh(lgm_rxsc->sw_rxsc->sa[i]);
		if (rx_sa) {
			ret = lgm_update_rxsa(eth_msec, lgm_rxsc->hw_sc_idx, secy,
					     rx_sa, lgm_rxsc->rx_sa_key[i], i);
			if (ret)
				return ret;
		}
	}

	ret = lgm_set_rxsc(eth_msec, rxsc_idx);
	if (ret)
		return ret;

	return ret;
}

static int lgm_apply_txsc_cfg(struct eth_priv *eth_msec, const int txsc_idx)
{
	struct lgm_macsec_txsc *lgm_txsc = &eth_msec->macsec_cfg->lgm_txsc[txsc_idx];
	const struct macsec_secy *secy = lgm_txsc->sw_secy;
	struct macsec_tx_sa *tx_sa;
	int ret = 0;
	int i;

	if (!netif_running(secy->netdev))
		return ret;

	for (i = 0; i < MACSEC_NUM_AN; i++) {
		tx_sa = rcu_dereference_bh(secy->tx_sc.sa[i]);
		if (tx_sa) {
			ret = lgm_update_txsa(eth_msec, lgm_txsc->hw_sc_idx, secy,
					     tx_sa, lgm_txsc->tx_sa_key[i], i);
			if (ret)
				return ret;
		}
	}

	ret = lgm_set_txsc(eth_msec, txsc_idx);
	if (ret)
		return ret;

	return ret;
}

static int lgm_apply_secy_cfg(struct eth_priv *eth_msec,
			     const struct macsec_secy *secy)
{
	struct macsec_rx_sc *rx_sc;
	int txsc_idx;
	int rxsc_idx;
	int ret = 0;

	txsc_idx = lgm_get_txsc_idx_from_secy(eth_msec->macsec_cfg, secy);
	if (txsc_idx >= 0)
		lgm_apply_txsc_cfg(eth_msec, txsc_idx);

	for (rx_sc = rcu_dereference_bh(secy->rx_sc); rx_sc && rx_sc->active;
	     rx_sc = rcu_dereference_bh(rx_sc->next)) {
		rxsc_idx = lgm_get_rxsc_idx_from_rxsc(eth_msec->macsec_cfg, rx_sc);
		if (unlikely(rxsc_idx < 0))
			continue;

		ret = lgm_apply_rxsc_cfg(eth_msec, rxsc_idx);
		if (ret)
			return ret;
	}

	return ret;
}

static int lgm_eth_macsec_get_rx_sa_stats(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	struct lgm_macsec_cfg *cfg = eth_msec->macsec_cfg;
	struct lgm_macsec_rxsc *lgm_rxsc;
	const struct macsec_secy *secy = ctx->secy;
	struct macsec_rx_sa_stats *ig_sa_stats;
	struct macsec_rx_sa *rx_sa;
	struct sa_next_pn next_pn;
	unsigned int sa_idx;
	int rxsc_idx;
	int ret;

	rxsc_idx = lgm_get_rxsc_idx_from_rxsc(cfg, ctx->rx_sc);
	if (rxsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare) {
		if (netif_carrier_ok(ctx->netdev) && netif_running(secy->netdev))
			return 0;
		return -ENODEV;
	}

	lgm_rxsc = &cfg->lgm_rxsc[rxsc_idx];
	ig_sa_stats = &lgm_rxsc->rx_sa_stats[ctx->sa.assoc_num];

	ctx->stats.rx_sa_stats->InPktsOK = ig_sa_stats->InPktsOK;
	ctx->stats.rx_sa_stats->InPktsInvalid = ig_sa_stats->InPktsInvalid;
	ctx->stats.rx_sa_stats->InPktsNotValid = ig_sa_stats->InPktsNotValid;
	ctx->stats.rx_sa_stats->InPktsNotUsingSA = ig_sa_stats->InPktsNotUsingSA;
	ctx->stats.rx_sa_stats->InPktsUnusedSA = ig_sa_stats->InPktsUnusedSA;
#if PRINT_DEBUG
	pr_debug("\t%40s:\t%u\n", "InPktsOK", ctx->stats.rx_sa_stats->InPktsOK);
	pr_debug("\t%40s:\t%u\n", "InPktsInvalid", ctx->stats.rx_sa_stats->InPktsInvalid);
	pr_debug("\t%40s:\t%u\n", "InPktsNotValid", ctx->stats.rx_sa_stats->InPktsNotValid);
	pr_debug("\t%40s:\t%u\n", "InPktsNotUsingSA", ctx->stats.rx_sa_stats->InPktsNotUsingSA);
	pr_debug("\t%40s:\t%u\n", "InPktsUnusedSA", ctx->stats.rx_sa_stats->InPktsUnusedSA);
#endif
	rx_sa = rcu_dereference_bh(lgm_rxsc->sw_rxsc->sa[ctx->sa.assoc_num]);
	sa_idx = lgm_rxsc->hw_sc_idx | ctx->sa.assoc_num;
	next_pn.sa_index = sa_idx;
	ret = eip160_get_ing_sa_next_pn(&eth_msec->macsec_cfg->dev, &next_pn);
	if (ret == 0) {
		spin_lock_bh(&rx_sa->lock);
		rx_sa->next_pn = next_pn.next_pn;
		spin_unlock_bh(&rx_sa->lock);
	}

	return ret;
}

static int lgm_eth_macsec_get_rx_sc_stats(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	struct lgm_macsec_cfg *cfg = eth_msec->macsec_cfg;
	struct lgm_macsec_rxsc *lgm_rxsc;
	const struct macsec_secy *secy = ctx->secy;
	struct macsec_rx_sa_stats *ig_sa_stats;
	struct macsec_rx_sc_stats *rx_sc_stats;
	struct ing_sa_stats _ig_sa_stats;
	unsigned int sa_idx;
	int rxsc_idx;
	int ret = 0;
	int i;

	rxsc_idx = lgm_get_rxsc_idx_from_rxsc(cfg, ctx->rx_sc);
	if (rxsc_idx < 0)
		return -ENOENT;

	if (ctx->prepare) {
		if (netif_carrier_ok(ctx->netdev) && netif_running(secy->netdev))
			return 0;
		return -ENODEV;
	}

	memset(&_ig_sa_stats, 0, sizeof(struct ing_sa_stats));
	lgm_rxsc = &cfg->lgm_rxsc[rxsc_idx];
	rx_sc_stats = &lgm_rxsc->rx_sc_stats;

	for (i = 0; i < MACSEC_NUM_AN; i++) {
		if (!test_bit(i, &lgm_rxsc->rx_sa_idx_busy))
			continue;

		sa_idx = lgm_rxsc->hw_sc_idx | i;
		_ig_sa_stats.rule_index = sa_idx;
		if (eip160_get_ing_sa_stats(&eth_msec->macsec_cfg->dev, &_ig_sa_stats))
			return -EIO;

		ig_sa_stats = &lgm_rxsc->rx_sa_stats[i];

		if (ctx->secy->protect_frames)
			rx_sc_stats->InOctetsValidated += _ig_sa_stats.InOctetsValidated;
		else if(ctx->secy->tx_sc.encrypt)
			rx_sc_stats->InOctetsDecrypted += _ig_sa_stats.InOctetsDecrypted;

		rx_sc_stats->InPktsUnchecked += _ig_sa_stats.InPktsUnchecked;
		rx_sc_stats->InPktsDelayed += _ig_sa_stats.InPktsDelayed;
		rx_sc_stats->InPktsLate += _ig_sa_stats.InPktsLate;

		ig_sa_stats->InPktsOK += _ig_sa_stats.InPktsOK;
		ig_sa_stats->InPktsInvalid += _ig_sa_stats.InPktsInvalid;
		ig_sa_stats->InPktsNotValid += _ig_sa_stats.InPktsNotValid;
		ig_sa_stats->InPktsNotUsingSA += _ig_sa_stats.InPktsNotUsingSA;
		ig_sa_stats->InPktsUnusedSA += _ig_sa_stats.InPktsUnusedSA;


		if(ctx->secy->tx_sc.encrypt)
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
#if PRINT_DEBUG
	pr_debug("\t%40s:\t%llu\n", "InOctetsValidated", ctx->stats.rx_sc_stats->InOctetsValidated);
	pr_debug("\t%40s:\t%llu\n", "InOctetsDecrypted", ctx->stats.rx_sc_stats->InOctetsDecrypted);
	pr_debug("\t%40s:\t%llu\n", "InPktsUnchecked", ctx->stats.rx_sc_stats->InPktsUnchecked);
	pr_debug("\t%40s:\t%llu\n", "InPktsDelayed", ctx->stats.rx_sc_stats->InPktsDelayed);
	pr_debug("\t%40s:\t%llu\n", "InPktsOK", ctx->stats.rx_sc_stats->InPktsOK);
	pr_debug("\t%40s:\t%llu\n", "InPktsInvalid", ctx->stats.rx_sc_stats->InPktsInvalid);
	pr_debug("\t%40s:\t%llu\n", "InPktsLate", ctx->stats.rx_sc_stats->InPktsLate);
	pr_debug("\t%40s:\t%llu\n", "InPktsNotValid", ctx->stats.rx_sc_stats->InPktsNotValid);
	pr_debug("\t%40s:\t%llu\n", "InPktsNotUsingSA", ctx->stats.rx_sc_stats->InPktsNotUsingSA);
	pr_debug("\t%40s:\t%llu\n", "InPktsUnusedSA", ctx->stats.rx_sc_stats->InPktsUnusedSA);
#endif
	return ret;
}

static int lgm_eth_macsec_get_tx_sa_stats(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	struct lgm_macsec_cfg *cfg = eth_msec->macsec_cfg;
	const struct macsec_secy *secy = ctx->secy;
	struct lgm_macsec_txsc *lgm_txsc;
	struct macsec_tx_sa_stats *eg_sa_stats;
	struct macsec_tx_sa *tx_sa;
	struct sa_next_pn next_pn;
	unsigned int sa_idx;
	int txsc_idx;
	int ret;

	txsc_idx = lgm_get_txsc_idx_from_secy(cfg, ctx->secy);
	if (txsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare) {
		if (netif_carrier_ok(ctx->netdev) && netif_running(secy->netdev))
			return 0;
		return -ENODEV;
	}

	lgm_txsc = &cfg->lgm_txsc[txsc_idx];
	eg_sa_stats = &lgm_txsc->tx_sa_stats[ctx->sa.assoc_num];

	if(ctx->secy->tx_sc.encrypt)
		ctx->stats.tx_sa_stats->OutPktsEncrypted = eg_sa_stats->OutPktsEncrypted;
	else if(ctx->secy->protect_frames)
		ctx->stats.tx_sa_stats->OutPktsProtected = eg_sa_stats->OutPktsProtected;
#if PRINT_DEBUG
	pr_debug("\t%40s:\t%u\n", "OutPktsProtected", ctx->stats.tx_sa_stats->OutPktsProtected);
	pr_debug("\t%40s:\t%u\n", "OutPktsEncrypted", ctx->stats.tx_sa_stats->OutPktsEncrypted);
#endif

	secy = lgm_txsc->sw_secy;
	tx_sa = rcu_dereference_bh(secy->tx_sc.sa[ctx->sa.assoc_num]);
	sa_idx = lgm_txsc->hw_sc_idx | ctx->sa.assoc_num;
	next_pn.sa_index = sa_idx;
	ret = eip160_get_egr_sa_next_pn(&eth_msec->macsec_cfg->dev, &next_pn);
	if (ret == 0) {
		spin_lock_bh(&tx_sa->lock);
		tx_sa->next_pn = next_pn.next_pn;
		spin_unlock_bh(&tx_sa->lock);
	}

	return ret;
}

static int lgm_eth_macsec_get_tx_sc_stats(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	struct lgm_macsec_txsc *lgm_txsc;
	struct macsec_tx_sa_stats *eg_sa_stats;
	struct macsec_tx_sc_stats *tx_sc_stats;
	struct egr_sa_stats _eg_sa_stats;
	int txsc_idx;
	const struct macsec_secy *secy = ctx->secy;

	txsc_idx = lgm_get_txsc_idx_from_secy(eth_msec->macsec_cfg, ctx->secy);
	if (txsc_idx < 0)
		return -ENOENT;

	if (ctx->prepare) {
		if (netif_carrier_ok(ctx->netdev) && netif_running(secy->netdev))
			return 0;
		return -ENODEV;
	}

	memset(&_eg_sa_stats, 0, sizeof(struct egr_sa_stats));
	_eg_sa_stats.rule_index = txsc_idx;
	if (eip160_get_egr_sa_stats(&eth_msec->macsec_cfg->dev, &_eg_sa_stats))
		return -EIO;

	lgm_txsc = &eth_msec->macsec_cfg->lgm_txsc[txsc_idx];
	tx_sc_stats =  &lgm_txsc->tx_sc_stats;
	eg_sa_stats = &lgm_txsc->tx_sa_stats[ctx->sa.assoc_num];

	tx_sc_stats->OutOctetsProtected += _eg_sa_stats.OutOctetsProtected;
	tx_sc_stats->OutOctetsEncrypted += _eg_sa_stats.OutOctetsEncrypted;

	eg_sa_stats->OutPktsEncrypted += _eg_sa_stats.OutPktsEncrypted;
	eg_sa_stats->OutPktsProtected += _eg_sa_stats.OutPktsProtected;
	
	if(ctx->secy->tx_sc.encrypt) {
		ctx->stats.tx_sc_stats->OutPktsEncrypted = eg_sa_stats->OutPktsEncrypted;
		ctx->stats.tx_sc_stats->OutOctetsEncrypted = tx_sc_stats->OutOctetsEncrypted;
	} else if(ctx->secy->protect_frames) {
		ctx->stats.tx_sc_stats->OutPktsProtected = eg_sa_stats->OutPktsProtected;
		ctx->stats.tx_sc_stats->OutOctetsProtected = tx_sc_stats->OutOctetsProtected;
	}
#if PRINT_DEBUG
	pr_debug("\t%40s:\t%llu\n", "OutPktsProtected", ctx->stats.tx_sc_stats->OutPktsProtected);
	pr_debug("\t%40s:\t%llu\n", "OutPktsEncrypted", ctx->stats.tx_sc_stats->OutPktsEncrypted);
	pr_debug("\t%40s:\t%llu\n", "OutOctetsProtected", ctx->stats.tx_sc_stats->OutOctetsProtected);
	pr_debug("\t%40s:\t%llu\n", "OutOctetsEncrypted", ctx->stats.tx_sc_stats->OutOctetsEncrypted);
#endif
	return 0;
}

static int lgm_eth_macsec_get_dev_stats(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	struct macsec_dev_stats *glb_stats = &eth_msec->macsec_cfg->glb_stats;
	struct ing_global_stats _ig_glb_stats;
	struct egr_global_stats _eg_glb_stats;
	const struct macsec_secy *secy = ctx->secy;

	if (ctx->prepare) {
		if (netif_carrier_ok(ctx->netdev) && netif_running(secy->netdev))
			return 0;
		return -ENODEV;
	}

	memset(&_ig_glb_stats, 0, sizeof(struct ing_global_stats));
	memset(&_eg_glb_stats, 0, sizeof(struct egr_global_stats));
	if (eip160_get_ing_global_stats(&eth_msec->macsec_cfg->dev, &_ig_glb_stats))
		return -EIO;

	if (eip160_get_egr_global_stats(&eth_msec->macsec_cfg->dev, &_eg_glb_stats))
		return -EIO;
#if PRINT_DEBUG
	pr_debug("\nRX GLOBAL Counters:\n----------------\n");
	pr_debug("\t%40s:\t%llu\n", "TransformErrorPkts", _ig_glb_stats.TransformErrorPkts);
	pr_debug("\t%40s:\t%llu\n", "InPktsCtrl", _ig_glb_stats.InPktsCtrl);
	pr_debug("\t%40s:\t%llu\n", "InPktsTagged", _ig_glb_stats.InPktsTagged);
	pr_debug("\t%40s:\t%llu\n", "InPktsUntaggedMiss", _ig_glb_stats.InPktsUntaggedMiss);
	pr_debug("\t%40s:\t%llu\n", "InOverSizePkts", _ig_glb_stats.InOverSizePkts);
	pr_debug("\nTX GLOBAL Counters:\n----------------\n");
	pr_debug("\t%40s:\t%llu\n", "TransformErrorPkts", eg_glb_stats->TransformErrorPkts);
	pr_debug("\t%40s:\t%llu\n", "OutPktsCtrl", eg_glb_stats->OutPktsCtrl);
	pr_debug("\t%40s:\t%llu\n", "OutPktsUnknownSA", eg_glb_stats->OutPktsUnknownSA);
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
	ctx->stats.dev_stats->InPktsOverrun = 0; //This condition cannot occur.
#if PRINT_DEBUG
	pr_debug("\t%40s:\t%llu\n", "OutPktsUntagged", ctx->stats.dev_stats->OutPktsUntagged);
	pr_debug("\t%40s:\t%llu\n", "OutPktsTooLong", ctx->stats.dev_stats->OutPktsTooLong);
	pr_debug("\t%40s:\t%llu\n", "InPktsUntagged", ctx->stats.dev_stats->InPktsUntagged);
	pr_debug("\t%40s:\t%llu\n", "InPktsNoTag", ctx->stats.dev_stats->InPktsNoTag);
	pr_debug("\t%40s:\t%llu\n", "InPktsBadTag", ctx->stats.dev_stats->InPktsBadTag);
	pr_debug("\t%40s:\t%llu\n", "InPktsUnknownSCI", ctx->stats.dev_stats->InPktsUnknownSCI);
	pr_debug("\t%40s:\t%llu\n", "InPktsNoSCI", ctx->stats.dev_stats->InPktsNoSCI);
	pr_debug("\t%40s:\t%llu\n", "InPktsOverrun", ctx->stats.dev_stats->InPktsOverrun);
#endif
	return 0;
}

static int lgm_eth_macsec_del_txsa(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	struct lgm_macsec_cfg *cfg = eth_msec->macsec_cfg;
	int txsc_idx;
	int ret = 0;

	txsc_idx = lgm_get_txsc_idx_from_secy(cfg, ctx->secy);
	if (txsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	ret = lgm_clear_txsa(eth_msec, &cfg->lgm_txsc[txsc_idx], ctx->sa.assoc_num,
			    LGM_CLEAR_ALL);

	return ret;
}

static int lgm_eth_macsec_upd_txsa(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	struct lgm_macsec_cfg *cfg = eth_msec->macsec_cfg;
	const struct macsec_secy *secy = ctx->secy;
	struct lgm_macsec_txsc *lgm_txsc;
	int txsc_idx;
	int ret = 0;

	txsc_idx = lgm_get_txsc_idx_from_secy(cfg, secy);
	if (txsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	lgm_txsc = &cfg->lgm_txsc[txsc_idx];
	if (netif_carrier_ok(ctx->netdev) && netif_running(secy->netdev))
		ret = lgm_update_txsa(eth_msec, lgm_txsc->hw_sc_idx, secy,
				     ctx->sa.tx_sa, NULL, ctx->sa.assoc_num);

	return ret;
}

static int lgm_eth_macsec_add_txsa(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	struct lgm_macsec_cfg *cfg = eth_msec->macsec_cfg;
	const struct macsec_secy *secy = ctx->secy;
	struct lgm_macsec_txsc *lgm_txsc;
	int txsc_idx;
	int ret = 0;

	txsc_idx = lgm_get_txsc_idx_from_secy(cfg, secy);
	if (txsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	lgm_txsc = &cfg->lgm_txsc[txsc_idx];
	set_bit(ctx->sa.assoc_num, &lgm_txsc->tx_sa_idx_busy);

	memcpy(lgm_txsc->tx_sa_key[ctx->sa.assoc_num], ctx->sa.key,
	       secy->key_len);

	if (netif_carrier_ok(ctx->netdev) && netif_running(secy->netdev))
		ret = lgm_update_txsa(eth_msec, lgm_txsc->hw_sc_idx, secy,
				     ctx->sa.tx_sa, ctx->sa.key,
				     ctx->sa.assoc_num);

	return ret;
}

static int lgm_eth_macsec_del_rxsa(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	const struct macsec_rx_sc *rx_sc = ctx->sa.rx_sa->sc;
	struct lgm_macsec_cfg *cfg = eth_msec->macsec_cfg;
	int rxsc_idx;
	int ret = 0;

	rxsc_idx = lgm_get_rxsc_idx_from_rxsc(cfg, rx_sc);
	if (rxsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	ret = lgm_clear_rxsa(eth_msec, &cfg->lgm_rxsc[rxsc_idx], ctx->sa.assoc_num,
			    LGM_CLEAR_ALL);

	return ret;
}

static int lgm_eth_macsec_upd_rxsa(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	const struct macsec_rx_sc *rx_sc = ctx->sa.rx_sa->sc;
	struct lgm_macsec_cfg *cfg = eth_msec->macsec_cfg;
	const struct macsec_secy *secy = ctx->secy;
	int rxsc_idx;
	int ret = 0;

	rxsc_idx = lgm_get_rxsc_idx_from_rxsc(cfg, rx_sc);
	if (rxsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	if (netif_carrier_ok(ctx->netdev) && netif_running(secy->netdev))
		ret = lgm_update_rxsa(eth_msec, cfg->lgm_rxsc[rxsc_idx].hw_sc_idx,
				     secy, ctx->sa.rx_sa, NULL,
				     ctx->sa.assoc_num);

	return ret;
}

static int lgm_eth_macsec_add_rxsa(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	const struct macsec_rx_sc *rx_sc = ctx->sa.rx_sa->sc;
	const struct macsec_secy *secy = ctx->secy;
	struct lgm_macsec_rxsc *lgm_rxsc;
	int rxsc_idx;
	int ret = 0;

	rxsc_idx = lgm_get_rxsc_idx_from_rxsc(eth_msec->macsec_cfg, rx_sc);
	if (rxsc_idx < 0)
		return -EINVAL;

	if (ctx->prepare)
		return 0;

	lgm_rxsc = &eth_msec->macsec_cfg->lgm_rxsc[rxsc_idx];
	set_bit(ctx->sa.assoc_num, &lgm_rxsc->rx_sa_idx_busy);

	memcpy(lgm_rxsc->rx_sa_key[ctx->sa.assoc_num], ctx->sa.key,
	       secy->key_len);

	if (netif_carrier_ok(ctx->netdev) && netif_running(secy->netdev))
		ret = lgm_update_rxsa(eth_msec, lgm_rxsc->hw_sc_idx, secy,
				     ctx->sa.rx_sa, ctx->sa.key,
				     ctx->sa.assoc_num);

	return ret;
}

static int lgm_eth_macsec_del_rxsc(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	enum lgm_clear_type clear_type = LGM_CLEAR_SW;
	int rxsc_idx;
	int ret = 0;

	rxsc_idx = lgm_get_rxsc_idx_from_rxsc(eth_msec->macsec_cfg, ctx->rx_sc);
	if (rxsc_idx < 0)
		return -ENOENT;

	if (ctx->prepare)
		return 0;

	if (netif_carrier_ok(ctx->netdev))
		clear_type = LGM_CLEAR_ALL;

	ret = lgm_clear_rxsc(eth_msec, rxsc_idx, clear_type);

	return ret;
}


static int lgm_eth_macsec_upd_rxsc(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	int rxsc_idx;
	int ret = 0;

	rxsc_idx = lgm_get_rxsc_idx_from_rxsc(eth_msec->macsec_cfg, ctx->rx_sc);
	if (rxsc_idx < 0)
		return -ENOENT;

	if (ctx->prepare)
		return 0;

	if (netif_carrier_ok(ctx->netdev) && netif_running(ctx->secy->netdev))
		ret = lgm_set_rxsc(eth_msec, rxsc_idx);

	return ret;
}

static int lgm_eth_macsec_add_rxsc(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	struct lgm_macsec_cfg *cfg = eth_msec->macsec_cfg;
	const u32 rxsc_idx_max = lgm_sc_idx_max(cfg->sc_sa);
	u32 rxsc_idx;
	int ret = 0;

	if (hweight32(cfg->rxsc_idx_busy) >= rxsc_idx_max)
		return -ENOSPC;

	rxsc_idx = ffz(cfg->rxsc_idx_busy);
	if (rxsc_idx >= rxsc_idx_max)
		return -ENOSPC;

	if (ctx->prepare)
		return 0;

	cfg->lgm_rxsc[rxsc_idx].hw_sc_idx = lgm_to_hw_sc_idx(rxsc_idx,
							   cfg->sc_sa);
	cfg->lgm_rxsc[rxsc_idx].sw_secy = ctx->secy;
	cfg->lgm_rxsc[rxsc_idx].sw_rxsc = ctx->rx_sc;

	if (netif_carrier_ok(ctx->netdev) && netif_running(ctx->secy->netdev))
		ret = lgm_set_rxsc(eth_msec, rxsc_idx);

	if (ret < 0)
		return ret;

	set_bit(rxsc_idx, &cfg->rxsc_idx_busy);

	return 0;
}

static int lgm_eth_macsec_del_secy(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	int ret = 0;

	if (ctx->prepare)
		return 0;

	if (!eth_msec->macsec_cfg)
		return 0;

	ret = lgm_clear_secy_cfg(eth_msec, ctx->secy, LGM_CLEAR_ALL);

	return ret;
}

static int lgm_eth_macsec_upd_secy(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	const struct macsec_secy *secy = ctx->secy;
	int txsc_idx;
	int ret = 0;

	txsc_idx = lgm_get_txsc_idx_from_secy(eth_msec->macsec_cfg, secy);
	if (txsc_idx < 0)
		return -ENOENT;

	if (ctx->prepare)
		return 0;

	if (netif_carrier_ok(ctx->netdev) && netif_running(secy->netdev))
		ret = lgm_set_txsc(eth_msec, txsc_idx);

	return ret;
}

static int lgm_eth_macsec_add_secy(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	struct lgm_macsec_cfg *cfg = eth_msec->macsec_cfg;
	const struct macsec_secy *secy = ctx->secy;
	enum lgm_macsec_sc_sa sc_sa;
	u32 txsc_idx;
	int ret = 0;

	//if (secy->xpn)
		//return -EOPNOTSUPP;

	sc_sa = sc_sa_from_num_an(MACSEC_NUM_AN);
	if (sc_sa == LGM_MACSEC_SA_SC_NOT_USED)
		return -EINVAL;

	if (hweight32(cfg->txsc_idx_busy) >= lgm_sc_idx_max(sc_sa))
		return -ENOSPC;

	txsc_idx = ffz(cfg->txsc_idx_busy);
	if (txsc_idx == LGM_MACSEC_MAX_SC)
		return -ENOSPC;

	if (ctx->prepare)
		return 0;

	cfg->sc_sa = sc_sa;
	cfg->lgm_txsc[txsc_idx].hw_sc_idx = lgm_to_hw_sc_idx(txsc_idx, sc_sa);
	cfg->lgm_txsc[txsc_idx].sw_secy = secy;
#if PRINT_DEBUG
	pr_debug("sc_sa = %d\n", sc_sa);
	pr_debug("available first index = %d\n", txsc_idx);
	pr_debug("hw_sc_idx = %d\n", cfg->lgm_txsc[txsc_idx].hw_sc_idx);
#endif
	if (netif_carrier_ok(ctx->netdev) && netif_running(secy->netdev))
		ret = lgm_set_txsc(eth_msec, txsc_idx);

	set_bit(txsc_idx, &cfg->txsc_idx_busy);

	return 0;
}

static int lgm_eth_macsec_dev_stop(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	int i;

	if (ctx->prepare)
		return 0;

	if (!eth_msec->macsec_cfg->initialized)
		return 0;

	for (i = 0; i < LGM_MACSEC_MAX_SC; i++) {
		if (eth_msec->macsec_cfg->txsc_idx_busy & BIT(i))
			lgm_clear_secy_cfg(eth_msec, eth_msec->macsec_cfg->lgm_txsc[i].sw_secy,
				      LGM_CLEAR_HW);
	}

/*
	ret = macsec_disable_all_rules(&eth_msec->macsec_cfg->dev);
	if (ret < 0) {
		pr_err("\nERROR: Deleting static rules failed.\n");
		return ret;
	}
*/
	eth_msec->macsec_cfg->dev.unprepare(ctx);
	eth_msec->macsec_cfg->initialized = false;

	return 0;
}

static int lgm_eth_macsec_dev_open(struct macsec_context *ctx)
{
	struct eth_priv *eth_msec = netdev_priv(ctx->netdev);
	int ret = 0;

	if (ctx->prepare)
		return 0;

	if (eth_msec->macsec_cfg->initialized)
		/* only 1 SecY is supported */
		return -EEXIST;

	ret = eth_msec->macsec_cfg->dev.prepare(ctx, NULL, NULL);
	if (ret < 0)
		return ret;

	/* MACsec module initialization */
	ret = eip160_init_ing_dev(&eth_msec->macsec_cfg->dev);
	if (ret < 0) {
		pr_err("ERROR: Init ING dev failed.\n");
		return -EFAULT;
	}

	ret = eip160_init_egr_dev(&eth_msec->macsec_cfg->dev);
	if (ret < 0) {
		pr_err("ERROR: Init EGR dev failed.\n");
		return -EFAULT;
	}

/*
	ret = macsec_load_static_rules(&eth_msec->macsec_cfg->dev);
	if (ret < 0) {
		pr_err("\nERROR: Adding static rules failed.\n");
		goto ERROR;
	}
*/
	eth_msec->macsec_cfg->initialized = true;

	if (netif_carrier_ok(ctx->netdev))
		ret = lgm_apply_secy_cfg(eth_msec, ctx->secy);

	return ret;
}

/**
 * struct macsec_ops - MACsec offloading operations
 */
const struct macsec_ops lgm_eth_macsec_ops = {
	/* Device wide */
	.mdo_dev_open = lgm_eth_macsec_dev_open,
	.mdo_dev_stop = lgm_eth_macsec_dev_stop,
	/* SecY */
	.mdo_add_secy = lgm_eth_macsec_add_secy,
	.mdo_upd_secy = lgm_eth_macsec_upd_secy,
	.mdo_del_secy = lgm_eth_macsec_del_secy,
	/* Security channels */
	.mdo_add_rxsc = lgm_eth_macsec_add_rxsc,
	.mdo_upd_rxsc = lgm_eth_macsec_upd_rxsc,
	.mdo_del_rxsc = lgm_eth_macsec_del_rxsc,
	/* Rx Security associations */
	.mdo_add_rxsa = lgm_eth_macsec_add_rxsa,
	.mdo_upd_rxsa = lgm_eth_macsec_upd_rxsa,
	.mdo_del_rxsa = lgm_eth_macsec_del_rxsa,
	/* Tx Security associations */
	.mdo_add_txsa = lgm_eth_macsec_add_txsa,
	.mdo_upd_txsa = lgm_eth_macsec_upd_txsa,
	.mdo_del_txsa = lgm_eth_macsec_del_txsa,
	/* Statistics */
	.mdo_get_dev_stats = lgm_eth_macsec_get_dev_stats,
	.mdo_get_tx_sc_stats = lgm_eth_macsec_get_tx_sc_stats,
	.mdo_get_tx_sa_stats = lgm_eth_macsec_get_tx_sa_stats,
	.mdo_get_rx_sc_stats = lgm_eth_macsec_get_rx_sc_stats,
	.mdo_get_rx_sa_stats = lgm_eth_macsec_get_rx_sa_stats,
};

int eip160_register(const struct eip160_pdev *pdev)
{
	struct lgm_macsec_cfg *cfg;
	struct eth_priv *eth_msec;

	pr_debug("entering %s\n", __func__);
	if (!pdev
	    || !pdev->prepare || !pdev->unprepare
	    || !pdev->ig_irq_ena || !pdev->ig_irq_dis
	    || !pdev->ig_reg_rd || !pdev->ig_reg_wr
	    || !pdev->eg_irq_ena || !pdev->eg_irq_dis
	    || !pdev->eg_reg_rd || !pdev->eg_reg_wr) {
		pr_err("%s: one of the required pointer is NULL.\n", __FUNCTION__);
		return -EINVAL;
	}

	if ((pdev->ctx.offload == MACSEC_OFFLOAD_MAC && !pdev->ctx.netdev)) {
		pr_err("%s: no netdev.\n", __FUNCTION__);
		return -EINVAL;
	}

	eth_msec = netdev_priv(pdev->ctx.netdev);
	if (!eth_msec)
		return -EINVAL;

	cfg = kzalloc(sizeof(*cfg), GFP_KERNEL);
	if (!cfg) {
		pr_err("%s: out of memory.\n", __FUNCTION__);
		return -ENOMEM;
	}

	eth_msec->macsec_cfg = cfg;
	eth_msec->macsec_cfg->dev = *pdev;
	pdev->ctx.netdev->features |= NETIF_F_HW_MACSEC;
	pdev->ctx.netdev->macsec_ops = &lgm_eth_macsec_ops;

	pr_debug("exiting %s\n", __func__);
	return 0;
}
EXPORT_SYMBOL(eip160_register);

void eip160_unregister(const struct eip160_pdev *pdev)
{
	struct eth_priv *eth_msec;

	pr_debug("entering %s\n", __func__);
	if (!pdev)
		return;
	eth_msec = netdev_priv(pdev->ctx.netdev);

	if (!eth_msec)
		return;

	kfree(eth_msec->macsec_cfg);
	eth_msec->macsec_cfg = NULL;
	pr_debug("exiting %s\n", __func__);
}
EXPORT_SYMBOL(eip160_unregister);

MODULE_AUTHOR("Govindaiah Mudepalli");
MODULE_DESCRIPTION("EIP160S MACsec driver");
MODULE_LICENSE("GPL v2");

