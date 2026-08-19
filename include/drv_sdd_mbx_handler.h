#ifndef _DRV_SDD_MBX_HANDLER_H
#define _DRV_SDD_MBX_HANDLER_H
/******************************************************************************

                            Copyright (c) 2014-2015
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/**
   \file drv_sdd_mbx_handler.h
   This file contains the implementation of TAPI Command line interface
   functions.
 */

/* ========================================================================== */
/*                                 Includes                                   */
/* ========================================================================== */
#include "drv_sdd_mbx.h"
#include "drv_sdd_mbx_config.h"
#include "ifxos_mutex.h"
#include "ifxos_event.h"
#include "ifxos_thread.h"
#include <linux/clk.h>

#ifdef DEBUG
   /* enable devfs filesystem */
   #define SDD_MBX_WITH_DEV_FS 1

   /* enable proc filesystem */
   #define SDD_MBX_WITH_PROC_FS 1
#endif

/* ========================================================================== */
/*                            Macro definitions                               */
/* ========================================================================== */
#define SDD_MBX_COMMAND_BUFFER_LENGTH 64

/* ========================================================================== */
/*                              Local variables                               */
/* ========================================================================== */
/* This structure holds device specific data. */
typedef struct
{
   /* SPI device interface */
   struct spi_device       *pSpiDev;
   /* 8kHz clock to the VCODEC */
   struct clk              *vcodec_clk;
   /* 8MHz clock to the SmartSLIC */
   struct clk              *sslic_clk;
   /* Status of device, see states defines. */
   IFX_vuint32_t           nDevState;
   /* IRQ number */
   irq_hw_number_t         irq;
   /* IRQ disable flag. Protection for unbalanced enable of IRQ line. */
   IFX_vuint32_t           irq_enabled;
   /* Command read data length */
   IFX_vuint32_t            cmd_buffer_length;
   /* Command buffer */
   IFX_uint8_t             cmd_buffer[SDD_MBX_COMMAND_BUFFER_LENGTH];
   /* Semaphore for events */
   IFXOS_event_t           cmd_irq_evt;
   /* Vcodec error flag */
   IFX_vint32_t            vcodec_error;
   /* Protection for SPI transaction */
   struct semaphore        spi_access_mtx;
   /* Protection for command read */
   struct mutex            cmd_mtx;
} SDD_MBX_DEVICE_t;

/* This structure holds the driver data. */
typedef struct
{
   /* Pointer to procfs */
#ifdef SDD_MBX_WITH_PROC_FS
   struct proc_dir_entry*  proc_dir;
#endif /* SDD_MBX_WITH_PROC_FS */
   /* Printing level filter */
   IFX_uint8_t             trace_level;
   /* Char device major */
   IFX_uint16_t            major_number;
   /* Char device minor */
   IFX_uint16_t            minor_number;
   /* Device state structure */
   SDD_MBX_DEVICE_t        device;
   /* Debug mode - stop processing events */
   enum SDD_Debug_Mode_t   debug_mode;
   /* Interface structure for HL driver */
   SDD_FIRMWARE_IF_t*      interface;
} SDD_MBX_CONTEXT_t;


/* ========================================================================== */
/*                              Bit operations                                */
/* ========================================================================== */
#define SET_BIT(value, bit)   ((value) |= (1<<(bit)))
#define CLEAR_BIT(value, bit) ((value) &= ~(1<<(bit)))
#define CHECK_BIT(value, bit) ((value & (1 << (bit))) == (1 << (bit)))

/* ========================================================================== */
/*                              Device states                                 */
/* ========================================================================== */
/* Firmware successfully downloaded */
#define   DS_FW_DLD                0x000001

/* Device is up (see system event: UP) */
#define   DS_DEV_UP                0x000002

/* SPI interface is active */
#define   DS_SPI_ACTIVE            0x000080

/* SPI interface setup phase */
#define   DS_SPI_SETUP             0x000100

/* ========================================================================== */
/*                           Function prototypes                              */
/* ========================================================================== */
extern void sdd_mbx_register_interface(void);
extern void sdd_mbx_unregister_interface(void);
extern IFX_int32_t sdd_mbx_ioctl_handler(IFX_uint32_t cmd, IFX_void_t *param);
extern IFX_int32_t sdd_mbx_event_read(SDD_MBX_DEVICE_t *pDev, IFX_uint8_t *evt,
                                      IFX_uint32_t buffer_len);
extern IFX_int32_t sdd_mbx_command_read(SDD_MBX_DEVICE_t *pDev);
#endif /* _DRV_SDD_MBX_HANDLER_H */
