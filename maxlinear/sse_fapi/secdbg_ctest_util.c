/*******************************************************************************

  Copyright © 2022-2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*  *****************************************************************************
 *         File Name    : secdbg_ctest_util.c									*
 *         Description  : ctest application for secure debug APIs.				*
 *						  CTEST is the API from CMake project.					*
 *  *****************************************************************************/

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "ctest.h"
#include "secdbg_test_app.h"
#include "secdbg_ctest_util.h"

#include "safe_str_lib.h"
#include "safe_lib.h"
#include "safe_mem_lib.h"

#define PRINT(fmt, args...)                                     \
    do {                                                        \
        printf("%s- "fmt"\n", __func__, ##args);                \
    } while (0)

#define PRINT_BUF(aBuf, nLen)						\
	do {											\
		uint32_t i = 0;								\
		for (i = 0; i < nLen; i++)	{				\
			printf("0x%02x ", aBuf[i]);				\
		}											\
		printf("\n");								\
	} while (0)

/* read and write retry on EINTR interrupt */
#define READ_WRITE_RETRY(nRet, expr)				\
	do {											\
		nRet = (expr);								\
	} while((nRet != EOF) && (errno == EINTR))

/* prepare for the test case */
#define PREPARE_AUTHINIT1()										\
	do {														\
		ctest_rename_authkey_file(nPortNum, false);				\
	} while (0)

#define AUTH_INIT_RETRY(nRet, expr)                             \
    int nTrials = 10;                                           \
    do {                                                        \
        nRet = (expr);                                          \
        sleep(1);                                               \
    } while ((nTrials--) && (nRet < 0) &&                       \
            (nRet == -SDBG_INVALID_AUTH_INIT_REQUEST))

#define PREPARE_AUTHINIT3()										\
	do {														\
		ctest_rename_authkey_file(nPortNum, false);				\
		ctest_backupnrestore_authkey_file(nPortNum, false);		\
		secdbg_delete_authkey(&xSstConfig, nPortNum);			\
	} while (0)

#define PREPARE_AUTHVERIFY3()									\
	do {														\
		ctest_rename_keypair_file(nPortNum, false);				\
		ctest_rename_authkey_file(nPortNum, false);				\
		ctest_backupnrestore_authkey_file(nPortNum, false);		\
		ctest_backupnrestore_keypair_file(nPortNum, false);		\
		secdbg_delete_authkey(&xSstConfig, nPortNum);			\
	} while (0)


/* wrapped authkey and private keypair file path */
#define WRAP_AUTHKEY_PATH   "/private/port%d_wrap_authkey.txt"
#define PRI_KEYPAIR_PATH	"/private/port%d_prikey.pem"

/* wrapped authkey and private keypair file path for backup purpos */
#define WRAP_AUTHKEY_PATH_BACKUP	"/private/port%d_wrap_authkey.txt.bak"
#define PRI_KEYPAIR_PATH_BACKUP		"/private/port%d_prikey.pem.bak"

/* wrapped authkey and private keypair file path for rename to perform
	file doesn't exists test cases */
#define WRAP_AUTHKEY_PATH_RENAME	"/private/port%d_wrap_authkey.txt.rename"
#define PRI_KEYPAIR_PATH_RENAME		"/private/port%d_prikey.pem.rename"

/* process name */
char p_name[MAX_USER_NAME];

/* default port number */
int nPortNum = 2;

/*
 * @brief ctest backup and restore files
 */
static int ctest_backupnrestore_files(const char *pcOrigPath, const char *pcBackPath)
{
	FILE *fpOrig = NULL;
	FILE *fpBack = NULL;
	int nRet = -1;
	
	if (fopen_s(&fpOrig, pcOrigPath, "r") != 0) {
		PRINT("failed to open %s file", pcOrigPath);
		nRet = -EFAULT;
		goto end;
	}
	if (fopen_s(&fpBack, pcBackPath, "w") != 0) {
		PRINT("failed to open %s file", pcBackPath);
		nRet = -EFAULT;
		goto end;
	}
	while (!feof(fpOrig)) {
		READ_WRITE_RETRY(nRet, fgetc(fpOrig));
		if (nRet == EOF) {
			break;
		}
		READ_WRITE_RETRY(nRet, fputc(nRet, fpBack));
	}
	nRet = 0;
end:
	if (fpOrig) {
		fclose(fpOrig);
	}
	if (fpBack) {
		fclose(fpBack);
	}
	return nRet;
}

/*
 * @brief ctest modify content of a file
 */
static int ctest_modify_content_file(const char *pcFile, int nLineNum)
{
	FILE *fp = NULL;
	int nRet = -1;
	int nLinesModify = 2;
	
	if (fopen_s(&fp, pcFile, "rw+") != 0) {
		PRINT("failed to open %s file", pcFile);
		nRet = -EFAULT;
		goto end;
	}

	while (!feof(fp)) {
		READ_WRITE_RETRY(nRet, fgetc(fp));
		if (nRet == EOF) {
			break;
		}
		/* exclude nLineNum lines from modification. */
		if (nLineNum > 0) {
			if (nRet == '\n') {
				nLineNum--;
			}
			continue;
		}
		if ((nLinesModify--) > 0) {
			fseek(fp, -1, SEEK_CUR);
			READ_WRITE_RETRY(nRet, fputc((nRet + 1), fp));
		}
	}
	nRet = 0;
end:
	if (fp) {
		fclose(fp);
	}
	return nRet;
}

/*
 * @brief modify the private keypair file
 */
static int ctest_modify_keypair_file(int nPort)
{
	char cPath[sizeof(PRI_KEYPAIR_PATH) + 1] = {0};

	if (sprintf_s(cPath, sizeof(PRI_KEYPAIR_PATH), PRI_KEYPAIR_PATH,
			nPort) < 0) {
		PRINT("failed to fill the buffer");
		return -EFAULT;
	}

	return ctest_modify_content_file(cPath, 3);
}

/*
 * @brief modify the wrapped authkey file
 */
static int ctest_modify_authkey_file(int nPort)
{
	char cPath[sizeof(WRAP_AUTHKEY_PATH) + 1] = {0};

	if (sprintf_s(cPath, sizeof(WRAP_AUTHKEY_PATH), WRAP_AUTHKEY_PATH,
			nPort) < 0) {
		PRINT("failed to fill the buffer");
		return -EFAULT;
	}

	return ctest_modify_content_file(cPath, 0);
}

/*
 * @brief backup and restore authkey file
 */
int ctest_backupnrestore_authkey_file(int nPort, bool nBackup)
{
	char cOrigPath[sizeof(WRAP_AUTHKEY_PATH) + 1] = {0};
	char cBackPath[sizeof(WRAP_AUTHKEY_PATH_BACKUP) + 1] = {0};
	int nRet = -1;

	if (sprintf_s(cOrigPath, sizeof(WRAP_AUTHKEY_PATH), WRAP_AUTHKEY_PATH,
			nPort) < 0) {
		PRINT("failed to fill the buffer");
		return nRet;
	}

	if (sprintf_s(cBackPath, sizeof(WRAP_AUTHKEY_PATH_BACKUP),
			WRAP_AUTHKEY_PATH_BACKUP, nPort) < 0) {
		PRINT("failed to fill the buffer");
		return nRet;
	}
	if (nBackup) {
		nRet = ctest_backupnrestore_files(cOrigPath, cBackPath);
	} else {
		nRet = ctest_backupnrestore_files(cBackPath, cOrigPath);
	}
	return nRet;
}

/*
 * @brief backup and restore private keypair file
 */
int ctest_backupnrestore_keypair_file(int nPort, bool nBackup)
{
	char cOrigPath[sizeof(PRI_KEYPAIR_PATH) + 1] = {0};
	char cBackPath[sizeof(PRI_KEYPAIR_PATH_BACKUP) + 1] = {0};
	int nRet = -1;

	if (sprintf_s(cOrigPath, sizeof(PRI_KEYPAIR_PATH),
			PRI_KEYPAIR_PATH, nPort) < 0) {
		PRINT("failed to fill the buffer");
		return nRet;
	}

	if (sprintf_s(cBackPath, sizeof(PRI_KEYPAIR_PATH_BACKUP),
			PRI_KEYPAIR_PATH_BACKUP, nPort) < 0) {
		PRINT("failed to fill the buffer");
		return nRet;
	}
	if (nBackup) {
		nRet = ctest_backupnrestore_files(cOrigPath, cBackPath);
	} else {
		nRet = ctest_backupnrestore_files(cBackPath, cOrigPath);
	}
	return nRet;

}

/*
 * @brief removes the backup files
 */
void ctest_remove_backup_files(int nPort)
{
	char cBackPath[sizeof(WRAP_AUTHKEY_PATH_RENAME) + 1] = {0};

	if (sprintf_s(cBackPath, sizeof(WRAP_AUTHKEY_PATH_BACKUP),
			WRAP_AUTHKEY_PATH_BACKUP, nPort) < 0) {
		PRINT("failed to fill the buffer");
	}
	if (remove(cBackPath) == 0) {
		PRINT("removed %s file", cBackPath);
	}

	memset_s(cBackPath, (sizeof(WRAP_AUTHKEY_PATH_RENAME) + 1), 0,
			sizeof(WRAP_AUTHKEY_PATH_RENAME));	
	if (sprintf_s(cBackPath, sizeof(PRI_KEYPAIR_PATH_BACKUP),
			PRI_KEYPAIR_PATH_BACKUP, nPort) < 0) {
		PRINT("failed to fill the buffer");
	}
	if (remove(cBackPath) == 0) {
		PRINT("removed %s file", cBackPath);
	}
	return;
}

/*
 * @brief rename keypair file
 */
int ctest_rename_keypair_file(int nPort, bool nRename)
{
	char cOrigPath[sizeof(PRI_KEYPAIR_PATH) + 1 ] = {0};
	char cRenamePath[sizeof(PRI_KEYPAIR_PATH_RENAME) + 1 ] = {0};
	int nRet = -1;

	if (sprintf_s(cOrigPath, sizeof(PRI_KEYPAIR_PATH), PRI_KEYPAIR_PATH,
			nPort) < 0) {
		PRINT("failed to fill the buffer");
		return nRet;
	}

	if (sprintf_s(cRenamePath, sizeof(PRI_KEYPAIR_PATH_RENAME),
			PRI_KEYPAIR_PATH_RENAME, nPort) < 0) {
		PRINT("failed to fill the buffer");
		return nRet;
	}

	if (nRename) {
		nRet = rename(cOrigPath, cRenamePath);
	} else {
		nRet = rename(cRenamePath, cOrigPath);
	}

	return nRet;
}

/*
 * @brief rename wrapped authkey file
 */
int ctest_rename_authkey_file(int nPort, bool nRename)
{
	char cOrigPath[sizeof(WRAP_AUTHKEY_PATH) + 1 ] = {0};
	char cRenamePath[sizeof(WRAP_AUTHKEY_PATH_RENAME) + 1 ] = {0};
	int nRet = -1;

	if (sprintf_s(cOrigPath, sizeof(WRAP_AUTHKEY_PATH), WRAP_AUTHKEY_PATH,
			nPort) < 0) {
		PRINT("failed to fill the buffer");
		return nRet;
	}

	if (sprintf_s(cRenamePath, sizeof(WRAP_AUTHKEY_PATH_RENAME),
			WRAP_AUTHKEY_PATH_RENAME, nPort) < 0) {
		PRINT("failed to fill the buffer");
		return nRet;
	}

	if (nRename) {
		nRet = rename(cOrigPath, cRenamePath);
	} else {
		nRet = rename(cRenamePath, cOrigPath);
	}

	return nRet;
}

/*
 * @brief  Display the policy values
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

    /* fill policy perm member*/
    unObjectConfig = unObjectConfig >> 2;
    pxSstConfig->policy_attr.u.field.lock = unObjectConfig & 1;
    unObjectConfig = unObjectConfig >> 1;
    pxSstConfig->policy_attr.u.field.no_load_to_userspace = unObjectConfig & 1;
    unObjectConfig = unObjectConfig >> 1;
    pxSstConfig->policy_attr.u.field.read_once = unObjectConfig & 1;

    /* Ignore the uid, pname and gid bit */
    unObjectConfig = unObjectConfig >> 1;
    pxSstConfig->policy_attr.u.field.ignore_uid = unObjectConfig & 1;
    unObjectConfig = unObjectConfig >> 1;
    pxSstConfig->policy_attr.u.field.ignore_gid = unObjectConfig & 1;
    unObjectConfig = unObjectConfig >> 1;
    pxSstConfig->policy_attr.u.field.ignore_pname = unObjectConfig & 1;
    unObjectConfig = unObjectConfig >> 1;
    pxSstConfig->policy_attr.u.field.wrap_flag = unObjectConfig & 3;

    /* Crypto mode flag */
    unObjectConfig = unObjectConfig >> 2;
    pxSstConfig->crypto_mode_flag = unObjectConfig & 0xFF;
}

