// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2019 Intel Corporation.
 * Lei Chuanhua <lchuanhua@maxlinear.com>
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

#include <linux/clk.h>
#include <linux/cpu.h>
#include <linux/debugfs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <clocksource/mxl-gptc-timer.h>
#ifdef CONFIG_X86
#include <linux/nmi.h>
#include <asm/nmi.h>
#endif

#define TIMER_PER_GPTC	3

#define BIT32TIMER	1
#define BIT16TIMER	0

#define BLOCKA		0
#define BLOCKB		1

#define MHZ(n)		((n) * 1000 * 1000)

static bool g_mxl_gptc_timer_loaded = false;

enum gptc_cnt_dir {
	GPTC_COUNT_DOWN = 0,
	GPTC_COUNT_UP,
};

enum gptc_timer_type {
	TIMER_TYPE_PREWARN,
	TIMER_TYPE_WDT,
	TIMER_TYPE_HEARTBEAT,
	TIMER_TYPE_SSO,
	TIMER_TYPE_MAX,
};

#define MAX_WDT_TIMER_PER_GPTC		2
struct wdttimer {
	u32 tid;
	u32 cpuid;
	u32 irqid;
};

struct mxl_match_data {
	u32 reg_type;
	bool msi;
};

struct gptc_struct;

struct gptc_timer {
	struct gptc_struct *gptc; /* Point back to parent */
	struct regmap *regmap;
	u32 gptcid;
	u32 tid; /* 0, 1, 2 only */
	u32 cpuid;
	u32 irqid;
	u32 irq;
	u32 type;
	u32 width;
	u32 block;
	u32 frequency;
	u32 cycles;
	enum gptc_cnt_dir dir;
	bool used;
	bool irq_registered;
	const struct mxl_match_data *soc_data;
	void (*irq_call_back)(void *);
	void *irq_call_back_param;
	struct list_head child; /* Node in parent list */
	struct list_head prewarn; /* Node in watchdog timer */
	struct list_head wdt; /* Node in watchdog timer */
	struct list_head heartbeat; /* Heartbeat */
};

/* Hardware GPTC struct */
struct gptc_struct {
	u32 id;
	struct device_node *np;
	struct dentry *debugfs;
	struct device *dev;
	struct regmap *regmap;
	struct clk *freqclk;
	struct clk *gateclk;
	u32 prewarn_num;
	u32 wdt_num;
	struct wdttimer prewarn[MAX_WDT_TIMER_PER_GPTC];
	struct wdttimer wdt[MAX_WDT_TIMER_PER_GPTC];
	u32 fpifreq;
	spinlock_t lock; /* Shared register access */
	struct list_head next; /* Link to next GPTC */
	const struct mxl_match_data *soc_data;
	struct gptc_timer timer[TIMER_PER_GPTC];
};

static LIST_HEAD(gptc_list);
static LIST_HEAD(gptc_prewarn_list);
static LIST_HEAD(gptc_wdt_list);
static LIST_HEAD(gptc_heartbeat_list);

static u32 __attribute__((unused))
gptc_read_counter(struct gptc_timer *timer)
{
	u32 ret;

	if (timer->width == BIT16TIMER && timer->block == BLOCKB)
		regmap_read(timer->regmap, GPTC_CNT_B(timer->tid), &ret);
	else
		regmap_read(timer->regmap, GPTC_CNT(timer->tid), &ret);

	return ret;
}

static inline void gptc_stop_counter(struct gptc_timer *timer)
{
	if (timer->width == BIT16TIMER && timer->block == BLOCKB)
		regmap_write(timer->regmap, GPTC_RUN_B(timer->tid), RUN_STOP);
	else
		regmap_write(timer->regmap, GPTC_RUN(timer->tid), RUN_STOP);
}

static inline u32 gptc_read_reload_counter(struct gptc_timer *timer)
{
	u32 ret;

	if (timer->width == BIT16TIMER && timer->block == BLOCKB)
		regmap_read(timer->regmap, GPTC_RLD_B(timer->tid), &ret);
	else
		regmap_read(timer->regmap, GPTC_RLD(timer->tid), &ret);

	return ret;
}

static inline void gptc_reload_counter(struct gptc_timer *timer,
				unsigned long cycles)
{
	if (timer->width == BIT16TIMER && cycles > U16_MAX)
		cycles = U16_MAX;
	else if (timer->width == BIT32TIMER && cycles > U32_MAX)
		cycles = U32_MAX;
	if (timer->width == BIT16TIMER && timer->block == BLOCKB)
		regmap_write(timer->regmap, GPTC_RLD_B(timer->tid), cycles);
	else
		regmap_write(timer->regmap, GPTC_RLD(timer->tid), cycles);
}

static inline void gptc_reset_counter(struct gptc_timer *timer)
{
	gptc_reload_counter(timer, 0);
}

static inline void gptc_start_counter(struct gptc_timer *timer)
{
	if (timer->width == BIT16TIMER && timer->block == BLOCKB)
		regmap_write(timer->regmap, GPTC_RUN_B(timer->tid), RUN_EN);
	else
		regmap_write(timer->regmap, GPTC_RUN(timer->tid), RUN_EN);
}

