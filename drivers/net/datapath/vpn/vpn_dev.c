// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
 * Copyright (C) 2019 Intel Corporation.
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
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/firmware.h>
#include <linux/genalloc.h>
#include <linux/iopoll.h>
#include <linux/kthread.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/of_platform.h>
#include <linux/regmap.h>
#include <linux/reset.h>

#include <linux/pp_api.h>
#include <soc/mxl/mxl_skb_ext.h>
#include <net/datapath_api.h>
#include <net/datapath_api_tx.h>
#include <net/ppa/ppa_api.h>
#include <net/ppa/ppa_hook.h>
#include <soc/mxl/mxl_sec.h>
#include <dt-bindings/soc/mxl,firewall.h>

#include "vpn.h"

#define FIRMWARE_NAME "vpn_fw.img"

#define FIRMWARE_REGION_SIZE (128 * SZ_1K)
#define DBG_BUF_REGION_SIZE (128 * SZ_1K)

enum soc_ver {
	LGM_A_SOC = 0xA0,
	LGM_B_SOC = 0xB0,
	LGM_C_SOC = 0xC0
};

/* global private data */
static const struct net_device_ops vpn_net_ops;
static dp_subif_t *vpn_subif;

/* dump debug buffer from arc-em */
static void vpn_print_dbg(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct vpn_data *priv = container_of(dwork, struct vpn_data, dbg_work);
	int index;

	if (!priv->genconf->dbg_enable)
		return;

	if (priv->genconf->dbg_rd_cnt == priv->genconf->dbg_wr_cnt)
		goto reschedule;

	while (priv->genconf->dbg_rd_cnt != priv->genconf->dbg_wr_cnt) {
		index = priv->genconf->dbg_rd_cnt & (DBG_BUF_COUNT - 1);
		pr_info("%s", (char *)priv->dbg_addr[index]);
		priv->genconf->dbg_rd_cnt++;
		priv->genconf->dbg_rd_idx++;
		if (priv->genconf->dbg_rd_idx == DBG_BUF_COUNT)
			priv->genconf->dbg_rd_idx = 0;

		usleep_range(1, 2);
	}

reschedule:
	schedule_delayed_work(dwork, usecs_to_jiffies(100));
}

/* Initialize firmware debug buffer.
 * vpn firmware will initialize SRAM during its boot, so debug buffer
 * must be configured after its boot.
 */
static int vpn_init_dbg(struct vpn_data *priv)
{
	int i;
	u8 *p;

	p = priv->dbg_base;
	priv->genconf->dbg_rd_cnt = 0;
	priv->genconf->dbg_wr_cnt = 0;

	for (i = 0; i < DBG_BUF_COUNT; i++) {
		priv->dbg_addr[i] = p;
		priv->genconf->dbg_addr[i] = virt_to_phys(p);
		p += DBG_BUF_SIZE * sizeof(u32);
	}

	INIT_DELAYED_WORK(&priv->dbg_work, vpn_print_dbg);

	dev_dbg(priv->dev, "Initialized debug ring buffer at: %x\n",
		priv->genconf->dbg_addr[0]);

	return 0;
}

static irqreturn_t vpn_irq(int irq, void *data)
{
	struct vpn_data *priv = data;

#if defined(CONFIG_DEBUG_FS)
	/* schedule debug print */
	schedule_delayed_work(&priv->dbg_work, msecs_to_jiffies(1));
#endif

	/* clear irq */
	regmap_update_bits(priv->syscfg, EM62ATOM, 1, 0);

	return IRQ_HANDLED;
}

/* load ARC-EM6 firmware */
static int vpn_load_firmware(struct vpn_data *priv)
{
	struct mxl_fw_dl_data data = {0};
	void *base_virt;
	dma_addr_t base_phys;
	size_t base_size;
	int align = SZ_1M;
	unsigned int stat;
	int ret;

	data.dev = priv->dev;
	data.sai = priv->sai;
	data.opt = MXL_FW_OPT_SKIP_FWRULE;
	priv->ddr_size = FIRMWARE_REGION_SIZE + DBG_BUF_REGION_SIZE + align;
	priv->ddr_virt = mxl_soc_alloc_firmware_buf(priv->dev, priv->ddr_size,
						    &data);
	if (!priv->ddr_virt) {
		dev_err(priv->dev, "failed to alloc firmware memory\n");
		return -EIO;
	}
	priv->ddr_phys = dma_map_single(priv->dev, priv->ddr_virt,
					priv->ddr_size, DMA_FROM_DEVICE);

	base_virt = PTR_ALIGN(priv->ddr_virt, align);
	base_phys = ALIGN(priv->ddr_phys, align);
	base_size = priv->ddr_size - (base_phys - priv->ddr_phys);
	priv->dbg_base = base_virt + FIRMWARE_REGION_SIZE;

	if (mxl_soc_request_firmware_into_buf(&priv->fw, FIRMWARE_NAME,
					      priv->dev, base_virt,
					      base_size, &data)) {
		dev_err(priv->dev, "failed to load firmware: %s\n",
			FIRMWARE_NAME);
		return -EIO;
	}

	/* firmware header allocation and compatible id check */
	priv->fw_hdr = devm_kzalloc(priv->dev, sizeof(struct fw_hdr),
				    GFP_KERNEL);
	if (!priv->fw_hdr) {
		ret = -ENOMEM;
		goto exit;
	}
	memcpy(priv->fw_hdr, priv->fw->data, sizeof(struct fw_hdr));
	if (priv->fw_hdr->compatible_id != FW_COMPATIBLE_ID) {
		dev_err(priv->dev, "Compat ID mismatch (%x:%x)\n",
			priv->fw_hdr->compatible_id, FW_COMPATIBLE_ID);
		ret = -EINVAL;
		goto exit;
	}

	/* shift fw body to aligned DDR */
	memmove(base_virt, base_virt + sizeof(struct fw_hdr),
		priv->fw->size - sizeof(struct fw_hdr));

	/* Set FW region to read only */
	mxl_soc_request_protect_buf(priv->dev, priv->ddr_virt, priv->ddr_size,
				    &data);

	/* disable ARC */
	regmap_update_bits(priv->syscfg, ARC_EM6_CR, 1, 0);
	reset_control_assert(priv->rst);

	/* assign genconf to SRAM */
	priv->genconf = priv->sram_virt + priv->fw_hdr->genconf_offset;
	priv->genconf->load_stat = VPN_FW_NOT_LOADED;

	/* enable/reset ARC */
	regmap_update_bits(priv->syscfg, ARC_EM6_FM_BASE, ~0, base_phys);
	regmap_update_bits(priv->syscfg, ARC_EM6_CR, 1, 1);
	usleep_range(500, 1000);
	reset_control_deassert(priv->rst);

	/* wait till ready */
	ret = vpn_poll_timeout(priv->genconf->load_stat == VPN_FW_LOADED_IN_CCM,
			       1000, priv->soc_data->timeout_ms * 1000);
	if (ret) {
		dev_err(priv->dev, "timedout waiting for firmware ready %d\n",
			priv->genconf->load_stat);
		ret = -ETIMEDOUT;
		goto exit;
	}

	ret = regmap_read(priv->syscfg, ARC_EM6_SR, &stat);
	if (ret) {
		dev_err(priv->dev, "failed to read status\n");
		ret = -EIO;
		goto exit;
	}

	dev_dbg(priv->dev, "Firmware %s loaded at %llx:%px stat 0x%x\n",
		FIRMWARE_NAME, base_phys, base_virt, stat);

exit:
	return ret;
}

static int vpn_init_genconf(struct vpn_data *priv)
{
	priv->genconf->dbg_out = 0;
	priv->genconf->e97_init_flag = 1;
	priv->genconf->e97_ring_id = 0;
	priv->genconf->e97_ring_nr = 1;
	priv->genconf->soc = priv->soc_data->soc;

	/* Mark status to firmware. This needs to be done after
	 * all other settings are done.
	 */
	wmb();
	priv->genconf->load_stat = VPN_FW_GENCONF_SET;

	return 0;
}

/* mark flag to inform firmware there is an update in config */
static void update_tunnel_flag(struct vpn_data *priv, int tunnel_id)
{
	u32 shift, clear;
	u32 *flag;
	int msg;

	flag = &priv->genconf->ipsec_flag;
	msg = INT_TUNL_UPD;

	shift = tunnel_id * 2;
	clear = GENMASK(shift + 1, shift);

	/* Update flag. This needs to be done after
	 * all other settings are done.
	 */
	wmb();
	*flag = (*flag & ~clear) | (TUN_UPD << shift);

	if (priv->soc_data->use_irq) {
		/* B-step, trigger Atom to ARC interrupt */
		priv->genconf->a2e_msg = msg;
		regmap_update_bits(priv->syscfg, ATOM2EM6, 1, 1);
	}
}

static void vpn_config_lookup(struct vpn_data *priv)
{
	struct dp_queue_map_set cfg = {0};

	cfg.q_id = priv->qid;
	cfg.map.dp_port = priv->dp_port;
	cfg.map.subif = priv->subif;
	cfg.map.egflag = 1;
	cfg.mask.class = 1;
	cfg.mask.subif = 1;
	cfg.mask.subif_id_mask = BIT(8);

	dp_queue_map_set(&cfg, 0);
}

