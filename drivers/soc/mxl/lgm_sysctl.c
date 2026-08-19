// SPDX-License-Identifier: GPL-2.0
/*
 *  Copyright (C) 2022 MaxLinear, Inc.
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program;
 *
 *  LGM sysctl driver - exports few system control parameters via /proc/sys/
 *  Rahul Tanwar <rtanwar@maxlinear.com>
 */
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/io.h>
#include <linux/kernel_stat.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>
#include <linux/sysctl.h>
#include <linux/pp_api.h>
#include <linux/reboot.h>

#define CGU_PLL0B_CFG1_OFFSET	0x64
#define PLL_CLK1_DIV		0
#define PLL_CLK1_DIV_MASK	GENMASK(3, 0)
#define PLL_CLK1_DIV_SHIFT	0
#define PLL_CLK3_DIV		1
#define PLL_CLK3_DIV_MASK	GENMASK(11, 8)
#define PLL_CLK3_DIV_SHIFT	8
#define READ			0
#define WRITE			1

#define	DWC_DDR_UMCTL2		0xEF100000
#define	SWCTL_OFFSET		0x320
#define	PWRCTL_OFFSET		0x30
#define	PWRTMG_OFFSET		0x34
#define	PWR_IDLE_DFLT		0x0000F100
#define SET_SWCTL		1
#define CLR_SWCTL		0
#define SR_IDLE_TIME		0xa08f04
#define SR_DEFAULT		0x00000180
#define SR_ENABLED		0x1
#define SR_DISABLED		0x0
#define SR_ENA_MASK		0x1
#define SR_ENA_SHIFT		0x0
#define DDR_REG_SZ		0x400

#define CLK_DIV_50		0xE /* based on PLLB=2400MHz */
#define CLK_DIV_600		0x3 /* based on PLLB=2400MHz */
#define PPV4_IDLE		0x2 /* used in idle_state */
#define CPU_IDLE		0x4 /* used in idle_state */
#define SYSIDLE			0x6 /* bit mask idle sources to be checked */
#define PPV4_RX_PKTS_THRESHOLD	1000 /* max number of packets for idle */
#define CPU_LOAD_THRESHOLD	  15 /* 15% */
#define TIME_IDLE		5000 /* time to be below threshold [ms] */

struct sysidle_priv {
	unsigned int sysidle_p;
	unsigned int sysidle_t;
	unsigned int sysidle_period;
	unsigned int clk1_50_set_once;
	unsigned int clk1_600_set_once;
	unsigned int idle_state; /* bit1:ppv4, bit2:cpu */
	unsigned int time_idle;
	unsigned int first_time_idle;
	unsigned int sysidle_ppv4t;
	unsigned int ppv4_threshold;
	unsigned int sysidle_cput;
	unsigned int cpu_threshold;
	struct timespec64 stv;
	struct pp_hal_stats stats_ppv4_rx_prev;
};

static struct sysidle_priv s_priv = {
	.time_idle = TIME_IDLE,
	.cpu_threshold = CPU_LOAD_THRESHOLD, /* % */
	.ppv4_threshold = PPV4_RX_PKTS_THRESHOLD, /* max packets */
};

struct lgm_sysctl_priv {
	struct regmap *cgu_base;
	void __iomem *ddr_base;
	/* Might add more reg bases in future e.g. DDR base */
};

static unsigned int max_pll_clk_div = 0xf;
static unsigned int min_pll_clk_div;
static unsigned int active_pll_clk1_div;
static unsigned int active_pll_clk3_div;
static struct lgm_sysctl_priv priv;
static unsigned int ddr_pwr_ctrl;

struct activity_data {
	u64 last_used;
	u64 last_boot;
};

static struct activity_data activity_data;
static void sysctl_sysidle_work_fn(struct work_struct *work);
static DECLARE_DELAYED_WORK(sched_sysidle_work, sysctl_sysidle_work_fn);
static int lgm_sysctl_rw_pll_clk_div(int read_or_write, int clk_div);
static int sysctl_reboot_notifier(struct notifier_block *nb,
				  unsigned long action,
				  void *data);

static struct notifier_block sysctl_reboot_nb = {
	.notifier_call = sysctl_reboot_notifier,
	/* Bigger value moves subscriber higher on waiting list.
	 *  SYSCTL module will get notification about ongoing reboot earlier
	 *  compared to some others.
	 */
	.priority = 511,
};

