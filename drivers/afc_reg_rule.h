/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "nl80211.h"

#define REGLIB_MHZ_TO_KHZ(freq) ((freq) * 1000)
#define REGLIB_KHZ_TO_MHZ(freq) ((freq) / 1000)
#define EIRP_UNIT_CONVERSION 100
#define PSD_TO_EIRP_CONVERSION(psd,bw) (psd + (10 * (log10(bw))))

struct ieee80211_freq_range {
	uint32_t  start_freq_khz;
	uint32_t  end_freq_khz;
	uint32_t  max_bandwidth_khz;
};

struct ieee80211_power_rule {
	uint32_t  max_antenna_gain;
	uint32_t  max_eirp;
};

struct ieee80211_wmm_ac {
	uint16_t  cw_min;
	uint16_t cw_max;
	uint16_t cot;
	uint8_t aifsn;
};

struct ieee80211_wmm_rule {
	struct ieee80211_wmm_ac client[4];
	struct ieee80211_wmm_ac ap[4];
};

struct ieee80211_reg_rule {
	struct ieee80211_freq_range freq_range;
	struct ieee80211_power_rule power_rule;
	struct ieee80211_wmm_rule wmm_rule;
	uint32_t  flags;
	uint32_t  dfs_cac_ms;
	bool has_wmm;
};

struct callback_head {
	struct callback_head *next;
	void (*func)(struct callback_head *head);
} __attribute__((aligned(sizeof(void *))));
#define rcu_head callback_head

struct mxl_ieee80211_regdomain {
	struct rcu_head rcu_head;
	uint32_t n_reg_rules;
	char alpha2[3];
	enum nl80211_dfs_regions dfs_region;
	struct ieee80211_reg_rule reg_rules[];
};

int afc_construct_regrule_from_afc_response(void *data);