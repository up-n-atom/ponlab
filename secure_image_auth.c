/*******************************************************************************

  Copyright © 2020 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*  Update Header */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/random.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/ioctl.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>
#include <linux/stat.h>
#include <crypto/skcipher.h>
#include <linux/scatterlist.h>
#include <crypto/skcipher.h>
#include <linux/scatterlist.h>
#include "drv_icc.h"
#include "drv_mps.h"
#include "secure_services.h"
#include <linux/genalloc.h>
#include <soc/mxl/mxl_sec.h>
#include <soc/mxl/mxl_gen_pool.h>
#include <linux/dma-direct.h>
#include <linux/platform_device.h>

typedef struct sse_iccpool_info {
	struct device *icc_dev;
	struct gen_pool *icc_pool;
} iccpool_info_t;

static iccpool_info_t iccpool_inf;
struct mutex sse_img_auth_mutex;
static DECLARE_WAIT_QUEUE_HEAD(img_auth_waitqueue);
static refcount_t img_auth_wakeup = REFCOUNT_INIT(0);
static wait_queue_head_t img_auth_wakeuplist;
static icc_msg_t rw;

#ifdef EIP123_DEF
static int sse_img_secure_img_auth(unsigned int size, void *phy_addr, int isMorechunks, uint32_t arb_farb_com)
{

	icc_msg_t img_auth_msg;
	dma_addr_t sec_img_addr;
	int ret = 0;
	int timeout;

#ifdef IMG_AUTH_DEBUG
	printk("<IMAGE AUTH MSG> Sending ICC msg of size %x dma address %p moreChunk %d\n", size, phy_addr, isMorechunks);
#endif

	memset (&img_auth_msg, 0, sizeof(icc_msg_t));
	memset(&rw, 0, sizeof(icc_msg_t));
	sec_img_addr = dma_map_single_attrs(iccpool_inf.icc_dev, (void *)phy_addr,
				MAX_SIZE, DMA_BIDIRECTIONAL, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(iccpool_inf.icc_dev, sec_img_addr)) {
		pr_err("<Secure Image Auth> Unable to map img data buffer\n");
		ret = -ENOMEM;
		goto out;
	}

	img_auth_msg.src_client_id = IMAGE_AUTH;
	img_auth_msg.dst_client_id = IMAGE_AUTH;
	img_auth_msg.msg_id       = ICC_CMD_IA_EXEC;
	img_auth_msg.param_attr   = ICC_PARAM_PTR | (ICC_PARAM_PTR_NON_IOCU << 1);

	dma_sync_single_for_device(iccpool_inf.icc_dev, sec_img_addr, size, DMA_TO_DEVICE);
	img_auth_msg.param[0]   = (uint32_t)sec_img_addr;
	img_auth_msg.param[1]   = size;
	img_auth_msg.param[2]   = isMorechunks;
	img_auth_msg.param[3]   = arb_farb_com;
	//bit 31 is set to identify the chunking at 4kec side
	if(isMorechunks)
		img_auth_msg.param[1] |= (0x1 << 31);
	else
		img_auth_msg.param[1] &= ~(0x1 << 31);
#ifdef IMG_AUTH_DEBUG
				printk("<IMAGE AUTH MSG> Sending ICC msg\n");
				hexdump(phy_addr, size);
				printk("\n");
#endif

	if (icc_write(IMAGE_AUTH, &img_auth_msg) <= 0) {
		pr_err("Failed to write icc msg \r\n");
		ret = -ECONNREFUSED;
		goto out;
	}
retry:
	timeout = wait_event_interruptible_timeout(img_auth_wakeuplist,
				(refcount_read(&img_auth_wakeup) != 0), msecs_to_jiffies(SSS_ICC_REQ_TIMEOUT * HZ));
	if (timeout == -ERESTARTSYS) {
		pr_debug("<Secure Image Auth> signal received and ignored");
		goto retry;
	}
	if (timeout == 0) {
		pr_err("<Secure Image Auth> request timeout");
		ret = -ETIMEDOUT;
		goto out;
	}

	if (icc_read(IMAGE_AUTH, &rw) < 0) {
		pr_err("Failed to read icc msg \r\n");
		ret = -ETIMEDOUT;
		goto out;
	} else {
		ret = (char)rw.param_attr;
	}

out:
	refcount_set(&img_auth_wakeup, 0);
	dma_sync_single_for_cpu(iccpool_inf.icc_dev, sec_img_addr, size, DMA_FROM_DEVICE);

	return ret;
}
#endif