static void sysidle_check_ppv4(struct sysidle_priv *s_priv)
{
	struct pp_hal_stats stats;
	int ret;
	long res;

	ret = pp_hal_stats_get(&stats);

	if (ret != 0)
		pr_info("ppv4 error: %i\n", ret);

	res = stats.rx_inline_packets - s_priv->stats_ppv4_rx_prev.rx_inline_packets;
	pr_debug("===> ppv4 ingress_packets = %lu\n", res);
	s_priv->stats_ppv4_rx_prev.rx_inline_packets = stats.rx_inline_packets;
	if (res < s_priv->ppv4_threshold) {
		s_priv->idle_state |= PPV4_IDLE;
	} else {
		pr_info("ppv4 is not idle\n");
		s_priv->idle_state &= ~PPV4_IDLE;
	}
}

static void sysidle_check_cpu(struct sysidle_priv *s_priv)
{
	int i, cpus;
	u64 curr_used;
	u64 curr_boot;
	s32 diff_used;
	s32 diff_boot;
	unsigned int usage;

	cpus = 0;
	curr_used = 0;

	for_each_online_cpu(i) {
		curr_used += kcpustat_cpu(i).cpustat[CPUTIME_USER]
			  +  kcpustat_cpu(i).cpustat[CPUTIME_NICE]
			  +  kcpustat_cpu(i).cpustat[CPUTIME_SYSTEM]
			  +  kcpustat_cpu(i).cpustat[CPUTIME_SOFTIRQ]
			  +  kcpustat_cpu(i).cpustat[CPUTIME_IRQ];
		cpus++;
	}

	curr_boot = ktime_get_boottime_ns() * cpus;
	diff_boot = (curr_boot - activity_data.last_boot) >> 16;
	diff_used = (curr_used - activity_data.last_used) >> 16;
	activity_data.last_boot = curr_boot;
	activity_data.last_used = curr_used;

	if (diff_boot <= 0 || diff_used < 0)
		usage = 0;
	else if (diff_used >= diff_boot)
		usage = 100;
	else
		usage = 100 * diff_used / diff_boot;

	pr_debug("===> curr cpu usage [%%] = %u\n", usage);

	if (usage < s_priv->cpu_threshold) {
		s_priv->idle_state |= CPU_IDLE;
	} else {
		pr_debug("cpu is not idle\n");
		s_priv->idle_state &= ~CPU_IDLE;
	}
}

static int sysidle_time_idle(struct sysidle_priv *s_priv)
{
	struct timespec64 etv;
	long usec;

	if (s_priv->first_time_idle == 0)
		return -1;
	ktime_get_real_ts64(&etv);
	usec = s_priv->stv.tv_sec * 1000000 + (s_priv->stv.tv_nsec / 1000);
	usec = (etv.tv_sec * 1000000 + (etv.tv_nsec / 1000)) - usec;
	if (s_priv->time_idle >= (usec / 1000))
		return -1;
	ktime_get_real_ts64(&s_priv->stv);
	return 0;
}

static void sysctl_sysidle_work_fn(struct work_struct *work)
{
	pr_debug("sysidle scheduled\n");

	if (s_priv.sysidle_period) {
		/* checking idleness */
		sysidle_check_ppv4(&s_priv);
		sysidle_check_cpu(&s_priv);
	}

	if (s_priv.idle_state == SYSIDLE && s_priv.first_time_idle == 0) {
		s_priv.first_time_idle = 1;
		ktime_get_real_ts64(&s_priv.stv);
	}

	if (s_priv.idle_state == SYSIDLE &&
	    s_priv.clk1_50_set_once == 0 &&
	    sysidle_time_idle(&s_priv) == 0) {
		pr_info("===== TOP_NOC =====> LOW\n");
		active_pll_clk1_div = CLK_DIV_50;
		lgm_sysctl_rw_pll_clk_div(WRITE, PLL_CLK1_DIV);
		s_priv.clk1_50_set_once = 1;
		s_priv.clk1_600_set_once = 0;
	} else if ((s_priv.idle_state != SYSIDLE) &&
		   (s_priv.clk1_600_set_once == 0)) {
		pr_info("===== TOP_NOC =====> HIGH\n");
		s_priv.first_time_idle = 0;
		active_pll_clk1_div = CLK_DIV_600;
		lgm_sysctl_rw_pll_clk_div(WRITE, PLL_CLK1_DIV);
		s_priv.clk1_50_set_once = 0;
		s_priv.clk1_600_set_once = 1;
	}

	if (s_priv.sysidle_period)
		schedule_delayed_work(&sched_sysidle_work,
				      msecs_to_jiffies(s_priv.sysidle_period));
}

