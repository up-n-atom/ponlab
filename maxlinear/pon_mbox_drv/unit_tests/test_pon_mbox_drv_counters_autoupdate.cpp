/******************************************************************************
 * Copyright (c) 2022 - 2025 MaxLinear, Inc.
 * Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 ******************************************************************************/

#include <gtest/gtest.h>
#include <cstdint>
#include <set>
#include <unordered_map>

#include "mocks.hpp"
#include <linux/types.h>

#define pon_mbox_send mock_pon_mbox_send

struct pon_mbox *pon_mbox_dev;

#include "drv_pon_mbox_counters_update.c"
#include "drv_pon_mbox_counters_autoupdate.c"

class CountersAutoupdate : public ::testing::Test {
public:
	struct pon_mbox pon_mbox_test_dev;
	const unsigned int SECOND = 1000u;
	struct counters_state *stat;
	union {
		struct pon_mbox_gtc_counters gtc;
		struct pon_mbox_xgtc_counters xgtc;
		struct pon_mbox_gem_port_counters gem_port;
		struct pon_alloc_counters alloc;
		struct pon_alloc_discard_counters alloc_discard;
		struct pon_eth_counters rx_eth;
		struct pon_eth_counters tx_eth;
		struct pon_mbox_twdm_tc_counters twdm_tc;
		struct pon_mbox_xgtc_ploam_ds_counters xgtc_ploam_ds;
		struct pon_mbox_gtc_ploam_ds_counters gtc_ploam_ds;
		struct pon_mbox_xgtc_ploam_us_counters xgtc_ploam_us;
		struct pon_mbox_gtc_ploam_us_counters gtc_ploam_us;
	} tmp;

	void SetUp()
	{
		mocks::reset();
		stat = pon_mbox_cnt_state_init();
		pon_mbox_cnt_reset(stat);
		memset(&pon_mbox_test_dev, 0, sizeof(pon_mbox_test_dev));
		pon_mbox_test_dev.cnt_state = stat;
		pon_mbox_test_dev.ploam_state = 50;
		memset(&tmp, 0, sizeof(tmp));
	}

	void TearDown()
	{
		pon_mbox_cnt_state_release(stat);
	}

	struct counters_last_update last_update()
	{
		struct counters_last_update upd;
		pon_mbox_last_update_get(&upd, stat);
		return upd;
	}
};


TEST_F(CountersAutoupdate, pon_mbox_cnt_autoupdate_create)
{
	auto cnt = pon_mbox_cnt_autoupdate_create(&pon_mbox_test_dev, 1000);
	ASSERT_TRUE(cnt);
	ASSERT_TRUE(cnt->running);
	pon_mbox_cnt_autoupdate_destroy(cnt);
}

TEST_F(CountersAutoupdate, pon_mbox_cnt_autoupdate_update_when_in_reset)
{
	auto cnt = pon_mbox_cnt_autoupdate_create(&pon_mbox_test_dev, 1000);
	pon_mbox_test_dev.in_reset = 1;

	mocks::state.mock_jiffies.ret_default = 10000;

	pon_mbox_cnt_autoupdate_update(cnt);
	pon_mbox_cnt_autoupdate_destroy(cnt);

	ASSERT_EQ(mocks::state.mock_pon_mbox_send.ret_calls, 0);
}

TEST_F(CountersAutoupdate, pon_mbox_cnt_autoupdate_update_multiple_calls)
{
	auto cnt = pon_mbox_cnt_autoupdate_create(&pon_mbox_test_dev, 1);
	pon_mbox_test_dev.mode = PON_MODE_9807_XGSPON;

	mocks::state.mock_jiffies.ret_default = SECOND;

	std::unordered_map<unsigned int, unsigned int> cmds;
	cmds[PONFW_ALLOC_LOST_COUNTERS_CMD_ID] = 0;
	cmds[PONFW_XGTC_COUNTERS_CMD_ID] = 0;
	cmds[PONFW_GTC_COUNTERS_CMD_ID] = 0;
	cmds[PONFW_ALLOC_ID_COUNTERS_CMD_ID] = 0;
	cmds[PONFW_GEM_PORT_COUNTERS_CMD_ID] = 0;
	cmds[PONFW_TWDM_TC_COUNTERS_CMD_ID] = 0;
	cmds[PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID] = 0;
	cmds[PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID] = 0;

	mocks::state.mock_pon_mbox_send.set_function_handler([&] (auto cmd, auto a, auto b, auto c, auto d, auto e) {
		(void) a;
		(void) b;
		(void) c;
		(void) d;
		(void) e;
		cmds[cmd]++;
		return 0;
	});

	pon_mbox_cnt_gem_port_add(0, stat);
	pon_mbox_cnt_alloc_id_add(0, stat);

	for (auto i = 0; i < 100; ++i)
		pon_mbox_cnt_autoupdate_update(cnt);

	pon_mbox_cnt_autoupdate_destroy(cnt);

	ASSERT_EQ(cmds[PONFW_ALLOC_LOST_COUNTERS_CMD_ID], 1u);
	ASSERT_EQ(cmds[PONFW_XGTC_COUNTERS_CMD_ID], 1u);
	ASSERT_EQ(cmds[PONFW_GTC_COUNTERS_CMD_ID], 1u);
	ASSERT_EQ(cmds[PONFW_ALLOC_ID_COUNTERS_CMD_ID], 1u);
	ASSERT_EQ(cmds[PONFW_GEM_PORT_COUNTERS_CMD_ID], 1u);
	ASSERT_EQ(cmds[PONFW_TWDM_TC_COUNTERS_CMD_ID], 0u); /* NG-PON2 only */
	ASSERT_EQ(cmds[PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID], 1u);
	ASSERT_EQ(cmds[PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID], 1u);
}

TEST_F(CountersAutoupdate, pon_mbox_cnt_autoupdate_update_multiple_calls_ngpon2)
{
	auto cnt = pon_mbox_cnt_autoupdate_create(&pon_mbox_test_dev, 1);
	pon_mbox_test_dev.mode = PON_MODE_989_NGPON2_2G5;

	mocks::state.mock_jiffies.ret_default = SECOND;

	std::unordered_map<unsigned int, unsigned int> cmds;
	cmds[PONFW_XGTC_COUNTERS_CMD_ID] = 0;
	cmds[PONFW_GTC_COUNTERS_CMD_ID] = 0;
	cmds[PONFW_TWDM_TC_COUNTERS_CMD_ID] = 0;
	cmds[PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID] = 0;
	cmds[PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID] = 0;
	cmds[PONFW_GTC_PLOAM_US_COUNTERS_CMD_ID] = 0;
	cmds[PONFW_TWDM_LODS_COUNTERS_CMD_ID] = 0;
	cmds[PONFW_TWDM_ONU_OPTIC_PL_COUNTERS_CMD_ID] = 0;


	mocks::state.mock_pon_mbox_send.set_function_handler([&] (auto cmd, auto a, auto b, auto c, auto d, auto e) {
		(void) a;
		(void) b;
		(void) c;
		(void) d;
		(void) e;
		cmds[cmd]++;
		return 0;
	});

	pon_mbox_cnt_gem_port_add(0, stat);
	pon_mbox_cnt_alloc_id_add(0, stat);

	for (auto i = 0; i < 100; ++i)
		pon_mbox_cnt_autoupdate_update(cnt);

	pon_mbox_cnt_autoupdate_destroy(cnt);

	ASSERT_EQ(cmds[PONFW_XGTC_COUNTERS_CMD_ID], 1u);
	ASSERT_EQ(cmds[PONFW_GTC_COUNTERS_CMD_ID], 1u);
	ASSERT_EQ(cmds[PONFW_TWDM_TC_COUNTERS_CMD_ID], 1u);
	ASSERT_EQ(cmds[PONFW_TWDM_LODS_COUNTERS_CMD_ID], 1u);
	ASSERT_EQ(cmds[PONFW_TWDM_ONU_OPTIC_PL_COUNTERS_CMD_ID], 1u);
	ASSERT_EQ(cmds[PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID], 1u);
	ASSERT_EQ(cmds[PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID], 1u);
	ASSERT_EQ(cmds[PONFW_GTC_PLOAM_US_COUNTERS_CMD_ID], 0u);
}