static inline void gptc_reload_and_run(struct gptc_timer *timer)
{
	if (timer->width == BIT16TIMER && timer->block == BLOCKB)
		regmap_write(timer->regmap, GPTC_RUN_B(timer->tid), RUN_EN | RUN_RELOAD);
	else
		regmap_write(timer->regmap, GPTC_RUN(timer->tid), RUN_EN | RUN_RELOAD);
}

static inline void gptc_clc_enable(struct gptc_struct *gptc)
{
	regmap_write(gptc->regmap, GPTC_CLC, CLC_SUSPEND | (CLC_RMC << CLC_RMC_SHIFT));
}

static inline void gptc_irq_mask_all(struct gptc_struct *gptc, u32 regtype)
{
	if (regtype)
		regmap_write(gptc->regmap, GPT_IRNENCLR, 0xFF);
	else
		regmap_write(gptc->regmap, GPTC_IRNEN, 0x00);
}

static inline void gptc_irq_clear_all(struct gptc_struct *gptc)
{
	regmap_write(gptc->regmap, GPTC_IRNCR, 0xFF);
}

static inline void gptc_irq_mask(struct gptc_timer *timer)
{
	unsigned long flags;
	struct gptc_struct *gptc = timer->gptc;

	if (timer->soc_data->reg_type) {
		regmap_write(timer->regmap, GPT_IRNENCLR,
				BIT((timer->tid * 2) + timer->block));
	} else {
		spin_lock_irqsave(&gptc->lock, flags);
		regmap_clear_bits(timer->regmap, GPTC_IRNEN,
				BIT((timer->tid * 2) + timer->block));
		spin_unlock_irqrestore(&gptc->lock, flags);
	}
}

static inline void gptc_irq_unmask(struct gptc_timer *timer)
{
	unsigned long flags;
	struct gptc_struct *gptc = timer->gptc;

	if (timer->soc_data->reg_type) {
		regmap_write(timer->regmap, GPTC_IRNEN,
				BIT((timer->tid * 2) + timer->block));
	} else {
		spin_lock_irqsave(&gptc->lock, flags);
		regmap_set_bits(timer->regmap, GPTC_IRNEN,
				BIT((timer->tid * 2) + timer->block));
		spin_unlock_irqrestore(&gptc->lock, flags);
	}
}

static inline void gptc_irq_ack(struct gptc_timer *timer)
{
	regmap_write(timer->regmap, GPTC_IRNCR,
			BIT((timer->tid * 2) + timer->block));
}

static inline int gptc_irq_read(struct gptc_timer *timer)
{
	u32 reg;

	regmap_read(timer->regmap, GPTC_IRNCR, &reg);
	if (reg & (BIT((timer->tid * 2) + timer->block)))
		return 1;
	else
		return 0;
}

static void gptc_set_32bit_timer(struct gptc_timer *timer)
{
	regmap_update_bits(timer->regmap, GPTC_CON(timer->tid), CON_EXT,
			(timer->width == BIT32TIMER) ? CON_EXT : 0);
}

static void gptc_count_dir(struct gptc_timer *timer)
{
	u32 reg = GPTC_CON(timer->tid);

	if (timer->width == BIT16TIMER && timer->block == BLOCKB)
		reg = GPTC_CON_B(timer->tid);

	regmap_update_bits(timer->regmap, reg, CON_COUNT_UP,
			(timer->dir == GPTC_COUNT_UP) ? CON_COUNT_UP : 0);
}

static void gptc_mode_setup(struct gptc_timer *timer, bool oneshot)
{
	u32 reg = GPTC_CON(timer->tid);

	if (timer->width == BIT16TIMER && timer->block == BLOCKB)
		reg = GPTC_CON_B(timer->tid);

	regmap_update_bits(timer->regmap, reg, CON_ONESHOT,
			oneshot ? CON_ONESHOT : 0);
}

static const char *const timer_type_to_str(u32 type)
{
	switch (type) {
	case TIMER_TYPE_PREWARN:
		return "prewarn";
	case TIMER_TYPE_WDT:
		return "WDT";
	case TIMER_TYPE_HEARTBEAT:
		return "heartbeat";
	case TIMER_TYPE_SSO:
		return "SSO";

	default:
		return "none";
	}
}

#if IS_ENABLED(CONFIG_MXL_LGM_GPTC_WDT)

static irqreturn_t gptc_prewarn_timer_interrupt(int irq, void *data)
{
	struct gptc_timer *timer = data;
	u32 reg;

	if (timer) {
		gptc_irq_mask(timer);
		gptc_irq_ack(timer);

		reg = gptc_read_reload_counter(timer);
		dev_err(timer->gptc->dev, "!!!Prewarn Timer IRQ %d!!! freq=%d RLD=%d timer period=%d\n",
			irq, timer->frequency, reg,
			reg * CLC_RMC / timer->frequency);

		/* Do the call back stuff */
		if (timer->irq_call_back)
			timer->irq_call_back(timer->irq_call_back_param);

		/* enable this pre-warning IRQ */
		gptc_irq_unmask(timer);
	}
	return IRQ_HANDLED;
}

#ifdef CONFIG_X86
static int gptc_wdt_pretimeout(unsigned int reason, struct pt_regs *regs)
{
	struct gptc_timer *timer;
	bool is_prewarn_nmi = false;

	list_for_each_entry(timer, &gptc_prewarn_list, prewarn) {

		if (gptc_irq_read(timer)) {
			/* Clear the prewarn interrupts */
			gptc_irq_ack(timer);
			is_prewarn_nmi = true;

			/* Do the call back stuff */
			if (timer->irq_call_back)
				timer->irq_call_back(timer->irq_call_back_param);

			}

		}
	if (!is_prewarn_nmi)
		return NMI_DONE;

	return NMI_HANDLED;
}

/* Core	pre-warning			pre-warn irq	WDT
 * 0	GPTC0-2 (TC0_2AB)	190				GPTC1-2 (TC1_2AB)
 * 1	GPTC0-3 (TC0_3AB)	185				GPTC1-3 (TC1_3AB)
 * 2	GPTC1-1 (TC1_1AB)	187				GPTC2-2 (TC2_2AB)
 * 3	GPTC2-1 (TC2_1AB))	191				GPTC2-3 (TC2_3AB)
 */
/* with cpu id to find wdt/prewarning timer and gptcid;
 */
static bool nmi_handler_registered;
#endif /*CONFIG_X86*/

int gptc_wdt_request(u32 cpuid, u32 flag, gptc_wdt_prewarn_cb prewarn_irq, void *prewarn_data)
{
	int ret = -1;
	struct gptc_timer *timer;
	struct gptc_struct *gptc;
	int wdt_gptcid = -1, prewarn_gptcid = -1;
	int wdt_tid = -1, prewarn_tid = -1;
	int prewarn_irqid = -1;

	struct device_node *np;
	int i;

	if (!g_mxl_gptc_timer_loaded)
		return -EPROBE_DEFER;

	list_for_each_entry(gptc, &gptc_list, next) {
		for (i = 0; i < gptc->wdt_num; i++) {
			if (gptc->wdt[i].cpuid == cpuid) {
				wdt_gptcid = gptc->id;
				wdt_tid = gptc->wdt[i].tid;
			}
		}
		for (i = 0; i < gptc->prewarn_num; i++) {
			if (gptc->prewarn[i].cpuid == cpuid) {
				prewarn_gptcid = gptc->id;
				prewarn_tid = gptc->prewarn[i].tid;
				prewarn_irqid = gptc->prewarn[i].irqid;
			}
		}
	}
	if (wdt_gptcid >=0 && prewarn_gptcid >=0 &&
	    wdt_gptcid == prewarn_gptcid && wdt_tid == prewarn_tid)
		return ret;


	/* flag is prewarning flag */
	if (flag) {
		list_for_each_entry(gptc, &gptc_list, next) {
			if (gptc->id != prewarn_gptcid)
				continue;

			timer = &gptc->timer[prewarn_tid];

			np = gptc->np;
			INIT_LIST_HEAD(&timer->child);
			timer->gptc = gptc;
			timer->regmap = gptc->regmap;
			timer->soc_data = gptc->soc_data;
			timer->gptcid = gptc->id;
			timer->tid = prewarn_tid;
			timer->irqid = prewarn_irqid;
			timer->type = TIMER_TYPE_PREWARN;
			timer->frequency = gptc->fpifreq;
			timer->width = BIT32TIMER;
			timer->used = true;
			timer->irq_call_back = prewarn_irq;
			timer->irq_call_back_param = prewarn_data;
			timer->irq = irq_of_parse_and_map(np, timer->irqid);
			timer->irq_registered = true;
			timer->dir = GPTC_COUNT_DOWN;
			timer->cpuid = cpuid;
			INIT_LIST_HEAD(&timer->prewarn);
			list_add_tail(&timer->prewarn, &gptc_prewarn_list);
			if (timer->soc_data->msi) {
#ifdef CONFIG_X86
				/* NMI MSI handler register only once */
				if (nmi_handler_registered)
					goto timer_setup;

				ret = register_nmi_handler(NMI_WATCHDOG, gptc_wdt_pretimeout, 0, "gptc_nmi_prewarn");
				if (ret) {
					dev_err(timer->gptc->dev, "Failed to register nmi interrupt\n");
					return -ENOENT;
				}
				nmi_handler_registered = true;
#else
				/* configuration error, should not be possible */
				return -ENOSYS;
#endif /*CONFIG_X86*/
			} else {
				/* enable this irq and mark this as gptc_wdt_prewarn_irq */
				ret = devm_request_irq(gptc->dev,
						timer->irq,
						gptc_prewarn_timer_interrupt,
						IRQF_TIMER | IRQF_NOBALANCING,
						"gptc_wdt_prewarn_irq",
						(void *)timer);
				if (ret) {
					dev_err(timer->gptc->dev, "%s: failed to request gptc_wdt_prewarn_irq - %d",
							__func__, timer->irq);
					return -ENOENT;
				}
			}
			irq_set_affinity(timer->irq, cpumask_of(timer->cpuid));
			timer->irq_registered = true;
timer_setup:
			gptc_count_dir(timer);
			gptc_set_32bit_timer(timer);
		}
	}

	ret = -1; /* this API returns success/failure of watchdog registration (not prewarn) so reset retcode here */
	list_for_each_entry(gptc, &gptc_list, next) {
		if (gptc->id == wdt_gptcid) {
			timer = &gptc->timer[wdt_tid];

			INIT_LIST_HEAD(&timer->child);
			timer->gptc = gptc;
			timer->regmap = gptc->regmap;
			timer->gptcid = gptc->id;
			timer->tid = wdt_tid;
			timer->soc_data = gptc->soc_data;
			timer->type = TIMER_TYPE_WDT;
			timer->frequency = gptc->fpifreq;
			timer->width = BIT32TIMER;
			timer->used = true;
			timer->irq_registered = false;
			timer->irq_call_back = NULL;
			timer->dir = GPTC_COUNT_DOWN;
			timer->cpuid = cpuid;
			INIT_LIST_HEAD(&timer->wdt);
			list_add_tail(&timer->wdt, &gptc_wdt_list);
			gptc_count_dir(timer);
			gptc_set_32bit_timer(timer);
			ret = 0;
		}
	}

	return ret;
}
EXPORT_SYMBOL(gptc_wdt_request);

