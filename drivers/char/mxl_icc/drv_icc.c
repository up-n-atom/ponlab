// SPDX-License-Identifier: GPL-2.0+
/******************************************************************************
**
** FILE NAME    : ICC_device.c
** PROJECT      : GRX500
** MODULES      : ICC
**
** DATE         : 10 MAY 2014
** AUTHOR       : Swaroop Sarma
** DESCRIPTION  : ICC module
** COPYRIGHT    :
**	Copyright (c) 2020 - 2022 MaxLinear Corporation
**	Copyright (c) 2017 - 2019 Intel Corporation
**	Copyright (c) 2011 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
**
**   Any use of this software is subject to the conclusion of a respective
**   License agreement. Without such a License agreement no rights to the
**   software are granted
**
** HISTORY
** $Date        $Author  $Comment
*******************************************************************************/

/* Group definitions for Doxygen */
/** \addtogroup API API-Functions */
/** \addtogroup Internal Internally used functions */
/*linux kernel headers*/
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/vmalloc.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/sem.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/semaphore.h>
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
/*asm header files*/
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/unistd.h>
#include <asm/cacheflush.h>
#include <linux/errno.h>
/*icc header file*/
#include "../mxl_mps2/drv_config.h"
#include <linux/icc/drv_icc.h>
/****************************Local definitions**************************/
#define FIFO_GET_READ_PTR(index)  (ICC_BUFFER[index].fifo_read)
#define FIFO_GET_WRITE_PTR(index) (ICC_BUFFER[index].fifo_write)
#define GET_ICC_READ_MSG(clientid , readptr) \
				(ICC_BUFFER[clientid].MPS_BUFFER[readptr])
#define GET_ICC_WRITE_MSG(clientid , wrptr) \
				(ICC_BUFFER[clientid].MPS_BUFFER[wrptr])
CREATE_TRACE_GROUP(ICC);

static DEFINE_SPINLOCK(g_icc_semidx_lock);

/********************************Local functions*************************/

unsigned int icc_poll(struct file *file_p, poll_table *wait);
int icc_read_d(struct file *file_p, char *buf, size_t count, loff_t *ppos);
int icc_write_d(struct file *file_p, char *buf, size_t count, loff_t *ppos);
int icc_mmap(struct file *file, struct vm_area_struct *vma);
long icc_ioctl(struct file *file_p, uint32_t nCmd, unsigned long arg);
extern int poll_mps_mbx_upstream(mps_message *pmsg, unsigned int sem_index, unsigned int seq_num, uint8_t client_id);

/******************************Global variable declaration***************/
#ifndef KTHREAD
struct ssd_client ssd_clients[] = {
	[ICC_SEC_UPGRADE_NR] = {"secupg", ICC_SECURE_SERVICE},
	[ICC_SEC_STORAGE_NR] = {"secstg", ICC_SECURE_SERVICE},
	[ICC_SEC_SIGNINIG_NR] = {"secsign", ICC_SECURE_SERVICE},
	[ICC_REGMAP_NR] = {"regmap", !ICC_SECURE_SERVICE},
	[ICC_FIREWALL_NR] = {"firewall", !ICC_SECURE_SERVICE},
	[ICC_SEC_STG_ADMIN_NR] = {"sec_store_admin", ICC_SECURE_SERVICE},
	[ICC_SEC_STG_NORMAL_NR] = {"sec_store_normal", ICC_SECURE_SERVICE},
	[ICC_SEC_DBG_ADMIN_NR] = {"secdbg", ICC_SECURE_SERVICE},
};
EXPORT_SYMBOL(ssd_clients);

typedef struct {
	struct work_struct icc_work;
	int    x;
	bool   s;
	int    m;
} icc_t;

typedef struct {
	struct delayed_work icc_work;
	int    x;
	bool   s;
	int    m;
} iccd_t;
#endif
typedef struct {
	uint32_t fifo_write;
	uint32_t fifo_read;
	mps_message MPS_BUFFER[MAX_DEPTH];
} icc_fifo_t;

typedef struct {
	uint64_t address[MAX_MMAP];
	uint64_t virtual_addr[MAX_MMAP];
	uint32_t count;
} mmap_addr_t;

static short icc_major_id;
char icc_dev_name[] = "mxl_icc";

/*Variable used*/
#ifdef KTHREAD
unsigned int g_num;
static struct task_struct *icc_kthread[MAX_CLIENT];
#else
static struct workqueue_struct *icc_wq[MAX_CLIENT];
static void icc_wq_function(struct work_struct *work);
#endif
static refcount_t icc_excpt[MAX_CLIENT];
static icc_dev iccdev[MAX_CLIENT];
uint32_t BLOCK_MSG[MAX_CLIENT];
static mmap_addr_t mmap_address[MAX_CLIENT];
static icc_fifo_t ICC_BUFFER[MAX_CLIENT];
#ifdef KTHREAD
static struct semaphore icc_callback_sem;
#endif

/* the driver callbacks */
static struct file_operations icc_fops = {
	owner: THIS_MODULE,
	poll : icc_poll,
	read : (void *)icc_read_d,
	write : (void *)icc_write_d,
	mmap : icc_mmap,
	unlocked_ioctl : icc_ioctl,
	open : icc_open,
	release : icc_close
};

/*Local functions*/
static void  FIFO_INC_WRITE_PTR(uint32_t index)
{
	if (ICC_BUFFER[index].fifo_write == MAX_DEPTH-1)
		ICC_BUFFER[index].fifo_write = 0;
	else
		ICC_BUFFER[index].fifo_write++;
}
static void FIFO_INC_READ_PTR(uint32_t index)
{
	if (ICC_BUFFER[index].fifo_read == MAX_DEPTH-1)
		ICC_BUFFER[index].fifo_read = 0;
	else
		ICC_BUFFER[index].fifo_read++;
}
static int FIFO_NOT_EMPTY(uint32_t index)
{
	if ((ICC_BUFFER[index].fifo_read == ICC_BUFFER[index].fifo_write))
		return 0;
	else
		return 1;
}

static int FIFO_AVAILABLE(uint32_t index)
{
	if (ICC_BUFFER[index].fifo_read <= ICC_BUFFER[index].fifo_write)
		return ((ICC_BUFFER[index].fifo_read - ICC_BUFFER[index].fifo_write) + MAX_DEPTH);
	else
		return (ICC_BUFFER[index].fifo_read - ICC_BUFFER[index].fifo_write - 1);
}

static uint32_t get_free_semidx(int CID)
{
	uint32_t idx;

	for (idx = 0; idx < MAX_PERC_MSG; idx++) {
		if(!((iccdev[CID].local_sem_idx >> idx) & 1))
			return idx;
	}

	pr_err("No semidx free for Client :%d\n",CID);
	return -EAGAIN;
}

/*******************************************************************************
Description:
Arguments:
Note:
*******************************************************************************/
void clear_mmap_addr(struct vm_area_struct *vma)
{
	uint32_t i, j, flag;
	flag = 0;
	for (i = 0; i < MAX_CLIENT; i++)
	{
		for (j = 0; j < MAX_MMAP; j++)
		{
			if (mmap_address[i].virtual_addr[j] == (uint32_t)vma->vm_start) {
				mmap_address[i].address[j] = 0;
				mmap_address[i].virtual_addr[j] = 0;
				mmap_address[i].count--;
				flag = 1;
				break;
			}
		}
		if (flag == 1)
		{
			break;
		}
	}
}

uint32_t fetch_userto_kernel_addr(uint32_t num, uint32_t Param)
{
	uint32_t j;
	for (j = 0; j < MAX_MMAP; j++)
	{
		if (mmap_address[num].virtual_addr[j] == Param) {
			return mmap_address[num].address[j];
		}
	}
	return 0xFFFFFFFF;
}


void simple_vma_open(struct vm_area_struct *vma)
{
	vma->vm_pgoff = (uintptr_t)mps_buffer.malloc(MEM_SEG_SIZE, 0xFF);
	vma->vm_pgoff = __pa(vma->vm_pgoff);
	vma->vm_pgoff = vma->vm_pgoff>>PAGE_SHIFT;
	TRACE(ICC, DBG_LEVEL_LOW, ("mmap without giving the physical address, so allocating memory\n"));
	TRACE(ICC, DBG_LEVEL_LOW, (KERN_NOTICE "Simple VMA open, virt %lx, phys pfn %lx\n",
	vma->vm_start, vma->vm_pgoff));
}

void simple_vma_close(struct vm_area_struct *vma)
{
	clear_mmap_addr(vma);
	mps_buffer.free((void *)__va(vma->vm_pgoff<<PAGE_SHIFT));
	TRACE(ICC, DBG_LEVEL_LOW, ("freeing the allocated memory on munmap, if allocated previously"));
	TRACE(ICC, DBG_LEVEL_LOW, (KERN_NOTICE "Simple VMA close.\n"));
}

static struct vm_operations_struct simple_remap_vm_ops = {
	.open =  simple_vma_open,
	.close = simple_vma_close,
};

#ifndef SYSTEM_4KEC
void complex_vma_open(struct vm_area_struct *vma)
{
	vma->vm_pgoff = (uintptr_t)alloc_pages_exact(vma->vm_end - vma->vm_start,GFP_KERNEL|GFP_DMA);
	vma->vm_pgoff = __pa(vma->vm_pgoff);
	vma->vm_pgoff = vma->vm_pgoff>>PAGE_SHIFT;
	TRACE(ICC, DBG_LEVEL_LOW, ("mmap without giving the physical address, so allocating memory\n"));

	TRACE(ICC, DBG_LEVEL_LOW, (KERN_NOTICE "Complex VMA open, virt %lx, phys pfn %lx\n",
	vma->vm_start, vma->vm_pgoff));
}

void complex_vma_close(struct vm_area_struct *vma)
{
	clear_mmap_addr(vma);
	free_pages_exact((void *)(__va(vma->vm_pgoff<<PAGE_SHIFT)), vma->vm_end - vma->vm_start);
	TRACE(ICC, DBG_LEVEL_LOW, ("freeing the allocated memory on munmap, if allocated previously"));
	TRACE(ICC, DBG_LEVEL_LOW, (KERN_NOTICE "Complex VMA close.\n"));
}

static struct vm_operations_struct complex_remap_vm_ops = {
	.open =  complex_vma_open,
	.close = complex_vma_close,
};
#endif

void generic_vma_open(struct vm_area_struct *vma)
{
	TRACE(ICC, DBG_LEVEL_LOW, (KERN_NOTICE "Generic VMA open\n"));
}

void generic_vma_close(struct vm_area_struct *vma)
{
	clear_mmap_addr(vma);
	TRACE(ICC, DBG_LEVEL_LOW, (KERN_NOTICE "Generic VMA close.\n"));
}


static struct vm_operations_struct generic_remap_vm_ops = {
	.open =  generic_vma_open,
	.close = generic_vma_close,
};


int icc_mmap(struct file *file, struct vm_area_struct *vma)
{
	size_t size = vma->vm_end - vma->vm_start;
	unsigned long offset;
	uint32_t num, i;
	offset = vma->vm_pgoff;

	num = (uintptr_t)file->private_data;
	if (mmap_address[num].count >= MAX_MMAP) {
		TRACE(ICC, DBG_LEVEL_HIGH, ("MAX Count %d exceeded for mmap\n", MAX_MMAP));
		return -EAGAIN;
	}

	/*For remap pfn range we have to give the page frame number as the third
	  argument*/
	if (offset == 0) {
#ifdef SYSTEM_4KEC
		if (size > MEM_SEG_SIZE) {
			TRACE(ICC, DBG_LEVEL_HIGH, ("Cant allocate from bootcore memory\n"));
			return -EINVAL;
		}
#else
		if (size > MEM_SEG_SIZE) {
			vma->vm_ops = &complex_remap_vm_ops;
			complex_vma_open(vma);
		} else
#endif
		{
			vma->vm_ops = &simple_remap_vm_ops;
			simple_vma_open(vma);
		}
		offset = vma->vm_pgoff;
	} else {
		vma->vm_ops = &generic_remap_vm_ops;
		generic_vma_open(vma);
	}

	for (i = 0; i < MAX_MMAP; i++) {
		if (mmap_address[num].address[i] == 0) {
			mmap_address[num].address[i] = (uintptr_t)__va(vma->vm_pgoff<<PAGE_SHIFT);
			mmap_address[num].virtual_addr[i] = (uint32_t)vma->vm_start;
			break;
		}
	}
	mmap_address[num].count++;
#ifdef SYSTEM_4KEC
	/*Always uncached access from 4kec, because of caching complexities since cache is VIPT*/
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
#endif

	/* Remap-pfn-range will mark the range VM_IO */
	if (remap_pfn_range(vma,
				vma->vm_start,
				offset,
				size,
				vma->vm_page_prot)) {
		TRACE(ICC, DBG_LEVEL_HIGH, ("remap failed\n"));
		return -EAGAIN;
	}
	return 0;
}

