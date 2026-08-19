/******************************************************************************

         Copyright (c) 2022, MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#ifndef __HW_MMB_PRIV_H__
#define __HW_MMB_PRIV_H__

#include "mhi_umi.h"
#include "mtlk_df.h"
#include "mtlkmsg.h"
#include "mtlk_assert.h"
#include "shram.h"
#include "mtlklist.h"
#include "mtlk_dfg.h"
#include "mtlk_df_nbuf.h"
#include "mtlk_df_fw.h"
#include "mtlk_gpl_helper.h"
#include "mtlk_vap_manager.h"
#include "mtlkdfdefs.h"
#include "mtlk_df_priv.h"
#include "mtlk_df_user_priv.h"
#include "mtlk_packets.h"
#include "mtlkhal.h"
#include "mtlk_snprintf.h"
#include "mtlkwlanirbdefs.h"
#include "mtlkwssa_drvinfo.h"
#include "mtlk_wss_debug.h"
#include "fw_recovery.h"
#include "wave_hal_stats.h"
#include "core_stats.h"
#include "core_config.h"
#include "mtlk_psdb.h"
#include "mmb_ops.h"
#include "drvver.h"
#include "linux/netdevice.h"
#ifdef CPTCFG_IWLWAV_SET_PM_QOS
#include "linux/pm_qos.h"
#endif
#include "core.h"
#include "channels.h"
#include "mtlk_dbg.h"
#ifdef CONFIG_WAVE_RTLOG_REMOTE
#include "mtlk_rtlog.h"
#endif
#ifdef CPTCFG_IWLWAV_PMCU_SUPPORT
#include "mtlk_pcoc.h"
#endif
#include "scan_support.h"
#include "mac80211.h"
#include "eth_parser.h"
#include "compat.h"

#define LOG_LOCAL_GID   GID_HWMMB
#define LOG_LOCAL_FID   2

#ifdef MTLK_DEBUG
    /* A semicolon ';' at the end of line is required for proper LOG macro generation */
    /* Format string is also required, even an empty one at the end of the 1st param */
    #define TRACE_PARAM_NOTE(str)       do { ILOG1_V(str ""); } while (0)
    #define TRACE_PARAM_INT(value)      do { ILOG1_SD("%s: 0x%08X", #value, (uint32)(value));                } while (0)
    #define TRACE_PARAM_PTR(value)      do { ILOG1_SP("%s: 0x%p",   #value, (value));                        } while (0)
    #define TRACE_PARAM_STR(value)      do { ILOG1_SS("%s: %s",     #value, (value));                        } while (0)
    #define TRACE_PARAM_MAC16(value)    do { ILOG1_SD("%s: 0x%04X", #value, MAC_TO_HOST16((uint16)(value))); } while (0)
    #define TRACE_PARAM_MAC32(value)    do { ILOG1_SD("%s: 0x%08X", #value, MAC_TO_HOST32((uint32)(value))); } while (0)
#else
    #define TRACE_PARAM_NOTE(str)
    #define TRACE_PARAM_INT(value)
    #define TRACE_PARAM_PTR(value)
    #define TRACE_PARAM_STR(value)
    #define TRACE_PARAM_MAC16(value)
    #define TRACE_PARAM_MAC32(value)
#endif

#if CPTCFG_IWLWAV_TRACER_HISTORY_LENGTH
/* tracer enum */
typedef enum _mtlk_tracer_id {
  MTLK_TR_NONE,
  MTLK_TR_INTR_ENTER,  /* interrupt handler enter */
  MTLK_TR_INTR_EXIT,   /* interrupt handler exit */
  MTLK_TR_INTR_DISABLE_INT,
  MTLK_TR_INTR_ENABLE_INT,
  MTLK_TR_INTR_CLEAR_INT,
  MTLK_TR_INTR_SCHED_TASKLET,   /* tasklet was scheduled */
  MTLK_TR_TASKLET_START,        /* tasklet was executed */
  MTLK_TR_TASKLET_STOP,         /* returned from tasklet */
  MTLK_TR_DATA_TXOUT_START,
  MTLK_TR_DATA_TXOUT_END,
  MTLK_TR_DATA_TXOUT_REM,
  MTLK_TR_DATA_RX_START,
  MTLK_TR_DATA_RX_END,
  MTLK_TR_DATA_RX_REM,
  MTLK_TR_BSS_RX_START,
  MTLK_TR_BSS_RX_END,
  MTLK_TR_BSS_RX_REM,
  MTLK_TR_BSS_CFM_START,
  MTLK_TR_BSS_CFM_END,
  MTLK_TR_BSS_CFM_REM,
  MTLK_TR_MSG_START,
  MTLK_TR_MSG_COUNT,
  MTLK_TR_MSG_END,
  MTLK_TR_MSG_REM,
  MTLK_TR_MSG_SEND_ID,        /* Sent message to FW */
  MTLK_TR_MSG_SEND_CFM_ID,    /* Received confirmation from FW for previously sent message */
  MTLK_TR_MSG_RECEIVE_ID,     /* Received message from FW */
  MTLK_TR_MSG_RECEIVE_CFM_ID, /* Sent confirmation to FW for previously received message */
  MTLK_TR_MSG_SEND_LEN,
  MTLK_TR_MSG_SEND_DESCR,
  MTLK_TR_COUNT
} mtlk_tracer_id;

struct _mtlk_tracer_entry {
  uint32          value;
  uint32          ts_low;
  uint32          ts_high;
  mtlk_tracer_id  id;
};

typedef struct _mtlk_tracer_t {
  struct _mtlk_tracer_entry    *mem;
  mtlk_osal_spinlock_t         lock;
  unsigned                     idx;
  BOOL                         enabled;
  BOOL                         no_print;
}mtlk_tracer_t;

#endif /* #if CPTCFG_IWLWAV_TRACER_HISTORY_LENGTH */

#define STRING_IF_VERSIONS_SIZE 768
#define STRING_PROGMODEL_SIZE   128

#define LOGGER_SEVERITY_DEFAULT_LEVEL      9 /* highest debug level */

#define MTLK_CHI_MAGIC_TIMEOUT_ASIC   ( 5 * MTLK_OSAL_MSEC_IN_SEC) /* ... seconds to ms */
#define MTLK_CHI_MAGIC_TIMEOUT_FPGA   ( 5 * MTLK_OSAL_MSEC_IN_MIN) /* ... minutes to ms */
#define MTLK_CHI_MAGIC_TIMEOUT_EMUL   (80 * MTLK_OSAL_MSEC_IN_MIN) /* ... minutes to ms */

#define MTLK_READY_CFM_TIMEOUT         10000 /* ms */
#define MTLK_SW_RESET_CFM_TIMEOUT      10000 /* ms */
#define MTLK_PRGMDL_LOAD_CHUNK_TIMEOUT 10000 /* ms */
#define MTLK_RX_BUFFS_RECOVERY_PERIOD  10000 /* ms */
#define MTLK_INTERRUPT_RECOVERY_PERIOD 50    /* ms */

#define BCL_ON_EXC_CTL_TIMEOUT           10 /* Timeout for SHARED_RAM_BCL_ON_EXCEPTION_CTL, in ms */
#define BCL_ON_EXC_CTL_LOOP_DELAY         5 /* Typical time for SHARED_RAM_BCL_ON_EXCEPTION_CTL to get executed, in microseconds */
                                            /* On GRX550 / GPB514H system a 5 microsecond delay is sufficient to get the result */
                                            /* ready at the first attempt in 99.995% of the cases*/

#define MTLK_MAX_RX_BUFFS_TO_RECOVER   ((uint16)-1) /* no limit */
#define HW_PCI_TXMM_MAX_MSGS 32
#define HW_PCI_TXDM_MAX_MSGS 8
#define HW_PCI_TXMM_GRANULARITY 1000
#define HW_PCI_TXDM_GRANULARITY 1000
#ifndef MTLK_RX_BUFF_ALIGNMENT
#define MTLK_RX_BUFF_ALIGNMENT 0     /* No special alignment required */
#endif
#define BYTE_COUNTER_RESOLUTION 64
#define MTLK_OWNER_DRV 1
#define MTLK_OWNER_FW  0
#define RX_DATA_QUE_POS 0
#define RX_MGMT_QUE_POS 1
#define BSS_MGMT_FRAME_SIZE     2348
#define RX_MGMT_ALIGN_SIZE      (16u) /* Gen6 buffer alignment for both Frame and RxMetrics */

#if !defined(ARCH_DMA_MINALIGN) || (ARCH_DMA_MINALIGN < 16)
#define RX_MGMT_BUF_PTR_ALIGN
#endif

#ifdef RX_MGMT_BUF_PTR_ALIGN
/*
 * To cater to the 16 byte ptr alignment of Rx DMA, request 16 byte more from kernel
 * so that we always get the required buffer for the actual mgmt frame
 */
#define RX_MGMT_BUF_PTR_ALIGN_SIZE  (16u)
#endif
#define RX_MGMT_ALIGN_MASK      (RX_MGMT_ALIGN_SIZE - 1)
#define RX_MGMT_ALIGNMENT(n)    ((n) + RX_MGMT_ALIGN_SIZE - ((n) & RX_MGMT_ALIGN_MASK))
#define RX_MGMT_PHY_METRIC_ALIGN(n) ((n) & RX_MGMT_ALIGN_MASK) ? ((n) + RX_MGMT_ALIGN_SIZE - ((n) & RX_MGMT_ALIGN_MASK)) : (n)
#define G6_RX_MGMT_FRAME_SIZE   (RX_MGMT_ALIGNMENT(BSS_MGMT_FRAME_SIZE) + sizeof(G6_RX_PHY_METRICS))
/* RX buffer: Offset, Frame and RxPhyMetrics. Reserve RX_MGMT_ALIGN_SIZE for the maximall offset */
#define RX_MGMT_ALLOC_SIZE      (RX_MGMT_ALIGN_SIZE + G6_RX_MGMT_FRAME_SIZE)

#define WAVE_ADDR_ALIGN_SIZE_N(ptr, n)  ((uintptr_t)(ptr) + n - ((uintptr_t)(ptr) & (n - 1)))

#define WAVE_HW_RX_HD_EP_MASK_GEN6_32          0x00
#define WAVE_HW_RX_HD_EP_MASK_GEN6_CDB_16_16   0x01
#define WAVE_HW_RX_HD_EP_MASK_GEN6_SB_SC_31_1  0x01
#define WAVE_HW_RX_HD_VAP_MASK_GEN6_32         0x1F
#define WAVE_HW_RX_HD_VAP_MASK_GEN6_CDB_16_16  0x0F
#define WAVE_HW_RX_HD_VAP_MASK_GEN6_SB_SC_31_1 0x1F
#ifdef MTLK_WAVE_700
#define WAVE_HW_RX_HD_REL_VAP_MASK_2G_5G_GEN7  0xF
#define WAVE_HW_RX_HD_REL_VAP_MASK_6G_GEN7     0x1F
#define WAVE_HW_RX_HD_EP_MASK_2G_5G_GEN7       0x01
#define WAVE_HW_RX_HD_EP_MASK_6G_GEN7          0x02
#define WAVE_HW_RX_HD_EP_MASK_6G_GEN7_UPPER16  0x03
#define WAVE_HW_GEN7_6G_IDX                    0x02
#endif


/******************************************************************************
 *   WSS counters for HW card statistics
 */
typedef enum
{
  /* Note: all HW source counters at first */
  MTLK_HW_SOURCE_CNT_FW_LOGGER_PACKETS_PROCESSED,
  MTLK_HW_SOURCE_CNT_FW_LOGGER_PACKETS_DROPPED,
  MTLK_HW_SOURCE_CNT_ISRS_TOTAL,
  MTLK_HW_SOURCE_CNT_ISRS_FOREIGN,
  MTLK_HW_SOURCE_CNT_ISRS_NOT_PENDING,
  MTLK_HW_SOURCE_CNT_ISRS_INIT,
  MTLK_HW_SOURCE_CNT_ISRS_TO_DPC,
  MTLK_HW_SOURCE_CNT_POST_ISR_DPCS,
  MTLK_HW_SOURCE_CNT_LEGACY_IND_RECEIVED,
  MTLK_HW_SOURCE_CNT_RX_BUF_ALLOC_FAILED,
  MTLK_HW_SOURCE_CNT_RX_BUF_REALLOC_FAILED,
  MTLK_HW_SOURCE_CNT_RX_BUF_REALLOCATED,
  MTLK_HW_SOURCE_CNT_BSS_RX_PACKETS_PROCESSED,
  MTLK_HW_SOURCE_CNT_BSS_RX_PACKETS_REJECTED,
#ifdef CPTCFG_IWLWAV_LEGACY_INT_RECOVERY_MON
  MTLK_HW_SOURCE_CNT_ISR_LOST_SUSPECT,
  MTLK_HW_SOURCE_CNT_ISR_LOST_RECOVER,
#endif
  MTLK_HW_SOURCE_CNT_LAST,

  /* HW listener counters */
  MTLK_HW_CNT_NOF_FAST_RCVRY_PROCESSED = MTLK_HW_SOURCE_CNT_LAST,
  MTLK_HW_CNT_NOF_FULL_RCVRY_PROCESSED,
  MTLK_HW_CNT_NOF_FAST_RCVRY_FAILED,
  MTLK_HW_CNT_NOF_FULL_RCVRY_FAILED,

  MTLK_HW_CARD_CNT_LAST
} mtlk_hw_source_wss_cnt_id_e;

