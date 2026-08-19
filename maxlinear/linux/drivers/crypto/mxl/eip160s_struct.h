/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Driver for MACsec Engine inside Intel CHD GSWIP or GPHY
 * Copyright 2020 - 2022 Maxlinear, Inc.
 * Copyright 2019 - 2020 Intel Corporation
 */

#ifndef _EIP160S_STRUCT_H_
#define _EIP160S_STRUCT_H_

#include <net/eip160.h>

#define PRINT_DEBUG		0
#define EN_MSEC_DEBUG_ACCESS	0
#define SUPPORT_MACSEC_VLAN	0

/**********************
	Constant
 **********************/

// Max number of sync retry
#define IN_FLIGHT_BUSY_RETRY 100

/*  Ethertype for MACSec frames. */
#define ETH_PROTO_MACSEC 0x88E5
/*  Ethertype for MACSec frames - byte swapped of ETH_PROTO_MACSEC */
#define BYTE_SWAP_16BIT(A) ((((A) & 0xFF00) >> 8) | (((A) & 0x00FF) << 8))
#define ETH_PROTO_MACSEC_BYTE_SWAPPED BYTE_SWAP_16BIT(ETH_PROTO_MACSEC)

/*  EIP-160 Signature */
#define EIP160_NR_CNR 0x5FA0u
/*  EIP-62 Signature */
#define EIP62_NR_CNR 0xC13Eu
/*  Secure Channel Id size */
#define MACSEC_SCI_LEN_BYTE 8
/*  Secure Channel Id size */
#define MACSEC_SCI_LEN_WORD 2

/*  Key size for 128-bit CA type */
#define MACSEC_CA128_KEY_LEN_BYTE 16
/*  Key size for 256-bit CA type */
#define MACSEC_CA256_KEY_LEN_BYTE 32
/*  Key size for 128-bit CA type */
#define MACSEC_CA128_KEY_LEN_WORD 4
/*  Key size for 256-bit CA type */
#define MACSEC_CA256_KEY_LEN_WORD 8
/*  Max Key size among all CA types */
#define MACSEC_KEY_LEN_BYTE 32

/*  H-Key size */
#define MACSEC_HKEY_LEN_BYTE 16
/*  H-Key size */
#define MACSEC_HKEY_LEN_WORD 4
/*  Context Salt size */
#define MACSEC_CSALT_LEN_WORD 3

/*  Short SCI size */
#define MACSEC_SSCI_LEN_BYTE 4
/*  Context Salt size */
#define MACSEC_SALT_LEN_BYTE 12

/*  Max number of SA rules supported */
#define MACSEC_MAX_SA_RULES 32

/*  Base Address of EIP_160S_IG_TR_PAGE0 */
#define EIP_160S_IG_TR_PAGE0_BASE 0x00000000u
/*  Base Address of EIP_160S_EG_TR_PAGE0 */
#define EIP_160S_EG_TR_PAGE0_BASE 0x00000000u

/*  Association Number */
#define MACSEC_MAX_AN 4
/*  CCW Fields */
/*  Field SN - Sequence Number */
#define CCW_EIPX_TR_CCW_SN_POS 28
/*  Field SN - Sequence Number */
#define CCW_EIPX_TR_CCW_SN_MASK 0x03000000u
/*  Field AN - Association Number */
#define CCW_EIPX_TR_CCW_AN_POS 26
/*  Field AN - Association Number */
#define CCW_EIPX_TR_CCW_AN_MASK 0x0C000000u
/*  Field CA - Crypto algorithm */
#define CCW_EIPX_TR_CCW_CA_POS 17
/*  Field CA - Crypto algorithm */
#define CCW_EIPX_TR_CCW_CA_MASK 0x000E0000u

/*  SA Update Control Fields */
/*  Field AN - Index of the next SA.  */
#define SA_UPDATE_SA_INDEX_POS 0
/*  Field AN - Index of the next SA */
#define SA_UPDATE_SA_INDEX_MASK 0x00003FFFu
/*  Field AN - SA expired summary register */
#define SA_UPDATE_SA_EXPIRED_IRQ_POS 14
/*  Field AN - SA expired summary register */
#define SA_UPDATE_SA_EXPIRED_IRQ_MASK 0x00004000u
/*  Field AN - Next SA index valid */
#define SA_UPDATE_SA_INDEX_VALID_POS 15
/*  Field AN - Next SA index valid */
#define SA_UPDATE_SA_INDEX_VALID_MASK 0x00008000u
/*  Field AN - Index of the flow control that holds the current SA */
#define SA_UPDATE_FLOW_INDEX_POS 16
/*  Field AN - Index of the flow control that holds the current SA */
#define SA_UPDATE_FLOW_INDEX_MASK 0x7FFF0000u
/*  Field AN - SA index update enable */
#define SA_UPDATE_SA_INDEX_UPDATE_EN_POS 31
/*  Field AN - SA index update enable */
#define SA_UPDATE_SA_INDEX_UPDATE_EN_MASK 0x80000000u

/*  TR context_size in words */
#define ING_MAX_TR_SIZE_WORDS 20
/*  TR context_size in bytes */
#define ING_MAX_TR_SIZE_BYTES (ING_MAX_TR_SIZE_WORDS * 4)

/*  TR context_size in words */
#define EGR_MAX_TR_SIZE_WORDS 24
/*  TR context_size in bytes */
#define EGR_MAX_TR_SIZE_BYTES (EGR_MAX_TR_SIZE_WORDS * 4)

/*  SAM params size in words */
#define IE_MAX_SAM_SIZE_WORDS 16
/*  SAM params size in bytes */
#define IE_MAX_SAM_SIZE_BYTES (IE_MAX_SAM_SIZE_WORDS * 4)

/*  Ingress Consistency Check params size in words */
#define ICC_MAX_MATCH_SIZE_WORDS 4
/*  Ingress Consistency Check params size in bytes */
#define ICC_MAX_MATCH_SIZE_BYTES (ICC_MAX_MATCH_SIZE_WORDS * 4)

/*  CPC params size in bytes */
#define IE_MAX_CPC_SIZE_BYTES 16
/*  CPC rule for DA + EHT size in bytes */
#define IE_CPC_DA_ETH_SIZE_BYTES 8
/*  CPC rule for EHT size in bytes */
#define IE_CPC_ETH_SIZE_BYTES 4

/*  SAM FCA size in bytes */
#define IE_MAX_SAM_FCA_BYTES 4

/*  SA statistic size in words */
#define MAX_SA_STATS_SIZE_WORDS 32
/*  SA statistic size in bytes */
#define MAX_SA_STATS_SIZE_BYTES (MAX_SA_STATS_SIZE_WORDS * 4)
/*  SA statistic size in words */
#define MAX_VLAN_COUNT_SUMM_WORDS 2
/*  Max Ingress SA counters */
#define MAX_ING_SA_COUNTERS 11
/*  Max Egress SA counters */
#define MAX_EGR_SA_COUNTERS 4
/*  Max VLAN counters for both Ingress & Egress */
#define MAX_VLAN_COUNTERS 5
/*  Max Ingress GLOBAL counters */
#define MAX_ING_GLOBAL_COUNTERS 14
/*  Max Egress GLOBAL counters */
#define MAX_EGR_GLOBAL_COUNTERS 5
/*  Max VLAN user priorities */
#define MAX_VLAN_USER_PRIORITIES 8

/*  Max entries in Control Packet Classification table */
#define MAX_CTRL_PKT_CLASS_ENTRIES 18

/*  Max Counter sync retry count */
#define EIP160_MAX_SYNC_RETRY_COUNT 100

/*  Ingress default CCW word for 32-bit PN & 128-bit AES type */
#define ING_32BPN_128KEY_DEF_CCW 0xD24BE06F
/*  Ingress default CCW word for 32-bit PN & 256-bit AES type */
#define ING_32BPN_256KEY_DEF_CCW 0xD24FE06F
/*  Ingress default CCW word for 64-bit PN & 128-bit AES type */
#define ING_64BPN_128KEY_DEF_CCW 0xE24BA0EF
/*  Ingress default CCW word for 64-bit PN & 256-bit AES type */
#define ING_64BPN_256KEY_DEF_CCW 0xE24FA0EF

/*  AN is variable field and should be inserted */
/*  Egress default CCW word for 32-bit PN & 128-bit AES type */
#define EGR_32BPN_128KEY_DEF_CCW 0x924BE066
/*  Egress default CCW word for 32-bit PN & 256-bit AES type */
#define EGR_32BPN_256KEY_DEF_CCW 0x924FE066
/*  Egress default CCW word for 64-bit PN & 128-bit AES type */
#define EGR_64BPN_128KEY_DEF_CCW 0xA24BE066
/*  Egress default CCW word for 64-bit PN & 256-bit AES type */
#define EGR_64BPN_256KEY_DEF_CCW 0xA24FE066

/*  Default register values. */
/*  Default value of SAM_NM_PARAMS reg. */
#define DEF_SAM_NM_PARAMS 0xE588007F

static inline int IG_REG_READ(struct eip160_pdev *pdev, u32 off, u32 *pdata)
{
	return pdev->ig_reg_rd(&pdev->ctx, off, pdata);
}

static inline int IG_REG_WRITE(struct eip160_pdev *pdev, u32 off, u32 data)
{
	return pdev->ig_reg_wr(&pdev->ctx, off, data);
}

static inline int EG_REG_READ(struct eip160_pdev *pdev, u32 off, u32 *pdata)
{
	return pdev->eg_reg_rd(&pdev->ctx, off, pdata);
}

static inline int EG_REG_WRITE(struct eip160_pdev *pdev, u32 off, u32 data)
{
	return pdev->eg_reg_wr(&pdev->ctx, off, data);
}

/*  MACsec direction. */
enum msec_dir {
    /*  Ingress (decryption) direction. */
    INGRESS_DIR = 0,
    /*  Egress (encryption) direction. */
    EGRESS_DIR = 1,
};

/*  EIP160 engine configuration information. */
struct eip160_config {
    /*  The max number of SA match & it's control sets supported. */
    u8 nr_of_sas;
    /*  (-i) The max number of Consistency Check match sets supported. */
    u8 consist_checks;
    /*  (-i)Enable SCI matching */
    u8 match_sci: 1;
    /*  Number of statistics counters per SA (11 for –i, 4 for –e). */
    u8 sa_counters: 4;
    /*  Number of statistics counters per VLAN User Priority (5 for –i, 
    5 for –e). */
    u8 vl_counters: 4;
    /*  Number of global statistics counters (14 for –i, 5 for –e). */
    u8 global_counters: 6;
    /*  Programmable locking of counters at maximum value. */
    u8 saturate_cntrs: 1;
    /*  Programmable enabling of statistics counters reset on read. */
    u8 auto_cntr_reset: 1;
    /*  Number of octet counters per SA. */
    u8 sa_octet_ctrs: 2;
    /*  Number of octet counters per VLAN User Priority. */
    u8 vl_octet_ctrs: 2;
    /*  Number of global octet counters. */
    u8 glob_octet_ctrs: 4;
    /*  Specialize the engine for ingress  & counters only. */
    u8 ingress_only: 1;
    /*  Specialize the engine for egress & counters only. */
    u8 egress_only: 1;
    /*  Bypassing VLAN tags before SecTAG is supported. */
    u8 tag_bypass: 1;
} ;

/*  EIP160 engine version information. */
struct eip160_version {
    /*  EIP number value, this is the EIP-160, so the value is 0xA0. */
    u8 eip_nr;
    /*  The one's complement of EIP number value, is 0x5F. */
    u8 eip_nr_complement;
    /*  Patch level for hardware module. */
    u8 patch_level: 4;
    /*  Minor version of hardware module. */
    u8 minor_version: 4;
    /*  Major version of hardware module. */
    u8 major_version: 4;
};

/*  EIP62 engine configuration information. */
struct eip62_config {
    /*  Minor HW revision number of EIP-62. */
    u8 minor_revision: 4;
    /*  Major HW revision number of EIP-62. */
    u8 major_revision: 4;
    /*  Optimized for FPGA implementation. */
    u8 fpga_solution: 1;
    /*  “Galois Field” S-boxes are not used in the AES cores. */
    u8 gf_sboxes: 1;
    /*  “Lookup table” S-boxes are used in the AES cores. */
    u8 lookup_sboxes: 1;
    /*  Core only includes AES as crypto-algorithm. */
    u8 macsec_aes_only: 1;
    /*  AES cores are available. */
    u8 aes_present: 1;
    /*  Feedback modes are available for the AES cores. */
    u8 aes_fb: 1;
    /*  Speed of the AES engine. 1 for the “s” configuration (1 AES 
    engine). */
    u8 aes_speed: 4;
    /*  AES-192 is not supported for the “s” and “d” configurations. */
    u8 aes192: 1;
    /*  AES-256 is supported for all configurations. */
    u8 aes256: 1;
    /*  Number of “parameter” bits passed through the EIP-62 pipeline 
    with the end-of-packet, 2 here. */
    u8 eop_param_bits: 3;
    /*  The harwdware can handle handle tag bypassing in front of SecTAG.
    */
    u8 sec_offset: 1;
    /*  GHASH core is available. */
    u8 ghash_present: 1;
	/*  Speed indicator for GHASH core: 3-cycle core (“s” configuration).
	*/
	u8 ghash_speed: 1;
} ;

