// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
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

#define pr_fmt(fmt) "mxl-eth: " fmt

#include <linux/version.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/kobject.h>
#include <linux/interrupt.h>
#include <linux/kernel.h> /* printk() */
#include <linux/types.h>  /* size_t */
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/proc_fs.h>
#include <linux/etherdevice.h> /* eth_type_trans */
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/clk.h>
#include <linux/of_net.h>
#include <linux/of_mdio.h>
#include <linux/of_irq.h>
#include <linux/of_device.h>
#include <linux/thermal.h>
#include <linux/bitmap.h>
#include <linux/phy.h>
#include <linux/phylink.h>
#include <net/datapath_api.h>
#include <net/mxl_cbm_api.h>
#include <net/switch_api/lantiq_gsw_api.h>
#include <net/switch_api/gsw_dev.h>
#ifdef CONFIG_XFRM_OFFLOAD
#include <net/xfrm.h>
#endif

#include "../../datapath/xpcs/xpcs.h"
#if IS_ENABLED(CONFIG_EIP160S_MACSEC)
#include <net/eip160.h>
//#include "mxl_eth_macsec.h"
#endif

#include "mxl_eth_drv.h"

/* 9600 - ethernet overhead of 18 bytes */
#define ETH_MAX_DATA_LEN 9582
/* Min IPv4 MTU per RFC791 */
#define ETH_MIN_MTU 68

#define DRV_MODULE_NAME		"mxl_eth_drv"
#define DRV_MODULE_VERSION	 "1.0"

/* length of time before we decide the hardware is borked,
 * and dev->eth_tx_timeout() should be called to fix the problem
 */
#define ETH_TX_TIMEOUT		(10 * HZ)

#define PHY_ID_GPY2xx		0x67C9DC00
#define PHY_ID_GPY241B		0x67C9DE40
#define PHY_ID_GPY241BM		0x67C9DE80
#define PHY_ID_GPY245B		0x67C9DEC0

#define VSPEC1_SGMII_CTRL   0x08

#define SUB_IF  4

#define MAC_RX_CFG 4

#define DEFAULT_MSG_ENABLE		 \
		(NETIF_MSG_DRV		   | \
		NETIF_MSG_PROBE		   | \
		NETIF_MSG_LINK		   | \
		NETIF_MSG_IFUP		   | \
		NETIF_MSG_IFDOWN)
static int debug = -1; /* -1 == use DEFAULT_MSG_ENABLE */
static int serdes_powersaving = 0;		/* serdes power saving support (0/1) */

module_param(debug, int, 0);
MODULE_PARM_DESC(debug, "Network interface Debug level setting(0=none,...,16=all see linux/netdevice.h for bitmap)");
module_param(serdes_powersaving, int, 0);
MODULE_PARM_DESC(serdes_powersaving, "Serdes power saving support (0/1)");

/* Init of the network device */
static int eth_init(struct net_device *dev);
/* Start the network device interface queue */
static int eth_open(struct net_device *dev);
/* Stop the network device interface queue */
static int eth_stop(struct net_device *dev);
/* Uninit the network device interface queue */
static void eth_uninit(struct net_device *dev);
/* Transmit packet from Tx Queue to MAC */
static int eth_start_xmit(struct sk_buff *skb, struct net_device *dev);
/* Get the network device statistics */
static void eth_get_stats(struct net_device *dev,
			  struct rtnl_link_stats64 *storage);
/* change MTU values */
static int eth_change_mtu(struct net_device *dev, int new_mtu);
/*  Set mac address*/
static int eth_set_mac_address(struct net_device *dev, void *p);
/* Transmit timeout*/
static void eth_tx_timeout(struct net_device *dev, unsigned int txqueue);
/* eth IOCTL */
static int eth_do_ioctl(struct net_device *netdev, struct ifreq *rq, int cmd);
/* interface change event handler */
static int phy_netdevice_event(struct notifier_block *nb, unsigned long action,
			       void *ptr);
/**
 *  Datapath directpath functions
 */
static int32_t dp_fp_stop_tx(struct net_device *);
static int32_t dp_fp_restart_tx(struct net_device *);
static int32_t dp_fp_rx(struct net_device *, struct net_device *,
			struct sk_buff *, int32_t);

static int p34x_link_counter;
static struct platform_device *pdevice;
static struct kobject *kobj;

static struct eth_hw eth_hw;

static char wan_iface[IFNAMSIZ] = "eth1";

static struct module g_eth_module[NUM_ETH_INF];
static struct eth_net_soc_data g_soc_data;
static struct net_device *eth_dev[NUM_ETH_INF][SUB_IF];

static const struct net_device_ops eth_drv_ops = {
	.ndo_init		= eth_init,
	.ndo_open		= eth_open,
	.ndo_stop		= eth_stop,
	.ndo_uninit		= eth_uninit,
	.ndo_start_xmit		= eth_start_xmit,
	.ndo_set_mac_address	= eth_set_mac_address,
	.ndo_change_mtu		= eth_change_mtu,
	.ndo_get_stats64	= eth_get_stats,
	.ndo_tx_timeout		= eth_tx_timeout,
	.ndo_eth_ioctl		= eth_do_ioctl,

	/* for tc command: normally it is for all mxl network devices */
	.ndo_setup_tc = dp_ndo_setup_tc,
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	/* for switchdev
	 * In LGM, normally it is for those mxl network devices under omci
	 * control, for example, vUNI, PON side network devices.
	 * In PRX, it is for all mxl network devices.
	 */
	.ndo_bridge_setlink = dp_bp_ndo_br_setlink,
	.ndo_bridge_dellink = dp_bp_ndo_br_dellink,
	.ndo_get_port_parent_id = dp_ndo_get_port_parent_id,
	.ndo_get_phys_port_id = dp_ndo_get_phys_port_id,
	.ndo_get_phys_port_name = dp_ndo_get_phys_port_name,
#endif
};

#ifdef CONFIG_XFRM_OFFLOAD
/* for xfrm/vpn
 * Normally it is for those mxl WAN network devices except PON Network devices.
 * for example, WAN ethernet network device, vUNI, VRX, docsis related Network
 * devices need to enable it
 */
static struct xfrmdev_ops xfrm_ops = {
	.xdo_dev_state_add = dp_xdo_dev_state_add,
	.xdo_dev_state_delete = dp_xdo_dev_state_delete,
	.xdo_dev_offload_ok = dp_xdo_dev_offload_ok,
};
#endif


static int eth_do_ioctl(struct net_device *netdev,
			     struct ifreq *rq, int cmd)
{
	switch (cmd) {
		case SIOCSHWTSTAMP:
		case SIOCGHWTSTAMP:
			return dp_ndo_do_ioctl(netdev, rq, cmd);
		default:
			return phy_do_ioctl(netdev, rq, cmd);
	}
}

static struct notifier_block netdevice_notifier = {
	.notifier_call = phy_netdevice_event
};

/* Get the driver information, used by ethtool_ops  */
static void get_drvinfo(struct net_device *dev, struct ethtool_drvinfo *info)
{
	/* driver short name (Max 32 characters) */
	strcpy(info->driver, DRV_MODULE_NAME);
	/* driver version(Max 32 characters) */
	strcpy(info->version, DRV_MODULE_VERSION);
}

static inline struct core_ops *get_swcore_ops(struct net_device *dev)
{
	struct eth_priv *priv = netdev_priv(dev);

	return gsw_get_swcore_ops(priv->gsw_devid);
}

static void get_pauseparam(struct net_device *dev,
			   struct ethtool_pauseparam *epause)
{
	int ret;
	struct core_ops *ops;
	GSW_portCfg_t cfg = {0};
	struct eth_priv *priv = netdev_priv(dev);

	ops = get_swcore_ops(dev);
	if (!ops) {
		dev_err(&dev->dev, "failed in getting SW Core Ops\n");
		return;
	}

	cfg.nPortId = priv->dp_port_id;
	ret = ops->gsw_common_ops.PortCfgGet(ops, &cfg);
	if (ret != GSW_statusOk) {
		dev_err(&dev->dev, "failed in getting Port Cfg: %d\n", ret);
		return;
	}

	switch (cfg.eFlowCtrl) {
	case GSW_FLOW_AUTO:
		epause->autoneg = 1;
		epause->rx_pause = 1;
		epause->tx_pause = 1;
		break;
	case GSW_FLOW_RXTX:
		epause->rx_pause = 1;
		epause->tx_pause = 1;
		break;
	case GSW_FLOW_RX:
		epause->rx_pause = 1;
		break;
	case GSW_FLOW_TX:
		epause->tx_pause = 1;
		break;
	case GSW_FLOW_OFF:
	default:
		epause->autoneg = 0;
	}
}

static int set_pauseparam(struct net_device *dev,
			  struct ethtool_pauseparam *epause)
{
	int ret;
	struct core_ops *ops;
	GSW_portCfg_t cfg = {0};
	struct eth_priv *priv = netdev_priv(dev);

	ops = get_swcore_ops(dev);
	if (!ops) {
		dev_err(&dev->dev, "failed in getting SW Core Ops\n");
		return -EINVAL;
	}

	cfg.nPortId = priv->dp_port_id;
	ret = ops->gsw_common_ops.PortCfgGet(ops, &cfg);
	if (ret != GSW_statusOk) {
		dev_err(&dev->dev, "failed in getting Port Cfg: %d\n", ret);
		return -EIO;
	}

	if (epause->autoneg)
		cfg.eFlowCtrl = GSW_FLOW_AUTO;
	else if (epause->rx_pause)
		cfg.eFlowCtrl = epause->tx_pause ? GSW_FLOW_RXTX : GSW_FLOW_RX;
	else if (epause->tx_pause)
		cfg.eFlowCtrl = GSW_FLOW_TX;
	else
		cfg.eFlowCtrl = GSW_FLOW_OFF;

	ret = ops->gsw_common_ops.PortCfgSet(ops, &cfg);
	if (ret != GSW_statusOk) {
		dev_err(&dev->dev, "failed in setting Port Cfg: %d\n", ret);
		return -EIO;
	}
	return 0;
}

/* Reset the device */
static int nway_reset(struct net_device *dev)
{
	/*TODO*/
	return 0;
}

static int is_p34x(struct phy_device *phydev)
{
	int ret, id;

	if (!phydev)
		return 0;

	ret = phy_read(phydev, MII_PHYSID1);
	if (ret < 0)
		return 0;

	id = ret << 16;

	ret = phy_read(phydev, MII_PHYSID2);
	if (ret < 0)
		return 0;

	id |= ret & GENMASK(31, 4);

	if (id == PHY_ID_GPY2xx ||
	    id == PHY_ID_GPY241B ||
	    id == PHY_ID_GPY241BM ||
	    id == PHY_ID_GPY245B)
		return 1;
	else
		return 0;
}

static void soc_serdes_ctl(struct net_device *dev, int ctl)
{
	struct eth_priv *priv = netdev_priv(dev);

	if (ctl)
		priv->p34x_xpcs_ops->power_on(&priv->p34x_xpcs_dev->dev);
	else
		priv->p34x_xpcs_ops->power_off(&priv->p34x_xpcs_dev->dev);

	return;
}

static void p34x_serdes_ctl(struct net_device *dev, int ctl)
{
	int ret;
	ret = phy_read_mmd(dev->phydev, MDIO_MMD_VEND1, VSPEC1_SGMII_CTRL);
	if (ret < 0) {
		phydev_err(dev->phydev, "Error: MMD register access failed: %d\n", ret);
		return;
	}
	if (ctl)
		ret = ret & ~0x800;
	else
		ret = ret | 0x800;
	phy_write_mmd(dev->phydev, MDIO_MMD_VEND1, VSPEC1_SGMII_CTRL, ret);
	return;
}

