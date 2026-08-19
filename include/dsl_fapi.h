/* Copyright 2017, Intel Corporation */
/******************************************************************************

                          Copyright (c) 2015-2017
                     Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _dsl_fapi_h
#define _dsl_fapi_h

#ifdef __cplusplus
#  ifndef __BEGIN_DECLS
#     define __BEGIN_DECLS extern "C" {
#  endif
#  ifndef __END_DECLS
#     define __END_DECLS   }
#  endif
#else
#  ifndef __BEGIN_DECLS
#     define __BEGIN_DECLS
#  endif
#  ifndef __END_DECLS
#     define __END_DECLS
#  endif
#endif

__BEGIN_DECLS

#include "dsl_fapi_config.h"
#include <stdint.h>
#include <stdbool.h>

/*! \file dsl_fapi.h
	\brief File contains the structure and function definitions for DSL functionality.
	The objects definitions are based on TR-181 definitions
*/

/** \defgroup FAPI_DSL DSL FAPI
	Lists the entire modules from the FAPI_DSL group.
*/

/** \addtogroup FAPI_DSL */
/* @{ */
#define DSL_CPE_FAPI_MAX_SUPPORTED_ENTITIES 2
#define DSL_CPE_FAPI_SINGLE_LINK 1
#define DSL_CPE_FAPI_BONDING 2

/*!
    \brief  WAN type enumerations
*/
typedef enum _wan_type {

	WAN_NONE = 0,		/* No WAN is active in system */
	ETH = 1,		/* Ethernet WAN is active in system */
	DSL_xTM = 2,		/* DSL with Auto-TC in system */
	DSL_PTM = 3,		/* DSL with PTM-TC in system */
	DSL_ATM = 4,		/*DSL with ATM-TC in system */
	CELL = 5		/* Cellular WAN in system */
} WAN_TYPE_t;

