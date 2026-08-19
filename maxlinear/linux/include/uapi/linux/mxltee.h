/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/*****************************************************************************
 * Copyright © 2024 MaxLinear, Inc.
 *
 *****************************************************************************/
/**
 * File contains TEE commands supported by secure crypto service trusted agent.
 */

#ifndef _MXLTEE_H_
#define _MXLTEE_H_

// Cryptographic enums and structures
/**
 * enum key_types - key type
 */
enum key_types {
	KEY_AES,
	KEY_PUBLIC,
	KEY_PRIVATE,
	KEY_RSA,
};

/**
 * enum key_format - key format plain or wrapped
 */
enum key_format {
	KEY_WRAP_FORMAT = 1,
	KEY_PLAIN_FORMAT
};

/**
 * enum sec_alg - supported secure crypto algorithms
 */
enum sec_alg {
	SEC_ALG_RSA_2048,
	SEC_ALG_RSA_3072,
	SEC_ALG_RSA_4096,
	SEC_ALG_ECDSA_P256,
	SEC_ALG_ECDSA_P384,
	SEC_ALG_AES_WRAP_UNWRAP,
	SEC_ALG_MAX
};

/**
 * enum mac_alg - algorithms supported for MAC generation
 */
enum mac_alg {
	MAC_ALGO_HMAC_SHA1,
	MAC_ALGO_HMAC_SHA224,
	MAC_ALGO_HMAC_SHA256,
	MAC_ALGO_HMAC_SHA384,
	MAC_ALGO_HMAC_SHA512,
	MAC_ALGO_AES_CMAC,
	MAC_ALGO_AES_CBC_MAC,
	MAC_ALGO_POLY1305
};

/**
 * enum hash_flag - options for chunk based transfer
 */
enum hash_flag
{
	SC_FLAG_PARTIAL,
	SC_FLAG_FINAL
};

/**
 * enum gen_key_flags - options for key generation request
 */
enum gen_key_flags
{
	SEC_CRYPTO_RETURN_KEY_PAIR,
	SEC_CRYPTO_RETURN_KEY_PUBLIC_KEY,
	SEC_CRYPTO_RETURN_KEY_PAIR_PLAIN,
	SEC_CRYPTO_RETURN_KEY_NONE
};

/**
 * enum load_key_flags - options for key loading request
 */
enum load_key_flags {
	PRIVATE_KEY_BLOB = 1,
	PRIVATE_KEY_PLAINTEXT,
};

/**
 * enum wrap_flag - options for wrapping
 */
enum wrap_flag {
	WRAP_INPUT_BUFFER,
	WRAP_INPUT_SS_HANDLE,
	WRAP_KEY_SS_HANDLE,
	WRAP_KEY_BUFFER,
	WRAP_KEY_TEE_ASSET,
};

/**
 * enum unwrap_flag - options for unwrapping
 */
enum unwrap_flag {
	UNWRAP_INPUT_BUFFER = 0x5,
	UNWRAP_INPUT_SS_HANDLE,
	UNWRAP_OUTPUT_BUFFER,
	UNWRAP_OUTPUT_SS_HANDLE,
	UNWRAP_OUTPUT_TEE_ASSET,
	UNWRAP_KEY_BUFFER,
	UNWRAP_KEY_SS_HANDLE,
	UNWRAP_KEY_OTP
};

/**
 * enum sign_flag - options required for signature generation and verification
 */
enum sign_flag {
	RSA_PKCS1_5_SHA1,
	RSA_PKCS1_5_SHA256,
	RSA_PKCS1_5_SHA384,
	RSA_PKCS1_5_SHA512,
	RSA_PSS_SHA1,
	RSA_PSS_SHA256,
	RSA_PSS_SHA384,
	RSA_PSS_SHA512,
	ECDSA_ASN1_SHA256,
	ECDSA_ASN1_SHA384,
	HASH_MAX
};

/**
 * enum mac_flags - options for MAC generation
 */
enum mac_flags {
	MAC_KEY_SECURE_STORE,
	MAC_KEY_BUFFER,
	MAC_KEY_TEE_ASSET,
	MAC_KEY_OTP
};

/**
 * struct big_number -	represents big number
 * @number_len	: length of @number in bytes
 * @number	:	pointer to number
 */
struct big_number {
	int num_len;
	uint8_t *num_ptr;
};
typedef struct big_number big_number;

/**
 * struct ecdsa_signature - represents ECDSA signature
 * @r	:	
 * @s	:	
 */
struct ecdsa_signature {
	big_number r;
	big_number s;
};

