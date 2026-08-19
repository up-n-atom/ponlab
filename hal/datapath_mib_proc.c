/*****************************************************************************
 * Copyright (c) 2022, MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

 ******************************************************************************/
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DEBUGFS)
#include <net/datapath_api.h>
#include <net/datapath_proc_api.h>
#include "../datapath.h"
#include "datapath_misc.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	#define MAX_GSWIP_PMAC 3
	#define MAX_BLK_PRINT (MAX_GSWIP_PMAC + 1)
#else
	#define MAX_GSWIP_PMAC 2
	#define MAX_BLK_PRINT (MAX_GSWIP_PMAC + 1)
#endif

struct dp_xgmac_count {
	u64 rx; /* Rx Packets */
	u64 rx_err; /* sum of RxCrcErrors, RxFifoErrors */
	u64 tx; /* Tx Packets */
	u64 tx_err; /* sum of TxPacketErrors, Txunderflowerror */
	bool rx_valid; /* if XGMAC rx counter valid */
	bool tx_valid; /* if XGMAC tx counter valid */
};

struct dp_ctp_count {
	u64 ctp_ig_rx; /* CTP Rx packet count */
	u64 ctp_ig_drop; /* CTP Rx packet drop count = sum of RxFilter, RxDrop,
			  * RxExtendedVlanDiscard, MtuExceedDiscard
			  */
	u64 ctp_eg_rx; /* CTP Tx packet count before QoS */
	u64 ctp_eg_drop; /* CTP Tx packet drop count = sum of TxDrop,TxAcmDrop*/
	bool ig_valid; /* if CTP ig rx counters valid */
	bool eg_valid; /* if CTP eg rx counters valid */
};

struct dp_ctp_bypass_count {
	u64 bypass_tx; /* CTP Tx PCE-Bypass count after QoS */
	u64 bypass_drop; /* CTP Tx PCE-Bypass drop count = sum of TxDrop,
			  * TxAcmDrop
			  */
	bool tx_valid; /* if Bypass tx counters valid */
};

struct dp_bp_count {
	u64 bp_ig_rx; /* Bridge port Rx packet count */
	u64 bp_ig_drop; /* Bridge port Rx drop packet count = sum of RxFilter,
			 * RxDrop, RxExtendedVlanDiscard, MtuExceedDiscard
			 */
	u64 bp_eg_rx; /* Bridge port Tx packet count */
	u64 bp_eg_drop; /* Bridge port Tx drop packet count = sum of TxDrop,
			 * TxAcmDrop
			 */
	bool b_ig_valid; /* if BP IG rx counters valid */
	bool b_eg_valid; /* if BP eg rx counters valid */
};

struct dp_pmac_count {
	u64 ig; /* Pmac Ingress counters */
	u64 ig_discard; /* Pmac Ingress counters */
	u64 eg; /* Pmac Ingress counters */
	u64 eg_err; /* Pmac Ingress counters */
	bool ig_valid; /* if rx counters valid */
	bool eg_valid; /* if rx counters valid */
};

struct dp_mib {
	struct dp_xgmac_count *xgmac;
	struct dp_ctp_count *ctp;
	struct dp_ctp_bypass_count *ctp_bypass;
	struct dp_bp_count *bp;
	struct dp_pmac_count pmac0[PMAC_MAX_NUM];
	struct dp_pmac_count pmac1[PMAC_MAX_NUM];
	struct dp_pmac_count pmac2[PMAC_MAX_NUM];
	/* maximum XGMAC per platform */
	u32 max_mac;
	int blk;
	/* Different seq file pos
	 * for each column in single blk */
	int bypass_h;
	int xg_rx_h;
	int xg_tx_h;
	int ctp_ig_h;
	int ctp_eg_h;
	int ctp_bypass_h;
	int bp_ig_h;
	int bp_eg_h;
	int p_ig_h[MAX_GSWIP_PMAC];
	int p_eg_h[MAX_GSWIP_PMAC];
};

static bool single_dump;
static u32 dump_type;
static int tmp_inst;

/* to print GSWIP counter dump individually */
enum dump_type {
	XGMAC = 1,
	CTP,
	BP,
	G_PMAC,
};

static struct dp_mib *get_dp_count(int inst, struct seq_file *s)
{
	struct dp_proc_file_entry *p;
	struct dp_mib *dp_mib;

	p = (struct dp_proc_file_entry *)s->private;
	if (!p || !p->data) {
		if (!p)
			pr_err("DPM: p NULL\n");
		else
			pr_err("DPM: p->data NULL\n");
		return NULL;
	}
	dp_mib = p->data;
	if (is_invalid_inst(inst)) {
		pr_err("DPM: wrong inst=%d\n", inst);
		dp_die(__func__, inst, DP_MAX_INST);
		return &dp_mib[0];
	}
	return &dp_mib[inst];
}

static int dp_clear_gswip_counters(int inst)
{
	struct core_ops *gsw_ops;
	struct rmon_ops *rmon;
	GSW_RMON_clear_t rmon_clear = {0};
	struct mac_ops *mac_ops;
	int i = 0;

	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	rmon = &gsw_ops->gsw_rmon_ops;
	rmon_clear.eRmonType = GSW_RMON_ALL_TYPE;

	if (rmon->RMON_Clear(gsw_ops, &rmon_clear))
		return DP_FAILURE;

	for (i = 0; i < (gsw_get_mac_subifcnt(0) + 2); i++) {
		mac_ops = dp_port_prop[inst].mac_ops[i];
		if (mac_ops)
			mac_ops->rmon_clr(mac_ops);
	}
	return DP_SUCCESS;
}

/* To read gswip debug rmon counters
 * port_type = 0  CTP Port Rx/Tx Counters
 * port_type = 2  Bridge Port Rx/Tx Counters
 * port_type = 4  Bypass-PCE Port Tx Counters
 * num - maximum capable number of ctp or bridge port
 */
