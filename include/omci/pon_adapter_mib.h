/******************************************************************************
 *
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_mib.h
 *
 * This is a PON Adapter header file, defining OMCI MIB functions.
 */

#ifndef _PON_ADAPTER_MIB_H_
#define _PON_ADAPTER_MIB_H_

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 *   @{
 */

/** \defgroup PON_ADAPTER_MIB PON Adapter MIB
 *  These functions are used to access the Management Information Base (MIB).
 *  @{
 */

/** OMCI MIB operations. */
struct pa_omci_mib_ops {
	/** OMCI MIB cleanup
	 *  Cleanup of global settings before new MIB is activated
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 */
	enum pon_adapter_errno (*cleanup)(void *ll_handle);
};

/** @} */ /* PON_ADAPTER_MIB */

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif
