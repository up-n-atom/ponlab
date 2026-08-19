#ifndef _LIB_TAPI_LT_GR909_H
#define _LIB_TAPI_LT_GR909_H
/****************************************************************************

                           Copyright (c) 2012 - 2016
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/**
   \file lib_tapi_lt_gr909.h  Linetesting library
   This file can be used by the application as an interface on top of the
   driver to get GR.909 results with floating point precision.

   This file declares the following user interface functions for:
   - Calibration of the system at open loop conditions:
      - IFX_TAPI_NLT_OLCalibration
      - IFX_TAPI_NLT_OLConfig_Set

   - GR.909 line testing:
      - Ifxphone_LT_GR909_Config
      - Ifxphone_LT_GR909_Start
      - Ifxphone_LT_GR909_Stop
      - Ifxphone_LT_GR909_GetResults

   - Line capacitance measurements:
      - IFX_TAPI_NLT_Capacitance_Start
      - IFX_TAPI_NLT_Capacitance_Stop
      - IFX_TAPI_NLT_Capacitance_GetResults

   - AC level metering:
      - IFX_TAPI_NLT_AclmStart
      - IFX_TAPI_NLT_AclmResultGet

   - Other:
      - IFX_TAPI_NLT_VersionGet
      - IFX_TAPI_NLT_ErrorStrGet
*/

/** \defgroup TAPI_INTERFACE_LT_GR909 Line Testing Interfaces
   This section describes line testing interfaces.*/

/** \addtogroup TAPI_INTERFACE_LT_GR909 */
/*@{*/

/* ============================= */
/* Global Defines & enums        */
/* ============================= */

/** GR-909 tests masks.*/
typedef enum
{
   /** Mask to select HPT. */
   IFX_LT_GR909_HPT_MASK  = (1 << 0),
   /** Mask to select FEMF. */
   IFX_LT_GR909_FEMF_MASK = (1 << 1),
   /** Mask to select RFT. */
   IFX_LT_GR909_RFT_MASK  = (1 << 2),
   /** Mask to select ROH. */
   IFX_LT_GR909_ROH_MASK  = (1 << 3),
   /** Mask to select RIT. */
   IFX_LT_GR909_RIT_MASK  = (1 << 4)
} IFX_LT_GR909_MASK_t;

/* ============================= */
/* Global Structures             */
/* ============================= */

/** Version number. */
typedef struct
{
   /** Library version, major number. */
   IFX_uint8_t  major;
   /** Library version, minor number. */
   IFX_uint8_t  minor;
   /** Library version, build number. */
   IFX_uint8_t  step;
   /** Library version, package type. */
   IFX_uint8_t  type;
} IFX_NLT_VERSION_t;

/** Results of the NLT capacitance measurement.*/
typedef struct
{
   /** Validity of measurement result tip to ring. */
   IFX_boolean_t  b_tip2ring_valid;
   /** Measured capacitance tip to ring [nF]. */
   IFX_float_t    f_tip2ring_cap;
   /** Validity of measurement results tip to ground and ring to ground. */
   IFX_boolean_t  b_line2gnd_valid;
   /** Measured capacitance tip to ground [nF]. */
   IFX_float_t    f_tip2gnd_cap;
   /** Measured capacitance ring to ground [nF]. */
   IFX_float_t    f_ring2gnd_cap;
} IFX_NLT_Capacitance_Result_t;

/** Hazardous potential test results. */
typedef struct
{
   /** HPT result, passed or failed. */
   IFX_boolean_t b_result;
   /** HPT AC ring wire to gnd value, [Vrms]. */
   IFX_float_t   f_hpt_ac_r2g;
   /** HPT AC TIP wire to GND value, [Vrms]. */
   IFX_float_t   f_hpt_ac_t2g;
   /** HPT AC tip wire to ring wire value, [Vrms]. */
   IFX_float_t   f_hpt_ac_t2r;
   /** HPT DC ring wire to gnd value, [V]. */
   IFX_float_t   f_hpt_dc_r2g;
   /** HPT DC tip wire to gnd value, [V]. */
   IFX_float_t   f_hpt_dc_t2g;
   /** HPT DC tip wire to ring wire value, [V].*/
   IFX_float_t   f_hpt_dc_t2r;
} IFX_LT_GR909_HPT_t;

/** Foreign electromotive forces test results. */
typedef struct
{
   /** FEMF result, passed or failed. */
   IFX_boolean_t b_result;
   /** FEMF ac ring wire to gnd value, [Vrms]. */
   IFX_float_t   f_femf_ac_r2g;
   /** fFEMF AC tip wire to gnd value, [Vrms]. */
   IFX_float_t   f_femf_ac_t2g;
   /** FEMF AC tip wire to ring wire value, [Vrms]. */
   IFX_float_t   f_femf_ac_t2r;
   /** FEMF DC ring wire to gnd value, [V]. */
   IFX_float_t   f_femf_dc_r2g;
   /** FEMF DC tip wire to gnd value, [V]. */
   IFX_float_t   f_femf_dc_t2g;
   /** FEMF DC tip wire to ring wire value, [V]. */
   IFX_float_t   f_femf_dc_t2r;
} IFX_LT_GR909_FEMF_t;

/** Resistive faults test results. */
typedef struct
{
   /** RFT result, passed or failed. */
   IFX_boolean_t b_result;
   /** RFT ring wire to gnd value, [Ohm]. */
   IFX_float_t   f_rft_r2g;
   /** RFT tip wire to gnd value, [Ohm]. */
   IFX_float_t   f_rft_t2g;
   /** RFT tip wire to ring wire value, [Ohm]. */
   IFX_float_t   f_rft_t2r;
} IFX_LT_GR909_RFT_t;

/** Receiver off-hook test results. */
typedef struct
{
   /** ROH result, passed or failed. */
   IFX_boolean_t b_result;
   /** ROH tip wire to ring wire value for low voltage, [Ohm]. */
   IFX_float_t   f_roh_t2r_l;
   /** ROH tip wire to ring wire value for high voltage, [Ohm]. */
   IFX_float_t   f_roh_t2r_h;
} IFX_LT_GR909_ROH_t;

/** Ringer impedance test results. */
typedef struct
{
   /** RIT result, passed or failed. */
   IFX_boolean_t b_result;
   /** RIT value, , [Ohm]. */
   IFX_float_t   f_rit_res;
} IFX_LT_GR909_RIT_t;

/** GR-909 results structure. */
typedef struct
{
   /** Valid results mask, set with \ref IFX_LT_GR909_MASK_t. */
   IFX_uint32_t         valid_mask;
   /** Hazardous potential test results. */
   IFX_LT_GR909_HPT_t   hpt;
   /** Foreign electromotive forces test results. */
   IFX_LT_GR909_FEMF_t  femf;
   /** Resistive faults test results. */
   IFX_LT_GR909_RFT_t   rft;
   /** Receiver off-hook test results. */
   IFX_LT_GR909_ROH_t   roh;
   /** Ringer impedance test results. */
   IFX_LT_GR909_RIT_t   rit;
} IFX_LT_GR909_RESULT_t;

/** GR-909 parameter configuration structure.
The values f_R1, f_R2 and f_R3 are chip-set dependent,
see also the product hardware design guidelines for reference.
R1 corresponds to the measurement resistor for voltage measurement RM.
The default value for DUSLIC-xT V1.4 is 1.5 MOhm. For all other devices
the default value is 1 MOhm. In future it will be allowed to set other
values than the default values for some devices only. Please refer to
the corresponding system release notes.
The values for R2 and R3 are used for backward compatibilty reasons
and shall not be changed. */
typedef struct
{
   /** High-ohmic resistor of the voltage divider connected to
    the line. The suggested value for DUSLIC-XTv1.3 is f_R1=Rm=1000000,
    for DUSLIC-XTv1.4 is f_R1=Rm=1500000.
    For SLIC-DC based products (e.g. VINAX-VE),
    the recommended value is f_R1=R1=1500000. */
   IFX_float_t f_R1;
   /** For DUSLIC-XT: f_R2=f_R3=2065.
    For SLIC-DC based products (e.g. VINAX-VE): low-ohmic resistor of the voltage
    divider in parallel to 1 MOhm (internal resistor).
    For example, if R2=3.3 kOhm, f_R2 = 3.3k || 1M = 3289.
    \remarks Please use the default value (f_R2 = 2065) and do not change it. */
   IFX_float_t f_R2;
   /** For DUSLIC-XT: f_R2=f_R3=2065.
    For SLIC-DC based products, (e.g. VINAX-VE): low-ohmic resistor of the voltage
    divider in parallel to 750 kOhm (internal resistor).
    For example, if R2=3.3 kOhm, f_R3 = 3.3k || 750k = 3286.
    \remarks Please use the default value (f_R2 = 2065) and do not change it. */
   IFX_float_t f_R3;
} IFX_LT_GR909_CFG_t;

#ifdef TAPI_ONE_DEVNODE
/** Device and channel number for which measurement ahould be done. */
typedef struct IFX_LT_DEV_CH_t_
{
   /** Device number (in). */
   IFX_uint16_t dev;
   /** Channel number (in). */
   IFX_uint16_t ch;
} IFX_LT_DevCh_t;
#endif /* TAPI_ONE_DEVNODE */

/** Type of AC level measurement. */
typedef enum
{
   /** Frequency Response measurement */
   DXS_ACLM_FR = 1,
   /** Transhybrid measurement */
   DXS_ACLM_TH,
   /** Gain Tracking measurement */
   DXS_ACLM_GT,
   /** Signal to Noise Ratio measurement */
   DXS_ACLM_SNR
} IFX_LT_ACLM_Measurement_t;

