// SPDX-License-Identifier: GPL-2.0
/*
 * P34X FW Download Driver
 *
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2019-2020, Intel Corporation.
 * Govindaiah Mudepalli <gmudepalli@maxlinear.com>
 * Xu Liang <lxu@maxlinear.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/platform_device.h>

#include "p34x_phy.h"
#include "p34x_phy_ptp.h"


static inline int mbox16_write32(struct p34x_priv_data *pdata, u32 idx,
				 u32 regaddr, u32 data)
{
	int ret;

	regaddr &= ~7;
	ret = mbox16_write(pdata, idx, regaddr, data & 0xFFFF);
	if (ret)
		return ret;
	return mbox16_write(pdata, idx, regaddr + 4, data >> 16);
}

static int ptp_adj_freq(struct ptp_clock_info *info, s32 ppb)
{
	struct slice_ptp *ptp = container_of(info, struct slice_ptp, info);
	struct p34x_priv_data *pdata = ptp->parent;
	const u32 idx = ptp->idx;
	u64 tmp;
	int ret;

	/*   addend = (update_freq << 32) / refclk
	 *          = (update_freq << 32) / (nominal_refclk / (1 + delta))
	 *          = def_addend * (1 + delta)
	 *          = def_addend * (10^9 + ppb) / 10^9 */
	tmp = (u64)ptp->def_addend * (1000000000 + ppb);
	tmp = div_u64(tmp, 1000000000);
	mbox16_write32(pdata, idx, 0xA33630, tmp);

	ret = mbox16_read(pdata, idx, 0xA33600);
	if (ret >= 0)
		mbox16_write(pdata, idx, 0xA33600, (u32)ret | 0x20);

	return 0;
}

static int ptp_adj_time(struct ptp_clock_info *info, s64 delta)
{
	struct slice_ptp *ptp = container_of(info, struct slice_ptp, info);
	struct p34x_priv_data *pdata = ptp->parent;
	const u32 idx = ptp->idx;
	int neg = 0;
	u32 sec, nsec;
	int ret;

	if (delta < 0) {
		delta = -delta;
		neg = 1;
	}

	sec = div_u64_rem(delta, NSEC_TO_SEC, &nsec);
	if (neg) {
		sec = 0x100000000ull - sec;
		nsec = 0xBB9ACA00 - nsec;
	}

	//spin_lock_bh(&ptp->lock);

	mbox16_write32(pdata, idx, 0xA33620, sec);
	mbox16_write32(pdata, idx, 0xA33628, nsec);

	ret = mbox16_read(pdata, idx, 0xA33600);
	if (ret >= 0)
		mbox16_write(pdata, idx, 0xA33600, (u32)ret | 8);

	//spin_unlock_bh(&ptp->lock);

	return 0;
}

static int ptp_get_time(struct ptp_clock_info *info, struct timespec64 *ts)
{
	struct slice_ptp *ptp = container_of(info, struct slice_ptp, info);
	struct p34x_priv_data *pdata = ptp->parent;
	const u32 idx = ptp->idx;
	int sech, seclh, secll, nsech, nsecl;

	//spin_lock_bh(&ptp->lock);
	sech = mbox16_read(pdata, idx, 0xA33638);
	secll = mbox16_read(pdata, idx, 0xA33610);
	seclh = mbox16_read(pdata, idx, 0xA33614);
	nsecl = mbox16_read(pdata, idx, 0xA33618);
	nsech = mbox16_read(pdata, idx, 0xA3361C);
	//spin_unlock_bh(&ptp->lock);

	if (sech < 0)
		return sech;
	if (seclh < 0)
		return seclh;
	if (secll < 0)
		return secll;
	if (nsech < 0)
		return nsech;
	if (nsecl < 0)
		return nsecl;

	ts->tv_sec = ((u64)sech << 32) | ((u32)seclh << 16) | secll;
	ts->tv_nsec = ((u32)nsech << 16) | nsecl;

	return 0;
}

static int ptp_set_time(struct ptp_clock_info *info, const struct timespec64 *ts)
{
	struct slice_ptp *ptp = container_of(info, struct slice_ptp, info);
	struct p34x_priv_data *pdata = ptp->parent;
	const u32 idx = ptp->idx;
	int ret;

	//spin_lock_bh(&ptp->lock);

	mbox16_write(pdata, idx, 0xA33638, ts->tv_sec >> 32);
	mbox16_write32(pdata, idx, 0xA33620, ts->tv_sec & 0xFFFFFFFF);
	mbox16_write32(pdata, idx, 0xA33628, ts->tv_nsec);

	ret = mbox16_read(pdata, idx, 0xA33600);
	if (ret >= 0)
		mbox16_write(pdata, idx, 0xA33600, (u32)ret | 4);

	//spin_unlock_bh(&ptp->lock);

	return 0;
}