static void lgm_sysctl_reinit(void)
{
	unsigned int ti = s_priv.time_idle;
	unsigned int ct = s_priv.cpu_threshold;
	unsigned int pt = s_priv.ppv4_threshold;

	memset(&s_priv, 0, sizeof(struct sysidle_priv));
	/* restore settings */
	s_priv.time_idle = ti;
	s_priv.cpu_threshold = ct;
	s_priv.ppv4_threshold = pt;
}

static int lgm_sysctl_sysidle_period(int read_or_write)
{
	if (read_or_write == READ) {
		s_priv.sysidle_p = s_priv.sysidle_period;
	} else { /* WRITE */
		if (s_priv.sysidle_p) {
			s_priv.sysidle_period = s_priv.sysidle_p;
			pr_info("sysidle enable\n");
			ktime_get_real_ts64(&s_priv.stv);
			memset(&s_priv.stats_ppv4_rx_prev, 0,
			       sizeof(struct pp_hal_stats));
			schedule_delayed_work(&sched_sysidle_work,
					      msecs_to_jiffies(s_priv.sysidle_period));
		} else if (s_priv.sysidle_p == SR_DISABLED) {
			lgm_sysctl_reinit();
			pr_info("sysidle disable\n");
		} else {
			pr_err("No valid sysidle mode.\n");
		}
	}
	return 0;
}

static int lgm_sysctl_sysidle_tidle(int read_or_write)
{
	if (read_or_write == READ)
		s_priv.sysidle_t = s_priv.time_idle;
	else /* WRITE */
		s_priv.time_idle = s_priv.sysidle_t;
	return 0;
}

static int lgm_sysidle_cputh(int read_or_write)
{
	if (read_or_write == READ)
		s_priv.sysidle_cput = s_priv.cpu_threshold;
	else /* WRITE */
		s_priv.cpu_threshold = s_priv.sysidle_cput;
	return 0;
}

static int lgm_sysidle_ppv4th(int read_or_write)
{
	if (read_or_write == READ)
		s_priv.sysidle_ppv4t = s_priv.ppv4_threshold;
	else /* WRITE */
		s_priv.ppv4_threshold = s_priv.sysidle_ppv4t;
	return 0;
}

static int lgm_sysctl_ddr_sfr_ctrl(int read_or_write)
{
	u32 data;

	if (read_or_write == READ) {
		data = ioread32(priv.ddr_base + PWRCTL_OFFSET);
		ddr_pwr_ctrl = (data & SR_ENA_MASK) >> SR_ENA_SHIFT;
	} else { /* WRITE */
		if (ddr_pwr_ctrl == SR_ENABLED) { /* Enable Selfrefresh */
			iowrite32(CLR_SWCTL, (priv.ddr_base + SWCTL_OFFSET));
			iowrite32(SR_IDLE_TIME, (priv.ddr_base + PWRTMG_OFFSET));
			iowrite32(SET_SWCTL, (priv.ddr_base + SWCTL_OFFSET));
			iowrite32(SR_DISABLED, (priv.ddr_base + PWRCTL_OFFSET));
			iowrite32(SR_ENABLED, (priv.ddr_base + PWRCTL_OFFSET));
		} else if (ddr_pwr_ctrl == SR_DISABLED) {
			iowrite32(CLR_SWCTL, (priv.ddr_base + SWCTL_OFFSET));
			iowrite32(PWR_IDLE_DFLT, (priv.ddr_base + PWRTMG_OFFSET));
			iowrite32(SET_SWCTL, (priv.ddr_base + SWCTL_OFFSET));
			iowrite32(SR_ENABLED, (priv.ddr_base + PWRCTL_OFFSET));
			iowrite32(SR_DEFAULT, (priv.ddr_base + PWRCTL_OFFSET));
		} else {
			pr_err("Not selected power save mode.\n");
		}
	}
	return 0;
}

