/****************************************************************************

                            Copyright (c) 2012, 2015
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/**
   \file lib_tapi_lt_gr909_vmmc.c  GR.909 linetesting library VMMC specific part
   SLIC100 specific calculation of result values.
*/


/* ============================= */
/* Includes                      */
/* ============================= */

#include "lib_tapi_nlt_config.h"

#if (defined (LIB_TAPI_NLT_VMMC_SUPPORT) && defined (LIB_TAPI_NLT_GR909))

#include "lib_tapi_nlt_osmap.h"

#include "drv_tapi_io.h"

#include "lib_tapi_lt_gr909.h"

/* ============================= */
/* Defines                       */
/* ============================= */

/* HPT and FEMF Formulas */
/* Formula for the conversion of HPT_AC_R2G, HPT_AC_T2G, HPT_AC_T2R into
   physical values: Vresult[Vrms] = Val * 0.00523 */
#define HPT_AC_CALC(res,val)  (res)=((val) * 0.00523)

/* Formula for the conversion of FEMF_AC_R2G, FEMF_AC_T2G and FEMF_AC_T2R into
   physical values: Vresult[Vrms] = Val * 0.00523 */
#define FEMF_AC_CALC(res,val) (res)=((val) * 0.00523)

/* Formula for the conversion of HPT_DC_R2G, HPT_DC_T2G, and HPT_DC_T2R into
   physical values: Vresult[Vrms] = VAL * 0.0074 */
#define HPT_DC_CALC(res,val)  (res)=((val) * 0.0074)

/* Formula for the conversion of FEMF_DC_R2G, FEMF_DC_T2G and FEMF_DC_T2R into
   physical values: Vresult[Vrms] = VAL * 0.0074 */
#define FEMF_DC_CALC(res,val) (res)=((val) * 0.0074)

/* RFT and ROH Formulas */
/* Formula for the conversion of RFT_R2G, RFT_T2G and RFT_T2R into
   physical values: Rresult[Ohm] = VAL * 19.56 */
#define RFT_CALC(res,val)  (res)=((val) * 19.56)

/* Formula for the conversion of ROH_T2R_L and ROH_T2R_H into
   physical values: Rresult[Ohm] = VAL * 19.56 */
#define ROH_CALC(res,val)  (res)=((val) * 19.56)

/* RIT Formula */
/* Formula for the conversion of RIT into physical values:
   Zresult[Ohm] = VAL * 19.56 */
#define RIT_CALC(res,val)  (res)=((val) * 19.56)

/* ============================= */
/* Local variable definition     */
/* ============================= */

/* default system configuration (SLIC 100) */
/* This is needed for compatibility but not used in this file any more. */
IFX_LT_GR909_CFG_t vmmc_cfg =
{
   /* default :  R1 = 1 MOhm  */
   1000000.0,
   /* not used for Slic 100 */
   0.0,
   /* not used for Slic 100 */
   0.0
};

/* ============================= */
/* Global variable definition    */
/* ============================= */

/* ============================= */
/* Local function declaration    */
/* ============================= */

static IFX_void_t lt_vmmc_getres_hpt  (
                        IFX_TAPI_GR909_RESULT_t *pTapiRes,
                        IFX_LT_GR909_HPT_t *p_hpt);

static IFX_void_t lt_vmmc_getres_femf (
                        IFX_TAPI_GR909_RESULT_t *pTapiRes,
                        IFX_LT_GR909_FEMF_t *p_femf);

static IFX_void_t lt_vmmc_getres_rft (
                        IFX_TAPI_GR909_RESULT_t *pTapiRes,
                        IFX_LT_GR909_RFT_t *p_rft);

static IFX_void_t lt_vmmc_getres_roh (
                        IFX_TAPI_GR909_RESULT_t *pTapiRes,
                        IFX_LT_GR909_ROH_t *p_roh);

static IFX_void_t lt_vmmc_getres_rit (
                        IFX_TAPI_GR909_RESULT_t *pTapiRes,
                        IFX_LT_GR909_RIT_t *p_rit);

IFX_void_t VMMC_LT_GR909_CalcResults (
                        IFX_LT_GR909_RESULT_t *p_res,
                        IFX_TAPI_GR909_RESULT_t *pTapiRes);

/* ============================= */
/* Local function definition     */
/* ============================= */

/**
   Convert HPT results from internal values to physical values.

   \param  pTapiRes     Pointer to struct with results in FW representation.
   \param  p_hpt        Pointer to struct for results in physical values.
*/
static IFX_void_t lt_vmmc_getres_hpt (
                        IFX_TAPI_GR909_RESULT_t *pTapiRes,
                        IFX_LT_GR909_HPT_t *p_hpt)
{
   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_HPT)
      p_hpt->b_result = IFX_TRUE;

   /* HPT AC RING wire to GND result */
   HPT_AC_CALC(p_hpt->f_hpt_ac_r2g, pTapiRes->HPT_AC_R2G);
   /* HPT AC TIP wire to GND result */
   HPT_AC_CALC(p_hpt->f_hpt_ac_t2g, pTapiRes->HPT_AC_T2G);
   /* HPT AC TIP wire to RING wire result */
   HPT_AC_CALC(p_hpt->f_hpt_ac_t2r, pTapiRes->HPT_AC_T2R);
   /* HPT DC RING wire to GND result */
   HPT_DC_CALC(p_hpt->f_hpt_dc_r2g, pTapiRes->HPT_DC_R2G);
   /* HPT DC TIP wire to GND result */
   HPT_DC_CALC(p_hpt->f_hpt_dc_t2g, pTapiRes->HPT_DC_T2G);
   /* HPT DC TIP wire to RING wire result */
   HPT_DC_CALC(p_hpt->f_hpt_dc_t2r, pTapiRes->HPT_DC_T2R);

   return;
}


