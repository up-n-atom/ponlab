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
 *  and data structures for controlling the PON IP module in one of the ITU PON
 *  operation modes (GPON, XG-PON, XGS-PON, NG-PON2).
 */

#ifndef _FAPI_PON_GPON_H_
#define _FAPI_PON_GPON_H_

/** \addtogroup PON_FAPI_REFERENCE
 *   @{
 */

/** \defgroup PON_FAPI_REFERENCE_GPON GPON-specific PON Library Functions
 *	      This chapter describes the functional API interface to access
 *	      functions that are specific to ITU-based PON technologies.
   @{
*/

/* GPON-specific enumerations */
/* ========================== */

/** Power Saving Mode states as defined by ITU-T G.984.3, ITU-T G.987.3,
 *  ITU-T G.989.3, and ITU-T G.9807.1.
 *  \remark Do not change values from 0 to 7 as they need to be in sync with the
 *  PON IP firmware! */
enum psm_state {
	/** Active Held state.
	 *  The ONU is fully responsive, forwarding downstream traffic and
	 *  responding to all bandwidth allocations. Power management state
	 *  transitions do not occur. The minimum sojourn in this state is
	 *  enforced by the Thold timer. Upon entry to this state, the ONU
	 *  sends a Sleep_Request (Awake) PLOAM message.
	 */
	PSM_STATE_ACTIVE_HELD = 0,
	/** Active Free state.
	 *  The ONU is fully responsive, forwarding downstream traffic and
	 *  responding to all bandwidth allocations. Power management state
	 *  transition requests are a local decision.
	 */
	PSM_STATE_ACTIVE_FREE = 1,
	/** Sleep Aware state.
	 *  For description see \ref PSM_STATE_DOZE_AWARE.
	 *  This state exists only in the cyclic sleep mode.
	 */
	PSM_STATE_SLEEP_AWARE = 2,
	/** Asleep state.
	 *  The ONU shuts down both its receiver and transmitter, retaining the
	 *  ability to wake up on local stimulus. This state persists for a
	 *  specified duration Ilowpower if not truncated by the arrival of a
	 *  local stimulus LWI. Before exiting this state, the ONU ensures that
	 *  it is fully powered up, synchronized, and capable of responding to
	 *  both upstream and downstream traffic and control.
	 *  This state exists only in the cyclic sleep mode.
	 */
	PSM_STATE_ASLEEP = 3,
	/** Doze Aware state.
	 *  Both ONU receiver and transmitter remain on. This state persists for
	 *  a specified duration Iaware if not truncated by the arrival of a
	 *  local stimulus LWI or receipt of SA(OFF) or FWI from the OLT.
	 *  The ONU forwards downstream traffic and responds to all bandwidth
	 *  allocations.
	 *  It is the responsibility of the OLT to transmit bandwidth
	 *  allocations containing the PLOAMu flag with frequency sufficient to
	 *  ensure that an aware ONU sees at least one.
	 *  This state exists only in the doze mode.
	 */
	PSM_STATE_DOZE_AWARE = 4,
	/** Listen state.
	 *  The ONU receiver is on; the transmitter is off. The ONU listens to
	 *  the downstream signal and forwards downstream traffic, while
	 *  retaining the ability to reactivate the transmitter on local or
	 *  remote stimulus. This state persists for a specified
	 *  duration Ilowpower if not truncated by the arrival of a local
	 *  stimulus LWI or receipt of SA(OFF) or FWI from the OLT. Before
	 *  exiting this state, the ONU ensures that it is fully powered up and
	 *  capable of responding to both upstream and downstream traffic and
	 *  control.
	 *  This state exists only in the doze mode.
	 */
	PSM_STATE_LISTEN = 5,
	/** Watch Aware state.
	 *  For description see \ref PSM_STATE_DOZE_AWARE.
	 *  This state exists only in the watchful sleep mode.
	 */
	PSM_STATE_WATCH_AWARE = 6,
	/** Watch state.
	 *  The ONU transmitter is off. The ONU periodically turns on the
	 *  receiver for a brief time to check the downstream signal for remote
	 *  wakeup indications. When the downstream signal is checked, the ONU
	 *  does not respond to bandwidth allocations and does not forward
	 *  downstream traffic. This state persists for a specified
	 *  duration Ilowpower if not truncated by the arrival of a local
	 *  stimulus LWI or receipt of SA(OFF) or FWI from the OLT. Before
	 *  exiting this state, the ONU ensures that it is fully powered up
	 *  and capable of responding to both upstream and downstream traffic
	 *  and control.
	 *  This state exists only in the watchful sleep mode.
	 */
	PSM_STATE_WATCH = 7,
	/** Wait state.
	 *  State to wait for receiver and transmitter to be ready.
	 */
	PSM_STATE_WAIT = 8,
	/** Idle state.
	 *  State to handle inactive power saving mode (PSM_OFF) and if
	 *  PLOAM state is not O5.
	 */
	PSM_STATE_IDLE = 9,
	/** Active state.
	 *  State to handle inactive power saving mode (PSM_OFF) and if
	 *  PLOAM state is O5.
	 */
	PSM_STATE_ACTIVE = 10,
};

/* GPON structure definitions */
/* ========================== */
/* Add GPON-specific structures here */

/** Structure to define the OMCI message encapsulation between the PON IP
 *  hardware and the receiving network interface.
 *  This structure is used by \ref fapi_pon_omci_cfg_set
 *  and \ref fapi_pon_omci_cfg_get.
 */
struct pon_omci_cfg {
	/** PON IP MAC address.
	 *  Used as MAC source address in OMCC downstream
	 *  and as MAC destination address in OMCC upstream.
	 */
	uint8_t mac_sa[6];
	/** SoC MAC address
	 *  Used as MAC destination address in OMCC downstream
	 *  and as MAC source address in OMCC upstream.
	 */
	uint8_t mac_da[6];
	/** Ethertype value, used in both directions. */
	uint16_t ethertype;
	/** Protocol value, used in both directions. */
	uint8_t protocol[5];
};

/** GEM port structure, provides the properties of a single GEM port.
 *  Used by \ref fapi_pon_gem_port_id_get and \ref fapi_pon_gem_port_index_get.
 */
struct pon_gem_port {
	/** GEM port index. */
	uint8_t gem_port_index;
	/** GEM port id. */
	uint16_t gem_port_id;
	/** Allocation ID (T-CONT ID). */
	uint16_t alloc_id;
	/** Allocation validity.
	 *  - 0: Invalid, There is no allocation assigned to the GEM
	 *       port, the alloc_id value is invalid and shall be ignored.
	 *  - 1: Valid, The allocation given by alloc_id is valid.
	 */
	uint32_t alloc_valid;
	/** Encryption key ring.
	 *  This value is used for XG-PON, XGS-PON, and NG-PON2 only
	 *  and otherwise ignored.
	 *  - 0: None, No encryption. The downstream key index is ignored,
	 *       and upstream traffic is transmitted with key index 0.
	 *  - 1: Unicast, Unicast payload encryption in both directions.
	 *       Keys are generated by the ONU and transmitted to the
	 *       OLT via the PLOAM channel.
	 *  - 2: Broadcast, Broadcast (multicast) encryption. Keys are
	 *       generated by the OLT and distributed via the OMCI.
	 *  - 3: Unicast downstream, Unicast encryption in downstream only.
	 *       Keys are generated by the ONU and transmitted to the OLT via
	 *       the PLOAM channel.
	 *  - Other: Reserved, Ignore and do not use.
	 */
	uint8_t encryption_key_ring;
	/** Downstream property.
	 *  - 0: Disabled, The GEM port is disabled in downstream direction.
	 *  - 1: Enabled, The GEM port is enabled in downstream direction.
	 */
	uint8_t is_downstream;
	/** Upstream property.
	 *  - 0: Disabled, The GEM port is disabled in upstream direction.
	 *  - 1: Enabled, The GEM port is enabled in upstream direction.
	 */
	uint8_t is_upstream;
	/** Payload type property.
	 *  This value is used to identify the payload type
	 *  which is transported over a GEM port.
	 *  - 0: Ethernet, This is an Ethernet GEM port.
	 *  - 1: OMCI, This is the OMCI GEM port.
	 *  - 2: Reserved, This type is reserved and shall not be used.
	 *  - 3: Other, This type is reserved and shall not be used.
	 */
	uint8_t payload_type;
	/** Maximum GEM/XGEM frame size.
	 *  The maximum size of GEM/XGEM frames to be sent in upstream
	 *  direction.
	 *  The value is given in number of bytes.
	 */
	uint16_t gem_max_size;
	/** This link reference identifies a specific linking of an allocation.
	 *  ID to an allocation's hardware index.
	 */
	uint32_t alloc_link_ref;
};

/** GPON allocation (T-CONT) structure.
 *
 *  If called with a given alloc_id (\ref fapi_pon_alloc_id_get), the selected
 *  alloc_index is returned upon successful completion.
 *
 *  If called with a given alloc_index (\ref fapi_pon_alloc_index_get),
 *  the selected alloc_id is returned upon successful completion.
 */
struct pon_allocation {
	/** Allocation ID (T-CONT ID). */
	uint16_t alloc_id;
	/** Allocation index (T-CONT index). */
	uint8_t alloc_index;
};

/** Allocation ID structure.
 *  Used by \ref fapi_pon_alloc_index_get, \ref fapi_pon_gem_port_alloc_get.
 */
