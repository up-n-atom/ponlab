
/***********************************************************************************
File:				B2MacWepRegs.h
Module:				b2MacWep
SOC Revision:		latest
Generated at:       2024-06-26 12:54:50
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_MAC_WEP_REGS_H_
#define _B2_MAC_WEP_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_MAC_WEP_BASE_ADDRESS                         MEMORY_MAP_UNIT_20017_BASE_ADDRESS
#define	REG_B2_MAC_WEP_CONTROL                           (B2_MAC_WEP_BASE_ADDRESS + 0x0)
#define	REG_B2_MAC_WEP_STATUS                            (B2_MAC_WEP_BASE_ADDRESS + 0x4)
#define	REG_B2_MAC_WEP_SRC_ADDRESS                       (B2_MAC_WEP_BASE_ADDRESS + 0x8)
#define	REG_B2_MAC_WEP_DST_ADDRESS                       (B2_MAC_WEP_BASE_ADDRESS + 0xC)
#define	REG_B2_MAC_WEP_CUR_ADDRESS_OFFSET                (B2_MAC_WEP_BASE_ADDRESS + 0x10)
#define	REG_B2_MAC_WEP_LENGTH                            (B2_MAC_WEP_BASE_ADDRESS + 0x14)
#define	REG_B2_MAC_WEP_KEY_LENGTH                        (B2_MAC_WEP_BASE_ADDRESS + 0x18)
#define	REG_B2_MAC_WEP_KEY_31TO0                         (B2_MAC_WEP_BASE_ADDRESS + 0x1C)
#define	REG_B2_MAC_WEP_KEY_63TO32                        (B2_MAC_WEP_BASE_ADDRESS + 0x20)
#define	REG_B2_MAC_WEP_KEY_95TO64                        (B2_MAC_WEP_BASE_ADDRESS + 0x24)
#define	REG_B2_MAC_WEP_KEY_127TO96                       (B2_MAC_WEP_BASE_ADDRESS + 0x28)
#define	REG_B2_MAC_WEP_CRC_RESULT                        (B2_MAC_WEP_BASE_ADDRESS + 0x2C)
#define	REG_B2_MAC_WEP_TX_ABORT                          (B2_MAC_WEP_BASE_ADDRESS + 0x30)
#define	REG_B2_MAC_WEP_TX_ABORT_STATUS                   (B2_MAC_WEP_BASE_ADDRESS + 0x34)
#define	REG_B2_MAC_WEP_WRITE10                           (B2_MAC_WEP_BASE_ADDRESS + 0x38)
#define	REG_B2_MAC_WEP_WRITE11                           (B2_MAC_WEP_BASE_ADDRESS + 0x3C)
#define	REG_B2_MAC_WEP_MODE                              (B2_MAC_WEP_BASE_ADDRESS + 0x40)
#define	REG_B2_MAC_WEP_WRITE12                           (B2_MAC_WEP_BASE_ADDRESS + 0x44)
#define	REG_B2_MAC_WEP_WRITE13                           (B2_MAC_WEP_BASE_ADDRESS + 0x48)
#define	REG_B2_MAC_WEP_WRITE14                           (B2_MAC_WEP_BASE_ADDRESS + 0x4C)
#define	REG_B2_MAC_WEP_WRITE15                           (B2_MAC_WEP_BASE_ADDRESS + 0x50)
#define	REG_B2_MAC_WEP_WRITE16                           (B2_MAC_WEP_BASE_ADDRESS + 0x54)
#define	REG_B2_MAC_WEP_READ5                             (B2_MAC_WEP_BASE_ADDRESS + 0x58)
#define	REG_B2_MAC_WEP_READ6                             (B2_MAC_WEP_BASE_ADDRESS + 0x5C)
#define	REG_B2_MAC_WEP_READ7                             (B2_MAC_WEP_BASE_ADDRESS + 0x60)
#define	REG_B2_MAC_WEP_HARDWARE_ID_W0                    (B2_MAC_WEP_BASE_ADDRESS + 0x64)
#define	REG_B2_MAC_WEP_CCM_FC_MASK                       (B2_MAC_WEP_BASE_ADDRESS + 0x68)
#define	REG_B2_MAC_WEP_CCM_SC_MASK                       (B2_MAC_WEP_BASE_ADDRESS + 0x6C)
#define	REG_B2_MAC_WEP_CCM_QC_MASK                       (B2_MAC_WEP_BASE_ADDRESS + 0x70)
#define	REG_B2_MAC_WEP_CR_USER_ID                        (B2_MAC_WEP_BASE_ADDRESS + 0x74)
#define	REG_B2_MAC_WEP_FULL_HW_ENC                       (B2_MAC_WEP_BASE_ADDRESS + 0x78)
#define	REG_B2_MAC_WEP_START_ENTRIES_ADDRESS             (B2_MAC_WEP_BASE_ADDRESS + 0x7C)
#define	REG_B2_MAC_WEP_ENTRY_NUMBER                      (B2_MAC_WEP_BASE_ADDRESS + 0x80)
#define	REG_B2_MAC_WEP_LENGTH_OFFSET                     (B2_MAC_WEP_BASE_ADDRESS + 0x84)
#define	REG_B2_MAC_WEP_TKIP_CONTROL                      (B2_MAC_WEP_BASE_ADDRESS + 0x88)
#define	REG_B2_MAC_WEP_TKIP_MIC_CONTROL                  (B2_MAC_WEP_BASE_ADDRESS + 0x8C)
#define	REG_B2_MAC_WEP_WEP_CONTROL                       (B2_MAC_WEP_BASE_ADDRESS + 0x90)
#define	REG_B2_MAC_WEP_TKIP_FRAGMENT_CONTROL             (B2_MAC_WEP_BASE_ADDRESS + 0x94)
#define	REG_B2_MAC_WEP_SHRAM_MASK_WRITE                  (B2_MAC_WEP_BASE_ADDRESS + 0x98)
#define	REG_B2_MAC_WEP_MAX_MPDU_ADDRESS                  (B2_MAC_WEP_BASE_ADDRESS + 0x9C)
#define	REG_B2_MAC_WEP_IV_GENERATE_VALUE                 (B2_MAC_WEP_BASE_ADDRESS + 0xA0)
#define	REG_B2_MAC_WEP_SM_DEBUG_REGISTER                 (B2_MAC_WEP_BASE_ADDRESS + 0xA4)
#define	REG_B2_MAC_WEP_WIRELESS_HEADER_LENGTH            (B2_MAC_WEP_BASE_ADDRESS + 0xA8)
#define	REG_B2_MAC_WEP_FRAGMENT_DB_BASE_ADDR             (B2_MAC_WEP_BASE_ADDRESS + 0xDC)
#define	REG_B2_MAC_WEP_TKIP_FRAGMENT2_CONTROL            (B2_MAC_WEP_BASE_ADDRESS + 0xE0)
#define	REG_B2_MAC_WEP_FRAGMENT_ENTRY_NUM                (B2_MAC_WEP_BASE_ADDRESS + 0xE4)
#define	REG_B2_MAC_WEP_TKIP_FRAGMENT3_CONTROL            (B2_MAC_WEP_BASE_ADDRESS + 0xE8)
#define	REG_B2_MAC_WEP_DESC_ARBITER                      (B2_MAC_WEP_BASE_ADDRESS + 0xEC)
#define	REG_B2_MAC_WEP_TX_INTERRUPTS_EN                  (B2_MAC_WEP_BASE_ADDRESS + 0xF0)
#define	REG_B2_MAC_WEP_TX_INTERRUPTS_STATUS              (B2_MAC_WEP_BASE_ADDRESS + 0xF4)
#define	REG_B2_MAC_WEP_TX_INTERRUPTS_CLEAR               (B2_MAC_WEP_BASE_ADDRESS + 0xF8)
#define	REG_B2_MAC_WEP_TX_FAIL_STATUS                    (B2_MAC_WEP_BASE_ADDRESS + 0xFC)
#define	REG_B2_MAC_WEP_RX_INTERRUPTS_EN                  (B2_MAC_WEP_BASE_ADDRESS + 0x100)
#define	REG_B2_MAC_WEP_RX_INTERRUPTS_STATUS              (B2_MAC_WEP_BASE_ADDRESS + 0x104)
#define	REG_B2_MAC_WEP_RX_INTERRUPTS_CLEAR               (B2_MAC_WEP_BASE_ADDRESS + 0x108)
#define	REG_B2_MAC_WEP_RX_FAIL_STATUS                    (B2_MAC_WEP_BASE_ADDRESS + 0x10C)
#define	REG_B2_MAC_WEP_COMPLETE_STATUS                   (B2_MAC_WEP_BASE_ADDRESS + 0x110)
#define	REG_B2_MAC_WEP_CR_MIC_KEY128_31TO0               (B2_MAC_WEP_BASE_ADDRESS + 0x114)
#define	REG_B2_MAC_WEP_CR_MIC_KEY128_63TO32              (B2_MAC_WEP_BASE_ADDRESS + 0x118)
#define	REG_B2_MAC_WEP_CR_MIC_KEY128_95TO64              (B2_MAC_WEP_BASE_ADDRESS + 0x11C)
#define	REG_B2_MAC_WEP_CR_MIC_KEY128_127TO96             (B2_MAC_WEP_BASE_ADDRESS + 0x120)
#define	REG_B2_MAC_WEP_SEC_LOGGER                        (B2_MAC_WEP_BASE_ADDRESS + 0x124)
#define	REG_B2_MAC_WEP_SEC_LOGGER_ACTIVE                 (B2_MAC_WEP_BASE_ADDRESS + 0x128)
#define	REG_B2_MAC_WEP_AAD_ADDR                          (B2_MAC_WEP_BASE_ADDRESS + 0x12C)
#define	REG_B2_MAC_WEP_AAD_DATA                          (B2_MAC_WEP_BASE_ADDRESS + 0x130)
#define	REG_B2_MAC_WEP_DLM_MPDU_LIST_IDX_OUT             (B2_MAC_WEP_BASE_ADDRESS + 0x134)
#define	REG_B2_MAC_WEP_SEC_LOGGER_FILTER                 (B2_MAC_WEP_BASE_ADDRESS + 0x138)
#define	REG_B2_MAC_WEP_FIFOS_STATUS                      (B2_MAC_WEP_BASE_ADDRESS + 0x13C)
#define	REG_B2_MAC_WEP_MIC_INDICATION_15TO0              (B2_MAC_WEP_BASE_ADDRESS + 0x140)
#define	REG_B2_MAC_WEP_MIC_INDICATION_31TO16             (B2_MAC_WEP_BASE_ADDRESS + 0x144)
#define	REG_B2_MAC_WEP_MIC_INDICATION_35TO32             (B2_MAC_WEP_BASE_ADDRESS + 0x148)
#define	REG_B2_MAC_WEP_USER_ACTIVE_INDICATION_15TO0      (B2_MAC_WEP_BASE_ADDRESS + 0x14C)
#define	REG_B2_MAC_WEP_USER_ACTIVE_INDICATION_31TO16     (B2_MAC_WEP_BASE_ADDRESS + 0x150)
#define	REG_B2_MAC_WEP_USER_ACTIVE_INDICATION_47TO32     (B2_MAC_WEP_BASE_ADDRESS + 0x154)
#define	REG_B2_MAC_WEP_USER_ACTIVE_INDICATION_63TO48     (B2_MAC_WEP_BASE_ADDRESS + 0x158)
#define	REG_B2_MAC_WEP_USER_ACTIVE_INDICATION_71TO64     (B2_MAC_WEP_BASE_ADDRESS + 0x15C)
#define	REG_B2_MAC_WEP_TX_IDLE_INDICATION                (B2_MAC_WEP_BASE_ADDRESS + 0x160)
#define	REG_B2_MAC_WEP_TX_DLM_FC_CONTROL                 (B2_MAC_WEP_BASE_ADDRESS + 0x164)
#define	REG_B2_MAC_WEP_RX_DLM_FC_CONTROL                 (B2_MAC_WEP_BASE_ADDRESS + 0x168)
#define	REG_B2_MAC_WEP_WR_FC_STROBES                     (B2_MAC_WEP_BASE_ADDRESS + 0x16C)
#define	REG_B2_MAC_WEP_CR_MAX_TKIP_FRAG_LENGTH           (B2_MAC_WEP_BASE_ADDRESS + 0x170)
#define	REG_B2_MAC_WEP_CR_USER_BYPASS_HC_RD_FC_31TO0     (B2_MAC_WEP_BASE_ADDRESS + 0x174)
#define	REG_B2_MAC_WEP_CR_USER_BYPASS_HC_RD_FC_35TO32    (B2_MAC_WEP_BASE_ADDRESS + 0x178)
#define	REG_B2_MAC_WEP_BIP_FC_MASK                       (B2_MAC_WEP_BASE_ADDRESS + 0x17C)
#define	REG_B2_MAC_WEP_SPARE_REG                         (B2_MAC_WEP_BASE_ADDRESS + 0x180)
#define	REG_B2_MAC_WEP_TX_MLDAR_CTRL                     (B2_MAC_WEP_BASE_ADDRESS + 0x184)
#define	REG_B2_MAC_WEP_TX_MLDAR_RA_TA_READ_ADDR          (B2_MAC_WEP_BASE_ADDRESS + 0x188)
#define	REG_B2_MAC_WEP_TX_MLDAR_RA_TA_RD_DATA0           (B2_MAC_WEP_BASE_ADDRESS + 0x18C)
#define	REG_B2_MAC_WEP_TX_MLDAR_RA_TA_RD_DATA1           (B2_MAC_WEP_BASE_ADDRESS + 0x190)
#define	REG_B2_MAC_WEP_TX_MLDAR_RA_TA_RD_DATA2           (B2_MAC_WEP_BASE_ADDRESS + 0x194)
#define	REG_B2_MAC_WEP_TX_MLDAR_DBG_BUSY_VEC             (B2_MAC_WEP_BASE_ADDRESS + 0x198)
#define	REG_B2_MAC_WEP_TX_MLDAR_DBG_SMC_ADDR             (B2_MAC_WEP_BASE_ADDRESS + 0x19C)
#define	REG_B2_MAC_WEP_TX_MLDAR_DBG                      (B2_MAC_WEP_BASE_ADDRESS + 0x1A0)
#define	REG_B2_MAC_WEP_TX_MLDAR_SKIP_USER                (B2_MAC_WEP_BASE_ADDRESS + 0x1A4)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_MAC_WEP_CONTROL 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 start : 1; //start, reset value: 0x0, access type: WO
		uint32 controlReg : 15; //Control register, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2MacWepControl_u;

/*REG_B2_MAC_WEP_STATUS 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 active : 1; //WEP2  active, reset value: 0x0, access type: RO
		uint32 crcfail : 1; //WEP2  crcfail, reset value: 0x0, access type: RO
		uint32 micfail : 1; //WEP2  micfail, reset value: 0x0, access type: RO
		uint32 notKeyValid : 1; //not_key_valid, reset value: 0x0, access type: RO
		uint32 notProtectedFrame : 1; //not_protected_frame, reset value: 0x0, access type: RO
		uint32 lengthError : 1; //length_error, reset value: 0x0, access type: RO
		uint32 eivFieldError : 1; //eiv_field_error, reset value: 0x0, access type: RO
		uint32 fragLengthShortError : 1; //frag_length_short_error, reset value: 0x0, access type: RO
		uint32 fragNumNewEqualsPrevError : 1; //frag_num_new_equals_prev_error, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegB2MacWepStatus_u;

/*REG_B2_MAC_WEP_SRC_ADDRESS 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srcAddress : 26; //WEP2  src address, reset value: 0x0, access type: RW
		uint32 encModeConfig : 4; //enc_mode, reset value: 0x0, access type: RW
		uint32 sppAMsduCapable : 1; //spp_a_msdu_capable, reset value: 0x0, access type: RW
		uint32 encryptMode : 1; //encrypt, reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepSrcAddress_u;

/*REG_B2_MAC_WEP_DST_ADDRESS 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dstAddress : 26; //WEP2  dst address, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 keyId : 2; //key_id, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB2MacWepDstAddress_u;

/*REG_B2_MAC_WEP_CUR_ADDRESS_OFFSET 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 curAddressOffset : 16; //WEP2  cur address offset, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2MacWepCurAddressOffset_u;

/*REG_B2_MAC_WEP_LENGTH 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 length : 16; //WEP2  length, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2MacWepLength_u;

/*REG_B2_MAC_WEP_KEY_LENGTH 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 keyLength : 6; //WEP2  key length, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB2MacWepKeyLength_u;

/*REG_B2_MAC_WEP_KEY_31TO0 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 key31To0 : 32; //WEP2  key 31to0, reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepKey31To0_u;

/*REG_B2_MAC_WEP_KEY_63TO32 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 key63To32 : 32; //WEP2  key 63to32, reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepKey63To32_u;

/*REG_B2_MAC_WEP_KEY_95TO64 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 key95To64 : 32; //WEP2  key 95to64, reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepKey95To64_u;

/*REG_B2_MAC_WEP_KEY_127TO96 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 key127To96 : 32; //WEP2  key 127to96, reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepKey127To96_u;

/*REG_B2_MAC_WEP_CRC_RESULT 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crcResult : 32; //WEP2  crc result, reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacWepCrcResult_u;

/*REG_B2_MAC_WEP_TX_ABORT 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAbortPulse : 1; //Trigger to start Tx abort., reset value: 0x0, access type: WO
		uint32 txFlushPulse : 1; //Trigger to start Tx descriptors flush., reset value: 0x0, access type: WO
		uint32 reserved0 : 30;
	} bitFields;
} RegB2MacWepTxAbort_u;

/*REG_B2_MAC_WEP_TX_ABORT_STATUS 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAbortDone : 1; //Tx abort done indication., reset value: 0x1, access type: RO
		uint32 txFlushDone : 1; //Tx flush done indication., reset value: 0x1, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB2MacWepTxAbortStatus_u;

/*REG_B2_MAC_WEP_WRITE10 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 write10 : 32; //write10_w0, reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepWrite10_u;

/*REG_B2_MAC_WEP_WRITE11 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 write11 : 32; //write11_w0, reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepWrite11_u;

/*REG_B2_MAC_WEP_MODE 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mode : 4; //Security mode, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2MacWepMode_u;

/*REG_B2_MAC_WEP_WRITE12 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 write12 : 32; //write12_w0, reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepWrite12_u;

/*REG_B2_MAC_WEP_WRITE13 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 write13 : 32; //write13_w0, reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepWrite13_u;

/*REG_B2_MAC_WEP_WRITE14 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 write14 : 32; //write14_w0, reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepWrite14_u;

/*REG_B2_MAC_WEP_WRITE15 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 write15 : 32; //write15_w0, reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepWrite15_u;

/*REG_B2_MAC_WEP_WRITE16 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 write16 : 32; //write16_w0, reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepWrite16_u;

/*REG_B2_MAC_WEP_READ5 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 read5 : 32; //read5_w0, reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacWepRead5_u;

/*REG_B2_MAC_WEP_READ6 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 read6 : 32; //read6_w0, reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacWepRead6_u;

/*REG_B2_MAC_WEP_READ7 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 read7 : 32; //read7_w0, reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacWepRead7_u;

/*REG_B2_MAC_WEP_HARDWARE_ID_W0 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hardwareIdAes : 1; //hardware id AES, reset value: 0x1, access type: RO
		uint32 hardwareIdOcb : 1; //hardware id OCB, reset value: 0x0, access type: RO
		uint32 hardwareIdCcm : 1; //hardware id CCM, reset value: 0x1, access type: RO
		uint32 hardwareIdWapi : 1; //hardware id WAPI, reset value: 0x1, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB2MacWepHardwareIdW0_u;

/*REG_B2_MAC_WEP_CCM_FC_MASK 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccmFcMask : 16; //ccm_fc_mask, reset value: 0xc78f, access type: RW
		uint32 ccmFcManagementMask : 16; //ccm_fc_management_mask, reset value: 0xc7ff, access type: RW
	} bitFields;
} RegB2MacWepCcmFcMask_u;

/*REG_B2_MAC_WEP_CCM_SC_MASK 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccmScMask : 16; //ccm_sc_mask, reset value: 0xf, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2MacWepCcmScMask_u;

/*REG_B2_MAC_WEP_CCM_QC_MASK 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccmQcMask : 16; //ccm_qc_mask, reset value: 0x8f, access type: RW
		uint32 managementNonceMask : 1; //management_nonce_mask, reset value: 0x1, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegB2MacWepCcmQcMask_u;

/*REG_B2_MAC_WEP_CR_USER_ID 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crUserId : 8; //user_id, used when descriptor_list_en is set to '0'., reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB2MacWepCrUserId_u;

/*REG_B2_MAC_WEP_FULL_HW_ENC 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fullHwEnc : 1; //full_hw_enc, reset value: 0x1, access type: RW
		uint32 rc4PreInitHwEn : 1; //rc4_pre_init_hw_en, reset value: 0x1, access type: RW
		uint32 descriptorListEn : 1; //descriptor_list_en, reset value: 0x1, access type: RW
		uint32 txDescModeGenErrIrqEn : 1; //tx_desc_mode_gen_err_irq_en, reset value: 0x0, access type: RW
		uint32 rxDescModeGenErrIrqEn : 1; //rx_desc_mode_gen_err_irq_en, reset value: 0x0, access type: RW
		uint32 fastGcmpMode : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB2MacWepFullHwEnc_u;

/*REG_B2_MAC_WEP_START_ENTRIES_ADDRESS 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 startEntriesAddress : 22; //start_entries_address, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 dbEntrySize : 4; //entry_length, reset value: 0x4, access type: RW
		uint32 differentDest : 1; //different_dest, reset value: 0x1, access type: RW
		uint32 prepareIv : 1; //prepare_iv, reset value: 0x1, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB2MacWepStartEntriesAddress_u;

/*REG_B2_MAC_WEP_ENTRY_NUMBER 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 entryNumber : 12; //entry_number, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 micEntryNumber : 12; //mic_entry_number, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
	} bitFields;
} RegB2MacWepEntryNumber_u;

/*REG_B2_MAC_WEP_LENGTH_OFFSET 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lengthOffsetEncrypt : 3; //length_offset_encrypt, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 lengthOffsetDecrypt : 3; //length_offset_decrypt, reset value: 0x0, access type: RW
		uint32 reserved1 : 25;
	} bitFields;
} RegB2MacWepLengthOffset_u;

/*REG_B2_MAC_WEP_TKIP_CONTROL 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tkipControlDec : 16; //tkip_control_dec, reset value: 0x580b, access type: RW
		uint32 tkipControlEnc : 16; //tkip_control_enc, reset value: 0x58af, access type: RW
	} bitFields;
} RegB2MacWepTkipControl_u;

/*REG_B2_MAC_WEP_TKIP_MIC_CONTROL 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tkipMicControlHeader : 16; //tkip_mic_control_header, reset value: 0x4009, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2MacWepTkipMicControl_u;

/*REG_B2_MAC_WEP_WEP_CONTROL 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wepControlDec : 16; //wep_control_dec, reset value: 0xb, access type: RW
		uint32 wepControlEnc : 16; //wep_control_enc, reset value: 0x3f, access type: RW
	} bitFields;
} RegB2MacWepWepControl_u;

/*REG_B2_MAC_WEP_TKIP_FRAGMENT_CONTROL 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tkipControlFragmentDec : 16; //tkip_control_fragment_dec, reset value: 0x402b, access type: RW
		uint32 tkipControlFragmentEnc : 16; //tkip_control_fragment_enc, reset value: 0x402f, access type: RW
	} bitFields;
} RegB2MacWepTkipFragmentControl_u;

/*REG_B2_MAC_WEP_SHRAM_MASK_WRITE 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramMaskWrite : 1; //shram_mask_write, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2MacWepShramMaskWrite_u;

/*REG_B2_MAC_WEP_MAX_MPDU_ADDRESS 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maxMpduAddress : 26; //Maximum MPDU address, used in SW mode only., reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB2MacWepMaxMpduAddress_u;

/*REG_B2_MAC_WEP_IV_GENERATE_VALUE 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ivGenerateValue : 32; //iv_generate_value, reset value: 0x12345678, access type: RW
	} bitFields;
} RegB2MacWepIvGenerateValue_u;

/*REG_B2_MAC_WEP_SM_DEBUG_REGISTER 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mutedHdrStateMachine : 4; //muted_hdr_state_machine, reset value: 0x0, access type: RO
		uint32 encStateMachine : 4; //enc_state_machine, reset value: 0x0, access type: RO
		uint32 wepStateMachine : 4; //wep_state_machine, reset value: 0x0, access type: RO
		uint32 ccmpStateMachine : 3; //ccmp_state_machine, reset value: 0x0, access type: RO
		uint32 gcmpStateMachine : 3; //gcmp_state_machine, reset value: 0x0, access type: RO
		uint32 bipCmacState : 2; //BIP-CMAC FSM state, reset value: 0x0, access type: RO
		uint32 bipGmacState : 3; //BIP-GMAC FSM state, reset value: 0x0, access type: RO
		uint32 keySmState : 2; //Read key FSM state, reset value: 0x0, access type: RO
		uint32 reserved0 : 7;
	} bitFields;
} RegB2MacWepSmDebugRegister_u;

/*REG_B2_MAC_WEP_WIRELESS_HEADER_LENGTH 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wirelessHeaderLength : 6; //wireless_header_length, reset value: 0x18, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB2MacWepWirelessHeaderLength_u;

/*REG_B2_MAC_WEP_FRAGMENT_DB_BASE_ADDR 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fragmentDbBaseAddr : 22; //fragment_db_base_addr, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB2MacWepFragmentDbBaseAddr_u;

/*REG_B2_MAC_WEP_TKIP_FRAGMENT2_CONTROL 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tkipMicControlFirstFrag : 16; //tkip_mic_control_first_frag, reset value: 0x5009, access type: RW
		uint32 tkipMicControlLastFrag : 16; //tkip_mic_control_last_frag, reset value: 0x6089, access type: RW
	} bitFields;
} RegB2MacWepTkipFragment2Control_u;

/*REG_B2_MAC_WEP_FRAGMENT_ENTRY_NUM 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fragmentEntryNum : 9; //fragment_entry_num, reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB2MacWepFragmentEntryNum_u;

/*REG_B2_MAC_WEP_TKIP_FRAGMENT3_CONTROL 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tkipMicControlMiddleFrag : 16; //tkip_mic_control_middle_frag, reset value: 0x6009, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2MacWepTkipFragment3Control_u;

/*REG_B2_MAC_WEP_DESC_ARBITER 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 arbiterResultValid : 1; //Valid descriptor arbiter result., reset value: 0x0, access type: RO
		uint32 arbiterResultTxRxN : 1; //When set descriptor arbiter selects Tx descriptor list, otherwise Rx descriptor list is selected., reset value: 0x0, access type: RO
		uint32 arbiterResult : 6; //The user selected by the descriptor arbiter., reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB2MacWepDescArbiter_u;

/*REG_B2_MAC_WEP_TX_INTERRUPTS_EN 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 txInterruptsEn : 8; //Tx Interrupts enable, reset value: 0xff, access type: RW
		uint32 reserved1 : 23;
	} bitFields;
} RegB2MacWepTxInterruptsEn_u;

/*REG_B2_MAC_WEP_TX_INTERRUPTS_STATUS 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDoneIndicationIrq : 1; //tx_done_indication_irq, reset value: 0x0, access type: RO
		uint32 txFailureIndicationIrq : 1; //tx_failure_indication_irq, reset value: 0x0, access type: RO
		uint32 txMicFailureIrq : 1; //tx_mic_failure_irq, reset value: 0x0, access type: RO
		uint32 txNotKeyValidIrq : 1; //tx_not_key_valid_irq, reset value: 0x0, access type: RO
		uint32 txNotProtectedFrameIrq : 1; //tx_not_protected_frame_irq, reset value: 0x0, access type: RO
		uint32 txLengthErrorIrq : 1; //tx_length_error_irq, reset value: 0x0, access type: RO
		uint32 txEivFieldErrorIrq : 1; //tx_eiv_field_error_irq, reset value: 0x0, access type: RO
		uint32 txFragLengthShortErrorIrq : 1; //tx_frag_length_short_error_irq, reset value: 0x0, access type: RO
		uint32 txFragNumNewEqualsPrevIrq : 1; //tx_frag_num_new_equals_prev_irq, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegB2MacWepTxInterruptsStatus_u;

/*REG_B2_MAC_WEP_TX_INTERRUPTS_CLEAR 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txInterruptsClear : 9; //Tx Interrupts clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 23;
	} bitFields;
} RegB2MacWepTxInterruptsClear_u;

/*REG_B2_MAC_WEP_TX_FAIL_STATUS 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srTxFailDescriptorPointer : 16; //Tx fail descriptor pointer., reset value: 0x0, access type: RO
		uint32 srTxFailUserId : 7; //Tx fail user ID., reset value: 0x0, access type: RO
		uint32 reserved0 : 9;
	} bitFields;
} RegB2MacWepTxFailStatus_u;

/*REG_B2_MAC_WEP_RX_INTERRUPTS_EN 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 rxInterruptsEn : 9; //Rx Interrupts enable, reset value: 0x1ff, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegB2MacWepRxInterruptsEn_u;

/*REG_B2_MAC_WEP_RX_INTERRUPTS_STATUS 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxDoneIndicationIrq : 1; //rx_done_indication_irq, reset value: 0x0, access type: RO
		uint32 rxFailureIndicationIrq : 1; //rx_failure_indication_irq, reset value: 0x0, access type: RO
		uint32 rxMicFailureIrq : 1; //rx_mic_failure_irq, reset value: 0x0, access type: RO
		uint32 rxNotKeyValidIrq : 1; //rx_not_key_valid_irq, reset value: 0x0, access type: RO
		uint32 rxNotProtectedFrameIrq : 1; //rx_not_protected_frame_irq, reset value: 0x0, access type: RO
		uint32 rxLengthErrorIrq : 1; //rx_length_error_irq, reset value: 0x0, access type: RO
		uint32 rxEivFieldErrorIrq : 1; //rx_eiv_field_error_irq, reset value: 0x0, access type: RO
		uint32 rxFragLengthShortErrorIrq : 1; //rx_frag_length_short_error_irq, reset value: 0x0, access type: RO
		uint32 rxFragNumNewEqualsPrevIrq : 1; //rx_frag_num_new_equals_prev_irq, reset value: 0x0, access type: RO
		uint32 rxUnexpectedBipIrq : 1; //Rx unexpected BIP-CMAC/GMAC mode received interrupt. Frame is treated as Skip mode in this case., reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB2MacWepRxInterruptsStatus_u;

/*REG_B2_MAC_WEP_RX_INTERRUPTS_CLEAR 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxInterruptsClear : 10; //Rx Interrupts clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 22;
	} bitFields;
} RegB2MacWepRxInterruptsClear_u;

/*REG_B2_MAC_WEP_RX_FAIL_STATUS 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srRxFailDescriptorPointer : 16; //Rx fail descriptor pointer., reset value: 0x0, access type: RO
		uint32 srRxFailUserId : 7; //Rx fail user ID., reset value: 0x0, access type: RO
		uint32 reserved0 : 9;
	} bitFields;
} RegB2MacWepRxFailStatus_u;

/*REG_B2_MAC_WEP_COMPLETE_STATUS 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 srDescriptorPointer : 16; //Complete descriptor pointer., reset value: 0x0, access type: RO
		uint32 srUserId : 7; //Complete user ID., reset value: 0x0, access type: RO
		uint32 reserved0 : 9;
	} bitFields;
} RegB2MacWepCompleteStatus_u;

/*REG_B2_MAC_WEP_CR_MIC_KEY128_31TO0 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crMicKey12831To0 : 32; //GCMP256 upper 128bit key, key[159:128], reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepCrMicKey12831To0_u;

/*REG_B2_MAC_WEP_CR_MIC_KEY128_63TO32 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crMicKey12863To32 : 32; //GCMP256 upper 128bit key, key[191:160], reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepCrMicKey12863To32_u;

/*REG_B2_MAC_WEP_CR_MIC_KEY128_95TO64 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crMicKey12895To64 : 32; //GCMP256 upper 128bit key, key[223:192], reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepCrMicKey12895To64_u;

/*REG_B2_MAC_WEP_CR_MIC_KEY128_127TO96 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crMicKey128127To96 : 32; //GCMP256 upper 128bit key, key[255:224], reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepCrMicKey128127To96_u;

/*REG_B2_MAC_WEP_SEC_LOGGER 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 secLoggerEn : 1; //Security engine logger enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 secLoggerPriority : 2; //Security engine logger priority, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegB2MacWepSecLogger_u;

/*REG_B2_MAC_WEP_SEC_LOGGER_ACTIVE 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 secLoggerActive : 1; //Security engine logger active, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2MacWepSecLoggerActive_u;

/*REG_B2_MAC_WEP_AAD_ADDR 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 aadAddr : 4; //AAD buffer address to be loaded by aad_data in SW mode. , Address resolution is 4 bytes (32bits)., reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2MacWepAadAddr_u;

/*REG_B2_MAC_WEP_AAD_DATA 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 aadData : 32; //AAD buffer data in SW mode., reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepAadData_u;

/*REG_B2_MAC_WEP_DLM_MPDU_LIST_IDX_OUT 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crDlmTxMpduListIdxOut : 8; //DLM Tx push descriptor pointer base list index., reset value: 0x50, access type: RW
		uint32 crDlmRxMpduListIdxOut : 8; //DLM Rx push descriptor pointer base list index., reset value: 0x2c, access type: RW
		uint32 crDlmFreeMpduListIdxOut : 8; //DLM Free push descriptor pointer base list index., reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2MacWepDlmMpduListIdxOut_u;

/*REG_B2_MAC_WEP_SEC_LOGGER_FILTER 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crLogTx : 1; //Filter only encryption logs, reset value: 0x1, access type: RW
		uint32 crLogRx : 1; //Filter only decryption logs, reset value: 0x1, access type: RW
		uint32 crLogError : 1; //Filter only erroneous logs, reset value: 0x1, access type: RW
		uint32 crLogNoError : 1; //Filter only non erroneous logs, reset value: 0x1, access type: RW
		uint32 crLogUserId0 : 8; //Filter only users with user_id equals cr_log_user_id0, reset value: 0x0, access type: RW
		uint32 crLogUserId1 : 8; //Filter only users with user_id equals cr_log_user_id0, reset value: 0x24, access type: RW
		uint32 crLogAllUsers : 1; //Log all users, reset value: 0x1, access type: RW
		uint32 crLogEncMode : 4; //Filter only users with enc_mode equals cr_log_enc_mode, reset value: 0x0, access type: RW
		uint32 crLogAllEncMode : 1; //Log all encryption modes, reset value: 0x1, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB2MacWepSecLoggerFilter_u;

/*REG_B2_MAC_WEP_FIFOS_STATUS 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qOrderFifoBytes : 2; //Valid entries in Q order FIFO., reset value: 0x0, access type: RO
		uint32 qOrderFifoEmpty : 1; //Q order FIFO empty indication., reset value: 0x1, access type: RO
		uint32 qOrderFifoFull : 1; //Q order FIFO full indication., reset value: 0x0, access type: RO
		uint32 rdBlockFifoBytes : 3; //Valid entries in read block FIFO., reset value: 0x0, access type: RO
		uint32 rdBlockFifoEmpty : 1; //Read block FIFO empty indication., reset value: 0x1, access type: RO
		uint32 rdBlockFifoFull : 1; //Read block FIFO full indication., reset value: 0x0, access type: RO
		uint32 wrBlockFifoBytes : 4; //Valid entries in write block FIFO., reset value: 0x0, access type: RO
		uint32 wrBlockFifoEmpty : 1; //Write block FIFO empty indication., reset value: 0x1, access type: RO
		uint32 wrBlockFifoFull : 1; //Write block FIFO full indication., reset value: 0x0, access type: RO
		uint32 wrRxDescriptorStatusFifoBytes : 4; //Valid entries in write rx descriptor status FIFO., reset value: 0x0, access type: RO
		uint32 wrRxDescriptorStatusFifoEmpty : 1; //Write rx descriptor status FIFO empty indication., reset value: 0x1, access type: RO
		uint32 wrRxDescriptorStatusFifoFull : 1; //Write rx descriptor status FIFO full indication, reset value: 0x0, access type: RO
		uint32 secCmpDataArbFifoBytes : 2; //Valid entries in CMP Data arbiter FIFO., reset value: 0x0, access type: RO
		uint32 secCmpDataArbFifoEmpty : 1; //CMP Data arbiter FIFO empty indication., reset value: 0x1, access type: RO
		uint32 secCmpDataArbFifoFull : 1; //CMP Data arbiter FIFO full indication., reset value: 0x0, access type: RO
		uint32 reserved0 : 7;
	} bitFields;
} RegB2MacWepFifosStatus_u;

/*REG_B2_MAC_WEP_MIC_INDICATION_15TO0 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 micValid15To0 : 16; //mic_valid[15:0], reset value: 0x0, access type: RO
		uint32 micIdx15To0 : 16; //mic_idx[15:0], reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacWepMicIndication15To0_u;

/*REG_B2_MAC_WEP_MIC_INDICATION_31TO16 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 micValid31To16 : 16; //mic_valid[31:16], reset value: 0x0, access type: RO
		uint32 micIdx31To16 : 16; //mic_idx[31:16], reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacWepMicIndication31To16_u;

/*REG_B2_MAC_WEP_MIC_INDICATION_35TO32 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 micValid35To32 : 4; //mic_valid[35:32], reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
		uint32 micIdx35To32 : 4; //mic_idx[35:32], reset value: 0x0, access type: RO
		uint32 reserved1 : 12;
	} bitFields;
} RegB2MacWepMicIndication35To32_u;

/*REG_B2_MAC_WEP_USER_ACTIVE_INDICATION_15TO0 0x14C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userActive15To0 : 16; //user_active[15:0], reset value: 0x0, access type: RO
		uint32 userActiveTxRxN15To0 : 16; //user_active_tx_rx_n[15:0], reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacWepUserActiveIndication15To0_u;

/*REG_B2_MAC_WEP_USER_ACTIVE_INDICATION_31TO16 0x150 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userActive31To16 : 16; //user_active[31:16], reset value: 0x0, access type: RO
		uint32 userActiveTxRxN31To16 : 16; //user_active_tx_rx_n[31:16], reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacWepUserActiveIndication31To16_u;

/*REG_B2_MAC_WEP_USER_ACTIVE_INDICATION_47TO32 0x154 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userActive47To32 : 16; //user_active[47:32], reset value: 0x0, access type: RO
		uint32 userActiveTxRxN47To32 : 16; //user_active_tx_rx_n[47:32], reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacWepUserActiveIndication47To32_u;

/*REG_B2_MAC_WEP_USER_ACTIVE_INDICATION_63TO48 0x158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userActive63To48 : 16; //user_active[63:48], reset value: 0x0, access type: RO
		uint32 userActiveTxRxN63To48 : 16; //user_active_tx_rx_n[63:48], reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacWepUserActiveIndication63To48_u;

/*REG_B2_MAC_WEP_USER_ACTIVE_INDICATION_71TO64 0x15C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userActive71To64 : 8; //user_active[71:64], reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
		uint32 userActiveTxRxN71To64 : 8; //user_active_tx_rx_n[71:64], reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB2MacWepUserActiveIndication71To64_u;

/*REG_B2_MAC_WEP_TX_IDLE_INDICATION 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txUserCnt : 7; //Tx active user counter., reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 txSecurityPipeIdle : 1; //When set there are no pending Tx jobs in pipe., reset value: 0x1, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegB2MacWepTxIdleIndication_u;

/*REG_B2_MAC_WEP_TX_DLM_FC_CONTROL 0x164 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crEnableTxDlm : 1; //Enable Tx DLM., reset value: 0x0, access type: RW
		uint32 crTxBypassRdFc : 1; //Tx bypass (ignore) read flow control., reset value: 0x0, access type: RW
		uint32 crTxDisableUpdateWrFc : 1; //Tx disable update write flow control., reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2MacWepTxDlmFcControl_u;

/*REG_B2_MAC_WEP_RX_DLM_FC_CONTROL 0x168 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crEnableRxDlm : 1; //Enable Rx DLM., reset value: 0x0, access type: RW
		uint32 crRxBypassRdFc : 1; //Rx bypass (ignore) read flow control., reset value: 0x0, access type: RW
		uint32 crRxDisableUpdateWrFc : 1; //Rx disable update write flow control., reset value: 0x0, access type: RW
		uint32 crBlockRxIfTxDescNotEmpty : 1; //When set Rx descriptors are blocked if there is Tx descriptor which is not empty. , Still if unset Tx gets priority over Rx, but then if there are 2 outstanding Tx jobs for each of the nonempty Tx users (up to two pending jobs per user may enter the pipe), Rx descriptors may get service., reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2MacWepRxDlmFcControl_u;

/*REG_B2_MAC_WEP_WR_FC_STROBES 0x16C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crClrRoofMinAddrPulse : 1; //Clear roof_min_addr flop for all users. Should be cleared on any new transmission by SW., reset value: 0x0, access type: WO
		uint32 crTxClearWrFcPulse : 1; //Tx clear write flow control., reset value: 0x0, access type: WO
		uint32 crRxClearWrFcPulse : 1; //Rx clear write flow control., reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2MacWepWrFcStrobes_u;

/*REG_B2_MAC_WEP_CR_MAX_TKIP_FRAG_LENGTH 0x170 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crMaxTkipFragLength : 16; //Max TKIP fragment frame length. When exceeded an error of type length_error will be indicated., reset value: 0x800, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2MacWepCrMaxTkipFragLength_u;

/*REG_B2_MAC_WEP_CR_USER_BYPASS_HC_RD_FC_31TO0 0x174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crUserBypassHcRdFc31To0 : 32; //Per user bypass (ignore) header conversion read flow control., reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepCrUserBypassHcRdFc31To0_u;

/*REG_B2_MAC_WEP_CR_USER_BYPASS_HC_RD_FC_35TO32 0x178 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crUserBypassHcRdFc35To32 : 4; //Per user bypass (ignore) header conversion read flow control., reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2MacWepCrUserBypassHcRdFc35To32_u;

/*REG_B2_MAC_WEP_BIP_FC_MASK 0x17C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crBipFcMask : 16; //BIP FC mask, reset value: 0xc7ff, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2MacWepBipFcMask_u;

/*REG_B2_MAC_WEP_SPARE_REG 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareReg : 32; //Spare register, reset value: 0x88888888, access type: RW
	} bitFields;
} RegB2MacWepSpareReg_u;

/*REG_B2_MAC_WEP_TX_MLDAR_CTRL 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMldarEna : 1; //Enables address replacement (default). Disable for read-out RA_TA table., reset value: 0x1, access type: RW
		uint32 txMldarOverridePrio : 1; //'1' - use "set_prio_value" config bit instead of HW logic., reset value: 0x0, access type: RW
		uint32 txMldarSetPrioValue : 1; //Config bit value to override the priority bit from logic., reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2MacWepTxMldarCtrl_u;

/*REG_B2_MAC_WEP_TX_MLDAR_RA_TA_READ_ADDR 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMldarRaTaReadAddr : 5; //RA_TA table read-out pulse and address, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB2MacWepTxMldarRaTaReadAddr_u;

/*REG_B2_MAC_WEP_TX_MLDAR_RA_TA_RD_DATA0 0x18C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMldarRaTaRdData0 : 32; //read-out data, LS 32 bits of {RA[47:0],TA{47:0}., reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacWepTxMldarRaTaRdData0_u;

/*REG_B2_MAC_WEP_TX_MLDAR_RA_TA_RD_DATA1 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMldarRaTaRdData1 : 32; //read-out data, bits[63:32] of {RA[47:0],TA{47:0}., reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacWepTxMldarRaTaRdData1_u;

/*REG_B2_MAC_WEP_TX_MLDAR_RA_TA_RD_DATA2 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMldarRaTaRdData2 : 32; //read-out data, MS 32 bits of {RA[47:0],TA{47:0}., reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacWepTxMldarRaTaRdData2_u;

/*REG_B2_MAC_WEP_TX_MLDAR_DBG_BUSY_VEC 0x198 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMldarDbgBusyVec : 32; //each bit will have '1' if the corresponding user's busy_cnt isn't zero., reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacWepTxMldarDbgBusyVec_u;

/*REG_B2_MAC_WEP_TX_MLDAR_DBG_SMC_ADDR 0x19C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMldarDbgSmcAddr : 22; //A1 CB pointer of the currently processed user., reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
	} bitFields;
} RegB2MacWepTxMldarDbgSmcAddr_u;

/*REG_B2_MAC_WEP_TX_MLDAR_DBG 0x1A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMldarDbgIsMld : 1; //current user is_mld flag, reset value: 0x0, access type: RO
		uint32 txMldarDbgMaxFifoUsedw : 4; //maximal FIFO utilisation metric since the last reset, reset value: 0x0, access type: RO
		uint32 txMldarDbgCurrUserId : 5; //currently processed user_id, fifo output, reset value: 0x0, access type: RO
		uint32 txMldarDbgSecUserId : 5; //last triggered user_id from Security, reset value: 0x0, access type: RO
		uint32 txMldarDbgSmState : 2; //current SM state, reset value: 0x0, access type: RO
		uint32 txMldarError : 1; //One of the following sets Error flag: , FIFO overflow , FIFO underflow , Busy_Cnt of some user is >3, reset value: 0x0, access type: RO
		uint32 txMldarPriority : 1; //'1' - means Tx Address Replacement block has priority on concurrent access to the Circular Buffer, arbitered with Security Engine., reset value: 0x0, access type: RO
		uint32 txMldarDbgIsClient : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB2MacWepTxMldarDbg_u;

/*REG_B2_MAC_WEP_TX_MLDAR_SKIP_USER 0x1A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMldarSkipUser : 32; //bit[i] = '1' means "skip address replacement for user[i]" , reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacWepTxMldarSkipUser_u;

//========================================
/* REG_MAC_WEP_CONTROL 0x0 */
#define B2_MAC_WEP_CONTROL_FIELD_START_MASK                                                                       0x00000001
#define B2_MAC_WEP_CONTROL_FIELD_CONTROL_REG_MASK                                                                 0x0000FFFE
//========================================
/* REG_MAC_WEP_STATUS 0x4 */
#define B2_MAC_WEP_STATUS_FIELD_ACTIVE_MASK                                                                       0x00000001
#define B2_MAC_WEP_STATUS_FIELD_CRCFAIL_MASK                                                                      0x00000002
#define B2_MAC_WEP_STATUS_FIELD_MICFAIL_MASK                                                                      0x00000004
#define B2_MAC_WEP_STATUS_FIELD_NOT_KEY_VALID_MASK                                                                0x00000008
#define B2_MAC_WEP_STATUS_FIELD_NOT_PROTECTED_FRAME_MASK                                                          0x00000010
#define B2_MAC_WEP_STATUS_FIELD_LENGTH_ERROR_MASK                                                                 0x00000020
#define B2_MAC_WEP_STATUS_FIELD_EIV_FIELD_ERROR_MASK                                                              0x00000040
#define B2_MAC_WEP_STATUS_FIELD_FRAG_LENGTH_SHORT_ERROR_MASK                                                      0x00000080
#define B2_MAC_WEP_STATUS_FIELD_FRAG_NUM_NEW_EQUALS_PREV_ERROR_MASK                                               0x00000100
//========================================
/* REG_MAC_WEP_SRC_ADDRESS 0x8 */
#define B2_MAC_WEP_SRC_ADDRESS_FIELD_SRC_ADDRESS_MASK                                                             0x03FFFFFF
#define B2_MAC_WEP_SRC_ADDRESS_FIELD_ENC_MODE_CONFIG_MASK                                                         0x3C000000
#define B2_MAC_WEP_SRC_ADDRESS_FIELD_SPP_A_MSDU_CAPABLE_MASK                                                      0x40000000
#define B2_MAC_WEP_SRC_ADDRESS_FIELD_ENCRYPT_MODE_MASK                                                            0x80000000
//========================================
/* REG_MAC_WEP_DST_ADDRESS 0xC */
#define B2_MAC_WEP_DST_ADDRESS_FIELD_DST_ADDRESS_MASK                                                             0x03FFFFFF
#define B2_MAC_WEP_DST_ADDRESS_FIELD_KEY_ID_MASK                                                                  0x30000000
//========================================
/* REG_MAC_WEP_CUR_ADDRESS_OFFSET 0x10 */
#define B2_MAC_WEP_CUR_ADDRESS_OFFSET_FIELD_CUR_ADDRESS_OFFSET_MASK                                               0x0000FFFF
//========================================
/* REG_MAC_WEP_LENGTH 0x14 */
#define B2_MAC_WEP_LENGTH_FIELD_LENGTH_MASK                                                                       0x0000FFFF
//========================================
/* REG_MAC_WEP_KEY_LENGTH 0x18 */
#define B2_MAC_WEP_KEY_LENGTH_FIELD_KEY_LENGTH_MASK                                                               0x0000003F
//========================================
/* REG_MAC_WEP_KEY_31TO0 0x1C */
#define B2_MAC_WEP_KEY_31TO0_FIELD_KEY_31TO0_MASK                                                                 0xFFFFFFFF
//========================================
/* REG_MAC_WEP_KEY_63TO32 0x20 */
#define B2_MAC_WEP_KEY_63TO32_FIELD_KEY_63TO32_MASK                                                               0xFFFFFFFF
//========================================
/* REG_MAC_WEP_KEY_95TO64 0x24 */
#define B2_MAC_WEP_KEY_95TO64_FIELD_KEY_95TO64_MASK                                                               0xFFFFFFFF
//========================================
/* REG_MAC_WEP_KEY_127TO96 0x28 */
#define B2_MAC_WEP_KEY_127TO96_FIELD_KEY_127TO96_MASK                                                             0xFFFFFFFF
//========================================
/* REG_MAC_WEP_CRC_RESULT 0x2C */
#define B2_MAC_WEP_CRC_RESULT_FIELD_CRC_RESULT_MASK                                                               0xFFFFFFFF
//========================================
/* REG_MAC_WEP_TX_ABORT 0x30 */
#define B2_MAC_WEP_TX_ABORT_FIELD_TX_ABORT_PULSE_MASK                                                             0x00000001
#define B2_MAC_WEP_TX_ABORT_FIELD_TX_FLUSH_PULSE_MASK                                                             0x00000002
//========================================
/* REG_MAC_WEP_TX_ABORT_STATUS 0x34 */
#define B2_MAC_WEP_TX_ABORT_STATUS_FIELD_TX_ABORT_DONE_MASK                                                       0x00000001
#define B2_MAC_WEP_TX_ABORT_STATUS_FIELD_TX_FLUSH_DONE_MASK                                                       0x00000002
//========================================
/* REG_MAC_WEP_WRITE10 0x38 */
#define B2_MAC_WEP_WRITE10_FIELD_WRITE10_MASK                                                                     0xFFFFFFFF
//========================================
/* REG_MAC_WEP_WRITE11 0x3C */
#define B2_MAC_WEP_WRITE11_FIELD_WRITE11_MASK                                                                     0xFFFFFFFF
//========================================
/* REG_MAC_WEP_MODE 0x40 */
#define B2_MAC_WEP_MODE_FIELD_MODE_MASK                                                                           0x0000000F
//========================================
/* REG_MAC_WEP_WRITE12 0x44 */
#define B2_MAC_WEP_WRITE12_FIELD_WRITE12_MASK                                                                     0xFFFFFFFF
//========================================
/* REG_MAC_WEP_WRITE13 0x48 */
#define B2_MAC_WEP_WRITE13_FIELD_WRITE13_MASK                                                                     0xFFFFFFFF
//========================================
/* REG_MAC_WEP_WRITE14 0x4C */
#define B2_MAC_WEP_WRITE14_FIELD_WRITE14_MASK                                                                     0xFFFFFFFF
//========================================
/* REG_MAC_WEP_WRITE15 0x50 */
#define B2_MAC_WEP_WRITE15_FIELD_WRITE15_MASK                                                                     0xFFFFFFFF
//========================================
/* REG_MAC_WEP_WRITE16 0x54 */
#define B2_MAC_WEP_WRITE16_FIELD_WRITE16_MASK                                                                     0xFFFFFFFF
//========================================
/* REG_MAC_WEP_READ5 0x58 */
#define B2_MAC_WEP_READ5_FIELD_READ5_MASK                                                                         0xFFFFFFFF
//========================================
/* REG_MAC_WEP_READ6 0x5C */
#define B2_MAC_WEP_READ6_FIELD_READ6_MASK                                                                         0xFFFFFFFF
//========================================
/* REG_MAC_WEP_READ7 0x60 */
#define B2_MAC_WEP_READ7_FIELD_READ7_MASK                                                                         0xFFFFFFFF
//========================================
/* REG_MAC_WEP_HARDWARE_ID_W0 0x64 */
#define B2_MAC_WEP_HARDWARE_ID_W0_FIELD_HARDWARE_ID_AES_MASK                                                      0x00000001
#define B2_MAC_WEP_HARDWARE_ID_W0_FIELD_HARDWARE_ID_OCB_MASK                                                      0x00000002
#define B2_MAC_WEP_HARDWARE_ID_W0_FIELD_HARDWARE_ID_CCM_MASK                                                      0x00000004
#define B2_MAC_WEP_HARDWARE_ID_W0_FIELD_HARDWARE_ID_WAPI_MASK                                                     0x00000008
//========================================
/* REG_MAC_WEP_CCM_FC_MASK 0x68 */
#define B2_MAC_WEP_CCM_FC_MASK_FIELD_CCM_FC_MASK_MASK                                                             0x0000FFFF
#define B2_MAC_WEP_CCM_FC_MASK_FIELD_CCM_FC_MANAGEMENT_MASK_MASK                                                  0xFFFF0000
//========================================
/* REG_MAC_WEP_CCM_SC_MASK 0x6C */
#define B2_MAC_WEP_CCM_SC_MASK_FIELD_CCM_SC_MASK_MASK                                                             0x0000FFFF
//========================================
/* REG_MAC_WEP_CCM_QC_MASK 0x70 */
#define B2_MAC_WEP_CCM_QC_MASK_FIELD_CCM_QC_MASK_MASK                                                             0x0000FFFF
#define B2_MAC_WEP_CCM_QC_MASK_FIELD_MANAGEMENT_NONCE_MASK_MASK                                                   0x00010000
//========================================
/* REG_MAC_WEP_CR_USER_ID 0x74 */
#define B2_MAC_WEP_CR_USER_ID_FIELD_CR_USER_ID_MASK                                                               0x000000FF
//========================================
/* REG_MAC_WEP_FULL_HW_ENC 0x78 */
#define B2_MAC_WEP_FULL_HW_ENC_FIELD_FULL_HW_ENC_MASK                                                             0x00000001
#define B2_MAC_WEP_FULL_HW_ENC_FIELD_RC4_PRE_INIT_HW_EN_MASK                                                      0x00000002
#define B2_MAC_WEP_FULL_HW_ENC_FIELD_DESCRIPTOR_LIST_EN_MASK                                                      0x00000004
#define B2_MAC_WEP_FULL_HW_ENC_FIELD_TX_DESC_MODE_GEN_ERR_IRQ_EN_MASK                                             0x00000008
#define B2_MAC_WEP_FULL_HW_ENC_FIELD_RX_DESC_MODE_GEN_ERR_IRQ_EN_MASK                                             0x00000010
#define B2_MAC_WEP_FULL_HW_ENC_FIELD_FAST_GCMP_MODE_MASK                                                          0x00000020
//========================================
/* REG_MAC_WEP_START_ENTRIES_ADDRESS 0x7C */
#define B2_MAC_WEP_START_ENTRIES_ADDRESS_FIELD_START_ENTRIES_ADDRESS_MASK                                         0x003FFFFF
#define B2_MAC_WEP_START_ENTRIES_ADDRESS_FIELD_DB_ENTRY_SIZE_MASK                                                 0x0F000000
#define B2_MAC_WEP_START_ENTRIES_ADDRESS_FIELD_DIFFERENT_DEST_MASK                                                0x10000000
#define B2_MAC_WEP_START_ENTRIES_ADDRESS_FIELD_PREPARE_IV_MASK                                                    0x20000000
//========================================
/* REG_MAC_WEP_ENTRY_NUMBER 0x80 */
#define B2_MAC_WEP_ENTRY_NUMBER_FIELD_ENTRY_NUMBER_MASK                                                           0x00000FFF
#define B2_MAC_WEP_ENTRY_NUMBER_FIELD_MIC_ENTRY_NUMBER_MASK                                                       0x0FFF0000
//========================================
/* REG_MAC_WEP_LENGTH_OFFSET 0x84 */
#define B2_MAC_WEP_LENGTH_OFFSET_FIELD_LENGTH_OFFSET_ENCRYPT_MASK                                                 0x00000007
#define B2_MAC_WEP_LENGTH_OFFSET_FIELD_LENGTH_OFFSET_DECRYPT_MASK                                                 0x00000070
//========================================
/* REG_MAC_WEP_TKIP_CONTROL 0x88 */
#define B2_MAC_WEP_TKIP_CONTROL_FIELD_TKIP_CONTROL_DEC_MASK                                                       0x0000FFFF
#define B2_MAC_WEP_TKIP_CONTROL_FIELD_TKIP_CONTROL_ENC_MASK                                                       0xFFFF0000
//========================================
/* REG_MAC_WEP_TKIP_MIC_CONTROL 0x8C */
#define B2_MAC_WEP_TKIP_MIC_CONTROL_FIELD_TKIP_MIC_CONTROL_HEADER_MASK                                            0x0000FFFF
//========================================
/* REG_MAC_WEP_WEP_CONTROL 0x90 */
#define B2_MAC_WEP_WEP_CONTROL_FIELD_WEP_CONTROL_DEC_MASK                                                         0x0000FFFF
#define B2_MAC_WEP_WEP_CONTROL_FIELD_WEP_CONTROL_ENC_MASK                                                         0xFFFF0000
//========================================
/* REG_MAC_WEP_TKIP_FRAGMENT_CONTROL 0x94 */
#define B2_MAC_WEP_TKIP_FRAGMENT_CONTROL_FIELD_TKIP_CONTROL_FRAGMENT_DEC_MASK                                     0x0000FFFF
#define B2_MAC_WEP_TKIP_FRAGMENT_CONTROL_FIELD_TKIP_CONTROL_FRAGMENT_ENC_MASK                                     0xFFFF0000
//========================================
/* REG_MAC_WEP_SHRAM_MASK_WRITE 0x98 */
#define B2_MAC_WEP_SHRAM_MASK_WRITE_FIELD_SHRAM_MASK_WRITE_MASK                                                   0x00000001
//========================================
/* REG_MAC_WEP_MAX_MPDU_ADDRESS 0x9C */
#define B2_MAC_WEP_MAX_MPDU_ADDRESS_FIELD_MAX_MPDU_ADDRESS_MASK                                                   0x03FFFFFF
//========================================
/* REG_MAC_WEP_IV_GENERATE_VALUE 0xA0 */
#define B2_MAC_WEP_IV_GENERATE_VALUE_FIELD_IV_GENERATE_VALUE_MASK                                                 0xFFFFFFFF
//========================================
/* REG_MAC_WEP_SM_DEBUG_REGISTER 0xA4 */
#define B2_MAC_WEP_SM_DEBUG_REGISTER_FIELD_MUTED_HDR_STATE_MACHINE_MASK                                           0x0000000F
#define B2_MAC_WEP_SM_DEBUG_REGISTER_FIELD_ENC_STATE_MACHINE_MASK                                                 0x000000F0
#define B2_MAC_WEP_SM_DEBUG_REGISTER_FIELD_WEP_STATE_MACHINE_MASK                                                 0x00000F00
#define B2_MAC_WEP_SM_DEBUG_REGISTER_FIELD_CCMP_STATE_MACHINE_MASK                                                0x00007000
#define B2_MAC_WEP_SM_DEBUG_REGISTER_FIELD_GCMP_STATE_MACHINE_MASK                                                0x00038000
#define B2_MAC_WEP_SM_DEBUG_REGISTER_FIELD_BIP_CMAC_STATE_MASK                                                    0x000C0000
#define B2_MAC_WEP_SM_DEBUG_REGISTER_FIELD_BIP_GMAC_STATE_MASK                                                    0x00700000
#define B2_MAC_WEP_SM_DEBUG_REGISTER_FIELD_KEY_SM_STATE_MASK                                                      0x01800000
//========================================
/* REG_MAC_WEP_WIRELESS_HEADER_LENGTH 0xA8 */
#define B2_MAC_WEP_WIRELESS_HEADER_LENGTH_FIELD_WIRELESS_HEADER_LENGTH_MASK                                       0x0000003F
//========================================
/* REG_MAC_WEP_FRAGMENT_DB_BASE_ADDR 0xDC */
#define B2_MAC_WEP_FRAGMENT_DB_BASE_ADDR_FIELD_FRAGMENT_DB_BASE_ADDR_MASK                                         0x003FFFFF
//========================================
/* REG_MAC_WEP_TKIP_FRAGMENT2_CONTROL 0xE0 */
#define B2_MAC_WEP_TKIP_FRAGMENT2_CONTROL_FIELD_TKIP_MIC_CONTROL_FIRST_FRAG_MASK                                  0x0000FFFF
#define B2_MAC_WEP_TKIP_FRAGMENT2_CONTROL_FIELD_TKIP_MIC_CONTROL_LAST_FRAG_MASK                                   0xFFFF0000
//========================================
/* REG_MAC_WEP_FRAGMENT_ENTRY_NUM 0xE4 */
#define B2_MAC_WEP_FRAGMENT_ENTRY_NUM_FIELD_FRAGMENT_ENTRY_NUM_MASK                                               0x000001FF
//========================================
/* REG_MAC_WEP_TKIP_FRAGMENT3_CONTROL 0xE8 */
#define B2_MAC_WEP_TKIP_FRAGMENT3_CONTROL_FIELD_TKIP_MIC_CONTROL_MIDDLE_FRAG_MASK                                 0x0000FFFF
//========================================
/* REG_MAC_WEP_DESC_ARBITER 0xEC */
#define B2_MAC_WEP_DESC_ARBITER_FIELD_ARBITER_RESULT_VALID_MASK                                                   0x00000001
#define B2_MAC_WEP_DESC_ARBITER_FIELD_ARBITER_RESULT_TX_RX_N_MASK                                                 0x00000002
#define B2_MAC_WEP_DESC_ARBITER_FIELD_ARBITER_RESULT_MASK                                                         0x000000FC
//========================================
/* REG_MAC_WEP_TX_INTERRUPTS_EN 0xF0 */
#define B2_MAC_WEP_TX_INTERRUPTS_EN_FIELD_TX_INTERRUPTS_EN_MASK                                                   0x000001FE
//========================================
/* REG_MAC_WEP_TX_INTERRUPTS_STATUS 0xF4 */
#define B2_MAC_WEP_TX_INTERRUPTS_STATUS_FIELD_TX_DONE_INDICATION_IRQ_MASK                                         0x00000001
#define B2_MAC_WEP_TX_INTERRUPTS_STATUS_FIELD_TX_FAILURE_INDICATION_IRQ_MASK                                      0x00000002
#define B2_MAC_WEP_TX_INTERRUPTS_STATUS_FIELD_TX_MIC_FAILURE_IRQ_MASK                                             0x00000004
#define B2_MAC_WEP_TX_INTERRUPTS_STATUS_FIELD_TX_NOT_KEY_VALID_IRQ_MASK                                           0x00000008
#define B2_MAC_WEP_TX_INTERRUPTS_STATUS_FIELD_TX_NOT_PROTECTED_FRAME_IRQ_MASK                                     0x00000010
#define B2_MAC_WEP_TX_INTERRUPTS_STATUS_FIELD_TX_LENGTH_ERROR_IRQ_MASK                                            0x00000020
#define B2_MAC_WEP_TX_INTERRUPTS_STATUS_FIELD_TX_EIV_FIELD_ERROR_IRQ_MASK                                         0x00000040
#define B2_MAC_WEP_TX_INTERRUPTS_STATUS_FIELD_TX_FRAG_LENGTH_SHORT_ERROR_IRQ_MASK                                 0x00000080
#define B2_MAC_WEP_TX_INTERRUPTS_STATUS_FIELD_TX_FRAG_NUM_NEW_EQUALS_PREV_IRQ_MASK                                0x00000100
//========================================
/* REG_MAC_WEP_TX_INTERRUPTS_CLEAR 0xF8 */
#define B2_MAC_WEP_TX_INTERRUPTS_CLEAR_FIELD_TX_INTERRUPTS_CLEAR_MASK                                             0x000001FF
//========================================
/* REG_MAC_WEP_TX_FAIL_STATUS 0xFC */
#define B2_MAC_WEP_TX_FAIL_STATUS_FIELD_SR_TX_FAIL_DESCRIPTOR_POINTER_MASK                                        0x0000FFFF
#define B2_MAC_WEP_TX_FAIL_STATUS_FIELD_SR_TX_FAIL_USER_ID_MASK                                                   0x007F0000
//========================================
/* REG_MAC_WEP_RX_INTERRUPTS_EN 0x100 */
#define B2_MAC_WEP_RX_INTERRUPTS_EN_FIELD_RX_INTERRUPTS_EN_MASK                                                   0x000003FE
//========================================
/* REG_MAC_WEP_RX_INTERRUPTS_STATUS 0x104 */
#define B2_MAC_WEP_RX_INTERRUPTS_STATUS_FIELD_RX_DONE_INDICATION_IRQ_MASK                                         0x00000001
#define B2_MAC_WEP_RX_INTERRUPTS_STATUS_FIELD_RX_FAILURE_INDICATION_IRQ_MASK                                      0x00000002
#define B2_MAC_WEP_RX_INTERRUPTS_STATUS_FIELD_RX_MIC_FAILURE_IRQ_MASK                                             0x00000004
#define B2_MAC_WEP_RX_INTERRUPTS_STATUS_FIELD_RX_NOT_KEY_VALID_IRQ_MASK                                           0x00000008
#define B2_MAC_WEP_RX_INTERRUPTS_STATUS_FIELD_RX_NOT_PROTECTED_FRAME_IRQ_MASK                                     0x00000010
#define B2_MAC_WEP_RX_INTERRUPTS_STATUS_FIELD_RX_LENGTH_ERROR_IRQ_MASK                                            0x00000020
#define B2_MAC_WEP_RX_INTERRUPTS_STATUS_FIELD_RX_EIV_FIELD_ERROR_IRQ_MASK                                         0x00000040
#define B2_MAC_WEP_RX_INTERRUPTS_STATUS_FIELD_RX_FRAG_LENGTH_SHORT_ERROR_IRQ_MASK                                 0x00000080
#define B2_MAC_WEP_RX_INTERRUPTS_STATUS_FIELD_RX_FRAG_NUM_NEW_EQUALS_PREV_IRQ_MASK                                0x00000100
#define B2_MAC_WEP_RX_INTERRUPTS_STATUS_FIELD_RX_UNEXPECTED_BIP_IRQ_MASK                                          0x00000200
//========================================
/* REG_MAC_WEP_RX_INTERRUPTS_CLEAR 0x108 */
#define B2_MAC_WEP_RX_INTERRUPTS_CLEAR_FIELD_RX_INTERRUPTS_CLEAR_MASK                                             0x000003FF
//========================================
/* REG_MAC_WEP_RX_FAIL_STATUS 0x10C */
#define B2_MAC_WEP_RX_FAIL_STATUS_FIELD_SR_RX_FAIL_DESCRIPTOR_POINTER_MASK                                        0x0000FFFF
#define B2_MAC_WEP_RX_FAIL_STATUS_FIELD_SR_RX_FAIL_USER_ID_MASK                                                   0x007F0000
//========================================
/* REG_MAC_WEP_COMPLETE_STATUS 0x110 */
#define B2_MAC_WEP_COMPLETE_STATUS_FIELD_SR_DESCRIPTOR_POINTER_MASK                                               0x0000FFFF
#define B2_MAC_WEP_COMPLETE_STATUS_FIELD_SR_USER_ID_MASK                                                          0x007F0000
//========================================
/* REG_MAC_WEP_CR_MIC_KEY128_31TO0 0x114 */
#define B2_MAC_WEP_CR_MIC_KEY128_31TO0_FIELD_CR_MIC_KEY128_31TO0_MASK                                             0xFFFFFFFF
//========================================
/* REG_MAC_WEP_CR_MIC_KEY128_63TO32 0x118 */
#define B2_MAC_WEP_CR_MIC_KEY128_63TO32_FIELD_CR_MIC_KEY128_63TO32_MASK                                           0xFFFFFFFF
//========================================
/* REG_MAC_WEP_CR_MIC_KEY128_95TO64 0x11C */
#define B2_MAC_WEP_CR_MIC_KEY128_95TO64_FIELD_CR_MIC_KEY128_95TO64_MASK                                           0xFFFFFFFF
//========================================
/* REG_MAC_WEP_CR_MIC_KEY128_127TO96 0x120 */
#define B2_MAC_WEP_CR_MIC_KEY128_127TO96_FIELD_CR_MIC_KEY128_127TO96_MASK                                         0xFFFFFFFF
//========================================
/* REG_MAC_WEP_SEC_LOGGER 0x124 */
#define B2_MAC_WEP_SEC_LOGGER_FIELD_SEC_LOGGER_EN_MASK                                                            0x00000001
#define B2_MAC_WEP_SEC_LOGGER_FIELD_SEC_LOGGER_PRIORITY_MASK                                                      0x00000300
//========================================
/* REG_MAC_WEP_SEC_LOGGER_ACTIVE 0x128 */
#define B2_MAC_WEP_SEC_LOGGER_ACTIVE_FIELD_SEC_LOGGER_ACTIVE_MASK                                                 0x00000001
//========================================
/* REG_MAC_WEP_AAD_ADDR 0x12C */
#define B2_MAC_WEP_AAD_ADDR_FIELD_AAD_ADDR_MASK                                                                   0x0000000F
//========================================
/* REG_MAC_WEP_AAD_DATA 0x130 */
#define B2_MAC_WEP_AAD_DATA_FIELD_AAD_DATA_MASK                                                                   0xFFFFFFFF
//========================================
/* REG_MAC_WEP_DLM_MPDU_LIST_IDX_OUT 0x134 */
#define B2_MAC_WEP_DLM_MPDU_LIST_IDX_OUT_FIELD_CR_DLM_TX_MPDU_LIST_IDX_OUT_MASK                                   0x000000FF
#define B2_MAC_WEP_DLM_MPDU_LIST_IDX_OUT_FIELD_CR_DLM_RX_MPDU_LIST_IDX_OUT_MASK                                   0x0000FF00
#define B2_MAC_WEP_DLM_MPDU_LIST_IDX_OUT_FIELD_CR_DLM_FREE_MPDU_LIST_IDX_OUT_MASK                                 0x00FF0000
//========================================
/* REG_MAC_WEP_SEC_LOGGER_FILTER 0x138 */
#define B2_MAC_WEP_SEC_LOGGER_FILTER_FIELD_CR_LOG_TX_MASK                                                         0x00000001
#define B2_MAC_WEP_SEC_LOGGER_FILTER_FIELD_CR_LOG_RX_MASK                                                         0x00000002
#define B2_MAC_WEP_SEC_LOGGER_FILTER_FIELD_CR_LOG_ERROR_MASK                                                      0x00000004
#define B2_MAC_WEP_SEC_LOGGER_FILTER_FIELD_CR_LOG_NO_ERROR_MASK                                                   0x00000008
#define B2_MAC_WEP_SEC_LOGGER_FILTER_FIELD_CR_LOG_USER_ID0_MASK                                                   0x00000FF0
#define B2_MAC_WEP_SEC_LOGGER_FILTER_FIELD_CR_LOG_USER_ID1_MASK                                                   0x000FF000
#define B2_MAC_WEP_SEC_LOGGER_FILTER_FIELD_CR_LOG_ALL_USERS_MASK                                                  0x00100000
#define B2_MAC_WEP_SEC_LOGGER_FILTER_FIELD_CR_LOG_ENC_MODE_MASK                                                   0x01E00000
#define B2_MAC_WEP_SEC_LOGGER_FILTER_FIELD_CR_LOG_ALL_ENC_MODE_MASK                                               0x02000000
//========================================
/* REG_MAC_WEP_FIFOS_STATUS 0x13C */
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_Q_ORDER_FIFO_BYTES_MASK                                                     0x00000003
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_Q_ORDER_FIFO_EMPTY_MASK                                                     0x00000004
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_Q_ORDER_FIFO_FULL_MASK                                                      0x00000008
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_RD_BLOCK_FIFO_BYTES_MASK                                                    0x00000070
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_RD_BLOCK_FIFO_EMPTY_MASK                                                    0x00000080
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_RD_BLOCK_FIFO_FULL_MASK                                                     0x00000100
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_WR_BLOCK_FIFO_BYTES_MASK                                                    0x00001E00
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_WR_BLOCK_FIFO_EMPTY_MASK                                                    0x00002000
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_WR_BLOCK_FIFO_FULL_MASK                                                     0x00004000
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_WR_RX_DESCRIPTOR_STATUS_FIFO_BYTES_MASK                                     0x00078000
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_WR_RX_DESCRIPTOR_STATUS_FIFO_EMPTY_MASK                                     0x00080000
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_WR_RX_DESCRIPTOR_STATUS_FIFO_FULL_MASK                                      0x00100000
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_SEC_CMP_DATA_ARB_FIFO_BYTES_MASK                                            0x00600000
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_SEC_CMP_DATA_ARB_FIFO_EMPTY_MASK                                            0x00800000
#define B2_MAC_WEP_FIFOS_STATUS_FIELD_SEC_CMP_DATA_ARB_FIFO_FULL_MASK                                             0x01000000
//========================================
/* REG_MAC_WEP_MIC_INDICATION_15TO0 0x140 */
#define B2_MAC_WEP_MIC_INDICATION_15TO0_FIELD_MIC_VALID_15TO0_MASK                                                0x0000FFFF
#define B2_MAC_WEP_MIC_INDICATION_15TO0_FIELD_MIC_IDX_15TO0_MASK                                                  0xFFFF0000
//========================================
/* REG_MAC_WEP_MIC_INDICATION_31TO16 0x144 */
#define B2_MAC_WEP_MIC_INDICATION_31TO16_FIELD_MIC_VALID_31TO16_MASK                                              0x0000FFFF
#define B2_MAC_WEP_MIC_INDICATION_31TO16_FIELD_MIC_IDX_31TO16_MASK                                                0xFFFF0000
//========================================
/* REG_MAC_WEP_MIC_INDICATION_35TO32 0x148 */
#define B2_MAC_WEP_MIC_INDICATION_35TO32_FIELD_MIC_VALID_35TO32_MASK                                              0x0000000F
#define B2_MAC_WEP_MIC_INDICATION_35TO32_FIELD_MIC_IDX_35TO32_MASK                                                0x000F0000
//========================================
/* REG_MAC_WEP_USER_ACTIVE_INDICATION_15TO0 0x14C */
#define B2_MAC_WEP_USER_ACTIVE_INDICATION_15TO0_FIELD_USER_ACTIVE_15TO0_MASK                                      0x0000FFFF
#define B2_MAC_WEP_USER_ACTIVE_INDICATION_15TO0_FIELD_USER_ACTIVE_TX_RX_N_15TO0_MASK                              0xFFFF0000
//========================================
/* REG_MAC_WEP_USER_ACTIVE_INDICATION_31TO16 0x150 */
#define B2_MAC_WEP_USER_ACTIVE_INDICATION_31TO16_FIELD_USER_ACTIVE_31TO16_MASK                                    0x0000FFFF
#define B2_MAC_WEP_USER_ACTIVE_INDICATION_31TO16_FIELD_USER_ACTIVE_TX_RX_N_31TO16_MASK                            0xFFFF0000
//========================================
/* REG_MAC_WEP_USER_ACTIVE_INDICATION_47TO32 0x154 */
#define B2_MAC_WEP_USER_ACTIVE_INDICATION_47TO32_FIELD_USER_ACTIVE_47TO32_MASK                                    0x0000FFFF
#define B2_MAC_WEP_USER_ACTIVE_INDICATION_47TO32_FIELD_USER_ACTIVE_TX_RX_N_47TO32_MASK                            0xFFFF0000
//========================================
/* REG_MAC_WEP_USER_ACTIVE_INDICATION_63TO48 0x158 */
#define B2_MAC_WEP_USER_ACTIVE_INDICATION_63TO48_FIELD_USER_ACTIVE_63TO48_MASK                                    0x0000FFFF
#define B2_MAC_WEP_USER_ACTIVE_INDICATION_63TO48_FIELD_USER_ACTIVE_TX_RX_N_63TO48_MASK                            0xFFFF0000
//========================================
/* REG_MAC_WEP_USER_ACTIVE_INDICATION_71TO64 0x15C */
#define B2_MAC_WEP_USER_ACTIVE_INDICATION_71TO64_FIELD_USER_ACTIVE_71TO64_MASK                                    0x000000FF
#define B2_MAC_WEP_USER_ACTIVE_INDICATION_71TO64_FIELD_USER_ACTIVE_TX_RX_N_71TO64_MASK                            0x00FF0000
//========================================
/* REG_MAC_WEP_TX_IDLE_INDICATION 0x160 */
#define B2_MAC_WEP_TX_IDLE_INDICATION_FIELD_TX_USER_CNT_MASK                                                      0x0000007F
#define B2_MAC_WEP_TX_IDLE_INDICATION_FIELD_TX_SECURITY_PIPE_IDLE_MASK                                            0x00000100
//========================================
/* REG_MAC_WEP_TX_DLM_FC_CONTROL 0x164 */
#define B2_MAC_WEP_TX_DLM_FC_CONTROL_FIELD_CR_ENABLE_TX_DLM_MASK                                                  0x00000001
#define B2_MAC_WEP_TX_DLM_FC_CONTROL_FIELD_CR_TX_BYPASS_RD_FC_MASK                                                0x00000002
#define B2_MAC_WEP_TX_DLM_FC_CONTROL_FIELD_CR_TX_DISABLE_UPDATE_WR_FC_MASK                                        0x00000004
//========================================
/* REG_MAC_WEP_RX_DLM_FC_CONTROL 0x168 */
#define B2_MAC_WEP_RX_DLM_FC_CONTROL_FIELD_CR_ENABLE_RX_DLM_MASK                                                  0x00000001
#define B2_MAC_WEP_RX_DLM_FC_CONTROL_FIELD_CR_RX_BYPASS_RD_FC_MASK                                                0x00000002
#define B2_MAC_WEP_RX_DLM_FC_CONTROL_FIELD_CR_RX_DISABLE_UPDATE_WR_FC_MASK                                        0x00000004
#define B2_MAC_WEP_RX_DLM_FC_CONTROL_FIELD_CR_BLOCK_RX_IF_TX_DESC_NOT_EMPTY_MASK                                  0x00000008
//========================================
/* REG_MAC_WEP_WR_FC_STROBES 0x16C */
#define B2_MAC_WEP_WR_FC_STROBES_FIELD_CR_CLR_ROOF_MIN_ADDR_PULSE_MASK                                            0x00000001
#define B2_MAC_WEP_WR_FC_STROBES_FIELD_CR_TX_CLEAR_WR_FC_PULSE_MASK                                               0x00000002
#define B2_MAC_WEP_WR_FC_STROBES_FIELD_CR_RX_CLEAR_WR_FC_PULSE_MASK                                               0x00000004
//========================================
/* REG_MAC_WEP_CR_MAX_TKIP_FRAG_LENGTH 0x170 */
#define B2_MAC_WEP_CR_MAX_TKIP_FRAG_LENGTH_FIELD_CR_MAX_TKIP_FRAG_LENGTH_MASK                                     0x0000FFFF
//========================================
/* REG_MAC_WEP_CR_USER_BYPASS_HC_RD_FC_31TO0 0x174 */
#define B2_MAC_WEP_CR_USER_BYPASS_HC_RD_FC_31TO0_FIELD_CR_USER_BYPASS_HC_RD_FC_31TO0_MASK                         0xFFFFFFFF
//========================================
/* REG_MAC_WEP_CR_USER_BYPASS_HC_RD_FC_35TO32 0x178 */
#define B2_MAC_WEP_CR_USER_BYPASS_HC_RD_FC_35TO32_FIELD_CR_USER_BYPASS_HC_RD_FC_35TO32_MASK                       0x0000000F
//========================================
/* REG_MAC_WEP_BIP_FC_MASK 0x17C */
#define B2_MAC_WEP_BIP_FC_MASK_FIELD_CR_BIP_FC_MASK_MASK                                                          0x0000FFFF
//========================================
/* REG_MAC_WEP_SPARE_REG 0x180 */
#define B2_MAC_WEP_SPARE_REG_FIELD_SPARE_REG_MASK                                                                 0xFFFFFFFF
//========================================
/* REG_MAC_WEP_TX_MLDAR_CTRL 0x184 */
#define B2_MAC_WEP_TX_MLDAR_CTRL_FIELD_TX_MLDAR_ENA_MASK                                                          0x00000001
#define B2_MAC_WEP_TX_MLDAR_CTRL_FIELD_TX_MLDAR_OVERRIDE_PRIO_MASK                                                0x00000002
#define B2_MAC_WEP_TX_MLDAR_CTRL_FIELD_TX_MLDAR_SET_PRIO_VALUE_MASK                                               0x00000004
//========================================
/* REG_MAC_WEP_TX_MLDAR_RA_TA_READ_ADDR 0x188 */
#define B2_MAC_WEP_TX_MLDAR_RA_TA_READ_ADDR_FIELD_TX_MLDAR_RA_TA_READ_ADDR_MASK                                   0x0000001F
//========================================
/* REG_MAC_WEP_TX_MLDAR_RA_TA_RD_DATA0 0x18C */
#define B2_MAC_WEP_TX_MLDAR_RA_TA_RD_DATA0_FIELD_TX_MLDAR_RA_TA_RD_DATA0_MASK                                     0xFFFFFFFF
//========================================
/* REG_MAC_WEP_TX_MLDAR_RA_TA_RD_DATA1 0x190 */
#define B2_MAC_WEP_TX_MLDAR_RA_TA_RD_DATA1_FIELD_TX_MLDAR_RA_TA_RD_DATA1_MASK                                     0xFFFFFFFF
//========================================
/* REG_MAC_WEP_TX_MLDAR_RA_TA_RD_DATA2 0x194 */
#define B2_MAC_WEP_TX_MLDAR_RA_TA_RD_DATA2_FIELD_TX_MLDAR_RA_TA_RD_DATA2_MASK                                     0xFFFFFFFF
//========================================
/* REG_MAC_WEP_TX_MLDAR_DBG_BUSY_VEC 0x198 */
#define B2_MAC_WEP_TX_MLDAR_DBG_BUSY_VEC_FIELD_TX_MLDAR_DBG_BUSY_VEC_MASK                                         0xFFFFFFFF
//========================================
/* REG_MAC_WEP_TX_MLDAR_DBG_SMC_ADDR 0x19C */
#define B2_MAC_WEP_TX_MLDAR_DBG_SMC_ADDR_FIELD_TX_MLDAR_DBG_SMC_ADDR_MASK                                         0x003FFFFF
//========================================
/* REG_MAC_WEP_TX_MLDAR_DBG 0x1A0 */
#define B2_MAC_WEP_TX_MLDAR_DBG_FIELD_TX_MLDAR_DBG_IS_MLD_MASK                                                    0x00000001
#define B2_MAC_WEP_TX_MLDAR_DBG_FIELD_TX_MLDAR_DBG_MAX_FIFO_USEDW_MASK                                            0x0000001E
#define B2_MAC_WEP_TX_MLDAR_DBG_FIELD_TX_MLDAR_DBG_CURR_USER_ID_MASK                                              0x000003E0
#define B2_MAC_WEP_TX_MLDAR_DBG_FIELD_TX_MLDAR_DBG_SEC_USER_ID_MASK                                               0x00007C00
#define B2_MAC_WEP_TX_MLDAR_DBG_FIELD_TX_MLDAR_DBG_SM_STATE_MASK                                                  0x00018000
#define B2_MAC_WEP_TX_MLDAR_DBG_FIELD_TX_MLDAR_ERROR_MASK                                                         0x00020000
#define B2_MAC_WEP_TX_MLDAR_DBG_FIELD_TX_MLDAR_PRIORITY_MASK                                                      0x00040000
#define B2_MAC_WEP_TX_MLDAR_DBG_FIELD_TX_MLDAR_DBG_IS_CLIENT_MASK                                                 0x00080000
//========================================
/* REG_MAC_WEP_TX_MLDAR_SKIP_USER 0x1A4 */
#define B2_MAC_WEP_TX_MLDAR_SKIP_USER_FIELD_TX_MLDAR_SKIP_USER_MASK                                               0xFFFFFFFF


#endif // _MAC_WEP_REGS_H_
