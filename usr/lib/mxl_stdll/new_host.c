/*
 * COPYRIGHT (c) International Business Machines Corp. 2015-2017
 *
 * This program is provided under the terms of the Common Public License,
 * version 1.0 (CPL-1.0). Any use, reproduction or distribution for this
 * software constitutes recipient's acceptance of CPL-1.0 terms which can be
 * found in the file LICENSE file or at
 * https://opensource.org/licenses/cpl1.0.php
 */

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <openssl/crypto.h>
#include <openssl/objects.h>
#include <openssl/evp.h>
#include <openssl/ecdsa.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/core_names.h>
#include <openssl/obj_mac.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/rsa.h>
#include <openssl/param_build.h>
#include "p11util.h"
#include "pkcs11types.h"
#include "stdll.h"

#include "defs.h"
#include "host_defs.h"
#include "h_extern.h"
#include "tok_spec_struct.h"
#include "pkcs32.h"
#include "trace.h"
#include "slotmgr.h"
#include "attributes.h"
#include "constant_time.h"
#include "ec_curves.h"

#include "../api/apiproto.h"
#include "../api/policy.h"

#include "mxl_loadsave.h"
#include "mxl_specific.h"

static const CK_BYTE prime256v1[] = OCK_PRIME256V1;
static const CK_BYTE secp384r1[] = OCK_SECP384R1;

CK_RV mxl_tok_init(STDLL_TokData_t * tokdata, CK_SLOT_ID slot_id,
                   char *conf_name);

void SC_SetFunctionList(void);

CK_RV SC_Finalize(STDLL_TokData_t *tokdata, CK_SLOT_ID sid, SLOT_INFO *sinfp,
                  struct trace_handle_t *t, CK_BBOOL in_fork_initializer);
CK_RV mxl_object_mgr_create_final(STDLL_TokData_t *tokdata,
                              SESSION *sess,
                              OBJECT *obj, CK_OBJECT_HANDLE *handle);
static void mxltok_generate_mxl_attribute(TEMPLATE *tmpl, unsigned char *attribute_buffer, uint32_t *attribute_length);

/* verify that the mech specified is in the
 * mech list for this token...
 */
CK_RV valid_mech(STDLL_TokData_t *tokdata, CK_MECHANISM_PTR m, CK_FLAGS f)
{
    CK_RV rc;
    CK_MECHANISM_INFO info;

    UNUSED(tokdata);

    if (m) {
        memset(&info, 0, sizeof(info));
        rc = ock_generic_get_mechanism_info(tokdata, m->mechanism, &info, NULL);
        if (rc != CKR_OK || !(info.flags & (f)))
            return CKR_MECHANISM_INVALID;
    }

    return CKR_OK;
}

/* In an STDLL this is called once for each card in the system
 * therefore the initialized only flags certain one time things.
 */
