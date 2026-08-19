/******************************************************************************

  Copyright (C) 2022 MaxLinear, Inc

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : sysupgrade_ubus.c                                          *
 *     Project    : UGW                                                        *
 *     Description: Ubus message handlers for sysupgrade such as get/modify    *
 *                  logging etc.                                               *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/select.h>
#include <ltq_api_include.h>
#include <sys/mman.h>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include "ugw_proto.h"
#include "sec_upgrade.h"
#include "include/sysupgrade.h"
#include "include/sysupgrade_ubus.h"
#include "include/sysupgrade_ubus_api.h"
#include "process_handle.h"
#include "fapi_img_auth.h"
#include <crc32.h>
#include <errno.h>

#define MAX_FILELINE_LEN	332

#define FIRMWARE_IMG "firmware.img"

enum {
	IMG_ACTIVATE_VALUE,
	__IMG_ACTIVATE_MAX
};

enum {
	IMAGE_NAME_VALUE,
	__WRITE_IMG_MAX
};

enum {
	SET_UB_COMMIT_BANK_VALUE,
	SET_UB_IMG_VERSIONA_VALUE,
	SET_UB_IMG_VERSIONB_VALUE,
	SET_UB_IMG_VALIDA_VALUE,
	SET_UB_IMG_VALIDB_VALUE,
	SET_UB_IMG_TYPE_VALUE,
	__SET_UB_MAX
};

enum {
	GET_UB_ACTIVE_BANK_VALUE,
	GET_UB_COMMIT_BANK_VALUE,
	GET_UB_IMG_VERSIONA_VALUE,
	GET_UB_IMG_VERSIONB_VALUE,
	GET_UB_IMG_VALIDA_VALUE,
	GET_UB_IMG_VALIDB_VALUE,
	GET_UB_IMG_TYPE_VALUE,
	__GET_UB_MAX
};


static const struct blobmsg_policy img_activate_policy[__IMG_ACTIVATE_MAX] = {
    [IMG_ACTIVATE_VALUE] = { .name = "bank", .type = BLOBMSG_TYPE_STRING },
};

static const struct blobmsg_policy write_image_policy[__WRITE_IMG_MAX] = {
    [IMAGE_NAME_VALUE] = { .name = "image_name", .type = BLOBMSG_TYPE_STRING },
};

static const struct blobmsg_policy set_uboot_env_policy[__SET_UB_MAX] = {
    [SET_UB_COMMIT_BANK_VALUE] = { .name = "commit_bank", .type = BLOBMSG_TYPE_STRING },
    [SET_UB_IMG_VERSIONA_VALUE] = { .name = "img_versionA", .type = BLOBMSG_TYPE_STRING },
    [SET_UB_IMG_VERSIONB_VALUE] = { .name = "img_versionB", .type = BLOBMSG_TYPE_STRING },
    [SET_UB_IMG_VALIDA_VALUE] = { .name = "img_validA", .type = BLOBMSG_TYPE_BOOL },
    [SET_UB_IMG_VALIDB_VALUE] = { .name = "img_validB", .type = BLOBMSG_TYPE_BOOL },
    [SET_UB_IMG_TYPE_VALUE] = { .name = "img_type", .type = BLOBMSG_TYPE_INT32 },
};

static const struct blobmsg_policy get_uboot_var_policy[__GET_UB_MAX] = {
    [GET_UB_ACTIVE_BANK_VALUE] = { .name = "active_bank", .type = BLOBMSG_TYPE_STRING },
    [GET_UB_COMMIT_BANK_VALUE] = { .name = "commit_bank", .type = BLOBMSG_TYPE_STRING },
    [GET_UB_IMG_VERSIONA_VALUE] = { .name = "img_versionA", .type = BLOBMSG_TYPE_STRING },
    [GET_UB_IMG_VERSIONB_VALUE] = { .name = "img_versionB", .type = BLOBMSG_TYPE_STRING },
    [GET_UB_IMG_VALIDA_VALUE] = { .name = "img_validA", .type = BLOBMSG_TYPE_STRING },
    [GET_UB_IMG_VALIDB_VALUE] = { .name = "img_validB", .type = BLOBMSG_TYPE_STRING },
    [GET_UB_IMG_TYPE_VALUE] = { .name = "img_type", .type = BLOBMSG_TYPE_STRING },
};

void sysupgrade_ubusRegister(void);
uint16_t sysupgrade_ubusInit(void);
static int sysupgrade_img_activate(struct ubus_context *ctx, struct ubus_object *obj,
	struct ubus_request_data *req, const char *method, struct blob_attr *msg);
static int sysupgrade_get_uboot_env(struct ubus_context *ctx, struct ubus_object *obj,
	struct ubus_request_data *req, const char *method, struct blob_attr *msg);
static int sysupgrade_get_uboot_param(struct ubus_context *ctx, struct ubus_object *obj,
	struct ubus_request_data *req, const char *method, struct blob_attr *msg);
static int sysupgrade_set_uboot_env(struct ubus_context *ctx, struct ubus_object *obj,
	struct ubus_request_data *req, const char *method, struct blob_attr *msg);
static int sysupgrade_write_img(struct ubus_context *ctx, struct ubus_object *obj,
	struct ubus_request_data *req, const char *method, struct blob_attr *msg);
static int sysupgrade_reboot(struct ubus_context *ctx, struct ubus_object *obj,
	struct ubus_request_data *req, const char *method, struct blob_attr *msg);

const struct ubus_method sysupgrade_methods[] = {
	UBUS_METHOD_NOARG(SYSUPGRADE_UBUS_METHOD_REBOOT,
		sysupgrade_reboot),
	UBUS_METHOD_NOARG(SYSUPGRADE_UBUS_METHOD_GET_UBOOT_ENV,
		sysupgrade_get_uboot_env),
	UBUS_METHOD(SYSUPGRADE_UBUS_METHOD_GET_UBOOT_PARAM,
		sysupgrade_get_uboot_param, get_uboot_var_policy),
	UBUS_METHOD(SYSUPGRADE_UBUS_METHOD_SET_UBOOT_ENV,
		sysupgrade_set_uboot_env, set_uboot_env_policy),
	UBUS_METHOD(SYSUPGRADE_UBUS_METHOD_WRITE_IMG,
		sysupgrade_write_img, write_image_policy),
	UBUS_METHOD(SYSUPGRADE_UBUS_METHOD_IMG_ACTIVATE,
		sysupgrade_img_activate, img_activate_policy),
};

struct ubus_object_type sysupgrade_object_type =
UBUS_OBJECT_TYPE(SYSUPGRADE_UBUS_SERVER, sysupgrade_methods);

struct ubus_object sysupgrade_object = {
	.name = SYSUPGRADE_UBUS_SERVER,
	.type = &sysupgrade_object_type,
	.methods = sysupgrade_methods,
	.n_methods = ARRAY_SIZE(sysupgrade_methods)
};

extern void *xSysupgradeExports[];

struct ubus_context *pxCtx;
static struct blob_buf blobbuf;
#define MAX_IMAGENAME_SIZE 50

/*=============================================================================
 * Function Name : sysupgrade_img_activate
 * Description   : ubus 'sysugrade-img_activate' method for sysupgrade.
 *                 Forwards the data to sysupgrade_ubusHandler
 *===========================================================================*/
static int sysupgrade_img_activate(
		struct ubus_context *ctx __attribute__((unused)),
		struct ubus_object *obj __attribute__((unused)),
		struct ubus_request_data *req __attribute__((unused)),
		const char *method __attribute__((unused)),
		struct blob_attr *msg)
{
	struct blob_attr *tb[__IMG_ACTIVATE_MAX];
	char *buf = NULL;

	blobmsg_parse(img_activate_policy, __IMG_ACTIVATE_MAX, tb, blob_data(msg), blob_len(msg));
	if (!tb[IMG_ACTIVATE_VALUE]) {
		printf("calling img_activate NULL\n");
	} else {
		buf = blobmsg_get_string(tb[IMG_ACTIVATE_VALUE]);
		if (buf)
			printf("Calling img_activate %s\n", buf);
	}
	int ret = fapi_ssActivateBank((unsigned char *)buf);
	return ret;
}

/*=============================================================================
 * Function Name : sysupgrade_get_uboot_env
 * Description   : ubus 'sysugrade-get_uboot_env' method for sysupgrade.
 *                 Forwards the data to sysupgrade_ubusHandler
 *===========================================================================*/
static int sysupgrade_get_uboot_env(struct ubus_context *ctx,
		struct ubus_object *obj __attribute__((unused)),
		struct ubus_request_data *req __attribute__((unused)),
		const char *method __attribute__((unused)),
		struct blob_attr *msg)
{
	struct blob_attr *tb[__GET_UB_MAX];
	uboot_value_t ub_val;
	blobmsg_parse(get_uboot_var_policy, __GET_UB_MAX, tb, blob_data(msg), blob_len(msg));
	blob_buf_init(&blobbuf, 0);

	memset(&ub_val, 0x0, sizeof(uboot_value_t));
	ub_val.type = STRING;
	if (fapi_ssGetUbootParam("img_activate", &ub_val) == UGW_SUCCESS)
		blobmsg_add_string(&blobbuf, "img_activate", ub_val.u.valuec);

	memset(&ub_val, 0x0, sizeof(uboot_value_t));
	ub_val.type = STRING;
	if (fapi_ssGetUbootParam("active_bank", &ub_val) == UGW_SUCCESS)
		blobmsg_add_string(&blobbuf, "active_bank", ub_val.u.valuec);

	memset(&ub_val, 0x0, sizeof(uboot_value_t));
	ub_val.type = STRING;
	if (fapi_ssGetUbootParam("commit_bank", &ub_val) == UGW_SUCCESS)
		blobmsg_add_string(&blobbuf, "commit_bank", ub_val.u.valuec);

	memset(&ub_val, 0x0, sizeof(uboot_value_t));
	ub_val.type = INTEGER;
	if (fapi_ssGetUbootParam("image_type", &ub_val) == UGW_SUCCESS)
		blobmsg_add_u32(&blobbuf, "image_type", ub_val.u.valuei);

	memset(&ub_val, 0x0, sizeof(uboot_value_t));
	ub_val.type = STRING;
	if (fapi_ssGetUbootParam("img_versionA", &ub_val) == UGW_SUCCESS)
		blobmsg_add_string(&blobbuf, "img_versionA", ub_val.u.valuec);

	memset(&ub_val, 0x0, sizeof(uboot_value_t));
	ub_val.type = STRING;
	if (fapi_ssGetUbootParam("img_versionB", &ub_val) == UGW_SUCCESS)
		blobmsg_add_string(&blobbuf, "img_versionB", ub_val.u.valuec);

	memset(&ub_val, 0x0, sizeof(uboot_value_t));
	ub_val.type = INTEGER;
	if (fapi_ssGetUbootParam("img_validA", &ub_val) == UGW_SUCCESS)
		blobmsg_add_u32(&blobbuf, "img_validA", ub_val.u.valuei);

	memset(&ub_val, 0x0, sizeof(uboot_value_t));
	ub_val.type = INTEGER;
	if (fapi_ssGetUbootParam("img_validB", &ub_val) == UGW_SUCCESS)
		blobmsg_add_u32(&blobbuf, "img_validB", ub_val.u.valuei);

	ubus_send_reply(ctx, req, blobbuf.head);

	return 0;
}

/*=============================================================================
 * Function Name : sysupgrade_get_uboot_param
 * Description   : ubus 'sysugrade-get_uboot_param' method for sysupgrade.
 *					forwards the data to sysupgrade_ubusHandler
 *===========================================================================*/
static int sysupgrade_get_uboot_param(struct ubus_context *ctx,
		struct ubus_object *obj __attribute__((unused)),
		struct ubus_request_data *req __attribute__((unused)),
		const char *method __attribute__((unused)),
		struct blob_attr *msg)
{
	struct blob_attr *tb[__GET_UB_MAX];
	char *buf = NULL;
	int ret;
	uboot_value_t ub_val;

