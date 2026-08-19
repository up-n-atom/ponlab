/****************************************************************************

                              Copyright (c) 2012
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/**
   \file lib_tapi_lt_gr909_vincpe.c  GR.909 linetesting library Vinetic 2CPE
   specific part
   Vinetic 2CPE specific calculation of result values.
*/


/* ============================= */
/* Includes                      */
/* ============================= */

#include "lib_tapi_nlt_osmap.h"

/* system includes */
#include <math.h>

#include "drv_tapi_io.h"

#include "lib_tapi_lt_gr909.h"
#include "lib_tapi_nlt_config.h"

#if (defined (LIB_TAPI_NLT_VINCPE_SUPPORT) && defined (LIB_TAPI_NLT_GR909))

/* ============================= */
/* Defines                       */
/* ============================= */


/* HPT Formulas */
/* formula for HPT AC calculations :
   R2G, T2G : Vresult[Vrms] = Value * (R2 + R1) / 32768 / 1.8844 / sqrt(2) / R2
        T2R : Vresult[Vrms] = Value * (R3 + R1) / 32768 / 1.8844 / sqrt(2) / R3
*/
#define HPT_AC_CALC(res,val,r_a,r_b) \
   (res)=(val)*((r_b)+(r_a))/32768.0/1.8844/sqrt(2)/(r_b);

/* formula for HPT DC calculations : R2G, T2G
   Vresult[V] = (Value * (R2 + R1) / R2 / 32768 / 1.8844) + 0.7
*/
#define HPT_DC_X2G_CALC(res,val,r_a,r_b) \
   (res)=((val)*((r_b)+(r_a))/(r_b)/32768.0/1.8844) + 0.7;

/* formula for HPT DC calculations : T2R
   Vresult[V] = (Value * (R3 + R1) / R3 / 32768 / 1.8844)
*/
#define HPT_DC_T2R_CALC(res,val,r_a,r_b) \
   (res)=((val)*((r_b)+(r_a))/(r_b)/32768.0/1.8844);

/* FEMF Formulas */
/* formula for HPT AC calculations :
   R2G, T2G : Vresult[Vrms] = Value * (R2 + R1) / 32768 / 1.8844 / sqrt(2) / R2
        T2R : Vresult[Vrms] = Value * (R3 + R1) / 32768 / 1.8844 / sqrt(2) / R3
*/
#define FEMF_AC_CALC(res,val,r_a,r_b) HPT_AC_CALC(res,val,r_a,r_b)

/* formula for HPT DC calculations : R2G, T2G
   Vresult[V] = (Value * (R2 + R1) / R2 / 32768 / 1.8844) + 0.7
*/
#define FEMF_DC_X2G_CALC(res,val,r_a,r_b) HPT_DC_X2G_CALC(res,val,r_a,r_b)

/* formula for HPT DC calculations : T2R
   Vresult[V] = (Value * (R3 + R1) / R3 / 32768 / 1.8844)
*/
#define FEMF_DC_T2R_CALC(res,val,r_a,r_b) HPT_DC_T2R_CALC(res,val,r_a,r_b)

/* RFT Formulas */
/* Formula for RFT calculations :
   R2G, T2G : Rresult[Ohm] = Value * (R2 + R1) / R2 / 26.12244898
        T2R : Rresult[Ohm] = Value * (R3 + R1) / R3 / 26.12244898
*/
#define RFT_CALC(res,val,r_a,r_b) \
   (res)=(val)*((r_b)+ (r_a))/(r_b)/26.12244898;

/* ROH Formulas */
/* Formula for ROG calculations :
   T2R_L, T2R_H  : Rresult[Ohm] = Value * (R3 + R1) / R3 / 26.12244898
*/
#define ROH_CALC(res,val,r_a,r_b) \
   (res)=(val)*((r_b)+ (r_a))/(r_b)/26.12244898;

/* RIT Formulas */
/* Formula for RIT calculations :
   Zresult[Ohm] = Value * (R3 + R1) / R3 / 105.3497998 * Git
   Git = 0.9653 for a 20 Hz Ringing signal used here for measurements
*/
#define RIT_CALC(res,val,r_a,r_b) \
   (res)=((val)*((r_b)+(r_a))/(r_b)/105.3497998*0.9653);

/* ============================= */
/* Local variable definition     */
/* ============================= */