static void serdes_st_reset(int reset) {
	struct eth_priv *priv_eth;
	struct net_device *dev;
	int i;

	for (i = 0; i < eth_hw.num_devs; i++) {
		dev = eth_hw.devs[i];
		if (!dev)
			continue;
		priv_eth = netdev_priv(dev);
		if (priv_eth->phy_p34x == 1) {
			soc_serdes_ctl(dev, reset);
			break;
		}
	}
	for (i = 0; i < eth_hw.num_devs; i++) {
		dev = eth_hw.devs[i];
		if (!dev)
			continue;
		priv_eth = netdev_priv(dev);
		if (priv_eth->phy_p34x == 1)
			p34x_serdes_ctl(dev, reset);
	}
}

/* status 1/0 is turn on/off Serdes Power Saving feature from sysfs */
static void serdes_link_st_update(int status)
{
	if (p34x_link_counter == 0){
		if (status)
			serdes_st_reset(0);
		else
			serdes_st_reset(1);
	}
}

static void serdes_ps_st_update(int status)
{
	if (status != serdes_powersaving) {
		serdes_powersaving = status;
		serdes_link_st_update(status);
	}
}

static ssize_t serdes_show(struct device *dev, struct device_attribute *attr, char *buf) 
{
	int off = 0;

	if (serdes_powersaving)
		off += sprintf(buf + off, "Serdes Power Saving feature Status:   on\n");
	else
		off += sprintf(buf + off, "Serdes Power Saving feature Status:   off\n");
	off += sprintf(buf + off, "\n\n");
	return off;
}

static ssize_t serdes_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
	int dev_int;

	sscanf(buf, "%d", &dev_int);
	if (dev_int == 0 || dev_int == 1) {
		serdes_ps_st_update(dev_int);
	} else {
		pr_err("Invalid parameters. "
			"Usage: \"echo <1/0> > serdes\" to turn on/off Serdes Power Saving feature.\n");
	}
	return count;
}

static struct device_attribute dev_attr_serdes = __ATTR(serdes, 0660, serdes_show, serdes_store);

static int sysfs_serdes_control_init(void) {
	int ret;

	pdevice = platform_device_register_simple("serdes_control", -1, NULL, 0);
	if (IS_ERR(pdevice))
		return PTR_ERR(pdevice);

	kobj = &pdevice->dev.kobj;
	ret = sysfs_create_file(kobj, &dev_attr_serdes.attr);
	if (ret)
		pr_info("failed to create the serdes file in /sys/devices/platform/serdes_control \n");

	return ret;
}

static void sysfs_serdes_control_exit(void) {
	sysfs_remove_file(kobj, &dev_attr_serdes.attr);
	platform_device_unregister(pdevice);
}

#if IS_ENABLED(CONFIG_MXL_ETH_THERMAL)
static void phy_serdes_off(struct net_device *ndev)
{
	struct net_device *dev;
	struct eth_priv *priv_eth;
	struct eth_priv *priv = netdev_priv(ndev);
	int i;

	if (priv->phy_p34x == 1) {
		priv->p34x_link_st = 0;
		p34x_link_counter--;
		if (serdes_powersaving && p34x_link_counter == 0) {
			for (i = 0; i < eth_hw.num_devs; i++) {
				dev = eth_hw.devs[i];
				if (!dev)
					continue;
				priv_eth = netdev_priv(dev);
				if (priv_eth->phy_p34x == 1) {
					p34x_serdes_ctl(dev, 0);
				}
			}
			soc_serdes_ctl(ndev, 0);
		}
	}
	return;
}

static void phy_serdes_on(struct net_device *ndev)
{
	struct net_device *dev;
	struct eth_priv *priv_eth;
	struct eth_priv *priv = netdev_priv(ndev);
	int i;

	if (priv->phy_p34x == 1) {
		priv->p34x_link_st = 1;
		p34x_link_counter++;
		if (serdes_powersaving && p34x_link_counter == 1) {
			soc_serdes_ctl(ndev, 1);
			for (i = 0; i < eth_hw.num_devs; i++) {
				dev = eth_hw.devs[i];
				if (!dev)
					continue;
				priv_eth = netdev_priv(dev);
				if (priv_eth->phy_p34x == 1) {
					p34x_serdes_ctl(dev, 1);
				}
			}
		}
	}
	return;
}
#endif

static int gpy2xx_eee_get(struct net_device *dev, struct ethtool_eee *data)
{
	/* if link partner does not advertise 2.5G, it may advertise 2.5G EEE
	 * by mistake.
	 */
	if (!linkmode_test_bit(ETHTOOL_LINK_MODE_2500baseT_Full_BIT,
			       dev->phydev->lp_advertising)) {
		data->lp_advertised &= ~SUPPORTED_2500baseX_Full;
		data->eee_active = !!(data->advertised & data->lp_advertised);
	}

	if (data->advertised) {
		data->eee_enabled = 1;
		data->tx_lpi_enabled = 1;
	}

	return 0;
}

static int ethtool_eee_get(struct net_device *dev,
			   struct ethtool_eee *eee_data)
{
	int ret;
	struct eth_priv *priv = netdev_priv(dev);
	struct mac_ops *ops;
	u32 val;

	ret = phylink_ethtool_get_eee(priv->phylink, eee_data);
	if (ret)
		return ret;

	if (is_p34x(dev->phydev)) {
		/* This is special handling of EEE feature for P34X (GPY2xx)
		 * in Ethernet driver. We skip all the other settings because
		 * EEE and LPI are handled fully by P34X (GPY2xx) and PHY
		 * device does NOT accept LPI initiated by MAC.
		 */
		return gpy2xx_eee_get(dev, eee_data);
	}

	if (priv->xgmac_id < MAC_2)
		return 0;

	ops = gsw_get_mac_ops(0, priv->xgmac_id);
	if (!ops)
		return 0;

	eee_data->tx_lpi_enabled = !!ops->get_lpi(ops);

	if (!ops->xgmac_reg_rd(ops, 0xd8, &val))
		eee_data->tx_lpi_timer = val;

	return 0;
}

static int  ethtool_eee_set(struct net_device *dev,
			    struct ethtool_eee *eee_data)
{
	int ret;
	struct eth_priv *priv = netdev_priv(dev);
	struct mac_ops *ops;

	ret = phylink_ethtool_set_eee(priv->phylink, eee_data);
	if (ret)
		return ret;

	/* This is special handling of EEE feature for P34X (GPY2xx)
	 * in Ethernet driver. We skip all the other settings because
	 * EEE and LPI are handled fully by P34X (GPY2xx) and PHY
	 * device does NOT accept LPI initiated by MAC.
	 */
	if (is_p34x(dev->phydev))
		return 0;

	if (priv->xgmac_id < MAC_2)
		return 0;

	ops = gsw_get_mac_ops(0, priv->xgmac_id);
	if (!ops)
		return 0;

	if (!eee_data->tx_lpi_enabled == !ops->get_lpi(ops))
		return 0;

	ops->set_lpi(ops, eee_data->tx_lpi_enabled, eee_data->tx_lpi_timer,
		     eee_data->tx_lpi_timer);

	return 0;
}

static int get_link_ksettings(struct net_device *dev,
			      struct ethtool_link_ksettings *cmd)
{
	struct eth_priv *priv = netdev_priv(dev);
	int ret;

	if (priv->phylink && cmd) {
		ret = phylink_ethtool_ksettings_get(priv->phylink, cmd);
		if (ret)
			return ret;
	}

	return 0;
}

static int set_link_ksettings(struct net_device *dev,
			      const struct ethtool_link_ksettings *cmd)
{
	struct eth_priv *priv = netdev_priv(dev);

	if (dev->phydev) {
		if (linkmode_test_bit(ETHTOOL_LINK_MODE_2500baseT_Full_BIT,
				      cmd->link_modes.advertising))
			linkmode_set_bit(ETHTOOL_LINK_MODE_2500baseT_Full_BIT,
					 dev->phydev->advertising);
		else
			linkmode_clear_bit(ETHTOOL_LINK_MODE_2500baseT_Full_BIT,
					   dev->phydev->advertising);
	}

	if (priv->phylink && cmd)
		return phylink_ethtool_ksettings_set(priv->phylink, cmd);

	return 0;
}

static u32 get_priv_flags(struct net_device *dev)
{
	struct eth_priv *np = netdev_priv(dev);

	return np->ethtool_flags;
}

static int set_priv_flags(struct net_device *dev, u32 flags)
{
	struct eth_priv *np = netdev_priv(dev);
	struct dp_bp_attr bp_conf = {0};
	int ret = 0;

	bp_conf.dev = dev;

	if (flags & ETHTOOL_FLAG_BP_CPU_ENABLE) {
		bp_conf.en = true;
		ret = dp_set_bp_attr(&bp_conf, 0);
		if (ret != 0)
			return ret;
		np->ethtool_flags |= ETHTOOL_FLAG_BP_CPU_ENABLE;
	} else {
		bp_conf.en = false;
		ret = dp_set_bp_attr(&bp_conf, 0);
		if (ret != 0)
			return ret;
		np->ethtool_flags &= ~ETHTOOL_FLAG_BP_CPU_ENABLE;
	}

	return ret;
}

static const char priv_flags_strings[][ETH_GSTRING_LEN] = {
	"bp_to_cpu_enable",
};

#define PRIV_FLAGS_STR_LEN ARRAY_SIZE(priv_flags_strings)

static void get_strings(struct net_device *netdev, u32 stringset, u8 *data)
{
	int i;

	switch (stringset) {
	case ETH_SS_PRIV_FLAGS:
		for (i = 0; i < PRIV_FLAGS_STR_LEN; i++) {
			memcpy(data, priv_flags_strings[i],
			       ETH_GSTRING_LEN);
			data += ETH_GSTRING_LEN;
		}
		break;
	case ETH_SS_STATS:
		dp_net_dev_get_ss_stat_strings(netdev, data);
		break;
	default:
		break;
	}
}

static int get_stringset_count(struct net_device *netdev, int sset)
{
	switch (sset) {
	case ETH_SS_PRIV_FLAGS:
		return PRIV_FLAGS_STR_LEN;
	case ETH_SS_STATS:
		return dp_net_dev_get_ss_stat_strings_count(netdev);
	default:
		return -EOPNOTSUPP;
	}
}

static void get_ethtool_stats(struct net_device *dev,
			      struct ethtool_stats *stats, u64 *data)
{
	dp_net_dev_get_ethtool_stats(dev, stats, data);
}

static void eth_drv_get_wol(struct net_device *dev,
				struct ethtool_wolinfo *wol)
{
	struct eth_priv *priv = netdev_priv(dev);
	if (priv->phylink) {
		phylink_ethtool_get_wol(priv->phylink, wol);
	} else {
		pr_err("phylink pointer is NULL\n");
	}
}

static int eth_drv_set_wol(struct net_device *dev,
				struct ethtool_wolinfo *wol)
{
	struct eth_priv *priv = netdev_priv(dev);
	int ret;
	if (priv->phylink) {
		ret = phylink_ethtool_set_wol(priv->phylink, wol);
	} else {
		pr_err("phylink pointer is NULL\n");
		ret = -EINVAL;
	}
	return ret;
}

