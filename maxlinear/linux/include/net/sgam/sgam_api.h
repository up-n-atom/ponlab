/* SPDX-License-Identifier: GPL-2.0 */
/* This header file mainly provides new skb extension used in Session based
 * Group Acounting and Metering(SGAM).
 *
 * Copyright (C) 2022-2024 MaxLinear, Inc.
 */
#ifndef __SGAM_H
#define __SGAM_H

#include <linux/types.h>
#include <linux/list.h>
#include <net/sgam_api.h>
#include <uapi/linux/netfilter/xt_GROUP.h>

#define SGAM_EXT_NAME     "sgam-ext"

/* Struct for skb extension SGAM_EXT_NAME */
struct sgam_skb_ext_info {
/* NOTE: Always make sure:
 * SGAM_MAX_METER < PP_SI_TBM_MAX and
 * SGAM_MAX_GROUP_ACCT < PP_SI_SGC_MAX
 */
#define SGAM_MAX_METER 2
#define SGAM_MAX_GROUP_ACCT 2
	__u16 meter_id[SGAM_MAX_METER];
	__u16 acct_id[SGAM_MAX_GROUP_ACCT];
	__u8  acct_pool[SGAM_MAX_GROUP_ACCT]; /* SGC group */
	__u8 remark_dscp;
};

enum pp_sgc_lvl {
	PP_SGC_LVL_0 = 0, /* Group 0 used for FBM(SSB) monitoring */
	PP_SGC_LVL_1, /* Used by SGAM */
	PP_SGC_LVL_2, /* Used by interface stats */
	PP_SGC_LVL_3, /* Used by interface stats */
	PP_SGC_LVL_4, /* Used by interface stats */
	PP_SGC_LVL_5, /* Used by interface stats */
	PP_SGC_LVL_6, /* Used by interface stats */
	PP_SGC_LVL_7, /* Used by SGAM */
	PP_SGC_LVL_MAX = PP_SGC_LVL_7
};

/* Forward declaration for sk_buff structure */
struct sk_buff;

/* For xt_GROUP */
int sgam_xt_get_id_from_name(struct xt_group_info *info);
int sgam_xt_update_db(struct xt_group_info *info);

/* Exported API's for SGAM users */
int sgam_add_new_account(struct sgam_acct_info *acct_info);
int sgam_account_attach_to_skb(struct sk_buff *skb,
		struct sgam_acct_info *acct_info);
int sgam_get_acct_name_list(struct sgam_list_info *lptr);
int sgam_get_account_stats(struct sgam_cmd_acct_stats *stats_info);
int sgam_del_account_entry(struct sgam_del_info *del_info);
int sgam_meter_attach_to_skb(struct sk_buff *skb,
		struct sgam_meter_info *meter);
int sgam_convert_pp_to_sgam_sgc_grp(int pp_sgc_grp);
int sgam_convert_sgam_to_pp_sgc_grp(int sgam_sgc_grp);
int sgam_acct_reset_sgc_counters_by_id(s32 sgc_grp, s32 sgc_id);

#endif /* __SGAM_H */