/* Statistic ALLOWED flags */
#define MTLK_HW_SOURCE_CNT_FW_LOGGER_PACKETS_PROCESSED_ALLOWED      MTLK_WWSS_WLAN_STAT_ID_FW_LOGGER_PACKETS_PROCESSED_ALLOWED
#define MTLK_HW_SOURCE_CNT_FW_LOGGER_PACKETS_DROPPED_ALLOWED        MTLK_WWSS_WLAN_STAT_ID_FW_LOGGER_PACKETS_DROPPED_ALLOWED
#define MTLK_HW_SOURCE_CNT_ISRS_TOTAL_ALLOWED                       MTLK_WWSS_WLAN_STAT_ID_ISRS_TOTAL_ALLOWED
#define MTLK_HW_SOURCE_CNT_ISRS_FOREIGN_ALLOWED                     MTLK_WWSS_WLAN_STAT_ID_ISRS_FOREIGN_ALLOWED
#define MTLK_HW_SOURCE_CNT_ISRS_NOT_PENDING_ALLOWED                 MTLK_WWSS_WLAN_STAT_ID_ISRS_NOT_PENDING_ALLOWED
#define MTLK_HW_SOURCE_CNT_ISRS_INIT_ALLOWED                        MTLK_WWSS_WLAN_STAT_ID_ISRS_INIT_ALLOWED
#define MTLK_HW_SOURCE_CNT_ISRS_TO_DPC_ALLOWED                      MTLK_WWSS_WLAN_STAT_ID_ISRS_TO_DPC_ALLOWED
#define MTLK_HW_SOURCE_CNT_POST_ISR_DPCS_ALLOWED                    MTLK_WWSS_WLAN_STAT_ID_POST_ISR_DPCS_ALLOWED
#define MTLK_HW_SOURCE_CNT_LEGACY_IND_RECEIVED_ALLOWED              MTLK_WWSS_WLAN_STAT_ID_LEGACY_IND_RECEIVED_ALLOWED
#define MTLK_HW_SOURCE_CNT_RX_BUF_ALLOC_FAILED_ALLOWED              MTLK_WWSS_WLAN_STAT_ID_RX_BUF_ALLOC_FAILED_ALLOWED
#define MTLK_HW_SOURCE_CNT_RX_BUF_REALLOC_FAILED_ALLOWED            MTLK_WWSS_WLAN_STAT_ID_RX_BUF_REALLOC_FAILED_ALLOWED
#define MTLK_HW_SOURCE_CNT_RX_BUF_REALLOCATED_ALLOWED               MTLK_WWSS_WLAN_STAT_ID_RX_BUF_REALLOCATED_ALLOWED
#define MTLK_HW_SOURCE_CNT_BSS_RX_PACKETS_PROCESSED_ALLOWED         MTLK_WWSS_WLAN_STAT_ID_BSS_RX_PACKETS_PROCESSED_ALLOWED
#define MTLK_HW_SOURCE_CNT_BSS_RX_PACKETS_REJECTED_ALLOWED          MTLK_WWSS_WLAN_STAT_ID_BSS_RX_PACKETS_REJECTED_ALLOWED
#ifdef CPTCFG_IWLWAV_LEGACY_INT_RECOVERY_MON
#define MTLK_HW_SOURCE_CNT_ISR_LOST_SUSPECT_ALLOWED                 MTLK_WWSS_WLAN_STAT_ID_ISR_LOST_SUSPECT_ALLOWED
#define MTLK_HW_SOURCE_CNT_ISR_LOST_RECOVER_ALLOWED                 MTLK_WWSS_WLAN_STAT_ID_ISR_LOST_RECOVER_ALLOWED
#endif

/******************************************************************************/

typedef struct
{
  uint32 percentage;  /* percent */
  uint32 min_buffers; /* nof     */
  uint32 data_size;   /* bytes   */
} mtlk_hw_rx_queue_cfg_t;

typedef struct
{
  uint16 max_que_size;  /* Maximum queue size */
  uint16 que_size;      /* Current queue size */
} mtlk_hw_rx_queue_t;


/*****************************************************
 * IND/REQ BD-related definitions
 *****************************************************/
typedef struct
{
  uint32 offset; /* BD offset (PAS) */
  uint16 size;   /* BD size         */
  uint16 idx;    /* BD access index */
} mtlk_hw_bd_t;

typedef struct
{
  mtlk_hw_bd_t ind;
  mtlk_hw_bd_t req;
} mtlk_hw_ind_req_bd_t;
/*****************************************************/

/*****************************************************
 * Data Rx-related definitions
 *****************************************************/
typedef struct
{
  mtlk_hw_mirror_hdr_t hdr;     /* Header */
  union {
    mtlk_nbuf_t        *nbuf;   /* Network buffer (in SW path) */
    void*              *data;   /* In FastPath mode */
  };
  uint32               dma_addr;/* DMA mapped address */
  uint32               size;    /* Data buffer size */
  uint8                que_idx; /* Rx Queue Index */
  mtlk_lslist_entry_t  pend_l;  /* Pending list entry */
} mtlk_hw_data_ind_mirror_t;
/*****************************************************/

/*****************************************************
 * Logger-related definitions
 *****************************************************/
typedef struct
{
  mtlk_hw_mirror_hdr_t hdr;         /* Header */
  void*                virt_addr;   /* data buffer virtual address    */
  uint32               dma_addr;    /* data buffer DMA mapped address */
  uint16               bufsize;
} mtlk_hw_log_ind_mirror_t;
/*****************************************************/

/*****************************************************
 * PHY_RX_STATUS definitions
 *****************************************************/
typedef struct
{
  wholePhyRxStatusDb_t *db_data;
  uint16                db_size;
  uint16                all_sta_sid;
  uint16                max_sid;
  uint8                 noise_offs;
  uint8                 rssi_offs;
} hw_phy_rx_status_t;
/*****************************************************/

/*****************************************************
 * Control Messages (CM = MM and DM) Tx-related definitions
 *****************************************************/
typedef struct _mtlk_hw_cm_req_obj_t
{
  mtlk_hw_mirror_hdr_t hdr;        /* Header */
#ifdef CPTCFG_IWLWAV_DEBUG
  mtlk_atomic_t        usage_cnt;  /* message usage counter */
#endif
  UMI_MSG_HEADER       msg_hdr;
} mtlk_hw_cm_req_mirror_t;
/*****************************************************/

/*****************************************************
 * Control Messages (CM = MM and DM) Rx-related definitions
 * NOTE: msg member must be 1st in these structures because
 *       it is used for copying messages to/from PAS and
 *       buffers that are used for copying to/from PAS must
 *       be aligned to 32 bits boundary (see
 *       _mtlk_mmb_memcpy...() functions)
 *****************************************************/
typedef struct
{
  mtlk_hw_mirror_hdr_t hdr;
  UMI_MSG_HEADER       msg_hdr;
} mtlk_hw_cm_ind_mirror_t;
/*****************************************************/

/*****************************************************
 * Auxiliary BD ring-related definitions
 *****************************************************/
/********************************************************************
 * Number of BD descriptors
 * PAS offset of BD array (ring)
 * Local BD mirror (array)
*********************************************************************/
typedef struct {
  uint32  nof_bds;
  void   *iom_bdr_pos;
  uint16  iom_bd_size;
  void   *hst_bdr_mirror;
  uint16  hst_bd_size;
  MTLK_DECLARE_INIT_STATUS;
} mtlk_mmb_basic_bdr_t;

/* BSS Management path stuff */
struct mtlk_hw_bss_req_mirror
{
  mtlk_hw_mirror_hdr_t hdr; /* index and doubly linked list ptrs */
  uint64 cookie;
  uint8 *virt_addr; /* mgmt frame */
  uint32 dma_addr;
  uint32 size;      /* size of the valid data in the buffer, not the buffer size */
  uint32 extra_processing;
  uint8  vap_id;
  uint8  radio_id;
  uint8  subtype;
  uint8  action_code;
  uint8  action_field;
  BOOL   is_broadcast;
  IEEE_ADDR dst_addr;
  mtlk_nbuf_t *skb;
  BOOL   power_mgmt_on;
  uint32 tid;
  BOOL is_dpp;     /* Flag to identify DPP vendor specific action frame  */
};