/* Structure of the ether tool operation  */
static const struct ethtool_ops ethtool_ops = {
	.get_drvinfo		= get_drvinfo,
	.nway_reset		= nway_reset,
	.get_link_ksettings	= get_link_ksettings,
	.set_link_ksettings	= set_link_ksettings,
	.get_pauseparam		= get_pauseparam,
	.set_pauseparam		= set_pauseparam,
	.get_eee		= ethtool_eee_get,
	.set_eee		= ethtool_eee_set,
	.get_strings		= get_strings,
	.get_sset_count		= get_stringset_count,
	.get_priv_flags		= get_priv_flags,
	.set_priv_flags		= set_priv_flags,
	.get_ethtool_stats	= get_ethtool_stats,
	.get_link		= ethtool_op_get_link,
	.get_wol		= eth_drv_get_wol,
	.set_wol		= eth_drv_set_wol,
#if IS_ENABLED(CONFIG_PTP_1588_CLOCK)
	/* Note: PTP handling in net_device_ops.ndo_eth_ioctl was already
	 * done in driver: SIOCSHWTSTAMP/SIOCGHWTSTAMP
	 * So here we only add get_ts_info in this patch
	 * Note: PTP is only for streaming port related mxl network devices
	 */
	.get_ts_info		= dp_get_ts_info,
#endif
};

/* open the network device interface*/
static int eth_open(struct net_device *dev)
{
	int ret;
	int flags = 1;  /* flag 1 for enable */
	struct eth_priv *priv = netdev_priv(dev);

	pr_debug("%s called for device %s\n", __func__, dev->name);
#if IS_ENABLED(CONFIG_GRX500_CBM)
	/* Enable the p2p channel at CBM */
	if (!cbm_turn_on_DMA_p2p())
		pr_info("p2p channel turned ON !\n");
	else
		pr_info("p2p channel already ON !\n");
#endif

	ret = dp_rx_enable(dev, dev->name, flags);

	if (ret != DP_SUCCESS) {
		pr_warn("%s: failed to enable for device: %s ret %d\n",
			__func__, dev->name, ret);
		return -1;
	}

	if (priv->has_phy || priv->phylink)
		phylink_start(priv->phylink);

	return 0;
}

/* Stop the network device interface*/
static int eth_stop(struct net_device *dev)
{
	int ret;
	int flags = 0;  /* flag 0 for disable */
	struct eth_priv *priv = netdev_priv(dev);

	pr_debug("%s called for device %s\n", __func__, dev->name);

	ret = dp_rx_enable(dev, dev->name, flags);

	if (ret != DP_SUCCESS) {
		pr_warn("%s: failed to disable for device: %s ret %d\n",
			__func__, dev->name, ret);
		return -1;
	}

	if (priv->mac_link_up_delay_ms) {
		struct mac_ops *mac_ops;

		cancel_delayed_work_sync(&priv->mac_link_up_work);
		mac_ops = gsw_get_mac_ops(0, priv->xgmac_id);
		if (mac_ops && mac_ops->mac_receiver_enable && mac_ops->set_link_sts)
		{
			mac_ops->mac_receiver_enable(mac_ops, 0);
			mac_ops->set_link_sts(mac_ops, GSW_PORT_LINK_DOWN);
		}
	}

	if (priv->has_phy || priv->phylink)
		phylink_stop(priv->phylink);

	return 0;
}

/* Uninit the network device interface*/
static void eth_uninit(struct net_device *dev)
{
	struct eth_priv *priv;
	struct dp_subif_data data = {0};
	int ret;

	pr_debug("%s called for device %s\n", __func__, dev->name);
	priv = netdev_priv(dev);

	if (priv->has_phy || priv->phylink) {
		pr_debug("Disconnect PHY for %s\n", dev->name);
		phylink_disconnect_phy(priv->phylink);
	}

	if (priv->dp_port_id == DP_FAILURE) {
		pr_debug("dp port id (%d) is invalid. ignore the deregister.\n",
			 priv->dp_port_id);
		return;
	}

	if (priv->lct_en == 1) {
		priv->dp_subif.subif = -1;
		priv->dp_subif.port_id = priv->dp_port_id;
		data.flag_ops |= DP_F_DATA_LCT_SUBIF;
		ret = dp_register_subif_ext(0, priv->owner,
					    dev, dev->name,
					    &priv->dp_subif,
					    &data, DP_F_DEREGISTER);
	} else {
		priv->dp_subif.subif = -1;
		priv->dp_subif.port_id = priv->dp_port_id;
		ret = dp_register_subif_ext(0, priv->owner,
					    dev, dev->name,
					    &priv->dp_subif,
					    NULL, DP_F_DEREGISTER);
	}
	if (ret != DP_SUCCESS)
		pr_err("%s: failed to close for device: %s ret %d\n",
		       __func__, dev->name, ret);
}

/* Send the packet to netwrok rx queue */
static void eth_rx(struct net_device *dev, int len, struct sk_buff *skb)
{
	struct eth_priv *priv;
	int result;

	priv = netdev_priv(dev);
	skb->dev = dev;
	skb->protocol = eth_type_trans(skb, dev);

	if (priv->rxcsum_enabled && (dev->features & NETIF_F_RXCSUM))
		skb->ip_summed = CHECKSUM_UNNECESSARY;

	pr_debug("passing to stack: protocol: %x\n", skb->protocol);
	result = netif_rx(skb);
	if (result == NET_RX_SUCCESS) {
		priv->stats.rx_packets++;
		priv->stats.rx_bytes += len;
	} else {
		priv->stats.rx_dropped++;
	}
}

static int32_t dp_fp_stop_tx(struct net_device *netif)
{
	return 0;
}

static int32_t dp_fp_restart_tx(struct net_device *netif)
{
	return 0;
}

static int32_t dp_fp_rx(struct net_device *rxif, struct net_device *txif,
			struct sk_buff *skb, int32_t len)
{
	/*skb_put(skb,len);*/
#ifdef DUMP_PACKET
	if (skb->data) {
		pr_info("raw data len:%d\n", len);
		print_hex_dump_debug("skb->data: ", DUMP_PREFIX_NONE, 16, 1,
				     skb->data, len, false);
	}

#endif

#if !IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	if (skb) {
		/*Remove PMAC to DMA header */
		len -= (sizeof(struct pmac_tx_hdr));
		skb_pull(skb, (sizeof(struct pmac_tx_hdr)));
	} else {
		pr_err("%s: skb from DP is null !\n", __func__);
		goto rx_err_exit;
	}
#endif

	/* Pass it to the stack */
#ifdef DUMP_PACKET

	if (skb->data) {
		pr_info("data sent to stack.\n");
		print_hex_dump_debug("skb->data: ", DUMP_PREFIX_NONE, 16, 1,
				     skb->data, len, false);
	}

#endif

	if (rxif) {
		pr_debug("%s: rxed a packet from DP lib on interface %s\n",
			 __func__, rxif->name);
		eth_rx(rxif, len, skb);
	} else {
		pr_err("%s: error: rxed a packet from DP lib on interface 0x%pS\n",
		       __func__, rxif);
		goto rx_err_exit;
	}

	return 0;
rx_err_exit:

	if (skb)
		dev_kfree_skb_any(skb);

	return -1;
}

/* Get the network device stats information */
static void eth_get_stats(struct net_device *dev,
			  struct rtnl_link_stats64 *storage)
{
	dp_get_netif_stats(dev, NULL, storage, 0);
}

/* Trasmit timeout */
static void eth_tx_timeout(struct net_device *dev, unsigned int txqueue)
{
	unsigned int tx_q = 0;

	while (tx_q < dev->num_tx_queues) {
		if (netif_tx_queue_stopped(netdev_get_tx_queue(dev, tx_q)))
			pr_debug("dev: %s Tx Timeout txq: %d\n",
				 dev->name, tx_q);
		tx_q++;
	}

	netif_tx_wake_all_queues(dev);
}

/* Set the MAC address */
static int eth_set_mac_address(struct net_device *dev, void *p)
{
	struct sockaddr *addr = p;
	struct eth_priv *priv = netdev_priv(dev);

	pr_debug("set_mac_addr called\n");
	if (netif_running(dev))
		return -EBUSY;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EINVAL;

	memcpy(dev->dev_addr, addr->sa_data, dev->addr_len);

	if (priv->xgmac_id >= 0) {
		struct mac_ops *ops;

		ops = gsw_get_mac_ops(0, priv->xgmac_id);
		if (ops)
			ops->set_macaddr(ops, dev->dev_addr);
	}
	return 0;
}

/* Change the MTU value of the netwrok device interfaces */
static int eth_change_mtu(struct net_device *dev, int new_mtu)
{
	struct eth_priv *priv = netdev_priv(dev);

	if (dp_set_mtu_size(dev, new_mtu) != 0) {
		pr_err("fail to set mtu size as %u\n", new_mtu);
		return -EINVAL;
	}

	if (priv->xgmac_id >= 0) {
		struct mac_ops *ops;

		ops = gsw_get_mac_ops(0, priv->xgmac_id);
		/* In OMCI daemon, need to use ifconfig to set mtu to
		 * [L2 Maximum Frame Size - 18] ex: (1518 - 18 = 1500)
		 * 18 bytes is the ETH header and FCS
		 */
		if (ops)
			ops->set_mtu(ops, (new_mtu + ETH_HLEN + ETH_FCS_LEN));
	}

	dev->mtu = new_mtu;

	return 0;
}

static int eth_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct eth_priv *priv = netdev_priv(dev);
	int ret;
	int len;

	/* Call the Datapath Library's TX function */
	((struct dma_tx_desc_1 *)&skb->DW1)->field.ep = priv->dp_subif.port_id;
	((struct dma_tx_desc_0 *)&skb->DW0)->field.dest_sub_if_id =
		priv->dp_subif.subif;

	len = skb->len;

	ret = dp_xmit(dev, &priv->dp_subif, skb, skb->len,
		      DP_TX_NEWRET |
		      (dev->features & NETIF_F_HW_CSUM ? DP_TX_CAL_CHKSUM : 0));
	if (!ret) {
		priv->stats.tx_packets++;
		priv->stats.tx_bytes += len;
	} else if (ret != NETDEV_TX_BUSY) {
		priv->stats.tx_dropped++;
	}
	return ret;
}

/* init of the network device */
static int eth_init(struct net_device *dev)
{
	struct eth_priv *priv;
	struct dp_subif_data data = {0};
	int ret;

	priv = netdev_priv(dev);
	pr_debug("probing for number of ports = %d\n", priv->num_port);
	pr_debug("%s called for device %s\n", __func__, dev->name);

	dev->ethtool_ops = &ethtool_ops;
	data.flag_ops |= DP_SUBIF_RX_FLAG;

	if (!(priv->ethtool_flags & ETHTOOL_FLAG_BP_CPU_ENABLE))
		data.flag_ops |= DP_SUBIF_BP_CPU_DISABLE;

	if (priv->lct_en == 1) {
		priv->dp_subif.subif = -1;
		priv->dp_subif.port_id = priv->dp_port_id;
		data.flag_ops |= DP_F_DATA_LCT_SUBIF | DP_SUBIF_NO_NDO_HACK;
		ret = dp_register_subif_ext(0, priv->owner,
					    dev, dev->name, &priv->dp_subif,
					    &data, 0);
	} else {
		priv->dp_subif.subif = -1;
		priv->dp_subif.port_id = priv->dp_port_id;

		/* Make switchdev enable for ethernet wan if for PON HGU sim */
		if (priv->wan && priv->pon_hgu_sim) {
			data.flag_ops |= DP_SUBIF_SWDEV;
			data.swdev_en_flag = 1;
		}
		data.flag_ops |= DP_SUBIF_NO_NDO_HACK;
		ret = dp_register_subif_ext(0, priv->owner,
					    dev, dev->name, &priv->dp_subif,
					    &data, 0);
	}
	if (ret != DP_SUCCESS) {
		pr_err("%s: failed to open for device: %s ret %d\n",
		       __func__, dev->name, ret);
		return -1;
	}
	pr_debug("\n subif reg for owner = %s portid = %d subifid = %d dev= %s\n",
		 priv->owner->name, priv->dp_port_id,
		 priv->dp_subif.subif, dev->name);

	/* get the minimum MTU and call the change mtu */
	dp_get_mtu_size(dev, &g_soc_data.mtu_limit);

	if (!(dev->mtu > ETH_MIN_MTU && dev->mtu < g_soc_data.mtu_limit))
		dev->mtu = g_soc_data.mtu_limit;

	dev->max_mtu = g_soc_data.mtu_limit;
	dev->min_mtu = ETH_MIN_MTU;

	if (priv->has_phy) {
		ret = phylink_of_phy_connect(priv->phylink, priv->dn, 0);
		if (ret) {
			netif_err(priv, link, dev,
				  "phy_connect filed for dev %s\n", dev->name);
			phylink_destroy(priv->phylink);
			return -1;
		}
	}

	return 0;
}

