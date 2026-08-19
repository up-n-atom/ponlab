/* SPDX-License-Identifier: GPL-2.0 */
/*****************************************************************************
 * Copyright © 2024 MaxLinear, Inc.
 *
 *****************************************************************************/

/**
 * File has definitions for communication parameters between TEP and ATOM.
 * These definitions must match the definitions on TEP side.
 */
#ifndef _TEP_INTERFACE_H_
#define _TEP_INTERFACE_H_

#include <uapi/linux/mxltee.h>
#include <private.h>

#define UUID_LENGTH		16

/*! ICC Commands from ATOM to TEP for a TEE session */
enum mxltee_req_type {
	ICC_CMD_ID_OPEN_SESSION	= 0x0,
	ICC_CMD_ID_INVOKE_CMD,
	ICC_CMD_ID_CLOSE_SESSION
};

/*! ICC command responses from TEP to ATOM for a TEP session */
enum mxltee_res_type {
	ICC_CMD_ID_OPEN_SESSION_REPLY = 0x80,
	ICC_CMD_ID_INVOKE_CMD_REPLY,
	ICC_CMD_ID_CLOSE_SESSION_REPLY
};

/**
 * struct open_session_param - TEE open session parameters
 * @scsa_uuid	:	Trusted agent uuid
 * @client_uuid	:	Client uuid
 */
struct open_session_param {
	uint8_t scsa_uuid[UUID_LENGTH];
	uint8_t client_uuid[UUID_LENGTH];
};

/**
 * struct close_session_param - TEE close session parameters.
 * @scsa_uuid		:	Trusted agent uuid
 * @client_uuid		:	Client uuid
 * @session_id		:	session identifier
 */
struct close_session_param {
	uint8_t scsa_uuid[UUID_LENGTH];
	uint8_t client_uuid[UUID_LENGTH];
	int32_t session_id;
};

/**
 * struct active_session_param - active TEE session parameters.
 * @scsa_uuid		:	Trusted agent uuid
 * @client_uuid		:	Client uuid
 * @session_id		:	session identifier
 * @sub_cmd			:	specifies the operation like TA_SECURE_CRYPTO_SIGN etc...
 */
struct active_session_param {
	uint8_t scsa_uuid[UUID_LENGTH];
	uint8_t client_uuid[UUID_LENGTH];
	int32_t session_id;
	uint8_t sub_cmd;
};

/**
 * struct secure_storage_params - represents secure storage information to store/load the key
 */
struct secure_storage_params_tep {
	uint64_t handle;
	uint32_t uid;
	uint32_t gid;
	uint8_t pname[TASK_COMM_LEN];
	uint8_t access_perm;
	uint16_t policy_attr;
	uint16_t crypto_mode_flag;
};

/**
 * struct seccrypto_gen_key	- key generation parameters for TA_SECURE_CRYPTO_GEN_KEYPAIR tee command
 * @genkey_algo	:	algorithm to be used for key generation
 * @flags	:	flags to be passed for keygen
 */
struct seccrypto_gen_key_tep {
	enum sec_alg genkey_algo;
	enum sign_flag hash_algo;
	enum gen_key_flags flag;
	struct secure_storage_params_tep sst_params;
	uint32_t key_ptr;
	uint32_t key_len;
} __attribute__ ((packed));

/**
 * struct seccrypto_load_key - key load parameters for TA_SECURE_CRYPTO_LOAD_KEY tee command
 * @sst_params	:	secure storage params like handle, policy etc...
 * @key_type	:	0-private key, 1-public key
 * @load_flags	:	load options like 0-keyblob, 1-plaintext
 * @load_algo	:	secure crypto algorithm to be used
 * @hash_algo	:	hash algorithm
 */
struct seccrypto_load_key_tep {
	struct secure_storage_params_tep sst_params;
	enum key_types key_type;
	enum load_key_flags load_flags;
	enum sec_alg load_algo;
	enum sign_flag hash_algo;
};

/**
 * struct seccrypto_sign_param_tep -  signature generation parameters for TA_SECURE_CRYPTO_SIGN tee command
 * @data	:	data for which signature needs to generated
 * @data_len	:	length of @data_len
 * @sign_algo	:	algorithm to be used for signature generation
 * @hash_flags	:	flags to be used for signature generation
 */
