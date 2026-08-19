/*******************************************************************************

  Copyright © 2020-2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*  *****************************************************************************
 *         File Name    : sec_storage_ctest.c                                   *
 *         Description  : test cases of test application to test secure service *
                          APIs                                                  *
 *  *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "ctest.h"
#include "fapi_sec_service.h"
#include "fapi_sec_storage.h"
#include <secure_services.h>

#include "safe_str_lib.h"
#include "safe_lib.h"
#include "safe_mem_lib.h"

#define HEADER_SIZE     276
#define DATA_FILE       "/opt/intel/csd/etc/datacfg"
#define ATOM_MSG        "Hello, I'm Atom"
#define MOD_MSG         "Content is modified"
#define MAX_DATA_BUF    (1024 * 1024)

void display_object_config(sst_obj_config_t *pxSstConfig);
void set_object_config(sst_obj_config_t *pxSstConfig, uint32_t unObjectConfig);
char p_name[MAX_USER_NAME];

/*
 * @brief  Display the sst_obj_config_t values
 * @param
 *  pxSstConfig config structure
 * @return
 *  void
 */
void display_object_config(sst_obj_config_t *pxSstConfig)
{
    fprintf(stdout, "user permission: 0x%x\n", pxSstConfig->access_perm.u.field.user);
    fprintf(stdout, "group permission: 0x%x\n", pxSstConfig->access_perm.u.field.group);
    fprintf(stdout, "other permission: 0x%x\n", pxSstConfig->access_perm.u.field.others);
    fprintf(stdout, "pname permission: 0x%x\n", pxSstConfig->access_perm.u.field.pname);
    fprintf(stdout, "lock bit: 0x%x\n", pxSstConfig->policy_attr.u.field.lock);
    fprintf(stdout, "load_to_userspace: 0x%x\n", pxSstConfig->policy_attr.u.field.no_load_to_userspace);
    fprintf(stdout, "read once bit: 0x%x\n", pxSstConfig->policy_attr.u.field.read_once);
    fprintf(stdout, "ignore uid bit: 0x%x\n", pxSstConfig->policy_attr.u.field.ignore_uid);
    fprintf(stdout, "ignore gid bit: 0x%x\n", pxSstConfig->policy_attr.u.field.ignore_gid);
    fprintf(stdout, "ignore pname bit: 0x%x\n", pxSstConfig->policy_attr.u.field.ignore_pname);
    fprintf(stdout, "crypto mode flag: 0x%x\n", pxSstConfig->crypto_mode_flag);
    fprintf(stdout, "admin node flag: 0x%x\n", pxSstConfig->policy_attr.u.field.admin_store);
    fprintf(stdout, "wrap flag: 0x%x\n", pxSstConfig->policy_attr.u.field.wrap_flag);
    fprintf(stdout, "wrap key location: 0x%x\n", pxSstConfig->wrap_asset.key_location);
    if (pxSstConfig->wrap_asset.key_location & KEY_IN_OTP) {
        fprintf(stdout, "wrap key asset ID: 0x%x\n", pxSstConfig->wrap_asset.u.asset_number);
    }
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

    /* Crypto mode flag */
    pxSstConfig->crypto_mode_flag = unObjectConfig & 0xFF;
	unObjectConfig = unObjectConfig >> 8;

	/* Admin/Normal node access */
	pxSstConfig->policy_attr.u.field.admin_store = unObjectConfig & 1;
}