int gptc_wdt_release(u32 cpuid)
{
	int ret = -1;
	struct gptc_timer *timer;

	list_for_each_entry(timer, &gptc_wdt_list, wdt) {
		if (timer->cpuid == cpuid) {
			gptc_irq_mask(timer);
			list_del(&timer->wdt);
			kfree(timer);
			ret = 0;
			break;
		}
	}

	list_for_each_entry(timer, &gptc_prewarn_list, prewarn) {
		if (timer->cpuid == cpuid) {
			gptc_irq_mask(timer);
			list_del(&timer->prewarn);
			kfree(timer);
			ret = 0;
			break;
		}
	}

	return ret;
}
EXPORT_SYMBOL(gptc_wdt_release);

int gptc_wdt_start(u32 cpuid)
{
	int ret = -1;
	struct gptc_timer *timer;

	list_for_each_entry(timer, &gptc_wdt_list, wdt) {
		if (timer->cpuid == cpuid) {
			gptc_irq_mask(timer);
			/*
			 * Clear any pending interrupts before
			 * starting watchdog timer.
			 */
			gptc_irq_ack(timer);
			gptc_mode_setup(timer, false);
			gptc_reload_counter(timer, timer->cycles);
			gptc_reload_and_run(timer);
			gptc_irq_unmask(timer);
			ret = 0;
			break;
		}
	}

	list_for_each_entry(timer, &gptc_prewarn_list, prewarn) {
		if (timer->cpuid == cpuid) {
			gptc_irq_mask(timer);
			/*
			 * Clear any pending interrupts before
			 * starting prewarn timer.
			 */
			gptc_irq_ack(timer);
			gptc_mode_setup(timer, false);
			gptc_reload_counter(timer, timer->cycles);
			gptc_reload_and_run(timer);
			gptc_irq_unmask(timer);
			ret = 0;
			break;
		}
	}

	return ret;
}
EXPORT_SYMBOL(gptc_wdt_start);

int gptc_wdt_stop(u32 cpuid)
{
	int ret = -1;
	struct gptc_timer *timer;

	list_for_each_entry(timer, &gptc_wdt_list, wdt) {
		if (timer->cpuid == cpuid) {
			gptc_irq_mask(timer); /* enable in wdt_start, disable in wdt_stop */
			gptc_stop_counter(timer);
			ret = 0;
			break;
		}
	}

	list_for_each_entry(timer, &gptc_prewarn_list, prewarn) {
		if (timer->cpuid == cpuid) {
			gptc_irq_mask(timer);
			gptc_stop_counter(timer);
			ret = 0;
			break;
		}
	}
	return ret;
}
EXPORT_SYMBOL(gptc_wdt_stop);

int gptc_wdt_set_timeout(u32 cpuid, u32 new_timeout, u32 new_pretimeout)
{
	int ret = -1;
	struct gptc_timer *timer;
	u64 cycles, interval, pre_interval;

	interval = new_timeout;
	pre_interval = new_timeout - new_pretimeout;

	list_for_each_entry(timer, &gptc_wdt_list, wdt) {
		if (timer->cpuid == cpuid) {
			cycles = interval * (timer->frequency / (u64)CLC_RMC);
			if (cycles > U32_MAX)
				cycles = U32_MAX;
			timer->cycles = cycles;
			gptc_reload_counter(timer, timer->cycles);
			ret = 0;
			break;
		}
	}

	list_for_each_entry(timer, &gptc_prewarn_list, prewarn) {
		if (timer->cpuid == cpuid) {
			cycles = pre_interval *
				 (timer->frequency / (u64)CLC_RMC);
			if ((cycles) > U32_MAX)
				cycles = U32_MAX;
			timer->cycles = cycles;
			gptc_reload_counter(timer, timer->cycles);
			ret = 0;
			break;
		}
	}
	return ret;
}
EXPORT_SYMBOL(gptc_wdt_set_timeout);

