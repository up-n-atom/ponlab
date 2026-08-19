/******************************************************************************
 *
 *  Copyright (c) 2022 - 2025 MaxLinear, Inc.
 *  Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
/**
 * \file
 *  This is the PON library core header file, used for debug purposes
 *  and non-interface functions.
 */

#ifndef _FAPI_PON_CORE_H_
#define _FAPI_PON_CORE_H_

#ifdef HAVE_CONFIG_H
#  include "pon_config.h"
#endif
#include "pon_mbox.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef LINUX
#  include <sys/socket.h>
#  include <sys/select.h>
#  include <errno.h>
#endif /* LINUX */
#ifdef WIN32
#  include <winsock2.h>
#  define gmtime_r(time, tm) (gmtime_s((tm), (time)) == 0)
#endif /* WIN32 */
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>

#define UNUSED(x) (void)(x)

/** \addtogroup PON_FAPI_REFERENCE
 *   @{
 */
/** \defgroup PON_FAPI_CORE PON Library Core Functions
 *   These are the PON library core functions.
 * @{
 */

/* PON enumeration definitions */
/* ========================= */

/** PON debug levels */
enum pon_debug_level {
	/** Message level, print everything. */
	PON_DBG_MSG = 0,
	/** Print level. */
	PON_DBG_PRN = 1,
	/** Warning level, print warnings and errors only. */
	PON_DBG_WRN = 2,
	/** Error level, print errors only. */
	PON_DBG_ERR = 3,
	/** Off, do not print anything. */
	PON_DBG_OFF = 4
};

/* PON structure definitions */
/* ========================= */

/** Structure for debug level selection. The debug level defines, which
 *  additional printouts are sent to the command interface.
 */
struct pon_dbg_level {
	/** Debug level.
	 *	- 0: Message level, print everything
	 *	- 1: Warning level, print warnings and errors only.
	 *	- 2: Error level, print errors only.
	 *	- 3: Debug output is disabled.
	 *	- Others: Reserved, do not use.
	 */
	enum pon_debug_level level;
};

/** PON library handle structure.
 *  Used by \ref fapi_pon_open and \ref fapi_pon_close.
 */
struct pon_ctx {
	/** Private data of the user application. */
	void *priv;
	/** Netlink socket. */
	struct nl_sock *nls;
	/** Netlink socket for event handling. */
	struct nl_sock *nls_event;
	/** Netlink Family number. */
	int family;
	/** Callback handler for XGTC message log request. */
	fapi_pon_get_xgtc_log xgtc_log;
	/** Callback handler for GTC message log request. */
	fapi_pon_get_gtc_log gtc_log;
	/** Callback handler for PLOAM STATE message log request. */
	fapi_pon_get_ploam_state ploam_state;
	/** Callback handler for GTC/XGTC alarm report request. */
	fapi_pon_alarm_report alarm_report;
	/** Callback handler for GTC/XGTC alarms clear request. */
	fapi_pon_alarm_report alarm_clear;
	/** Callback handler for firmware init information. */
	fapi_pon_fw_init_complete fw_init_complete;
	/** Callback handler for XGTC power level request. */
	fapi_pon_get_xgtc_power_level xgtc_power_level;
	/** Callback handler for ONU TOD SYNC message log request. */
	fapi_pon_get_onu_tod_sync onu_tod_sync;
	/** Callback handler for TWDM wavelength switch check */
	fapi_pon_twdm_wl_check twdm_wl_check;
	/** Callback handler for TWDM configuration request */
	fapi_pon_twdm_wl_config twdm_wl_conf;
	/** Callback handler for TWDM tuning request */
	fapi_pon_twdm_us_wl_tuning twdm_wl_tun;
	/** Callback handler for TWDM channel profile */
	fapi_pon_twdm_ch_profile twdm_ch_profile;
	/** Callback handler for TWDM config event */
	fapi_pon_twdm_config twdm_config;
	/** Callback handler for calibration record state */
	fapi_pon_get_cal_record_state pon_cal_record_state;
	/** Callback handler for synchronous Ethernet status request */
	fapi_pon_synce_status synce_status;
	/** Callback handler for ONU random challenge table */
	fapi_pon_onu_rnd_chl_tbl onu_rnd_chl_tbl;
	/** Callback handler for ONU authentication result table */
	fapi_pon_onu_auth_res_tbl onu_auth_res_tbl;
	/** Callback handler for unlink all request */
	fapi_pon_unlink_all unlink_all;
	/** File descriptor to EEPROM data. */
	int eeprom_fd[PON_DDMI_MAX];
	/** Cache for FW capabilities information */
	struct pon_cap caps_data;
	/** Set to 1 if cached capabilities value is valid */
	int caps_valid;
	/** Cache for FW version information */
	struct pon_version ver_data;
	/** Set to 1 if cached version value is valid */
	int ver_valid;
	/** Cache for GEM port and allocation limits */
	struct pon_range_limits limits_data;
	/** Set to 1 if cached limits values are valid */
	int limits_valid;
	/** Cache for PON mode information */
	enum pon_mode mode;
	/** Set to 1 if cached PON mode value is valid */
	int mode_valid;
	/** Platform type as established by a PONIP FW FW_VERSION msg. readout,
	 *  if yet 0 it is invalid
	 */
	int actual_plat_type;
	/** Cache for optic external calibration type */
	bool ext_calibrated;
	/** Set to 1 if optic external calibration type value is valid */
	int ext_cal_valid;
};

