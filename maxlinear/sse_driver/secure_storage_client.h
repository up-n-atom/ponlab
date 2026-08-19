/******************************************************************************

  Copyright (C) 2022 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : secure_storage_client.h                                          *
 *     Project    : UGW                                                        *
 *     Description: secure storage header file inclusions.                               *
 *                                                                             *
 ******************************************************************************/

#ifndef _SECURE_STORAGE_H
#define _SECURE_STORAGE_H

#include "secure_services.h"

enum icc_sst_request {
    SS_SST_CREATE_OPEN = 0,
    SS_SST_SAVE,
    SS_SST_RETRIEVE,
    SS_SST_DELETE_CLOSE,
    SS_SST_CREATE_OPEN_RES = 0x80,
    SS_SST_SAVE_RES,
    SS_SST_RETRIEVE_RES,
    SS_SST_DELETE_CLOSE_RES
};

typedef struct {
    uid_t uid_val;              /* !< UID of the calling process */
    gid_t gid_val;              /* !< GID of the calling process */
	unsigned char pname[MAX_USER_NAME]; /* Process name */
	sst_access_perm_t  access_perm;
	sst_policy_attr_t policy_attr;
} sst_access_policy_t;

/*!
 *     \brief Contains the secure storage config parameters
 */
typedef struct {
    uid_t uid_val;					/* UID of the calling process */
    gid_t gid_val;					/* GID of the calling process */
    unsigned int pdata_buf;			/* pointer to Data Object */
    unsigned int data_len;			/* Data length */
    char pname[MAX_USER_NAME];		/* Process name */
    char obj_name[FILEPNAME_MAX];	/* Unique Identifier of Object Data */
    sst_access_policy_t policy;		/* secure store access policy */
    sst_flags_t flags;				/* Secure Store flags */
    secure_wrap_asset_tep_t wrap_asset;
} sst_config_t;

int sse_secure_storage_create_open(sst_param_t *sst_param,
								sst_config_t *secure_store_config);
int sse_secure_storage_save(sst_data_param_t *sst_save_param,
								sst_config_t *secure_store_config);
int sse_secure_storage_load(sst_data_param_t *sst_save_param,
								sst_config_t *secure_store_config);
int sse_secure_storage_load(sst_data_param_t *sst_save_param,
								sst_config_t *secure_store_config);
int sse_secure_storage_close_delete(sst_param_t *sst_del_param,
								sst_config_t *secure_store_config);
int sse_secure_storage_send_icc_msg(icc_msg_t *ssc_client_msg,
								icc_msg_t *ssc_reply_msg,
								sst_config_t *secure_store_config);
#endif