	blobmsg_parse(get_uboot_var_policy, __GET_UB_MAX, tb, blob_data(msg), blob_len(msg));
	blob_buf_init(&blobbuf, 0);

	if (tb[GET_UB_ACTIVE_BANK_VALUE]) {
		memset(&ub_val, 0x0, sizeof(uboot_value_t));
		buf = blobmsg_get_string(tb[GET_UB_ACTIVE_BANK_VALUE]);
		if(strncmp(buf, "String", sizeof("String")-1) == 0)
		ub_val.type = STRING;
		ret = fapi_ssGetUbootParam("active_bank", &ub_val);
		if(ret == UGW_SUCCESS)
			blobmsg_add_string(&blobbuf, "active_bank", ub_val.u.valuec);
	}

	if (tb[GET_UB_COMMIT_BANK_VALUE]) {
		memset(&ub_val, 0x0, sizeof(uboot_value_t));
		buf = blobmsg_get_string(tb[GET_UB_COMMIT_BANK_VALUE]);
		ub_val.type = STRING;
		ret = fapi_ssGetUbootParam("commit_bank", &ub_val);
		if(ret == UGW_SUCCESS)
			blobmsg_add_string(&blobbuf, "commit_bank", ub_val.u.valuec);
	}

	if (tb[GET_UB_IMG_TYPE_VALUE]) {
		memset(&ub_val, 0x0, sizeof(uboot_value_t));
		buf = blobmsg_get_string(tb[GET_UB_IMG_TYPE_VALUE]);
		if(strncmp(buf, "Integer", sizeof("Integer")-1) == 0)
			ub_val.type = INTEGER;
		ret = fapi_ssGetUbootParam("image_type", &ub_val);
		if(ret == UGW_SUCCESS)
			blobmsg_add_u32(&blobbuf, "image_type", ub_val.u.valuei);
	}

	if (tb[GET_UB_IMG_VERSIONA_VALUE]) {
		memset(&ub_val, 0x0, sizeof(uboot_value_t));
		buf = blobmsg_get_string(tb[GET_UB_IMG_VERSIONA_VALUE]);
		ub_val.type = STRING;
		ret = fapi_ssGetUbootParam("img_versionA", &ub_val);
		if(ret == UGW_SUCCESS)
			blobmsg_add_string(&blobbuf, "img_versionA", ub_val.u.valuec);
	}
	if (tb[GET_UB_IMG_VERSIONB_VALUE]) {
		memset(&ub_val, 0x0, sizeof(uboot_value_t));
		buf = blobmsg_get_string(tb[GET_UB_IMG_VERSIONA_VALUE]);
		ub_val.type = STRING;
		ret = fapi_ssGetUbootParam("img_versionB", &ub_val);
		if(ret == UGW_SUCCESS)
			blobmsg_add_string(&blobbuf, "img_versionB", ub_val.u.valuec);
	}
	if (tb[GET_UB_IMG_VALIDA_VALUE]) {
		memset(&ub_val, 0x0, sizeof(uboot_value_t));
		buf = blobmsg_get_string(tb[GET_UB_IMG_VALIDA_VALUE]);
		ub_val.type = INTEGER;
		ret = fapi_ssGetUbootParam("img_validA", &ub_val);
		if(ret == UGW_SUCCESS)
			blobmsg_add_u32(&blobbuf, "img_validA", ub_val.u.valuei);
	}
	if (tb[GET_UB_IMG_VALIDB_VALUE])  {
		memset(&ub_val, 0x0, sizeof(uboot_value_t));
		buf = blobmsg_get_string(tb[GET_UB_IMG_VALIDB_VALUE]);
		ub_val.type = INTEGER;
		ret = fapi_ssGetUbootParam("img_validB", &ub_val);
		if(ret == UGW_SUCCESS)
			blobmsg_add_u32(&blobbuf, "img_validB", ub_val.u.valuei);
	}
	ubus_send_reply(ctx, req, blobbuf.head);
	return 0;
}

