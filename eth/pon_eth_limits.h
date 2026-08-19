/******************************************************************************
 *
 *  Copyright (c) 2025 MaxLinear, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 ******************************************************************************/

#ifndef __PON_ETH_LIMITS_H
#define __PON_ETH_LIMITS_H

/**
 * TODO: In the driver, treat these limits as the maximal supported range.
 * For actual HW/FW, allow configuration of smaller values as needed.
 */

/** Maximum number of allocations/t-conts supported by the PON IP hardware
 * (hardware-imposed limit)
 */
#define PON_ALLOC_MAX 64

/** Maximum number of GEMs supported by the PON IP hardware
 * (hardware-imposed limit)
 */
#define PON_GEM_MAX 256

#endif /* __PON_ETH_LIMITS_H */
