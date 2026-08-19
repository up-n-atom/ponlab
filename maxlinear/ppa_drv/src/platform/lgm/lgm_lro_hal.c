/******************************************************************************
**
** FILE NAME	: lgm_lro_hal.c
** PROJECT	: LGM
** MODULES	: PPA LRO HAL
**
** DATE		: 18 July 2019
** AUTHOR	: Kamal Eradath
** DESCRIPTION	: LRO hardware abstraction layer
** COPYRIGHT	: Copyright (c) 2020-2022 MaxLinear, Inc.
**                Copyright (c) 2014 Intel Corporation.
**
**	 For licensing information, see the file 'LICENSE' in the root folder of
**	 this software module.
**
** HISTORY
** $Date		$Author			$Comment
** 18 July 2018		Kamal Eradath		Initial Version
*******************************************************************************/
/*
 * ####################################
 *		Head File
 * ####################################
 */
/*
 *	Common Head File
 */
#include <linux/types.h>
#include <linux/spinlock.h>

#include <net/datapath_api.h>
#if IS_ENABLED(CONFIG_SOC_LGM)
#include <net/toe_np_lro.h>
#else
#include <net/intel_np_lro.h>
#endif
#include <net/ppa/ppa_api.h>
#include <net/ppa/ppa_hal_api.h>
#include <net/ppa/ppa_drv_wrapper.h>
#include <net/ppa/ppa_hal_wrapper.h>
#include "../../ppa_api/ppa_api_session.h"
#include "lgm_lro_hal.h"

static DEFINE_PER_CPU(PPA_HAL_STATS, lrostats);
#define PPA_HAL_STATS_INC(var, field) raw_cpu_inc(var.field)
#define PPA_HAL_STATS_DEC(var, field) raw_cpu_dec(var.field)

/* LRO table */
static ppa_lro_entry g_lro_table[MAX_LRO_ENTRIES];
static uint16_t g_lro_qid;
static int is_lro_init;
static spinlock_t port_lock;
static bool g_lro_enabled = true;
static struct dentry *ppa_lrohal_debugfs_dir;
static bool lro_hal_dbg_enable;

#define hal_err(fmt, arg...) pr_err(fmt, ##arg)
#define hal_dbg(fmt, arg...) pr_warn(fmt, ##arg)
#define lro_hal_err(fmt, arg...) hal_err("[LRO HAL]: " fmt, ##arg)
#define lro_hal_dbg(fmt, arg...) do {              \
	if (lro_hal_dbg_enable)                    \
		hal_dbg("[LRO HAL]: " fmt, ##arg); \
} while (0)

static void init_lro_table(void)
{
	struct dp_spl_cfg conn = {0};
	struct dp_qos_q_logic q_logic = {0};

	/* clear the lro table */
	ppa_memset(g_lro_table, 0, sizeof(ppa_lro_entry) * MAX_LRO_ENTRIES);

	/* get spl conn parameteres 0 means failure */
	if (!dp_spl_conn_get(0, DP_SPL_TOE, &conn, 1)) {
		is_lro_init = 0;
		lro_hal_err("LRO init failed\n");
		return;
	}

	/* egress port qid */;
	q_logic.q_id = conn.egp[0].qid;

	/* physical to logical qid */
	if (dp_qos_get_q_logic(&q_logic, 0) == DP_FAILURE) {
		lro_hal_err("%s:%d ERROR Failed to Logical Queue Id\n",
		       __func__, __LINE__);
		return;
	}

	/* store the logical qid */
	g_lro_qid = q_logic.q_logic_id;
	lro_hal_dbg("q_logic.q_id:%d g_lro_qid:%d\n",
		    q_logic.q_id, g_lro_qid);
	is_lro_init = 1;
}

/*!
 *	\fn int32_t add_lro_entry( )
 *	\ingroup GRX500_PPA_PAE_GLOBAL_FUNCTIONS
 *	\brief add a LRO HW session
 *	\param PPA_LRO_INFO * lro_entry
 *	\return >= 0: lro session id, otherwise: fail
 */
