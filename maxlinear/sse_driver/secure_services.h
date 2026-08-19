/******************************************************************************

  Copyright (C) 2020-2022 MaxLinear, Inc.
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : secure_services.h                                          *
 *     Project    : UGW                                                        *
 *     Description: secure fapi header file inclusions.                               *
 *                                                                             *
 ******************************************************************************/

#ifndef _SECURE_SERVICES_H
#define _SECURE_SERVICES_H


/*!
    \brief This macro denotes maximum file line length.
*/
#define MAX_FILELINE_LEN       332

/*!
    \brief This macro defines alignment requirement for secure upgrade.
*/
#define ALIGN_SIZE (4*1024)

/*!
    \brief This macro defines allocation units size for secure upgrade.
*/
#define MIN_ALLOC_SIZE (512*1024)
#define NEXT_CHUNK_SIZE (3000*1024)

/*!
    \brief This macro defines secure image header size.
*/
//#define SBIF_HEADER 264
#define SBIF_HEADER 0x248

/*!
    \brief This macro defines alignment requirement for secure header.
*/
#define SBIF_ALIGN  64

#define MAX_SIZE  (4000 * 1024)
#define block_length 16
#define MIN_SIZE (8 * 1024)
#define SS_DEV_NAME "secure_service_driver"
#define SS_PATH "/dev/secure_service_driver"
#define SEC_UPG_PATH "/dev/secupg"
#define SEC_STG_PATH "/dev/secstg"
#define SEC_SIGN_PATH "/dev/secsign"
#define SEC_STORE_ADMIN "/dev/sec_store_admin"
#define SEC_STORE_NORMAL "/dev/sec_store_normal"
#define SEC_DEBUG_PATH "/dev/secdbg"

#define MAX_FILE_NAME 256
#define MAX_USER_NAME 16
#define FILEPNAME_MAX 128
#define OBJ_ID_SIZE FILEPNAME_MAX

#define SSS_ICC_REQ_TIMEOUT				500

typedef unsigned long sshandle_t;
#ifndef __KERNEL__
typedef unsigned char sst_flags_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
#else
typedef uint8_t sst_flags_t;
#endif

/* SST ICC Reply Message Code */
enum sst_response_code {
	SST_SUCCESSFUL = 0,				/* Successful */
	SST_OBJ_NOT_FOUND_ERR,			/* Object not found */
	SST_OBJ_POLICY_NOT_FOUND_ERR,	/* Policy not found */
	SST_POLICY_NOT_MATCHED_ERR,		/* Policy not matched */
	SST_SSHANDLE_NOT_MATCHED_ERR,	/* SShandle not matched */
	SST_OBJ_ATR_NOT_MATCHED_ERR,	/* Object attr not matched */
	SST_OBJ_ID_GENERATION_ERR,		/* Object ID geneation error */
	SST_OBJ_NODE_MEM_ALLOC_ERR,		/* Node momory allocation error */
	SST_ICC_POOL_ALLOC_ERR,			/* ICC mempool alloc error */
	SST_OBJ_ALREADY_EXIST_ERR,		/* Object already exist error */
	SST_OBJ_CREATE_REQ_ERR,			/* Create request error */
	SST_OBJ_OPEN_REQ_ERR,			/* Open request error */
	SST_OBJ_SAVE_REQ_ERR,			/* Save request error */
	SST_OBJ_LOAD_REQ_ERR,			/* Load request error */
	SST_OBJ_DELETE_REQ_ERR,			/* Delete requesr error */
};

/*Secure Debug Reply Message Code */
enum secdbg_response_code  {
	SDBG_SUCCESSFUL = 0,				/* Successful */
	SDBG_AUTH_INIT_ERR,					/* Debug Port Authentication Init Error */
	SDBG_AUTH_VERIFY_ERR,				/* Debug Port Authentication Verify Error */
	SDBG_INVALID_AUTHINIT_REQUEST, 		/* Debug port Auth Init request is invalid */
	SDBG_INVALID_AUTHVERIFY_REQUEST,	/* Debug port Auth verify request is invalid */
	SDBG_PORT_ALREADY_UNLOCKED,			/* Debug Port is already unlocked */
};

/*! 
 *     \brief enum for the secure store access permissions
 */
typedef enum {
	SS_OBJ_PERM_NO_ACCESS	=	0x0,
	SS_OBJ_PERM_READ		=	0x1,
	SS_OBJ_PERM_WRITE		=	0x2,
	SS_OBJ_PERM_READ_WRITE	=	0x3,
} ss_obj_perm_t;

