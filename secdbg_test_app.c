/*******************************************************************************
         Copyright © 2022-2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*  *****************************************************************************
 *         File Name    : secdbg_test_app.c										*
 *         Description  : test application to test secure debug APIs			*
 *  *****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <openssl/rsa.h>
#include <openssl/aes.h>
#include <arpa/inet.h>

#include "secdbg_test_app.h"

#include "safe_str_lib.h"
#include "safe_lib.h"
#include "safe_mem_lib.h"

#define PRINT(fmt, args...)										\
	do {														\
		printf("%s- "fmt"\n", __func__, ##args);				\
	} while (0)

#define STR_TO_HEX(str, hex)									\
	do {														\
		char *endptr = NULL;									\
		hex = (uint8_t)strtol((const char*)str, &endptr, 16);	\
		if (!endptr) {											\
			PRINT("invalid hex integer");						\
			break;												\
		}														\
	} while (0)

/* Max secure storage object name length */
#define MAX_OBJ_NAME_LEN	32

#define WRAP_AUTHKEY_PATH	"/tmp/port%d_wrap_authkey.txt"
#define RANDOM_NONCE_PATH	"/tmp/port%d_RaRb.bin"
#define SIGNATURE_PATH		"/tmp/port%d_sign.sign"
#define GEN_SIGNATURE_CMD	"openssl dgst -sha256 -sign /tmp/port%d_prikey.pem -out /tmp/port%d_sign.sign /tmp/port%d_RaRb.bin"

#define INVALID_PORT(nPort)										\
		(((nPort < SDBG_RESERVED_0_PORT) || (nPort > SDBG_RESERVED_7_PORT)) ? true : false)

#ifdef MXLTEE
#define RSA_PUBLIC_KEY_LEN	8
#define MAX_WRAP_AUTHKEY_LEN	512
#define RSA_KEY_OBJECT_NAME	"port%d_rsa_keypair"
#define AES_WRAPKEY_OBJECT_NAME	"port%d_aes_wrapkey"
#define WRAPPED_AUTHKEY_OBJECT_NAME	"port%d_wrap_authkey"
#define DWORD_ALIGN(x)	do {				\
		uintptr_t addr = (uintptr_t)x;		\
		x = (void*) ((addr + 3) & ~3);		\
	} while(0)
extern struct secdbg_context secdbg_ctx;
#endif

static char sstDebugPortName[][MAX_OBJ_NAME_LEN] = {
		"secure_tep_core", "pon_uart_port", "atom_uart_port", "arc_jtag_port",
		"spi_debug_port", "normal_tep_core", "console_uart_port", "other_debug_port"
	};

/*
 * /dev/urandom file descriptor.
 */
static int nRandomFd = -1;

int secdbg_get_wrap_authkey(uint8_t *puAuthBuf, int nPort)
{
	FILE *fp = NULL;
	char cPath[sizeof(WRAP_AUTHKEY_PATH) + 1] = {0};
	int nRet = -1;
	int nLen = 0;
	uint8_t nHex = 0;
	uint8_t nHexStr[3] = {0};

	if ((!puAuthBuf) || INVALID_PORT(nPort)) {
		PRINT("invalid parameters");
		return -EINVAL;
	}

	if (sprintf_s(cPath, sizeof(WRAP_AUTHKEY_PATH), WRAP_AUTHKEY_PATH,
				nPort) < 0) {
		PRINT("failed to fill path buffer");
		return -EFAULT;
	}
try_open:
	if (fopen_s(&fp, cPath, "r") != 0) {
		PRINT("failed to open %s file", cPath);
		return -EFAULT;
	}
	if (!fp) {
		if (errno == EINTR) {
			goto try_open;
		}
		PRINT("failed to open wrap_authkey.txt - %s", strerror(errno));
		return -EFAULT;
	}

	while (!feof(fp)) {
try_read:
		nRet = fread(&nHexStr, sizeof(uint16_t), 1, fp);
		if ((nRet <= 0) && (!feof(fp))) {
			if (errno == EINTR) {
				goto try_read;
			}
			PRINT("failed to read - %s", strerror(errno));
			fclose(fp);
			return nRet;
		}
		if ((nHexStr[0] == '\n') || (nHexStr[0] == '\0')) {
			break;
		}
		STR_TO_HEX(nHexStr, nHex);
		(*puAuthBuf) = nHex;
		puAuthBuf ++;
		nLen ++;
		memset(nHexStr, 0x0, sizeof(nHexStr));
	}
	fclose(fp);
	return nLen;
}

int secdbg_get_random_number(void *pvBuf, int nLen)
{
	int nRet = -1;

	if ((!pvBuf) || (nLen < 0)) {
		PRINT("invalid parameters");
		return nRet;
	}

try_open:
	if (nRandomFd < 0) {
		nRandomFd = open("/dev/urandom", O_RDONLY);
		if (nRandomFd < 0) {
			if (errno == EINTR) {
				goto try_open;
			}
			PRINT("unable to open [/dev/urandom] - %s", strerror(errno));
			return nRet;
		}
	}

try_read:
	nRet = read(nRandomFd, pvBuf, nLen);
	if (nRet < 0) {
		if (errno == EINTR) {
			goto try_read;
		}
		PRINT("failed to read [/dev/urandom] - %s", strerror(errno));
		return nRet;
	}
	return 0;
}

static int secdbg_nonceAB_to_file(const uint8_t *pcNonceB, uint32_t nnNonceLen,
		int nPort)
{
	FILE *fp = NULL;
	char cPath[sizeof(RANDOM_NONCE_PATH) + 1] = {0};
	int nRet = -1;

	if ((!pcNonceB) || INVALID_PORT(nPort) || (nnNonceLen != SECDBG_NONCE_LEN)) {
		PRINT("invalid parameters");
		return -EINVAL;
	}

	if (sprintf_s(cPath, sizeof(RANDOM_NONCE_PATH), RANDOM_NONCE_PATH,
				nPort) < 0) {
		PRINT("failed to fill path buffer");
		return -EFAULT;
	}

try_open:
	if (fopen_s(&fp, cPath, "w") != 0) {
		PRINT("failed to open %s file", cPath);
		return -EFAULT;
	}
	if (!fp) {
		if (errno == EINTR) {
			goto try_open;
		}
		PRINT("failed to open RaRb.bin - %s", strerror(errno));
		return -1;
	}
try_write:
	nRet = fwrite(pcNonceB, nnNonceLen, 1, fp);
	if (nRet < 0) {
		if (errno == EINTR) {
			goto try_write;
		}
		PRINT("failed write data - %s", strerror(errno));
	}
	fclose(fp);
	sync();
	return nRet;
}

int secdbg_gen_signature(uint8_t *puSignBuf, uint32_t *puLen, int nPort)
{
	char cSslCmd[sizeof(GEN_SIGNATURE_CMD) + 1] = {0};
	FILE *pfSign = NULL;
	int nRet = -1;

	if ((!puSignBuf) || (!puLen) || INVALID_PORT(nPort)) {
		PRINT("invalid parameters");
		return -EINVAL;
	}

	if (sprintf_s(cSslCmd, sizeof(GEN_SIGNATURE_CMD), GEN_SIGNATURE_CMD, nPort,
				nPort, nPort) < 0) {
		PRINT("failed to fill command buffer");
		return -EFAULT;
	}

	nRet = system(cSslCmd);
	if (nRet != 0) {
		PRINT("failed to execute %s command", cSslCmd);
		return -EFAULT;
	}
	sync();

	memset(cSslCmd, 0x0, sizeof(cSslCmd));
	if (sprintf_s(cSslCmd, sizeof(SIGNATURE_PATH), SIGNATURE_PATH, nPort) < 0) {
		PRINT("failed to fill path buffer");
		return -EFAULT;
	}

try_open:
	if (fopen_s(&pfSign, cSslCmd, "r") != 0) {
		PRINT("failed to open %s file", cSslCmd);
		return -EFAULT;
	}
	if (!pfSign) {
		if (errno == EINTR) {
			goto try_open;
		}
		PRINT("failed to open %s - %s", cSslCmd, strerror(errno));
		return -1;
	}
	*puLen = 0;
	while (!feof(pfSign)) {
try_read:
		nRet = fread(puSignBuf + (*puLen), sizeof(uint8_t), 1, pfSign);
		if ((nRet <= 0) && (!feof(pfSign))) {
			if (errno == EINTR) {
				goto try_read;
			}
			PRINT("failed to read - %s", strerror(errno));
			fclose(pfSign);
			return nRet;
		}
		(*puLen) ++;
	}
	*(puSignBuf + (*puLen)) = '\0';
	(*puLen) --;
	fclose(pfSign);
	return 0;
}

int secdbg_store_authkey(sst_obj_config_t *pxSstConfig, int nPort,
		const uint8_t *puAuthKey, uint32_t nnAuthKeyLen)
{
	sshandle_t xSsHandle = 0;
	int nRet = -1;

	if ((!pxSstConfig) || INVALID_PORT(nPort) || (!puAuthKey) ||
			(nnAuthKeyLen > MAX_AUTHKEY_LEN)) {
		PRINT("invalid parameters");
		return nRet;
	}

	nRet = securestore_create_open(sstDebugPortName[nPort], pxSstConfig,
            SS_CREATE, &xSsHandle);
	if (nRet < 0) {
		PRINT("failed to create/open obj- error: %d", nRet);
		return nRet;
	}

	nRet = securestore_save(xSsHandle, NULL, (uint8_t*)puAuthKey, nnAuthKeyLen);
	if (nRet < 0) {
		PRINT("securestore_save failed nRet:%d", nRet);
		securestore_close(xSsHandle);
		return nRet;
	}
	securestore_close(xSsHandle);
	return 0;
}

int secdbg_load_authkey(sst_obj_config_t *pxSstConfig, int nPort,
		uint8_t *puAuthKey, uint32_t *puAuthKeyLen)
{
	sshandle_t xSsHandle = 0;
	int nRet = -1;

	if (INVALID_PORT(nPort) || (!puAuthKey) || (!puAuthKeyLen)) {
		PRINT("invalid parameters");
		return nRet;
	}

	*puAuthKeyLen = 0;
	nRet = securestore_create_open(sstDebugPortName[nPort], pxSstConfig,
			0, &xSsHandle);
	if (nRet < 0) {
		PRINT("failed to create/open obj- error: %d", nRet);
		return nRet;
	}

	nRet = securestore_retrieve(xSsHandle, NULL, puAuthKey, (uint32_t)MAX_AUTHKEY_LEN,
			puAuthKeyLen);
	if (nRet < 0) {
		PRINT("securestore_save failed nRet:%d", nRet);
		securestore_close(xSsHandle);
		return nRet;
	}

	securestore_close(xSsHandle);
	return 0;
}

