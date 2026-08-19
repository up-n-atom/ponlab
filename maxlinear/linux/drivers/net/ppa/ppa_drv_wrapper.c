/******************************************************************************
 **
 ** FILE NAME	: ppe_drv_wrapper.c
 ** PROJECT	: PPA
 ** MODULES	: PPA Wrapper for PPE Driver API
 **
 ** DATE	: 14 Mar 2011
 ** AUTHOR	: Shao Guohua
 ** DESCRIPTION	: PPA Wrapper for PPE Driver API
 ** COPYRIGHT	: Copyright (c) 2020 - 2023 MaxLinear, Inc
 ** COPYRIGHT	: Copyright (c) 2017 Intel Corporation
 ** Copyright (c) 2011 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 ** HISTORY
 ** $Date		$Author				$Comment
 ** 14 MAR 2011	Shao Guohua			Initiate Version
 ** 10 DEC 2012	Manamohan Shetty		 Added the support for RTP,MIB mode 
 **									 Features
 ** 05 JUL 2017	Kamal Eradath			Merged ppe_drv_wrapper.c and ppa_datapath_wrapper.c
 *******************************************************************************/
/*
 * ####################################
 *				Head File
 * ####################################
 */
/*
 *	Common Head File
 */
#include <linux/version.h>
#include <generated/autoconf.h>

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/netdevice.h>
#include <linux/atmdev.h>
#include <net/sock.h>
#include <linux/debugfs.h>

/*
 *	Chip Specific Head File
 */
#include <net/ppa/ppa_api.h>
#include <net/ppa/ppa_hal_api.h>
#include <net/ppa/ppa_drv_wrapper.h>
#include <net/qos_mgr/qos_hal_api.h>

#include <net/datapath_api.h>

static struct dentry *ppa_debugfs_dir;
static struct dentry *ppa_hal_debugfs_dir;

/*Hook API for PPE Driver's Datapath layer: these hook will be set in PPE datapath driver*/
int32_t (*ppa_sw_add_session_hook)(PPA_BUF *skb, void *pitem) = NULL;
EXPORT_SYMBOL(ppa_sw_add_session_hook);

int32_t (*ppa_sw_update_session_hook)(PPA_BUF *skb, void *pitem, void *txifinfo) = NULL;
EXPORT_SYMBOL(ppa_sw_update_session_hook);

void (*ppa_sw_del_session_hook)(void *pitem) = NULL;
EXPORT_SYMBOL(ppa_sw_del_session_hook);

int32_t (*ppa_sw_fastpath_enable_hook)(uint32_t f_enable, uint32_t flags) = NULL;
EXPORT_SYMBOL(ppa_sw_fastpath_enable_hook);

int32_t (*ppa_get_sw_fastpath_status_hook)(uint32_t *f_enable, uint32_t flags) = NULL;
EXPORT_SYMBOL(ppa_get_sw_fastpath_status_hook);

int32_t (*ppa_sw_session_enable_hook)(void *pitem, uint32_t f_enable, uint32_t flags) = NULL;
EXPORT_SYMBOL(ppa_sw_session_enable_hook);

int32_t (*ppa_get_sw_session_status_hook)(void *pitem, uint32_t *f_enable, uint32_t flags) = NULL;
EXPORT_SYMBOL(ppa_get_sw_session_status_hook);

int32_t (*ppa_sw_fastpath_send_hook)(PPA_SKBUF *skb) = NULL;
EXPORT_SYMBOL(ppa_sw_fastpath_send_hook);

#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH)
int32_t (*ppa_sw_litepath_tcp_send_hook)(PPA_SKBUF *skb) = NULL;
EXPORT_SYMBOL(ppa_sw_litepath_tcp_send_hook);

int32_t (*ppa_tcp_litepath_enable_hook)(uint32_t f_enable, uint32_t flags) = NULL;
EXPORT_SYMBOL(ppa_tcp_litepath_enable_hook);

int32_t (*ppa_get_tcp_litepath_status_hook)(uint32_t *f_enable, uint32_t flags) = NULL;
EXPORT_SYMBOL(ppa_get_tcp_litepath_status_hook);
#endif

#if IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
int32_t (*ppa_sw_litepath_udp_send_hook)(PPA_SKBUF *skb) = NULL;
EXPORT_SYMBOL(ppa_sw_litepath_udp_send_hook);

int32_t (*ppa_udp_litepath_enable_hook)(uint32_t f_enable, uint32_t flags) = NULL;
EXPORT_SYMBOL(ppa_udp_litepath_enable_hook);

int32_t (*ppa_get_udp_litepath_status_hook)(uint32_t *f_enable, uint32_t flags) = NULL;
EXPORT_SYMBOL(ppa_get_udp_litepath_status_hook);
#endif

/*Hook API for PPE Driver's HAL layer: these hook will be set in PPE HAL driver */
int32_t (*ppa_drv_hal_generic_hook)(PPA_GENERIC_HOOK_CMD cmd, void *buffer, uint32_t flag) = NULL;
EXPORT_SYMBOL(ppa_drv_hal_generic_hook);