/*!
 *@brief Look for an existing tunnel info from SPI
 *@param[in] dev: device
 *@param[in] spi: SPI index
 *@param[out] info: tunnel info
 *@return Returns 0 on succeed
 */
static int vpn_get_tunnel_info(struct device *dev, u32 spi,
			       struct mxl_vpn_tunnel_info *info)
{
	struct vpn_data *priv = dev_get_drvdata(dev);
	struct tunnel_info *tunnel;
	int i;

	spin_lock_bh(&priv->lock);

	for (i = 0; i < IPSEC_TUN_MAX; i++) {
		tunnel = &priv->tunnels[i];
		if (tunnel->spi == spi) {
			info->tunnel_id = i;
			info->mode = tunnel->mode;
			info->ip_mode = tunnel->ip_mode;
			info->iv_sz = tunnel->iv_sz;
			info->vpn_if = priv->netdev;
			info->gpid = priv->gpid;
			info->qid = priv->qid;
			info->subif = priv->subif;
			spin_unlock_bh(&priv->lock);
			return 0;
		}
	}

	spin_unlock_bh(&priv->lock);

	return -EINVAL;
}

/*!
 *@brief Update outbound action of tunnel
 *@param[in] dev: device
 *@param[in] tunnel_id: tunnel id
 *@param[in] act: action
 *@return Returns 0 on succeed
 */
static int vpn_update_tunnel_act(struct device *dev, int tunnel_id,
				 struct mxl_vpn_ipsec_act *act)
{
	struct vpn_data *priv = dev_get_drvdata(dev);
	struct ipsec_act *dwt;

	if (tunnel_id < 0 || tunnel_id >= IPSEC_TUN_MAX) {
		dev_err(priv->dev, "Invalid tunnel id %d\n", tunnel_id);
		return -EINVAL;
	}

	spin_lock_bh(&priv->lock);

	if (!priv->tunnels[tunnel_id].ctx) {
		dev_err(priv->dev, "Tunnel %d does not exist\n", tunnel_id);
		spin_unlock_bh(&priv->lock);
		return -EINVAL;
	}

	dwt = &priv->genconf->dwt[tunnel_id];
	dwt->dw0_mask = act->dw0_mask;
	dwt->dw0_val = act->dw0_val;
	dwt->dw1_mask = act->dw1_mask;
	dwt->dw1_val = act->dw1_val;
	dwt->enq_qos = act->enq_qos ? 1 : 0;

	update_tunnel_flag(priv, tunnel_id);

	spin_unlock_bh(&priv->lock);

	return 0;
}

/*!
 *@brief Update ingress netdev of tunnel
 *@param[in] dev: device
 *@param[in] tunnel_id: tunnel id
 *@param[in] netdev: netdev
 *@return Returns 0 on succeed
 */
static int vpn_update_tunnel_in_netdev(struct device *dev, int tunnel_id,
				       struct net_device *netdev)
{
	struct vpn_data *priv = dev_get_drvdata(dev);
	struct tunnel_info *tunnel;

	if (tunnel_id < 0 || tunnel_id >= IPSEC_TUN_MAX) {
		dev_err(priv->dev, "Invalid tunnel id %d\n", tunnel_id);
		return -EINVAL;
	}

	spin_lock_bh(&priv->lock);

	tunnel = &priv->tunnels[tunnel_id];
	tunnel->tx_if = netdev;

	spin_unlock_bh(&priv->lock);

	return 0;
}

/*!
 *@brief Add a session into tunnel
 *@param[in] dev: device
 *@param[in] tunnel_id: tunnel id
 *@param[in] session_id: session id
 *@param[in] act: action
 *@return Returns session id if one already exist, or error code otherwise
 */
static int vpn_add_session(struct device *dev, int tunnel_id,
			   struct mxl_vpn_ipsec_act *act)
{
	struct vpn_data *priv = dev_get_drvdata(dev);
	struct ipsec_act *sess_act;
	struct session_info *sess;
	struct tunnel_info *tunnel;
	int i, free_sess = IPSEC_TUN_SESS_MAX;

	if (tunnel_id < 0 || tunnel_id >= IPSEC_TUN_MAX) {
		dev_err(priv->dev, "Invalid tunnel id %d\n", tunnel_id);
		return -EINVAL;
	}

	spin_lock_bh(&priv->lock);

	tunnel = &priv->tunnels[tunnel_id];
	if (!tunnel->ctx[VPN_DIRECTION_OUT]) {
		spin_unlock_bh(&priv->lock);
		dev_err(priv->dev, "Tunnel %d does not exist\n", tunnel_id);
		return -EINVAL;
	}

	/* find an existing session, sessions are distinct by the actions
	 * types we set to them, thus, different actions, different sessions.
	 */
	for (i = 0; i < IPSEC_TUN_SESS_MAX; i++) {
		sess = &tunnel->session[i];
		if (!sess->on) {
			if (free_sess == IPSEC_TUN_SESS_MAX)
				free_sess = i;
			continue;
		}

		sess_act = &priv->genconf->sess_act_out[tunnel_id][i];
		if (sess_act->dw0_mask != act->dw0_mask)
			continue;
		if (sess_act->dw0_val != act->dw0_val)
			continue;
		if (sess_act->dw1_mask != act->dw1_mask)
			continue;
		if (sess_act->dw1_val != act->dw1_val)
			continue;
		if (!!sess_act->enq_qos != !!act->enq_qos)
			continue;

		spin_unlock_bh(&priv->lock);
		return i;
	}

	/* no free session found, return error */
	if (free_sess == IPSEC_TUN_SESS_MAX) {
		spin_unlock_bh(&priv->lock);
		dev_err(priv->dev, "Unable to find free session slot\n");
		return -ENOENT;
	}

	/* found a free session */
	sess = &tunnel->session[free_sess];
	if (sess->on) {
		spin_unlock_bh(&priv->lock);
		dev_err(priv->dev, "free session %u isn't free!!!\n",
			free_sess);
		return -EFAULT;
	}

	/* copy the new action to the session */
	sess_act = &priv->genconf->sess_act_out[tunnel_id][free_sess];
	sess_act->dw0_mask = act->dw0_mask;
	sess_act->dw0_val = act->dw0_val;
	sess_act->dw1_mask = act->dw1_mask;
	sess_act->dw1_val = act->dw1_val;
	sess_act->enq_qos = act->enq_qos ? 1 : 0;
	sess->on = true;
	update_tunnel_flag(priv, tunnel_id);
	spin_unlock_bh(&priv->lock);

	return free_sess;
}

/*!
 *@brief Update a session action
 *@param[in] dev: device
 *@param[in] tunnel_id: tunnel id
 *@param[in] session_id: session id
 *@param[in] act: action
 *@return Returns 0 on succeed
 */
static int vpn_update_session(struct device *dev, int tunnel_id, int session_id,
			      struct mxl_vpn_ipsec_act *act)
{
	struct vpn_data *priv = dev_get_drvdata(dev);
	struct ipsec_act *session_act;

	if (tunnel_id < 0 || tunnel_id >= IPSEC_TUN_MAX) {
		dev_err(priv->dev, "Invalid tunnel id %d\n", tunnel_id);
		return -EINVAL;
	}

	if (session_id < 0 || session_id >= IPSEC_TUN_SESS_MAX) {
		dev_err(priv->dev, "Invalid session id %d\n", session_id);
		return -EINVAL;
	}

	spin_lock_bh(&priv->lock);

	if (!priv->tunnels[tunnel_id].ctx[VPN_DIRECTION_OUT]) {
		dev_err(priv->dev, "Tunnel %d does not exist\n", tunnel_id);
		spin_unlock_bh(&priv->lock);
		return -EINVAL;
	}

	if (!priv->tunnels[tunnel_id].session[session_id].on) {
		dev_err(priv->dev, "Session %d:%d does not exist\n",
			tunnel_id, session_id);
		spin_unlock_bh(&priv->lock);
		return -EINVAL;
	}

	session_act = &priv->genconf->sess_act_out[tunnel_id][session_id];
	session_act->dw0_mask = act->dw0_mask;
	session_act->dw0_val = act->dw0_val;
	session_act->dw1_mask = act->dw1_mask;
	session_act->dw1_val = act->dw1_val;
	session_act->enq_qos = act->enq_qos ? 1 : 0;

	update_tunnel_flag(priv, tunnel_id);

	spin_unlock_bh(&priv->lock);
	return 0;
}

/*!
 *@brief Delete a session from tunnel
 *@param[in] dev: device
 *@param[in] tunnel_id: tunnel id
 *@param[in] session_id: session id
 *@return Returns 0 on succeed
 */
static int vpn_delete_session(struct device *dev, int tunnel_id, int session_id)
{
	struct vpn_data *priv = dev_get_drvdata(dev);
	struct session_info *session;

	if (tunnel_id < 0 || tunnel_id >= IPSEC_TUN_MAX) {
		dev_err(priv->dev, "Invalid tunnel id %d\n", tunnel_id);
		return -EINVAL;
	}

	if (session_id < 0 || session_id >= IPSEC_TUN_SESS_MAX) {
		dev_err(priv->dev, "Invalid session id %d\n", session_id);
		return -EINVAL;
	}

	spin_lock_bh(&priv->lock);

	session = &priv->tunnels[tunnel_id].session[session_id];

	if (!session->on) {
		dev_err(priv->dev, "Session %d:%d does not exist\n",
			tunnel_id, session_id);
		spin_unlock_bh(&priv->lock);
		return -EINVAL;
	}

	session->on = false;

	spin_unlock_bh(&priv->lock);
	return 0;
}

/* construct ipsec connection from xfrm param */
static int vpn_add_sa(struct vpn_data *priv, struct xfrm_state *x,
		      int tunnel_id, int dir)
{
	void *ctx;
	struct vpn_sa_params params = {0,};
	struct ipsec_info *info;
	struct tunnel_info *tunnel = &priv->tunnels[tunnel_id];
	struct dc_desc0_out_enc dc_desc_0;
	struct dc_desc1 dc_desc_1;
	struct ipsec_act *dwt;

	dev_dbg(priv->dev, "%s spi 0x%x auth %px enc %px aead %px dir %d\n",
		__func__, x->id.spi, x->aalg, x->ealg, x->aead, dir);

	/* setup params */
	params.direction = dir;
	params.spi = be32_to_cpu(x->id.spi); /* xfrm spi is big endian */
	if (x->aalg) {
		dev_dbg(priv->dev, "%s auth alg %s\n", __func__,
			x->aalg->alg_name);
		params.auth_algo = x->aalg->alg_name;
		params.authkey = x->aalg->alg_key;
		params.authkeylen = (x->aalg->alg_key_len + 7)
				 / 8; /* bit to bytes */
		params.icv_size = (x->aalg->alg_trunc_len + 7) / 8;
	}
	if (x->ealg) {
		dev_dbg(priv->dev, "%s enc alg %s\n", __func__,
			x->ealg->alg_name);
		params.enc_algo = x->ealg->alg_name;
		params.key = x->ealg->alg_key;
		params.key_len = (x->ealg->alg_key_len + 7)
				 / 8; /* bit to bytes */
		params.iv = NULL;
	}

	if (x->props.family == AF_INET6)
		params.ip_mode = VPN_IP_MODE_IPV6;
	else
		params.ip_mode = VPN_IP_MODE_IPV4;

	if (x->props.mode & XFRM_MODE_FLAG_TUNNEL)
		params.mode = VPN_MODE_TUNNEL;
	else
		params.mode = VPN_MODE_TRANSPORT;

	info = &priv->genconf->ipsec_info[tunnel_id];
	params.ctx_buffer = &priv->genconf->ctx[tunnel_id];
	params.token_buffer = &priv->genconf->acd_tmpl[tunnel_id];
	params.cdr_buffer = &info->cd_info;

	ctx = vpn_eip197_create_sa(priv, &params);
	if (!ctx) {
		dev_err(priv->dev, "fail to create crypto engine sa\n");
		return -EINVAL;
	}

	if (params.mode == VPN_MODE_TRANSPORT)
		info->mode = ESP_TR;
	else
		info->mode = ESP_TU;

	/* set context and token pointer */
	info->cd_info.dw4.acdlo = priv->sram_phys +
				  offsetof(struct genconf,
					   acd_tmpl[tunnel_id]);
	info->cd_info.dw8.ctxlo = priv->sram_phys +
				  offsetof(struct genconf,
					   ctx[tunnel_id]) +
				  0x2 /* large transform marker */;

	/* misc info */
	info->blk_size = params.pad_blk_size;
	info->cd_size = CD_SIZE;
	info->icv_len = params.icv_size;
	info->iv_len = params.iv_size;

	if (x->props.family == AF_INET6)
		info->ip_hlen = sizeof(struct ipv6hdr);
	else
		info->ip_hlen = sizeof(struct iphdr);

	if (dir)
		info->pad_en = 0;
	else
		info->pad_en = 1;

	/* token position info; this is used by firmware to manipulate token
	 * content
	 */
	info->ipcsum_instr_offset = params.ipcsum_offs;
	info->crypto_instr_offset = params.crypto_offs;
	info->pad_instr_offset = params.total_pad_offs;
	info->hash_pad_instr_offset = params.hash_pad_offs;
	info->msg_len_instr_offset = params.msg_len_offs;

	vpn_config_lookup(priv);

	/* Setup genconf actions */
	dwt = &priv->genconf->dwt[tunnel_id];

	dc_desc_0.all = 0;
	dc_desc_0.field.subif_offset = priv->subif >> 9;
	dc_desc_0.field.tunnel_id = tunnel_id;
	if (!dir)
		dc_desc_0.field.enc = 1;
	dwt->dw0_mask = ~(0xFFFF); /* clear subif, tunnel-id, enc */
	dwt->dw0_val = dc_desc_0.all;

	dc_desc_1.all = 0;
	dc_desc_1.field.port = priv->gpid;
	dwt->dw1_mask = ~(0xFF3F80); /* clear port, dev_qos, pmac, prel2 */
	dwt->dw1_val = dc_desc_1.all; /* set port to VPN GPID */

	/* with qos */
	dwt->enq_qos = 1;

	update_tunnel_flag(priv, tunnel_id);

	/* save to tunnel table */
	tunnel->spi = params.spi;
	tunnel->ctx[dir] = ctx;
	tunnel->id = tunnel_id;
	tunnel->mode = params.mode;
	tunnel->ip_mode = params.ip_mode;
	tunnel->iv_sz = params.iv_size;
	tunnel->family = x->props.family;

	/* save tunnel handle to xfrm */
	x->xso.offload_handle = (unsigned long)tunnel;

	dev_dbg(priv->dev,
		"ipsec buf constructed dir %d tunnel_id %d info %px:en %px\n",
		dir, tunnel_id, info,
		&priv->genconf->ipsec_flag);

	return 0;
}

/* look for existing context and destroy */
static void vpn_delete_sa(struct vpn_data *priv, struct xfrm_state *x,
			  int tunnel_id, int dir)
{
	struct tunnel_info *tunnel = &priv->tunnels[tunnel_id];
	struct ipsec_info *info;
	struct ctx *ctx2;
	struct tkn *tkn;
	u32 shift, clear;
	u32 *flag;
	int i;

	dev_dbg(priv->dev, "%s spi 0x%x dir %d\n", __func__, x->id.spi, dir);

	/* delete PPA session */
	for (i = 0; i < IPSEC_TUN_SESS_MAX; i++)
		tunnel->session[i].on = false;
#if IS_ENABLED(CONFIG_PPA)
	if (ppa_vpn_tunn_del_hook)
		ppa_vpn_tunn_del_hook(tunnel_id);
#endif

	info = &priv->genconf->ipsec_info[tunnel_id];
	ctx2 = &priv->genconf->ctx[tunnel_id];
	tkn = &priv->genconf->acd_tmpl[tunnel_id];
	flag = &priv->genconf->ipsec_flag;

	/* set disable flag bits */
	shift = tunnel_id * 2;
	clear = GENMASK(shift + 1, shift);
	*flag = (*flag & ~clear) | (TUN_DIS << shift);

	/* clear info, context, token.
	 * This needs to be done after flag status is disabled.
	 */
	wmb();
	udelay(priv->umt_period_us);
	memset(info, 0, sizeof(*info));
	memset(ctx2, 0, sizeof(*ctx2));
	memset(tkn, 0, sizeof(*tkn));

	/* destroy eip197 sa */
	vpn_eip197_delete_sa(priv, tunnel->ctx[dir]);
}

static int vpn_find_free_tunnel(struct vpn_data *priv, int spi, int dir)
{
	int i;

	/* look for existing tunnel with same spi */
	for (i = 0; i < IPSEC_TUN_MAX; i++) {
		if (priv->tunnels[i].spi == spi) {
			if (!priv->tunnels[i].ctx[dir])
				return i;

			dev_err(priv->dev,
				"Tunnel %d already exists for spi 0x%x\n",
				i, spi);
			return -EINVAL;
		}
	}

	/* look for free tunnel entry */
	for (i = 0; i < IPSEC_TUN_MAX; i++) {
		if (!priv->tunnels[i].ctx[0] && !priv->tunnels[i].ctx[1])
			return i;
	}

	return -ENOMEM;
}

