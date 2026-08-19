/* cs_caltest.h
 *
 * Configuration Settings for the CAL test suite.
 */

/*****************************************************************************
* Copyright (c) 2011-2013 INSIDE Secure B.V. All Rights Reserved.
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

#define LOG_SEVERITY_MAX  LOG_SEVERITY_WARN

// control the performance of the RSA key generation test by changing the
// step size in bits
//#define CALTEST_RSA_KEYGEN_MODULUS_STEP_SIZE 512
// how much random data to retrieve from CAL
// the limit depends on the CAL implementation
// this also limits the size of the dynamically allocated buffer
//#define CALTEST_RANDNUM_MAX 16384

// include the very-long random test (N x 2500 bytes)
// this ensures the automatic reseed and alarm handling is done correctly
// NOTE: Defining CALTEST_RAND_VERYLONG as 0 means that the test runs forever
#define CALTEST_RAND_VERYLONG  100

// limit the length of the HMAC key used when testing the Asset Store
//#define CALTEST_ASSET_MAXLEN_HMAC_KEY  32


#ifndef INCLUDE_GUARD_C_CALTEST_H
#error "Please include c_caltest.h instead of cs_caltest.h"
#endif

/* end of file cs_caltest.h */