static int proc_lgm_sysctl_sysidle_p(struct ctl_table *table, int write,
				     void *buffer, size_t *lenp, loff_t *ppos)
{
	int r;

	if (!write)
		lgm_sysctl_sysidle_period(READ);

	r = proc_dointvec_minmax(table, write, buffer, lenp, ppos);
	if (!write || r)
		return r;

	return lgm_sysctl_sysidle_period(WRITE);
}

static int proc_lgm_sysctl_sysidle_t(struct ctl_table *table, int write,
				     void *buffer, size_t *lenp, loff_t *ppos)
{
	int r;

	if (!write)
		lgm_sysctl_sysidle_tidle(READ);

	r = proc_dointvec_minmax(table, write, buffer, lenp, ppos);
	if (!write || r)
		return r;

	return lgm_sysctl_sysidle_tidle(WRITE);
}

static int proc_lgm_sysidle_cpu_th(struct ctl_table *table, int write,
				   void *buffer, size_t *lenp, loff_t *ppos)
{
	int r;

	if (!write)
		lgm_sysidle_cputh(READ);

	r = proc_dointvec_minmax(table, write, buffer, lenp, ppos);
	if (!write || r)
		return r;

	return lgm_sysidle_cputh(WRITE);
}

static int proc_lgm_sysidle_ppv4_th(struct ctl_table *table, int write,
				    void *buffer, size_t *lenp, loff_t *ppos)
{
	int r;

	if (!write)
		lgm_sysidle_ppv4th(READ);

	r = proc_dointvec_minmax(table, write, buffer, lenp, ppos);
	if (!write || r)
		return r;

	return lgm_sysidle_ppv4th(WRITE);
}

static int proc_lgm_sysctl_ddr_sfr(struct ctl_table *table, int write,
				   void *buffer, size_t *lenp, loff_t *ppos)
{
	int r;

	if (!write)
		lgm_sysctl_ddr_sfr_ctrl(READ);

	r = proc_dointvec_minmax(table, write, buffer, lenp, ppos);
	if (!write || r)
		return r;

	return lgm_sysctl_ddr_sfr_ctrl(WRITE);
}

static int lgm_sysctl_rw_pll_clk_div(int read_or_write, int clk_div)
{
	int ret = 0;
	u32 data;

	if (read_or_write == READ) {
		if (regmap_read(priv.cgu_base, CGU_PLL0B_CFG1_OFFSET, &data)) {
			pr_err("regmap read fails for CGU\n");
			return -EIO;
		}
		if (clk_div == PLL_CLK1_DIV)
			active_pll_clk1_div = (data & PLL_CLK1_DIV_MASK) >> PLL_CLK1_DIV_SHIFT;
		else // PLL_CLK3_DIV
			active_pll_clk3_div = (data & PLL_CLK3_DIV_MASK) >> PLL_CLK3_DIV_SHIFT;
	} else { // WRITE
		if (clk_div == PLL_CLK1_DIV)
			ret = regmap_update_bits(priv.cgu_base,
						 CGU_PLL0B_CFG1_OFFSET,
						 PLL_CLK1_DIV_MASK,
						 active_pll_clk1_div << PLL_CLK1_DIV_SHIFT);
		else // PLL_CLK3_DIV
			ret = regmap_update_bits(priv.cgu_base,
						 CGU_PLL0B_CFG1_OFFSET,
						 PLL_CLK3_DIV_MASK,
						 active_pll_clk3_div << PLL_CLK3_DIV_SHIFT);
	}
	return ret;
}

static int sysctl_reboot_notifier(struct notifier_block *nb,
				  unsigned long action,
				  void *data)
{
	lgm_sysctl_reinit();
	pr_info("sysidle disable\n");
	/* Notification handled, proceed with next callback. */
	return NOTIFY_OK;
}

