// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2021-2024 MaxLinear, Inc.
 *
 * lgm_noc_firewall.c - NOC bus firewall driver.
 * Driver design one domain for one device
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
#include <asm/dma-mapping.h>
#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/notifier.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/firewall.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <dt-bindings/soc/mxl,firewall.h>
#include <soc/mxl/mxl_sec.h>
#include <soc/mxl/mxl_gen_pool.h>
#include <uapi/linux/icc/drv_icc.h>

struct fw_device;
static struct mutex frw_lock;

enum fw_action {
	FW_RULE_ADD,
	FW_RULE_ADD_SPL,
	FW_RULE_ADD_IND,
	FW_RULE_UPDATE,
	FW_RULE_UPDATE_SPL,
	FW_RULE_DEL,
	FW_RULE_DEL_IND,
	FW_RULE_MAX
};

enum fw_status {
	FW_RULE_NOT_APPLIED,
	FW_RULE_APPLIED,
	FW_RULE_REJECTED
};

struct fw_pseudo_addrs {
	u64 addr[64];
};

struct fw_rule_info {
	struct list_head list;
	int sai;
	int independent;
	u64 start_addr;
	u64 end_addr;
	u32 permission;
	enum fw_action op;
	int deferred;
	int pseudo_flags;
	enum fw_status stat;
	enum firewall_id fw_id; /* reserved for future use */
	bool is_static; /* NOC FE driver don't revoke static rules */
};

struct fw_domain_info {
	struct device *dev; /* initialized user device in dev attach */
	struct fw_device *fwdev; /* firewall domain device */
	struct device_node *np; /* device node of static rules */
	const char *name;
	int sai;  /* effective SAI ID, negative value means N.A. */
	struct list_head rules;
	struct list_head dm_list;
	int status; /* on / off status */
	struct generic_fw_domain genfd;
	struct mutex mlock; /* lock to protect rule list */
	struct dentry *debugfs;
};

struct fw_device {
	struct device *dev;
	struct list_head domains;
	struct list_head fd_list;
	struct notifier_block nb;
	struct dentry *debugfs;
	struct fw_pseudo_addrs addrs;
};

struct fw_fd_list {
	struct list_head list;
	const struct fw_domain_info *fd;
	struct list_head rules;
};

enum {
	LGM_DM_STATUS_OFF = 0,
	LGM_DM_STATUS_ON,
};

static int noc_fw_debug;

struct fw_sai {
	int id;
	const char *name;
};

#define SAI_NAME(x)	(#x)

static const struct fw_sai fw_sais[] = {
	{ LGM_SAI_CPU,     SAI_NAME(LGM_SAI_CPU)    },
	{ LGM_SAI_MEMAXI,  SAI_NAME(LGM_SAI_MEMAXI) },
	{ LGM_SAI_DMATX,   SAI_NAME(LGM_SAI_DMATX)  },
	{ LGM_SAI_DMARX,   SAI_NAME(LGM_SAI_DMARX)  },
	{ LGM_SAI_DMA3,    SAI_NAME(LGM_SAI_DMA3)   },
	{ LGM_SAI_DMA0,    SAI_NAME(LGM_SAI_DMA0)   },
	{ LGM_SAI_TOEDMA,  SAI_NAME(LGM_SAI_TOEDMA) },
	{ LGM_SAI_PCIE10,  SAI_NAME(LGM_SAI_PCIE10) },
	{ LGM_SAI_PCIE11,  SAI_NAME(LGM_SAI_PCIE11) },
	{ LGM_SAI_PCIE20,  SAI_NAME(LGM_SAI_PCIE20) },
	{ LGM_SAI_PCIE21,  SAI_NAME(LGM_SAI_PCIE21) },
	{ LGM_SAI_PCIE30,  SAI_NAME(LGM_SAI_PCIE30) },
	{ LGM_SAI_PCIE31,  SAI_NAME(LGM_SAI_PCIE31) },
	{ LGM_SAI_PCIE40,  SAI_NAME(LGM_SAI_PCIE40) },
	{ LGM_SAI_PCIE41,  SAI_NAME(LGM_SAI_PCIE41) },
	{ LGM_SAI_USB1,    SAI_NAME(LGM_SAI_USB1)   },
	{ LGM_SAI_USB2,    SAI_NAME(LGM_SAI_USB2)   },
	{ LGM_SAI_SATA0,   SAI_NAME(LGM_SAI_SATA0)  },
	{ LGM_SAI_SATA1,   SAI_NAME(LGM_SAI_SATA1)  },
	{ LGM_SAI_SATA2,   SAI_NAME(LGM_SAI_SATA2)  },
	{ LGM_SAI_SATA3,   SAI_NAME(LGM_SAI_SATA3)  },
	{ LGM_SAI_V130,    SAI_NAME(LGM_SAI_V130)   },
	{ LGM_SAI_CQMDEQ,  SAI_NAME(LGM_SAI_CQMDEQ) },
	{ LGM_SAI_CQMENQ,  SAI_NAME(LGM_SAI_CQMENQ) },
	{ LGM_SAI_PON,     SAI_NAME(LGM_SAI_PON)    },
	{ LGM_SAI_TOE,     SAI_NAME(LGM_SAI_TOE)    },
	{ LGM_SAI_MSIGEN,  SAI_NAME(LGM_SAI_MSIGEN) },
	{ LGM_SAI_VOICE,   SAI_NAME(LGM_SAI_VOICE)  },
	{ LGM_SAI_EIP197,  SAI_NAME(LGM_SAI_EIP197) },
	{ LGM_SAI_SDXC,    SAI_NAME(LGM_SAI_SDXC)   },
	{ LGM_SAI_EMMC,    SAI_NAME(LGM_SAI_EMMC)   },
	{ LGM_SAI_PPV4,    SAI_NAME(LGM_SAI_PPV4)   },
	{ LGM_SAI_EPU,     SAI_NAME(LGM_SAI_EPU)    },
	{ LGM_SAI_VPN,     SAI_NAME(LGM_SAI_VPN)    },
	{ -1, 0 },
};

static const char *fw_get_sai_name(int sai)
{
	int i;

	if (sai < 0)
		return "Invalid";

	for (i = 0; fw_sais[i].id != -1; i++) {
		if (fw_sais[i].id == sai)
			return fw_sais[i].name;
	}

	return "Invalid";
}

#if IS_ENABLED(CONFIG_DEBUG_FS)
static int fw_dm_rules_show(struct seq_file *s, void *v)
{
	struct fw_domain_info *fd = s->private;
	struct fw_rule_info *rule;

	static const char * const perm_name[] = {
		"No Permission",
		"Read Permission",
		"Write Permission",
		"",
		"Read & Write Permission"
	};

	static const char * const act_str[] = {
		"ADD",
		"ADD and SPLIT",
		"ADD INDEPENDENT",
		"UPDATE",
		"UPDATE and SPLIT",
		"DEL",
		"DEL INDEPENDENT",
	};

	static const char * const stat_str[] = {
		"RULE NOT APPLIED",
		"RULE APPLIED",
		"RULE REJECTED by TEP",
	};

	mutex_lock(&fd->mlock);
	list_for_each_entry(rule, &fd->rules, list) {
		seq_puts(s, "------------------------------------\n");
		seq_printf(s, "SAI id              %s(0x%x)\n",
			   fw_get_sai_name(rule->sai), rule->sai);
		seq_printf(s, "Start Addr:         0x%llx\n", rule->start_addr);
		seq_printf(s, "End Addr:           0x%llx\n", rule->end_addr);
		seq_printf(s, "Permission:         %s\n",
			   perm_name[(rule->permission & 0xf) >> 1]);
		seq_printf(s, "op:                 %s\n",
			   act_str[rule->op]);
		if (rule->deferred)
			seq_puts(s, "Rule deferred\n");
		if (rule->pseudo_flags)
			seq_puts(s, "Rule address not resolved yet\n");
		seq_printf(s, "status:             %s\n", stat_str[rule->stat]);
	}
	mutex_unlock(&fd->mlock);

	return 0;
}
DEFINE_SHOW_ATTRIBUTE(fw_dm_rules);

