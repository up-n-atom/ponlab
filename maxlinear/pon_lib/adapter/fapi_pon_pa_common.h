/*****************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 *  \file fapi_pon_pa_common.h
 * This is the PON ADAPTER WRAPPER header file.
 */

#ifndef _FAPI_PON_PA_COMMON_H_
#define _FAPI_PON_PA_COMMON_H_

#include "fapi_pon.h" /* for pon_ctx */
#include "fapi_pon_mapper_table.h"
#include "fapi_pon_os.h"
#include "pon_adapter.h"
#include "pon_adapter_event_handlers.h"
#include "fapi_pon_pa_twdm.h"

#define DEBUG_MODULE libpon
#include "pon_adapter_debug_common.h"

#include <semaphore.h>
#include <pthread.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#define UNUSED(x) (void)(x)

#define sizeof_member(type, member) sizeof(((type *)0)->member)

/**
 * This macro can be used to perform assertions at compile time.
 * Example: PON_STATIC_ASSERT(sizeof(int) == 4, int_must_be_4_bytes)
 * The example will cause the code not to compile unless sizeof(int) == 4
 */
#define PON_STATIC_ASSERT(condition, msg) \
do { \
	char assert_##msg[(condition) ? 1 : -1]; \
	UNUSED(assert_##msg); \
} while (0)

/** Optic timing offsets per transceiver
 *  Signed value to be added to base (board) settings,
 *  same units as in optic config.
 */
struct pon_optic_offsets {
	/** Laser setup time (ps). */
	int32_t laser_setup_time;
	/** Laser hold time (ps). */
	int32_t laser_hold_time;
	/** SerDes setup time (ps). */
	int32_t serdes_setup_time;
	/** SerDes hold time (ps). */
	int32_t serdes_hold_time;
	/** Bias current enable signal setup time (ps). */
	int32_t bias_setup_time;
	/** Bias current enable signal hold time (ps). */
	int32_t bias_hold_time;
};

/** Configuration parameters for FAPI PON */
struct fapi_pon_wrapper_cfg {
	/** ONU identifier */
	uint32_t ident;
	/** Serial number. */
	uint8_t serial_no[PON_SERIAL_NO_SIZE];
	/** Registration ID. */
	uint8_t reg_id[PON_REG_ID_SIZE];
	/** PON IP MAC address.
	 * Used as MAC source address in OMCC downstream
	 * and as MAC destination address in OMCC upstream.
	 */
	uint8_t mac_sa[6];
	/** SoC MAC address
	 * Used as MAC destination address in OMCC downstream
	 * and as MAC source address in OMCC upstream.
	 */
	uint8_t mac_da[6];
	/** Ethertype value, used in both directions. */
	uint16_t ethertype;
	/** Protocol value, used in both directions. */
	uint8_t protocol[5];
	/**  PLOAM Timeout Timer 0 (Discovery Timer) */
	uint32_t ploam_timeout_0;
	/**  PLOAM Timeout Timer 1 (Ranging Timer) */
	uint32_t ploam_timeout_1;
	/**  PLOAM Timeout Timer 2 (LODS Timer) */
	uint32_t ploam_timeout_2;
	/**  PLOAM Timeout Timer 3 (LODS Timer with WLCP) */
	uint32_t ploam_timeout_3;
	/**  PLOAM Timeout Timer 4 (Downstream Tuning Timer) */
	uint32_t ploam_timeout_4;
	/**  PLOAM Timeout Timer 5 (Upstream Tuning Timer) */
	uint32_t ploam_timeout_5;
	/**  PLOAM Timeout Timer 6 (Lost ONU Timer) */
	uint32_t ploam_timeout_6;
	/**  PLOAM Timeout Timer CPL (Change Transmit Power Level) */
	uint32_t ploam_timeout_cpl;
	/**  PLOAM Timeout Timer TPD (Change TProfileDwell timer) */
	uint32_t ploam_timeout_tpd;
	/**  PLOAM Timeout Timer CPI (Channel Partition Index) */
	uint32_t ploam_timeout_cpi;
	/** The PON mode to use. See \ref enum pon_mode */
	enum pon_mode mode;
	/** Optical interface configuration. */
	struct pon_optic_cfg optic;
	/** Optical interface configuration, transceiver offsets. */
	struct pon_optic_offsets optic_offsets;
	/** GPIO configuration. */
	struct pon_gpio_cfg gpio;
	/** Serializer/Deserializer configuration. */
	struct pon_serdes_cfg serdes;
	/** PLOAM emergency stop state. */
	uint32_t ploam_emerg_stop_state;
	/** Power leveling capability.*/
	uint32_t plev_cap;
	/** EEPROM dmi filename */
	char eeprom_dmi[64];
	/** EEPROM serial id filename */
	char eeprom_serial_id[64];
	/** SFP EEPROM tweaks,
	 * see SFP_TWEAK_SKIP_SOFT_TX_DISABLE define below
	 */
	uint32_t sfp_tweaks;
	/** Bias threshold */
	uint16_t bias_threshold;
	/** Lower optical threshold */
	int8_t lower_receive_optical_threshold;
	/** Upper optical threshold */
	int8_t upper_receive_optical_threshold;
	/** Lower transmit power threshold */
	int8_t lower_transmit_power_threshold;
	/** Upper transmit power threshold */
	int8_t upper_transmit_power_threshold;
	/** TDM co-existence operation of XG-PON and XGS-PON. */
	uint32_t tdm_coexistence;
	/** Time of Day configuration. */
	struct pon_tod_cfg tod;
	/** Asymmetry correction factor for GPON */
	double tod_asymm_corr_gpon;
	/** Asymmetry correction factor for XGSPON/XGPON */
	double tod_asymm_corr_xgspon;
	/** Asymmetry correction factor for NGPON2 */
	double tod_asymm_corr_ngpon2;
	/** Extra offset for ToD in pico seconds for 2.5G PON*/
	int32_t tod_offset_pico_seconds_2g5;
	/** Extra offset for ToD in pico seconds for 10G PON*/
	int32_t tod_offset_pico_seconds_10g;
	/** Interoperability mode setting. */
	uint32_t iop_mask;
	/** Multiple wavelengths config method */
	uint8_t twdm_config_method;
	/** TWDM tuning method */
	uint8_t twdm_tuning;
	/** TWDM channel mask */
	uint8_t twdm_channel_mask;
	/** Multiple wavelengths configuration */
	struct pon_twdm_cfg twdm;
	/** TWDM configuration to PONIP FW by means of TWDM_CONFIG message */
	struct pon_twdm_wlse_config twdm_wlse_config;
	/** Mutual authentication pre-shared key (128 bit) */
	uint8_t psk[MAX_AUTH_TABLE_SIZE];
	/** Mutual authentication key size */
	uint8_t psk_size;
	/** Mutual authentication mode */
	uint8_t auth_mode;
	/** true if PON is enabled by default */
	uint8_t enabled;
	/** Dying Gasp disable */
	uint32_t dg_dis;
	/** Downstream Ethernet FCS Mode (-1: Use value from driver) */
	int32_t ds_fcs_en;
	/** Downstream Timestamp Disable (-1: Use value from driver) */
	int32_t ds_ts_dis;
	/** Calibration status record */
	uint64_t cal_status_record;
};

/**
 * SFP EEPROM tweaks
 * Each bit can be used to enable/disable specific behaviour.
 * The bits are defined in the SFP_TWEAK_* macros.
 */
/** bit 0: Skip toggle of SOFT_TX_DISABLE */
#define SFP_TWEAK_SKIP_SOFT_TX_DISABLE (1 << 0)

struct fapi_pon_ani_g_data {
	/** Lock for data structure */
	pthread_mutex_t lock;
	/** Update status of ANI-G ME */
	bool update_status;
	/** Thread identifier */
	pthread_t tid;
	/** Lower optic threshold */
	int32_t lower_optic_thr;
	/** Upper optic threshold */
	int32_t upper_optic_thr;
	/** Lower transmit power threshold */
	int32_t lower_tx_power_thr;
	/** Upper transmit power threshold */
	int32_t upper_tx_power_thr;

	/** Lower optic alarm state */
	bool lower_optic_alarm;
	/** Upper optic alarm state */
	bool upper_optic_alarm;
	/** Lower transmit power alarm state */
	bool lower_tx_power_alarm;
	/** Upper transmit power alarm state */
	bool upper_tx_power_alarm;
	/** laser bias alarm state */
	bool bias_current_alarm;

	/** Signal fail */
	bool signal_fail;
	/** Signal degrade */
	bool signal_degrade;
};

struct pa_config;
struct fapi_pon_wrapper_ctx {
	/** FAPI PON handle structure */
	struct pon_ctx *pon_ctx;
	/** FAPI PON event context handle structure */
	struct pon_ctx *ponevt_ctx;
	/** Pointer to context of higher layer */
	void *hl_ctx;
	/* locks this structure */
	pthread_mutex_t lock;

	/** Configuration parameters for FAPI PON */
	struct fapi_pon_wrapper_cfg cfg;
	/** Config manipulation functions */
	const struct pa_config *cfg_ops;

	/** Thread identifier */
	pthread_t tid;

	/** init done event */
	sem_t init_done;
	/** result of fw init */
	enum fapi_pon_errorcode init_result;

	/** event callbacks for each event type */
	struct pa_eh_ops event_handlers;

	/** Control parameters for ANI-G ME */
	struct fapi_pon_ani_g_data ani_g_data;

	/** true in case the FW init was done */
	bool init_done_fw;
	/** true in case the OMCI init was done */
	bool init_done_omci;
	/** protects the initialization sequence of the FW */
	pthread_mutex_t init_lock;

	/** Array of mappers (for each ID type) */
	struct mapper *mapper[MAPPER_IDTYPE_MAX];

	/** Pointer to TWDM OPS */
	const struct pon_twdm_ops *twdm_ops;
	/** Selected twdm downstream channel id */
	int used_dwlch_id;
};

/**
 *	Starts event processing.
 *
 *	\param[in] ctx     Wrapper context.
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_pa_event_handling_init(struct fapi_pon_wrapper_ctx *ctx);

/**
 *	Stops event processing.
 *
 *	\param[in] ctx     Wrapper context.
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_pa_event_handling_stop(struct fapi_pon_wrapper_ctx *ctx);

/**
 *	Starts alarm checking of optical values.
 *
 *	\param[in] ctx     Wrapper context.
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_pa_ani_g_alarm_check_start(struct fapi_pon_wrapper_ctx *ctx);

/**
 *	Stops alarm checking for optical values.
 *
 *	\param[in] ctx     Wrapper context.
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_pa_ani_g_alarm_check_stop(struct fapi_pon_wrapper_ctx *ctx);

/**
 *	Retrigger the alarm checking for optical values.
 *
 *	\param[in] ctx     Wrapper context.
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_ani_g_alarm_recheck(struct fapi_pon_wrapper_ctx *ctx);

/**
 *	Registers some of the events
 *
 *	\param[in] ctx     Wrapper context.
 */
enum pon_adapter_errno pon_pa_event_start(struct fapi_pon_wrapper_ctx *ctx);

/**
 *	Converts a FAPI PON error code into a PON adapter error code.
 *
 *	\param[in] err     FAPI PON error code
 *
 *	\return matching PON adapter error code
 */
enum pon_adapter_errno pon_fapi_to_pa_error(enum fapi_pon_errorcode err);

/**
 *	Stops given thread using cancellation API
 *
 *	\param[in] th_id		pointer to the pthread object
 *	\param[in] th_name		thread name
 *	\param[in] time		timeout [s] for closing the thread
 *
 *	\return Return value as follows:
 *	- PON_ADAPTER_SUCCESS: If successful
 *	- Other: An error code in case of error.
 */
enum pon_adapter_errno pon_fapi_thread_stop(const pthread_t *th_id,
					    const char *th_name, int time);

/* This writes a configuration value and optionally commits the file.
 *
 *  \param[in]     caller	Caller context pointer
 *  \param[in]     path		Config path
 *  \param[in]     sec		Config section
 *  \param[in]     opt		Config option
 *  \param[in]     val		Option value
 *  \param[in]     commit	Optional commit the config file
 */
enum pon_adapter_errno pon_pa_config_write(struct fapi_pon_wrapper_ctx *ctx,
					   const char *path,
					   const char *sec,
					   const char *opt,
					   const char *val,
					   bool commit);

#endif /* _FAPI_PON_PA_COMMON_H_ */