long sse_img_auth_ioctl(struct file *fd, unsigned int cmd, unsigned long arg)
{
	img_param_t img_param;
	int ret=0;
	int32_t header_cnt=0;
	int32_t mem_cnt=0;
	int32_t size = 0;
	void *mem_addr = NULL;
	void *stcdata = NULL;
	uint32_t arb_farb_com = 0x0;

	stcdata = (void *)gen_pool_alloc(iccpool_inf.icc_pool, MAX_SIZE);

	if (stcdata == NULL) {
		pr_err("Memory Allocation For stcdata failed. \r\n");
		ret = -ENOMEM;
		goto finish;
	}

	mutex_lock(&sse_img_auth_mutex);

	switch(cmd){

		case SS_IOC_SEC_IMGAUTH:

			if (!capable(CAP_SYS_ADMIN)){
				ret = -EACCES;
				goto finish;
			}

			memset(&img_param, 0, sizeof(img_param_t));
			if (copy_from_user (&img_param, (void *) arg, sizeof(img_param_t))) {
				printk(KERN_INFO "copy_from_user error\r\n");
				ret = -EFAULT;
				goto finish;
			}

			size = img_param.src_img_len;
			mem_addr = img_param.src_img_addr;
			if (img_param.chkARB)
				arb_farb_com |= (0x1 << 31);
			if (img_param.chkFARB)
				arb_farb_com |= (0x1 << 30);
			if (img_param.commit)
				arb_farb_com |= (0x1 << 29);
			header_cnt=0;
			do {
				if(header_cnt == 0){
					/*to make it aligned with secure boot requirements, where SBIF_HEADER is header length in bytes and SBIF_ALIGN
					  is the alignment requirement*/
					//mem_cnt = NEXT_CHUNK_SIZE;
					mem_cnt = MIN_ALLOC_SIZE - ((MIN_ALLOC_SIZE - SBIF_HEADER) % SBIF_ALIGN);
					if(size <= mem_cnt)
						mem_cnt = size;
					header_cnt++;
				}

				if(mem_cnt > MAX_SIZE){
					printk("Requested data for header count %d is more than reserved memory\n",header_cnt);
					ret=-EAGAIN;
					goto finish;
				}

				memset(stcdata, 0, MAX_SIZE);
				if (copy_from_user (stcdata, mem_addr, mem_cnt)) {
					printk(KERN_INFO "copy_from_user error\r\n");
					ret = -EAGAIN;
					goto finish;
				}

#ifdef EIP123_DEF
				if ((size - mem_cnt) >  0) {
					ret = sse_img_secure_img_auth(mem_cnt,stcdata,1, arb_farb_com);
				} else {
					ret = sse_img_secure_img_auth(mem_cnt,stcdata,0, arb_farb_com);
				}

				if(ret != 0) {
					ret = -EAGAIN;
					goto finish;
				}
#endif
				/* Move pointer to next nMemac lenght */
				mem_addr = (void *)((uintptr_t)mem_addr+mem_cnt);
				size -= mem_cnt;
				mem_cnt = (size < NEXT_CHUNK_SIZE) ? size : NEXT_CHUNK_SIZE;
			} while (size > 0);
			header_cnt = 0;
			
			if(ret < 0)
				printk("image validation failed\n");

			break;
		default:
			ret = -EINVAL;
			printk(KERN_INFO "Invalid cmd\r\n");
			break;
	}

finish:
	mutex_unlock(&sse_img_auth_mutex);
	if (stcdata)
		gen_pool_free(iccpool_inf.icc_pool, (unsigned long)stcdata, MAX_SIZE);
	return ret;
}

unsigned int sse_img_auth_poll(struct file *fp, poll_table *wait){
	int ret = 0;

	if (!capable(CAP_NET_ADMIN)) {
		printk ("Polling Permission denied\n");
		return 0;
	}

	poll_wait(fp, &img_auth_waitqueue, wait);

	if(rw.param[0] != 0)
		ret |= (POLLIN | POLLRDNORM);
	return ret;
}
/**
  ====================================================================================================
 * @brief secure service callback routine
 ====================================================================================================
 */

void pfn_ss_img_auth_callback(icc_wake_type wake_type)
{
	if((wake_type & ICC_READ)){
		refcount_set(&img_auth_wakeup, 1);
		wake_up_interruptible(&img_auth_wakeuplist);
	}
}

int  __init sse_img_auth_init(void)
{
	int ret=0;

	ret = icc_open((struct inode *)IMAGE_AUTH, NULL);
	if(ret < 0) {
		printk(KERN_INFO "open ICC Failed for image authentication\n");
		goto finish;
	}

	ret = icc_register_callback(IMAGE_AUTH, &pfn_ss_img_auth_callback);
	if(ret < 0){
		printk(KERN_INFO "CallBack Register with ICC Failed for image authentication\n");
		goto finish;
	}

	iccpool_inf.icc_dev = icc_get_genpool_dev();
	iccpool_inf.icc_pool = icc_get_genpool();
		if ( iccpool_inf.icc_pool == NULL ) {
		pr_err("icc_get_genpool failure\n");
				return -1;
	}

	init_waitqueue_head(&img_auth_wakeuplist);

	mutex_init(&sse_img_auth_mutex);
	return ret;

finish:
        icc_unregister_callback(IMAGE_AUTH);
	icc_close((struct inode *)IMAGE_AUTH, NULL);
	return ret;
}

void  __exit sse_img_auth_cleanup(void)
{
	icc_unregister_callback(IMAGE_AUTH);
	icc_close((struct inode *)IMAGE_AUTH, NULL);
}
