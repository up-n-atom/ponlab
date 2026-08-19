#include <string.h>
#include <strings.h>
#include <errno.h>
#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <fcntl.h>

#include "pkcs11types.h"
#include "defs.h"
#include "host_defs.h"
#include "h_extern.h"
#include "tok_struct.h"
#include "mxl_specific.h"
#include "trace.h"

const char manuf[] = "Maxlinear";
const char model[] = "URX";
const char descr[] = "Maxlinear Token";
const char label[] = "mxl-tok";

struct app_ctx_s scsa_app = {
    .context = {0},
    .session = {0},
};

static struct app_ctx_s *scsa_app_ctx = NULL;
static uint64_t ss_handle = -1ULL;

CK_RV mxl_rng(CK_BYTE *output, CK_ULONG bytes)
{
    int ranfd;
    int rlen;
    unsigned int totallen = 0;

    ranfd = open("/dev/prandom", 0);
    if (ranfd < 0)
        ranfd = open("/dev/urandom", 0);
    if (ranfd >= 0) {
        do {
            rlen = read(ranfd, output + totallen, bytes - totallen);
            if (rlen <= 0) {
                close(ranfd);
                return CKR_FUNCTION_FAILED;
            }
            totallen += rlen;
        } while (totallen < bytes);
        close(ranfd);
        return CKR_OK;
    }

    return CKR_FUNCTION_FAILED;
}
static const MECH_LIST_ELEMENT mxl_mech_list[] = {
    {CKM_ECDSA_SHA1, {512, 4096, CKF_HW | CKF_SIGN | CKF_VERIFY | CKF_EC_F_P |
      CKF_EC_NAMEDCURVE | CKF_EC_UNCOMPRESS}},
    {CKM_ECDSA_SHA224, {512, 4096, CKF_HW | CKF_SIGN | CKF_VERIFY | CKF_EC_F_P |
      CKF_EC_NAMEDCURVE | CKF_EC_UNCOMPRESS}},
    {CKM_ECDSA_SHA256, {512, 4096, CKF_HW | CKF_SIGN | CKF_VERIFY | CKF_EC_F_P |
      CKF_EC_NAMEDCURVE | CKF_EC_UNCOMPRESS}},
    {CKM_ECDSA_SHA384, {512, 4096, CKF_HW | CKF_SIGN | CKF_VERIFY | CKF_EC_F_P |
      CKF_EC_NAMEDCURVE | CKF_EC_UNCOMPRESS}},
    {CKM_ECDSA_SHA512, {512, 4096, CKF_HW | CKF_SIGN | CKF_VERIFY | CKF_EC_F_P |
      CKF_EC_NAMEDCURVE | CKF_EC_UNCOMPRESS}},
    {CKM_ECDSA, {160, 521, CKF_HW | CKF_SIGN | CKF_VERIFY | CKF_EC_F_P |
      CKF_EC_NAMEDCURVE | CKF_EC_UNCOMPRESS}},
    {CKM_EC_KEY_PAIR_GEN, {160, 521, CKF_HW | CKF_GENERATE_KEY_PAIR | CKF_EC_F_P |
      CKF_EC_NAMEDCURVE | CKF_EC_UNCOMPRESS}},
    {CKM_SHA224_RSA_PKCS, {1024, 3072, CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY}},
    {CKM_SHA256_RSA_PKCS, {1024, 3072, CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY}},
    {CKM_SHA384_RSA_PKCS, {1024, 3072, CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY}},
    {CKM_SHA512_RSA_PKCS, {1024, 3072, CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY}},
    {CKM_SHA1_RSA_PKCS_PSS, {1024, 3072, CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY}},
    {CKM_SHA256_RSA_PKCS_PSS, {1024, 3072, CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY}},
    {CKM_SHA384_RSA_PKCS_PSS, {1024, 3072, CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY}},
    {CKM_SHA512_RSA_PKCS_PSS, {1024, 3072, CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY}},
    {CKM_RSA_PKCS, {1024, 3072, CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY}},
    {CKM_RSA_PKCS_PSS, {1024, 3072, CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY}},
    {CKM_RSA_X_509, {1024, 3072, CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY}},
    {CKM_RSA_PKCS_OAEP,{1024, 3072, CKF_ENCRYPT | CKF_DECRYPT | CKF_WRAP | CKF_UNWRAP}},
};