static int ptp_tx_work(struct work_struct *work)
{
	//struct slice_ptp *ptp = container_of(work, struct slice_ptp, tx_work);

	return 1;
}

static int slice_reset_tx_fifo(struct slice_ptp *ptp)
{
	return 0;
}

static int slice_hw_init(struct slice_ptp *ptp)
{
	int ret;
	struct p34x_priv_data *pdata = ptp->parent;
	const u32 idx = ptp->idx;
	u32 inc, sub_sec_inc, sub_nsec_inc;
	u64 tmp;
	struct timespec64 now;

	/* Reset and enable clock for PTP modules */
	mbox16_write(pdata, idx, 0xA38058, 0x0035);
	mbox16_write(pdata, idx, 0xA38058, 0x00F5);

	/* Reset and enable timestamp FIFO */
	mbox16_write(pdata, idx, 0xA3805C, 0x0000);
	mbox16_write(pdata, idx, 0xA3805C, 0xC0C0);

	/* sub second/nanosecond increment */
	inc = ((NSEC_TO_SEC / 1000) << 8) / PTP_UPDATE_FREQ_KHZ;
	sub_sec_inc = inc >> 8;
	sub_nsec_inc = inc & 0xFF;
	mbox16_write(pdata, idx, 0xA33608, sub_nsec_inc << 8);
	mbox16_write(pdata, idx, 0xA3360C, sub_sec_inc);

	/* default addend
	 *   addend = 2^32 / (PTP Ref Clk / Update Freq)
	 *          = ((Update Freq) << 32) / (PTP Ref Clk)
	 */
	tmp = ((u64)PTP_UPDATE_FREQ_KHZ * 1000) << 32;
	ptp->def_addend = div_u64(tmp, PTP_REF_CLK);
	mbox16_write32(pdata, idx, 0xA33630, ptp->def_addend);

	/* Set and start the system time (PHC) */
	ret = mbox16_read(pdata, idx, 0xA33600);
	if (ret < 0)
		return ret;
	ktime_get_real_ts64(&now);
	mbox16_write(pdata, idx, 0xA33638, now.tv_sec >> 32);
	mbox16_write32(pdata, idx, 0xA33620, now.tv_sec & 0xFFFFFFFF);
	mbox16_write32(pdata, idx, 0xA33628, now.tv_nsec);
	mbox16_write(pdata, idx, 0xA33600, (u32)ret | 0x0227);

	return 0;
}

static int slice_ptp_init(struct slice_ptp *ptp)
{
	int ret;
	struct ptp_clock_info *info = &ptp->info;

	if (ptp->clock)
		/* initialized */
		return 0;

	spin_lock_init(&ptp->lock);

	slice_reset_tx_fifo(ptp);

	INIT_WORK(&ptp->tx_work, (work_func_t)ptp_tx_work);

	info->owner = THIS_MODULE;
	info->max_adj = MAX_FREQ_ADJUSTMENT;
	info->n_ext_ts = N_EXT_TS;
	info->adjfreq = ptp_adj_freq;
	info->adjtime = ptp_adj_time;
	info->gettime64 = ptp_get_time;
	info->settime64 = ptp_set_time;
	info->enable = NULL;
	ptp->clock = ptp_clock_register(info, &ptp->parent->pdev->dev);
	if (IS_ERR(ptp->clock)) {
		dev_err(&ptp->parent->pdev->dev,
			"ptp_clock_register failed: %lu\n",
			PTR_ERR(ptp->clock));
		ptp->clock = NULL;
		return -1;
	}

	ret = slice_hw_init(ptp);
	if (ret)
		return ret;

	// TODO: enable timestamp interrupt for ExtTS and Tx TS

	return 0;
}

static void slice_ptp_uninit(struct slice_ptp *ptp)
{
	if (!ptp->clock)
		return;

	cancel_work_sync(&ptp->tx_work);

	ptp_clock_unregister(ptp->clock);

	ptp->clock = NULL;
}

int p34x_ptp_init(struct p34x_priv_data *priv)
{
	int ret;
	size_t i;

	for (i = 0; i < ARRAY_SIZE(priv->ptp); i++) {
		priv->ptp[i].parent = priv;
		priv->ptp[i].idx = i;
		ret = slice_ptp_init(&priv->ptp[i]);
		if (ret)
			goto ERROR;
	}

	pr_err("p34x_ptp_init: successful\n");
	return 0;

ERROR:
	p34x_ptp_uninit(priv);
	return ret;
}

void p34x_ptp_uninit(struct p34x_priv_data *priv)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(priv->ptp); i++)
		slice_ptp_uninit(&priv->ptp[i]);
}