TEST_F(CountersAutoupdate, pon_mbox_cnt_autoupdate_update_updates_counters)
{
	auto cnt = pon_mbox_cnt_autoupdate_create(&pon_mbox_test_dev, 1);
	std::set<unsigned int> cmds;
	pon_mbox_test_dev.mode = PON_MODE_9807_XGSPON;

	mocks::state.mock_pon_mbox_send.set_function_handler([&] (auto cmd, auto a, auto b, auto c, auto d, auto e) {
		(void) a;
		(void) b;
		(void) c;
		(void) d;
		(void) e;
		cmds.insert(cmd);
		return 0;
	});

	//Travel 2 seconds in time
	mocks::state.mock_jiffies.ret_default = 2 * SECOND;

	{
		SCOPED_TRACE("Initial update times should be 0");

		auto times = last_update();

		ASSERT_EQ(0u, times.gtc_counters);
		ASSERT_EQ(0u, times.xgtc_counters);
		ASSERT_EQ(0u, times.alloc_discard_counters);
	}

	{
		SCOPED_TRACE("Updates counters");
		pon_mbox_cnt_autoupdate_update(cnt);
		ASSERT_GT(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 0u);
		ASSERT_GT(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 0u);
		ASSERT_GT(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_ID_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GEM_PORT_COUNTERS_CMD_ID), 0u);

		auto times = last_update();

		ASSERT_EQ(2 * SECOND, times.gtc_counters);
		ASSERT_EQ(2 * SECOND, times.xgtc_counters);
		ASSERT_EQ(2 * SECOND, times.alloc_discard_counters);
		cmds.clear();
	}

	//Travel 0.5 seconds in time
	mocks::state.mock_jiffies.ret_default += SECOND / 2;

	{
		SCOPED_TRACE("Counters should not be updated if update time didn't exceed threshold");
		pon_mbox_cnt_autoupdate_update(cnt);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_ID_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GEM_PORT_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_TWDM_TC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID), 0u);

		auto times = last_update();

		ASSERT_EQ(2 * SECOND, times.gtc_counters);
		ASSERT_EQ(2 * SECOND, times.xgtc_counters);
		ASSERT_EQ(2 * SECOND, times.alloc_discard_counters);
		cmds.clear();
	}

	//Travel 1 second in time
	mocks::state.mock_jiffies.ret_default += SECOND;

	{
		SCOPED_TRACE("Refreshing counters should prevent update");
		pon_mbox_alloc_lost_counters_update(&tmp.alloc_discard,
						    &pon_mbox_test_dev);
		pon_mbox_gtc_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.gtc, &pon_mbox_test_dev);

		//Travel 0.5 second in time
		mocks::state.mock_jiffies.ret_default += SECOND / 2;

		cmds.clear();
		pon_mbox_cnt_autoupdate_update(cnt);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_ID_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GEM_PORT_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_TWDM_TC_COUNTERS_CMD_ID), 0u); /* NG-PON2 only */
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID), 1u);
		cmds.clear();
	}

	//Travel 0.5 second in time
	mocks::state.mock_jiffies.ret_default += SECOND / 2;

	{
		SCOPED_TRACE("Only counters that need update should be updated");
		pon_mbox_cnt_autoupdate_update(cnt);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_ID_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GEM_PORT_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_TWDM_TC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID), 0u);
		cmds.clear();
	}

	//Don't travel in time

	{
		SCOPED_TRACE("After adding gem ports they should be updated");
		pon_mbox_cnt_gem_port_add(0, stat);
		pon_mbox_cnt_autoupdate_update(cnt);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_ID_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GEM_PORT_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_TWDM_TC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID), 0u);
		cmds.clear();

		//Travel 10 seconds in time
		mocks::state.mock_jiffies.ret_default += 10 * SECOND;

		//Everything including gem ports should be updated again

		pon_mbox_cnt_autoupdate_update(cnt);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_ID_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GEM_PORT_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_TWDM_TC_COUNTERS_CMD_ID), 0u); /* NG-PON2 only */
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID), 1u);
		cmds.clear();
	}

	//Travel 10 seconds in time
	mocks::state.mock_jiffies.ret_default += 10 * SECOND;

	{
		SCOPED_TRACE("Deleting a gem port should not update it");
		pon_mbox_cnt_gem_port_del(0, stat);
		pon_mbox_cnt_autoupdate_update(cnt);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_ID_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GEM_PORT_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_TWDM_TC_COUNTERS_CMD_ID), 0u); /* NG-PON2 only */
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID), 1u);
		cmds.clear();
	}

	//Don't travel in time

	{
		SCOPED_TRACE("Update should update gem port after adding deleting and adding a gem port");
		pon_mbox_cnt_gem_port_add(1, stat);
		pon_mbox_cnt_autoupdate_update(cnt);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_ID_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GEM_PORT_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_TWDM_TC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID), 0u);
		cmds.clear();

		pon_mbox_cnt_gem_port_del(1, stat);
		pon_mbox_cnt_autoupdate_update(cnt);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_ID_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GEM_PORT_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_TWDM_TC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID), 0u);
		cmds.clear();

		pon_mbox_cnt_gem_port_add(1, stat);
		pon_mbox_cnt_autoupdate_update(cnt);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_ID_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GEM_PORT_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_TWDM_TC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID), 0u);
		cmds.clear();
	}

	//Don't travel in time

	{
		SCOPED_TRACE("Update should work after reset");
		pon_mbox_cnt_autoupdate_update(cnt);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_ID_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GEM_PORT_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_TWDM_TC_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID), 0u);
		cmds.clear();

		//Do a reset
		pon_mbox_cnt_reset(stat);

		pon_mbox_cnt_autoupdate_update(cnt);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_ALLOC_ID_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_GEM_PORT_COUNTERS_CMD_ID), 0u);
		ASSERT_EQ(cmds.count(PONFW_TWDM_TC_COUNTERS_CMD_ID), 0u); /* NG-PON2 only */
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID), 1u);
		ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID), 1u);
		cmds.clear();
	}

	pon_mbox_cnt_autoupdate_destroy(cnt);

	ASSERT_GT(mocks::state.mock_pon_mbox_send.ret_calls, 0);
}

