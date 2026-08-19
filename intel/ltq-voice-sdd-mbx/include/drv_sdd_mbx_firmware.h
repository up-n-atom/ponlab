#ifndef _DRV_SDD_MBX_FIRMWARE_H
#define _DRV_SDD_MBX_FIRMWARE_H
/******************************************************************************

                            Copyright (c) 2014-2015
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/**
   \file drv_sdd_mbx_firmware.h
   This file contains the declaration of the download and CRC structures,
   macros and functions.
*/

/* ========================================================================== */
/*                                 Includes                                   */
/* ========================================================================== */
#include "drv_sdd_mbx_handler.h"

/* ========================================================================== */
/*                             Type definitions                               */
/* ========================================================================== */
typedef enum
{
   /** Boot directly from ROM */
   SDD_MBX_BOOT_ROM = 0,
   /** Boot from i/o address SPI command inbox (FW patch download) */
   SDD_MBX_BOOT_SPI
} SDD_MBX_BOOT_SOURCE_t;

/* ========================================================================== */
/*                           Function prototypes                              */
/* ========================================================================== */
extern IFX_int32_t sdd_mbx_firmware(
                        SDD_MBX_DEVICE_t* pDev,
                        IFX_uint8_t *pFwImage,
                        IFX_uint32_t nFwSize);

extern IFX_int32_t sdd_mbx_reset_controller (
                        SDD_MBX_DEVICE_t* pDev,
                        SDD_MBX_BOOT_SOURCE_t bootInfo);
#endif /* _DRV_SDD_MBX_FIRMWARE_H */