static void
eth_drv_eth_addr_setup(struct net_device *dev, int port)
{
	struct eth_priv *priv = netdev_priv(dev);

	if (is_valid_ether_addr(dev->dev_addr))
		return;

	eth_hw_addr_random(dev);
	pr_debug("using random mac for port %d.\n", port);

	if (priv->xgmac_id >= 0) {
		struct mac_ops *ops;

		ops = gsw_get_mac_ops(0, priv->xgmac_id);
		if (ops)
			ops->set_macaddr(ops, dev->dev_addr);
	}
}

/* This function is event handler for net_device change notify */
static int phy_netdevice_event(struct notifier_block *nb, unsigned long action,
			       void *ptr)
{
	struct eth_priv *priv = NULL;
	struct net_device *dev = NULL;

	if (!ptr)
		return 0;

	dev = netdev_notifier_info_to_dev(ptr);

	switch (action) {
	case NETDEV_CHANGENAME: {
		priv = netdev_priv(dev);

		if (priv && priv->wan) {
			pr_debug("\nUpdate WAN iface from [%s] to [%s]\n",
				 wan_iface, dev->name);
			memcpy(wan_iface, dev->name, IFNAMSIZ);
		}
	}
	}

	return NOTIFY_OK;
}

#if IS_ENABLED(CONFIG_MXL_ETH_THERMAL)

static int eth_thermal_get_max_state(struct thermal_cooling_device *cdev,
				     unsigned long *state)
{
	struct eth_hw *hw = cdev->devdata;
	struct eth_thermal *thermal = &hw->thermal;

	*state = thermal->max_state;

	return 0;
}

static int eth_thermal_get_cur_state(struct thermal_cooling_device *cdev,
				     unsigned long *state)
{
	struct eth_hw *hw = (struct eth_hw *)cdev->devdata;
	struct eth_thermal *thermal = &hw->thermal;

	*state = thermal->cur_state;

	return 0;
}

static __ETHTOOL_DECLARE_LINK_MODE_MASK(advertising_min);

static int eth_thermal_recover_state(struct net_device *dev)
{
	struct eth_priv *priv = netdev_priv(dev);
	int ret = 0;

	if (!priv)
		return 0;

	pr_debug("%s: %s: needs_recovery %d", __func__, netdev_name(dev),
		 test_bit(THERMAL_NEEDS_RECOVERY, &priv->tflags));

	if (test_and_clear_bit(THERMAL_NEEDS_RECOVERY, &priv->tflags)) {
		rtnl_lock();
		ret = set_link_ksettings(dev, &priv->bkup_cmd);
		rtnl_unlock();
		if (ret)
			return ret;

		if (!netif_carrier_ok(dev)) { /* in case the link loss */
			pr_debug("%s: %s: the link needs recovery, but is not acive anymore",
				 __func__, netdev_name(dev));
			clear_bit(THERMAL_THROTTLING_PENDING, &priv->tflags);
		} else {
			set_bit(THERMAL_THROTTLING_PENDING, &priv->tflags);
		}
	}

	return ret;
}

static bool eth_thermal_is_throttling_pending(struct eth_hw *hw)
{
	int i;
	struct eth_priv *priv;

	if (!hw)
		return false;

	for (i = 0; i < hw->num_devs; i++) {
		priv = netdev_priv(hw->devs[i]);

		if (test_bit(THERMAL_THROTTLING_PENDING, &priv->tflags))
			return true;
	}

	return false;
}

static void eth_thermal_pending_timeout(struct work_struct *work)
{
	struct eth_priv *priv = container_of(work, struct eth_priv,
						 pending_work.work);
	struct eth_hw *hw = priv->hw;
	struct eth_thermal *thermal = &hw->thermal;

	pr_debug("%s: %s: Link TIMEOUT state %d throttled %d pending %d (%d)",
		 __func__, netdev_name(priv->net_dev),
		 thermal->cur_state, thermal->throttled,
		 thermal->pending_state,
		 eth_thermal_is_throttling_pending(priv->hw));

	if (test_and_clear_bit(THERMAL_THROTTLING_PENDING, &priv->tflags)) {
		/* link DOWN during renegotiation */
		pr_debug("%s: %s: Link TIMEOUT (is pending)", __func__,
			 netdev_name(priv->net_dev));

		if (!eth_thermal_is_throttling_pending(priv->hw) &&
		    thermal->pending_state != STATE_NO_PENDING) {
			thermal->cur_state = thermal->throttled;
			thermal->pending_state = STATE_NO_PENDING;
		}
	} else if (test_bit(THERMAL_NEEDS_RECOVERY, &priv->tflags)) {
		/* throttled link is DOWN */
		pr_debug("%s: %s: Link TIMEOUT (not pending, throttled)",
			 __func__, netdev_name(priv->net_dev));
		/* should never happen */
	} else {
		/* not throttled link is DOWN */
		pr_debug("%s: %s: Link TIMEOUT (not pending, not throttled)",
			 __func__, netdev_name(priv->net_dev));
		/* should never happen */
	}

	pr_debug("%s: %s: Link TIMEOUT state %d throttled %d pending %d (%d)",
		 __func__, netdev_name(priv->net_dev),
		 thermal->cur_state, thermal->throttled,
		 thermal->pending_state,
		 eth_thermal_is_throttling_pending(priv->hw));
}

static int eth_thermal_set_cur_state(struct thermal_cooling_device *cdev,
				     unsigned long state)
{
	struct ethtool_link_ksettings cmd;

	int ret = 0, i, j;

	bool skip = false;

	struct eth_hw *hw = cdev->devdata;
	struct eth_thermal *thermal = &hw->thermal;

	pr_debug("%s: cooling state change %d -> %ld (pending %d)", __func__,
		 thermal->cur_state, state, thermal->pending_state);

	if (WARN_ON(state > thermal->max_state))
		return -EINVAL;

	if (thermal->cur_state == state || thermal->pending_state == state)
		return 0;

	if (eth_thermal_is_throttling_pending(hw)) {
		pr_debug("%s: Still state %d is pending", __func__,
			 thermal->pending_state);
		return -EBUSY;
	}

	for (i = 0, j = 0; i < hw->num_devs; i++) {
		struct net_device *dev = hw->devs[i];
		struct eth_priv *priv = netdev_priv(dev);

		if (state == 0) { /* cooling disabled, recover all throttled */
			pr_debug("%s: %s: cooling disabled, recovering",
				 __func__, netdev_name(dev));

			if (eth_thermal_recover_state(dev))
				pr_err("%s: %s: failed to recover settings",
				       __func__, netdev_name(dev));
			continue;
		}

		if (priv->wan) {
			pr_debug("%s: %s: skipping WAN iface", __func__,
				 netdev_name(dev));
			continue;
		}

		if (!netif_carrier_ok(dev)) {
			pr_debug("%s: %s: skipping no active link ", __func__,
				 netdev_name(dev));
			continue;  /* phy puts hw in sleep if no link */
		}

		if (!priv->has_phy)
			skip = true;

		if (skip) {
			pr_debug("%s: %s: skipping non-p34x port", __func__,
				 netdev_name(dev));
			skip = false;
			continue;
		}

		rtnl_lock();
		ret = get_link_ksettings(dev, &cmd);
		rtnl_unlock();
		if (ret) {
			pr_err("%s: %s: Failed to get settings", __func__,
			       netdev_name(dev));
			continue;
		}

		if (j < state) {
			/* reduce speed for the 'dev' port */
			if (bitmap_equal(cmd.link_modes.advertising,
					 advertising_min,
					 __ETHTOOL_LINK_MODE_MASK_NBITS)) {
				pr_debug("%s: %s: port already throttled",
					 __func__, netdev_name(dev));
				j++;
				continue;
			}

			/* backup original settings */
			priv->bkup_cmd.base.phy_address = cmd.base.phy_address;
			priv->bkup_cmd.base.autoneg = cmd.base.autoneg;
			bitmap_copy(priv->bkup_cmd.link_modes.advertising,
				    cmd.link_modes.advertising,
				    __ETHTOOL_LINK_MODE_MASK_NBITS);
			cmd.base.cmd = ETHTOOL_SLINKSETTINGS;
			cmd.base.autoneg = AUTONEG_ENABLE;
			bitmap_copy(cmd.link_modes.advertising,
				    advertising_min,
				    __ETHTOOL_LINK_MODE_MASK_NBITS);

			rtnl_lock();
			ret = set_link_ksettings(dev, &cmd);
			rtnl_unlock();
			if (ret) {
				pr_err("%s: %s: failed to set link ksettings",
				       __func__, netdev_name(dev));
				continue;
			}

			j++;

			set_bit(THERMAL_NEEDS_RECOVERY,  &priv->tflags);
			set_bit(THERMAL_THROTTLING_PENDING, &priv->tflags);

			pr_debug("%s: %s: setup timeout (cur_state %d, thro %d rstate %ld)",
				 __func__, netdev_name(dev),
				 thermal->cur_state, thermal->throttled, state);

			queue_delayed_work(system_freezable_wq,
					   &priv->pending_work,
					   msecs_to_jiffies(thermal->pending_delay));
		} else { /* remaining ports recover to previous settings
			  * if needed
			  */
			pr_debug("%s: %s: recover previous settings", __func__,
				 netdev_name(dev));

			if (eth_thermal_recover_state(dev)) {
				pr_err("%s: %s: recover link settings fail",
				       __func__, netdev_name(dev));
				continue;
			}
		}
	}

	/* Advertising bitmap change will cause a link drop. It will come back
	 * when the link renegotiation is completed. When it is completed we
	 * update the curr_satate. Don't set cur_state imediatelly here when
	 * the change is pending.
	 */

	if (eth_thermal_is_throttling_pending(hw))
		thermal->pending_state = state;
	else   /* still throttling, but nothing pending */
		thermal->cur_state = thermal->throttled; /* just update the
							  * state now
							  */
	/* else
	 *      no throttling, no links, stay at cur_state
	 **/

	pr_debug("%s: Done (cur_state %d, pending %d, requested_state %ld thro: %d",
		 __func__, thermal->cur_state,
		 thermal->pending_state, state, thermal->throttled);

	return ret;
}

static struct thermal_cooling_device_ops eth_thermal_ops = {
	.get_max_state = eth_thermal_get_max_state,
	.get_cur_state = eth_thermal_get_cur_state,
	.set_cur_state = eth_thermal_set_cur_state,
};

