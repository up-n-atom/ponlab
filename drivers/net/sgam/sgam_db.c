// SPDX-License-Identifier: GPL-2.0
/* Allocating accounts(SGC) and meters(TBM) are handled here. Also databses of
 * configured meters and accounts are managed here.
 *
 * Copyright (C) 2022-2024 MaxLinear, Inc.
 */
#define pr_fmt(fmt) "[sgam:%s:%d] " fmt, __func__, __LINE__

#include <linux/ioctl.h>
#include <linux/string.h>
#include <net/sgam/sgam_api.h>
#include <soc/mxl/mxl_skb_ext.h>

#include "sgam_db.h"

struct sgam_database {
	/* Head nodes for each database */
	struct list_head acct_db_head; /* For sgam_grp_db */
	struct list_head mtr_db_head; /* For sgam_mtr_db */

	/* Global locks for each database */
	spinlock_t acct_db_lock; /* For sgam_grp_db */
	spinlock_t mtr_db_lock; /* For sgam_mtr_db */

	/* Counters for total number of accounts and meters configured */
	atomic_t total_acct_ctr; /* For sgam_grp_db */
	atomic_t total_mtr_ctr; /* For sgam_mtr_db */
} g_sgam_db;

/* Maps SGAM user-group ID's to corresponding PP SGC groups */
#ifdef DOCSIS_SUPPORT
#define SGAM_MAX_ACCT_GRP 4
static const int sgam_pp_sgc_grp_map[] = {
	PP_SGC_LVL_1,
	PP_SGC_LVL_2,
	PP_SGC_LVL_4,
	PP_SGC_LVL_5
};
#else
#define SGAM_MAX_ACCT_GRP 2
static const int sgam_pp_sgc_grp_map[] = {
	PP_SGC_LVL_1,
	PP_SGC_LVL_7,
};
#endif /* DOCSIS_SUPPORT */

/* MXL SKB extension id for SGAM */
unsigned int sgam_ext_id = MXL_SKB_EXT_INVALID;

static inline int is_valid_label(const char *label)
{
	return (label && (label[0] && strlen(label) <= MAX_LABEL_LEN));
}

int sgam_convert_sgam_to_pp_sgc_grp(int sgam_sgc_grp)
{
	if (sgam_sgc_grp > 0 && sgam_sgc_grp <= SGAM_MAX_ACCT_GRP)
		return sgam_pp_sgc_grp_map[sgam_sgc_grp - 1];
	else
		return SGAM_FAILURE;
}
EXPORT_SYMBOL(sgam_convert_sgam_to_pp_sgc_grp);

int sgam_convert_pp_to_sgam_sgc_grp(int pp_sgc_grp)
{
	int sgam_sgc_grp;

	for (sgam_sgc_grp = 0; sgam_sgc_grp < SGAM_MAX_ACCT_GRP; sgam_sgc_grp++) {
		if (sgam_pp_sgc_grp_map[sgam_sgc_grp] == pp_sgc_grp)
			return (sgam_sgc_grp + 1);
	}
	return SGAM_FAILURE;
}
EXPORT_SYMBOL(sgam_convert_pp_to_sgam_sgc_grp);

static int sgam_alloc_sgc(u16 sgc_grp, u16 *sgc_id)
{
	int ret = SGAM_SUCCESS;
	u32 owner = PP_SGC_SHARED_OWNER;

	ret = pp_sgc_alloc(owner, sgc_grp, sgc_id, 1);
	if (unlikely(ret)) {
		pr_err("pp_sgc_alloc failed for SGAM_ACCT_GRP%u\n",
			sgam_convert_pp_to_sgam_sgc_grp(sgc_grp));
	}

	return ret;
}

static int sgam_free_meters(void)
{
	struct sgam_meter_node *node, *tmp;
	int ret = SGAM_SUCCESS;

	spin_lock_bh(&g_sgam_db.mtr_db_lock);
	list_for_each_entry_safe(node, tmp, &g_sgam_db.mtr_db_head, list) {
		pr_debug("freeing meter: %s\n", node->name);
		ret = pp_dual_tbm_free(node->tbm_id);
		if (unlikely(ret)) {
			pr_err("pp_dual_tbm_free() failed for id %u\n",
			       node->tbm_id);
		}
		list_del(&node->list);
		sgam_free((void *)&node);
	}
	spin_unlock_bh(&g_sgam_db.mtr_db_lock);

	return ret;
}