int32_t add_lro_entry(PPA_LRO_INFO *lro_entry)
{
	int32_t i = 0, ret = PPA_SUCCESS;
	struct lro_ops *lro_ops;

	if (!is_lro_init) {
		init_lro_table();
		if (!is_lro_init) {
			lro_hal_err("%s not ready yet!\n", __func__);
			PPA_HAL_STATS_INC(lrostats, uc_dropped_sess);
			return PPA_ENOTAVAIL;
		}
	}

	if (!g_lro_enabled) {
		lro_hal_dbg("%s not enabled yet!\n", __func__);
		return PPA_ENOTAVAIL;
	}

	spin_lock_bh(&port_lock);
	lro_ops = (struct lro_ops *)dp_get_ops(0, DP_OPS_LRO);
	if (lro_ops && lro_ops->find_free_lro_port)
		i = lro_ops->find_free_lro_port(lro_ops->toe, NULL);
	spin_unlock_bh(&port_lock);

	if (i == -1) {
		lro_hal_dbg("%s no LRO port available!\n", __func__);
		PPA_HAL_STATS_INC(lrostats, uc_dropped_sess);
		return PPA_FAILURE;
	}

	lro_hal_dbg("%s allocated port:%d\n", __func__, i);
	if (i < MAX_LRO_ENTRIES) {
		/* flow empty entry is i*/
		ppa_memset(&(g_lro_table[i]), 0, sizeof(ppa_lro_entry));

		switch (lro_entry->lro_type) {
		case LRO_TYPE_TCPV6:
		case LRO_TYPE_UDPV6:
			g_lro_table[i].f_ipv6 = 1;
			break;
		default:
			g_lro_table[i].f_ipv6 = 0;
		}

		/* call the lro driver */
		if (lro_ops && lro_ops->lro_start) {
			ret = lro_ops->lro_start(lro_ops->toe, i,
						 lro_entry->lro_type);
			if (ret < 0) {
				lro_hal_err("lro_start() failed for port:%d status:%d\n", i, ret);
				/* TODO: free the lro port, allocated by find_free_lro_port() */
				PPA_HAL_STATS_INC(lrostats, uc_dropped_sess);
				return ret;
			}
			g_lro_table[i].enabled = 1;
			g_lro_table[i].session_id = i;
			lro_entry->session_id = g_lro_table[i].session_id;
			lro_entry->dst_q = g_lro_qid;
			lro_hal_dbg("port:%d dst_q:%d\n",
				    lro_entry->session_id, lro_entry->dst_q);
		}
	}

	if (g_lro_table[i].f_ipv6)
		PPA_HAL_STATS_INC(lrostats, curr_uc_ipv6_session);
	else
		PPA_HAL_STATS_INC(lrostats, curr_uc_ipv4_session);
	lro_hal_dbg("%s succeeded on port:%d\n", __func__, i);
	return PPA_SUCCESS;
}

/*!
 *	\fn int32_t del_lro_entry( )
 *	\ingroup GRX500_PPA_PAE_GLOBAL_FUNCTIONS
 *	\brief delete a LRO HW session
 *	\param uint8_t sessionid
 *	\return >= 0: success, otherwise: fail
 */

int32_t del_lro_entry(uint8_t sessionid)
{
	struct lro_ops *lro_ops;

	lro_hal_dbg("%s requested for port:%d\n", __func__, sessionid);
	if (!is_lro_init) {
		lro_hal_err("%s not ready yet!\n", __func__);
		return PPA_FAILURE;
	}

	if (!g_lro_table[sessionid].enabled) {
		lro_hal_err("%s port:%d is not allocated yet!\n",
			    __func__, sessionid);
		return PPA_FAILURE;
	}

	spin_lock_bh(&port_lock);
	lro_ops = (struct lro_ops *)dp_get_ops(0, DP_OPS_LRO);
	if (lro_ops && lro_ops->lro_stop)
		lro_ops->lro_stop(lro_ops->toe, sessionid);
	spin_unlock_bh(&port_lock);

	if (g_lro_table[sessionid].f_ipv6)
		PPA_HAL_STATS_DEC(lrostats, curr_uc_ipv6_session);
	else
		PPA_HAL_STATS_DEC(lrostats, curr_uc_ipv4_session);

	/* free the flow table */
	ppa_memset(&(g_lro_table[sessionid]), 0, sizeof(ppa_lro_entry));
	lro_hal_dbg("%s succeeded on port:%d\n", __func__, sessionid);
	return PPA_SUCCESS;
}

