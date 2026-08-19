/******************************************************************************

		 Copyright (c) 2023-2024, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "afc_reg_rule.h"
#include "afc.h"
#include "afc_nl80211.h"
#include "utils.h"

struct mxl_ieee80211_regdomain *regd;

static uint32_t afc_global_op_class_to_bw_khz(uint16_t global_op_cls)
{
	uint32_t bw;

	/* Ref: IEEE 802.11-20/0646r0 */
	switch (global_op_cls) {
	case 131:
		bw = REGLIB_MHZ_TO_KHZ(20);
		break;
	case 132:
		bw = REGLIB_MHZ_TO_KHZ(40);
		break;
	case 133:
		bw = REGLIB_MHZ_TO_KHZ(80);
		break;
	case 134:
		bw = REGLIB_MHZ_TO_KHZ(160);
		break;
	case 137:
		bw = REGLIB_MHZ_TO_KHZ(320);
		break;
	default:
		bw = 0;
		break;
	}

	return bw;
}

static uint32_t afc_6ghz_channel_to_freq(int chan)
{
	uint32_t center_freq = 0;

	if (chan == 2)
		center_freq = 5935;
	if (chan <= 253)
		center_freq = (5950 + chan * 5);

	return center_freq;
}

static uint32_t afc_calculate_6ghz_bandwidth(uint16_t low_freq, uint16_t high_freq)
{
	uint32_t bw = 0;
	uint32_t diff = high_freq - low_freq;

	if (diff == 20 || diff == 40 || diff == 80 || diff == 160 || diff == 320)
		bw = REGLIB_MHZ_TO_KHZ(diff);

	return bw;
}

static uint32_t afc_calculate_psd_to_eirp(double psd, uint32_t bw)
{
	uint32_t eirp;

	bw = REGLIB_KHZ_TO_MHZ(bw);
	eirp = (uint32_t)(PSD_TO_EIRP_CONVERSION(psd,bw));

	return eirp;
}

static void afc_add_regulatory_rule(uint32_t idx, uint32_t start_freq_khz,
									uint32_t end_freq_khz, uint32_t bw, uint32_t eirp)
{
	if (start_freq_khz && end_freq_khz && bw && eirp) {
		regd->reg_rules[idx].freq_range.start_freq_khz = start_freq_khz;
		regd->reg_rules[idx].freq_range.end_freq_khz = end_freq_khz;
		regd->reg_rules[idx].freq_range.max_bandwidth_khz = bw;
		regd->reg_rules[idx].power_rule.max_eirp = (eirp * EIRP_UNIT_CONVERSION);
	}
}

static void afc_process_chan_regrule_info(struct afc_spectrum_inquiry_resp *afc_response)
{
	uint32_t num_chan_arr;
	uint32_t chan_idx, reg_idx = 0;
	uint32_t bw_khz;
	uint32_t start_freq_khz;
	uint32_t end_freq_khz;
	uint32_t max_eirp;
	uint32_t center_freq;

	for (num_chan_arr = 0; num_chan_arr < afc_response->num_chan_info; num_chan_arr++) {
		for (chan_idx = 0; chan_idx < afc_response->chan_info[num_chan_arr].num_chan_cfi; chan_idx++) {
			bw_khz = afc_global_op_class_to_bw_khz(afc_response->chan_info[num_chan_arr].global_op_class);
			if (!bw_khz)
				continue;

			center_freq = afc_6ghz_channel_to_freq(afc_response->chan_info[num_chan_arr].channel_cfi[chan_idx]);
			if (!center_freq)
				continue;

			start_freq_khz = center_freq - (REGLIB_KHZ_TO_MHZ(bw_khz) / 2);
			end_freq_khz = center_freq + (REGLIB_KHZ_TO_MHZ(bw_khz) / 2);
			max_eirp = (uint32_t)(afc_response->chan_info[num_chan_arr].max_eirp[chan_idx]);
			afc_add_regulatory_rule(reg_idx, REGLIB_MHZ_TO_KHZ(start_freq_khz),
									REGLIB_MHZ_TO_KHZ(end_freq_khz), bw_khz, max_eirp);
			reg_idx += 1;
		}
	}
}