struct mtlk_hw_bss_ind_mirror
{
  mtlk_hw_mirror_hdr_t  hdr;        /* index, etc. */
  uint32                dma_addr;
  uint8                 *virt_addr; /* mgmt frame */
#ifdef RX_MGMT_BUF_PTR_ALIGN
  uint8                 *virt_addr_prealign; /* virtual addr before alignment */
#endif
  uint32 size;                      /* size of the buffer */
};

/* FW <--> DRV messages interface */

typedef struct {
  uint16 index;
  uint8  radio_id;
  uint8  vap_id;
} him_msg_t;

/* HIM logger indication */
typedef struct {
  unsigned short index;
  unsigned short data_size;
} him_log_ind_msg_t;

/* Workaround: make correct alignment for HOST_DSC */
typedef struct _HOST_DSC __attribute__((aligned(4))) HOST_DSC_TYPE;
#define HOST_DSC HOST_DSC_TYPE

typedef struct
{
  uint32                  size;          /* ring size */
  uint32                  read_ptr;      /* ring read pointer */
  uint32                  write_ptr;     /* ring write pointer */
  HOST_DSC               *base_addr;     /* pointer to ring buffer for access*/
  dma_addr_t              dma_addr;      /* physical address of ring buffer */
  mtlk_ring_regs          regs;          /* registers in shared mem and their local copies */
} mtlk_ring_buffer;

typedef uint32 him_descr_t;

typedef struct {
  mtlk_mmb_basic_bdr_t basic;
  mtlk_dlist_t         free_list;
  mtlk_dlist_t         used_list;
  mtlk_osal_spinlock_t lock;
  MTLK_DECLARE_INIT_STATUS;
  MTLK_DECLARE_INIT_LOOP(REG_FREE);
} mtlk_mmb_advanced_bdr_t;

typedef struct
{
  mtlk_lslist_t     lbufs; /* Rx Data Buffers to be re-allocated */
  mtlk_osal_timer_t timer; /* Recovery Timer */
} mtlk_hw_rx_pbufs_t; /* failed RX buffers allocations recovery */

typedef enum
{
  MTLK_ISR_NONE,
  MTLK_ISR_INIT_EVT,
  MTLK_ISR_MSGS_PUMP,
  MTLK_ISR_LAST
} mtlk_hw_mmb_isr_type_e;

typedef struct
{
  uint32 buf_size;
  uint32 data_size;
  void*  data;
} mtlk_bss_rx_eapol;

struct mtlk_bss_management
{
  mtlk_mmb_advanced_bdr_t tx_bdr;  /* Tx BSS control */
  mtlk_mmb_advanced_bdr_t rx_bdr;  /* Rx BSS control */
  mtlk_mmb_advanced_bdr_t tx_res_bdr;  /* Tx BSS reserve control */
  mtlk_ring_buffer        rx_ring;
  mtlk_ring_buffer        tx_ring;
  mtlk_atomic_t           tx_ring_last_cfm_ts;
  uint32                  tx_bdr_max_used_bds; /* Maximal number of used REQ BD descriptors */
  uint32                  tx_res_bdr_max_used_bds; /* Maximal number of used reserved REQ BD descriptors */
  uint64                  cookie_counter;
  mtlk_bss_rx_eapol       rx_eapol; /* Struct for defragmentation of EAPOL frames */
#ifdef BD_DBG
  volatile unsigned long *used_bd_map;   /* bitmap of BD buffers, currently being processed within FW, one per BD */
#endif
};

struct mtlk_tx
{
  mtlk_mmb_advanced_bdr_t bdr_data;      /* Tx Data BDs */
  uint32                  nof_free_bds;  /* Number of free REQ BD descriptors */
  uint32                  max_used_bds;  /* Maximal number of used REQ BD descriptors */
  mtlk_ring_buffer        ring;          /* TX ring data */
#ifdef MTLK_DEBUG
  uint32                  max_received_cfm;      /* max received cfm for packets within one tasklet */
  uint64                  max_int_tasklet_time;  /* time between interrupt to tasklet */
  uint64                  max_int_processed_cfm; /* time between interrupt and cfm processed */
#endif
#ifdef BD_DBG
  volatile unsigned long *used_bd_map;   /* bitmap of BD buffers, currently being processed within FW, one per BD */
#endif
};

struct mtlk_rx
{
  mtlk_mmb_basic_bdr_t    bdr_data;      /* Rx Data BDs */
  mtlk_hw_rx_pbufs_t      pending;       /* Rx Data Buffers recovery */
  mtlk_ring_buffer        ring;          /* TX ring data */
  mtlk_nbuf_t            *first_fragment;/* fragments received and not yet sent up */
  mtlk_nbuf_t            *last_fragment; /* fragments received and not yet sent up */
#ifdef MTLK_DEBUG
  uint32                  max_received_pckts;       /* max received packets within one tasklet */
  uint64                  max_int_tasklet_time;     /* time between interrupt to tasklet */
  uint64                  max_int_processed_packet; /* time between interrupt to packet processed */
#endif
};

#ifdef CPTCFG_IWLWAV_SET_PM_QOS
typedef struct
{
    struct pm_qos_request pm_qos;
} mtlk_pm_qos_t;
#endif

struct mtlk_ctrl
{
  mtlk_ring_buffer        ring;          /* RX ctrl message ring data */
};

#define HW_FLAGS_PREPARED_MAN_REQ_BDR           0x00000001
#define HW_FLAGS_PREPARED_MAN_IND_BDR           0x00000002
#define HW_FLAGS_PREPARED_DBG_REQ_BDR           0x00000004
#define HW_FLAGS_PREPARED_DBG_IND_BDR           0x00000008
#define HW_FLAGS_PREPARED_DATA_REQ_BDR          0x00000010
#define HW_FLAGS_PREPARED_DATA_IND_BDR          0x00000020
#define HW_FLAGS_PREPARED_MGMT_IND_BDR          0x00000040
#define HW_FLAGS_PREPARED_LOG_IND_BDR           0x00000080
#define HW_FLAGS_PREPARED_RX_DATA_BUFFERS       0x00000100
#define HW_FLAGS_PREPARED_RX_MGMT_BUFFERS       0x00000200
#define HW_FLAGS_PREPARED_BSS_REQ_BDR           0x00000400

typedef struct {
  MTLK_DECLARE_START_STATUS;
} hw_start_final_t;

typedef struct {
  MTLK_DECLARE_START_STATUS;
} hw_start_eeprom_t;

typedef void (mtlk_isr_action_fn_t)(mtlk_hw_t *hw, mtlk_irq_handler_data *ihd);

typedef struct _mtlk_hw_mc_t
{
  mtlk_mc_addr_t mc_addr;
  uint32         ref_cnt;
} mtlk_hw_mc_t;

/*****************************************************/
/* HW card info */
typedef struct _mtlk_hw_card_info_t {
  mtlk_card_type_t  card_type;
  uint16            chip_id;
  uint8             chip_type; /* Efused marked, used in WAVE700 for Tri/Dual Band selection */
  uint8             hw_type;
  uint8             hw_vers; /* version/revision */
  BOOL              is_asic; /* ASIC i.e. real chip but not FPGA/Emul */
  BOOL              is_emul; /* Emulation: Palladium i.e. non ASIC */
  BOOL              is_fpga; /* FPGA */
  BOOL              is_phy_dummy; /* FALSE: Real, TRUE: Dummy */
  BOOL              is_secure_boot;
} mtlk_hw_card_info_t;

