/******************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 *  The driver implements a simple interface to user space to check
 *  kernel API provided by Pon Multicast driver (mod_pon_mcc)
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/debugfs.h>

#include <pon/pon_mcc_ikm.h>

struct bridge_port_map_data {
	/*
	 * id of a bridge port assigned to the type of packets;
	 * value id = 255 for bridge port data structure
	 * indicates that processing map_value
	 */
	u8 id;
	/* mask of bits to be cleared (16 bits) zero-bits stay unchanged */
	u16 mask_to_clear;
	/* bits marked to be set (16 bits) zero-bits stay unchanged */
	u16 mask_to_set;
};

/* Definitions/shortcuts/usage of bridge_port_map_data */
#define CHKR_BR_PORT_TO_SKIP	255

/*
 * data structure representing entry to be added to multicast table
 * or pattern for entry to be deleted from multicast table
 */
struct dbgfs_table_entry {
	union mcc_l3addr grp, src;
	u8 is_ipv6;
	u8 ifindex;
	u8 flt_mode;
};

static struct dbgfs_table_entry tab_entry;

/*
 * Init values for bridge data:
 * - id - CHKR_BR_PORT_TO_SKIP means that no change will be applied to map value
 * - mask_to_clear - 0xffff means clear all bits (if it comes to apply changes)
 * - mask_to_set - 0x0000 means no bit to set
 */
static struct bridge_port_map_data
	/* bridge port which is the source of multicast data (ANI) */
	br_port_data = {
		CHKR_BR_PORT_TO_SKIP, 0xffff, 0x0000
	},
	/* bridge port which is the first source of membership reports (UNI) */
	br_port_rep0 = {
		CHKR_BR_PORT_TO_SKIP, 0xffff, 0x0000
	},
	/* bridge port which is the second source of membership reports (UNI) */
	br_port_rep1 = {
		CHKR_BR_PORT_TO_SKIP, 0xffff, 0x0000
	},
	/* bridge port which is the source of queries (ANI) */
	br_port_qry = {
		CHKR_BR_PORT_TO_SKIP, 0xffff, 0x0000
	};

static struct dentry *mcc_dbgfs_dir;

static const char usage_info[] =
	"This is debugfs interface to mod_pon_mcc_chk kernel module.\n\n"
	"The module allows to test mod_pon_mcc module API.\n\n"
	"This test interface gives you an opportunity to:\n"
	" - add new multicast table entry: echo 1 > /sys/kernel/debug/pon/mcc\n"
	" - delete multicast table entry:  echo 0 > /sys/kernel/debug/pon/mcc\n"
	" - configure bridge port index:   echo 5 > /sys/kernel/debug/pon/mcc\n"
	" - read all multicast entries:    cat /sys/kernel/debug/pon/mcc\n\n"
	" - entry to be added/deleted should be prepared in a directory\n"
	"   /sys/kernel/debug/pon/entry by echoing values of entry parameters\n"
	"   to corresponding files in this directory\n\n";
static const char usage_example1[] =
	"Example for adding ipv6 entry:\n"
	"        cd /sys/kernel/debug/pon/entry\n"
	"        for x in $(ls .); do echo $x; cat $x; done\n"
	"        echo $((0x0100000002000000)) > 60grp-addr\n"
	"        echo $((0x0300000004000000)) > 61grp-addr\n"
	"        echo 3 > ifindex\n"
	"        echo 1 > is-ipv6\n"
	"        for x in $(ls .); do echo $x; cat $x; done\n"
	"        cd ..\n"
	"        echo 1 > mcc\n";
static const char usage_example2[] =
	"Example for setting snooping assuming that we receive multicast\n"
	"data traffic on bridge port no. 6:\n"
	"        cd /sys/kernel/debug/pon/conf\n"
	"        for x in $(ls .); do echo $x; cat $x; done\n"
	"        echo 6 > br-port-id\n"
	"        echo 0 > src-awareness\n"
	"        for x in $(ls .); do echo $x; cat $x; done\n"
	"        cd ..\n"
	"        echo 5 > mcc\n"
	"        echo 6 > mcc\n"
	"        cat mcc\n";

/* this helper function prints key details */
static void show_key(const struct mcc_drv_l3tbl_key *key)
{
	if (key->proto == MCC_L3_PROTO_IPV6)
		pr_info("port id: %d, proto: IPv%d, Gda: %pI6, Gsa: %pI6, flt_mode: %d, valid: %d\n",
			key->ifindex, 6, &key->grp.addr6,
			&key->src.addr6, key->flt_mode, key->valid);
	else
		pr_info("port id: %d, proto: IPv%d, Gda: %pI4, Gsa: %pI4, flt_mode: %d, valid: %d\n",
			key->ifindex, 4, &key->grp.addr.s_addr,
			&key->src.addr.s_addr, key->flt_mode, key->valid);
}

