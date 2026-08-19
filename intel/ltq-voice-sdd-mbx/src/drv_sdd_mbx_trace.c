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
#include <linux/sched.h>
#include "drv_sdd_mbx_handler.h"
#include "drv_sdd_mbx_trace.h"

/* ========================================================================== */
/*                             Local variables                                */
/* ========================================================================== */
extern SDD_MBX_CONTEXT_t sdd_mbx_context;

/* ========================================================================== */
/*                         Function implementation                            */
/* ========================================================================== */
void sdd_mbx_trace(uint8_t level, const IFX_char_t* function, const char* message, ...)
{
   va_list args;
   char buffer[256];

   if(level < sdd_mbx_context.trace_level)
   {
      return;
   }

   va_start(args, message);
   vsnprintf(buffer, 256, message, args);
   va_end(args);

   if(level == TRACE_ERROR)
   {
      printk(KERN_ERR "SDD MBX: [CPU: %d] (%d): %s() %s!\n",
             smp_processor_id(), current->pid, function, buffer);
   }

   if((level == TRACE_INFO) || (level == TRACE_DEBUG))
   {
      printk(KERN_DEBUG "SDD MBX: [CPU: %d] (%d): %s() %s\n",
             smp_processor_id(), current->pid, function, buffer);
   }
}