typedef void (*mtlk_mmb_data_handler_fn_t) (mtlk_irq_handler_data *ihd);


typedef union {
  uint32 words[7];
  VECTOR_AREA_CARD_CONFIGURATION_EXTENSION_DATA card_cfg;
} fw_chi_card_cfg_t;

#if WAVE_USE_BSS_TX_MONITOR
/* BSS TX queue monitor */
typedef struct {
  mtlk_osal_timestamp_t tx_ts;
  IEEE_ADDR dst_addr;
  uint16 sid;
  uint8 used;
  uint8 radio_id;
  uint8 vap_id;
  uint8 type;
  uint8 stype;
} bss_tx_hd_map_t;
#endif

/******************************************************************************/

struct _mtlk_hw_t
{
  mtlk_ccr_t            *ccr;
  mtlk_hw_mmb_t         *mmb;
  unsigned char         *mmb_base;
  unsigned char         *mmb_pas;
  unsigned char         *mmb_base_phy;
  char                  fw_version[512];

  char                  if_version[STRING_IF_VERSIONS_SIZE];
  char                  progmodel[STRING_PROGMODEL_SIZE];

  uint8                 card_idx;

  /* TODO: maybe need it atomic or protected with locks */
  uint8                 rx_hd_ep_mask;     /* from HD: EP bits to Radio index */
  uint8                 rx_hd_vap_mask;    /* from HD: VAP bits to VAP id */
  wave_hw_radio_band_cfg_t radio_band_cfg; /* radio band configuration */
  fw_chi_card_cfg_t      fw_card_cfg;      /* card configuration for fw */

  uint8                  max_vaps_fw; /* FW supported */
  uint8                  max_vaps;    /* Radios config */
  uint16                 max_stas;

  mtlk_eeprom_data_t*   ee_data; /* EEPROM parsed data */
  uint8                 cal_storage_type;
  mtlk_hw_card_info_t   card_info;

  /* mtlk_vap_manager_t    *vap_manager; */
  wave_radio_descr_t    *radio_descr;

  mtlk_hw_state_e        state;
  mtlk_irq_mode_e        irq_mode; /* Save mode of irqs for recovery */
  uint32                 irq_no;   /* IRQ list to be served/enabled */

  VECTOR_AREA            chi_area;

  void                   *calibr_buffer;  /* Calibration Extension related */
  uint32                 calibr_dma_addr;

  uint8                  fw_log_is_supported;    /* FW Logger Extension related */
  uint8                  fw_log_is_supported_ex;
  mtlk_mmb_basic_bdr_t   fw_log_buffers;        /* FW Logger Buffers Queue */

  mtlk_osal_spinlock_t   reg_lock;  /* used in IRQ as irqsave lock */
  mtlk_osal_spinlock_t   version_lock; /* used as spinlock to protect version info */
  volatile int           init_evt; /* used during the initialization */
  mtlk_isr_action_fn_t  *perform_isr_action;
  uint32                 last_irq_status;

  mtlk_hw_ind_req_bd_t   bds;     /* IND/REQ BD */

  struct mtlk_tx         tx;      /* DAT TX-related variables */
#ifdef MTLK_DEBUG
  mtlk_dbg_hres_ts_t     tx_interrupt_time;
#endif

  struct mtlk_ctrl       ctrl;    /* MSG CNTRL related variables */

  struct mtlk_rx         rx;      /* DAT RX-related variables */
#ifdef MTLK_DEBUG
  mtlk_dbg_hres_ts_t     rx_interrupt_time;
#endif

  mtlk_mmb_advanced_bdr_t tx_man;  /* Tx MM related */
  mtlk_mmb_basic_bdr_t    rx_man;  /* Rx MM related */
  mtlk_mmb_advanced_bdr_t tx_dbg;  /* Tx DM related */
  mtlk_mmb_basic_bdr_t    rx_dbg;  /* Rx DM related */

  struct mtlk_bss_management bss_mgmt; /* BSS management frame related things */

  mtlk_txmm_t            txmm;
  mtlk_txmm_t            txdm;

  mtlk_txmm_base_t       txmm_base;
  mtlk_txmm_base_t       txdm_base;
  hw_start_final_t       hw_start_fin;
  hw_start_eeprom_t      hw_start_eeprom;

  int                    mac_events_stopped; /* No INDs must be passed to Core except those needed to perform cleanup */
  int                    mac_events_stopped_completely; /* No INDs must be passed to Core at all*/
  BOOL                   mac_reset_logic_initialized;

  BOOL                   fast_path_is_available;
  BOOL                   dcdp_path_is_available;

  mtlk_irbd_t            *irbd;
  mtlk_wss_t             *wss;
  mtlk_wss_cntr_handle_t *wss_hcntrs[MTLK_HW_SOURCE_CNT_LAST];

  struct {
    mtlk_osal_spinlock_t  lock;
#ifdef MTLK_WAVE_700
    wave_vap_id_t         in_use[WAVE_MAX_SID_GEN7]; /* filled with vap_id_fw unique per HW */
    uint16                aids[WAVE_MAX_SID_GEN7];   /* filled with AIDs for SID */
#else
    wave_vap_id_t         in_use[WAVE_MAX_SID]; /* filled with vap_id_fw unique per HW */
    uint16                aids[WAVE_MAX_SID];   /* filled with AIDs for SID */
#endif
  } sids;

  /* todo: will be moved to radio module */
  hw_phy_rx_status_t    phy_rx_status;

  hw_statistics_t       hw_stats;

  hw_psdb_t             psdb;

  uint8                 tx_ant_num;    /* Number of TX antennas */
  uint8                 rx_ant_num;    /* Number of RX antennas */
  uint8                 tx_ant_mask;   /* TX antennas mask */
  uint8                 rx_ant_mask;   /* RX antennas mask */

  uint32                flags;
  mtlk_atomic_t         radars_detected;    /* Radar detected in operational antennas */
  mtlk_atomic_t         zwdfs_radars_detected;    /* Radars detected in Wav700 zwdfs antenna */

  mtlk_mmb_data_handler_fn_t data_txout_process;
  mtlk_mmb_data_handler_fn_t data_rx_process;

  struct pci_dev       *pdev;
  mtlk_atomic_t         pci_bus_in_stuck;
  mtlk_atomic_t         pci_bus_err_logs_remain;

#if MTLK_USE_DIRECTCONNECT_DP_API
  mtlk_dcdp_dev_t       dp_dev;
#endif

  /* consider using struct tasklet_limits from mtlk_coreui.h here? */
  unsigned              data_txout_lim;
  unsigned              data_rx_lim;
  unsigned              bss_rx_lim;
  unsigned              bss_cfm_lim;
  unsigned              legacy_lim;

  mtlk_osal_spinlock_t  mc_group_lock;                    /* multicast_group lock */
  mtlk_hw_mc_t          mc_groups[MC_GROUPS];
#if CPTCFG_IWLWAV_TRACER_HISTORY_LENGTH
  mtlk_tracer_t         tracer;
#endif

#ifdef CPTCFG_IWLWAV_SET_PM_QOS
  mtlk_pm_qos_t         pm_qos;                           /* To control wakeup latency */
#endif

  /* SID for logger, taken from insmod param, but kept here for fast runtime access */
  uint16 logger_sid;
#ifdef MTLK_WAVE_700
  uint8 logger_fifo_mux_cfg;
#endif
#ifdef CPTCFG_IWLWAV_LEGACY_INT_RECOVERY_MON
  BOOL                 need_legacy_int_recovery;         /* Flag to recover lost legacy interrupt */
  mtlk_osal_timer_t    recover_timer;                    /* Recovery monitor timer */
#endif

