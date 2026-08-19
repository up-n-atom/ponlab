/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Driver for MACsec Engine inside Intel CHD GSWIP or GPHY
 * Copyright 2020 - 2022 Maxlinear, Inc.
 * Copyright 2019 - 2020 Intel Corporation
 */

#ifndef _EIP160S_INTERNAL_H_
#define _EIP160S_INTERNAL_H_

/* Egress engine registers */
#include "registers/eip160s_egr_classifier_control_packet.h"
#include "registers/eip160s_egr_classifier_debug.h"
#include "registers/eip160s_egr_classifier_various_control_regs.h"
#include "registers/eip160s_egr_flow_control_page0.h"
#include "registers/eip160s_egr_oppe.h"
#include "registers/eip160s_egr_sam_enable_ctrl.h"
#include "registers/eip160s_egr_sam_rules_page0.h"
#include "registers/eip160s_egr_stat_control.h"
#include "registers/eip160s_egr_stat_global.h"
#include "registers/eip160s_egr_stat_sa_page0.h"
#include "registers/eip160s_egr_stat_vlan.h"
#include "registers/eip160s_egr_system_control.h"

/* Ingress engine registers */
#include "registers/eip160s_ing_classifier_control_packet.h"
#include "registers/eip160s_ing_classifier_debug.h"
#include "registers/eip160s_ing_classifier_various_control_regs.h"
#include "registers/eip160s_ing_flow_control_page0.h"
#include "registers/eip160s_ing_oppe.h"
#include "registers/eip160s_ing_sam_enable_ctrl.h"
#include "registers/eip160s_ing_sam_rules_page0.h"
#include "registers/eip160s_ing_stat_control.h"
#include "registers/eip160s_ing_stat_global.h"
#include "registers/eip160s_ing_stat_sa_page0.h"
#include "registers/eip160s_ing_stat_vlan.h"
#include "registers/eip160s_ing_system_control.h"

/* Advanced Interrupt Controller (AIC) engine registers */
#include "registers/eip201_egr_aic_registers.h"
#include "registers/eip201_ing_aic_registers.h"

/* Crypto Core engine registers */
#include "registers/eip62_egr_ccore_registers.h"
#include "registers/eip62_ing_ccore_registers.h"

/* Ingress Consistency Check engine registers */
#include "registers/eip160s_ing_cc_control.h"
#include "registers/eip160s_ing_cc_rules_page0.h"

#include "eip160s_struct.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#endif

#define REG_FIELD_GET(reg, field) ((reg & field ## _MASK) >> field ## _POS)
#define REG_FIELD_SET(reg, field) ((reg << field ## _POS) & field ## _MASK)
#define REG_FIELD_REPL(src, reg, field) (src = ((src & (~field ## _MASK)) | REG_FIELD_SET(reg, field)))

int _macsec_pop_ccw(enum msec_dir dir, struct ctx_ctrl_word *ccw, u32 *_ccw);
int _macsec_pop_key(struct crypto_parms *cp, u32 *le_key);
int _macsec_pop_hkey(struct crypto_parms *cp, u32 *le_hkey);
int _macsec_pop_sci(struct crypto_parms *cp, u32 *le_iv);
int _macsec_pop_csalt(struct crypto_parms *cp, u32 *le_ctx_salt);
int _macsec_pop_csalt(struct crypto_parms *cp, u32 *le_ctx_salt);
int _macsec_copy_key(struct crypto_parms *cp, u32 *le_key);
int _macsec_copy_hkey(struct crypto_parms *cp, u32 *le_hkey);
int _macsec_copy_sci(struct crypto_parms *cp, u32 *le_iv);
int _macsec_copy_csalt(struct crypto_parms *cp, u32 *le_ctx_salt);

/* Internal used for structure eip160_config reg. */
struct _eip160_config {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 nr_of_sas: 8;
            u32 consist_checks: 8;
            u32 match_sci: 1;
            u32 reserved: 15;
#elif defined(__BIG_ENDIAN)
            u32 reserved: 15;
            u32 match_sci: 1;
            u32 consist_checks: 8;
            u32 nr_of_sas: 8;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_eip160_cfg;
    };
} ;

