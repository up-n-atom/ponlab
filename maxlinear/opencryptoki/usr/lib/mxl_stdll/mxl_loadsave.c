#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <syslog.h>
#include <openssl/evp.h>
#include "platform.h"
#include "pkcs11types.h"
#include "defs.h"
#include "host_defs.h"
#include "h_extern.h"
#include "trace.h"
#include "ock_syslog.h"
#include "slotmgr.h" // for ock_snprintf
#include <fapi_sec_storage.h>
#include "mxl_specific.h"
#include "mxl_loadsave.h"

#define NVDAT_INVALID_HANDLE 0xFFFFFFFF
extern const char label[];
static sshandle_t mxl_nvdat_handle = NVDAT_INVALID_HANDLE;
static unsigned char mxl_nvdat_data[sizeof(TOKEN_DATA)] = {0};
int mxl_nvdat_open(void)
{
    int ret, rc=0;
    char fname[MXL_PATH_MAX];
    sst_obj_config_t xSstConfig = {0};
    sprintf(fname, "%s", NVDAT_FILE_NAME);
    if(mxl_nvdat_handle != NVDAT_INVALID_HANDLE) {
        return 0;
    }
    set_object_config(&xSstConfig, SST_CONFIG);
    ret = securestore_create_open(fname, &xSstConfig, 0, &mxl_nvdat_handle);
    if(ret < 0) {
        ret = securestore_create_open(fname, &xSstConfig, SS_CREATE, &mxl_nvdat_handle);
        rc = 1;
    }
    if(ret < 0) {
       TRACE_ERROR("securestore_create_open failed with error:%d\n",ret);
       return -1;
    }
    return rc;

}

void mxl_nvdat_close(void)
{
    if(mxl_nvdat_handle != NVDAT_INVALID_HANDLE) {
        securestore_close(mxl_nvdat_handle);
        mxl_nvdat_handle = NVDAT_INVALID_HANDLE;
    }
}

static int mxl_nvdat_save(unsigned char *buf, uint32_t buflen)
{
    int ret;
    if(buflen == sizeof(TOKEN_DATA)) {
        if(!memcmp(buf, mxl_nvdat_data, buflen)) {
            return 0;
        }
    }
    ret = securestore_save(mxl_nvdat_handle, NULL, buf, buflen);
    if(ret >= 0) {
        memcpy(mxl_nvdat_data, buf, buflen);
    }
    return ret;
}

static int mxl_nvdat_retrieve(unsigned char *buf, uint32_t buflen, uint32_t *ret_len)
{
    int ret;
    ret = securestore_retrieve(mxl_nvdat_handle, NULL, buf, buflen, ret_len);
    if(ret >= 0) {
        memcpy(mxl_nvdat_data, buf, buflen);
    }
    return ret;
}