int vpn_add_xfrm_sa(struct xfrm_state *x)
{
	struct mxl_vpn_ops *vpn = dp_get_vpn_ops(0);
	struct vpn_data *priv;
	enum mxl_vpn_direction dir;
	int ret;
	int tunnel_id;

	if (!vpn)
		return -EINVAL;

	priv = dev_get_drvdata(vpn->dev);

	spin_lock_bh(&priv->lock);

	if (x->xso.flags & XFRM_OFFLOAD_INBOUND)
		dir = VPN_DIRECTION_IN;
	else
		dir = VPN_DIRECTION_OUT;

	tunnel_id = vpn_find_free_tunnel(priv, be32_to_cpu(x->id.spi), dir);
	if (tunnel_id < 0) {
		dev_err(priv->dev, "Failed to get free tunnels\n");
		ret = -ENOMEM;
		goto exit;
	}

	ret = vpn_add_sa(priv, x, tunnel_id, dir);

	/* disable replay check in slow-path, as we cannot guarantee sync
	 * between accelerated vs non-accelerated path
	 */
	x->props.replay_window = 0;
	if (x->replay_esn)
		x->replay_esn->replay_window = 0;

exit:
	spin_unlock_bh(&priv->lock);
	return ret;
}

void vpn_delete_xfrm_sa(struct xfrm_state *x)
{
	struct mxl_vpn_ops *vpn = dp_get_vpn_ops(0);
	struct vpn_data *priv;
	enum mxl_vpn_direction dir;
	struct tunnel_info *tunnel;
	int i;

	if (!vpn)
		return;

	priv = dev_get_drvdata(vpn->dev);

	spin_lock_bh(&priv->lock);

	if (x->xso.flags & XFRM_OFFLOAD_INBOUND)
		dir = VPN_DIRECTION_IN;
	else
		dir = VPN_DIRECTION_OUT;

	for (i = 0; i < IPSEC_TUN_MAX; i++) {
		tunnel = &priv->tunnels[i];
		/* xfrm spi is big endian */
		if (tunnel->spi == be32_to_cpu(x->id.spi)) {
			vpn_delete_sa(priv, x, i, dir);
			tunnel->ctx[dir] = NULL;
			spin_unlock_bh(&priv->lock);
			return;
		}
	}

	spin_unlock_bh(&priv->lock);
	dev_err(priv->dev, "%s Unable to find spi entry\n", __func__);
}

static void skb_len_manipulate(struct vpn_data *priv, struct sk_buff *skb,
			       struct xfrm_state *x, int mtu)
{
	unsigned int *ext;
	u32 ext_id = priv->ext_id;

	dev_dbg(priv->dev, "%s skb len %d > mtu %d ext_id 0x%u\n",
		__func__, skb->len, mtu, ext_id);

	/* get vpn ext on the SKB */
	ext = mxl_skb_ext_add(skb, ext_id);
	if (!ext) {
		dev_err(priv->dev, "Failed to add vpn ext\n");
		return;
	}

	/* save payload size on the ext */
	if (x->props.mode == XFRM_MODE_TRANSPORT)
		*ext = skb->len - sizeof(struct iphdr);
	else
		*ext = skb->len;

	skb->len = mtu;
}

/* Map protocol to 4-bit enum */
static u8 vpn_map_next_header(u8 proto)
{
	switch (proto) {
	case IPPROTO_IPIP:
		return ESP_NH_IP;
	case IPPROTO_IPV6:
		return ESP_NH_IP6;
	case IPPROTO_UDP:
		return ESP_NH_UDP;
	case IPPROTO_TCP:
		return ESP_NH_TCP;
	case IPPROTO_GRE:
		return ESP_NH_GRE;
	case IPPROTO_ICMP:
		return ESP_NH_ICMP;
	case IPPROTO_ICMPV6:
		return ESP_NH_ICMP6;
	case IPPROTO_L2TP:
		return ESP_NH_L2TP;
	}

	return ESP_NH_MAX;
}

static bool vpn_is_transport_proto_supported(struct vpn_data *priv,
					     struct sk_buff *skb)
{
	u8 proto;
	struct iphdr *iph = ip_hdr(skb);
#if IS_ENABLED(CONFIG_IPV6)
	struct ipv6hdr *ip6h = ipv6_hdr(skb);
	__be16 frag_off;
#endif

	if (iph && iph->version == 4) {
		proto = iph->protocol;
#if IS_ENABLED(CONFIG_IPV6)
	} else if (ip6h && ip6h->version == 6) {
		proto = ip6h->nexthdr;
		if (ipv6_skip_exthdr(skb, sizeof(*ip6h), &proto,
				     &frag_off) < 0) {
			dev_dbg(priv->dev, "invalid ipv6 ext hdr\n");
			return false;
		}
#endif
	} else {
		return false;
	}

	if (vpn_map_next_header(proto) == ESP_NH_MAX) {
		dev_dbg(priv->dev, "prot %d is not supported\n", proto);
		return false;
	}

	return true;
}

bool vpn_xfrm_offload_ok(struct sk_buff *skb, struct xfrm_state *x)
{
	struct mxl_vpn_ops *vpn = dp_get_vpn_ops(0);
	struct vpn_data *priv;
	struct xfrm_dst *xdst = (struct xfrm_dst *)skb_dst(skb);
	int mtu = xfrm_state_mtu(x, xdst->child_mtu_cached);
	struct iphdr *iph;

	if (!vpn)
		return -EINVAL;

	priv = dev_get_drvdata(vpn->dev);

	dev_dbg(priv->dev, "%s skb len %d mtu %d\n", __func__, skb->len, mtu);

	if (x->props.mode == XFRM_MODE_TRANSPORT) {
		if (!vpn_is_transport_proto_supported(priv, skb))
			return false;
	}

	if (skb->len > mtu) {
		iph = ip_hdr(skb);
		if (iph->version != 4)
			return false;

		/* if don't fragment flag is set, avoid manipulation */
		if (iph->frag_off & htons(IP_DF))
			return false;

		skb_len_manipulate(priv, skb, x, mtu);
	}

	return true;
}

static int vpn_proto_to_next_header(struct device *dev, u8 proto,
				    u8 *next_header)
{
	u8 vpn_nh;

	if (!next_header)
		return -EINVAL;

	vpn_nh = vpn_map_next_header(proto);
	if (vpn_nh == ESP_NH_MAX) {
		dev_err(dev, "Unsupported protocol %d\n", proto);
		return -EPROTONOSUPPORT;
	}

	*next_header = vpn_nh;
	return 0;
}

static void vpn_netdev_setup(struct net_device *dev)
{
	dev->features = 0;
	dev->netdev_ops = &vpn_net_ops;
	/* set high MTU so PP won't fragment the packets */
	dev->mtu = ETH_MAX_MTU - ETH_HLEN;
}

/* Translate phys addr to vpn firmware cqm addr.
 * This is required by VPN firmware to allow common
 * binary for linux/non-linux platform.
 */
static u32 phys_to_vpn_cqm(void *paddr)
{
	const int tag = 0xD00000;
	const int mask = BIT(20) - 1;

	return ((u64)paddr & mask) | tag;
}

static int get_ip_hdr_size(struct sk_buff *skb, u32 offset)
{
	struct iphdr *iph, _iph;
#if IS_ENABLED(CONFIG_IPV6)
	struct ipv6hdr *ip6h, _ip6h;

	ip6h = skb_header_pointer(skb, offset, sizeof(*ip6h),
				  &_ip6h);
	if (!ip6h || ip6h->version != 6)
		goto ipv4;

	return sizeof(*ip6h);
#endif

ipv4:
	iph = skb_header_pointer(skb, offset, sizeof(*iph),
				 &_iph);
	if (!iph || iph->version != 4)
		return -EINVAL;

	return iph->ihl * sizeof(u32);
}

static int get_inner_ip_size(struct sk_buff *skb, u32 offset)
{
	struct iphdr *iph, _iph;
#if IS_ENABLED(CONFIG_IPV6)
	struct ipv6hdr *ip6h, _ip6h;

	ip6h = skb_header_pointer(skb, offset, sizeof(*ip6h),
				  &_ip6h);
	if (!ip6h || ip6h->version != 6)
		goto ipv4;

	return ntohs(ip6h->payload_len) + sizeof(*ip6h);
#endif

ipv4:
	iph = skb_header_pointer(skb, offset, sizeof(*iph),
				 &_iph);
	if (!iph || iph->version != 4)
		return -EINVAL;

	return ntohs(iph->tot_len);
}

static u8 get_next_header(struct sk_buff *skb, u32 offset)
{
	struct iphdr *iph, _iph;
#if IS_ENABLED(CONFIG_IPV6)
	struct ipv6hdr *ip6h, _ip6h;

	ip6h = skb_header_pointer(skb, offset, sizeof(*ip6h),
				  &_ip6h);
	if (!ip6h || ip6h->version != 6)
		goto ipv4;

	return ESP_NH_IP6;
#endif

ipv4:
	iph = skb_header_pointer(skb, offset, sizeof(*iph),
				 &_iph);
	if (!iph || iph->version != 4)
		return ESP_NH_MAX;

	return ESP_NH_IP;
}

static int vpn_l2_to_l3(struct sk_buff *skb, struct net_device *dev)
{
	__be16 proto;
	struct vlan_hdr *vlanhdr;

	proto = ntohs(eth_type_trans(skb, dev));

redo_parse:
	switch (proto) {
	case ETH_P_8021Q:
	case ETH_P_8021AD:
		vlanhdr = (struct vlan_hdr *)(skb->data);
		proto = ntohs(vlanhdr->h_vlan_encapsulated_proto);
		skb_pull(skb, VLAN_HLEN);
		goto redo_parse;
	case ETH_P_PPP_SES:
		skb_pull(skb, PPPOE_SES_HLEN);
		break;
	}

	skb_reset_network_header(skb);
	return 0;
}