static char *get_next_param(char *buf, char **param)
{
	int i;

	for (i = 0; buf[i] == ' '; i++)
		;
	if (buf[i] == 0)
		return NULL;

	*param = buf + i;

	for (i = 0; buf[i] != ' ' && buf[i] != 0; i++)
		;
	buf[i] = 0;

	return buf + i + 1;
}

static void fw_dm_debug_addr_resolve(struct fw_domain_info *fd, char *p)
{
	struct fw_nb_resolve_data nb_data;
	char *cpaddr, *craddr;
	u64 paddr, raddr;

	p = get_next_param(p, &cpaddr);
	if (!p)
		goto __err_help;
	pr_debug("pseudo addr: %s\n", cpaddr);
	if (kstrtou64(cpaddr, 0, &paddr))
		goto __err_help;

	p = get_next_param(p, &craddr);
	if (!p)
		goto __err_help;
	pr_debug("resolve addr: %s\n", craddr);
	if (kstrtou64(craddr, 0, &raddr))
		goto __err_help;

	nb_data.dev = fd->dev;
	nb_data.pseudo_addr = paddr;
	nb_data.resolve_addr = raddr;
	mxl_fw_notifier_blocking_chain(NOC_FW_EVENT_RESOLVE_ADDR, &nb_data);

	return;

__err_help:
	pr_info("Please run cat debug for command format information\n");
}

#define DEBUG_ADDR_PAIR_NUM 10
static void fw_dm_debug_pcie_addr_resolve(struct fw_domain_info *fd, char *p)
{
	struct fw_nb_pcie_resolve_data nb_data;
	char *cpaddr, *craddr, *cnum;
	u64 paddr, raddr;
	unsigned int num, i;
	struct fw_addr_pair pair[DEBUG_ADDR_PAIR_NUM];

	p = get_next_param(p, &cnum);
	if (!p)
		goto __err_help;
	pr_debug("address pair num: %s\n", cnum);
	if (kstrtou32(cnum, 0, &num) || num > DEBUG_ADDR_PAIR_NUM)
		goto __err_help;

	for (i = 0; i < num; i++) {
		p = get_next_param(p, &cpaddr);
		if (!p)
			goto __err_help;
		pr_debug("pseudo pcie addr suffix: %s\n", cpaddr);
		if (kstrtou64(cpaddr, 0, &paddr))
			goto __err_help;

		p = get_next_param(p, &craddr);
		if (!p)
			goto __err_help;
		pr_debug("resolve addr: %s\n", craddr);
		if (kstrtou64(craddr, 0, &raddr))
			goto __err_help;

		pair[i].pcie_pseudo_suffix = paddr;
		pair[i].resolve_addr = raddr;
	}

	nb_data.dev  = fd->dev;
	nb_data.num  = num;
	nb_data.pair = pair;
	mxl_fw_notifier_blocking_chain(NOC_FW_EVENT_PCIE_RESOLVE_ADDR, &nb_data);

	return;

__err_help:
	pr_info("Please run cat debug for command format information\n");
}

static void fw_dm_debug_apply_deferred_rules(struct fw_domain_info *fd)
{
	struct fw_nb_apply_deferred_data nb_data;

	nb_data.dev = fd->dev;
	mxl_fw_notifier_blocking_chain(NOC_FW_EVENT_APPLY_DEFE_RULE, &nb_data);
}

static ssize_t fw_dm_debug_write(struct file *filp, const char __user *buffer,
				 size_t count, loff_t *pos)
{
	char buf[128] = {0};
	char *cop, *csai, *caddr, *csize, *cperm, *p;
	int op, sai, perm, len, ret;
	u64 addr, size;
	struct fw_nb_data nb_data;
	struct fw_domain_info *fd =
		((struct seq_file *)filp->private_data)->private;

	len = count < sizeof(buf) ? count : sizeof(buf) - 1;
	if (copy_from_user(buf, buffer, len))
		return -EFAULT;

	pr_debug("buf: %s\n", buf);
	p = buf;

	p = get_next_param(p, &cop);
	if (!p)
		goto __err_help;
	pr_debug("op: %s\n", cop);
	if (kstrtou32(cop, 0, &op))
		goto __err_help;

	if (op == NOC_FW_EVENT_RESOLVE_ADDR) {
		fw_dm_debug_addr_resolve(fd, p);
		return count;
	} else if (op == NOC_FW_EVENT_APPLY_DEFE_RULE) {
		fw_dm_debug_apply_deferred_rules(fd);
		return count;
	} else if (op == NOC_FW_EVENT_PCIE_RESOLVE_ADDR) {
		fw_dm_debug_pcie_addr_resolve(fd, p);
		return count;
	}

	if (op > NOC_FW_EVENT_OP_MAX)
		goto __err_help;
	p = get_next_param(p, &csai);
	if (!p)
		goto __err_help;
	pr_debug("sai: %s\n", csai);
	if (kstrtou32(csai, 0, &sai))
		goto __err_help;

	p = get_next_param(p, &caddr);
	if (!p)
		goto __err_help;
	pr_debug("addr: %s\n", caddr);
	if (kstrtou64(caddr, 0, &addr))
		goto __err_help;

	p = get_next_param(p, &csize);
	if (!p)
		goto __err_help;
	pr_debug("size: %s\n", csize);
	if (kstrtou64(csize, 0, &size))
		goto __err_help;

	p = get_next_param(p, &cperm);
	if (!p)
		goto __err_help;
	pr_debug("perm: %s\n", cperm);
	if (kstrtou32(cperm, 0, &perm))
		goto __err_help;

	nb_data.dev = fd->dev;
	nb_data.sai = sai;
	nb_data.start_addr = addr;
	nb_data.size = size;
	nb_data.permission = BIT(perm);
	ret = mxl_fw_notifier_blocking_chain(op, &nb_data);
	if (op == NOC_FW_EVENT_CHK) {
		if (ret != NOTIFY_OK)
			pr_err("access rejected by TEP\n");
		else
			pr_info("access accepted by TEP\n");
	}

	return count;

__err_help:
	pr_info("Please run cat debug for command format info\n");
	return count;
}

static int fw_dm_debug_show(struct seq_file *s, void *v)
{
	seq_puts(s, "debug firewall operate function:\n");
	seq_puts(s, "echo <op> <sai> <start addr> <size> <perm> > debug\n");
	seq_printf(s, "op: %u - add, %u - add split, %u - add independent\n",
		   NOC_FW_EVENT_ADD, NOC_FW_EVENT_ADD_SPL, NOC_FW_EVENT_ADD_IND);
	seq_printf(s, "op: %u - update, %u - update split\n",
		   NOC_FW_EVENT_UPDATE, NOC_FW_EVENT_UPDATE_SPL);
	seq_printf(s, "op: %u - delete, %u - delete independent\n",
		   NOC_FW_EVENT_DEL, NOC_FW_EVENT_DEL_IND);
	seq_printf(s, "op: %u - check access\n", NOC_FW_EVENT_CHK);
	seq_puts(s, "perm: 1 - read only, 2 - write only, 3: read & write\n");
	seq_puts(s, "-----------------------------------------------\n\n");

	seq_puts(s, "debug pseudo address feature:\n");
	seq_puts(s, "echo <op> <pseudo addr> <resolve addr> > debug\n");
	seq_printf(s, "op: %u,  address must be 64bits\n",
		   NOC_FW_EVENT_RESOLVE_ADDR);
	seq_puts(s, "-----------------------------------------------\n\n");

	seq_puts(s, "debug pseudo pcie address resolve feature:\n");
	seq_puts(s, "echo <op> <address pair num> <pseudo addr suffix> <resolve addr> ...> debug\n");
	seq_printf(s, "op: %u,  address must be 64bits, address pair num must be less than %d\n",
		   NOC_FW_EVENT_PCIE_RESOLVE_ADDR, DEBUG_ADDR_PAIR_NUM);
	seq_puts(s, "-----------------------------------------------\n\n");

	seq_puts(s, "debug to enable deferred rules\n");
	seq_printf(s, "echo %u > debug\n", NOC_FW_EVENT_APPLY_DEFE_RULE);
	seq_puts(s, "-----------------------------------------------\n\n");

	return 0;
}