static const CK_ULONG mxl_mech_list_len =
                (sizeof(mxl_mech_list) / sizeof(MECH_LIST_ELEMENT));

/*
 * Called during C_Initialize.
 */
CK_RV mxl_tok_init(STDLL_TokData_t * tokdata, CK_SLOT_ID slot_id,
                   char *conf_name)
{
    CK_RV rc;
    UNUSED(conf_name);
    TRACE_INFO("MXL %s slot=%lu running\n", __func__, slot_id);

    /* Check Slot ID */
    if (slot_id >= NUMBER_SLOTS_MANAGED) {
        TRACE_ERROR("Invalid slot ID: %lu\n", slot_id);
        return CKR_FUNCTION_FAILED;
    }

    rc = ock_generic_filter_mechanism_list(tokdata,
                                           mxl_mech_list,
                                           mxl_mech_list_len,
                                           &(tokdata->mech_list),
                                           &(tokdata->mech_list_len));
    if (rc != CKR_OK) {
        TRACE_ERROR("Mechanism filtering failed!  rc = 0x%lx\n", rc);
        return rc;
    }

    return rc;
}

/*
 * @brief  set the object config received from command line
 * @param
 *  pxSstConfig pointer to config structure to be filled
 *  unObjectConfig object config received from command line
 * @return
 *  void
 */
void set_object_config(sst_obj_config_t *pxSstConfig, uint32_t unObjectConfig)
{
    /* Fill the policy access permission flag */
    pxSstConfig->access_perm.u.field.user = unObjectConfig & 3;  /* rw for user */
    unObjectConfig = unObjectConfig >> 2;
    pxSstConfig->access_perm.u.field.group = unObjectConfig & 3; /* rw for group */
    unObjectConfig = unObjectConfig >> 2;
    pxSstConfig->access_perm.u.field.others = unObjectConfig & 3; /* access for others */
    unObjectConfig = unObjectConfig >> 2;
    pxSstConfig->access_perm.u.field.pname = unObjectConfig & 3;   /* rw for user */
    unObjectConfig = unObjectConfig >> 2;

    /* fill policy attribute members*/
    pxSstConfig->policy_attr.u.field.lock = unObjectConfig & 1;
    unObjectConfig = unObjectConfig >> 1;
    pxSstConfig->policy_attr.u.field.no_load_to_userspace = unObjectConfig & 1;
    unObjectConfig = unObjectConfig >> 1;
    pxSstConfig->policy_attr.u.field.read_once = unObjectConfig & 1;
    unObjectConfig = unObjectConfig >> 1;
    pxSstConfig->policy_attr.u.field.ignore_uid = unObjectConfig & 1;
    unObjectConfig = unObjectConfig >> 1;
    pxSstConfig->policy_attr.u.field.ignore_gid = unObjectConfig & 1;
    unObjectConfig = unObjectConfig >> 1;
    pxSstConfig->policy_attr.u.field.ignore_pname = unObjectConfig & 1;
    unObjectConfig = unObjectConfig >> 1;
    pxSstConfig->policy_attr.u.field.wrap_flag = unObjectConfig & 3;
    unObjectConfig = unObjectConfig >> 2;
    /* Admin/Normal node access */
    pxSstConfig->policy_attr.u.field.admin_store = unObjectConfig & 1;
    unObjectConfig = unObjectConfig >> 1;
    //pxSstConfig->policy_attr.u.field.tee_only  = unObjectConfig & 1;
    unObjectConfig = unObjectConfig >> 1;
    pxSstConfig->policy_attr.u.field.reserve  = unObjectConfig & 0x3F;
    unObjectConfig = unObjectConfig >> 6;

    pxSstConfig->crypto_mode_flag = unObjectConfig & 0xFF;
}

