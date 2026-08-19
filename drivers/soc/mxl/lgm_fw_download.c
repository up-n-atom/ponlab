// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2021-2022 MaxLinear, Inc.
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

#include <linux/debugfs.h>
#include <linux/dma-mapping.h>
#include <linux/firmware.h>
#include <linux/genalloc.h>
#include <linux/init.h>
#include <linux/log2.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <soc/mxl/mxl_sec.h>
#include <soc/mxl/mxl_gen_pool.h>
#include <dt-bindings/soc/mxl,firewall.h>

/**
 * struct lgm_fw_dl_dev - private data of firmware download device
 * @dev: device point of fw_dl device
 * @align: align requirement of firewall
 * @pool: pool pointer to ro-pool
 */
struct lgm_fw_dl_dev {
	struct device *dev;
	size_t size;
	unsigned int align;
	struct gen_pool *pool;
};

static struct lgm_fw_dl_dev *g_fwdl;

enum lgm_fw_action {
	FW_ADD_CPU_RW,
	FW_DEL_CPU_RW,
	FW_UPDATE_HW_RW,
	FW_UPDATE_HW_RO,
};

/**
 * sai: MEMAXI or user device's sai
 */
static int lgm_firewall_set(struct lgm_fw_dl_dev *fwdl, const void *buf,
			    size_t size, enum lgm_fw_action act,
			    struct mxl_fw_dl_data *data)
{
	struct fw_nb_data fwdata = {0};
	int ret;
	unsigned int action;

	if (act == FW_DEL_CPU_RW && (data->opt & MXL_FW_OPT_SKIP_FWRULE))
		return 0;

	fwdata.size = size;
	/**
	 * MEMAXI always use noncoherent address
	 * which translated in firewall FE driver
	 */
	fwdata.start_addr = dma_map_single(data->dev, (void *)buf,
					   size, DMA_TO_DEVICE);
	dma_sync_single_for_device(data->dev, fwdata.start_addr, size,
				   DMA_TO_DEVICE);

	switch (act) {
	case FW_ADD_CPU_RW:
		action = NOC_FW_EVENT_ADD_IND;
		fwdata.dev = fwdl->dev;
		fwdata.sai = LGM_SAI_MEMAXI;
		fwdata.permission = FW_READ_WRITE;
		break;
	case FW_DEL_CPU_RW:
		action = NOC_FW_EVENT_DEL_IND;
		fwdata.dev = fwdl->dev;
		fwdata.sai = LGM_SAI_MEMAXI;
		fwdata.permission = FW_READ_WRITE;
		break;
	case FW_UPDATE_HW_RW:
		action = NOC_FW_EVENT_UPDATE;
		fwdata.dev = data->dev;
		fwdata.sai = data->sai;
		fwdata.permission = FW_READ_WRITE;
		break;
	case FW_UPDATE_HW_RO:
		action = NOC_FW_EVENT_UPDATE;
		fwdata.dev = data->dev;
		fwdata.sai = data->sai;
		fwdata.permission = FW_READ_ONLY;
		break;
	default:
		return -EINVAL;
	}

	ret = mxl_fw_notifier_blocking_chain(action, &fwdata);

	return notifier_to_errno(ret);
}

static void *lgm_fw_dl_request_buf(size_t size, struct mxl_fw_dl_data *data)
{
	struct lgm_fw_dl_dev *fwdl = g_fwdl;
	struct mxl_pool_alloc_data allocdata;
	unsigned long addr;

	allocdata.dev = data->dev;
	allocdata.sai = data->sai;
	allocdata.opt = data->opt;
	if (data->opt & MXL_FW_OPT_RW_HW_FWRULE)
		allocdata.perm = FW_READ_WRITE;
	else
		allocdata.perm = FW_READ_ONLY;

	addr = mxl_soc_pool_alloc(fwdl->pool, size, &allocdata);
	if (!addr)
		dev_err(fwdl->dev, "Failed to allocate size: 0x%lx from genpool\n",
			size);
	return (void *)addr;
}