static int fw_dm_debug_open(struct inode *inode, struct file *file)
{
	return single_open(file, fw_dm_debug_show, inode->i_private);
}

static const struct file_operations fw_dm_debug_fops = {
	.owner	= THIS_MODULE,
	.open	= fw_dm_debug_open,
	.write	= fw_dm_debug_write,
	.read	= seq_read,
	.llseek = noop_llseek,
};

static int fw_dm_sai_show(struct seq_file *s, void *v)
{
	int i;

	for (i = 0; fw_sais[i].id > 0; i++)
		seq_printf(s, "SAI ID: %4x\t SAI Name: %s\n",
			   fw_sais[i].id, fw_sais[i].name);

	return 0;
}

static int fw_dm_sai_open(struct inode *inode, struct file *file)
{
	return single_open(file, fw_dm_sai_show, inode->i_private);
}

static const struct file_operations fw_dm_sai_fops = {
	.owner	= THIS_MODULE,
	.open	= fw_dm_sai_open,
	.read	= seq_read,
	.llseek = noop_llseek,
};

static int fw_dm_fw_show(struct seq_file *s, void *v)
{
	seq_puts(s, "firewall default operate function:\n");
	seq_puts(s, "echo <op> <firewall_id> > firewall\n");
	seq_puts(s, "op: 0 - disable, 1 - enable\n");
	seq_puts(s, "firewall_id:\n");
	seq_printf(s, "HSIOR      Firewall: %u\n", HSIOR_FW);
	seq_printf(s, "HSIOL      Firewall: %u\n", HSIOL_FW);
	seq_printf(s, "Peripheral Firewall: %u\n", PERI_FW);
	seq_printf(s, "SSB        Firewall: %u\n", SSB_FW);
	seq_printf(s, "Ethernet   Firewall: %u\n", ETH_FW);
	seq_printf(s, "LBAXI      Firewall: %u\n", LBAXI_FW);
	seq_printf(s, "DDR        Firewall: %u\n", DDR_FW);

	return 0;
}

static ssize_t fw_dm_fw_write(struct file *filp, const char __user *buffer,
			      size_t count, loff_t *pos)
{
	char buf[128] = {0};
	char *cop, *cid, *p;
	int op, id, len;

	len = count < sizeof(buf) ? count : sizeof(buf) - 1;
	if (copy_from_user(buf, buffer, len))
		return -EFAULT;

	pr_debug("buf: %s\n", buf);
	p = buf;

	p = get_next_param(p, &cop);
	if (!p)
		goto __err_help;
	pr_debug("op: %s\n", cop);
	if (kstrtou32(cop, 0, &op))
		goto __err_help;

	p = get_next_param(p, &cid);
	if (!p)
		goto __err_help;
	pr_debug("id: %s\n", cid);
	if (kstrtou32(cid, 0, &id))
		goto __err_help;

#if IS_ENABLED(CONFIG_MXL_ICC)
	icc_msc_noc_firewall_cfg(id, op, 0, 0);
#endif
	return count;

__err_help:
	pr_info("Please run cat debug for command format info\n");

	return count;
}

static int fw_dm_fw_open(struct inode *inode, struct file *file)
{
	return single_open(file, fw_dm_fw_show, inode->i_private);
}

static const struct file_operations fw_dm_fw_fops = {
	.owner	= THIS_MODULE,
	.open	= fw_dm_fw_open,
	.write	= fw_dm_fw_write,
	.read	= seq_read,
	.llseek = noop_llseek,
};

static int lgm_fw_domain_debugfs_init(struct fw_domain_info *fd)
{
	struct dentry *file;

	fd->debugfs = debugfs_create_dir(fd->name, fd->fwdev->debugfs);
	if (IS_ERR_OR_NULL(fd->debugfs))
		return PTR_ERR(fd->debugfs);

	file = debugfs_create_file("rules", 0444, fd->debugfs,
				   fd, &fw_dm_rules_fops);
	if (IS_ERR_OR_NULL(file))
		goto err;

	file = debugfs_create_file("debug", 0200, fd->debugfs,
				   fd, &fw_dm_debug_fops);
	if (IS_ERR_OR_NULL(file))
		goto err;

	return 0;

err:
	debugfs_remove_recursive(fd->debugfs);
	return PTR_ERR(file);
}

static void lgm_fw_domain_debugfs_remove(struct fw_domain_info *fd)
{
	debugfs_remove_recursive(fd->debugfs);
}

static int lgm_firewall_debugfs_init(struct fw_device *fwdev)
{
	struct dentry *file;

	fwdev->debugfs = debugfs_create_dir("noc_firewall", NULL);
	if (IS_ERR_OR_NULL(fwdev->debugfs))
		return PTR_ERR(fwdev->debugfs);

	file = debugfs_create_file("sai_list", 0400, fwdev->debugfs,
				   fwdev, &fw_dm_sai_fops);
	if (IS_ERR_OR_NULL(file))
		goto err;

	file = debugfs_create_file("firewall", 0444, fwdev->debugfs,
				   fwdev, &fw_dm_fw_fops);
	if (IS_ERR_OR_NULL(file))
		goto err;

	return 0;

err:
	dev_err(fwdev->dev, "Failed to create firewall debugfs\n");
	debugfs_remove_recursive(fwdev->debugfs);
	return PTR_ERR(file);
}
#else
static int lgm_fw_domain_debugfs_init(struct fw_domain_info *fd)
{
	return 0;
}

static void lgm_fw_domain_debugfs_remove(struct fw_domain_info *fd)
{
}

static int lgm_firewall_debugfs_init(struct fw_device *fwdev)
{
	return 0;
}
#endif

static BLOCKING_NOTIFIER_HEAD(firewall_blocking_chain);

int mxl_fw_notifier_blocking_chain(unsigned long val, void *v)
{
	return blocking_notifier_call_chain(&firewall_blocking_chain, val, v);
}
EXPORT_SYMBOL_GPL(mxl_fw_notifier_blocking_chain);

/**
 * ICC API parameter format:
 * param0: bit 0-7:    sai value
 *         bit 8:      ADD
 *         bit 9:      UPDATE
 *         bit 10:     DEL
 *         bit 11:     SPLIT
 *         bit 12:     INDEPENDENT
 *         bit 13-31:  reserved
 *
 * param1: 64bit start address
 * param2: 64bit end   address
 *
 * param3: bit 0:      No Access
 *         bit 1:      read-only
 *         bit 2:      write-only
 *         bit 3:      read-write
 *         bit 4:      protected access
 *         bit 5-7:    protected value
 *         bit 8:      IED aligned access
 *         bit 9:      virtual machine access
 *         bit 10-15:  reserved
 *         bit 16-24:  virtual user
 *         bit 25-31:  reserved
 */

static u32 lgm_form_param0(int sai, enum fw_action action)
{
	u32 param = 0;
	unsigned int action_map[] = {
		BIT(8),			/* ADD */
		BIT(8) | BIT(11),	/* ADD_SPL */
		BIT(8) | BIT(12),	/* ADD_IND */
		BIT(9),			/* UPDATE */
		BIT(9) | BIT(11),	/* UPDATE_SPL */
		BIT(10),		/* DEL */
		BIT(10) | BIT(12),	/* DEL_IND */
	};

	param = sai & 0xFF;

	return param | action_map[action];
}

static phys_addr_t lgm_form_addr_param(int sai, phys_addr_t addr)
{
	/* for MEMAXI rules, it must be NON-Coherent address as per LGM SPEC */

	if (sai != LGM_SAI_MEMAXI || (addr & BIT_ULL(35)))
		return addr;

	return ioc_addr_to_nioc_addr(addr);
}

static u32 lgm_form_param3(unsigned int perm)
{
	u32 param = 0;

	param |= perm;

	return param;
}

