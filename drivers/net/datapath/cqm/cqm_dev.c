// SPDX-License-Identifier: GPL-2.0
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

#include "cqm_dev.h"
#include <dt-bindings/net/mxl,lgm-cqm.h>
#define CQM_NUM_DEV_SUPP 3
#define MAX_NUM_BASE_ADDR 16
#define MAX_NUM_INTR 8
#define MAX_POLICY_PER_DEV 8
#define PON_FLAG 1
#define ETH_FLAG 0
#define MAX_DTS_POOL_NUM 2
#define MAX_DTS_POLICY_NUM ((PON_FLAG + 1) * POLICY_VAL_SET)
#define POLICY_VAL_SET 2
#define PON_POLICY_ST (PON_FLAG * POLICY_VAL_SET)
#define ETH_POLICY_ST (ETH_FLAG * POLICY_VAL_SET)
#define LPID_EPG_MAP_VAL_SET 4

static struct dt_node_inst dev_node_name[CQM_NUM_DEV_SUPP] = {
	{FALCON_DEV_NAME, "cqm_prx300", 0},
	{GRX500_DEV_NAME, "cbm", 1},
	{LGM_DEV_NAME, "cqm_lgm", 2},
};

static struct device_node *parse_dts(int j, void **pdata, struct resource **res,
				     int *num_res);
static int cqm_platdev_parse_dts(void);

static int parse_cqm_pool_dts(struct device_node *node,
			      struct cqm_data *pdata, u32 flag)
{
	struct device_node *bm_pools, *pool_p = NULL;
	u32 val_array[2];
	u32 val, idx;
	u32 num;

	bm_pools = of_find_node_by_name(node, "cqm,bm_pools");
	if (!bm_pools) {
		pr_err("Unable to get node %s\n", "cqm,bm_pools");
		return CBM_FAILURE;
	}
	for_each_child_of_node(bm_pools, pool_p) {
		of_property_read_u32(pool_p, "reg", &idx);
		of_property_read_u32(pool_p, "pool,buff_sz",
				     (u32 *)&pdata->pool_size[idx]);
		num = of_property_read_variable_u32_array(pool_p,
							  "pool,pool-num_buffs",
							  val_array, 1,
							  MAX_DTS_POOL_NUM);
		of_property_read_u32(pool_p, "pool,type",
				     &val);
		pdata->pool_type[idx] = val;
		if (val == CQM_NIOC_SHARED || val == SSB_NIOC_SHARED)
			pdata->num_sys_pools++;
		else
			pdata->num_bm_pools++;
		if (flag <= num - 1)
			pdata->pool_ptrs[idx] = val_array[flag];
		else
			pdata->pool_ptrs[idx] = val_array[0];
		pr_debug("%s pool[%d]:pool_size:%ld, pool_ptrs:%ld\n",
			 __func__, idx, pdata->pool_size[idx],
			 pdata->pool_ptrs[idx]);
	}
	return CBM_SUCCESS;
}

static void inc_policy_cnt(struct cqm_policy_res *p, u32 dir)
{
	p->count++;
	if (dir == CQM_TX) {
		p->e_cnt++;
	} else if (dir == CQM_RX) {
		p->i_cnt++;
	} else if (dir == CQM_TX_RX) {
		p->e_cnt++;
		p->i_cnt++;
	}
}

static void copy_policy_cnt(u32 st_idx, u32 end_idx, struct cqm_data *pdata)
{
	u32 i;

	for (i = st_idx + 1; i < end_idx; i++) {
		pdata->policy_res[i].count = pdata->policy_res[st_idx].count;
		pdata->policy_res[i].i_cnt = pdata->policy_res[st_idx].i_cnt;
		pdata->policy_res[i].e_cnt = pdata->policy_res[st_idx].e_cnt;
	}
}

static s32 parse_policy_details(struct device_node *node,
				struct cqm_data *pdata, u32 pon_flag)
{
	struct device_node *p, *tmp;
	u32 val;
	u32 idx, num, cnt_type = 0, cnt_idx = 0, res_id;
	u32 v4[4];
	u32 v2[2 * CBM_MAX_POOLS_PER_POLICY];
	u32 j;
	int count, pool_idx;

	tmp = of_find_node_by_name(node, "cqm,bm_policies");
	if (!tmp || !of_device_is_available(tmp)) {
		pr_err("Unable to get node %s\n", "policy_defs");
		return CBM_FAILURE;
	}

