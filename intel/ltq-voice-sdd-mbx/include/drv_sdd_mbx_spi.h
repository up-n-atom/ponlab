#ifndef _DRV_SDD_MBX_SPI_H
#define _DRV_SDD_MBX_SPI_H
/******************************************************************************

                            Copyright (c) 2014-2015
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/**
   \file drv_sdd_mbx_spi.h
   SPI access functions.
*/

/* ========================================================================== */
/*                                 Includes                                   */
/* ========================================================================== */
#include "drv_sdd_mbx_handler.h"

/* ========================================================================== */
/*                            Macro definitions                               */
/* ========================================================================== */
/* The SPI interface buffer has a size of 32 bytes. This is also the maximum
   number of bytes that the SVIP SPI driver accepts in each call of
   spi_write_then_read() below.
   Limited to 16 bytes as workaround for SPI driver problem VOICECPE_SW-1010. */
#define SPI_MAXBYTES_SIZE                16

/* ========================================================================== */
/*                           Function prototypes                              */
/* ========================================================================== */
extern IFX_int32_t sdd_mbx_spi_init(SDD_MBX_DEVICE_t *pDev);

extern IFX_int32_t sdd_mbx_spi_exit(SDD_MBX_DEVICE_t *pDev);

extern IFX_int32_t sdd_mbx_spi_read(SDD_MBX_DEVICE_t *pDev, IFX_uint8_t offset,
                                 IFX_uint16_t *pbuf,
                                 IFX_uint32_t len);

extern IFX_int32_t sdd_mbx_spi_write(SDD_MBX_DEVICE_t *pDev, IFX_uint8_t offset,
                                 IFX_uint16_t *pbuf,
                                 IFX_uint32_t len);

extern void sdd_mbx_cpb2w (
                                 IFX_uint16_t *pWbuf,
                                 const IFX_uint8_t * const pBbuf,
                                 const IFX_uint32_t nB);

extern IFX_uint32_t SDD_MBX_spi_blocksize_get(void);

#endif /* _DRV_SDD_MBX_SPI_H */
