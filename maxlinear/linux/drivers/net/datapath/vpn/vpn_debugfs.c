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

#include <linux/debugfs.h>
#include <linux/device.h>
#include <linux/iopoll.h>
#include <linux/regmap.h>
#include <linux/skbuff.h>

#include <net/mxl_cbm_api.h>

#include "vpn.h"

static int vpn_genconf_show(struct seq_file *s, void *v)
{
	struct vpn_data *priv = dev_get_drvdata(s->private);
	struct genconf *genconf = priv->genconf;
	struct ipsec_info *info;
	int i, j;

	seq_puts(s, "VPN genconf:\n");

	for (i = 0; i < MAX_RING; i++)
		seq_printf(s, "\tfw_cur_state[%d]: 0x%x\n", i,
			   genconf->fw_cur_state[i]);
	for (i = 0; i < 8; i++)
		seq_printf(s, "\tres1[%d]: 0x%x\n", i,
			   genconf->res1[i]);
	for (i = 0; i < DBG_BUF_COUNT; i++)
		seq_printf(s, "\tdbg_addr[%d]: 0x%x\n", i,
			   genconf->dbg_addr[i]);
	seq_printf(s, "\tfw_dbg_state: 0x%x\n", genconf->fw_dbg_state);
	seq_printf(s, "\tdbg_enable: 0x%x\n", genconf->dbg_enable);
	seq_printf(s, "\tdbg_rd_idx: 0x%x\n", genconf->dbg_rd_idx);
	seq_printf(s, "\tdbg_wr_idx: 0x%x\n", genconf->dbg_wr_idx);
	seq_printf(s, "\tdbg_rd_cnt: 0x%x\n", genconf->dbg_rd_cnt);
	seq_printf(s, "\tdbg_wr_cnt: 0x%x\n", genconf->dbg_wr_cnt);
	seq_printf(s, "\tdbg_rw_reg_owner: 0x%x\n", genconf->dbg_rw_reg_owner);
	seq_printf(s, "\tdbg_rw_reg_mode: 0x%x\n", genconf->dbg_rw_reg_mode);
	seq_printf(s, "\tdbg_rw_reg_bit_offset: 0x%x\n",
		   genconf->dbg_rw_reg_bit_offset);
	seq_printf(s, "\tdbg_rw_reg_bit_num: 0x%x\n",
		   genconf->dbg_rw_reg_bit_num);
	seq_printf(s, "\tdbg_out: 0x%x\n", genconf->dbg_out);
	seq_printf(s, "\te97_init_flag: 0x%x\n", genconf->e97_init_flag);
	seq_printf(s, "\te97_ring_nr: 0x%x\n", genconf->e97_ring_nr);
	seq_printf(s, "\te97_ring_id: 0x%x\n", genconf->e97_ring_id);
	seq_printf(s, "\tload_stat: 0x%x\n", genconf->load_stat);
	seq_printf(s, "\ta2e_msg: 0x%x\n", genconf->a2e_msg);
	seq_printf(s, "\te2a_msg: 0x%x\n", genconf->e2a_msg);
	seq_printf(s, "\tdbg_rw_reg_addr: 0x%x\n", genconf->dbg_rw_reg_addr);
	seq_printf(s, "\tdbg_rw_reg_value: 0x%x\n", genconf->dbg_rw_reg_value);
	seq_printf(s, "\tvpn_umt_cnt: 0x%x\n", genconf->vpn_umt_cnt);
	seq_printf(s, "\tvpn_umt_cnt_rx: 0x%x\n", genconf->vpn_umt_cnt_rx);
	seq_printf(s, "\tsoc: 0x%x\n", genconf->soc);
	seq_printf(s, "\tdc_buf_rtn_cnt: 0x%x\n", genconf->dc_buf_rtn_cnt);

	for (i = 0; i < IPSEC_TUN_MAX; i++) {
		info = &genconf->ipsec_info[i];
		seq_printf(s, "\tipsec_info[%d]:\n", i);
		seq_printf(s, "\t\t[0x%08x][0x%08x][0x%08x][0x%08x]\n",
			   info->cd_info.dw0.all,
			   info->cd_info.dw1.res,
			   info->cd_info.dw2.bplo,
			   info->cd_info.dw3.bphi);
		seq_printf(s, "\t\t[0x%08x][0x%08x][0x%08x][0x%08x]\n",
			   info->cd_info.dw4.acdlo,
			   info->cd_info.dw5.acdhi,
			   info->cd_info.dw6.all,
			   info->cd_info.dw7.all);
		seq_printf(s, "\t\t[0x%08x][0x%08x][0x%08x][0x%08x]\n",
			   info->cd_info.dw8.ctxlo,
			   info->cd_info.dw9.ctxhi,
			   info->cd_info.dwa.all,
			   info->cd_info.dwb.all);
		seq_printf(s, "\t\tcd_size: 0x%x\n", info->cd_size);
		seq_printf(s, "\t\tmode: 0x%x\n", info->mode);
		seq_printf(s, "\t\tpad_en: 0x%x\n", info->pad_en);
		seq_printf(s, "\t\tpad_instr_offset: 0x%x\n",
			   info->pad_instr_offset);
		seq_printf(s, "\t\tcrypto_instr_offset: 0x%x\n",
			   info->crypto_instr_offset);
		seq_printf(s, "\t\tblk_size: 0x%x\n", info->blk_size);
		seq_printf(s, "\t\thash_pad_instr_offset: 0x%x\n",
			   info->hash_pad_instr_offset);
		seq_printf(s, "\t\tmsg_len_instr_offset: 0x%x\n",
			   info->msg_len_instr_offset);
		seq_printf(s, "\t\ticv_len: 0x%x\n", info->icv_len);
		seq_printf(s, "\t\tiv_len: 0x%x\n", info->iv_len);
		seq_printf(s, "\t\tip_hlen: 0x%x\n", info->ip_hlen);
	}

	for (i = 0; i < IPSEC_TUN_MAX; i++) {
		seq_printf(s, "\tctx[%d]:\n", i);
		for (j = 0; j < CTX_SIZE / 4; j++)
			seq_printf(s, "\t\t[0x%08x][0x%08x][0x%08x][0x%08x]\n",
				   genconf->ctx[i].buf[j],
				   genconf->ctx[i].buf[j + 1],
				   genconf->ctx[i].buf[j + 2],
				   genconf->ctx[i].buf[j + 3]);
	}

	seq_printf(s, "\tipsec_flag: 0x%08x\n", genconf->ipsec_flag);

	for (i = 0; i < IPSEC_TUN_MAX; i++) {
		seq_printf(s, "\tacd_tmpl[%d]:\n", i);
		for (j = 0; j < ACD_SIZE / 4; j++)
			seq_printf(s, "\t\t[0x%08x][0x%08x][0x%08x][0x%08x]\n",
				   genconf->acd_tmpl[i].buf[j],
				   genconf->acd_tmpl[i].buf[j + 1],
				   genconf->acd_tmpl[i].buf[j + 2],
				   genconf->acd_tmpl[i].buf[j + 3]);
	}

	for (i = 0; i < IPSEC_TUN_MAX; i++) {
		seq_printf(s, "\tdwt[%d]: ", i);
		seq_printf(s, "[0x%08x]/[0x%08x] [0x%08x]/[0x%08x] qos %d\n",
			   genconf->dwt[i].dw0_mask, genconf->dwt[i].dw0_val,
			   genconf->dwt[i].dw1_mask, genconf->dwt[i].dw1_val,
			   genconf->dwt[i].enq_qos);
	}

	for (i = 0; i < IPSEC_TUN_MAX; i++) {
		for (j = 0; j < IPSEC_TUN_SESS_MAX; j++) {
			seq_printf(s, "\tsess_act_out[%d][%d]: ", i, j);
			seq_printf(s, "[0x%08x]/[0x%08x] [0x%08x]/[0x%08x] qos %d\n",
				   genconf->sess_act_out[i][j].dw0_mask,
				   genconf->sess_act_out[i][j].dw0_val,
				   genconf->sess_act_out[i][j].dw1_mask,
				   genconf->sess_act_out[i][j].dw1_val,
				   genconf->sess_act_out[i][j].enq_qos);
		}
	}

	seq_printf(s, "\tdc_deq_base: %x\n", genconf->dc_deq_base);
	seq_printf(s, "\tdc_buf_rtn_base: %x\n", genconf->dc_buf_rtn_base);
	seq_printf(s, "\tdc_enq_base[0]: %x\n", genconf->dc_enq_base[0]);
	seq_printf(s, "\tdc_enq_base[1]: %x\n", genconf->dc_enq_base[1]);
	seq_printf(s, "\tdc_buf_rtn_size: %x\n", genconf->dc_buf_rtn_size);
	seq_printf(s, "\tdc_deq_size: %x\n", genconf->dc_deq_size);
	seq_printf(s, "\tdc_enq_size[0]: %x\n", genconf->dc_enq_size[0]);
	seq_printf(s, "\tdc_enq_size[1]: %x\n", genconf->dc_enq_size[1]);

	for (i = 0; i < MAX_ARC_TIMER; i++) {
		seq_printf(s, "\tumt_txin period: %uus\n",
			   genconf->umt_txin[i].period);
		for (j = 0; j < MAX_PORT_PER_TIMER; j++) {
			seq_printf(s, "\t\taddr:0x%x port:%d type:%d\n",
				   genconf->umt_txin[i].info[j].addr,
				   genconf->umt_txin[i].info[j].port,
				   genconf->umt_txin[i].info[j].type);
		}
	}

	return 0;
}

