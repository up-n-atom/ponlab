/*
 * Copyright (C) 2023-2025 MaxLinear, Inc.
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
 * Description: dpl logic
 */

#define pr_fmt(fmt) "dpl_logic: %s:%d: " fmt, __func__, __LINE__

#include <linux/netfilter.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>
#include <linux/bitmap.h>
#include <linux/pktprs.h>
#include "dpl_logic_internal.h"
#include "dpl_logic.h"

/* time in ms to trigger callback to clean old entries */
#define DPL_LOGIC_CLEANER_DELAY_MS (250)
/* old entry time in ms */
#define DPL_LOGIC_OLD_ENTRY_MS     (750)

struct dpl_logic_db {
	struct dpl_hash_bit_cb cb;
	bool is_hash_bit_active;
	unsigned int old_entry_to; /* ms to clean the old entry */
	unsigned int cleaner_interval; /* ms delay between each clean */
	unsigned int num_hash_bit_entries;
	unsigned long *hash_bit;
	unsigned long long *timestamp; /* relevant only if bit[ind] is set */
	struct workqueue_struct *workq;
	struct delayed_work dwork;
	spinlock_t lock;
};

static struct dpl_logic_db *db;

static inline bool is_old_entry(unsigned int ind, unsigned long long curr_time)
{
	unsigned long long diff = curr_time - db->timestamp[ind];

	return (jiffies_to_msecs(diff) > db->old_entry_to);
}

static inline bool is_ind_valid(unsigned int ind)
{
	if (ind < db->num_hash_bit_entries)
		return true;

	pr_err("invalid ind, ind must be positive < %u\n",
	       db->num_hash_bit_entries);

	return false;
}

static unsigned int get_correct_hdr_lvl(struct pktprs_hdr *h)
{
	if (PKTPRS_IS_MULTI_IP(h)) {
		if (!(PKTPRS_IS_ESP(h, PKTPRS_HDR_LEVEL0)))
			return PKTPRS_HDR_LEVEL1; /* inner */
	}

	return PKTPRS_HDR_LEVEL0;
}

unsigned int dpl_logic_num_entries_get(void)
{
	if (!db)
		return 0;

	return db->num_hash_bit_entries;
}

int dpl_logic_hash_bit_get(unsigned long *bmap, unsigned long long *timestamp,
			   unsigned int num)
{
	if (!db || !bmap || !timestamp)
		return -EINVAL;

	num = min(num, db->num_hash_bit_entries);

	spin_lock_bh(&db->lock);

	bitmap_copy(bmap, db->hash_bit, num);
	memcpy(timestamp, db->timestamp, num * sizeof(*timestamp));

	spin_unlock_bh(&db->lock);

	return 0;
}

bool dpl_logic_is_enable(void)
{
	if (!db) {
		pr_err("dpl logic DB is not initialized\n");
		return false;
	}
	return db->is_hash_bit_active;
}

union ip {
	volatile struct iphdr v4;
	volatile struct ipv6hdr v6;
};

/**
 * @brief calculate hash_bit ind
 * @param h pktprs hdr
 * @param ind pointer to get the ind
 * @return true if bit hash support this header, false otherwise
 */
static bool calc_hash_bit_ind(struct pktprs_hdr *h, unsigned int *ind)
{
	unsigned int hash = 0xFFFFFFFF;
	unsigned short hash16 = 0xFFFF;
	unsigned int bit = 0;
	union ip *ip;
	unsigned int lvl = get_correct_hdr_lvl(h);

	if (PKTPRS_IS_IPV4(h, lvl)) {
		ip = (union ip *)pktprs_ipv4_hdr(h, lvl);
		hash = ip->v4.saddr ^ ip->v4.daddr;
		hash ^= ip->v4.protocol;
	} else if (PKTPRS_IS_IPV6(h, lvl)) {
		ip = (union ip *)pktprs_ipv6_hdr(h, lvl);
		hash = ip->v6.saddr.in6_u.u6_addr32[0] ^
		       ip->v6.saddr.in6_u.u6_addr32[1] ^
		       ip->v6.saddr.in6_u.u6_addr32[2] ^
		       ip->v6.saddr.in6_u.u6_addr32[3] ^
		       ip->v6.daddr.in6_u.u6_addr32[0] ^
		       ip->v6.daddr.in6_u.u6_addr32[1] ^
		       ip->v6.daddr.in6_u.u6_addr32[2] ^
		       ip->v6.daddr.in6_u.u6_addr32[3];
		hash ^= ip->v6.nexthdr;
	} else {
		return false;
	}

	if (PKTPRS_IS_TCP(h, lvl)) {
		struct tcphdr *tcp_hdr = pktprs_tcp_hdr(h, lvl);
		if (tcp_hdr->rst || tcp_hdr->fin)
			return false;
		hash ^= tcp_hdr->source ^ tcp_hdr->dest;
	} else if (PKTPRS_IS_UDP(h, lvl)) {
		struct udphdr *udphdr = pktprs_udp_hdr(h, lvl);
		hash ^= udphdr->source ^ udphdr->dest;
	} else if (PKTPRS_IS_ICMP(h, lvl)) {
		struct icmphdr *icmp = pktprs_icmp_hdr(h, lvl);
		if (icmp->type != ICMP_ECHO)
			return false;
		hash ^= icmp->un.echo.id;
	} else if (PKTPRS_IS_ICMP6(h, lvl)) {
		struct icmp6hdr *icmp6 = pktprs_icmp6_hdr(h, lvl);
		if (icmp6->icmp6_type != ICMPV6_ECHO_REQUEST)
			return false;
		hash ^= icmp6->icmp6_dataun.u_echo.identifier;
	} else {
		return false;
	}

	/* get bit between 0 to num of entries */
	hash16 = ((unsigned short)hash) ^ ((unsigned short)(hash >> 16));
	bit = hash16 % db->num_hash_bit_entries;

	*ind = bit;

	return true;
}

