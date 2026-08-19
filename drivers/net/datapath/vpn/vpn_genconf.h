/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2019 Intel Corporation.
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

#ifndef __VPN_GENCONF_H_
#define __VPN_GENCONF_H_

#define FW_COMPATIBLE_ID    0xB

#define MAX_VERSION_DESC_LEN 8 /*!< Maximum FW version description string
				* length
				*/
#define MAX_RING 1 /*!< Performance vs complexity */
#define RD_RING_SIZE 16 /*!< Size of RDR buffer i.e the number of RD in this
			 * ring \n (HIA_RDR_y_RING_SIZE.rdr_size =
			 * RD_RING_SIZE * rd_offset
			 */
#define CD_RING_SIZE 16 /*!< Size of CDR buffer i.e the number of CD in this
			 * ring \n (HIA_CDR_y_RING_SIZE.cdr_size =
			 * CD_RING_SIZE * cd_offset
			 */
#define RD_OFFSET 16 /*!< RD(Result Descriptor)  default: 8 DWORDS \n
		      * HIA_CDR_y_DESC_SIZE.cd_offset
		      */
#define CD_OFFSET 16 /*!< CD(Command Descriptor) default: 16 DWORDS  \n
		      * HIA_RDR_y_DESC_SIZE.rd_offset
		      */
#define ACD_SIZE 16 /*!< Size of ACD in number of DWORDS */
#define CTX_SIZE 32 /*!< Size of CTX in number of DWORDS */
#define CD_SIZE 12 /*!< Size of CD words in number of DWORDS */
#define RD_SIZE 12 /*!< Size of RD words in number of DWORDS */
#define IPSEC_TUN_MAX 16 /*!< MAX IPSEC Tunnel number supported. */
#define IPSEC_TUN_SESS_MAX 4 /*!< MAX sessions per tunnel */

#define MAX_DC_PORT_DEQ_SIZE 32 /*!< MAX ring used in the DC DEQ port (1..32) */
#define MAX_DC_PORT_ENQ_SIZE 32 /*!< MAX ring used in the DC ENQ port (1..32) */
#define MAX_DC_BUF_RTN_SIZE 32 /*!< MAX ring used in the DC BUF RTN port (1..32)
				*/
#define DBG_BUF_COUNT 16 /*!< MAX buffer index used in DDR debug print area */
#define DBG_BUF_SIZE 1024

#define GC_PART_SIZE 4096 /*!< Alignment partition size in SRAM */
enum {
	VPN_DBG_CNT0 = 0,
	VPN_DBG_CNT1,
	VPN_DBG_CNT2,
	VPN_DBG_CNT3,
	VPN_DBG_CNT4,
	VPN_DBG_CNT5,
	VPN_DBG_CNT6,
	VPN_DBG_CNT7,
};

enum PROFILING_MACRO {
	PROFILE_START = 1,		/*!<  Firmware profile starts */
	PROFILE_STOP  = 2,		/*!<  Firmware profile stop */
	PROFILE_SHOW  = 3,		/*!<  Firmware profile results show */
};

/**  \brief vpn FW Header File Structure. Its contect is in big endian mode */
struct fw_hdr { /* Note: all fields in this structure should follow fw_endian
		 * setting
		 */
	u8 fw_endian; /*!< FW Endian : 0--Little Endian, 1--Big Endian. It is
		       * set during compilation
		       */
	u8 family; /*!< FW family, refer to genconf. It is set during
		    * compilation. 0-LGM
		    */
	u8 v_maj; /*!< FW v_maj, refer to genconf. It is set during
		   * compilation
		   */
	u8 v_mid; /*!< FW v_mid, refer to genconf. It is set during
		   * compilation
		   */

	u8 v_min; /*!< FW v_min, refer to genconf. It is set during
		   * compilation
		   */
	u8 v_tag; /*!< FW v_tag, refer to genconf. It is set during
		   * compilation
		   */
	u16 genconf_offset; /*!< Genconf offset based on VPN SRAM base address.
			     * It is set by post compilation script
			     */

	u8 v_desc[MAX_VERSION_DESC_LEN]; /*!< FW v_desc, refer to genconf. It is
					  * set during compilation
					  */
	u32 compatible_id; /*!< vpn FW and vpn HAL both should maintain its
			    * compitability id.\nFW's compatible_id is set
			    * during compilation.\nIf FW's compatible_id not
			    * equal to vpn HAL's, it means vpn FW and vpn HAL
			    * are not compatible.\nIn this case, vpn HAL or vpn
			    * FW have to be updated.\nFor FW, normally
			    * compatible_id needs to be increased by 1 if any
			    * change in FW will cause incompatibility,\n
			    * for example, genconf/session action changes.\n
			    * For vpn HAL, it needs to check FW's compatible_id
			    * before starting vpn FW since FW is release in
			    * binary file.\nIt is readonly to vpn FW/HAL.
			    */
	u32 hdr_size; /*!< FW Header size. It is set during compilation*/
} __packed;

/*!
 *\brief This is the structure for EIP97 Command Descriptor DW0
 */
/* Check table 1 of SafeXcel-IP-197-HW3.0_Programmer-Manual_RevA */
struct	e97_cdw_0 {
	union {
		struct {
			u32 buf_size: 17; /*!< size of input data buffer in
					   * bytes
					   */
			u32 res: 5; /*!< reserved */
			u32 last_seg: 1; /*!< last segment indicator */
			u32 first_seg: 1; /*!< first segment indicator */
			u32 acd_size: 8; /*!< additional control data size */
		} __packed field;
		u32 all;
	};
} __packed;

/*!
 *\brief This is the structure for EIP97 Command Descriptor DW1
 */
struct e97_cdw_1 {
	u32 res; /*!< reserved */
} __packed;

/*!
 *\brief This is the structure for EIP97 Command Descriptor DW2
 */
struct e97_cdw_2 {
	u32 bplo; /*!< buffer data pointer (low data pointer DWORD) */
} __packed;

/*!
 *\brief This is the structure for EIP97 Command Descriptor DW3
 */
struct e97_cdw_3 {
	u32 bphi; /*!< buffer data pointer (high data pointer DWORD) */
} __packed;

/*!
 * \brief This is the structure for EIP97 Command Descriptor DW4
 */
struct e97_cdw_4 {
	u32 acdlo; /*!< Additional CData pointer (low ACD pointer DWORD) */
};

/*!
 *\brief This is the structure for EIP97 Command Descriptor DW5
 */
struct e97_cdw_5 {
	u32 acdhi; /*!< Additional CData pointer (high ACD pointer DWORD) */
} __packed;

/*!
 *\brief This is the structure for EIP97 Command Descriptor DW6
 */
/* This is processing engine (PE) control data word 0 as described in table 4
 * The first DWORD is an input token diagram as explained in detail in section
 * 7.2 of SafeXcel-IP-96_HW4.2_Hardware-Reference-and-Programmer-Manual_RevB
 */
struct e97_cdw_6	{
	union {
		struct {
			u32 len: 17; /*!< input packet length */
			u32 ip: 1; /*!< EIP97 Mode */
			u32 cp: 1; /*!< context pointer 64bit */
			u32 ct: 1; /*!< context type, reserved */
			u32 rc: 2; /*!< reuse context */
			u32 too: 3; /*!< type of output */
			u32 c: 1; /*!< context control words present in token */
			u32 iv: 3; /*!< IV??? */
			u32 u: 1; /*!< upper layer header from token */
			u32 type: 2; /*!< type of token, reserved*/
		} __packed field;
		u32 all;
	};
} __packed;

/*!
 *\brief This is the structure for EIP97 Command Descriptor DW7
 */
/* This is processing engine (PE) control data word 1 as described in table 4 */
struct e97_cdw_7 {
	union {
		struct {
			u32 res0: 9; /*!< reserved */
			u32 app_id: 7; /*!< application id e.g for tagging the
					* packet
					*/
			u32 res: 16; /*!< reserved */
		} __packed field;
		u32 all;
	};
} __packed;

/*!
 *\brief This is the structure for EIP97 Command Descriptor DW8
 */
/* This is processing engine (PE) control data word 2 as described in table 4
 * See SafeXcel-IP-197_FW2.6_Firmware-Reference-Manual_RevC section 4.4.1.
 */
struct e97_cdw_8 {
	u32 ctxlo; /*!< The pointer to theContext in Host memory (low pointer
		    * DWORD)
		    */
} __packed;

/*!
 *\brief This is the structure for EIP97 Command Descriptor DW9
 */
/* This is processing engine (PE) control data word 3 as described in table 4 */
struct e97_cdw_9 {
	u32 ctxhi; /*!< The pointer to the Context in Host memory (high pointer
		    * DWORD)
		    */
} __packed;

/*!
 *\brief This is the structure for EIP97 Command Descriptor DW10
 */
/* This is firmware control field word 0 as described in table 4
 * or Extended Command Token word 4 as mentioned in section 3.3.3
 * in SafeXcel-IP-197_FW2.6_Firmware-Reference-Manual_RevC.
 */
struct e97_cdw_a {
	union {
		struct {
			u32 res:24; /*!< reserved */
			u32 hw_service:6; /*!< packet flow selection */
			u32 res0:1; /*!< reserved */
			u32 flow_lup:1; /*!< look up setting */
		} __packed field;
		u32 all;
	};
} __packed;

/*!
 *\brief This is the structure for EIP97 Command Descriptor DW11
 */
/* This is firmware control field word 1 as described in table 4
 * or Extended Command Token word 5 as mentioned in section 3.3.3
 * in SafeXcel-IP-197_FW2.6_Firmware-Reference-Manual_RevC.
 */
struct e97_cdw_b {
	union {
		struct {
			u32 res:8; /*!< reserved */
			u32 offset:8; /*!< offset to the end of the bypassed
				       * data from the beginning of the packet
				       */
			u32 nxthdr:8; /*!< only for LAIP-TB outbound, for other
				       * packet flows must be 0
				       */
			u32 res0:8; /*!< reserved */
		} __packed field;
		u32 all;
	};
} __packed;

/*!
 *\brief This is the structure for Command Descriptor (CD) words.
 */
struct e97_cdw {
	struct e97_cdw_0 dw0; /*!<Command Descriptor word 0 */
	struct e97_cdw_1 dw1; /*!<Command Descriptor word 1 */
	struct e97_cdw_2 dw2; /*!<Command Descriptor word 2 */
	struct e97_cdw_3 dw3; /*!<Command Descriptor word 3 */
	struct e97_cdw_4 dw4; /*!<Command Descriptor word 4 */
	struct e97_cdw_5 dw5; /*!<Command Descriptor word 5 */
	struct e97_cdw_6 dw6; /*!<Command Descriptor word 6 */
	struct e97_cdw_7 dw7; /*!<Command Descriptor word 7 */
	struct e97_cdw_8 dw8; /*!<Command Descriptor word 8 */
	struct e97_cdw_9 dw9; /*!<Command Descriptor word 9 */
	struct e97_cdw_a dwa; /*!<Command Descriptor word 10 */
	struct e97_cdw_b dwb; /*!<Command Descriptor word 11 */
	u32    res[4];
} __packed;

/*!
 *\brief This is the structure for EIP97 Result Descriptor DW0
 */
struct e97_rdw_0	{
	union {
		struct {
			u32 buf_size: 17; /*!< size of the output data segment
					   * buffer in bytes
					   */
			u32 res: 3; /*!< reserved */
			u32 desc_ovf: 1; /*!< buffer overflow error indicator */
			u32 buf_ovf: 1;	/*!< buffer overflow error indicator */
			u32 last_seg: 1; /*!< last segment indicator */
			u32 first_seg: 1; /*!< first segment indicator */
			u32 result_size: 8; /*!< number of result data words
					     * written to this result descriptor
					     */
		} __packed field;
		u32 all;
	};
} __packed;

/*!
 *\brief This is the structure for EIP97 Result Descriptor DW1
 */
struct e97_rdw_1 {
	u32 res; /*!< reserved */
} __packed;

/*!
 *\brief This is the structure for EIP97 Result Descriptor DW2
 */
struct e97_rdw_2 {
	u32 bplo; /*!< data buffer pointer (low data pointer DWORD) */
} __packed;

/*!
 *\brief This is the structure for EIP97 Result Descriptor DW3
 */
struct e97_rdw_3 {
	u32 bphi; /*!< data buffer pointer (high data pointer DWORD)*/
} __packed;

/*!
 *\brief This is the structure for EIP97 Result Descriptor DW4
 */
struct e97_rdw_4 {
	union {
		struct {
			u32 pkt_len: 17; /*!< the length of the output packet in
					  * bytes; \nexcluding the appended
					  * result fields
					  */
			u32 err_code:15; /*!<  the error code (E14..E0) that is
					  * returned by the EIP-96 Packet Engine
					  */
		} __packed field;
		u32 all;
	};
} __packed;

/*!
 *\brief This is the structure for EIP97 Result Descriptor DW5
 */
struct e97_rdw_5	{
	union {
		struct {
			u32 bypass_len: 4; /*!< the number of DWORDs of
					    * Bypass_Token_Words in the Result
					    * Descriptor
					    */
			u32 E15: 1; /*!< the error code (E15) that is returned
				     * by the EIP-96 Packet Engine
				     */
			u32 EO0:  1;
			u32 EO1:  1;
			u32 EO2:  1;
			u32 EO14: 1;
			u32 res: 12; /*!< reserved */
			u32 H: 1; /*!< Hash Byte field appended at the end of
				   * the packet data flag
				   */
			u32 hash_len: 6; /*!< The number of appended hash bytes
					  * at the end of the packet data. \n
					  * Valid when H = 1
					  */
			u32 B: 1; /*!< Generic Bytes field appended at the end
				   * of the packet data flag
				   */
			u32 C: 1; /*!< Checksum field appended at the end of the
				   * packet data flag
				   */
			u32 N: 1; /*!< Next Header field appended at the end of
				   * the packet data flag
				   */
			u32 L: 1; /*!< Length field appended at the end of the
				   * packet data flag
				   */
		} __packed field;
	};
} __packed;

/*!
 *\brief This is the structure for EIP97 Result Descriptor DW6
 */
struct e97_rdw_6 {
	union {
		struct {
			u32 res0:9; /*!< reserved */
			u32 app_id: 7; /*!< application id wrote in the cdw */
			u32 res: 16; /*!< reserved */
		} __packed field;
		u32 all;
	};
} __packed;

/*!
 *\brief This is the structure for EIP97 Result Descriptor DW7
 */
struct e97_rdw_7	{
	union {
		struct {
			u32 next_hdr: 8; /*!< next header result value from
					  * IPSec trailer
					  */
			u32 pad_len: 8;	/*!< number of detected (and removed)
					 * padding bytes
					 */
			u32 res: 16; /*!< reserved*/
		} __packed field;
		u32 all;
	};
} __packed;

/*!
 *\brief This is the structure for Result Descriptor (RD) words.
 */
struct e97_rdw {
	struct e97_rdw_0 dw0; /*!<Result Descriptor word 0 */
	struct e97_rdw_1 dw1; /*!<Result Descriptor word 1 */
	struct e97_rdw_2 dw2; /*!<Result Descriptor word 2 */
	struct e97_rdw_3 dw3; /*!<Result Descriptor word 3 */
	struct e97_rdw_4 dw4; /*!<Result Descriptor word 4 */
	struct e97_rdw_5 dw5; /*!<Result Descriptor word 5 */
	struct e97_rdw_6 dw6; /*!<Result Descriptor word 6 */
	struct e97_rdw_7 dw7; /*!<Result Descriptor word 7 */
	u32    res[8];
} __packed;

/*!
 *\brief IPSEC mode setting
 */
enum IPSEC_MODE {
	ESP_TU = 0, /*!< IPSEC in ESP Tunnel mode */
	ESP_TR, /*!< IPSEC in ESP Transport mode */
	AH_TU, /*!< IPSEC in AH Tunnel mode */
	AH_TR /*!< IPSEC in AH Transport mode */
};

/*!
 *\brief IPSEC ESP token next header info from ingress DC descriptor
 */
enum ESP_NEXT_HDR {
	ESP_NH_IP,
	ESP_NH_IP6,
	ESP_NH_UDP,
	ESP_NH_TCP,
	ESP_NH_GRE,
	ESP_NH_ICMP,
	ESP_NH_ICMP6,
	ESP_NH_L2TP,
	ESP_NH_MAX = 16
};

/*!
 *\brief This is the structure for IPSec info shared between VPN HAL and VPN FW
 */
struct ipsec_info {
	/* EIP197 specific */
	struct e97_cdw	cd_info; /*!< EIP97 CD(Command Descriptor Information)*/
	u8 cd_size; /*!< Command Descriptor Size in DWORD (4 bytes). If follow
		     * this implementation proposal, its size is EIP97_CD_SIZE
		     */
	u8 mode; /*!< Choose IPSEC configuration: ESP/AH encapsulation, tunnel
		  * or transport mode. \nThere are four values: ESP_TU, ESP_TR,
		  * AH_TU, AH_TR and default is ESP_TU. \nThe value must be set
		  * in inbound tunnel by VPN HAL
		  */

	/* IP header variables */
	u8 ipcsum_instr_offset; /*!< IPv4/6 instruction offset (in bytes)
				 * in the ACD for crypto to encrypt/decrypt data
				 */

	/* padding variables */
	u8 pad_en; /*!< flag whether pad needed or not. For block ciphers, it
		    * must be set to 1, otherwise 0
		    */
	u8 pad_instr_offset; /*!< the insert instruction offset (in bytes) in
			      * the ACD for padding. valid only if pad_en is 1.
			      * VPN FW need to update it accordingly if pad_en
			      * is set
			      */
	u8 crypto_instr_offset; /*!< direction instruction offset (in bytes) in
				 * the ACD for crypto to encrypt/decrypt data
				 */
	u8 blk_size; /*!< crypto block size for calculating padding length in
		      * bytes
		      */

	/* AES-CCM specific */
	u32 hash_pad_instr_offset; /*!< insert instr offset (in bytes) in the
				    * ACD for adding hash padding length. \n
				    * Offset > 0 for AES-CCM mode and must be 0
				    * for other modes.
				    */

	u32 msg_len_instr_offset; /*!< message length instr offset (in bytes) in
				   * the ACD. \n
				   * Offset > 0 for AES-CCM mode and must be 0
				   * for other modes.
				   */

	/* IP header length */
	u8 ip_hlen; /*!< IP header length, use only for ESP transport
		     */

	/* Integrity Check Value (ICV) */
	u8 icv_len; /*!< ICV length in bytes. Used by FW for MTU sanity
		     * checking
		     */

	/* instruction vectors (IV), check its source i.e from token, context
	 * record, or hardware generated randomly
	 */
	u8 iv_len; /*!< IV length in bytes. Used by FW for MTU sanity
		    * checking
		    */
} __packed;

/* Encryption PPv4 to VPN-FW format */
struct dc_desc0_enc {
	union {
		struct {
		/* DC Desc 0 */
			u32 ipoffset:8;
			u32 enc:1;
			u32 subif:4;
			u32 next_hdr:4;
			u32 vp_sess:3;
			u32 tunnel_id:5;
			u32 res:3;
			u32 class:4;
		} __packed field;
		u32 all;
	};
} __packed;

/* Decryption PPv4 to VPN-FW format */
struct dc_desc0_dec {
	union {
		struct {
		/* DC Desc 0 */
			u32 ipoffset:8;
			u32 enc:1;
			u32 uc_ip_offset:8;
			u32 res:3;
			u32 tunnel_id:5;
			u32 res1:3;
			u32 class:4;
		} __packed field;
		u32 all;
	};
} __packed;

/* Encryption VPN-FW to host format */
struct dc_desc0_out_enc {
	union {
		struct {
			u32 tunnel_id:5;
			u32 res:4;
			u32 subif_offset:4;
			u32 res2:2;
			u32 enc:1;
			u32 res3:12;
			u32 class:4;
		} __packed field;
		u32 all;
	};
} __packed;

/* Decryption VPN-FW to PP IPSec UC format */
struct dc_desc0_out_dec_uc {
	union {
		struct {
			u32 next_hdr:8;
			u32 enc:1;
			u32 subif_offset:4;
			u32 uc_ip_offset:7;
			u32 tunnel_id:8;
			u32 class:4;
		} __packed field;
		u32 all;
	};
} __packed;

/* Decryption VPN-FW/PP IPSec UC to host format */
struct dc_desc0_out_dec {
	union {
		struct {
			u32 nexthdr:8;
			u32 res:1;
			u32 subif_offset:4;
			u32 res2:2;
			u32 enc:1;
			u32 res3:4;
			/*! bit 27 is used by HW as egress bit and cannot
			 *  be used by FW, hence tunnel id cannot be 8 bits long
			 *  keep it aligned to encrypt path (dc_desc0_out_enc)
			 */
			u32 tunnel_id:5;
			u32 res4:3;
			u32 class:4;
		} __packed field;
		u32 all;
	};
} __packed;

struct dc_desc0 {
	union {
		struct dc_desc0_dec d;
		struct dc_desc0_enc e;
		struct dc_desc0_out_dec_uc o_d_uc;
		struct dc_desc0_out_dec o_d;
		struct dc_desc0_out_enc o_e;
	};
} __packed;

struct dc_desc1 {
	union {
		struct {
		/* DC Desc 1 */
			u32 buf_ptr:4;
			u32 res :3;
			u32 pmac:1;
			u32 dev_qos :4;
			u32 prel2:2;
			u32 color :2;
			u32 port :8;
			u32 frag_cnt:4;
			u32 class :4;
		} __packed field;
		u32 all;
	};
} __packed;

struct dc_desc2 {
	union {
		struct {
		/* DC Desc 2 */
			//u32 byte_offset:4;
			u32 buf_ptr;
		} __packed field;
		u32 all;
	};
} __packed;

struct dc_desc3 {
	union {
		struct {
		/* DC Desc 3 */
			u32 data_len:14;
			u32 res:2;
			u32 poll_policy:8;
			u32 src_pool:4;
			u32 eop:1;
			u32 sop:1;
			u32 res1:1;
			u32 own:1;
		} __packed field;
		u32 all;
	};
} __packed;

struct dc_desc {
	struct dc_desc0 desc0;
	struct dc_desc1 desc1;
	struct dc_desc2 desc2;
	struct dc_desc3 desc3;
} __packed;

struct buf_rtn_dc_desc0 {
	union {
		struct {
		/* buf rtn desc 0 */
			u32 bufl;
		} __packed field;
		u32 all;
	};
} __packed;

struct buf_rtn_dc_desc1 {
	union {
		struct {
		/* buf rtn desc 1 */
			u32 pollid:4;
			u32 res:10;
			u32 policyid:8;
			u32 res1: 1;
			u32 bufh:4;
			u32 res2: 5;
		} __packed field;
		u32 all;
	};
} __packed;

struct buf_rtn_dc_desc {
	struct buf_rtn_dc_desc0 desc0;
	struct buf_rtn_dc_desc1 desc1;
} __packed;

struct ctx {
	u32 buf[CTX_SIZE];
} __packed;

struct tkn {
	u32 buf[ACD_SIZE];
} __packed;

struct ipsec_act {
	u32 dw0_mask; /*!< mask set by PPA
		       * mask 0: reset the bit
		       * mask 1: keep the bit value
		       */
	u32 dw0_val;  /*!< new dw value set by PPA
		       * final dw0 = (receiving_dw0 & dw0_mask ) | dw0_val
		       */
	u32 dw1_mask; /*!< mask set by PPA
		       * mask 0: reset the bit
		       * mask 1: keep the bit value
		       */
	u32 dw1_val;  /*!< new dw value set by PPA
		       * final dw0 = (receiving_dw0 & dw0_mask ) | dw0_val
		       */
	u32 enq_qos : 1; /*!< flag to indicate enqueue with or without qos
			  *   1: enqueue with qos
			  *   0: enqueue bypass qos
			  */
} __packed;

/*!
 * \brief FW Current state
 */
enum state_definition {
	VPN_NO_RUN = 0,         /*!< Initial FW not started yet */
	VPN_PROC_START = 1,     /*!< FW main() is entered */
	VPN_UART_READY,         /*!< ARC UART setup is ready */
	VPN_UDMA_READY,         /*!< ARC uDMA setup is ready */
	VPN_TIMER_READY,        /*!< ARC TIMER setup is ready */
	VPN_E97_HW_SET,         /*!< EIP197 HW setup is ready */
	VPN_DC_DEQ_DESC_AVAIL,  /*!< DC DEQ descriptor is read by FW */
	VPN_ENC_CONFIG_CMD,	/*!< FW reads ENC info from ipsec_info and DC
				 *   desc
				 */
	VPN_DEC_CONFIG_CMD,     /*!< FW reads DEC info from ipsec_info and DC
				 *   desc
				 */
	VPN_PREP_EIP197_CMD,    /*!< FW prepares EIP197 CDR, RDR command
				 *   words
				 */
	VPN_SEND_EIP197_CMD,    /*!< FW sends EIP197 commands */
	VPN_WAIT_EIP197_RES,    /*!< FW starts reading EIP197 results */
	VPN_DC_ENQ_DESC_AVAIL   /*!< DC ENQ descriptor is sent by FW */
};

enum IPSEC_DIR {
	OUT = 0,
	INB = 1
};

enum IPSEC_TUN_FLAG {
	TUN_DIS = 0, /* Disable */
	TUN_EN, /* Enable */
	TUN_UPD, /* Update */
	TUN_MAX
};

enum FW_LOAD_STATUS {
	VPN_FW_NOT_LOADED = 0,
	VPN_FW_LOADED_IN_CCM,
	VPN_FW_GENCONF_SET,
	VPN_FW_STARTS,
	VPN_FW_ACK_IA,
	INT_DBG_WRITE,
	INT_DBG_IO,
	INT_TUNL_UPD,
	INT_TUNL_CFG,
	INT_TXIN_UPDATE
};

enum VPN_DBG_PRINT_OUT {
	LINUX_PRINT = 0,
	UART_PRINT
};

enum VPN_DBG_F {
	VPN_DBG_F_OFF = 0,
	VPN_DBG_F_TX = BIT(0), /*!< print TX packet information when it is set
				*
				*/
	VPN_DBG_F_RX = BIT(1), /*!< print RX packet information when it is set
				*
				*/
	VPN_DBG_F_RW = BIT(2), /*!< Enable ARC memory read/write operation when
				*  it is set
				*/
	VPN_DBG_F_SYS = BIT(15) /*!< Debugging system level, valid for TX and
				 * RX when it is set
				 */
};

/*!
 *\brief Enumeration for genconf dbg_rw_reg_owner
 */
enum VPN_DBG_RW_OWNER {
	VPN_DBG_RW_OWNER_NONE = 0, /*!< owners list: no owner */
	VPN_DBG_RW_OWNER_FW, /*!< owners belong to VPN FW */
	VPN_DBG_RW_OWNER_DRV, /*!< owners belong to VPN driver */
};

/*!
 *\brief Enumeration for genconf dbg_rw_reg_mode
 */
enum VPN_DBG_RW_MODE {
	VPN_DBG_RW_READ = 0, /*!< read value from one ARC address */
	VPN_DBG_RW_WRITE, /*!< write value to one ARC address */
	VPN_DBG_RW_INFO, /*!< get FW informaton from ARC FW */
};

/*@}*/ /* VPN_PROFILING */

/** \addtogroup  VPN_PROFILING */
/*@{*/
/*\brief This is the structure for api level and full profiling.
 */
struct vpn_profiling {
	u64 ipsec_dec_sum; /*!< Accumulated cycles for decryption preparation */
	u64 ipsec_enc_sum; /*!< Accumulated cycles for encryption preparation */
	u64 crypto_api_sum; /*!< Accumulated cycles for CD/RD preparation */
	u64 result_avail_sum; /*!< Accumulated cycles for preparing result */
	u64 e197_ivld_sum; /*!< Accumulated cycles for tunnel invalidate */
	u64 upd_tunl_cfg_sum; /*!< Accumulated cycles for tunnel update */
	u64 buf_rtn_dc_sum; /*!< Accumulated cycles for buffer return DC */
	u32 profile_flag; /*!< wk full profiling control flag */
	u32 pkt; /*!< no: of acc pkts for profiling */
	u32 num_upd_tunl; /*!< no: of update tunnel occurences */
	u32 num_e197_invl; /*!< no: of eip197 inval occurences */
	u32 num_buf_rtn; /*!< no: of buffer return occurences */
} __packed;

/* UMT TXIN information.
 */
#define MAX_ARC_TIMER 2
#define MAX_PORT_PER_TIMER 8

enum vpn_umt_type {
	VPN_UMT_TXIN,
	VPN_UMT_RXOUT,
};

struct umt_txin_update {
	u8 period;
	struct umt_update_info {
		u32 addr;
		u8 port;
		u8 type;
		u8 dpid;
	} __packed info[MAX_PORT_PER_TIMER];
} __packed;

/** \addtogroup VPN_GENERAL_CONFIG */
/*@{*/
/*\brief VPN General Configuration Structure Definition. Accessible to
 * both ATOM and ARCEM
 */
struct genconf {
	struct fw_hdr hdr; /*!< VPN FW header. It is set during
			    * compilation or by post scripts. It is
			    * readonly to VPN FW/HAL
			    */
	u32 res1[8]; /*!< Reserved spare 32 bytes */

	u32 fw_cur_state[MAX_RING]; /*!< FW current state per ring
				     * based. Set by VPN FW, read only
				     * to VPN HAL.
				     */
	u32 dbg_addr[DBG_BUF_COUNT]; /*!< VPN FW real time debug buffer
				      * FW writes, HAL reads
				      * Note: Driver should make sure
				      * allocate lower 4G DDR range
				      */
	u16 fw_dbg_state; /*!< FW global debug state. Set by VPN FW,
			   * read only to VPN HAL.
			   */
	u16 dbg_enable; /*!< Used to enable/disable debugging
			 * message support set by VPN HAL.
			 * For detail, refer to \ref enum VPN_DBG_F
			 */
	u8 dbg_wr_idx; /*!< VPN FW real time debugging:FW writes,
			* HAL reads
			*/
	u8 dbg_wr_cnt; /*!< VPN FW real time debugging: FW writes,
			* HAL reads
			*/
	u8 dbg_rd_idx; /*!< VPN FW real time debugging: HAL writes,
			* FW reads
			*/
	u8 dbg_rd_cnt; /*!< VPN FW real time debugging: HAL writes,
			* FW reads
			*/

	u8 dbg_rw_reg_owner;/*!< \ref enum VPN_DBG_RW_OWNER
			     * owner transimition flow:
			     * Initially the owner is none, ie,
			     * VPN_DBG_RW_OWNER_NONE. Once VPN
			     * driver set dbg_rx_reg_mode/... value,
			     * driver set owner to
			     * VPN_DBG_RW_OWNER_FW. FW handle it and
			     * write the data back if necessary
			     * and set owner to VPN_DBG_RW_OWNER_DRV
			     * Driver read and print the message
			     * and set owner to
			     * VPN_DBG_RW_OWNER_NONE
			     */

	u8 dbg_rw_reg_mode; /*!< \ref enum VPN_DBG_RW_MODE.
			     * Note: for VPN_DBG_RW_INFO, its output
			     * is not via dbg_rw_reg_value,
			     * but via FW dump msg
			     */
	u8 dbg_rw_reg_bit_offset; /*!< the bit offset to read/write:
				   * 0 to 31
				   */
	u8 dbg_rw_reg_bit_num; /*!< the num of bit to read/write:
				* 1 to 32
				*/
	u8 dbg_out; /*!< Debug print output: 0 -- kernel prints,
		     * 1 -- Arc UART print
		     */
	u8 e97_init_flag; /*!< The flag is configured by VPN HAL
			   * during kernel boot up. \n
			   * 1: EIP97 HW is initialized by Linux
			   * Driver
			   * 0: VPN FW need to initialise EIP97 e.g
			   * in chiptest
			   */
	u8 e97_ring_nr; /*!< Number of ring configured for VPN FW.
			 * VPN HAL writes, FW reads
			 */
	u8 e97_ring_id; /*!< The first ring id configured for VPN
			 * FW. VPN HAL writes, FW reads
			 */
	u8 load_stat; /*!< Use to communicate between HAL and FW,
		       * to indicate if FW is loaded, and can start
		       * running \nIf it is ready, HAL can program
		       * the GENCONF safely \n
		       * VPN_FW_NOT_LOADED: Initial state \n
		       * VPN_FW_LOADED_IN_CCM: FW image is copied
		       * to ICCM, DCCM and SRAM. \n
		       * VPN_FW_GENCONF_SET: GENCONF setting is
		       * ready by HAL and FW can continue \n
		       * VPN_FW_STARTS: FW starts running
		       */
	u8 a2e_msg; /*!< B0 only, interrupt based, int message from ATOM to
		     * ARCEM6
		     */
	u8 e2a_msg; /*!< B0 only, interrupt based, ack message from ARCEM6 to
		     * ATOM
		     */
	u8 res4; /*!< reserved */

	u32 dbg_rw_reg_addr; /*!< Read/write arc register address:
			      * from ARC point of view
			      */
	u32 dbg_rw_reg_value; /*!< Read/write arc register address:
			       * from ARC point of view
			       */
	u32 vpn_umt_cnt_rx; /*!< UMT CNT RX */
	u32 vpn_umt_cnt; /*!< UMT CNT TX */
	u8  soc; /*!< SOC type, VPN HAL writes, FW reads */
	u8  res[3];
	u32 dc_buf_rtn_cnt; /*!< Number of DC buffer return counter:
			     * Updated by VPN_FW
			     */

	/* IPSEC Tunnel Info base address. VPN HAL defines them per IPSEC
	 * tunnel
	 */
	/*!< IPSEC Tunnel inbound info */
	struct ipsec_info ipsec_info[IPSEC_TUN_MAX];

	/* IPSEC Tunnel Flag */
	u32 ipsec_flag; /*!< Tunnel flag: 0 --its configuration not
			 * valid, 1 --valid, 2 --update
			 * bit [1:0] for tunnel[0], bit [3:2] for tunnel[1]
			 * and so on
			 */

	/* It is base address per tunnel. VPN HAL need to allocate the buffer
	 * for IPSEC traffic. VPN FW copies ACD info from VPN HAL to this
	 * location and do necessary editing before passed to Eip197
	 */
	struct tkn acd_tmpl[IPSEC_TUN_MAX]; /*!< VPN HAL alloc per ACD buffer */
	struct ipsec_act dwt[IPSEC_TUN_MAX]; /*!< DMA descriptor template */

	/*!< ENC session action */
	struct ipsec_act sess_act_out[IPSEC_TUN_MAX]
						[IPSEC_TUN_SESS_MAX];
	/* CQM DC port */
	u32 dc_deq_base; /*!< CQM DC dequeue port mapped address.
			  * VPN HAL writes, VPN FW reads
			  */
	u32 dc_buf_rtn_base; /*!< CQM DC dequeue buffer return address.
			      * VPN HAL writes, VPN FW reads
			      */
	u32 dc_enq_base[2]; /*!< CQM DC enqueue port mapped address.
			     * VPN HAL writes, VPN FW reads
			     * dc_enq_base[1] enqueue with qos
			     * dc_enq_base[0] enqueue bypass qos
			     */
	u8 dc_deq_size; /*!< CQM DC dequeue port number of rings.
			 * Maximum is MAX_DC_PORT_DEQ_SIZE. \n
			 * VPN HAL writes, VPN FW reads
			 */
	u8 dc_buf_rtn_size; /*!< CQM DC dequeue buffer return size.
			     * Maximum is MAX_DC_BUF_RTN_SIZE. \n
			     * VPN HAL writes, VPN FW reads
			     */
	u8 dc_enq_size[2]; /*!< CQM DC enqueue port number of rings.
			    * Maximum is MAX_DC_PORT_ENQ_SIZE. \n
			    * VPN HAL writes, VPN FW reads
			    */
	struct vpn_profiling mprof; /*!< Used for VPN cycle profiling.
				     *   Strictly for lab testing
				     */
	struct umt_txin_update umt_txin[MAX_ARC_TIMER]; /*!< umt txin info */
	u8 umt_txin_enable;

	/*!< Second partition SRAM */
	/* ARC and Host and EIP197 access */
	u32 genconf_2nd_part __aligned(GC_PART_SIZE);
	struct ctx ctx[IPSEC_TUN_MAX]; /*!< IPSEC Tunnel SA info */

	/*!< Third partition SRAM */
	/* ARC and EIP197 access only */
	u32 genconf_3rd_part __aligned(GC_PART_SIZE);

	/*!< Eip197 Result descriptor buffer per Ring*/
	struct e97_rdw rdr[MAX_RING][RD_RING_SIZE];

	/*!< Eip197 Command descriptor buffer per Ring */
	struct e97_cdw cdr[MAX_RING][CD_RING_SIZE];

	/*!< Eip197 ACD buffer per Ring */
	struct tkn acd[MAX_RING][CD_RING_SIZE];

	/*!< Fourth partition SRAM */
	/* Optional for future if needed */
} __packed;

#endif //__VPN_GENCONF_H_

