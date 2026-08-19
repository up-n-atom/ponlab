/*****************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2019 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 *  \file
 *  This is the PON library header file, defining the API functions
 *  and data structures for controlling the PON IP module in NG-PON2
 *  operation modes.
 */

#ifndef _FAPI_PON_TWDM_H_
#define _FAPI_PON_TWDM_H_

/** PON library ctx structure.
 *  Used by TWDM FAPI functions.
 */
struct pon_ctx;

/** PON library FEC Counter structure.
 *  Used by \ref fapi_pon_twdm_fec_counters_get.
 */
struct pon_fec_counters;


/** \addtogroup PON_FAPI_REFERENCE
 *   @{
 */

/** \defgroup PON_FAPI_REFERENCE_TWDM TWDM-specific PON Library Functions
 *	      This chapter describes the functional API interface to access
 *	      functions that are specific to TWDM PON technologies.
 *   @{
 */

/* TWDM-specific enumerations */
/* ========================== */

/** PON TWDM configuration methods to access the PMD of the optical interface.
 */
enum pon_twdm_config_method {
	/** Dummy method, changes nothing */
	PON_TWDM_CONF_METHOD0 = 0,
	/** Password EEPROM access */
	PON_TWDM_CONF_METHOD1 = 1,
	/** Read-write EEPROM access */
	PON_TWDM_CONF_METHOD2 = 2,
	/** US & DS wavelength set together,
	 * no individual selection per direction
	 */
	PON_TWDM_CONF_METHOD3 = 3,
};

/** PON TWDM Configuration operation types */
enum pon_twdm_oper_type {
	/** Upstream configuration */
	PON_TWDM_US_WL_CONF = 0,
	/** Downstream configuration */
	PON_TWDM_DS_WL_CONF = 1,
	/** Tuning configuration */
	PON_TWDM_US_WL_TUNING = 2,
};

/** PON TWDM Configuration supported wavelength types */
enum pon_twdm_wl_type {
	/** US: 1532.68 nm = 195.6 THz | DS: 1596.34 nm = 187.8 THz */
	PON_TWDM_WL_TYPE0 = 0x00,
	/** Related to type0 upstream frequency in firmware */
	PON_TWDM_US_WL_TYPE0 = 1956,
	/** Related to type0 downstream frequency in firmware */
	PON_TWDM_DS_WL_TYPE0 = 1878,
	/** US: 1533.47 nm = 195.5 THz | DS: 1597.19 nm = 187.7 THz */
	PON_TWDM_WL_TYPE1 = 0x01,
	/** Related to type1 upstream frequency in firmware */
	PON_TWDM_US_WL_TYPE1 = 1955,
	/** Related to type1 downstream frequency in firmware */
	PON_TWDM_DS_WL_TYPE1 = 1877,
	/** US: 1534.25 nm = 195.4 THz | DS: 1598.04 nm = 187.6 THz */
	PON_TWDM_WL_TYPE2 = 0x02,
	/** Related to type2 upstream frequency in firmware */
	PON_TWDM_US_WL_TYPE2 = 1954,
	/** Related to type2 downstream frequency in firmware */
	PON_TWDM_DS_WL_TYPE2 = 1876,
	/** US: 1535.04 nm = 195.3 THz | DS: 1898.89 nm = 187.5 THz */
	PON_TWDM_WL_TYPE3 = 0x03,
	/** Related to type3 upstream frequency in firmware */
	PON_TWDM_US_WL_TYPE3 = 1953,
	/** Related to type3 downstream frequency in firmware */
	PON_TWDM_DS_WL_TYPE3 = 1875,
};

/** Structure to handle the optical interface configuration specific to the
 *  handling of multiple wavelengths (TWDM operation in NG-PON2).
 *  Used by \ref fapi_pon_twdm_cfg_set and \ref fapi_pon_twdm_cfg_get.
 */
struct pon_twdm_cfg {
	/** Supported optical link type.
	 *  This is the optical link type as defined in ITU-T G.989.2.
	 *  - 0: NONE, The link type is unspecified.
	 *  - 1: A, Link type A is supported, link type B is not supported.
	 *  - 2: B, Link type B is supported, link type A is not supported.
	 *  - 3: AB, Both link types A and B are supported.
	 */
	uint32_t link_type;
	/** Last downstream wavelength channel ID.
	 *  This is the wavelength channel ID on which the last successful
	 *  downstream connection has been established.
	 *  The valid range is from 1 to 8.
	 *  Other wavelengths are for future definition.
	 *  - 1: CH1, 187.8 THz - 1596.34 nm
	 *  - 2: CH2, 187.7 THz - 1597.19 nm
	 *  - 3: CH3, 187.6 THz - 1598.04 nm
	 *  - 4: CH4, 187.5 THz - 1598.89 nm
	 *  - 5: CH5, 187.4 THz - 1599.75 nm
	 *  - 6: CH6, 187.3 THz - 1600.60 nm
	 *  - 7: CH7, 187.2 THz - 1601.46 nm
	 *  - 8: CH8, 187.1 THz - 1602.31 nm
	 */
	uint32_t dwlch_id;
	/** Free spectral range.
	 *  If a cyclic WM is used in the upstream, this is represented as an
	 *  unsigned integer indicating the value in units of 0.1 GHz.
	 */
	uint32_t free_spectral_range;
	/** Wavelength channel spacing.
	 *  This is the wavelength grid to which the optical interface can be
	 *  tuned. The value is given in multiples of 0.1 GHz.
	 */
	uint32_t wl_ch_spacing;
	/** Maximum spectral excursion.
	 *  This value is represented as an unsigned integer indicating the
	 *  value in units of 1 GHz.
	 */
	uint32_t max_spectral_excursion;
	/** Tuning granularity.
	 *  This is an unsigned integer value which represents the tuning
	 *  granularity of the ONU transmitter expressed in units of 1 GHz.
	 *  The value of 0x00 indicates that the ONU does not support fine
	 *  tuning/dithering.
	 */
	uint32_t tuning_gran;
	/** Receiver wavelength switching time.
	 *  This is an unsigned integer value which represents the value of
	 *  the maximum switching time from one downstream wavelength to another
	 *  downstream wavelength, expressed in units of 125 us.
	 */
	uint32_t rx_wl_switch_time;
	/** Transmitter wavelength switching time.
	 *  This is an unsigned integer value which represents the value of
	 *  the maximum switching time from one upstream wavelength to another
	 *  upstream wavelength, expressed in units of 125 us.
	 */
	uint32_t tx_wl_switch_time;
	/** Channel Partition Index.
	 *  This is an unsigned integer value in the range from 0 to 15.
	 */
	uint32_t ch_partition_index;
	/** One-step fine tuning time.
	 *  This is an unsigned integer value which represents the value
	 *  of the wavelength fine tuning time for a single granularity step,
	 *  expressed in units of PHY frames (125 μs) up to a value of 255.
	 *  The value of 0 indicates that the ONU does not support
	 *  fine tuning/dithering.
	 */
	uint32_t fine_tune_time;
	/** Wavelength lock flag.
	 *  The boolean value indicates if used optic transceiver allows
	 *  independent selection of downstream and upstream wavelength.
	 *  If the LOCK is set the selection is common for both directions.
	 */
	uint32_t wl_lock;

};

/** structure to handle the TWDM configuration.
 *  used by \ref fapi_pon_twdm_wlse_config_set
 *  and \ref fapi_pon_twdm_wlse_config_get.
 */
struct pon_twdm_wlse_config {
	/** TWDM Wavelength Switching Delay in 125 µs. */
	uint32_t wl_switch_delay;
	/** This is slowing down the wavelength search during initial
	 * wavelength scan mode if no signal applied to save power.
	 */
	uint32_t wl_sw_delay_init;
	/** The maximum number of wavelength switching rounds through all
	 * wavelengths in initial scan mode (with faster WL changes) before
	 * switching to regular scan mode.
	 */
	uint32_t wl_sw_rounds_init;
};

/**
 * Structure to handle the calibration record status.
 * Used by \ref fapi_pon_get_cal_record_state
 */
struct pon_twdm_cal_record {
	/* Calibration record status octets */
	uint8_t cal_rec[8];
};

/** TWDM channel status information, related to the TWDM channel that is
 *  currently in use (if any).
 *  Used by \ref fapi_pon_twdm_status_get for NG-PON2 systems.
 */
struct pon_twdm_status {
	/** TWDM System Identifier.
	 *  This is a 20-bit value to identify the system to which the ONU is
	 *  connected.
	 */
	uint32_t sys_id;
	/** TWDM Upstream channel index.
	 *  Index of the upstream TWDM channel that is in use.
	 *  The index values range from 1 to the number of supported upstream
	 *  TWDM wavelengths, a value of 0 indicates that no upstream
	 *  wavelength has been configured.
	 */
	uint32_t us_ch_index;
	/** TWDM Downstream channel index.
	 *  Index of the downstream TWDM channel that is in use.
	 *  The index values range from 1 to the number of supported downstream
	 *  TWDM wavelengths, a value of 0 indicates that no downstream
	 *  wavelength has been configured.
	 */
	uint32_t ds_ch_index;
	/** TWDM Upstream channel frequency.
	 *  Frequency of the upstream TWDM channel which is in use.
	 *  The value is given in multiples of 0.1 GHz, a value of 0 indicates
	 *  that no upstream wavelength has been configured.
	 */
	uint32_t us_ch_freq;
	/** TWDM Downstream channel frequency.
	 *  Frequency of the downstream TWDM channel which is in use.
	 *  The value is given in multiples of 0.1 GHz, a value of 0 indicates
	 *  that no upstream wavelength has been configured.
	 */
	uint32_t ds_ch_freq;
	/** TWDM System profile version.
	 *  This is a 4-bit value to identify the system profile which is in
	 *  use.
	 */
	uint32_t sys_profile_version;
	/** TWDM wavelength channel count.
	 *  This is a 4-bit value to announce the number of wavelength channels
	 *  provided by the OLT.
	 */
	uint32_t ch_count;
	/** TWDM Upstream wavelength adjustment amplitude.
	 *  This is the accumulated value of all wavelength adjustments which
	 *  have been applied to the upstream transmitter.
	 *  The value is given as a signed integer value, counting steps of
	 *  0.1 GHz for adjustment in positive or negative direction.
	 */
	int32_t us_wl_adj_amplitude;
	/** TWDM Upstream wavelength channel spacing.
	 *  The value is given in multiples of 0.1 GHz.
	 *  A value of 0 indicates that the upstream channel spacing is unknown
	 *  or the system uses only a single wavelength.
	 */
	uint32_t us_ch_spacing;
	/** TWDM Upstream wavelength band option.
	 *  One of the following options is reported by the OLT:
	 *  - 0: WB, Wide band option.
	 *  - 1: RB, Reduced band option.
	 *  - 2: NB, Narrow band option.
	 *  - Other: Unused.
	 */
	uint32_t us_wl_band_option;
};

/** Structure to show the channel profile information for a selected channel
 *  profile identifier (for TWDM operation in NG-PON2).
 *  Used by \ref fapi_pon_twdm_channel_profile_status_get.
 */
