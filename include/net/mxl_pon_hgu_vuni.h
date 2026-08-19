/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2023 MaxLinear, Inc.
 *
 *****************************************************************************/
#ifndef _MXL_PON_HGU_VUNI_
#define _MXL_PON_HGU_VUNI_

/**
 * brief Get used port ID as well as owner information
 * param[in] interface Interface number
 * param[out] dp_port_id Pointer to data path port ID
 * param[out] owner Pointer to kernel module owner
 * return 0 / -EINVAL
 */
int mxl_vuni_drv_iphost_info_get(unsigned int interface, int *dp_port_id,
				 struct module **owner);

#endif