int secdbg_nonce_formatnstore(uint8_t *puNonceA, const uint8_t *puNonceB,
		int nNonceLen, int nPort)
{
	uint8_t aNonce[SECDBG_NONCE_LEN] = {0};
	int idx = 0;
	int nCount = -1;
	int nRet = -1;

	if ((!puNonceB) || (!puNonceA) || INVALID_PORT(nPort) ||
			(nNonceLen != NONCE_BUF_LEN)) {
		PRINT("invalid parameters");
		return nRet;
	}

	nRet = secdbg_get_random_number(puNonceA, NONCE_BUF_LEN);
	if (nRet < 0) {
		PRINT("failed to get random number");
		return nRet;
	}

	for (idx = 0; idx < NONCE_BUF_LEN; idx++) {
		aNonce[++nCount] = puNonceA[idx];
	}
	for (idx = 0; idx < NONCE_BUF_LEN; idx++) {
		aNonce[++nCount] = puNonceB[idx];
	}

	nRet = secdbg_nonceAB_to_file(aNonce, SECDBG_NONCE_LEN, nPort);
	if (nRet < 0) {
		PRINT("failed to store nonce to a file");
		return nRet;
	}

	return nRet;
}

#ifdef MXLTEE
static int secdbg_read_hexfile_to_bytes(const char *pcFile, uint8_t *pBuf, int nBufLen)
{
	uint8_t nHexStr[3] = {0};
	uint8_t nHex = 0;
	FILE *fp = NULL;
	int nRet = -1;
	int nLen = 0;

	if (fopen_s(&fp, pcFile, "r") != 0) {
		PRINT("failed to open %s file", pcFile);
		return -EFAULT;
	}
	if (!fp) {
		PRINT("failed to open %s - %s", pcFile, strerror(errno));
		return -EFAULT;
	}

	while (!feof(fp)) {
		nRet = fread(&nHexStr, sizeof(uint16_t), 1, fp);
		if ((nRet <= 0) && (!feof(fp))) {
			PRINT("failed to read - %s", strerror(errno));
			fclose(fp);
			return nRet;
		}
		if ((nHexStr[0] == '\n') || (nHexStr[0] == '\0')) {
			break;
		}
		STR_TO_HEX(nHexStr, nHex);
		pBuf[nLen ++] = nHex;
		memset(nHexStr, 0x0, sizeof(nHexStr));
		if (nLen > nBufLen)
			break;
	}
	fclose(fp);
	return nLen;
}

int secdbg_unwrap_kek(const char *pcFile, uint32_t nOffset, uint32_t *nAssetNumber)
{
	uint8_t nUnwrapInput[WRAPPED_SDKEK_LEN + 1] = {0};
	struct seccrypto_wrap_unwrap unwrap = {0};
	TEEC_Operation op ={0};
	uint32_t origin = 0;
	uint32_t nInputSize;
	TEEC_Result result;
	struct stat st = {0};

	if (stat(pcFile, &st) == 0) {
		if (st.st_size != WRAPPED_SDKEK_LEN*2) {
			PRINT("wrapped SD_KEK number of chars expected:%d actual:%ld", WRAPPED_SDKEK_LEN*2, st.st_size);
			return -1;
		}
	} else {
		PRINT("stat failed for %s - %s\n", pcFile, strerror(errno));
		return -1;
	}

	nInputSize = secdbg_read_hexfile_to_bytes(pcFile, nUnwrapInput, WRAPPED_SDKEK_LEN);
	if (nInputSize != WRAPPED_SDKEK_LEN) {
		PRINT("wrapped SD_KEK expected:%u actual:%u", WRAPPED_SDKEK_LEN, nInputSize);
		return -1;
	}

	unwrap.input = nUnwrapInput;
	unwrap.input_size = nInputSize;
	unwrap.flags |= (1 << UNWRAP_INPUT_BUFFER);

	unwrap.key = (uint8_t *)&nOffset;
	unwrap.key_size = sizeof(nOffset);
	unwrap.flags |= (1 << UNWRAP_KEY_OTP);

	unwrap.output = (uint8_t *)nAssetNumber;
	unwrap.output_size = sizeof(*nAssetNumber);
	unwrap.flags |= (1 << UNWRAP_OUTPUT_TEE_ASSET);

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	op.params[0].tmpref.buffer = &unwrap;
	op.params[0].tmpref.size = sizeof(unwrap);

	result = TEEC_InvokeCommand(&secdbg_ctx.session, TA_SECURE_CRYPTO_UNWRAP, &op, &origin);
	if (result != TEEC_SUCCESS) {
		PRINT("unwrap operation failed ret:%u origin:%u", (uint32_t)result, (uint32_t)origin);
		return -1;
	}
	return 0;
}

