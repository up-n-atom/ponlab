/******************************************************************************
 *
 *  Copyright (c) 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the TWDM Channel
 * Tuning Performance Monitoring History Data managed entity (part 1-3).
 */

#ifndef _pon_adapter_twdm_channel_tuning_pmhd_h
#define _pon_adapter_twdm_channel_tuning_pmhd_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_TWDM_CHANNEL_TUNING_PMHD TWDM Channel Tuning PMHD
 *
 * This ME collects certain tuning-control-related PM data associated with the
 * slot/circuit pack, hosting one or more ANI-G MEs, for a specific TWDM
 * channel. Instances of this ME are created and deleted by the OLT.
 * The relevant events this ME is concerned with are counted towards the PM
 * statistics associated with the source TWDM channel. The attribute
 * descriptions refer to the ONU activation cycle states and timers specified
 * in clause 12 of [ITU-T G.989.3]. This ME contains the counters characterized
 * as mandatory in clause 14 of [ITU-T G.989.3].
 *
 * An instance of this ME is associated with an instance of TWDM channel ME.
 *
 *  @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
 * instance of the TWDM Channel Entity.
 */

/** Structure to collect counters related to Tuning-related PM data.
 *  These values are used by the TWDM Channel Tuning PMHD1 managed entity.
 */
struct pa_twdm_channel_tuning_pmhd1 {
	/** Tuning control requests for Rx only or Rx and Tx
	 *
	 * The counter of received Tuning_Control PLOAM messages with Request
	 * operation code that contain tuning instructions either for receiver
	 * only or for both receiver and transmitter.
	 */
	uint64_t tcr_rx;

	/** Tuning control requests for Tx only
	 *
	 * The counter of received Tuning_Control PLOAM messages with Request
	 * operation code that contain tuning instructions for transmitter only
	 */
	uint64_t tcr_tx;

	/** Tuning control requests rejected/INT_SFC
	 *
	 * The counter of transmitted Tuning_Response PLOAM messages with NACK
	 * operation code and INT_SFC response code, indicating inability to
	 * start transceiver tuning by the specified time (SFC)
	 */
	uint64_t tcr_rej_int_sfc;

	/** Tuning control requests rejected/DS_xxx
	 *
	 * The aggregate counter of transmitted Tuning_Response PLOAM messages
	 * with NACK operation code and any DS_xxx response code, indicating
	 * target downstream wavelength channel inconsistency.
	 */
	uint64_t tcr_rej_ds;

	/** Tuning control requests rejected/US_xxx
	 *
	 * The aggregate counter of transmitted Tuning_Response PLOAM messages
	 * with NACK operation code and any US_xxx response code, indicating
	 * target upstream wavelength channel inconsistency.
	 */
	uint64_t tcr_rej_us;

	/** Tuning control requests fulfilled with ONU reacquired at target ch
	 *
	 * The counter of controlled tuning attempts for which an upstream
	 * tuning confirmation has been obtained in the target channel.
	 */
	uint64_t tcr_ful_reac;

	/** Tuning control requests failed due to
	 * target DS wavelength channel not found
	 *
	 * The counter of controlled tuning attempts that failed due to timer
	 * TO4 expiration in the DS Tuning state (O8) in the target channel.
	 */
	uint64_t tcr_fail_tar_not_fnd;

	/** Tuning control requests failed due to
	 * no feedback in target DS wavelength channel
	 *
	 * The counter of controlled tuning attempts that failed due to timer
	 * TO5 expiration in the US Tuning state (O9) in the target channel
	 */
	uint64_t tcr_fail_tar_no_fb;

	/** Tuning control requests resolved with ONU reacquired
	 * at discretionary channel
	 *
	 * The counter of controlled tuning attempts for which an upstream
	 * tuning confirmation has been obtained in the discretionary channel.
	 */
	uint64_t tcr_res_reac_disc;

	/** Tuning control requests Rollback/COM_DS
	 *
	 * The counter of controlled tuning attempts that failed due to
	 * communication condition in the target channel, as indicated by
	 * the Tuning_Response PLOAM message with Rollback operation code and
	 * COM_DS response code
	 */
	uint64_t tcr_rb_com_ds;

	/** Tuning control requests Rollback/DS_xxx
	 *
	 * The aggregate counter of controlled tuning attempts that failed due
	 * to target downstream wavelength channel inconsistency, as indicated
	 * by the Tuning_Response PLOAM message with Rollback operation code
	 * and any DS_xxx response code.
	 */
	uint64_t tcr_rb_ds;