static int vpn_fw_hdr_show(struct seq_file *s, void *v)
{
	struct vpn_data *priv = dev_get_drvdata(s->private);
	struct fw_hdr *fw_hdr = priv->fw_hdr;
	int i;

	seq_puts(s, "VPN fw_hdr:\n");
	seq_printf(s, "\tfw_endian: 0x%x\n", fw_hdr->fw_endian);
	seq_printf(s, "\tfamily: 0x%x\n", fw_hdr->family);
	seq_printf(s, "\tv_maj: 0x%x\n", fw_hdr->v_maj);
	seq_printf(s, "\tv_mid: 0x%x\n", fw_hdr->v_mid);
	seq_printf(s, "\tv_min: 0x%x\n", fw_hdr->v_min);
	seq_printf(s, "\tv_tag: 0x%x\n", fw_hdr->v_tag);
	seq_printf(s, "\tgenconf_offset: 0x%x\n", fw_hdr->genconf_offset);

	seq_puts(s, "\tv_desc:\n");
	seq_puts(s, "\t\t");
	for (i = 0; i < MAX_VERSION_DESC_LEN; i++)
		seq_printf(s, "[0x%x]", fw_hdr->v_desc[i]);
	seq_puts(s, "\n");

	seq_printf(s, "\tcompatible_id: 0x%x\n", fw_hdr->compatible_id);
	seq_printf(s, "\thdr_size: 0x%x\n", fw_hdr->hdr_size);

	return 0;
}

