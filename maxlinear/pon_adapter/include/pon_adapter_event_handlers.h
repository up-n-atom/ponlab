/******************************************************************************
 *
 *  Copyright (c) 2022 MaxLinear, Inc.
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_event_handlers.h
 *
 * This is the PON adapter event handlers header file, defining event callbacks.
 */

#ifndef _PON_ADAPTER_EVENT_HANDLERS_H_
#define _PON_ADAPTER_EVENT_HANDLERS_H_

#include <stdbool.h>
#include "pon_adapter_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 *
 *  @{
 */

/** PON Adapter Event Handler Operations */
struct pa_eh_ops {
	/** Send alarm status message to OLT for the chosen Managed Entity.
	 *
	 * \param[in]	caller		Caller context pointer.
	 * \param[in]	class_id	Managed Entity class id.
	 * \param[in]	instance_id	Managed Entity instance id.
	 * \param[in]	alarm		Triggered alarm.
	 * \param[in]	active		Alarm status.
	 */
	void (*alarm)(void *caller, uint16_t class_id, uint16_t instance_id,
		      unsigned int alarm, bool active);

	/** Send optic alarm status message to OLT.
	 *
	 * \param[in]	caller		Caller context pointer.
	 * \param[in]	alarm		Triggered alarm. Intended to use
	 *				"enum pon_adapter_ani_g_alarm".
	 * \param[in]	active		Alarm status.
	 */
	void (*optic_alarm)(void *caller, int alarm,
			    bool active);

	/** Callback for omci_msg event
	 *  \remarks: This event is obsolete. There will be a dedicated handler
	 *	      for OMCI messages in PON Adapter
	 *
	 * \param[in]	caller		Caller context pointer.
	 * \param[in]	msg		Pointer to raw message.
	 * \param[in]	len		Length of raw message.
	 */
	void (*omci_msg)(void *caller, const uint8_t *msg, const uint16_t len);

	/** Update software ploam state
	 *
	 * \param[in]	caller		Caller context pointer.
	 * \param[in]	prev_state	Previous ploam state.
	 * \param[in]	curr_state	Current ploam state.
	 */
	void (*ploam_state)(void *caller, int prev_state, int curr_state);

	/** Callback for performance monitor interval_end event
	 *
	 * \param[in]	caller			Caller context pointer.
	 * \param[in]	interval_end_time	???.
	 */
	void (*interval_end)(void *caller, uint8_t interval_end_time);

	/** Callback for link_state event
	 *
	 * \param[in]	caller		Caller context pointer.
	 * \param[in]	instance_id	Managed Entity instance id(PPTP_UNI).
	 * \param[in]	state		Link state.
	 * \param[in]	config_ind	???.
	 */
	void (*link_state)(void *caller, uint16_t instance_id, bool state,
			   uint8_t config_ind);

	/** Callback for net_state event
	 *
	 * \param[in]	caller		Caller context pointer.
	 * \param[in]	iface_name	Interface name.
	 * \param[in]	iface_up	Interface status.
	 */
	void (*net_state)(void *caller, const char *iface_name,
			  const bool iface_up);

	/** Callback for link_init event
	 *
	 * \param[in]	caller		Caller context pointer.
	 * \param[in]	instance_id	Managed Entity instance id(PPTP_UNI).
	 * \param[in]	is_initialized	Link initialization status.
	 */
	void (*link_init)(void *caller, uint16_t instance_id,
			  bool is_initialized);

	/** Callback for loop_detect event
	 *
	 * \param[in]	caller		Caller context pointer.
	 * \param[in]	instance_id	Managed Entity instance id(PPTP_UNI).
	 */
	void (*loop_detect)(void *caller, uint16_t instance_id);

	/** The ONU random challenge table is ready and can be read.
	 *
	 * \param[in]	caller		Caller context pointer.
	 * \param[in]	onu_rnd_chl_tbl	ONU random challenge table
	 * \param[in]	len		Data size in bytes
	 */
	void (*onu_rnd_chl_tbl)(void *caller, const uint8_t *onu_rnd_chl_tbl,
				uint16_t len);

	/** The ONU authentication result table is ready and can be read.
	 *
	 * \param[in]	caller		Caller context pointer.
	 * \param[in]	onu_auth_result	ONU authentication result
	 * \param[in]	len		Data size in bytes
	 */
	void (*auth_result_rdy)(void *caller, const uint8_t *onu_auth_result,
				uint16_t len);

	/** The mutual authentication status has changed.
	 *
	 * \param[in]	caller		Caller context pointer.
	 * \param[in]	status		For mutual authentication status only.
	 */
	void (*auth_status_chg)(void *caller, uint32_t status);

	/** OMCI Integrity Key has changed.
	 *
	 * \param[in]	caller		Caller context pointer.
	 * \param[in]	omci_ik		Updated OMCI integrity key.
	 */
	void (*omci_ik_update)(void *caller, const struct pa_omci_ik *omci_ik);

	/** Callback to trigger a complete MIB reset
	 *
	 * \param[in]	caller		Caller context pointer.
	 */
	void (*mib_reset)(void *caller);

	/** Callback to EPON_LINK_EVENT
	 * \param[in]	caller		Caller context pointer.
	 * \param[in]	link_index	Link index
	 * \param[in]	llid		LLID
	 * \param[in]	status		Status
	 * \param[in]	status_prev	Previous status
	 */
	void (*epon_link)(void *caller, uint32_t link_index, uint32_t llid,
			  uint32_t status, uint32_t status_prev);

	/** Callback to EPON_STATE_EVENTS
	 * \param[in]	caller		Caller context pointer.
	 * \param[in]	prev_counter	Time duration of the previous EPON
	 *				state, given in multiples of 125 µs
	 * \param[in]	state_act	Current EPON ONU state
	 * \param[in]	state_prev	Previous EPON ONU state
	 */
	void (*epon_state)(void *caller, uint32_t prev_counter,
			   uint32_t state_act, uint32_t state_prev);

	/** Callback to trigger a Channel Partition Index (CPI) value in OMCI
	 * reset to 0
	 * \remarks: This event is only used by NG-PON2
	 *
	 * \param[in]	caller		Caller context pointer.
	 */
	void (*ch_partition_index_reset)(void *caller);

	/** Callback to trigger a channel profile update
	 * \remarks: This event is only used by NG-PON2
	 *
	 * \param[in]	caller		Caller context pointer.
	 */
	void (*twdm_ch_profile_update)(void *caller, bool is_active,
				       uint8_t ch_index);

	/** Callback to inform about PON FW alarm
	 * \remarks: This event is used by the ubus HAL interface
	 *
	 * \param[in]	caller		Caller context pointer.
	 * \param[out]	alarm_id	Each alarm is identified by a unique ID.
	 * \param[out]	alarm_status	Alarm status.
	 */
	void (*pon_alarm)(void *caller, uint16_t alarm_id,
			  uint8_t alarm_status);
};

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif /* _PON_ADAPTER_EVENT_HANDLERS_H_ */