static int vpn_process_v4_outbound(struct sk_buff *skb, struct net_device *dev)
{
	struct iphdr *iph;
	struct flowi4 fl4 = {0};
	struct rtable *rt;
	int ret;
	u32 hlen;

	iph = ip_hdr(skb);

	hlen = iph->ihl * sizeof(u32);
	fl4.flowi4_oif = dev->ifindex;
	fl4.daddr = iph->daddr;
	fl4.saddr = iph->saddr;
	rt = ip_route_output_key(dev_net(dev), &fl4);
	if (IS_ERR(rt)) {
		consume_skb(skb);
		dev->stats.tx_errors++;
		return -EIO;
	}

	skb->transport_header = skb->network_header + hlen;
	skb_dst_set(skb, &rt->dst);

	ret = ip_local_out(dev_net(dev), skb->sk, skb);
	if (unlikely(net_xmit_eval(ret)))
		dev->stats.tx_errors++;

	return ret;
}

#if IS_ENABLED(CONFIG_IPV6)
static int vpn_process_v6_outbound(struct sk_buff *skb, struct net_device *dev)
{
	struct ipv6hdr *ip6h;
	struct flowi6 fl6;
	struct dst_entry *dst;
	__be16 frag_off;
	u8 nexthdr;
	int ret;
	int hlen;

	ip6h = ipv6_hdr(skb);
	nexthdr = ip6h->nexthdr;
	if (ipv6_ext_hdr(nexthdr)) {
		hlen = ipv6_skip_exthdr(skb, sizeof(*ip6h), &nexthdr,
					&frag_off);
		if (hlen < 0) {
			consume_skb(skb);
			dev->stats.tx_errors++;
			return -EIO;
		}
	} else {
		hlen = sizeof(*ip6h);
	}

	fl6.flowi6_oif = dev->ifindex;
	fl6.daddr = ip6h->daddr;
	fl6.saddr = ip6h->saddr;
	dst = ip6_route_output(dev_net(dev), skb->sk, &fl6);
	if (dst->error) {
		dst_release(dst);
		consume_skb(skb);
		dev->stats.tx_errors++;
		return -EIO;
	}

	skb->transport_header = skb->network_header + hlen;
	skb_dst_set(skb, dst);

	ret = ip6_local_out(dev_net(dev), skb->sk, skb);
	if (unlikely(net_xmit_eval(ret)))
		dev->stats.tx_errors++;

	return ret;
}
#endif

static int vpn_process_outbound(struct vpn_data *priv, struct sk_buff *skb,
				struct net_device *dev)
{
	struct iphdr *iph;
#if IS_ENABLED(CONFIG_IPV6)
	struct ipv6hdr *ip6h;
#endif

	/* Reset the skb offsets */
	skb_reset_mac_header(skb);
	skb_reset_network_header(skb);
	skb_reset_transport_header(skb);
	skb_reset_mac_len(skb);

	/* Set skb->dev to egress netdev (WAN) */
	skb->dev = dev;
	skb->pkt_type = PACKET_OUTGOING;

	if (vpn_l2_to_l3(skb, dev))
		goto err;

	IPCB(skb)->flags |= IPSKB_REROUTED;

	/* Send the packet to the IP stack out.
	 * This will update the IP length and check if frag is needed
	 */
	iph = ip_hdr(skb);
	if (likely(iph->version == 4))
		return vpn_process_v4_outbound(skb, dev);

#if IS_ENABLED(CONFIG_IPV6)
	ip6h = ipv6_hdr(skb);
	if (likely(ip6h->version == 6))
		return vpn_process_v6_outbound(skb, dev);
#endif

err:
	dev_warn_ratelimited(priv->dev, "Dropped invalid outbound packet\n");
	consume_skb(skb);
	return -EIO;
}

/**
 * @brief Create xfrm secpath and offload for the packet to mark
 *        that the packet was already offloaded and decrypted
 *        by HW
 * @param skb the packet
 * @param spi tunnel's spi
 * @param nexthdr next header protocol after the ESP
 * @param family tunnel family, AF_INET, AF_INET6, etc...
 * @return int 0 on success, 0 otherwise
 */
static int vpn_rx_dec_create_secpath(struct vpn_data *priv, struct sk_buff *skb,
				     u32 spi, u8 nexthdr, u16 family)
{
	struct xfrm_state *x;
	struct xfrm_offload *xo;
	struct sec_path *sp;

	x = xfrm_state_lookup_byspi(dev_net(skb->dev), spi, family);
	if (!x) {
		dev_err(priv->dev, "cannot find xfrm state\n");
		return -ENOENT;
	}

	sp = secpath_set(skb);
	if (!sp) {
		dev_err(priv->dev, "failed to create secpath\n");
		return -EINVAL;
	}

	sp->xvec[sp->len++] = x;
	sp->olen++;
	xo = xfrm_offload(skb);
	if (!xo)
		return -EINVAL;

	xo->flags = CRYPTO_DONE;
	/* @todo: get error code from FW and set the right one so xfrm
	 * will be able to follow IPSec policies
	 */
	xo->status = CRYPTO_SUCCESS;
	xo->flags |= XFRM_ESP_NO_TRAILER;
	xo->proto = nexthdr;

	return 0;
}

/* DP RX callback.
 * This implements below logic at slowpath:
 * - Outbound
 *   - Stage 1 (2nd pass):
 *     - Call PPA outbound hook
 * - Inbound
 *   - Stage 2:
 *     - Call PPA inbound hook
 */
static int vpn_dp_rx_fn(struct net_device *rxif, struct net_device *txif,
			struct sk_buff *skb, int len)
{
	struct vpn_net_data *net_data = netdev_priv(rxif);
	struct vpn_data *priv = net_data->priv;
	struct tunnel_info *tunnel;
	struct dc_desc0 *desc_0;
	int tunnel_id;
	int ret;

	dev_dbg(priv->dev, "%s enter\n", __func__);

	desc_0 = (struct dc_desc0 *)&skb->DW0;
	if (desc_0->o_e.field.enc)
		/* outbound: retrieve tunnel-id from dw0[7:0] */
		tunnel_id = desc_0->o_e.field.tunnel_id;
	else
		/* inbound: retrieve tunnel-id from dw0[24:20] */
		tunnel_id = desc_0->o_d.field.tunnel_id;

	tunnel = &priv->tunnels[tunnel_id];

#if defined(DEBUG)
	print_hex_dump(KERN_DEBUG, "skb dump: ", DUMP_PREFIX_OFFSET,
		       16, 1, skb->data, skb->len, false);
#endif

	if (desc_0->o_e.field.enc) {
		if (!tunnel->rx_if) {
			dev_err(priv->dev, "No valid rx interface\n");
			ret = -EIO;
			goto err_consume;
		}

		ret = vpn_process_outbound(priv, skb, tunnel->rx_if);
	} else {
		if (!tunnel->tx_if) {
			dev_err(priv->dev, "No valid tx interface\n");
			ret = -EIO;
			goto err_consume;
		}
#if IS_ENABLED(CONFIG_PPA)
		if (!ppa_vpn_ig_lrn_hook) {
			dev_err(priv->dev, "No PPA hook registered for ingress learning\n");
			ret = -EIO;
			goto err_consume;
		}
		ret = ppa_vpn_ig_lrn_hook(skb, tunnel->tx_if, tunnel_id);
		if (ret) {
			dev_err(priv->dev, "Failed at ppa learn hook %d\n",
				ret);
			goto err_consume;
		}
#endif

		/* create xfrm sec path */
		ret = vpn_rx_dec_create_secpath(priv, skb, htonl(tunnel->spi),
						desc_0->o_d.field.nexthdr,
						tunnel->family);
		if (ret) {
			dev_err(priv->dev, "Failed to create secpath, err %d\n",
				ret);
			goto err_consume;
		}

		skb->protocol = eth_type_trans(skb, tunnel->tx_if);
		skb->ip_summed = CHECKSUM_UNNECESSARY;
		/* @TBD: use 'napi_gro_receive' as specified by the kernel
		 * documentation for IPSec offloading
		 */
		netif_rx(skb);
	}

	return ret;

err_consume:
	consume_skb(skb);
	return ret;
}

