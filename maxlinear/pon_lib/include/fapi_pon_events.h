/******************************************************************************
 *
 *  Copyright (c) 2022 - 2024 MaxLinear, Inc.
 *  Copyright (c) 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
******************************************************************************/

/**
 *  \file
 *  This is the PON library header file, defining the API functions
 *  and data structures for controlling the PON IP module events.
 */

#ifndef _FAPI_PON_EVENTS_H_
#define _FAPI_PON_EVENTS_H_

#include <stdbool.h>
#include "fapi_pon_gpon.h"

struct pon_ctx;

/** \addtogroup PON_FAPI_REFERENCE
 *   @{
 */

/** \defgroup PON_FAPI_REFERENCE_EVENT PON Library Event Handling
 *  This chapter describes the functions needed to handle events sent by
 *  the firmware through the mailbox driver.
 *
 *  @{
 */

/*	An application can use this interface like this:
 *
 *	\code
 *	enum fapi_pon_errorcode foo_rand(void *priv, void *random,
 *					 size_t size)
 *	{
 *		code handling random callback
 *	}
 *
 *	void bar(void)
 *	{
 *		struct pon_ctx *fapi_ctx;
 *		enum fapi_pon_errorcode ret;
 *
 *		ret = fapi_pon_open(&fapi_ctx);
 *		ret = fapi_pon_listener_connect(fapi_ctx, NULL);
 *		fapi_pon_register_random(fapi_ctx, foo_rand);
 *		while (true) {
 *			ret = fapi_pon_listener_run(fapi_ctx);
 *			if (ret != PON_STATUS_OK)
 *				break;
 *		}
 *		fapi_pon_close(fapi_ctx);
 *	}
 *	\endcode
 *   @{
 */

/**
 *	Connects the context to the event group.
 *
 *	All states are stored in the \ref pon_ctx
 *	structure, it is possible to use multiple of these in one application.
 *	Each thread which wants to handle events should use its own
 *	\ref pon_ctx structure context, the structure itself is not
 *	thread safe, but one instance per thread can be used independently.
 *	This is not bound to a thread.
 *
 *	Please call \ref fapi_pon_listener_run to actually listen to events.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] priv Private data of the application calling this function,
 *		which will be given to each callback call. The PON library does
 *		not care about the contents, this can be NULL.
 *
 *	\return Returns a pointer to the context in which the callback
 *		functions can be registered.
 */
#ifndef SWIG
enum fapi_pon_errorcode fapi_pon_listener_connect(struct pon_ctx *ctx,
						  void *priv);
#endif

/**
 *	Execute the listening for messages.
 *
 *	This function blocks until it handles the first message and then
 *	returns. Call it in an endless loop.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open., in
 *		addition \ref fapi_pon_listener_connect has to be called before.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode fapi_pon_listener_run(struct pon_ctx *ctx);
#endif

/**
 *	Type definition for the function to be called when the firmware
 *	requests random data.
 *
 *	\param[in] priv Pointer to private data given
 *		in \ref fapi_pon_listener_connect
 *	\param[out] random Pointer to allocated memory of size bytes
 *		to be filled with random data.
 *	\param[in] size Size of the random data pointer
 *
 *	\return Returns a value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
typedef enum fapi_pon_errorcode (*fapi_pon_get_random)(void *priv,
						       void *random,
						       size_t size);

/**
 *	Registers a function which should be called when the firmware
 *	requests a random number.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_get_random
 *
 *	\return Returns function which was previously registered as random
 *		function or NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_get_random fapi_pon_register_random(struct pon_ctx *ctx,
					     fapi_pon_get_random func);
#endif

/**
 *	Typedef for the function to be called when the firmware requests the
 *	PLOAM XGTC message log.
 *	This applies to all 10G ITU PON modes.
 *
 *	\param[in] priv Pointer to private data given
 *	in \ref fapi_pon_listener_connect
 *	\param[out] xgtc_log Pointer to the structure to be filled with PLOAM
 *		XGTC message log data.
 *
 */
typedef void (*fapi_pon_get_xgtc_log)(void *priv,
				const struct pon_xgtc_ploam_message *xgtc_log);

