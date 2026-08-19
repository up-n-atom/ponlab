
/***********************************************************************************
File:				HobMacBfmRegs.h
Module:				hobMacBfm
SOC Revision:		latest
Generated at:       2024-06-26 12:55:13
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _HOB_MAC_BFM_REGS_H_
#define _HOB_MAC_BFM_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define HOB_MAC_BFM_BASE_ADDRESS                    MEMORY_MAP_UNIT_501_BASE_ADDRESS
#define	REG_HOB_MAC_BFM_BFEE_STA                      (HOB_MAC_BFM_BASE_ADDRESS + 0x0)
#define	REG_HOB_MAC_BFM_LENGTH_WITH_DELIMITERS        (HOB_MAC_BFM_BASE_ADDRESS + 0x4)
#define	REG_HOB_MAC_BFM_LENGTH_WITHOUT_DELIMITERS     (HOB_MAC_BFM_BASE_ADDRESS + 0x8)
#define	REG_HOB_MAC_BFM_RD_BF_RPT_VAP_TID0_DB_ADDR    (HOB_MAC_BFM_BASE_ADDRESS + 0xC)
#define	REG_HOB_MAC_BFM_BFEE_PHY_MODE                 (HOB_MAC_BFM_BASE_ADDRESS + 0x10)
#define	REG_HOB_MAC_BFM_RXC_PHY_NDP_BW_MRU            (HOB_MAC_BFM_BASE_ADDRESS + 0x14)
#define	REG_HOB_MAC_BFM_TX_RECIPE_BASE_ADDR           (HOB_MAC_BFM_BASE_ADDRESS + 0x18)
#define	REG_HOB_MAC_BFM_BFEE_TX_NUM_OF_MPDUS_TF       (HOB_MAC_BFM_BASE_ADDRESS + 0x1C)
#define	REG_HOB_MAC_BFM_BFEE_TX_MPDUS_VECTOR_TF       (HOB_MAC_BFM_BASE_ADDRESS + 0x20)
#define	REG_HOB_MAC_BFM_HOB_REAL_AMPDU_LENGTH         (HOB_MAC_BFM_BASE_ADDRESS + 0x24)
#define	REG_HOB_MAC_BFM_TXC_SECTION_A_BYTE_LEN        (HOB_MAC_BFM_BASE_ADDRESS + 0x28)
#define	REG_HOB_MAC_BFM_TXC_SECTION_B_BYTE_LEN        (HOB_MAC_BFM_BASE_ADDRESS + 0x2C)
#define	REG_HOB_MAC_BFM_TXC_SECTION_A_POINTER         (HOB_MAC_BFM_BASE_ADDRESS + 0x30)
#define	REG_HOB_MAC_BFM_TXC_SECTION_B_POINTER         (HOB_MAC_BFM_BASE_ADDRESS + 0x34)
#define	REG_HOB_MAC_BFM_ACTION_AND_CATEGORY           (HOB_MAC_BFM_BASE_ADDRESS + 0x38)
#define	REG_HOB_MAC_BFM_BFEE_GEN_FCS_VALID            (HOB_MAC_BFM_BASE_ADDRESS + 0x3C)
#define	REG_HOB_MAC_BFM_BFEE_RESPONSE_TYPE_TF         (HOB_MAC_BFM_BASE_ADDRESS + 0x40)
#define	REG_HOB_MAC_BFM_BFEE_INVALID_TX_DATA          (HOB_MAC_BFM_BASE_ADDRESS + 0x44)
#define	REG_HOB_MAC_BFM_BFEE_SM                       (HOB_MAC_BFM_BASE_ADDRESS + 0x4C)
#define	REG_HOB_MAC_BFM_BFEE_MIMO_CONTROL_0           (HOB_MAC_BFM_BASE_ADDRESS + 0x50)
#define	REG_HOB_MAC_BFM_BFEE_MIMO_CONTROL_1           (HOB_MAC_BFM_BASE_ADDRESS + 0x54)
#define	REG_HOB_MAC_BFM_BFEE_MIMO_CONTROL_2           (HOB_MAC_BFM_BASE_ADDRESS + 0x58)
#define	REG_HOB_MAC_BFM_BFEE_MIMO_CONTROL_EXT         (HOB_MAC_BFM_BASE_ADDRESS + 0x5C)
#define	REG_HOB_MAC_BFM_SECTION_C_BASE_ADDR_0         (HOB_MAC_BFM_BASE_ADDRESS + 0x60)
#define	REG_HOB_MAC_BFM_SECTION_C_LENGTH_0            (HOB_MAC_BFM_BASE_ADDRESS + 0x64)
#define	REG_HOB_MAC_BFM_SECTION_D_BASE_ADDR_0         (HOB_MAC_BFM_BASE_ADDRESS + 0x68)
#define	REG_HOB_MAC_BFM_SECTION_D_LENGTH_0            (HOB_MAC_BFM_BASE_ADDRESS + 0x6C)
#define	REG_HOB_MAC_BFM_SECTION_C_BASE_ADDR_1         (HOB_MAC_BFM_BASE_ADDRESS + 0x70)
#define	REG_HOB_MAC_BFM_SECTION_C_LENGTH_1            (HOB_MAC_BFM_BASE_ADDRESS + 0x74)
#define	REG_HOB_MAC_BFM_SECTION_D_BASE_ADDR_1         (HOB_MAC_BFM_BASE_ADDRESS + 0x78)
#define	REG_HOB_MAC_BFM_SECTION_D_LENGTH_1            (HOB_MAC_BFM_BASE_ADDRESS + 0x7C)
#define	REG_HOB_MAC_BFM_SECTION_C_BASE_ADDR_2         (HOB_MAC_BFM_BASE_ADDRESS + 0x80)
#define	REG_HOB_MAC_BFM_SECTION_C_LENGTH_2            (HOB_MAC_BFM_BASE_ADDRESS + 0x84)
#define	REG_HOB_MAC_BFM_SECTION_D_BASE_ADDR_2         (HOB_MAC_BFM_BASE_ADDRESS + 0x88)
#define	REG_HOB_MAC_BFM_SECTION_D_LENGTH_2            (HOB_MAC_BFM_BASE_ADDRESS + 0x8C)
#define	REG_HOB_MAC_BFM_BFEE_MPDU_LENGTH_0            (HOB_MAC_BFM_BASE_ADDRESS + 0xB8)
#define	REG_HOB_MAC_BFM_BFEE_MPDU_LENGTH_1            (HOB_MAC_BFM_BASE_ADDRESS + 0xBC)
#define	REG_HOB_MAC_BFM_BFEE_MPDU_LENGTH_2            (HOB_MAC_BFM_BASE_ADDRESS + 0xC0)
#define	REG_HOB_MAC_BFM_BFEE_STATIC_INDIACTIONS       (HOB_MAC_BFM_BASE_ADDRESS + 0xD0)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_HOB_MAC_BFM_BFEE_STA 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxStaIdxUsr0Smp : 9; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 7;
		uint32 rxcVapIdxUsr0Smp : 5; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 11;
	} bitFields;
} RegHobMacBfmBfeeSta_u;

/*REG_HOB_MAC_BFM_LENGTH_WITH_DELIMITERS 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lengthWithDelimiters : 15; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegHobMacBfmLengthWithDelimiters_u;

/*REG_HOB_MAC_BFM_LENGTH_WITHOUT_DELIMITERS 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lengthWithoutDelimiters : 15; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegHobMacBfmLengthWithoutDelimiters_u;

/*REG_HOB_MAC_BFM_RD_BF_RPT_VAP_TID0_DB_ADDR 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdBfRptVapTid0DbAddr : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegHobMacBfmRdBfRptVapTid0DbAddr_u;

/*REG_HOB_MAC_BFM_BFEE_PHY_MODE 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mpPhyModeIsHt : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 mpPhyMode : 3; //MBFM phy mode encoding (0-HT ...), reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 txPhyMode : 4; //phy_mode_enum encoding, reset value: 0x0, access type: RO
		uint32 reserved2 : 4;
		uint32 txPhyModeAddDelimiter : 1; //This bit is 1 in case of tx_phy_mode VHT or HE or EHT, reset value: 0x0, access type: RO
		uint32 reserved3 : 15;
	} bitFields;
} RegHobMacBfmBfeePhyMode_u;

/*REG_HOB_MAC_BFM_RXC_PHY_NDP_BW_MRU 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcPhyNdpBwSmp : 4; //no description, reset value: 0x0, access type: RO
		uint32 rxcPhyNdpMruSmp : 4; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegHobMacBfmRxcPhyNdpBwMru_u;

/*REG_HOB_MAC_BFM_TX_RECIPE_BASE_ADDR 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txTcrBaseAddr : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegHobMacBfmTxRecipeBaseAddr_u;

/*REG_HOB_MAC_BFM_BFEE_TX_NUM_OF_MPDUS_TF 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeTxNumOfMpdusTf : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegHobMacBfmBfeeTxNumOfMpdusTf_u;

/*REG_HOB_MAC_BFM_BFEE_TX_MPDUS_VECTOR_TF 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeTxMpdusVectorTf : 3; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegHobMacBfmBfeeTxMpdusVectorTf_u;

/*REG_HOB_MAC_BFM_HOB_REAL_AMPDU_LENGTH 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hobRealAmpduLength : 15; //Length of current AMPDU, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegHobMacBfmHobRealAmpduLength_u;

/*REG_HOB_MAC_BFM_TXC_SECTION_A_BYTE_LEN 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txcSectionAByteLen : 13; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 19;
	} bitFields;
} RegHobMacBfmTxcSectionAByteLen_u;

/*REG_HOB_MAC_BFM_TXC_SECTION_B_BYTE_LEN 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txcSectionB1ByteLen : 3; //1-4 to tell the amount of bytes on section B first section , , reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
		uint32 txcSectionB2ByteLen : 3; //0-4 to tell the amount of bytes on section B second section, reset value: 0x0, access type: RO
		uint32 reserved1 : 21;
	} bitFields;
} RegHobMacBfmTxcSectionBByteLen_u;

/*REG_HOB_MAC_BFM_TXC_SECTION_A_POINTER 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txcSectionAPointer : 26; //Pointer for section a in the TX circular RAM, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegHobMacBfmTxcSectionAPointer_u;

/*REG_HOB_MAC_BFM_TXC_SECTION_B_POINTER 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txcSectionBPointer : 26; //Pointer for section a in the TX circular RAM, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegHobMacBfmTxcSectionBPointer_u;

/*REG_HOB_MAC_BFM_ACTION_AND_CATEGORY 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 categoryIn : 8; //no description, reset value: 0x0, access type: RO
		uint32 actionIn : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegHobMacBfmActionAndCategory_u;

/*REG_HOB_MAC_BFM_BFEE_GEN_FCS_VALID 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeGenFcsValid : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHobMacBfmBfeeGenFcsValid_u;

/*REG_HOB_MAC_BFM_BFEE_RESPONSE_TYPE_TF 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeResponseTypeTf : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegHobMacBfmBfeeResponseTypeTf_u;

/*REG_HOB_MAC_BFM_BFEE_INVALID_TX_DATA 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeInvalidTxData : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegHobMacBfmBfeeInvalidTxData_u;

/*REG_HOB_MAC_BFM_BFEE_SM 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeBfrSuccess : 1; //no description, reset value: 0x0, access type: RO
		uint32 bfBfeeSmIdle : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegHobMacBfmBfeeSm_u;

/*REG_HOB_MAC_BFM_BFEE_MIMO_CONTROL_0 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeMimoControl0 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegHobMacBfmBfeeMimoControl0_u;

/*REG_HOB_MAC_BFM_BFEE_MIMO_CONTROL_1 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeMimoControl1 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegHobMacBfmBfeeMimoControl1_u;

/*REG_HOB_MAC_BFM_BFEE_MIMO_CONTROL_2 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeMimoControl2 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegHobMacBfmBfeeMimoControl2_u;

/*REG_HOB_MAC_BFM_BFEE_MIMO_CONTROL_EXT 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mimoControlExtension : 8; //extending mimo control to 5 bytes, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegHobMacBfmBfeeMimoControlExt_u;

/*REG_HOB_MAC_BFM_SECTION_C_BASE_ADDR_0 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionCBaseAddr0 : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegHobMacBfmSectionCBaseAddr0_u;

/*REG_HOB_MAC_BFM_SECTION_C_LENGTH_0 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionCLength0 : 14; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegHobMacBfmSectionCLength0_u;

/*REG_HOB_MAC_BFM_SECTION_D_BASE_ADDR_0 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionDBaseAddr0 : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegHobMacBfmSectionDBaseAddr0_u;

/*REG_HOB_MAC_BFM_SECTION_D_LENGTH_0 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionDLength0 : 11; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegHobMacBfmSectionDLength0_u;

/*REG_HOB_MAC_BFM_SECTION_C_BASE_ADDR_1 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionCBaseAddr1 : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegHobMacBfmSectionCBaseAddr1_u;

/*REG_HOB_MAC_BFM_SECTION_C_LENGTH_1 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionCLength1 : 14; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegHobMacBfmSectionCLength1_u;

/*REG_HOB_MAC_BFM_SECTION_D_BASE_ADDR_1 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionDBaseAddr1 : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegHobMacBfmSectionDBaseAddr1_u;

/*REG_HOB_MAC_BFM_SECTION_D_LENGTH_1 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionDLength1 : 11; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegHobMacBfmSectionDLength1_u;

/*REG_HOB_MAC_BFM_SECTION_C_BASE_ADDR_2 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionCBaseAddr2 : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegHobMacBfmSectionCBaseAddr2_u;

/*REG_HOB_MAC_BFM_SECTION_C_LENGTH_2 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionCLength2 : 14; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegHobMacBfmSectionCLength2_u;

/*REG_HOB_MAC_BFM_SECTION_D_BASE_ADDR_2 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionDBaseAddr2 : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegHobMacBfmSectionDBaseAddr2_u;

/*REG_HOB_MAC_BFM_SECTION_D_LENGTH_2 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionDLength2 : 11; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegHobMacBfmSectionDLength2_u;

/*REG_HOB_MAC_BFM_BFEE_MPDU_LENGTH_0 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeMpduLength0 : 14; //Length in bytes for MPDU 0. This field is valid only when MPDU 0 is required for this TX BFee session, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegHobMacBfmBfeeMpduLength0_u;

/*REG_HOB_MAC_BFM_BFEE_MPDU_LENGTH_1 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeMpduLength1 : 14; //Length in bytes for MPDU 1. This field is valid only when MPDU 1 is required for this TX BFee session, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegHobMacBfmBfeeMpduLength1_u;

/*REG_HOB_MAC_BFM_BFEE_MPDU_LENGTH_2 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeMpduLength2 : 14; //Length in bytes for MPDU 2. This field is valid only when MPDU 2 is required for this TX BFee session, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegHobMacBfmBfeeMpduLength2_u;

/*REG_HOB_MAC_BFM_BFEE_STATIC_INDIACTIONS 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeLengthReady : 1; //no description, reset value: 0x0, access type: RO
		uint32 txcSectionAReady : 1; //no description, reset value: 0x0, access type: RO
		uint32 txcSectionBReady : 1; //no description, reset value: 0x0, access type: RO
		uint32 txcSectionCReady : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegHobMacBfmBfeeStaticIndiactions_u;

//========================================
/* REG_HOB_MAC_BFM_BFEE_STA 0x0 */
#define B0_HOB_MAC_BFM_BFEE_STA_FIELD_RX_STA_IDX_USR0_SMP_MASK                                                    0x000001FF
#define B0_HOB_MAC_BFM_BFEE_STA_FIELD_RXC_VAP_IDX_USR0_SMP_MASK                                                   0x001F0000
//========================================
/* REG_HOB_MAC_BFM_LENGTH_WITH_DELIMITERS 0x4 */
#define B0_HOB_MAC_BFM_LENGTH_WITH_DELIMITERS_FIELD_LENGTH_WITH_DELIMITERS_MASK                                   0x00007FFF
//========================================
/* REG_HOB_MAC_BFM_LENGTH_WITHOUT_DELIMITERS 0x8 */
#define B0_HOB_MAC_BFM_LENGTH_WITHOUT_DELIMITERS_FIELD_LENGTH_WITHOUT_DELIMITERS_MASK                             0x00007FFF
//========================================
/* REG_HOB_MAC_BFM_RD_BF_RPT_VAP_TID0_DB_ADDR 0xC */
#define B0_HOB_MAC_BFM_RD_BF_RPT_VAP_TID0_DB_ADDR_FIELD_RD_BF_RPT_VAP_TID0_DB_ADDR_MASK                           0x03FFFFFF
//========================================
/* REG_HOB_MAC_BFM_BFEE_PHY_MODE 0x10 */
#define B0_HOB_MAC_BFM_BFEE_PHY_MODE_FIELD_MP_PHY_MODE_IS_HT_MASK                                                 0x00000001
#define B0_HOB_MAC_BFM_BFEE_PHY_MODE_FIELD_MP_PHY_MODE_MASK                                                       0x00000070
#define B0_HOB_MAC_BFM_BFEE_PHY_MODE_FIELD_TX_PHY_MODE_MASK                                                       0x00000F00
#define B0_HOB_MAC_BFM_BFEE_PHY_MODE_FIELD_TX_PHY_MODE_ADD_DELIMITER_MASK                                         0x00010000
//========================================
/* REG_HOB_MAC_BFM_RXC_PHY_NDP_BW_MRU 0x14 */
#define B0_HOB_MAC_BFM_RXC_PHY_NDP_BW_MRU_FIELD_RXC_PHY_NDP_BW_SMP_MASK                                           0x0000000F
#define B0_HOB_MAC_BFM_RXC_PHY_NDP_BW_MRU_FIELD_RXC_PHY_NDP_MRU_SMP_MASK                                          0x000000F0
//========================================
/* REG_HOB_MAC_BFM_TX_RECIPE_BASE_ADDR 0x18 */
#define B0_HOB_MAC_BFM_TX_RECIPE_BASE_ADDR_FIELD_TX_TCR_BASE_ADDR_MASK                                            0x03FFFFFF
//========================================
/* REG_HOB_MAC_BFM_BFEE_TX_NUM_OF_MPDUS_TF 0x1C */
#define B0_HOB_MAC_BFM_BFEE_TX_NUM_OF_MPDUS_TF_FIELD_BFEE_TX_NUM_OF_MPDUS_TF_MASK                                 0x00000003
//========================================
/* REG_HOB_MAC_BFM_BFEE_TX_MPDUS_VECTOR_TF 0x20 */
#define B0_HOB_MAC_BFM_BFEE_TX_MPDUS_VECTOR_TF_FIELD_BFEE_TX_MPDUS_VECTOR_TF_MASK                                 0x00000007
//========================================
/* REG_HOB_MAC_BFM_HOB_REAL_AMPDU_LENGTH 0x24 */
#define B0_HOB_MAC_BFM_HOB_REAL_AMPDU_LENGTH_FIELD_HOB_REAL_AMPDU_LENGTH_MASK                                     0x00007FFF
//========================================
/* REG_HOB_MAC_BFM_TXC_SECTION_A_BYTE_LEN 0x28 */
#define B0_HOB_MAC_BFM_TXC_SECTION_A_BYTE_LEN_FIELD_TXC_SECTION_A_BYTE_LEN_MASK                                   0x00001FFF
//========================================
/* REG_HOB_MAC_BFM_TXC_SECTION_B_BYTE_LEN 0x2C */
#define B0_HOB_MAC_BFM_TXC_SECTION_B_BYTE_LEN_FIELD_TXC_SECTION_B1_BYTE_LEN_MASK                                  0x00000007
#define B0_HOB_MAC_BFM_TXC_SECTION_B_BYTE_LEN_FIELD_TXC_SECTION_B2_BYTE_LEN_MASK                                  0x00000700
//========================================
/* REG_HOB_MAC_BFM_TXC_SECTION_A_POINTER 0x30 */
#define B0_HOB_MAC_BFM_TXC_SECTION_A_POINTER_FIELD_TXC_SECTION_A_POINTER_MASK                                     0x03FFFFFF
//========================================
/* REG_HOB_MAC_BFM_TXC_SECTION_B_POINTER 0x34 */
#define B0_HOB_MAC_BFM_TXC_SECTION_B_POINTER_FIELD_TXC_SECTION_B_POINTER_MASK                                     0x03FFFFFF
//========================================
/* REG_HOB_MAC_BFM_ACTION_AND_CATEGORY 0x38 */
#define B0_HOB_MAC_BFM_ACTION_AND_CATEGORY_FIELD_CATEGORY_IN_MASK                                                 0x000000FF
#define B0_HOB_MAC_BFM_ACTION_AND_CATEGORY_FIELD_ACTION_IN_MASK                                                   0x0000FF00
//========================================
/* REG_HOB_MAC_BFM_BFEE_GEN_FCS_VALID 0x3C */
#define B0_HOB_MAC_BFM_BFEE_GEN_FCS_VALID_FIELD_BFEE_GEN_FCS_VALID_MASK                                           0x00000001
//========================================
/* REG_HOB_MAC_BFM_BFEE_RESPONSE_TYPE_TF 0x40 */
#define B0_HOB_MAC_BFM_BFEE_RESPONSE_TYPE_TF_FIELD_BFEE_RESPONSE_TYPE_TF_MASK                                     0x00000001
//========================================
/* REG_HOB_MAC_BFM_BFEE_INVALID_TX_DATA 0x44 */
#define B0_HOB_MAC_BFM_BFEE_INVALID_TX_DATA_FIELD_BFEE_INVALID_TX_DATA_MASK                                       0xFFFFFFFF
//========================================
/* REG_HOB_MAC_BFM_BFEE_SM 0x4C */
#define B0_HOB_MAC_BFM_BFEE_SM_FIELD_BFEE_BFR_SUCCESS_MASK                                                        0x00000001
#define B0_HOB_MAC_BFM_BFEE_SM_FIELD_BF_BFEE_SM_IDLE_MASK                                                         0x00000002
//========================================
/* REG_HOB_MAC_BFM_BFEE_MIMO_CONTROL_0 0x50 */
#define B0_HOB_MAC_BFM_BFEE_MIMO_CONTROL_0_FIELD_BFEE_MIMO_CONTROL_0_MASK                                         0xFFFFFFFF
//========================================
/* REG_HOB_MAC_BFM_BFEE_MIMO_CONTROL_1 0x54 */
#define B0_HOB_MAC_BFM_BFEE_MIMO_CONTROL_1_FIELD_BFEE_MIMO_CONTROL_1_MASK                                         0xFFFFFFFF
//========================================
/* REG_HOB_MAC_BFM_BFEE_MIMO_CONTROL_2 0x58 */
#define B0_HOB_MAC_BFM_BFEE_MIMO_CONTROL_2_FIELD_BFEE_MIMO_CONTROL_2_MASK                                         0xFFFFFFFF
//========================================
/* REG_HOB_MAC_BFM_BFEE_MIMO_CONTROL_EXT 0x5C */
#define B0_HOB_MAC_BFM_BFEE_MIMO_CONTROL_EXT_FIELD_MIMO_CONTROL_EXTENSION_MASK                                    0x000000FF
//========================================
/* REG_HOB_MAC_BFM_SECTION_C_BASE_ADDR_0 0x60 */
#define B0_HOB_MAC_BFM_SECTION_C_BASE_ADDR_0_FIELD_SECTION_C_BASE_ADDR_0_MASK                                     0x03FFFFFF
//========================================
/* REG_HOB_MAC_BFM_SECTION_C_LENGTH_0 0x64 */
#define B0_HOB_MAC_BFM_SECTION_C_LENGTH_0_FIELD_SECTION_C_LENGTH_0_MASK                                           0x00003FFF
//========================================
/* REG_HOB_MAC_BFM_SECTION_D_BASE_ADDR_0 0x68 */
#define B0_HOB_MAC_BFM_SECTION_D_BASE_ADDR_0_FIELD_SECTION_D_BASE_ADDR_0_MASK                                     0x03FFFFFF
//========================================
/* REG_HOB_MAC_BFM_SECTION_D_LENGTH_0 0x6C */
#define B0_HOB_MAC_BFM_SECTION_D_LENGTH_0_FIELD_SECTION_D_LENGTH_0_MASK                                           0x000007FF
//========================================
/* REG_HOB_MAC_BFM_SECTION_C_BASE_ADDR_1 0x70 */
#define B0_HOB_MAC_BFM_SECTION_C_BASE_ADDR_1_FIELD_SECTION_C_BASE_ADDR_1_MASK                                     0x03FFFFFF
//========================================
/* REG_HOB_MAC_BFM_SECTION_C_LENGTH_1 0x74 */
#define B0_HOB_MAC_BFM_SECTION_C_LENGTH_1_FIELD_SECTION_C_LENGTH_1_MASK                                           0x00003FFF
//========================================
/* REG_HOB_MAC_BFM_SECTION_D_BASE_ADDR_1 0x78 */
#define B0_HOB_MAC_BFM_SECTION_D_BASE_ADDR_1_FIELD_SECTION_D_BASE_ADDR_1_MASK                                     0x03FFFFFF
//========================================
/* REG_HOB_MAC_BFM_SECTION_D_LENGTH_1 0x7C */
#define B0_HOB_MAC_BFM_SECTION_D_LENGTH_1_FIELD_SECTION_D_LENGTH_1_MASK                                           0x000007FF
//========================================
/* REG_HOB_MAC_BFM_SECTION_C_BASE_ADDR_2 0x80 */
#define B0_HOB_MAC_BFM_SECTION_C_BASE_ADDR_2_FIELD_SECTION_C_BASE_ADDR_2_MASK                                     0x03FFFFFF
//========================================
/* REG_HOB_MAC_BFM_SECTION_C_LENGTH_2 0x84 */
#define B0_HOB_MAC_BFM_SECTION_C_LENGTH_2_FIELD_SECTION_C_LENGTH_2_MASK                                           0x00003FFF
//========================================
/* REG_HOB_MAC_BFM_SECTION_D_BASE_ADDR_2 0x88 */
#define B0_HOB_MAC_BFM_SECTION_D_BASE_ADDR_2_FIELD_SECTION_D_BASE_ADDR_2_MASK                                     0x03FFFFFF
//========================================
/* REG_HOB_MAC_BFM_SECTION_D_LENGTH_2 0x8C */
#define B0_HOB_MAC_BFM_SECTION_D_LENGTH_2_FIELD_SECTION_D_LENGTH_2_MASK                                           0x000007FF
//========================================
/* REG_HOB_MAC_BFM_BFEE_MPDU_LENGTH_0 0xB8 */
#define B0_HOB_MAC_BFM_BFEE_MPDU_LENGTH_0_FIELD_BFEE_MPDU_LENGTH_0_MASK                                           0x00003FFF
//========================================
/* REG_HOB_MAC_BFM_BFEE_MPDU_LENGTH_1 0xBC */
#define B0_HOB_MAC_BFM_BFEE_MPDU_LENGTH_1_FIELD_BFEE_MPDU_LENGTH_1_MASK                                           0x00003FFF
//========================================
/* REG_HOB_MAC_BFM_BFEE_MPDU_LENGTH_2 0xC0 */
#define B0_HOB_MAC_BFM_BFEE_MPDU_LENGTH_2_FIELD_BFEE_MPDU_LENGTH_2_MASK                                           0x00003FFF
//========================================
/* REG_HOB_MAC_BFM_BFEE_STATIC_INDIACTIONS 0xD0 */
#define B0_HOB_MAC_BFM_BFEE_STATIC_INDIACTIONS_FIELD_BFEE_LENGTH_READY_MASK                                       0x00000001
#define B0_HOB_MAC_BFM_BFEE_STATIC_INDIACTIONS_FIELD_TXC_SECTION_A_READY_MASK                                     0x00000002
#define B0_HOB_MAC_BFM_BFEE_STATIC_INDIACTIONS_FIELD_TXC_SECTION_B_READY_MASK                                     0x00000004
#define B0_HOB_MAC_BFM_BFEE_STATIC_INDIACTIONS_FIELD_TXC_SECTION_C_READY_MASK                                     0x00000008


#endif // _HOB_MAC_BFM_REGS_H_