struct pon_twdm_channel_profile {
	/** PON ID.
	 *  This is a 32-bit static value which is carried in the operation
	 *  control (OC) structure of each downstream PHY frame in the specified
	 *  TWDM channel. It consists of the 28-bit administrative label and the
	 *  DWLCH ID (downstream wavelength channel identifier).
	 */
	uint32_t pon_id;
	/** Default response channel. */
	uint32_t def_resp_ch;
	/** Upstream wavelength channel frequency.
	 *  This is the frequency of the TWDM wavelength channel to be used in
	 *  upstream direction.
	 *  The frequency is given in multiples of 0.1 GHz.
	 */
	uint32_t us_frq;
	/** ONU power leveling response threshold.
	 *  This is an unsigned integer representing the maximum number of PLOAM
	 *  messages the ONU can transmit at non-zero attenuation level while
	 *  attempting to establish communication with OLT CT. It is zero, if
	 *  the OLT CT does not encourage ONU-activated power leveling.
	 */
	uint8_t resp_thr;
	/** Downstream frequency offset.
	 *  This value specifies the frequency offset of the downstream
	 *  wavelength from the nominal value. The value represented as a signed
	 *  integer in complementary code, and expressed in units of 0.1 GHz.
	 *
	 *  Special values are:
	 *  - 0x00: None, No frequency offset.
	 *  - 0x80: Unknown, The frequency offset is unknown.
	 */
	uint8_t ds_offset;
	/** Cloned configuration indication.
	 *  For more detail see ITU-T G.989.3.
	 */
	uint8_t cloned;
	/** Channel profile version.
	 *  Each time this Channel Profile is updated by the OLT CT, the version
	 *  is incremented. The value rolls over from 255 to 0 upon overflow.
	 */
	uint8_t version;
	/** This indicates if downstream data is available in this System
	 *  Profile.
	 *  - 0: NA, Downstream data available.
	 *  - 1: Avail, Downstream data is available.
	 */
	uint8_t ds_valid;
	/** This indicates if the OLT applies downstream FEC for the channel
	 *  profile.
	 *  - 0: FEC_OFF, Downstream FEC is inactive.
	 *  - 1: FEC_ON, Downstream FEC is active.
	 */
	uint8_t ds_fec;
	/** Downstream data rate.
	 *  - 0: HIGH, 9.95328 Gbit/s.
	 *  - 1: LOW, 2.48832 Gbit/s.
	 */
	uint8_t ds_rate;
	/** This indicates if upstream data is available in this System Profile.
	 *  - 0: NA, Upstream data available.
	 *  - 1: Avail, Upstream data is available.
	 */
	uint8_t us_valid;
	/** Upstream optical link type support.
	 *  - 0: NONE, No upstream is supported (this value is not used).
	 *  - 1: B, Only type B optical links are supported.
	 *  - 2: A, Only type A optical links are supported.
	 *  - 3: AB, Type A and type B optical links are supported.
	 */
	uint8_t us_type;
	/** Upstream data rate support.
	 *  - 0: NONE, No upstream is supported (unused value).
	 *  - 1: LOW, 2.48832 Gbit/s.
	 *  - 2: HIGH, 9.95328 Gbit/s.
	 *  - 3: BOTH, 2.48832 and 9.95328 Gbit/s.
	 */
	uint8_t us_rate;
	/** Downstream wavelength channel identifier.
	 *  The valid range is from 0 to 15.
	 */
	uint8_t dswlch_id;
	/** Upstream wavelength channel identifier.
	 *  The valid range is from 0 to 15.
	 */
	uint8_t uswlch_id;
	/** Default attenuation.
	 *  This value specifies the default attenuation value that shall be
	 *  used as a start value for ONU power leveling (if supported by the
	 *  ONU).
	 *  The attenuation values can be defined in steps of 3 dB.
	 *  The valid range is from 0 to 7.
	 *  - 0: No attenuation is requested.
	 *  - 1: 3 dB attenuation
	 *  - 2: 6 dB attenuation
	 *  ...
	 *  - 7: 21 dB attenuation
	 */
	uint8_t def_att;
	/** Channel Partition Index (CPI).
	 *  This value indicates to which Channel Partition this Channel Profile
	 *  belongs.
	 *  The valid range is from 0 to 15.
	 */
	uint8_t cpi;
	/** This indicates if the channel profile is related to the
	 *   currently used wavelength channel.
	 *  - 0: OTHER, The profile belongs to another wavelength channel
	 *  - 1: THIS, The profile belongs to the currently active wavelength
	 *       channel.
	 */
	uint8_t this_wl_channel;
};

/** Structure to handle the wavelength-specific XGTC counters.
 *  The counters accumulate their values while operating on a selected
 *  wavelength pair.
 *  Used by \ref fapi_pon_twdm_xgtc_counters_get.
 *  This set of counters can be used to implement the OMCI managed entity
 *  "TWDM channel PHY/LODS monitoring history data".
 */
struct pon_twdm_xgtc_counters {
	/** Uncorrected PSBd HEC errors */
	uint64_t psbd_hec_err_uncorr;
	/** PSBd HEC errors.
	 *  This is the number of HEC errors detected in any of the fields
	 *  of the downstream physical sync block.
	 */
	uint64_t psbd_hec_err_corr;
	/** Uncorrected FS HEC errors */
	uint64_t fs_hec_err_uncorr;
	/** Corrected FS HEC errors */
	uint64_t fs_hec_err_corr;
	/** Lost words due to uncorrectable HEC errors.
	 *  This is the number of four-byte words lost because of an
	 *  XGEM frame HEC error. In general, all XGTC payload following
	 *  the error it lost, until the next PSBd event.
	 */
	uint64_t lost_words;
	/** PLOAM MIC errors.
	 *  This is the number of received PLOAM messages with an invalid
	 *  Message Integrity Check (MIC) field.
	 */
	uint64_t ploam_mic_err;
	/** XGEM HEC Error count */
	uint64_t xgem_hec_err_corr;
	/** Discarded XGEM frames */
	uint64_t xgem_hec_err_uncorr;
	/** Unknown burst profiles
	 *  The firmware does not support this counter due to
	 *  performance limitations and always reports the value 0.
	 */
	uint64_t burst_profile_err;
	/** Number of 4-byte words received with BIP protection. */
	uint64_t words;
	/** Number of BIP errors. */
	uint64_t bip_errors;
	/** Number of LODS events */
	uint64_t lods_events_all;
	/** Number of LODS events restored in the operating TWDM channel */
	uint64_t lods_restored_oper;
	/** Number of LODS events restored in the protection TWDM channel */
	uint64_t lods_restored_prot;
	/** Number of LODS events restored in the discretionary TWDM channel */
	uint64_t lods_restored_disc;
	/** Number of LODS events causing reactivation */
	uint64_t lods_reactivation;
	/** Number of LODS events causing reactivation after unsuccessful
	 *  protection switching
	 */
	uint64_t lods_reactivation_prot;
	/** Number of LODS events causing reactivation after unsuccessful
	 * retuning to the discretionary channel
	 */
	uint64_t lods_reactivation_disc;
};

