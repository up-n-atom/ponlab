/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2019-2020 Intel Corporation.
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

#ifndef __LGM_EPU_RCU_H
#define __LGM_EPU_RCU_H

#include <dt-bindings/power/lgm-power.h>

struct epu_rcu_tbl {
	int rcuids[2];
};

static const struct epu_rcu_tbl power_tbl[] = {
	[LGM_EPU_PD_PPV4]	= { {0x801F1F, 0} },
	[LGM_EPU_PD_GSW]	= { {0x700808, 0} },
	[LGM_EPU_PD_CQM]	= { {0x700B0B, 0} },
	[LGM_EPU_PD_VOICE]	= { {0x100202, 0} },
	[LGM_EPU_PD_PON]	= { {0x700909, 0} }, /* PON need one more rst in B0 */
	[LGM_EPU_PD_EIP197]	= { {0x100606, 0} },
	[LGM_EPU_PD_VAULT130]	= { {0x100707, 0} },
	[LGM_EPU_PD_TOE]	= { {0x100808, 0} },
	[LGM_EPU_PD_SDXC]	= { {0x100A0A, 0} },
	[LGM_EPU_PD_EMMC]	= { {0x100B0B, 0} },
	[LGM_EPU_PD_QSPI]	= { {0x100101, 0} },
	[LGM_EPU_PD_SSB]	= { {0x101313, 0} },
	[LGM_EPU_PD_USB1]	= { {0x701616, 0} },
	[LGM_EPU_PD_USB2]	= { {0x701717, 0} },
	[LGM_EPU_PD_ASC0]	= { {0x301111, 0} },
	[LGM_EPU_PD_ASC1]	= { {0x301212, 0} },
	[LGM_EPU_PD_ASC2]	= { {0x301313, 0} },
	[LGM_EPU_PD_SSC0]	= { {0x300404, 0} },
	[LGM_EPU_PD_SSC1]	= { {0x300505, 0} },
	[LGM_EPU_PD_SSC2]	= { {0x300606, 0} },
	[LGM_EPU_PD_SSC3]	= { {0x300707, 0} },
	[LGM_EPU_PD_I2C1]	= { {0x300909, 0} },
	[LGM_EPU_PD_I2C2]	= { {0x300A0A, 0} },
	[LGM_EPU_PD_I2C3]	= { {0x300B0B, 0} },
	[LGM_EPU_PD_LEDC0]	= { {0x301616, 0} },
	[LGM_EPU_PD_PCM0]	= { {0x301C1C, 0} },
	[LGM_EPU_PD_EBU]	= { {0x300101, 0} },
	[LGM_EPU_PD_I2S0]	= { {0x301919, 0} },
	[LGM_EPU_PD_I2S1]	= { {0x301A1A, 0} },
	[LGM_EPU_PD_EM4]	= { {0x700C0C, 0} },
	[LGM_EPU_PD_PCM1]	= { {0x301D1D, 0} },
	[LGM_EPU_PD_PCM2]	= { {0x301E1E, 0} },
	[LGM_EPU_PD_XPCS5]	= { {0x700F0F, 0} },
	[LGM_EPU_PD_LEDC1]	= { {0x301717, 0} },
	[LGM_EPU_PD_PCIE30]	= { {0x500000, 0} },
	[LGM_EPU_PD_PCIE40]	= { {0x500101, 0} },
	[LGM_EPU_PD_SATA0]	= { {0x500F0F, 0} },
	[LGM_EPU_PD_SATA1]	= { {0x501010, 0} },
	[LGM_EPU_PD_PCIE31]	= { {0x600000, 0} },
	[LGM_EPU_PD_PCIE41]	= { {0x600101, 0} },
	[LGM_EPU_PD_SATA2]	= { {0x600F0F, 0} },
	[LGM_EPU_PD_SATA3]	= { {0x601010, 0} },
	[LGM_EPU_PD_PCIE10]	= { {0x500202, 0} },
	[LGM_EPU_PD_PCIE20]	= { {0x500303, 0} },
	[LGM_EPU_PD_PCIE11]	= { {0x600202, 0} },
	[LGM_EPU_PD_PCIE21]	= { {0x600303, 0} },
	[LGM_EPU_PD_XPCS10]	= { {0x500C0C, 0} },
	[LGM_EPU_PD_XPCS11]	= { {0x500D0D, 0} },
	[LGM_EPU_PD_XPCS20]	= { {0x600C0C, 0} },
	[LGM_EPU_PD_XPCS21]	= { {0x600D0D, 0} },
};

static inline const int *lgm_epu_to_rcu_id(unsigned int epuid)
{
	if (epuid >= ARRAY_SIZE(power_tbl))
		return NULL;

	if (!power_tbl[epuid].rcuids[0])
		return NULL;

	return power_tbl[epuid].rcuids;
}

static inline int lgm_rcu_to_epu_id(int rcuid)
{
	int i;
	const int size = ARRAY_SIZE(power_tbl);

	for (i = 0; i < size; i++) {
		if (power_tbl[i].rcuids[0] == rcuid)
			return i;
	}

	return -1;
}
#endif /*_ _LGM_EPU_RCU_H */
