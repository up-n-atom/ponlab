/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/* Generic header file for Session base Group Accounting and Metering(SGAM).
 * This provides all structs,IOCTLs etc used in SGAM.
 *
 * Copyright (C) 2022-2024 MaxLinear, Inc.
 */
#ifndef _UAPI_SGAM_API_H
#define _UAPI_SGAM_API_H

#include <linux/types.h>

#define MAX_LABEL_LEN 15
#define SGAM_IOC_MAGIC ((__u32)'s')
#define SGAM_DEV_NAME "sgam_dev"

#define SGAM_ACCT_DB_FLAG (1 << 0)
#define SGAM_METER_DB_FLAG (1 << 1)
#define SGAM_DB_MASK (SGAM_ACCT_DB_FLAG | SGAM_METER_DB_FLAG)
#define IS_ACCT_DB(flag) (flag & SGAM_ACCT_DB_FLAG)
#define IS_METER_DB(flag) (flag & SGAM_METER_DB_FLAG)

#define SGAM_SUCCESS		0   /*!< Operation was successful. */
#define SGAM_FAILURE		-1  /*!< Operation failed */

enum sgam_meter_types {
	SGAM_TBM_MODE_FIRST,
	/* RFC 4115 Color blind mode, i.e.,  dtrtcm_cb */
	SGAM_TBM_MODE_4115_CB = SGAM_TBM_MODE_FIRST,
	/* RFC 4115 Color aware mode, i.e., dtrtcm_ca */
	SGAM_TBM_MODE_4115_CA,
	/* RFC 2698 Color blind mode, i.e., trtcm_cb*/
	SGAM_TBM_MODE_2698_CB,
	/* RFC 2698 Color aware mode, i.e., trtcm_ca */
	SGAM_TBM_MODE_2698_CA,
	SGAM_TBM_MODE_LAST = SGAM_TBM_MODE_2698_CA,
	SGAM_TBM_MODES_NUM,
};

struct sgam_stats_info {
	__u64 packets; /*! number of packets */
	__u64 bytes; /*! number of bytes */
	__u64 ing_dropped_bytes; /* number of ingress dropped bytes */
	__u64 egr_dropped_bytes; /* number of egress dropped bytes */
};

enum sgam_ioctl_cmd_nr {
	SGAM_LIST_CMD_NR = 0,
	SGAM_DEL_CMD_NR,
	SGAM_ADD_ACCOUNT_NR,
	SGAM_ADD_METER_NR,
	SGAM_SHOW_ACCOUNT_NR,
	SGAM_SHOW_METER_NR,
	SGAM_GET_COUNT_NR,
	SGAM_IOC_MAX_NR
};

/* Group account related structs */
struct sgam_acct_info {
	char name[MAX_LABEL_LEN + 1];
	__u8 sgc_grp; /* PP_SGC_LVL_0 to PP_SGC_LVL_7 */
	__u16 sgc_id;
};

/* Group meter related structs */
struct sgam_meter_info {
	char name[MAX_LABEL_LEN + 1];
	__u16 tbm_id;
	__u64 cir;
	__u64 pir;
	__u32 cbs;
	__u32 pbs;
	__u8 remark_dscp;
	enum sgam_meter_types mtr_type;
};

/* Structs used for IOCTLs */
struct sgam_count_info {
	int count;
	__u8 db_flag;
};

struct sgam_name_list {
	char name[MAX_LABEL_LEN + 1];
};

struct sgam_list_info {
	struct sgam_count_info count_info;
	struct sgam_name_list namelist[1];
};

struct sgam_del_info {
	char name[MAX_LABEL_LEN + 1];
	__u8 sgc_grp;
	__u16 sgc_id;
	__u8 db_flag;
	__u16 ref_count;
};

struct sgam_cmd_acct_stats {
	char name[MAX_LABEL_LEN + 1];		/* Name of the account */
	__u8 sgc_grp;				/* Account pool */
	__u16 sgc_id;
	struct sgam_stats_info acct_stat;	/* Struct for HW stats */
};

/* IOCTLs */
#define SGAM_LIST_CMD _IOWR(SGAM_IOC_MAGIC, SGAM_LIST_CMD_NR, struct sgam_list_info)

#define SGAM_GET_COUNT_CMD _IOWR(SGAM_IOC_MAGIC, SGAM_GET_COUNT_NR, struct sgam_count_info)

#define SGAM_DEL_CMD _IOWR(SGAM_IOC_MAGIC, SGAM_DEL_CMD_NR, struct sgam_del_info)

#define SGAM_ADD_ACCOUNT _IOWR(SGAM_IOC_MAGIC, SGAM_ADD_ACCOUNT_NR, struct sgam_acct_info)

#define SGAM_ADD_METER _IOWR(SGAM_IOC_MAGIC, SGAM_ADD_METER_NR, struct sgam_meter_info)

#define SGAM_SHOW_ACCOUNT _IOWR(SGAM_IOC_MAGIC, SGAM_SHOW_ACCOUNT_NR, struct sgam_cmd_acct_stats)

#define SGAM_SHOW_METER _IOWR(SGAM_IOC_MAGIC, SGAM_SHOW_METER_NR, struct sgam_meter_info)

#endif /* _UAPI_SGAM_API_H */
