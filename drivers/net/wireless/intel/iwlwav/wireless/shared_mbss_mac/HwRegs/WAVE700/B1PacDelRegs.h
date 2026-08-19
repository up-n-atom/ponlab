
/***********************************************************************************
File:				B1PacDelRegs.h
Module:				b1PacDel
SOC Revision:		latest
Generated at:       2022-05-04 10:57:42
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B1_PAC_DEL_REGS_H_
#define _B1_PAC_DEL_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B1_PAC_DEL_BASE_ADDRESS                             MEMORY_MAP_UNIT_10008_BASE_ADDRESS
#define	REG_B1_PAC_DEL_BASE_0                               (B1_PAC_DEL_BASE_ADDRESS + 0x0)
#define	REG_B1_PAC_DEL_BASE_1                               (B1_PAC_DEL_BASE_ADDRESS + 0x4)
#define	REG_B1_PAC_DEL_BASE_2                               (B1_PAC_DEL_BASE_ADDRESS + 0x8)
#define	REG_B1_PAC_DEL_BASE_3                               (B1_PAC_DEL_BASE_ADDRESS + 0xC)
#define	REG_B1_PAC_DEL_BASE_4                               (B1_PAC_DEL_BASE_ADDRESS + 0x10)
#define	REG_B1_PAC_DEL_BASE_5                               (B1_PAC_DEL_BASE_ADDRESS + 0x14)
#define	REG_B1_PAC_DEL_BASE_6                               (B1_PAC_DEL_BASE_ADDRESS + 0x18)
#define	REG_B1_PAC_DEL_RETURN_ADDRESS                       (B1_PAC_DEL_BASE_ADDRESS + 0x1C)
#define	REG_B1_PAC_DEL_RECIPE_POINTER                       (B1_PAC_DEL_BASE_ADDRESS + 0x20)
#define	REG_B1_PAC_DEL_DELIA_STATUS                         (B1_PAC_DEL_BASE_ADDRESS + 0x24)
#define	REG_B1_PAC_DEL_IFS_COMPENSATION                     (B1_PAC_DEL_BASE_ADDRESS + 0x28)
#define	REG_B1_PAC_DEL_DELIA_CONTROL                        (B1_PAC_DEL_BASE_ADDRESS + 0x30)
#define	REG_B1_PAC_DEL_INDEXED_JUMP_ADDR                    (B1_PAC_DEL_BASE_ADDRESS + 0x34)
#define	REG_B1_PAC_DEL_MP_TX_DATA_WORD_COUNT                (B1_PAC_DEL_BASE_ADDRESS + 0x38)
#define	REG_B1_PAC_DEL_DELIA_LAST_INST_ADDR                 (B1_PAC_DEL_BASE_ADDRESS + 0x3C)
#define	REG_B1_PAC_DEL_AUTO_REPLY_MODE                      (B1_PAC_DEL_BASE_ADDRESS + 0x40)
#define	REG_B1_PAC_DEL_RECIPE_POINTER_DEBUG_WR              (B1_PAC_DEL_BASE_ADDRESS + 0x48)
#define	REG_B1_PAC_DEL_TX_ADDR1_31TO0                       (B1_PAC_DEL_BASE_ADDRESS + 0x4C)
#define	REG_B1_PAC_DEL_TX_ADDR1_47TO32                      (B1_PAC_DEL_BASE_ADDRESS + 0x50)
#define	REG_B1_PAC_DEL_CPU_DEL_WR_EVENTS                    (B1_PAC_DEL_BASE_ADDRESS + 0x5C)
#define	REG_B1_PAC_DEL_CPU_DEL_RD_EVENTS                    (B1_PAC_DEL_BASE_ADDRESS + 0x60)
#define	REG_B1_PAC_DEL_CPU_DEL_WR_CNTR0_EVENT               (B1_PAC_DEL_BASE_ADDRESS + 0x64)
#define	REG_B1_PAC_DEL_CPU_DEL_RD_CNTR0_EVENT               (B1_PAC_DEL_BASE_ADDRESS + 0x68)
#define	REG_B1_PAC_DEL_CPU_DEL_WR_CNTR1_EVENT               (B1_PAC_DEL_BASE_ADDRESS + 0x6C)
#define	REG_B1_PAC_DEL_CPU_DEL_RD_CNTR1_EVENT               (B1_PAC_DEL_BASE_ADDRESS + 0x70)
#define	REG_B1_PAC_DEL_LAST_CPU_DEL_CNTR_EVENT              (B1_PAC_DEL_BASE_ADDRESS + 0x74)
#define	REG_B1_PAC_DEL_LAST_RD_ADDR_TRANSFER                (B1_PAC_DEL_BASE_ADDRESS + 0x7C)
#define	REG_B1_PAC_DEL_MP_TX_TCR_RCR_WORD_COUNT             (B1_PAC_DEL_BASE_ADDRESS + 0x80)
#define	REG_B1_PAC_DEL_MP_FIFO_DEBUG                        (B1_PAC_DEL_BASE_ADDRESS + 0x84)
#define	REG_B1_PAC_DEL_TX_DATA_STRUCTURE_ADDR               (B1_PAC_DEL_BASE_ADDRESS + 0x88)
#define	REG_B1_PAC_DEL_EMU_PRBS_GCLK_BYPASS                 (B1_PAC_DEL_BASE_ADDRESS + 0x8C)
#define	REG_B1_PAC_DEL_BF_TX_MAX_THRESHOLD_TIMER            (B1_PAC_DEL_BASE_ADDRESS + 0x90)
#define	REG_B1_PAC_DEL_DEL2GENRISC_START_TX_INT             (B1_PAC_DEL_BASE_ADDRESS + 0x94)
#define	REG_B1_PAC_DEL_DEL2GENRISC_START_TX_CLR_INT         (B1_PAC_DEL_BASE_ADDRESS + 0x98)
#define	REG_B1_PAC_DEL_NOP_WAIT_EVENT_VECTOR                (B1_PAC_DEL_BASE_ADDRESS + 0x9C)
#define	REG_B1_PAC_DEL_LAST_NOP_WAIT_EVENT_VECTOR           (B1_PAC_DEL_BASE_ADDRESS + 0xA0)
#define	REG_B1_PAC_DEL_DELIA_CNTRL_REGISTER                 (B1_PAC_DEL_BASE_ADDRESS + 0xA4)
#define	REG_B1_PAC_DEL_DELIA_LOGGER_CONTROL                 (B1_PAC_DEL_BASE_ADDRESS + 0xBC)
#define	REG_B1_PAC_DEL_DELIA_LOGGER_ACTIVE                  (B1_PAC_DEL_BASE_ADDRESS + 0xC0)
#define	REG_B1_PAC_DEL_TX_END_STATUS                        (B1_PAC_DEL_BASE_ADDRESS + 0xC4)
#define	REG_B1_PAC_DEL_PHY_RD_COUNT                         (B1_PAC_DEL_BASE_ADDRESS + 0xC8)
#define	REG_B1_PAC_DEL_PHY_DATA_FIFO_WR_COUNT               (B1_PAC_DEL_BASE_ADDRESS + 0xCC)
#define	REG_B1_PAC_DEL_RECIPE_WR_CNT                        (B1_PAC_DEL_BASE_ADDRESS + 0xD0)
#define	REG_B1_PAC_DEL_PHY_DATA_FIFO_AF_COUNT               (B1_PAC_DEL_BASE_ADDRESS + 0xE0)
#define	REG_B1_PAC_DEL_PHY_DATA_FIFO_RD_COUNT               (B1_PAC_DEL_BASE_ADDRESS + 0xE4)
#define	REG_B1_PAC_DEL_TOTAL_PSDU_LENGTH_128B               (B1_PAC_DEL_BASE_ADDRESS + 0xE8)
#define	REG_B1_PAC_DEL_PHY_TOTAL_REQ_SIZE                   (B1_PAC_DEL_BASE_ADDRESS + 0xEC)
#define	REG_B1_PAC_DEL_DELIMITER_VALUE_PRE_TX_DATA          (B1_PAC_DEL_BASE_ADDRESS + 0xF4)
#define	REG_B1_PAC_DEL_DEL2GENRISC_START_TX_ENABLE_INT      (B1_PAC_DEL_BASE_ADDRESS + 0xF8)
#define	REG_B1_PAC_DEL_USER_DONE_HIGH                       (B1_PAC_DEL_BASE_ADDRESS + 0xFC)
#define	REG_B1_PAC_DEL_USER_DONE_LOW                        (B1_PAC_DEL_BASE_ADDRESS + 0x100)
#define	REG_B1_PAC_DEL_NEW_HW_FIFO_EMPTY                    (B1_PAC_DEL_BASE_ADDRESS + 0x104)
#define	REG_B1_PAC_DEL_FIFO_CLEAN                           (B1_PAC_DEL_BASE_ADDRESS + 0x108)
#define	REG_B1_PAC_DEL_NEW_HW_DESC_LISTS_STATUS_CONTROL     (B1_PAC_DEL_BASE_ADDRESS + 0x110)
#define	REG_B1_PAC_DEL_PHY_DATA_FIFO_AF_TH                  (B1_PAC_DEL_BASE_ADDRESS + 0x114)
#define	REG_B1_PAC_DEL_FIFO_ERROR_STICKY                    (B1_PAC_DEL_BASE_ADDRESS + 0x118)
#define	REG_B1_PAC_DEL_ZLDS_TYPE_0_REG                      (B1_PAC_DEL_BASE_ADDRESS + 0x11C)
#define	REG_B1_PAC_DEL_ZLDS_TYPE_1_REG                      (B1_PAC_DEL_BASE_ADDRESS + 0x120)
#define	REG_B1_PAC_DEL_ZLDS_TYPE_2_REG                      (B1_PAC_DEL_BASE_ADDRESS + 0x124)
#define	REG_B1_PAC_DEL_DESC_AF_REG                          (B1_PAC_DEL_BASE_ADDRESS + 0x128)
#define	REG_B1_PAC_DEL_DATA_AF_REG                          (B1_PAC_DEL_BASE_ADDRESS + 0x12C)
#define	REG_B1_PAC_DEL_NEW_HW_STATE_MACHINES                (B1_PAC_DEL_BASE_ADDRESS + 0x130)
#define	REG_B1_PAC_DEL_NEW_HW_DONE                          (B1_PAC_DEL_BASE_ADDRESS + 0x134)
#define	REG_B1_PAC_DEL_DESC_AF_EVENT_STICKY_HIGH            (B1_PAC_DEL_BASE_ADDRESS + 0x138)
#define	REG_B1_PAC_DEL_DESC_AF_EVENT_STICKY_LOW             (B1_PAC_DEL_BASE_ADDRESS + 0x13C)
#define	REG_B1_PAC_DEL_DESC_AF_EVENT_COUNT                  (B1_PAC_DEL_BASE_ADDRESS + 0x140)
#define	REG_B1_PAC_DEL_FIFO_AF_STATUS                       (B1_PAC_DEL_BASE_ADDRESS + 0x144)
#define	REG_B1_PAC_DEL_BAND_IDX                             (B1_PAC_DEL_BASE_ADDRESS + 0x148)
#define	REG_B1_PAC_DEL_COM_TCR0_3                           (B1_PAC_DEL_BASE_ADDRESS + 0x14C)
#define	REG_B1_PAC_DEL_COM_TCR0_2                           (B1_PAC_DEL_BASE_ADDRESS + 0x150)
#define	REG_B1_PAC_DEL_COM_TCR0_1                           (B1_PAC_DEL_BASE_ADDRESS + 0x154)
#define	REG_B1_PAC_DEL_COM_TCR0_0                           (B1_PAC_DEL_BASE_ADDRESS + 0x158)
#define	REG_B1_PAC_DEL_COM_TCR1_3                           (B1_PAC_DEL_BASE_ADDRESS + 0x15C)
#define	REG_B1_PAC_DEL_COM_TCR1_2                           (B1_PAC_DEL_BASE_ADDRESS + 0x160)
#define	REG_B1_PAC_DEL_COM_TCR1_1                           (B1_PAC_DEL_BASE_ADDRESS + 0x164)
#define	REG_B1_PAC_DEL_COM_TCR1_0                           (B1_PAC_DEL_BASE_ADDRESS + 0x168)
#define	REG_B1_PAC_DEL_USER0_TCR_3                          (B1_PAC_DEL_BASE_ADDRESS + 0x16C)
#define	REG_B1_PAC_DEL_USER0_TCR_2                          (B1_PAC_DEL_BASE_ADDRESS + 0x170)
#define	REG_B1_PAC_DEL_USER0_TCR_1                          (B1_PAC_DEL_BASE_ADDRESS + 0x174)
#define	REG_B1_PAC_DEL_USER0_TCR_0                          (B1_PAC_DEL_BASE_ADDRESS + 0x178)
#define	REG_B1_PAC_DEL_DLM_PUSH_DPL_IDX_BASE                (B1_PAC_DEL_BASE_ADDRESS + 0x17C)
#define	REG_B1_PAC_DEL_REG2HOB_GP0                          (B1_PAC_DEL_BASE_ADDRESS + 0x180)
#define	REG_B1_PAC_DEL_REG2HOB_GP1                          (B1_PAC_DEL_BASE_ADDRESS + 0x184)
#define	REG_B1_PAC_DEL_SKIP_FLOW_CONTROL                    (B1_PAC_DEL_BASE_ADDRESS + 0x188)
#define	REG_B1_PAC_DEL_DEBUG_DUP_CTRL                       (B1_PAC_DEL_BASE_ADDRESS + 0x18C)
#define	REG_B1_PAC_DEL_TCR_BASE_ADDR1                       (B1_PAC_DEL_BASE_ADDRESS + 0x190)
#define	REG_B1_PAC_DEL_TCR_BASE_ADDR2                       (B1_PAC_DEL_BASE_ADDRESS + 0x194)
#define	REG_B1_PAC_DEL_RCR_BASE_ADDR                        (B1_PAC_DEL_BASE_ADDRESS + 0x198)
#define	REG_B1_PAC_DEL_SENDER_MPDU_DURATION                 (B1_PAC_DEL_BASE_ADDRESS + 0x19C)
#define	REG_B1_PAC_DEL_MAC_PHY_IF_SIGNALS_MONITOR           (B1_PAC_DEL_BASE_ADDRESS + 0x1A0)
#define	REG_B1_PAC_DEL_START_RECIPE_CNT                     (B1_PAC_DEL_BASE_ADDRESS + 0x1A4)
#define	REG_B1_PAC_DEL_PHY_DATA_FIFO_AF_COUNT_WM            (B1_PAC_DEL_BASE_ADDRESS + 0x1A8)
#define	REG_B1_PAC_DEL_PHY_DEBUG                            (B1_PAC_DEL_BASE_ADDRESS + 0x1AC)
#define	REG_B1_PAC_DEL_PAC_PHY_MONITOR_CTRL                 (B1_PAC_DEL_BASE_ADDRESS + 0x1B0)
#define	REG_B1_PAC_DEL_PAC_PHY_MONITOR_DATA_3               (B1_PAC_DEL_BASE_ADDRESS + 0x1B4)
#define	REG_B1_PAC_DEL_PAC_PHY_MONITOR_DATA_2               (B1_PAC_DEL_BASE_ADDRESS + 0x1B8)
#define	REG_B1_PAC_DEL_PAC_PHY_MONITOR_DATA_1               (B1_PAC_DEL_BASE_ADDRESS + 0x1BC)
#define	REG_B1_PAC_DEL_PAC_PHY_MONITOR_DATA_0               (B1_PAC_DEL_BASE_ADDRESS + 0x1C0)
#define	REG_B1_PAC_DEL_PHY_DATA_FIFO_HOST_RD_ADDRESS        (B1_PAC_DEL_BASE_ADDRESS + 0x1C4)
#define	REG_B1_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_3           (B1_PAC_DEL_BASE_ADDRESS + 0x1C8)
#define	REG_B1_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_2           (B1_PAC_DEL_BASE_ADDRESS + 0x1CC)
#define	REG_B1_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_1           (B1_PAC_DEL_BASE_ADDRESS + 0x1D0)
#define	REG_B1_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_0           (B1_PAC_DEL_BASE_ADDRESS + 0x1D4)
#define	REG_B1_PAC_DEL_FC_WR_PTR_SELECT_USER_ID             (B1_PAC_DEL_BASE_ADDRESS + 0x1D8)
#define	REG_B1_PAC_DEL_FC_WR_PTR_FOR_SELECTED_USER          (B1_PAC_DEL_BASE_ADDRESS + 0x1DC)
#define	REG_B1_PAC_DEL_GOOD_MPDU_COUNT_SELECT_USER_ID       (B1_PAC_DEL_BASE_ADDRESS + 0x1E0)
#define	REG_B1_PAC_DEL_GOOD_MPDU_COUNT_FOR_SELECTED_USER    (B1_PAC_DEL_BASE_ADDRESS + 0x1E4)
#define	REG_B1_PAC_DEL_NEW_HW_SKIP_CHECK_FC                 (B1_PAC_DEL_BASE_ADDRESS + 0x1E8)
#define	REG_B1_PAC_DEL_DESC_LIST_EMPTY_HIGH                 (B1_PAC_DEL_BASE_ADDRESS + 0x1EC)
#define	REG_B1_PAC_DEL_DESC_LIST_EMPTY_LOW                  (B1_PAC_DEL_BASE_ADDRESS + 0x1F0)
#define	REG_B1_PAC_DEL_SPARE_ECO                            (B1_PAC_DEL_BASE_ADDRESS + 0x1FC)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B1_PAC_DEL_BASE_0 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 base0 : 32; //DEL  base 0, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelBase0_u;

/*REG_B1_PAC_DEL_BASE_1 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 base1 : 32; //DEL  base 1, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelBase1_u;

/*REG_B1_PAC_DEL_BASE_2 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 base2 : 32; //DEL  base 2, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelBase2_u;

/*REG_B1_PAC_DEL_BASE_3 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 base3 : 32; //DEL  base 3, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelBase3_u;

/*REG_B1_PAC_DEL_BASE_4 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 base4 : 32; //DEL  base 4, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelBase4_u;

/*REG_B1_PAC_DEL_BASE_5 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 base5 : 32; //DEL  base 5, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelBase5_u;

/*REG_B1_PAC_DEL_BASE_6 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 base6 : 32; //DEL  base 6, reset value: 0x400000, access type: RO
	} bitFields;
} RegB1PacDelBase6_u;

/*REG_B1_PAC_DEL_RETURN_ADDRESS 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 returnAddress : 24; //DEL  return address, reset value: 0x0, access type: RO
		uint32 reserved1 : 6;
	} bitFields;
} RegB1PacDelReturnAddress_u;

/*REG_B1_PAC_DEL_RECIPE_POINTER 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 recipePointer : 24; //DEL  recipe pointer, reset value: 0x0, access type: RO
		uint32 reserved1 : 6;
	} bitFields;
} RegB1PacDelRecipePointer_u;

/*REG_B1_PAC_DEL_DELIA_STATUS 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 deliaStatusBusy : 1; //DEL  delia status, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 deliaState : 4; //DEL  delia state, reset value: 0x0, access type: RO
		uint32 reserved1 : 24;
	} bitFields;
} RegB1PacDelDeliaStatus_u;

/*REG_B1_PAC_DEL_IFS_COMPENSATION 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ifsCompensationOfdm : 16; //DEL ifs compensation when the last Tx PHY mode is OFDM, reset value: 0x0, access type: RW
		uint32 ifsCompensation11B : 16; //DEL ifs compensation when the last Tx PHY mode is 11B, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PacDelIfsCompensation_u;

/*REG_B1_PAC_DEL_DELIA_CONTROL 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyAutoStart : 1; //Phy auto start, reset value: 0x1, access type: RW
		uint32 tcrTxopDurationUnspecifiedEn : 1; //Enable setting TCR TXOP duration unspecified value for Auto-reply and BF report frames, reset value: 0x1, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PacDelDeliaControl_u;

/*REG_B1_PAC_DEL_INDEXED_JUMP_ADDR 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 indexedJumpAddr : 24; //Indexed jump address, reset value: 0x0, access type: RO
		uint32 reserved1 : 6;
	} bitFields;
} RegB1PacDelIndexedJumpAddr_u;

/*REG_B1_PAC_DEL_MP_TX_DATA_WORD_COUNT 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mpTxDataWordCtr : 18; //Tx data word count, reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1PacDelMpTxDataWordCount_u;

/*REG_B1_PAC_DEL_DELIA_LAST_INST_ADDR 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastFetchedInstruction : 32; //Last fetched instruction, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelDeliaLastInstAddr_u;

/*REG_B1_PAC_DEL_AUTO_REPLY_MODE 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoPowerMode : 1; //Auto Tx power mode for auto frames, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PacDelAutoReplyMode_u;

/*REG_B1_PAC_DEL_RECIPE_POINTER_DEBUG_WR 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 recipePointerDebugWr : 24; //DEL  write debug recipe pointer, reset value: 0x0, access type: WO
		uint32 reserved1 : 6;
	} bitFields;
} RegB1PacDelRecipePointerDebugWr_u;

/*REG_B1_PAC_DEL_TX_ADDR1_31TO0 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAddr131To0 : 32; //tx addr1 latch 31to0, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelTxAddr131To0_u;

/*REG_B1_PAC_DEL_TX_ADDR1_47TO32 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAddr147To32 : 16; //tx addr1 latch 47to32, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PacDelTxAddr147To32_u;

/*REG_B1_PAC_DEL_CPU_DEL_WR_EVENTS 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuDelWrEvents : 8; //CPU delia wr events, reset value: 0x0, access type: WO
		uint32 reserved0 : 24;
	} bitFields;
} RegB1PacDelCpuDelWrEvents_u;

/*REG_B1_PAC_DEL_CPU_DEL_RD_EVENTS 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuDelRdEvents : 8; //CPU delia rd events, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB1PacDelCpuDelRdEvents_u;

/*REG_B1_PAC_DEL_CPU_DEL_WR_CNTR0_EVENT 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuDelWrCntr0Event : 12; //CPU delia wr event counter0, reset value: 0x0, access type: WO
		uint32 reserved0 : 20;
	} bitFields;
} RegB1PacDelCpuDelWrCntr0Event_u;

/*REG_B1_PAC_DEL_CPU_DEL_RD_CNTR0_EVENT 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuDelRdCntr0Event : 12; //CPU delia rd event counter0, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
		uint32 hobDelRdCntr0Event : 12; //HOB delia rd event counter0, reset value: 0x0, access type: RO
		uint32 reserved1 : 4;
	} bitFields;
} RegB1PacDelCpuDelRdCntr0Event_u;

/*REG_B1_PAC_DEL_CPU_DEL_WR_CNTR1_EVENT 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuDelWrCntr1Event : 12; //CPU delia wr event counter1, reset value: 0x0, access type: WO
		uint32 reserved0 : 20;
	} bitFields;
} RegB1PacDelCpuDelWrCntr1Event_u;

/*REG_B1_PAC_DEL_CPU_DEL_RD_CNTR1_EVENT 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuDelRdCntr1Event : 12; //CPU delia rd event counter1, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
		uint32 hobDelRdCntr1Event : 12; //HOB delia rd event counter1, reset value: 0x0, access type: RO
		uint32 reserved1 : 4;
	} bitFields;
} RegB1PacDelCpuDelRdCntr1Event_u;

/*REG_B1_PAC_DEL_LAST_CPU_DEL_CNTR_EVENT 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastCpuDelCntr0Event : 12; //last CPU delia event counter0, reset value: 0x0, access type: RO
		uint32 lastCpuDelCntr1Event : 12; //last CPU delia event counter1, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB1PacDelLastCpuDelCntrEvent_u;

/*REG_B1_PAC_DEL_LAST_RD_ADDR_TRANSFER 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastRdAddrTransfer : 16; //last rd ShRAM addr transfer, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PacDelLastRdAddrTransfer_u;

/*REG_B1_PAC_DEL_MP_TX_TCR_RCR_WORD_COUNT 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mpTxTcrWordCtr : 7; //Tx TCR word count, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 mpTxRcrWordCtr : 7; //Tx RCR word count, reset value: 0x0, access type: RO
		uint32 reserved1 : 17;
	} bitFields;
} RegB1PacDelMpTxTcrRcrWordCount_u;

/*REG_B1_PAC_DEL_MP_FIFO_DEBUG 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mpFifoFull : 1; //MP FIFO full, reset value: 0x0, access type: RO
		uint32 mpFifoEmpty : 1; //MP FIFO empty, reset value: 0x1, access type: RO
		uint32 reserved0 : 2;
		uint32 ovenAlign : 2; //Num bytes in Oven, reset value: 0x0, access type: RO
		uint32 writeToWord : 2; //next write to word oven, reset value: 0x0, access type: RO
		uint32 mpFifoNumBytes : 7; //Num bytes in MP FIFO, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 mpFifoRdPtr : 6; //MP FIFO read pointer, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 mpFifoWrPtr : 6; //MP FIFO write pointer, reset value: 0x0, access type: RO
		uint32 reserved3 : 2;
	} bitFields;
} RegB1PacDelMpFifoDebug_u;

/*REG_B1_PAC_DEL_TX_DATA_STRUCTURE_ADDR 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDataStructureAddr : 26; //Tx data structure addr, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB1PacDelTxDataStructureAddr_u;

/*REG_B1_PAC_DEL_EMU_PRBS_GCLK_BYPASS 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPrbsGclkBypass : 1; //PRBS gclk bypass, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PacDelEmuPrbsGclkBypass_u;

/*REG_B1_PAC_DEL_BF_TX_MAX_THRESHOLD_TIMER 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfTxMaxThresholdTimer11Ag : 10; //Maximum time (in resolution of 1/8 [us]) that Delia must start transmit BF matrix in PHY mode 11ag, reset value: 0x1, access type: RW
		uint32 bfTxMaxThresholdTimer11N : 10; //Maximum time (in resolution of 1/8 [us]) that Delia must start transmit BF matrix in PHY mode 11n, reset value: 0x70, access type: RW
		uint32 bfTxMaxThresholdTimer11Ac : 10; //Maximum time (in resolution of 1/8 [us]) that Delia must start transmit BF matrix in PHY mode 11ac, reset value: 0x70, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB1PacDelBfTxMaxThresholdTimer_u;

/*REG_B1_PAC_DEL_DEL2GENRISC_START_TX_INT 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 del2GenriscStartTxInt : 6; //Delia to GenRisc interrupts, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB1PacDelDel2GenriscStartTxInt_u;

/*REG_B1_PAC_DEL_DEL2GENRISC_START_TX_CLR_INT 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 del2GenriscStartTxClrInt : 6; //Delia to GenRisc clear interrupts, reset value: 0x0, access type: WO
		uint32 reserved0 : 26;
	} bitFields;
} RegB1PacDelDel2GenriscStartTxClrInt_u;

/*REG_B1_PAC_DEL_NOP_WAIT_EVENT_VECTOR 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 nopWaitEventVector : 22; //nop wait event vector, reset value: 0x40c1, access type: RO
		uint32 reserved0 : 10;
	} bitFields;
} RegB1PacDelNopWaitEventVector_u;

/*REG_B1_PAC_DEL_LAST_NOP_WAIT_EVENT_VECTOR 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastNopWaitEventVector : 22; //Last NOP wait event vector, reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
	} bitFields;
} RegB1PacDelLastNopWaitEventVector_u;

/*REG_B1_PAC_DEL_DELIA_CNTRL_REGISTER 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enableEarlyAfRecovery : 1; //Enable AF recovery mechanism, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PacDelDeliaCntrlRegister_u;

/*REG_B1_PAC_DEL_DELIA_LOGGER_CONTROL 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 deliaLoggerEn : 1; //Delia logger enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 deliaLoggerPriority : 2; //Delia logger priority, reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
		uint32 deliaAfLoggerEn : 1; //delia_af_logger_en, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 deliaAfLoggerPriority : 2; //delia_af_logger_priority, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
		uint32 deliaAfLoggerSelectUserId : 7; //delia_af_logger_select_user_id, reset value: 0x0, access type: RW
		uint32 deliaAfLoggerCaptureOnlySopEop : 1; //delia_af_logger_capture_only_sop_eop, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PacDelDeliaLoggerControl_u;

/*REG_B1_PAC_DEL_DELIA_LOGGER_ACTIVE 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 deliaLoggerActive : 1; //Delia logger active, reset value: 0x0, access type: RO
		uint32 deliaAfLoggerActive : 1; //Delia AF logger active, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PacDelDeliaLoggerActive_u;

/*REG_B1_PAC_DEL_TX_END_STATUS 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txEndPhyDataFifoNotEmpty : 1; //Tx end PHY data FIFO is not empty, reset value: 0x0, access type: RO
		uint32 phyRdLessThanExpected : 1; //no description, reset value: 0x0, access type: RO
		uint32 txEndDeliaRecipeAborted : 1; //Tx end delia recipe aborted, reset value: 0x0, access type: RO
		uint32 txEndAutoFillOccured : 1; //Tx end auto fill occured, reset value: 0x0, access type: RO
		uint32 txEndAutoFillPayloadOccured : 1; //Tx end auto fill occurred only on the payload section, reset value: 0x0, access type: RO
		uint32 phyRdMoreThanExpected : 1; //no description, reset value: 0x0, access type: RO
		uint32 recipeWroteMoreThanExpcted : 1; //recipe_wrote_more_than_expcted, reset value: 0x0, access type: RO
		uint32 txEndAutoFillTcrOccured : 1; //Auto Fill occurred on TCRs, reset value: 0x0, access type: RO
		uint32 txEndAutoFillRcrOccured : 1; //Auto Fill occurred on RCRs, reset value: 0x0, access type: RO
		uint32 recipeWroteLessThanExpcted : 1; //recipe_wrote_less_than_expcted, reset value: 0x0, access type: RO
		uint32 phyReqLessThanExpected : 1; //phy_req_less_than_expected, reset value: 0x0, access type: RO
		uint32 phyReqMoreThanExpected : 1; //phy_req_more_than_expected, reset value: 0x0, access type: RO
		uint32 txEndOtherFifosNotEmpty : 1; //tx_end_other_fifos_not_empty, reset value: 0x0, access type: RO
		uint32 txEndFifoOvfUnfError : 1; //tx_end_fifo_ovf_unf_error, reset value: 0x0, access type: RO
		uint32 missingLastMpdu : 1; //MPDU last indication is missing on at least one active user , reset value: 0x0, access type: RO
		uint32 usedNewHw : 1; //Recipe used new HW, reset value: 0x0, access type: RO
		uint32 descListsNotEmpty : 1; //At least one descriptor lists is not empty, reset value: 0x0, access type: RO
		uint32 phyRdBeforeReq : 1; //PHY reads before request (relevant only in MU), reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB1PacDelTxEndStatus_u;

/*REG_B1_PAC_DEL_PHY_RD_COUNT 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyRdCount : 32; //Counts the number of read transactions from the PHY to the data FIFO. , Cleared on the rising edge of the PHY enable signal. , reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelPhyRdCount_u;

/*REG_B1_PAC_DEL_PHY_DATA_FIFO_WR_COUNT 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoWrCount : 32; //Counts the number of 128-bit words that were pushed into the PHY data FIFO. , Cleared on the rising edge of the PHY enable signal., reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelPhyDataFifoWrCount_u;

/*REG_B1_PAC_DEL_RECIPE_WR_CNT 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 recipeWrCnt : 32; //Recipe byte write count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelRecipeWrCnt_u;

/*REG_B1_PAC_DEL_PHY_DATA_FIFO_AF_COUNT 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoAfCount : 32; //Counts the number of 128-bit words that were auto-filled due to PHY data FIFO empty. , Cleared on the rising edge of the PHY enable signal., reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelPhyDataFifoAfCount_u;

/*REG_B1_PAC_DEL_PHY_DATA_FIFO_RD_COUNT 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoRdCount : 32; //Counts the number of 128-bit words that were pulled from the PHY data FIFO. , Cleared on the rising edge of the PHY enable signal., reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelPhyDataFifoRdCount_u;

/*REG_B1_PAC_DEL_TOTAL_PSDU_LENGTH_128B 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 totalPsduLength128B : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelTotalPsduLength128B_u;

/*REG_B1_PAC_DEL_PHY_TOTAL_REQ_SIZE 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyTotalReqSize : 32; //Total request size (all users), reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelPhyTotalReqSize_u;

/*REG_B1_PAC_DEL_DELIMITER_VALUE_PRE_TX_DATA 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 delimiterValuePreTxData : 32; //Delimiter value in case need to transmit at the start of the PSDU,  , Can be read by HOB., reset value: 0x4e0000f0, access type: RW
	} bitFields;
} RegB1PacDelDelimiterValuePreTxData_u;

/*REG_B1_PAC_DEL_DEL2GENRISC_START_TX_ENABLE_INT 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 del2GenriscStartTxEnableInt : 6; //Delia to GenRisc enable interrupts, reset value: 0x3f, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB1PacDelDel2GenriscStartTxEnableInt_u;

/*REG_B1_PAC_DEL_USER_DONE_HIGH 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userDoneHigh : 4; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB1PacDelUserDoneHigh_u;

/*REG_B1_PAC_DEL_USER_DONE_LOW 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userDoneLow : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelUserDoneLow_u;

/*REG_B1_PAC_DEL_NEW_HW_FIFO_EMPTY 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyReqFifoEmpty : 1; //Should be red after new HW task is done. , When '0', indicates that the FIFO is not empty (something went wrong) and it must be cleaned., reset value: 0x0, access type: RO
		uint32 mainSmCmdFifoEmpty : 1; //Should be red after new HW task is done. , When '0', indicates that the FIFO is not empty (something went wrong) and it must be cleaned., reset value: 0x0, access type: RO
		uint32 descReleaseFifoEmpty : 1; //Should be red after new HW task is done. , When '0', indicates that the FIFO is not empty (something went wrong) and it must be cleaned., reset value: 0x0, access type: RO
		uint32 rdDataCmdFifoEmpty : 1; //Should be red after new HW task is done. , When '0', indicates that the FIFO is not empty (something went wrong) and it must be cleaned., reset value: 0x0, access type: RO
		uint32 rdDataDataFifoEmpty : 1; //Should be red after new HW task is done. , When '0', indicates that the FIFO is not empty (something went wrong) and it must be cleaned., reset value: 0x0, access type: RO
		uint32 builderDataFifoEmpty : 1; //Should be red after new HW task is done. , When '0', indicates that the FIFO is not empty (something went wrong) and it must be cleaned., reset value: 0x0, access type: RO
		uint32 builderCmdFifoEmpty : 1; //Should be red after new HW task is done. , When '0', indicates that the FIFO is not empty (something went wrong) and it must be cleaned., reset value: 0x0, access type: RO
		uint32 crcDataFifoEmpty : 1; //Should be red after new HW task is done. , When '0', indicates that the FIFO is not empty (something went wrong) and it must be cleaned., reset value: 0x0, access type: RO
		uint32 crcCmdFifoEmpty : 1; //Should be red after new HW task is done. , When '0', indicates that the FIFO is not empty (something went wrong) and it must be cleaned., reset value: 0x0, access type: RO
		uint32 phyDataFifoEmpty : 1; //Should be red after new HW task is done. , When '0', indicates that the FIFO is not empty (something went wrong) and it must be cleaned., reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB1PacDelNewHwFifoEmpty_u;

/*REG_B1_PAC_DEL_FIFO_CLEAN 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifoClean : 1; //Clean all FIFOs (includes sticky error bits), reset value: 0x0, access type: WO
		uint32 reserved0 : 7;
		uint32 fifoCleanOnStart : 1; //fifo_clean_on_start, reset value: 0x1, access type: RW
		uint32 reserved1 : 23;
	} bitFields;
} RegB1PacDelFifoClean_u;

/*REG_B1_PAC_DEL_NEW_HW_DESC_LISTS_STATUS_CONTROL 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descListsEmpty : 1; //When '1', indicates that all descriptor lists are empty., reset value: 0x0, access type: RO
		uint32 cleanDescLists : 1; //Setting this bit will cause the descriptor cache controller to pop all the remaining descriptors of the active users and push them into the release list., reset value: 0x0, access type: WO
		uint32 cleanDescListsDone : 1; //When '1', indicates that the process of cleaning the descriptor lists is done. , Cleared by HW after clean_desc_lists is cleared., reset value: 0x0, access type: RO
		uint32 allUsersLastMpduDone : 1; //When set, all active users reached last MPDU, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB1PacDelNewHwDescListsStatusControl_u;

/*REG_B1_PAC_DEL_PHY_DATA_FIFO_AF_TH 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoAfTh : 7; //Sets the threshold of the PHY data FIFO for autofill., reset value: 0x4, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB1PacDelPhyDataFifoAfTh_u;

/*REG_B1_PAC_DEL_FIFO_ERROR_STICKY 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mainSmFifoErrorSticky : 4; //bit 0 - PHY request FIFO underflow , bit 1 - PHY request FIFO overflow , bit 2 - command FIFO underflow , bit 3 - command FIFO overflow , reset value: 0x0, access type: RO
		uint32 descReleaseFifoErrorSticky : 2; //bit 0 - Desc. release FIFO underflow , bit 1 - Desc. release FIFO overflow, reset value: 0x0, access type: RO
		uint32 rdDataFifoErrorSticky : 4; //bit 0 - data FIFO underflow , bit 1 - data  FIFO overflow , bit 2 - command FIFO underflow , bit 3 - command FIFO overflow , reset value: 0x0, access type: RO
		uint32 builderFifoErrorSticky : 4; //bit 0 - data FIFO underflow , bit 1 - data  FIFO overflow , bit 2 - command FIFO underflow , bit 3 - command FIFO overflow, reset value: 0x0, access type: RO
		uint32 crcFifoErrorSticky : 4; //bit 0 - data FIFO underflow , bit 1 - data  FIFO overflow , bit 2 - command FIFO underflow , bit 3 - command FIFO overflow, reset value: 0x0, access type: RO
		uint32 phyDataFifoErrorSticky : 2; //bit 0 - data FIFO underflow , bit 1 - data  FIFO overflow, reset value: 0x0, access type: RO
		uint32 legacyMpFifoErrorSticky : 2; //bit 0 - legacy mac-phy FIFO underflow , bit 1 - legacy mac-phy FIFO overflow, reset value: 0x0, access type: RO
		uint32 consecPhyRdErrorSticky : 1; //PHY asserted FIFO read signal for two consecutive cycles., reset value: 0x0, access type: RO
		uint32 reserved0 : 9;
	} bitFields;
} RegB1PacDelFifoErrorSticky_u;

/*REG_B1_PAC_DEL_ZLDS_TYPE_0_REG 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 zldsType0Reg : 32; //"Regular ZLDs", inserted after MPDU data according to its descriptor., reset value: 0x4E140000, access type: RW
	} bitFields;
} RegB1PacDelZldsType0Reg_u;

/*REG_B1_PAC_DEL_ZLDS_TYPE_1_REG 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 zldsType1Reg : 32; //"Special ZLDs", inserted when MPDU length in the descriptor = 0 , reset value: 0x4E140000, access type: RW
	} bitFields;
} RegB1PacDelZldsType1Reg_u;

/*REG_B1_PAC_DEL_ZLDS_TYPE_2_REG 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 zldsType2Reg : 32; //When the end of the last MPDU is not 16 bytes aligned, 2 to 4 words of this register will be appended. For any additional request from the PHY, 4 words of this register will be transmitted., reset value: 0x4E790001, access type: RW
	} bitFields;
} RegB1PacDelZldsType2Reg_u;

/*REG_B1_PAC_DEL_DESC_AF_REG 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descAfReg : 32; //AutoFill value in case that the descriptor is not available, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PacDelDescAfReg_u;

/*REG_B1_PAC_DEL_DATA_AF_REG 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dataAfReg : 32; //AutoFill value in case that the MPDU data is not available, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PacDelDataAfReg_u;

/*REG_B1_PAC_DEL_NEW_HW_STATE_MACHINES 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mainSmOut : 2; //0' - IDLE state, reset value: 0x0, access type: RO
		uint32 descCacheCtrlSmOut : 3; //0' - IDLE state, reset value: 0x0, access type: RO
		uint32 descReleaseCtrlSmOut : 3; //0' - IDLE state, reset value: 0x0, access type: RO
		uint32 rdDataSmOut : 2; //0' - IDLE state, reset value: 0x0, access type: RO
		uint32 builderSmOut : 3; //0' - IDLE state, reset value: 0x0, access type: RO
		uint32 crcSmOut : 2; //0' - IDLE state, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB1PacDelNewHwStateMachines_u;

/*REG_B1_PAC_DEL_NEW_HW_DONE 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 newHwDone : 1; //1' indicates the new HW completed its task. Cleared when a new task starts., reset value: 0x0, access type: RO
		uint32 afAllUsersDone : 1; //1' Indicates that per each user, the number of reads from the PHY data FIFO is equal to the length specified in the user TCRs. Cleared when a new task starts., reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PacDelNewHwDone_u;

/*REG_B1_PAC_DEL_DESC_AF_EVENT_STICKY_HIGH 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descAfEventStickyHigh : 4; //bit per user (along with desc_af_event_sticky_low) , '1' indicates that descriptor autofill event occurred for the specific user. , Cleared at the beginning of a new transmission.  , , reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB1PacDelDescAfEventStickyHigh_u;

/*REG_B1_PAC_DEL_DESC_AF_EVENT_STICKY_LOW 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descAfEventStickyLow : 32; //bit per user (along with desc_af_event_sticky_high) , '1' indicates that descriptor autofill event occurred for the specific user. , Cleared at the beginning of a new transmission., reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelDescAfEventStickyLow_u;

/*REG_B1_PAC_DEL_DESC_AF_EVENT_COUNT 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descAfEventCount : 24; //Counts the number of 128-bit word that were transmitted with at least 1 word of DESC_AF. Cleared at the beginning of a new transmission., reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB1PacDelDescAfEventCount_u;

/*REG_B1_PAC_DEL_FIFO_AF_STATUS 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifoAfTcr : 1; //FIFO AutoFill occurred on TCRs, reset value: 0x0, access type: RO
		uint32 fifoAfRcr : 1; //FIFO AutoFill occurred on RCRs, reset value: 0x0, access type: RO
		uint32 fifoAfData : 1; //FIFO AutoFill occurred on Data, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1PacDelFifoAfStatus_u;

/*REG_B1_PAC_DEL_BAND_IDX 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bandIdx : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1PacDelBandIdx_u;

/*REG_B1_PAC_DEL_COM_TCR0_3 0x14C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr03 : 32; //Common TCR0 bits 127:96, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelComTcr03_u;

/*REG_B1_PAC_DEL_COM_TCR0_2 0x150 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr02 : 32; //Common TCR0 bits 95:64, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelComTcr02_u;

/*REG_B1_PAC_DEL_COM_TCR0_1 0x154 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr01 : 32; //Common TCR0 bits 63:32, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelComTcr01_u;

/*REG_B1_PAC_DEL_COM_TCR0_0 0x158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr00 : 32; //Common TCR0 bits 31:0, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelComTcr00_u;

/*REG_B1_PAC_DEL_COM_TCR1_3 0x15C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr13 : 32; //Common TCR1 bits 127:96, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelComTcr13_u;

/*REG_B1_PAC_DEL_COM_TCR1_2 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr12 : 32; //Common TCR1 bits 95:64, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelComTcr12_u;

/*REG_B1_PAC_DEL_COM_TCR1_1 0x164 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr11 : 32; //Common TCR1 bits 63:32, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelComTcr11_u;

/*REG_B1_PAC_DEL_COM_TCR1_0 0x168 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr10 : 32; //Common TCR1 bits 31:0, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelComTcr10_u;

/*REG_B1_PAC_DEL_USER0_TCR_3 0x16C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 user0Tcr3 : 32; //User TCR bits 127:96, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelUser0Tcr3_u;

/*REG_B1_PAC_DEL_USER0_TCR_2 0x170 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 user0Tcr2 : 32; //User TCR bits 95:64, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelUser0Tcr2_u;

/*REG_B1_PAC_DEL_USER0_TCR_1 0x174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 user0Tcr1 : 32; //User TCR bits 63:32, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelUser0Tcr1_u;

/*REG_B1_PAC_DEL_USER0_TCR_0 0x178 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 user0Tcr0 : 32; //User TCR bits 31:0, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelUser0Tcr0_u;

/*REG_B1_PAC_DEL_DLM_PUSH_DPL_IDX_BASE 0x17C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlmPushDplIdxBase : 8; //List index for released MPDU descriptors (in normal case should be the free list), reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB1PacDelDlmPushDplIdxBase_u;

/*REG_B1_PAC_DEL_REG2HOB_GP0 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reg2HobGp0 : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PacDelReg2HobGp0_u;

/*REG_B1_PAC_DEL_REG2HOB_GP1 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reg2HobGp1 : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB1PacDelReg2HobGp1_u;

/*REG_B1_PAC_DEL_SKIP_FLOW_CONTROL 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 skipFlowControl : 1; //skip_flow_control, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PacDelSkipFlowControl_u;

/*REG_B1_PAC_DEL_DEBUG_DUP_CTRL 0x18C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 useDebugDupField : 1; //Use debug dup field, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PacDelDebugDupCtrl_u;

/*REG_B1_PAC_DEL_TCR_BASE_ADDR1 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrBaseAddr1 : 26; //TCR base address 1, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB1PacDelTcrBaseAddr1_u;

/*REG_B1_PAC_DEL_TCR_BASE_ADDR2 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrBaseAddr2 : 26; //TCR base address 2, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB1PacDelTcrBaseAddr2_u;

/*REG_B1_PAC_DEL_RCR_BASE_ADDR 0x198 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rcrBaseAddr : 26; //RCR base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB1PacDelRcrBaseAddr_u;

/*REG_B1_PAC_DEL_SENDER_MPDU_DURATION 0x19C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 senderMpduDuration : 16; //Sender MPDU duration, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PacDelSenderMpduDuration_u;

/*REG_B1_PAC_DEL_MAC_PHY_IF_SIGNALS_MONITOR 0x1A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macPhyEnable : 1; //mac_phy_enable, reset value: 0x0, access type: RO
		uint32 phyMacReady : 1; //phy_mac_ready, reset value: 0x0, access type: RO
		uint32 macPhyValid : 1; //mac_phy_valid, reset value: 0x0, access type: RO
		uint32 macPhyReadAck : 1; //mac_phy_read_ack, reset value: 0x0, access type: RO
		uint32 macPhyBusState : 4; //mac_phy_bus_state, reset value: 0x0, access type: RO
		uint32 phyMacReqId : 8; //phy_mac_req_id, reset value: 0x0, access type: RO
		uint32 phyMacReqSize : 11; //phy_mac_req_size, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 phyMacReq : 1; //phy_mac_req, reset value: 0x0, access type: RO
		uint32 phyMacRead : 1; //phy_mac_read, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelMacPhyIfSignalsMonitor_u;

/*REG_B1_PAC_DEL_START_RECIPE_CNT 0x1A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 startRecipeCnt : 32; //start recipe counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelStartRecipeCnt_u;

/*REG_B1_PAC_DEL_PHY_DATA_FIFO_AF_COUNT_WM 0x1A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoAfCountWm : 28; //phy_data_fifo_af_count_wm, reset value: 0x0, access type: RO
		uint32 phyDataFifoAfCountWmClear : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 3;
	} bitFields;
} RegB1PacDelPhyDataFifoAfCountWm_u;

/*REG_B1_PAC_DEL_PHY_DEBUG 0x1AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDebugDontOvverideTcrDur : 1; //phy_debug_dont_ovveride_tcr_dur, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PacDelPhyDebug_u;

/*REG_B1_PAC_DEL_PAC_PHY_MONITOR_CTRL 0x1B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pacPhyMonitorCnt : 28; //When pac_phy_monitor_cnt == phy_rd_cnt the data is captured (if enabled), reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 pacPhyMonitorEnable : 1; //When set to 1, capture is enabled, reset value: 0x1, access type: RW
	} bitFields;
} RegB1PacDelPacPhyMonitorCtrl_u;

/*REG_B1_PAC_DEL_PAC_PHY_MONITOR_DATA_3 0x1B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pacPhyMonitorData3 : 32; //pac_phy_monitor_data - MSB, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelPacPhyMonitorData3_u;

/*REG_B1_PAC_DEL_PAC_PHY_MONITOR_DATA_2 0x1B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pacPhyMonitorData2 : 32; //pac_phy_monitor_data_2, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelPacPhyMonitorData2_u;

/*REG_B1_PAC_DEL_PAC_PHY_MONITOR_DATA_1 0x1BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pacPhyMonitorData1 : 32; //pac_phy_monitor_data_1, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelPacPhyMonitorData1_u;

/*REG_B1_PAC_DEL_PAC_PHY_MONITOR_DATA_0 0x1C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pacPhyMonitorData0 : 32; //pac_phy_monitor_data_0, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelPacPhyMonitorData0_u;

/*REG_B1_PAC_DEL_PHY_DATA_FIFO_HOST_RD_ADDRESS 0x1C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoHostRdAddress : 9; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB1PacDelPhyDataFifoHostRdAddress_u;

/*REG_B1_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_3 0x1C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoHostRdata3 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelPhyDataFifoHostRdata3_u;

/*REG_B1_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_2 0x1CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoHostRdata2 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelPhyDataFifoHostRdata2_u;

/*REG_B1_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_1 0x1D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoHostRdata1 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelPhyDataFifoHostRdata1_u;

/*REG_B1_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_0 0x1D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoHostRdata0 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1PacDelPhyDataFifoHostRdata0_u;

/*REG_B1_PAC_DEL_FC_WR_PTR_SELECT_USER_ID 0x1D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcWrPtrSelectUserId : 10; //fc_wr_ptr_select_user_id, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB1PacDelFcWrPtrSelectUserId_u;

/*REG_B1_PAC_DEL_FC_WR_PTR_FOR_SELECTED_USER 0x1DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcWrPtrForSelectedUser : 15; //fc_wr_ptr_for_selected_user, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB1PacDelFcWrPtrForSelectedUser_u;

/*REG_B1_PAC_DEL_GOOD_MPDU_COUNT_SELECT_USER_ID 0x1E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 goodMpduCountSelectUserId : 10; //good_mpdu_count_select_user_id, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB1PacDelGoodMpduCountSelectUserId_u;

/*REG_B1_PAC_DEL_GOOD_MPDU_COUNT_FOR_SELECTED_USER 0x1E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 goodMpduCountForSelectedUser : 12; //good_mpdu_count_for_selected_user, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegB1PacDelGoodMpduCountForSelectedUser_u;

/*REG_B1_PAC_DEL_NEW_HW_SKIP_CHECK_FC 0x1E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 newHwSkipCheckFc : 1; //new_hw_skip_check_fc, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1PacDelNewHwSkipCheckFc_u;

/*REG_B1_PAC_DEL_DESC_LIST_EMPTY_HIGH 0x1EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descListEmptyHigh : 4; //desc_list_empty for users 35:32, reset value: 0xf, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB1PacDelDescListEmptyHigh_u;

/*REG_B1_PAC_DEL_DESC_LIST_EMPTY_LOW 0x1F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descListEmptyLow : 32; //desc_list_empty for users 31:0, reset value: 0xffffffff, access type: RO
	} bitFields;
} RegB1PacDelDescListEmptyLow_u;

/*REG_B1_PAC_DEL_SPARE_ECO 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareEco : 16; //Spare register for optional ECO, reset value: 0x8888, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1PacDelSpareEco_u;



#endif // _B1_PAC_DEL_REGS_H_
