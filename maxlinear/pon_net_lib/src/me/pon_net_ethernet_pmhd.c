/*****************************************************************************
 *
 * Copyright (c) 2021 - 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <pon_adapter.h>
#include <omci/me/pon_adapter_ethernet_pmhd.h>

#include "../pon_net_debug.h"
#include "../pon_net_netlink.h"
#include "../pon_net_common.h"
#include "../pon_net_ethtool.h"

struct counter_cfg {
	const char *name;
	uint16_t offset;
};
#define COUNTER_CFG(s, n, o) { \
	.name = n, \
	.offset = offsetof(s, o)}

#define EXT_CNT_CFG(n, o) COUNTER_CFG(struct pa_eth_ext_cnt, n, o)

static const struct counter_cfg ext_rx_counters[] = {
	EXT_CNT_CFG("RxDroppedPkts", drop_events),
	EXT_CNT_CFG("RxGoodBytes", octets),
	EXT_CNT_CFG("RxGoodPkts", frames),
	EXT_CNT_CFG("RxBroadcastPkts", bc_frames),
	EXT_CNT_CFG("RxMulticastPkts", mc_frames),
	EXT_CNT_CFG("RxFCSErrorPkts", crc_err_frames),
	EXT_CNT_CFG("RxUnderSizeGoodPkts", usize_frames),
	EXT_CNT_CFG("RxOversizeGoodPkts", osize_frames),
	EXT_CNT_CFG("Rx64BytePkts", frames_64),
	EXT_CNT_CFG("Rx127BytePkts", frames_65_127),
	EXT_CNT_CFG("Rx255BytePkts", frames_128_255),
	EXT_CNT_CFG("Rx511BytePkts", frames_256_511),
	EXT_CNT_CFG("Rx1023BytePkts", frames_512_1023),
	EXT_CNT_CFG("RxMaxBytePkts", frames_1024_1518),
};

static const struct counter_cfg ext_tx_counters[] = {
	EXT_CNT_CFG("TxDroppedPkts", drop_events),
	EXT_CNT_CFG("TxGoodBytes", octets),
	EXT_CNT_CFG("TxGoodPkts", frames),
	EXT_CNT_CFG("TxBroadcastPkts", bc_frames),
	EXT_CNT_CFG("TxMulticastPkts", mc_frames),
	EXT_CNT_CFG("TxUnderSizeGoodPkts", usize_frames),
	EXT_CNT_CFG("TxOversizeGoodPkts", osize_frames),
	EXT_CNT_CFG("Tx64BytePkts", frames_64),
	EXT_CNT_CFG("Tx127BytePkts", frames_65_127),
	EXT_CNT_CFG("Tx255BytePkts", frames_128_255),
	EXT_CNT_CFG("Tx511BytePkts", frames_256_511),
	EXT_CNT_CFG("Tx1023BytePkts", frames_512_1023),
	EXT_CNT_CFG("TxMaxBytePkts", frames_1024_1518),
};

#define CNT_CFG(n, o) COUNTER_CFG(struct pa_eth_cnt, n, o)

static const struct counter_cfg counters[] = {
	CNT_CFG("RxFCSErrorPkts", cnt_fcs_error),
	CNT_CFG("TxExcessCollCount", cnt_excessive_collisions),
	CNT_CFG("TxLateCollCount", cnt_late_collisions),
	CNT_CFG("MtuExceedDiscardPkts", cnt_frames_too_long),
	CNT_CFG("RxOverflowError", cnt_rx_buffer_overflow_events),
	/* CNT_CFG("", cnt_tx_buffer_overflow_events), */
	CNT_CFG("TxSingleCollCount", cnt_single_collisions),
	CNT_CFG("TxMultCollCount", cnt_multiple_collisions),
	/* CNT_CFG("", cnt_sqe_test), */
	CNT_CFG("TxPauseCount", cnt_deferred_transmissions),
	/* CNT_CFG("", cnt_tx_mac_errors), */
	/* CNT_CFG("", cnt_carrier_sense_errors), */
	CNT_CFG("RxAlignErrorPkts", cnt_alignment_error),
	/* CNT_CFG("", cnt_rx_mac_errors), */
};

static void
ethtool_index_parse(struct ethtool_gstrings *strings,
		    const struct counter_cfg *cfg,
		    int *cnt_idx)
{
	int idx;

	idx = pon_net_ethtool_find_string_idx(strings, cfg->name);
	if (idx < 0) {
		dbg_msg("%s: could not parse %s\n", __func__,
			cfg->name);
		return;
	}

	*cnt_idx = idx;
}

static enum pon_adapter_errno
ethtool_strings_parse(struct pon_net_context *ctx,
		      const char *ifname,
		      const struct counter_cfg *cfg,
		      uint32_t cfg_len,
		      struct pon_net_cnt_idx *cnt_idxs)
{
	struct ethtool_gstrings *strings;
	uint32_t i;

	dbg_in_args("%p, \"%s\", %p, %u, %p", ctx, ifname, cfg, cfg_len,
		    cnt_idxs);

	if (cnt_idxs->parsed) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	strings = pon_net_ethtool_strings_get(ctx, ifname, ETH_SS_STATS);
	if (!strings) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_IF_NOT_FOUND);
		return PON_ADAPTER_ERR_IF_NOT_FOUND;
	}

	if (cfg_len > ARRAY_SIZE(cnt_idxs->indexes)) {
		free(strings);
		dbg_err("too many ethtool counters configured for parsing (%d)",
			cfg_len);
		dbg_out_ret("%d", PON_ADAPTER_ERR_OUT_OF_BOUNDS);
		return PON_ADAPTER_ERR_OUT_OF_BOUNDS;
	}

	for (i = 0; i < cfg_len; i++)
		ethtool_index_parse(strings, &cfg[i], &cnt_idxs->indexes[i]);

	free(strings);
	cnt_idxs->parsed = true;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static void
ethtool_counter_get(struct ethtool_stats *stats,
		    const struct counter_cfg *cfg,
		    void *cnt,
		    int idx)
{
	uint64_t *p_cnt = (uint64_t *)((uint8_t *)cnt + cfg->offset);

	if (idx <= ETH_PMHD_CNT_IDX_UNKNOWN)
		return;

	*p_cnt += stats->data[idx];
}

static enum pon_adapter_errno
ethtool_counters_parse(struct pon_net_context *ctx,
		       const char *ifname,
		       const struct counter_cfg *cfg,
		       uint32_t cfg_len,
		       void *cnt,
		       const struct pon_net_cnt_idx *cnt_idxs)
{
	struct ethtool_stats *stats;
	uint32_t i;

	dbg_in_args("%p, \"%s\", %p, %u, %p, %p", ctx, ifname, cfg, cfg_len,
		    cnt, cnt_idxs);

	stats = pon_net_ethtool_stats_get(ctx, ifname);
	if (!stats) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_IF_NOT_FOUND);
		return PON_ADAPTER_ERR_IF_NOT_FOUND;
	}

	if (cfg_len > ARRAY_SIZE(cnt_idxs->indexes)) {
		free(stats);
		dbg_err("too many ethtool counters configured for parsing (%d)",
			cfg_len);
		dbg_out_ret("%d", PON_ADAPTER_ERR_OUT_OF_BOUNDS);
		return PON_ADAPTER_ERR_OUT_OF_BOUNDS;
	}

	for (i = 0; i < cfg_len; i++)
		ethtool_counter_get(stats, &cfg[i], cnt, cnt_idxs->indexes[i]);

	free(stats);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ethtool_get_counters(struct pon_net_context *ctx, const char *ifname,
		     const struct counter_cfg *cfg, uint32_t cfg_len,
		     struct pon_net_cnt_idx *cnt_idxs, void *dst)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %p, %u, %p, %p", ctx, ifname, cfg, cfg_len,
		    cnt_idxs, dst);

	ret = ethtool_strings_parse(ctx, ifname, cfg, cfg_len, cnt_idxs);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	ret = ethtool_counters_parse(ctx, ifname, cfg, cfg_len, dst, cnt_idxs);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
eth_ext_cnt_stats_parse(struct pon_net_context *ctx,
			const char *ifname,
			struct pa_eth_ext_cnt *cnt,
			bool tx)
{
	const struct counter_cfg *cfg;
	uint32_t cfg_len;
	struct pon_net_cnt_idx *cnt_idxs;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\", %p, %d", ctx, ifname, cnt, tx);

	PON_NET_STATIC_ASSERT(
		ARRAY_SIZE(ext_tx_counters) <= ARRAY_SIZE(cnt_idxs->indexes),
		ext_tx_counters_exceeed_maximum_number_of_counter_indexes);
	PON_NET_STATIC_ASSERT(
		ARRAY_SIZE(ext_rx_counters) <= ARRAY_SIZE(cnt_idxs->indexes),
		ext_rx_counters_exceeed_maximum_number_of_counter_indexes);

	if (tx) {
		cfg = ext_tx_counters;
		cfg_len = ARRAY_SIZE(ext_tx_counters);
		cnt_idxs = &ctx->eth_ext_pmhd_cnt_idx.tx;
	} else {
		cfg = ext_rx_counters;
		cfg_len = ARRAY_SIZE(ext_rx_counters);
		cnt_idxs = &ctx->eth_ext_pmhd_cnt_idx.rx;
	}

	ret = ethtool_get_counters(ctx, ifname, cfg, cfg_len, cnt_idxs, cnt);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Read Multicast and Broadcast counters for PPTP_ETHERNET_UNI. */