static inline bool is_there_free_ind_hash_bit(void)
{
	int used = bitmap_weight(db->hash_bit, db->num_hash_bit_entries);

	if (used >= DPL_LOGIC_MAX_USED_HASH_ENTRIES(db->num_hash_bit_entries))
		return false;
	return true;
}

static int _hash_bit_ind_enable(unsigned int ind, unsigned long long cur_time)
{
	int ret;

	db->timestamp[ind] = cur_time;

	if (test_bit(ind, db->hash_bit))
		return 0;

	if (!is_there_free_ind_hash_bit())
		return -ENOMEM;

	ret = db->cb.enable_ind(ind);
	if (!ret)
		set_bit(ind, db->hash_bit);

	return ret;
}

static int _hash_bit_ind_disable(unsigned int ind)
{
	int ret;

	if (!test_bit(ind, db->hash_bit))
		return 0;

	ret = db->cb.disable_ind(ind);
	if (!ret)
		clear_bit(ind, db->hash_bit);

	return ret;
}

int dpl_logic_hash_bit_ind_enable(unsigned int ind)
{
	int ret = 0;
	unsigned long long cur_time = get_jiffies_64();

	if (!db || !is_ind_valid(ind))
		return -EINVAL;

	spin_lock_bh(&db->lock);

	if (db->is_hash_bit_active)
		ret = _hash_bit_ind_enable(ind, cur_time);

	spin_unlock_bh(&db->lock);
	return ret;
}

int dpl_logic_hash_bit_ind_disable(unsigned int ind)
{
	int ret = 0;

	if (!db || !is_ind_valid(ind))
		return -EINVAL;

	spin_lock_bh(&db->lock);

	if (db->is_hash_bit_active)
		ret = _hash_bit_ind_disable(ind);

	spin_unlock_bh(&db->lock);
	return ret;
}

static int _hash_bit_reset(void)
{
	int ret;

	ret = db->cb.reset();
	if (!ret)
		bitmap_zero(db->hash_bit, db->num_hash_bit_entries);

	return ret;
}

int dpl_logic_hash_bit_reset(void)
{
	int ret;

	if (!db)
		return -EINVAL;

	spin_lock_bh(&db->lock);
	ret = _hash_bit_reset();
	spin_unlock_bh(&db->lock);

	return ret;
}

static void clean_old_entries_cb(struct work_struct *w)
{
	unsigned int i;
	unsigned long long curr_time;

	spin_lock_bh(&db->lock);

	if (!db->is_hash_bit_active)
		goto unlock;

	curr_time = get_jiffies_64();
	for_each_set_bit(i, db->hash_bit, db->num_hash_bit_entries) {
		if (is_old_entry(i, curr_time))
			_hash_bit_ind_disable(i);
	}

	queue_delayed_work(db->workq, &db->dwork,
			   msecs_to_jiffies(db->cleaner_interval));

unlock:
	spin_unlock_bh(&db->lock);
}

static int hash_bit_handler(struct pktprs_hdr *tx, struct pktprs_hdr *rx)
{
	unsigned int ind;

	if (!db->is_hash_bit_active)
		return 0;

	if (!db->cb.is_dev_protected(tx->ifindex))
		return 0;

	/* if the rx and tx are both protected devices, doing nothing */
	if (rx && db->cb.is_dev_protected(rx->ifindex))
		return 0;

	if (calc_hash_bit_ind(tx, &ind))
		dpl_logic_hash_bit_ind_enable(ind);

	return 0;
}

static int dpl_logic_tx_event(struct notifier_block *unused, unsigned long ev,
			      void *data)
{
	struct pktprs_desc *desc = (struct pktprs_desc *)data;

	if (!db || !desc || !desc->tx)
		return NOTIFY_DONE;

	hash_bit_handler(desc->tx, desc->rx);

	return NOTIFY_OK;
}