struct pon_allocation_id {
	/** Allocation ID (T-CONT ID).
	 *  The valid range is from 0 to 16383.
	 */
	uint16_t alloc_id;
	/** Allocation Index Status Indication
	 *  - 0: UNUSED The Alloc ID has not been assigned by the
	 *       OLT and is free to be used. In this case ALLOC_ID and
	 *       ALLOC_LINK_REF will return zero.
	 *  - 1: REQUESTED The Alloc ID has been requested by the
	 *       OLT through OMCI but the OLT has not yet assigned
	 *       this Alloc ID through PLOAM.
	 *  - 2: ASSIGNED The Alloc ID has been assigned by the
	 *       OLT through PLOAM but the software has not yet
	 *       requested this for OMCI-controlled GEM port
	 *       mapping. It can be deleted through PLOAM without
	 *       further notice to the software.
	 *  - 3: LINKED The Alloc ID has been assigned by the OLT
	 *       through PLOAM and the software has requested this for
	 *       OMCI-controlled GEM port mapping. It cannot be
	 *       deleted through PLOAM without waiting for software
	 *       confirmation through ALLOC_ID_UNLINK.
	 *  - 4: BLOCKED The Alloc ID has been de-assigned by the
	 *       OLT through PLOAM but the software has requested
	 *       this for OMCI-controlled GEM port mapping. It cannot
	 *       be deleted through PLOAM without waiting for
	 *       software confirmation through ALLOC_ID_UNLINK.
	 */
	uint8_t status;
	/** This link reference identifies a specific linking of an allocation.
	 *  ID to an allocation's hardware index.
	 */
	uint32_t alloc_link_ref;
};

/** Allocation index structure.
 *  Used by \ref fapi_pon_alloc_id_get.
 */
struct pon_allocation_index {
	/** Allocation Index (T-CONT Index).
	 *  The valid range is defined by the PON IP capabilities.
	 */
	uint8_t alloc_index;
	/** Allocation ID Hardware Status
	 *  This flag indicates, if the requested Allocation ID creation
	 *  and linking could be completed as it was already created
	 *  through the PLOAM channel in the hardware table.
	 *  - 0: REQUESTED, Allocation ID creation is pending in state
	 *       REQUESTED or ASSIGNED.
	 *  - 1: LINKED Allocation ID creation is complete.
	 */
	uint8_t hw_status;
	/** This link reference identifies a specific linking of an allocation.
	 *  ID to an allocation's hardware index.
	 */
	uint32_t alloc_link_ref;
};

/** Set the QoS port index to this value to delete it. */
#define PON_QOS_CFG_REMOVE	 -1

/** Registration ID size (number of bytes) */
#define PON_REG_ID_SIZE 36

/** PON IP basic configuration structure for GPON modes.
 *  Used by \ref fapi_pon_gpon_cfg_set and \ref fapi_pon_gpon_cfg_get.
 */
struct pon_gpon_cfg {
	/** PON operation mode.
	 *  Selection of a not supported operation mode is rejected and
	 *  answered by an error code.*/
	enum pon_mode mode;
	/** Serial number. */
	uint8_t serial_no[PON_SERIAL_NO_SIZE];
	/** Password, this is used in G.984 mode only.
	 *  If unused, it is ignored in the "set" function.
	 *  For data security reasons the password cannot be read back from
	 *  the PON IP hardware and is reported as a string of spaces by
	 *  the "get" function.
	 */
	uint8_t password[PON_PASSWD_SIZE];
	/** Registration ID, this is used only in operation modes other than
	 *  G.984. In G.984 mode, this value is ignored by the "set" function.
	 *  For data security reasons the reg_id cannot be read back from
	 *  the PON IP hardware and is reported as a string of spaces by
	 *  the "get" function.
	 */
	uint8_t reg_id[PON_REG_ID_SIZE];
	/** ONU identifier, a value between 0 and 15 can be used which shall be
	 *  different for each ONU which is connected to the same OLT port.
	 */
	uint32_t ident;
	/** PLOAM emergency stop state (activation state O7).
	 *  - 0: Normal, The last state before restart was not O7,
	 *       the PLOAM state machine shall start up normally.
	 *  - 1: Stop, The last state before restart was O7,
	 *       the PLOAM state machine shall start up in this state.
	 */
	uint32_t stop;
	/** Power leveling capability.
	 *  This parameter is used only for the NG-PON2 operation mode and
	 *  ignored otherwise (to be set to 0). It informs the OLT about the
	 *  optical transmitter's ability to respond to transmit power level
	 *  change requests. It is a seven-bit bitmap of the form 0b0CCC CCCC,
	 *  whereby a bit in the K-th least significant position indicates that
	 *  the ONU supports the attenuation level of 3K dB. The attempt to set
	 *  any other value will be rejected.
	 *  For example, 0b0000 0010 indicates support of 6 dB attenuation
	 *  level.
	 */
	uint32_t plev_cap;
	/** Discovery Timer time-out value, given in multiples of 1 ms */
	uint32_t ploam_timeout_0;
	/** Ranging Timer time-out value, given in multiples of 1 ms */
	uint32_t ploam_timeout_1;
	/** LODS Timer time-out value, given in multiples of 1 ms */
	uint32_t ploam_timeout_2;
	/** LODS Timer with WLCP time-out value, given in multiples of 1 ms.
	 *  This value is ignored in G.984 operation mode and reported back
	 *  as 0.
	 */
	uint32_t ploam_timeout_3;
	/** Downstream Tuning Timer time-out value, given in multiples of 1 ms.
	 *  This value is ignored in G.984 operation mode and reported back
	 *  as 0.
	 */
	uint32_t ploam_timeout_4;
	/** Upstream Tuning Timer time-out value, given in multiples of 1 ms.
	 *  This value is ignored in G.984 operation mode and reported back
	 *  as 0.
	 */
	uint32_t ploam_timeout_5;
	/** Lost ONU Time time-out value, given in multiples of 1 ms.
	 *  This value is ignored in G.984 operation mode and reported back
	 *  as 0.
	 */
	uint32_t ploam_timeout_6;
	/** Change Transmit Power Level time-out value, given in multiples
	 *  of 1 ms.
	 */
	uint32_t ploam_timeout_cpl;
	/** Timeout for Channel Partition Index Searching.
	 *  Given in multiples of 1 ms.
	 *  Valid times are in the range from 1 ms to 2^20-1 ms.
	 *  The special value of 0 disables the timeout (infinity).
	 *  This value is applicable in NG-PON2 operation mode only and
	 *  else shall be set to 0.
	 */
	uint32_t ploam_timeout_cpi;
	/** Timeout for the TProfileDwell timer.
	 *  Given in multiples of 1 ms.
	 *  Valid times are in the range from 1 ms to 2^16-1 ms.
	 *  ITU-T G.989 recommends a value of at least 10 s (10000 ms).
	 *  The special value of 0 disables the timeout.
	 *  This value is applicable in NG-PON2 operation mode only and
	 *  else shall be set to 0.
	 */
	uint32_t ploam_timeout_tpd;
	/** Enable TDM co-existence operation of XG-PON and XGS-PON.
	 *  - 0: DIS, TDM co-existence operation mode is disabled.
	 *  - 1: EN, TDM co-existence operation mode is enabled.
	 */
	uint32_t tdm_coexistence;
	/** Control Dying Gasp handling.
	 *  This setting initializes the handling of under-voltage events.
	 *  - 0: DGEN, Dying Gasp handling is enabled (PLOAM message for GPON or
	 *             XGTC flag in other ITU PON modes).
	 *  - 1: DGDIS, Dying Gasp handling is disabled (for applications
	 *              which do not provide the under-voltage detection input
	 *              signal).
	 */
	uint32_t dg_dis;
	/** Downstream Ethernet FCS Mode
	 *  - 0: The FCS bytes are stripped from the Ethernet frames
	 *  - 1: The new FCS bytes are added to the Ethernet frames after the
	 *       old FCS has been removed
	 */
	uint32_t ds_fcs_en;
	/** Downstream Timestamp Disable
	 *  - 0: A timestamp is attached to the Ethernet packets
	 *  - 1: No timestamp is attached to the Ethernet packets.
	 */
	uint32_t ds_ts_dis;
};

/** PON IP credential configuration structure for ITU PON modes.
 *  Used by \ref fapi_pon_cred_set and \ref fapi_pon_cred_get.
 */
struct pon_cred_cfg {
	/** Serial number.
	 *  For data security reasons the serial number cannot be read back from
	 *  the PON IP hardware and is reported as a string of spaces by
	 *  the "get" function, if debug mode is not enabled.
	 */
	uint8_t serial_no[PON_SERIAL_NO_SIZE];
	/** Password, this is used in G.984 mode only.
	 *  If unused, it is ignored in the "set" function.
	 *  For data security reasons the password cannot be read back from
	 *  the PON IP hardware and is reported as a string of spaces by
	 *  the "get" function, if debug mode is not enabled.
	 */
	uint8_t password[PON_PASSWD_SIZE];
	/** Registration ID, this is used only in operation modes other than
	 *  G.984. In G.984 mode, this value is ignored by the "set" function.
	 *  For data security reasons the registration ID cannot be read back
	 *  from the PON IP hardware and is reported as a string of spaces by
	 *  the "get" function, if debug mode is not enabled.
	 */
	uint8_t reg_id[PON_REG_ID_SIZE];
};

/** GPON PLOAM activation state information.
 *  Used by \ref fapi_pon_ploam_state_get.
 */