/**
 *	Registers a function which should be called when the firmware
 *	requests a PLOAM XGTC message log.
 *	This applies to all 10G ITU PON modes.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_get_xgtc_log
 *
 *	\return Returns function which was previously registered as PLOAM XGTC
 *	message log function or NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_get_xgtc_log
	fapi_pon_register_xgtc_log(struct pon_ctx *ctx,
				   fapi_pon_get_xgtc_log func);
#endif

/**
 *	Type definition for the function to be called when the firmware requests
 *	PLOAM GTC message log.
 *	This applies to GPON mode only.
 *
 *	\param[in] priv Pointer to private data given
 *	in \ref fapi_pon_listener_connect
 *	\param[out] gtc_log Pointer to the structure to be filled with PLOAM
 *		GTC message log data.
 */
typedef void (*fapi_pon_get_gtc_log)(void *priv,
				const struct pon_gtc_ploam_message *gtc_log);

/**
 *	Registers a function which should be called when the firmware
 *	PLOAM GTC message log.
 *	This applies to GPON Mode only.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_get_gtc_log
 *
 *	\return Returns function which was previously registered as PLOAM GTC
 *	message log function or NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_get_gtc_log
	fapi_pon_register_gtc_log(struct pon_ctx *ctx,
				  fapi_pon_get_gtc_log func);
#endif

/**
 *	Type definition for the function to be called when the firmware requests
 *	PLOAM STATE message log.
 *	This applies to all ITU PON modes.
 *
 *	\param[in] priv Pointer to private data given
 *	in \ref fapi_pon_listener_connect
 *	\param[out] ploam_state Pointer to the structure to be filled with
 *		PLOAM STATE message log data.
 */
typedef void (*fapi_pon_get_ploam_state)(void *priv,
				const struct pon_ploam_state_evt *ploam_state);

/**
 *	Registers a function which should be called when the firmware
 *	requests a PLOAM STATE message log.
 *	This applies to all ITU PON modes.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_get_ploam_state
 *
 *	\return Returns function which was previously registered as PLOAM STATE
 *	message log function or NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_get_ploam_state
	fapi_pon_register_ploam_state(struct pon_ctx *ctx,
				      fapi_pon_get_ploam_state func);
#endif

/**
 *	Type definition for the function to be called when the firmware requests
 *	ONU TOD SYNC message log.
 *
 *	This is only called when sending the ONU TOD SYNC message is
 *	activated with \ref fapi_pon_1pps_event_enable
 *
 *	\param[in] priv Pointer to private data given
 *	in \ref fapi_pon_listener_connect
 *	\param[out] onu_tod_sync Pointer to the structure to be filled with ONU
 *		TOD SYNC message log data.
 *
 *	\return Returns a value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
typedef enum fapi_pon_errorcode (*fapi_pon_get_onu_tod_sync)(void *priv,
					struct pon_gpon_tod_sync *onu_tod_sync);

/**
 *	Registers a function which should be called when the firmware
 *	requests a ONU TOD SYNC message log.
 *
 *	Call \ref fapi_pon_1pps_event_enable to activate sending the
 *	ONU TOD SYNC message and use \ref fapi_pon_1pps_event_disable to
 *	disable sending this message again. This is independent from
 *	registering the callback.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_get_onu_tod_sync
 *
 *	\return Returns function which was previously registered as ONU TOD SYNC
 *	message log function or NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_get_onu_tod_sync
	fapi_pon_register_onu_tod_sync(struct pon_ctx *ctx,
				       fapi_pon_get_onu_tod_sync func);
#endif

/**
 *	Type definition for the function to be called when the firmware sends
 *	an alarms report.
 *
 *	\param[in] priv Pointer to private data given
 *	in \ref fapi_pon_listener_connect
 *	\param[out] alarms Pointer to the structure to be filled with
 *		alarms report data.
 */
typedef void (*fapi_pon_alarm_report)(void *priv,
				      const struct pon_alarm_status *alarms);