	idx = 0;
	for_each_available_child_of_node(tmp, p) {
		of_property_read_u32(p, "reg", &val);
		count = of_property_count_u32_elems(p, "policy,pool");
		/* "policy,pool" will have entries with format
		 * <pool_idx interface_type>. Pool_count will be
		 * count / 2.
		 */
		pdata->policy_params[idx].pool_count = count / 2;
		pr_debug("policy %d has %d pools\n",
			 idx, pdata->policy_params[idx].pool_count);
		of_property_read_u32_array(p, "policy,pool", v2, count);
		for (pool_idx = 0; 2 * pool_idx < count; pool_idx++) {
			pdata->policy_params[idx].pool_id[pool_idx] =
				v2[2 * pool_idx];
			/* pp_buffer_mgr defines max_allowed for each pool in
			 * a policy. For now, assign pool buffer to
			 * max_allowed for each pool in the policy.
			 */
			pdata->policy_params[idx].pool_max_allowed[pool_idx] =
				pdata->pool_ptrs[v2[2 * pool_idx]];
		}

		/* It is expected the resources in a policy are the same.
		 * Set the interface_type and res_id based on first entry.
		 */
		pdata->policy_params[idx].interface_type = v2[1];
		pdata->policy_res[idx].id = 0;
		if (v2[1] == DP_RES_ID_WAV614 ||
		    v2[1] == DP_RES_ID_WAV624 ||
		    v2[1] == DP_RES_ID_WAV700) {
			pdata->policy_params[idx].interface_type = DP_RES_ID_WAV;
			pdata->policy_res[idx].id = v2[1];
		} else if (v2[1] == DP_RES_ID_DOCSIS ||
			   v2[1] == DP_RES_ID_DOCSIS_MMM ||
			   v2[1] == DP_RES_ID_DOCSIS_VOICE ||
			   v2[1] == DP_RES_ID_DOCSIS_MPEG) {
			pdata->policy_params[idx].interface_type = DP_RES_ID_DOCSIS;
			pdata->policy_res[idx].id = v2[1];
		}

		pdata->policy_params[idx].policy_id = val;

		num = of_property_read_variable_u32_array(p, "policy,alloc",
							  v4, POLICY_VAL_SET,
							  MAX_DTS_POLICY_NUM);
		if (pon_flag && num >= MAX_DTS_POLICY_NUM &&
		    !v4[PON_POLICY_ST])
			continue;

		if (num >= MAX_DTS_POLICY_NUM)
			j = pon_flag ? PON_POLICY_ST : ETH_POLICY_ST;
		else
			j = ETH_POLICY_ST;

		pdata->policy_params[idx].min_guaranteed = v4[j];
		pdata->policy_params[idx].max_allowed = v4[j + 1];

		of_property_read_u32(p, "policy,direction",
				     &pdata->policy_params[idx].direction);

		/* For WAV policies, multiple sets of RX/TX policies may be
		 * defined for different radios. In such cases, they should be
		 * considered different groups.
		 * If they were grouped together, the rule that RX and TX
		 * policies in a group should be continuous would be broken.
		 * Each WAV radio should have 2 policies, one for RX and one for
		 * TX. RX policy should be before TX policy.
		 */
		if (idx == 0 ||
		    (cnt_type != pdata->policy_params[idx].interface_type ||
		     res_id != pdata->policy_res[idx].id) ||
		    (pdata->policy_params[idx].interface_type == DP_RES_ID_WAV &&
		     (pdata->policy_params[idx].direction & CQM_RX))) {
			copy_policy_cnt(cnt_idx, idx, pdata);
			cnt_type = pdata->policy_params[idx].interface_type;
			cnt_idx = idx;
			res_id = pdata->policy_res[idx].id;
			inc_policy_cnt(&pdata->policy_res[idx],
				       pdata->policy_params[idx].direction);
		} else if (cnt_type ==
			   pdata->policy_params[idx].interface_type &&
			   res_id == pdata->policy_res[idx].id) {
			inc_policy_cnt(&pdata->policy_res[cnt_idx],
				       pdata->policy_params[idx].direction);
		}
		idx++;
	}

	copy_policy_cnt(cnt_idx, idx, pdata);
	return CBM_SUCCESS;
}

static s32 parse_voice_mem_details(struct device_node *c55_ddr,
				   struct cqm_data *pdata)
{
	const __be32 *addrp;
	u64 addr, size;

	if (!IS_ERR_OR_NULL(c55_ddr)) {
		addrp = of_get_address(c55_ddr, 0, &size, NULL);
		if (!IS_ERR_OR_NULL(addrp))
			addr = of_read_number(addrp, of_n_addr_cells(c55_ddr));
		else
			return -EINVAL;
		pdata->voice_buf_start = addr;
		pdata->voice_buf_size = size;
		pr_debug("vocie mem:start:%lx, size:%x, addr:%lx\n",
			 (unsigned long)pdata->voice_buf_start,
			 (u32)pdata->voice_buf_size, (unsigned long)addr);
		return CBM_SUCCESS;
	}
	return -EINVAL;
}