/**
	DSL Line object declaration, based on TR-181 definitions
*/
struct dsl_fapi_line_obj {
	/** Enables or disables the DSL line
		 This parameter is based on ifAdminStatus from [RFC2863]
		 \note marked as W (write) within TR-181 */
	bool enable;
	/** The current operational state of the DSL line (see [Section 4.2.2/TR-181i2]).
		 This parameter is based on ifOperStatus from [RFC2863]
		 Enumeration of
		 - Up
		 - Down
		 - Unknown (not used)
		 - Dormant (not used)
		 - NotPresent (not used)
		 - LowerLayerDown (not used)
		 - Error */
	char status[16];
	/** Indicates whether the interface points towards the Internet (true) or towards End Devices (false).
		 This parameter is always returned as true */
	bool upstream;
	/** A string identifying the version of the modem firmware currently installed for this interface.
		 This is applicable only when the modem firmware is separable from the overall CPE software */
	char firmware_version[64];
	/** Status of the DSL physical link. Enumeration of
		 - Up
		 - Initializing
		 - EstablishingLink
		 - NoSignal (not used, upper layer handling)
		 - Disabled
		 - Error */
	char link_status[20];
	/** List items indicate which DSL standards and recommendations are supported by the Line instance.
		 Comma-separated list of strings. Each list item is an enumeration of
		 - T1.413
		 - ETSI_101_388
		 - G.992.1_Annex_A
		 - G.992.1_Annex_B
		 - G.992.1_Annex_C (not supported)
		 - G.992.2_Annex_A
		 - G.992.2_Annex_B (not supported)
		 - G.992.2_Annex_C (not supported)
		 - G.992.3_Annex_A
		 - G.992.3_Annex_B
		 - G.992.4_Annex_A
		 - G.992.3_Annex_I
		 - G.992.3_Annex_J
		 - G.992.4_Annex_I (not supported)
		 - G.992.3_Annex_L
		 - G.992.3_Annex_M
		 - G.992.5_Annex_A
		 - G.992.5_Annex_B
		 - G.992.5_Annex_I
		 - G.992.5_Annex_J
		 - G.992.5_Annex_M
		 - G.993.2_Annex_A
		 - G.993.2_Annex_B
		 - G.993.2_Annex_C
		 \note In G.997.1, this parameter is called "xDSL Transmission system capabilities".
		       See ITU-T Recommendation [G.997.1]*/
	char standards_supported[512];
	/** This configuration parameter defines the transmission system types to be allowed by the xTU on this Line instance.
		 Each bit is 0 if not allowed or 1 if allowed, as defined for the xTU transmission system enabling (XTSE)
		 in ITU-T G.997.1
		 \note This parameter is defined as xDSL Transmission system enabling (XTSE) in ITU-T G.997.1.
		       For a full definition, see Clause 7.3.1.1.1 of ITU-T Recommendation [G.997.1]*/
	unsigned char xtse[8];
	/** The value MUST be a member of the list reported by the \ref standards_supported parameter.
		 Indicates the standard that the Line instance is using for the connection.
		 \note In G.997.1, this parameter is called "xDSL Transmission system".
		       See ITU-T Recommendation [G.997.1] */
	char standard_used[20];
	/** This parameter indicates which DSL standard and recommendation are currently in use by the Line instance.
		 Only one bit is set, as defined for the xTU transmission system enabling (XTSE) in ITU-T G.997.1*/
	unsigned char xtse_used[8];
	/** The line encoding method used in establishing the Layer 1 DSL connection between the CPE and the DSLAM.
		 This parameter is always returned as "DMT" */
	char line_encoding[5];
	/** List items indicate which VDSL2 profiles are allowed on the line.
		 Comma-separated list of strings. Each list item is an enumeration of
		 - 8a
		 - 8b
		 - 8c
		 - 8d
		 - 12a
		 - 12b
		 - 17a
		 - 30a
		 - 35b
		 \note In G.997.1, this parameter is called PROFILES.
		       See ITU-T Recommendation [G.997.1]*/
	char allowed_profiles[40];
	/** The value MUST be a member of the list reported by the \ref allowed_profiles parameter.
		 Or else be an empty string. Indicates which VDSL2 profile is currently in use on the line*/
	char current_profile[5];
	/** The power management state of the line. Enumeration of
		 - L0
		 - L1
		 - L2
		 - L3
		 - L4 (not supported)
		 \note See ITU-T Recommendation [G.997.1]. */
	char power_management_state[4];
	/** The success failure cause of the initialization.
		 An enumeration of the following integer values
		 - 0: Successful
		 - 1: Configuration error
		      This error occurs with inconsistencies in configuration parameters, e.g. when the line is
		      initialized in an xDSL Transmission system where an xTU does not support the configured
		      Maximum Delay or the configured Minimum or Maximum Data Rate for one or more bearer channels
		 - 2: Configuration not feasible on the line
		      This error occurs if the Minimum Data Rate cannot be reached on the line with the Minimum
		      Noise Margin, Maximum PSD level, Maximum Delay and Maximum Bit Error Ratio for one
		      or more bearer channels
		 - 3: Communication problem
		      This error occurs, for example, due to corrupted messages or bad syntax messages or
		      if no common mode can be selected in the G.994.1 handshaking procedure or due to a timeout
		 - 4: No peer xTU detected
		      This error occurs if the peer xTU is not powered or not connected or if the line is too
		      long to allow detection of a peer xTU
		 - 5: Any other or unknown Initialization Failure cause
		 \note In G.997.1, this parameter is called "Initialization success/failure cause"
		       See ITU-T Recommendation [G.997.1]*/
	unsigned int success_failure_cause;
	/** This parameter represents the last successful transmitted initialization state in the downstream direction
		 in the last full initialization performed on the line. Initialization states are defined in the individual
		 xDSL Recommendations and are counted from 0 (if G.994.1 is used) or 1 (if G.994.1 is not used) up to Showtime
		 This parameter needs to be interpreted along with the xDSL Transmission System
		 This parameter is available only when, after a failed full initialization, the line diagnostics procedures
		 are activated on the line
		 \note See ITU-T Recommendation [G.997.1]*/
	unsigned int last_state_transmitted_downstream;
	/** This parameter represents the last successful transmitted initialization state in the upstream direction
		 in the last full initialization performed on the line. Initialization states are defined in the individual
		 xDSL Recommendations and are counted from 0 (if G.994.1 is used) or 1 (if G.994.1 is not used) up to Showtime.
		 This parameter needs to be interpreted along with the xDSL Transmission System.
		 This parameter is available only when, after a failed full initialization, the line diagnostics procedures
		 are activated on the line.
		 \note See ITU-T Recommendation [G.997.1]*/
	unsigned int last_state_transmitted_upstream;
	/** Indicates the allowed VDSL2 US0 PSD masks for Annex A operation
		 Bit mask as specified in ITU-T Recommendation G.997.1
		 \note Note: For a VDSL2-capable multimode device operating in a mode other than VDSL2,
		       the value of this parameter SHOULD be set to 0
		 \note See ITU-T Recommendation [G.997.1] */
	unsigned int us0_mask;
	/** Reports whether trellis coding is enabled in the downstream direction
		 A value of 1 indicates that trellis coding is in use, and a value of 0 indicates that
		 the trellis is disabled
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		       the value of this parameter SHOULD be set to -1
		 \note See ITU-T Recommendation [G.997.1] */
	int trellis_ds;
	/** Reports whether trellis coding is enabled in the upstream direction.
		 A value of 1 indicates that trellis coding is in use, and a value of 0 indicates that
		 the trellis is disabled
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		       the value of this parameter SHOULD be set to -1
		 \note See ITU-T Recommendation [G.997.1] */
	int trellis_us;
	/** Reports whether the OPTIONAL virtual noise mechanism is in use in the downstream direction.
		 A value of 1 indicates the virtual noise mechanism is not in use, and a value of 2 indicates
		 the virtual noise mechanism is in use
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		       the value of this parameter SHOULD be set to 0
		 \note See ITU-T Recommendation [G.997.1] */
	unsigned int act_snr_mode_ds;
	/** Reports whether the OPTIONAL virtual noise mechanism is in use in the upstream direction.
		 A value of 1 indicates the virtual noise mechanism is not in use, and a value of 2 indicates
		 the virtual noise mechanism is in use
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		       the value of this parameter SHOULD be set to 0
		 \note See ITU-T Recommendation [G.997.1] */
	unsigned int act_snr_mode_us;
	/** Signifies the line pair that the modem is using to connection
		 LineNumber = 1 is the innermost pair */
	int line_number;
	/** The current maximum attainable data rate upstream (expressed in Kbps)
		 \note This parameter is related to the G.997.1 parameter ATTNDRus, which is measured in bits/s
		 \note See ITU-T Recommendation [G.997.1] */
	unsigned int upstream_max_bit_rate;
	/** The current maximum attainable data rate downstream (expressed in Kbps)
		 \note This parameter is related to the G.997.1 parameter ATTNDRds, which is measured in bits/s
		 \note See ITU-T Recommendation [G.997.1] */
	unsigned int downstream_max_bit_rate;
	/** The current signal-to-noise ratio margin (expressed in 0.1dB) in the upstream direction
		 \note In G.997.1, this parameter is called SNRMus
		 \note See ITU-T Recommendation [G.997.1] */
	int upstream_noise_margin;
	/** The current signal-to-noise ratio margin (expressed in 0.1dB) in the downstream direction
		 \note In G.997.1, this parameter is called SNRMds
		 \note See ITU-T Recommendation [G.997.1] */
	int downstream_noise_margin;
	/** Comma-separated list (maximum list length 24) of strings.
		 Indicates the current signal-to-noise ratio margin of each upstream band
		 \note Interpretation of the values is as defined in ITU-T Rec. G.997.1
		 \note See ITU-T Recommendation [G.997.1] */
	char snr_mpb_us[256];
	/** Comma-separated list (maximum list length 24) of strings.
		 Indicates the current signal-to-noise ratio margin of each downstream band.
		 \note Interpretation of the values is as defined in ITU-T Rec. G.997.1
		 \note See ITU-T Recommendation [G.997.1] */
	char snr_mpb_ds[256];
	/** The current output power at the CPE's DSL line (expressed in 0.1dBmV) */
	int upstream_power;
	/** The current received power at the CPE's DSL line (expressed in 0.1dBmV) */
	int downstream_power;
	/** xTU-R vendor identifier as defined in G.994.1 and T1.413.
		 In the case of G.994.1 this corresponds to the four-octet provider code,
		 which MUST be represented as eight hexadecimal digits.
		 \note This parameter is OPTIONAL at the G and S/T interfaces in G.997.1 Amendment 1
		 \note If the parameter is implemented but no value is available, it MUST have the value "00000000"
		 \note In G.997.1, this parameter is called "xTU-R G.994.1 Vendor ID"
		 \note See ITU-T Recommendation [G.997.1] */
	unsigned char xtur_vendor[4];
	/** T.35 country code of the xTU-R vendor as defined in G.994.1, where the two-octet value defined
		 in G.994.1 MUST be represented as four hexadecimal digits
		 \note This parameter is OPTIONAL at the G and S/T interfaces in G.997.1 Amendment 1
		 \note If the parameter is implemented but no value is available, it MUST have the value "0000"
		 \note In G.997.1, this parameter is called "xTU-R G.994.1 Vendor ID"
		 \note See ITU-T Recommendation [G.997.1] */
	unsigned char xtur_country[2];
	/** xTU-R T1.413 Revision Number as defined in T1.413 Issue 2
		 \note When T1.413 modulation is not in use, the parameter value SHOULD be 0 */
	unsigned int xtur_ansi_std;
	/** xTU-R Vendor Revision Number as defined in T1.413 Issue 2.
		 \note When T1.413 modulation is not in use, the parameter value SHOULD be 0 */
	unsigned int xtur_ansi_rev;
	/** xTU-C vendor identifier as defined in G.994.1 and T1.413.
		 In the case of G.994.1 this corresponds to the four-octet provider code,
		 which MUST be represented as eight hexadecimal digits
		 \note This parameter is OPTIONAL at the G and S/T interfaces in G.997.1 Amendment 1
		 \note If the parameter is implemented but no value is available, it MUST have the value "00000000" */
	unsigned int xtuc_vendor[4];
	/** T.35 country code of the xTU-C vendor as defined in G.994.1,
		 where the two-octet value defined in G.994.1 MUST be represented as four hexadecimal digits.
		 \note This parameter is OPTIONAL at the G and S/T interfaces in G.997.1 Amendment 1.
		 \note If the parameter is implemented but no value is available, it MUST have the value "0000". */
	unsigned short xtuc_country[2];
	/** xTU-C T1.413 Revision Number as defined in T1.413 Issue 2.
		 \note When T1.413 modulation is not in use, the parameter value SHOULD be 0. */
	unsigned int xtuc_ansi_std;
	/** xTU-C Vendor Revision Number as defined in T1.413 Issue 2.
		 \note When T1.413 modulation is not in use, the parameter value SHOULD be 0. */
	unsigned int xtuc_ansi_rev;
	/** Comma-separated list of unsigned integers (value 0 to 1280, or 2047).
		 VTU-R estimated upstream power back-off electrical length per band.
		 This parameter is a vector of UPBO electrical length per-band estimates for each supported downstream band,
		 expressed in 0.1 dB at 1 MHz (kl0) calculated by the VTU-R, based on separate measurements in the supported
		 downstream bands. The value of each list element ranges from 0 to 128 dB in steps of 0.1 dB, with special
		 value 204.7 which indicates that the estimate is greater than 128 dB.
		 This parameter is required for the alternative electrical length estimation method (ELE-M1).
		 The value of each list element is coded as an unsigned 16 bit number in the range 0 (coded as 0)
		 to 128 dB (coded as 1280) in steps of 0.1 dB.

		 \note This parameter is defined as UPBOKLE-R-pb in Clause 7.5.1.23.4 of ITU-T Recommendation [G.997.1]. */
	char upbokler_pb[26];
	/** UPBO downstream receiver signal level threshold.
		 This parameter reports the downstream received signal level threshold value used in the alternative electrical
		 length estimation method (ELE-M1). This parameter represents an offset from -100 dBm/Hz, and ranges
		 from -64 dB to 0 dB in steps of 0.1 dB.
		 The value is coded as a 16 bit number in the range -64 (coded as -640) to 0 dB (coded as 0) in steps of 0.1 dB.

		 \note See ITU-T Recommendation [G.997.1]. */
	char rxthrsh_ds[26];
	/** This parameter indicates the actual active rate adaptation mode in the downstream direction.
		 - 1: the link is operating in RA-MODE 1 (MANUAL).
		 - 2: the link is operating in RA-MODE 2 (AT_INIT).
		 - 3: the link is operating in RA-MODE 3 (DYNAMIC).
		 - 4: the link is operating in RA-MODE 4 (DYNAMIC with SOS).
		 \note This parameter is defined as ACT-RA-MODEds in Clause 7.5.1.33.1 of ITU-T Recommendation [G.997.1]. */
	unsigned int act_ra_mode_ds;
	/** This parameter indicates the actual active rate adaptation mode in the upstream direction.
		 - 1: the link is operating in RA-MODE 1 (MANUAL).
		 - 2: the link is operating in RA-MODE 2 (AT_INIT).
		 - 3: the link is operating in RA-MODE 3 (DYNAMIC).
		 - 4: the link is operating in RA-MODE 4 (DYNAMIC with SOS).
		 \note This parameter is defined as ACT-RA-MODEus in Clause 7.5.1.33.2 of ITU-T Recommendation [G.997.1]. */
	unsigned int act_ra_mode_us;
	/** This parameter reports the actual signal-to-noise margin of the robust overhead channel (ROC)
		 in the upstream direction (expressed in 0.1 dB). The format is identical to the format of the line
		 status parameter SNRM margin (upstream_noise_margin).
		 \note This parameter is defined as SNRM-ROC-us in Clause 7.5.1.35.2 ITU-T Recommendation [G.997.1]. */
	unsigned int snr_mroc_us;
	/** The current upstream signal loss (expressed in 0.1dB). */
	int upstream_attenuation;
	/** The current downstream signal loss (expressed in 0.1dB). */
	int downstream_attenuation;
};

