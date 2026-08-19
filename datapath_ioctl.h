// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_IOCTL_H
#define DATAPATH_IOCTL_H

#include <net/datapath_api.h>
#include "datapath.h"
#include "datapath_instance.h"

int dp_register_ptp_ioctl(struct net_device *dev, int reset);
int dp_ops_set(void **dev_ops, const int ndo_offset, size_t ops_size,
		    void **dp_saved_ops, void *dp_new_ops,
		    const void *ndo_cb, int *ops_cnt,int *all_ops_cnt, u32 flag);
void *dp_ndo_ops_get(void *ops_base, const int offset);

#endif /*DATAPATH_IOCTL_H*/