static int dp_read_gswip_counters(int inst, GSW_RMON_portType_t port_type,
		int num, struct dp_mib *dp_count)
{
	struct core_ops *gsw_ops;
	GSW_Debug_RMON_Port_cnt_t *dbg_counters;
	struct debug_ops *gsw_debug;
	int i = 0;
	struct dp_ctp_count *ctp;
	struct dp_ctp_bypass_count *ctp_bypass;
	struct dp_bp_count *bp;
	u32 type = port_type;

	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	gsw_debug = &gsw_ops->gsw_debug_ops;

	dbg_counters = dp_kzalloc(sizeof(GSW_RMON_Port_cnt_t), GFP_ATOMIC);
	if (!dbg_counters)
		return DP_FAILURE;
	ctp_bypass = dp_count->ctp_bypass;
	ctp = dp_count->ctp;
	bp = dp_count->bp;

	for (i = 0; i < num; i++) {
		dbg_counters->ePortType = port_type;
		dbg_counters->nPortId = i;
		/* read CTP counters */
		if (gsw_debug->DEBUG_RMON_Port_Get(gsw_ops, dbg_counters)) {
			kfree(dbg_counters);
			return DP_FAILURE;
		}

		if (type == GSW_RMON_CTP_PORT_RX) {
			ctp[i].ctp_ig_rx = dbg_counters->nRxGoodPkts;
			ctp[i].ctp_ig_drop = dbg_counters->nRxFilteredPkts +
				dbg_counters->nRxDroppedPkts +
				dbg_counters->nRxExtendedVlanDiscardPkts +
				dbg_counters->nMtuExceedDiscardPkts;
			if ((ctp[i].ctp_ig_rx > 0) || (ctp[i].ctp_ig_drop > 0))
				ctp[i].ig_valid = true;
		} else if (type == GSW_RMON_CTP_PORT_TX) {
			ctp[i].ctp_eg_rx = dbg_counters->nTxGoodPkts;
			ctp[i].ctp_eg_drop = dbg_counters->nTxDroppedPkts +
				dbg_counters->nTxAcmDroppedPkts;
			if ((ctp[i].ctp_eg_rx > 0) || (ctp[i].ctp_eg_drop > 0))
				ctp[i].eg_valid = true;
		} else if (type == GSW_RMON_CTP_PORT_PCE_BYPASS) {
			ctp_bypass[i].bypass_tx = dbg_counters->nTxGoodPkts;
			ctp_bypass[i].bypass_drop =
					dbg_counters->nTxDroppedPkts +
					dbg_counters->nTxAcmDroppedPkts;
			if ((ctp_bypass[i].bypass_tx > 0) ||
			    (ctp_bypass[i].bypass_drop > 0))
				ctp_bypass[i].tx_valid = true;
		} else if (type == GSW_RMON_BRIDGE_PORT_RX) {
			bp[i].bp_ig_rx = dbg_counters->nRxGoodPkts;
			bp[i].bp_ig_drop = dbg_counters->nRxFilteredPkts +
				dbg_counters->nRxDroppedPkts +
				dbg_counters->nRxExtendedVlanDiscardPkts +
				dbg_counters->nMtuExceedDiscardPkts;
			if ((bp[i].bp_ig_rx > 0) || (bp[i].bp_ig_drop > 0))
				bp[i].b_ig_valid = true;
		} else if (type == GSW_RMON_BRIDGE_PORT_TX) {
			bp[i].bp_eg_rx = dbg_counters->nTxGoodPkts;
			bp[i].bp_eg_drop = dbg_counters->nTxDroppedPkts +
				dbg_counters->nTxAcmDroppedPkts;
			if ((bp[i].bp_eg_rx > 0) || (bp[i].bp_eg_drop > 0))
				bp[i].b_eg_valid = true;
		}
	}

	kfree(dbg_counters);
	return DP_SUCCESS;
}

/* To read gswip xgmac counters
 * num - number of MAC as per GSWIP version
 */
static int dp_read_xgmac_counters(int inst, int num, struct dp_mib *dp_count)
{
	struct mac_ops *mac_ops;
	struct dp_cap *cap;
	struct mac_rmon *xgmac_rmon = NULL;
	int i = 0;
	struct dp_xgmac_count *xgmac = NULL;

	cap = &get_dp_prop_info(inst)->cap;
	xgmac_rmon = dp_kzalloc(sizeof(*xgmac_rmon), GFP_ATOMIC);
	if (!xgmac_rmon)
		return DP_FAILURE;

	xgmac = dp_count->xgmac;
	for (i = 0; i < num; i++) {
		mac_ops = dp_port_prop[inst].mac_ops[i];
		if (mac_ops) {
			mac_ops->rmon_get(mac_ops, xgmac_rmon);
			xgmac[i].rx = xgmac_rmon->cnt[RMON_RX_PKTS];
			xgmac[i].rx_err =
				xgmac_rmon->cnt[RMON_RX_CRC_ERR] +
				xgmac_rmon->cnt[RMON_RX_OVERFLOW];
			xgmac[i].tx = xgmac_rmon->cnt[RMON_TX_PKTS];
			/* Tx_Packet_Errors = (good packets - tx packets)
			 * in switch_cli
			 */
			xgmac[i].tx_err =
				xgmac_rmon->cnt[RMON_TX_UNDERFLOW] +
				(xgmac_rmon->cnt[RMON_TX_GOOD_PKTS] -
				 xgmac_rmon->cnt[RMON_TX_PKTS]);
			if ((xgmac[i].tx > 0) || (xgmac[i].tx_err > 0))
				xgmac[i].tx_valid = true;
			if ((xgmac[i].rx > 0) || (xgmac[i].rx_err > 0))
				xgmac[i].rx_valid = true;
		}
	}
	kfree(xgmac_rmon);
	return DP_SUCCESS;
}

