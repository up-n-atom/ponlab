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

#include <linux/delay.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/etherdevice.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/of_net.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/version.h>
#include <net/datapath_api.h>
#include <net/datapath_br_domain.h>
#include <dt-bindings/net/mxl,vuni.h>

#define DRV_MODULE_NAME		"pon_hgu_vuni"
#define DRV_MODULE_VERSION	 "1.0"

#define MAX_SUB_IF 4
#define NUM_IF 9

/* 9600 - refer to ethernet drv and overhead is 18 bytes */
#define MXL_VUNI_MAX_DATA_LEN 9582
/* Min IPv4 MTU per RFC791 */
#define MXL_VUNI_MIN_MTU 68

/* standard MTU packet size*/
#define MXL_VUNI_STD_MTU_SIZE 1500

const char *string_array[MAX_SUB_IF] = {NULL};
static u32 vuni_subif_type[MAX_SUB_IF];
static u32 vuni_subif_domain[MAX_SUB_IF];

struct vuni_hw {
	struct net_device *devs[NUM_IF];
	int num_devs;
};

/**
 * This structure is used internal purpose
 */
struct mxl_vuni_priv {
	/*!< network device interface Statistics */
	struct rtnl_link_stats64 stats;
	struct sk_buff *skb; /*!< skb buffer structure*/
	int current_speed; /*!< interface current speed*/
	int full_duplex; /*!< duplex mode*/
	int current_duplex; /*!< current interface duplex mode*/
	unsigned int                flags;  /*!< flags */
	struct module *owner;
	dp_subif_t dp_subif;
	s32 dev_port; /*dev  instance */
	s32 f_dp;   /* status for register to datapath*/
	u32 dp_port_id;
	int num_port;
	struct vuni_hw *hw;
	int id;
	int vani;
	int extra_subif;
	u32 extra_subif_domain;
	/*! min netdevices for extra subif/lct */
	int start;
	/*! max netdevices for extra subif/lct */
	int end;
	struct ethtool_cmd bkup_cmd;
	u32 ethtool_flags;
	u8 needs_recovery;
	GSW_portCfg_t cfg;
	u32 num_ip_host; /* number of IP Host interfaces */
};

/**
 * This structure is used internal purpose
 */

struct mxl_net_soc_data {
	u8 need_defer;
	unsigned int queue_num;
	u32 mtu_limit;
};

/* length of time before we decide the hardware is borked,
 * and dev->eth_tx_timeout() should be called to fix the problem
 */
#define MXL_VUNI_TX_TIMEOUT	 (10 * HZ)

static int vuni_init(struct net_device *dev);
static int vuni_open(struct net_device *dev);
static int vuni_stop(struct net_device *dev);
static void vuni_uninit(struct net_device *dev);
static int vuni_xmit(struct sk_buff *skb, struct net_device *dev);
static int vuni_set_mac_address(struct net_device *dev, void *p);
static int vuni_ioctl(struct net_device *dev, struct ifreq *ifr, int);
static void vuni_get_stats(struct net_device *, struct rtnl_link_stats64 *);
static void vuni_tx_timeout(struct net_device *dev, unsigned int txqueue);
static int vuni_change_mtu(struct net_device *dev, int new_mtu);

/**
 *  Datapath directpath functions
 */
static int32_t dp_fp_stop_tx(struct net_device *);
static int32_t dp_fp_restart_tx(struct net_device *);
static int32_t dp_fp_rx(struct net_device *, struct net_device *,
			struct sk_buff *, int32_t);

static struct vuni_hw vuni_hw;
static struct module g_mxl_vuni_module[NUM_IF];
static struct mxl_net_soc_data g_soc_data;
static struct net_device *vuni_if[NUM_IF][MAX_SUB_IF] = {0};
static const struct net_device_ops mxl_vuni_drv_ops = {
	.ndo_init		= vuni_init,
	.ndo_open		= vuni_open,
	.ndo_stop		= vuni_stop,
	.ndo_uninit		= vuni_uninit,
	.ndo_start_xmit		= vuni_xmit,
	.ndo_set_mac_address	= vuni_set_mac_address,
	.ndo_get_stats64	= vuni_get_stats,
	.ndo_do_ioctl		= vuni_ioctl,
	.ndo_tx_timeout		= vuni_tx_timeout,
	.ndo_change_mtu		= vuni_change_mtu,
};