void mps_icc_fill(mps_message *pMpsmsg, icc_msg_t *rw)
{
	int i;
	rw->src_client_id = (uint8_t)pMpsmsg->header.Hd.src_id;
	rw->dst_client_id = (uint8_t)pMpsmsg->header.Hd.dst_id;
	rw->msg_id = (uint8_t)pMpsmsg->header.Hd.msg_id;
	rw->param_attr = (uint8_t)pMpsmsg->header.Hd.param_attr;
	rw->seq_num.icc_seq_num = (uint32_t)pMpsmsg->seq_num.icc_seq_num;
	rw->icc_flags.sem_idx = pMpsmsg->mps_flags.sem_idx;
	rw->icc_flags.icc_sync = pMpsmsg->mps_flags.icc_sync;
	rw->icc_flags.priority = pMpsmsg->mps_flags.priority;

	for (i = 0; i < MAX_UPSTRM_DATAWORDS; i++)
		rw->param[i] = pMpsmsg->data[i];

}

void icc_mps_fill(icc_msg_t *rw, mps_message *pMpsmsg)
{
	int i;

	pMpsmsg->header.Hd.src_id = (uint8_t)rw->src_client_id;
	pMpsmsg->header.Hd.dst_id = (uint8_t)rw->dst_client_id;
	pMpsmsg->header.Hd.msg_id = (uint8_t)rw->msg_id;
	pMpsmsg->header.Hd.param_attr = (uint8_t)rw->param_attr;
	pMpsmsg->seq_num.icc_seq_num = (uint32_t)rw->seq_num.icc_seq_num;
	pMpsmsg->mps_flags.sem_idx = rw->icc_flags.sem_idx;
	pMpsmsg->mps_flags.icc_sync = rw->icc_flags.icc_sync;
	pMpsmsg->mps_flags.priority = rw->icc_flags.priority;

	for (i = 0; i < MAX_UPSTRM_DATAWORDS ; i++)
		pMpsmsg->data[i] = rw->param[i];
}

/*API for read*/
int icc_read(icc_devices icdev, icc_msg_t *rw)
{
	uint32_t num = (uint32_t)icdev;
	int readptr;
	mps_message *pMpsmsg;
	icc_msg_t icc_msg;
	if ((num >= MAX_CLIENT) || (!FIFO_NOT_EMPTY(num)))/*fifo is empty*/
		return -EFAULT;
	readptr = FIFO_GET_READ_PTR(num);
	FIFO_INC_READ_PTR(num);
	pMpsmsg = &GET_ICC_READ_MSG(num, readptr);

	/*Convert data from MPS to ICC*/
	mps_icc_fill(pMpsmsg, rw);
	/*memset the global structure*/
	memset(pMpsmsg, 0, sizeof(mps_message));
	if (FIFO_AVAILABLE(num) >= MAX_THRESHOLD && BLOCK_MSG[num] == 1) {
		memset(&icc_msg, 0, sizeof(icc_msg_t));
		icc_msg.src_client_id = icc_msg.dst_client_id = ICC_Client;
		icc_msg.msg_id = ICC_MSG_FLOW_CONTROL;
		icc_msg.param[0] = 0;
		icc_msg.param[1] = num;
		icc_write(ICC_Client, &icc_msg);
	}

	return sizeof(icc_msg_t);
}
EXPORT_SYMBOL(icc_read);


/*ioctl functions supported by icc*/
long icc_ioctl(struct file *file_p, uint32_t nCmd, unsigned long arg)
{
	uint32_t num, i;
	icc_commit_t icc_address;

	switch (nCmd) {
		case ICC_IOC_REG_CLIENT:
			num = (int)arg;
			/*check for validity of client Id*/
			if (num >= 0 && num < (MAX_CLIENT)) {
				/*check if its already opened*/
				if (iccdev[num].Installed == 1) {
					TRACE(ICC, DBG_LEVEL_HIGH, (" Device %d is already open!\n", num));
					return -EMFILE;
				}
				/*mark that the device is opened in global structure
				  to avoid further open of the device*/
				iccdev[num].Installed = 1;
#ifdef CONFIG_SOC_GRX500_BOOTCORE
				/*Intimate to IAP that bootcore is ready if IA gets registered*/
				if (num == IA)
				{
					mps_message Mpsmsg;

					memset(&Mpsmsg, 0, sizeof(mps_message));
					Mpsmsg.header.Hd.src_id = Mpsmsg.header.Hd.dst_id = 0;
					Mpsmsg.header.Hd.msg_id = ICC_BOOTCORE_UP;
					/*write to the mps mailbox*/
					mps_write_mailbox(&Mpsmsg);
				}
#endif
				/* Initialize a wait_queue list for the system poll/wait_event function. */
				init_waitqueue_head(&iccdev[num].wakeuplist);
				file_p->private_data = (void *)(uintptr_t)num;
			}
			break;
		case ICC_IOC_MEM_INVALIDATE:
		case ICC_IOC_MEM_COMMIT:
#ifdef SYSTEM_4KEC
			/*As the 4kec is uncached access no need of cache flushing, but for safety do a sync*/
			__asm__ __volatile__(" sync \n");
			return 0;
#endif
			num = (uintptr_t)file_p->private_data;

			/* Initialize destination and copy mps_message from usermode */
			memset (&icc_address, 0, sizeof (icc_commit_t));
			if (0 < copy_from_user (&icc_address, (void *) arg, sizeof (icc_commit_t))) {
				TRACE(ICC, DBG_LEVEL_HIGH, ("[%s %s %d]: copy_from_user error\r\n",
							__FILE__, __func__, __LINE__));
				return -EFAULT;
			}

			for (i = 0; i < icc_address.count; i++) {
				uint32_t mmap_addr;

				mmap_addr = fetch_userto_kernel_addr(num, icc_address.address[i]);

				if (mmap_addr == 0xFFFFFFFF) {
					mmap_addr = icc_address.address[i];
				}

				icc_address.address[i] = mmap_addr+icc_address.offset[i];
#ifdef SYSTEM_ATOM
				if (nCmd == ICC_IOC_MEM_COMMIT)
					clflush_cache_range((void *)(uintptr_t)icc_address.address[i], icc_address.length[i]);
#else
				if (nCmd == ICC_IOC_MEM_COMMIT)
					cache_wb_inv(icc_address.address[i], icc_address.length[i]);
				else
					cache_inv(icc_address.address[i], icc_address.length[i]);
#endif
			}
			break;
	}
	return 0;
}


