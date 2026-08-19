/*
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * Copyright (c) 2017 - 2019 Intel Corporation
 * Copyright (c) 2011 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 */

#include <linux/kernel.h>

#include <asm/cpu.h>
#include <asm/processor.h>
#include <asm/barrier.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/wait.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/icc/drv_icc.h>
#include <linux/genalloc.h>
#include <linux/of_device.h>
#include <linux/dma-mapping.h>
#include <linux/dma-direct.h>
#include <soc/mxl/mxl_sec.h>
#include <soc/mxl/mxl_gen_pool.h>

extern uint32_t BLOCK_MSG[MAX_CLIENT];
extern void process_icc_message(unsigned int uiClientId, bool sync, uint32_t msg_id);
static lgm_iccpool_dev_t gl_iccpool_dev;

/*
 * icc_get_genpool_dev - Get the ICC Pool device
 *
 *@return Pointer to ICC pool device structure
 *
 *@Description: ICC client driver which needs to do DMA operation on
 * memory allocated by genpool APIs from ICC Pool needs to use ICC pool
 * device struct as one of parameter
 */
struct device *icc_get_genpool_dev(void)
{
	return &gl_iccpool_dev.pdev->dev;
}
EXPORT_SYMBOL(icc_get_genpool_dev);

/*
 * icc_get_genpool - Get the ICC genpool
 *
 *@return Pointer to ICC genpool structure
 *
 *@Description: ICC client driver which needs to allocate the memory
 * using gen_pool APIs needs to use ICC genpool struct as one of
 * parameter.
 */
struct gen_pool *icc_get_genpool(void)
{
	return gl_iccpool_dev.pool;
}
EXPORT_SYMBOL(icc_get_genpool);

lgm_iccpool_dev_t *get_icc_pool(void)
{
	struct device_node *icc_pool_dn, *parent;

	icc_pool_dn = of_find_node_by_path(NODE_PATH);
	if (!icc_pool_dn) {
		pr_err("%s: could not find socpool dt node\n", __func__);
		goto func_end;
	}

	gl_iccpool_dev.pdev = of_find_device_by_node(icc_pool_dn);
	if (!gl_iccpool_dev.pdev) {
		/* Check if named gen_pool is created by parent node device */
		parent = of_get_parent(icc_pool_dn);
		gl_iccpool_dev.pdev = of_find_device_by_node(parent);
		of_node_put(parent);
	}

	if (!gl_iccpool_dev.pdev)
		goto func_end;

	gl_iccpool_dev.pool = gen_pool_get(&gl_iccpool_dev.pdev->dev, icc_pool_dn->name);
	if (!gl_iccpool_dev.pool)
		goto func_end;

	return &gl_iccpool_dev;

func_end:
	return ERR_PTR(-ENODEV);
}

static dma_addr_t icc_gen_pool_alloc(int buf_size)
{
	unsigned long vaddr = 0;
	dma_addr_t dma_addr = 0;

	pr_debug("%s func called with buf size :%x", __func__, buf_size);
	if (!gl_iccpool_dev.pool) {
		pr_debug("ICC pool from genpool is not mapped");
		goto out_pool;
	}
	vaddr = gen_pool_alloc(gl_iccpool_dev.pool, buf_size);
	if (!vaddr) {
		pr_err("Failed to allocate memory of buf size :%x\n", buf_size);
		goto out_pool;
	}

	dma_addr = dma_map_single_attrs(&gl_iccpool_dev.pdev->dev, (void *)vaddr,
			buf_size, DMA_BIDIRECTIONAL, DMA_ATTR_NON_CONSISTENT);

out_pool:
	return dma_addr;
}

static void icc_gen_pool_free(dma_addr_t dma_addr, int buf_size)
{
	void *vaddr;

	pr_debug("%s func called with buf size :%x", __func__, buf_size);
	dma_sync_single_for_device(&gl_iccpool_dev.pdev->dev, GET_DMA_ADDR(dma_addr), buf_size, DMA_BIDIRECTIONAL);
	vaddr = phys_to_virt(dma_addr);
	gen_pool_free(gl_iccpool_dev.pool, (unsigned long)vaddr, buf_size);
}