struct seccrypto_sign_param_tep {
	u32 data_ptr;
	u32 data_len;
	enum sec_alg sign_algo;
	enum sign_flag hash_algo;
	enum hash_flag hash_flags;
	u32 sign_ptr;
	u32 sign_len;
};

/**
 * struct seccrypto_verify_param_tep - signature verify parameters for TA_SECURE_CRYPTO_VERIFY tee command
 * @data	:	data to be verified by @signature
 * @data_len	:	length of @data
 * @signature	: signature of @data
 * @sign_len	:	length of @signature
 * @sign_algo	:	algorithm used while generating @signature
 * @hash_flags	:	flags used while generating @signature
 */
struct seccrypto_verify_param_tep {
	u32 data_ptr;
	u32 data_len;
	u32 sign_ptr;
	u32 sign_len;
	enum sec_alg sign_algo;
	enum sign_flag hash_algo;
	enum hash_flag hash_flags;
};

/**
 * struct seccrypto_sign_digest_param - signature generation parameters for TA_SECURE_CRYPTO_SIGN_DIGEST tee command
 * @digest	:	hash digest input for signature generation
 * @digest_len	:	length of @digest
 * @sign_algo	:	algorithm to be used for signature generation
 * @sign_flags	:	flags to be used for signature generation
 */
struct seccrypto_sign_digest_param {
	uint8_t *digest;
	uint32_t digest_len;
	enum sec_alg sign_algo;
	uint8_t sign_flags;
};

/**
 * struct seccrypto_verify_digest_param - signature verification paramters for TA_SECURE_CRYPTO_DIGEST_VERIFY tee command
 * @digest	:	hash digest to verified by @signature
 * @digest_len	:	length of @digest
 * @signature	:	signature of @digest
 * @sign_len	:	length of @signature
 * @sign_algo	:	signature algorithm used while signing hash diest
 * @sign_flags	:	signature flags used while sigining hash digest
 */
struct seccrypto_verify_digest_param {
	uint8_t *digest;
	uint32_t digest_len;
	uint8_t *signature;
	uint32_t sign_len;
	enum sec_alg sign_algo;
	uint8_t sign_flags;
};

/**
 * struct seccrypto_wrap_unwrap_tep - wrap/unwrap parameters for TA_SECURE_CRYPTO_WRAP/UNWRAP tee command
 * @input	:	pointer to secret data to be wrapped/unwrapped
 * @output	:	pointer to wrapped/unwrapped data
 * @key	:	pointer to wrapping/unwrapping key or key information
 * @input_size	:	length of input data i.e @input
 * @output_size	:	length of output data i.e @output
 * @key_size	:	length of key i.e @key
 * @flags	:	wrap/unwrap flags like INPUT_BUFFER, KEY_SS_HANDLE etc...
 */
struct seccrypto_wrap_unwrap_tep {
	u32 input;
	u32 output;
	u32 key;
	s32 input_size;
	s32 output_size;
	s32 key_size;
	u32 flags;
};

/**
 * struct seccrypto_mac_gen_tep - MAC generation parameters for TA_SECURE_CRYPTO_GEN_MAC_SINGLE tee command
 * @mac_algo	:	MAC generation algorithms
 * @input		:	Input data to generate MAC
 * @output		:	MAC
 * @mac_key		:	Key used to generate MAC
 * @input_size	:	Input data size i.e size of @input
 * @output_size	:	MAC size i.e size of @output
 * @key_size	:	MAC Key size i.e size of @mac_key
 * @flags		:	Options for MAC generation
 */
struct seccrypto_mac_gen_tep {
	enum mac_alg mac_algo;
	u32 input;
	u32 output;
	u32 mac_key;
	u32 input_size;
	u32 output_size;
	u32 key_size;
	enum mac_flags flags;
};

/**
 * struct big_number -	represents big number
 * @number_len	: length of @number in bytes
 * @number	:	pointer to number
 */