struct pon_ploam_state_evt {
	/** Current PLOAM state.
	 *  - 10: O1, Initial state
	 *  - 11: O1.1, Initial off-sync state
	 *  - 12: O1.2, Initial profile learning state
	 *  - 23: O23, Serial number state
	 *  - 40: O4, Ranging state
	 *  - 50: O5, Operational state
	 *  - 51: O5.1, Associated state
	 *  - 52: O5.2, Pending state
	 *  - 60: O6, Intermittent LOS state
	 *  - 70: O7, Emergency stop state
	 *  - 80: O8, Downstream tuning state
	 *  - 81: O8.1, Downstream tuning off-sync state
	 *  - 82: O8.2, Downstream tuning profile learning state
	 *  - 90: O9, Upstream tuning state
	 */
	uint32_t current;
	/** Previous PLOAM state.
	 *  - 10: O1, Initial state
	 *  - 11: O1.1, Initial off-sync state
	 *  - 12: O1.2, Initial profile learning state
	 *  - 23: O23, Serial number state
	 *  - 40: O4, Ranging state
	 *  - 50: O5, Operational state
	 *  - 51: O5.1, Associated state
	 *  - 52: O5.2, Pending state
	 *  - 60: O6, Intermittent LOS state
	 *  - 70: O7, Emergency stop state
	 *  - 80: O8, Downstream tuning state
	 *  - 81: O8.1, Downstream tuning off-sync state
	 *  - 82: O8.2, Downstream tuning profile learning state
	 *  - 90: O9, Upstream tuning state
	 */
	uint32_t previous;
	/** PLOAM change time, given in multiples of 125 us.
	 *  This is the time which has been spent in the previous state before
	 *  the most recent state change.
	 */
	uint64_t time_prev;
	/** reason for last activation (a.k.a. PLOAM) state change */
	uint32_t change_reason;
};

/** GPON PLOAM activation state information.
 *  Used by \ref fapi_pon_ploam_state_get.
 */
struct pon_ploam_state {
	/** Current PLOAM state.
	 *  - 10: O1, Initial state
	 *  - 11: O1.1, Initial off-sync state
	 *  - 12: O1.2, Initial profile learning state
	 *  - 23: O23, Serial number state
	 *  - 40: O4, Ranging state
	 *  - 50: O5, Operational state
	 *  - 51: O5.1, Associated state
	 *  - 52: O5.2, Pending state
	 *  - 60: O6, Intermittent LOS state
	 *  - 70: O7, Emergency stop state
	 *  - 80: O8, Downstream tuning state
	 *  - 81: O8.1, Downstream tuning off-sync state
	 *  - 82: O8.2, Downstream tuning profile learning state
	 *  - 90: O9, Upstream tuning state
	 */
	uint32_t current;
	/** Previous PLOAM state.
	 *  - 10: O1, Initial state
	 *  - 11: O1.1, Initial off-sync state
	 *  - 12: O1.2, Initial profile learning state
	 *  - 23: O23, Serial number state
	 *  - 40: O4, Ranging state
	 *  - 50: O5, Operational state
	 *  - 51: O5.1, Associated state
	 *  - 52: O5.2, Pending state
	 *  - 60: O6, Intermittent LOS state
	 *  - 70: O7, Emergency stop state
	 *  - 80: O8, Downstream tuning state
	 *  - 81: O8.1, Downstream tuning off-sync state
	 *  - 82: O8.2, Downstream tuning profile learning state
	 *  - 90: O9, Upstream tuning state
	 */
	uint32_t previous;
	/** PLOAM State Time, given in multiple of 1 second.
	 * This is the time which has been spent in the current state after
	 * the most recent state change
	 */
	uint64_t time_curr;
	/** reason for last activation (a.k.a. PLOAM) state change */
	uint32_t change_reason;
};

/** GPON overall status information.
 *  Used by \ref fapi_pon_gpon_status_get.
 */
struct pon_gpon_status {
	/** Number of available GEM/XGEM ports. */
	uint32_t gem_ports;
	/** Number of available allocation IDs (T-CONTs). */
	uint16_t alloc_id;
	/** ONU response time (ns). */
	uint32_t onu_resp_time;
	/** GTC synchronization status.
	 *  - 0: Asynchronous, The GTC/XGTC hardware is in asynchronous state.
	 *  - 1: Synchronous, The GTC/XGTC hardware is in synchronous state.
	 */
	uint32_t gtc_stat;
	/** Current PLOAM state.
	 *  - 10: O1, Initial state
	 *  - 11: O1.1, Initial off-sync state
	 *  - 12: O1.2, Initial profile learning state
	 *  - 23: O23, Serial number state
	 *  - 40: O4, Ranging state
	 *  - 50: O5, Operational state
	 *  - 51: O5.1, Associated state
	 *  - 52: O5.2, Pending state
	 *  - 60: O6, Intermittent LOS state
	 *  - 70: O7, Emergency stop state
	 *  - 80: O8, Downstream tuning state
	 *  - 81: O8.1, Downstream tuning off-sync state
	 *  - 82: O8.2, Downstream tuning profile learning state
	 *  - 90: O9, Upstream tuning state
	 */
	uint32_t ploam_state;
	/** Previous PLOAM state.
	 *  - 10: O1, Initial state
	 *  - 11: O1.1, Initial off-sync state
	 *  - 12: O1.2, Initial profile learning state
	 *  - 23: O23, Serial number state
	 *  - 40: O4, Ranging state
	 *  - 50: O5, Operational state
	 *  - 51: O5.1, Associated state
	 *  - 52: O5.2, Pending state
	 *  - 60: O6, Intermittent LOS state
	 *  - 70: O7, Emergency stop state
	 *  - 80: O8, Downstream tuning state
	 *  - 81: O8.1, Downstream tuning off-sync state
	 *  - 82: O8.2, Downstream tuning profile learning state
	 *  - 90: O9, Upstream tuning state
	 */
	uint32_t ploam_state_previous;
	/** PLOAM O5 change time, time of last state change from or into O5
	state, given in multiples of 125 us related to the current time. */
	uint64_t time_prev;
	/** reason for last activation (a.k.a. PLOAM) state change */
	uint32_t ploam_state_change_reason;
	/** Power saving state.
	 *  - 0: AH, Active held state.
	 *  - 1: AF, Active free state.
	 *  - 2: SA, Sleep aware state.
	 *  - 3: AS, Asleep state.
	 *  - 4: DA, Doze aware state.
	 *  - 5: LS, Listen state.
	 *  - 6: WA, Watch aware state.
	 *  - 7: WT, Watch state.
	 */
	uint32_t psm_state;
	/** FEC status downstream.
	 *  - 0: Disabled, FEC is not used in downstream direction.
	 *  - 1: Enabled, FEC is used in downstream direction.
	 */
	uint32_t fec_status_ds;
	/** FEC status upstream.
	 *  - 0: Disabled, FEC is not used in upstream direction.
	 *  - 1: Enabled, FEC is used in upstream direction.
	 */
	uint32_t fec_status_us;
	/** ONU ID */
	uint32_t onu_id;
	/** Equalization delay, given in multiples of upstream bit periods
	 *  related to the nominal upstream rate of 2.48832 Gbit/s, independent
	 *  of the actual update data rate in use (2.48832 or 9.95328 Gbit/s).
	 *  One bit period represents a time of 0.4018 ns.
	 */
	uint32_t eq_del;
	/** ONU authentication status.
	 *  - 0: Indeterminate, The mutual authentication has not been
	 *       completed.
	 *  - 1: Reserved, Set to 0.
	 *  - 2: Reserved, Set to 0.
	 *  - 3: Success, The mutual authentication has been successfully
	 *       completed.
	 *  - 4: Fail, The mutual authentication has failed.
	 */
	uint32_t auth_status;
	/** PON ID value as received through PLOAMd (in G.984 operation mode)
	 *  or within the OC downstream header field (in other operation modes).
	 *  This value is valid only if in PLOAM state O2, O3, O4, or O5,
	 * otherwise a value of 0 is reported which shall be ignored.
	 *  If in G.984 operation mode, all 7 bytes are used. In all other
	 *  operation modes only the least significant 4 bytes are used,
	 *  others are set to 0.
	 */
	uint8_t pon_id[7];
	/** Transmit Optical Level (TOL) value received in the OC structure of
	 *  the XGTC downstream header. The lower 9 bit are valid.
	 *  Its value is an integer representing a logarithmic power measure
	 *  having 0.1 dB granularity with respect to −30 dBm.
	 *  The default value of 0x1FF indicates that TOL is not supported on
	 *  the given PON interface.
	 */
	uint32_t oc_tol;
	/** PON ID Type (PIT) value received in the OC structure of the XGTC
	 *  downstream header. The 8-bit PIT is held in the LSB part of oc_pit.
	 *  - Bit 1:0: Link type
	 *  - Bit 2: Protocol indication flag (P flag).
	 *           Set to 1 for G.989.3 TC layer mode.
	 *  - Bit 3: Downstream FEC indication, set to 1 if enabled by the OLT.
	 *  - Bit 6:4: ODN class as defined by G.989.2
	 *  - Bit 7: RE flag, indicates a reach extender between ONU and OLT.
	 */
	uint32_t oc_pit;
	/** C value (TOL reference point indicator) received in the OC
	 *  structure of the XGTC downstream header.
	 *  - 0: The TOL value below refers to the S/R-CG reference point.
	 *  - 1: The TOL value below refers to the S/R-CP reference point.
	 */
	uint32_t oc_c;
	/** R value received in the OC structure of the XGTC downstream
	 *  header. This value is reserved for future use.
	 */
	uint32_t oc_r;
	/** The P value received in the OC structure of the XGTC downstream
	 *  header is not stable but changes between downstream frames.
	 *  This is an indication that the OLT runs the system in
	 *  "XG-PON/XGS-PON/NG-PON2 coexistence mode".
	 *  - 0: FIXED, The operation mode is fixed to what is indicated
	 *       by the P bit of the PIT field.
	 *  - 1: COEX, The operation mode is changing, the P bit toggles.
	 */
	uint32_t oc_coex;
	/** Downstream TWDM channel ID.
	 *  The valid value range is from 0 to 7.
	 *  This value is set to 0 for single-wavelength applications.#
	 */
	uint32_t ds_ch_index;
	/** Upstream TWDM channel ID.
	 *  The valid value range is from 0 to 7.
	 *  This value is set to 0 for single-wavelength applications.
	 */
	uint32_t us_ch_index;
	/** GPON operation mode. */
	enum pon_mode pon_mode;
	/** GPON downstream data rate. */
	enum pon_ds_rate pon_ds_rate;
	/** PON upstream data rate. */
	enum pon_us_rate pon_us_rate;
};

/** XGTC reboot request information.
 *  TODO: Implementation of the event handling for this function.
 */
struct pon_xgtc_reboot_request {
	/** This is the reboot request PLOAM message type which has been
	 *  received.
	 *  - 0: OFF, Alarm is not active.
	 *  - 1: MIB, A MIB reset has been requested by the OLT.
	 *  - 2: OMCI, A reset similar to an OMCI-triggered reset has been
	 *       requested by the OLT.
	 *  - 3: PC, A power cycle reset has been requested by the OLT.
	 *  - 4: CONFIG, A configuration reset followed by a MIB reset has
	 *       been requested by the OLT.
	 */
	uint32_t reboot;
	/** Reboot request condition.
	 *  This is the reboot condition propagated through the PLOAM
	 *  message, coded as a bit map with the following meaning:
	 *  - Bit 0 == 0: Use the currently committed image to reboot.
	 *  - Bit 0 == 1: Use the currently not committed image to reboot.
	 *  - Bit 1 == 0: Reboot independently of the PLOAM state.
	 *  - Bit 1 == 1: Reboot only if in PLOAM state O1, O2, or O3.
	 *  - Bit 3:2 == 00: Reboot regardless of the VoIP status.
	 *  - Bit 3:2 == 01: Reboot only if no VoIP call is in progress.
	 *  - Bit 3:2 == 10: Reboot only if no emergency call is in
	 *                   progress.
	 */
	uint32_t reboot_condition;
};

/** GEM/XGEM port counters.
 *  Used by \ref fapi_pon_gem_port_counters_get and
 *  \ref fapi_pon_gem_all_counters_get.
 */
struct pon_gem_port_counters {
	/** GEM port ID for which the counters shall be reported. */
	uint16_t gem_port_id;
	/** Transmitted GEM frames. */
	uint64_t tx_frames;
	/** Transmitted GEM frame fragments. */
	uint64_t tx_fragments;
	/** Transmitted bytes in GEM frames.
	 *  This value reports user payload bytes only, not including
	 *  header bytes.
	 */
	uint64_t tx_bytes;
	/** Received GEM frames.
	 *  This value reports the number of valid GEM frames that have
	 *  been received without uncorrectable errors and a valid HEC.
	 */
	uint64_t rx_frames;
	/** Received GEM frame fragments. */
	uint64_t rx_fragments;
	/** Received bytes in valid GEM frames.
	 *  This value reports user payload bytes only, not including
	 *  header bytes.
	 */
	uint64_t rx_bytes;
	/** XGEM key errors.
	 *  The number of received key errors is provided for XG-PON,
	 *  XGS-PON, and NG-PON2 only. The value is set to 0 for GPON
	 *  implementations.
	 */
	uint64_t key_errors;
};

/** Structure to collect counters related to GEM TC reception and transmission.
*/
struct pon_gtc_counters {
	/** Number of BIP errors. */
	uint64_t bip_errors;
	/** Number of discarded GEM frames due to an invalid HEC.
	 *  Functionally the same as gem_hec_errors_uncorr,
	 *  which is not provided as a separate counter.
	 */
	uint64_t disc_gem_frames;
	/** Number of corrected GEM HEC errors. */
	uint64_t gem_hec_errors_corr;
	/** Number of uncorrected GEM HEC errors. */
	uint64_t gem_hec_errors_uncorr;
	/** Number of corrected bandwidth map HEC errors. */
	uint64_t bwmap_hec_errors_corr;
	/** Number of bytes received in corrected FEC codewords. */
	uint64_t bytes_corr;
	/** Number of FEC codewords corrected */
	uint64_t fec_codewords_corr;
	/** Number of uncorrectable FEC codewords */
	uint64_t fec_codewords_uncorr;
	/** Number of total received frames */
	uint64_t total_frames;
	/** Number FEC errored seconds */
	uint64_t fec_sec;
	/** Number Idle GEM errors */
	uint64_t gem_idle;
	/** Number of downstream synchronization losses */
	uint64_t lods_events;
	/** Dying Gasp activation time, given in multiples of 125 us */
	uint64_t dg_time;
	/** Number of PLOAM CRC errors */
	uint64_t ploam_crc_errors;
};

/** Structure to collect counters related to XGEM TC reception and transmission.
 *  Used by \ref fapi_pon_xgtc_counters_get.
*/
struct pon_xgtc_counters {
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
};

/** Downstream FEC counters for ITU PON operation modes.
 *  Used by \ref fapi_pon_fec_counters_get.
 */
struct pon_fec_counters {
	/** Corrected bytes.
	 *  This is the number of bytes that could be corrected by the
	 *  downstream FEC process.
	 */
	uint64_t bytes_corr;
	/** Corrected code words.
	 *  This is the number of FEC code words which could be corrected.
	 */
	uint64_t words_corr;
	/** Uncorrectable code words.
	 *  This is the number of received FEC code words which could not
	 *  be corrected.
	 */
	uint64_t words_uncorr;
	/** Total number of code words.
	 *  This is the total number of received FEC code words.
	 */
	uint64_t words;
	/** FEC errored seconds.
	 *  Number of one-second intervals in which at least one
	 *  uncorrectable FEC error has been observed.
	 */
	uint64_t seconds;
};

/** Allocation-specific counters.
 *  Used by \ref fapi_pon_alloc_counters_get.
 */
struct pon_alloc_counters {
	/** Allocations received.
	 *  This is the number of individual allocations that have been
	 *  received for a given allocation (T-CONT).
	 */
	uint64_t allocations;
	/** GEM idle frames.
	 *  This is the number of GEM idle frames that have been sent
	 *  within the selected allocation (T-CONT).
	 *  It represents the available but unused upstream bandwidth.
	 */
	uint64_t idle;
	/** Upstream average bandwidth.
	 *  This is the assigned upstream bandwidth, averaged over 1 second.
	 *  The value is given in units of bit/s.
	 */
	uint64_t us_bw;
};

/** Structure to hold the GTC PLOAM message information for logging. */
struct pon_gtc_ploam_message {
	/** Message direction.
	 *  - 0: PON_DS, Downstream.
	 *  - 1: PON_US, Upstream.
	 */
	uint8_t direction;
	/** Message time stamp. */
	uint32_t time_stamp;
	/** ONU ID (valid range: 8-bit value). */
	uint32_t onu_id;
	/** Message type ID (valid range: 8-bit value). */
	uint32_t message_type_id;
	/** Message contents (10 byte).*/
	uint8_t message[10];
};

/** Structure to hold the XGTC PLOAM message information for logging. */
struct pon_xgtc_ploam_message {
	/** Message direction.
	 *  - 0: PON_DS, Downstream.
	 *  - 1: PON_US, Upstream.
	 */
	uint8_t direction;
	/** Message time stamp. */
	uint32_t time_stamp;
	/** ONU ID (valid range: 16-bit value). */
	uint32_t onu_id;
	/** Message type ID (valid range: 8-bit value). */
	uint32_t message_type_id;
	/** Message sequence number (valid range: 8-bit value).*/
	uint32_t message_seq_no;
	/** Message contents (36 byte).*/
	uint8_t message[36];
};

/** PLOAM downstream message counters.
 *  The available PLOAM downstream message types depend on the operation
 *  mode (GPON, XG-PON, XGS-PON, or NG-PON2).
 *  The counter increment rate is 2/125 us or slower.
 *  Counters for unused messages are always reported as 0.
 *  Used by \ref fapi_pon_ploam_ds_counters_get.
 */
