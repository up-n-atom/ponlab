/****************************************************************************

                              Copyright (c) 2012
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/**
   \file lib_tapi_lt_gr909_dxs.c  Linetesting library DxS specific part.
   DuSLIC-xS specific calculation of result values.
*/


/* ============================= */
/* Includes                      */
/* ============================= */

#include "lib_tapi_nlt_osmap.h"

/* system includes */
#include <math.h>

#include "drv_tapi_io.h"

#include "lib_tapi_lt_gr909.h"

#ifndef VXWORKS
#include "lib_tapi_nlt_config.h"
#endif /* !VXWORKS */

#ifdef VXWORKS
#define inline __inline__
#endif /* VXWORKS */

#if (defined (LIB_TAPI_NLT_DXS_SUPPORT) && defined (LIB_TAPI_NLT_GR909))

/* ============================= */
/* Defines                       */
/* ============================= */

/* ============================= */
/* Local variable definition     */
/* ============================= */

const IFX_float_t fVoltSensRes = 1500000.0;
const IFX_float_t fResistanceScale = 16;

/* ============================= */
/* Global variable definition    */
/* ============================= */

/* default system configuration */
IFX_LT_GR909_CFG_t dxs_cfg =
{
   /* default :  R1 = 1.5 MOhm */
   1500000.0,
   /* not used for DuSLIC-xS */
   0.0,
   /* not used for DuSLIC-xS */
   0.0,
};

/* ============================= */
/* Local function declaration    */
/* ============================= */

static IFX_void_t lt_dxs_getres_hpt  (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                      IFX_LT_GR909_HPT_t *p_hpt);
static IFX_void_t lt_dxs_getres_femf (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                      IFX_LT_GR909_FEMF_t *p_femf);
static IFX_void_t lt_dxs_getres_rft (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                     IFX_LT_GR909_RFT_t *p_rft,
                                     IFX_boolean_t bExtended);
static IFX_void_t lt_dxs_getres_roh  (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                      IFX_LT_GR909_ROH_t *p_roh);
static IFX_void_t lt_dxs_getres_rit  (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                      IFX_LT_GR909_RIT_t *p_rit);
IFX_void_t DXS_LT_GR909_CalcResults (IFX_LT_GR909_RESULT_t *p_res,
                                     IFX_TAPI_GR909_RESULT_t *pTapiRes);

/* ============================= */
/* Local function definition     */
/* ============================= */

/* Formula for HPT AC calculations. */
static inline IFX_float_t HPT_FEMF_AC_CALC(IFX_float_t fR1)
{
   return (0.00707 * fR1 / fVoltSensRes);
}

/* Formula for HPT DC calculations */
static inline IFX_float_t HPT_FEMF_DC_CALC(IFX_float_t fR1)
{
   return (0.01 * fR1 / fVoltSensRes);
}

/* Formula for RFT, ROH and RIT calculations */
static inline IFX_float_t RFT_ROH_RIT_CALC(IFX_float_t fR1)
{
   return (fResistanceScale * fR1 / fVoltSensRes);
}

/**
   Calculates RFT results taking into consideration open loop calibration
   factors.

   \param  nMeas        measured value (in)
   \param  fOLFactor    open loop calibration factor (in)
   \param  pCalc        pointer to a calculated value (out)
*/
static IFX_return_t rft_ol_calc (IFX_uint32_t nMeas,
                                 IFX_float_t  fOLFactor,
                                 IFX_float_t  *pCalc)
{
   const IFX_float_t fLimit = 3000000;

   if (pCalc == IFX_NULL)
      return IFX_ERROR;

   *pCalc = (nMeas >> 4) << (nMeas & 0x000F);

   if ( -fOLFactor <= *pCalc)
   {
      /* avoid division by zero and negative results */
      *pCalc = fLimit;
   }
   else
   {
      /* consider open loop calibration */
      *pCalc = *pCalc * fOLFactor / (*pCalc + fOLFactor);
   }

   /* saturate */
   if (*pCalc > fLimit)
   {
      *pCalc = fLimit;
   }

   return IFX_SUCCESS;
}