struct qos_hal_qos_stats;
int32_t (*qos_hal_get_csum_ol_mib_hook_fn)(
		struct qos_hal_qos_stats *csum_mib,
		uint32_t flag) = NULL;
EXPORT_SYMBOL(qos_hal_get_csum_ol_mib_hook_fn);

int32_t (*qos_hal_clear_csum_ol_mib_hook_fn)(
		uint32_t flag) = NULL;
EXPORT_SYMBOL(qos_hal_clear_csum_ol_mib_hook_fn);

int32_t(*qos_hal_get_qos_mib_hook_fn)(
		struct net_device *netdev,
		dp_subif_t *subif_id,
		int32_t queueid,
		struct qos_hal_qos_stats *qos_mib,
		uint32_t flag) = NULL;
EXPORT_SYMBOL(qos_hal_get_qos_mib_hook_fn);

int32_t (*qos_hal_clear_qos_mib_hook_fn)(
		struct net_device *netdev,
		dp_subif_t *subif_id,
		int32_t queueid,
		uint32_t flag) = NULL;
EXPORT_SYMBOL(qos_hal_clear_qos_mib_hook_fn);

/*	others:: these hook will be set in PPE datapath driver	*/
int (*ppa_drv_get_dslwan_qid_with_vcc_hook)(struct atm_vcc *vcc) = NULL;
int (*ppa_drv_get_netif_qid_with_pkt_hook)(PPA_SKBUF *skb, void *arg, int is_atm_vcc) = NULL;
int (*ppa_drv_get_atm_qid_with_pkt_hook)(PPA_SKBUF *skb, void *arg, int is_atm_vcc) = NULL;
int (*ppa_drv_ppe_clk_change_hook)(unsigned int arg, unsigned int flags) = NULL;
int (*ppa_drv_ppe_pwm_change_hook)(unsigned int arg, unsigned int flags) = NULL;
int32_t (*ppa_drv_datapath_generic_hook)(PPA_GENERIC_HOOK_CMD cmd, void *buffer, uint32_t flag) = NULL;
/*below hook will be exposed from datapath driver and called by its hal driver.*/
int32_t (*ppa_drv_datapath_mac_entry_setting)(uint8_t	*mac, uint32_t fid, uint32_t portid,
		uint32_t agetime, uint32_t st_entry , uint32_t action) = NULL;

/* Hook API for datapath A1 to get MPoA type */
int32_t (*ppa_drv_hal_get_mpoa_type_hook)(uint32_t dslwan_qid, uint32_t *mpoa_type) = NULL;

int32_t (*ppa_api_get_mapped_queue)(PPA_NETIF *netdev,
		 struct netdev_attr *attr) = NULL;
EXPORT_SYMBOL(ppa_api_get_mapped_queue);

int ppa_drv_get_dslwan_qid_with_vcc(struct atm_vcc *vcc)
{
	if (!ppa_drv_get_dslwan_qid_with_vcc_hook) return 0;
	else return ppa_drv_get_dslwan_qid_with_vcc_hook(vcc);
}

int ppa_drv_get_netif_qid_with_pkt(PPA_SKBUF *skb, void *arg, int is_atm_vcc)
{
	if (!ppa_drv_get_netif_qid_with_pkt_hook) return 0;
	else return ppa_drv_get_netif_qid_with_pkt_hook(skb, arg, is_atm_vcc);

}

int ppa_drv_ppe_clk_change(unsigned int arg, unsigned int flags)
{
	if (!ppa_drv_ppe_clk_change_hook)
		return PPA_FAILURE;
	else return ppa_drv_ppe_clk_change_hook(arg, flags);
}

int ppa_drv_ppe_pwm_change(unsigned int arg, unsigned int flags)
{
	if (!ppa_drv_ppe_pwm_change_hook)
		return PPA_FAILURE;
	else return ppa_drv_ppe_pwm_change_hook(arg, flags);
}

uint32_t ppa_drv_dp_sb_addr_to_fpi_addr_convert(PPA_FPI_ADDR*a, uint32_t flag)
{
	if (!ppa_drv_datapath_generic_hook)
		return PPA_FAILURE;
	return ppa_drv_datapath_generic_hook(PPA_GENERIC_DATAPATH_ADDR_TO_FPI_ADDR, (void *)a, flag);

}

