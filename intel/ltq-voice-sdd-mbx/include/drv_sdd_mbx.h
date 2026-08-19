#ifndef _DRV_SDD_MBX_H
#define _DRV_SDD_MBX_H
/*******************************************************************************

                            Copyright (c) 2014-2015
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/**
   \file drv_sdd_mbx.h
   This file contains the defines specific to the SDD driver interface
   and is used by firmware driver.
 */

/* ========================================================================== */
/*                                 Includes                                   */
/* ========================================================================== */
#include "ifx_types.h"

/* ========================================================================== */
/*                             Macro definitions                              */
/* ========================================================================== */
/** Event data buffer length */
#define SDD_MBX_EVENT_SIZE   16

/* ========================================================================== */
/*                             Type definitions                               */
/* ========================================================================== */
/* SDD FW driver error codes */
enum e_SDD_FW_Errors
{
   SDD_FW_ERR_NO_ERROR = IFX_SUCCESS,
   SDD_FW_ERR_INVALID_PARAMS,
   SDD_FW_ERR_NO_MEMORY,
   SDD_FW_ERR_MBX_READ,
   SDD_FW_ERR_DOWNLOAD_BINARY,
   SDD_FW_ERR_FW_DWLD_TIMEOUT,
   SDD_FW_ERR_WAIT_TIMEOUT,
   SDD_FW_ERR_ENODATA,
   SDD_FW_ERR_WAIT_INT,
   SDD_FW_ERR_SPI_ACCESS,
   SDD_FW_ERR_CTRL_RES,
   SDD_FW_ERR_SET_BOOT_CFG,
   SDD_FW_ERR_CMD_MBX_NO_SPACE,
   SDD_FW_ERR_CMD_MBX_OFL,
   SDD_FW_ERR_CMD_MBX_UFL,
   SDD_FW_ERR_EVT_MBX_OFL,
   SDD_FW_ERR_EVT_MBX_UFL
};

/* IOCTL indexes */
enum
{
   SDD_MBX_DEBUG_MODE_IDX,
   SDD_MBX_RESET_IDX,
   SDD_MBX_REG_READ_IDX,
   SDD_MBX_REG_WRITE_IDX,
   SDD_MBX_EVT_READ_IDX,
   SDD_MBX_CMD_READ_IDX,
   SDD_MBX_CMD_WRITE_IDX,
   SDD_MBX_FW_DOWNLOAD_IDX
};

/* Debug interface modes */
enum SDD_Debug_Mode_t
{
   SDD_DEBUG_OFF,
   SDD_DEBUG_ON
};

/* Structure used for GRX 500 firmware download. */
typedef struct
{
   /* Device index. */
   IFX_uint16_t dev;
   /* Valid Firmware byte pointer */
   IFX_uint8_t    *pPRAMfw;
   /* Size of firmware in bytes */
   IFX_uint32_t   pram_size;
} SDD_MBX_FW_Download_t;

/* Structure for event IOCTL data. */
typedef struct
{
   /** SDD event data */
   IFX_uint32_t data [SDD_MBX_EVENT_SIZE];
} FIO_SDD_MBX_READ_t;

/* Structure for read the register data. */
typedef struct
{
   /* Offset of register */
   IFX_uint16_t reg;
   /* Buffer for data */
   IFX_uint16_t data;
} FIO_SDD_MBX_REG_IO_t;

/* Mailbox handler type */
typedef IFX_uint8_t SDD_MBX_t;

