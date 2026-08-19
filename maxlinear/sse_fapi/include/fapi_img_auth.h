/******************************************************************************

  Copyright (C) 2022 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : fapi_img_auth.h                                            *
 *     Project    : UGW                                                        *
 *     Description: secure fapi APIs for image authentication                  *
 *                                                                             *
 ******************************************************************************/

/*! \file fapi_img_auth.h
 \brief File contains image authentication APIs prototype
*/

#ifndef _IMG_AUTH_FAPI__
#define _IMG_AUTH_FAPI__

#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>

#define MAX_SS_BUF_SIZE  16
#define MAX_WRITE 2
#define TEMP_BUF                4096
#define UPG_STATE_FILE VENDOR_PATH "/etc/upgrade/.upg_state_file"
#define FLASH_CNT_CHK VENDOR_PATH "/etc/upgrade/.flash_cnt_chk"

#define YR_MAGIC 1900
#define MM_MAGIC 1

#define F_MAX_FLASH_CNT "max_flash_cnt="
#define F_FLASH_CNT "cur_flash_cnt="
#define F_LAST_UPDATE_TIME_STAMP "l_updated_ts="
#define F_DAY_CNT	"day_flash_cnt="

/*calculate from flash driver instead hardcode */
#define MAX_FLASH_CNT_SUPPORT MAX_FLASH_CNT
#define MAX_DAY_FLASH_CNT 5

/* ROOTFS 16 MB chunk + 0x248 header */
#define ROOTFS_CHUNK_SIZE 0x1000000
#define ROOTFS_CHUNK_ALIGN 0x1100000

#define UBOOT_NAME_NULL 1
#define UBOOT_SET_OPERATION_FAIL 2
#define UBOOT_VALUE_ERROR 3
#define UBOOT_VARIABLE_NOT_EXIST 4

#define GET_DEVICE_INFO_DATA_FAILURE -1


#define IMAGE_VALIDATION_FAILED 3
#define IMAGE_WRITE_FAILED 4
#define IMAGE_UPGSTATE_ERROR 5

#ifdef LGM_SECURE
#include <secure_services.h>
#else
/* original definitions are available in sse_driver_lgm */
#define MAX_FILE_NAME 256
#define SEC_UPG_PATH "/dev/secupg"
#define SS_MAGIC 'H'
#define SS_IOC_SEC_IMGAUTH _IOW(SS_MAGIC, 3, img_param_t)

typedef struct img_param_ {
    int src_img_fd;
    unsigned char *src_img_addr;
    size_t src_img_len;
    unsigned char write_to_upg_part;
    unsigned char img_type;
    char img_name[MAX_FILE_NAME];
#ifdef IMG_AUTH
	uint8_t commit;
	uint8_t chkFARB;
	uint8_t chkARB;
#endif
} img_param_t;
#endif

typedef enum State_t {
	UPG_NONE,
	UPG_REQ,
	UPG_INPROG,
	UPG_SUCC,
	UPG_FAIL,
	UPG_COMMIT_REQ,
	UPG_COMMIT_SUCC,
	UPG_COMMIT_FAIL,
	UPG_RES, /* reserved for future use */
	MAX_STATE_CNT,
} State_t;

typedef enum udt_State_t {
	IMAGE_STATE_NONE,
	IMAGE_UPG_INITIATED,
	IMAGE_AUTH_FAIL,
	IMAGE_UPG_FAIL,
	IMAGE_HEALTH_CHECK,
	IMAGE_COMMIT
} udt_state_t;

typedef enum type_t {
	INTEGER,
	STRING
} type_t;

typedef struct uboot_value{
	type_t type;
	union {
		int valuei;
		char *valuec;
	}u;
} uboot_value_t;

/*! 
        \brief API to authenticate image
        \param[in] image_auth image_auth_t structure
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssImgAuth(img_param_t image_auth);

/*! 
        \brief API to authenticate image
        \param[in] image type
        \param[in] image length
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssImgValidateAndCommit(char *image_type, int len);
/*! 
        \brief API to Upgrade image to partition
        \param[in] image_auth image_auth_t structure
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssImgUpgrade(img_param_t image_auth);
/*! 
        \brief API to get upgrade state 
        \param[in] ppcState
        \return  current state on successful / UGW_FAILURE on failure
*/
int fapi_ssGetUpgState(unsigned char **ppcState);

/*! 
        \brief API to set upgrade state
        \param[in] pcState 
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssSetUpgState(unsigned char *pcState);

/*! 
        \brief API to get Active bank 
        \param[in] ppcState 
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssGetActiveBank(unsigned char **ppcstate);

/*! 
        \brief API to Activate bank 
        \param[in] pvalue 
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssActivateBank(unsigned char *pvalue);

/*! 
        \brief API to get commit bank 
        \param[in] ppcState 
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssGetCommitBank(unsigned char **ppcState);

/*! 
        \brief API to set commit bank 
        \param[in] pvalue 
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssSetCommitBank(unsigned char *pvalue);

/*! 
        \brief API to set uboot param 
        \param[in] name 
        \param[in] pvalue 
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssSetUbootParam(char *pname, uboot_value_t *pvalue);

/*! 
        \brief API to set udt param 
        \param[in] pvalue 
        \param[in] ivalue 
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssSetUdt(unsigned char *pvalue,uint32_t ivalue);

/*! 
        \brief API to do image authentication 
        \param[in] path 
        \return 0 on successful / error code on failure
*/
int fapi_Image_Verify(const char *path);

/*! 
        \brief API to do ext4 image authentication 
        \param[in] path 
        \return 0 on successful / error code on failure
*/
int fapi_Ext4_Image_Verify(const char *path);

/*! 
        \brief API to do image upgrade from linux 
        \param[in] path 
        \return 0 on successful / error code on failure
*/
int fapi_Image_upgrade(const char *path);

/*! 
        \brief API to perform commit 
        \param[in] 
        \return 0 on successful / error code on failure
*/
int fapi_Image_commit(void);

/*! 
        \brief API to get last upgrade status 
        \param[out] value  
        \return 0 on successful / error code on failure
*/
int fapi_Get_lastupg_status(char *value);

/*! 
        \brief API to perform commit 
        \param[out] value 
        \return 0 on successful / error code on failure
*/
int fapi_Get_lastupg_time(char *value);

/*!
        \brief API to switch bank
        \param[in] actbnk
        \return 0 on successful / error code on failure
*/
int fapi_Switch_bank(char *actbnk);
#endif

/* @} */