static int vpn_tunnel_trim(struct vpn_data *priv, struct sk_buff *skb,
			   u32 offset, u32 iv_len, u8 *p_next_hdr)
{
	int outer_ip_hdr_size;
	int inner_ip_size;
	u32 trim_size;
	u8 next_hdr;

	trim_size = offset; /* MAC/PPPoE header */
	outer_ip_hdr_size = get_ip_hdr_size(skb, trim_size);
	if (outer_ip_hdr_size < 0) {
		dev_err(priv->dev, "Failed to get inner IP size\n");
		return DP_TX_FN_DROPPED;
	}
	trim_size += outer_ip_hdr_size +
		     sizeof(struct ip_esp_hdr) +
		     iv_len;

	/* inner IP */
	inner_ip_size = get_inner_ip_size(skb, trim_size);
	if (inner_ip_size < 0) {
		dev_err(priv->dev, "Failed to get inner IP size\n");
		return DP_TX_FN_DROPPED;
	}

	/* next header */
	next_hdr = get_next_header(skb, trim_size);
	if (next_hdr == ESP_NH_MAX) {
		dev_err(priv->dev, "Unsupported inner packet\n");
		return DP_TX_FN_DROPPED;
	}

	trim_size += inner_ip_size;

	dev_dbg(priv->dev, "trimmed skb from %d to %d\n", skb->len, trim_size);
	skb_linearize(skb);
	skb_trim(skb, trim_size);
	*p_next_hdr = next_hdr;

	return 0;
}

static int vpn_transport_trim(struct vpn_data *priv, struct sk_buff *skb,
			      u32 iv_len, u32 icv_len, u8 *p_next_hdr)
{
	struct xfrm_offload *xo = xfrm_offload(skb);
	u32 trim_size;
	u8 pad_len;
	u8 next_hdr;

	if (!xo)
		return -EINVAL;

	skb_linearize(skb);

	/* next header */
	next_hdr = vpn_map_next_header(xo->proto);
	if (next_hdr == ESP_NH_MAX) {
		dev_err(priv->dev, "Unsupported protocol\n");
		return DP_TX_FN_DROPPED;
	}

	pad_len = *(skb->data + skb->len - icv_len - 2);
	trim_size = skb->len - pad_len - 2 /* nh */ - icv_len;

	dev_dbg(priv->dev, "trimmed skb from %d to %d\n", skb->len, trim_size);
	skb_trim(skb, trim_size);
	*p_next_hdr = next_hdr;

	return 0;
}

/* DP TX callback.
 * This implements below logic at slowpath:
 * - Outbound
 *   - Stage 1:
 *     - Store egress netdev
 *     - Setup descriptor/GPID (redirect to VPN firmware)
 *     - Forward back to next dp tx processors
 *   - Stage 2:
 *     - Same as stage 1
 * - Inbound
 *   - N/A
 */
static int vpn_dp_tx_fn(struct sk_buff *skb, struct dp_tx_common *cmn,
			void *p)
{
	struct vpn_data *priv = (struct vpn_data *)p;
	struct ipsec_info *tunnel_info;
	struct dc_desc0_enc *desc_0;
	struct dma_rx_desc_1 *desc_1;
	struct tunnel_info *tunnel;
	struct xfrm_offload *xo;
	struct xfrm_state *x;
	u8 next_hdr;
	u32 ip_offset;
	int ret;
	u32 flags;

	xo = xfrm_offload(skb);

	if (likely(!xo || skb_is_gso(skb)))
		return DP_TX_FN_CONTINUE;

	if (cmn->flags & DP_TX_BYPASS_FLOW) {
		dev_dbg(priv->dev, "Bypass flow set, ignoring\n");
		return DP_TX_FN_CONTINUE;
	}

	if (xo->flags & CRYPTO_DONE) {
		secpath_reset(skb);
		return DP_TX_FN_CONTINUE;
	}

	dev_dbg(priv->dev, "%s enter skb %px xfrm %px ip offset %d\n", __func__,
		skb, xo, skb_network_offset(skb));

	x = xfrm_input_state(skb);
	if (!x) {
		dev_err(priv->dev, "Failed to get xfrm input state\n");
		return DP_TX_FN_DROPPED;
	}

	tunnel = (struct tunnel_info *)x->xso.offload_handle;
	tunnel_info = &priv->genconf->ipsec_info[tunnel->id];
	ip_offset = skb_network_offset(skb);
	if (skb_protocol(skb, true) == htons(ETH_P_PPP_SES))
		ip_offset += PPPOE_SES_HLEN;

	/* At this point, linux/xfrm already construct the packet in ESP format
	 * (including outer header and trailer, but not encrypted).
	 * Since firmware/eip197 need to know the original data length (instead
	 * of padded length), we trim the skb back to its original length.
	 */
	if (x->props.mode & XFRM_MODE_FLAG_TUNNEL) {
		ret = vpn_tunnel_trim(priv, skb, ip_offset, tunnel_info->iv_len,
				      &next_hdr);
		if (ret)
			return ret;

		ip_offset += get_ip_hdr_size(skb, ip_offset);
	} else { /* transport mode */
		ret = vpn_transport_trim(priv, skb, tunnel_info->iv_len,
					 tunnel_info->icv_len, &next_hdr);
		if (ret)
			return ret;

		ip_offset += get_ip_hdr_size(skb, ip_offset);
	}

#if defined(DEBUG)
	print_hex_dump(KERN_DEBUG, "skb dump: ", DUMP_PREFIX_OFFSET,
		       16, 1, skb->data, skb->len, false);
#endif

	/* Setup descriptor for encryption */
	desc_0 = (struct dc_desc0_enc *)&skb->DW0;
	desc_1 = (struct dma_tx_desc_1 *)&skb->DW1;
	desc_0->all = 0;
	desc_0->field.enc = 1;
	desc_0->field.ipoffset = ip_offset;
	desc_0->field.subif = priv->subif >> 9;
	desc_0->field.vp_sess = IPSEC_TUN_SESS_MAX;
	desc_0->field.next_hdr = next_hdr;
	desc_0->field.tunnel_id = tunnel->id;

	/* Redirect to vpn firmware */
	desc_1->field.ep = priv->gpid;
	desc_1->field.redir = 1; /* egflag */
	vpn_subif->subif = priv->subif;
	vpn_subif->port_id = priv->dp_port;

	/* mark bypass flag */
	flags = DP_TX_BYPASS_FLOW;

	/* Store egress netdev */
	tunnel->rx_if = x->xso.dev;

	/* call dp_xmit back */
	dev_dbg(priv->dev, "Redirecting to vpn firmware\n");
	ret = dp_xmit(priv->netdev, vpn_subif, skb, skb->len, flags);
	if (ret) {
		dev_err(priv->dev, "Failed to dp_xmit\n");
		return DP_TX_FN_DROPPED;
	}

	return DP_TX_FN_CONSUMED;
}

static dp_cb_t vpn_dp_cb = {
	.rx_fn = vpn_dp_rx_fn,
};

static int vpn_init_netdev(struct vpn_data *priv)
{
	struct vpn_net_data *data;
	struct dp_spl_cfg *conn = NULL;
	int ret;

	priv->netdev = alloc_netdev(sizeof(struct vpn_net_data),
				    "mxl_vpn", NET_NAME_UNKNOWN,
				    vpn_netdev_setup);
	if (!priv->netdev)
		return -ENOMEM;

	data = netdev_priv(priv->netdev);
	data->priv = priv;

	ret = register_netdev(priv->netdev);
	if (ret) {
		dev_err(priv->dev, "Failed to register netdev\n");
		ret = -EIO;
		goto error;
	}

	priv->ext_id = mxl_skb_ext_register("mxl_vpn", sizeof(unsigned int));
	if (priv->ext_id == MXL_SKB_EXT_INVALID) {
		dev_err(priv->dev, "Failed to register mxl skb ext\n");
		ret = -ENOMEM;
		goto error;
	}

	conn = devm_kzalloc(priv->dev, sizeof(*conn), GFP_KERNEL);
	if (!conn) {
		ret = -ENOMEM;
		goto error;
	}

	conn->type = DP_SPL_VPNA;
	conn->dp_cb = &vpn_dp_cb;
	conn->dev = priv->netdev;
	conn->f_subif = 1;
	conn->f_gpid = 1;
	conn->f_hostif = 1;
	conn->umt[0].ctl.dst_addr_cnt = 2;
	conn->umt[0].ctl.dst[0] = (dma_addr_t)(priv->sram_phys +
				 priv->fw_hdr->genconf_offset +
				 offsetof(struct genconf, vpn_umt_cnt_rx));
	conn->umt[0].ctl.dst[1] = conn->umt[0].ctl.dst[0] + 4;
	conn->umt[0].ctl.msg_interval = priv->umt_period_us;
	conn->umt[0].ctl.msg_mode = UMT_MSG_SELFCNT;
	conn->umt[0].ctl.cnt_mode = priv->soc_data->cnt_mode;
	conn->umt[0].ctl.sw_msg = UMT_MSG0_MSG1;
	conn->umt[0].ctl.enable = 1;
	conn->umt[0].ctl.fflag = UMT_NOT_SND_ZERO_CNT;

	ret = dp_spl_conn(0, conn);
	if (ret != DP_SUCCESS) {
		dev_err(priv->dev, "Error configuring DP SPL %d\n", ret);
		goto error;
	}

	dp_register_tx(DP_TX_VPNA, vpn_dp_tx_fn, priv);

	/* pass igp/egp port info to fw */
	priv->genconf->dc_deq_base = phys_to_vpn_cqm(conn->egp[0].egp_paddr);
	priv->genconf->dc_deq_size = conn->egp[0].egp_ring_size;
	priv->genconf->dc_buf_rtn_base =
		phys_to_vpn_cqm(conn->egp[0].free_paddr);
	priv->genconf->dc_buf_rtn_size = conn->egp[0].egp_ring_size;

	priv->spl_id = conn->spl_id;
	priv->egp_id = conn->egp[0].egp_id;
	priv->qid = conn->egp[0].qid;
	priv->gpid = conn->gpid;
	priv->subif = conn->subif;
	priv->dp_port = conn->dp_port;
	priv->genconf->dc_enq_size[1] = conn->igp[0].igp_ring_size;
	priv->genconf->dc_enq_base[1] = phys_to_vpn_cqm(conn->igp[0].igp_paddr);
	priv->genconf->umt_txin_enable = 1;

	dev_dbg(priv->dev, "egp_id 0x%x qid 0x%x spl_id 0x%x gpid 0x%x\n",
		conn->egp[0].egp_id, conn->egp[0].qid, conn->spl_id,
		conn->gpid);

	devm_kfree(priv->dev, conn);
	return 0;

error:
	if (conn)
		devm_kfree(priv->dev, conn);

	if (priv->netdev) {
		unregister_netdev(priv->netdev);
		free_netdev(priv->netdev);
		priv->netdev = NULL;
	}
	return ret;
}

