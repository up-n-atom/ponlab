/******************************************************************************
 *
 *  Copyright (c) 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_crypt.h
 *
 * This is the PON adapter header file for EPON cryptographic related
 * functions.
 */

#ifndef _PON_ADAPTER_CRYPT_H_
#define _PON_ADAPTER_CRYPT_H_

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 * @{
 */

/** \defgroup PON_ADAPTER_CRYPT cryptographic related functions
 *  @{
 */

/** EPON low-level crypto key operations */
struct epon_crypt_ops {
	/**
	 * \brief Set EPON downstream encryption key
	 *
	 * \param[in] ll_handle      Lower layer context pointer
	 * \param[in] link_index     LINK_INDEX to which key shall be set
	 * \param[in] index	     key of index to set (0 or 1)
	 * \param[in] key	     key content
	 * \param[in] key_size	     key size (up to 16 bytes for EPON)
	 */
	enum pon_adapter_errno (*ds_key_set)(void *ll_handle,
					     uint8_t link_index,
					     uint8_t key_index, uint8_t *key,
					     size_t key_size);

	/**
	 * \brief Set EPON upstream encryption key
	 *
	 * \param[in] ll_handle      Lower layer context pointer
	 * \param[in] link_index     LINK_INDEX to which key shall be set
	 * \param[in] index	     key of index to set (0 or 1)
	 * \param[in] key	     key content
	 * \param[in] key_size	     key size (up to 16 bytes for EPON)
	 */
	enum pon_adapter_errno (*us_key_set)(void *ll_handle,
					     uint8_t link_index,
					     uint8_t key_index, uint8_t *key,
					     size_t key_size);

	/**
	 * \brief Set EPON cryptographic configuration
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] link_index	LINK_INDEX to which key shall be set
	 * \param[in] enc_mode		Encryption mode (disable 0 or enable 1)
	 */
	enum pon_adapter_errno (*crypt_config)(void *ll_handle,
					       uint8_t link_index,
					       uint8_t enc_mode);
};

/** @} */ /* PON_ADAPTER_CRYPT */

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif
