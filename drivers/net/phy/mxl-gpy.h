/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2024 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2, as published
 * by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MXL_GPY_H
#define __MXL_GPY_H

#if IS_ENABLED(CONFIG_EIP160S_MACSEC)
#include "mxl-gpy-macsec.h"

extern struct gpy2xx_macsec_cfg *(*gpy_get_macsec_cfg_fn)(struct phy_device *phydev);

struct gpy2xx_macsec_cfg *gpy_get_macsec_cfg(struct phy_device *phydev);
#endif

#endif /* __MXL_GPY_H */