static int vpn_tunnel_show(struct seq_file *s, void *v)
{
	struct vpn_data *priv = dev_get_drvdata(s->private);
	struct tunnel_info *tunnel;
	int i, j;

	for (i = 0; i < IPSEC_TUN_MAX; i++) {
		tunnel = &priv->tunnels[i];
		seq_printf(s, "VPN tunnel[%d]:\n", i);
		seq_printf(s, "\tctx: 0x%llx\n", (u64)tunnel->ctx);
		seq_printf(s, "\tspi: 0x%x\n", tunnel->spi);
		for (j = 0; j < IPSEC_TUN_SESS_MAX; j++) {
			seq_printf(s, "\tsession[%d]:\n", j);
			seq_printf(s, "\t\ton: 0x%x\n", tunnel->session[j].on);
		}
	}

	return 0;
}

static ssize_t vpn_io_write(struct file *file, const char __user *buf,
			    size_t count, loff_t *ppos)
{
	ulong addr = 0, bit_offset = 0, bit_num = 0, value = 0;
	static const char * const mode_strings[] = {
		[VPN_DBG_RW_READ] = "read",
		[VPN_DBG_RW_WRITE] = "write",
		[VPN_DBG_RW_INFO] = "info",
		NULL
	};
	char str[64];
	int len = count;
	struct vpn_data *priv;
	int mode;
	char *p;
	char *tokens[5];
	int ret;
	int i;

	if (!count)
		return 0;

	priv = ((struct seq_file *)file->private_data)->private;
	len = min(count, sizeof(str));

	if (copy_from_user(str, buf, len)) {
		dev_err(priv->dev, "fail to read from user %d\n", len);
		return count;
	}

	str[len - 1] = '\0';
	dev_dbg(priv->dev, "%s command is %s len %d\n", __func__, str, len);

	/* Parse arguments.
	 * At the moment we simply use fixed integer arguments
	 * to avoid the need of complex parsing.
	 */
	p = str;
	for (i = 0; i < 5; i++)
		tokens[i] = strsep(&p, " ");

	if (!tokens[0]) {
		dev_err(priv->dev, "invalid command\n");
		return count;
	}

	mode = match_string(mode_strings, -1, tokens[0]);
	if (mode < 0) {
		dev_err(priv->dev, "invalid command\n");
		return count;
	}

	if (!tokens[1] || kstrtoul(tokens[1], 0, &addr)) {
		dev_err(priv->dev, "invalid address\n");
		return count;
	}

	if (mode == VPN_DBG_RW_READ || mode == VPN_DBG_RW_WRITE) {
		i = 2;
		if (mode == VPN_DBG_RW_WRITE) {
			if (!tokens[i] || kstrtoul(tokens[i++], 0, &value)) {
				dev_err(priv->dev, "invalid value\n");
				return count;
			}
		}

		/* optional options */
		if (!tokens[i] || kstrtoul(tokens[i++], 0, &bit_offset))
			bit_offset = 0;
		if (!tokens[i] || kstrtoul(tokens[i++], 0, &bit_num))
			bit_num = 0;
	}

	dev_info(priv->dev, "mode %x addr %lx value %lx offset %lx num %lx\n",
		 mode, addr, value, bit_offset, bit_num);

	/* wait for firmware to release any ownership */
	ret = vpn_poll_timeout(priv->genconf->dbg_rw_reg_owner !=
			       VPN_DBG_RW_OWNER_FW, 100, 20000);
	if (ret) {
		dev_err(priv->dev, "Timedout waiting for ownership release\n");
		return count;
	}

	priv->genconf->dbg_rw_reg_addr = addr;
	priv->genconf->dbg_rw_reg_value = value;
	priv->genconf->dbg_rw_reg_bit_offset = bit_offset;
	priv->genconf->dbg_rw_reg_bit_num = bit_num;
	priv->genconf->dbg_rw_reg_mode = mode;

	/* set ownership to firmware */
	wmb();
	priv->genconf->dbg_rw_reg_owner = VPN_DBG_RW_OWNER_FW;

	/* wait till firmware gives back ownership */
	ret = vpn_poll_timeout(priv->genconf->dbg_rw_reg_owner ==
			       VPN_DBG_RW_OWNER_DRV, 100, 20000);
	if (ret) {
		dev_err(priv->dev, "Timedout waiting for ownership from fw\n");
		return count;
	}

	/* clear ownership bit */
	priv->genconf->dbg_rw_reg_owner = VPN_DBG_RW_OWNER_NONE;

	return count;
}

