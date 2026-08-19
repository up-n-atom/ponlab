/******************************************************************************
 *
 *  Copyright (c) 2020 - 2026 MaxLinear, Inc.
 *  Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * Physical Path Termination Point Ethernet UNI.
 */

#ifndef _pon_adapter_me_pptp_ethernet_uni
#define _pon_adapter_me_pptp_ethernet_uni

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_PPTP_ETHERNET_UNI\
 *  Physical Path Termination Point Ethernet UNI
 *
 *  This Managed Entity represents the point at an Ethernet UNI where the
 *  physical path terminates and Ethernet physical level functions are
 *  performed.
 *
 *  The ONU automatically creates an instance of this Managed Entity per port:
 *  - When the ONU has Ethernet ports built into its factory configuration.
 *  - When a cardholder is provisioned to expect a circuit pack of Ethernet
 *    type.
 *  - When a cardholder provisioned for plug-and-play is equipped with a circuit
 *     pack of Ethernet type. Note that the installation of a plug-and-play card
 *     may indicate the presence of Ethernet ports via equipment ID as well as
 *     its type, and indeed may cause the ONU to instantiate a port mapping
 *     package that specifies Ethernet ports.
 *
 *  The ONU automatically deletes instances of this Managed Entity when a
 *  cardholder is neither provisioned to expect an Ethernet circuit pack, nor is
 *  it equipped with an Ethernet circuit pack.
 *
 *  @{
 */

/** Supported values of the auto detection configuration parameter */
enum pa_pptp_eth_uni_auto_detect_cfg {
	/** Rate = Auto, DUPLEX = Auto */
	PA_PPTPETHUNI_DETECT_AUTO = 0x00,
	/** Rate = 10 only, DUPLEX = FULL only */
	PA_PPTPETHUNI_DETECT_10_FULL = 0x01,
	/** Rate = 100 only, DUPLEX = FULL only */
	PA_PPTPETHUNI_DETECT_100_FULL = 0x02,
	/** Rate = 1000 only, DUPLEX = FULL only */
	PA_PPTPETHUNI_DETECT_1000_FULL = 0x03,
	/** Rate = Auto, DUPLEX = FULL only */
	PA_PPTPETHUNI_DETECT_AUTO_FULL = 0x04,
	/** Rate = 10000 only, DUPLEX = FULL only */
	PA_PPTPETHUNI_DETECT_10000_FULL = 0x05,
	/** Rate = 2500 only, DUPLEX = FULL only */
	PA_PPTPETHUNI_DETECT_2500_FULL = 0x06,
	/** Rate = 5000 only, DUPLEX = FULL only */
	PA_PPTPETHUNI_DETECT_5000_FULL = 0x07,
	/** Rate = 25000 only, DUPLEX = FULL only */
	PA_PPTPETHUNI_DETECT_25000_FULL = 0x08,
	/** Rate = 40000 only, DUPLEX = FULL only */
	PA_PPTPETHUNI_DETECT_40000_FULL = 0x09,
	/** Rate = 10 only, DUPLEX = AUTO */
	PA_PPTPETHUNI_DETECT_10_AUTO = 0x10,
	/** Rate = 10 only, DUPLEX = Half only */
	PA_PPTPETHUNI_DETECT_10_HALF = 0x11,
	/** Rate = 100 only, DUPLEX = Half only */
	PA_PPTPETHUNI_DETECT_100_HALF = 0x12,
	/** Rate = 1000 only, DUPLEX = Half only */
	PA_PPTPETHUNI_DETECT_1000_HALF = 0x13,
	/** Rate = Auto, DUPLEX = Half only */
	PA_PPTPETHUNI_DETECT_AUTO_HALF = 0x14,
	/** Rate = 1000 only, DUPLEX = Auto */
	PA_PPTPETHUNI_DETECT_1000_AUTO = 0x20,
	/** Rate = 100 only, DUPLEX = Auto */
	PA_PPTPETHUNI_DETECT_100_AUTO = 0x30,
};

/** Supported values of configuration status of the Ethernet UNI */
enum pa_pptp_eth_uni_cfg_ind {
	/** Rate = Auto, DUPLEX = Auto */
	PA_PPTPETHUNI_CFG_IND_AUTO = 0x00,
	/** Rate = 10 only, DUPLEX = FULL only */
	PA_PPTPETHUNI_CFG_IND_10_FULL = 0x01,
	/** Rate = 100 only, DUPLEX = FULL only */
	PA_PPTPETHUNI_CFG_IND_100_FULL = 0x02,
	/** Rate = 1000 only, DUPLEX = FULL only */
	PA_PPTPETHUNI_CFG_IND_1000_FULL = 0x03,
	/** Rate = 10000, DUPLEX = FULL only */
	PA_PPTPETHUNI_CFG_IND_10000_FULL = 0x04,
	/** Rate = 2500, DUPLEX = FULL only */
	PA_PPTPETHUNI_CFG_IND_2500_FULL = 0x05,
	/** Rate = 5000, DUPLEX = FULL only */
	PA_PPTPETHUNI_CFG_IND_5000_FULL = 0x06,
	/** Rate = 25000 only, DUPLEX = FULL only */
	PA_PPTPETHUNI_CFG_IND_25000_FULL = 0x07,
	/** Rate = 40000 only, DUPLEX = FULL only */
	PA_PPTPETHUNI_CFG_IND_40000_FULL = 0x08,
	/** Rate = 10 only, DUPLEX = Half only */
	PA_PPTPETHUNI_CFG_IND_10_HALF = 0x11,
	/** Rate = 100 only, DUPLEX = Half only */
	PA_PPTPETHUNI_CFG_IND_100_HALF = 0x12,
	/** Rate = 1000 only, DUPLEX = Half only */
	PA_PPTPETHUNI_CFG_IND_1000_HALF = 0x13,
};

enum pa_pptp_eth_uni_expected_type {
	/** Default value - auto negotiation */
	PA_PPTPETHUNI_EXPECTED_TYPE_NOLIM = 0,
	/** 10BASE-T Ethernet LAN IF */
	PA_PPTPETHUNI_EXPECTED_TYPE_10BASET = 22,
	/** 100BASE-T Ethernet LAN IF */
	PA_PPTPETHUNI_EXPECTED_TYPE_100BASET = 23,
	/** 10/100BASE-T Ethernet LAN IF */
	PA_PPTPETHUNI_EXPECTED_TYPE_10_100BASET = 24,
	/** 10/100/1000BASE-T Ethernet LAN IF */
	PA_PPTPETHUNI_EXPECTED_TYPE_10_100_1000BASET = 47,
	/** 10GBASE-T Ethernet LAN IF */
	PA_PPTPETHUNI_EXPECTED_TYPE_10G_ETH = 49,
	/** 2.5GBASE-T Ethernet LAN IF */
	PA_PPTPETHUNI_EXPECTED_TYPE_2_5G_ETH = 50,
	/** 5GBASE-T Ethernet LAN IF */
	PA_PPTPETHUNI_EXPECTED_TYPE_5G_ETH = 51,
	/** 25GBASE-T Ethernet LAN IF */
	PA_PPTPETHUNI_EXPECTED_TYPE_25G_ETH = 52,
	/** 40GBASE-T Ethernet LAN IF */
	PA_PPTPETHUNI_EXPECTED_TYPE_40G_ETH = 53,
	/** Multi-Rate-10GBASE-T Ethernet LAN IF */
	PA_PPTPETHUNI_EXPECTED_TYPE_10G_MULTI_ETH = 68,
};

/** Supported values of the Ethernet loopback configuration parameter */
enum pa_pptp_eth_uni_ethernet_loopback {
	/** No loopback */
	PA_PPTPETHUNI_LOOPBACK_DISABLED = 0,
	/** Loopback of downstream traffic after PHY transceiver */
	PA_PPTPETHUNI_LOOPBACK_ENABLED = 3
};

/** Supported values of the Bridged or IP ind parameter */
enum pa_bridge_or_router_cfg {
	/** Bridged */
	PA_BRIDGE,
	/** IP router */
	PA_ROUTER,
	/** Depends on the parent circuit pack */
	PA_PARENT_DEPEND
};

/** PPTP Ethernet UNI update structure */
struct pa_pptp_eth_uni_data {
	/** Expected type */
	uint8_t expected_type;
	/** Auto detection configuration */
	uint8_t auto_detect_cfg;
	/** Ethernet loopback configuration */
	uint8_t ethernet_loopback;
	/** Max frame size */
	uint16_t max_frame_size;
	/** DTE or DCE ind */
	uint8_t dte_dce_ind;
	/** Pause time */
	uint16_t pause_time;
	/** Bridged or IP ind */
	uint8_t bridge_or_router_cfg;
	/** PPPoE filter */
	uint8_t pppoe_filter;
	/** Power control */
	uint8_t power_control;
};

/** PPTP Ethernet UNI operations structure */
struct pa_pptp_eth_uni_ops {
	/** Lock LAN port
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*lock)(void *ll_handle, uint16_t me_id);

	/** Unlock LAN port
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*unlock)(void *ll_handle, uint16_t me_id);

	/** Check if the related LAN port is available
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 * \param[out] available           Pointer to availability flag
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*lan_is_available)(void *ll_handle,
		   uint16_t me_id, bool *available);

	/** Enable Managed Entity identifier mapping to driver index
	 *  and initialize corresponding driver structures
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*create)(void *ll_handle, uint16_t me_id);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle, uint16_t me_id);

	/** Update PPTP Ethernet UNI Managed Entity resources
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 * \param[in] update_data          PPTP Ethernet UNI update data
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*update)(void *ll_handle, uint16_t me_id,
					 const struct pa_pptp_eth_uni_data
					 *update_data);

	/** Retrieve current Configuration indication
	 *
	 * \param[in]  ll_handle      Lower layer context pointer
	 * \param[in]  me_id          Managed Entity identifier
	 * \param[out] cfg_ind        Configuration ind
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*conf_ind_get)(void *ll_handle,
					       uint16_t me_id,
					       uint8_t *cfg_ind);

	/** Retrieve current Operational State
	 *
	 * \param[in]  ll_handle      Lower layer context pointer
	 * \param[in]  me_id          Managed Entity identifier
	 * \param[out] oper_state     Operational State
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*oper_state_get)(void *ll_handle,
						 uint16_t me_id,
						 uint8_t *oper_state);

	/** Retrieve current Sensed Type
	 *
	 * \param[in]  ll_handle      Lower layer context pointer
	 * \param[in]  me_id          Managed Entity identifier
	 * \param[out] sensed_type    Sensed Type
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*sensed_type_get)(void *ll_handle,
						  uint16_t me_id,
						  uint8_t *sensed_type);

	/** Enable/disable
	 *
	 * \param[in]  ll_handle      Lower layer context pointer
	 * \param[in]  me_id          Managed Entity identifier
	 * \param[out] enable         Enable/disable
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*lan_port_enable)(void *ll_handle,
						  const uint16_t me_id,
						  const bool enable);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
