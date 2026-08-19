
/***********************************************************************************
File:				B2RxPpRegs.h
Module:				b2RxPp
SOC Revision:		latest
Generated at:       2024-06-26 12:54:59
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_RX_PP_REGS_H_
#define _B2_RX_PP_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_RX_PP_BASE_ADDRESS                         MEMORY_MAP_UNIT_1247_BASE_ADDRESS
#define	REG_B2_RX_PP_SW_INST_CTL                     (B2_RX_PP_BASE_ADDRESS + 0x0)
#define	REG_B2_RX_PP_SW_INST_ADDR                    (B2_RX_PP_BASE_ADDRESS + 0x4)
#define	REG_B2_RX_PP_SW_HALT                         (B2_RX_PP_BASE_ADDRESS + 0x8)
#define	REG_B2_RX_PP_MPDU_IN_LIST                    (B2_RX_PP_BASE_ADDRESS + 0xC)
#define	REG_B2_RX_PP_ERR_LIST                        (B2_RX_PP_BASE_ADDRESS + 0x10)
#define	REG_B2_RX_PP_UMAC_DONE_LIST                  (B2_RX_PP_BASE_ADDRESS + 0x14)
#define	REG_B2_RX_PP_LIBERATOR_LIST                  (B2_RX_PP_BASE_ADDRESS + 0x18)
#define	REG_B2_RX_PP_RD_TYPE0_FORWARD_LIST           (B2_RX_PP_BASE_ADDRESS + 0x1C)
#define	REG_B2_RX_PP_RD_TYPE1_FORWARD_LIST           (B2_RX_PP_BASE_ADDRESS + 0x20)
#define	REG_B2_RX_PP_RD_TYPE2_FORWARD_LIST           (B2_RX_PP_BASE_ADDRESS + 0x24)
#define	REG_B2_RX_PP_RD_TYPE3_FORWARD_LIST           (B2_RX_PP_BASE_ADDRESS + 0x28)
#define	REG_B2_RX_PP_RD_TYPE4_FORWARD_LIST           (B2_RX_PP_BASE_ADDRESS + 0x2C)
#define	REG_B2_RX_PP_RD_TYPE5_FORWARD_LIST           (B2_RX_PP_BASE_ADDRESS + 0x30)
#define	REG_B2_RX_PP_RD_TYPE6_FORWARD_LIST           (B2_RX_PP_BASE_ADDRESS + 0x34)
#define	REG_B2_RX_PP_RD_TYPE7_FORWARD_LIST           (B2_RX_PP_BASE_ADDRESS + 0x38)
#define	REG_B2_RX_PP_RD_TYPE8_FORWARD_LIST           (B2_RX_PP_BASE_ADDRESS + 0x3C)
#define	REG_B2_RX_PP_RD_TYPE9_FORWARD_LIST           (B2_RX_PP_BASE_ADDRESS + 0x40)
#define	REG_B2_RX_PP_RD_TYPE10_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x44)
#define	REG_B2_RX_PP_RD_TYPE11_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x48)
#define	REG_B2_RX_PP_RD_TYPE12_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x4C)
#define	REG_B2_RX_PP_RD_TYPE13_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x50)
#define	REG_B2_RX_PP_RD_TYPE14_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x54)
#define	REG_B2_RX_PP_RD_TYPE15_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x58)
#define	REG_B2_RX_PP_RD_TYPE16_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x5C)
#define	REG_B2_RX_PP_RD_TYPE17_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x60)
#define	REG_B2_RX_PP_RD_TYPE18_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x64)
#define	REG_B2_RX_PP_RD_TYPE19_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x68)
#define	REG_B2_RX_PP_RD_TYPE20_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x6C)
#define	REG_B2_RX_PP_RD_TYPE21_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x70)
#define	REG_B2_RX_PP_RD_TYPE22_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x74)
#define	REG_B2_RX_PP_RD_TYPE23_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x78)
#define	REG_B2_RX_PP_RD_TYPE24_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x7C)
#define	REG_B2_RX_PP_RD_TYPE25_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x80)
#define	REG_B2_RX_PP_RD_TYPE26_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x84)
#define	REG_B2_RX_PP_RD_TYPE27_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x88)
#define	REG_B2_RX_PP_RD_TYPE28_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x8C)
#define	REG_B2_RX_PP_RD_TYPE29_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x90)
#define	REG_B2_RX_PP_RD_TYPE30_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x94)
#define	REG_B2_RX_PP_RD_TYPE31_FORWARD_LIST          (B2_RX_PP_BASE_ADDRESS + 0x98)
#define	REG_B2_RX_PP_RD_TYPE_SKIP_CFG                (B2_RX_PP_BASE_ADDRESS + 0xA0)
#define	REG_B2_RX_PP_RD_TYPE_SEC_SKIP_CFG            (B2_RX_PP_BASE_ADDRESS + 0xA4)
#define	REG_B2_RX_PP_RD_TYPE_UMAC_CFG                (B2_RX_PP_BASE_ADDRESS + 0xA8)
#define	REG_B2_RX_PP_SHRAM_FIFO_CFG                  (B2_RX_PP_BASE_ADDRESS + 0xAC)
#define	REG_B2_RX_PP_SHRAM_FIFO_BASE_ADD             (B2_RX_PP_BASE_ADDRESS + 0xB0)
#define	REG_B2_RX_PP_SHRAM_FIFO_DEPTH_MINUS_ONE      (B2_RX_PP_BASE_ADDRESS + 0xB4)
#define	REG_B2_RX_PP_SHRAM_FIFO_CLEAR_STRB           (B2_RX_PP_BASE_ADDRESS + 0xB8)
#define	REG_B2_RX_PP_SHRAM_FIFO_RD_ENTRIES_NUM       (B2_RX_PP_BASE_ADDRESS + 0xBC)
#define	REG_B2_RX_PP_SHRAM_FIFO_NUM_ENTRIES_COUNT    (B2_RX_PP_BASE_ADDRESS + 0xC0)
#define	REG_B2_RX_PP_SHRAM_FIFO_DEBUG                (B2_RX_PP_BASE_ADDRESS + 0xC4)
#define	REG_B2_RX_PP_SYN_FIFO_STATUS                 (B2_RX_PP_BASE_ADDRESS + 0xC8)
#define	REG_B2_RX_PP_CONTROL_BACKDOOR_UPDATE         (B2_RX_PP_BASE_ADDRESS + 0xCC)
#define	REG_B2_RX_PP_RX_PP_SPARE                     (B2_RX_PP_BASE_ADDRESS + 0xD0)
#define	REG_B2_RX_PP_RX_COUNTERS_CFG                 (B2_RX_PP_BASE_ADDRESS + 0xD4)
#define	REG_B2_RX_PP_RX_LOGGER_CFG                   (B2_RX_PP_BASE_ADDRESS + 0xD8)
#define	REG_B2_RX_PP_RX_LOGGER_BUSY                  (B2_RX_PP_BASE_ADDRESS + 0xDC)
#define	REG_B2_RX_PP_SW_REKEY_EN                     (B2_RX_PP_BASE_ADDRESS + 0xE0)
#define	REG_B2_RX_PP_SW_REKEY_INST_LSB               (B2_RX_PP_BASE_ADDRESS + 0xE4)
#define	REG_B2_RX_PP_SW_REKEY_INST_MSB               (B2_RX_PP_BASE_ADDRESS + 0xE8)
#define	REG_B2_RX_PP_FRAG_DIS_LIB_ERR_LIST           (B2_RX_PP_BASE_ADDRESS + 0xEC)
#define	REG_B2_RX_PP_SM_STATUS_PART1                 (B2_RX_PP_BASE_ADDRESS + 0xF0)
#define	REG_B2_RX_PP_SM_STATUS_PART2                 (B2_RX_PP_BASE_ADDRESS + 0xF4)
#define	REG_B2_RX_PP_UC_MGMT_TID_CFG                 (B2_RX_PP_BASE_ADDRESS + 0xF8)
#define	REG_B2_RX_PP_MC_CFG                          (B2_RX_PP_BASE_ADDRESS + 0xFC)
#define	REG_B2_RX_PP_RX_PP_INT_ERROR_STATUS          (B2_RX_PP_BASE_ADDRESS + 0x100)
#define	REG_B2_RX_PP_RX_PP_INT_ERROR_EN              (B2_RX_PP_BASE_ADDRESS + 0x104)
#define	REG_B2_RX_PP_RX_PP_INT_ERROR_CLEAR           (B2_RX_PP_BASE_ADDRESS + 0x108)
#define	REG_B2_RX_PP_RX_PP_CONTROL_BITS              (B2_RX_PP_BASE_ADDRESS + 0x10C)
#define	REG_B2_RX_PP_RXDD_SW_UPDATE_LAST_SN          (B2_RX_PP_BASE_ADDRESS + 0x110)
#define	REG_B2_RX_PP_RXDD_SW_UPDATE_DUP_ENABLE       (B2_RX_PP_BASE_ADDRESS + 0x114)
#define	REG_B2_RX_PP_RXDD_SW_UPDATE_SN_VALID         (B2_RX_PP_BASE_ADDRESS + 0x118)
#define	REG_B2_RX_PP_RXDD_SW_RD_DUP_DB               (B2_RX_PP_BASE_ADDRESS + 0x11C)
#define	REG_B2_RX_PP_RXDD_SN_0_INFO_DBG              (B2_RX_PP_BASE_ADDRESS + 0x120)
#define	REG_B2_RX_PP_RXDD_SN_1_INFO_DBG              (B2_RX_PP_BASE_ADDRESS + 0x124)
#define	REG_B2_RX_PP_RXDD_SN_2_INFO_DBG              (B2_RX_PP_BASE_ADDRESS + 0x128)
#define	REG_B2_RX_PP_RXDD_SN_3_INFO_DBG              (B2_RX_PP_BASE_ADDRESS + 0x12C)
#define	REG_B2_RX_PP_RXDD_SN_4_INFO_DBG              (B2_RX_PP_BASE_ADDRESS + 0x130)
#define	REG_B2_RX_PP_RXDD_SN_5_INFO_DBG              (B2_RX_PP_BASE_ADDRESS + 0x134)
#define	REG_B2_RX_PP_RXDD_SN_6_INFO_DBG              (B2_RX_PP_BASE_ADDRESS + 0x138)
#define	REG_B2_RX_PP_RXDD_SN_7_INFO_DBG              (B2_RX_PP_BASE_ADDRESS + 0x13C)
#define	REG_B2_RX_PP_MC_DATA_CFG                     (B2_RX_PP_BASE_ADDRESS + 0x140)
#define	REG_B2_RX_PP_GADF_VAP_IDX_OVERWRITE_B0       (B2_RX_PP_BASE_ADDRESS + 0x144)
#define	REG_B2_RX_PP_GADF_VAP_IDX_OVERWRITE_B1       (B2_RX_PP_BASE_ADDRESS + 0x148)
#define	REG_B2_RX_PP_GADF_VAP_IDX_OVERWRITE_B2       (B2_RX_PP_BASE_ADDRESS + 0x14C)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_RX_PP_SW_INST_CTL 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 baWindowSize : 9; //BA window size, reset value: 0x0, access type: RW
		uint32 statusBitValue : 1; //no description, reset value: 0x0, access type: RW
		uint32 fragEnValue : 1; //no description, reset value: 0x0, access type: RW
		uint32 fragEnUpdate : 1; //Frag_en bit update. Relevant for BAA only, reset value: 0x0, access type: RW
		uint32 newSn : 12; //New SN value. Valid when sn_update is asserted, reset value: 0x0, access type: RW
		uint32 clearDest : 1; //Deciding where to send SNs in list when clearing it , reset value: 0x0, access type: RW
		uint32 noBaForce : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 mulTids : 1; //Indication that a SW command shall be applied to multiple TIDs (TID list is given in mul_tids_en field). This field can be set only in case of FLUSH and FRAG_EN commands. It is expected to be 0 for all other commands, reset value: 0x0, access type: RW
		uint32 snUpdate : 1; //New SN update, reset value: 0x0, access type: RW
		uint32 winSizeUpdate : 1; //Window size update, reset value: 0x0, access type: RW
		uint32 flush : 1; //Flushing all SNs from list, reset value: 0x0, access type: RW
		uint32 statusBitUpdate : 1; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB2RxPpSwInstCtl_u;

/*REG_B2_RX_PP_SW_INST_ADDR 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tid : 4; //TID address. Valid values are 0-8, reset value: 0x0, access type: RW
		uint32 sta : 8; //STA address. Valid values are 0-127, reset value: 0x0, access type: RW
		uint32 swInstDone : 1; //no description, reset value: 0x0, access type: RO
		uint32 mcVapAccess : 1; //no description, reset value: 0x0, access type: RW
		uint32 mulTidsEn : 9; //TID list to be used in case mul_tids field is set. Relevant only in case of FLUSH and FRAG_EN commands, reset value: 0x0, access type: RW
		uint32 mulTidsFragEnValue : 9; //frag_en value per TID in case of FRAG_EN commands, reset value: 0x0, access type: RW
	} bitFields;
} RegB2RxPpSwInstAddr_u;

/*REG_B2_RX_PP_SW_HALT 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swHalt : 1; //Halts the block from moving to the next MPDU, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxPpSwHalt_u;

/*REG_B2_RX_PP_MPDU_IN_LIST 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mpduInListIdx : 6; //MPDU list index number at the RX lists DLM, reset value: 0x11, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB2RxPpMpduInList_u;

/*REG_B2_RX_PP_ERR_LIST 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errListIdx : 7; //Host IF list index number at the RX lists DLM. MSBit serves as destination ID, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpErrList_u;

/*REG_B2_RX_PP_UMAC_DONE_LIST 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 umacDoneListIdx : 7; //UMAC done list index number at the RX lists DLM. MSBit serves as destination ID, reset value: 0x12, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpUmacDoneList_u;

/*REG_B2_RX_PP_LIBERATOR_LIST 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 liberatorListIdx : 7; //Drop src0 Host IF list index number at the RX lists DLM. MSBit serves as destination ID, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpLiberatorList_u;

/*REG_B2_RX_PP_RD_TYPE0_FORWARD_LIST 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType0ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType0ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE1_FORWARD_LIST 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType1ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType1ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE2_FORWARD_LIST 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType2ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType2ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE3_FORWARD_LIST 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType3ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType3ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE4_FORWARD_LIST 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType4ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType4ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE5_FORWARD_LIST 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType5ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType5ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE6_FORWARD_LIST 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType6ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType6ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE7_FORWARD_LIST 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType7ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType7ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE8_FORWARD_LIST 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType8ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType8ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE9_FORWARD_LIST 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType9ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType9ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE10_FORWARD_LIST 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType10ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType10ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE11_FORWARD_LIST 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType11ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType11ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE12_FORWARD_LIST 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType12ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType12ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE13_FORWARD_LIST 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType13ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType13ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE14_FORWARD_LIST 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType14ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType14ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE15_FORWARD_LIST 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType15ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType15ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE16_FORWARD_LIST 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType16ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType16ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE17_FORWARD_LIST 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType17ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType17ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE18_FORWARD_LIST 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType18ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType18ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE19_FORWARD_LIST 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType19ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType19ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE20_FORWARD_LIST 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType20ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType20ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE21_FORWARD_LIST 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType21ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType21ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE22_FORWARD_LIST 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType22ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType22ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE23_FORWARD_LIST 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType23ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType23ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE24_FORWARD_LIST 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType24ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType24ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE25_FORWARD_LIST 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType25ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType25ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE26_FORWARD_LIST 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType26ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType26ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE27_FORWARD_LIST 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType27ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType27ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE28_FORWARD_LIST 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType28ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType28ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE29_FORWARD_LIST 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType29ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType29ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE30_FORWARD_LIST 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType30ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType30ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE31_FORWARD_LIST 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdType31ListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpRdType31ForwardList_u;

/*REG_B2_RX_PP_RD_TYPE_SKIP_CFG 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 skipBit : 32; //no description, reset value: 0xFFFFFFFF, access type: RW
	} bitFields;
} RegB2RxPpRdTypeSkipCfg_u;

/*REG_B2_RX_PP_RD_TYPE_SEC_SKIP_CFG 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 secSkipBit : 32; //no description, reset value: 0xFFFFFFFF, access type: RW
	} bitFields;
} RegB2RxPpRdTypeSecSkipCfg_u;

/*REG_B2_RX_PP_RD_TYPE_UMAC_CFG 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 umacBit : 32; //no description, reset value: 0xFFFFFFFF, access type: RW
	} bitFields;
} RegB2RxPpRdTypeUmacCfg_u;

/*REG_B2_RX_PP_SHRAM_FIFO_CFG 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramFifoEn : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxPpShramFifoCfg_u;

/*REG_B2_RX_PP_SHRAM_FIFO_BASE_ADD 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramFifoBaseAddr : 22; //SHRAM Frag FIFO base address - Byte aligned, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB2RxPpShramFifoBaseAdd_u;

/*REG_B2_RX_PP_SHRAM_FIFO_DEPTH_MINUS_ONE 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramFifoDepthMinusOne : 10; //no description, reset value: 0x100, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2RxPpShramFifoDepthMinusOne_u;

/*REG_B2_RX_PP_SHRAM_FIFO_CLEAR_STRB 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramFifoClearStrb : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxPpShramFifoClearStrb_u;

/*REG_B2_RX_PP_SHRAM_FIFO_RD_ENTRIES_NUM 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramFifoRdEntriesNum : 11; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 21;
	} bitFields;
} RegB2RxPpShramFifoRdEntriesNum_u;

/*REG_B2_RX_PP_SHRAM_FIFO_NUM_ENTRIES_COUNT 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramFifoNumEntriesCount : 11; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB2RxPpShramFifoNumEntriesCount_u;

/*REG_B2_RX_PP_SHRAM_FIFO_DEBUG 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramFifoWrPtr : 10; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
		uint32 shramFifoNotEmpty : 1; //no description, reset value: 0x0, access type: RO
		uint32 shramFifoFull : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 6;
		uint32 shramFifoFullDropCtr : 8; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2RxPpShramFifoDebug_u;

/*REG_B2_RX_PP_SYN_FIFO_STATUS 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramSynFifoBytesInFifo : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 shramSynFifoFull : 1; //no description, reset value: 0x0, access type: RO
		uint32 shramSynFifoEmpty : 1; //no description, reset value: 0x0, access type: RO
		uint32 shramSynFifoPopWhileEmpty : 1; //no description, reset value: 0x0, access type: RO
		uint32 shramSynFifoPushWhileFull : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 24;
	} bitFields;
} RegB2RxPpSynFifoStatus_u;

/*REG_B2_RX_PP_CONTROL_BACKDOOR_UPDATE 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 controlSnNumValid : 1; //SN num valid bit value, reset value: 0x0, access type: RW
		uint32 controlPnNumValid : 1; //PN num valid bit value, reset value: 0x0, access type: RW
		uint32 controlFragEn : 1; //Frag_en bit. Used in BAA only, reset value: 0x0, access type: RW
		uint32 controlHighestSnNumValid : 1; //Highest SN in window valid, reset value: 0x0, access type: RW
		uint32 controlBe : 4; //Valid bits be, reset value: 0x0, access type: RW
		uint32 controlUpdateDone : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegB2RxPpControlBackdoorUpdate_u;

/*REG_B2_RX_PP_RX_PP_SPARE 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxPpSpare : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB2RxPpRxPpSpare_u;

/*REG_B2_RX_PP_RX_COUNTERS_CFG 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qosRxEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 rdCountEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 rdDelayedCountEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 rdSwDropCountEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 rdDuplicateDropCountEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 missingSnSwUpdateCountEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 mpduUniOrMgmtEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 mpduRetryEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 amsduCountEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 dropMpduEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 mpduTypeNotSupportedEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 barMpduCountEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 replayEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 replayMgmtEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 tkipCountEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 failureCountEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 rxClassAmsduBytesStbEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 rxcAmpduStbEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 rxClassMpduInAmpduStbEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 rxClassOctetMpduStbEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 rxClassDropStbEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 rxClassSecurMisStbEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 rxcCrcErrorStbEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 delineatorCrcErrorStbEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 bcMcCountEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 qosStaTidRxEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 qosRxDecEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 rxCountCleanFifo : 1; //no description, reset value: 0x0, access type: WO
	} bitFields;
} RegB2RxPpRxCountersCfg_u;

/*REG_B2_RX_PP_RX_LOGGER_CFG 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxPpLoggerPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 rxPpLoggerEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2RxPpRxLoggerCfg_u;

/*REG_B2_RX_PP_RX_LOGGER_BUSY 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxPpLoggerBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxPpRxLoggerBusy_u;

/*REG_B2_RX_PP_SW_REKEY_EN 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rekeyEn : 1; //ReKey feature enable, reset value: 0x0, access type: RW
		uint32 keyIdCheckEn : 1; //Enable KEY_ID check. When enabled, every incoming MPDU is checked for correct key, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB2RxPpSwRekeyEn_u;

/*REG_B2_RX_PP_SW_REKEY_INST_LSB 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rekeySta : 8; //STA/VAP ID parameter for Rekey command. When rekey_vap_access is set, this field is interpreted as VAP ID. Otherwise, it is STA_ID parameter., reset value: 0x0, access type: RW
		uint32 rekeyVapAccess : 1; //Rekey command parameter. Designates whether the command is VAP access command  (MC) or STA command, reset value: 0x0, access type: RW
		uint32 rekeyDataUpdateOnly : 1; //Rekey command parameter. When set, the Rekey commnad shall only update the contents of the STA bitmap RAM (rekey_pn, key, mgmt_tid_en, old_key), without waiting for Rekey HIT (i.e. rekey_set shall not be set), reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 rekeyKey : 2; //KEY_ID parameter for the Rekey command, reset value: 0x0, access type: RW
		uint32 rekeyMgmtTidEn : 1; //Rekey command parameter. Designates whether the command includes mgmt_tid (TID 8). In case rekey_vap_access is set, this field is don---t  care, reset value: 0x0, access type: RW
		uint32 rekeyDone : 1; //Done indication of ReKey sequence activation., reset value: 0x0, access type: RO
		uint32 rekeyPn015 : 16; //LSB ReKey PN parameter for the Rekey command, reset value: 0x0, access type: RW
	} bitFields;
} RegB2RxPpSwRekeyInstLsb_u;

/*REG_B2_RX_PP_SW_REKEY_INST_MSB 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rekeyPn1647 : 32; //MSB ReKey PN parameter for the Rekey command, reset value: 0x0, access type: RW
	} bitFields;
} RegB2RxPpSwRekeyInstMsb_u;

/*REG_B2_RX_PP_FRAG_DIS_LIB_ERR_LIST 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fragDisLibErrList : 1; //When fragment is disabled for the STA-TID it is dropped to: , 0x0: Liberator , 0x1: Error list, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxPpFragDisLibErrList_u;

/*REG_B2_RX_PP_SM_STATUS_PART1 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clearMpduSm : 6; //no description, reset value: 0x0, access type: RO
		uint32 oooDigMpduSm : 5; //no description, reset value: 0x0, access type: RO
		uint32 ssnClearMpduClearSm : 5; //no description, reset value: 0x0, access type: RO
		uint32 impMpduClearSm : 3; //no description, reset value: 0x0, access type: RO
		uint32 noBaaClearSm : 5; //no description, reset value: 0x0, access type: RO
		uint32 flushSm : 4; //no description, reset value: 0x0, access type: RO
		uint32 rekeyHitSm : 4; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2RxPpSmStatusPart1_u;

/*REG_B2_RX_PP_SM_STATUS_PART2 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 statusUpdateSm : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 setRekeySm : 3; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 fragEnUpdateSm : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 mainMpduSm : 5; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 3;
		uint32 snUpdateSm : 4; //no description, reset value: 0x0, access type: RO
		uint32 loggerSm : 3; //no description, reset value: 0x0, access type: RO
		uint32 reserved4 : 1;
		uint32 windowSizeSm : 3; //no description, reset value: 0x0, access type: RO
		uint32 shramFifoSm : 1; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB2RxPpSmStatusPart2_u;

/*REG_B2_RX_PP_UC_MGMT_TID_CFG 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ucMgmtTidCfg : 32; //Indication which RD Type is diverted to queue 8 (Management)., reset value: 0x7FF9FFF6, access type: RW
	} bitFields;
} RegB2RxPpUcMgmtTidCfg_u;

/*REG_B2_RX_PP_MC_CFG 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mcCfg : 32; //Indication which RD Type is defined as MC., reset value: 0x10104, access type: RW
	} bitFields;
} RegB2RxPpMcCfg_u;

/*REG_B2_RX_PP_RX_PP_INT_ERROR_STATUS 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramSynFifoOverflowStatus : 1; //SHRAM SYN FIFO overflow (push while full) event, reset value: 0x0, access type: RO
		uint32 shramFifoOverflow : 1; //SHRAM FIFO dropped entries event, reset value: 0x0, access type: RO
		uint32 shramFifoDecrementLessThanZero : 1; //SHRAM FIFO decrement less than zero event. Number of entries of SHRAM FIFO  has been decremented to less than zero, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2RxPpRxPpIntErrorStatus_u;

/*REG_B2_RX_PP_RX_PP_INT_ERROR_EN 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramSynFifoOverflowEn : 1; //SHRAM SYN FIFO overflow (push while full) interrupt enable, reset value: 0x1, access type: RW
		uint32 shramFifoFullDropCtrEn : 1; //SHRAM FIFO dropped entries interrupt enable, reset value: 0x1, access type: RW
		uint32 shramFifoDecLessThanZeroEn : 1; //SHRAM FIFO decrement less than zero interrupt enable, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2RxPpRxPpIntErrorEn_u;

/*REG_B2_RX_PP_RX_PP_INT_ERROR_CLEAR 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shramSynFifoClearOverflowStrb : 1; //SHRAM SYN FIFO overflow (push while full) interrupt clear, reset value: 0x0, access type: WO
		uint32 shramFifoClearFullDropCtrStrb : 1; //SHRAM FIFO dropped entries interrupt clear. Writing '1' to this field shall also clear the value of the SHRAM FIFO drop counter, reset value: 0x0, access type: WO
		uint32 shramFifoClearDecLessThanZeroStrb : 1; //SHRAM FIFO decrement less than zero interrupt clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2RxPpRxPpIntErrorClear_u;

/*REG_B2_RX_PP_RX_PP_CONTROL_BITS 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 trafficIndicatorEn : 1; //When this bit is set, traffic indication is enabled. Otherwise, it is disabled, reset value: 0x1, access type: RW
		uint32 rxPpGadfMldEnable : 1; //When this bit is set, GADF Duplicate Detection in MLD feature is enabled. When this bit is inactive, RX Dup Detect module is not accessed, reset value: 0x1, access type: RW
		uint32 rxPpGadfUmacPrecedesDupDetect : 1; //When this bit is set, in case the conditions for GADF Duplicate Detection apply, checking whether RD is destined to UMAC (based on status bit + umac_bit[rd_type]) shall be done before checking duplication (by activating RX_DUP_DETECT). When this bit is disabled, an RD which is destined to UMAC and is a GADF MLD duplicate, shall be dropped without raching to UMAC. This bit is relavant only in case rx_pp_gadf_mld_enable is enabled, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2RxPpRxPpControlBits_u;

/*REG_B2_RX_PP_RXDD_SW_UPDATE_LAST_SN 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swUpdateLastSn : 12; //Last SN value to write to Rx-Dup Detect DB, reset value: 0x0, access type: RW
		uint32 reserved0 : 20;
	} bitFields;
} RegB2RxPpRxddSwUpdateLastSn_u;

/*REG_B2_RX_PP_RXDD_SW_UPDATE_DUP_ENABLE 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swUpdateDupEnable : 1; //Dup Enable value to write to Rx-Dup Detect DB, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxPpRxddSwUpdateDupEnable_u;

/*REG_B2_RX_PP_RXDD_SW_UPDATE_SN_VALID 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swUpdateSnValid : 1; //SN Valid value to write to Rx-Dup Detect DB, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxPpRxddSwUpdateSnValid_u;

/*REG_B2_RX_PP_RXDD_SW_RD_DUP_DB 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swRdLastSn : 12; //Current Last SN value in the Rx-Dup Detect DB, reset value: 0x0, access type: RO
		uint32 swRdSnValid : 1; //Current SN Valid value in the Rx-Dup Detect DB, reset value: 0x0, access type: RO
		uint32 swRdDupEnable : 1; //Current Dup Enable value in the Rx-Dup Detect DB, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB2RxPpRxddSwRdDupDb_u;

/*REG_B2_RX_PP_RXDD_SN_0_INFO_DBG 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queriedSn0 : 12; //SN for which a query was generated, reset value: 0x0, access type: RO
		uint32 lastSn0 : 12; //Last SN within the DB at the time of the query, reset value: 0x0, access type: RO
		uint32 dropResult0 : 1; //Drop result of the query, reset value: 0x0, access type: RO
		uint32 rxPpId0 : 1; //designates the RX_PP which made the query.  0 - RX_PP01; 1 - RX_PP2, reset value: 0x0, access type: RO
		uint32 ptrVal0 : 4; //Rolling Write Pointer, reset value: 0x0, access type: RO
		uint32 valid0 : 1; //designates the validity of this entry, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB2RxPpRxddSn0InfoDbg_u;

/*REG_B2_RX_PP_RXDD_SN_1_INFO_DBG 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queriedSn1 : 12; //SN for which a query was generated, reset value: 0x0, access type: RO
		uint32 lastSn1 : 12; //Last SN within the DB at the time of the query, reset value: 0x0, access type: RO
		uint32 dropResult1 : 1; //Drop result of the query, reset value: 0x0, access type: RO
		uint32 rxPpId1 : 1; //designates the RX_PP which made the query.  0 - RX_PP01; 1 - RX_PP2, reset value: 0x0, access type: RO
		uint32 ptrVal1 : 4; //Rolling Write Pointer, reset value: 0x0, access type: RO
		uint32 valid1 : 1; //designates the validity of this entry, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB2RxPpRxddSn1InfoDbg_u;

/*REG_B2_RX_PP_RXDD_SN_2_INFO_DBG 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queriedSn2 : 12; //SN for which a query was generated, reset value: 0x0, access type: RO
		uint32 lastSn2 : 12; //Last SN within the DB at the time of the query, reset value: 0x0, access type: RO
		uint32 dropResult2 : 1; //Drop result of the query, reset value: 0x0, access type: RO
		uint32 rxPpId2 : 1; //designates the RX_PP which made the query.  0 - RX_PP01; 1 - RX_PP2, reset value: 0x0, access type: RO
		uint32 ptrVal2 : 4; //Rolling Write Pointer, reset value: 0x0, access type: RO
		uint32 valid2 : 1; //designates the validity of this entry, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB2RxPpRxddSn2InfoDbg_u;

/*REG_B2_RX_PP_RXDD_SN_3_INFO_DBG 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queriedSn3 : 12; //SN for which a query was generated, reset value: 0x0, access type: RO
		uint32 lastSn3 : 12; //Last SN within the DB at the time of the query, reset value: 0x0, access type: RO
		uint32 dropResult3 : 1; //Drop result of the query, reset value: 0x0, access type: RO
		uint32 rxPpId3 : 1; //designates the RX_PP which made the query.  0 - RX_PP01; 1 - RX_PP2, reset value: 0x0, access type: RO
		uint32 ptrVal3 : 4; //Rolling Write Pointer, reset value: 0x0, access type: RO
		uint32 valid3 : 1; //designates the validity of this entry, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB2RxPpRxddSn3InfoDbg_u;

/*REG_B2_RX_PP_RXDD_SN_4_INFO_DBG 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queriedSn4 : 12; //SN for which a query was generated, reset value: 0x0, access type: RO
		uint32 lastSn4 : 12; //Last SN within the DB at the time of the query, reset value: 0x0, access type: RO
		uint32 dropResult4 : 1; //Drop result of the query, reset value: 0x0, access type: RO
		uint32 rxPpId4 : 1; //designates the RX_PP which made the query.  0 - RX_PP01; 1 - RX_PP2, reset value: 0x0, access type: RO
		uint32 ptrVal4 : 4; //Rolling Write Pointer, reset value: 0x0, access type: RO
		uint32 valid4 : 1; //designates the validity of this entry, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB2RxPpRxddSn4InfoDbg_u;

/*REG_B2_RX_PP_RXDD_SN_5_INFO_DBG 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queriedSn5 : 12; //SN for which a query was generated, reset value: 0x0, access type: RO
		uint32 lastSn5 : 12; //Last SN within the DB at the time of the query, reset value: 0x0, access type: RO
		uint32 dropResult5 : 1; //Drop result of the query, reset value: 0x0, access type: RO
		uint32 rxPpId5 : 1; //designates the RX_PP which made the query.  0 - RX_PP01; 1 - RX_PP2, reset value: 0x0, access type: RO
		uint32 ptrVal5 : 4; //Rolling Write Pointer, reset value: 0x0, access type: RO
		uint32 valid5 : 1; //designates the validity of this entry, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB2RxPpRxddSn5InfoDbg_u;

/*REG_B2_RX_PP_RXDD_SN_6_INFO_DBG 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queriedSn6 : 12; //SN for which a query was generated, reset value: 0x0, access type: RO
		uint32 lastSn6 : 12; //Last SN within the DB at the time of the query, reset value: 0x0, access type: RO
		uint32 dropResult6 : 1; //Drop result of the query, reset value: 0x0, access type: RO
		uint32 rxPpId6 : 1; //designates the RX_PP which made the query.  0 - RX_PP01; 1 - RX_PP2, reset value: 0x0, access type: RO
		uint32 ptrVal6 : 4; //Rolling Write Pointer, reset value: 0x0, access type: RO
		uint32 valid6 : 1; //designates the validity of this entry, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB2RxPpRxddSn6InfoDbg_u;

/*REG_B2_RX_PP_RXDD_SN_7_INFO_DBG 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queriedSn7 : 12; //SN for which a query was generated, reset value: 0x0, access type: RO
		uint32 lastSn7 : 12; //Last SN within the DB at the time of the query, reset value: 0x0, access type: RO
		uint32 dropResult7 : 1; //Drop result of the query, reset value: 0x0, access type: RO
		uint32 rxPpId7 : 1; //designates the RX_PP which made the query.  0 - RX_PP01; 1 - RX_PP2, reset value: 0x0, access type: RO
		uint32 ptrVal7 : 4; //Rolling Write Pointer, reset value: 0x0, access type: RO
		uint32 valid7 : 1; //designates the validity of this entry, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegB2RxPpRxddSn7InfoDbg_u;

/*REG_B2_RX_PP_MC_DATA_CFG 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mcDataCfg : 32; //Indication which RD Type is defined as MC Data. Used only for Duplicate detection in case of MLD, reset value: 0x4, access type: RW
	} bitFields;
} RegB2RxPpMcDataCfg_u;

/*REG_B2_RX_PP_GADF_VAP_IDX_OVERWRITE_B0 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gadfNetIfcIdxB0 : 7; //GADF net_ifc_idx of band 0. In case of receiving GADF, RxPP invokes the DupDetect and if the result is not drop, it will overwrite net_ifc_idx field (within the RD) with this register value (in case of band 0), thus setting the net_ifc_idx to the NI index that corresponds to the MLD, in case of MLD., reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpGadfVapIdxOverwriteB0_u;

/*REG_B2_RX_PP_GADF_VAP_IDX_OVERWRITE_B1 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gadfNetIfcIdxB1 : 7; //GADF net_ifc_idx of band 1. In case of receiving GADF, RxPP invokes the DupDetect and if the result is not drop, it will overwrite net_ifc_idx field (within the RD) with this register value (in case of band 1), thus setting the net_ifc_idx to the NI index that corresponds to the MLD, in case of MLD., reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpGadfVapIdxOverwriteB1_u;

/*REG_B2_RX_PP_GADF_VAP_IDX_OVERWRITE_B2 0x14C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gadfNetIfcIdxB2 : 7; //GADF net_ifc_idx of band 2. In case of receiving GADF, RxPP invokes the DupDetect and if the result is not drop, it will overwrite net_ifc_idx field (within the RD) with this register value (in case of band 2), thus setting the net_ifc_idx to the NI index that corresponds to the MLD, in case of MLD., reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2RxPpGadfVapIdxOverwriteB2_u;

//========================================
/* REG_RX_PP_SW_INST_CTL 0x0 */
#define B2_RX_PP_SW_INST_CTL_FIELD_BA_WINDOW_SIZE_MASK                                                            0x000001FF
#define B2_RX_PP_SW_INST_CTL_FIELD_STATUS_BIT_VALUE_MASK                                                          0x00000200
#define B2_RX_PP_SW_INST_CTL_FIELD_FRAG_EN_VALUE_MASK                                                             0x00000400
#define B2_RX_PP_SW_INST_CTL_FIELD_FRAG_EN_UPDATE_MASK                                                            0x00000800
#define B2_RX_PP_SW_INST_CTL_FIELD_NEW_SN_MASK                                                                    0x00FFF000
#define B2_RX_PP_SW_INST_CTL_FIELD_CLEAR_DEST_MASK                                                                0x01000000
#define B2_RX_PP_SW_INST_CTL_FIELD_NO_BA_FORCE_MASK                                                               0x02000000
#define B2_RX_PP_SW_INST_CTL_FIELD_MUL_TIDS_MASK                                                                  0x08000000
#define B2_RX_PP_SW_INST_CTL_FIELD_SN_UPDATE_MASK                                                                 0x10000000
#define B2_RX_PP_SW_INST_CTL_FIELD_WIN_SIZE_UPDATE_MASK                                                           0x20000000
#define B2_RX_PP_SW_INST_CTL_FIELD_FLUSH_MASK                                                                     0x40000000
#define B2_RX_PP_SW_INST_CTL_FIELD_STATUS_BIT_UPDATE_MASK                                                         0x80000000
//========================================
/* REG_RX_PP_SW_INST_ADDR 0x4 */
#define B2_RX_PP_SW_INST_ADDR_FIELD_TID_MASK                                                                      0x0000000F
#define B2_RX_PP_SW_INST_ADDR_FIELD_STA_MASK                                                                      0x00000FF0
#define B2_RX_PP_SW_INST_ADDR_FIELD_SW_INST_DONE_MASK                                                             0x00001000
#define B2_RX_PP_SW_INST_ADDR_FIELD_MC_VAP_ACCESS_MASK                                                            0x00002000
#define B2_RX_PP_SW_INST_ADDR_FIELD_MUL_TIDS_EN_MASK                                                              0x007FC000
#define B2_RX_PP_SW_INST_ADDR_FIELD_MUL_TIDS_FRAG_EN_VALUE_MASK                                                   0xFF800000
//========================================
/* REG_RX_PP_SW_HALT 0x8 */
#define B2_RX_PP_SW_HALT_FIELD_SW_HALT_MASK                                                                       0x00000001
//========================================
/* REG_RX_PP_MPDU_IN_LIST 0xC */
#define B2_RX_PP_MPDU_IN_LIST_FIELD_MPDU_IN_LIST_IDX_MASK                                                         0x0000003F
//========================================
/* REG_RX_PP_ERR_LIST 0x10 */
#define B2_RX_PP_ERR_LIST_FIELD_ERR_LIST_IDX_MASK                                                                 0x0000007F
//========================================
/* REG_RX_PP_UMAC_DONE_LIST 0x14 */
#define B2_RX_PP_UMAC_DONE_LIST_FIELD_UMAC_DONE_LIST_IDX_MASK                                                     0x0000007F
//========================================
/* REG_RX_PP_LIBERATOR_LIST 0x18 */
#define B2_RX_PP_LIBERATOR_LIST_FIELD_LIBERATOR_LIST_IDX_MASK                                                     0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE0_FORWARD_LIST 0x1C */
#define B2_RX_PP_RD_TYPE0_FORWARD_LIST_FIELD_RD_TYPE0_LIST_IDX_MASK                                               0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE1_FORWARD_LIST 0x20 */
#define B2_RX_PP_RD_TYPE1_FORWARD_LIST_FIELD_RD_TYPE1_LIST_IDX_MASK                                               0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE2_FORWARD_LIST 0x24 */
#define B2_RX_PP_RD_TYPE2_FORWARD_LIST_FIELD_RD_TYPE2_LIST_IDX_MASK                                               0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE3_FORWARD_LIST 0x28 */
#define B2_RX_PP_RD_TYPE3_FORWARD_LIST_FIELD_RD_TYPE3_LIST_IDX_MASK                                               0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE4_FORWARD_LIST 0x2C */
#define B2_RX_PP_RD_TYPE4_FORWARD_LIST_FIELD_RD_TYPE4_LIST_IDX_MASK                                               0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE5_FORWARD_LIST 0x30 */
#define B2_RX_PP_RD_TYPE5_FORWARD_LIST_FIELD_RD_TYPE5_LIST_IDX_MASK                                               0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE6_FORWARD_LIST 0x34 */
#define B2_RX_PP_RD_TYPE6_FORWARD_LIST_FIELD_RD_TYPE6_LIST_IDX_MASK                                               0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE7_FORWARD_LIST 0x38 */
#define B2_RX_PP_RD_TYPE7_FORWARD_LIST_FIELD_RD_TYPE7_LIST_IDX_MASK                                               0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE8_FORWARD_LIST 0x3C */
#define B2_RX_PP_RD_TYPE8_FORWARD_LIST_FIELD_RD_TYPE8_LIST_IDX_MASK                                               0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE9_FORWARD_LIST 0x40 */
#define B2_RX_PP_RD_TYPE9_FORWARD_LIST_FIELD_RD_TYPE9_LIST_IDX_MASK                                               0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE10_FORWARD_LIST 0x44 */
#define B2_RX_PP_RD_TYPE10_FORWARD_LIST_FIELD_RD_TYPE10_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE11_FORWARD_LIST 0x48 */
#define B2_RX_PP_RD_TYPE11_FORWARD_LIST_FIELD_RD_TYPE11_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE12_FORWARD_LIST 0x4C */
#define B2_RX_PP_RD_TYPE12_FORWARD_LIST_FIELD_RD_TYPE12_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE13_FORWARD_LIST 0x50 */
#define B2_RX_PP_RD_TYPE13_FORWARD_LIST_FIELD_RD_TYPE13_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE14_FORWARD_LIST 0x54 */
#define B2_RX_PP_RD_TYPE14_FORWARD_LIST_FIELD_RD_TYPE14_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE15_FORWARD_LIST 0x58 */
#define B2_RX_PP_RD_TYPE15_FORWARD_LIST_FIELD_RD_TYPE15_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE16_FORWARD_LIST 0x5C */
#define B2_RX_PP_RD_TYPE16_FORWARD_LIST_FIELD_RD_TYPE16_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE17_FORWARD_LIST 0x60 */
#define B2_RX_PP_RD_TYPE17_FORWARD_LIST_FIELD_RD_TYPE17_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE18_FORWARD_LIST 0x64 */
#define B2_RX_PP_RD_TYPE18_FORWARD_LIST_FIELD_RD_TYPE18_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE19_FORWARD_LIST 0x68 */
#define B2_RX_PP_RD_TYPE19_FORWARD_LIST_FIELD_RD_TYPE19_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE20_FORWARD_LIST 0x6C */
#define B2_RX_PP_RD_TYPE20_FORWARD_LIST_FIELD_RD_TYPE20_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE21_FORWARD_LIST 0x70 */
#define B2_RX_PP_RD_TYPE21_FORWARD_LIST_FIELD_RD_TYPE21_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE22_FORWARD_LIST 0x74 */
#define B2_RX_PP_RD_TYPE22_FORWARD_LIST_FIELD_RD_TYPE22_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE23_FORWARD_LIST 0x78 */
#define B2_RX_PP_RD_TYPE23_FORWARD_LIST_FIELD_RD_TYPE23_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE24_FORWARD_LIST 0x7C */
#define B2_RX_PP_RD_TYPE24_FORWARD_LIST_FIELD_RD_TYPE24_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE25_FORWARD_LIST 0x80 */
#define B2_RX_PP_RD_TYPE25_FORWARD_LIST_FIELD_RD_TYPE25_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE26_FORWARD_LIST 0x84 */
#define B2_RX_PP_RD_TYPE26_FORWARD_LIST_FIELD_RD_TYPE26_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE27_FORWARD_LIST 0x88 */
#define B2_RX_PP_RD_TYPE27_FORWARD_LIST_FIELD_RD_TYPE27_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE28_FORWARD_LIST 0x8C */
#define B2_RX_PP_RD_TYPE28_FORWARD_LIST_FIELD_RD_TYPE28_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE29_FORWARD_LIST 0x90 */
#define B2_RX_PP_RD_TYPE29_FORWARD_LIST_FIELD_RD_TYPE29_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE30_FORWARD_LIST 0x94 */
#define B2_RX_PP_RD_TYPE30_FORWARD_LIST_FIELD_RD_TYPE30_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE31_FORWARD_LIST 0x98 */
#define B2_RX_PP_RD_TYPE31_FORWARD_LIST_FIELD_RD_TYPE31_LIST_IDX_MASK                                             0x0000007F
//========================================
/* REG_RX_PP_RD_TYPE_SKIP_CFG 0xA0 */
#define B2_RX_PP_RD_TYPE_SKIP_CFG_FIELD_SKIP_BIT_MASK                                                             0xFFFFFFFF
//========================================
/* REG_RX_PP_RD_TYPE_SEC_SKIP_CFG 0xA4 */
#define B2_RX_PP_RD_TYPE_SEC_SKIP_CFG_FIELD_SEC_SKIP_BIT_MASK                                                     0xFFFFFFFF
//========================================
/* REG_RX_PP_RD_TYPE_UMAC_CFG 0xA8 */
#define B2_RX_PP_RD_TYPE_UMAC_CFG_FIELD_UMAC_BIT_MASK                                                             0xFFFFFFFF
//========================================
/* REG_RX_PP_SHRAM_FIFO_CFG 0xAC */
#define B2_RX_PP_SHRAM_FIFO_CFG_FIELD_SHRAM_FIFO_EN_MASK                                                          0x00000001
//========================================
/* REG_RX_PP_SHRAM_FIFO_BASE_ADD 0xB0 */
#define B2_RX_PP_SHRAM_FIFO_BASE_ADD_FIELD_SHRAM_FIFO_BASE_ADDR_MASK                                              0x003FFFFF
//========================================
/* REG_RX_PP_SHRAM_FIFO_DEPTH_MINUS_ONE 0xB4 */
#define B2_RX_PP_SHRAM_FIFO_DEPTH_MINUS_ONE_FIELD_SHRAM_FIFO_DEPTH_MINUS_ONE_MASK                                 0x000003FF
//========================================
/* REG_RX_PP_SHRAM_FIFO_CLEAR_STRB 0xB8 */
#define B2_RX_PP_SHRAM_FIFO_CLEAR_STRB_FIELD_SHRAM_FIFO_CLEAR_STRB_MASK                                           0x00000001
//========================================
/* REG_RX_PP_SHRAM_FIFO_RD_ENTRIES_NUM 0xBC */
#define B2_RX_PP_SHRAM_FIFO_RD_ENTRIES_NUM_FIELD_SHRAM_FIFO_RD_ENTRIES_NUM_MASK                                   0x000007FF
//========================================
/* REG_RX_PP_SHRAM_FIFO_NUM_ENTRIES_COUNT 0xC0 */
#define B2_RX_PP_SHRAM_FIFO_NUM_ENTRIES_COUNT_FIELD_SHRAM_FIFO_NUM_ENTRIES_COUNT_MASK                             0x000007FF
//========================================
/* REG_RX_PP_SHRAM_FIFO_DEBUG 0xC4 */
#define B2_RX_PP_SHRAM_FIFO_DEBUG_FIELD_SHRAM_FIFO_WR_PTR_MASK                                                    0x000003FF
#define B2_RX_PP_SHRAM_FIFO_DEBUG_FIELD_SHRAM_FIFO_NOT_EMPTY_MASK                                                 0x00010000
#define B2_RX_PP_SHRAM_FIFO_DEBUG_FIELD_SHRAM_FIFO_FULL_MASK                                                      0x00020000
#define B2_RX_PP_SHRAM_FIFO_DEBUG_FIELD_SHRAM_FIFO_FULL_DROP_CTR_MASK                                             0xFF000000
//========================================
/* REG_RX_PP_SYN_FIFO_STATUS 0xC8 */
#define B2_RX_PP_SYN_FIFO_STATUS_FIELD_SHRAM_SYN_FIFO_BYTES_IN_FIFO_MASK                                          0x00000003
#define B2_RX_PP_SYN_FIFO_STATUS_FIELD_SHRAM_SYN_FIFO_FULL_MASK                                                   0x00000010
#define B2_RX_PP_SYN_FIFO_STATUS_FIELD_SHRAM_SYN_FIFO_EMPTY_MASK                                                  0x00000020
#define B2_RX_PP_SYN_FIFO_STATUS_FIELD_SHRAM_SYN_FIFO_POP_WHILE_EMPTY_MASK                                        0x00000040
#define B2_RX_PP_SYN_FIFO_STATUS_FIELD_SHRAM_SYN_FIFO_PUSH_WHILE_FULL_MASK                                        0x00000080
//========================================
/* REG_RX_PP_CONTROL_BACKDOOR_UPDATE 0xCC */
#define B2_RX_PP_CONTROL_BACKDOOR_UPDATE_FIELD_CONTROL_SN_NUM_VALID_MASK                                          0x00000001
#define B2_RX_PP_CONTROL_BACKDOOR_UPDATE_FIELD_CONTROL_PN_NUM_VALID_MASK                                          0x00000002
#define B2_RX_PP_CONTROL_BACKDOOR_UPDATE_FIELD_CONTROL_FRAG_EN_MASK                                               0x00000004
#define B2_RX_PP_CONTROL_BACKDOOR_UPDATE_FIELD_CONTROL_HIGHEST_SN_NUM_VALID_MASK                                  0x00000008
#define B2_RX_PP_CONTROL_BACKDOOR_UPDATE_FIELD_CONTROL_BE_MASK                                                    0x000000F0
#define B2_RX_PP_CONTROL_BACKDOOR_UPDATE_FIELD_CONTROL_UPDATE_DONE_MASK                                           0x00000100
//========================================
/* REG_RX_PP_RX_PP_SPARE 0xD0 */
#define B2_RX_PP_RX_PP_SPARE_FIELD_RX_PP_SPARE_MASK                                                               0xFFFFFFFF
//========================================
/* REG_RX_PP_RX_COUNTERS_CFG 0xD4 */
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_QOS_RX_EN_MASK                                                             0x00000001
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_RD_COUNT_EN_MASK                                                           0x00000002
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_RD_DELAYED_COUNT_EN_MASK                                                   0x00000004
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_RD_SW_DROP_COUNT_EN_MASK                                                   0x00000008
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_RD_DUPLICATE_DROP_COUNT_EN_MASK                                            0x00000010
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_MISSING_SN_SW_UPDATE_COUNT_EN_MASK                                         0x00000020
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_MPDU_UNI_OR_MGMT_EN_MASK                                                   0x00000040
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_MPDU_RETRY_EN_MASK                                                         0x00000080
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_AMSDU_COUNT_EN_MASK                                                        0x00000100
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_DROP_MPDU_EN_MASK                                                          0x00000200
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_MPDU_TYPE_NOT_SUPPORTED_EN_MASK                                            0x00000400
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_BAR_MPDU_COUNT_EN_MASK                                                     0x00000800
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_REPLAY_EN_MASK                                                             0x00001000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_REPLAY_MGMT_EN_MASK                                                        0x00002000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_TKIP_COUNT_EN_MASK                                                         0x00004000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_FAILURE_COUNT_EN_MASK                                                      0x00008000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_RX_CLASS_AMSDU_BYTES_STB_EN_MASK                                           0x00010000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_RXC_AMPDU_STB_EN_MASK                                                      0x00020000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_RX_CLASS_MPDU_IN_AMPDU_STB_EN_MASK                                         0x00040000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_RX_CLASS_OCTET_MPDU_STB_EN_MASK                                            0x00080000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_RX_CLASS_DROP_STB_EN_MASK                                                  0x00100000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_RX_CLASS_SECUR_MIS_STB_EN_MASK                                             0x00200000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_RXC_CRC_ERROR_STB_EN_MASK                                                  0x00400000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_DELINEATOR_CRC_ERROR_STB_EN_MASK                                           0x00800000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_BC_MC_COUNT_EN_MASK                                                        0x01000000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_QOS_STA_TID_RX_EN_MASK                                                     0x20000000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_QOS_RX_DEC_EN_MASK                                                         0x40000000
#define B2_RX_PP_RX_COUNTERS_CFG_FIELD_RX_COUNT_CLEAN_FIFO_MASK                                                   0x80000000
//========================================
/* REG_RX_PP_RX_LOGGER_CFG 0xD8 */
#define B2_RX_PP_RX_LOGGER_CFG_FIELD_RX_PP_LOGGER_PRIORITY_MASK                                                   0x00000003
#define B2_RX_PP_RX_LOGGER_CFG_FIELD_RX_PP_LOGGER_EN_MASK                                                         0x00000004
//========================================
/* REG_RX_PP_RX_LOGGER_BUSY 0xDC */
#define B2_RX_PP_RX_LOGGER_BUSY_FIELD_RX_PP_LOGGER_BUSY_MASK                                                      0x00000001
//========================================
/* REG_RX_PP_SW_REKEY_EN 0xE0 */
#define B2_RX_PP_SW_REKEY_EN_FIELD_REKEY_EN_MASK                                                                  0x00000001
#define B2_RX_PP_SW_REKEY_EN_FIELD_KEY_ID_CHECK_EN_MASK                                                           0x00000002
//========================================
/* REG_RX_PP_SW_REKEY_INST_LSB 0xE4 */
#define B2_RX_PP_SW_REKEY_INST_LSB_FIELD_REKEY_STA_MASK                                                           0x000000FF
#define B2_RX_PP_SW_REKEY_INST_LSB_FIELD_REKEY_VAP_ACCESS_MASK                                                    0x00000100
#define B2_RX_PP_SW_REKEY_INST_LSB_FIELD_REKEY_DATA_UPDATE_ONLY_MASK                                              0x00000200
#define B2_RX_PP_SW_REKEY_INST_LSB_FIELD_REKEY_KEY_MASK                                                           0x00003000
#define B2_RX_PP_SW_REKEY_INST_LSB_FIELD_REKEY_MGMT_TID_EN_MASK                                                   0x00004000
#define B2_RX_PP_SW_REKEY_INST_LSB_FIELD_REKEY_DONE_MASK                                                          0x00008000
#define B2_RX_PP_SW_REKEY_INST_LSB_FIELD_REKEY_PN_0_15_MASK                                                       0xFFFF0000
//========================================
/* REG_RX_PP_SW_REKEY_INST_MSB 0xE8 */
#define B2_RX_PP_SW_REKEY_INST_MSB_FIELD_REKEY_PN_16_47_MASK                                                      0xFFFFFFFF
//========================================
/* REG_RX_PP_FRAG_DIS_LIB_ERR_LIST 0xEC */
#define B2_RX_PP_FRAG_DIS_LIB_ERR_LIST_FIELD_FRAG_DIS_LIB_ERR_LIST_MASK                                           0x00000001
//========================================
/* REG_RX_PP_SM_STATUS_PART1 0xF0 */
#define B2_RX_PP_SM_STATUS_PART1_FIELD_CLEAR_MPDU_SM_MASK                                                         0x0000003F
#define B2_RX_PP_SM_STATUS_PART1_FIELD_OOO_DIG_MPDU_SM_MASK                                                       0x000007C0
#define B2_RX_PP_SM_STATUS_PART1_FIELD_SSN_CLEAR_MPDU_CLEAR_SM_MASK                                               0x0000F800
#define B2_RX_PP_SM_STATUS_PART1_FIELD_IMP_MPDU_CLEAR_SM_MASK                                                     0x00070000
#define B2_RX_PP_SM_STATUS_PART1_FIELD_NO_BAA_CLEAR_SM_MASK                                                       0x00F80000
#define B2_RX_PP_SM_STATUS_PART1_FIELD_FLUSH_SM_MASK                                                              0x0F000000
#define B2_RX_PP_SM_STATUS_PART1_FIELD_REKEY_HIT_SM_MASK                                                          0xF0000000
//========================================
/* REG_RX_PP_SM_STATUS_PART2 0xF4 */
#define B2_RX_PP_SM_STATUS_PART2_FIELD_STATUS_UPDATE_SM_MASK                                                      0x00000003
#define B2_RX_PP_SM_STATUS_PART2_FIELD_SET_REKEY_SM_MASK                                                          0x00000070
#define B2_RX_PP_SM_STATUS_PART2_FIELD_FRAG_EN_UPDATE_SM_MASK                                                     0x00000300
#define B2_RX_PP_SM_STATUS_PART2_FIELD_MAIN_MPDU_SM_MASK                                                          0x0001F000
#define B2_RX_PP_SM_STATUS_PART2_FIELD_SN_UPDATE_SM_MASK                                                          0x00F00000
#define B2_RX_PP_SM_STATUS_PART2_FIELD_LOGGER_SM_MASK                                                             0x07000000
#define B2_RX_PP_SM_STATUS_PART2_FIELD_WINDOW_SIZE_SM_MASK                                                        0x70000000
#define B2_RX_PP_SM_STATUS_PART2_FIELD_SHRAM_FIFO_SM_MASK                                                         0x80000000
//========================================
/* REG_RX_PP_UC_MGMT_TID_CFG 0xF8 */
#define B2_RX_PP_UC_MGMT_TID_CFG_FIELD_UC_MGMT_TID_CFG_MASK                                                       0xFFFFFFFF
//========================================
/* REG_RX_PP_MC_CFG 0xFC */
#define B2_RX_PP_MC_CFG_FIELD_MC_CFG_MASK                                                                         0xFFFFFFFF
//========================================
/* REG_RX_PP_RX_PP_INT_ERROR_STATUS 0x100 */
#define B2_RX_PP_RX_PP_INT_ERROR_STATUS_FIELD_SHRAM_SYN_FIFO_OVERFLOW_STATUS_MASK                                 0x00000001
#define B2_RX_PP_RX_PP_INT_ERROR_STATUS_FIELD_SHRAM_FIFO_OVERFLOW_MASK                                            0x00000002
#define B2_RX_PP_RX_PP_INT_ERROR_STATUS_FIELD_SHRAM_FIFO_DECREMENT_LESS_THAN_ZERO_MASK                            0x00000004
//========================================
/* REG_RX_PP_RX_PP_INT_ERROR_EN 0x104 */
#define B2_RX_PP_RX_PP_INT_ERROR_EN_FIELD_SHRAM_SYN_FIFO_OVERFLOW_EN_MASK                                         0x00000001
#define B2_RX_PP_RX_PP_INT_ERROR_EN_FIELD_SHRAM_FIFO_FULL_DROP_CTR_EN_MASK                                        0x00000002
#define B2_RX_PP_RX_PP_INT_ERROR_EN_FIELD_SHRAM_FIFO_DEC_LESS_THAN_ZERO_EN_MASK                                   0x00000004
//========================================
/* REG_RX_PP_RX_PP_INT_ERROR_CLEAR 0x108 */
#define B2_RX_PP_RX_PP_INT_ERROR_CLEAR_FIELD_SHRAM_SYN_FIFO_CLEAR_OVERFLOW_STRB_MASK                              0x00000001
#define B2_RX_PP_RX_PP_INT_ERROR_CLEAR_FIELD_SHRAM_FIFO_CLEAR_FULL_DROP_CTR_STRB_MASK                             0x00000002
#define B2_RX_PP_RX_PP_INT_ERROR_CLEAR_FIELD_SHRAM_FIFO_CLEAR_DEC_LESS_THAN_ZERO_STRB_MASK                        0x00000004
//========================================
/* REG_RX_PP_RX_PP_CONTROL_BITS 0x10C */
#define B2_RX_PP_RX_PP_CONTROL_BITS_FIELD_TRAFFIC_INDICATOR_EN_MASK                                               0x00000001
#define B2_RX_PP_RX_PP_CONTROL_BITS_FIELD_RX_PP_GADF_MLD_ENABLE_MASK                                              0x00000002
#define B2_RX_PP_RX_PP_CONTROL_BITS_FIELD_RX_PP_GADF_UMAC_PRECEDES_DUP_DETECT_MASK                                0x00000004
//========================================
/* REG_RX_PP_RXDD_SW_UPDATE_LAST_SN 0x110 */
#define B2_RX_PP_RXDD_SW_UPDATE_LAST_SN_FIELD_SW_UPDATE_LAST_SN_MASK                                              0x00000FFF
//========================================
/* REG_RX_PP_RXDD_SW_UPDATE_DUP_ENABLE 0x114 */
#define B2_RX_PP_RXDD_SW_UPDATE_DUP_ENABLE_FIELD_SW_UPDATE_DUP_ENABLE_MASK                                        0x00000001
//========================================
/* REG_RX_PP_RXDD_SW_UPDATE_SN_VALID 0x118 */
#define B2_RX_PP_RXDD_SW_UPDATE_SN_VALID_FIELD_SW_UPDATE_SN_VALID_MASK                                            0x00000001
//========================================
/* REG_RX_PP_RXDD_SW_RD_DUP_DB 0x11C */
#define B2_RX_PP_RXDD_SW_RD_DUP_DB_FIELD_SW_RD_LAST_SN_MASK                                                       0x00000FFF
#define B2_RX_PP_RXDD_SW_RD_DUP_DB_FIELD_SW_RD_SN_VALID_MASK                                                      0x00001000
#define B2_RX_PP_RXDD_SW_RD_DUP_DB_FIELD_SW_RD_DUP_ENABLE_MASK                                                    0x00002000
//========================================
/* REG_RX_PP_RXDD_SN_0_INFO_DBG 0x120 */
#define B2_RX_PP_RXDD_SN_0_INFO_DBG_FIELD_QUERIED_SN_0_MASK                                                       0x00000FFF
#define B2_RX_PP_RXDD_SN_0_INFO_DBG_FIELD_LAST_SN_0_MASK                                                          0x00FFF000
#define B2_RX_PP_RXDD_SN_0_INFO_DBG_FIELD_DROP_RESULT_0_MASK                                                      0x01000000
#define B2_RX_PP_RXDD_SN_0_INFO_DBG_FIELD_RX_PP_ID_0_MASK                                                         0x02000000
#define B2_RX_PP_RXDD_SN_0_INFO_DBG_FIELD_PTR_VAL_0_MASK                                                          0x3C000000
#define B2_RX_PP_RXDD_SN_0_INFO_DBG_FIELD_VALID_0_MASK                                                            0x40000000
//========================================
/* REG_RX_PP_RXDD_SN_1_INFO_DBG 0x124 */
#define B2_RX_PP_RXDD_SN_1_INFO_DBG_FIELD_QUERIED_SN_1_MASK                                                       0x00000FFF
#define B2_RX_PP_RXDD_SN_1_INFO_DBG_FIELD_LAST_SN_1_MASK                                                          0x00FFF000
#define B2_RX_PP_RXDD_SN_1_INFO_DBG_FIELD_DROP_RESULT_1_MASK                                                      0x01000000
#define B2_RX_PP_RXDD_SN_1_INFO_DBG_FIELD_RX_PP_ID_1_MASK                                                         0x02000000
#define B2_RX_PP_RXDD_SN_1_INFO_DBG_FIELD_PTR_VAL_1_MASK                                                          0x3C000000
#define B2_RX_PP_RXDD_SN_1_INFO_DBG_FIELD_VALID_1_MASK                                                            0x40000000
//========================================
/* REG_RX_PP_RXDD_SN_2_INFO_DBG 0x128 */
#define B2_RX_PP_RXDD_SN_2_INFO_DBG_FIELD_QUERIED_SN_2_MASK                                                       0x00000FFF
#define B2_RX_PP_RXDD_SN_2_INFO_DBG_FIELD_LAST_SN_2_MASK                                                          0x00FFF000
#define B2_RX_PP_RXDD_SN_2_INFO_DBG_FIELD_DROP_RESULT_2_MASK                                                      0x01000000
#define B2_RX_PP_RXDD_SN_2_INFO_DBG_FIELD_RX_PP_ID_2_MASK                                                         0x02000000
#define B2_RX_PP_RXDD_SN_2_INFO_DBG_FIELD_PTR_VAL_2_MASK                                                          0x3C000000
#define B2_RX_PP_RXDD_SN_2_INFO_DBG_FIELD_VALID_2_MASK                                                            0x40000000
//========================================
/* REG_RX_PP_RXDD_SN_3_INFO_DBG 0x12C */
#define B2_RX_PP_RXDD_SN_3_INFO_DBG_FIELD_QUERIED_SN_3_MASK                                                       0x00000FFF
#define B2_RX_PP_RXDD_SN_3_INFO_DBG_FIELD_LAST_SN_3_MASK                                                          0x00FFF000
#define B2_RX_PP_RXDD_SN_3_INFO_DBG_FIELD_DROP_RESULT_3_MASK                                                      0x01000000
#define B2_RX_PP_RXDD_SN_3_INFO_DBG_FIELD_RX_PP_ID_3_MASK                                                         0x02000000
#define B2_RX_PP_RXDD_SN_3_INFO_DBG_FIELD_PTR_VAL_3_MASK                                                          0x3C000000
#define B2_RX_PP_RXDD_SN_3_INFO_DBG_FIELD_VALID_3_MASK                                                            0x40000000
//========================================
/* REG_RX_PP_RXDD_SN_4_INFO_DBG 0x130 */
#define B2_RX_PP_RXDD_SN_4_INFO_DBG_FIELD_QUERIED_SN_4_MASK                                                       0x00000FFF
#define B2_RX_PP_RXDD_SN_4_INFO_DBG_FIELD_LAST_SN_4_MASK                                                          0x00FFF000
#define B2_RX_PP_RXDD_SN_4_INFO_DBG_FIELD_DROP_RESULT_4_MASK                                                      0x01000000
#define B2_RX_PP_RXDD_SN_4_INFO_DBG_FIELD_RX_PP_ID_4_MASK                                                         0x02000000
#define B2_RX_PP_RXDD_SN_4_INFO_DBG_FIELD_PTR_VAL_4_MASK                                                          0x3C000000
#define B2_RX_PP_RXDD_SN_4_INFO_DBG_FIELD_VALID_4_MASK                                                            0x40000000
//========================================
/* REG_RX_PP_RXDD_SN_5_INFO_DBG 0x134 */
#define B2_RX_PP_RXDD_SN_5_INFO_DBG_FIELD_QUERIED_SN_5_MASK                                                       0x00000FFF
#define B2_RX_PP_RXDD_SN_5_INFO_DBG_FIELD_LAST_SN_5_MASK                                                          0x00FFF000
#define B2_RX_PP_RXDD_SN_5_INFO_DBG_FIELD_DROP_RESULT_5_MASK                                                      0x01000000
#define B2_RX_PP_RXDD_SN_5_INFO_DBG_FIELD_RX_PP_ID_5_MASK                                                         0x02000000
#define B2_RX_PP_RXDD_SN_5_INFO_DBG_FIELD_PTR_VAL_5_MASK                                                          0x3C000000
#define B2_RX_PP_RXDD_SN_5_INFO_DBG_FIELD_VALID_5_MASK                                                            0x40000000
//========================================
/* REG_RX_PP_RXDD_SN_6_INFO_DBG 0x138 */
#define B2_RX_PP_RXDD_SN_6_INFO_DBG_FIELD_QUERIED_SN_6_MASK                                                       0x00000FFF
#define B2_RX_PP_RXDD_SN_6_INFO_DBG_FIELD_LAST_SN_6_MASK                                                          0x00FFF000
#define B2_RX_PP_RXDD_SN_6_INFO_DBG_FIELD_DROP_RESULT_6_MASK                                                      0x01000000
#define B2_RX_PP_RXDD_SN_6_INFO_DBG_FIELD_RX_PP_ID_6_MASK                                                         0x02000000
#define B2_RX_PP_RXDD_SN_6_INFO_DBG_FIELD_PTR_VAL_6_MASK                                                          0x3C000000
#define B2_RX_PP_RXDD_SN_6_INFO_DBG_FIELD_VALID_6_MASK                                                            0x40000000
//========================================
/* REG_RX_PP_RXDD_SN_7_INFO_DBG 0x13C */
#define B2_RX_PP_RXDD_SN_7_INFO_DBG_FIELD_QUERIED_SN_7_MASK                                                       0x00000FFF
#define B2_RX_PP_RXDD_SN_7_INFO_DBG_FIELD_LAST_SN_7_MASK                                                          0x00FFF000
#define B2_RX_PP_RXDD_SN_7_INFO_DBG_FIELD_DROP_RESULT_7_MASK                                                      0x01000000
#define B2_RX_PP_RXDD_SN_7_INFO_DBG_FIELD_RX_PP_ID_7_MASK                                                         0x02000000
#define B2_RX_PP_RXDD_SN_7_INFO_DBG_FIELD_PTR_VAL_7_MASK                                                          0x3C000000
#define B2_RX_PP_RXDD_SN_7_INFO_DBG_FIELD_VALID_7_MASK                                                            0x40000000
//========================================
/* REG_RX_PP_MC_DATA_CFG 0x140 */
#define B2_RX_PP_MC_DATA_CFG_FIELD_MC_DATA_CFG_MASK                                                               0xFFFFFFFF
//========================================
/* REG_RX_PP_GADF_VAP_IDX_OVERWRITE_B0 0x144 */
#define B2_RX_PP_GADF_VAP_IDX_OVERWRITE_B0_FIELD_GADF_NET_IFC_IDX_B0_MASK                                         0x0000007F
//========================================
/* REG_RX_PP_GADF_VAP_IDX_OVERWRITE_B1 0x148 */
#define B2_RX_PP_GADF_VAP_IDX_OVERWRITE_B1_FIELD_GADF_NET_IFC_IDX_B1_MASK                                         0x0000007F
//========================================
/* REG_RX_PP_GADF_VAP_IDX_OVERWRITE_B2 0x14C */
#define B2_RX_PP_GADF_VAP_IDX_OVERWRITE_B2_FIELD_GADF_NET_IFC_IDX_B2_MASK                                         0x0000007F


#endif // _RX_PP_REGS_H_