/** AC level metering results structure. */
typedef struct
{
  /** Number of valid measurement values in each of the two arrays. */
  IFX_uint8_t mp_count;
  /** Array of input values used for the AC level metering  measurement.
      The meaning of the value depends on the selected measurement:
      - frequency for "Frequency Response measurement", [Hz]
      - frequency for "Transhybrid measurement", [Hz]
      - level for "Gain Tracking measurement", [dBm0]
      - level for "Signal to Noise Ratio measurement", [dBm0] */
  IFX_int32_t mp_arg[IFX_TAPI_ACLM_MAX_MP_RESULTS];
  /** Array with result values from AC level metering measurement.
      The result values correspond to the input values of the same index.
      The meaning of the value depends on the selected measurement:
      - level for "Frequency Response measurement", [dB]
      - level for for "Transhybrid measurement", [dB]
      - gain variation for "Gain Tracking measurement", [dB]
      - signal noise ratio for "Signal to Noise Ratio measurement", [dB] */
  IFX_float_t mp_val[IFX_TAPI_ACLM_MAX_MP_RESULTS];
} IFX_LT_ACLM_Result_t;

/* ============================= */
/* Global function declaration   */
/* ============================= */

/**
   Get Line Testing library version.

\param  pVersion      Pointer to a struct of
                        \ref IFX_NLT_VERSION_t

\return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_VersionGet(IFX_NLT_VERSION_t *pVersion);

/**
   Translates error number to string.

\param  nErrNo      error code returned by NLT library function

\return Returns string with description of error.
*/
IFX_char_t *IFX_TAPI_NLT_ErrorStrGet(IFX_int32_t nErrNo);

/**
   Starts measurement of capacitance values.

\param  fd           Line file descriptor.
\param  dev_ch       Used device and channel numbers,
                     available only if TAPI V4 API is used.

\return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_Capacitance_Start (IFX_int32_t fd
#ifdef TAPI_ONE_DEVNODE
                                            , IFX_LT_DevCh_t dev_ch
#endif /* TAPI_ONE_DEVNODE */
                                            );

/**
   Stops measurement of capacitance values immediately.

\param  fd           Line file descriptor.
\param  dev_ch       Used device and channel numbers,
                     available only if TAPI V4 API is used.

\return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_Capacitance_Stop (IFX_int32_t fd
#ifdef TAPI_ONE_DEVNODE
                                           , IFX_LT_DevCh_t dev_ch
#endif /* TAPI_ONE_DEVNODE */
                                           );

/**
   Get measurement results of the capacitance measurement.

\param  fd           Line file descriptor.
\param  dev_ch       Used device and channel numbers,
                     available only if TAPI V4 API is used.
\param  pResult      Pointer to a struct of \ref IFX_NLT_Capacitance_Result_t
                        to be filled with the results.

\return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/

IFX_int32_t IFX_TAPI_NLT_Capacitance_GetResults (
                                         IFX_int32_t fd,
#ifdef TAPI_ONE_DEVNODE
                                         IFX_LT_DevCh_t dev_ch,
#endif /* TAPI_ONE_DEVNODE */
                                         IFX_NLT_Capacitance_Result_t *pResult);

/**
   Configures the open loop calibration factors of the measurement path
   for line testing.

\param  fd           Line file descriptor.
\param  pConfig      Pointer to a struct of
                        \ref IFX_TAPI_NLT_CONFIGURATION_OL_t

\return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_OLConfig_Set (
                                    IFX_int32_t fd,
                                    IFX_TAPI_NLT_CONFIGURATION_OL_t *pConfig);

/**
   Return configuration of the open loop calibration factors of the measurement
   path for line testing.

   \param  fd           Line file descriptor.
   \param  pConfig      Pointer to a struct of
                        \ref IFX_TAPI_NLT_CONFIGURATION_OL_t

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_OLConfig_Get (
                                    IFX_int32_t fd,
                                    IFX_TAPI_NLT_CONFIGURATION_OL_t *pConfig);

/**
   This function does open-loop calibration,
   which is needed for capacitance and GR-909 measurements.

\param  fd           Line file descriptor.
\param  nNrLoops     Number of loops for calibration; 1 is sufficient.
\param  b_euLike     IFX_TRUE  : EU-like power line frequency (50 Hz)
                     IFX_FALSE : US-like power line frequency (60 Hz)
\param  pConfig      Pointer to a struct of
                        \ref IFX_TAPI_NLT_CONFIGURATION_OL_t. Calculated
                        calibration factors are stored to this structure.

\return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
\remark
   This function sets calibration factors in the driver, so that the user does not
   need to do this. Open-loop calibration should be executed only once, when
   the phone is NOT connected to the FXS port. The user should store the calibration factors
   in non-volatile memory and execute \ref IFX_TAPI_NLT_OLConfig_Set
   during the following system start. This saves time during startup and still gives
   the most accurate measurement results.
   This function is blocking for the whole calibration process, where each
   loop of calibration takes approximately 4 s. Therefore, the whole measurement
   takes about 4 s multiplied by the number of loops for calibration.
*/
IFX_int32_t IFX_TAPI_NLT_OLCalibration (
                                      IFX_int32_t fd,
                                      IFX_uint32_t nNrLoops,
                                      IFX_boolean_t b_euLike,
                                      IFX_TAPI_NLT_CONFIGURATION_OL_t *pConfig);

