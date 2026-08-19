/******************************************************************************

                           Copyright (c) 2012 - 2016
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/**
   \file lib_tapi_lt_gr909.c  Linetesting library
   The functions in this file serve as an API for the linetesting functionality.
   They can be used by the application and provide the interface to the
   driver to control linetesting and get the results. The functions mainly take
   care about scaling the driver results into floating point precision.
   The functions here can only be used where floating point is supported.

   Contains implementation of following user interface functions for:

   - Calibration of the system at open loop conditions:
      - IFX_TAPI_NLT_OLCalibration
      - IFX_TAPI_NLT_OLConfig_Set

   - GR.909 line testing:
      - Ifxphone_LT_GR909_Config
      - Ifxphone_LT_GR909_Start
      - Ifxphone_LT_GR909_Stop
      - Ifxphone_LT_GR909_GetResults

   - Line capacitance measurements:
      - IFX_TAPI_NLT_Capacitance_Start
      - IFX_TAPI_NLT_Capacitance_Stop
      - IFX_TAPI_NLT_Capacitance_GetResults

   - AC level metering:
      - IFX_TAPI_NLT_AclmStart
      - IFX_TAPI_NLT_AclmResultGet

   - Other:
      - IFX_TAPI_NLT_VersionGet
      - IFX_TAPI_NLT_ErrorStrGet
*/

/* ============================= */
/* Includes                      */
/* ============================= */

#include "lib_tapi_nlt_osmap.h"

#include "drv_tapi_io.h"
#include "drv_tapi_errno.h"

#include "lib_tapi_lt_gr909.h"
#include "lib_tapi_nlt_version.h"

#include "lib_tapi_nlt_errno.h"
#include "lib_tapi_nlt_strerrno.h"
#include "stdio.h"
/* system includes */
#include <math.h>


/* ============================= */
/* Defines                       */
/* ============================= */

/* Linefeeding to set before starting a measurement. */
#define TAPI_LIB_LT_GR909_START_LINE_FEED IFX_TAPI_LINE_FEED_DISABLED

/* ============================= */
/* Local macros & definitions    */
/* ============================= */

/* ============================= */
/* Local function declaration    */
/* ============================= */

#if (defined (LIB_TAPI_NLT_VMMC_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
extern IFX_void_t VMMC_LT_GR909_CalcResults(IFX_LT_GR909_RESULT_t *p_res,
                                            IFX_TAPI_GR909_RESULT_t *pTapiRes);
extern IFX_LT_GR909_CFG_t vmmc_cfg;
#endif
#if (defined (LIB_TAPI_NLT_VINCPE_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
extern IFX_void_t VINCPE_LT_GR909_CalcResults(IFX_LT_GR909_RESULT_t *p_res,
                                            IFX_TAPI_GR909_RESULT_t *pTapiRes);
extern IFX_LT_GR909_CFG_t vincpe_cfg;
#endif
#if (defined (LIB_TAPI_NLT_DXT_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
extern IFX_void_t DXT_LT_GR909_CalcResults(IFX_LT_GR909_RESULT_t *p_res,
                                            IFX_TAPI_GR909_RESULT_t *pTapiRes);
extern IFX_LT_GR909_CFG_t dxt_cfg;
#endif
#if (defined (LIB_TAPI_NLT_DXS_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
extern IFX_void_t DXS_LT_GR909_CalcResults(IFX_LT_GR909_RESULT_t *p_res,
                                            IFX_TAPI_GR909_RESULT_t *pTapiRes);
extern IFX_LT_GR909_CFG_t dxs_cfg;
#endif
#if (defined (LIB_TAPI_NLT_S220_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
extern IFX_void_t S220_LT_GR909_CalcResults(IFX_LT_GR909_RESULT_t *p_res,
                                               IFX_TAPI_GR909_RESULT_t *pTapiRes);
extern IFX_LT_GR909_CFG_t s220_cfg;
#endif

/* ============================= */
/* Local function definition     */
/* ============================= */

/**
   Get pointer to the IFX_LT_GR909_CFG_t config structure.

   \param  dev          Enum of \ref IFX_TAPI_GR909_DEV_t type.
   \param  ppCfg        Pointer to IFX_LT_GR909_CFG_t structure.

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_return_t ifx_nlt_GR909_GetConfigPtr (IFX_TAPI_GR909_DEV_t dev,
                                         IFX_LT_GR909_CFG_t **ppCfg)
{
   IFX_return_t ret = IFX_SUCCESS;

   /* use user config */
   switch (dev)
   {
#if (defined (LIB_TAPI_NLT_VMMC_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
      case IFX_TAPI_GR909_DEV_VMMC:
         *ppCfg = &vmmc_cfg;
         break;
#endif
#if (defined (LIB_TAPI_NLT_VINCPE_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
      case IFX_TAPI_GR909_DEV_VINCPE:
         *ppCfg = &vincpe_cfg;
         break;
#endif
#if (defined (LIB_TAPI_NLT_DXT_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
      case IFX_TAPI_GR909_DEV_DXT:
         *ppCfg = &dxt_cfg;
         break;
#endif
#if (defined (LIB_TAPI_NLT_DXS_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
      case IFX_TAPI_GR909_DEV_DXS:
         *ppCfg = &dxs_cfg;
         break;
#endif
#if (defined (LIB_TAPI_NLT_S220_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
      case IFX_TAPI_GR909_DEV_S220:
         *ppCfg = &s220_cfg;
         break;
#endif
      default:
         *ppCfg = IFX_NULL;
         ret = IFX_NLT_ERR_IncorrectDeviceType;
   }

   return ret;
}