int mxl_init_scsa_session(void)
{
    TEEC_Result result = 0;
    uint32_t origin = 0;
    uint32_t login_type = TEEC_LOGIN_USER;
    TEEC_UUID uuid = TA_SECURE_CRYPTO_UUID; 
    struct app_ctx_s *ctx = &scsa_app;
    if (scsa_app_ctx != NULL) {
        return TEEC_SUCCESS;
    }
    result = TEEC_InitializeContext(NULL, &ctx->context);
    if (result != TEEC_SUCCESS) {
        TRACE_ERROR("Initialize the context with TEEC failed.. \n");
        return result;
    }

    result = TEEC_OpenSession(&ctx->context, &ctx->session, &uuid,
                                login_type, NULL, NULL, &origin);

    if (result != TEEC_SUCCESS) {
        TRACE_ERROR("opening session with TEEC failed.. \n");
    }
    scsa_app_ctx = ctx;
    return result;
}

int mxl_execute_scsa_command(uint32_t cmd, TEEC_Operation *op)
{
    TEEC_Result result = 0;
    uint32_t origin = 0;
    struct app_ctx_s *ctx = scsa_app_ctx;
    if (ctx == NULL) {
        return TEEC_ERROR_BAD_PARAMETERS;
    }
    result = TEEC_InvokeCommand(&ctx->session, cmd, op, &origin);
    return result;
}

void mxl_exit_scsa_session(void)
{
    struct app_ctx_s *ctx = scsa_app_ctx;
    if (ctx == NULL)
        return;
    TEEC_CloseSession(&ctx->session);
    TEEC_FinalizeContext(&ctx->context);
    scsa_app_ctx = NULL;
}

uint64_t mxl_securestore_open(void)
{
    uint32_t sst_config = SST_CONFIG;
    struct secure_storage_params sst_params;
    char fname[MXL_PATH_MAX];
    if (ss_handle != -1ULL) {
        return ss_handle;
    }
    sprintf(fname, "%s", OBJECT_INDEX_FILE_NAME );
    if (mxl_fill_secure_storage_details(fname, sst_config, &sst_params) < 0)
        return -1;
    ss_handle = sst_params.handle;
    return sst_params.handle;

}

void mxl_securestore_close(void)
{
    if (ss_handle != -1ULL) {
        securestore_close(ss_handle);
        ss_handle = -1ULL;
    }
}

void mxl_securestore_delete(void)
{
    if (ss_handle != -1ULL) {
        securestore_delete(ss_handle);
        ss_handle = -1ULL;
    }
}

void mxl_securestore_create_keypair_object_name(char *key_name, uint32_t key_id)
{
    memcpy(key_name, MXL_SECURE_STORAGE_HANDLE_PRIFIX, 8);
    sprintf(key_name + MXL_SECURE_STORAGE_NAME_OFFSET, "%08X", key_id);
}

int mxl_securestore_delete_keypair(uint32_t key_id)
{
    char fname[MXL_KEYLENGTH_MAX] = {0};
    uint32_t sst_config = SST_CONFIG;
    struct secure_storage_params sst_params;
    if (!key_id)
        return 0;

    mxl_securestore_create_keypair_object_name(fname, key_id);
    if (mxl_fill_secure_storage_details(fname, sst_config, &sst_params) < 0)
        return -1;
    if (securestore_delete(sst_params.handle) < 0)
        return -1;
    return 0;

}

