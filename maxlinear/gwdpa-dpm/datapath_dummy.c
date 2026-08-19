// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/
#include <linux/module.h>

/* With latest UGW we found gwdpa-dpm will be compiled in bootcore model
*  So here we just add workaround to make build work
*  of course, if UGW build system issue is fixed, this workaround is not used
*  at all
*/
static volatile int dp_dummy_mode = 0;
static int __init dp_dummy_init(void)
{
	dp_dummy_mode = 1;
	return 0;
}

static void __exit dp_dummy_exit(void)
{
}

module_init(dp_dummy_init);
module_exit(dp_dummy_exit);
MODULE_LICENSE("GPL");

