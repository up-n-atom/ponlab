/******************************************************************************
 *
 * Copyright (c) 2020 - 2026 MaxLinear, Inc.
 * Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter.h
 *
 * This is the PON Adapter header file, defining the API functions
 * and data structures for an adaptation layer between a higher layer
 * (such as the OMCI stack) and some lower layers
 * (such as OMCI API or PON FAPI).
 */

#ifndef _PON_ADAPTER_H_
#define _PON_ADAPTER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "pon_adapter_base.h"
#include "pon_adapter_debug.h"
#include "pon_adapter_errno.h"

/** \defgroup PON_ADAPTER PON Adaptation Layer Global Functions
 *
 * This adaptation layer provides the connection between the
 * hardware-independent management stack implementation and the underlying
 * hardware-specific implementation.
 * The target management stacks are OMCI and SIEPON.
 *
 * \remark The current implementation supports only OMCI.
 *
 *  @{
 */

/** PON-Adapter Interface (PA IF) version - Interface revision */
#define PA_IF_VERSION_REVISION		3
/** PA IF version - feature
 * Feature enhancements which are backwards compatible to older OMCI-daemon
 * binaries of the same revision
 */
#define PA_IF_VERSION_FEATURE		0
/** PA IF version, build number - step */
#define PA_IF_VERSION_STEP		0

/** PA IF version as number */
#define PA_IF_VER_NUMBER \
	((PA_IF_VERSION_REVISION << 16) | \
	(PA_IF_VERSION_FEATURE << 8) | \
	(PA_IF_VERSION_STEP))

#define GET_PA_IF_REVISION(version) (((version) >> 16) & (0xFF))
#define GET_PA_IF_FEATURE(version) (((version) >> 8) & (0xFF))
#define GET_PA_IF_STEP(version) (((version) >> 0) & (0xFF))

/** PA IF 1st version is used for backwards-compatibility checks */
#define PA_IF_VERSION_1ST_REVISION	1	/** Interface revision */
#define PA_IF_VERSION_1ST_FEATURE	0	/** feature */
#define PA_IF_VERSION_1ST_STEP		0	/** build number - step */
/** PA IF 1st version as number */
#define PA_IF_1ST_VER_NUMBER \
	((PA_IF_VERSION_1ST_REVISION << 16) | \
	(PA_IF_VERSION_1ST_FEATURE << 8) | \
	(PA_IF_VERSION_1ST_STEP))
/** ========================================================================
 * PON adapter interface version checks
 * =========================================================================
 */
/**
 * Version Check - equal with the given version
 */
#define PA_IF_VERSION_CHECK_EQ(version) \
	((GET_PA_IF_REVISION(version) == (uint32_t)PA_IF_VERSION_REVISION) && \
	(GET_PA_IF_FEATURE(version) == (uint32_t)PA_IF_VERSION_FEATURE) && \
	(GET_PA_IF_STEP(version) == (uint32_t)PA_IF_VERSION_STEP))

/**
 * Compatibility Check
 * verifies whether PA Interface versions of omci-daemon (as used during
 * compilation of the binary) and the lower layer modules are compatible
 * Decision criteria:
 * 1. revision number must match (mismatch indicates major deltas)
 * 2. feature number of the omci-daemon must be equal or lower than lower
 *    layer module version
 * If the versions are not identical, but compatible the interface is still
 * operable. In this case
 * - all features of the omci-daemon are supported, but
 * - feature enhancement of the lower layer modules may not be usable, if the
 *   used version of the omci-daemon does not support these features
 */
#define PA_IF_VERSION_CHECK_COMPATIBLE(version) \
	((GET_PA_IF_REVISION(version) == (uint32_t)PA_IF_VERSION_REVISION) && \
	(GET_PA_IF_FEATURE(version) <= (uint32_t)PA_IF_VERSION_FEATURE))

/* Macro to check if the given params form a valid pointer chain */
#define PA_EXISTS1(ptr) \
	(ptr)
#define PA_EXISTS2(ptr, e1) \
	(PA_EXISTS1(ptr) && ((ptr)->e1))
#define PA_EXISTS3(ptr, e1, e2) \
	(PA_EXISTS2(ptr, e1) && ((ptr)->e1->e2))
#define PA_EXISTS4(ptr, e1, e2, e3) \
	(PA_EXISTS3(ptr, e1, e2) && ((ptr)->e1->e2->e3))
#define PA_EXISTS5(ptr, e1, e2, e3, e4) \
	(PA_EXISTS4(ptr, e1, e2, e3) && ((ptr)->e1->e2->e3->e4))

#define EVAL(x) x
#define SELECT_PA_EXISTS(arg_1, arg_2, arg_3, arg_4, arg_5, N, ...) N
/** This macro provides syntactic sugar for checking if certain functions
 *  within pa_ops and *_ops structures are implemented by lower layer.
 *  It selects PA_EXISTS1-5 by number of given arguments.
 */