static int sgam_free_accounts(void)
{
	struct sgam_acct_node *node, *tmp;
	u32 owner = PP_SGC_SHARED_OWNER;
	int ret = SGAM_SUCCESS;

	spin_lock_bh(&g_sgam_db.acct_db_lock);
	list_for_each_entry_safe(node, tmp, &g_sgam_db.acct_db_head, list) {
		pr_debug("freeing account: %s\n", node->info.name);
		ret = pp_sgc_free(owner, node->info.sgc_grp,
				  &node->info.sgc_id, 1);
		if (unlikely(ret)) {
			pr_err("pp_sgc_free() failed for sgc_id %u\n",
			       node->info.sgc_id);
		}
		list_del(&node->list);
		sgam_free((void *)&node);
	}
	spin_unlock_bh(&g_sgam_db.acct_db_lock);

	return ret;
}

void sgam_get_acct_count(int *count)
{
	*count = atomic_read(&g_sgam_db.total_acct_ctr);
}
EXPORT_SYMBOL(sgam_get_acct_count);

void sgam_get_meter_count(int *count)
{
	*count = atomic_read(&g_sgam_db.total_mtr_ctr);
}

int sgam_get_acct_name_list(struct sgam_list_info *lptr)
{
	struct sgam_acct_node *node, *tmp;
	int i = 0, ret = SGAM_SUCCESS;

	if (!lptr) {
		pr_err("Invalid list info!!\n");
		return SGAM_FAILURE;
	}

	/* Loop through the list and copy names to list_info */
	spin_lock_bh(&g_sgam_db.acct_db_lock);
	list_for_each_entry_safe(node, tmp, &g_sgam_db.acct_db_head, list) {
		if (i < lptr->count_info.count) {
			strncpy(lptr->namelist[i].name, node->info.name, MAX_LABEL_LEN);
			i++;
		} else {
			/* If control comes here, it means there are entries
			 * added in between SGAM_GET_COUNT_CMD and
			 * SGAM_LIST_CMD calls
			 */
			break;
		}
	}
	spin_unlock_bh(&g_sgam_db.acct_db_lock);

	return ret;
}
EXPORT_SYMBOL(sgam_get_acct_name_list);

int sgam_get_meter_name_list(struct sgam_list_info *lptr)
{
	struct sgam_meter_node *node, *tmp;
	int i = 0, ret = SGAM_SUCCESS;

	/* Loop through the list and copy names to list_info */
	spin_lock_bh(&g_sgam_db.mtr_db_lock);
	list_for_each_entry_safe(node, tmp, &g_sgam_db.mtr_db_head, list) {
		if (i < lptr->count_info.count) {
			strncpy(lptr->namelist[i].name, node->name, MAX_LABEL_LEN);
			i++;
		} else {
			/* If control comes here, it means there are entries
			 * added in between SGAM_GET_COUNT_CMD and
			 * SGAM_LIST_CMD calls
			 */
			break;
		}
	}
	spin_unlock_bh(&g_sgam_db.mtr_db_lock);

	return ret;
}

int sgam_del_meter_entry(struct sgam_del_info *del_info)
{
	struct sgam_meter_node *node, *tmp;
	u16 ref_count;
	int ret = -EINVAL;

	spin_lock_bh(&g_sgam_db.mtr_db_lock);
	list_for_each_entry_safe(node, tmp, &g_sgam_db.mtr_db_head, list) {
		/* If name is not maching, continue to next node */
		if (strncmp(node->name, del_info->name, MAX_LABEL_LEN))
			continue;

		ref_count = atomic_read(&node->ref_count);
		if (ref_count) {
			pr_notice("%s is still used in %u iptable entries!\n",
				  node->name, ref_count);
			del_info->ref_count = ref_count;
			ret = -EBUSY;
			break;
		}

		ret = pp_dual_tbm_free(node->tbm_id);
		if (unlikely(ret)) {
			pr_err("pp_dual_tbm_free() failed for id %u\n",
			       node->tbm_id);
			break;
		}
		list_del(&node->list);
		sgam_free((void *)&node);
		atomic_dec_if_positive(&g_sgam_db.total_mtr_ctr);
		ret = SGAM_SUCCESS;
		break;
	}
	spin_unlock_bh(&g_sgam_db.mtr_db_lock);

	return ret;
}

