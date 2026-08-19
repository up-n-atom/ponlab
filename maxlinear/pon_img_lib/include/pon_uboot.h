/******************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 * Copyright (c) 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef _PON_UBOOT_H_
#define _PON_UBOOT_H_

#include <pon_adapter.h>
#include <pon_img_register.h>

/** \addtogroup PON_IMG_LIB
 *  @{
 */

/** Maximum supported length of U-Boot variable value */
#define UBOOT_VAL_LEN_MAX 64

/** Predefined name for U-Boot image active status */
#define UBOOT_VAR_IMG_ACTIVE "active_bank"
/** Predefined name for U-Boot image activation */
#define UBOOT_VAR_IMG_ACTIVATE "img_activate"
/** Predefined name for U-Boot image committing */
#define UBOOT_VAR_IMG_COMMIT "commit_bank"
/** Predefined name for U-Boot image version storage */
#define UBOOT_VAR_IMG_VERSION "img_version"
/** Predefined name for U-Boot image validity storage */
#define UBOOT_VAR_IMG_VALID "img_valid"
/** Status value representing validity of image */
#define UBOOT_VAL_IMG_VALID true
/** Status value representing invalidity of image */
#define UBOOT_VAL_IMG_INVALID false

/**	Function to write a U-Boot variable with specified string value.
 *
 *	\param[in] name		U-Boot variable name
 *	\param[in] value	U-Boot variable value
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno pon_uboot_set_str(struct pon_img_context *ctx,
					 const char *name, const char *value);

/**	Function to write a U-Boot variable with specified boolean value.
 *
 *	\param[in] name		U-Boot variable name
 *	\param[in] value	U-Boot variable value
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno pon_uboot_set_bool(struct pon_img_context *ctx,
					  const char *name, bool value);

/**	Function to read a U-Boot variable to specified value buffer.
 *
 *	\param[in] name		U-Boot variable name
 *	\param[in] value	U-Boot variable buffer for value
 *	\param[in] value_size	U-Boot variable buffer size for value
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno pon_uboot_get(struct pon_img_context *ctx,
				     const char *name, char *value,
				     const unsigned int value_size);

/** @} */

#endif /* _PON_UBOOT_H_ */