/**
	DSL Line Stats object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_line_stats_obj {
	/** The number of seconds since the beginning of the period used for collection of Total statistics.
		 Statistics SHOULD continue to be accumulated across CPE reboots,
		 though this might not always be possible.
		 \note total_start SHOULD NOT be reset when the interface statistics are reset via
		 an interface disable / enable cycle. */
	unsigned int total_start;
	/** The number of seconds since the most recent DSL Showtime - the beginning of the period used
		 for collection of Showtime statistics.
		 Showtime is defined as successful completion of the DSL link establishment process.
		 The Showtime statistics are those collected since the most recent establishment of the DSL link.
		 \note showtime_start SHOULD NOT be reset when the interface statistics are reset via
		 an interface disable / enable cycle. */
	unsigned int showtime_start;
	/** The number of seconds since the second most recent DSL Showtime-the beginning of the period used for collection
		 of LastShowtime statistics.
		 If the CPE has not retained information about the second most recent Showtime (e.g., on reboot),
		 the start of LastShowtime statistics MAY temporarily coincide with the start of Showtime statistics.
		 \note last_showtime_start SHOULD NOT be reset when the interface statistics are reset via
		 an interface disable / enable cycle. */
	unsigned int last_showtime_start;
	/** The number of seconds since the beginning of the period used for collection of CurrentDay statistics.
		 The CPE MAY align the beginning of each CurrentDay interval with days in the UTC time zone,
		 but is not required to do so.
		 Statistics SHOULD continue to be accumulated across CPE reboots, though this might not always be possible.
		 \note current_day_start SHOULD NOT be reset when the interface statistics are reset via
		 an interface disable / enable cycle. */
	unsigned int current_day_start;
	/** The number of seconds since the beginning of the period used for collection of QuarterHour statistics.
		 The CPE MAY align the beginning of each QuarterHour interval with real-time quarter-hour intervals,
		 but is not required to do so.
		 Statistics SHOULD continue to be accumulated across CPE reboots, though this might not always be possible.
		 \note quarter_hour_start SHOULD NOT be reset when the interface statistics are reset via
		 an interface disable / enable cycle. */
	unsigned int quarter_hour_start;
};

/**
	DSL Line object total statistics declaration, based on TR-181 definitions.
	DSL Line object showtime statistics declaration, based on TR-181 definitions.
	DSL Line object last showtime statistics declaration, based on TR-181 definitions.
	DSL Line object current day statistics declaration, based on TR-181 definitions.
	DSL Line object quarter hour statistics declaration, based on TR-181 definitions.
*/
struct dsl_fapi_line_stats_interval_obj {
	/** Common data structure for the several objects.
		 - Stats.Total: Total number of errored seconds.
		 \note (ES-L as defined in ITU-T Rec. [G.997.1]).
		 - Stats.Showtime: Number of errored seconds since the most recent DSL Showtime.
		 \note (ES-L as defined in ITU-T Rec. [G.997.1]).
		 - Stats.LastShowtime: Number of errored seconds since the second most recent DSL Showtime.
		 \note (ES-L as defined in ITU-T Rec. [G.997.1]).
		 - Stats.CurrentDay: Number of errored seconds accumulated during the current day.
		 \note (ES-L as defined in ITU-T Rec. [G.997.1]).
		 - Stats.QuarterHour: Number of errored seconds accumulated during the current quarter hour.
		 \note (ES-L as defined in ITU-T Rec. [G.997.1]).
		 \note This parameter is OPTIONAL at the G and S/T interfaces in G.997.1 Amendment 1. */
	unsigned int errored_secs;
	/** Common data structure for the several objects.
		 - Stats.Total: Total number of severely errored seconds.
		 \note (SES-L as defined in ITU-T Rec. [G.997.1]).
		 - Stats.Showtime: Number of severely errored seconds since the most recent DSL Showtime.
		 \note (SES-L as defined in ITU-T Rec. [G.997.1]).
		 - Stats.LastShowtime: Number of severely errored seconds since the second most recent DSL Showtime.
		 \note (SES-L as defined in ITU-T Rec. [G.997.1]).
		 - Stats.CurrentDay: Number of severely errored seconds accumulated during the current day.
		 \note (SES-L as defined in ITU-T Rec. [G.997.1]).
		 - Stats.QuarterHour: Number of severely errored seconds accumulated during the current quarter hour.
		 \note (SES-L as defined in ITU-T Rec. [G.997.1]).
		 \note This parameter is OPTIONAL at the G and S/T interfaces in G.997.1 Amendment 1. */
	unsigned int severely_errored_secs;
};

/**
	DSL Line TestParams object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_line_test_params_obj {
	/** Number of sub-carriers per sub-carrier group in the downstream direction for \ref hlogps_ds.
		 Valid values are 1, 2, 4, and 8.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to 1. */
	unsigned int hlogg_ds;
	/** Number of sub-carriers per sub-carrier group in the upstream direction for \ref hlogps_us.
		 Valid values are 1, 2, 4, and 8.

		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to 1. */
	unsigned int hlogg_us;
	/** Comma-separated list (maximum list length 2559) of strings.
		 Indicates the downstream logarithmic line characteristics per sub-carrier group.
		 The maximum number of elements is 256 for G.992.3, and 512 for G.992.5. For G.993.2,
		 the number of elements will depend on the value of HLOGGds but will not exceed 512.
		 Interpretation of the values is as defined in ITU-T Rec. G.997.1.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to None.
		 \note hlogps_ds is measured during initialization and is not updated during Showtime. */
	char hlogps_ds[2562];
	/** Comma-separated list (maximum list length 2559) of strings.
		 Indicates the upstream logarithmic line characteristics per sub-carrier group.
		 The maximum number of elements is 64 for G.992.3 and G.992.5. For G.993.2,
		 the number of elements will depend on the value of HLOGGus but will not exceed 512.
		 Interpretation of the values is as defined in ITU-T Rec. G.997.1.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to None.
		 \note hlogps_us is measured during initialization and is not updated during Showtime. */
	char hlogps_us[2562];
	/** Indicates the number of symbols over which \ref hlogps_ds was measured.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to 0. */
	unsigned int hlogmt_ds;
	/** Indicates the number of symbols over which \ref hlogps_us was measured.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to 0. */
	unsigned int hlogmt_us;
	/** Number of sub-carriers per sub-carrier group in the downstream direction for \ref qlnps_ds.
		 Valid values are 1, 2, 4, and 8.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to 1. */
	unsigned int qlng_ds;
	/** Number of sub-carriers per sub-carrier group in the upstream direction for \ref qlnps_us.
		 Valid values are 1, 2, 4, and 8.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to 1. */
	unsigned int qlng_us;
	/** Comma-separated list (maximum list length 2047) of integers.
		 Indicates the downstream quiet line noise per subcarrier group.
		 The maximum number of elements is 256 for G.992.3 and G.992.5. For G.993.2, the number of
		 elements will depend on the value of QLNGds but will not exceed 512.
		 Interpretation of the values is as defined in ITU-T Rec. G.997.1.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to None.
		 \note qlnps_ds is measured during initialization and is not updated during Showtime. */
	char qlnps_ds[2049];
	/** Comma-separated list (maximum list length 2047) of strings.
		 Indicates the upstream quiet line noise per subcarrier group.
		 The maximum number of elements is 64 for G.992.3 and G.992.5. For G.993.2, the number of
		 elements will depend on the value of QLNGus but will not exceed 512.
		 Interpretation of the values is as defined in ITU-T Rec. G.997.1.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to None.
		 \note qlnps_us is measured during initialization and is not updated during Showtime. */
	char qlnps_us[2049];
	/** Indicates the number of symbols over which \ref qlnps_ds was measured.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to 0. */
	unsigned int qlnmt_ds;
	/** Indicates the number of symbols over which \ref qlnps_us was measured.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to 0. */
	unsigned int qlnmt_us;
	/** Number of sub-carriers per sub-carrier group in the downstream direction for \ref snrps_ds.
		 Valid values are 1, 2, 4, and 8.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to 1. */
	unsigned int snrg_ds;
	/** Number of sub-carriers per sub-carrier group in the upstream direction for \ref snrps_us.
		 Valid values are 1, 2, 4, and 8.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to 1. */
	unsigned int snrg_us;
	/** Comma-separated list (maximum list length 2047) of integers.
		 Indicates the downstream SNR per subcarrier group. The maximum number of elements is 256 for G.992.3,
		 and 512 for G.992.5. For G.993.2, the number of elements will depend on the value of snrg_ds
		 but will not exceed 512. Interpretation of the values is as defined in ITU-T Rec. G.997.1.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to None.
		 \note snrps_ds is first measured during initialization and is updated during Showtime. */
	char snrps_ds[2049];
	/** Comma-separated list (maximum list length 2047) of strings.
		 Indicates the upstream SNR per subcarrier group. The maximum number of elements is 64 for G.992.3 and G.992.5.
		 For G.993.2, the number of elements will depend on the value of snrg_us but will not exceed 512.
		 Interpretation of the values is as defined in ITU-T Rec. G.997.1.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to None.
		 \note snrps_us is first measured during initialization and is updated during Showtime. */
	char snrps_us[2049];
	/** Indicates the number of symbols over which \ref snrps_ds was measured.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to 0. */
	unsigned int snrmt_ds;
	/** Indicates the number of symbols over which \ref snrps_us was measured.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to 0. */
	unsigned int snrmt_us;
	/** Comma-separated list (maximum list length 24) of strings.
		 Indicates the downstream line attenuation per usable band, as computed during initialization.
		 Number of elements is dependent on the number of downstream bands but will exceed one only for G.993.2.
		 Interpretation of latn_ds is as defined in ITU-T Rec. G.997.1.
		 \note See ITU-T Recommendation [G.997.1]. */
	char latn_ds[24];
	/** Comma-separated list (maximum list length 24) of strings.
		 Indicates the upstream line attenuation per usable band, as computed during initialization.
		 Number of elements is dependent on the number of upstream bands but will exceed one only for G.993.2.
		 Interpretation of latn_us is as defined in ITU-T Rec. G.997.1.
		 \note See ITU-T Recommendation [G.997.1]. */
	char latn_us[24];
	/** Comma-separated list (maximum list length 24) of strings.
		 Indicates the downstream signal attenuation per usable band, as computed during the L0 (i.e., Showtime) state.
		 Number of elements is dependent on the number of downstream bands but will exceed one only for G.993.2.
		 Interpretation of satn_ds is as defined in ITU-T Rec. G.997.1.
		 \note See ITU-T Recommendation [G.997.1]. */
	char satn_ds[24];
	/** Comma-separated list (maximum list length 24) of strings.
		 Indicates the upstream signal attenuation per usable band, as computed during the L0 (i.e., Showtime) state.
		 Number of elements is dependent on the number of downstream bands but will exceed one only for G.993.2.
		 Interpretation of satn_us is as defined in ITU-T Rec. G.997.1.
		 \note See ITU-T Recommendation [G.997.1]. */
	char satn_us[24];
};