/* Create/Save/Load object with invalid wrap asset number */
CTEST(suite11, wrap_flag_01_test1) {
    sst_obj_config_t xSstConfig = {0};
    secure_wrap_asset_t xWrapConfig = {0};
    sshandle_t xSsHandle = 0;
    /* below series of hex values indicates the wrapped data of "Hello, I'm Atom1234" */
    unsigned int cBuf[] = {0x652241e6, 0x4c5aa580, 0x96e34630, 0x24cef880, 0xb587a507, 0x74c29952, 0x461eaf00, 0x13b1667e};
    unsigned char cLoadBuf[32] = {0};
    uint32_t nActLen = 0;
    int nRet = -1;
    unsigned int *ppcLoadData = NULL;

    set_object_config(&xSstConfig, 0x0140CF);
    xSstConfig.wrap_asset.key_location = KEY_IN_OTP; /* 0b01 - OTP */
    xSstConfig.wrap_asset.u.asset_number = OTP_CUST_ASSET_12; /* valid range: 11-30 */
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("wrap_otp", &xSstConfig, SS_CREATE, &xSsHandle);
    if (nRet == -SST_OBJ_ALREADY_EXIST_ERROR) {
        nRet = securestore_create_open("wrap_otp", &xSstConfig, 0, &xSsHandle);
    }
    ASSERT_EQUAL(0, nRet);

    xWrapConfig.key_location = KEY_IN_OTP; /* 0b01 - OTP */
    xWrapConfig.u.asset_number = OTP_CUST_ASSET_12; /* valid range: 11-30 */

    nRet = securestore_save(xSsHandle, &xWrapConfig, (unsigned char *)cBuf, (unsigned int)sizeof(cBuf));
    ASSERT_EQUAL(0, nRet);

    securestore_retrieve(xSsHandle, &xWrapConfig, cLoadBuf, (unsigned int)sizeof(cLoadBuf), &nActLen);

    ppcLoadData = (unsigned int *)cLoadBuf;
    for (unsigned int i = 0; i < (nActLen/sizeof(int)); i++) {
        fprintf(stdout, "%x\n", *ppcLoadData);
        ppcLoadData++;
    }

    nRet = securestore_delete(xSsHandle);
    ASSERT_EQUAL(0, nRet);
}

/* Create/Open/Save/Load object with invalid wrap asset number */
CTEST(suite10, invalid_asset_number_test1) {
    sst_obj_config_t xSstConfig = {0};
    secure_wrap_asset_t xWrapConfig = {0};
    sshandle_t xSsHandle = 0;
    unsigned char cBuf[] = ATOM_MSG;
    unsigned char cLoadBuf[32] = {0};
    uint32_t nActLen = 0;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0180CF);
    xSstConfig.wrap_asset.key_location = KEY_IN_OTP; /* 0b01 - OTP */
    xSstConfig.wrap_asset.u.asset_number = 1; /* valid range: 11-30 */
    display_object_config(&xSstConfig);

    /* invalid asset number to create FAPI */
    nRet = securestore_create_open("wrap_otp", &xSstConfig, SS_CREATE, &xSsHandle);
    if (nRet == -SST_INVALID_ASSET_ID_ERROR)
        fprintf(stdout, "[Create] Invalid asset number(%d) blocked\n", xSstConfig.wrap_asset.u.asset_number);
    ASSERT_NOT_EQUAL(0, nRet);

    /* invalid asset number to open FAPI */
    nRet = securestore_create_open("wrap_otp", &xSstConfig, 0, &xSsHandle);
    if (nRet == -SST_INVALID_ASSET_ID_ERROR)
        fprintf(stdout, "[Open] Invalid asset number(%d) blocked\n", xSstConfig.wrap_asset.u.asset_number);
    ASSERT_NOT_EQUAL(0, nRet);

    memset_s(&xSstConfig, sizeof(xSstConfig), 0x0, sizeof(xSstConfig));
    set_object_config(&xSstConfig, 0x0180CF);
    xSstConfig.wrap_asset.key_location = KEY_IN_OTP; /* 0b01 - OTP */
    xSstConfig.wrap_asset.u.asset_number = OTP_CUST_ASSET_12; /* valid range: 11-30 */
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("wrap_otp", &xSstConfig, SS_CREATE, &xSsHandle);
    if (nRet == -SST_OBJ_ALREADY_EXIST_ERROR) {
        nRet = securestore_create_open("wrap_otp", &xSstConfig, 0, &xSsHandle);
    }
    ASSERT_EQUAL(0, nRet);

    xWrapConfig.key_location = KEY_IN_OTP; /* 0b01 - OTP */
    xWrapConfig.u.asset_number = 31; /* valid range: 11-30 */

    /* invalid asset number to save FAPI */
    nRet = securestore_save(xSsHandle, &xWrapConfig, cBuf, (unsigned int)sizeof(cBuf));
    if (nRet == -SST_INVALID_ASSET_ID_ERROR)
        fprintf(stdout, "[Save] Invalid asset number(%d) blocked\n", xWrapConfig.u.asset_number);
    ASSERT_NOT_EQUAL(0, nRet);

    /* invalid asset number to load FAPI */
    nRet = securestore_retrieve(xSsHandle, &xWrapConfig, cLoadBuf, (unsigned int)sizeof(cLoadBuf), &nActLen);
    if (nRet == -SST_INVALID_ASSET_ID_ERROR)
        fprintf(stdout, "[Load] Invalid asset number(%d) blocked\n", xWrapConfig.u.asset_number);
    ASSERT_NOT_EQUAL(0, nRet);

    nRet = securestore_delete(xSsHandle);
    ASSERT_EQUAL(0, nRet);
}

