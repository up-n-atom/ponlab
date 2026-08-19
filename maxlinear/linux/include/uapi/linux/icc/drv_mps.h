#ifndef _DRV_MPS_VMMC_H
#define _DRV_MPS_VMMC_H
/******************************************************************************

               Copyright (c) 2017 - 2019 Intel Corporation
               Copyright (c) 2011 - 2016 Lantiq Beteiligungs-GmbH & Co. KG


	For licensing information, see the file 'LICENSE' in the root folder of
	this software module.

****************************************************************************
   Module      : drv_mps_vmmc.h
   Description : This file contains the defines, the structures declarations
		the tables declarations and the global functions declarations.
*******************************************************************************/

#define MAX_UPSTRM_DATAWORDS 4

#define ICC_PARAM_PTR 0x1
#define ICC_PARAM_NO_PTR 0x0
#define ICC_PARAM_PTR_IOCU 0x1
#define ICC_PARAM_PTR_NON_IOCU 0x0

#define CHECK_PTR(Attrs,Index) (Attrs&(ICC_PARAM_PTR << (Index*2)))
#define CHECK_PTR_IOCU(Attrs,Index) (Attrs&(ICC_PARAM_PTR_IOCU << ((Index*2)+1)))
#define SET_PTR(Attrs,Index) (Attrs|(ICC_PARAM_PTR << (Index*2)))
#define SET_PTR_IOCU(Attrs,Index) (Attrs|(ICC_PARAM_PTR_IOCU << ((Index*2)+1)))

#define MEM_SEG_SIZE 4096
#define MEM_SIZE	32

/* ============================= */
/* MPS Common defines            */
/* ============================= */

typedef struct {
	unsigned int icc_seq_num:16;
	unsigned int app_seq_num:16;
} mps_msg_seq_num_t;

typedef struct {
	unsigned int sem_idx:4;
	unsigned int icc_sync:1; /* indicates Synchronous message or not */
	unsigned int priority:1; /* Valid only for async messages Icc priority queue/capability*/
	unsigned int reserved:26;
} mps_flags_t;

typedef struct {
	uint8_t src_id;
	uint8_t dst_id;
	uint8_t msg_id;
	uint8_t param_attr;
} mpsHd;

typedef union {
	uint32_t val;
	mpsHd Hd;
} mpshd_u;
 

typedef struct {
	mpshd_u header;
	mps_msg_seq_num_t seq_num;
	mps_flags_t mps_flags;
	uint32_t data[MAX_UPSTRM_DATAWORDS];
} mps_message;

/******************************************************************************
 * Exported IOCTLs
 ******************************************************************************/
/** magic number */
#define MPS_MAGIC 'O'

/**
 * Read Message from Mailbox.
 * \param   arg Pointer to structure #mps_message
 * \ingroup IOCTL
 */
#define FIO_MPS_MB_READ _IOR(MPS_MAGIC, 3, mps_message)
/**
 * Write Message to Mailbox.
 * \param   arg Pointer to structure #mps_message
 * \ingroup IOCTL
 */
#define FIO_MPS_MB_WRITE _IOW(MPS_MAGIC, 4, mps_message)
/**
 * Reset Voice CPU.
 * \ingroup IOCTL
 */
#define FIO_MPS_RESET _IO(MPS_MAGIC, 6)
/**
 * Restart Voice CPU.
 * \ingroup IOCTL
 */
#define FIO_MPS_RESTART _IO(MPS_MAGIC, 7)
/**
 * Read Version String.
 * \param   arg Pointer to version string.
 * \ingroup IOCTL
 */
#define FIO_MPS_GETVERSION      _IOR(MPS_MAGIC, 8, char*)

/******************************************************************************
 * Register structure definitions
 ******************************************************************************/
typedef struct {    /**< Register structure for Common status registers MPS_RAD0SR, MPS_SAD0SR,
		      MPS_CAD0SR and MPS_AD0ENR  */

#if defined(__BIG_ENDIAN_BITFIELD)
	uint32_t res1:31;
	uint32_t du_mbx:1;
#else
	uint32_t du_mbx:1;
	uint32_t res1:31;
#endif
} MPS_Ad0Reg_s;

typedef union {
	uint32_t val;
	MPS_Ad0Reg_s fld;
} MPS_Ad0Reg_u;


/******************************************************************************
 * Exported functions
 ******************************************************************************/
#ifdef __KERNEL__
bool check_mps_fifo_not_empty(void);
int mps_init(void);
#endif

#ifdef __KERNEL__
#include <linux/fs.h>
typedef struct /**< mps buffer monitoring structure */
{
	void *(*malloc) (size_t size, int32_t priority); /**< Buffer alloc function (def. kmalloc) */
	void (*free) (const void *ptr);  /**< Buffer free  function (def. kfree) */
	int32_t (*init) (void); /** Manager init function */
	int32_t (*close) (void); /** Manager shutdown function */
} mps_buf_mng_t;

extern mps_buf_mng_t mps_buffer;
int32_t mps_open(struct inode *inode, struct file *file_p);
int32_t mps_close(struct inode *inode, struct file *filp);
long mps_ioctl(struct file *filp, uint32_t nCmd, unsigned long arg);
int32_t mps_register_callback(void (*callback) (void));
int32_t mps_unregister_callback(void);
int32_t mps_read_mailbox(mps_message * rw);
int32_t mps_write_mailbox(mps_message * rw);
void mps_bufman_register(void *(*malloc) (size_t size, int32_t priority),
void (*free) (const void * ptr));

extern void cache_inv(ulong addr, uint32_t len);
extern void cache_wb_inv(ulong addr, uint32_t len);
#endif /*__KERNEL__*/
#endif /* _DRV_MPS_VMMC_H */
