/*******************************************************************************
         Copyright © 2022-2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*  *****************************************************************************
 *         File Name    : secdbg_test_app.h										*
 *         Description  : secure debug header declaration						*
 *  *****************************************************************************/

#include <stdio.h>
#include <stdint.h>

#ifndef __SECDBG_TOOL
#define __SECDBG_TOOL
#include "fapi_sec_service.h"
#include "fapi_sec_storage.h"
#include "fapi_sec_debug.h"

#ifdef MXLTEE
#include <tee_client_api.h>
#include <linux/mxltee.h>
#endif

#define MAX_SIGN_LEN	410
#define MAX_AUTHKEY_LEN	512
#define NONCE_BUF_LEN	16
#define SECDBG_NONCE_LEN	32

#ifdef MXLTEE
#define WRAP_BYTES_LEN		8
#define AES_WRAP_KEY_LEN    32
#define WRAPPED_SDKEK_LEN   (32 + WRAP_BYTES_LEN)
/*
 * secure crypto service TEE context for secure debug
 */
struct secdbg_context {
	TEEC_Context context;
	TEEC_Session session;
	uint32_t login_type;
	TEEC_UUID uuid;
};
#endif

/*
 * reads the wrapped auth key into auth_buf.
 */
int secdbg_get_wrap_authkey(uint8_t *puAuthBuf, int nPort);

/*
 * generates the random number of len bytes.
 */
int secdbg_get_random_number(void *pvBuf, int nLen);

/*
 * generates the signature data for noce data.
 */
int secdbg_gen_signature(uint8_t *pcSignBuf, uint32_t *puLen, int nPort);

/*
 * stores the authentication to secure storage
 */
int secdbg_store_authkey(sst_obj_config_t *pxSstConfig, int nPort,
		const uint8_t *puAuthKey, uint32_t nnAuthKeyLen);

/*
 * loads the authentication from secure storage
 */
int secdbg_load_authkey(sst_obj_config_t *pxSstConfig, int nPort,
		uint8_t *puAuthKey, uint32_t *puAuthKeyLen);

/*
 * formats the nonce A & B and stores to a binary file
 */
int secdbg_nonce_formatnstore(uint8_t *puNonceA, const uint8_t *puNonceB,
		int nNonceLen, int nPort);

#ifdef MXLTEE
/*
 * unwraps the secure debug key and loads to TEP dynamic asset.
 */
int secdbg_unwrap_kek(const char *pcFile, uint32_t nOffset, uint32_t *nAssetNumber);

/*
 * reads external AES wrap key from file(hex format) and stores into secure storage
 */
int secdbg_read_aes_wrapkey_and_store(uint16_t nPort, uint32_t nSstConfig,
	const uint8_t *pAESWrapKeyFile, uint8_t *pAESWrapKey);

/*
 * generates rsa keypair, stores to secure storage
 * object name with port number i.e port%d_rsa_keypair
 */
int secdbg_gen_rsa_keypair_and_cryptoformatstore(uint16_t nPort, uint32_t nSstConfig,
		uint8_t **pKey, int *pLen, const uint8_t *pAESWrapKey);

/*
 * loads RSA key to TEP asset store from secure storage
 */
int secdbg_load_rsa_key_to_tep(uint16_t nPort, enum key_types nType,
		uint32_t nSstConfig, uint8_t nRSAKeyBlob);

/*
 * loads AES Wrap key to TEP asset store from secure storage
 */
int secdbg_load_aes_wrap_key_to_tep(uint16_t nPort, enum key_types nType,
		uint32_t nSstConfig);

/*
 * Generates wrapped authentication key, will be stored to secure storage.
 * object name with port number i.e port%d_wrap_authkey
 */
int secdbg_generate_wrapped_auth_key(uint16_t nPort, uint32_t nSstConfig,
		uint32_t *nAssetNumber, uint8_t *pAuthKey, uint32_t nAuthKeyLen,
		uint8_t **pWrappedAuthKey, uint32_t *nWrapLen);

/*
 * checks whether wrapped authentication key secure storage object already exists
 */
int secdbg_wrap_authkey_exists(uint16_t nPort, uint32_t nSstConfig);

/*
 * reads wrapped authentication key from secure storage
 */
int secdbg_get_wrapped_auth_key(uint16_t nPort, uint32_t nSstConfig,
		uint8_t **pWrappedAuthKey, uint32_t *nWrapLen);

/*
 * generates nonce_a and appends it with nonce_b
 */
int secdbg_complete_nonce(uint8_t *puNonce, uint8_t *puNonceA, const uint8_t *puNonceB);

/*
 * generates rsa signature for nonce data
 */
int secdbg_gen_crypto_sign(uint8_t *puNonce, uint32_t nLen,
		struct rsa_signature **rsa_sign);
#endif

#ifdef SECDBG_CTEST
/*
 * deletes the authentication key from secure storage
 */
int secdbg_delete_authkey(sst_obj_config_t *pcxObjConfig, int nPort);
#endif
int free_sec_store_objects(uint32_t nSstConfig, int nPort);
#endif /* __SECDBG_TOOL */