void *mxl_soc_alloc_firmware_buf(struct device *device, size_t size,
				   struct mxl_fw_dl_data *data)
{
	struct lgm_fw_dl_dev *fwdl = g_fwdl;

	if (!data || ALIGN(size, fwdl->align) != size) {
		dev_err(fwdl->dev,
			"data point is NULL or size is not aligned!\n");
		return NULL;
	}

	if (!data->dev)
		data->dev = device;

	return lgm_fw_dl_request_buf(size, data);
}
EXPORT_SYMBOL_GPL(mxl_soc_alloc_firmware_buf);

int mxl_soc_request_protect_buf(struct device *device, const void *buf,
				size_t size, struct mxl_fw_dl_data *data)
{
	struct lgm_fw_dl_dev *fwdl = g_fwdl;

	if (!data) {
		dev_err(fwdl->dev, "data point is NULL!\n");
		return -EINVAL;
	}

	if (!data->dev)
		data->dev = device;

	data->opt &= ~MXL_FW_OPT_SKIP_FWRULE;
	return lgm_firewall_set(fwdl, buf, size, FW_DEL_CPU_RW, data);

}
EXPORT_SYMBOL_GPL(mxl_soc_request_protect_buf);

int mxl_soc_request_unprotect_buf(struct device *device, const void *buf,
				  size_t size, struct mxl_fw_dl_data *data)
{
	struct lgm_fw_dl_dev *fwdl = g_fwdl;

	if (!data) {
		dev_err(fwdl->dev, "data point is NULL!\n");
		return -EINVAL;
	}

	if (!data->dev)
		data->dev = device;

	data->opt &= ~MXL_FW_OPT_SKIP_FWRULE;
	return lgm_firewall_set(fwdl, buf, size, FW_ADD_CPU_RW, data);
}
EXPORT_SYMBOL_GPL(mxl_soc_request_unprotect_buf);

int mxl_soc_request_protect_buf_from_dev(struct device *device,
					 const void *buf, size_t size,
					 struct mxl_fw_dl_data *data)
{
	struct lgm_fw_dl_dev *fwdl = g_fwdl;

	if (!data) {
		dev_err(fwdl->dev, "data point is NULL!\n");
		return -EINVAL;
	}

	if (!data->dev)
		data->dev = device;

	return lgm_firewall_set(fwdl, buf, size, FW_UPDATE_HW_RO, data);
}
EXPORT_SYMBOL_GPL(mxl_soc_request_protect_buf_from_dev);

int mxl_soc_request_unprotect_buf_from_dev(struct device *device,
					   const void *buf, size_t size,
					   struct mxl_fw_dl_data *data)
{
	struct lgm_fw_dl_dev *fwdl = g_fwdl;

	if (!data) {
		dev_err(fwdl->dev, "data point is NULL!\n");
		return -EINVAL;
	}

	if (!data->dev)
		data->dev = device;

	return lgm_firewall_set(fwdl, buf, size, FW_UPDATE_HW_RW, data);
}
EXPORT_SYMBOL_GPL(mxl_soc_request_unprotect_buf_from_dev);

static int
lgm_firmware_download(struct lgm_fw_dl_dev *fwdl, const struct firmware **fw,
		      const char *name, struct device *device,
		      void *buf, size_t size, struct mxl_fw_dl_data *data)
{
	int ret;

	/* request CPU read/write permssion on this buf */
	if (lgm_firewall_set(fwdl, buf, size, FW_ADD_CPU_RW, data))
		return -EINVAL;

	ret = request_firmware_into_buf(fw, name, device, buf, size);
	if (ret)
		return ret;

	/* Remove CPU read-write temp rule */
	if (lgm_firewall_set(fwdl, buf, size, FW_DEL_CPU_RW, data))
		return -EINVAL;

	return ret;
}

int mxl_soc_request_firmware(const struct firmware **fw, char *name,
			     struct device *device, size_t size,
			     struct mxl_fw_dl_data *data)
{
	struct lgm_fw_dl_dev *fwdl = g_fwdl;
	void *buf;

