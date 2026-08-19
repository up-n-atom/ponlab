/*
 * Copyright (C) 2023 MaxLinear, Inc.
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
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: MXL SKB Extension
 */

#define pr_fmt(fmt) "[MXL_SKB_EXT]:%s:%d: " fmt, __func__, __LINE__

#include <linux/skbuff.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/bitops.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <linux/bitmap.h>
#include <soc/mxl/mxl_skb_ext.h>

int mxl_skb_ext_dbg_init(void);
void mxl_skb_ext_dbg_clean(void);

#define DATA_EXTS_SIZE (SKB_EXT_ARCH_EXT_SIZE - sizeof(mxl_skb_ext_bmap))
#define EXT_NAME_SIZE                      (32)

#define IS_EXT_ID_VALID(id) ((id) >= 0 && (id) < MXL_SKB_EXTS_MAX)

#ifdef CONFIG_DEBUG_FS
#include <linux/atomic.h>
#define STATS_INC_DEBUG(ctr) atomic_inc(ctr)

/**
 * @struct mxl_ext_id_stats
 * @brief stats per ext_id
 */
struct mxl_ext_id_stats {
	atomic_t added;
	atomic_t deleted;
	atomic_t find_id_by_name; /* user find his ext_id by name */
	atomic_t find_found; /* user used find API, and find his ext */
	atomic_t find_n_found; /* user used find API, and didn't find his ext */
};

/**
 * @struct mxl_ext_id_stats
 * @brief stats for gernel stats, for all users
 */
struct mxl_ext_stats {
	atomic_t error_reg; /* user register falied */
	atomic_t error_inactive_id; /* used inactive ext_id */
};
#else
#define STATS_INC_DEBUG(ctr)
#endif /* CONFIG_DEBUG_FS */

struct mxl_skb_ext {
	mxl_skb_ext_bmap active_bmap;
	unsigned char data[DATA_EXTS_SIZE];
};

struct mxl_ext_id {
	char name[EXT_NAME_SIZE]; /* ext_id's name */
	unsigned short offset; /* offset in bytes from mxl_skb_ext.data */
	unsigned short size; /* size of the extension */
};

struct mxl_skb_ext_db {
	struct mxl_ext_id ext_id[MXL_SKB_EXTS_MAX];
	mxl_skb_ext_bmap ext_ids_bmap;
	unsigned short used_data;
	spinlock_t lock;
#ifdef CONFIG_DEBUG_FS
	struct mxl_ext_id_stats ext_id_stats[MXL_SKB_EXTS_MAX];
	struct mxl_ext_stats stats;
#endif
};

static struct mxl_skb_ext_db *db = NULL;

static inline int __is_ext_id_active(unsigned int ext_id)
{
	return (IS_EXT_ID_VALID(ext_id) && test_bit(ext_id, &db->ext_ids_bmap));
}

static inline int is_ext_id_active(unsigned int ext_id)
{
	int active;

	active = __is_ext_id_active(ext_id);
	if (!active) {
		pr_err("user %d is not active\n", ext_id);
		STATS_INC_DEBUG(&db->stats.error_inactive_id);
	}

	return active;
}

/**
 * @brief translate from mxl_skb_ext extension to ext_id's extension
 */
static inline void *get_ext(struct mxl_skb_ext *ext, unsigned int id)
{
	return ext->data + db->ext_id[id].offset;
}

static inline int is_ext_exist(struct mxl_skb_ext *ext, unsigned int id)
{
	return test_bit(id, &ext->active_bmap);
}

/**
 * @brief create mxl_skb_ext extension on the SKB, if already exist, returns it
 */
static struct mxl_skb_ext *mxl_ext_add(struct sk_buff *skb)
{
	struct mxl_skb_ext *mxl_ext;
	bool is_old = skb_ext_exist(skb, SKB_EXT_ARCH);

	mxl_ext = skb_ext_add(skb, SKB_EXT_ARCH);
	if (!mxl_ext) {
		pr_err("can't alloc memory for extension\n");
		return mxl_ext;
	}

	if (!is_old)
		mxl_ext->active_bmap = 0;

	return mxl_ext;
}

static struct mxl_skb_ext *mxl_ext_find(const struct sk_buff *skb)
{
	return (struct mxl_skb_ext *)skb_ext_find(skb, SKB_EXT_ARCH);
}

unsigned int mxl_skb_ext_register(const char *name, unsigned short size)
{
	unsigned int id;

	if (!name || !db)
		return MXL_SKB_EXT_INVALID;

	spin_lock_bh(&db->lock);
	/* check if another user with same name already exists */
	if (mxl_skb_ext_get_ext_id(name) != MXL_SKB_EXT_INVALID) {
		pr_err("Failed to allocate user %s, name already taken\n", name);
		goto err;
	}

	/* find the first unused ext_id */
	id = bitmap_weight(&db->ext_ids_bmap, MXL_SKB_EXTS_MAX);
	if (id == MXL_SKB_EXTS_MAX) {
		pr_err("Failed to allocate user %s, no free ids\n", name);
		goto err;
	}

	/* make sure there is enough space in data's extensions */
	if (size + db->used_data >= DATA_EXTS_SIZE) {
		pr_err("Failed to allocate user %s, no memory\n", name);
		goto err;
	}

	/* set ext_id's fileds */
	db->ext_id[id].offset = db->used_data;
	db->ext_id[id].size = size;
	strncpy(db->ext_id[id].name, name, EXT_NAME_SIZE);

	/* allocate ext_id */
	db->used_data += size;
	set_bit(id, &db->ext_ids_bmap);

	spin_unlock_bh(&db->lock);

	return id;
err:
	spin_unlock_bh(&db->lock);
	STATS_INC_DEBUG(&db->stats.error_reg);
	return MXL_SKB_EXT_INVALID;
}
EXPORT_SYMBOL(mxl_skb_ext_register);

unsigned int mxl_skb_ext_get_ext_id(const char *name)
{
	unsigned int i;

	if (!db)
		return MXL_SKB_EXT_INVALID;

	for_each_set_bit(i, &db->ext_ids_bmap, MXL_SKB_EXTS_MAX) {
		if (!strncmp(db->ext_id[i].name, name, EXT_NAME_SIZE)) {
			STATS_INC_DEBUG(&db->ext_id_stats[i].find_id_by_name);
			return i;
		}
	}

	return MXL_SKB_EXT_INVALID;
}
EXPORT_SYMBOL(mxl_skb_ext_get_ext_id);

void *mxl_skb_ext_add(struct sk_buff *skb, unsigned int ext_id)
{
	struct mxl_skb_ext *mxl_ext;

	if (!db || !skb || !is_ext_id_active(ext_id))
		return NULL;
	
	mxl_ext = mxl_ext_add(skb);
	if (!mxl_ext) 
		return NULL;

	set_bit(ext_id, &mxl_ext->active_bmap);
	STATS_INC_DEBUG(&db->ext_id_stats[ext_id].added);

	return get_ext(mxl_ext, ext_id);
}
EXPORT_SYMBOL(mxl_skb_ext_add);

void *mxl_skb_ext_find(const struct sk_buff *skb, unsigned int ext_id)
{
	struct mxl_skb_ext *mxl_ext;

	if (!skb || !is_ext_id_active(ext_id))
		return NULL;

	mxl_ext = mxl_ext_find(skb);
	if (!mxl_ext || !is_ext_exist(mxl_ext, ext_id)) {
		STATS_INC_DEBUG(&db->ext_id_stats[ext_id].find_n_found);
		return NULL;
	}

	STATS_INC_DEBUG(&db->ext_id_stats[ext_id].find_found);
	return get_ext(mxl_ext, ext_id);
}
EXPORT_SYMBOL(mxl_skb_ext_find);

void mxl_skb_ext_del(const struct sk_buff *skb, unsigned int ext_id)
{
	struct mxl_skb_ext *mxl_ext;

	if (!skb || !is_ext_id_active(ext_id))
		return;

	mxl_ext = mxl_ext_find(skb);
	if (!mxl_ext)
		return;

	STATS_INC_DEBUG(&db->ext_id_stats[ext_id].deleted);
	clear_bit(ext_id, &mxl_ext->active_bmap);
}
EXPORT_SYMBOL(mxl_skb_ext_del);

/**
 * @brief copy active ext_ids to ext_ids array
 * @param ext_ids array to copy the data to
 * @return number of active ext_ids
 * @note used by debugfs
 */
unsigned int get_ext_ids_info(struct mxl_ext_id ext_ids[MXL_SKB_EXTS_MAX])
{
	unsigned int num;

	if (!db || !ext_ids)
		return 0;

	/* get the number of active ext_ids */
	num = bitmap_weight(&db->ext_ids_bmap, MXL_SKB_EXTS_MAX);
	/* copy all the info of the active ext_ids */
	memcpy(ext_ids, db->ext_id, num * sizeof(*ext_ids));

	return num;
}

static int __init mxl_skb_ext_init(void)
{
	pr_debug("init mxl_skb_ext\n");

	db = kzalloc(sizeof(*db), GFP_KERNEL);
	if (!db) {
		pr_err("can't alloc mxl_skb_ext db\n");
		return -ENOMEM;
	}

	spin_lock_init(&db->lock);
	mxl_skb_ext_dbg_init();

	return 0;
}
arch_initcall(mxl_skb_ext_init);