/**
   Set measurement path according to Rmes (R1) resistor.

   \param  fd           Line file descriptor.
   \param  dev_ch       Used device and channel numbers,
                        available only if TAPI V4 API is used.

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_return_t ifx_nlt_SetRmesConf (IFX_int32_t fd
#ifdef TAPI_ONE_DEVNODE
                                  , IFX_LT_DevCh_t dev_ch
#endif /* TAPI_ONE_DEVNODE */
                                  )
{
   IFX_return_t ret;
   IFX_TAPI_NLT_CONFIGURATION_OL_t OlConf = {0};
   IFX_TAPI_NLT_CONFIGURATION_RMES_t RmesConf = {0};
   IFX_LT_GR909_CFG_t       *pCfg = IFX_NULL;

#ifdef TAPI_ONE_DEVNODE
   OlConf.dev = dev_ch.dev;
   OlConf.ch  = dev_ch.ch;
#endif /* TAPI_ONE_DEVNODE */
   /* read current open loop configuration to get device type. */
   ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_NLT_CONFIGURATION_OL_GET,
                             (IFX_int32_t) &OlConf);
   if (ret != IFX_SUCCESS)
   {
      /* In case of an error check the reason. If the error occured because
         the driver does not support the IOCTL, then return success to maintain
         compatiblity with drivers that do not have this interface and run
         with default values in this case. */
      IFX_TAPI_Error_t error;

      memset (&error, 0, sizeof (error));

#ifdef TAPI_ONE_DEVNODE
      error.nDev = dev_ch.dev;
#endif /* TAPI_ONE_DEVNODE */

      ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_LASTERR, (IFX_uintptr_t)&error);
      if (ret == IFX_SUCCESS &&
          (((error.nCode) >> 16) & 0x0000FFFF) == TAPI_statusLLNotSupp)
      {
         return IFX_SUCCESS;
      }

      return IFX_NLT_ERR_OL_ConfGetFail;
   }

   ret = ifx_nlt_GR909_GetConfigPtr (OlConf.dev_type, &pCfg);
   if (ret != IFX_SUCCESS)
      return ret;

   if (pCfg->f_R1 > 1200000)
      RmesConf.nRmeas = IFX_TAPI_NLT_RMEAS_1_5MOHM;
   else
      RmesConf.nRmeas = IFX_TAPI_NLT_RMEAS_1MOHM;

#ifdef TAPI_ONE_DEVNODE
   RmesConf.dev = dev_ch.dev;
   RmesConf.ch  = dev_ch.ch;
#endif /* TAPI_ONE_DEVNODE */
   /* Set measurement path according to Rmes resistor. */
   ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_NLT_CONFIGURATION_RMES_SET,
                             (IFX_int32_t) &RmesConf);
   if (ret != IFX_SUCCESS)
   {
      /* In case of an error check the reason. If the error occured because
         the driver does not support the IOCTL, then return success to maintain
         compatiblity with drivers that do not have this interface and run
         with default values in this case. */
      IFX_TAPI_Error_t error;

      memset (&error, 0, sizeof (error));

#ifdef TAPI_ONE_DEVNODE
      error.nDev = dev_ch.dev;
#endif /* TAPI_ONE_DEVNODE */
      ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_LASTERR,
                                (IFX_uintptr_t)&error);
      if (ret == IFX_SUCCESS &&
          (((error.nCode) >> 16) & 0x0000FFFF) == TAPI_statusLLNotSupp)
      {
         ret = IFX_SUCCESS;
      }
      else
      {
         ret = IFX_NLT_ERR_ConfRmesSetFail;
      }
   }

   return ret;
}

/* ============================= */
/* Global function definition    */
/* ============================= */

/**
   Get Line Testing library version.

   \param  pVersion      Pointer to a struct of \ref IFX_NLT_VERSION_t

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_VersionGet(IFX_NLT_VERSION_t *pVersion)
{
   if (pVersion == IFX_NULL)
   {
      return IFX_NLT_ERR_InvalidInputParam;
   }

   pVersion->major = LIB_TAPI_NLT_VER_MAJOR;
   pVersion->minor = LIB_TAPI_NLT_VER_MINOR;
   pVersion->step  = LIB_TAPI_NLT_VER_STEP;
   pVersion->type  = LIB_TAPI_NLT_VER_TYPE;

   return IFX_SUCCESS;
}

/**
   Translates error number to string.

   \param  nErrNo      error code returned by NLT library function

   \return Returns string with description of error.
*/
IFX_char_t *IFX_TAPI_NLT_ErrorStrGet(IFX_int32_t nErrNo)
{
   IFX_NLT_ERR_2_STR_t *pErr2Str = aErr2Str;

   /* look through string table */
   while (pErr2Str->nErrNo != -1 && pErr2Str->pName != IFX_NULL)
   {
      if (pErr2Str->nErrNo == nErrNo)
      {
         /* return string */
         return pErr2Str->pName;
      }
      pErr2Str++;
   }

   return IFX_NLT_ERR_STR_UNKNOWN;
}

