/******************************************************************************

  Copyright © 2023-2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/
/***************************************************************************** *
 *     File Name  : seccrypto_client.c										   *
 *     Description: Secure crypto service client application.				   *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <signal.h>
#include <openssl/rsa.h>
#include <openssl/aes.h>

#include <fapi_sec_storage.h>
#include <tee_client_api.h>
#include <linux/mxltee.h>

#define MAX_COMMAND_LENGTH	200
#define MAX_COMMAND_COUNT	10

/*! Sample UUID for Secure crypto service Trusted agent */
#define TA_SECURE_CRYPTO_UUID   {0xFFFF0000, 0x0001, 0x0002, \
		{0x0, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07}}

#define AES_WRAP_KEY_SIZE	32
#define MAX_BUF_SIZE		1024
#define MAX_OBJNAME_LEN		30
#define WRAP_BYTES_LEN		8

#define DWORD_ALIGN(x)	do {				\
		uintptr_t addr = (uintptr_t)x;		\
		x = (void*) ((addr + 3) & ~3);		\
	} while(0)

static uint8_t aes_kek[AES_WRAP_KEY_SIZE];

struct APP_Context {
	TEEC_Context context;
	TEEC_Session session;
	uint32_t login_type;
	TEEC_UUID uuid;
};

static inline uint32_t signature_len(enum sec_alg algo)
{
	uint32_t sign_len = 0;

	switch (algo) {
		case SEC_ALG_RSA_2048:
			sign_len = 2048/8;
			break;
		case SEC_ALG_RSA_3072:
			sign_len = 3072/8;
			break;
		case SEC_ALG_RSA_4096:
			sign_len = 4096/8;
			break;
		case SEC_ALG_ECDSA_P256:
			sign_len = 256/8;
			break;
		case SEC_ALG_ECDSA_P384:
			sign_len = 384/8;
			break;
	}
	return sign_len;

}

static uint32_t read_file(const char *fname, uint8_t **data)
{
	uint32_t data_len = 0;
	uint8_t *buf = NULL;
	FILE *fp = NULL;

	*data = NULL;
	fp = fopen(fname, "r");
	if (!fp) {
		fprintf(stderr, "unable to open %s file\n", fname);
		return data_len;
	}

	fseek(fp, 0, SEEK_END);

	data_len = ftell(fp);
	if (!data_len) {
		fclose(fp);
		fprintf(stderr, "no data found in %s file\n", fname);
		return data_len;
	}
	rewind(fp);

	buf = (uint8_t*) calloc(1, data_len);
	if (!buf) {
		fclose(fp);
		return 0;
	}
	if (!fread(buf, data_len, 1, fp)) {
		fclose(fp);
		fprintf(stderr, "unable to read complete data\n");
		return 0;
	}
	buf[data_len] = '\0';
	*data = buf;
	fclose(fp);
	return data_len;
}

static int write_file(const char *fname, uint8_t *data, uint32_t data_len)
{
	FILE *fp = NULL;

	fp = fopen(fname, "w");
	if (!fp) {
		fprintf(stderr, "unable to open %s file\n", fname);
		return -1;
	}

	data_len = fwrite(data, data_len, 1, fp);
	fclose(fp);
	return data_len;
}

static void write_sign2file(const char *fname, void *buffer, int rsa)
{
	struct rsa_signature *rsa_sign = NULL;
	struct ecdsa_signature *ecdsa_sign = NULL;
	FILE *fp = NULL;

	fp = fopen(fname, "wb");
	if (!fp) {
		fprintf(stderr, "unable to open %s file\n", fname);
		return;
	}

	fwrite(&rsa, 1, sizeof(int), fp);
	if (rsa) {
		rsa_sign = buffer;
		fwrite(rsa_sign, 1, sizeof(*rsa_sign), fp);
		fwrite(rsa_sign->rsa_signature.num_ptr, 1, rsa_sign->rsa_signature.num_len, fp);
	} else {
		ecdsa_sign = buffer;
		fwrite(ecdsa_sign, 1, sizeof(*ecdsa_sign), fp);
		fwrite(ecdsa_sign->r.num_ptr, 1, ecdsa_sign->r.num_len, fp);
		fwrite(ecdsa_sign->s.num_ptr, 1, ecdsa_sign->s.num_len, fp);
	}
	fclose(fp);
}

static int read_file2sign(const char *fname, uint8_t **buffer)
{
	struct rsa_signature *rsa_sign = NULL;
	struct ecdsa_signature *ecdsa_sign = NULL;
	FILE *fp = NULL;
	int rsa = 0;

	fp = fopen(fname, "rb");
	if (!fp) {
		fprintf(stderr, "unable to open %s file\n", fname);
		return -1;
	}

	if (fread(&rsa, 1, sizeof(int), fp) <= 0) {
		fclose(fp);
		fprintf(stderr, "unable to read %s file\n", fname);
		return -1;
	}
	if (rsa) {
		rsa_sign = malloc(sizeof(*rsa_sign));
		if (!rsa_sign) {
			fclose(fp);
			return -1;
		}
		if (fread(rsa_sign, 1, sizeof(*rsa_sign), fp) <= 0) {
			free(rsa_sign);
			fclose(fp);
			fprintf(stderr, "unable to read %s file\n", fname);
			return -1;
		}
		*buffer = realloc(rsa_sign, sizeof(*rsa_sign) + rsa_sign->rsa_signature.num_len);
		if (!*buffer) {
			free(rsa_sign);
			fclose(fp);
			return -1;
		}
		rsa_sign = (struct rsa_signature *) *buffer;
		rsa_sign->rsa_signature.num_ptr = ((uint8_t *)rsa_sign) + sizeof(*rsa_sign);
		if (fread(rsa_sign->rsa_signature.num_ptr, 1, rsa_sign->rsa_signature.num_len, fp) <= 0) {
			free(*buffer);
			fclose(fp);
			fprintf(stderr, "unable to read %s file\n", fname);
			return -1;
		}
		rsa = 1;
	} else {
		ecdsa_sign = malloc(sizeof(*ecdsa_sign));
		if (!ecdsa_sign) {
			fclose(fp);
			return -1;
		}
		if (fread(ecdsa_sign, 1, sizeof(*ecdsa_sign), fp) <= 0) {
			free(ecdsa_sign);
			fclose(fp);
			fprintf(stderr, "unable to read %s file\n", fname);
			return -1;
		}
		*buffer = realloc(ecdsa_sign, sizeof(*ecdsa_sign) + ecdsa_sign->r.num_len + ecdsa_sign->s.num_len);
		if (!*buffer) {
			fclose(fp);
			free(ecdsa_sign);
			return -1;
		}
		ecdsa_sign = (struct ecdsa_signature *) *buffer;
		ecdsa_sign->r.num_ptr = ((uint8_t *)ecdsa_sign) + sizeof(*ecdsa_sign);
		ecdsa_sign->s.num_ptr = ecdsa_sign->r.num_ptr + ecdsa_sign->r.num_len;
		if (fread(ecdsa_sign->r.num_ptr, 1, ecdsa_sign->r.num_len, fp) <= 0) {
			free(*buffer);
			fclose(fp);
			fprintf(stderr, "unable to read %s file\n", fname);
			return -1;
		}
		if (fread(ecdsa_sign->s.num_ptr, 1, ecdsa_sign->s.num_len, fp) <= 0) {
			free(*buffer);
			fclose(fp);
			fprintf(stderr, "unable to read %s file\n", fname);
			return -1;
		}
		rsa = 0;
	}
	fclose(fp);
	return rsa;
}

static void print_signature(enum sec_alg algo, void *signature, char *output_fname)
{
	struct rsa_signature *rsa_sign = NULL;
	struct ecdsa_signature *ecdsa_sign = NULL;
	int idx = 0;

	if ((algo == SEC_ALG_RSA_2048) || (algo == SEC_ALG_RSA_4096)
			|| (algo == SEC_ALG_RSA_3072)) {
		rsa_sign = (struct rsa_signature *)signature;
		printf("RSA Signature: 0x");
		for (idx = 0; idx < rsa_sign->rsa_signature.num_len; idx++) {
			printf("%02x", *(rsa_sign->rsa_signature.num_ptr + idx));
		}
		printf("\n");
		if (output_fname)
			write_sign2file(output_fname, signature, 1);
	} else if ((algo == SEC_ALG_ECDSA_P256) || (algo == SEC_ALG_ECDSA_P384)) {
		ecdsa_sign = (struct ecdsa_signature *)signature;
		printf("ECDSA Signature(r): 0x");
		for (idx = 0; idx < ecdsa_sign->r.num_len; idx++) {
			printf("%02x", *(ecdsa_sign->r.num_ptr + idx));
		}
		printf("\n");
		printf("ECDSA Signature(s): 0x");
		for (idx = 0; idx < ecdsa_sign->s.num_len; idx++) {
			printf("%02x", *(ecdsa_sign->s.num_ptr + idx));
		}
		printf("\n");
		if (output_fname)
			write_sign2file(output_fname, signature, 0);
	}
}