static void get_lro_stats(PPA_HAL_STATS *stat)
{
	unsigned int cpu;
	const PPA_HAL_STATS *rt;

	memset(stat, 0, sizeof(PPA_HAL_STATS));
	stat->max_uc_session = MAX_LRO_ENTRIES;
	stat->max_uc_ipv4_session = -1;
	stat->max_uc_ipv6_session = -1;
	for_each_possible_cpu(cpu) {
		rt = per_cpu_ptr(&lrostats, cpu);
		stat->curr_uc_ipv4_session += rt->curr_uc_ipv4_session;
		stat->curr_uc_ipv6_session += rt->curr_uc_ipv6_session;
		stat->uc_dropped_sess += rt->uc_dropped_sess;
	}
}

/*!
	\fn void get_acc_mode(uint32_t *p_acc_mode)
	\ingroup PPA_lgm_lro_hal_GLOBAL_FUNCTIONS
	\brief get acceleration mode for interfaces (LAN/WAN)
	\param acc_mode a u32 data pointer to get acceleration mode
	\return no return value
 */
static inline void get_acc_mode(uint32_t *p_acc_mode)
{
	if (g_lro_enabled)
		*p_acc_mode = 1;
	else
		*p_acc_mode = 0;
}

/*!
	\fn void set_acc_mode(uint32_t acc_mode)
	\ingroup PPA_lgm_lro_hal_GLOBAL_FUNCTIONS
	\brief set LRO acceleration mode
	\param acc_mode acceleration mode
	\return no return value
*/
static inline void set_acc_mode(uint32_t acc_mode)
{
	if (acc_mode)
		g_lro_enabled = true;
	else
		g_lro_enabled = false;
}

static int32_t lgm_lro_hal_generic_hook(PPA_GENERIC_HOOK_CMD cmd, void *buffer, uint32_t flag)
{
	switch (cmd) {
	case PPA_GENERIC_HAL_INIT:
		init_lro_table();
		return PPA_SUCCESS;
	case PPA_GENERIC_HAL_EXIT:
		return PPA_SUCCESS;
	case PPA_GENERIC_HAL_GET_STATS:
	{
		PPA_HAL_STATS *stat = (PPA_HAL_STATS *)buffer;

		if (!stat)
			return PPA_FAILURE;

		get_lro_stats(stat);
		return PPA_SUCCESS;
	}
	case PPA_GENERIC_HAL_SET_ACC_ENABLE: {
		PPA_ACC_ENABLE *cfg = (PPA_ACC_ENABLE *)buffer;

		set_acc_mode(cfg->f_enable);
		return PPA_SUCCESS;
	}
	case PPA_GENERIC_HAL_GET_ACC_ENABLE: {
		PPA_ACC_ENABLE *cfg = (PPA_ACC_ENABLE *)buffer;

		get_acc_mode(&cfg->f_enable);
		return PPA_SUCCESS;
	}
	default:
		lro_hal_dbg("command:0x%0x not supported!\n", cmd);
		return PPA_FAILURE;
	}
}

static int proc_read_lro_accel(struct seq_file *seq, void *v)
{
	uint32_t accel_mode;

	if (!capable(CAP_SYSLOG)) {
		hal_err("Read Permission denied");
		return 0;
	}

	get_acc_mode(&accel_mode);
	seq_printf(seq, "LRO session learning	: %s\n",
		   (accel_mode ? "enabled" : "disabled"));
	return 0;
}

