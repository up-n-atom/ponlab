// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation.
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
 
#ifndef DATAPATH_API_PPV4_H
#define DATAPATH_API_PPV4_H
#include <asm/byteorder.h>

#if IS_ENABLED(CONFIG_LITTLE_ENDIAN)
struct ppv4_ud_0_dw_0 {
	/* DWORD 0 */
	union {
		struct {
			u32 ig_port:8;
			u32 int_prot_info:4;
			u32 ext_prot_info:4;
			u32 ttl:1;
			u32 opt:1;
			u32 exdf:1;
			u32 indf:1;
			u32 exfragt:2;
			u32 infragt:2;
			u32 fin:1;
			u32 syn:1;
			u32 rst:1;
			u32 ack:1;
			u32 tcp_data_off:4;
		} __packed field;
		u32 all;
	};
} __packed;

struct ppv4_ud_0_dw_1 {
	/* DWORD 1 */
	union {
		struct {
			u32 ext_l3_off:8;
			u32 int_l3_off:8;
			u32 ext_l4_off:8;
			u32 int_l4_off:8;
		} __packed field;
		u32 all;
	};
} __packed;

struct ppv4_ud_0_dw_2 {
	/*DWORD 2 */
	union {
		struct {
			u32 ext_frag_info_off:8;
			u32 int_frag_info_off:8;
			u32 turbo_dox_flow:10;
			u32 res:2;
			u32 lro:1;
			u32 res1:1;
			u32 l2_off:2;
		} __packed field;
		u32 all;
	};

} __packed;

struct ppv4_ud_0_dw_3 {
	/*DWORD 3 */
	union {
		struct {
			u32 sess_idx:24;
			u32 ud1_exist:1;
			u32 res:3;
			u32 err:1;
			u32 drop_pkt:3;
		} __packed field;
		u32 all;
	};
} __packed;

struct ppv4_ud_1_dw_0 {
	/* DWORD 0 */
	union {
		struct {
			u32 cls_hash;
		} __packed field;
		u32 all;
	};
} __packed;

struct ppv4_ud_1_dw_1 {
	/* DWORD 1 */
	union {
		struct {
			u32 cls_hash;
		} __packed field;
		u32 all;
	};
} __packed;

struct ppv4_ud_1_dw_2 {
	/*DWORD 2 */
	union {
		struct {
			u32 cls_hash:8;
			u32 l3_hdr_off_5:8;
			u32 l3_hdr_off_4:8;
			u32 l3_hdr_off_3:8;
		} __packed field;
		u32 all;
	};

} __packed;

struct ppv4_ud_1_dw_3 {
	/*DWORD 3 */
	union {
		struct {
			u32 l3_hdr_off_2:8;
			u32 l3_hdr_off_1:8;
			u32 l3_hdr_off_0:8;
			u32 tun_in_off:3;
			u32 payload_off:3;
			u32 res:2;
		} __packed field;
		u32 all;
	};
} __packed;

#else /* Big Endian */

struct ppv4_ud_0_dw_0 {
	/* DWORD 0 */
	union {
		struct {
			u32 tcp_data_off:4;
			u32 ack:1;
			u32 rst:1;
			u32 syn:1;
			u32 fin:1;
			u32 infragt:2;
			u32 exfragt:2;
			u32 indf:1;
			u32 exdf:1;
			u32 opt:1;
			u32 ttl:1;
			u32 ext_prot_info:4;
			u32 int_prot_info:4;
			u32 ig_port:8;
		} __packed field;
		u32 all;
	};
} __packed;

struct ppv4_ud_0_dw_1 {
	/* DWORD 1 */
	union {
		struct {
			u32 int_l4_off:8;
			u32 ext_l4_off:8;
			u32 int_l3_off:8;
			u32 ext_l3_off:8;
		} __packed field;
		u32 all;
	};
} __packed;

struct ppv4_ud_0_dw_2 {
	/*DWORD 2 */
	union {
		struct {
			u32 l2_off:2;
			u32 res1:1;
			u32 lro:1;
			u32 res:2;
			u32 turbo_dox_flow:10;
			u32 int_frag_info_off:8;
			u32 ext_frag_info_off:8;
		} __packed field;
		u32 all;
	};

} __packed;

struct ppv4_ud_0_dw_3 {
	/*DWORD 3 */
	union {
		struct {
			u32 drop_pkt:3;
			u32 err:1;
			u32 res:3;
			u32 ud1_exist:1;
			u32 sess_idx:24;
		} __packed field;
		u32 all;
	};
} __packed;

struct ppv4_ud_1_dw_0 {
	/* DWORD 0 */
	union {
		struct {
			u32 cls_hash;
		} __packed field;
		u32 all;
	};
} __packed;

struct ppv4_ud_1_dw_1 {
	/* DWORD 1 */
	union {
		struct {
			u32 cls_hash;
		} __packed field;
		u32 all;
	};
} __packed;

struct ppv4_ud_1_dw_2 {
	/*DWORD 2 */
	union {
		struct {
			u32 l3_hdr_off_3:8;
			u32 l3_hdr_off_4:8;
			u32 l3_hdr_off_5:8;
			u32 cls_hash:8;
		} __packed field;
		u32 all;
	};

} __packed;

struct ppv4_ud_1_dw_3 {
	/*DWORD 3 */
	union {
		struct {
			u32 res:2;
			u32 payload_off:3;
			u32 tun_in_off:3;
			u32 l3_hdr_off_0:8;
			u32 l3_hdr_off_1:8;
			u32 l3_hdr_off_2:8;
		} __packed field;
		u32 all;
	};
} __packed;

#endif

struct ppv4_ud_0 {
	struct ppv4_ud_0_dw_0 dw0;
	struct ppv4_ud_0_dw_1 dw1;
	struct ppv4_ud_0_dw_2 dw2;
	struct ppv4_ud_0_dw_3 dw3;
};

struct ppv4_ud_1 {
	struct ppv4_ud_1_dw_0 dw0;
	struct ppv4_ud_1_dw_1 dw1;
	struct ppv4_ud_1_dw_2 dw2;
	struct ppv4_ud_1_dw_3 dw3;
};

static inline u8 pp_get_rx_port(char *buf_base)
{
	return buf_base[17];
}

static inline u32 pp_get_signature(char *buf_base)
{
	return le32_to_cpup((__le32 *)(buf_base + 32));
}

static inline int pp_get_hash(char *buf_base, u32 *h1, u32 *h2)
{
	u64 val = le64_to_cpup((__le64 *)(buf_base + 36));

	if (!buf_base || !h1 || !h2)
		return -1;
	*h1 = val & 0xfffff;
	*h2 = (val >> 20) & 0xfffff;
	return 0;
}

#endif /*DATAPATH_API_PPV4_H*/