static int generate_rsa_key_pair(struct rsa_key *rsa_key, enum sec_alg sec_algo)
{
	BIGNUM *exp = NULL;
	RSA *rsa = NULL;
	int bits = 0;

	exp = BN_new();
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
		return -1;

	rsa_key->pub_exp.num_len = sizeof(uint32_t);
	rsa_key->pub_exp.num_ptr = malloc(rsa_key->pub_exp.num_len);
	BN_bn2bin(RSA_get0_e(rsa), rsa_key->pub_exp.num_ptr);

	rsa_key->pri_exp.num_len = BN_num_bytes(RSA_get0_d(rsa));
	rsa_key->pri_exp.num_ptr = malloc(rsa_key->pri_exp.num_len);
	BN_bn2bin(RSA_get0_d(rsa), rsa_key->pri_exp.num_ptr);

	rsa_key->modulus.num_len = BN_num_bytes(RSA_get0_n(rsa));
	rsa_key->modulus.num_ptr = malloc(rsa_key->modulus.num_len);
	BN_bn2bin(RSA_get0_n(rsa), rsa_key->modulus.num_ptr);

	BN_free(exp);
	RSA_free(rsa);

	return 0;
}

static void aes_wrap(struct big_number *number, uint8_t *dst)
{
	AES_KEY wrap_key = {0};
	int ret, idx;

	AES_set_encrypt_key(aes_kek, (AES_WRAP_KEY_SIZE * 8), &wrap_key);
	ret = AES_wrap_key(&wrap_key, NULL, dst, number->num_ptr, number->num_len);
	printf("len:%d wrapped private exp : 0x", ret);
	for (idx = 0; idx < ret; idx ++)
		printf("%02x", dst[idx]);
	printf("\n");
	number->num_len = ret;
}

static int rsa_key_generate_and_store(enum key_format format, enum sec_alg algo,
		int pub_key_len, sshandle_t handle)
{
	struct key_meta_info *meta = NULL;
	struct rsa_key rsa_key = {0};
	uint8_t *buffer = NULL;
	uint8_t dummy_buf[10];
	uint32_t cont_len = 0;
	size_t buf_len = 0;
	int opt_index = 0;
	int opt = 0;
	char yesno;
	int idx = 0;

	/* Check data exists or not */
	if (securestore_retrieve(handle, dummy_buf, sizeof(dummy_buf), &cont_len)) {
		if (cont_len > 0) {
			printf("Key information exists, want to overwrite y/n - ");
			fflush(stdout);
			if (scanf("%c", &yesno) <= 0)
				yesno = 'y';
			printf("\n");
			if ((yesno == 'n') || (yesno == 'N'))
				return 0;
		}
	}

	generate_rsa_key_pair(&rsa_key, algo);
	rsa_key.pub_exp.num_len = pub_key_len;

	printf("len:%d modulus:0x", rsa_key.modulus.num_len);
	for (idx = 0; idx < rsa_key.modulus.num_len; idx ++)
		printf("%02x", *(rsa_key.modulus.num_ptr + idx));
	printf("\n");

	buf_len = (sizeof(*meta) + (sizeof(uint32_t) * 3) + rsa_key.pub_exp.num_len
			+ rsa_key.pri_exp.num_len + rsa_key.modulus.num_len + 12);
	if (format == KEY_WRAP_FORMAT)
		buf_len += WRAP_BYTES_LEN;
	meta = malloc(buf_len);
	if (!meta)
		goto free;
	meta->format = format;
	meta->sec_algo = algo;
	meta->total_key_len = (rsa_key.pub_exp.num_len + rsa_key.pri_exp.num_len + rsa_key.modulus.num_len);
	buffer = (uint8_t*) (((uint8_t*)meta) + sizeof(*meta));
	// copy public key
	memcpy(buffer, &rsa_key.pub_exp.num_len, sizeof(uint32_t));
	buffer += sizeof(uint32_t);
	if (pub_key_len == 4) {
		for (idx = 0; idx < rsa_key.pub_exp.num_len; idx++)
			*(buffer + idx) = *(rsa_key.pub_exp.num_ptr + rsa_key.pub_exp.num_len - idx - 1);
	} else {
		memcpy(buffer, rsa_key.pub_exp.num_ptr, rsa_key.pub_exp.num_len);
	}
	printf("len:%d public key:0x", rsa_key.pub_exp.num_len);
	for (idx = 0; idx < rsa_key.pub_exp.num_len; idx ++)
		printf("%02x", *(buffer + idx));
	printf("\n");
	buffer += rsa_key.pub_exp.num_len;
	DWORD_ALIGN(buffer);
	// copy private key
	if (format == KEY_WRAP_FORMAT)
		aes_wrap(&rsa_key.pri_exp, (buffer + sizeof(uint32_t)));
	else
		memcpy(buffer + sizeof(uint32_t), rsa_key.pri_exp.num_ptr, rsa_key.pri_exp.num_len);
	memcpy(buffer, &rsa_key.pri_exp.num_len, sizeof(uint32_t));
	buffer += sizeof(uint32_t);
	buffer += rsa_key.pri_exp.num_len;
	DWORD_ALIGN(buffer);
	// copy modulus
	memcpy(buffer, &rsa_key.modulus.num_len, sizeof(uint32_t));
	buffer += sizeof(uint32_t);
	memcpy(buffer, rsa_key.modulus.num_ptr, rsa_key.modulus.num_len);

free:
	if (rsa_key.pub_exp.num_ptr)
		free(rsa_key.pub_exp.num_ptr);
	if (rsa_key.pri_exp.num_ptr)
		free(rsa_key.pri_exp.num_ptr);
	if (rsa_key.modulus.num_ptr)
		free(rsa_key.modulus.num_ptr);

	if (securestore_save(handle, (const unsigned char *)meta, buf_len) < 0) {
		free(meta);
		return -1;
	}
	free(meta);
	return 0;
}

static int aes_key_generate_and_store(enum key_format format, sshandle_t handle)
{
	struct key_meta_info *key_meta = NULL;
	struct aes_key *aes_key = NULL;
	uint8_t *buffer = NULL;
	uint32_t cont_len = 0;
	uint32_t len = 0;
	char yesno;
	int idx;

	len = sizeof(*key_meta) + sizeof(*aes_key) + AES_WRAP_KEY_SIZE;
	key_meta = calloc(1, len);
	if (!key_meta)
		return -1;
	aes_key = (struct aes_key*) ((void*)key_meta + sizeof(*key_meta));

	/* Check data exists or not */
	if (securestore_retrieve(handle, (uint8_t*)key_meta, len, &cont_len)) {
		if (cont_len > 0) {
			printf("Key information exists, want to overwrite y/n - ");
			fflush(stdout);
			if (scanf("%c", &yesno) <= 0)
				yesno = 'y';
			printf("\n");
			if ((yesno == 'n') || (yesno == 'N')) {
				memcpy(&aes_kek[0], &aes_key->key[0], sizeof(aes_kek));
				free(key_meta);
				return 0;
			}
		}
	}

	/* key_meta_info values */
	key_meta->total_key_len = sizeof(*aes_key) + AES_WRAP_KEY_SIZE;
	key_meta->format = format;
	key_meta->sec_algo = SEC_ALG_AES_WRAP_UNWRAP;
	/* aes_key values */
	aes_key->key_len = AES_WRAP_KEY_SIZE;
	for (idx = 0; idx < AES_WRAP_KEY_SIZE; idx++)
		aes_key->key[idx] = rand() % 256;

	printf( "AES_KEY: 0x");
	for (idx = 0; idx < AES_WRAP_KEY_SIZE; idx++)
		printf("%02x", aes_key->key[idx]);
	printf("\n");

	if (securestore_save(handle, (const unsigned char *)key_meta, len) < 0) {
		free(key_meta);
		return -1;
	}
	memcpy(&aes_kek[0], &aes_key->key[0], sizeof(aes_kek));
	return 0;
}

static int fill_secure_storage_details(const char *name, uint32_t config,
		struct secure_storage_params *sst_params)
{
	sst_obj_config_t sst_config = {0};
	sshandle_t sst_handle = -1;
	int ret = 0;

	sst_config.access_perm.u.perms = config & 0xFF;
	sst_config.policy_attr.u.attr = (config >> 8) & 0xFFFF;
	sst_config.crypto_mode_flag = (sst_crypto_mode_flag_t)(config >> 24) & 0xFF;

	ret = securestore_create_open(name, &sst_config, 0, &sst_handle);
	if (ret < 0) {
		ret = securestore_create_open(name, &sst_config, SS_CREATE, &sst_handle);
	}
	if (ret < 0) {
		fprintf(stderr, "unable to get secure storage handle for %s object\n", name);
		return -1;
	}
	sst_params->handle = sst_handle;
	sst_params->access_perm = sst_config.access_perm.u.perms;
	sst_params->policy_attr = sst_config.policy_attr.u.attr;
	sst_params->crypto_mode_flag = (uint8_t)sst_config.crypto_mode_flag;
	return 0;
}

static void print_wrap_data(struct seccrypto_wrap_unwrap *wrap, const char *fname)
{
	int idx;

	printf("output_size:%u wrap->output: 0x", wrap->output_size);
	for (idx = 0; idx < wrap->output_size; idx++)
		printf("%02x", wrap->output[idx]);
	printf("\n");
	write_file(fname, wrap->output, wrap->output_size);
}