/* Max capacity test for data saving */
CTEST(suite9, 1MB_test) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    unsigned char *pc1MbBuf = NULL;
    int nCount = 0;
    int nFd = -1;
    int nRet = -1;

    pc1MbBuf = (unsigned char*) malloc(MAX_DATA_BUF);
    if (!pc1MbBuf) {
        fprintf(stdout, "no memory available\n");
        ASSERT_TRUE(false);
    }
    ASSERT_TRUE(pc1MbBuf != NULL);

read_cont:
    nFd = open(DATA_FILE, O_RDONLY);
    if (nFd < 0) {
        fprintf(stdout, "%s open failed\n", DATA_FILE);
        free(pc1MbBuf);
    }
    ASSERT_TRUE(nFd >= 0);

    while ((nCount < MAX_DATA_BUF) && ((nRet = read(nFd, (pc1MbBuf + nCount), sizeof(char)))) > 0) {
        nCount ++;
    }
    if (nRet < 0) {
        fprintf(stdout, "failed to read\n");
        free(pc1MbBuf);
    }
    close(nFd);
    if (nCount < MAX_DATA_BUF) {
        goto read_cont;
    }
    ASSERT_EQUAL(1, nRet);

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("1mb-data.txt", &xSstConfig, SS_CREATE,
            &xSsHandle);
    if (nRet < 0) {
        fprintf(stdout, "failed to create '1mb-data.txt' object\n");
        free(pc1MbBuf);
    }
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_save(xSsHandle, NULL, pc1MbBuf, nCount);
    if (nRet < 0) {
        fprintf(stdout, "failed to save the data\n");
        free(pc1MbBuf);
    }
    fprintf(stdout, "saved data size:%d\n", nRet);
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_delete(xSsHandle);
    if (nRet < 0) {
        fprintf(stdout, "failed to delete the object\n");
        free(pc1MbBuf);
    }
    ASSERT_EQUAL(0, nRet);
    free(pc1MbBuf);
}

CTEST(suite8, uid_policy_test2) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandleNew = 0;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0110ff);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("tst_normal_uid", &xSstConfig, SS_CREATE,
            &xSsHandleNew);
    if (nRet == -SST_OBJ_ALREADY_EXIST_ERROR) {
        nRet = securestore_create_open("tst_normal_uid", &xSstConfig,
                0, &xSsHandleNew);
    }
    fprintf(stdout, "###ss handle = %ld nRet:%d\n", xSsHandleNew, nRet);
}

CTEST(suite7, uid_policy_test2) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandleNew = 0;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0118ff);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("tst_normal_uid", &xSstConfig, SS_CREATE,
            &xSsHandleNew);
    if (nRet == -SST_OBJ_ALREADY_EXIST_ERROR) {
        nRet = securestore_create_open("tst_normal_uid", &xSstConfig,
                0, &xSsHandleNew);
    }
    ASSERT_TRUE(nRet == 0);
}