  /* counters for BSS RX/TX tasklets execution */
  unsigned              bss_rx_zero_hds_num;
  unsigned              bss_cfm_num;
  unsigned              bss_ind_num;

  /* BSS TX HD map */
#if WAVE_USE_BSS_TX_MONITOR
  bss_tx_hd_map_t       *bss_tx_hd_map;
  uint16                bss_tx_hd_map_size; /* tx mgmt ring hd map size (Not bytes size) */
  mtlk_osal_spinlock_t  bss_tx_hd_map_lock;
#endif

  uint32                stats_poll_period;

  uint8                 chan_util_value[GEN7_NUM_OF_BANDS];
  mtlk_osal_hr_timestamp_t chan_statistics_ts;
  wave_drv_channel_stats_t chan_statistics[NUM_TOTAL_CHAN_STATS_SIZE];
#ifdef CONFIG_WAVE_RTLOG_REMOTE
  BOOL                  rtlog_fw_streams[IWLWAV_RTLOG_FW_MAX_STREAM];
#endif
#ifdef CONFIG_WAVE_DEBUG
  int dbg_unprotected_deauth;
  int dbg_pn_reset;
#endif
  mtlk_osal_spinlock_t   whm_lock;
  BOOL                   whm_enable;
  BOOL                   whm_drv_warn;
  BOOL                   whm_fw_warn;
  BOOL                   whm_phy_warn;
  wave_whm_state_mac     whm_state_machine;
  uint8                  *fw_trace_data;
  BOOL                   pcie_auto_transition; /* enable or disable pcie auto speed change */
  MTLK_DECLARE_INIT_STATUS;
  MTLK_DECLARE_START_STATUS;
};

/******************************************************************************
 * WSS counters for HW card statistics
 */
/**********************************************************************
 **********************************************************************/
/* with checking ALLOWED */
#define _mtlk_mmb_hw_inc_cnt(hw, id)    { if (id##_ALLOWED) __mtlk_mmb_hw_inc_cnt(hw, id); }

static __INLINE void
__mtlk_mmb_hw_inc_cnt(mtlk_hw_t                  *hw,
                     mtlk_hw_source_wss_cnt_id_e cnt_id)
{
  MTLK_ASSERT(cnt_id >= 0 && cnt_id < MTLK_HW_SOURCE_CNT_LAST);

  mtlk_wss_cntr_inc(hw->wss_hcntrs[cnt_id]);
}

/******************************************************************************/

int _mtlk_mmb_bss_mgmt_init(mtlk_hw_t *hw);
int _mtlk_mmb_recover_bss_mgmt(mtlk_hw_t *hw);
void _mtlk_mmb_bss_mgmt_cleanup(mtlk_hw_t *hw);
int _mtlk_mmb_handle_bss_ind(mtlk_hw_t *hw, volatile HOST_DSC *pHD);
int _mtlk_mmb_handle_bss_cfm(mtlk_hw_t *hw, HOST_DSC *pHD);

/******************************************************************/

/******************************************************************/

static __INLINE wave_radio_t *
__mtlk_hw_wave_radio_get (mtlk_hw_t *hw, wave_uint idx)
{
    return wave_radio_descr_wave_radio_get(hw->radio_descr, idx);
}

static __INLINE unsigned compute_stat_index(wave_radio_t *radio, int channel_num)
{
  unsigned idx;
  mtlk_hw_band_e band = wave_radio_band_get(radio);

  if (MTLK_HW_BAND_6_GHZ == band) {
    idx = channum6g2cssidx(channel_num);
    if (CHAN_IDX_ILLEGAL != idx)
      idx += NUM_TOTAL_CHANS;
  }
  else {
    idx = channum2cssidx(channel_num);
  }

  return idx;
}

static __INLINE mtlk_vap_handle_t
_mtlk_mmb_get_vap_handle_from_vap_id (mtlk_hw_t *hw, unsigned radio_idx, uint8 vap_id)
{
  mtlk_vap_handle_t vap_handle = NULL;

  /* Note that vap_hadle will not changed in case of error, so just ignore return code */
  mtlk_vap_manager_get_vap_handle_by_secondary_id(wave_radio_descr_vap_manager_get(hw->radio_descr, radio_idx),
                                                  vap_id, &vap_handle);

  return vap_handle;
}

static __INLINE BOOL
_mtlk_mmb_get_vap_is_not_ready (mtlk_hw_t *hw, unsigned radio_idx, uint8 vap_id)
{
  return mtlk_vap_manager_vap_is_not_ready(wave_radio_descr_vap_manager_get(hw->radio_descr, radio_idx), vap_id);
}

static __INLINE wave_vap_id_t
_mtlk_mmb_get_vap_id_from_vap_handle (mtlk_vap_handle_t vap_handle)
{
  wave_vap_id_t vap_index = 0;
  if (vap_handle) {
    vap_index = mtlk_vap_get_id(vap_handle);
  }

  MTLK_ASSERT(vap_index <= MTLK_BFIELD_GET((uint32)~0, HIM_DESC_VAP));
  return vap_index;
}

#define DATA_REQ_MIRROR_PTR(msg) ((mtlk_hw_data_req_mirror_t *)(msg))
#define MAN_IND_MIRROR_PTR(msg)  ((mtlk_hw_man_ind_mirror_t *)(msg))
#define MAN_DBG_MIRROR_PTR(msg)  ((mtlk_hw_dbg_ind_mirror_t *)(msg))

#define FRAG_FIRST ((1 << 1) | 0) /* SoP == 1, EoP == 0 */
#define FRAG_LAST  ((0 << 1) | 1) /* SoP == 0, EoP == 1 */
#define FRAG_MID   ((0 << 1) | 0) /* SoP == 0, EoP == 0 */
#define NO_FRAG    ((1 << 1) | 1) /* SoP == 1, EoP == 1 */

#define DBG_DUMP_HD     0   /* FIXME: HDs debug */
#define DBG_HW_LOGGER   0   /* FIXME: HW Logger */
#define OWN_ITER_CNTR   100
#define OWN_ITER_CNTR_EMUL   8000

/* Convert card index to wlan interface index of the master AP VAP
 * for hw idx 0/1/2 interface names are wlan0/wlan2/wlan4 respectively
 * AP interfaces use the even numbers while sta interfaces use the odd indexes */
static __INLINE int
card_idx_to_master_vap_idx (int card_idx)
{
  return card_idx * 2;
}

#if MTLK_USE_DIRECTCONNECT_DP_API

static __INLINE BOOL
__mtlk_mmb_cntr_big_endian_supported (mtlk_hw_t *hw)
{
     return TRUE;
}

static __INLINE BOOL
__mtlk_mmb_fastpath_supported(mtlk_hw_t *hw)
{
     return TRUE;
}

static __INLINE BOOL
__mtlk_mmb_fastpath_available(mtlk_hw_t *hw)
{
    return hw->fast_path_is_available;
}