int32_t ppa_hook_set_lan_seperate_flag(uint32_t flag)
{
	if (!ppa_drv_datapath_generic_hook)
		return PPA_FAILURE;
	return ppa_drv_datapath_generic_hook(PPA_GENERIC_DATAPATH_SET_LAN_SEPARATE_FLAG, NULL, flag);
}
int32_t ppa_hook_get_lan_seperate_flag(uint32_t flag)
{
	if (!ppa_drv_datapath_generic_hook)
		return PPA_FAILURE;
	return ppa_drv_datapath_generic_hook(PPA_GENERIC_DATAPATH_GET_LAN_SEPARATE_FLAG, NULL, flag);
}
uint32_t ppa_hook_set_wan_seperate_flag(uint32_t flag)
{
	if (!ppa_drv_datapath_generic_hook)
		return PPA_FAILURE;
	return ppa_drv_datapath_generic_hook(PPA_GENERIC_DATAPATH_SET_WAN_SEPARATE_FLAG, NULL, flag);
}
uint32_t ppa_hook_get_wan_seperate_flag(uint32_t flag)
{
	if (!ppa_drv_datapath_generic_hook)
		return PPA_FAILURE;
	return ppa_drv_datapath_generic_hook(PPA_GENERIC_DATAPATH_GET_WAN_SEPARATE_FLAG, NULL, flag);
}

int32_t ppa_debugfs_create(struct dentry *root,
			   const char *dname, struct dentry **parent,
			   struct ppa_debugfs_files *files, uint32_t count)
{
	int i;
	struct dentry *dent;

	if (!root) {
		pr_err("%s: root is NULL\n", __func__);
		return PPA_EINVAL;
	}

	dent = root;
	if (dname) {
		dent = debugfs_create_dir(dname, root);
		if (IS_ERR_OR_NULL(dent)) {
			pr_err("%s: Failed to create '%s' dir\n",
				__func__, dname);
			return PPA_ENOMEM;
		}

		if (parent)
			*parent = dent;
	}

	for (i = 0; i < count; i++) {
		files[i].file =
			debugfs_create_file(files[i].name, files[i].perm,
					    dent, NULL, files[i].fops);
		if (IS_ERR_OR_NULL(files[i].file))
			pr_err("%s: Failed to create '%s' file\n",
				__func__, files[i].name);
	}

	return PPA_SUCCESS;
}
EXPORT_SYMBOL(ppa_debugfs_create);

void ppa_debugfs_remove(struct dentry *parent,
			struct ppa_debugfs_files *files, uint32_t count)
{
	if (parent)
		debugfs_remove_recursive(parent);
}
EXPORT_SYMBOL(ppa_debugfs_remove);

struct dentry *ppa_debugfs_dir_get()
{
	return ppa_debugfs_dir;
}
EXPORT_SYMBOL(ppa_debugfs_dir_get);

struct dentry *ppa_hal_debugfs_dir_get()
{
	return ppa_hal_debugfs_dir;
}
EXPORT_SYMBOL(ppa_hal_debugfs_dir_get);

static int __init ppa_drv_wrapper_init(void)
{
	ppa_debugfs_dir = debugfs_create_dir("ppa", NULL);
	if (IS_ERR_OR_NULL(ppa_debugfs_dir)) {
		pr_err("%s: Failed to create 'ppa' dir\n", __func__);
		goto exit;
	}

	ppa_hal_debugfs_dir =
		debugfs_create_dir("hal", ppa_debugfs_dir);
	if (IS_ERR_OR_NULL(ppa_hal_debugfs_dir))
		pr_err("%s: Failed to create 'hal' dir\n", __func__);

exit:
	return 0;
}

static void __exit ppa_drv_wrapper_exit(void)
{
	if (!IS_ERR_OR_NULL(ppa_hal_debugfs_dir))
		debugfs_remove_recursive(ppa_hal_debugfs_dir);

	if (!IS_ERR_OR_NULL(ppa_debugfs_dir))
		debugfs_remove_recursive(ppa_debugfs_dir);
}

module_init(ppa_drv_wrapper_init);
module_exit(ppa_drv_wrapper_exit);

EXPORT_SYMBOL(ppa_drv_get_dslwan_qid_with_vcc_hook);
EXPORT_SYMBOL(ppa_drv_get_netif_qid_with_pkt_hook);
EXPORT_SYMBOL(ppa_drv_get_atm_qid_with_pkt_hook);
EXPORT_SYMBOL(ppa_drv_hal_get_mpoa_type_hook);
EXPORT_SYMBOL(ppa_drv_ppe_clk_change_hook);
EXPORT_SYMBOL(ppa_drv_ppe_pwm_change_hook);
EXPORT_SYMBOL(ppa_drv_get_dslwan_qid_with_vcc);
EXPORT_SYMBOL(ppa_drv_get_netif_qid_with_pkt);
EXPORT_SYMBOL(ppa_drv_ppe_clk_change);
EXPORT_SYMBOL(ppa_drv_ppe_pwm_change);
EXPORT_SYMBOL(ppa_drv_datapath_generic_hook);
EXPORT_SYMBOL(ppa_drv_datapath_mac_entry_setting);
EXPORT_SYMBOL(ppa_drv_dp_sb_addr_to_fpi_addr_convert);
EXPORT_SYMBOL(ppa_hook_get_lan_seperate_flag);
EXPORT_SYMBOL(ppa_hook_set_lan_seperate_flag);
EXPORT_SYMBOL(ppa_hook_get_wan_seperate_flag);
EXPORT_SYMBOL(ppa_hook_set_wan_seperate_flag);