static int vpn_init_firewall(struct vpn_data *priv)
{
	struct fw_nb_apply_deferred_data def_data;
	struct fw_nb_resolve_data res_data;
	struct device_node *np;
	int ret;

	priv->sai = -1;
	np = of_parse_phandle(priv->dev->of_node, "firewall-domains", 0);
	if (np)
		of_property_read_s32(np, "sai", &priv->sai);

	res_data.dev = priv->dev;
	res_data.pseudo_addr = (((u64)NOC_FW_SPECIAL_REGION_DYN) << 32) |
				NOC_FW_DYN_VPNFW_START;
	res_data.resolve_addr = priv->sram_phys;
	ret = mxl_fw_notifier_blocking_chain(NOC_FW_EVENT_RESOLVE_ADDR,
					     &res_data);
	if (notifier_to_errno(ret)) {
		dev_err(priv->dev, "Failed resolving firewall start addr\n");
		return notifier_to_errno(ret);
	}

	res_data.pseudo_addr = (((u64)NOC_FW_SPECIAL_REGION_DYN << 32)) |
				NOC_FW_DYN_VPNFW_END;
	res_data.resolve_addr = priv->sram_phys +
				gen_pool_size(priv->sram_pool) - 1;
	ret = mxl_fw_notifier_blocking_chain(NOC_FW_EVENT_RESOLVE_ADDR,
					     &res_data);
	if (notifier_to_errno(ret)) {
		dev_err(priv->dev, "Failed resolving firewall end addr\n");
		return notifier_to_errno(ret);
	}

	def_data.dev = priv->dev;
	ret = mxl_fw_notifier_blocking_chain(NOC_FW_EVENT_APPLY_DEFE_RULE,
					     &def_data);
	if (notifier_to_errno(ret)) {
		dev_err(priv->dev, "Failed applying firewall deferred rule\n");
		return notifier_to_errno(ret);
	}

	return 0;
}

static int vpn_enable_txin_update(struct vpn_data *priv,
				  struct dp_umt_port *umt, int dpid,
				  enum vpn_umt_type type)
{
	struct umt_update_info *info;
	u32 interval;
	int i, j;
	u32 addr;
	u8 port;

	/* only apply in incremental mode */
	if (umt->ctl.cnt_mode != UMT_CNT_INC)
		return -EINVAL;

	switch (type) {
	case VPN_UMT_TXIN:
		if (umt->ctl.dst_addr_cnt != 2 && umt->ctl.dst_addr_cnt != 4) {
			dev_err(priv->dev, "Invalid umt txin dst addr cnt %d\n",
				umt->ctl.dst_addr_cnt);
			return -EINVAL;
		}
		addr = umt->ctl.dst[1] /* TXIN offset */;
		port = umt->res.cqm_dq_pid;
		break;
	case VPN_UMT_RXOUT:
		if (umt->ctl.dst_addr_cnt != 4) {
			dev_err(priv->dev, "Invalid umt rxout dst addr cnt %d\n",
				umt->ctl.dst_addr_cnt);
			return -EINVAL;
		}
		addr = umt->ctl.dst[2] /* RXOUT offset */;
		port = umt->res.cqm_enq_pid;
		break;
	default:
		return -EINVAL;
	}

	/* look for existing entry with same port (shared umt) */
	for (i = 0; i < MAX_ARC_TIMER; i++) {
		for (j = 0; j < MAX_PORT_PER_TIMER; j++) {
			info = &priv->genconf->umt_txin[i].info[j];
			if (info->port == port && info->type == type) {
				dev_dbg(priv->dev, "Duplicated umt txin entry\n");
				set_bit(dpid, &priv->umt_txin_dpid_mask[i][j]);
				return -EEXIST;
			}
		}
	}

	interval = umt->ctl.msg_interval >> umt->ctl.msg_interval_div;

	/* look for existing timer with same period */
	for (i = 0; i < MAX_ARC_TIMER; i++) {
		if (priv->genconf->umt_txin[i].period == interval)
			break;
	}

	if (i < MAX_ARC_TIMER)
		goto search_slot;

	/* look for unused timer */
	for (i = 0; i < MAX_ARC_TIMER; i++) {
		if (!priv->genconf->umt_txin[i].period)
			break;
	}
	if (i >= MAX_ARC_TIMER) {
		dev_err(priv->dev, "Failed to find valid timer\n");
		return -EINVAL;
	}

search_slot:
	for (j = 0; j < MAX_PORT_PER_TIMER; j++) {
		if (!priv->genconf->umt_txin[i].info[j].addr)
			break;
	}
	if (j >= MAX_PORT_PER_TIMER) {
		dev_err(priv->dev, "Timer %d has no free slot\n", i);
		return -EINVAL;
	}

	priv->genconf->umt_txin[i].period = interval;
	info = &priv->genconf->umt_txin[i].info[j];
	info->addr = addr;
	info->port = port;
	info->type = type;
	set_bit(dpid, &priv->umt_txin_dpid_mask[i][j]);

	priv->genconf->a2e_msg = INT_TXIN_UPDATE;
	regmap_update_bits(priv->syscfg, ATOM2EM6, 1, 1);
	return 0;
}

static int vpn_disable_txin_update(struct vpn_data *priv, int dpid)
{
	struct umt_update_info *info;
	int i, j;
	bool enable;

	for (i = 0; i < MAX_ARC_TIMER; i++) {
		enable = false;

		for (j = 0; j < MAX_PORT_PER_TIMER; j++) {
			info = &priv->genconf->umt_txin[i].info[j];

			clear_bit(dpid, &priv->umt_txin_dpid_mask[i][j]);
			if (!priv->umt_txin_dpid_mask[i][j]) {
				info->addr = 0;
				info->port = 0;
			}

			if (info->addr)
				enable = true;
		}

		/* disable the timer if no one is using */
		if (!enable)
			priv->genconf->umt_txin[i].period = 0;
	}

	priv->genconf->a2e_msg = INT_TXIN_UPDATE;
	regmap_update_bits(priv->syscfg, ATOM2EM6, 1, 1);
	return 0;
}

static int32_t vpn_dp_register_cb(struct dp_event_info *info)
{
	struct vpn_data *priv = info->data;
	struct dp_dev_data *dev_data;
	struct dp_umt_port *umt;
	enum vpn_umt_type type;

	if (priv->soc_data->soc == LGM_C_SOC) {
		dev_dbg(priv->dev, "No VPN workaround for LGM-C\n");
		return 0;
	}

	switch (info->type) {
	case DP_EVENT_REGISTER_DEV:
		dev_data = info->reg_dev_info.dev_data;
		if (!dev_data) {
			dev_warn(priv->dev,
				 "Ignoring event due to invalid data\n");
			return 0;
		}

		if (info->alloc_flags & DP_F_DOCSIS) {
			/* for docsis this feature is only used when umt
			 * hardware is disabled via f_not_alloc.
			 */
			if (!dev_data->umt_dc[2].f_not_alloc)
				return 0;

			umt = &dev_data->umt[2];
			type = VPN_UMT_RXOUT;
		} else if (info->alloc_flags & DP_F_ACA) {
			umt = &dev_data->umt[0];
			type = VPN_UMT_TXIN;
		} else {
			/* we only care about DC/Docsis ports */
			return 0;
		}

		dev_dbg(priv->dev,
			"umt_txin %s addr %llx,%llx,%llx,%llx itrvl %u div %u dq %d eq %d\n",
			info->reg_dev_info.dev->name, umt->ctl.dst[0],
			umt->ctl.dst[1], umt->ctl.dst[2], umt->ctl.dst[3],
			umt->ctl.msg_interval, umt->ctl.msg_interval_div,
			umt->res.cqm_dq_pid, umt->res.cqm_enq_pid);

		vpn_enable_txin_update(priv, umt, info->reg_dev_info.dpid, type);
		break;
	case DP_EVENT_DE_REGISTER_DEV:
		dev_dbg(priv->dev, "name %s dpid %d\n",
			info->reg_dev_info.dev->name, info->reg_dev_info.dpid);
		vpn_disable_txin_update(priv, info->reg_dev_info.dpid);
		break;
	default:
		return 0;
	}

	return 0;
}