static int
lgm_fw_send_request(struct fw_rule_info *rule, enum fw_action action)
{
	u32 param0, param3;
	phys_addr_t start, end;
#if IS_ENABLED(CONFIG_MXL_ICC)
	int ret;
#endif

	static const char * const act_str[] = {
		"ADD",
		"ADD_SPL",
		"ADD_IND",
		"UPDATE",
		"UPDATE_SPL",
		"DEL",
		"DEL_IND",
	};

	static const char * const perm_str[] = {
		"No Permission",
		"Read Only",
		"Write Only",
		"",
		"Read and Write"
	};

#if IS_ENABLED(CONFIG_MXL_ICC)
	static const char * const tep_ret_str[] = {
		"SUCCESSFUL",
		"POLICY_NOT_FOUND_ERROR",
		"RULE_NO_MATCH_ERROR",
		"RULE_EXACT_MATCH_ERROR",
		"RULE_PARTIAL_MATCH_ERROR",
		"RULE_MORE_THAN_ONE_MATCH_ERROR",
		"POLICY_ERROR",
		"RULE_ERROR",
		"RULE_IDX_FULL_ERROR",
		"NOC_TO_FRW_MAPPING_ERROR"
	};
#endif

	pr_debug("%s FW rule: SAI: 0x%x, name: %s,  base: 0x%llx, end: 0x%llx, perm: %s\n",
		 act_str[action],
		 rule->sai, fw_get_sai_name(rule->sai),
		 rule->start_addr, rule->end_addr,
		 perm_str[rule->permission >> 1]);

	param0 = lgm_form_param0(rule->sai, action);
	start = lgm_form_addr_param(rule->sai, rule->start_addr);
	end = lgm_form_addr_param(rule->sai, rule->end_addr);
	param3 = lgm_form_param3(rule->permission);
	pr_debug("p0: 0x%x, p1: 0x%llx, p2: 0x%llx, p3: 0x%x\n",
		 param0, start, end, param3);

#if IS_ENABLED(CONFIG_MXL_ICC)
	mutex_lock(&frw_lock);
	ret = icc_msc_noc_firewall_sync_cfg(param0, start, end, param3);
	mutex_unlock(&frw_lock);
	if (ret > 0) {
		rule->stat = FW_RULE_REJECTED;
		pr_debug("===Rule rejected by TEP FW: %s====\n", tep_ret_str[ret]);
	} else if (ret == 0) {
		rule->stat = FW_RULE_APPLIED;
	} else {
		pr_debug("===Rule rejected by TPE FW: ====\n");
	}
#else
	rule->stat = FW_RULE_APPLIED;
#endif

	return 0;
}

static struct fw_domain_info *genfd_to_fd(struct generic_fw_domain *gfd)
{
	return container_of(gfd, struct fw_domain_info, genfd);
}

static int lgm_fw_check_rule(int sai, u64 start, u64 end, u32 perm)
{
	u32 param0, param3;
	phys_addr_t loaddr, hiaddr;

	param0 = sai & 0xff;
	loaddr = lgm_form_addr_param(sai, start);
	hiaddr = lgm_form_addr_param(sai, end);
	param3 = lgm_form_param3(perm);

	pr_debug("p0: 0x%x, p1: 0x%llx, p2: 0x%llx, p3: 0x%x\n",
		 param0, loaddr, hiaddr, param3);

#if IS_ENABLED(CONFIG_MXL_ICC)
	return icc_msc_noc_firewall_check_access(param0, loaddr,
						 hiaddr, param3);
#else
	return 0;
#endif
}

/**
 * assumption that all dynamic rule can be reverted
 * if this assumption not meet, driver has to remove all the sanity check.
 * Same for modify rule function.
 */
static int lgm_fw_del_rule(struct fw_domain_info *fd, int sai,
			   u64 start, u64 end, enum fw_action action)
{
	struct fw_rule_info *rule, *next;
	int ind;

	dev_dbg(fd->fwdev->dev, "Del(0x%llx:0x%llx) in domain: %s\n",
		start, end, fd->name);

	ind = action == FW_RULE_DEL_IND ? 1 : 0;
	mutex_lock(&fd->mlock);
	list_for_each_entry_safe(rule, next, &fd->rules, list) {
		if (rule->start_addr == start &&
		    rule->end_addr == end &&
		    rule->sai == sai &&
		    rule->independent == ind) {
			list_del(&rule->list);
			mutex_unlock(&fd->mlock);
			goto _rm_rule;
		}
	}
	mutex_unlock(&fd->mlock);

	dev_dbg(fd->fwdev->dev,
		"rule(sai:0x%x:0x%llx:0x%llx) not found in domain: %s\n",
		sai, start, end, fd->name);
	return -ENODEV;

_rm_rule:
	lgm_fw_send_request(rule, action);
	kfree(rule);

	return 0;
}

static int lgm_fw_update_rule(struct fw_domain_info *fd, int sai,
			      u64 start, u64 end, u32 perm,
			      enum fw_action action)
{
	struct fw_rule_info *rule, *next;

	dev_dbg(fd->fwdev->dev, "Update(0x%llx:0x%llx) in domain: %s\n",
		start, end, fd->name);

	mutex_lock(&fd->mlock);
	list_for_each_entry_safe(rule, next, &fd->rules, list) {
		if (rule->start_addr <= start &&
		    rule->end_addr >= end &&
		    rule->sai == sai && rule->independent == 0
		    ) {
			rule->start_addr = start;
			rule->end_addr = end;
			rule->permission = perm;
			mutex_unlock(&fd->mlock);
			goto _update_rule;
		}
	}
	mutex_unlock(&fd->mlock);

	dev_dbg(fd->fwdev->dev,
		"rule(sai:0x%x:0x%llx:0x%llx) not found in domain: %s\n",
		sai, start, end, fd->name);
	return -ENODEV;

_update_rule:
	lgm_fw_send_request(rule, action);

	return 0;
}

static int
lgm_fw_add_rule(struct fw_domain_info *fd, int sai,
		u64 start, u64 end, u32 perm, enum fw_action action)
{
	struct fw_rule_info *rule, *next;

	if (sai < 0) {
		dev_dbg(fd->fwdev->dev,
			"Domain: %s doesn't have valid SAI value\n", fd->name);
		return -EINVAL;
	}

	/* check whether there's exist rule there */
	if (action == FW_RULE_ADD) {
		mutex_lock(&fd->mlock);
		list_for_each_entry_safe(rule, next, &fd->rules, list) {
			if (rule->start_addr <= start &&
			    rule->end_addr >= end &&
			    rule->sai == sai && rule->independent == 0
			    ) {
				mutex_unlock(&fd->mlock);
				goto _add_err;
			}
		}
		mutex_unlock(&fd->mlock);
	}

	rule = kzalloc(sizeof(*rule), GFP_KERNEL);
	if (!rule)
		return -ENOMEM;

	rule->sai = sai;
	rule->start_addr = start;
	rule->end_addr = end;
	rule->permission = perm;
	rule->op = action;
	if (action == FW_RULE_ADD_IND)
		rule->independent = 1;
	else
		rule->independent = 0;

	mutex_lock(&fd->mlock);
	list_add_tail(&rule->list, &fd->rules);
	mutex_unlock(&fd->mlock);

	if (fd->status == LGM_DM_STATUS_ON)
		lgm_fw_send_request(rule, action);

	return 0;

_add_err:
	dev_err(fd->fwdev->dev, "duplicated rule:(sai: 0x%x:0x%llx:0x%llx)\n",
		rule->sai, rule->start_addr, rule->end_addr);
	return -EINVAL;
}

static inline bool match_pseudo_dyn_addr(u64 rule_addr, u64 dyn_addr)
{
	bool match = false;

	/* Only check the LSWord of the special dynamic addresses */
	if ((rule_addr & (NOC_DT_ADDR_SP_REGION_DYN | 0xFFFFFFFF)) ==
	    (dyn_addr & (NOC_DT_ADDR_SP_REGION_DYN | 0xFFFFFFFF)))
		match = (rule_addr >> 32) ? true : false;

	return match;
}

/* Adjust address based on flags defined in DT upper bit address */
static u64 adjust_addr_by_flag(u64 dt_addr, u64 real_addr)
{
	int offset = 0;
	int non_coherent = 0;

	if (dt_addr & NOC_DT_ADDR_SP_PLUS1)
		offset = 1;
	if (dt_addr & NOC_DT_ADDR_SP_MINUS1)
		offset = -1;
	if (dt_addr & NOC_DT_ADDR_SP_NON_COH)
		non_coherent = 1;

	real_addr += offset;
	if (non_coherent)
		/* skip conversion if given address is already non-coherent */
		if (!(real_addr & BIT_ULL(35)))
			real_addr = (u64)ioc_addr_to_nioc_addr(real_addr);

	return real_addr;
}

