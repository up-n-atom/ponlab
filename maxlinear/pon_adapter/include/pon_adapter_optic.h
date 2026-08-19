/******************************************************************************
 *
 *  Copyright (c) 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI header file for accessing optical interface
 */

#ifndef _pon_adapter_optic
#define _pon_adapter_optic

#include "pon_adapter_base.h"
#include "pon_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_OPTIC Optical interface access
 *
 *  These functions and structures are used for accessing optical interface
 *
 *  @{
 */

/** DDMI memory page selection. */
enum pa_ddmi_page {
	/* Select the 0xA0/0x50 page */
	PA_DDMI_A0 = 0,
	/* Select the 0xA2/0x51 page */
	PA_DDMI_A2 = 1,
	/** Number of DDMI memory pages */
	PA_DDMI_MAX = 2,
};

/** Optical interface static properties information. */
struct pa_optic_properties {
	/** Vendor name.
	 *  ASCII character string.
	 */
	char vendor_name[17];
	/** Vendor OUI.
	 *  The OUI is represented by three binary values.
	 */
	uint8_t vendor_oui[4];
	/** Part number.
	 *  ASCII character string.
	 */
	char part_number[17];
	/** Revision number.
	 *  ASCII character string.
	 */
	char revision[5];
	/** Serial number.
	 *  ASCII character string.
	 */
	char serial_number[17];
	/** Date code.
	 *  ASCII character string.
	 */
	char date_code[9];
	/**
	 * Type of transceiver.
	 *
	 * 00h Unknown or unspecified
	 * 01h GBIC
	 * 02h Module soldered to motherboard (ex: SFF)
	 * 03h SFP or SFP+
	 */
	uint8_t identifier;
	/**
	 * Code for connector type
	 * (see SFF-8024 SFF Module Management Reference Code Tables)
	 */
	uint8_t connector;
	/**
	 * Nominal signaling rate, units of 100 MBd.
	 * (see details for rates > 25.4 GBd)
	 */
	uint32_t signaling_rate;
	/** Transmitter wavelength at room temperature [nm]. */
	uint32_t tx_wavelength;
	/** High Power Level Declaration.
	 *  - 0: LVL12, Identifies standard Power Levels 1 and 2 as indicated by
	 *       power level declaration.
	 *  - 1: LVL3, Identifies Power Level 3 requirement.
	 */
	uint32_t high_power_lvl_decl;
	/** Paging implemented indicator.
	 *  - 0: NO_PGN, Indicates that paging is not implemented.
	 *  - 1: PGN, Indicates that paging is implemented and byte 127 of
	 *       device address 0xA2/0x51 is used for page selection.
	 */
	uint32_t paging_implemented_ind;
	/** Retimer or CDR indicator.
	 *  - 0: NO_RET, Indicates that the transceiver does not provide an
	 *       internal retimer or Clock and Data Recovery (CDR) circuit.
	 *  - 1: RET, Indicates that the transceiver has an internal
	 *       retimer or Clock and Data Recovery (CDR) circuit.
	 */
	uint32_t retimer_ind;
	/** Cooled Transceiver Declaration.
	 *  - 0: UNCOOL, Identifies a conventional uncooled (or unspecified)
	 *       laser implementation.
	 *  - 1: COOL, Identifies a cooled laser transmitter implementation.
	 */
	uint32_t cooled_transceiver_decl;
	/** Power Level Declaration.
	 *  - 0: PL1, Identifies Power Level 1 (or unspecified) requirements.
	 *  - 1: PL2, Identifies Power Level 2 requirement.
	 */
	uint32_t power_lvl_decl;
	/** Linear Receiver Output Implemented.
	 *  - 0: LIM, Identifies a conventional limiting (or unspecified)
	 *       receiver output.
	 *  - 1: LIN, Identifies a linear receiver output.
	 */
	uint32_t linear_rx_output_impl;
	/** Receiver decision threshold implemented.
	 *  - 0: NO_RDT, Indicates that RDT is not implemented.
	 *  - 1: RDT, Indicates that RDT is implemented.
	 */
	uint32_t rx_decision_thr_impl;
	/** Tunable transmitter technology.
	 *  - 0: FIX, Indicates that the transmitter wavelength/frequency is not
	 *       tunable.
	 *  - 1: TUNE, Indicates that the transmitter wavelength/frequency is
	 *       tunable.
	 */
	uint32_t tunable_transmitter;
	/** RATE_SELECT functionality implemented.
	 *  - 0: NO, Not implemented.
	 *  - 1: YES, Implemented.
	 */
	uint32_t rate_select;
	/** TX_DISABLE implemented and disables the high speed serial output.
	 *  - 0: NO, Not implemented.
	 *  - 1: YES, Implemented.
	 */
	uint32_t tx_disable;
	/** TX_FAULT implemented.
	 *  - 0: NO, Not implemented.
	 *  - 1: YES, Implemented.
	 */
	uint32_t tx_fault;
	/** Loss of Signal implemented, signal inverted from standard definition
	 *  in SFP MSA (often called "Signal Detect").
	 *  - 0: NO, Not implemented.
	 *  - 1: YES, Implemented.
	 */
	uint32_t signal_detect;
	/** Loss of Signal implemented, signal as defined in SFP MSA (often
	 *  called "Rx_LOS").
	 *  - 0: NO, Not implemented.
	 *  - 1: YES, Implemented.
	 */
	uint32_t rx_los;
	/** Digital monitoring - must be '1' for comply with SFF-8472,
	 *  "Management Interface for SFP+" document.
	 */
	uint32_t digital_monitoring;
	/** Internally calibrated.
	 *  - 0: NO, Not internally calibrated
	 *  - 1: YES, Internally calibrated
	 */
	uint32_t int_calibrated;
	/** Externally calibrated.
	 *  - 0: NO, Not externally calibrated
	 *  - 1: YES, Externally calibrated
	 */
	uint32_t ext_calibrated;
	/** Received power measurement type.
	 *  - 0: OMA, OMA-based receive power measurement.
	 *  - 1: AVG, Average-based receive power measurement.
	 */
	uint32_t rx_power_measurement_type;
	/** Address change required.
	 *  - 0: NO, Not required.
	 *  - 1: YES, Required.
	 */
	uint32_t address_change_req;
	/** Optional Alarm/warning flags implemented for all monitored
	 *  quantities.
	 *  - 0: NO, Not implemented.
	 *  - 1: YES, Implemented.
	 */
	uint32_t optional_flags_impl;
	/** Optional soft TX_DISABLE control and monitoring implemented.
	 *  - 0: NO, Not implemented.
	 *  - 1: YES, Implemented.
	 */
	uint32_t soft_tx_disable_monitor;
	/** Optional soft TX_FAULT monitoring implemented.
	 *  - 0: NO, Not implemented.
	 *  - 1: YES, Implemented.
	 */
	uint32_t soft_tx_fault_monitor;
	/** Optional soft RX_LOS monitoring implemented.
	 *  - 0: NO, Not implemented.
	 *  - 1: YES, Implemented.
	 */
	uint32_t soft_rx_los_monitor;
	/** Optional soft RATE_SELECT control and monitoring implemented.
	 *  - 0: NO, Not implemented.
	 *  - 1: YES, Implemented.
	 */
	uint32_t soft_rate_select_monitor;
	/** Optional Application Select control implemented
	 *  - 0: NO, Not implemented.
	 *  - 1: YES, Implemented.
	 */
	uint32_t app_select_impl;
	/** Optional soft Rate Select control implemented
	 *  - 0: NO, Not implemented.
	 *  - 1: YES, Implemented.
	 */
	uint32_t soft_rate_select_ctrl_impl;
	/** SFF-8472 compliance.
	 *  - 0: NONE, Not included or undefined.
	 *  - 1: V09_3, SFF-8472 Rev 9.3
	 *  - 2: V09_5, SFF-8472 Rev 9.5
	 *  - 3: V10_2, SFF-8472 Rev 10.2
	 *  - 4: V10_4, SFF-8472 Rev 10.4
	 *  - 5: V11_0, SFF-8472 Rev 11.0
	 *  - 6: V11_3, SFF-8472 Rev 11.3
	 *  - 7: V11_4, SFF-8472 Rev 11.4
	 *  - 8: V12_3, SFF-8472 Rev 12.3
	 *  - 9: V12_4, SFF-8472 Rev 12.4
	 *  - Others: Reserved.
	 */
	uint32_t compliance;
	/** PON Mode this transceiver is using.
	 * This value is read from our internal database and unknown if not
	 * detected correctly.
	 */
	enum pa_pon_op_mode pon_mode;
};

/** PON optical interface status structure. */
struct pa_optic_status {
	/** Optical transceiver temperature.
	 *  The signed value is given in a 32-bit signed 2s complement format,
	 *  in increments of 1/256 degree centigrade.
	 *  The output number range is from -32767 to 32767.
	 *  The physical temperature range is from -128 deg C to +128 deg C.
	 */
	int32_t temperature;
	/** Transceiver supply voltage.
	 *  The value is given as an unsigned 32-bit integer with an LSB
	 *  representing a step of 100 uV.
	 *  The valid number range is from 0 to 65535,
	 *  representing physical values from 0 V to 6.5535 V.
	 */
	uint32_t voltage;
	/** Transmit bias current.
	 *  The value is given as an unsigned 32-bit integer with an LSB
	 *  representing a step of 2 uA.
	 *  The valid number range is from 0 to 65535,
	 *  representing physical values from 0 mA to 131 mA.
	 */
	uint32_t bias;
	/** Transmit power.
	 *  The signed value is given in a 2s complement integer format,
	 *  in dBm (referred to 1 mW). The granularity is 0.002 dB.
	 *  The valid value number range is from -20000 to 4082.
	 *  To get dBm divide this value by 500. This will
	 *  represent physical values from -40 dBm (0.1 uW) to
	 *  +8.164 dBm (6.5535 mW).
	 *  A transmit power of 0 mW will be reported as -32767.
	 */
	int32_t tx_power;
	/** Receive power.
	 *  The signed value is given in a 2s complement integer format,
	 *  in dBm (referred to 1 mW). The granularity is 0.002 dB.
	 *  The valid value number range is from -20000 to 4082.
	 *  To get dBm divide this value by 500.
	 *  This will represent physical values from -40 dBm (0.1 uW) to
	 *  +8.164 dBm (6.5535 mW).
	 *  A receive power of 0 mW will be reported as -32767.
	 */
	int32_t rx_power;
	/** Receiver loss of signal status.
	 *  - 0: OK, The received signal level is above the LOS threshold.
	 *  - 1: LOS, The received signal level is below or equal to the LOS
	 *  threshold.
	 */
	uint32_t rx_los;
	/** Transmitter disable status.
	 *  - 0: EN, The transmitter is enabled.
	 *  - 1: DIS, The transmitter is disabled.
	 */
	uint32_t tx_disable;
	/** Transmitter fault status.
	 *  - 0: OK, The transmitter is ok.
	 *  - 1: FAULT, The transmitter is in fault state.
	 */
	uint32_t tx_fault;
};

/** Optic operations structure */
struct pa_optic_ops {
	/** Get data from the EEPROM-memory-mapped configuration.
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[in] ddmi_page DDMI memory page address.
	 * \param[out] data Pointer to the EEPROM memory buffer.
	 * \param[in] offset Address offset value.
	 * \param[in] data_size Size of the EEPROM memory buffer.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*eeprom_data_get)(void *ll_handle,
						  enum pa_ddmi_page ddmi_page,
						  unsigned char *data,
						  long offset,
						  size_t data_size);

	/**
	 * Get the optical interface properties
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[out] param Pointer to a structure as defined
	 * by \ref pa_optic_properties.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*optic_properties_get)(void *ll_handle,
				struct pa_optic_properties *props);

	/**
	 * Get the optical status
	 *
	 * \param[in] ll_handle Lower layer handle
	 * \param[out] param Pointer to a structure as defined
	 * by \ref pa_optic_status.
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno
	(*optic_status_get)(void *ll_handle,
			    struct pa_optic_status *props);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif
#endif
