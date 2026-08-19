/******************************************************************************

  Copyright (C) 2022-2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : fapi_sec_debugs.c					                       *
 *     Project    : UGW                                                        *
 *     Description: secure debug fapi APIs.						           *
 *                                                                             *
 ******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "fapi_sec_service.h"
#include "fapi_sec_debug.h"
#include "help_error.h"
#include "help_logging.h"

#include "safe_str_lib.h"
#include "safe_lib.h"
#include "safe_mem_lib.h"

/**===============================================================================================
 * @brief Initializes and starts a authenticatation session for opening a debug port
 *
 * @param ucPortNum
 * Debug port number
 *
 * @param pucNonce
 * Pointer to nonce
 *
 * @param *punNonceLen
 * Pointer to hold length of nonce
 *
 * @param pucAuthKey
 * Pointer to wrapped authentication key
 *
 * @param unKeyLen
 * Length of wrapped authentication key
 *
 * @return
 * Value 0 on success and negative value in case of failure

 ==================================================================================================
 */
int securedebug_auth_init(const unsigned char ucPortNum,
		unsigned char *pucNonce, unsigned int *punNonceLen,
		const unsigned char *pucAuthKey, const unsigned int unKeyLen)
{
	sec_dbg_auth_start_t xUnlockStart;
	int nOpenFd = -1;
	int retval = -SDBG_INVALID_DEVNODE_ERROR;
	/* Validate Debug port number */
	if (((char)ucPortNum != SDBG_ATOM_CORE_JTAG_PORT) && ((char)ucPortNum != SDBG_TEP_CONSOLE_UART_PORT)) {
		LOGF_LOG_ERROR("<Secure debug> Invalid Debug port number\n");
		retval = -SDBG_INVALID_DBGPORT_ERROR;
		goto finish;
	}

	/* Validate Nonce Pointer (RB) number */
	if (!pucNonce) {
		LOGF_LOG_ERROR("<Secure debug> Invalid OUT Nonce (RB) pointer\n");
		retval = -SDBG_INVALID_NONCE_ERROR;
		goto finish;
	}

	/* Validate IN/OUT Nonce Length */
	if (!punNonceLen || ((int)*punNonceLen <= 0)) {
		LOGF_LOG_ERROR("<Secure debug> Invalid IN/OUT Nonce (RB) Length and Pointer\n");
		retval = -SDBG_INVALID_NONCE_ERROR;
		goto finish;
	}

	/* Validate Auth Key Pointer */
	if (!pucAuthKey) {
		LOGF_LOG_ERROR("<Secure debug> Invalid Auth Key pointer\n");
		retval = -SDBG_INVALID_AUTHKEY_ERROR;
		goto finish;
	}

	/* Validate Auth Key Length */
	if ((int)unKeyLen < 0) {
		LOGF_LOG_ERROR("<Secure debug> Invalid Auth Key Length\n");
		retval = -SDBG_INVALID_AUTHKEY_ERROR;
		goto finish;
	}

	memset_s(&xUnlockStart, sizeof(sec_dbg_auth_start_t), 0,
				sizeof(sec_dbg_auth_start_t));

	xUnlockStart.port_num = ucPortNum;
	xUnlockStart.nonce = pucNonce;
	xUnlockStart.nonce_len = *punNonceLen;
	xUnlockStart.auth_key = pucAuthKey;
	xUnlockStart.auth_key_len = unKeyLen;

	if ((nOpenFd = open(SEC_DEBUG_PATH, O_RDONLY)) < 0) {
		LOGF_LOG_ERROR
		("<Secure debug open> Failed to open the Device\n");
		goto finish;
	}

	if ((retval = ioctl(nOpenFd, SSC_SECURE_DEBUG_AUTH_START, &xUnlockStart)) < 0) {
		retval = -errno;
		LOGF_LOG_ERROR("<Secure debug> Debug Authentication Start Failed with error : %s\n", strerror(errno));
		goto finish;
	}
	/* Get the RB nonce lenth */
	*punNonceLen = xUnlockStart.nonce_len;

finish:
	if (nOpenFd >= 0)
		close(nOpenFd);
	return retval;
}

/**===============================================================================================
 * @brief  Starts the Authenticated Unlock verify
 scheme for given debug port.
 *
 * @param ucPortNum
 * Debug port number
 *
 * @param pucSignature
 * Pointer to signature of credential
 *
 * @param unSignatureLen
 * Length of signature
 *
 * @param pucNonce
 * Pointer to nonce
 *
 * @param unNonceLen
 * Length of nonce
 *
 * @return
 * Value 0 on success and negative value in case of failure

 ==================================================================================================
 */
int securedebug_auth_verify(const unsigned char ucPortNum,
		const unsigned char *pucSignature, const unsigned int unSignatureLen,
		const unsigned char *pucNonce, const unsigned int unNonceLen)
{
	sec_dbg_auth_unlock_t xUnlockVerify;
	int nOpenFd = -1;
	int retval = -SDBG_INVALID_DEVNODE_ERROR;

	/* Validate Debug port number */
	if (((char)ucPortNum != SDBG_ATOM_CORE_JTAG_PORT) && ((char)ucPortNum != SDBG_TEP_CONSOLE_UART_PORT)) {
		LOGF_LOG_ERROR("<Secure debug> Invalid Debug port number\n");
		retval = -SDBG_INVALID_DBGPORT_ERROR;
		goto finish;
	}

	/* Validate Signature Pointer */
	if (!pucSignature) {
		LOGF_LOG_ERROR("<Secure debug> Invalid Signature pointer\n");
		retval = -SDBG_INVALID_SIGNATURE_ERROR;
		goto finish;
	}

	/* Validate Signature Length */
	if ((int)unSignatureLen < 0) {
		LOGF_LOG_ERROR("<Secure debug> Invalid Signature Length\n");
		retval = -SDBG_INVALID_SIGNATURE_ERROR;
		goto finish;
	}

	/* Validate Nonce Pointer (RA) number */
	if (!pucNonce) {
		LOGF_LOG_ERROR("<Secure debug> Invalid IN Nonce (RA) pointer\n");
		retval = -SDBG_INVALID_NONCE_ERROR;
		goto finish;
	}

	/* Validate IN/OUT Nonce Length */
	if ((int)unNonceLen <= 0) {
		LOGF_LOG_ERROR("<Secure debug> Invalid IN Nonce (RA) Length\n");
		retval = -SDBG_INVALID_NONCE_ERROR;
		goto finish;
	}

	memset_s(&xUnlockVerify, sizeof(sec_dbg_auth_unlock_t), 0,
			sizeof(sec_dbg_auth_unlock_t));

	xUnlockVerify.port_num = ucPortNum;
	xUnlockVerify.p_signature = pucSignature;
	xUnlockVerify.sign_sz = unSignatureLen;
	xUnlockVerify.random_nonce = pucNonce;
	xUnlockVerify.rd_nonce_len = unNonceLen;

	if ((nOpenFd = open(SEC_DEBUG_PATH, O_RDONLY)) < 0) {
		LOGF_LOG_ERROR
			("<Secure debug open> Failed to open the Device\n");
		goto finish;
	}

	if ((retval = ioctl(nOpenFd, SSC_SECURE_DEBUG_AUTH_VERIFY, &xUnlockVerify)) < 0) {
		retval = -errno;
		LOGF_LOG_ERROR("<Secure debug> Debug Authentication Verify Failed with error : %s\n",strerror(errno));
	}
finish:
	if (nOpenFd >= 0)
		close(nOpenFd);
	return retval;
}
