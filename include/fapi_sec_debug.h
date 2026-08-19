/******************************************************************************

  Copyright (C) 2022 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : fapi_sec_debug.h                                           *
 *     Project    : UGW                                                        *
 *     Description: secure debug fapi header file inclusions.			       *
 *                                                                             *
 ******************************************************************************/

/*! \file fapi_sec_debug.h
 \brief File contains secure debug APIs prototype
*/

#ifndef _SEC_DEBUG_FAPI__
#define _SEC_DEBUG_FAPI__

#include <secure_services.h>

/*SECDBG reply*/
typedef enum secdbg_error_code {
	SDBG_SUCCESS= 0,					/* Successful */
	SDBG_AUTH_INIT_ERROR,				/* Debug port authentication init error */
	SDBG_AUTH_VERIFY_ERROR,				/* Debug port authentication verify error */
	SDBG_INVALID_AUTH_INIT_REQUEST,		/* Debug port Auth Init request is invalid */
	SDBG_INVALID_AUTH_VERIFY_REQUEST,	/* Debug port Auth verify request is invalid */
	SDBG_PORT_ALREADY_UNLOCKED_ERROR,	/* Debug port is already unlocked */
	SDBG_INVALID_DEVNODE_ERROR,			/* Device node /dev/secdbg open error */
	SDBG_INVALID_DBGPORT_ERROR,			/* Debug port is invalid */
	SDBG_INVALID_SIGNATURE_ERROR,		/* Signature is invalid */
	SDBG_INVALID_NONCE_ERROR,			/* Nonce is invalid */
	SDBG_INVALID_AUTHKEY_ERROR,			/* Auth key is invalid */
} fapi_secdbg_error_code_t;

typedef enum secdbg_port_idx {
	SDBG_RESERVED_0_PORT = 0,		/* PORT 0 is reserved */
	SDBG_RESERVED_1_PORT,			/* PORT 1 is reserved */
	SDBG_ATOM_CORE_JTAG_PORT,		/* ATOM Core JTAG Port */
	SDBG_RESERVED_3_PORT,			/* PORT 3 is reserved */
	SDBG_RESERVED_4_PORT,			/* PORT 4 is reserved */
	SDBG_RESERVED_5_PORT,			/* PORT 5 is reserved */
	SDBG_TEP_CONSOLE_UART_PORT,	/* TEP Console UART Port */
	SDBG_RESERVED_7_PORT,			/* PORT 7 is reserved */
	SDBG_MAX_PORT,
} secdbg_port_idx_t;

/*!
	\brief Initializes and starts a authenticatation session for opening a debug port.
	\param[in] ucPortNum Debug port number
	\param[out] pucNonce Pointer to Nonce
	\param[out] punNonceLen Pointer to length of nonce
	\param[in] pucAuthKey Pointer to wrapped authentication key
	\param[in] nuKeyLen Length of auth key
	\return Value 0 on success and negative value in case of failure
*/
int securedebug_auth_init(const unsigned char ucPortNum,
		unsigned char *pucNonce, unsigned int *punNonceLen,
		const unsigned char *pucAuthKey, const unsigned int unKeyLen);
/*!
	\brief Authenticated Unlocked verify scheme.
	\param[in] ucPortNum Debug port number
	\param[in] pucSignature Pointer to signature of credential
	\param[in] unSignatureLen Length of signature
	\param[in] pucNonce Pointer to nonce
	\param[in] unNonceLen Length of nonce
	\return Value 0 on success and negative value in case of failure
*/
int securedebug_auth_verify(const unsigned char ucPortNum,
		const unsigned char *pucSignature, const unsigned int unSignatureLen,
		const unsigned char *pucNonce, const unsigned int unNonceLen);
#endif

/* @} */
