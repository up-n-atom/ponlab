// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_INSTANCE_H
#define DATAPATH_INSTANCE_H

extern int dp_cap_num;
extern struct dp_hw_cap hw_cap_list[DP_MAX_HW_CAP];

#define DP_DEV_HASH_BIT_LENGTH 10
#define DP_DEV_HASH_SIZE (1 << DP_DEV_HASH_BIT_LENGTH)

#define DP_MOD_HASH_BIT_LENGTH 10
#define DP_MOD_HASH_SIZE (1 << DP_MOD_HASH_BIT_LENGTH)

#define NO_NEED_PMAC(flags)  ((flags & \
		(DP_F_FAST_WLAN | DP_F_FAST_DSL)) && \
		!((flags) & (DP_TX_CAL_CHKSUM | DP_TX_DSL_FCS)))

extern struct hlist_head dp_dev_list[DP_DEV_HASH_SIZE];
u32 dp_dev_hash(void *dev);
struct dp_dev *dp_dev_lookup(struct net_device *dev);

struct dp_mod {
	struct hlist_node hlist;
	struct module *mod;
	u16 ep;
	int inst;
};

struct dp_dev_subif {
	struct list_head list;
	int subif; /* for matching during delete */
	struct dp_subif_info *sif;
	struct logic_dev *logic_dev;
};

#define MAX_NDO_PER_OPS  12  /* Maximum number of ndo_xxx to hack per ops */
struct ndo_offset {
	bool f_hacked; /* whether nod_xxx hacked or not */
	int offset;  /* the ndo_xxx offset in the ops */
};
struct dp_dev {
	struct hlist_node hlist;
	struct net_device *dev;

	/* below variables used just for workaround when dev de-register from dpm,
	 * but it is still under bridge yet
	 */
	int inst;
	int ep;
	int bp; /* if this dev is ctp dev only, bp can be -1 */
	int fid;
	int bp_learning_limit; /* Bridge port learning limit value */
	int learning_limit_en; /* Bridge port learning limit enable/disable */
	bool swdev_en; /* swdev enable or not */

	/* count means the number of times for this device to register to dpm,
	 * ie, call dp_register_subif. For normal device, it should be 1 only.
	 * But for pmapper, it can be up to 8.
	 * count will be inc/dec during register_subif/de_register_subif
	 * for pmapper case, it will call mutliple times.
	 * if count == 0, it means not registered to dpm yet
	 */
	u32 count;
	int all_ops_cnt; /* overall number of ndo_ hacked in all ops */

	/* ops/ndo hacking related variables */
	int dev_ops_cnt; /* the number of ndo_ hacked in net_device_ops */
	const struct net_device_ops *old_dev_ops;
	struct net_device_ops new_dev_ops;
	struct ndo_offset net_dev_offset[MAX_NDO_PER_OPS];
	char *net_dev_memo[MAX_NDO_PER_OPS];
	int ethtool_ops_cnt; /* the number of ndo_ hacked in ethtool_ops */
	const struct ethtool_ops *old_ethtool_ops;
	struct ethtool_ops new_ethtool_ops;
	struct ndo_offset ethtool_offset[MAX_NDO_PER_OPS];
	char *ethtool_memo[MAX_NDO_PER_OPS];
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV) && \
    LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
	int swdev_ops_cnt; /* the number of ndo_ hacked in ethtool_ops */
	struct switchdev_ops *old_swdev_ops;
	struct switchdev_ops new_swdev_ops;
	struct ndo_offset swdev_offset[MAX_NDO_PER_OPS];
	char *swdev_memo[MAX_NDO_PER_OPS];
#endif

#if IS_ENABLED(CONFIG_INTEL_VPN) || IS_ENABLED(CONFIG_MXL_VPN)
	int xfrm_ops_cnt; /* the number of ndo_ hacked in xfrmdev_ops */
	struct xfrmdev_ops *old_xfrm_ops;
	struct xfrmdev_ops new_xfrm_ops;
	struct ndo_offset xfrm_offset[MAX_NDO_PER_OPS];
	char *xfrm_memo[MAX_NDO_PER_OPS];