u32 gptc_wdt_get_timeleft(u32 cpuid)
{
	u32 ret = 0;
	struct gptc_timer *timer;
	unsigned long cycles, interval;

	list_for_each_entry(timer, &gptc_wdt_list, wdt) {
		if (timer->cpuid == cpuid) {
			/* interval is the unit of microsecond */
			cycles = gptc_read_counter(timer);
			interval = cycles / (timer->frequency / CLC_RMC);
			ret = interval;
			dev_dbg(timer->gptc->dev, "timeleft=%lu cycles=%lu\n",
				interval, cycles);
			break;
		}
	}
	return ret;
}
EXPORT_SYMBOL(gptc_wdt_get_timeleft);
#endif /* CONFIG_MXL_LGM_GPTC_WDT */

static irqreturn_t gptc_heartbeat_timer_interrupt(int irq, void *data)
{
	struct gptc_timer *timer = data;
	u32 reg;

	gptc_irq_mask(timer);
	gptc_irq_ack(timer);

	reg = gptc_read_reload_counter(timer);
	dev_dbg(timer->gptc->dev, "!!!Heart Beat Timer IRQ!!! freq=%d RLD=%d timer period=%d\n",
		timer->frequency, reg, reg * CLC_RMC / timer->frequency);
	gptc_irq_unmask(timer);
	return IRQ_HANDLED;
}

static void gptc_global_init(struct gptc_struct *gptc)
{
	gptc_clc_enable(gptc);
	gptc_irq_mask_all(gptc, gptc->soc_data->reg_type);
	gptc_irq_clear_all(gptc);
}

static void gptc_per_timer_init(struct gptc_timer *timer)
{
	gptc_count_dir(timer);
	gptc_set_32bit_timer(timer);
	if (timer->type == TIMER_TYPE_HEARTBEAT) {
		/* 1 second is timer->frequency/10, X second is timer->frequency/10 * X */
		gptc_reload_counter(timer, (timer->frequency / CLC_RMC) * 20);
		gptc_reload_and_run(timer);
	} else if (timer->type == TIMER_TYPE_SSO) {
		u32 cnt = timer->gptc->fpifreq;

		do_div(cnt, CLC_RMC * timer->frequency);
		if (cnt <= 1)
			cnt = 1;
		else
			cnt -= 1;
		gptc_reload_counter(timer, cnt);
		gptc_reload_and_run(timer);
	}
	else {
		gptc_reset_counter(timer);
	}
}

static void gptc_of_config_print(struct gptc_struct *gptc)
{
	int i = 0;
	struct gptc_timer *timer;

	for (i = 0; i < TIMER_PER_GPTC; i++) {
		timer = &gptc->timer[i];
		dev_dbg(gptc->dev, "timer%d gptcid %u freq %u tid %u cpuid %u irq %u clk %s\n",
			 i, timer->gptcid, timer->frequency,
			 timer->tid, timer->cpuid, timer->irq,
			 timer_type_to_str(timer->type));
	}
}

static int gptc_clock_init(struct gptc_struct *gptc)
{
	struct device *dev = gptc->dev;

	gptc->gateclk = devm_clk_get_enabled(dev, "gptc");
	if (IS_ERR(gptc->gateclk)) {
		dev_err(dev, "Failed to get gptc gate clk: %ld\n",
			PTR_ERR(gptc->gateclk));
		return PTR_ERR(gptc->gateclk);
	}

	gptc->freqclk = devm_clk_get_enabled(dev, "freq");
	if (IS_ERR(gptc->freqclk)) {
		dev_err(gptc->dev, "Failed to get gptc frequency clk: %ld\n",
			PTR_ERR(gptc->freqclk));
		return PTR_ERR(gptc->freqclk);
	}

	gptc->fpifreq = clk_get_rate(gptc->freqclk);

	return 0;
}

static void gptc_of_parse_sso_freq(struct gptc_timer *timer)
{
	struct gptc_struct *gptc = timer->gptc;
	struct device_node *np;
	const __be32 *prop;

	prop = of_get_property(gptc->np, "mxl,sso", NULL);
	if (!prop) {
		dev_info(gptc->dev, "no sso phandle, use default frequency!\n");
		goto __sso_node_err;
	}

	np = of_find_node_by_phandle(be32_to_cpup(prop));
	if (!np) {
		dev_err(gptc->dev, "Can't find ssoled node\n");
		goto __sso_node_err;
	}

	if (of_property_read_u32(np, "mxl,gptc-clkrate", &timer->frequency)) {
		dev_err(gptc->dev, "Cannot find mxl,gptc-clkrate in phandle\n");
		goto __sso_node_err;
	}

	return;

__sso_node_err:
	timer->frequency = MHZ(10);
	return;
}

static int gptc_of_parse_timer(struct gptc_struct *gptc)