/*read call back function registered with driver */
int icc_read_d(struct file *file_p, char *buf, size_t count, loff_t *ppos)
{
	uint32_t num = (uintptr_t)file_p->private_data;/*Accessing the stored client number
							 in file data*/
	icc_msg_t icc_msg;
	mps_message *pMpsmsg;
	int readptr;

	if (!FIFO_NOT_EMPTY(num))/*fifo is empty*/
		return -EFAULT;

	readptr = FIFO_GET_READ_PTR(num);
	FIFO_INC_READ_PTR(num);
	pMpsmsg = &GET_ICC_READ_MSG(num, readptr);
	memset(&icc_msg, 0, sizeof(icc_msg_t));
	/*Convert data from MPS to ICC*/
	mps_icc_fill(pMpsmsg, &icc_msg);

	if (0 < copy_to_user (buf, &icc_msg, sizeof(icc_msg_t))) {
		TRACE(ICC, DBG_LEVEL_HIGH, ("[%s %s %d]: copy_to_user error\r\n", __FILE__, __func__, __LINE__));
		return -EAGAIN;
	}

	/*memset the global structure*/
	memset(pMpsmsg, 0, sizeof(mps_message));
	if (FIFO_AVAILABLE(num) >= MAX_THRESHOLD && BLOCK_MSG[num] == 1) {
		memset(&icc_msg, 0, sizeof(icc_msg_t));
		icc_msg.src_client_id = icc_msg.dst_client_id = ICC_Client;
		icc_msg.msg_id = ICC_MSG_FLOW_CONTROL;
		icc_msg.param[0] = 0;
		icc_msg.param[1] = num;
		icc_write(ICC_Client, &icc_msg);
	}

	return sizeof(icc_msg_t);
}

/* regmap read function */
int icc_regmap_sync_read(phys_addr_t paddr, unsigned int *val)
{
	icc_msg_t *retmsg, msg;
	int ret;

	if (!val)
		return -1;

	msg.src_client_id = RM;
	msg.dst_client_id = RM;
	msg.msg_id = REGMAP_RD_MSGID;
	msg.param[0] = (uint32_t)paddr;

	retmsg = icc_sync_write(RM, &msg);
	if (!retmsg || retmsg->dst_client_id != RM)
		return -EINVAL;

	ret = retmsg->param_attr;
	if (!ret)
		*val = retmsg->param[1];

	kfree(retmsg);

	return ret;
}
EXPORT_SYMBOL(icc_regmap_sync_read);

/* regmap write function */
int icc_regmap_sync_write(phys_addr_t paddr, unsigned int val)
{
	icc_msg_t *retmsg, msg;
	int ret;

	msg.src_client_id = RM;
	msg.dst_client_id = RM;
	msg.msg_id = REGMAP_WR_MSGID;
	msg.param[0] = (uint32_t)paddr;
	msg.param[1] = val;

	retmsg = icc_sync_write(RM, &msg);
	if (!retmsg || retmsg->dst_client_id != RM)
		return -EINVAL;
	ret = retmsg->param_attr;
	kfree(retmsg);
	return ret;
}
EXPORT_SYMBOL(icc_regmap_sync_write);

/* regmap update bits function */
int icc_regmap_sync_update_bits(phys_addr_t paddr, unsigned int mask,
				unsigned int val)
{
	icc_msg_t *retmsg, msg;
	int ret;

	msg.src_client_id = RM;
	msg.dst_client_id = RM;
	msg.msg_id = REGMAP_UB_MSGID;
	msg.param[0] = (uint32_t)paddr;
	msg.param[1] = mask;
	msg.param[2] = val;

	retmsg = icc_sync_write(RM, &msg);
	if (!retmsg || retmsg->dst_client_id != RM)
		return -EINVAL;

	ret = retmsg->param_attr;
	kfree(retmsg);

	return ret;
}
EXPORT_SYMBOL(icc_regmap_sync_update_bits);

int icc_msc_noc_firewall_sync_cfg(unsigned int param0,
				unsigned long long loaddr,
				unsigned long long hiaddr,
				unsigned int param3)
{
	icc_msg_t *retmsg = NULL, msg;
	int ret;

	msg.src_client_id = ND;
	msg.dst_client_id = ND;
	msg.msg_id = ICC_MSG_NOC_FIREWALL_WRITE;
	msg.param[0] = param0;
	msg.param[1] = loaddr >> 4;
	msg.param[2] = hiaddr >> 4;
	msg.param[3] = param3;
	retmsg = icc_sync_write(ND, &msg);
	if (!retmsg || retmsg->dst_client_id != ND) {
	       pr_err("%s icc_sync_write fail \n",__func__);
		return -EINVAL;
	}
	ret = retmsg->param_attr;
	kfree(retmsg);
	return ret;
}
EXPORT_SYMBOL(icc_msc_noc_firewall_sync_cfg);

int icc_msc_noc_firewall_check_access(unsigned int param0,
				      unsigned long long loaddr,
				      unsigned long long hiaddr,
				      unsigned int param3)
{
	icc_msg_t *retmsg = NULL, msg;
	int ret;

	msg.src_client_id = ND;
	msg.dst_client_id = ND;
	msg.msg_id = ICC_MSG_NOC_FIREWALL_CHECK_ACCESS;
	msg.param[0] = param0;
	msg.param[1] = loaddr >> 4;
	msg.param[2] = hiaddr >> 4;
	msg.param[3] = param3;

	retmsg = icc_sync_write(ND, &msg);
	if (!retmsg || retmsg->dst_client_id != ND) {
	       pr_err("%s icc_sync_write fail \n", __func__);
		return -EINVAL;
	}
	ret = retmsg->param_attr;
	kfree(retmsg);
	return ret;
}
EXPORT_SYMBOL(icc_msc_noc_firewall_check_access);