#define PA_EXISTS(...) EVAL(SELECT_PA_EXISTS(\
	__VA_ARGS__, PA_EXISTS5, PA_EXISTS4, PA_EXISTS3,\
	PA_EXISTS2, PA_EXISTS1)(__VA_ARGS__))

/** Length definition of the OMCI integrity key (OMCI-IK). */
#define OMCI_IK_LEN 16

/** This generic structure contains the OMCI integrity key (OMCI-IK). */
struct pa_omci_ik {
	/** OMCI-IK value */
	uint8_t key[OMCI_IK_LEN];
};

/** PON Adapter operational state */
enum pa_oper_state {
	/** Operational state is enabled */
	PON_ADAPTER_OPER_STATE_ENABLED = 0,
	/** Operational state is disabled */
	PON_ADAPTER_OPER_STATE_DISABLED = 1
};

/** PON Adapter system operations */
struct pa_system_ops;
/** PON Adapter integrity check operations */
struct pa_integrity_ops;
/** PON Adapter common OMCI MSG operations */
struct pa_msg_ops;
/** PON Adapter OMCI MIB operations */
struct pa_omci_mib_ops;
/** PON Adapter OMCI Managed Entity operations */
struct pa_omci_me_ops;
/** PON Adapter OMCI Multicast Control operations */
struct pa_omci_mcc_ops;
/** PON Adapter Meter operations */
struct pa_omci_meter_ops;
/** PON Adapter EPON ops */
struct pa_epon_ops;

/** System lower layer debug operations */
struct pa_ll_dbg_lvl_ops {
	/** Set the lower layer debug level.
	 *
	 * \param[in] level		Lower layer debug level
	 */
	void (*set)(const uint8_t level);

	/** Get the lower layer debug level.
	 *
	 * \return Lower layer debug level
	 */
	uint8_t (*get)(void);
};

/** System capabilities
 * Each module only needs to implement the functions for the parts it is
 * handling.
 * If no module has implemented it in a system, a value of 0 is assumed.
 */
struct pa_system_cap_ops {
	/** Get maximum number of supported Bridge Ports */
	enum pon_adapter_errno(*get_bridgeports)(void *ll_handle,
						  uint32_t *bridgeports_num);
	/** Get maximum number of supported UNI (LAN) ports */
	enum pon_adapter_errno(*get_lanports)(void *ll_handle,
						uint32_t *lanports_num);
	/** Get an 8 byte size Serial Number */
	enum pon_adapter_errno (*get_serial_number)(void *ll_handle,
						    uint8_t *serial_number);
	/** Get maximum number of supported GEM ports */
	enum pon_adapter_errno (*get_gemports)(void *ll_handle,
					       uint16_t *gemport_num);
};

/** PON Operation Modes */
enum pa_pon_op_mode {
	/** The PON operation mode is not known */
	PA_PON_MODE_UNKNOWN,
	/** GPON mode according to ITU-T G.984.3 */
	PA_PON_MODE_G984,
	/** XG-PON mode according to ITU-T G.987.3 */
	PA_PON_MODE_G987,
	/** NG-PON2 mode according to ITU-T G.989.3 */
	PA_PON_MODE_G989,
	/** XGS-PON mode according to ITU-T G.9807.1 */
	PA_PON_MODE_G9807,
	/** EPON mode according to IEEE 802.3 */
	PA_PON_MODE_IEEE_1GEPON,
	/** 10G/1G EPON mode according to IEEE 802.3 */
	PA_PON_MODE_IEEE_10_1GEPON,
	/** 10G EPON mode according to IEEE 802.3 (symmetrical) */
	PA_PON_MODE_IEEE_10GEPON
};

/** Loss of signal */
#define PA_PON_ALARM_STATIC_LOS		0x0000
/** Loss of frame (G984.3 only) */
#define PA_PON_ALARM_STATIC_LOF		0x0001
/** Loss of downstream synchronization (GTC, XGTC, TWDM, IEEE) */
#define PA_PON_ALARM_STATIC_LODS	0x0002
/** Signal failed */
#define PA_PON_ALARM_STATIC_SF		0x0003
/** Signal degraded */
#define PA_PON_ALARM_STATIC_SD		0x0004
/** Loss of GEM channel delineation */
#define PA_PON_ALARM_STATIC_LCDG	0x0005
/** Transmitter failure */
#define PA_PON_ALARM_STATIC_TF		0x0006
/** Start-up failure */
#define PA_PON_ALARM_STATIC_SUF		0x0007
/** Link mismatch (GTC only) */
#define PA_PON_ALARM_STATIC_MIS		0x0008
/** Emergency stop state entered (PLOAM state O7/07.1/07.2) */
#define PA_PON_ALARM_STATIC_O7		0x0009
/** Clock alarm reported by SoC */
#define PA_PON_ALARM_STATIC_CLK		0x000B
/** Unknown PLOAM message */
#define PA_PON_ALARM_EDGE_UPM		0x000F
/** Deactivate ONU message received (GTC, XGTC) */
#define PA_PON_ALARM_EDGE_DACT		0x0010
/** Physical equipment error (GTC only) */
#define PA_PON_ALARM_STATIC_PEE		0x0011
/** Remote defect indication (GTC only) */
#define PA_PON_ALARM_EDGE_RDI		0x0012
/** Rogue ONU detected */
#define PA_PON_ALARM_EDGE_ROG		0x0013

