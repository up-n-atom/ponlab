/******************************************************************************
 *
 *  Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_qos.h
 *
 * This is the PON adapter header file for EPON Quality of Service related
 * functions.
 */

#ifndef _PON_ADAPTER_QOS_H_
#define _PON_ADAPTER_QOS_H_

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 * @{
 */

/** \defgroup PON_ADAPTER_QOS
 *  @{
 */

/** EPON Quality of Service related operations */
struct epon_qos_ops {
	/**
	 * \brief Operation to set configuration of number of queue sets for
	 * report threshold for queue indexes.
	 *
	 * \param[in] ll_handle      Lower layer context pointer
	 * \param[in] qos_idx        Index under which firmware stores QoS
	 *                           queue index
	 * \param[in] virt_size	     The number of QoS queue entries which
	 *                           shall be aggregated in one unit for
	 *                           threshold reporting towards OLT expressed
	 *                           in bytes.
	 * \param[in] th[4]          Array of thresholds for this QoS queue.
	 *                           If set to 0 no threshold is set.
	 */
	enum pon_adapter_errno (*thresholds_set)(void *ll_handle,
							uint8_t qos_idx,
							uint16_t virt_size,
							uint32_t th[4]);

	/**
	 * \brief Operation to get configuration of number of queue sets for
	 * report threshold for queue indexes.
	 *
	 * \param[in] ll_handle      Lower layer context pointer
	 * \param[in] qos_idx        Index under which firmware stores QoS
	 *                           queue index
	 * \param[out] virt_size     The number of QoS queue entries which
	 *                           shall be aggregated in one unit for
	 *                           threshold reporting towards OLT expressed
	 *                           in bytes.
	 * \param[out] th[4]         Array of thresholds for this QoS queue.
	 *                           If set to 0 no threshold is set.
	 */
	enum pon_adapter_errno (*thresholds_get)(void *ll_handle,
							uint8_t qos_idx,
							uint16_t *virt_size,
							uint32_t th[4]);

};

/** @} */ /* PON_ADAPTER_QOS */

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif
