/*******************************************************************************
 *
 * Copyright (c) 2020 - 2024 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

#ifndef PON_MBOX_COUNTERS_H
#define PON_MBOX_COUNTERS_H

#include <linux/types.h>

/** \addtogroup PON_MBOX
 *  @{
 */

/** \defgroup PON_MBOX_REFERENCE_COUNTERS Counter Handling by the PON Mailbox
 *  This chapter describes the counter structures used by the PON mailbox
 *  driver.
 *  @{
 */

/** GEM/XGEM port counters.
 *  Used by fapi_pon_gem_port_counters_get.
 */
struct pon_mbox_gem_port_counters {
	/** GEM port ID for which the counters shall be reported. */
	__u32 gem_port_id;
	/** Transmitted GEM frames. */
	__u64 tx_frames;
	/** Transmitted GEM frame fragments. */
	__u64 tx_fragments;
	/** Transmitted bytes in GEM frames.
	 * This value reports user payload bytes only, not including header
	 * bytes.
	 */
	__u64 tx_bytes;
	/** Received GEM frames.
	 * This value reports the number of valid GEM frames that have been
	 * received without uncorrectable errors and a valid HEC.
	 */
	__u64 rx_frames;
	/** Received GEM frame fragments. */
	__u64 rx_fragments;
	/** Received bytes in valid GEM frames.
	 * This value reports user payload bytes only, not including header
	 * bytes.
	 */
	__u64 rx_bytes;
	/** XGEM key errors.
	 * The number of received key errors is provided for XG-PON,
	 * XGS-PON, and NG-PON2 only. The value is set to 0 for GPON
	 * implementations.
	 */
	__u64 key_errors;
};

/** Structure to collect counter related to GEM TC reception and transmission.
 */
struct pon_mbox_gtc_counters {
	/** Number of BIP errors. */
	__u64 bip_errors;
	/** Number of discarded GEM frames due to an invalid HEC.
	 * Functionally the same as gem_hec_errors_uncorr,
	 * which is not provided as a separate counter.
	 */
	__u64 disc_gem_frames;
	/** Number of corrected GEM HEC errors. */
	__u64 gem_hec_errors_corr;
	/** Number of uncorrected GEM HEC errors. */
	__u64 gem_hec_errors_uncorr;
	/** Number of corrected bandwidth map HEC errors. */
	__u64 bwmap_hec_errors_corr;
	/** Number of bytes corrected */
	__u64 bytes_corr;
	/** Number of FEC codewords corrected */
	__u64 fec_codewords_corr;
	/** Number of uncorrectable FEC codewords */
	__u64 fec_codewords_uncorr;
	/** Number of total received frames */
	__u64 total_frames;
	/** FEC errored seconds */
	__u64 fec_sec;
	/** Idle GEM errors */
	__u64 gem_idle;
	/** Number of downstream synchronization losses */
	__u64 lods_events;
	/** Dying Gasp activation time, given in multiples of 125 us */
	__u64 dg_time;
	/** Number of PLOAM CRC errors */
	__u64 ploam_crc_errors;
};

/** Structure to collect counter related to XGEM TC reception and transmission.
 *  Used by fapi_pon_xgtc_counters_get.
 */
struct pon_mbox_xgtc_counters {
	/** Uncorrected PSBd HEC errors ID2 */
	__u64 psbd_hec_err_uncorr;
	/** PSBd HEC errors.
	 * This is the number of HEC errors detected in any of the fields
	 * of the downstream physical sync block.
	 */
	__u64 psbd_hec_err_corr;
	/** Uncorrected FS HEC errors ID3 */
	__u64 fs_hec_err_uncorr;
	/** Corrected FS HEC errors ID5 */
	__u64 fs_hec_err_corr;
	/** Lost words due to uncorrectable HEC errors.
	 * This is the number of four-byte words lost because of an
	 * XGEM frame HEC error. In general, all XGTC payload following
	 * the error it lost, until the next PSBd event.
	 */
	__u64 lost_words;
	/** PLOAM MIC errors.
	 * This is the number of received PLOAM messages with an invalid
	 * MIC field.
	 */
	__u64 ploam_mic_err;
};

/** Allocation-specific counters.
 *  Used by fapi_pon_alloc_counters_get.
 */
struct pon_alloc_counters {
	/** Allocations received.
	 * This is the number of individual allocations that have been
	 * received for a given allocation (T-CONT).
	 */
	__u64 allocations;
	/** GEM idle frames.
	 * This is the number of GEM idle frames that have been sent
	 * within the selected allocation (T-CONT).
	 * It represents the available but unused upstream bandwidth.
	 */
	__u64 idle;
	/** Upstream average bandwidth.
	 * This is the assigned upstream bandwidth, averaged over 1 second.
	 * The value is given in units of bit/s.
	 */
	__u64 us_bw;
};

/** Number of allocation discard counters. */
#define PON_ALLOC_DISC_COUNTERS 8
/** Number of allocation rule mismatch counters. */
#define PON_ALLOC_RULE_COUNTERS 17