static int generate_rsa_key_pair(struct rsa_key *rsa_key, enum sec_alg sec_algo)
{
	BIGNUM *exp = NULL;
	RSA *rsa = NULL;
	int bits = 0;

	exp = BN_new();
	if (!exp)
		return -1;
	BN_set_word(exp, RSA_F4);

	switch (sec_algo) {
		case SEC_ALG_RSA_2048:
			bits = 2048;
			break;
		case SEC_ALG_RSA_3072:
			bits = 3072;
			break;
		case SEC_ALG_RSA_4096:
			bits = 4096;
			break;
		default:
			bits = 2048;
			break;
	}

	rsa = RSA_new();
	if (!RSA_generate_key_ex(rsa, bits, exp, NULL))
		goto free;

	rsa_key->pub_exp.num_len = sizeof(uint32_t);
	rsa_key->pub_exp.num_ptr = malloc(rsa_key->pub_exp.num_len);
	if (!rsa_key->pub_exp.num_ptr)
		goto free;
	BN_bn2bin(RSA_get0_e(rsa), rsa_key->pub_exp.num_ptr);

	rsa_key->pri_exp.num_len = BN_num_bytes(RSA_get0_d(rsa));
	rsa_key->pri_exp.num_ptr = malloc(rsa_key->pri_exp.num_len);
	if (!rsa_key->pri_exp.num_ptr)
		goto free;
	BN_bn2bin(RSA_get0_d(rsa), rsa_key->pri_exp.num_ptr);

	rsa_key->modulus.num_len = BN_num_bytes(RSA_get0_n(rsa));
	rsa_key->modulus.num_ptr = malloc(rsa_key->modulus.num_len);
	if (!rsa_key->modulus.num_ptr)
		goto free;
	BN_bn2bin(RSA_get0_n(rsa), rsa_key->modulus.num_ptr);

	BN_free(exp);
	RSA_free(rsa);
	return 0;

free:
	if (rsa_key->pub_exp.num_ptr)
		free(rsa_key->pub_exp.num_ptr);
	if (rsa_key->pri_exp.num_ptr)
		free(rsa_key->pri_exp.num_ptr);
	if (rsa)
		RSA_free(rsa);
	if (exp)
		BN_free(exp);
	return -1;
}

static int wrap_rsa_private_exp(struct big_number *pri_exp,
		const uint8_t *aes_wrapkey, uint8_t *dst)
{
	AES_KEY wrap_key = {0};
	int ret;

	AES_set_encrypt_key(aes_wrapkey, (AES_WRAP_KEY_LEN * 8), &wrap_key);
	ret = AES_wrap_key(&wrap_key, NULL, dst, pri_exp->num_ptr, pri_exp->num_len);
	if (ret <= 0)
		return -1;
	pri_exp->num_len = ret;
	return 0;
}

static int rsa_key_generate_and_store(enum sec_alg algo, int pub_key_len,
		sshandle_t handle, struct rsa_key *rsa_key, const uint8_t *aes_wrapkey)
{
	struct key_meta_info *meta = NULL;
	uint8_t *buffer = NULL;
	uint8_t dummy_buf[10];
	uint32_t cont_len = 0;
	size_t buf_len = 0;
	char yesno;
	int i = 0;

	/* Check data exists or not */
	if (securestore_retrieve(handle, NULL, dummy_buf, sizeof(dummy_buf), &cont_len)) {
		if (cont_len > 0) {
			printf("Key information exists, want to overwrite y/n - ");
			fflush(stdout);
			scanf("%c", &yesno);
			printf("\n");
			if ((yesno == 'n') || (yesno == 'N'))
				return 0;
		}
	}

	generate_rsa_key_pair(rsa_key, algo);
	rsa_key->pub_exp.num_len = pub_key_len;

	buf_len = (sizeof(*meta) + (sizeof(uint32_t) * 3) + rsa_key->pub_exp.num_len
			+ rsa_key->pri_exp.num_len + rsa_key->modulus.num_len + 12);
	if (aes_wrapkey)
		buf_len += WRAP_BYTES_LEN;
	meta = malloc(buf_len);
	if (!meta)
		return -1;
	meta->format = aes_wrapkey ? KEY_WRAP_FORMAT : KEY_PLAIN_FORMAT;
	meta->sec_algo = algo;
	meta->total_key_len = (rsa_key->pub_exp.num_len + rsa_key->pri_exp.num_len
			+ rsa_key->modulus.num_len);
	buffer = (uint8_t*) (((uint8_t*)meta) + sizeof(*meta));
	memcpy(buffer, &rsa_key->pub_exp.num_len, sizeof(uint32_t));
	buffer += sizeof(uint32_t);
	if (pub_key_len == 4) {
		for (i = 0; i < rsa_key->pub_exp.num_len; i++)
			*(buffer + i) = *(rsa_key->pub_exp.num_ptr + rsa_key->pub_exp.num_len - i - 1);
	} else {
		memcpy(buffer, rsa_key->pub_exp.num_ptr, rsa_key->pub_exp.num_len);
	}
	buffer += rsa_key->pub_exp.num_len;
	DWORD_ALIGN(buffer);
	if (aes_wrapkey) {
		if (wrap_rsa_private_exp(&rsa_key->pri_exp, aes_wrapkey,
					buffer + sizeof(uint32_t)) < 0) {
			free(meta);
			return -1;
		}
	} else {
		memcpy(buffer + sizeof(uint32_t), rsa_key->pri_exp.num_ptr, rsa_key->pri_exp.num_len);
	}
	memcpy(buffer, &rsa_key->pri_exp.num_len, sizeof(uint32_t));
	buffer += sizeof(uint32_t);
	buffer += rsa_key->pri_exp.num_len;
	DWORD_ALIGN(buffer);
	memcpy(buffer, &rsa_key->modulus.num_len, sizeof(uint32_t));
	buffer += sizeof(uint32_t);
	memcpy(buffer, rsa_key->modulus.num_ptr, rsa_key->modulus.num_len);

	if (securestore_save(handle, NULL, (const unsigned char *)meta, buf_len) < 0) {
		free(meta);
		return -1;
	}
	free(meta);
	return 0;
}

