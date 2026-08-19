// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 MaxLinear, Inc.
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
#include <linux/debugfs.h>
#include <linux/interrupt.h>
#include <linux/iopoll.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/reset.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>

#define I2S_CLC	0x0
#define I2S_CLC_RUN	(1 << 0)
#define I2S_CLC_STOP	(2 << 0)

#define I2S_CLC_STAT	0x8
#define I2S_CLC_STAT_RUN	BIT(0)

#define I2S_FIFO_CFG	0x30
#define I2S_FIFO_CFG_TXFA2	(1 << 12)
#define I2S_FIFO_CFG_TXBS1	(0 << 4)
#define I2S_FIFO_CFG_TXBS2	(1 << 4)
#define I2S_FIFO_CFG_TXBS4	(2 << 4)
#define I2S_FIFO_CFG_TXBS8	(3 << 4)
#define I2S_FIFO_CFG_TXBS16	(4 << 4)
#define I2S_FIFO_CFG_TXFC	BIT(7)

#define I2S_FIFO_CTRL	0x34
#define I2S_TPS_CTRL	0x40
#define I2S_FIFO_STAT	0x44
#define I2S_FIFO_STAT_TXFFS_SHIFT 16
#define I2S_FIFO_STAT_TXFFS_MASK GENMASK(23, 16)

#define I2S_IMSC	0x84
#define I2S_IMSC_TX_INT		BIT(9)
#define I2S_IMSC_TX_BREQ	BIT(7)
#define I2S_IMSC_TX_LBREQ	BIT(6)
#define I2S_IMSC_TX_SREQ	BIT(5)
#define I2S_IMSC_TX_LSREQ	BIT(4)

#define I2S_MIS		0x88
#define I2S_ICR		0x98

#define I2S_CTRL	0x100
#define I2S_CTRL_I2SON	BIT(0)
#define I2S_CTRL_TXSTART	BIT(1)

#define I2S_CSEL	0x104

#define I2S_DIV0	0x10c
#define I2S_DIV1	0x110
#define I2S_DIV_NUMERATOR_SHIFT 16
#define I2S_DIV_DENOMINATOR_SHIFT 0

#define I2S_TXCONF	0x128
#define I2S_CONF_WIDTH_B16	(0 << 5)
#define I2S_CONF_WIDTH_B24	(6 << 5)
#define I2S_CONF_WIDTH_B32	(7 << 5)
#define I2S_CONF_PERIOD_C64	(0 << 3)
#define I2S_CONF_PERIOD_C48	(1 << 3)
#define I2S_CONF_PERIOD_C32	(2 << 3)
#define I2S_CONF_DEL		BIT(1)

#define I2S_TXD		0x4000

#define MXL_I2S_FIFO_CHUNK (0x4000)
#define WAV_HEADER_SIZE (44)
#define WAV_RIFF_ID	(0x46464952)
#define WAV_HEADER_CH_OFFSET	(22)
#define WAV_HEADER_FREQ_OFFSET	(24)
#define WAV_HEADER_BIT_OFFSET	(34)

struct mxl_i2s_data {
	struct device *dev;
	void __iomem *base;
	struct clk *clk;
	struct reset_control *rst;
	int irq;

	struct dentry *debugfs;

	struct {
		u8 *buf;
		u32 len;
		u32 pointer;
		u32 num_ch;
		u32 burst_size;
		u32 sample_rate;
		u32 bit_size;
		u32 hw_bit_size;
		bool retrigger;
		struct completion complete;
		struct snd_pcm_substream __rcu *substream;
		u32 (*prepare_txd)(struct mxl_i2s_data *priv);
		u32 txd_count;
		u32 txd_len;
		u32 period_count;
	} tx;
};

static void mxl_i2s_writel_mask(u32 clr, u32 set, volatile void __iomem *addr)
{
	u32 val = readl(addr);

	val &= ~clr;
	val |= set;
	writel(val, addr);
}

static u32 txd_1ch_16b(struct mxl_i2s_data *priv)
{
	u16 *p = (u16 *)(priv->tx.buf + priv->tx.pointer);
	u32 val;

	if (priv->tx.pointer > priv->tx.len - sizeof(*p)) {
		dev_dbg(priv->dev, "exceed size, dummy data sent\n");
		return 0;
	}

	val = *p;

	/* We always transmit in stereo, so duplicate left to right */
	val |= val << 16;

	priv->tx.pointer += sizeof(*p);
	return val;
}