TEST_F(CountersAutoupdate, pon_mbox_cnt_oldest_update_time_get_returns_oldest_update_time)
{
	auto cnt = pon_mbox_cnt_autoupdate_create(&pon_mbox_test_dev, 1);
	auto mask = pon_mbox_counters_enabled_get(&pon_mbox_test_dev);

	mocks::state.mock_jiffies.ret_default = 20 * SECOND;

	pon_mbox_cnt_gem_port_add(1, stat);
	pon_mbox_cnt_alloc_id_add(1, stat);

	ASSERT_EQ(0u, pon_mbox_cnt_oldest_update_time_get(stat, mask));

	pon_mbox_alloc_lost_counters_update(&tmp.alloc_discard, &pon_mbox_test_dev);
	pon_mbox_gtc_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.gtc, &pon_mbox_test_dev);
	pon_mbox_xgtc_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.xgtc, &pon_mbox_test_dev);
	pon_mbox_alloc_counters_update(1, &tmp.alloc, &pon_mbox_test_dev);
	pon_mbox_gem_port_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, 1, &tmp.gem_port, &pon_mbox_test_dev);
	pon_mbox_twdm_tc_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.twdm_tc, &pon_mbox_test_dev);
	pon_mbox_xgtc_ploam_ds_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.xgtc_ploam_ds, &pon_mbox_test_dev);
	pon_mbox_gtc_ploam_ds_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.gtc_ploam_ds, &pon_mbox_test_dev);
	pon_mbox_xgtc_ploam_us_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.xgtc_ploam_us, &pon_mbox_test_dev);
	pon_mbox_gtc_ploam_us_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.gtc_ploam_us, &pon_mbox_test_dev);

	ASSERT_EQ(20 * SECOND, pon_mbox_cnt_oldest_update_time_get(stat, mask));

	mocks::state.mock_jiffies.ret_default = 40 * SECOND;

	pon_mbox_xgtc_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.xgtc, &pon_mbox_test_dev);
	pon_mbox_alloc_counters_update(1, &tmp.alloc, &pon_mbox_test_dev);
	pon_mbox_gem_port_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, 1, &tmp.gem_port, &pon_mbox_test_dev);
	pon_mbox_twdm_tc_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.twdm_tc, &pon_mbox_test_dev);
	pon_mbox_xgtc_ploam_ds_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.xgtc_ploam_ds, &pon_mbox_test_dev);
	pon_mbox_gtc_ploam_ds_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.gtc_ploam_ds, &pon_mbox_test_dev);
	pon_mbox_xgtc_ploam_us_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.xgtc_ploam_us, &pon_mbox_test_dev);
	pon_mbox_gtc_ploam_us_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.gtc_ploam_us, &pon_mbox_test_dev);

	ASSERT_EQ(20 * SECOND, pon_mbox_cnt_oldest_update_time_get(stat, mask));

	pon_mbox_alloc_lost_counters_update(&tmp.alloc_discard, &pon_mbox_test_dev);
	pon_mbox_gtc_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.gtc, &pon_mbox_test_dev);

	ASSERT_EQ(40 * SECOND, pon_mbox_cnt_oldest_update_time_get(stat, mask));

	pon_mbox_cnt_autoupdate_destroy(cnt);
}

