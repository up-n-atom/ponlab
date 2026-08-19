/******************************************************************************
 *
 * Copyright (c) 2022 - 2023 MaxLinear, Inc.
 * Copyright (c) 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/
/**
   \file pon_img_register.h
   This is the PON ADAPTER WRAPPER header file.
*/

#ifndef _PON_IMG_REGISTER_H_
#define _PON_IMG_REGISTER_H_

#include <pon_adapter.h>
#include <pon_adapter_errno.h>

/** \addtogroup PON_IMG_LIB
 *  @{
 */

/** Status information for currently active SW download. */
struct pon_image_info {
	/** File descriptor for image download file before flash storage */
	int fd;
	/** Image size */
	uint32_t size;
	/** Image offset */
	uint32_t offset;
	/** temp crc */
	uint32_t crc;
	/** next window number which shall be handled */
	uint32_t next_window;
};

/** Private information for pon_img_lib */
struct pon_img_context {
	/** SW image handle to support Software Download */
	struct pon_image_info image;

	/** Callbacks to access uci and ubus */
	const struct pa_config *pa_config;

	/** OMCI context */
	void *hl_handle;

	/** Time of last update of U-Boot vars.
	 * Can be set to 0 to drop cached data.
	 */
	time_t last_ubus_ubootvars;

	/** Currently detected UBUS path (fwupgrade/system) */
	const char *ubus_path;

	/** Flag to indicate only the reboot is supported via ubus */
	bool ubus_reboot_only;
};

/**
 * Register lower layer functions in higher layer module.
 *
 * \param[in] hl_handle_legacy Pointer must be set to NULL.
 * \param[out] pa_ops     Pointer to lower layer operations structure.
 * \param[out] ll_handle  Pointer to lower layer module.
 * \param[in] hl_handle   Pointer to higher layer module.
 * \param[in] if_version  PON-Adapter IF version used by calling function.
 *
 * \remarks The function returns an error code in case of error.
 * The error code is described in \ref pon_adapter_errno.
 *
 * \return Return value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
libponimg_ll_register_ops(void *hl_handle_legacy,
			  const struct pa_ops **pa_ops,
			  void **ll_handle,
			  void *hl_handle,
			  uint32_t if_version);

/** @} */

#endif /* _PON_IMG_REGISTER_H_ */