{
	u32 type;
	struct of_phandle_args clkspec;
	int index, ret, nr_timers;
	struct gptc_timer *timer;
	u32 tid;
	u32 cpuid;
	u32 width = BIT32TIMER;
	u32 block = BLOCKA;
	struct device_node *np = gptc->np;
	int i = 0, j = 0;

	if (!of_property_read_u32_index(np, "prewarn-num", 0, &gptc->prewarn_num)) {
		dev_dbg(gptc->dev, "gptc%d: prewarn timer num =%d\n",
			gptc->id, gptc->prewarn_num);
		if (gptc->prewarn_num > MAX_WDT_TIMER_PER_GPTC) {
			dev_err(gptc->dev, "gptc%d: prewarn timer num =%d\n",
				gptc->id, gptc->prewarn_num);
			return -EINVAL;
		}
		for (i = 0; i < gptc->prewarn_num; i++) {
			if (!of_property_read_u32_index(np, "prewarn-timer", j, &gptc->prewarn[i].tid)) {
				dev_dbg(gptc->dev, "gptc%d: prewarn timer =%d\n",
					gptc->id, gptc->prewarn[i].tid);
			} else {
				dev_err(gptc->dev, "gptc%d: no prewarn timer defined in DTS at %s\n",
					gptc->id, np->full_name);
			}
			j++;
			if (!of_property_read_u32_index(np, "prewarn-timer", j, &gptc->prewarn[i].cpuid)) {
				dev_dbg(gptc->dev, "gptc%d: prewarn timer cpuid=%d\n",
					gptc->id, gptc->prewarn[i].cpuid);
			} else {
				dev_err(gptc->dev, "gptc%d: no prewarn timer defined in DTS at %s\n",
					gptc->id, np->full_name);
			}
			j++;
			if (!of_property_read_u32_index(np, "prewarn-timer", j, &gptc->prewarn[i].irqid)) {
				dev_dbg(gptc->dev, "gptc%d: prewarn timer irqid=%d\n",
					gptc->id, gptc->prewarn[i].irqid);
			} else {
				dev_err(gptc->dev, "gptc%d: no prewarn timer defined in DTS at %s\n",
					gptc->id, np->full_name);
			}
			j++;
		}
	} else {
		dev_dbg(gptc->dev, "gptc%d: no prewarn timer defined in DTS at %s\n",
			gptc->id, np->full_name);
	}

	j = 0;
	if (!of_property_read_u32_index(np, "wdt-num", 0, &gptc->wdt_num)) {
		dev_dbg(gptc->dev, "gptc%d: wdt timer num =%d\n",
			gptc->id, gptc->wdt_num);
		if (gptc->wdt_num > MAX_WDT_TIMER_PER_GPTC) {
			dev_err(gptc->dev, "gptc%d: wdt timer num =%d\n",
				gptc->id, gptc->wdt_num);
			return -EINVAL;
		}
		for (i = 0; i < gptc->wdt_num; i++) {
			if (!of_property_read_u32_index(np, "wdt-timer", j, &gptc->wdt[i].tid)) {
				dev_dbg(gptc->dev, "gptc%d: wdt timer =%d\n",
					gptc->id, gptc->wdt[i].tid);
			} else {
				dev_err(gptc->dev, "gptc%d: no wdt timer defined in DTS at %s\n",
					gptc->id, np->full_name);
			}
			j++;
			if (!of_property_read_u32_index(np, "wdt-timer", j, &gptc->wdt[i].cpuid)) {
				dev_dbg(gptc->dev, "gptc%d: wdt timer cpuid=%d\n",
					gptc->id, gptc->wdt[i].cpuid);
			} else {
				dev_err(gptc->dev, "gptc%d: no wdt timer defined in DTS at %s\n",
					gptc->id, np->full_name);
			}
			j++;
		}
	} else {
		dev_dbg(gptc->dev, "gptc%d: no wdt timer defined in DTS at %s\n",
			gptc->id, np->full_name);
	}

	nr_timers = of_count_phandle_with_args(np, "mxl,clk", "#gptc-cells");
	if (nr_timers <= 0) {
		dev_dbg(gptc->dev, "gptc%d: no timer defined in DTS at %s\n",
			gptc->id, np->full_name);
		return 0;
	}
	for (index = 0; index < nr_timers; index++) {
		ret = of_parse_phandle_with_args(np, "mxl,clk", "#gptc-cells",
						 index, &clkspec);
		if (ret < 0)
			return ret;
		dev_dbg(gptc->dev, "%s args_count %d arg[0] %d arg[1] %d arg[2] %d arg[3] %d arg[4] %d\n",
			 __func__, clkspec.args_count, clkspec.args[0],
			 clkspec.args[1], clkspec.args[2],
			 clkspec.args[3], clkspec.args[4]);

		if (clkspec.args_count == 3) {
			width = BIT32TIMER;
			dev_dbg(gptc->dev, "%s: Default 32 bit timer\n", __func__);
		} else if (clkspec.args_count == 5) {
			if (clkspec.args[3] == 16)
				width = BIT16TIMER;
			block = clkspec.args[4];
			/* Only Block A/B in 16 bit timer */
			if (block > 1) {
				dev_err(gptc->dev, "%s: invalid gptc clk property\n", __func__);
				return -EINVAL;
			}
		} else {
			dev_err(gptc->dev, "%s: invalid gptc clk property\n", __func__);
			return -EINVAL;
		}

		type = clkspec.args[0];
		tid = clkspec.args[1];
		cpuid = clkspec.args[2];
		if (clkspec.args_count == 3)
			dev_dbg(gptc->dev, "%s type %d tid %d cpuid %d\n",
				 __func__, type, tid, cpuid);
		else
			dev_dbg(gptc->dev, "%s type %d tid %d cpuid %d width %d block %d\n",
				 __func__, type, tid, cpuid, clkspec.args[3], clkspec.args[4]);

		/* Ignore CPU id check */
		if (type > TIMER_TYPE_MAX || tid > (TIMER_PER_GPTC - 1)) {
			dev_err(gptc->dev, "%s invalid clk type %d or timer id %d\n",
				__func__, type, tid);
			return -EINVAL;
		}

		timer = &gptc->timer[tid];

		INIT_LIST_HEAD(&timer->child);
		timer->gptc = gptc;
		timer->regmap = gptc->regmap;
		timer->gptcid = gptc->id;
		timer->cpuid = cpuid;
		timer->tid = tid;
		timer->type = type;
		timer->soc_data = gptc->soc_data;
		timer->width = width;
		timer->block = block;
		timer->frequency = gptc->fpifreq;
		timer->used = false;
		timer->irq_registered = false;
		timer->irq_call_back = NULL;
		switch (type) {
		case TIMER_TYPE_HEARTBEAT:
		case TIMER_TYPE_SSO:
			INIT_LIST_HEAD(&timer->heartbeat);
			timer->irq = irq_of_parse_and_map(np, timer->tid + timer->block);
			WARN_ON(timer->irq <= 0);
			timer->dir = GPTC_COUNT_DOWN;
			list_add_tail(&timer->heartbeat, &gptc_heartbeat_list);
			if (type == TIMER_TYPE_SSO)
				gptc_of_parse_sso_freq(timer);
			break;
		default:
			break;
		}
	}
	return 0;
}