CK_RV mxl_save_token_data(STDLL_TokData_t *tokdata, CK_SLOT_ID slot_id)
{
    int ret;
    TOKEN_DATA td;
    CK_RV rc;
    UNUSED(slot_id);

    ret = mxl_nvdat_open();
    if (ret < 0) {
        TRACE_ERROR("open_token_nvdat with error: %d \n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }

    /* Write generic token data */
    memcpy(&td, tokdata->nv_token_data, sizeof(TOKEN_DATA));

    td.token_info.flags = htobe32(td.token_info.flags);
    td.token_info.ulMaxSessionCount = htobe32(td.token_info.ulMaxSessionCount);
    td.token_info.ulSessionCount = htobe32(td.token_info.ulSessionCount);
    td.token_info.ulMaxRwSessionCount
      = htobe32(td.token_info.ulMaxRwSessionCount);
    td.token_info.ulRwSessionCount = htobe32(td.token_info.ulRwSessionCount);
    td.token_info.ulMaxPinLen = htobe32(td.token_info.ulMaxPinLen);
    td.token_info.ulMinPinLen = htobe32(td.token_info.ulMinPinLen);
    td.token_info.ulTotalPublicMemory
      = htobe32(td.token_info.ulTotalPublicMemory);
    td.token_info.ulFreePublicMemory
      = htobe32(td.token_info.ulFreePublicMemory);
    td.token_info.ulTotalPrivateMemory
      = htobe32(td.token_info.ulTotalPrivateMemory);
    td.token_info.ulFreePrivateMemory
      = htobe32(td.token_info.ulFreePrivateMemory);
    td.tweak_vector.allow_weak_des = htobe32(td.tweak_vector.allow_weak_des);
    td.tweak_vector.check_des_parity
      = htobe32(td.tweak_vector.check_des_parity);
    td.tweak_vector.allow_key_mods = htobe32(td.tweak_vector.allow_key_mods);
    td.tweak_vector.netscape_mods = htobe32(td.tweak_vector.netscape_mods);
    td.dat.version = htobe32(td.dat.version);
    td.dat.so_login_it = htobe64(td.dat.so_login_it);
    td.dat.user_login_it = htobe64(td.dat.user_login_it);
    td.dat.so_wrap_it = htobe64(td.dat.so_wrap_it);
    td.dat.user_wrap_it = htobe64(td.dat.user_wrap_it);

    ret = mxl_nvdat_save((unsigned char *)&td, sizeof(TOKEN_DATA));
    mxl_nvdat_close();
    if (ret < 0) {
        TRACE_ERROR("mxl_nvdat_save failed: with error %d !\n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }
    rc = CKR_OK;
done:
    return rc;
}

CK_RV mxl_init_token_data(STDLL_TokData_t *tokdata, CK_SLOT_ID slot_id)
{
    CK_RV rc = CKR_OK;
    TEEC_Operation op ={0};

    memset((char *) tokdata->nv_token_data, 0, sizeof(TOKEN_DATA));

    if (mxl_execute_scsa_command(TA_SECURE_CRYPTO_INIT_PIN, &op) != TEEC_SUCCESS) {
        rc = CKR_FUNCTION_FAILED;
        TRACE_ERROR("TEE execute SCSA command INIT PIN failed with error = %ld \n", rc);
        return rc;
    }
    // generate the master key used for signing the Operation State information
    //                          `
    memset(tokdata->nv_token_data->token_info.label, ' ',
           sizeof(tokdata->nv_token_data->token_info.label));
    memcpy(tokdata->nv_token_data->token_info.label, label,
           strlen(label));

    tokdata->nv_token_data->tweak_vector.allow_weak_des = TRUE;
    tokdata->nv_token_data->tweak_vector.check_des_parity = FALSE;
    tokdata->nv_token_data->tweak_vector.allow_key_mods = TRUE;
    tokdata->nv_token_data->tweak_vector.netscape_mods = TRUE;

    init_tokenInfo(tokdata->nv_token_data);
    tokdata->nv_token_data->token_info.flags |= CKF_TOKEN_INITIALIZED;
    tokdata->nv_token_data->token_info.flags &= ~(CKF_USER_PIN_INITIALIZED |
    CKF_USER_PIN_LOCKED | CKF_USER_PIN_FINAL_TRY |
    CKF_USER_PIN_COUNT_LOW);

    rc = mxl_save_token_data(tokdata, slot_id);

    return rc;
}

CK_RV mxl_load_token_data(STDLL_TokData_t *tokdata, CK_SLOT_ID slot_id)
{
    int ret;
    TOKEN_DATA td;
    CK_RV rc;
    uint32 act_len;
    slot_id = slot_id;

    ret = mxl_nvdat_open();
    if (ret < 0) {
        TRACE_ERROR("mxl_nvdat_open failed with error ret = %d \n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }

    ret = mxl_nvdat_retrieve((unsigned char *) &td, (unsigned int)sizeof(TOKEN_DATA), &act_len);
    if (ret < 0) {
        TRACE_ERROR("mxl_nvdat_retrieve failed with error ret = %d \n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }

    /* data marshalling */
    td.token_info.flags = be32toh(td.token_info.flags);
    td.token_info.ulMaxSessionCount = be32toh(td.token_info.ulMaxSessionCount);
    td.token_info.ulSessionCount = be32toh(td.token_info.ulSessionCount);
    td.token_info.ulMaxRwSessionCount
      = be32toh(td.token_info.ulMaxRwSessionCount);
    td.token_info.ulRwSessionCount = be32toh(td.token_info.ulRwSessionCount);
    td.token_info.ulMaxPinLen = be32toh(td.token_info.ulMaxPinLen);
    td.token_info.ulMinPinLen = be32toh(td.token_info.ulMinPinLen);
    td.token_info.ulTotalPublicMemory
      = be32toh(td.token_info.ulTotalPublicMemory);
    td.token_info.ulFreePublicMemory
      = be32toh(td.token_info.ulFreePublicMemory);
    td.token_info.ulTotalPrivateMemory
      = be32toh(td.token_info.ulTotalPrivateMemory);
    td.token_info.ulFreePrivateMemory
      = be32toh(td.token_info.ulFreePrivateMemory);
    td.tweak_vector.allow_weak_des = be32toh(td.tweak_vector.allow_weak_des);
    td.tweak_vector.check_des_parity
      = be32toh(td.tweak_vector.check_des_parity);
    td.tweak_vector.allow_key_mods = be32toh(td.tweak_vector.allow_key_mods);
    td.tweak_vector.netscape_mods = be32toh(td.tweak_vector.netscape_mods);
    td.dat.version = be32toh(td.dat.version);
    td.dat.so_login_it = be64toh(td.dat.so_login_it);
    td.dat.user_login_it = be64toh(td.dat.user_login_it);
    td.dat.so_wrap_it = be64toh(td.dat.so_wrap_it);
    td.dat.user_wrap_it = be64toh(td.dat.user_wrap_it);

    memcpy(tokdata->nv_token_data, &td, sizeof(TOKEN_DATA));

    TRACE_INFO("max pin len = %d \n", td.token_info.ulMaxPinLen);
    TRACE_INFO("min pin len = %d \n", td.token_info.ulMinPinLen);

    rc = CKR_OK;

done:
    mxl_nvdat_close();
    return rc;
}

CK_RV mxl_save_token_object(STDLL_TokData_t *tokdata, OBJECT *obj)
{
    CK_RV rc;
    rc = mxl_save_token_object_internal(tokdata, obj, object_is_private(obj));
    TRACE_INFO("%s:rc = 0x%ld\n", __func__, rc);
    return rc;
}

CK_RV mxl_save_private_token_object(STDLL_TokData_t *tokdata, OBJECT *obj)
{
    CK_RV rc;
    rc = mxl_save_token_object_internal(tokdata, obj, TRUE);
    TRACE_INFO("%s:rc = 0x%ld\n", __func__, rc);
    return rc;
}

CK_RV mxl_save_public_token_object(STDLL_TokData_t *tokdata, OBJECT *obj)
{
    CK_RV rc;
    rc = mxl_save_token_object_internal(tokdata, obj, FALSE);
    TRACE_INFO("%s:rc = 0x%ld\n", __func__, rc);
    return rc;
}

CK_RV mxl_load_public_token_objects(STDLL_TokData_t *tokdata)
{
    CK_RV rc;
    rc = mxl_load_token_objects(tokdata, FALSE);
    TRACE_INFO("%s:rc = 0x%ld\n", __func__, rc);
    return rc;
}

CK_RV mxl_load_private_token_objects(STDLL_TokData_t *tokdata)
{
    CK_RV rc;
    rc = mxl_load_token_objects(tokdata, TRUE);
    TRACE_INFO("%s:rc = 0x%ld\n", __func__, rc);
    return rc;
}

CK_RV mxl_reload_token_object(STDLL_TokData_t *tokdata, OBJECT *obj)
{
    CK_BYTE *buf = NULL;
    CK_RV rc = CKR_OK;
    int ret;
    char fname[MXL_PATH_MAX];
    uint32_t i_buff_len, ret_len, o_length;
    char *i_buff, *o_buff;

    i_buff_len = MXL_SECURE_STORAGE_MAX_FILE_LEN;
    i_buff = calloc(1,i_buff_len);
    if(i_buff == NULL)
        return CKR_HOST_MEMORY;

    if(mxl_securestore_open() == -1UL) {
        TRACE_ERROR("%s:mxl_securestore_open failed\n", __func__);
        free(i_buff);
        return CKR_FUNCTION_FAILED;
    }

    ret = mxl_securestore_load(i_buff, i_buff_len, &ret_len);
    mxl_securestore_close();
    if((ret != 0) || (ret_len == 0)) {
        free(i_buff);
        TRACE_ERROR("%s:mxl_securestore_load failed with error %d\n", __func__, ret);
        return CKR_KEY_HANDLE_INVALID;

    } else {
        i_buff_len = ret_len;
        o_buff = i_buff;
        while (i_buff_len > 4) {
            memcpy(&o_length, o_buff, 4);
            if(i_buff_len < o_length ) {
                TRACE_ERROR("%s:Wrong Object Length\n", __func__);
                rc = CKR_GENERAL_ERROR;
                break;
            }
            i_buff_len -= o_length;
            if( !memcmp(o_buff+5, obj->name, 8) ) {
                buf = (CK_BYTE *)o_buff + MXL_HEADER_LEN;
                memset(fname,'\0', MXL_PATH_MAX);
                sprintf(fname, "%s/%s/", tokdata->data_store, PK_LITE_OBJ_DIR);
                strncat(fname, o_buff+5, 8);
                rc = object_mgr_restore_obj(tokdata, buf, obj, fname);
                if (rc != CKR_OK) {
                    TRACE_ERROR("object_mgr_restore_obj failed with error:%ld\n",rc);
                }
                break;
            }
            o_buff += o_length;
        }
    }

    if (i_buff)
        free(i_buff);

    TRACE_INFO("%s:rc = 0x%ld\n", __func__, rc);
    return rc;
}

static CK_RV mxl_delete_token_object_internal(OBJECT *obj, char *data, uint32_t len, uint32_t* retlen)
{
    CK_RV rc = CKR_OK;
    uint32_t i_buff_len, o_buff_len, o_length;
    char *i_buff, *o_buff, *buffer;
    buffer = calloc(1, len);
    if(buffer == NULL)
        return CKR_HOST_MEMORY;
    o_buff_len = len;
    i_buff_len = 0;
    o_buff = data;
    i_buff = buffer;
    while (o_buff_len > 4) {
        memcpy(&o_length, o_buff, 4);
        if(o_buff_len < o_length ) {
            TRACE_ERROR("%s:Wrong Object Length\n", __func__);
            rc = CKR_GENERAL_ERROR;
            break;
        }
        if(memcmp(o_buff+5, obj->name, 8)) {
            memcpy(i_buff, o_buff, o_length);
            i_buff_len += o_length;
            i_buff += o_length;
        }
        o_buff += o_length;
        o_buff_len -= o_length;
   }
   if(i_buff_len == len) {
       TRACE_INFO("%s:Object not found\n", __func__);
   } else {
       TRACE_INFO("%s:Object found\n", __func__);
       memcpy(data, buffer, i_buff_len);
       *retlen = i_buff_len;
   }
   free(buffer);
   return rc;
}

CK_RV mxl_save_token_object_internal(STDLL_TokData_t *tokdata, OBJECT *obj, CK_BBOOL is_private)
{
    CK_BYTE *clear = NULL;
    CK_ULONG clear_len;
    CK_RV rc = CKR_OK;
    CK_ULONG_32 total_len, i_buff_len, ret_len, o_len;
    char *data, *i_buff = NULL;
    int ret;
    UNUSED(tokdata);
    i_buff_len = MXL_SECURE_STORAGE_MAX_FILE_LEN;
    i_buff = calloc(1,i_buff_len);
    if(i_buff == NULL)
        return CKR_HOST_MEMORY;

    if(mxl_securestore_open() == -1UL) {
        TRACE_ERROR("%s:mxl_securestore_open failed\n", __func__);
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }

    rc = object_flatten(obj, &clear, &clear_len);
    if (rc != CKR_OK) {
        goto done;
    }
    ret = mxl_securestore_load(i_buff, i_buff_len, &ret_len);
    if(ret != 0)  {
        TRACE_ERROR("%s:mxl_securestore_load failed with error %d\n", __func__, ret);
        rc = CKR_KEY_HANDLE_INVALID;
        goto done;
    }
    rc =  mxl_delete_token_object_internal(obj, i_buff, ret_len, &ret_len);
    if( rc != CKR_OK ) {
        TRACE_ERROR("%s:mxl_delete_token_object failed with error %ld\n", __func__, rc);
        goto done;
    }
    o_len = (CK_ULONG_32)clear_len + MXL_HEADER_LEN;
    total_len = ret_len + o_len ;
    data = i_buff + ret_len;
    memcpy(data, &o_len, 4);
    memcpy(data+4, &is_private, 1);
    memcpy(data+5, obj->name, 8);
    memcpy(data+13, &obj->ex_data_len, 4);
    memset(data+17, 0x0, 3);
    memcpy(data+MXL_HEADER_LEN, clear, clear_len);
    ret = mxl_securestore_save(i_buff, total_len);
    if(ret != 0) {
        TRACE_ERROR("%s:mxl_securestore_save failed with error %d\n", __func__, ret);
        rc = CKR_FUNCTION_FAILED;
    } else {
        rc = CKR_OK;
    }
done:
    mxl_securestore_close();
    if (clear)
        free(clear);
    if (i_buff)
        free(i_buff);
    return rc;
}

CK_RV mxl_load_token_objects(STDLL_TokData_t *tokdata, CK_BBOOL is_private)
{
    CK_BYTE *buf = NULL;
    char fname[MXL_PATH_MAX];
    CK_BBOOL  priv;
    CK_ULONG_32 size;
    CK_RV rc = CKR_OK;
    int ret;
    uint32_t i_buff_len, ret_len, o_length;
    char *i_buff, *o_buff;
    i_buff_len = MXL_SECURE_STORAGE_MAX_FILE_LEN;
    i_buff = calloc(1,i_buff_len);
    if(i_buff == NULL)
        return CKR_HOST_MEMORY;

    if(mxl_securestore_open() == -1UL) {
        TRACE_ERROR("%s:mxl_securestore_open failed\n", __func__);
        free(i_buff);
        return CKR_FUNCTION_FAILED;
    }
    ret = mxl_securestore_load(i_buff, i_buff_len, &ret_len);
    mxl_securestore_close();
    if((ret != 0) || (ret_len == 0)) {
        free(i_buff);
        return CKR_OK;

    } else {
        i_buff_len = ret_len;
        o_buff = i_buff;
        while (i_buff_len > 4) {
            memcpy(&o_length, o_buff, 4);
            if(i_buff_len < o_length ) {
                TRACE_ERROR("%s:Wrong Object Length\n", __func__);
                rc = CKR_GENERAL_ERROR;
                break;
            }

            i_buff_len -= o_length;
            memcpy(&priv, o_buff+4, 1);
            if (priv != is_private) {
                o_buff += o_length;
                continue;
            }

            memset(fname,'\0', MXL_PATH_MAX);
            sprintf(fname, "%s/%s/", tokdata->data_store, PK_LITE_OBJ_DIR);
            strncat(fname, o_buff+5, 8);
            size = o_length - MXL_HEADER_LEN;
            buf = (CK_BYTE *) o_buff + MXL_HEADER_LEN;

            // ... grab object mutex here.
            if (object_mgr_restore_obj_withSize(tokdata,
                                            buf, NULL, size, fname) != CKR_OK) {
                OCK_SYSLOG(LOG_ERR,"Cannot restore token object %s (ignoring it)", fname);
            }
            o_buff += o_length;

        }
    }
    if (i_buff)
        free(i_buff);
    return rc;
}
CK_RV mxl_delete_token_object(STDLL_TokData_t *tokdata, OBJECT *obj)
{
    CK_RV rc = CKR_OK;
    CK_ULONG_32 total_len, i_buff_len, ret_len;
    char *i_buff = NULL;
    int ret;
    UNUSED(tokdata);
    i_buff_len = MXL_SECURE_STORAGE_MAX_FILE_LEN;
    i_buff = calloc(1,i_buff_len);
    if(i_buff == NULL)
        return CKR_HOST_MEMORY;

    if(mxl_securestore_open() == -1UL) {
        TRACE_ERROR("%s:mxl_securestore_open failed\n", __func__);
        return CKR_FUNCTION_FAILED;
    }
    ret = mxl_securestore_load(i_buff, i_buff_len, &ret_len);
    if(ret != 0)  {
        TRACE_ERROR("%s:mxl_securestore_load failed with error %d\n", __func__, ret);
        rc = CKR_KEY_HANDLE_INVALID;
        goto done;
    }
    total_len = ret_len;
    rc =  mxl_delete_token_object_internal(obj, i_buff, ret_len, &ret_len);
    if( rc != CKR_OK ) {
        TRACE_ERROR("%s:mxl_delete_token_object_internal failed with error %ld\n", __func__, rc);
        goto done;
    }
    if(total_len == ret_len) {
        rc = CKR_OK;
        goto done;
    }
    if (!ret_len) {
        mxl_securestore_delete();
        goto done;
    }
    ret = mxl_securestore_save(i_buff, ret_len);
    if(ret != 0) {
        TRACE_ERROR("%s:mxl_securestore_save failed with error %d\n", __func__, ret);
        rc = CKR_FUNCTION_FAILED;
    } else {
        rc = CKR_OK;
    }
done:
    mxl_securestore_close();
    if (i_buff)
        free(i_buff);
    TRACE_INFO("%s:rc = 0x%ld\n", __func__, rc);
    return rc;
}

CK_RV mxl_destroy_token_objects(STDLL_TokData_t *tokdata)
{
    CK_RV rc = CKR_OK;
    int ret;
    uint32_t i_buff_len, ret_len, o_length, key_name;
    char *i_buff, *o_buff;
    UNUSED(tokdata);
    i_buff_len = MXL_SECURE_STORAGE_MAX_FILE_LEN;
    i_buff = calloc(1,i_buff_len);
    if(i_buff == NULL)
        return CKR_HOST_MEMORY;

    if(mxl_securestore_open() == -1UL) {
        TRACE_ERROR("%s:mxl_securestore_open failed\n",__func__);
        return CKR_FUNCTION_FAILED;
    }
    ret = mxl_securestore_load(i_buff, i_buff_len, &ret_len);
    if((ret != 0) || (ret_len == 0)) {
        free(i_buff);
        mxl_securestore_close();
        return CKR_OK;

    } else {
        i_buff_len = ret_len;
        o_buff = i_buff;
        while (i_buff_len > 4) {
            memcpy(&o_length, o_buff, 4);
            if(i_buff_len < o_length ) {
                TRACE_ERROR("%s:Wrong Object Length\n", __func__);
                rc = CKR_GENERAL_ERROR;
                break;
            }
            i_buff_len -= o_length;
            memcpy(&key_name, o_buff+13, 4);
            mxl_securestore_delete_keypair(key_name);
            o_buff += o_length;

        }
    }
    mxl_securestore_delete();
    if (i_buff)
        free(i_buff);
    TRACE_INFO("%s:rc = 0x%ld\n", __func__, rc);
    return rc;

}