static void resolve_addr_in_domain(struct fw_domain_info *fd,
				   struct fw_nb_resolve_data *nb_data)
{
	struct fw_rule_info *rule, *next;
	struct list_head tmp_hdr;
	bool match;

	INIT_LIST_HEAD(&tmp_hdr);
	/* search for pseudo address */
	mutex_lock(&fd->mlock);
	list_for_each_entry_safe(rule, next, &fd->rules, list) {
		if (rule->pseudo_flags & BIT(0)) {
			match = match_pseudo_dyn_addr(rule->start_addr,
						      nb_data->pseudo_addr);
			if (match) {
				dev_dbg(fd->fwdev->dev,
					"rule:start<0x%llx>, resolve<0x%llx>, match<%d>\n",
					rule->start_addr, nb_data->resolve_addr,
					match);
				rule->start_addr =
					adjust_addr_by_flag(rule->start_addr,
							    nb_data->resolve_addr);
				rule->pseudo_flags ^= BIT(0);
			}
		}

		if (rule->pseudo_flags & BIT(1)) {
			match = match_pseudo_dyn_addr(rule->end_addr,
						      nb_data->pseudo_addr);
			if (match) {
				dev_dbg(fd->fwdev->dev,
					"rule:end<0x%llx>, resolve<0x%llx>, match<%d>\n",
					rule->end_addr, nb_data->resolve_addr,
					match);
				rule->end_addr =
					adjust_addr_by_flag(rule->end_addr,
							    nb_data->resolve_addr);
				rule->pseudo_flags ^= BIT(1);
			}
		}
	}
	mutex_unlock(&fd->mlock);

	list_for_each_entry(rule, &tmp_hdr, list) {
		if (!rule->deferred) {
			dev_dbg(fd->fwdev->dev, "Trigger Rule OP ...\n");
			lgm_fw_send_request(rule, rule->op);
		}
	}

	/* add back to fd */
	mutex_lock(&fd->mlock);
	list_for_each_entry_safe(rule, next, &tmp_hdr, list) {
		list_del(&rule->list);
		list_add_tail(&rule->list, &fd->rules);
	}
	mutex_unlock(&fd->mlock);
}

static int lgm_fw_resolve_addr(struct fw_device *fwdev, void *data)
{
	struct fw_nb_resolve_data *nb_data = data;
	struct fw_domain_info *fd;
	int match_cnt = 0;

	dev_dbg(fwdev->dev, "nbdata: dev: %s, pseudo_addr: 0x%llx, resolve_addr: 0x%llx\n",
		nb_data->dev ? dev_name(nb_data->dev) : "NULL",
		nb_data->pseudo_addr, nb_data->resolve_addr);

	if (!nb_data->dev)
		return -EINVAL;

	/* find matching domains */
	list_for_each_entry(fd, &fwdev->domains, dm_list)
		if (fd->dev == nb_data->dev) {
			match_cnt++;
			dev_dbg(fwdev->dev, "[%d]Matched domain <%s>\n",
				match_cnt, fd->name);
			resolve_addr_in_domain(fd, nb_data);
		}

	if (!match_cnt) {
		dev_err(fwdev->dev, "Couldn't found firewall domain for request addr: dev: %s\n",
			nb_data->dev ? dev_name(nb_data->dev) : "NULL");

		return -EINVAL;
	}

	return 0;
}

static void apply_def_rules_in_domain(struct fw_domain_info *fd,
				      struct fw_nb_apply_deferred_data *nb_data)
{
	struct fw_rule_info *rule, *next;
	struct list_head tmp_hdr;

	INIT_LIST_HEAD(&tmp_hdr);
	mutex_lock(&fd->mlock);
	list_for_each_entry_safe(rule, next, &fd->rules, list) {
		if (rule->deferred && !rule->pseudo_flags) {
			rule->deferred = 0;
			list_del(&rule->list);
			list_add_tail(&rule->list, &tmp_hdr);
		}
	}
	mutex_unlock(&fd->mlock);

	list_for_each_entry(rule, &tmp_hdr, list)
		lgm_fw_send_request(rule, rule->op);

	mutex_lock(&fd->mlock);
	list_for_each_entry_safe(rule, next, &tmp_hdr, list) {
		list_del(&rule->list);
		list_add_tail(&rule->list, &fd->rules);
	}
	mutex_unlock(&fd->mlock);
}

static int lgm_fw_apply_deferred_rules(struct fw_device *fwdev, void *data)
{
	struct fw_nb_apply_deferred_data *nb_data = data;
	struct fw_domain_info *fd;
	int match_cnt = 0;

	if (!nb_data->dev)
		return -EINVAL;

	/* find matching domains */
	list_for_each_entry(fd, &fwdev->domains, dm_list)
		if (fd->dev == nb_data->dev) {
			match_cnt++;
			apply_def_rules_in_domain(fd, nb_data);
		}

	if (!match_cnt) {
		dev_err(fwdev->dev, "Couldn't found firewall domain for request addr: dev: %s\n",
			nb_data->dev ? dev_name(nb_data->dev) : "NULL");

		return -EINVAL;
	}

	return 0;
}

static u64 lgm_fw_pcie_pseudo_addr(unsigned int sai, unsigned int addr_suffix)
{
	return  (u64)NOC_FW_SPECIAL_REGION_DYN << 32 |
		(fw_pcie_sai_to_prefix(sai) << NOC_FW_DYN_PCIE_PREFIX_OFFSET) | addr_suffix;
}

static int lgm_fw_pcie_addr_resolve(struct fw_device *fwdev, void *data)
{
	struct fw_nb_pcie_resolve_data *nb_data = data;
	struct fw_domain_info *fd;
	int match_cnt = 0;
	int i;

	if (!nb_data->dev)
		return -EINVAL;

	/* find matching domains */
	list_for_each_entry(fd, &fwdev->domains, dm_list)
		if (fd->dev == nb_data->dev) {
			match_cnt++;
			break;
		}

	if (!match_cnt) {
		dev_err(fwdev->dev, "Couldn't found firewall domain for request pcie address resolve: dev: %s\n",
			nb_data->dev ? dev_name(nb_data->dev) : "NULL");

		return -EINVAL;
	}

	for (i = 0; i < nb_data->num; i++) {
		struct fw_nb_resolve_data data;

		data.dev = nb_data->dev;
		data.pseudo_addr = lgm_fw_pcie_pseudo_addr(fd->sai,
							   nb_data->pair[i].pcie_pseudo_suffix);
		data.resolve_addr = nb_data->pair[i].resolve_addr;

		resolve_addr_in_domain(fd, &data);
	}

	return 0;
}