#endif
	/* below varibles are added since 5.x */

	/* pointer to bridge device if applicable */
	struct net_device *br_dev, *last_br_dev; /* no use in new design */
	struct br_info *br_info, *last_br_info;
	struct bridge_member_port *br_member_port;

	/* true: ops set, false, ops not set
	 * Note: ops will be set only via dp_register_subif flow
	 */
	bool f_ops_set;  /* maybe need per ops flag */
	bool f_stp_disabling;
	/* points to subif. For pmapper case, it can be up to 8 subifs.
	 * For the number of entryies, please refer to count value;
	 * Note: for pmapper device, we cannot support symetric vlan
	 */
	struct list_head subif_list;  /* point to struct dp_dev_subif */
	bool f_add_dev_wa; /* workaround flag if dpm_reguster_subif is called
			    * before dpm receive NETDEV_REGISTER event
			    */
	bool f_del_dev_wa; /* workaround flag if dpm_de-reguster_subif is called
			    * after dpm receive NETDEV_UNREGISTER event
			    * Note: later we can remove it since we are using
			    * ndo_uninit now
			    */
	bool f_ctp_dev; /* true means it is only dtp dev */
	bool f_orig_tc_flag; /* original NETIF_F_HW_TC flag */
};

extern struct kmem_cache *cache_dev_list;
/*dp_inst_p: dp instance basic property */
int dp_get_inst_via_dev(struct net_device *dev,
			char *subif_name, u32 flag);
int dp_get_inst_via_module(struct module *owner, u16 ep, u32 flag);
struct dp_hw_cap *match_hw_cap(struct dp_inst_info *info, u32 flag);
int dp_inc_dev(struct dp_subif_info *sif, struct net_device *dev, int inst,
		    int ep, int bp, int ctp, u32 flag, bool f_ctp_dev);
int dp_dec_dev(struct net_device *dev, int inst,
		    int ep, u16 ctp, u32 flag);

enum DP_DEL_DEV_STAT {
	DP_DEL_DEV_NETDEV_UNREGISTER = 1, /* trigger by NETDEV_UNREGISTER */
	DP_DEL_DEV_NDO_UNINIT, /* trigger by nod_uninit */
	DP_DEL_DEV_DEC_DEV /* trigger by dp_dec_dev */
};
int dp_del_dev(struct net_device *dev, u32 flag);
int dp_update_dev_br(struct net_device *dev, struct net_device *br_dev,
			int br_flag); /* note: br_flag: DP_BR_NOCHANGE, ... */
int dp_inst_insert_mod(struct module *owner, u16 ep, u32 inst, u32 flag);
int dp_inst_del_mod(struct module *owner, u16 ep, u32 flag);
int dp_set_net_dev_ops_priv(struct net_device *dev,
	const void *ops_cb, const int ops_offset, u32 flag, const char *memo);

int proc_dev_dump(struct seq_file *s, int pos);
int proc_dev_start(void *param);
int proc_dev_ops_start(void *param);
int proc_dev_ops_dump(struct seq_file *s, int pos);
ssize_t proc_dev_ops_write(struct file *file, const char *buf,
				    size_t count, loff_t *ppos);
int proc_br_dump(struct seq_file *s, int pos);
int proc_br_start(void *param);
int proc_inst_mod_dump(struct seq_file *s, int pos);
int proc_inst_hal_dump(struct seq_file *s, int pos);
int proc_inst_dump(struct seq_file *s, int pos);

int proc_inst_mod_start(void *param);
int set_dev_ops(struct dp_dev *dp_dev, struct net_device *dev, int flag);
int reset_dev_ops(struct dp_dev *dp_dev, struct net_device *dev);
int dp_add_dev(struct net_device *dev);
int dp_del_in_free_dev(struct net_device *dev);
int free_remain_dev(void);
struct logic_dev *logic_dev_lookup(struct list_head *head,
				   struct net_device *dev);

#endif
