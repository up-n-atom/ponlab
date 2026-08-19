/******************************************************************************
 *
 *  Copyright (c) 2022 - 2026 MaxLinear, Inc.
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for
 * ANI-G.
 */

#ifndef _pon_adapter_me_ani_g_h
#define _pon_adapter_me_ani_g_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup PON_ADAPTER_ME OMCI Managed Entity Adaptation
 *
 *  Dedicated structures are defined for each of the implemented Managed
 *  Entities.
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_ANI_G ANI-G
 *  This Managed Entity models the ONU upstream interface towards the OLT.
 *  @{
 */

/** ANI-G update structure */
struct pon_adapter_ani_g_update_data {
	/** Dynamic bandwidth reporting mode */
	uint8_t dba_mode;
	/** GEM Block Length */
	uint16_t gem_block_len;
	/** Signal Fail Threshold */
	uint8_t sf_threshold;
	/** Signal Degrade Threshold */
	uint8_t sd_threshold;
	/** Lower optic threshold */
	uint8_t lower_optic_thr;
	/** Upper optic threshold */
	uint8_t upper_optic_thr;
	/** Lower transmit power threshold */
	uint8_t lower_tx_power_thr;
	/** Upper transmit power threshold */
	uint8_t upper_tx_power_thr;
};

/** ANI-G Alarm Types */
enum pon_adapter_ani_g_alarm {
	/** Received downstream optical power below threshold */
	PA_ALARM_ID_ANIG_LOW_RX_OPT_POWER = 0,
	/** Received downstream optical power above threshold */
	PA_ALARM_ID_ANIG_HIGH_RX_OPT_POWER = 1,
	/** Bit error-based signal fail. Industry practice normally
	 * expects the BER to improve by at least an order of
	 * magnitude before clearing the alarm
	 */
	PA_ALARM_ID_ANIG_SF = 2,
	/** Bit error-based signal degrade. Industry practice normally
	 * expects the BER to improve by at least an order of
	 * magnitude before clearing the alarm
	 */
	PA_ALARM_ID_ANIG_SD = 3,
	/** Transmit optical power below lower threshold */
	PA_ALARM_ID_ANIG_LOW_TX_OPT_POWER = 4,
	/** Transmit optical power above upper threshold */
	PA_ALARM_ID_ANIG_HIGH_TX_OPT_POWER = 5,
	/** Laser bias current above threshold determined by vendor;
	 * laser end of life pending
	 */
	PA_ALARM_ID_ANIG_LASER_BIAS_CURRENT = 6
};

/** ANI-G dba piggyback reporting modes */
enum pon_adapter_ani_g_dba_mode {
	/** DBA piggyback mode 0 only */
	PA_ANIG_DBA_PB_MODE_0_ONLY = 0,
	/** DBA piggyback reporting modes 0 and 1 */
	PA_ANIG_DBA_PB_MODE_0_1 = 1,
	/** DBA piggyback not supported */
	PA_ANIG_DBA_PB_MODE_NOT_SUPPORTED = 4
};

/** ANI-G attribute handling operations structure */
struct pon_adapter_ani_g_attr_data_ops {
	/** Retrieve dynamic bandwidth reporting mode
	 *
	 * \param[in]  ll_handle   Lower layer context pointer
	 * \param[in]  me_id       Managed Entity identifier
	 * \param[out] dba_mode    DBA reporting mode
	 */
	enum pon_adapter_errno (*dba_mode_get)(void *ll_handle,
					       uint16_t me_id,
					       uint8_t *dba_mode);

	/** Retrieve SR indication mode
	 *
	 * \param[in]  ll_handle   Lower layer context pointer
	 * \param[in]  me_id       Managed Entity identifier
	 * \param[out] indication  SR indication mode
	 */
	enum pon_adapter_errno (*sr_indication_get)(void *ll_handle,
						    uint16_t me_id,
						    uint8_t *indication);

	/** Retrieve total available T-CONT number
	 *
	 * \param[in]  ll_handle  Lower layer context pointer
	 * \param[in]  me_id      Managed Entity identifier
	 * \param[out] num        Total available T-CONT number
	 */
	enum pon_adapter_errno (*total_tcon_num_get)(void *ll_handle,
						     uint16_t me_id,
						     uint16_t *num);

	/** Retrieve actual ONU response time
	 *
	 * \param[in]  ll_handle      Lower layer context pointer
	 * \param[in]  me_id          Managed Entity identifier
	 * \param[out] response_time  ONU Response Time
	 */
	enum pon_adapter_errno (*response_time_get)(void *ll_handle,
						    uint16_t me_id,
						    uint16_t *response_time);

	/** Retrieve ONU ID
	 *
	 * \param[in]  ll_handle      Lower layer context pointer
	 * \param[out] onu_id  ONU ID
	 */
	enum pon_adapter_errno (*onu_id_get)(void *ll_handle, uint32_t *onu_id);

	/** Retrieve Supply Voltage if supported by hardware
	 *
	 * \param[in]  ll_handle  Lower layer context pointer
	 * \param[in]  me_id      Managed Entity identifier
	 * \param[out] voltage    Supply Voltage (20 mV resolution)
	 */
	enum pon_adapter_errno (*supply_voltage_get)(void *ll_handle,
						     uint16_t me_id,
						     uint16_t *voltage);
};

/** ANI-G optical signal level attribute operations structure */
struct pon_adapter_ani_g_optic_signal_lvl_ops {
	/** Retrieve current Optical Signal Level
	 *  (0.002 dBm resolution)
	 *
	 * \param[in]  ll_handle  Lower layer context pointer
	 * \param[in]  me_id      Managed Entity identifier
	 * \param[out] level      Optical Signal level
	 */
	enum pon_adapter_errno (*rx_get)(void *ll_handle,
					 uint16_t me_id,
					 int16_t *level);

	/** Retrieve current Optical Signal Level
	 *  (0.002 dBu resolution)
	 *
	 * \param[in]  ll_handle  Lower layer context pointer
	 * \param[in]  me_id      Managed Entity identifier
	 * \param[out] level      Optical Signal level
	 */
	enum pon_adapter_errno (*rx_dbu_get)(void *ll_handle,
					     uint16_t me_id,
					     int16_t *level);

	/** Retrieve current Transmit Optical Level
	 *  (0.002 dBm resolution)
	 *
	 * \param[in]  ll_handle  Lower layer context pointer
	 * \param[in]  me_id      Managed Entity identifier
	 * \param[out] level      Transmit Optical Level
	 */
	enum pon_adapter_errno (*tx_get)(void *ll_handle,
					 uint16_t me_id,
					 int16_t *level);

	/** Retrieve current Transmit Optical Level
	 *  (0.002 dBu resolution)
	 *
	 * \param[in]  ll_handle  Lower layer context pointer
	 * \param[in]  me_id      Managed Entity identifier
	 * \param[out] level      Transmit Optical Level
	 */
	enum pon_adapter_errno (*tx_dbu_get)(void *ll_handle,
					     uint16_t me_id,
					     int16_t *level);
};

/** ANI-G transmit laser attribute operations structure */
struct pon_adapter_ani_g_laser_ops {
	/** Retrieve Laser Bias Current
	 *
	 * \param[in]  ll_handle    Lower layer context pointer
	 * \param[in]  me_id        Managed Entity identifier
	 * \param[out] bias_current Laser Bias Current (2 uA resolution)
	 */
	enum pon_adapter_errno (*bias_current_get)(void *ll_handle,
						   uint16_t me_id,
						   uint16_t *bias_current);

	/** Retrieve Laser Temperature
	 *
	 * \param[in]  ll_handle   Lower layer context pointer
	 * \param[in]  me_id       Managed Entity identifier
	 * \param[out] temperature Laser Temperature (1/256 degree C resolution)
	 */
	enum pon_adapter_errno (*temperature_get)(void *ll_handle,
						  uint16_t me_id,
						  int16_t *temperature);
};

/** ANI-G operations structure */
struct pon_adapter_ani_g_ops {
	/** Update ANI-G Managed Entity resources
	 *
	 * \param[in] ll_handle    Lower layer context pointer
	 * \param[in] me_id        Managed Entity identifier
	 * \param[in] update_data  ANI-G update data
	 */
	enum pon_adapter_errno (*update)(void *ll_handle, uint16_t me_id,
		const struct pon_adapter_ani_g_update_data *update_data);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 * \param[in] ll_handle  Lower layer context pointer
	 * \param[in] me_id      Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle, uint16_t me_id);

	/** Initialize required Managed Entity resources
	 *
	 * \param[in] ll_handle  Lower layer context pointer
	 * \param[in] me_id      Managed Entity identifier
	 */
	enum pon_adapter_errno (*create)(void *ll_handle, uint16_t me_id);

	/** ANI-G attribute handling operations */
	const struct pon_adapter_ani_g_attr_data_ops *attr_data;
	/** ANI-G optical level operations */
	const struct pon_adapter_ani_g_optic_signal_lvl_ops *signal_lvl;
	/** ANI-G transmit laser attribute operations */
	const struct pon_adapter_ani_g_laser_ops *laser;
};

/** @} */ /* PON_ADAPTER_ME_ANI_G */

/** @} */ /* PON_ADAPTER_ME */

#ifdef __cplusplus
}
#endif

#endif