/** TWDM optic power leveling counters.
 *  Used by \ref fapi_pon_twdm_optic_pl_counters_get.
 */
struct pon_twdm_optic_pl_counters {
	/** Power leveling reject counter.
	 *  This is the number of rejected power leveling attempts by the OLT.
	 *  It counts the number of "change power level" PLOAMd messages which
	 *  have been answered with the "parameter error" completion code.
	 */
	uint64_t rejected;
	/** Power leveling incomplete counter.
	 *  This is the number of rejected power leveling attempts by the OLT.
	 *  It counts the number of "change power level" PLOAMd messages which
	 *  have been answered with the "busy" completion code.
	 */
	uint64_t incomplete;
	/** Power leveling complete counter.
	 *  This is the number of successfiul power leveling attempts by the
	 *  OLT. It counts the number of "change power level" PLOAMd messages
	 *  which have been answered with the "OK" completion code.
	 */
	uint64_t complete;
};

/**
 *	Set optical interface TWDM configuration.
 *	This function is used only in TWDM applications (such as NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined by
 *	\ref pon_twdm_cfg.
 *
 *	\remarks This function can only be called once and must be called
 *	before \ref fapi_pon_optic_cfg_set!
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode
fapi_pon_twdm_cfg_set(struct pon_ctx *ctx, const struct pon_twdm_cfg *param);
#endif

/**
 *	Read the optical interface TWDM configuration back.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_twdm_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_twdm_cfg_get(struct pon_ctx *ctx, struct pon_twdm_cfg *param);

/**
 *	Get the TWDM channel status information of the active downatream and
 *	upstream channels.
 *	This function is available for NG-PON2 operation only.
 *
 *	\param[in] ctx  PON FAPI context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_twdm_status.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_twdm_status_get(struct pon_ctx *ctx, struct pon_twdm_status *param);

/**
 *	Read the TWDM channel information for a selected channel profile.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] cp_id TWDM channel profile identifier.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_twdm_channel_profile.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_twdm_channel_profile_status_get
	(struct pon_ctx *ctx, uint32_t cp_id,
	 struct pon_twdm_channel_profile *param);

/**
 *	Read the TWDM channel partition index from FW.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] channel_partition_index TWDM channel partition index.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_twdm_cpi_get(struct pon_ctx *ctx,
					      uint8_t *channel_partition_index);

/**
 *	Write the TWDM channel partition index to FW.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] channel_partition_index TWDM channel partition index.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_twdm_cpi_set(struct pon_ctx *ctx,
					      uint8_t channel_partition_index);

/**
 *	Read the TWDM configuration from FW.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param points to a structure of pon_twdm_wlse_config type
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_twdm_wlse_config_get(struct pon_ctx *ctx,
			      struct pon_twdm_wlse_config *param);

/**
 *	Write the TWDM configuration to FW.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param points to a structure of pon_twdm_wlse_config type
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_twdm_wlse_config_set(struct pon_ctx *ctx,
			      const struct pon_twdm_wlse_config *param);

/**
 *	Set the TWDM wavelength channel ID for the driver counters.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] dswlch_id TWDM DS wavelength channel ID.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode
fapi_pon_twdm_counter_wlchid_set(struct pon_ctx *ctx,
				 const uint8_t dswlch_id);
#endif

/**
 *	Function to read the wavelength-specific XGTC counters.
 *	The counters accumulate their values while operating on a selected
 *	wavelength pair.
 *	Used by \ref fapi_pon_twdm_xgtc_counters_get.
 *	This set of counters can be used to implement the OMCI managed entity
 *	"TWDM channel PHY/LODS monitoring history data".
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] dswlch_id Downstream wavelength channel ID
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_twdm_xgtc_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_twdm_xgtc_counters_get(struct pon_ctx *ctx,
				const uint8_t dswlch_id,
				struct pon_twdm_xgtc_counters *param);

/**
 *	Function to read the FEC related counters on a selected
 *	TWDM wavelength pair, identified by its downstream wavelength channel ID.
 *	This function can be used to implement the OMCI managed entity
 *	"FEC performance monitoring history data" if this is associated with a
 *	"TWDM channel" managed entity.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] dswlch_id Downstream wavelength channel ID
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_fec_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_twdm_fec_counters_get(struct pon_ctx *ctx,
			       const uint8_t dswlch_id,
			       struct pon_fec_counters *param);

/**
 *	Function to read the TWDM power leveling counters.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] dswlch_id Downstream wavelength channel ID
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_twdm_optic_pl_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_twdm_optic_pl_counters_get(struct pon_ctx *ctx,
				    const uint8_t dswlch_id,
				    struct pon_twdm_optic_pl_counters *param);

/**
 *	Function to read TWDM XGEM port related counters.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] dswlch_id Downstream wavelength channel ID.
 *	\param[in] gem_port_id XGEM port ID to get counters for.
 *		The valid ranges are:
 *		- 0 to 65536 for XG-PON, XGS-PON, and NG-PON2 (lower 16 bit)
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_gem_port_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_twdm_xgem_port_counters_get(struct pon_ctx *ctx,
				     const uint8_t dswlch_id,
				     uint16_t gem_port_id,
				     struct pon_gem_port_counters *param);

/**
 *	Function to read the XGEM related counters on a selected
 *	TWDM wavelength pair, identified by its downstream wavelength
 *	channel ID. The counters are accumulated across all active XGEM ports.
 *	This function can be used to implement the OMCI managed entity
 *	"TWDM channel XGEM performance monitoring history data".
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] dswlch_id Downstream wavelength channel ID
 *	\param[out] param Pointer to a structure as defined
 *	by \ref fapi_pon_twdm_xgem_counters_get.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode
fapi_pon_twdm_xgem_all_counters_get(struct pon_ctx *ctx,
				    const uint8_t dswlch_id,
				    struct pon_gem_port_counters *param);
#endif

/** PLOAM downstream message counters per TWDM wavelength.
 *  A common value is reported for unicast and broadcast messages.
 *  The counter increment rate is 2/125 us or slower.
 *  Counters for unused messages are always reported as 0.
 *  Used by \ref fapi_pon_twdm_ploam_ds_counters_get.
 */
