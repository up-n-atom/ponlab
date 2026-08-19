/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2022-2024 MaxLinear, Inc.
 */
#ifndef _SGAM_DB_H_
#define _SGAM_DB_H_

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/pp_api.h>

#define SGAM_INIT_LIST_HEAD INIT_LIST_HEAD
#define SGAM_ADD_TAIL_NODE(node, head) list_add_tail(node, head)
#define SGAM_ADD_NODE(node, head) list_add(node, head)
#define SGAM_DEL_NODE(node, head) list_del(node, head)

/* Group account related structs */
struct sgam_acct_node {
	struct list_head list;
	struct sgam_acct_info info;
	atomic_t ref_count;
};

/* Group meter related structs */
struct sgam_meter_node {
	struct list_head list; /* Same as sgam_group_node */
	char name[MAX_LABEL_LEN + 1];
	u16 tbm_id;
	atomic_t ref_count;
	__u8 remark_dscp;
	struct pp_dual_tbm tbm;
};

static inline void *sgam_malloc(u32 size)
{
	gfp_t flags = 0;

	flags |= GFP_KERNEL;
	return kzalloc(size, flags);
}

static inline void sgam_free(void **ptr)
{
	/* kfree(NULL) is safe, hence null checking of ptr is not required */
	kfree(*ptr);
	*ptr = NULL;
}

/* APIs for accessing DB */
int sgam_init_db(void);
int sgam_exit_db(void);
int sgam_del_meter_entry(struct sgam_del_info *del_info);
int sgam_del_account_entry(struct sgam_del_info *del_info);
int sgam_add_new_account(struct sgam_acct_info *acct_info);
int sgam_add_new_meter(const struct sgam_meter_info *meter_info);
int sgam_get_meter_info(struct sgam_meter_info *meter_info);
int sgam_get_account_stats(struct sgam_cmd_acct_stats *stats_info);
void sgam_get_acct_count(int *count);
void sgam_get_meter_count(int *count);
int sgam_get_acct_name_list(struct sgam_list_info *lptr);
int sgam_get_meter_name_list(struct sgam_list_info *lptr);
int sgam_acct_reset_sgc_counters_by_id(s32 sgc_grp, s32 sgc_id);

/* APIs for xt_GROUP operations */
int sgam_xt_get_sgc_info(char *name, u16 *sgc_id, u8 *sgc_grp);
int sgam_xt_get_tbm_info(char *name, struct xt_group_info *info);
int sgam_xt_update_sgc_info(struct xt_group_info *info);
int sgam_xt_update_tbm_info(struct xt_group_info *info);

#endif /* _SGAM_DB_H_ */
