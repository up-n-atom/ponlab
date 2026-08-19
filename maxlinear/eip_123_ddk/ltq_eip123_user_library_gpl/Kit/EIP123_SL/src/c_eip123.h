/* c_eip123.h
 *
 * Configuration options for the EIP123 module.
 * The project-specific cs_eip123.h file is included,
 * whereafter defaults are provided for missing parameters.
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

/*----------------------------------------------------------------
 * Defines that can be used in the cs_xxx.h file
 */

/* currently none */


/*----------------------------------------------------------------
 * inclusion of cs_eip123.h
 */
#include "cs_eip123.h"


/*----------------------------------------------------------------
 * provide backup values for all missing configuration parameters
 */

#ifndef EIP123_MAX_PHYSICAL_FRAGMENTS
#define EIP123_MAX_PHYSICAL_FRAGMENTS  8
#endif


/*----------------------------------------------------------------
 * other configuration parameters that cannot be set in cs_xxx.h
 * but are considered product-configurable anyway
 */


/* end of file c_eip123.h */