/* PON FAPI function definitions */
/* ============================= */

/**
 *	Type definition of the callback function to be implemented to get the
 *	answer to a message which has been sent to the firmware.
 *
 *	\param[in] ctx PON FAPI context
 *	\param[in] data Data of the received message
 *	\param[in] data_size Size of the data pointer
 *	\param[in] priv Pointer to private data given to the function with
 *			the callback.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
typedef enum fapi_pon_errorcode (*fapi_pon_copy)(struct pon_ctx *ctx,
						 const void *data,
						 size_t data_size,
						 void *priv);

/**
 *	Type definition of the callback function to be implemented to get the
 *	acknowledge code when some error occurred and we did not received
 *	a ACK for a request from the firmware.
 *
 *	\param[in] ctx PON FAPI context
 *	\param[in] ack data received in ACK field.
 *	\param[in] priv Pointer to private data given to the function with
 *			the callback.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
typedef enum fapi_pon_errorcode (*fapi_pon_error)(struct pon_ctx *ctx,
						  uint8_t ack, void *priv);

/**
 *	Decodes Netlink attributes
 *
 *	\param[in] ctx PON FAPI context
 *	\param[in] attrs Netlink attributes to parse
 *	\param[in] priv User data given for a Netlink callback
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
typedef enum fapi_pon_errorcode (*fapi_pon_decode)(struct pon_ctx *ctx,
						   struct nlattr **attrs,
						   void *priv);

/**
 *	Function to retrieve the PON module information.
 *
 *	\param[in] ctx PON FAPI context created by \ref fapi_pon_open.
 *	\param[in] command Number representing used command.
 *	\param[in] in_buf Pointer to a structure used to write information.
 *	\param[in] in_size Number representing the size of the structure used to
 *		write information.
 *	\param[in] copy Callback function which converts the data from
 *		the firmware format into the FAPI format.
 *	\param[in] error_cb Callback function which gets called in case a
 *		NACK is received from the firmware. Set this to NULL to use the
 *		default handler.
 *	\param[in] copy_priv Private data given to the copy callback function.
 *	\param[in] msg_type Type of Netlink message to send.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_generic_error_get(struct pon_ctx *ctx,
						   uint32_t command,
						   const void *in_buf,
						   size_t in_size,
						   fapi_pon_copy copy,
						   fapi_pon_error error_cb,
						   void *copy_priv,
						   uint8_t msg_type);

/**
 *	Function to retrieve the PON module information.
 *
 *	\param[in] ctx PON FAPI context created by \ref fapi_pon_open.
 *	\param[in] command Number representing used command.
 *	\param[in] in_buf Pointer to a structure used to write information.
 *	\param[in] in_size Number representing the size of the structure used to
 *		write information.
 *	\param[in] copy Callback function which converts the data from
 *		the firmware format into the FAPI format.
 *	\param[in] copy_priv Private data given to the copy callback function.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
static inline
enum fapi_pon_errorcode fapi_pon_generic_get(struct pon_ctx *ctx,
					     uint32_t command,
					     const void *in_buf,
					     size_t in_size,
					     fapi_pon_copy copy,
					     void *copy_priv)
{
	return fapi_pon_generic_error_get(ctx, command, in_buf, in_size, copy,
					  NULL, copy_priv, PON_MBOX_C_MSG);
}

/**
 *	Function to set the PON module information.
 *
 *	\param[in] ctx PON FAPI context created by \ref fapi_pon_open.
 *	\param[in] command Number representing used command.
 *	\param[in] param Pointer to a structure used to write information.
 *	\param[in] sizeof_param Number representing the size of the structure
 *		used to write information.
 *	\param[in] error_cb Callback function which gets called in case a
 *		NACK is received from the firmware. Set this to NULL to use the
 *		default handler.
 *	\param[in] copy_priv Private data given to the copy callback function.
 *	\param[in] msg_type Type of Netlink message to send.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_generic_error_set(struct pon_ctx *ctx,
						   uint32_t command,
						   const void *param,
						   uint32_t sizeof_param,
						   fapi_pon_error error_cb,
						   void *copy_priv,
						   uint8_t msg_type);

/**
 *	Function to set the PON module information.
 *
 *	\param[in] ctx PON FAPI context created by \ref fapi_pon_open.
 *	\param[in] command Number representing used command.
 *	\param[in] param Pointer to a structure used to write information.
 *	\param[in] sizeof_param Number representing the size of the structure
 *		used to write information.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
static inline
enum fapi_pon_errorcode fapi_pon_generic_set(struct pon_ctx *ctx,
					     uint32_t command,
					     const void *param,
					     uint32_t sizeof_param)
{
	return fapi_pon_generic_error_set(ctx, command, param, sizeof_param,
					  NULL, NULL, PON_MBOX_C_MSG);
}

/**
 *	Send an Netlink answer to an earlier received message
 *
 *	\param[in] ctx Handler Containing information about the current state
 *	\param[in] msg Original message received earlier
 *	\param[in] attrs Pointer to the attributes of the previous message
 *	\param[in] ack Ack type to send
 *	\param[in] buf Buffer with the message to write
 *	\param[in] size Size of buf
 *	\param[in] msg_type Type of Netlink message to send
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
	fapi_pon_send_msg_answer(struct pon_ctx *ctx, struct nl_msg *msg,
				 struct nlattr **attrs, uint32_t ack, void *buf,
				 size_t size, uint8_t msg_type);

/**
 *	Receives all events received from the firmware and does the message
 *	specific handling.
 *
 *	\param[in] command Command id of the received message
 *	\param[in] ctx Handler containing information about the current state
 *	\param[in] msg Message received
 *	\param[in] attrs Pointer to the attributes of the message
 */