/**
 * struct ec_public_key - represents ECDSA public key
 * ECDSA domain parameters
 * @modulus	:	modulus
 * @a	:	constant a
 * @b	:	constant b
 * @g_order	:	curve order
 * ECC base point of the curve
 * @Gx	:	x co-ordinate of G
 * @Gy	:	y co-ordinate of G
 * @Qx	:	x co-ordinate of Q
 * @Qy	:	y co-ordinate of Q
 */
struct ec_public_key {
	big_number modulus;
	big_number a;
	big_number b;
	big_number g_order;
	big_number Gx;
	big_number Gy;
	big_number Qx;
	big_number Qy;
};

/**
 * struct ecdsa_private_key	-	represents ECDSA private key
 * @private_key	:	private key
 */
struct ecdsa_private_key {
	big_number pri_key;
};

/**
 * struct ecdsa_key_pair - represents ECDSA key pair
 * @public	:	public key
 * @private	:	private key
 */
struct ecdsa_key_pair {
	struct ec_public_key pub_key;
	struct ecdsa_private_key pri_key;
};

/**
 * struct rsa_signature - represents RSA signature
 * @rsa_signature	:	RSA signature
 */
struct rsa_signature {
	big_number rsa_signature;
};

/**
 * struct key_meta_info - maintains wrapper information for key
 * @format	:	key format like plain or wrapped
 * @sec_algo	:	crypto algorithm
 * @total_key_len	:	total key len i.e lengths of public + private + modulus
 */
struct key_meta_info {
	enum key_format format;
	enum sec_alg sec_algo;
	uint32_t total_key_len;
};

/**
 * structure to store keys into secure storage
 * @key_len	:	size of @key
 * @key		:	buffer of length @key_len
 */
struct key_data {
	uint32_t key_len;
	uint32_t key[];
};

/**
 * struct rsa_key - RSA key format
 * @pub_exp	:	public key exponent
 * @pri_exp	:	private key exponent
 * @modulus	:	modulus
 */
struct rsa_key {
	struct big_number pub_exp;
	struct big_number pri_exp;
	struct big_number modulus;
};

/**
 * struct aes_key - AES key format used in for wrap operation
 * @key_len	:	size of @key
 * @key		:	buffer of length @key_len
 */
struct aes_key {
	uint32_t key_len;
	uint8_t key[];
};

enum MXLTEE_TA_SECURE_CRYPTO_CMD {
	/**
	 * TEE command to generate signature for plain data
	 * param0 - secure crypto algorithm
	 */
	TA_SECURE_CRYPTO_SIGN = 0x00,

	/**
	 * TEE command to verify the signature of signed data
	 * param0 - secure crypto algorithm
	 */
	TA_SECURE_CRYPTO_VERIFY = 0x01,

	/**
	 * TEE command to generate signature for hash digest
	 * param0	-	secure crypto algorithm
	 */
	TA_SECURE_CRYPTO_SIGN_DIGEST = 0x02,

	/**
	 * TEE command to verify signature of signed hash digest
	 * param0	-	secure crypto algorithm
	 */
	TA_SECURE_CRYPTO_VERIFY_DIGEST = 0x03,

	/**
	 * TEE command to load the key to secure crypto service
	 * param0 - Reference to secure crypto load key structure i.e struct seccrypto_load_key
	 */
	TA_SECURE_CRYPTO_LOAD_KEY = 0x04,

	/**
	 * TEE command to generate keypair
	 * param0 - Reference to secure crypto key generation structure i.e struct seccrypto_gen_key
	 */
	TA_SECURE_CRYPTO_GEN_KEYPAIR = 0x05,

	/**
	 * TEE command to wrap the private data
	 * param0	-	private data or wrap input
	 */
	TA_SECURE_CRYPTO_WRAP = 0x06,

	/**
	 * TEE command to unwrap the wrapped data
	 * param0	-	wrapped data
	 */
	TA_SECURE_CRYPTO_UNWRAP = 0x07,

	/**
	 * TEE command to generate MAC i.e Message Authentication Code
	 * param0	- MAC generation parameters
	 */
	TA_SECURE_CRYPTO_GEN_MAC_SINGLE = 0x08,
};

/**
 * struct secure_storage_params - represents secure storage information to store/load the key
 * @handle	:	64-bit secure storage handle of opened sst object
 * @access_perm	:	access permissions associated with sst object
 * @policy_attr	:	policy attributes for enforcing policy check
 * @crypto_mode_flag	:	crypto mode options like integrity, encryption etc...
 */