static enum pon_adapter_errno pptp_eth_uni_ext_cnt_get(
		struct pon_net_context *ctx,
		const uint16_t me_id,
		bool tx,
		struct pa_eth_ext_cnt *eth_ext_cnt)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE];
	uint16_t lport;

	dbg_in_args("%p, %u, %d, %p", ctx, me_id, tx, eth_ext_cnt);

	/* Read only TX counters */
	if (!tx) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_uni_lport_get(me_id, &lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_uni_ifname_get(ctx, UNI_PPTP_MC, lport, ifname,
				     sizeof(ifname));
	if (ret == PON_ADAPTER_SUCCESS)
		(void)eth_ext_cnt_stats_parse(ctx, ifname, eth_ext_cnt, tx);

	ret = pon_net_uni_ifname_get(ctx, UNI_PPTP_BC, lport, ifname,
				     sizeof(ifname));
	if (ret == PON_ADAPTER_SUCCESS)
		(void)eth_ext_cnt_stats_parse(ctx, ifname, eth_ext_cnt, tx);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Read Multicast and Broadcast counters for VEIP. */
static enum pon_adapter_errno veip_ext_cnt_get(
		struct pon_net_context *ctx,
		const uint16_t me_id,
		bool tx,
		struct pa_eth_ext_cnt *eth_ext_cnt)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE];
	uint16_t lport;

	dbg_in_args("%p, %u, %d, %p", ctx, me_id, tx, eth_ext_cnt);

	/* Read only TX counters */
	if (!tx) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_uni_lport_get(me_id, &lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_uni_ifname_get(ctx, UNI_VEIP_MC, lport, ifname,
				     sizeof(ifname));
	if (ret == PON_ADAPTER_SUCCESS)
		(void)eth_ext_cnt_stats_parse(ctx, ifname, eth_ext_cnt, tx);

	ret = pon_net_uni_ifname_get(ctx, UNI_VEIP_BC, lport, ifname,
				     sizeof(ifname));
	if (ret == PON_ADAPTER_SUCCESS)
		(void)eth_ext_cnt_stats_parse(ctx, ifname, eth_ext_cnt, tx);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno eth_ext_cnt_get(
		void *ll_handle,
		const uint16_t parent_class_id,
		const uint16_t parent_me_id,
		const enum pon_adapter_eth_ext_pmhd_dir dir,
		struct pa_eth_ext_cnt *eth_ext_cnt)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE];
	bool tx = dir == PON_ADAPTER_ETH_EXT_PMHD_DIR_DS;

	dbg_in_args("%p, %u, %u, %d, %p", ll_handle,
		    parent_class_id, parent_me_id,
		    dir, eth_ext_cnt);

	/* Swap direction for ANI counters */
	if (parent_class_id == PON_CLASS_ID_ANI_G)
		tx = !tx;

	ret = pon_net_ifname_get(ctx, parent_class_id, parent_me_id,
				 ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_IF_NOT_FOUND);
		return PON_ADAPTER_ERR_IF_NOT_FOUND;
	}

	ret = eth_ext_cnt_stats_parse(ctx, ifname, eth_ext_cnt, tx);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_IF_NOT_FOUND);
		return PON_ADAPTER_ERR_IF_NOT_FOUND;
	}

	switch (parent_class_id) {
	case PON_CLASS_ID_PPTP_ETHERNET_UNI:
		ret = pptp_eth_uni_ext_cnt_get(ctx, parent_me_id, tx,
					       eth_ext_cnt);
		dbg_out_ret("%d", ret);
		return ret;
	case PON_CLASS_ID_VEIP:
		ret = veip_ext_cnt_get(ctx, parent_me_id, tx, eth_ext_cnt);
		dbg_out_ret("%d", ret);
		return ret;
	default:
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno eth_cnt_get(void *ll_handle, uint16_t me_id,
					  struct pa_eth_cnt *eth_cnt)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE];
	struct pon_net_cnt_idx *cnt_idxs = &ctx->eth_pmhd_cnt_idx;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, eth_cnt);

	PON_NET_STATIC_ASSERT(
		ARRAY_SIZE(counters) <= ARRAY_SIZE(cnt_idxs->indexes),
		eth_counters_exceeed_maximum_number_of_counter_indexes);

	ret = pon_net_ifname_get(ctx, PON_CLASS_ID_PPTP_ETHERNET_UNI, me_id,
				 ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_IF_NOT_FOUND);
		return PON_ADAPTER_ERR_IF_NOT_FOUND;
	}

	ret = ethtool_get_counters(ctx, ifname, counters, ARRAY_SIZE(counters),
				   cnt_idxs, eth_cnt);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_eth_pmhd_ops eth_pmhd_ops = {
	.eth_ext_cnt_get = eth_ext_cnt_get,
	.eth_cnt_get = eth_cnt_get,
};