/*  EIP-62 Transform engine module version information. */
struct eip62_version {
    /*  EIP number, this is the EIP-62, so the value is 0x3E. */
    u8 eip_nr;
    /*  The one's complement EIP number value, is 0xC1. */
    u8 eip_nr_complement;
    /*  Patch level for hardware module. */
    u8 patch_level: 4;
    /*  Minor version of hardware module. */
    u8 minor_version: 4;
    /*  Major version of hardware module. */
    u8 major_version: 4;
};

/*  Advanced Interrupt Controller (AIC) module configuration data. */
struct eip201_config {
    /*  Number of interrupt inputs supported. */
    u8 number_of_inputs;
};

/*  Advanced Interrupt Controller (AIC) module version information. */
struct eip201_version {
    /*  EIP number, this is the EIP-201, so the value is 0xC9. */
    u8 eip_nr;
    /*  The one's complement EIP number value, is 0x36. */
    u8 eip_nr_complement;
    /*  Patch level for hardware module. */
    u8 patch_level: 4;
    /*  Minor version of hardware module. */
    u8 minor_version: 4;
    /*  Major version of hardware module. */
    u8 major_version: 4;
};

/*  MACSEC HW capabilities information. */
struct eip160_hw_cap {
    /*  EIP160 module configuration information. */
    struct eip160_config eip_cfg;
	/*  EIP-62 Transform-Engine module configuration information. */
	struct eip62_config ten_cfg;
    /*  EIP1201 (AIC) module configuration information. */
    struct eip201_config aic_opt;

    /*  EIP160 module version information. */
    struct eip160_version eip_ver;	
	/*  EIP-62 Transform-Engine module version information. */
	struct eip62_version ten_ver;
    /*  EIP1201 (AIC) module version information. */
    struct eip201_version aic_ver;
};

/*  Crypto Algorithm for data encryption.  */
enum crypto_algo {
    /*  AES-CTR-128. */
    CA_AES_CTR_128 = 5,
    /*  AES-CTR-192
    Not supported for GPY2xx. */
    CA_AES_CTR_192 = 6,
    /*  AES-CTR-256. */
    CA_AES_CTR_256 = 7,
};

/*  Sequence Number type. */
enum sn_type {
    /*  32-bit sequence number. */
    SN_32_BIT = 1,
    /*  64-bit sequence number. */
    SN_64_BIT = 2,
};

/*  Context Control word. */
struct ctx_ctrl_word {
    /*  (-e)Association Number inserted into the SecTag for egress. */
    u8 an: 2;
    /*  Sequence number type.
    Valid values are defined in \ref sn_type enum. */
    enum sn_type sn_type;
    /*  Crypto algorithm type.
    Valid values are defined in \ref crypto_algo enum. */
    enum crypto_algo ca_type;
};

/*  Context Salt (applicable only for 64-bit PN). */
struct ctx_salt {
    /*  The Salt. */
    u8 salt[MACSEC_SALT_LEN_BYTE];
    /*  Short-SCI to xor-ed with Salt-0. */
    u8 s_sci[MACSEC_SSCI_LEN_BYTE];
};

/*  Cryptographic parameters. */
struct crypto_parms {
    /*  AES encryption/decryption key for given MACsec SA. */
    u8 key[MACSEC_KEY_LEN_BYTE];
    /*  Hash key for authentication operation (internally derived from 
    key). */
    u8 hkey[MACSEC_HKEY_LEN_BYTE];
    union {
        struct {
            /*  LS 32bits of 64-bit SCI (code internal use). */
            u32 sci0;
            /*  MS 32bits of 64-bit SCI (code internal use). */
            u32 sci1;
        };
        /*  The SCI that belongs to the specific MACsec SA
        (NA for 64-bit PN Ingress case). */
        sci_t scid;
    };
    /*  Context Salt \ref ctx_salt. */
    struct ctx_salt c_salt;
};

/*  Packet Numbering and Replay check. */
struct pn_replay_check {
    union {
        struct {
            /*  LS 32bits of 64-bit SN (code internal use). */
            u32 seq0;
            /*  MS 32bits of 64-bit SN (code internal use). */
            u32 seq1;
        };
        /*  Sequence Number. */
        u64 seq_num;
    };
    /*  (-i)Replay Window size for ingress sequence number checking
    Value 0 is enforced for strict ordering. */
    u32 mask;
};

/*  (-e)Update SA Control Word of given transform record. */
struct update_sa_cw {
    /*  Index of the current SA to update the SA CW. */
    u8 sa_index:5;
    /*  Index of next SA installed to switch if current SA expired. */
    u8 next_sa_index:5;
    /*  To annotation into the SA expired summary register generate a
    subsequent interrupt when the SA expires regardless whether the next SA is 
    available. */
    u8 sa_expired_irq:1;
    /*  Next SA index is installed and valid to use. */
    u8 sa_index_valid:1;
    /*  Index of the flow control register that holds the current SA.
    In case it is updated, it gets the next SA index. */
    u8 flow_index:5;
    /*  Enable updating SA index when the Packet Number for the current
    transform record has expired. */
    u8 sa_ind_update_en:1;
};

/*  Transform Record containing Secure Association parameters. */
struct transform_rec {
    /*  Transform Record entry index. */
    u8 sa_index;
    /*  Context control word \ref ctx_ctrl_word. */
    struct ctx_ctrl_word ccw;
    /*  Cryptographic parameters \ref crypto_parms. */
    struct crypto_parms cp;
    /*  Packet numbering and replay check \ref pn_replay_check. */
    struct pn_replay_check pn_rc;
    /*  (-e)SA update control word \ref update_sa_cw. */
    struct update_sa_cw sa_cw;
};

/*  Next Packet Number in Transform Record. */
struct sa_next_pn {
    /*  Transform Record entry index. */
    u8 sa_index;
    /*  Next Sequence Number. */
    u64 next_pn;
};

/*  Transform Record to clear. */
struct clr_trans_rec {
	/*  Transform Record entry index. */
    u8 sa_index;
};

/*  Secure Association Matching (SAM) Misc params. */
struct sam_misc_parm {
    /*  Packet is control packet (as pre-decoded) compare value. */
    u8 control_packet: 1;
    /*  Allow packets without a MACsec tag to match. */
    u8 untagged: 1;
    /*  Allow packets with a standard and valid MACsec tag to match. */
    u8 tagged: 1;
    /*  Allow packets with an invalid MACsec tag to match. */
    u8 bad_tag: 1;
    /*  Allow packets with a MACsec tag indicating KaY handling to be
    done to match. */
    u8 kay_tag: 1;
    /*  Source port compare value \ref sam_fca_dpt. */
    u8 source_port: 2;
    /*  Priority of this entry for determining the actual transform
    used on a match when multiple entries match. */
    u8 match_priority: 4;
    /*  MACsec TCI/AN byte compare value. */
    u8 macsec_tci_an: 8;
};

/*  Secure Association Matching (SAM) Mask params. */
struct sam_mask_parm {
    /*  Enable MAC_SA bits (mask bit per byte). */
    u8 mac_sa_mask: 6;
    /*  Enable MAC_DA bits (mask bit per byte). */
    u8 mac_da_mask: 6;
    /*  Enable parsed Ether-type field. */
    u8 mac_etype_mask: 1;
    /*  Enable source port. */
    u8 source_port_mask: 1;
    /*  Enable control packet status. */
    u8 ctrl_packet_mask: 1;
    /*  Enable MACsec header SCI. */
    u8 macsec_sci_mask: 1;
    /*  Enable MACSec TCI/AN (only for MACsec tagged frames, individually
    masked). */
    u8 tci_an_mask: 8;
#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
    /*  Enable parsed VLAN valid flag. */
    u8 vlan_valid_mask: 1;
    /*  Enable parsed QinQ found flag. */
    u8 qinq_found_mask: 1;
    /*  Enable parsed STAG valid flag. */
    u8 stag_valid_mask: 1;
    /*  Enable parsed QTAG valid flag. */
    u8 qtag_valid_mask: 1;
    /*  Enable parsed VLAN User Priority value. */
    u8 vlan_up_mask: 1;
    /*  Enable parsed VLAN ID value. */
    u8 vlan_id_mask: 1;
    /*  Enable parsed inner VLAN User Priority value. */
    u8 vlan_up_inner_mask: 1;
    /*  Enable parsed inner VLAN ID value. */
    u8 vlan_id_inner_mask: 1;
#endif
};

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
/*  Secure Association Matching (SAM) extended params. */
struct sam_vlan_parm {
    /*  Parsed outer VLAN User Priority. */
    u8 vlan_up: 3;
    /*  Parsed inner VLAN User Priority. */
    u8 vlan_up_inner: 3;
    /*  Parsed outer VLAN ID. */
    u16 vlan_id: 12;
    /*  Parsed inner VLAN ID. */
    u16 vlan_id_inner: 12;
    /*  Parsed VLAN valid flag compare value. */
    u8 vlan_valid: 1;
    /*  Parsed QinQ found flag compare value. */
    u8 qinq_found: 1;
    /*  Parsed STAG valid flag compare value. */
    u8 stag_valid: 1;
    /*  Parsed QTAG valid flag compare value. */
    u8 qtag_valid: 1;
};
#endif

/*  Secure Association Matching (SAM) rule params. */
struct sa_match_parm {
    /*  SA match rule number */
    u8 rule_index;
    /*  Index of the flow control register that defines an action for
    the corresponding SA matching rule number. */
    u8 flow_index: 5;
    /*  Src MAC address. */
    u8 sa_mac[6];
    /*  Dst MAC address. */
    u8 da_mac[6];
    /*  Parsed Ether-type. */
    u16 eth_type;
    union {
        struct {
	    /*  (-i)The SCI */
            /*  LS 32bits of 64-bit SCI (code internal use). */
            u32 sci0;
            /*  MS 32bits of 64-bit SCI (code internal use). */
            u32 sci1;			
        };
        /*  (-i)The SCI (compared only for MACsec tagged frames). */
        sci_t scid;
    };
#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
    /*  Secure Association Matching (SAM) extended params. */
    struct sam_vlan_parm vlan_par;
#endif
    /*  Miscellaneous (bit-fields) match \ref sam_misc_parm. */
    struct sam_misc_parm misc_par;
    /*  SAM Mask (bit-fields) \ref sam_mask_parm. */
    struct sam_mask_parm mask_par;
};

/*  Secure Association Matching (SAM) rule params to clear. */
struct clr_sam_par {
	/*  SA match rule number */
    u8 rule_index;
};

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
/*  VLAN tag parsing parameters for SA matching logic. */
struct vlan_parse_parm {
    /*  Take care of packets with two VLAN tags. */
    u8 parse_qinq: 1;
    /*  Take care of packets with an “802.1s” VLAN tag. */
    u8 parse_stag: 1;
    /*  Take care of packets with an “802.1Q” VLAN tag. */
    u8 parse_qtag: 1;
    /*  Enable user priority processing for “802.1s” packets. */
    u8 stag_up_en: 1;
    /*  Enable user priority processing for “802.1Q” packets. */
    u8 qtag_up_en: 1;
    /*  Translation table to derive the user priority from the outer PCP 
    field in “802.1Q” tags. Index 0 for PCP-0, so on. */
    u8 map_tbl_opcp_x[8];
    /*  Translation table to derive the user priority from the inner PCP 
    field in “802.1QinQ” tags. Index 0 for PCP-0, so on. */
    u8 map_tbl_ipcp_x[8];
    /*  Default user priority, assigned to non-VLAN packets and to VLAN 
    packets for which the VLAN user priority processing is disabled. */
    u8 def_up: 3;
    /*  Ether-type value used for “802.1Q” tag type. */
    u16 qtag_value: 16;
    /*  Ether-type value used for “802.1s” tag type. */
    u16 stag_value: 16;
    /*  Secondary ether-type value used for “802.1s” tag type. */
    u16 stag_value2: 16;
    /*  Tertiary ether-type value used for “802.1s” tag type. */
    u16 stag_value3: 16;
};
#endif

/*  Secure Association Matching (SAM) Flow Control Action (FCA) flow type. */
enum sam_fca_ft {
    /*  Bypass */
    SAM_FCA_FLOW_BYPASS = 0,
    /*  Drop */
    SAM_FCA_FLOW_DROP = 1,
    /*  MACsec ingress */
    SAM_FCA_FLOW_INGRESS = 2,
    /*  MACsec egress */
    SAM_FCA_FLOW_EGRESS = 3,
};

/*  Secure Association Matching (SAM) Flow Control Action (FCA) dest-port type. 
*/
enum sam_fca_dpt {
    /*  Common port. */
    SAM_FCA_DPT_COMMON = 0,
    /*  Reserved port. */
    SAM_FCA_DPT_RES = 1,
    /*  Control port. */
    SAM_FCA_DPT_CONTROL = 2,
    /*  Un-Control port. */
    SAM_FCA_DPT_UNCONTROL = 3,
};

/*  Secure Association Matching (SAM) Flow Control Action (FCA) drop-action 
type. */
enum sam_fca_dat {
    /*  Bypass with CRC corruption indicator. */
    SAM_FCA_DROP_AS_CRC = 0,
    /*  Bypass with bad packet indicator. */
    SAM_FCA_DROP_AS_BAD_PKT = 1,
    /*  Internal drop by crypto-core. */
    SAM_FCA_DROP_AS_INTERNAL = 2,
    /*  do not drop (for debugging only). */
    SAM_FCA_NO_DROP = 3,
};

/*  Secure Association Matching (SAM) Flow Control Action (FCA) 
frame-validation type. */
enum sam_fca_fvt {
    /*  Disabled */
    SAM_FCA_VALIDATE_DIS = 0,
    /*  Check */
    SAM_FCA_VALIDATE_CHECK = 1,
    /*  Strict */
    SAM_FCA_VALIDATE_STRICT = 2,
    /*  Reserved. */
    SAM_FCA_VALIDATE_RES = 3,
};

/*  Secure Association Matching (SAM) Flow Control Action (FCA) Integrity 
Vector type. */
enum sam_fca_iv {
    /*  IV is loaded via the transform record. */
    SAM_FCA_LOAD_IV_VIA_TR = 0,
    /*  IV is loaded via input frame located in front of the frame. */
    SAM_FCA_LOAD_IV_VIA_START_INFRAME = 1,
    /*  IV is loaded via input frame located at end of the frame. */
    SAM_FCA_LOAD_IV_VIA_END_INFRAME = 2,
    /*  First three IV words are loaded via the input frame. */
    SAM_FCA_LOAD_IV3_VIA_INFRAME = 3,
};

/*  Secure Association Matching (SAM) Flow Control Action (FCA) of 
Drop/Bypass/Ing/Egr/Crypt_Auth params. */
struct sam_flow_ctrl {
    /*  Flow control action index to be configured. */
    u8 flow_index;
    /*  Index of the associated transform record (SA). */
    u8 sa_index: 5;
    /*  Flow type (action type).
    Valid values are defined in \ref sam_fca_ft enum. */
    enum sam_fca_ft flow_type;
    /*  Destination port.
    Valid values are defined in \ref sam_fca_dpt enum. */
    enum sam_fca_dpt dest_port;
    /*  (-i)Perform 'drop_action' if packet is not from reserved port. */
    u8 drop_non_reserved: 1;
    /*  Enables basic crypt-authenticate mode. */
    u8 flow_crypt_auth: 1;
    /*  Drop operation type.
    Valid values are defined in \ref sam_fca_dat enum. */
    enum sam_fca_dat drop_action;
    union {
        /*  (-i)Enable replay protection. */
        u8 replay_protect: 1;
        /*  (-e)Enable frame  protection. */
        u8 protect_frames: 1;
    };
    /*  (-crypto)IV load type.
    Valid values are defined in \ref sam_fca_iv enum. */
    enum sam_fca_iv iv;
    /*  MACsec SA inUse for the looked-up SA. */
    u8 sa_in_use: 1;
    /*  (-e)Include explicit SCI in packet. */
    u8 include_sci: 1;
    /*  (-i)Frame validation level. This value is used for 802.1AE
    compliant processing when MACsec secure channel is found.
    Valid values are defined in \ref sam_fca_fvt enum. */
    enum sam_fca_fvt validate_frames;
    /*  (-crypto) Bit 7 of the AAD length for AES-GCM operations. */
    u8 conf_offset_7: 1;
    union {
        /*  (-e)ES (End Station) bit in TCI field. */
        u8 use_es: 1;
        /*  (-e)(-crypto)Append the calculated ICV. */
        u8 icv_append: 1;
    };
    union {
        /*  (-e)SCB (Single Copy Broadcast) bit in TCI field. */
        u8 use_scb: 1;
        /*  (-i)(-crypto)Retrieve and verify the ICV against the
        calculated internally. */
        u8 icv_verify: 1;
    };
    /*  (-e)The number of 4-byte tags to be bypassed. */
    u8 tag_bypass_size;
    /*  (-e)Control bit for 'sa_in_use' flag. */
    u8 sa_index_update;
    /*  The number of bytes (0-127) that are authenticated but not 
    encrypted (AAD length).. */
    u8 conf_offset;
    /*  (-e)Enable confidentiality protection. */
    u8 encrypt_frames;
    /*  (-crypto)Specifies length (in bytes) of the bypass data. */
    u8 crypt_auth_bypass_len;
};

/*  SAM Flow Control action to clear. */
struct clr_sam_flow_ctrl {
	/*  Flow control action index to be configured. */
    u8 flow_index;
};

/*  Secure Association Matching (SAM) Entry Enable control. */
struct sam_enable_ctrl {
    /*  Index of a SAM entry to enable when set_enable = 1b. */
    u8 sam_index_set: 5;
    /*  Enable the SAM entry with index sam_index_set. */
    u8 set_enable: 1;
    /*  Enable all entries. */
    u8 set_all: 1;
    /*  Index of a SAM entry to disable when set_clear = 1b. */
    u8 sam_index_clear: 5;
    /*  Disable the SAM entry with index sam_index_clear. */
    u8 clear_enable: 1;
    /*  Disable all entries. */
    u8 clear_all: 1;
};

/*  Secure Association Matching (SAM) Entry Enable flags type. */
enum sam_ena_type {
    /*  Entry Enable flags for debugging purpose. */
    SAM_ENTRY_ENABLE = 0,
    /*   Entry Enable toggles the value of every enable flag. */
    SAM_ENTRY_TOGGLE = 1,
    /*   Entry Enable set. */
    SAM_ENTRY_SET = 2,
    /*   Entry Enable clear. */
    SAM_ENTRY_CLEAR = 3,
};

/*  Output Secure Association Matching (SAM) Entry Enable flags. */
struct sam_enable_flags {
    /*  SAM Entry Enable feature type.
    Valid values are defined in \ref sam_eex_type enum. */
    enum sam_ena_type flags_type;
    /*  Max SA Match rule flags size. */
    u32 raw_sam_flags[MACSEC_MAX_SA_RULES / 32];
};

/*  Ingress - Per-SA statistics. */
struct ing_sa_stats {
    /*  SA stats index to read. */
    u8 rule_index;
    union {
        /*  Total input octets decrypted. */
        u64 InOctetsDecrypted;
        /*  Total input octets validated. */
        u64 InOctetsValidated;
    };
    union {
        /*  Total input octets decrypted (extended). */
        u64 InOctetsDecrypted2;
        /*  Total input octets validated (extended). */
        u64 InOctetsValidated2;
    };
    union {
        /*  Total input packets Unchecked. */
        u64 InPktsUnchecked;
        /*  Total input packets HitDrop. */
        u64 InPktsHitDrop;
    };
    /*  Total input packets Delayed. */
    u64 InPktsDelayed;
    /*  Total input packets Late. */
    u64 InPktsLate;
    /*  Total input packets Decrypted. */
    u64 InPktsOK;
    /*  Total input packets Invalid. */
    u64 InPktsInvalid;
    /*  Total input packets NotValid. */
    u64 InPktsNotValid;
    /*  Total input packets NotUsingSA. */
    u64 InPktsNotUsingSA;
    /*  Total input packets UnusedSA. */
    u64 InPktsUnusedSA;
    /*  Total input packets those are not MACsec tagged. */
    u64 InPktsUntaggedHit;
};

/*  Egress - Per-SA statistics. */
struct egr_sa_stats {
    /*  SA stats index to read. */
    u8 rule_index;
    union {
        /*  Total output octets encrypted. */
        u64 OutOctetsEncrypted;
        /*  Total output octets protected. */
        u64 OutOctetsProtected;
    };
    union {
        /*  Total output octets encrypted (extended). */
        u64 OutOctetsEncrypted2;
        /*  Total output octets protected (extended). */
        u64 OutOctetsProtected2;
    };
    union {
        /*  Total output packets encrypted. Authentication with encryption (i.e TCI.E = 1). */
        u64 OutPktsEncrypted;
        /*  Total output packets protected. Authentication only (i.e TCI.E = 0). */
        u64 OutPktsProtected;
        /*  Total output packets HitDrop. Bypass/drop flow was found in SA table. */
        u64 OutPktsHitDrop;
    };
    /*  Total output packets those exceed MTU size after encrypted. */
    u64 OutPktsTooLong;
};

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
/*  Ingress Per-VLAN user priority statistics. */
struct ing_vlan_stats {
    /*  VLAN User Priority (CoS). */
    u8 vlan_up;
    /*  Total octets before decryption. */
    u64 InOctetsVL;
    /*  Total octets before decryption (extended). */
    u64 InOctetsVL2;
    /*  Total packets Decrypted. */
    u64 InPktsVL;
    /*  Total packets Dropped. */
    u64 InDroppedPktsVL;
    /*  Total packets OverSize. */
    u64 InOverSizePktsVL;
};

/*  Egress Per-VLAN user priority statistics. */
struct egr_vlan_stats {
    /*  VLAN User Priority (CoS). */
    u8 vlan_up;
    /*  Total octets after encryption. */
    u64 OutOctetsVL;
    /*  Total octets after encryption (extended). */
    u64 OutOctetsVL2;
    /*  Total packets Encrypted. */
    u64 OutPktsVL;
    /*  Total packets Dropped. */
    u64 OutDroppedPktsVL;
    /*  Total packets OverSize. */
    u64 OutOverSizePktsVL;
};
#endif

/*  Ingress - Global statistics. */
struct ing_global_stats {
    /*  Total transform errors. */
    u64 TransformErrorPkts;
    /*  Total input control frames. */
    u64 InPktsCtrl;
    /*  Total input 'Strict' validated non MACsec tagged frames. */
    u64 InPktsNoTag;
    /*  Total input non 'Strict' validated non MACsec tagged frames. */
    u64 InPktsUntagged;
    /*  Total input MACsec tagged frames. */
    u64 InPktsTagged;
    /*  Total input (non MACsec tagged) bad-tagged frames. */
    u64 InPktsBadTag;
    /*  Total input (non MACsec tagged) SA not found frames. */
    u64 InPktsUntaggedMiss;
    /*  Total input (MACsec tagged) but without SCI frames. */
    u64 InPktsNoSCI;
    /*  Total input (MACsec tagged) but with unknown SCI frames. */
    u64 InPktsUnknownSCI;
    /*  Total input Ingress CC failed Control frames. */
    u64 InConsistCheckControlledNotPass;
    /*  Total input Ingress CC failed Uncontrol frames. */
    u64 InConsistCheckUncontrolledNotPass;
    /*  Total input Ingress CC passed Control frames. */
    u64 InConsistCheckControlledPass;
    /*  Total input Ingress CC passed Uncontrol frames. */
    u64 InConsistCheckUncontrolledPass;
    /*  Total input non-VLAN frames decrypted. */
    u64 InOverSizePkts;
};

/*  Egress - Global statistics. */
struct egr_global_stats {
    /*  Total transform errors.	*/
    u64 TransformErrorPkts;
    /*  Total output control frames. */
    u64 OutPktsCtrl;
    /*  Total output frames for those SA not found. */
    u64 OutPktsUnknownSA;
    /*  Total output frames without protection. */
    u64 OutPktsUntagged;
    /*  Total output non-VLAN frames encrypted. */
    u64 OutOverSizePkts;
};

#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
/*  Statistics counters module debug registers */
struct debug_stats {
    /*  SA counters that were incremented for the last packet, refer 
    \ref ing_psa_cntr_types for Ingress and \ref egr_psa_cntr_types for Egress bit 
    mapped input.*/
    u16 sa_inc_mask: 15;
    /*  The octet increment value for SA-related octet counters that were
    incremented for the last packet. */
    u16 sa_octet_inc: 15;
    /*  VLAN counters that were incremented for the last packet, refer 
    \ref ing_vlan_cntr_types for Ingress and \ref egr_vlan_cntr_types for Egress bit 
    mapped input.*/
    u16 vlan_inc_mask: 15;
    /*  The octet increment value for VLAN-related octet counters that 
    were incremented for the last packet. */
    u16 vlan_octet_inc: 15;
    /** Global counters that were incremented for the last packet. */
    u32 global_inc_mask;
    /*  The octet increment value for global octet counters that were 
    incremented for the last packet. */
    u16 global_octet_inc: 15;
    /*  Indicates SA increment (sa_inc_mask) has been written with test 
    data. */
    u8 sa_test: 1;
    /*  Indicates VLAN increment (vlan_inc_mask) has been written with 
    test data. */
    u8 vlan_test: 1;
    /*  Indicates Global increment (global_inc_mask) has been written 
    with test data. */
    u8 global_test: 1;
    /*  The SA index used for SA-related counters that were incremented 
    for the last packet. */
    u8 sa_nr: 5;
    /*  The VLAN User Priority used for VLAN-related counters that were 
    incremented for the last packet. */
    u8 vlan_up: 3;
};
#endif

/*  Thresholds for the frame or octet counters. */
struct frm_oct_thold {
    /** Frame threshold. */
    u64 frame_threshold;
    /** Octet threshold. */
    u64 octet_threshold;
};

/*  Counters control params. */
struct cntrs_ctrl {
    /*  Reset all statistics counters to zero also clears all statistics
    summary registers. */
    u8 reset_all: 1;
    /*  Enable normal read/write access to the counter memory for dbg. */
    u8 debug_access: 1;
    /*  Saturate or wrap back. */
    u8 saturate_cntrs: 1;
    /*  Counters cleared on a read. */
    u8 auto_cntr_reset: 1;
    /*  Resets all statistics threshold summary registers. */
    u8 reset_summary: 1;
    /*  VLAN counters is allowed to be incremented, refer 
    \ref pup_vlan_cntr_sum0 and \ref pup_vlan_cntr_sum1 for per VLAN counter type which 
    are inturn mapped to individual counter types of either \ref ing_vlan_cntr_types 
    for Ingress and \ref egr_vlan_cntr_types for Egress bit mapped input. */
    u8 vlan_inc_en : 5;
    /*  SA counters is allowed to be incremented, refer 
    \ref ing_psa_cntr_types for Ingress and \ref egr_psa_cntr_types for Egress bit 
    mapped input. */
    u16 psa_inc_en: 11;
    /*  Global counters is allowed to be incremented, refer 
    \ref ing_global_cntr_types for Ingress and \ref egr_global_cntr_types for Egress bit 
    mapped input. */
    u16 global_inc_en: 14;
};

/*  A packet counter increment for which a security fail is declared. */
struct sec_fail_evnt {
    /*  SA-related counter increments are regarded a security fail, refer
    \ref ing_psa_cntr_types for Ingress and \ref egr_psa_cntr_types for Egress bit mapped 
    input. */
    u32 sa_secfail_mask: 15;
    /*  If to update any VLAN counters for a non-VLAN packet using the
    default User Priority assigned to them. */
    u32 vlan_cntr_update: 1;
    /*  (-i)If to use result of consistency checking module. */
    u32 gate_cons_check: 1;
    /*  Which global counter increments are regarded a security fail
    event, refer \ref ing_global_cntr_types for Ingress and \ref egr_global_cntr_types for
    Egress bit mapped input. */
    u32 gbl_secfail_mask;
};

/*  Misc control params. */
struct misc_ctrl {
    /*  Debug feature to disable the MACsec crypto-core. */
    u8 static_bypass: 1;
    /*  802.1AE compliant processing for correctly MACsec tagged packets
    for which no valid MACsec secure channel is found. */
    u8 nm_macsec_en: 1;
    /*  Detecting the MACsec SecTAG after one or two VLAN tags. */
    u8 sectag_after_vlan: 1;
    /*  Fixed latency forcing value for the MACsec core. The range
    0 - 255, possible extension in future. */
    u16 mc_latency_fix: 8;
    /*  Global frame validation level. Used if nm_macsec_en is set.
    Valid values are defined in \ref sam_fca_fvt enum. */
    enum sam_fca_fvt validate_frames;
};

/*  SA expired summary. */
struct sa_exp_sum {
    /*  (-e)SA expired summary word. */
    u32 sa_exp_sum[MACSEC_MAX_SA_RULES / 32];
};

/*  SA non-match SecY packet type. */
enum sanm_secy_pt {
    /*  Non-MACsec tag. */
    SA_NM_SECY_UNTAGGED = 0,
    /*  Valid MACsec tagged. */
    SA_NM_SECY_TAGGED = 1,
    /*  Invalid MACsec tagged. */
    SA_NM_SECY_BAD_TAGGED = 2,
    /*  Valid MACsec tagged handled by KaY. */
    SA_NM_SECY_KAY_TAGGED = 3,
    /*  Same action for all above types of frames. */
    SA_NM_SECY_ALL_TAGGED = 4,
};

/*  SA non-match FCA flow type. */
enum sanm_fca_ft {
    /*  Bypass. */
    SA_NM_FCA_FLOW_BYPASS = 0,
    /*  Drop. */
    SA_NM_FCA_FLOW_DROP = 1,
};

/*  SA non-match FCA dest-port type */
enum sanm_fca_dpt {
    /*  Common port. */
    SA_NM_FCA_DPT_COMMON = 0,
    /*  Reserved port. */
    SA_NM_FCA_DPT_RES = 1,
    /*  Control port. */
    SA_NM_FCA_DPT_CONTROL = 2,
    /*  Un-Control port. */
    SA_NM_FCA_DPT_UNCONTROL = 3,
};

/*  SA non-match FCA drop-action type. */
enum sanm_fca_dat {
    /*  Bypass with CRC corruption indicator. */
    SA_NM_FCA_DROP_AS_CRC = 0,
    /*  Bypass with bad packet indicator. */
    SA_NM_FCA_DROP_AS_BAD_PKT = 1,
    /*  Internal drop by crypto-core. */
    SA_NM_FCA_DROP_AS_INTERNAL = 2,
    /*  Do not drop (for debugging only). */
    SA_NM_FCA_NO_DROP = 3,
};

/*  SN non-match flow control action. */
struct sanm_fc_act {
    /*  (-i)Perform drop_action. */
    u8 drop_non_reserved: 1;
    /*  Flow type
    Valid values are defined in \ref sanm_fca_ft enum. */
    enum sanm_fca_ft flow_type;
    /*  Destination port
    Valid values are defined in \ref sanm_fca_dpt enum. */
    enum sanm_fca_dpt dest_port;
    /*  Drop action
    Valid values are defined in \ref sanm_fca_dat enum. */
    enum sanm_fca_dat drop_action;
};

/*  SN non-match flow control action. */
struct sanm_flow_ctrl {
    /*  SecY packet type.
    Valid values are defined in \ref sanm_secy_pt enum. */
    enum sanm_secy_pt pkt_type;
    /*  SN no match flow control action for control packets. */
    struct sanm_fc_act ctrl_pkt;
    /*  SN no match flow control action for non-control packets. */
    struct sanm_fc_act nctrl_pkt;
};

/*  Ingresss Per-SA counters threshold summary. Bit assigned input/output
	is mapped to this structure in which the member InOctetsDecryptedValidated 
	is mapped to LS bit 0, InPktsUncheckedHitDropReserved to LS bit 2 and so on.
	*/
struct ing_psa_cntr_types {
    /*  Total input octets decrypted (aka validated). */
    u8 InOctetsDecryptedValidated: 1;
    /*  Reserved. */
    u8 reserved: 1;
    /*  Total input packets Unchecked (aks HitDropReserved). */
    u8 InPktsUncheckedHitDropReserved: 1;
    /*  Total input packets Delayed. */
    u8 InPktsDelayed: 1;
    /*  Total input packets Late. */
    u8 InPktsLate: 1;
    /*  Total input packets Decrypted. */
    u8 InPktsOK: 1;
    /*  Total input packets Invalid. */
    u8 InPktsInvalid: 1;
    /*  Total input packets NotValid. */
    u8 InPktsNotValid: 1;
    /*  Total input packets NotUsingSA. */
    u8 InPktsNotUsingSA: 1;
    /*  Total input packets UnusedSA. */
    u8 InPktsUnusedSA: 1;
    /*  Total input packets those are not MACsec tagged. */
    u8 InPktsUntaggedHit: 1;
};

/*  Egresss Per-SA counters threshold summary. Bit assigned input/output
	is mapped to this structure in which the member OutOctetsEncryptedProtected 
	is mapped to LS bit 0, OutPktsEncryptedProtectedHitDropReserved to LS bit 2 
	and so on. */
struct egr_psa_cntr_types {
    /*  Total output octets encrypted (aka protected). */
    u8 OutOctetsEncryptedProtected: 1;
    /*  Reserved. */
    u8 reserved: 1;
    /*  Total output packets encrypted (aks HitDropReserved). */
    u8 OutPktsEncryptedProtectedHitDropReserved: 1;
    /*  Total output packets those exceed MTU size after encrypted. */
    u8 OutPktsTooLong: 1;
};

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
/*  Per-VLAN counters summary. */
struct pup_vlan_cntr_sum0 {
    /*  Threshold summary for VLAN counter 0 (i.e (-i)InOctetsVL
    or (-e)OutOctetsVL) for all UP values, meaning LS bit 0 is counter 0
    for UP=0, LS bit 1 is counter 0 for UP=1 etc. */
    u8 vlan_cntr_0: 8;
    /*  Reserved. */
    u8 reserved: 8;
    /*  Threshold summary for VLAN counter 2 (i.e (-i)InPktsVL or
    (-e)OutPktsVL) for all UP values, meaning LS bit 0 is counter 2 for
    UP=0, LS bit 1 is counter 2 for UP=1 etc. */
    u8 vlan_cntr_2: 8;
    /*  Threshold summary for VLAN counter 3 (i.e (-i)InDroppedPktsVL or 
    (-e)OutDroppedPktsVL) for all UP values, meaning LS bit 0 is counter 3 for 
    UP=0, LS bit 1 is counter 3 for UP=1 etc. */
    u8 vlan_cntr_3: 8;
};

/*  Per-VLAN counters summary */
struct pup_vlan_cntr_sum1 {
    /*  Threshold summary for VLAN counter 4 (i.e (-i)InOverSizePktsVL or
    (-e)OutOverSizePktsVL) or (-e)OutOctetsVL) for all UP values, meaning LS 
    bit 0 is counter 4 for UP=0, LS bit 1 is counter 4 for UP=1 etc. */
    u8 vlan_cntr_4: 8;
};

/*  Ingresss VLAN counters threshold summary. Bit assigned input/output
	is mapped to this structure in which the member InOctetsVL is mapped to LS 
	bit 0, InPktsVL to LS bit 2 and so on. */
struct ing_vlan_cntr_types {
    /*  Total decrypted octets. */
    u32 InOctetsVL: 1;
    /*  Reserved. */
    u32 reserved: 1;
    /*  Total decrypted packets. */
    u32 InPktsVL: 1;
    /*  Total dropped packets. */
    u32 InDroppedPktsVL: 1;
    /*  Total OverSize packets. */
    u32 InOverSizePktsVL: 1;
};

/*  Egresss VLAN counters threshold summary. Bit assigned input/output
	is mapped to this structure in which the member OutOctetsVL is mapped to LS 
	bit 0, OutPktsVL to LS bit 2 and so on. */
struct egr_vlan_cntr_types {
    /*  Total encrypted octets. */
    u8 OutOctetsVL: 1;
    /*  Reserved. */
    u8 reserved: 1;
    /*  Total encrypted packets. */
    u8 OutPktsVL: 1;
    /*  Total dropped packets. */
    u8 OutDroppedPktsVL: 1;
    /*  Total OverSize packets. */
    u8 OutOverSizePktsVL: 1;
};
#endif

/*  Ingress Global counters threshold summary. Bit assigned input/output
	is mapped to this structure in which the member TransformErrorPkts is mapped
	to LS bit 0, InPktsCtrl to LS bit 1 and so on. */
struct ing_global_cntr_types {
    /*  Total transform errors. */
    u8 TransformErrorPkts: 1;
    /*  Total input control frames. */
    u8 InPktsCtrl: 1;
    /*  Total input 'Strict' validated non MACsec tagged frames. */
    u8 InPktsNoTag: 1;
    /*  Total input non 'Strict' validated non MACsec tagged frames. */
    u8 InPktsUntagged: 1;
    /*  Total input MACsec tagged frames. */
    u8 InPktsTagged: 1;
    /*  Total input (non MACsec tagged) bad-tagged frames. */
    u8 InPktsBadTag: 1;
    /*  Total input (non MACsec tagged) SA not found frames. */
    u8 InPktsUntaggedMiss: 1;
    /*  Total input (MACsec tagged) but without SCI frames. */
    u8 InPktsNoSCI: 1;
    /*  Total input (MACsec tagged) but with unknown SCI frames. */
    u8 InPktsUnknownSCI: 1;
    /*  Total input Ingress CC failed Controlled frames. */
    u8 InConsistCheckControlledNotPass: 1;
    /*  Total input Ingress CC failed Uncontrolled frames. */
    u8 InConsistCheckUncontrolledNotPass: 1;
    /*  Total input Ingress CC passed Controlled frames. */
    u8 InConsistCheckControlledPass: 1;
    /*  Total input Ingress CC passed Uncontrolled frames. */
    u8 InConsistCheckUncontrolledPass: 1;
    /*  Total input non-VLAN frames decrypted. */
    u8 InOverSizePkts: 1;
};

/*  Egress Global counters threshold summary. Bit assigned input/output
	is mapped to this structure in which the member TransformErrorPkts is mapped
	to LS bit 0, OutPktsCtrl to LS bit 1 and so on.  */
struct egr_global_cntr_types {
    /*  Total transform errors.	*/
    u32 TransformErrorPkts: 1;
    /*  Total output control frames. */
    u32 OutPktsCtrl: 1;
    /*  Total output frames for those SA not found. */
    u32 OutPktsUnknownSA: 1;
    /*  Total output frames without protection. */
    u32 OutPktsUntagged: 1;
    /*  Total output non-VLAN frames encrypted. */
    u32 OutOverSizePkts: 1;
};

/*  SA expired/pn-thr/psa summary */
struct cntrs_thold_sum {
    /*  Global conuters threshold summary, refer \ref ing_global_cntr_types
    for Ingress and \ref egr_global_cntr_types for Egress bit mapped input/output. */
    u32 gl_cntr_sum;
#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
    /*  Per VLAN user priority conuters threshold summary, refer 
    \ref pup_vlan_cntr_sum0 and \ref pup_vlan_cntr_sum1 for per VLAN counter type which 
    are inturn mapped to individual counter types of either \ref ing_vlan_cntr_types 
    for Ingress and \ref egr_vlan_cntr_types for Egress bit mapped input/output. */
    u32 vlan_cntr_sum[MAX_VLAN_COUNT_SUMM_WORDS];
#endif
    /*  This summary is a vector with one flag per SA to indicate that
    any counter within the corresponding SA statistics group crossed the
    programmed threshold (there can be multiple at the same time). */
    u32 psa_cntr_vec[MACSEC_MAX_SA_RULES / 32];
    /*  When per SA bit set in above field 'psa_cntr_vec', there is a
    separate summary for all counters of this SA, refer \ref ing_psa_cntr_types for 
    Ingress and \ref egr_psa_cntr_types for Egress bit mapped input/output. */
    u32 psa_cntr_sum[MACSEC_MAX_SA_RULES];
};

/*  Control Packet Classification entry type. */
enum cpc_entry_type {
    /*  Entries 0 - 7 for rule matching on DA and/or ET fields. */
    CPC_ENTRY_DA_ANDOR_ETH = 0,
    /*  Entries 8 - 9 for rule matching on DA and ET fields combined. */
    CPC_ENTRY_DA_ETH_COMBINED = 1,
    /*  Enable MAC_DA compare against programmable 44 bits “constant”. */
    CPC_ENTRY_DA_CONST44 = 2,
    /*  Enable MAC_DA compare against programmable 48 bits “constant”. */
    CPC_ENTRY_DA_CONST48 = 3,
    /*  DA range compare with lower and upper bound values. */
    CPC_ENTRY_DA_RANGE = 4,
    /*  Entries 10 - 17 for rule matching on ET fields only. */
    CPC_ENTRY_ETH_ONLY = 5,
    /*  Entries 10 - 17 for rule matching on outer VLAN ID and/or PCP fields only. */
    CPC_ENTRY_OUTER_VLAN_ONLY = 6,
};

/*  Control Packet match rule params. */
struct cp_match_parm {
    /*  CPC entry type to program. */
    enum cpc_entry_type cpc_type;
    /*  CPC rule entry number as below:
    	0 - 7 for cpc_type = CPC_ENTRY_DA_ANDOR_ETH,
    	8 - 9 for cpc_type = CPC_ENTRY_DA_ETH_COMBINED,
    	10 - 17 for cpc_type = CPC_ENTRY_ETH_ONLY or CPC_ENTRY_OUTER_VLAN_ONLY,
    	NA for all other types of cpc_type. */
    u8 cpc_index;
    /*  Dst MAC of either individual address or start address in case of 
    range, fixed address of 44/48 bits “constant”. */
    u8 mac_da[6];
    /*  Dst MAC end address in case of range, NA in other cases. */
    u8 mac_da_end[6];
    union {
        struct {
            /*  Outer VLAN id. */
            u16 vlan_id: 12;
            /*  Outer VLAN PCP. */
            u8 vlan_up: 3;
            /*  Disable using vlan_up. */
            u8 disable_up: 1;
        };
        /*  Ether-type. */
        u16 ether_type;
    };
};

/*  Control packet match's mode & enable configuration */
struct cpm_mode_ena {
    /*  The source of eth-type (1 = parsed, 0 = packet) in match for 
    entries 0 - 17, this is the bit mapped input/output in which LS bit 0 for 
    e_type_sel_0, LS bit 1 for e_type_sel_1 and so on. */
    u32 e_type_sel_0_17: 18;
    /*  Enable MAC_DA compare for entries 0 - 7. */
    u8 mac_da_0_7;
    /*  Enable ether-type compare for entries 0 - 7. */
    u8 e_type_0_7;
    /*  Enable combined MAC_DA/ether-type compare for entries 8 - 9. */
    u8 combined_8_9: 2;
    /*  Enable MAC_DA range compare. */
    u8 dmac_range:1;
    /*  Enable MAC_DA compare against programmable 44 bits “constant”. */
    u8 dmac_const_44:1;
    /*  Enable MAC_DA compare against programmable 48 bits “constant”. */
    u8 dmac_const_48:1;
    union {
        /*  Enable ether-type compare for entries 10 - 17. */
        u8 e_type_10_17;
        /*  Enable matching on VLAN ID and/or UP compare for entries 
        10 - 17, with this the corresponding e_type_sel_10_17 are don’t care. */
        u8 vlan_sel_0_7;
    };
};

/*  Ingress Consistency Check match's misc control. */
struct ccknm_flow_ctrl {
    /*  No match action for 'non-Control' packets. */
    u8 nm_act_ncp: 1;
    /*  No match action for 'Control' packets. */
    u8 nm_act_cp: 1;
    /*  Ether-type field compare value for the etype_valid comparison. */
    u16 cp_etype_max_len;
};

/*  Ingress Consistency Check match's misc parameters. Bit assigned 
	input/output is mapped to this structure in which the member sai_hit mapped 
	to LS bit 0, vlan_valid to LS bit 1 and so on. */
struct iccm_misc_parm {
    /*  SA entry matched for this packet. */
    u8 sai_hit: 1;
    /*  Packet is tagged as 802.1Q or 802.1s VLAN packet. */
    u8 vlan_valid: 1;
    /*  First Ether-type in packet > cp_etype_max_len. */
    u8 etype_valid: 1;
    /*  Packet was classified as control packet. */
    u8 ctrl_packet: 1;
    /*  Enable sai_hit field compare. */
    u8 sa_hit_mask: 1;
    /*  Enable sa_or_rule_nr field compare with SA entry number (the 
    rule_nr_mask must be 0b). */
    u8 sa_nr_mask: 1;
    /*  Enable vlan_valid field compare. */
    u8 vlan_valid_mask: 1;
    /*  Enable vlan_id field compare (do not set to 1b if no VLAN packet 
    is expected here). */
    u8 vlan_id_mask: 1;
    /*  Enable etype_valid field compare. */
    u8 etype_valid_mask: 1;
    /*  Enable payload_e_type field compare. */
    u8 payl_etype_mask: 1;
    /*  Enable ctrl_packet field compare. */
    u8 ctrl_packet_mask: 1;
    /*  Enable sa_or_rule_nr field compare with SA matching rule number 
    (the sa_nr_mask must be 0b). */
    u8 rule_nr_mask: 1;
    /*  Priority of this entry for determining the match action on a 
    match, 0 = lowest, 7 = highest. */
    u8 match_priority: 3;
    /*  Packet match action, 0b = drop, 1b = pass. */
    u8 match_action: 1;
    /*  Compare value for SA matching rule number or SA entry number, 
    depending on which flag is set (rule_nr_mask or sa_nr_mask). */
    u8 sa_or_rule_nr: 8;
    /*  Enable parsed inner VLAN User Priority value compare with 
    vlan_up_inner field. */
    u8 vlan_up_inner_mask: 1;
    /*  Enable parsed inner VLAN ID value compare with vlan_up_inner 
    field. */
    u8 vlan_id_inner_mask: 1;
    /*  SA matching rule matched for this packet. */
    u8 rule_hit: 1;
    /*  Enable rule_hit field compare. */
    u8 rule_hit_mask: 1;
};

/*  Ingress Consistency Check parameters. */
struct icc_match_parm {
    /** ICC index to configure. */
    u8 icc_index;
    /*  Parsed inner VLAN Up compare value. */
    u8 inr_vlan_up: 3;
    /*  Outer VLAN Id compare value. */
    u16 otr_vlan_id: 12;
    /*  Parsed inner VLAN Id compare value. */
    u16 inr_vlan_id: 12;
    /*  Ether-type compare value against actual Ether-type field. */
    u16 payload_e_type: 16;
    /*  Bit assigned input/output (as in \ref iccm_misc_parm) misc match 
    and mask parameters, in which the member sai_hit as LS bit 0, vlan_valid as
    LS bit 1 and so on. */
    u32 iccm_misc_flags;
};

/*  Ingress Consistency Check match entry enable control. */
struct ccm_ena_ctrl {
    /*  Index of a CC entry to enable when set_enable = 1b. */
    u8 cc_index_set: 8;
    /*  Enable the CC entry with index cc_index_set. */
    u8 set_enable: 1;
    /*  Enable all entries. */
    u8 set_all: 1;
    /*  Index of a CC entry to disable when set_clear = 1b. */
    u8 cc_index_clear: 8;
    /*  Disable the CC entry with index cc_index_clear. */
    u8 clear_enable: 1;
    /*  Disable all entries. */
    u8 clear_all: 1;
};

/*  In/Out ICC Entry Enable flags. */
struct ccm_ena_flags {
    /*  Max ICC Match rule flags size. */
    u32 raw_ccm_flags[MACSEC_MAX_SA_RULES / 32];
};

/*  OPP's Packet number threshold summary. */
struct pn_thold_sum {
    /*  (-e)Packet number threshold summary. */
    u32 pn_thold_sum[MACSEC_MAX_SA_RULES / 32];
};

/*  Sequence number threshold */
struct sn_thold {
    /*  Sequence number type.
    Valid values are defined in \ref sn_type enum. */
    enum sn_type sn_type;
    /** SN threshold. */
    u64 sn_threshold;
};

#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
/* Context Status provides the context status of the currently active
	frame. Bit assigned output is mapped to this structure in which the member 
	e0 mapped to LS bit 0, e1 to LS bit 1 and so on. */
struct eip62_ctx_stat {
    /* Frame length error. */
    u8 e0: 1;
    /* Token error. */
    u8 e1: 1;
    /* Frame input length error. */
    u8 e2: 1;
    /* Counter overflow. */
    u8 e3: 1;
    /* Token configuration error. */
    u8 e4: 1;
    /* Invalid command/algorithm/mode/combination. */
    u8 e5: 1;
    /* Prohibited algorithm. */
    u8 e6: 1;
    /* (-i)Packet number is zero. */
    u8 e7: 1;
    /* Sequence number threshold overflow. */
    u8 e8: 1;
    /* Authentication failed. */
    u8 e9: 1;
    /* (-i)Sequence number check failed. (-e)Sequence number rollover
    detected. */
    u8 e10: 1;
};
#endif

/* Crypto Cores's interrupt status. Bit assigned output is mapped to 
	this structure in which the member input_error mapped to LS bit 0, 
	processing_error to LS bit 2 and so on. */
struct eip62_intrp_stat {
    /* Input data does not properly receive all data (e2). */
    u8 input_error: 1;
    /* Reserved. */
    u8 reserved: 1;
    /* A error occurred while processing the frame or parsing the token.
    Combination of errors e0, e1, e3 and e4. */
    u8 processing_error: 1;
    /* An invalid context has been fetched. Combination of errors e5 and
    e6. */
    u8 context_error: 1;
    /* If a sequence number exceeds the programmed sequence number
    threshold (refer to 0) due to an outbound sequence number increment this
    interrupt is triggered (e8). */
    u8 seq_threshold: 1;
    /* If a sequence number roll-over (increment from maximum to zero)
    due to an outbound sequence number increment this interrupt is triggered
    (e10 egress only). */
    u8 seq_rollover: 1;
    /* Reserved. */
    u16 reserved2: 9;
    /* Actual value of the output pin, read-only. */
    u8 interrupt_pin: 1;
};

/* Crypto Cores's interrupt enable control. Bit assigned input is mapped
	to this structure in which the member input_error mapped to LS bit 0, 
	processing_error to LS bit 2 and so on. */
struct eip62_intrp_ctrl {
    /* Enable the input_error interrupt. */
    u8 input_error_en: 1;
    /* Enable the output_error interrupt. */
    u8 output_error_en: 1;
    /* Enable the processing_error interrupt. */
    u8 processing_error_en: 1;
    /* Enable the context_error interrupt. */
    u8 context_error_en: 1;
    /* Enable the seq_threshold interrupt. */
    u8 seq_threshold_en: 1;
    /* Enable the seq_rollover interrupt. */
    u8 seq_rollover_en: 1;
    /* Reserved. */
    u8 reserved3: 8;
    /* Enable the fatal_error interrupt. */
    u8 fatal_error_en: 1;
    /* Set to 1b if the interrupt signal must appear on the EIP-62 IRQ
    output. */
    u8 interrupt_pin_en: 1;
};

/* Interrupt bit assignments in Advanced Interrupt Controller (EIP201).
	Bit assigned input/output is mapped to this structure in which the member 
	drop_class mapped to LS bit 0, drop_pp to LS bit 1 and so on. */
struct eip201_intrpts {
    /* Packet drop pulse from classification logic. */
    u8 drop_class: 1;
    /* Packet drop pulse from post-processor logic. */
    u8 drop_pp: 1;
    /* Packet drop pulse from MTU checking logic. */
    u8 drop_mtu: 1;
    /* Interrupt pulse from EIP-62 MACsec crypto-engine core. */
    u8 eng_irq: 1;
    /* Interrupt pulse from statistics module (SA statistics). */
    u8 stat_sa_thr: 1;
    /* Interrupt pulse from statistics module (global statistics). */
    u8 stat_global_thr: 1;
    /* Interrupt pulse from statistics module (VLAN statistics). */
    u8 stat_vlan_thr: 1;
    /* (-i)Packet drop pulse from consistency checking logic. */
    u8 drop_cc: 1;
    /* (-e)Pulse from post processor. */
    u8 sa_pn_thr: 1;
    /* (-e)Pulse from classifier. */
    u8 sa_expired: 1;
};

/*  MACsec crypto-core control/status registers */
struct crypto_intrp_csr {
    /*  Crypto Cores's interrupt enable control. Bit assigned input as in
    (\ref eip62_intrp_ctrl). */
    u16 intrp_ctrl;
    /*  Crypto Cores's interrupt hit status. Bit assigned output as in
    (\ref eip62_intrp_stat). */
    u16 intrp_stat;
#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
    /*  Crypto Cores's Context status (for debugging only). Bit assigned 
    output as in (\ref eip62_ctx_stat). */
    u16 ctx_stat;
#endif
};

/*  Advanced Interrupt Control */
struct adv_intrp_ctrl {
    /*  Bit assigned (\ref eip201_intrpts) “enable control” (disabled vs 
    enabled) bits per interrupt input. */
    u16 enable_ctrl;
    /*  Bit assigned (\ref eip201_intrpts) “interrupt enable” (enable the
    interrupt by setting the corresponding 'enable_ctrl' varible) bits per 
    interrupt input. */
    u16 enable_set;
    /*  Bit assigned (\ref eip201_intrpts) “acknowledge” (resets the edge
    detector for the interrupt input) bits per interrupt input. */
    u16 ack_intrpt;
    /*  Bit assigned (\ref eip201_intrpts) “interrupt disable” bits per 
    interrupt input. */
    u16 enable_clr;
};

/*  Advanced Interrupt Status */
struct adv_intrp_stat {
    /*  Bit assigned (\ref eip201_intrpts) “polarity control” (low level 
    or falling edge vs high level or rising edge) bits per interrupt output. */
    u16 inpol_ctrl;
    /*  Bit assigned (\ref eip201_intrpts) “type control” (level vs edge 
    detect) bits per interrupt output. */
    u16 type_ctrl;
    /*  Bit assigned (\ref eip201_intrpts) “raw status” (i.e. the status 
    before masking) (inactive vs pending) bits per interrupt output. */
    u16 raw_stat;
    /*  Bit assigned (\ref eip201_intrpts) “enabled status”  (i.e. the 
    status after masking) (inactive vs pending) bits per interrupt output. */
    u16 masked_stat;
    /*  Bit assigned (\ref eip201_intrpts) “enable control” (disabled vs 
    enabled) bits per interrupt output. */
    u16 enable_ctrl;
};


/**********************
Internal APIs
 **********************/
/*
	os_mem_free
	OS specific free func
*/
static inline void os_mem_free(void *p)
{
    if (p != NULL)
#ifndef __KERNEL__
        free(p);
#else
        kfree(p);
#endif
}

/*
	os_mem_alloc
	OS specific malloc func
*/
static inline void *os_mem_alloc(size_t size)
{
    if (size > 0) {
#ifndef __KERNEL__
        return malloc(size);
#else
        return kzalloc(size, in_atomic() ? GFP_ATOMIC : GFP_KERNEL);
#endif
    } else
        return NULL;
}

/**********************
	Function prototypes
 **********************/

/**
	This API initializes the one time settings before configuring rules.

	@pdev: Pointer to type void (\ref ).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_init_ing_dev(struct eip160_pdev *pdev);
int eip160_init_egr_dev(struct eip160_pdev *pdev);

/**
	Gets the MACSec hardware capabilities.

	@pdev: Pointer to type void (\ref ).
	@hw_cap Pointer to HW cap (\ref eip160_hw_cap).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_ing_hw_cap(struct eip160_pdev *pdev, struct eip160_hw_cap *hw_cap);
int eip160_get_egr_hw_cap(struct eip160_pdev *pdev, struct eip160_hw_cap *hw_cap);

/**
	Configures/Get/Clear the Secure Association (SA) parameters.

	It is also called a transform record, which contains SA parameters 
	of common for both Ingress and Egress device, such as Crypto Algorithm type,
	Sequence Number type (32-bit vs 64-bit), SA Key, Secure Channel Identifier 
	(SCI), Sequence Number (SN), Context Salt and Short SCI. 

	Along with above common parameters, the additional parameters applicable to
	Ingress device only are Replay Window size, and for Egress device only are 
	Association Number (AN), Next SA index, Next Flow index, SA expired IRQ 
	event, SA invalid and SA index update enable flags.

	A MACsec transform record is a data structure containing all security 
	transform information (such as keys and sequence numbers) needed to carry 
	out a MACsec transform.

	All fields of the transform record must be populated by the Host software 
	before the corresponding SA flow can be enabled. For bypass and drop flows, 
	the transform record is not used. For MACsec transformations, the hardware 
	only updates the sequence number field; it will not modify the other fields 
	during MACsec egress and ingress processing.

	@pdev: Pointer to type void (\ref ).
	@sa_par Pointer to TR config (\ref transform_rec).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_sa_parms(struct eip160_pdev *pdev,
                             struct transform_rec *sa_par);
int eip160_cfg_egr_sa_parms(struct eip160_pdev *pdev,
                             struct transform_rec *sa_par);
int eip160_get_ing_sa_parms(struct eip160_pdev *pdev,
                             struct transform_rec *sa_par);
int eip160_get_egr_sa_parms(struct eip160_pdev *pdev,
                             struct transform_rec *sa_par);
int eip160_clr_ing_sa_parms(struct eip160_pdev *pdev,
                             const struct clr_trans_rec *clr_tr);
int eip160_clr_egr_sa_parms(struct eip160_pdev *pdev,
                             const struct clr_trans_rec *clr_tr);

/**
	Gets the next sequence number of given SA. 

	This is used to check the PN exhaustion and extend the current 
	flow's life. For MACsec transformations, the hardware only updates the 
	sequence number field of transform record.

	@pdev: Pointer to type void (\ref ).
	@next_pn Pointer to TR config (\ref sa_current_pn).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_ing_sa_next_pn(struct eip160_pdev *pdev,
                              struct sa_next_pn *next_pn);
int eip160_get_egr_sa_next_pn(struct eip160_pdev *pdev,
                              struct sa_next_pn *next_pn);

/**
	This API updates the SA control word, that controls how the SA index
		is switched when the packet number for the current SA is expired.

	If automatic SA switching is used, this API updates parameters such as
		Next SA index, SA expired IRQ event, Flow index and SA index update enable
		fields for the Egress device.

		If the next SA was not installed in time and the current SA expired,
		the SA is invalidated and an SA expired interrupt is generated along
		with a flag set in the SA expired summary register.

		If sa_ind_update_en is set, the current SA index in the corresponding flow
		control register is updated with the next_sa_index if sa_index_valid is set,
		otherwise the SA is invalidated by clearing the value of the sa_in_use field
		in the flow control register.

	@pdev: Pointer to type void (\ref ).
	@sa_cw Pointer to SA Update Control config (\ref update_sa_cw).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_egr_auto_sa_sw(struct eip160_pdev *pdev,
                              const struct update_sa_cw *sa_cw);
int eip160_get_egr_auto_sa_sw(struct eip160_pdev *pdev,
                                    struct update_sa_cw *sa_cw);

/**
	Configures the SA matching flow control action, which contains
		   parameters such as flow type (\ref sam_fca_ft), destination port type
		   (\ref sam_fca_dpt), drop non-reserved, cryptographic authentication,
		   drop type (\ref sam_fca_dat), SA index, replay protection, validation
		   level, confidentiality offset, IV load type (\ref sam_fca_iv),
		   bit 7 of the AAD length, append ICV, and verify ICV of Ingress or  device.

	@pdev: Pointer to type void (\ref ).
	@sam_fca Pointer to SA matching flow control action (\ref sam_flow_ctrl).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_sam_flow_ctrl(struct eip160_pdev *pdev,
                                 const struct sam_flow_ctrl *sam_fca);

/**
	Configures the SA matching flow control action, which contains
		   parameters such as flow type (\ref sam_fca_ft), destination port type
		   (\ref sam_fca_dpt), cryptographic authentication, drop type
		   (\ref sam_fca_dat), SA index, frame protection, include SCI,
		   confidentiality offset, IV load type (\ref sam_fca_iv),
		   bit 7 of the AAD length, append ICV, verify ICV, end station
		   & single copy broadcast flags, tag bypass size, SA index update flag, and
		   confidentiality protection flag for the Egress device.

	@pdev: Pointer to type void (\ref ).
	@sam_fca Pointer to SA Matching Flow Control action (\ref sam_flow_ctrl).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_egr_sam_flow_ctrl(struct eip160_pdev *pdev,
                                 const struct sam_flow_ctrl *sam_fca);

/**
	Gets the SA matching flow control action, which contains
		   parameters such as flow type (\ref sam_fca_ft), destination port type
		   (\ref sam_fca_dpt), drop non-reserved, cryptographic authentication,
		   drop type (\ref sam_fca_dat), SA index, replay protection, validation
		   level, confidentiality offset, IV load type (\ref sam_fca_iv),
		   bit 7 of the AAD length, append ICV, and verify ICV of Ingress or  device.

	@pdev: Pointer to type void (\ref ).
	@sam_fca Pointer to SA matching flow control action (\ref sam_flow_ctrl).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_ing_sam_flow_ctrl(struct eip160_pdev *pdev,
                                 struct sam_flow_ctrl *sam_fca);

/**
	Gets the SA matching flow control action, which contains
		   parameters such as flow type (\ref sam_fca_ft), destination port type
		   (\ref sam_fca_dpt), cryptographic authentication, drop type
		   (\ref sam_fca_dat), SA index, frame protection, include SCI,
		   confidentiality offset, IV load type (\ref sam_fca_iv),
		   bit 7 of the AAD length, append ICV, verify ICV, end station
		   & single copy broadcast flags, tag bypass size, SA index update flag, and
		   confidentiality protection flag for the Egress device.

	@pdev: Pointer to type void (\ref ).
	@sam_fca Pointer to SA matching flow control action (\ref sam_flow_ctrl).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_egr_sam_flow_ctrl(struct eip160_pdev *pdev,
                                 struct sam_flow_ctrl *sam_fca);
int eip160_clr_ing_sam_flow_ctrl(struct eip160_pdev *pdev,
                             const struct clr_sam_flow_ctrl *sam_fca);
int eip160_clr_egr_sam_flow_ctrl(struct eip160_pdev *pdev,
                             const struct clr_sam_flow_ctrl *sam_fca);

/**
	Provides the possibility to track the number of packets in the
	pipeline for hardware/software synchronization purposes of Ingress or
	Egress device.

	After disabling an SA, Host software should wait until this field
	is zero before final readout of the SA statistics and re-using the transform
	record. This mechanism also can be used when collecting any type statistics
	(event ones for an active SA) to make sure that the read statistics includes
	the packets that are in the pipeline.

	@pdev: Pointer to type void (\ref ).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_ck_ing_sam_in_flight(struct eip160_pdev *pdev);
int eip160_ck_egr_sam_in_flight(struct eip160_pdev *pdev);

/**
	Configures the SA matching rule, which contains parameters such as
		   Src MAC, Dst MAC, Ether type, Misc Match (\ref sam_misc_parm),
		   SCI, Match Mask (\ref sam_mask_parm) and Flow Index of Ingress or
		   device.

	@pdev: Pointer to type void (\ref ).
	@sam_par Pointer to SA matching rule config (\ref sa_match_parm).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_sam_rule(struct eip160_pdev *pdev,
                            const struct sa_match_parm *sam_par);

/**
	Configures the SA matching rule, which contains parameters such as
		   Src MAC, Dst MAC, Ether type, Misc Match (\ref sam_misc_parm),
		   SCI, Match Mask (\ref sam_mask_parm) and Flow Index for the Egress
		   device.

	@pdev: Pointer to type void (\ref ).
	@sam_par Pointer to SA matching rule config (\ref sa_match_parm).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_egr_sam_rule(struct eip160_pdev *pdev,
                            const struct sa_match_parm *sam_par);

/**
	Gets the SA matching rule, which contains parameters such as
		   Src MAC, Dst MAC, Ether type, Misc Match (\ref sam_misc_parm),
		   SCI, Match Mask (\ref sam_mask_parm) and Flow Index of Ingress or
		   device.

	@pdev: Pointer to type void (\ref ).
	@sam_par Pointer to SA matching rule config (\ref sa_match_parm).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_ing_sam_rule(struct eip160_pdev *pdev,
                            struct sa_match_parm *sam_par);

/**
	Gets the SA matching rule, which contains parameters such as
		   Src MAC, Dst MAC, Ether type, Misc Match (\ref sam_misc_parm),
		   SCI, Match Mask (\ref sam_mask_parm) and Flow Index for the Egress
		   device.

	@pdev: Pointer to type void (\ref ).
	@sam_par Pointer to SA matching rule config (\ref sa_match_parm).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_egr_sam_rule(struct eip160_pdev *pdev,
                            struct sa_match_parm *sam_par);
int eip160_clr_ing_sam_rule(struct eip160_pdev *pdev,
                             const struct clr_sam_par *clr_sam);
int eip160_clr_egr_sam_rule(struct eip160_pdev *pdev,
                             const struct clr_sam_par *clr_sam);

/**
	Configures the SAM entry enable control of Ingress or  device.
		   This offers combined functionality to be performed with a single write action:

	a) Enabling or disabling an entry.
	b) Enabling one and disabling another entry at the same time.
	c) Enabling or disabling all entries.

	@pdev: Pointer to type void (\ref ).
	@ee_ctrl Pointer to SAM EE Control (\ref sam_enable_ctrl).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_sam_ena_ctrl(struct eip160_pdev *pdev,
                                const struct sam_enable_ctrl *ee_ctrl);
int eip160_cfg_egr_sam_ena_ctrl(struct eip160_pdev *pdev,
                                const struct sam_enable_ctrl *ee_ctrl);

/**
	Gets the SAM entry enable flags of Ingress or Egress device.

	@pdev: Pointer to type void (\ref ).
	@en_flags Pointer to SAM EE Control (\ref sam_enable_flags).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_ing_sam_ena_flags(struct eip160_pdev *pdev,
                                 struct sam_enable_flags *en_flags);
int eip160_get_egr_sam_ena_flags(struct eip160_pdev *pdev,
                                 struct sam_enable_flags *en_flags);

/**
	Configures/gets the the control packet classification rules of
	Ingress or Egress device.

	One of the first parallel operations of the packet classification is to
	determine whether or not this is a control packet. The MAC_DA and
	Ether-type fields of the packet are compared to several values (individually
	and in combination). If one of these comparisons returns a match, the packet
	is tagged as a control packet. Each of the comparisons can be enabled
	individually.

	@pdev: Pointer to type void (\ref ).
	@cpm_par Pointer to CPC rule (\ref cp_match_parm).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_cpm_rule(struct eip160_pdev *pdev,
                            const struct cp_match_parm *cpm_par);
int eip160_cfg_egr_cpm_rule(struct eip160_pdev *pdev,
                            const struct cp_match_parm *cpm_par);
int eip160_get_ing_cpm_rule(struct eip160_pdev *pdev,
                            struct cp_match_parm *cpm_par);
int eip160_get_egr_cpm_rule(struct eip160_pdev *pdev,
                            struct cp_match_parm *cpm_par);

/**
	Configures/gets the the control packet match's mode and enable flags
	of Ingress or Egress device.

	@pdev: Pointer to type void (\ref ).
	@cpc_mm Pointer to CPC rule (\ref cpm_mode_ena).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_cpm_ena_ctrl(struct eip160_pdev *pdev,
                                const struct cpm_mode_ena *cpc_mm);
int eip160_cfg_egr_cpm_ena_ctrl(struct eip160_pdev *pdev,
                                const struct cpm_mode_ena *cpc_mm);
int eip160_get_ing_cpm_ena_flags(struct eip160_pdev *pdev,
                                 struct cpm_mode_ena *cpc_mm);
int eip160_get_egr_cpm_ena_flags(struct eip160_pdev *pdev,
                                 struct cpm_mode_ena *cpc_mm);

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
/**
	Configures/gets VLAN parsing parameters of the Ingress or Egress
	device.

	It delivers a number of “parsed” parameters to the main SA matching
	logic to determine which SA to use. It inspects VLAN tags immediately
	following the src-mac. The parsing result of output signals are VLAN valid
	(i.e VLAN tag is successfully parsed), Outer and/or Inner VLAN fields
	(ID, PCP), QTAG valid (i.e first valid VLAN tag is an “802.1Q” tag), STAG
	valid (i,e first valid VLAN tag is an “802.1s” tag) and QinQ valid (i.e two
	valid VLAN tags were found).

	Parsed ether-type (i.e. the first non-VLAN ether-type found in the frame)
	determined by the VLAN parsing algorithm.

	An xtension to the standard MACsec frame that allows having one or two VLAN
	tags in front of the SecTAG. The VLAN parser does not look beyond SecTAG.

	Note: Packets with VLAN tags that are subsequently MACsec protected cannot
	be recognized by the VLAN tag parsing logic. A packet cannot have both a
	valid VLAN tag and a valid MACsec tag. “VLAN valid” can only be true if the
	MACsec tag status is “untagged”.

	@pdev: Pointer to type void (\ref ).
	@vlan_par Pointer to VLAN config (\ref vlan_parse_parm).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_vlan_parser(struct eip160_pdev *pdev,
                               const struct vlan_parse_parm *vlan_par);
int eip160_cfg_egr_vlan_parser(struct eip160_pdev *pdev,
                               const struct vlan_parse_parm *vlan_par);
int eip160_get_ing_vlan_parser(struct eip160_pdev *pdev,
                               const struct vlan_parse_parm *vlan_par);
int eip160_get_egr_vlan_parser(struct eip160_pdev *pdev,
                               const struct vlan_parse_parm *vlan_par);
#endif

/**
	This API controls how received packets (both control & non-control)
	that do not match any of the SA matching rules are handled of Ingress or
	Egress device.

	If the input classification of the EIP-160 is used, the non-
	matching flows are subdivided into eight packet type categories, split by
	whether or not the packet was classified as a control packet and the output
	of a separate MACsec tag classification logic.

	The eight packet type categories are non-control & control packet type of
	untagged, MACsec-tagged, bad-tagged and KaY-tagged frames.

	For these non-matching flows, the only possible actions are bypass and drop
	(MACsec operations cannot be selected here). It also specifies the
	destination port. The way a packet must be dropped can also be specified.

	For Ingress it also specifies for the packets that are not from the reserved
	port (source port is not equal to 01b) are to be dropped.

	@pdev: Pointer to type void (\ref ).
	@sanm_fca Pointer to SA No-Match flow ctrl action (\ref sanm_flow_ctrl).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_sanm_flow_ctrl(struct eip160_pdev *pdev,
                                  const struct sanm_flow_ctrl *sanm_fca);
int eip160_cfg_egr_sanm_flow_ctrl(struct eip160_pdev *pdev,
                                  const struct sanm_flow_ctrl *nm_ncp);

/**
	Configures following of Ingress or Egress device:

	a) Enabling static bypass mode, a debug feature to disable the MACsec
	   crypto-core, feeding packet data around it to minimize latency while
	   leaving classification functionality.
	b) Controlling the latency equalization function.
	c) Enabling 802.1AE compliant handling of correctly tagged MACsec packets
	   for which no MACsec SA is available. Processing is done according to the
	   global validation (validate_frames field in this register).

	   If not enabled, the packet is processed as defined by the SA non-match
	   flow control for this packet. Only 802.1AE compliant statistics are
	   counted based on the global validation (InPktsNoSCI or InPktsUnknownSCI).
	d) Global frame validation level. This value is used only if 802.1AE
	   compliant processing is enabled as in point c above.

	   If a MACsec secure channel was found, the validate_frames field from the
	   corresponding SA flow control register is used.
	e) Enabling detection of the MACsec SecTAG after one or two VLAN tags.
	   If set, the MACsec SecTAG that resides after one or two VLAN tags is
	   detected and this information is used to determine how many VLAN tags
	   need to bypass the MACsec ingress processing.

	   If clear, the MACsec SecTAG is only detected when residing immediately
	   after the MAC SA/DA.

	   Note: For the egress-only configuration, setting this flag to 1b is not
	   needed if no MACsec tagged packets with VLAN tags before SecTAG are
	   expected. The number of VLAN tags to bypass the MACsec egress processing
	   is determined by the flow control register (section 3.3.5.1). If MACsec
	   tagged packets with VLAN tags before SecTAG are expected (e.g. KaY
	   packets), this flag needs to be set to 1b.

	If validate_frames != 2 & nm_macsec_en = 1, if a MACsec SecY receives a
	MACsec packet on the common port for which it has no MACsec SA and the
	packet’s payload needs no decryption (i.e. an authenticate-only operation is
	indicated with TCI.C = 0b and TCI.E = 0b), it may still forward (if set to
	bypass) the packet to the controlled port without checking authentication,
	simply by stripping the SecTag and ICV.

	@pdev: Pointer to type void (\ref ).
	@misc_ctrl Pointer to misc control (\ref misc_ctrl).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_misc_flow_ctrl(struct eip160_pdev *pdev,
                                  const struct misc_ctrl *misc_ctrl);
int eip160_cfg_egr_misc_flow_ctrl(struct eip160_pdev *pdev,
                                  const struct misc_ctrl *misc_ctrl);

/**
	Gets/clears the the SA expired summary for the Egress device.

	The SA expired summary register indicates which MACsec Egress
	SA has expired due to the packet number reaching the maximum allowed value
	and no valid next SA being installed.

	The SA expired flag is set only in case of automatic SA expiry/switching.
	If the SA is disabled/switched by the host, no summary is annotated.

	The SA expired summary register is a vector with length equal to the number
	of supported SAs, so one flag per SA. When the packet number of a MACsec SA
	reaches the maximum allowed value, the MACsec core updates the sa_index and
	sa_in_use fields in the flow control register based on the content of the SA
	update control word. If the SA update control word indicates that no valid
	next SA is installed, the EIP-160 invalidates the current SA by forcing the
	sa_in_use field in the flow control register to zero and sets the
	corresponding flag in the summary register.

	The event of setting a flag in the summary register is used to generate an
	interrupt event for the embedded interrupt controller as well as for the
	sa_expired signal that is available at top-level of the EIP-160. The host
	can get information on which of SA has expired by reading this API.

	Writing 1b to the flags that are set in read vector clears the flags.

	@pdev: Pointer to type void (\ref ).
	@exp_sum Pointer to SA expired summary (\ref sa_exp_sum).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_clr_egr_sa_exp_sum(struct eip160_pdev *pdev,
                              const struct sa_exp_sum *exp_sum);
int eip160_get_egr_sa_exp_sum(struct eip160_pdev *pdev,
                              struct sa_exp_sum *exp_sum);

/**
	Clears the SA packet number threshold summary for the Egress
		   device.

	The packet number threshold summary register indicates any MACsec egress SA for which
	the packet number will expire imminently such that the host must initiate retrieval of
	a new (next) key.

	@pdev: Pointer to type void (\ref ).
	@pnthr_sum Pointer to threshold summary (\ref pn_thold_sum).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_clr_egr_pn_thold_sum(struct eip160_pdev *pdev,
                                const struct pn_thold_sum *pnthr_sum);

/**
	Gets the SA packet number threshold summary for the Egress device.

	The packet number threshold summary register indicates any MACsec egress SA for which
	the packet number will expire imminently such that the host must initiate retrieval of
	a new (next) key.

	@pdev: Pointer to type void (\ref ).
	@pnthr_sum Pointer to threshold summary (\ref pn_thold_sum).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_egr_pn_thold_sum(struct eip160_pdev *pdev,
                                struct pn_thold_sum *pnthr_sum);

/**
	Configures the consistency check matching rule of Ingress or  device.
		   In the Ingress data path, packets are partially re-inspected after
		   leaving the MACsec engine core for a consistency check.

	@pdev: Pointer to type void (\ref ).
	@ccm_par Pointer to consistency check rule param (\ref icc_match_parm).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_ccm_rule(struct eip160_pdev *pdev,
                            const struct icc_match_parm *ccm_par);

/**
	Gets the consistency check matching rule of Ingress or  device.
		   In the Ingress data path, packets are partially re-inspected after
		   leaving the MACsec engine core for a consistency check.

	@pdev: Pointer to type void (\ref ).
	@ccm_par Pointer to consistency check rule param (\ref icc_match_parm).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_ing_ccm_rule(struct eip160_pdev *pdev,
                            struct icc_match_parm *ccm_par);

/**
	Configures the consistency check entry enable control for
		   the Ingress device. In the Ingress data path, packets are partially
		   re-inspected after leaving the MACsec engine core for a consistency
		   check.

	@pdev: Pointer to type void (\ref ).
	@ee_ctrl Pointer to consistency check rule param (\ref ccm_ena_ctrl).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_ccm_ena_ctrl(struct eip160_pdev *pdev,
                                const struct ccm_ena_ctrl *ee_ctrl);

/**
	Gets the consistency check entry enable flags of Ingress or
		   device. In the Ingress data path, packets are partially re-inspected
		   after leaving the MACsec engine core for a consistency check.

	@pdev: Pointer to type void (\ref ).
	@ccm_par Pointer to consistency check rule param (\ref ccm_ena_flags).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_ing_ccm_ena_flags(struct eip160_pdev *pdev, 
                                struct ccm_ena_flags *ccm_par);

/**
	Configures the consistency check control of Ingress or  device.
	This offers functionality to configure what action (drop/pass) is to be taken
	when non-matching control & non-control frames are received.

	@pdev: Pointer to type void (\ref ).
	@ccm_parc Pointer to consistency check control (\ref ccknm_flow_ctrl).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_ccknm_flow_ctrl(struct eip160_pdev *pdev,
                                 const struct ccknm_flow_ctrl *ccm_parc);

/**
	Gets the Per-SA statistics of Ingress or  device.
		   The statistics counters are maintained in RAM to reduce the size
		   of the engine.

	@pdev: Pointer to type void (\ref ).
	@sa_stats Pointer to Per-SA counters (\ref ing_sa_stats).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_ing_sa_stats(struct eip160_pdev *pdev,
                            struct ing_sa_stats *sa_stats);

/**
	Gets the Per-SA statistics for the Egress device.
		   The statistics counters are maintained in RAM to reduce the size
		   of the engine.

	@pdev: Pointer to type void (\ref ).
	@sa_stats Pointer to Per-SA counters (\ref egr_sa_stats).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_egr_sa_stats(struct eip160_pdev *pdev,
                            struct egr_sa_stats *sa_stats);

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
/**
	Gets the per-VLAN user priority statistics of Ingress or  device.
		   The statistics counters are maintained in RAM to reduce the size
		   of the engine.

	@pdev: Pointer to type void (\ref ).
	@vlan_stats Pointer to per-VLAN UP counters (\ref ing_vlan_stats).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_ing_vlan_stats(struct eip160_pdev *pdev,
                              struct ing_vlan_stats *vlan_stats);

/**
	Gets the per-VLAN User Priority statistics for the Egress device.
		   The statistics counters are maintained in RAM to reduce the size
		   of the engine.

	@pdev: Pointer to type void (\ref ).
	@vlan_stats Pointer to per-VLAN UP counters (\ref egr_vlan_stats).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_egr_vlan_stats(struct eip160_pdev *pdev,
                              struct egr_vlan_stats *vlan_stats);
#endif

/**
	Gets the global statistics of Ingress or  device.
		   The statistics counters are maintained in RAM to reduce the size
		   of the engine.

	@pdev: Pointer to type void (\ref ).
	@glob_stats Pointer to global counters (\ref ing_global_stats).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_ing_global_stats(struct eip160_pdev *pdev,
                                struct ing_global_stats *glob_stats);

/**
	Gets the global statistics for the Egress device.
		   The statistics counters are maintained in RAM to reduce the size
		   of the engine.

	@pdev: Pointer to type void (\ref ).
	@glob_stats Pointer to global counters (\ref egr_global_stats).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_egr_global_stats(struct eip160_pdev *pdev,
                                struct egr_global_stats *glob_stats);

#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
/**
	Gets the debug statistics of Ingress or  device.
		   The statistics counters are maintained in RAM to reduce the size
		   of the engine.

	@pdev: Pointer to type void (\ref ).
	@dbg_stats Pointer to debug counters (\ref debug_stats).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_ing_debug_stats(struct eip160_pdev *pdev,
                               struct debug_stats *dbg_stats);

/**
	Gets the debug statistics for the Egress device.
		   The statistics counters are maintained in RAM to reduce the size
		   of the engine.

	@pdev: Pointer to type void (\ref ).
	@dbg_stats Pointer to Debug Counters (\ref debug_stats).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_egr_debug_stats(struct eip160_pdev *pdev,
                               struct debug_stats *dbg_stats);
#endif

/**
	Configures behavior of the counters of Ingress or  device.
		   The counters can be made to saturate, and/or to clear on read:
	a) Saturating means that the counter remains at its maximum value of
	   0xFFFF_FFFF_FFFF_FFFF when it overflows and does not wrap around to zero.
	b) Clear on read means that the counter is cleared to zero when its data is
	   read. Reading and clearing is an atomic operation performed by hardware;
	   no counted events are lost between obtaining the value and clearing the
	   counter.

	@pdev: Pointer to type void (\ref ).
	@cnt_ctrl Pointer to counter control (\ref cntrs_ctrl).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_cntrs_ctrl(struct eip160_pdev *pdev,
                              const struct cntrs_ctrl *cnt_ctrl);
int eip160_cfg_egr_cntrs_ctrl(struct eip160_pdev *pdev,
                              const struct cntrs_ctrl *cnt_ctrl);

/**
	Configures frame and octet counter thresholds of Ingress or Egress
	device.

	\detals The counters summary registers hold the summary on which of
	statistics counters crossed the programmed threshold in this API . The frame
	threshold is used for the frame counters. The octet threshold is used for
	the octet counters.

	Note: Setting the threshold registers to zero will only cause summary
	events (Global statistics summary, Per-SA statistics summary and All SA
	counters statistics summary) in the case of counter overflow (rollover from
	maximum value to zero).

	@pdev: Pointer to type void (\ref ).
	@cnt_thr Pointer to counter thresholds (\ref frm_oct_thold).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_cntrs_thold(struct eip160_pdev *pdev,
                               const struct frm_oct_thold *cnt_thr);
int eip160_cfg_egr_cntrs_thold(struct eip160_pdev *pdev,
                               const struct frm_oct_thold *cnt_thr);

/**
	Configures the security fail and error mask of Ingress or  device.

	A packet for which a security failure is declared (using the secfail_mask
	fields in these registers) is tagged to be transmitted with a CRC error.

	@pdev: Pointer to type void (\ref ).
	@sec_fail Pointer to SEC fail events (\ref sec_fail_evnt).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_sec_fail_evnt(struct eip160_pdev *pdev,
                                 const struct sec_fail_evnt *sec_fail);

/**
	Configures the security fail and error mask for the Egress device.

	A packet for which a security failure is declared (using the secfail_mask
	fields in these registers) is tagged to be transmitted with a CRC error.

	@pdev: Pointer to type void (\ref ).
	@sec_fail Pointer to SEC fail events (\ref sec_fail_evnt).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_egr_sec_fail_evnt(struct eip160_pdev *pdev,
                                 const struct sec_fail_evnt *sec_fail);

/**
	Clears/Gets the per-SA statistics summary, global statistics
		   summary and per-VLAN-UP statistics summary of Ingress or  device.

	The counter summary registers hold a summary of which statistics
	counters have crossed the programmable threshold. The frame threshold is used
	for the frame counters. The octet threshold is used for the octet counters.
	The following summary flags are available:

	a) One for each global statistics counter.
	b) One for each VLAN statistics counter.
	c) One for each SA statistics block when one of the counters in the SA
	   statistics crossed the corresponding (frame or octet) programmable
	   threshold.
	d) One for each counter of each SA.

	This summary register is a vector with one flag per SA to indicate that any
	counter within the corresponding SA statistics group crossed the programmed
	threshold (there can be multiple at the same time).

	The event of setting a flag in the SA summary register is used to set the
	flag for the corresponding SA in the per-SA summary register and
	consequently to generate the interrupt event.

	@pdev: Pointer to type void (\ref ).
	@thold_sum Pointer to statistics summary (\ref cntrs_thold_sum).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_clr_ing_cntrs_thold_sum(struct eip160_pdev *pdev,
                                   const struct cntrs_thold_sum *thold_sum);
int eip160_clr_egr_cntrs_thold_sum(struct eip160_pdev *pdev,
                                   const struct cntrs_thold_sum *thold_sum);
int eip160_get_ing_cntrs_thold_sum(struct eip160_pdev *pdev,
                                   struct cntrs_thold_sum *thold_sum);
int eip160_get_egr_cntrs_thold_sum(struct eip160_pdev *pdev,
                                   struct cntrs_thold_sum *thold_sum);

/**
	Clears the interrupt status of Ingress or  device.

	Clears the context, HW & SW interrupt status, of the currently active
	frame.

	@pdev: Pointer to type void (\ref ).
	@cr_intc Pointer to CC interrupts (\ref crypto_intrp_csr).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_crypto_intrp_ctrl(struct eip160_pdev *pdev,
                                   const struct crypto_intrp_csr *cr_intc);
int eip160_cfg_egr_crypto_intrp_ctrl(struct eip160_pdev *pdev,
                                   const struct crypto_intrp_csr *cr_intc);

/**
	Gets the interrupt status of Ingress or  device.

	Gets the context, HW & SW interrupt status, of the currently active
	frame.

	@pdev: Pointer to type void (\ref ).
	@cr_ints Pointer to CC interrupts (\ref crypto_intrp_csr).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_ing_crypto_intrp_stat(struct eip160_pdev *pdev,
                                   struct crypto_intrp_csr *cr_ints);
int eip160_get_egr_crypto_intrp_stat(struct eip160_pdev *pdev,
                                   struct crypto_intrp_csr *cr_ints);

/**
	Configures the sequence number threshold of Ingress or Egress
	device.

	The EIP-62 provides the possibility of setting a separate threshold
	for cipher suites with 32-bit and 64-bit packet numbering.

	These thresholds can be configured to trigger an informative error e8
	indicating that the inbound sequence number equals the programmed value in
	this register, which can trigger an interrupt. With this interrupt, the
	system has time to start a re-key before the rollover error fires. If this
	threshold is non-zero and the next (incremented) sequence number is equal to
	or greater than this value, the sequence number threshold overflow bit e8 in
	the result token is set, e8 and e10 errors both fire if the sequence number
	rolls over to zero.

	If this threshold is set to zero, the error bit e8 is only set on a rollover
	together with the roll-over error bit e10.

	Note: For efficiency, this feature should be used via the packet number
	threshold summary API  \ref eip160_clr_egr_pn_thold_sum,
	\ref eip160_get_egr_pn_thold_sum.

	@pdev: Pointer to type void (\ref ).
	@sn_thr Pointer to SN threshold (\ref sn_thold).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_sn_thold(struct eip160_pdev *pdev,
                            const struct sn_thold *sn_thr);
int eip160_cfg_egr_sn_thold(struct eip160_pdev *pdev,
                            const struct sn_thold *sn_thr);

/**
	Configures the Advanced Interrupt Controller (AIC) device of Ingress
	or Egress.

	The AIC combines all internal interrupts in a single interrupt
	output, providing edge detection of the interrupt pulses and masking.

	Configures the AIC properties, such as “enable control” of disabled vs
	enabled,  “interrupt enable” of enabling the interrupt by setting the
	corresponding 'enable_ctrl' varible, “acknowledge” by resetting the edge
	detector for the interrupt input, “interrupt disable” by disabling the
	interrupt by clearing the corresponding 'enable_ctrl' individual bits per
	interrupt input.

	It is advisable to “acknowledge” an interrupt before enabling it (to prevent
	spurious interrupts).

	@pdev: Pointer to type void (\ref ).
	@intrp_ctrl Pointer to AIC config (\ref adv_intrp_ctrl).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_cfg_ing_adv_intrp_ctrl(struct eip160_pdev *pdev,
                                const struct adv_intrp_ctrl *intrp_ctrl);
int eip160_cfg_egr_adv_intrp_ctrl(struct eip160_pdev *pdev,
                                const struct adv_intrp_ctrl *intrp_ctrl);

/**
	Gets the Advanced Interrupt Controller (AIC) configuration of Ingress
	or Egress device.

	The AIC combines all internal interrupts in a single interrupt
	output, providing edge detection of the interrupt pulses and masking.

	Gets the AIC properties, such as “polarity control” of low level or falling
	edge vs high level or rising edge, “type control” of level vs edge detect,
	“raw status” (i.e. the status before masking) of inactive vs pending,
	“enabled status”  (i.e. the status after masking) of inactive vs pending,
	“enable control” of disabled vs enabled individual bits per interrupt input.

	@pdev: Pointer to type void (\ref ).
	@intrp_stat Pointer to AIC config (\ref adv_intrp_stat).

	Return values:
	- =0: successful
	- <0: error code
*/
int eip160_get_ing_adv_intrp_stat(struct eip160_pdev *pdev,
                                struct adv_intrp_stat *intrp_stat);
int eip160_get_egr_adv_intrp_stat(struct eip160_pdev *pdev,
                                struct adv_intrp_stat *intrp_stat);

int derive_aes_hkey(const u8 *key, u8 *output, u32 keylen);

#endif /*  _EIP160S_STRUCT_H_ */