void pfn_icc_client_callback(icc_wake_type wake_type) 
{
	int ret, buf_sz;
	icc_msg_t rw, icc_reply;
	dma_addr_t dma_handler;

	if ((wake_type & ICC_READ)) {
		memset(&rw, 0, sizeof(icc_msg_t));
		ret = icc_read(ICC_Client, &rw);
		if (ret > 0) {
			/*message from self*/
			if (rw.src_client_id != ICC_Client && rw.msg_id == ICC_MSG_FLOW_CONTROL) {
				rw.src_client_id = ICC_Client;
				icc_write(ICC_Client, &rw);
			}
		} else {
			pr_err("Read from ICC Client 0 driver failed");
			return;
		}
	}

	switch (rw.msg_id) {
	case ICC_MSG_FLOW_CONTROL:
				/*Flow control OFF for the client and wake it up*/
		if (rw.param[0]) {
			BLOCK_MSG[rw.param[1]] = 0;
			process_icc_message(rw.param[1], 0, 0);
		} else {/*Flow control turn on for the client*/
			BLOCK_MSG[rw.param[1]] = 1;
		}
		break;
	case ICC_REMOTE_MEM_ALLOC:
		buf_sz = rw.param[0];
		memset(&icc_reply, 0, sizeof(icc_msg_t));
		dma_handler = icc_gen_pool_alloc(buf_sz);
		if (!dma_handler) {
			pr_err("Failed to allocate memory\n");
			icc_reply.param[1] = 0;
		} else {
			pr_debug("Successfully allocated the memory\n");
			icc_reply.param[0] = 0x1;
			/* Using masking of higher 4 bit instead of shifting */
			icc_reply.param[1] = (uint32_t)dma_handler;
			icc_reply.param[2] = buf_sz;
		}
		icc_reply.src_client_id = ICC_Client;
		icc_reply.dst_client_id = rw.src_client_id;
		icc_reply.msg_id  = ICC_REMOTE_MEM_ALLOC_REPLY;
		icc_write(ICC_Client, &icc_reply);
		break;
	case ICC_REMOTE_MEM_FREE:
		pr_debug("Free the memory from ICC mempool");
		dma_handler = rw.param[0];
		buf_sz = rw.param[1];
		icc_gen_pool_free(dma_handler, buf_sz);
		break;
	default:
		pr_err("Not a valid message for icc client\n");
		break;
	}

	return;
}

int icc_send_mem_adapter_cfg(void)
{
	lgm_iccpool_dev_t *iccpool_dev;
	icc_msg_t *retmsg = NULL, msg;
	int ret;
	struct mxl_pool_event_data data;

	iccpool_dev = get_icc_pool();
	if (IS_ERR(iccpool_dev))
		return PTR_ERR(iccpool_dev);

	ret = mxl_noc_genpl_notifier_blocking_chain(NOC_GENPL_EVENT_ICC_POOL,
						    &data);
	if (notifier_to_errno(ret)) {
		pr_err("Failed to get ICC POOL range!\n");
		return -EINVAL;
	}

	data.paddr = ioc_addr_to_nioc_addr(data.paddr);
	pr_debug("base addr: 0x%lx, size: %lx\n", data.paddr, data.size);

	msg.src_client_id = ICC_Client;
	msg.dst_client_id = ND;
	msg.msg_id = ICC_MSG_TEP_ADAPTER_CFG;
	msg.param[0] = data.paddr >> BIT_SHIFT;
	msg.param[1] = data.size;
	retmsg = icc_sync_write(ICC_Client, &msg);
	if (!retmsg || retmsg->src_client_id != ND) {
		pr_err("%s icc_sync_write fail \n", __func__);
		return -EINVAL;
	}
	ret = retmsg->param_attr;
	kfree(retmsg);
	return ret;
}

static int __init init_icc_client(void)
{
	int result = 0, ret = 0;
	result = icc_open((struct inode *)ICC_Client, NULL);
	if (result < 0) {
		pr_err("open ICC client  Failed\n");
		return result;
	}
	result = icc_register_callback(ICC_Client, &pfn_icc_client_callback);
	if (result < 0) {
		pr_err("Call back register for ICC client  Failed\n");
		return result;
	}
	ret = icc_send_mem_adapter_cfg();
	if (ret < 0) {
		BUG_ON("ICC not received ACK from tep \n");
	}
	return 0;
}
MODULE_LICENSE("GPL");
/* Automatically create the entry */
module_init(init_icc_client);