/*=============================================================================
 * Function Name : sysupgrade_set_uboot_env
 * Description   : ubus 'sysugrade-set_uboot_param' method for sysupgrade.
 *                 Forwards the data to sysupgrade_ubusHandler
 *===========================================================================*/
static int sysupgrade_set_uboot_env(
		struct ubus_context *ctx __attribute__((unused)),
		struct ubus_object *obj __attribute__((unused)),
		struct ubus_request_data *req __attribute__((unused)),
		const char *method __attribute__((unused)),
		struct blob_attr *msg)
{
	struct blob_attr *tb[__SET_UB_MAX];
	char *buf;
	int ret = -1;
	uboot_value_t ub_val;
	blobmsg_parse(set_uboot_env_policy, __SET_UB_MAX, tb, blob_data(msg), blob_len(msg));

	blob_buf_init(&blobbuf, 0);

	if (tb[SET_UB_COMMIT_BANK_VALUE]) {
		buf = blobmsg_get_string(tb[SET_UB_COMMIT_BANK_VALUE]);
		ub_val.type = STRING;
		ub_val.u.valuec = buf;
		ret = fapi_ssSetUbootParam("commit_bank", &ub_val);
		goto RETURN;
	} else if (tb[SET_UB_IMG_TYPE_VALUE]) {
		uint32_t image_type = blobmsg_get_u32(tb[SET_UB_IMG_TYPE_VALUE]);
		if (image_type <= 2)
			printf("image_type is %d\n", image_type);
		else {
			printf("image_type is invalid\n");
			return -1;
		}
		ub_val.type = INTEGER;
		ub_val.u.valuei = image_type;
		ret = fapi_ssSetUbootParam("image_type", &ub_val);
		goto RETURN;
	} else if (tb[SET_UB_IMG_VERSIONA_VALUE]) {
		buf = blobmsg_get_string(tb[SET_UB_IMG_VERSIONA_VALUE]);
		ub_val.type = STRING;
		ub_val.u.valuec = buf;
		ret = fapi_ssSetUbootParam("img_versionA", &ub_val);
		goto RETURN;
	} else if (tb[SET_UB_IMG_VERSIONB_VALUE]) {
		buf = blobmsg_get_string(tb[SET_UB_IMG_VERSIONB_VALUE]);
		ub_val.type = STRING;
		ub_val.u.valuec = buf;
		ret = fapi_ssSetUbootParam("img_versionB", &ub_val);
		goto RETURN;
	} else if (tb[SET_UB_IMG_VALIDA_VALUE]) {
		bool valid_image = blobmsg_get_bool(tb[SET_UB_IMG_VALIDA_VALUE]);
		ub_val.type = INTEGER;
		ub_val.u.valuei = valid_image;
		ret = fapi_ssSetUbootParam("img_validA", &ub_val);
		goto RETURN;
	} else {
		if (tb[SET_UB_IMG_VALIDB_VALUE]) {
		bool valid_image = blobmsg_get_bool(tb[SET_UB_IMG_VALIDB_VALUE]);
		ub_val.type = INTEGER;
		ub_val.u.valuei = valid_image;
		ret = fapi_ssSetUbootParam("img_validB", &ub_val);
		goto RETURN;
		} else {
			goto RETURN;
		}
	}

RETURN:
	/*TO DO Presently sending set operation failure Need to associate proper failure message send later ret */
	if(ret != UGW_SUCCESS)
	{
		ret = UBOOT_SET_OPERATION_FAIL;	
	}
	blobmsg_add_u32(&blobbuf, "retval", ret);
	ubus_send_reply(ctx, req, blobbuf.head);

	return ret;
}