/**
 *	Registers a function which should be called when the firmware
 *	sends an alarms report.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_alarm_report
 *
 *	\return Returns function which was previously registered as GTC/XGTC
 *	alarms report function or NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_alarm_report
	fapi_pon_register_alarm_report(struct pon_ctx *ctx,
				       fapi_pon_alarm_report func);
#endif

/**
 *	Registers a function which should be called when the firmware
 *	sends an alarm clear indication.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_alarm_report
 *
 *	\return Returns function which was previously registered as GTC/XGTC
 *	alarms clear function or NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_alarm_report
	fapi_pon_register_alarm_clear(struct pon_ctx *ctx,
				      fapi_pon_alarm_report func);
#endif

/**
 *	Type definition for the function to be called when the firmware
 *	initialization is complete.
 *
 *	\param[in] priv Pointer to private data given
 *		in \ref fapi_pon_listener_connect
 *
 *	\return Returns a value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
typedef enum fapi_pon_errorcode (*fapi_pon_fw_init_complete)(void *priv);

/**
 *	Registers a function which should be called when the firmware
 *	initialization is complete.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_fw_init_complete
 *
 *	\return Returns function which was previously registered as handler
 *	which gets informed when the initialization is complete or
 *	NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_fw_init_complete
	fapi_pon_register_fw_init_complete(struct pon_ctx *ctx,
					   fapi_pon_fw_init_complete func);
#endif

/**
 *	Type definition for the function to be called when the firmware requests
 *	a change of the XGTC power level.
 *
 *	\param[in] priv Pointer to private data given
 *	in \ref fapi_pon_listener_connect
 *	\param[out] oper Operation Type.
 *	\param[out] attenuation Attenuation Level.
 *
 *	\return Returns a value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
typedef enum fapi_pon_errorcode (*fapi_pon_get_xgtc_power_level)(void *priv,
					 uint32_t oper,
					 uint32_t *attenuation);

/**
 *	Registers a function which should be called when the firmware
 *	requests a change of the XGTC power level.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_get_xgtc_power_level
 *
 *	\return Returns function which was previously registered as XGTC power
 *	level function or NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_get_xgtc_power_level
	fapi_pon_register_xgtc_power_level(struct pon_ctx *ctx,
					   fapi_pon_get_xgtc_power_level func);
#endif

/**
 *	Type definition for the function which can be called to check
 *	if wavelength switching to given channel is possible
 *
 *	\param[in] priv PON FAPI wrapper context
 *	\param[in] oper_type Operation Type \ref pon_twdm_oper_type
 *	\param[in] ch_id Requested new upstream or downstream channel id
 *	\param[in] execute True when channel id change is required
 *
 *	\return Returns a value as follows:
 *	- PON_STATUS_OK: If the given channel id can be used
 *	- PON_STATUS_SUPPORT: If the given channel id do not match
 *			      the allowed channel mask
 *	- PON_STATUS_VALUE_RANGE_ERR: If the US channel id is different that
 *				      already used DS channel id
 */
#ifndef SWIG
typedef enum fapi_pon_errorcode (*fapi_pon_twdm_wl_check)
				(void *priv,
				 const enum pon_twdm_oper_type oper_type,
				 uint8_t ch_id, bool execute);
#endif

/**
 *	Registers a function which should be called when the functions
 *	for change of the upstream or downstream wavelength configuration
 *	needs to check if the change is possible/allowed
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_twdm_wl_check
 *
 *	\return Returns a function which was previously registered as TWDM WL
 *		check function or NULL, if no function was registered before.
 */
#ifndef SWIG
fapi_pon_twdm_wl_check fapi_pon_register_twdm_wl_check(
						struct pon_ctx *ctx,
						fapi_pon_twdm_wl_check func);
#endif

