#ifndef MXL_COMMON_H
#define MXL_COMMON_H

#include <linux/mxltee.h>
#include <tee_client_api.h>
#include <fapi_sec_storage.h>

/*! Sample UUID for Secure crypto service Trusted agent */
#define TA_SECURE_CRYPTO_UUID   {0xFFFF0000, 0x0001, 0x0002, \
                {0x0, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07}}

#define MAX_BUF_SIZE            1024
#define MXL_PATH_MAX 1024
#define SST_CONFIG 0x10020CF
#define TEE_ONLY_SST_CONFIG 0x10220CF
#define OBJECT_INDEX_FILE_NAME "pkcs_object_index"
#define CKA_MXL_SECURE_STORAGE_HANDLE CKA_IBM_USE_AS_DATA
#define CKA_MXL_PKCS8_KEY_HANDLE CKA_IBM_MACKEY
#define CKA_MXL_KEY_TYPE CKA_IBM_KEYTYPE
#define MXL_SECURE_STORAGE_HANDLE_PRIFIX "pkcs_obj"
#define MXL_SECURE_STORAGE_NAME_LENGTH 4
#define MXL_SECURE_STORAGE_NAME_OFFSET 8
#define MXL_KEYLENGTH_MAX 20
#define MXL_PK_DIR "pkcs_store"
#define NVDAT_FILE_NAME   "pkcs_nvdat"
#define MXL_SECURE_STORAGE_MAX_FILE_LEN 0x40000
#define MXL_HEADER_LEN 20
#define MAX_RSA_DECRYPT_LEN 512

struct app_ctx_s {
    TEEC_Context context;
    TEEC_Session session;
};

typedef struct mxl_attribute_s {
    uint32_t type;
    uint32_t len;
} mxl_attribute_t;

CK_RV mxl_rng(CK_BYTE *output, CK_ULONG bytes);
void set_object_config(sst_obj_config_t *pxSstConfig, uint32_t unObjectConfig);
int mxl_init_scsa_session(void);
int mxl_execute_scsa_command(uint32_t cmd, TEEC_Operation *op);
void mxl_exit_scsa_session(void);
uint64_t mxl_securestore_open(void);
void mxl_securestore_close(void);
void mxl_securestore_delete(void);
void mxl_securestore_create_keypair_object_name(char *key_name, uint32_t key_id);
int mxl_securestore_delete_keypair(uint32_t key_id);
int mxl_securestore_write_key(uint32_t key_id, uint8_t *key_data, uint32_t key_size);
int mxl_securestore_load(void *buff, uint32_t len, uint32_t *ret_len);
int mxl_securestore_save(void *buff, uint32_t len);
int mxl_fill_secure_storage_details(const char *name, uint32_t config,
                struct secure_storage_params *sst_params);
int mxl_prepare_generate_key_pair(const char *object_name, enum sec_alg algo, struct seccrypto_gen_key *genkey, TEEC_Operation *op);
int mxl_prepare_generate_sign(struct seccrypto_sign_param *sign, char *input, int input_len, TEEC_Operation *op);
int mxl_prepare_sign_verify(struct seccrypto_verify_param *verify, char *input, int input_len,  uint8_t *signature, TEEC_Operation *op);
int mxl_prepare_generate_sign_digest(struct seccrypto_sign_digest_param *sign_digest, char *input, int input_len, TEEC_Operation *op);
int mxl_prepare_digest_verify(struct seccrypto_digest_verify_param *verify, char *input, int input_len,  uint8_t *signature, TEEC_Operation *op);
int mxl_prepare_rsa_decrypt(struct seccrypto_asym_enc_dec *rsadec, char *input, int input_len, TEEC_Operation *op);
int mxl_prepare_load_key(char *object_name, struct seccrypto_load_key *loadkey, TEEC_Operation *op);
int mxl_prepare_pin_info(seccrypto_pin_info_t *pin_info,
			CK_USER_TYPE userType, CK_CHAR_PTR pPin,
			CK_ULONG ulPinLen, TEEC_Operation *op);
int mxl_prepare_pin_set_info(seccrypto_pin_set_info_t *pin_info,
			CK_USER_TYPE userType, CK_CHAR_PTR pPin, CK_ULONG ulPinLen,
			CK_CHAR_PTR pNewPin, CK_ULONG ulNewPinLen, TEEC_Operation *op);
int mxl_prepare_set_attribute(char *object_name, struct seccrypto_set_attribute *attribute, TEEC_Operation *op);
#endif

