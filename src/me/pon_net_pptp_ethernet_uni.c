/*****************************************************************************
 *
 * Copyright (c) 2020 - 2026 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <netlink/route/link.h>
#include <netlink/route/qdisc.h>
#include <pon_adapter.h>

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "../pon_net_netlink.h"
#include "../pon_net_ethtool.h"
#include "../pon_net_uni.h"
#include <omci/me/pon_adapter_pptp_ethernet_uni.h>

#include <omci/me/pon_adapter_mac_bridge_port_config_data.h>

/* The "printf" in the following macros should be optimized out on correct
 * usage. If someone tries to define a mode in the wrong index, this will give
 * a compilation error.
 */
#define LINK_MODE_MASK0(base_name)	\
	((ETHTOOL_LINK_MODE_ ## base_name ## _BIT < 32) ? \
	(1UL << ETHTOOL_LINK_MODE_ ## base_name ## _BIT) : \
	(uint32_t)printf("invalid"))
#define LINK_MODE_MASK1(base_name)	\
	((ETHTOOL_LINK_MODE_ ## base_name ## _BIT < 32) ? \
	(uint32_t)printf("invalid") : \
	(1UL << (ETHTOOL_LINK_MODE_ ## base_name ## _BIT - 32)))

static const struct ethernet_port_cfg {
	enum pa_pptp_eth_uni_auto_detect_cfg value;
	/* The speed of the link - useful only if autoneg is AUTONEG_DISABLE */
	int32_t speed;
	/* The duplex of the link - useful only if autoneg is AUTONEG_DISABLE */
	uint8_t duplex;
	/* If autoneg is AUTONEG_ENABLE, speed and duplex are ignored
	   and advertising is used */
	uint32_t advertising[ETHTOOL_LINK_MODE_NUM_U32];
} ethernet_port_cfgs[] = {
	{
		.value = PA_PPTPETHUNI_DETECT_AUTO,
		.speed = SPEED_UNKNOWN,
		.duplex = DUPLEX_UNKNOWN,
		.advertising = {
			LINK_MODE_MASK0(10baseT_Full) |
			LINK_MODE_MASK0(100baseT_Full) |
			LINK_MODE_MASK0(1000baseT_Full) |
			LINK_MODE_MASK0(2500baseX_Full) |
			LINK_MODE_MASK0(10000baseT_Full) |
			LINK_MODE_MASK0(10000baseKR_Full) |
			LINK_MODE_MASK0(10baseT_Half) |
			LINK_MODE_MASK0(100baseT_Half) |
			LINK_MODE_MASK0(1000baseT_Half) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			LINK_MODE_MASK1(1000baseX_Full) |
			LINK_MODE_MASK1(2500baseT_Full) |
			LINK_MODE_MASK1(5000baseT_Full) |
			LINK_MODE_MASK1(10000baseCR_Full) |
			LINK_MODE_MASK1(10000baseSR_Full) |
			LINK_MODE_MASK1(10000baseLR_Full)
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_10_FULL,
		.speed = SPEED_10,
		.duplex = DUPLEX_FULL,
		.advertising = {
			LINK_MODE_MASK0(10baseT_Full) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			0
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_100_FULL,
		.speed = SPEED_100,
		.duplex = DUPLEX_FULL,
		.advertising = {
			LINK_MODE_MASK0(100baseT_Full) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			0
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_1000_FULL,
		.speed = SPEED_1000,
		.duplex = DUPLEX_FULL,
		.advertising = {
			LINK_MODE_MASK0(1000baseT_Full) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			LINK_MODE_MASK1(1000baseX_Full)
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_10000_FULL,
		.speed = SPEED_10000,
		.duplex = DUPLEX_FULL,
		.advertising = {
			LINK_MODE_MASK0(10000baseT_Full) |
			LINK_MODE_MASK0(10000baseKR_Full) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			LINK_MODE_MASK1(10000baseCR_Full) |
			LINK_MODE_MASK1(10000baseSR_Full) |
			LINK_MODE_MASK1(10000baseLR_Full)
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_2500_FULL,
		.speed = SPEED_2500,
		.duplex = DUPLEX_FULL,
		.advertising = {
			LINK_MODE_MASK0(2500baseX_Full) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			LINK_MODE_MASK1(2500baseT_Full)
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_5000_FULL,
		.speed = SPEED_5000,
		.duplex = DUPLEX_FULL,
		.advertising = {
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			LINK_MODE_MASK1(5000baseT_Full)
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_25000_FULL,
		.speed = SPEED_25000,
		.duplex = DUPLEX_FULL,
		.advertising = {
			LINK_MODE_MASK0(25000baseCR_Full) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			LINK_MODE_MASK1(25000baseKR_Full) |
			LINK_MODE_MASK1(25000baseSR_Full)
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_40000_FULL,
		.speed = SPEED_40000,
		.duplex = DUPLEX_FULL,
		.advertising = {
			LINK_MODE_MASK0(40000baseKR4_Full) |
			LINK_MODE_MASK0(40000baseCR4_Full) |
			LINK_MODE_MASK0(40000baseSR4_Full) |
			LINK_MODE_MASK0(40000baseLR4_Full) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			0
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_AUTO_FULL,
		.speed = SPEED_UNKNOWN,
		.duplex = DUPLEX_FULL,
		.advertising = {
			LINK_MODE_MASK0(10baseT_Full) |
			LINK_MODE_MASK0(100baseT_Full) |
			LINK_MODE_MASK0(1000baseT_Full) |
			LINK_MODE_MASK0(2500baseX_Full) |
			LINK_MODE_MASK0(10000baseT_Full) |
			LINK_MODE_MASK0(10000baseKR_Full) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			LINK_MODE_MASK1(1000baseX_Full) |
			LINK_MODE_MASK1(2500baseT_Full) |
			LINK_MODE_MASK1(5000baseT_Full) |
			LINK_MODE_MASK1(10000baseCR_Full) |
			LINK_MODE_MASK1(10000baseSR_Full) |
			LINK_MODE_MASK1(10000baseLR_Full)
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_10_AUTO,
		.speed = SPEED_10,
		.duplex = DUPLEX_UNKNOWN,
		.advertising = {
			LINK_MODE_MASK0(10baseT_Full) |
			LINK_MODE_MASK0(10baseT_Half) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			0
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_10_HALF,
		.speed = SPEED_10,
		.duplex = DUPLEX_HALF,
		.advertising = {
			LINK_MODE_MASK0(10baseT_Half) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			0
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_100_HALF,
		.speed = SPEED_100,
		.duplex = DUPLEX_HALF,
		.advertising = {
			LINK_MODE_MASK0(100baseT_Half) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			0
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_1000_HALF,
		.speed = SPEED_1000,
		.duplex = DUPLEX_HALF,
		.advertising = {
			LINK_MODE_MASK0(1000baseT_Half) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			0
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_AUTO_HALF,
		.speed = SPEED_UNKNOWN,
		.duplex = DUPLEX_HALF,
		.advertising = {
			LINK_MODE_MASK0(10baseT_Half) |
			LINK_MODE_MASK0(100baseT_Half) |
			LINK_MODE_MASK0(1000baseT_Half) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			0
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_1000_AUTO,
		.speed = SPEED_1000,
		.duplex = DUPLEX_UNKNOWN,
		.advertising = {
			LINK_MODE_MASK0(1000baseT_Full) |
			LINK_MODE_MASK0(1000baseT_Half) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			LINK_MODE_MASK1(1000baseX_Full)
		}
	},
	{
		.value = PA_PPTPETHUNI_DETECT_100_AUTO,
		.speed = SPEED_100,
		.duplex = DUPLEX_UNKNOWN,
		.advertising = {
			LINK_MODE_MASK0(100baseT_Full) |
			LINK_MODE_MASK0(100baseT_Half) |
			LINK_MODE_MASK0(Autoneg) |
			LINK_MODE_MASK0(Pause) |
			LINK_MODE_MASK0(Asym_Pause),
			0
		}
	},
};

static const struct ethernet_port_cfg
*interpret_ethernet_port_cfg(enum pa_pptp_eth_uni_auto_detect_cfg value)
{
	const struct ethernet_port_cfg *cfg = &ethernet_port_cfgs[0];
	unsigned int i = 0;

	dbg_in_args("%d", value);

	for (i = 0; i < ARRAY_SIZE(ethernet_port_cfgs); ++i) {
		if (ethernet_port_cfgs[i].value == value) {
			cfg = &ethernet_port_cfgs[i];
			break;
		}
	}

	dbg_out_ret("%p", cfg);
	return cfg;
}

static enum pon_adapter_errno
apply_ethernet_port_cfg(struct pon_net_context *ctx,
			const char *ifname,
			const struct ethernet_port_cfg *cfg)
{
	enum pon_adapter_errno ret;
	struct pon_ethtool_link_settings link_settings = { 0 };
	uint32_t *advertising;
	int i, mode_masks_nwords;
	bool changed = false;

	dbg_in_args("%p, \"%s\", %p", ctx, ifname, cfg);

	ret = pon_net_ethtool_link_settings_get(ctx, ifname, &link_settings);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_wrn_fn_ret(pon_net_ethtool_link_settings_get, ret);
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	mode_masks_nwords = link_settings.req.link_mode_masks_nwords;
	/* advertising is the second parameter -> starts after
	 * mode_masks_nwords
	 */
	advertising = &link_settings.link_mode_data[mode_masks_nwords];
	for (i = 0; i < mode_masks_nwords; i++) {
		/* mask all autoneg values with the supported modes */
		advertising[i] =
			cfg->advertising[i] &
				link_settings.link_mode_data[i];
	}
	if (((*advertising & ~(LINK_MODE_MASK0(Autoneg) |
	    LINK_MODE_MASK0(Pause) | LINK_MODE_MASK0(Asym_Pause))) == 0) &&
	    (*(advertising + 1) == 0)) {
		dbg_err("no remaining advertising modes\n");
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	if (cfg->speed != SPEED_UNKNOWN &&
	    link_settings.req.speed != (unsigned int)cfg->speed) {
		link_settings.req.speed = (unsigned int)cfg->speed;
		changed = true;
	}
	if (cfg->duplex != DUPLEX_UNKNOWN &&
	    link_settings.req.duplex != cfg->duplex) {
		link_settings.req.duplex = cfg->duplex;
		changed = true;
	}

	ret = pon_net_ethtool(ctx, ifname, ETHTOOL_SLINKSETTINGS,
			      &link_settings);
	if (ret != PON_ADAPTER_SUCCESS && changed) {
		/* only error if speed/duplex cannot be changed */
		dbg_wrn_fn_ret(ETHTOOL_SLINKSETTINGS, ret);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
pptp_eth_uni_config(struct pon_net_context *ctx,
		    uint16_t me_id,
		    const struct pa_pptp_eth_uni_data *update_data)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	const struct ethernet_port_cfg *cfg;
	char ifname[IF_NAMESIZE];

	dbg_in_args("%p, %u, %p", ctx, me_id, update_data);

	ret = pon_net_ifname_get(ctx, PON_CLASS_ID_PPTP_ETHERNET_UNI, me_id,
				 ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_ifname_get, ret);
		return ret;
	}

	ret = netlink_netdevice_mtu_set(ctx->netlink,
					ifname,
					update_data->max_frame_size);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_netdevice_mtu_set, ret);
		return ret;
	}

	cfg = interpret_ethernet_port_cfg(update_data->auto_detect_cfg);
	ret = apply_ethernet_port_cfg(ctx, ifname, cfg);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, apply_ethernet_port_cfg, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
pptp_eth_uni_update(void *ll_handle,
		    uint16_t me_id,
		    const struct pa_pptp_eth_uni_data *update_data)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, update_data);

	ret = pptp_eth_uni_config(ctx, me_id, update_data);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pptp_eth_uni_config, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
pptp_eth_uni_create(void *ll_handle, uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u", ll_handle, me_id);

	ret = pon_net_uni_create(ll_handle, PON_CLASS_ID_PPTP_ETHERNET_UNI,
				 me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_uni_create, ret);
		return ret;
	}

	ret = pon_net_uni_eg_tc_reassign(ll_handle,
					 PON_CLASS_ID_PPTP_ETHERNET_UNI,
					 me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_uni_eg_tc_reassign, ret);
		return ret;
	}

	ret = pon_net_extern_uni_cfg(ll_handle, PON_CLASS_ID_PPTP_ETHERNET_UNI,
				  me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_extern_uni_cfg, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
pptp_eth_uni_destroy(void *ll_handle, uint16_t me_id)
{
	enum pon_adapter_errno ret, ret2;

	dbg_in_args("%p, %u", ll_handle, me_id);

	ret = pon_net_uni_eg_tc_reassign_clr(ll_handle,
					     PON_CLASS_ID_PPTP_ETHERNET_UNI,
					     me_id);
	if (ret != PON_ADAPTER_SUCCESS)
		FN_ERR_RET(ret, pon_net_uni_eg_tc_reassign_clr, ret);

	ret = pon_net_extern_uni_clr(ll_handle, PON_CLASS_ID_PPTP_ETHERNET_UNI,
				  me_id);
	if (ret != PON_ADAPTER_SUCCESS)
		FN_ERR_RET(ret, pon_net_extern_uni_clr, ret);

	ret2 = pon_net_uni_destroy(ll_handle, PON_CLASS_ID_PPTP_ETHERNET_UNI,
				  me_id);
	if (ret2 != PON_ADAPTER_SUCCESS)
		FN_ERR_RET(ret2, pon_net_uni_destroy, ret2);

	if (ret != PON_ADAPTER_SUCCESS || ret2 != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno pptp_eth_uni_lock(void *ll_handle,
						uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u", ll_handle, me_id);
	ret = pon_net_tp_state_set(ll_handle, PA_BP_TP_TYPE_PPTP_UNI, me_id,
				   ADMIN_STATE_LOCKED);
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno pptp_eth_uni_unlock(void *ll_handle,
						  uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u", ll_handle, me_id);
	ret = pon_net_tp_state_set(ll_handle, PA_BP_TP_TYPE_PPTP_UNI, me_id,
				   ADMIN_STATE_UNLOCKED);
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno pptp_eth_uni_lan_is_available(void *ll_handle,
						uint16_t me_id, bool *available)
{
	dbg_in_args("%p, %u", ll_handle, me_id);

	if (!available) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}
	*available = true;
	/* Check to the port will be added later */
	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno pptp_eth_uni_conf_ind_get(void *ll_handle,
							uint16_t me_id,
							uint8_t *cfg_ind)
{
	dbg_in_args("%p, %u, %p", ll_handle, me_id, cfg_ind);

	if (!cfg_ind) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}
	*cfg_ind = 0;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno pptp_eth_uni_oper_state_get(void *ll_handle,
							  uint16_t me_id,
							  uint8_t *oper_state)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE];
	bool is_up = false;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, oper_state);

	/* The OMCI operational state definition is inverted.
	 * The default value is "disabled" == "true".
	 */
	*oper_state = !is_up;

	ret = pon_net_ifname_get(ctx, PON_CLASS_ID_PPTP_ETHERNET_UNI, me_id,
				 ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_ifname_get, ret);
		return ret;
	}

	ret = netlink_netdevice_operstate_get(ctx->netlink, ifname, &is_up);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_netdevice_operstate_get, ret);
		return ret;
	}

	/* The OMCI operational state definition is inverted. */
	*oper_state = !is_up;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static bool has_common_bits(const uint32_t *mask1, const uint32_t *mask2)
{
	if ((mask1[0] & mask2[0]) != 0 || (mask1[1] & mask2[1]) != 0)
		return true;
	return false;
}

static enum pa_pptp_eth_uni_expected_type interpret_type(uint32_t *supported)
{
	enum pa_pptp_eth_uni_expected_type type =
		PA_PPTPETHUNI_EXPECTED_TYPE_NOLIM;
	static const uint32_t modes_10g[ETHTOOL_LINK_MODE_NUM_U32] = {
		LINK_MODE_MASK0(10000baseT_Full) |
		LINK_MODE_MASK0(10000baseKR_Full),
		LINK_MODE_MASK1(10000baseCR_Full) |
		LINK_MODE_MASK1(10000baseSR_Full) |
		LINK_MODE_MASK1(10000baseLR_Full)};
	static const uint32_t modes_2_5g[ETHTOOL_LINK_MODE_NUM_U32] = {
		LINK_MODE_MASK0(2500baseX_Full),
		LINK_MODE_MASK1(2500baseT_Full)};
	static const uint32_t modes_1g[ETHTOOL_LINK_MODE_NUM_U32] = {
		LINK_MODE_MASK0(1000baseT_Full) |
		LINK_MODE_MASK0(1000baseT_Half),
		LINK_MODE_MASK1(1000baseX_Full)};
	static const uint32_t modes_100m[ETHTOOL_LINK_MODE_NUM_U32] = {
		LINK_MODE_MASK0(100baseT_Full) |
		LINK_MODE_MASK0(100baseT_Half),
		0};
	static const uint32_t modes_10m[ETHTOOL_LINK_MODE_NUM_U32] = {
		LINK_MODE_MASK0(10baseT_Full) |
		LINK_MODE_MASK0(10baseT_Half),
		0};
	bool supports10, supports100, supports1000, supports2500, supports10000;

	dbg_in_args("%p", supported);

	supports10000 = has_common_bits(modes_10g, supported);
	supports2500 = has_common_bits(modes_2_5g, supported);
	supports1000 = has_common_bits(modes_1g, supported);
	supports100 = has_common_bits(modes_100m, supported);
	supports10 = has_common_bits(modes_10m, supported);

	if (supports10000)
		type = PA_PPTPETHUNI_EXPECTED_TYPE_10G_ETH;
	else if (supports2500)
		type = PA_PPTPETHUNI_EXPECTED_TYPE_2_5G_ETH;
	else if (supports10 && supports100 && supports1000)
		type = PA_PPTPETHUNI_EXPECTED_TYPE_10_100_1000BASET;
	else if (supports10 && supports100)
		type = PA_PPTPETHUNI_EXPECTED_TYPE_10_100BASET;
	else if (supports100)
		type = PA_PPTPETHUNI_EXPECTED_TYPE_100BASET;
	else if (supports10)
		type = PA_PPTPETHUNI_EXPECTED_TYPE_10BASET;

	dbg_out_ret("%d", type);
	return type;
}

static enum pon_adapter_errno
pptp_eth_uni_sensed_type_get(void *ll_handle,
			     uint16_t me_id,
			     uint8_t *sensed_type)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret;
	char ifname[IF_NAMESIZE] = "";
	struct pon_ethtool_link_settings link_settings = { 0 };

	dbg_in_args("%p, %u, %p", ll_handle, me_id, sensed_type);

	ret = pon_net_ifname_get(ctx, PON_CLASS_ID_PPTP_ETHERNET_UNI, me_id,
				 ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_ifname_get, ret);
		return ret;
	}

	ret = pon_net_ethtool_link_settings_get(ctx, ifname, &link_settings);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_wrn_fn_ret(pon_net_ethtool_link_settings_get, ret);
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	*sensed_type = interpret_type(link_settings.link_mode_data);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
pptp_eth_uni_lan_port_enable(void *ll_handle,
			     const uint16_t me_id,
			     const bool enable)
{
	dbg_in_args("%p, %u, %d", ll_handle, me_id, enable);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_pptp_eth_uni_ops pptp_eth_uni_ops = {
	.lock = pptp_eth_uni_lock,
	.unlock = pptp_eth_uni_unlock,
	.lan_is_available = pptp_eth_uni_lan_is_available,
	.create = pptp_eth_uni_create,
	.destroy = pptp_eth_uni_destroy,
	.update = pptp_eth_uni_update,
	.conf_ind_get = pptp_eth_uni_conf_ind_get,
	.oper_state_get = pptp_eth_uni_oper_state_get,
	.sensed_type_get = pptp_eth_uni_sensed_type_get,
	.lan_port_enable = pptp_eth_uni_lan_port_enable,
};