static ssize_t show_advrt_min(struct device *dev, struct device_attribute *attr,
			      char *buf)
{
	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	return bitmap_print_to_pagebuf(false, buf, advertising_min,
			__ETHTOOL_LINK_MODE_MASK_NBITS);
}

static ssize_t store_advrt_min(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t count)
{
	int advrt_min;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	if (kstrtoint(buf, 10, &advrt_min))
		return -EINVAL;

	linkmode_set_bit(advrt_min, advertising_min);

	return 0;
}

static DEVICE_ATTR(advrt_min, 0600, show_advrt_min, store_advrt_min);

static int eth_thermal_init(struct device_node *np, struct eth_hw *hw)
{
	struct thermal_cooling_device *cdev;
	int ret;
	u32 max = 0;

	hw->thermal.max_state = hw->num_devs;
	hw->thermal.cur_state = 0;
	hw->thermal.pending_delay = 8 * 1000;
	hw->thermal.pending_state = STATE_NO_PENDING;
	hw->thermal.throttled = 0;

	ret = of_property_read_u32(np, "mxl,eth-cooling-max-state", &max);
	if (ret == 0)
		hw->thermal.max_state = max;

	cdev = thermal_of_cooling_device_register(np, "eth_thermal", hw,
						  &eth_thermal_ops);
	if (IS_ERR(cdev)) {
		pr_err("Failed to setup cooling device (err %ld)\n",
		       PTR_ERR(cdev));

		return PTR_ERR(cdev);
	}

	hw->thermal.cdev = cdev;

	if (device_create_file(&cdev->device, &dev_attr_advrt_min)) {
		dev_err(&cdev->device,
			"couldn't create device file for advrt_min\n");
	}

	linkmode_set_bit(ETHTOOL_LINK_MODE_10baseT_Half_BIT, advertising_min);
	linkmode_set_bit(ETHTOOL_LINK_MODE_10baseT_Full_BIT, advertising_min);
	linkmode_set_bit(ETHTOOL_LINK_MODE_100baseT_Half_BIT, advertising_min);
	linkmode_set_bit(ETHTOOL_LINK_MODE_100baseT_Full_BIT, advertising_min);
	linkmode_set_bit(ETHTOOL_LINK_MODE_Autoneg_BIT, advertising_min);

	return 0;
}

void eth_thermal_exit(struct eth_hw *hw)
{
	int i;
	struct eth_priv *priv;

	/* cancel all pending state work items on exit */
	for (i = 0; i < hw->num_devs; i++) {
		priv = netdev_priv(hw->devs[i]);

		if (test_and_clear_bit(THERMAL_THROTTLING_PENDING,
				       &priv->tflags))
			cancel_delayed_work_sync(&priv->pending_work);
	}

	device_remove_file(&hw->thermal.cdev->device, &dev_attr_advrt_min);
	thermal_cooling_device_unregister(hw->thermal.cdev);
}

#else /* CONFIG_MXL_ETH_THERMAL */

static int eth_thermal_init(struct device_node *np, struct eth_hw *hw)
{
	return 0;
}

static void eth_thermal_exit(struct eth_hw *hw)
{
}

#endif /* CONFIG_MXL_ETH_THERMAL */

#if IS_ENABLED(CONFIG_EIP160S_MACSEC)
static int macsec_prepare(struct macsec_context *ctx, macsec_irq_handler_t ig,
			  macsec_irq_handler_t eg)
{
	int ret;
	struct eth_priv *priv;
	struct mac_ops *ops;

	pr_debug("E160: Enter %s\n", __func__);

	if (!ctx || !ctx->netdev)
		return -EINVAL;

	priv = netdev_priv(ctx->netdev);
	ops = gsw_get_mac_ops(0, priv->xgmac_id);
	if (!ops)
		return -EINVAL;

	ret = ops->mac_eip160_prepare(ops, &priv->e160_ops);
	pr_debug("E160: Exit %s with ret code %d\n", __func__, ret);
	return ret;
}

static void macsec_unprepare(struct macsec_context *ctx)
{
	int ret;
	struct eth_priv *priv;
	struct mac_ops *ops;

	pr_debug("E160: Enter %s\n", __func__);

	if (!ctx || !ctx->netdev)
		return;

	priv = netdev_priv(ctx->netdev);
	ops = gsw_get_mac_ops(0, priv->xgmac_id);
	if (!ops)
		return;

	ret = ops->mac_eip160_unprepare(ops, &priv->e160_ops);
	if (ret)
		return;

	pr_debug("E160: Exit %s\n", __func__);
}

static int macsec_dummy_irq(struct macsec_context *ctx)
{
	pr_debug("E160: Enter %s\n", __func__);
	return 0;
}

static int macsec_ig_reg_rd(struct macsec_context *ctx, u32 off, u32 *pdata)
{
	struct eth_priv *priv;

	if (!ctx || !ctx->netdev || !pdata)
		return -EINVAL;

	priv = netdev_priv(ctx->netdev);
	return priv->e160_ops.reg_rd(priv->e160_ops.ig_pdev, off, pdata);
}

static int macsec_ig_reg_wr(struct macsec_context *ctx, u32 off, u32 data)
{
	struct eth_priv *priv;

	if (!ctx || !ctx->netdev)
		return -EINVAL;

	priv = netdev_priv(ctx->netdev);
	return priv->e160_ops.reg_wr(priv->e160_ops.ig_pdev, off, data);
}

static int macsec_eg_reg_rd(struct macsec_context *ctx, u32 off, u32 *pdata)
{
	struct eth_priv *priv;

	if (!ctx || !ctx->netdev || !pdata)
		return -EINVAL;

	priv = netdev_priv(ctx->netdev);
	return priv->e160_ops.reg_rd(priv->e160_ops.eg_pdev, off, pdata);
}

static int macsec_eg_reg_wr(struct macsec_context *ctx, u32 off, u32 data)
{
	struct eth_priv *priv;

	if (!ctx || !ctx->netdev)
		return -EINVAL;

	priv = netdev_priv(ctx->netdev);
	return priv->e160_ops.reg_wr(priv->e160_ops.eg_pdev, off, data);
}

int register_macsec(struct net_device *netdev)
{
	struct eip160_pdev e160;
	struct eth_priv *priv;
	struct mac_ops *ops;
	int offload_mac;

	pr_debug("E160: Enter %s: %s\n", __func__, netdev->name);

	priv = netdev_priv(netdev);
	ops = gsw_get_mac_ops(0, priv->xgmac_id);
	if (!ops)
		return -EINVAL;

	offload_mac = ops->mac_offload_mac_check(ops, &priv->e160_ops);
	if (offload_mac)
		return -EINVAL;

	memset(&e160, 0, sizeof(e160));

	//e160.ctx.offload = MACSEC_OFFLOAD_MAC;
	e160.ctx.netdev = netdev;

	e160.prepare	= macsec_prepare;
	e160.unprepare	= macsec_unprepare;
	e160.ig_irq_ena	= macsec_dummy_irq;
	e160.ig_irq_dis	= macsec_dummy_irq;
	e160.ig_reg_rd	= macsec_ig_reg_rd;
	e160.ig_reg_wr	= macsec_ig_reg_wr;
	e160.eg_irq_ena	= macsec_dummy_irq;
	e160.eg_irq_dis	= macsec_dummy_irq;
	e160.eg_reg_rd	= macsec_eg_reg_rd;
	e160.eg_reg_wr	= macsec_eg_reg_wr;

	pr_debug("E160: Exit %s\n", __func__);
	return eip160_register(&e160);
}

void unregister_macsec(struct net_device *netdev)
{
	struct eip160_pdev e160;

	pr_debug("Enter %s: %s\n", __func__, netdev->name);

	memset(&e160, 0, sizeof(e160));

	//e160.ctx.offload = MACSEC_OFFLOAD_OFF;
	e160.ctx.netdev = netdev;

	eip160_unregister(&e160);
}
#endif

static int eth_dev_reg(struct eth_hw *hw, u32 xgmac_id_param,
		       u32 lct_en, u32 dp_port, int start, int end,
		       const char *label)
{
	int i, err, num = 1;
	struct eth_priv *priv;
	char name[IFNAMSIZ];

	if (!label)
		snprintf(name, IFNAMSIZ, "eth0_%d", hw->id[hw->num_devs]);
	else
		snprintf(name, IFNAMSIZ, "%s", label);
	for (i = start; i < end; i++) {
		eth_dev[hw->num_devs][i] =
			alloc_etherdev_mq(sizeof(struct eth_priv),
					  g_soc_data.queue_num);
		if (!eth_dev[hw->num_devs][i]) {
			pr_debug("allocated failed for interface %d\n", i);
			return -ENOMEM;
		}
		priv = netdev_priv(eth_dev[hw->num_devs][i]);
		priv->dp_port_id = dp_port;
		priv->xgmac_id = xgmac_id_param;
		priv->hw = hw;
		priv->id = hw->id[hw->num_devs];
		priv->owner = &g_eth_module[hw->num_devs];
		priv->ethtool_flags |= ETHTOOL_FLAG_BP_CPU_ENABLE;
		sprintf(priv->owner->name, "module%02d", priv->id);
		if (start == i && lct_en == 1) {
			priv->lct_en = 1;
			snprintf(eth_dev[hw->num_devs][i]->name, IFNAMSIZ,
				 "%s_%d_lct", name, num);
		} else {
			priv->lct_en = 0;
			snprintf(eth_dev[hw->num_devs][i]->name, IFNAMSIZ,
				 "%s_%d", name, num);
		}
		eth_dev[hw->num_devs][i]->netdev_ops = &eth_drv_ops;
#ifdef CONFIG_XFRM_OFFLOAD
		eth_dev[hw->num_devs][i]->xfrmdev_ops = &xfrm_ops;
		dp_dev_update_xfrm(eth_dev[hw->num_devs][i]); /* update dev features for xfrm */
#endif
		/* update dev features for tc */
		dp_dev_update_tc(eth_dev[hw->num_devs][i]); /* update dev features for tc */
		dp_dev_update_toe(eth_dev[hw->num_devs][i]); /* update dev features for toc */

		eth_drv_eth_addr_setup(eth_dev[hw->num_devs][i], priv->id);
		err = register_netdev(eth_dev[hw->num_devs][i]);
		if (err) {
			pr_err("%s: failed to register netdevice: %p %d\n",
			       __func__, eth_dev[hw->num_devs][i], err);
				return -1;
		}
#if IS_ENABLED(CONFIG_EIP160S_MACSEC)
		register_macsec(eth_dev[hw->num_devs][i]);
#endif
		num++;
	}
	return 0;
}

static int eth_dev_dereg_subif(int num_dev, int start, int end)
{
	int i;
	int res = DP_SUCCESS;
	struct eth_priv *priv;
	struct dp_subif_data data = {0};

	for (i = start; i < end; i++) {
		struct net_device *dev = eth_dev[num_dev][i];

		priv = netdev_priv(dev);
		priv->dp_subif.subif = priv->dp_subif.subif;
		priv->dp_subif.port_id = priv->dp_subif.port_id;
		if (priv->lct_en == 1) {
			pr_debug("owner = %s portid = %d subifid = %d dev= %s\n",
				 priv->owner->name, priv->dp_port_id,
				 priv->dp_subif.subif, dev->name);
			res = dp_register_subif_ext(0, priv->owner,
						    dev, dev->name,
						    &priv->dp_subif,
						    &data, DP_F_DEREGISTER);
		}
		if (priv->lct_en == 0) {
			pr_debug("owner = %s portid = %d subifid = %d dev= %s\n",
				 priv->owner->name, priv->dp_port_id,
				 priv->dp_subif.subif, dev->name);
			res = dp_register_subif_ext(0, priv->owner,
						    dev, dev->name,
						    &priv->dp_subif, NULL,
						    DP_F_DEREGISTER);
		}
		if (res != DP_SUCCESS) {
			pr_err("%s: failed to deregister subif for device: %s ret %d\n",
			       __func__, dev->name, res);
			return -1;
		}
	}
	return 0;
}

