/******************************************************************************
 *
 *  Copyright (c) 2020 - 2021 MaxLinear, Inc.
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * ONU-G.
 */

#ifndef _pon_adapter_me_onu_g_h
#define _pon_adapter_me_onu_g_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_ONU_G ONU-G
 *
 *  This Managed Entity represents the ONU as equipment. The ONU automatically
 *  creates an instance of this Managed Entity. It assigns values to read-only
 *  attributes according to data within the ONU itself.
 *
 *  @{
 */

/** The ONU's QoS is priority controlled. */
#define PA_TMO_PRIO 0
/** The ONU's QoS is rate controlled. */
#define PA_TMO_RATE 1
/** The ONU's QoS is priority and rate controlled. */
#define PA_TMO_PRIO_RATE 2

/** Equipment alarm */
#define PA_ALARM_ID_ONUG_EQUIPMENT 0
/** Powering alarm */
#define PA_ALARM_ID_ONUG_POWER 1
/** Battery missing */
#define PA_ALARM_ID_ONUG_BATT_MISSING 2
/** Battery failure */
#define PA_ALARM_ID_ONUG_BATT_FAIL 3
/** Battery low */
#define PA_ALARM_ID_ONUG_BATT_LOW 4
/** Physical intrusion */
#define PA_ALARM_ID_ONUG_INTRUSION 5
/** ONU self-test failure */
#define PA_ALARM_ID_ONUG_SELF_TEST_FAIL 6
/** Dying gasp */
#define PA_ALARM_ID_ONUG_DYING_GASP 7
/** Temperature yellow */
#define PA_ALARM_ID_ONUG_TEMP_YELLOW 8
/** Temperature red */
#define PA_ALARM_ID_ONUG_TEMP_RED 9
/** Voltage yellow */
#define PA_ALARM_ID_ONUG_VOLTAGE_YELLOW 10
/** Voltage red */
#define PA_ALARM_ID_ONUG_VOLTAGE_RED 11
/** ONU manual power off */
#define PA_ALARM_ID_ONUG_MANUAL_OFF 12
/** Software image is invalid */
#define PA_ALARM_ID_ONUG_INV_IMAGE 13

/** Flag to enable support for ITU-T G.984.3 Annex C, PON-ID maintenance */
#define TC_LAYER_OPTIONS_BIT_1	(0x1)
/** Flag to enable support for ITU-T G.984.3 Annex D, PLOAM channel
 *  enhancements: swift_POPUP and Ranging_adjustment messages.
 */
#define TC_LAYER_OPTIONS_BIT_2	(0x2)
/** Combined flag mask of supported TC-Layer options. */
#define TC_LAYER_OPTIONS (TC_LAYER_OPTIONS_BIT_1 | TC_LAYER_OPTIONS_BIT_2)

/** ONU-G operations structure */
struct pa_onu_g_ops {
	/** Enable Managed Entity identifier mapping to driver index and
	 *  initialize corresponding driver structures.
	 *
	 *  \param[in] ll_handle      Lower layer context pointer
	 *  \param[in] me_id          Managed Entity identifier
	 *  \param[in] battery_backup Battery backup
	 *  \param[in] traff_mgmt_opt Traffic management option
	 */
	enum pon_adapter_errno (*create)(void *ll_handle,
					 uint16_t me_id,
					 uint8_t battery_backup,
					 uint8_t traff_mgmt_opt);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures.
	 *
	 *  \param[in] ll_handle     Lower layer context pointer
	 *  \param[in] me_id         Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle,
					  uint16_t me_id);

	/** Updates ONU-G Managed Entity resources.
	 *
	 *  \param[in] ll_handle      Lower layer context pointer
	 *  \param[in] me_id          Managed Entity identifier
	 *  \param[in] battery_backup Battery backup
	 *  \param[in] traff_mgmt_opt Traffic management option
	 */
	enum pon_adapter_errno (*update)(void *ll_handle,
					 uint16_t me_id,
					 uint8_t battery_backup,
					 uint8_t traff_mgmt_opt);

	/** Get ONU-G operational state.
	 *
	 *  \param[in]  ll_handle      Lower layer context pointer
	 *  \param[out] oper_state     Operational state (Enabled/Disabled)
	 */
	enum pon_adapter_errno (*oper_state_get)(void *ll_handle,
						 uint8_t *state);

	/** Get the extended TC-layer options.
	 *
	 *  \param[in]  ll_handle      Lower layer context pointer
	 *  \param[out] bitmask        Specified TC layer options
	 */
	enum pon_adapter_errno (*ext_tc_opt_get)(void *ll_handle,
						 uint16_t *bitmask);

	/** Reset counters, start a new 15-min interval and start counting
	 *  intervals from zero.
	 *
	 *  \param[in] ll_handle      Lower layer context pointer
	 */
	enum pon_adapter_errno (*sync_time)(void *ll_handle);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
