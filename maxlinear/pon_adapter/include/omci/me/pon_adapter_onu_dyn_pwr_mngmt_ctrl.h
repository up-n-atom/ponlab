/******************************************************************************
 *
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * ONU Dynamic Power Management Control.
 */

#ifndef _pon_adapter_me_onu_dyn_pwr_mngmt_ctrl_h
#define _pon_adapter_me_onu_dyn_pwr_mngmt_ctrl_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PA_ME_ONU_DYN_PWR_MNGMT_CTRL\
 *	      ONU Dynamic Power Management Control
 *
 *   This Managed Entity models the ONU's ability to enter power conservation
 *   modes in cooperation with the OLT.
 *   An ONU that supports power conservation modes automatically creates an
 *   instance of this Managed Entity.
 *
 *  @{
 */

/** Power reduction Doze mode mask */
#define PA_ONU_DYN_PWR_MNGMT_CTRL_DOZE_MODE_MASK			0x1
/** Power reduction Cyclic sleep mode mask */
#define PA_ONU_DYN_PWR_MNGMT_CTRL_CYCLIC_SLEEP_MODE_MASK		0x2
/** Power reduction Watchful Sleep mode mask */
#define PA_ONU_DYN_PWR_MNGMT_CTRL_CYCLIC_WATCHFUL_SLEEP_MODE_MASK	0x4

/** ONU Dynamic Power Management Control update structure */
struct pa_onu_dyn_pwr_mngmt_ctrl_update_data {
	/** Power reduction management node */
	uint8_t pwr_reduction_mngmt_mode;
	/** Maximum sleep interval */
	uint32_t max_sleep_interval;
	/** Maximum receiver-off interval */
	uint32_t max_rx_off_interval;
	/** Minimum aware interval */
	uint32_t min_aware_interval;
	/** Minimum active held interval */
	uint16_t min_active_held_interval;
	/** Maximum sleep interval extension - doze */
	uint32_t max_sleep_interval_doze_ext;
	/** Maximum sleep interval extension - cyclic sleep */
	uint32_t max_sleep_interval_cyclic_sleep_ext;
	/** EPON setup extension */
	uint8_t epon_setup_ext;
	/** Missing consecutive bursts threshold */
	uint32_t miss_consecutive_bursts_thr;
};

/** ONU Dynamic Power Management Control Attribute Data operations structure */
struct pa_onu_dyn_pwr_mngmt_ctrl_attr_data_ops {
	/** Retrieve power reduction management capability
	 *
	 * \param[in]  ll_handle    Lower layer context pointer
	 * \param[in]  me_id        Managed Entity identifier
	 * \param[out] cap          Capability
	 */
	enum pon_adapter_errno (*pwr_reduction_mngmt_cap_get)(void *ll_handle,
							      uint16_t me_id,
							      uint8_t *cap);
	/** Retrieve power reduction management mode
	 *
	 * \param[in]  ll_handle    Lower layer context pointer
	 * \param[in]  me_id        Managed Entity identifier
	 * \param[out] mode         Management mode
	 */
	enum pon_adapter_errno (*pwr_reduction_mngmt_mode_get)(void *ll_handle,
							       uint16_t me_id,
							       uint8_t *mode);
	/** Retrieve Itransinit
	 *
	 * \param[in]  ll_handle    Lower layer context pointer
	 * \param[in]  me_id        Managed Entity identifier
	 * \param[out] itrans       Itransinit
	 */
	enum pon_adapter_errno (*itransinit_get)(void *ll_handle,
						 uint16_t me_id,
						 uint16_t *itrans);
	/** Retrieve Itxinit
	 *
	 * \param[in]  ll_handle    Lower layer context pointer
	 * \param[in]  me_id        Managed Entity identifier
	 * \param[out] itx          Itxinit
	 */
	enum pon_adapter_errno (*itxinit_get)(void *ll_handle,
					      uint16_t me_id,
					      uint16_t *itx);
	/** Retrieve maximum sleep interval
	 *
	 * \param[in]  ll_handle    Lower layer context pointer
	 * \param[in]  me_id        Managed Entity identifier
	 * \param[out] interval     Maximum sleep interval (us)
	 */
	enum pon_adapter_errno (*max_sleep_interval_get)(void *ll_handle,
							 uint16_t me_id,
							 uint32_t *interval);
	/** Retrieve minimum aware interval
	 *
	 * \param[in]  ll_handle    Lower layer context pointer
	 * \param[in]  me_id        Managed Entity identifier
	 * \param[out] interval     Minimum aware interval (us)
	 */
	enum pon_adapter_errno (*min_aware_interval_get)(void *ll_handle,
							 uint16_t me_id,
							 uint32_t *interval);
	/** Retrieve minimum active held interval
	 *
	 * \param[in]  ll_handle    Lower layer context pointer
	 * \param[in]  me_id        Managed Entity identifier
	 * \param[out] interval     Minimum active held interval (us)
	 */
	enum pon_adapter_errno (*min_active_held_interval_get)(void *ll_handle,
					uint16_t me_id, uint16_t *interval);
	/** Retrieve maximum sleep interval extension
	 *
	 * \param[in]  ll_handle    Lower layer context pointer
	 * \param[in]  me_id        Managed Entity identifier
	 * \param[out] interval     Maximum sleep interval extension (us)
	 */
	enum pon_adapter_errno (*max_sleep_interval_ext_get)(void *ll_handle,
					uint16_t me_id, uint64_t *interval);
};

/** ONU Dynamic Power Management Control operations structure */
struct pa_onu_dyn_pwr_mngmt_ctrl_ops {
	/** Updates ONU dynamic power management control Managed Entity
	 *  resources
	 *
	 * \param[in] ll_handle			Lower layer context pointer
	 * \param[in] me_id			Managed Entity identifier
	 * \param[in] pwr_reduction_mngmt_mode	Power reduction management mode
	 * \param[in] max_sleep_interval	Maximum sleep interval
	 * \param[in] max_rx_off_interval	Maximum rx-off interval
	 * \param[in] min_aware_interval	Minimum aware interval
	 * \param[in] min_active_held_interval	Minimum active held interval
	 * \param[in] max_sleep_interval_doze_ext Maximum Sleep Interval
	 *					Extension (Doze Mode)
	 * \param[in] max_sleep_interval_cyclic_sleep_ext Maximum Sleep Interval
	 *						  Extension (Cyclic
	 *						  Sleep Mode)
	 * \param[in] epon_setup_ext	EPON setup extension
	 * \param[in] miss_consecutive_bursts_thr Missing Consecutive Bursts
	 *	      Threshold
	*/
	enum pon_adapter_errno (*update)(void *ll_handle, uint16_t me_id,
	    const struct pa_onu_dyn_pwr_mngmt_ctrl_update_data *data);
	/** Attribute data */
	const struct pa_onu_dyn_pwr_mngmt_ctrl_attr_data_ops *attr_data;
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