static int eth_dev_dereg(int num_dev, int start, int end)
{
	int i;

	for (i = start; i < end; i++) {
		struct eth_priv *priv;
		struct net_device *dev = eth_dev[num_dev][i];

		netif_stop_queue(dev);
		priv = netdev_priv(dev);
		priv->dp_port_id = DP_FAILURE;
#if IS_ENABLED(CONFIG_EIP160S_MACSEC)
		unregister_macsec(dev);
#endif
		unregister_netdev(dev);
		free_netdev(dev);
	}
	return 0;
}

static phy_interface_t mac_xpcs_mode_to_phy_int(enum xpcs_mode_t xpcs_mode)
{
	phy_interface_t phy_int;
	switch(xpcs_mode) {
		case XPCS_MODE_SGMII:
			phy_int = PHY_INTERFACE_MODE_SGMII;
			break;
		case XPCS_MODE_1000BASEX:
			phy_int = PHY_INTERFACE_MODE_1000BASEX;
			break;
		case XPCS_MODE_2500BASEX:
			phy_int = PHY_INTERFACE_MODE_2500BASEX;
			break;
		case XPCS_MODE_5GBASER:
			phy_int = PHY_INTERFACE_MODE_5GBASER;
			break;
		case XPCS_MODE_10GBASER:
			phy_int = PHY_INTERFACE_MODE_10GBASER;
			break;
		case XPCS_MODE_SUSXGMII:
			phy_int = PHY_INTERFACE_MODE_USXGMII;
			break;
		case XPCS_MODE_10GKR:
			phy_int = PHY_INTERFACE_MODE_10GKR;
			break;
		case XPCS_MODE_QUSXGMII:
			phy_int = PHY_INTERFACE_MODE_MAX;
			break;
		case XPCS_MODE_3000BASEX:
			phy_int = PHY_INTERFACE_MODE_SGMII;
			break;
		default:
			phy_int = PHY_INTERFACE_MODE_MAX;
			break;
	}

	return phy_int;
}

static enum xpcs_mode_t mac_phy_int_to_xpcs_mode(phy_interface_t phy_int, int speed)
{
	enum xpcs_mode_t xpcs_mode;
	switch(phy_int) {
		case PHY_INTERFACE_MODE_SGMII:
			/* 3000BASE-X is not standard. It is a special mode
			 * to support 3G SGMII. Network interface should be
			 * fixed-link with speed 3000 and phy-mode sgmii.
			 */
			if (speed == 3000)
				xpcs_mode = XPCS_MODE_3000BASEX;
			else
				xpcs_mode = XPCS_MODE_SGMII;
			break;
		case PHY_INTERFACE_MODE_1000BASEX:
			xpcs_mode = XPCS_MODE_1000BASEX;
			break;
		case PHY_INTERFACE_MODE_2500BASEX:
			xpcs_mode = XPCS_MODE_2500BASEX;
			break;
		case PHY_INTERFACE_MODE_5GBASER:
			xpcs_mode = XPCS_MODE_5GBASER;
			break;
		case PHY_INTERFACE_MODE_10GBASER:
			xpcs_mode = XPCS_MODE_10GBASER;
			break;
		case PHY_INTERFACE_MODE_USXGMII:
			xpcs_mode = XPCS_MODE_SUSXGMII;
			break;
		case PHY_INTERFACE_MODE_10GKR:
			xpcs_mode = XPCS_MODE_10GKR;
			break;
		default:
			xpcs_mode = XPCS_MODE_MAX;
			break;
	}
	return xpcs_mode;
}

static void mac_validate(struct phylink_config *config,
			 unsigned long *supported,
			 struct phylink_link_state *state)
{
	struct net_device *ndev = to_net_dev(config->dev);
	struct eth_priv *priv = netdev_priv(ndev);
	__ETHTOOL_DECLARE_LINK_MODE_MASK(mask) = { 0, };

	if (state->interface != PHY_INTERFACE_MODE_NA &&
	    state->interface != PHY_INTERFACE_MODE_SGMII &&
	    state->interface != PHY_INTERFACE_MODE_10GBASER &&
	    state->interface != PHY_INTERFACE_MODE_10GKR &&
	    state->interface != PHY_INTERFACE_MODE_USXGMII &&
	    state->interface != PHY_INTERFACE_MODE_2500BASEX &&
	    state->interface != PHY_INTERFACE_MODE_1000BASEX) {
		netdev_err(ndev, "cannot use PHY mode %s, supported: %s\n",
			   phy_modes(state->interface),
			   phy_modes(priv->phy_mode));
		bitmap_zero(supported, __ETHTOOL_LINK_MODE_MASK_NBITS);
		return;
	}

	phylink_set(mask, 10baseT_Half);
	phylink_set(mask, 10baseT_Full);
	phylink_set(mask, 100baseT_Half);
	phylink_set(mask, 100baseT_Full);
	phylink_set(mask, 1000baseT_Full);
	phylink_set(mask, 2500baseT_Full);
	phylink_set(mask, 10000baseT_Full);

	if (!bitmap_intersects(supported, mask,
			       __ETHTOOL_LINK_MODE_MASK_NBITS))
		netdev_warn(ndev, "No supported link modes in 0x%lx\n",
			    *supported);

	if (!bitmap_intersects(state->advertising, mask,
			       __ETHTOOL_LINK_MODE_MASK_NBITS))
		netdev_warn(ndev, "No supported link modes in adv 0x%lx\n",
			    *supported);
}

static void mac_pcs_get_state(struct phylink_config *config,
			      struct phylink_link_state *state)
{
	struct net_device *ndev = to_net_dev(config->dev);
	struct eth_priv *priv = netdev_priv(ndev);
	struct platform_device *pdev;
	struct xpcs_ops *xpcs_ops;
	enum xpcs_mode_t xpcs_mode;
	struct mac_ops *ops;

	ops = gsw_get_mac_ops(0, priv->xgmac_id);
	if (!ops)
		return;

	state->speed = ops->get_stat_speed(ops);	//GSW_portSpeed_t
	state->duplex = ops->get_stat_duplex(ops);	//GSW_portDuplex_t
	state->pause = ops->get_stat_flow_ctl(ops);	//GSW_portFlow_t
	state->link = ops->get_stat_link_sts(ops);	//GSW_portLink_t
	state->an_complete = state->link;

	pdev = of_find_device_by_node(priv->xpcs_node);
	if (!pdev) {
		pr_err("couldn't find xpcs device for dev %s\n", ndev->name);
		return;
	}

	xpcs_ops = xpcs_pdev_ops(pdev);
	if (IS_ERR_OR_NULL(xpcs_ops)) {
		put_device(&pdev->dev);	/* reference taken by of_find_device_by_node */
		pr_err("xpcs_ops get fail for dev %s\n", ndev->name);
		return;
	}
	xpcs_ops->xpcs_get_mode(&pdev->dev, &xpcs_mode);
	put_device(&pdev->dev);	/* reference taken by of_find_device_by_node */
	state->interface = mac_xpcs_mode_to_phy_int(xpcs_mode);
	if (state->interface == PHY_INTERFACE_MODE_MAX)
		dev_err(&ndev->dev,
			"Incorrect phy_interface %d for xpcs_mode %u\n",
			state->interface, xpcs_mode);

	/* WA: Queue phylink_resolve to work_queue until link becomes UP */
	if (state->link == 0)
		phylink_mac_change(priv->phylink, 0);

	pr_debug("name: %s s->i: %d speed: %d duplex: %d link: %d\n",
		 ndev->name,	state->interface, state->speed, state->duplex,
		 state->link);
}

static void mac_an_restart(struct phylink_config *config)
{
	/* Not Supported */
}

static void mac_config(struct phylink_config *config,
		       unsigned int mode,
		       const struct phylink_link_state *state)
{
	int ret = 0;
	struct net_device *ndev = to_net_dev(config->dev);
	struct eth_priv *priv = netdev_priv(ndev);
	struct platform_device *pdev;
	enum xpcs_mode_t xpcs_mode;
	struct xpcs_ops *ops;
	struct mac_ops *mac_ops;

	/* SGMII is in-band mode(side band signal)? If so the MAC receives
	 * the resolved link state, speed, duplex and flow control
	 * from the PHY, hence no need to configure the MAC.
	 */
	if (priv->has_phy && phylink_autoneg_inband(mode)) {
		pr_err("xpcs is in inband mode\n");
		return;
	}

	if (!priv->xpcs_node) {
		pr_info("no xpcs node for dev %s\n", ndev->name);
		return;
	}

	pdev = of_find_device_by_node(priv->xpcs_node);
	if (!pdev) {
		pr_info("couldn't find xpcs device for dev %s\n", ndev->name);
		return;
	}

	mac_ops = gsw_get_mac_ops(0, priv->xgmac_id);
	if (mac_ops && mac_ops->mac_config_mode)
		mac_ops->mac_config_mode(mac_ops, state->interface);

	ops = xpcs_pdev_ops(pdev);
	if (IS_ERR_OR_NULL(ops)) {
		put_device(&pdev->dev);	/* reference taken by of_find_device_by_node */
		pr_err("xpcs_ops get fail for dev %s\n", ndev->name);
		return;
	}

	xpcs_mode = mac_phy_int_to_xpcs_mode(state->interface, state->speed);
	if (xpcs_mode != XPCS_MODE_MAX)
		ret = ops->reinit_xpcs(&pdev->dev, xpcs_mode);
	else
		dev_err(&ndev->dev,
			"Incorrect xpcs_mode %u for phy interface %d\n",
			xpcs_mode, state->interface);

	put_device(&pdev->dev);	/* reference taken by of_find_device_by_node */
	pr_debug("%s: name: %s i: %d speed: %d duplex: %d link: %d\n",
		 __func__, ndev->name, state->interface, state->speed, state->duplex,
		 state->link);

	if (ret < 0)
		pr_err("xpcs_reinit failed for dev %s\n", ndev->name);
}

static void mac_link_down(struct phylink_config *config,
			  unsigned int mode,
			  phy_interface_t interface)
{
#if IS_ENABLED(CONFIG_MXL_ETH_THERMAL)
	struct net_device *ndev = to_net_dev(config->dev);
	struct eth_thermal *thermal;
	struct eth_priv *priv = netdev_priv(ndev);

	thermal = &priv->hw->thermal;

	pr_debug("%s: %s: Link DOWN state %d throttled %d pending %d (%d)",
		 __func__, netdev_name(ndev),
		 thermal->cur_state, thermal->throttled,
		 thermal->pending_state,
		 eth_thermal_is_throttling_pending(priv->hw));

	if (test_bit(THERMAL_THROTTLING_PENDING, &priv->tflags)) {
		/* pending link is DOWN - due to renegotiation */
		pr_debug("%s: %s: Link DOWN (is pending)", __func__,
			 netdev_name(ndev));
	} else if (test_bit(THERMAL_NEEDS_RECOVERY, &priv->tflags)) {
		/* throttled link is DOWN */
		pr_debug("%s: %s: Link DOWN (not pending, throttled)",
			 __func__, netdev_name(ndev));

		thermal->throttled--;
		thermal->cur_state = thermal->throttled;
	} else {
		/* not throttled link is DOWN */
		pr_debug("%s: %s: Link DOWN (not pending, not throttled)",
			 __func__, netdev_name(ndev));
	}

	pr_debug("%s: %s: Link DOWN state %d throttled %d pending %d (%d)",
		 __func__, netdev_name(ndev),
		 thermal->cur_state, thermal->throttled,
		 thermal->pending_state,
		 eth_thermal_is_throttling_pending(priv->hw));
	phy_serdes_off(ndev);
#endif
}