static u32 txd_2ch(struct mxl_i2s_data *priv)
{
	u32 *p = (u32 *)(priv->tx.buf + priv->tx.pointer);
	u32 val;

	if (priv->tx.pointer > priv->tx.len - sizeof(*p)) {
		dev_dbg(priv->dev, "exceed size, dummy data sent\n");
		return 0;
	}

	val = *p;

	priv->tx.pointer += sizeof(*p);
	return val;
}

static u32 txd_1ch_32b(struct mxl_i2s_data *priv)
{
	u32 *p = (u32 *)(priv->tx.buf + priv->tx.pointer);
	u32 val;

	if (priv->tx.pointer > priv->tx.len - sizeof(*p)) {
		dev_dbg(priv->dev, "exceed size, dummy data sent\n");
		return 0;
	}

	val = *p;

	/* We always transmit in stereo, so duplicate left to right.
	 * Only increase pointer after right channel is done.
	 */
	if (priv->tx.txd_count % 2)
		priv->tx.pointer += sizeof(*p);

	return val;
}

static u32 txd_2ch_24b(struct mxl_i2s_data *priv)
{
	u8 *p = priv->tx.buf + priv->tx.pointer;
	u32 val;

	if (priv->tx.pointer > priv->tx.len - 3) {
		dev_dbg(priv->dev, "exceed size, dummy data sent\n");
		return 0;
	}

	val = (p[2] << 24) | (p[1] << 16) | (p[0] << 8);

	priv->tx.pointer += 3;
	return val;
}

static u32 txd_1ch_24b(struct mxl_i2s_data *priv)
{
	u8 *p = priv->tx.buf + priv->tx.pointer;
	u32 val;

	if (priv->tx.pointer > priv->tx.len - 3) {
		dev_dbg(priv->dev, "exceed size, dummy data sent\n");
		return 0;
	}

	val = (p[2] << 24) | (p[1] << 16) | (p[0] << 8);

	/* We always transmit in stereo, so duplicate left to right.
	 * Only increase pointer after right channel is done.
	 */
	if (priv->tx.txd_count % 2)
		priv->tx.pointer += 3;

	return val;
}

static void mxl_i2s_update_elapsed(struct mxl_i2s_data *priv,
				   struct snd_pcm_substream *substream)
{
	u32 frame_words = priv->tx.hw_bit_size / 16;

	if (!(priv->tx.txd_count % frame_words))
		priv->tx.period_count++;

	if (priv->tx.period_count == substream->runtime->period_size) {
		snd_pcm_period_elapsed(substream);
		priv->tx.period_count = 0;
		dev_dbg(priv->dev, "elapsed (%u)\n", priv->tx.txd_count);
	}
}

static irqreturn_t mxl_i2s_irq(int irq, void *data)
{
	struct mxl_i2s_data *priv = data;
	struct snd_pcm_substream *substream;
	u32 irq_stat;
	u32 loop;
	u32 val;
	int i;

	irq_stat = readl(priv->base + I2S_MIS);

	rcu_read_lock();
	substream = rcu_dereference(priv->tx.substream);

	if (irq_stat & I2S_IMSC_TX_INT) {
		if (priv->tx.retrigger) {
			priv->tx.pointer = 0;
			priv->tx.txd_count = 0;
			priv->tx.period_count = 0;
			writel(priv->tx.txd_len, priv->base + I2S_TPS_CTRL);

			dev_dbg(priv->dev, "retrigger (%u)\n",
				priv->tx.txd_len);
		}

		complete(&priv->tx.complete);

	} else {
		if (irq_stat & (I2S_IMSC_TX_BREQ | I2S_IMSC_TX_LBREQ))
			loop = priv->tx.burst_size;
		else
			loop = 1;

		for (i = 0; i < loop; i++) {
			val = priv->tx.prepare_txd(priv);
			writel(val, priv->base + I2S_TXD);

			if (substream && snd_pcm_running(substream))
				mxl_i2s_update_elapsed(priv, substream);

			priv->tx.txd_count++;
		}
	}

	rcu_read_unlock();

	writel(irq_stat, priv->base + I2S_ICR);
	return IRQ_HANDLED;
}