static inline void update_sst_config(uint32_t nSstConfig, sst_obj_config_t *sSstConfig)
{
	sSstConfig->access_perm.u.perms = nSstConfig & 0xFF;
	sSstConfig->policy_attr.u.attr = (nSstConfig >> 8) & 0xFFF;
	sSstConfig->crypto_mode_flag = (sst_crypto_mode_flag_t)(nSstConfig >> 20) & 0xFF;
}

static uint32_t generate_secdbg_authinfo(uint16_t nPort)
{
	uint32_t nAuthInfo = 0x64060000; /* Fixed value for secure debug info */
	int nReservedBits = 8;		 /* Future reserved bits */

	nAuthInfo |= (1 << (nReservedBits + nPort));
	return nAuthInfo;
}

int secdbg_read_aes_wrapkey_and_store(uint16_t nPort, uint32_t nSstConfig,
	const uint8_t *pAESWrapKeyFile, uint8_t *pAESWrapKey)
{
	char pcObjectName[MAX_OBJ_NAME_LEN] = {0};
	struct key_meta_info *pKeyMeta = NULL;
	struct aes_key *pAESKey = NULL;
	sst_obj_config_t sSstConfig = {0};
	sshandle_t sSstHandle = -1;
	int nRet = -1;
	struct stat st = {0};

	if (!pAESWrapKeyFile || !pAESWrapKey) {
		PRINT("invalid parameters");
		return nRet;
	}

	update_sst_config(nSstConfig, &sSstConfig);
	snprintf(pcObjectName, MAX_OBJ_NAME_LEN, AES_WRAPKEY_OBJECT_NAME, nPort);
	nRet = securestore_create_open(pcObjectName, &sSstConfig, 0, &sSstHandle);
	if (nRet < 0) {
		nRet = securestore_create_open(pcObjectName, &sSstConfig, SS_CREATE, &sSstHandle);
	}
	if (nRet < 0) {
		PRINT("unable to get secure storage handle for %s object", pcObjectName);
		return -1;
	}
	if (stat((const char *)pAESWrapKeyFile, &st) == 0) {
		if (st.st_size != AES_WRAP_KEY_LEN*2) {
			PRINT("AES Wrap key number of chars expected:%d actual:%ld", AES_WRAP_KEY_LEN*2, st.st_size);
			return -1;
		}
	} else {
		PRINT("stat failed for %s - %s\n", (const char *)pAESWrapKeyFile, strerror(errno));
		return -1;
	}
	nRet = secdbg_read_hexfile_to_bytes((const char *)pAESWrapKeyFile, pAESWrapKey,
			AES_WRAP_KEY_LEN);
	if (nRet != AES_WRAP_KEY_LEN) {
		PRINT("AES Wrap key expected:%u actual:%d", AES_WRAP_KEY_LEN, nRet);
		return -1;
	}

	nRet = sizeof(*pKeyMeta) + sizeof(*pAESKey) + AES_WRAP_KEY_LEN;
	pKeyMeta = calloc(1, nRet);
	if (!pKeyMeta)
		return -1;
	pAESKey = (struct aes_key *) ((uint8_t *)pKeyMeta + sizeof(*pKeyMeta));
	pKeyMeta->total_key_len = sizeof(*pAESKey) + AES_WRAP_KEY_LEN;
	pKeyMeta->format = KEY_PLAIN_FORMAT;
	pKeyMeta->sec_algo = SEC_ALG_AES_WRAP_UNWRAP;
	pAESKey->key_len = AES_WRAP_KEY_LEN;
	memcpy(pAESKey->key, pAESWrapKey, AES_WRAP_KEY_LEN);
	nRet = securestore_save(sSstHandle, NULL, (const unsigned char *)pKeyMeta, nRet);
	free(pKeyMeta);
	if (nRet < 0) {
		PRINT("Failed to save AES Wrap key");
		securestore_close(sSstHandle);
		return nRet;
	}
	securestore_close(sSstHandle);
	return 0;
}

int secdbg_gen_rsa_keypair_and_cryptoformatstore(uint16_t nPort, uint32_t nSstConfig,
		uint8_t **pKey, int *pLen, const uint8_t *pAESWrapKey)
{
	char pcObjectName[MAX_OBJ_NAME_LEN] = {0};
	sst_obj_config_t sSstConfig = {0};
	struct rsa_key rsa_key = {0};
	sshandle_t sSstHandle = -1;
	uint32_t nAuthInfo;
	int nRet = -1;

	if (!pKey || !pLen) {
		PRINT("invalid parameters");
		return nRet;
	}

	update_sst_config(nSstConfig, &sSstConfig);
	snprintf(pcObjectName, MAX_OBJ_NAME_LEN, RSA_KEY_OBJECT_NAME, nPort);
	nRet = securestore_create_open(pcObjectName, &sSstConfig, 0, &sSstHandle);
	if (nRet < 0) {
		nRet = securestore_create_open(pcObjectName, &sSstConfig, SS_CREATE, &sSstHandle);
	}
	if (nRet < 0) {
		PRINT("unable to get secure storage handle for %s object", pcObjectName);
		return -1;
	}

	nRet = rsa_key_generate_and_store(SEC_ALG_RSA_3072, 3,sSstHandle, &rsa_key,
			pAESWrapKey);
	if (nRet < 0)
		return nRet;
	nAuthInfo = generate_secdbg_authinfo(nPort);

	*pLen = rsa_key.modulus.num_len + sizeof(nAuthInfo) + RSA_PUBLIC_KEY_LEN;
	*pKey = malloc(*pLen);
	if (!*pKey)
		goto free;
	memset(*pKey, 0x0, *pLen);

	memcpy(*pKey, &nAuthInfo, sizeof(nAuthInfo));
	*(uint32_t *)*pKey = htonl(nAuthInfo);
	memcpy((*pKey + sizeof(nAuthInfo) + RSA_PUBLIC_KEY_LEN - rsa_key.pub_exp.num_len),
			rsa_key.pub_exp.num_ptr, rsa_key.pub_exp.num_len);
	memcpy((*pKey + sizeof(nAuthInfo) + RSA_PUBLIC_KEY_LEN), rsa_key.modulus.num_ptr,
			rsa_key.modulus.num_len);
	nRet = 0;
free:
	if (rsa_key.pub_exp.num_ptr)
		free(rsa_key.pub_exp.num_ptr);
	if (rsa_key.pri_exp.num_ptr)
		free(rsa_key.pri_exp.num_ptr);
	if (rsa_key.modulus.num_ptr)
		free(rsa_key.modulus.num_ptr);

	securestore_close(sSstHandle);
	return nRet;
}

