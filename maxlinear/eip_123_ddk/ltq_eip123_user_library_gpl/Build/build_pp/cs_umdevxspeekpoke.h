/* cs_umdevxspeekpoke.h
 *
 * Configuration Switches
 */

/*****************************************************************************
* Copyright (c) 2009-2013 INSIDE Secure B.V. All Rights Reserved.
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

#define UMDEVXSPEEKPOKE_SHMEM_PROVIDER
#define UMDEVXSPEEKPOKE_SHMEM_OBTAINER
#define UMDEVXSPEEKPOKE_INTERRUPT

// some platforms need to manipulate the address retrieved from the GBI
// comment-out if your platform does not need this
//#define UMDEVXSPEEKPOKE_GBI_ADDR_ADD  0x00000000

/* end of file cs_umdevxspeekpoke.h */
