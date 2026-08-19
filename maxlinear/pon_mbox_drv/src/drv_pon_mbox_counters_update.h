/******************************************************************************
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 ******************************************************************************/

#ifndef DRV_PON_MBOX_COUNTERS_UPDATE_H
#define DRV_PON_MBOX_COUNTERS_UPDATE_H

#include <drv_pon_mbox_counters.h>

struct pon_mbox;

/** \addtogroup PON_MBOX_REFERENCE_COUNTERS
 *  @{
 */

/**
 * @brief Counters update function for GEM_PORT_COUNTERS firmware message.
 *
 * @param[in] dswlch_id TWDM DS Wavelength Channel ID
 * @param[in] gem_port_idx GEM Port Index
 * @param[out] cnt	   Pointer to GEM Port Counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
int pon_mbox_gem_port_counters_update(u8 dswlch_id, u8 gem_port_idx,
				      struct pon_mbox_gem_port_counters *cnt,
				      struct pon_mbox *pon_mbox_dev);

/**
 * @brief Update function for accumulated GEM port counters.
 *
 * @param[in] dswlch_id TWDM DS Wavelength Channel ID
 * @param[out] cnt	   Pointer to accumulated GEM Port counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 */
void pon_mbox_gem_all_counters_update(u8 dswlch_id,
				      struct pon_mbox_gem_port_counters *cnt,
				      struct pon_mbox *pon_mbox_dev);

/**
 * @brief Counters update function for ALLOC_ID_COUNTERS firmware message.
 *
 * @param[in] alloc_idx    Alloc Index
 * @param[out] cnt	   Pointer to Alloc ID Counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
int pon_mbox_alloc_counters_update(u8 alloc_idx, struct pon_alloc_counters *cnt,
				   struct pon_mbox *pon_mbox_dev);

/**
 * @brief Counters update function for ALLOC_LOST_COUNTERS firmware message.
 *
 * @param[out] cnt	   Pointer to Alloc Discard Counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
int pon_mbox_alloc_lost_counters_update(struct pon_alloc_discard_counters *cnt,
					struct pon_mbox *pon_mbox_dev);

/**
 * @brief Counters update function for GTC_COUNTERS firmware message.
 *
 * @param[in] dswlch_id    TWDM DS Wavelength Channel ID
 * @param[out] cnt	   Pointer to GTC Counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
int pon_mbox_gtc_counters_update(u8 dswlch_id,
				 struct pon_mbox_gtc_counters *cnt,
				 struct pon_mbox *pon_mbox_dev);

/**
 * @brief Counters update function for XGTC_COUNTERS firmware message.
 *
 * @param[in] dswlch_id    TWDM DS Wavelength Channel ID
 * @param[out] cnt	   Pointer to XGTC Counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
int pon_mbox_xgtc_counters_update(u8 dswlch_id,
				  struct pon_mbox_xgtc_counters *cnt,
				  struct pon_mbox *pon_mbox_dev);

/**
 * @brief Counters update function for PONFW_RX_ETH_COUNTERS_CMD_ID firmware
 *        message.
 *
 * @param[in] gem_port_idx GEM Port Index
 * @param[out] cnt	   Pointer to RX_ETH Counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
int pon_mbox_rx_eth_counters_update(u8 gem_port_idx,
				    struct pon_eth_counters *cnt,
				    struct pon_mbox *pon_mbox_dev);

/**
 * @brief Counters update function for PONFW_TX_ETH_COUNTERS_CMD_ID firmware
 *        message.
 *
 * @param[in] gem_port_idx GEM Port Index
 * @param[out] cnt	   Pointer to TX_ETH Counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
int pon_mbox_tx_eth_counters_update(u8 gem_port_idx,
				    struct pon_eth_counters *cnt,
				    struct pon_mbox *pon_mbox_dev);

/**
 * @brief Return mask of enabled counters
 *
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return mask of enabled counters
 */
u64 pon_mbox_counters_enabled_get(struct pon_mbox *pon_mbox_dev);

/**
 * @brief Counters update function for PONFW_TWDM_LODS_COUNTERS_CMD_ID
 *        firmware message.
 *
 * @param[out] dswlch_id   TWDM DS wavelength channel ID
 * @param[out] cnt	   Pointer to TWDM_LODS Counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
int pon_mbox_twdm_lods_counters_update(u8 dswlch_id,
				       struct pon_mbox_twdm_lods_counters *cnt,
				       struct pon_mbox *pon_mbox_dev);

/**
 * @brief Counters update function for PONFW_TWDM_ONU_OPTIC_PL_COUNTERS_CMD_ID
 *        firmware message.
 *
 * @param[out] dswlch_id   TWDM DS wavelength channel ID
 * @param[out] cnt	   Pointer to TWDM_OPTIC_PL Counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
int pon_mbox_twdm_optic_pl_counters_update(
	u8 dswlch_id, struct pon_mbox_twdm_optic_pl_counters *cnt,
	struct pon_mbox *pon_mbox_dev);

/**
 * @brief Counters update function for PONFW_TWDM_TC_COUNTERS_CMD_ID
 *        firmware message.
 *
 * @param[out] dswlch_id   DS wavelength channel ID
 * @param[out] cnt	   Pointer to TWDM_TC Counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
int pon_mbox_twdm_tc_counters_update(u8 dswlch_id,
				     struct pon_mbox_twdm_tc_counters *cnt,
				     struct pon_mbox *pon_mbox_dev);

/**
 * @brief Counters update function for PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID
 *        firmware message.
 *
 * @param[out] dswlch_id   DS wavelength channel ID
 * @param[out] cnt	   Pointer to XGTC_PLOAM_DS_CNT Counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
int pon_mbox_xgtc_ploam_ds_counters_update(
	u8 dswlch_id, struct pon_mbox_xgtc_ploam_ds_counters *cnt,
	struct pon_mbox *pon_mbox_dev);

/**
 * @brief Counters update function for PONFW_GTC_PLOAM_DS_COUNTERS_CMD_ID
 *        firmware message.
 *
 * @param[out] dswlch_id   DS wavelength channel ID
 * @param[out] cnt	   Pointer to GTC_PLOAM_DS_CNT Counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
int pon_mbox_gtc_ploam_ds_counters_update(
	u8 dswlch_id, struct pon_mbox_gtc_ploam_ds_counters *cnt,
	struct pon_mbox *pon_mbox_dev);

/**
 * @brief Counters update function for PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID
 *        firmware message.
 *
 * @param[out] dswlch_id   DS wavelength channel ID
 * @param[out] cnt	   Pointer to XGTC_PLOAM_US_CNT Counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
int pon_mbox_xgtc_ploam_us_counters_update(
	u8 dswlch_id, struct pon_mbox_xgtc_ploam_us_counters *cnt,
	struct pon_mbox *pon_mbox_dev);

/**
 * @brief Counters update function for PONFW_GTC_PLOAM_US_COUNTERS_CMD_ID
 *        firmware message.
 *
 * @param[out] dswlch_id   DS wavelength channel ID
 * @param[out] cnt	   Pointer to GTC_PLOAM_US_CNT Counters table
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
int pon_mbox_gtc_ploam_us_counters_update(
	u8 dswlch_id, struct pon_mbox_gtc_ploam_us_counters *cnt,
	struct pon_mbox *pon_mbox_dev);

/*! @} */

#endif /* end of include guard: DRV_PON_MBOX_COUNTERS_UPDATE_H */