int dpl_logic_hash_bit_activate(void)
{
	if (!db)
		return -EPERM;

	spin_lock_bh(&db->lock);

	if (db->is_hash_bit_active)
		goto unlock;

	db->is_hash_bit_active = true;
	/* Trigger delayed work queue to clean old entries in hash_bit */
	queue_delayed_work(db->workq, &db->dwork,
			   msecs_to_jiffies(db->cleaner_interval));

unlock:
	spin_unlock_bh(&db->lock);

	return 0;
}

void dpl_logic_hash_bit_deactivate(void)
{
	if (!db)
		return;

	spin_lock_bh(&db->lock);

	if (!db->is_hash_bit_active)
		goto unlock;

	db->is_hash_bit_active = false;
	cancel_delayed_work(&db->dwork);
	_hash_bit_reset();

unlock:
	spin_unlock_bh(&db->lock);
}

void dpl_logic_old_entry_to_get(unsigned int *tout)
{
	if (!db || !tout) {
		pr_err("dpl logic DB is not initialized or tout is NULL\n");
		return;
	}
	*tout = db->old_entry_to;
}

void dpl_logic_old_entry_to_set(unsigned int tout)
{
	if (!db) {
		pr_err("dpl logic DB is not initialized\n");
		return;
	}
	db->old_entry_to = tout;
}

void dpl_logic_cleaner_interval_get(unsigned int *tout)
{
	if (!db || !tout) {
		pr_err("dpl logic DB is not initialized or tout is NULL\n");
		return;
	}

	*tout = db->cleaner_interval;
}

void dpl_logic_cleaner_interval_set(unsigned int tout)
{
	if (!db) {
		pr_err("dpl logic DB is not initialized\n");
		return;
	}
	db->cleaner_interval = tout;
}

static struct notifier_block notifier = {
	.notifier_call = dpl_logic_tx_event,
};

static int set_db_param(struct dpl_logic_param *p)
{
	unsigned int flags = WQ_UNBOUND;

	if (!p || !p->cb.is_dev_protected || !p->cb.enable_ind ||
	    !p->cb.disable_ind || !p->cb.reset)
		return -EINVAL;

	spin_lock_init(&db->lock);
	memcpy(&db->cb, &p->cb, sizeof(p->cb));
	db->num_hash_bit_entries = p->hash_bit_num_entries;

	db->hash_bit = bitmap_zalloc(db->num_hash_bit_entries, GFP_KERNEL);
	if (!db->hash_bit) {
		pr_err("failed to allocate dpl logic hash_bit\n");
		return -ENOMEM;
	}

	db->timestamp = kcalloc(db->num_hash_bit_entries,
				sizeof(*db->timestamp), GFP_KERNEL);
	if (!db->timestamp) {
		pr_err("failed to allocate dpl logic timestamp\n");
		goto free_bitmap;
	}

	db->workq = alloc_workqueue("%s", flags, 1, "DPL logic");
	if (!db->workq) {
		pr_err("failed to allocate workq for dpl logic\n");
		goto free_timestamp;
	}
	INIT_DELAYED_WORK(&db->dwork, clean_old_entries_cb);

	return 0;

free_timestamp:
	kfree(db->timestamp);
	db->timestamp = NULL;
free_bitmap:
	bitmap_free(db->hash_bit);
	db->hash_bit = NULL;

	return -ENOMEM;
}

int __init dpl_logic_init(struct dpl_logic_param *p, struct dentry *dir)
{
	int ret;

	db = kzalloc(sizeof(*db), GFP_KERNEL);
	if (!db) {
		pr_err("failed to allocate dpl_logic_db\n");
		return -ENOMEM;
	}

	ret = set_db_param(p);
	if (ret) {
		pr_err("failed to set dpl logic db\n");
		kfree(db);
		db = NULL;
		return ret;
	}

	db->cb.reset();
	if (pktprs_register(&notifier, PKTPRS_TX))
		pr_err("failed to register to pktprs\n");

	/* Initialize the delayed work for cleaning old entries */
	db->old_entry_to = DPL_LOGIC_OLD_ENTRY_MS;
	db->cleaner_interval = DPL_LOGIC_CLEANER_DELAY_MS;

	dpl_logic_dbg_init(dir);

	return 0;
}

void __exit dpl_logic_exit(void)
{
	pktprs_unregister(&notifier, PKTPRS_TX);

	if (!db)
		return;

	db->is_hash_bit_active = false;
	cancel_delayed_work_sync(&db->dwork);
	destroy_workqueue(db->workq);
	kfree(db->timestamp);
	bitmap_free(db->hash_bit);
	kfree(db);
	db = NULL;

	dpl_logic_dbg_clean();
}