/**
   This function does an open loop calibration needed for capacitance and
   GR.909 measurements.

   This function automatically sets the calculated calibration factors into
   the driver, so user doesn't need to do it.

   Open Loop calibration should be executed only once, when
   phone is NOT connected to the FXS port. User should store calibration factors
   in non-volatile memory and execute \ref IFX_TAPI_NLT_OLConfig_Set
   during next system start. This will save time during startup and still give
   most accurate measurement results.

   \param  fd           Line file descriptor.
   \param  nNrLoops     number of loops for calibration. 1 is sufficient.
   \param  b_euLike     IFX_TRUE  : EU like powerline frequency (50 Hz)
                        IFX_FALSE : US like power line frequency (60 Hz)
   \param  pConfig      Pointer to a struct of
                        \ref IFX_TAPI_NLT_CONFIGURATION_OL_t. Calculated
                        calibration factors are stored to this structure.

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error

   \remarks
   This function automatically sets the calculated calibration factors into
   the driver, so user doesn't need to do it.
   Open Loop calibration should be executed only once, when the phone is NOT
   connected to the FXS port. The resulting calibration factors should then
   be stored in non-volatile memory and on next systems start the restored
   using \ref IFX_TAPI_NLT_OLConfig_Set. This will save time during startup
   and still give most accurate measurement results.
*/
IFX_int32_t IFX_TAPI_NLT_OLCalibration (
                                      IFX_int32_t fd,
                                      IFX_uint32_t nNrLoops,
                                      IFX_boolean_t b_euLike,
                                      IFX_TAPI_NLT_CONFIGURATION_OL_t *pConfig)
{
   IFX_return_t ret;
   IFX_int32_t  i;
   IFX_float_t fCalCapT2R = 0.0;
   IFX_float_t fCalCapT2G = 0.0;
   IFX_float_t fCalCapR2G = 0.0;
   IFX_float_t fCalResT2R = 1000000000000.0;
   IFX_float_t fCalResT2G = 1000000000000.0;
   IFX_float_t fCalResR2G = 1000000000000.0;
   IFX_TAPI_GR909_START_t   GR909_measStart;
   IFX_TAPI_GR909_RESULT_t  GR909_measRes;
   IFX_TAPI_NLT_CAPACITANCE_START_t CapStartArg = {0};
   IFX_TAPI_NLT_CAPACITANCE_RESULT_t CapRes = {0};
   /* used to cast int16 into uint32 without implicit sign extension */
   IFX_uint32_t value;
   IFX_float_t fResTemp;
   IFX_int32_t param_line_feed;
   IFX_int32_t param_calib_start;
#ifdef TAPI_ONE_DEVNODE
   IFX_LT_DevCh_t dev_ch;
   IFX_TAPI_LINE_FEED_t line_feed = {0};
   IFX_TAPI_CALIBRATION_t calib_start = {0};
#endif /* TAPI_ONE_DEVNODE */

   if ((pConfig == IFX_NULL) || (nNrLoops == 0))
      return IFX_NLT_ERR_InvalidInputParam;

#ifdef TAPI_ONE_DEVNODE
   line_feed.dev = pConfig->dev;
   line_feed.ch  = pConfig->ch;
   line_feed.lineMode = TAPI_LIB_LT_GR909_START_LINE_FEED;
   param_line_feed = (IFX_int32_t) &line_feed;
#else /* TAPI_ONE_DEVNODE */
   param_line_feed = TAPI_LIB_LT_GR909_START_LINE_FEED;
#endif /* TAPI_ONE_DEVNODE */

   ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_LINE_FEED_SET,
                             param_line_feed);
   if (ret != IFX_SUCCESS)
       return IFX_NLT_ERR_LineFeedDisFail;

#ifdef TAPI_ONE_DEVNODE
   calib_start.dev = pConfig->dev;
   calib_start.ch  = pConfig->ch;
   param_calib_start = (IFX_int32_t) &calib_start;
#else /* TAPI_ONE_DEVNODE */
   param_calib_start = 0;
#endif /* TAPI_ONE_DEVNODE */

   ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_CALIBRATION_START, param_calib_start);
   if (ret != IFX_SUCCESS)
       return IFX_NLT_ERR_CalibStartFail;
   LIB_NLT_SecSleep(1);

#ifdef TAPI_ONE_DEVNODE
   CapStartArg.dev = pConfig->dev;
   CapStartArg.ch  = pConfig->ch;
   CapRes.dev = pConfig->dev;
   CapRes.ch  = pConfig->ch;
#endif /* TAPI_ONE_DEVNODE */

   /* Calibration for capacitance factors. */
   for (i=0; i<nNrLoops; i++)
   {
      ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_NLT_CAPACITANCE_START,
                                (IFX_int32_t)&CapStartArg);
      if (ret != IFX_SUCCESS)
      {
         return IFX_NLT_ERR_CapacStartFail;
      }
      LIB_NLT_SecSleep(3);

      ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_NLT_CAPACITANCE_RESULT_GET,
                                (IFX_int32_t)&CapRes);
      if (ret != IFX_SUCCESS)
         return IFX_NLT_ERR_CapacResGetFail;

      /* calculate the caoacitance factors. */
      if (CapRes.bValidLine2Gnd == IFX_TRUE)
      {
         fCalCapT2G = fCalCapT2G + (IFX_float_t)CapRes.nCapTip2Gnd / nNrLoops;
         fCalCapR2G = fCalCapR2G + (IFX_float_t)CapRes.nCapRing2Gnd / nNrLoops;
      }
      if (CapRes.bValidTip2Ring == IFX_TRUE)
      {
         fCalCapT2R = fCalCapT2R + (IFX_float_t)CapRes.nCapTip2Ring / nNrLoops;
      }
   }

#ifdef TAPI_ONE_DEVNODE
   dev_ch.dev = pConfig->dev;
   dev_ch.ch  = pConfig->ch;
#endif /* TAPI_ONE_DEVNODE */

   ret = ifx_nlt_SetRmesConf(fd
#ifdef TAPI_ONE_DEVNODE
                             , dev_ch
#endif /* TAPI_ONE_DEVNODE */
                             );
   if (ret != IFX_SUCCESS)
      return ret;

#ifdef TAPI_ONE_DEVNODE
   GR909_measStart.dev = pConfig->dev;
   GR909_measStart.ch  = pConfig->ch;
   GR909_measRes.dev = pConfig->dev;
   GR909_measRes.ch  = pConfig->ch;
#endif /* TAPI_ONE_DEVNODE */

   GR909_measStart.test_mask = IFX_TAPI_GR909_RFT;
   GR909_measStart.pl_freq = ((b_euLike == IFX_TRUE) ? IFX_TAPI_GR909_EU_50HZ :
                                                       IFX_TAPI_GR909_US_60HZ);

   /* Calibration for GR909 factors. */
   for (i=0; i<nNrLoops; i++)
   {
      /* start GR909 measurment. */
      ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_GR909_START,
                                (IFX_int32_t) &GR909_measStart);
      if (ret != IFX_SUCCESS)
         return IFX_NLT_ERR_GR909StartFail;
      LIB_NLT_SecSleep(2);

      /* read results */
      ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_GR909_RESULT,
                                (IFX_int32_t)&GR909_measRes);
      if (ret != IFX_SUCCESS)
         return IFX_NLT_ERR_GR909ResGetFail;
      if (GR909_measRes.valid & IFX_TAPI_GR909_RFT_EXT_VALID)
      {
         /* RFT tip to ring */
         value = (IFX_uint32_t)((IFX_uint16_t)GR909_measRes.RFT_T2R);
         fResTemp = (value >> 4) << (value & 0x000F);
         fCalResT2R = 1 / (1/fCalResT2R - 1/fResTemp/nNrLoops);

         /* RFT ring to ground */
         value = (IFX_uint32_t)((IFX_uint16_t)GR909_measRes.RFT_R2G);
         fResTemp = (value >> 4) << (value & 0x000F);
         fCalResR2G = 1 / (1/fCalResR2G - 1/fResTemp/nNrLoops);

         /* RFT tip to ground */
         value = (IFX_uint32_t)((IFX_uint16_t)GR909_measRes.RFT_T2G);
         fResTemp = (value >> 4) << (value & 0x000F);
         fCalResT2G = 1 / (1/fCalResT2G - 1/fResTemp/nNrLoops);
      }
      pConfig->dev_type = GR909_measRes.dev_type;
   }

   /* return calibration factors. */
   pConfig->fOlCapTip2Ring = fCalCapT2R;
   pConfig->fOlCapTip2Gnd = fCalCapT2G;
   pConfig->fOlCapRing2Gnd = fCalCapR2G;
   pConfig->fOlResTip2Ring = fCalResT2R;
   pConfig->fOlResTip2Gnd = fCalResT2G;
   pConfig->fOlResRing2Gnd = fCalResR2G;

   /* set calibration factors in driver, so user doesn't need to do it. */
   ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_NLT_CONFIGURATION_OL_SET,
                             (IFX_int32_t)pConfig);
   if (ret != IFX_SUCCESS)
   {
      return IFX_NLT_ERR_OL_ConfSetFail;
   }
   return ret;
}

/**
   Starts measurement of capacitance values.

   \param  fd           Line file descriptor.
   \param  dev_ch       Used device and channel numbers,
                        available only if TAPI V4 API is used.

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_Capacitance_Start (IFX_int32_t fd
#ifdef TAPI_ONE_DEVNODE
                                            , IFX_LT_DevCh_t dev_ch
#endif /* TAPI_ONE_DEVNODE */
                                            )
{
   IFX_return_t             ret;
   IFX_TAPI_NLT_CAPACITANCE_START_t cap = {0};

#ifdef TAPI_ONE_DEVNODE
   cap.dev = dev_ch.dev;
   cap.ch  = dev_ch.ch;
#endif /* TAPI_ONE_DEVNODE */

   ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_NLT_CAPACITANCE_START,
                             (IFX_int32_t)&cap);

   if (ret != IFX_SUCCESS)
   {
      return IFX_NLT_ERR_CapacStartFail;
   }

   return ret;
}

/**
   Stops measurement of capacitance values immediately.

   \param  fd           Line file descriptor.
   \param  dev_ch       Used device and channel numbers,
                        available only if TAPI V4 API is used.

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_Capacitance_Stop (IFX_int32_t fd
#ifdef TAPI_ONE_DEVNODE
                                           , IFX_LT_DevCh_t dev_ch
#endif /* TAPI_ONE_DEVNODE */
                                           )
{
   IFX_return_t ret;
   IFX_TAPI_NLT_CAPACITANCE_STOP_t cap = {0};

#ifdef TAPI_ONE_DEVNODE
   cap.dev = dev_ch.dev;
   cap.ch  = dev_ch.ch;
#endif /* TAPI_ONE_DEVNODE */

   ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_NLT_CAPACITANCE_STOP,
                             (IFX_int32_t)&cap);

   if (ret != IFX_SUCCESS)
   {
      return IFX_NLT_ERR_CapacStopFail;
   }

   return ret;
}

