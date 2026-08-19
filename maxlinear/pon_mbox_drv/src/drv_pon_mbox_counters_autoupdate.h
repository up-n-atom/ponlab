/******************************************************************************
 *
 * Copyright (c) 2023 - 2025 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

#ifndef DRV_PON_MBOX_COUNTERS_AUTOUPDATE_H
#define DRV_PON_MBOX_COUNTERS_AUTOUPDATE_H

/* The default counter update time in seconds */
#define PON_COUNTERS_DEFAULT_UPDATE_TIME 840
#define PON_COUNTERS_NO_UPDATE		 0

struct pon_mbox;
struct cnt_autoupdate;

/** \addtogroup PON_MBOX_REFERENCE_COUNTERS
 *  @{
 */

/**
 * @brief Creates the context for automatic update of counters
 *
 * @param[in] pon_mbox_dev PON Mbox Device
 * @param[in] update_time_in_s Update time in seconds
 *
 * @return instance of cnt_autoupdate, NULL in case of failure
 */
struct cnt_autoupdate *
pon_mbox_cnt_autoupdate_create(struct pon_mbox *pon_mbox_dev,
			       unsigned int update_time_in_s);

/**
 * @brief Refreshes the counters if update is needed. This is normally called
 *        periodically using timers created in
 *        @ref pon_mbox_cnt_autoupdate_create, so please don't call it manually
 *
 * @param[in] cnt_autoupdate	instance of cnt_autoupdate.
 */
void pon_mbox_cnt_autoupdate_update(struct cnt_autoupdate *cnt_autoupdate);

/**
 * @brief In NG-PON2 mode a counter update is required before switching the
 *        wavelength channel. Therefore the update is triggered and after this
 *        the new wavelength channel IDs are set.
 *
 * @param[in] cnt_autoupdate	Instance of cnt_autoupdate.
 * @param[in] new_dswlch_id	New DS wavelength channel ID.
 * @param[in] new_uswlch_id	New US wavelength channel ID.
 */
void pon_mbox_cnt_autoupdate_wl_switch(struct cnt_autoupdate *cnt_autoupdate,
				       u8 new_dswlch_id, u8 new_uswlch_id);

/**
 * @brief Destroys the automatic update of counters
 *
 * @param[in] cnt_autoupdate	instance of cnt_autoupdate.
 *				It is safe to pass NULL here
 */
void pon_mbox_cnt_autoupdate_destroy(struct cnt_autoupdate *cnt_autoupdate);

/*! @} */ /* PON_MBOX_REFERENCE_COUNTERS */

#endif /* end of include guard: DRV_PON_MBOX_COUNTERS_AUTOUPDATE_H */