/** Allocation discard counters.
 *  Used by fapi_pon_alloc_discard_counters_get.

	These are the individually counted discard reasons:
	- 0: DISC0, Number of skipped allocations due to bandwidth map burst
			errors.
	- 1: DISC1, Number of foreign allocations in the bandwidth map.
	- 2: DISC2, Number of own allocations in foreign bursts.
	- 3: DISC3, Number of foreign allocations in no bursts (default).
	- 4: DISC4, Number of foreign allocations as start of bursts
			(default2).
	- 5: DISC5, Number of bandwidth map HEC errors.
	- 6: DISC6, Number of times that more than 64 allocations have been
			received.
	- 7: DISC7, Number of HLend HEC errors.
	- Others: RES, Reserved for future extension.

	These are the individually counted allocation rule violations:
	- 0: RULE0, Reserved.
	- 1: RULE1, Reserved.
	- 2: RULE2, Number of times that the spacing of adjacent bursts has
			been violated.
	- 3: RULE3, Reserved.
	- 4: RULE4, Number of times that the start time has exceeded a value of
			9719 maximum start time).
	- 5: RULE5, Reserved.
	- 6: RULE6, Number of times that the number of allocation structures
			has exceeded a value of 16 per single allocation.
	- 7: RULE7, Number of times that the number of allocation structures
			has exceeded a value of 64 for this ONU in total.
	- 8: RULE8, Number of times that the number of burst allocation series
			has exceeded a value of 16 for this ONU.
	- 9: RULE9, Number of times that the maximum grant size has been
			exceeded.
	- 10: RULE10, Number of times that the maximum FS burst size has been
			exceeded.
	- 11: RULE11, Number of times that the following rule has not been met:
			BurstStartTime + (sum of GrantSize) <= 14580.
	- 12: RULE12, Number of times that no allocations were found in the
			current frame after downstream time 13.168 us has
			passed and the downstream FIFO is still empty.
	- 13: RULE13, Number of times that the firmware unexpectedly missed to
			read all downstream allocations in time.
	- 14: RULE14, Number of times that the upstream allocation FIFO is not
			ready to receive the next burst.
	- 15: RULE15, Number of unknown profiles received.
	- 16: RULE16, Number of allocations successfully written into the
			upstream allocation FIFO.
	- Others: RES, Reserved for future extension.
*/
struct pon_alloc_discard_counters {
	/** This array holds the counters for discarded allocations. */
	__u64 disc[PON_ALLOC_DISC_COUNTERS];
	/** This array holds the counters for allocation rule violations. */
	__u64 rule[PON_ALLOC_RULE_COUNTERS];
};

/** Ethernet frame packet and byte counters per GEM port.
 *  Used by \ref pon_mbox_rx_eth_counters_update,
 *          \ref pon_mbox_tx_eth_counters_update,
 *          \ref pon_mbox_cnt_rx_eth_fw2pon, and
 *          \ref pon_mbox_cnt_tx_eth_fw2pon.
 */
struct pon_eth_counters {
	/** Ethernet payload bytes. */
	__u64 bytes;
	/** Ethernet packets below 64 byte size. */
	__u64 frames_lt_64;
	/** Ethernet packets of 64 byte size. */
	__u64 frames_64;
	/** Ethernet packets of 65 to 127 byte size. */
	__u64 frames_65_127;
	/** Ethernet packets of 128 to 255 byte size. */
	__u64 frames_128_255;
	/** Ethernet packets of 256 to 511 byte size. */
	__u64 frames_256_511;
	/** Ethernet packets of 512 to 1023 byte size. */
	__u64 frames_512_1023;
	/** Ethernet packets of 1024 to 1518 byte size. */
	__u64 frames_1024_1518;
	/** Ethernet packets of more than 1518 byte size. */
	__u64 frames_gt_1518;
	/** Ethernet packets with incorrect FCS value. */
	__u64 frames_fcs_err;
	/** Ethernet payload bytes in packets with incorrect FCS value. */
	__u64 bytes_fcs_err;
	/** Ethernet packets which exceed the maximum length. */
	__u64 frames_too_long;
};

/** Structure to collect counters related to TWDM LODS.
 *  Used by fapi_pon_twdm_lods_counters_get.
 */

struct pon_mbox_twdm_lods_counters {
	/** LODS Total Events */
	__u64 events_all;
	/* LODS Reactivation Events */
	__u64 react_oper;
	/* LODS Reactivation Events in the Protection Channel */
	__u64 react_prot;
	/* LODS Reactivation Events in a Discretionary Channel */
	__u64 react_disc;
	/* LODS Restored to the Operational Channel */
	__u64 rest_oper;
	/* LODS Restored to the Protection Channel */
	__u64 rest_prot;
	/* LODS Restored to a Discretionary Channel */
	__u64 rest_disc;
};

/** Structure to collect counters related to TWDM Optic Power Level.
 *  Used by fapi_pon_twdm_optic_pl_counters_get.
 */
struct pon_mbox_twdm_optic_pl_counters {
	/** Power Leveling Rejected Counter */
	__u64 rejected;
	/* Power Leveling Incomplete Counter */
	__u64 incomplete;
	/* Power Leveling Complete Counter */
	__u64 complete;
};