static int vpn_dp_register_event(struct vpn_data *priv, u32 flag)
{
	struct dp_event event_info = {0};
	int ret;

	event_info.owner = DP_EVENT_OWNER_OTHERS;
	event_info.type = DP_EVENT_REGISTER_DEV | DP_EVENT_DE_REGISTER_DEV;
	event_info.dp_event_cb = vpn_dp_register_cb;
	event_info.data = priv;
	ret = dp_register_event_cb(&event_info, flag);
	if (ret != DP_SUCCESS) {
		dev_err(priv->dev, "Failed to register dp event\n");
		return ret;
	}

	return 0;
}

static void vpn_cleanup(struct vpn_data *priv)
{
	struct dp_spl_cfg *conn;
	struct mxl_fw_dl_data data = {0};

	vpn_dp_register_event(priv, DP_F_DEREGISTER);

	/* unregister dp ops */
	dp_register_ops(0, DP_OPS_VPN, NULL);

	/* disable ARC */
	if (priv->syscfg)
		regmap_update_bits(priv->syscfg, ARC_EM6_CR, 1, 0);
	if (priv->rst)
		reset_control_assert(priv->rst);

#if defined(CONFIG_DEBUG_FS)
	if (priv->dbg_work.work.func)
		flush_delayed_work(&priv->dbg_work);
#endif

	if (priv->spl_id) {
		conn = devm_kzalloc(priv->dev, sizeof(*conn), GFP_KERNEL);
		if (conn) {
			conn->flag = DP_F_DEREGISTER;
			conn->type = DP_SPL_VPNA;
			conn->spl_id = priv->spl_id;
			dp_spl_conn(0, conn);
			devm_kfree(priv->dev, conn);
		}
	}

	if (priv->netdev) {
		unregister_netdev(priv->netdev);
		free_netdev(priv->netdev);
	}

#if defined(CONFIG_DEBUG_FS)
	debugfs_remove_recursive(priv->debugfs);
#endif

	if (!IS_ERR(priv->clk))
		clk_disable_unprepare(priv->clk);

	if (priv->fw) {
		data.dev = priv->dev;
		data.sai = priv->sai;
		mxl_soc_release_firmware(priv->fw, priv->dev, NULL, 0, &data);
	}

	if (priv->ddr_virt)
		mxl_soc_free_firmware_buf(priv->dev, priv->ddr_virt,
					  priv->ddr_size, &data);

	if (priv->sram_virt)
		gen_pool_free(priv->sram_pool, (unsigned long)priv->sram_virt,
			      gen_pool_size(priv->sram_pool));
}

static int vpn_probe(struct platform_device *pdev)
{
	struct vpn_data *priv;
	int ret;

	vpn_subif = devm_kzalloc(&pdev->dev, sizeof(*vpn_subif), GFP_KERNEL);
	if (!vpn_subif)
		return -ENOMEM;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;
	
	priv->ext_id = MXL_SKB_EXT_INVALID;
	priv->soc_data = of_device_get_match_data(&pdev->dev);
	if (!priv->soc_data) {
		dev_err(&pdev->dev, "Failed to find soc data!\n");
		return -ENODEV;
	}

	priv->dev = &pdev->dev;
	platform_set_drvdata(pdev, priv);

	/* get sram */
	priv->sram_pool = of_gen_pool_get(priv->dev->of_node, "mxl,sram", 0);
	if (!priv->sram_pool) {
		dev_err(&pdev->dev, "fail to get mxl,sram pool\n");
		return -ENOMEM;
	}

	priv->sram_virt = gen_pool_dma_alloc(priv->sram_pool,
					     gen_pool_size(priv->sram_pool),
					     &priv->sram_phys);
	if (!priv->sram_virt) {
		dev_err(&pdev->dev, "fail to allocate sram from pool\n");
		return -ENOMEM;
	}

	ret = vpn_init_firewall(priv);
	if (ret) {
		ret = -EPROBE_DEFER;
		goto exit;
	}

	priv->clk = devm_clk_get(&pdev->dev, "arcem");
	if (IS_ERR(priv->clk)) {
		dev_err(&pdev->dev, "unable to get gate clk\n");
		ret = PTR_ERR(priv->clk);
		goto exit;
	}

	ret = clk_prepare_enable(priv->clk);
	if (ret) {
		dev_err(&pdev->dev, "failed to enable clk\n");
		goto exit;
	}

	/* get chiptop regmap */
	priv->syscfg = syscon_regmap_lookup_by_phandle(priv->dev->of_node,
						       "mxl,syscon");
	if (IS_ERR(priv->syscfg)) {
		dev_err(priv->dev, "No phandle for mxl,syscon\n");
		ret = PTR_ERR(priv->syscfg);
		goto exit;
	}

	/* get reset */
	priv->rst = devm_reset_control_get(priv->dev, "arcem");
	if (IS_ERR(priv->rst)) {
		dev_err(priv->dev, "fail to get reset\n");
		ret = PTR_ERR(priv->rst);
		goto exit;
	}

	if (priv->soc_data->use_irq) {
		priv->irq = platform_get_irq(pdev, 0);
		if (priv->irq < 0) {
			dev_err(priv->dev, "fail to get irq\n");
			ret = priv->irq;
			goto exit;
		}

		ret = devm_request_irq(priv->dev, priv->irq, vpn_irq, 0,
				       dev_name(priv->dev), priv);
		if (ret) {
			dev_err(priv->dev, "fail to request irq\n");
			goto exit;
		}
	}

	if (device_property_read_u32(priv->dev, "umt-period-us",
				     &priv->umt_period_us))
		priv->umt_period_us = 10;

	spin_lock_init(&priv->lock);

	ret = vpn_load_firmware(priv);
	if (ret) {
		ret = -EPROBE_DEFER;
		goto exit;
	}

#if defined(CONFIG_DEBUG_FS)
	if (!vpn_init_debugfs(priv))
		vpn_init_dbg(priv);
#endif

	ret = vpn_init_netdev(priv);
	if (ret) {
		ret = -EPROBE_DEFER;
		goto exit;
	}

	vpn_init_genconf(priv);

	/* register API to dp */
	priv->ops.dev = priv->dev;
	priv->ops.get_tunnel_info = vpn_get_tunnel_info;
	priv->ops.update_tunnel_in_act = vpn_update_tunnel_act;
	priv->ops.update_tunnel_out_act = vpn_update_tunnel_act;
	priv->ops.update_tunnel_in_netdev = vpn_update_tunnel_in_netdev;
	priv->ops.add_session = vpn_add_session;
	priv->ops.update_session = vpn_update_session;
	priv->ops.delete_session = vpn_delete_session;
	priv->ops.add_xfrm_sa = vpn_add_xfrm_sa;
	priv->ops.delete_xfrm_sa = vpn_delete_xfrm_sa;
	priv->ops.xfrm_offload_ok = vpn_xfrm_offload_ok;
	priv->ops.proto_to_next_header = vpn_proto_to_next_header;

	dp_register_ops(0, DP_OPS_VPN, (void *)&priv->ops);

	vpn_dp_register_event(priv, 0);

	dev_info(priv->dev, "VPN driver loaded\n");

exit:
	if (ret)
		vpn_cleanup(priv);

	return ret;
}

static int vpn_remove(struct platform_device *pdev)
{
	struct vpn_data *priv;

	priv = platform_get_drvdata(pdev);
	vpn_cleanup(priv);

	return 0;
}

static const struct vpn_soc_data lgm_a_soc_data = {
	.use_irq = false,
	.cnt_mode = UMT_CNT_INC,
	.timeout_ms = 100,
	.soc = LGM_A_SOC,
};

static const struct vpn_soc_data lgm_b_soc_data = {
	.use_irq = true,
	.cnt_mode = UMT_CNT_ACC,
	.timeout_ms = 1000, /* haps requires longer timeout */
	.soc = LGM_B_SOC,
};

static const struct vpn_soc_data lgm_c_soc_data = {
	.use_irq = true,
	.cnt_mode = UMT_CNT_ACC,
	.timeout_ms = 1000, /* haps requires longer timeout */
	.soc = LGM_C_SOC,
};

static const struct of_device_id vpn_match[] = {
	{
		.compatible = "mxl,lgm-vpn",
		.data = &lgm_a_soc_data,
	},
	{
		.compatible = "mxl,lgm-vpn-b",
		.data = &lgm_b_soc_data,
	},
	{
		.compatible = "mxl,lgm-vpn-c",
		.data = &lgm_c_soc_data,
	},
	{}
};
MODULE_DEVICE_TABLE(of, vpn_match);

static struct platform_driver vpn_driver = {
	.probe = vpn_probe,
	.remove = vpn_remove,
	.driver = {
		.name = "mxl-vpn",
		.of_match_table = vpn_match,
	},
};

module_platform_driver(vpn_driver);
MODULE_LICENSE("GPL");