int sgam_del_account_entry(struct sgam_del_info *del_info)
{
	struct sgam_acct_node *node = NULL, *tmp;
	bool node_found = false;
	u16 ref_count;
	u32 owner = PP_SGC_SHARED_OWNER;
	int ret = -EINVAL;
	int pp_sgc_grp = -1;

	if (!del_info) {
		pr_err("Invalid del_info!!\n");
		return -EINVAL;
	}

	if (del_info->sgc_id != PP_SGC_INVALID) {
		pp_sgc_grp = sgam_convert_sgam_to_pp_sgc_grp(del_info->sgc_grp);
		if (pp_sgc_grp == -1) {
			pr_err("Invalid group number %d, valid values: '1' to '%d'\n",
					del_info->sgc_grp, SGAM_MAX_ACCT_GRP);
			return -EINVAL;
		}
	}

	spin_lock_bh(&g_sgam_db.acct_db_lock);
	list_for_each_entry_safe(node, tmp, &g_sgam_db.acct_db_head, list) {
		/* Break the loop if account is found */
		if ((is_valid_label(del_info->name) &&
			!strncmp(node->info.name, del_info->name, MAX_LABEL_LEN)) ||
			(node->info.sgc_id == del_info->sgc_id &&
			node->info.sgc_grp == pp_sgc_grp)) {
			node_found = true;
			break;
		}
	}

	if (!node_found) {
		pr_err("Invalid SGC account details!!\n");
		ret = -EINVAL;
		goto DONE;
	}

	ref_count = atomic_read(&node->ref_count);
	if (ref_count) {
		pr_notice("%s is still used in %u iptable entries!\n",
			  node->info.name, ref_count);
		del_info->ref_count = ref_count;
		ret = -EBUSY;
		goto DONE;
	}

	ret = pp_sgc_free(owner, node->info.sgc_grp,
			  &node->info.sgc_id, 1);
	if (unlikely(ret)) {
		pr_err("pp_sgc_free() failed for sgc_id %u\n",
		       node->info.sgc_id);
		goto DONE;
	}
	list_del(&node->list);
	atomic_dec_if_positive(&g_sgam_db.total_acct_ctr);
	sgam_free((void *)&node);
	ret = SGAM_SUCCESS;

DONE:
	spin_unlock_bh(&g_sgam_db.acct_db_lock);

	return ret;
}
EXPORT_SYMBOL(sgam_del_account_entry);

static int is_acct_entry_exists(const char *name)
{
	struct sgam_acct_node *node, *tmp;
	int ret = SGAM_FAILURE;

	spin_lock_bh(&g_sgam_db.acct_db_lock);
	list_for_each_entry_safe(node, tmp, &g_sgam_db.acct_db_head, list) {
		if (!strncmp(node->info.name, name, MAX_LABEL_LEN)) {
			ret = SGAM_SUCCESS;
			break;
		}
	}
	spin_unlock_bh(&g_sgam_db.acct_db_lock);

	return ret;
}

