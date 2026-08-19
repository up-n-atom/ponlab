/*****************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#include <linux/types.h>
#include <linux/err.h>
#include <linux/mutex.h>
#include <linux/jiffies.h>
#include <linux/slab.h>

#include "pon/pon_mbox.h"
#include <pon/pon_ip_msg.h>
#include "pon/pon_mbox_counters.h"
#include "drv_pon_mbox_counters.h"
#include "drv_pon_mbox.h"

static inline void process_counter(__u64 *dst, __u64 *fw_response, __u64 *last,
				   __u64 max_bits)
{
	if (*last <= *fw_response)
		*dst += *fw_response - *last;
	else
		*dst += max_bits - *last + *fw_response;

	*last = *fw_response;
}

static inline void process_acc_counter(__u64 *dst, __u64 *fw_response,
				       __u64 *last, __u64 max_bits,
				       __u64 *acc)
{
	if (!dst || !fw_response || !last || !acc)
		return;

	if (*last <= *fw_response) {
		*dst += *fw_response - *last;
		*acc += *fw_response - *last;
	} else {
		*dst += max_bits - *last + *fw_response;
		*acc += max_bits - *last + *fw_response;
	}
	*last = *fw_response;
}

/**
 * @brief Macro used for processing overflow in GEM port counters table with
 *	  additional accumulating counters for all active GEMs
 *
 * @param counter counter name in destination table
 * @param max_bits bits for the counter in firmware message
 * @param table pointer to destination table (from storage)
 * @param last pointer to temporary table (last firmware reading)
 * @param fw_response pointer to current reading from the firmware (converter to
 *	  pon from ponfw)
 * @param acc_table pointer to destination table (from storage) for accumulated
 *	  GEM port counters
 */
#define PROCESS_ACC_COUNTER(counter, max_bits, table, last, fw_response,       \
			    acc_table)					       \
	process_acc_counter(&table->counter, &fw_response->counter,            \
			    &last->counter, ((__u64)1 << (max_bits)),	       \
			    &acc_table->counter)

/**
 * @brief Macro used for processing overflow in counter tables
 *
 * @param counter counter name in destination table
 * @param max_bits bits for the counter in firmware message
 * @param table pointer to destination table (from storage)
 * @param last pointer to temporary table (last firmware reading)
 * @param fw_response pointer to current reading from the firmware (converter to
 *	  pon from ponfw)
 */
#define PROCESS_COUNTER(counter, max_bits, table, last, fw_response)           \
	process_counter(&table->counter, &fw_response->counter,                \
			&last->counter, ((__u64)1 << (max_bits)))

/**
 * @brief Same as PROCESS_COUNTER, but for 64 bits wide firmware counter
 *
 * @param counter counter name in destination table
 * @param table pointer to destination table (from storage)
 * @param last pointer to temporary table (last firmware reading)
 * @param fw_response pointer to current reading from the firmware (converter to
 *	  pon from ponfw)
 */
#define PROCESS_COUNTER64(counter, table, last, fw_response)                   \
	process_counter(&table->counter, &fw_response->counter,                \
			&last->counter, 0)

static inline void process_counter_total(__u64 *dst, __u64 *fw_response,
					 __u64 *total, __u64 *last,
					 __u64 max_bits)
{
	__u64 tmp = *total;

	if (*last <= *fw_response)
		*total += *fw_response - *last;
	else
		*total += max_bits - *last + *fw_response;

	*dst += *total - tmp;
	*last = *fw_response;
}

/**
 * @brief Macro used for processing overflow in counter tables
 *
 * @param counter counter name in destination table
 * @param max_bits bits for the counter in firmware message
 * @param total pointer to total counter (from storage)
 * @param table pointer to destination table (from storage)
 * @param last pointer to temporary table (last firmware reading)
 * @param fw_response pointer to current reading from the firmware (converter
 *	  to pon from ponfw)
 */
#define PROCESS_COUNTER_T(counter, max_bits, total, table, last, \
			 fw_response)	\
	process_counter_total(&table->counter, &fw_response->counter,	\
			&total->counter, &last->counter,		\
			((__u64)1 << (max_bits)))

/**
 * @brief Same as PROCESS_COUNTER_TOTAL, but for 64 bits wide firmware counter
 *
 * @param counter counter name in destination table
 * @param total pointer to total counter (from storage)
 * @param table pointer to destination table (from storage)
 * @param last pointer to temporary table (last firmware reading)
 * @param fw_response pointer to current reading from the firmware (converter to
 *	  pon from ponfw)
 */
#define PROCESS_COUNTER64_T(counter, total, table, last, fw_response)	\
	process_counter_total(&table->counter, &fw_response->counter,	\
			&total->counter, &last->counter, 0)

static inline void process_acc_counter_t(__u64 *dst, __u64 *fw_response,
					 __u64 *total, __u64 *last,
					 __u64 max_bits, __u64 *acc)
{
	__u64 tmp = *total;

	if (*last <= *fw_response)
		*total += *fw_response - *last;
	else
		*total += max_bits - *last + *fw_response;

	*dst += *total - tmp;
	*acc += *total - tmp;
	*last = *fw_response;
}

/**
 * @brief Macro used for processing overflow in GEM port counters table with
 *	  additional accumulating counters for all active GEMs
 *
 * @param counter counter name in destination table
 * @param total pointer to total counter (per GEM port over all wavelengths)
 * @param max_bits bits for the counter in firmware message
 * @param table pointer to destination table (from storage)
 * @param last pointer to temporary table (last firmware reading)
 * @param fw_response pointer to current reading from the firmware (converter
 *	  to pon from ponfw)
 * @param acc_table pointer to destination table (from storage) for accumulated
 *	  GEM port counters (per wavelength)
 */
#define PROCESS_ACC_COUNTER_T(counter, max_bits, total, table, last,	\
			      fw_response, acc_table)			\
	process_acc_counter_t(&table->counter, &fw_response->counter,	\
			      &total->counter, &last->counter,		\
			      ((__u64)1 << (max_bits)), &acc_table->counter)

int pon_mbox_cnt_gem_port_add(u8 index, struct counters_state *state)
{
	if (index >= GEM_PORTS_MAX)
		return -EINVAL;

	mutex_lock(&state->lock);

	if (state->gem_ports_used[index]) {
		mutex_unlock(&state->lock);
		return -EEXIST;
	}

	state->gem_ports_used[index] = 1;

	mutex_unlock(&state->lock);

	return 0;
}

int pon_mbox_cnt_gem_port_del(u8 index, struct counters_state *state)
{
	if (index >= GEM_PORTS_MAX)
		return -EINVAL;

	mutex_lock(&state->lock);

	if (!state->gem_ports_used[index]) {
		mutex_unlock(&state->lock);
		return -ENOENT;
	}

	state->gem_ports_used[index] = 0;
	state->last_update.gem_port_counters[index] = 0;

	mutex_unlock(&state->lock);

	return 0;
}

int pon_mbox_cnt_alloc_id_add(u8 index, struct counters_state *state)
{
	if (index >= ALLOC_IDS_MAX)
		return -EINVAL;

	mutex_lock(&state->lock);

	if (state->alloc_id_used[index]) {
		mutex_unlock(&state->lock);
		return -EEXIST;
	}

	state->alloc_id_used[index] = 1;

	mutex_unlock(&state->lock);

	return 0;
}

int pon_mbox_cnt_alloc_id_del(u8 index, struct counters_state *state)
{
	if (index >= ALLOC_IDS_MAX)
		return -EINVAL;

	mutex_lock(&state->lock);

	if (!state->alloc_id_used[index]) {
		mutex_unlock(&state->lock);
		return -ENOENT;
	}

	state->alloc_id_used[index] = 0;
	state->last_update.alloc_counters[index] = 0;

	mutex_unlock(&state->lock);

	return 0;
}

struct pon_mbox_gtc_counters
*pon_mbox_cnt_gtc_table_get(u8 twdm_dswlch_id,
			    struct counters_state *state)
{
	struct pon_mbox_gtc_counters *ptr;

	mutex_lock(&state->lock);

	if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_ACC) {
		ptr = &state->storage.gpon_total.gtc_counters;
	} else {
		if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_CURR)
			twdm_dswlch_id = state->twdm_ds_idx;

		ptr = &state->storage.gpon.gtc_counters[twdm_dswlch_id];
	}

	mutex_unlock(&state->lock);

	return ptr;
}

int pon_mbox_cnt_gtc_table_add(struct pon_mbox_gtc_counters *cnt,
			       struct counters_state *state)
{
	struct pon_mbox_gtc_counters *table, *last, *total;

	if (!cnt)
		return -EINVAL;

	mutex_lock(&state->lock);
	table = &state->storage.gpon.gtc_counters[state->twdm_ds_idx];
	last = &state->last_counters.gpon.gtc;
	total = &state->storage.gpon_total.gtc_counters;

	PROCESS_COUNTER_T(bip_errors, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(disc_gem_frames, 40, total, table, last, cnt);
	PROCESS_COUNTER_T(gem_hec_errors_corr, 40, total, table, last, cnt);
	PROCESS_COUNTER_T(gem_hec_errors_uncorr, 40, total, table, last, cnt);
	PROCESS_COUNTER_T(bwmap_hec_errors_corr, 40, total, table, last, cnt);
	PROCESS_COUNTER_T(bytes_corr, 40, total, table, last, cnt);
	PROCESS_COUNTER_T(fec_codewords_corr, 40, total, table, last, cnt);
	PROCESS_COUNTER_T(fec_codewords_uncorr, 40, total, table, last, cnt);
	PROCESS_COUNTER_T(total_frames, 40, total, table, last, cnt);
	PROCESS_COUNTER_T(fec_sec, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(gem_idle, 40, total, table, last, cnt);
	PROCESS_COUNTER_T(lods_events, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(dg_time, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(ploam_crc_errors, 32, total, table, last, cnt);

	state->last_update.gtc_counters = jiffies;
	mutex_unlock(&state->lock);

	return 0;
}

struct pon_mbox_xgtc_counters
*pon_mbox_cnt_xgtc_table_get(u8 twdm_dswlch_id,
			     struct counters_state *state)
{
	struct pon_mbox_xgtc_counters *ptr;

	mutex_lock(&state->lock);

	if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_ACC) {
		ptr = &state->storage.gpon_total.xgtc_counters;
	} else {
		if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_CURR)
			twdm_dswlch_id = state->twdm_ds_idx;

		ptr = &state->storage.gpon.xgtc_counters[twdm_dswlch_id];
	}

	mutex_unlock(&state->lock);

	return ptr;
}

int pon_mbox_cnt_xgtc_table_add(struct pon_mbox_xgtc_counters *cnt,
				struct counters_state *state)
{
	struct pon_mbox_xgtc_counters *table, *last, *total;

	if (!cnt)
		return -EINVAL;

	mutex_lock(&state->lock);
	table = &state->storage.gpon.xgtc_counters[state->twdm_ds_idx];
	last = &state->last_counters.gpon.xgtc;
	total = &state->storage.gpon_total.xgtc_counters;

	PROCESS_COUNTER_T(psbd_hec_err_uncorr, 40, total, table, last, cnt);
	PROCESS_COUNTER_T(psbd_hec_err_corr, 40, total, table, last, cnt);
	PROCESS_COUNTER_T(fs_hec_err_uncorr, 40, total, table, last, cnt);
	PROCESS_COUNTER_T(fs_hec_err_corr, 40, total, table, last, cnt);
	PROCESS_COUNTER_T(lost_words, 40, total, table, last, cnt);
	PROCESS_COUNTER_T(ploam_mic_err, 32, total, table, last, cnt);

	state->last_update.xgtc_counters = jiffies;
	mutex_unlock(&state->lock);

	return 0;
}

struct pon_mbox_gem_port_counters
*pon_mbox_cnt_gem_all_table_get(u8 twdm_dswlch_id,
				struct counters_state *state)
{
	u8 dswlch_id, uswlch_id;
	struct pon_mbox_gem_port_counters *ptr;

	mutex_lock(&state->lock);

	if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_CURR ||
	    twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_ACC) {
		dswlch_id = state->twdm_ds_idx;
		uswlch_id = state->twdm_us_idx;
	} else {
		dswlch_id = twdm_dswlch_id;
		uswlch_id = twdm_dswlch_id;
	}

	ptr = &state->storage.gpon.gem_all_counters[dswlch_id][uswlch_id];

	mutex_unlock(&state->lock);

	return ptr;
}

struct pon_mbox_gem_port_counters
*pon_mbox_cnt_gem_port_table_get(u8 twdm_dswlch_id, u8 index,
				 struct counters_state *state)
{
	u8 dswlch_id, uswlch_id;
	struct pon_mbox_gem_port_counters *ptr;

	if (index >= GEM_PORTS_MAX)
		return NULL;

	mutex_lock(&state->lock);

	if (!state->gem_ports_used[index]) {
		mutex_unlock(&state->lock);
		return NULL;
	}

	if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_ACC) {
		ptr = &state->storage.gpon_total.gem_port_counters[index];
	} else {
		if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_CURR) {
			dswlch_id = state->twdm_ds_idx;
			uswlch_id = state->twdm_us_idx;
		} else {
			dswlch_id = twdm_dswlch_id;
			uswlch_id = twdm_dswlch_id;
		}
		ptr = &state->storage.gpon.gem_port_counters
			[dswlch_id][uswlch_id][index];
	}

	mutex_unlock(&state->lock);

	return ptr;
}

int pon_mbox_cnt_gem_port_table_add(u8 index,
				    struct pon_mbox_gem_port_counters *cnt,
				    struct counters_state *state)
{
	struct pon_mbox_gem_port_counters *table, *last, *acc_table, *total;

	if (!cnt)
		return -EINVAL;

	if (index >= GEM_PORTS_MAX)
		return -EINVAL;

	mutex_lock(&state->lock);

	if (!state->gem_ports_used[index]) {
		mutex_unlock(&state->lock);
		return -ENOENT;
	}

	acc_table = &state->storage.gpon.gem_all_counters
				  [state->twdm_ds_idx]
				  [state->twdm_us_idx];
	table = &state->storage.gpon.gem_port_counters[state->twdm_ds_idx]
				  [state->twdm_us_idx]
				  [index];
	last = &state->last_counters.gpon.gem_port[index];
	total = &state->storage.gpon_total.gem_port_counters[index];

	PROCESS_ACC_COUNTER_T(tx_frames, 40,
			      total, table, last, cnt, acc_table);
	PROCESS_ACC_COUNTER_T(tx_fragments, 40,
			      total, table, last, cnt, acc_table);
	PROCESS_ACC_COUNTER_T(tx_bytes, 40,
			      total, table, last, cnt, acc_table);
	PROCESS_ACC_COUNTER_T(rx_frames, 40,
			      total, table, last, cnt, acc_table);
	PROCESS_ACC_COUNTER_T(rx_fragments,
			      40, total, table, last, cnt, acc_table);
	PROCESS_ACC_COUNTER_T(rx_bytes,
			      40, total, table, last, cnt, acc_table);
	PROCESS_ACC_COUNTER_T(key_errors, 40,
			      total, table, last, cnt, acc_table);