static s32 parse_lpid_config_details(struct device_node *lpid_config,
				     struct cqm_data *pdata)
{
	u32 v4[LPID_EPG_MAP_VAL_SET * LPID_COUNT];
	u32 val = 0;
	int num;
	int idx;

	if (IS_ERR_OR_NULL(lpid_config))
		return -EINVAL;

	if (of_property_read_u32(lpid_config, "mxl,port-mode",
				 &val))
		pdata->lpid_port_mode = LPID_PORT_INVALID;
	else
		pdata->lpid_port_mode = val;

	if (of_property_read_u32(lpid_config, "mxl,wan-mode",
				 &val))
		pdata->lpid_wan_mode = LPID_WAN_INVALID;
	else
		pdata->lpid_wan_mode = (u8)val;

	num = of_property_read_variable_u32_array(lpid_config,
						  "mxl,lpid-epg-map",
						  v4,
						  LPID_EPG_MAP_VAL_SET,
						  LPID_EPG_MAP_VAL_SET * LPID_COUNT);
	for (idx = 0; idx < LPID_COUNT; idx++) {
		if (idx < (num / LPID_EPG_MAP_VAL_SET)) {
			pdata->lpid_epg_map[idx].lpid = v4[idx * LPID_EPG_MAP_VAL_SET];
			pdata->lpid_epg_map[idx].epg = v4[idx * LPID_EPG_MAP_VAL_SET + 1];
			pdata->lpid_epg_map[idx].txdma_ctrl = (u8)v4[idx * LPID_EPG_MAP_VAL_SET + 2];
			pdata->lpid_epg_map[idx].txdma_chan = (u8)v4[idx * LPID_EPG_MAP_VAL_SET + 3];
		} else {
			pdata->lpid_epg_map[idx].lpid = 0;
			pdata->lpid_epg_map[idx].epg = 0;
			pdata->lpid_epg_map[idx].txdma_ctrl = 0;
			pdata->lpid_epg_map[idx].txdma_chan = 0;
		}
	}

	return CBM_SUCCESS;
}

static s32 parse_bm_buff_size_details(struct device_node *config,
				      struct cqm_data *pdata)
{
	u32 v[BM_BUF_SIZE_MAX];
	int num;
	int idx;

	if (IS_ERR_OR_NULL(config))
		return -EINVAL;

	num = of_property_read_variable_u32_array(config,
						  "mxl,bm-buff-pool-size",
						  v,
						  1,
						  BM_BUF_SIZE_MAX);
	if (num < 0)
		return -EINVAL;

	for (idx = 0; idx < num; idx++)
			pdata->bm_buf_sizes[idx] = v[idx];

	return CBM_SUCCESS;
}

static s32 parse_deq_dma_delay(struct device_node *node,
			       struct cqm_data *pdata)
{
	u32 v[3 * DLY_PORT_COUNT];
	int num;
	int idx;

	if (IS_ERR_OR_NULL(node))
		return -EINVAL;

	num = of_property_read_variable_u32_array(node,
						  "mxl,deq_dma_delay",
						  v,
						  3,
						  3 * DLY_PORT_COUNT);
	for (idx = 0; idx < DLY_PORT_COUNT; idx++) {
		if (idx < (num / 3)) {
			pdata->deq_dma_delay[idx].dcp = v[idx * 3];
			pdata->deq_dma_delay[idx].deq_port = v[idx * 3 + 1];
			pdata->deq_dma_delay[idx].delay = v[idx * 3 + 2];
		} else {
			pdata->deq_dma_delay[idx].dcp = 0;
			pdata->deq_dma_delay[idx].deq_port = 0;
			pdata->deq_dma_delay[idx].delay = 0;
		}
	}

	return CBM_SUCCESS;
}

