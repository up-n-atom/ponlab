/******************************************************************************
 *
 *  Copyright (c) 2022 MaxLinear, Inc.
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * Physical Path Termination Point POTS UNI.
 */

#ifndef _pon_adapter_me_pptp_pots_uni
#define _pon_adapter_me_pptp_pots_uni

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME

   @{
*/

/** \defgroup PON_ADAPTER_ME_PPTP_POTS_UNI PPTP POTS UNI

    This Managed Entity represents a POTS UNI in the ONU, where a physical path
    terminates and physical path level functions (analogue telephony) are
    performed.

    The ONU automatically creates an instance of this Managed Entity per port:
       *  When the ONU has POTS ports built into its factory configuration
       *  When a cardholder is provisioned to expect a circuit pack of POTS type
       *  When a cardholder provisioned for plug and play is equipped with a
	  circuit pack of POTS type. Note that the installation of a plug and
	  play card may indicate the presence of POTS ports via equipment ID as
	  well as type, and indeed may cause the ONU to instantiate a port
	  mapping package that specifies POTS ports.

    The ONU automatically deletes instances of this Managed Entity when a
    cardholder is neither provisioned to expect a POTS circuit pack, nor is
    equipped with a POTS circuit pack.

   @{
*/

/** PPTP POTS UNI update structure */
struct pa_pptp_pots_uni_data {
	/** Impedance */
	uint8_t impedance;
	/** Transmission Path */
	uint8_t transmission_path;
	/** Receive Gain */
	uint8_t rx_gain;
	/** Transmit Gain */
	uint8_t tx_gain;
	/** POTS Holdover Time */
	uint16_t pots_holdover_time;
	/** Nominal feed voltage */
	uint8_t nominal_feed_voltage;
	/** Loss of softswitch */
	bool loss_of_softswitch;
};

/** PPTP POTS UNI operations structure */
struct pa_pptp_pots_uni_ops {
	/** Lock LAN port
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*lock)(void *ll_handle,
				       uint16_t me_id);

	/** Unlock LAN port
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*unlock)(void *ll_handle,
					 uint16_t me_id);

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
	enum pon_adapter_errno (*create)(void *ll_handle,
					 uint16_t me_id);

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
	enum pon_adapter_errno (*destroy)(void *ll_handle,
					  uint16_t me_id);

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
	enum pon_adapter_errno (*update)(void *ll_handle,
					 uint16_t me_id,
					 const struct pa_pptp_pots_uni_data
					 *update_data);

	/** Get hook state
	 *
	 * \param[in]  ll_handle      Lower layer context pointer
	 * \param[in]  me_id          Managed Entity identifier
	 * \param[out] hook_state     Returns hook state
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*hook_state_get)(void *ll_handle,
						 uint16_t me_id,
						 uint8_t *hook_state);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