int sgam_add_new_account(struct sgam_acct_info *acct_info)
{
	int ret = SGAM_SUCCESS;
	int pp_sgc_grp;
	struct sgam_acct_node *tmp_node = NULL;

	if (!acct_info) {
		pr_err("Invalid acct_info!!\n");
		return -EINVAL;
	}

	if (!is_valid_label(acct_info->name)) {
		pr_err("Invalid account name %s!!\n", acct_info->name);
		return -EINVAL;
	}

	if (is_acct_entry_exists(acct_info->name) == SGAM_SUCCESS) {
		pr_err("Group account already exists for %s\n", acct_info->name);
		return -EEXIST;
	}

	pp_sgc_grp = sgam_convert_sgam_to_pp_sgc_grp(acct_info->sgc_grp);
	if (pp_sgc_grp == -1) {
		pr_err("Invalid group number %d, valid values: '1' to '%d'\n",
				acct_info->sgc_grp, SGAM_MAX_ACCT_GRP);
		return -EINVAL;
	}

	tmp_node = sgam_malloc(sizeof(struct sgam_acct_node));
	if (!tmp_node) {
		pr_err("Unable to alloc memory!!\n");
		return -ENOMEM;
	}

	/* Fill up node info */
	strncpy(tmp_node->info.name, acct_info->name, MAX_LABEL_LEN);
	tmp_node->info.sgc_grp = pp_sgc_grp;
	INIT_LIST_HEAD(&tmp_node->list);
	ret = sgam_alloc_sgc(tmp_node->info.sgc_grp, &tmp_node->info.sgc_id);
	if (unlikely(ret != SGAM_SUCCESS)) {
		pr_err("Could not alloc SGC for %s!!\n", acct_info->name);
		sgam_free((void *)&tmp_node);
		return ret;
	}
	/* Add node to the acct db and increment counter */
	spin_lock_bh(&g_sgam_db.acct_db_lock);
	list_add(&tmp_node->list, &g_sgam_db.acct_db_head);
	spin_unlock_bh(&g_sgam_db.acct_db_lock);
	atomic_inc(&g_sgam_db.total_acct_ctr);
	acct_info->sgc_id = tmp_node->info.sgc_id;

	return ret;
}
EXPORT_SYMBOL(sgam_add_new_account);

static int is_meter_entry_exists(const char *name)
{
	struct sgam_meter_node *node, *tmp;
	int ret = SGAM_FAILURE;

	spin_lock_bh(&g_sgam_db.mtr_db_lock);
	list_for_each_entry_safe(node, tmp, &g_sgam_db.mtr_db_head, list) {
		if (!strncmp(node->name, name, MAX_LABEL_LEN)) {
			ret = SGAM_SUCCESS;
			break;
		}
	}
	spin_unlock_bh(&g_sgam_db.mtr_db_lock);

	return ret;
}

/* This is for avoiding the effect of changes in "enum pp_tbm_mode"
 * by pp team
 */
static enum pp_tbm_mode get_tbm_mode(enum sgam_meter_types m_type)
{
	switch (m_type) {
	case SGAM_TBM_MODE_4115_CB:
		return PP_TBM_MODE_4115_CB;
	case SGAM_TBM_MODE_4115_CA:
		return PP_TBM_MODE_4115_CA;
	case SGAM_TBM_MODE_2698_CB:
		return PP_TBM_MODE_2698_CB;
	case SGAM_TBM_MODE_2698_CA:
		return PP_TBM_MODE_2698_CA;
	default: /* unlikely case */
		return PP_TBM_MODE_4115_CB;
	}
}

int sgam_add_new_meter(const struct sgam_meter_info *meter_info)
{
	int ret = SGAM_SUCCESS;
	struct sgam_meter_node *tmp_node = NULL;

	if (!is_valid_label(meter_info->name)) {
		pr_err("Invalid meter name %s!!\n", meter_info->name);
		return -EINVAL;
	}

	if (is_meter_entry_exists(meter_info->name) == SGAM_SUCCESS) {
		pr_err("Meter already exists for %s\n", meter_info->name);
		return -EEXIST;
	}

	tmp_node = sgam_malloc(sizeof(struct sgam_meter_node));
	if (!tmp_node) {
		pr_err("Unable to alloc memory!!\n");
		return -ENOMEM;
	}

	/* Fill up meter name info */
	INIT_LIST_HEAD(&tmp_node->list);
	strncpy(tmp_node->name, meter_info->name, MAX_LABEL_LEN);
	tmp_node->tbm_id = 0;
	tmp_node->remark_dscp = meter_info->remark_dscp;
	/* Fill up tbm info */
	tmp_node->tbm.enable = true;
	tmp_node->tbm.len_type = PP_TBM_NEW_LEN;
	tmp_node->tbm.mode = get_tbm_mode(meter_info->mtr_type);
	tmp_node->tbm.cir = meter_info->cir;
	tmp_node->tbm.pir = meter_info->pir;
	tmp_node->tbm.cbs = meter_info->cbs;
	tmp_node->tbm.pbs = meter_info->pbs;

	/* Alloc TBM form pp */
	ret = pp_dual_tbm_alloc(&tmp_node->tbm_id, &tmp_node->tbm);
	if (unlikely(ret != SGAM_SUCCESS)) {
		pr_err("Could not alloc TBM for %s!!\n", meter_info->name);
		sgam_free((void *)&tmp_node);
		return ret;
	}

	/* Add node to the meter db and increment counter */
	spin_lock_bh(&g_sgam_db.mtr_db_lock);
	list_add(&tmp_node->list, &g_sgam_db.mtr_db_head);
	spin_unlock_bh(&g_sgam_db.mtr_db_lock);
	atomic_inc(&g_sgam_db.total_mtr_ctr);

	return ret;
}