struct device_node *parse_dts(int j, void **pdata, struct resource **res,
			      int *num_res)
{
	struct device_node *node = NULL, *cpu_deq_port = NULL;
	struct device_node *ret_node = NULL;
	struct device_node *c55_ddr_node;
	struct device_node *wav_node;
	struct device_node *config_node;
	int idx = 0;
	int dpdk_idx = 0;
	struct cqm_data *cqm_pdata = NULL;
	unsigned int intr[MAX_NUM_INTR];
	struct resource resource[MAX_NUM_BASE_ADDR];
	struct property *prop;
	const __be32 *p;
	u32 dt_cfg;
	s32 count = 0;
	u32 sys_type_flag = 0;
	struct of_phandle_iterator it;
	int err;

	node = of_find_node_by_name(NULL, dev_node_name[j].node_name);
	if (!of_device_is_available(node))
		return NULL;
	*pdata = kzalloc(sizeof(*cqm_pdata), GFP_KERNEL);
	if (!*pdata) {
		pr_err("%s: Failed to allocate pdata.\n", __func__);
		goto err_free_pdata;
	}
	cqm_pdata = (struct cqm_data *)(*pdata);

	for (idx = 0; idx < MAX_NUM_BASE_ADDR; idx++) {
		if (of_address_to_resource(node, idx, &resource[idx]))
			break;
	}
	*res = kmalloc_array(idx, sizeof(struct resource),
			     GFP_KERNEL);
	if (!*res) {
		pr_info("error allocating memory\n");
		goto err_free_pdata;
	}
	memcpy(*res, resource, (sizeof(struct resource) * idx));
	cqm_pdata->num_resources = idx;
	*num_res = idx;

	for (idx = 0; idx < MAX_NUM_INTR; idx++) {
		intr[idx] = irq_of_parse_and_map(node, idx);
		if (!intr[idx])
			break;
	}
	cqm_pdata->num_intrs = idx;
	cqm_pdata->intrs = kmalloc_array(idx, sizeof(unsigned int),
					 GFP_KERNEL);
	memcpy(cqm_pdata->intrs, intr, (sizeof(unsigned int) * idx));
	cqm_pdata->rcu_reset = of_reset_control_get(node, "cqm");
	if (IS_ERR(cqm_pdata->rcu_reset)) {
		pr_err("No rcu reset for %s\n", dev_node_name[j].node_name);
		/*return PTR_ERR(cqm_pdata->rcu_reset)*/;
	}

	cqm_pdata->cqm_clk[0] = (void *)of_clk_get_by_name(node, "freq");
	if (IS_ERR(cqm_pdata->cqm_clk[0]))
		pr_err("Error getting freq clk\n");
	cqm_pdata->cqm_clk[1] = (void *)of_clk_get_by_name(node, "cbm");
	if (IS_ERR(cqm_pdata->cqm_clk[1]))
		pr_err("Error getting cqm clk\n");
	cqm_pdata->syscfg = syscon_regmap_lookup_by_phandle(node,
							    "mxl,syscon");
	if (IS_ERR(cqm_pdata->syscfg)) {
		pr_err("No syscon phandle specified for wan mux\n");
		cqm_pdata->syscfg = NULL;
	}
	cqm_pdata->force_xpcs = of_property_read_bool(node, "mxl,force-xpcs");

	of_property_read_u32(node, "mxl,wav700",
			     &cqm_pdata->wav700);

	config_node = of_parse_phandle(node, "mxl,lpid_config", 0);
	if (parse_lpid_config_details(config_node, cqm_pdata) < 0)
		pr_err("err in lpid_config data!\n");

	config_node = of_parse_phandle(node, "mxl,bm-buff-size", 0);
	if (parse_bm_buff_size_details(config_node, cqm_pdata) < 0)
		pr_err("err in mxl,bm-buff-size data!\n");

	if ((cqm_pdata->lpid_wan_mode & LPID_WAN_PON) ||
	    (cqm_pdata->lpid_wan_mode & LPID_WAN_DOCSIS)) {
		sys_type_flag = 1;
		pr_debug("This is CQM PON SYS\n");
	}

	cqm_pdata->txin_fallback = of_property_read_bool(node, "mxl,txin-fallback");
	cqm_pdata->cpu_enq_chk = of_property_read_bool(node, "mxl,cpu-pool-enq-chk");

	if (parse_deq_dma_delay(node, cqm_pdata))
		pr_warn("Failed to read deq dma delay data!\n");

	parse_cqm_pool_dts(node, cqm_pdata, sys_type_flag);
	parse_policy_details(node, cqm_pdata, sys_type_flag);

	idx = 0;
	dpdk_idx = 0;
	for_each_available_child_of_node(node, cpu_deq_port) {
		if (idx >= MAX_CPU_DQ_PORT_N_TYPE)
			break;

		count = of_property_count_u32_elems(cpu_deq_port,
						    "mxl,deq-port");
		if (unlikely(count < 0))
			continue;

		if (count != MAX_CPU_DQ_PORT_ARGS) {
			pr_err("Invalid args in %s\n", cpu_deq_port->name);
			continue;
		}

		of_property_for_each_u32(cpu_deq_port, "mxl,deq-port", prop,
					 p, cqm_pdata->dq_port[idx]) {
			idx++;
		}

		count = of_property_count_u32_elems(cpu_deq_port,
						    "mxl,np-dpdk");
		if (count != MAX_DPDK_DQ_PORT_ARGS) {
			dpdk_idx = idx;
			continue;
		}

		of_property_for_each_u32(cpu_deq_port, "mxl,np-dpdk", prop,
					 p, cqm_pdata->dpdk_cfg[dpdk_idx]) {
			dpdk_idx++;
		}
	}
	cqm_pdata->num_dq_port = idx;

	idx = 0;
	wav_node = of_parse_phandle(node, "mxl,wav-port-resv", 0);
	of_property_for_each_u32(wav_node, "mxl,wav-port-resv",
				 prop, p, dt_cfg) {
		cqm_pdata->port_resv[idx++] = dt_cfg;
	}

	c55_ddr_node = of_parse_phandle(node, "mxl,c55_ddr_mem", 0);
	if (parse_voice_mem_details(c55_ddr_node, cqm_pdata) < 0)
		pr_err("err in c55 ddr resource data!\n");

	ret_node = node;
	for (idx = 0; idx < MAX_CQM_FW_DOMAIN; idx++)
		cqm_pdata->sai[idx] = -1;
	idx = 0;
	of_for_each_phandle(&it, err, node, "firewall-domains", NULL, 0) {
		if (idx >= MAX_CQM_FW_DOMAIN)
			pr_err("err in Firewall domain entries\n");
		else
			of_property_read_s32(it.node, "sai", &cqm_pdata->sai[idx]);
		idx++;
	}
	if (of_property_read_s32(node, "mxl,sys_type",
				 &cqm_pdata->sys_type))
		cqm_pdata->sys_type = -1;

	if (of_property_read_s32(node, "mxl,cqm_clkrate",
				 &cqm_pdata->cqm_clkrate))
		cqm_pdata->cqm_clkrate = -1;

	return ret_node;

err_free_pdata:
	kfree(pdata);
	return NULL;
}

