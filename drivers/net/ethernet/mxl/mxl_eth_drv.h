/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2016-2020 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2, as published
 * by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MXL_ETH_DRV_H_
#define _MXL_ETH_DRV_H_

#ifdef CONFIG_SW_ROUTING_MODE
    #define CONFIG_PMAC_DMA_ENABLE          1   /*g_pmac_dma */
    #define CONFIG_DMA_PMAC_ENABLE          1   /*g_dma_pmac*/
#else
    #define CONFIG_PMAC_DMA_ENABLE          0   /*g_pmac_dma */
    #define CONFIG_DMA_PMAC_ENABLE          0   /*g_dma_pmac*/
#endif

#if defined(CONFIG_PMAC_DMA_ENABLE) && CONFIG_PMAC_DMA_ENABLE
    #define NUM_ETH_INF                     1
#else
    #define NUM_ETH_INF                     9
#endif

#define STATE_NO_PENDING -1   /* initial state for pending_state*/
struct eth_thermal {
	struct thermal_cooling_device *cdev;
	unsigned int cur_state;
	unsigned int pending_state;
	unsigned int max_state;
	int pending_delay;
	int throttled;  /* number of throttled interfaces */
};

struct eth_hw {
	struct net_device *devs[NUM_ETH_INF];
	int num_devs;
	int id[NUM_ETH_INF];
	struct eth_thermal thermal;
};

/**
 * This structure is used internal purpose
 */
struct eth_priv {
	/*!< network device interface Statistics */
	struct rtnl_link_stats64 stats;
	/*!< structure of dma device information */
	struct dma_device_info *dma_device;
	struct sk_buff *skb; /*!< skb buffer structure*/

	struct net_device *net_dev;
	struct platform_device *pdev;
	struct phylink *phylink;
	struct phylink_config phylink_config;
	bool has_phy;
	phy_interface_t phy_mode;
	struct device_node *dn;
	u32 msg_enable;

	spinlock_t lock; /*!< spin lock */
	int phy_addr; /*!< interface mdio phy address*/
	int current_speed; /*!< interface current speed*/
	int full_duplex; /*!< duplex mode*/
	int current_duplex; /*!< current interface duplex mode*/
	void __iomem                *base_addr; /*!< Base address */
	unsigned int                flags;  /*!< flags */
	struct module *owner;
	dp_subif_t dp_subif;
	s32 dev_port; /*dev  instance */
	s32 f_dp;   /* status for register to datapath*/
	u32 dp_port_id;
	int xgmac_id;
	u32 gsw_devid;
	struct device_node *xpcs_node;

	int num_port;
	struct eth_hw				*hw;
	unsigned short port_map;
	int id;
	int wan;
	u32 pon_hgu_sim;
	int jumbo_enabled;
	int rxcsum_enabled;
	int lct_en;
	int extra_subif;
	/*! min netdevices for extra subif/lct */
	int start;
	/*! max netdevices for extra subif/lct */
	int end;
	int p34x_link_st;
	int phy_p34x;
	struct xpcs_ops *p34x_xpcs_ops;
	struct platform_device *p34x_xpcs_dev;

	struct ethtool_link_ksettings bkup_cmd;
	#define THERMAL_NEEDS_RECOVERY 0
	#define THERMAL_THROTTLING_PENDING 1
	unsigned long tflags; /* thermal flags */
	struct delayed_work pending_work; /* pending cooling state queue */

	u32 ethtool_flags;
	#define ETHTOOL_FLAG_BP_CPU_ENABLE	BIT(0)
#if IS_ENABLED(CONFIG_EIP160S_MACSEC)
	void *macsec_data;
	struct mac_eip160_ops e160_ops;
	struct lgm_macsec_cfg *macsec_cfg;
#endif
	u32 mac_link_up_delay_ms;
	struct delayed_work mac_link_up_work;
};

extern int g_xway_gphy_fw_loaded;

struct eth_net_soc_data {
	bool need_defer;
	bool hw_checksum;
	unsigned int queue_num;
	u32 mtu_limit;
};

#endif /* _MXL_ETH_DRV_H_ */