struct pon_twdm_ploam_ds_counters {
	/** Assign ONU ID message (GPON, XG-PON, NG-PON2, XGS-PON). */
	uint64_t assign_onu_id;
	/** Ranging time message (GPON, XG-PON, NG-PON2, XGS-PON). */
	uint64_t ranging_time;
	/** Deactivate ONU ID message (GPON, XG-PON, NG-PON2, XGS-PON). */
	uint64_t deact_onu;
	/** Disable serial number message (GPON, XG-PON, NG-PON2, XGS-PON). */
	uint64_t disable_ser_no;
	/** Assign allocation ID message (GPON, XG-PON, NG-PON2, XGS-PON). */
	uint64_t assign_alloc_id;
	/** Change Power Level (CPL) message (GPON, NG-PON2). */
	uint64_t cpl;
	/** Sleep allow message (GPON, XG-PON, NG-PON2, XGS-PON). */
	uint64_t sleep_allow;
	/** Request registration message (XG-PON, NG-PON2, XGS-PON). */
	uint64_t req_reg;
	/** Key control message (XG-PON, NG-PON2, XGS-PON). */
	uint64_t key_control;
	/** Burst profile message (NG-PON2, XGS-PON, XG-PON). */
	uint64_t burst_profile;
	/** Calibration request message (NG-PON2). */
	uint64_t cal_req;
	/** Adjust transmitter wavelength message (NG-PON2). */
	uint64_t tx_wavelength;
	/** Tuning control message with operation code "request" (NG-PON2). */
	uint64_t tuning_request;
	/** Tuning control message with operation code "complete" (NG-PON2). */
	uint64_t tuning_complete;
	/** System profile message (NG-PON2). */
	uint64_t system_profile;
	/** Channel profile message (NG-PON2). */
	uint64_t channel_profile;
	/** Protection control message (NG-PON2). */
	uint64_t protection;
	/** Power consumption inquire message (NG-PON2). */
	uint64_t power;
	/** Rate control message (NG-PON2). */
	uint64_t rate;
	/** Reset (reboot) ONU message. */
	uint64_t reset;
	/** Unknown message. */
	uint64_t unknown;
	/** Sum of all messages. */
	uint64_t all;

