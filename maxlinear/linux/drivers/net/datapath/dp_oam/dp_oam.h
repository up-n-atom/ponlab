/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2021 - 2024 MaxLinear, Inc.
 *
 *****************************************************************************/
#ifndef _DP_OAM_
#define _DP_OAM_

/** DP OAM action: extract */
#define DP_OAM_ACT_EXTRACT 1
/** DP OAM action: drop */
#define DP_OAM_ACT_DROP 2
/** DP OAM action: counter */
#define DP_OAM_ACT_COUNTER 3
/** DP OAM action: rmon */
#define DP_OAM_ACT_RMON 4
/** DP OAM action: timestamp */
#define DP_OAM_ACT_TIMESTAMP 5
/** DP OAM action: max */
#define DP_OAM_ACT_SIZE_MAX 8

/** Extraction points */
enum dp_oam_ext_pts {
	DP_OAM_EXT_NO_VLAN = 0,
	DP_OAM_EXT_VLAN = 1,
	DP_OAM_EXT_Q_NO_VLAN = 2,
	DP_OAM_EXT_Q_VLAN = 3
};

/** Timestamping format */
enum dp_oam_time_format {
	DP_OAM_TS_DIGITAL_10B = 0,
	DP_OAM_TS_BINARY_10B = 1,
	DP_OAM_TS_DIGITAL_8B = 2,
	DP_OAM_TS_BINARY_8B = 3
};

/** Action arguments */
struct dp_oam_args {
	/** OAM action */
	DECLARE_BITMAP(act, DP_OAM_ACT_SIZE_MAX);

	/** Use specified netdevice,
	 * if not used configure globally on all ports.
	 */
	bool dev_en;
	/** Netdevice ifindex */
	int ifindex;
	/** Configuration per netdevice port */
	bool global;

	/** Extraction point */
	enum dp_oam_ext_pts ext_pt;

	/** Use OAM level */
	bool level_en;
	/** OAM Level
	 *  - represents target level for extraction
	 *  - represents maximum level range for drop
	 */
	u8 level;

	/** Use OAM operational code */
	bool opcode_en;
	/** OAM operational code */
	u8 opcode;

	/** Use VLAN information */
	bool vlan_en;
	/** VLAN id */
	u16 vlan_id;
	/** untagged packets only */
	bool untagged;

	/** counter id
	 * [out] for counter action
	 * [in] for rmon action
	 */
	u32 counter_id;
	/** OAM counter direction (false: RX, true: TX) */
	bool rmon_tx;
	/** OAM bypass transmit counter */
	bool rmon_bypass;
	/** Byte offset (2~255) to insert counter */
	u32 rmon_offset;

	/** Timestamping format */
	enum dp_oam_time_format ts_format;
	/** Byte offset (2~255) to insert timestamp */
	u32 ts_offset;
	/** Signed time compensation value for OAM delay measurement.
	 *  The valid values are -4,294,967,295 ~ 4,294,967,295.
	 */
	long long ts_comp;

	/** OAM rule priority (optional, default: 0) */
	u32 prio;
	/** OAM rule index - returned after success */
	u32 index;
};

int dp_oam_act_add(struct dp_oam_args *args);
int dp_oam_act_del(struct dp_oam_args *args);

extern int (*qos_tc_cfm_offload_fn)(struct dp_oam_args *args);
extern int (*qos_tc_cfm_unoffload_fn)(struct dp_oam_args *args);

#if IS_ENABLED(CONFIG_QOS_TC)

void dp_oam_tc_hook_fn_set(void);
void dp_oam_tc_hook_fn_unset(void);

#elif !IS_ENABLED(CONFIG_QOS_TC)

static void dp_oam_tc_hook_fn_set(void)
{
}

static void dp_oam_tc_hook_fn_unset(void)
{
}

#endif

#endif