static int afc_process_freq_and_chan_info(struct afc_spectrum_inquiry_resp *afc_response)
{
	uint8_t rule_exist;
	uint16_t next_num_rule;
	uint32_t eirp;
	uint32_t freq_iter, chan_iter, chan_idx;
	uint32_t start_freq_khz;
	uint32_t end_freq_khz;
	uint32_t bw;
	uint32_t chan_last_idx = 0;

	for (chan_idx = 0; chan_idx < afc_response->num_chan_info; chan_idx++)
		chan_last_idx += afc_response->chan_info[chan_idx].num_chan_cfi;

	next_num_rule = chan_last_idx;
	if (afc_response->num_chan_info)
		afc_process_chan_regrule_info(afc_response);

	for (freq_iter = 0; freq_iter <  afc_response->num_freq_info; freq_iter++) {
		rule_exist = 0;
		start_freq_khz = REGLIB_MHZ_TO_KHZ(afc_response->freq_info[freq_iter].freq_range.low_frequency);
		end_freq_khz = REGLIB_MHZ_TO_KHZ(afc_response->freq_info[freq_iter].freq_range.high_frequency);
		bw = afc_calculate_6ghz_bandwidth(afc_response->freq_info[freq_iter].freq_range.low_frequency,
				afc_response->freq_info[freq_iter].freq_range.high_frequency);
		if (!bw)
			continue;

		eirp = afc_calculate_psd_to_eirp(afc_response->freq_info[freq_iter].max_psd, bw);

		/* below check is to avoid the duplicate regulatory rule in the regd because
		there is a possibility to receive same data in channel and freq response */
		for (chan_iter = 0; chan_iter < chan_last_idx; chan_iter++) {
			if (eirp == (regd->reg_rules[chan_iter].power_rule.max_eirp/EIRP_UNIT_CONVERSION)) {
				if (start_freq_khz == regd->reg_rules[chan_iter].freq_range.start_freq_khz &&
							end_freq_khz == regd->reg_rules[chan_iter].freq_range.end_freq_khz) {
					rule_exist = 1;
					break;
				}
			}
		}

		if (!rule_exist) {
			afc_add_regulatory_rule(next_num_rule, start_freq_khz, end_freq_khz, bw, eirp);
			next_num_rule++;
		}
	}
	return (next_num_rule - 1);
}

static void afc_process_freq_regrule_info(struct afc_spectrum_inquiry_resp *afc_response)
{
	int freq_idx;
	uint32_t bw;
	uint32_t eirp;

	for (freq_idx = 0; freq_idx < afc_response->num_freq_info; freq_idx++) {
		bw = afc_calculate_6ghz_bandwidth(afc_response->freq_info[freq_idx].freq_range.low_frequency,
										  afc_response->freq_info[freq_idx].freq_range.high_frequency);
		if (!bw)
			continue;

		eirp = afc_calculate_psd_to_eirp(afc_response->freq_info[freq_idx].max_psd, bw);
		afc_add_regulatory_rule(freq_idx,
				REGLIB_MHZ_TO_KHZ(afc_response->freq_info[freq_idx].freq_range.low_frequency),
				REGLIB_MHZ_TO_KHZ(afc_response->freq_info[freq_idx].freq_range.high_frequency),
				bw, eirp);
	}
}

static size_t afc_reglib_array_len(size_t baselen, unsigned int elemcount, size_t elemlen)
{
	if (elemcount > (SIZE_MAX - baselen) / elemlen) {
		afc_printf(MSG_ERROR, "invalid database file, count too large!");
		return 0;
	}

	return baselen + elemcount * elemlen;
}

