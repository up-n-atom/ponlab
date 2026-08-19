
/***********************************************************************************
File:				B0HobMacBfmRegs.h
Module:				b0HobMacBfm
SOC Revision:		latest
Generated at:       2022-05-04 10:57:07
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_HOB_MAC_BFM_REGS_H_
#define _B0_HOB_MAC_BFM_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_HOB_MAC_BFM_BASE_ADDRESS                             MEMORY_MAP_UNIT_501_BASE_ADDRESS
#define	REG_B0_HOB_MAC_BFM_BFEE_STA                      (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x0)
#define	REG_B0_HOB_MAC_BFM_LENGTH_WITH_DELIMITERS        (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x4)
#define	REG_B0_HOB_MAC_BFM_LENGTH_WITHOUT_DELIMITERS     (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x8)
#define	REG_B0_HOB_MAC_BFM_RD_BF_RPT_VAP_TID0_DB_ADDR    (B0_HOB_MAC_BFM_BASE_ADDRESS + 0xC)
#define	REG_B0_HOB_MAC_BFM_BFEE_PHY_MODE                 (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x10)
#define	REG_B0_HOB_MAC_BFM_RXC_PHY_NDP_BW_MRU            (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x14)
#define	REG_B0_HOB_MAC_BFM_TX_RECIPE_BASE_ADDR           (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x18)
#define	REG_B0_HOB_MAC_BFM_BFEE_TX_NUM_OF_MPDUS_TF       (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x1C)
#define	REG_B0_HOB_MAC_BFM_BFEE_TX_MPDUS_VECTOR_TF       (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x20)
#define	REG_B0_HOB_MAC_BFM_HOB_REAL_AMPDU_LENGTH         (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x24)
#define	REG_B0_HOB_MAC_BFM_TXC_SECTION_A_BYTE_LEN        (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x28)
#define	REG_B0_HOB_MAC_BFM_TXC_SECTION_B_BYTE_LEN        (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x2C)
#define	REG_B0_HOB_MAC_BFM_TXC_SECTION_A_POINTER         (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x30)
#define	REG_B0_HOB_MAC_BFM_TXC_SECTION_B_POINTER         (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x34)
#define	REG_B0_HOB_MAC_BFM_ACTION_AND_CATEGORY           (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x38)
#define	REG_B0_HOB_MAC_BFM_BFEE_GEN_FCS_VALID            (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x3C)
#define	REG_B0_HOB_MAC_BFM_BFEE_RESPONSE_TYPE_TF         (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x40)
#define	REG_B0_HOB_MAC_BFM_BFEE_INVALID_TX_DATA          (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x44)
#define	REG_B0_HOB_MAC_BFM_BFEE_SM                       (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x4C)
#define	REG_B0_HOB_MAC_BFM_BFEE_MIMO_CONTROL_0           (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x50)
#define	REG_B0_HOB_MAC_BFM_BFEE_MIMO_CONTROL_1           (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x54)
#define	REG_B0_HOB_MAC_BFM_BFEE_MIMO_CONTROL_2           (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x58)
#define	REG_B0_HOB_MAC_BFM_BFEE_MIMO_CONTROL_EXT         (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x5C)
#define	REG_B0_HOB_MAC_BFM_SECTION_C_BASE_ADDR_0         (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x60)
#define	REG_B0_HOB_MAC_BFM_SECTION_C_LENGTH_0            (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x64)
#define	REG_B0_HOB_MAC_BFM_SECTION_D_BASE_ADDR_0         (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x68)
#define	REG_B0_HOB_MAC_BFM_SECTION_D_LENGTH_0            (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x6C)
#define	REG_B0_HOB_MAC_BFM_SECTION_C_BASE_ADDR_1         (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x70)
#define	REG_B0_HOB_MAC_BFM_SECTION_C_LENGTH_1            (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x74)
#define	REG_B0_HOB_MAC_BFM_SECTION_D_BASE_ADDR_1         (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x78)
#define	REG_B0_HOB_MAC_BFM_SECTION_D_LENGTH_1            (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x7C)
#define	REG_B0_HOB_MAC_BFM_SECTION_C_BASE_ADDR_2         (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x80)
#define	REG_B0_HOB_MAC_BFM_SECTION_C_LENGTH_2            (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x84)
#define	REG_B0_HOB_MAC_BFM_SECTION_D_BASE_ADDR_2         (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x88)
#define	REG_B0_HOB_MAC_BFM_SECTION_D_LENGTH_2            (B0_HOB_MAC_BFM_BASE_ADDRESS + 0x8C)
#define	REG_B0_HOB_MAC_BFM_BFEE_MPDU_LENGTH_0            (B0_HOB_MAC_BFM_BASE_ADDRESS + 0xB8)
#define	REG_B0_HOB_MAC_BFM_BFEE_MPDU_LENGTH_1            (B0_HOB_MAC_BFM_BASE_ADDRESS + 0xBC)
#define	REG_B0_HOB_MAC_BFM_BFEE_MPDU_LENGTH_2            (B0_HOB_MAC_BFM_BASE_ADDRESS + 0xC0)
#define	REG_B0_HOB_MAC_BFM_BFEE_STATIC_INDIACTIONS       (B0_HOB_MAC_BFM_BASE_ADDRESS + 0xD0)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_HOB_MAC_BFM_BFEE_STA 0x0 */
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
} RegB0HobMacBfmBfeeSta_u;