/**
 *	Type definition for the function which should be called when the
 *	firmware requests a change of the upstream or downstream wavelength
 *	configuration.
 *
 *	\param[in] priv PON FAPI wrapper context.
 *	\param[in] oper_type Operation type.
 *	\param[in] ch_id Requested new upstream/downstream channel id.
 *
 *	\return Returns a value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
typedef enum fapi_pon_errorcode (*fapi_pon_twdm_wl_config)(void *priv,
				   const enum pon_twdm_oper_type oper_type,
				   uint8_t ch_id);
#endif

/**
 *	Registers a function which should be called when the firmware
 *	requests a change of the upstream or downstream wavelength
 *	configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_twdm_wl_config
 *
 *	\return Returns a function which was previously registered as TWDM WL
 *		config function or NULL, if no function was registered before.
 */
#ifndef SWIG
fapi_pon_twdm_wl_config
	fapi_pon_register_twdm_wl_config(struct pon_ctx *ctx,
					 fapi_pon_twdm_wl_config func);
#endif

/**
 *	Type definition for the function which should be called when the
 *	firmware requests upstream wavelength tuning.
 *
 *	\param[in] priv PON FAPI wrapper context.
 *
 *	\return Returns a value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
typedef enum fapi_pon_errorcode (*fapi_pon_twdm_us_wl_tuning)(void *priv);
#endif

/**
 *	Registers a function which should be called when the firmware
 *	requests upstream wavelength tuning.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_twdm_us_wl_tuning
 *
 *	\return Returns function which was previously registered as TWDM US WL
 *		tuning function or NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_twdm_us_wl_tuning
	fapi_pon_register_twdm_us_wl_tuning(struct pon_ctx *ctx,
					    fapi_pon_twdm_us_wl_tuning func);
#endif

/**
 *	Type definition for the function to be called when the firmware sends
 *	a notification about changed synchronous Ethernet settings.
 *
 *	\param[in] priv Pointer to private data given
 *	in \ref fapi_pon_listener_connect
 *	\param[in] param Pointer to the structure to be filled
 *		in pon_synce_status data.
 */
typedef void (*fapi_pon_synce_status)(void *priv,
				const struct pon_synce_status *param);

/**
 *	Registers a function which should be called when the firmware
 *	sends a notification about changed synchronous Ethernet settings.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_synce_status
 *
 *	\return Returns function which was previously registered as callback
 *	function or NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_synce_status fapi_pon_register_synce_status(struct pon_ctx *ctx,
						fapi_pon_synce_status func);
#endif

/**
 *	Type definition for the function to be called when the firmware requests
 *	the XGTC_ONU_RND_CHAL_TABLE message handling.
 *	This applies to all ITU PON modes.
 *
 *	\param[in] priv Pointer to private data given
 *	in \ref fapi_pon_listener_connect
 *	\param[out] onu_rnd_chl_tbl Pointer to the structure to be filled with
 *		XGTC_ONU_RND_CHAL_TABLE message log data.
 */
typedef void
(*fapi_pon_onu_rnd_chl_tbl)(void *priv,
		const struct pon_generic_auth_table *onu_rnd_chl_tbl);

/**
 *	Type definition for the function to be called when the firmware requests
 *	the PONFW_XGTC_ONU_AUTH_RESULT_TABLE message handling.
 *	This applies to all ITU PON modes.
 *
 *	\param[in] priv Pointer to private data given
 *	in \ref fapi_pon_listener_connect
 *	\param[out] onu_auth_res_tbl Pointer to the structure to be filled with
 *		XGTC_ONU_AUTH_RESULT_TABLE message log data.
 */
typedef void
(*fapi_pon_onu_auth_res_tbl)(void *priv,
		const struct pon_generic_auth_table *onu_auth_res_tbl);

