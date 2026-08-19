/*
 * Copyright (C) 2023 MaxLinear, Inc.
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
 * Description: dpl logic h file
 */

#ifndef _DPL_LOGIC_H_
#define _DPL_LOGIC_H_

#include <linux/types.h>
#include <linux/init.h>

/**
 * struct dpl_hash_bit_cb
 * @is_dev_protected: cb to get if dev (ifindex) is protected
 * @enable_ind: cb to set bit in hash_bit
 * @disable_ind: cb to reset bit in hash_bit
 * @reset: cb to reset all entries in hash_bit
 */
struct dpl_hash_bit_cb {
	bool (*is_dev_protected)(int);
	int (*enable_ind)(unsigned int);
	int (*disable_ind)(unsigned int);
	int (*reset)(void);
};

/**
 * struct dpl_logic_param - param to to dpl_logic
 * @cb: callbacks to reset/enable/disable index in bithash table
 * @hash_bit_num_entries: num enries in bithash
 */
struct dpl_logic_param {
	struct dpl_hash_bit_cb cb;
	unsigned int hash_bit_num_entries;
};

/**
 * @brief activate the bit hash
 * @return 0 on success, error code otherwise
 */
int dpl_logic_hash_bit_activate(void);

/**
 * @brief deactivate the bit hash
 */
void dpl_logic_hash_bit_deactivate(void);

/**
 * @brief init dpl logic module
 * @param param dpl logic param
 * @param dir pointer to parent dir
 * @return 0 on success, error code otherwise
 */
int __init dpl_logic_init(struct dpl_logic_param *param, struct dentry *dir);

/**
 * @brief exit dpl logic module
 */
void __exit dpl_logic_exit(void);

#endif /* _DPL_LOGIC_H_ */