const static struct regmap_config gptc_regmap_config = {
	.reg_bits = 32,
	.reg_stride = 4,
	.val_bits = 32,
	.disable_locking = true,
};

static int gptc_of_init(struct platform_device *pdev, struct gptc_struct *gptc)

{
	int ret;
	u32 gptcid;
	void __iomem *base;
	struct regmap *regmap;
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	const struct mxl_match_data *match_data = of_device_get_match_data(dev);

	/* Which GPTC is being handled */
	gptcid = of_alias_get_id(np, "timer");
	base = devm_of_iomap(dev, np, 0, NULL);
	if (IS_ERR(base)) {
		dev_err(dev, "Can't map GPTC base address\n");
		return PTR_ERR(base);
	}
	regmap = regmap_init_mmio(dev, base, &gptc_regmap_config);
	if (IS_ERR(regmap)) {
		dev_err(dev, "Failed to init regmap\n");
		return PTR_ERR(regmap);
	}
	INIT_LIST_HEAD(&gptc->next);
	spin_lock_init(&gptc->lock);
	gptc->np = np;
	gptc->dev = dev;
	gptc->id = gptcid;
	gptc->soc_data = match_data;
	gptc->prewarn_num = 0;
	gptc->wdt_num = 0;
	gptc->regmap = regmap;

	ret = gptc_clock_init(gptc);
	if (ret)
		return ret;
	ret = gptc_of_parse_timer(gptc);
	if (ret)
		return ret;
	list_add_tail(&gptc->next, &gptc_list);
	/* GPTC level initialization */
	gptc_global_init(gptc);
	gptc_of_config_print(gptc);
	return 0;
}

static int gptc_heartbeat_init(void)
{
	struct gptc_timer *timer;
	int ret;

	list_for_each_entry(timer, &gptc_heartbeat_list, heartbeat) {
		if (!timer->used) {
			gptc_per_timer_init(timer);
			timer->used = true;
			gptc_irq_unmask(timer);
			gptc_reload_and_run(timer);
			if (!timer->irq_registered) {
				ret = devm_request_irq(timer->gptc->dev,
						timer->irq,
						gptc_heartbeat_timer_interrupt,
						IRQF_TIMER | IRQF_NOBALANCING,
						NULL,
						(void *)timer);
				if (ret) {
					dev_err(timer->gptc->dev, "gptc irq %d register failed, ret=%d\n",
						timer->irq, ret);
					break;
				}
				irq_set_affinity(timer->irq, cpumask_of(timer->cpuid));
				timer->irq_registered = true;
				gptc_irq_unmask(timer);
				gptc_reload_and_run(timer);
				gptc_mode_setup(timer, false);
				dev_dbg(timer->gptc->dev, "gptc %d timer %d irq %d register @cpu%d\n",
					timer->gptcid, timer->tid, timer->irq, timer->cpuid);
			}
		}
	}
	return -EINVAL;
}

static inline u32 gptc_dbg_read(struct gptc_struct *gptc, u32 reg)
{
	u32 val;
	regmap_read(gptc->regmap, reg, &val);
	return val;
}