int add_cqm_dev(int i)
{
	struct platform_device *pdev = NULL;
	void *pdata = NULL;
	struct device_node *node = NULL;
	struct resource *res;
	int ret = CBM_SUCCESS, num_res;

	node = parse_dts(i, &pdata, &res, &num_res);
	if (!node) {
		pr_debug("%s(#%d): parse_dts fail for %s\n",
			 __func__, __LINE__, dev_node_name[i].dev_name);
		return CBM_FAILURE;
	}
	pdev = platform_device_alloc(dev_node_name[i].dev_name, 1);
	if (!pdev) {
		pr_err("%s(#%d): platform_device_alloc fail for %s\n",
		       __func__, __LINE__, dev_node_name[i].node_name);
		return -ENOMEM;
	}

	/* Attach node into platform device of_node */
	pdev->dev.of_node = node;
	/* Set the  private data */
	if (pdata)
		platform_set_drvdata(pdev, pdata);
	/* Add resources to platform device */
	if (num_res > 0 && res) {
		ret = platform_device_add_resources(pdev, res, num_res);
		if (ret) {
			pr_info("%s: Failed to add resources for %s.\n",
				__func__, dev_node_name[i].node_name);
			goto err_free_pdata;
		}
	}

	/* Add platform device */
	ret = platform_device_add(pdev);
	if (ret) {
		pr_info("%s: Failed to add platform device for %s.\n",
			__func__, dev_node_name[i].node_name);
		goto err_free_pdata;
	}

	kfree(res);
	return ret;

err_free_pdata:
	kfree(pdata);
	kfree(res);
	return ret;
}

int nodefromdevice(const char *dev)
{
	int i;

	for (i = 0; i < CQM_NUM_DEV_SUPP; i++) {
		if (strcmp(dev_node_name[i].dev_name, dev) == 0)
			return i;
	}
	return -1;
}

static int cqm_platdev_parse_dts(void)
{
	int i, count = 0;

	for (i = 0; i < CQM_NUM_DEV_SUPP; i++) {
		if (!of_find_node_by_name(NULL, dev_node_name[i].node_name))
			continue;

		if (!add_cqm_dev(i))
			count++;
	}
	pr_info("Totally %d CQM device registered!\n", count);
	return 0;
}

static __init int cqm_platdev_init(void)
{
	cqm_platdev_parse_dts();
	return 0;
}

device_initcall(cqm_platdev_init);

MODULE_LICENSE("GPL");