static int vpn_io_read(struct seq_file *s, void *v)
{
	seq_puts(s, "Help:\n");
	seq_printf(s, "%s%s", "\techo read <arc_addr>",
		   "[<bit_offset>][<bit_num>] > io\n");
	seq_puts(s, "\t\tRead arc internal register\n");
	seq_printf(s, "%s%s", "\techo write <arc_addr> <value>",
		   "[<bit_offset>][<bit_num>] > io\n");
	seq_puts(s, "\t\tWrite arc internal register\n");
	seq_puts(s, "\techo info <info_num> > io\n");
	seq_puts(s, "\t\tDump vpn firmware global variables\n");

	return 0;
}

static int vpn_io_open(struct inode *inode, struct file *file)
{
	return single_open(file, vpn_io_read, inode->i_private);
}

static const struct file_operations vpn_io_fops = {
	.owner		= THIS_MODULE,
	.open		= vpn_io_open,
	.read		= seq_read,
	.write		= vpn_io_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#if IS_ENABLED(CONFIG_MXL_VPN_TEST)
/* Create a tunnel. This has the same outcome as running below ip cmd:
 * [ip xfrm state add src 192.0.0.10 dst 192.0.0.1 proto esp spi 0x37
 *  reqid 0 mode tunnel enc "cbc(aes)" "0xd7b1d9aa515b5ba8558bbf69d33db6dc"
 *  auth "hmac(sha1)" "" offload dev mxl_vpn dir out]
 */
static int vpn_test_add_sa(struct vpn_data *priv, int mode)
{
	int ret = 0;
	struct vpn_net_data *data;
	struct net_device *netdev;
	struct xfrm_state x;
	static const u32 ealg_key[4] = {
		0xaad9b1d7, 0xa85b5b51, 0x69bf8b55, 0xdcb63dd3};
	static const u32 aalg_key[5] = {
		0xaad9b1d7, 0xa85b5b51, 0x69bf8b55, 0xdcb63dd3, 0xcafebabe};
	struct xfrm_algo_auth *aalg;
	struct xfrm_algo *ealg;
	int i;

	ealg = devm_kzalloc(priv->dev,
			    sizeof(struct xfrm_algo) + sizeof(ealg_key),
			    GFP_KERNEL);
	if (!ealg)
		return -ENOMEM;

	aalg = devm_kzalloc(priv->dev,
			    sizeof(struct xfrm_algo_auth) + sizeof(aalg_key),
			    GFP_KERNEL);
	if (!aalg)
		return -ENOMEM;

	/* allocate dummy netdev */
	netdev = devm_kzalloc(priv->dev,
			      sizeof(struct net_device) +
			      sizeof(struct vpn_net_data), GFP_KERNEL);
	if (!netdev)
		return -ENOMEM;

	data = netdev_priv(netdev);
	data->priv = priv;

	sprintf(ealg->alg_name, "cbc(aes)");
	ealg->alg_key_len = sizeof(ealg_key) * 8;
	memcpy(ealg->alg_key, ealg_key, sizeof(ealg_key));

	sprintf(aalg->alg_name, "hmac(sha1)");
	aalg->alg_key_len = sizeof(aalg_key) * 8;
	memcpy(aalg->alg_key, aalg_key, sizeof(aalg_key));

	x.xso.dev = netdev;
	x.aalg = aalg;
	x.ealg = ealg;
	x.props.family = AF_INET;
	x.props.mode = mode;

	for (i = 0; i < IPSEC_TUN_MAX; i++) {
		x.id.spi = cpu_to_be32(0x37 + i); /* xfrm spi is big endian */

		/* add outbound */
		x.xso.flags = 0;
		vpn_add_xfrm_sa(&x);

		/* add inbound */
		x.xso.flags = XFRM_OFFLOAD_INBOUND;
		vpn_add_xfrm_sa(&x);
	}

	devm_kfree(priv->dev, netdev);
	return ret;
}

/* Delete a tunnel. This has the same outcome as running below ip cmd:
 * [ip xfrm state del src 192.0.0.10 dst 192.0.0.1 proto esp spi 0x37]
 */
static int vpn_test_delete_sa(struct vpn_data *priv)
{
	int ret = 0;
	struct vpn_net_data *data;
	struct net_device *netdev;
	struct xfrm_state x;
	int i;

	/* allocate dummy netdev */
	netdev = devm_kzalloc(priv->dev,
			      sizeof(struct net_device) +
			      sizeof(struct vpn_net_data), GFP_KERNEL);
	if (!netdev)
		return -ENOMEM;

	data = netdev_priv(netdev);
	data->priv = priv;

	x.xso.dev = netdev;

	for (i = 0; i < IPSEC_TUN_MAX; i++) {
		x.id.spi = cpu_to_be32(0x37 + i); /* xfrm spi is big endian */

		/* delete outbound */
		x.xso.flags = 0;
		vpn_delete_xfrm_sa(&x);

		/* delete inbound */
		x.xso.flags = XFRM_OFFLOAD_INBOUND;
		vpn_delete_xfrm_sa(&x);
	}

	devm_kfree(priv->dev, netdev);
	return ret;
}

static const u8 tunnel_in[] = {
	/* mac header + outer-ip + esp header + iv + ip packet */
	0x00, 0x1b, 0x21, 0x86, 0x83, 0x85, 0x00, 0xe0,
	0x92, 0x00, 0x01, 0x50, 0x08, 0x00, 0x45, 0x00,
	0x00, 0x78, 0x01, 0xfb, 0x00, 0x00, 0x40, 0x32,
	0xe2, 0xa3, 0xc0, 0xa8, 0x0a, 0x01, 0xc0, 0xa8,
	0x0a, 0x64, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x45, 0x00, 0x00, 0x3c, 0x7d, 0xc9,
	0x00, 0x00, 0x7f, 0x01, 0xc1, 0x82, 0xc0, 0xa8,
	0x01, 0x23, 0xc0, 0xa8, 0x7a, 0x01, 0x08, 0x00,
	0x41, 0x06, 0x00, 0x01, 0x0c, 0x55, 0x61, 0x62,
	0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a,
	0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72,
	0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 0x63,
	0x64, 0x65, 0x66, 0x67, 0x68, 0x69
};

static const u8 tunnel_out[] = {
	/* mac header (no-outer-ip) + esp header + iv + encrypted ip packet */
	0x00, 0x1b, 0x21, 0x86, 0x83, 0x85, 0x00, 0xe0,
	0x92, 0x00, 0x01, 0x50, 0x08, 0x00, 0x00, 0x00,
	0x00, 0x37, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9a, 0x00,
	0x5a, 0xd9, 0x67, 0xa6, 0x1b, 0x68, 0x03, 0x53,
	0x8e, 0x45, 0x49, 0xf6, 0xd5, 0x66, 0xea, 0x4c,
	0x4a, 0x3c, 0x80, 0x33, 0xa0, 0x9d, 0xcd, 0x8a,
	0x2b, 0xc4, 0x58, 0x79, 0x57, 0xe8, 0x85, 0xcb,
	0x9d, 0x0e, 0x63, 0xd3, 0x0c, 0x51, 0x7c, 0x3f,
	0x67, 0xbe, 0x5b, 0x53, 0x39, 0x8f, 0x0c, 0x2c,
	0x84, 0xcd, 0x5b, 0x7c, 0x95, 0x2c, 0x71, 0xa1,
	0x02, 0x0a, 0x47, 0x5b, 0xe1, 0x9c, 0x70, 0x3f,
	0xa7, 0x78, 0xd0, 0x7e, 0x88, 0x8f, 0x00, 0x85,
	0x51, 0xa5
};

static const u8 transport_in[] = {
	/* mac header + ip header + esp header + iv + ip payload */
	0x00, 0x1b, 0x21, 0x86, 0x83, 0x85, 0x00, 0xe0,
	0x92, 0x00, 0x01, 0x50, 0x08, 0x00, 0x45, 0x00,
	0x00, 0x3c, 0x7d, 0xc9, 0x00, 0x00, 0x7f, 0x01,
	0xc1, 0x82, 0xc0, 0xa8, 0x01, 0x23, 0xc0, 0xa8,
	0x7a, 0x01, 0x08, 0x00, 0x41, 0x06, 0x00, 0x01,
	0x0c, 0x55, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
	0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e,
	0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
	0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
	0x68, 0x69
};

static const u8 transport_out[] = {
	/* mac header + ip header + esp header + iv + encrypted ip payload */
	0x00, 0x1b, 0x21, 0x86, 0x83, 0x85, 0x00, 0xe0,
	0x92, 0x00, 0x01, 0x50, 0x08, 0x00, 0x45, 0x00,
	0x00, 0x68, 0x7d, 0xc9, 0x00, 0x00, 0x7f, 0x32,
	0xc1, 0x25, 0xc0, 0xa8, 0x01, 0x23, 0xc0, 0xa8,
	0x7a, 0x01, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7e, 0xd8, 0x10, 0xa8, 0xb1, 0x89,
	0xa9, 0xe0, 0x21, 0xc0, 0xe3, 0xdc, 0x9f, 0xd7,
	0x07, 0x30, 0xf5, 0xa0, 0x2c, 0xbc, 0xd4, 0xb1,
	0x32, 0x38, 0xbd, 0x70, 0x80, 0x88, 0x94, 0x2c,
	0xf7, 0xbd, 0x11, 0xb7, 0xb4, 0x8a, 0xc9, 0x50,
	0x29, 0xd9, 0x47, 0x53, 0xda, 0xc8, 0x2b, 0xf6,
	0x90, 0x4a, 0x02, 0x1b, 0xb2, 0xde, 0x2e, 0x4e,
	0xa8, 0x56, 0x87, 0x80, 0x36, 0x91
};

/* enqueue hardcoded packet directly to cqm */
static int vpn_test_enq_cqm(struct vpn_data *priv, int is_dec, int tunnel_id,
			    int mode)
{
	int ret;
	u32 flags;
	struct sk_buff *skb;
	struct cbm_tx_data data;
	const u8 *buf, *encrypted_buf;
	u32 size, encrypted_size;
	u32 ipoffset = ETH_HLEN + sizeof(struct iphdr);
	struct dc_desc0_enc *desc_0;
	struct dma_rx_desc_1 *desc_1;

	/* send packet to cqm */
	flags = 0;
	data.dp_inst = 0;
	data.cbm_inst = 0;
	data.pmac = NULL;
	data.pmac_len = 0;
	data.f_byqos = 0;

	skb = alloc_skb(1024, GFP_KERNEL);
	if (!skb)
		return -ENOMEM;

	if (mode == XFRM_MODE_TUNNEL) {
		buf = tunnel_in;
		size = sizeof(tunnel_in);
		encrypted_buf = tunnel_out;
		encrypted_size = sizeof(tunnel_out);

		/* outer IP header already removed by PPv4 */
		if (is_dec)
			ipoffset -= sizeof(struct iphdr);
	} else {
		buf = transport_in;
		size = sizeof(transport_in);
		encrypted_buf = transport_out;
		encrypted_size = sizeof(transport_out);
	}

	if (is_dec) {
		skb_put_zero(skb, encrypted_size);
		memcpy(skb->data, encrypted_buf, encrypted_size);
	} else {
		skb_put_zero(skb, size);
		memcpy(skb->data, buf, size);
	}
	dev_dbg(priv->dev, "skb %px data %px : %llx\n", skb, skb->data,
		virt_to_phys(skb->data));

	/* Setup descriptor for encryption */
	desc_0 = (struct dc_desc0_enc *)&skb->DW0;
	desc_1 = (struct dma_tx_desc_1 *)&skb->DW1;
	desc_0->all = 0;
	desc_0->field.subif = priv->subif >> 9;
	desc_0->field.vp_sess = 7;
	desc_0->field.next_hdr = ESP_NH_IP;
	desc_0->field.tunnel_id = tunnel_id;
	desc_0->field.ipoffset = ipoffset;

	/* Redirect to vpn firmware */
	desc_1->field.ep = priv->gpid;
	desc_1->field.redir = 1; /* egflag */

	if (!is_dec) /* outbound */
		desc_0->field.enc = 1;

	/* clear net interface, since we do not use them for unit-test */
	priv->tunnels[tunnel_id].rx_if = NULL;
	priv->tunnels[tunnel_id].tx_if = NULL;

	dev_dbg(priv->dev, "dw: 0x%x 0x%x 0x %x 0x%x\n",
		skb->DW0, skb->DW1, skb->DW2, skb->DW3);

	ret = cbm_cpu_pkt_tx(skb, &data, flags);
	if (ret)
		dev_err(priv->dev, "Error sending cqm packet %d\n", ret);

	return ret;
}

static ssize_t vpn_test_write(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)
{
	enum {
		VPN_TEST_ENC,
		VPN_TEST_DEC,
		VPN_TEST_ADD_TUNNEL_SA,
		VPN_TEST_ADD_TRANSPORT_SA,
		VPN_TEST_DEL_SA,
	};
	static const char * const mode_strings[] = {
		[VPN_TEST_ENC] = "enc",
		[VPN_TEST_DEC] = "dec",
		[VPN_TEST_ADD_TUNNEL_SA] = "add_tun_sa",
		[VPN_TEST_ADD_TRANSPORT_SA] = "add_trans_sa",
		[VPN_TEST_DEL_SA] = "delete_sa",
		NULL
	};
	static int mode;
	char str[64];
	int len = count;
	struct vpn_data *priv;
	int i;
	char *p;
	char *tokens[3];
	u32 tunnel_id = 0;
	u32 num = 1;

	priv = ((struct seq_file *)file->private_data)->private;
	len = min(count, sizeof(str));

	if (copy_from_user(str, buf, len)) {
		dev_err(priv->dev, "fail to read from user %d\n", len);
		return count;
	}

	str[len - 1] = '\0';
	dev_dbg(priv->dev, "%s command is %s len %d\n", __func__, str, len);

	p = str;
	for (i = 0; i < 3; i++)
		tokens[i] = strsep(&p, " ");

	if (!tokens[0]) {
		dev_err(priv->dev, "invalid command\n");
		return count;
	}

	if (tokens[1]) {
		if (kstrtou32(tokens[1], 0, &tunnel_id)) {
			dev_err(priv->dev, "invalid tunnel-id\n");
			return count;
		}
	}

	if (tokens[2]) {
		if (kstrtou32(tokens[2], 0, &num)) {
			dev_err(priv->dev, "invalid num\n");
			return count;
		}
	}

	switch (match_string(mode_strings, -1, tokens[0])) {
	case VPN_TEST_ENC:
		for (i = 0; i < num; i++)
			vpn_test_enq_cqm(priv, 0, tunnel_id, mode);
		break;
	case VPN_TEST_DEC:
		for (i = 0; i < num; i++)
			vpn_test_enq_cqm(priv, 1, tunnel_id, mode);
		break;
	case VPN_TEST_ADD_TUNNEL_SA:
		mode = XFRM_MODE_TUNNEL;
		vpn_test_add_sa(priv, mode);
		break;
	case VPN_TEST_ADD_TRANSPORT_SA:
		mode = XFRM_MODE_TRANSPORT;
		vpn_test_add_sa(priv, mode);
		break;
	case VPN_TEST_DEL_SA:
		vpn_test_delete_sa(priv);
		break;
	default:
		dev_err(priv->dev, "Invalid command\n");
		break;
	};

	return count;
}

static int vpn_test_read(struct seq_file *s, void *v)
{
	seq_puts(s, "Help:\n");
	seq_puts(s, "\techo add_tun_sa > test\n");
	seq_puts(s, "\t\tCreate eip197 tunnel\n");
	seq_puts(s, "\techo add_trans_sa > test\n");
	seq_puts(s, "\t\tCreate eip197 transport\n");
	seq_puts(s, "\techo delete_sa > test\n");
	seq_puts(s, "\t\tDelete eip197 tunnel\n");
	seq_puts(s, "\techo enc [tunnel_id] [loop] > test\n");
	seq_puts(s, "\t\tGenerate packet and enqueue to cqm (encrypt)\n");
	seq_puts(s, "\techo dec [tunnel_id] [loop] > test\n");
	seq_puts(s, "\t\tGenerate packet and enqueue to cqm (decrypt)\n");

	return 0;
}

static int vpn_test_open(struct inode *inode, struct file *file)
{
	return single_open(file, vpn_test_read, inode->i_private);
}

static const struct file_operations vpn_test_fops = {
	.owner		= THIS_MODULE,
	.open		= vpn_test_open,
	.read		= seq_read,
	.write		= vpn_test_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#endif

static int vpn_prof_show(struct seq_file *s)
{
	struct vpn_data *priv = s->private;

	seq_printf(s, "\t\tipsec_dec_sum: %llu\n",
		   priv->genconf->mprof.ipsec_dec_sum);
	seq_printf(s, "\t\tipsec_enc_sum: %llu\n",
		   priv->genconf->mprof.ipsec_enc_sum);
	seq_printf(s, "\t\tcrypto_api_sum: %llu\n",
		   priv->genconf->mprof.crypto_api_sum);
	seq_printf(s, "\t\tresult_avail_sum: %llu\n",
		   priv->genconf->mprof.result_avail_sum);
	seq_printf(s, "\t\te197_ivld_sum: %llu\n",
		   priv->genconf->mprof.e197_ivld_sum);
	seq_printf(s, "\t\tupd_tunl_cfg_sum: %llu\n",
		   priv->genconf->mprof.upd_tunl_cfg_sum);
	seq_printf(s, "\t\tbuf_rtn_dc_sum: %llu\n",
		   priv->genconf->mprof.buf_rtn_dc_sum);
	seq_printf(s, "\t\tprof flag: %u\n",
		   priv->genconf->mprof.profile_flag);
	seq_printf(s, "\t\tpkt: %u\n",
		   priv->genconf->mprof.pkt);
	seq_printf(s, "\t\tnum_upd_tunl: %u\n",
		   priv->genconf->mprof.num_upd_tunl);
	seq_printf(s, "\t\tnum_e197_invl: %u\n",
		   priv->genconf->mprof.num_e197_invl);
	seq_printf(s, "\t\tnum_buf_rtn: %u\n",
		   priv->genconf->mprof.num_buf_rtn);

	return 0;
}

static ssize_t vpn_prof_write(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)
{
	enum {
		VPN_PROF_START,
		VPN_PROF_STOP,
	};
	static const char * const mode_strings[] = {
		[VPN_PROF_START] = "start",
		[VPN_PROF_STOP] = "stop",
		NULL
	};
	char str[64];
	int len = count;
	struct vpn_data *priv;
	int i;
	char *p;
	char *tokens[3];

	if (!count)
		return 0;

	priv = ((struct seq_file *)file->private_data)->private;
	len = min(count, sizeof(str));

	if (copy_from_user(str, buf, len)) {
		dev_err(priv->dev, "fail to read from user %d\n", len);
		return count;
	}

	str[len - 1] = '\0';
	dev_dbg(priv->dev, "%s command is %s len %d\n", __func__, str, len);

	p = str;
	for (i = 0; i < 3; i++)
		tokens[i] = strsep(&p, " ");

	if (!tokens[0]) {
		dev_err(priv->dev, "invalid command\n");
		return count;
	}

	switch (match_string(mode_strings, -1, tokens[0])) {
	case VPN_PROF_START:
		memset(&priv->genconf->mprof, 0, sizeof(struct vpn_profiling));

		/* ensure starts are reset before triggering start */
		wmb();

		priv->genconf->mprof.profile_flag = PROFILE_START;
		break;
	case VPN_PROF_STOP:
		priv->genconf->mprof.profile_flag = PROFILE_STOP;
		break;
	default:
		dev_err(priv->dev, "Invalid command\n");
		break;
	};

	return count;
}

static int vpn_prof_read(struct seq_file *s, void *v)
{
	return vpn_prof_show(s);
}

static int vpn_prof_open(struct inode *inode, struct file *file)
{
	return single_open(file, vpn_prof_read, inode->i_private);
}

static const struct file_operations vpn_prof_fops = {
	.owner		= THIS_MODULE,
	.open		= vpn_prof_open,
	.read		= seq_read,
	.write		= vpn_prof_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static ssize_t vpn_dbg_write(struct file *file, const char __user *buf,
			     size_t count, loff_t *ppos)
{
	char str[8];
	int len = count;
	struct vpn_data *priv;
	ulong val;

	if (!count)
		return 0;

	priv = ((struct seq_file *)file->private_data)->private;
	len = min(count, sizeof(str));

	if (copy_from_user(str, buf, len)) {
		dev_err(priv->dev, "fail to read from user %d\n", len);
		return count;
	}

	str[len - 1] = '\0';
	if (kstrtoul(str, 0, &val))
		return count;

	priv->genconf->dbg_enable = val & 0xFFFF;

	if (priv->soc_data->use_irq) {
		/* B-step, trigger Atom to ARC interrupt */
		priv->genconf->a2e_msg = INT_DBG_WRITE;
		regmap_update_bits(priv->syscfg, ATOM2EM6, 1, 1);
	} else {
		/* A-step, schedule debug work for polling */
		if (priv->genconf->dbg_enable)
			schedule_delayed_work(&priv->dbg_work,
					      msecs_to_jiffies(1));
	}

	return count;
}

static int vpn_dbg_read(struct seq_file *s, void *v)
{
	seq_puts(s, "Help:\n");
	seq_puts(s, "\techo 0 > dbg\n");
	seq_puts(s, "\t\tDisable debug message from firmware\n");
	seq_puts(s, "\techo 1 > dbg\n");
	seq_puts(s, "\t\tEnable debug message (tx) from firmware\n");
	seq_puts(s, "\techo 2 > dbg\n");
	seq_puts(s, "\t\tEnable debug message (rx) from firmware\n");
	seq_puts(s, "\techo 4 > dbg\n");
	seq_puts(s, "\t\tEnable arc memory debug (rw)\n");
	seq_puts(s, "\tValue is bitwise and can be combined, e.g.:\n");
	seq_puts(s, "\techo 3 > dbg\n");
	seq_puts(s, "\t\tEnable debug message (tx and rx)\n");

	return 0;
}

static int vpn_dbg_open(struct inode *inode, struct file *file)
{
	return single_open(file, vpn_dbg_read, inode->i_private);
}

static const struct file_operations vpn_dbg_fops = {
	.owner		= THIS_MODULE,
	.open		= vpn_dbg_open,
	.read		= seq_read,
	.write		= vpn_dbg_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int vpn_umt_txin_get(void *data, u64 *val)
{
	struct vpn_data *priv = (struct vpn_data *)data;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	*val = priv->genconf->umt_txin_enable;
	return 0;
}

static int vpn_umt_txin_set(void *data, u64 val)
{
	struct vpn_data *priv = (struct vpn_data *)data;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	priv->genconf->umt_txin_enable = !!val;
	priv->genconf->a2e_msg = INT_TXIN_UPDATE;
	regmap_update_bits(priv->syscfg, ATOM2EM6, 1, 1);

	return 0;
}

DEFINE_DEBUGFS_ATTRIBUTE(vpn_umt_txin_fops, vpn_umt_txin_get, vpn_umt_txin_set,
			 "%llu\n");

int vpn_init_debugfs(struct vpn_data *priv)
{
	struct dentry *node;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	priv->debugfs = debugfs_create_dir("vpn", NULL);
	if (!priv->debugfs)
		goto error;

	debugfs_create_devm_seqfile(priv->dev, "genconf", priv->debugfs,
				    &vpn_genconf_show);
	debugfs_create_devm_seqfile(priv->dev, "fw_hdr", priv->debugfs,
				    &vpn_fw_hdr_show);
	debugfs_create_devm_seqfile(priv->dev, "tunnel", priv->debugfs,
				    &vpn_tunnel_show);

	node = debugfs_create_file("io", 0644, priv->debugfs, priv,
				   &vpn_io_fops);
	if (!node)
		goto error;

#if IS_ENABLED(CONFIG_MXL_VPN_TEST)
	node = debugfs_create_file("test", 0644, priv->debugfs, priv,
				   &vpn_test_fops);
	if (!node)
		goto error;
#endif

	node = debugfs_create_file("profile", 0644, priv->debugfs, priv,
				   &vpn_prof_fops);
	if (!node)
		goto error;

	node = debugfs_create_file("dbg", 0644, priv->debugfs, priv,
				   &vpn_dbg_fops);
	if (!node)
		goto error;

	node = debugfs_create_file("umt_txin_enable", 0600, priv->debugfs, priv,
				   &vpn_umt_txin_fops);
	if (!node)
		goto error;

	return 0;

error:
	dev_err(priv->dev, "Fail to create debugfs\n");
	debugfs_remove_recursive(priv->debugfs);
	return -ENOMEM;
}