static void _mac_link_up_rx_enable(struct work_struct *work)
{
	struct eth_priv *priv = container_of(work, struct eth_priv, mac_link_up_work.work);
	struct mac_ops *mac_ops;

	mac_ops = gsw_get_mac_ops(0, priv->xgmac_id);
	if (mac_ops && mac_ops->set_link_sts && mac_ops->mac_receiver_enable) {
		mac_ops->set_link_sts(mac_ops, GSW_PORT_LINK_AUTO);
		mac_ops->mac_receiver_enable(mac_ops, 1);
	}
}

static void mac_link_up(struct phylink_config *config,
			struct phy_device *phy, unsigned int mode,
			phy_interface_t interface, int speed, int duplex,
			bool tx_pause, bool rx_pause)
{
#if IS_ENABLED(CONFIG_MXL_ETH_THERMAL)
	struct net_device *ndev = to_net_dev(config->dev);
	struct eth_priv *priv = netdev_priv(ndev);
	struct eth_thermal *thermal = &priv->hw->thermal;

	if (!priv || !priv->hw)
		return;

	pr_debug("%s: %s: Link UP state %d throttled %d pending %d (%d)",
		 __func__, netdev_name(ndev),
		 thermal->cur_state, thermal->throttled,
		 thermal->pending_state,
		 eth_thermal_is_throttling_pending(priv->hw));

	if (test_and_clear_bit(THERMAL_THROTTLING_PENDING, &priv->tflags)) {
		cancel_delayed_work_sync(&priv->pending_work);

		if (test_bit(THERMAL_NEEDS_RECOVERY, &priv->tflags)) {
			pr_debug("%s: %s: Link UP (is pending)", __func__,
				 netdev_name(ndev));
			thermal->throttled++; /* UP of throttled link */
		} else {
			pr_debug("%s: %s: Link UP (is pending, recovering)",
				 __func__, netdev_name(ndev));
			thermal->throttled--;  /* UP after link recovery */
		}

		/* Update cur_state only when nothing else is pending */
		if (!eth_thermal_is_throttling_pending(priv->hw) &&
		    thermal->pending_state != STATE_NO_PENDING) {
			thermal->cur_state = thermal->throttled;
			thermal->pending_state = STATE_NO_PENDING;
		}
	} else if (test_bit(THERMAL_NEEDS_RECOVERY, &priv->tflags)) {
		/* throttled link is UP */
		pr_debug("%s: %s: Link UP (not pending, throttled)",
			 __func__, netdev_name(ndev));

		thermal->throttled++;

		if (!eth_thermal_is_throttling_pending(priv->hw))
			thermal->cur_state = thermal->throttled;
	} else {
		/* a new link, not throttled link is UP */
		pr_debug("%s: %s: Link UP (new, not pending, not throttled)",
			 __func__, netdev_name(ndev));
	}

	pr_debug("%s: %s: Link UP state %d throttled %d pending %d (%d)",
		 __func__, netdev_name(ndev),
		 thermal->cur_state, thermal->throttled,
		 thermal->pending_state,
		 eth_thermal_is_throttling_pending(priv->hw));
	phy_serdes_on(ndev);
#endif

	if (priv->mac_link_up_delay_ms) 
		/* create a delayed work queue to call _mac_link_up_rx_enable */
		schedule_delayed_work(&priv->mac_link_up_work,
				      msecs_to_jiffies(priv->mac_link_up_delay_ms));
}

static const struct phylink_mac_ops phylink_ops = {
	.validate = mac_validate,
	.mac_pcs_get_state = mac_pcs_get_state,
	.mac_an_restart = mac_an_restart,
	.mac_config = mac_config,
	.mac_link_down = mac_link_down,
	.mac_link_up = mac_link_up,
};

static int phylink_setup(struct eth_priv *priv)

{
	struct phylink *phylink;
	int ret = 0;

	netif_dbg(priv, probe, priv->net_dev, "entering %s\n", __func__);

	ret = of_get_phy_mode(priv->dn, &priv->phy_mode);
	if (ret) {
		netif_err(priv, probe, priv->net_dev,
			  "missing phy-mode property in DT\n");
		return ret;
	}

	priv->phylink_config.dev = &priv->net_dev->dev;
	priv->phylink_config.type = PHYLINK_NETDEV;

	if (priv->has_phy)
		phylink = phylink_create(&priv->phylink_config, priv->pdev->dev.fwnode,
					 priv->phy_mode, &phylink_ops);
	else
		phylink = phylink_create(&priv->phylink_config,
					 of_fwnode_handle(priv->dn), priv->phy_mode,
					 &phylink_ops);

	if (IS_ERR(phylink)) {
		netif_err(priv, probe, priv->net_dev,
			  "phylink create failed for this port\n");
		return PTR_ERR(phylink);
	}

	priv->phylink = phylink;
	return 0;
}

static bool is_pmac_rxcsum_enabled(struct net_device *dev)
{
	int ret;
	struct eth_priv *priv = netdev_priv(dev);
	struct core_ops *ops;
	GSW_QoS_queuePort_t q_port = {0};
	GSW_PMAC_Glbl_Cfg_t glbl_cfg = {0};

	ops = gsw_get_swcore_ops(priv->gsw_devid);
	if (!ops) {
		dev_err(&dev->dev, "failed in getting SW Core Ops\n");
		return false;
	}

	q_port.nPortId = priv->dp_port_id;
	q_port.nTrafficClassId = 0;
	ret = ops->gsw_qos_ops.QoS_QueuePortGet(ops, &q_port);
	if (ret) {
		dev_err(&dev->dev, "failed in getting QoS QueuePort: %d\n", ret);
		return false;
	}

	glbl_cfg.nPmacId = 2; /* to PPv4 */
	ret = ops->gsw_pmac_ops.Pmac_Gbl_CfgGet(ops, &glbl_cfg);
	if (ret) {
		dev_err(&dev->dev, "failed in getting PMAC Gbl Cfg: %d\n", ret);
		return false;
	}

	return (!q_port.bEnableIngressPceBypass && !glbl_cfg.bIPTransChkVerDis);
}

static int net_create_interface(struct eth_hw *hw, struct device_node *iface,
				struct platform_device *pdev)
{
	struct eth_priv *priv;
	struct dp_dev_data dev_data = {0};
	struct device_node *mac_np;
	u32 dp_dev_port_param, dp_port_id_param, xgmac_id_param;
	u32 lct_en_param = 0, extra_subif_param = 0;
	struct net_device *dev;
	dp_cb_t cb = {0};
	const char *label = NULL;
	const char *altname;
	char name[16];
	u32 dpid;
	int ret;
	struct dp_port_data data = {0};
	struct device_node *p34x_xpcs_node = NULL;

	/* alloc the network device */
	dev = alloc_etherdev_mq(sizeof(struct eth_priv),
				g_soc_data.queue_num);

	if (!dev) {
		pr_debug("allocated failed for interface %d\n",
			 hw->num_devs);
		return -ENOMEM;
	}

	priv = netdev_priv(dev);
	priv->ethtool_flags |= ETHTOOL_FLAG_BP_CPU_ENABLE;
	hw->devs[hw->num_devs] = dev;
	priv->msg_enable = netif_msg_init(debug, DEFAULT_MSG_ENABLE);

	/* is this the wan interface ? */
	if (of_get_property(iface, "mxl,wan", NULL))
		priv->wan = 1;
	else
		priv->wan = 0;
	/* use label name if available */
	ret = of_property_read_string(iface, "label", &label);
	if (!ret) {
		snprintf(name, sizeof(name), label);
	} else {
		if (priv->wan) {
			snprintf(name, sizeof(name), wan_iface);
		} else {
			hw->id[hw->num_devs] = of_alias_get_id(iface, "eth0_");
			if (hw->id[hw->num_devs] < 0 && ret) {
				pr_err("failed to get alias id, errno %d\n",
				       hw->id[hw->num_devs]);
				return -EINVAL;
			}

			snprintf(name, sizeof(name), "eth0_%d", hw->id[hw->num_devs]);
		}
	}

	ret = of_property_read_u32(iface, "mxl,dp-dev-port",
				   &dp_dev_port_param);
	if (ret < 0) {
		pr_info("ERROR : Property mxl,dp-dev-port not read from DT for if %s\n",
			name);
		return ret;
	}

	ret = of_property_read_u32(iface, "mxl,dp-port-id",
				   &dp_port_id_param);
	if (ret < 0) {
		pr_info("ERROR : Property mxl,dp-port-id not read from DT for if %s\n",
			name);
		return ret;
	}

	if(of_property_read_bool(iface, "mxl,dp-port-10g"))
		data.link_speed_cap = SPEED_10000;

	priv->xgmac_id = -1;

	mac_np = of_parse_phandle(iface, "mac", 0);
	if (mac_np) {
		ret = of_property_read_u32(mac_np, "mac_idx", &xgmac_id_param);
		if (ret < 0) {
			pr_info("ERROR : Property mac_idx not read from DT for if %s\n",
				name);
			return ret;
		}

		priv->xgmac_id = xgmac_id_param;

		priv->xpcs_node = of_parse_phandle(mac_np, "xpcs", 0);
		if (!priv->xpcs_node)
			pr_info("Cannot get xpcs node\n");
	}

	p34x_xpcs_node = of_parse_phandle(iface, "p34x-xpcs-node", 0);
	if (p34x_xpcs_node) {
		struct mac_ops *ops;
		// Reset and init xgmac
		ops = gsw_get_mac_ops(0, priv->xgmac_id);
		if (ops && ops->mac_reset && ops->init) {
			ops->mac_reset(ops, 1);
			ops->init(ops);
		} else {
			pr_info("xgmac %d not reset/init\n", priv->xgmac_id);
		}
		priv->phy_p34x = 1;
	} else {
		priv->phy_p34x = 0;
	}
	priv->p34x_xpcs_dev = of_find_device_by_node(p34x_xpcs_node);
	if (!priv->p34x_xpcs_dev) {
		pr_info("couldn't find lan xpcs device for dev %s\n", dev->name);
	} else {
		priv->p34x_xpcs_ops = xpcs_pdev_ops(priv->p34x_xpcs_dev);
		if (IS_ERR(priv->p34x_xpcs_ops)) {
			netdev_err(dev, "xpcs_ops get fail for dev %s\n", priv->p34x_xpcs_dev->name);
		}
	}

	ret = of_property_read_u32(iface, "mxl,lct-en",
				   &lct_en_param);
	if (ret < 0) {
		pr_debug("Property mxl,lct-en not exist for if %s\n",
			 name);
		priv->lct_en = -1;
	}
			pr_debug("Property mxl,lct-en for if %s %d\n",
				 name, lct_en_param);
	ret = of_property_read_u32(iface, "mxl,extra-subif",
				   &extra_subif_param);
	if (ret < 0) {
		pr_info("Property mxl,extra-subif not exist for if %s %d\n",
			name, extra_subif_param);
		priv->extra_subif = -1;
	} else {
		pr_info("Property mxl,extra-subif for if %s %d\n",
			name, extra_subif_param);
		priv->extra_subif = extra_subif_param;
		priv->start = 0;
		priv->end = priv->extra_subif;
		if (priv->end > SUB_IF) {
			pr_err("%s : Too many sub devices requested: %i\n",
			       name, priv->end);
			return -EINVAL;
		}
		/* Traffic between diff subif */
		if (!of_property_read_bool(iface, "mxl,allow_subif_data_loop"))
			data.flag_ops = DP_F_DATA_NO_LOOP;
	}
	ret = of_property_read_u32(iface, "mxl,gsw-devid",
				   &priv->gsw_devid);
	if (ret < 0) {
		priv->gsw_devid = 0;
		ret = 0;
	}