static void print_unwrap_data(struct seccrypto_wrap_unwrap *unwrap)
{
	int idx;

	printf("output_size:%u unwrap->output: 0x", unwrap->output_size);
	for (idx = 0; idx < unwrap->output_size; idx++)
		printf("%02x", unwrap->output[idx]);
	printf("\n");
}

static void print_macgen_output(struct seccrypto_mac_gen *macgen)
{
	int idx;

	printf("output_size:%u macgen->output: 0x", macgen->output_size);
	for (idx = 0; idx < macgen->output_size; idx++)
		printf("%02x", macgen->output[idx]);
	printf("\n");
}

static void print_key_buffer(struct seccrypto_gen_key *genkey)
{
	struct ecdsa_key_pair *key_pair = genkey->key_ptr;
	int key_len = genkey->key_len;
	int idx;

	if (genkey->flag == SEC_CRYPTO_RETURN_KEY_NONE) {
		printf("keys will be stored in secure storage\n");
		return;
	}

	printf("key_pair->pub_key.modulus.num_len:%u\n", key_pair->pub_key.modulus.num_len);
	for (idx = 0; idx < key_pair->pub_key.modulus.num_len; idx++) {
		printf("%02x", *(uint8_t*)(key_pair->pub_key.modulus.num_ptr + idx));
	}
	printf("\n");

	printf("key_pair->pub_key.a.num_len:%u\n", key_pair->pub_key.a.num_len);
	for (idx = 0; idx < key_pair->pub_key.a.num_len; idx++) {
		printf("%02x", *(uint8_t*)(key_pair->pub_key.a.num_ptr + idx));
	}
	printf("\n");

	printf("key_pair->pub_key.b.num_len:%u\n", key_pair->pub_key.b.num_len);
	for (idx = 0; idx < key_pair->pub_key.b.num_len; idx++) {
		printf("%02x", *(uint8_t*)(key_pair->pub_key.b.num_ptr + idx));
	}
	printf("\n");

	printf("key_pair->pub_key.g_order.num_len:%u\n", key_pair->pub_key.g_order.num_len);
	for (idx = 0; idx < key_pair->pub_key.g_order.num_len; idx++) {
		printf("%02x", *(uint8_t*)(key_pair->pub_key.g_order.num_ptr + idx));
	}
	printf("\n");

	printf("key_pair->pub_key.Gx.num_len:%u\n", key_pair->pub_key.Gx.num_len);
	for (idx = 0; idx < key_pair->pub_key.Gx.num_len; idx++) {
		printf("%02x", *(uint8_t*)(key_pair->pub_key.Gx.num_ptr + idx));
	}
	printf("\n");

	printf("key_pair->pub_key.Gy.num_len:%u\n", key_pair->pub_key.Gy.num_len);
	for (idx = 0; idx < key_pair->pub_key.Gy.num_len; idx++) {
		printf("%02x", *(uint8_t*)(key_pair->pub_key.Gy.num_ptr + idx));
	}
	printf("\n");

	printf("key_pair->pub_key.Qx.num_len:%u\n", key_pair->pub_key.Qx.num_len);
	for (idx = 0; idx < key_pair->pub_key.Qx.num_len; idx++) {
		printf("%02x", *(uint8_t*)(key_pair->pub_key.Qx.num_ptr + idx));
	}
	printf("\n");

	printf("key_pair->pub_key.Qy.num_len:%u\n", key_pair->pub_key.Qy.num_len);
	for (idx = 0; idx < key_pair->pub_key.Qy.num_len; idx++) {
		printf("%02x", *(uint8_t*)(key_pair->pub_key.Qy.num_ptr + idx));
	}
	printf("\n");

	printf("key_pair->pri_key.pri_key.num_len:%u\n", key_pair->pri_key.pri_key.num_len);
	for (idx = 0; idx < key_pair->pri_key.pri_key.num_len; idx++) {
		printf("%02x", *(uint8_t*)(key_pair->pri_key.pri_key.num_ptr + idx));
	}
	printf("\n");
}

#define KEYGEN_CONFIG	"a:o:n:c:"
static struct option keygen_options[] = {
	{"algo", required_argument, NULL, 'a'},
	{"options", required_argument, NULL, 'o'},
	{"name", required_argument, NULL, 'n'},
	{"config", required_argument, NULL, 'c'},
	{NULL, 0, NULL, 0}
};

static int parse_keygen_options(int argc, char **argv, TEEC_Operation *op, struct seccrypto_gen_key *genkey)
{
	struct secure_storage_params *sst_params = NULL;
	char object_name[MAX_OBJNAME_LEN] = {0};
	uint32_t sst_config = 0;
	int opt_index = 0;
	int opt = 0;

	sst_params = &genkey->sst_params;
	while ((opt = getopt_long(argc, argv, KEYGEN_CONFIG, keygen_options, &opt_index)) != -1) {
		switch (opt) {
			case 'a':
				genkey->genkey_algo = strtoul(optarg, NULL, 16);
				break;
			case 'o':
				genkey->flag = strtoul(optarg, NULL, 16);
				break;
			case 'n':
				strncpy(object_name, optarg, sizeof(object_name) - 1);
				object_name[strlen(optarg)] = '\0';
				break;
			case 'c':
				sst_config = strtoul(optarg, NULL, 16);
				break;
			default:
				fprintf(stderr, "keygen: invalid command line option\n");
				return -1;
		}
	}
	if (!strnlen(object_name, MAX_OBJNAME_LEN)) {
		fprintf(stderr, "keygen: sst object name is invalid\n");
		return -1;
	}
	if (!sst_config) {
		fprintf(stderr, "keygen: secure storage configuration required\n");
		return -1;
	}
	if (fill_secure_storage_details(object_name, sst_config, sst_params) < 0)
		return -1;

	if ((genkey->genkey_algo != SEC_ALG_ECDSA_P256) && (genkey->genkey_algo != SEC_ALG_ECDSA_P384)) {
		fprintf(stderr, "keygen: only ECDSA key generation is supported from SCSA\n");
		return -1;
	}

	if ((genkey->flag < SEC_CRYPTO_RETURN_KEY_PAIR) || (genkey->flag > SEC_CRYPTO_RETURN_KEY_NONE)) {
		fprintf(stderr, "keygen: invalid key generation options\n");
		return -1;
	}
	if (genkey->flag != SEC_CRYPTO_RETURN_KEY_NONE) {
		genkey->key_ptr = calloc(1, MAX_BUF_SIZE);
		genkey->key_len = MAX_BUF_SIZE;
	}
	if (!sst_params) {
		fprintf(stderr, "keygen: secure storage parameters required for key generation\n");
		return -1;
	}

	op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	op->params[0].tmpref.buffer = genkey;
	op->params[0].tmpref.size = sizeof(*genkey);
	return 0;
}

#define KEYLOAD_CONFIG	"a:h:o:t:n:c:"
static struct option keyload_options[] = {
	{"algo", required_argument, NULL, 'a'},
	{"hash", required_argument, NULL, 'h'},
	{"options", required_argument, NULL, 'o'},
	{"ktype", required_argument, NULL, 't'},
	{"name", required_argument, NULL, 'n'},
	{"config", required_argument, NULL, 'c'},
	{NULL, 0, NULL, 0}
};

static int parse_keyload_options(int argc, char **argv, TEEC_Operation *op, struct seccrypto_load_key *loadkey)
{
	struct secure_storage_params *sst_params = NULL;
	char object_name[MAX_OBJNAME_LEN] = {0};
	uint32_t sst_config = 0;
	int pub_key_len = 4;
	int opt_index = 0;
	int opt = 0;

	sst_params = &loadkey->sst_params;
	while ((opt = getopt_long(argc, argv, KEYLOAD_CONFIG, keyload_options, &opt_index)) != -1) {
		switch (opt) {
			case 'a':
				loadkey->load_algo = strtoul(optarg, NULL, 16);
				break;
			case 'h':
				loadkey->hash_algo = strtoul(optarg, NULL, 16);
				break;
			case 'o':
				loadkey->load_flags = strtoul(optarg, NULL, 16);
				break;
			case 't':
				loadkey->key_type = strtoul(optarg, NULL, 16);
				break;
			case 'n':
				strncpy(object_name, optarg, sizeof(object_name) - 1);
				object_name[strlen(optarg)] = '\0';
				break;
			case 'c':
				sst_config = strtoul(optarg, NULL, 16);
				break;
			default:
				fprintf(stderr, "keyload: invalid key loading options\n");
				return -1;
		}
	}
	if (!sst_config) {
		fprintf(stderr, "keyload: secure storage configuration required\n");
		return -1;
	}
	if (!strnlen(object_name, MAX_OBJNAME_LEN)) {
		fprintf(stderr, "keyload: sst object name is invalid\n");
		return -1;
	}
	if ((loadkey->load_flags < PRIVATE_KEY_BLOB) || (loadkey->load_flags > PRIVATE_KEY_PLAINTEXT)) {
		fprintf(stderr, "keyload: load key options are invalid\n");
		return -1;
	}
	if ((loadkey->key_type < KEY_AES) || (loadkey->key_type > KEY_RSA)) {
		fprintf(stderr, "keyload: loadkey key type option invalid\n");
		return -1;
	}
	if ((loadkey->hash_algo < RSA_PKCS1_5_SHA1) || (loadkey->hash_algo > ECDSA_ASN1_SHA384)) {
		fprintf(stderr, "keyload: hash algo is invalid\n");
		return -1;
	}
	if ((loadkey->hash_algo >= RSA_PKCS1_5_SHA1) && (loadkey->hash_algo <= RSA_PKCS1_5_SHA512))
		pub_key_len = 3;
	if (fill_secure_storage_details(object_name, sst_config, sst_params) < 0)
		return -1;
	switch (loadkey->load_algo) {
		case SEC_ALG_RSA_2048:
		case SEC_ALG_RSA_3072:
		case SEC_ALG_RSA_4096:
			if (rsa_key_generate_and_store((int)loadkey->load_flags,
					loadkey->load_algo, pub_key_len, (sshandle_t)sst_params->handle)) {
				fprintf(stderr, "keyload: RSA key failed to store in secure storage\n");
				return -1;
			}
			break;
		case SEC_ALG_ECDSA_P256:
		case SEC_ALG_ECDSA_P384:
			break;
		case SEC_ALG_AES_WRAP_UNWRAP:
			if (aes_key_generate_and_store((int)loadkey->load_flags,
					(sshandle_t)sst_params->handle) < 0) {
				fprintf(stderr, "keyload: AES key failed to store in secure storage\n");
				return -1;
			}
			break;
	}
	op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	op->params[0].tmpref.buffer = loadkey;
	op->params[0].tmpref.size = sizeof(*loadkey);
	return 0;
}

