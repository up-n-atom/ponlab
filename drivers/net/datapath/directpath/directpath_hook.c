// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2023 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <net/directpath_api.h>

dpdp_register_dev_hook_t __rcu *dpdp_register_dev_hook __read_mostly;
EXPORT_SYMBOL_GPL(dpdp_register_dev_hook);
dpdp_register_subif_hook_t __rcu *dpdp_register_subif_hook __read_mostly;
EXPORT_SYMBOL_GPL(dpdp_register_subif_hook);

dpdp_fwd_offload_hook_t __rcu *dpdp_fwd_offload_hook __read_mostly;
EXPORT_SYMBOL_GPL(dpdp_fwd_offload_hook);
dpdp_qos_offload_hook_t __rcu *dpdp_qos_offload_hook __read_mostly;
EXPORT_SYMBOL_GPL(dpdp_qos_offload_hook);