/*REG_B0_HOB_MAC_BFM_LENGTH_WITH_DELIMITERS 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lengthWithDelimiters : 15; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB0HobMacBfmLengthWithDelimiters_u;

/*REG_B0_HOB_MAC_BFM_LENGTH_WITHOUT_DELIMITERS 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lengthWithoutDelimiters : 15; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB0HobMacBfmLengthWithoutDelimiters_u;

/*REG_B0_HOB_MAC_BFM_RD_BF_RPT_VAP_TID0_DB_ADDR 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdBfRptVapTid0DbAddr : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB0HobMacBfmRdBfRptVapTid0DbAddr_u;

/*REG_B0_HOB_MAC_BFM_BFEE_PHY_MODE 0x10 */
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
} RegB0HobMacBfmBfeePhyMode_u;

/*REG_B0_HOB_MAC_BFM_RXC_PHY_NDP_BW_MRU 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcPhyNdpBwSmp : 4; //no description, reset value: 0x0, access type: RO
		uint32 rxcPhyNdpMruSmp : 4; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0HobMacBfmRxcPhyNdpBwMru_u;

/*REG_B0_HOB_MAC_BFM_TX_RECIPE_BASE_ADDR 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txTcrBaseAddr : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB0HobMacBfmTxRecipeBaseAddr_u;

/*REG_B0_HOB_MAC_BFM_BFEE_TX_NUM_OF_MPDUS_TF 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeTxNumOfMpdusTf : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0HobMacBfmBfeeTxNumOfMpdusTf_u;

/*REG_B0_HOB_MAC_BFM_BFEE_TX_MPDUS_VECTOR_TF 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeTxMpdusVectorTf : 3; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0HobMacBfmBfeeTxMpdusVectorTf_u;

/*REG_B0_HOB_MAC_BFM_HOB_REAL_AMPDU_LENGTH 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hobRealAmpduLength : 15; //Length of current AMPDU, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB0HobMacBfmHobRealAmpduLength_u;

/*REG_B0_HOB_MAC_BFM_TXC_SECTION_A_BYTE_LEN 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txcSectionAByteLen : 13; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 19;
	} bitFields;
} RegB0HobMacBfmTxcSectionAByteLen_u;

/*REG_B0_HOB_MAC_BFM_TXC_SECTION_B_BYTE_LEN 0x2C */
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
} RegB0HobMacBfmTxcSectionBByteLen_u;