static int get_link_ksettings(struct net_device *dev,
			      struct ethtool_link_ksettings *cmd)
{
	linkmode_set_bit(ETHTOOL_LINK_MODE_100baseT_Full_BIT,
			 cmd->link_modes.supported);
	linkmode_set_bit(ETHTOOL_LINK_MODE_100baseT_Full_BIT,
			 cmd->link_modes.advertising);
	linkmode_set_bit(ETHTOOL_LINK_MODE_100baseT_Full_BIT,
			 cmd->link_modes.lp_advertising);
	linkmode_set_bit(ETHTOOL_LINK_MODE_1000baseT_Full_BIT,
			 cmd->link_modes.supported);
	linkmode_set_bit(ETHTOOL_LINK_MODE_1000baseT_Full_BIT,
			 cmd->link_modes.advertising);
	linkmode_set_bit(ETHTOOL_LINK_MODE_1000baseT_Full_BIT,
			 cmd->link_modes.lp_advertising);
	linkmode_set_bit(ETHTOOL_LINK_MODE_10000baseT_Full_BIT,
			 cmd->link_modes.supported);
	linkmode_set_bit(ETHTOOL_LINK_MODE_10000baseT_Full_BIT,
			 cmd->link_modes.advertising);
	linkmode_set_bit(ETHTOOL_LINK_MODE_10000baseT_Full_BIT,
			 cmd->link_modes.lp_advertising);
	ethtool_link_ksettings_add_link_mode(cmd, supported, TP);
	ethtool_link_ksettings_add_link_mode(cmd, supported, MII);

	cmd->base.port = PORT_MII;
	cmd->base.speed = 10000;
	cmd->base.duplex = DUPLEX_FULL;
	return 0;
}

static int set_link_ksettings(struct net_device *dev,
			      const struct ethtool_link_ksettings *cmd)
{
	return 0;
}

static void get_pauseparam(struct net_device *dev,
			   struct ethtool_pauseparam *epause)
{
	struct mxl_vuni_priv *priv;
	GSW_portCfg_t *p_cfg;

	priv = netdev_priv(dev);
	p_cfg = &priv->cfg;