/* Policy enforcement for matching UID */
CTEST(suite6, uid_policy_test1) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0110ff);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("tst_normal_uid", &xSstConfig, SS_CREATE,
            &xSsHandle);
    ASSERT_EQUAL(0, nRet);

    system("su - nobody -c 'sse_ctest_util suite8' --preserve-environment");

    nRet = securestore_delete(xSsHandle);
    ASSERT_EQUAL(0, nRet);
}

/* Policy enforcement for matching UID */
CTEST(suite6, uid_policy_test2) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0118ff);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("tst_normal_uid", &xSstConfig, SS_CREATE,
            &xSsHandle);
    ASSERT_EQUAL(0, nRet);

    system("su - nobody -c 'sse_ctest_util suite7' --preserve-environment");

    nRet = securestore_delete(xSsHandle);
    ASSERT_EQUAL(0, nRet);

    fprintf(stdout, "return value for delete [%d]\n", nRet);
}

/* Policy enforcement for matching pname */
CTEST(suite5, pname_policy_test2) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    sshandle_t xSsHandleNew = 0;
    int nStatus = -1;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0120ff);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("tst_normal_pname", &xSstConfig, SS_CREATE,
            &xSsHandle);
    ASSERT_EQUAL(0, nRet);

    if ((fork()) == 0) {
        fprintf(stdout, "child process invoked\n");

        prctl(PR_SET_NAME, "sse_pdummy\0", NULL, NULL, NULL);

        set_object_config(&xSstConfig, 0x0120ff);
        display_object_config(&xSstConfig);

        nRet = securestore_create_open("tst_normal_pname", &xSstConfig, 0,
                &xSsHandleNew);

        fprintf(stdout, "##########sl handle = %ld\n", xSsHandleNew);
        if (nRet != 0) {
            exit(0);
        } else {
            exit(1);
        }
    } else {
        if (wait(&nStatus) < 0) {
            fprintf(stdout, "child state doesn't changed\n");
        } else {
            fprintf(stdout, "nStatus:%d\n", nStatus);
        }

        nRet = securestore_delete(xSsHandle);
        ASSERT_EQUAL(0, nRet);
   }
}

/* Policy enforcement for matching pname */
CTEST(suite5, pname_policy_test1) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    sshandle_t xSsHandleNew = 0;
    int nStatus = -1;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100ff);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("test_admin_pname", &xSstConfig, SS_CREATE,
            &xSsHandle);
    ASSERT_EQUAL(0, nRet);

    if (fork() == 0) {
        fprintf(stdout, "child process invoked\n");

        prctl(PR_SET_NAME, "sse_pdummy\0", NULL, NULL, NULL);

        set_object_config(&xSstConfig, 0x0100ff);
        display_object_config(&xSstConfig);

        nRet = securestore_create_open("test_admin_pname", &xSstConfig,
                0, &xSsHandleNew);

        fprintf(stdout, "ss handle:%ld\n", xSsHandleNew);
        if (nRet != 0) {
            exit(0);
        } else {
            exit(1);
        }
    } else {
        if (wait(&nStatus) < 0) {
            fprintf(stdout, "child state doesn't changed\n");
        } else {
            fprintf(stdout, "nStatus:%d\n", nStatus);
        }

        nRet = securestore_delete(xSsHandle);
        ASSERT_EQUAL(0, nRet);
    }
}

/* Readonce policy enforcement for cpdo */
CTEST(suite4, readonce_policy_test2) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    unsigned char cLoadBuf[512] = {0};
    uint32_t nActLen = 0;
    int nRet = -1;

    display_object_config(&xSstConfig);

    nRet = securestore_create_open("test_admin_rdone", &xSstConfig, 0,
            &xSsHandle);
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_retrieve(xSsHandle, NULL, cLoadBuf,
            (unsigned int)sizeof(cLoadBuf), &nActLen);

    fprintf(stdout, "####### nRet = %d\n", nRet);
    ASSERT_TRUE(nRet < 0);

    nRet = securestore_delete(xSsHandle);
    ASSERT_EQUAL(0, nRet);
}

