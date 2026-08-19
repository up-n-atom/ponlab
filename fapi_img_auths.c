/**************************************************************************

  Copyright (C) 2022-2025 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 *************************************************************************/

/**************************************************************************
 *     File Name  : fapi_img_auths.c					                  *
 *     Project    : UGW                                                   *
 *     Description: provides the APIs for image authentication            *
 *                                                                        *
 *************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <crc32.h>
#include <arpa/inet.h>
#include <sys/mount.h>

#include "fapi_sec_service.h"
#include "fapi_img_auth.h"
#include "sec_upgrade.h"
#include "secure_boot.h"
#include "libfdt.h"
#include "ltq_api_include.h"
#include "help_error.h"
#include "help_logging.h"
#include "safe_str_lib.h"
#include "safe_lib.h"
#include "safe_mem_lib.h"
#include <sys/mman.h>

#define MAX_PATH_LEN 256
#define MAX_BUFFER_LENGTH 256
#define BLW_LENGTH 8 /* Number of bytes of singing header */
#define FILE_SIZE 500
#define ARRAY_SIZE(arr) (int)(sizeof(arr) / sizeof((arr)[0]))
#ifdef IMG_AUTH
static int fapi_ssImgValidate(img_param_t *pxImgParam, uint8_t upgradeOrCommit);
#endif
#ifdef FIT_IMG
static int fapi_ssValidateNestedFit(const void *fit, img_param_t image_auth, int *currentimage);
static int fapi_ssUpgradeNestedFit(const void *fit, img_param_t image_auth, int *earlyboot);
static const char *fapi_ssNormalizeFitNodeName(const char *node_name);
#endif
static int set_uboot_param_str(const char *var_name, const char* value);
static int get_uboot_param(char *var_name, char *value);
static int add_uboot_param(const char *uboot_var, const int value);
static int set_uboot_param_int(const char *var_name, const int value);
struct map_table {
	char name[MAX_FILE_NAME];
	char part[MAX_PATH_LEN];
	bool early_boot; 		/* to check for late/early boot */
};
#define UDT_NEW_IMAGE 11
#define UDT_IMAGE_RECOVERED 10
#define UDT_IMAGE_NO_ACTION 0
#define RBE 0x1
#ifdef FIT_IMG
#define BOOTLOADERFIT 0x40
#define TEPFIT 0x80
#define ROOTFSFIT 0x200
#define KERNELDTBFIT 0x100
#define ACTIVE_PART_NAME "active"
#define INACTIVE_PART_NAME "inactive"
#else
#define BOOTLOADER 0x2
#define TEP 0x4
#define ROOTFS 0x8
#define KERNEL 0x10
#define DTB 0x20
#define UPDATE_FILE  "/etc/scripts/upgrade_done.sh"
#define RECOVERED_FILE "/etc/scripts/upgrade_recovered.sh"
#endif
/* Supported image list for software upgrade */
static const int image_list[] = {
	RBE,
#ifdef FIT_IMG
	BOOTLOADERFIT,
	TEPFIT,
	KERNELDTBFIT|ROOTFSFIT,
#ifdef IMG_AUTH
	RBE|BOOTLOADERFIT|TEPFIT|ROOTFSFIT|KERNELDTBFIT,
#else
	RBE|BOOTLOADERFIT|ROOTFSFIT|KERNELDTBFIT,
#endif
#else
	BOOTLOADER,
	TEP,
	ROOTFS|KERNEL|DTB,
#ifdef IMG_AUTH
	RBE|BOOTLOADER|TEP|ROOTFS|KERNEL|DTB,
#else
	RBE|BOOTLOADER|ROOTFS|KERNEL|DTB,
#endif
#endif
};
#define ARRAY_SIZE(arr) (int)(sizeof(arr) / sizeof((arr)[0]))

/* for other flash we need to see how fetch this count */
#define LOCK_FILE "/opt/intel/etc/sse/sse_lock"
#define CONFIG_FILE "/opt/intel/etc/sse/config_partition.yaml"

#ifdef IMG_AUTH
#define EMMC_PAGE_SIZE 0x800
#define NAND_PAGE_SIZE 0x1000
static int fapi_ssvalidateImg(int nFd, void *addr, uint32_t size, uint8_t upgradeOrCommit);
static int fapi_ssValidateDTB(int nFd, const void *addr, uint8_t upgradeOrCommit);
static int is_image_hcrc_valid(image_header_t *hdr);
#endif
#ifdef IMG_AUTH
static int fapi_ssReadFromPartition(char *dev_path, char * type, long unsigned int len);
#endif
static char fapi_ssCheckActiveBank(void);
static int fapi_ssMountLateboot(char *mnt_part, char *mnt_path);
static void fapi_ssGetFileNameFromUboot(char *file_name, char *cPath);
static int fapi_ssWriteToPartition(const unsigned char *image_src_address, int image_len, char *dev_path);
static char *fapi_ssGetParitionNameEMMC(char *image_type, char bank);
extern int check_boardtype(void);
/*
 * Function to extract device name from partition name
 *
 * @name - name of the partition
 * @type - type of device to return for NAND flash:
 *         B/b - for block device /dev/mtdblockX (e.g. /dev/mtdblock16)
 *         C/c - for character device /dev/ubiX_Y (e.g. /dev/ubi1_0)
 */
extern char *getDevFromPartition(char *name, char type);


/**====================================================================
 * @brief  image header CRC  check
 *
 * @return
 *  UGW_SUCCESS on success
 *  UGW_FAILURE on failure
 ======================================================================
 */
static int is_image_hcrc_valid(image_header_t *hdr)
{
	uint32_t un_crc;
	int len = sizeof(image_header_t);
	image_header_t header;

	memmove(&header, (char *)hdr, len);
	header.img_hdr_hcrc = 0;

	un_crc = 0x00000000 ^ 0xffffffff;
	un_crc = crc32(un_crc, &header, len);

	un_crc ^= 0xffffffff;
	return (un_crc == ntohl(hdr->img_hdr_hcrc));
}

#ifdef IMG_AUTH
/**=====================================================================
 * @brief  Securely validate the image
 *
 * @param nFd
 * The fd of the file that need to be validated
 *
 * @return
 *  UGW_SUCCESS on success
 *  UGW_FAILURE on failure
 =======================================================================
 */
static int fapi_ssImgValidate(img_param_t *pxImgParam, uint8_t upgradeOrCommit)
{
	int nRet = UGW_SUCCESS;
	int nSecFd = -1, boardtype;
	image_header_t *pxImgHeader = NULL;
	unsigned char *img_ptr;
	uint32_t auth_size = 0;

	if (pxImgParam->src_img_addr == NULL)
		return UGW_FAILURE;

	img_ptr = pxImgParam->src_img_addr;
	/* Open the driver device */
	nSecFd = fapi_Fileopen(SEC_UPG_PATH,
			(O_RDWR & (~O_NONBLOCK)), U_RDWR_G_RD);
	if (nSecFd < 0) {
		LOGF_LOG_ERROR("Opening Secure driver file failed.\n");
		nRet = UGW_FAILURE;
		goto failure;
	}

	pxImgHeader = (image_header_t *)img_ptr;
	if (ntohl(*(uint32_t *)img_ptr) == FLATDT_MAGIC) {
		LOGF_LOG_DEBUG(" Authenticating the FIT image, upgradeOrCommit%d\n", upgradeOrCommit);
		nRet = fapi_ssValidateDTB(nSecFd, img_ptr, upgradeOrCommit);
		close(nSecFd);
		if (nRet < 0)
			return nRet;
		return UGW_SUCCESS;
	}
	auth_size = ntohl(pxImgHeader->img_hdr_size);

	switch (pxImgHeader->img_hdr_type) {
#ifndef FIT_IMG
		case IMG_HDR_VAR_KERNEL:
			nRet = fapi_ssvalidateImg(nSecFd, img_ptr + sizeof(image_header_t), auth_size, upgradeOrCommit);
			if (nRet != UGW_SUCCESS)
				goto failure;
			LOGF_LOG_DEBUG("Kernel Image Successfully authenticated.\n");
			break;
		case IMG_HDR_VAR_FILESYSTEM:
			nRet = fapi_ssvalidateImg(nSecFd, img_ptr + sizeof(image_header_t), (auth_size - BLW_LENGTH), upgradeOrCommit);
			if (nRet != UGW_SUCCESS)
				goto failure;
			LOGF_LOG_DEBUG("Rootfs Image Successfully authenticated.\n");
			break;
		case IMG_HDR_VAR_UBOOT:
			LOGF_LOG_DEBUG("Validating uboot image\n");
			nRet = fapi_ssvalidateImg(nSecFd, img_ptr + sizeof(image_header_t), auth_size - BLW_LENGTH, upgradeOrCommit);
			if (nRet != UGW_SUCCESS) {
				LOGF_LOG_ERROR("uboot Image Authentication Failed!\n");
				goto failure;
			}
			LOGF_LOG_DEBUG("Uboot Image Successfully authenticated.\n");
			break;
#endif
		case IMG_HDR_VAR_FIRMWARE:
			LOGF_LOG_DEBUG("Validating firmware image\n");
			if (strncmp((char *)pxImgHeader->img_hdr_name, "RBE", sizeof(pxImgHeader->img_hdr_name)) == 0)
			{
				boardtype = check_boardtype();
				if (boardtype == FLASH_TYPE_EMMC)
					img_ptr += EMMC_PAGE_SIZE + sizeof(image_header_t) + 12;
#ifndef FIT_IMG
				else
					img_ptr += NAND_PAGE_SIZE + sizeof(image_header_t) + 12;
#endif
				memcpy_s(&auth_size, 4, img_ptr - BLW_LENGTH , 4);
				nRet = fapi_ssvalidateImg(nSecFd, img_ptr, (auth_size - 4), upgradeOrCommit);
#ifndef FIT_IMG
			}
			else {
				nRet = fapi_ssvalidateImg(nSecFd, img_ptr + sizeof(image_header_t), (auth_size - BLW_LENGTH), upgradeOrCommit);
#endif
			}
			if (nRet != UGW_SUCCESS) {
				LOGF_LOG_ERROR("firmware Image Authentication Failed!\n");
				goto failure;
			}
			LOGF_LOG_DEBUG("%s image Successfully authenticated.\n", pxImgHeader->img_hdr_name);
			break;
		}
	close(nSecFd);
	return UGW_SUCCESS;

failure:
	LOGF_LOG_ERROR("Verification of image failed!\n");
	close(nSecFd);
	return UGW_FAILURE;
}

static int fapi_ssvalidateImg(int nFd, void *addr, uint32_t size, uint8_t commit)
{
	int nRet = UGW_SUCCESS;
	img_param_t xImgParam = {};
	xImgParam.src_img_addr = addr;
	xImgParam.src_img_len = size;
	xImgParam.commit = commit;
	xImgParam.chkFARB = true;
	xImgParam.chkARB = true;

	nRet = ioctl(nFd, SS_IOC_SEC_IMGAUTH, &xImgParam);
	if (nRet != UGW_SUCCESS) {
		if (commit == false) {
			LOGF_LOG_ERROR("Image authentication ioctl failed.\n");
			return UGW_FAILURE;
		} else {
			LOGF_LOG_ERROR("Image commit ioctl failed.\n");
			return UGW_FAILURE;
		}
	}
	LOGF_LOG_DEBUG ("validate/commit success!!!\n");
	return nRet;
}

static int fapi_ssValidateDTB(int nFd, const void *addr, uint8_t upgradeOrCommit)
{
	void *data;
	char* header;
	int noffset, depth = 0;
	int nRet = 0;
	int len = 0;
	int image_size;

	noffset = fdt_path_offset(addr, FIT_IMAGES_PATH);
	if (noffset < 0) {
		LOGF_LOG_ERROR("Unable to find images within fit image\n");
		return fdt_totalsize(addr);
	}
	do {
		noffset = fdt_next_node(addr, noffset, &depth);
		if (depth < 1)
			break;
		if (upgradeOrCommit && (strcmp(fdt_get_name(addr, noffset, NULL), "device-tree") == 0)) {
			fprintf(stderr,"device-tree image, nothing to be handlled while commit operation\n");
			continue;
		} 
		if (depth == 1) {
			LOGF_LOG_DEBUG( "\nnoffset %d depth %d name %s\n", noffset, depth,
				fdt_get_name(addr, noffset, NULL));
			data = (void *)fdt_getprop(addr, noffset, FIT_DATA_PROP, &len);
			if (!len) {
				LOGF_LOG_ERROR("Unable to find dtb data\n");
				return fdt_totalsize(addr);
			}
			image_size = fdt_totalsize(addr);
			LOGF_LOG_DEBUG(" Header size %d Image size %d, image_size %d\n", SBIF_ECDSA_GetHeaderSize(
				(SBIF_ECDSA_Header_t *)data, len), getImageLen((SBIF_ECDSA_Header_t *)(data)), image_size);
			/* we authenticate only the image after the FIT headers */

			header = data;
			if ((strcmp(fdt_get_name(addr, noffset, NULL), "uboot") == 0) || (strcmp(fdt_get_name(addr, noffset, NULL), "u-boot") == 0))
				nRet = fapi_ssvalidateImg(nFd, header + sizeof(image_header_t), len - sizeof(image_header_t) - BLW_LENGTH, upgradeOrCommit);
			else
				nRet = fapi_ssvalidateImg(nFd, header, len - BLW_LENGTH, upgradeOrCommit);

			if (nRet == UGW_SUCCESS) {
				nRet = image_size;
				LOGF_LOG_DEBUG("fapi_ssvalidateImg  passed for %s\n",fdt_get_name(addr, noffset, NULL));
			}
			else {
				LOGF_LOG_ERROR("\nfapi_ssvalidateImg  failed for %s\n",fdt_get_name(addr, noffset, NULL));
				break;
			}
		}
	} while (noffset >= 0);

	return nRet;
}
#endif