/*! 
 *     \brief enum for the secure store crypto mode flags
 */
typedef enum {
    SS_CI = 0x01,               /* confidentiality and integrity */
    SS_I = 0x02,                /* Integrity protection only */
    SS_RP = 0x08,               /* Anti-replay protection */
    SS_OVWINV = 0x40,           /* overwrite is the object is invalid */
    SS_OVW = 0x80               /* Modify/Overwrite the object */
} sst_crypto_mode_flag_t;

typedef enum {
    SS_CREATE = 0x10,           /* Create the object if does not exist */
    SS_DELETE = 0x20,           /* Delete the object if it exist */
} sst_obj_ops_flag_t;

typedef enum messages_ {
    SS_KEY_GEN = 100,           /* Key Generator */
    SS_KEY_PURGE,               /* For removing the key */
    SS_KEY_DERIVE,              /* To derive the key */
    SS_SEC_ENCRYPT,             /* Encryption */
    SS_SEC_DECRYPT,             /* Decryption */
    SS_SEC_IMG_AUTH             /* Image authentication */
} messages;

enum icc_ia_request {
    ICC_CMD_IA_EXEC = 0,
    ICC_CMD_IA_READ
};

enum icc_secdbg_request {
	SECURE_DEBUG_AUTH_START = 0,
	SECURE_DEBUG_AUTH_VERIFY,
	SECURE_DEBUG_AUTH_STOP,
	SECURE_DEBUG_AUTH_START_RES = 0x80,
	SECURE_DEBUG_AUTH_VERIFY_RES,
	SECURE_DEBUG_AUTH_STOP_RES,
};

enum key_location {
	NO_WRAP_KEY = 0b00,
	KEY_IN_OTP = 0b01,            /* OTP */
	KEY_IN_SST = 0b10             /* Secure Storage */
};

/*! 
 *     \brief Contains the file parameter list
 */
typedef struct file_param_ {
    unsigned char *address;
    int size;
} file_param_t;

typedef struct img_param_ {
    int src_img_fd;
    unsigned char *src_img_addr;
    size_t src_img_len;
    unsigned char write_to_upg_part;
    unsigned char img_type;
	uint8_t commit;
	uint8_t chkFARB;
	uint8_t chkARB;
    char img_name[MAX_FILE_NAME];
} img_param_t;

typedef struct {
    union {
        struct {
            uint8_t user:2;		/* !< UID access permissions */
            uint8_t group:2;	/* !< GID access permissions */
            uint8_t others:2;	/* !< Others access permissions */
            uint8_t pname:2;
        } field;
        uint8_t perms;
    } u;
} sst_access_perm_t;

typedef struct {
	union {
		struct {
			uint16_t lock:1;			/* !< set lock bit secure store. secure store
								   will no longer modify this file. */
			uint16_t no_load_to_userspace:1;	/* !< SSC holds onto object and allows
								   application in userspace to use it
								   in signing/encryption operations by
								   handle */
			uint16_t read_once:1;		/* !< Read the object once per boot */
			uint16_t ignore_uid:1;		/* !< Ignore UID in Policy */
			uint16_t ignore_gid:1;		/* !< Ignore GID in Policy Enforcement */
			uint16_t ignore_pname:1;		/* !< Ignore pname in policy Enforcement */
			uint16_t wrap_flag:2;		/* Wrapped flag */
			uint16_t admin_store:1;		/* Admin/Normal store access */
			uint16_t tee_only:1;		/* TEE only access mode */
			uint16_t reserve:6;			/* Reserved for furture use */
		} field;
		uint16_t attr;
	} u;
} sst_policy_attr_t;

/**
 * struct sst_wrap_params - represents secure storage information of the wrap key
 * @handle  : 64-bit secure storage handle of opened sst wrap key object
 * @access_perm : access permissions associated with sst wrap key object
 * @policy_attr : policy attributes for enforcing policy check
 * @crypto_mode_flag  : crypto mode options like integrity, encryption etc...
 */
struct sst_wrap_params {
	uint64_t handle;
	uint8_t access_perm;
	uint16_t policy_attr;
	uint16_t crypto_mode_flag;
};

typedef struct {
	enum key_location key_location;
	union {/* secure storage object wrap params or OTP asset ID */
		struct sst_wrap_params sst_wrap;
		unsigned int asset_number;
	} u;
} secure_wrap_asset_t;

