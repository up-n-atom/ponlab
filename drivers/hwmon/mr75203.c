// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020 MaxLinear, Inc.
 *
 * This driver is a hardware monitoring driver for PVT controller
 * (MR75203) which is used to configure & control Moortec embedded
 * analog IP to enable multiple embedded temperature sensor(TS),
 * voltage monitor(VM) & process detector(PD) modules.
 */
#include <linux/bitfield.h>
#include <linux/clk.h>
#include <linux/debugfs.h>
#include <linux/hwmon.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/mutex.h>
#include <linux/platform_data/lgm_epu.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/regmap.h>
#include <linux/reset.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/units.h>

/* PVT Common register */
#define PVT_IP_CONFIG	0x04
#define TS_NUM_MSK	GENMASK(4, 0)
#define TS_NUM_SFT	0
#define PD_NUM_MSK	GENMASK(12, 8)
#define PD_NUM_SFT	8
#define VM_NUM_MSK	GENMASK(20, 16)
#define VM_NUM_SFT	16
#define CH_NUM_MSK	GENMASK(31, 24)
#define CH_NUM_SFT	24

#define VM_NUM_MAX	(VM_NUM_MSK >> VM_NUM_SFT)

#define PVT_IRQ_EN	0x40
#define TS_IRQ_EN_MSK	BIT(1)
#define IRQ_EN		0x1

#define IRQ_TS_MSK	0x54
#define IRQ_TS_STAT	0x64

/* Macro Common Register */
#define CLK_SYNTH		0x00
#define CLK_SYNTH_LO_SFT	0
#define CLK_SYNTH_HI_SFT	8
#define CLK_SYNTH_HOLD_SFT	16
#define CLK_SYNTH_EN		BIT(24)
#define CLK_SYS_CYCLES_MAX	514
#define CLK_SYS_CYCLES_MIN	2

#define SDIF_DISABLE	0x04

#define SDIF_STAT	0x08
#define SDIF_BUSY	BIT(0)
#define SDIF_LOCK	BIT(1)

#define SDIF_W		0x0c
#define SDIF_PROG	BIT(31)
#define SDIF_WRN_W	BIT(27)
#define SDIF_WRN_R	0x00
#define SDIF_ADDR_SFT	24

#define SDIF_HALT	0x10
#define SDIF_CTRL	0x14
#define SDIF_SMPL_CTRL	0x20

/* TS & PD Individual Macro Register */
#define COM_REG_SIZE	0x40

#define TP_IRQ_EN(n)	(COM_REG_SIZE + 0x40 * (n))
#define TP_IRQ_STAT(n)	(COM_REG_SIZE + 0x4 + 0x40 * (n))
#define TP_IRQ_CLR(n)	(COM_REG_SIZE + 0x8 + 0x40 * (n))
#define IRQ_ALARMA_MSK	BIT(3)
#define IRQ_ALARMB_MSK	BIT(4)

#define SDIF_DONE(n)		(COM_REG_SIZE + 0x14 + 0x40 * (n))
#define SDIF_SMPL_DONE		BIT(0)

#define SDIF_DATA(n)	(COM_REG_SIZE + 0x18 + 0x40 * (n))
#define SAMPLE_DATA_MSK	GENMASK(15, 0)

#define TP_ALARMA_CFG(n)	(COM_REG_SIZE + 0x20 + 0x40 * (n))
#define TP_ALARMB_CFG(n)	(COM_REG_SIZE + 0x24 + 0x40 * (n))
#define HYST_THRESH_MSK		GENMASK(15, 0)
#define ALARM_THRESH_MSK	GENMASK(31, 16)

#define HILO_RESET(n)	(COM_REG_SIZE + 0x2c + 0x40 * (n))

/* VM Individual Macro Register */
#define VM_COM_REG_SIZE	0x200
#define VM_SDIF_DONE(vm)	(VM_COM_REG_SIZE + 0x34 + 0x200 * (vm))
#define VM_SDIF_DATA(vm, ch)	\
	(VM_COM_REG_SIZE + 0x40 + 0x200 * (vm) + 0x4 * (ch))

/* SDA Slave Register */
#define IP_CTRL			0x00
#define IP_RST_REL		BIT(1)
#define IP_RUN_CONT		BIT(3)
#define IP_AUTO			BIT(8)
#define IP_VM_MODE		BIT(10)

#define IP_CFG			0x01
#define CFG0_MODE_2		BIT(0)
#define CFG0_PARALLEL_OUT	0
#define CFG0_12_BIT		0
#define CFG1_VOL_MEAS_MODE	0
#define CFG1_PARALLEL_OUT	0
#define CFG1_14_BIT		0

#define IP_DATA		0x03

#define IP_POLL		0x04
#define VM_CH_INIT	BIT(20)
#define VM_CH_REQ	BIT(21)

#define IP_TMR			0x05
#define POWER_DELAY_CYCLE_256	0x100
#define POWER_DELAY_CYCLE_64	0x40

#define PVT_POLL_DELAY_US	20
#define PVT_POLL_TIMEOUT_US	20000
#define PVT_CONV_BITS		10
#define PVT_N_CONST		90
#define PVT_R_CONST		245805

#define PVT_TEMP_MIN_mC		-40000
#define PVT_TEMP_MAX_mC		125000

/* Temperature coefficients for series 5 */
#define PVT_SERIES5_H_CONST	200000
#define PVT_SERIES5_G_CONST	60000
#define PVT_SERIES5_J_CONST	-100
#define PVT_SERIES5_CAL5_CONST	4094

/* Temperature coefficients for series 6 */
#define PVT_SERIES6_H_CONST	249400
#define PVT_SERIES6_G_CONST	57400
#define PVT_SERIES6_J_CONST	0
#define PVT_SERIES6_CAL5_CONST	4096

#define TEMPERATURE_SENSOR_SERIES_5	5
#define TEMPERATURE_SENSOR_SERIES_6	6

#define PRE_SCALER_X1	1
#define PRE_SCALER_X2	2

/**
 * struct voltage_device - VM single input parameters.
 * @vm_map: Map channel number to VM index.
 * @ch_map: Map channel number to channel index.
 * @pre_scaler: Pre scaler value (1 or 2) used to normalize the voltage output
 *              result.
 *
 * The structure provides mapping between channel-number (0..N-1) to VM-index
 * (0..num_vm-1) and channel-index (0..ch_num-1) where N = num_vm * ch_num.
 * It also provides normalization factor for the VM equation.
 */
struct voltage_device {
	u32 vm_map;
	u32 ch_map;
	u32 pre_scaler;
};

/**
 * struct voltage_channels - VM channel count.
 * @total: Total number of channels in all VMs.
 * @max: Maximum number of channels among all VMs.
 *
 * The structure provides channel count information across all VMs.
 */
struct voltage_channels {
	u32 total;
	u8 max;
};

struct temp_coeff {
	u32 h;
	u32 g;
	u32 cal5;
	s32 j;
};

struct pvt_device {
	struct regmap		*c_map;
	struct regmap		*t_map;
	struct regmap		*p_map;
	struct regmap		*v_map;
	struct device		*dev;
	struct clk		*clk;
	struct reset_control	*rst;
	struct dentry		*dbgfs_dir;
	struct voltage_device	*vd;
	struct voltage_channels	vm_channels;
	struct temp_coeff	ts_coeff;
	u32			t_num;
	u32			p_num;
	u32			v_num;
	u32			c_num;
	u32			ip_freq;
#if IS_ENABLED(CONFIG_DEBUG_FS)
	int			irq;
	u32			int_en;
	u32			irq_cnt;
	struct delayed_work	work;
	u32			irq_test_cnt;
#endif
};