/**
   Calculates HPT results from the low level results

   \param  pTapiRes     low level results (in)
   \param  p_hpt        HPT results (out)
*/
static IFX_void_t lt_dxs_getres_hpt  (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                     IFX_LT_GR909_HPT_t *p_hpt)
{
   IFX_float_t fConstFactor = 0.0;

   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_HPT)
      p_hpt->b_result = IFX_TRUE;

   fConstFactor = HPT_FEMF_AC_CALC(dxs_cfg.f_R1);
   /* HPT AC RING wire to GND result */
   p_hpt->f_hpt_ac_r2g = fConstFactor * (IFX_uint16_t)(pTapiRes->HPT_AC_R2G);
   /* HPT AC TIP wire to GND result */
   p_hpt->f_hpt_ac_t2g = fConstFactor * (IFX_uint16_t)(pTapiRes->HPT_AC_T2G);
   /* HPT AC TIP wire to RING wire result */
   p_hpt->f_hpt_ac_t2r = fConstFactor * (IFX_uint16_t)(pTapiRes->HPT_AC_T2R);

   fConstFactor = HPT_FEMF_DC_CALC(dxs_cfg.f_R1);
   /* HPT DC RING wire to GND result */
   p_hpt->f_hpt_dc_r2g = fConstFactor * pTapiRes->HPT_DC_R2G;
   /* HPT DC TIP wire to GND result */
   p_hpt->f_hpt_dc_t2g = fConstFactor * pTapiRes->HPT_DC_T2G;
   /* HPT DC TIP wire to RING wire result */
   p_hpt->f_hpt_dc_t2r = fConstFactor * pTapiRes->HPT_DC_T2R;

   return;
}

/**
   Calculates FEMF results from the low level results

   \param  pTapiRes     low level results, in
   \param  p_femf       FEMF results, out
*/
static IFX_void_t lt_dxs_getres_femf (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                      IFX_LT_GR909_FEMF_t *p_femf)
{
   IFX_float_t fConstFactor = 0.0;

   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_FEMF)
      p_femf->b_result = IFX_TRUE;

   fConstFactor = HPT_FEMF_AC_CALC(dxs_cfg.f_R1);
   /* FEMF AC RING wire to GND result */
   p_femf->f_femf_ac_r2g = fConstFactor * pTapiRes->FEMF_AC_R2G;
   /* FEMF AC TIP wire to GND result */
   p_femf->f_femf_ac_t2g = fConstFactor * pTapiRes->FEMF_AC_T2G;
   /* FEMF AC TIP wire to RING wire result */
   p_femf->f_femf_ac_t2r = fConstFactor * pTapiRes->FEMF_AC_T2R;

   fConstFactor = HPT_FEMF_DC_CALC(dxs_cfg.f_R1);
   /* FEMF DC RING wire to GND result */
   p_femf->f_femf_dc_r2g = fConstFactor * pTapiRes->FEMF_DC_R2G;
   /* FEMF DC TIP wire to GND result */
   p_femf->f_femf_dc_t2g = fConstFactor * pTapiRes->FEMF_DC_T2G;
   /* FEMF DC TIP wire to RING wire result */
   p_femf->f_femf_dc_t2r = fConstFactor * pTapiRes->FEMF_DC_T2R;

   return;
}

/**
   Calculates RFT results from the low level results

   \param  pTapiRes     low level results (in)
   \param  p_rft        RFT results (out)
   \param  bExtended    select the data representation of the FW (in)
*/
static IFX_void_t lt_dxs_getres_rft (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                     IFX_LT_GR909_RFT_t *p_rft,
                                     IFX_boolean_t bExtended)
{
   IFX_float_t fConstFactor = 0.0;

   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_RFT)
      p_rft->b_result = IFX_TRUE;

   if (bExtended == IFX_FALSE)
   {
      fConstFactor = RFT_ROH_RIT_CALC(dxs_cfg.f_R1);
      /* RFT RING wire to GND result */
      p_rft->f_rft_r2g = fConstFactor * (IFX_uint16_t)(pTapiRes->RFT_R2G);
      /* RFT TIP wire to GND result */
      p_rft->f_rft_t2g = fConstFactor * (IFX_uint16_t)(pTapiRes->RFT_T2G);
      /* RFT TIP wire to RING wire result */
      p_rft->f_rft_t2r = fConstFactor * (IFX_uint16_t)(pTapiRes->RFT_T2R);
   }
   else
   {
      /* RFT RING wire to GND result */
      rft_ol_calc((IFX_uint32_t)((IFX_uint16_t)pTapiRes->RFT_R2G),
                  pTapiRes->OLR_R2G, &p_rft->f_rft_r2g);
      /* RFT TIP wire to GND result */
      rft_ol_calc((IFX_uint32_t)((IFX_uint16_t)pTapiRes->RFT_T2G),
                  pTapiRes->OLR_T2G, &p_rft->f_rft_t2g);
      /* RFT TIP wire to RING wire result */
      rft_ol_calc((IFX_uint32_t)((IFX_uint16_t)pTapiRes->RFT_T2R),
                  pTapiRes->OLR_T2R, &p_rft->f_rft_t2r);
   }

   return;
}

