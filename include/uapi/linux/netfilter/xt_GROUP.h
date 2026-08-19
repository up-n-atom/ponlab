/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _XT_GROUP_H
#define _XT_GROUP_H

#include <net/sgam_api.h>

/* For fetching SGC/TBM info from SGAM */
struct xt_group_info {
	char name[MAX_LABEL_LEN + 1];
	__u8 flags;
	__u8 grp;
	__u16 id;
	__u8 remark_dscp;
};
#endif /* _XT_GROUP_H */
