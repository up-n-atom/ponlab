#ifndef _DRV_SDD_MBX_TRACE_H
#define _DRV_SDD_MBX_TRACE_H
/******************************************************************************

                            Copyright (c) 2014-2015
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/**
   \file drv_sdd_mbx_trace.h
   This file contains the implementation of tracing functions.
 */

/* ========================================================================== */
/*                                 Includes                                   */
/* ========================================================================== */
#include "ifx_types.h"

/* ========================================================================== */
/*                            Macro definitions                               */
/* ========================================================================== */
#define TRACE_ERROR        3
#define TRACE_INFO         2
#define TRACE_DEBUG        1

/* ========================================================================== */
/*                           Function prototypes                              */
/* ========================================================================== */
extern void sdd_mbx_trace(IFX_uint8_t level, const IFX_char_t* function, const IFX_char_t* message, ...);

#define TRACE(format, args...) sdd_mbx_trace(TRACE_DEBUG, __FUNCTION__, format, ## args)

#define ERROR(format, args...) sdd_mbx_trace(TRACE_ERROR, __FUNCTION__, format, ## args)

#endif /* _DRV_SDD_MBX_LINUX_H */