static int32_t chkImage(img_param_t img_auth, char *sErrMsg)
{
	char sBuf[TEMP_BUF] = {0};
	int32_t nFd = 0, nLen = 0;
	int32_t nSize = 0, nRet = UGW_FAILURE;
	uint32_t unCrc = 0;
	image_header_t img_header;

	printf("Firmware Upgrade image checksum validation\n");

	lseek(img_auth.src_img_fd, 0, SEEK_SET);

	memset_s(&img_header, sizeof(image_header_t), 0x00, sizeof(image_header_t));
	nSize = read(img_auth.src_img_fd, &img_header, sizeof(image_header_t));
	if(nSize < 0){
		sprintf_s(sErrMsg, MAX_FILELINE_LEN - 1, "%s",
                        "Firmware Upgrade failed as the image does not contain valid header.");
		printf("Firmware upgrade failed to read the header %s\n",strerror(errno));
                nRet = UGW_FAILURE;
                goto abort;
	}

	if (ntohl(img_header.img_hdr_magic) != IMG_HDR_MAGIC) {
		sprintf_s(sErrMsg, MAX_FILELINE_LEN - 1, "%s",
			"Firmware Upgrade failed as the image does not contain valid header.");
		printf("Firmware upgrade failed magic %s\n",strerror(errno));
		nRet = UGW_FAILURE;
		goto abort;
	}

	unCrc = 0x00000000 ^ 0xffffffff;
	nLen = 0;
	while ((nLen = read(img_auth.src_img_fd, sBuf, sizeof(sBuf))) > 0) {
		unCrc = crc32(unCrc, sBuf, nLen);
		nLen = 0;
		memset_s(sBuf, sizeof(sBuf), 0, sizeof(sBuf));
	}
	unCrc ^= 0xffffffff;

	if (unCrc != ntohl(img_header.img_hdr_dcrc)) {
		sprintf_s(sErrMsg, MAX_FILELINE_LEN - 1, "%s [%u:%u]\n\n",
			"Firmware Upgrade failed as the image does not contain valid checksum.", unCrc, img_header.img_hdr_dcrc);
		nRet = UGW_FAILURE;
		printf("Firmware upgrade failed magic %s\n",strerror(errno));
		goto abort;
	}
	LOGF_LOG_DEBUG("Firmware Upgrade check pass as the image checksum match.\n");
	nRet = UGW_SUCCESS;
abort:
	if(nFd != -1)
		close(nFd);
	return nRet;
}

static void update_ImgValid(char *sImgName)
{
	int ret = UGW_SUCCESS;
	char img_ver_var[20] = {0};
	char img_ver_valid[20] = {0};
	uboot_value_t ub_val;
	char new_active_bank = '\0';

	memset(&ub_val, 0x0, sizeof(uboot_value_t));
	ub_val.type = STRING;
	if (fapi_ssGetUbootParam("active_bank", &ub_val) == UGW_SUCCESS) {
		if(ub_val.u.valuec[0] == 'A')
			new_active_bank = 'B';
		else
			new_active_bank = 'A';
		snprintf(img_ver_var, sizeof(img_ver_var), "img_version%c", new_active_bank);
		snprintf(img_ver_valid, sizeof(img_ver_valid), "img_valid%c", new_active_bank);
		printf(" %s():sImgName:%s active bank:%c img_ver_var:%s img_ver_valid:%s\n", __func__, sImgName, ub_val.u.valuec[0], img_ver_var, img_ver_valid);

		FILE* fp = fopen("/tmp/abc.txt","a+");

		fprintf(fp,"In file:%s, function:%s() and line:%d: \n",__FILE__,__func__,__LINE__);

		fclose(fp);

		memset(&ub_val, 0x0, sizeof(uboot_value_t));
		ub_val.type = STRING;
		ub_val.u.valuec = (char *)sImgName;
		ret = fapi_ssSetUbootParam(img_ver_var, &ub_val);
		if(ret == UGW_SUCCESS){
			printf("Uboot variable %s update with value: %s Success\n", img_ver_var, sImgName);
		} else {
			printf("Uboot variable %s update with value: %s Fail\n", img_ver_var, sImgName);
		}

		memset(&ub_val, 0x0, sizeof(uboot_value_t));
		ub_val.type = INTEGER;
		ub_val.u.valuei = 1;
		ret = fapi_ssSetUbootParam(img_ver_valid, &ub_val);
		if(ret == UGW_SUCCESS){
			printf("Uboot variable %s update Success\n", img_ver_valid);
		} else {
			printf("Uboot variable %s update Fail\n", img_ver_valid);
		}
	} else {
		printf("Failed to get value of active_bank\n");
	}

	return;
}