/**
   Get measurement results of the capacitance measurement.

   \param  fd           Line file descriptor.
   \param  dev_ch       Used device and channel numbers,
                        available only if TAPI V4 API is used.
   \param  pResult      Pointer to a struct of \ref IFX_NLT_Capacitance_Result_t
                        to be filled with the results.

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_Capacitance_GetResults (IFX_int32_t fd,
#ifdef TAPI_ONE_DEVNODE
                                       IFX_LT_DevCh_t dev_ch,
#endif /* TAPI_ONE_DEVNODE */
                                       IFX_NLT_Capacitance_Result_t *pResult)
{
   IFX_return_t                       ret;
   IFX_TAPI_NLT_CAPACITANCE_RESULT_t  meas_res;

   /* reset structures */
   memset (&meas_res, 0, sizeof (meas_res));
   memset (pResult, 0, sizeof (*pResult));

#ifdef TAPI_ONE_DEVNODE
   meas_res.dev = dev_ch.dev;
   meas_res.ch  = dev_ch.ch;
#endif /* TAPI_ONE_DEVNODE */

   /* read results */
   ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_NLT_CAPACITANCE_RESULT_GET,
                             (IFX_int32_t)&meas_res);
   if (ret != IFX_SUCCESS)
      return IFX_NLT_ERR_CapacResGetFail;

   /* line 2 ground capacitance valid? */
   if (meas_res.bValidLine2Gnd == IFX_TRUE)
   {
      pResult->b_line2gnd_valid = IFX_TRUE;
      pResult->f_tip2gnd_cap = meas_res.nCapTip2Gnd - meas_res.fOlCapTip2Gnd;
      if (pResult->f_tip2gnd_cap < 0)
         pResult->f_tip2gnd_cap = 0;
      pResult->f_ring2gnd_cap = meas_res.nCapRing2Gnd -
                                meas_res.fOlCapRing2Gnd;
      if (pResult->f_ring2gnd_cap < 0)
         pResult->f_ring2gnd_cap = 0;
   }

   /* tip to ring capacitance valid? */
   if (meas_res.bValidTip2Ring == IFX_TRUE)
   {
      pResult->b_tip2ring_valid = IFX_TRUE;
      pResult->f_tip2ring_cap = meas_res.nCapTip2Ring -
                                meas_res.fOlCapTip2Ring -
                                (pResult->f_tip2gnd_cap / 4) -
                                (pResult->f_ring2gnd_cap / 4);
      if (pResult->f_tip2ring_cap < 0)
         pResult->f_tip2ring_cap = 0;
   }

   return ret;
}

/**
   Configures the open loop calibration factors of the measurement path
   for line testing.

   \param  fd           Line file descriptor.
   \param  pConfig      Pointer to a struct of
                        \ref IFX_TAPI_NLT_CONFIGURATION_OL_t

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_OLConfig_Set (IFX_int32_t fd,
                                       IFX_TAPI_NLT_CONFIGURATION_OL_t *pConfig)
{
   IFX_return_t ret;

   ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_NLT_CONFIGURATION_OL_SET,
                             (IFX_int32_t)pConfig);
   if (ret != IFX_SUCCESS)
   {
      return IFX_NLT_ERR_OL_ConfSetFail;
   }

   return ret;
}

/**
   Return configuration of the open loop calibration factors of the measurement
   path for line testing.

   \param  fd           Line file descriptor.
   \param  pConfig      Pointer to a struct of
                        \ref IFX_TAPI_NLT_CONFIGURATION_OL_t

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_OLConfig_Get (IFX_int32_t fd,
                                       IFX_TAPI_NLT_CONFIGURATION_OL_t *pConfig)
{
   IFX_return_t ret;

   ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_NLT_CONFIGURATION_OL_GET,
                             (IFX_int32_t)pConfig);
   if (ret != IFX_SUCCESS)
   {
      return IFX_NLT_ERR_OL_ConfSetFail;
   }

   return ret;
}

/**
   Configure system parameters (SLIC) to use for values calculation,
    e.g Voltage divider resistors.

   \param  dev          Enum of \ref IFX_TAPI_GR909_DEV_t type.
   \param  p_cfg        Pointer to IFX_LT_GR909_CFG_t structure.

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error

   \remark
   Usage of this function is optional. Default parameters are used otherwise.
*/
IFX_int32_t Ifxphone_LT_GR909_Config (IFX_TAPI_GR909_DEV_t dev,
                                      IFX_LT_GR909_CFG_t *p_cfg)
{
   IFX_return_t ret = IFX_SUCCESS;
   IFX_LT_GR909_CFG_t *pUserCfg = IFX_NULL;

   /* check config integrity to ensure software safety */
   if ((p_cfg->f_R1 == 0.0) || (p_cfg->f_R2 == 0.0) || (p_cfg->f_R3 == 0.0))
      return IFX_NLT_ERR_InvalidInputParam;

   /* use user config */
   ret = ifx_nlt_GR909_GetConfigPtr (dev, &pUserCfg);
   if (pUserCfg != IFX_NULL)
   {
      *pUserCfg = *p_cfg;
   }

   return ret;
}

