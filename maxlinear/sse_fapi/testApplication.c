/*******************************************************************************
         Copyright © 2020-2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*  ***************************************************************************** 
 *         File Name    : testApplication.c                                     *
 *         Description  : test application to test secure service APIs          *
 *  *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/mman.h>
#include <sys/mount.h>

#include "fapi_sec_service.h"
#include "fapi_img_auth.h"
#include "fapi_sec_storage.h"
#include "fapi_sec_debug.h"
#include "testApplication.h"

#include "safe_str_lib.h"
#include "safe_lib.h"
#include "safe_mem_lib.h"

#ifdef SECDBG_TOOL
#include "secdbg_test_app.h"
#endif

/* number of storage options */
#define NUM_STORAGE_OPTS    20

/* max option buffer length */
#define MAX_OPTBUF_LEN      56

/* max data buffer length of 1KB */
#define MAX_DATABUF_LEN     1024

/* macro's to string error code */
#define TO_STR(STR)         #STR

/* debug print format */
#define PRINT(format, args...)                                  \
    do {                                                        \
        fprintf(stdout, "[sse_test_app %s():%d] "format"\n",    \
                __func__, __LINE__, ##args);                    \
    } while (0)

/* storage operations */
static char cStorageOps[NUM_STORAGE_OPTS][MAX_OPTBUF_LEN] = {
        "decrypt", "encrypt", "img_auth",
        "sec_create", "sec_save", "sec_load", "sec_delete",
#ifdef SECDBG_TOOL
        "secdbg",
#endif
#ifdef UBOOT_OPS
		"getcommitbank","getactivebank","getubootparam",
		"setcommitbank","activatebank","setubootparam",
#endif
        };

enum wrap_key_location {
    WRAP_KEY_SST_HANDLE = 0x6,
    WRAP_KEY_OTP        = 0x7,
};

/* function declarations */
void display_object_config(sst_obj_config_t *pxSsstConfig);
void set_object_config(sst_obj_config_t *pxSstConfig, uint32_t unObjectConfig);
int img_auth_option(char *pcStorageOp, const char *pcFileName);
int secure_storage_option(char* pcStorageOp, const char *pcObjectName,
        uint32_t unObjectConfig, const char *pcOrigData, uint32_t unDataLen,
        unsigned char wrap_key_asset);
#ifdef SECDBG_TOOL
int secure_debug_option(uint16_t unPortNum, uint32_t unObjectConfig);
#endif
#ifdef UBOOT_OPS
int uboot_operation_option(char* pcUbootOp, char *pcEnvName,char *pcValue);
#endif

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

    /* Ignore the uid, pname and gid bit */ 
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

/* function to convert macro's to string error code */
static const char* to_string(int nRespCode)
{
    switch (nRespCode) {
        case -SST_INVALID_OBJ_ERROR:
            return TO_STR(SST_INVALID_OBJ_ERROR);
        case -SST_OBJ_ACCESS_PERMS_ERROR:
            return TO_STR(SST_OBJ_ACCESS_PERMS_ERROR);
        case -SST_OBJ_RESOURCE_ALLOC_ERROR:
            return TO_STR(SST_OBJ_RESOURCE_ALLOC_ERROR);
        case -SST_INVALID_SSHANDLE_ERROR:
            return TO_STR(SST_INVALID_SSHANDLE_ERROR);
        case -SST_INVALID_DEVNODE_ERROR:
            return TO_STR(SST_INVALID_DEVNODE_ERROR);
        case -SST_INVALID_DATABUF_ERROR:
            return TO_STR(SST_INVALID_DATABUF_ERROR);
        case -SST_INVALID_CRYPTO_MODE_ERROR:
            return TO_STR(SST_INVALID_CRYPTO_MODE_ERROR);
        case -SST_OBJ_CREATE_ERROR:
            return TO_STR(SST_OBJ_CREATE_ERROR);
        case -SST_OBJ_OPEN_ERROR:
            return TO_STR(SST_OBJ_OPEN_ERROR);
        case -SST_OBJ_SAVE_ERROR:
            return TO_STR(SST_OBJ_SAVE_ERROR);
        case -SST_OBJ_LOAD_ERROR:
            return TO_STR(SST_OBJ_LOAD_ERROR);
        case -SST_OBJ_DELETE_ERROR:
            return TO_STR(SST_OBJ_DELETE_ERROR);
        case -SST_OBJ_CLOSE_ERROR:
            return TO_STR(SST_OBJ_CLOSE_ERROR);
    }
    return "Invalid response code";
}

/* function to display object config bit information */
static void help_object_config(void)
{
    fprintf(stdout, "secure store object config is 32-bit hex value\n");
    fprintf(stdout, "BitPosition- Configuration\n");
    fprintf(stdout, "   0       - User read\n");
    fprintf(stdout, "   1       - User write\n");
    fprintf(stdout, "   2       - Group read\n");
    fprintf(stdout, "   3       - Group write\n");
    fprintf(stdout, "   4       - Other read\n");
    fprintf(stdout, "   5       - Other write\n");
    fprintf(stdout, "   6       - Process name read for user\n");
    fprintf(stdout, "   7       - Process name modify for user\n");
    fprintf(stdout, "   8       - Lock the secure store object\n");
    fprintf(stdout, "   9       - No load secure store object to user space\n");
    fprintf(stdout, "   10      - Read only once secure store object\n");
    fprintf(stdout, "   11      - Ignore UID\n");
    fprintf(stdout, "   12      - Ignore GID\n");
    fprintf(stdout, "   13      - Ignore process name\n");
    fprintf(stdout, "   14:15   - Wrap flag\n");
    fprintf(stdout, "   16:23   - Crypto mode flag(8 bits)\n");
    fprintf(stdout, "   24      - Admin/Normal node(1 bit)\n");
    fprintf(stdout, "   25:31   - Reserved for future user(7 bits)\n");
    exit(0);
}

/* function to display example commands */
static void help_cmdexample(void)
{
    fprintf(stdout, "sse_test_app -o <image auth operation> -n <file name>\n");
    fprintf(stdout, "sse_test_app -o <secure store operation> -n <object name> -c <object config> -d <data>\n");
#ifdef SECDBG_TOOL
    fprintf(stdout, "sse_test_app -o <secdbg operation> -p <port num> -c <object config>\n");
#endif
    fprintf(stdout, "command examples:\n");
    fprintf(stdout, "sse_test_app -o decrypt -n file_name\n");
    fprintf(stdout, "sse_test_app -o encrypt -n file_name\n");
    fprintf(stdout, "sse_test_app -o img_auth -n file_name\n");
    fprintf(stdout, "sse_test_app -o sec_create -n object_name -c 0x0100cf [-k -t <asset_number>]\n");
    fprintf(stdout, "sse_test_app -o sec_save -n object_name -d \"Hello, I'm Atom\" [-k -t <asset_number>]\n");
    fprintf(stdout, "sse_test_app -o sec_load -n object_name [-k -t <asset_number>]\n");
    fprintf(stdout, "sse_test_app -o sec_delete -n object_name\n");
#ifdef SECDBG_TOOL
    fprintf(stdout, "sse_test_app -o secdbg -p 2 -c 0x0100cf\n");
    fprintf(stdout, "sse_test_app -o secdbg -p 2\n");
#endif
#ifdef UBOOT_OPS 
    fprintf(stdout, "sse_test_app -u getcommitbank \n");
    fprintf(stdout, "sse_test_app -u getactivebank \n");
    fprintf(stdout, "sse_test_app -u getubootparam -e <uboot_varaible>\n");
    fprintf(stdout, "sse_test_app -u setcommitbank -v A/B \n");
    fprintf(stdout, "sse_test_app -u activatebank -v A/B\n");
    fprintf(stdout, "sse_test_app -u setubootparam -e <uboot_varaible> -v <value>\n");
#endif
    exit(0);
}

/* function to display commands help */
static void help_cmdopt(void)
{
    fprintf(stdout, "sse_test_app: Usage details\n");
    fprintf(stdout, "   -o storage operation   => decrypt, encrypt, img_auth, sec_create, sec_save, sec_load, sec_delete and secdbg\n");
    fprintf(stdout, "   -n object or file name => test_admin, test_normal etc.. is optional for secure debug operation\n");
    fprintf(stdout, "   -c object config       => required in case of sec_create and secdbg store operation\n");
    fprintf(stdout, "   -k wrap key usage      => required when the wrap support is needed - sec_create, sec_save and sec_load\n");
    fprintf(stdout, "   -t otp wrap key        => range 11-30 (wrap key asset number in the otp)\n");
#ifdef SECDBG_TOOL
    fprintf(stdout, "   -p debug port number   => range 0-7\n");
#endif
    fprintf(stdout, "   -d string data         => message enclosed in doule quotes\n");
#ifdef UBOOT_OPS 
    fprintf(stdout, "   -u uboot operation     => getcommitbank, getactivebank,getubootparam,setcommitbank,activatebank,setubootparam\n");
	fprintf(stdout, "   -v uboot op value      => A or B\n");
	fprintf(stdout, "   -e uboot op env        => Uboot enviroment variable\n");
#endif
    fprintf(stdout, "   -h help\n");
    exit(0);
}

/* function to validate storage operation parameter */
static int validate_storage_ops(const char* pcStorageOp)
{
    int nIdx;

    for (nIdx = 0; nIdx < NUM_STORAGE_OPTS; nIdx++) {
        if (strncmp(pcStorageOp, cStorageOps[nIdx], MAX_OPTBUF_LEN) == 0) {
            return nIdx;
        }
    }
    return -1;
}
/*
 * @brief  main function to perform different openration as per
 * command line argv
 * @return
 *  returns 0 on success and -1 on failure
 */
int main(int argc, char **argv)
{
    char cFileName[MAX_OPTBUF_LEN] = {0};
#ifdef UBOOT_OPS 
    char cEnvName[MAX_OPTBUF_LEN] = {0};
    char cValue[MAX_OPTBUF_LEN] = {0};
#endif
    char cStorageOp[MAX_OPTBUF_LEN] = {0};
    char *pcOrigData = NULL;
    unsigned char wrap_key_asset = 0;
    uint32_t unObjectConfig = 0;
#ifdef SECDBG_TOOL
    uint16_t unPortNum = 0xFFFF;
#endif
    uint32_t unDataLen = 0;
    char *cEndPtr = NULL;
    int nIdx = -1;
    int prev_opt = -1;
    int asset_id = 0;

    if (argc == 1) {
        help_cmdopt();
    }

    while ((nIdx = getopt(argc, argv, "o:n:c:d:p:h:u:e:v:t:k")) != -1) {
        switch (nIdx) {
            case 'o':
                prev_opt = nIdx;
                if (sprintf_s(cStorageOp, MAX_OPTBUF_LEN, "%s", optarg) < 0) {
                    PRINT("error in copying storage operation");
                    return -1;
                }
                break;
#ifdef UBOOT_OPS 
            case 'u':
                if (sprintf_s(cStorageOp, MAX_OPTBUF_LEN, "%s", optarg) < 0) {
                    PRINT("error in copying uboot operation");
                    return -1;
                }
                break;
            case 'e':
                if (sprintf_s(cEnvName, MAX_OPTBUF_LEN, "%s", optarg) < 0) {
                    PRINT("error in uboot Env name");
                    return -1;
                }
				printf("case e %s\n",cEnvName);
                break;
            case 'v':
                if (sprintf_s(cValue, MAX_OPTBUF_LEN, "%s", optarg) < 0) {
                    PRINT("error in value");
                    return -1;
                }
                break;
#endif
            case 'n':
                if (sprintf_s(cFileName, MAX_OPTBUF_LEN, "%s", optarg) < 0) {
                    PRINT("error in copying file or object name");
                    return -1;
                }
                break;
            case 'c':
                if ((unObjectConfig = (uint32_t)strtoul(optarg, &cEndPtr, 16))
                        == 0) {
                    PRINT("failed to convert object config");
                    return -1;
                }
                break;
            case 'd':
                unDataLen = strnlen_s(optarg, MAX_DATABUF_LEN);
                pcOrigData = (char*) malloc(unDataLen+1);
                if (!pcOrigData) {
                    PRINT("memory allocation failed");
                    return -1;
                }
                if (strncpy_s(pcOrigData, unDataLen+1, optarg, (unDataLen))
                        != EOK) {
                    PRINT("error in copying the data");
                    return -1;
                }
                break;
            case 'k':
                prev_opt = nIdx;
                break;
            case 't':
                if (prev_opt != 'k') {
                    PRINT("user option is misaligned - error in processing [-%c]", nIdx);
                    return -1;
                }
                if (wrap_key_asset == 0) {
                    wrap_key_asset = (1 << WRAP_KEY_OTP);
                    asset_id = atoi(optarg);
                    if ((asset_id < MIN_OTP_ASSET_ID) || (asset_id > MAX_OTP_ASSET_ID)) {
                        PRINT("invalid asset ID");
                        return -1;
                    } else {
                      wrap_key_asset |= asset_id;
                    }
                } else {
                    PRINT("configuring wrap_key multiple times");
                    return -1;
                }
                break;
#ifdef SECDBG_TOOL
            case 'p':
                if ((unPortNum = (uint16_t)atoi(optarg)) == 0xFFFF) {
                    PRINT("failed to convert port number");
                    return -1;
                }
                break;
#endif
            case 'h':
            default:
                help_cmdopt();
        }
    }

    nIdx = validate_storage_ops(cStorageOp);

    if ((nIdx >= 0) && (nIdx <= 2)) {
#ifdef SECDBG_TOOL
        if (unPortNum != 0xFFFF) {
            PRINT("port number is not required");
        }
#endif
        if (unObjectConfig != 0) {
            PRINT("object configuration is not required");
            help_cmdexample();
        }
        if (strnlen_s(cFileName, MAX_OPTBUF_LEN) == 0) {
            PRINT("file name should not be null");
            help_cmdexample();
        }
        if (unDataLen != 0) {
            PRINT("data option is not required");
            help_cmdexample();
        }
        img_auth_option(cStorageOp, cFileName);
    } else if ((nIdx >= 3) && (nIdx <= 6)) {
        if (strnlen_s(cFileName, MAX_OPTBUF_LEN) == 0) {
            PRINT("object name should not be null");
            help_cmdexample();
        }
        secure_storage_option(cStorageOp, cFileName, unObjectConfig, pcOrigData,
                unDataLen, wrap_key_asset);
#ifdef SECDBG_TOOL
    } else if (nIdx == 7) {
        if (strnlen_s(cFileName, MAX_OPTBUF_LEN) != 0) {
            PRINT("object name is not required");
            help_cmdexample();
        }
        if (unDataLen != 0) {
            PRINT("data option is not required");
            help_cmdexample();
        }
        if (unPortNum == 0xFFFF) {
            PRINT("provide valid secure debug port number");
            return -1;
        }
        secure_debug_option(unPortNum, unObjectConfig);
#endif
#ifdef UBOOT_OPS 
    } else if ((nIdx == 8)) {
        if (strnlen_s(cValue, MAX_OPTBUF_LEN) != 0) {
            PRINT("Value not required");
            help_cmdexample();
        }
        uboot_operation_option(cStorageOp, cEnvName, cValue);
    } else if ((nIdx == 9)) {
        if (strnlen_s(cValue, MAX_OPTBUF_LEN) != 0) {
            PRINT("Value not required");
            help_cmdexample();
        }
        uboot_operation_option(cStorageOp, cEnvName, cValue);
    } else if ((nIdx == 10)) {
        if (strnlen_s(cValue, MAX_OPTBUF_LEN) != 0) {
            PRINT("Value not required");
            help_cmdexample();
        }
        uboot_operation_option(cStorageOp, cEnvName, cValue);
    } else if ((nIdx == 11)) {
        if (strnlen_s(cValue, MAX_OPTBUF_LEN) == 0) {
            PRINT("Value required");
            help_cmdexample();
        }
        if (strnlen_s(cEnvName, MAX_OPTBUF_LEN) != 0) {
            PRINT("EnvName not required");
            help_cmdexample();
        }
        uboot_operation_option(cStorageOp, cEnvName, cValue);
    } else if ((nIdx == 12)) {
         if (strnlen_s(cValue, MAX_OPTBUF_LEN) == 0) {
            PRINT("Value required");
            help_cmdexample();
        }
        if (strnlen_s(cEnvName, MAX_OPTBUF_LEN) != 0) {
            PRINT("EnvName not required");
            help_cmdexample();
        }
        uboot_operation_option(cStorageOp, cEnvName, cValue);
    } else if ((nIdx == 13)) {
        if (strnlen_s(cValue, MAX_OPTBUF_LEN) == 0) {
            PRINT("Value required");
            help_cmdexample();
        }
        if (strnlen_s(cEnvName, MAX_OPTBUF_LEN) == 0) {
            PRINT("EnvName required");
            help_cmdexample();
        }
        uboot_operation_option(cStorageOp, cEnvName, cValue);
#endif
    } else {
        PRINT("invalid storage operation");
    }

    return 0;
}

/*
 * @brief function to perform image authentication tests
 * @param
 *  pcStorageOp storage option
 *  pcFileName  file or object name
 * @return
 *  return 0 on success and -ve value on failure
 */
int img_auth_option(char *pcStorageOp, const char *pcFileName)
{
    sse_param_t xSSEParamList = {0};
    int nRet = -1;

    if ((!pcStorageOp) || (strnlen_s(pcStorageOp, MAX_OPTBUF_LEN) == 0)) {
        PRINT("storage operation should not be empty");
        goto end;
    }
    if ((!pcFileName) || (strnlen_s(pcFileName, MAX_OPTBUF_LEN) == 0)) {
        PRINT("file name should not be empty");
        goto end;
    }

    switchs (pcStorageOp) {
        cases(cStorageOps[0])
            sse_buf_alloc(&xSSEParamList);

            sprintf_s(xSSEParamList.sPath, MAX_SS_FILE_PATH_SIZE, "%s",
                    pcFileName);
            nRet = fapi_ssFRead(&xSSEParamList);
            if (nRet == -1) {
                PRINT("Secure read failed");
                goto end;
            }
            PRINT("(len=%zu)  %s\n", xSSEParamList.len, xSSEParamList.pcBuf);
            sse_buf_free(&xSSEParamList);
        break;

        cases(cStorageOps[1])
            sse_buf_alloc(&xSSEParamList);

            sprintf_s(xSSEParamList.sPath, MAX_SS_FILE_PATH_SIZE, "%s",
                    pcFileName);
            nRet = fapi_ssFWrite(&xSSEParamList);
            if (nRet < -1) {
                PRINT("Secure write failed");
                goto end;
            }
            PRINT("%s", xSSEParamList.pcBuf);
            sse_buf_free(&xSSEParamList);
        break;

        cases(cStorageOps[2])
            int nFd = -1;
            img_param_t img = {0};
            struct stat filesize = {0};

            nFd = open(pcFileName, O_RDWR);
            if (nFd < 0) {
                PRINT("The file %s could not be opened", pcFileName);
                goto end;
            }

            if (fstat(nFd, &filesize)) {
                PRINT("fstat error: [%s]\n", strerror(errno));
                close(nFd);
                goto end;
            }

            img.src_img_fd = nFd;
            img.src_img_len = filesize.st_size;
            //img.img_type = 2;

            strncpy_s(img.img_name, sizeof(img.img_name), "kernel", sizeof("kernel"));
            img.write_to_upg_part = 1;
            img.src_img_addr = mmap(0, img.src_img_len, (PROT_READ|PROT_WRITE),
                    MAP_SHARED, img.src_img_fd, 0);

            if (img.src_img_addr == MAP_FAILED) {
                PRINT("MMAP failed... %s", strerror(errno));
                close(nFd);
                goto end;
            }

            nRet = fapi_ssImgAuth(img);
            if (nRet != 0) {
                PRINT("Image validation failed");
            }

            if (munmap(img.src_img_addr, img.src_img_len) == -1) {
                PRINT("munmap failed");
            }

            if (nFd >= 0) {
                close(nFd);
            }
        break;

        defaults
            PRINT("invalid image authentication option");
        break;
    }
    switchs_end;
end:
    return nRet;
}

/*
 * @brief function to perform secure storage tests
 * @param
 *  pcStorageOp storage option
 *  pcObjectName secure store object name
 *  unObjectConfig secure store object configuration
 *  pcOrigData string data
 *  unDataLen data length
 * @return
 *  return 0 on success and -ve value on failure
 */
int secure_storage_option(char* pcStorageOp, const char *pcObjectName,
        uint32_t unObjectConfig, const char *pcOrigData, uint32_t unDataLen,
        unsigned char wrap_key_asset)
{
    sst_obj_config_t xSstConfig = {0};
    secure_wrap_asset_t xWrapConfig = {0};
    sshandle_t xSsHandle = -1;
    int nRet = -1;
    int asset_id = 0;

    if ((!pcStorageOp) || (strnlen_s(pcStorageOp, MAX_OPTBUF_LEN) == 0)) {
        PRINT("storage operation should not be empty");
        goto end;
    }
    if ((!pcObjectName) || (strnlen_s(pcObjectName, MAX_OPTBUF_LEN) == 0)) {
        PRINT("file name should not be empty");
        goto end;
    }

    switchs (pcStorageOp) {
        cases(cStorageOps[3])
            if (unObjectConfig == 0) {
                PRINT("object configuration is required for \"%s\" operation",
                        cStorageOps[3]);
                help_object_config();
                goto end;
            }
            if (unDataLen) {
                PRINT("data is not required for \"%s\" operation",
                        cStorageOps[3]);
                goto end;
            }

            set_object_config(&xSstConfig, unObjectConfig);

            if (wrap_key_asset) {
                if (wrap_key_asset & (1 << WRAP_KEY_OTP)) {
                    asset_id = wrap_key_asset & 0x3F;
                    /* share the wrap asset details during create */
                    xSstConfig.wrap_asset.key_location = KEY_IN_OTP; /* 0b01 - OTP */
                    xSstConfig.wrap_asset.u.asset_number = asset_id;
                }
            }
            display_object_config(&xSstConfig);

            nRet = securestore_create_open(pcObjectName, &xSstConfig, SS_CREATE,
                    &xSsHandle);
            if (nRet < 0) {
                PRINT("failed to create object:%s nRet:%s", pcObjectName,
                        to_string(nRet));
                goto end;
            }
            PRINT("ss handle received - [%ld]", xSsHandle);
            PRINT("created \"%s\" secure store object", pcObjectName);
	    securestore_close(xSsHandle);
        break;

        cases(cStorageOps[4])
            if (unObjectConfig != 0) {
                PRINT("object configuration is not required for \"%s\" operation",
                        cStorageOps[4]);
                goto end;
            }
            if (unDataLen == 0) {
                PRINT("data should not be null for \"%s\" operation",
                        cStorageOps[4]);
                goto end;
            }

            if (wrap_key_asset) {
                if (wrap_key_asset & (1 << WRAP_KEY_OTP)) {
                    asset_id = wrap_key_asset & 0x3F;
                    /* share the wrap asset details during open */
                    xSstConfig.wrap_asset.key_location = KEY_IN_OTP; /* 0b01 - OTP */
                    xSstConfig.wrap_asset.u.asset_number = asset_id;
                    display_object_config(&xSstConfig);
                    /* share the wrap asset details during save */
                    xWrapConfig.key_location = KEY_IN_OTP; /* 0b01 - OTP */
                    xWrapConfig.u.asset_number = asset_id;
                }
            }

            nRet = securestore_create_open(pcObjectName, &xSstConfig, 0,
                    &xSsHandle);
            if (nRet < 0) {
                PRINT("failed to open object:%s nRet:%s", pcObjectName,
                        to_string(nRet));
                goto end;
            }
            PRINT("ss handle received - [%ld]", xSsHandle);

            /* saves the cOrigData in cryptographically protected form */
            nRet = securestore_save(xSsHandle, &xWrapConfig, (const unsigned char *)pcOrigData,
                    unDataLen);
            if (nRet < 0) {
                PRINT("failed to save data nRet:%s", to_string(nRet));
                goto end;
            }
            PRINT("saved data of len:%d to \"%s\" secure store object",
                    unDataLen, pcObjectName);
            PRINT("\"%s\"", pcOrigData);
            free((void*)pcOrigData);
            pcOrigData = NULL;
	    securestore_close(xSsHandle);
        break;

        cases(cStorageOps[5])
            uint32_t nActLen = 0;
            unsigned char *pcLoadData = NULL;

            if (unObjectConfig != 0) {
                PRINT("object configuration is not required for \"%s\" operation",
                    cStorageOps[5]);
                goto end;
            }
            if (unDataLen) {
                PRINT("data is not required for \"%s\" operation",
                        cStorageOps[5]);
                goto end;
            }

            pcLoadData = (unsigned char*) malloc(MAX_DATABUF_LEN);
            if (!pcLoadData) {
                PRINT("memory allocation failed");
                goto end;
            }

            if (wrap_key_asset) {
                if (wrap_key_asset & (1 << WRAP_KEY_OTP)) {
                    asset_id = wrap_key_asset & 0x3F;
                    /* share the wrap asset details during open */
                    xSstConfig.wrap_asset.key_location = KEY_IN_OTP; /* 0b01 - OTP */
                    xSstConfig.wrap_asset.u.asset_number = asset_id;
                    display_object_config(&xSstConfig);
                    /* share the wrap asset details during load */
                    xWrapConfig.key_location = KEY_IN_OTP; /* 0b01 - OTP */;
                    xWrapConfig.u.asset_number = asset_id;
                }
            }

            nRet = securestore_create_open(pcObjectName, &xSstConfig, 0,
                    &xSsHandle);
            if (nRet < 0) {
                PRINT("failed to open object:%s nRet:%s", pcObjectName,
                        to_string(nRet));
                goto end;
            }
            PRINT("ss handle received - [%ld]", xSsHandle);

            /* retrieves the data and displayed in plain text */
            nRet = securestore_retrieve(xSsHandle, &xWrapConfig, pcLoadData,
                    MAX_DATABUF_LEN, &nActLen);
            if (nRet < 0) {
                PRINT("failed to load nRet:%s", to_string(nRet));
                goto end;
            }
            PRINT("retrieved data of len:%d from \"%s\" secure store object",
                    nActLen, pcObjectName);
            PRINT("\"%s\"", pcLoadData);
            free(pcLoadData);
            pcLoadData = NULL;
	    securestore_close(xSsHandle);
        break;

        cases(cStorageOps[6])
            if (unObjectConfig != 0) {
                PRINT("object configuration is not required for \"%s\" operation",
                    cStorageOps[6]);
                goto end;
            }
            if (unDataLen) {
                PRINT("data is not required for \"%s\" operation",
                        cStorageOps[6]);
                goto end;
            }

            nRet = securestore_create_open(pcObjectName, &xSstConfig, 0,
                    &xSsHandle);
            if (nRet < 0) {
                PRINT("failed to open object:%s nRet:%s", pcObjectName,
                        to_string(nRet));
                goto end;
            }
            PRINT("ss handle received - [%ld]", xSsHandle);

            /* deletes the secure store object */
            nRet = securestore_delete(xSsHandle);
            if (nRet < 0) {
                PRINT("failed to delete object:%s nRet:%s", pcObjectName,
                        to_string(nRet));
                goto end;
            }
            PRINT("deleted \"%s\" secure store object", pcObjectName);
        break;

        defaults
            PRINT("Invalid secure storage operation");
        break;
    }
    switchs_end;

end:
    return nRet;
}

#ifdef SECDBG_TOOL
/*
 * @brief function to perform secure debug tests
 * @param
 *  unPortNum debug port number
 *  unObjectConfig secure store object configuration
 * @return
 *  return 0 on success and -ve value on failure
 */
int secure_debug_option(uint16_t unPortNum, uint32_t unObjectConfig)
{
    sst_obj_config_t xSstConfig = {0};
    uint8_t aNonceA[NONCE_BUF_LEN] = {0};
    uint8_t aNonceB[NONCE_BUF_LEN] = {0};
    uint8_t aSignature[MAX_SIGN_LEN] = {0};
    uint8_t aAuthKey[MAX_AUTHKEY_LEN] = {0};
    uint32_t nAuthKeyLen = 0;
    int nNonceLen = sizeof(aNonceB);
    uint32_t nSize = 0;
    int nRet = -1;
#ifdef ENABLE_DEBUG
    uint32_t nIdx = 0;
#endif

    PRINT("Port num is :%u", unPortNum);

    if (unObjectConfig != 0) {
        set_object_config(&xSstConfig, unObjectConfig);
#ifdef ENABLE_DEBUG
        display_object_config(&xSstConfig);
#endif
        if (xSstConfig.crypto_mode_flag != SS_CI) {
            PRINT("unsupported crypto mode flag");
            goto end;
        }
        nAuthKeyLen = secdbg_get_wrap_authkey(aAuthKey, unPortNum);
        if (((int)nAuthKeyLen) < 0) {
            PRINT("Wrapped authentication key is not available");
            goto end;
        }
        if ((nRet = secdbg_store_authkey(&xSstConfig, unPortNum, aAuthKey,
                nAuthKeyLen)) < 0) {
            PRINT("failed to store authentication key in secure storage");
            goto end;
        }
    } else {
        if ((nRet = secdbg_load_authkey(&xSstConfig, unPortNum, aAuthKey,
                    &nAuthKeyLen)) < 0) {
            PRINT("failed to load the authentication key from secure storage");
            goto end;
        }
    }

#ifdef ENABLE_DEBUG
    PRINT("Wrapped authentication key & length:%d", nAuthKeyLen);
    for (nIdx = 0; nIdx < nAuthKeyLen; nIdx++) {
        fprintf(stdout, "0x%x ", aAuthKey[nIdx]);
    }
    PRINT("");
#endif

    nRet = securedebug_auth_init(unPortNum, aNonceB, (uint32_t*)&nNonceLen,
            aAuthKey, nAuthKeyLen);
    if (nRet < 0) {
        PRINT("Error in secure debug auth init nRet:%d", nRet);
        goto end;
    }
    PRINT("securedebug_auth_init success");

    nRet = secdbg_nonce_formatnstore(aNonceA, aNonceB, nNonceLen, unPortNum);
    if (nRet < 0) {
        PRINT("failed to store random number");
        goto end;
    }
    PRINT("secdbg_nonce_formatnstore success");

    nRet = secdbg_gen_signature(aSignature, &nSize, unPortNum);
    if (nRet < 0) {
        PRINT("signature generate failed");
        goto end;
    }

#ifdef ENABLE_DEBUG
    PRINT("Signature for debug port:%u of size :%d", unPortNum, nSize);
    for (nIdx = 0; nIdx < nSize; nIdx++) {
        fprintf(stdout, "0x%02x ", aSignature[nIdx]);
    }
    PRINT("");
#endif

    nRet = securedebug_auth_verify(unPortNum, aSignature, nSize, aNonceA,
            sizeof(aNonceA));
    if (nRet < 0) {
        PRINT("Error in secure debug auth verify nRet:%d", nRet);
        goto end;
    }
    PRINT("securedebug_auth_verify success");

end:
    return nRet;
}
#endif
#ifdef UBOOT_OPS
int uboot_operation_option(char* pcUbootOp, char *pcEnvName,
        char *pcValue)
{
    int nRet = -1;
	unsigned char *ptr=NULL;

    if ((!pcUbootOp) || (strnlen_s(pcUbootOp, MAX_OPTBUF_LEN) == 0)) {
        PRINT("storage operation should not be empty");
        goto end;
    }

    switchs (pcUbootOp) {
        cases(cStorageOps[8])
			if (strnlen_s(pcValue, MAX_OPTBUF_LEN) != 0) {
				PRINT("value is not required for \"%s\" operation ", cStorageOps[8]);
				goto end;
			}
			if (strnlen_s(pcEnvName, MAX_OPTBUF_LEN) != 0) {
				PRINT("EnvName is not required for \"%s\" operation ", cStorageOps[8]);
				goto end;
			}
            nRet = fapi_ssGetCommitBank(&ptr);
            if (nRet < 0 ) {
        		PRINT("Error in get commit bank nRet:%d", nRet);
                goto end;
            }
            PRINT("fapi_ssGetCommitBank Returned :%s", ptr);
        break;
        cases(cStorageOps[9])
			if (strnlen_s(pcValue, MAX_OPTBUF_LEN) != 0) {
				PRINT("value is not required for \"%s\" operation ", cStorageOps[9]);
				goto end;
			}
			if (strnlen_s(pcEnvName, MAX_OPTBUF_LEN) != 0) {
				PRINT("EnvName is not required for \"%s\" operation ", cStorageOps[9]);
				goto end;
			}
            nRet = fapi_ssGetActiveBank(&ptr);
            if (nRet < 0 ) {
        		PRINT("Error in get active bank nRet:%d", nRet);
                goto end;
            }
            PRINT("fapi_ssGetAciveBank Returned :%s", ptr);
        break;
        cases(cStorageOps[10])
			if (strnlen_s(pcValue, MAX_OPTBUF_LEN) != 0) {
				PRINT("value is not required for \"%s\" operation ", cStorageOps[10]);
				goto end;
			}
			if (strnlen_s(pcEnvName, MAX_OPTBUF_LEN) == 0) {
				PRINT("EnvName is required for \"%s\" operation ", cStorageOps[10]);
				goto end;
			}
            /*nRet = fapi_ssGetUbootParam(pcEnvName, &ptr);
            if (nRet < 0 ) {
				PRINT("Error in get uboot param nRet:%d", nRet);
                goto end;
            }*/
            PRINT("fapi_ssGetUbootParam Returned :%s", ptr);
        break;
        cases(cStorageOps[11])
			if (strnlen_s(pcValue, MAX_OPTBUF_LEN) == 0) {
				PRINT("value is required for \"%s\" operation ", cStorageOps[11]);
				goto end;
			}
			if (strnlen_s(pcEnvName, MAX_OPTBUF_LEN) != 0) {
				PRINT("EnvName is not required for \"%s\" operation ", cStorageOps[11]);
				goto end;
			}
            nRet = fapi_ssSetCommitBank((unsigned char*)pcValue);
            if (nRet < 0 ) {
				PRINT("Error in set commit bank nRet:%d", nRet);
                goto end;
            }
        break;
        cases(cStorageOps[12])
			if (strnlen_s(pcValue, MAX_OPTBUF_LEN) == 0) {
				PRINT("value is required for \"%s\" operation ", cStorageOps[12]);
				goto end;
			}
			if (strnlen_s(pcEnvName, MAX_OPTBUF_LEN) != 0) {
				PRINT("EnvName is not required for \"%s\" operation ", cStorageOps[12]);
				goto end;
			}
            nRet = fapi_ssActivateBank((unsigned char*)pcValue);
            if (nRet < 0 ) {
				PRINT("Error in set activate bank nRet:%d", nRet);
                goto end;
            }
        break;
       cases(cStorageOps[13])
			if (strnlen_s(pcValue, MAX_OPTBUF_LEN) == 0) {
				PRINT("value is required for \"%s\" operation ", cStorageOps[13]);
				goto end;
			}
			if (strnlen_s(pcEnvName, MAX_OPTBUF_LEN) == 0) {
				PRINT("EnvName is required for \"%s\" operation ", cStorageOps[13]);
				goto end;
			}
            /*nRet = fapi_ssSetUbootParam(pcEnvName, (unsigned char*)pcValue);
            if (nRet < 0 ) {
				PRINT("Error in set uboot param nRet:%d", nRet);
                goto end;
            }*/
        break;
 defaults
            PRINT("Invalid uboot operation");
        break;
    }
    switchs_end;

end:
    return nRet;
}
#endif
