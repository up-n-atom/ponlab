/******************************************************************************
 * Copyright (c) 2023 - 2025 MaxLinear, Inc.
 * Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 ******************************************************************************/

#include <gtest/gtest.h>
#include <cstdint>

#include "mocks.hpp"
#include <linux/types.h>
#include "drv_pon_mbox_counters.c"
#include "drv_pon_mbox_counters_conv.c"

TEST(Counters, pon_mbox_cnt_locks)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_lock(stat);
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls, 1);
	ASSERT_EQ(mocks::state.mutex_unlock.ret_calls, 0);
	pon_mbox_cnt_unlock(stat);
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls, 1);
	ASSERT_EQ(mocks::state.mutex_unlock.ret_calls, 1);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, alloc_ids)
{
	mocks::reset();
	struct counters_state *statx = pon_mbox_cnt_state_init();
	ASSERT_EQ(0, pon_mbox_cnt_alloc_id_add(0, statx));
	ASSERT_EQ(-ENOENT, pon_mbox_cnt_alloc_id_del(63, statx));
	ASSERT_EQ(0, pon_mbox_cnt_alloc_id_add(63, statx));
	ASSERT_EQ(-EINVAL, pon_mbox_cnt_alloc_id_add(ALLOC_IDS_MAX, statx));
	ASSERT_EQ(-EEXIST, pon_mbox_cnt_alloc_id_add(0, statx));
	ASSERT_EQ(0, pon_mbox_cnt_alloc_id_del(0, statx));
	ASSERT_EQ(-ENOENT, pon_mbox_cnt_alloc_id_del(0, statx));
	ASSERT_EQ(0, pon_mbox_cnt_alloc_id_del(63, statx));
	ASSERT_EQ(-ENOENT, pon_mbox_cnt_alloc_id_del(63, statx));

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(statx);
}

TEST(Counters, gem_port)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	ASSERT_EQ(0, pon_mbox_cnt_gem_port_add(0, stat));
	ASSERT_EQ(-EEXIST, pon_mbox_cnt_gem_port_add(0, stat));
	ASSERT_EQ(0, pon_mbox_cnt_gem_port_del(0, stat));
	ASSERT_EQ(-ENOENT, pon_mbox_cnt_gem_port_del(0, stat));

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, gem_port_get)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);
	pon_mbox_cnt_gem_port_del(0, stat);
	pon_mbox_cnt_gem_port_del(1, stat);

	struct pon_mbox_gem_port_counters *cnt =
		pon_mbox_cnt_gem_port_table_get(
				PON_MBOX_D_DSWLCH_ID_CURR, 0, stat);

	ASSERT_EQ(cnt, nullptr);

	pon_mbox_cnt_gem_port_add(0, stat);
	cnt = pon_mbox_cnt_gem_port_table_get(
				PON_MBOX_D_DSWLCH_ID_CURR, 0, stat);

	ASSERT_NE(cnt, nullptr);
	cnt->tx_frames = 10;

	cnt = pon_mbox_cnt_gem_port_table_get(
				PON_MBOX_D_DSWLCH_ID_CURR, 1, stat);
	ASSERT_EQ(cnt, nullptr);
	pon_mbox_cnt_gem_port_add(1, stat);
	cnt = pon_mbox_cnt_gem_port_table_get(
				PON_MBOX_D_DSWLCH_ID_CURR, 1, stat);

	ASSERT_NE(cnt, nullptr);
	cnt->tx_frames = 20;

	cnt = pon_mbox_cnt_gem_port_table_get(
				PON_MBOX_D_DSWLCH_ID_CURR, 0, stat);
	ASSERT_EQ(cnt->tx_frames, (__u64) 10);

	cnt = pon_mbox_cnt_gem_port_table_get(
				PON_MBOX_D_DSWLCH_ID_CURR, 1, stat);
	ASSERT_EQ(cnt->tx_frames, (__u64) 20);
	ASSERT_EQ(0, pon_mbox_cnt_gem_port_del(0, stat));
	ASSERT_EQ(0, pon_mbox_cnt_gem_port_del(1, stat));
	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, static_tables)
{
	struct counters_state *stat = pon_mbox_cnt_state_init();
	ASSERT_NE(pon_mbox_cnt_gtc_table_get(PON_MBOX_D_DSWLCH_ID_CURR,
					     stat), nullptr);
	ASSERT_NE(pon_mbox_cnt_xgtc_table_get(PON_MBOX_D_DSWLCH_ID_CURR,
					      stat), nullptr);
	ASSERT_NE(pon_mbox_cnt_alloc_lost_table_get(stat), nullptr);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, alloc_id_table_get)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	pon_mbox_cnt_lock(stat);
	pon_mbox_cnt_alloc_id_del(0, stat);
	pon_mbox_cnt_alloc_id_del(1, stat);

	struct pon_alloc_counters *cnt = pon_mbox_cnt_alloc_id_table_get(0, stat);

	ASSERT_EQ(cnt, nullptr);

	pon_mbox_cnt_alloc_id_add(0, stat);
	cnt = pon_mbox_cnt_alloc_id_table_get(0, stat);

	ASSERT_NE(cnt, nullptr);
	cnt->allocations = 10;

	cnt = pon_mbox_cnt_alloc_id_table_get(1, stat);
	ASSERT_EQ(cnt, nullptr);
	pon_mbox_cnt_alloc_id_add(1, stat);
	cnt = pon_mbox_cnt_alloc_id_table_get(1, stat);

	ASSERT_NE(cnt, nullptr);
	cnt->allocations = 20;

	cnt = pon_mbox_cnt_alloc_id_table_get(0, stat);
	ASSERT_EQ(cnt->allocations, (__u64) 10);

	cnt = pon_mbox_cnt_alloc_id_table_get(1, stat);
	ASSERT_EQ(cnt->allocations, (__u64) 20);
	ASSERT_EQ(0, pon_mbox_cnt_alloc_id_del(0, stat));
	ASSERT_EQ(0, pon_mbox_cnt_alloc_id_del(1, stat));
	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, eth_tx_table_get)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);
	pon_mbox_cnt_gem_port_del(0, stat);
	pon_mbox_cnt_gem_port_del(1, stat);

	struct pon_eth_counters *cnt = pon_mbox_cnt_tx_eth_table_get(0, stat);

	ASSERT_EQ(cnt, nullptr);

	pon_mbox_cnt_gem_port_add(0, stat);
	cnt = pon_mbox_cnt_tx_eth_table_get(0, stat);

	ASSERT_NE(cnt, nullptr);
	cnt->bytes = 10;

	cnt = pon_mbox_cnt_tx_eth_table_get(1, stat);
	ASSERT_EQ(cnt, nullptr);
	pon_mbox_cnt_gem_port_add(1, stat);
	cnt = pon_mbox_cnt_tx_eth_table_get(1, stat);

	ASSERT_NE(cnt, nullptr);
	cnt->bytes = 20;

	cnt = pon_mbox_cnt_tx_eth_table_get(0, stat);
	ASSERT_EQ(cnt->bytes, (__u64) 10);

	cnt = pon_mbox_cnt_tx_eth_table_get(1, stat);
	ASSERT_EQ(cnt->bytes, (__u64) 20);
	ASSERT_EQ(0, pon_mbox_cnt_gem_port_del(0, stat));
	ASSERT_EQ(0, pon_mbox_cnt_gem_port_del(1, stat));
	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		  mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, eth_rx_table_get)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);
	pon_mbox_cnt_gem_port_del(0, stat);
	pon_mbox_cnt_gem_port_del(1, stat);

	struct pon_eth_counters *cnt = pon_mbox_cnt_rx_eth_table_get(0, stat);

	ASSERT_EQ(cnt, nullptr);

	pon_mbox_cnt_gem_port_add(0, stat);
	cnt = pon_mbox_cnt_rx_eth_table_get(0, stat);

	ASSERT_NE(cnt, nullptr);
	cnt->bytes = 10;

	cnt = pon_mbox_cnt_rx_eth_table_get(1, stat);
	ASSERT_EQ(cnt, nullptr);
	pon_mbox_cnt_gem_port_add(1, stat);
	cnt = pon_mbox_cnt_rx_eth_table_get(1, stat);

	ASSERT_NE(cnt, nullptr);
	cnt->bytes = 20;

	cnt = pon_mbox_cnt_rx_eth_table_get(0, stat);
	ASSERT_EQ(cnt->bytes, (__u64) 10);

	cnt = pon_mbox_cnt_rx_eth_table_get(1, stat);
	ASSERT_EQ(cnt->bytes, (__u64) 20);
	ASSERT_EQ(0, pon_mbox_cnt_gem_port_del(0, stat));
	ASSERT_EQ(0, pon_mbox_cnt_gem_port_del(1, stat));
	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, process_counter)
{
	__u64 dst = 0, fw_resp, last;

	// +10
	fw_resp = 10;
	last = 0;
	process_counter(&dst, &fw_resp, &last, 0x10);

	ASSERT_EQ(dst, (__u64)10);
	ASSERT_EQ(last, (__u64)10);
	ASSERT_EQ(fw_resp, (__u64)10);

	// add to full range of fw counter
	fw_resp = 0xf;
	last = 10;
	process_counter(&dst, &fw_resp, &last, 0x10);

	ASSERT_EQ(dst, (__u64)0xf);
	ASSERT_EQ(last, (__u64) 0xf);
	ASSERT_EQ(fw_resp, (__u64)0xf);

	// overflow fw counter by 1
	fw_resp = 0x0;
	process_counter(&dst, &fw_resp, &last, 0x10);

	ASSERT_EQ(dst, (__u64)0xf+1);
	ASSERT_EQ(last, (__u64)0);
	ASSERT_EQ(fw_resp, (__u64)0);

	// count 1 more
	fw_resp = 0x1;
	process_counter(&dst, &fw_resp, &last, 0x10);

	ASSERT_EQ(dst, (__u64)0xf+2);
	ASSERT_EQ(last, (__u64)0x1);
	ASSERT_EQ(fw_resp, (__u64)0x1);
}

template<typename Setter>
struct updater {
	const __u64 m_max;
	Setter &m_setter;

	updater(__u64 max, Setter &sfun):
		m_max(max), m_setter(sfun)
	{
	}

	void set(const __u64 val)
	{
		m_setter(val);
	}

	void maximize()
	{
		m_setter(m_max);
	}
};

/**
 * @brief Common counter test code, checks for correctness of overflows and
 *	  hi/lo usage
 *
 * @tparam CounterType deduced from counter type
 * @param cnt counter reference
 * @param fwmax maximum value stored in firmware counter
 * @param adder function which will add values to counter table (*_add and
 *		helpers)
 * @param updater firmware message fields updater function - will set fields to
 *	  given value
 */
template<typename CounterType>
void test_counter(CounterType &cnt, const CounterType fwmax,
		  std::function<int()> adder, std::function<void(__u64)> updater)
{
	// reset internals
	updater(0);

	ASSERT_EQ(0, adder());

	cnt = 0;

	// **** simple add ****
	cnt = 5;

	// fw set to the same value as 'last' - no difference
	updater(0);

	ASSERT_EQ(0, adder());
	ASSERT_EQ((__u64)5, cnt);

	// now 10 events more, cnt should be +10
	updater(10);

	ASSERT_EQ(0, adder());
	ASSERT_EQ((__u64)15, cnt);

	// manually change cnt value to be different from fw
	// now, counter should not change as fw was not changed, but the value
	// of the cnt should be different as we manually increased it
	cnt += 20;

	ASSERT_EQ(0, adder());
	ASSERT_EQ((__u64)35, cnt);

	// increase one more time, difference is '1'
	updater(11);

	ASSERT_EQ(0, adder());
	ASSERT_EQ((__u64)36, cnt);

	// **** overflow test ****
	// prepare internals - reset counter to 0
	updater(0);

	ASSERT_EQ(0, adder());

	cnt = 0;

	// set last value and counter to maximum
	updater(fwmax);

	ASSERT_EQ(0, adder());
	ASSERT_EQ(fwmax, cnt);

	// increase by 1
	updater(0);

	ASSERT_EQ(0, adder());
	ASSERT_EQ(fwmax+1, cnt);

	// increase by 1 again
	updater(1);

	ASSERT_EQ(0, adder());
	ASSERT_EQ(fwmax+2, cnt);
	//
}

/**
 * @brief Splits 'var' into hi and lo variables putting 8bits into 'hi' and 32
 *	  bits 'lo'
 *
 * @param val value to split
 * @param hi high part destination variable
 * @param lo low part destination variable
 */
#define HI8LO32(val, hi, lo) \
{\
	hi = (val & ((__u64) 0xff << 32)) >> 32;\
	lo = val & 0xffffffff;\
}

/**
 * @brief Splits 'var' into hi and lo variables putting 32 bits into 'hi' and 32
 *	  bits 'lo'
 *
 * @param val value to split
 * @param hi high part destination variable
 * @param lo low part destination variable
 */
#define HI32LO32(val, hi, lo) \
{\
	hi = (val & ((__u64) 0xffffffff << 32)) >> 32;\
	lo = val & 0xffffffff;\
}

/**
 * @brief get integer with number of 'bits' set in, starting from first bit
 *
 * @param bits number of bits (up to 63)
 */
#define MAX_ON_BITS(bits) (((__u64) 1 << (bits))-1)


/**
 * @brief Set magic value for field (0x55 pattern)
 *
 * @param field pointer and sizeof will be taken from given value
 */
#define RESET_FOR_CSUM(field) memset(&(field), 0x55, sizeof((field)))

/**
 * @brief Calculate checksum for the structure
 *
 * @param data pointer to structure
 * @param bytes size in bytes of the structure
 *
 * @return checksum value
 */
uint64_t checksum(void *data, size_t bytes)
{
	uint64_t ret = 0;

	for (size_t i = 0; i < bytes; i++) {
		ret += ((uint8_t*)data)[i];
	}

	return ret;
}

