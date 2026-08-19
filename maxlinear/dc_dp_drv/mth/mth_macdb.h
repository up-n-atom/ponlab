// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2022, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef _MTH_MACDB_H_
#define _MTH_MACDB_H_

enum mth_db_event_type {
	MTH_MACDB_ADD,
	MTH_MACDB_DEL,
	MTH_BRFDB_ADD,
	MTH_BRFDB_DEL,
};

struct mth_db_event_info {
	struct net_device *dev;
	struct net_device *realdev;
	char mac[ETH_ALEN];
};

int mth_register_db_notifier(struct notifier_block *nb);
int mth_unregister_db_notifier(struct notifier_block *nb);

#endif /* _MTH_MACDB_H_ */
