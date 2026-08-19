/* Copyright 2017, Intel Corporation */
/******************************************************************************

                          Copyright (c) 2015-2017
                     Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _dsl_fapi_common_h
#define _dsl_fapi_common_h

#define IN
#define OUT
#define IN_OUT
#define VENDOR_PATH "/opt/intel"

#include "dsl_fapi.h"
#include "dsl_fapi_config.h"
#include "drv_dsl_cpe_api.h"
#include "drv_dsl_cpe_api_ioctl.h"
#include "ifx_types.h"
#include "drv_mei_cpe_interface.h"
#include "dsl_fapi_safec_wrapper.h"

#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#define DLL_LOCAL  __attribute__ ((visibility ("hidden")))

struct standarts {
	unsigned int octet;
	unsigned int bit;
	char *name;
};
struct standarts standarts_table[] = {
	{0, XTSE_1_01_A_T1_413, ",T1.413"},
	{0, XTSE_1_02_C_TS_101388, ",ETSI_101_388"},
	{0, XTSE_1_03_A_1_NO | XTSE_1_04_A_1_O, ",G.992.1_Annex_A"},
	{0, XTSE_1_05_B_1_NO | XTSE_1_06_B_1_O, ",G.992.1_Annex_B"},
	{0, XTSE_1_07_C_1_NO | XTSE_1_08_C_1_O, ",G.992.1_Annex_C"},

	{1, XTSE_2_01_A_2_NO, ",G.992.2_Annex_A"},
	{1, XTSE_2_02_B_2_O, ",G.992.2_Annex_B"},
	{1, XTSE_2_03_C_2_NO | XTSE_2_04_C_2_O, ",G.992.2_Annex_C"},

	{2, XTSE_3_03_A_3_NO | XTSE_3_04_A_3_O, ",G.992.3_Annex_A"},
	{2, XTSE_3_05_B_3_NO | XTSE_3_06_B_3_O, ",G.992.3_Annex_B"},

	{3, XTSE_4_01_A_4_NO | XTSE_4_02_A_4_O, ",G.992.4_Annex_A"},
	{3, XTSE_4_05_I_3_NO | XTSE_4_06_I_3_O, ",G.992.3_Annex_I"},
	{3, XTSE_4_07_J_3_NO | XTSE_4_08_J_3_O, ",G.992.3_Annex_J"},

	{4, XTSE_5_01_I_4_NO | XTSE_5_02_I_4_O, ",G.992.4_Annex_I"},
	{4, XTSE_5_03_L_3_NO | XTSE_5_04_L_3_NO, ",G.992.3_Annex_L"},
	{4, XTSE_5_05_L_3_O | XTSE_5_06_L_3_O, ",G.992.3_Annex_L"},
	{4, XTSE_5_07_M_3_NO | XTSE_5_08_M_3_O, ",G.992.3_Annex_M"},

	{5, XTSE_6_01_A_5_NO | XTSE_6_02_A_5_O, ",G.992.5_Annex_A"},
	{5, XTSE_6_03_B_5_NO | XTSE_6_04_B_5_O, ",G.992.5_Annex_B"},
	{5, XTSE_6_07_I_5_NO | XTSE_6_08_I_5_O, ",G.992.5_Annex_I"},

	{6, XTSE_7_01_J_5_NO | XTSE_7_02_J_5_O, ",G.992.5_Annex_J"},
	{6, XTSE_7_03_M_5_NO | XTSE_7_04_M_5_O, ",G.992.5_Annex_M"},

	{7, XTSE_8_01_A, ",G.993.2_Annex_A"},
	{7, XTSE_8_02_B, ",G.993.2_Annex_B"},
	{7, XTSE_8_03_C, ",G.993.2_Annex_C"}
};

char *profiles_table[] = { ",8a", ",8b", ",8c", ",8d", ",12a", ",12b", ",17a", ",30a", ",35b" };
char *power_table[] = { "L0", "L1", "L2", "L3" };

#endif
