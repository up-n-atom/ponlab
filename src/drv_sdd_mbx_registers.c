/******************************************************************************

                            Copyright (c) 2014-2015
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/**
   \file drv_sdd_mbx_registers.c
   This file contains the implementation of register layer of the driver.
 */

/* ========================================================================== */
/*                                 Includes                                   */
/* ========================================================================== */
#include "drv_sdd_mbx_spi.h"
#include "drv_sdd_mbx_registers.h"

/* ========================================================================== */
/*                         Function implementation                            */
/* ========================================================================== */
IFX_int32_t sdd_mbx_reg_read(SDD_MBX_DEVICE_t *pDev, IFX_uint8_t offset,
                                 IFX_uint16_t *pbuf,
                                 IFX_uint32_t len)
{
   IFX_int32_t ret = SDD_FW_ERR_NO_ERROR;
   ret = sdd_mbx_spi_read(pDev, offset, pbuf, len << 1);

#ifdef EVENT_LOGGER_DEBUG
   EL_LOG_EVENT_REG_RD(DEV_TYPE_SDD, 0, 0, offset, pbuf, len);
#endif

   return ret;
}

IFX_int32_t sdd_mbx_reg_read_word(SDD_MBX_DEVICE_t *pDev, IFX_uint8_t offset,
                                 IFX_uint16_t *pbuf)
{
   return sdd_mbx_reg_read(pDev, offset, pbuf, 1);
}

IFX_int32_t sdd_mbx_reg_write(SDD_MBX_DEVICE_t *pDev, IFX_uint8_t offset,
                                 IFX_uint16_t *pbuf,
                                 IFX_uint32_t len)
{
   IFX_int32_t ret = SDD_FW_ERR_NO_ERROR;
   ret = sdd_mbx_spi_write(pDev, offset, pbuf, len << 1);

#ifdef EVENT_LOGGER_DEBUG
   EL_LOG_EVENT_REG_WR(DEV_TYPE_SDD, 0, 0, offset, pbuf, len);
#endif

   return ret;
}

IFX_int32_t sdd_mbx_reg_write_word(SDD_MBX_DEVICE_t *pDev, IFX_uint8_t offset,
                                 IFX_uint16_t *pbuf)
{
   return sdd_mbx_reg_write(pDev, offset, pbuf, 1);
}
