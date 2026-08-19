/*******************************************************************************
         Copyright © 2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*  ***************************************************************************** 
 *         File Name    : secdbg.c												*
 *         Description  : test applications for secure debug utility			*
 *						  using secure crypto services							*
 *  *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

#include "secdbg_test_app.h"

/*! Sample UUID for Secure crypto service Trusted agent */
#define	TA_SECURE_CRYPTO_UUID	{0xFFFF0000, 0x0001, 0x0002, \
		{0x0, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07}}

struct secdbg_context secdbg_ctx = {
	.uuid = TA_SECURE_CRYPTO_UUID,
	.context = {0},
	.session = 0,
	.login_type = TEEC_LOGIN_USER,
};

int secure_debug(uint16_t port_num, uint32_t sst_config, const uint8_t *kek_file,
		uint32_t aeskey_offset, const uint8_t *aes_wrapkey_file);

static int init_scsa_session(struct secdbg_context *ctx)
{
	TEEC_Result result = 0;
	uint32_t origin = 0;

	result = TEEC_InitializeContext(NULL, &ctx->context);
	if (result != TEEC_SUCCESS) {
		fprintf(stderr, "SCSA context initialization failed, ret:%d\n", (int)result);
		return result;
	}

	result = TEEC_OpenSession(&ctx->context, &ctx->session, &ctx->uuid,
			ctx->login_type, NULL, NULL, &origin);
	if (result != TEEC_SUCCESS) {
		fprintf(stderr, "SCSA open session failed ret:%d origin:%d\n", (int)result, (int)origin);
		return result;
	}
	return result;
}

static void exit_scsa_session(struct secdbg_context *ctx)
{
	TEEC_CloseSession(&ctx->session);
	TEEC_FinalizeContext(&ctx->context);
}

/* function to display example commands */
static void print_help(void)
{
	printf("secdbg -p <port num> -c <secure storage config> -k <wrap sd_kek file> -o <OTP asset number> [ -w <aes wrap key file> ]\n");
	printf("sample command:\n");
	printf("secdbg_test_app -p 6 -c 0x1000CF -k wrapped_sdkek.txt -o 2\n");
	printf("secdbg_test_app -p 2 -c 0x1000CF -k wrapped_sdkek.txt -o 2 -w aes_wrapkey.txt\n");
	printf("Note: wrapped_sdkek.txt and aes_wrapkey.txt should be in hexadecimal format\n");
    exit(0);
}

#define SECDBG_OPTIONS	"p:c:k:o:w:h"
static struct option secdbg_options[] = {
	{"port", required_argument, NULL, 'p'},
	{"config", required_argument, NULL, 'c'},
	{"sdkek_file", required_argument, NULL, 'k'},
	{"otp", required_argument, NULL, 'o'},
	{"aeswrapkey_file", required_argument, NULL, 'w'},
	{"help", required_argument, NULL, 'h'},
	{NULL, 0, NULL, 0},
};

/*
 * @brief  main function to secure debug utility
 * @return
 *  returns 0 on success and -1 on failure
 */
int main(int argc, char **argv)
{
	uint8_t aes_wrapkey_file[20] = {0};
	uint8_t kek_file[20] = {0};
	uint32_t sst_config = 0x0;
	uint32_t aeskey_offset = 0x0;
	uint16_t port_num = 0x0;
	int opt_index, opt;
	int ret = -1;

	if (argc == 1)
        print_help();

	while ((opt = getopt_long(argc, argv, SECDBG_OPTIONS, secdbg_options, &opt_index)) != -1) {
		switch (opt) {
			case 'c':
				if ((sst_config = (uint32_t)strtoul(optarg, NULL, 16)) == 0) {
					fprintf(stderr, "failed to convert object config\n");
					return -1;
				}
				break;
			case 'p':
				if ((port_num = (uint16_t)atol(optarg)) == 0) {
					fprintf(stderr, "failed to convert port number\n");
					return -1;
				}
				break;
			case 'k':
				strncpy((char *)kek_file, optarg, sizeof(kek_file) - 1);
				kek_file[sizeof(kek_file) - 1] = '\0';
				break;
			case 'o':
				if ((aeskey_offset = strtoul(optarg, NULL, 16)) == 0) {
					fprintf(stderr, "failed to convert aeskeyoffset\n");
					return -1;
				}
				break;
			case 'w':
				strncpy((char *)aes_wrapkey_file, optarg, sizeof(aes_wrapkey_file) - 1);
				aes_wrapkey_file[sizeof(aes_wrapkey_file) - 1] = '\0';
				break;
			case 'h':
			default:
				print_help();
				break;
		}
	}
	ret = strlen((char *)kek_file);
	if (ret <= 0) {
		fprintf(stderr, "Key Encryption Key input file name is invalid\n");
		return ret;
	}

	if (init_scsa_session(&secdbg_ctx) < 0)
		return ret;

	ret = strlen((char *)aes_wrapkey_file);
	if (ret <= 0) {
		printf("AES Encryption Key input file name is empty, continuing with RSA plaintext\n");
		ret = secure_debug(port_num, sst_config, kek_file, aeskey_offset, NULL);
	} else {
		ret = secure_debug(port_num, sst_config, kek_file, aeskey_offset, aes_wrapkey_file);
	}
	exit_scsa_session(&secdbg_ctx);
	return ret;
}