/* default system configuration (SLIC DC) */
IFX_LT_GR909_CFG_t vincpe_cfg =
{
   /* default :  R1 = 1.5 MOhm */
   1500000.0,
   /* default :  R2 = 3289 Ohm */
   3289.0,
   /* default :  R3 = 3286 Ohm */
   3286.0,
};

/* ============================= */
/* Global variable definition    */
/* ============================= */

/* ============================= */
/* Local function declaration    */
/* ============================= */

static IFX_void_t lt_vincpe_getres_hpt  (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                         IFX_LT_GR909_HPT_t        *p_hpt);
static IFX_void_t lt_vincpe_getres_femf (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                         IFX_LT_GR909_FEMF_t       *p_femf);
static IFX_void_t lt_vincpe_getres_rft (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                        IFX_LT_GR909_RFT_t        *p_rft);
static IFX_void_t lt_vincpe_getres_roh  (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                         IFX_LT_GR909_ROH_t        *p_roh);
static IFX_void_t lt_vincpe_getres_rit  (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                         IFX_LT_GR909_RIT_t        *p_rit);
IFX_void_t VINCPE_LT_GR909_CalcResults (IFX_LT_GR909_RESULT_t *p_res,
                                            IFX_TAPI_GR909_RESULT_t *pTapiRes);

/* ============================= */
/* Local function definition     */
/* ============================= */

/**
   Calculates HPT results out of low level results
\param pTapiRes - low level results, in
\param p_hpt - HPT results, out
*/
static IFX_void_t lt_vincpe_getres_hpt (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                        IFX_LT_GR909_HPT_t        *p_hpt)
{
   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_HPT)
      p_hpt->b_result = IFX_TRUE;

   /* HPT AC RING wire to GND result */
   HPT_AC_CALC(p_hpt->f_hpt_ac_r2g, pTapiRes->HPT_AC_R2G, vincpe_cfg.f_R1, vincpe_cfg.f_R2);
   /* HPT AC TIP wire to GND result */
   HPT_AC_CALC(p_hpt->f_hpt_ac_t2g, pTapiRes->HPT_AC_T2G, vincpe_cfg.f_R1, vincpe_cfg.f_R2);
   /* HPT AC TIP wire to RING wire result */
   HPT_AC_CALC(p_hpt->f_hpt_ac_t2r, pTapiRes->HPT_AC_T2R, vincpe_cfg.f_R1, vincpe_cfg.f_R3);
   /* HPT DC RING wire to GND result */
   HPT_DC_X2G_CALC(p_hpt->f_hpt_dc_r2g, pTapiRes->HPT_DC_R2G, vincpe_cfg.f_R1, vincpe_cfg.f_R2);
   /* HPT DC TIP wire to GND result */
   HPT_DC_X2G_CALC(p_hpt->f_hpt_dc_t2g, pTapiRes->HPT_DC_T2G, vincpe_cfg.f_R1, vincpe_cfg.f_R2);
   /* HPT DC TIP wire to RING wire result */
   HPT_DC_T2R_CALC(p_hpt->f_hpt_dc_t2r, pTapiRes->HPT_DC_T2R, vincpe_cfg.f_R1, vincpe_cfg.f_R3);

   return;
}

/**
   Calculates FEMF results out of low level results
\param pTapiRes - low level results, in
\param p_hpt - FEMF results, out
*/
static IFX_void_t lt_vincpe_getres_femf (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                         IFX_LT_GR909_FEMF_t       *p_femf)
{
   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_FEMF)
      p_femf->b_result = IFX_TRUE;

   /* FEMF AC RING wire to GND result */
   FEMF_AC_CALC(p_femf->f_femf_ac_r2g, pTapiRes->FEMF_AC_R2G, vincpe_cfg.f_R1, vincpe_cfg.f_R2);
   /* FEMF AC TIP wire to GND result */
   FEMF_AC_CALC(p_femf->f_femf_ac_t2g, pTapiRes->FEMF_AC_T2G, vincpe_cfg.f_R1, vincpe_cfg.f_R2);
   /* FEMF AC TIP wire to RING wire result */
   FEMF_AC_CALC(p_femf->f_femf_ac_t2r, pTapiRes->FEMF_AC_T2R, vincpe_cfg.f_R1, vincpe_cfg.f_R3);
   /* FEMF DC RING wire to GND result */
   FEMF_DC_X2G_CALC(p_femf->f_femf_dc_r2g, pTapiRes->FEMF_DC_R2G, vincpe_cfg.f_R1, vincpe_cfg.f_R2);
   /* FEMF DC TIP wire to GND result */
   FEMF_DC_X2G_CALC(p_femf->f_femf_dc_t2g, pTapiRes->FEMF_DC_T2G, vincpe_cfg.f_R1, vincpe_cfg.f_R2);
   /* FEMF DC TIP wire to RING wire result */
   FEMF_DC_T2R_CALC(p_femf->f_femf_dc_t2r, pTapiRes->FEMF_DC_T2R, vincpe_cfg.f_R1, vincpe_cfg.f_R3);

   return;
}

