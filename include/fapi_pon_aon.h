/******************************************************************************
 *
 *  Copyright (c) 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
******************************************************************************/

/**
 *  \file
 *  This is the PON library header file, defining the API functions
 *  and data structures for AON (active Ethernet) operation mode.
 */

#ifndef _FAPI_PON_AON_H_
#define _FAPI_PON_AON_H_

/** \addtogroup PON_FAPI_REFERENCE
 *   @{
 */

/** \defgroup PON_FAPI_REFERENCE_AON PON Library AON Functions
 *   This chapter describes the functional API interface to access
 *   functions that are used for AON system operation.
 *  @{
 */

/* AON-specific enumerations */
/* ========================== */
/* Add AON-specific enumerations here */

/* AON structure definitions */
/* ========================== */
/* Add AON-specific structures here.
 * \remark AON functions are available only if the PON IP is not initialized.
 */

/** AON configuration data.
 *  Used by \ref fapi_pon_aon_cfg_set and \ref fapi_pon_aon_cfg_get.
 */
struct pon_aon_cfg {
	/** Transmitter enable signal polarity.
	 *  - 0: NORMAL, Direct polarity, active high.
	 *  - 1: INVERTED, Inverted polarity, active low.
	 */
	uint32_t tx_en_pol;
};

/* PON library AON function definitions */
/* ====================================== */

/**
 *	Set the AON configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_aon_cfg.
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
fapi_pon_aon_cfg_set(struct pon_ctx *ctx, const struct pon_aon_cfg *param);
#endif

/**
 *	Read the AON configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_aon_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_aon_cfg_get(struct pon_ctx *ctx, struct pon_aon_cfg *param);

/**
 *	Enable optical transmission in AON operation mode.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open,
 *	or NULL to implicitly create one for this function call.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_aon_tx_enable(struct pon_ctx *ctx);

/**
 *	Disable optical transmission in AON operation mode.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open,
 *	or NULL to implicitly create one for this function call.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_aon_tx_disable(struct pon_ctx *ctx);

/* Add more AON functions here */

/*! @} */ /* End of PON library AON functions */

/*! @} */ /* End of PON library definitions */

#endif /* _FAPI_PON_AON_H_ */