static int proc_lgm_sysctl_pll_clk1_div(struct ctl_table *table, int write,
					void *buffer, size_t *lenp,
					loff_t *ppos)
{
	int r;

	if (!write) {
		r = lgm_sysctl_rw_pll_clk_div(READ, PLL_CLK1_DIV);
		if (r)
			return r;
	}

	r = proc_douintvec_minmax(table, write, buffer, lenp, ppos);
	if (!write || r)
		return r;

	return lgm_sysctl_rw_pll_clk_div(WRITE, PLL_CLK1_DIV);
}

static int proc_lgm_sysctl_pll_clk3_div(struct ctl_table *table, int write,
					void *buffer, size_t *lenp,
					loff_t *ppos)
{
	int r;

	if (!write) {
		r = lgm_sysctl_rw_pll_clk_div(READ, PLL_CLK3_DIV);
		if (r)
			return r;
	}

	r = proc_douintvec_minmax(table, write, buffer, lenp, ppos);
	if (!write || r)
		return r;

	return lgm_sysctl_rw_pll_clk_div(WRITE, PLL_CLK3_DIV);
}

static struct ctl_table lgm_sysctl_proc_table[] = {
	{
		.procname	= "pll_clk1_div",
		.data		= &active_pll_clk1_div,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_lgm_sysctl_pll_clk1_div,
		.extra1		= &min_pll_clk_div,
		.extra2		= &max_pll_clk_div,
	},
	{
		.procname	= "pll_clk3_div",
		.data		= &active_pll_clk3_div,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_lgm_sysctl_pll_clk3_div,
		.extra1		= &min_pll_clk_div,
		.extra2		= &max_pll_clk_div,
	},
	{
		.procname	= "ddr_selfrefresh",
		.data		= &ddr_pwr_ctrl,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_lgm_sysctl_ddr_sfr,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_ONE,
	},
	{
		.procname	= "sysidle_period",
		.data		= &s_priv.sysidle_p,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_lgm_sysctl_sysidle_p,
		.extra1		= SYSCTL_ZERO,
		.extra2		= NULL,
	},
	{
		.procname	= "sysidle_tidle",
		.data		= &s_priv.sysidle_t,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_lgm_sysctl_sysidle_t,
		.extra1		= SYSCTL_ZERO,
		.extra2		= NULL,
	},
	{
		.procname	= "sysidle_cpu_threshold",
		.data		= &s_priv.sysidle_cput,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_lgm_sysidle_cpu_th,
		.extra1		= SYSCTL_ZERO,
		.extra2		= NULL,
	},
	{
		.procname	= "sysidle_ppv4_threshold",
		.data		= &s_priv.sysidle_ppv4t,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_lgm_sysidle_ppv4_th,
		.extra1		= SYSCTL_ZERO,
		.extra2		= NULL,
	},
	/*
	 * Might add/export more params in future based
	 * on requirement & team's decision e.g. DDR base
	 */
	{}
};

static struct ctl_table lgm_sysctl_tbl[] = {
	{
		.procname	= "lgm_sysctl",
		.mode		= 0555,
		.child		= lgm_sysctl_proc_table
	},
	{}
};

static int __init lgm_init_sysctl_drv(void)
{
	struct ctl_table_header *lgm_sysctl_tblhdr;
	int ret;

	priv.cgu_base = syscon_regmap_lookup_by_compatible("intel,cgu-lgm");
	if (IS_ERR(priv.cgu_base))
		return PTR_ERR(priv.cgu_base);

	priv.ddr_base = ioremap(DWC_DDR_UMCTL2, DDR_REG_SZ);
	if (IS_ERR(priv.ddr_base))
		return PTR_ERR(priv.ddr_base);

	lgm_sysctl_tblhdr = register_sysctl_table(lgm_sysctl_tbl);
	if (!lgm_sysctl_tblhdr) {
		pr_err("Failed to register LGM sysctl\n");
		return -ENOMEM;
	}

	register_reboot_notifier(&sysctl_reboot_nb);

	ret = lgm_sysctl_rw_pll_clk_div(READ, PLL_CLK1_DIV);
	if (ret)
		return ret;

	return lgm_sysctl_rw_pll_clk_div(READ, PLL_CLK3_DIV);
}

late_initcall(lgm_init_sysctl_drv);

static void __exit lgm_exit_sysctl_drv(void)
{
	iounmap(priv.ddr_base);
	unregister_reboot_notifier(&sysctl_reboot_nb);
}

__exitcall(lgm_exit_sysctl_drv);
