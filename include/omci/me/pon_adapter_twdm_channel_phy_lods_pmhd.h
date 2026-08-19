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
 * This is a PON Adapter OMCI Managed Entity header file for the
 * TWDM Channel PHY/LODS Performance Monitoring History Data managed entity.
 */

#ifndef _pon_adapter_twdm_channel_phy_lods_h
#define _pon_adapter_twdm_channel_phy_lods_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_TWDM_CHANNEL_PHY_LODS_PMHD
 * 		TWDM Channel PHY/LODS PMHD
 *
 * This ME collects certain PM data associated with the slot/circuit pack,
 * hosting one or more ANI-G MEs, and a specific TWDM channel. Instances of
 * this ME are created and deleted by the OLT.
 *
 * An instance of this ME is associated with an instance of TWDM channel ME.
 *
 *  @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
 * instance of the TWDM Channel Entity.
 */

/** Structure to collect counters related to PHY/LODS-related PM data.
 *  These values are used by the TWDM Channel PHY/LODS PMHD managed entity.
 */
struct pa_twdm_channel_phy_lods_pmhd {
	/** Total received words protected by bit-interleaved parity-32 (BIP-32)
	 *
	 *  The count of 4 byte words included in BIP-32 check.
	 *  This is a product of the number of downstream FS frames received
	 *  by the size of the downstream FS frame after the FEC parity byte,
	 *  if any, have been removed. The count applies to the entire
	 *  downstream data flow, whether or not addressed to that ONT.
	 */
	uint64_t total_words;

	/** BIP-32 bit error count
	 *
	 *  Count of the bit errors in the received downstream FS frames as
	 *  measured using BIP-32. If FEC is supported in the downstream
	 *  direction, the BIP-32 count applies to the downstream FS frame
	 *  after the FEC correction has been applied and the FEC parity
	 *  bytes have been removed.
	 */
	uint32_t bip32_errors;

	/** Corrected PSBd HEC error count
	 *
	 *  The count of the errors in either CFC or OCS fields of the PSBd
	 *  block that have been corrected using the HEC technique.
	 */
	uint32_t psbd_hec_err_corr;

	/** Uncorrectable PSBd HEC error count
	 *
	 *  The count of the errors in either CFC or OCS fields of the PSBd
	 *  block that could not be corrected using the HEC technique.
	 */
	uint32_t psbd_hec_err_uncorr;

	/** Corrected downstream FS header HEC error count
	 *
	 *  The count of the errors in the downstream FS header that have
	 *  been corrected using the HEC technique.
	 */
	uint32_t fs_hec_err_corr;

	/** Uncorrectable downstream FS header HEC error count
	 *
	 *  The count of the errors in the downstream FS header that could
	 *  not be corrected using the HEC technique.
	 */
	uint32_t fs_hec_err_uncorr;

	/** Total number of LODS events
	 *
	 *  The count of the state transitions from O5.1/O5.2 to O6,
	 *  referring to the ONU activation cycle state machine,
	 *  clause 12 of [ITUT G.989.3].
	 */
	uint32_t lods_events_all;

	/** LODS events restored in operating TWDM channel
	 *
	 *  The count of LODS events cleared automatically without retuning.
	 */
	uint32_t lods_restored_oper;

	/** LODS events restored in protection TWDM channel
	 *
	 *  The count of LODS events resolved by retuning to a pre-configured
	 *  protection TWDM channel. The event is counted against the original
	 *  operating channel.
	 */
	uint32_t lods_restored_prot;

	/** LODS events restored in discretionary TWDM channel
	 *
	 *  The count of LODS events resolved by retuning to a TWDM channel
	 *  chosen by the ONU, without retuning. Implies that the wavelength
	 *  channel protection for the operating channel is not active. The
	 *  event is counted against the original operating channel.
	 */
	uint32_t lods_restored_disc;

	/** LODS events resulting in reactivation
	 *
	 *  The count of LODS events resolved through ONU reactivation; that
	 *  is, either TO2 (without WLCP) or TO3 + TO4 (with WLCP) expires
	 *  before the downstream channel is reacquired, referring to the ONU
	 *  activation cycle state machine, clause 12 of [ITU-T G.989.3]. The
	 *  event is counted against the original operating channel
	 */
	uint32_t lods_reactivation;

	/** LODS events resulting in reactivation after
	 *  retuning to protection TWDM channel.
	 *
	 *  The count of LODS events resolved through ONU reactivation after
	 *  attempted protection switching, which turns unsuccessful due to a
	 *  handshake failure.
	 */
	uint32_t lods_reactivation_prot;

	/** LODS events resulting in reactivation after
	 *  retuning to discretionary TWDM channel.
	 *
	 * The count of LODS events resolved through ONU reactivation after
	 *  attempted retuning to a discretionary channel, which turns
	 *  unsuccessful due to a handshake failure.
	 */
	uint32_t lods_reactivation_disc;
};

struct pa_twdm_channel_phy_lods_pmhd_ops {
	/**
	 * Get PHY/LODS (XGTC) counters
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[in] me_id Managed Entity ID
	 * \param[out] param Pointer to a structure as defined
	 * by \ref pa_twdm_channel_phy_lods_pmhd.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*cnt_get)(void *ll_handle, uint16_t me_id,
		   struct pa_twdm_channel_phy_lods_pmhd *props);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