static int mxl_i2s_tx_configure(struct mxl_i2s_data *priv)
{
	const u32 fixed_numerator = 400;
	unsigned long ref_hz = clk_get_rate(priv->clk);
	u32 clk_hz = priv->tx.sample_rate * priv->tx.bit_size * 2;
	u64 denom = (fixed_numerator * (ref_hz / clk_hz)) / 4;
	u32 val;

	mxl_i2s_writel_mask(I2S_CTRL_TXSTART | I2S_CTRL_I2SON, 0,
			    priv->base + I2S_CTRL);
	writel(I2S_CLC_STOP, priv->base + I2S_CLC);

	switch (priv->tx.bit_size) {
	case 16:
		val = I2S_CONF_WIDTH_B16 | I2S_CONF_PERIOD_C32;
		if (priv->tx.num_ch == 1)
			priv->tx.prepare_txd = txd_1ch_16b;
		else
			priv->tx.prepare_txd = txd_2ch;
		break;
	case 24:
		val = I2S_CONF_WIDTH_B24 | I2S_CONF_PERIOD_C48;
		if (priv->tx.num_ch == 1)
			priv->tx.prepare_txd = txd_1ch_24b;
		else
			priv->tx.prepare_txd = txd_2ch_24b;
		break;
	case 32:
		val = I2S_CONF_WIDTH_B32 | I2S_CONF_PERIOD_C64;
		if (priv->tx.num_ch == 1)
			priv->tx.prepare_txd = txd_1ch_32b;
		else
			priv->tx.prepare_txd = txd_2ch;
		break;
	default:
		dev_err(priv->dev, "Invalid bit size %d\n", priv->tx.bit_size);
		return -EINVAL;
	}

	val |= I2S_CONF_DEL;
	writel(val, priv->base + I2S_TXCONF);

	val = (fixed_numerator << I2S_DIV_NUMERATOR_SHIFT) |
	      (denom << I2S_DIV_DENOMINATOR_SHIFT);
	writel(val, priv->base + I2S_DIV0);
	writel(val, priv->base + I2S_DIV1);

	switch (priv->tx.burst_size) {
	case 1:
		val = I2S_FIFO_CFG_TXBS1;
		break;
	case 2:
		val = I2S_FIFO_CFG_TXBS2;
		break;
	case 4:
		val = I2S_FIFO_CFG_TXBS4;
		break;
	case 8:
		val = I2S_FIFO_CFG_TXBS8;
		break;
	case 16:
		val = I2S_FIFO_CFG_TXBS16;
		break;
	default:
		dev_err(priv->dev, "Invalid burst size %d\n",
			priv->tx.burst_size);
		return -EINVAL;
	}

	val |= I2S_FIFO_CFG_TXFA2 | I2S_FIFO_CFG_TXFC;
	writel(val, priv->base + I2S_FIFO_CFG);

	mxl_i2s_writel_mask(0, I2S_CTRL_I2SON, priv->base + I2S_CTRL);

	writel(I2S_CLC_RUN, priv->base + I2S_CLC);
	if (readl_poll_timeout(priv->base + I2S_CLC_STAT, val,
			       val & I2S_CLC_STAT_RUN, 100, 3000)) {
		dev_err(priv->dev, "Timedout waiting for CLC stat\n");
		return -ETIMEDOUT;
	}

	return 0;
}

static void mxl_i2s_tx_start(struct mxl_i2s_data *priv, u32 len)
{
	priv->tx.txd_len = (len / sizeof(u16)) *
			   priv->tx.hw_bit_size / priv->tx.bit_size;

	/* mono: duplicate left to right */
	if (priv->tx.num_ch == 1)
		priv->tx.txd_len *= 2;

	writel(I2S_IMSC_TX_INT | I2S_IMSC_TX_BREQ | I2S_IMSC_TX_LBREQ |
	       I2S_IMSC_TX_SREQ | I2S_IMSC_TX_LSREQ,
	       priv->base + I2S_IMSC);
	writel(priv->tx.txd_len, priv->base + I2S_TPS_CTRL);

	mxl_i2s_writel_mask(0, I2S_CTRL_TXSTART, priv->base + I2S_CTRL);
}