/**
   Start a GR909 test or test sequence according to measurement mask
   set with \ref IFX_LT_GR909_MASK_t.

   \param  fd_line      Line file descriptor.
   \param  dev_ch       Used device and channel numbers,
                        available only if TAPI V4 API is used.
   \param  b_euLike     IFX_TRUE  : EU like powerline frequency (50 Hz)
                        IFX_FALSE : US like power line frequency (60 Hz).
   \param  meas_mask    Measurement mask set with values out
                        of \ref IFX_LT_GR909_MASK_t.

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t Ifxphone_LT_GR909_Start (IFX_int32_t   fd_line,
#ifdef TAPI_ONE_DEVNODE
                                     IFX_LT_DevCh_t dev_ch,
#endif /* TAPI_ONE_DEVNODE */
                                     IFX_boolean_t b_euLike,
                                     IFX_uint32_t  meas_mask)
{
   IFX_return_t             ret;
   IFX_TAPI_GR909_START_t   meas_start;
   IFX_uint32_t             *mask = &meas_start.test_mask;
#ifdef TAPI_ONE_DEVNODE
   IFX_TAPI_LINE_FEED_t line_feed = {0};
#endif /* TAPI_ONE_DEVNODE */
   IFX_int32_t param_line_feed;

#ifdef TAPI_ONE_DEVNODE
   line_feed.dev = dev_ch.dev;
   line_feed.ch  = dev_ch.ch;
   line_feed.lineMode = TAPI_LIB_LT_GR909_START_LINE_FEED;
   param_line_feed = (IFX_int32_t) &line_feed;
#else /* TAPI_ONE_DEVNODE */
   param_line_feed = TAPI_LIB_LT_GR909_START_LINE_FEED;
#endif /* TAPI_ONE_DEVNODE */

   ret = LIB_NLT_DeviceControl(fd_line, IFX_TAPI_LINE_FEED_SET,
                             param_line_feed);
   if (ret != IFX_SUCCESS)
       return IFX_NLT_ERR_LineFeedDisFail;

   ret = ifx_nlt_SetRmesConf(fd_line
#ifdef TAPI_ONE_DEVNODE
                             , dev_ch
#endif /* TAPI_ONE_DEVNODE */
                             );
   if (ret != IFX_SUCCESS)
      return ret;

   memset (&meas_start, 0, sizeof (meas_start));

#ifdef TAPI_ONE_DEVNODE
   meas_start.dev = dev_ch.dev;
   meas_start.ch  = dev_ch.ch;
#endif /* TAPI_ONE_DEVNODE */
   /* setup powerline frequency  */
   meas_start.pl_freq = ((b_euLike == IFX_TRUE) ? IFX_TAPI_GR909_EU_50HZ :
                                                       IFX_TAPI_GR909_US_60HZ);
   /* setup driver test mask according to user mask */
   *mask |= ((meas_mask & IFX_LT_GR909_HPT_MASK)  ? IFX_TAPI_GR909_HPT  : 0);
   *mask |= ((meas_mask & IFX_LT_GR909_FEMF_MASK) ? IFX_TAPI_GR909_FEMF : 0);
   *mask |= ((meas_mask & IFX_LT_GR909_RFT_MASK)  ? IFX_TAPI_GR909_RFT  : 0);
   *mask |= ((meas_mask & IFX_LT_GR909_ROH_MASK)  ? IFX_TAPI_GR909_ROH  : 0);
   *mask |= ((meas_mask & IFX_LT_GR909_RIT_MASK)  ? IFX_TAPI_GR909_RIT  : 0);

   /* no test ? */
   if (*mask == 0)
      return IFX_NLT_ERR_InvalidInputParam;

   ret = LIB_NLT_DeviceControl(fd_line, IFX_TAPI_GR909_START,
                             (IFX_int32_t) &meas_start);

   if (ret != IFX_SUCCESS)
   {
      return IFX_NLT_ERR_GR909StartFail;
   }

   return ret;
}

/**
   Stop a GR909 test or test sequence

   \param  fd_line      Line file descriptor.
   \param  dev_ch       Used device and channel numbers,
                        available only if TAPI V4 API is used.

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t Ifxphone_LT_GR909_Stop (IFX_int32_t fd_line
#ifdef TAPI_ONE_DEVNODE
                                    , IFX_LT_DevCh_t dev_ch
#endif /* TAPI_ONE_DEVNODE */
                                    )
{
   IFX_return_t ret;
   IFX_int32_t param = 0;
#ifdef TAPI_ONE_DEVNODE
   IFX_TAPI_GR909_STOP_t stop;

   stop.dev = dev_ch.dev;
   stop.ch  = dev_ch.ch;
   param  = (IFX_int32_t) &stop;
#endif /* TAPI_ONE_DEVNODE */

   ret = LIB_NLT_DeviceControl(fd_line, IFX_TAPI_GR909_STOP, param);

   if (ret != IFX_SUCCESS)
   {
      return IFX_NLT_ERR_GR909StopFail;
   }

   return ret;
}