/**
	DSL Channel object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_channel_obj {
	/** Enables or disables the channel.
		 \note This parameter is based on ifAdminStatus from [RFC2863].
		 \note marked as W (write) within TR-181 */
	bool enable;
	/** The current operational state of the DSL channel (see [Section 4.2.2/TR-181i2]).
		 This parameter is based on ifOperStatus from [RFC2863].
		 Enumeration of
		 - Up
		 - Down
		 - Unknown (not used)
		 - Dormant (not used)
		 - NotPresent (not used)
		 - LowerLayerDown (not used)
		 - Error */
	char status[16];
	/** Comma-separated list of strings. List items indicate which link encapsulation standards and
		 recommendations are supported by the Channel instance.
		 Enumeration of:
		 - G.992.3_Annex_K_ATM
		 - G.992.3_Annex_K_PTM
		 - G.993.2_Annex_K_ATM
		 - G.993.2_Annex_K_PTM
		 - G.994.1 (Auto) */
	char link_encapsulation_supported[82];
	/** Indicates the link encapsulation standard that the Channel instance is using for the connection.
		 Enumeration of:
		 - G.992.3_Annex_K_ATM
		 - G.992.3_Annex_K_PTM
		 - G.993.2_Annex_K_ATM
		 - G.993.2_Annex_K_PTM */
	char link_encapsulation_used[40];
	/** Reports the index of the latency path supporting the bearer channel.
		 \note See ITU-T Recommendation [Section 7.5.2.7/G.997.1]. */
	unsigned int lpath;
	/** Reports the interleaver depth D for the latency path indicated in \ref lpath.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to 0. */
	unsigned int intlvdepth;
	/** Reports the interleaver block length in use on the latency path indicated in \ref lpath.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to -1. */
	int intlvblock;
	/** Reports the actual delay, in milliseconds, of the latency path due to interleaving.
		 \note See ITU-T Recommendation [G.997.1]. */
	unsigned int actual_interleaving_delay;
	/** Reports the actual impulse noise protection (INP) provided by the latency path indicated in \ref lpath.
		 The value is the actual INP in the L0 (i.e., Showtime) state.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to -1. */
	int actinp;
	/** Reports whether the value reported in \ref actinp was computed assuming the receiver does not use erasure decoding.
		 Valid values:
		 - 0 (computed per the formula assuming no erasure decoding)
		 - 1 (computed by taking into account erasure decoding capabilities of receiver).
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to false. */
	bool inpreport;
	/** Reports the size, in octets, of the Reed-Solomon codeword in use on the latency path indicated in \ref lpath.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to -1. */
	int nfec;
	/** Reports the number of redundancy bytes per Reed-Solomon codeword on the latency path indicated in \ref lpath.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to -1. */
	int rfec;
	/** Reports the number of bits per symbol assigned to the latency path indicated in \ref lpath.
		 This value does not include overhead due to trellis coding.
		 \note See ITU-T Recommendation [G.997.1].
		 \note For a multimode device operating in a mode in which this parameter does not apply,
		 the value of this parameter SHOULD be set to -1. */
	int lsymb;
	/** The current physical layer aggregate data rate (expressed in Kbps) of the upstream DSL connection. */
	unsigned int upstream_curr_rate;
	/** The current physical layer aggregate data rate (expressed in Kbps) of the downstream DSL connection. */
	unsigned int downstream_curr_rate;
	/** Obsolete actual net data rate */
	unsigned int actndr;
	/** Actual net data rate expressed in Kbps in downstream direction:
		 In L0 state, this parameter reports the net data rate at which the bearer channel is operating.
		 In L2 state, the parameter contains the net data rate in the previous L0 state.
		 \note See [ITU T G.992.3], [ITU T G.992.5], [ITU T G.993.2]
		 \note See ITU-T Recommendation [G.997.1]." */
	unsigned int actndr_ds;
	/** Actual net data rate expressed in Kbps in upstream direction:
		 In L0 state, this parameter reports the net data rate at which the bearer channel is operating.
		 In L2 state, the parameter contains the net data rate in the previous L0 state.
		 \note See [ITU T G.992.3], [ITU T G.992.5], [ITU T G.993.2]
		 \note See ITU-T Recommendation [G.997.1]." */
	unsigned int actndr_us;
	/** Obsolete actual impulse noise protection */
	unsigned int actinprein;
	/** Actual impulse noise protection in downstream direction against REIN, expressed in 0.1 DMT symbols.
		 If retransmission is used in a given transmit direction, this parameter reports the
		 actual impulse noise protection (INP) against REIN (under specific conditions detailed in [ITU T G.998.4])
		 on the bearer channel in the L0 state. In the L2 state, the parameter contains the INP in the previous L0 state.
		 The value is coded in fractions of DMT symbols with a granularity of 0.1 symbols.
		 The range is from 0 to 25.4. A special value of 25.5 indicates an ACTINP_REIN of 25.5 or higher.
		 \note This parameter is defined as ACTINP_REIN in Clause 7.5.2.9 of ITU-T Recommendation [G.997.1]. */
	unsigned int actinprein_ds;
	/** Actual impulse noise protection in upstream direction against REIN, expressed in 0.1 DMT symbols.
		 If retransmission is used in a given transmit direction, this parameter reports the
		 actual impulse noiseprotection (INP) against REIN (under specific conditions detailed in [ITU T G.998.4])
		 on the bearer channel in the L0 state. In the L2 state, the parameter contains the INP in the previous L0 state.
		 The value is coded in fractions of DMT symbols with a granularity of 0.1 symbols.
		 The range is from 0 to 25.4. A special value of 25.5 indicates an ACTINP_REIN of 25.5 or higher.
		 \note This parameter is defined as ACTINP_REIN in Clause 7.5.2.9 of ITU-T Recommendation [G.997.1]. */
	unsigned int actinprein_us;
};

/**
	DSL Channel Stats object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_channel_stats_obj {
	/** The Number of seconds since the beginning of the period used for collection of Total statistics.
		 Statistics SHOULD continue to be accumulated across CPE reboots, though this might not always be possible.
		 \note total_start SHOULD NOT be reset when the interface statistics are reset via
		 an interface disable / enable cycle. */
	unsigned int total_start;
	/** The Number of seconds since the most recent DSL Showtime - the beginning of the period
		 used for collection of Showtime statistics.
		 Showtime is defined as successful completion of the DSL link establishment process.
		 The Showtime statistics are those collected since the most recent establishment of the DSL link.
		 \note showtime_start SHOULD NOT be reset when the interface statistics are reset via
		 an interface disable / enable cycle. */
	unsigned int showtime_start;
	/** The Number of seconds since the second most recent DSL Showtime-the beginning of the period
		 used for collection of LastShowtime statistics.
		 If the CPE has not retained information about the second most recent Showtime (e.g., on reboot),
		 the start of LastShowtime statistics MAY temporarily coincide with the start of Showtime statistics.
		 \note last_showtime_start SHOULD NOT be reset when the interface statistics are reset via
		 an interface disable / enable cycle. */
	unsigned int last_showtime_start;
	/** The Number of seconds since the beginning of the period used for collection of CurrentDay statistics.
		 Statistics SHOULD continue to be accumulated across CPE reboots, though this might not always be possible.
		 \note current_day_start SHOULD NOT be reset when the interface statistics are reset via
		 an interface disable / enable cycle. */
	unsigned int current_day_start;
	/** The Number of seconds since the beginning of the period used for collection of QuarterHour statistics.
		 Statistics SHOULD continue to be accumulated across CPE reboots, though this might not always be possible.
		 \note quarter_hour_start SHOULD NOT be reset when the interface statistics are reset via
		 an interface disable / enable cycle. */
	unsigned int quarter_hour_start;
};