/**
   Calculates RFT results out of low level results
\param pTapiRes - low level results, in
\param p_hpt - RFT results, out
*/
static IFX_void_t lt_vincpe_getres_rft (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                        IFX_LT_GR909_RFT_t        *p_rft)
{
   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_RFT)
      p_rft->b_result = IFX_TRUE;

   /* RFT RING wire to GND result */
   RFT_CALC(p_rft->f_rft_r2g, pTapiRes->RFT_R2G, vincpe_cfg.f_R1, vincpe_cfg.f_R2);
   /* RFT TIP wire to GND result */
   RFT_CALC(p_rft->f_rft_t2g, pTapiRes->RFT_T2G, vincpe_cfg.f_R1, vincpe_cfg.f_R2);
   /* RFT TIP wire to RING wire result */
   RFT_CALC(p_rft->f_rft_t2r, pTapiRes->RFT_T2R, vincpe_cfg.f_R1, vincpe_cfg.f_R3);

   return;
}

/**
   Calculates ROH results out of low level results
\param pTapiRes - low level results, in
\param p_hpt - ROH results, out
*/
static IFX_void_t lt_vincpe_getres_roh (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                        IFX_LT_GR909_ROH_t        *p_roh)
{
   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_ROH)
      p_roh->b_result = IFX_TRUE;

   /* ROH TIP wire to RING wire result for low voltage */
   ROH_CALC(p_roh->f_roh_t2r_l, pTapiRes->ROH_T2R_L, vincpe_cfg.f_R1, vincpe_cfg.f_R3);
   /* ROH TIP wire to RING wire result for high voltage */
   ROH_CALC(p_roh->f_roh_t2r_h, pTapiRes->ROH_T2R_H, vincpe_cfg.f_R1, vincpe_cfg.f_R3);

   return;
}

/**
   Calculates RIT results out of low level results
\param pTapiRes - low level results, in
\param p_hpt   - RIT results, out
*/
static IFX_void_t lt_vincpe_getres_rit (IFX_TAPI_GR909_RESULT_t *pTapiRes,
                                        IFX_LT_GR909_RIT_t        *p_rit)
{
   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_RIT)
      p_rit->b_result = IFX_TRUE;

   /* RIT result */
   RIT_CALC(p_rit->f_rit_res, pTapiRes->RIT_RES, vincpe_cfg.f_R1, vincpe_cfg.f_R3);

   return;
}

/**
   Calculates floating point GR909 measurement result values
   from TAPI values.

   \param p_res  - ptr to \ref IFX_LT_GR909_RESULT_t structure
   \param pTapiRes  - ptr to \ref IFX_TAPI_GR909_RESULT_t structure
   \return
      none
   \remark
      Result is filled in by the pointer p_res.
*/
IFX_void_t VINCPE_LT_GR909_CalcResults (IFX_LT_GR909_RESULT_t *p_res,
                                             IFX_TAPI_GR909_RESULT_t *meas_res)
{
   /* HPT results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_HPT)
   {
      p_res->valid_mask |= IFX_LT_GR909_HPT_MASK;
      lt_vincpe_getres_hpt (meas_res, &p_res->hpt);
   }
   /* FEMF results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_FEMF)
   {
      p_res->valid_mask |= IFX_LT_GR909_FEMF_MASK;
      lt_vincpe_getres_femf (meas_res, &p_res->femf);
   }
   /* RFT results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_RFT)
   {
      p_res->valid_mask |= IFX_LT_GR909_RFT_MASK;
      lt_vincpe_getres_rft (meas_res, &p_res->rft);
   }
   /* ROH results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_ROH)
   {
      p_res->valid_mask |= IFX_LT_GR909_ROH_MASK;
      lt_vincpe_getres_roh (meas_res, &p_res->roh);
   }
   /* RIT results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_RIT)
   {
      p_res->valid_mask |= IFX_LT_GR909_RIT_MASK;
      lt_vincpe_getres_rit (meas_res, &p_res->rit);
   }
}

#endif /* VIN_2CPE && TAPI_GR909 */



