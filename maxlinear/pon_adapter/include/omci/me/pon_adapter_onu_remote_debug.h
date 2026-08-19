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
 * ONU Remote Debug.
 */

#ifndef _pon_adapter_me_onu_remote_debug
#define _pon_adapter_me_onu_remote_debug

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 * @{
 */

/** \defgroup PON_ADAPTER_REMOTE_DEBUG Remote Debug
 *  These structures are used to handle the remote debug feature.
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_REMOTE_DEBUG ONU Remote Debug
 *  This Managed Entity implements the remote debug access from OLT side.
 *  @{
 */

/** ONU Remote Debug operations structure */
struct pa_onu_remote_debug_ops {
	/** Execute debug command
	 *
	 * \param[in] ll_handle           Lower layer context pointer
	 * \param[in] cli                 Command line
	 * \param[out] data               Output data
	 * \param[in] data_size           Maximum output data size
	 */
	enum pon_adapter_errno (*exec_cmd)(
		void *ll_handle,
		const char *cli,
		char *data,
		unsigned int data_size);

	/** Returns the maximum length of the cmd buffer.
	 *
	 * \param[in] ll_handle           Lower layer context pointer
	 * \param[out] length		  Maximum length of the cmd buffer
	 */
	enum pon_adapter_errno (*cmd_max_length_get)(void *ll_handle,
						     uint32_t *length);
};

/** @} */

/** @} */ /*PON_ADAPTER_ME */

/** @} */ /* PON_ADAPTER_ME_REMOTE_DEBUG */

#ifdef __cplusplus
}
#endif

#endif
