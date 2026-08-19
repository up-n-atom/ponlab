/*
 * Copyright (C) 2020-2021 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR APARTICULARPURPOSE.See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public
 * License along with this program; if not,see
 * <http://www.gnu.org/licenses/>.
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: Cuckoo Hash test
 */

#include <linux/types.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/random.h>
#include <linux/cuckoo_hash.h>

#ifdef CONFIG_CUCKOO_HASH_DEBUG
#define TEST_DBG_HIST_SZ      (16)
#define BENCH_ITER_SHIFT      (5)
/*
 * Be carful with this number of iterations
 * Since each iteration should take some time
 */
#define BENCH_NUM_ITER        (1 << BENCH_ITER_SHIFT)
#define BENCH_NUM_BITS        (12)
#define BENCH_BKT_SIZE        (8)
#define BENCH_MAX_RPLC        (2)
#define BENCH_COMP_HASH       true
#define BENCH_LOCK_INT        true
#define BENCH_LU_ON_INSR      false
#define BENCH_NUM_ITEMS       ((2 << BENCH_NUM_BITS) * BENCH_BKT_SIZE)
#define BENCH_FOREACH_ITER(i) for ((i) = 0; (i) < BENCH_NUM_ITER; (i)++)
#endif /* CONFIG_CUCKOO_HASH_DEBUG */

#define RPLC_TEST_NUM_BITS    (4)
#define RPLC_TEST_BKT_SIZE    (1)
#define RPLC_TEST_MAX_RPLC    (4)
#define RPLC_TEST_COMP_HASH   false
#define RPLC_TEST_LOCK_INT    true
#define RPLC_TEST_LU_ON_INSR  false

static void __replace_test_event_handler(const struct cuckoo_hash_action *event)
{
	switch (event->type) {
	case ITEM_ADDED:
		pr_debug("CUCKOO HASH TEST: ITEM_ADDED EVENT:   new_table %hhu new_slot %hhu\n",
			 event->new_tbl, event->new_slot);
		break;
	case ITEM_MOVED:
		pr_debug("CUCKOO HASH TEST: ITEM_MOVED EVENT:   new_table %hhu new_slot %hhu old_table %hhu old_slot %hhu\n",
			 event->new_tbl, event->new_slot,
			 event->old_tbl, event->old_slot);
		break;
	case ITEM_REMOVED:
		pr_debug("CUCKOO HASH TEST: ITEM_REMOVED EVENT: old_table %hhu old_slot %hhu\n",
			 event->old_tbl, event->old_slot);
		break;
	default:
		pr_debug("CUCKOO HASH TEST: INVALID EVENT TYPE %d\n",
			 event->type);
		break;
	}
}

/**
 * @brief test the cuckoo hash by inserting a static items to
 *        create two replacements and verify all items was
 *        inserted successfully
 */
static void __cuckoo_hash_replace_test(void)
{
	struct cuckoo_hash_param param;
	void *hash;
	u32 key;
	struct cuckoo_hash_item item;

	pr_info("CUCKOO HASH TEST: Start replacement test...\n");
	param.hash_bits    = RPLC_TEST_NUM_BITS;
	param.bucket_size  = RPLC_TEST_BKT_SIZE;
	param.max_replace  = RPLC_TEST_MAX_RPLC;
	param.lock_enable  = RPLC_TEST_LOCK_INT;
	param.hash_compute = RPLC_TEST_COMP_HASH;
	param.lu_on_insert = RPLC_TEST_LU_ON_INSR;
#ifdef CONFIG_CUCKOO_HASH_DEBUG
	param.dbg_hist_sz  = TEST_DBG_HIST_SZ;
#endif /* CONFIG_CUCKOO_HASH_DEBUG */
	param.event_cb     = __replace_test_event_handler;
	param.keycmp_cb    = NULL;

	hash = cuckoo_hash_create(&param);
	if (unlikely(!hash)) {
		pr_err("CUCKOO HASH TEST ERROR: replacement test failed to create cuckoo hash\n");
		goto error;
	}

	key          = 5555;
	item.value   = &key;
	item.key     = &key;
	item.key_len = sizeof(key);

	item.h1      = 0;
	item.h2      = 1;
	if (cuckoo_hash_insert(hash, &item)) {
		pr_err("CUCKOO HASH TEST ERROR: replacement test failed to insert item[0]\n");
		goto error_destroy;
	}
	item.h1      = 3;
	item.h2      = 4;
	key--;
	if (cuckoo_hash_insert(hash, &item)) {
		pr_err("CUCKOO HASH TEST ERROR: replacement test failed to insert item[1]\n");
		goto error_destroy;
	}
	item.h1      = 2;
	item.h2      = 6;
	key--;
	if (cuckoo_hash_insert(hash, &item)) {
		pr_err("CUCKOO HASH TEST ERROR: replacement test failed to insert item[2]\n");
		goto error_destroy;
	}
	item.h1      = 2;
	item.h2      = 1;
	key--;
	if (cuckoo_hash_insert(hash, &item)) {
		pr_err("CUCKOO HASH TEST ERROR: replacement test failed to insert item[3]\n");
		goto error_destroy;
	}
	item.h1      = 3;
	item.h2      = 5;
	key--;
	if (cuckoo_hash_insert(hash, &item)) {
		pr_err("CUCKOO HASH TEST ERROR: replacement test failed to insert item[4]\n");
		goto error_destroy;
	}
	item.h1      = 0;
	item.h2      = 5;
	key--;
	if (cuckoo_hash_insert(hash, &item)) {
		pr_err("CUCKOO HASH TEST ERROR: replacement test failed to insert item[5]\n");
		goto error_destroy;
	}

	pr_info("CUCKOO HASH TEST: Replacement test flush tables\n");
	if (unlikely(cuckoo_hash_flush(hash))) {
		pr_err("CUCKOO HASH TEST ERROR: replacement test failed to flush tables\n");
		goto error_destroy;
	}

	if (unlikely(cuckoo_hash_destroy(hash))) {
		pr_err("CCUCKOO HASH TEST ERROR: replacement test failed to destroy cuckoo hash\n");
		goto error;
	}

	pr_info("CUCKOO HASH TEST: Replacement test done successfully\n");
	return;

error_destroy:
	if (unlikely(cuckoo_hash_destroy(hash)))
		pr_err("CUCKOO HASH TEST ERROR: replacement test failed to destroy cuckoo hash\n");

error:
	pr_err("CUCKOO HASH TEST ERROR: replacement test done with a failure\n");
}