TEST(Counters, gem_port_counters_add)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);
	pon_mbox_cnt_gem_port_del(0, stat);

	pon_mbox_cnt_gem_port_add(0, stat);
	struct pon_mbox_gem_port_counters *cnt =
		pon_mbox_cnt_gem_port_table_get(
			PON_MBOX_D_DSWLCH_ID_CURR, 0, stat);

	ASSERT_NE(cnt, nullptr);
	struct ponfw_gem_port_counters fw_cnt = {};

	auto adder = [&]() {
			struct pon_mbox_gem_port_counters cntrs = {};
			pon_mbox_cnt_gem_port_fw2pon(&fw_cnt, &cntrs);
			return pon_mbox_cnt_gem_port_table_add(0, &cntrs, stat);
			};

	uint64_t csum = 0;

	RESET_FOR_CSUM(*cnt);
	csum = checksum(cnt, sizeof(*cnt));

	SCOPED_TRACE("tx_frames");
	test_counter(cnt->tx_frames,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.tx_gem_fram_hi,
				fw_cnt.tx_gem_fram_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->tx_frames);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("tx_fragments");
	test_counter(cnt->tx_fragments,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.tx_gem_frag_hi,
				fw_cnt.tx_gem_frag_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->tx_fragments);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("tx_bytes");
	test_counter(cnt->tx_bytes,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.tx_bytes_hi,
				fw_cnt.tx_bytes_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->tx_bytes);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rx_frames");
	test_counter(cnt->rx_frames,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_gem_fram_hi,
				fw_cnt.rx_gem_fram_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->rx_frames);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rx_fragments");
	test_counter(cnt->rx_fragments,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_gem_frag_hi,
				fw_cnt.rx_gem_frag_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->rx_fragments);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rx_bytes");
	test_counter(cnt->rx_bytes,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_bytes_hi,
				fw_cnt.rx_bytes_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->rx_bytes);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("key_errors");
	test_counter(cnt->key_errors,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.ds_xgem_key_err_hi,
				fw_cnt.ds_xgem_key_err_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->key_errors);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, gtc_counters_add)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);

	struct pon_mbox_gtc_counters *cnt =
		pon_mbox_cnt_gtc_table_get(PON_MBOX_D_DSWLCH_ID_CURR, stat);

	ASSERT_NE(cnt, nullptr);
	struct ponfw_gtc_counters fw_cnt1 = {};
	struct ponfw_gtc_enhanced_counters fw_cnt2 = {};

	auto adder = [&]() {
			struct pon_mbox_gtc_counters cntrs = {};
			pon_mbox_cnt_gtc_fw2pon(&fw_cnt1, &cntrs);
			pon_mbox_cnt_gtc_enh_fw2pon(&fw_cnt2, &cntrs);
			return pon_mbox_cnt_gtc_table_add(&cntrs, stat);
			};

	uint64_t csum = 0;

	RESET_FOR_CSUM(*cnt);
	csum = checksum(cnt, sizeof(*cnt));

	SCOPED_TRACE("bip_errors");
	test_counter(cnt->bip_errors,
		     MAX_ON_BITS(32),
		     adder,
		     [&](__u64 val) {
			fw_cnt1.ds_bip_err = (__u32) val;
		     }
		);

	RESET_FOR_CSUM(cnt->bip_errors);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("disc_gem_frames");
	test_counter(cnt->disc_gem_frames,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt1.dis_gem_hi,
				fw_cnt1.dis_gem_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->disc_gem_frames);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("gem_hec_errors_corr");
	test_counter(cnt->gem_hec_errors_corr,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt2.gem_hec_err_corr_hi,
				fw_cnt2.gem_hec_err_corr_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->gem_hec_errors_corr);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("gem_hec_errors_uncorr");
	test_counter(cnt->gem_hec_errors_uncorr,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt1.gem_hec_err_hi,
				fw_cnt1.gem_hec_err_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->gem_hec_errors_uncorr);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("bwmap_hec_errors_corr");
	test_counter(cnt->bwmap_hec_errors_corr,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt2.bwmap_err_corr_hi,
				fw_cnt2.bwmap_err_corr_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->bwmap_hec_errors_corr);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("bytes_corr");
	test_counter(cnt->bytes_corr,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			 HI8LO32(val, fw_cnt1.corr_bytes_hi,
				fw_cnt1.corr_bytes_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->bytes_corr);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("fec_codewords_corr");
	test_counter(cnt->fec_codewords_corr,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			 HI8LO32(val, fw_cnt1.corr_fec_cw_hi,
				fw_cnt1.corr_fec_cw_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->fec_codewords_corr);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("fec_codewords_uncorr");
	test_counter(cnt->fec_codewords_uncorr,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			 HI8LO32(val, fw_cnt1.uncorr_fec_cw_hi,
				fw_cnt1.uncorr_fec_cw_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->fec_codewords_uncorr);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("total_frames");
	test_counter(cnt->total_frames,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			 HI8LO32(val, fw_cnt1.frames_total_hi,
				fw_cnt1.frames_total_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->total_frames);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("fec_sec");
	test_counter(cnt->fec_sec,
		     MAX_ON_BITS(32),
		     adder,
		     [&](__u64 val) {
			 fw_cnt1.fec_sec = (__u32) val;
		     }
		);

	RESET_FOR_CSUM(cnt->fec_sec);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("gem_idle");
	test_counter(cnt->gem_idle,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			 HI8LO32(val, fw_cnt1.idle_gem_hi,
				fw_cnt1.idle_gem_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->gem_idle);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("lods_events");
	test_counter(cnt->lods_events,
		     MAX_ON_BITS(32),
		     adder,
		     [&](__u64 val) {
			fw_cnt2.total_lods_lof = (__u32) val;
		     }
		);

	RESET_FOR_CSUM(cnt->lods_events);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("dg_time");
	test_counter(cnt->dg_time,
		     MAX_ON_BITS(32),
		     adder,
		     [&](__u64 val) {
			fw_cnt2.dg_time = (__u32) val;
		     }
		);

	RESET_FOR_CSUM(cnt->dg_time);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("ploam_crc_errors");
	test_counter(cnt->ploam_crc_errors,
		     MAX_ON_BITS(32),
		     adder,
		     [&](__u64 val) {
			fw_cnt1.ploam_crc_err = (__u32) val;
		     }
		);

	RESET_FOR_CSUM(cnt->ploam_crc_errors);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, xgtc_counters_add)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);

	struct pon_mbox_xgtc_counters *cnt =
		pon_mbox_cnt_xgtc_table_get(PON_MBOX_D_DSWLCH_ID_CURR, stat);

	ASSERT_NE(cnt, nullptr);
	struct ponfw_xgtc_counters fw_cnt = {};

	auto adder = [&]() {
			struct pon_mbox_xgtc_counters cntrs = {};
			pon_mbox_cnt_xgtc_fw2pon(&fw_cnt, &cntrs);
			return pon_mbox_cnt_xgtc_table_add(&cntrs, stat);
			};

	uint64_t csum = 0;

	RESET_FOR_CSUM(*cnt);
	csum = checksum(cnt, sizeof(*cnt));

	SCOPED_TRACE("psbd_hec_err_uncorr");
	test_counter(cnt->psbd_hec_err_uncorr,
			MAX_ON_BITS(40),
			adder,
			[&](__u64 val) {
			     HI8LO32(val, fw_cnt.psbd_err_uncorr_hi,
				     fw_cnt.psbd_err_uncorr_lo);
			});

	RESET_FOR_CSUM(cnt->psbd_hec_err_uncorr);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("psbd_hec_err_corr");
	test_counter(cnt->psbd_hec_err_corr,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.psbd_err_corr_hi,
				fw_cnt.psbd_err_corr_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->psbd_hec_err_corr);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("fs_hec_err_uncorr");
	test_counter(cnt->fs_hec_err_uncorr,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.fs_err_uncorr_hi,
				fw_cnt.fs_err_uncorr_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->fs_hec_err_uncorr);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("fs_hec_err_corr");
	test_counter(cnt->fs_hec_err_corr,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.fs_err_corr_hi,
				fw_cnt.fs_err_corr_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->fs_hec_err_corr);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("lost_words");
	test_counter(cnt->lost_words,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.lost_hec_wc_hi,
				fw_cnt.lost_hec_wc_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->lost_words);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("ploam_mic_err");
	test_counter(cnt->ploam_mic_err,
		     MAX_ON_BITS(32),
		     adder,
		     [&](__u64 val) {
			fw_cnt.ploam_mic_err = (__u32) val;
		     }
		);

	RESET_FOR_CSUM(cnt->ploam_mic_err);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, alloc_id_counters_add)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);
	pon_mbox_cnt_alloc_id_del(0, stat);
	pon_mbox_cnt_alloc_id_add(0, stat);

	struct pon_alloc_counters *cnt = pon_mbox_cnt_alloc_id_table_get(0, stat);

	ASSERT_NE(cnt, nullptr);
	struct ponfw_alloc_id_counters fw_cnt1 = {};
	struct ponfw_alloc_bw fw_cnt2 = {};

	auto adder = [&]() {
			struct pon_alloc_counters cntrs = {};
			pon_mbox_cnt_alloc_id_fw2pon(&fw_cnt1, &cntrs);
			pon_mbox_cnt_alloc_bw_fw2pon(&fw_cnt2, &cntrs);
			return pon_mbox_cnt_alloc_id_table_add(0, &cntrs, stat);
			};

	uint64_t csum = 0;

	// make sure the us_bw fields are set to the reset pattern
	RESET_FOR_CSUM(fw_cnt2.us_bw_hi);
	RESET_FOR_CSUM(fw_cnt2.us_bw_lo);

	RESET_FOR_CSUM(*cnt);
	csum = checksum(cnt, sizeof(*cnt));

	SCOPED_TRACE("allocations");
	test_counter(cnt->allocations,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt1.rec_tcont_alloc_hi,
				fw_cnt1.rec_tcont_alloc_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->allocations);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("idle");
	test_counter(cnt->idle,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt1.us_gem_idle_hi,
				fw_cnt1.us_gem_idle_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->idle);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, alloc_lost_counters_add)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);

	struct pon_alloc_discard_counters *cnt =
	    pon_mbox_cnt_alloc_lost_table_get(stat);

	ASSERT_NE(cnt, nullptr);
	struct ponfw_alloc_lost_counters fw_cnt = {};

	auto adder = [&]() {
			struct pon_alloc_discard_counters cntrs = {};
			pon_mbox_cnt_alloc_lost_fw2pon(&fw_cnt, &cntrs);
			return pon_mbox_cnt_alloc_lost_table_add(&cntrs, stat);
			};

	uint64_t csum = 0;

	RESET_FOR_CSUM(*cnt);
	csum = checksum(cnt, sizeof(*cnt));

	SCOPED_TRACE("disc0");
	test_counter(cnt->disc[0],
			MAX_ON_BITS(40),
			adder,
			[&](__u64 val) {
			     HI8LO32(val, fw_cnt.discard0_hi,
				     fw_cnt.discard0_lo);
			});

	RESET_FOR_CSUM(cnt->disc[0]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("disc1");
	test_counter(cnt->disc[1],
			MAX_ON_BITS(40),
			adder,
			[&](__u64 val) {
			     HI8LO32(val, fw_cnt.discard1_hi,
				     fw_cnt.discard1_lo);
			});

	RESET_FOR_CSUM(cnt->disc[1]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("disc2");
	test_counter(cnt->disc[2],
			MAX_ON_BITS(40),
			adder,
			[&](__u64 val) {
			     HI8LO32(val, fw_cnt.discard2_hi,
				     fw_cnt.discard2_lo);
			});

	RESET_FOR_CSUM(cnt->disc[2]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("disc3");
	test_counter(cnt->disc[3],
			MAX_ON_BITS(40),
			adder,
			[&](__u64 val) {
			     HI8LO32(val, fw_cnt.discard3_hi,
				     fw_cnt.discard3_lo);
			});

	RESET_FOR_CSUM(cnt->disc[3]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("disc4");
	test_counter(cnt->disc[4],
			MAX_ON_BITS(40),
			adder,
			[&](__u64 val) {
			     HI8LO32(val, fw_cnt.discard4_hi,
				     fw_cnt.discard4_lo);
			});

	RESET_FOR_CSUM(cnt->disc[4]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("disc5");
	test_counter(cnt->disc[5],
			MAX_ON_BITS(40),
			adder,
			[&](__u64 val) {
			     HI8LO32(val, fw_cnt.discard5_hi,
				     fw_cnt.discard5_lo);
			});

	RESET_FOR_CSUM(cnt->disc[5]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("disc6");
	test_counter(cnt->disc[6],
			MAX_ON_BITS(40),
			adder,
			[&](__u64 val) {
			     HI8LO32(val, fw_cnt.discard6_hi,
				     fw_cnt.discard6_lo);
			});

	RESET_FOR_CSUM(cnt->disc[6]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("disc7");
	test_counter(cnt->disc[7],
			MAX_ON_BITS(40),
			adder,
			[&](__u64 val) {
			     HI8LO32(val, fw_cnt.discard7_hi,
				     fw_cnt.discard7_lo);
			});

	RESET_FOR_CSUM(cnt->disc[7]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));
// rule

	SCOPED_TRACE("rule2");
	test_counter(cnt->rule[2],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.rule2 = (__u32) val;
			});

	RESET_FOR_CSUM(cnt->rule[2]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rule4");
	test_counter(cnt->rule[4],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.rule4 = (__u32) val;
			});

	RESET_FOR_CSUM(cnt->rule[4]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));


	SCOPED_TRACE("rule6");
	test_counter(cnt->rule[6],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.rule6 = (__u32) val;
			});

	RESET_FOR_CSUM(cnt->rule[6]);

	SCOPED_TRACE("rule7");
	test_counter(cnt->rule[7],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.rule7 = (__u32) val;
			});

	RESET_FOR_CSUM(cnt->rule[7]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));



	SCOPED_TRACE("rule8");
	test_counter(cnt->rule[8],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.rule8 = (__u32) val;
			});

	RESET_FOR_CSUM(cnt->rule[8]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));


	SCOPED_TRACE("rule9");
	test_counter(cnt->rule[9],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.rule9 = (__u32) val;
			});

	RESET_FOR_CSUM(cnt->rule[9]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));


	SCOPED_TRACE("rule10");
	test_counter(cnt->rule[10],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.rule10 = (__u32) val;
			});

	RESET_FOR_CSUM(cnt->rule[10]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));


	SCOPED_TRACE("rule11");
	test_counter(cnt->rule[11],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.rule11 = (__u32) val;
			});

	RESET_FOR_CSUM(cnt->rule[11]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));


	SCOPED_TRACE("rule12");
	test_counter(cnt->rule[12],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.rule12 = (__u32) val;
			});

	RESET_FOR_CSUM(cnt->rule[12]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));


	SCOPED_TRACE("rule13");
	test_counter(cnt->rule[13],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.rule13 = (__u32) val;
			});

	RESET_FOR_CSUM(cnt->rule[13]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));


	SCOPED_TRACE("rule14");
	test_counter(cnt->rule[14],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.rule14 = (__u32) val;
			});

	RESET_FOR_CSUM(cnt->rule[14]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));


	SCOPED_TRACE("rule15");
	test_counter(cnt->rule[15],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.rule15 = (__u32) val;
			});

	RESET_FOR_CSUM(cnt->rule[15]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));


	SCOPED_TRACE("rule16");
	test_counter(cnt->rule[16],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.rule16 = (__u32) val;
			});

	RESET_FOR_CSUM(cnt->rule[16]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, rx_eth_counters_add)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);
	pon_mbox_cnt_gem_port_del(0, stat);

	pon_mbox_cnt_gem_port_add(0, stat);
	struct pon_eth_counters *cnt = pon_mbox_cnt_rx_eth_table_get(0, stat);

	ASSERT_NE(cnt, nullptr);
	struct ponfw_rx_eth_counters fw_cnt = {};

	auto adder = [&]() {
			struct pon_eth_counters cntrs = {};
			pon_mbox_cnt_rx_eth_fw2pon(&fw_cnt, &cntrs);
			return pon_mbox_cnt_rx_eth_table_add(0, &cntrs, stat);
			};

	uint64_t csum = 0;

	RESET_FOR_CSUM(*cnt);
	csum = checksum(cnt, sizeof(*cnt));

	SCOPED_TRACE("rx_bytes");
	test_counter(cnt->bytes,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_eth_bytes_hi,
				fw_cnt.rx_eth_bytes_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->bytes);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rx_frames_lt_64");
	test_counter(cnt->frames_lt_64,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_eth_fr_lt64_hi,
				fw_cnt.rx_eth_fr_lt64_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_lt_64);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rx_frames_64");
	test_counter(cnt->frames_64,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_eth_fr_eq64_hi,
				fw_cnt.rx_eth_fr_eq64_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_64);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rx_frames_65_127");
	test_counter(cnt->frames_65_127,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_eth_fr_lt128_hi,
				fw_cnt.rx_eth_fr_lt128_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_65_127);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rx_frames_128_255");
	test_counter(cnt->frames_128_255,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_eth_fr_lt256_hi,
				fw_cnt.rx_eth_fr_lt256_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_128_255);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rx_frames_256_511");
	test_counter(cnt->frames_256_511,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_eth_fr_lt512_hi,
				fw_cnt.rx_eth_fr_lt512_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_256_511);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rx_frames_512_1023");
	test_counter(cnt->frames_512_1023,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_eth_fr_lt1k_hi,
				fw_cnt.rx_eth_fr_lt1k_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_512_1023);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rx_frames_1024_1518");
	test_counter(cnt->frames_1024_1518,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_eth_fr_lt1k5_hi,
				fw_cnt.rx_eth_fr_lt1k5_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_1024_1518);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rx_frames_gt_1518");
	test_counter(cnt->frames_gt_1518,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_eth_fr_gt1k5_hi,
				fw_cnt.rx_eth_fr_gt1k5_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_gt_1518);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rx_frames_fcs_err");
	test_counter(cnt->frames_fcs_err,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_eth_fcs_err_hi,
				fw_cnt.rx_eth_fcs_err_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_fcs_err);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rx_bytes_fcs_err");
	test_counter(cnt->bytes_fcs_err,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_eth_fcs_bytes_hi,
				fw_cnt.rx_eth_fcs_bytes_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->bytes_fcs_err);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("rx_frames_too_long");
	test_counter(cnt->frames_too_long,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.rx_eth_too_long_hi,
				fw_cnt.rx_eth_too_long_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_too_long);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, tx_eth_counters_add)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);
	pon_mbox_cnt_gem_port_del(0, stat);

	pon_mbox_cnt_gem_port_add(0, stat);
	struct pon_eth_counters *cnt = pon_mbox_cnt_tx_eth_table_get(0, stat);

	ASSERT_NE(cnt, nullptr);
	struct ponfw_tx_eth_counters fw_cnt = {};

	auto adder = [&]() {
			struct pon_eth_counters cntrs = {};
			pon_mbox_cnt_tx_eth_fw2pon(&fw_cnt, &cntrs);
			return pon_mbox_cnt_tx_eth_table_add(0, &cntrs, stat);
			};

	uint64_t csum = 0;

	RESET_FOR_CSUM(*cnt);
	csum = checksum(cnt, sizeof(*cnt));

	SCOPED_TRACE("tx_bytes");
	test_counter(cnt->bytes,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.tx_eth_bytes_hi,
				fw_cnt.tx_eth_bytes_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->bytes);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("tx_frames_lt_64");
	test_counter(cnt->frames_lt_64,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.tx_eth_fr_lt64_hi,
				fw_cnt.tx_eth_fr_lt64_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_lt_64);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("tx_frames_64");
	test_counter(cnt->frames_64,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.tx_eth_fr_eq64_hi,
				fw_cnt.tx_eth_fr_eq64_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_64);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("tx_frames_65_127");
	test_counter(cnt->frames_65_127,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.tx_eth_fr_lt128_hi,
				fw_cnt.tx_eth_fr_lt128_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_65_127);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("tx_frames_128_255");
	test_counter(cnt->frames_128_255,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.tx_eth_fr_lt256_hi,
				fw_cnt.tx_eth_fr_lt256_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_128_255);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("tx_frames_256_511");
	test_counter(cnt->frames_256_511,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.tx_eth_fr_lt512_hi,
				fw_cnt.tx_eth_fr_lt512_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_256_511);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("tx_frames_512_1023");
	test_counter(cnt->frames_512_1023,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.tx_eth_fr_lt1k_hi,
				fw_cnt.tx_eth_fr_lt1k_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_512_1023);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("tx_frames_1024_1518");
	test_counter(cnt->frames_1024_1518,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.tx_eth_fr_lt1k5_hi,
				fw_cnt.tx_eth_fr_lt1k5_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_1024_1518);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("tx_frames_gt_1518");
	test_counter(cnt->frames_gt_1518,
		     MAX_ON_BITS(40),
		     adder,
		     [&](__u64 val) {
			HI8LO32(val, fw_cnt.tx_eth_fr_gt1k5_hi,
				fw_cnt.tx_eth_fr_gt1k5_lo);
		     }
		);

	RESET_FOR_CSUM(cnt->frames_gt_1518);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

