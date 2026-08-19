/******************************************************************************
 *
 *  Copyright (c) 2023 MaxLinear, Inc.
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_msg.h
 *
 * This is a PON Adapter header file, defining OMCI message functions.
 */

#ifndef _PON_ADAPTER_MSG_H_
#define _PON_ADAPTER_MSG_H_

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 *
 *   @{
 */

/** \defgroup OMCI_MESSAGE_HANDLING OMCI Message Handling
 *
 * These functions and structures are used to register the callback functions
 * for OMCI message exchange between the higher layer (hl) and
 * the lower layer (ll) modules.
 *
 * @{
 */

/** OMCI message operations */
struct pa_msg_ops {
	/** OMCI message reception callback registration
	 *
	 *  \param[in] ll_handle         Lower layer context pointer
	 *  \param[in] receive_callback  CB routine for received OMCI msg
	 *  \param[in] hl_handle         Higher layer context pointer
	 *
	 */
	enum pon_adapter_errno (*msg_rx_cb_register)(
		void *ll_handle,
		enum pon_adapter_errno (*receive_callback)(void *hl_handle,
							   const uint8_t *msg,
							   const uint16_t len,
							   const uint32_t *crc),
		void *hl_handle);

	/** OMCI message reception callback cleanup
	 *
	 *  \param[in] ll_handle         Lower layer context pointer
	 *  \param[in] hl_handle         Higher layer context pointer
	 *
	 */
	enum pon_adapter_errno (*msg_rx_cb_clear)(void *ll_handle,
						  void *hl_handle);

	/** OMCI message sending
	 *
	 *  \param[in] ll_handle        Lower layer context pointer
	 *  \param[in] msg              OMCI msg to send
	 *  \param[in] len              Length of OMCI msg to send
	 *  \param[in] crc              (optional) CRC of OMCI msg
	 *                              if NULL, CRC is ignored
	 */
	enum pon_adapter_errno (*msg_send)(void *ll_handle,
					   const uint8_t *msg,
					   const uint16_t len,
					   const uint32_t *crc);
};

/** OMCI integrity key operations */
struct pa_integrity_ops {
	/** OMCI integrity key (OMCI-IK) get function */
	enum pon_adapter_errno (*key_get)(void *ll_handle, uint8_t *key,
					  uint32_t key_len);
};

/** @} */ /* OMCI_MESSAGE_HANDLING */

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif
