/******************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 * Copyright (c) 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef _PON_IMG_H_
#define _PON_IMG_H_

#include <pon_adapter.h>
#include <pon_img_register.h>

/** \defgroup PON_IMG_LIB PON Image Library
 *  @{
 */

/**	Function to execute the image upgrade.
 *
 *	\param[in] id		Partition identifier ('A' or 'B').
 *	\param[in] filename	Image file name.
 *				The upgrade feature called via ubus is expecting
 *				a specific file, so if this is not
 *				"/tmp/upgrade/firmware.img",
 *				a copy of the referenced file will be created
 *				in that location.
 *
 *	\remark This function automatically triggers a create/re-create of
 *		partitions with given size for known image types.
 *		Be careful which partition you overwrite!
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno pon_img_upgrade(struct pon_img_context *ctx,
				       const char id, const char *filename);

/**	Function to set activate (temporary activation) status for image stored
 *	in flash at specified partition.
 *
 *	\param[in] id		Partition identifier ('A' or 'B').
 *
 *	\remark This function automatically deactivate active status of other
 *		partition!
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno pon_img_active_set(struct pon_img_context *ctx,
					  const char id);

/**	Function to get activate (temporary activation) status of image stored
 *	in flash at specified partition.
 *
 *	\param[in] id		Partition identifier ('A' or 'B').
 *	\param[out] active	Activation status.
 *				- true: if active
 *				- false: if inactive
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno  pon_img_active_get(struct pon_img_context *ctx,
					   const char id, bool *active);

/**	Function to set commit (permanent activation) status for image stored
 *	in flash at specified partition.
 *
 *	\param[in] id		Partition identifier ('A' or 'B').
 *
 *	\remark This function automatically deactivate commit status of other
 *		partition!
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno pon_img_commit_set(struct pon_img_context *ctx,
					  const char id);

/**	Function to get commit (permanent activation) status of image stored in
 *	flash at specified partition.
 *
 *	\param[in] id		Partition identifier ('A' or 'B').
 *	\param[out] committed	Commit status.
 *				- true: if committed
 *				- false: if non committed
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno pon_img_commit_get(struct pon_img_context *ctx,
					  const char id, bool *committed);

/**	Function to set version of image stored in flash at specified partition.
 *
 *	\param[in] id		Partition identifier ('A' or 'B').
 *	\param[in] buff		Buffer for version string.
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno pon_img_version_set(struct pon_img_context *ctx,
					   const char id, const char *buff);

/**	Function to get version of image stored in flash at specified partition.
 *
 *	\param[in] id		Partition identifier ('A' or 'B').
 *	\param[in] buff		Buffer for version string.
 *	\param[in] len		Length of version string.
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno pon_img_version_get(struct pon_img_context *ctx,
					   const char id, char *buff,
					   const uint8_t len);

/**	Function to set validity of image stored in flash at specified
 *	partition.
 *
 *	\param[in] id		Partition identifier ('A' or 'B').
 *	\param[in] valid	Validity of image (true|false).
 *				- true: if valid
 *				- false: if invalid
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno pon_img_valid_set(struct pon_img_context *ctx,
					 const char id, const bool valid);

/**	Function to get validity of image stored in flash at specified
 *	partition.
 *
 *	\param[in] id		Partition identifier.
 *	\param[out] valid	Validity of image (true|false).
 *				- true: if valid
 *				- false: if invalid
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno pon_img_valid_get(struct pon_img_context *ctx,
					 const char id, bool *valid);

/** @} */

#endif /* _PON_IMG_H_ */