static int gptc_status_show(struct seq_file *s, void *v)
{
	int i = 0;
	struct gptc_struct *gptc = s->private;
	struct gptc_timer *timer;

	if (!gptc) {
		seq_printf(s, "error\n");
		return 0;
	}

	seq_printf(s, "GPTC%u freq %u\n",
			gptc->id, gptc->fpifreq);
	seq_printf(s, "CLC %08x ID %08x IRNEN %08x IRNICR %08x IRNCR %08x\n",
			gptc_dbg_read(gptc, GPTC_CLC),
			gptc_dbg_read(gptc, GPTC_ID),
			gptc_dbg_read(gptc, GPT_IRNENSTAT),
			gptc_dbg_read(gptc, GPTC_IRNICR),
			gptc_dbg_read(gptc, GPTC_IRNCR));

	for (i = 0; i < TIMER_PER_GPTC; i++) {
		timer = &gptc->timer[i];
		seq_printf(s, "\ttimer%d freq %u tid %u cpuid %u irq %u clk %s %s %s\n",
				i, timer->frequency, timer->tid,
				timer->cpuid, timer->irq,
				timer_type_to_str(timer->type),
				(timer->width == BIT32TIMER) ? "BIT32Timer" : "BIT16Timer",
				timer->used ? "used" : "unused");
		if (timer->width == BIT16TIMER && timer->block == BLOCKB)
			seq_printf(s, "\tCON %08x RUN %08x RLD %08x CNT %08x\n",
				gptc_dbg_read(gptc, GPTC_CON_B(i)),
				gptc_dbg_read(gptc, GPTC_RUN_B(i)),
				gptc_dbg_read(gptc, GPTC_RLD_B(i)),
				gptc_dbg_read(gptc, GPTC_CNT_B(i)));
		else
			seq_printf(s, "\tCON %08x RUN %08x RLD %08x CNT %08x\n",
				gptc_dbg_read(gptc, GPTC_CON(i)),
				gptc_dbg_read(gptc, GPTC_RUN(i)),
				gptc_dbg_read(gptc, GPTC_RLD(i)),
				gptc_dbg_read(gptc, GPTC_CNT(i)));
	}
	seq_putc(s, '\n');
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(gptc_status);

static int gptc_debugfs_init(struct platform_device *pdev)
{
	struct dentry *file;
	struct dentry *debugfs;
	struct device *dev = &pdev->dev;
	struct gptc_struct *priv = platform_get_drvdata(pdev);

	priv->debugfs = debugfs_create_dir(dev_name(dev), NULL);
	debugfs = priv->debugfs;
	if (!debugfs)
		return -ENOMEM;

	file = debugfs_create_file("status", 0400, debugfs, priv, &gptc_status_fops);
	if (!file)
		goto remove;

	return 0;
remove:
	debugfs_remove_recursive(debugfs);
	debugfs = NULL;
	return -ENOMEM;
}

static int mxl_gptc_timer_drv_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct gptc_struct *drvdata;
	int ret;

	if (!np)
		return -ENODEV;
	drvdata = devm_kzalloc(dev, sizeof(*drvdata), GFP_KERNEL);
	if (IS_ERR(drvdata))
		return -ENOMEM;

	ret = gptc_of_init(pdev, drvdata);
	if (ret < 0) {
		dev_err(dev, "Error: GPTC initialize error\n");
		return ret;
	}
	gptc_heartbeat_init();

	platform_set_drvdata(pdev, drvdata);
	g_mxl_gptc_timer_loaded = true;

#ifdef CONFIG_DEBUG_FS
	gptc_debugfs_init(pdev);
#endif
	dev_info(dev, "MxL GPTC Timer driver loaded.\n");
	return 0;
}

static int mxl_gptc_timer_drv_remove(struct platform_device *pdev)
{
	struct gptc_struct *drvdata = platform_get_drvdata(pdev);
#ifdef CONFIG_DEBUG_FS
	debugfs_remove_recursive(drvdata->debugfs);
#endif
	dev_info(&pdev->dev, "MxL GPTC Timer driver remove!\n");
	return 0;
}

static struct mxl_match_data lgm_b = {
	.reg_type = 1,
	.msi = true,
};

static struct mxl_match_data lgm = {
	.reg_type = 1,
	.msi = false,
};

static struct mxl_match_data prx = {
	.reg_type = 0,
	.msi = false,
};

static const struct of_device_id mxl_gptc_timer_drv_match[] = {
	{ .compatible = "mxl,gptc-b", .data = &lgm_b},
	{ .compatible = "mxl,lgm-b-gptc", .data = &lgm_b},
	{ .compatible = "lgm,gptc", .data = &lgm},
	{ .compatible = "prx300,gptc", .data = &prx},
	{ .compatible = "mxl,prx300-gptc", .data = &prx},
	{}
};

static struct platform_driver mxl_gptc_timer_driver = {
	.driver = {
		.name = "mxl-gptc",
		.of_match_table = mxl_gptc_timer_drv_match,
		.owner = THIS_MODULE,
	},
	.probe = mxl_gptc_timer_drv_probe,
	.remove = mxl_gptc_timer_drv_remove,
};

module_platform_driver(mxl_gptc_timer_driver);

MODULE_LICENSE("GPL v2");
