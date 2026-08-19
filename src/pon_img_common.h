/******************************************************************************
 *
 * Copyright (c) 2022 - 2024 MaxLinear, Inc.
 * Copyright (c) 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/
/**
   \file pon_img_common.h
*/

#ifndef _PON_IMG_COMMON_H_
#define _PON_IMG_COMMON_H_

#include <stdio.h>
#include <pon_adapter.h>

#include "pon_config.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

/** \addtogroup PON_IMG_LIB
 *  @{
 */

#define UBUS_SYSTEM_PATH			"system"
#define UBUS_METHOD_GET_UBOOTVARS	"get_uboot_env"
#define UBUS_METHOD_SET_UBOOTVAR	"set_uboot_env"
#define UBUS_METHOD_UPGRADE		"write_img"
#define UBUS_METHOD_REBOOT		"reboot"

/** Individual timeout for writing the image.
 *  In a secure environment this can take quite long.
 */
#define UBUS_TIMEOUT_UPGRADE 300000

/** default file name for upgrade image file */
#define SWIMAGE_NAME			"firmware.img"
/** default directory for upgrade image file */
#define SWIMAGE_PATH			"/tmp/upgrade/" SWIMAGE_NAME

/** Details of a image to download */
/** Reference to SW Image operations provided by this library */
extern const struct pa_sw_image_ops sw_image_ops;

struct ubus_request;
struct blob_attr;
/** Callback for ubus_call to get a "retval" */
void retval_get(struct ubus_request *req, int type, struct blob_attr *msg);

/** @} */

#endif