static __INLINE BOOL
__mtlk_mmb_dcdp_path_available(mtlk_hw_t *hw)
{
    /*
    return  (MTLK_DC_DP_MODE_UNREGISTERED  != hw->dp_dev.dp_mode) &&
            (MTLK_DC_DP_MODE_UNKNOWN       != hw->dp_dev.dp_mode) &&
            (MTLK_DC_DP_MODE_LITEPATH_ONLY != hw->dp_dev.dp_mode);
            */
    return hw->dcdp_path_is_available;
}

static __INLINE BOOL
__wave_mmb_dcdp_4umt_cntr_mode(mtlk_hw_t *hw)
{
#ifdef WAVE_DCDP_4UMT_SUPPORTED
  return (hw->dp_dev.dp_cap.fastpath.hw_dcmode == DC_DP_MODE_TYPE_1_EXT) &&
         (hw->dp_dev.dp_cap.fastpath.hw_cmode.soc2dev_write &
          DC_DP_F_DCCNTR_MODE_4MSG_MODE) &&
         (hw->dp_dev.dp_cap.fastpath.hw_cmode.soc2dev_write &
          DC_DP_F_DCCNTR_MODE_INCREMENTAL);
#else
  return FALSE;
#endif /* WAVE_DCDP_4UMT_SUPPORTED */
}

#else /* MTLK_USE_DIRECTCONNECT_DP_API */

#define __mtlk_mmb_fastpath_supported(hw)  (FALSE)
#define __mtlk_mmb_fastpath_available(hw)  (FALSE)
#define __mtlk_mmb_dcdp_path_available(hw) (FALSE)
#define __wave_mmb_dcdp_4umt_cntr_mode(hw) (FALSE)
/* mtlk_mmb_fastpath_available() and mtlk_mmb_fastpath_available() are defined in hw_mmb.h */

#endif /* MTLK_USE_DIRECTCONNECT_DP_API */

static __INLINE char* find_print_position(char *text, int* bytes_left)
{
  int   len = wave_strlen(text, *bytes_left);
  char* print_position = text + len;
  *bytes_left -= len;
  return print_position;
}


/* Word offset for Base/Current release numbers depends on HW */
#define WAVE_GENRISC_BASE_RELEASE_WORD_OFFS_G6_A0_B0    0x10
#define WAVE_GENRISC_BASE_RELEASE_WORD_OFFS_G6_D2_G7    0x19

#define MTLK_GENRISC_LOCAL_BINARY_FLAG   (1<<16)
#define WAV700_LOGGER_6G_LAST_SID   511
/******************************************************************************
 * HW Platform specific
 */

static __INLINE BOOL
__hw_mmb_card_is_asic (mtlk_hw_t *hw)
{
    return hw->card_info.is_asic;
}

static __INLINE BOOL
__hw_mmb_card_is_emul (mtlk_hw_t *hw)
{
    return hw->card_info.is_emul;
}

static __INLINE BOOL
__hw_mmb_card_is_phy_real (mtlk_hw_t *hw)
{
    return !hw->card_info.is_phy_dummy;
}

static __INLINE BOOL
__hw_mmb_card_is_phy_dummy (mtlk_hw_t *hw)
{
    return hw->card_info.is_phy_dummy;
}

static __INLINE BOOL
__hw_mmb_card_is_fpga (mtlk_hw_t *hw)
{
    return hw->card_info.is_fpga;
}

static __INLINE BOOL
__hw_mmb_card_is_secure_boot (mtlk_hw_t *hw)
{
    return hw->card_info.is_secure_boot;
}

static __INLINE BOOL
_hw_type_is_gen6(mtlk_hw_t *hw)
{
  return _chipid_is_gen6(hw->ccr->chip_info->id);
}

static __INLINE BOOL
_hw_type_is_gen6_b0 (mtlk_hw_t *hw)
{
  return _chipid_is_gen6_b0(hw->ccr->chip_info->id);
}

static __INLINE BOOL
_hw_type_is_gen6_d2 (mtlk_hw_t *hw)
{
  return _chipid_is_gen6_d2(hw->ccr->chip_info->id);
}

static __INLINE BOOL
_hw_type_is_gen6_d2_or_gen7 (mtlk_hw_t *hw)
{
  return (_chipid_is_gen6_d2_or_gen7(hw->ccr->chip_info->id));
}

static __INLINE BOOL
_hw_type_is_gen7 (mtlk_hw_t *hw)
{
  return _chipid_is_gen7(hw->ccr->chip_info->id);
}

static __INLINE BOOL
_mtlk_is_gen7_dual_band (mtlk_hw_t *hw)
{
  return _chip_type_gen7_is_dual_band(hw->card_info.chip_type);
}

static __INLINE BOOL
_mtlk_is_gen7_triple_band (mtlk_hw_t *hw)
{
  return _chip_type_gen7_is_triple_band(hw->card_info.chip_type);
}


/* FIXME D.2 - currently there is support for PHY_STATISTICS_MAX_RX_ANT_D2 in driver */
static __INLINE unsigned
_hw_num_statistics_antennas (mtlk_hw_t *hw)
{
  return _hw_type_is_gen6_d2_or_gen7(hw) ? PHY_STATISTICS_MAX_RX_ANT_D2 : PHY_STATISTICS_MAX_RX_ANT;
}

void __MTLK_IFUNC   mtlk_psdb_data_cleanup (hw_psdb_t *psdb);
int  __MTLK_IFUNC   mtlk_psdb_file_read_and_parse(mtlk_hw_t* hw, mtlk_hw_api_t *hw_api, hw_psdb_t *psdb,
                              uint32 chip_id, uint32 chip_type, uint32 hw_type,
                              uint32 hw_rev, bool band6g_supported);

static __INLINE hw_psdb_t *
_mtlk_hw_get_psdb (mtlk_hw_t *hw)
{
    return &hw->psdb;
}

/******************************************************************************
 * Composition of the Firmware file name
 */

#define FW_AP_NAME_PREFFIX            "ap"
#define FW_STA_NAME_PREFFIX           "sta"

#define FW_NAME_SUFFIX_NONE           ""
#define FW_NAME_SUFFIX_WRX500         "_wrx_500"
#define FW_NAME_SUFFIX_WRX514         "_wrx_514"

#define FW_NAME_SUFFIX_WRX600A        "_wrx_600"
#define FW_NAME_SUFFIX_WRX600B        "_wrx_600b"
#define FW_NAME_SUFFIX_WRX600D2       "_wrx_600d2"
#define FW_NAME_SUFFIX_WRX600D2B      "_wrx_600d2b"

#define FW_NAME_SUFFIX_WRX700         "_wrx_700"
#define FW_NAME_SUFFIX_WRX700_MIX     "_wrx_700_mixed"

#define FW_PHY_TYPE_SUFFIX_DUMMY      "dummy"
#define FW_PHY_TYPE_SUFFIX_REAL       "real"

#define FW_ARCH_TYPE_SUFFIX_SB        "_sb"
#define FW_ARCH_TYPE_SUFFIX_CDB       "_cdb"

#define FW_UPPER_NAME_FMT             "%s_upper_%s%s.bin"
#define FW_LOWER_NAME_FMT             "%s_lower_%s%s.bin"

#define FW_TRANS_NAME_FMT             "tx_sender_%s.bin"
#define FW_RXHANDLER_NAME_FMT         "rx_handler_%s.bin"
#define FW_HOSTIF_NAME_FMT            "host_interface_%s%s.bin"