int icc_msc_noc_firewall_cfg(unsigned int param0,
				unsigned long param1,
				unsigned int param2,
				unsigned int param3)
{
	icc_msg_t *retmsg = NULL, msg;
	int ret;

	msg.src_client_id = ND;
	msg.dst_client_id = ND;
	msg.msg_id = ICC_MSG_NOC_FIREWALL_ENABLE_DISABLE;
	msg.param[0] = param0;
	msg.param[1] = param1;
	msg.param[2] = param2;
	msg.param[3] = param3;

	retmsg = icc_sync_write(ND, &msg);
	if (!retmsg || retmsg->dst_client_id != ND) {
	       pr_err("%s icc_sync_write fail \n", __func__);
		return -EINVAL;
	}
	ret = retmsg->param_attr;
	kfree(retmsg);
	return ret;
}
EXPORT_SYMBOL(icc_msc_noc_firewall_cfg);

/*******************************************************************************
Description:
Arguments:
Note:
*******************************************************************************/
int icc_write(icc_devices icdev, icc_msg_t *buf)
{
	mps_message Mpsmsg;
	uint16_t cli_d = (uint16_t)icdev;
	int ret = 0;

	if (!buf || buf->src_client_id != cli_d) {
		TRACE(ICC, DBG_LEVEL_HIGH,
		("icc_msg_t is NULL or src_client_id and icdev not match\n"));
		return ret;
	}

        if (cli_d >= MAX_CLIENT) {
		TRACE(ICC, DBG_LEVEL_HIGH, ("Client out of Range !! \n"));
		return ret;
        }

	memset(&Mpsmsg, 0, sizeof(mps_message));
	/*Fill in the mps data structure*/
	icc_mps_fill(buf, &Mpsmsg);
	{
		/*write to the mps mailbox*/
		ret = mps_write_mailbox(&Mpsmsg);
		if (ret == -EIO) {
			TRACE(ICC, DBG_LEVEL_HIGH,
				("Peer side not responding, needs reboot\n"));
			BLOCK_MSG[buf->src_client_id] = 1;
		}
		/*release the semaphore of data mailbox after writing*/
	}
	if (ret < 0)
		return ret;
	else
  		return sizeof(icc_msg_t);
}
EXPORT_SYMBOL(icc_write);

uint32_t icc_get_seqnum(uint32_t CID)
{
	unsigned long flags;

	spin_lock_irqsave(&g_icc_semidx_lock, flags);
	iccdev[CID].local_seq_num = iccdev[CID].local_seq_num + 1;
	if (iccdev[CID].local_seq_num > (USHRT_MAX-1))
		iccdev[CID].local_seq_num = 1;
	spin_unlock_irqrestore(&g_icc_semidx_lock, flags);
	return iccdev[CID].local_seq_num;
}

uint32_t icc_get_semidx(uint32_t CID)
{
	unsigned long flags;
	uint32_t idx = 0;

	spin_lock_irqsave(&g_icc_semidx_lock, flags);
	idx = get_free_semidx(CID);
	if (idx >= 0 )
		iccdev[CID].local_sem_idx |= (1 << idx);
	spin_unlock_irqrestore(&g_icc_semidx_lock, flags);
	return idx;
}

void icc_put_semidx(uint32_t CID, uint32_t sem_idx)
{
	unsigned long flags;

	if (sem_idx < 0)
		return;
	spin_lock_irqsave(&g_icc_semidx_lock, flags);
	iccdev[CID].local_sem_idx &= (~(1 << sem_idx));
	spin_unlock_irqrestore(&g_icc_semidx_lock, flags);
}

/*******************************************************************************
Description:
Arguments:
Note:
*******************************************************************************/
icc_msg_t *icc_sync_write(icc_devices icdev, icc_msg_t *buf)
{
	mps_message Mpsmsg;
	icc_msg_t *rx_icc = NULL;
	uint16_t CID = (uint16_t)icdev;
	int ret, sem_idx;
	int i = 0;

	if (!buf || buf->src_client_id != CID) {
                TRACE(ICC, DBG_LEVEL_HIGH,
		("icc_msg_t is NULL or src_client_id and icdev not matching !! \n"));
		return NULL;
	}

	if (CID >= MAX_CLIENT) {
		TRACE(ICC, DBG_LEVEL_HIGH, ("Client out of Range !! \n"));
		return NULL;
	}

	memset(&Mpsmsg, 0, sizeof(mps_message));

	sem_idx = (uint8_t)icc_get_semidx(CID);
	if (sem_idx < 0) {
		pr_err("Free semidx for client : %d not found\n",CID);
		return NULL;
	}
	buf->icc_flags.sem_idx = sem_idx;
	buf->seq_num.icc_seq_num = icc_get_seqnum(CID);
	buf->icc_flags.icc_sync = 1;
	buf->icc_flags.priority = 1;
	/*Fill in the mps data structure*/
	icc_mps_fill(buf, &Mpsmsg);
	{
		/*write to the mps mailbox*/
		ret = mps_write_mailbox(&Mpsmsg);
		if (ret == -EIO) {
			TRACE(ICC, DBG_LEVEL_HIGH,
			("Peer side not responding, needs reboot\n"));
			BLOCK_MSG[buf->src_client_id] = 1;
			icc_put_semidx(buf->src_client_id, sem_idx);
			return NULL;
		}
	}

	if (ret < 0) {
		TRACE(ICC, DBG_LEVEL_HIGH, ("icc_sync_write ret < 0\n"));
		icc_put_semidx(buf->src_client_id, sem_idx);
		return NULL;
	}

	if(!irqs_disabled()) {
		ret = wait_event_interruptible_timeout(
			iccdev[CID].csem[sem_idx].perC_wakeuplist,
				(refcount_read(&icc_excpt[CID]) != 0), (100 * HZ));
	} else {
		ret = poll_mps_mbx_upstream(&Mpsmsg, buf->icc_flags.sem_idx,
				buf->seq_num.icc_seq_num, buf->src_client_id);
		icc_put_semidx(buf->src_client_id, sem_idx);
	}

	if (ret < 0) {
		icc_put_semidx(CID, sem_idx);
		TRACE(ICC, DBG_LEVEL_HIGH, ("Peer side not responding\n"));
		return NULL;
	}

	if(!irqs_disabled()) {
		for (i = 0; ((refcount_read(&icc_excpt[i]) == 0) && (i < MAX_CLIENT)); i++);

		refcount_set(&icc_excpt[i], 0);
		icc_put_semidx(i, sem_idx);
		rx_icc = kzalloc(sizeof(icc_msg_t), GFP_ATOMIC);
		ret = icc_read(i, rx_icc);
		if (ret < 0) {
			pr_err("Read from ICC client %d failed\n", i);
			return NULL;
		}
	} else {
		rx_icc = kzalloc(sizeof(icc_msg_t), GFP_ATOMIC);
		mps_icc_fill(&Mpsmsg, rx_icc);
	}

	return rx_icc;
}
EXPORT_SYMBOL(icc_sync_write);