/**
	DSL BondignGroup.{i}. object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_bonding_obj {
	/** Enables or disables the bonding group.
		This parameter is based on ifAdminStatus from [RFC2863]. */
	bool enable;
	/** The current operational state of the bonding group (see [Section 4.2.2/TR-181i2]).
		Enumeration of:
		Up, Down, Unknown, Dormant, NotPresent, LowerLayerDown, Error (OPTIONAL)
		When Enable is false then Status SHOULD normally be Down
		(or NotPresent or Error if there is a fault condition on the interface).
		When Enable is changed to true then Status SHOULD change to Up if and only
		if the interface is able to transmit and receive network traffic;
		it SHOULD change to Dormant if and only if the interface is operable
		but is waiting for external actions before it can transmit and receive
		network traffic (and subsequently change to Up if still operable when the
		expected actions have completed); it SHOULD change to LowerLayerDown if
		and only if the interface is prevented from entering the Up state because
		one or more of the interfaces beneath it is down; it SHOULD remain in the
		Error state if there is an error or other fault condition detected on the
		interface; it SHOULD remain in the NotPresent state if the interface has missing
		(typically hardware) components; it SHOULD change to Unknown if the state of the
		interface can not be determined for some reason.
		This parameter is based on ifOperStatus from [RFC2863].*/
	char status[16];
	/** The accumulated time in seconds since the bonding group entered its current operational state. */
	unsigned int last_change;
	/** Comma-separated list (maximum list length 1024) of strings.
		Each list item MUST be the path name of an interface object that
		is stacked immediately below this interface object.
		If the referenced object is deleted, the corresponding item MUST
		be removed from the list. See [Section 4.2.1/TR-181i2].
		LowerLayers is read-only for this object because bonding is expected
		to be configured by the CPE, not by the ACS. */
	char lower_layers[1024];
	/** Comma-separated list of strings. Supported DSL bonding schemes.
		Each list item is an enumeration of:
		ATM ([G.998.1] ATM-based bonding)
		Ethernet ([G.998.2] Ethernet-based bonding)
		TDIM ([G.998.3] TDIM-based bonding)
		Corresponds to [TR-159] oBondingGroup.aGroupBondSchemesSupported.
		ATM,Ethernet,TDIM */
	char bond_chemes_supported[19];
	/** The value MUST be a member of the list reported by the BondSchemesSupported parameter.
		Currently operating bonding scheme. Corresponds to [TR-159] aGroupOperBondScheme. */
	char bond_scheme[10];
	/** DSL bonding group capacity, i.e. the maximum number of channels that can be
		bonded in this group. Corresponds to [TR-159] oBondingGroup.aGroupCapacity. */
	unsigned int group_capacity;
	/** The accumulated time in seconds for which this bonding group has been operationally up.
		Corresponds to [Section 11.4.2/G.998.1] Group Running Time.*/
	unsigned int running_time;

};

/**
	DSL BondignGroup.{i}.BondedChannel.{i}. object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_bonding_channel_obj {
	/** The value MUST be the path name of a row in the DSL.Channel table.
		This is the channel that is being bonded.
		This is read-only because bonding is expected to be configured by
		the CPE, not by the ACS. */
	char channel[256];
};

/**
	DSL BondignGroup.{i}.BondedChannel.{i}.Ethernet. object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_bonding_channel_ethernet_obj {

};

/**
	DSL BondignGroup.{i}.BondedChannel.{i}.Ethernet.Stats. object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_bonding_channel_ethernet_stats_obj {
};

/**
	DSL BondignGroup.{i}.Stats. object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_bonding_stats_obj {
	/** The Number of seconds since the beginning of the period  used for collection of Total statistics.
		Statistics SHOULD continue to be accumulated across CPE reboots,
		though this might not always be possible.
		Note: TotalStart SHOULD NOT be reset when the interface statistics
 	 	are reset via an interface disable / enable cycle. */
	unsigned int total_start;
	/** The number of seconds since the beginning of the period used for collection of CurrentDay statistics.
		The CPE MAY align the beginning of each CurrentDay interval with days in the UTC time zone,
		but is not required to do so.
		Statistics SHOULD continue to be accumulated across CPE reboots, though this might not always be possible.
		\note current_day_start SHOULD NOT be reset when the interface statistics are reset via
		an interface disable / enable cycle. */
	unsigned int current_day_start;
	/** The number of seconds since the beginning of the period used for collection of QuarterHour statistics.
		The CPE MAY align the beginning of each QuarterHour interval with real-time quarter-hour intervals,
		but is not required to do so.
		Statistics SHOULD continue to be accumulated across CPE reboots, though this might not always be possible.
		\note quarter_hour_start SHOULD NOT be reset when the interface statistics are reset via
		an interface disable / enable cycle. */
	unsigned int quarter_hour_start;
};

/**
	DSL BondignGroup.{i}.Stats.Total. object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_bonding_stats_total_obj {
	/** The achieved upstream data rate in bits per second (which might change subject to
		dynamic link usage conditions). Corresponds to [Section 11.4.2/G.998.1]
		Achieved Aggregate Data Rate. */
	unsigned int upstream_rate;
	/** The achieved downstream data rate in bits per second (which might change subject
		to dynamic link usage conditions).
		Corresponds to [Section 11.4.2/G.998.1] Achieved Aggregate Data Rate. */
	unsigned int downstram_rate;
	/** The achieved upstream differential delay in milliseconds
		(which might change subject to dynamic link usage conditions).*/
	unsigned int upstream_differential_delay;
	/** The achieved downstream differential delay in milliseconds
		(which might change subject to dynamic link usage conditions). */
	unsigned int downstram_differential_delay;
	/** The number of times that the group was declared Unavailable during the accumulation period.
		Corresponds to [Section 11.4.3/G.998.1] Group Failure Count. */
	unsigned int failure_count;
	/** The time in seconds during which the group was declared Errored during the accumulation period.
		Corresponds to [TR-159] oBondingGroup.aGroupPerf**ES. */
	unsigned int errored_seconds;
	/** The time in seconds during which the group was declared SeverelyErrored during the accumulation period.
		Corresponds to [TR-159] oBondingGroup.aGroupPerf**SES. */
	unsigned int severely_errored_seconds;
	/** The time in seconds during which the group was declared Unavailable during the accumulation period.
		Corresponds to [TR-159] oBondingGroup.aGroupPerf**UAS. */
	unsigned int unavailable_seconds;
};

/**
	DSL BondignGroup.{i}.Stats.CurrentDay. object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_bonding_stats_day_obj {
	/** Comma-separated list of strings. Indicates the failure conditions that have
		occurred during the accumulation period. Each list item is an enumeration of:
		LowRate (Minimum data rate not met)
		ExcessiveDelay (Differential delay tolerance exceeded)
		InsufficientBuffers (Insufficient buffers on receiver)
		Other (Other failure occurred)
		Corresponds to [Section 11.4.3/G.998.1] Current Group Failure Reason. */
	char failure_reasons[64];
	/** The achieved upstream data rate in bits per second (which might change subject to dynamic
	link usage conditions). Corresponds to [Section 11.4.2/G.998.1] Achieved Aggregate Data Rate. */
	unsigned int upstream_rate;
	/** The achieved downstream data rate in bits per second (which might change subject to dynamic
		link usage conditions). Corresponds to [Section 11.4.2/G.998.1] Achieved Aggregate Data Rate. */
	unsigned int downstream_rate;
	/** The achieved upstream differential delay in milliseconds (which might change subject
		to dynamic link usage conditions). */
	unsigned int upstream_differential_delay;
	/** The achieved downstream differential delay in milliseconds
		(which might change subject to dynamic link usage conditions). */
	unsigned int downstream_differential_delay;
	/** The number of times that the group was declared Unavailable during the accumulation period.
		Corresponds to [Section 11.4.3/G.998.1] Group Failure Count. */
	unsigned int failure_count;
	/** The time in seconds during which the group was declared Errored during the accumulation period.
		Corresponds to [TR-159] oBondingGroup.aGroupPerf**ES. */
	unsigned int errored_seconds;
	/** The time in seconds during which the group was declared SeverelyErrored during the accumulation period.
		Corresponds to [TR-159] oBondingGroup.aGroupPerf**SES. */
	unsigned int severely_errored_seconds;
	/** The time in seconds during which the group was declared Unavailable during the accumulation period.
		Corresponds to [TR-159] oBondingGroup.aGroupPerf**UAS. */
	unsigned int unavailable_seconds;
};

/**
	DSL BondignGroup.{i}.Stats.QuarterHour. object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_bonding_stats_hour_obj {
	/** Comma-separated list of strings. Indicates the failure conditions that have
		occurred during the accumulation period. Each list item is an enumeration of:
		LowRate (Minimum data rate not met)
		ExcessiveDelay (Differential delay tolerance exceeded)
		InsufficientBuffers (Insufficient buffers on receiver)
		Other (Other failure occurred)
		Corresponds to [Section 11.4.3/G.998.1] Current Group Failure Reason. */
	char failure_reasons[64];
	/** The achieved upstream data rate in bits per second (which might change subject to dynamic
	link usage conditions). Corresponds to [Section 11.4.2/G.998.1] Achieved Aggregate Data Rate. */
	unsigned int upstream_rate;
	/** The achieved downstream data rate in bits per second (which might change subject to dynamic
		link usage conditions). Corresponds to [Section 11.4.2/G.998.1] Achieved Aggregate Data Rate. */
	unsigned int downstream_rate;
	/** The achieved upstream differential delay in milliseconds (which might change subject
		to dynamic link usage conditions). */
	unsigned int upstream_differential_delay;
	/** The achieved downstream differential delay in milliseconds
		(which might change subject to dynamic link usage conditions). */
	unsigned int downstream_differential_delay;
	/** The number of times that the group was declared Unavailable during the accumulation period.
		Corresponds to [Section 11.4.3/G.998.1] Group Failure Count. */
	unsigned int failure_count;
	/** The time in seconds during which the group was declared Errored during the accumulation period.
		Corresponds to [TR-159] oBondingGroup.aGroupPerf**ES. */
	unsigned int errored_seconds;
	/** The time in seconds during which the group was declared SeverelyErrored during the accumulation period.
		Corresponds to [TR-159] oBondingGroup.aGroupPerf**SES. */
	unsigned int severely_errored_seconds;
	/** The time in seconds during which the group was declared Unavailable during the accumulation period.
		Corresponds to [TR-159] oBondingGroup.aGroupPerf**UAS. */
	unsigned int unavailable_seconds;

};