TEST_F(CountersAutoupdate, pon_mbox_cnt_oldest_update_time_get_returns_oldest_update_time_alloc_id_and_gem_ports)
{
	auto cnt = pon_mbox_cnt_autoupdate_create(&pon_mbox_test_dev, 1);
	auto mask = pon_mbox_counters_enabled_get(&pon_mbox_test_dev);

	mocks::state.mock_jiffies.ret_default = 20 * SECOND;

	pon_mbox_cnt_gem_port_add(1, stat);
	pon_mbox_cnt_alloc_id_add(1, stat);

	pon_mbox_alloc_lost_counters_update(&tmp.alloc_discard, &pon_mbox_test_dev);
	pon_mbox_gtc_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.gtc, &pon_mbox_test_dev);
	pon_mbox_xgtc_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.xgtc, &pon_mbox_test_dev);
	pon_mbox_gem_port_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, 1, &tmp.gem_port, &pon_mbox_test_dev);
	pon_mbox_twdm_tc_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.twdm_tc, &pon_mbox_test_dev);
	pon_mbox_xgtc_ploam_ds_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.xgtc_ploam_ds, &pon_mbox_test_dev);
	pon_mbox_gtc_ploam_ds_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.gtc_ploam_ds, &pon_mbox_test_dev);
	pon_mbox_xgtc_ploam_us_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.xgtc_ploam_us, &pon_mbox_test_dev);
	pon_mbox_gtc_ploam_us_counters_update(PON_MBOX_D_DSWLCH_ID_CURR, &tmp.gtc_ploam_us, &pon_mbox_test_dev);

	/* Alloc ID should not be updated */
	ASSERT_EQ(0u * SECOND, pon_mbox_cnt_oldest_update_time_get(stat, mask));

	pon_mbox_alloc_counters_update(1, &tmp.alloc, &pon_mbox_test_dev);

	/* Alloc ID should be updated */
	ASSERT_EQ(20 * SECOND, pon_mbox_cnt_oldest_update_time_get(stat, mask));

	pon_mbox_cnt_autoupdate_destroy(cnt);
}

