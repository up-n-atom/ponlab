/******************************************************************************
 *
 * Copyright (c) 2021 - 2024 MaxLinear, Inc.
 * Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/
/**
   \file fapi_pon_pa_register.h
   This is the PON ADAPTER WRAPPER header file.
*/

#ifndef _FAPI_PON_PA_REGISTER_H_
#define _FAPI_PON_PA_REGISTER_H_

#include "pon_adapter.h"
#include "pon_adapter_errno.h"

/* extern declarations of all existing ops structures */
extern const struct pon_adapter_ani_g_ops pon_pa_ani_g_ops;
extern const struct pon_adapter_enh_sec_ctrl_ops pon_pa_enh_sec_ctrl_ops;
extern const struct pa_fec_pmhd_ops pon_pa_fec_pmhd_ops;
extern const struct pa_gem_port_net_ctp_ops pon_pa_gem_port_net_ctp_ops;
extern const struct pa_gem_port_net_ctp_pmhd_ops
			pon_pa_gem_port_net_ctp_pmhd_ops;
extern const struct pa_enhanced_tc_pmhd_ops pon_pa_enhanced_tc_pmhd_ops;
extern const struct pa_management_cnt_ops pon_pa_mngmt_cnt_ops;
extern const struct pa_olt_g_ops pon_pa_olt_g_ops;
extern const struct pa_onu_g_ops pon_pa_onu_g_ops;
extern const struct pa_onu_dyn_pwr_mngmt_ctrl_ops
			pon_pa_onu_dyn_pwr_mngmt_ctrl_ops;
extern const struct pa_twdm_system_profile_ops pon_pa_twdm_system_profile_ops;
extern const struct pa_twdm_channel_ops pon_pa_twdm_channel_ops;
extern const struct pa_twdm_channel_xgem_pmhd_ops
			pon_pa_twdm_channel_xgem_pmhd_ops;
extern const struct pa_twdm_channel_phy_lods_pmhd_ops
			pon_pa_twdm_channel_phy_lods_pmhd_ops;
extern const struct pa_twdm_channel_ploam_pmhd_ops
			pon_pa_twdm_channel_ploam_pmhd_ops;
extern const struct pa_twdm_channel_tuning_pmhd_ops
		  pon_pa_twdm_channel_tuning_pmhd_ops;

/**
 * Register lower layer functions in higher layer module.
 * \param[in] hl_handle_legacy Pointer must be set to NULL.
 * \param[out] pa_ops Pointer to lower layer operations structure.
 * \param[out] ll_handle Pointer to lower layer module.
 * \param[in] if_version PON-Adapter IF version used by calling function.
 * \param[in] hl_handle Pointer to higher layer module.
 *
 * \remarks The function returns an error code in case of error.
 * The error code is described in \ref pon_adapter_errno.
 *
 * \return Return value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno libpon_ll_register_ops(void *hl_handle_legacy,
					      const struct pa_ops **pa_ops,
					      void **ll_handle,
					      void *hl_handle,
					      uint32_t if_version);

#endif /* _FAPI_PON_PA_REGISTER_H_ */