/* To read Ingress, egress PMAC counters */
static int dp_read_pmac_counters(int inst, u8 pmacid, struct dp_mib *dp_count)
{
	struct core_ops *gsw_ops;
	GSW_PMAC_Cnt_t *pmac_counters;
	struct pmac_ops *gsw_pmac;
	struct dp_pmac_count *pmac;
	int i;

	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	gsw_pmac = &gsw_ops->gsw_pmac_ops;
	if (pmacid == 0)
		pmac = dp_count->pmac0;
	else if (pmacid == 1)
		pmac = dp_count->pmac1;
	else if ((pmacid == 2) && (is_soc_lgm(inst)))
		pmac = dp_count->pmac2;
	else
		return DP_FAILURE;

	pmac_counters = dp_kzalloc(sizeof(GSW_RMON_Port_cnt_t),
			GFP_ATOMIC);
	if (!pmac_counters)
		return DP_FAILURE;
	for (i = 0; i < PMAC_MAX_NUM; i++) {
		pmac_counters->nPmacId = pmacid;
		pmac_counters->nTxDmaChanId = i;
		if (gsw_pmac->Pmac_CountGet(gsw_ops, pmac_counters)) {
			kfree(pmac_counters);
			return DP_FAILURE;
		}
		pmac[i].ig = pmac_counters->nIngressPktsCount;
		pmac[i].ig_discard = pmac_counters->nDiscPktsCount;
		pmac[i].eg = pmac_counters->nEgressPktsCount;
		pmac[i].eg_err = pmac_counters->nChkSumErrPktsCount;
		if ((pmac[i].ig > 0) || (pmac[i].ig_discard))
			pmac[i].ig_valid = true;
		if ((pmac[i].eg > 0) || (pmac[i].eg_err))
			pmac[i].eg_valid = true;
	}

	kfree(pmac_counters);
	return DP_SUCCESS;
}

static void print_xgmac_info(struct seq_file *s, struct dp_mib *dp_count)
{
	int i;

	dp_sprintf(s, "%-30s\n", "XGMAC Counters");
	dp_sprintf(s, "%4s%13s%13s%4s%13s%13s\n",
		   "Port", "rx", "err ", "Port", "tx", "err");
	for (i = 0; i < dp_count->max_mac; i++) {
		if ((dp_count->xgmac[i].rx_valid) ||
		    (dp_count->xgmac[i].tx_valid)) {
			if (dp_count->xgmac[i].rx_valid) {
				dp_sprintf(s, "%3d:", i);
				dp_sprintf(s, "%13llu", dp_count->xgmac[i].rx);
				dp_sprintf(s, "%13llu",
					   dp_count->xgmac[i].rx_err);
			} else {
				dp_sprintf(s, "%30s", " ");
			}
			if ((dp_count->xgmac[i].tx_valid)) {
				dp_sprintf(s, "%3d:", i);
				dp_sprintf(s, "%13llu", dp_count->xgmac[i].tx);
				dp_sprintf(s, "%13llu",
					   dp_count->xgmac[i].tx_err);
			} else {
				dp_sprintf(s, "%30s\n", " ");
			}
			dp_sputs(s, "\n");
		}
	}
	return;
}

static void print_ctp_info(struct seq_file *s, int inst, int num,
			   struct dp_mib *dp_count)
{
	int i;
	struct dp_ctp_count *ctp;
	struct dp_ctp_bypass_count *bypass;

	ctp = dp_count->ctp;
	bypass = dp_count->ctp_bypass;
	dp_sprintf(s, "%-30s\n", "CTP/Bypass Counters");
	dp_sprintf(s, "%4s%13s%13s%4s%13s%13s%4s%13s%13s\n",
		   "Port", "ig_rx", "ig_err ", "Port", "eg_rx", "eg_err ",
		   "Port", "tx", "err");
	for (i = 0; i < num; i++) {
		if ((ctp[i].ig_valid) || (ctp[i].eg_valid) ||
		    (bypass[i].tx_valid)) {
			if (ctp[i].ig_valid) {
				dp_sprintf(s, "%3d:", i);
				dp_sprintf(s, "%13llu", ctp[i].ctp_ig_rx);
				dp_sprintf(s, "%13llu", ctp[i].ctp_ig_drop);
			} else {
				dp_sprintf(s, "%30s", " ");
			}
			if (ctp[i].eg_valid) {
				dp_sprintf(s, "%3d:", i);
				dp_sprintf(s, "%13llu", ctp[i].ctp_eg_rx);
				dp_sprintf(s, "%13llu", ctp[i].ctp_eg_drop);
			} else {
				dp_sprintf(s, "%30s", " ");
			}
			if (bypass[i].tx_valid) {
				dp_sprintf(s, "%3d:", i);
				dp_sprintf(s, "%13llu", bypass[i].bypass_tx);
				dp_sprintf(s, "%13llu", bypass[i].bypass_drop);
			} else {
				dp_sprintf(s, "%30s", " ");
			}
			dp_sputs(s, "\n");
		}
	}
	return;
}

static void print_bp_info(struct seq_file *s, int inst, int num,
			  struct dp_mib *dp_count)
{
	int i;
	struct dp_bp_count *bp;

	bp = dp_count->bp;
	dp_sprintf(s, "%30s\n", "BP Counters");
	dp_sprintf(s, "%4s%13s%13s%4s%13s%13s\n",
		   "Port", "ig_rx", "ig_err ", "Port", "eg_rx", "eg_err");
	for (i = 0; i < num; i++) {
		if ((bp[i].b_ig_valid) || (bp[i].b_eg_valid)) {
			if (bp[i].b_ig_valid) {
				dp_sprintf(s, "%3d:", i);
				dp_sprintf(s, "%13llu", bp[i].bp_ig_rx);
				dp_sprintf(s, "%13llu", bp[i].bp_ig_drop);
			} else {
				dp_sprintf(s, "%30s", " ");
			}
			if (bp[i].b_eg_valid) {
				dp_sprintf(s, "%3d:", i);
				dp_sprintf(s, "%13llu", bp[i].bp_eg_rx);
				dp_sprintf(s, "%13llu", bp[i].bp_eg_drop);
			} else {
				dp_sprintf(s, "%30s", " ");
			}
			dp_sputs(s, "\n");
		}
	}
	return;
}

