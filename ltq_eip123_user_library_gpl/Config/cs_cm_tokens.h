/* cs_cm_tokens.h
 *
 * Configuration Settings for the CM Tokens module.
 */

/*****************************************************************************
* Copyright (c) 2010-2013 INSIDE Secure B.V. All Rights Reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

// this option enables function call parameter checking
// disable it to reduce code size and reduce overhead
// make sure upper layer does noy rely on these checks!
#define CMTOKENS_STRICT_ARGS

//#define CMTOKENS_REMOVE_ERROR_DESCRIPTIONS

// use the options below to selectively remove unused functions
//#define CMTOKENS_REMOVE_PARSERESPONSE_ERRORDETAILS
//#define CMTOKENS_REMOVE_PARSERESPONSE_NVMERRORDETAILS
//#define CMTOKENS_REMOVE_CRYPTO_3DES
//#define CMTOKENS_REMOVE_CRYPTO_AES_F8
//#define CMTOKENS_REMOVE_CRYPTO_ARC4
//#define CMTOKENS_REMOVE_CRYPTO_CAMELLIA
//#define CMTOKENS_REMOVE_CRYPTO_C2
//#define CMTOKENS_REMOVE_CRYPTO_MULTI2

/* end of file cs_cm_tokens.h */
