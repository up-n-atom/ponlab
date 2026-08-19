#ifndef _DRV_SDD_MBX_INTERRUPT_H
#define _DRV_SDD_MBX_INTERRUPT_H
/******************************************************************************

                            Copyright (c) 2014-2015
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/**
   \file drv_sdd_mbx_interrupt.h
   This file contains the implementation of interrupt handling.
*/

/* ========================================================================== */
/*                                 Includes                                   */
/* ========================================================================== */
#include "ifx_types.h"

/* ========================================================================== */
/*                           Function prototypes                              */
/* ========================================================================== */
extern IFX_int32_t sdd_mbx_irq_register(void);
extern IFX_void_t  sdd_mbx_irq_unregister(void);
extern IFX_int32_t sdd_mbx_irq_vcodec_enable(SDD_MBX_DEVICE_t* pDev);
extern IFX_void_t  sdd_mbx_irq_system_enable(SDD_MBX_DEVICE_t* pDev);
extern IFX_void_t  sdd_mbx_irq_system_disable(SDD_MBX_DEVICE_t* pDev);
extern IFX_int32_t sdd_mbx_irq_host_enable(SDD_MBX_DEVICE_t* pDev,
                                      SDD_MBX_t mbx);
extern IFX_int32_t sdd_mbx_irq_host_disable(SDD_MBX_DEVICE_t* pDev, SDD_MBX_t mbx);
#endif /* _DRV_SDD_MBX_INTERRUPT_H */