/* Readonce policy enforcement for cpdo */
CTEST(suite4, readonce_policy_test) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    unsigned char cLoadBuf[512] = {0};
    uint32_t nActLen = 0;
    unsigned char cBuf[] = ATOM_MSG;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0104ff);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("test_admin_rdone", &xSstConfig, SS_CREATE,
            &xSsHandle);
    if (nRet < 0) {
        fprintf(stdout, "opening already created cpdo\n");

        set_object_config(&xSstConfig, 0x000000);
        display_object_config(&xSstConfig);

        nRet = securestore_create_open("test_admin_rdone", &xSstConfig, 0,
                &xSsHandle);
    } else {
        nRet = securestore_save(xSsHandle, NULL, cBuf, (unsigned int)sizeof(cBuf));
    }
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_retrieve(xSsHandle, NULL, cLoadBuf,
            (unsigned int)sizeof(cLoadBuf), &nActLen);
    ASSERT_STR((char*)cBuf, (char*)cLoadBuf);
}

/* test the lock policy enforcement for cpdo */
CTEST(suite4, lock_policy_test) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    unsigned char cBuf[] = ATOM_MSG;
    unsigned char cModBuf[] = MOD_MSG;
    unsigned char cLoadBuf[512] = {0};
    uint32_t nActLen = 0;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0101ff);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("test_admin_lock", &xSstConfig, SS_CREATE,
            &xSsHandle);
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_save(xSsHandle, NULL, cBuf, (unsigned int)sizeof(cBuf));
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_retrieve(xSsHandle, NULL, cLoadBuf,
            (unsigned int)sizeof(cLoadBuf), &nActLen);
    ASSERT_EQUAL(sizeof(cBuf), (intmax_t)nRet);

    nRet = securestore_save(xSsHandle, NULL, cModBuf, (unsigned int)sizeof(cModBuf));
    ASSERT_NOT_EQUAL(0, nRet);

/*  Needs further discussion to delete locked objects */
/*
    nRet = securestore_delete(xSsHandle);
    ASSERT_EQUAL(0, nRet);
*/
}

/* Policy enforcement for read permission */
CTEST(suite3, read_policy_test1) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    unsigned char cBuf[] = ATOM_MSG;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x01008a);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("test_admin_read", &xSstConfig, SS_CREATE,
            &xSsHandle);
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_save(xSsHandle, NULL, cBuf, (unsigned int)sizeof(cBuf));
    ASSERT_EQUAL(-SST_OBJ_ACCESS_PERMS_ERROR, nRet);

    nRet = securestore_delete(xSsHandle);
    ASSERT_EQUAL(0, nRet);
}

/* Policy enforcement for write permission */
CTEST(suite3, write_policy_test2) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    unsigned char cLoadBuf[512] = {0};
    uint32_t nActLen = 0;
    unsigned char cBuf[] = ATOM_MSG;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100c5);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("test_normal_wrt", &xSstConfig, SS_CREATE,
            &xSsHandle);
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_retrieve(xSsHandle, NULL, cLoadBuf,
            (unsigned int)sizeof(cLoadBuf), &nActLen);
    if (nRet > 0) {
        ASSERT_STR((char*)cBuf, (char*)cLoadBuf);
    }

    nRet = securestore_delete(xSsHandle);
    ASSERT_EQUAL(0, nRet);
}

/* Policy enforcement for write permission */
CTEST(suite3, write_policy_test1) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    unsigned char cBuf[] = ATOM_MSG;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100c5);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("test_normal_wrt", &xSstConfig, SS_CREATE,
            &xSsHandle);
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_save(xSsHandle, NULL, cBuf, (unsigned int)sizeof(cBuf));
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_delete(xSsHandle);
    ASSERT_EQUAL(0, nRet);
}