static void __cuckoo_hash_benchmark_test(void)
{
	struct cuckoo_hash_param param;
	void *hash;
	u32 item_id, iter_id, key;
	struct cuckoo_hash_item item;
	u64 sum = 0;
	u32 minimum = U32_MAX;
	u32 maximum = 0;
	unsigned long jif;
	u32 *insert_fail = NULL;
	u32 *ins_time_25 = NULL;
	u32 *ins_time_50 = NULL;
	u32 *ins_time_75 = NULL;
	u32 *ins_time_100 = NULL;
#ifdef CONFIG_CUCKOO_HASH_DEBUG
	struct cuckoo_hash_stats stats;
	u32 *replacements = NULL;
	u32 *utilization = NULL;
#endif /* CONFIG_CUCKOO_HASH_DEBUG */

	pr_info("\n");
	pr_info("CUCKOO HASH BENCHMARK: Starting benchmark test:\n");
	pr_info("                       Benchmark running %u iterations of tests\n",
		BENCH_NUM_ITER);
	pr_info("                       Each test trying to insert %u items to cuckoo hash\n",
		BENCH_NUM_ITEMS);
	pr_info("                       Cuckoo hash database including %u slots:\n",
		BENCH_NUM_ITEMS);
	pr_info("                       2 tables of %u entries with bucket size %u\n",
		1 << BENCH_NUM_BITS, BENCH_BKT_SIZE);
	pr_info("                       Maximum replacements allowed %u\n",
		BENCH_MAX_RPLC);
	pr_info("                       Hash functions are computed internally: %s\n",
		BENCH_COMP_HASH ? "YES" : "NO");
	pr_info("\n");
	pr_info("CUCKOO HASH BENCHMARK: please wait for benchmark results...\n");

	param.hash_bits    = BENCH_NUM_BITS;
	param.bucket_size  = BENCH_BKT_SIZE;
	param.max_replace  = BENCH_MAX_RPLC;
	param.lock_enable  = BENCH_LOCK_INT;
	param.hash_compute = BENCH_COMP_HASH;
	param.lu_on_insert = BENCH_LU_ON_INSR;
	param.event_cb     = NULL;
	param.keycmp_cb    = NULL;
#ifdef CONFIG_CUCKOO_HASH_DEBUG
	param.dbg_hist_sz  = TEST_DBG_HIST_SZ;

	replacements = kcalloc(BENCH_NUM_ITER, sizeof(*replacements),
			       GFP_KERNEL);
	if (unlikely(!replacements)) {
		pr_err("CUCKOO HASH BENCHMARK ERROR: failed to allocate memory\n");
		goto error;
	}
	utilization = kcalloc(BENCH_NUM_ITER, sizeof(*utilization),
			      GFP_KERNEL);
	if (unlikely(!utilization)) {
		pr_err("CUCKOO HASH BENCHMARK ERROR: failed to allocate memory\n");
		goto error;
	}
#endif /* CONFIG_CUCKOO_HASH_DEBUG */
	insert_fail = kcalloc(BENCH_NUM_ITER, sizeof(*insert_fail),
			      GFP_KERNEL);
	if (unlikely(!insert_fail)) {
		pr_err("CUCKOO HASH BENCHMARK ERROR: failed to allocate memory\n");
		goto error;
	}

	ins_time_25 = kcalloc(BENCH_NUM_ITER, sizeof(*ins_time_25),
			      GFP_KERNEL);
	if (unlikely(!ins_time_25)) {
		pr_err("CUCKOO HASH BENCHMARK ERROR: failed to allocate memory\n");
		goto error;
	}
	ins_time_50 = kcalloc(BENCH_NUM_ITER, sizeof(*ins_time_50),
			      GFP_KERNEL);
	if (unlikely(!ins_time_50)) {
		pr_err("CUCKOO HASH BENCHMARK ERROR: failed to allocate memory\n");
		goto error;
	}
	ins_time_75 = kcalloc(BENCH_NUM_ITER, sizeof(*ins_time_75),
			      GFP_KERNEL);
	if (unlikely(!ins_time_75)) {
		pr_err("CUCKOO HASH BENCHMARK ERROR: failed to allocate memory\n");
		goto error;
	}
	ins_time_100 = kcalloc(BENCH_NUM_ITER, sizeof(*ins_time_100),
			       GFP_KERNEL);
	if (unlikely(!ins_time_100)) {
		pr_err("CUCKOO HASH BENCHMARK ERROR: failed to allocate memory\n");
		goto error;
	}

	hash = cuckoo_hash_create(&param);
	if (unlikely(!hash)) {
		pr_err("CUCKOO HASH BENCHMARK ERROR: failed to create cuckoo hash\n");
		goto error;
	}

	/* Reset counters */
	BENCH_FOREACH_ITER(iter_id) {
		ins_time_25[iter_id]  = 0;
		ins_time_50[iter_id]  = 0;
		ins_time_75[iter_id]  = 0;
		ins_time_100[iter_id] = 0;
		insert_fail[iter_id]  = 100;
#ifdef CONFIG_CUCKOO_HASH_DEBUG
		utilization[iter_id]  = 100;
		replacements[iter_id] = 0;
#endif /* CONFIG_CUCKOO_HASH_DEBUG */
	}

	item.key     = &key;
	item.key_len = sizeof(key);
	item.value   = &item_id;

	BENCH_FOREACH_ITER(iter_id) {
		jif = jiffies;
		for (item_id = 0; item_id < BENCH_NUM_ITEMS; item_id++) {

			key = prandom_u32();

			if ((cuckoo_hash_insert(hash, &item)) &&
			    (insert_fail[iter_id] == 100))
				insert_fail[iter_id] =
					item_id * 100 / BENCH_NUM_ITEMS;

			switch (item_id) {
			case BENCH_NUM_ITEMS >> 2:/* After 25% */
				ins_time_25[iter_id] = jiffies - jif;
				jif = jiffies;
				break;
			case BENCH_NUM_ITEMS >> 1:/* After 50% */
				ins_time_50[iter_id] = jiffies - jif;
				jif = jiffies;
				break;
			case (BENCH_NUM_ITEMS >> 2) * 3:/* After 75% */
				ins_time_75[iter_id] = jiffies - jif;
				jif = jiffies;
				break;
			case BENCH_NUM_ITEMS - 1:/* After 100% */
				ins_time_100[iter_id] = jiffies - jif;
				break;
			default:
				break;
			}
		}

#ifdef CONFIG_CUCKOO_HASH_DEBUG
		if (unlikely(cuckoo_hash_stats_get(hash, &stats))) {
			pr_err("CUCKOO HASH BENCHMARK ERROR: failed to get iter[%u] statistics\n",
			       iter_id);
			goto error_destroy;
		}
		replacements[iter_id] = stats.ins_done ?
			(stats.rplc_total * 100 / stats.ins_done) : 100;
		utilization[iter_id] = stats.ins_req ?
			(stats.ins_done * 100 / stats.ins_req) : 0;
#endif /* CONFIG_CUCKOO_HASH_DEBUG */

		if (unlikely(cuckoo_hash_flush(hash))) {
			pr_err("CUCKOO HASH BENCHMARK ERROR: failed to flush cuckoo hash iter[%u]\n",
			       iter_id);
			goto error_destroy;
		}
	}

	/* Analyze results */
	pr_info("\n");
	pr_info("+-------------------------------------------------------------+\n");
	pr_info("|               Cuckoo Hash Benchmark Results                 |\n");
	pr_info("+-------------------------------------------------------------+\n");
	pr_info("|                               |   AVG   |   MAX   |   MIN   |\n");
	pr_info("+===============================+=========+=========+=========+\n");
#ifdef CONFIG_CUCKOO_HASH_DEBUG
	BENCH_FOREACH_ITER(iter_id) {
		sum += utilization[iter_id];
		minimum = min(utilization[iter_id], minimum);
		maximum = max(utilization[iter_id], maximum);
	}
	pr_info("| %-30s| %-8llu| %-8u| %-8u|\n", "Utilization [\%]",
		sum >> BENCH_ITER_SHIFT, maximum, minimum);
	pr_info("+-------------------------------+---------+---------+---------+\n");
	sum = 0;
	minimum = U32_MAX;
	maximum = 0;
	BENCH_FOREACH_ITER(iter_id) {
		sum += replacements[iter_id];
		minimum = min(replacements[iter_id], minimum);
		maximum = max(replacements[iter_id], maximum);
	}
	pr_info("| %-30s| %-8llu| %-8u| %-8u|\n", "Replacements [\%]",
		sum >> BENCH_ITER_SHIFT, maximum, minimum);
	pr_info("+-------------------------------+---------+---------+---------+\n");
#endif /* CONFIG_CUCKOO_HASH_DEBUG */
	sum = 0;
	minimum = U32_MAX;
	maximum = 0;
	BENCH_FOREACH_ITER(iter_id) {
		sum += insert_fail[iter_id];
		minimum = min(insert_fail[iter_id], minimum);
		maximum = max(insert_fail[iter_id], maximum);
	}
	pr_info("| %-30s| %-8llu| %-8u| %-8u|\n",
		"First 'table full' after [\%]",
		sum >> BENCH_ITER_SHIFT, maximum, minimum);
	pr_info("+-------------------------------+---------+---------+---------+\n");
	sum = 0;
	minimum = U32_MAX;
	maximum = 0;
	BENCH_FOREACH_ITER(iter_id) {
		sum += ins_time_25[iter_id];
		minimum = min(ins_time_25[iter_id], minimum);
		maximum = max(ins_time_25[iter_id], maximum);
	}
	pr_info("| %-30s| %-8llu| %-8llu| %-8llu|\n",
		"Insert time 00\%..25\%  [nSEC]",
		jiffies_to_nsecs(sum >> BENCH_ITER_SHIFT) /
		(BENCH_NUM_ITEMS >> 2),
		jiffies_to_nsecs(maximum) / (BENCH_NUM_ITEMS >> 2),
		jiffies_to_nsecs(minimum) / (BENCH_NUM_ITEMS >> 2));
	pr_info("+-------------------------------+---------+---------+---------+\n");
	sum = 0;
	minimum = U32_MAX;
	maximum = 0;
	BENCH_FOREACH_ITER(iter_id) {
		sum += ins_time_50[iter_id];
		minimum = min(ins_time_50[iter_id], minimum);
		maximum = max(ins_time_50[iter_id], maximum);
	}
	pr_info("| %-30s| %-8llu| %-8llu| %-8llu|\n",
		"Insert time 25\%..50\%  [nSEC]",
		jiffies_to_nsecs(sum >> BENCH_ITER_SHIFT) /
		(BENCH_NUM_ITEMS >> 2),
		jiffies_to_nsecs(maximum) / (BENCH_NUM_ITEMS >> 2),
		jiffies_to_nsecs(minimum) / (BENCH_NUM_ITEMS >> 2));
	pr_info("+-------------------------------+---------+---------+---------+\n");
	sum = 0;
	minimum = U32_MAX;
	maximum = 0;
	BENCH_FOREACH_ITER(iter_id) {
		sum += ins_time_75[iter_id];
		minimum = min(ins_time_75[iter_id], minimum);
		maximum = max(ins_time_75[iter_id], maximum);
	}
	pr_info("| %-30s| %-8llu| %-8llu| %-8llu|\n",
		"Insert time 50\%..75\%  [nSEC]",
		jiffies_to_nsecs(sum >> BENCH_ITER_SHIFT) /
		(BENCH_NUM_ITEMS >> 2),
		jiffies_to_nsecs(maximum) / (BENCH_NUM_ITEMS >> 2),
		jiffies_to_nsecs(minimum) / (BENCH_NUM_ITEMS >> 2));
	pr_info("+-------------------------------+---------+---------+---------+\n");
	sum = 0;
	minimum = U32_MAX;
	maximum = 0;
	BENCH_FOREACH_ITER(iter_id) {
		sum += ins_time_100[iter_id];
		minimum = min(ins_time_100[iter_id], minimum);
		maximum = max(ins_time_100[iter_id], maximum);
	}
	pr_info("| %-30s| %-8llu| %-8llu| %-8llu|\n",
		"Insert time 75\%..100\% [nSEC]",
		jiffies_to_nsecs(sum >> BENCH_ITER_SHIFT) /
		(BENCH_NUM_ITEMS >> 2),
		jiffies_to_nsecs(maximum) / (BENCH_NUM_ITEMS >> 2),
		jiffies_to_nsecs(minimum) / (BENCH_NUM_ITEMS >> 2));
	pr_info("+-------------------------------+---------+---------+---------+\n");
	pr_info("\n");

	if (unlikely(cuckoo_hash_destroy(hash))) {
		pr_err("CUCKOO HASH BENCHMARK ERROR: failed to destroy cuckoo hash\n");
		goto error;
	}

	pr_info("CUCKOO HASH BENCHMARK: done successfully\n");
	return;

error_destroy:
	if (unlikely(cuckoo_hash_destroy(hash)))
		pr_err("CUCKOO HASH BENCHMARK ERROR: failed to destroy cuckoo hash\n");

error:
#ifdef CONFIG_CUCKOO_HASH_DEBUG
	kfree(utilization);
	kfree(replacements);
#endif /* CONFIG_CUCKOO_HASH_DEBUG */
	kfree(insert_fail);
	kfree(ins_time_25);
	kfree(ins_time_50);
	kfree(ins_time_75);
	kfree(ins_time_100);
	pr_err("CUCKOO HASH BENCHMARK ERROR: done with a failure\n");
}

static int __init cuckoo_hash_test_init(void)
{
	pr_info("CUCKOO HASH TEST\n");

	__cuckoo_hash_replace_test();
	__cuckoo_hash_benchmark_test();

	pr_info("CUCKOO HASH TEST END\n");

	return -EAGAIN; /* Fail will directly unload the module */
}

static void __exit cuckoo_hash_test_exit(void)
{
	pr_info("CUCKOO HASH TEST EXIT\n");
}

module_init(cuckoo_hash_test_init)
module_exit(cuckoo_hash_test_exit)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Elad Fernandes");
MODULE_DESCRIPTION("Cuckoo Hash Test");