static void __exit mxl_skb_ext_exit(void)
{
	pr_debug("exit mxl_skb_ext\n");
	kfree(db);
	db = NULL;

	mxl_skb_ext_dbg_clean();
}
__exitcall(mxl_skb_ext_exit);

#ifdef CONFIG_DEBUG_FS
#include <linux/debugfs.h>

static struct dentry *dir;

static inline void reset_stats(void)
{
	if (!db)
		return;

	memset(&db->stats, 0, sizeof(db->stats));
	memset(db->ext_id_stats, 0,
	       MXL_SKB_EXTS_MAX * sizeof(*db->ext_id_stats));
}

/**
 * @brief prints all the extensions id's
 */
static int get_ext_ids(struct seq_file *f, void *unused)
{
	unsigned int num, i;

	struct mxl_ext_id *ext_ids = kzalloc(
		MXL_SKB_EXTS_MAX * sizeof(struct mxl_ext_id), GFP_ATOMIC);
	if (!ext_ids) {
		seq_puts(f, "failed to alloc memroy\n");
		return -1;
	}

	num = get_ext_ids_info(ext_ids);

	seq_puts(f,
		 "+----+----------------------------------+------+--------+\n");
	seq_printf(f, "| %-2s | %-32s | %-4s | %-6s |\n", "id", "name", "size",
		   "offset");
	seq_puts(f,
		 "+----+----------------------------------+------+--------+\n");
	for (i = 0; i < num; i++, ext_ids++) {
		seq_printf(f, "| %-2u | %-32s | %-4u | %-6u |\n", i,
			   ext_ids->name, ext_ids->size, ext_ids->offset);
		seq_puts(
			f,
			"+----+----------------------------------+------+--------+\n");
	}

	kfree(ext_ids);
	return 0;
}

/**
 * @brief prints stats
 */
static int get_stats(struct seq_file *f, void *unused)
{
	int num_ids, i;

	if (!db)
		return -1;

	seq_printf(f,
		   "register failed error: %d\ninactive id error: %d\n\n",
		   atomic_read(&db->stats.error_reg),
		   atomic_read(&db->stats.error_inactive_id));

	/* get the number of active ext_ids */
	num_ids = bitmap_weight(&db->ext_ids_bmap, MXL_SKB_EXTS_MAX);

	seq_puts(
		f,
		"+----+-----------------+-----------+-----------+-----------+-----------+\n");
	seq_printf(f, "| %-2s | %-15s | %-9s | %-9s | %-9s | %-9s |\n", "id",
		   "find id by name", "added", "deleted", "found", "not found");
	seq_puts(
		f,
		"+----+-----------------+-----------+-----------+-----------+-----------+\n");
	for (i = 0; i < num_ids; i++) {
		seq_printf(f, "| %-2u | %-15d | %-9d | %-9d | %-9d | %-9d |\n",
			   i, atomic_read(&db->ext_id_stats[i].find_id_by_name),
			   atomic_read(&db->ext_id_stats[i].added),
			   atomic_read(&db->ext_id_stats[i].deleted),
			   atomic_read(&db->ext_id_stats[i].find_found),
			   atomic_read(&db->ext_id_stats[i].find_n_found));
	}
	seq_puts(
		f,
		"+----+-----------------+-----------+-----------+-----------+-----------+\n\n");
	
	return 0;
}

static int dbg_get_ext_open(struct inode *inode, struct file *file)
{
	return single_open(file, get_ext_ids, NULL);
}

static const struct file_operations dbg_ext_fops = {
	.open = dbg_get_ext_open,
	.read = seq_read,
	.release = single_release,
};

static int dbg_get_stats_open(struct inode *inode, struct file *file)
{
	return single_open(file, get_stats, NULL);
}

static const struct file_operations dbg_stats_fops = {
	.open = dbg_get_stats_open,
	.read = seq_read,
	.release = single_release,
};

int __init mxl_skb_ext_dbg_init(void)
{
	struct dentry *dent;

	dir = debugfs_create_dir("mxl_skb_ext", NULL);

	dent = debugfs_create_file("extensions", 0400, dir, NULL, &dbg_ext_fops);
	if (!dent)
		goto fail;

	dent = debugfs_create_file("stats", 0400, dir, NULL, &dbg_stats_fops);
	if (!dent)
		goto fail;

	reset_stats();

	return 0;
fail:
	pr_err("mxl_skb_ext debugfs_create_file failed\n");
	debugfs_remove_recursive(dir);
	return -ENOENT;
}

void mxl_skb_ext_dbg_clean(void)
{
	debugfs_remove_recursive(dir);
	dir = NULL;
}
#else
int __init mxl_skb_ext_dbg_init(void)
{
	return 0;
}
void mxl_skb_ext_dbg_clean(void)
{
}
#endif /* CONFIG_DEBUG_FS */
