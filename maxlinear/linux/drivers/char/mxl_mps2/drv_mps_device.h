#ifndef _DRV_MPS_VMMC_DEVICE_H
#define _DRV_MPS_VMMC_DEVICE_H
/******************************************************************************

			Copyright (c) 2013
			Lantiq Deutschland GmbH
			http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

****************************************************************************
   Module      : drv_mps_vmmc_device.h
   Description : This file contains the defines, the structures declarations
		the tables declarations and the device specific functions
		declarations.
*******************************************************************************/
//#include <irq.h>

/** This variable holds the actual base address of the MPS register block. */
extern unsigned long mps_reg_base;  /* MPS registers */
/** This variable holds the actual base address of the MPS SRAM area. */
extern unsigned long mps_ram_base;  /* MPS memory */
/** This variable holds the interrupt number of the IRQ associated with the
    MPS status register 4 which is used for AFE and DFE 0 status. */
extern uint32_t mps_ir4;       /* MPS AD0 register interrupt */
extern uint32_t mps_ir0;       /* MPS register interrupt */
extern uint32_t mps_ir1;       /* MPS register interrupt */

/*Macros used in MPS/ICC*/
#define SUCCESS 0
#define ERROR -1
#define TRUE 1
#define FALSE 0

/* MPS register block */
#ifndef SYSTEM_ATOM
	#define MPS_BASE_ADDR        (KSEG1 | 0x1F107400)
	#define MPS_SRAM             ((volatile u32 *)(KSEG1 | 0x1F201000))

	#define MPS_RAD0SR              ((u32 *)(mps_reg_base + 0x0040))
	#define MPS_RAD1SR              ((u32 *)(mps_reg_base + 0x0084))
	#define MPS_SAD0SR              ((u32 *)(mps_reg_base + 0x0048))
	#define MPS_SAD1SR              ((u32 *)(mps_reg_base + 0x0070))
	#define MPS_CAD0SR              ((u32 *)(mps_reg_base + 0x0050))
	#define MPS_CAD1SR              ((u32 *)(mps_reg_base + 0x0080))
	#define MPS_AD0ENR              ((u32 *)(mps_reg_base + 0x0058))
	#define MPS_AD1ENR              ((u32 *)(mps_reg_base + 0x0074))
#else   /* FIXME: LGM */
	#define MPS_BASE_ADDR        (0xECC00000)
	#define MPS_SRAM             (0xECC40000)

	#define MPS_GIRR 0x78
	#define MPS_GIER 0x7C
	#define MPS_GIRDR 0x88
	#define MPS_GICR 0x8C

	#define MPS_VPE0_2_VPE1_IRR 0x70
	#define MPS_VPE0_2_VPE1_IER 0x74
	#define MPS_VPE0_2_VPE1_ICR 0x80
	#define MPS_VPE0_2_VPE1_IRDR 0x84

#endif

#ifndef SYSTEM_ATOM
/* Interrupt vectors */
	#define MPS_IR4  /* AD0 */          		 22
	#define MPS_IR0  /* Global interrupt */          223
	#define MPS_IR1  /* Global interrupt */          224
#else /* FIXME: LGM */
	
	#define MPS_REG_VPE0_2_VPE1_IRR 	((u32 *)(mps_reg_base + MPS_VPE0_2_VPE1_IRR))
        #define MPS_REG_VPE0_2_VPE1_IER         ((u32 *)(mps_reg_base + MPS_VPE0_2_VPE1_IER))
        #define MPS_REG_GIRR			((u32 *)(mps_reg_base + MPS_GIRR))
        #define MPS_REG_GIER			((u32 *)(mps_reg_base + MPS_GIER))

        #define MPS_REG_VPE0_2_VPE1_ICR         ((u32 *)(mps_reg_base + MPS_VPE0_2_VPE1_ICR))
        #define MPS_REG_VPE0_2_VPE1_IRDR        ((u32 *)(mps_reg_base + MPS_VPE0_2_VPE1_IRDR))
        #define MPS_REG_GIRDR			((u32 *)(mps_reg_base + MPS_GIRDR))
        #define MPS_REG_GICR			((u32 *)(mps_reg_base + MPS_GICR))

  /* ATOM to TEP Interrupt Numbers */
        #define MPS_IR9                                         30
        #define MPS_IR10                                        31
        #define MPS_IR11                                        32
        #define MPS_IR12                                        33
        #define MPS_IR13                                        34
        #define MPS_IR14                                        35
        #define MPS_IR15                                        36
        #define MPS_IR_SEM                                      37
        #define MPS_IR_GLOBAL                           	38

        /*TEP to ATOM Interrupt Numbers */
        #define MPS_IR0                                         167
        #define MPS_IR1                                         168
        #define MPS_IR2                                         169
        #define MPS_IR3                                         170
        #define MPS_IR4                                         171
        #define MPS_IR5                                         172
        #define MPS_IR6                                         173
        #define MPS_IR_SEM_ATOM                         	174
        #define MPS_IR_GLOBAL_ATOM                      	175