static void afc_print_reg_rule_data(struct mxl_ieee80211_regdomain *regd)
{
	int reg_rule_idx;

	afc_printf(MSG_INFO, "regd.n_reg_rules : %d", regd->n_reg_rules);
	afc_printf(MSG_INFO, "%-4s %-15s %-15s %-20s %-10s", "S.no", "start_freq_mhz", "end_freq_mhz",
			   "max_bandwidth_mhz", "max_eirp_dbm");

	for (reg_rule_idx = 0; reg_rule_idx < regd->n_reg_rules; reg_rule_idx++) {
		afc_printf(MSG_INFO, "%-4d %-15d %-15d %-20d %-10d", reg_rule_idx,
				   REGLIB_KHZ_TO_MHZ(regd->reg_rules[reg_rule_idx].freq_range.start_freq_khz),
				   REGLIB_KHZ_TO_MHZ(regd->reg_rules[reg_rule_idx].freq_range.end_freq_khz),
				   REGLIB_KHZ_TO_MHZ(regd->reg_rules[reg_rule_idx].freq_range.max_bandwidth_khz),
				   regd->reg_rules[reg_rule_idx].power_rule.max_eirp/EIRP_UNIT_CONVERSION);
	}
}

int afc_construct_regrule_from_afc_response(void *data)
{
	int chan_idx;
	int num_rules;
	int new_num_rules;
	size_t reg_size;
	struct afc_spectrum_inquiry_resp *afc_response = (struct afc_spectrum_inquiry_resp *)data;

	/* calculating the number of reg rules from the sum of freq and channel resp.
	the values in the channel cfi is for each global_op_class of 6GHz, so iterate
	through the loop of channel resp to find out the num of channel rule*/
	num_rules = afc_response->num_freq_info;
	for (chan_idx = 0; chan_idx < afc_response->num_chan_info; chan_idx++)
		num_rules += afc_response->chan_info[chan_idx].num_chan_cfi;

	reg_size = afc_reglib_array_len(sizeof(struct mxl_ieee80211_regdomain),
									num_rules , sizeof(struct ieee80211_reg_rule));
	afc_printf(MSG_INFO, "reg_size = %zu", reg_size);
	if (!reg_size)
		return AFC_STATUS_FAILURE;

	regd = (struct mxl_ieee80211_regdomain *)zalloc(reg_size);
	if (!regd)
		return AFC_STATUS_FAILURE;

	regd->n_reg_rules = num_rules;
	memcpy(regd->alpha2, afc_response->country, 2);

	afc_printf(MSG_INFO, "number of frequency information : %d", afc_response->num_freq_info);
	afc_printf(MSG_INFO, "number of channel information : %d", (num_rules - afc_response->num_freq_info));

	if (afc_response->num_freq_info && afc_response->num_chan_info) {
		afc_printf(MSG_INFO, "construct regulatory rule from frequency and channel based response");
		new_num_rules = afc_process_freq_and_chan_info(afc_response);
		/* update the num of reg rule after removing the duplicate data */
		regd->n_reg_rules = new_num_rules;
		reg_size = afc_reglib_array_len(sizeof(struct mxl_ieee80211_regdomain), new_num_rules,
										sizeof(struct ieee80211_reg_rule));
		if (!reg_size)
			goto fail;
	} else if (afc_response->num_freq_info) {
		afc_printf(MSG_INFO, "construct regulatory rule from frequency based response");
		afc_process_freq_regrule_info(afc_response);
	} else if (afc_response->num_chan_info) {
		afc_printf(MSG_INFO, "construct regulatory rule from channel based response");
		afc_process_chan_regrule_info(afc_response);
	} else {
		afc_printf(MSG_ERROR, "no valid data from AFC server,so "
				   "sending number of regulatory rules as zero");
		regd->n_reg_rules = 0;
	}

	if (regd->n_reg_rules)
		afc_print_reg_rule_data(regd);

	if (afc_nl80211_send_afc_info_to_drv(regd, reg_size))
		goto fail;

	free(regd);
	return AFC_STATUS_SUCCESS;

fail:
	free(regd);
	return AFC_STATUS_FAILURE;
}