static int fapi_ssCopyImgBPtoBP(char *dev_path, char *image_type, int boardtype, int bank)
{
	FILE *pFile;
	unsigned char *buffer;
	unsigned int lSize;
	char name[MAX_PATH_LEN] = {0};
	char *part_name;
	int ret = 0;

	if ((fopen_s(&pFile, dev_path, "rb") != EOK) || !pFile) {
		LOGF_LOG_ERROR("file open failed - %s\n", strerror(errno));
		return 1;
	}

	fseek(pFile, 0, SEEK_END);
	lSize = (unsigned int)ftell (pFile);
	fseek(pFile, 0, SEEK_SET);

	buffer = (unsigned char*) malloc (lSize);

	if (buffer == NULL) {
		fputs ("Memory error",stderr);
		if (pFile != NULL)
			fclose(pFile);
		return 2;
	}

	fread (buffer,1,lSize,pFile);
#ifdef FIT_IMG
	if (strcmp(image_type, "u-boot") == 0) {
		sprintf_s(name, sizeof(name), "u-boot-%s", (bank == 0 ? INACTIVE_PART_NAME : ACTIVE_PART_NAME));
#else
	if (strcmp(image_type, "uboot") == 0) {
		sprintf_s(name, sizeof(name), "uboot_%c", (bank == 0 ? 'b': 'a'));
#endif
		part_name = getDevFromPartition(name, 'b');
		memset(name, 0, sizeof(name));
		sprintf_s(name, MAX_PATH_LEN, "/dev/%s", part_name);
	} else if (strcmp(image_type, "tep") == 0) {
#ifdef FIT_IMG
		sprintf_s(name, sizeof(name), "tep-%s", (bank == 0 ? INACTIVE_PART_NAME : ACTIVE_PART_NAME));
#else
		sprintf_s(name, sizeof(name), "tep_firmware_%c", (bank == 0 ? 'b': 'a'));
#endif
		part_name = getDevFromPartition(name, 'b');
		memset(name, 0, sizeof(name));
		sprintf_s(name, MAX_PATH_LEN, "/dev/%s", part_name);
	} else if (strcmp(image_type, "rbe") == 0) {
		if (boardtype == FLASH_TYPE_EMMC)
			sprintf_s(name, sizeof(name), "/dev/mmcblk0boot%c", (bank == 0 ? '1': '0'));
#ifndef FIT_IMG
		else {
			sprintf_s(name, sizeof(name), "rbe_%c", (bank == 0 ? 'b': 'a'));
			part_name = getDevFromPartition(name, 'b');
			memset(name, 0, sizeof(name));
			sprintf_s(name, MAX_PATH_LEN, "/dev/%s", part_name);
		}
#endif
	}
	ret = fapi_ssWriteToPartition(buffer, lSize, name);
	free(buffer);

	return ret;
}

static char *fapi_ssGetParitionNameEMMC(char *image_type, char bank)
{
	char actbnk;
	char *part_name;
	char name[MAX_PATH_LEN] = {0};
	char mnt_path[MAX_PATH_LEN] = {0};
	char cPath[MAX_PATH_LEN] = {0};
	char dev_path[MAX_PATH_LEN] = {0};
	static char file_path[MAX_PATH_LEN] = {0};
	int nRet;

	actbnk = fapi_ssCheckActiveBank();
	if ((strcmp(image_type, "kernel") == 0) || (strcmp(image_type, "rootfs") == 0))
#ifdef FIT_IMG
		sprintf_s(name, sizeof(name), "kernel-%s", (actbnk == 'A' ? ACTIVE_PART_NAME : INACTIVE_PART_NAME));
	else
		sprintf_s(name, sizeof(name), "%s-%s", image_type, (bank == 'a' ? ACTIVE_PART_NAME : INACTIVE_PART_NAME));
#else
		sprintf_s(name, sizeof(name), "extended_boot_%c", (actbnk == 'A' ? tolower('A'): tolower('B')));
	else if (strcmp(image_type, "dtb") == 0)
		sprintf_s(name, sizeof(name), "%s_%c", image_type, (actbnk == 'A' ? tolower('A'): tolower('B')));
	else if (strcmp(image_type, "tep") == 0)
		sprintf_s(name, sizeof(name), "tep_firmware_%c", bank);
	else
		sprintf_s(name, sizeof(name), "%s_%c", image_type, bank);
#endif
	part_name = getDevFromPartition(name, 'b');
	sprintf_s(dev_path, MAX_PATH_LEN, "/dev/%s", part_name);
	if ((strcmp(image_type, "kernel") == 0) || (strcmp(image_type, "rootfs") == 0)) {
		sprintf_s(mnt_path, sizeof(mnt_path), "/tmp/%s", name);
		nRet = fapi_ssMountLateboot(dev_path, mnt_path);
		if (nRet != 0)
			return NULL;
		fapi_ssGetFileNameFromUboot(image_type, cPath);
		if (cPath[0] == '\0') {
			LOGF_LOG_ERROR("Error dev_path %s not valid for kernel!\n", mnt_path);
			return NULL;
		}
		sprintf_s(dev_path, sizeof(dev_path), "%s/%s", mnt_path, cPath);
	}
	strncpy_s(file_path, MAX_PATH_LEN, dev_path, MAX_PATH_LEN);

	return file_path;
}

static void fapi_ssUnmountEMMC(void)
{
	char actbnk;
	char name[MAX_PATH_LEN] = {0};
	char mnt_path[MAX_PATH_LEN] = {0};
	int nRetValue, ret;

	actbnk = fapi_ssCheckActiveBank();
#ifdef FIT_IMG
	sprintf_s(name, sizeof(name), "kernel-%s", (actbnk == 'A' ? ACTIVE_PART_NAME : INACTIVE_PART_NAME));
#else
	sprintf_s(name, sizeof(name), "extended_boot_%c", (actbnk == 'A' ? tolower('A'): tolower('B')));
#endif
	sprintf_s(mnt_path, sizeof(mnt_path), "/tmp/%s", name);
	if (umount(mnt_path) < 0)
		perror("umount error:");
	LOGF_LOG_DEBUG("%s unmount done!\n", mnt_path);
	sprintf_s(name, sizeof(name), "rm -rf %s\n", mnt_path);
	ret = scapi_spawn(name, 1, &nRetValue);
	if (ret == UGW_SUCCESS) {
		LOGF_LOG_DEBUG("Deleted %s!\n", mnt_path);
	} else {
		perror("mount path delete error:");
	}
}

int fapi_ssImgValidateAndCommit(char *image_type, int len)
{
#ifndef IMG_AUTH
	(void)len;
#endif
	char actbnk;
#ifndef FIT_IMG
	char *part_name;
#endif
	char name[MAX_PATH_LEN] = {0};
	char *dev_path;
	int boardtype = 0, nRet = UGW_FAILURE, err, flag = 0;
	char cPath[MAX_PATH_LEN] = {0};

	boardtype = check_boardtype();
	actbnk = fapi_ssCheckActiveBank();

	if (!(actbnk == 'A' || actbnk == 'B')) {
		LOGF_LOG_ERROR("Invalid active_bank '%c'\n", actbnk);
		return UGW_FAILURE;
	}

	err = strcmp_s(image_type, 6, "kernel", &nRet);
	if ((err == EOK) && (nRet == 0)) {
		flag = 1;
		if (boardtype == FLASH_TYPE_EMMC) {
			dev_path = fapi_ssGetParitionNameEMMC(image_type, actbnk);
			if (dev_path == NULL) {
				return UGW_FAILURE;
			}
#ifdef IMG_AUTH
			nRet = fapi_ssReadFromPartition(dev_path, image_type, len);
#endif
			fapi_ssUnmountEMMC();
#ifndef FIT_IMG
		} else {
			sprintf_s(name, sizeof(name), "kernel_%c", (actbnk == 'A' ? tolower('A'): tolower('B')));
			part_name = getDevFromPartition(name, 'b');
			memset(name, 0, sizeof(name));
			sprintf_s(name, MAX_PATH_LEN, "/dev/%s", part_name);
#ifdef IMG_AUTH
			nRet = fapi_ssReadFromPartition(name, image_type, len);
#endif
#endif
		}
	}
	if (flag == 1)
		return nRet;
	err = strcmp_s(image_type, 6, "rootfs", &nRet);
	if ((err == EOK) && (nRet == 0)) {
		flag = 1;
		if (boardtype == FLASH_TYPE_EMMC) {
			dev_path = fapi_ssGetParitionNameEMMC(image_type, actbnk);
			if (dev_path == NULL) {
				return UGW_FAILURE;
			}
#ifdef IMG_AUTH
			nRet = fapi_ssReadFromPartition(dev_path, image_type, len);
#endif
			fapi_ssUnmountEMMC();
#ifndef FIT_IMG
		} else {
			sprintf_s(name, sizeof(name), "rootfs_%c", (actbnk == 'A' ? tolower('A'): tolower('B')));
			part_name = getDevFromPartition(name, 'b');
			memset(name, 0, sizeof(name));
			sprintf_s(name, MAX_PATH_LEN, "/dev/%s", part_name);
#ifdef IMG_AUTH
			nRet = fapi_ssReadFromPartition(name, image_type, len);
#endif
#endif
		}
	}
	if (flag == 1)
		return nRet;
#ifdef FIT_IMG
	err = strcmp_s(image_type, 6, "u-boot", &nRet);
#else
	err = strcmp_s(image_type, 5, "uboot", &nRet);
#endif
	if ((err == EOK) && (nRet == 0)) {
		flag = 1;
		if (boardtype == FLASH_TYPE_EMMC) {
			dev_path = fapi_ssGetParitionNameEMMC(image_type, 'a');
			if (dev_path == NULL) {
				return UGW_FAILURE;
			}
#ifdef IMG_AUTH
			nRet = fapi_ssReadFromPartition(dev_path, image_type, len);
			if (nRet == UGW_SUCCESS)
#endif
				nRet = fapi_ssCopyImgBPtoBP(dev_path, image_type, boardtype, 0);
#ifndef FIT_IMG
		} else {
			sprintf_s(name, sizeof(name), "uboot_a");
			part_name = getDevFromPartition(name, 'b');
			memset(name, 0, sizeof(name));
			sprintf_s(name, MAX_PATH_LEN, "/dev/%s", part_name);
#ifdef IMG_AUTH
			err = strcmp_s(image_type, 5, "uboot", &nRet);
			nRet = fapi_ssReadFromPartition(name, image_type, len);
			if (nRet == UGW_SUCCESS)
#endif
				nRet = fapi_ssCopyImgBPtoBP(name, image_type, boardtype, 0);
#endif
		}
		if (get_uboot_param("uboot_a_filesize", cPath) != 0) {
			fprintf(stderr, "Failed to get variable\n");
			nRet = -ENOENT;
		}
		if (set_uboot_param_str("uboot_b_filesize", cPath) != 0) {
			fprintf(stderr, "Setting the uboot_b_filesize value Failed\n");
			nRet = -EINVAL;
		}
	}

	if (flag == 1)
		return nRet;
	err = strcmp_s(image_type, 3, "tep", &nRet);
	if ((err == EOK) && (nRet == 0)) {
		flag = 1;
		if (boardtype == FLASH_TYPE_EMMC) {
			dev_path = fapi_ssGetParitionNameEMMC(image_type, 'a');
			if (dev_path == NULL) {
				return UGW_FAILURE;
			}
#ifdef IMG_AUTH
			nRet = fapi_ssReadFromPartition(dev_path, image_type, len);
			if (nRet == UGW_SUCCESS)
#endif
				nRet = fapi_ssCopyImgBPtoBP(dev_path, image_type, boardtype, 0);
#ifndef FIT_IMG
		} else {
			sprintf_s(name, sizeof(name), "tep_firmware_a");
			part_name = getDevFromPartition(name, 'b');
			memset(name, 0, sizeof(name));
			sprintf_s(name, MAX_PATH_LEN, "/dev/%s", part_name);
#ifdef IMG_AUTH
			nRet = fapi_ssReadFromPartition(name, image_type, len);
			if (nRet == UGW_SUCCESS)
#endif
				nRet = fapi_ssCopyImgBPtoBP(name, image_type, boardtype, 0);
#endif
		}
		if (get_uboot_param("tep_firmware_a_filesize", cPath) != 0) {
			fprintf(stderr, "Failed to get variable\n");
			nRet = -ENOENT;
		}
		if (set_uboot_param_str("tep_firmware_b_filesize", cPath) != 0) {
			fprintf(stderr, "Setting the tep_firmware_b_filesize value Failed\n");
			nRet = -EINVAL;
		}
	}

	if (flag == 1)
		return nRet;
	err = strcmp_s(image_type, 3, "rbe", &nRet);
	if ((err == EOK) && (nRet == 0)) {
		flag = 1;
		if (boardtype == FLASH_TYPE_EMMC) {
			sprintf_s(name, MAX_PATH_LEN, "/dev/mmcblk0boot0");
#ifndef FIT_IMG
		} else {
			sprintf_s(name, sizeof(name), "rbe_a");
			part_name = getDevFromPartition(name, 'b');
			memset(name, 0, sizeof(name));
			sprintf_s(name, MAX_PATH_LEN, "/dev/%s", part_name);
#endif
		}
#ifdef IMG_AUTH
		nRet = fapi_ssReadFromPartition(name, image_type, len);
		if (nRet == UGW_SUCCESS)
#endif
		{
			nRet = fapi_ssCopyImgBPtoBP(name, image_type, boardtype, 0);
			if (get_uboot_param("rbe_a_filesize", cPath) != 0) {
				fprintf(stderr, "Failed to get variable\n");
				nRet = -ENOENT;
			}
			if (set_uboot_param_str("rbe_b_filesize", cPath) != 0) {
				fprintf(stderr, "Setting the rbe_b_filesize value Failed\n");
				nRet = -EINVAL;
			}
		}
	}
	return nRet;
}

#ifdef IMG_AUTH
static int fapi_ssReadFromPartition(char *dev_path, char* type, long unsigned int len)
{
#ifdef FIT_IMG
	(void)len;
#endif
	FILE *pFile;
	unsigned int lSize;
	char *buffer;
	int ret = 0;
	int nSecFd = -1, boardtype;
#ifndef FIT_IMG
	image_header_t x_img_header;
#endif
	uint32_t auth_size = 0;

	if ((fopen_s(&pFile, dev_path, "rb") != EOK) || !pFile) {
		LOGF_LOG_ERROR("File open failed - %s\n", strerror(errno));
		return UGW_FAILURE;
	}
	fseek (pFile, 0, SEEK_END);
	lSize = (unsigned int)ftell (pFile);
	fseek(pFile, 0, SEEK_SET);

	/* Open the driver device */
	nSecFd = fapi_Fileopen(SEC_UPG_PATH,
			(O_RDWR & (~O_NONBLOCK)), U_RDWR_G_RD);
	if (nSecFd < 0) {
		LOGF_LOG_ERROR("Opening Secure driver file failed.\n");
		fclose(pFile);
		return ret;
	}

	buffer = (char*) malloc (lSize);
	if (buffer == NULL) {
		fputs ("Memory error",stderr);
		close(nSecFd);
		if (pFile != NULL)
			fclose(pFile);
		return UGW_FAILURE;
	}
	boardtype = check_boardtype();

	/* copy the file into the buffer */
	fread (buffer, 1, lSize, pFile);

#ifndef FIT_IMG
	x_img_header = *((image_header_t *)buffer);
#endif
	if (ntohl(*(uint32_t *)buffer) == FLATDT_MAGIC) {
		LOGF_LOG_DEBUG(" Authenticating the image with upgradeOrCommit value 1\n");
		ret = fapi_ssValidateDTB(nSecFd, buffer, 1);
		close(nSecFd);
		free(buffer);
		fclose(pFile);
		if (ret < 0)
			return ret;
		return UGW_SUCCESS;
	}

	if (strcmp(type,"rbe") == 0) {
		if (ret == 0) {
			if (boardtype == FLASH_TYPE_EMMC) {
				memcpy_s(&auth_size, 4, buffer + EMMC_PAGE_SIZE + 12 - BLW_LENGTH , 4);
				ret = fapi_ssvalidateImg(nSecFd, buffer + EMMC_PAGE_SIZE +12 , auth_size - 4, 1);
			}
#ifndef FIT_IMG
			else {
				memcpy_s(&auth_size, 4, buffer + NAND_PAGE_SIZE + 12 - BLW_LENGTH , 4);
				ret = fapi_ssvalidateImg(nSecFd, buffer + NAND_PAGE_SIZE +12 , auth_size - 4, 1);
			}
		}
	} else if (strcmp(type,"kernel") == 0) {
		if (boardtype == FLASH_TYPE_EMMC)
			ret = fapi_ssvalidateImg(nSecFd, buffer + sizeof(image_header_t), ntohl(x_img_header.img_hdr_size), 1);
		else
			ret = fapi_ssvalidateImg(nSecFd, buffer + sizeof(image_header_t), ntohl(x_img_header.img_hdr_size) - sizeof(image_header_t), 1);
	} else if (strcmp(type,"rootfs") == 0) {
		if (ret == 0) {
			if (boardtype == FLASH_TYPE_EMMC)
				ret = fapi_ssvalidateImg(nSecFd, buffer, len - BLW_LENGTH, 1);
			else
				ret = fapi_ssvalidateImg(nSecFd, buffer , len + sizeof(image_header_t) - BLW_LENGTH, 1);
		}
	} else if (strcmp(type,"tep_firmware") == 0) {
		if (ret == 0) {
			ret = fapi_ssvalidateImg(nSecFd, buffer , len - BLW_LENGTH , 1);
		}
	} else if (strcmp(type,"uboot") == 0) {
		if (ret == 0) {
			ret = fapi_ssvalidateImg(nSecFd, buffer + sizeof(image_header_t), ntohl(x_img_header.img_hdr_size) - BLW_LENGTH, 1);
		}
	} else {
		if (ret == 0)
			ret = fapi_ssvalidateImg(nSecFd, buffer , len - BLW_LENGTH, 1);
#else
		}
#endif
	}

	close(nSecFd);
	free(buffer);
	fclose(pFile);
	if (ret != UGW_SUCCESS)
			return UGW_FAILURE;
	LOGF_LOG_DEBUG(" Image Successfully commited.\n");
	return ret;
}
#endif
static int fapi_ssWriteToPartition(const unsigned char *image_src_address, int image_len, char *dev_path)
{
	FILE *fp1 = NULL;
	int flag = 0;

	if (strncmp(dev_path, "/dev/mmcblk0boot0", MAX_PATH_LEN) == 0)
		system("/bin/echo 0 > /sys/block/mmcblk0boot0/force_ro");
	else if	(strncmp(dev_path, "/dev/mmcblk0boot1", MAX_PATH_LEN) == 0)
		system("/bin/echo 0 > /sys/block/mmcblk0boot1/force_ro");

	if (fopen_s(&fp1, dev_path, "w") != EOK) {
		LOGF_LOG_ERROR("Error %s!\n", strerror(errno));
		return UGW_FAILURE;
	}
	if (fp1 == NULL) {
		LOGF_LOG_ERROR("File pointer is NULL!\n");
		return UGW_FAILURE;
	}

	flag = fwrite(image_src_address, image_len, 1, fp1);
	if (flag) {
		LOGF_LOG_DEBUG("Contents of the structure written successfully\n");
	} else {
		LOGF_LOG_ERROR("Error Writing to File!\n");
	}

	fclose(fp1);
	if (strncmp(dev_path, "/dev/mmcblk0boot0", MAX_PATH_LEN) == 0)
		system("/bin/echo 1 > /sys/block/mmcblk0boot0/force_ro");
	else if	(strncmp(dev_path, "/dev/mmcblk0boot1", MAX_PATH_LEN) == 0)
		system("/bin/echo 1 > /sys/block/mmcblk0boot1/force_ro");
	return UGW_SUCCESS;
}