TEST_F(CountersAutoupdate, pon_mbox_cnt_autoupdate_update_works_when_jiffies_overflow)
{
	std::set<unsigned int> cmds;

	mocks::state.mock_pon_mbox_send.set_function_handler([&] (auto cmd, auto a, auto b, auto c, auto d, auto e) {
		(void) a;
		(void) b;
		(void) c;
		(void) d;
		(void) e;
		cmds.insert(cmd);
		return 0;
	});

	auto cnt = pon_mbox_cnt_autoupdate_create(&pon_mbox_test_dev, 1);
	pon_mbox_test_dev.mode = PON_MODE_9807_XGSPON;

	mocks::state.mock_jiffies.ret_default = static_cast<unsigned long>(-1);

	pon_mbox_cnt_autoupdate_update(cnt);

	ASSERT_EQ(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 1u);
	ASSERT_EQ(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 1u);
	ASSERT_EQ(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 1u);
	ASSERT_EQ(cmds.count(PONFW_TWDM_TC_COUNTERS_CMD_ID), 0u); /* NG-PON2 only */
	ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID), 1u);
	ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID), 1u);
	cmds.clear();

	mocks::state.mock_jiffies.ret_default += SECOND / 2;

	pon_mbox_cnt_autoupdate_update(cnt);

	ASSERT_EQ(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 0u);
	ASSERT_EQ(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 0u);
	ASSERT_EQ(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 0u);
	ASSERT_EQ(cmds.count(PONFW_TWDM_TC_COUNTERS_CMD_ID), 0u);
	ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID), 0u);
	ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID), 0u);
	cmds.clear();

	mocks::state.mock_jiffies.ret_default += SECOND / 2;

	pon_mbox_cnt_autoupdate_update(cnt);

	ASSERT_EQ(cmds.count(PONFW_ALLOC_LOST_COUNTERS_CMD_ID), 1u);
	ASSERT_EQ(cmds.count(PONFW_XGTC_COUNTERS_CMD_ID), 1u);
	ASSERT_EQ(cmds.count(PONFW_GTC_COUNTERS_CMD_ID), 1u);
	ASSERT_EQ(cmds.count(PONFW_TWDM_TC_COUNTERS_CMD_ID), 0u); /* NG-PON2 only */
	ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID), 1u);
	ASSERT_EQ(cmds.count(PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID), 1u);
	cmds.clear();

	pon_mbox_cnt_autoupdate_destroy(cnt);
}

TEST_F(CountersAutoupdate, pon_mbox_cnt_autoupdate_works_in_different_modes)
{
	auto ploam_states = std::vector<u32> { 10, 20, 30, 40, 50, 70 };
	auto pon_modes = std::vector<pon_mode> {
		PON_MODE_984_GPON,
		PON_MODE_987_XGPON,
		PON_MODE_9807_XGSPON,
		PON_MODE_989_NGPON2_2G5,
		PON_MODE_989_NGPON2_10G,
	};

	//Iterate over every combination of PON mode and PLOAM state
	for (auto pon_mode : pon_modes) {
		for (auto ploam_state : ploam_states) {
			pon_mbox_test_dev.mode = pon_mode;
			pon_mbox_test_dev.ploam_state = ploam_state;

			auto cnt = pon_mbox_cnt_autoupdate_create(&pon_mbox_test_dev, 1);

			auto mask = pon_mbox_counters_enabled_get(&pon_mbox_test_dev);

			//Start from 100 seconds
			mocks::state.mock_jiffies.ret_default = SECOND * 100u;

			//Trigger counters autoupdate
			pon_mbox_cnt_autoupdate_update(cnt);

			//Last update time should equal 100 seconds
			ASSERT_EQ(100u * SECOND, pon_mbox_cnt_oldest_update_time_get(stat, mask));

			//Travel 100 seconds in time
			mocks::state.mock_jiffies.ret_default += SECOND * 100u;

			//Last update time should equal 100 seconds
			ASSERT_EQ(100u * SECOND, pon_mbox_cnt_oldest_update_time_get(stat, mask));

			//Trigger counters autoupdate
			pon_mbox_cnt_autoupdate_update(cnt);

			//Last update time should equal 200 seconds
			ASSERT_EQ(200u * SECOND, pon_mbox_cnt_oldest_update_time_get(stat, mask));

			//Clean up
			pon_mbox_cnt_autoupdate_destroy(cnt);
		}
	}
}