/**
   Gets Gr909 measurement results.

   \param  fd_line      Line file descriptor.
   \param  dev_ch       Used device and channel numbers,
                        available only if TAPI V4 API is used.
   \param  p_res        Pointer to user result structure.

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error

   \remark
   Only evaluate results which are marked as valid.
*/
IFX_int32_t Ifxphone_LT_GR909_GetResults (IFX_int32_t            fd_line,
#ifdef TAPI_ONE_DEVNODE
                                          IFX_LT_DevCh_t dev_ch,
#endif /* TAPI_ONE_DEVNODE */
                                          IFX_LT_GR909_RESULT_t *p_res)
{
   IFX_int32_t              ret;
   IFX_TAPI_GR909_RESULT_t  meas_res;
   IFX_void_t (*pf_calc)(IFX_LT_GR909_RESULT_t *pRes,
                                 IFX_TAPI_GR909_RESULT_t *pTapiRes) = IFX_NULL;

   /* reset structures */
   memset (&meas_res, 0, sizeof (meas_res));
   memset (p_res, 0, sizeof (*p_res));

#ifdef TAPI_ONE_DEVNODE
   meas_res.dev = dev_ch.dev;
   meas_res.ch  = dev_ch.ch;
#endif /* TAPI_ONE_DEVNODE */

   /* read results */
   ret = LIB_NLT_DeviceControl(fd_line, IFX_TAPI_GR909_RESULT,
                             (IFX_int32_t)&meas_res);
   if (ret != IFX_SUCCESS)
      return IFX_NLT_ERR_GR909ResGetFail;

   switch (meas_res.dev_type)
   {
#if (defined (LIB_TAPI_NLT_VMMC_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
      case IFX_TAPI_GR909_DEV_VMMC:
         pf_calc = VMMC_LT_GR909_CalcResults;
         break;
#endif /* VMMC */

#if (defined (LIB_TAPI_NLT_VINCPE_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
      case IFX_TAPI_GR909_DEV_VINCPE:
         pf_calc = VINCPE_LT_GR909_CalcResults;
         break;
#endif /* VIN_2CPE */

#if (defined (LIB_TAPI_NLT_DXT_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
      case IFX_TAPI_GR909_DEV_DXT:
         pf_calc = DXT_LT_GR909_CalcResults;
         break;
#endif /* DXT */

#if (defined (LIB_TAPI_NLT_DXS_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
      case IFX_TAPI_GR909_DEV_DXS:
         pf_calc = DXS_LT_GR909_CalcResults;
         break;
#endif /* DXS */

#if (defined (LIB_TAPI_NLT_S220_SUPPORT) && defined (LIB_TAPI_NLT_GR909))
      case IFX_TAPI_GR909_DEV_S220:
         pf_calc = S220_LT_GR909_CalcResults;
         break;
#endif /* VMMC */

      default:
         return IFX_NLT_ERR_IncorrectDeviceType;
   }

   if (pf_calc != IFX_NULL)
   {
      pf_calc(p_res, &meas_res);
   }
   else
   {
      ret = IFX_NLT_ERR_PfCalcNull;
   }

   return ret;
}


/**
   Starts AC level meter measurement.

   \param  fd           Line file descriptor.
   \param  dev_ch       Used device and channel numbers,
                        available only if TAPI V4 API is used.
   \param  nType        Type of measurement.

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_AclmStart (
                        IFX_int32_t fd,
#ifdef TAPI_ONE_DEVNODE
                        IFX_LT_DevCh_t dev_ch,
#endif /* TAPI_ONE_DEVNODE */
                        IFX_LT_ACLM_Measurement_t nType)
{
   IFX_return_t                 ret;
   IFX_TAPI_NLT_TEST_START_t    test_start = {0};

#ifdef TAPI_ONE_DEVNODE
   test_start.dev = dev_ch.dev;
   test_start.ch  = dev_ch.ch;
#endif /* TAPI_ONE_DEVNODE */
   switch (nType)
   {
   /** Frequency Response measurement */
   case DXS_ACLM_FR:
      test_start.testID = IFX_TAPI_NLT_AC_FREQRESPONSE_ID;
      break;
   /** Transhybrid measurement */
   case DXS_ACLM_TH:
      test_start.testID = IFX_TAPI_NLT_AC_TRANSHYBRID_ID;
      break;
   /** Gain Tracking measurement */
   case DXS_ACLM_GT:
      test_start.testID = IFX_TAPI_NLT_AC_GAINTRACKING_ID;
      break;
   /** Signal to Noise Ratio measurement */
   case DXS_ACLM_SNR:
      test_start.testID = IFX_TAPI_NLT_AC_IDLENOISE_ID;
      break;
   default:
      return IFX_NLT_ERR_InvalidInputParam;
   }

   ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_NLT_TEST_START,
                             (IFX_int32_t)&test_start);
   if (ret != IFX_SUCCESS)
   {
      return IFX_NLT_ERR_AclmStartFail;
   }

   return ret;
}