#define SIGN_CONFIG	"a:h:o:i:s:"
static struct option sign_options[] = {
	{"algo", required_argument, NULL, 'a'},
	{"hash", required_argument, NULL, 'h'},
	{"options", required_argument, NULL, 'o'},
	{"input", required_argument, NULL, 'i'},
	{"sign", required_argument, NULL, 's'},
	{NULL, 0, NULL, 0}
};

static int parse_signing_options(int argc, char **argv, TEEC_Operation *op,
		struct seccrypto_sign_param *sign, char *input_fname, char *output_fname)
{
	struct rsa_signature *rsa_sign = NULL;
	struct ecdsa_signature *ecdsa_sign = NULL;
	uint8_t *buffer = NULL;
	FILE *fp = NULL;
	int opt_index = 0;
	int opt = 0;

	while ((opt = getopt_long(argc, argv, SIGN_CONFIG, sign_options, &opt_index)) != -1) {
		switch (opt) {
			case 'a':
				sign->sign_algo = strtoul(optarg, NULL, 16);
				break;
			case 'o':
				sign->hash_flags = strtoul(optarg, NULL, 16);
				break;
			case 'i':
				strncpy(input_fname, optarg, MAX_COMMAND_LENGTH - 1);
				input_fname[strlen(optarg)] = '\0';
				break;
			case 's':
				strncpy(output_fname, optarg, MAX_COMMAND_LENGTH - 1);
				output_fname[strlen(optarg)] = '\0';
				break;
			case 'h':
				sign->hash_algo = strtoul(optarg, NULL, 16);
				break;
			default:
				fprintf(stderr, "sign: option is invalid\n");
				return -1;
		}
	}
	if ((sign->hash_flags < SC_FLAG_PARTIAL) || (sign->hash_flags > SC_FLAG_FINAL)) {
		fprintf(stderr, "sign: signature generation flags are invalid\n");
		return -1;
	}
	if ((sign->hash_algo < RSA_PKCS1_5_SHA1) || (sign->hash_algo > ECDSA_ASN1_SHA384)) {
		fprintf(stderr, "sign: signature hash algo is invalid\n");
		return -1;
	}
	if (strlen(input_fname) < 0) {
		fprintf(stderr, "sign: input file name is required to sign the content\n");
		return -1;
	}
	if (strlen(output_fname) < 0) {
		fprintf(stderr, "sign: output file name is required to store signature\n");
		return -1;
	}

	sign->data_len = read_file(input_fname, &sign->data);
	if (!sign->data_len)
		return -1;
	if ((sign->sign_algo == SEC_ALG_RSA_2048) || (sign->sign_algo == SEC_ALG_RSA_4096)
			|| (sign->sign_algo == SEC_ALG_RSA_3072)) {
		sign->sign_len = sizeof(*sign);
		buffer = calloc(1, sizeof(*rsa_sign) + signature_len(sign->sign_algo));
	} else if ((sign->sign_algo == SEC_ALG_ECDSA_P256) || (sign->sign_algo == SEC_ALG_ECDSA_P384)) {
		sign->sign_len = sizeof(*sign);
		buffer = calloc(1, sizeof(*ecdsa_sign) + signature_len(sign->sign_algo) * 2);
	} else {
		fprintf(stderr, "sign: algorithm is not supported\n");
		free(sign->data);
		return -1;
	}
	sign->signature = buffer;
	op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	op->params[0].tmpref.buffer = sign;
	op->params[0].tmpref.size = sizeof(*sign) + sign->data_len + sign->sign_len + signature_len(sign->sign_algo);
	return 0;
}

#define VERIFY_CONFIG	"a:h:o:i:s:"
static struct option verify_options[] = {
	{"algo", required_argument, NULL, 'a'},
	{"hash", required_argument, NULL, 'h'},
	{"options", required_argument, NULL, 'o'},
	{"input", required_argument, NULL, 'i'},
	{"sign", required_argument, NULL, 's'},
	{NULL, 0, NULL, 0}
};

static int parse_verify_options(int argc, char **argv, TEEC_Operation *op,
	struct seccrypto_verify_param *verify, char *input_fname, char *output_fname)
{
	struct rsa_signature *rsa_sign = NULL;
	struct ecdsa_signature *ecdsa_sign = NULL;
	uint8_t *buffer = NULL;
	FILE *fp = NULL;
	int opt_index = 0;
	int opt = 0;

	while ((opt = getopt_long(argc, argv, VERIFY_CONFIG, verify_options, &opt_index)) != -1) {
		switch (opt) {
			case 'a':
				verify->sign_algo = strtoul(optarg, NULL, 16);
				break;
			case 'o':
				verify->hash_flags = strtoul(optarg, NULL, 16);
				break;
			case 'i':
				strncpy(input_fname, optarg, MAX_COMMAND_LENGTH - 1);
				input_fname[strlen(optarg)] = '\0';
				break;
			case 's':
				strncpy(output_fname, optarg, MAX_COMMAND_LENGTH - 1);
				output_fname[strlen(optarg)] = '\0';
				break;
			case 'h':
				verify->hash_algo = strtoul(optarg, NULL, 16);
				break;
			default:
				fprintf(stderr, "verify: option is invalid\n");
				return -1;
		}
	}
	if ((verify->hash_flags < SC_FLAG_PARTIAL) || (verify->hash_flags > SC_FLAG_FINAL)) {
		fprintf(stderr, "verify: signature verification flags are invalid\n");
		return -1;
	}
	if ((verify->hash_algo < RSA_PKCS1_5_SHA1) || (verify->hash_algo > ECDSA_ASN1_SHA384)) {
		fprintf(stderr, "verify: signature hash algo is invalid\n");
		return -1;
	}
	if (strlen(input_fname) < 0) {
		fprintf(stderr, "verify: input file name is required to input data\n");
		return -1;
	}
	if (strlen(output_fname) < 0) {
		fprintf(stderr, "verify: output file name is required to store signature\n");
		return -1;
	}
	verify->data_len = read_file(input_fname, &verify->data);
	if (!verify->data_len)
		return -1;
	if ((verify->sign_algo == SEC_ALG_RSA_2048) || (verify->sign_algo == SEC_ALG_RSA_4096)
			|| (verify->sign_algo == SEC_ALG_RSA_3072)) {
		verify->sign_len = sizeof(*rsa_sign);
		if (read_file2sign(output_fname, &verify->signature) < 0) {
			fprintf(stderr, "verify: signature doesn't exists\n");
			free(verify->data);
			return -1;
		}
	} else if ((verify->sign_algo == SEC_ALG_ECDSA_P256) || (verify->sign_algo == SEC_ALG_ECDSA_P384)) {
		verify->sign_len = sizeof(*ecdsa_sign);
		if (read_file2sign(output_fname, &verify->signature) < 0) {
			fprintf(stderr, "verify: signature doesn't exists\n");
			free(verify->data);
			return -1;
		}
		print_signature(verify->sign_algo, verify->signature, NULL);
	} else {
		fprintf(stderr, "verify: algorithm is not supported\n");
		free(verify->data);
		return -1;
	}
	op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	op->params[0].tmpref.buffer = verify;
	op->params[0].tmpref.size = sizeof(*verify);
	return 0;
}

#define WRAP_CONFIG	"kio:n:c:f:s:t"
static struct option wrap_options[] = {
	{"key", no_argument, NULL, 'k'},
	{"input", no_argument, NULL, 'i'},
	{"output", required_argument, NULL, 'o'},
	{"name", required_argument, NULL, 'n'},
	{"config", required_argument, NULL, 'c'},
	{"file", required_argument, NULL, 'f'},
	{"string", required_argument, NULL, 's'},
	{"tep", no_argument, NULL, 't'},
	{NULL, 0, NULL, 0}
};

static int parse_wrap_options(int argc, char **argv, TEEC_Operation *op,
	struct seccrypto_wrap_unwrap *wrap, char *output_fname)
{
	struct secure_storage_params *sst_params = NULL;
	char file_name[MAX_COMMAND_LENGTH] = {0};
	char object_name[MAX_OBJNAME_LEN] = {0};
	uint8_t dummy_buff[MAX_BUF_SIZE];
	uint32_t sst_config = 0;
	uint32_t data_len = 0;
	uint8_t *data = NULL;
	int opt_index, opt;
	char prev = '0', main_opt;

	while ((opt = getopt_long(argc, argv, WRAP_CONFIG, wrap_options, &opt_index)) != -1) {
		switch (opt) {
			case 'k':
				main_opt = opt;
				break;
			case 'i':
				main_opt = opt;
				break;
			case 'o':
				strncpy(output_fname, optarg, MAX_OBJNAME_LEN - 1);
				output_fname[strlen(optarg)] = '\0';
				break;
			case 'n':
				if (prev == 'k' || prev == 'i') {
					strncpy(object_name, optarg, sizeof(object_name) - 1);
					object_name[strlen(optarg)] = '\0';
					if (prev == 'k')
						wrap->flags |= (1 << WRAP_KEY_SS_HANDLE);
					else
						wrap->flags |= (1 << WRAP_INPUT_SS_HANDLE);
				} else {
					fprintf(stderr, "wrap: -n should be followed by -i or -k\n");
					goto free;
				}
				break;
			case 'c':
				if (prev == 'n') {
					sst_config = strtoul(optarg, NULL, 16);
					if (!sst_config) {
						fprintf(stderr, "wrap: secure storage configuration required\n");
						goto free;
					}
					if (!strnlen(object_name, MAX_OBJNAME_LEN)) {
						fprintf(stderr, "wrap: sst object name is invalid\n");
						goto free;
					}
					sst_params = calloc(1, sizeof(*sst_params));
					if (!sst_params) {
						fprintf(stderr, "wrap: sst params is null\n");
						goto free;
					}
					if (main_opt == 'k') {
						if (fill_secure_storage_details(object_name, sst_config, sst_params) < 0) {
							free(sst_params);
							goto free;
						}
						securestore_retrieve((sshandle_t)sst_params->handle, dummy_buff,
								sizeof(dummy_buff), &data_len);
						if (!data_len) {
							free(sst_params);
							goto free;
						}
						wrap->key = (uint8_t*)sst_params;
						wrap->key_size = data_len;
					} else {
						if (fill_secure_storage_details(object_name, sst_config, sst_params) < 0) {
							free(sst_params);
							goto free;
						}
						securestore_retrieve((sshandle_t)sst_params->handle, dummy_buff,
								sizeof(dummy_buff), &data_len);
						if (!data_len) {
							free(sst_params);
							goto free;
						}
						wrap->input = (uint8_t*)sst_params;
						wrap->input_size = data_len;
					}
				} else {
					fprintf(stderr, "wrap: -c should be followed by -n\n");
					goto free;
				}
				break;
			case 'f':
				if (prev == 'k' || prev == 'i') {
					strncpy(file_name, optarg, MAX_COMMAND_LENGTH - 1);
					file_name[strlen(optarg)] = '\0';
					if (!strnlen(file_name, MAX_COMMAND_LENGTH)) {
						fprintf(stderr, "wrap: file name is invalid\n");
						goto free;
					}
					data_len = read_file(file_name, &data);
					if (!data_len)
						goto free;
					if (prev == 'k') {
						wrap->key = data;
						wrap->key_size = data_len;
						wrap->flags |= (1 << WRAP_KEY_BUFFER);
					} else {
						wrap->input = data;
						wrap->input_size = data_len;
						wrap->flags |= (1 << WRAP_INPUT_BUFFER);
					}
				} else {
					fprintf(stderr, "wrap: -f should be followed by -i or -k\n");
					goto free;
				}
				break;
			case 's':
				if (prev == 'k' || prev == 'i') {
					data_len = strlen(optarg);
					if (!data_len)
						goto free;
					data = malloc(data_len);
					if (!data)
						goto free;
					strncpy(data, optarg, data_len);
					data[data_len] = '\0';
					if (prev == 'k') {
						wrap->key = data;
						wrap->key_size = data_len;
						wrap->flags |= (1 << WRAP_KEY_BUFFER);
					} else {
						wrap->input = data;
						wrap->input_size = data_len;
						wrap->flags |= (1 << WRAP_INPUT_BUFFER);
					}
				} else {
					fprintf(stderr, "wrap: -s should be followed by -i or -k\n");
				}
				break;
			case 't':
				if (prev == 'k') {
					wrap->flags |= (1 << WRAP_KEY_TEE_ASSET);
					wrap->key = NULL;
					wrap->key_size = 0;
				} else {
					fprintf(stderr, "wrap: -t should be followed by -k\n");
					goto free;
				}
				break;
			default:
				fprintf(stderr, "wrap: option is invalid\n");
				goto free;
		}
		prev = opt;
	}
	if (!wrap->input) {
		fprintf(stderr, "wrap: input option cannot be null\n");
		goto free;
	}
	if (!wrap->key && !((wrap->flags >> WRAP_KEY_TEE_ASSET) & 0x1)) {
		fprintf(stderr, "wrap: key option cannot be null\n");
		goto free;
	}
	if (wrap->input_size) {
		wrap->output_size = wrap->input_size + 8;
		wrap->output = malloc(wrap->output_size);
	}

	if (((wrap->flags >> WRAP_KEY_BUFFER) & 0x1) && wrap->key) {
		printf("key_size:%u wrap->key:%s\n", wrap->key_size, (char*)wrap->key);
	} else if ((wrap->flags >> WRAP_KEY_TEE_ASSET) & 0x1) {
		printf("wrap key is in asset store\n");
	} else {
		printf("wrap key is in secure storage\n");
	}

	if (((wrap->flags >> WRAP_INPUT_BUFFER) & 0x1) && wrap->input) {
		printf("input_size:%u wrap->input:%s\n", wrap->input_size, (char*)wrap->input);
	} else {
		printf("wrap input data is in secure storage\n");
	}

	printf("flags:0x%x\n", wrap->flags);

	op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	op->params[0].tmpref.buffer = wrap;
	op->params[0].tmpref.size = sizeof(*wrap);
	return 0;
free:
	if (wrap->output)
		free(wrap->output);
	if (wrap->input)
		free(wrap->input);
	if (wrap->key)
		free(wrap->key);
	return -1;
}

#define UNWRAP_CONFIG	"kion:c:f:s:t"
static struct option unwrap_options[] = {
	{"key", no_argument, NULL, 'k'},
	{"input", no_argument, NULL, 'i'},
	{"output", required_argument, NULL, 'o'},
	{"name", required_argument, NULL, 'n'},
	{"config", required_argument, NULL, 'c'},
	{"file", required_argument, NULL, 'f'},
	{"string", required_argument, NULL, 's'},
	{"tep", no_argument, NULL, 't'},
	{NULL, 0, NULL, 0}
};

static int parse_unwrap_options(int argc, char **argv, TEEC_Operation *op,
	struct seccrypto_wrap_unwrap *unwrap)
{
	struct secure_storage_params *sst_params = NULL;
	char file_name[MAX_COMMAND_LENGTH] = {0};
	char object_name[MAX_OBJNAME_LEN] = {0};
	uint8_t dummy_buff[MAX_BUF_SIZE];
	uint32_t sst_config = 0;
	uint32_t data_len = 0;
	uint8_t *data = NULL;
	int opt_index, opt;
	char prev = '0', main_opt;

	while ((opt = getopt_long(argc, argv, UNWRAP_CONFIG, unwrap_options, &opt_index)) != -1) {
		switch (opt) {
			case 'k':
			case 'i':
			case 'o':
				main_opt = opt;
				break;
			case 'n':
				if (prev == 'k' || prev == 'i' || prev == 'o') {
					strncpy(object_name, optarg, sizeof(object_name) - 1);
					object_name[strlen(optarg)] = '\0';
					if (prev == 'k')
						unwrap->flags |= (1 << UNWRAP_KEY_SS_HANDLE);
					else if (prev == 'i')
						unwrap->flags |= (1 << UNWRAP_INPUT_SS_HANDLE);
					else
						unwrap->flags |= (1 << UNWRAP_OUTPUT_SS_HANDLE);
				} else {
					fprintf(stderr, "unwrap: -n should be followed by -i or -k or -o\n");
					goto free;
				}
				break;
			case 'c':
				if (prev == 'n') {
					sst_config = strtoul(optarg, NULL, 16);
					if (!sst_config) {
						fprintf(stderr, "unwrap: secure storage configuration required\n");
						goto free;
					}
					if (!strnlen(object_name, MAX_OBJNAME_LEN)) {
						fprintf(stderr, "unwrap: sst object name is invalid\n");
						goto free;
					}
					sst_params = calloc(1, sizeof(*sst_params));
					if (!sst_params) {
						fprintf(stderr, "unwrap: sst params memory allocation failed\n");
						goto free;
					}
					if (main_opt == 'k') {
						if (fill_secure_storage_details(object_name, sst_config, sst_params) < 0) {
							free(sst_params);
							goto free;
						}
						securestore_retrieve((sshandle_t)sst_params->handle, dummy_buff,
								sizeof(dummy_buff), &data_len);
						if (!data_len) {
							free(sst_params);
							goto free;
						}
						unwrap->key = (uint8_t*)sst_params;
						unwrap->key_size = data_len;
					} else if (main_opt == 'i') {
						if (fill_secure_storage_details(object_name, sst_config, sst_params) < 0) {
							free(sst_params);
							goto free;
						}
						securestore_retrieve((sshandle_t)sst_params->handle, dummy_buff,
								sizeof(dummy_buff), &data_len);
						if (!data_len) {
							free(sst_params);
							goto free;
						}
						unwrap->input = (uint8_t*)sst_params;
						unwrap->input_size = data_len;
					} else {
						if (fill_secure_storage_details(object_name, sst_config, sst_params) < 0) {
							free(sst_params);
							goto free;
						}
						unwrap->output = (uint8_t*)sst_params;
					}
				} else {
					fprintf(stderr, "unwrap: -c should be followed by -n\n");
					goto free;
				}
				break;
			case 'f':
				if (prev == 'k' || prev == 'i') {
					strncpy(file_name, optarg, MAX_COMMAND_LENGTH - 1);
					file_name[strlen(optarg)] = '\0';
					if (!strnlen(file_name, MAX_COMMAND_LENGTH)) {
						fprintf(stderr, "unwrap: file name is invalid\n");
						goto free;
					}
					data_len = read_file(file_name, &data);
					if (!data_len)
						goto free;
					if (prev == 'k') {
						unwrap->key = data;
						unwrap->key_size = data_len;
						unwrap->flags |= (1 << UNWRAP_KEY_BUFFER);
					} else {
						unwrap->input = data;
						unwrap->input_size = data_len;
						unwrap->flags |= (1 << UNWRAP_INPUT_BUFFER);
					}
				} else {
					fprintf(stderr, "unwrap: -f should be followed by -i or -k\n");
					goto free;
				}
				break;
			case 's':
				if (prev == 'k' || prev == 'i') {
					data_len = strlen(optarg);
					if (!data_len)
						goto free;
					data = malloc(data_len);
					if (!data)
						goto free;
					strncpy(data, optarg, data_len);
					data[data_len] = '\0';
					if (prev == 'k') {
						unwrap->key = data;
						unwrap->key_size = data_len;
						unwrap->flags |= (1 << UNWRAP_KEY_BUFFER);
					} else {
						unwrap->input = data;
						unwrap->input_size = data_len;
						unwrap->flags |= (1 << UNWRAP_INPUT_BUFFER);
					}
				} else {
					fprintf(stderr, "unwrap: -s should be followed by -i or -k\n");
				}
				break;
			case 't':
				if (prev == 'k' || prev == 'o') {
					if (prev == 'k') {
						unwrap->flags |= (1 << UNWRAP_KEY_OTP);
						unwrap->key = NULL;
						unwrap->key_size = 32;
					} else {
						unwrap->flags |= (1 << UNWRAP_OUTPUT_TEE_ASSET);
						unwrap->output = NULL;
					}
				} else {
					fprintf(stderr, "unwrap: -t should be followed by -k or -o\n");
					goto free;
				}
				break;
			default:
				fprintf(stderr, "unwrap: option is invalid\n");
				goto free;
		}
		prev = opt;
	}
	if (!unwrap->input) {
		fprintf(stderr, "unwrap: input option cannot be null\n");
		goto free;
	}
	if (!unwrap->key && !((unwrap->flags >> UNWRAP_KEY_OTP) & 0x1)) {
		fprintf(stderr, "unwrap: key option cannot be null\n");
		goto free;
	}
	if ((unwrap->flags >> UNWRAP_OUTPUT_SS_HANDLE) & 0x1) {
		unwrap->output_size = unwrap->input_size - 8;
	} else if ((unwrap->flags >> UNWRAP_OUTPUT_TEE_ASSET) & 0x1) {
		unwrap->output_size = unwrap->input_size - 8;
	} else {
		unwrap->flags |= (1 << UNWRAP_OUTPUT_BUFFER);
		unwrap->output_size = unwrap->input_size - 8;
		unwrap->output = calloc(1, unwrap->output_size);
	}

	if (((unwrap->flags >> UNWRAP_KEY_BUFFER) & 0x1) && unwrap->key) {
		printf("key_size:%u unwrap->key:%s\n", unwrap->key_size, (char*)unwrap->key);
	} else if ((unwrap->flags >> UNWRAP_KEY_OTP) & 0x1) {
		printf("unwrap key is in asset store\n");
	} else {
		printf("unwrap key is in secure storage\n");
	}

	if ((unwrap->flags >> UNWRAP_INPUT_BUFFER) & 0x1) {
		printf("input_size:%u unwrap->input:%s\n", unwrap->input_size, (char*)unwrap->input);
	} else {
		printf("unwrap input data is in secure storage\n");
	}

	printf("flags:0x%x\n", unwrap->flags);

	op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	op->params[0].tmpref.buffer = unwrap;
	op->params[0].tmpref.size = sizeof(*unwrap);
	return 0;
free:
	if (unwrap->output)
		free(unwrap->output);
	if (unwrap->key)
		free(unwrap->key);
	if (unwrap->input)
		free(unwrap->input);
	return -1;
}

#define MACGEN_CONFIG	"a:ikn:c:f:t:"
static struct option macgen_options[] = {
	{"algo", required_argument, NULL, 'a'},
	{"input", no_argument, NULL, 'i'},
	{"key", no_argument, NULL, 'k'},
	{"name", required_argument, NULL, 'n'},
	{"config", required_argument, NULL, 'c'},
	{"file", required_argument, NULL, 'f'},
	{"tep", required_argument, NULL, 't'},
	{NULL, 0, NULL, 0}
};

static int parse_macgen_options(int argc, char **argv, TEEC_Operation *op,
		struct seccrypto_mac_gen *macgen)
{
	struct secure_storage_params *sst_params = NULL;
	char file_name[MAX_COMMAND_LENGTH] = {0};
	char object_name[MAX_OBJNAME_LEN] = {0};
	uint8_t dummy_buff[MAX_BUF_SIZE];
	uint32_t sst_config = 0;
	uint32_t data_len = 0;
	uint8_t *data = NULL;
	int opt_index, opt;
	char prev = '0', main_opt;

	while ((opt = getopt_long(argc, argv, MACGEN_CONFIG, macgen_options, &opt_index)) != -1) {
		switch (opt) {
			case 'k':
			case 'i':
				main_opt = opt;
				break;
			case 'a':
				macgen->mac_algo = strtoul(optarg, NULL, 16);
				break;
			case 'n':
				if (prev != 'k') {
					fprintf(stderr, "macgen: -n should be followed by -k\n");
					goto free;
				}
				strncpy(object_name, optarg, sizeof(object_name) - 1);
				object_name[strlen(optarg)] = '\0';
				break;
			case 'c':
				if (prev != 'n' || main_opt != 'k') {
					fprintf(stderr, "macgen: -c should be followed by -n. Applicable only for key input\n");
					goto free;
				}
				sst_config = strtoul(optarg, NULL, 16);
				if (!sst_config) {
					fprintf(stderr, "macgen: secure storage configuration required\n");
					goto free;
				}
				if (!strnlen(object_name, MAX_OBJNAME_LEN)) {
					fprintf(stderr, "macgen: sst object name is invalid\n");
					goto free;
				}
				sst_params = calloc(1, sizeof(*sst_params));
				if (!sst_params) {
					fprintf(stderr, "macgen: sst params memory allocation failed\n");
					goto free;
				}
				if (fill_secure_storage_details(object_name, sst_config, sst_params) < 0) {
					free(sst_params);
					goto free;
				}
				securestore_retrieve((sshandle_t)sst_params->handle, dummy_buff,
						sizeof(dummy_buff), &data_len);
				if (!data_len) {
					free(sst_params);
					goto free;
				}
				macgen->mac_key = (uint8_t*)sst_params;
				macgen->key_size = data_len;
				macgen->flags |= (1 << MAC_KEY_SECURE_STORE);
				break;
			case 'f':
				if (prev == 'k' || prev == 'i') {
					strncpy(file_name, optarg, MAX_COMMAND_LENGTH - 1);
					file_name[strlen(optarg)] = '\0';
					if (!strnlen(file_name, MAX_COMMAND_LENGTH)) {
						fprintf(stderr, "macgen: file name is invalid\n");
						goto free;
					}
					data_len = read_file(file_name, &data);
					if (!data_len) {
						fprintf(stderr, "macgen: invalid data length\n");
						goto free;
					}
					if (prev == 'k') {
						macgen->mac_key = data;
						macgen->key_size = data_len;
						macgen->flags |= (1 << MAC_KEY_BUFFER);
					} else {
						macgen->input = data;
						macgen->input_size = data_len;
					}
				} else {
					fprintf(stderr, "macgen: -f should be followed by -i or -k\n");
					goto free;
				}
				break;
			case 't':
				if (prev != 'k') {
					fprintf(stderr, "macgen: -t should be followed by -k\n");
					goto free;
				}
				data = calloc(1, sizeof(uint32_t));
				if (!data) {
					fprintf(stderr, "macgen: memory allocation failed\n");
					goto free;
				}
				*data = strtoul(optarg, NULL, 16);
				macgen->mac_key = data;
				macgen->key_size = sizeof(uint32_t);
				macgen->flags |= (1 << MAC_KEY_OTP);
				break;
			default:
				fprintf(stderr, "macgen: '-%c' option is invalid\n", opt);
				goto free;
		}
		prev = opt;
	}
	switch (macgen->mac_algo) {
		case MAC_ALGO_HMAC_SHA1:
			macgen->output_size = 20;
			break;
		case MAC_ALGO_HMAC_SHA224:
			macgen->output_size = 28;
			break;
		case MAC_ALGO_HMAC_SHA256:
			macgen->output_size = 32;
			break;
		case MAC_ALGO_HMAC_SHA384:
			macgen->output_size = 48;
			break;
		case MAC_ALGO_HMAC_SHA512:
			macgen->output_size = 64;
			break;
		case MAC_ALGO_AES_CMAC:
			macgen->output_size = 20;
			break;
		case MAC_ALGO_AES_CBC_MAC:
			macgen->output_size = 20;
			break;
		case MAC_ALGO_POLY1305:
			macgen->output_size = 20;
			break;
		default:
			fprintf(stderr, "macgen: MAC generation algorithm is invalid\n");
			goto free;
	}
	if (!macgen->mac_key || !macgen->key_size ||
			(macgen->flags < (1 << MAC_KEY_SECURE_STORE)) ||
			(macgen->flags > (1 << MAC_KEY_OTP))) {
		fprintf(stderr, "macgen: MAC key input is invalid\n");
		goto free;
	}
	if (!macgen->input || !macgen->input_size) {
		fprintf(stderr, "macgen: MAC input data or salt is invalid\n");
		goto free;
	}
	macgen->output = calloc(1, macgen->output_size);
	if (!macgen->output) {
		fprintf(stderr, "macgen: MAC output buffer is not allocated\n");
		goto free;
	}
	op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	op->params[0].tmpref.buffer = macgen;
	op->params[0].tmpref.size = sizeof(*macgen);
	return 0;
free:
	if (macgen->input)
		free(macgen->input);
	if (macgen->output)
		free(macgen->output);
	if (macgen->mac_key)
		free(macgen->mac_key);
	return -1;
}

static void handle_signal(int signo)
{
	switch (signo) {
		case SIGINT:
			printf("\nseccrypto$ ");
			fflush(stdout);
			break;
		default:
			break;
	}
}

void print_help(void)
{
	printf("Usage: <command> [parameters]\n");
	printf("gen  [key generation parameters]	-	Key generation like ECDSA, RSA\n");
	printf("load [key loading parameters]		-	Key loading to asset\n");
	printf("sign [signature generation parameters]		-	Signature generation for user data\n");
	printf("verify [signature verification parameters]	-	Signature verification for user data\n");
	printf("wrap   [wrap parameters]			-	Wrap parameters\n");
	printf("unwrap [unwrap parameters]			-	Unwrap parameters\n");
	printf("macgen [macgen parameters]			-	MAC generation parameters\n");

	printf("\n");

	printf("[key generation parameters]\n");
	printf("-a or --algo	-	algorithms like ALG_ECDSA_P256, ALG_RSA_2048 etc.. see below for more\n");
	printf("-o or --options	-	options for key generation like 0-RETURN_KEY_PAIR, 1-RETURN_PUBLIC_KEY, 2-RETURN_KEY_PAIR_PLAIN, 3-RETURN_KEY_NONE\n");
	printf("-n or --name	-	secure storage object name to create handle like ecdsakey\n");
	printf("-c or --config	-	secure storage config to create handle\n");

	printf("[key loading parameters]\n");
	printf("-a or --algo	-	algorithms like ALG_ECDSA_P256, ALG_RSA_2048 etc.. see below for more\n");
	printf("-h or --hash	-	hash algorithm like RSA_PKCS1_5_SHA1, ECDSA_ASN1_SHA384 etc.. see below for more\n");
	printf("-o or --options	-	options like 1-KEYBLOB, 2-PLAINTEXT\n");
	printf("-t or --type	-	key type like 0-AES, 1-PUBLIC, 2-PRIVATE\n");
	printf("-n or --name	-	secure storage object name to create handle like aeskey\n");
	printf("-c or --config	-	secure storage config to create handle\n");

	printf("[signature generation parameters]\n");
	printf("-a or --algo	-	algorithms like ALG_ECDSA_P256, ALG_RSA_2048 etc.. see below for more\n");
	printf("-h or --hash	-	hash algorithm like RSA_PKCS1_5_SHA1, ECDSA_ASN1_SHA384 etc.. see below for more\n");
	printf("-o or --options	-	options like 0-PARTIAL, 1-FINAL. To generate parital or full signature for data\n");
	printf("-i or --input	-	data file to generate signature\n");
	printf("-s or --sign	-	output file to store generated signature\n");

	printf("[signature verification parameters]\n");
	printf("-a or --algo	-	algorithms like ALG_ECDSA_P256, ALG_RSA_2048 etc.. see below for more\n");
	printf("-h or --hash	-	hash algorithm like RSA_PKCS1_5_SHA1, ECDSA_ASN1_SHA384 etc.. see below for more\n");
	printf("-o or --options	-	options like 0-PARTIAL, 1-FINAL. To verify parital or full signature for data\n");
	printf("-i or --input	-	data file to verify signature\n");
	printf("-s or --sign	-	generated signature file to verify signature\n");

	printf("[wrap parameters]\n");
	printf("-k or --key	-	wrap key options like stream(-s) or secure store(-n&-c) or input file(-f) or tep(-t)\n");
	printf("-i or --input	-	input options like stream(-s) or secure store(-n&-c) or input file(-f)\n");
	printf("-o or --output	-	output file name to store wrap data like -o output.txt\n");
	printf("-n or --name	-	secure storage object name to create handle like object_name\n");
	printf("-c or --config	-	secure storage config to create handle like -n object_name -c 0x10000CF\n");
	printf("-f or --file	-	file name to store/load data like -f file.txt\n");
	printf("-s or --stream	-	stream of data like -s 0x123344\n");
	printf("-t or --tep	-	key already loaded in tep like -t\n");

	printf("[unwrap parameters]\n");
	printf("-k or --key	-	unwrap key options like stream(-s) or secure store(-n&-c) or input file(-f) or tep(-t)\n");
	printf("-i or --input	-	input options like stream(-s) or secure store(-n&-c) or input file(-f)\n");
	printf("-o or --output	-	output option secure storage(-n&-c)\n");
	printf("-n or --name	-	secure storage object name to create handle like object_name\n");
	printf("-c or --config	-	secure storage config to create handle like -n object_name -c 0x10000CF\n");
	printf("-f or --file	-	file name to store/load data like -f file.txt\n");
	printf("-s or --stream	-	stream of data like -s 123344\n");
	printf("-t or --tep	-	key already loaded in tep like -t\n");

	printf("[macgen parameters]\n");
	printf("-a or --mac_algo-	MAC generation algorithms like HMAC_SHA1, HMAC_SHA224, HMAC_SHA256 etc.. see below for more\n");
	printf("-k or --key	-	MAC generation key options like secure store(-n&-c) or input file(-f) or tep(-t). Ex: -k -t 4\n");
	printf("-i or --input	-	input data or salt options like input file(-f). Ex: -i -f filename\n");
	printf("-n or --name	-	secure storage object name to create handle like object_name. Ex: -k -n obj_name -c 0x10000CF\n");
	printf("-c or --config	-	secure storage config to create handle like -n object_name -c 0x10000CF\n");
	printf("-f or --file	-	file name to read data or key like -f file.txt\n");
	printf("-s or --stream	-	stream of data like -s 123344\n");
	printf("-t or --tep	-	OTP asset number\n");

	printf("\n");
	printf("algo	: 0-RSA_2048, 1-RSA_3072, 2-RSA_4096, 3-ECDSA_P256, 4-ECDSA_P384, 5-AES\n");
	printf("hash	: 0-RSA_PKCS1_5_SHA1, 1-RSA_PKCS1_5_SHA256, 2-RSA_PKCS1_5_SHA384, 3-RSA_PKCS1_5_SHA512, 4-RSA_PSS_SHA1, 5-RSA_PSS_SHA256, ");
	printf("6-RSA_PSS_SHA384, 7-RSA_PSS_SHA512, 8-ECDSA_ASN1_SHA256, 9-ECDSA_ASN1_SHA384\n");
	printf("mac_algo: 0-HMAC_SHA1, 1-HMAC_SHA224, 2-HMAC_SHA256, 3-HMAC_SHA384, 4-HMAC_SHA512, 5-AES_CMAC, 6-AES_CBC_MAC, 7-POLY1305\n"); 
	printf("config	: 0x10000CF sample secure storage configuration\n");
}

static struct APP_Context scsa_app = {
	.uuid = TA_SECURE_CRYPTO_UUID,
	.context = {0},
	.session = 0,
	.login_type = TEEC_LOGIN_USER,
};