/*write call back function of the driver*/
int icc_write_d(struct file *file_p, char *buf, size_t count, loff_t *ppos)
{
	mps_message Mpsmsg;
	icc_msg_t icc_msg;
	int ret , i;
	uint32_t num;
	memset(&Mpsmsg, 0, sizeof(mps_message));
	memset(&icc_msg, 0, sizeof(icc_msg_t));
	if (0 < copy_from_user(&icc_msg, buf, sizeof(icc_msg_t)))
  {
		TRACE(ICC, DBG_LEVEL_HIGH, ("[%s %s %d]: copy_from_user error\r\n", __FILE__, __func__, __LINE__));
		return -EAGAIN;
  }
	num = (uintptr_t)file_p->private_data;
	for (i = 0; i < MAX_UPSTRM_DATAWORDS; i++) {
		if (icc_msg.param[i] != 0 && CHECK_PTR(icc_msg.param_attr, i) &&
				!(CHECK_PTR_IOCU(icc_msg.param_attr, i))) {
				uint32_t mmap_addr;
				mmap_addr = fetch_userto_kernel_addr(num, icc_msg.param[i]);
				if (mmap_addr != 0xFFFFFFFF) {
					icc_msg.param[i] = mmap_addr;
					TRACE(ICC, DBG_LEVEL_NORMAL, ("user pointer maintained by driver\n"));
				}
		}
	}
	/*Fill in the mps data structure*/
	icc_mps_fill(&icc_msg, &Mpsmsg);
	{
		/*write to mps mailbox*/
		ret = mps_write_mailbox(&Mpsmsg);
		if (ret == -EIO)
		{
			TRACE(ICC, DBG_LEVEL_HIGH, ("Peer side not responding, needs reboot\n"));
			BLOCK_MSG[icc_msg.src_client_id] = 1;
		}
	}
	if (ret < 0)
		return ret;
	else
  	return sizeof(icc_msg_t);
}

/*poll function handler of the driver*/
unsigned int icc_poll(struct file *file_p, poll_table *wait)
{
	int ret = 0;
	uint32_t num = (uintptr_t)file_p->private_data;
	icc_dev *pIccdev;
	pIccdev = &iccdev[num];
	/* install the poll queues of events to poll on */
	poll_wait(file_p, &pIccdev->wakeuplist, wait);
	/*If exception flag is set unlock bot read and write fd*/
	if (refcount_read(&icc_excpt[num]) == 1) {
		if (FIFO_NOT_EMPTY(num))
			ret |= (POLLIN | POLLRDNORM);
		if (BLOCK_MSG[num] == 0)
			ret |= (POLLOUT | POLLWRNORM);
		refcount_set(&icc_excpt[num], 0);
		return ret;
	}
	return ret;
}

/*register the char region with the linux*/
int icc_os_register (void)
{
	int ret = 0;
	ret = register_chrdev(icc_major_id, icc_dev_name, &icc_fops);
	if (ret < 0)
	{
		TRACE(ICC, DBG_LEVEL_HIGH, ("Not able to register chrdev\n"));
		return ret;
	} else if (icc_major_id == 0) {
		icc_major_id = ret;
	}
	TRACE(ICC, DBG_LEVEL_HIGH, ("Major Id is %d\n", icc_major_id));
	TRACE(ICC, DBG_LEVEL_HIGH, ("ICC driver registered\n"));
	return ret;
}

/**
   This function unregisters char device from kernel.
*/
void icc_os_unregister (void)
{
	 /*unregister the driver region completely*/
   unregister_chrdev(icc_major_id, icc_dev_name);
	 TRACE(ICC, DBG_LEVEL_HIGH, ("ICC driver un-registered\n"));
}

/*Global Functions*/
/**
 * Open ICC device.
 * Open the device from user mode (e.g. application) or kernel mode. An inode
 * value of 1..MAX_CLIENT-1 indicates a kernel mode access. In such a case the inode value
 * is used as minor ID.
 *
 * \param   inode   Pointer to device inode
 * \param   file_p  Pointer to file descriptor
 * \return  0       device opened
 * \return  EMFILE  Device already open
 * \return  EINVAL  Invalid minor ID
 * \ingroup API
 */
int icc_open (struct inode * inode, struct file * file_p)
{
	int from_kernel = 0;
	uint32_t  num, i;
	/* a trick: if inode value is
	   [1..MAX_CLIENT-1], then we make sure that we are calling from
	   kernel space. */
	if (((uintptr_t) inode >= 0) &&
			((uintptr_t) inode < (MAX_CLIENT))) {
		from_kernel = 1;
		num = (uintptr_t) inode;
	}
	else
	{
		return 0;        /* the real device */
	}
	/*check for validity of client Id*/
	if (num >= 0 && num < (MAX_CLIENT)) {
		/*check if its already opened*/
		if (iccdev[num].Installed == 1) {
			TRACE(ICC, DBG_LEVEL_HIGH, (" Device %d is already open!\n", num));
			return -EMFILE;
		}
		/*mark that the device is opened in global structure
		  to avoid further open of the device*/
		iccdev[num].Installed = 1;
		/* Initialize a wait_queue list for the system poll/wait_event function. */
		init_waitqueue_head(&iccdev[num].wakeuplist);

		for (i=0 ; i < MAX_PERC_MSG; i++) {
			init_waitqueue_head(&iccdev[num].csem[i].perC_wakeuplist);
		}
	} else {
		TRACE(ICC, DBG_LEVEL_HIGH, ("Max device number exceeded\n"));
		return -EINVAL;
	}
	return 0;
}
EXPORT_SYMBOL(icc_open);

/**
 * Close ICC device.
 * Close the device from user mode (e.g. application) or kernel mode. An inode
 * value of 1..MAX_CLIENT-1 indicates a kernel mode access. In such a case the inode value
 * is used as minor ID.
 *
 * \param   inode   Pointer to device inode
 * \param   file_p  Pointer to file descriptor

 * \return  0       device closed
 * \return  ENODEV  Device invalid
 * \return  EINVAL  Invalid minor ID
 * \ingroup API
 */