static int mcc_dbgfs_on_write_op(void *data, u64 value)
{
	struct mcc_drv_l3tbl_key key;
	int flags = 0;

	memset(&key, 0, sizeof(key));

	if (tab_entry.is_ipv6) {
		key.grp.addr6 = tab_entry.grp.addr6;
		key.src.addr6 = tab_entry.src.addr6;
	} else {
		key.grp.addr.s_addr = tab_entry.grp.addr.s_addr;
		key.src.addr.s_addr = tab_entry.src.addr.s_addr;
	}
	key.proto = tab_entry.is_ipv6 ?
		MCC_L3_PROTO_IPV6 : MCC_L3_PROTO_IPV4;
	key.ifindex = tab_entry.ifindex;
	switch (tab_entry.flt_mode) {
	case 0:
		key.flt_mode = SRC_FLT_MODE_NA;
		break;
	case 1:
		key.flt_mode = SRC_FLT_MODE_INCLUDE;
		break;
	case 2:
		key.flt_mode = SRC_FLT_MODE_EXCLUDE;
		break;
	default:
		pr_info("invalid flt_mode for mcc entry\n");
		return 1;
	}

	switch (value) {
	case 0: /* delete by the entry parameters */
		mcc_drv_l3_tbl_del(&key, flags);
		pr_info("deleted mcc entry:\n");
		show_key(&key);
		break;
	case 1: /* add */
		mcc_drv_l3_tbl_add(&key, flags);
		pr_info("added mcc entry:\n");
		show_key(&key);
		break;
	default:
		pr_info("%s", usage_info);
		pr_info("%s", usage_example1);
		pr_info("%s", usage_example2);
		break;
	}

	return 0;
}

static int mcc_dbgfs_on_read_op(void *data, u64 *value)
{
	struct mcc_drv_l3tbl_key key;
	int flags = 0;
	int ret;
	bool activity;
	bool initial;
	bool last;
	DECLARE_BITMAP(portmap, PORTMAP_SIZE) = { 0 };

	memset(&key, 0, sizeof(key));
	pr_info("read all valid entries\n");
	/* set initial */
	initial = true;
	do {
		/* read next table entry */
		ret = mcc_drv_l3_tbl_get(&key, &activity, initial,
					 &last, &flags, portmap);
		if (ret != 0) {
			pr_err("%s: mcc_drv_l3_tbl_get returned %d\n",
			       __func__, ret);
			break;
		}
		initial = false;
		if (key.valid) {
			show_key(&key);
			memset(&key, 0, sizeof(key));
		}
	} while (!last);
	*value = 0;
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(mcc_dbgfs_ops, mcc_dbgfs_on_read_op,
			mcc_dbgfs_on_write_op, "%llu\n");

#define ADDRESSES_TO_FILES(a, b) \
	do { \
		debugfs_create_u32("4" a, 0644, tmp_subdir, \
				&tab_entry.b.addr.s_addr); \
		debugfs_create_u64("60" a, 0644, tmp_subdir, \
				((u64 *)(&tab_entry.b.addr6))); \
		debugfs_create_u64("61" a, 0644, tmp_subdir, \
				((u64 *)(&tab_entry.b.addr6)) + 1); \
	} while (0)

static int __init pon_mcc_chk_init(void)
{
	static struct dentry *tmp_subdir;

	pr_debug("Starting mcc check module\n");

	mcc_dbgfs_dir = debugfs_create_dir("pon", 0);
	debugfs_create_file("mcc", 0644, mcc_dbgfs_dir, NULL, &mcc_dbgfs_ops);
	tmp_subdir = debugfs_create_dir("entry", mcc_dbgfs_dir);

	ADDRESSES_TO_FILES("grp-addr", grp);
	ADDRESSES_TO_FILES("src-addr", src);
	debugfs_create_u8("is-ipv6", 0644, tmp_subdir, &tab_entry.is_ipv6);
	debugfs_create_u8("ifindex", 0644, tmp_subdir, &tab_entry.ifindex);
	debugfs_create_u8("mode", 0644, tmp_subdir, &tab_entry.flt_mode);

	tmp_subdir = debugfs_create_dir("conf", mcc_dbgfs_dir);

	debugfs_create_u8("br-port-data-id", 0644, tmp_subdir,
			&(br_port_data.id));
	debugfs_create_u16("br-port-data-mask-to-clear", 0644, tmp_subdir,
			&(br_port_data.mask_to_clear));
	debugfs_create_u16("br-port-data-mask-to-clear", 0644, tmp_subdir,
			&(br_port_data.mask_to_set));

	debugfs_create_u8("br-port-rep-id", 0644, tmp_subdir,
			&(br_port_rep0.id));
	debugfs_create_u16("br-port-rep0-mask-to-clear", 0644, tmp_subdir,
			&(br_port_rep0.mask_to_clear));
	debugfs_create_u16("br-port-rep0-mask-to-set", 0644, tmp_subdir,
			&(br_port_rep0.mask_to_set));

	debugfs_create_u8("br-port-rep1-id", 0644, tmp_subdir,
			&(br_port_rep1.id));
	debugfs_create_u16("br-port-rep1-mask-to-clear", 0644, tmp_subdir,
			&(br_port_rep1.mask_to_clear));
	debugfs_create_u16("br-port-rep1-mask-to-set", 0644, tmp_subdir,
			&(br_port_rep1.mask_to_set));

	debugfs_create_u8("br-port-qry-id", 0644, tmp_subdir,
			&(br_port_qry.id));
	debugfs_create_u16("br-port-qry-mask-to-clear", 0644, tmp_subdir,
			&(br_port_qry.mask_to_clear));
	debugfs_create_u16("br-port-qry-mask-to-set", 0644, tmp_subdir,
			&(br_port_qry.mask_to_set));

	return 0;
}

module_init(pon_mcc_chk_init);

static void __exit pon_mcc_chk_exit(void)
{
	pr_debug("Stopping mcc check module\n");

	debugfs_remove_recursive(mcc_dbgfs_dir);
}

module_exit(pon_mcc_chk_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("driver intended to test PON Multicast Driver API");
MODULE_AUTHOR("MaxLinear Inc");
