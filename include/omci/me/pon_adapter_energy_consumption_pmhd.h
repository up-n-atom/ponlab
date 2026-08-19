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
 * Energy Consumption PMHD.
 */

#ifndef _pon_adapter_me_energy_consumption_pmhd
#define _pon_adapter_me_energy_consumption_pmhd

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 * @{
 */

/** \defgroup PON_ADAPTER_ME_ENERGY_CONSUMPTION_PMHD Energy Consumption PMHD
 * This Managed Entity collects performance monitoring data associated with
 * the ONU's energy consumption.
 *
 * Instances of this Managed Entity are created and deleted by the OLT.
 *
 * @{
 */

/** Energy Consumption PMHD operations structure */
struct pa_energy_consumption_pmhd_ops {
	/** Retrieve total counters for Energy Consumption PMHD
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[out] cnt_doze_time    Time in Doze mode
	 * \param[out] cnt_cyclic_time  Time in Cyclic Sleep mode
	 */
	enum pon_adapter_errno (*cnt_get)(
		void *ll_handle,
		uint16_t me_id,
		uint64_t *cnt_doze_time,
		uint64_t *cnt_cyclic_time);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