int mxl_securestore_write_key(uint32_t key_id, uint8_t *key_data, uint32_t key_size)
{
    char fname[MXL_KEYLENGTH_MAX] = {0};
    uint32_t sst_config = TEE_ONLY_SST_CONFIG;
    struct secure_storage_params sst_params;
    int ret;

    mxl_securestore_create_keypair_object_name(fname, key_id);
    if (mxl_fill_secure_storage_details(fname, sst_config, &sst_params) < 0)
        return -1;
    ret = securestore_save(sst_params.handle, NULL, key_data, key_size);
    securestore_close(sst_params.handle);
    if (ret < 0) {
        TRACE_ERROR("securestore_save failed with error ret = %d \n",ret);
        return -1;
    }
    return 0;

}

int mxl_securestore_load(void *buff, uint32_t len, uint32_t *ret_len)
{
    int ret;
    if ((buff == NULL) || (len == 0) || (ret_len == NULL)) {
        TRACE_ERROR(" mxl_securestore_load failed with wrong input parameters \n");
        return -1;
    }
    *ret_len = 0;
    ret = securestore_retrieve(ss_handle, NULL, (unsigned char *)buff, len, ret_len);
    if (ret < 0) {
        TRACE_ERROR("securestore_retrieve failed with error ret = %d \n",ret);
        return -1;
    }
    return 0;

}

int mxl_securestore_save(void *buff, uint32_t len)
{
    int ret;
    if ((buff == NULL) || (len == 0)) {
        TRACE_ERROR(" mxl_securestore_save failed with wrong input parameters \n");
        return -1;
    }
    ret = securestore_save(ss_handle, NULL, (unsigned char *)buff, len);
    if (ret < 0) {
        TRACE_ERROR("securestore_save failed with error ret = %d \n",ret);
        return -1;
    }
    return 0;
}

int mxl_fill_secure_storage_details(const char *name, uint32_t config,
                struct secure_storage_params *sst_params)
{
    sst_obj_config_t sst_config = {0};
    sshandle_t sst_handle = -1;
    int ret = 0;

    sst_config.access_perm.u.perms = config & 0xFF;
    sst_config.policy_attr.u.attr = (config >> 8) & 0xFFFF;
    sst_config.crypto_mode_flag = (sst_crypto_mode_flag_t)(config >> 24) & 0xFF;
    //set_object_config(&sst_config, config);

    ret = securestore_create_open(name, &sst_config, 0, &sst_handle);
    if (ret < 0) {
        ret = securestore_create_open(name, &sst_config, SS_CREATE, &sst_handle);
    	if (ret < 0) {
            TRACE_ERROR("securestore_create_open failed to create the key file: %d \n", ret);
            return -1;
	}
    }

    sst_params->handle = sst_handle;
    sst_params->access_perm = sst_config.access_perm.u.perms;
    sst_params->policy_attr = sst_config.policy_attr.u.attr;
    sst_params->crypto_mode_flag = (uint8_t)sst_config.crypto_mode_flag;
    return 0;
}

int mxl_prepare_generate_key_pair(const char *object_name, enum sec_alg algo, struct seccrypto_gen_key *genkey, TEEC_Operation *op)
{
    
    uint32_t sst_config =  TEE_ONLY_SST_CONFIG;
    struct secure_storage_params *sst_params = &genkey->sst_params;

    genkey->genkey_algo = algo;
    //genkey->flag = SEC_CRYPTO_RETURN_KEY_NONE;
    genkey->flag = SEC_CRYPTO_RETURN_KEY_PAIR_PLAIN;

    if (mxl_fill_secure_storage_details(object_name, sst_config, sst_params) < 0)
        return -1;

    if ((genkey->genkey_algo != SEC_ALG_ECDSA_P256) && (genkey->genkey_algo != SEC_ALG_ECDSA_P384)) {
        return -1;
    }

    if ((genkey->flag < SEC_CRYPTO_RETURN_KEY_PAIR) || (genkey->flag > SEC_CRYPTO_RETURN_KEY_NONE)) {
        return -1;
    }

    if (genkey->flag != SEC_CRYPTO_RETURN_KEY_NONE) {
        genkey->key_ptr = calloc(1, MAX_BUF_SIZE);
        genkey->key_len = MAX_BUF_SIZE;
    }

    op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op->params[0].tmpref.buffer = genkey;
    op->params[0].tmpref.size = sizeof(*genkey);

    return 0;
}