/* Firmware driver interface functions */
typedef struct
{
   /** Open the mailbox
      \param s       Mailbox name that can be either "command" or "event"
      \return
         SDD_FW_ERR_NO_ERROR on success or e_SDD_FW_Errors in case of an error.
   */
   IFX_int32_t (*open)(IFX_char_t *s);

   /** Close the mailbox
      \param s       Mailbox name that can be either "command" or "event"
      \return
         SDD_FW_ERR_NO_ERROR on success or e_SDD_FW_Errors in case of an error.
   */
   IFX_int32_t (*close)(IFX_char_t *s);

   /** Download and start a firmware
      \param  pFwImage     Pointer to address with FW image.
      \param  nFwSize      Size of the firmware image in bytes.
      \return
         SDD_FW_ERR_NO_ERROR on success or e_SDD_FW_Errors in case of an error.
   */
   IFX_int32_t (*fw_dwld)(IFX_uint8_t *pFwImage, IFX_uint32_t nFwSize);

   /** Execute a command read
      \param pData    Starting of buffer into which the data will be stored
      \param len      Buffer size
      \return
         SDD_FW_ERR_NO_ERROR on success or e_SDD_FW_Errors in case of an error.
   */
   IFX_int32_t (*cmd_read)(IFX_uint8_t *pData, IFX_uint16_t len);

   /** Execute a command write
       \param pData    Pointer to a buffer to be written
       \param len      Byte count to be written
       \return
         SDD_FW_ERR_NO_ERROR on success or e_SDD_FW_Errors in case of an error.
   */
   IFX_int32_t (*cmd_write)(IFX_uint8_t *pData, IFX_uint16_t len);

   /** This function is executed as a callback during VCODEC IRQ. This function
       should procees the event, without blocking.
      \param pData   Pointer to readed data
      \param len     Readed data length
      \return
         SDD_FW_ERR_NO_ERROR on success or e_SDD_FW_Errors in case of an error.
    */
   IFX_int32_t (*evt_read_callback)(IFX_uint8_t *pData, IFX_uint16_t len);

   /** This function is executed as a callback when VCODEC error occurs. In case
       of error IRQ is disabled until reset.

      \param error   Error number
      \return IFX_SUCCESS
   */
   IFX_int32_t (*error_callback)(IFX_int32_t error);

   /** Mailbox ioctl
       \param cmd      Ioctl command
       \param param    Ioctl command parameters
       \return
         SDD_FW_ERR_NO_ERROR on success or e_SDD_FW_Errors in case of an error.
   */
   IFX_int32_t (*ioctl)(IFX_uint32_t cmd, IFX_void_t *param);

   /** This function count additional delay in the driver. It is needed in case
       of event logger and debug traces level.

      \return
         IFX_int32_t delay time in ms.
   */
   IFX_int32_t (*path_delay)(IFX_void_t);

   /**
   Get the DC/DC converter name
   \param  pDcDcName    Pointer to buffer where to copy the name.
   \param  nNameLength  Length of the buffer in bytes.
   \return
    Last error code
   */
   IFX_int32_t (*dcdc_name)(IFX_char_t **pDcDcName);
} SDD_FIRMWARE_IF_t;

/* ========================================================================== */
/*                                 IOCTLS                                     */
/* ========================================================================== */
#define SDD_MBX_IOCTL_MAGIC 'X'

#define SDD_MBX_DEBUG_MODE  _IOW  (SDD_MBX_IOCTL_MAGIC, SDD_MBX_DEBUG_MODE_IDX,\
                                   enum SDD_Debug_Mode_t)
#define SDD_MBX_RESET       _IO   (SDD_MBX_IOCTL_MAGIC, SDD_MBX_RESET_IDX)

#define SDD_MBX_REG_READ    _IOWR (SDD_MBX_IOCTL_MAGIC, SDD_MBX_REG_READ_IDX,  \
                                   FIO_SDD_MBX_REG_IO_t)
#define SDD_MBX_REG_WRITE   _IOW  (SDD_MBX_IOCTL_MAGIC, SDD_MBX_REG_WRITE_IDX,  \
                                   FIO_SDD_MBX_REG_IO_t)
#define SDD_MBX_EVT_READ    _IOW  (SDD_MBX_IOCTL_MAGIC, SDD_MBX_EVT_READ_IDX,   \
                                   FIO_SDD_MBX_READ_t)
#define SDD_MBX_CMD_READ    _IOW  (SDD_MBX_IOCTL_MAGIC, SDD_MBX_CMD_READ_IDX,   \
                                   FIO_SDD_MBX_READ_t)
#define SDD_MBX_CMD_WRITE   _IOW  (SDD_MBX_IOCTL_MAGIC, SDD_MBX_CMD_WRITE_IDX,  \
                                   FIO_SDD_MBX_READ_t)
#define SDD_MBX_FW_DOWNLOAD _IOW  (SDD_MBX_IOCTL_MAGIC, SDD_MBX_FW_DOWNLOAD_IDX,\
                                   SDD_MBX_FW_Download_t)
/* ========================================================================== */
/*                           Function prototypes                              */
/* ========================================================================== */
extern SDD_FIRMWARE_IF_t* sdd_mbx_interface_get(IFX_void_t);
extern IFX_void_t sdd_mbx_interface_release(IFX_void_t);

#endif /* _DRV_SDD_MBX_H */