static int fw_blocking_notify(struct notifier_block *nb, unsigned long event,
			      void *data)
{
	struct fw_device *fwdev;
	struct fw_domain_info *fd;
	struct fw_nb_data *nb_data;
	int ret = 0;
	u64 start_addr, end_addr;
	u32 perm;
	int sai;

	fwdev = container_of(nb, struct fw_device, nb);

	switch (event) {
	case NOC_FW_EVENT_RESOLVE_ADDR:
		ret = lgm_fw_resolve_addr(fwdev, data);
		return notifier_from_errno(ret);
	case NOC_FW_EVENT_APPLY_DEFE_RULE:
		ret = lgm_fw_apply_deferred_rules(fwdev, data);
		return notifier_from_errno(ret);
	case NOC_FW_EVENT_PCIE_RESOLVE_ADDR:
		ret = lgm_fw_pcie_addr_resolve(fwdev, data);
		return notifier_from_errno(ret);
	}

	nb_data = data;
	dev_dbg(fwdev->dev, "fwdata: dev: %s, SAI: %d, start addr: 0x%llx, size: 0x%llx, perm: %d\n",
		nb_data->dev ? dev_name(nb_data->dev) : "NULL", nb_data->sai,
		nb_data->start_addr, nb_data->size, nb_data->permission);

	if (nb_data->size <= 0 || (!nb_data->dev && nb_data->sai < 0) ||
	    event >= NOC_FW_EVENT_OP_MAX) {
		dev_dbg(fwdev->dev, "fw notify format error!\n");
		return NOTIFY_BAD;
	}

	list_for_each_entry(fd, &fwdev->domains, dm_list) {
		if (!nb_data->dev)
			/* use default domain */
			break;

		if (fd->dev == nb_data->dev) {
			if (nb_data->sai == -1 || fd->sai == -1)
				/* use first matching device */
				break;

			if (fd->sai == nb_data->sai)
				/* use matching device + sai */
				break;
		}
	}

	if (&fd->dm_list == &fwdev->domains) {
		dev_err(fwdev->dev, "Couldn't found firewall domain for request rule: dev: %s, sai: %s\n",
			nb_data->dev ? dev_name(nb_data->dev) : "NULL",
			fw_get_sai_name(nb_data->sai));

		return NOTIFY_BAD;
	}

	dev_dbg(fwdev->dev, "fw notify found fd: %s\n", fd->name);
	start_addr = nb_data->start_addr;
	end_addr = nb_data->start_addr + nb_data->size - 1;
	perm = nb_data->permission;
	if (nb_data->sai >= 0)
		sai = nb_data->sai;
	else
		sai = fd->sai;

	if (start_addr != ALIGN(start_addr, SZ_4K) ||
	    (nb_data->size > SZ_4K &&
	     nb_data->size != ALIGN(nb_data->size, SZ_4K))) {
		dev_err(fwdev->dev, "Notify start address or size not 4K aligned\n");
		return NOTIFY_BAD;
	}

	switch (event) {
	case NOC_FW_EVENT_DEL:
	case NOC_FW_EVENT_DEL_IND:
		ret = lgm_fw_del_rule(fd, sai, start_addr, end_addr, event);
		break;
	case NOC_FW_EVENT_ADD:
	case NOC_FW_EVENT_ADD_IND:
	case NOC_FW_EVENT_ADD_SPL:
		ret = lgm_fw_add_rule(fd, sai, start_addr,
				      end_addr, perm, event);
		break;
	case NOC_FW_EVENT_UPDATE:
	case NOC_FW_EVENT_UPDATE_SPL:
		ret = lgm_fw_update_rule(fd, sai, start_addr,
					 end_addr, perm, event);
		break;
	case NOC_FW_EVENT_CHK:
		ret = lgm_fw_check_rule(sai, start_addr, end_addr, perm);
		break;
	}

	return notifier_from_errno(ret);
}

static bool is_static_rule_enabled(struct fw_domain_info *fd,
				   struct device_node *np, const char *name)
{
	struct device_node *tnode;
	u32 ph;

	if (!of_property_read_u32(np, name, &ph)) {
		tnode = of_find_node_by_phandle(ph);
		if (!tnode) {
			dev_err(fd->fwdev->dev, "%s node point to nowhere\n",
				name);
			return false;
		}
		of_node_put(tnode);
		return of_device_is_available(tnode);
	}

	return true;
}

static bool lgm_fw_is_pseudo_addr(u64 addr)
{
	if ((addr >> 32) & 0xFFFFFFE0)
		return true;
	else
		return false;
}

static u32 lgm_convert_dt_perm(struct fw_device *fwdev, u32 perm)
{
	switch (perm) {
	case NOC_FW_F_NONE:
		return FW_NO_ACCESS;
	case NOC_FW_F_R:
		return FW_READ_ONLY;
	case NOC_FW_F_W:
		return FW_WRITE_ONLY;
	case NOC_FW_F_RW:
		return FW_READ_WRITE;
	};

	dev_err(fwdev->dev, "DT permission is not correct: %u\n", perm);
	return NOC_FW_F_NONE;
}

static u32 lgm_convert_dt_op(struct fw_device *fwdev, u32 op)
{
	if (op == NOC_FW_OP_ADD)
		return FW_RULE_ADD;

	if (op == NOC_FW_OP_MODIFY)
		return FW_RULE_UPDATE;

	if (op == NOC_FW_OP_MODIFY_SPLIT)
		return FW_RULE_UPDATE_SPL;

	dev_err(fwdev->dev, "Can't identify Firewall OP\n");
	return FW_RULE_ADD;
}

enum dyn_addr_action {
	DYN_ADDR_SET,
	DYN_ADDR_GET,
	DYN_ADDR_DUMP,
};

static u64 lgm_fw_resolve_sp_addr(u32 addr_idx,
				  enum dyn_addr_action action, u64 addr)
{
	static u64 noc_sp_addr[NOC_FW_SP_ADDR_MAX];
	int i;

	if (addr_idx >= NOC_FW_SP_ADDR_MAX)
		return -1;

	if (action == DYN_ADDR_GET)
		return noc_sp_addr[addr_idx - 1];

	if (action == DYN_ADDR_SET) {
		noc_sp_addr[addr_idx - 1] = addr;
		return addr;
	}

	/* dump address, debug only, MACRO Define start from 1 */
	for (i = 0; i < NOC_FW_SP_ADDR_MAX - 1; ++i)
		pr_debug("=====addr[%i]: 0x%llx====\n", i, noc_sp_addr[i]);

	return 0;
}

static int lgm_fw_resolve_pool_addr(void)
{
	struct mxl_pool_event_data data;
	int ret,  event;

	for (event = 0; event < NOC_GENPL_EVENT_MAX; ++event) {
		ret = mxl_noc_genpl_notifier_blocking_chain(event, &data);
		if (notifier_to_errno(ret)) {
			pr_err("Failed to get genpool:%u POOL range!\n", event);
			return -EINVAL;
		}

		switch (event) {
		case NOC_GENPL_EVENT_CPU_POOL:
			lgm_fw_resolve_sp_addr(NOC_FW_CPUPOOL_START, DYN_ADDR_SET, data.paddr);
			lgm_fw_resolve_sp_addr(NOC_FW_CPUPOOL_END, DYN_ADDR_SET,
					       data.paddr + data.size - 1);
			break;
		case NOC_GENPL_EVENT_RO_POOL:
			lgm_fw_resolve_sp_addr(NOC_FW_ROPOOL_START, DYN_ADDR_SET, data.paddr);
			lgm_fw_resolve_sp_addr(NOC_FW_ROPOOL_END, DYN_ADDR_SET,
					       data.paddr + data.size - 1);
			break;
		case NOC_GENPL_EVENT_SYS_POOL:
			lgm_fw_resolve_sp_addr(NOC_FW_SYSPOOL_START, DYN_ADDR_SET, data.paddr);
			lgm_fw_resolve_sp_addr(NOC_FW_SYSPOOL_END, DYN_ADDR_SET,
					       data.paddr + data.size - 1);
			break;
		case NOC_GENPL_EVENT_RW_POOL:
			lgm_fw_resolve_sp_addr(NOC_FW_RWPOOL_START, DYN_ADDR_SET, data.paddr);
			lgm_fw_resolve_sp_addr(NOC_FW_RWPOOL_END, DYN_ADDR_SET,
					       data.paddr + data.size - 1);
			break;
		case NOC_GENPL_EVENT_ICC_POOL:
			lgm_fw_resolve_sp_addr(NOC_FW_ICCPOOL_START, DYN_ADDR_SET, data.paddr);
			lgm_fw_resolve_sp_addr(NOC_FW_ICCPOOL_END, DYN_ADDR_SET,
					       data.paddr + data.size - 1);
			break;
		case NOC_GENPL_EVENT_GENPOOL:
			lgm_fw_resolve_sp_addr(NOC_FW_DDR_GENERIC_RW_END, DYN_ADDR_SET,
					       data.paddr - 1);
			break;
		default:
			pr_err("unsupported pool event!\n");
			return -EINVAL;
		}
	}

	return 0;
}

/**
 * return True if pseudo address
 * return False if resloved
 */