/*REG_B0_HOB_MAC_BFM_TXC_SECTION_A_POINTER 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txcSectionAPointer : 26; //Pointer for section a in the TX circular RAM, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB0HobMacBfmTxcSectionAPointer_u;

/*REG_B0_HOB_MAC_BFM_TXC_SECTION_B_POINTER 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txcSectionBPointer : 26; //Pointer for section a in the TX circular RAM, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB0HobMacBfmTxcSectionBPointer_u;

/*REG_B0_HOB_MAC_BFM_ACTION_AND_CATEGORY 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 categoryIn : 8; //no description, reset value: 0x0, access type: RO
		uint32 actionIn : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0HobMacBfmActionAndCategory_u;

/*REG_B0_HOB_MAC_BFM_BFEE_GEN_FCS_VALID 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeGenFcsValid : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0HobMacBfmBfeeGenFcsValid_u;

/*REG_B0_HOB_MAC_BFM_BFEE_RESPONSE_TYPE_TF 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeResponseTypeTf : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0HobMacBfmBfeeResponseTypeTf_u;

/*REG_B0_HOB_MAC_BFM_BFEE_INVALID_TX_DATA 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeInvalidTxData : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0HobMacBfmBfeeInvalidTxData_u;

/*REG_B0_HOB_MAC_BFM_BFEE_SM 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeBfrSuccess : 1; //no description, reset value: 0x0, access type: RO
		uint32 bfBfeeSmIdle : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0HobMacBfmBfeeSm_u;

/*REG_B0_HOB_MAC_BFM_BFEE_MIMO_CONTROL_0 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeMimoControl0 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0HobMacBfmBfeeMimoControl0_u;

/*REG_B0_HOB_MAC_BFM_BFEE_MIMO_CONTROL_1 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeMimoControl1 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0HobMacBfmBfeeMimoControl1_u;

/*REG_B0_HOB_MAC_BFM_BFEE_MIMO_CONTROL_2 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeMimoControl2 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0HobMacBfmBfeeMimoControl2_u;

/*REG_B0_HOB_MAC_BFM_BFEE_MIMO_CONTROL_EXT 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mimoControlExtension : 8; //extending mimo control to 5 bytes, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0HobMacBfmBfeeMimoControlExt_u;

/*REG_B0_HOB_MAC_BFM_SECTION_C_BASE_ADDR_0 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionCBaseAddr0 : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB0HobMacBfmSectionCBaseAddr0_u;

/*REG_B0_HOB_MAC_BFM_SECTION_C_LENGTH_0 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionCLength0 : 14; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB0HobMacBfmSectionCLength0_u;

/*REG_B0_HOB_MAC_BFM_SECTION_D_BASE_ADDR_0 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionDBaseAddr0 : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB0HobMacBfmSectionDBaseAddr0_u;

/*REG_B0_HOB_MAC_BFM_SECTION_D_LENGTH_0 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionDLength0 : 11; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB0HobMacBfmSectionDLength0_u;

/*REG_B0_HOB_MAC_BFM_SECTION_C_BASE_ADDR_1 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionCBaseAddr1 : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB0HobMacBfmSectionCBaseAddr1_u;

/*REG_B0_HOB_MAC_BFM_SECTION_C_LENGTH_1 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionCLength1 : 14; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB0HobMacBfmSectionCLength1_u;

/*REG_B0_HOB_MAC_BFM_SECTION_D_BASE_ADDR_1 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionDBaseAddr1 : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB0HobMacBfmSectionDBaseAddr1_u;

/*REG_B0_HOB_MAC_BFM_SECTION_D_LENGTH_1 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionDLength1 : 11; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB0HobMacBfmSectionDLength1_u;

/*REG_B0_HOB_MAC_BFM_SECTION_C_BASE_ADDR_2 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionCBaseAddr2 : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB0HobMacBfmSectionCBaseAddr2_u;

/*REG_B0_HOB_MAC_BFM_SECTION_C_LENGTH_2 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionCLength2 : 14; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB0HobMacBfmSectionCLength2_u;

/*REG_B0_HOB_MAC_BFM_SECTION_D_BASE_ADDR_2 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionDBaseAddr2 : 26; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB0HobMacBfmSectionDBaseAddr2_u;

/*REG_B0_HOB_MAC_BFM_SECTION_D_LENGTH_2 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sectionDLength2 : 11; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB0HobMacBfmSectionDLength2_u;

/*REG_B0_HOB_MAC_BFM_BFEE_MPDU_LENGTH_0 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeMpduLength0 : 14; //Length in bytes for MPDU 0. This field is valid only when MPDU 0 is required for this TX BFee session, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB0HobMacBfmBfeeMpduLength0_u;

/*REG_B0_HOB_MAC_BFM_BFEE_MPDU_LENGTH_1 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeMpduLength1 : 14; //Length in bytes for MPDU 1. This field is valid only when MPDU 1 is required for this TX BFee session, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB0HobMacBfmBfeeMpduLength1_u;

/*REG_B0_HOB_MAC_BFM_BFEE_MPDU_LENGTH_2 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeMpduLength2 : 14; //Length in bytes for MPDU 2. This field is valid only when MPDU 2 is required for this TX BFee session, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB0HobMacBfmBfeeMpduLength2_u;

/*REG_B0_HOB_MAC_BFM_BFEE_STATIC_INDIACTIONS 0xD0 */
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
} RegB0HobMacBfmBfeeStaticIndiactions_u;



#endif // _B0_HOB_MAC_BFM_REGS_H_