	switch (p_cfg->eFlowCtrl) {
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
	struct mxl_vuni_priv *priv;
	GSW_portCfg_t *p_cfg;

	priv = netdev_priv(dev);
	p_cfg = &priv->cfg;
	if (epause->autoneg)
		p_cfg->eFlowCtrl = GSW_FLOW_AUTO;
	else if (epause->rx_pause)
		p_cfg->eFlowCtrl =
			epause->tx_pause ? GSW_FLOW_RXTX : GSW_FLOW_RX;
	else if (epause->tx_pause)
		p_cfg->eFlowCtrl = GSW_FLOW_TX;
	else
		p_cfg->eFlowCtrl = GSW_FLOW_OFF;
	return 0;
}

/* Get the driver information, used by ethtool_ops  */
static void get_drvinfo(struct net_device *dev, struct ethtool_drvinfo *info)
{
	/* driver short name (Max 32 characters) */
	strcpy(info->driver, DRV_MODULE_NAME);
	/* driver version(Max 32 characters) */
	strcpy(info->version, DRV_MODULE_VERSION);
}

#define ETHTOOL_FLAG_CLASS_SELECT_SPCP_DSCP BIT(0)

static const char priv_flags_strings[][ETH_GSTRING_LEN] = {
	"qos_class_select_spcp_dscp",
};

#define ETHTOOL_PRIV_FLAGS_STR_LEN ARRAY_SIZE(priv_flags_strings)

static u32 get_priv_flags(struct net_device *dev)
{
	struct mxl_vuni_priv *priv = netdev_priv(dev);

	return priv->ethtool_flags;
}

static int set_priv_flags(struct net_device *dev, u32 flags)
{
	struct mxl_vuni_priv *priv = netdev_priv(dev);
	struct core_ops *gsw_handle = NULL;
	GSW_return_t ret = 0;
	GSW_QoS_portCfg_t qosPortCfg = {0, };

	if (priv->vani)
		return -EPERM;

	if (!(flags ^ priv->ethtool_flags))
		return 0;

	if (priv->dp_port_id == DP_FAILURE) {
		pr_err("%s failed for device %s due to invalid dp port id\n",
		       __func__, dev->name);
		return -EINVAL;
	}

	gsw_handle = gsw_get_swcore_ops(0);
	if (!gsw_handle) {
		pr_err("%s failed for device: %s - no gsw handle\n",
		       __func__, dev->name);
		return -EINVAL;
	}

	qosPortCfg.nPortId = priv->dp_port_id;
	if (flags & ETHTOOL_FLAG_CLASS_SELECT_SPCP_DSCP)
		qosPortCfg.eClassMode = GSW_QOS_CLASS_SELECT_SPCP_DSCP;
	else
		qosPortCfg.eClassMode = GSW_QOS_CLASS_SELECT_NO;

	ret = gsw_handle->gsw_qos_ops.QoS_PortCfgSet(gsw_handle,
						     &qosPortCfg);
	if (ret != GSW_statusOk) {
		pr_err("%s: QoS_PortCfgSet failed for port %d: %d\n",
		       __func__, qosPortCfg.nPortId, ret);
		return ret;
	}
	priv->ethtool_flags = flags & ETHTOOL_FLAG_CLASS_SELECT_SPCP_DSCP;

	return ret;
}

static void get_strings(struct net_device *netdev, u32 stringset, u8 *data)
{
	int i;

	switch (stringset) {
	case ETH_SS_PRIV_FLAGS:
		for (i = 0; i < ETHTOOL_PRIV_FLAGS_STR_LEN; i++) {
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
		return ETHTOOL_PRIV_FLAGS_STR_LEN;
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

static const struct ethtool_ops ethtool_ops = {
	.get_drvinfo		= get_drvinfo,
	.get_link		= ethtool_op_get_link,
	.get_link_ksettings	= get_link_ksettings,
	.set_link_ksettings	= set_link_ksettings,
	.get_pauseparam		= get_pauseparam,
	.set_pauseparam		= set_pauseparam,
	.get_strings		= get_strings,
	.get_sset_count		= get_stringset_count,
	.get_ethtool_stats	= get_ethtool_stats,
	.get_priv_flags		= get_priv_flags,
	.set_priv_flags		= set_priv_flags,
};

/* Get used port ID as well as owner information. Used by PON ethernet driver.
 * This information is reused when creating IP Host devices.
 */
int mxl_vuni_drv_iphost_info_get(unsigned int interface, int *dp_port_id,
				 struct module **owner)
{
	struct net_device *dev;
	struct mxl_vuni_priv *priv;

	if (interface >= NUM_IF) {
		pr_err("[%s] called with invalid interface number %u\n",
		       __func__, interface);
		return -EINVAL;
	}

	dev = vuni_hw.devs[interface];
	priv = netdev_priv(dev);

	if (!dp_port_id || !owner || !priv) {
		pr_err("[%s] called with invalid parameter\n", __func__);
		return -EINVAL;
	}

	*dp_port_id = priv->dp_port_id;
	*owner = priv->owner;

	return 0;
}
EXPORT_SYMBOL(mxl_vuni_drv_iphost_info_get);

/* Set the MAC address */
static int vuni_set_mac_address(struct net_device *dev, void *p)
{
	struct sockaddr *addr = p;

	if (netif_running(dev))
		return -EBUSY;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EINVAL;

	memcpy(dev->dev_addr, addr->sa_data, dev->addr_len);
	return 0;
}

/* open the network device interface*/
static int vuni_open(struct net_device *dev)
{
	struct mxl_vuni_priv *priv;
	struct net_device *vani_dev;
	bool flags = true;
	int ret;

	priv = netdev_priv(dev);

	if (dp_rx_enable(dev, dev->name, flags) != DP_SUCCESS) {
		pr_err("[%s] failed to enable the device: %s\n",
		       __func__, dev->name);
		return -1;
	}

	/* If the VUNI interface is put into up state also activate the carrier
	 * on the VANI interface to signal that the PON is up to the other
	 * side.
	 */
	if (priv->vani == 0) {
		vani_dev = vuni_if[priv->id][SUBIF_VANI0_0];
		/* This check is assuming that if the device is not a vani
		 * device then it is any of the vuni device (vuni_dev0_0,
		 * vuni_dev0_1 or vuni_dev0_2)
		 */
		if (vani_dev != dev) {
			netif_carrier_on(dev);
			/* Setting of IFF_UP flag is not working from vuni_init
			 * , as link is not present yet (__LINK_STATE_PRESENT
			 * is not set). So, for now setting the flag from
			 * vuni_open.
			 */
			ret = dev_change_flags(vani_dev,
					       vani_dev->flags | IFF_UP, NULL);
			if (ret < 0) {
				pr_err("[%s] Failed to set the state of the device : %s\n",
				       __func__, vani_dev->name);
				return ret;
			}
			netif_carrier_on(vani_dev);
		}
	}

	return 0;
}

/* Stop the network device interface*/
static int vuni_stop(struct net_device *dev)
{
	struct mxl_vuni_priv *priv;
	struct net_device *vani_dev;
	struct net_device *vuni_dev;
	bool flags = false;

	priv = netdev_priv(dev);

	if (dp_rx_enable(dev, dev->name, flags) != DP_SUCCESS) {
		pr_err("%s: failed to disable the device: %s\n",
		       __func__, dev->name);
		return -1;
	}

	/* If the VUNI interface is put into down state also deactivate the
	 * carrier on the VANI interface to signal that the PON is down to the
	 * other side.
	 */
	if (priv->vani == 0) {
		vani_dev = vuni_if[priv->id][SUBIF_VANI0_0];
		vuni_dev = vuni_if[priv->id][SUBIF_VUNI0_0];
		if (vuni_dev == dev)
			netif_carrier_off(vani_dev);
	}

	return 0;
}

/* Uninit the network device interface*/
static void vuni_uninit(struct net_device *dev)
{
	struct dp_subif_data data = {0};
	struct mxl_vuni_priv *priv;
	int ret;

	priv = netdev_priv(dev);

	if (priv->dp_port_id == DP_FAILURE) {
		pr_err("dp port id (%d) is invalid. ignore the deregister.\n",
		       priv->dp_port_id);
		return;
	}
	priv->dp_subif.subif = -1;
	priv->dp_subif.port_id = priv->dp_port_id;
	if (priv->vani) {
		data.flag_ops = DP_SUBIF_VANI | DP_SUBIF_SWDEV;
		ret = dp_register_subif_ext(0, priv->owner, dev, dev->name,
					    &priv->dp_subif, &data,
					    DP_F_DEREGISTER);
	} else {
		ret = dp_register_subif_ext(0, priv->owner, dev, dev->name,
					    &priv->dp_subif, NULL,
					    DP_F_DEREGISTER);
	}

	if (ret != DP_SUCCESS)
		pr_err("%s: failed to close the device: %s ret %d\n",
		       __func__, dev->name, ret);
}

/* Send the packet to network rx queue */
static void vuni_rx(struct net_device *dev, int len, struct sk_buff *skb)
{
	struct mxl_vuni_priv *priv;

	priv = netdev_priv(dev);
	skb->protocol = eth_type_trans(skb, dev);

	if (dev->features & NETIF_F_RXCSUM)
		skb->ip_summed = CHECKSUM_UNNECESSARY;

	netif_rx(skb);
	priv->stats.rx_packets++;
	priv->stats.rx_bytes += len;
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
	struct mxl_vuni_priv *priv;
	struct net_device *vani_dev;
	struct net_device *vuni_dev;
	u32 vani_flag = 0;

#ifdef DUMP_PACKET
	if (skb && skb->data) {
		print_hex_dump(KERN_INFO, "",
			       DUMP_PREFIX_ADDRESS, 16, 1,
			       skb->data, len, true);
	}
#endif
	if (!skb) {
		pr_err("%s: skb from DP is null !\n", __func__);
		return -1;
	}
#ifndef CONFIG_DPM_DATAPATH_HAL_GSWIP32
	if (skb) {
		/*Remove PMAC to DMA header */
		len -= DP_MAX_PMAC_LEN;
		skb_pull(skb, DP_MAX_PMAC_LEN);
	}
#endif
	/* Pass it to the stack */
#ifdef DUMP_PACKET

	if (skb && skb->data) {
		print_hex_dump(KERN_INFO, "",
			       DUMP_PREFIX_ADDRESS, 16, 1,
			       skb->data, len, true);
	}

#endif

	/* DS,
	 * DW0.b16=0 and we need to convert vUNI Rx to vANI Rx and send to
	 * n/w stack
	 * US,
	 * DW0.b16=1 set by vANI Tx and packet to CPU (IGMP/OMCI) should be
	 * converted in vUNI0_0 rx fn and send to n/w stack
	 */
	if (!(skb->DW0 & (1 << 16)))
		vani_flag = 1;

	priv = netdev_priv(txif ? txif : rxif);
	if (vani_flag) {
		/* DW0 bit 16 = 0 DS data, packet is from VUNI
		 * and forward to vANI
		 */
		vani_dev = vuni_if[priv->id][SUBIF_VANI0_0];
		pr_debug("%s, to use vani_dev\n", __func__);
		skb->dev = vani_dev;
	} else {
		/* DW0 bit 16 = 1; packet is from vANI and forward to VUNI0_0 */
		vuni_dev = vuni_if[priv->id][SUBIF_VUNI0_0];
		pr_debug("%s, to use vuni_us\n", __func__);
		skb->dev = vuni_dev;
	}

	/* For LGM txif will never be set since GSWIP don't sent EgFlag=1
	 * Downstream after GSWIP bridging vUNI driver will see RX with rxif and
	 * VUNI to VANI conversion will take effect
	 * Upstream, there is no RX to vUNI driver,
	 * since CPU is removed from bridge port
	 */
	skb->offload_fwd_mark = 0;

	if (vani_flag) {
		vuni_rx(vani_dev, len, skb);
	} else {
		/* for ctrl packets Upstream
		 * there is RX to PON multicast driver
		 */
		vuni_rx(vuni_dev, len, skb);
	}

	return 0;
}

/* Get the network device stats information */
static
void vuni_get_stats(struct net_device *dev,
		    struct rtnl_link_stats64 *storage)
{
	struct mxl_vuni_priv *priv = netdev_priv(dev);
#ifndef CONFIG_DPM_DATAPATH_MIB
	*storage = priv->stats;
#else
	struct net_device *vani_dev = vuni_if[priv->id][SUBIF_VANI0_0];
	struct rtnl_link_stats64 temp_stor;
	struct net_device *vuni_dev;
	int  i, id;

	if (dev != vani_dev) {
		dp_get_netif_stats(dev, NULL, storage, 0);

		storage->tx_dropped += priv->stats.tx_dropped;
		storage->tx_errors += priv->stats.tx_errors;
		storage->rx_dropped += priv->stats.rx_dropped;
		storage->rx_errors += priv->stats.rx_errors;
	} else {
		id = priv->id;
		for (i = 1; i < MAX_SUB_IF; i++) {
			vuni_dev =  vuni_if[id][i];
			if (vuni_dev) {
				priv = netdev_priv(vuni_dev);
				memset(&temp_stor, 0, sizeof(temp_stor));

				dp_get_netif_stats(dev, NULL, &temp_stor, 0);
				storage->tx_dropped += priv->stats.tx_dropped
							 + temp_stor.tx_dropped;
				storage->tx_errors += priv->stats.tx_errors
							+ temp_stor.tx_errors;
				storage->tx_bytes += priv->stats.tx_bytes
							+ temp_stor.tx_bytes;
				storage->tx_packets += priv->stats.tx_packets
							+ temp_stor.tx_packets;
				storage->rx_dropped += priv->stats.rx_dropped
							+ temp_stor.rx_dropped;
				storage->rx_errors += priv->stats.rx_errors
							+ temp_stor.rx_errors;
				storage->rx_bytes += priv->stats.rx_bytes
							+ temp_stor.rx_bytes;
				storage->rx_packets += priv->stats.rx_packets
							+ temp_stor.rx_packets;
			}
		}
	}
#endif
}

/* Trasmit timeout */
static void vuni_tx_timeout(struct net_device *dev, unsigned int txqueue)
{
}

static int vuni_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct mxl_vuni_priv *priv = netdev_priv(dev);
	struct net_device *vani_dev;
	struct net_device *vuni_dev;
	unsigned int len;
	int ret;

	if (skb_put_padto(skb, ETH_ZLEN)) {
		priv->stats.tx_dropped++;
		return NETDEV_TX_OK;
	}

	vani_dev = vuni_if[priv->id][SUBIF_VANI0_0];
	vuni_dev = vuni_if[priv->id][SUBIF_VUNI0_0];
	if (dev != vani_dev) {
		/*drop the packet*/
		priv->stats.tx_dropped++;
		dev_kfree_skb_any(skb);
		return 0;
	}
	/*convert vANIx to vUNIx_0, use the first subif*/
	skb->dev = vuni_dev;
	priv = netdev_priv(vuni_dev);
	/* Call the Datapath Library's TX function */
	((struct dma_tx_desc_1 *)&skb->DW1)->field.ep = priv->dp_subif.port_id;
	((struct dma_tx_desc_0 *)&skb->DW0)->field.dest_sub_if_id =
		priv->dp_subif.subif;

	len = skb->len;

	/* set DW0 Bit 16 to 1
	 * make sure it's maintained until cqm tx to HW
	 */
	skb->DW0 |= (1 << 16);

	ret = dp_xmit(vuni_dev, &priv->dp_subif, skb, skb->len,
		      dev->features & NETIF_F_HW_CSUM ? DP_TX_CAL_CHKSUM : 0);
	if (!ret) {
		priv->stats.tx_packets++;
		priv->stats.tx_bytes += len;
	} else {
		priv->stats.tx_dropped++;
	}
	return 0;
}

/* Platform specific IOCTL's handler */
static int vuni_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	return -EOPNOTSUPP;
}

/* Change the MTU value of the network device interfaces */
static int vuni_change_mtu(struct net_device *dev, int new_mtu)
{
	if (new_mtu > g_soc_data.mtu_limit)
		new_mtu = g_soc_data.mtu_limit;
	else if (new_mtu < MXL_VUNI_MIN_MTU)
		new_mtu = MXL_VUNI_STD_MTU_SIZE;

	if (dp_set_mtu_size(dev, new_mtu) != 0) {
		pr_err("fail to set mtu size as %u\n", new_mtu);
		return -EINVAL;
	}
	dev->mtu = new_mtu;
	return 0;
}

static void vuni_domain_set(struct net_device *dev,
			    struct dp_subif_data *data)
{
	struct mxl_vuni_priv *priv;

	priv = netdev_priv(dev);
	if (priv->extra_subif_domain != VUNI_BR_DOMAIN_NO) {
		data->flag_ops |= DP_SUBIF_BR_DOMAIN;

		switch (priv->extra_subif_domain) {
		case VUNI_BR_DOMAIN_UC:
			data->domain_id = DP_BR_DM_UC;
			break;
		case VUNI_BR_DOMAIN_MC:
			data->domain_id = DP_BR_DM_MC;
			break;
		case VUNI_BR_DOMAIN_BC:
			data->domain_id = DP_BR_DM_BC1;
			break;
		default:
			data->domain_id = DP_BR_DM_UC;
			break;
		}

		if (priv->extra_subif_domain == VUNI_BR_DOMAIN_UC)
			data->domain_members = DP_BR_DM_MEMBER(DP_BR_DM_UC) |
					       DP_BR_DM_MEMBER(DP_BR_DM_UCA) |
					       DP_BR_DM_MEMBER(DP_BR_DM_BC1);

		pr_debug("domain set for dev %s id %d member 0x%x\n",
			 dev->name, data->domain_id, data->domain_members);
	}
}

/* init of the network device */
static int vuni_init(struct net_device *dev)
{
	struct dp_subif_data data = {0};
	struct mxl_vuni_priv *priv;
	int ret;
	struct dp_bp_attr conf;

	priv = netdev_priv(dev);

	if (priv->vani == 1) {
		priv->dp_subif.subif = -1;
		priv->dp_subif.port_id = priv->dp_port_id;
		data.flag_ops = DP_SUBIF_VANI | DP_SUBIF_SWDEV;
		ret = dp_register_subif_ext(0, priv->owner,
					    dev, dev->name, &priv->dp_subif,
					    &data, 0);
	} else {
		priv->dp_subif.subif = -1;
		priv->dp_subif.port_id = priv->dp_port_id;
		data.flag_ops = DP_SUBIF_SEG_EN;
		vuni_domain_set(dev, &data);
		ret = dp_register_subif_ext(0, priv->owner, dev, dev->name,
					    &priv->dp_subif, &data, 0);
		/* For vUNI need to remove CPU from bridge port member list */
		conf.inst = 0;
		conf.dev = dev;
		conf.en = 0;
		dp_set_bp_attr(&conf, 0);
	}

	if (ret != DP_SUCCESS) {
		pr_err("failed to open device: %s\n", dev->name);
		return -1;
	}

	netif_carrier_off(dev);

	/* get the minimum MTU and call the change mtu */
	dp_get_mtu_size(dev, &g_soc_data.mtu_limit);

	if (!(dev->mtu > MXL_VUNI_MIN_MTU && dev->mtu < g_soc_data.mtu_limit))
		dev->mtu = g_soc_data.mtu_limit;
	dev->max_mtu = g_soc_data.mtu_limit;

	vuni_change_mtu(dev, dev->mtu);

	dev->ethtool_ops = &ethtool_ops;
	return 0;
}

static void
vuni_drv_eth_addr_setup(struct net_device *dev, int port, int vani)
{
	u8 values[ETH_ALEN] = {0};
	int i = 0;

	if (is_valid_ether_addr(dev->dev_addr))
		return;

	/* read MAC address from the MAC table and put them into device */
	if (is_valid_ether_addr(values)) {
		for (i = 0; i < ETH_ALEN; i++)
			dev->dev_addr[i] = values[i];
	} else {
		eth_hw_addr_random(dev);
	}
}

static int mxl_vuni_dev_reg(struct net_device *dev_0, struct vuni_hw *hw,
			    u32 dp_port, int start, int end)
{
	struct net_device *dev = dev_0;
	struct mxl_vuni_priv *priv;
	int err;
	int i;

	for (i = start; i <= end; i++) {
		if (i) {
			dev = alloc_etherdev_mq(sizeof(struct mxl_vuni_priv),
						g_soc_data.queue_num);
			if (!dev) {
				pr_err("allocation failed for interface %d\n",
				       i);
				return -ENOMEM;
			}
		}
		vuni_if[hw->num_devs][i] = dev;
		priv = netdev_priv(vuni_if[hw->num_devs][i]);
		priv->dp_port_id = dp_port;
		priv->hw = hw;
		priv->id = hw->num_devs;
		priv->owner = &g_mxl_vuni_module[hw->num_devs];
		snprintf(priv->owner->name, IFNAMSIZ, "module%02d", priv->id);
		if (start == i) {
			priv->vani = 1;
			snprintf(vuni_if[hw->num_devs][i]->name, IFNAMSIZ,
				 string_array[i], hw->num_devs);
		} else {
			priv->vani = 0;
			snprintf(vuni_if[hw->num_devs][i]->name, IFNAMSIZ,
				 string_array[i], hw->num_devs);
			priv->extra_subif_domain = vuni_subif_domain[i];
		}

		vuni_if[hw->num_devs][i]->netdev_ops = &mxl_vuni_drv_ops;
		vuni_drv_eth_addr_setup(vuni_if[hw->num_devs][i],
					priv->id, priv->vani);
		err = register_netdev(vuni_if[hw->num_devs][i]);
		if (err) {
			pr_err("%s: failed to register netdevice: %p %d, %d %s\n",
			       __func__, vuni_if[hw->num_devs][i], hw->num_devs,
			       err, vuni_if[hw->num_devs][i]->name);
				return -1;
		}
	}
	return 0;
}

static int vuni_dev_dereg_subif(int dev_num, int start, int end)
{
	struct dp_subif_data data = {0};
	struct mxl_vuni_priv *priv;
	int res;
	int i;

	for (i = start; i <= end; i++) {
		struct net_device *dev = vuni_if[dev_num][i];

		priv = netdev_priv(dev);
		priv->dp_subif.subif = priv->dp_subif.subif;
		priv->dp_subif.port_id = priv->dp_subif.port_id;
		if (priv->vani == 1) {
			data.flag_ops = DP_SUBIF_VANI | DP_SUBIF_SWDEV;
			res = dp_register_subif_ext(0, priv->owner,
						    dev, dev->name,
						    &priv->dp_subif,
						    &data, DP_F_DEREGISTER);
		} else {
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

static int vuni_dev_dereg(int dev_num, int start, int end)
{
	u8 i;

	for (i = start; i <= end; i++) {
		struct mxl_vuni_priv *priv;
		struct net_device *dev = vuni_if[dev_num][i];

		netif_stop_queue(dev);
		priv = netdev_priv(dev);
		priv->dp_port_id = DP_FAILURE;
		unregister_netdev(dev);
		free_netdev(dev);
	}
	return 0;
}

static int vuni_of_iface(struct vuni_hw *hw, struct device_node *iface,
			 struct platform_device *pdev)
{
	struct dp_dev_data dev_data = {0};
	struct dp_port_data data = {0};
	struct mxl_vuni_priv *priv;
	u32 extra_subif_param;
	struct net_device *dev;
	dp_pmac_cfg_t pmac_cfg;
	u32 dp_port_id = 0;
	int extra_subif_num = 0;
	int extra_subif_domain_num = 0;
	int net_start = 0;
	dp_cb_t cb = {0};
	int net_end = 0;
	char name[16];
	int ret;

	/* alloc the network device */
	dev = alloc_etherdev_mq(sizeof(struct mxl_vuni_priv),
				g_soc_data.queue_num);

	if (!dev) {
		pr_err("allocation failed for interface %d\n",
		       hw->num_devs);
		return -ENOMEM;
	}

	hw->devs[hw->num_devs] = dev;
	priv = netdev_priv(hw->devs[hw->num_devs]);

	/* setup the network device */
	snprintf(name, sizeof(name), "vANI%d", hw->num_devs);
	extra_subif_param =
		of_property_count_strings(iface, "mxl,extra-subif");
	if (extra_subif_param > MAX_SUB_IF) {
		pr_err("Vuni subif exceeds the max limit\n");
		return -EINVAL;
	}
	ret = of_property_read_string_array(iface, "mxl,extra-subif",
					    string_array,
					    extra_subif_param);
	extra_subif_num =
		of_property_read_variable_u32_array(iface,
						    "mxl,extra-subif-type",
						    vuni_subif_type,
						    MAX_SUB_IF, MAX_SUB_IF);
	extra_subif_domain_num =
		of_property_read_variable_u32_array(iface,
						    "mxl,extra-subif-domain",
						    vuni_subif_domain,
						    MAX_SUB_IF, MAX_SUB_IF);

	if (ret < 0) {
		priv->extra_subif = 0;
	} else if (extra_subif_num < 0 ||
		   extra_subif_num != extra_subif_param) {
		priv->extra_subif = 0;
	} else if ((extra_subif_domain_num < 0 &&
		    extra_subif_domain_num != -EINVAL) ||
		   (extra_subif_domain_num > 0 &&
		    extra_subif_domain_num != extra_subif_param)) {
		priv->extra_subif = 0;
	} else {
		priv->extra_subif = extra_subif_param;
		/* Traffic between diff subif */
		if (!of_property_read_bool(iface,
					   "mxl,allow_subif_data_loop"))
			data.flag_ops = DP_F_DATA_NO_LOOP;
	}

	ret = of_property_read_u32_index(iface, "mxl,num_ip_host", 0,
					 &priv->num_ip_host);
	if (ret)
		priv->num_ip_host = 0;

	priv->start = 0;
	priv->end = priv->extra_subif - 1;

	strncpy(hw->devs[hw->num_devs]->name, name, IFNAMSIZ);
	hw->devs[hw->num_devs]->netdev_ops = &mxl_vuni_drv_ops;
	hw->devs[hw->num_devs]->watchdog_timeo = MXL_VUNI_TX_TIMEOUT;
	hw->devs[hw->num_devs]->needed_headroom = DP_MAX_PMAC_LEN;
	SET_NETDEV_DEV(hw->devs[hw->num_devs], &pdev->dev);

	/* setup our private data */
	priv->hw = hw;
	priv->id = hw->num_devs;

	priv->owner = &g_mxl_vuni_module[hw->num_devs];
	snprintf(priv->owner->name, IFNAMSIZ, "module%02d", priv->id);

	memset(&pmac_cfg, 0, sizeof(dp_pmac_cfg_t));
	pmac_cfg.eg_pmac_flags = EG_PMAC_F_PMAC;
	pmac_cfg.eg_pmac.pmac = 1;

	dp_port_id  = dp_alloc_port_ext(0, priv->owner, hw->devs[hw->num_devs],
					0, 0, &pmac_cfg, &data, DP_F_VUNI);

	if (dp_port_id == DP_FAILURE) {
		pr_err("dp_alloc_port failed for %s with port_id %d\n",
		       hw->devs[hw->num_devs]->name, priv->id + 1);
		return -ENODEV;
	}

	hw->devs[hw->num_devs]->dev_id = dp_port_id;
	priv->dp_port_id = dp_port_id;
	if (priv->extra_subif >= 0)
		dev_data.max_ctp = extra_subif_param + priv->num_ip_host + 1;
	cb.stop_fn = dp_fp_stop_tx;
	cb.restart_fn  = (dp_restart_tx_fn_t)dp_fp_restart_tx;
	cb.rx_fn = (dp_rx_fn_t)dp_fp_rx;

	if (dp_register_dev_ext(0, priv->owner,
				dp_port_id, &cb,
				&dev_data, 0) != DP_SUCCESS) {
		pr_err("dp_register_dev failed for %s\n and port_id %d",
		       hw->devs[hw->num_devs]->name, dp_port_id);
		dp_alloc_port_ext(0, priv->owner, hw->devs[hw->num_devs],
				  dp_port_id, dp_port_id,
				  NULL, NULL, DP_F_DEREGISTER);
		return -ENODEV;
	}

	net_start = priv->start;
	net_end = priv->end;
	mxl_vuni_dev_reg(dev, &vuni_hw, dp_port_id, net_start, net_end);
	hw->num_devs++;
	return 0;
}

/* Initialization Ethernet module */
static int mxl_vuni_drv_init(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	struct device_node *iface_np;

	memset(g_mxl_vuni_module, 0, sizeof(g_mxl_vuni_module));
	memset(vuni_subif_domain, VUNI_BR_DOMAIN_NO, sizeof(vuni_subif_domain));

	/* load the interfaces */
	/* add a dummy interface */
	vuni_hw.num_devs = 0;
	for_each_available_child_of_node(node, iface_np) {
		if (vuni_hw.num_devs < NUM_IF) {
			if (vuni_of_iface(&vuni_hw, iface_np, pdev))
				return -EINVAL;
		} else {
			dev_err(&pdev->dev,
				"only %d interfaces allowed\n",
				NUM_IF);
				return -EINVAL;
		}
	}

	if (!vuni_hw.num_devs) {
		dev_dbg(&pdev->dev, "failed to load interfaces\n");
		return -ENOENT;
	}

	g_soc_data.mtu_limit = MXL_VUNI_MAX_DATA_LEN;

	pr_info("MXL vUNI driver init.\n");
	return 0;
}

static void mxl_vuni_drv_exit(struct platform_device *pdev)
{
	int ret;
	int i;

	for (i = 0; i < vuni_hw.num_devs; i++) {
		struct mxl_vuni_priv *priv;
		struct dp_dev_data dev_data = {0};
		struct net_device *dev = vuni_hw.devs[i];

		if (!dev)
			continue;

		netif_stop_queue(dev);

		priv = netdev_priv(dev);
		vuni_dev_dereg_subif(i, priv->start, priv->end);
		ret = dp_register_dev_ext(0, priv->owner,
					  priv->dp_port_id, NULL,
					  &dev_data,
					  DP_F_DEREGISTER);
		if (ret != DP_SUCCESS) {
			pr_err("%s: failed to call deregister dp_port_id : %d\n",
			       __func__, priv->dp_port_id);
		}
		ret = dp_alloc_port_ext(0, priv->owner, NULL, 0,
					priv->dp_port_id, NULL,
					NULL, DP_F_DEREGISTER);
		if (ret != DP_SUCCESS) {
			pr_err("%s: failed to call dealloc for dp_port_id : %d\n",
			       __func__, priv->dp_port_id);
		}

		priv->dp_port_id = DP_FAILURE;
			/* Deregister netdevices */
		vuni_dev_dereg(i, priv->start, priv->end);
	}
	memset(&vuni_hw, 0, sizeof(vuni_hw));
}

static int mxl_vuni_drv_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct mxl_net_soc_data *soc_data = NULL;

	if (!dp_is_ready())
		return -EPROBE_DEFER;

	soc_data = (struct mxl_net_soc_data *)of_device_get_match_data(dev);
	if (!soc_data) {
		dev_err(dev, "No data found for vuni drv!\n");
		return -EINVAL;
	}

	memcpy(&g_soc_data, soc_data, sizeof(*soc_data));
	return mxl_vuni_drv_init(pdev);
}

static int mxl_vuni_drv_remove(struct platform_device *pdev)
{
	/* Just do the exit */
	mxl_vuni_drv_exit(pdev);
	return 0;
}

static const struct mxl_net_soc_data prx300_net_data = {
	.need_defer = false,
	.queue_num = 8,
};

static const struct of_device_id mxl_vuni_drv_match[] = {
	{ .compatible = "mxl,hgu-vuni", .data = &prx300_net_data},
	{},
};

MODULE_DEVICE_TABLE(of, mxl_vuni_drv_match);

static struct platform_driver mxl_vuni_driver = {
	.probe = mxl_vuni_drv_probe,
	.remove = mxl_vuni_drv_remove,
	.driver = {
		.name = "mxl,hgu-vuni",
		.of_match_table = mxl_vuni_drv_match,
		.owner = THIS_MODULE,
	},
};

module_platform_driver(mxl_vuni_driver);

MODULE_DESCRIPTION("PON HGU vUNI driver");
MODULE_LICENSE("GPL v2");