/* Modify the CPDO data */
CTEST(suite2, modify_data_test) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    unsigned char cBuf[] = ATOM_MSG;
    unsigned char cModBuf[] = MOD_MSG;
    unsigned char cLoadBuf[512] = {0};
    uint32_t nActLen = 0;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x10100ff);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("test_admin_save", &xSstConfig, SS_CREATE,
            &xSsHandle);
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_save(xSsHandle, NULL, cBuf, (unsigned int)sizeof(cBuf));
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_retrieve(xSsHandle, NULL, cLoadBuf,
            (unsigned int)sizeof(cLoadBuf), &nActLen);
    fprintf(stdout, "Content before modifying -'%s'\n", cLoadBuf);

    nRet = securestore_save(xSsHandle, NULL, cModBuf, (unsigned int)sizeof(cModBuf));
    ASSERT_EQUAL(0, nRet);

    memset_s(cLoadBuf, sizeof(cLoadBuf), 0x0, sizeof(cLoadBuf));
    nRet = securestore_retrieve(xSsHandle, NULL, cLoadBuf,
            (unsigned int)sizeof(cLoadBuf), &nActLen);
    fprintf(stdout, "Content after modifying -'%s'\n", cLoadBuf);

    nRet = securestore_delete(xSsHandle);
    ASSERT_EQUAL(0, nRet);
}

/* Save data to CPDO */
CTEST(suite2, save_data_test) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    unsigned char cBuf[] = ATOM_MSG;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x10100ff);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("test_admin_save", &xSstConfig, SS_CREATE,
            &xSsHandle);
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_save(xSsHandle, NULL, cBuf, (unsigned int)sizeof(cBuf));
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_delete(xSsHandle);
    ASSERT_EQUAL(0, nRet);
}

/* Load data from CPDO */
CTEST(suite2, load_cpdo_test) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    unsigned char cLoadBuf[512] = {0};
    uint32_t nActLen = 0;
    unsigned char cBuf[] = ATOM_MSG;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100ff);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("test_normal_load", &xSstConfig, 0,
            &xSsHandle);
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_retrieve(xSsHandle, NULL, cLoadBuf,
            (unsigned int)sizeof(cLoadBuf), &nActLen);
    ASSERT_STR((char*)cBuf, (char*)cLoadBuf);

    nRet = securestore_delete(xSsHandle);
    ASSERT_EQUAL(0, nRet);
}

/* Save and load from CPDO */
CTEST(suite2, load_save_test) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    unsigned char cLoadBuf[512] = {0};
    uint32_t nActLen = 0;
    unsigned char cBuf[] = ATOM_MSG;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100ff);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("test_normal_load", &xSstConfig, SS_CREATE,
            &xSsHandle);
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_save(xSsHandle, NULL, cBuf, (unsigned int)sizeof(cBuf));
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_retrieve(xSsHandle, NULL, cLoadBuf,
            (unsigned int)sizeof(cLoadBuf), &nActLen);
    ASSERT_STR((char*)cBuf, (char*)cLoadBuf);
}

/*Open an already created object */
CTEST(suite1, open_created_cpdo) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x1000000);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("test_admin_open", &xSstConfig, 0,
            &xSsHandle);
    fprintf(stdout, "###ss handle = %ld\n", xSsHandle);
    ASSERT_EQUAL(0, nRet);

    nRet = securestore_delete(xSsHandle);
    ASSERT_EQUAL(0, nRet);
}

/* Create admin CPDO object */
CTEST(suite1, create_cpdo_test) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x10100ff);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("test_admin_open", &xSstConfig, SS_CREATE,
            &xSsHandle);
    fprintf(stdout, "###ss handle = %ld\n", xSsHandle);
    ASSERT_EQUAL(0, nRet);
}

/* Open an object without creating it */
CTEST(suite1, open_object) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100ff);
    display_object_config(&xSstConfig);

    nRet = securestore_create_open("test_open", &xSstConfig, 0,
            &xSsHandle);
    fprintf(stdout, "###ss handle = %ld\n", xSsHandle);
    ASSERT_NOT_EQUAL(0, nRet);
}

/* Open object with NULL object name */
CTEST(suite1, open_null_object) {
    sst_obj_config_t xSstConfig = {0};
    sshandle_t xSsHandle = 0;
    int nRet = -1;

    nRet = securestore_create_open(NULL, &xSstConfig, SS_CREATE, &xSsHandle);
    ASSERT_NOT_EQUAL(0, nRet);
}