/*
 * @brief function to perform secure debug tests
 * @param
 * port_num	-	secure debug port number to unlocked
 * sst_config	-	secure storage object configuration
 * kek_file	-	wrapped secure debug KEK file name
 * aeskey_offset	-	AES wrapped key offset number in OTP
 * aes_wrapkey_file	-	External AES wrap key for RSA keyblob
 * @return
 * return 0 on success and -ve value on failure
 */
int secure_debug(uint16_t port_num, uint32_t sst_config, const uint8_t *kek_file,
		uint32_t aeskey_offset, const uint8_t *aes_wrapkey_file)
{
	uint8_t aes_wrapkey[AES_WRAP_KEY_LEN] = {0};
    uint8_t nonce[SECDBG_NONCE_LEN] = {0};
    uint8_t nonce_a[NONCE_BUF_LEN] = {0};
    uint8_t nonce_b[NONCE_BUF_LEN] = {0};
	struct rsa_signature *rsa_sign = NULL;
    int nonce_len = NONCE_BUF_LEN;
	uint32_t wrap_auth_key_len = 0;
	uint8_t *wrap_auth_key = NULL;
	uint8_t *auth_pub_mod = NULL;
	uint32_t asset_number = 0;
	int auth_pub_mod_len = 0;
	int ret = -1;

    printf("Secure Debug Unlocking Port : %u\n", port_num);

	ret = secdbg_unwrap_kek((const char *)kek_file, aeskey_offset, &asset_number);
	if (ret < 0) {
		fprintf(stderr, "failed to unwrap secure debug key\n");
		return -1;
	}

	ret = secdbg_get_wrapped_auth_key(port_num, sst_config, &wrap_auth_key,
		&wrap_auth_key_len);
	if (ret < 0) {
		if (aes_wrapkey_file) {
			ret = secdbg_read_aes_wrapkey_and_store(port_num, sst_config,
					aes_wrapkey_file, aes_wrapkey);
			if (ret < 0) {
				fprintf(stderr, "failed to read AES wrap key and store into secure storage\n");
				return ret;
			}
			ret = secdbg_gen_rsa_keypair_and_cryptoformatstore(port_num, sst_config,
					&auth_pub_mod, &auth_pub_mod_len, aes_wrapkey);
		} else {
			ret = secdbg_gen_rsa_keypair_and_cryptoformatstore(port_num,
					sst_config, &auth_pub_mod, &auth_pub_mod_len, NULL);
		}
		if (ret < 0) {
			fprintf(stderr, "RSA keypair generation and storing failed\n");
			return -1;
		}

		ret = secdbg_generate_wrapped_auth_key(port_num, sst_config, &asset_number,
				auth_pub_mod, auth_pub_mod_len, &wrap_auth_key, &wrap_auth_key_len);
		free(auth_pub_mod);
		if (ret < 0) {
			fprintf(stderr, "wrap authentication key generation failed\n");
			return -1;
		}
	}

	ret = secdbg_load_aes_wrap_key_to_tep(port_num, KEY_AES, sst_config);
	if ( ret < 0 && ret != -EAGAIN) {
		fprintf(stderr, "failed to load AES wrap key\n");
		goto wrap_auth_free;
	}

	if (aes_wrapkey_file)
		ret = secdbg_load_rsa_key_to_tep(port_num, KEY_PRIVATE, sst_config, 1);
	else
		ret = secdbg_load_rsa_key_to_tep(port_num, KEY_PRIVATE, sst_config, 0);
	if (ret < 0) {
		fprintf(stderr, "failed to load private key for signature generation\n");
		goto wrap_auth_free;
	}

	ret = securedebug_auth_init(port_num, nonce_b, (uint32_t*)&nonce_len,
			wrap_auth_key, wrap_auth_key_len);
	if (ret < 0) {
		fprintf(stderr, "Error in secure debug auth init ret:%d\n", ret);
		goto wrap_auth_free;
	}
	printf("securedebug_auth_init success\n");

	ret = secdbg_complete_nonce(nonce, nonce_a, nonce_b);
	if (ret < 0) {
		fprintf(stderr, "failed to get nonce data secure debug\n");
		goto wrap_auth_free;
	}
	nonce_len = ret;

    ret = secdbg_gen_crypto_sign(nonce, nonce_len, &rsa_sign);
	if ((ret < 0) || !rsa_sign) {
		fprintf(stderr, "signature generate failed\n");
		goto wrap_auth_free;
	}
    ret = securedebug_auth_verify(port_num, rsa_sign->rsa_signature.num_ptr,
			rsa_sign->rsa_signature.num_len, nonce_a, sizeof(nonce_a));
    if (ret < 0) {
		fprintf(stderr, "Error in secure debug auth verify ret:%d", ret);
		goto rsa_sign_free;
	}
	printf("securedebug_auth_verify success\n");

rsa_sign_free:
	free(rsa_sign);
wrap_auth_free:
	free(wrap_auth_key);
	free_sec_store_objects(sst_config, port_num);
	return ret;
}