	state->last_update.gem_port_counters[index] = jiffies;
	mutex_unlock(&state->lock);

	return 0;
}

struct pon_alloc_counters
*pon_mbox_cnt_alloc_id_table_get(u8 index,
				 struct counters_state *state)
{
	struct pon_alloc_counters *ptr;

	if (index >= ALLOC_IDS_MAX)
		return NULL;

	mutex_lock(&state->lock);

	if (!state->alloc_id_used[index]) {
		mutex_unlock(&state->lock);
		return NULL;
	}

	ptr = &state->storage.gpon.alloc_counters[state->twdm_us_idx][index];

	mutex_unlock(&state->lock);

	return ptr;
}

int pon_mbox_cnt_alloc_id_table_add(u8 index,
				    struct pon_alloc_counters *cnt,
				    struct counters_state *state)
{
	struct pon_alloc_counters *table, *last;

	if (index >= ALLOC_IDS_MAX)
		return -EINVAL;

	mutex_lock(&state->lock);

	if (!state->alloc_id_used[index]) {
		mutex_unlock(&state->lock);
		return -EINVAL;
	}

	table = &state->storage.gpon.alloc_counters[state->twdm_us_idx][index];
	last = &state->last_counters.gpon.alloc[index];

	PROCESS_COUNTER(allocations, 40, table, last, cnt);
	PROCESS_COUNTER(idle, 40, table, last, cnt);

	/* us_bw is a snapshot value representing the current upstream
	 * bandwidth, not a counter that accumulates over time, so we assign it
	 * directly instead of processing it for overflow or accumulation.
	 */
	table->us_bw = cnt->us_bw;

	state->last_update.alloc_counters[index] = jiffies;
	mutex_unlock(&state->lock);

	return 0;
}

struct pon_alloc_discard_counters
*pon_mbox_cnt_alloc_lost_table_get(struct counters_state *state)
{
	struct pon_alloc_discard_counters *ptr;

	mutex_lock(&state->lock);

	ptr = &state->storage.gpon.alloc_discard_counters[state->twdm_us_idx];

	mutex_unlock(&state->lock);

	return ptr;
}

int pon_mbox_cnt_alloc_lost_table_add(struct pon_alloc_discard_counters *cnt,
				      struct counters_state *state)
{
	struct pon_alloc_discard_counters *table, *last;

	if (!cnt)
		return -EINVAL;

	mutex_lock(&state->lock);
	table = &state->storage.gpon.alloc_discard_counters[state->twdm_us_idx];
	last = &state->last_counters.gpon.alloc_discard;

	PROCESS_COUNTER(disc[0], 40, table, last, cnt);
	PROCESS_COUNTER(disc[1], 40, table, last, cnt);
	PROCESS_COUNTER(disc[2], 40, table, last, cnt);
	PROCESS_COUNTER(disc[3], 40, table, last, cnt);
	PROCESS_COUNTER(disc[4], 40, table, last, cnt);
	PROCESS_COUNTER(disc[5], 40, table, last, cnt);
	PROCESS_COUNTER(disc[6], 40, table, last, cnt);
	PROCESS_COUNTER(disc[7], 40, table, last, cnt);

	PROCESS_COUNTER(rule[2], 32, table, last, cnt);
	PROCESS_COUNTER(rule[4], 32, table, last, cnt);
	PROCESS_COUNTER(rule[6], 32, table, last, cnt);
	PROCESS_COUNTER(rule[7], 32, table, last, cnt);
	PROCESS_COUNTER(rule[8], 32, table, last, cnt);
	PROCESS_COUNTER(rule[9], 32, table, last, cnt);
	PROCESS_COUNTER(rule[10], 32, table, last, cnt);
	PROCESS_COUNTER(rule[11], 32, table, last, cnt);
	PROCESS_COUNTER(rule[12], 32, table, last, cnt);
	PROCESS_COUNTER(rule[13], 32, table, last, cnt);
	PROCESS_COUNTER(rule[14], 32, table, last, cnt);
	PROCESS_COUNTER(rule[15], 32, table, last, cnt);
	PROCESS_COUNTER(rule[16], 32, table, last, cnt);

	state->last_update.alloc_discard_counters = jiffies;
	mutex_unlock(&state->lock);

	return 0;
}

struct pon_eth_counters
*pon_mbox_cnt_rx_eth_table_get(u8 index,
			       struct counters_state *state)
{
	struct pon_eth_counters *ptr;

	if (index >= GEM_PORTS_MAX)
		return NULL;

	mutex_lock(&state->lock);

	if (!state->gem_ports_used[index]) {
		mutex_unlock(&state->lock);
		return NULL;
	}