void fapi_pon_listener_msg(uint16_t command, struct pon_ctx *ctx,
			   struct nl_msg *msg, struct nlattr **attrs);

/**
 *	Receives the "firmware initialization complete" message from the
 *	mailbox driver.
 *
 *	\param[in] ctx Handler containing information about the current state
 *	\param[in] msg Message received
 *	\param[in] attrs Pointer to the attributes of the message
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_fw_init_complete_msg(struct pon_ctx *ctx,
			      struct nl_msg *msg,
			      struct nlattr **attrs);

/**
 *	Default NACK handler method. This functions converts a NACK from the
 *	firmware into a PON library error code.
 *
 *	\param[in] ack ACK or NACK received from the firmware
 *
 *	\return Return value as follows:
 *	- PON_STATUS_FW_NACK: Got NACK or CMD error from firmware
 *	- PON_STATUS_DUPLICATE_ERR: Got NACK from firmware, duplicate entry
 *	- PON_STATUS_FW_DBG: Got NACK from firmware, debug deactivated
 *	- PON_STATUS_ERR: Unknown error
 */
enum fapi_pon_errorcode pon_handle_error_default(uint8_t ack);

/**	This structure provides the information to the Netlink callback
 *	handlers, and is also used by the Netlink callback handler,
 *	to provide information to the calling function.
 *	The Netlink callback handlers are executed when an answer,
 *	to the given Netlink request is received.
 */