CK_RV ST_Initialize(API_Slot_t *sltp, CK_SLOT_ID SlotNumber,
                    SLOT_INFO *sinfp, struct trace_handle_t t)
{
    CK_RV rc = CKR_OK;
    char abs_tokdir_name[MXL_PATH_MAX];
    int ret, pklen;
    CK_BBOOL locked = FALSE;

    install_token_fuctionlist(&mxl_reload_token_object, &mxl_delete_token_object, &mxl_save_token_object, &mxl_object_mgr_create_final);

    /* set trace info */
    set_trace(t);

    rc = bt_init(&sltp->TokData->sess_btree, free);
    rc |= bt_init(&sltp->TokData->object_map_btree, free);
    rc |= bt_init(&sltp->TokData->sess_obj_btree, call_object_free);
    rc |= bt_init(&sltp->TokData->priv_token_obj_btree, call_object_free);
    rc |= bt_init(&sltp->TokData->publ_token_obj_btree, call_object_free);
    if (rc != CKR_OK) {
        TRACE_ERROR("Btree init failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }
    TRACE_DEVEL("Btree init passed \n");
    if (strlen(sinfp->tokname)) {
        if (ock_snprintf(abs_tokdir_name, MXL_PATH_MAX, "%s/%s",
                         MXL_PK_DIR, sinfp->tokname) != 0) {
            TRACE_ERROR("token directory path buffer overflow\n");
            rc = CKR_FUNCTION_FAILED;
            goto done;
        }
    } else {
        if (ock_snprintf(abs_tokdir_name, MXL_PATH_MAX, "%s",
                         MXL_PK_DIR) != 0) {
            TRACE_ERROR("token directory path buffer overflow\n");
            rc = CKR_FUNCTION_FAILED;
            goto done;
        }
    }
    pklen = strlen(abs_tokdir_name) + 1;
    sltp->TokData->pk_dir = (char *) calloc(pklen, 1);
    if (!(sltp->TokData->pk_dir)) {
        TRACE_ERROR("calloc failed\n");
        rc = CKR_HOST_MEMORY;
        goto done;
    }
    if (ock_snprintf(sltp->TokData->pk_dir, pklen, "%s", abs_tokdir_name) != 0) {
        TRACE_ERROR("ock_snprintf failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }

    sltp->TokData->version = sinfp->version;
    TRACE_DEVEL("Token version: %u.%u\n",
                (unsigned int)(sinfp->version >> 16),
                (unsigned int)(sinfp->version & 0xffff));

    /* Initialize Lock */
    if (XProcLock_Init(sltp->TokData) != CKR_OK) {
        TRACE_ERROR("Thread lock failed.\n");
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }

    /* Create lockfile */
    if (CreateXProcLock(sinfp->tokname, sltp->TokData) != CKR_OK) {
        TRACE_ERROR("Process lock failed.\n");
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }

    /* Handle global initialization issues first if we have not
     * been initialized.
     */
    if (sltp->TokData->initialized == FALSE) {
        rc = attach_shm(sltp->TokData, SlotNumber);
        if (rc != CKR_OK) {
            TRACE_ERROR("Could not attach to shared memory.\n");
            goto done;
        }

        sltp->TokData->nv_token_data =
            &(sltp->TokData->global_shm->nv_token_data);
        SC_SetFunctionList();

        rc = mxl_tok_init(sltp->TokData, SlotNumber, sinfp->confname);
        if (rc != 0) {
            sltp->FcnList = NULL;
            detach_shm(sltp->TokData, 0);
            final_data_store(sltp->TokData);
            TRACE_DEVEL("Token Specific Init failed.\n");
            goto done;
        }

        sltp->TokData->initialized = TRUE;
    }


    rc = XProcLock(sltp->TokData);
    if (rc != CKR_OK)
        goto done;
    locked = TRUE;

    if (mxl_init_scsa_session() != TEEC_SUCCESS) {
        TRACE_ERROR("mxl_init_scsa_session failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }

    ret = mxl_nvdat_open();
    if (ret != 0) {
        /* Better error checking added */
        if (ret > 0) {
            TRACE_DEVEL("init the token data !! \n");
            rc = mxl_init_token_data(sltp->TokData, SlotNumber);
            if (rc != CKR_OK) {
                TRACE_DEVEL("Failed to init token data. (rc=0x%02lx)\n", rc);
                goto done;
            }
        }
        else if (ret < 0) {
            TRACE_ERROR("mxl_nvdat_open failed\n");
            rc = CKR_FUNCTION_FAILED;
            goto done;
        }
    }

    rc = mxl_load_token_data(sltp->TokData, SlotNumber);
    if (rc != CKR_OK) {
        sltp->FcnList = NULL;
        final_data_store(sltp->TokData);
        TRACE_DEVEL("Failed to load token data. (rc=0x%02lx)\n", rc);
        goto done;
    }

    mxl_load_public_token_objects(sltp->TokData);
    sltp->TokData->global_shm->publ_loaded = TRUE;

    init_slotInfo(&(sltp->TokData->slot_info));

    (sltp->FcnList) = &function_list;

done:
    mxl_nvdat_close();
    mxl_exit_scsa_session();
    if (locked) {
        if (rc == CKR_OK) {
            rc = XProcUnLock(sltp->TokData);
            if (rc != CKR_OK) {
                TRACE_ERROR("Failed to release Process Lock.\n");
            }
        } else {
            /* return error that occurred first */
            XProcUnLock(sltp->TokData);
        }
    }
    if (rc != CKR_OK && sltp->TokData != NULL) {
        if (sltp->TokData->initialized) {
            SC_Finalize(sltp->TokData, SlotNumber, sinfp, NULL, 0);
        } else {
            CloseXProcLock(sltp->TokData);
            final_data_store(sltp->TokData);
            bt_destroy(&sltp->TokData->sess_btree);
            bt_destroy(&sltp->TokData->object_map_btree);
            bt_destroy(&sltp->TokData->sess_obj_btree);
            bt_destroy(&sltp->TokData->priv_token_obj_btree);
            bt_destroy(&sltp->TokData->publ_token_obj_btree);
        }
    }

    TRACE_INFO("ST_Initialize: rc = 0x%08lx\n", rc);
    return rc;
}

CK_RV SC_Finalize(STDLL_TokData_t *tokdata, CK_SLOT_ID sid, SLOT_INFO *sinfp,
                  struct trace_handle_t *t, CK_BBOOL in_fork_initializer)
{
    CK_RV rc = CKR_OK;

    UNUSED(sid);
    UNUSED(sinfp);

    if (t != NULL)
        set_trace(*t);

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        return CKR_CRYPTOKI_NOT_INITIALIZED;
    }

    tokdata->initialized = FALSE;

    session_mgr_close_all_sessions(tokdata);
    object_mgr_purge_token_objects(tokdata);

    /* Finally free the nodes on free list. */
    bt_destroy(&tokdata->sess_btree);
    bt_destroy(&tokdata->object_map_btree);
    bt_destroy(&tokdata->sess_obj_btree);
    bt_destroy(&tokdata->priv_token_obj_btree);
    bt_destroy(&tokdata->publ_token_obj_btree);

    detach_shm(tokdata, in_fork_initializer);

    /* close spin lock file */
    CloseXProcLock(tokdata);

    final_data_store(tokdata);
    TRACE_INFO("SC_Finalize: rc = 0x%08lx\n", rc);

    return rc;
}

/*
 * This routine should only be called if no other processes are
 * attached to the token.  we need to somehow check that this is the
 * only process Meta API should prevent this since it knows session
 * states in the shared memory.
*/
CK_RV SC_InitToken(STDLL_TokData_t *tokdata, CK_SLOT_ID sid, CK_CHAR_PTR pPin,
                   CK_ULONG ulPinLen, CK_CHAR_PTR pLabel)
{
    CK_RV rc = CKR_OK;
    CK_BBOOL locked = FALSE;
    TEEC_Operation op ={0};
    seccrypto_pin_info_t pin_info = {0};

    if (pthread_mutex_lock(&tokdata->login_mutex)) {
        TRACE_ERROR("Failed to get mutex lock.\n");
        return CKR_FUNCTION_FAILED;
    }

    if (!pPin || !pLabel) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        rc = CKR_ARGUMENTS_BAD;
        goto done;
    }

    rc = XProcLock(tokdata);
    if (rc != CKR_OK)
        goto done;

    locked = TRUE;

    if (mxl_init_scsa_session() != TEEC_SUCCESS) {
        TRACE_ERROR("mxl_init_scsa_session failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }

    if (mxl_prepare_pin_info(&pin_info, CKU_SO, pPin, ulPinLen, &op) < 0) {
        TRACE_ERROR("mxl_prepare_pin_info failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }

    if  (mxl_execute_scsa_command(TA_SECURE_CRYPTO_AUTH_PIN, &op) != TEEC_SUCCESS) {
        TRACE_ERROR("TEE execute SCSA command failed with error = %lx \n", rc);
        rc = CKR_PIN_INCORRECT;
        goto done;
    }

    /* Before we reconstruct all the data, we should delete the
     * token objects from the secure storage.
     */
    mxl_destroy_token_objects(tokdata);
    rc = mxl_init_token_data(tokdata, sid);
    if (rc != CKR_OK) {
        TRACE_DEVEL("Failed to init token data. (rc=0x%02lx)\n", rc);
        goto done;
    }
    mxl_load_token_data(tokdata, sid);
    init_slotInfo(&(tokdata->slot_info));
    tokdata->nv_token_data->token_info.flags |= CKF_TOKEN_INITIALIZED;
    tokdata->nv_token_data->token_info.flags &= ~(CKF_USER_PIN_INITIALIZED |
            CKF_USER_PIN_LOCKED | CKF_USER_PIN_FINAL_TRY |
            CKF_USER_PIN_COUNT_LOW);
    memcpy(tokdata->nv_token_data->token_info.label, pLabel, 32);

    rc = mxl_save_token_data(tokdata, sid);
    if (rc != CKR_OK) {
        TRACE_DEVEL("Failed to save token data.\n");
        goto done;
    }

done:
    mxl_exit_scsa_session();
    if (locked) {
        if (rc == CKR_OK) {
            rc = XProcUnLock(tokdata);
            if (rc != CKR_OK) {
                TRACE_ERROR("Failed to release Process Lock.\n");
            }
        } else {
            /* return error that occurred first */
            XProcUnLock(tokdata);
        }
    }
    pthread_mutex_unlock(&tokdata->login_mutex);

    TRACE_INFO("SC_InitToken: rc = 0x%08lx\n", rc);

    return rc;
}

CK_RV SC_GetTokenInfo(STDLL_TokData_t *tokdata, CK_SLOT_ID sid,
                      CK_TOKEN_INFO_PTR pInfo)
{
    CK_RV rc = CKR_OK;
    time_t now;

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }
    if (!pInfo) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        rc = CKR_ARGUMENTS_BAD;
        goto done;
    }
    if (sid >= NUMBER_SLOTS_MANAGED) {
        TRACE_ERROR("%s\n", ock_err(ERR_SLOT_ID_INVALID));
        rc = CKR_SLOT_ID_INVALID;
        goto done;
    }
    copy_token_contents_sensibly(pInfo, tokdata->nv_token_data);

    /* Set the time */
    now = time((time_t *) NULL);
    strftime((char *) pInfo->utcTime, 16, "%Y%m%d%H%M%S", localtime(&now));
    pInfo->utcTime[14] = '0';
    pInfo->utcTime[15] = '0';

done:
    TRACE_INFO("SC_GetTokenInfo: rc = 0x%08lx\n", rc);

    return rc;
}

CK_RV SC_OpenSession(STDLL_TokData_t *tokdata, CK_SLOT_ID sid, CK_FLAGS flags,
                     CK_SESSION_HANDLE_PTR phSession)
{
    CK_RV rc = CKR_OK;
    SESSION *sess;

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        return CKR_CRYPTOKI_NOT_INITIALIZED;
    }
    if (phSession == NULL) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        return CKR_ARGUMENTS_BAD;
    }
    if (sid >= NUMBER_SLOTS_MANAGED) {
        TRACE_ERROR("%s\n", ock_err(ERR_SLOT_ID_INVALID));
        return CKR_SLOT_ID_INVALID;
    }
    flags |= CKF_SERIAL_SESSION;
    if ((flags & CKF_RW_SESSION) == 0) {
        if (session_mgr_so_session_exists(tokdata)) {
            TRACE_ERROR("%s\n", ock_err(ERR_SESSION_READ_WRITE_SO_EXISTS));
            return CKR_SESSION_READ_WRITE_SO_EXISTS;
        }
    }

    rc = session_mgr_new(tokdata, flags, sid, phSession);
    if (rc != CKR_OK) {
        TRACE_DEVEL("session_mgr_new() failed\n");
        return rc;
    }

    sess = session_mgr_find_reset_error(tokdata, *phSession);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        return CKR_SESSION_HANDLE_INVALID;
    }
    sess->handle = *phSession;

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    TRACE_INFO("SC_OpenSession: rc = 0x%08lx sess = %lu\n", rc, sess->handle);

    return rc;
}

CK_RV SC_Login(STDLL_TokData_t *tokdata, ST_SESSION_HANDLE *sSession,
               CK_USER_TYPE userType, CK_CHAR_PTR pPin, CK_ULONG ulPinLen)
{
    SESSION *sess = NULL;
    CK_FLAGS_32 *flags = NULL;
    CK_BBOOL locked = FALSE;
    CK_RV rc = CKR_OK;
    TEEC_Operation op ={0};
    seccrypto_pin_info_t pin_info = {0};

    /* In v2.11, logins should be exclusive, since token
     * specific flags may need to be set for a bad login. - KEY
     */
    if (pthread_mutex_lock(&tokdata->login_mutex)) {
        TRACE_ERROR("Failed to get mutex lock.\n");
        return CKR_FUNCTION_FAILED;
    }

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }
    flags = &tokdata->nv_token_data->token_info.flags;

    if (!pPin || ulPinLen > MAX_PIN_LEN) {
        set_login_flags(userType, flags);
        TRACE_ERROR("%s\n", ock_err(ERR_PIN_INCORRECT));
        rc = CKR_PIN_INCORRECT;
        goto done;
    }

    /* PKCS #11 v2.01 requires that all sessions have the same login status:
     * --> all sessions are public, all are SO or all are USER
     */
    switch (userType) {
    case CKU_USER:
        if (session_mgr_so_session_exists(tokdata)) {
            TRACE_ERROR("%s\n", ock_err(ERR_USER_ANOTHER_ALREADY_LOGGED_IN));
            rc = CKR_USER_ANOTHER_ALREADY_LOGGED_IN;
        }
        if (session_mgr_user_session_exists(tokdata)) {
            TRACE_ERROR("%s\n", ock_err(ERR_USER_ALREADY_LOGGED_IN));
            rc = CKR_USER_ALREADY_LOGGED_IN;
        }
        break;

    case CKU_SO:
        if (session_mgr_user_session_exists(tokdata)) {
            TRACE_ERROR("%s\n", ock_err(ERR_USER_ANOTHER_ALREADY_LOGGED_IN));
            rc = CKR_USER_ANOTHER_ALREADY_LOGGED_IN;
        }
        if (session_mgr_so_session_exists(tokdata)) {
            TRACE_ERROR("%s\n", ock_err(ERR_USER_ALREADY_LOGGED_IN));
            rc = CKR_USER_ALREADY_LOGGED_IN;
        }
        if (session_mgr_readonly_session_exists(tokdata)) {
            TRACE_ERROR("%s\n", ock_err(ERR_SESSION_READ_ONLY_EXISTS));
            rc = CKR_SESSION_READ_ONLY_EXISTS;
        }
        break;

    case CKU_CONTEXT_SPECIFIC:
        /*
         * Although not explicitly required by the PKCS#11 standard, check that
         * a USER session exists. C_Login with CKU_CONTEXT_SPECIFIC is performed
         * due to CKA_ALWAYS_AUTHENTICATE=TRUE on a key used with an operation.
         * CKA_ALWAYS_AUTHENTICATE=TRUE is only allowed for keys of class
         * CKO_PRIVATE_KEY that have CKA_PRIVATE=TRUE. Key objects with
         * CKA_PRIVATE=TRUE can only be accessed in a USER session, so a
         * C_Login with CKU_CONTEXT_SPECIFIC can also only happen when a USER
         * session exists.
         */
        if (!session_mgr_user_session_exists(tokdata)) {
            TRACE_ERROR("%s\n", ock_err(ERR_USER_NOT_LOGGED_IN));
            rc = CKR_USER_NOT_LOGGED_IN;
        }
        if (!(sess->sign_ctx.active && sess->sign_ctx.auth_required) &&
            !(sess->decr_ctx.active && sess->decr_ctx.auth_required)) {
            TRACE_ERROR("%s\n", ock_err(ERR_OPERATION_NOT_INITIALIZED));
            rc = CKR_OPERATION_NOT_INITIALIZED;
        }
        break;

    default:
        rc = CKR_USER_TYPE_INVALID;
        TRACE_ERROR("%s\n", ock_err(ERR_USER_TYPE_INVALID));
        break;
    }
    if (rc != CKR_OK)
        goto done;

    rc = XProcLock(tokdata);
    if (rc != CKR_OK) {
        TRACE_ERROR("Failed to get process lock.\n");
        goto done;
    }
    locked = TRUE;

    if (mxl_init_scsa_session() != TEEC_SUCCESS) {
        TRACE_ERROR("mxl_init_scsa_session failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }

    switch (userType) {
    case CKU_USER:
        if (*flags & CKF_USER_PIN_LOCKED) {
            TRACE_ERROR("%s\n", ock_err(ERR_PIN_LOCKED));
            rc = CKR_PIN_LOCKED;
            goto done;
        }

        if (!(*flags & CKF_USER_PIN_INITIALIZED)) {
            TRACE_ERROR("%s\n", ock_err(ERR_USER_PIN_NOT_INITIALIZED));
            rc = CKR_USER_PIN_NOT_INITIALIZED;
            goto done;
        }

        if (mxl_prepare_pin_info(&pin_info, userType, pPin, ulPinLen, &op) < 0) {
            rc = CKR_FUNCTION_FAILED;
            goto done;
        }

        if (mxl_execute_scsa_command(TA_SECURE_CRYPTO_AUTH_PIN, &op) != TEEC_SUCCESS) {
            TRACE_ERROR("TEE execute SCSA command failed with error = %lx \n", rc);
            set_login_flags(userType, flags);
            rc = CKR_PIN_INCORRECT;
            goto done;
        }

        /* Successful login, clear flags */
        *flags &= ~(CKF_USER_PIN_LOCKED |
                        CKF_USER_PIN_FINAL_TRY | CKF_USER_PIN_COUNT_LOW);

        mxl_load_private_token_objects(tokdata);
        tokdata->global_shm->priv_loaded = TRUE;
        break;
    case CKU_SO:
        if (*flags & CKF_SO_PIN_LOCKED) {
            TRACE_ERROR("%s\n", ock_err(ERR_PIN_LOCKED));
            rc = CKR_PIN_LOCKED;
            goto done;
        }
        if (mxl_prepare_pin_info(&pin_info, userType, pPin, ulPinLen, &op) < 0) {
            TRACE_ERROR("mxl_prepare_pin_info failed\n");
            rc = CKR_FUNCTION_FAILED;
            goto done;
        }

        if (mxl_execute_scsa_command(TA_SECURE_CRYPTO_AUTH_PIN, &op) != TEEC_SUCCESS) {
            TRACE_ERROR("TEE execute SCSA command failed with error = %lx \n", rc);
            set_login_flags(userType, flags);
            rc = CKR_PIN_INCORRECT;
            goto done;
        }
        /* Successful login, clear flags */
        *flags &= ~(CKF_SO_PIN_LOCKED | CKF_SO_PIN_FINAL_TRY |
                        CKF_SO_PIN_COUNT_LOW);

        break;
    case CKU_CONTEXT_SPECIFIC:
        if (*flags & CKF_USER_PIN_LOCKED) {
            TRACE_ERROR("%s\n", ock_err(ERR_PIN_LOCKED));
            rc = CKR_PIN_LOCKED;
            goto done;
        }

        if (!(*flags & CKF_USER_PIN_INITIALIZED)) {
            TRACE_ERROR("%s\n", ock_err(ERR_USER_PIN_NOT_INITIALIZED));
            rc = CKR_USER_PIN_NOT_INITIALIZED;
            goto done;
        }
        if (mxl_prepare_pin_info(&pin_info, userType, pPin, ulPinLen, &op) < 0) {
            TRACE_ERROR("mxl_prepare_pin_info failed\n");
            rc = CKR_FUNCTION_FAILED;
            goto done;
        }
        if (mxl_execute_scsa_command(TA_SECURE_CRYPTO_AUTH_PIN, &op) != TEEC_SUCCESS) {
            TRACE_ERROR("TEE execute SCSA command failed with error = %lx \n", rc);
            set_login_flags(userType, flags);
            rc = CKR_PIN_INCORRECT;
            goto done;
        }

        /* Successful login, clear flags */
        *flags &= ~(CKF_USER_PIN_LOCKED |
                        CKF_USER_PIN_FINAL_TRY | CKF_USER_PIN_COUNT_LOW);

        /*
         * Reset flag in operation context to indicate that a login has been
         * successfully performed
         */
        if (sess->sign_ctx.active && sess->sign_ctx.auth_required)
            sess->sign_ctx.auth_required = FALSE;
        if (sess->decr_ctx.active && sess->decr_ctx.auth_required)
            sess->decr_ctx.auth_required = FALSE;
        break;

    default:
        rc = CKR_USER_TYPE_INVALID;
        TRACE_ERROR("%s\n", ock_err(ERR_USER_TYPE_INVALID));
        goto done;
    }

done:
    if (rc == CKR_OK && userType != CKU_CONTEXT_SPECIFIC) {
        rc = session_mgr_login_all(tokdata, userType);
        if (rc != CKR_OK)
            TRACE_DEVEL("session_mgr_login_all failed.\n");
    }

    /*
     * PKCS#11 states for failing C_Login with user type CKU_CONTEXT_SPECIFIC:
     * '... repeated failed re-authentication attempts may cause the PIN to be
     * locked. C_Login returns in this case CKR_PIN_LOCKED and this also logs
     * the user out from the token'
     */
    if (userType == CKU_CONTEXT_SPECIFIC &&
        rc == CKR_PIN_INCORRECT &&
        pin_locked(&sess->session_info,
                    tokdata->nv_token_data->token_info.flags)) {
        TRACE_DEVEL("USER pin now locked, logout the user\n");
        //SC_Logout(tokdata, sSession);
        rc = CKR_PIN_LOCKED;
    }

    if (sess)
        mxl_save_token_data(tokdata, sess->session_info.slotID);

    mxl_exit_scsa_session();

    if (locked) {
        if (rc == CKR_OK) {
            rc = XProcUnLock(tokdata);
            if (rc != CKR_OK) {
                TRACE_ERROR("Failed to release Process Lock.\n");
            }
        } else {
            /* return error that occurred first */
            XProcUnLock(tokdata);
        }
    }

    pthread_mutex_unlock(&tokdata->login_mutex);

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    TRACE_INFO("SC_Login: rc = 0x%08lx\n", rc);

    return rc;
}

CK_RV SC_InitPIN(STDLL_TokData_t * tokdata, ST_SESSION_HANDLE * sSession,
                 CK_CHAR_PTR pPin, CK_ULONG ulPinLen)
{
    SESSION *sess = NULL;
    CK_BBOOL locked = FALSE;
    CK_RV rc = CKR_OK;
    TEEC_Operation op ={0};
    seccrypto_pin_set_info_t pin_info = {0};

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        return CKR_CRYPTOKI_NOT_INITIALIZED;
    }
    if (!pPin) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        return CKR_ARGUMENTS_BAD;
    }

    if (pthread_mutex_lock(&tokdata->login_mutex)) {
        TRACE_ERROR("Failed to get mutex lock.\n");
        return CKR_FUNCTION_FAILED;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }
    if (pin_locked(&sess->session_info,
                   tokdata->nv_token_data->token_info.flags) == TRUE) {
        TRACE_ERROR("%s\n", ock_err(ERR_PIN_LOCKED));
        rc = CKR_PIN_LOCKED;
        goto done;
    }
    if (sess->session_info.state != CKS_RW_SO_FUNCTIONS) {
        TRACE_ERROR("%s\n", ock_err(ERR_USER_NOT_LOGGED_IN));
        rc = CKR_USER_NOT_LOGGED_IN;
        goto done;
    }

    if ((ulPinLen < MIN_PIN_LEN) || (ulPinLen > MAX_PIN_LEN)) {
        TRACE_ERROR("%s\n", ock_err(ERR_PIN_LEN_RANGE));
        rc = CKR_PIN_LEN_RANGE;
        goto done;
    }

    rc = XProcLock(tokdata);
    if (rc != CKR_OK) {
        TRACE_ERROR("Failed to get process lock.\n");
        goto done;
    }
    locked = TRUE;

    if (mxl_init_scsa_session() != TEEC_SUCCESS) {
        TRACE_ERROR("mxl_init_scsa_session failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }

    if (mxl_prepare_pin_set_info(&pin_info, CKU_USER, NULL, 0, pPin, ulPinLen, &op) < 0) {
        rc = CKR_PIN_INVALID;
        goto done;
    }
    if (mxl_execute_scsa_command(TA_SECURE_CRYPTO_SET_PIN, &op) != TEEC_SUCCESS) {
        TRACE_ERROR("TEE execute SCSA command failed with error = %lx \n", rc);
        rc = CKR_PIN_INVALID;
        goto done;
    }

    tokdata->nv_token_data->token_info.flags |= CKF_USER_PIN_INITIALIZED;
    tokdata->nv_token_data->token_info.flags &= ~(CKF_USER_PIN_TO_BE_CHANGED);
    tokdata->nv_token_data->token_info.flags &= ~(CKF_USER_PIN_LOCKED);

    rc = mxl_save_token_data(tokdata, sess->session_info.slotID);
    if (rc != CKR_OK) {
        TRACE_DEVEL("Failed to save token data.\n");
        goto done;
    }

done:
    mxl_exit_scsa_session();
    if (locked) {
        if (rc == CKR_OK) {
            rc = XProcUnLock(tokdata);
            if (rc != CKR_OK) {
                TRACE_ERROR("Failed to release Process Lock.\n");
            }
        } else {
            /* return error that occurred first */
            XProcUnLock(tokdata);
        }
    }

    pthread_mutex_unlock(&tokdata->login_mutex);

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    TRACE_INFO("SC_InitPIN: rc = 0x%08lx, session = %lu\n",
               rc, sSession->sessionh);

    return rc;
}

CK_RV SC_SetPIN(STDLL_TokData_t *tokdata, ST_SESSION_HANDLE *sSession,
                CK_CHAR_PTR pOldPin, CK_ULONG ulOldLen, CK_CHAR_PTR pNewPin,
                CK_ULONG ulNewLen)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;
    CK_BBOOL locked = FALSE;
    TEEC_Operation op ={0};
    seccrypto_pin_set_info_t pin_info = {0};

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        return CKR_CRYPTOKI_NOT_INITIALIZED;
    }

    if (pthread_mutex_lock(&tokdata->login_mutex)) {
        TRACE_ERROR("Failed to get mutex lock.\n");
        return CKR_FUNCTION_FAILED;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }
    if (pin_locked(&sess->session_info,
                   tokdata->nv_token_data->token_info.flags) == TRUE) {
        TRACE_ERROR("%s\n", ock_err(ERR_PIN_LOCKED));
        rc = CKR_PIN_LOCKED;
        goto done;
    }

    /* Check if token has a specific handler for this, otherwise fall back
     * to default behaviour.
     */

    if ((ulNewLen < MIN_PIN_LEN) || (ulNewLen > MAX_PIN_LEN)) {
        TRACE_ERROR("%s\n", ock_err(ERR_PIN_LEN_RANGE));
        rc = CKR_PIN_LEN_RANGE;
        goto done;
    }

    rc = XProcLock(tokdata);
    if (rc != CKR_OK) {
        TRACE_DEVEL("Failed to get process lock.\n");
        goto done;
    }
    locked = TRUE;

    if (mxl_init_scsa_session() != TEEC_SUCCESS) {
        TRACE_ERROR("mxl_init_scsa_session failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto done;
    }

    /* From the PKCS#11 2.20 spec: "C_SetPIN modifies the PIN of
     * the user that is currently logged in, or the CKU_USER PIN
     * if the session is not logged in."  A non R/W session fails
     * with CKR_SESSION_READ_ONLY.
     */
    if ((sess->session_info.state == CKS_RW_USER_FUNCTIONS) ||
        (sess->session_info.state == CKS_RW_PUBLIC_SESSION)) {

	if (mxl_prepare_pin_set_info(&pin_info, CKU_USER, pOldPin, ulOldLen, pNewPin, ulNewLen, &op) < 0) {
	    rc = CKR_PIN_INVALID;
	    goto done;
	}

	if (mxl_execute_scsa_command(TA_SECURE_CRYPTO_SET_PIN, &op) != TEEC_SUCCESS) {
            TRACE_ERROR("TEE execute SCSA command failed with error = %lx \n", rc);
            rc = CKR_PIN_INVALID;
            goto done;
        }

        tokdata->nv_token_data->token_info.flags &=
            ~(CKF_USER_PIN_TO_BE_CHANGED);

    } else if (sess->session_info.state == CKS_RW_SO_FUNCTIONS) {

        if (mxl_prepare_pin_set_info(&pin_info, CKU_SO, pOldPin, ulOldLen, pNewPin, ulNewLen, &op) < 0) {
	    rc = CKR_PIN_INVALID;
	    goto done;
	}

	if (mxl_execute_scsa_command(TA_SECURE_CRYPTO_SET_PIN, &op) != TEEC_SUCCESS) {
            TRACE_ERROR("TEE execute SCSA command failed with error = %lx \n", rc);
            rc = CKR_PIN_INVALID;
            goto done;
	}

        tokdata->nv_token_data->token_info.flags &= ~(CKF_SO_PIN_TO_BE_CHANGED);

    } else {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_READ_ONLY));
        rc = CKR_SESSION_READ_ONLY;
    }

    rc = mxl_save_token_data(tokdata, sess->session_info.slotID);
    if (rc != CKR_OK) {
        TRACE_DEVEL("Failed to save token data.\n");
        goto done;
    }

done:
    mxl_exit_scsa_session();
    if (locked) {
        if (rc == CKR_OK) {
            rc = XProcUnLock(tokdata);
            if (rc != CKR_OK) {
                TRACE_ERROR("Failed to release Process Lock.\n");
            }
        } else {
            /* return error that occurred first */
            XProcUnLock(tokdata);
        }
    }
    pthread_mutex_unlock(&tokdata->login_mutex);

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    TRACE_INFO("SC_SetPIN: rc = 0x%08lx, session = %lu\n",
               rc, sSession->sessionh);

    return rc;
}

CK_RV SC_CloseSession(STDLL_TokData_t *tokdata, ST_SESSION_HANDLE *sSession,
                      CK_BBOOL in_fork_initializer)
{
    CK_RV rc = CKR_OK;
    SESSION *sess = NULL;
    UNUSED(in_fork_initializer);

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    if (session_mgr_so_session_exists(tokdata) ||
        session_mgr_user_session_exists(tokdata)) {
        sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
        if (!sess) {
            TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
            rc = CKR_SESSION_HANDLE_INVALID;
            goto done;
        }

        session_mgr_put(tokdata, sess);
        sess = NULL;
    }

    rc = session_mgr_close_session(tokdata, sSession->sessionh);
done:
    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    TRACE_INFO("SC_CloseSession: rc = 0x%08lx, sess = %lu\n",
               rc, sSession->sessionh);

    return rc;
}

CK_RV SC_GetSessionInfo(STDLL_TokData_t *tokdata, ST_SESSION_HANDLE *sSession,
                        CK_SESSION_INFO_PTR pInfo)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    if (!pInfo) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        rc = CKR_ARGUMENTS_BAD;
        goto done;
    }

    sess = session_mgr_find(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }

    memcpy(pInfo, &sess->session_info, sizeof(CK_SESSION_INFO));

done:
    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    TRACE_INFO("SC_GetSessionInfo: sess = %lu\n", sSession->sessionh);

    return rc;
}

CK_RV SC_Logout(STDLL_TokData_t *tokdata, ST_SESSION_HANDLE *sSession)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        return CKR_CRYPTOKI_NOT_INITIALIZED;
    }

    if (pthread_mutex_lock(&tokdata->login_mutex)) {
        TRACE_ERROR("Failed to get mutex lock.\n");
        return CKR_FUNCTION_FAILED;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }

    /* all sessions have the same state so we just have to check one */
    if (session_mgr_public_session_exists(tokdata)) {
        TRACE_ERROR("%s\n", ock_err(ERR_USER_NOT_LOGGED_IN));
        rc = CKR_USER_NOT_LOGGED_IN;
        goto done;
    }

    rc = session_mgr_logout_all(tokdata);
    if (rc != CKR_OK)
        TRACE_DEVEL("session_mgr_logout_all failed.\n");

    object_mgr_purge_private_token_objects(tokdata);

done:

    pthread_mutex_unlock(&tokdata->login_mutex);

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    TRACE_INFO("SC_Logout: rc = 0x%08lx\n", rc);

    return rc;
}

/*
 * Get the mechanism info for the current type and token.
 */
CK_RV SC_GetMechanismInfo(STDLL_TokData_t * tokdata, CK_SLOT_ID sid,
                          CK_MECHANISM_TYPE type, CK_MECHANISM_INFO_PTR pInfo)
{
    CK_RV rc = CKR_OK;

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto out;
    }
    if (pInfo == NULL) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        rc = CKR_ARGUMENTS_BAD;
        goto out;
    }
    if (sid >= NUMBER_SLOTS_MANAGED) {
        TRACE_ERROR("%s\n", ock_err(ERR_SLOT_ID_INVALID));
        rc = CKR_SLOT_ID_INVALID;
        goto out;
    }
    rc = ock_generic_get_mechanism_info(tokdata, type, pInfo, NULL);
