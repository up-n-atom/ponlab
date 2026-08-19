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
 * Description: dpl logic internal h file
 */

#ifndef __PP_DPL_LOGIC_INTERNAL_H__
#define __PP_DPL_LOGIC_INTERNAL_H__

#include <linux/types.h>

/* max entries allowed in bithash relative to num entries */
#define DPL_LOGIC_MAX_USED_HASH_ENTRIES(entries) ((entries) / 10)

/**
 * @brief get num of max bit hash entries
 */
unsigned int dpl_logic_num_entries_get(void);

/**
 * @brief get hash bit and timestamp
 * @param bmap array of bmap to copy the data to
 * @param time array of timestamp to copy the data to
 * @param num bmap entries number
 * @return 0 on success, error code otherwise
 */
int dpl_logic_hash_bit_get(unsigned long *bmap, unsigned long long *timestamp,
			   unsigned int num);

/**
 * @brief enable ind in hash bit table
 * @param ind ind to enable
 * @return 0 on success, error code otherwise
 */
int dpl_logic_hash_bit_ind_enable(unsigned int ind);

/**
 * @brief disable ind in hash bit table
 * @param ind ind to disable
 * @return 0 on success, error code otherwise
 */
int dpl_logic_hash_bit_ind_disable(unsigned int ind);

/**
 * @brief reset all entreis in bit ahsh table
 * @return 0 on success, error code otherwise
 */
int dpl_logic_hash_bit_reset(void);

/**
 * @brief get the dpl logic status
 * @return true if it is active, false otherwise
 */
bool dpl_logic_is_enable(void);

/**
 * @brief get timeout for old hash entries
 * @param tout ptr to store the timeout value
 */
void dpl_logic_old_entry_to_get(unsigned int *tout);

/**
 * @brief set timeout for old hash entries
 * @param tout timeout value to set
 */
void dpl_logic_old_entry_to_set(unsigned int tout);

/**
 * @brief get cleaner interval
 * @param tout ptr to store the interval value
 */
void dpl_logic_cleaner_interval_get(unsigned int *tout);

/**
 * @brief set cleaner interval
 * @param tout interval value to set
 */
void dpl_logic_cleaner_interval_set(unsigned int tout);

#ifdef CONFIG_DEBUG_FS
int dpl_logic_dbg_init(struct dentry *parent_dir);
int dpl_logic_dbg_clean(void);
#else /* !CONFIG_DEBUG_FS */
inline int dpl_logic_dbg_init(struct dentry *parent_dir)
{
	return 0;
}
inline int dpl_logic_dbg_clean(void)
{
	return 0;
}
#endif /* CONFIG_DEBUG_FS */
#endif /* __PP_DPL_LOGIC_INTERNAL_H__ */