int mxl_prepare_pin_info(seccrypto_pin_info_t *pin_info,
			CK_USER_TYPE userType, CK_CHAR_PTR pPin,
			CK_ULONG ulPinLen, TEEC_Operation *op)
{
    pin_info->user_type = userType;
    pin_info->pin_hash_algo = 0;
    pin_info->pin_len = ulPinLen;
    pin_info->pin = pPin;

    op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op->params[0].tmpref.buffer = pin_info;
    op->params[0].tmpref.size = sizeof(*pin_info);

    return 0;
}

int mxl_prepare_pin_set_info(seccrypto_pin_set_info_t *pin_info,
			CK_USER_TYPE userType, CK_CHAR_PTR pPin, CK_ULONG ulPinLen,
			CK_CHAR_PTR pNewPin, CK_ULONG ulNewPinLen, TEEC_Operation *op)
{
    pin_info->old_pin_info.user_type = userType;
    pin_info->old_pin_info.pin_hash_algo = 0;
    pin_info->old_pin_info.pin_len = ulPinLen;
    pin_info->old_pin_info.pin = pPin;

    pin_info->new_pin_len = ulNewPinLen;
    pin_info->new_pin = pNewPin;

    op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op->params[0].tmpref.buffer = pin_info;
    op->params[0].tmpref.size = sizeof(*pin_info);

    return 0;
}

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
        case SEC_ALG_RSA_1024:
            sign_len = 1024/8;
            break;
        case SEC_ALG_ECDSA_P256:
            sign_len = 256/8;
            break;
        case SEC_ALG_ECDSA_P384:
            sign_len = 384/8;
            break;
        default:
            sign_len = 0;
            break;
    }
    return sign_len;

}

int mxl_prepare_generate_sign(struct seccrypto_sign_param *sign, char *input, int input_len, TEEC_Operation *op)
{
    uint8_t *buffer = NULL;
    sign->hash_flags = SC_FLAG_FINAL;
    if ((input == NULL) || (input_len == 0)) {
        TRACE_ERROR("sign: Wrong input parameters\n");
        return -1;
    }

    sign->data = (uint8_t *)input;
    sign->data_len = input_len;

    if ((sign->hash_algo < RSA_PKCS1_5_SHA1) || (sign->hash_algo >  ECDSA_ASN1_SHA384)) {
        TRACE_ERROR("sign: hash algorithm is not supported\n");
        return -1;
    }

    if ((sign->sign_algo == SEC_ALG_RSA_2048) || (sign->sign_algo == SEC_ALG_RSA_1024)
                        || (sign->sign_algo == SEC_ALG_RSA_3072)) {
        sign->sign_len = sizeof(*sign);
        buffer = calloc(1, sizeof(struct rsa_signature) + signature_len(sign->sign_algo));
    }
    else if ((sign->sign_algo == SEC_ALG_ECDSA_P256) || (sign->sign_algo == SEC_ALG_ECDSA_P384)) {
        sign->sign_len = sizeof(*sign);
        buffer = calloc(1, sizeof(struct ecdsa_signature) + signature_len(sign->sign_algo) * 2);
    }
    else {
        TRACE_ERROR("sign: algorithm is not supported\n");
        return -1;
    }

    sign->signature = buffer;
    op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op->params[0].tmpref.buffer = sign;
    op->params[0].tmpref.size = sizeof(*sign) + sign->sign_len + signature_len(sign->sign_algo);
    return 0;
}

