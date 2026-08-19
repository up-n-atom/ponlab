#ifndef _LIB_TAPI_NLT_ERRNO_H
#define _LIB_TAPI_NLT_ERRNO_H
/****************************************************************************

                           Copyright (c) 2012 - 2016
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/**
   \file lib_tapi_nlt_errno.h for GR.909 linetesting library
   This file holds numbers of error codes.
*/

/** \addtogroup TAPI_INTERFACE_LT_GR909 */
/*@{*/

/* ============================= */
/* Global Defines & enums        */
/* ============================= */

/** Enumeration for interface function return status. */
typedef enum
{
   /** Invalid input parameters */
   IFX_NLT_ERR_InvalidInputParam  = 0x7001,
   /** Incorrect device type */
   IFX_NLT_ERR_IncorrectDeviceType,
   /** pf_calc is NULL */
   IFX_NLT_ERR_PfCalcNull,
   /** Open Loop configuration get failed */
   IFX_NLT_ERR_OL_ConfGetFail,
   /** Open Loop configuration set failed */
   IFX_NLT_ERR_OL_ConfSetFail,
   /** Set measurement path according to Rmes resistor failed */
   IFX_NLT_ERR_ConfRmesSetFail,
   /** Failed to set line feed to disabled */
   IFX_NLT_ERR_LineFeedDisFail,
   /** Failed to start calibration */
   IFX_NLT_ERR_CalibStartFail,
   /** Failed to start capacitance measurement */
   IFX_NLT_ERR_CapacStartFail,
   /** Failed to stop capacitance measurement */
   IFX_NLT_ERR_CapacStopFail,
   /** Failed to get capacitance measurement results */
   IFX_NLT_ERR_CapacResGetFail,
   /** Failed to start GR.909 measurement */
   IFX_NLT_ERR_GR909StartFail,
   /** Failed to stop GR.909 measurement */
   IFX_NLT_ERR_GR909StopFail,
   /** Failed to get GR.909 measurement results */
   IFX_NLT_ERR_GR909ResGetFail,
   /** Failed to start AC-level-metering measurement */
   IFX_NLT_ERR_AclmStartFail,
} IFX_NLT_ERR_NUM_t;

/* ============================= */
/* Global Structures             */
/* ============================= */

/* ============================= */
/* Global function declaration   */
/* ============================= */

/*@}*/ /* TAPI_INTERFACE_LT_GR909 */
#endif /* _LIB_TAPI_NLT_ERRNO_H */