/**
   Retrieves and calculate AC level meter measurement results.

   \param  fd           Line file descriptor.
   \param  dev_ch       Used device and channel numbers,
                        available only if TAPI V4 API is used.
   \param  nType        Type of measurement.
   \param  pResults     Pointer to results stucture.

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_AclmResultGet (
                        IFX_int32_t fd,
#ifdef TAPI_ONE_DEVNODE
                        IFX_LT_DevCh_t dev_ch,
#endif /* TAPI_ONE_DEVNODE */
                        IFX_LT_ACLM_Measurement_t nType,
                        IFX_LT_ACLM_Result_t *pResults)
{
   IFX_return_t ret;
   IFX_TAPI_NLT_RESULT_GET_t resultsGet = {0};
   IFX_TAPI_NLT_ACLM_Result_t testResults;
   IFX_TAPI_NLT_ACLM_MP_Result_t *pMeasMP;
   IFX_int32_t i;
   IFX_float_t fCalcResult = 0, ref_level = 0, fCalcRef = 0;
   IFX_int32_t shift;
   IFX_float_t fInput, qPeakLevel;

#ifdef TAPI_ONE_DEVNODE
   resultsGet.dev = dev_ch.dev;
   resultsGet.ch  = dev_ch.ch;
#endif /* TAPI_ONE_DEVNODE */

   switch (nType)
   {
   /** Frequency Response measurement */
   case DXS_ACLM_FR:
      resultsGet.testID = IFX_TAPI_NLT_AC_FREQRESPONSE_ID;
      break;
   /** Transhybrid measurement */
   case DXS_ACLM_TH:
      resultsGet.testID = IFX_TAPI_NLT_AC_TRANSHYBRID_ID;
      break;
   /** Gain Tracking measurement */
   case DXS_ACLM_GT:
      resultsGet.testID = IFX_TAPI_NLT_AC_GAINTRACKING_ID;
      break;
   /** Signal to Noise Ratio measurement */
   case DXS_ACLM_SNR:
      resultsGet.testID = IFX_TAPI_NLT_AC_IDLENOISE_ID;
      break;
   default:
      return IFX_NLT_ERR_InvalidInputParam;
   }

   resultsGet.pTestResults = &testResults;
   ret = LIB_NLT_DeviceControl(fd, IFX_TAPI_NLT_RESULT_GET,
                             (IFX_int32_t)&resultsGet);
   if (ret != IFX_SUCCESS)
   {
      return ret;
   }

   for (i = 0; i < testResults.mp_count; i++)
   {
      pMeasMP = &testResults.mp_val[i];

      switch (nType)
      {
      /** Transhybrid measurement */
      case DXS_ACLM_TH:
      /** Frequency Response measurement */
      case DXS_ACLM_FR:

         shift = 21 - pMeasMP->AcInbSh;
         if (shift >= 0)
         {
            /* input value */
            fInput = (IFX_float_t)pMeasMP->AcInb/(1 << shift)/(pMeasMP->Int << 3);
            qPeakLevel = sqrt (fInput);
            /* calculate resulting gain from factor */
            fCalcResult = 20 * log10( qPeakLevel ) + 3.14;

            pResults->mp_val[i] = fCalcResult;
            pResults->mp_arg[i] = pMeasMP->freq;

         }
         else
         {
            pResults->mp_val[i] = 0;
            pResults->mp_arg[i] = 0;
            continue;
         }

         if (nType == DXS_ACLM_FR && pMeasMP->freq == 1000)
         {
            ref_level = fCalcResult;
         }
         pResults->mp_count = testResults.mp_count;
         break;

      /** Gain Tracking measurement */
      case DXS_ACLM_GT:

         shift = 21 - pMeasMP->AcOutbSh;
         if (shift >= 0)
         {
            /* input value */
            fInput = (IFX_float_t)pMeasMP->AcOutb/(1 << shift)/(pMeasMP->Int << 3);
            qPeakLevel = sqrt (fInput);
            /* calculate resulting gain from factor */
            fCalcRef = 20 * log10(qPeakLevel) + 3.14;
         }
         else
         {
            pResults->mp_val[i] = 0;
            pResults->mp_arg[i] = 0;
            continue;
         }

         shift = 21 - pMeasMP->AcInbSh;
         if (shift >= 0)
         {
            /* input value */
            fInput = (IFX_float_t)pMeasMP->AcInb/(1 << shift)/(pMeasMP->Int << 3);
            qPeakLevel = sqrt (fInput);
            /* calculate resulting gain from factor */
            fCalcResult = 20 * log10(qPeakLevel) + 3.14;
            pResults->mp_val[i] = fCalcResult - (IFX_float_t)pMeasMP->level -
                  (fCalcRef + 10);
            pResults->mp_val[i] = pResults->mp_val[i];
            pResults->mp_arg[i] = pMeasMP->level;
         }
         else
         {
            pResults->mp_val[i] = 0;
            pResults->mp_arg[i] = 0;
            continue;
         }
         pResults->mp_count = testResults.mp_count;
         break;

      /** Signal to Noise Ratio measurement */
      case DXS_ACLM_SNR:
         shift = 21 - pMeasMP->AcOutbSh;
         if (shift >= 0)
         {
            /* input value */
            fInput = (IFX_float_t)pMeasMP->AcOutb/(1 << shift)/(pMeasMP->Int << 3);
            qPeakLevel = sqrt (fInput);
            /* calculate resulting gain from factor */
            fCalcRef = 20 * log10(qPeakLevel) + 3.14;
         }
         else
         {
            pResults->mp_val[i] = 0;
            pResults->mp_arg[i] = 0;
            continue;
         }


         shift = 21 - pMeasMP->AcInbSh;
         if (shift >= 0)
         {
            /* input value */
            fInput = (IFX_float_t)pMeasMP->AcInb/(1 << shift)/(pMeasMP->Int << 3);
            qPeakLevel = sqrt (fInput);
            /* calculate resulting gain from factor */
            fCalcResult = 20 * log10(qPeakLevel) + 3.14;

            pResults->mp_val[i] = fCalcResult - fCalcRef;
            pResults->mp_arg[i] = pMeasMP->level;
         }
         else
         {
            pResults->mp_val[i] = 0;
            pResults->mp_arg[i] = 0;
            continue;
         }

         break;
      default:
         break;
      }
   }

   if (nType == DXS_ACLM_FR)
   {
      for (i = 0; i < testResults.mp_count; i++)
      {
         pResults->mp_val[i] -= ref_level;
      }
   }
   pResults->mp_count = testResults.mp_count;

   return ret;
}