	/** Unsatisfied Adjust transmitter wavelength message */
	uint64_t tx_wavelength_err;
	/** PLOAM downstream messages with MIC errors */
	uint64_t mic_err;
	/* Tuning Control */
	uint64_t tune_ctrl;
};

/** Function to read the PLOAM downstream counters on a selected
 *  TWDM wavelength pair, identified by its downstream wavelength channel ID.
 *  This function is used to implement the OMCI managed entity
 *  "TWDM channel PLOAM PMHD part 1" and "TWDM channel PLOAM PMHD part 2"
 *  that are associated with a "TWDM channel" managed entity.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] dswlch_id Downstream wavelength channel ID
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_twdm_ploam_ds_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_twdm_ploam_ds_counters_get(struct pon_ctx *ctx,
				    const uint8_t dswlch_id,
				    struct pon_twdm_ploam_ds_counters *param);

/** PLOAM upstream message counters per TWDM wavelength.
 *  A common value is reported for unicast and broadcast messages.
 *  The counter increment rate is 2/125 us or slower.
 *  Counters for unused messages are always reported as 0.
 *  Used by \ref fapi_pon_twdm_ploam_us_counters_get.
 */
struct pon_twdm_ploam_us_counters {
	/** Serial number ONU message */
	uint64_t ser_no;
	/** Registration message */
	uint64_t reg;
	/** Key report message */
	uint64_t key_rep;
	/** Acknowledge message */
	uint64_t ack;
	/** Sleep request message */
	uint64_t sleep_req;
	/** Tuning response message with ACK or NACK */
	uint64_t tuning_resp_ack_nack;
	/** Tuning response message with Complete_u or Rollback */
	uint64_t tuning_resp_complete_rollback;
	/** Power consumption report message */
	uint64_t power_rep;
	/** Change power level parameter error response messages */
	uint64_t cpl_err;
	/** Sum of all upstream PLOAM messages. */
	uint64_t all;
};