#endif

/* ============================= */
/* MPS Common defines            */
/* ============================= */
/*---------------------------------------------------------------------------*/
/* Mailbox definitions                                                       */
/*---------------------------------------------------------------------------*/
#ifdef VPE0
	#if defined(SYSTEM_GRX500) || defined(SYSTEM_4KEC)
		#define MBX_DATA_UPSTRM_FIFO_SIZE 496
		#define MBX_DATA_DNSTRM_FIFO_SIZE 492
	#else
		#define MBX_DATA_UPSTRM_FIFO_SIZE 240
		#define MBX_DATA_DNSTRM_FIFO_SIZE 236
	#endif
	#define MBX_RW_POINTER_AREA_SIZE 32
	/* base addresses for mailboxes (upstream and downstream ) */
	#define MBX_UPSTRM_DATA_FIFO_BASE   (mps_ram_base + MBX_RW_POINTER_AREA_SIZE)
	#define MBX_DNSTRM_DATA_FIFO_BASE   (MBX_UPSTRM_DATA_FIFO_BASE + MBX_DATA_UPSTRM_FIFO_SIZE)
#else
	#if defined(SYSTEM_GRX500)||defined(SYSTEM_4KEC)||defined(SYSTEM_ATOM)
		/* For SYSTEM_ATOM - sizeof mps_message and icc_msg_t is 0x1c (28) */
		#define MBX_DATA_UPSTRM_FIFO_SIZE (17 * (sizeof(mps_message)))
		#define MBX_DATA_DNSTRM_FIFO_SIZE (17 * (sizeof(mps_message)))
	#else
		#define MBX_DATA_UPSTRM_FIFO_SIZE 236
		#define MBX_DATA_DNSTRM_FIFO_SIZE 240
	#endif
	#define MBX_RW_POINTER_AREA_SIZE 64
	/* base addresses for mailboxes (upstream and downstream ) */
	#define MBX_DNSTRM_DATA_FIFO_BASE   (mps_ram_base + MBX_RW_POINTER_AREA_SIZE)
	#define MBX_UPSTRM_DATA_FIFO_BASE   (MBX_DNSTRM_DATA_FIFO_BASE + MBX_DATA_DNSTRM_FIFO_SIZE)
#endif


/* FIXME: Check on the LGM - it doesn't need it */
#if defined(SYSTEM_GRX500)||defined(SYSTEM_4KEC)
	#define MBX_DATA_WORDS 247
#else 
	#define MBX_DATA_WORDS 119
#endif


#define MAX_FIFO_WRITE_RETRIES 80
#define MBX_LENGTH sizeof(mps_message) /* 0x20 = 32 bytes*/

/*---------------------------------------------------------------------------*/
/* MPS buffer provision management structure definitions                   */
/*---------------------------------------------------------------------------*/

#define MPS_BUFFER_INITIAL   MEM_SIZE 
#define MPS_MEM_SEG_DATASIZE MEM_SEG_SIZE

/*---------------------------------------------------------------------------*/
/* DEVICE DEPENDENCIES                                                       */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Register structure definitions                                            */
/*---------------------------------------------------------------------------*/
typedef enum {
	UPSTREAM,
	DOWNSTREAM
} MbxDirection_e;

typedef struct {
	uint32_t src_id;
	uint32_t dst_id;
	uint32_t msg_id;
	uint32_t param_attr;
} MbxMsgHd;

typedef union {
	uint32_t val;
	MbxMsgHd Hd;
} MbxMsgHd_u;
 

typedef struct {
	MbxMsgHd_u header;
	uint32_t capabilities;
	uint32_t seq_num;
	uint32_t data[MAX_UPSTRM_DATAWORDS];
} MbxMsg_s;

/*---------------------------------------------------------------------------*/
/* FIFO structure                                                            */
/*---------------------------------------------------------------------------*/
typedef struct {
	volatile unsigned long *pstart;     /**< Pointer to FIFO's read/write start address */
	volatile unsigned long *pend;       /**< Pointer to FIFO's read/write end address */
	volatile unsigned long *pwrite_off; /**< Pointer to FIFO's write index location */
	volatile unsigned long *pread_off;  /**< Pointer to FIFO's read index location */
	volatile uint32_t size;        /**< FIFO size */
	volatile uint32_t min_space;   /**< FIFO size */
	volatile uint32_t bytes;
	volatile uint32_t pkts;
	volatile uint32_t discards;
} mps_fifo;

/*
 * This structure represents the MPS mailbox definition area that is shared
 * by CCPU and VCPU. It comprises the mailboxes' base addresses and sizes in bytes as well as the
 *
 *
 */
typedef struct {
#if 0
	volatile uint32_t *MBX_UPSTR_DATA_BASE; /**< Upstream Data FIFO Base Address */
	volatile uint32_t MBX_UPSTR_DATA_SIZE;  /**< Upstream Data FIFO size in byte */
	volatile uint32_t *MBX_DNSTR_DATA_BASE; /**< Downstream Data FIFO Base Address */
	volatile uint32_t MBX_DNSTR_DATA_SIZE;  /**< Downstream Data FIFO size in byte */
	volatile uint32_t MBX_UPSTR_DATA_READ;   /**< Upstream Data FIFO Read Index */
	volatile uint32_t MBX_UPSTR_DATA_WRITE;  /**< Upstream Data FIFO Write Index */
	volatile uint32_t MBX_DNSTR_DATA_READ;   /**< Downstream Data FIFO Read Index */
	volatile uint32_t MBX_DNSTR_DATA_WRITE;  /**< Downstream Data FIFO Write Index */
#else
	volatile unsigned long *MBX_DNSTR_DATA_BASE; /**< Downstream Data FIFO Base Address */
	volatile unsigned long MBX_DNSTR_DATA_SIZE;  /**< Downstream Data FIFO size in byte */
	volatile unsigned long *MBX_UPSTR_DATA_BASE; /**< Upstream Data FIFO Base Address */
	volatile unsigned long MBX_UPSTR_DATA_SIZE;  /**< Upstream Data FIFO size in byte */
	volatile unsigned long MBX_DNSTR_DATA_READ;   /**< Downstream Data FIFO Read Index */
	volatile unsigned long MBX_DNSTR_DATA_WRITE;  /**< Downstream Data FIFO Write Index */
	volatile unsigned long MBX_UPSTR_DATA_READ;   /**< Upstream Data FIFO Read Index */
	volatile unsigned long MBX_UPSTR_DATA_WRITE;  /**< Upstream Data FIFO Write Index */
#endif
	volatile uint32_t MBX_DATA[MBX_DATA_WORDS];
	volatile uint32_t MBX_VPE1_PTR;  				/**< Downstream Data FIFO Write Index */
} mps_mbx_reg;

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Device connection structure                                               */
/*---------------------------------------------------------------------------*/

/**
 * Mailbox Device Structure.
 * This Structure holds top level parameters of the mailboxes used to allow
 * the communication between the control CPU and the Voice CPU
 */
typedef struct {
	/* Wakeuplist for the select mechanism */
	wait_queue_head_t  mps_wakeuplist;
	mps_fifo *upstrm_fifo;    /**< Data exchange FIFO for read (upstream) */
	mps_fifo *dwstrm_fifo;    /**< Data exchange FIFO for write (downstream) */
	void (*up_callback) (void);
	int32_t devID;
	volatile int32_t Installed;
	struct cdev *mps_cdev;
} mps_mbx_dev;

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Device structure                                                          */
/*---------------------------------------------------------------------------*/

/**
 * Mailbox Device Structure.
 * This Structure represents the communication device that provides the resources
 * for the communication between CPU0 and CPU1
 */
typedef struct {
	mps_mbx_reg *base_global;   /**< global register pointer for the ISR */
	uint32_t flags;                   /**< Pointer to private date of the specific handler */
	mps_mbx_dev mb;     /**< Data upstream and downstream mailboxes */
	mps_fifo upstrm_fifo;
	mps_fifo dwstrm_fifo;
	mps_fifo sw_upstrm_fifo;
} mps_comm_dev;

/*---------------------------------------------------------------------------*/
int32_t mps_common_open (mps_comm_dev * pDev, mps_mbx_dev * pMBDev, bool from_kernel);
int32_t mps_common_close (mps_mbx_dev * pMBDev, bool from_kernel);
int32_t mps_mbx_read (mps_mbx_dev * pMBDev, mps_message * pPkg, int32_t timeout);
int32_t mps_mbx_write_data (mps_mbx_dev * pMBDev, mps_message * readWrite);
int32_t mps_init_structures (mps_comm_dev * pDev);
uint32_t mps_fifo_mem_available (mps_fifo * mbx);
int32_t mps_bufman_init (void);
void mps_bufman_free (const void * ptr);
void *mps_bufman_malloc (size_t size, int32_t priority);
void mps_disable_mailbox_int (void);
void mps_disable_all_int (void);
void mps_enable_mailbox_int (void);
void tep2atom_enable_mailbox_int(void);
void tep2atom_disable_mailbox_int(void);
void tep2atom_disable_all_int(void);

DECLARE_TRACE_GROUP(MPS2);
#endif /* _DRV_MPS_VMMC_DEVICE_H */