	/** Tuning control requests Rollback/US_xxx
	 *
	 * The aggregate counter of controlled tuning attempts that failed due
	 * to target upstream wavelength channel parameter violation, as
	 * indicated by the Tuning_Response PLOAM message with Rollback
	 * operation code and US_xxx response code
	 */
	uint64_t tcr_rb_us;

	/** Tuning control requests failed with ONU reactivation
	 *
	 * The counter of controlled tuning attempts that failed on any reason,
	 * with expiration of timers TO4 or TO5 causing the ONU transition
	 * into state O1
	 */
	uint64_t tcr_fail_reac;
};

/** Structure to collect counters related to Tuning-related PM data.
 *  These values are used by the TWDM Channel Tuning PMHD2 managed entity.
 */
struct pa_twdm_channel_tuning_pmhd2 {
	/** Tuning control requests rejected/DS_ALBL
	 *
	 * The counter of transmitted Tuning_Response PLOAM messages with NACK
	 * operation code and DS_ALBL response code, indicating downstream
	 * administrative label inconsistency.
	 */
	uint64_t tcr_rej_ds_albl;

	/** Tuning control requests rejected/DS_VOID
	 *
	 * The counter of transmitted Tuning_Response
	 * PLOAM messages with NACK operation code and DS_VOID response code,
	 * indicating that the target downstream wavelength channel descriptor
	 * is void.
	 */
	uint64_t tcr_rej_ds_void;

	/** Tuning control requests rejected/DS_PART
	 *
	 * The counter of transmitted Tuning_Response
	 * PLOAM messages with NACK operation code and DS_PART response code,
	 * indicating that tuning request involves channel partition violation.
	 */
	uint64_t tcr_rej_ds_part;

	/** Tuning control requests rejected/DS_TUNR
	 *
	 * The counter of transmitted Tuning_Response
	 * PLOAM messages with NACK operation code and DS_TUNR response code,
	 * indicating that the target DS wavelength channel is out of receiver
	 * tuning range.
	 */
	uint64_t tcr_rej_ds_tunr;

	/** Tuning control requests rejected/DS_LNRT
	 *
	 * The counter of transmitted Tuning_Response PLOAM messages with
	 * NACK operation code and DS_LNRT response code, indicating downstream
	 * line rate inconsistency in the target channel.
	 */
	uint64_t tcr_rej_ds_lnrt;

	/** Tuning control requests rejected/DS_LNCD
	 *
	 * The counter of transmitted Tuning_Response
	 * PLOAM messages with NACK operation code and DS_LNCD response code,
	 * indicating downstream line code inconsistency in the target channel.
	 */
	uint64_t tcr_rej_ds_lncd;

	/** Tuning control requests rejected/US_ALBL
	 *
	 * The counter of transmitted Tuning_Response
	 * PLOAM messages with NACK operation code and US_ALBL response code,
	 * indicating upstream administrative label inconsistency.
	 */
	uint64_t tcr_rej_us_albl;

	/** Tuning control requests rejected/US_VOID
	 *
	 * The counter of transmitted Tuning_Response
	 * PLOAM messages with NACK operation code and US_VOID response code,
	 * indicating that the target upstream wavelength channel descriptor
	 * is void.
	 */
	uint64_t tcr_rej_us_void;

	/** Tuning control requests rejected/US_TUNR
	 *
	 * The counter of transmitted Tuning_Response
	 * PLOAM messages with NACK operation code and US_TUNR response code,
	 * indicating that the target US wavelength channel is out of
	 * transmitter tuning range.
	 */
	uint64_t tcr_rej_us_tunr;

	/** Tuning control requests rejected/US_CLBR
	 *
	 * The counter of transmitted Tuning_Response
	 * PLOAM messages with NACK operation code and US_CLBR response code,
	 * indicating that the transmitter has insufficient calibration
	 * accuracy in the target US wavelength channel.
	 */
	uint64_t tcr_rej_us_clbr;

	/** Tuning control requests rejected/US_LKTP
	 *
	 * The counter of transmitted Tuning_Response
	 * PLOAM messages with NACK operation code and US_LKTP response code,
	 * indicating upstream optical link type inconsistency.
	 */
	uint64_t tcr_rej_us_lktp;

	/** Tuning control requests rejected/US_LNRT
	 *
	 * The counter of transmitted Tuning_Response
	 * PLOAM messages with NACK operation code and US_LNRT response code,
	 * indicating upstream line rate inconsistency in the target channel.
	 */
	uint64_t tcr_rej_us_lnrt;