struct read_cmd_cb {
	/** Callback status
	 *  - 1: The callback handler was not called yet.
	 *  - 0: The callback handler was called and this structure was filled,
	 *  and the copy or error_cb function was executed.
	 */
	int running;
	/** The error code filled by the callback handler */
	enum fapi_pon_errorcode err;
	/** Function to call in case of a successful answer
	 *  from the PON IP firmware to handle the data.
	 */
	fapi_pon_copy copy;
	/** Function to call in case of a successful answer
	 *  from the PON IP firmware to interpret Netlink attributes.
	 */
	fapi_pon_decode decode;
	/** Function which gets called in case the PON IP firmware returned an
	 *  error, to handle this error case.
	 */
	fapi_pon_error error_cb;
	/** The PON context */
	struct pon_ctx *ctx;
	/** Private data provided to the copy,
	 *  decode and the error_cb function
	 */
	void *priv;
};

/** Netlink message preparation */
enum fapi_pon_errorcode fapi_pon_nl_msg_prepare(struct pon_ctx *ctx,
						struct nl_msg **msg,
						struct read_cmd_cb *cb_data,
						uint32_t *seq,
						fapi_pon_copy copy,
						fapi_pon_error error_cb,
						void *copy_priv,
						uint8_t msg_type);

/** Netlink message decoding preparation */
enum fapi_pon_errorcode
fapi_pon_nl_msg_prepare_decode(struct pon_ctx *ctx,
			       struct nl_msg **msg,
			       struct read_cmd_cb *cb_data,
			       uint32_t *seq,
			       fapi_pon_decode decode,
			       fapi_pon_error error_cb,
			       void *copy_priv,
			       uint8_t msg_type);

/** Send a Netlink message */
enum fapi_pon_errorcode fapi_pon_nl_msg_send(struct pon_ctx *ctx,
					     struct nl_msg **msg,
					     struct read_cmd_cb *cb_data,
					     uint32_t *seq);

/** Message preparation */
enum fapi_pon_errorcode fapi_pon_msg_prepare(struct pon_ctx **ctx,
					     struct nl_msg **msg,
					     uint8_t cmd);

/**
 * \brief Get clock cycle from PON IP capabilities
 *
 * \param[in] caps pointer to capability bitmap
 *
 * \return 0 on NULL pointer provided, GPON_CLOCK_CYCLE on success
 */
int get_clock_cycle_from_caps(struct pon_cap *caps);

/**
 * \brief Copy bytes and set it in correct endianness
 *
 * \param[in] dst Pointer to the destination data where content is to be copied
 * \param[in] src Pointer to the source of data to be copied
 * \param[in] size Number of bytes to copy
 */
void pon_byte_copy(uint8_t *dst, const uint8_t *src, int size);

/*! @} */ /* PON_FAPI_CORE */

/*! @} */ /* PON_FAPI_REFERENCE */

#endif /* _FAPI_PON_CORE_H_ */