static void mxl_i2s_tx_stop(struct mxl_i2s_data *priv)
{
	writel(0, priv->base + I2S_IMSC);
	mxl_i2s_writel_mask(I2S_CTRL_TXSTART, 0, priv->base + I2S_CTRL);
}

static const struct snd_pcm_hardware pcm_hardware = {
	.info = SNDRV_PCM_INFO_INTERLEAVED |
		SNDRV_PCM_INFO_MMAP_VALID |
		SNDRV_PCM_INFO_BLOCK_TRANSFER,
	.rates = SNDRV_PCM_RATE_8000_96000,
	.formats = SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S24_LE |
		   SNDRV_PCM_FMTBIT_S24_3LE | SNDRV_PCM_FMTBIT_S32_LE,
	.channels_min = 1,
	.channels_max = 2,
	.period_bytes_min = 2,
	.period_bytes_max = MXL_I2S_FIFO_CHUNK / 2 * 8,
	.periods_min = 2,
	.periods_max = MXL_I2S_FIFO_CHUNK * 8 / 2,
	.buffer_bytes_max = MXL_I2S_FIFO_CHUNK * 8,
	.fifo_size = 16,
};

static int pcm_open(struct snd_soc_component *component,
		    struct snd_pcm_substream *substream)
{
	snd_soc_set_runtime_hwparams(substream, &pcm_hardware);
	return 0;
}

static int pcm_close(struct snd_soc_component *component,
		     struct snd_pcm_substream *substream)
{
	synchronize_rcu();
	return 0;
}

static int pcm_hw_params(struct snd_soc_component *component,
			 struct snd_pcm_substream *substream,
			 struct snd_pcm_hw_params *hw_params)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct mxl_i2s_data *priv;

	priv = snd_soc_dai_get_dma_data(asoc_rtd_to_cpu(rtd, 0), substream);
	runtime->private_data = priv;

	priv->tx.num_ch = params_channels(hw_params);
	priv->tx.sample_rate = params_rate(hw_params);
	priv->tx.burst_size = 2;

	switch (params_format(hw_params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		priv->tx.bit_size = 16;
		priv->tx.hw_bit_size = 16;
		break;
	case SNDRV_PCM_FORMAT_S24_3LE:
		priv->tx.bit_size = 24;

		/* hardware reads 32-bit, with lowest 8-bit discarded */
		priv->tx.hw_bit_size = 32;
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
	case SNDRV_PCM_FORMAT_S32_LE:
		priv->tx.bit_size = 32;
		priv->tx.hw_bit_size = 32;
		break;
	default:
		dev_err(priv->dev, "Invalid hw params\n");
		return -EINVAL;
	}

	dev_dbg(priv->dev, "param: %u ch %u kHz %u-bit\n", priv->tx.num_ch,
		priv->tx.sample_rate, priv->tx.bit_size);

	return mxl_i2s_tx_configure(priv);
}

static void refill_fifo(struct mxl_i2s_data *priv, void *buf, u32 len)
{
	WRITE_ONCE(priv->tx.pointer, 0);
	priv->tx.buf = buf;
	priv->tx.len = len;
	priv->tx.retrigger = true;
	priv->tx.txd_count = 0;
	priv->tx.period_count = 0;

	mxl_i2s_tx_start(priv, len);

	dev_dbg(priv->dev, "trigger (%u)\n", priv->tx.txd_len);
}

static int pcm_trigger(struct snd_soc_component *component,
		       struct snd_pcm_substream *substream, int cmd)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct mxl_i2s_data *priv = runtime->private_data;
	int ret = 0;

	dev_dbg(priv->dev, "cmd 0x%x\n", cmd);

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		rcu_assign_pointer(priv->tx.substream, substream);
		refill_fifo(priv, runtime->dma_area, runtime->dma_bytes);
		break;

	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		mxl_i2s_tx_stop(priv);
		rcu_assign_pointer(priv->tx.substream, NULL);
		break;

	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static snd_pcm_uframes_t pcm_pointer(struct snd_soc_component *component,
				     struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct mxl_i2s_data *priv = runtime->private_data;
	const u32 parse_size = (priv->tx.bit_size * priv->tx.num_ch) / 8;
	snd_pcm_uframes_t pos = READ_ONCE(priv->tx.pointer) / parse_size;

	return pos < runtime->buffer_size ? pos : 0;
}

