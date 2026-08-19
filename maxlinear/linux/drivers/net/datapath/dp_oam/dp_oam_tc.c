// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2024 MaxLinear, Inc.
 *
 ******************************************************************************/
#include <net/pkt_cls.h>
#include "dp_oam.h"

void dp_oam_tc_hook_fn_set(void)
{
	rtnl_lock();
	qos_tc_cfm_offload_fn = dp_oam_act_add;
	qos_tc_cfm_unoffload_fn = dp_oam_act_del;
	rtnl_unlock();
}

void dp_oam_tc_hook_fn_unset(void)
{
	rtnl_lock();
	qos_tc_cfm_offload_fn = NULL;
	qos_tc_cfm_unoffload_fn = NULL;
	rtnl_unlock();
}
