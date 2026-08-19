/******************************************************************************
 *
 *  Copyright (c) 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "pon_img_debug.h"

uint8_t libponimg_dbg_lvl = DBG_ERR;

static void set(const uint8_t level)
{
	dbg_in_args("%u", level);
	libponimg_dbg_lvl = level;
	dbg_out();
}

static uint8_t get(void)
{
	dbg_in();
	dbg_out_ret("%d", libponimg_dbg_lvl);
	return libponimg_dbg_lvl;
}

struct pa_ll_dbg_lvl_ops libponimg_dbg_lvl_ops = {
	.set = set,
	.get = get
};