static int fapi_ssMountLateboot(char *mnt_part, char *mnt_path)
{
	char param[MAX_PATH_LEN] = {0};
	int nRetValue, ret;

	LOGF_LOG_DEBUG("mnt_part: %s, mnt_path: %s\n", mnt_part, mnt_path);
	sprintf_s(param, sizeof(param), "mkdir -p %s\n", mnt_path);
	ret = scapi_spawn(param, 1, &nRetValue);

	if (ret == UGW_SUCCESS) {
		sprintf_s(param, sizeof(param), "mount -t ext4 %s %s/\n", mnt_part, mnt_path);
#ifdef FIT_IMG
		ret = scapi_spawn(param, 1, &nRetValue);
		if (nRetValue != UGW_SUCCESS){
			sprintf_s(param, sizeof(param), "resize2fs %s && mount -t ext4 %s %s/\n", mnt_part, mnt_part, mnt_path);
			printf("\ntrying resize2fs and mount again : %s\n",param);
			return scapi_spawn(param, 1, &nRetValue);
		}
#else
		return scapi_spawn(param, 1, &nRetValue);
#endif
	}
	return ret;
}

static char fapi_ssCheckActiveBank(void)
{
	FILE *output = NULL;
	char sActBnk;

	output = popen("uboot_env --get --name active_bank", "r");
	if (output == NULL) {
		LOGF_LOG_ERROR("Error launching the cmd to get active_bank the uboot environment\n");
		return UGW_SUCCESS;
	}
	if (fread(&sActBnk, 1, sizeof(sActBnk), output) > 0) {
		pclose(output);
		return sActBnk;
	} else {
		LOGF_LOG_ERROR("variable not found in uboot\n");
		pclose(output);
		return UGW_SUCCESS;
	}
}

static void fapi_ssGetFileNameFromUboot(char *file_name, char *cPath)
{
	FILE *output;
	char name[MAX_PATH_LEN] = {0};
	char cmd[MAX_PATH_LEN] = {0};
	int len;

	if (strncmp(file_name, "kernel", sizeof("kernel")) == 0 || strcmp(file_name, "kernel-dtb") == 0)
		sprintf_s(cmd, sizeof(cmd), "uboot_env --get --name bootfile");
	else if (strncmp(file_name, "filesystem", sizeof("filesystem")) == 0)
		sprintf_s(cmd, sizeof(cmd), "uboot_env --get --name rootfs");
	else
		sprintf_s(cmd, sizeof(cmd), "uboot_env --get --name %s", file_name);

	output = popen(cmd, "r");
	if (output == NULL) {
		LOGF_LOG_ERROR("Error launching the cmd to get active_bank the uboot environment\n");
		return;
	}
	len = fread(&name, sizeof(char), MAX_PATH_LEN - 1, output);
	if (len > 0) {
		pclose(output);
		strncpy_s(cPath, MAX_PATH_LEN, name, MAX_PATH_LEN);
		while (len && (cPath[len - 1] == '\n' || cPath[len - 1] == '\r'))
			cPath[--len] = 0;
	} else {
		LOGF_LOG_ERROR("variable not found in uboot\n");
		pclose(output);
	}
}

static int parse_config(struct map_table *map, char *name)
{
	char line[256];
	char part1[4][256];
	int index = 0, ret = 1;
	char var[256] = "name: ";
	FILE *file = fopen(CONFIG_FILE, "r");
	if (file == NULL) {
		perror("Error opening file");
		return 1;
	}
	strcat(var, name);
	while (fgets(line, sizeof(line), file)) {
		strcpy(part1[index], line);
		index++;
		if (index == 4) {
			if (strstr(part1[1], var)) {
				ret = 0;
				for (int i = 1; i < 4; i++) {
					char *value = strchr(part1[i], ':');

					if (value != NULL) {
						value++;
						while (*value == ' ') value++;
						value[strcspn(value, "\n")] = '\0';
						if (i == 1)
							strcpy(map->name, value);
						else if(i == 2)
							strcpy(map->part, value);
						else
							map->early_boot = strcmp(value, "Yes") ? false : true;
					}
				}
				goto finish;
			}
			index = 0;
		}
	}
finish:
	fclose(file);
	return ret;
}

static int fapi_ssCheckFileSize(img_param_t image_auth)
{
	int  nRet=UGW_FAILURE;
	char actbnk;
	char name[MAX_PATH_LEN] = {0};
	char dev_path[MAX_PATH_LEN] = {0};
	char *part_name = NULL;
	int boardtype;
	FILE *fp1 = NULL;
	unsigned int lSize;
	int noffset, depth = 0;
	int len = 0;
	struct map_table map;
#ifdef FIT_IMG
	const char *cfg_name = NULL;
	char cfg_name_buf[MAX_PATH_LEN] = {0};
#endif

	actbnk = fapi_ssCheckActiveBank();
	if (!(actbnk == 'A' || actbnk == 'B')) {
		LOGF_LOG_ERROR("Invalid active_bank '%c'\n", actbnk);
		nRet = UGW_FAILURE;
		goto finish;
	}

	boardtype = check_boardtype();
	if (boardtype < 0) {
		LOGF_LOG_ERROR("invalid board type\n");
		nRet = UGW_FAILURE;
		goto finish;
	}
#ifdef FIT_IMG
	cfg_name = fapi_ssNormalizeFitNodeName(image_auth.img_name);
	if (!cfg_name) {
		LOGF_LOG_ERROR("Invalid image name\n");
		nRet = UGW_FAILURE;
		goto finish;
	}
	strncpy_s(cfg_name_buf, sizeof(cfg_name_buf), cfg_name, strnlen(cfg_name, MAX_PATH_LEN));
	nRet = parse_config(&map, cfg_name_buf);
#else
	nRet = parse_config(&map, image_auth.img_name);
#endif
	if (nRet == 1) {
		LOGF_LOG_ERROR("Failed to parse config for %s\n", image_auth.img_name);
		nRet = UGW_FAILURE;
		goto finish;
	}

		memset(dev_path, 0, sizeof(dev_path));
		if (map.early_boot == true) {
			/* Early boot component always written in primary bank */
			if ((strncmp(map.name, "rbe", sizeof(image_auth.img_name)) == 0) && (boardtype == FLASH_TYPE_EMMC))
				sprintf_s(dev_path, sizeof(dev_path), "/dev/mmcblk0boot0");
			else
#ifdef FIT_IMG
				sprintf_s(name, sizeof(name), "%s-%s", map.part, ACTIVE_PART_NAME);
#else
				sprintf_s(name, sizeof(name), "%s_a", map.part);
#endif
		} else {
			/* Late boot component always written in non-active bank */
			if (boardtype == FLASH_TYPE_EMMC) {
#ifdef FIT_IMG
				sprintf_s(name, sizeof(name), "%s-%s", map.part, (actbnk == 'A' ? INACTIVE_PART_NAME : ACTIVE_PART_NAME));
#else
				sprintf_s(name, sizeof(name), "%s_%c", map.part, (actbnk == 'A' ? tolower('B'): tolower('A')));
			} else {
				/* for NAND model partition name like roofs_*, kernel_* and dtb_* */
				if (strncmp(image_auth.img_name, "rootfs", sizeof(image_auth.img_name)) == 0)
					sprintf_s(name, sizeof(name), "rootfs_%c", (actbnk == 'A' ? tolower('B'): tolower('A')));
				else if (strncmp(image_auth.img_name, "kernel", sizeof(image_auth.img_name)) == 0)
					sprintf_s(name, sizeof(name), "kernel_%c", (actbnk == 'A' ? tolower('B'): tolower('A')));
				else
					sprintf_s(name, sizeof(name), "%s_%c", map.part, (actbnk == 'A' ? tolower('B'): tolower('A')));
#endif
			}
		}
		if (dev_path[0] == '\0') {
			part_name = getDevFromPartition(name, 'b');
			sprintf_s(dev_path, sizeof(dev_path), "/dev/%s", part_name);
		}
		if ((fopen_s(&fp1, dev_path, "rb") != EOK) || !fp1) {
			LOGF_LOG_ERROR("File error - %s\n", strerror(errno));
			nRet = UGW_FAILURE;
			goto finish;
		}
		fseek (fp1, 0, SEEK_END);
		lSize = (unsigned int)ftell (fp1);
		fseek(fp1, 0, SEEK_SET);
		fclose(fp1);
#ifdef FIT_IMG
		if ((strncmp(map.name, "kernel-dtb", sizeof(map.name)) == 0) ||
			(strncmp(map.name, "uboot", sizeof(map.name)) == 0) ||
			(strncmp(map.name, "tep", sizeof(map.name)) == 0) ||
			(strncmp(map.name, "filesystem", sizeof(map.name)) == 0)) {
			image_auth.src_img_len = fdt_totalsize(image_auth.src_img_addr);
			if (image_auth.src_img_len > lSize) {
				LOGF_LOG_ERROR("Given %s image length(%lu) is greater than partition lenth(%u)!\n", map.name, image_auth.src_img_len, lSize);
				nRet = UGW_FAILURE;
				goto finish;
			} else {
				LOGF_LOG_DEBUG("Given %s image size (%lu) is less than partition size %u!\n", map.name, image_auth.src_img_len, lSize);
				return image_auth.src_img_len;
			}
		} else
#endif			
		if (strncmp(map.name, "dtb", sizeof(map.name)) != 0) {
			if ((unsigned int)image_auth.src_img_len > lSize) {
				LOGF_LOG_ERROR("Given %s image length(%lu) is greater than partition lenth(%u)!\n", map.name, image_auth.src_img_len, lSize);
				nRet = UGW_FAILURE;
				goto finish;
			} else {
				LOGF_LOG_DEBUG("Given %s image size is less than partition!\n", map.name);
				nRet = UGW_SUCCESS;
			}
 		} else {
			noffset = fdt_path_offset(image_auth.src_img_addr, FIT_IMAGES_PATH);
			if (noffset < 0) {
				fprintf(stderr,"Unable to find dtb within fit image\n");
				return UGW_FAILURE;
			}

			do {
				noffset = fdt_next_node(image_auth.src_img_addr, noffset, &depth);
				LOGF_LOG_DEBUG("noffset %d depth %d name %s\n", noffset, depth,
					fdt_get_name(image_auth.src_img_addr, noffset, NULL));
				if (depth == 1)
					break;
			} while (noffset >= 0);

			if (noffset < 0) {
				LOGF_LOG_ERROR("Unable to find dtb within fit image\n");
				return UGW_FAILURE;
			}

			(void)fdt_getprop(image_auth.src_img_addr, noffset, FIT_DATA_PROP, &len);
			if (!len) {
				LOGF_LOG_ERROR("Unable to find dtb data\n");
				return UGW_FAILURE;
			}
			if ((unsigned int)len > lSize) {
				LOGF_LOG_ERROR("Given %s image length(%lu) is greater than partition lenth(%u)!\n", map.name, image_auth.src_img_len, lSize);
				nRet = UGW_FAILURE;
				goto finish;
			} else {
				LOGF_LOG_DEBUG("Given %s image size (%d) is less than partition size %u!\n", map.name, len, lSize);
				return fdt_totalsize(image_auth.src_img_addr);
			}
		}
finish:
	return nRet;
}