struct secure_storage_params {
	uint64_t handle;
	uint8_t access_perm;
	uint16_t policy_attr;
	uint16_t crypto_mode_flag;
};

/**
 * struct seccrypto_gen_key	- key generation parameters for TA_SECURE_CRYPTO_GEN_KEYPAIR tee command
 * @genkey_algo	:	algorithm to be used for key generation
 * @flag	:	flags to be passed for keygen
 * @sst_params	:	secure storage parameters like handle, permission etc...
 * @key_ptr	:	key context pointer for returning key context back to ATOM
 * @key_len	:	buffer length of @key_ptr
 */
struct seccrypto_gen_key {
	enum sec_alg genkey_algo;
	enum gen_key_flags flag;
	struct secure_storage_params sst_params;
	void *key_ptr;
	uint32_t key_len;
};

/**
 * struct seccrypto_load_key - key load parameters for TA_SECURE_CRYPTO_LOAD_KEY tee command
 * @sst_params	:	secure storage params like handle, policy etc...
 * @key_type	:	0-private key, 1-public key
 * @load_flags	:	load options like 0-keyblob, 1-plaintext
 * @load_algo	:	secure crypto algorithm to be used
 * @hash_algo	:	hash algorithm
 */
struct seccrypto_load_key {
	struct secure_storage_params sst_params;
	enum key_types key_type;
	enum load_key_flags load_flags;
	enum sec_alg load_algo;
	enum sign_flag hash_algo;
};

/**
 * struct seccrypto_sign_param -  signature generation parameters for TA_SECURE_CRYPTO_SIGN tee command
 * @data	:	data for which signature needs to generated
 * @data_len	:	length of @data_len
 * @sign_algo	:	algorithm to be used for signature generation
 * @hash_algo	:	hash algorithm to be used for signature generation
 * @hash_flags	:	flags to be used for signature generation
 * @signature	: signature of @data
 * @sign_len	:	length of @signature
 */
struct seccrypto_sign_param {
	uint8_t *data;
	uint32_t data_len;
	enum sec_alg sign_algo;
	enum sign_flag hash_algo;
	enum hash_flag hash_flags;
	uint8_t *signature;
	uint32_t sign_len;
};

/**
 * struct seccrypto_verify_param - signature verify parameters for TA_SECURE_CRYPTO_VERIFY tee command
 * @data	:	data to be verified by @signature
 * @data_len	:	length of @data
 * @signature	: signature of @data
 * @sign_len	:	length of @signature
 * @sign_algo	:	algorithm used while generating @signature
 * @hash_algo	:	hash algorithm to be used for signature verification
 * @hash_flags	:	flags used while generating @signature
 */
struct seccrypto_verify_param {
	uint8_t *data;
	uint32_t data_len;
	uint8_t *signature;
	uint32_t sign_len;
	enum sec_alg sign_algo;
	enum sign_flag hash_algo;
	enum hash_flag hash_flags;
};

/**
 * struct seccrypto_wrap_unwrap - wrap/unwrap parameters for TA_SECURE_CRYPTO_WRAP/UNWRAP tee command
 * @input	:	pointer to secret data to be wrapped/unwrapped
 * @output	:	pointer to wrapped/unwrapped data
 * @key	:	pointer to wrapping/unwrapping key or key information
 * @input_size	:	length of input data i.e @input
 * @output_size	:	length of output data i.e @output
 * @key_size	:	length of wrap key i.e @key
 * @flags	:	flags like INPUT_BUFFER, KEY_SS_HANDLE etc...
 */
struct seccrypto_wrap_unwrap {
	uint8_t *input;
	uint8_t *output;
	uint8_t *key;
	uint32_t input_size;
	uint32_t output_size;
	uint32_t key_size;
	uint32_t flags;
};

/**
 * struct seccrypto_mac_gen - MAC generation parameters for TA_SECURE_CRYPTO_GEN_MAC_SINGLE tee command
 * @mac_algo	:	MAC generation algorithms
 * @input		:	Input data to generate MAC
 * @output		:	MAC
 * @mac_key		:	Key used to generate MAC
 * @input_size	:	Input data size i.e size of @input
 * @output_size	:	MAC size i.e size of @output
 * @key_size	:	MAC Key size i.e size of @mac_key
 * @flags		:	Options for MAC generation
 */
struct seccrypto_mac_gen {
	enum mac_alg mac_algo;
	uint8_t *input;
	uint8_t *output;
	uint8_t *mac_key;
	uint32_t input_size;
	uint32_t output_size;
	uint32_t key_size;
	enum mac_flags flags;
};
#endif //_MXLTEE_H_