static void print_pmac_info(struct seq_file *s, struct dp_mib *dp_count,
			    int num)
{
	int k;
	struct dp_pmac_count *pmac0, *pmac1;

	pmac0 = dp_count->pmac0;
	pmac1 = dp_count->pmac1;
	dp_sprintf(s, "%-30s\n", "PMAC Counters");
	dp_sprintf(s, "%10s %-10s%10s %10s %-10s%10s %10s %-10s%10s %10s %-10s%10s\n",
		   " ", "PMAC0_EG", " ", " ", "PMAC0_IG", " ", " ", "PMAC1_EG",
		   " ", " ", "PMAC1_IG", " ");
	dp_sprintf(s, "%4s%13s%13s%4s%13s%13s%4s%13s%13s%4s%13s%13s\n",
		   "Port", "rx", "err ", "DMA1Ch", "tx", "err ", "Port", "rx",
		   "err ", "DMA2Ch", "tx", "err");
	for (k = 0; k < num; k++) {
		if ((pmac0[k].eg_valid) || (pmac0[k].ig_valid) ||
		    (pmac1[k].eg_valid) || (pmac1[k].ig_valid)) {
			if (pmac0[k].eg_valid) {
				dp_sprintf(s, "%3d:", k);
				dp_sprintf(s, "%13llu", pmac0[k].eg);
				dp_sprintf(s, "%13llu", pmac0[k].eg_err);
			} else {
				dp_sprintf(s, "%30s", " ");
			}
			if (pmac0[k].ig_valid) {
				dp_sprintf(s, "%3d:", k);
				dp_sprintf(s, "%13llu", pmac0[k].ig);
				dp_sprintf(s, "%13llu", pmac0[k].ig_discard);
			} else {
				dp_sprintf(s, "%30s", " ");
			}
			if (pmac1[k].eg_valid) {
				dp_sprintf(s, "%3d:", k);
				dp_sprintf(s, "%13llu", pmac1[k].eg);
				dp_sprintf(s, "%13llu", pmac1[k].eg_err);
			} else {
				dp_sprintf(s, "%30s", " ");
			}
			if (pmac1[k].ig_valid) {
				dp_sprintf(s, "%3d:", k);
				dp_sprintf(s, "%13llu", pmac1[k].ig);
				dp_sprintf(s, "%13llu", pmac1[k].ig_discard);
			} else {
				dp_sprintf(s, "%30s", " ");
			}
			dp_sputs(s, "\n");
		}
	}
	return;
}

static int dump_blk0_info(struct seq_file *s, int inst, struct dp_mib *dp_count)
{
	struct dp_cap *cap;
	int i, j, k;
	struct dp_xgmac_count *xgmac;
	struct dp_ctp_count *ctp;
	struct dp_bp_count *bp;

	xgmac = dp_count->xgmac;
	ctp = dp_count->ctp;
	bp = dp_count->bp;
	cap = &get_dp_prop_info(inst)->cap;
	if ((!xgmac) || (!ctp) || (!bp))
		return -1;
	i = dp_count->xg_rx_h;
	j = dp_count->ctp_ig_h;
	k = dp_count->bp_ig_h;
	if ((i >= dp_count->max_mac) && (j >= cap->max_num_subif) &&
	    (k >= cap->max_num_bridge_port))
		return -1;
	while ((i < dp_count->max_mac) && (!xgmac[i].rx_valid))
		i++;
	while ((j < cap->max_num_subif) && (!ctp[j].ig_valid))
		j++;
	while ((k < cap->max_num_bridge_port) && (!bp[k].b_ig_valid))
		k++;
	dp_sprintf(s, "%s", "|");
	if ((i < dp_count->max_mac) && (xgmac[i].rx_valid)) {
		dp_sprintf(s, "%4d  ", i);
		dp_sprintf(s, "%13llu", xgmac[i].rx);
		dp_sprintf(s, "%13llu", xgmac[i].rx_err);
	} else {
		dp_sprintf(s, "%32s", " ");
	}
	dp_sprintf(s, "%s", "|");
	if ((j < cap->max_num_subif) && (ctp[j].ig_valid)) {
		dp_sprintf(s, "%4d  ", j);
		dp_sprintf(s, "%13llu", ctp[j].ctp_ig_rx);
		dp_sprintf(s, "%13llu", ctp[j].ctp_ig_drop);
	} else {
		dp_sprintf(s, "%32s", " ");
	}
	dp_sprintf(s, "%s", "|");
	if ((k < cap->max_num_bridge_port) && (bp[k].b_ig_valid)) {
		dp_sprintf(s, "%4d  ", k);
		dp_sprintf(s, "%13llu", bp[k].bp_ig_rx);
		dp_sprintf(s, "%13llu", bp[k].bp_ig_drop);
	} else {
		dp_sprintf(s, "%32s", " ");
	}
	dp_sputs(s, "|\n");
	if (i < dp_count->max_mac)
		dp_count->xg_rx_h = i + 1;
	else
		dp_count->xg_rx_h = dp_count->max_mac;
	if (j < cap->max_num_subif)
		dp_count->ctp_ig_h = j + 1;
	else
		dp_count->ctp_ig_h = cap->max_num_subif;
	if (k < cap->max_num_bridge_port)
		dp_count->bp_ig_h = k + 1;
	else
		dp_count->bp_ig_h = cap->max_num_bridge_port;
	return 0;
}

static int dump_blk1_info(struct seq_file *s, int inst, struct dp_mib *dp_count)
{
	struct dp_cap *cap;
	struct dp_ctp_count *ctp;
	struct dp_bp_count *bp;
	struct dp_pmac_count *pmac0, *pmac1;
	int i = 0, j = 0, k = 0, l = 0;

	cap = &get_dp_prop_info(inst)->cap;
	ctp = dp_count->ctp;
	bp = dp_count->bp;
	pmac0 = dp_count->pmac0;
	pmac1 = dp_count->pmac1;
	if ((!ctp) || (!bp) || (!pmac0) || (!pmac1))
		return -1;
	i = dp_count->bp_eg_h;
	j = dp_count->ctp_eg_h;
	k = dp_count->p_eg_h[0];
	l = dp_count->p_eg_h[1];
	if ((i >= cap->max_num_bridge_port) && (j >= cap->max_num_subif) &&
	    (k >= PMAC_MAX_NUM) && (l >= PMAC_MAX_NUM))
		return -1;
	while ((i < cap->max_num_bridge_port) && (!bp[i].b_eg_valid))
		i++;
	while ((j < cap->max_num_subif) && (!ctp[j].eg_valid))
		j++;
	while ((k < PMAC_MAX_NUM) && (!pmac0[k].eg_valid))
		k++;
	while ((l < PMAC_MAX_NUM) && (!pmac1[l].eg_valid))
		l++;
	dp_sprintf(s, "%s", "|");
	if ((i < cap->max_num_bridge_port) && (bp[i].b_eg_valid)) {
		dp_sprintf(s, "%4d  ", i);
		dp_sprintf(s, "%13llu", bp[i].bp_eg_rx);
		dp_sprintf(s, "%13llu", bp[i].bp_eg_drop);
	} else {
		dp_sprintf(s, "%32s", " ");
	}
	dp_sprintf(s, "%s", "|");
	if ((j < cap->max_num_subif) && (ctp[j].eg_valid)) {
		dp_sprintf(s, "%4d  ", j);
		dp_sprintf(s, "%13llu", ctp[j].ctp_eg_rx);
		dp_sprintf(s, "%13llu", ctp[j].ctp_eg_drop);
	} else {
		dp_sprintf(s, "%32s", " ");
	}
	dp_sprintf(s, "%s", "|");
	if ((k < PMAC_MAX_NUM) && (pmac0[k].eg_valid)) {
		dp_sprintf(s, "%4d  ", k);
		dp_sprintf(s, "%13llu", pmac0[k].eg);
		dp_sprintf(s, "%13llu", pmac0[k].eg_err);
	} else {
		dp_sprintf(s, "%32s", " ");
	}
	dp_sprintf(s, "%s", "|");
	if ((l < PMAC_MAX_NUM) && (pmac1[l].eg_valid)) {
		dp_sprintf(s, "%4d  ", l);
		dp_sprintf(s, "%13llu", pmac1[l].eg);
		dp_sprintf(s, "%13llu", pmac1[l].eg_err);
	} else {
		dp_sprintf(s, "%32s", " ");
	}
	dp_sputs(s, "|\n");
	if (i < cap->max_num_bridge_port)
		dp_count->bp_eg_h = i + 1;
	else
		dp_count->bp_eg_h = cap->max_num_bridge_port;
	if (j < cap->max_num_subif)
		dp_count->ctp_eg_h = j + 1;
	else
		dp_count->ctp_eg_h = cap->max_num_subif;
	if (k < PMAC_MAX_NUM)
		dp_count->p_eg_h[0] = k + 1;
	else
		dp_count->p_eg_h[0] = PMAC_MAX_NUM;
	if (l < PMAC_MAX_NUM)
		dp_count->p_eg_h[1] = l + 1;
	else
		dp_count->p_eg_h[1] = PMAC_MAX_NUM;
	return 0;
}

static int dump_blk2_info(struct seq_file *s, int inst, struct dp_mib *dp_count)
{
	struct dp_cap *cap;
	struct dp_ctp_bypass_count *ctp;
	struct dp_xgmac_count *xgmac;
	struct dp_pmac_count *pmac0, *pmac1;
	int i = 0, j = 0, k = 0, l = 0;

	cap = &get_dp_prop_info(inst)->cap;
	ctp = dp_count->ctp_bypass;
	pmac0 = dp_count->pmac0;
	pmac1 = dp_count->pmac1;
	xgmac = dp_count->xgmac;
	if ((!ctp) || (!xgmac) || (!pmac0) || (!pmac1))
		return -1;
	i = dp_count->p_ig_h[0];
	j = dp_count->p_ig_h[1];
	k = dp_count->ctp_bypass_h;
	l = dp_count->xg_tx_h;
	if ((i >= PMAC_MAX_NUM) && (j >= PMAC_MAX_NUM) &&
	    (k >= cap->max_num_subif) && (l >= (dp_count->max_mac)))
		return -1;
	while ((i < PMAC_MAX_NUM) && (!pmac0[i].ig_valid))
		i++;
	while ((j < PMAC_MAX_NUM) && (!pmac1[j].ig_valid))
		j++;
	while ((k < cap->max_num_subif) && (!ctp[k].tx_valid))
		k++;
	while ((l < dp_count->max_mac) && (!xgmac[l].tx_valid))
		l++;
	dp_sprintf(s, "%s", "|");
	if ((i < PMAC_MAX_NUM) && (pmac0[i].ig_valid)) {
		dp_sprintf(s, "%4d  ", i);
		dp_sprintf(s, "%13llu", pmac0[i].ig);
		dp_sprintf(s, "%13llu", pmac0[i].ig_discard);
	} else {
		dp_sprintf(s, "%32s", " ");
	}
	dp_sprintf(s, "%s", "|");
	if ((j < PMAC_MAX_NUM) && (pmac1[j].ig_valid)) {
		dp_sprintf(s, "%4d  ", j);
		dp_sprintf(s, "%13llu", pmac1[j].ig);
		dp_sprintf(s, "%13llu", pmac1[j].ig_discard);
	} else {
		dp_sprintf(s, "%32s", " ");
	}
	dp_sprintf(s, "%s", "|");
	if ((k < cap->max_num_subif) && (ctp[k].tx_valid)) {
		dp_sprintf(s, "%4d  ", k);
		dp_sprintf(s, "%13llu", ctp[k].bypass_tx);
		dp_sprintf(s, "%13llu", ctp[k].bypass_drop);
	} else {
		dp_sprintf(s, "%32s", " ");
	}
	dp_sprintf(s, "%s", "|");
	if ((l < dp_count->max_mac) && (xgmac[l].tx_valid)) {
		dp_sprintf(s, "%4d  ", l);
		dp_sprintf(s, "%13llu", xgmac[l].tx);
		dp_sprintf(s, "%13llu", xgmac[l].tx_err);
	} else {
		dp_sprintf(s, "%32s", " ");
	}
	dp_sputs(s, "|\n");
	if (i < PMAC_MAX_NUM)
		dp_count->p_ig_h[0] = i + 1;
	else
		dp_count->p_ig_h[0] = PMAC_MAX_NUM;
	if (j < PMAC_MAX_NUM)
		dp_count->p_ig_h[1] = j + 1;
	else
		dp_count->p_ig_h[1] = PMAC_MAX_NUM;
	if (k < cap->max_num_subif)
		dp_count->ctp_bypass_h = k + 1;
	else
		dp_count->ctp_bypass_h = cap->max_num_subif;
	if (l < dp_count->max_mac)
		dp_count->xg_tx_h = l + 1;
	else
		dp_count->xg_tx_h = dp_count->max_mac;
	return 0;
}

static int dump_blk3_info(struct seq_file *s, int inst, struct dp_mib *dp_count)
{
	struct dp_pmac_count *pmac2;
	int i = 0, j = 0;

	pmac2 = dp_count->pmac2;
	if (!pmac2)
		return -1;

	i = dp_count->p_eg_h[MAX_GSWIP_PMAC - 1];
	j = dp_count->p_ig_h[MAX_GSWIP_PMAC - 1];
	if ((i >= PMAC_MAX_NUM) && (j >= PMAC_MAX_NUM))
		return -1;
	while ((i < PMAC_MAX_NUM) && (!pmac2[i].eg_valid))
		i++;
	while ((j < PMAC_MAX_NUM) && (!pmac2[j].ig_valid))
		j++;
	dp_sprintf(s, "%s", "|");
	if ((i < PMAC_MAX_NUM) && (pmac2[i].eg_valid)) {
		dp_sprintf(s, "%4d: ", i);
		dp_sprintf(s, "%13llu", pmac2[i].eg);
		dp_sprintf(s, "%13llu", pmac2[i].eg_err);
	} else {
		dp_sprintf(s, "%32s", " ");
	}
	dp_sprintf(s, "%s", "|");
	if ((j < PMAC_MAX_NUM) && (pmac2[j].ig_valid)) {
		dp_sprintf(s, "%4d: ", j);
		dp_sprintf(s, "%13llu", pmac2[j].ig);
		dp_sprintf(s, "%13llu", pmac2[j].ig_discard);
	} else {
		dp_sprintf(s, "%32s", " ");
	}
	dp_sputs(s, "|\n");
	if (i < PMAC_MAX_NUM)
		dp_count->p_eg_h[MAX_GSWIP_PMAC - 1] = i + 1;
	else
		dp_count->p_eg_h[MAX_GSWIP_PMAC - 1] = PMAC_MAX_NUM;
	if (j < PMAC_MAX_NUM)
		dp_count->p_ig_h[MAX_GSWIP_PMAC - 1] = j + 1;
	else
		dp_count->p_ig_h[MAX_GSWIP_PMAC - 1] = PMAC_MAX_NUM;
	return 0;
}

int proc_mib_count_init(void *param)
{
	struct dp_proc_file_entry *p;
	struct dp_mib *dp_count;
	struct dp_cap *cap;

	tmp_inst = 0;
	cap = &get_dp_prop_info(tmp_inst)->cap;
	dp_count = dp_kzalloc(2 * sizeof(struct dp_mib), GFP_ATOMIC);
	if (!dp_count)
		return DP_FAILURE;
	dp_count->max_mac = gsw_get_mac_subifcnt(0) + 2;
	/* Allocate memory for GSWIP related counters */
	dp_count->ctp = dp_kzalloc(sizeof(struct dp_ctp_count) *
			cap->max_num_subif, GFP_ATOMIC);
	if (!dp_count->ctp) {
		kfree(dp_count);
		return DP_FAILURE;
	}
	dp_count->ctp_bypass = dp_kzalloc(sizeof(struct dp_ctp_bypass_count) *
					  cap->max_num_subif, GFP_ATOMIC);
	if (!dp_count->ctp_bypass) {
		kfree(dp_count->ctp);
		kfree(dp_count);
		return DP_FAILURE;
	}
	dp_count->bp = dp_kzalloc(sizeof(struct dp_bp_count) *
				  cap->max_num_bridge_port, GFP_ATOMIC);
	if (!dp_count->bp) {
		kfree(dp_count->ctp);
		kfree(dp_count->ctp_bypass);
		kfree(dp_count);
		return DP_FAILURE;
	}
	dp_count->xgmac = dp_kzalloc(sizeof(struct dp_xgmac_count) *
				     dp_count->max_mac, GFP_ATOMIC);
	if (!dp_count->xgmac) {
		kfree(dp_count->ctp);
		kfree(dp_count->ctp_bypass);
		kfree(dp_count->bp);
		kfree(dp_count);
		return DP_FAILURE;
	}
	p = param;
	p->data = dp_count;
	return DP_SUCCESS;
}

void print_gswip_specific_count(struct seq_file *s, struct dp_mib *dp_count,
				int *pos)
{
	struct dp_cap *cap;

	cap = &get_dp_prop_info(tmp_inst)->cap;
	if ((dp_count->blk > 0) && (*pos != 0))
		goto exit;
	switch (dump_type) {
	case XGMAC:
		print_xgmac_info(s, dp_count);
		break;
	case CTP:
		print_ctp_info(s, tmp_inst, cap->max_num_subif, dp_count);
		break;
	case BP:
		print_bp_info(s, tmp_inst, cap->max_num_bridge_port, dp_count);
		break;
	case G_PMAC:
		print_pmac_info(s, dp_count, PMAC_MAX_NUM);
		break;
	default:
		pr_info("Enter valid option(dump:%d)\n", dump_type);
		break;
	}
exit:
	single_dump = 0;
	*pos = -1;
	return;
}

static void free_mib_count_res(struct dp_mib *dp_count)
{
	kfree(dp_count->ctp);
	kfree(dp_count->ctp_bypass);
	kfree(dp_count->bp);
	kfree(dp_count->xgmac);
}

int proc_mib_count_dump(struct seq_file *s, int pos)
{
	struct dp_mib *dp_count;
	struct dp_cap *cap;
	int pmacid, ret;
	static const char hyphen[] = "--------------------------------";

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return -1;
	}

	dp_count = get_dp_count(tmp_inst, s);
	if (!dp_count)
		return -1;
	cap = &get_dp_prop_info(tmp_inst)->cap;
	if ((dp_count->blk == 0) && (pos == 0)) {
		/* read & store CTP counters */
		if (dp_read_gswip_counters(tmp_inst, GSW_RMON_CTP_PORT_RX,
					   cap->max_num_subif, dp_count))
			goto exit;
		if (dp_read_gswip_counters(tmp_inst, GSW_RMON_CTP_PORT_TX,
					   cap->max_num_subif, dp_count))
			goto exit;
		/* read & store BP counters */
		if (dp_read_gswip_counters(tmp_inst, GSW_RMON_BRIDGE_PORT_RX,
					   cap->max_num_bridge_port, dp_count))
			goto exit;
		if (dp_read_gswip_counters(tmp_inst, GSW_RMON_BRIDGE_PORT_TX,
					   cap->max_num_bridge_port, dp_count))
			goto exit;
		/* read & store CTP by-pass counters */
		if (dp_read_gswip_counters(tmp_inst,
					   GSW_RMON_CTP_PORT_PCE_BYPASS,
					   cap->max_num_subif, dp_count))
			goto exit;

		/* read & store CTP by-pass counters */
		if (dp_read_xgmac_counters(tmp_inst, dp_count->max_mac,
					   dp_count))
			goto exit;
		for (pmacid = 0; pmacid < MAX_GSWIP_PMAC; pmacid++) {
			if (dp_read_pmac_counters(tmp_inst, pmacid, dp_count))
				goto exit;
		}
	}
	if (single_dump) {
		print_gswip_specific_count(s, dp_count, &pos);
		goto exit;
	}
	if (dp_count->blk == 0) {
		if (pos == 0) {
			dp_sputs(s, "Rx Counters\n");
			dp_sputs(s, "============\n");
			dp_sprintf(s, "%s%32s%s%32s%s%32s%s\n",
				   "-", hyphen, "=", hyphen, "=", hyphen, "-");
			dp_sprintf(s, "%s%10s %-10s%10s %s%10s %-10s%10s %s%10s %-10s%10s %s\n",
				   "|", " ", "XGMAC", " ", "|", " ", "CTP Ig",
				   " ", "|", " ", "BP Ig", " ", "|");
			dp_sprintf(s, "%s%6s%13s%13s%s%6s%13s%13s%s%6s%13s%13s%s\n",
				   "|", "Port", "rx", "err", "|", "Port", "rx",
				   "err", "|", "Port", "rx", "err", "|");
			dp_sprintf(s, "%s%32s%s%32s%s%32s%s\n",
				   "-", hyphen, "=", hyphen, "=", hyphen, "-");
			if (!dp_seq_has_overflowed(s))
				pos++;
			return pos;
		}
		ret = dump_blk0_info(s, tmp_inst, dp_count);
		if (ret == -1) {
			pos = 0;
			dp_count->blk++;
			return pos;
		}
		if (!dp_seq_has_overflowed(s))
			pos++;
		return pos;
	}

	if (dp_count->blk == 1) {
		if (pos == 0) {
			dp_sprintf(s, "%s%32s%s%32s%s%32s%s%32s%s\n",
				   "-", hyphen, "=", hyphen, "=", hyphen, "=", hyphen, "-");
			dp_sprintf(s, "%s%10s %-10s%10s %s%8s%-16s%8s%s%8s%-16s%8s%s%8s%-16s%8s%s\n",
				   "|", " ", "BP EG", " ", "|", " ", "CTP EG",
				   " ", "|", " ", "PMAC0 EG", " ", "|", " ",
				   "PMAC1 EG", " ", "|");
			dp_sprintf(s, "%s%6s%13s%13s%s%6s%13s%13s%s%6s%13s%13s%s%6s%13s%13s%s\n",
				   "|", "Port", "rx", "err", "|", "Port",
				   "rx", "err", "|", "Port", "rx", "err",
				   "|", "Port", "rx", "err", "|");
			dp_sprintf(s, "%s%32s%s%32s%s%32s%s%32s%s\n",
				   "-", hyphen, "=", hyphen, "=", hyphen,
				   "=", hyphen, "-");
			if (!dp_seq_has_overflowed(s))
				pos++;
			return pos;
		}
		ret = dump_blk1_info(s, tmp_inst, dp_count);
		if (ret == -1) {
			pos = 0;
			dp_sprintf(s, "%s%30s%s%30s%s%30s%s%30s%s\n",
				   "-", hyphen, "=", hyphen, "=", hyphen,
				   "=", hyphen, "-");
			dp_count->blk++;
			return pos;
		}
		if (!dp_seq_has_overflowed(s))
			pos++;
		return pos;
	}

	if (dp_count->blk == 2) {
		if (pos == 0) {
			dp_sputs(s, "Tx Counters\n");
			dp_sputs(s, "============\n");
			dp_sprintf(s, "%s%32s%s%32s%s%32s%s%32s%s\n",
				   "-", hyphen, "=", hyphen, "=", hyphen, "=", hyphen, "-");
			dp_sprintf(s, "%s%10s %-10s%10s %s%10s %-10s%10s %s%10s %-10s%10s %s%10s %-10s%10s %s\n",
				   "|", " ", "PMAC0 Ig", " ", "|", " ",
				   "PMAC1 Ig", " ", "|", " ", "CTP Bypass",
				   " ", "|", " ", "XGMAC", " ", "|");
			if (is_soc_prx(tmp_inst)) {
				dp_sprintf(s, "%s%6s%13s%13s%s%6s%13s%13s",
					   "|", "DMA1Ch", "tx", "err", "|",
					   "DMA2Ch", "tx", "err");
			} else if (is_soc_lgm(tmp_inst)) {
				dp_sprintf(s, "%s%6s%13s%13s%s%6s%13s%13s",
					   "|", "DMA0Ch", "tx", "err", "|",
					   "DMA1Ch", "tx", "err");
			} else {
				dp_sprintf(s, "%s%6s%13s%13s%s%6s%13s%13s",
					   "|", " ", "tx", "err", "|",
					   " ", "tx", "err");
			}
			dp_sprintf(s, "%s%6s%13s%13s%s%6s%13s%13s%s\n",
				   "|", "Port", "tx", "err", "|", "Port", "tx",
				   "err", "|");
			dp_sprintf(s, "%s%32s%s%32s%s%32s%s%32s%s\n",
				   "-", hyphen, "=", hyphen, "=", hyphen,
				   "=", hyphen, "-");
			if (!dp_seq_has_overflowed(s))
				pos++;
			return pos;
		}
		ret = dump_blk2_info(s, tmp_inst, dp_count);
		if (ret == -1) {
			pos = 0;
			dp_sprintf(s, "%s%32s%s%32s%s%32s%s%32s%s\n",
				   "-", hyphen, "=", hyphen, "=", hyphen,
				   "=", hyphen, "-");
			dp_count->blk++;
			return pos;
		}
		if (!dp_seq_has_overflowed(s))
			pos++;
		return pos;
	}

	if (is_soc_prx(tmp_inst))
		goto blk_check;
	if (dp_count->blk == 3) {
		if (pos == 0) {
			dp_sputs(s, "PMAC2\n");
			dp_sputs(s, "======\n");
			dp_sprintf(s, "%s%32s%s%32s%s\n",
				   "-", hyphen, "=", hyphen, "-");
			dp_sprintf(s, "%s%10s %-10s%10s %s%10s %-10s%10s %s\n",
				   "|", " ", "PMAC2 Eg", " ", "|", " ",
				   "PMAC2 Ig", " ", "|");
			dp_sprintf(s, "%s%6s%13s%13s%s%6s%13s%13s%s\n",
				   "|", "Port", "rx", "err", "|", "DMA2Ch",
				   "tx", "err", "|");
			dp_sprintf(s, "%s%32s%s%32s%s\n",
				   "-", hyphen, "=", hyphen, "-");
			if (!dp_seq_has_overflowed(s))
				pos++;
			return pos;
		}
		ret = dump_blk3_info(s, tmp_inst, dp_count);
		if (ret == -1) {
			pos = 0;
			dp_sprintf(s, "%s%32s%s%32s%s\n",
				   "-", hyphen, "=", hyphen, "-");
			dp_count->blk++;
			return pos;
		}
		if (!dp_seq_has_overflowed(s))
			pos++;
		return pos;
	}
blk_check:
	if (dp_count->blk >= MAX_BLK_PRINT) {
		tmp_inst++;
		dp_count->blk = 0;
		pos = 0;
	}
	if (tmp_inst >= dp_inst_num) {
		pos = -1;	/*end of the loop */
		goto exit;
	}
	return pos;
exit:
	free_mib_count_res(dp_count);
	return pos;
}

ssize_t proc_mib_count_write(struct file *file, const char *buf,
			     size_t count, loff_t *ppos)
{
	char *param_list[10] = {NULL};
	char *str, *p;
	int num;
	if (!capable(CAP_SYS_PACCT))
		return count;

	str = dp_kzalloc(count + 1, GFP_ATOMIC);
	if (!str)
		return -ENOMEM;

	if (dp_copy_from_user(str, buf, count)) {
		kfree(str);
		return -EFAULT;
	}
	p = str;
	for (num = 0; num < ARRAY_SIZE(param_list); num++) {
		if (!strlen(p))
			break;
		param_list[num] = dp_strsep(&p, " \n");
		if (!p)
			break;
	}
	if (!strcasecmp(param_list[0], "get_mib")) {
		single_dump = 1;
		if (!strcasecmp(param_list[1], "xgmac"))
			dump_type = XGMAC;
		else if (!strcasecmp(param_list[1], "ctp"))
			dump_type = CTP;
		else if (!strcasecmp(param_list[1], "bp"))
			dump_type = BP;
		else if (!strcasecmp(param_list[1], "pmac"))
			dump_type = G_PMAC;
	} else if (!strcasecmp(param_list[0], "clear")) {
		/* Can further enhance to clear MIB counters later */
		single_dump = 0;
		dp_clear_gswip_counters(tmp_inst);
	} else {
		pr_info("Usage: echo get_mib xgmac/ctp/bp/pmac > mib_count\n");
			pr_info("Usage: echo get_mib pmac <pmacid> > mib_count\n");
		pr_info("Usage: echo clear > mib_count\n");
	}
	kfree(str);
	return count;
}
/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP

#endif

