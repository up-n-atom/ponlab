/* SPDX-License-Identifier: GPL-2.0 */
/*
 * P34X FW Download Driver
 *
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2019-2020, Intel Corporation.
 * Govindaiah Mudepalli <gmudepalli@maxlinear.com>
 * Xu Liang <lxu@maxlinear.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _P34X_PHY_PTP_H_
#define _P34X_PHY_PTP_H_

#define MAX_FREQ_ADJUSTMENT	500000000

#define N_EXT_TS		1

#define NSEC_TO_SEC		1000000000

#define PTP_REF_CLK		812500000
#define PTP_PRECISION_NS	2
#define PTP_UPDATE_FREQ_KHZ	(NSEC_TO_SEC / 1000 / PTP_PRECISION_NS)

#endif	/* _P34X_PHY_PTP_H_ */