/** Function to read the PLOAM upstream counters on a selected
 *  TWDM wavelength pair, identified by its downstream wavelength channel ID.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] dswlch_id Downstream wavelength channel ID
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_twdm_ploam_us_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_twdm_ploam_us_counters_get(struct pon_ctx *ctx,
				    const uint8_t dswlch_id,
				    struct pon_twdm_ploam_us_counters *param);

/** Structure to collect counters related to TWDM wavelength tuning.
 *  The counter functions are as follows:
 *  - 0: TC_0, Count of PLOAMd Tuning_Control (Request) messages
 *       for Rx or Tx/Rx.
 *  - 1: TC_1, Count of PLOAMd Tuning_Control (Request) messages
 *       for TX.
 *  - 2: TC_2, Count of PLOAMu Tuning_Response (NACK) messages
 *       with Response Code = INT_SFC.
 *  - 3: TC_3, Count of PLOAMu Tuning_Response (NACK) messages
 *       with Response Code = DS_xxx.
 *  - 4: TC_4, Count of PLOAMu Tuning_Response (NACK) messages
 *       with Response Code = DS_ALBL.
 *  - 5: TC_5, Count of PLOAMu Tuning_Response (NACK) messages
 *       with Response Code = DS_VOID.
 *  - 6: TC_6, Count of PLOAMu Tuning_Response (NACK) messages
 *       with Response Code = DS_PART.
 *  - 7: TC_7, Count of PLOAMu Tuning_Response (NACK) messages
 *       with Response Code = DS_TUNR.
 *  - 8: TC_8, Count of PLOAMu Tuning_Response (NACK) messages
 *       with Response Code = DS_LNRT.
 *  - 9: TC_9, Count of PLOAMu Tuning_Response (NACK) messages
 *       with Response Code = DS_LNCD.
 *  - 10: TC_10, Count of PLOAMu Tuning_Response (NACK) messages
 *        with Response Code = US_xxx .
 *  - 11: TC_11, Count of PLOAMu Tuning_Response (NACK) messages
 *        with Response Code = US_ALBL .
 *  - 12: TC_12, Count of PLOAMu Tuning_Response (NACK) messages
 *        with Response Code = US_VOID.
 *  - 13: TC_13, Count of PLOAMu Tuning_Response (NACK) messages
 *        with Response Code = US_TUNR.
 *  - 14: TC_14, Count of PLOAMu Tuning_Response (NACK) messages
 *        with Response Code = US_CLBR.
 *  - 15: TC_15, Count of PLOAMu Tuning_Response (NACK) messages
 *        with Response Code = US_LKTP.
 *  - 16: TC_16, Count of PLOAMu Tuning_Response (NACK) messages
 *        with Response Code = US_LNRT.
 *  - 17: TC_17, Count of PLOAMu Tuning_Response (NACK) messages
 *        with Response Code = US_LNCD.
 *  - 18: TC_18, Count of Tuning_Control (Request) PLOAM messages received,
 *        for which US tuning confirmation has been obtained in the specified
 *        target channel.
 *  - 19: TC_19, Count of times that TO4 expiration in DS Tuning state (O8)
 *        in the target channel.
 *  - 20: TC_20, Number of times that TO5 expires in US Tuning state (O9)
 *        in the target channel.
 *  - 21: TC_21, Number of times that the ONU fails to re-tune to the specified
 *        target channel, but re-tunes to the channel of its choice
 *        (discretionary channel).
 *  - 22: TC_22, Count of PLOAMu Tuning_Response (Rollback) messages
 *        with Response Code = COM_DS.
 *  - 23: TC_23, Count of PLOAMu Tuning_Response (Rollback) messages
 *        with Response Code = DS_xxx.
 *  - 24: TC_24, Count of PLOAMu Tuning_Response (Rollback) messages
 *        with Response Code = DS_ALBL.
 *  - 25: TC_25, Count of PLOAMu Tuning_Response (Rollback) messages
 *        with Response Code = DS_LKTP.
 *  - 26: TC_26, Count of PLOAMu Tuning_Response (Rollback) messages
 *        with Response Code = US_xxx.
 *  - 27: TC_27, Count of PLOAMu Tuning_Response (Rollback) messages
 *        with Response Code = US_ALBL.
 *  - 28: TC_28, Count of PLOAMu Tuning_Response (Rollback) messages
 *        with Response Code = US_VOID.
 *  - 29: TC_29, Count of PLOAMu Tuning_Response (Rollback) messages
 *        with Response Code = US_TUNR.
 *  - 30: TC_30, Count of PLOAMu Tuning_Response (Rollback) messages
 *        with Response Code = US_LKTP.
 *  - 31: TC_31, Count of PLOAMu Tuning_Response (Rollback) messages
 *        with Response Code = US_LNRT.
 *  - 32: TC_32, Count of PLOAMu Tuning_Response (Rollback) messages
 *        with Response Code = US_LNCD.
 *  - 33: TC_33, Count of state transitions from O5.2 to O8 followed by
 *        expiration of TO4 or TO5, thus causing a transition to O1.
 */
struct pon_twdm_tuning_counters {
	/** Tuning control counter array . */
	uint64_t counters[34];
};

/** Function to read the TWDM tuning control counters.
 *  The counters accumulate the different reasons for successful and
 *  unsuccessful wavelength tuning attempts.
 *
 *  \param[in] ctx PON FAPI context created by \ref fapi_pon_open, or NULL
 *  to implicitly create one for this function call.
 *  \param[in] dswlch_id Downstream wavelength channel ID for which the
 *  counters shall be reported.
 *  \param[out] param Pointer to a structure as defined
 *  by \ref pon_twdm_tuning_counters.
 *
 *  \remarks The function returns an error code in case of error.
 *  The error code is described in \ref fapi_pon_errorcode.
 *
 *  \return Return value as follows:
 *  - PON_STATUS_OK: If successful
 *  - Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_twdm_tuning_counters_get(struct pon_ctx *ctx,
				  const uint8_t dswlch_id,
				  struct pon_twdm_tuning_counters *param);

/*! @} */ /* End of TWDM functions */

/*! @} */ /* End of PON library definitions */

#endif /* _FAPI_PON_TWDM_H_ */
