/******************************************************************************
 *
 *  Copyright (c) 2022 MaxLinear, Inc.
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
 * ONU Enhanced Security Control.
 */

#ifndef _pon_adapter_me_enhanced_security_control_h
#define _pon_adapter_me_enhanced_security_control_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 * @{
 */

/** \defgroup PON_ADAPTER_ME_ENHANCED_SECURITY_CONTROL\
 *	      Enhanced Security Control
 *
 * This Managed Entity contains the capabilities, parameters and controls of
 * enhanced security features when they are negotiated via the OMCI.
 * @{
 */

/** Specification of the encryption algorithm type */
enum pon_adapter_enh_sec_ctrl_enc {
	/** AES-CMAC-128 */
	ENC_AES_CMAC_128 = 0,
	/** HMAC-SHA-256 */
	ENC_HMAC_SHA_256 = 1,
	/** HMAC-SHA-512 */
	ENC_HMAC_SHA_512 = 2
};

/** Enhanced security control operations structure */
struct pon_adapter_enh_sec_ctrl_ops {
	/** Retrieve supported encryption mode
	 *
	 * \param[in]  ll_handle   Lower layer context pointer
	 * \param[in]  me_id       Managed Entity identifier
	 * \param[out] enc         Supported encryption
	 */
	enum pon_adapter_errno (*encryption_get)(void *ll_handle,
					uint16_t me_id,
					enum pon_adapter_enh_sec_ctrl_enc *enc);

	/** Set supported encryption mode
	 *
	 * \param[in]  ll_handle   Lower layer context pointer
	 * \param[in]  me_id       Managed Entity identifier
	 * \param[out] enc         Supported encryption
	 */
	enum pon_adapter_errno (*encryption_set)(void *ll_handle,
				uint16_t me_id,
				const enum pon_adapter_enh_sec_ctrl_enc enc);

	/** Provide OLT random challenge
	 *
	 * \param[in]  ll_handle     Lower layer context pointer
	 * \param[in]  me_id         Managed Entity identifier
	 * \param[in]  olt_rand_chl  OLT random challenge
	 * \param[in]  len           Data size in bytes
	 */
	enum pon_adapter_errno (*olt_rand_chl_set)(void *ll_handle,
						   uint16_t me_id,
						   uint8_t *olt_rand_chl,
						   size_t len);

	/** Provide OLT authentication result
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[in]  olt_auth_result  OLT authentication result
	 * \param[in]  len              Data size in bytes
	 */
	enum pon_adapter_errno (*olt_auth_result_set)(void *ll_handle,
						uint16_t me_id,
						uint8_t *olt_auth_result,
						size_t len);

	/** Retrieve master session key name
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[out] msk              Master session key, need to be free by
	 *				the caller on success
	 * \param[in]  len              Data size in bytes
	 */
	enum pon_adapter_errno (*msk_get)(void *ll_handle,
					  uint16_t me_id,
					  uint8_t *msk,
					  size_t *len);

	/** Set authorization broadcast ONU key
	 *
	 * \param[in]  ll_handle    Lower layer context pointer
	 * \param[in]  me_id        Managed Entity identifier
	 * \param[in]  bc_key	    Broadcast key table entry
	 * \param[in]  len	    Broadcast key length
	 * \param[in]  index	    Broadcast key index (zero based integer
	 *			    with value 0 or 1)
	 */
	enum pon_adapter_errno (*bc_key_set)(void *ll_handle,
					uint16_t me_id,
					uint8_t *bc_key,
					size_t len,
					unsigned int index);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
