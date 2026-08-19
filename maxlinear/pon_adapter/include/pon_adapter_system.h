/******************************************************************************
 *
 *  Copyright (c) 2021 - 2025 MaxLinear, Inc.
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_system.h
 *
 * This is the PON adapter header file for lower layer system functions.
*/

#ifndef _PON_ADAPTER_SYSTEM_H_
#define _PON_ADAPTER_SYSTEM_H_

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER

    @{
*/

/** \defgroup PON_ADAPTER_SYSTEM PON Adapter Lower Layer System Control
 *   These functions are used for lower layer system control by the PON
 *   adaptation layer.
 *
 *   @{
 */

struct pa_config;
struct pa_eh_ops;

/** OMCI low-level system operations structure */
struct pa_system_ops {
	/** Initialize the device.
	 *  To be called once.
	 *
	 * \param[in]     init_data	Init data
	 * \param[in]     config	UCI configuration access
	 *				This pointer has to life till the
	 *				reboot or shutdown function was	called.
	 *				The calling function is allowed to
	 *				store this pointer and use it later.
	 * \param[in]     event_handler	Event handler pointer
	 * \param[in,out] ll_handle	Lower layer context pointer
	 *				This pointer has to life till the
	 *				reboot or shutdown function was	called.
	 *				The calling function is allowed to
	 *				store this pointer and use it later.
	 */
	enum pon_adapter_errno (*init)(char const * const *init_data,
				       const struct pa_config *config,
				       const struct pa_eh_ops *event_handler,
				       void *ll_handle);
	/** Make the basic initialization (enable events, interrupts, ...).
	 *  To be called once.
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 */
	enum pon_adapter_errno (*start)(void *ll_handle);
	/** Reboot ONU
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] timeout_ms	Reboot in timeout_ms (in ms)
	 */
	enum pon_adapter_errno (*reboot)(void *ll_handle,
					 unsigned long timeout_ms);
	/** Terminate device.
	 *  To be called once.
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 */
	enum pon_adapter_errno (*shutdown)(void *ll_handle);
};

/** @} */ /* PON_ADAPTER_SYSTEM */

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif
