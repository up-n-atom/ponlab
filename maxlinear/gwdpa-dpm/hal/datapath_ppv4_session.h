// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_PPV4_SESSION_H_
#define DATAPATH_PPV4_SESSION_H_
#include <net/datapath_api.h>
#include "../datapath.h"

#define DP_CLASS_OFFSET  28

enum GPID_TYPE {
	DP_RES_GPID = 0, /*   0 -  15, Dont Use, as it overlaps with LPID */
	DP_DYN_GPID,     /*  16 - 239, Dynamically allocated by DP */
	DP_SPL_GPID      /* 240 - 255, 16 Special GPID per DPID */
};

#define DP_RES_GPID_LPID_START	0
#define DP_RES_GPID_LPID_END	15
#define DP_DYN_GPID_START	16
#define DP_DYN_GPID_END		239
#define DP_SPL_GPID_START	240
#define DP_SPL_GPID_END		255

#define IS_SPECIAL_GPID(gpid)	\
	(((gpid >= DP_SPL_GPID_START) && (gpid <= DP_SPL_GPID_END)) ? 1 : 0)
/* Get Special GPID via DPID with fixed algo:
 * DPID    special_gpid
 * 15      255
 * 14      254
 * 13      253
 * ...
 * 10 .... 250
 */
#define SPL_GPID_VIA_DPID(dpid)	\
	(DP_SPL_GPID_END - (DP_DYN_GPID_START - 1 - dpid))
struct dp_dflt_hostif {
	int inst;
	int qid;
	int gpid;
	int color;
};

struct dp_session {
	int inst; /* reserved for future */
	int in_port; /* ingress GPID: Special GPID for this DC LPID.*/
	int eg_port; /* eg/dst GPID:  Actual GPID (DC LPID + subif) */
	int qid; /* physical qid */
	int class; /* traffic class */
	int vap;  /* same as subif_grp */
	u32 h1;  /* hash1 function result */
	u32 h2;  /* hash2 function result */
	u32 sig; /* signature */
	u32 sess_id; /* created session id */
};

#define dp_min_tx_pkt_len pp_min_tx_pkt_len

int alloc_gpid(int inst, enum GPID_TYPE type, int gpid_num, int dpid);
int free_gpid(int inst, int base, int gpid_num, int gpid_spl);
int get_dpid_from_gpid(int inst, int gpid);
int dp_add_dflt_hostif(struct dp_dflt_hostif *hostif, int flag);
int dp_add_pp_gpid(int inst, int dpid, int vap, int gpid, int spl_gpid,
		   u32 flag);
int dp_del_pp_gpid(int inst, int dpid, int vap);
int dp_subif_pp_set(int inst, int portid, int vap, u32 flag);
int dp_subif_pp_reset(int inst, int portid, int vap, u32 flag);
int dp_subif_pp_change_mtu(int inst, int dpid, int vap, u32 mtu);
int dp_del_default_egress_sess(struct dp_subif_info *p_subif,
			       int flag);
int dp_add_default_egress_sess(struct dp_session *sess, int flag);
int dp_voice_dflt_egress_session(int inst, int portid, int vap, u32 flag);
int dp_update_hostif(int inst, int dpid, int vap, int flag);
int dp_get_q_logic(int inst, int qid);
int dp_add_hostif(int inst, int dpid, int vap);
int dp_del_hostif(int inst, int dpid, int vap);

#endif