typedef struct {
	enum key_location key_location;
	union {/* secure storage object wrap param pointer or OTP asset ID */
		unsigned int wrap_handle;
		unsigned int asset_number;
	} u;
} secure_wrap_asset_tep_t;

/*! 
 *     \brief Contains the secure storage access policy parameters
 */
typedef struct {
	sst_access_perm_t  access_perm;				/* Object access permission*/
	sst_policy_attr_t policy_attr;				/* Object access attributs */
	sst_crypto_mode_flag_t crypto_mode_flag;	/* Object crypto flag */
	secure_wrap_asset_t wrap_asset;				/* Wrap Object configuration */
} sst_obj_config_t;


typedef struct {
	sshandle_t ss_handle;       		/* 16 byte Handle to save/retrieve the object */
	int sobject_len;					/* length of the object */
	const char *objectname;				/* Object name to be opened/created */
	sst_obj_config_t sst_access_policy; /* Object policy configuration */
	sst_flags_t secure_store_flags;		/* Object crypto flag */
} sst_param_t;

typedef struct {
	sshandle_t ss_handle;
	secure_wrap_asset_t wrap_asset;	/* Wrap Object configuration */
	size_t payload_len;
	const unsigned char *payload;
} sst_data_param_t;

typedef struct {
	unsigned int auth_key_addr;
	unsigned int auth_key_len;
} auth_key_info_t;

typedef struct {
	unsigned int p_signature_addr;
	unsigned int sign_sz;
} signature_info_t;

typedef struct {
	unsigned char port_num;
	unsigned char *nonce;
	unsigned int nonce_len;
	const unsigned char *auth_key;
	unsigned int auth_key_len;
} sec_dbg_auth_start_t;

typedef struct {
	unsigned char port_num;
	const unsigned char *p_signature;
	unsigned int sign_sz;
	const unsigned char *random_nonce;
	unsigned int rd_nonce_len;
} sec_dbg_auth_unlock_t;

typedef struct {
	unsigned int nonce;
	unsigned int nonce_len;
	union {
		auth_key_info_t auth_info;
		signature_info_t sig_info;
	} u;
} sec_dbg_config_t;

typedef enum {
	SS_ICC_PARAM_0 = 0,
	SS_ICC_PARAM_1,
	SS_ICC_PARAM_2,
	SS_ICC_PARAM_3
} ss_icc_params_idx_t;

/******************************************************************************
 * Exported functions
 ******************************************************************************/
 /** magic number */
#define SS_MAGIC 'H'
#define SS_IOC_SEC_DECRYPT _IOW(SS_MAGIC, 1, file_param_t)
#define SS_IOC_SEC_ENCRYPT _IOW(SS_MAGIC, 2, file_param_t)
#define SS_IOC_SEC_IMGAUTH _IOW(SS_MAGIC, 3, img_param_t)
#define SS_STG_CREATE_OPEN _IOWR(SS_MAGIC, 4, sst_param_t)
#define SS_STG_SAVE _IOW(SS_MAGIC, 5, sst_data_param_t)
#define SS_STG_RESTORE _IOW(SS_MAGIC, 6, sst_data_param_t)
#define SS_STG_DELETE_CLOSE _IOW(SS_MAGIC, 7, sst_data_param_t)
#define SSC_SECURE_DEBUG_AUTH_START _IOWR(SS_MAGIC, 8, sec_dbg_auth_start_t)
#define SSC_SECURE_DEBUG_AUTH_VERIFY _IOWR(SS_MAGIC, 9, sec_dbg_auth_unlock_t)

#ifdef __KERNEL__

void __exit sse_img_auth_cleanup(void);
int __init sse_img_auth_init(void);
long sse_img_auth_ioctl(struct file *fd, unsigned int cmd, unsigned long arg);
unsigned int sse_img_auth_poll(struct file *fp, poll_table * wait);

long sse_sec_storage_client_ioctl(struct file *fd,
							unsigned int cmd, unsigned long arg);
int __init sse_sec_storage_client_init(void);
void __exit sse_sec_storage_client_cleanup(void);
int __init secure_debug_client_init(void);
void __exit secure_debug_client_cleanup(void);
long sse_sec_dbg_client_ioctl(struct file *fd,
							unsigned int cmd, unsigned long arg);
void dump_ss_icc_msg(icc_msg_t *dump_msg);
#endif

#endif                          /* _SECURE_SERVICES_H */