static int init_scsa_session(struct APP_Context *ctx)
{
	TEEC_Result result = 0;
	uint32_t origin = 0;

	result = TEEC_InitializeContext(NULL, &ctx->context);
	if (result != TEEC_SUCCESS) {
		fprintf(stderr, "SCSA context initialization failed, ret:%d\n", result);
		return result;
	}

	result = TEEC_OpenSession(&ctx->context, &ctx->session, &ctx->uuid,
			ctx->login_type, NULL, NULL, &origin);
	if (result != TEEC_SUCCESS) {
		fprintf(stderr, "SCSA open session failed ret:%d origin:%d\n", result, origin);
		return result;
	}
	return result;
}

static int execute_scsa_command(struct APP_Context *ctx, uint32_t cmd, TEEC_Operation *op)
{
	TEEC_Result result = 0;
	uint32_t origin = 0;

	result = TEEC_InvokeCommand(&ctx->session, cmd, op, &origin);
	switch (result) {
		case TEEC_SUCCESS:
			printf("Command success\n");
			break;
		default:
			fprintf(stderr, "Command failed ret:%d origin:%d\n", result, origin);
	}
	return result;
}

static void exit_scsa_session(struct APP_Context *ctx)
{
	TEEC_CloseSession(&ctx->session);
	TEEC_FinalizeContext(&ctx->context);
}

int main(int argc, char **argv)
{
	struct seccrypto_sign_param sign = {0};
	struct seccrypto_verify_param verify = {0};
	struct seccrypto_gen_key genkey = {0};
	struct seccrypto_load_key loadkey = {0};
	struct seccrypto_wrap_unwrap unwrap = {0};
	struct seccrypto_wrap_unwrap wrap = {0};
	struct seccrypto_mac_gen macgen = {0};
	char command[MAX_COMMAND_LENGTH] = {0};
	char output_fname[MAX_COMMAND_LENGTH] = {0};
	char input_fname[MAX_COMMAND_LENGTH] = {0};
	char *subcmd[MAX_COMMAND_COUNT] = {NULL};
	TEEC_Operation op ={0};
	bool dont_exit = true;
	int argcount = 0;
	char *cmd = NULL;

	signal(SIGINT, handle_signal);
	if (init_scsa_session(&scsa_app) != TEEC_SUCCESS) {
		fprintf(stderr, "SCSA client application initilization failed!\n");
		return -1;
	}
	printf("SCSA client application initilized!\n");

	while (dont_exit) {
		argcount = 0;
		optind = 1;
		printf("seccrypto$ ");
		fflush(stdout);

		memset(&command[0], MAX_COMMAND_LENGTH, 0x0);
		if (fgets(command, sizeof(command), stdin) == NULL) {
			fprintf(stderr, "input command is invalid\n");
			continue;
		};
		command[strcspn(command, "\n")] = '\0';

		cmd = strtok(command, " \n");
		if (!cmd) continue;

		if (strcmp(cmd, "quit") == 0) {
			dont_exit = false;
		} else if (strcmp(cmd, "help") == 0) {
			print_help();
		} else if (strcmp(cmd, "load") == 0) {
			memset(&loadkey, 0x0, sizeof(loadkey));
			subcmd[argcount++] = cmd;
			while (cmd = strtok(NULL, " ")) {
				subcmd[argcount++] = cmd;
			}
			if (argcount < 2) {
				fprintf(stderr, "'%s' requires sub arguments\n", subcmd[0]);
				continue;
			}
			if (parse_keyload_options(argcount, subcmd, &op, &loadkey) < 0) {
				fprintf(stderr, "invalid command line arguments for keygen service\n");
				continue;
			}
			if (execute_scsa_command(&scsa_app, TA_SECURE_CRYPTO_LOAD_KEY, &op) != TEEC_SUCCESS)
				continue;
		} else if (strcmp(cmd, "gen") == 0) {
			memset(&genkey, 0x0, sizeof(genkey));
			subcmd[argcount++] = cmd;
			while (cmd = strtok(NULL, " ")) {
				subcmd[argcount++] = cmd;
			}
			if (argcount < 2) {
				fprintf(stderr, "'%s' requires sub arguments\n", subcmd[0]);
				continue;
			}
			if (parse_keygen_options(argcount, subcmd, &op, &genkey) < 0) {
				fprintf(stderr, "invalid command line arguments for keygen service\n");
				continue;
			}
			if (execute_scsa_command(&scsa_app, TA_SECURE_CRYPTO_GEN_KEYPAIR, &op) != TEEC_SUCCESS)
				continue;
			print_key_buffer(&genkey);
			if (genkey.key_ptr)
				free(genkey.key_ptr);
		} else if (strcmp(cmd, "sign") == 0) {
			memset(&sign, 0x0, sizeof(sign));
			subcmd[argcount++] = cmd;
			while (cmd = strtok(NULL, " ")) {
				subcmd[argcount++] = cmd;
			}
			if (argcount < 2) {
				fprintf(stderr, "'%s' requires sub arguments\n", subcmd[0]);
				continue;
			}
			if (parse_signing_options(argcount, subcmd, &op, &sign, input_fname, output_fname) < 0) {
				fprintf(stderr, "invalid command line arguments for signature generation\n");
				continue;
			}
			if (execute_scsa_command(&scsa_app, TA_SECURE_CRYPTO_SIGN, &op) != TEEC_SUCCESS)
				continue;
			print_signature(sign.sign_algo, sign.signature, output_fname);
			if (sign.data)
				free(sign.data);
			if (sign.signature)
				free(sign.signature);
		} else if (strcmp(cmd, "verify") == 0) {
			memset(&sign, 0x0, sizeof(sign));
			subcmd[argcount++] = cmd;
			while (cmd = strtok(NULL, " ")) {
				subcmd[argcount++] = cmd;
			}
			if (argcount < 2) {
				fprintf(stderr, "'%s' requires sub arguments\n", subcmd[0]);
				continue;
			}
			if (parse_verify_options(argcount, subcmd, &op, &verify, input_fname, output_fname) < 0) {
				fprintf(stderr, "invalid command line arguments for signature verification\n");
				continue;
			}
			if (execute_scsa_command(&scsa_app, TA_SECURE_CRYPTO_VERIFY, &op) != TEEC_SUCCESS)
				continue;
			if (verify.data)
				free(verify.data);
		} else if (strcmp(cmd, "wrap") == 0) {
			memset(&wrap, 0x0, sizeof(wrap));
			subcmd[argcount++] = cmd;
			while (cmd = strtok(NULL, " ")) {
				subcmd[argcount++] = cmd;
			}
			if (argcount < 2) {
				fprintf(stderr, "'%s' requires sub arguments\n", subcmd[0]);
				continue;
			}
			if (parse_wrap_options(argcount, subcmd, &op, &wrap, output_fname) < 0) {
				fprintf(stderr, "invalid command line arguments for wrap\n");
				continue;
			}
			if (execute_scsa_command(&scsa_app, TA_SECURE_CRYPTO_WRAP, &op) != TEEC_SUCCESS)
				continue;
			print_wrap_data(&wrap, output_fname);
			if (wrap.key)
				free(wrap.key);
			if (wrap.input)
				free(wrap.input);
			if (wrap.output)
				free(wrap.output);
		} else if (strcmp(cmd, "unwrap") == 0) {
			memset(&unwrap, 0x0, sizeof(unwrap));
			subcmd[argcount++] = cmd;
			while (cmd = strtok(NULL, " ")) {
				subcmd[argcount++] = cmd;
			}
			if (argcount < 2) {
				fprintf(stderr, "'%s' requires sub arguments\n", subcmd[0]);
				continue;
			}
			if (parse_unwrap_options(argcount, subcmd, &op, &unwrap) < 0) {
				fprintf(stderr, "invalid command line arguments for unwrap\n");
				continue;
			}
			if (execute_scsa_command(&scsa_app, TA_SECURE_CRYPTO_UNWRAP, &op) != TEEC_SUCCESS)
				continue;
			print_unwrap_data(&unwrap);
			if (unwrap.key)
				free(unwrap.key);
			if (unwrap.input)
				free(unwrap.input);
			if (unwrap.output)
				free(unwrap.output);
		} else if (strcmp(cmd, "macgen") == 0) {
			memset(&macgen, 0x0, sizeof(macgen));
			subcmd[argcount++] = cmd;
			while (cmd = strtok(NULL, " ")) {
				subcmd[argcount++] = cmd;
			}
			if (argcount < 2) {
				fprintf(stderr, "'%s' requires sub arguments\n", subcmd[0]);
				continue;
			}
			if (parse_macgen_options(argcount, subcmd, &op, &macgen) < 0) {
				fprintf(stderr, "invalid command line arguments for macgen\n");
				continue;
			}
			if (execute_scsa_command(&scsa_app, TA_SECURE_CRYPTO_GEN_MAC_SINGLE, &op) != TEEC_SUCCESS)
				continue;
			print_macgen_output(&macgen);
			if (macgen.input)
				free(macgen.input);
			if (macgen.output)
				free(macgen.output);
			if (macgen.mac_key)
				free(macgen.mac_key);
		} else {
			fprintf(stderr, "'%s' is not supported\n", cmd);
			continue;
		}
	}
	exit_scsa_session(&scsa_app);

	printf("SCSA client application exiting!\n");
	return 0;
}