/** Configures SLIC system parameters to be used for GR-909 calculation.
  This function shall be used only if the voltage divider resistors
  connected to the SLIC do not follow the Lantiq hardware design
  guidelines document.
  This interface is for future use, the existing GR-909 supported devices
  have to deal with the default configuration.

\param dev - device type of \ref IFX_TAPI_GR909_DEV_t.
\param p_cfg Handles to \ref IFX_LT_GR909_CFG_t structure.

\return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error

\remark Use of this function is optional; default parameters are used
   otherwise.
*/
IFX_int32_t Ifxphone_LT_GR909_Config (IFX_TAPI_GR909_DEV_t dev,
                                      IFX_LT_GR909_CFG_t *p_cfg);

/** Starts a GR909 test sequence according to measurement mask set with
 \ref IFX_LT_GR909_MASK_t. All tests in the mask must be selected.

\param fd_line   Line file descriptor.
\param  dev_ch   Used device and channel numbers,
                 available only if TAPI V4 API is used.
\param b_euLike  IFX_TRUE: EU-like power line frequency (50 Hz).
                 IFX_FALSE: US-like power line frequency (60 Hz).
\param meas_mask Measurement mask set with values out of \ref IFX_LT_GR909_MASK_t.

\return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t Ifxphone_LT_GR909_Start (IFX_int32_t   fd_line,
#ifdef TAPI_ONE_DEVNODE
                                     IFX_LT_DevCh_t dev_ch,
#endif /* TAPI_ONE_DEVNODE */
                                     IFX_boolean_t b_euLike,
                                     IFX_uint32_t  meas_mask);


/**
   Stop a GR909 test or test sequence

\param  fd_line      Line file descriptor.
\param  dev_ch       Used device and channel numbers,
                     available only if TAPI V4 API is used.

\return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t Ifxphone_LT_GR909_Stop (IFX_int32_t fd_line
#ifdef TAPI_ONE_DEVNODE
                                    , IFX_LT_DevCh_t dev_ch
#endif /* TAPI_ONE_DEVNODE */
                                    );

/** Receives Gr909 measurement results.

\param fd_line Line file descriptor.
\param dev_ch  Used device and channel numbers.
\param p_res   Handles to result structure.

\return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error

\remark  Only evaluates results which are marked as valid.
*/
IFX_int32_t Ifxphone_LT_GR909_GetResults (IFX_int32_t  fd_line,
#ifdef TAPI_ONE_DEVNODE
                                          IFX_LT_DevCh_t dev_ch,
#endif /* TAPI_ONE_DEVNODE */
                                          IFX_LT_GR909_RESULT_t *p_res);


/**
   Starts AC level meter measurement.

   \param  fd           Line file descriptor.
   \param  dev_ch       Used device and channel numbers,
                        available only if TAPI V4 API is used.
   \param  nType        Type of measurement.

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_AclmStart (
                        IFX_int32_t fd,
#ifdef TAPI_ONE_DEVNODE
                        IFX_LT_DevCh_t dev_ch,
#endif /* TAPI_ONE_DEVNODE */
                        IFX_LT_ACLM_Measurement_t nType);


/**
   Retrieves and calculate AC level meter measurement results.

   \param  fd           Line file descriptor.
   \param  dev_ch       Used device and channel numbers,
                        available only if TAPI V4 API is used.
   \param  nType        Type of measurement.
   \param  pResults     Pointer to results stucture.

   \return Returns value as follows:
   - \ref IFX_SUCCESS: if successful
   - errorcode in case of an error
*/
IFX_int32_t IFX_TAPI_NLT_AclmResultGet (
                        IFX_int32_t fd,
#ifdef TAPI_ONE_DEVNODE
                        IFX_LT_DevCh_t dev_ch,
#endif /* TAPI_ONE_DEVNODE */
                        IFX_LT_ACLM_Measurement_t nType,
                        IFX_LT_ACLM_Result_t *pResults);

/*@}*/ /* TAPI_INTERFACE_LT_GR909 */
#endif /* _LIB_TAPI_LT_GR909_H */