static ssize_t pvt_ts_coeff_j_read(struct file *file, char __user *user_buf,
				   size_t count, loff_t *ppos)
{
	struct pvt_device *pvt = file->private_data;
	unsigned int len;
	char buf[13];

	len = scnprintf(buf, sizeof(buf), "%d\n", pvt->ts_coeff.j);

	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static ssize_t pvt_ts_coeff_j_write(struct file *file,
				    const char __user *user_buf,
				    size_t count, loff_t *ppos)
{
	struct pvt_device *pvt = file->private_data;
	int ret;

	ret = kstrtos32_from_user(user_buf, count, 0, &pvt->ts_coeff.j);
	if (ret)
		return ret;

	return count;
}

static const struct file_operations pvt_ts_coeff_j_fops = {
	.read = pvt_ts_coeff_j_read,
	.write = pvt_ts_coeff_j_write,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

static void devm_pvt_ts_dbgfs_remove(void *data)
{
	struct pvt_device *pvt = (struct pvt_device *)data;

	debugfs_remove_recursive(pvt->dbgfs_dir);
	pvt->dbgfs_dir = NULL;
}

static int pvt_ts_dbgfs_create(struct pvt_device *pvt, struct device *dev)
{
	pvt->dbgfs_dir = debugfs_create_dir(dev_name(dev), NULL);

	debugfs_create_u32("ts_coeff_h", 0644, pvt->dbgfs_dir,
			   &pvt->ts_coeff.h);
	debugfs_create_u32("ts_coeff_g", 0644, pvt->dbgfs_dir,
			   &pvt->ts_coeff.g);
	debugfs_create_u32("ts_coeff_cal5", 0644, pvt->dbgfs_dir,
			   &pvt->ts_coeff.cal5);
	debugfs_create_file("ts_coeff_j", 0644, pvt->dbgfs_dir, pvt,
			    &pvt_ts_coeff_j_fops);

	return devm_add_action_or_reset(dev, devm_pvt_ts_dbgfs_remove, pvt);
}

static umode_t pvt_is_visible(const void *data, enum hwmon_sensor_types type,
			      u32 attr, int channel)
{
	switch (type) {
	case hwmon_temp:
		if (attr == hwmon_temp_input)
			return 0444;
		break;
	case hwmon_in:
		if (attr == hwmon_in_input)
			return 0444;
		break;
	default:
		break;
	}
	return 0;
}

static long pvt_calc_temp(struct pvt_device *pvt, u32 nbs)
{
	/*
	 * Convert the register value to degrees centigrade temperature:
	 * T = G + H * (n / cal5 - 0.5) + J * F
	 */
	struct temp_coeff *ts_coeff = &pvt->ts_coeff;

	s64 tmp = ts_coeff->g +
		div_s64(ts_coeff->h * (s64)nbs, ts_coeff->cal5) -
		ts_coeff->h / 2 +
		div_s64(ts_coeff->j * (s64)pvt->ip_freq, HZ_PER_MHZ);

	return clamp_val(tmp, PVT_TEMP_MIN_mC, PVT_TEMP_MAX_mC);
}

static int pvt_read_temp(struct device *dev, u32 attr, int channel, long *val)
{
	struct pvt_device *pvt = dev_get_drvdata(dev);
	struct regmap *t_map = pvt->t_map;
	u32 stat, nbs;
	int ret;

	switch (attr) {
	case hwmon_temp_input:
		ret = regmap_read_poll_timeout(t_map, SDIF_DONE(channel),
					       stat, stat & SDIF_SMPL_DONE,
					       PVT_POLL_DELAY_US,
					       PVT_POLL_TIMEOUT_US);
		if (ret)
			return ret;

		ret = regmap_read(t_map, SDIF_DATA(channel), &nbs);
		if (ret < 0)
			return ret;

		nbs &= SAMPLE_DATA_MSK;

		/*
		 * Convert the register value to
		 * degrees centigrade temperature
		 */
		*val = pvt_calc_temp(pvt, nbs);

		return 0;
	default:
		return -EOPNOTSUPP;
	}
}

static int pvt_read_in(struct device *dev, u32 attr, int channel, long *val)
{
	struct pvt_device *pvt = dev_get_drvdata(dev);
	struct regmap *v_map = pvt->v_map;
	u32 n, stat, pre_scaler;
	u8 vm_idx, ch_idx;
	int ret;

	if (channel >= pvt->vm_channels.total)
		return -EINVAL;

	vm_idx = pvt->vd[channel].vm_map;
	ch_idx = pvt->vd[channel].ch_map;

	switch (attr) {
	case hwmon_in_input:
		ret = regmap_read_poll_timeout(v_map, VM_SDIF_DONE(vm_idx),
					       stat, stat & SDIF_SMPL_DONE,
					       PVT_POLL_DELAY_US,
					       PVT_POLL_TIMEOUT_US);
		if (ret)
			return ret;

		ret = regmap_read(v_map, VM_SDIF_DATA(vm_idx, ch_idx), &n);
		if (ret < 0)
			return ret;

		n &= SAMPLE_DATA_MSK;
		pre_scaler = pvt->vd[channel].pre_scaler;
		/*
		 * Convert the N bitstream count into voltage.
		 * To support negative voltage calculation for 64bit machines
		 * n must be cast to long, since n and *val differ both in
		 * signedness and in size.
		 * Division is used instead of right shift, because for signed
		 * numbers, the sign bit is used to fill the vacated bit
		 * positions, and if the number is negative, 1 is used.
		 * BIT(x) may not be used instead of (1 << x) because it's
		 * unsigned.
		 */
		*val = pre_scaler * (PVT_N_CONST * (long)n - PVT_R_CONST) /
			(1 << PVT_CONV_BITS);

		return 0;
	default:
		return -EOPNOTSUPP;
	}
}

static int pvt_read(struct device *dev, enum hwmon_sensor_types type,
		    u32 attr, int channel, long *val)
{
	switch (type) {
	case hwmon_temp:
		return pvt_read_temp(dev, attr, channel, val);
	case hwmon_in:
		return pvt_read_in(dev, attr, channel, val);
	default:
		return -EOPNOTSUPP;
	}
}

static struct hwmon_channel_info pvt_temp = {
	.type = hwmon_temp,
};

static struct hwmon_channel_info pvt_in = {
	.type = hwmon_in,
};

static const struct hwmon_ops pvt_hwmon_ops = {
	.is_visible = pvt_is_visible,
	.read = pvt_read,
};

static struct hwmon_chip_info pvt_chip_info = {
	.ops = &pvt_hwmon_ops,
};

static u32 pvt_calc_reg(struct pvt_device *pvt, s64 temp)
{
	struct temp_coeff *ts_coeff = &pvt->ts_coeff;
	s64 val;

	val = clamp_val((s64)temp, PVT_TEMP_MIN_mC, PVT_TEMP_MAX_mC);
	val = val - ts_coeff->g + (ts_coeff->h >> 1) -
	      div_s64(ts_coeff->j * (s64)pvt->ip_freq, HZ_PER_MHZ);
	val = div_s64(val * ts_coeff->cal5, ts_coeff->h);

	return (u32)val;
}

static void pvt_isr_init(struct pvt_device *pvt)
{
	struct regmap *c_map = pvt->c_map;
	struct regmap *t_map = pvt->t_map;
	struct device *dev = pvt->dev;
	u32 val, t_num = pvt->t_num;
	u32 alarm_a, hsyt_a, alarm_b, hsyt_b;
	int i;

	if (device_property_read_u32(dev, "rising-alarm", &alarm_a)) {
		dev_info(dev,
			 "rising alarm is not specified, the default value 85 degree is specified\n");
		alarm_a = 85000;
	}

	if (device_property_read_u32(dev, "rising-hsyt", &hsyt_a)) {
		dev_info(dev,
			 "rising hyst is not specified, the default value 80 degree is specified\n");
		hsyt_a = 80000;
	}

	if (device_property_read_u32(dev, "falling-alarm", &alarm_b)) {
		dev_info(dev,
			 "rising alarm is not specified, the default value 80 degree is specified\n");
		alarm_b = 80000;
	}

	if (device_property_read_u32(dev, "falling-hsyt", &hsyt_b)) {
		dev_info(dev,
			 "rising alarm is not specified, the default value 85 degree is specified\n");
		hsyt_b = 85000;
	}

	regmap_write(c_map, PVT_IRQ_EN, FIELD_PREP(TS_IRQ_EN_MSK, IRQ_EN));
	regmap_write(c_map, IRQ_TS_MSK, GENMASK(t_num - 1, NR_CPUS));

	for (i = 0; i < t_num; i++) {
		regmap_write(t_map, TP_IRQ_CLR(i), 0xffffffff);

		if (i >= NR_CPUS)
			break;

		regmap_write(t_map, TP_IRQ_EN(i), 0x0);

		regmap_update_bits(t_map, TP_IRQ_EN(i), IRQ_ALARMA_MSK,
				   FIELD_PREP(IRQ_ALARMA_MSK, IRQ_EN));

		val = pvt_calc_reg(pvt, hsyt_a);
		regmap_update_bits(t_map, TP_ALARMA_CFG(i), HYST_THRESH_MSK,
				   FIELD_PREP(HYST_THRESH_MSK, val));

		val = pvt_calc_reg(pvt, alarm_a);
		regmap_update_bits(t_map, TP_ALARMA_CFG(i), ALARM_THRESH_MSK,
				   FIELD_PREP(ALARM_THRESH_MSK, val));

		val = pvt_calc_reg(pvt, hsyt_b);
		regmap_update_bits(t_map, TP_ALARMB_CFG(i), HYST_THRESH_MSK,
				   FIELD_PREP(HYST_THRESH_MSK, val));

		val = pvt_calc_reg(pvt, alarm_b);
		regmap_update_bits(t_map, TP_ALARMB_CFG(i), ALARM_THRESH_MSK,
				   FIELD_PREP(ALARM_THRESH_MSK, val));
	}
}

#if IS_ENABLED(CONFIG_DEBUG_FS)
static int rising_alarm_set(void *data, u64 val)
{
	struct pvt_device *pvt = data;
	u32 i, alarm_a;

	alarm_a = pvt_calc_reg(pvt, (s64)val);

	for (i = 0; i < pvt->t_num; i++) {
		regmap_update_bits(pvt->t_map, TP_ALARMA_CFG(i),
				   ALARM_THRESH_MSK,
				   FIELD_PREP(ALARM_THRESH_MSK, alarm_a));
	}

	return 0;
}

static int rising_alarm_get(void *data, u64 *val)
{
	struct pvt_device *pvt = data;
	u32 alarm_a;

	regmap_read(pvt->t_map, TP_ALARMA_CFG(0), &alarm_a);
	*val = pvt_calc_temp(pvt, FIELD_GET(ALARM_THRESH_MSK, alarm_a));

	return 0;
}

static int rising_hyst_set(void *data, u64 val)
{
	struct pvt_device *pvt = data;
	u32 i, hyst_a;

	hyst_a = pvt_calc_reg(pvt, (s64)val);

	for (i = 0; i < pvt->t_num; i++) {
		regmap_update_bits(pvt->t_map, TP_ALARMA_CFG(i),
				   HYST_THRESH_MSK,
				   FIELD_PREP(HYST_THRESH_MSK, hyst_a));
	}

	return 0;
}

static int rising_hyst_get(void *data, u64 *val)
{
	struct pvt_device *pvt = data;
	u32 hyst_a;

	regmap_read(pvt->t_map, TP_ALARMA_CFG(0), &hyst_a);
	*val = pvt_calc_temp(pvt, FIELD_GET(HYST_THRESH_MSK, hyst_a));

	return 0;
}

static int falling_alarm_set(void *data, u64 val)
{
	struct pvt_device *pvt = data;
	u32 i, alarm_b;

	alarm_b = pvt_calc_reg(pvt, (s64)val);
	for (i = 0; i < pvt->t_num; i++) {
		regmap_update_bits(pvt->t_map, TP_ALARMB_CFG(i),
				   ALARM_THRESH_MSK,
				   FIELD_PREP(ALARM_THRESH_MSK, alarm_b));
	}

	return 0;
}

static int falling_alarm_get(void *data, u64 *val)
{
	struct pvt_device *pvt = data;
	u32 alarm_b;

	regmap_read(pvt->t_map, TP_ALARMB_CFG(0), &alarm_b);
	*val = pvt_calc_temp(pvt, FIELD_GET(ALARM_THRESH_MSK, alarm_b));

	return 0;
}

static int falling_hyst_set(void *data, u64 val)
{
	struct pvt_device *pvt = data;
	u32 i, hyst_b;

	hyst_b = pvt_calc_reg(pvt, (s64)val);
	for (i = 0; i < pvt->t_num; i++) {
		regmap_update_bits(pvt->t_map, TP_ALARMB_CFG(i),
				   HYST_THRESH_MSK,
				   FIELD_PREP(HYST_THRESH_MSK, hyst_b));
	}

	return 0;
}

static int falling_hyst_get(void *data, u64 *val)
{
	struct pvt_device *pvt = data;
	u32 hyst_b;

	regmap_read(pvt->t_map, TP_ALARMB_CFG(0), &hyst_b);
	*val = pvt_calc_temp(pvt, FIELD_GET(HYST_THRESH_MSK, hyst_b));

	return 0;
}

static int interrupt_en_set(void *data, u64 val)
{
	struct pvt_device *pvt = data;

	if (val == pvt->int_en)
		return 0;

	if (val)
		enable_irq(pvt->irq);
	else
		disable_irq(pvt->irq);

	pvt->int_en = val;

	return 0;
}

static int interrupt_en_get(void *data, u64 *val)
{
	struct pvt_device *pvt = data;

	*val = pvt->int_en;

	return 0;
}

static void pvt_ts_handle_test_work(struct work_struct *work)
{
	struct pvt_device *pvt = container_of(work, struct pvt_device, work.work);
	int cpu = smp_processor_id();

	epu_notifier_blocking_chain(I2C_SEM_EVENT_REQUEST, &cpu);
	epu_notifier_blocking_chain(I2C_SEM_EVENT_RELEASE, &cpu);

	pvt->irq_cnt++;

	if (pvt->irq_cnt < pvt->irq_test_cnt)
		schedule_delayed_work(&pvt->work, usecs_to_jiffies(1));
}

static int interrupt_trg_set(void *data, u64 val)
{
	struct pvt_device *pvt = data;

	/* disable interrupt if pseudo interrupt handling is enabled */
	if (!val) {
		cancel_delayed_work_sync(&pvt->work);
		pvt->irq_cnt = 0;
		enable_irq(pvt->irq);
	} else {
		cancel_delayed_work_sync(&pvt->work);
		disable_irq(pvt->irq);
		pvt->irq_cnt = 0;
		schedule_delayed_work(&pvt->work, usecs_to_jiffies(2));
	}
	pvt->int_en = !!val;
	pvt->irq_test_cnt = (u32)val;

	return 0;
}

static int interrupt_trg_get(void *data, u64 *val)
{
	struct pvt_device *pvt = data;

	*val = pvt->irq_cnt;

	return 0;
}

DEFINE_DEBUGFS_ATTRIBUTE(rising_alarm_fops, rising_alarm_get,
			 rising_alarm_set, "%llu\n");
DEFINE_DEBUGFS_ATTRIBUTE(rising_hyst_fops, rising_hyst_get,
			 rising_hyst_set, "%llu\n");
DEFINE_DEBUGFS_ATTRIBUTE(falling_alarm_fops, falling_alarm_get,
			 falling_alarm_set, "%llu\n");
DEFINE_DEBUGFS_ATTRIBUTE(falling_hyst_fops, falling_hyst_get,
			 falling_hyst_set, "%llu\n");
DEFINE_DEBUGFS_ATTRIBUTE(interrupt_en_fops, interrupt_en_get,
			 interrupt_en_set, "%llu\n");
DEFINE_DEBUGFS_ATTRIBUTE(interrupt_trg_fops, interrupt_trg_get,
			 interrupt_trg_set, "%llu\n");

void pvt_debugfs_init(struct pvt_device *pvt)
{
	struct dentry *pvt_dir, *file;

	pvt_dir = debugfs_create_dir("pvt", NULL);
	if (!pvt_dir)
		return;

	file = debugfs_create_file("rising_alarm", 0644, pvt_dir, pvt,
				   &rising_alarm_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("rising_hyst", 0644, pvt_dir, pvt,
				   &rising_hyst_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("falling_alarm", 0644, pvt_dir, pvt,
				   &falling_alarm_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("falling_hyst", 0644, pvt_dir, pvt,
				   &falling_hyst_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("interrupt_enable", 0644, pvt_dir, pvt,
				   &interrupt_en_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("pseudo_interrupt", 0644, pvt_dir, pvt,
				   &interrupt_trg_fops);
	if (!file)
		goto err;

	INIT_DELAYED_WORK(&pvt->work, pvt_ts_handle_test_work);

	return;
err:
	debugfs_remove_recursive(pvt_dir);
}
#endif

static int pvt_init(struct pvt_device *pvt)
{
	u16 sys_freq, key, middle, low = 4, high = 8;
	struct regmap *t_map = pvt->t_map;
	struct regmap *p_map = pvt->p_map;
	struct regmap *v_map = pvt->v_map;
	u32 t_num = pvt->t_num;
	u32 p_num = pvt->p_num;
	u32 v_num = pvt->v_num;
	u32 clk_synth, val;
	int ret;

	sys_freq = clk_get_rate(pvt->clk) / HZ_PER_MHZ;
	while (high >= low) {
		middle = (low + high + 1) / 2;
		key = DIV_ROUND_CLOSEST(sys_freq, middle);
		if (key > CLK_SYS_CYCLES_MAX) {
			low = middle + 1;
			continue;
		} else if (key < CLK_SYS_CYCLES_MIN) {
			high = middle - 1;
			continue;
		} else {
			break;
		}
	}

	/*
	 * The system supports 'clk_sys' to 'clk_ip' frequency ratios
	 * from 2:1 to 512:1
	 */
	key = clamp_val(key, CLK_SYS_CYCLES_MIN, CLK_SYS_CYCLES_MAX) - 2;

	clk_synth = ((key + 1) >> 1) << CLK_SYNTH_LO_SFT |
		    (key >> 1) << CLK_SYNTH_HI_SFT |
		    (key >> 1) << CLK_SYNTH_HOLD_SFT | CLK_SYNTH_EN;

	pvt->ip_freq = clk_get_rate(pvt->clk) / (key + 2);

	if (t_num) {
		ret = regmap_write(t_map, SDIF_SMPL_CTRL, 0x0);
		if (ret < 0)
			return ret;

		ret = regmap_write(t_map, SDIF_HALT, 0x0);
		if (ret < 0)
			return ret;

		ret = regmap_write(t_map, CLK_SYNTH, clk_synth);
		if (ret < 0)
			return ret;

		ret = regmap_write(t_map, SDIF_DISABLE, 0x0);
		if (ret < 0)
			return ret;

		ret = regmap_read_poll_timeout(t_map, SDIF_STAT,
					       val, !(val & SDIF_BUSY),
					       PVT_POLL_DELAY_US,
					       PVT_POLL_TIMEOUT_US);
		if (ret)
			return ret;

		val = CFG0_MODE_2 | CFG0_PARALLEL_OUT | CFG0_12_BIT |
		      IP_CFG << SDIF_ADDR_SFT | SDIF_WRN_W | SDIF_PROG;
		ret = regmap_write(t_map, SDIF_W, val);
		if (ret < 0)
			return ret;

		ret = regmap_read_poll_timeout(t_map, SDIF_STAT,
					       val, !(val & SDIF_BUSY),
					       PVT_POLL_DELAY_US,
					       PVT_POLL_TIMEOUT_US);
		if (ret)
			return ret;

		val = POWER_DELAY_CYCLE_256 | IP_TMR << SDIF_ADDR_SFT |
			      SDIF_WRN_W | SDIF_PROG;
		ret = regmap_write(t_map, SDIF_W, val);
		if (ret < 0)
			return ret;

		ret = regmap_read_poll_timeout(t_map, SDIF_STAT,
					       val, !(val & SDIF_BUSY),
					       PVT_POLL_DELAY_US,
					       PVT_POLL_TIMEOUT_US);
		if (ret)
			return ret;

		val = IP_RST_REL | IP_RUN_CONT | IP_AUTO |
		      IP_CTRL << SDIF_ADDR_SFT |
		      SDIF_WRN_W | SDIF_PROG;
		ret = regmap_write(t_map, SDIF_W, val);
		if (ret < 0)
			return ret;

		pvt_isr_init(pvt);
	}

	if (p_num) {
		ret = regmap_write(p_map, SDIF_HALT, 0x0);
		if (ret < 0)
			return ret;

		ret = regmap_write(p_map, SDIF_DISABLE, BIT(p_num) - 1);
		if (ret < 0)
			return ret;

		ret = regmap_write(p_map, CLK_SYNTH, clk_synth);
		if (ret < 0)
			return ret;
	}

	if (v_num) {
		ret = regmap_write(v_map, SDIF_SMPL_CTRL, 0x0);
		if (ret < 0)
			return ret;

		ret = regmap_write(v_map, SDIF_HALT, 0x0);
		if (ret < 0)
			return ret;

		ret = regmap_write(v_map, CLK_SYNTH, clk_synth);
		if (ret < 0)
			return ret;

		ret = regmap_write(v_map, SDIF_DISABLE, 0x0);
		if (ret < 0)
			return ret;

		ret = regmap_read_poll_timeout(v_map, SDIF_STAT,
					       val, !(val & SDIF_BUSY),
					       PVT_POLL_DELAY_US,
					       PVT_POLL_TIMEOUT_US);
		if (ret)
			return ret;

		val = (BIT(pvt->vm_channels.max) - 1) | VM_CH_INIT |
		      IP_POLL << SDIF_ADDR_SFT | SDIF_WRN_W | SDIF_PROG;
		ret = regmap_write(v_map, SDIF_W, val);
		if (ret < 0)
			return ret;

		ret = regmap_read_poll_timeout(v_map, SDIF_STAT,
					       val, !(val & SDIF_BUSY),
					       PVT_POLL_DELAY_US,
					       PVT_POLL_TIMEOUT_US);
		if (ret)
			return ret;

		val = (BIT(pvt->c_num) - 1) | VM_CH_INIT |
		      IP_POLL << SDIF_ADDR_SFT | SDIF_WRN_W | SDIF_PROG;
		ret = regmap_write(v_map, SDIF_W, val);
		if (ret < 0)
			return ret;

		ret = regmap_read_poll_timeout(v_map, SDIF_STAT,
					       val, !(val & SDIF_BUSY),
					       PVT_POLL_DELAY_US,
					       PVT_POLL_TIMEOUT_US);
		if (ret)
			return ret;

		val = CFG1_VOL_MEAS_MODE | CFG1_PARALLEL_OUT |
		      CFG1_14_BIT | IP_CFG << SDIF_ADDR_SFT |
		      SDIF_WRN_W | SDIF_PROG;
		ret = regmap_write(v_map, SDIF_W, val);
		if (ret < 0)
			return ret;

		ret = regmap_read_poll_timeout(v_map, SDIF_STAT,
					       val, !(val & SDIF_BUSY),
					       PVT_POLL_DELAY_US,
					       PVT_POLL_TIMEOUT_US);
		if (ret)
			return ret;

		val = POWER_DELAY_CYCLE_64 | IP_TMR << SDIF_ADDR_SFT |
		      SDIF_WRN_W | SDIF_PROG;
		ret = regmap_write(v_map, SDIF_W, val);
		if (ret < 0)
			return ret;

		ret = regmap_read_poll_timeout(v_map, SDIF_STAT,
					       val, !(val & SDIF_BUSY),
					       PVT_POLL_DELAY_US,
					       PVT_POLL_TIMEOUT_US);
		if (ret)
			return ret;

		val = IP_RST_REL | IP_RUN_CONT | IP_AUTO | IP_VM_MODE |
		      IP_CTRL << SDIF_ADDR_SFT |
		      SDIF_WRN_W | SDIF_PROG;
		ret = regmap_write(v_map, SDIF_W, val);
		if (ret < 0)
			return ret;
	}

	return 0;
}

static volatile int cpum_irq_counter[2];

static irqreturn_t pvt_isr(int irq, void *dev_id)
{
	struct pvt_device *pvt = (struct pvt_device *)dev_id;
	struct regmap *t_map = pvt->t_map;
	u32 stat, val, ind;
	int i;

	regmap_read(pvt->c_map, IRQ_TS_STAT, &stat);
	dev_dbg(pvt->dev, "IRQ_TS_STAT = 0x%x\n", stat);

	for_each_set_bit(i, (unsigned long *)&stat, pvt->t_num) {
		if (i > 3)
			continue;

		if (!cpu_online(i)) {
			regmap_write(t_map, TP_IRQ_CLR(i), 0xffffffff);
			continue;
		}

		regmap_read(t_map, TP_IRQ_STAT(i), &val);

		ind = i >> 1;

		if (val & IRQ_ALARMA_MSK) {
			regmap_update_bits(t_map, TP_IRQ_EN(i),
					   IRQ_ALARMA_MSK, 0x0);

			if (!cpum_irq_counter[ind]) {
				epu_notifier_blocking_chain(I2C_SEM_EVENT_REQUEST, &ind);
				epu_notifier_blocking_chain(I2C_SEM_EVENT_RELEASE, NULL);
			}

			cpum_irq_counter[ind]++;

			regmap_update_bits(t_map, TP_IRQ_EN(i),
					   IRQ_ALARMB_MSK,
					   FIELD_PREP(IRQ_ALARMB_MSK, IRQ_EN));

			mb();

			dev_dbg(pvt->dev, "Rising interrupt(CPU%d) is triggered\n", i);

		} else if (val & IRQ_ALARMB_MSK) {
			regmap_update_bits(t_map, TP_IRQ_EN(i),
					   IRQ_ALARMB_MSK, 0x0);

			cpum_irq_counter[ind]--;

			if (!cpum_irq_counter[ind]) {
				epu_notifier_blocking_chain(I2C_SEM_EVENT_REQUEST, &ind);
				epu_notifier_blocking_chain(I2C_SEM_EVENT_RELEASE, NULL);
			}

			regmap_update_bits(t_map, TP_IRQ_EN(i),
					   IRQ_ALARMA_MSK,
					   FIELD_PREP(IRQ_ALARMA_MSK, IRQ_EN));

			mb();

			dev_dbg(pvt->dev, "falling interrupt(CPU%d) is triggered\n", i);
		}

		regmap_write(t_map, TP_IRQ_CLR(i), 0xffffffff);
	}

	return IRQ_HANDLED;
}
static struct regmap_config pvt_regmap_config = {
	.reg_bits = 32,
	.reg_stride = 4,
	.val_bits = 32,
};

static int pvt_get_regmap(struct platform_device *pdev, char *reg_name,
			  struct pvt_device *pvt)
{
	struct device *dev = &pdev->dev;
	struct regmap **reg_map;
	void __iomem *io_base;

	if (!strcmp(reg_name, "common"))
		reg_map = &pvt->c_map;
	else if (!strcmp(reg_name, "ts"))
		reg_map = &pvt->t_map;
	else if (!strcmp(reg_name, "pd"))
		reg_map = &pvt->p_map;
	else if (!strcmp(reg_name, "vm"))
		reg_map = &pvt->v_map;
	else
		return -EINVAL;

	io_base = devm_platform_ioremap_resource_byname(pdev, reg_name);
	if (IS_ERR(io_base))
		return PTR_ERR(io_base);

	pvt_regmap_config.name = reg_name;
	*reg_map = devm_regmap_init_mmio(dev, io_base, &pvt_regmap_config);
	if (IS_ERR(*reg_map)) {
		dev_err(dev, "failed to init register map\n");
		return PTR_ERR(*reg_map);
	}

	return 0;
}

static void pvt_reset_control_assert(void *data)
{
	struct pvt_device *pvt = data;

	reset_control_assert(pvt->rst);
}

static int pvt_reset_control_deassert(struct device *dev, struct pvt_device *pvt)
{
	int ret;

	ret = reset_control_deassert(pvt->rst);
	if (ret)
		return ret;

	return devm_add_action_or_reset(dev, pvt_reset_control_assert, pvt);
}

static int pvt_get_active_channel(struct device *dev, struct pvt_device *pvt,
				  u32 vm_num, u32 ch_num, u8 *vm_idx)
{
	u8 vm_active_ch[VM_NUM_MAX];
	int ret, i, j, k;

	ret = device_property_read_u8_array(dev, "moortec,vm-active-channels",
					    vm_active_ch, vm_num);
	if (ret) {
		/*
		 * Incase "moortec,vm-active-channels" property is not defined,
		 * we assume each VM sensor has all of its channels active.
		 */
		memset(vm_active_ch, ch_num, vm_num);
		pvt->vm_channels.max = ch_num;
		pvt->vm_channels.total = ch_num * vm_num;
	} else {
		for (i = 0; i < vm_num; i++) {
			if (vm_active_ch[i] > ch_num) {
				dev_err(dev, "invalid active channels: %u\n",
					vm_active_ch[i]);
				return -EINVAL;
			}

			pvt->vm_channels.total += vm_active_ch[i];

			if (vm_active_ch[i] > pvt->vm_channels.max)
				pvt->vm_channels.max = vm_active_ch[i];
		}
	}

	/*
	 * Map between the channel-number to VM-index and channel-index.
	 * Example - 3 VMs, "moortec,vm_active_ch" = <5 2 4>:
	 * vm_map = [0 0 0 0 0 1 1 2 2 2 2]
	 * ch_map = [0 1 2 3 4 0 1 0 1 2 3]
	 */
	pvt->vd = devm_kcalloc(dev, pvt->vm_channels.total, sizeof(*pvt->vd),
			       GFP_KERNEL);
	if (!pvt->vd)
		return -ENOMEM;

	k = 0;
	for (i = 0; i < vm_num; i++) {
		for (j = 0; j < vm_active_ch[i]; j++) {
			pvt->vd[k].vm_map = vm_idx[i];
			pvt->vd[k].ch_map = j;
			k++;
		}
	}

	return 0;
}

static int pvt_get_pre_scaler(struct device *dev, struct pvt_device *pvt)
{
	u8 *pre_scaler_ch_list;
	int i, ret, num_ch;
	u32 channel;

	/* Set default pre-scaler value to be 1. */
	for (i = 0; i < pvt->vm_channels.total; i++)
		pvt->vd[i].pre_scaler = PRE_SCALER_X1;

	/* Get number of channels configured in "moortec,vm-pre-scaler-x2". */
	num_ch = device_property_count_u8(dev, "moortec,vm-pre-scaler-x2");
	if (num_ch <= 0)
		return 0;

	pre_scaler_ch_list = kcalloc(num_ch, sizeof(*pre_scaler_ch_list),
				     GFP_KERNEL);
	if (!pre_scaler_ch_list)
		return -ENOMEM;

	/* Get list of all channels that have pre-scaler of 2. */
	ret = device_property_read_u8_array(dev, "moortec,vm-pre-scaler-x2",
					    pre_scaler_ch_list, num_ch);
	if (ret)
		goto out;

	for (i = 0; i < num_ch; i++) {
		channel = pre_scaler_ch_list[i];
		pvt->vd[channel].pre_scaler = PRE_SCALER_X2;
	}

out:
	kfree(pre_scaler_ch_list);

	return ret;
}

static int pvt_set_temp_coeff(struct device *dev, struct pvt_device *pvt)
{
	struct temp_coeff *ts_coeff = &pvt->ts_coeff;
	u32 series;
	int ret;

	/* Incase ts-series property is not defined, use default 5. */
	ret = device_property_read_u32(dev, "moortec,ts-series", &series);
	if (ret)
		series = TEMPERATURE_SENSOR_SERIES_5;

	switch (series) {
	case TEMPERATURE_SENSOR_SERIES_5:
		ts_coeff->h = PVT_SERIES5_H_CONST;
		ts_coeff->g = PVT_SERIES5_G_CONST;
		ts_coeff->j = PVT_SERIES5_J_CONST;
		ts_coeff->cal5 = PVT_SERIES5_CAL5_CONST;
		break;
	case TEMPERATURE_SENSOR_SERIES_6:
		ts_coeff->h = PVT_SERIES6_H_CONST;
		ts_coeff->g = PVT_SERIES6_G_CONST;
		ts_coeff->j = PVT_SERIES6_J_CONST;
		ts_coeff->cal5 = PVT_SERIES6_CAL5_CONST;
		break;
	default:
		dev_err(dev, "invalid temperature sensor series (%u)\n",
			series);
		return -EINVAL;
	}

	dev_dbg(dev, "temperature sensor series = %u\n", series);

	/* Override ts-coeff-h/g/j/cal5 if they are defined. */
	device_property_read_u32(dev, "moortec,ts-coeff-h", &ts_coeff->h);
	device_property_read_u32(dev, "moortec,ts-coeff-g", &ts_coeff->g);
	device_property_read_u32(dev, "moortec,ts-coeff-j", &ts_coeff->j);
	device_property_read_u32(dev, "moortec,ts-coeff-cal5", &ts_coeff->cal5);

	dev_dbg(dev, "ts-coeff: h = %u, g = %u, j = %d, cal5 = %u\n",
		ts_coeff->h, ts_coeff->g, ts_coeff->j, ts_coeff->cal5);

	return 0;
}

static void pvt_clk_disable(void *data)
{
	struct pvt_device *pvt = data;

	clk_disable_unprepare(pvt->clk);
}

static int mr75203_probe(struct platform_device *pdev)
{
	u32 ts_num, vm_num, pd_num, ch_num, val, index, i;
	const struct hwmon_channel_info **pvt_info;
	struct device *dev = &pdev->dev;
	u32 *temp_config, *in_config;
	struct device *hwmon_dev;
	struct pvt_device *pvt;
	int irq, ret;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(dev, "failed to irq: %d\n", irq);
		return irq;
	}

	pvt = devm_kzalloc(dev, sizeof(*pvt), GFP_KERNEL);
	if (!pvt)
		return -ENOMEM;

	pvt->dev = dev;
	ret = pvt_get_regmap(pdev, "common", pvt);
	if (ret)
		return ret;

	pvt->clk = devm_clk_get(dev, NULL);
	if (IS_ERR(pvt->clk))
		return dev_err_probe(dev, PTR_ERR(pvt->clk), "failed to get clock\n");

	ret = clk_prepare_enable(pvt->clk);
	if (ret)
		return dev_err_probe(dev, PTR_ERR(pvt->clk), "failed to enable clock\n");

	ret = devm_add_action_or_reset(dev, pvt_clk_disable, pvt);
	if (ret)
		return dev_err_probe(dev, PTR_ERR(pvt->clk), "failure in dem_add_action_or_reset()\n");

	pvt->rst = devm_reset_control_get_optional_exclusive(dev, NULL);
	if (IS_ERR(pvt->rst))
		return dev_err_probe(dev, PTR_ERR(pvt->rst),
				     "failed to get reset control\n");

	if (pvt->rst) {
		ret = pvt_reset_control_deassert(dev, pvt);
		if (ret)
			return dev_err_probe(dev, ret,
					     "cannot deassert reset control\n");
	}

	ret = regmap_read(pvt->c_map, PVT_IP_CONFIG, &val);
	if (ret < 0)
		return ret;

	ts_num = (val & TS_NUM_MSK) >> TS_NUM_SFT;
	pd_num = (val & PD_NUM_MSK) >> PD_NUM_SFT;
	vm_num = (val & VM_NUM_MSK) >> VM_NUM_SFT;
	ch_num = (val & CH_NUM_MSK) >> CH_NUM_SFT;
	pvt->t_num = ts_num;
	pvt->p_num = pd_num;
	pvt->v_num = vm_num;
	pvt->c_num = ch_num;
	val = 0;
	if (ts_num)
		val++;
	if (vm_num)
		val++;
	if (!val)
		return -ENODEV;

	pvt_info = devm_kcalloc(dev, val + 2, sizeof(*pvt_info), GFP_KERNEL);
	if (!pvt_info)
		return -ENOMEM;
	pvt_info[0] = HWMON_CHANNEL_INFO(chip, HWMON_C_REGISTER_TZ);
	index = 1;

	if (ts_num) {
		ret = pvt_get_regmap(pdev, "ts", pvt);
		if (ret)
			return ret;

		ret = pvt_set_temp_coeff(dev, pvt);
		if (ret)
			return ret;

		temp_config = devm_kcalloc(dev, ts_num + 1,
					   sizeof(*temp_config), GFP_KERNEL);
		if (!temp_config)
			return -ENOMEM;

		memset32(temp_config, HWMON_T_INPUT, ts_num);
		pvt_temp.config = temp_config;
		pvt_info[index++] = &pvt_temp;

		pvt_ts_dbgfs_create(pvt, dev);
	}

	if (pd_num) {
		ret = pvt_get_regmap(pdev, "pd", pvt);
		if (ret)
			return ret;
	}

	if (vm_num) {
		u8 vm_idx[VM_NUM_MAX];

		ret = pvt_get_regmap(pdev, "vm", pvt);
		if (ret)
			return ret;

		ret = device_property_read_u8_array(dev, "intel,vm-map", vm_idx,
						    vm_num);
		if (ret) {
			/*
			 * Incase intel,vm-map property is not defined, we
			 * assume incremental channel numbers.
			 */
			for (i = 0; i < vm_num; i++)
				vm_idx[i] = i;
		} else {
			for (i = 0; i < vm_num; i++)
				if (vm_idx[i] >= vm_num || vm_idx[i] == 0xff) {
					pvt->v_num = i;
					vm_num = i;
					break;
				}
		}

		ret = pvt_get_active_channel(dev, pvt, vm_num, ch_num, vm_idx);
		if (ret)
			return ret;

		ret = pvt_get_pre_scaler(dev, pvt);
		if (ret)
			return ret;

		in_config = devm_kcalloc(dev, pvt->vm_channels.total + 1,
					 sizeof(*in_config), GFP_KERNEL);
		if (!in_config)
			return -ENOMEM;

		memset32(in_config, HWMON_I_INPUT, pvt->vm_channels.total);
		in_config[pvt->vm_channels.total] = 0;
		pvt_in.config = in_config;

		pvt_info[index++] = &pvt_in;
	}

	ret = pvt_init(pvt);
	if (ret) {
		dev_err(dev, "failed to init pvt: %d\n", ret);
		return ret;
	}

	ret = devm_request_threaded_irq(dev, irq, NULL, pvt_isr, IRQF_ONESHOT,
					dev_name(dev), pvt);
	if (ret) {
		dev_err(dev, "failure requesting irq %i: %d\n", irq, ret);
		return ret;
	}

	if(IS_ENABLED(CONFIG_DEBUG_FS)) {
		pvt_debugfs_init(pvt);
		pvt->int_en = 1;
		pvt->irq = irq;
	}

	pvt_chip_info.info = pvt_info;
	hwmon_dev = devm_hwmon_device_register_with_info(dev, "pvt",
							 pvt,
							 &pvt_chip_info,
							 NULL);

	return PTR_ERR_OR_ZERO(hwmon_dev);
}

static const struct of_device_id moortec_pvt_of_match[] = {
	{ .compatible = "moortec,mr75203" },
	{ }
};
MODULE_DEVICE_TABLE(of, moortec_pvt_of_match);

static struct platform_driver moortec_pvt_driver = {
	.driver = {
		.name = "moortec-pvt",
		.of_match_table = moortec_pvt_of_match,
	},
	.probe = mr75203_probe,
};
module_platform_driver(moortec_pvt_driver);

MODULE_LICENSE("GPL v2");