	ptr = &state->storage.gpon.eth_rx_counters[state->twdm_ds_idx][index];

	mutex_unlock(&state->lock);

	return ptr;
}

int pon_mbox_cnt_rx_eth_table_add(u8 index,
				  struct pon_eth_counters *cnt,
				  struct counters_state *state)
{
	struct pon_eth_counters *table, *last;

	if (!cnt)
		return -EINVAL;

	if (index >= GEM_PORTS_MAX)
		return -EINVAL;

	mutex_lock(&state->lock);

	if (!state->gem_ports_used[index]) {
		mutex_unlock(&state->lock);
		return -ENOENT;
	}

	table = &state->storage.gpon.eth_rx_counters[state->twdm_ds_idx][index];
	last = &state->last_counters.gpon.rx_eth[index];

	PROCESS_COUNTER(bytes, 40, table, last, cnt);
	PROCESS_COUNTER(frames_lt_64, 40, table, last, cnt);
	PROCESS_COUNTER(frames_64, 40, table, last, cnt);
	PROCESS_COUNTER(frames_65_127, 40, table, last, cnt);
	PROCESS_COUNTER(frames_128_255, 40, table, last, cnt);
	PROCESS_COUNTER(frames_256_511, 40, table, last, cnt);
	PROCESS_COUNTER(frames_512_1023, 40, table, last, cnt);
	PROCESS_COUNTER(frames_1024_1518, 40, table, last, cnt);
	PROCESS_COUNTER(frames_gt_1518, 40, table, last, cnt);
	PROCESS_COUNTER(frames_fcs_err, 40, table, last, cnt);
	PROCESS_COUNTER(bytes_fcs_err, 40, table, last, cnt);
	PROCESS_COUNTER(frames_too_long, 40, table, last, cnt);

	state->last_update.eth_rx_counters[index] = jiffies;
	mutex_unlock(&state->lock);

	return 0;
}

struct pon_eth_counters
*pon_mbox_cnt_tx_eth_table_get(u8 index,
			       struct counters_state *state)
{
	struct pon_eth_counters *ptr;

	if (index >= GEM_PORTS_MAX)
		return NULL;

	mutex_lock(&state->lock);

	if (!state->gem_ports_used[index]) {
		mutex_unlock(&state->lock);
		return NULL;
	}

	ptr = &state->storage.gpon.eth_tx_counters[state->twdm_us_idx][index];

	mutex_unlock(&state->lock);

	return ptr;
}

int pon_mbox_cnt_tx_eth_table_add(u8 index,
				  struct pon_eth_counters *cnt,
				  struct counters_state *state)
{
	struct pon_eth_counters *table, *last;

	if (!cnt)
		return -EINVAL;

	if (index >= GEM_PORTS_MAX)
		return -EINVAL;

	mutex_lock(&state->lock);

	if (!state->gem_ports_used[index]) {
		mutex_unlock(&state->lock);
		return -ENOENT;
	}

	table = &state->storage.gpon.eth_tx_counters[state->twdm_ds_idx][index];
	last = &state->last_counters.gpon.tx_eth[index];

	PROCESS_COUNTER(bytes, 40, table, last, cnt);
	PROCESS_COUNTER(frames_lt_64, 40, table, last, cnt);
	PROCESS_COUNTER(frames_64, 40, table, last, cnt);
	PROCESS_COUNTER(frames_65_127, 40, table, last, cnt);
	PROCESS_COUNTER(frames_128_255, 40, table, last, cnt);
	PROCESS_COUNTER(frames_256_511, 40, table, last, cnt);
	PROCESS_COUNTER(frames_512_1023, 40, table, last, cnt);
	PROCESS_COUNTER(frames_1024_1518, 40, table, last, cnt);
	PROCESS_COUNTER(frames_gt_1518, 40, table, last, cnt);
	PROCESS_COUNTER(frames_fcs_err, 40, table, last, cnt);
	PROCESS_COUNTER(bytes_fcs_err, 40, table, last, cnt);
	PROCESS_COUNTER(frames_too_long, 40, table, last, cnt);

	state->last_update.eth_tx_counters[index] = jiffies;
	mutex_unlock(&state->lock);

	return 0;
}

struct pon_mbox_twdm_lods_counters
*pon_mbox_cnt_twdm_lods_table_get(u8 twdm_dswlch_id,
				  struct counters_state *state)
{
	struct pon_mbox_twdm_lods_counters *ptr;

	mutex_lock(&state->lock);

	if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_ACC) {
		ptr = &state->storage.gpon_total.twdm_lods_counter;
	} else {
		if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_CURR)
			twdm_dswlch_id = state->twdm_ds_idx;

		ptr = &state->storage.gpon.twdm_lods[twdm_dswlch_id];
	}

	mutex_unlock(&state->lock);

	return ptr;
}

int pon_mbox_cnt_twdm_lods_table_add(struct pon_mbox_twdm_lods_counters *cnt,
				     struct counters_state *state)
{
	struct pon_mbox_twdm_lods_counters *table, *last, *total;

	if (!cnt)
		return -EINVAL;

