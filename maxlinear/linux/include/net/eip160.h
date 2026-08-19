/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Driver for MACsec Engine inside Intel CHD GSWIP or GPHY
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright 2019 - 2020 Intel Corporation
 */

#ifndef _EIP160_H_
#define _EIP160_H_

#include <linux/netdevice.h>
#include <net/macsec.h>

typedef int (*macsec_irq_handler_t)(struct macsec_context *);

struct eip160_pdev {
	/* section to be provided for eip160_register/eip160_unregister */
	struct macsec_context ctx;
	int (*prepare)(struct macsec_context *ctx, macsec_irq_handler_t ig,
		       macsec_irq_handler_t eg);
	void (*unprepare)(struct macsec_context *ctx);
	int (*ig_irq_ena)(struct macsec_context *ctx);
	int (*ig_irq_dis)(struct macsec_context *ctx);
	int (*eg_irq_ena)(struct macsec_context *ctx);
	int (*eg_irq_dis)(struct macsec_context *ctx);
	int (*eg_reg_rd)(struct macsec_context *ctx, u32 off, u32 *pdata);
	int (*eg_reg_wr)(struct macsec_context *ctx, u32 off, u32 data);
	int (*ig_reg_rd)(struct macsec_context *ctx, u32 off, u32 *pdata);
	int (*ig_reg_wr)(struct macsec_context *ctx, u32 off, u32 data);
	/*
	 * The number of SA match sets, flow control registers and SAs
	 * (transform record) supported.
	 */
	u32 ig_nr_of_sas;
	u32 eg_nr_of_sas;
};

extern int eip160_register(const struct eip160_pdev *pdev);
extern void eip160_unregister(const struct eip160_pdev *pdev);

#endif