static bool lgm_fw_convert_dt_addr(struct fw_device *fwdev,
				   u64 addr, u64 *caddr)
{
	u32 spaddr;

	if (!lgm_fw_is_pseudo_addr(addr)) {
		*caddr = addr;
		return false;
	}

	if (!(addr & NOC_DT_ADDR_SP_REGION)) {
		*caddr = addr;
		return true;
	}

	spaddr = addr & 0xFFFFFFFF;
	if (spaddr > NOC_FW_DDR_GENERIC_RW_END) {
		dev_err(fwdev->dev,
			"Special address cannot be resolved: 0x%llx\n", addr);
		*caddr = addr;
		return true;
	}

	spaddr = lgm_fw_resolve_sp_addr(spaddr, DYN_ADDR_GET, 0);
	*caddr = adjust_addr_by_flag(addr, spaddr);
	return false;
}

static bool is_domain_loaded(struct fw_device *fwdev, struct fw_domain_info *fd)
{
	struct fw_fd_list *fw_fd;

	list_for_each_entry(fw_fd, &fwdev->fd_list, list) {
		if (fw_fd->fd == fd)
			return true;
	}

	return false;
}

static void add_domain_fd_to_fwdev(struct fw_device *fwdev,
				   const struct fw_domain_info *fd)
{
	struct fw_fd_list *fw_fd;

	fw_fd = kzalloc(sizeof(*fw_fd), GFP_KERNEL);
	if (!fw_fd)
		return;

	INIT_LIST_HEAD(&fw_fd->list);
	fw_fd->fd = fd;
	INIT_LIST_HEAD(&fw_fd->rules);
	list_add_tail(&fw_fd->list, &fwdev->fd_list);
}

static void save_rules_to_fw_fd_list(struct fw_device *fwdev,
				     struct fw_domain_info *fd)
{
	struct fw_fd_list *fw_fd;

	list_for_each_entry(fw_fd, &fwdev->fd_list, list) {
		if (fw_fd->fd == fd && !list_empty(&fd->rules)) {
			list_replace_init(&fd->rules, &fw_fd->rules);
			break;
		}
	}
}

static void restore_rules_from_fw_fd_list(struct fw_device *fwdev,
					  struct fw_domain_info *fd)
{
	struct fw_fd_list *fw_fd;

	list_for_each_entry(fw_fd, &fwdev->fd_list, list) {
		if (fw_fd->fd == fd && !list_empty(&fw_fd->rules)) {
			list_replace_init(&fw_fd->rules, &fd->rules);
			break;
		}
	}
}

static void lgm_fw_domain_add_static_rules(struct fw_domain_info *fd)
{
	struct fw_device *fwdev = fd->fwdev;
	struct fw_rule_info *rule;
	struct device_node *mid, *child;
	u32 sai, perm, op;
	u64 start_addr, end_addr;

	/* No static rules for this domain */
	if (!fd->np) {
		dev_dbg(fwdev->dev, "No static rule for domain: %s\n",
			fd->name);
		return;
	}

	if (is_domain_loaded(fwdev, fd)) {
		dev_dbg(fwdev->dev, "domain: %s has been loaded before!\n",
			fd->name);
		restore_rules_from_fw_fd_list(fwdev, fd);
		return;
	}

	dev_dbg(fwdev->dev, "attach domain %s\n", fd->name);
	add_domain_fd_to_fwdev(fwdev, fd);

	for_each_available_child_of_node(fd->np, mid) {
		for_each_available_child_of_node(mid, child) {
			if (of_property_read_u32(child, "sai", &sai) ||
			    of_property_read_u64(child, "base", &start_addr) ||
			    of_property_read_u64(child, "end", &end_addr) ||
			    of_property_read_u32(child, "perm", &perm) ||
			    of_property_read_u32(child, "op", &op)) {
				dev_err(fwdev->dev, "np(%s)'s format is not correct!\n",
					child->name);
				continue;
			}

			if (!is_static_rule_enabled(fd, child, "initiator"))
				continue;
			if (!is_static_rule_enabled(fd, child, "target"))
				continue;

			rule = kzalloc(sizeof(*rule), GFP_KERNEL);
			if (!rule)
				return;

			if (of_property_read_bool(child, "deferred"))
				rule->deferred = 1;

			if (lgm_fw_convert_dt_addr(fwdev, start_addr, &start_addr))
				rule->pseudo_flags |= BIT(0);

			if (lgm_fw_convert_dt_addr(fwdev, end_addr, &end_addr))
				rule->pseudo_flags |= BIT(1);

			rule->sai = sai;
			rule->start_addr = start_addr;
			rule->end_addr = end_addr;
			rule->permission = lgm_convert_dt_perm(fwdev, perm);
			rule->op = lgm_convert_dt_op(fwdev, op);
			rule->is_static = true;

			dev_dbg(fwdev->dev,
				"Add static rule: sai: 0x%x, start: 0x%llx, end: 0x%llx, per: %u, op: %u\n",
				 rule->sai, rule->start_addr,
				 rule->end_addr, rule->permission, rule->op);
			if (rule->pseudo_flags)
				dev_dbg(fwdev->dev, "Rule pseudo addr\n");
			if (rule->deferred)
				dev_dbg(fwdev->dev, "Rule deferred\n");
			list_add_tail(&rule->list, &fd->rules);
		}
	}
}

static void lgm_fw_domain_remove_rules(struct fw_domain_info *fd)
{
	struct fw_device *fwdev = fd->fwdev;
	struct fw_rule_info *rule, *next;

	list_for_each_entry_safe(rule, next, &fd->rules, list) {
		if (rule->is_static)
			continue;

		list_del(&rule->list);
		/* lgm_fw_send_request(rule, FW_RULE_DEL); */
		kfree(rule);
	}
	save_rules_to_fw_fd_list(fwdev, fd);
}

/**
 * TODO: need check tep fw return value
 * Now the TEP return fail in case rule is duplicated. it cause driver load failure.
 * Need fix it after TEP return success or identified return value
 */
static int lgm_firewall_on(struct generic_fw_domain *genfd)
{
	struct fw_domain_info *fd = genfd_to_fd(genfd);
	struct fw_rule_info *rule;

	mutex_lock(&fd->mlock);
	fd->status = LGM_DM_STATUS_ON;
	list_for_each_entry(rule, &fd->rules, list) {
		if (!rule->deferred && !rule->pseudo_flags &&
		    rule->stat != FW_RULE_APPLIED)
			lgm_fw_send_request(rule, rule->op);
	}
	mutex_unlock(&fd->mlock);

	return 0;
}

/**
 * Here the assumption is if a driver can be load/unloaded,
 * it's rule must only has add.
 * Because modify/delete rule cannot be reverted in unload process.
 */
static int lgm_firewall_off(struct generic_fw_domain *genfd)
{
	struct fw_domain_info *fd = genfd_to_fd(genfd);
	struct fw_rule_info *rule;
	enum fw_action action;

	mutex_lock(&fd->mlock);
	fd->status = LGM_DM_STATUS_OFF;
	list_for_each_entry_reverse(rule, &fd->rules, list) {
		if (rule->deferred || rule->pseudo_flags || rule->is_static) {
			continue;
		} else {
			if (rule->op == FW_RULE_ADD_IND)
				action = FW_RULE_DEL_IND;
			else
				action = FW_RULE_DEL;
			lgm_fw_send_request(rule, action);
		}
	}
	mutex_unlock(&fd->mlock);

	return 0;
}

static int
lgm_fw_attach_dev(struct generic_fw_domain *genfd, struct device *dev)
{
	struct fw_domain_info *fd = genfd_to_fd(genfd);

	fd->dev = dev;

	/* load static rules */
	mutex_lock(&fd->mlock);
	lgm_fw_domain_add_static_rules(fd);
	mutex_unlock(&fd->mlock);

	lgm_fw_domain_debugfs_init(fd);
	return 0;
}

static int
lgm_fw_detach_dev(struct generic_fw_domain *genfd, struct device *dev)
{
	struct fw_domain_info *fd = genfd_to_fd(genfd);

	/* remove and free all rules including static and dynamic rules */
	mutex_lock(&fd->mlock);
	lgm_fw_domain_remove_rules(fd);
	if (fd->np)
		of_node_put(fd->np);
	mutex_unlock(&fd->mlock);

	lgm_fw_domain_debugfs_remove(fd);

	return 0;
}