out:
    TRACE_INFO("SC_GetMechanismInfo: rc = 0x%08lx, mech type = 0x%08lx\n",
               rc, type);

    return rc;
}

/*
 * Get the mechanism type list for the current token.
 */
CK_RV SC_GetMechanismList(STDLL_TokData_t *tokdata, CK_SLOT_ID sid,
                          CK_MECHANISM_TYPE_PTR pMechList, CK_ULONG_PTR count)
{
    CK_RV rc = CKR_OK;

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto out;
    }
    if (count == NULL) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        rc = CKR_ARGUMENTS_BAD;
        goto out;
    }
    if (sid >= NUMBER_SLOTS_MANAGED) {
        TRACE_ERROR("%s\n", ock_err(ERR_SLOT_ID_INVALID));
        rc = CKR_SLOT_ID_INVALID;
        goto out;
    }
    rc = ock_generic_get_mechanism_list(tokdata, pMechList, count, NULL);
    if (rc == CKR_OK) {
        /* To accomodate certain special cases, we may need to
         * make adjustments to the token's mechanism list.
         */
        mechanism_list_transformations(pMechList, count);
    }

out:
    TRACE_INFO("SC_GetMechanismList:  rc = 0x%08lx, # mechanisms: %lu\n",
               rc, (count ? *count : 0));

    return rc;
}

/*
 * Finalizes the object creation and adds the object into the appropriate
 * btree and also the object map btree.
 * When this function succeeds, object obj must not be freed! It has been added
 * to the btree and thus must be kept intact.
 * When this function fails, then the object obj must be freed by the caller
 * using object_free() (not object_put() nor bt_put_node_value() !)
 */
static CK_RV mxl_object_mgr_create_final_internal(STDLL_TokData_t *tokdata,
                              SESSION *sess,
                              OBJECT *obj, CK_OBJECT_HANDLE *handle, uint32_t key_id)
{
    CK_BBOOL sess_obj;
    CK_BBOOL priv_obj;
    CK_BBOOL locked = FALSE;
    CK_RV rc;
    uint64_t rand;
    unsigned long obj_handle;
    char fname[MXL_PATH_MAX] = "";

    if (!sess || !obj || !handle) {
        TRACE_ERROR("Invalid function arguments.\n");
        return CKR_FUNCTION_FAILED;
    }

    TRACE_DEBUG("Attributes at create final:\n");
    TRACE_DEBUG_DUMPTEMPL(obj->template);

    sess_obj = object_is_session_object(obj);
    priv_obj = object_is_private(obj);

    if (sess_obj) {
        obj->session = sess;
        memset(obj->name, 0x0, sizeof(CK_BYTE) * 8);

        if ((obj_handle = bt_node_add(&tokdata->sess_obj_btree, obj)) == 0) {
            TRACE_ERROR("%s\n", ock_err(ERR_HOST_MEMORY));
            return CKR_HOST_MEMORY;
        }
    } else {
        // we'll be modifying nv_token_data so we should protect this part
        // with 'XProcLock'
        //
        rc = XProcLock(tokdata);
        if (rc != CKR_OK) {
            TRACE_ERROR("Failed to get Process Lock.\n");
            return rc;
        }
        locked = TRUE;

        // Determine if we have already reached our Max Token Objects
        //
        if (priv_obj) {
            if (tokdata->global_shm->num_priv_tok_obj >= MAX_TOK_OBJS) {
                TRACE_ERROR("%s\n", ock_err(ERR_HOST_MEMORY));
                rc = CKR_HOST_MEMORY;
                goto done;
            }
        } else {
            if (tokdata->global_shm->num_publ_tok_obj >= MAX_TOK_OBJS) {
                TRACE_ERROR("%s\n", ock_err(ERR_HOST_MEMORY));
                rc = CKR_HOST_MEMORY;
                goto done;
            }
        }

        obj->session = NULL;
        memset(fname, '\0', MXL_PATH_MAX);
        mxl_rng((CK_BYTE *)&rand, 8);
        sprintf(fname, "%lx", rand);
        memcpy(&obj->name, fname, 8);
        obj->ex_data_len = (size_t) key_id;

        rc = mxl_save_token_object(tokdata, obj);
        if (rc != CKR_OK)
            goto done;

        // add the object identifier to the shared memory segment
        //
        object_mgr_add_to_shm(obj, tokdata->global_shm);

        // now, store the object in the token object btree
        //
        if (priv_obj)
            obj_handle = bt_node_add(&tokdata->priv_token_obj_btree, obj);
        else
            obj_handle = bt_node_add(&tokdata->publ_token_obj_btree, obj);

        if (!obj_handle) {
            TRACE_ERROR("%s\n", ock_err(ERR_HOST_MEMORY));
            rc = CKR_HOST_MEMORY;
            goto done;
        }
    }

    rc = object_mgr_add_to_map(tokdata, sess, obj, obj_handle, handle);
    if (rc != CKR_OK) {
        TRACE_DEVEL("object_mgr_add_to_map failed.\n");
        // this is messy but we need to remove the object from whatever
        // list we just added it to
        //
        if (sess_obj) {
            // put the binary tree node which holds obj on the free list, but
            // pass NULL here, so that obj (the binary tree node's value
            // pointer) isn't touched.
            // It is free'd by the caller of object_mgr_create_final
            bt_node_free(&tokdata->sess_obj_btree, obj_handle, FALSE);
        } else {
            mxl_delete_token_object(tokdata, obj);

            if (priv_obj) {
                // put the binary tree node which holds obj on the free list,
                // but pass NULL here, so that obj (the binary tree node's value
                // pointer) isn't touched. It is free'd by the caller of
                // object_mgr_create_final
                bt_node_free(&tokdata->priv_token_obj_btree, obj_handle, FALSE);
            } else {
                // put the binary tree node which holds obj on the free list,
                // but pass NULL here, so that obj (the binary tree node's value
                // pointer) isn't touched. It is free'd by the caller of
                // object_mgr_create_final
                bt_node_free(&tokdata->publ_token_obj_btree, obj_handle, FALSE);
            }

            object_mgr_del_from_shm(obj, tokdata->global_shm);
        }
    }

done:
    if (locked) {
        if (rc == CKR_OK) {
            rc = XProcUnLock(tokdata);
            if (rc != CKR_OK) {
                TRACE_ERROR("Failed to release Process Lock.\n");
            }
        } else {
            /* return error that occurred first */
            XProcUnLock(tokdata);
        }
    }

    if (rc == CKR_OK)
        TRACE_DEVEL("Object created: handle: %lu\n", *handle);

    return rc;
}

static CK_RV mxl_object_mgr_create_final_certificate(STDLL_TokData_t *tokdata,
                              SESSION *sess,
                              OBJECT *obj, CK_OBJECT_HANDLE *handle)
{
    CK_RV rc;
    uint32_t key_id = 0;

    rc = template_build_update_attribute(obj->template, CKA_MXL_SECURE_STORAGE_HANDLE,(CK_BYTE *)&key_id, sizeof(key_id));
    if (rc != CKR_OK) {
        TRACE_ERROR("%s template_build_update_attribute failed with rc=0x%lx\n", __func__, rc);
        return rc;
    }

    rc = mxl_object_mgr_create_final_internal(tokdata, sess, obj, handle, key_id);
    if (rc != CKR_OK) {
        TRACE_ERROR("mxl_object_mgr_create_final_internal failed.\n");
    }
    return rc;
}

int mxl_write_rsa_pkcs8_private_key(OBJECT *key_obj, uint32_t key_id)
{
    int ret = -1;
    BIO *bio = NULL;
    BUF_MEM *mem = NULL;
    uint8_t buffer[3072] = {0};
    EVP_PKEY *pkey = NULL;
    EVP_PKEY_CTX *ctx = NULL;
    uint32_t len;
    OSSL_PARAM_BLD *tmpl = NULL;
    OSSL_PARAM *params = NULL;
    CK_ATTRIBUTE *modulus = NULL;
    CK_ATTRIBUTE *pub_exp = NULL;
    CK_ATTRIBUTE *priv_exp = NULL;
    CK_ATTRIBUTE *prime1 = NULL;
    CK_ATTRIBUTE *prime2 = NULL;
    CK_ATTRIBUTE *exp_1 = NULL;
    CK_ATTRIBUTE *exp_2 = NULL;
    CK_ATTRIBUTE *coeff = NULL;
    BIGNUM *bn_mod, *bn_pub_exp, *bn_priv_exp, *bn_p1, *bn_p2, *bn_e1, *bn_e2, *bn_cf;

    template_attribute_get_non_empty(key_obj->template, CKA_MODULUS, &modulus);
    template_attribute_get_non_empty(key_obj->template,  CKA_PUBLIC_EXPONENT, &pub_exp);
    template_attribute_find(key_obj->template, CKA_PRIVATE_EXPONENT, &priv_exp);
    template_attribute_find(key_obj->template, CKA_PRIME_1, &prime1);
    template_attribute_find(key_obj->template, CKA_PRIME_2, &prime2);
    template_attribute_find(key_obj->template, CKA_EXPONENT_1, &exp_1);
    template_attribute_find(key_obj->template, CKA_EXPONENT_2,&exp_2);
    template_attribute_find(key_obj->template, CKA_COEFFICIENT, &coeff);

    if (!modulus || !pub_exp || !priv_exp || !prime1 || !prime2
        || !exp_1 || !exp_2 || !coeff) {
        TRACE_ERROR("RSA parameters not found\n");
        goto error;
    }

    bn_mod = BN_new();
    bn_pub_exp = BN_new();
    bn_priv_exp = BN_new();
    bn_p1 = BN_new();
    bn_p2 = BN_new();
    bn_e1 = BN_new();
    bn_e2 = BN_new();
    bn_cf = BN_new();

    if ((bn_cf == NULL) || (bn_e2 == NULL) || (bn_e1 == NULL) ||
        (bn_p2 == NULL) || (bn_p1 == NULL) || (bn_priv_exp == NULL) ||
        (bn_pub_exp == NULL) || (bn_mod == NULL)) {
        TRACE_ERROR("BN_new() failed\r\n");
        return -1;
    }

    BN_bin2bn((unsigned char *) modulus->pValue, modulus->ulValueLen, bn_mod);
    BN_bin2bn((unsigned char *) pub_exp->pValue, pub_exp->ulValueLen, bn_pub_exp);
    BN_bin2bn((unsigned char *) priv_exp->pValue, priv_exp->ulValueLen, bn_priv_exp);
    BN_bin2bn((unsigned char *) prime1->pValue, prime1->ulValueLen, bn_p1);
    BN_bin2bn((unsigned char *) prime2->pValue, prime2->ulValueLen, bn_p2);
    BN_bin2bn((unsigned char *) exp_1->pValue, exp_1->ulValueLen, bn_e1);
    BN_bin2bn((unsigned char *) exp_2->pValue, exp_2->ulValueLen, bn_e2);
    BN_bin2bn((unsigned char *) coeff->pValue, coeff->ulValueLen, bn_cf);

    tmpl = OSSL_PARAM_BLD_new();
    if (tmpl == NULL)
        goto error;

    if (!OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_RSA_N, bn_mod) ||
        !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_RSA_E, bn_pub_exp) ||
        !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_RSA_D, bn_priv_exp) ||
        !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_RSA_FACTOR1, bn_p1) ||
        !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_RSA_FACTOR2, bn_p2) ||
        !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_RSA_EXPONENT1, bn_e1) ||
        !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_RSA_EXPONENT2, bn_e2) ||
        !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_RSA_COEFFICIENT1, bn_cf)) {
        TRACE_ERROR("OSSL_PARAM_BLD_push_BN failed\r\n");
        goto error;
    }

    params = OSSL_PARAM_BLD_to_param(tmpl);
    if (params == NULL)
        goto error;

    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (ctx == NULL)
        goto error;

    if (!EVP_PKEY_fromdata_init(ctx) ||
        !EVP_PKEY_fromdata(ctx, &pkey, EVP_PKEY_KEYPAIR, params)) {
        TRACE_ERROR("EVP_PKEY_fromdata failed\r\n");
        goto error;
    }
    bio = BIO_new(BIO_s_mem());
    if (!bio) {
        TRACE_ERROR("BIO_new failed\r\n");
        goto error;
    }
    if (!PEM_write_bio_PKCS8PrivateKey(bio, pkey, NULL, NULL, 0, NULL, NULL)) {
        TRACE_ERROR("PEM_write_bio_PKCS8PrivateKey failed\r\n");
        goto error;
    }
    BIO_get_mem_ptr(bio, &mem);
    len = mem->length;
    memcpy(buffer, mem->data, len);
    buffer[len] = '\0';
    ret = mxl_securestore_write_key(key_id, buffer, len+1);
    if (ret != 0) {
        TRACE_ERROR("mxl_securestore_write_key failed with error %d\r\n",ret);
        goto error;
    }
    ret = 0;
error:
    if (bio) BIO_free(bio);
    if (pkey) EVP_PKEY_free(pkey);
    if (ctx) EVP_PKEY_CTX_free(ctx);
    if (tmpl) OSSL_PARAM_BLD_free(tmpl);
    if (params) OSSL_PARAM_free(params);
    if (bn_mod) BN_free(bn_mod);
    if (bn_pub_exp) BN_free(bn_pub_exp);
    if (bn_priv_exp) BN_free(bn_priv_exp);
    if (bn_p1) BN_free(bn_p1);
    if (bn_p2) BN_free(bn_p2);
    if (bn_e1) BN_free(bn_e1);
    if (bn_e2) BN_free(bn_e2);
    if (bn_cf) BN_free(bn_cf);
    return ret;
}