struct big_number_tep {
	int num_len;
	uint32_t num_ptr;
};
typedef struct big_number_tep big_number_tep;

/**
 * struct ecdsa_signature - represents ECDSA signature
 * @r	:
 * @s	:
 */
struct ecdsa_signature_tep {
	big_number_tep r;
	big_number_tep s;
};

/**
 * struct ec_public_key - represents ECDSA public key
 * ECDSA domain parameters
 * @modulus	:
 * @a		:
 * @b
 * @g_order	:
 * ECC base point of the curve
 * @x_coordinate_G	:
 * @y_coordinate_G	:
 * @x_coordinate_Q	:
 * @y_coordinate_Q	:
 */
struct ec_public_key_tep {
	big_number_tep modulus;
	big_number_tep a;
	big_number_tep b;
	big_number_tep g_order;
	big_number_tep Gx;
	big_number_tep Gy;
	big_number_tep Qx;
	big_number_tep Qy;
};

/**
 * struct ecdsa_private_key	-	represents ECDSA private key
 * @private_key	:	private key
 */
struct ecdsa_private_key_tep {
	big_number_tep pri_key;
};

/**
 * struct ecdsa_key_pair - represents ECDSA key pair
 * @public	:	public key
 * @private	:	private key
 */
struct ecdsa_key_pair_tep {
	struct ec_public_key_tep pub_key;
	struct ecdsa_private_key_tep pri_key;
};

/**
 * struct rsa_signature - represents RSA signature
 * @rsa_signature	:	RSA signature
 */
struct rsa_signature_tep {
	big_number_tep rsa_signature;
};

/**
 * struct rsa_public_key - represents RSA public key
 * @n	:
 * @e	:
 */
struct rsa_key_tep {
	big_number_tep n;
	big_number_tep e;
	big_number_tep m;
};

/**
 * fill_secure_storage_params() - Fills uid, pid and process name for secure storage details
 */
void fill_secure_storage_params(struct secure_storage_params_tep *params);

/**
 * icc_write_and_read() - writes icc message and reads response
 */
int icc_write_and_read(icc_msg_t *icc_msg);

/**
 * validate_icc_reply() - validates response message against return value and session id
 */
int validate_icc_reply(icc_msg_t *icc_msg, uint32_t session_id);

/**
 * get_active_scsa_session() - function to get active session params for icc communication
 */
struct active_session_param *get_active_scsa_session(struct mxltee_driver *drv,
		struct mxltee_session *session, uint32_t sub_command, dma_addr_t *dma_active_session);

/**
 * handle_keyload_command() - communicate key loading command with TEP
 */
int handle_keyload_command(struct mxltee_driver *drv, struct mxltee_session *session,
		uint32_t num_params, struct tee_param *param);

/**
 * handle_keygen_command() - communicate key generation command with TEP
 */
int handle_keygen_command(struct mxltee_driver *drv, struct mxltee_session *session,
		uint32_t num_params, struct tee_param *param);

/**
 * get_hash_algo() - get supported hash algorithm for crypto algorithm
 */
int get_hash_algo(enum sec_alg algo);

/**
 * handle_sign_command() - communicate signature generation command with TEP
 */
s32 handle_sign_command(struct mxltee_driver *drv, struct mxltee_session *session,
		u32 num_params, struct tee_param *param);

/**
 * handle_verify_command() - communicate signature verify command with TEP
 */
s32 handle_verify_command(struct mxltee_driver *drv, struct mxltee_session *session,
		u32 num_params, struct tee_param *param);

/* handle_wrap_command() - communicate wrap command with TEP */
s32 handle_wrap_command(struct mxltee_driver *drv, struct mxltee_session *session,
		u32 num_params, struct tee_param *param);

/* handle_unwrap_command() - communicate un-wrap command with TEP */
s32 handle_unwrap_command(struct mxltee_driver *drv, struct mxltee_session *session,
		u32 num_params, struct tee_param *param);

/* handle_macgen_command() - communicate MAC_GEN command with TEP */
s32 handle_macgen_command(struct mxltee_driver *drv, struct mxltee_session *session,
		u32 num_params, struct tee_param *param);
#endif //_TEP_INTERFACE_H_