/* Internal used for structure eip160_config2 reg. */
struct _eip160_extcfg {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 sa_counters: 4;
            u32 vl_counters: 4;
            u32 global_counters: 6;
            u32 saturate_cntrs: 1;
            u32 auto_cntr_reset: 1;
            u32 sa_octet_ctrs: 2;
            u32 vl_octet_ctrs: 2;
            u32 glob_octet_ctrs: 4;
            u32 ingress_only: 1;
            u32 egress_only: 1;
            u32 reserved: 1;
            u32 tag_bypass: 1;
            u32 reserved2: 4;
#elif defined(__BIG_ENDIAN)
            u32	reserved2: 4;
            u32	tag_bypass: 1;
            u32	reserved: 1;
            u32	egress_only: 1;
            u32	ingress_only: 1;
            u32	glob_octet_ctrs: 4;
            u32	vl_octet_ctrs: 2;
            u32	sa_octet_ctrs: 2;
            u32	auto_cntr_reset: 1;
            u32	saturate_cntrs: 1;
            u32	global_counters: 6;
            u32	vl_counters: 4;
            u32	sa_counters: 4;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_eip160_extcfg;
    };
};

/* Internal used for structure eip160_version, eip201_version, eip62_version. */
struct _eip_eng_ver {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 eip_nr: 8;
            u32 eip_nr_complement: 8;
            u32 patch_level: 4;
            u32 minor_version: 4;
            u32 major_version: 4;
            u32 reserved: 4;
#elif defined(__BIG_ENDIAN)
            u32 reserved: 4;
            u32 major_version: 4;
            u32 minor_version: 4;
            u32 patch_level: 4;
            u32 eip_nr_complement: 8;
            u32 eip_nr: 8;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_engxxx_ver;
    };
};

/* Internal used for structure eip62_config. */
struct _eip62_config {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
			u32 minor_revision: 4;
			u32 major_revision: 4;
			u32 fpga_solution: 1;
			u32 gf_sboxes: 1;
			u32 lookup_sboxes: 1;
			u32 macsec_aes_only: 1;
			u32 aes_present: 1;
			u32 aes_fb: 1;
			u32 aes_speed: 4;
			u32 aes192: 1;
			u32 aes256: 1;
			u32 eop_param_bits: 3;
			u32 reserved: 4;
			u32 sec_offset: 1;
			u32 reserved2: 2;
			u32 ghash_present: 1;
			u32 ghash_speed: 1;
#elif defined(__BIG_ENDIAN)
			u32 ghash_speed: 1;
			u32 ghash_present: 1;
			u32 reserved2: 2;
			u32 sec_offset: 1;
			u32 reserved: 4;
			u32 eop_param_bits: 3;
			u32 aes256: 1;
			u32 aes192: 1;
			u32 aes_speed: 4;
			u32 aes_fb: 1;
			u32 aes_present: 1;
			u32 macsec_aes_only: 1;
			u32 lookup_sboxes: 1;
			u32 gf_sboxes: 1;
			u32 fpga_solution: 1;
			u32 major_revision: 4;
			u32 minor_revision: 4;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_eip62_cfg;
    };
};

/* Internal structure used for eip201_config. */
struct _eip201_config {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 number_of_inputs: 6;
            u32 reserved: 26;
#elif defined(__BIG_ENDIAN)
            u32 reserved: 26;
            u32 number_of_inputs: 6;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_aic_opt;
    };
};

/* Internal used for structure update_sa_cw. */
struct _update_sa_cw {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 next_sa_index: 14;
            u32 sa_expired_irq: 1;
            u32 sa_index_valid: 1;
            u32 flow_index: 15;
            u32 sa_ind_update_en: 1;
#elif defined(__BIG_ENDIAN)
            u32 sa_ind_update_en: 1;
            u32 flow_index: 15;
            u32 sa_index_valid: 1;
            u32 sa_expired_irq: 1;
            u32 next_sa_index: 14;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_sa_cw;
    };
};

/* Output Transform Record - Internal use for transform_rec. */
struct _raw_trans_rec {
    u8 sa_index;
    u32 raw_trans_rec[EGR_MAX_TR_SIZE_WORDS];
};

/* HW Transform Record for CA type of GCM-AES-128 - Internal use for 
transform_rec. */
struct _ing_tr_32bpn_128bak {
    u32 ccw;
    u32 ctx_id;
    u32 key[MACSEC_CA128_KEY_LEN_WORD];
    u32 hkey[MACSEC_CA128_KEY_LEN_WORD];
    u32 seq;
    u32 mask;
    u32 iv0;
    u32 iv1;
    u32 res0[6];
};

/* HW Transform Record for CA type of GCM-AES-256 - Internal use for 
transform_rec. */
struct _ing_tr_32bpn_256bak {
    u32 ccw;
    u32 ctx_id;
    u32 key[MACSEC_CA256_KEY_LEN_WORD];
    u32 hkey[MACSEC_CA128_KEY_LEN_WORD];
    u32 seq;
    u32 mask;
    u32 iv0;
    u32 iv1;
    u32 res0[2];
};

/* HW Transform Record for CA type of GCM-AES-XPN-128 - Internal use for 
transform_rec. */
struct _ing_tr_64bpn_128bak {
    u32 ccw;
    u32 ctx_id;
    u32 key[MACSEC_CA128_KEY_LEN_WORD];
    u32 hkey[MACSEC_CA128_KEY_LEN_WORD];
    u32 seq0;
    u32 seq1;
    u32 mask;
    u32 iv0;
    u32 iv1;
    u32 iv2;
    u32 res[4];
};

/* HW Transform Record for CA type of GCM-AES-XPN-256 - Internal use for 
transform_rec. */
struct _ing_tr_64bpn_256bak {
    u32 ccw;
    u32 ctx_id;
    u32 key[MACSEC_CA256_KEY_LEN_WORD];
    u32 hkey[MACSEC_CA128_KEY_LEN_WORD];
    u32 seq0;
    u32 seq1;
    u32 mask;
    u32 iv0;
    u32 iv1;
    u32 iv2;
};

/* HW Transform Record for CA type of GCM-AES-128 - Internal use for 
transform_rec. */
struct _egr_tr_32bpn_128bak {
    u32 ccw;
    u32 ctx_id;
    u32 key[MACSEC_CA128_KEY_LEN_WORD];
    u32 hkey[MACSEC_CA128_KEY_LEN_WORD];
    u32 seq;
    u32 iv0;
    u32 iv1;
    u32 res0[2];
    u32 upd_sa_cw;
    u32 res1[8];
};

/* HW Transform Record for CA type of GCM-AES-256 - Internal use for 
transform_rec. */
struct _egr_tr_32bpn_256bak {
    u32 ccw;
    u32 ctx_id;
    u32 key[MACSEC_CA256_KEY_LEN_WORD];
    u32 hkey[MACSEC_CA128_KEY_LEN_WORD];
    u32 seq;
    u32 iv0;
    u32 iv1;
    u32 res0[2];
    u32 upd_sa_cw;
    u32 res1[4];
};

/* HW Transform Record for CA type of GCM-AES-XPN-128 - Internal use for 
transform_rec. */
struct _egr_tr_64bpn_128bak {
    u32 ccw;
    u32 ctx_id;
    u32 key[MACSEC_CA128_KEY_LEN_WORD];
    u32 hkey[MACSEC_CA128_KEY_LEN_WORD];
    u32 seq0;
    u32 seq1;
    u32 res0;
    u32 is0;
    u32 is1;
    u32 is2;
    u32 iv0;
    u32 iv1;
    u32 res1;
    u32 upd_sa_cw;
    u32 res2[4];
};

/* HW Transform Record for CA type of GCM-AES-XPN-256 - Internal use for 
transform_rec. */
struct _egr_tr_64bpn_256bak {
    u32 ccw;
    u32 ctx_id;
    u32 key[MACSEC_CA256_KEY_LEN_WORD];
    u32 hkey[MACSEC_CA128_KEY_LEN_WORD];
    u32 seq0;
    u32 seq1;
    u32 res0;
    u32 is0;
    u32 is1;
    u32 is2;
    u32 iv0;
    u32 iv1;
    u32 res1;
    u32 upd_sa_cw;
};

/* Internal used for structure sam_misc_parm, sam_vlan_parm. */
struct _sam_misc_par {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 vlan_valid: 1;
            u32 qinq_found: 1;
            u32 stag_valid: 1;
            u32 qtag_valid: 1;
            u32 vlan_up: 3;
            u32 control_packet: 1;
            u32 untagged: 1;
            u32 tagged: 1;
            u32 bad_tag: 1;
            u32 kay_tag: 1;
            u32 source_port: 2;
            u32 reserved1: 2;
            u32 match_priority: 4;
            u32 reserved2: 4;
            u32 macsec_tci_an: 8;
#elif defined(__BIG_ENDIAN)
            u32 macsec_tci_an: 8;
            u32 reserved2: 4;
            u32 match_priority: 4;
            u32 reserved1: 2;
            u32 source_port: 2;
            u32 kay_tag: 1;
            u32 bad_tag: 1;
            u32 tagged: 1;
            u32 untagged: 1;
            u32 control_packet: 1;
            u32 vlan_up: 3;
            u32 qtag_valid: 1;
            u32 stag_valid: 1;
            u32 qinq_found: 1;
            u32 vlan_valid: 1;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_sam_misc;
    };
};

/* Internal used for structure sam_mask_parm. */
struct _sam_mask_par {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 mac_sa_mask: 6;
            u32 mac_da_mask: 6;
            u32 mac_etype_mask: 1;
            u32 vlan_valid_mask: 1;
            u32 qinq_found_mask: 1;
            u32 stag_valid_mask: 1;
            u32 qtag_valid_mask: 1;
            u32 vlan_up_mask: 1;
            u32 vlan_id_mask: 1;
            u32 source_port_mask: 1;
            u32 ctrl_packet_mask: 1;
            u32 vlan_up_inner_mask: 1;
            u32 vlan_id_inner_mask: 1;
            u32 macsec_sci_mask: 1;
            u32 tci_an_mask: 8;
#elif defined(__BIG_ENDIAN)
            u32	tci_an_mask: 8;
            u32	macsec_sci_mask: 1;
            u32	vlan_id_inner_mask: 1;
            u32	vlan_up_inner_mask: 1;
            u32	ctrl_packet_mask: 1;
            u32	source_port_mask: 1;
            u32	vlan_id_mask: 1;
            u32	vlan_up_mask: 1;
            u32	qtag_valid_mask: 1;
            u32	stag_valid_mask: 1;
            u32	qinq_found_mask: 1;
            u32	vlan_valid_mask: 1;
            u32	mac_etype_mask: 1;
            u32 mac_da_mask: 6;
            u32 mac_sa_mask: 6;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_sam_mask;
    };
};

/* Internal use for extended parameters match. */
struct _sam_extn_par {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 vlan_id_inner: 12;
            u32 vlan_up_inner: 3;
            u32 reserved1: 1;
            u32 flow_index: 8;
            u32 reserved2: 8;
#elif defined(__BIG_ENDIAN)
            u32	reserved2: 8;
            u32	flow_index: 8;
            u32	reserved1: 1;
            u32	vlan_up_inner: 3;
            u32	vlan_id_inner: 12;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_sam_extn;
    };
};

/* Output Secure Association Matching (SAM) rule params - Internal use for 
sa_match_parm. */
struct _raw_sam_par {
    u8 rule_index;
    u32 raw_sam_par[IE_MAX_SAM_SIZE_WORDS];
};

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
/* Internal structure used for sam_cp_tag. */
struct _sam_cp_tag {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 map_tbl_pcp_0: 3;
            u32 map_tbl_pcp_1: 3;
            u32 map_tbl_pcp_2: 3;
            u32 map_tbl_pcp_3: 3;
            u32 map_tbl_pcp_4: 3;
            u32 map_tbl_pcp_5: 3;
            u32 map_tbl_pcp_6: 3;
            u32 map_tbl_pcp_7: 3;
            u32 def_up: 3;
            u32 stag_up_en: 1;
            u32 qtag_up_en: 1;
            u32 parse_qinq: 1;
            u32 parse_stag: 1;
            u32 parse_qtag: 1;
#elif defined(__BIG_ENDIAN)
            u32 parse_qtag: 1;
            u32 parse_stag: 1;
            u32 parse_qinq: 1;
            u32 qtag_up_en: 1;
            u32 stag_up_en: 1;
            u32 def_up: 3;
            u32 map_tbl_pcp_7: 3;
            u32 map_tbl_pcp_6: 3;
            u32 map_tbl_pcp_5: 3;
            u32 map_tbl_pcp_4: 3;
            u32 map_tbl_pcp_3: 3;
            u32 map_tbl_pcp_2: 3;
            u32 map_tbl_pcp_1: 3;
            u32 map_tbl_pcp_0: 3;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_ocp_tag;
    };
};

/* Internal structure used for sam_pp_tags. */
struct _sam_pp_tags {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 qtag_value: 16;
            u32 stag_value: 16;
#elif defined(__BIG_ENDIAN)
            u32 stag_value: 16;
            u32 qtag_value: 16;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_pp_tags;
    };
};

/* Internal structure used for sam_pp_tags2. */
struct _sam_pp_tags2 {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 stag_value2: 16;
            u32 stag_value3: 16;
#elif defined(__BIG_ENDIAN)
            u32 stag_value3: 16;
            u32 stag_value2: 16;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_pp_tags2;
    };
};

/* Internal structure used for sam_cp_tag2. */
struct _sam_cp_tag2 {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 map_tbl2_pcp_0: 3;
            u32 map_tbl2_pcp_1: 3;
            u32 map_tbl2_pcp_2: 3;
            u32 map_tbl2_pcp_3: 3;
            u32 map_tbl2_pcp_4: 3;
            u32 map_tbl2_pcp_5: 3;
            u32 map_tbl2_pcp_6: 3;
            u32 map_tbl2_pcp_7: 3;
            u32 reserved: 8;
#elif defined(__BIG_ENDIAN)
            u32 reserved: 8;
            u32 map_tbl2_pcp_7: 3;
            u32 map_tbl2_pcp_6: 3;
            u32 map_tbl2_pcp_5: 3;
            u32 map_tbl2_pcp_4: 3;
            u32 map_tbl2_pcp_3: 3;
            u32 map_tbl2_pcp_2: 3;
            u32 map_tbl2_pcp_1: 3;
            u32 map_tbl2_pcp_0: 3;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_icp_tag;
    };
};
#endif

/* Internal structure for SAM Flow Control Action of decryption orencryption. */
struct _sam_fca_macsec {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 flow_type: 2;
            u32 dest_port: 2;
            u32 drop_non_reserved: 1;
            u32 flow_crypt_auth: 1;
            u32 drop_action: 2;
            u32 sa_index: 8;
            u32 rep_prot_fr: 1;
            u32 sa_in_use: 1;
            u32 include_sci: 1;
            u32 vlevel_es_scb: 2;
            u32 tag_bypass_size: 2;
            u32 sa_index_update: 1;
            u32 conf_offset: 7;
            u32 encrypt_frames: 1;
#elif defined(__BIG_ENDIAN)
            u32 encrypt_frames: 1;
            u32 conf_offset: 7;
            u32 sa_index_update: 1;
            u32 tag_bypass_size: 2;
            u32 vlevel_es_scb: 2;
            u32 include_sci: 1;
            u32 sa_in_use: 1;
            u32 rep_prot_fr: 1;
            u32 sa_index: 8;
            u32 drop_action: 2;
            u32 flow_crypt_auth: 1;
            u32 drop_non_reserved: 1;
            u32 dest_port: 2;
            u32 flow_type: 2;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_fca_macsec;
    };
};

/* Internal structure for SAM Flow Control Action of bypass or drop. */
struct _sam_fca_bod {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 flow_type: 2;
            u32 dest_port: 2;
            u32 drop_non_reserved: 1;
            u32 flow_crypt_auth: 1;
            u32 drop_action: 2;
            u32 sa_index: 8;
            u32 reserved: 1;
            u32 sa_in_use: 1;
            u32 reserved2: 14;
#elif defined(__BIG_ENDIAN)
            u32 reserved2: 14;
            u32 sa_in_use: 1;
            u32 reserved: 1;
            u32 sa_index: 8;
            u32 drop_action: 2;
            u32 flow_crypt_auth: 1;
            u32 drop_non_reserved: 1;
            u32 dest_port: 2;
            u32 flow_type: 2;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_fca_bod;
    };
};

/* Internal structure for SAM Flow Control Action of crypt-authentication. */
struct _sam_fca_crypt {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 reserved: 2;
            u32 dest_port: 2;
            u32 drop_non_reserved: 1;
            u32 flow_crypt_auth: 1;
            u32 drop_action: 2;
            u32 sa_index: 8;
            u32 iv: 2;
            u32 conf_offset_7: 1;
            u32 icv_append: 1;
            u32 icv_verify: 1;
            u32 reserved2: 3;
            u32 conf_offset: 7;
            u32 encrypt_frames: 1;
#elif defined(__BIG_ENDIAN)
            u32 encrypt_frames: 1;
            u32 conf_offset: 7;
            u32 reserved2: 3;
            u32 icv_verify: 1;
            u32 icv_append: 1;
            u32 conf_offset_7: 1;
            u32 iv: 2;
            u32 sa_index: 8;
            u32 drop_action: 2;
            u32 flow_crypt_auth: 1;
            u32 drop_non_reserved: 1;
            u32 dest_port: 2;
            u32 reserved: 2;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_fca_crypt;
    };
};

/* Internal structure for SAM Flow Control Action of crypt-authentication. */
struct _sam_fca_cac {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 crypt_auth_bypass_len: 8;
            u32 reserved: 24;
#elif defined(__BIG_ENDIAN)
            u32 reserved: 24;
            u32 crypt_auth_bypass_len: 8;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_fca_cac;
    };
};

/* Internal structure for use for sam_flow_ctrl. */
struct _sam_flow_ctrl {
    u8 flow_index;
    union {
        struct _sam_fca_macsec dec_enc;
        struct _sam_fca_bod bypass_drop;
        struct _sam_fca_crypt crypt_auth;
    };
    struct _sam_fca_cac cabp_len;
};

/* Internal used for structure sam_enable_ctrl. */
struct _sam_ena_ctrl {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u8 sam_index_set: 5;
			u8 reserved1:6;
            u8 set_enable: 1;
            u8 set_all: 1;
            u8 sam_index_clear: 5;
			u8 reserved2:6;
            u8 clear_enable: 1;
            u8 clear_all: 1;
#elif defined(__BIG_ENDIAN)
            u8 clear_all: 1;
            u8 clear_enable: 1;
			u8 reserved2:6;
            u8 sam_index_clear: 5;
            u8 set_all: 1;
            u8 set_enable: 1;
			u8 reserved1:6;
            u8 sam_index_set: 5;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_sam_eec;
    };
};

#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
/* Internal structure used for count_debug1 */
struct _debug_count1 {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 sa_inc_mask: 15;
            u32 reserved: 1;
            u32 sa_octet_inc: 15;
            u32 reserved2: 1;
#elif defined(__BIG_ENDIAN)
            u32 reserved2: 1;
            u32 sa_octet_inc: 15;
            u32 reserved: 1;
            u32 sa_inc_mask: 15;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_dbg_cnt1;
    };
};

/* Internal structure used for count_debug2 */
struct _debug_count2 {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 vlan_inc_mask: 15;
            u32 reserved: 1;
            u32 vlan_octet_inc: 15;
            u32 reserved2: 1;
#elif defined(__BIG_ENDIAN)
            u32 reserved2: 1;
            u32 vlan_octet_inc: 15;
            u32 reserved: 1;
            u32 vlan_inc_mask: 15;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_dbg_cnt2;
    };
};

/* Internal structure used for count_debug4 */
struct _debug_count4 {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 sa_test: 1;
            u32 vlan_test: 1;
            u32 global_test: 1;
            u32 reserved: 1;
            u32 sa_nr: 8;
            u32 vlan_up: 3;
            u32 reserved2: 1;
            u32 global_octet_inc: 15;
            u32 reserved3: 1;
#elif defined(__BIG_ENDIAN)
            u32 reserved3: 1;
            u32 global_octet_inc: 15;
            u32 reserved2: 1;
            u32 vlan_up: 3;
            u32 sa_nr: 8;
            u32 reserved: 1;
            u32 global_test: 1;
            u32 vlan_test: 1;
            u32 sa_test: 1;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_dbg_cnt4;
    };
};
#endif

/* Internal structure used for cntrs_ctrl. */
struct _cntrs_ctrl {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 reset_all: 1;
            u32 debug_access: 1;
            u32 saturate_cntrs: 1;
            u32 auto_cntr_reset: 1;
            u32 reset_summary: 1;
            u32 reserved: 27;
#elif defined(__BIG_ENDIAN)
			u32 reserved: 27;
			u32 reset_summary: 1;
			u32 auto_cntr_reset: 1;
			u32 saturate_cntrs: 1;
			u32 debug_access: 1;
			u32 reset_all: 1;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_cnt_ctrl;
    };
};

/* Internal structure used for count_incen1. */
struct _cntr_inc_en {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 psa_inc_en: 15;
            u32 reserved: 1;
            u32 vlan_inc_en: 15;
            u32 reserved2: 1;
#elif defined(__BIG_ENDIAN)
			u32 reserved2: 1;
			u32 vlan_inc_en: 15;
			u32 reserved: 1;
			u32 sa_inc_en: 15;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_cnt_inc_en;
    };
};

/* Internal structure used for sec_fail_evnt. */
struct _sec_fail_evnt {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 sa_secfail_mask: 15;
            u32 vlan_cntr_update: 1;
            u32 gate_cons_check: 1;
            u32 reserved: 15;
#elif defined(__BIG_ENDIAN)
            u32 reserved: 15;
            u32 gate_cons_check: 1;
            u32 vlan_cntr_update: 1;
            u32 sa_secfail_mask: 15;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_sec_fail;
    };
};

/* Internal structure used for misc_ctrl. */
struct _misc_ctrl {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 mc_latency_fix: 8;
            u32 static_bypass: 1;
            u32 nm_macsec_en: 1;
            u32 validate_frames: 2;
            u32 sectag_after_vlan: 1;
            u32 reserved: 11;
            u32 xform_rec_size: 2;
            u32 mc_latency_fix_m: 6; //Future extension, should be set to zero.
#elif defined(__BIG_ENDIAN)
			/* Future extension, should be set to zero. */
			u32 mc_latency_fix_m: 6;
			u32 xform_rec_size: 2;
			u32 reserved: 11;
			u32 sectag_after_vlan: 1;
			u32 validate_frames: 2;
			u32 nm_macsec_en: 1;
			u32 static_bypass: 1;
			u32 mc_latency_fix: 8;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_misc_ctrl;
    };
};

/* Internal structure used for sanm_flow_ctrl per frame type */
struct _sanm_fca_pft {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 utag_flow_type: 1;
            u32 utag_reserved: 1;
            u32 utag_dest_port: 2;
            u32 utag_drop_non_res: 1;
            u32 utag_reserved2: 1;
            u32 utag_drop_action: 2;

            u32 tag_flow_type: 1;
            u32 tag_reserved: 1;
            u32 tag_dest_port: 2;
            u32 tag_drop_non_res: 1;
            u32 tag_reserved2: 1;
            u32 tag_drop_action: 2;

            u32 btag_flow_type: 1;
            u32 btag_reserved: 1;
            u32 btag_dest_port: 2;
            u32 btag_drop_non_res: 1;
            u32 btag_reserved2: 1;
            u32 btag_drop_action: 2;

            u32 ktag_flow_type: 1;
            u32 ktag_reserved: 1;
            u32 ktag_dest_port: 2;
            u32 ktag_drop_non_res: 1;
            u32 ktag_reserved2: 1;
            u32 ktag_drop_action: 2;
#elif defined(__BIG_ENDIAN)
			u32 ktag_drop_action: 2;
			u32 ktag_reserved2: 1;
			u32 ktag_drop_non_res: 1;
			u32 ktag_dest_port: 2;
			u32 ktag_reserved: 1;
			u32 ktag_flow_type: 1;
			
			u32 btag_drop_action: 2;
			u32 btag_reserved2: 1;
			u32 btag_drop_non_res: 1;
			u32 btag_dest_port: 2;
			u32 btag_reserved: 1;
			u32 btag_flow_type: 1;
			
			u32 tag_drop_action: 2;
			u32 tag_reserved2: 1;
			u32 tag_drop_non_res: 1;
			u32 tag_dest_port: 2;
			u32 tag_reserved: 1;
			u32 tag_flow_type: 1;
			
			u32 utag_drop_action: 2;
			u32 utag_reserved2: 1;
			u32 utag_drop_non_res: 1;
			u32 utag_dest_port: 2;
			u32 utag_reserved: 1;
			u32 utag_flow_type: 1;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_sanm_fc;
    };
};

/* Internal structure used for outer VLAN match in cp_match_parm. */
struct _cpc_vlan_par {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 reserved: 16;
            u32 vlan_id: 12;
            u32 vlan_up: 3;
            u32 disable_up: 1;
#elif defined(__BIG_ENDIAN)
            u32 disable_up: 1;
            u32 vlan_up: 3;
            u32 vlan_id: 12;
            u32 reserved: 16;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32  raw_cpc_vlan;
    };
};

/* Internal structure used for cp_match_parm. */
struct _cp_match_mode {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 vlan_sel_0_7: 8;
            u32 e_type_sel_0_9: 10;
            u32 reserved: 3;
            u32 e_type_sel_10_17: 8;
            u32 reserved2: 3;
#elif defined(__BIG_ENDIAN)
            u32 reserved2: 3;
            u32 e_type_sel_10_17: 8;
            u32 reserved: 3;
            u32 e_type_sel_0_9: 10;
            u32 vlan_sel_0_7: 8;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_cpm_mode;
    };
};

/* Internal structure used for cpm_mode_ena. */
struct _cp_match_ena {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 mac_da_0_7: 8;
            u32 e_type_0_7: 8;
            u32 combined_8_9: 2;
            u32 dmac_range: 1;
            u32 dmac_const_44: 1;
            u32 dmac_const_48: 1;
            u32 e_type_10_17:8;
            u32 reserved: 3;
#elif defined(__BIG_ENDIAN)
            u32 reserved: 3;
            u32 dmac_const_48: 1;
            u32 dmac_const_44: 1;
            u32 dmac_range: 3;
            u32 combined_8_9: 2;
            u32 e_type_0_7: 8;
            u32 mac_da_0_7: 8;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_cpm_enable;
    };
};

/* Internal structure used for misc params of icc_match_parm. */
struct _iccm_misc_par {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 sai_hit: 1;
            u32 vlan_valid: 1;
            u32 etype_valid: 1;
            u32 ctrl_packet: 1;
            u32 sa_hit_mask: 1;
            u32 sa_nr_mask: 1;
            u32 vlan_valid_mask: 1;
            u32 vlan_id_mask: 1;
            u32 etype_valid_mask: 1;
            u32 payl_etype_mask: 1;
            u32 ctrl_packet_mask: 1;
            u32 rule_nr_mask: 1;
            u32 match_priority: 3;
            u32 match_action: 1;
            u32 sa_or_rule_nr: 8;
            u32 vlan_up_inner_mask: 1;
            u32 vlan_id_inner_mask: 1;
            u32 rule_hit: 1;
            u32 rule_hit_mask: 1;
            u32 reserved: 4;
#elif defined(__BIG_ENDIAN)
            u32 reserved: 4;
            u32 rule_hit_mask: 1;
            u32 rule_hit: 1;
            u32 vlan_id_inner_mask: 1;
            u32 vlan_up_inner_mask: 1;
            u32 sa_or_rule_nr: 8;
            u32 match_action: 1;
            u32 match_priority: 3;
            u32 rule_nr_mask: 1;
            u32 ctrl_packet_mask: 1;
            u32 payl_etype_mask: 1;
            u32 etype_valid_mask: 1;
            u32 vlan_id_mask: 1;
            u32 vlan_valid_mask: 1;
            u32 sa_nr_mask: 1;
            u32 sa_hit_mask: 1;
            u32 ctrl_packet: 1;
            u32 etype_valid: 1;
            u32 vlan_valid: 1;
            u32 sai_hit: 1;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_iccm_misc;
    };
};

/* Internal structure used for inner VLAN info of icc_match_parm. */
struct _iccm_inr_vlan {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 inr_vlan_id: 12;
            u32 inr_vlan_up: 3;
            u32 reserved: 17;
#elif defined(__BIG_ENDIAN)
            u32 reserved: 17;
            u32 inr_vlan_up: 3;
            u32 inr_vlan_id: 12;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_iccm_ivl;
    };
};

/* Internal structure used for outer VLAN, payload eth-type of icc_match_parm. */
struct _iccm_ovlan_etht {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 vlan_id: 12;
            u32 reserved: 4;
            u32 payload_e_type: 16;
#elif defined(__BIG_ENDIAN)
            u32 payload_e_type: 16;
            u32 reserved: 4;
            u32 vlan_id: 12;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_iccm_ovl_eth;
    };
};

/* Internal structure used for ICC flow control of non match packets of
	Control and non-Control frames. */
struct _icc_nm_fc {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 reserved: 14;
            u32 nm_act_cp: 1;
            u32 nm_act_ncp: 1;
            u32 reserved2: 16;
#elif defined(__BIG_ENDIAN)
			u32 reserved2: 16;
			u32 nm_act_ncp: 1;
			u32 nm_act_cp: 1;
			u32 reserved: 14;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_icc_nm_fc;
    };
};

/* Internal structure used for ICC flow control of ether-type field compare
	value for the etype_valid comparison. */
struct _icc_etype_mlen {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 reserved: 16;
            u32 etype_max_len: 16;
#elif defined(__BIG_ENDIAN)
            u32 etype_max_len: 16;
            u32 reserved: 16;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_icc_eth_len;
    };
};

/* Internal structure used for ccm_ena_ctrl. */
struct _ccm_ena_ctrl {
    union {
        struct {
#if defined(__LITTLE_ENDIAN)
            u32 cc_index_set: 8;
            u32 reserved: 6;
            u32 set_enable: 1;
            u32 set_all: 1;
            u32 cc_index_clear: 8;
            u32 reserved2: 6;
            u32 clear_enable: 1;
            u32 clear_all: 1;
#elif defined(__BIG_ENDIAN)
            u32 clear_all: 1;
            u32 clear_enable: 1;
            u32 reserved2: 6;
            u32 cc_index_clear: 8;
            u32 set_all: 1;
            u32 set_enable: 1;
            u32 reserved: 6;
            u32 cc_index_set: 8;
#else
#error "__LITTLE_ENDIAN or __BIG_ENDIAN must be defined"
#endif
        };
        u32 raw_icc_eec;
    };
};

#endif /* _EIP160S_INTERNAL_H_ */
