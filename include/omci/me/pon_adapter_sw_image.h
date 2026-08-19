/******************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * Software Image.
 */

#ifndef _pon_adapter_me_sw_image_h
#define _pon_adapter_me_sw_image_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_SW_IMAGE Software Image
 *
 *  This Managed Entity models an executable software image stored in the ONU.
 *  The ONU automatically creates two instances of this Managed Entity upon the
 *  creation of each Managed Entity that contains independently-manageable
 *  software, either the ONU itself or an individual circuit pack.
 *
 *  The Managed Entity attributes are populated according to data within the
 *  ONU or the circuit pack.
 *
 *  Some pluggable equipment types may contain no software. Others may contain
 *  software that is intrinsically bound to the ONU's own software image. No
 *  software image Managed Entity need exist for such equipment, though it may
 *  be convenient for the ONU to create them to support software version audit
 *  from the OLT. In this case, the dependent Managed Entities would support
 *  only the get action.
 *  A slot may contain various types of equipment over its lifetime, and if
 *  software image Managed Entities exist, the ONU must automatically create and
 *  delete them as the equipage changes. When controller packs are duplicated,
 *  each can be expected to contain two software image Managed Entities,
 *  managed through reference to the individual controller packs themselves.
 *  When this occurs, the ONU should not have a global pair of software images
 *  Managed Entities (instance 0), since an action (download, activate, commit)
 *  directed to instance 0 would be ambiguous.
 *
 *  @{
 */

/** SW Image operations structure */
struct pa_sw_image_ops {
	/** Preparation of image download
	 *
	 *  \param[in] ll_handle        Lower layer context pointer
	 *  \param[in] id               SW image id
	 *  \param[in] size             Size of image to download
	 */
	enum pon_adapter_errno (*download_start)(void *ll_handle,
						 const uint8_t id,
						 const uint32_t size);

	/** Stop image download, eg. by error or timeout.
	 *
	 *  \param[in] ll_handle        Lower layer context pointer
	 *  \param[in] id               SW image id
	 */
	enum pon_adapter_errno (*download_stop)(void *ll_handle,
						const uint8_t id);

	/** Check CRC and size of the image. If image is ready to store,
	 *  file name/path of temp image file is returned
	 *
	 *  \param[in] ll_handle        Lower layer context pointer
	 *  \param[in] id               SW image id
	 *  \param[in] size             Size of downloaded image to check
	 *  \param[in] crc              Image CRC to check
	 *  \param[in] filepath_size    Supported temp file path size
	 *  \param[out] filepath        Temp file path
	 */
	enum pon_adapter_errno (*download_end)(void *ll_handle,
					       const uint8_t id,
					       const uint32_t size,
					       const uint32_t crc,
					       const uint8_t filepath_size,
					       char *filepath);

	/** Concatenate a Window to the image under download.
	 *
	 *  \param[in] ll_handle        Lower layer context pointer
	 *  \param[in] id               SW image id
	 *  \param[in] window_nr        Number of Window to handle
	 *  \param[in] window           Pointer to Window byte array
	 *  \param[in] length           Length of Window byte array
	 */
	enum pon_adapter_errno (*handle_window)(void *ll_handle,
						const uint8_t id,
						const uint32_t window_nr,
						const uint8_t *window,
						const uint16_t length);

	/**** flash/uboot vars related operations ****/

	/** Store a referenced image into flash.
	 *
	 *  \param[in] ll_handle        Lower layer context pointer
	 *  \param[in] id               SW image id
	 *  \param[in] filepath_size    Supported temp file path size
	 *  \param[in] filepath         Temp file path
	 */
	enum pon_adapter_errno (*store)(void *ll_handle,
					const uint8_t id,
					const uint8_t filepath_size,
					const char *filepath);

	/** Get the valid status of a (stored) image.
	 *
	 *  \param[in] ll_handle        Lower layer context pointer
	 *  \param[in] id               SW image id
	 *  \param[out] valid           Valid flag of the image
	 */
	enum pon_adapter_errno (*valid_get)(void *ll_handle,
					    const uint8_t id,
					    uint8_t *valid);

	/** Get the version of a (stored) image.
	 *
	 *  \param[in] ll_handle        Lower layer context pointer
	 *  \param[in] id               SW image id
	 *  \param[in] version_size     Supported version string size
	 *  \param[out] version         Version string
	 */
	enum pon_adapter_errno (*version_get)(void *ll_handle,
					      const uint8_t id,
					      const uint8_t version_size,
					      char *version);

	/** Commit a (stored) image.
	 *
	 *  \param[in] ll_handle        Lower layer context pointer
	 *  \param[in] id               SW image id
	 */
	enum pon_adapter_errno (*commit)(void *ll_handle,
					 const uint8_t id);

	/** Get the committed status of a (stored) image.
	 *
	 *  \param[in] ll_handle        Lower layer context pointer
	 *  \param[in] id               SW image id
	 *  \param[out] committed       Commit flag of the image
	 */
	enum pon_adapter_errno (*commit_get)(void *ll_handle,
					     const uint8_t id,
					     uint8_t *committed);

	/** Activate = boot a (stored) image.
	 *
	 *  \param[in] ll_handle        Lower layer context pointer
	 *  \param[in] id               SW image id
	 *  \param[in] timeout          Reboot timeout in ms
	 */
	enum pon_adapter_errno (*activate)(void *ll_handle,
					   const uint8_t id,
					   const uint32_t timeout);

	/** Get the activation status of a (stored) image.
	 *
	 *  \param[in] ll_handle        Lower layer context pointer
	 *  \param[in] id               SW image id
	 *  \param[out] active          Active flag of the image
	 */
	enum pon_adapter_errno (*active_get)(void *ll_handle,
					     const uint8_t id,
					     uint8_t *active);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
