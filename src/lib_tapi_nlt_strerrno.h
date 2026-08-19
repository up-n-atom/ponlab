#ifndef _LIB_TAPI_NLT_STRERRNO_H
#define _LIB_TAPI_NLT_STRERRNO_H
/****************************************************************************

                           Copyright (c) 2012 - 2016
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/**
   \file lib_tapi_nlt_strerrno.h for GR.909 linetesting library,
   holds string representation for error enums.
*/

/* ============================= */
/* Global Defines & enums        */
/* ============================= */

/** Return this string with IFX_TAPI_NLT_ErrorStrGet()
    if unable to get error string */
#define IFX_NLT_ERR_STR_UNKNOWN  "Unable to get error string"

/* ============================= */
/* Global Structures             */
/* ============================= */

/** Structure to hold error number and string that describes it. */
typedef struct
{
   /* Error number. */
   IFX_NLT_ERR_NUM_t nErrNo;
   /* Pointer to error string. */
   IFX_char_t *pName;
} IFX_NLT_ERR_2_STR_t;

IFX_NLT_ERR_2_STR_t aErr2Str[] =
{
   {IFX_NLT_ERR_InvalidInputParam,
    "Invalid input parameters"},
   {IFX_NLT_ERR_IncorrectDeviceType,
    "Incorrect device type"},
   {IFX_NLT_ERR_PfCalcNull,
    "pf_calc is NULL"},
   {IFX_NLT_ERR_OL_ConfGetFail,
    "Open Loop configuration get failed"},
   {IFX_NLT_ERR_OL_ConfSetFail,
    "Open Loop configuration set failed"},
   {IFX_NLT_ERR_ConfRmesSetFail,
    "Set measurement path according to Rmes resistor failed"},
   {IFX_NLT_ERR_LineFeedDisFail,
    "Failed to set line feed to disabled"},
   {IFX_NLT_ERR_CalibStartFail,
    "Failed to start calibration"},
   {IFX_NLT_ERR_CapacStartFail,
    "Failed to start capacitance measurement"},
   {IFX_NLT_ERR_CapacStopFail,
    "Failed to stop capacitance measurement"},
   {IFX_NLT_ERR_CapacResGetFail,
    "Failed to get capacitance measurement results"},
   {IFX_NLT_ERR_GR909StartFail,
    "Failed to start GR.909 measurement"},
   {IFX_NLT_ERR_GR909StopFail,
    "Failed to stop GR.909 measurement"},
   {IFX_NLT_ERR_GR909ResGetFail,
    "Failed to get GR.909 measurement results"},
   {IFX_NLT_ERR_AclmStartFail,
    "Failed to start AC-level-metering measurement"},
   { -1,
    IFX_NULL}
};

/* ============================= */
/* Global function declaration   */
/* ============================= */

#endif /* _LIB_TAPI_NLT_STRERRNO_H */