int icc_close(struct inode * inode, struct file * file_p)
{
	int from_kernel = 0;
	uint32_t num;
	/* a trick: if inode value is
	   [1..MAX_CLIENT-1], then we make sure that we are calling from
	   kernel space. */
	if (((uintptr_t) inode > 0) && ((uintptr_t) inode <= (MAX_CLIENT-1))) {
		from_kernel = 1;
		num = (uintptr_t) inode;
	} else
	{
		num = (uintptr_t)file_p->private_data;        /* the real device */
	}
	/*check for valid client Id and whether it is installed already or not*/
	if (num >= 0 && num < (MAX_CLIENT)) {
		if (iccdev[num].Installed == 1) {
			/*Flush all the messages*/
			//memset(&MPS_BUFFER[num],0,sizeof(mps_message));
			/*Mark the global structure that its free to open now*/
			iccdev[num].Installed = 0;
			return 0;
		}
	}

	TRACE(ICC, DBG_LEVEL_HIGH, ("Invalid device Id %d\n", num+1));
	return -ENODEV;
}
EXPORT_SYMBOL(icc_close);


void process_icc_message(unsigned int uiClientId, bool sync, uint32_t msg_id)
{
#ifndef KTHREAD
	iccd_t *icc_workq;
#endif
#ifndef KTHREAD
			if (icc_wq[uiClientId]) {
				icc_workq = (iccd_t *)kmalloc(sizeof(iccd_t), GFP_ATOMIC);
				if (icc_workq) {
					INIT_DELAYED_WORK((struct delayed_work *)icc_workq, icc_wq_function);
					icc_workq->x = uiClientId;
					icc_workq->s = sync;
					icc_workq->m = msg_id;
					queue_delayed_work(icc_wq[uiClientId], (struct delayed_work *)icc_workq, (uiClientId + msg_id));
				}
				else {
					TRACE(ICC, DBG_LEVEL_HIGH, ("Allocation failed cant schedule icc wq %d\n", uiClientId));
					return;
				}
			}
#endif

}

void pfn_icc_callback(void)
{
	int ret;
	unsigned int uiClientId;
	unsigned long wrptr ;
	mps_message rw;
	mps_message *mps_msg;
	uiClientId = 0;
	memset(&rw, 0, sizeof(mps_message));
	ret = mps_read_mailbox(&rw);

	if (ret == 0) {
		uiClientId = rw.header.Hd.dst_id;
		if (iccdev[uiClientId].Installed == 0) {
			TRACE(ICC, DBG_LEVEL_HIGH, ("client Id %d not opened yet\n", uiClientId));
			return;
		}

		if (FIFO_AVAILABLE(uiClientId) > 0) {
			wrptr = FIFO_GET_WRITE_PTR(uiClientId);
			FIFO_INC_WRITE_PTR(uiClientId);

			memcpy(&GET_ICC_WRITE_MSG(uiClientId, wrptr), &rw, sizeof(mps_message));

			process_icc_message(uiClientId, rw.mps_flags.icc_sync, rw.mps_flags.sem_idx);

			/*Flow control logic*/
			/*since we cant write in interrupt context schedule it and write*/
			/*To avoid potential problems disable interrupts till we send this message out*/
			if (FIFO_AVAILABLE(uiClientId) <= (MIN_THRESHOLD) && BLOCK_MSG[uiClientId] == 0) {
				wrptr = FIFO_GET_WRITE_PTR(ICC_Client);
				FIFO_INC_WRITE_PTR(ICC_Client);
				mps_msg = &GET_ICC_WRITE_MSG(ICC_Client, wrptr);
				memset(mps_msg, 0, sizeof(mps_message));
				mps_msg->header.Hd.src_id = uiClientId;
				mps_msg->header.Hd.dst_id = ICC_Client;
				mps_msg->header.Hd.msg_id = ICC_MSG_FLOW_CONTROL;
				mps_msg->data[0] = 1;
				mps_msg->data[1] = uiClientId;
				process_icc_message(ICC_Client, 0, 0);
			}
		} else {
			TRACE(ICC, DBG_LEVEL_HIGH, ("Fifo full  for client %d \n", uiClientId));
			return;
		}

	} else {
		TRACE(ICC, DBG_LEVEL_HIGH, ("Mailbox read error is %d\n", ret));
	}
}

/* Dummy Callback function initialized to all client calling ICC from Kernel */
void pfn_client_cb_dummy(icc_wake_type wake_type) {
	return;
}

/*Init module routine*/
int icc_init_module (void) {
	int result = 0, i;

	result = mps_open((struct inode *)1, NULL);
	if (result < 0) {
		TRACE(ICC, DBG_LEVEL_HIGH, ("open MPS2 Failed\n"));
		return result;
	}

	result = mps_register_callback(&pfn_icc_callback);
	if (result < 0) {
		TRACE(ICC, DBG_LEVEL_HIGH, ("Data CallBack Register with MPS2 Failed\n"));
		return result;
	}

	/*Init structures if required*/
	/* register char module in kernel */
	result = icc_os_register();
	if (result < 0)
		return result;

	for (i = 1; i < ARRAY_SIZE(ssd_clients); i++) {
		if (!ssd_clients[i].ssd && ssd_clients[i].name != NULL) {
			/* initailze icc_open and icc_callback_register */
			/* for Non SSD drivers */
			result = icc_open((struct inode *)(uintptr_t)i , NULL);
			if (result < 0) {
				TRACE(ICC, DBG_LEVEL_HIGH,
			("open ICC Failed for client %d \n", i));
				continue;
			}

			result = icc_register_callback(i, &pfn_client_cb_dummy);
			if (result < 0) {
				TRACE(ICC, DBG_LEVEL_HIGH,
			("CallBack Register Failed for Client %d \n", i));
				continue;
			}
		}
	}

#ifdef KTHREAD
	sema_init(&icc_callback_sem, 1);
#endif
	return result;
}
EXPORT_SYMBOL(icc_init_module);