/* TODO TWDM tests */

TEST(Counters, xgtc_ploam_ds_counters_add)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);

	struct pon_mbox_xgtc_ploam_ds_counters *cnt =
		pon_mbox_cnt_xgtc_ploam_ds_table_get(PON_MBOX_D_DSWLCH_ID_CURR, stat);

	ASSERT_NE(cnt, nullptr);
	struct ponfw_xgtc_ploam_ds_counters fw_cnt = {};

	auto adder = [&]() {
			struct pon_mbox_xgtc_ploam_ds_counters cntrs = {};
			pon_mbox_cnt_xgtc_ploam_ds_fw2pon(&fw_cnt, &cntrs);
			return pon_mbox_cnt_xgtc_ploam_ds_table_add(&cntrs, stat);
			};

	uint64_t csum = 0;

	RESET_FOR_CSUM(*cnt);
	csum = checksum(cnt, sizeof(*cnt));

	SCOPED_TRACE("bst_profile");
	test_counter(cnt->bst_profile,
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.bst_profile = val;
			});
	RESET_FOR_CSUM(cnt->bst_profile);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("tune_compl");
	test_counter(cnt->tune_compl,
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.tune_compl = val;
			});
	RESET_FOR_CSUM(cnt->tune_compl);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	/* add other counters as well */

	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, gtc_ploam_ds_counters_add)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);

	struct pon_mbox_gtc_ploam_ds_counters *cnt = pon_mbox_cnt_gtc_ploam_ds_table_get(stat);

	ASSERT_NE(cnt, nullptr);
	struct ponfw_gtc_ploam_ds_counters fw_cnt = {};

	auto adder = [&]() {
			struct pon_mbox_gtc_ploam_ds_counters cntrs = {};
			pon_mbox_cnt_gtc_ploam_ds_fw2pon(&fw_cnt, &cntrs);
			return pon_mbox_cnt_gtc_ploam_ds_table_add(&cntrs, stat);
			};

	uint64_t csum = 0;

	RESET_FOR_CSUM(*cnt);
	csum = checksum(cnt, sizeof(*cnt));

	SCOPED_TRACE("swift_popup");
	test_counter(cnt->swift_popup,
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.swift_popup = val;
			});
	RESET_FOR_CSUM(cnt->swift_popup);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("ass_onu");
	test_counter(cnt->ass_onu,
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.ass_onu = val;
			});
	RESET_FOR_CSUM(cnt->ass_onu);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	/* add other counters as well */

	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, xgtc_ploam_us_counters_add)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);

	struct pon_mbox_xgtc_ploam_us_counters *cnt =
		pon_mbox_cnt_xgtc_ploam_us_table_get(PON_MBOX_D_DSWLCH_ID_CURR, stat);

	ASSERT_NE(cnt, nullptr);
	struct ponfw_xgtc_ploam_us_counters fw_cnt = {};

	auto adder = [&]() {
			struct pon_mbox_xgtc_ploam_us_counters cntrs = {};
			pon_mbox_cnt_xgtc_ploam_us_fw2pon(&fw_cnt, &cntrs);
			return pon_mbox_cnt_xgtc_ploam_us_table_add(&cntrs, stat);
			};

	uint64_t csum = 0;

	RESET_FOR_CSUM(*cnt);
	csum = checksum(cnt, sizeof(*cnt));

	SCOPED_TRACE("ser_onu");
	test_counter(cnt->ser_onu,
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.ser_onu = val;
			});
	RESET_FOR_CSUM(cnt->ser_onu);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("tun_res_crb");
	test_counter(cnt->tun_res_crb,
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.tun_res_crb = val;
			});
	RESET_FOR_CSUM(cnt->tun_res_crb);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	/* add other counters as well */

	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, gtc_ploam_us_counters_add)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);

	struct pon_mbox_gtc_ploam_us_counters *cnt = pon_mbox_cnt_gtc_ploam_us_table_get(stat);

	ASSERT_NE(cnt, nullptr);
	struct ponfw_gtc_ploam_us_counters fw_cnt = {};

	auto adder = [&]() {
			struct pon_mbox_gtc_ploam_us_counters cntrs = {};
			pon_mbox_cnt_gtc_ploam_us_fw2pon(&fw_cnt, &cntrs);
			return pon_mbox_cnt_gtc_ploam_us_table_add(&cntrs, stat);
			};

	uint64_t csum = 0;

	RESET_FOR_CSUM(*cnt);
	csum = checksum(cnt, sizeof(*cnt));

	SCOPED_TRACE("password");
	test_counter(cnt->password,
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.password = val;
			});
	RESET_FOR_CSUM(cnt->password);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("slp_req");
	test_counter(cnt->slp_req,
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.slp_req = val;
			});
	RESET_FOR_CSUM(cnt->slp_req);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	/* add other counters as well */

	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}

