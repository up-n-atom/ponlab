/******************************************************************************
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 ******************************************************************************/

#ifndef DRV_PON_MBOX_COUNTERS_CONV_H
#define DRV_PON_MBOX_COUNTERS_CONV_H

/** \addtogroup PON_MBOX_REFERENCE_COUNTERS
 *  @{
 */

#include <asm/byteorder.h>
#include <pon/pon_ip_msg.h>
#include <pon/pon_mbox_counters.h>

/**
 * @brief Converts firmware structure data into PON structure format.
 *	  Will overwrite only fields covered by input structure.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int pon_mbox_cnt_gtc_fw2pon(struct ponfw_gtc_counters *in,
			    struct pon_mbox_gtc_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *	  Will overwrite only fields covered by input structure.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int pon_mbox_cnt_gtc_enh_fw2pon(struct ponfw_gtc_enhanced_counters *in,
				struct pon_mbox_gtc_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int pon_mbox_cnt_xgtc_fw2pon(struct ponfw_xgtc_counters *in,
			     struct pon_mbox_xgtc_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int pon_mbox_cnt_gem_port_fw2pon(struct ponfw_gem_port_counters *in,
				 struct pon_mbox_gem_port_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *	  Will overwrite only fields covered by input structure.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int pon_mbox_cnt_alloc_id_fw2pon(struct ponfw_alloc_id_counters *in,
				 struct pon_alloc_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *	  Will overwrite only fields covered by input structure.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int pon_mbox_cnt_alloc_bw_fw2pon(struct ponfw_alloc_bw *in,
				 struct pon_alloc_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int pon_mbox_cnt_alloc_lost_fw2pon(struct ponfw_alloc_lost_counters *in,
				   struct pon_alloc_discard_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int pon_mbox_cnt_rx_eth_fw2pon(struct ponfw_rx_eth_counters *in,
			       struct pon_eth_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int pon_mbox_cnt_tx_eth_fw2pon(struct ponfw_tx_eth_counters *in,
			       struct pon_eth_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int pon_mbox_cnt_twdm_lods_fw2pon(struct ponfw_twdm_lods_counters *in,
				  struct pon_mbox_twdm_lods_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int pon_mbox_cnt_twdm_optic_pl_fw2pon(
	struct ponfw_twdm_onu_optic_pl_counters *in,
	struct pon_mbox_twdm_optic_pl_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int pon_mbox_cnt_twdm_tc_fw2pon(struct ponfw_twdm_tc_counters *in,
				struct pon_mbox_twdm_tc_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int
pon_mbox_cnt_xgtc_ploam_ds_fw2pon(struct ponfw_xgtc_ploam_ds_counters *in,
				  struct pon_mbox_xgtc_ploam_ds_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int
pon_mbox_cnt_gtc_ploam_ds_fw2pon(struct ponfw_gtc_ploam_ds_counters *in,
				 struct pon_mbox_gtc_ploam_ds_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int
pon_mbox_cnt_xgtc_ploam_us_fw2pon(struct ponfw_xgtc_ploam_us_counters *in,
				  struct pon_mbox_xgtc_ploam_us_counters *out);

/**
 * @brief Converts firmware structure data into PON structure format.
 *
 * @param in pointer to firmware structure
 * @param out pointer to PON structure
 *
 * @return 0 on success, -EINVAL on invalid structures
 */
int
pon_mbox_cnt_gtc_ploam_us_fw2pon(struct ponfw_gtc_ploam_us_counters *in,
				 struct pon_mbox_gtc_ploam_us_counters *out);

/*! @} */

#endif /* end of include guard: DRV_PON_MBOX_COUNTERS_CONV_H */