int mxl_prepare_sign_verify(struct seccrypto_verify_param *verify, char *input, int input_len,  uint8_t *signature, TEEC_Operation *op)
{
    verify->hash_flags = SC_FLAG_FINAL;
    if ((input == NULL) || (input_len == 0)) {
        TRACE_ERROR("verify: Wrong input parameters\n");
        return -1;
    }

    verify->data = (uint8_t *)input;
    verify->data_len = input_len;

    if ((verify->hash_algo < RSA_PKCS1_5_SHA1) || (verify->hash_algo > ECDSA_ASN1_SHA384)) {
        TRACE_ERROR("verify: algorithm is not supported\n");
        return -1;
    }

    if ((verify->sign_algo == SEC_ALG_RSA_2048) || (verify->sign_algo == SEC_ALG_RSA_1024)
                        || (verify->sign_algo == SEC_ALG_RSA_3072)) {
        verify->sign_len = sizeof(struct rsa_signature);
    } else if ((verify->sign_algo == SEC_ALG_ECDSA_P256) || (verify->sign_algo == SEC_ALG_ECDSA_P384)) {
        verify->sign_len = sizeof(struct ecdsa_signature);
    }

    verify->signature = signature;
    op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op->params[0].tmpref.buffer = verify;
    op->params[0].tmpref.size = sizeof(*verify);
    return 0;
}
int mxl_prepare_generate_sign_digest(struct seccrypto_sign_digest_param *sign_digest, char *input, int input_len, TEEC_Operation *op)
{
    uint8_t *buffer = NULL;
    if ((input == NULL) || (input_len == 0)) {
        TRACE_ERROR("sign: Wrong input parameters\n");
        return -1;
    }

    sign_digest->digest = (uint8_t *)input;
    sign_digest->digest_len = input_len;

    if ((sign_digest->hash_algo < RSA_PKCS1_5_SHA1) || (sign_digest->hash_algo > ECDSA_ASN1_SHA384)) {
        TRACE_ERROR("sign: hash algorithm is not supported\n");
        return -1;
    }

    if ((sign_digest->sign_algo == SEC_ALG_RSA_2048) || (sign_digest->sign_algo == SEC_ALG_RSA_1024)
                       || (sign_digest->sign_algo == SEC_ALG_RSA_3072)) {
        sign_digest->sign_len = sizeof(*sign_digest);
        buffer = calloc(1, sizeof(struct rsa_signature) + signature_len(sign_digest->sign_algo));
    }
    else if ((sign_digest->sign_algo == SEC_ALG_ECDSA_P256) || (sign_digest->sign_algo == SEC_ALG_ECDSA_P384)) {
        sign_digest->sign_len = sizeof(*sign_digest);
        buffer = calloc(1, sizeof(struct ecdsa_signature) + signature_len(sign_digest->sign_algo) * 2);
    }
    else {
        TRACE_ERROR("sign: algorithm is not supported\n");
        return -1;
    }

    sign_digest->signature = buffer;
    op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op->params[0].tmpref.buffer = sign_digest;
    op->params[0].tmpref.size = sizeof(*sign_digest) + sign_digest->sign_len + signature_len(sign_digest->sign_algo);
    return 0;
}
int mxl_prepare_digest_verify(struct seccrypto_digest_verify_param *digest_verify, char *input, int input_len,
                              uint8_t *signature, TEEC_Operation *op)
{
    if ((input == NULL) || (input_len == 0)) {
        TRACE_ERROR("verify: Wrong input parameters\n");
        return -1;
    }

    digest_verify->digest = (uint8_t *)input;
    digest_verify->digest_len = input_len;

    if ((digest_verify->hash_algo < RSA_PKCS1_5_SHA1) || (digest_verify->hash_algo > ECDSA_ASN1_SHA384)) {
        TRACE_ERROR("vefify: hash algorithm is not supported\n");
        return -1;
    }

    if ((digest_verify->sign_algo == SEC_ALG_RSA_2048) || (digest_verify->sign_algo == SEC_ALG_RSA_1024)
                        || (digest_verify->sign_algo == SEC_ALG_RSA_3072)) {
        digest_verify->sign_len = sizeof(struct rsa_signature);
    }
    else if ((digest_verify->sign_algo == SEC_ALG_ECDSA_P256) || (digest_verify->sign_algo == SEC_ALG_ECDSA_P384)) {
        digest_verify->sign_len = sizeof(struct ecdsa_signature);
    }
    else {
        TRACE_ERROR("verify: sign algorithm is not supported\n");
        return -1;
    }

    digest_verify->signature = signature;
    op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op->params[0].tmpref.buffer = digest_verify;
    op->params[0].tmpref.size = sizeof(*digest_verify);
    return 0;
}

