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
#ifndef __MXL_VUNI_H
#define __MXL_VUNI_H

#define SUBIF_VANI0_0 0
#define SUBIF_VUNI0_0 1
#define SUBIF_VUNI0_1 2
/* temporary dummy sub interface for PON broadcast */
#define SUBIF_VUNI0_2 3

/* Bridge domain: none */
#define VUNI_BR_DOMAIN_NO 0
/* Bridge domain: unicast */
#define VUNI_BR_DOMAIN_UC 1
/* Bridge domain: multicast */
#define VUNI_BR_DOMAIN_MC 2
/* Bridge domain: broadcast */
#define VUNI_BR_DOMAIN_BC 3

#endif
