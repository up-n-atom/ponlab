/* cs_eip123.h
 *
 * Configuration Settings for the EIP123_SL module.
 */

/*****************************************************************************
* Copyright (c) 2007-2013 INSIDE Secure B.V. All Rights Reserved.
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

// set this option to enable checking of all arguments to all EIP123 functions
// disable it to reduce code size and reduce overhead
#define EIP123_STRICT_ARGS

// footprint reduction switches
//#define EIP123_REMOVE_VERIFYDEVICECOMMS
#define EIP123_REMOVE_GETOPTIONS
#define EIP123_REMOVE_MAILBOXACCESSCONTROL
//#define EIP123_REMOVE_LINK
#define EIP123_REMOVE_UNLINK
//#define EIP123_REMOVE_CANWRITETOKEN

/* end of file cs_eip123.h */
