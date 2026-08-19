// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_SPL_CONN_H_
#define DATAPATH_SPL_CONN_H_
#include <net/datapath_api.h>

struct dp_spl_cfg;

int _dp_spl_conn(int inst, struct dp_spl_cfg *conn);

int _dp_spl_conn_get(int inst, enum DP_SPL_TYPE type,
		       struct dp_spl_cfg *conns, u8 cnt);

int dp_spl_conn_get_num_of_egps(int inst, int egps[16]);

const char *dp_spl_conn_get_name_from_egp(int inst, int egp_id);

int dp_is_spl_conn(int inst, int egp);

#endif /* end of include guard: DATAPATH_SPL_CONN_H_ */