/* This is to aoid effect of changes in "enum pp_tbm_mode" by pp team */
static enum sgam_meter_types get_meter_type(enum pp_tbm_mode tbm_mode)
{
	switch (tbm_mode) {
	case PP_TBM_MODE_4115_CB:
		return SGAM_TBM_MODE_4115_CB;
	case PP_TBM_MODE_4115_CA:
		return SGAM_TBM_MODE_4115_CA;
	case PP_TBM_MODE_2698_CB:
		return SGAM_TBM_MODE_2698_CB;
	case PP_TBM_MODE_2698_CA:
		return SGAM_TBM_MODE_2698_CA;
	default: /* unlikely case */
		return SGAM_TBM_MODE_4115_CB;
	}
}

int sgam_get_meter_info(struct sgam_meter_info *meter_info)
{
	int ret = SGAM_FAILURE;
	struct sgam_meter_node *node = NULL, *tmp = NULL;

	if (!is_valid_label(meter_info->name)) {
		pr_err("Invalid meter name %s!!\n", meter_info->name);
		return -EINVAL;
	}

	spin_lock_bh(&g_sgam_db.mtr_db_lock);
	list_for_each_entry_safe(node, tmp, &g_sgam_db.mtr_db_head, list) {
		if (!strncmp(node->name, meter_info->name, MAX_LABEL_LEN)) {
			ret = SGAM_SUCCESS;
			break;
		}
	}
	if (ret == SGAM_SUCCESS && node) {
		meter_info->tbm_id = node->tbm_id;
		meter_info->cir = node->tbm.cir;
		meter_info->pir = node->tbm.pir;
		meter_info->cbs = node->tbm.cbs;
		meter_info->pbs = node->tbm.pbs;
		meter_info->remark_dscp = node->remark_dscp;
		meter_info->mtr_type = get_meter_type(node->tbm.mode);
	}
	spin_unlock_bh(&g_sgam_db.mtr_db_lock);

	return ret;
}

int sgam_get_account_stats(struct sgam_cmd_acct_stats *stats_info)
{
	int ret = SGAM_FAILURE;
	int pp_sgc_grp = -1, pp_sgc_id = PP_SGC_INVALID;
	struct pp_stats hw_stats = { 0 };
	struct sgam_acct_node *node = NULL, *tmp = NULL;

	if (!stats_info) {
		pr_err("Invalid stats_info!!\n");
		return -EINVAL;
	}

	if (stats_info->sgc_id != PP_SGC_INVALID) {
		pp_sgc_grp = sgam_convert_sgam_to_pp_sgc_grp(stats_info->sgc_grp);
		if (pp_sgc_grp == -1) {
			pr_err("Invalid group number %d, valid values: '1' to '%d'\n",
					stats_info->sgc_grp, SGAM_MAX_ACCT_GRP);
			ret = -EINVAL;
			goto DONE;
		}
		pp_sgc_id = stats_info->sgc_id;
	} else if (is_valid_label(stats_info->name)) {
		/* Get sgc_id and sgc_grp */
		spin_lock_bh(&g_sgam_db.acct_db_lock);
		list_for_each_entry_safe(node, tmp, &g_sgam_db.acct_db_head, list) {
			if (!strncmp(node->info.name, stats_info->name, MAX_LABEL_LEN)) {
				pp_sgc_grp = node->info.sgc_grp;
				pp_sgc_id = node->info.sgc_id;
				break;
			}
		}
		spin_unlock_bh(&g_sgam_db.acct_db_lock);
	}

	if (pp_sgc_id == PP_SGC_INVALID ||
		pp_sgc_grp < 0 || pp_sgc_grp > PP_SGC_LVL_MAX) {
		pr_err("Invalid acct details!!\n");
		ret = -EINVAL;
		goto DONE;
	}

	ret = pp_sgc_get(pp_sgc_grp, pp_sgc_id, &hw_stats, NULL);
	if (unlikely(ret)) {
		pr_err("pp_sgc_get failed! grp: %u, id: %u\n",
				stats_info->sgc_grp,	stats_info->sgc_id);
		ret = SGAM_FAILURE;
		goto DONE;
	}
	stats_info->sgc_grp = sgam_convert_pp_to_sgam_sgc_grp(pp_sgc_grp);
	stats_info->acct_stat.packets = hw_stats.packets;
	stats_info->acct_stat.bytes = hw_stats.bytes;
	stats_info->acct_stat.ing_dropped_bytes = hw_stats.ing_dropped_bytes;
	stats_info->acct_stat.egr_dropped_bytes = hw_stats.egr_dropped_bytes;
	ret = SGAM_SUCCESS;

DONE:
	return ret;
}
EXPORT_SYMBOL(sgam_get_account_stats);