/**
	DSL BondignGroup.{i}.Ethernet. object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_bonding_ethernet_obj {

};

/**
	DSL BondignGroup.{i}.Ethernet.Stats. object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_bonding_ethernet_stats_obj {
};

/**
	DSL Channel total statistics object declaration, based on TR-181 definitions.
	DSL Channel showtime statistics object declaration, based on TR-181 definitions.
	DSL Channel last showtime statistics object declaration, based on TR-181 definitions.
	DSL Channel current day statistics object declaration, based on TR-181 definitions.
	DSL Channel current quarter hour object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_channel_stats_interval_obj {
	/** Common data structure for the several objects.
		 - Stats.Total: Total number of FEC errors detected.
		 - Stats.Showtime: Number of FEC errors detected since the most recent DSL Showtime.
		 - Stats.LastShowtime: Number of FEC errors detected since the second most recent DSL Showtime.
		 - Stats.CurrentDay: Number of FEC errors detected since the second most recent DSL Showtime.
		 - Stats.QuarterHour: Number of FEC errors detected since the second most recent DSL Showtime.
 		 \note (FEC-C as defined in ITU-T Rec. [G.997.1]). */
	unsigned int xtu_rfec_errors;
	/** Common data structure for the several objects.
		 - Stats.Total: Total number of FEC errors detected by the ATU-C.
		 - Stats.Showtime: Number of FEC errors detected by the ATU-C since the most recent DSL Showtime.
		 - Stats.LastShowtime: Number of FEC errors detected by the ATU-C since the second most recent DSL Showtime.
		 - Stats.CurrentDay: Number of FEC errors detected by the ATU-C since the second most recent DSL Showtime.
		 - Stats.QuarterHour: Number of FEC errors detected by the ATU-C since the second most recent DSL Showtime.
 		 \note (FEC-CFE as defined in ITU-T Rec. [G.997.1]). */
	unsigned int xtu_cfec_errors;
	/** Common data structure for the several objects.
		 - Stats.Total: Total number of HEC errors detected.
		 - Stats.Showtime: Number of HEC errors detected since the most recent DSL Showtime.
		 - Stats.LastShowtime: Number of HEC errors detected since the second most recent DSL Showtime.
		 - Stats.CurrentDay: Number of HEC errors detected since the second most recent DSL Showtime.
		 - Stats.QuarterHour: Number of HEC errors detected since the second most recent DSL Showtime.
 		 \note (HEC-P as defined in ITU-T Rec. [G.997.1]). */
	unsigned int xtu_rhec_errors;
	/** Common data structure for the several objects.
		 - Stats.Total: Total number of HEC errors detected by the ATU-C.
		 - Stats.Showtime: Number of HEC errors detected by the ATU-C since the most recent DSL Showtime.
		 - Stats.LastShowtime: Number of HEC errors detected by the ATU-C since the second most recent DSL Showtime.
		 - Stats.CurrentDay: Number of HEC errors detected by the ATU-C since the second most recent DSL Showtime.
		 - Stats.QuarterHour: Number of HEC errors detected by the ATU-C since the second most recent DSL Showtime.
 		 \note (HEC-PFE as defined in ITU-T Rec. [G.997.1]). */
	unsigned int xtu_chec_errors;
	/** Common data structure for the several objects.
		 - Stats.Total: Total number of CRC errors detected.
		 - Stats.Showtime: Number of CRC errors detected since the most recent DSL Showtime.
		 - Stats.LastShowtime: Number of CRC errors detected since the second most recent DSL Showtime.
		 - Stats.CurrentDay: Number of CRC errors detected since the second most recent DSL Showtime.
		 - Stats.QuarterHour: Number of CRC errors detected since the second most recent DSL Showtime.
 		 \note (CV-C as defined in ITU-T Rec. [G.997.1]). */
	unsigned int xtu_rcrc_errors;
	/** Common data structure for the several objects.
		 - Stats.Total: Total number of CRC errors detected by the ATU-C.
		 - Stats.Showtime: Number of CRC errors detected by the ATU-C since the most recent DSL Showtime.
		 - Stats.LastShowtime: Number of CRC errors detected by the ATU-C since the second most recent DSL Showtime.
		 - Stats.CurrentDay: Number of CRC errors detected by the ATU-C since the second most recent DSL Showtime.
		 - Stats.QuarterHour: Number of CRC errors detected by the ATU-C since the second most recent DSL Showtime.
 		 \note (CV-CFE as defined in ITU-T Rec. [G.997.1]). */
	unsigned int xtu_ccrc_errors;
};

/**
	DSL X_LANTIQ_COM_Config object declaration, based on TR-181 definitions.
*/
struct dsl_fapi_x_lantiq_com_config_obj {
	/** Enables or disables the Bitswap feature for Upstream direction on the DSL line in VDSL.
		 Initial default: Enabled (bitswap_us_v=1)
		 \note marked as W (write) within TR-181 */
	bool bitswap_us_v;
	/** Enables or disables the Bitswap feature for Upstream direction on the DSL line in ADSL.
		 Initial default: Enabled (bitswap_us_a=1)
		 \note marked as W (write) within TR-181 */
	bool bitswap_us_a;
	/** Enables or disables the Bitswap feature for Downstream direction on the DSL line in VDSL.
		 Initial default: Enabled (bitswap_ds_v=1)
		 \note marked as W (write) within TR-181 */
	bool bitswap_ds_v;
	/** Enables or disables the Bitswap feature for Downstream direction on the DSL line in ADSL.
		 Initial default: Enabled (bitswap_ds_a=1)
		 \note marked as W (write) within TR-181 */
	bool bitswap_ds_a;
	/** Enables or disables the Retransmission feature for Upstream direction on the DSL line.
		 Initial default: Enabled (retx_us=1)
		 \note marked as W (write) within TR-181 */
	bool retx_us;
	/** Enables or disables the Retransmission feature for Downstream direction on the DSL line in VDSL.
		 Initial default: Enabled (retx_ds_v=1)
		 \note marked as W (write) within TR-181 */
	bool retx_ds_v;
	/** Enables or disables the Retransmission feature for Downstream direction on the DSL line in ADSL.
		 Initial default: Enabled (retx_ds_a=0)
		 \note marked as W (write) within TR-181 */
	bool retx_ds_a;
	/** Enables or disables SRA feature for Upstream direction on the DSL line in VDSL.
		Initial default: Disabled (sra_us_v=1)
		 \note marked as W (write) within TR-181 */
	bool sra_us_v;
	/** Enables or disables SRA feature for Upstream direction on the DSL line in ADSL.
		Initial default: Enabled (sra_us_a=0)
		 \note marked as W (write) within TR-181 */
	bool sra_us_a;
	/** Enables or disables SRA feature for Downstream direction on the DSL line in VDSL.
		Initial default: Disabled (sra_ds_v=1)
		 \note marked as W (write) within TR-181 */
	bool sra_ds_v;
	/** Enables or disables SRA feature for Downstream direction on the DSL line in ADSL.
		Initial default: Enabled (sra_ds_a=0)
		 \note marked as W (write) within TR-181 */
	bool sra_ds_a;
	/** Enables or disables the Virtual Noise feature for Upstream direction on the DSL line.
		 Initial default: Enabled (virtual_noise_us=1)
		 \note marked as W (write) within TR-181 */
	bool virtual_noise_us;
	/** Enables or disables the  Virtual Noise feature for Downstream direction on the DSL line.
		 Initial default: Enabled (virtual_noise_ds=1)
		 \note marked as W (write) within TR-181 */
	bool virtual_noise_ds;
	/** The current operational vectoring state of the DSL line
		 Enumeration of:
		 Off
		 On
		 Friendly
		 Auto           - (Initial default)
		 \note marked as W (write) within TR-181 */
	char vectoring[9];
	/** This configuration parameter defines the transmission system types to be allowed by the
		 xTU at initialization time. Please also refer to \ref dsl_fapi_line_obj.xtse for a
		 detailed definition this values.
		 \note marked as W (write) within TR-181 */
	unsigned char xtse[8];
	/** Comma-separated list of strings. List items indicate which link encapsulation standards are initially used.
		 Enumeration of:
		 G.992.3_Annex_K_ATM   - (Initial default)
		 G.992.3_Annex_K_PTM
		 G.993.2_Annex_K_ATM
		 G.993.2_Annex_K_PTM   - (Initial default)
 		 \note Default initialization value means tat ADSL uses ATM and VDSL uses PTM.
		 \note marked as W (write) within TR-181 */
	char link_encapsulation_config[80];
	/** The first/next mode that should be activated for initialization of the DSL line.
		 Enumeration of:
		 ApiDefault
		 ADSL
		 VDSL            - (Initial default)
 		 \note Reboot required on change!"
		 \note marked as W (write) within TR-181 */
	char dsl_next_mode[5];
	/** The next TC mode in which the device has to initialize.
		 Enumeration of:
		 PTM       - (Initial default)
		 ATM
		 \note marked as W (write) within TR-181 */
	char next_tc_mode[4];
	/** Selection of activation sequence for initialization of the DSL line.
		 Enumeration of:
		 Auto  (n/a)
		 StandardGHS       - (Initial default)
		 NonStandard
 		 \note Reboot required on change!"
		 \note marked as W (write) within TR-181 */
	char dsl_act_seq[12];
	/** Selection of activation start mode for initialization of the DSL line.
		 Enumeration of:
		 GHS       - (Initial default)
		 T1413
 		 \note Reboot required on change!"
		 \note marked as W (write) within TR-181 */
	char dsl_act_mode[8];
	/** Configures the “bRemember” flag for ADSL/VDSL multimode handling. If enabled, the parameters
		 dsl_next_mode and dsl_act_mode needs to be updated after showtime is reached.
		 Initial default: Enabled (dsl_remember=1)
 		 \note Reboot required on change!"
		 \note marked as W (write) within TR-181 */
	bool dsl_remember;
	/** The current enabled entities (lines)
		 Enumeration of:
		 None
		 Single link
		 Bonding, Auto (default)
		 \note Reboot required on increase of entities quantity */
	char dsl_entities_enabled[12];
};