static int pcm_new(struct snd_soc_component *component,
		   struct snd_soc_pcm_runtime *rtd)
{
	struct snd_card *card = rtd->card->snd_card;
	size_t size = MXL_I2S_FIFO_CHUNK;

	snd_pcm_set_managed_buffer_all(rtd->pcm, SNDRV_DMA_TYPE_DEV,
				       card->dev, size, size);
	return 0;
}

static const struct snd_soc_component_driver i2s_component = {
	.name = "mxl-i2s",
	.open = pcm_open,
	.close = pcm_close,
	.hw_params = pcm_hw_params,
	.trigger = pcm_trigger,
	.pointer = pcm_pointer,
	.pcm_construct = pcm_new,
};

static int startup(struct snd_pcm_substream *substream, struct snd_soc_dai *dai)
{
	struct mxl_i2s_data *priv = snd_soc_dai_get_drvdata(dai);

	snd_soc_dai_set_dma_data(dai, substream, priv);
	return 0;
}

static const struct snd_soc_dai_ops dai_ops = {
	.startup = startup,
};

static struct snd_soc_dai_driver i2s_dai = {
	.ops = &dai_ops,
	.playback.channels_min = pcm_hardware.channels_min,
	.playback.channels_max = pcm_hardware.channels_max,
	.playback.rates = pcm_hardware.rates,
	.playback.formats = pcm_hardware.formats,
};

static int mxl_i2s_debugfs_show(struct seq_file *seq, void *offset)
{
	seq_puts(seq, "How to use:\n");
	seq_puts(seq, "\tcat <wav_file> > txbuf\n");
	return 0;
}

static int mxl_i2s_debugfs_open(struct inode *inode, struct file *file)
{
	return single_open(file, mxl_i2s_debugfs_show, inode->i_private);
}

static ssize_t mxl_i2s_debugfs_write(struct file *file,
				     const char __user *buffer,
				     size_t count, loff_t *ppos)
{
	struct mxl_i2s_data *priv;
	u8 header[WAV_HEADER_SIZE];
	u32 skip = 0;

	priv = ((struct seq_file *)file->private_data)->private;

	if (copy_from_user(header, buffer, sizeof(header))) {
		dev_err(priv->dev, "fail to read from user\n");
		goto exit;
	}

	if (*(u32 *)header == WAV_RIFF_ID) {
		skip = sizeof(header);
		priv->tx.num_ch = header[WAV_HEADER_CH_OFFSET];
		priv->tx.bit_size = header[WAV_HEADER_BIT_OFFSET];
		if (priv->tx.bit_size == 24)
			/* hardware reads 32-bit, with lowest 8-bit discarded */
			priv->tx.hw_bit_size = 32;
		else
			priv->tx.hw_bit_size = priv->tx.bit_size;

		priv->tx.sample_rate = *((u16 *)(header +
						 WAV_HEADER_FREQ_OFFSET));
		priv->tx.burst_size = 2;

		dev_dbg(priv->dev, "WAV %u ch %u kHz %u-bit\n", priv->tx.num_ch,
			priv->tx.sample_rate, priv->tx.bit_size);
	}

	priv->tx.buf = devm_kzalloc(priv->dev, count, GFP_KERNEL);
	if (!priv->tx.buf) {
		dev_err(priv->dev, "Failed to allocated mem\n");
		goto exit;
	}

	priv->tx.retrigger = false;
	priv->tx.len = count;
	priv->tx.pointer = skip;
	priv->tx.txd_count = 0;
	priv->tx.period_count = 0;
	if (copy_from_user(priv->tx.buf, buffer, count)) {
		dev_err(priv->dev, "fail to read from user\n");
		goto exit;
	}

	reinit_completion(&priv->tx.complete);

	if (mxl_i2s_tx_configure(priv))
		goto exit;

	mxl_i2s_tx_start(priv, count - skip);
	if (!wait_for_completion_timeout(&priv->tx.complete,
					 msecs_to_jiffies(5000)))
		dev_err(priv->dev, "Timedout waiting for tx finish\n");

	mxl_i2s_tx_stop(priv);

exit:
	if (priv->tx.buf) {
		devm_kfree(priv->dev, priv->tx.buf);
		priv->tx.buf = NULL;
		priv->tx.len = 0;
	}

	return count;
}