/*=============================================================================
 * Function Name : sysupgrade_write_img
 * Description   : ubus 'sysugrade-write_img' method for sysupgrade.
 *                 Formwards the data to sysupgrade_ubusHandler
 *===========================================================================*/
static int sysupgrade_write_img(struct ubus_context *ctx,
		struct ubus_object *obj __attribute__((unused)),
		struct ubus_request_data *req __attribute__((unused)),
		const char *method __attribute__((unused)),
		struct blob_attr *msg)
{
	struct blob_attr *tb[__WRITE_IMG_MAX];
	char pcFileName[MAX_IMAGENAME_SIZE] = {0};
	char *bl_buf = NULL, sErrMsg[MAX_FILELINE_LEN] = {0};
	char buf[MAX_IMAGENAME_SIZE];
	image_header_t x_img_header;
	int ret = 0,nRet = UGW_SUCCESS;;
	int nFd = -1;
	img_param_t img = {0};
	struct stat filesize = {0};

	blobmsg_parse(write_image_policy, __WRITE_IMG_MAX, tb, blob_data(msg), blob_len(msg));
	blob_buf_init(&blobbuf, 0);
	if (tb[IMAGE_NAME_VALUE]) {
		bl_buf = blobmsg_get_string(tb[IMAGE_NAME_VALUE]);
		sprintf_s(buf,sizeof(buf),"%s",bl_buf);
	} else {
		sprintf_s(buf,sizeof(buf),"%s",FIRMWARE_IMG);
	}

	if (sprintf_s(pcFileName, MAX_IMAGENAME_SIZE, "/tmp/upgrade/%s", buf) <= 0) {
		LOGF_LOG_ERROR("sprintf_s buffer failed reason(%s)\n", strerror(errno));
		return UGW_FAILURE;
	}
	LOGF_LOG_DEBUG("pcFileName %s\n", pcFileName);

	nFd = open(pcFileName, O_RDWR);
	if (nFd < 0) {
		LOGF_LOG_DEBUG("The file %s could not be opened", pcFileName);
		return -1;
	}
	if (fstat(nFd, &filesize)) {
		LOGF_LOG_DEBUG("fstat error: [%s]\n", strerror(errno));
		close(nFd);
		return -1;
	}
	img.src_img_fd = nFd;
	img.src_img_len = filesize.st_size;

	printf("Debug file size %ld\n",img.src_img_len);
	strncpy_s(img.img_name, sizeof(img.img_name), buf, strnlen_s(buf,MAX_IMAGENAME_SIZE));	
	printf("filepath %s img.img_name %s\n",buf,img.img_name);

	nRet = chkImage(img,sErrMsg);
	if (nRet == UGW_FAILURE) {
		printf("Firmware image verification : failed [%s].\n", sErrMsg);
		blobmsg_add_u32(&blobbuf, "retval", IMAGE_VALIDATION_FAILED);
		ubus_send_reply(ctx, req, blobbuf.head);
		close(nFd);
		return -1;
	}

	img.write_to_upg_part = 1;
	img.src_img_addr = mmap(0, img.src_img_len, (PROT_READ|PROT_WRITE),
							MAP_SHARED, img.src_img_fd, 0);

	if (img.src_img_addr == MAP_FAILED) {
		PRINT("MMAP failed... %s", strerror(errno));
		close(nFd);
		return -1;
	}

	x_img_header = *((image_header_t *)img.src_img_addr);

	if((x_img_header.img_hdr_type != IMG_HDR_VAR_MULTI))
	{
		fprintf(stderr, "Unknown image type!!\n");
		return -1;
	}

