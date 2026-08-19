/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2019 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdint.h>

#include <sys/socket.h>
#include <net/if.h>
#include <linux/sockios.h>

#include "pon_net_ethtool.h"
#include "pon_net_common.h"
#include "pon_net_debug.h"
#include "pon_net_extern.h"

enum pon_adapter_errno
pon_net_ethtool(struct pon_net_context *ctx,
		const char *ifname,
		uint32_t cmd,
		void *data)
{

	struct ifreq ifreq = { 0 };
	int err;

	dbg_in_args("%p, \"%s\", %d, %p", ctx, ifname, cmd, data);

	((struct ethtool_value *)data)->cmd = cmd;
	ifreq.ifr_data = (char *)data;
	snprintf(ifreq.ifr_name, sizeof(ifreq.ifr_name), "%s", ifname);
	err = pon_net_ioctl(ctx->ioctl_fd, SIOCETHTOOL, &ifreq);
	if (err) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);

	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_ethtool_pause_frames_tx_disable(struct pon_net_context *ctx,
					const char *ifname)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct ethtool_pauseparam pauseparam;

	dbg_in_args("%p, \"%s\"", ctx, ifname);

	/* Read */
	ret = pon_net_ethtool(ctx, ifname, ETHTOOL_GPAUSEPARAM, &pauseparam);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, ETHTOOL_GPAUSEPARAM, ret);
		return ret;
	}

	/* Modify */
	pauseparam.tx_pause = 0;
	pauseparam.autoneg = 0;

	/* Write */
	ret = pon_net_ethtool(ctx, ifname, ETHTOOL_SPAUSEPARAM, &pauseparam);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, ETHTOOL_SPAUSEPARAM, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static uint32_t
ethtool_sset_len_get(struct pon_net_context *ctx,
		     const char *ifname,
		     enum ethtool_stringset set_id)
{
	enum pon_adapter_errno ret;
	struct {
		struct ethtool_sset_info hdr;
		uint32_t buf; /* will store size of sset */
	} sset_info = { 0 };

	sset_info.hdr.sset_mask = 1ULL << set_id;
	ret = pon_net_ethtool(ctx, ifname, ETHTOOL_GSSET_INFO, &sset_info);
	if (!ret && sset_info.hdr.sset_mask)
		return sset_info.buf;

	return 0;
}

struct ethtool_stats *
pon_net_ethtool_stats_get(struct pon_net_context *ctx,
			  const char *ifname)
{
	enum pon_adapter_errno ret;
	struct ethtool_stats *stats;
	uint32_t len;

	len = ethtool_sset_len_get(ctx, ifname, ETH_SS_STATS);
	if (!len)
		return NULL;

	stats = calloc(1, sizeof(*stats) + len * sizeof(uint64_t));
	if (!stats)
		return NULL;

	ret = pon_net_ethtool(ctx, ifname, ETHTOOL_GSTATS, stats);
	if (ret) {
		free(stats);
		return NULL;
	}

	return stats;
}

struct ethtool_gstrings *
pon_net_ethtool_strings_get(struct pon_net_context *ctx,
			    const char *ifname,
			    enum ethtool_stringset set_id)
{
	enum pon_adapter_errno ret;
	struct ethtool_gstrings *strings;
	uint32_t len;

	len = ethtool_sset_len_get(ctx, ifname, set_id);
	if (!len)
		return NULL;

	strings = calloc(1, sizeof(*strings) + len * ETH_GSTRING_LEN);
	if (!strings)
		return NULL;

	strings->string_set = set_id;
	strings->len = len;
	ret = pon_net_ethtool(ctx, ifname, ETHTOOL_GSTRINGS, strings);
	if (ret) {
		free(strings);
		return NULL;
	}

	return strings;
}

#define STRINGS_LEN_MAX 100000
int pon_net_ethtool_find_string_idx(const struct ethtool_gstrings *strings,
				    const char *str)
{
	int i;
	int len_max;

	len_max = (strings->len > STRINGS_LEN_MAX) ?
				STRINGS_LEN_MAX : (int)strings->len;

	for (i = 0; i < len_max; i++) {
		if (strncmp(str,
			    ((const char *)strings->data + i * ETH_GSTRING_LEN),
			    ETH_GSTRING_LEN) == 0)
			return i;
	}

	return -1;
}

enum pon_adapter_errno
pon_net_ethtool_priv_flag_set(struct pon_net_context *ctx,
			      const char *ifname,
			      const char *flagname,
			      uint8_t enable)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct ethtool_gstrings *strings;
	int idx;
	struct ethtool_value flags = { 0 };

	dbg_in_args("%p, \"%s\", \"%s\", %u", ctx, ifname, flagname, enable);

	/* find flag in strings, no error if not found,
	 * as not all drivers supports this flag
	 */
	strings = pon_net_ethtool_strings_get(ctx, ifname, ETH_SS_PRIV_FLAGS);
	if (!strings) {
		dbg_msg("No 'priv flags' strings found for %s\n", ifname);
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}
	idx = pon_net_ethtool_find_string_idx(strings, flagname);
	if (idx < 0 || idx >= 32) {
		free(strings);
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}
	free(strings);

	ret = pon_net_ethtool(ctx, ifname, ETHTOOL_GPFLAGS, &flags);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, ETHTOOL_GPFLAGS, ret);
		return ret;
	}

	if (enable)
		flags.data |= 1U << idx;
	else
		flags.data &= ~(1U << idx);

	ret = pon_net_ethtool(ctx, ifname, ETHTOOL_SPFLAGS, &flags);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, ETHTOOL_SPFLAGS, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_ethtool_link_settings_get(struct pon_net_context *ctx,
				  const char *ifname,
				  struct pon_ethtool_link_settings
					*link_settings)
{
	enum pon_adapter_errno ret;

	ret = pon_net_ethtool(ctx, ifname, ETHTOOL_GLINKSETTINGS,
			      link_settings);
	if (ret == PON_ADAPTER_SUCCESS &&
	    link_settings->req.link_mode_masks_nwords < 0 &&
	    link_settings->req.link_mode_masks_nwords >=
						-ETHTOOL_LINK_MODE_NUM_U32) {
		link_settings->req.link_mode_masks_nwords =
			(int8_t)(-link_settings->req.link_mode_masks_nwords);
		ret = pon_net_ethtool(ctx, ifname, ETHTOOL_GLINKSETTINGS,
				      link_settings);
	}
	if (ret != PON_ADAPTER_SUCCESS ||
	    link_settings->req.link_mode_masks_nwords <= 0) {
		dbg_wrn_fn_ret(ETHTOOL_GLINKSETTINGS, ret);
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}