#define PROGMODEL_MODIFIED_MASK    0x80000000u
#define PROGMODEL_VERSION_MASK     0x7FFFFFFFu
#define PROGMODEL_VER_MODIFIED_BIT 31
#define PROGMODEL_VER_OLD_BIT      29

#define WAVE_SIZE_TO_U32(n_) ((n_) / sizeof(uint32))

/******************************************************************************
 * INIT/CLAENUP step definitions
 */
MTLK_INIT_STEPS_LIST_BEGIN(mmb_basic_bdr)
  MTLK_INIT_STEPS_LIST_ENTRY(mmb_basic_bdr, ALLOC_HST_BDR_MIRROR)
MTLK_INIT_INNER_STEPS_BEGIN(mmb_basic_bdr)
MTLK_INIT_STEPS_LIST_END(mmb_basic_bdr);

MTLK_INIT_STEPS_LIST_BEGIN(mmb_advanced_bdr)
  MTLK_INIT_STEPS_LIST_ENTRY(mmb_advanced_bdr, BASIC_BDR)
  MTLK_INIT_STEPS_LIST_ENTRY(mmb_advanced_bdr, LIST_LOCK)
  MTLK_INIT_STEPS_LIST_ENTRY(mmb_advanced_bdr, FREE_LIST)
  MTLK_INIT_STEPS_LIST_ENTRY(mmb_advanced_bdr, REG_FREE)
  MTLK_INIT_STEPS_LIST_ENTRY(mmb_advanced_bdr, USED_LIST)
MTLK_INIT_INNER_STEPS_BEGIN(mmb_advanced_bdr)
MTLK_INIT_STEPS_LIST_END(mmb_advanced_bdr);

MTLK_INIT_STEPS_LIST_BEGIN(hw_mmb_card)
#if CPTCFG_IWLWAV_TRACER_HISTORY_LENGTH
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_TRACER_INIT)
#endif
#if MTLK_USE_DIRECTCONNECT_DP_API
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_DCDP_INIT)
#endif
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_DATA_HANDLERS_INIT)
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_IRB_ALLOC)
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_IRB_INIT)
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_WSS_CREATE)
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_SOURCE_CNTRs)
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_RX_DATA_LIST)
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_REG_LOCK)
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_VERSION_LOCK)
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_REQ_MC_LOCK)
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_INIT_EVT)
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_WHM_LOCK)
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_RX_PEND_TIMER)
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_RX_TX_LIMITS)
#ifdef CPTCFG_IWLWAV_SET_PM_QOS
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_PM_QOS_INIT)
#endif
#ifdef CPTCFG_IWLWAV_LEGACY_INT_RECOVERY_MON
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_INT_REC_MON)
#endif
MTLK_INIT_INNER_STEPS_BEGIN(hw_mmb_card)
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb_card, HW_IRB_NAME)
MTLK_INIT_STEPS_LIST_END(hw_mmb_card);

MTLK_START_STEPS_LIST_BEGIN(hw_mmb_card)
#ifdef CPTCFG_IWLWAV_PMCU_SUPPORT
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_PMCU_INIT_CLIENT)
#endif /*CPTCFG_IWLWAV_PMCU_SUPPORT */
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_POWER_ON)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_BIST_READ)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_INIT_PLL)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_CHI_INIT)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_LOAD_FIRMWARE)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_RUN_FIRMWARE)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_WAIT_CHI_MAGIC)
#if MTLK_DCDP_SEPARATE_REG
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_DATA_PATH_REGISTER)
#endif
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_MAN_REQ_BDR)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_MAN_IND_BDR)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_DBG_REQ_BDR)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_DBG_IND_BDR)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_DAT_REQ_BDR)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_DAT_IND_BDR)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_BSS_MGMT)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_RX_DATA_BUFFERS)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_RX_MGMT_BUFFERS)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_CALIBRATION_CACHE)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card, HW_MIPS_CONTROL)
MTLK_START_INNER_STEPS_BEGIN(hw_mmb_card)
MTLK_START_STEPS_LIST_END(hw_mmb_card);

MTLK_START_STEPS_LIST_BEGIN(hw_mmb_card_fin)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card_fin, HW_TXMM)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card_fin, HW_TXDM)
#if !MTLK_DCDP_SEPARATE_REG
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card_fin, HW_DATA_PATH_REGISTER)
#endif
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card_fin, HW_DATA_PATH)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card_fin, HW_HW_DEP_CFG)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card_fin, HW_FW_LOG_SEVERITY)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card_fin, HW_RX_PEND_TIMER)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card_fin, HW_STATISTICS)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_card_fin, HW_SIDS)
MTLK_START_INNER_STEPS_BEGIN(hw_mmb_card_fin)
MTLK_START_STEPS_LIST_END(hw_mmb_card_fin);

MTLK_START_STEPS_LIST_BEGIN(hw_mmb_eeprom)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_eeprom, HW_EEPROM_READ)
  MTLK_START_STEPS_LIST_ENTRY(hw_mmb_eeprom, HW_PSDB_READ)
MTLK_START_INNER_STEPS_BEGIN(hw_mmb_eeprom)
MTLK_START_STEPS_LIST_END(hw_mmb_eeprom);

MTLK_INIT_STEPS_LIST_BEGIN(hw_mmb)
  MTLK_INIT_STEPS_LIST_ENTRY(hw_mmb, HW_MMB_LOCK)
MTLK_INIT_INNER_STEPS_BEGIN(hw_mmb)
MTLK_INIT_STEPS_LIST_END(hw_mmb);

/******************************************************************************
 * HW Statistics
 */
#include "Statistics/WAVE600B/Statistics_Descriptors.h"
#include "Statistics/WAVE600D2/Statistics_Descriptors.h"
#include "Statistics/WAVE700/Statistics_Descriptors.h"

/* Enable the use of HW TSF as Channel statistics timestamp.
 * Otherwise the kernel timestamp will be used instead.
 */
#define WAVE_HW_CHAN_STATS_TSF_ENABLE

static __INLINE mtlk_osal_hr_timestamp_t
wave_hw_chan_stats_ts (mtlk_hw_t *hw)
{
#if defined(WAVE_HW_CHAN_STATS_TSF_ENABLE) && defined(CPTCFG_IWLWAV_TSF_TIMER_ACCESS_ENABLED)
  return wave_hw_get_timestamp_64(hw);
#else
  return mtlk_osal_hr_timestamp_us();
#endif
}

int  __MTLK_IFUNC hw_init_phy_rx_status (mtlk_hw_t *hw);
void __MTLK_IFUNC hw_cleanup_phy_rx_status (mtlk_hw_t *hw);
int  __MTLK_IFUNC mtlk_hw_statistics_init (mtlk_hw_t *hw);
void __MTLK_IFUNC mtlk_hw_statistics_cleanup (mtlk_hw_t *hw);

static __INLINE uint32
__mtlk_hw_get_all_sta_sid (mtlk_hw_t *hw)
{
    return hw->phy_rx_status.all_sta_sid;
}

static __INLINE uint32
__mtlk_hw_get_max_sid (mtlk_hw_t *hw)
{
    return hw->phy_rx_status.max_sid;
}

static __INLINE BOOL
mtlk_hw_is_sid_valid (mtlk_hw_t *hw, uint32 sid)
{
    return (sid < __mtlk_hw_get_max_sid(hw));
}


#undef LOG_LOCAL_GID
#undef LOG_LOCAL_FID

#endif /* __HW_MMB_PRIV_H__ */