	ret = fapi_ssImgAuth(img);
	switch (ret) {
	case IMAGE_VALIDATION_FAILED:
		LOGF_LOG_DEBUG("Image validation failed");
		blobmsg_add_u32(&blobbuf, "retval", IMAGE_VALIDATION_FAILED);
		break;
	case IMAGE_WRITE_FAILED:
		LOGF_LOG_DEBUG("Image write failed");
		blobmsg_add_u32(&blobbuf, "retval", IMAGE_WRITE_FAILED);
		ubus_send_reply(ctx, req, blobbuf.head);
		break;
	case UGW_SUCCESS:
		LOGF_LOG_DEBUG("Success");
		blobmsg_add_u32(&blobbuf, "retval", UGW_SUCCESS);
		update_ImgValid((char *)x_img_header.img_hdr_name);
		fapi_ssSetUdt((unsigned char *)buf, 1);
		ubus_send_reply(ctx, req, blobbuf.head);
		break;
	case IMAGE_UPGSTATE_ERROR:
		LOGF_LOG_DEBUG("upg state error");
		blobmsg_add_u32(&blobbuf, "retval", IMAGE_UPGSTATE_ERROR);
		ubus_send_reply(ctx, req, blobbuf.head);
		break;
	default:
		LOGF_LOG_DEBUG("Unknown failure");
		blobmsg_add_u32(&blobbuf, "retval", ret);
	}

	if (munmap(img.src_img_addr, img.src_img_len) == -1) {
		PRINT("munmap failed");
	}

	if (nFd >= 0) {
		close(nFd);
	}
	return 0;
}

/*=============================================================================
 * Function Name : sysupgrade_reboot
 * Description   : ubus 'sysugrade-reboot' method for sysupgrade.
 *                 Forwards the data to sysupgrade_ubusHandler
 *===========================================================================*/
static int sysupgrade_reboot(struct ubus_context *ctx __attribute__((unused)),
		struct ubus_object *obj __attribute__((unused)),
		struct ubus_request_data *req __attribute__((unused)),
		const char *method __attribute__((unused)),
		struct blob_attr *msg __attribute__((unused)))
{
	printf("calling system call for reboot\n");
	return reboot(LINUX_REBOOT_CMD_RESTART);
}

void sysupgrade_ubusRegister(void)
{
	int nRet;
	LOGF_LOG_DEBUG("Registering sysupgrade to ubus\n");

	nRet = ubus_add_object(pxCtx, &sysupgrade_object);
	if (nRet)
		fprintf(stderr, "Failed to add object: %s\n", ubus_strerror(nRet));

}

static void sysupgrade_ubusReconnectTimer(
	struct uloop_timeout *timeout __attribute__((unused)))
{
	const char *pcUbusSocket = NULL;
	static struct uloop_timeout retry = {
		.cb = sysupgrade_ubusReconnectTimer,
	};
	int t = 2;

	if (ubus_reconnect(pxCtx, pcUbusSocket) != 0) {
		LOGF_LOG_INFO("Failed to reconnect, trying again in %d seconds\n", t);
		uloop_timeout_set(&retry, t * 1000);
		return;
	}

	LOGF_LOG_INFO("Reconnected to ubus, new id: %08x\n", pxCtx->local_id);
	ubus_add_uloop(pxCtx);
}

static void sysupgrade_ubusConnectionLost(
	struct ubus_context *pxCtI __attribute__((unused)))
{
	sysupgrade_ubusReconnectTimer(NULL);
}

uint16_t sysupgrade_ubusInit(void)
{
	const char *pcUbusSocket = NULL;
	pxCtx = ubus_connect(pcUbusSocket);
	if (!pxCtx) {
		LOGF_LOG_CRITICAL("Unable to connect to ubus.. Exiting..\n");
		return UGW_FAILURE;
	}

	pxCtx->connection_lost = sysupgrade_ubusConnectionLost;

	ubus_add_uloop(pxCtx);

	return UGW_SUCCESS;
}

void sysupgrade_ubusDone(void)
{
	ubus_free(pxCtx);
}