	if (!data)
		return -EINVAL;

	if (!data->dev)
		data->dev = device;

	buf = lgm_fw_dl_request_buf(size, data);
	if (!buf)
		return -EINVAL;

	return lgm_firmware_download(fwdl, fw, name, device, buf, size, data);
}
EXPORT_SYMBOL_GPL(mxl_soc_request_firmware);

void mxl_soc_free_firmware_buf(struct device *device, const void *buf,
			       size_t size, struct mxl_fw_dl_data *data)
{
	struct lgm_fw_dl_dev *fwdl = g_fwdl;
	struct mxl_pool_free_data freedata;

	if (!buf || !data || data->opt & MXL_FW_OPT_BUF_FROM_CALLER)
		return;

	freedata.dev = data->dev ? data->dev : device;
	freedata.sai = data->sai;
	freedata.opt = data->opt;
	if (data->opt & MXL_FW_OPT_RW_HW_FWRULE)
		freedata.perm = FW_READ_WRITE;
	else
		freedata.perm = FW_READ_ONLY;
	freedata.size = size;
	freedata.vaddr = (unsigned long)buf;

	gen_pool_for_each_chunk(fwdl->pool, mxl_soc_pool_free, &freedata);
}
EXPORT_SYMBOL_GPL(mxl_soc_free_firmware_buf);

/**
 * fw->data might not be the same pointer that user allocated from
 * firmware download driver due to alignment requirement of user.
 * That's why separate buf pointer and size is required in this API.
 */
void mxl_soc_release_firmware(const struct firmware *fw, struct device *device,
			      const void *buf, size_t size,
			      struct mxl_fw_dl_data *data)
{
	if (fw)
		release_firmware(fw);

	mxl_soc_free_firmware_buf(device, buf, size, data);
}
EXPORT_SYMBOL_GPL(mxl_soc_release_firmware);

int mxl_soc_request_firmware_into_buf(const struct firmware **firmware_p,
				      const char *name, struct device *device,
				      void *buf, size_t size,
				      struct mxl_fw_dl_data *data)
{
	struct lgm_fw_dl_dev *fwdl = g_fwdl;

	if (!data)
		return -EINVAL;

	if (!data->dev)
		data->dev = device;

	return lgm_firmware_download(fwdl, firmware_p, name, device,
				     buf, size, data);
}
EXPORT_SYMBOL_GPL(mxl_soc_request_firmware_into_buf);

static int lgm_fw_dl_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct lgm_fw_dl_dev *fwdl;

	fwdl = devm_kzalloc(dev, sizeof(*fwdl), GFP_KERNEL);
	if (!fwdl)
		return 0;

	fwdl->dev = dev;
	fwdl->align = SZ_4K; /* LGM firewall alignment requirement */
	fwdl->pool = of_gen_pool_get(dev->of_node, "pool", 0);
	if (!fwdl->pool) {
		dev_err(dev, "Failed to get RO Pool pointer\n");
		return -ENODEV;
	}
	platform_set_drvdata(pdev, fwdl);

	g_fwdl = fwdl;
	dev_info(dev, "mxl firmware download driver loaded\n");

	return 0;
}

static const struct of_device_id lgm_fw_dl_dt_ids[] = {
	{.compatible = "mxl,lgm-noc-fw-dl"},
	{ },
};

static struct platform_driver lgm_fw_dl_driver = {
	.probe = lgm_fw_dl_probe,
	.driver = {
		.name = "mxl-noc-fw-download-driver",
		.of_match_table = of_match_ptr(lgm_fw_dl_dt_ids),
		/* Firmware download driver cannot be removed. */
		.suppress_bind_attrs = true,
	},
};

static int __init lgm_fw_dl_drv_register(void)
{
	return platform_driver_register(&lgm_fw_dl_driver);
}

device_initcall(lgm_fw_dl_drv_register);