/*Exit module routine*/
void __exit icc_cleanup_module (void) {
	mps_close((struct inode *)1, NULL);
	mps_unregister_callback();
	icc_os_unregister ();
	return;
}
#ifdef KTHREAD
/*Common thread handler function for the ICC kernel threads*/
static int icc_thread_handler(void *arg)
{
	unsigned int num;
	icc_wake_type wake_type;
	/*Take the global variable of thread number into local variable*/
	num = g_num;
	/*Release the call back semaphore as other callback can now update thread number*/
	up(&icc_callback_sem);
	/*check whether the thread has to be stopped*/
	/*if not*/
	while (!kthread_should_stop()) {
		TRACE(ICC, DBG_LEVEL_LOW, ("thread %u going to wait\n", num));
		/*wait for an event to occur and check for exception condition*/
		wait_event_interruptible(iccdev[num].wakeuplist, (refcount_read(&icc_excpt[num]) != 0));
		/*reset exception variable*/
		refcount_set(&icc_excpt[num], 0);
		/*check the whether the wake up event is for stopping the thread*/
		if (kthread_should_stop())
			break;
		/*if not for stopping thread set current state to running and
		  invoke callback*/
		__set_current_state(TASK_RUNNING);
		TRACE(ICC, DBG_LEVEL_LOW, ("Thread %u woken up\n", num));
		/*calling call back to process data*/
		/*complete functionality inside the call back will now run in
		  kernel thread context*/
		wake_type = ICC_INVALID;
		if (BLOCK_MSG[num] == 0)
			wake_type |= ICC_WRITE;
		if (FIFO_NOT_EMPTY(num))
			wake_type |= ICC_READ;
		iccdev[num].up_callback(wake_type);
	}
	/*if thread stop is set, set current state to running and exit
	  from thread*/
	__set_current_state(TASK_RUNNING);

	return 0;
}
#else
void icc_wq_function(struct work_struct *work)
{
	iccd_t *icc_work = (iccd_t *)work;
	int num = icc_work->x;
	bool i_sync = icc_work->s;
	int sem_idx = icc_work->m;

	icc_wake_type wake_type;

	wake_type = ICC_INVALID;
	if (BLOCK_MSG[num] == 0)
		wake_type |= ICC_WRITE;
	if (FIFO_NOT_EMPTY(num)) {
		wake_type |= ICC_READ;
	}

	if (i_sync == 1) {
		refcount_set(&icc_excpt[num], 1);
		wake_up_interruptible(&iccdev[num].csem[sem_idx].perC_wakeuplist);
	} else {
		iccdev[num].up_callback(wake_type);
	}

	kfree((void *)icc_work);
	return;
}
#endif
/**
 * Register callback.
 * Allows the upper layer to register a callback function  *
 * \param   type
 * \param   callback Callback function to register
 * \return  0        callback registered successfully
 * \return  ENXIO    Wrong device entity
 * \return  EBUSY    Callback already registered
 * \return  EINVAL   Callback parameter null
 * \ingroup API
 */
int icc_register_callback (icc_devices type, void(*callback) (icc_wake_type))
{
	unsigned int num = type;
	char cThreadName[20] = {0};
	/*check that call back is not null*/
	if (callback == NULL) {
		return (-EINVAL);
	}
	/* check validity of device number */
	if (type > (MAX_CLIENT-1))
		return (-ENXIO);
	/*check whether the call back is already registered*/
	if (iccdev[num].up_callback != NULL) {
		return (-EBUSY);
	} /*if not registered*/
	else {
		/*Assign the call back to global structure*/
		iccdev[num].up_callback = callback;
		/*Make the thread name with client Id as postfix*/
#ifdef KTHREAD
		sprintf(cThreadName, "ICCThread%d", type);
#else
		sprintf(cThreadName, "ICCWQ%d", type);
#endif
#ifdef KTHREAD
		/*Lock the call back semaphore, so that subsequent registration
		  of call back will succeed only after releasing this in kernel thread creation*/
		if (down_interruptible(&icc_callback_sem))
			return -EBUSY;
		/*take the thread number to a global variable*/
		g_num = num;
		/*Store the task_struct into global structure, we can use it for stopping the thread*/
		icc_kthread[num] = kthread_run(icc_thread_handler,
				NULL, cThreadName);
		/*If thread creation fails*/
		if (icc_kthread[num] < 0) {
			TRACE(ICC, DBG_LEVEL_HIGH, ("Thread creation failed un-registering callback for %d\n", num+1));
			/*Assign global variable as null*/
			icc_kthread[num] = NULL;
			/*point call back to null*/
			iccdev[num].up_callback = NULL;
			/*release the call back semaphore*/
			up(&icc_callback_sem);
			return -ENOMEM;
		}

#else
		icc_wq[num] = create_workqueue(cThreadName);
		/*If thread creation fails*/
		if (icc_wq[num] < 0) {
			TRACE(ICC, DBG_LEVEL_HIGH, ("WQ creation failed for client %d un-registering callback\n", num+1));
			/*Assign global variable as null*/
			icc_wq[num] = NULL;
			/*point call back to null*/
			iccdev[num].up_callback = NULL;
			return -ENOMEM;
		}

#endif
	}
	return 0;
}
EXPORT_SYMBOL(icc_register_callback);

/**
 * UnRegister callback.
 * Allows the upper layer to unregister a callback function
 * \param   type
 * \param   callback Callback function to register
 * \return  0        callback registered successfully
 * \return  ENXIO    Wrong device entity
 * \return  EINVAL   nothing to unregister
 * \ingroup API
 */
int icc_unregister_callback(icc_devices type)
{
	unsigned int num = type;
	/* check validity of register type */
	if (type > (MAX_CLIENT-1))
		return (-ENXIO);
	/*check whether the call back is registered or not*/
	if (iccdev[num].up_callback == NULL) {
		return (-EINVAL);
	}
	else {
		/*Make the call back null*/
		iccdev[num].up_callback = NULL;
#ifdef KTHREAD
		/*set the exception flag on device, condition after wake*/
		refcount_set(&icc_excpt[num], 1)
		/*stop the kthread*/
		kthread_stop(icc_kthread[num]);
		/*After kthread returns make the global task struct null*/
		icc_kthread[num] = NULL;
#else
		flush_workqueue(icc_wq[num]);
		destroy_workqueue(icc_wq[num]);
#endif
	}
	return 0;
}
EXPORT_SYMBOL(icc_unregister_callback);

/*Module related definitions*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Swaroop Sarma");
MODULE_PARM_DESC(icc_major_id, "Major ID of device");
module_param(icc_major_id, short, 0);
/*module functions of ICC driver*/

/*
 * ICC driver & MPS driver are very tightly coupled and depend on strict
 * ordering of their initialization. If there is any delay between these
 * two driver inits, then it may cause random errors/crashes if in between
 * some core msg is sent via MPS mailbox fifo or any core ICC msg transfer.
 * Hence, it does not make sense to have separate independent inits for ICC
 * & MPS. Instead forcing the init order by calling ICC init from MPS driver
 * init module.
 */
/* subsys_initcall(icc_init_module); */

module_exit(icc_cleanup_module);
