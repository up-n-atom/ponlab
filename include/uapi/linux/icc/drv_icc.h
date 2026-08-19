#ifndef _DRV_UAPI_ICC_H
#define _DRV_UAPI_ICC_H
/******************************************************************************

		Copyright (c) 2017 - 2019 Intel Corporation
		Copyright (c) 2011 - 2016 Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

****************************************************************************
   Module      : drv_icc.h
   Description : This file contains the defines, the structures declarations
		the tables declarations and the global functions declarations.
*******************************************************************************/
#include <linux/fs.h>
#include "drv_mps.h"
/* ============================= */
/* ICC Common defines            */
/* ============================= */
/*---------------------------------------------------------------------------*/
#define MAX_MMAP 16
#define MAX_DEPTH 16
#define MAX_PERC_MSG 16
#define MIN_THRESHOLD 4
#define MAX_THRESHOLD 12
#define ICC_MSG_FLOW_CONTROL 0x1
#define ICC_REMOTE_MEM_FREE			0x7D
#define ICC_REMOTE_MEM_ALLOC		0x7E
#define ICC_REMOTE_MEM_ALLOC_REPLY	0xFE
#define SEC_STORE_ID 0x3
#define ICC_MSG_NOC_FIREWALL_WRITE 0
#define ICC_MSG_NOC_FIREWALL_CHECK_ACCESS 1
#define ICC_MSG_NOC_FIREWALL_GET_EFF_SAI 2
#define ICC_MSG_NOC_FIREWALL_CFG_DUMP 3
#define ICC_MSG_NOC_FIREWALL_ENABLE_DISABLE 4
#define ICC_MSG_TEP_ADAPTER_CFG 5
#define BIT_SHIFT 4
#define NODE_PATH "socpool/icc_pool"


/*DebugInfo*/
#define DBG_LEVEL_OFF      4
#define DBG_LEVEL_HIGH     3
#define DBG_LEVEL_NORMAL   2
#define DBG_LEVEL_LOW      1
#define GET_DMA_ADDR(addr)		(addr | (0xCUL << 32))

/* For LGM Secure services ICC Client driver */
#define ICC_SECURE_SERVICE	1

#define CREATE_TRACE_GROUP(name) unsigned int G_nTraceGroup##name = DBG_LEVEL_HIGH
#define DECLARE_TRACE_GROUP(name) extern unsigned int G_nTraceGroup##name
#define PRINTF printk
#define TRACE(name, level, message) do { if (level >= G_nTraceGroup##name) \
      { PRINTF message ; } } while (0)

/*---------------------------------------------------------------------------*/
/* Device connection structure                                               */
/*---------------------------------------------------------------------------*/

typedef struct{
        unsigned int icc_seq_num:16;
        unsigned int app_seq_num:16;
} icc_msg_seq_num_t;

typedef struct {
	unsigned int sem_idx:4;
	unsigned int icc_sync:1; /* indicates Synchronous message or not */
	unsigned int priority:1; /* Valid only for async messages Icc priority queue/capability*/
	unsigned int reserved:26;
} icc_flags_t;

typedef struct {
	uint8_t src_client_id;
	uint8_t dst_client_id;
	uint8_t msg_id;
	uint8_t param_attr;
	icc_msg_seq_num_t seq_num;  /* ICC sequence number */
	icc_flags_t icc_flags;
	uint32_t param[MAX_UPSTRM_DATAWORDS];
} icc_msg_t;

typedef struct {
	uint32_t address[MAX_UPSTRM_DATAWORDS];
	uint32_t length[MAX_UPSTRM_DATAWORDS];
	uint32_t offset[MAX_UPSTRM_DATAWORDS];
	uint32_t count;
} icc_commit_t;

typedef enum {
	ICC_CLIENT_ID0_NR,
	ICC_SEC_UPGRADE_NR,
	ICC_SEC_STORAGE_NR,
	ICC_SEC_SIGNINIG_NR,
	ICC_IMAGE_AUTH_NR,
	ICC_REGMAP_NR,
	ICC_FIREWALL_NR,
	ICC_SEC_STG_ADMIN_NR,
	ICC_SEC_STG_NORMAL_NR,
	ICC_SEC_DBG_ADMIN_NR,
	ICC_MAX_MINOR_NR,
} icc_dev_minor_num;