/**
	DSL FAPI context
*/
struct fapi_dsl_ctx {
	/** device entity */
	int entity;
};

/**
	DSL FAPI global variables shared for all processes
*/
struct fapi_global_vars {
	/** CPE kernel drivers load counter */
	int drv_load_cnt;
	/** CPE control application load counter */
	int appl_start_cnt;
	/** CPE device count */
	int device_count;
	/** CPE entities enabled count */
	int dsl_entities_enabled;
	/** CPE reboot needed.
		 Prevent SL from opening lower layers in power save mode. */
	bool reboot_needed;
	/** CPE entites uninit operation status
		 Prevent from unnecessary update of dsl_web.cfg */
	bool uninit_entities_performed;
};

/**
	DSL FAPI open contexts repository
*/
struct fapi_shmem_data {
	/** Pointer to the DSL FAPI context */
	struct fapi_dsl_ctx *ctx_ptr;
	/** Copy of the DSL FAPI context */
	struct fapi_dsl_ctx ctx;
};

#define DSL_FAPI_SHMEM_CTX_CNT 10

/**
	DSL FAPI shared memory
*/
struct fapi_shmem {
	/** Global variables */
	struct fapi_global_vars global_vars;
	/** Counter of DSL FAPI contexts stored
		Max amount is DSL_FAPI_SHMEM_CTX_CNT */
	int open_cnt;
	/** DSL FAPI contexts stored */
	struct fapi_shmem_data data[DSL_FAPI_SHMEM_CTX_CNT];
};

/**
	The FAPI library context contains global information.
*/
enum fapi_dsl_status {
	/** Common error */
	FAPI_DSL_STATUS_ERROR = -1,
	/** Success */
	FAPI_DSL_STATUS_SUCCESS = 0,
	/** Warning group */
	FAPI_DSL_STATUS_WARNING = 1000
};

/**
	Structure that includes a single parameter.
	Possible name/values combination set:
	- name "X_LANTIQ_COM_BitswapUs_V"
	- values "0", "1"

	- name "X_LANTIQ_COM_BitswapUs_A"
	- values "0", "1"

	- name "X_LANTIQ_COM_BitswapDs_V"
	- values "0", "1"

	- name "X_LANTIQ_COM_BitswapDs_A"
	- values "0", "1"

	- name "X_LANTIQ_COM_SraUs_V"
	- values "0", "1"

	- name "X_LANTIQ_COM_SraUs_A"
	- values "0", "1"

	- name "X_LANTIQ_COM_SraDs_V"
	- values "0", "1"

	- name "X_LANTIQ_COM_SraDs_A"
	- values "0", "1"

	- name "X_LANTIQ_COM_ReTxUs"
	- values "0", "1"

	- name "X_LANTIQ_COM_ReTxDs_V"
	- values "0", "1"

	- name "X_LANTIQ_COM_ReTxDs_A"
	- values "0", "1"

	- name "X_LANTIQ_COM_VirtualNoiseUs"
	- values "0", "1"

	- name "X_LANTIQ_COM_VirtualNoiseDs"
	- values "0", "1"

	- name "X_LANTIQ_COM_Vectoring"
	- values "OFF", "ON", "Friendly"

	- name "X_LANTIQ_COM_XTSE"
	- values "0xXX 0xXX 0xXX 0xXX 0xXX 0xXX 0xXX 0xXX"

	- name "X_LANTIQ_COM_DSLNextMode"
	- values "ADSL", "VDSL"

	- name "X_LANTIQ_COM_NextTCMode"
	- values "ATM", "PTM"

	- name "X_LANTIQ_COM_LinkEncapsulationConfig"
	- values "G.992.3_Annex_K_ATM", "G.992.3_Annex_K_PTM",
		 "G.993.2_Annex_K_ATM", "G.993.2_Annex_K_PTM"

	- name "X_LANTIQ_COM_DSLActSeq"
	- values "Auto", "StandardGHS", "NonStandard"

	- name "X_LANTIQ_COM_DSLActMode"
	- values "GHS", "T1413"

	- name "X_LANTIQ_COM_DSLRemember"
	- values "0", "1"

	- name "X_LANTIQ_COM_EntitiesEnabled"
	- values "0", "1", "2"
	*/
struct fapi_dsl_init_parameter {
	char *name;
	char *value;
};

/**
	Structure that includes a group of single parameters.
*/
struct fapi_dsl_init_parameter_box {
	unsigned int array_size;
	struct fapi_dsl_init_parameter *array;
};

/**
	Structure that includes configuration parameter.
*/
struct fapi_dsl_init_cfg {
	struct fapi_dsl_init_parameter_box params;
};

/**
	This function opens DSL FAPI context.

	\param entity
		VRX device number.

	\return
		DSL FAPI context on successful,
		NULL if an error.
*/
struct fapi_dsl_ctx *fapi_dsl_open(unsigned int entity);

/**
	This function returns the requested DSL FAPI context.

	\param entity
		entity number of DSL context
	\return
		DSL FAPI context on successful,
		NULL if no open context is available.
*/
struct fapi_dsl_ctx *fapi_dsl_context_get(int entity);

/**
	This function releases the requested DSL FAPI context.

	\param ctx
		DSL Fapi context to be released

*/
void fapi_dsl_context_free(struct fapi_dsl_ctx *ctx);

/**
	This function closes DSL FAPI context.

	\param ctx
		pointer reference for internal FAPI context.

	\return
		DSL FAPI context on successful,
		NULL if an error.
*/
enum fapi_dsl_status fapi_dsl_close(struct fapi_dsl_ctx *ctx);

/**
	This function initializes the DSL Subsystem.

	\param ctx
		pointer reference for internal FAPI context. On initialization call, this
		pointer should be given as NULL; it returns the handle that needs to
		be stored within upper layer (SL) and is used for any successive FAPI
		function call.

	\param cfg
		pointer to the configuration parameters that needs to be applied.

	\return
		0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_init(struct fapi_dsl_ctx *ctx, const struct fapi_dsl_init_cfg *cfg);


/**
	This function updates dsl_web.cfg

	\param cfg
		pointer to the configuration parameters that needs to be applied.

	\param tc_mode
		pointer to TC mode value gathered from user

	\return
		None
*/
void fapi_dsl_update_web_config(const struct fapi_dsl_init_cfg *cfg, WAN_TYPE_t *tc_mode);