/*
 * Start the Auth debug varify when debug port is unlocked.
 */
CTEST(suite4, SDBG_15) {
    sst_obj_config_t xSstConfig = {0};
	uint8_t aAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aNonceB[NONCE_BUF_LEN] = {0};
	uint8_t aNonceA[NONCE_BUF_LEN] = {0};
	uint8_t aSignature[MAX_SIGN_LEN] = {0};
	uint32_t nAuthKeyLen = 0;
	uint32_t nSize = 0;
	int nNonceLen = NONCE_BUF_LEN;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

	PREPARE_AUTHVERIFY3();

	nAuthKeyLen = secdbg_get_wrap_authkey(aAuthKey, nPortNum);
	ASSERT_TRUE(((int)nAuthKeyLen) > 0);

	nRet = secdbg_store_authkey(&xSstConfig, nPortNum, aAuthKey, nAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	/* clear the buffer before loading from secure storage */
	memset_s(aAuthKey, MAX_AUTHKEY_LEN, 0, MAX_AUTHKEY_LEN);
	nAuthKeyLen = 0;

	nRet = secdbg_load_authkey(&xSstConfig, nPortNum, aAuthKey, &nAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	AUTH_INIT_RETRY(nRet, securedebug_auth_init(nPortNum, aNonceB, (uint32_t*)&nNonceLen,
			aAuthKey, nAuthKeyLen));
	if (nRet < 0) {
		PRINT("secure debug auth init already done nRet:%d", nRet);
	} else {
		PRINT("secure debug auth init success");
	}
	ASSERT_EQUAL(-SDBG_PORT_ALREADY_UNLOCKED_ERROR, nRet);

	nRet = secdbg_nonce_formatnstore(aNonceA, aNonceB, nNonceLen, nPortNum);
	ASSERT_TRUE(nRet > 0);

	nRet = secdbg_gen_signature(aSignature, &nSize, nPortNum);
	if (nRet < 0) {
		PRINT("signature generation failed nRet:%d", nRet);
	} else {
		PRINT("signature generation success");
	}
	ASSERT_EQUAL(0, nRet);

	nRet = securedebug_auth_verify(nPortNum, aSignature, nSize, aNonceA,
			sizeof(aNonceA));
	if (nRet < 0) {
		PRINT("secure debug auth verify already unlocked nRet:%d", nRet);
		PRINT(" Pass");
	} else {
		PRINT("secure debug auth verify success");
	}
	ASSERT_TRUE(nRet == -SDBG_PORT_ALREADY_UNLOCKED_ERROR);
}

/*
 * Start the Auth debug varify when valid private key pair is stored in
 * /private folder and valid wrapped key is inside secure store
 */
CTEST(suite4, SDBG_14) {
    sst_obj_config_t xSstConfig = {0};
	uint8_t aAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aNonceB[NONCE_BUF_LEN] = {0};
	uint8_t aNonceA[NONCE_BUF_LEN] = {0};
	uint8_t aSignature[MAX_SIGN_LEN] = {0};
	uint32_t nAuthKeyLen = 0;
	uint32_t nSize = 0;
	int nNonceLen = NONCE_BUF_LEN;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

	PREPARE_AUTHVERIFY3();

	nAuthKeyLen = secdbg_get_wrap_authkey(aAuthKey, nPortNum);
	ASSERT_TRUE(((int)nAuthKeyLen) > 0);

	nRet = secdbg_store_authkey(&xSstConfig, nPortNum, aAuthKey, nAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	/* clear the buffer before loading from secure storage */
	memset_s(aAuthKey, MAX_AUTHKEY_LEN, 0, MAX_AUTHKEY_LEN);
	nAuthKeyLen = 0;

	nRet = secdbg_load_authkey(&xSstConfig, nPortNum, aAuthKey, &nAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	AUTH_INIT_RETRY(nRet, securedebug_auth_init(nPortNum, aNonceB, (uint32_t*)&nNonceLen,
			aAuthKey, nAuthKeyLen));
	if (nRet < 0) {
		PRINT("secure debug auth init failed nRet:%d", nRet);
		ASSERT_EQUAL(-SDBG_PORT_ALREADY_UNLOCKED_ERROR, nRet);
	} else {
		PRINT("secure debug auth init success");
		ASSERT_EQUAL(0, nRet);
	}

	nRet = secdbg_nonce_formatnstore(aNonceA, aNonceB, nNonceLen, nPortNum);
	ASSERT_TRUE(nRet > 0);

	nRet = secdbg_gen_signature(aSignature, &nSize, nPortNum);
	if (nRet < 0) {
		PRINT("signature generation failed nRet:%d", nRet);
	} else {
		PRINT("signature generation success");
	}
	ASSERT_EQUAL(0, nRet);

	nRet = securedebug_auth_verify(nPortNum, aSignature, nSize, aNonceA,
			sizeof(aNonceA));
	if (nRet < 0) {
		PRINT("secure debug auth verify failed nRet:%d", nRet);
		ASSERT_EQUAL(-SDBG_PORT_ALREADY_UNLOCKED_ERROR, nRet);
	} else {
		PRINT("secure debug auth verify success");
		PRINT(" Pass");
	}
	ASSERT_TRUE(nRet == 0);
}

/*
 * Trigger Auth debug varify after signature wait(5 seconds) timer expires.
 */
CTEST(suite4, SDBG_13) {
    sst_obj_config_t xSstConfig = {0};
	uint8_t aAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aNonceB[NONCE_BUF_LEN] = {0};
	uint8_t aNonceA[NONCE_BUF_LEN] = {0};
	uint8_t aSignature[MAX_SIGN_LEN] = {0};
	uint32_t nAuthKeyLen = 0;
	uint32_t nSize = 0;
	int nNonceLen = NONCE_BUF_LEN;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

	PREPARE_AUTHVERIFY3();

	nAuthKeyLen = secdbg_get_wrap_authkey(aAuthKey, nPortNum);
	ASSERT_TRUE(((int)nAuthKeyLen) > 0);

	nRet = secdbg_store_authkey(&xSstConfig, nPortNum, aAuthKey, nAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	/* clear the buffer before loading from secure storage */
	memset_s(aAuthKey, MAX_AUTHKEY_LEN, 0, MAX_AUTHKEY_LEN);
	nAuthKeyLen = 0;

	nRet = secdbg_load_authkey(&xSstConfig, nPortNum, aAuthKey, &nAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	AUTH_INIT_RETRY(nRet, securedebug_auth_init(nPortNum, aNonceB, (uint32_t*)&nNonceLen,
			aAuthKey, nAuthKeyLen));
	if (nRet < 0) {
		PRINT("secure debug auth init failed nRet:%d", nRet);
		ASSERT_EQUAL(-SDBG_PORT_ALREADY_UNLOCKED_ERROR, nRet);
	} else {
		PRINT("secure debug auth init success");
		ASSERT_EQUAL(0, nRet);
	}

	PRINT("Waiting for 5 seconds");
	sleep(5);

	nRet = secdbg_nonce_formatnstore(aNonceA, aNonceB, nNonceLen, nPortNum);
	ASSERT_TRUE(nRet > 0);

	nRet = secdbg_gen_signature(aSignature, &nSize, nPortNum);
	if (nRet < 0) {
		PRINT("signature generation failed nRet:%d", nRet);
	} else {
		PRINT("signature generation success");
	}
	ASSERT_EQUAL(0, nRet);

	nRet = securedebug_auth_verify(nPortNum, aSignature, nSize, aNonceA,
			sizeof(aNonceA));
	if (nRet < 0) {
		PRINT("secure debug auth verify failed nRet:%d", nRet);
		PRINT(" Pass");
	} else {
		PRINT("secure debug auth verify success");
	}
	ASSERT_TRUE((-SDBG_INVALID_AUTH_VERIFY_REQUEST) == nRet);
}

/*
 * Start the Auth debug varify when invalid/wrong private key pair was stored
 * in /private folder and valid wrapped key is inside secure store.
 */
CTEST(suite4, SDBG_12) {
    sst_obj_config_t xSstConfig = {0};
	uint8_t aAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aNonceB[NONCE_BUF_LEN] = {0};
	uint8_t aNonceA[NONCE_BUF_LEN] = {0};
	uint8_t aSignature[MAX_SIGN_LEN] = {0};
	uint32_t nAuthKeyLen = 0;
	uint32_t nSize = 0;
	int nNonceLen = NONCE_BUF_LEN;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

	PREPARE_AUTHVERIFY3();

	nAuthKeyLen = secdbg_get_wrap_authkey(aAuthKey, nPortNum);
	ASSERT_TRUE(((int)nAuthKeyLen) > 0);

	nRet = secdbg_store_authkey(&xSstConfig, nPortNum, aAuthKey, nAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	/* clear the buffer before loading from secure storage */
	memset_s(aAuthKey, MAX_AUTHKEY_LEN, 0, MAX_AUTHKEY_LEN);
	nAuthKeyLen = 0;

	nRet = secdbg_load_authkey(&xSstConfig, nPortNum, aAuthKey, &nAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	AUTH_INIT_RETRY(nRet, securedebug_auth_init(nPortNum, aNonceB, (uint32_t*)&nNonceLen,
			aAuthKey, nAuthKeyLen));
	if (nRet < 0) {
		PRINT("secure debug auth init failed nRet:%d", nRet);
		ASSERT_EQUAL(-SDBG_PORT_ALREADY_UNLOCKED_ERROR, nRet);
	} else {
		PRINT("secure debug auth init success");
		ASSERT_EQUAL(0, nRet);
	}

	nRet = secdbg_nonce_formatnstore(aNonceA, aNonceB, nNonceLen, nPortNum);
	ASSERT_TRUE(nRet > 0);

	/* modify the content of keypair file */
	nRet = ctest_modify_keypair_file(nPortNum);
	ASSERT_EQUAL(0, nRet);

	nRet = secdbg_gen_signature(aSignature, &nSize, nPortNum);
	if (nRet < 0) {
		PRINT("signature generation failed nRet:%d", nRet);
		PRINT(" Pass");
	} else {
		PRINT("signature generation success");
	}
	ASSERT_EQUAL(0, nRet);

	nRet = securedebug_auth_verify(nPortNum, aSignature, nSize, aNonceA,
			sizeof(aNonceA));
	if (nRet < 0) {
		PRINT("secure debug auth verify failed nRet:%d", nRet);
		PRINT(" Pass");
	} else {
		PRINT("secure debug auth verify success");
	}
	ASSERT_TRUE(nRet == -SDBG_AUTH_VERIFY_ERROR);
}

/*
 * Start the Auth debug varify when private key pair file doesn't exists
 * in /private/ folder
 */
CTEST(suite4, SDBG_11) {
    sst_obj_config_t xSstConfig = {0};
	uint8_t aAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aNonceB[NONCE_BUF_LEN] = {0};
	uint8_t aNonceA[NONCE_BUF_LEN] = {0};
	uint8_t aSignature[MAX_SIGN_LEN] = {0};
	uint32_t nAuthKeyLen = 0;
	uint32_t nSize = 0;
	int nNonceLen = NONCE_BUF_LEN;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

	PREPARE_AUTHVERIFY3();

	nAuthKeyLen = secdbg_get_wrap_authkey(aAuthKey, nPortNum);
	ASSERT_TRUE(((int)nAuthKeyLen) > 0);

	nRet = secdbg_store_authkey(&xSstConfig, nPortNum, aAuthKey, nAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	/* clear the buffer before loading from secure storage */
	memset_s(aAuthKey, MAX_AUTHKEY_LEN, 0, MAX_AUTHKEY_LEN);
	nAuthKeyLen = 0;

	nRet = secdbg_load_authkey(&xSstConfig, nPortNum, aAuthKey, &nAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	AUTH_INIT_RETRY(nRet, securedebug_auth_init(nPortNum, aNonceB, (uint32_t*)&nNonceLen,
			aAuthKey, nAuthKeyLen));
	if (nRet < 0) {
		PRINT("secure debug auth init failed nRet:%d", nRet);
		ASSERT_EQUAL(-SDBG_PORT_ALREADY_UNLOCKED_ERROR, nRet);
	} else {
		PRINT("secure debug auth init success");
		ASSERT_EQUAL(0, nRet);
	}

	nRet = secdbg_nonce_formatnstore(aNonceA, aNonceB, nNonceLen, nPortNum);
	ASSERT_TRUE(nRet > 0);

	if (ctest_rename_keypair_file(nPortNum, true) < 0) {
		PRINT("renaming the private keypair file failed");
	} else {
		PRINT("renaming the private keypair file success");
	}

	nRet = secdbg_gen_signature(aSignature, &nSize, nPortNum);
	if (nRet < 0) {
		PRINT("signature generation failed nRet:%d", nRet);
		PRINT(" Pass");
	} else {
		PRINT("signature generation success");
	}
	ASSERT_TRUE(nRet < 0);
}

/*
 * Start the Auth key initialization when invalid key was stored in
 * secure store and /private folder hold valid key
 */
CTEST(suite3, SDBG_10) {
	sst_obj_config_t xSstConfig = {0};
	uint8_t aValidAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aInvalidAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aNonceB[NONCE_BUF_LEN] = {0};
	uint32_t nValidAuthKeyLen = 0;
	uint32_t nInvalidAuthKeyLen = 0;
	int nNonceLen = NONCE_BUF_LEN;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

	PREPARE_AUTHINIT3();

	nRet = ctest_modify_authkey_file(nPortNum);
	ASSERT_EQUAL(0, nRet);

	nInvalidAuthKeyLen = secdbg_get_wrap_authkey(aInvalidAuthKey, nPortNum);
	ASSERT_TRUE(((int)nInvalidAuthKeyLen) > 0);

	nRet = secdbg_store_authkey(&xSstConfig, nPortNum, aInvalidAuthKey, nInvalidAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	/* clear the buffer before loading from secure storage */
	memset_s(aInvalidAuthKey, MAX_AUTHKEY_LEN, 0, MAX_AUTHKEY_LEN);
	nInvalidAuthKeyLen = 0;

	/* load the invalid authkey from secure storage */
	nRet = secdbg_load_authkey(&xSstConfig, nPortNum, aInvalidAuthKey, &nInvalidAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	/* restore valid wrapped authkey from backup file */
	nRet = ctest_backupnrestore_authkey_file(nPortNum, false);
	ASSERT_EQUAL(0, nRet);

	/* load valid authkey from authkey file */
	nValidAuthKeyLen = secdbg_get_wrap_authkey(aValidAuthKey, nPortNum);
	ASSERT_TRUE(((int)nValidAuthKeyLen) > 0);

	PRINT("Invalid wrapped authkey from secure storage:");
	PRINT_BUF(aInvalidAuthKey, nInvalidAuthKeyLen);
	PRINT("Valid wrapped authkey from /private/ file:");
	PRINT_BUF(aValidAuthKey, nValidAuthKeyLen);

	AUTH_INIT_RETRY(nRet, securedebug_auth_init(nPortNum, aNonceB, (uint32_t*)&nNonceLen,
			aInvalidAuthKey, nInvalidAuthKeyLen));
	if (nRet < 0) {
		PRINT("secure debug auth init failed nRet:%d", nRet);
		PRINT(" Pass");
	} else {
		PRINT("secure debug auth init success");
	}
	ASSERT_TRUE(nRet < 0);
}

/*
 * Start the Auth key initialization when valid wrapped auth key was stored in
 * secure store but /private folder hold wrong/invalid wrapped auth key.
 */
CTEST(suite3, SDBG_9) {
    sst_obj_config_t xSstConfig = {0};
	uint8_t aValidAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aInvalidAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aNonceB[NONCE_BUF_LEN] = {0};
	uint32_t nValidAuthKeyLen = 0;
	uint32_t nInvalidAuthKeyLen = 0;
	int nNonceLen = NONCE_BUF_LEN;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

	PREPARE_AUTHINIT3();

	nValidAuthKeyLen = secdbg_get_wrap_authkey(aValidAuthKey, nPortNum);
	ASSERT_TRUE(((int)nValidAuthKeyLen) > 0);

	nRet = secdbg_store_authkey(&xSstConfig, nPortNum, aValidAuthKey, nValidAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	/* clear the buffer before loading from secure storage */
	memset_s(aValidAuthKey, MAX_AUTHKEY_LEN, 0, MAX_AUTHKEY_LEN);
	nValidAuthKeyLen = 0;

	/* load the valid authkey from secure storage */
	nRet = secdbg_load_authkey(&xSstConfig, nPortNum, aValidAuthKey, &nValidAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);
	
	nRet = ctest_modify_authkey_file(nPortNum);
	ASSERT_EQUAL(0, nRet);

	/* load invalid authkey from authkey file */
	nInvalidAuthKeyLen = secdbg_get_wrap_authkey(aInvalidAuthKey, nPortNum);
	ASSERT_TRUE(((int)nInvalidAuthKeyLen) > 0);

	PRINT("Valid wrapped authkey from secure storage:");
	PRINT_BUF(aValidAuthKey, nValidAuthKeyLen);
	PRINT("Invalid wrapped authkey from /private/ file:");
	PRINT_BUF(aInvalidAuthKey, nInvalidAuthKeyLen);

	AUTH_INIT_RETRY(nRet, securedebug_auth_init(nPortNum, aNonceB, (uint32_t*)&nNonceLen,
			aValidAuthKey, nValidAuthKeyLen));
	if (nRet < 0) {
		PRINT("secure debug auth init failed nRet:%d", nRet);
	} else {
		PRINT("secure debug auth init success");
		PRINT(" Pass");
	}
	ASSERT_EQUAL(0, nRet);
}

/*
 * Start the Auth key initialization with key store operation for a debug port
 * with invalid wrapped auth key
 */
CTEST(suite3, SDBG_8) {
    sst_obj_config_t xSstConfig = {0};
	uint8_t aAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aNonceB[NONCE_BUF_LEN] = {0};
	uint32_t nAuthKeyLen = 0;
	int nNonceLen = NONCE_BUF_LEN;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

	PREPARE_AUTHINIT3();

	nRet = ctest_modify_authkey_file(nPortNum);
	ASSERT_EQUAL(0, nRet);

	nAuthKeyLen = secdbg_get_wrap_authkey(aAuthKey, nPortNum);
	ASSERT_TRUE(((int)nAuthKeyLen) > 0);

	nRet = secdbg_store_authkey(&xSstConfig, nPortNum, aAuthKey, nAuthKeyLen);
	if (nRet < 0) {
		PRINT("can't able to store invalid wrapped authkey to secure storage");
	} else {
		PRINT("successfully store invalid wrapped authkey to secure storage");
	}
	ASSERT_EQUAL(0 ,nRet);

	/* clear the buffer before loading from secure storage */
	memset_s(aAuthKey, MAX_AUTHKEY_LEN, 0, MAX_AUTHKEY_LEN);
	nAuthKeyLen = 0;

	nRet = secdbg_load_authkey(&xSstConfig, nPortNum, aAuthKey, &nAuthKeyLen);
	if (nRet < 0) {
		PRINT("can't able to load wrapped authkey from secure storage");
	} else {
		PRINT("successfully load wrapped authkey from secure storage");
	}
	ASSERT_EQUAL(0 ,nRet);

	AUTH_INIT_RETRY(nRet, securedebug_auth_init(nPortNum, aNonceB, (uint32_t*)&nNonceLen,
			aAuthKey, nAuthKeyLen));
	if (nRet < 0) {
		PRINT("secure debug auth init failed nRet:%d", nRet);
		PRINT(" Pass");
	} else {
		PRINT("secure debug auth init success");
	}
	ASSERT_TRUE(nRet < 0);
}

/*
 * Start the Auth key initialization with key load operation for a debug port
 * with valid wrapped auth key
 */
CTEST(suite3, SDBG_7) {
    sst_obj_config_t xSstConfig = {0};
	uint8_t aAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aNonceB[NONCE_BUF_LEN] = {0};
	uint32_t nAuthKeyLen = 0;
	int nNonceLen = NONCE_BUF_LEN;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

	PREPARE_AUTHINIT3();

	nAuthKeyLen = secdbg_get_wrap_authkey(aAuthKey, nPortNum);
	ASSERT_TRUE(((int)nAuthKeyLen) > 0);

	nRet = secdbg_store_authkey(&xSstConfig, nPortNum, aAuthKey, nAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	/* clear the buffer before loading from secure storage */
	memset_s(aAuthKey, MAX_AUTHKEY_LEN, 0, MAX_AUTHKEY_LEN);
	nAuthKeyLen = 0;

	nRet = secdbg_load_authkey(&xSstConfig, nPortNum, aAuthKey, &nAuthKeyLen);
	if (nRet < 0) {
		PRINT("can't able to load wrapped authkey from secure storage");
	} else {
		PRINT("successfully loaded wrapped authkey from secure storage");
	}
	ASSERT_EQUAL(0 ,nRet);

	AUTH_INIT_RETRY(nRet, securedebug_auth_init(nPortNum, aNonceB, (uint32_t*)&nNonceLen,
			aAuthKey, nAuthKeyLen));
	if (nRet < 0) {
		PRINT("secure debug auth init failed nRet:%d", nRet);
	} else {
		PRINT("secure debug auth init success");
		PRINT(" Pass");
	}
	ASSERT_EQUAL(0, nRet);
	ASSERT_TRUE(NONCE_BUF_LEN == nNonceLen);
}

/*
 * Start the Auth key initialization with key store operation for a debug port
 * with valid wrapped auth key
 */
CTEST(suite3, SDBG_6) {
    sst_obj_config_t xSstConfig = {0};
	uint8_t aAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aNonceB[NONCE_BUF_LEN] = {0};
	uint32_t nAuthKeyLen = 0;
	int nNonceLen = NONCE_BUF_LEN;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

	PREPARE_AUTHINIT3();

	nAuthKeyLen = secdbg_get_wrap_authkey(aAuthKey, nPortNum);
	ASSERT_TRUE(((int)nAuthKeyLen) > 0);

	nRet = secdbg_store_authkey(&xSstConfig, nPortNum, aAuthKey, nAuthKeyLen);
	if (nRet < 0) {
		PRINT("can't able to store wrapped authkey to secure storage");
	} else {
		PRINT("successfully store wrapped authkey to secure storage");
	}
	ASSERT_EQUAL(0 ,nRet);

	AUTH_INIT_RETRY(nRet, securedebug_auth_init(nPortNum, aNonceB, (uint32_t*)&nNonceLen,
			aAuthKey, nAuthKeyLen));
	if (nRet < 0) {
		PRINT("secure debug auth init failed nRet:%d", nRet);
	} else {
		PRINT("secure debug auth init success");
		PRINT(" Pass");
	}

	ASSERT_EQUAL(0, nRet);
	ASSERT_EQUAL(NONCE_BUF_LEN, nNonceLen);
}


/*
 * Load the Wrapped Auth key from Secure store when auth key is not present
 * in /private/ folder.
 */
CTEST(suite2, SDBG_5) {
    sst_obj_config_t xSstConfig = {0};
	uint8_t aAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aFileAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aStoreAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint32_t nAuthKeyLen = 0;
	uint32_t nFileAuthKeyLen = 0;
	uint32_t nStoreAuthKeyLen = 0;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

	PREPARE_AUTHINIT3();

	nFileAuthKeyLen = secdbg_get_wrap_authkey(aFileAuthKey, nPortNum);
	ASSERT_TRUE(((int)nFileAuthKeyLen) > 0);

	nRet = secdbg_store_authkey(&xSstConfig, nPortNum, aFileAuthKey, nFileAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	/* rename the authkey file */
	nRet = ctest_rename_authkey_file(nPortNum, true);
	if (nRet < 0) {
		PRINT("wrapped authkey file rename failed");
		ASSERT_EQUAL(0, nRet);
	}

	nAuthKeyLen = secdbg_get_wrap_authkey(aAuthKey, nPortNum);
	ASSERT_TRUE(((int)nAuthKeyLen) < 0);

	nRet = secdbg_load_authkey(&xSstConfig, nPortNum, aStoreAuthKey, &nStoreAuthKeyLen);
	if (nRet < 0) {
		PRINT("can't able to load wrapped authkey from secure storage");
	} else {
		PRINT("successfully loaded wrapped authkey from secure storage");
		PRINT(" Pass");
	}
	ASSERT_EQUAL(0, nRet);

	ASSERT_EQUAL(nFileAuthKeyLen, nStoreAuthKeyLen);
	ASSERT_STR((char*)&aFileAuthKey[0], (char*)&aStoreAuthKey[0]);
}

/*
 * Load the Wrapped Auth key from Secure store when Auth key is inside
 * /private/ folder as well.
 */
CTEST(suite2, SDBG_4) {
    sst_obj_config_t xSstConfig = {0};
	uint8_t aFileAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint8_t aStoreAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint32_t nFileAuthKeyLen = 0;
	uint32_t nStoreAuthKeyLen = 0;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

	PREPARE_AUTHINIT3();

	nFileAuthKeyLen = secdbg_get_wrap_authkey(aFileAuthKey, nPortNum);
	ASSERT_TRUE(((int)nFileAuthKeyLen) > 0);

	nRet = secdbg_store_authkey(&xSstConfig, nPortNum, aFileAuthKey, nFileAuthKeyLen);
	ASSERT_EQUAL(0 ,nRet);

	nRet = secdbg_load_authkey(&xSstConfig, nPortNum, aStoreAuthKey, &nStoreAuthKeyLen);
	if (nRet < 0) {
		PRINT("can't able to load wrapped authkey from secure storage");
	} else {
		PRINT("successfully loaded wrapped authkey from secure storage");
		PRINT(" Pass");
	}
	ASSERT_EQUAL(0, nRet);

	ASSERT_EQUAL(nFileAuthKeyLen, nStoreAuthKeyLen);
	ASSERT_STR((char*)aFileAuthKey, (char*)aStoreAuthKey);
}

/*
 * Store the Wrapped Auth key in Secure store from /private folder using load.
 */
CTEST(suite1, SDBG_3) {
    sst_obj_config_t xSstConfig = {0};
	uint8_t aAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint32_t nAuthKeyLen = 0;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

	PREPARE_AUTHINIT3();

	nAuthKeyLen = secdbg_get_wrap_authkey(aAuthKey, nPortNum);
	ASSERT_TRUE(((int)nAuthKeyLen) > 0);

	nRet = secdbg_load_authkey(&xSstConfig, nPortNum, aAuthKey, &nAuthKeyLen);
	if (nRet < 0) {
		PRINT("can't load wrapped authkey from secure storage");
		PRINT(" Pass");
	} else {
		PRINT("wrapped authkey load succeded");
	}
	ASSERT_TRUE(nRet < 0);
}

/*
 * Store the Wrapped Auth key in Secure store when key is not
 * present in /private/ folder.
 */
CTEST(suite1, SDBG_2) {
	uint8_t aAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint32_t nAuthKeyLen = 0;

	PREPARE_AUTHINIT1();

	/* rename the wrapped auth file */
	if (ctest_rename_authkey_file(nPortNum, true) < 0) {
		PRINT("wrapped authkey file not found");
		ASSERT_TRUE(false);
	}

	nAuthKeyLen = secdbg_get_wrap_authkey(aAuthKey, nPortNum);
	if (((int)nAuthKeyLen) < 0) {
		PRINT("Wrapped authkey doesn't exists");
		PRINT(" Pass");
	} else {
		PRINT("Wrapped authkey exists");
	}
	ASSERT_TRUE(((int)nAuthKeyLen) < 0);
}

/*
 * Store the Wrapped Auth key in Secure store from /private/ folder.
 */
CTEST(suite1, SDBG_1) {
    sst_obj_config_t xSstConfig = {0};
	uint8_t aAuthKey[MAX_AUTHKEY_LEN] = {0};
	uint32_t nAuthKeyLen = 0;
    int nRet = -1;

    set_object_config(&xSstConfig, 0x0100cf);
    display_object_config(&xSstConfig);

	PREPARE_AUTHINIT3();

	nAuthKeyLen = secdbg_get_wrap_authkey(aAuthKey, nPortNum);
	ASSERT_TRUE(((int)nAuthKeyLen) > 0);

	nRet = secdbg_store_authkey(&xSstConfig, nPortNum, aAuthKey, nAuthKeyLen);
	if (nRet < 0) {
		PRINT("secdbg_store_authkey failed");
	} else {
		PRINT("stored wrapped authkey to secure store");
		PRINT(" Pass");
	}
	ASSERT_EQUAL(0 ,nRet);
}