/** Structure to collect counters related to TWDM XGTC PLOAM messages.
 *  Used by fapi_pon_xgtc_ploam_ds_counters_get.
 */
struct pon_mbox_xgtc_ploam_ds_counters {
	/* Burst Profile */
	__u64 bst_profile;
	/* Assign ONU ID */
	__u64 ass_onu;
	/* Ranging Time */
	__u64 rng_time;
	/* Deactivate ONU ID */
	__u64 deact_onu;
	/* Disable Serial Number */
	__u64 dis_ser;
	/* Request Registration */
	__u64 req_reg;
	/* Assign Alloc ID */
	__u64 ass_alloc;
	/* Key Control */
	__u64 key_ctrl;
	/* Sleep Allow */
	__u64 slp_allow;
	/* Calibration Request */
	__u64 calib_req;
	/* Adjust Transmitter Wavelength */
	__u64 adj_tx_wl;
	/* Tuning Control */
	__u64 tune_ctrl;
	/* System Profile */
	__u64 sys_profile;
	/* Channel Profile */
	__u64 ch_profile;
	/* Protection Control */
	__u64 prot_control;
	/* Change Power Level */
	__u64 chg_pw_lvl;
	/* Power Consumption Inquire */
	__u64 pw_cons;
	/* Rate Control */
	__u64 rate_ctrl;
	/* Reboot ONU Message */
	__u64 reboot_onu;
	/* Unknown PLOAM Message */
	__u64 unknown;
	/* Adjust Transmitter Wavelength Failure */
	__u64 adj_tx_wl_fail;
	/* Tuning Control Request */
	__u64 tune_req;
	/* Tuning Control Complete */
	__u64 tune_compl;
};

/** Structure to collect counters related to GTC PLOAM DS message.
 *  Used by fapi_pon_xgtc_ploam_ds_counters_get.
 */
struct pon_mbox_gtc_ploam_ds_counters {
	/* Upstream Overhead */
	__u64 us_ovh;
	/* Assign ONU ID */
	__u64 ass_onu;
	/* Ranging Time */
	__u64 rng_time;
	/* Deactivate ONU ID */
	__u64 deact_onu;
	/* Disable Serial Number */
	__u64 dis_ser;
	/* Encrypted Port ID */
	__u64 enc_gem_pid;
	/* Request Password */
	__u64 req_pw;
	/* Assign Alloc ID */
	__u64 ass_alloc;
	/* No Message */
	__u64 no_msg;
	/* Popup */
	__u64 popup;
	/* Request Key */
	__u64 req_key;
	/* Configure Port ID */
	__u64 cfg_port;
	/* Physical Equipment Error */
	__u64 phy_ee;
	/* Change Power Level */
	__u64 chg_pl;
	/* PST Message */
	__u64 pst;
	/* BER Interval */
	__u64 ber;
	/* Key Switching Time */
	__u64 key_swtime;
	/* Extended Burst Length */
	__u64 ext_burst;
	/* PON ID */
	__u64 pon_id;
	/* Swift POPUP */
	__u64 swift_popup;
	/* Ranging Adjust */
	__u64 rng_adj;
	/* Sleep Allow */
	__u64 slp_allow;
	/* Unknown PLOAM message */
	__u64 unknown;
};

struct pon_mbox_gtc_ploam_us_counters {
	/* Serial Number ONU */
	__u64 ser_onu;
	/* Password */
	__u64 password;
	/* Dying Gasp */
	__u64 dyg_gasp;
	/* No Message */
	__u64 no_msg;
	/* Encryption Key */
	__u64 enc_key;
	/* Physical Equipment Error */
	__u64 phy_ee;
	/* PST Message */
	__u64 pst_msg;
	/* Remote Error Indication */
	__u64 rem_err;
	/* Acknowledge */
	__u64 ack;
	/* Sleep Request */
	__u64 slp_req;
};

struct pon_mbox_xgtc_ploam_us_counters {
	/* Serial Number ONU */
	__u64 ser_onu;
	/* Registration */
	__u64 reg;
	/* Key Report */
	__u64 key_rep;
	/* Acknowledge */
	__u64 ack;
	/* Sleep Request */
	__u64 slp_req;
	/* Tuning Response */
	__u64 tun_res;
	/* Power Consumption Report */
	__u64 pw_cons;
	/* Rate Response */
	__u64 rate_resp;
	/* Change Power Level Parameter Error Response */
	__u64 cpl_err;
	/* Tuning Response ACK or NACK */
	__u64 tun_res_an;
	/* Tuning Response Complete or Rollback */
	__u64 tun_res_crb;
};

/** Structure to collect counters related to TWDM Tuning Control.
 *  Used by fapi_pon_twdm_tuning_counters_get.
 */
struct pon_mbox_twdm_tc_counters {
	/* Tuning Control Counter 0 to 33 */
	__u64 tc_[34];
};

 /*! @} */ /* PON_MBOX_REFERENCE_COUNTERS */

 /*! @} */ /* PON_MBOX */

#endif /* end of include guard: PON_MBOX_COUNTERS_H */