/**
	This function exits/shutdown the DSL Subsystem.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\return
		returns 0 on successful, negative if an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_uninit(struct fapi_dsl_ctx *ctx);

/**
	This function reads data from DSL Line object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL line object.

	\return
		returns 0 on successful, negative if an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning, refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_get(struct fapi_dsl_ctx *ctx, struct dsl_fapi_line_obj *obj);

/**
	This function writes data to DSL Line object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line object.

	\return
		returns 0 on successful, negative if an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning, please
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_set(struct fapi_dsl_ctx *ctx, const struct dsl_fapi_line_obj *obj);

/**
	This function reads data from DSL Line Stats object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line Stats object.

	\return
		returns 0 on successful, negative if an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning, please
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_stats_get(struct fapi_dsl_ctx *ctx, struct dsl_fapi_line_stats_obj *obj);

/**
	This function writes data to DSL Line Stats object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line Stats object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning, please
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_stats_set(struct fapi_dsl_ctx *ctx,
					     const struct dsl_fapi_line_stats_obj *obj);

/**
	This function reads data from DSL Line Stats Total object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line Stats Total object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning, please
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_stats_total_get(struct fapi_dsl_ctx *ctx,
						   struct dsl_fapi_line_stats_interval_obj *obj);

/**
	This function writes data to DSL Line Stats Total object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line Stats Total object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_stats_total_set(struct fapi_dsl_ctx *ctx,
						   const struct dsl_fapi_line_stats_interval_obj *obj);

/**
	This function reads data from DSL Line Stats Showtime object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line Stats Showtime object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_stats_showtime_get(struct fapi_dsl_ctx *ctx,
						      struct dsl_fapi_line_stats_interval_obj *obj);

/**
	This function writes data to DSL Line Stats Showtime object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line Stats Showtime object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_stats_showtime_set(struct fapi_dsl_ctx *ctx,
						      const struct dsl_fapi_line_stats_interval_obj *obj);

/**
	This function reads data from DSL Line Stats LastShowtime object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line Stats LastShowtime object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_stats_last_showtime_get(struct fapi_dsl_ctx *ctx,
							   struct dsl_fapi_line_stats_interval_obj *obj);

/**
	This function writes data to DSL Line Stats LastShowtime object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line Stats LastShowtime object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_stats_last_showtime_set(struct fapi_dsl_ctx *ctx,
							   const struct dsl_fapi_line_stats_interval_obj
							   *obj);

/**
	This function reads data from DSL Line Stats CurrentDay object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line Stats CurrentDay object.

	\return
		retruns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_stats_current_day_get(struct fapi_dsl_ctx *ctx,
							 struct dsl_fapi_line_stats_interval_obj *obj);

/**
	This function writes data to DSL Line Stats CurrentDay object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line Stats CurrentDay object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_stats_current_day_set(struct fapi_dsl_ctx *ctx,
							 const struct dsl_fapi_line_stats_interval_obj *obj);

/**
	This function reads data from DSL Line Stats QuarterHour object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line Stats QuarterHour object.

	\return
		retruns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_stats_quarter_hour_get(struct fapi_dsl_ctx *ctx,
							  struct dsl_fapi_line_stats_interval_obj *obj);

/**
	This function writes data to DSL Line Stats QuarterHour object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line Stats QuarterHour object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_stats_quarter_hour_set(struct fapi_dsl_ctx *ctx,
							  const struct dsl_fapi_line_stats_interval_obj *obj);

/**
	This function reads data from DSL Line TestParams object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line TestParams object.

	\return
		retrusn 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_test_params_get(struct fapi_dsl_ctx *ctx,
						   struct dsl_fapi_line_test_params_obj *obj);

/**
	This function writes data to DSL Line TestParams object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Line TestParams object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_line_test_params_set(struct fapi_dsl_ctx *ctx,
						   const struct dsl_fapi_line_test_params_obj *obj);

/**
	This function reads data from DSL Channel object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Channel object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_channel_get(struct fapi_dsl_ctx *ctx, struct dsl_fapi_channel_obj *obj);

/**
	This function writes data to DSL Channel object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Channel object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_channel_set(struct fapi_dsl_ctx *ctx, const struct dsl_fapi_channel_obj *obj);

/**
	This function reads data from DSL Channel Stats object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Channel Stats object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_channel_stats_get(struct fapi_dsl_ctx *ctx,
						struct dsl_fapi_channel_stats_obj *obj);

/**
	This function writes data to DSL Channel Stats object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Channel Stats object.

	\return
		retruns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_channel_stats_set(struct fapi_dsl_ctx *ctx,
						const struct dsl_fapi_channel_stats_obj *obj);

/**
	This function reads data from DSL Channel Stats Total object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Channel Stats Total object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_channel_stats_total_get(struct fapi_dsl_ctx *ctx,
						      struct dsl_fapi_channel_stats_interval_obj *obj);

/**
	This function writes data to DSL Channel Stats Total object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Channel Stats Total object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_channel_stats_total_set(struct fapi_dsl_ctx *ctx,
						      const struct dsl_fapi_channel_stats_interval_obj *obj);

/**
	This function reads data from DSL Channel Stats Showtime object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Channel Stats Showtime object.

	\return
		retruns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_channel_stats_showtime_get(struct fapi_dsl_ctx *ctx,
							 struct dsl_fapi_channel_stats_interval_obj *obj);

/**
	This function writes data to DSL Channel Stats Showtime object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Channel Stats Showtime object.

	\return
		retruns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_channel_stats_showtime_set(struct fapi_dsl_ctx *ctx,
							 const struct dsl_fapi_channel_stats_interval_obj
							 *obj);

/**
	This function reads data from DSL Channel Stats LastShowtime object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Channel Stats LastShowtime object.

	\return
		retruns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer to error codes which are defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_channel_stats_last_showtime_get(struct fapi_dsl_ctx *ctx,
							      struct dsl_fapi_channel_stats_interval_obj
							      *obj);

/**
	This function writes data to DSL Channel Stats LastShowtime object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Channel Stats LastShowtime object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_channel_stats_last_showtime_set(struct fapi_dsl_ctx *ctx,
							      const struct dsl_fapi_channel_stats_interval_obj
							      *obj);

/**
	This function reads data from DSL Channel Stats CurrentDay object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Channel Stats CurrentDay object.

	\return
		retruns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_channel_stats_current_day_get(struct fapi_dsl_ctx *ctx,
							    struct dsl_fapi_channel_stats_interval_obj *obj);

/**
	This function writes data to DSL Channel Stats CurrentDay object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Channel Stats CurrentDay object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_channel_stats_current_day_set(struct fapi_dsl_ctx *ctx,
							    const struct dsl_fapi_channel_stats_interval_obj
							    *obj);

/**
	This function reads data from DSL Channel Stats QuarterHour object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL Channel Stats QuarterHour object.

	\return
		retruns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_channel_stats_quarter_hour_get(struct fapi_dsl_ctx *ctx,
							     struct dsl_fapi_channel_stats_interval_obj *obj);

/**
	This function writes data to DSL Channel Stats QuarterHour object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		Channel Stats QuarterHour object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_channel_stats_quarter_hour_set(struct fapi_dsl_ctx *ctx,
							     const struct dsl_fapi_channel_stats_interval_obj
							     *obj);

/**
	This function reads data to DSL BondingGroup object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		BondingGroup object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_bonding_get(struct fapi_dsl_ctx *ctx, struct dsl_fapi_bonding_obj *obj);

/**
	This function writes data to DSL BondingGroup object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		BondingGroup object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_bonding_set(struct fapi_dsl_ctx *ctx, struct dsl_fapi_bonding_obj *obj);

/**
	This function reads data to DSL BondingGroup.{i}.Channel object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		BondingGroup.{i}.Channel object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_bonding_channel_get(struct fapi_dsl_ctx *ctx,
						  struct dsl_fapi_bonding_channel_obj *obj);

/**
	This function writes data to DSL BondingGroup.{i}.Channel object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		BondingGroup.{i}.Channel object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_bonding_channel_set(struct fapi_dsl_ctx *ctx,
						  struct dsl_fapi_bonding_channel_obj *obj);

/**
	This function reads data to DSL BondingGroup.{i}.Stats object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		BondingGroup.{i}.Stats object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_bonding_stats_get(struct fapi_dsl_ctx *ctx,
						struct dsl_fapi_bonding_stats_obj *obj);

/**
	This function writes data to DSL BondingGroup.{i}.Stats object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		BondingGroup.{i}.Stats object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_bonding_stats_set(struct fapi_dsl_ctx *ctx,
						struct dsl_fapi_bonding_stats_obj *obj);

/**
	This function reads data to DSL BondingGroup.{i}.Stats object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		BondingGroup.{i}.Stats object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_bonding_stats_day_get(struct fapi_dsl_ctx *ctx,
						    struct dsl_fapi_bonding_stats_day_obj *obj);

/**
	This function reads data to DSL BondingGroup.{i}.Stats object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		BondingGroup.{i}.Stats object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_bonding_stats_hour_get(struct fapi_dsl_ctx *ctx,
						     struct dsl_fapi_bonding_stats_hour_obj *obj);

/**
	This function reads data to DSL BondingGroup.{i}.Stats object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		BondingGroup.{i}.Stats object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_bonding_stats_total_get(struct fapi_dsl_ctx *ctx,
						      struct dsl_fapi_bonding_stats_total_obj *obj);

/**
	This function reads data to DSL BondingGroup.{i}.Stats object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		BondingGroup.{i}.Stats object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_bonding_ethernet_stats_get(struct fapi_dsl_ctx *ctx,
							 struct dsl_fapi_bonding_ethernet_stats_obj *obj);

/**
	This function reads data from DSL X_LANTIQ_COM_Config object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL X_LANTIQ_COM_Config object.

	\return
		retruns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_x_lantiq_com_config_get(struct fapi_dsl_ctx *ctx,
						      struct dsl_fapi_x_lantiq_com_config_obj *obj);

/**
	This function writes data to DSL X_LANTIQ_COM_Config object.

	\param ctx
		FAPI handle (context) that was initialized during and returned to calling
		instance within context of \ref fapi_dsl_init function call.

	\param obj
		DSL X_LANTIQ_COM_Config object.

	\return
		returns 0 on successful, negative on an error (where -1 is a common error)
		and positive if a warning. In case of an error or warning,
		refer the error codes defined within DSL CPE API by
		DSL_Error_t type.
*/
enum fapi_dsl_status fapi_dsl_x_lantiq_com_config_set(struct fapi_dsl_ctx *ctx,
						      const struct dsl_fapi_x_lantiq_com_config_obj *obj);

/**
	This function reads status of uninit entites operation.

	\param
		None

	\return
		returns status of uninit entites operation
*/
bool fapi_dsl_uninit_entities_status_get(void);

/**
	This function writes status of uninit entites operation.

	\param new_status
		bool value

	\return
		None
*/
void fapi_dsl_uninit_entities_status_set(const bool new_status);

/**
	This function returns number of available devices.

	\param
		None

	\return
		Available devices count
*/
int fapi_dsl_get_device_count(void);

/**
	This function returns number of shutdowned devices.

	\param entity
		MEI entity number. By default 0.

	\return
		Shutdowned devices count
*/
int fapi_dsl_get_entity_shutdown_count(unsigned int entity);

/**
	This function sets FAPI's log level and log type..

	\param log_level
		FAPI log level

	\param log_type
		FAPI log type

	\return
		None
*/
void fapi_dsl_log_set(int16_t log_level, int16_t log_type);

/* @} */
__END_DECLS
#endif
