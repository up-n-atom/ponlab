/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2021-2024 MaxLinear, Inc.
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

#ifndef _MXL_SEC_H
#define _MXL_SEC_H

#include <dt-bindings/soc/mxl,noc_fw_defs.h>

/* MXL firewall data structure and APIs */
enum firewall_id {
	HSIOL_FW,
	HSIOR_FW,
	PERI_FW,
	SSB_FW,
	ETH_FW,
	LBAXI_FW,
	DDR_FW,
	FW_MAX
};

enum fw_perm {
	FW_NO_ACCESS  = BIT(0),
	FW_READ_ONLY  = BIT(1),
	FW_WRITE_ONLY = BIT(2),
	FW_READ_WRITE = BIT(3),
};

/* Notfiy chain event data info */
struct fw_nb_data {
	struct device *dev;  /* dev of IA */
	int sai; /* if sai is -1, derive it from dev, otherwise use given val */
	u64 start_addr;
	u64 size;
	enum fw_perm permission;
};

struct fw_nb_resolve_data {
	struct device *dev; /* dev of domain */
	u64 pseudo_addr;
	u64 resolve_addr;
};

struct fw_addr_pair {
	u64 pcie_pseudo_suffix;
	u64 resolve_addr;
};

struct fw_nb_pcie_resolve_data {
	struct device *dev; /* dev of domain */
	unsigned int num;  /* number of pair of addresses */
	struct fw_addr_pair *pair;
};

/**
 * This is a unified structure for firewall events
 * fw_nb_data/fw_nb_resolve_data can be used separately for different events
 */
struct fw_nb_event_data {
	struct device *dev;
	union {
		struct {
			int sai;
			u64 start_addr;
			u64 size;
			enum fw_perm permission;
		} fw_rule_op;

		struct {
			u64 spl_region_id;
			u64 addr;
		} fw_addr_resolve;

		struct {
			unsigned int num;
			struct fw_addr_pair *pair;
		} fw_pcie_addr_resolve;
	} nb_event;
};

struct fw_nb_apply_deferred_data {
	struct device *dev;
};

/* Notify chain event ID */
enum {
	NOC_FW_EVENT_ADD,
	NOC_FW_EVENT_ADD_SPL, /* add and split, should be default for add */
	NOC_FW_EVENT_ADD_IND, /* add without check, only for temp rule*/
	NOC_FW_EVENT_UPDATE,  /* Rule address range must exactly match */
	NOC_FW_EVENT_UPDATE_SPL,
	NOC_FW_EVENT_DEL,
	NOC_FW_EVENT_DEL_IND,
	NOC_FW_EVENT_CHK,
	NOC_FW_EVENT_OP_MAX,
	NOC_FW_EVENT_RESOLVE_ADDR,
	NOC_FW_EVENT_APPLY_DEFE_RULE,
	NOC_FW_EVENT_PCIE_RESOLVE_ADDR,
	NOC_FW_EVENT_MAX
};

#if IS_ENABLED(CONFIG_MXL_NOC_FIREWALL)
int mxl_fw_notifier_blocking_chain(unsigned long val, void *v);
#else
static inline int mxl_fw_notifier_blocking_chain(unsigned long val, void *v)
{
	return NOTIFY_OK;
}
#endif

static inline unsigned long fw_pcie_sai_to_prefix(int sai)
{
	switch (sai) {
	case SAI_IA_PCI_0_VAL:
		return NOC_FW_DYN_PCIE30_PREFIX;
	case SAI_IA_PCI_1_VAL:
		return NOC_FW_DYN_PCIE31_PREFIX;
	case SAI_IA_PCI_2_VAL:
		return NOC_FW_DYN_PCIE10_PREFIX;
	case SAI_IA_PCI_3_VAL:
		return NOC_FW_DYN_PCIE11_PREFIX;
	case SAI_IA_PCI_4_VAL:
		return NOC_FW_DYN_PCIE40_PREFIX;
	case SAI_IA_PCI_5_VAL:
		return NOC_FW_DYN_PCIE41_PREFIX;
	case SAI_IA_PCI_6_VAL:
		return NOC_FW_DYN_PCIE20_PREFIX;
	case SAI_IA_PCI_7_VAL:
		return NOC_FW_DYN_PCIE21_PREFIX;
	}

	return -1;
}

/* MXL firmware download structure and APIs */
#include <linux/dma-mapping.h>
#include <linux/firmware.h>