#ifndef FIT_IMG
static int fapi_ssWriteToDtbPartition(img_param_t image_auth, char *name)
{
	const void *data;
	int noffset, depth = 0;
	int len = 0, ret = 0;

	noffset = fdt_path_offset(image_auth.src_img_addr, FIT_IMAGES_PATH);
	do {
		noffset = fdt_next_node(image_auth.src_img_addr, noffset, &depth);
		LOGF_LOG_DEBUG("noffset %d depth %d name %s\n", noffset, depth,
				fdt_get_name(image_auth.src_img_addr, noffset, NULL));
		if (depth == 1)
			break;
	} while (noffset >= 0);

	if (noffset < 0) {
		LOGF_LOG_ERROR("Unable to find dtb within fit image\n");
		return fdt_totalsize(image_auth.src_img_addr);
	}

	data = fdt_getprop(image_auth.src_img_addr, noffset, FIT_DATA_PROP, &len);
	if (!len) {
		LOGF_LOG_ERROR("Unable to find dtb data\n");
		return fdt_totalsize(image_auth.src_img_addr);
	}
	LOGF_LOG_DEBUG("dtb image dev_path: %s\n", name);
	ret = fapi_ssWriteToPartition(data, len, name);
	if (ret == UGW_SUCCESS)
		ret = fdt_totalsize(image_auth.src_img_addr);

	return ret;
}
#endif
int fapi_ssImgUpgrade(img_param_t image_auth)
{
	int nRet = UGW_FAILURE;
	char actbnk;
	char name[MAX_PATH_LEN] = {0};
	char dev_path[MAX_PATH_LEN] = {0};
	char mnt_path[MAX_PATH_LEN] = {0};
	char *part_name = NULL;
	char cPath[MAX_PATH_LEN] = {0};
	int boardtype;
	int nLockFd = -1, nRetValue;
	struct map_table map;
	char rootfs_size[MAX_PATH_LEN] = {0};
#ifdef FIT_IMG
	const char *cfg_name = NULL;
	char cfg_name_buf[MAX_PATH_LEN] = {0};
#endif

	nLockFd = fapi_Fileopen(LOCK_FILE, O_RDONLY, 0);
	if (nLockFd < 0) {
		LOGF_LOG_ERROR("LOCK FILE open failed [%s]\n", strerror(errno));
		return UGW_FAILURE;
	}

	if (flock(nLockFd, LOCK_EX) < 0) {
		LOGF_LOG_ERROR("flock failed with reason [%s]\n", strerror(errno));
		if (close(nLockFd) < 0)
			LOGF_LOG_DEBUG("close failed with reason [%s]\n", strerror(errno));
		return UGW_FAILURE;
	}

	if (image_auth.src_img_fd < 0) {
		LOGF_LOG_ERROR("image_auth.src_img_fd is less than 0\n");
		nRet = ERR_BAD_FD;
		goto finish;
	}
	actbnk = fapi_ssCheckActiveBank();
	if (!(actbnk == 'A' || actbnk == 'B')) {
		LOGF_LOG_ERROR("Invalid active_bank '%c'\n", actbnk);
		nRet = UGW_FAILURE;
		goto finish;
	}
#ifdef FIT_IMG
	cfg_name = fapi_ssNormalizeFitNodeName(image_auth.img_name);
	if (!cfg_name) {
		LOGF_LOG_ERROR("Invalid image name\n");
		nRet = UGW_FAILURE;
		goto finish;
	}
	strncpy_s(cfg_name_buf, sizeof(cfg_name_buf), cfg_name, strnlen(cfg_name, MAX_PATH_LEN));
	nRet = parse_config(&map, cfg_name_buf);
#else
	nRet = parse_config(&map, image_auth.img_name);
#endif
	if (nRet == 1) {
		LOGF_LOG_ERROR("image_auth.img_name %s map.name %s not matched \n", image_auth.img_name, map.name);
		nRet = UGW_FAILURE;
		goto finish;
	}

		boardtype = check_boardtype();
		memset(dev_path, 0, sizeof(dev_path));
		if (map.early_boot == true) {
			/* Early boot component always written in primary bank */
			if ((strncmp(map.name, "rbe", sizeof(image_auth.img_name)) == 0) && (boardtype == FLASH_TYPE_EMMC)) {
				sprintf_s(dev_path, sizeof(dev_path), "/dev/mmcblk0boot0");
				if (add_uboot_param("rbe_a_filesize", 0) != 0) {
					fprintf(stderr, "Setting the rbe_a_filesize value Failed\n");
					nRet = -EINVAL;
					goto finish;
				}
			} else {
#ifdef FIT_IMG
				sprintf_s(name, sizeof(name), "%s-%s", map.part,ACTIVE_PART_NAME);
#else
				sprintf_s(name, sizeof(name), "%s_a", map.part);
#endif
#ifdef FIT_IMG
				if (strncmp(map.name, "tep", sizeof(map.name)) == 0) {
#else
				if (strncmp(map.name, "firmware", sizeof(map.name)) == 0) {
#endif
					if (add_uboot_param("tep_firmware_a_filesize", 0) != 0) {
						fprintf(stderr, "Setting the tep_firmware_a_filesize value Failed\n");
						nRet = -EINVAL;
						goto finish;
					}
				}
				if (strncmp(map.name, "uboot", sizeof(map.name)) == 0) {
					if (add_uboot_param("uboot_a_filesize", 0) != 0) {
						fprintf(stderr, "Setting the uboot_a_filesize value Failed\n");
						nRet = -EINVAL;
						goto finish;
					}
				}
			}
		} else {
			/* Late boot component always written in non-active bank */
			if (boardtype == FLASH_TYPE_EMMC) {
#ifdef FIT_IMG
				sprintf_s(name, sizeof(name), "%s-%s", map.part, (actbnk == 'A' ? INACTIVE_PART_NAME : ACTIVE_PART_NAME));
#else
				sprintf_s(name, sizeof(name), "%s_%c", map.part, (actbnk == 'A' ? tolower('B'): tolower('A')));
			} else {
				/* for NAND model partition name like roofs_*, kernel_* and dtb_* */
				if (strncmp(image_auth.img_name, "rootfs", sizeof(image_auth.img_name)) == 0)
					sprintf_s(name, sizeof(name), "rootfs_%c", (actbnk == 'A' ? tolower('B'): tolower('A')));
				else if (strncmp(image_auth.img_name, "kernel", sizeof(image_auth.img_name)) == 0)
					sprintf_s(name, sizeof(name), "kernel_%c", (actbnk == 'A' ? tolower('B'): tolower('A')));
				else
					sprintf_s(name, sizeof(name), "%s_%c", map.part, (actbnk == 'A' ? tolower('B'): tolower('A')));
#endif
			}
		}
		if (dev_path[0] == '\0') {
			part_name = getDevFromPartition(name, 'b');
			sprintf_s(dev_path, sizeof(dev_path), "/dev/%s", part_name);
		}
		/* For EMMC rootfs and kernel mount is needed */
		if (boardtype == FLASH_TYPE_EMMC) {
#ifdef FIT_IMG
			if ((strncmp(map.name, "filesystem", sizeof(map.name)) == 0) ||
				(strncmp(map.name, "kernel-dtb", sizeof(map.name)) == 0)) {
#else
			if ((strncmp(map.name, "rootfs", sizeof(map.name)) == 0) ||
				(strncmp(map.name, "kernel", sizeof(map.name)) == 0)) {
#endif
				sprintf_s(mnt_path, sizeof(mnt_path), "/tmp/%s", name);
				fprintf(stdout,"mnt_path %s dev_path %s\n", mnt_path, dev_path);
				nRet = fapi_ssMountLateboot(dev_path, mnt_path);
				if (nRet != UGW_SUCCESS) {
					LOGF_LOG_DEBUG("already mounted\n");
				} else {
					LOGF_LOG_DEBUG("mount success\n");
				}
				memset(dev_path, 0, sizeof(dev_path));
				fapi_ssGetFileNameFromUboot(map.name, cPath);
				if (cPath[0] == '\0') {
					LOGF_LOG_ERROR("Error dev_path %s not valid for kernel!\n", mnt_path);
					nRet = UGW_FAILURE;
					goto finish;
				}
				sprintf_s(dev_path, sizeof(dev_path), "%s/%s", mnt_path, cPath);
			}
		}
#ifndef FIT_IMG
		if (strncmp(map.name, "dtb", sizeof(map.name)) == 0) {
			nRet = fapi_ssWriteToDtbPartition(image_auth, dev_path);
			goto finish;
		} else
#endif
		{
			LOGF_LOG_DEBUG("image is :%s and dev_path: %s\n", map.name, dev_path);
			nRet = fapi_ssWriteToPartition(image_auth.src_img_addr, image_auth.src_img_len, dev_path);
			snprintf(rootfs_size, sizeof(rootfs_size), "%08lx", (unsigned long)image_auth.src_img_len);
			printf("## image_auth.src_img_len %lu fdttotal_size %u rootfs_size %s\n", image_auth.src_img_len,fdt_totalsize(image_auth.src_img_addr),rootfs_size);
#ifdef FIT_IMG
			if (strncmp(map.name, "tep", sizeof(map.name)) == 0) {
#else
			if (strncmp(map.name, "firmware", sizeof(map.name)) == 0) {
#endif
				if (set_uboot_param_str("tep_firmware_a_filesize", rootfs_size) != 0) {
					fprintf(stderr, "Setting the tep_firmware_a_filesize value Failed\n");
					nRet = -EINVAL;
					goto finish;
				}
			}
			if (strncmp(map.name, "uboot", sizeof(map.name)) == 0) {
				if (set_uboot_param_str("uboot_a_filesize", rootfs_size) != 0) {
					fprintf(stderr, "Setting the uboot_a_filesize value Failed\n");
					nRet = -EINVAL;
					goto finish;
				}
			}
			if (strncmp(map.name, "rbe", sizeof(map.name)) == 0) {
				if (set_uboot_param_str("rbe_a_filesize", rootfs_size) != 0) {
					fprintf(stderr, "Setting the rbe_a_filesize value Failed\n");
					nRet = -EINVAL;
					goto finish;
				}
			}
			if (boardtype == 1) {
#ifdef FIT_IMG
			if ((strncmp(map.name, "filesystem", sizeof(map.name)) == 0) ||
				(strncmp(map.name, "kernel-dtb", sizeof(map.name)) == 0)) {
#else
				if ((strncmp(map.name, "rootfs", sizeof(map.name)) == 0) ||
					(strncmp(map.name, "kernel", sizeof(map.name)) == 0)) {
#endif
						if (actbnk == 'A') {
#ifdef FIT_IMG
							if (strncmp(map.name, "filesystem", sizeof(map.name)) == 0) {
#else
							if (strncmp(map.name, "rootfs", sizeof(map.name)) == 0) {
#endif
								if (set_uboot_param_str("rootfs_b_filesize", rootfs_size) != 0) {
									fprintf(stderr, "Setting the rootfs_b_filesize value Failed\n");
									nRet = -EINVAL;
									goto finish;
								}
							} else {
								if (set_uboot_param_str("kernel_b_filesize", rootfs_size) != 0) {
									fprintf(stderr, "Setting the kernel_b_filesize value Failed\n");
									nRet = -EINVAL;
									goto finish;
								}
							}
						} else if (actbnk == 'B') {
#ifdef FIT_IMG
							if (strncmp(map.name, "filesystem", sizeof(map.name)) == 0) {
#else
							if (strncmp(map.name, "rootfs", sizeof(map.name)) == 0) {
#endif
								if (set_uboot_param_str("rootfs_a_filesize", rootfs_size) != 0) {
									fprintf(stderr, "Setting the rootfs_a_filesize value Failed\n");
									nRet = -EINVAL;
									goto finish;
								}
							} else {
								if (set_uboot_param_str("kernel_a_filesize", rootfs_size) != 0) {
									fprintf(stderr, "Setting the kernel_a_filesize value Failed\n");
									nRet = -EINVAL;
									goto finish;
								}
							}
						}
					if (umount(mnt_path) < 0) {
						perror("umount error:");
					} else {
						sprintf_s(name, sizeof(name), "rm -rf %s\n", mnt_path);
						nRet = scapi_spawn(name, 1, &nRetValue);
						if (nRet == UGW_SUCCESS) {
							LOGF_LOG_DEBUG("Deleted %s!\n", mnt_path);
						} else {
							perror("mount path delete error:");
						}
					}
				}
			}
			goto finish;
		}
finish:
	if (flock(nLockFd, LOCK_UN) < 0)
		LOGF_LOG_DEBUG("unlock failed with reason [%s]\n",
		strerror(errno));

	if (close(nLockFd) < 0)
		LOGF_LOG_DEBUG("close failed with reason [%s]\n",
		strerror(errno));

	return nRet;
}

/**=====================================================================
 * @brief  Securely validate the image.
 *
 * @param image_auth
 * structure to validate the image
 *
 * @return
 *  UGW_SUCCESS on success
 *  UGW_FAILURE on failure
 =======================================================================
 */

int fapi_ssImgAuth(img_param_t image_auth)
{
	int nRet = UGW_SUCCESS;
	int nLockFd = -1;

	nLockFd = fapi_Fileopen(LOCK_FILE, O_RDONLY, 0);
	if (nLockFd < 0) {
		LOGF_LOG_ERROR("LOCK FILE open failed [%s]\n", strerror(errno));
		return UGW_FAILURE;
	}

	if (flock(nLockFd, LOCK_EX) < 0) {
		LOGF_LOG_ERROR("flock failed with reason [%s]\n", strerror(errno));
		if (close(nLockFd) < 0)
			LOGF_LOG_DEBUG("close failed with reason [%s]\n", strerror(errno));
		return UGW_FAILURE;
	}

	if (image_auth.src_img_fd < 0) {
		LOGF_LOG_ERROR("image_auth.src_img_fd is less than 0\n");
		nRet = ERR_BAD_FD;
		goto finish;
	}

#ifdef IMG_AUTH
	nRet = fapi_ssImgValidate(&image_auth, 0);
#endif
	if (nRet == UGW_SUCCESS) {
		nRet = fapi_ssCheckFileSize(image_auth);
		if (nRet < 0) {
			LOGF_LOG_ERROR("Image validation failed\n");
			goto finish;
		}
	}

finish:
	if (flock(nLockFd, LOCK_UN) < 0)
		LOGF_LOG_DEBUG("unlock failed with reason [%s]\n",
		strerror(errno));

	if (close(nLockFd) < 0)
		LOGF_LOG_DEBUG("close failed with reason [%s]\n",
		strerror(errno));

	return nRet;
}

int fapi_ssGetActiveBank(unsigned char **pcState)
{
	unsigned char *env_data = NULL;

	readenv();

	env_data = get_env("active_bank");
	if (env_data == NULL)
		return UBOOT_VARIABLE_NOT_EXIST;

	*pcState = env_data;

	return UGW_SUCCESS;
}

int fapi_ssSetCommitBank(unsigned char *value)
{
	int ret = 0;

	readenv();

	if (value != NULL) {
		ret = set_env("commit_bank", (char *)value);
		if (ret != UGW_SUCCESS)
			return UBOOT_SET_OPERATION_FAIL;
		saveenv();
	}

	return UGW_SUCCESS;
}

int fapi_ssGetCommitBank(unsigned char **pcState)
{
	unsigned char *env_data = NULL;

	readenv();

	env_data = get_env("commit_bank");
	if (!env_data)
		return UBOOT_VARIABLE_NOT_EXIST;

	*pcState = env_data;

	return UGW_SUCCESS;
}

int fapi_ssActivateBank(unsigned char *pvalue)
{
	int ret = 0;

	readenv();

	if (!pvalue)
		return UBOOT_VALUE_ERROR;

	if (pvalue != NULL) {
		ret = set_env("img_activate", (char *)pvalue);
		if (ret != UGW_SUCCESS)
			return UBOOT_SET_OPERATION_FAIL;
		saveenv();
	}

	return UGW_SUCCESS;
}

int fapi_ssSetUdt(unsigned char *pvalue, uint32_t ivalue)
{
	int ret = 0;
	char value[255];

	readenv();

	if (!pvalue)
		return UBOOT_VALUE_ERROR;

	if (pvalue != NULL) {
		ret = set_env("udt_file", (char *)pvalue);

		if (ret != UGW_SUCCESS)
			return UBOOT_SET_OPERATION_FAIL;

		snprintf(value, sizeof(value), "%d", ivalue);
		ret = set_env("udt_status", (char *)value);

		if (ret != UGW_SUCCESS)
			return UBOOT_SET_OPERATION_FAIL;

		saveenv();
	}
	return UGW_SUCCESS;
}

static int add_uboot_param(const char *uboot_var, const int value)
{
	FILE *fp = NULL;
	char cmd[MAX_PATH_LEN] = {0};

	sprintf_s(cmd, sizeof(cmd), "/usr/sbin/uboot_env --get --name %s", uboot_var);
	if (system(cmd) == 0)
		return 0;
	memset(cmd, 0, sizeof(cmd));
	sprintf_s(cmd, sizeof(cmd), "/usr/sbin/uboot_env --add --name %s --value %d", uboot_var, value);
	fp = popen(cmd, "w");
	if (fp == NULL) {
		fprintf(stderr, "Error launching the cmd to update the uboot environment\n");
		return -ENOENT;
	}
	if (pclose(fp) < 0 ) {
		fprintf(stderr, "Error setting the environement variable\n");
		return -ENOENT;
	}
	return 0;
}

static int set_uboot_param_int(const char *var_name, const int value)
{
	FILE *fp = NULL;
	char cmd[MAX_PATH_LEN] = {0};

	sprintf_s(cmd, sizeof(cmd), "/usr/sbin/uboot_env --set --name %s --value %d", var_name, value);
	fp = popen(cmd, "w");
	if (fp == NULL) {
		fprintf(stderr, "Error launching the cmd to update the uboot environment\n");
		return -ENOENT;
	}
	if (pclose(fp) < 0 ) {
		fprintf(stderr, "Error setting the environement variable\n");
		return -ENOENT;
	}
	return 0;
}

static int set_uboot_param_str(const char *var_name, const char* value)
{
	FILE *fp = NULL;
	char cmd[MAX_PATH_LEN] = {0};
	sprintf_s(cmd, sizeof(cmd), "/usr/sbin/uboot_env --set --name %s --value %s", var_name, value);
	fp = popen(cmd, "w");
	if (fp == NULL) {
		fprintf(stderr, "Error launching the cmd to update the uboot environment\n");
		return -ENOENT;
	}
	if (pclose(fp) < 0 ) {
		fprintf(stderr, "Error setting the environement variable\n");
		return -ENOENT;
	}
	return 0;
}

static int get_uboot_param(char *var_name, char *value)
{
	FILE *output = NULL;
	char name[MAX_PATH_LEN] = {0};
	char cmd[MAX_PATH_LEN] = {0};

	sprintf_s(cmd, sizeof(cmd), "/usr/sbin/uboot_env --get --name %s", var_name);
	output = popen(cmd, "r");
	if (output == NULL) {
		fprintf(stderr, "Error launching the cmd to get %s the uboot environment\n", var_name);
		return -ENOENT;
	}
	if (fread(name, 1, MAX_PATH_LEN, output) > 0) {
		strncpy_s(value, MAX_PATH_LEN, name, MAX_PATH_LEN);
		if (value[strnlen_s(name, MAX_PATH_LEN) - 1] == '\n')
			value[strnlen_s(name, MAX_PATH_LEN) - 1] = 0;
	} else {
		fprintf(stderr, "%s variable not found in uboot\n", var_name);
		return -ENOENT;
	}

	pclose(output);
	return 0;
}

static int img_validate_and_commit(void)
{
	int i, ret = 0, val;
	char cPath[MAX_PATH_LEN] = {0};

	if (get_uboot_param("upgrade_image", cPath) != 0) {
		fprintf(stderr, "Failed to get variable\n");
		ret = -ENOENT;
		goto finish;
	}
	val = atoi(cPath);
	for (i = 0; i < ARRAY_SIZE(image_list); i++) {
		if (image_list[i] != val) {
			ret = -EINVAL;
			continue;
		} else {
#ifdef FIT_IMG
			if (val & BOOTLOADERFIT) {
#else
			if (val & BOOTLOADER) {
#endif
				/* UBOOT */
				if (add_uboot_param("uboot_b_filesize", 0) != 0) {
					fprintf(stderr, "Setting the uboot_b_filesize value Failed\n");
					ret = -EINVAL;
					goto finish;
				}
#ifdef FIT_IMG
				ret = fapi_ssImgValidateAndCommit("u-boot", 0);
#else
				ret = fapi_ssImgValidateAndCommit("uboot", 0);
#endif
				if (ret != 0) {
					ret = -EPERM;
					goto finish;
				}
			}
			if (val & RBE) {
				/* RBE */
				memset(cPath, 0, sizeof(cPath));
				if (add_uboot_param("rbe_b_filesize", 0) != 0) {
					fprintf(stderr, "Setting the rbe_b_filesize value Failed\n");
					ret = -EINVAL;
					goto finish;
				}
				if (get_uboot_param("rbe_size", cPath) != 0) {
					fprintf(stderr, "Failed to get variable\n");
					ret = -ENOENT;
					goto finish;
				}
				ret = fapi_ssImgValidateAndCommit("rbe", atoi(cPath));
				if (ret != 0) {
					ret = -EPERM;
					goto finish;
				}
			}
#ifdef FIT_IMG
			if (val & TEPFIT) {
				memset(cPath, 0, sizeof(cPath));
#else
			if (val & TEP) {
				/* TEP Firmware */
				memset(cPath, 0, sizeof(cPath));
				if (get_uboot_param("tep_size", cPath) != 0) {
					fprintf(stderr, "Failed to get variable\n");
					ret = -ENOENT;
					goto finish;
				}
#endif
				if (add_uboot_param("tep_firmware_b_filesize", 0) != 0) {
					fprintf(stderr, "Setting the tep_firmware_b_filesize value Failed\n");
					ret = -EINVAL;
					goto finish;
				}
				ret = fapi_ssImgValidateAndCommit("tep", atoi(cPath));
				if (ret != 0) {
					ret = -EPERM;
					goto finish;
				}
			}
#ifdef FIT_IMG
			if (val & KERNELDTBFIT) {
#else
			if (val & KERNEL) {
#endif
				/* kernel */
#ifdef IMG_AUTH
				ret = fapi_ssImgValidateAndCommit("kernel", 0);
				if (ret != 0) {
					ret = -EPERM;
					goto finish;
				}
#else
				ret = 0;
#endif
			}
#ifdef FIT_IMG
			if (val & ROOTFSFIT) {
#else
			if (val & ROOTFS) {
#endif
				/* rootfs */
#ifdef IMG_AUTH
				memset(cPath, 0, sizeof(cPath));
#ifndef FIT_IMG
				if (get_uboot_param("rootfs_size", cPath) != 0) {
					fprintf(stderr, "Failed to get variable\n");
					ret = -ENOENT;
					goto finish;
				}
#endif
				ret = fapi_ssImgValidateAndCommit("rootfs", atoi(cPath));
				if (ret != 0) {
					ret = -EPERM;
					goto finish;
				}
#else
				ret = 0;
#endif
			}
#ifndef FIT_IMG
			if (val & DTB) {
				fprintf(stderr, "DTB Image, nothing to be done\n");
				ret = 0;
			}
#endif
			break;
		}
	}
finish:
	return ret;
}


/**=====================================================================
 * @brief  API to switch activebank
 *
 * @param actbnk bank to switch
 *
 * @return
 *  0 on success
 *  error code on failure
 =======================================================================
 */
int fapi_Switch_bank(char *actbnk)
{
	int nRet = 0;
	char cPath[MAX_PATH_LEN] = {0};

	if (access("/tmp/.upg_progress", F_OK) == 0) {
		fprintf(stderr, "upgrade in progress, do reboot before switch bank\n");
		return -ECANCELED;
	}
	if (!(strcmp(actbnk, "A") == 0 || strcmp(actbnk, "B") == 0)) {
		LOGF_LOG_ERROR("Invalid active_bank '%s'\n", actbnk);
		return -EINVAL;
	}
	if (get_uboot_param("active_bank", cPath) != 0) {
		fprintf(stderr, "Failed to get variable\n");
		return -ENOENT;
	}
	if ((strcmp(actbnk, cPath) == 0)) {
		fprintf(stderr, "Current active_bank is %s, bank switching not needed\n", actbnk);
		return -ECANCELED;
	}

	if (get_uboot_param("udt_status", cPath) != 0) {
		fprintf(stderr, "Failed to get variable\n");
		return -ENOENT;
	}
	if (atoi(cPath) != UDT_IMAGE_NO_ACTION) {
		fprintf(stderr, "Previous Upgrade not completed yet as udt_status is %d!!!\n", atoi(cPath));
		return -ECANCELED;
	}

	if (set_uboot_param_str("active_bank", actbnk) != 0) {
		fprintf(stderr, "Failed to set active_bank\n");
		nRet = -EPERM;
	}
	return nRet;
}

/**=====================================================================
 * @brief  API to get last upgrade status
 *
 * @param value upgrade status  
 *
 * @return
 *  0 on success
 *  error code on failure
 =======================================================================
 */
int fapi_Get_lastupg_status(char *value)
{
	char cPath[MAX_PATH_LEN] = {0};

	if (get_uboot_param("last_upg_status", cPath) != 0) {
		fprintf(stderr, "Failed to get variable\n");
		return -ENOENT;
	}
	strncpy_s(value, MAX_PATH_LEN, cPath, MAX_PATH_LEN);
	return 0;
}

/**=====================================================================
 * @brief  API to get last upgrade time
 *
 * @param value upgrade time 
 *
 * @return
 *  0 on success
 *  error code on failure
 =======================================================================
 */
int fapi_Get_lastupg_time(char *value)
{
	char cPath[MAX_PATH_LEN] = {0};
	if (get_uboot_param("last_upg_time", cPath) != 0) {
		fprintf(stderr, "Failed to get variable\n");
		return -ENOENT;
	}
	strncpy_s(value, MAX_PATH_LEN, cPath, MAX_PATH_LEN);
	return 0;
}

/**=====================================================================
 * @brief  API to commit image
 *
 * @param void
 *
 * @return
 *  0 on success
 *  error code on failure
 =======================================================================
 */
int fapi_Image_commit(void)
{
	int ret = 0, udt_status;
	char cPath[MAX_PATH_LEN] = {0};
	struct timespec ts;
	struct tm *tm_info;
	char date_string[MAX_PATH_LEN];

	if (access("/tmp/.upg_progress", F_OK) == 0) {
		fprintf(stderr, "upgrade in progress, do reboot before commit\n");
		return -ECANCELED;
	}

	/*get date and time info*/
	clock_gettime(CLOCK_REALTIME, &ts);
	tm_info = localtime(&ts.tv_sec);
	strftime(date_string, sizeof(date_string), "%Y%m%d%H%M%S", tm_info);
	
	if (get_uboot_param("udt_status", cPath) != 0) {
		fprintf(stderr, "Failed to get variable\n");
		ret = -ENOENT;
		goto finish;
	}
	/* check udt_status is UDT_UPG_LINUX_NEW_IMAGE or not */
	udt_status = atoi(cPath);
	if (udt_status != UDT_NEW_IMAGE) {
		if (udt_status != UDT_IMAGE_RECOVERED) {
			fprintf(stderr, "udt_status value not Recovered or New Image, commit failed\n");
			return -ECANCELED;
		} else {
			fprintf(stdout, "A new S/W UPGRADE, in previous boot, failed and rejected." 
					"The original version was RECOVERED\n");
#ifdef FIT_IMG
			if (set_uboot_param_str("last_upg_time", date_string) != 0) {
				fprintf(stderr, "Failed to set last_upg_time'\n");
				ret = -EPERM;
			}
			if (set_uboot_param_str("last_upg_status", "Failed") != 0) {
				fprintf(stderr, "Failed to set 'last_upg_status' as 'Failed'\n");
				ret = -EPERM;
			}
#endif
#ifndef FIT_IMG
			if (access(RECOVERED_FILE, F_OK) != -1)
				system(RECOVERED_FILE);
			if (set_uboot_param_int("tep_size", 0) != 0) {
				fprintf(stderr, "Failed to set 'tep_size' to 0\n");
				ret = -EPERM;
			}
			if (set_uboot_param_int("rootfs_size", 0) != 0) {
				fprintf(stderr, "Failed to set 'rootfs_size' to 0\n");
				ret = -EPERM;
			}
#endif
			if (set_uboot_param_int("rbe_size", 0) != 0) {
				fprintf(stderr, "Failed to set 'rbe_size' to 0\n");
				ret = -EPERM;
			}
			if (set_uboot_param_int("upgrade_image", 0) != 0) {
				fprintf(stderr, "Failed to set 'upgrade_image' to 0\n");
				ret = -EPERM;
			}
			if (set_uboot_param_str("early_boot", "valid") != 0) {
				fprintf(stderr, "Failed to set 'early_boot' as 'valid'\n");
				ret = -EPERM;
			}
			if (set_uboot_param_str("late_boot", "valid") != 0) {
				fprintf(stderr, "Failed to set 'late_boot' as 'valid'\n");
				ret = -EPERM;
			}
			if (set_uboot_param_int("udt_status", UDT_IMAGE_NO_ACTION) != 0) {
				fprintf(stderr, "Fail to set 'udt_status' to zero\n");
				ret = -EPERM;
			}
			if (set_uboot_param_int("boot_count", 0) != 0) {
				fprintf(stderr, "Fail to set 'boot_count' to zero\n");
				ret = -EPERM;
			}
			memset(cPath, 0, sizeof(cPath));
			if (get_uboot_param("uboot_b_filesize", cPath) != 0) {
				fprintf(stderr, "Failed to get variable\n");
				ret = -ENOENT;
			}
			if (set_uboot_param_str("uboot_a_filesize", cPath) != 0) {
				fprintf(stderr, "Setting the uboot_a_filesize value Failed\n");
				ret = -EINVAL;
			}
			memset(cPath, 0, sizeof(cPath));
			if (get_uboot_param("tep_firmware_b_filesize", cPath) != 0) {
				fprintf(stderr, "Failed to get variable\n");
				ret = -ENOENT;
			}
			if (set_uboot_param_str("tep_firmware_a_filesize", cPath) != 0) {
				fprintf(stderr, "Setting the tep_firmware_a_filesize value Failed\n");
				ret = -EINVAL;
			}
			memset(cPath, 0, sizeof(cPath));
			if (get_uboot_param("rbe_b_filesize", cPath) != 0) {
				fprintf(stderr, "Failed to get variable\n");
				ret = -ENOENT;
			}
			if (set_uboot_param_str("rbe_a_filesize", cPath) != 0) {
				fprintf(stderr, "Setting the rbe_a_filesize value Failed\n");
				ret = -EINVAL;
			}
		}
	} else {
		
		fprintf(stdout, 
			"A new S/W UPGRADE, in previous boot, was detected and accepted."
			"Now Commiting the image.\n");
#ifdef FIT_IMG
		if (set_uboot_param_str("last_upg_time", date_string) != 0) {
			fprintf(stderr, "Failed to set last_upg_time'\n");
			ret = -EPERM;
		}
		if (set_uboot_param_str("last_upg_status", "Success") != 0) {
			fprintf(stderr, "Failed to set 'last_upg_status' as 'Failed'\n");
			ret = -EPERM;
		}
#endif
#ifndef FIT_IMG
		if (access(UPDATE_FILE, F_OK) != -1)
			if (system(UPDATE_FILE) != 0) {
				ret = -EPERM;
				if (set_uboot_param_int("udt_status", UDT_IMAGE_NO_ACTION) != 0) {
					fprintf(stderr, "upgrade_done.sh script is failing and also failed to reset 'udt_status'\n");
					goto finish;
				}
				system("reboot");
			}
#endif
		if (ret != 0)
			goto finish;
		ret = img_validate_and_commit();
		if (ret < 0) {
			fprintf(stderr, "Image commit failed\n");
			goto finish;
		}
		memset(cPath, 0, sizeof(cPath));
		if (get_uboot_param("early_boot", cPath) != 0) {
			fprintf(stderr, "Failed to get variable\n");
			ret = -ENOENT;
		}
		if (strncmp(cPath, "upgrade", strlen("upgrade")) == 0 ) {
			if (set_uboot_param_str("early_boot", "valid") != 0) {
				fprintf(stderr, "Failed to set 'early_boot' to valid\n");
				ret = -EPERM;
				goto finish;
			}
			memset(cPath, 0, sizeof(cPath));
			if (get_uboot_param("rbe_size", cPath) != 0) {
				fprintf(stderr, "Failed to get variable\n");
				ret = -ENOENT;
			}
			if (atoi(cPath) != 0)
				if (set_uboot_param_int("rbe_size", 0) != 0) {
					fprintf(stderr, "Failed to set 'rbe_size' to 0\n");
					ret = -EPERM;
					goto finish;
				}
#ifndef FIT_IMG
			memset(cPath, 0, sizeof(cPath));
			if (get_uboot_param("tep_size", cPath) != 0) {
				fprintf(stderr, "Failed to get variable\n");
				ret = -ENOENT;
			}
			if (atoi(cPath) != 0)
				if (set_uboot_param_int("tep_size", 0) != 0) {
					fprintf(stderr, "Failed to set 'tep_size' to 0\n");
					ret = -EPERM;
					goto finish;
				}
#endif
		}
		memset(cPath, 0, sizeof(cPath));
		if (get_uboot_param("late_boot", cPath) != 0) {
			fprintf(stderr, "Failed to get variable\n");
			ret = -ENOENT;
		}
		if (strncmp(cPath, "upgrade", strlen("upgrade")) == 0 ) {
			if (set_uboot_param_str("late_boot", "valid") != 0) {
				fprintf(stderr, "Setting the late_boot value Failed as valid\n");
				ret = -EPERM;
				goto finish;
			}
#ifndef FIT_IMG
			memset(cPath, 0, sizeof(cPath));
			if (get_uboot_param("rootfs_size", cPath) != 0) {
				fprintf(stderr, "Failed to get variable\n");
				ret = -ENOENT;
			}
			if (atoi(cPath) != 0)
				if (set_uboot_param_int("rootfs_size", 0) != 0) {
					fprintf(stderr, "Failed to set 'rootfs_size' to 0\n");
					ret = -EPERM;
					goto finish;
				}
#endif
		}
		memset(cPath, 0, sizeof(cPath));
		if (get_uboot_param("upgrade_image", cPath) != 0) {
			fprintf(stderr, "Failed to get variable\n");
			ret = -ENOENT;
		}
		if (atoi(cPath) != 0)
			if (set_uboot_param_int("upgrade_image", 0) != 0) {
				fprintf(stderr, "Failed to set 'upgrade_image' to 0\n");
				ret = -EPERM;
				goto finish;
			}
		if (set_uboot_param_int("udt_status", UDT_IMAGE_NO_ACTION) != 0) {
			fprintf(stderr, "Fail to set 'udt_status' to zero\n");
			ret = -EPERM;
			goto finish;
		}
		if (set_uboot_param_int("boot_count", 0) != 0) {
			fprintf(stderr, "Fail to set 'boot_count' to zero\n");
			ret = -EPERM;
			goto finish;
		}
	}
finish:
	return ret;
}

static int image_hdr_validation(image_header_t *pxImgHeader)
{
	/* if mkimage header is not available, exit */
	if (pxImgHeader) {
		switch (ntohl(pxImgHeader->img_hdr_magic)) {
			case IMG_HDR_MAGIC:
				if (!is_image_hcrc_valid(pxImgHeader)) {
					fprintf(stderr, "Bad Header Checksum\n");
					return -EINVAL;
				}
				fprintf(stdout, "Successful validation of image header and checksum\n");
				break;
			case FLATDT_MAGIC:
				/* DTB crc is not available, so not added checking for it */
				break;
			default:
				fprintf(stderr, "no mkimage header\n");
				return -EINVAL;
		}
	}

	return 0;
}
#ifdef FIT_IMG

static const char *fapi_ssNormalizeFitNodeName(const char *node_name)
{
	if (!node_name)
		return NULL;

	if (strcmp(node_name, "u-boot") == 0)
		return "uboot";

	if (strcmp(node_name, "kernel-1") == 0)
		return "kernel-dtb";

	if (strcmp(node_name, "rootfs") == 0)
		return "filesystem";

	return node_name;
}

int fapi_ssUpgradeNestedFit(const void *fit, img_param_t image_auth, int *early_boot)
{
	int noffset;
	int depth = 0, len = 0;
	const void *data;
	void *aligned_node_buf = NULL;
	const char* node_name;
	uint32_t file_size = 0;
	
	noffset = fdt_path_offset(fit, FIT_IMAGES_PATH);
	if (noffset < 0) {
		fprintf(stderr, "FIT is missing image nodes\n");
		return -EINVAL;
	}
	
	while ((noffset = fdt_next_node(fit, noffset, &depth)) >= 0) {
		if (depth < 1)
			break;
		if (depth == 1) {
			fprintf(stdout,"\nnoffset %d depth %d name %s\n", noffset, depth,
                      		fdt_get_name(fit, noffset, NULL));
			data = fdt_getprop(fit, noffset, FIT_DATA_PROP, &len);
			if (!data || len <= 0)
				continue;
			
			aligned_node_buf = malloc(len);
			if(aligned_node_buf == NULL) {
				return -EINVAL;
			}
			memcpy(aligned_node_buf, data, len);
			
			node_name = fdt_get_name(fit, noffset, NULL);
			node_name = fapi_ssNormalizeFitNodeName(node_name);

			if((strcmp(node_name, "uboot") == 0) || (strcmp(node_name, "tep") == 0)
				|| (strcmp(node_name, "rbe") == 0)) {
				*early_boot |= 1;
				strncpy_s(image_auth.img_name, sizeof(image_auth.img_name), node_name, strlen(node_name));
			} else if(strcmp(node_name, "kernel-dtb") == 0) {
				*early_boot |= 2;
				strncpy_s(image_auth.img_name, sizeof(image_auth.img_name), node_name, strlen(node_name));
			} else if(strcmp(node_name, "filesystem") == 0) {
				*early_boot |= 2;
				strncpy_s(image_auth.img_name, sizeof(image_auth.img_name), node_name, strlen(node_name));
			}
			if (!fdt_check_header(aligned_node_buf)) { 
				image_auth.src_img_addr = (unsigned char *)aligned_node_buf;
				image_auth.src_img_len = len;
			} else {
				if (strcmp(node_name, "rbe") == 0) {
					uint32_t cur_par_size=0, pad, file_read_size =0;
					image_header_t x_img_header;
 					/*RBE image came here, may be part of total image.Pass just the data as RBE is not fit image*/
					image_auth.src_img_addr = (unsigned char *)aligned_node_buf;
					x_img_header = *((image_header_t *)image_auth.src_img_addr);
					fprintf(stdout, "Image contains header with name [%s]\n",x_img_header.img_hdr_name);

					cur_par_size = sizeof(image_header_t) + ntohl(x_img_header.img_hdr_size);
					pad = (16 - (cur_par_size % 16)) % 16;
					file_read_size = cur_par_size + pad;
					fprintf(stdout, "This is not kernel or uboot image and so removing header\n");
					image_auth.src_img_addr += sizeof(image_header_t);
					file_size = image_auth.src_img_len;
					image_auth.src_img_len = file_read_size - pad - sizeof(image_header_t);
					strncpy_s(image_auth.img_name, MAX_PATH_LEN, node_name, MAX_PATH_LEN);
				} else {
					image_auth.src_img_addr = (unsigned char *)fit;
					image_auth.src_img_len = fdt_totalsize(fit);
				}
			}
			fprintf(stdout, "fapi_ssImgUpgrade called for %s\n", image_auth.img_name);
			if ((strcmp(node_name, "kernel-dtb") == 0) || (strcmp(node_name, "filesystem") == 0))
				image_auth.src_img_len = len;
			if (fapi_ssImgUpgrade(image_auth) < 0) {
				fprintf(stderr, "%s Image upgrade failed\n", image_auth.img_name);
				if (aligned_node_buf != NULL) {
					free(aligned_node_buf);
					aligned_node_buf = NULL;
				}
				return -EPERM;
			} else {
				fprintf(stderr, "%s Image upgrade passed\n", image_auth.img_name);
				if (strcmp(node_name, "rbe") == 0) {
					if (set_uboot_param_int("rbe_size", image_auth.src_img_len) != 0) {
						fprintf(stderr, "Setting the rbe_size value Failed\n");
						image_auth.src_img_addr -= sizeof(image_header_t);
						if (aligned_node_buf != NULL) {
							free(aligned_node_buf);
							aligned_node_buf = NULL;
						}
						return -EINVAL;
					}
					image_auth.src_img_addr -= sizeof(image_header_t);
					image_auth.src_img_len = file_size;
				}
			}
		}
	}
	if (aligned_node_buf != NULL) {
		free(aligned_node_buf);
		aligned_node_buf = NULL;
	}
	return fdt_totalsize(fit);
}
static int fapi_ssValidateNestedFit(const void *fit, img_param_t image_auth, int *currentimage) 
{
	int noffset = -1;
	int depth = 0, len;
	const void *data;
	void *aligned_node_buf = NULL;
	const char* node_name;
	uint32_t file_size = 0;

	noffset = fdt_path_offset(fit, FIT_IMAGES_PATH);
	if (noffset < 0) {
		fprintf(stderr, "FIT is missing image nodes\n");
		return -EINVAL;
	}
	
	while ((noffset = fdt_next_node(fit, noffset, &depth)) >= 0) {
		LOGF_LOG_DEBUG("noffset %d depth %d name %s\n", noffset, depth,
						fdt_get_name(fit, noffset, NULL));
		if (depth < 1)
			break;
		if (depth == 1) {
			fprintf(stdout,"\nnoffset %d depth %d name %s\n", noffset, depth,
					fdt_get_name(fit, noffset, NULL));
			data = fdt_getprop(fit, noffset, FIT_DATA_PROP, &len);
			if (!data || len <= 0)
				continue;
			
			aligned_node_buf = malloc(len);
			if(aligned_node_buf == NULL)
				return -EINVAL;

			memcpy(aligned_node_buf, data, len);
			
			node_name = fdt_get_name(fit, noffset, NULL);
			node_name = fapi_ssNormalizeFitNodeName(node_name);

			if (node_name && strcmp(node_name, "uboot") == 0) {
				*currentimage += BOOTLOADERFIT;
			} else if(node_name && strcmp(node_name, "tep") == 0) {
				*currentimage += TEPFIT;
			} else if(node_name && strcmp(node_name, "kernel-dtb") == 0) {
				*currentimage += KERNELDTBFIT;
			} else if(node_name && strcmp(node_name, "filesystem") == 0) {
				*currentimage += ROOTFSFIT;
			} else if(node_name && strcmp(node_name, "rbe") == 0) {
				*currentimage += RBE;
			} else if(node_name && (strcmp(node_name, "fdt-1") == 0 || strcmp(node_name, "fdt-2") == 0)) {
				continue;
			} else {
				fprintf(stderr, "Skipping unsupported FIT node during validation: %s\n", node_name ? node_name : "<null>");
				continue;
			}
			strncpy_s(image_auth.img_name, sizeof(image_auth.img_name), node_name, (strlen("node_name")+1));
			if (!fdt_check_header(aligned_node_buf)) {
				image_auth.src_img_addr = (unsigned char *)aligned_node_buf;
				fprintf(stdout, "Image authentication called for %s\n", image_auth.img_name);
				if (fapi_ssImgAuth(image_auth) < 0) {
					fprintf(stderr, "%s Validation failed\n", image_auth.img_name);
					if (aligned_node_buf != NULL)
						free(aligned_node_buf);
					return -EPERM;
				} else 
					fprintf(stdout, "%s Validation passed\n", image_auth.img_name);
			} else {
				if (strcmp(node_name, "rbe") == 0) {
					uint32_t cur_par_size=0, pad, file_read_size =0;
					image_header_t x_img_header;
					/*RBE image came here, may be part of total image*/
					image_auth.src_img_addr = (unsigned char *)aligned_node_buf; 
					x_img_header = *((image_header_t *)image_auth.src_img_addr);
					fprintf(stdout, "Image contains header with name [%s]\n",x_img_header.img_hdr_name);

					cur_par_size = sizeof(image_header_t) + ntohl(x_img_header.img_hdr_size);
					pad = (16 - (cur_par_size % 16)) % 16;
					file_read_size = cur_par_size + pad;
					fprintf(stdout, "This is not kernel or uboot image and so removing header\n");
					image_auth.src_img_addr += sizeof(image_header_t);
					file_size = image_auth.src_img_len;
					image_auth.src_img_len = file_read_size - pad - sizeof(image_header_t);
					strncpy_s(image_auth.img_name, MAX_PATH_LEN, node_name, MAX_PATH_LEN);
				} else { 
					image_auth.src_img_addr = (unsigned char *)fit;
				}
				fprintf(stdout, "Image authentication called for %s\n", image_auth.img_name);
				if (fapi_ssImgAuth(image_auth) < 0){
					fprintf(stderr, "%s Validation failed\n", image_auth.img_name);
					if (aligned_node_buf != NULL)
						free(aligned_node_buf);
					return -EPERM;
				} else 
					fprintf(stderr, "\n%s Validation passed\n", image_auth.img_name);
				if (strcmp(node_name, "rbe") == 0) {
					image_auth.src_img_addr -= sizeof(image_header_t);
					image_auth.src_img_len = file_size;
				}
			}
		}
	}
	if (aligned_node_buf != NULL)
		free(aligned_node_buf);
	return fdt_totalsize(fit);
}
#endif

static int fapi_ssValidate_Image(const img_param_t img, bool upgrade)
{
	uint32_t cur_par_size=0, pad, file_read_size =0, total_file_read_size = 0;
	unsigned char *header = NULL;
	image_header_t x_img_header, *img_header = NULL;
	char name[MAX_PATH_LEN] = {0};
	int ret = 0, i, currentimage = 0;
#ifndef FIT_IMG
	int fullimage = 0;
#endif
	img_param_t img_param;
	img_param.src_img_fd = img.src_img_fd;
	header = img.src_img_addr;
	do {
		x_img_header = *((image_header_t *)header);
#ifndef FIT_IMG
		if(x_img_header.img_hdr_type == IMG_HDR_VAR_MULTI) {
			img_header = &x_img_header;
			if (image_hdr_validation(img_header) < 0) {
				ret = -EINVAL;
				goto finish;
			}
			fullimage = 1;
			cur_par_size = sizeof(image_header_t) + 8;
			total_file_read_size += cur_par_size;
			header = img.src_img_addr + total_file_read_size;
			continue;
		}
#endif
		cur_par_size = sizeof(image_header_t) + ntohl(x_img_header.img_hdr_size);
		pad = (16 - (cur_par_size % 16)) % 16;
		header = img.src_img_addr + total_file_read_size;
		file_read_size = cur_par_size + pad;

		img_header = &x_img_header;
		if (image_hdr_validation(img_header) < 0) {
			ret = -EINVAL;
			goto finish;
		}
		if (ntohl(*(uint32_t *)header) == FLATDT_MAGIC) {
#ifdef FIT_IMG
			ret = fapi_ssValidateNestedFit((void *)header, img_param, &currentimage);
			if(ret > 0) {
				file_read_size = ret;
				header += sizeof(image_header_t);
				ret = 0;
				fprintf(stdout, "fapi_ssValidateNestedFit passed file_read_size %u\n", file_read_size);
				goto dtb_finish;
			} else if(ret < 0) { 
				fprintf(stderr, " fapi_ssValidateNestedFit returned err , %s Image Validation failed\n", img_param.img_name);
				ret = -EPERM;
				goto finish;
			}
#else
			if (fullimage != 1) {
				fprintf(stderr, "Only DTB image upgrade not supported!\n");
				ret = -EINVAL;
				goto finish;
			} else {
				currentimage |= DTB;
				memcpy(&img_param, &img, sizeof(img));
				strncpy_s(img_param.img_name, sizeof(img_param.img_name), "dtb", strlen("dtb"));
				img_param.src_img_addr = header;
				ret = fapi_ssImgAuth(img_param);
				if (ret < 0) {
					fprintf(stderr, "%s Image Validation failed\n", img_param.img_name);
					ret = -EPERM;
					goto finish;
				}
				file_read_size = ret;
				header += sizeof(image_header_t);
				ret = 0;
				goto dtb_finish;
			}
#endif
		}

		switch(x_img_header.img_hdr_type) {
#ifndef FIT_IMG
			case IMG_HDR_VAR_FILESYSTEM:
				if (fullimage != 1) {
					fprintf(stderr, "Only Rootfs image upgrade not supported!\n");
					ret = -EINVAL;
					goto finish;
				}
				currentimage |= ROOTFS;
				sprintf_s(name, sizeof(name),"rootfs");
				break;
			case IMG_HDR_VAR_KERNEL:
				if (fullimage != 1) {
					fprintf(stderr, "Only kernel image upgrade not supported!\n");
					ret = -EINVAL;
					goto finish;
				}
				currentimage |= KERNEL;
				sprintf_s(name, sizeof(name),"kernel");
				break;
			case IMG_HDR_VAR_UBOOT:
				sprintf_s(name, sizeof(name), "uboot");
				currentimage |= BOOTLOADER;
				break;
#endif
			case IMG_HDR_VAR_FIRMWARE:
				if (strncmp((char *)x_img_header.img_hdr_name, "RBE", sizeof(x_img_header.img_hdr_name)) == 0) {
					sprintf_s(name, sizeof(name), "rbe");
					currentimage |= RBE;
#ifndef FIT_IMG
				} else if (strncmp((char *)x_img_header.img_hdr_name, "TEP firmware", sizeof(x_img_header.img_hdr_name)) == 0) {
					sprintf_s(name, sizeof(name),"firmware");
					currentimage |= TEP;
#endif
				} else {
					fprintf(stderr, "Unknown image type, not a RBE or TEP!!\n");//JP
					ret = -EINVAL;
					goto finish;
				}
				break;
			default:
				fprintf(stderr, "Unknown image type!!\n");
				ret = -EINVAL;
				goto finish;
		}
		memcpy(&img_param, &img, sizeof(img));
		strncpy_s(img_param.img_name, MAX_PATH_LEN, name, MAX_PATH_LEN);

		img_param.src_img_addr = header;
		if ((x_img_header.img_hdr_type != IMG_HDR_VAR_KERNEL) &&
			(x_img_header.img_hdr_type != IMG_HDR_VAR_UBOOT))
			img_param.src_img_len = file_read_size - pad - sizeof(image_header_t);
		else
			img_param.src_img_len = file_read_size - pad;

		ret = fapi_ssImgAuth(img_param);
		if (ret != 0) {
			fprintf(stderr, "%s Image validation failed\n", img_param.img_name);
			ret = -EPERM;
			goto finish;
		}
dtb_finish:
		total_file_read_size += file_read_size;

		if ((x_img_header.img_hdr_type != IMG_HDR_VAR_KERNEL) &&
			(x_img_header.img_hdr_type != IMG_HDR_VAR_UBOOT))
			header += cur_par_size;
		else
			header += file_read_size;
	} while (img.src_img_len > total_file_read_size);

	if (upgrade == true) {
		for (i = 0; i < ARRAY_SIZE(image_list); i++) {
			if (image_list[i] != currentimage) {
				ret = -EINVAL;
				continue;
			}

			fprintf(stderr, "provided image list %d is supported for upgrade\n",currentimage);
			ret = set_uboot_param_int("upgrade_image", currentimage);
			if (ret < 0) {
				fprintf(stderr, "Setting the upgrade_image value Failed\n");
				ret = -EINVAL;
				goto finish;
			}
			break;
		}
	}

finish:
	return ret;
}

static int fapi_ssUpgradeImage(const img_param_t img)
{
	uint32_t cur_par_size=0, pad, file_read_size =0, total_file_read_size = 0;
	unsigned char *header = NULL;
	image_header_t x_img_header;
	char name[MAX_PATH_LEN] = {0}, early_boot[MAX_PATH_LEN] = {0}, late_boot[MAX_PATH_LEN] = {0};
	char cPath[MAX_PATH_LEN] = {0};
	int ret = 0;
#ifdef FIT_IMG
	int earlyboot = 0;
#else
	int fullimage = 0;
#endif
	img_param_t img_param;
	header = img.src_img_addr;
	do {
		x_img_header = *((image_header_t *)header);
#ifndef FIT_IMG
		if(x_img_header.img_hdr_type == IMG_HDR_VAR_MULTI) {
			fullimage = 1;
			cur_par_size = sizeof(image_header_t) + 8;
			total_file_read_size += cur_par_size;
			header =  img.src_img_addr + total_file_read_size;
			continue;
		}
#endif
		cur_par_size = sizeof(image_header_t) + ntohl(x_img_header.img_hdr_size);
		pad = (16 - (cur_par_size % 16)) % 16;
		header =  img.src_img_addr + total_file_read_size;
		file_read_size = cur_par_size + pad;

		if (ntohl(*(uint32_t *)header) == FLATDT_MAGIC) {
			memcpy(&img_param, &img, sizeof(img));
			img_param.src_img_addr = header;
#ifdef FIT_IMG
			ret = fapi_ssUpgradeNestedFit((void *)header, img_param, &earlyboot);
			if (ret > 0) {
				if (earlyboot & 1)
					sprintf_s(early_boot, sizeof(name), "early_boot");
				if (earlyboot & 2)
					sprintf_s(late_boot, sizeof(name), "late_boot");

				file_read_size = ret;
				header += sizeof(image_header_t);
				ret = 0;
				goto dtb_finish;
			} else if (ret < 0) {
				fprintf(stderr, "\nfapi_ssUpgradeNestedFit returned err , %s Image upgrade failed\n", img_param.img_name);
				ret = -EPERM;
				goto finish;
			}
#else
			if (fullimage != 1) {
				fprintf(stderr, "Only DTB image upgrade not supported!\n");
				ret = -EINVAL;
				goto finish;
			} else {
				strncpy_s(img_param.img_name, sizeof(img_param.img_name), "dtb", strlen("dtb"));
				ret = fapi_ssImgUpgrade(img_param);
				if (ret < 0) {
					fprintf(stderr, "%s Image upgrade failed\n", img_param.img_name);
					ret = -EINVAL;
					goto finish;
				}
				sprintf_s(late_boot, sizeof(name), "late_boot");
				file_read_size = ret;
				header += sizeof(image_header_t);
				ret = 0;
				goto dtb_finish;
			}
#endif
		}

		switch(x_img_header.img_hdr_type) {
#ifndef FIT_IMG
			case IMG_HDR_VAR_FILESYSTEM:
				if (fullimage != 1) {
					fprintf(stderr, "Only Rootfs image upgrade not supported!\n");
					ret = -EINVAL;
					goto finish;
				}
				sprintf_s(name, sizeof(name),"rootfs");
				sprintf_s(late_boot, sizeof(name), "late_boot");
				break;
			case IMG_HDR_VAR_KERNEL:
				if (fullimage != 1) {
					fprintf(stderr, "Only kernel image upgrade not supported!\n");
					ret = -EINVAL;
					goto finish;
				}
				sprintf_s(name, sizeof(name),"kernel");
				sprintf_s(late_boot, sizeof(name), "late_boot");
				break;
			case IMG_HDR_VAR_UBOOT:
				sprintf_s(name, sizeof(name), "uboot");
				sprintf_s(early_boot, sizeof(name), "early_boot");
				break;
#endif
			case IMG_HDR_VAR_FIRMWARE:
				if (strncmp((char *)x_img_header.img_hdr_name, "RBE", sizeof(x_img_header.img_hdr_name)) == 0)
					sprintf_s(name, sizeof(name), "rbe");
#ifndef FIT_IMG
				else if (strncmp((char *)x_img_header.img_hdr_name, "TEP firmware", sizeof(x_img_header.img_hdr_name)) == 0)
					sprintf_s(name, sizeof(name),"firmware");
#endif
				else {
					fprintf(stderr, "Unknown image type, not a RBE or TEP!!\n");
					ret = -EINVAL;
					goto finish;
				}
				sprintf_s(early_boot, sizeof(name), "early_boot");
				break;
			default:
				fprintf(stderr, "Unknown image type!!\n");
				ret = -EINVAL;
				goto finish;
		}
		memcpy(&img_param, &img, sizeof(img));
		strncpy_s(img_param.img_name, MAX_PATH_LEN, name, MAX_PATH_LEN);

		if (ntohl(x_img_header.img_hdr_magic) == IMG_HDR_MAGIC) {
			fprintf(stdout, "Image contains header with name [%s]\n",x_img_header.img_hdr_name);
			if ((x_img_header.img_hdr_type != IMG_HDR_VAR_KERNEL) &&
				(x_img_header.img_hdr_type != IMG_HDR_VAR_UBOOT)) {
				fprintf(stdout, "This is not kernel or uboot image and so removing header\n");
				header += sizeof(image_header_t);
				cur_par_size -= sizeof(image_header_t);
			}
		}

		img_param.src_img_addr = header;
		if ((x_img_header.img_hdr_type != IMG_HDR_VAR_KERNEL) &&
			(x_img_header.img_hdr_type != IMG_HDR_VAR_UBOOT))
			img_param.src_img_len = file_read_size - pad - sizeof(image_header_t);
		else
			img_param.src_img_len = file_read_size - pad;

		ret = fapi_ssImgUpgrade(img_param);
		if (ret == 0) {
			if (strncmp((char *)x_img_header.img_hdr_name, "RBE", sizeof(x_img_header.img_hdr_name)) == 0) {
				if (set_uboot_param_int("rbe_size", img_param.src_img_len) != 0) {
					fprintf(stderr, "Setting the rbe_size value Failed\n");
					ret = -EINVAL;
					goto finish;
				}
#ifndef FIT_IMG
			} else if (x_img_header.img_hdr_type == IMG_HDR_VAR_FILESYSTEM) {
				if (set_uboot_param_int("filesystem_size", img_param.src_img_len) != 0) {
					fprintf(stderr, "Setting the filesystem_size value Failed\n");
					ret = -EINVAL;
					goto finish;
				}
				if (set_uboot_param_int("rootfs_size", img_param.src_img_len) != 0) {
					fprintf(stderr, "Setting the rootfs_size value Failed\n");
					ret = -EINVAL;
					goto finish;
				}
			} else if (strncmp((char *)x_img_header.img_hdr_name, "TEP firmware", sizeof(x_img_header.img_hdr_name)) == 0) {
				if (set_uboot_param_int("tep_size", img_param.src_img_len) != 0) {
					fprintf(stderr, "Setting the tep_size value Failed\n");
					ret = -EINVAL;
					goto finish;
				}
#endif
			}
		} else {
			fprintf(stderr, "%s Image Upgrade failed\n", img_param.img_name);
			ret = -EINVAL;
			goto finish;
		}
dtb_finish:
		total_file_read_size += file_read_size;

		if ((x_img_header.img_hdr_type != IMG_HDR_VAR_KERNEL) &&
			(x_img_header.img_hdr_type != IMG_HDR_VAR_UBOOT))
			header += cur_par_size;
		else
			header += file_read_size;
	} while (img.src_img_len > total_file_read_size);

	if (ret == 0) {
		if (strncmp(early_boot, "early_boot", strlen("early_boot")) == 0) {
			if (set_uboot_param_str("early_boot", "upgrade") != 0) {
				fprintf(stderr, "Setting the early boot status Failed\n");
				ret = -EINVAL;
				goto finish;
			}
		}
		if (strncmp(late_boot, "late_boot", strlen("late_boot")) == 0) {
			memset(cPath, 0, sizeof(cPath));
			if (get_uboot_param("active_bank", cPath) != 0) {
				fprintf(stderr, "Failed to get variable\n");
				ret = -ENOENT;
				goto finish;
			}
			if (cPath[0] == 'A')
				ret = set_uboot_param_str("active_bank", "B");
			else if (cPath[0] == 'B')
				ret = set_uboot_param_str("active_bank", "A");
			else
				ret = -EINVAL;
			if (ret != 0) {
				fprintf(stderr, "Toggle active bank Failed\n");
				ret = -EINVAL;
				goto finish;
			}
			if (set_uboot_param_str("late_boot", "upgrade") != 0) {
				fprintf(stderr, "Setting the late boot status Failed\n");
				ret = -EINVAL;
				goto finish;
			}
		}
	}
finish:
	return ret;
}

int fapi_Image_Verify(const char *path)
{
	int ret = 0, file_fd = 0, flag = 0;
	struct stat filestat = {0};
	img_param_t img, img_param;

	file_fd = open(path, O_RDONLY);
	if (file_fd < 0) {
		fprintf(stderr, "The file %s could not be opened\n", path);
		ret = -ENOENT;
		goto finish;
	}

	if (fstat(file_fd, &filestat)) {
		fprintf(stderr, "fstat error: [%s]\n",strerror(errno));
		close(file_fd);
		ret = -ENOENT;
		goto finish;
	}

	img.src_img_fd=file_fd;
	img.src_img_len=filestat.st_size;
	if (!img.src_img_len) {
		fprintf(stderr, "Empty File...\n");
		ret = -ENOENT;
		goto finish;
	} else {
		img.src_img_addr = mmap(0, img.src_img_len, PROT_READ, MAP_SHARED, img.src_img_fd, 0);
		if(img.src_img_addr == MAP_FAILED) {
			fprintf(stderr, "MMAP failed... %s\n",strerror(errno));
			ret = -ENOENT;
			goto finish;
		}
		flag = 1;
	}
	memcpy(&img_param, &img, sizeof(img));
	ret = fapi_ssValidate_Image(img_param, false);
	if (ret < 0) {
		fprintf(stderr, "Image validation failed\n");
		goto finish;
	}
	fprintf(stdout, "Image validation passed \n");

finish:
	if ((img.src_img_addr != NULL) && (flag == 1)) {
		if(munmap(img.src_img_addr,  img.src_img_len) != 0)
			fprintf(stderr, "unmaping failied\n");
	}
	if(file_fd >= 0)
		close(file_fd);
	return ret;
}

int fapi_Ext4_Image_Verify(const char *path)
{
	int ret = 0;
	char mnt_template[] = "/tmp/secupg_ext4_XXXXXX";
	char *mnt_dir = NULL;
	bool mounted = false;
	char cmd[MAX_PATH_LEN * 2] = {0};

	if (!path || path[0] == '\0') {
		fprintf(stderr, "Invalid ext4 image path\n");
		return -EINVAL;
	}

	/* Create unique mount directory */
	mnt_dir = mkdtemp(mnt_template);
	if (!mnt_dir) {
		fprintf(stderr, "mkdtemp failed: %s\n", strerror(errno));
		return -errno;
	}

	/* Mount ext4 image with read-only options to preserve hash */
	snprintf(cmd, sizeof(cmd), "mount -o loop,ro,noload,norecovery -t ext4 '%s' '%s'", path, mnt_dir);
	ret = system(cmd);
	if (ret != 0) {
		fprintf(stderr, "Failed to mount ext4 image '%s' at '%s'\n", path, mnt_dir);
		ret = -EPERM;
		goto cleanup;
	}
	mounted = true;

	/* Authenticate mandatory ITBs inside the mounted image */
	static const char *itb_files[] = { "kernel.itb", "rootfs.itb" };
	for (size_t i = 0; i < (sizeof(itb_files) / sizeof(itb_files[0])); i++) {
		char itb_path[MAX_PATH_LEN * 2] = {0};
		struct stat st;

		snprintf(itb_path, sizeof(itb_path), "%s/%s", mnt_dir, itb_files[i]);

		if (stat(itb_path, &st) != 0) {
			fprintf(stderr, "File not found in ext4 image: %s (%s)\n", itb_path, strerror(errno));
			ret = -ENOENT;
			goto cleanup;
		}
		if (!S_ISREG(st.st_mode)) {
			fprintf(stderr, "Not a regular file: %s\n", itb_path);
			ret = -EINVAL;
			goto cleanup;
		}

		fprintf(stdout, "Authenticating %s...\n", itb_files[i]);
		ret = fapi_Image_Verify(itb_path);
		if (ret != 0) {
			fprintf(stderr, "Authentication failed for %s\n", itb_path);
			goto cleanup;
		}
		fprintf(stdout, "Authentication successful for %s\n", itb_files[i]);
	}

cleanup:
	if (mounted) {
		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "umount '%s'", mnt_dir);
		system(cmd);
	}

	/* Remove directory */
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "rm -rf '%s'", mnt_dir);
	system(cmd);

	return ret;
}

/**=====================================================================
 * @brief  image upgrade from linux
 *
 * @param path
 * image path to be updated
 *
 * @return
 *  0 on success
 *  error code on failure
 =======================================================================
 */
int fapi_Image_upgrade(const char *path)
{
	int ret = 0, val = 0, file_fd = 0, flag = 0;
	struct stat filestat = {0};
	img_param_t img, img_param;
	char cPath[MAX_PATH_LEN] = {0};
	
	if (get_uboot_param("udt_status", cPath) != 0) {
		fprintf(stderr, "Failed to get variable\n");
		ret = -ENOENT;
		goto finish;
	}
	if (atoi(cPath) != UDT_IMAGE_NO_ACTION) {
		fprintf(stderr, "Previous Upgrade not completed yet as udt_status is %d!!!\n", atoi(cPath));
		ret = -ECANCELED;
		goto finish;
	}

	if (access("/tmp/.upg_progress", F_OK) == 0) {
		fprintf(stderr, "upgrade in progress, do reboot and commit before another upgrade\n");
		return -ECANCELED;
	}

	file_fd = open(path, O_RDONLY);
	if (file_fd < 0) {
		fprintf(stderr, "The file %s could not be opened\n", path);
		ret = -ENOENT;
		goto finish;
	}

	if (fstat(file_fd, &filestat)) {
		fprintf(stderr, "fstat error: [%s]\n",strerror(errno));
		close(file_fd);
		ret = -ENOENT;
		goto finish;
	}

	img.src_img_fd=file_fd;
	img.src_img_len=filestat.st_size;
	if (!img.src_img_len) {
		fprintf(stderr, "Empty File...\n");
		ret = -ENOENT;
		goto finish;
	} else {
		img.src_img_addr = mmap(0, img.src_img_len, PROT_READ, MAP_SHARED, img.src_img_fd, 0);
		if(img.src_img_addr == MAP_FAILED) {
			fprintf(stderr, "MMAP failed... %s\n",strerror(errno));
			ret = -ENOENT;
			goto finish;
		}
		flag = 1;
	}
	ret = add_uboot_param("upgrade_image", 0);
	if (ret < 0) {
		fprintf(stderr, "upgrade_image value addition Failed\n");
		goto finish;
	}

	memcpy(&img_param, &img, sizeof(img));
	ret = fapi_ssValidate_Image(img_param, true);
	if (ret < 0) {
		fprintf(stderr, "Image validation failed\n");
		goto finish;
	}
	fprintf(stdout, "Image validation passed \n");
	if (set_uboot_param_int("udt_status", UDT_NEW_IMAGE) != 0) {
		fprintf(stderr, "Setting the udt status Failed\n");
		ret = -EPERM;
		goto finish;
	}
	memset(cPath, 0, sizeof(cPath));
	if (get_uboot_param("upgrade_image", cPath) != 0) {
		fprintf(stderr, "Failed to get variable\n");
		ret = -ENOENT;
		goto finish;
	}
	val = atoi(cPath);

	if (((val & RBE) == RBE) ||
#ifdef FIT_IMG
		((val & BOOTLOADERFIT) == BOOTLOADERFIT) || ((val & TEPFIT) == TEPFIT)) {
#else
		((val & BOOTLOADER) == BOOTLOADER) || ((val & TEP) == TEP)) {
#endif
		if (set_uboot_param_str("early_boot", "invalid") != 0) {
			fprintf(stderr, "Setting the early boot status Failed\n");
			ret = -EPERM;
			goto finish;
		}
#ifndef FIT_IMG
		if (val & TEP) {
			ret = add_uboot_param("tep_size", 0);
			if (ret < 0) {
				fprintf(stderr, "tep_size value addition Failed\n");
				goto finish;
			}
		}
#endif
		if (val & RBE) {
			ret = add_uboot_param("rbe_size", 0);
			if (ret < 0) {
				fprintf(stderr, "rbe_size value addition Failed\n");
				goto finish;
			}
		}
	}
#ifdef FIT_IMG
	if (((val & KERNELDTBFIT) == KERNELDTBFIT) || ((val & ROOTFSFIT) == ROOTFSFIT)) {
#else
	if (((val & KERNEL) == KERNEL) || ((val & ROOTFS) == ROOTFS) || ((val & DTB) == DTB)) {
#endif
		if (set_uboot_param_str("late_boot", "invalid") != 0) {
			fprintf(stderr, "Setting the late boot status Failed\n");
			ret = -EPERM;
			goto finish;
		}
#ifndef FIT_IMG
		ret = add_uboot_param("rootfs_size", 0);
		if (ret < 0) {
			fprintf(stderr, "rootfs_size value addition Failed\n");
			goto finish;
		}
#endif
	}
	ret = fapi_ssUpgradeImage(img);
	if (ret == 0)
		system("touch /tmp/.upg_progress");

finish:
	if ((img.src_img_addr != NULL) && (flag == 1)) {
		if(munmap(img.src_img_addr,  img.src_img_len) != 0)
			fprintf(stderr, "unmaping failied\n");
	}
	if(file_fd >= 0)
		close(file_fd);
	return ret;
}