TEST(Counters, twdm_tc_counters_add)
{
	mocks::reset();
	struct counters_state *stat = pon_mbox_cnt_state_init();
	/* delete, we need to make sure we won't have anything there */
	pon_mbox_cnt_lock(stat);

	struct pon_mbox_twdm_tc_counters *cnt =
		pon_mbox_cnt_twdm_tc_table_get(PON_MBOX_D_DSWLCH_ID_CURR, stat);

	ASSERT_NE(cnt, nullptr);
	struct ponfw_twdm_tc_counters fw_cnt = {};

	auto adder = [&]() {
			struct pon_mbox_twdm_tc_counters cntrs = {};
			pon_mbox_cnt_twdm_tc_fw2pon(&fw_cnt, &cntrs);
			return pon_mbox_cnt_twdm_tc_table_add(&cntrs, stat);
			};

	uint64_t csum = 0;

	RESET_FOR_CSUM(*cnt);
	csum = checksum(cnt, sizeof(*cnt));

	SCOPED_TRACE("tc_0");
	test_counter(cnt->tc_[0],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.tc_[0] = val;
			});
	RESET_FOR_CSUM(cnt->tc_[0]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	SCOPED_TRACE("tc_33");
	test_counter(cnt->tc_[33],
			MAX_ON_BITS(32),
			adder,
			[&](__u64 val) {
			     fw_cnt.tc_[33] = val;
			});
	RESET_FOR_CSUM(cnt->tc_[33]);
	ASSERT_EQ(csum, checksum(cnt, sizeof(*cnt)));

	/* add other counters as well */

	pon_mbox_cnt_unlock(stat);

	// checks for paired locks
	ASSERT_EQ(mocks::state.mutex_lock.ret_calls,
		 mocks::state.mutex_unlock.ret_calls);

	//checks if we were using locks
	ASSERT_GT(mocks::state.mutex_lock.ret_calls, 0);
	pon_mbox_cnt_state_release(stat);
}