/* sgam <-> xt_GROUP related methods */
int sgam_xt_get_sgc_info(char *name, u16 *sgc_id, u8 *sgc_grp)
{
	int ret = SGAM_FAILURE;
	struct sgam_acct_node *node, *tmp;

	/* NOTE: Caller (xt_GROUP module) must make sure
	 *       len of name is < NAX_LABEL_LEN
	 */
	spin_lock_bh(&g_sgam_db.acct_db_lock);
	list_for_each_entry_safe(node, tmp, &g_sgam_db.acct_db_head, list) {
		if (!strncmp(node->info.name, name, MAX_LABEL_LEN)) {
			*sgc_id = node->info.sgc_id;
			*sgc_grp = node->info.sgc_grp;
			atomic_inc(&node->ref_count);
			ret = SGAM_SUCCESS;
			break;
		}
	}
	spin_unlock_bh(&g_sgam_db.acct_db_lock);

	return ret;
}

int sgam_xt_get_tbm_info(char *name, struct xt_group_info *info)
{
	int ret = SGAM_FAILURE;
	struct sgam_meter_node *node, *tmp;

	/* NOTE: Caller (xt_GROUP module) has to make sure
	 *       len of name is < NAX_LABEL_LEN
	 */
	spin_lock_bh(&g_sgam_db.mtr_db_lock);
	list_for_each_entry_safe(node, tmp, &g_sgam_db.mtr_db_head, list) {
		if (!strncmp(node->name, name, MAX_LABEL_LEN)) {
			info->id = node->tbm_id;
			info->remark_dscp = node->remark_dscp;
			atomic_inc(&node->ref_count);
			ret = SGAM_SUCCESS;
			break;
		}
	}
	spin_unlock_bh(&g_sgam_db.mtr_db_lock);

	return ret;
}

int sgam_xt_update_sgc_info(struct xt_group_info *info)
{
	int ret = SGAM_FAILURE;
	struct sgam_acct_node *node, *tmp;

	spin_lock_bh(&g_sgam_db.acct_db_lock);
	list_for_each_entry_safe(node, tmp, &g_sgam_db.acct_db_head, list) {
		if (node->info.sgc_grp == info->grp &&
		    node->info.sgc_id == info->id) {
			atomic_dec_if_positive(&node->ref_count);
			ret = SGAM_SUCCESS;
			break;
		}
	}
	spin_unlock_bh(&g_sgam_db.acct_db_lock);

	return ret;
}

int sgam_xt_update_tbm_info(struct xt_group_info *info)
{
	int ret = SGAM_FAILURE;
	struct sgam_meter_node *node, *tmp;

	spin_lock_bh(&g_sgam_db.mtr_db_lock);
	list_for_each_entry_safe(node, tmp, &g_sgam_db.mtr_db_head, list) {
		if (node->tbm_id == info->id) {
			atomic_dec_if_positive(&node->ref_count);
			ret = SGAM_SUCCESS;
			break;
		}
	}
	spin_unlock_bh(&g_sgam_db.mtr_db_lock);

	return ret;
}