static int proc_read_lro_accel_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_lro_accel, NULL);
}

static ssize_t proc_set_lro_accel(struct file *file, const char __user *buf, size_t count, loff_t *data)
{
	int len;
	char str[40];
	char *p;

	if (!capable(CAP_NET_ADMIN)) {
		hal_err("Write Permission denied");
		return 0;
	}

	len = min(count, (size_t)(sizeof(str) - 1));
	len -= ppa_copy_from_user(str, buf, len);
	while (len && str[len - 1] <= ' ')
		len--;
	str[len] = 0;
	for (p = str; *p && *p <= ' '; p++, len--);
	if (!*p)
		return count;

	if (strncmp(p, "enable", 6) == 0) {
		set_acc_mode(1);
		hal_dbg("LRO session learning enabled!!!\n");
	} else if (strncmp(p, "disable", 7) == 0) {
		set_acc_mode(0);
		hal_dbg("LRO session learning disabled!!!\n");
	} else {
		hal_dbg("usage : echo <enable/disable> > <debugfs>/ppa/hal/lro/accel\n");
	}

	return len;
}

static int proc_read_lro_dbg(struct seq_file *seq, void *v)
{
	if (!capable(CAP_SYSLOG)) {
		hal_err("Read Permission denied");
		return 0;
	}
	seq_printf(seq, "LRO Debug	: %s\n",
		   (lro_hal_dbg_enable ? "enabled" : "disabled"));
	return 0;
}

static int proc_read_lro_dbg_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_lro_dbg, NULL);
}

static ssize_t proc_set_lro_dbg(struct file *file, const char __user *buf, size_t count, loff_t *data)
{
	int len;
	char str[40];
	char *p;

	if (!capable(CAP_NET_ADMIN)) {
		hal_err("Write Permission denied");
		return 0;
	}

	len = min(count, (size_t)(sizeof(str) - 1));
	len -= ppa_copy_from_user(str, buf, len);
	while (len && str[len - 1] <= ' ')
		len--;
	str[len] = 0;
	for (p = str; *p && *p <= ' '; p++, len--);
	if (!*p)
		return count;

	if (strncmp(p, "enable", 6) == 0) {
		lro_hal_dbg_enable = true;
		hal_dbg("LRO debug enabled\n");
	} else if (strncmp(p, "disable", 7) == 0) {
		lro_hal_dbg_enable = false;
		hal_dbg("LRO debug disabled\n");
	} else {
		hal_dbg("usage : echo <enable/disable> > <debugfs>/ppa/hal/lro/dbg\n");
	}

	return len;
}

static const struct file_operations dbgfs_file_lro_accel_seq_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_lro_accel_seq_open,
	.read		= seq_read,
	.write		= proc_set_lro_accel,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static const struct file_operations dbgfs_file_lro_dbg_seq_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_lro_dbg_seq_open,
	.read		= seq_read,
	.write		= proc_set_lro_dbg,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static struct ppa_debugfs_files lro_hal_debugfs_files[] = {
	{ "accel", 0600, &dbgfs_file_lro_accel_seq_fops },
	{ "dbg", 0600, &dbgfs_file_lro_dbg_seq_fops },
};

void init_lro_hal(void)
{
	ppa_drv_generic_hal_register(LRO_HAL, lgm_lro_hal_generic_hook);
	ppa_debugfs_create(ppa_hal_debugfs_dir_get(), "lro",
		&ppa_lrohal_debugfs_dir, lro_hal_debugfs_files,
		ARRAY_SIZE(lro_hal_debugfs_files));
}

void uninit_lro_hal(void)
{
	ppa_debugfs_remove(ppa_lrohal_debugfs_dir,
		lro_hal_debugfs_files,
		ARRAY_SIZE(lro_hal_debugfs_files));
	ppa_drv_generic_hal_deregister(LRO_HAL);
}