/**
   Convert FEMF results from internal values to physical values.

   \param  pTapiRes     Pointer to struct with results in FW representation.
   \param  p_femf       Pointer to struct for results in physical values.
*/
static IFX_void_t lt_vmmc_getres_femf (
                        IFX_TAPI_GR909_RESULT_t *pTapiRes,
                        IFX_LT_GR909_FEMF_t *p_femf)
{
   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_FEMF)
      p_femf->b_result = IFX_TRUE;

   /* FEMF AC RING wire to GND result */
   FEMF_AC_CALC(p_femf->f_femf_ac_r2g, pTapiRes->FEMF_AC_R2G);
   /* FEMF AC TIP wire to GND result */
   FEMF_AC_CALC(p_femf->f_femf_ac_t2g, pTapiRes->FEMF_AC_T2G);
   /* FEMF AC TIP wire to RING wire result */
   FEMF_AC_CALC(p_femf->f_femf_ac_t2r, pTapiRes->FEMF_AC_T2R);
   /* FEMF DC RING wire to GND result */
   FEMF_DC_CALC(p_femf->f_femf_dc_r2g, pTapiRes->FEMF_DC_R2G);
   /* FEMF DC TIP wire to GND result */
   FEMF_DC_CALC(p_femf->f_femf_dc_t2g, pTapiRes->FEMF_DC_T2G);
   /* FEMF DC TIP wire to RING wire result */
   FEMF_DC_CALC(p_femf->f_femf_dc_t2r, pTapiRes->FEMF_DC_T2R);

   return;
}


/**
   Convert RFT results from internal values to physical values.

   \param  pTapiRes     Pointer to struct with results in FW representation.
   \param  p_rft        Pointer to struct for results in physical values.
*/
static IFX_void_t lt_vmmc_getres_rft (
                        IFX_TAPI_GR909_RESULT_t *pTapiRes,
                        IFX_LT_GR909_RFT_t *p_rft)
{
   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_RFT)
      p_rft->b_result = IFX_TRUE;

   /* RFT RING wire to GND result */
   RFT_CALC(p_rft->f_rft_r2g, pTapiRes->RFT_R2G);
   /* RFT TIP wire to GND result */
   RFT_CALC(p_rft->f_rft_t2g, pTapiRes->RFT_T2G);
   /* RFT TIP wire to RING wire result */
   RFT_CALC(p_rft->f_rft_t2r, pTapiRes->RFT_T2R);

   return;
}


/**
   Convert ROH results from internal values to physical values.

   \param  pTapiRes     Pointer to struct with results in FW representation.
   \param  p_roh        Pointer to struct for results in physical values.
*/
static IFX_void_t lt_vmmc_getres_roh (
                        IFX_TAPI_GR909_RESULT_t *pTapiRes,
                        IFX_LT_GR909_ROH_t *p_roh)
{
   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_ROH)
      p_roh->b_result = IFX_TRUE;

   /* ROH TIP wire to RING wire result for low voltage */
   ROH_CALC(p_roh->f_roh_t2r_l, pTapiRes->ROH_T2R_L);
   /* ROH TIP wire to RING wire result for high voltage */
   ROH_CALC(p_roh->f_roh_t2r_h, pTapiRes->ROH_T2R_H);

   return;
}


/**
   Convert RIT results from internal values to physical values.

   \param  pTapiRes     Pointer to struct with results in FW representation.
   \param  p_rit        Pointer to struct for results in physical values.
*/
static IFX_void_t lt_vmmc_getres_rit (
                        IFX_TAPI_GR909_RESULT_t *pTapiRes,
                        IFX_LT_GR909_RIT_t *p_rit)
{
   /* test passed ? */
   if (pTapiRes->passed & IFX_TAPI_GR909_RIT)
      p_rit->b_result = IFX_TRUE;

   /* RIT result */
   RIT_CALC(p_rit->f_rit_res, pTapiRes->RIT_RES);

   return;
}


/**
   Calculates GR909 measurement result physical values in floating point
   representation from TAPI internal integer values.

   \param  p_res        Pointer to \ref IFX_LT_GR909_RESULT_t structure.
                        The physical values are written into this struct after
                        the conversion.
   \param  pTapiRes     Pointer to \ref IFX_TAPI_GR909_RESULT_t structure.
                        The values in this struct are the input to the
                        conversion.
*/
IFX_void_t VMMC_LT_GR909_CalcResults (
                        IFX_LT_GR909_RESULT_t *p_res,
                        IFX_TAPI_GR909_RESULT_t *meas_res)
{
   /* HPT results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_HPT)
   {
      p_res->valid_mask |= IFX_LT_GR909_HPT_MASK;
      lt_vmmc_getres_hpt (meas_res, &p_res->hpt);
   }
   /* FEMF results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_FEMF)
   {
      p_res->valid_mask |= IFX_LT_GR909_FEMF_MASK;
      lt_vmmc_getres_femf (meas_res, &p_res->femf);
   }
   /* RFT results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_RFT)
   {
      p_res->valid_mask |= IFX_LT_GR909_RFT_MASK;
      lt_vmmc_getres_rft (meas_res, &p_res->rft);
   }
   /* ROH results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_ROH)
   {
      p_res->valid_mask |= IFX_LT_GR909_ROH_MASK;
      lt_vmmc_getres_roh (meas_res, &p_res->roh);
   }
   /* RIT results valid ? */
   if (meas_res->valid & IFX_TAPI_GR909_RIT)
   {
      p_res->valid_mask |= IFX_LT_GR909_RIT_MASK;
      lt_vmmc_getres_rit (meas_res, &p_res->rit);
   }
}

#endif /* VMMC && TAPI_GR909 */