static int lgm_fw_add_domain(struct fw_device *fwdev, struct device_node *np)
{
	int ret;
	struct fw_domain_info *fd;
	struct generic_fw_domain *genfd;
	u32 sai;

	if (of_property_read_u32(np, "sai", &sai)) {
		dev_err(fwdev->dev,
			"%s: failed to retrieve SAI id\n", np->name);
		return -EINVAL;
	}

	fd = devm_kzalloc(fwdev->dev, sizeof(*fd), GFP_KERNEL);
	if (!fd)
		return -ENOMEM;

	fd->sai = sai;
	fd->status = LGM_DM_STATUS_OFF;
	fd->fwdev = fwdev;
	fd->name = np->name;
	//fd->np = of_find_node_by_name(np, "rules");
	fd->np = np;
	INIT_LIST_HEAD(&fd->rules);
	mutex_init(&fd->mlock);

	genfd = &fd->genfd;
	genfd->name = np->name;
	genfd->ops.firewall_on = lgm_firewall_on;
	genfd->ops.firewall_off = lgm_firewall_off;
	genfd->ops.attach_dev = lgm_fw_attach_dev;
	genfd->ops.detach_dev = lgm_fw_detach_dev;

	dev_dbg(fwdev->dev, "sai: 0x%x, name: %s\n", fd->sai, fd->name);
	ret = genfw_domain_init(genfd, 1);
	if (ret) {
		dev_err(fwdev->dev, "fw domain: %s init failed\n", fd->name);
		return ret;
	}

	list_add_tail(&fd->dm_list, &fwdev->domains);
	return of_genfd_add_provider_simple(np, genfd);
}

/**
 * lgm_firewall_domain_dt_init - parse DT child node and create
 * firewall domain for each child node
 */
static int lgm_firewall_domain_dt_init(struct fw_device *fwdev)
{
	struct device_node *np = fwdev->dev->of_node;
	struct device_node *child;
	int ret;

	if (!np) {
		dev_err(fwdev->dev, "device tree node not found\n");
		return -ENODEV;
	}

	for_each_available_child_of_node(np, child) {
		ret = lgm_fw_add_domain(fwdev, child);
		if (ret) {
			dev_err(fwdev->dev, "failed to handle node %s: %d\n",
				child->name, ret);
			of_node_put(child);
			return ret;
		}
	}

	return 0;
}

static void __maybe_unused disable_firewall(void)
{
	pr_debug("========disable firewall======\n");
	icc_msc_noc_firewall_cfg(HSIOL_FW, 0, 0, 0);
	icc_msc_noc_firewall_cfg(HSIOR_FW, 0, 0, 0);
	icc_msc_noc_firewall_cfg(ETH_FW, 0, 0, 0);
	icc_msc_noc_firewall_cfg(DDR_FW, 0, 0, 0);
	icc_msc_noc_firewall_cfg(SSB_FW, 0, 0, 0);
	icc_msc_noc_firewall_cfg(LBAXI_FW, 0, 0, 0);
	icc_msc_noc_firewall_cfg(PERI_FW, 0, 0, 0);
}

static int lgm_firewall_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct fw_device *fwdev;
	int ret;

	fwdev = devm_kzalloc(dev, sizeof(*fwdev), GFP_KERNEL);
	if (!fwdev)
		return -ENOMEM;

	fwdev->dev = dev;
	INIT_LIST_HEAD(&fwdev->domains);
	INIT_LIST_HEAD(&fwdev->fd_list);
	platform_set_drvdata(pdev, fwdev);

	ret = lgm_firewall_domain_dt_init(fwdev);
	if (ret)
		return ret;

	fwdev->nb.notifier_call = fw_blocking_notify;
	blocking_notifier_chain_register(&firewall_blocking_chain, &fwdev->nb);
	lgm_fw_resolve_pool_addr();
	lgm_fw_resolve_sp_addr(0, DYN_ADDR_DUMP, 0);

	lgm_firewall_debugfs_init(fwdev);
	//disable_firewall();
	mutex_init(&frw_lock);
	dev_info(dev, "LGM firewall driver init done!\n");

	return 0;
}

/* TODO: enable all firewall rules in debug mode */
static int __init lgm_noc_firewall_setup(char *str)
{
	if (!strcmp(str, "debug"))
		noc_fw_debug = 1;

	return 1;
}
__setup("noc_firewall=", lgm_noc_firewall_setup);

static int __init lgm_noc_firewall_kernel_addr_start_setup(char *str)
{
	ssize_t ret;
	unsigned long long addr;

	if (!str)
		return 0;

	ret = kstrtoull(str, 0, &addr);
	if (ret)
		return 0;

	lgm_fw_resolve_sp_addr(NOC_FW_KERNEL_START, DYN_ADDR_SET, addr);
	return 1;
}
__setup("kernel_startaddr=", lgm_noc_firewall_kernel_addr_start_setup);

static int __init lgm_noc_firewall_kernel_addr_end_setup(char *str)
{
	ssize_t ret;
	unsigned long long addr;

	if (!str)
		return 0;

	ret = kstrtoull(str, 0, &addr);
	if (ret)
		return 0;

	lgm_fw_resolve_sp_addr(NOC_FW_KERNEL_END, DYN_ADDR_SET, addr);
	lgm_fw_resolve_sp_addr(NOC_FW_DDR_GENERIC_RW_START,
			       DYN_ADDR_SET, addr + 1);
	return 1;
}
__setup("kernel_endaddr=", lgm_noc_firewall_kernel_addr_end_setup);

static int __init lgm_noc_firewall_rootfs_addr_start_setup(char *str)
{
	ssize_t ret;
	unsigned long long addr;

	if (!str)
		return 0;

	ret = kstrtoull(str, 0, &addr);
	if (ret)
		return 0;

	lgm_fw_resolve_sp_addr(NOC_FW_ROOTFS_START, DYN_ADDR_SET, addr);
	return 1;
}
__setup("rootfs_startaddr=", lgm_noc_firewall_rootfs_addr_start_setup);

static int __init lgm_noc_firewall_rootfs_addr_end_setup(char *str)
{
	ssize_t ret;
	unsigned long long addr;

	if (!str)
		return 0;

	ret = kstrtoull(str, 0, &addr);
	if (ret)
		return 0;

	lgm_fw_resolve_sp_addr(NOC_FW_ROOTFS_END, DYN_ADDR_SET, addr);
	return 1;
}
__setup("rootfs_endaddr=", lgm_noc_firewall_rootfs_addr_end_setup);

/**
 * NOC_FW_DDR_UPPER_3GB_END could be an invalid address if
 * DDR size is less than 3GB.
 * It's fine since there is firewall rule that uses it only if
 * the DDR size larger than 3GB
 */
static int __init lgm_noc_firewall_memsize_setup(char *p)
{
	unsigned long long size, start;
	const unsigned long addr_3gb = 0x100000000UL;

	start = 0;
	size = memparse(p, &p);
	if (*p == '@')
		start = memparse(p + 1, &p);

	lgm_fw_resolve_sp_addr(NOC_FW_DDR_UPPER_4GB_START,
			       DYN_ADDR_SET, addr_3gb);
	lgm_fw_resolve_sp_addr(NOC_FW_DDR_UPPER_4GB_END,
			       DYN_ADDR_SET, size - 1);

	//testing purpose to expand range
	//lgm_fw_resolve_sp_addr(NOC_FW_DDR_GENERIC_RW_END, DYN_ADDR_SET, size - 1);

	return 0;
}
__setup("mem=", lgm_noc_firewall_memsize_setup);

static const struct of_device_id lgm_firewall_dt_ids[] = {
	{.compatible = "maxlinear, firewall" },
	{ },
};

static struct platform_driver lgm_firewall_driver = {
	.probe = lgm_firewall_probe,
	.driver = {
		.name = "mxl-noc-firewall",
		.of_match_table = of_match_ptr(lgm_firewall_dt_ids),
		/* Firewall driver cannot be removed. */
		.suppress_bind_attrs = true,
	},
};

static int __init lgm_firewall_drv_register(void)
{
	return platform_driver_register(&lgm_firewall_driver);
}
arch_initcall_sync(lgm_firewall_drv_register);