int sgam_init_db(void)
{
	/* Init db heads */
	INIT_LIST_HEAD(&g_sgam_db.acct_db_head);
	INIT_LIST_HEAD(&g_sgam_db.mtr_db_head);

	/* Init db locks */
	spin_lock_init(&g_sgam_db.acct_db_lock);
	spin_lock_init(&g_sgam_db.mtr_db_lock);

	/* Init db counters to 0 */
	atomic_set(&g_sgam_db.total_acct_ctr, 0);
	atomic_set(&g_sgam_db.total_mtr_ctr, 0);

	/* Register to MXL skb extensions */
	sgam_ext_id = mxl_skb_ext_register(SGAM_EXT_NAME,
			sizeof(struct sgam_skb_ext_info));
	if (sgam_ext_id == MXL_SKB_EXT_INVALID)
		pr_err("sgam failed to register to mxl_skb_ext!\n");

	return 0;
}

int sgam_exit_db(void)
{
	int ret = 0;

	if (sgam_free_accounts() != SGAM_SUCCESS) {
		pr_err("Failure in freeing accounts\n");
		ret = SGAM_FAILURE;
	}
	if (sgam_free_meters() != SGAM_SUCCESS) {
		pr_err("Failure in freeing meters\n");
		ret = SGAM_FAILURE;
	}

	return ret;
}

static void *sgam_skb_ext_add(struct sk_buff *skb, unsigned int ext_id)
{
	int i = 0;
	struct sgam_skb_ext_info *ext_ptr = NULL;

	if (sgam_ext_id == MXL_SKB_EXT_INVALID) {
		pr_warn_once("INVALID sgam_ext_id!\n");
		return NULL;
	}

	/* Check whether mxl_skb_ext already present.
	 * Update it if yes, create new if no.
	 */
	ext_ptr = mxl_skb_ext_find(skb, sgam_ext_id);
	if (!ext_ptr) {
		/* Extension not present, create new one */
		ext_ptr = mxl_skb_ext_add(skb, sgam_ext_id);
		if (unlikely(!ext_ptr)) {
			pr_warn_once("mxl skb extension add failed!!\n");
			return NULL;
		}
		/* Initialize the fields */
		for (i = 0; i < SGAM_MAX_METER; i++)
			ext_ptr->meter_id[i] = PP_TBM_INVALID;

		for (i = 0; i < SGAM_MAX_GROUP_ACCT; i++) {
			ext_ptr->acct_id[i] = PP_SGC_INVALID;
			ext_ptr->acct_pool[i] = -1;
		}
	}
	return ext_ptr;
}

int sgam_meter_attach_to_skb(struct sk_buff *skb, struct sgam_meter_info *meter)
{
	int i = 0;
	bool attached = false;
	struct xt_group_info grp_meter_info = {0};
	struct sgam_skb_ext_info *ext_ptr = NULL;

	if (!meter) {
		pr_warn_once("INVALID meter info!\n");
		return SGAM_FAILURE;
	}

	grp_meter_info.id = meter->tbm_id;
	grp_meter_info.remark_dscp = meter->remark_dscp;

	if (meter->tbm_id == PP_TBM_INVALID) {
		if (is_valid_label(meter->name)) {
			/* Get the meter info by meter_name */
			if (sgam_xt_get_tbm_info(meter->name,
					&grp_meter_info) != SGAM_SUCCESS) {
				pr_warn_once("INVALID sgam meter name: %s!\n", meter->name);
				return SGAM_FAILURE;
			}
		} else {
			pr_warn_once("INVALID sgam meter info!\n");
			return SGAM_FAILURE;
		}
	}

	ext_ptr = sgam_skb_ext_add(skb, sgam_ext_id);
	if (!ext_ptr)
		return SGAM_FAILURE;

	for (i = 0; i < SGAM_MAX_METER; i++) {
		if (ext_ptr->meter_id[i] == PP_TBM_INVALID) {
			ext_ptr->meter_id[i] = grp_meter_info.id;
			ext_ptr->remark_dscp = grp_meter_info.remark_dscp;
			attached = true;
			break;
		}
	}
	if (!attached && i >= SGAM_MAX_METER)
		pr_warn_once("More than %d meters per session is not allowed!!\n",
			     SGAM_MAX_METER);

	return SGAM_SUCCESS;
}
EXPORT_SYMBOL(sgam_meter_attach_to_skb);