/**
 *	Registers a function that shall be called when the firmware
 *	requests a XGTC_ONU_RND_CHAL_TABLE message log.
 *	This applies to all ITU PON modes.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_onu_rnd_chl_tbl
 *
 *	\return Returns the function that was previously registered as
 *	XGTC_ONU_RND_CHAL_TABLE message handling function or
 *	NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_onu_rnd_chl_tbl
	fapi_pon_register_onu_rnd_chl_tbl(struct pon_ctx *ctx,
					  fapi_pon_onu_rnd_chl_tbl func);
#endif

/**
 *	Registers a function that shall be called when the firmware
 *	requests a PONFW_XGTC_ONU_AUTH_RESULT_TABLE message log.
 *	This applies to all ITU PON modes.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_onu_auth_res_tbl
 *
 *	\return Returns the function that was previously registered as
 *	XGTC_ONU_AUTH_RESULT_TABLE message handling function or
 *	NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_onu_auth_res_tbl
	fapi_pon_register_onu_auth_res_tbl(struct pon_ctx *ctx,
					   fapi_pon_onu_auth_res_tbl func);
#endif

/**
 *	Type definition for the function to be called when the firmware sends
 *	a notification to clean-up resources on operational state loss.
 *
 *	\param[in] priv Pointer to private data given
 *	in \ref fapi_pon_listener_connect
 *
 *	\return Returns a value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
typedef enum fapi_pon_errorcode (*fapi_pon_unlink_all)(void *priv);

/**
 *	Registers a function which should be called when the firmware sends
 *	a notification to clean-up resources on operational state loss.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_unlink_all
 *
 *	\return Returns the function which was previously registered as
 *	callback function or NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_unlink_all
	fapi_pon_register_unlink_all(struct pon_ctx *ctx,
				     fapi_pon_unlink_all func);
#endif

/**
 *	Type definition for the function which should be called when the
 *	firmware requests PON FAPI - Calibration record status request
 *	event handling
 *
 *	\param[in] priv   Pointer to private data given
 *			  in \ref fapi_pon_listener_connect
 *	\param[out] cal_record Pointer to the structure to be filled
 *			  in cal_record_state data.
 *	\return Returns a value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
typedef enum fapi_pon_errorcode (*fapi_pon_get_cal_record_state)
				(void *priv,
				struct pon_twdm_cal_record *cal_record);
#endif

/**
 *	Registers a function which should be called when the firmware sends
 *	a notification about the calibration record status request
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_get_cal_record_state
 *
 *	\return Returns the function which was previously registered as
 *	callback function or NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_get_cal_record_state
fapi_pon_register_cal_record_state(struct pon_ctx *ctx,
				   fapi_pon_get_cal_record_state func);
#endif

/**
 *	Type definition for the function which should be called when the
 *	firmware sends TWDM CHANNEL PROFILE message
 *	\param[in] priv   Pointer to private data given
 *			  in \ref fapi_pon_listener_connect
 *	\param[out] twdm_ch_prfl Pointer to the structure containing information
 *			  about changed twdm channel
 *	\return Returns a value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
typedef enum fapi_pon_errorcode (*fapi_pon_twdm_ch_profile)
				(void *priv,
				struct pon_twdm_channel_profile *twdm_ch_prfl);
#endif

/**
 *	Registers a function which should be called when the firmware sends
 *	a notification about the twdm channel profile
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_twdm_ch_profile
 *
 *	\return Returns the function which was previously registered as
 *	callback function or NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_twdm_ch_profile
fapi_pon_register_twdm_ch_profile(struct pon_ctx *ctx,
				  fapi_pon_twdm_ch_profile func);
#endif

/**
 *	Type definition for the function which should be called when the
 *	firmware sends TWDM CONFIG message
 *	\param[in] priv      Pointer to private data given
 *			     in \ref fapi_pon_listener_connect
 *	\param[in] cpi       Channel Partition Index
 *	\param[in] dwlch_id  Last Used Downstream Channel ID
 *
 *	\return Returns a value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
typedef enum fapi_pon_errorcode (*fapi_pon_twdm_config)
				(void *priv, uint8_t cpi, uint8_t dwlch_id);
#endif

/**
 *	Registers a function which should be called when the firmware sends
 *	a notification about the twdm config change
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] func Function of the type \ref fapi_pon_twdm_config
 *
 *	\return Returns the function which was previously registered as
 *	callback function or NULL if no function was registered before.
 */
#ifndef SWIG
fapi_pon_twdm_config
fapi_pon_register_twdm_config(struct pon_ctx *ctx,
			      fapi_pon_twdm_config func);
#endif

/*! @} */ /* End of event functions */

/*! @} */ /* End of PON library definitions */

#endif /* _FAPI_PON_EVENTS_H_ */