struct pon_ploam_ds_counters {
	/** Upstream overhead message (GPON). */
	uint64_t us_overhead;
	/** Assign ONU ID message (GPON, XG-PON, NG-PON2, XGS-PON). */
	uint64_t assign_onu_id;
	/** Ranging time message (GPON, XG-PON, NG-PON2, XGS-PON). */
	uint64_t ranging_time;
	/** Deactivate ONU ID message (GPON, XG-PON, NG-PON2, XGS-PON). */
	uint64_t deact_onu;
	/** Disable serial number message (GPON, XG-PON, NG-PON2, XGS-PON). */
	uint64_t disable_ser_no;
	/** Encrypted port ID message (GPON). */
	uint64_t enc_port_id;
	/** Request password message (GPON). */
	uint64_t req_passwd;
	/** Assign allocation ID message (GPON, XG-PON, NG-PON2, XGS-PON). */
	uint64_t assign_alloc_id;
	/** No message (GPON). */
	uint64_t no_message;
	/** Popup message (GPON). */
	uint64_t popup;
	/** Request key message (GPON). */
	uint64_t req_key;
	/** Configure port ID message (GPON). */
	uint64_t config_port_id;
	/** Physical Equipment Error (PEE) message (GPON). */
	uint64_t pee;
	/** Change Power Level (CPL) message (GPON, NG-PON2). */
	uint64_t cpl;
	/** PON Section Trace (PST) message (GPON). */
	uint64_t pst;
	/** BER interval message (GPON). */
	uint64_t ber_interval;
	/** Key switching time message (GPON). */
	uint64_t key_switching;
	/** Extended burst length message (GPON). */
	uint64_t ext_burst;
	/** PON ID message (GPON). */
	uint64_t pon_id;
	/** Swift popup message (GPON). */
	uint64_t swift_popup;
	/** Ranging adjustment message (GPON). */
	uint64_t ranging_adj;
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
	/** Reset message. */
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

/** PLOAM upstream message counters.
 *  The available PLOAM upstream message types depend on the operation mode
 *  (GPON, XG-PON, XGS-PON, or NG-PON2).
 *  The counter increment rate is 1/125 us or slower.
 *  Counters for unused messages are always reported as 0.
 *  Used by \ref fapi_pon_ploam_us_counters_get.
 */
struct pon_ploam_us_counters {
	/** Serial number ONU message (GPON, XG-PON, NG-PON2, XGS-PON). */
	uint64_t ser_no;
	/** Password message (GPON). */
	uint64_t passwd;
	/** Dying Gasp (DG) message (GPON). */
	uint64_t dying_gasp;
	/** No message (GPON). */
	uint64_t no_message;
	/** Encryption key message (GPON). */
	uint64_t enc_key;
	/** Physical Equipment Error (PEE) message (GPON). */
	uint64_t pee;
	/** PON Section Trace (PST) message (GPON). */
	uint64_t pst;
	/** Remote Error Indication (REI) message (GPON). */
	uint64_t rei;
	/** Acknowledge message (GPON, XG-PON, NG-PON2, XGS-PON). */
	uint64_t ack;
	/** Sleep request message (GPON, XG-PON, NG-PON2, XGS-PON). */
	uint64_t sleep_req;
	/** Registration message (XG-PON, NG-PON2, XGS-PON). */
	uint64_t reg;
	/** Key report message (XG-PON, NG-PON2, XGS-PON). */
	uint64_t key_rep;
	/** Tuning response message (NG-PON2). */
	uint64_t tuning_resp;
	/** Power consumption report message (NG-PON2). */
	uint64_t power_rep;
	/** Rate response message (NG-PON2). */
	uint64_t rate_resp;
	/** Sum of all messages. */
	uint64_t all;
};

/** PLOAM downstream message forwarding configuration.
 *  Used by \ref fapi_pon_ploamd_cfg_set,
 *  \ref fapi_pon_ploamd_cfg_get.
 */
struct pon_ploamd_cfg {
	/** Array of PON downstream message type IDs.
	 *  Any values can be configured, even if not defined by the
	 *  PON standard which is in use. This allows to implement
	 *  vendor-specific PLOAMd message handling in software.
	 */
	uint8_t msg_type_id[16];
	/** Message forwarding enable.
	 *  Each entry in this array is related to one dedicated PLOAMd message
	 *  type as defined in msg_type_id.
	 *  - 0: DIS, PLOAMd forwarding is disabled.
	 *  - 1: EN, PLOAMd forwarding message is enabled.
	 */
	uint8_t enable[16];
};

/** PON GTC/XGTC configuration structure.
 *  Used by \ref fapi_pon_gtc_cfg_set and \ref fapi_pon_gtc_cfg_get
 *  to configure the downstream bit error rate thresholds.
 */
struct pon_gtc_cfg {
	/** Signal degrade threshold.
	 *  The valid range to be configured is from 4 to 10,
	 *  related to a bit error rate from 10exp-4 to 10exp-10.
	 */
	uint32_t sd_threshold;
	/** Signal fail threshold.
	 *  The valid range to be configured is from 3 to 9,
	 *  related to a bit error rate from 10exp-3 to 10exp-9.
	 */
	uint32_t sf_threshold;
};

/** Maximum size of GEM Port frame.
 * The maximum GEM frame size for ITU-T G.984 (GPON) operation mode is 4095.
 * The maximum XGEM frame size for ITU-T G.987, 9807, and 989 operation modes is
 * 16384. For compatibility across all modes we set this to 4095 by default.
 */
#define PON_GEM_PORT_MAX_FRAME_SIZE 4095

/** Priority Queue Scale Factor (PQSF) configuration.
 *  Used by \ref fapi_pon_pqsf_cfg_set and \ref fapi_pon_pqsf_cfg_get.
 */
struct pon_pqsf_cfg {
	/** Priority queue scale factor.
	 *  This factor defines the reporting unit of the priority queue filling
	 *  level through dynamic bandwidth reporting.
	 */
	int32_t pqsf;
};

/** Encryption configuration.
 *  Used by \ref fapi_pon_auth_enc_cfg_set and
 *  \ref fapi_pon_auth_enc_cfg_get
 *  for mutual authentication between OLT and ONU.
 */
struct pon_enc_cfg {
	/** Encryption mode */
	uint32_t enc_mode;
	/** Pre-shared key length */
	uint32_t key_size;
	/** Pre-shared key */
	/* FIXME: Replace with a pointer for flexible length support. */
	uint8_t psk[MAX_AUTH_TABLE_SIZE];
};

/** Generic structure to handle mutual authentication tables
 *
 *  Used by \ref fapi_pon_auth_olt_challenge_set,
 *	    \ref fapi_pon_auth_olt_result_set,
 *	    \ref fapi_pon_onu_rnd_chl_tbl,
 *	    \ref fapi_pon_onu_auth_res_tbl,
 *  for mutual authentication between OLT and ONU.
 *
 */
struct pon_generic_auth_table {
	/** Table size [byte]. */
	uint32_t size;
	/** Mutual authentication table. */
	uint8_t *table;
};

/** ONU Master Session Key hash value.
 *  Used by \ref fapi_pon_auth_onu_msk_hash_get
 *  for mutual authentication between OLT and ONU.
 */
struct pon_onu_msk_hash {
	/** Hash Algorithm Type.
	 *  - 0: AES-CMAC-128
	 *  - 1: HMAC-SHA-256
	 *  - 2: HMAC-SHA-512
	 *  Other values are reserved.
	 */
	uint8_t type;
	/** MSK hash value. */
	uint8_t hash[MAX_AUTH_TABLE_SIZE];
};

/** KEK-encrypted broadcast key value.
 *  Used by \ref fapi_pon_auth_onu_bc_key_set.
 *  If invalid values are used, the error code PON_STATUS_VALUE_RANGE_ERR
 *  will be returned.
 */
struct pon_onu_bc_key {
	/** Table size [byte]. */
	uint32_t size;
	/** Broadcast key value. */
	uint8_t *table;
	/** Broadcast key index.
	 *  - 0: ERR, This value shall never be used and will return an error.
	 *  - 1: FIRST, This is the first broadcast key definition, to be used
	 *       when the key index in the XGEM frame header is set to 0b01.
	 *  - 2: SECOND, This is the second broadcast key definition, to be used
	 *       when the key index in the XGEM frame header is set to 0b10.
	 *  - 3: RES, Reserved by ITU, this value shall not be used and return.
	 */
	uint32_t index;
};

/** Structure to set the Time of Day synchronously with the GPON, XG-PON,
 *  NG-PON2, or XGS-PON TC sublayer.
 *  Used by \ref fapi_pon_gpon_tod_sync_set, \ref fapi_pon_gpon_tod_sync_get
 */
struct pon_gpon_tod_sync {
	/** Multiframe (GTC/XGTC superframe) counter value for time
	 *  synchronization.
	 */
	uint32_t multiframe_count;
	/** Time of Day higher part, given in units of seconds. */
	uint32_t tod_seconds;
	/** Time of Day extended part, given in units of seconds. */
	uint32_t tod_extended_seconds;
	/** Time of Day lower part, given in units of nanoseconds. */
	uint32_t tod_nano_seconds;
	/** Time of Day application specific correction value, given in units
	 *  of picoseconds.
	 */
	int32_t tod_offset_pico_seconds;
	/** Time of Day clock quality level.
	 *  - 0: LOC, Local clock oscillator (free running mode).
	 *  - 1: OLT, Locked to the OLT's clock.
	 */
	int32_t tod_quality;
};

/** Size of the OMCI integrity key */
#define PON_OMCI_IKEY_SIZE 16

/** Structure to hold the OMCI integrity key (OMCI_IK).
 *  This structure is used by \ref fapi_pon_omci_ik_get.
 */
struct pon_omci_ik {
	/** OMCI integrity key.
	 *  key(0) = OMCI_IK(127:120)
	 *  key(1) = OMCI_IK(119:112)
	 *  up to
	 *  key(14) = OMCI_IK(15:8)
	 *  key(15) = OMCI_IK(7:0).
	 */
	uint8_t key[PON_OMCI_IKEY_SIZE];
};

/** GPON power saving mode configuration. Used by \ref fapi_pon_psm_cfg_set
 *  and \ref fapi_pon_psm_cfg_get.
 */
struct pon_psm_cfg {
	/** Power save state machine enable.
	 *  - 0: DIS, Disabled
	 *  - 1: EN, Enabled
	 */
	uint32_t enable;
	/** Power save state machine operation mode.
	 *   Only one of the operation modes can be selected.
	 *  Other codepoints are invalid.
	 *  - 1: DM, Doze mode
	 *  - 2: CSM, Cyclic sleep mode
	 *  - 4: WSL, Watchful sleep mode.
	 */
	uint32_t mode;
	/** Maximum sleep interval, given in multiples of 125 us. */
	uint32_t max_sleep_interval;
	/** Minimum aware interval, given in multiples of 125 us. */
	uint32_t min_aware_interval;
	/** Minimum active held interval, given in multiples of 125 us. */
	uint32_t min_active_held_interval;
	/** Maximum cyclic sleep interval, given in multiples of 125 us. */
	uint32_t max_rx_off_interval;
	/** Maximum doze mode interval, given in multiples of 125 us. */
	uint32_t max_doze_interval;
};

/** Power State Machine (PSM) counters.
 *  Used by \ref fapi_pon_psm_counters_get.
 */
struct pon_psm_counters {
	/** Time spent in "doze" mode, given in us. */
	uint64_t doze_time;
	/** Time spent in "cyclic sleep" mode, given in us. */
	uint64_t cyclic_sleep_time;
	/** Time spent in "watchful sleep" mode, given in us. */
	uint64_t watchful_sleep_time;
};

/** Power Saving State Machine (PSM) state reporting structure.
 *  Used by \ref fapi_pon_psm_state_get.
 */
struct pon_psm_state {
	/** Current PSM FSM state. */
	enum psm_state current;
};

/** PSM FSM state time. This reports the time which has been spent in each of
 *  the supported power saving states. Used by \ref fapi_pon_psm_time_get.
 */
struct pon_psm_fsm_time {
	/** Total IDLE state time, ms. */
	uint32_t state_idle;
	/** Total ACTIVE state time, ms. */
	uint32_t state_active;
	/** Total ACTIVE_HELD state time, ms. */
	uint32_t state_active_held;
	/** Total ACTIVE_FREE state time, ms. */
	uint32_t state_active_free;
	/** Total ASLEEP state time, ms. */
	uint32_t state_asleep;
	/** Total LISTEN state time, ms. */
	uint32_t state_listen;
	/** Total WATCH state time, ms. */
	uint32_t state_watch;
	/** Total DOZE_AWARE state time, ms. */
	uint32_t state_doze_aware;
	/** Total WATCH_AWARE state time, ms. */
	uint32_t state_watch_aware;
	/** Total SLEEP_AWARE state time, ms. */
	uint32_t state_sleep_aware;
	/** Total WAIT state time, ms. */
	uint32_t state_wait;
};

/** ITU Interoperability configuration.
 *  Used by \ref fapi_pon_iop_cfg_set and \ref fapi_pon_iop_cfg_get.
 */
struct pon_iop_cfg {
	/** Interoperability mask.
	 *  This mask defines the selected interoperability options.
	 *  See the definitions of "PON_IOP_CFG_..." for more detail.
	 */
	uint32_t iop_mask;
};

/** Ethernet frame receive or transmit packet and byte counters per GEM port.
 *  Used by \ref fapi_pon_eth_rx_counters_get and
 *  \ref fapi_pon_eth_tx_counters_get.
 */
struct pon_eth_counters {
	/** Ethernet payload bytes. */
	uint64_t bytes;
	/** Ethernet packets below 64 byte size. */
	uint64_t frames_lt_64;
	/** Ethernet packets of 64 byte size. */
	uint64_t frames_64;
	/** Ethernet packets of 65 to 127 byte size. */
	uint64_t frames_65_127;
	/** Ethernet packets of 128 to 255 byte size. */
	uint64_t frames_128_255;
	/** Ethernet packets of 256 to 511 byte size. */
	uint64_t frames_256_511;
	/** Ethernet packets of 512 to 1023 byte size. */
	uint64_t frames_512_1023;
	/** Ethernet packets of 1024 to 1518 byte size. */
	uint64_t frames_1024_1518;
	/** Ethernet packets of more than 1518 byte size. */
	uint64_t frames_gt_1518;
	/** Ethernet packets with incorrect FCS value. */
	uint64_t frames_fcs_err;
	/** Ethernet payload bytes in packets with incorrect FCS value. */
	uint64_t bytes_fcs_err;
	/** Ethernet packets which exceed the maximum length. */
	uint64_t frames_too_long;
};

/** PLOAM state machine re-ranging configuration and status indication.
 *  Re-ranging is triggered by simulation an LODS condition for the time
 *  defined by lods_time. For the duration of the simulated LODS condition
 *  the value of "active" is set to 1.
 *  Used by \ref fapi_pon_gpon_rerange_cfg_set and
 *  \ref fapi_pon_gpon_rerange_status_get.
 */
struct pon_gpon_rerange_cfg {
	/** Simulated LODS time.
	 *  The time is given in us, in a granularity of 125 us.
	 *  The valid range is from 3 (375 us) to 8000 (1 s).
	 *  The \ref fapi_pon_gpon_rerange_status_get function reports this
	 *  as 0.
	 */
	uint16_t lods_time;
	/** Activity indication.
	 *  This value shall be set to 0 when calling the
	 *  \ref fapi_pon_gpon_rerange_cfg_set function.
	 *  - 0: INACTIVE, The simulated LODS assertion is inactive.
	 *  - 1: ACTIVE, The simulated LODS assertion to start the re-ranging
	 *               process is active.
	 */
	int8_t active;
	/** Disable the PLOAM state machine and force state O1.x.
	 *  When enabled again, the PLOAM state machine commences normal
	 *  operation.
	 *  - 0: EN, The PLOAM state machine is enabled (normal operation).
	 *  - 1: DIS, The PLOAM state machine is stopped in state O1.x.
	 */
	int8_t psmdis;
	/** Disable the optical transmitter.
	 *  This disables the optical transmitter without direct PLOAM state
	 *  change.
	 *  - 0: EN, The optical transmitter is enabled (normal operation).
	 *  - 1: DIS, The optical transmitter is disabled.
	 */
	int8_t txdis;
};

/** Alarm limiter configuration.
 *  This structure is used to configure the firmware alarm limiter function.
 *  The function limits the number of consecutive firmware alarm events of
 *  the same type to avoid the software being overloaded with instable alarm
 *  information
 *  Used by \ref fapi_pon_alarm_limit_cfg_set
 *  and \ref fapi_pon_alarm_limit_cfg_get.
 */
struct pon_alarm_limit_cfg {
	/** Alarm threshold.
	 *  This is the number of consecutive alarms of the same type that are
	 *  allowed before the alarm suppression is taking effect.
	 *  The valid range is from 1 to 15.
	 */
	uint8_t alarm_thr;
	/** Alarm Observation Time.
	 *  This is the time for which alarms are suppressed,
	 *  given in units of ms.
	 *  The valid range is from 1 to 1000 ms.
	 */
	uint16_t alarm_ot;
};

/** Configuration parameters for PON time-out values, used by
 *  \ref fapi_pon_timeout_cfg_set and \ref fapi_pon_timeout_cfg_get.
 */
struct pon_timeout_cfg {
	/** Discovery Timer time-out value, given in multiples of 1 ms.
	 */
	uint32_t ploam_timeout_0;
	/** Ranging Timer time-out value, given in multiples of 1 ms.
	 *  Valid times are in the range from 1 ms to 2^16-1 ms.
	 *  This value is not applicable in G-PON operation mode.
	 */
	uint32_t ploam_timeout_1;
	/** LODS Timer time-out value, given in multiples of 1 ms.
	 *  Valid times are in the range from 1 ms to 2^16-1 ms.
	 */
	uint32_t ploam_timeout_2;
	/** LODS Timer with WLCP time-out value, given in multiples of 1 ms.
	 *  This value is ignored in G.984 operation mode and reported back
	 *  as 0. Valid times are in the range from 1 ms to 2^16-1 ms.
	 *  This value is applicable in NG-PON2 operation mode only and
	 *  else shall be set to 0.
	 */
	uint32_t ploam_timeout_3;
	/** Downstream Tuning Timer time-out value, given in multiples of 1 ms.
	 *  This value is ignored in G.984 operation mode and reported back
	 *  as 0. Valid times are in the range from 1 ms to 2^16-1 ms.
	 *  This value is applicable in NG-PON2 operation mode only and
	 *  else shall be set to 0.
	 */
	uint32_t ploam_timeout_4;
	/** Upstream Tuning Timer time-out value, given in multiples of 1 ms.
	 *  This value is ignored in G.984 operation mode and reported back
	 *  as 0. Valid times are in the range from 1 ms to 2^16-1 ms.
	 *  This value is applicable in NG-PON2 operation mode only and
	 *  else shall be set to 0.
	 */
	uint32_t ploam_timeout_5;
	/** Lost ONU Time time-out value, given in multiples of 1 ms.
	 *  This value is ignored in G.984 operation mode and reported back
	 *  as 0. Valid times are in the range from 1 ms to 2^16-1 ms.
	 *  This value is applicable in NG-PON2 operation mode only and
	 *  else shall be set to 0.
	 */
	uint32_t ploam_timeout_6;
	/** Change Transmit Power Level time-out value, given in multiples
	 *  of 1 ms. Valid times are in the range from 1 ms to 2^16-1 ms.
	 */
	uint32_t ploam_timeout_cpl;
	/** Channel Partition Waiver Timer time-out value, given in multiples
	 *  of 1 ms. Valid times are in the range from 1 ms to 2^20-1 ms.
	 *  The special value of 0 disables the timeout (infinity).
	 *  This value is applicable in NG-PON2 operation mode only and
	 *  else shall be set to 0.
	 */
	uint32_t ploam_timeout_cpi;
	/** Timeout for the TProfileDwell timer.
	 *  Given in multiples of 1 ms.
	 *  Valid times are in the range from 1 ms to 2^16-1 ms.
	 *  ITU-T G.989 recommends a value of at least 10 s (10000 ms).
	 *  The special value of 0 disables the timeout.
	 *  This value is applicable in NG-PON2 operation mode only and
	 *  else shall be set to 0.
	 */
	uint32_t ploam_timeout_tpd;
};

/** Datapath configuration for PON.
 *  Used by \ref fapi_pon_dp_config_get.
 */
struct pon_dp_config {
	/** The datapath expects the FCS in rx packets */
	uint8_t with_rx_fcs;
	/** The datapath expects the FCS in tx packets */
	uint8_t with_tx_fcs;
	/** The datapath expects no timestamp in rx packets */
	uint8_t without_timestamp;
};

/** Structure to handle the LODS specific counters.
 *  Used by \ref fapi_pon_xgspon_lods_counters_get.
 *  This set of counters can be used to implement the OMCI managed entity
 *  "XG-PON and Enhanced TC performance monitoring history data".
 */
 struct pon_xgspon_lods_counters {
	/** Number of LODS events */
	uint64_t lods_events_all;
	/** Number of LODS events restored */
	uint64_t lods_restored_oper;
	/** Reserved. This counter isn't used in non-NGPON2 operation mode. */
	uint64_t lods_restored_prot;
	/** Reserved. This counter isn't used in non-NGPON2 operation mode. */
	uint64_t lods_restored_disc;
	/** Number of LODS events causing reactivation */
	uint64_t lods_reactivation;
	/** Reserved. This counter isn't used in non-NGPON2 operation mode. */
	uint64_t lods_reactivation_prot;
	/** Reserved. This counter isn't used in non-NGPON2 operation mode. */
	uint64_t lods_reactivation_disc;
};

/* GPON-specific PON library function definitions */
/* ============================================== */

/**
 *	Function to configure the PON IP upon start-up in GPON mode.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined by \ref pon_gpon_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_gpon_cfg_set(struct pon_ctx *ctx,
					      const struct pon_gpon_cfg *param);

/**
 *	Function to read the PON IP GPON start-up configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *		by \ref pon_gpon_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_gpon_cfg_get(struct pon_ctx *ctx,
					      struct pon_gpon_cfg *param);

/**
 *	Function to configure the PON IP credentials in ITU PON modes.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined by \ref pon_cred_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_cred_set(struct pon_ctx *ctx,
					  const struct pon_cred_cfg *param);

/**
 *	Function to read the PON IP credentials in ITU PON modes.
 *	This function is available only if debug mode is enabled,
 *	otherwise empty values will be returned.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *		by \ref pon_cred_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_cred_get(struct pon_ctx *ctx,
					  struct pon_cred_cfg *param);

/**
 *	Function to change the priority queue scale factor configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined by
 *		\ref pon_pqsf_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *      - PON_STATUS_OK: If successful
 *      - Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_pqsf_cfg_set(struct pon_ctx *ctx,
					     const struct pon_pqsf_cfg *param);

/**
 *	Function to read the priority queue scale factor configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_pqsf_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_pqsf_cfg_get(struct pon_ctx *ctx,
					     struct pon_pqsf_cfg *param);

/**
 *	Function to configure the OMCI message encapsulation into
 *	Ethernet packets. This is needed to handle OMCI messages in software
 *	as regular Ethernet packets while on the PON these are exchanged in
 *	raw GEM frames. The configuration shall match the expectations of the
 *	OMCI software stack and the network interface configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_omci_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *	\remarks This function is currently not supported.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_omci_cfg_set(struct pon_ctx *ctx,
					      const struct pon_omci_cfg *param);

/**
 *	Function to read the configuration for the OMCI message encapsulation
 *	into Ethernet packets.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_omci_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_omci_cfg_get(struct pon_ctx *ctx,
					      struct pon_omci_cfg *param);

/**
 *	Function to read the GEM/XGEM port properties
 *	for a given GEM port index.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] gpix GEM port index to get configuration for
 *	\param[out] param_out Pointer to a structure as defined
 *		by \ref pon_gem_port.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_gem_port_index_get(struct pon_ctx *ctx,
				    uint8_t gpix,
				    struct pon_gem_port *param_out);

/**
 *	Function to read the GEM/XGEM port properties for a given GEM port ID.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] gem_port_id GEM/XGEM port ID to get information for.
 *		The valid ranges are:
 *		- 0 to 4095 for GPON (lower 12 bit)
 *		- 0 to 65536 for XG-PON, XGS-PON, and NG-PON2 (lower 16 bit)
 *	\param[out] param_out Pointer to a structure as defined
 *		by \ref pon_gem_port.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_gem_port_id_get(struct pon_ctx *ctx,
					 uint16_t gem_port_id,
					 struct pon_gem_port *param_out);

/**
 *	Function to read the allocation index of an upstream
 *	time slot allocation (T-CONT) for a given allocation ID.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] alloc_id The allocation ID for which the index
 *	shall be returned.
 *	\param[out] param_out Pointer to a structure as defined by
 *	\ref pon_allocation_index to return the requested allocation index.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: Allocation ID successfully configured.
 *	- PON_STATUS_VALUE_RANGE_ERR: Allocation ID is invalid.
 *	- PON_STATUS_ALLOC_ID_NOT_EXISTS_ERR: The allocation ID does not exist.
 */
enum fapi_pon_errorcode fapi_pon_alloc_id_get(struct pon_ctx *ctx,
					      uint16_t alloc_id,
					struct pon_allocation_index *param_out);

/**
 *	Function to read the overall GPON status.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined by
 *	\ref pon_gpon_status.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_gpon_status_get(struct pon_ctx *ctx,
						 struct pon_gpon_status *param);

/**
 *	Function to read the GPON alarm status.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	All alarms that have been active at least once since the last access to
 *	this function are indicated. If calling this function twice, first
 *	shows all alarms that have been active while the second
 *	indicates only those alarms which are still active.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] pon_alarm_id Alarm type identification.
 *	\param[out] param Pointer to a structure as defined by
 *	\ref pon_alarm_status.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
	fapi_pon_alarm_status_get(struct pon_ctx *ctx,
				  uint16_t pon_alarm_id,
				  struct pon_alarm_status *param);

/**
 *	Function to configure the firmware alarm limiter function.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2) and AON mode (Active Ethernet).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined by
 *		\ref pon_alarm_limit_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- MODE_ERROR: If mode is not equal to MODE_ITU_PON or MODE_AON
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_alarm_limit_cfg_set(struct pon_ctx *ctx,
			     const struct pon_alarm_limit_cfg *param);

/**
 *	Function to read the firmware alarm limiter function configuration.
 *      This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2) and AON mode (Active Ethernet).
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *		by \ref pon_alarm_limit_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *      - MODE_ERROR: If mode is not equal to MODE_ITU_PON or MODE_AON
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_alarm_limit_cfg_get(struct pon_ctx *ctx,
			     struct pon_alarm_limit_cfg *param);

/**
 *	Function to read GEM/XGEM port related counters.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] gem_port_id GEM/XGEM port ID to get counters for.
 *		The valid ranges are:
 *		- 0 to 4095 for GPON (lower 12 bit)
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
enum fapi_pon_errorcode fapi_pon_gem_port_counters_get(struct pon_ctx *ctx,
					uint16_t gem_port_id,
					struct pon_gem_port_counters *param);

/**
 *	Function to read the GEM/XGEM related counters.
 *	The counters are accumulated across all active GEM ports.
 *	For individual counters per GEM port see
 *	\ref fapi_pon_gem_port_counters_get.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
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
enum fapi_pon_errorcode fapi_pon_gem_all_counters_get(struct pon_ctx *ctx,
					struct pon_gem_port_counters *param);

/**
 *	Function to read GPON TC (GTC) counters.
 *	This function is applicable only in GPON operation mode.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_gtc_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_gtc_counters_get(struct pon_ctx *ctx,
					struct pon_gtc_counters *param);

/**
 *	Function to read XGTC counters.
 *	This function is applicable in XG-PON, XGS-PON, and NG-PON2
 *	operation mode.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_xgtc_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_xgtc_counters_get(struct pon_ctx *ctx,
					struct pon_xgtc_counters *param);

/**
 *	Function to read FEC counters.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
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
enum fapi_pon_errorcode fapi_pon_fec_counters_get(struct pon_ctx *ctx,
					struct pon_fec_counters *param);

/**
 *	Function to read T-CONT/allocation-specific counters,
 *	selected by the allocation index.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] alloc_index Selects the allocation by its index.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_alloc_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_alloc_counters_get(struct pon_ctx *ctx,
					uint8_t alloc_index,
					struct pon_alloc_counters *param);

/**
 *	Function to read PLOAM downstream message counters.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_ploam_ds_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_ploam_ds_counters_get(struct pon_ctx *ctx,
					struct pon_ploam_ds_counters *param);

/**
 *	Function to read PLOAM upstream message counters.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_ploam_us_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_ploam_us_counters_get(struct pon_ctx *ctx,
					struct pon_ploam_us_counters *param);

/**
 *	Function to read GEM port related Ethernet receive counters.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open,
 *	or NULL to implicitly create one for this function call.
 *	\param[in] gem_port_id GEM port ID to get counters for.
 *	The valid ranges are:
 *	- 0 to 4095 for GPON (lower 12 bit)
 *	- 0 to 65535 for XG-PON, XGS-PON, and NG-PON2 (lower 16 bit)
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_eth_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_eth_rx_counters_get(struct pon_ctx *ctx,
		    uint32_t gem_port_id,
		    struct pon_eth_counters *param);

/**
 *	Function to read GEM port related Ethernet transmit counters.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open,
 *	or NULL to implicitly create one for this function call.
 *	\param[in] gem_port_id GEM port ID to get counters for.
 *	The valid ranges are:
 *	- 0 to 4095 for GPON (lower 12 bit)
 *	- 0 to 65535 for XG-PON, XGS-PON, and NG-PON2 (lower 16 bit)
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_eth_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_eth_tx_counters_get(struct pon_ctx *ctx,
		    uint32_t gem_port_id,
		    struct pon_eth_counters *param);

/**
 *	Function to enable or disable individual PLOAM downstream messages
 *	to be forwarded to the software.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open,
 *		or NULL to implicitly create one for this function call.
 *	\param[in] param Pointer to a structure as defined by
 *		\ref pon_ploamd_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
	fapi_pon_ploamd_cfg_set(struct pon_ctx *ctx,
				const struct pon_ploamd_cfg *param);

/**
 *	Function to read the PLOAM downstream message configuration.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open,
 *		or NULL to implicitly create one for this function call.
 *	\param[out] param Pointer to a structure as defined
 *		by \ref pon_ploamd_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_ploamd_cfg_get(struct pon_ctx *ctx,
						struct pon_ploamd_cfg *param);

/**
 *	Function to configure GTC/XGTX-specific settings.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_gtc_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_gtc_cfg_set(struct pon_ctx *ctx,
					     const struct pon_gtc_cfg *param);

/**
 *	Function to read GTC/XGTC-specific settings.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_gtc_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_gtc_cfg_get(struct pon_ctx *ctx,
					     struct pon_gtc_cfg *param);

/**
 *	Function to read back the GEM/XGEM port T-CONT allocation setting.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] gem_port_id GEM/XGEM port ID value.
 *		The valid ranges are:
 *		- 0 to 4095 for GPON (lower 12 bit)
 *		- 0 to 65536 for XG-PON, XGS-PON, and NG-PON2 (lower 16 bit)
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_allocation_id.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_gem_port_alloc_get(struct pon_ctx *ctx,
					uint16_t gem_port_id,
					struct pon_allocation_id *param);

/**
 *	Function to read back all the GEM/XGEM ports which are configured to
 *	a single T-CONT allocation.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] alloc_id Allocation ID value.
 *	\param[in,out] gem_ports_num Size of buffer used to read GEM ports.
 *	\param[in,out] gem_ports Buffer used to read GEM/XGEM ports.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_alloc_gem_port_get(struct pon_ctx *ctx,
					uint16_t alloc_id,
					uint32_t *gem_ports_num,
					uint32_t *gem_ports);

/**
 *	Function to send the pre-shared key and the encryption mode
 *	for mutual authentication to the firmware.
 *	This function is applicable to all 10G ITU PON standards
 *	(XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_enc_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_auth_enc_cfg_set(struct pon_ctx *ctx,
				const struct pon_enc_cfg *param);

/**
 *	Function to get the pre-shared key and the encryption mode
 *	for mutual authentication from the firmware.
 *	This function is applicable to all 10G ITU PON standards
 *	(XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_enc_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_auth_enc_cfg_get(struct pon_ctx *ctx,
				struct pon_enc_cfg *param);

/**
 *	Function to send the OLT random challenge table
 *	for mutual authentication to the firmware.
 *	This function is applicable to all 10G ITU PON standards
 *	(XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_generic_auth_table.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode fapi_pon_auth_olt_challenge_set(struct pon_ctx *ctx,
				const struct pon_generic_auth_table *param);
#endif

/**
 *	Function to send the OLT mutual authentication result table
 *	to the firmware.
 *	This function is applicable to all 10G ITU PON standards
 *	(XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_generic_auth_table.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode fapi_pon_auth_olt_result_set(struct pon_ctx *ctx,
				const struct pon_generic_auth_table *param);
#endif

/**
 *	Function to read the hash value of the Master Session Key (MSK)
 *	from the firmware.
 *	This function is applicable to all 10G ITU PON standards
 *	(XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_onu_msk_hash.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode fapi_pon_auth_onu_msk_hash_get(struct pon_ctx *ctx,
						struct pon_onu_msk_hash *param);
#endif

/**
 *	Function to send the broadcast encryption key value to the firmware.
 *	The broadcast key itself is encrypted by using the Key Encryption Key
 *	(KEK).
 *	This function is applicable to 10G ITU PON standards
 *	(XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_onu_bc_key.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode fapi_pon_auth_onu_bc_key_set(struct pon_ctx *ctx,
					const struct pon_onu_bc_key *param);
#endif

/**
 *	Set the Time of Day to be synchronous with the GPON/XGPON TC sublayer.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_gpon_tod_sync.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_gpon_tod_sync_set(struct pon_ctx *ctx,
					const struct pon_gpon_tod_sync *param);

/**
 *	Read back the latest GPON/XGPON Time of Day synchronization parameters.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_gpon_tod_sync.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 *
 *	- All values are returned as 0 until the first synchronization to the
 *	  configured superframe has been achieved by the hardware.
 *	- After the hardware has been synchronized, the function reports back
 *	  the actual values (not the configured values).
 */
enum fapi_pon_errorcode fapi_pon_gpon_tod_sync_get(struct pon_ctx *ctx,
					struct pon_gpon_tod_sync *param);

/**
 *	Function to read the OMCI integrity key.
 *	This function is applicable to all 10G ITU PON standards
 *	(XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_omci_ik.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_omci_ik_get(struct pon_ctx *ctx,
					     struct pon_omci_ik *param);

/**
 *	Set the Power Saving State Machine (PSM) configuration.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined by \ref pon_psm_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- 0: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_psm_cfg_set(struct pon_ctx *ctx,
					     const struct pon_psm_cfg *param);

/**
 *	Read the Power Saving State Machine (PSM) configuration back.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined by \ref pon_psm_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- 0: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_psm_cfg_get(struct pon_ctx *ctx,
					     struct pon_psm_cfg *param);

/**
 *	Function to read the power save mode counters.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *		by \ref pon_psm_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_psm_counters_get(struct pon_ctx *ctx, struct pon_psm_counters *param);

/**
 *	Function to read the Power Saving State Machine (PSM) state time.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to \ref pon_psm_fsm_time.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- 0: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_psm_time_get(struct pon_ctx *ctx,
					      struct pon_psm_fsm_time *param);

/**
 *	Enable/activate the Power Saving State Machine (PSM).
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- 0: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_psm_enable(struct pon_ctx *ctx);

/**
 *	Disable/deactivate the Power Saving State Machine (PSM).
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- 0: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_psm_disable(struct pon_ctx *ctx);

/**
 *	Read the Power Saving State Machine (PSM) state.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *
 *	\param[out] param Pointer to a structure as defined
 *		by \ref pon_psm_state.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- 0: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_psm_state_get(struct pon_ctx *ctx,
					       struct pon_psm_state *param);

/**
 *	Interoperability option configuration.
 *	Some OLT devices require special non-standard handling. To enable
 *	interoperability with such systems, several dedicated interoperability
 *	modes can be activated.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *		by \ref pon_iop_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_iop_cfg_set(struct pon_ctx *ctx,
					     const struct pon_iop_cfg *param);

/**
 *	Read back the interoperability option configuration.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *		by \ref pon_iop_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_iop_cfg_get(struct pon_ctx *ctx,
					     struct pon_iop_cfg *param);

/**
 *	Enable PLOAM message logging.
 *	This is a debug function to enable the logging of all PLOAM upstream and
 *	downstream messages. PLOAM message logging can be disabled by calling
 *	\ref fapi_pon_ploam_log_disable.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	By default, PLOAM message logging is disabled.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_ploam_log_enable(struct pon_ctx *ctx);

/**
 *	Disable PLOAM message logging.
 *	This is function disables the logging of all PLOAM upstream and
 *	downstream messages. PLOAM message logging can be enabled by calling
 *	\ref fapi_pon_ploam_log_enable.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	By default, PLOAM message logging is disabled.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_ploam_log_disable(struct pon_ctx *ctx);

/**
 *	Read GPON PLOAM state information.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_ploam_state.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_ploam_state_get(struct pon_ctx *ctx,
						struct pon_ploam_state *param);

/**
 *	Function to configure the PON IP to perform re-ranging.
 *	The configuration defines the time for a simulated LODS event and
 *	triggers the event. During the time of this event, this function
 *	must not be called again.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined by
 *		   \ref pon_gpon_rerange_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- PON_STATUS_FW_NACK: If the function is called while still active.
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_gpon_rerange_cfg_set(struct pon_ctx *ctx,
			      const struct pon_gpon_rerange_cfg *param);

/**
 *	Function to read the PON IP re-ranging status information.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *		by \ref pon_gpon_rerange_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_gpon_rerange_status_get(struct pon_ctx *ctx,
				 struct pon_gpon_rerange_cfg *param);

/**
 *	Function to configure the PON time-out values.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined by
 *		   \ref pon_timeout_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_timeout_cfg_set(struct pon_ctx *ctx,
			 const struct pon_timeout_cfg *param);

/**
 *	Function to read the PON time-out configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *		by \ref pon_timeout_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_timeout_cfg_get(struct pon_ctx *ctx,
			 struct pon_timeout_cfg *param);

/**
 *	Function to read the PON datapath configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *		by \ref pon_dp_config.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_dp_config_get(struct pon_ctx *ctx,
		       struct pon_dp_config *param);

/**
 *	Function to read lods counters.
 *	This function is applicable in XG-PON, XGS-PON operation mode.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_xgspon_lods_counters.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_xgspon_lods_counters_get(struct pon_ctx *ctx,
			struct pon_xgspon_lods_counters *param);
/*! @} */ /* End of GPON functions */

/*! @} */ /* End of PON library definitions */

#endif /* _FAPI_PON_GPON_H_ */