/*Always keep your new clients within InvalidClient and MAX_CLIENT*/
typedef enum {
	ICC_Client,/*Icc client Id*/
	IMAGE_AUTH,/*Image authenticator*/
	SECURE_STORAGE,/*Secure Storage*/
	SECURE_SIGN_SERVICE,/*Secure sign service*/
	PR,/*procfs redirector*/
	RM,/*Regmap driver for CGU,RCU,EPU*/
	ND,/*NOC Driver*/
	SSD,/*SSAL Driver*/
	SECURE_DEBUG, /* Secure Debug */
	ICC_UBOOT,	/* ICC Uboot */
	SEC_STORE_SERVICE = 10, /* User Applications 10 -20 */
	EMD,/*eMMC Driver*/
	MAX_CLIENT = 21/*MAX_CLIENT_ID*/
} icc_devices;/*enum for all the possible clients*/

enum {
	REGMAP_WR_MSGID,
	REGMAP_RD_MSGID,
	REGMAP_UB_MSGID
};


/* Clients through SSD and Driver Name */
struct ssd_client {
        const char *name;
        uint8_t ssd;
};

/******************************************************************************
 * Exported functions
 ******************************************************************************/
/** magic number */
#define ICC_MAGIC 'S'
#define ICC_IOC_REG_CLIENT _IOW(ICC_MAGIC, 1, uint32_t)
#define ICC_IOC_MEM_COMMIT _IOW(ICC_MAGIC, 2, icc_commit_t)
#define ICC_IOC_MEM_INVALIDATE _IOW(ICC_MAGIC, 3, icc_commit_t)

#define ICC_TEP_UP 0x1

#ifdef __KERNEL__

typedef enum {
	ICC_INVALID = 0x0,
	ICC_READ = 0x1,
	ICC_WRITE = 0x2,
	ICC_RW = 0x3
} icc_wake_type;

/**
 * ICC Device Structure.
 */

typedef struct {
	wait_queue_head_t perC_wakeuplist;
} client_sem_t;

typedef struct {
	/* Wakeuplist for the select mechanism */
	wait_queue_head_t wakeuplist;
	void (*up_callback) (icc_wake_type);
	volatile int Installed;
	uint32_t local_seq_num;
	uint32_t local_sem_idx;
	client_sem_t csem[MAX_PERC_MSG];
} icc_dev;

typedef struct {
	struct platform_device *pdev;
	struct gen_pool *pool;
} lgm_iccpool_dev_t;

int icc_init_module(void);
int icc_open(struct inode *inode, struct file *file_p);
int icc_close(struct inode *inode, struct file *filp);
int icc_register_callback(icc_devices type,
void (*callback) (icc_wake_type));
int icc_unregister_callback(icc_devices type);
int icc_read(icc_devices type, icc_msg_t *rw);
int icc_write(icc_devices type, icc_msg_t *rw);
icc_msg_t *icc_sync_write(icc_devices type, icc_msg_t *rw);
int icc_regmap_sync_read(phys_addr_t paddr, unsigned int *val);
int icc_regmap_sync_write(phys_addr_t paddr, unsigned int val);
int icc_regmap_sync_update_bits(phys_addr_t paddr, unsigned int mask,
				unsigned int val);
int icc_msc_noc_firewall_sync_cfg(unsigned int param0,
				  unsigned long long loaddr,
				  unsigned long long hiaddr,
				  unsigned int param3);
int icc_msc_noc_firewall_check_access(uint32_t param0,
				      uint64_t loaddr,
				      uint64_t hiaddr,
				      uint32_t param3);
/*API used for debug purpose to get the firewall rules */
/* param 0 - firewall id
 * param 1 - 0 means disable, 1 means eanble
 * param 2 -  not used
 * param 3 -  not used*/
int icc_msc_noc_firewall_cfg(uint32_t param0,
			     unsigned long param1,
			     uint32_t param2,
			     uint32_t param3);
int icc_send_mem_adapter_cfg(void);
extern struct device *icc_get_genpool_dev(void);
extern struct gen_pool *icc_get_genpool(void);
#endif /*__KERNEL__*/
#endif /* _DRV_UAPI_ICC_H */