/** System status functions */
struct pa_system_status_ops {
	/** Get PON Operational Mode
	 *
	 * \param[in]   ll_handle	Lower layer context pointer
	 * \param[out]  mode		PON Operational Mode
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*get_pon_op_mode)(void *ll_handle,
						   enum pa_pon_op_mode *mode);

	/** Callback to inform about PON FW alarm
	 *
	 * \param[in]   ll_handle	Lower layer context pointer
	 * \param[out]  alarm_id	Alarm ID
	 *				 - 0: OFF, Alarm is inactive.
	 *				 - 1: ON, Alarm is active.
	 * \param[out]  status		Alarm status
	 */
	enum pon_adapter_errno (*get_alarm_status)(void *ll_handle,
						   uint16_t alarm_id,
						   uint8_t *status);

	/** Callback to get PON state.
	 *
	 * \param[in]   ll_handle	Lower layer context pointer
	 * \param[out]  enabled		PON state of the system.
	 *				 - 0: OFF, PLOAM state machine and the
	 *				      optical transmitter disabled
	 *				 - 1: ON, PLOAM state machine and the
	 *					  optical transmitter enabled
	 */
	enum pon_adapter_errno (*get_pon_status)(void *ll_handle,
						 bool *enabled);

	/** Callback to change PON state.
	 *
	 * \param[in]   ll_handle	Lower layer context pointer
	 * \param[in]   enable		PON state of the system.
	 *				 - 0: OFF, PLOAM state machine and the
	 *				      optical transmitter disabled
	 *				 - 1: ON, PLOAM state machine and the
	 *					  optical transmitter enabled
	 */
	enum pon_adapter_errno (*set_pon_status)(void *ll_handle, bool enable);
};

/** This structure contains the entire interface of the PON Adapter.
 * It can hold all interface functions but may get populated just partly
 * dependent on the module registered.
*/
struct pa_ops {
	/** PON Adapter system operations */
	const struct pa_system_ops *system_ops;
	/** PON Adapter system capabilities operations */
	const struct pa_system_cap_ops *sys_cap_ops;
	/** PON Adapter system status operations */
	const struct pa_system_status_ops *sys_sts_ops;
	/** PON Adapter integrity operations */
	const struct pa_integrity_ops *integrity_ops;
	/** PON Adapter OMCI msg operations */
	const struct pa_msg_ops *msg_ops;
	/** PON Adapter OMCI MIB operations */
	const struct pa_omci_mib_ops *omci_mib_ops;
	/** PON Adapter OMCI Managed Entity operations */
	const struct pa_omci_me_ops *omci_me_ops;
	/** PON Adapter OMCI Multicast Control operations */
	const struct pa_omci_mcc_ops *omci_mcc_ops;
	/** PON Adapter Meter operations */
	const struct pa_omci_meter_ops *omci_meter_ops;
	/** PON Adapter EPON operations */
	const struct pa_epon_ops *epon_ops;
	/** PON Adapter VLAN Flow operations */
	const struct pa_vlan_flow_ops *vlan_flow_ops;
	/** PON Adapter debug level operations */
	const struct pa_ll_dbg_lvl_ops *dbg_lvl_ops;
	/** PON Adapter optical interface operations */
	const struct pa_optic_ops *optic_ops;
};

/** Prepare function definition for lower layer operations structure
 * registration.
 * \param[in] hl_handle_legacy Pointer must be set to NULL.
 * \param[out] pa_ops PON Adapter operation.
 * \param[out] ll_handle Low-level handle.
 * \param[in] hl_handle High-level handle.
 * \param[in] if_version PON-Adapter interface version used by calling function.
 */
enum pon_adapter_errno pa_ll_register_ops(void *hl_handle_legacy,
					  const struct pa_ops *pa_ops,
					  void **ll_handle,
					  void *hl_handle,
					  uint32_t if_version);

/**
 *	Set the PON Adapter debug level.
 *
 *	\param[in] level	PON Adapter debug level
 */
void pa_dbg_level_set(const enum pon_adapter_dbg level);

/**
 *	Get the PON Adapter debug level.
 *
 *	\return PON Adapter debug level
 */
enum pon_adapter_dbg pa_dbg_level_get(void);

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif /* _PON_ADAPTER_H_ */