enum mxl_fw_opt {
	/* Skip Deleting ATOM RW rule */
	MXL_FW_OPT_SKIP_FWRULE = BIT(0),
	/* Peripheral requests to use noncoherent address */
	MXL_FW_OPT_USE_NONCOHERENT = BIT(1),
	/* Skip peripheral firewall Readonly rule */
	MXL_FW_OPT_SKIP_HW_FWRULE = BIT(2),
	/* Don't allocate/free buf for caller */
	MXL_FW_OPT_BUF_FROM_CALLER = BIT(3),
	/* Give RW permission for hardware instead of default RO */
	MXL_FW_OPT_RW_HW_FWRULE = BIT(4),
};

/**
 * struct mxl_fw_dl_data  -  parameters from user driver
 * @dev: User device pointer
 * @sai: SAI of the HW module, It should be only used in special case.
 * @opt: options.
 */
struct mxl_fw_dl_data {
	struct device *dev;
	int sai;
	unsigned long opt;
};

#if IS_ENABLED(CONFIG_MXL_FW_DOWNLOAD)
int mxl_soc_request_firmware(const struct firmware **fw, char *name,
			     struct device *device, size_t size,
			     struct mxl_fw_dl_data *data);

void mxl_soc_release_firmware(const struct firmware *fw, struct device *device,
			      const void *buf, size_t size,
			      struct mxl_fw_dl_data *data);

int mxl_soc_request_firmware_into_buf(const struct firmware **firmware_p,
				      const char *name, struct device *device,
				      void *buf, size_t size,
				      struct mxl_fw_dl_data *data);

void *mxl_soc_alloc_firmware_buf(struct device *device, size_t size,
				 struct mxl_fw_dl_data *data);
void mxl_soc_free_firmware_buf(struct device *device, const void *buf,
			       size_t size, struct mxl_fw_dl_data *data);

/* Add RO firewall region */
int mxl_soc_request_protect_buf(struct device *device, const void *buf,
				size_t size, struct mxl_fw_dl_data *data);
/* Add RW firewall region */
int mxl_soc_request_unprotect_buf(struct device *device, const void *buf,
				  size_t size, struct mxl_fw_dl_data *data);

/* Add RO firewall region from device */
int mxl_soc_request_protect_buf_from_dev(struct device *device,
					 const void *buf, size_t size,
					 struct mxl_fw_dl_data *data);
/* Add RW firewall region from device */
int mxl_soc_request_unprotect_buf_from_dev(struct device *device,
					   const void *buf, size_t size,
					   struct mxl_fw_dl_data *data);

#else
static inline int
mxl_soc_request_firmware(const struct firmware **fw, char *name,
			 struct device *device, size_t size,
			 struct mxl_fw_dl_data *data)
{
	return request_firmware(fw, name, device);
}

static inline void
mxl_soc_release_firmware(const struct firmware *fw, struct device *device,
			 const void *buf, size_t size,
			 struct mxl_fw_dl_data *data)
{
	if (fw)
		release_firmware(fw);
}

static inline void *
mxl_soc_alloc_firmware_buf(struct device *device, size_t size,
			   struct mxl_fw_dl_data *data)
{
	dma_addr_t dma_handle;

	return dmam_alloc_coherent(device, size, &dma_handle, GFP_KERNEL);
}

static inline void
mxl_soc_free_firmware_buf(struct device *device, const void *buf, size_t size,
			  struct mxl_fw_dl_data *data)
{
}

static inline int
mxl_soc_request_firmware_into_buf(const struct firmware **firmware_p,
				  const char *name, struct device *device,
				  void *buf, size_t size,
				  struct mxl_fw_dl_data *data)
{
	return request_firmware_into_buf(firmware_p, name, device, buf, size);
}

static inline int
mxl_soc_request_protect_buf(struct device *device, const void *buf, size_t size,
			    struct mxl_fw_dl_data *data)
{
	return 0;
}

static inline int
mxl_soc_request_unprotect_buf(struct device *device, const void *buf,
			      size_t size, struct mxl_fw_dl_data *data)
{
	return 0;
}

static inline
int mxl_soc_request_protect_buf_from_dev(struct device *device,
					 const void *buf, size_t size,
					 struct mxl_fw_dl_data *data)
{
	return 0;
}

static inline
int mxl_soc_request_unprotect_buf_from_dev(struct device *device,
					   const void *buf, size_t size,
					   struct mxl_fw_dl_data *data)
{
	return 0;
}

#endif /* CONFIG_MXL_FW_DOWNLOAD */

#endif /* _MXL_SEC_H */