static CK_RV mxl_object_mgr_create_final_rsa_key(STDLL_TokData_t *tokdata,
                              SESSION *sess,
                              OBJECT *obj, CK_OBJECT_HANDLE *handle)
{
    CK_BBOOL priv_obj;
    CK_RV rc;
    CK_ATTRIBUTE *attr = NULL;
    unsigned char *attribute_buffer = NULL;
    uint32_t key_id = 0, attribute_size, rsa_key_size, rsa_key[1024] = {0};
    enum sec_alg keyalgo;
    int ret, i;
    CK_BYTE priv_key[512], modulus[512], pub_key[4] = {0};
    CK_ULONG priv_key_len, modulus_len, pub_key_len;
    rc = template_attribute_get_non_empty(obj->template, CKA_PUBLIC_EXPONENT, &attr);
    if (rc != CKR_OK) {
        TRACE_ERROR("Could not find CKA_PUBLIC_EXPONENT.\n");
        goto error;
    }
    pub_key_len = attr->ulValueLen;
    memcpy(&pub_key[0], attr->pValue, pub_key_len);
    rc = template_attribute_get_non_empty(obj->template, CKA_MODULUS, &attr);
    if (rc != CKR_OK) {
        TRACE_ERROR("Could not find CKA_MODULUS for the key.\n");
        goto error;
    }
    modulus_len =  attr->ulValueLen;
    if (modulus_len == 128) {
        keyalgo = SEC_ALG_RSA_1024;
    }
    else if (modulus_len == 256) {
        keyalgo = SEC_ALG_RSA_2048;
    }
    else if (modulus_len == 384) {
        keyalgo = SEC_ALG_RSA_3072;
    }
    else {
        TRACE_ERROR("Not Supported RSA Key.\n");
        rc = CKR_ATTRIBUTE_VALUE_INVALID;
        goto error;
    }
    memcpy(&modulus[0], attr->pValue, modulus_len);
    priv_obj = object_is_private(obj);
    if (priv_obj) {
        uint32_t pkcs8_key_id;
        mxl_rng((CK_BYTE *)&pkcs8_key_id, MXL_SECURE_STORAGE_NAME_LENGTH);
        rc = template_build_update_attribute(obj->template, CKA_MXL_PKCS8_KEY_HANDLE,(CK_BYTE *)&pkcs8_key_id, sizeof(pkcs8_key_id));
        if (rc != CKR_OK) {
            TRACE_ERROR("%s template_build_update_attribute failed with rc=0x%lx\n", __func__, rc);
            goto error;
        }
        ret = mxl_write_rsa_pkcs8_private_key(obj, pkcs8_key_id);
        if (ret != 0) {
            TRACE_ERROR("mxl_write_rsa_pkcs8_key failed with error %d\r\n",ret);
            rc = CKR_FUNCTION_FAILED;
            goto error;
        }
        rc = template_attribute_get_non_empty(obj->template, CKA_PRIVATE_EXPONENT, &attr);
        if (rc != CKR_OK) {
            TRACE_ERROR("Could not find CKA_PUBLIC_EXPONENT.\n");
            goto error;
        }
        priv_key_len = attr->ulValueLen;
        memcpy(&priv_key[0], attr->pValue, priv_key_len);
        template_remove_attribute(obj->template, CKA_PRIVATE_EXPONENT);
        template_remove_attribute(obj->template, CKA_PRIME_1);
        template_remove_attribute(obj->template, CKA_PRIME_2);
        template_remove_attribute(obj->template, CKA_EXPONENT_1);
        template_remove_attribute(obj->template, CKA_EXPONENT_2);
        template_remove_attribute(obj->template, CKA_COEFFICIENT);
    }
    mxl_rng((CK_BYTE *)&key_id, MXL_SECURE_STORAGE_NAME_LENGTH);
    rc = template_build_update_attribute(obj->template, CKA_MXL_SECURE_STORAGE_HANDLE,(CK_BYTE *)&key_id, sizeof(key_id));
    if (rc != CKR_OK) {
        TRACE_ERROR("%s template_build_update_attribute failed with rc=0x%lx\n", __func__, rc);
        goto error;
    }

    rsa_key[0] = 0x2;
    rsa_key[1] = (uint32_t) keyalgo;
    if (priv_obj) {
        rsa_key[2] = priv_key_len + modulus_len;
        rsa_key[3] = 0;
        rsa_key[4] = priv_key_len;
        memcpy(&rsa_key[5], priv_key, priv_key_len);
        i = priv_key_len/4;
        rsa_key[5 + i] = modulus_len;
        memcpy(&rsa_key[6 + i], modulus, modulus_len);
        attribute_buffer =  (unsigned char*)&rsa_key[0] + 32 + priv_key_len + modulus_len;
        attribute_size = 0;
        mxltok_generate_mxl_attribute(obj->template, attribute_buffer , &attribute_size);
        attribute_size = (attribute_size + 3) & ~0x3;
	i += modulus_len/4;
        rsa_key[7 + i] = attribute_size;
        rsa_key_size = 32 + priv_key_len + modulus_len + attribute_size;
    }
    else {
        CK_BBOOL mxl_key_flag = TRUE;
        rc = template_build_update_attribute(obj->template, CKA_MXL_KEY_TYPE, (CK_BYTE *)&mxl_key_flag, sizeof(CK_BBOOL));
        if (rc != CKR_OK) {
            TRACE_ERROR("%s template_build_update_attribute failed with rc=0x%lx\n", __func__, rc);
            goto error;
        }
        rsa_key[2] = pub_key_len + modulus_len;
        rsa_key[3] = pub_key_len;
        memcpy(&rsa_key[4], pub_key, pub_key_len);
        rsa_key[5] = 0;
        rsa_key[6] = modulus_len;
        memcpy(&rsa_key[7], modulus, modulus_len);
        attribute_buffer =  (unsigned char*)&rsa_key[0] + 32 + modulus_len;
        attribute_size = 0;
        mxltok_generate_mxl_attribute(obj->template, attribute_buffer , &attribute_size);
        attribute_size = (attribute_size + 3) & ~0x3;
        i = modulus_len/4;
        rsa_key[7 + i] = attribute_size;
        rsa_key_size = 36 + modulus_len + attribute_size;
    }
    ret = mxl_securestore_write_key(key_id, (uint8_t *)&rsa_key[0], rsa_key_size);
    if (ret != 0) {
        TRACE_ERROR("mxl_securestore_write_key failed with error %d\r\n",ret);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    rc = mxl_object_mgr_create_final_internal(tokdata, sess, obj, handle, key_id);
    if (rc != CKR_OK) {
        TRACE_ERROR("mxl_object_mgr_create_final_internal failed.\n");
        goto error;
    }
error:
    return rc;

}

static CK_RV mxl_object_mgr_create_final_ec_key(STDLL_TokData_t *tokdata,
                              SESSION *sess,
                              OBJECT *obj, CK_OBJECT_HANDLE *handle)
{
    CK_BBOOL priv_obj;
    CK_RV rc;
    CK_ATTRIBUTE *attr = NULL;
    unsigned char *attribute_buffer = NULL;
    uint32_t key_id = 0, attribute_size, ec_key_size, ec_key[192] = {0};
    CK_ULONG pub_key_len;
    int i, ret;
    enum sec_alg keyalgo;

    priv_obj = object_is_private(obj);
    if (priv_obj) {
        EC_POINT *point = NULL;
        EC_GROUP *group = NULL;
        BIGNUM *bn_priv = NULL;
        unsigned char *pub_key = NULL;
        unsigned char *ec_point = NULL;
        point_conversion_form_t form;
        CK_BYTE priv_key[48];
        CK_ULONG priv_key_len, ec_point_len;
        int nid;
        rc = template_attribute_get_non_empty(obj->template, CKA_EC_PARAMS, &attr);
        if (rc != CKR_OK) {
            TRACE_ERROR("Could not find CKA_EC_PARAMS for the key.\n");
            goto error;
        }

        if (sizeof(prime256v1) == attr->ulValueLen &&
            (memcmp(attr->pValue, &prime256v1, attr->ulValueLen) == 0)) {
            keyalgo = SEC_ALG_ECDSA_P256;
            nid = NID_X9_62_prime256v1;
            priv_key_len = 32;
        }
        else if (sizeof(secp384r1) == attr->ulValueLen &&
            (memcmp(attr->pValue, &secp384r1, attr->ulValueLen) == 0)) {
            keyalgo = SEC_ALG_ECDSA_P384;
            nid = NID_secp384r1;
            priv_key_len = 48;
        }
        else {
            TRACE_ERROR("Algo not supported by MXL.\n");
            rc = CKR_CURVE_NOT_SUPPORTED;
            goto error;
        }
        rc = template_attribute_get_non_empty(obj->template, CKA_VALUE, &attr);
        if (rc != CKR_OK) {
            TRACE_ERROR("Could not find CKA_VALUE for the key.\n");
            goto error;
        }
        memcpy(priv_key, attr->pValue, priv_key_len);
        group = EC_GROUP_new_by_curve_name(nid);
        if (group == NULL) {
            TRACE_ERROR("EC_GROUP_new_by_curve_name failed\n");
            rc = CKR_CURVE_NOT_SUPPORTED;
            goto done;
        }

        point = EC_POINT_new(group);
        if (point == NULL) {
            TRACE_ERROR("EC_POINT_new failed\n");
            rc = CKR_FUNCTION_FAILED;
            goto done;
        }

        bn_priv = BN_bin2bn(priv_key, priv_key_len, NULL);
        if (bn_priv == NULL) {
            rc = CKR_FUNCTION_FAILED;
            goto done;
        }

        if (!EC_POINT_mul(group, point, bn_priv, NULL, NULL, NULL)) {
            TRACE_ERROR("EC_POINT_mul failed\n");
            rc = CKR_FUNCTION_FAILED;
            goto done;
        }

        form = EC_GROUP_get_point_conversion_form(group);
        pub_key_len = EC_POINT_point2buf(group, point, form, &pub_key, NULL);
        if (pub_key_len == 0) {
            TRACE_ERROR("EC_POINT_point2buf failed\n");
            rc = CKR_FUNCTION_FAILED;
            goto done;
        }
        ec_point = (unsigned char *)&ec_key[0];
        ec_point[0] = 0x4;
        ec_point[1] = (CK_BYTE)pub_key_len & 0xFF;
        memcpy(&ec_point[2], pub_key, pub_key_len);
        ec_point_len = pub_key_len + 2;
        rc = template_build_update_attribute(obj->template, CKA_EC_POINT, ec_point, ec_point_len);
        if (rc != CKR_OK) {
            TRACE_ERROR("%s template_build_update_attribute failed with rc=0x%lx\n", __func__, rc);
            goto done;
        }
        template_remove_attribute(obj->template, CKA_VALUE);
        mxl_rng((CK_BYTE *)&key_id, MXL_SECURE_STORAGE_NAME_LENGTH);
        rc = template_build_update_attribute(obj->template, CKA_MXL_SECURE_STORAGE_HANDLE,(CK_BYTE *)&key_id, sizeof(key_id));
        if (rc != CKR_OK) {
            TRACE_ERROR("%s template_build_update_attribute failed with rc=0x%lx\n", __func__, rc);
            goto done;
        }
        ec_key[0] = 0x2; /* Plain Text */
        ec_key[1] = (uint32_t) keyalgo;
        ec_key[2] = priv_key_len;
        ec_key[3] = 0;
        ec_key[4] = priv_key_len;
        memcpy(&ec_key[5], priv_key, priv_key_len);
        i = priv_key_len/4;
        ec_key[5 + i] = 0;
        ec_key[6 + i] = 0;
        attribute_buffer =  (unsigned char*)&ec_key[0] + 32 + priv_key_len;
        attribute_size = 0;
        mxltok_generate_mxl_attribute(obj->template, attribute_buffer , &attribute_size);
        attribute_size = (attribute_size + 3) & ~0x3;
        ec_key[7 + i] = attribute_size;
        ec_key_size = 32 + priv_key_len + attribute_size;
        ret = mxl_securestore_write_key(key_id, (uint8_t *)&ec_key[0], ec_key_size);
        if (ret != 0) {
            TRACE_ERROR("mxl_securestore_write_key failed with error %d\r\n",ret);
            rc = CKR_FUNCTION_FAILED;
        }
done:
        if (point != NULL)
            EC_POINT_free(point);
        if (group != NULL)
            EC_GROUP_free(group);
        if (bn_priv != NULL)
            BN_free(bn_priv);
        if (pub_key != NULL)
            OPENSSL_free(pub_key);

    } else {
        CK_BBOOL mxl_key_flag = TRUE;
        rc = template_attribute_get_non_empty(obj->template, CKA_EC_PARAMS, &attr);
        if (rc != CKR_OK) {
            TRACE_ERROR("Could not find CKA_EC_PARAMS for the key.\n");
            goto error;
        }
        if (sizeof(prime256v1) == attr->ulValueLen &&
            (memcmp(attr->pValue, &prime256v1, attr->ulValueLen) == 0)) {
            keyalgo = SEC_ALG_ECDSA_P256;
        }
        else if (sizeof(secp384r1) == attr->ulValueLen &&
            (memcmp(attr->pValue, &secp384r1, attr->ulValueLen) == 0)) {
            keyalgo = SEC_ALG_ECDSA_P384;
        }
        else {
            TRACE_ERROR("Algo not supported by MXL.\n");
            rc = CKR_CURVE_NOT_SUPPORTED;
            goto error;
        }
        rc = template_attribute_get_non_empty(obj->template, CKA_EC_POINT, &attr);
        if (rc != CKR_OK) {
            TRACE_ERROR("Could not find CKA_VALUE for the key.\n");
            goto error;
        }
        pub_key_len = attr->ulValueLen - 3;
        mxl_rng((CK_BYTE *)&key_id, MXL_SECURE_STORAGE_NAME_LENGTH);
        rc = template_build_update_attribute(obj->template, CKA_MXL_SECURE_STORAGE_HANDLE,(CK_BYTE *)&key_id, sizeof(key_id));
        if (rc != CKR_OK) {
            TRACE_ERROR("%s template_build_update_attribute failed with rc=0x%lx\n", __func__, rc);
            goto error;;
        }
        rc = template_build_update_attribute(obj->template, CKA_MXL_KEY_TYPE, (CK_BYTE *)&mxl_key_flag, sizeof(CK_BBOOL));
        if (rc != CKR_OK) {
            TRACE_ERROR("%s template_build_update_attribute failed with rc=0x%lx\n", __func__, rc);
            goto error;
        }
        ec_key[0] = 0x2; /* Plain Text */
        ec_key[1] = (uint32_t) keyalgo;
        ec_key[2] = pub_key_len;
        ec_key[3] = pub_key_len;
        memcpy(&ec_key[4], (unsigned char*)attr->pValue + 3, pub_key_len);
        i = pub_key_len/4;
        ec_key[4 + i] = 0;
        ec_key[5 + i] = 0;
        attribute_buffer =  (unsigned char*)&ec_key[0] + 28 + pub_key_len;
        attribute_size = 0;
        mxltok_generate_mxl_attribute(obj->template, attribute_buffer , &attribute_size);
        attribute_size = (attribute_size + 3) & ~0x3;
        ec_key[6 + i] = attribute_size;
        ec_key_size = 32 + pub_key_len + attribute_size;
        ret = mxl_securestore_write_key(key_id, (uint8_t *)&ec_key[0], ec_key_size);
        if (ret != 0) {
            TRACE_ERROR("mxl_securestore_write_key failed with error %d\r\n",ret);
            rc = CKR_FUNCTION_FAILED;
        }
    }

    rc = mxl_object_mgr_create_final_internal(tokdata, sess, obj, handle, key_id);
    if (rc != CKR_OK) {
        TRACE_ERROR("mxl_object_mgr_create_final_internal failed.\n");
        goto error;
    }

error:
    return rc;
}

CK_RV mxl_object_mgr_create_final(STDLL_TokData_t *tokdata,
                              SESSION *sess,
                              OBJECT *obj, CK_OBJECT_HANDLE *handle)
{
    CK_RV rc;
    CK_ATTRIBUTE *attr = NULL;
    CK_BBOOL found;
    CK_ULONG class, subclass;
    if (!sess || !obj || !handle) {
        TRACE_ERROR("Invalid function arguments.\n");
        return CKR_FUNCTION_FAILED;
    }
    found = template_get_class(obj->template, &class, &subclass);
    if (found == FALSE) {
        TRACE_ERROR("Failed to find CKA_CLASS in object template.\n");
        return CKR_FUNCTION_FAILED;
    }
    if (class == CKO_CERTIFICATE) {
        rc = mxl_object_mgr_create_final_certificate(tokdata, sess, obj, handle);
        if (rc != CKR_OK) {
            TRACE_ERROR("%s mxl_object_mgr_create_final_certificate failed with rc=0x%lx\n", __func__, rc);
        }
        return rc;
    }
    rc = template_attribute_get_non_empty(obj->template, CKA_EC_PARAMS, &attr);
    if (rc == CKR_OK) {
        rc = mxl_object_mgr_create_final_ec_key(tokdata, sess, obj, handle);
    }
    else {
        rc = mxl_object_mgr_create_final_rsa_key(tokdata, sess, obj, handle);
    }
    return rc;
}

static void mxltok_generate_mxl_attribute(TEMPLATE *tmpl, unsigned char *attribute_buffer, uint32_t *attribute_length)
{
    DL_NODE *node = NULL;
    CK_ATTRIBUTE *a = NULL;
    uint32_t total_length = 0, attribute_len;
    mxl_attribute_t attr;
    *attribute_length = 0;
    node = tmpl->attribute_list;
    while (node) {
        a = (CK_ATTRIBUTE *) node->data;
        attr.type = a->type;
        attr.len = a->ulValueLen;
        memcpy(attribute_buffer, &attr, sizeof(mxl_attribute_t));
        memcpy(attribute_buffer + sizeof(mxl_attribute_t), a->pValue, a->ulValueLen);
        attribute_len = sizeof(mxl_attribute_t) + a->ulValueLen;
        attribute_buffer += attribute_len;
        total_length += attribute_len;
        node = node->next;
    }
    *attribute_length = total_length;
}

static CK_RV mxltok_generate_key_pair(STDLL_TokData_t *tokdata,
                         SESSION *sess,
                         CK_MECHANISM_PTR pMechanism,
                         CK_ATTRIBUTE_PTR pPublicKeyTemplate,
                         CK_ULONG ulPublicKeyAttributeCount,
                         CK_ATTRIBUTE_PTR pPrivateKeyTemplate,
                         CK_ULONG ulPrivateKeyAttributeCount,
                         CK_OBJECT_HANDLE_PTR phPublicKey,
                         CK_OBJECT_HANDLE_PTR phPrivateKey)
{
    CK_RV rc = CKR_OK;
    enum sec_alg keyalgo;
    struct seccrypto_gen_key genkey = {0};
    TEEC_Operation op ={0};
    OBJECT *public_key_obj = NULL;
    struct ecdsa_key_pair *key_pair = NULL;
    OBJECT *private_key_obj = NULL;
    CK_ULONG priv_ktype, publ_ktype;
    CK_ULONG class;
    CK_ATTRIBUTE *attr = NULL;
    uint8_t pub_key_ec_point[128];

    unsigned char *attribute_buffer = NULL;
    int ret, pub_key_ec_point_len, pub_key_len;
    char key_name[MXL_KEYLENGTH_MAX] = {0};
    uint32_t key_id;

    if (pMechanism->mechanism != CKM_ECDSA_KEY_PAIR_GEN) {
        TRACE_ERROR("Key Type not supported\n");
        return CKR_MECHANISM_INVALID;
    }

    /* Get the keytype to use when creating the key object */
    rc = pkcs_get_keytype(pPrivateKeyTemplate, ulPrivateKeyAttributeCount,
                          pMechanism, &priv_ktype, &class);
    if (rc != CKR_OK) {
        TRACE_ERROR("%s get_keytype failed with rc=0x%lx\n", __func__, rc);
        goto error;
    }

    rc = pkcs_get_keytype(pPublicKeyTemplate, ulPublicKeyAttributeCount,
                          pMechanism, &publ_ktype, &class);
    if (rc != CKR_OK) {
        TRACE_ERROR("%s get_keytype failed with rc=0x%lx\n", __func__, rc);
        goto error;
    }

    /* Now build the skeleton key. */
    rc = object_mgr_create_skel(tokdata, sess, pPublicKeyTemplate,
                                ulPublicKeyAttributeCount, MODE_KEYGEN,
                                CKO_PUBLIC_KEY, publ_ktype, &public_key_obj);
    if (rc != CKR_OK) {
        TRACE_DEVEL("%s Object mgr create skeleton failed\n", __func__);
        goto error;
    }

    rc = object_mgr_create_skel(tokdata, sess, pPrivateKeyTemplate,
                                ulPrivateKeyAttributeCount, MODE_KEYGEN,
                                CKO_PRIVATE_KEY, priv_ktype, &private_key_obj);
    if (rc != CKR_OK) {
        TRACE_DEVEL("%s Object mgr create skeleton failed\n", __func__);
        goto error;
    }

    rc = template_attribute_get_non_empty(public_key_obj->template, CKA_EC_PARAMS, &attr);
    if (rc != CKR_OK) {
        TRACE_ERROR("Could not find CKA_EC_PARAMS for the key.\n");
        return CK_FALSE;
    }

    if (sizeof(prime256v1) == attr->ulValueLen &&
        (memcmp(attr->pValue, &prime256v1, attr->ulValueLen) == 0)) {
        keyalgo = SEC_ALG_ECDSA_P256;
    }
    else if (sizeof(secp384r1) == attr->ulValueLen &&
        (memcmp(attr->pValue, &secp384r1, attr->ulValueLen) == 0)) {
        keyalgo = SEC_ALG_ECDSA_P384;
    }
    else {
        TRACE_ERROR("Algo not supported by MXL.\n");
        rc = CKR_CURVE_NOT_SUPPORTED;
        return rc;
    }

    mxl_rng((CK_BYTE *)&key_id, MXL_SECURE_STORAGE_NAME_LENGTH);
    mxl_securestore_create_keypair_object_name(key_name, key_id);

    attribute_buffer = calloc(1, MAX_BUF_SIZE);
    if (attribute_buffer == NULL) {
        TRACE_ERROR("%s\n", ock_err(ERR_HOST_MEMORY));
        rc = CKR_HOST_MEMORY;
        goto error;
    }
    genkey.public_attribute_size = 0;
    genkey.public_key_attribute = (uint32_t *)attribute_buffer;
    mxltok_generate_mxl_attribute(public_key_obj->template, attribute_buffer , &genkey.public_attribute_size);
    genkey.public_attribute_size = (genkey.public_attribute_size + 3) & ~0x3;
    attribute_buffer +=  genkey.public_attribute_size;

    genkey.private_attribute_size = 0;
    genkey.private_key_attribute = (uint32_t *)attribute_buffer;
    mxltok_generate_mxl_attribute(private_key_obj->template, attribute_buffer, &genkey.private_attribute_size);
    genkey.private_attribute_size = (genkey.private_attribute_size + 3) & ~0x3;
    attribute_buffer = (unsigned char *)genkey.public_key_attribute;

    rc = XProcLock(tokdata);
    if (rc != CKR_OK) {
        TRACE_ERROR("Failed to get Process Lock.\n");
        goto error;
    }

    if (mxl_init_scsa_session() != TEEC_SUCCESS) {
        TRACE_ERROR("mxl_init_scsa_session failed\n");
        XProcUnLock(tokdata);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if (mxl_prepare_generate_key_pair(key_name, keyalgo, &genkey, &op) < 0) {
        TRACE_ERROR("mxl_prepare_generate_key_pair failed\r\n");
        XProcUnLock(tokdata);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    ret = mxl_execute_scsa_command(TA_SECURE_CRYPTO_GEN_KEYPAIR, &op);
    if (ret != TEEC_SUCCESS) {
        TRACE_ERROR("TEE execute SCSA command TA_SECURE_CRYPTO_GEN_KEYPAIR failed with error = %x \n", ret);
        XProcUnLock(tokdata);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if (genkey.sst_params.handle)
	     securestore_close(genkey.sst_params.handle);
    mxl_exit_scsa_session();

    rc = XProcUnLock(tokdata);
    if (rc != CKR_OK) {
        TRACE_ERROR("Failed to release Process Lock.\n");
        goto error;
    }

    key_pair = genkey.key_ptr;
    pub_key_len = key_pair->pub_key.Qx.num_len +  key_pair->pub_key.Qy.num_len;
    pub_key_ec_point_len =  pub_key_len + 3;
    pub_key_ec_point[0] = 0x4;
    pub_key_ec_point[1] = (pub_key_len + 1) & 0xFF;
    pub_key_ec_point[2] = 0x4;
    memcpy(&pub_key_ec_point[3], key_pair->pub_key.Qx.num_ptr, key_pair->pub_key.Qx.num_len);
    memcpy(&pub_key_ec_point[3 + key_pair->pub_key.Qx.num_len],  key_pair->pub_key.Qy.num_ptr, key_pair->pub_key.Qy.num_len);

    rc = template_build_update_attribute(public_key_obj->template, CKA_EC_POINT, &pub_key_ec_point[0], pub_key_ec_point_len);
    if (rc != CKR_OK) {
        TRACE_ERROR("%s ttemplate_build_update_attribute failed with rc=0x%lx\n", __func__, rc);
        goto error;
    }

    rc = template_build_update_attribute(private_key_obj->template, CKA_EC_POINT, &pub_key_ec_point[0], pub_key_ec_point_len);
    if (rc != CKR_OK) {
        TRACE_ERROR("%s template_update_attribute failed with rc=0x%lx\n", __func__, rc);
        goto error;
    }

    /* copy CKA_EC_PARAMS to private template  */
    rc = template_attribute_get_non_empty(public_key_obj->template, CKA_EC_PARAMS, &attr);
    if (rc == CKR_OK) {
        rc = template_build_update_attribute(private_key_obj->template, attr->type, attr->pValue, attr->ulValueLen);
        if (rc != CKR_OK) {
            TRACE_ERROR("%s template_build_update_attribute failed with " "rc=0x%lx\n", __func__, rc);
            goto error;
        }
    }

    rc = template_build_update_attribute(private_key_obj->template, CKA_MXL_SECURE_STORAGE_HANDLE,(CK_BYTE *)&key_id, sizeof(key_id));
    if (rc != CKR_OK) {
        TRACE_ERROR("%s template_build_update_attribute failed with " "rc=0x%lx\n", __func__, rc);
        goto error;
    }

    rc = template_build_update_attribute(public_key_obj->template, CKA_MXL_SECURE_STORAGE_HANDLE,(CK_BYTE *)&key_id, sizeof(key_id));
    if (rc != CKR_OK) {
        TRACE_ERROR("%s template_update_attribute failed with " "rc=0x%lx\n", __func__, rc);
        goto error;
    }

    /* Keys should be fully constructed,
     * assign object handles and store keys.
     */
    rc = mxl_object_mgr_create_final_internal(tokdata, sess, public_key_obj, phPublicKey, 0);
    if (rc != CKR_OK) {
        TRACE_DEVEL("%s Object mgr create final failed\n", __func__);
        goto error;
    }

    rc = mxl_object_mgr_create_final_internal(tokdata, sess, private_key_obj, phPrivateKey, key_id);
    if (rc != CKR_OK) {
        TRACE_DEVEL("%s Object mgr create final failed for Private Key \n", __func__);
        goto error;
    }
error:
    if ((rc != CKR_OK) && key_pair) {
        mxl_securestore_delete_keypair(key_id);
    }
    if (key_pair)
        free(key_pair);
    if (attribute_buffer)
        free(attribute_buffer);
    return rc;

}

CK_RV SC_GenerateKeyPair(STDLL_TokData_t *tokdata,
                         ST_SESSION_HANDLE *sSession,
                         CK_MECHANISM_PTR pMechanism,
                         CK_ATTRIBUTE_PTR pPublicKeyTemplate,
                         CK_ULONG ulPublicKeyAttributeCount,
                         CK_ATTRIBUTE_PTR pPrivateKeyTemplate,
                         CK_ULONG ulPrivateKeyAttributeCount,
                         CK_OBJECT_HANDLE_PTR phPublicKey,
                         CK_OBJECT_HANDLE_PTR phPrivateKey)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    if (!pMechanism || !phPublicKey || !phPrivateKey ||
        (!pPublicKeyTemplate && (ulPublicKeyAttributeCount != 0)) ||
        (!pPrivateKeyTemplate && (ulPrivateKeyAttributeCount != 0))) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        rc = CKR_ARGUMENTS_BAD;
        goto done;
    }
    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }
    //set the handle into the session.
    sess->handle = sSession->sessionh;
    rc = tokdata->policy->is_mech_allowed(tokdata->policy, pMechanism, NULL,
                                          POLICY_CHECK_KEYGEN, sess);
    if (rc != CKR_OK) {
        TRACE_ERROR("POLICY VIOLATION: Keypair generation mechanism not allowed\n");
        goto done;
    }
    if (pin_expired(&sess->session_info,
                    tokdata->nv_token_data->token_info.flags) == TRUE) {
        TRACE_ERROR("%s\n", ock_err(ERR_PIN_EXPIRED));
        rc = CKR_PIN_EXPIRED;
        goto done;
    }

    rc = mxltok_generate_key_pair(tokdata, sess, pMechanism,
                                   pPublicKeyTemplate,
                                   ulPublicKeyAttributeCount,
                                   pPrivateKeyTemplate,
                                   ulPrivateKeyAttributeCount,
                                   phPublicKey, phPrivateKey);
    if (rc != CKR_OK)
        TRACE_DEVEL("mxltok_generate_key_pair() failed.\n");
done:
    TRACE_INFO("C_GenerateKeyPair: rc = 0x%08lx, sess = %ld, mech = 0x%lx\n",
               rc, (sess == NULL) ? -1 : ((CK_LONG) sess->handle),
               (pMechanism ? pMechanism->mechanism : (CK_ULONG)-1));

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

#ifdef DEBUG
    CK_ATTRIBUTE *attr;
    CK_ULONG i;

    if (rc == CKR_OK) {
        TRACE_DEBUG("Public handle: %lu, Private handle: %lu\n",
                    *phPublicKey, *phPrivateKey);
    }

    TRACE_DEBUG("Public Template:\n");
    attr = pPublicKeyTemplate;
    for (i = 0; i < ulPublicKeyAttributeCount && attr != NULL; i++, attr++) {
        TRACE_DEBUG_DUMPATTR(attr);
    }

    TRACE_DEBUG("Private Template:\n");
    attr = pPrivateKeyTemplate;
    for (i = 0; i < ulPrivateKeyAttributeCount && attr != NULL; i++, attr++) {
        TRACE_DEBUG_DUMPATTR(attr);
    }
#endif

    return rc;
}

CK_RV SC_GetAttributeValue(STDLL_TokData_t *tokdata,
                           ST_SESSION_HANDLE *sSession,
                           CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate,
                           CK_ULONG ulCount)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }

    rc = object_mgr_get_attribute_values(tokdata, sess, hObject, pTemplate,
                                         ulCount);
    if (rc != CKR_OK)
        TRACE_DEVEL("object_mgr_get_attribute_value() failed.\n");

