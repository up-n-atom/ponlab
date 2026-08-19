/******************************************************************************
 *
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_errno.h
 *
 * This is the PON adapter error header file, defining a list of error codes.
 */

#ifndef _PON_ADAPTER_ERRNO_H_
#define _PON_ADAPTER_ERRNO_H_

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 *
 *  @{
 */

/** PON adapter error numbering */
enum pon_adapter_errno {
	/** Status unchanged */
	PON_ADAPTER_EUNCHANGED					= 2,
	/** Try again */
	PON_ADAPTER_EAGAIN					= 1,
	/** Success */
	PON_ADAPTER_SUCCESS					= 0,
	/** Unspecific error */
	PON_ADAPTER_ERROR					= -1,
	/** Resource was not found */
	PON_ADAPTER_ERR_NOT_FOUND				= -2,
	/** Resource was not available */
	PON_ADAPTER_ERR_NOT_AVAIL				= -3,
	/** Memory allocation error */
	PON_ADAPTER_ERR_NO_MEMORY				= -4,
	/** The requested functionality is not supported */
	PON_ADAPTER_ERR_NOT_SUPPORTED				= -5,
	/** No data is available */
	PON_ADAPTER_ERR_NO_DATA					= -6,
	/** Configuration mismatch */
	PON_ADAPTER_ERR_CONFIG_MISMATCH				= -7,
	/** The resource exists */
	PON_ADAPTER_ERR_RESOURCE_EXISTS				= -8,
	/** The resource is not available */
	PON_ADAPTER_ERR_RESOURCE_NOT_AVAIL			= -9,
	/** The resource has not been found */
	PON_ADAPTER_ERR_RESOURCE_NOT_FOUND			= -10,
	/** A match has not been found (by MCC) */
	PON_ADAPTER_ERR_MATCH_NOT_FOUND				= -11,
	/** Invalid attribute value */
	PON_ADAPTER_ERR_INVALID_VAL				= -12,
	/** Driver request was not executed */
	PON_ADAPTER_ERR_DRV					= -13,
	/** OMCI message FIFO is full */
	PON_ADAPTER_ERR_OMCI_MSG_FIFO_FULL			= -14,
	/** Managed Entity ID is invalid */
	PON_ADAPTER_ERR_OMCI_ME_INVALID				= -15,
	/** Managed Entity was not found */
	PON_ADAPTER_ERR_OMCI_ME_NOT_FOUND			= -16,
	/** Managed Entity already exists */
	PON_ADAPTER_ERR_OMCI_ME_EXISTS				= -17,
	/** OMCI Action Error according ITU-T G.984.4 II.1.3 */
	PON_ADAPTER_ERR_OMCI_ACTION				= -18,
	/** Managed Entity class is not supported */
	PON_ADAPTER_ERR_OMCI_ME_NOT_SUPPORTED			= -19,
	/** Managed Entity attribute position is out of range or invalid */
	PON_ADAPTER_ERR_OMCI_ME_ATTR_INVALID			= -20,
	/** Managed Entity action is out of range or not supported */
	PON_ADAPTER_ERR_OMCI_ME_ACTION_INVALID			= -21,
	/** Locking error */
	PON_ADAPTER_ERR_LOCKING					= -22,
	/** Pointer is invalid */
	PON_ADAPTER_ERR_PTR_INVALID				= -23,
	/** Parameters are out of bounds */
	PON_ADAPTER_ERR_OUT_OF_BOUNDS				= -24,
	/** The interface implementation was not found */
	PON_ADAPTER_ERR_IF_NOT_FOUND				= -25,
	/** The size check was not successful */
	PON_ADAPTER_ERR_SIZE					= -26,
	/** The CRC check was not successful */
	PON_ADAPTER_ERR_CRC					= -27,
	/** Invalid access to memory (access to null pointer, memory overlap,
	 * incorrect size etc.) */
	PON_ADAPTER_ERR_MEM_ACCESS				= -28,
	/** OMCI Message received with invalid TCI header */
	PON_ADAPTER_ERR_OMCI_MSG_INVALID_TCI			= -29,
};

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif /* _PON_ADAPTER_ERRNO_H_ */