	/* PON HGU Simulating using Ethernet WAN */
	if (priv->wan && !of_property_read_u32(iface, "mxl,pon_hgu_sim",
					       &priv->pon_hgu_sim)) {
		if (priv->pon_hgu_sim) {
			pr_info("PON HGU Simulation Enabled on Ethernet WAN\n");
			data.flag_ops |= DP_F_DATA_PCE_PATH_EN |
					 DP_F_DATA_PON_HGU_SEC_LOOP;
		}
	}

	/* For RTL8261 phy, the signal to MAC is unable when the link is up.
	 * So the MAC should only be enabled with some delay when mac_link_up
	 * callback is called.
	 */
	if (of_property_read_u32(iface, "mxl,mac_link_up_delay_ms",
				 &priv->mac_link_up_delay_ms))
		priv->mac_link_up_delay_ms = 0;

	if (priv->mac_link_up_delay_ms)
		INIT_DELAYED_WORK(&priv->mac_link_up_work, _mac_link_up_rx_enable);

	strcpy(dev->name, name);
	dev->netdev_ops = &eth_drv_ops;

#ifdef CONFIG_XFRM_OFFLOAD
	dev->xfrmdev_ops = &xfrm_ops;
	dp_dev_update_xfrm(dev); /* update dev features for xfrm */
#endif
	/* update dev features for tc */
	dp_dev_update_tc(dev); /* update dev features for tc */
	dp_dev_update_toe(dev); /* update dev features for toc */

	dev->watchdog_timeo = ETH_TX_TIMEOUT;
	dev->needed_headroom = sizeof(struct pmac_tx_hdr);
	SET_NETDEV_DEV(dev, &pdev->dev);

	/* setup our private data */
	priv->hw = hw;
	priv->id = hw->id[hw->num_devs];
	spin_lock_init(&priv->lock);

	priv->owner = &g_eth_module[hw->num_devs];
	sprintf(priv->owner->name, "module%02d", priv->id);

	dpid = dp_alloc_port_ext(0, priv->owner, dev, dp_dev_port_param,
				 dp_port_id_param, NULL, &data,
				 priv->wan ? DP_F_FAST_ETH_WAN : DP_F_FAST_ETH_LAN);
	if (dpid == DP_FAILURE) {
		pr_err("dp_alloc_port_ext failed for %s with port_id %d\n",
		       dev->name, priv->id + 1);
		return -ENODEV;
	}

	priv->dp_port_id = dpid;
	if (priv->extra_subif >= 0)
		dev_data.max_ctp = extra_subif_param + 1;
	cb.stop_fn = (dp_stop_tx_fn_t)dp_fp_stop_tx;
	cb.restart_fn  = (dp_restart_tx_fn_t)dp_fp_restart_tx;
	cb.rx_fn = (dp_rx_fn_t)dp_fp_rx;

	if (dp_register_dev_ext(0, priv->owner, dpid, &cb, &dev_data, 0)
	    != DP_SUCCESS) {
		pr_err("dp_register_dev failed for %s\n and port_id %d",
		       dev->name, dpid);
		dp_alloc_port(priv->owner, dev, dp_dev_port_param,
			      dp_port_id_param, NULL, DP_F_DEREGISTER);
		return -ENODEV;
	}

	eth_drv_eth_addr_setup(dev, priv->id);
	priv->dn = iface;
	priv->net_dev = dev;
	priv->pdev = pdev;
	priv->has_phy = !!of_find_property(iface, "phy", NULL);
	ret = phylink_setup(priv);
	if (ret) {
		pr_err("phylink setup failed for dev %s\n", dev->name);
		goto free_netdev;
	}

	priv->rxcsum_enabled = is_pmac_rxcsum_enabled(dev);

	/* register the actual device */
	if (!register_netdev(dev)) {
		pr_debug("%s: priv->extra_subif = %d interface %s !\n",
			 __func__, priv->extra_subif, name);
#if IS_ENABLED(CONFIG_EIP160S_MACSEC)
		register_macsec(dev);
#endif
	}
	if (extra_subif_param >= 1) {
		eth_dev_reg(&eth_hw, xgmac_id_param, lct_en_param,
			    dp_port_id_param, priv->start, priv->end, label);
	}

#if IS_ENABLED(CONFIG_MXL_ETH_THERMAL)
	/* setup pending cooling state work for ndev */
	INIT_DELAYED_WORK(&priv->pending_work,
			  eth_thermal_pending_timeout);
#endif
	hw->num_devs++;

	altname = of_get_property(iface, "altname", NULL);
	if (altname) {
		ret = netdev_name_node_alt_create(dev, altname);
		if (ret) {
			dev_err(&pdev->dev,
				"failed to create altname %s\n",
				altname);
		}
	}

	return 0;

free_netdev:
	free_netdev(dev);
	return ret;
}

/* Initialization Ethernet module */
static int eth_drv_init(struct platform_device *pdev)
{
	int ret = 0;
	struct device_node *node = pdev->dev.of_node;
	struct device_node *child_node;

	memset(g_eth_module, 0, sizeof(g_eth_module));
	g_soc_data.mtu_limit = ETH_MAX_DATA_LEN;

	/* load the interfaces */
	/* add a dummy interface */
	eth_hw.num_devs = 0;
	for_each_available_child_of_node(node, child_node) {
		if (of_device_is_compatible(child_node, "mxl,pdi")) {
			pr_debug("adding the interface: %d\n",
				 eth_hw.num_devs);

			if (!of_device_is_available(child_node)) {
				pr_debug("device not available.\n");
				continue;
			}
			if (eth_hw.num_devs < NUM_ETH_INF) {
				net_create_interface(&eth_hw, child_node, pdev);
			} else {
				dev_err(&pdev->dev,
					"only %d interfaces allowed\n",
					NUM_ETH_INF);
			}
		}
	}

	if (!eth_hw.num_devs) {
		dev_err(&pdev->dev, "failed to load interfaces\n");
		return -ENOENT;
	}

	if (of_find_property(node, "#cooling-cells", NULL)) {
		ret = eth_thermal_init(node, &eth_hw);
		if (ret) {
			pr_err("%s: net cooling device not registered (err %d)\n",
			       __func__, ret);
		}
	}

	pr_info("Mxl ethernet driver init.\n");
	return 0;
}

static void eth_drv_exit(struct platform_device *pdev)
{
	int i, ret;
	struct eth_priv *priv;
	struct net_device *dev;
	struct dp_dev_data *dev_data_ptr = NULL;

	for (i = 0; i < eth_hw.num_devs; i++) {
		int net_start = 0, net_end = 0;

		dev = eth_hw.devs[i];

		if (!dev)
			continue;

		netif_stop_queue(dev);
		priv = netdev_priv(dev);
		pr_debug("[%s] i %d dev_id %d pid %d for dev %s\n", __func__,
			 i, priv->dev_port, priv->dp_port_id, dev->name);
		net_start = priv->start;
		net_end = priv->end;
		priv->dp_subif.subif = 0;
		priv->dp_subif.port_id = priv->dp_port_id;

		ret = dp_register_subif(priv->owner, dev,
					dev->name, &priv->dp_subif,
					DP_F_DEREGISTER);

		if (ret != DP_SUCCESS) {
			pr_err("%s: failed to call deregister subif: %s\n",
			       __func__, dev->name);
		}

		if (priv->extra_subif >= 1)
			eth_dev_dereg_subif(i, net_start, net_end);

		ret = dp_register_dev_ext(0, priv->owner,
					  priv->dp_port_id, NULL,
					  dev_data_ptr,
					  DP_F_DEREGISTER);
		if (ret != DP_SUCCESS) {
			pr_err("%s: failed to call deregister device: %s\n",
			       __func__, dev->name);
		}

		ret = dp_alloc_port(priv->owner, dev, priv->dev_port,
				    priv->dp_port_id, NULL, DP_F_DEREGISTER);
		if (ret != DP_SUCCESS) {
			pr_err("%s: failed to call dealloc for device : %s\n",
			       __func__, dev->name);
		}

		priv->dp_port_id = DP_FAILURE;
#if IS_ENABLED(CONFIG_EIP160S_MACSEC)
		unregister_macsec(dev);
#endif
		unregister_netdev(dev);

		if (priv->phylink)
			phylink_destroy(priv->phylink);

		free_netdev(dev);
		if (priv->extra_subif >= 1)
			eth_dev_dereg(i, net_start, net_end);
	}

	eth_thermal_exit(&eth_hw);

	memset(&eth_hw, 0, sizeof(eth_hw));

	pr_info("Mxl ethernet driver remove.\n");
}

static int eth_drv_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct eth_net_soc_data *soc_data = NULL;
	int ret;

	soc_data = (struct eth_net_soc_data *)of_device_get_match_data(dev);
	if (!soc_data) {
		pr_err("No data found for mxl eth drv!\n");
		return -EINVAL;
	}

	if (soc_data->need_defer/* && !is_xway_gphy_fw_loaded() */)
		return -EPROBE_DEFER;

	memcpy(&g_soc_data, soc_data, sizeof(*soc_data));
	/* Just do the init */
	eth_drv_init(pdev);
	register_netdevice_notifier(&netdevice_notifier);

	p34x_link_counter = 0;
	ret = sysfs_serdes_control_init();

	return ret;
}

static void eth_drv_shutdown(struct platform_device *pdev)
{
	int i;
	struct eth_priv *priv;

	for (i = 0; i < eth_hw.num_devs; i++) {
		struct net_device *dev = eth_hw.devs[i];

		netif_device_detach(dev);

		priv = netdev_priv(dev);
		if (priv->xgmac_id >= 0) {
			struct mac_ops *ops;

			ops = gsw_get_mac_ops(0, priv->xgmac_id);
			if (ops)
				ops->xgmac_reg_wr(ops, MAC_RX_CFG, 0);
		}
	}
}

static int eth_drv_remove(struct platform_device *pdev)
{
	/* Just do the exit */
	sysfs_serdes_control_exit();
	unregister_netdevice_notifier(&netdevice_notifier);
	eth_drv_exit(pdev);
	return 0;
}

static const struct eth_net_soc_data lgm_net_data = {
	.need_defer = false,
	.queue_num = 8,
};

static const struct of_device_id eth_drv_match[] = {
	{ .compatible = "mxl,lgm-eth", .data = &lgm_net_data},
	{ .compatible = "mxl,mxl-eth", .data = &lgm_net_data},
	{},
};
MODULE_DEVICE_TABLE(of, eth_drv_match);

static struct platform_driver eth_driver = {
	.probe = eth_drv_probe,
	.remove = eth_drv_remove,
	.shutdown = eth_drv_shutdown,
	.driver = {
		.name = "mxl-eth",
		.of_match_table = eth_drv_match,
		.owner = THIS_MODULE,
	},
};

module_platform_driver(eth_driver);
MODULE_DESCRIPTION("MaxLinear ethernet driver");
MODULE_LICENSE("GPL v2");
MODULE_VERSION(DRV_MODULE_VERSION);
