// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2019 Intel Corporation.
 * Wu ZhiXian <wzhixian@maxlinear.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MXL_GPTC_TIMER_H
#define _MXL_GPTC_TIMER_H

#define GPTC_CLC	0x00
#define CLC_DIS		BIT(0)
#define CLC_SUSPEND	BIT(4)
#define CLC_RMC		10 /* divider 10 to produce 200 / 10 = 20 MHz clock */
#define CLC_RMC_SHIFT	8

#define GPTC_ID		0x08
#define ID_VER		0x1Fu
#define ID_VER_S	0
#define ID_CFG		0xE0u
#define ID_CFG_S	5
#define ID_ID		0xFF00u
#define ID_ID_S		8

#define GPTC_CON(X)	(0x10 + (X) * 0x20)
#define GPTC_CON_B(X)	(0x14 + (X) * 0x20)
#define CON_EN_STAT	BIT(0) /* RO only */
#define CON_COUNT_UP	BIT(1) /* Down up or down */
#define CON_CNT		BIT(2)
#define CON_ONESHOT	BIT(3) /* Stop or continue when overflowing */
#define CON_EXT		BIT(4) /* 32 or 16 bit */
#define CON_EDGE_RISE	BIT(6)
#define CON_EDGE_FALL	BIT(7)
#define CON_EDGE_ANY	(CON_EDGE_RISE | CON_EDGE_FALL)
#define CON_SYNC	BIT(8) /* Signal sync to module clock or not */

#define GPTC_RUN(X)	(0x18 + (X) * 0x20)
#define GPTC_RUN_B(X)	(0x1C + (X) * 0x20)
#define RUN_EN		BIT(0)
#define RUN_STOP	BIT(1)
#define RUN_RELOAD	BIT(2)

#define GPTC_RLD(X)	(0x20 + (X) * 0x20)
#define GPTC_RLD_B(X)	(0x24 + (X) * 0x20)
#define GPTC_CNT(X)	(0x28 + (X) * 0x20)
#define GPTC_CNT_B(X)	(0x2C + (X) * 0x20)

#define GPT_IRNENSTAT	0xEC
#define GPT_IRNENCLR	0xF0
#define GPTC_IRNEN		0xF4
#define GPTC_IRNICR		0xF8
#define GPTC_IRNCR		0xFC

/* RCU MACROs read WDT status and enable/disable WDT */
#define BASEIA_WDT_RST_EN				0x90
#define BASEIA_WDT_RST_EN_B0				0x1E0
#define WDT0		BIT(0)
#define WDT1		BIT(1)
#define WDT2		BIT(2)
#define WDT3		BIT(3)
#define BIA_WDT		BIT(6)

#define RST_STAT_TNOC					0x14

#ifdef CONFIG_MXL_LGM_GPTC_WDT
typedef void (*gptc_wdt_prewarn_cb)(void *);
extern int gptc_wdt_request(u32 cpuid, u32 flag, gptc_wdt_prewarn_cb prewarn_irq, void *data);
extern int gptc_wdt_release(u32 cpuid);
extern int gptc_wdt_start(u32 cpuid);
extern int gptc_wdt_stop(u32 cpuid);
extern int gptc_wdt_set_timeout(u32 cpuid, u32 timeout, u32 pretimeout);
extern u32 gptc_wdt_get_timeleft(u32 cpuid);
#endif

#endif
