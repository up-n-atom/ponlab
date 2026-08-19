/*****************************************************************************
 *
 *  Copyright (c) 2016 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
/**
 * \file
 * This file holds the PON library error code definitions.
 */

#ifndef _FAPI_PON_ERROR_H_
#define _FAPI_PON_ERROR_H_

/** \addtogroup PON_FAPI_REFERENCE
 *   @{
 */
/** \defgroup PON_FAPI_ERROR PON Library Error Codes
 *  This chapter describes the error codes of the PON library functions.
 *   @{
 */

/** Enumeration for function status return. */
enum fapi_pon_errorcode {
	/** No error. */
	PON_STATUS_OK =			    0,
	/** No ACK or NACK is sent to the firmware by an event,
	 *  the data handled is just logged.
	 */
	PON_STATUS_OK_NO_RESPONSE = -101,
	/** A generic or unknown error has occurred. */
	PON_STATUS_ERR =		-1001,
	/** A memory error has occurred. */
	PON_STATUS_MEM_ERR =		-1002,
	/** At least one value is out of range. */
	PON_STATUS_VALUE_RANGE_ERR =	-1003,
	/** The requested resource is not available. */
	PON_STATUS_RESOURCE_ERR =	-1004,
	/** The allocation ID (T-CONT ID) to be added already exists. */
	PON_STATUS_ALLOC_ID_EXISTS =	-1005,
	/** The allocation ID (T-CONT ID) to be deleted does not exist. */
	PON_STATUS_ALLOC_ID_MISSING =	-1006,
	/** The QoS port index to be assigned is already in use. */
	PON_STATUS_QOS_PORT_INDEX_ERR =	-1007,
	/** The requested functionality is not supported. */
	PON_STATUS_SUPPORT =		-1008,
	/** The firmware message was not acknowledged by the firmware. */
	PON_STATUS_FW_NACK =		-1009,
	/** The software received an unexpected message from the firmware. */
	PON_STATUS_FW_UNEXPECTED =	-1010,
	/** The calling software received an error from the mailbox driver.  */
	PON_STATUS_MBOX_ERR =		-1011,
	/** Did not receive an answer in time.  */
	PON_STATUS_TIMEOUT =		-1012,
	/** NACK from firmware, because adding of a duplicate entry has been
	 *  rejected.
	 */
	PON_STATUS_FW_DUP_ERR =		-1013,
	/** NACK from firmware, because PON IP debug mode is disabled. */
	PON_STATUS_FW_DBG =		-1014,
	/** Duplicate GEM port entry. */
	PON_STATUS_DUPLICATE_GEM_ERR =	-1015,
	/** Error in the PON IP message header. */
	PON_STATUS_FW_CMDERR =		-1016,
	/** A netlink operation returned an error. */
	PON_STATUS_NL_ERR =		-1017,
	/** AN unexpected netlink message has been received. */
	PON_STATUS_NL_MSG =		-1018,
	/** One of the input parameters is wrong. */
	PON_STATUS_INPUT_ERR =		-1019,
	/** Cannot resolve the pon mailbox netlink name. */
	PON_STATUS_NL_NAME_ERR =	-1020,
	/** The mailbox driver returned -ENOMEM. */
	PON_STATUS_MBOX_ENOMEM =	-1021,
	/** The mailbox driver returned -EINVAL. */
	PON_STATUS_MBOX_EINVAL =	-1022,
	/** The mailbox driver returned -ENODEV. */
	PON_STATUS_MBOX_ENODEV =	-1023,
	/** Data setting error occurred. */
	PON_STATUS_DATA_SET_ERR =	-1024,
	/** The GEM port ID does not exist. */
	PON_STATUS_GEM_PORT_ID_NOT_EXISTS_ERR = -1025,
	/** NACK from firmware, because the PON IP firmware is in a wrong
	 *  state.
	 */
	PON_STATUS_FW_STATE =		-1026,
	/** The PON IP trace module is not ready. */
	PON_STATUS_TRACE_MODULE_NOT_READY = -1027,
	/** File open error. */
	PON_STATUS_EEPROM_OPEN_ERR = -1028,
	/** File seek error. */
	PON_STATUS_EEPROM_SEEK_ERR = -1029,
	/** File read error. */
	PON_STATUS_EEPROM_READ_ERR = -1030,
	/** File write error. */
	PON_STATUS_EEPROM_WRITE_ERR = -1031,
	/** Operation mode error. */
	PON_STATUS_OPERATION_MODE_ERR = -1032,
	/** Local wake-up indication by software control is disabled. */
	PON_STATUS_LWI_SW_CONTROL_DISABLE = -1033,
	/** An unexpected burst profile status rate has been chosen. */
	PON_STATUS_BURST_PROFILE_RATE_ERR = -1034,
	/** Not enough memory to copy data. */
	PON_STATUS_MEM_NOT_ENOUGH = -1035,
	/** No GEM port mapped to the allocation ID (T-CONT ID) is present. */
	PON_STATUS_ALLOC_GEM_MAP_ERR = -1036,
	/** PRBS upstream for optical calibration is disabled. */
	PON_STATUS_PRBS_DISABLE = -1037,
	/** Wrong Synchronous Ethernet operation state. */
	PON_STATUS_SYNCE_STATE_ERR = -1038,
	/** OMCI Integrity Key is invalid. */
	PON_STATUS_OMCI_IK_ERR = -1039,
	/** Internal error returned by memory copy function */
	PON_STATUS_MEMCPY_ERR = -1100
};

/*! @} */ /* PON_FAPI_ERROR */

/*! @} */ /* PON_FAPI_REFERENCE */

#endif /* _FAPI_PON_ERROR_H_ */
