// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DATAPATH_PROC_H
#define DATAPATH_PROC_H

#include <linux/kernel.h>	/*kmalloc */
#include <linux/ctype.h>
#include <linux/debugfs.h>	/*file_operations */
#include <linux/seq_file.h>	/*seq_file */
#include <linux/uaccess.h>	/*copy_from_user */

#define set_ltq_dbg_flag(v, e, f) do {;\
	if ((e) > 0)\
		(v) |= (uint64_t)(f);\
	else\
		(v) &= (uint64_t)(~f); } \
	while (0)

typedef void (*dp_proc_single_callback_t) (struct seq_file *);

/* Example to extract the private data which is set at dp_proc_init_callback_t:
 *   struct dp_proc_file_entry *p = (struct dp_proc_file_entry *) s->private;
 *   struct my_struct_data *data = p->data;
 */
typedef int (*dp_proc_callback_t) (struct seq_file *, int);

/* caller can alloc buffer and pass to param. Later it can be used
 * during dp_proc_callback_t callback.
 * For example to allocate private data during dp_proc_init_callback_t call:
 *   struct dp_proc_file_entry *p = (struct dp_proc_file_entry *) param;
 *   struct my_struct_data *data = kzalloc(sizeof(*data), GFP_ATOMIC);
 *   p->data = data;
 */ 
typedef int (*dp_proc_init_callback_t) (void *param);
typedef ssize_t(*dp_proc_write_callback_t) (struct file *file,
					     const char __user *input,
					     size_t size, loff_t *loff);

struct dp_proc_file_entry {
	dp_proc_callback_t multi_callback;
	dp_proc_single_callback_t single_callback;
	int pos;
	int single_call_only;
	/* note: data must be allocated via kmalloc/kzalloc by caller
	 *       during dp_proc_init_callback_t API call.
	 *       This memory will be freed automatically by this wrapper
	 */
	void *data;
};

struct dp_proc_entry {
	char *name;
	dp_proc_single_callback_t single_callback;
	dp_proc_callback_t multi_callback;
	dp_proc_init_callback_t init_callback;
	dp_proc_write_callback_t write_callback;
	struct file_operations ops;
};

int dp_getopt(char *cmd[], int cmd_size, int *cmd_offset,
	      char **optarg, const char *optstring);

void dp_proc_entry_create(struct dentry *parent_node,
			  struct dp_proc_entry *proc_entry);

int dp_atoi(unsigned char *str);
int dp_strncmpi(const char *s1, const char *s2, size_t n);
void dp_replace_ch(char *p, int len, char orig_ch, char new_ch);

/*Split buffer to multiple segment with separator space.
 *And put pointer to array[].
 *By the way, original buffer will be overwritten with '\0' at some place.
 */
int dp_split_buffer(char *buffer, char *array[], int max_param_num);

/* below ltq_xxx api is for back-compatible only */
#define ltq_atoi dp_atoi
#define ltq_strncmpi dp_strncmpi
#define ltq_replace_ch dp_replace_ch
#define ltq_remove_leading_whitespace dp_remove_leading_whitespace
#define ltq_split_buffer dp_split_buffer
#define set_start_end_id dp_set_start_end_id
void dp_set_start_end_id(unsigned int new_start, unsigned int new_end,
			 unsigned int max_start, unsigned int max_end,
			 unsigned int default_start, unsigned int default_end,
			 unsigned int *start, unsigned int *end);

#endif				/*DATAPATH_PROC_H */