int mxl_prepare_rsa_decrypt(struct seccrypto_asym_enc_dec *rsadec, char *input, int input_len, TEEC_Operation *op)
{
    if ((input == NULL) || (input_len == 0)) {
        TRACE_ERROR("rsadec: Wrong input parameters\n");
        return -1;
    }

    if ((rsadec->hash_alg < ASYM_ENC_HASH_SHA1) || (rsadec->hash_alg > ASYM_ENC_HASH_SHA512)) {
        TRACE_ERROR("rsadec: hash algo is invalid\n");
        return -1;
    }

    if ((rsadec->encod_schm < RSA_ENCOD_OAEP) || (rsadec->encod_schm > RSA_ENCOD_PKCS1_V1_5)) {
        TRACE_ERROR("rsadec: encoding scheme is invalid\n");
        return -1;
    }

    if ((rsadec->asym_flags < ASYM_FLAG_OAEP_ADDNL_INPUT_HASH) || (rsadec->asym_flags > ASYM_FLAG_OAEP_ADDNL_INPUT_DATA)) {
        TRACE_ERROR("rsadec: crypto flag is invalid\n");
        return -1;
    }

    rsadec->input_data_len = input_len;
    rsadec->input_data = input;

    rsadec->output_data_len = MAX_RSA_DECRYPT_LEN;
    rsadec->output_data = calloc(1, rsadec->output_data_len);
    if (!rsadec->output_data) {
        TRACE_ERROR( "rsadec: calloc failed\n");
        return -1;
    }

    op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op->params[0].tmpref.buffer = rsadec;
    op->params[0].tmpref.size = sizeof(*rsadec);
    return 0;
}

int mxl_prepare_load_key(char *object_name, struct seccrypto_load_key *loadkey, TEEC_Operation *op)
{
    uint32_t sst_config = SST_CONFIG;
    struct secure_storage_params *sst_params = &loadkey->sst_params;
    if ((loadkey->hash_algo < RSA_PKCS1_5_SHA1) || (loadkey->hash_algo > ECDSA_ASN1_SHA384)) {
        TRACE_ERROR("load: hash algorithm is not supported\n");
        return -1;
    }

    if (loadkey->load_algo == SEC_ALG_RSA_4096) {
        TRACE_ERROR("load: sign algorithm is not supported\n");
        return -1;
    }

    if (mxl_fill_secure_storage_details(object_name, sst_config, sst_params) < 0)
        return -1;

    op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op->params[0].tmpref.buffer = loadkey;
    op->params[0].tmpref.size = sizeof(*loadkey);
    return 0;
}

int mxl_prepare_set_attribute(char *object_name, struct seccrypto_set_attribute *attribute, TEEC_Operation *op)
{
    uint32_t sst_config = SST_CONFIG;
    struct secure_storage_params *sst_params = &attribute->sst_params;

    if (mxl_fill_secure_storage_details(object_name, sst_config, sst_params) < 0)
        return -1;

    op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op->params[0].tmpref.buffer = attribute;
    op->params[0].tmpref.size = sizeof(*attribute);
    return 0;
}
