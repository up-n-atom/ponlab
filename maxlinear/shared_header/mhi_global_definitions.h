/******************************************************************************

                               Copyright (c) 2012
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/
/****************************************************************************
 ****************************************************************************
 **
 ** COMPONENT:      ENET SW - RSN 802.11i
 **
 ** MODULE:         $File: //bwp/enet/demo153_sw/develop/src/shared/mhi_global_definitions.h $
 **
 ** DESCRIPTION:    global definitions. 
 **
 ** AUTHOR:         
 **
 ** THIS VERSION:   $Revision: #1 $
 **   CHANGED ON:   $Date:  $
 **           BY:   $Author: $
 **
 ****************************************************************************
 **
 ** 
 **
 ** 
 **
 ****************************************************************************
 ****************************************************************************/

#ifndef __MHI_GLOBAL_DEFINITIONS_H
#define __MHI_GLOBAL_DEFINITIONS_H

#define   MTLK_PACK_ON
#include "mtlkpack.h"

/***************************************************************************/
/***                           Public Defines                            ***/
/***************************************************************************/
#define MAX_UINT8   ((uint8) 0xFF)                  /* maximum uint8 value  */
#define MAX_UINT16  ((uint16)0xFFFF)                /* maximum uint16 value */
#define MAX_UINT32  ((uint32)0xFFFFFFFFU)           /* maximum uint32 value */
#define MAX_UINT64  ((uint64)0xFFFFFFFFFFFFFFFFULL) /* maximum uint64 value */

#define MAX_INT8    ((int8) 0x7F)                   /* maximum int8 value  */
#define MAX_INT16   ((int16)0x7FFF)                 /* maximum int16 value */
#define MAX_INT32   ((int32)0x7FFFFFFF)             /* maximum int32 value */
#define MAX_INT64   ((int64)0x7FFFFFFFFFFFFFFFLL)   /* maximum int64 value */

#define MIN_INT8    ((int8) 0x80)                   /* minimum int8 value  */
#define MIN_INT16   ((int16)0x8000)                 /* minimum int16 value */
#define MIN_INT32   ((int32)0x80000000)             /* minimum int32 value */
#define MIN_INT64   ((int64)0X8000000000000000LL)   /* minimum int64 value */


/***************************************************************************/
/***                        Public Typedefs                              ***/
/***************************************************************************/


#define   MTLK_PACK_OFF
#include "mtlkpack.h"

#endif // __MHI_GLOBAL_DEFINITIONS_H