	mutex_lock(&state->lock);
	table = &state->storage.gpon.twdm_lods[state->twdm_ds_idx];
	last = &state->last_counters.gpon.twdm_lods;
	total = &state->storage.gpon_total.twdm_lods_counter;

	PROCESS_COUNTER_T(events_all, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(react_oper, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(react_prot, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(react_disc, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(rest_oper, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(rest_prot, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(rest_disc, 32, total, table, last, cnt);

	state->last_update.twdm_lods_counters = jiffies;
	mutex_unlock(&state->lock);

	return 0;
}

struct pon_mbox_twdm_optic_pl_counters
*pon_mbox_cnt_twdm_optic_pl_table_get(u8 twdm_dswlch_id,
				      struct counters_state *state)
{
	struct pon_mbox_twdm_optic_pl_counters *ptr;

	mutex_lock(&state->lock);

	if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_ACC) {
		ptr = &state->storage.gpon_total.twdm_optic_pl_counter;
	} else {
		if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_CURR)
			twdm_dswlch_id = state->twdm_ds_idx;

		ptr = &state->storage.gpon.twdm_optic_pl[twdm_dswlch_id];
	}

	mutex_unlock(&state->lock);

	return ptr;
}

int pon_mbox_cnt_twdm_optic_pl_table_add(
				struct pon_mbox_twdm_optic_pl_counters *cnt,
				struct counters_state *state)
{
	struct pon_mbox_twdm_optic_pl_counters *table, *last, *total;

	if (!cnt)
		return -EINVAL;

	mutex_lock(&state->lock);
	table = &state->storage.gpon.twdm_optic_pl[state->twdm_ds_idx];
	last = &state->last_counters.gpon.twdm_optic_pl;
	total = &state->storage.gpon_total.twdm_optic_pl_counter;

	PROCESS_COUNTER_T(rejected, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(incomplete, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(complete, 32, total, table, last, cnt);

	state->last_update.twdm_optic_pl_counters = jiffies;
	mutex_unlock(&state->lock);

	return 0;
}

struct pon_mbox_twdm_tc_counters
*pon_mbox_cnt_twdm_tc_table_get(u8 twdm_dswlch_id,
				struct counters_state *state)
{
	struct pon_mbox_twdm_tc_counters *ptr;

	mutex_lock(&state->lock);

	if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_ACC) {
		ptr = &state->storage.gpon_total.twdm_tc_counter;
	} else {
		if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_CURR)
			twdm_dswlch_id = state->twdm_ds_idx;

		ptr = &state->storage.gpon.twdm_tc[twdm_dswlch_id];
	}

	mutex_unlock(&state->lock);

	return ptr;
}

int pon_mbox_cnt_twdm_tc_table_add(struct pon_mbox_twdm_tc_counters *cnt,
				   struct counters_state *state)
{
	struct pon_mbox_twdm_tc_counters *table, *last, *total;
	unsigned int i;

	if (!cnt)
		return -EINVAL;

	mutex_lock(&state->lock);
	table = &state->storage.gpon.twdm_tc[state->twdm_ds_idx];
	last = &state->last_counters.gpon.twdm_tc;
	total = &state->storage.gpon_total.twdm_tc_counter;

	for (i = 0; i < ARRAY_SIZE(table->tc_); i++)
		PROCESS_COUNTER_T(tc_[i], 32, total, table, last, cnt);

	state->last_update.twdm_tc_counters = jiffies;
	mutex_unlock(&state->lock);

	return 0;
}

struct pon_mbox_xgtc_ploam_ds_counters
*pon_mbox_cnt_xgtc_ploam_ds_table_get(u8 twdm_dswlch_id,
				      struct counters_state *state)
{
	struct pon_mbox_xgtc_ploam_ds_counters *ptr;

	mutex_lock(&state->lock);

	if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_ACC) {
		ptr = &state->storage.gpon_total.xgtc_ploam_ds_counter;
	} else {
		if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_CURR)
			twdm_dswlch_id = state->twdm_ds_idx;

		ptr = &state->storage.gpon.xgtc_ploam_ds[twdm_dswlch_id];
	}

	mutex_unlock(&state->lock);

	return ptr;
}

int pon_mbox_cnt_xgtc_ploam_ds_table_add(
				struct pon_mbox_xgtc_ploam_ds_counters *cnt,
				struct counters_state *state)
{
	struct pon_mbox_xgtc_ploam_ds_counters *table, *last, *total;

	if (!cnt)
		return -EINVAL;

	mutex_lock(&state->lock);
	table = &state->storage.gpon.xgtc_ploam_ds[state->twdm_ds_idx];
	last = &state->last_counters.gpon.xgtc_ploam_ds;
	total = &state->storage.gpon_total.xgtc_ploam_ds_counter;

	PROCESS_COUNTER_T(bst_profile, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(ass_onu, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(rng_time, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(deact_onu, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(dis_ser, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(req_reg, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(ass_alloc, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(key_ctrl, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(slp_allow, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(calib_req, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(adj_tx_wl, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(tune_ctrl, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(sys_profile, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(ch_profile, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(prot_control, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(chg_pw_lvl, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(pw_cons, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(rate_ctrl, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(reboot_onu, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(unknown, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(adj_tx_wl_fail, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(tune_req, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(tune_compl, 32, total, table, last, cnt);

	state->last_update.xgtc_ploam_ds_counters = jiffies;
	mutex_unlock(&state->lock);

	return 0;
}

struct pon_mbox_gtc_ploam_ds_counters
*pon_mbox_cnt_gtc_ploam_ds_table_get(struct counters_state *state)
{
	struct pon_mbox_gtc_ploam_ds_counters *ptr;

	mutex_lock(&state->lock);

	/* no GTC counters with TWDM, channel is always 0 */
	ptr = &state->storage.gpon.gtc_ploam_ds[0];

	mutex_unlock(&state->lock);

	return ptr;
}

int pon_mbox_cnt_gtc_ploam_ds_table_add(
				struct pon_mbox_gtc_ploam_ds_counters *cnt,
				struct counters_state *state)
{
	struct pon_mbox_gtc_ploam_ds_counters *table, *last, *total;

	if (!cnt)
		return -EINVAL;

	mutex_lock(&state->lock);
	/* no GTC counters with TWDM, channel is always 0 */
	table = &state->storage.gpon.gtc_ploam_ds[0];
	last = &state->last_counters.gpon.gtc_ploam_ds;
	total = &state->storage.gpon_total.gtc_ploam_ds_counter;

	PROCESS_COUNTER_T(us_ovh, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(ass_onu, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(rng_time, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(deact_onu, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(dis_ser, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(enc_gem_pid, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(req_pw, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(ass_alloc, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(no_msg, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(popup, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(req_key, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(cfg_port, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(phy_ee, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(chg_pl, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(pst, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(ber, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(key_swtime, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(ext_burst, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(pon_id, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(swift_popup, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(rng_adj, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(slp_allow, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(unknown, 32, total, table, last, cnt);

	state->last_update.gtc_ploam_ds_counters = jiffies;
	mutex_unlock(&state->lock);

	return 0;
}

struct pon_mbox_xgtc_ploam_us_counters
*pon_mbox_cnt_xgtc_ploam_us_table_get(u8 twdm_dswlch_id,
				      struct counters_state *state)
{
	struct pon_mbox_xgtc_ploam_us_counters *ptr;

	mutex_lock(&state->lock);

	if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_ACC) {
		ptr = &state->storage.gpon_total.xgtc_ploam_us_counter;
	} else {
		if (twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_CURR)
			twdm_dswlch_id = state->twdm_ds_idx;

		ptr = &state->storage.gpon.xgtc_ploam_us[twdm_dswlch_id];
	}

	mutex_unlock(&state->lock);

	return ptr;
}

int pon_mbox_cnt_xgtc_ploam_us_table_add(
				struct pon_mbox_xgtc_ploam_us_counters *cnt,
				struct counters_state *state)
{
	struct pon_mbox_xgtc_ploam_us_counters *table, *last, *total;

	if (!cnt)
		return -EINVAL;

	mutex_lock(&state->lock);
	table = &state->storage.gpon.xgtc_ploam_us[state->twdm_ds_idx];
	last = &state->last_counters.gpon.xgtc_ploam_us;
	total = &state->storage.gpon_total.xgtc_ploam_us_counter;

	PROCESS_COUNTER_T(ser_onu, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(reg, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(key_rep, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(ack, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(slp_req, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(tun_res, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(pw_cons, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(rate_resp, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(cpl_err, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(tun_res_an, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(tun_res_crb, 32, total, table, last, cnt);

	state->last_update.xgtc_ploam_us_counters = jiffies;
	mutex_unlock(&state->lock);

	return 0;
}

struct pon_mbox_gtc_ploam_us_counters *pon_mbox_cnt_gtc_ploam_us_table_get
				(struct counters_state *state)
{
	struct pon_mbox_gtc_ploam_us_counters *ptr;

	mutex_lock(&state->lock);

	/* no GTC counters with TWDM, channel is always 0 */
	ptr = &state->storage.gpon.gtc_ploam_us[0];

	mutex_unlock(&state->lock);

	return ptr;
}

int pon_mbox_cnt_gtc_ploam_us_table_add(
				struct pon_mbox_gtc_ploam_us_counters *cnt,
				struct counters_state *state)
{
	struct pon_mbox_gtc_ploam_us_counters *table, *last, *total;

	if (!cnt)
		return -EINVAL;

	mutex_lock(&state->lock);
	/* no GTC counters with TWDM, channel is always 0 */
	table = &state->storage.gpon.gtc_ploam_us[0];
	last = &state->last_counters.gpon.gtc_ploam_us;
	total = &state->storage.gpon_total.gtc_ploam_us_counter;

	PROCESS_COUNTER_T(ser_onu, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(password, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(dyg_gasp, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(no_msg, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(enc_key, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(phy_ee, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(pst_msg, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(rem_err, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(ack, 32, total, table, last, cnt);
	PROCESS_COUNTER_T(slp_req, 32, total, table, last, cnt);

	state->last_update.gtc_ploam_us_counters = jiffies;
	mutex_unlock(&state->lock);

	return 0;
}

struct counters_state *pon_mbox_cnt_state_init(void)
{
	struct counters_state *state = kzalloc(sizeof(*state), GFP_KERNEL);

	if (!state)
		return NULL;

	mutex_init(&state->lock);
	mutex_init(&state->storage.tables_lock);

	return state;
}

void pon_mbox_cnt_state_release(struct counters_state *cnt)
{
	mutex_destroy(&cnt->lock);
	mutex_destroy(&cnt->storage.tables_lock);
	kfree(cnt);
}

/* we do not need this function now, but we can keep using it, if we would need
 * to change static tables to dynamically allocated
 */
void pon_mbox_cnt_release(void *counter)
{
	(void)(counter);
}

void pon_mbox_cnt_reset(struct counters_state *state)
{
	mutex_lock(&state->storage.tables_lock);
	memset(&state->storage.gpon, 0, sizeof(state->storage.gpon));
	mutex_unlock(&state->storage.tables_lock);

	mutex_lock(&state->lock);
	state->twdm_us_idx = 0;
	state->twdm_ds_idx = 0;
	memset(&state->last_counters, 0, sizeof(state->last_counters));
	memset(state->alloc_id_used, 0, sizeof(state->alloc_id_used));
	memset(state->gem_ports_used, 0, sizeof(state->gem_ports_used));
	memset(&state->last_update, 0, sizeof(state->last_update));
	mutex_unlock(&state->lock);
}

int pon_mbox_activate_counter_set(u8 us_index,
				  u8 ds_index,
				  struct counters_state *state)
{
	if (us_index >= TWDM_US_MAX || ds_index >= TWDM_DS_MAX)
		return -EINVAL;

	mutex_lock(&state->lock);

	state->twdm_us_idx = us_index;
	state->twdm_ds_idx = ds_index;

	memset(&state->last_counters, 0, sizeof(state->last_counters));
	memset(&state->last_update, 0, sizeof(state->last_update));

	mutex_unlock(&state->lock);

	return 0;
}

int pon_mbox_used_gem_ports_get(u8 *gem_port_idx,
				unsigned int size,
				struct counters_state *state)
{
	unsigned int j = 0, i;

	mutex_lock(&state->lock);

	for (i = 0; i < GEM_PORTS_MAX && j < size; ++i) {
		if (state->gem_ports_used[i]) {
			gem_port_idx[j] = i;
			j++;
		}
	}

	mutex_unlock(&state->lock);

	return j;
}

int pon_mbox_used_alloc_ids_get(u8 *alloc_id_used,
				unsigned int size,
				struct counters_state *state)
{
	unsigned int j = 0, i;

	mutex_lock(&state->lock);

	for (i = 0; i < ALLOC_IDS_MAX && j < size; ++i) {
		if (state->alloc_id_used[i]) {
			alloc_id_used[j] = i;
			j++;
		}
	}

	mutex_unlock(&state->lock);

	return j;
}

void pon_mbox_last_update_get(struct counters_last_update *last_update,
			      struct counters_state *state)
{
	mutex_lock(&state->lock);
	*last_update = state->last_update;
	mutex_unlock(&state->lock);
}

#define MIN_TIME(a, b) (time_before((a), (b)) ? (a) : (b))

/**
 * Returns oldest update time of GPON related counters.
 * We need this to know how soon should we schedule the next counters update
 */
unsigned long pon_mbox_cnt_oldest_update_time_get(struct counters_state *state,
						  u64 mask)
{
	unsigned int i;
	unsigned long min = jiffies;

	mutex_lock(&state->lock);

	if (mask & BIT(ALLOC_LOST_CNT))
		min = MIN_TIME(min, state->last_update.alloc_discard_counters);

	if (mask & BIT(GTC_CNT))
		min = MIN_TIME(min, state->last_update.gtc_counters);

	if (mask & BIT(XGTC_CNT))
		min = MIN_TIME(min, state->last_update.xgtc_counters);

	if (mask & BIT(GEM_PORT_CNT)) {
		for (i = 0; i < GEM_PORTS_MAX; ++i) {
			if (!state->gem_ports_used[i])
				continue;

			min = MIN_TIME(min,
				       state->last_update.gem_port_counters[i]);
		}
	}

	if (mask & BIT(ALLOC_CNT)) {
		for (i = 0; i < ALLOC_IDS_MAX; ++i) {
			if (!state->alloc_id_used[i])
				continue;

			min = MIN_TIME(min,
				       state->last_update.alloc_counters[i]);
		}
	}

	mutex_unlock(&state->lock);

	return min;
}

void pon_mbox_cnt_lock(struct counters_state *state)
{
	mutex_lock(&state->storage.tables_lock);
}

void pon_mbox_cnt_unlock(struct counters_state *state)
{
	mutex_unlock(&state->storage.tables_lock);
}