int secdbg_load_aes_wrap_key_to_tep(uint16_t nPort, enum key_types nType,
		uint32_t nSstConfig)
{
	struct secure_storage_params sSstParams = {0};
	char pcObjectName[MAX_OBJ_NAME_LEN] = {0};
	struct seccrypto_load_key loadkey = {0};
	sst_obj_config_t sSstConfig = {0};
	sshandle_t sSstHandle = -1;
	TEEC_Operation op ={0};
	uint32_t origin = 0;
	TEEC_Result result;
	int nRet = 0;

	update_sst_config(nSstConfig, &sSstConfig);
	snprintf(pcObjectName, MAX_OBJ_NAME_LEN, AES_WRAPKEY_OBJECT_NAME, nPort);
	nRet = securestore_create_open(pcObjectName, &sSstConfig, 0, &sSstHandle);
	if (nRet < 0) {
		PRINT("unable to get secure storage handle for %s object", pcObjectName);
		return -EAGAIN;
	}
	sSstParams.handle = sSstHandle;
	sSstParams.access_perm = sSstConfig.access_perm.u.perms;
	sSstParams.policy_attr = sSstConfig.policy_attr.u.attr;
	sSstParams.crypto_mode_flag = (uint8_t)sSstConfig.crypto_mode_flag;

	loadkey.load_algo = SEC_ALG_AES_WRAP_UNWRAP;
	loadkey.hash_algo = 0;
	loadkey.load_flags = PRIVATE_KEY_PLAINTEXT;
	loadkey.key_type = nType;
	memcpy(&loadkey.sst_params, &sSstParams, sizeof(loadkey.sst_params));

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE,
			TEEC_NONE);
	op.params[0].tmpref.buffer = &loadkey;
	op.params[0].tmpref.size = sizeof(loadkey);

	result = TEEC_InvokeCommand(&secdbg_ctx.session, TA_SECURE_CRYPTO_LOAD_KEY,
			&op, &origin);
	if (result != TEEC_SUCCESS) {
		PRINT("loadkey operation failed ret:%u origin:%u", (uint32_t)result, (uint32_t)origin);
		securestore_close(sSstHandle);
		return -1;
	}
	securestore_close(sSstHandle);
	return 0;
}

int secdbg_load_rsa_key_to_tep(uint16_t nPort, enum key_types nType,
		uint32_t nSstConfig, uint8_t nRSAKeyBlob)
{
	struct secure_storage_params sSstParams = {0};
	char pcObjectName[MAX_OBJ_NAME_LEN] = {0};
	struct seccrypto_load_key loadkey = {0};
	sst_obj_config_t sSstConfig = {0};
	sshandle_t sSstHandle = -1;
	TEEC_Operation op ={0};
	uint32_t origin = 0;
	TEEC_Result result;
	int nRet = 0;

	update_sst_config(nSstConfig, &sSstConfig);
	snprintf(pcObjectName, MAX_OBJ_NAME_LEN, RSA_KEY_OBJECT_NAME, nPort);
	nRet = securestore_create_open(pcObjectName, &sSstConfig, 0, &sSstHandle);
	if (nRet < 0) {
		nRet = securestore_create_open(pcObjectName, &sSstConfig, SS_CREATE, &sSstHandle);
	}
	if (nRet < 0) {
		PRINT("unable to get secure storage handle for %s object", pcObjectName);
		return -1;
	}
	sSstParams.handle = sSstHandle;
	sSstParams.access_perm = sSstConfig.access_perm.u.perms;
	sSstParams.policy_attr = sSstConfig.policy_attr.u.attr;
	sSstParams.crypto_mode_flag = (uint8_t)sSstConfig.crypto_mode_flag;

	loadkey.load_algo = SEC_ALG_RSA_3072;
	loadkey.hash_algo = RSA_PKCS1_5_SHA256;
	if (nRSAKeyBlob)
		loadkey.load_flags = PRIVATE_KEY_BLOB;
	else
		loadkey.load_flags = PRIVATE_KEY_PLAINTEXT;
	loadkey.key_type = nType;
	memcpy(&loadkey.sst_params, &sSstParams, sizeof(loadkey.sst_params));

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE,
			TEEC_NONE);
	op.params[0].tmpref.buffer = &loadkey;
	op.params[0].tmpref.size = sizeof(loadkey);

	result = TEEC_InvokeCommand(&secdbg_ctx.session, TA_SECURE_CRYPTO_LOAD_KEY,
			&op, &origin);
	if (result != TEEC_SUCCESS) {
		PRINT("loadkey operation failed ret:%u origin:%u", (uint32_t)result, (uint32_t)origin);
		securestore_close(sSstHandle);
		return -1;
	}
	securestore_close(sSstHandle);
	return 0;
}

int secdbg_generate_wrapped_auth_key(uint16_t nPort, uint32_t nSstConfig,
		uint32_t *nAssetNumber, uint8_t *pAuthKey, uint32_t nAuthKeyLen,
		uint8_t **pWrappedAuthKey, uint32_t *nWrapLen)
{
	char pcObjectName[MAX_OBJ_NAME_LEN] = {0};
	struct seccrypto_wrap_unwrap wrap = {0};
	sst_obj_config_t sSstConfig = {0};
	sshandle_t sSstHandle = -1;
	TEEC_Operation op = {0};
	uint32_t origin = 0;
	TEEC_Result result;
	int nRet = -1;

	if (!nAssetNumber || !pAuthKey || !pWrappedAuthKey || !nWrapLen) {
		PRINT("invalid parameters");
		return nRet;
	}

	*nWrapLen = nAuthKeyLen + WRAP_BYTES_LEN;
	*pWrappedAuthKey = malloc(*nWrapLen + 8);
	if (!*pWrappedAuthKey)
		return nRet;

	wrap.input = pAuthKey;
	wrap.input_size = nAuthKeyLen;
	wrap.flags |= (1 << WRAP_INPUT_BUFFER);

	wrap.key = (uint8_t *)nAssetNumber;
	wrap.key_size = sizeof(*nAssetNumber);
	wrap.flags |= (1 << WRAP_KEY_TEE_ASSET);

	wrap.output = *pWrappedAuthKey;
	wrap.output_size = *nWrapLen;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE,
			TEEC_NONE);
	op.params[0].tmpref.buffer = &wrap;
	op.params[0].tmpref.size = sizeof(wrap);

	result = TEEC_InvokeCommand(&secdbg_ctx.session, TA_SECURE_CRYPTO_WRAP, &op,
			&origin);
	if (result != TEEC_SUCCESS) {
		PRINT("wrap operation failed ret:%u origin:%u", (uint32_t)result, (uint32_t)origin);
		free(*pWrappedAuthKey);
		return nRet;
	}

	update_sst_config(nSstConfig, &sSstConfig);
	snprintf(pcObjectName, MAX_OBJ_NAME_LEN, WRAPPED_AUTHKEY_OBJECT_NAME, nPort);
	nRet = securestore_create_open(pcObjectName, &sSstConfig, 0, &sSstHandle);
	if (nRet < 0) {
		nRet = securestore_create_open(pcObjectName, &sSstConfig, SS_CREATE, &sSstHandle);
	}
	if (nRet < 0) {
		PRINT("unable to get secure storage handle for %s object", pcObjectName);
		free(*pWrappedAuthKey);
		return nRet;
	}
	*nWrapLen = wrap.output_size;
	nRet = securestore_save(sSstHandle, NULL, (const uint8_t *)*pWrappedAuthKey, *nWrapLen);
	securestore_close(sSstHandle);
	return nRet;
}

int secdbg_wrap_authkey_exists(uint16_t nPort, uint32_t nSstConfig)
{
	char pcObjectName[MAX_OBJ_NAME_LEN + 1] = {0};
	sst_obj_config_t sSstConfig = {0};
	sshandle_t sSstHandle = -1;
	uint8_t cDummyBuf[10] = {0};
	uint32_t nLen = 0;
	int nRet;

	update_sst_config(nSstConfig, &sSstConfig);
	snprintf(pcObjectName, MAX_OBJ_NAME_LEN, WRAPPED_AUTHKEY_OBJECT_NAME, nPort);
	nRet = securestore_create_open(pcObjectName, &sSstConfig, 0, &sSstHandle);
	if (nRet < 0) {
		PRINT("unable to get secure storage handle for %s object", pcObjectName);
		return nRet;
	}
	nRet = securestore_retrieve(sSstHandle, NULL, cDummyBuf, sizeof(cDummyBuf), &nLen);
	if ((nRet < 0) || !nLen) {
		securestore_close(sSstHandle);
		return nRet;
	}
	securestore_close(sSstHandle);
	return nRet;
}

int secdbg_get_wrapped_auth_key(uint16_t nPort, uint32_t nSstConfig,
		uint8_t **pWrappedAuthKey, uint32_t *nWrapLen)
{
	char pcObjectName[MAX_OBJ_NAME_LEN] = {0};
	sst_obj_config_t sSstConfig = {0};
	sshandle_t sSstHandle = -1;
	int nRet = -1;

	if (!pWrappedAuthKey || !nWrapLen) {
		PRINT("invalid parameters");
		return nRet;
	}

	update_sst_config(nSstConfig, &sSstConfig);
	snprintf(pcObjectName, MAX_OBJ_NAME_LEN, WRAPPED_AUTHKEY_OBJECT_NAME, nPort);
	nRet = securestore_create_open(pcObjectName, &sSstConfig, 0, &sSstHandle);
	if (nRet < 0) {
		PRINT("unable to get secure storage handle for %s object", pcObjectName);
		return nRet;
	}

	*nWrapLen = MAX_WRAP_AUTHKEY_LEN;
	*pWrappedAuthKey = malloc(*nWrapLen);
	if (!*pWrappedAuthKey)
		return -1;

	nRet = securestore_retrieve(sSstHandle, NULL, *pWrappedAuthKey, MAX_WRAP_AUTHKEY_LEN,
			nWrapLen);
	if ((nRet < 0) || !(*nWrapLen)) {
		free(*pWrappedAuthKey);
		*pWrappedAuthKey = NULL;
		securestore_close(sSstHandle);
		return nRet;
	}
	securestore_close(sSstHandle);
	return nRet;
}

int secdbg_complete_nonce(uint8_t *puNonce, uint8_t *puNonceA, const uint8_t *puNonceB)
{
	int idx = 0;
	int nCount = -1;
	int nRet = -1;

	if ((!puNonceB) || (!puNonce) || (!puNonceA)) {
		PRINT("invalid parameters");
		return nRet;
	}

	nRet = secdbg_get_random_number(puNonceA, NONCE_BUF_LEN);
	if (nRet < 0) {
		PRINT("failed to get random number");
		return nRet;
	}

	for (idx = 0; idx < NONCE_BUF_LEN; idx++) {
		puNonce[++nCount] = puNonceA[idx];
	}
	for (idx = 0; idx < NONCE_BUF_LEN; idx++) {
		puNonce[++nCount] = puNonceB[idx];
	}
	return (nCount + 1);
}

int secdbg_gen_crypto_sign(uint8_t *puNonce, uint32_t nLen,
		struct rsa_signature **rsa_sign)
{
	struct seccrypto_sign_param sign = {0};
	struct rsa_signature *rsa_sign_tmp;
	TEEC_Operation op = {0};
	uint32_t origin = 0;
	TEEC_Result result;
	int nRet = -1;

	sign.sign_algo = SEC_ALG_RSA_3072;
	sign.hash_algo = RSA_PKCS1_5_SHA256;
	sign.hash_flags = SC_FLAG_FINAL;
	sign.sign_len = sizeof(*rsa_sign);
	sign.data = puNonce;
	sign.data_len = nLen;

	rsa_sign_tmp = (struct rsa_signature *) malloc(sizeof(*rsa_sign_tmp) + 3072/8);
	if (!rsa_sign_tmp)
		return nRet;
	sign.signature = (uint8_t *)rsa_sign_tmp;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE,
			TEEC_NONE);
	op.params[0].tmpref.buffer = &sign;
	op.params[0].tmpref.size = sizeof(sign);

	result = TEEC_InvokeCommand(&secdbg_ctx.session, TA_SECURE_CRYPTO_SIGN, &op,
			&origin);
	if (result != TEEC_SUCCESS) {
		PRINT("sign operation failed ret:%u origin:%u", (uint32_t)result, (uint32_t)origin);
		free(rsa_sign_tmp);
		return nRet;
	}
	*rsa_sign = rsa_sign_tmp;
	return 0;
}

int free_sec_store_objects(uint32_t nSstConfig, int nPort)
{
	int nRet = 0;
	sshandle_t sSstHandle = 0;
	sst_obj_config_t sSstConfig = {0};
	char pcObjectName[MAX_OBJ_NAME_LEN] = {0};

	update_sst_config(nSstConfig, &sSstConfig);
	snprintf(pcObjectName, MAX_OBJ_NAME_LEN, WRAPPED_AUTHKEY_OBJECT_NAME, nPort);
	nRet = securestore_create_open(pcObjectName, &sSstConfig, 0, &sSstHandle);
	if (nRet == SST_SUCCESS) {
		nRet = securestore_delete(sSstHandle);
		if (nRet < 0) {
			PRINT("securestore_delete failed nRet:%d", nRet);
			return nRet;
		}
	}

	memset(pcObjectName, 0x0, MAX_OBJ_NAME_LEN);
	snprintf(pcObjectName, MAX_OBJ_NAME_LEN, AES_WRAPKEY_OBJECT_NAME, nPort);
	nRet = securestore_create_open(pcObjectName, &sSstConfig, 0, &sSstHandle);
	if (nRet == SST_SUCCESS) {
		nRet = securestore_delete(sSstHandle);
		if (nRet < 0) {
			PRINT("securestore_delete failed nRet:%d", nRet);
			return nRet;
		}
	}

	memset(pcObjectName, 0x0, MAX_OBJ_NAME_LEN);
	snprintf(pcObjectName, MAX_OBJ_NAME_LEN, RSA_KEY_OBJECT_NAME, nPort);
	nRet = securestore_create_open(pcObjectName, &sSstConfig, 0, &sSstHandle);
	if (nRet == SST_SUCCESS) {
		nRet = securestore_delete(sSstHandle);
		if (nRet < 0) {
			PRINT("securestore_delete failed nRet:%d", nRet);
			return nRet;
		}
	}

	return nRet;
}
#endif

#ifdef SECDBG_CTEST
int secdbg_delete_authkey(sst_obj_config_t *pcxObjConfig, int nPort)
{
	sshandle_t xSsHandle = 0;
	int nRet = -1;

	if ((!pcxObjConfig) || INVALID_PORT(nPort)) {
		PRINT("invalid parameters");
		return nRet;
	}

	nRet = securestore_create_open(sstDebugPortName[nPort], pcxObjConfig,
			0, &xSsHandle);
	if (nRet < 0) {
		PRINT("failed to create/open obj- error: %d", nRet);
		return nRet;
	}

	nRet = securestore_delete(xSsHandle);
	if (nRet < 0) {
		PRINT("securestore_delete failed nRet:%d", nRet);
		return nRet;
	}
	return 0;
}
#endif