done:
    TRACE_DEVEL("SC_GetAttributeValue: rc = 0x%08lx, handle = %lu\n",
               rc, hObject);

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

#ifdef DEBUG
    CK_ATTRIBUTE *attr = NULL;
    CK_BYTE val[4];
    CK_ULONG i;

    attr = pTemplate;
    for (i = 0; i < ulCount && attr != NULL; i++, attr++) {
        TRACE_DEBUG("%lu: Attribute type: 0x%08lx, Value Length: %lu\n",
                    i, attr->type, attr->ulValueLen);
        if (rc == CKR_OK && attr->ulValueLen != CK_UNAVAILABLE_INFORMATION) {
            if (attr->ulValueLen >= sizeof(val) && attr->pValue != NULL) {
                memset(val, 0, sizeof(val));
                memcpy(val, attr->pValue, attr->ulValueLen > sizeof(val) ?
                       sizeof(val) : attr->ulValueLen);
                TRACE_DEBUG("First 4 bytes: %02x %02x %02x %02x\n",
                        val[0], val[1], val[2], val[3]);
            }
        }
    }
#endif

    return rc;
}

CK_RV mxltok_set_attribute_value(STDLL_TokData_t *tokdata,
                           CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate,
                           CK_ULONG ulCount, uint32_t key_id)
{
    CK_RV rc = CKR_OK;
    int ret;
    TEEC_Operation op ={0};
    mxl_attribute_t *mxl_attr;
    struct seccrypto_set_attribute attribute = {0};
    char key_name[MXL_KEYLENGTH_MAX] = {0};
    OBJECT_MAP *map = NULL;
    CK_ATTRIBUTE *attr = pTemplate;

    if (ulCount != 1) {
        TRACE_ERROR("%s: more than 1 attributes not supported\n", __func__);
        return  CKR_ARGUMENTS_BAD;
    }

    attribute.key_attribute_size = sizeof(mxl_attribute_t) + attr->ulValueLen;
    attribute.key_attributes = calloc(1,  attribute.key_attribute_size);
    if (attribute.key_attributes == NULL) {
        TRACE_ERROR("%s\n", ock_err(ERR_HOST_MEMORY));
        rc = CKR_HOST_MEMORY;
        goto error;
    }
    mxl_attr =  (mxl_attribute_t *)attribute.key_attributes;
    mxl_attr->type = attr->type;
    mxl_attr->len = attr->ulValueLen;
    memcpy(attribute.key_attributes + sizeof(mxl_attribute_t), attr->pValue, attr->ulValueLen);

    map = bt_get_node_value(&tokdata->object_map_btree, hObject);
    if (!map) {
        TRACE_ERROR("%s handle: %lu\n", ock_err(ERR_OBJECT_HANDLE_INVALID),hObject);
        rc = CKR_OBJECT_HANDLE_INVALID;
        goto error;
    }
    if (map->is_private) {
        attribute.type = ATTRIBUTE_TYPE_PRIVATE;
    } else {
        attribute.type = ATTRIBUTE_TYPE_PUBLIC;
    }

    mxl_securestore_create_keypair_object_name(key_name, key_id);

    if (mxl_init_scsa_session() != TEEC_SUCCESS) {
        TRACE_ERROR("mxl_init_scsa_session failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if (mxl_prepare_set_attribute(key_name, &attribute, &op) < 0) {
        TRACE_ERROR("mxl_prepare_set_attribute failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }
    ret = mxl_execute_scsa_command(TA_SECURE_CRYPTO_SET_ATTRIBUTE, &op);
    if (ret != TEEC_SUCCESS) {
        TRACE_ERROR("TEE execute SCSA command TA_SECURE_CRYPTO_SET_ATTRIBUTE failed with error = %x \n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    rc = CKR_OK;

error:
    if (mxl_attr)
        free(mxl_attr);
    if (attribute.sst_params.handle)
        securestore_close(attribute.sst_params.handle);
    mxl_exit_scsa_session();
    return rc;

}

CK_RV SC_SetAttributeValue(STDLL_TokData_t *tokdata,
                           ST_SESSION_HANDLE *sSession,
                           CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate,
                           CK_ULONG ulCount)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;
    uint32_t key_id;
    CK_ATTRIBUTE key_id_template[] = {
        {CKA_MXL_SECURE_STORAGE_HANDLE, &key_id, sizeof(key_id)},
    };

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }

    rc = object_mgr_get_attribute_values(tokdata, sess, hObject, key_id_template, 1);
    if (rc != CKR_OK) {
        TRACE_ERROR("object_mgr_get_attribute_value() failed, rc =%ld.\n",rc);
        goto done;
    }

    rc = XProcLock(tokdata);
    if (rc != CKR_OK) {
        TRACE_ERROR("Failed to get process lock.\n");
        goto done;
    }

    rc = mxltok_set_attribute_value(tokdata, hObject, pTemplate, ulCount, key_id);
    XProcUnLock(tokdata);

    if (rc != CKR_OK) {
        TRACE_ERROR("mxltok_set_attribute_value failed with error rc = 0x%08lx\n", rc);
        goto done;
    }

    rc = object_mgr_set_attribute_values(tokdata, sess, hObject, pTemplate,
                                         ulCount);
    if (rc != CKR_OK)
        TRACE_DEVEL("object_mgr_set_attribute_values() failed.\n");

done:
    TRACE_DEVEL("SC_SetAttributeValue: rc = 0x%08lx, handle = %lu\n",
               rc, hObject);

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

#ifdef DEBUG
    CK_ATTRIBUTE *attr = NULL;
    CK_BYTE val[4];
    CK_ULONG i;

    attr = pTemplate;
    for (i = 0; i < ulCount && attr != NULL; i++, attr++) {
        TRACE_DEBUG("%lu: Attribute type: 0x%08lx, Value Length: %lu\n",
                    i, attr->type, attr->ulValueLen);

        if (attr->ulValueLen >= sizeof(val) && attr->pValue != NULL) {
            memset(val, 0, sizeof(val));
            memcpy(val, attr->pValue, attr->ulValueLen > sizeof(val) ?
                                            sizeof(val) : attr->ulValueLen);
            TRACE_DEBUG("First 4 bytes: %02x %02x %02x %02x\n",
                        val[0], val[1], val[2], val[3]);
        }
    }
#endif

    return rc;
}

CK_RV SC_SignInit(STDLL_TokData_t *tokdata, ST_SESSION_HANDLE *sSession,
                  CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;
    uint32_t key_id;
    CK_BYTE modulus[384];
    CK_BYTE ec_params[sizeof(prime256v1)] = {0};
    int sign_algo;
    CK_ATTRIBUTE key_id_template[] = {
        {CKA_MXL_SECURE_STORAGE_HANDLE, &key_id, sizeof(key_id)}
    };
    CK_ATTRIBUTE ec_params_template[] = {
        {CKA_EC_PARAMS, &ec_params[0], sizeof(ec_params)}
    };
    CK_ATTRIBUTE modulus_template[] = {
        {CKA_MODULUS, &modulus[0], sizeof(modulus)}
    };

    TRACE_INFO("C_SignInit: mech = 0x%lx\n",(pMechanism ? pMechanism->mechanism : (CK_ULONG)-1));

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    if (!pMechanism) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        rc = CKR_ARGUMENTS_BAD;
        goto done;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }
    //set the handle into the session.
    sess->handle = sSession->sessionh;

    if (pin_expired(&sess->session_info,
                    tokdata->nv_token_data->token_info.flags) == TRUE) {
        TRACE_ERROR("%s\n", ock_err(ERR_PIN_EXPIRED));
        rc = CKR_PIN_EXPIRED;
        goto done;
    }

    if (sess->sign_ctx.active == TRUE) {
        rc = CKR_OPERATION_ACTIVE;
        TRACE_ERROR("%s\n", ock_err(ERR_OPERATION_ACTIVE));
        goto done;
    }
    rc = object_mgr_get_attribute_values(tokdata, sess, hKey, key_id_template, 1);
    if (rc != CKR_OK) {
        TRACE_DEVEL("object_mgr_get_attribute_value() failed, rc =%ld.\n",rc);
        goto done;
    }
    rc = object_mgr_get_attribute_values(tokdata, sess, hKey, ec_params_template, 1);
    if (rc == CKR_OK) {
        if (memcmp(&ec_params, &prime256v1, sizeof(prime256v1))== 0) {
            sign_algo = SEC_ALG_ECDSA_P256;
        }
        else if (memcmp(&ec_params, &secp384r1, sizeof(secp384r1))== 0) {
            sign_algo = SEC_ALG_ECDSA_P384;
        }
        else {
            TRACE_ERROR("Algo not supported by MXL.\n");
            rc = CKR_MECHANISM_INVALID;
            goto done;
        }
    }
    else {
        rc = object_mgr_get_attribute_values(tokdata, sess, hKey, modulus_template, 1);
        if (rc == CKR_OK) {
            if (modulus_template[0].ulValueLen == 128)
                sign_algo = SEC_ALG_RSA_1024;
	    else if (modulus_template[0].ulValueLen == 256)
                sign_algo = SEC_ALG_RSA_2048;
            else if (modulus_template[0].ulValueLen  == 384)
                sign_algo = SEC_ALG_RSA_3072;
        }
        else {
            TRACE_ERROR("Algo not supported by MXL.\n");
            rc = CKR_MECHANISM_INVALID;
            goto done;
        }
    }
    sess->sign_ctx.key = key_id;
    sess->sign_ctx.mech.mechanism = pMechanism->mechanism;
    sess->sign_ctx.mech.mechanism |= (sign_algo << 16);
    sess->sign_ctx.active = TRUE;
    rc = CKR_OK;

done:
    TRACE_INFO("SC_SignInit: rc = 0x%08lx, sess = %ld, mech = 0x%lx\n",
               rc, (sess == NULL) ? -1 : (CK_LONG) sess->handle,
               (pMechanism ? pMechanism->mechanism : (CK_ULONG)-1));

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    return rc;
}

static void mxltok_print_signature(unsigned char *r, unsigned char *s, int len)
{
    unsigned char ecdsa_r[100] = {0};
    unsigned char ecdsa_s[100] = {0};
    int i, offset=0;
    for(i = 0; i < len; i++) {
        sprintf((char *)&ecdsa_r[offset], "%02x", r[i]);
        sprintf((char *)&ecdsa_s[offset], "%02x", s[i]);
        offset += 2;
    }
    TRACE_INFO("ECDSA Signature(r): 0x%s\n",ecdsa_r);
    TRACE_INFO("ECDSA Signature(s): 0x%s\n",ecdsa_s);
}

CK_RV mxltok_sign(SESSION *sess, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen)
{
    CK_RV rc = CKR_OK;
    int ret;
    CK_ULONG len;
    TEEC_Operation op ={0};
    struct seccrypto_sign_param sign_params;
    struct seccrypto_load_key loadkey = {0};
    char key_name[MXL_KEYLENGTH_MAX] = {0};
    CK_MECHANISM_TYPE mechanism = sess->sign_ctx.mech.mechanism & 0xFFFF;
    int sign_algo = (sess->sign_ctx.mech.mechanism >> 16) & 0xFFFF;
    uint32_t key_id = sess->sign_ctx.key;
    sign_params.signature = NULL;
    sign_params.sign_algo = sign_algo;
    loadkey.load_algo = sign_algo;
    switch (sign_algo) {
        case SEC_ALG_ECDSA_P256:
            sign_params.hash_algo =  ECDSA_ASN1_SHA256;
            loadkey.hash_algo =  ECDSA_ASN1_SHA256;
            break;
	case SEC_ALG_ECDSA_P384:
            sign_params.hash_algo =  ECDSA_ASN1_SHA384;
            loadkey.hash_algo =  ECDSA_ASN1_SHA384;
            break;
        case SEC_ALG_RSA_1024:
        case SEC_ALG_RSA_2048:
        case SEC_ALG_RSA_3072:
            switch (mechanism) {
                case CKM_SHA1_RSA_PKCS:
                    sign_params.hash_algo = RSA_PKCS1_5_SHA1;
                    loadkey.hash_algo = RSA_PKCS1_5_SHA1;
                    break;
		case CKM_SHA256_RSA_PKCS:
                    sign_params.hash_algo = RSA_PKCS1_5_SHA256;
                    loadkey.hash_algo = RSA_PKCS1_5_SHA256;
                    break;
		case CKM_SHA384_RSA_PKCS:
                    sign_params.hash_algo = RSA_PKCS1_5_SHA384;
                    loadkey.hash_algo = RSA_PKCS1_5_SHA384;
                    break;
                case CKM_SHA512_RSA_PKCS:
                    sign_params.hash_algo = RSA_PKCS1_5_SHA512;
                    loadkey.hash_algo = RSA_PKCS1_5_SHA256;
                    break;
		case CKM_SHA1_RSA_PKCS_PSS:
                    sign_params.hash_algo = RSA_PSS_SHA1;
                    loadkey.hash_algo = RSA_PSS_SHA1;
                    break;
                case CKM_SHA256_RSA_PKCS_PSS:
                    sign_params.hash_algo = RSA_PSS_SHA256;
                    loadkey.hash_algo = RSA_PSS_SHA256;
                    break;
                case CKM_SHA384_RSA_PKCS_PSS:
                    sign_params.hash_algo = RSA_PSS_SHA384;
                    loadkey.hash_algo = RSA_PSS_SHA384;
                    break;
                case CKM_SHA512_RSA_PKCS_PSS:
                    sign_params.hash_algo = RSA_PSS_SHA512;
                    loadkey.hash_algo = RSA_PSS_SHA512;
                    break;
                default:
                    return CKR_MECHANISM_INVALID;
                    break;
            }
            break;
        default:
            TRACE_ERROR("mxltok_sign failed, wrong sign_algo %d\r\n", sign_algo);
            return CKR_ARGUMENTS_BAD;
            break;
    }
    mxl_securestore_create_keypair_object_name(key_name, key_id);
    loadkey.load_flags = PRIVATE_KEY_PLAINTEXT;
    loadkey.key_type =  KEY_PRIVATE;

    if (mxl_init_scsa_session() != TEEC_SUCCESS) {
        TRACE_ERROR("mxl_init_scsa_session failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if (mxl_prepare_load_key(key_name, &loadkey, &op) < 0) {
        TRACE_ERROR("mxl_prepare_load_key failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    ret = mxl_execute_scsa_command(TA_SECURE_CRYPTO_LOAD_KEY, &op);
    if (ret != TEEC_SUCCESS) {
        TRACE_ERROR("TEE execute SCSA command TA_SECURE_CRYPTO_LOAD_KEY failed with error = %x \n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if (mxl_prepare_generate_sign(&sign_params, (char *)pData, (int)ulDataLen, &op)){
        TRACE_ERROR("mxl_prepare_generate_sign failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    ret = mxl_execute_scsa_command(TA_SECURE_CRYPTO_SIGN, &op);
    if (ret != TEEC_SUCCESS) {
        TRACE_ERROR("TEE execute SCSA command TA_SECURE_CRYPTO_SIGN failed with error = %x \n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if ((sign_algo == SEC_ALG_ECDSA_P256) || (sign_algo == SEC_ALG_ECDSA_P384))  {
        struct ecdsa_signature *ecdsa_sign = NULL;
        ecdsa_sign = (struct ecdsa_signature *)sign_params.signature;
        len = ecdsa_sign->r.num_len;
        memcpy(pSignature, ecdsa_sign->r.num_ptr, len);
        memcpy(pSignature+len, ecdsa_sign->s.num_ptr, ecdsa_sign->s.num_len);
        len += ecdsa_sign->s.num_len;
        mxltok_print_signature(ecdsa_sign->r.num_ptr, ecdsa_sign->s.num_ptr, ecdsa_sign->r.num_len);
    }
    else {
        struct rsa_signature *rsa_sign = NULL;
        rsa_sign = (struct rsa_signature *)sign_params.signature;
        len = rsa_sign->rsa_signature.num_len;
        memcpy(pSignature, rsa_sign->rsa_signature.num_ptr, rsa_sign->rsa_signature.num_len);
    }

    *pulSignatureLen = len;
    rc = CKR_OK;

error:
    if (loadkey.sst_params.handle)
        securestore_close(loadkey.sst_params.handle);
    mxl_exit_scsa_session();
    if (sign_params.signature)
         free(sign_params.signature);
    TRACE_INFO("%s:rc = 0x%08lx\n", __func__, rc);
    return rc;

}

CK_RV mxltok_sign_digest(SESSION *sess, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen)
{
    CK_RV rc = CKR_OK;
    int ret;
    CK_ULONG len;
    TEEC_Operation op ={0};
    struct  seccrypto_sign_digest_param sign_digest_params;
    struct seccrypto_load_key loadkey = {0};
    char key_name[MXL_KEYLENGTH_MAX] = {0};
    CK_MECHANISM_TYPE mech = sess->sign_ctx.mech.mechanism & 0xFFFF;
    int sign_algo = (sess->sign_ctx.mech.mechanism >> 16) & 0xFFFF;
    uint32_t key_id = sess->sign_ctx.key;
    sign_digest_params.signature = NULL;
    sign_digest_params.sign_algo = sign_algo;
    loadkey.load_algo = sign_algo;

    switch (sign_algo) {
        case SEC_ALG_ECDSA_P256:
            sign_digest_params.hash_algo =  ECDSA_ASN1_SHA256;
            loadkey.hash_algo =  ECDSA_ASN1_SHA256;
            break;
        case SEC_ALG_ECDSA_P384:
            sign_digest_params.hash_algo =  ECDSA_ASN1_SHA384;
            loadkey.hash_algo =  ECDSA_ASN1_SHA384;
            break;
        case SEC_ALG_RSA_1024:
        case SEC_ALG_RSA_2048:
        case SEC_ALG_RSA_3072:
            if (mech == CKM_RSA_PKCS) {
               switch (ulDataLen) {
                   case 20:
                       sign_digest_params.hash_algo = RSA_PKCS1_5_SHA1;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA1;
                       break;
                   case 32:
                       sign_digest_params.hash_algo = RSA_PKCS1_5_SHA256;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA256;
                       break;
                   case 48:
                       sign_digest_params.hash_algo = RSA_PKCS1_5_SHA384;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA384;
                       break;
                   case 64:
                       sign_digest_params.hash_algo = RSA_PKCS1_5_SHA512;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA512;
                       break;
                   case 35:
                       sign_digest_params.hash_algo = RSA_PKCS1_5_SHA1;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA1;
                       pData += 15;
                       ulDataLen = 20;
                       break;
                   case 51:
                       sign_digest_params.hash_algo = RSA_PKCS1_5_SHA256;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA256;
                       pData += 19;
                       ulDataLen = 32;
                       break;
                   case 67:
                       sign_digest_params.hash_algo = RSA_PKCS1_5_SHA384;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA384;
                       pData += 19;
                       ulDataLen = 48;
                       break;
                   case 83:
                       sign_digest_params.hash_algo = RSA_PKCS1_5_SHA512;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA512;
                       pData += 19;
                       ulDataLen = 64;
                       break;
                   default:
                       return CKR_MECHANISM_INVALID;
                       break;
               }
            }
            else if (mech == CKM_RSA_X_509 || mech == CKM_RSA_PKCS_PSS) {
               switch (ulDataLen) {
                   case 20:
                       sign_digest_params.hash_algo = RSA_PSS_SHA1;
                       loadkey.hash_algo = RSA_PSS_SHA1;
                       break;
                   case 32:
                       sign_digest_params.hash_algo = RSA_PSS_SHA256;
                       loadkey.hash_algo = RSA_PSS_SHA256;
                       break;
                   case 48:
                       sign_digest_params.hash_algo = RSA_PSS_SHA384;
                       loadkey.hash_algo = RSA_PSS_SHA384;
                       break;
                   case 64:
                       sign_digest_params.hash_algo = RSA_PSS_SHA512;
                       loadkey.hash_algo = RSA_PSS_SHA512;
                       break;
		   default:
                       return CKR_MECHANISM_INVALID;
                       break;
               }
            }
            else {
                TRACE_ERROR("mxltok_sign_digest failed, wrong rsa algo:%ld\r\n",mech);
                return CKR_MECHANISM_INVALID;
            }
            break;
        default:
            return CKR_ARGUMENTS_BAD;
            break;
    }

    if ((sign_algo == SEC_ALG_ECDSA_P384) && (ulDataLen == 32)) {
        sign_digest_params.hash_algo =  ECDSA_ASN1_SHA256;
        loadkey.hash_algo =  ECDSA_ASN1_SHA256;
    }

    mxl_securestore_create_keypair_object_name(key_name, key_id);
    loadkey.load_flags = PRIVATE_KEY_PLAINTEXT;
    loadkey.key_type =  KEY_PRIVATE;

    if (mxl_init_scsa_session() != TEEC_SUCCESS) {
        TRACE_ERROR("mxl_init_scsa_session failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if (mxl_prepare_load_key(key_name, &loadkey, &op) < 0) {
        TRACE_ERROR("mxl_prepare_load_key failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    ret = mxl_execute_scsa_command(TA_SECURE_CRYPTO_LOAD_KEY, &op);
    if (ret != TEEC_SUCCESS) {
        TRACE_ERROR("TEE execute SCSA command TA_SECURE_CRYPTO_LOAD_KEY failed with error = %x \n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if (mxl_prepare_generate_sign_digest(&sign_digest_params, (char *)pData, (int)ulDataLen, &op)){
        TRACE_ERROR("mxl_prepare_generate_sign_digest failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    ret = mxl_execute_scsa_command(TA_SECURE_CRYPTO_SIGN_DIGEST, &op);
    if (ret != TEEC_SUCCESS) {
        TRACE_ERROR("TEE execute SCSA command TA_SECURE_CRYPTO_SIGN_DIGEST failed with error = %x \n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if ((sign_algo == SEC_ALG_ECDSA_P256) || (sign_algo == SEC_ALG_ECDSA_P384))  {
        struct ecdsa_signature *ecdsa_sign = NULL;
        ecdsa_sign = (struct ecdsa_signature *)sign_digest_params.signature;
        len = ecdsa_sign->r.num_len;
        memcpy(pSignature, ecdsa_sign->r.num_ptr, len);
        memcpy(pSignature+len, ecdsa_sign->s.num_ptr, ecdsa_sign->s.num_len);
        len += ecdsa_sign->s.num_len;
        mxltok_print_signature(ecdsa_sign->r.num_ptr, ecdsa_sign->s.num_ptr, ecdsa_sign->r.num_len);
    }
    else {
        struct rsa_signature *rsa_sign = NULL;
        rsa_sign = (struct rsa_signature *)sign_digest_params.signature;
        len = rsa_sign->rsa_signature.num_len;
        memcpy(pSignature, rsa_sign->rsa_signature.num_ptr, rsa_sign->rsa_signature.num_len);
    }

    *pulSignatureLen = len;
    rc = CKR_OK;
error:
    if (loadkey.sst_params.handle)
        securestore_close(loadkey.sst_params.handle);
    mxl_exit_scsa_session();
    if (sign_digest_params.signature)
         free(sign_digest_params.signature);
    TRACE_INFO("%s:rc = 0x%08lx\n", __func__, rc);
    return rc;

}

CK_RV SC_Sign(STDLL_TokData_t *tokdata, ST_SESSION_HANDLE *sSession,
              CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature,
              CK_ULONG_PTR pulSignatureLen)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;
    CK_MECHANISM_TYPE mechanism;
    CK_BBOOL locked = FALSE;

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }
    //set the handle into the session.
    sess->handle = sSession->sessionh;

    if (!pData || !pulSignatureLen) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        rc = CKR_ARGUMENTS_BAD;
        goto done;
    }

    if (sess->sign_ctx.active == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_OPERATION_NOT_INITIALIZED));
        rc = CKR_OPERATION_NOT_INITIALIZED;
        goto done;
    }

    rc = XProcLock(tokdata);
    if (rc != CKR_OK) {
        TRACE_ERROR("Failed to get process lock.\n");
        goto done;
    }
    locked = TRUE;

    mechanism = sess->sign_ctx.mech.mechanism & 0xFFFF;
    switch (mechanism) {
        case CKM_ECDSA:
        case CKM_RSA_PKCS:
        case CKM_RSA_PKCS_PSS:
        case CKM_RSA_X_509:
            rc = mxltok_sign_digest(sess, pData, ulDataLen, pSignature, pulSignatureLen);
            break;
        default:
            rc = mxltok_sign(sess, pData, ulDataLen, pSignature, pulSignatureLen);
            break;
    }

    if (rc != CKR_OK) {
        TRACE_ERROR("mxltok_sign_digest/mxltok_sign failed with error:%ld\r",rc);
    }
done:
    if (locked) {
        if (rc == CKR_OK) {
            rc = XProcUnLock(tokdata);
            if (rc != CKR_OK) {
                TRACE_ERROR("Failed to release Process Lock.\n");
            }
        } else {
            /* return error that occurred first */
            XProcUnLock(tokdata);
        }
    }
    sess->sign_ctx.active = FALSE;
    if (rc != CKR_BUFFER_TOO_SMALL && (rc != CKR_OK || pSignature)) {
        if (sess != NULL)
            sign_mgr_cleanup(tokdata, sess, &sess->sign_ctx);
    }

    TRACE_INFO("SC_Sign: rc = 0x%08lx, sess = %ld, datalen = %ld\n",
               rc, (sess == NULL) ? -1 : (CK_LONG) sess->handle, ulDataLen);

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    return rc;
}

CK_RV SC_VerifyInit(STDLL_TokData_t *tokdata, ST_SESSION_HANDLE *sSession,
                    CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;
    uint32_t key_id;
    CK_BYTE ec_params[sizeof(prime256v1)] = {0};
    CK_BYTE modulus[384];
    int sign_algo;
    CK_ATTRIBUTE key_id_template[] = {
        {CKA_MXL_SECURE_STORAGE_HANDLE, &key_id, sizeof(key_id)}
    };
    CK_ATTRIBUTE ec_params_template[] = {
        {CKA_EC_PARAMS, &ec_params[0], sizeof(ec_params)}
    };
    CK_ATTRIBUTE modulus_template[] = {
        {CKA_MODULUS, &modulus[0], sizeof(modulus)}
    };

    TRACE_INFO("C_VerifyInit: mech = 0x%lx\n",(pMechanism ? pMechanism->mechanism : (CK_ULONG)-1));

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }
    if (!pMechanism) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        rc = CKR_ARGUMENTS_BAD;
        goto done;
    }

    rc = valid_mech(tokdata, pMechanism, CKF_VERIFY);
    if (rc != CKR_OK)
        goto done;

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }
    //set the handle into the session.
    sess->handle = sSession->sessionh;

    if (pin_expired(&sess->session_info,
                    tokdata->nv_token_data->token_info.flags) == TRUE) {
        TRACE_ERROR("%s\n", ock_err(ERR_PIN_EXPIRED));
        rc = CKR_PIN_EXPIRED;
        goto done;
    }

    if (sess->verify_ctx.active == TRUE) {
        rc = CKR_OPERATION_ACTIVE;
        TRACE_ERROR("%s\n", ock_err(ERR_OPERATION_ACTIVE));
        goto done;
    }

    rc = object_mgr_get_attribute_values(tokdata, sess, hKey, key_id_template, 1);
    if (rc != CKR_OK) {
        TRACE_DEVEL("object_mgr_get_attribute_value() failed, rc =%ld.\n",rc);
        goto done;
    }
    if (key_id == 0) {
        TRACE_ERROR("Wrong Public keyid.\n");
        rc = CKR_MECHANISM_INVALID;
        goto done;
    }

    rc = object_mgr_get_attribute_values(tokdata, sess, hKey, ec_params_template, 1);
    if (rc == CKR_OK) {
        if (memcmp(&ec_params, &prime256v1, sizeof(prime256v1))== 0) {
            sign_algo = SEC_ALG_ECDSA_P256;
        }
        else if (memcmp(&ec_params, &secp384r1, sizeof(secp384r1))== 0) {
            sign_algo = SEC_ALG_ECDSA_P384;
        }
        else {
            TRACE_ERROR("Algo not supported by MXL.\n");
            rc = CKR_MECHANISM_INVALID;
            goto done;
        }
    }
    else {
        rc = object_mgr_get_attribute_values(tokdata, sess, hKey, modulus_template, 1);
        if (rc == CKR_OK) {
            if (modulus_template[0].ulValueLen == 128)
                sign_algo = SEC_ALG_RSA_1024;
            else if (modulus_template[0].ulValueLen == 256)
                sign_algo = SEC_ALG_RSA_2048;
            else if (modulus_template[0].ulValueLen  == 384)
                sign_algo = SEC_ALG_RSA_3072;
        }
        else {
            TRACE_ERROR("Algo not supported by MXL.\n");
            rc = CKR_MECHANISM_INVALID;
            goto done;
        }
    }

    sess->verify_ctx.key = key_id;
    sess->verify_ctx.mech.mechanism = pMechanism->mechanism;
    sess->verify_ctx.mech.mechanism |= (sign_algo << 16);
    sess->verify_ctx.active = TRUE;
    rc = CKR_OK;

done:
    TRACE_INFO("SC_VerifyInit: rc = 0x%08lx, sess = %ld, mech = 0x%lx\n",
               rc, (sess == NULL) ? -1 : (CK_LONG) sess->handle,
               (pMechanism ? pMechanism->mechanism : (CK_ULONG)-1));

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    return rc;
}

CK_RV mxltok_verify(SESSION *sess, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen)
{
    CK_RV rc = CKR_OK;
    int ret, sign_len;
    CK_ULONG digest_len;
    TEEC_Operation op ={0};
    struct seccrypto_verify_param verify_params;
    struct seccrypto_load_key loadkey = {0};
    unsigned char *buffer = NULL;
    char key_name[MXL_KEYLENGTH_MAX] = {0};
    CK_MECHANISM_TYPE mechanism = sess->verify_ctx.mech.mechanism & 0xFFFF;
    int sign_algo = (sess->verify_ctx.mech.mechanism >> 16) & 0xFFFF;
    uint32_t key_id = sess->verify_ctx.key;
    verify_params.sign_algo = sign_algo;
    loadkey.load_algo = sign_algo;

    switch (sign_algo) {
        case SEC_ALG_ECDSA_P256:
            verify_params.hash_algo =  ECDSA_ASN1_SHA256;
            loadkey.hash_algo =  ECDSA_ASN1_SHA256;
            break;
	case SEC_ALG_ECDSA_P384:
            verify_params.hash_algo =  ECDSA_ASN1_SHA384;
            loadkey.hash_algo =  ECDSA_ASN1_SHA384;
            break;
        case SEC_ALG_RSA_1024:
        case SEC_ALG_RSA_2048:
        case SEC_ALG_RSA_3072:
            switch (mechanism) {
                case CKM_SHA1_RSA_PKCS:
                    verify_params.hash_algo = RSA_PKCS1_5_SHA1;
                    loadkey.hash_algo = RSA_PKCS1_5_SHA1;
                    break;
                case CKM_SHA256_RSA_PKCS:
                    verify_params.hash_algo = RSA_PKCS1_5_SHA256;
                    loadkey.hash_algo = RSA_PKCS1_5_SHA256;
                    break;
                case CKM_SHA384_RSA_PKCS:
                    verify_params.hash_algo = RSA_PKCS1_5_SHA384;
                    loadkey.hash_algo = RSA_PKCS1_5_SHA384;
                    break;
                case CKM_SHA512_RSA_PKCS:
                    verify_params.hash_algo = RSA_PKCS1_5_SHA512;
                    loadkey.hash_algo = RSA_PKCS1_5_SHA256;
                    break;
                case CKM_SHA1_RSA_PKCS_PSS:
                    verify_params.hash_algo = RSA_PSS_SHA1;
                    loadkey.hash_algo = RSA_PSS_SHA1;
                    break;
                case CKM_SHA256_RSA_PKCS_PSS:
                    verify_params.hash_algo = RSA_PSS_SHA256;
                    loadkey.hash_algo = RSA_PSS_SHA256;
                    break;
                case CKM_SHA384_RSA_PKCS_PSS:
                    verify_params.hash_algo = RSA_PSS_SHA384;
                    loadkey.hash_algo = RSA_PSS_SHA384;
                    break;
                case CKM_SHA512_RSA_PKCS_PSS:
                    verify_params.hash_algo = RSA_PSS_SHA512;
                    loadkey.hash_algo = RSA_PSS_SHA512;
                    break;
                default:
                    return CKR_MECHANISM_INVALID;
                    break;
            }
            break;
        default:
            return CKR_ARGUMENTS_BAD;
            break;
    }

    mxl_securestore_create_keypair_object_name(key_name, key_id);
    loadkey.load_flags = PRIVATE_KEY_PLAINTEXT;
    loadkey.key_type =  KEY_PUBLIC;

    if (mxl_init_scsa_session() != TEEC_SUCCESS) {
        TRACE_ERROR("mxl_init_scsa_session failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if (mxl_prepare_load_key(key_name, &loadkey, &op) < 0) {
        TRACE_ERROR("mxl_prepare_load_key failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    ret = mxl_execute_scsa_command(TA_SECURE_CRYPTO_LOAD_KEY, &op);
    if (ret != TEEC_SUCCESS) {
        TRACE_ERROR("TEE execute SCSA command TA_SECURE_CRYPTO_LOAD_KEY failed with error = %x \n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if ((sign_algo == SEC_ALG_ECDSA_P256) || (sign_algo == SEC_ALG_ECDSA_P384))  {
        struct ecdsa_signature *ecdsa_sign = NULL;
        digest_len = ulSignatureLen/2;
        sign_len = sizeof(*ecdsa_sign) + digest_len + digest_len;
        buffer = calloc(1, sign_len);
        if (buffer == NULL) {
            TRACE_ERROR("%s\n", ock_err(ERR_HOST_MEMORY));
            rc = CKR_HOST_MEMORY;
            goto error;
        }
        ecdsa_sign = (struct ecdsa_signature *) buffer;
        ecdsa_sign->r.num_len = ecdsa_sign->s.num_len = digest_len;
        ecdsa_sign->r.num_ptr = buffer + sizeof(*ecdsa_sign);
        ecdsa_sign->s.num_ptr = ecdsa_sign->r.num_ptr + ecdsa_sign->r.num_len;
        memcpy(ecdsa_sign->r.num_ptr, pSignature, digest_len);
        memcpy(ecdsa_sign->s.num_ptr, pSignature + digest_len, digest_len);
        mxltok_print_signature(ecdsa_sign->r.num_ptr, ecdsa_sign->s.num_ptr, digest_len);
    }
    else {
        struct rsa_signature *rsa_sign = NULL;
        digest_len = ulSignatureLen;
        sign_len = sizeof(*rsa_sign) + digest_len;
        buffer = calloc(1, sign_len);
        if (buffer == NULL) {
            TRACE_ERROR("%s\n", ock_err(ERR_HOST_MEMORY));
            rc = CKR_HOST_MEMORY;
            goto error;
        }
        rsa_sign = (struct rsa_signature *) buffer;
        rsa_sign->rsa_signature.num_len = digest_len;
        rsa_sign->rsa_signature.num_ptr = buffer + sizeof(*rsa_sign);
        memcpy(rsa_sign->rsa_signature.num_ptr, pSignature, digest_len);
    }

    if (mxl_prepare_sign_verify(&verify_params, (char *)pData, (int)ulDataLen, buffer, &op)){
        TRACE_ERROR("mxl_prepare_sign_verify failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }
    ret = mxl_execute_scsa_command(TA_SECURE_CRYPTO_VERIFY, &op);
    if (ret != TEEC_SUCCESS) {
        TRACE_ERROR("TEE execute SCSA command TA_SECURE_CRYPTO_VERIFY failed with error = %x \n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

error:
    if (loadkey.sst_params.handle)
        securestore_close(loadkey.sst_params.handle);
    mxl_exit_scsa_session();
    if (buffer != NULL)
        free(buffer);
    TRACE_INFO("%s:rc = 0x%08lx\n", __func__, rc);
    return rc;
}

CK_RV mxltok_verify_digest(SESSION *sess, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen)
{
    CK_RV rc = CKR_OK;
    int ret, sign_len;
    CK_ULONG digest_len;
    TEEC_Operation op ={0};
    struct seccrypto_digest_verify_param digest_verify_params;
    struct seccrypto_load_key loadkey = {0};
    unsigned char *buffer = NULL;
    char key_name[MXL_KEYLENGTH_MAX] = {0};
    CK_MECHANISM_TYPE mech = sess->verify_ctx.mech.mechanism & 0xFFFF;
    int sign_algo = (sess->verify_ctx.mech.mechanism >> 16) & 0xFFFF;
    uint32_t key_id = sess->verify_ctx.key;
    digest_verify_params.sign_algo = sign_algo;
    loadkey.load_algo = sign_algo;
    switch (sign_algo) {
        case SEC_ALG_ECDSA_P256:
            digest_verify_params.hash_algo =  ECDSA_ASN1_SHA256;
            loadkey.hash_algo =  ECDSA_ASN1_SHA256;
            break;
	case SEC_ALG_ECDSA_P384:
            digest_verify_params.hash_algo =  ECDSA_ASN1_SHA384;
            loadkey.hash_algo =  ECDSA_ASN1_SHA384;
            break;
        case SEC_ALG_RSA_1024:
        case SEC_ALG_RSA_2048:
        case SEC_ALG_RSA_3072:
            if (mech == CKM_RSA_PKCS) {
               switch (ulDataLen) {
                   case 20:
                       digest_verify_params.hash_algo = RSA_PKCS1_5_SHA1;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA1;
                       break;
                   case 32:
                       digest_verify_params.hash_algo = RSA_PKCS1_5_SHA256;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA256;
                       break;
                   case 48:
                       digest_verify_params.hash_algo = RSA_PKCS1_5_SHA384;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA384;
                       break;
                   case 64:
                       digest_verify_params.hash_algo = RSA_PKCS1_5_SHA512;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA512;
                       break;
                   case 35:
                       digest_verify_params.hash_algo = RSA_PKCS1_5_SHA1;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA1;
                       pData += 15;
                       ulDataLen = 20;
                       break;
                   case 51:
                       digest_verify_params.hash_algo = RSA_PKCS1_5_SHA256;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA256;
                       pData += 19;
                       ulDataLen = 32;
                       break;
                   case 67:
                       digest_verify_params.hash_algo = RSA_PKCS1_5_SHA384;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA384;
                       pData += 19;
                       ulDataLen = 48;
                       break;
		   case 112:
                       digest_verify_params.hash_algo = RSA_PKCS1_5_SHA512;
                       loadkey.hash_algo = RSA_PKCS1_5_SHA512;
                       pData += 48;
                       ulDataLen = 64;
                       break;
                   default:
                       return CKR_MECHANISM_INVALID;
                       break;
               }
            }
            else if (mech == CKM_RSA_X_509 || mech == CKM_RSA_PKCS_PSS) {
               switch (ulDataLen) {
                   case 20:
                       digest_verify_params.hash_algo = RSA_PSS_SHA1;
                       loadkey.hash_algo = RSA_PSS_SHA1;
                       break;
                   case 32:
                       digest_verify_params.hash_algo = RSA_PSS_SHA256;
                       loadkey.hash_algo = RSA_PSS_SHA256;
                       break;
                   case 48:
                       digest_verify_params.hash_algo = RSA_PSS_SHA384;
                       loadkey.hash_algo = RSA_PSS_SHA384;
                       break;
                   case 64:
                       digest_verify_params.hash_algo = RSA_PSS_SHA512;
                       loadkey.hash_algo = RSA_PSS_SHA512;
                       break;
                    default:
                       return CKR_MECHANISM_INVALID;
                       break;
               }
            }
            else {
                TRACE_ERROR("mxltok_verify_digest failed, wrong rsa algo:%ld\r\n",mech);
                return CKR_MECHANISM_INVALID;
            }
            break;
        default:
            TRACE_ERROR("mxltok_verify_digest failed, wrong algo\r\n");
            return CKR_ARGUMENTS_BAD;
            break;
    }

    if ((sign_algo == SEC_ALG_ECDSA_P384) && (ulDataLen == 32)) {
        digest_verify_params.hash_algo =  ECDSA_ASN1_SHA256;
        loadkey.hash_algo =  ECDSA_ASN1_SHA256;
    }

    mxl_securestore_create_keypair_object_name(key_name, key_id);
    loadkey.load_flags = PRIVATE_KEY_PLAINTEXT;
    loadkey.key_type =  KEY_PUBLIC;

    if (mxl_init_scsa_session() != TEEC_SUCCESS) {
        TRACE_ERROR("mxl_init_scsa_session failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if (mxl_prepare_load_key(key_name, &loadkey, &op) < 0) {
        TRACE_ERROR("mxl_prepare_load_key failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    ret = mxl_execute_scsa_command(TA_SECURE_CRYPTO_LOAD_KEY, &op);
    if (ret != TEEC_SUCCESS) {
        TRACE_ERROR("TEE execute SCSA command TA_SECURE_CRYPTO_LOAD_KEY failed with error = %x \n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if ((sign_algo == SEC_ALG_ECDSA_P256) || (sign_algo == SEC_ALG_ECDSA_P384))  {
        struct ecdsa_signature *ecdsa_sign = NULL;
        digest_len = ulSignatureLen/2;
        sign_len = sizeof(*ecdsa_sign) + digest_len + digest_len;
        buffer = calloc(1, sign_len);
        if (buffer == NULL) {
            TRACE_ERROR("%s\n", ock_err(ERR_HOST_MEMORY));
            rc = CKR_HOST_MEMORY;
            goto error;
        }
        ecdsa_sign = (struct ecdsa_signature *) buffer;
        ecdsa_sign->r.num_len = ecdsa_sign->s.num_len = digest_len;
        ecdsa_sign->r.num_ptr = buffer + sizeof(*ecdsa_sign);
        ecdsa_sign->s.num_ptr = ecdsa_sign->r.num_ptr + ecdsa_sign->r.num_len;
        memcpy(ecdsa_sign->r.num_ptr, pSignature, digest_len);
        memcpy(ecdsa_sign->s.num_ptr, pSignature + digest_len, digest_len);
        mxltok_print_signature(ecdsa_sign->r.num_ptr, ecdsa_sign->s.num_ptr, digest_len);
    }
    else {
        struct rsa_signature *rsa_sign = NULL;
        digest_len = ulSignatureLen;
        sign_len = sizeof(*rsa_sign) + digest_len;
        buffer = calloc(1, sign_len);
        if (buffer == NULL) {
            TRACE_ERROR("%s\n", ock_err(ERR_HOST_MEMORY));
            rc = CKR_HOST_MEMORY;
            goto error;
        }
        rsa_sign = (struct rsa_signature *) buffer;
        rsa_sign->rsa_signature.num_len = digest_len;
        rsa_sign->rsa_signature.num_ptr = buffer + sizeof(*rsa_sign);
        memcpy(rsa_sign->rsa_signature.num_ptr, pSignature, digest_len);
    }

    if (mxl_prepare_digest_verify(&digest_verify_params, (char *)pData, (int)ulDataLen, buffer, &op)){
        TRACE_ERROR("mxl_prepare_digest_verify failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    ret = mxl_execute_scsa_command(TA_SECURE_CRYPTO_VERIFY_DIGEST, &op);
    if (ret != TEEC_SUCCESS) {
        TRACE_ERROR("TEE execute SCSA command TA_SECURE_CRYPTO_VERIFY_DIGEST failed with error = %x \n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

error:
    if (loadkey.sst_params.handle)
        securestore_close(loadkey.sst_params.handle);
    mxl_exit_scsa_session();
    if (buffer != NULL)
        free(buffer);
    TRACE_INFO("%s:rc = 0x%08lx\n", __func__, rc);
    return rc;
}

CK_RV SC_Verify(STDLL_TokData_t *tokdata, ST_SESSION_HANDLE *sSession,
                CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature,
                CK_ULONG ulSignatureLen)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;
    CK_MECHANISM_TYPE mechanism;
    CK_BBOOL locked = FALSE;
    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }
    //set the handle into the session.
    sess->handle = sSession->sessionh;

    if (!pData || !pSignature) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        rc = CKR_ARGUMENTS_BAD;
        goto done;
    }

    if (sess->verify_ctx.active == FALSE) {
        rc = CKR_OPERATION_NOT_INITIALIZED;
        TRACE_ERROR("%s\n", ock_err(ERR_OPERATION_NOT_INITIALIZED));
        goto done;
    }

    rc = XProcLock(tokdata);
    if (rc != CKR_OK) {
        TRACE_ERROR("Failed to get process lock.\n");
        goto done;
    }
    locked = TRUE;

    mechanism = sess->verify_ctx.mech.mechanism & 0xFFFF;
    switch (mechanism) {
        case CKM_ECDSA:
        case CKM_RSA_PKCS:
        case CKM_RSA_PKCS_PSS:
        case CKM_RSA_X_509:
            rc = mxltok_verify_digest(sess, pData, ulDataLen, pSignature, ulSignatureLen);
            break;
        default:
            rc = mxltok_verify(sess, pData, ulDataLen, pSignature, ulSignatureLen);
            break;
    }

    if (rc != CKR_OK) {
        TRACE_ERROR("mxltok_verify_digest/mxltok_verify failed with error:%ld\r",rc);
    }

done:
    if (locked) {
        if (rc == CKR_OK) {
            rc = XProcUnLock(tokdata);
            if (rc != CKR_OK) {
                TRACE_ERROR("Failed to release Process Lock.\n");
            }
        } else {
            /* return error that occurred first */
            XProcUnLock(tokdata);
        }
    }
    sess->verify_ctx.active = FALSE;

    TRACE_INFO("SC_Verify: rc = 0x%08lx, sess = %ld, datalen = %lu\n",
               rc, (sess == NULL) ? -1 : (CK_LONG) sess->handle, ulDataLen);

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    return rc;
}

CK_RV SC_DecryptInit(STDLL_TokData_t *tokdata, ST_SESSION_HANDLE *sSession,
                    CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;
    uint32_t key_id;
    CK_BYTE modulus[384];
    int sign_algo;
    CK_RSA_PKCS_OAEP_PARAMS *oaep = NULL;
    CK_ATTRIBUTE key_id_template[] = {
        {CKA_MXL_PKCS8_KEY_HANDLE, &key_id, sizeof(key_id)}
    };
    CK_ATTRIBUTE modulus_template[] = {
        {CKA_MODULUS, &modulus[0], sizeof(modulus)}
    };

    TRACE_INFO("C_DecryptInit: mech = 0x%lx\n",(pMechanism ? pMechanism->mechanism : (CK_ULONG)-1));

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }
    if (!pMechanism) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        rc = CKR_ARGUMENTS_BAD;
        goto done;
    }

    rc = valid_mech(tokdata, pMechanism, CKF_DECRYPT);
    if (rc != CKR_OK)
        goto done;

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }
    sess->handle = sSession->sessionh;

    if (pin_expired(&sess->session_info,
                    tokdata->nv_token_data->token_info.flags) == TRUE) {
        TRACE_ERROR("%s\n", ock_err(ERR_PIN_EXPIRED));
        rc = CKR_PIN_EXPIRED;
        goto done;
    }

    if (sess->decr_ctx.active == TRUE) {
        rc = CKR_OPERATION_ACTIVE;
        TRACE_ERROR("%s\n", ock_err(ERR_OPERATION_ACTIVE));
        goto done;
    }

    rc = object_mgr_get_attribute_values(tokdata, sess, hKey, key_id_template, 1);
    if (rc != CKR_OK) {
        TRACE_DEVEL("object_mgr_get_attribute_value() failed, rc =%ld.\n",rc);
        goto done;
    }
    else {
        rc = object_mgr_get_attribute_values(tokdata, sess, hKey, modulus_template, 1);
        if (rc == CKR_OK) {
            if (modulus_template[0].ulValueLen == 128)
                sign_algo = SEC_ALG_RSA_1024;
            else if (modulus_template[0].ulValueLen == 256)
                sign_algo = SEC_ALG_RSA_2048;
            else if (modulus_template[0].ulValueLen  == 384)
                sign_algo = SEC_ALG_RSA_3072;
            else {
                TRACE_ERROR("Algo not supported by MXL.\n");
                rc = CKR_MECHANISM_INVALID;
                goto done;
            }
        }
        else {
            TRACE_ERROR("Algo not supported by MXL.\n");
            rc = CKR_MECHANISM_INVALID;
            goto done;
        }
    }

    sess->decr_ctx.key = key_id;
    sess->decr_ctx.mech.mechanism = pMechanism->mechanism;
    sess->decr_ctx.mech.mechanism |= (sign_algo << 16);
    sess->decr_ctx.active = TRUE;
    sess->digest_ctx.mech.mechanism = 0;
    if (pMechanism->mechanism == CKM_RSA_PKCS_OAEP) {
        oaep = (CK_RSA_PKCS_OAEP_PARAMS *)pMechanism->pParameter;
        if (oaep != NULL &&  pMechanism->ulParameterLen == sizeof(CK_RSA_PKCS_OAEP_PARAMS)){
            sess->digest_ctx.mech.mechanism = oaep->hashAlg;
        }
    }
    rc = CKR_OK;

done:
    TRACE_INFO("SC_DecryptInit: rc = 0x%08lx, sess = %ld, mech = 0x%lx\n",
               rc, (sess == NULL) ? -1 : (CK_LONG) sess->handle,
               (pMechanism ? pMechanism->mechanism : (CK_ULONG)-1));

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    return rc;
}

CK_RV mxltok_decrypt(SESSION *sess, CK_BYTE_PTR pEncryptedData,
                 CK_ULONG ulEncryptedDataLen, CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen)
{
    CK_RV rc = CKR_OK;
    int ret;
    struct seccrypto_asym_enc_dec rsadec = {0};
    struct seccrypto_load_key loadkey = {0};
    TEEC_Operation op ={0};
    char key_name[MXL_KEYLENGTH_MAX] = {0};
    CK_MECHANISM_TYPE mechanism =  sess->decr_ctx.mech.mechanism & 0xFFFF;
    int sign_algo = (sess->decr_ctx.mech.mechanism >> 16) & 0xFFFF;
    uint32_t key_id =  sess->decr_ctx.key;
    loadkey.load_algo = sign_algo;
    loadkey.hash_algo = RSA_PKCS1_5_SHA1;
    switch (mechanism) {
        case CKM_RSA_PKCS:
            rsadec.hash_alg = ASYM_ENC_HASH_SHA1;
            rsadec.encod_schm = RSA_ENCOD_PKCS1_V1_5;
            break;
        case CKM_RSA_X_509:
            rsadec.hash_alg = ASYM_ENC_HASH_SHA1;
            rsadec.encod_schm =  RSA_ENCOD_OAEP;
            break;
        case CKM_RSA_PKCS_OAEP:
            rsadec.hash_alg = ASYM_ENC_HASH_SHA1;
            rsadec.encod_schm =  RSA_ENCOD_OAEP;
            if (sess->digest_ctx.mech.mechanism){
                switch (sess->digest_ctx.mech.mechanism)
	        {
                    case CKM_SHA_1:
                        rsadec.hash_alg = ASYM_ENC_HASH_SHA1;
                        break;
		    case CKM_SHA224:
                        rsadec.hash_alg = ASYM_ENC_HASH_SHA224;
                        break;
                    case CKM_SHA256:
                        rsadec.hash_alg = ASYM_ENC_HASH_SHA256;
                        break;
                    case CKM_SHA384:
                        rsadec.hash_alg = ASYM_ENC_HASH_SHA384;
                        break;
                    case CKM_SHA512:
                        rsadec.hash_alg = ASYM_ENC_HASH_SHA512;
                        break;
                    default:
                        break;
                }
            }
            break;
	default:
            TRACE_ERROR("mxltok_decrypt failed, wrong rsa algo:%ld\r\n",mechanism);
            return CKR_MECHANISM_INVALID;
            break;
    }

    mxl_securestore_create_keypair_object_name(key_name, key_id);
    loadkey.load_flags = PRIVATE_KEY_PKCS8;
    loadkey.key_type =  KEY_PRIVATE;

    if (mxl_init_scsa_session() != TEEC_SUCCESS) {
        TRACE_ERROR("mxl_init_scsa_session failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if (mxl_prepare_load_key(key_name, &loadkey, &op) < 0) {
        TRACE_ERROR("mxl_prepare_load_key failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    ret = mxl_execute_scsa_command(TA_SECURE_CRYPTO_LOAD_KEY, &op);
    if (ret != TEEC_SUCCESS) {
        TRACE_ERROR("TEE execute SCSA command TA_SECURE_CRYPTO_LOAD_KEY failed with error = %x \n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    if (mxl_prepare_rsa_decrypt(&rsadec, (char *)pEncryptedData, ulEncryptedDataLen, &op)){
        TRACE_ERROR("mxl_prepare_rsa_decrypt failed\n");
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    ret = mxl_execute_scsa_command(TA_SECURE_CRYPTO_ASYM_DECRYPT, &op);
    if (ret != TEEC_SUCCESS) {
        TRACE_ERROR("TEE execute SCSA command TA_SECURE_CRYPTO_ASYM_DECRYPT failed with error = %x \n", ret);
        rc = CKR_FUNCTION_FAILED;
        goto error;
    }

    memcpy(pData, rsadec.output_data, rsadec.output_data_len);
    *pulDataLen = rsadec.output_data_len;
error:
    if (rsadec.output_data)
        free(rsadec.output_data);
    if (loadkey.sst_params.handle)
        securestore_close(loadkey.sst_params.handle);
    mxl_exit_scsa_session();

    TRACE_INFO("%s:rc = 0x%08lx\n", __func__, rc);
    return rc;

}

CK_RV SC_Decrypt(STDLL_TokData_t *tokdata, ST_SESSION_HANDLE *sSession,
                 CK_BYTE_PTR pEncryptedData, CK_ULONG ulEncryptedDataLen, CK_BYTE_PTR pData,
                 CK_ULONG_PTR pulDataLen)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;
    CK_BBOOL locked = FALSE;

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }
    sess->handle = sSession->sessionh;

    if (!pEncryptedData || !pulDataLen) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        rc = CKR_ARGUMENTS_BAD;
        goto done;
    }

    if (sess->decr_ctx.active == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_OPERATION_NOT_INITIALIZED));
        rc = CKR_OPERATION_NOT_INITIALIZED;
        goto done;
    }

    rc = XProcLock(tokdata);
    if (rc != CKR_OK) {
        TRACE_ERROR("Failed to get process lock.\n");
        goto done;
    }
    locked = TRUE;

    rc = mxltok_decrypt(sess, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen);
    if (rc != CKR_OK) {
        TRACE_ERROR("mxltok_decrypt failed with error:%ld\r",rc);
    }

done:
    if (locked) {
        if (rc == CKR_OK) {
            rc = XProcUnLock(tokdata);
            if (rc != CKR_OK) {
                TRACE_ERROR("Failed to release Process Lock.\n");
            }
        } else {
            XProcUnLock(tokdata);
        }
    }
    sess->decr_ctx.active = FALSE;

    TRACE_INFO("SC_Decrypt: rc = 0x%08lx, sess = %ld, datalen = %ld\n",
               rc, (sess == NULL) ? -1 : (CK_LONG) sess->handle, ulEncryptedDataLen);

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    return rc;
}

CK_RV SC_FindObjectsInit(STDLL_TokData_t *tokdata,
                         ST_SESSION_HANDLE *sSession,
                         CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }

    if (pin_expired(&sess->session_info,
                    tokdata->nv_token_data->token_info.flags) == TRUE) {
        TRACE_ERROR("%s\n", ock_err(ERR_PIN_EXPIRED));
        rc = CKR_PIN_EXPIRED;
        goto done;
    }

    if (sess->find_active == TRUE) {
        TRACE_ERROR("%s\n", ock_err(ERR_OPERATION_ACTIVE));
        rc = CKR_OPERATION_ACTIVE;
        goto done;
    }

    rc = object_mgr_find_init(tokdata, sess, pTemplate, ulCount);

done:
    TRACE_INFO("SC_FindObjectsInit: rc = 0x%08lx\n", rc);

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

#ifdef DEBUG
    CK_ATTRIBUTE *attr = NULL;
    CK_BYTE val[4];
    CK_ULONG i;

    attr = pTemplate;
    for (i = 0; i < ulCount && attr != NULL; i++, attr++) {
        TRACE_DEBUG("%lu: Attribute type: 0x%08lx, Value Length: %lu\n",
                    i, attr->type, attr->ulValueLen);

        if (attr->ulValueLen >= sizeof(val) && attr->pValue != NULL) {
            memset(val, 0, sizeof(val));
            memcpy(val, attr->pValue, attr->ulValueLen > sizeof(val) ?
                                            sizeof(val) : attr->ulValueLen);
            TRACE_DEBUG("First 4 bytes: %02x %02x %02x %02x\n",
                        val[0], val[1], val[2], val[3]);
        }
    }
#endif

    return rc;
}
CK_RV SC_FindObjects(STDLL_TokData_t *tokdata, ST_SESSION_HANDLE *sSession,
                     CK_OBJECT_HANDLE_PTR phObject, CK_ULONG ulMaxObjectCount,
                     CK_ULONG_PTR pulObjectCount)
{
    SESSION *sess = NULL;
    CK_ULONG count = 0;
    CK_RV rc = CKR_OK;

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    if (!phObject || !pulObjectCount) {
        TRACE_ERROR("%s\n", ock_err(ERR_ARGUMENTS_BAD));
        rc = CKR_ARGUMENTS_BAD;
        goto done;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }

    if (sess->find_active == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_OPERATION_NOT_INITIALIZED));
        rc = CKR_OPERATION_NOT_INITIALIZED;
        goto done;
    }

    if (!sess->find_list) {
        TRACE_DEVEL("sess->find_list is NULL.\n");
        rc = CKR_OPERATION_NOT_INITIALIZED;
        goto done;
    }
    count = MIN(ulMaxObjectCount, (sess->find_count - sess->find_idx));

    memcpy(phObject, sess->find_list + sess->find_idx,
           count * sizeof(CK_OBJECT_HANDLE));
    *pulObjectCount = count;

    sess->find_idx += count;
    rc = CKR_OK;

done:
    TRACE_INFO("SC_FindObjects: rc = 0x%08lx, returned %lu objects\n",
               rc, count);

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    return rc;
}

CK_RV SC_FindObjectsFinal(STDLL_TokData_t *tokdata,
                          ST_SESSION_HANDLE *sSession)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }

    if (sess->find_active == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_OPERATION_NOT_INITIALIZED));
        rc = CKR_OPERATION_NOT_INITIALIZED;
        goto done;
    }

    if (sess->find_list)
        free(sess->find_list);

    sess->find_list = NULL;
    sess->find_len = 0;
    sess->find_idx = 0;
    sess->find_active = FALSE;

    rc = CKR_OK;

done:
    TRACE_INFO("SC_FindObjectsFinal: rc = 0x%08lx\n", rc);

    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    return rc;
}

CK_RV SC_CreateObject(STDLL_TokData_t *tokdata, ST_SESSION_HANDLE *sSession,
                      CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount,
                      CK_OBJECT_HANDLE_PTR phObject)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;

    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }
    //set the handle into the session.
    sess->handle = sSession->sessionh;

    if (pin_expired(&sess->session_info,
                    tokdata->nv_token_data->token_info.flags)) {
        TRACE_ERROR("%s\n", ock_err(ERR_PIN_EXPIRED));
        rc = CKR_PIN_EXPIRED;
        goto done;
    }

    /* Enforces policy */
    rc = object_mgr_add(tokdata, sess, pTemplate, ulCount, phObject);
    if (rc != CKR_OK)
        TRACE_DEVEL("object_mgr_add() failed.\n");

done:
    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    TRACE_INFO("C_CreateObject: rc = 0x%08lx\n", rc);

#ifdef DEBUG
    CK_ULONG i;

    for (i = 0; i < ulCount; i++) {
        if (pTemplate[i].type == CKA_CLASS &&
            pTemplate[i].ulValueLen == sizeof(CK_ULONG) &&
            pTemplate[i].pValue != NULL) {
            TRACE_DEBUG("Object Type:  0x%02lx\n",
                        *((CK_ULONG *) pTemplate[i].pValue));
        }
    }
    if (rc == CKR_OK)
        TRACE_DEBUG("Handle: %lu\n", *phObject);
#endif
    return rc;
}

CK_RV SC_DestroyObject(STDLL_TokData_t *tokdata, ST_SESSION_HANDLE *sSession,
                       CK_OBJECT_HANDLE hObject)
{
    SESSION *sess = NULL;
    CK_RV rc = CKR_OK;
    OBJECT_MAP *map = NULL;

    uint32_t key_id, pkcs8_key_id;
    CK_BBOOL mxl_key_flag;
    CK_ATTRIBUTE key_id_template[] = {
        {CKA_MXL_SECURE_STORAGE_HANDLE, &key_id, sizeof(key_id)},
    };
    CK_ATTRIBUTE public_key_id_template[] = {
        {CKA_MXL_KEY_TYPE, &mxl_key_flag, sizeof(CK_BBOOL)},
    };
    CK_ATTRIBUTE pkcs8_key_id_template[] = {
        {CKA_MXL_PKCS8_KEY_HANDLE, &pkcs8_key_id, sizeof(pkcs8_key_id)},
    };
    if (tokdata->initialized == FALSE) {
        TRACE_ERROR("%s\n", ock_err(ERR_CRYPTOKI_NOT_INITIALIZED));
        rc = CKR_CRYPTOKI_NOT_INITIALIZED;
        goto done;
    }

    sess = session_mgr_find_reset_error(tokdata, sSession->sessionh);
    if (!sess) {
        TRACE_ERROR("%s\n", ock_err(ERR_SESSION_HANDLE_INVALID));
        rc = CKR_SESSION_HANDLE_INVALID;
        goto done;
    }

    if (pin_expired(&sess->session_info,
                    tokdata->nv_token_data->token_info.flags) == TRUE) {
        TRACE_ERROR("%s\n", ock_err(ERR_PIN_EXPIRED));
        rc = CKR_PIN_EXPIRED;
        goto done;
    }
    map = bt_get_node_value(&tokdata->object_map_btree, hObject);
    if (!map) {
        TRACE_ERROR("%s handle: %lu\n", ock_err(ERR_OBJECT_HANDLE_INVALID),hObject);
        rc = CKR_OBJECT_HANDLE_INVALID;
        goto done;
    }
    rc = object_mgr_get_attribute_values(tokdata, sess, hObject, key_id_template, 1);
    if (rc != CKR_OK) {
        TRACE_ERROR("object_mgr_get_attribute_value() failed, rc =%ld.\n",rc);
        goto done;
    }
    if (map->is_private) {
        mxl_securestore_delete_keypair(key_id);
        rc = object_mgr_get_attribute_values(tokdata, sess, hObject, pkcs8_key_id_template, 1);
        if (rc == CKR_OK) {
            mxl_securestore_delete_keypair(pkcs8_key_id);
        }
    }
    else {
        rc = object_mgr_get_attribute_values(tokdata, sess, hObject, public_key_id_template, 1);
        if (rc != CKR_OK) {
            TRACE_ERROR("object_mgr_get_attribute_value() failed, rc =%ld.\n",rc);
        }
        else if (mxl_key_flag == TRUE) {
             mxl_securestore_delete_keypair(key_id);
        }
    }

    rc = object_mgr_destroy_object(tokdata, sess, hObject);
    if (rc != CKR_OK)
        TRACE_DEVEL("object_mgr_destroy_object() failed\n");

done:
    if (sess != NULL)
        session_mgr_put(tokdata, sess);

    TRACE_INFO("SC_DestroyObject: rc = 0x%08lx, handle = %lu\n", rc, hObject);

    return rc;
}

void SC_SetFunctionList(void)
{
    function_list.ST_Initialize = ST_Initialize;
    function_list.ST_InitToken = SC_InitToken;
    function_list.ST_GetTokenInfo = SC_GetTokenInfo;
    function_list.ST_OpenSession = SC_OpenSession;
    function_list.ST_CloseSession = SC_CloseSession;
    function_list.ST_GetSessionInfo = SC_GetSessionInfo;
    function_list.ST_Login = SC_Login;
    function_list.ST_Logout = SC_Logout;
    function_list.ST_InitPIN = SC_InitPIN;
    function_list.ST_SetPIN = SC_SetPIN;
    function_list.ST_GetMechanismInfo = SC_GetMechanismInfo;
    function_list.ST_GetMechanismList = SC_GetMechanismList;
    function_list.ST_GenerateKeyPair = SC_GenerateKeyPair;
    function_list.ST_GetAttributeValue = SC_GetAttributeValue;
    function_list.ST_SetAttributeValue = SC_SetAttributeValue;
    function_list.ST_SignInit = SC_SignInit;
    function_list.ST_Sign = SC_Sign;
    function_list.ST_VerifyInit = SC_VerifyInit;
    function_list.ST_Verify = SC_Verify;
    function_list.ST_DecryptInit = SC_DecryptInit;
    function_list.ST_Decrypt = SC_Decrypt;
    function_list.ST_FindObjectsInit = SC_FindObjectsInit;
    function_list.ST_FindObjects = SC_FindObjects;
    function_list.ST_FindObjectsFinal = SC_FindObjectsFinal;
    function_list.ST_CreateObject = SC_CreateObject;
    function_list.ST_DestroyObject = SC_DestroyObject;
}