static const struct file_operations mxl_i2s_debugfs_ops = {
	.owner		= THIS_MODULE,
	.open		= mxl_i2s_debugfs_open,
	.read		= seq_read,
	.write		= mxl_i2s_debugfs_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static void mxl_i2s_cleanup(struct mxl_i2s_data *priv)
{
	writel(I2S_CLC_STOP, priv->base + I2S_CLC);
	writel(0, priv->base + I2S_CTRL);

	debugfs_remove_recursive(priv->debugfs);

	if (priv->rst)
		reset_control_assert(priv->rst);

	if (!IS_ERR(priv->clk))
		clk_disable_unprepare(priv->clk);
}

static int mxl_i2s_remove(struct platform_device *pdev)
{
	struct mxl_i2s_data *priv;

	priv = platform_get_drvdata(pdev);
	mxl_i2s_cleanup(priv);

	return 0;
}

static int mxl_i2s_probe(struct platform_device *pdev)
{
	struct mxl_i2s_data *priv;
	struct dentry *node;
	int ret = 0;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	platform_set_drvdata(pdev, priv);
	priv->dev = &pdev->dev;

	priv->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(priv->base)) {
		ret = PTR_ERR(priv->base);
		goto exit;
	}

	priv->clk = devm_clk_get(priv->dev, "freq");
	if (IS_ERR(priv->clk)) {
		dev_err(priv->dev, "Failed to get clk\n");
		ret = PTR_ERR(priv->clk);
		goto exit;
	}

	ret = clk_prepare_enable(priv->clk);
	if (ret) {
		dev_err(priv->dev, "Failed to enable clk\n");
		goto exit;
	}

	priv->rst = devm_reset_control_get(priv->dev, "i2s");
	if (IS_ERR(priv->rst)) {
		dev_err(priv->dev, "Failed to get reset\n");
		ret = PTR_ERR(priv->rst);
		goto exit;
	}

	priv->irq = platform_get_irq(pdev, 0);
	if (priv->irq < 0) {
		dev_err(priv->dev, "Failed to get irq\n");
		ret = priv->irq;
		goto exit;
	}

	ret = devm_request_threaded_irq(priv->dev, priv->irq, NULL, mxl_i2s_irq,
					IRQF_SHARED | IRQF_ONESHOT,
					dev_name(priv->dev), priv);
	if (ret) {
		dev_err(priv->dev, "Failed to request irq\n");
		goto exit;
	}

	init_completion(&priv->tx.complete);

	reset_control_deassert(priv->rst);
	writel(I2S_CTRL_I2SON, priv->base + I2S_CTRL);

	ret = devm_snd_soc_register_component(priv->dev, &i2s_component,
					      &i2s_dai, 1);
	if (ret) {
		dev_err(priv->dev, "Failed to register component\n");
		goto exit;
	}

	priv->debugfs = debugfs_create_dir("mxl_i2s", NULL);
	if (!priv->debugfs) {
		dev_err(priv->dev, "Failed to create debugfs\n");
		goto exit;
	}

	node = debugfs_create_file("txbuf", 0644, priv->debugfs, priv,
				   &mxl_i2s_debugfs_ops);
	if (!node) {
		dev_err(priv->dev, "Failed to create debugfs node\n");
		goto exit;
	}

exit:
	if (ret)
		mxl_i2s_cleanup(priv);

	return ret;
}

static const struct of_device_id mxl_i2s_of_match[] = {
	{
		.compatible = "mxl,lgm-i2s",
	},
	{},
};
MODULE_DEVICE_TABLE(of, mxl_i2s_of_match);

static struct platform_driver mxl_i2s_driver = {
	.driver = {
		.name = "mxl,i2s",
		.of_match_table = mxl_i2s_of_match,
	},
	.probe = mxl_i2s_probe,
	.remove = mxl_i2s_remove,
};
module_platform_driver(mxl_i2s_driver);

MODULE_DESCRIPTION("Maxlinear SoC i2s Interface");
MODULE_LICENSE("GPL");