int sgam_account_attach_to_skb(struct sk_buff *skb,
		struct sgam_acct_info *acct_info)
{
	int i;
	bool attached = false;
	u16 pp_sgc_id = PP_SGC_INVALID;
	u8 pp_sgc_grp = -1;
	struct sgam_skb_ext_info *ext_ptr = NULL;

	if (sgam_ext_id == MXL_SKB_EXT_INVALID) {
		pr_warn_once("INVALID sgam_ext_id!\n");
		return SGAM_FAILURE;
	}

	if (!acct_info) {
		pr_warn_once("INVALID account info!\n");
		return SGAM_FAILURE;
	}

	if (acct_info->sgc_id != PP_SGC_INVALID) {
		pp_sgc_grp = sgam_convert_sgam_to_pp_sgc_grp(acct_info->sgc_grp);
		if (pp_sgc_grp == -1) {
			pr_err("Invalid group number %d, valid values: '1' to '%d'\n",
					acct_info->sgc_grp, SGAM_MAX_ACCT_GRP);
			return SGAM_FAILURE;
		}
		pp_sgc_id = acct_info->sgc_id;
	} else if (is_valid_label(acct_info->name)) {
		/* Get sgc_id and sgc_grp */
		if (sgam_xt_get_sgc_info(acct_info->name, &pp_sgc_id,
				&pp_sgc_grp) != SGAM_SUCCESS) {
			pr_warn_once("INVALID sgam account name!\n");
			return SGAM_FAILURE;
		}
	}

	if (pp_sgc_id == PP_SGC_INVALID ||
		pp_sgc_grp < 0 || pp_sgc_grp > PP_SGC_LVL_MAX) {
		pr_err("Invalid acct details!!\n");
		return SGAM_FAILURE;
	}

	ext_ptr = sgam_skb_ext_add(skb, sgam_ext_id);
	if (!ext_ptr)
		return SGAM_FAILURE;

	for (i = 0; i < SGAM_MAX_GROUP_ACCT; i++) {
		if (likely(ext_ptr->acct_id[i] == PP_SGC_INVALID)) {
			ext_ptr->acct_id[i] = pp_sgc_id;
			ext_ptr->acct_pool[i] = pp_sgc_grp;
			attached = true;
			break;
		} else if (unlikely(ext_ptr->acct_pool[i] == pp_sgc_grp)) {
			/* Control comes here if there is already an
			 * acct present from the same pool(sgc grp)
			 * which is not allowed.
			 */
			pr_warn_once("Session can't be added to accounts from same"
					" pool!\n");
		}
	}
	if (!attached && i >= SGAM_MAX_GROUP_ACCT)
		pr_warn_once("More than %d accounts per session is not allowed!!\n",
			     SGAM_MAX_GROUP_ACCT);

	return SGAM_SUCCESS;
}
EXPORT_SYMBOL(sgam_account_attach_to_skb);

int sgam_acct_reset_sgc_counters_by_id(s32 sgc_grp, s32 sgc_id)
{
	int ret;
	s32 pp_sgc_grp = -1;
	u32 owner = PP_SGC_SHARED_OWNER;

	if (sgc_id != PP_SGC_INVALID) {
		pp_sgc_grp = sgam_convert_sgam_to_pp_sgc_grp(sgc_grp);
		if (pp_sgc_grp == -1) {
			pr_err("Invalid group number %d, valid values: '1' to '%d'\n",
					sgc_grp, SGAM_MAX_ACCT_GRP);
			return -EINVAL;
		}
	}

	if (sgc_id == PP_SGC_INVALID ||
		pp_sgc_grp < 0 || pp_sgc_grp > PP_SGC_LVL_MAX) {
		pr_err("Invalid acct details!!\n");
		return -EINVAL;
	}

	ret = pp_sgc_mod(owner, pp_sgc_grp, sgc_id, PP_STATS_RESET, 0, 0);
	if (unlikely(ret)) {
		pr_err("ERROR: pp_sgc_mod failed\n");
		return -EINVAL;
	}

	return SGAM_SUCCESS;
}
EXPORT_SYMBOL(sgam_acct_reset_sgc_counters_by_id);