	/** Tuning control requests rejected/US_LNCD
	 *
	 * The counter of transmitted Tuning_Response
	 * PLOAM messages with NACK operation code and US_LNCD response code,
	 * indicating upstream line code inconsistency in the target channel.
	 */
	uint64_t tcr_rej_us_lncd;
};

/** Structure to collect counters related to Tuning-related PM data.
 *  These values are used by the TWDM Channel Tuning PMHD3 managed entity.
 */
struct pa_twdm_channel_tuning_pmhd3 {
	/** Tuning control requests Rollback/DS_ALBL
	 *
	 * The counter of controlled tuning attempts that failed due to
	 * downstream administrative label inconsistency, as indicated
	 * by the Tuning_Response PLOAM message with Rollback operation code
	 * and DS_ALBL response code.
	 */
	uint64_t tcr_rb_ds_albl;

	/** Tuning control requests Rollback/DS_LKTP
	 *
	 * The counter of controlled tuning attempts that failed due to
	 * downstream optical link type inconsistency, as indicated by
	 * the Tuning_Response PLOAM message with Rollback operation code and
	 * DS_LKTP response code.
	 */
	uint64_t tcr_rb_ds_lktp;

	/** Tuning control requests Rollback/US_ALBL
	 *
	 * The counter of controlled tuning attempts that failed due to
	 * upstream administrative label violation, as indicated by the
	 * Tuning_Response PLOAM message with Rollback operation code and
	 * US_ALBL response code.
	 */
	uint64_t tcr_rb_us_albl;

	/** Tuning control requests Rollback/US_VOID
	 *
	 * The counter of controlled tuning attempts that failed due to the
	 * target upstream wavelength channel descriptor being void, as
	 * indicated by the Tuning_Response PLOAM message with Rollback
	 * operation code and US_VOID response code.
	 */
	uint64_t tcr_rb_us_void;

	/** Tuning control requests Rollback/US_TUNR
	 *
	 * The counter of controlled tuning attempts that failed due to the
	 * transmitter tuning range violation, as indicated by the
	 * Tuning_Response PLOAM message with Rollback operation code and
	 * US_TUNR response code.
	 */
	uint64_t tcr_rb_us_tunr;

	/** Tuning control requests Rollback/US_LKTP
	 *
	 * The counter of controlled tuning attempts that failed due to the
	 * upstream optical link type violation, as indicated by the
	 * Tuning_Response PLOAM message with Rollback operation code and
	 * US_LKTP response code.
	 */
	uint64_t tcr_rb_us_lktp;

	/** Tuning control requests Rollback/US_LNRT
	 *
	 * The counter of controlled tuning attempts that failed due to the
	 * upstream line rate violation, as indicated by the Tuning_Response
	 * PLOAM message with Rollback operation code and US_LNRT response code
	 */
	uint64_t tcr_rb_us_lnrt;

	/** Tuning control requests Rollback/US_LNCD
	 *
	 * The counter of controlled tuning attempts that failed due to the
	 * upstream line code violation, as indicated by the Tuning_Response
	 * PLOAM message with Rollback operation code and US_LNCD response code
	 */
	uint64_t tcr_rb_us_lncd;
};

/** TWDM Channel Tuning PMHD attribute handling operations structure */
struct pa_twdm_channel_tuning_pmhd_ops {
	/**
	 * Get TWDM Channel Tuning counters (part 1)
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[in] me_id Managed Entity ID
	 * \param[out] param Pointer to a structure as defined
	 * by \ref pa_twdm_channel_tuning_pmhd1.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*cnt_get_pmhd1)(void *ll_handle, uint16_t me_id,
			 struct pa_twdm_channel_tuning_pmhd1 *props);

	/**
	 * Get TWDM Channel Tuning counters (part 2)
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[in] me_id Managed Entity ID
	 * \param[out] param Pointer to a structure as defined
	 * by \ref pa_twdm_channel_tuning_pmhd2.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*cnt_get_pmhd2)(void *ll_handle, uint16_t me_id,
			 struct pa_twdm_channel_tuning_pmhd2 *props);

	/**
	 * Get TWDM Channel Tuning counters (part 3)
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[in] me_id Managed Entity ID
	 * \param[out] param Pointer to a structure as defined
	 * by \ref pa_twdm_channel_tuning_pmhd3.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*cnt_get_pmhd3)(void *ll_handle, uint16_t me_id,
			 struct pa_twdm_channel_tuning_pmhd3 *props);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