/**
   Calculates ROH results from the low level results

   \param  pTapiRes     low level results (in)
   \param  p_roh        ROH results (out)
*/
static IFX_void_t lt_dxs_getres_roh  (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                     IFX_LT_GR909_ROH_t *p_roh)
{
   IFX_float_t fConstFactor = 0.0;

   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_ROH)
      p_roh->b_result = IFX_TRUE;

   fConstFactor = RFT_ROH_RIT_CALC(dxs_cfg.f_R1);
   /* ROH result low value */
   p_roh->f_roh_t2r_l = fConstFactor * (IFX_uint16_t)(pTapiRes->ROH_T2R_L);
   /* ROH result high value */
   p_roh->f_roh_t2r_h = fConstFactor * (IFX_uint16_t)(pTapiRes->ROH_T2R_H);

   return;
}

/**
   Calculates RIT results from the low level results

   \param  pTapiRes     low level results (in)
   \param  p_rit        RIT results (out)
*/
static IFX_void_t lt_dxs_getres_rit  (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                     IFX_LT_GR909_RIT_t *p_rit)
{
   IFX_float_t fConstFactor = 0.0;

   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_RIT)
      p_rit->b_result = IFX_TRUE;

   fConstFactor = RFT_ROH_RIT_CALC(dxs_cfg.f_R1);
   /* RIT result */
   p_rit->f_rit_res = fConstFactor * (IFX_uint16_t)(pTapiRes->RIT_RES);

   return;
}

/**
   Calculates floating point GR909 measurement result values
   from values provided by the TAPI driver.

   \param  p_res        Ptr to \ref IFX_LT_GR909_RESULT_t structure.
   \param  pTapiRes     Ptr to \ref IFX_TAPI_GR909_RESULT_t structure.

   \remark
   Result is filled in by the pointer p_res.
*/
IFX_void_t DXS_LT_GR909_CalcResults (IFX_LT_GR909_RESULT_t *p_res,
                                       IFX_TAPI_GR909_RESULT_t *meas_res)
{
   /* HPT results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_HPT)
   {
      p_res->valid_mask |= IFX_LT_GR909_HPT_MASK;
      lt_dxs_getres_hpt (meas_res, &p_res->hpt);
   }
   /* FEMF results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_FEMF)
   {
      p_res->valid_mask |= IFX_LT_GR909_FEMF_MASK;
      lt_dxs_getres_femf (meas_res, &p_res->femf);
   }
   /* RFT results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_RFT_VALID)
   {
      p_res->valid_mask |= IFX_LT_GR909_RFT_MASK;
      lt_dxs_getres_rft (meas_res, &p_res->rft, IFX_FALSE);
   }
   if (meas_res->valid & IFX_TAPI_GR909_RFT_EXT_VALID)
   {
      p_res->valid_mask |= IFX_LT_GR909_RFT_MASK;
      lt_dxs_getres_rft (meas_res, &p_res->rft, IFX_TRUE);
   }
   /* ROH results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_ROH)
   {
      p_res->valid_mask |= IFX_LT_GR909_ROH_MASK;
      lt_dxs_getres_roh (meas_res, &p_res->roh);
   }
   /* RIT results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_RIT)
   {
      p_res->valid_mask |= IFX_LT_GR909_RIT_MASK;
      lt_dxs_getres_rit (meas_res, &p_res->rit);
   }
}

#endif /* DXS && TAPI_GR909 */
