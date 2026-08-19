/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2021-2022 MaxLinear, Inc.
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
#ifndef __DATAPATH_BR_DOMAIN_H
#define __DATAPATH_BR_DOMAIN_H

/*! @brief enum DP_SUBIF_BR_DM_TT
 *  Specific domains for traffic types.
 */
enum DP_SUBIF_BR_DM_TT {
	DP_BR_DM_UC = 0, /*!< Unicast UNI domain */
	DP_BR_DM_UCA = 1, /*!< Unicast ANI domain */
	DP_BR_DM_MC = 2, /*!< Multicast domain */
	DP_BR_DM_BC1 = 3, /*!< Broadcast domain one */
	DP_BR_DM_BC2 = 4, /*!< Broadcast domain two */
	DP_BR_DM_US = 5, /*!< Upstream domain */
	DP_BR_DM_IPH = 6, /*!< IP Host domain */
};

#endif
