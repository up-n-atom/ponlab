
/***********************************************************************************
File:				B0PacDelRegs.h
Module:				b0PacDel
SOC Revision:		latest
Generated at:       2024-06-26 12:54:26
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_PAC_DEL_REGS_H_
#define _B0_PAC_DEL_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_PAC_DEL_BASE_ADDRESS                            MEMORY_MAP_UNIT_8_BASE_ADDRESS
#define	REG_B0_PAC_DEL_BASE_0                               (B0_PAC_DEL_BASE_ADDRESS + 0x0)
#define	REG_B0_PAC_DEL_BASE_1                               (B0_PAC_DEL_BASE_ADDRESS + 0x4)
#define	REG_B0_PAC_DEL_BASE_2                               (B0_PAC_DEL_BASE_ADDRESS + 0x8)
#define	REG_B0_PAC_DEL_BASE_3                               (B0_PAC_DEL_BASE_ADDRESS + 0xC)
#define	REG_B0_PAC_DEL_BASE_4                               (B0_PAC_DEL_BASE_ADDRESS + 0x10)
#define	REG_B0_PAC_DEL_BASE_5                               (B0_PAC_DEL_BASE_ADDRESS + 0x14)
#define	REG_B0_PAC_DEL_BASE_6                               (B0_PAC_DEL_BASE_ADDRESS + 0x18)
#define	REG_B0_PAC_DEL_RETURN_ADDRESS                       (B0_PAC_DEL_BASE_ADDRESS + 0x1C)
#define	REG_B0_PAC_DEL_RECIPE_POINTER                       (B0_PAC_DEL_BASE_ADDRESS + 0x20)
#define	REG_B0_PAC_DEL_DELIA_STATUS                         (B0_PAC_DEL_BASE_ADDRESS + 0x24)
#define	REG_B0_PAC_DEL_IFS_COMPENSATION                     (B0_PAC_DEL_BASE_ADDRESS + 0x28)
#define	REG_B0_PAC_DEL_DELIA_CONTROL                        (B0_PAC_DEL_BASE_ADDRESS + 0x30)
#define	REG_B0_PAC_DEL_INDEXED_JUMP_ADDR                    (B0_PAC_DEL_BASE_ADDRESS + 0x34)
#define	REG_B0_PAC_DEL_MP_TX_DATA_WORD_COUNT                (B0_PAC_DEL_BASE_ADDRESS + 0x38)
#define	REG_B0_PAC_DEL_DELIA_LAST_INST_ADDR                 (B0_PAC_DEL_BASE_ADDRESS + 0x3C)
#define	REG_B0_PAC_DEL_AUTO_REPLY_MODE                      (B0_PAC_DEL_BASE_ADDRESS + 0x40)
#define	REG_B0_PAC_DEL_RECIPE_POINTER_DEBUG_WR              (B0_PAC_DEL_BASE_ADDRESS + 0x48)
#define	REG_B0_PAC_DEL_TX_ADDR1_31TO0                       (B0_PAC_DEL_BASE_ADDRESS + 0x4C)
#define	REG_B0_PAC_DEL_TX_ADDR1_47TO32                      (B0_PAC_DEL_BASE_ADDRESS + 0x50)
#define	REG_B0_PAC_DEL_CPU_DEL_WR_EVENTS                    (B0_PAC_DEL_BASE_ADDRESS + 0x5C)
#define	REG_B0_PAC_DEL_CPU_DEL_RD_EVENTS                    (B0_PAC_DEL_BASE_ADDRESS + 0x60)
#define	REG_B0_PAC_DEL_CPU_DEL_WR_CNTR0_EVENT               (B0_PAC_DEL_BASE_ADDRESS + 0x64)
#define	REG_B0_PAC_DEL_CPU_DEL_RD_CNTR0_EVENT               (B0_PAC_DEL_BASE_ADDRESS + 0x68)
#define	REG_B0_PAC_DEL_CPU_DEL_WR_CNTR1_EVENT               (B0_PAC_DEL_BASE_ADDRESS + 0x6C)
#define	REG_B0_PAC_DEL_CPU_DEL_RD_CNTR1_EVENT               (B0_PAC_DEL_BASE_ADDRESS + 0x70)
#define	REG_B0_PAC_DEL_LAST_CPU_DEL_CNTR_EVENT              (B0_PAC_DEL_BASE_ADDRESS + 0x74)
#define	REG_B0_PAC_DEL_LAST_RD_ADDR_TRANSFER                (B0_PAC_DEL_BASE_ADDRESS + 0x7C)
#define	REG_B0_PAC_DEL_MP_TX_TCR_RCR_WORD_COUNT             (B0_PAC_DEL_BASE_ADDRESS + 0x80)
#define	REG_B0_PAC_DEL_MP_FIFO_DEBUG                        (B0_PAC_DEL_BASE_ADDRESS + 0x84)
#define	REG_B0_PAC_DEL_TX_DATA_STRUCTURE_ADDR               (B0_PAC_DEL_BASE_ADDRESS + 0x88)
#define	REG_B0_PAC_DEL_EMU_PRBS_GCLK_BYPASS                 (B0_PAC_DEL_BASE_ADDRESS + 0x8C)
#define	REG_B0_PAC_DEL_BF_TX_MAX_THRESHOLD_TIMER            (B0_PAC_DEL_BASE_ADDRESS + 0x90)
#define	REG_B0_PAC_DEL_DEL2GENRISC_START_TX_INT             (B0_PAC_DEL_BASE_ADDRESS + 0x94)
#define	REG_B0_PAC_DEL_DEL2GENRISC_START_TX_CLR_INT         (B0_PAC_DEL_BASE_ADDRESS + 0x98)
#define	REG_B0_PAC_DEL_NOP_WAIT_EVENT_VECTOR                (B0_PAC_DEL_BASE_ADDRESS + 0x9C)
#define	REG_B0_PAC_DEL_LAST_NOP_WAIT_EVENT_VECTOR           (B0_PAC_DEL_BASE_ADDRESS + 0xA0)
#define	REG_B0_PAC_DEL_DELIA_CNTRL_REGISTER                 (B0_PAC_DEL_BASE_ADDRESS + 0xA4)
#define	REG_B0_PAC_DEL_DELIA_LOGGER_CONTROL                 (B0_PAC_DEL_BASE_ADDRESS + 0xBC)
#define	REG_B0_PAC_DEL_DELIA_LOGGER_ACTIVE                  (B0_PAC_DEL_BASE_ADDRESS + 0xC0)
#define	REG_B0_PAC_DEL_TX_END_STATUS                        (B0_PAC_DEL_BASE_ADDRESS + 0xC4)
#define	REG_B0_PAC_DEL_PHY_RD_COUNT                         (B0_PAC_DEL_BASE_ADDRESS + 0xC8)
#define	REG_B0_PAC_DEL_PHY_DATA_FIFO_WR_COUNT               (B0_PAC_DEL_BASE_ADDRESS + 0xCC)
#define	REG_B0_PAC_DEL_RECIPE_WR_CNT                        (B0_PAC_DEL_BASE_ADDRESS + 0xD0)
#define	REG_B0_PAC_DEL_PHY_DATA_FIFO_AF_COUNT               (B0_PAC_DEL_BASE_ADDRESS + 0xE0)
#define	REG_B0_PAC_DEL_PHY_DATA_FIFO_RD_COUNT               (B0_PAC_DEL_BASE_ADDRESS + 0xE4)
#define	REG_B0_PAC_DEL_TOTAL_PSDU_LENGTH_128B               (B0_PAC_DEL_BASE_ADDRESS + 0xE8)
#define	REG_B0_PAC_DEL_PHY_TOTAL_REQ_SIZE                   (B0_PAC_DEL_BASE_ADDRESS + 0xEC)
#define	REG_B0_PAC_DEL_DELIMITER_VALUE_PRE_TX_DATA          (B0_PAC_DEL_BASE_ADDRESS + 0xF4)
#define	REG_B0_PAC_DEL_DEL2GENRISC_START_TX_ENABLE_INT      (B0_PAC_DEL_BASE_ADDRESS + 0xF8)
#define	REG_B0_PAC_DEL_USER_DONE_HIGH                       (B0_PAC_DEL_BASE_ADDRESS + 0xFC)
#define	REG_B0_PAC_DEL_USER_DONE_LOW                        (B0_PAC_DEL_BASE_ADDRESS + 0x100)
#define	REG_B0_PAC_DEL_NEW_HW_FIFO_EMPTY                    (B0_PAC_DEL_BASE_ADDRESS + 0x104)
#define	REG_B0_PAC_DEL_FIFO_CLEAN                           (B0_PAC_DEL_BASE_ADDRESS + 0x108)
#define	REG_B0_PAC_DEL_NEW_HW_DESC_LISTS_STATUS_CONTROL     (B0_PAC_DEL_BASE_ADDRESS + 0x110)
#define	REG_B0_PAC_DEL_PHY_DATA_FIFO_AF_TH                  (B0_PAC_DEL_BASE_ADDRESS + 0x114)
#define	REG_B0_PAC_DEL_FIFO_ERROR_STICKY                    (B0_PAC_DEL_BASE_ADDRESS + 0x118)
#define	REG_B0_PAC_DEL_ZLDS_TYPE_0_REG                      (B0_PAC_DEL_BASE_ADDRESS + 0x11C)
#define	REG_B0_PAC_DEL_ZLDS_TYPE_1_REG                      (B0_PAC_DEL_BASE_ADDRESS + 0x120)
#define	REG_B0_PAC_DEL_ZLDS_TYPE_2_REG                      (B0_PAC_DEL_BASE_ADDRESS + 0x124)
#define	REG_B0_PAC_DEL_DESC_AF_REG                          (B0_PAC_DEL_BASE_ADDRESS + 0x128)
#define	REG_B0_PAC_DEL_DATA_AF_REG                          (B0_PAC_DEL_BASE_ADDRESS + 0x12C)
#define	REG_B0_PAC_DEL_NEW_HW_STATE_MACHINES                (B0_PAC_DEL_BASE_ADDRESS + 0x130)
#define	REG_B0_PAC_DEL_NEW_HW_DONE                          (B0_PAC_DEL_BASE_ADDRESS + 0x134)
#define	REG_B0_PAC_DEL_DESC_AF_EVENT_STICKY_HIGH            (B0_PAC_DEL_BASE_ADDRESS + 0x138)
#define	REG_B0_PAC_DEL_DESC_AF_EVENT_STICKY_LOW             (B0_PAC_DEL_BASE_ADDRESS + 0x13C)
#define	REG_B0_PAC_DEL_DESC_AF_EVENT_COUNT                  (B0_PAC_DEL_BASE_ADDRESS + 0x140)
#define	REG_B0_PAC_DEL_FIFO_AF_STATUS                       (B0_PAC_DEL_BASE_ADDRESS + 0x144)
#define	REG_B0_PAC_DEL_BAND_IDX                             (B0_PAC_DEL_BASE_ADDRESS + 0x148)
#define	REG_B0_PAC_DEL_COM_TCR0_3                           (B0_PAC_DEL_BASE_ADDRESS + 0x14C)
#define	REG_B0_PAC_DEL_COM_TCR0_2                           (B0_PAC_DEL_BASE_ADDRESS + 0x150)
#define	REG_B0_PAC_DEL_COM_TCR0_1                           (B0_PAC_DEL_BASE_ADDRESS + 0x154)
#define	REG_B0_PAC_DEL_COM_TCR0_0                           (B0_PAC_DEL_BASE_ADDRESS + 0x158)
#define	REG_B0_PAC_DEL_COM_TCR1_3                           (B0_PAC_DEL_BASE_ADDRESS + 0x15C)
#define	REG_B0_PAC_DEL_COM_TCR1_2                           (B0_PAC_DEL_BASE_ADDRESS + 0x160)
#define	REG_B0_PAC_DEL_COM_TCR1_1                           (B0_PAC_DEL_BASE_ADDRESS + 0x164)
#define	REG_B0_PAC_DEL_COM_TCR1_0                           (B0_PAC_DEL_BASE_ADDRESS + 0x168)
#define	REG_B0_PAC_DEL_USER0_TCR_3                          (B0_PAC_DEL_BASE_ADDRESS + 0x16C)
#define	REG_B0_PAC_DEL_USER0_TCR_2                          (B0_PAC_DEL_BASE_ADDRESS + 0x170)
#define	REG_B0_PAC_DEL_USER0_TCR_1                          (B0_PAC_DEL_BASE_ADDRESS + 0x174)
#define	REG_B0_PAC_DEL_USER0_TCR_0                          (B0_PAC_DEL_BASE_ADDRESS + 0x178)
#define	REG_B0_PAC_DEL_DLM_PUSH_DPL_IDX_BASE                (B0_PAC_DEL_BASE_ADDRESS + 0x17C)
#define	REG_B0_PAC_DEL_REG2HOB_GP0                          (B0_PAC_DEL_BASE_ADDRESS + 0x180)
#define	REG_B0_PAC_DEL_REG2HOB_GP1                          (B0_PAC_DEL_BASE_ADDRESS + 0x184)
#define	REG_B0_PAC_DEL_SKIP_FLOW_CONTROL                    (B0_PAC_DEL_BASE_ADDRESS + 0x188)
#define	REG_B0_PAC_DEL_DEBUG_DUP_CTRL                       (B0_PAC_DEL_BASE_ADDRESS + 0x18C)
#define	REG_B0_PAC_DEL_TCR_BASE_ADDR1                       (B0_PAC_DEL_BASE_ADDRESS + 0x190)
#define	REG_B0_PAC_DEL_TCR_BASE_ADDR2                       (B0_PAC_DEL_BASE_ADDRESS + 0x194)
#define	REG_B0_PAC_DEL_RCR_BASE_ADDR                        (B0_PAC_DEL_BASE_ADDRESS + 0x198)
#define	REG_B0_PAC_DEL_SENDER_MPDU_DURATION                 (B0_PAC_DEL_BASE_ADDRESS + 0x19C)
#define	REG_B0_PAC_DEL_MAC_PHY_IF_SIGNALS_MONITOR           (B0_PAC_DEL_BASE_ADDRESS + 0x1A0)
#define	REG_B0_PAC_DEL_START_RECIPE_CNT                     (B0_PAC_DEL_BASE_ADDRESS + 0x1A4)
#define	REG_B0_PAC_DEL_PHY_DATA_FIFO_AF_COUNT_WM            (B0_PAC_DEL_BASE_ADDRESS + 0x1A8)
#define	REG_B0_PAC_DEL_PHY_DEBUG                            (B0_PAC_DEL_BASE_ADDRESS + 0x1AC)
#define	REG_B0_PAC_DEL_PAC_PHY_MONITOR_CTRL                 (B0_PAC_DEL_BASE_ADDRESS + 0x1B0)
#define	REG_B0_PAC_DEL_PAC_PHY_MONITOR_DATA_3               (B0_PAC_DEL_BASE_ADDRESS + 0x1B4)
#define	REG_B0_PAC_DEL_PAC_PHY_MONITOR_DATA_2               (B0_PAC_DEL_BASE_ADDRESS + 0x1B8)
#define	REG_B0_PAC_DEL_PAC_PHY_MONITOR_DATA_1               (B0_PAC_DEL_BASE_ADDRESS + 0x1BC)
#define	REG_B0_PAC_DEL_PAC_PHY_MONITOR_DATA_0               (B0_PAC_DEL_BASE_ADDRESS + 0x1C0)
#define	REG_B0_PAC_DEL_PHY_DATA_FIFO_HOST_RD_ADDRESS        (B0_PAC_DEL_BASE_ADDRESS + 0x1C4)
#define	REG_B0_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_3           (B0_PAC_DEL_BASE_ADDRESS + 0x1C8)
#define	REG_B0_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_2           (B0_PAC_DEL_BASE_ADDRESS + 0x1CC)
#define	REG_B0_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_1           (B0_PAC_DEL_BASE_ADDRESS + 0x1D0)
#define	REG_B0_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_0           (B0_PAC_DEL_BASE_ADDRESS + 0x1D4)
#define	REG_B0_PAC_DEL_FC_WR_PTR_SELECT_USER_ID             (B0_PAC_DEL_BASE_ADDRESS + 0x1D8)
#define	REG_B0_PAC_DEL_FC_WR_PTR_FOR_SELECTED_USER          (B0_PAC_DEL_BASE_ADDRESS + 0x1DC)
#define	REG_B0_PAC_DEL_GOOD_MPDU_COUNT_SELECT_USER_ID       (B0_PAC_DEL_BASE_ADDRESS + 0x1E0)
#define	REG_B0_PAC_DEL_GOOD_MPDU_COUNT_FOR_SELECTED_USER    (B0_PAC_DEL_BASE_ADDRESS + 0x1E4)
#define	REG_B0_PAC_DEL_NEW_HW_SKIP_CHECK_FC                 (B0_PAC_DEL_BASE_ADDRESS + 0x1E8)
#define	REG_B0_PAC_DEL_DESC_LIST_EMPTY_HIGH                 (B0_PAC_DEL_BASE_ADDRESS + 0x1EC)
#define	REG_B0_PAC_DEL_DESC_LIST_EMPTY_LOW                  (B0_PAC_DEL_BASE_ADDRESS + 0x1F0)
#define	REG_B0_PAC_DEL_SPARE_ECO                            (B0_PAC_DEL_BASE_ADDRESS + 0x1FC)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_PAC_DEL_BASE_0 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 base0 : 32; //DEL  base 0, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelBase0_u;

/*REG_B0_PAC_DEL_BASE_1 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 base1 : 32; //DEL  base 1, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelBase1_u;

/*REG_B0_PAC_DEL_BASE_2 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 base2 : 32; //DEL  base 2, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelBase2_u;

/*REG_B0_PAC_DEL_BASE_3 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 base3 : 32; //DEL  base 3, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelBase3_u;

/*REG_B0_PAC_DEL_BASE_4 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 base4 : 32; //DEL  base 4, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelBase4_u;

/*REG_B0_PAC_DEL_BASE_5 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 base5 : 32; //DEL  base 5, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelBase5_u;

/*REG_B0_PAC_DEL_BASE_6 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 base6 : 32; //DEL  base 6, reset value: 0x400000, access type: RO
	} bitFields;
} RegB0PacDelBase6_u;

/*REG_B0_PAC_DEL_RETURN_ADDRESS 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 returnAddress : 24; //DEL  return address, reset value: 0x0, access type: RO
		uint32 reserved1 : 6;
	} bitFields;
} RegB0PacDelReturnAddress_u;

/*REG_B0_PAC_DEL_RECIPE_POINTER 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 recipePointer : 24; //DEL  recipe pointer, reset value: 0x0, access type: RO
		uint32 reserved1 : 6;
	} bitFields;
} RegB0PacDelRecipePointer_u;

/*REG_B0_PAC_DEL_DELIA_STATUS 0x24 */
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
} RegB0PacDelDeliaStatus_u;

/*REG_B0_PAC_DEL_IFS_COMPENSATION 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ifsCompensationOfdm : 16; //DEL ifs compensation when the last Tx PHY mode is OFDM, reset value: 0x0, access type: RW
		uint32 ifsCompensation11B : 16; //DEL ifs compensation when the last Tx PHY mode is 11B, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PacDelIfsCompensation_u;

/*REG_B0_PAC_DEL_DELIA_CONTROL 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyAutoStart : 1; //Phy auto start, reset value: 0x1, access type: RW
		uint32 tcrTxopDurationUnspecifiedEn : 1; //Enable setting TCR TXOP duration unspecified value for Auto-reply and BF report frames, reset value: 0x1, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PacDelDeliaControl_u;

/*REG_B0_PAC_DEL_INDEXED_JUMP_ADDR 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 indexedJumpAddr : 24; //Indexed jump address, reset value: 0x0, access type: RO
		uint32 reserved1 : 6;
	} bitFields;
} RegB0PacDelIndexedJumpAddr_u;

/*REG_B0_PAC_DEL_MP_TX_DATA_WORD_COUNT 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mpTxDataWordCtr : 18; //Tx data word count, reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB0PacDelMpTxDataWordCount_u;

/*REG_B0_PAC_DEL_DELIA_LAST_INST_ADDR 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastFetchedInstruction : 32; //Last fetched instruction, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelDeliaLastInstAddr_u;

/*REG_B0_PAC_DEL_AUTO_REPLY_MODE 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 autoPowerMode : 1; //Auto Tx power mode for auto frames, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PacDelAutoReplyMode_u;

/*REG_B0_PAC_DEL_RECIPE_POINTER_DEBUG_WR 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 2;
		uint32 recipePointerDebugWr : 24; //DEL  write debug recipe pointer, reset value: 0x0, access type: WO
		uint32 reserved1 : 6;
	} bitFields;
} RegB0PacDelRecipePointerDebugWr_u;

/*REG_B0_PAC_DEL_TX_ADDR1_31TO0 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAddr131To0 : 32; //tx addr1 latch 31to0, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelTxAddr131To0_u;

/*REG_B0_PAC_DEL_TX_ADDR1_47TO32 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txAddr147To32 : 16; //tx addr1 latch 47to32, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PacDelTxAddr147To32_u;

/*REG_B0_PAC_DEL_CPU_DEL_WR_EVENTS 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuDelWrEvents : 8; //CPU delia wr events, reset value: 0x0, access type: WO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PacDelCpuDelWrEvents_u;

/*REG_B0_PAC_DEL_CPU_DEL_RD_EVENTS 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuDelRdEvents : 8; //CPU delia rd events, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PacDelCpuDelRdEvents_u;

/*REG_B0_PAC_DEL_CPU_DEL_WR_CNTR0_EVENT 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuDelWrCntr0Event : 12; //CPU delia wr event counter0, reset value: 0x0, access type: WO
		uint32 reserved0 : 20;
	} bitFields;
} RegB0PacDelCpuDelWrCntr0Event_u;

/*REG_B0_PAC_DEL_CPU_DEL_RD_CNTR0_EVENT 0x68 */
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
} RegB0PacDelCpuDelRdCntr0Event_u;

/*REG_B0_PAC_DEL_CPU_DEL_WR_CNTR1_EVENT 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuDelWrCntr1Event : 12; //CPU delia wr event counter1, reset value: 0x0, access type: WO
		uint32 reserved0 : 20;
	} bitFields;
} RegB0PacDelCpuDelWrCntr1Event_u;

/*REG_B0_PAC_DEL_CPU_DEL_RD_CNTR1_EVENT 0x70 */
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
} RegB0PacDelCpuDelRdCntr1Event_u;

/*REG_B0_PAC_DEL_LAST_CPU_DEL_CNTR_EVENT 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastCpuDelCntr0Event : 12; //last CPU delia event counter0, reset value: 0x0, access type: RO
		uint32 lastCpuDelCntr1Event : 12; //last CPU delia event counter1, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PacDelLastCpuDelCntrEvent_u;

/*REG_B0_PAC_DEL_LAST_RD_ADDR_TRANSFER 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastRdAddrTransfer : 16; //last rd ShRAM addr transfer, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PacDelLastRdAddrTransfer_u;

/*REG_B0_PAC_DEL_MP_TX_TCR_RCR_WORD_COUNT 0x80 */
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
} RegB0PacDelMpTxTcrRcrWordCount_u;

/*REG_B0_PAC_DEL_MP_FIFO_DEBUG 0x84 */
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
} RegB0PacDelMpFifoDebug_u;

/*REG_B0_PAC_DEL_TX_DATA_STRUCTURE_ADDR 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDataStructureAddr : 26; //Tx data structure addr, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PacDelTxDataStructureAddr_u;

/*REG_B0_PAC_DEL_EMU_PRBS_GCLK_BYPASS 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emuPrbsGclkBypass : 1; //PRBS gclk bypass, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PacDelEmuPrbsGclkBypass_u;

/*REG_B0_PAC_DEL_BF_TX_MAX_THRESHOLD_TIMER 0x90 */
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
} RegB0PacDelBfTxMaxThresholdTimer_u;

/*REG_B0_PAC_DEL_DEL2GENRISC_START_TX_INT 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 del2GenriscStartTxInt : 6; //Delia to GenRisc interrupts, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegB0PacDelDel2GenriscStartTxInt_u;

/*REG_B0_PAC_DEL_DEL2GENRISC_START_TX_CLR_INT 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 del2GenriscStartTxClrInt : 6; //Delia to GenRisc clear interrupts, reset value: 0x0, access type: WO
		uint32 reserved0 : 26;
	} bitFields;
} RegB0PacDelDel2GenriscStartTxClrInt_u;

/*REG_B0_PAC_DEL_NOP_WAIT_EVENT_VECTOR 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 nopWaitEventVector : 22; //nop wait event vector, reset value: 0x40c1, access type: RO
		uint32 reserved0 : 10;
	} bitFields;
} RegB0PacDelNopWaitEventVector_u;

/*REG_B0_PAC_DEL_LAST_NOP_WAIT_EVENT_VECTOR 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastNopWaitEventVector : 22; //Last NOP wait event vector, reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
	} bitFields;
} RegB0PacDelLastNopWaitEventVector_u;

/*REG_B0_PAC_DEL_DELIA_CNTRL_REGISTER 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 enableEarlyAfRecovery : 1; //Enable AF recovery mechanism, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PacDelDeliaCntrlRegister_u;

/*REG_B0_PAC_DEL_DELIA_LOGGER_CONTROL 0xBC */
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
} RegB0PacDelDeliaLoggerControl_u;

/*REG_B0_PAC_DEL_DELIA_LOGGER_ACTIVE 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 deliaLoggerActive : 1; //Delia logger active, reset value: 0x0, access type: RO
		uint32 deliaAfLoggerActive : 1; //Delia AF logger active, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PacDelDeliaLoggerActive_u;

/*REG_B0_PAC_DEL_TX_END_STATUS 0xC4 */
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
} RegB0PacDelTxEndStatus_u;

/*REG_B0_PAC_DEL_PHY_RD_COUNT 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyRdCount : 32; //Counts the number of read transactions from the PHY to the data FIFO. , Cleared on the rising edge of the PHY enable signal. , reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelPhyRdCount_u;

/*REG_B0_PAC_DEL_PHY_DATA_FIFO_WR_COUNT 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoWrCount : 32; //Counts the number of 128-bit words that were pushed into the PHY data FIFO. , Cleared on the rising edge of the PHY enable signal., reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelPhyDataFifoWrCount_u;

/*REG_B0_PAC_DEL_RECIPE_WR_CNT 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 recipeWrCnt : 32; //Recipe byte write count, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelRecipeWrCnt_u;

/*REG_B0_PAC_DEL_PHY_DATA_FIFO_AF_COUNT 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoAfCount : 32; //Counts the number of 128-bit words that were auto-filled due to PHY data FIFO empty. , Cleared on the rising edge of the PHY enable signal., reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelPhyDataFifoAfCount_u;

/*REG_B0_PAC_DEL_PHY_DATA_FIFO_RD_COUNT 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoRdCount : 32; //Counts the number of 128-bit words that were pulled from the PHY data FIFO. , Cleared on the rising edge of the PHY enable signal., reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelPhyDataFifoRdCount_u;

/*REG_B0_PAC_DEL_TOTAL_PSDU_LENGTH_128B 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 totalPsduLength128B : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelTotalPsduLength128B_u;

/*REG_B0_PAC_DEL_PHY_TOTAL_REQ_SIZE 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyTotalReqSize : 32; //Total request size (all users), reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelPhyTotalReqSize_u;

/*REG_B0_PAC_DEL_DELIMITER_VALUE_PRE_TX_DATA 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 delimiterValuePreTxData : 32; //Delimiter value in case need to transmit at the start of the PSDU,  , Can be read by HOB., reset value: 0x4e0000f0, access type: RW
	} bitFields;
} RegB0PacDelDelimiterValuePreTxData_u;

/*REG_B0_PAC_DEL_DEL2GENRISC_START_TX_ENABLE_INT 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 del2GenriscStartTxEnableInt : 6; //Delia to GenRisc enable interrupts, reset value: 0x3f, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB0PacDelDel2GenriscStartTxEnableInt_u;

/*REG_B0_PAC_DEL_USER_DONE_HIGH 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userDoneHigh : 4; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PacDelUserDoneHigh_u;

/*REG_B0_PAC_DEL_USER_DONE_LOW 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userDoneLow : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelUserDoneLow_u;

/*REG_B0_PAC_DEL_NEW_HW_FIFO_EMPTY 0x104 */
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
} RegB0PacDelNewHwFifoEmpty_u;

/*REG_B0_PAC_DEL_FIFO_CLEAN 0x108 */
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
} RegB0PacDelFifoClean_u;

/*REG_B0_PAC_DEL_NEW_HW_DESC_LISTS_STATUS_CONTROL 0x110 */
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
} RegB0PacDelNewHwDescListsStatusControl_u;

/*REG_B0_PAC_DEL_PHY_DATA_FIFO_AF_TH 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoAfTh : 7; //Sets the threshold of the PHY data FIFO for autofill., reset value: 0x4, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB0PacDelPhyDataFifoAfTh_u;

/*REG_B0_PAC_DEL_FIFO_ERROR_STICKY 0x118 */
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
} RegB0PacDelFifoErrorSticky_u;

/*REG_B0_PAC_DEL_ZLDS_TYPE_0_REG 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 zldsType0Reg : 32; //"Regular ZLDs------, inserted after MPDU data according to its descriptor., reset value: 0x4E140000, access type: RW
	} bitFields;
} RegB0PacDelZldsType0Reg_u;

/*REG_B0_PAC_DEL_ZLDS_TYPE_1_REG 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 zldsType1Reg : 32; //---Special ZLDs---, inserted when MPDU length in the descriptor = 0 , reset value: 0x4E140000, access type: RW
	} bitFields;
} RegB0PacDelZldsType1Reg_u;

/*REG_B0_PAC_DEL_ZLDS_TYPE_2_REG 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 zldsType2Reg : 32; //When the end of the last MPDU is not 16 bytes aligned, 2 to 4 words of this register will be appended. For any additional request from the PHY, 4 words of this register will be transmitted., reset value: 0x4E790001, access type: RW
	} bitFields;
} RegB0PacDelZldsType2Reg_u;

/*REG_B0_PAC_DEL_DESC_AF_REG 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descAfReg : 32; //AutoFill value in case that the descriptor is not available, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PacDelDescAfReg_u;

/*REG_B0_PAC_DEL_DATA_AF_REG 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dataAfReg : 32; //AutoFill value in case that the MPDU data is not available, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PacDelDataAfReg_u;

/*REG_B0_PAC_DEL_NEW_HW_STATE_MACHINES 0x130 */
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
} RegB0PacDelNewHwStateMachines_u;

/*REG_B0_PAC_DEL_NEW_HW_DONE 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 newHwDone : 1; //1' indicates the new HW completed its task. Cleared when a new task starts., reset value: 0x0, access type: RO
		uint32 afAllUsersDone : 1; //1' Indicates that per each user, the number of reads from the PHY data FIFO is equal to the length specified in the user TCRs. Cleared when a new task starts., reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PacDelNewHwDone_u;

/*REG_B0_PAC_DEL_DESC_AF_EVENT_STICKY_HIGH 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descAfEventStickyHigh : 4; //bit per user (along with desc_af_event_sticky_low) , '1' indicates that descriptor autofill event occurred for the specific user. , Cleared at the beginning of a new transmission.  , , reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PacDelDescAfEventStickyHigh_u;

/*REG_B0_PAC_DEL_DESC_AF_EVENT_STICKY_LOW 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descAfEventStickyLow : 32; //bit per user (along with desc_af_event_sticky_high) , '1' indicates that descriptor autofill event occurred for the specific user. , Cleared at the beginning of a new transmission., reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelDescAfEventStickyLow_u;

/*REG_B0_PAC_DEL_DESC_AF_EVENT_COUNT 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descAfEventCount : 24; //Counts the number of 128-bit word that were transmitted with at least 1 word of DESC_AF. Cleared at the beginning of a new transmission., reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB0PacDelDescAfEventCount_u;

/*REG_B0_PAC_DEL_FIFO_AF_STATUS 0x144 */
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
} RegB0PacDelFifoAfStatus_u;

/*REG_B0_PAC_DEL_BAND_IDX 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bandIdx : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0PacDelBandIdx_u;

/*REG_B0_PAC_DEL_COM_TCR0_3 0x14C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr03 : 32; //Common TCR0 bits 127:96, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelComTcr03_u;

/*REG_B0_PAC_DEL_COM_TCR0_2 0x150 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr02 : 32; //Common TCR0 bits 95:64, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelComTcr02_u;

/*REG_B0_PAC_DEL_COM_TCR0_1 0x154 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr01 : 32; //Common TCR0 bits 63:32, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelComTcr01_u;

/*REG_B0_PAC_DEL_COM_TCR0_0 0x158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr00 : 32; //Common TCR0 bits 31:0, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelComTcr00_u;

/*REG_B0_PAC_DEL_COM_TCR1_3 0x15C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr13 : 32; //Common TCR1 bits 127:96, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelComTcr13_u;

/*REG_B0_PAC_DEL_COM_TCR1_2 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr12 : 32; //Common TCR1 bits 95:64, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelComTcr12_u;

/*REG_B0_PAC_DEL_COM_TCR1_1 0x164 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr11 : 32; //Common TCR1 bits 63:32, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelComTcr11_u;

/*REG_B0_PAC_DEL_COM_TCR1_0 0x168 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 comTcr10 : 32; //Common TCR1 bits 31:0, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelComTcr10_u;

/*REG_B0_PAC_DEL_USER0_TCR_3 0x16C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 user0Tcr3 : 32; //User TCR bits 127:96, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelUser0Tcr3_u;

/*REG_B0_PAC_DEL_USER0_TCR_2 0x170 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 user0Tcr2 : 32; //User TCR bits 95:64, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelUser0Tcr2_u;

/*REG_B0_PAC_DEL_USER0_TCR_1 0x174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 user0Tcr1 : 32; //User TCR bits 63:32, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelUser0Tcr1_u;

/*REG_B0_PAC_DEL_USER0_TCR_0 0x178 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 user0Tcr0 : 32; //User TCR bits 31:0, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelUser0Tcr0_u;

/*REG_B0_PAC_DEL_DLM_PUSH_DPL_IDX_BASE 0x17C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlmPushDplIdxBase : 8; //List index for released MPDU descriptors (in normal case should be the free list), reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0PacDelDlmPushDplIdxBase_u;

/*REG_B0_PAC_DEL_REG2HOB_GP0 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reg2HobGp0 : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PacDelReg2HobGp0_u;

/*REG_B0_PAC_DEL_REG2HOB_GP1 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reg2HobGp1 : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB0PacDelReg2HobGp1_u;

/*REG_B0_PAC_DEL_SKIP_FLOW_CONTROL 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 skipFlowControl : 1; //skip_flow_control, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PacDelSkipFlowControl_u;

/*REG_B0_PAC_DEL_DEBUG_DUP_CTRL 0x18C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 useDebugDupField : 1; //Use debug dup field, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PacDelDebugDupCtrl_u;

/*REG_B0_PAC_DEL_TCR_BASE_ADDR1 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrBaseAddr1 : 26; //TCR base address 1, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PacDelTcrBaseAddr1_u;

/*REG_B0_PAC_DEL_TCR_BASE_ADDR2 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tcrBaseAddr2 : 26; //TCR base address 2, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PacDelTcrBaseAddr2_u;

/*REG_B0_PAC_DEL_RCR_BASE_ADDR 0x198 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rcrBaseAddr : 26; //RCR base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
	} bitFields;
} RegB0PacDelRcrBaseAddr_u;

/*REG_B0_PAC_DEL_SENDER_MPDU_DURATION 0x19C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 senderMpduDuration : 16; //Sender MPDU duration, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PacDelSenderMpduDuration_u;

/*REG_B0_PAC_DEL_MAC_PHY_IF_SIGNALS_MONITOR 0x1A0 */
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
} RegB0PacDelMacPhyIfSignalsMonitor_u;

/*REG_B0_PAC_DEL_START_RECIPE_CNT 0x1A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 startRecipeCnt : 32; //start recipe counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelStartRecipeCnt_u;

/*REG_B0_PAC_DEL_PHY_DATA_FIFO_AF_COUNT_WM 0x1A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoAfCountWm : 28; //phy_data_fifo_af_count_wm, reset value: 0x0, access type: RO
		uint32 phyDataFifoAfCountWmClear : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 3;
	} bitFields;
} RegB0PacDelPhyDataFifoAfCountWm_u;

/*REG_B0_PAC_DEL_PHY_DEBUG 0x1AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDebugDontOvverideTcrDur : 1; //phy_debug_dont_ovveride_tcr_dur, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PacDelPhyDebug_u;

/*REG_B0_PAC_DEL_PAC_PHY_MONITOR_CTRL 0x1B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pacPhyMonitorCnt : 28; //When pac_phy_monitor_cnt == phy_rd_cnt the data is captured (if enabled), reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 pacPhyMonitorEnable : 1; //When set to 1, capture is enabled, reset value: 0x1, access type: RW
	} bitFields;
} RegB0PacDelPacPhyMonitorCtrl_u;

/*REG_B0_PAC_DEL_PAC_PHY_MONITOR_DATA_3 0x1B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pacPhyMonitorData3 : 32; //pac_phy_monitor_data - MSB, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelPacPhyMonitorData3_u;

/*REG_B0_PAC_DEL_PAC_PHY_MONITOR_DATA_2 0x1B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pacPhyMonitorData2 : 32; //pac_phy_monitor_data_2, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelPacPhyMonitorData2_u;

/*REG_B0_PAC_DEL_PAC_PHY_MONITOR_DATA_1 0x1BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pacPhyMonitorData1 : 32; //pac_phy_monitor_data_1, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelPacPhyMonitorData1_u;

/*REG_B0_PAC_DEL_PAC_PHY_MONITOR_DATA_0 0x1C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pacPhyMonitorData0 : 32; //pac_phy_monitor_data_0, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelPacPhyMonitorData0_u;

/*REG_B0_PAC_DEL_PHY_DATA_FIFO_HOST_RD_ADDRESS 0x1C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoHostRdAddress : 9; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB0PacDelPhyDataFifoHostRdAddress_u;

/*REG_B0_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_3 0x1C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoHostRdata3 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelPhyDataFifoHostRdata3_u;

/*REG_B0_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_2 0x1CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoHostRdata2 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelPhyDataFifoHostRdata2_u;

/*REG_B0_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_1 0x1D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoHostRdata1 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelPhyDataFifoHostRdata1_u;

/*REG_B0_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_0 0x1D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyDataFifoHostRdata0 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0PacDelPhyDataFifoHostRdata0_u;

/*REG_B0_PAC_DEL_FC_WR_PTR_SELECT_USER_ID 0x1D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcWrPtrSelectUserId : 10; //fc_wr_ptr_select_user_id, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB0PacDelFcWrPtrSelectUserId_u;

/*REG_B0_PAC_DEL_FC_WR_PTR_FOR_SELECTED_USER 0x1DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcWrPtrForSelectedUser : 15; //fc_wr_ptr_for_selected_user, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
	} bitFields;
} RegB0PacDelFcWrPtrForSelectedUser_u;

/*REG_B0_PAC_DEL_GOOD_MPDU_COUNT_SELECT_USER_ID 0x1E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 goodMpduCountSelectUserId : 10; //good_mpdu_count_select_user_id, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB0PacDelGoodMpduCountSelectUserId_u;

/*REG_B0_PAC_DEL_GOOD_MPDU_COUNT_FOR_SELECTED_USER 0x1E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 goodMpduCountForSelectedUser : 12; //good_mpdu_count_for_selected_user, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegB0PacDelGoodMpduCountForSelectedUser_u;

/*REG_B0_PAC_DEL_NEW_HW_SKIP_CHECK_FC 0x1E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 newHwSkipCheckFc : 1; //new_hw_skip_check_fc, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0PacDelNewHwSkipCheckFc_u;

/*REG_B0_PAC_DEL_DESC_LIST_EMPTY_HIGH 0x1EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descListEmptyHigh : 4; //desc_list_empty for users 35:32, reset value: 0xf, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0PacDelDescListEmptyHigh_u;

/*REG_B0_PAC_DEL_DESC_LIST_EMPTY_LOW 0x1F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descListEmptyLow : 32; //desc_list_empty for users 31:0, reset value: 0xffffffff, access type: RO
	} bitFields;
} RegB0PacDelDescListEmptyLow_u;

/*REG_B0_PAC_DEL_SPARE_ECO 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareEco : 16; //Spare register for optional ECO, reset value: 0x8888, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0PacDelSpareEco_u;

//========================================
/* REG_PAC_DEL_BASE_0 0x0 */
#define B0_PAC_DEL_BASE_0_FIELD_BASE_0_MASK                                                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_BASE_1 0x4 */
#define B0_PAC_DEL_BASE_1_FIELD_BASE_1_MASK                                                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_BASE_2 0x8 */
#define B0_PAC_DEL_BASE_2_FIELD_BASE_2_MASK                                                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_BASE_3 0xC */
#define B0_PAC_DEL_BASE_3_FIELD_BASE_3_MASK                                                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_BASE_4 0x10 */
#define B0_PAC_DEL_BASE_4_FIELD_BASE_4_MASK                                                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_BASE_5 0x14 */
#define B0_PAC_DEL_BASE_5_FIELD_BASE_5_MASK                                                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_BASE_6 0x18 */
#define B0_PAC_DEL_BASE_6_FIELD_BASE_6_MASK                                                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_RETURN_ADDRESS 0x1C */
#define B0_PAC_DEL_RETURN_ADDRESS_FIELD_RETURN_ADDRESS_MASK                                                       0x03FFFFFC
//========================================
/* REG_PAC_DEL_RECIPE_POINTER 0x20 */
#define B0_PAC_DEL_RECIPE_POINTER_FIELD_RECIPE_POINTER_MASK                                                       0x03FFFFFC
//========================================
/* REG_PAC_DEL_DELIA_STATUS 0x24 */
#define B0_PAC_DEL_DELIA_STATUS_FIELD_DELIA_STATUS_BUSY_MASK                                                      0x00000001
#define B0_PAC_DEL_DELIA_STATUS_FIELD_DELIA_STATE_MASK                                                            0x000000F0
//========================================
/* REG_PAC_DEL_IFS_COMPENSATION 0x28 */
#define B0_PAC_DEL_IFS_COMPENSATION_FIELD_IFS_COMPENSATION_OFDM_MASK                                              0x0000FFFF
#define B0_PAC_DEL_IFS_COMPENSATION_FIELD_IFS_COMPENSATION_11B_MASK                                               0xFFFF0000
//========================================
/* REG_PAC_DEL_DELIA_CONTROL 0x30 */
#define B0_PAC_DEL_DELIA_CONTROL_FIELD_PHY_AUTO_START_MASK                                                        0x00000001
#define B0_PAC_DEL_DELIA_CONTROL_FIELD_TCR_TXOP_DURATION_UNSPECIFIED_EN_MASK                                      0x00000002
//========================================
/* REG_PAC_DEL_INDEXED_JUMP_ADDR 0x34 */
#define B0_PAC_DEL_INDEXED_JUMP_ADDR_FIELD_INDEXED_JUMP_ADDR_MASK                                                 0x03FFFFFC
//========================================
/* REG_PAC_DEL_MP_TX_DATA_WORD_COUNT 0x38 */
#define B0_PAC_DEL_MP_TX_DATA_WORD_COUNT_FIELD_MP_TX_DATA_WORD_CTR_MASK                                           0x0003FFFF
//========================================
/* REG_PAC_DEL_DELIA_LAST_INST_ADDR 0x3C */
#define B0_PAC_DEL_DELIA_LAST_INST_ADDR_FIELD_LAST_FETCHED_INSTRUCTION_MASK                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_AUTO_REPLY_MODE 0x40 */
#define B0_PAC_DEL_AUTO_REPLY_MODE_FIELD_AUTO_POWER_MODE_MASK                                                     0x00000001
//========================================
/* REG_PAC_DEL_RECIPE_POINTER_DEBUG_WR 0x48 */
#define B0_PAC_DEL_RECIPE_POINTER_DEBUG_WR_FIELD_RECIPE_POINTER_DEBUG_WR_MASK                                     0x03FFFFFC
//========================================
/* REG_PAC_DEL_TX_ADDR1_31TO0 0x4C */
#define B0_PAC_DEL_TX_ADDR1_31TO0_FIELD_TX_ADDR1_31TO0_MASK                                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_TX_ADDR1_47TO32 0x50 */
#define B0_PAC_DEL_TX_ADDR1_47TO32_FIELD_TX_ADDR1_47TO32_MASK                                                     0x0000FFFF
//========================================
/* REG_PAC_DEL_CPU_DEL_WR_EVENTS 0x5C */
#define B0_PAC_DEL_CPU_DEL_WR_EVENTS_FIELD_CPU_DEL_WR_EVENTS_MASK                                                 0x000000FF
//========================================
/* REG_PAC_DEL_CPU_DEL_RD_EVENTS 0x60 */
#define B0_PAC_DEL_CPU_DEL_RD_EVENTS_FIELD_CPU_DEL_RD_EVENTS_MASK                                                 0x000000FF
//========================================
/* REG_PAC_DEL_CPU_DEL_WR_CNTR0_EVENT 0x64 */
#define B0_PAC_DEL_CPU_DEL_WR_CNTR0_EVENT_FIELD_CPU_DEL_WR_CNTR0_EVENT_MASK                                       0x00000FFF
//========================================
/* REG_PAC_DEL_CPU_DEL_RD_CNTR0_EVENT 0x68 */
#define B0_PAC_DEL_CPU_DEL_RD_CNTR0_EVENT_FIELD_CPU_DEL_RD_CNTR0_EVENT_MASK                                       0x00000FFF
#define B0_PAC_DEL_CPU_DEL_RD_CNTR0_EVENT_FIELD_HOB_DEL_RD_CNTR0_EVENT_MASK                                       0x0FFF0000
//========================================
/* REG_PAC_DEL_CPU_DEL_WR_CNTR1_EVENT 0x6C */
#define B0_PAC_DEL_CPU_DEL_WR_CNTR1_EVENT_FIELD_CPU_DEL_WR_CNTR1_EVENT_MASK                                       0x00000FFF
//========================================
/* REG_PAC_DEL_CPU_DEL_RD_CNTR1_EVENT 0x70 */
#define B0_PAC_DEL_CPU_DEL_RD_CNTR1_EVENT_FIELD_CPU_DEL_RD_CNTR1_EVENT_MASK                                       0x00000FFF
#define B0_PAC_DEL_CPU_DEL_RD_CNTR1_EVENT_FIELD_HOB_DEL_RD_CNTR1_EVENT_MASK                                       0x0FFF0000
//========================================
/* REG_PAC_DEL_LAST_CPU_DEL_CNTR_EVENT 0x74 */
#define B0_PAC_DEL_LAST_CPU_DEL_CNTR_EVENT_FIELD_LAST_CPU_DEL_CNTR0_EVENT_MASK                                    0x00000FFF
#define B0_PAC_DEL_LAST_CPU_DEL_CNTR_EVENT_FIELD_LAST_CPU_DEL_CNTR1_EVENT_MASK                                    0x00FFF000
//========================================
/* REG_PAC_DEL_LAST_RD_ADDR_TRANSFER 0x7C */
#define B0_PAC_DEL_LAST_RD_ADDR_TRANSFER_FIELD_LAST_RD_ADDR_TRANSFER_MASK                                         0x0000FFFF
//========================================
/* REG_PAC_DEL_MP_TX_TCR_RCR_WORD_COUNT 0x80 */
#define B0_PAC_DEL_MP_TX_TCR_RCR_WORD_COUNT_FIELD_MP_TX_TCR_WORD_CTR_MASK                                         0x0000007F
#define B0_PAC_DEL_MP_TX_TCR_RCR_WORD_COUNT_FIELD_MP_TX_RCR_WORD_CTR_MASK                                         0x00007F00
//========================================
/* REG_PAC_DEL_MP_FIFO_DEBUG 0x84 */
#define B0_PAC_DEL_MP_FIFO_DEBUG_FIELD_MP_FIFO_FULL_MASK                                                          0x00000001
#define B0_PAC_DEL_MP_FIFO_DEBUG_FIELD_MP_FIFO_EMPTY_MASK                                                         0x00000002
#define B0_PAC_DEL_MP_FIFO_DEBUG_FIELD_OVEN_ALIGN_MASK                                                            0x00000030
#define B0_PAC_DEL_MP_FIFO_DEBUG_FIELD_WRITE_TO_WORD_MASK                                                         0x000000C0
#define B0_PAC_DEL_MP_FIFO_DEBUG_FIELD_MP_FIFO_NUM_BYTES_MASK                                                     0x00007F00
#define B0_PAC_DEL_MP_FIFO_DEBUG_FIELD_MP_FIFO_RD_PTR_MASK                                                        0x003F0000
#define B0_PAC_DEL_MP_FIFO_DEBUG_FIELD_MP_FIFO_WR_PTR_MASK                                                        0x3F000000
//========================================
/* REG_PAC_DEL_TX_DATA_STRUCTURE_ADDR 0x88 */
#define B0_PAC_DEL_TX_DATA_STRUCTURE_ADDR_FIELD_TX_DATA_STRUCTURE_ADDR_MASK                                       0x03FFFFFF
//========================================
/* REG_PAC_DEL_EMU_PRBS_GCLK_BYPASS 0x8C */
#define B0_PAC_DEL_EMU_PRBS_GCLK_BYPASS_FIELD_EMU_PRBS_GCLK_BYPASS_MASK                                           0x00000001
//========================================
/* REG_PAC_DEL_BF_TX_MAX_THRESHOLD_TIMER 0x90 */
#define B0_PAC_DEL_BF_TX_MAX_THRESHOLD_TIMER_FIELD_BF_TX_MAX_THRESHOLD_TIMER_11AG_MASK                            0x000003FF
#define B0_PAC_DEL_BF_TX_MAX_THRESHOLD_TIMER_FIELD_BF_TX_MAX_THRESHOLD_TIMER_11N_MASK                             0x000FFC00
#define B0_PAC_DEL_BF_TX_MAX_THRESHOLD_TIMER_FIELD_BF_TX_MAX_THRESHOLD_TIMER_11AC_MASK                            0x3FF00000
//========================================
/* REG_PAC_DEL_DEL2GENRISC_START_TX_INT 0x94 */
#define B0_PAC_DEL_DEL2GENRISC_START_TX_INT_FIELD_DEL2GENRISC_START_TX_INT_MASK                                   0x0000003F
//========================================
/* REG_PAC_DEL_DEL2GENRISC_START_TX_CLR_INT 0x98 */
#define B0_PAC_DEL_DEL2GENRISC_START_TX_CLR_INT_FIELD_DEL2GENRISC_START_TX_CLR_INT_MASK                           0x0000003F
//========================================
/* REG_PAC_DEL_NOP_WAIT_EVENT_VECTOR 0x9C */
#define B0_PAC_DEL_NOP_WAIT_EVENT_VECTOR_FIELD_NOP_WAIT_EVENT_VECTOR_MASK                                         0x003FFFFF
//========================================
/* REG_PAC_DEL_LAST_NOP_WAIT_EVENT_VECTOR 0xA0 */
#define B0_PAC_DEL_LAST_NOP_WAIT_EVENT_VECTOR_FIELD_LAST_NOP_WAIT_EVENT_VECTOR_MASK                               0x003FFFFF
//========================================
/* REG_PAC_DEL_DELIA_CNTRL_REGISTER 0xA4 */
#define B0_PAC_DEL_DELIA_CNTRL_REGISTER_FIELD_ENABLE_EARLY_AF_RECOVERY_MASK                                       0x00000001
//========================================
/* REG_PAC_DEL_DELIA_LOGGER_CONTROL 0xBC */
#define B0_PAC_DEL_DELIA_LOGGER_CONTROL_FIELD_DELIA_LOGGER_EN_MASK                                                0x00000001
#define B0_PAC_DEL_DELIA_LOGGER_CONTROL_FIELD_DELIA_LOGGER_PRIORITY_MASK                                          0x00000300
#define B0_PAC_DEL_DELIA_LOGGER_CONTROL_FIELD_DELIA_AF_LOGGER_EN_MASK                                             0x00010000
#define B0_PAC_DEL_DELIA_LOGGER_CONTROL_FIELD_DELIA_AF_LOGGER_PRIORITY_MASK                                       0x00300000
#define B0_PAC_DEL_DELIA_LOGGER_CONTROL_FIELD_DELIA_AF_LOGGER_SELECT_USER_ID_MASK                                 0x7F000000
#define B0_PAC_DEL_DELIA_LOGGER_CONTROL_FIELD_DELIA_AF_LOGGER_CAPTURE_ONLY_SOP_EOP_MASK                           0x80000000
//========================================
/* REG_PAC_DEL_DELIA_LOGGER_ACTIVE 0xC0 */
#define B0_PAC_DEL_DELIA_LOGGER_ACTIVE_FIELD_DELIA_LOGGER_ACTIVE_MASK                                             0x00000001
#define B0_PAC_DEL_DELIA_LOGGER_ACTIVE_FIELD_DELIA_AF_LOGGER_ACTIVE_MASK                                          0x00000002
//========================================
/* REG_PAC_DEL_TX_END_STATUS 0xC4 */
#define B0_PAC_DEL_TX_END_STATUS_FIELD_TX_END_PHY_DATA_FIFO_NOT_EMPTY_MASK                                        0x00000001
#define B0_PAC_DEL_TX_END_STATUS_FIELD_PHY_RD_LESS_THAN_EXPECTED_MASK                                             0x00000002
#define B0_PAC_DEL_TX_END_STATUS_FIELD_TX_END_DELIA_RECIPE_ABORTED_MASK                                           0x00000004
#define B0_PAC_DEL_TX_END_STATUS_FIELD_TX_END_AUTO_FILL_OCCURED_MASK                                              0x00000008
#define B0_PAC_DEL_TX_END_STATUS_FIELD_TX_END_AUTO_FILL_PAYLOAD_OCCURED_MASK                                      0x00000010
#define B0_PAC_DEL_TX_END_STATUS_FIELD_PHY_RD_MORE_THAN_EXPECTED_MASK                                             0x00000020
#define B0_PAC_DEL_TX_END_STATUS_FIELD_RECIPE_WROTE_MORE_THAN_EXPCTED_MASK                                        0x00000040
#define B0_PAC_DEL_TX_END_STATUS_FIELD_TX_END_AUTO_FILL_TCR_OCCURED_MASK                                          0x00000080
#define B0_PAC_DEL_TX_END_STATUS_FIELD_TX_END_AUTO_FILL_RCR_OCCURED_MASK                                          0x00000100
#define B0_PAC_DEL_TX_END_STATUS_FIELD_RECIPE_WROTE_LESS_THAN_EXPCTED_MASK                                        0x00000200
#define B0_PAC_DEL_TX_END_STATUS_FIELD_PHY_REQ_LESS_THAN_EXPECTED_MASK                                            0x00000400
#define B0_PAC_DEL_TX_END_STATUS_FIELD_PHY_REQ_MORE_THAN_EXPECTED_MASK                                            0x00000800
#define B0_PAC_DEL_TX_END_STATUS_FIELD_TX_END_OTHER_FIFOS_NOT_EMPTY_MASK                                          0x00001000
#define B0_PAC_DEL_TX_END_STATUS_FIELD_TX_END_FIFO_OVF_UNF_ERROR_MASK                                             0x00002000
#define B0_PAC_DEL_TX_END_STATUS_FIELD_MISSING_LAST_MPDU_MASK                                                     0x00004000
#define B0_PAC_DEL_TX_END_STATUS_FIELD_USED_NEW_HW_MASK                                                           0x00008000
#define B0_PAC_DEL_TX_END_STATUS_FIELD_DESC_LISTS_NOT_EMPTY_MASK                                                  0x00010000
#define B0_PAC_DEL_TX_END_STATUS_FIELD_PHY_RD_BEFORE_REQ_MASK                                                     0x00020000
//========================================
/* REG_PAC_DEL_PHY_RD_COUNT 0xC8 */
#define B0_PAC_DEL_PHY_RD_COUNT_FIELD_PHY_RD_COUNT_MASK                                                           0xFFFFFFFF
//========================================
/* REG_PAC_DEL_PHY_DATA_FIFO_WR_COUNT 0xCC */
#define B0_PAC_DEL_PHY_DATA_FIFO_WR_COUNT_FIELD_PHY_DATA_FIFO_WR_COUNT_MASK                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_RECIPE_WR_CNT 0xD0 */
#define B0_PAC_DEL_RECIPE_WR_CNT_FIELD_RECIPE_WR_CNT_MASK                                                         0xFFFFFFFF
//========================================
/* REG_PAC_DEL_PHY_DATA_FIFO_AF_COUNT 0xE0 */
#define B0_PAC_DEL_PHY_DATA_FIFO_AF_COUNT_FIELD_PHY_DATA_FIFO_AF_COUNT_MASK                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_PHY_DATA_FIFO_RD_COUNT 0xE4 */
#define B0_PAC_DEL_PHY_DATA_FIFO_RD_COUNT_FIELD_PHY_DATA_FIFO_RD_COUNT_MASK                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_TOTAL_PSDU_LENGTH_128B 0xE8 */
#define B0_PAC_DEL_TOTAL_PSDU_LENGTH_128B_FIELD_TOTAL_PSDU_LENGTH_128B_MASK                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_PHY_TOTAL_REQ_SIZE 0xEC */
#define B0_PAC_DEL_PHY_TOTAL_REQ_SIZE_FIELD_PHY_TOTAL_REQ_SIZE_MASK                                               0xFFFFFFFF
//========================================
/* REG_PAC_DEL_DELIMITER_VALUE_PRE_TX_DATA 0xF4 */
#define B0_PAC_DEL_DELIMITER_VALUE_PRE_TX_DATA_FIELD_DELIMITER_VALUE_PRE_TX_DATA_MASK                             0xFFFFFFFF
//========================================
/* REG_PAC_DEL_DEL2GENRISC_START_TX_ENABLE_INT 0xF8 */
#define B0_PAC_DEL_DEL2GENRISC_START_TX_ENABLE_INT_FIELD_DEL2GENRISC_START_TX_ENABLE_INT_MASK                     0x0000003F
//========================================
/* REG_PAC_DEL_USER_DONE_HIGH 0xFC */
#define B0_PAC_DEL_USER_DONE_HIGH_FIELD_USER_DONE_HIGH_MASK                                                       0x0000000F
//========================================
/* REG_PAC_DEL_USER_DONE_LOW 0x100 */
#define B0_PAC_DEL_USER_DONE_LOW_FIELD_USER_DONE_LOW_MASK                                                         0xFFFFFFFF
//========================================
/* REG_PAC_DEL_NEW_HW_FIFO_EMPTY 0x104 */
#define B0_PAC_DEL_NEW_HW_FIFO_EMPTY_FIELD_PHY_REQ_FIFO_EMPTY_MASK                                                0x00000001
#define B0_PAC_DEL_NEW_HW_FIFO_EMPTY_FIELD_MAIN_SM_CMD_FIFO_EMPTY_MASK                                            0x00000002
#define B0_PAC_DEL_NEW_HW_FIFO_EMPTY_FIELD_DESC_RELEASE_FIFO_EMPTY_MASK                                           0x00000004
#define B0_PAC_DEL_NEW_HW_FIFO_EMPTY_FIELD_RD_DATA_CMD_FIFO_EMPTY_MASK                                            0x00000008
#define B0_PAC_DEL_NEW_HW_FIFO_EMPTY_FIELD_RD_DATA_DATA_FIFO_EMPTY_MASK                                           0x00000010
#define B0_PAC_DEL_NEW_HW_FIFO_EMPTY_FIELD_BUILDER_DATA_FIFO_EMPTY_MASK                                           0x00000020
#define B0_PAC_DEL_NEW_HW_FIFO_EMPTY_FIELD_BUILDER_CMD_FIFO_EMPTY_MASK                                            0x00000040
#define B0_PAC_DEL_NEW_HW_FIFO_EMPTY_FIELD_CRC_DATA_FIFO_EMPTY_MASK                                               0x00000080
#define B0_PAC_DEL_NEW_HW_FIFO_EMPTY_FIELD_CRC_CMD_FIFO_EMPTY_MASK                                                0x00000100
#define B0_PAC_DEL_NEW_HW_FIFO_EMPTY_FIELD_PHY_DATA_FIFO_EMPTY_MASK                                               0x00000200
//========================================
/* REG_PAC_DEL_FIFO_CLEAN 0x108 */
#define B0_PAC_DEL_FIFO_CLEAN_FIELD_FIFO_CLEAN_MASK                                                               0x00000001
#define B0_PAC_DEL_FIFO_CLEAN_FIELD_FIFO_CLEAN_ON_START_MASK                                                      0x00000100
//========================================
/* REG_PAC_DEL_NEW_HW_DESC_LISTS_STATUS_CONTROL 0x110 */
#define B0_PAC_DEL_NEW_HW_DESC_LISTS_STATUS_CONTROL_FIELD_DESC_LISTS_EMPTY_MASK                                   0x00000001
#define B0_PAC_DEL_NEW_HW_DESC_LISTS_STATUS_CONTROL_FIELD_CLEAN_DESC_LISTS_MASK                                   0x00000002
#define B0_PAC_DEL_NEW_HW_DESC_LISTS_STATUS_CONTROL_FIELD_CLEAN_DESC_LISTS_DONE_MASK                              0x00000004
#define B0_PAC_DEL_NEW_HW_DESC_LISTS_STATUS_CONTROL_FIELD_ALL_USERS_LAST_MPDU_DONE_MASK                           0x00000008
//========================================
/* REG_PAC_DEL_PHY_DATA_FIFO_AF_TH 0x114 */
#define B0_PAC_DEL_PHY_DATA_FIFO_AF_TH_FIELD_PHY_DATA_FIFO_AF_TH_MASK                                             0x0000007F
//========================================
/* REG_PAC_DEL_FIFO_ERROR_STICKY 0x118 */
#define B0_PAC_DEL_FIFO_ERROR_STICKY_FIELD_MAIN_SM_FIFO_ERROR_STICKY_MASK                                         0x0000000F
#define B0_PAC_DEL_FIFO_ERROR_STICKY_FIELD_DESC_RELEASE_FIFO_ERROR_STICKY_MASK                                    0x00000030
#define B0_PAC_DEL_FIFO_ERROR_STICKY_FIELD_RD_DATA_FIFO_ERROR_STICKY_MASK                                         0x000003C0
#define B0_PAC_DEL_FIFO_ERROR_STICKY_FIELD_BUILDER_FIFO_ERROR_STICKY_MASK                                         0x00003C00
#define B0_PAC_DEL_FIFO_ERROR_STICKY_FIELD_CRC_FIFO_ERROR_STICKY_MASK                                             0x0003C000
#define B0_PAC_DEL_FIFO_ERROR_STICKY_FIELD_PHY_DATA_FIFO_ERROR_STICKY_MASK                                        0x000C0000
#define B0_PAC_DEL_FIFO_ERROR_STICKY_FIELD_LEGACY_MP_FIFO_ERROR_STICKY_MASK                                       0x00300000
#define B0_PAC_DEL_FIFO_ERROR_STICKY_FIELD_CONSEC_PHY_RD_ERROR_STICKY_MASK                                        0x00400000
//========================================
/* REG_PAC_DEL_ZLDS_TYPE_0_REG 0x11C */
#define B0_PAC_DEL_ZLDS_TYPE_0_REG_FIELD_ZLDS_TYPE_0_REG_MASK                                                     0xFFFFFFFF
//========================================
/* REG_PAC_DEL_ZLDS_TYPE_1_REG 0x120 */
#define B0_PAC_DEL_ZLDS_TYPE_1_REG_FIELD_ZLDS_TYPE_1_REG_MASK                                                     0xFFFFFFFF
//========================================
/* REG_PAC_DEL_ZLDS_TYPE_2_REG 0x124 */
#define B0_PAC_DEL_ZLDS_TYPE_2_REG_FIELD_ZLDS_TYPE_2_REG_MASK                                                     0xFFFFFFFF
//========================================
/* REG_PAC_DEL_DESC_AF_REG 0x128 */
#define B0_PAC_DEL_DESC_AF_REG_FIELD_DESC_AF_REG_MASK                                                             0xFFFFFFFF
//========================================
/* REG_PAC_DEL_DATA_AF_REG 0x12C */
#define B0_PAC_DEL_DATA_AF_REG_FIELD_DATA_AF_REG_MASK                                                             0xFFFFFFFF
//========================================
/* REG_PAC_DEL_NEW_HW_STATE_MACHINES 0x130 */
#define B0_PAC_DEL_NEW_HW_STATE_MACHINES_FIELD_MAIN_SM_OUT_MASK                                                   0x00000003
#define B0_PAC_DEL_NEW_HW_STATE_MACHINES_FIELD_DESC_CACHE_CTRL_SM_OUT_MASK                                        0x0000001C
#define B0_PAC_DEL_NEW_HW_STATE_MACHINES_FIELD_DESC_RELEASE_CTRL_SM_OUT_MASK                                      0x000000E0
#define B0_PAC_DEL_NEW_HW_STATE_MACHINES_FIELD_RD_DATA_SM_OUT_MASK                                                0x00000300
#define B0_PAC_DEL_NEW_HW_STATE_MACHINES_FIELD_BUILDER_SM_OUT_MASK                                                0x00001C00
#define B0_PAC_DEL_NEW_HW_STATE_MACHINES_FIELD_CRC_SM_OUT_MASK                                                    0x00006000
//========================================
/* REG_PAC_DEL_NEW_HW_DONE 0x134 */
#define B0_PAC_DEL_NEW_HW_DONE_FIELD_NEW_HW_DONE_MASK                                                             0x00000001
#define B0_PAC_DEL_NEW_HW_DONE_FIELD_AF_ALL_USERS_DONE_MASK                                                       0x00000002
//========================================
/* REG_PAC_DEL_DESC_AF_EVENT_STICKY_HIGH 0x138 */
#define B0_PAC_DEL_DESC_AF_EVENT_STICKY_HIGH_FIELD_DESC_AF_EVENT_STICKY_HIGH_MASK                                 0x0000000F
//========================================
/* REG_PAC_DEL_DESC_AF_EVENT_STICKY_LOW 0x13C */
#define B0_PAC_DEL_DESC_AF_EVENT_STICKY_LOW_FIELD_DESC_AF_EVENT_STICKY_LOW_MASK                                   0xFFFFFFFF
//========================================
/* REG_PAC_DEL_DESC_AF_EVENT_COUNT 0x140 */
#define B0_PAC_DEL_DESC_AF_EVENT_COUNT_FIELD_DESC_AF_EVENT_COUNT_MASK                                             0x00FFFFFF
//========================================
/* REG_PAC_DEL_FIFO_AF_STATUS 0x144 */
#define B0_PAC_DEL_FIFO_AF_STATUS_FIELD_FIFO_AF_TCR_MASK                                                          0x00000001
#define B0_PAC_DEL_FIFO_AF_STATUS_FIELD_FIFO_AF_RCR_MASK                                                          0x00000002
#define B0_PAC_DEL_FIFO_AF_STATUS_FIELD_FIFO_AF_DATA_MASK                                                         0x00000004
//========================================
/* REG_PAC_DEL_BAND_IDX 0x148 */
#define B0_PAC_DEL_BAND_IDX_FIELD_BAND_IDX_MASK                                                                   0x00000003
//========================================
/* REG_PAC_DEL_COM_TCR0_3 0x14C */
#define B0_PAC_DEL_COM_TCR0_3_FIELD_COM_TCR0_3_MASK                                                               0xFFFFFFFF
//========================================
/* REG_PAC_DEL_COM_TCR0_2 0x150 */
#define B0_PAC_DEL_COM_TCR0_2_FIELD_COM_TCR0_2_MASK                                                               0xFFFFFFFF
//========================================
/* REG_PAC_DEL_COM_TCR0_1 0x154 */
#define B0_PAC_DEL_COM_TCR0_1_FIELD_COM_TCR0_1_MASK                                                               0xFFFFFFFF
//========================================
/* REG_PAC_DEL_COM_TCR0_0 0x158 */
#define B0_PAC_DEL_COM_TCR0_0_FIELD_COM_TCR0_0_MASK                                                               0xFFFFFFFF
//========================================
/* REG_PAC_DEL_COM_TCR1_3 0x15C */
#define B0_PAC_DEL_COM_TCR1_3_FIELD_COM_TCR1_3_MASK                                                               0xFFFFFFFF
//========================================
/* REG_PAC_DEL_COM_TCR1_2 0x160 */
#define B0_PAC_DEL_COM_TCR1_2_FIELD_COM_TCR1_2_MASK                                                               0xFFFFFFFF
//========================================
/* REG_PAC_DEL_COM_TCR1_1 0x164 */
#define B0_PAC_DEL_COM_TCR1_1_FIELD_COM_TCR1_1_MASK                                                               0xFFFFFFFF
//========================================
/* REG_PAC_DEL_COM_TCR1_0 0x168 */
#define B0_PAC_DEL_COM_TCR1_0_FIELD_COM_TCR1_0_MASK                                                               0xFFFFFFFF
//========================================
/* REG_PAC_DEL_USER0_TCR_3 0x16C */
#define B0_PAC_DEL_USER0_TCR_3_FIELD_USER0_TCR_3_MASK                                                             0xFFFFFFFF
//========================================
/* REG_PAC_DEL_USER0_TCR_2 0x170 */
#define B0_PAC_DEL_USER0_TCR_2_FIELD_USER0_TCR_2_MASK                                                             0xFFFFFFFF
//========================================
/* REG_PAC_DEL_USER0_TCR_1 0x174 */
#define B0_PAC_DEL_USER0_TCR_1_FIELD_USER0_TCR_1_MASK                                                             0xFFFFFFFF
//========================================
/* REG_PAC_DEL_USER0_TCR_0 0x178 */
#define B0_PAC_DEL_USER0_TCR_0_FIELD_USER0_TCR_0_MASK                                                             0xFFFFFFFF
//========================================
/* REG_PAC_DEL_DLM_PUSH_DPL_IDX_BASE 0x17C */
#define B0_PAC_DEL_DLM_PUSH_DPL_IDX_BASE_FIELD_DLM_PUSH_DPL_IDX_BASE_MASK                                         0x000000FF
//========================================
/* REG_PAC_DEL_REG2HOB_GP0 0x180 */
#define B0_PAC_DEL_REG2HOB_GP0_FIELD_REG2HOB_GP0_MASK                                                             0xFFFFFFFF
//========================================
/* REG_PAC_DEL_REG2HOB_GP1 0x184 */
#define B0_PAC_DEL_REG2HOB_GP1_FIELD_REG2HOB_GP1_MASK                                                             0xFFFFFFFF
//========================================
/* REG_PAC_DEL_SKIP_FLOW_CONTROL 0x188 */
#define B0_PAC_DEL_SKIP_FLOW_CONTROL_FIELD_SKIP_FLOW_CONTROL_MASK                                                 0x00000001
//========================================
/* REG_PAC_DEL_DEBUG_DUP_CTRL 0x18C */
#define B0_PAC_DEL_DEBUG_DUP_CTRL_FIELD_USE_DEBUG_DUP_FIELD_MASK                                                  0x00000001
//========================================
/* REG_PAC_DEL_TCR_BASE_ADDR1 0x190 */
#define B0_PAC_DEL_TCR_BASE_ADDR1_FIELD_TCR_BASE_ADDR1_MASK                                                       0x03FFFFFF
//========================================
/* REG_PAC_DEL_TCR_BASE_ADDR2 0x194 */
#define B0_PAC_DEL_TCR_BASE_ADDR2_FIELD_TCR_BASE_ADDR2_MASK                                                       0x03FFFFFF
//========================================
/* REG_PAC_DEL_RCR_BASE_ADDR 0x198 */
#define B0_PAC_DEL_RCR_BASE_ADDR_FIELD_RCR_BASE_ADDR_MASK                                                         0x03FFFFFF
//========================================
/* REG_PAC_DEL_SENDER_MPDU_DURATION 0x19C */
#define B0_PAC_DEL_SENDER_MPDU_DURATION_FIELD_SENDER_MPDU_DURATION_MASK                                           0x0000FFFF
//========================================
/* REG_PAC_DEL_MAC_PHY_IF_SIGNALS_MONITOR 0x1A0 */
#define B0_PAC_DEL_MAC_PHY_IF_SIGNALS_MONITOR_FIELD_MAC_PHY_ENABLE_MASK                                           0x00000001
#define B0_PAC_DEL_MAC_PHY_IF_SIGNALS_MONITOR_FIELD_PHY_MAC_READY_MASK                                            0x00000002
#define B0_PAC_DEL_MAC_PHY_IF_SIGNALS_MONITOR_FIELD_MAC_PHY_VALID_MASK                                            0x00000004
#define B0_PAC_DEL_MAC_PHY_IF_SIGNALS_MONITOR_FIELD_MAC_PHY_READ_ACK_MASK                                         0x00000008
#define B0_PAC_DEL_MAC_PHY_IF_SIGNALS_MONITOR_FIELD_MAC_PHY_BUS_STATE_MASK                                        0x000000F0
#define B0_PAC_DEL_MAC_PHY_IF_SIGNALS_MONITOR_FIELD_PHY_MAC_REQ_ID_MASK                                           0x0000FF00
#define B0_PAC_DEL_MAC_PHY_IF_SIGNALS_MONITOR_FIELD_PHY_MAC_REQ_SIZE_MASK                                         0x07FF0000
#define B0_PAC_DEL_MAC_PHY_IF_SIGNALS_MONITOR_FIELD_PHY_MAC_REQ_MASK                                              0x40000000
#define B0_PAC_DEL_MAC_PHY_IF_SIGNALS_MONITOR_FIELD_PHY_MAC_READ_MASK                                             0x80000000
//========================================
/* REG_PAC_DEL_START_RECIPE_CNT 0x1A4 */
#define B0_PAC_DEL_START_RECIPE_CNT_FIELD_START_RECIPE_CNT_MASK                                                   0xFFFFFFFF
//========================================
/* REG_PAC_DEL_PHY_DATA_FIFO_AF_COUNT_WM 0x1A8 */
#define B0_PAC_DEL_PHY_DATA_FIFO_AF_COUNT_WM_FIELD_PHY_DATA_FIFO_AF_COUNT_WM_MASK                                 0x0FFFFFFF
#define B0_PAC_DEL_PHY_DATA_FIFO_AF_COUNT_WM_FIELD_PHY_DATA_FIFO_AF_COUNT_WM_CLEAR_MASK                           0x10000000
//========================================
/* REG_PAC_DEL_PHY_DEBUG 0x1AC */
#define B0_PAC_DEL_PHY_DEBUG_FIELD_PHY_DEBUG_DONT_OVVERIDE_TCR_DUR_MASK                                           0x00000001
//========================================
/* REG_PAC_DEL_PAC_PHY_MONITOR_CTRL 0x1B0 */
#define B0_PAC_DEL_PAC_PHY_MONITOR_CTRL_FIELD_PAC_PHY_MONITOR_CNT_MASK                                            0x0FFFFFFF
#define B0_PAC_DEL_PAC_PHY_MONITOR_CTRL_FIELD_PAC_PHY_MONITOR_ENABLE_MASK                                         0x80000000
//========================================
/* REG_PAC_DEL_PAC_PHY_MONITOR_DATA_3 0x1B4 */
#define B0_PAC_DEL_PAC_PHY_MONITOR_DATA_3_FIELD_PAC_PHY_MONITOR_DATA_3_MASK                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_PAC_PHY_MONITOR_DATA_2 0x1B8 */
#define B0_PAC_DEL_PAC_PHY_MONITOR_DATA_2_FIELD_PAC_PHY_MONITOR_DATA_2_MASK                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_PAC_PHY_MONITOR_DATA_1 0x1BC */
#define B0_PAC_DEL_PAC_PHY_MONITOR_DATA_1_FIELD_PAC_PHY_MONITOR_DATA_1_MASK                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_PAC_PHY_MONITOR_DATA_0 0x1C0 */
#define B0_PAC_DEL_PAC_PHY_MONITOR_DATA_0_FIELD_PAC_PHY_MONITOR_DATA_0_MASK                                       0xFFFFFFFF
//========================================
/* REG_PAC_DEL_PHY_DATA_FIFO_HOST_RD_ADDRESS 0x1C4 */
#define B0_PAC_DEL_PHY_DATA_FIFO_HOST_RD_ADDRESS_FIELD_PHY_DATA_FIFO_HOST_RD_ADDRESS_MASK                         0x000001FF
//========================================
/* REG_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_3 0x1C8 */
#define B0_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_3_FIELD_PHY_DATA_FIFO_HOST_RDATA_3_MASK                               0xFFFFFFFF
//========================================
/* REG_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_2 0x1CC */
#define B0_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_2_FIELD_PHY_DATA_FIFO_HOST_RDATA_2_MASK                               0xFFFFFFFF
//========================================
/* REG_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_1 0x1D0 */
#define B0_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_1_FIELD_PHY_DATA_FIFO_HOST_RDATA_1_MASK                               0xFFFFFFFF
//========================================
/* REG_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_0 0x1D4 */
#define B0_PAC_DEL_PHY_DATA_FIFO_HOST_RDATA_0_FIELD_PHY_DATA_FIFO_HOST_RDATA_0_MASK                               0xFFFFFFFF
//========================================
/* REG_PAC_DEL_FC_WR_PTR_SELECT_USER_ID 0x1D8 */
#define B0_PAC_DEL_FC_WR_PTR_SELECT_USER_ID_FIELD_FC_WR_PTR_SELECT_USER_ID_MASK                                   0x000003FF
//========================================
/* REG_PAC_DEL_FC_WR_PTR_FOR_SELECTED_USER 0x1DC */
#define B0_PAC_DEL_FC_WR_PTR_FOR_SELECTED_USER_FIELD_FC_WR_PTR_FOR_SELECTED_USER_MASK                             0x00007FFF
//========================================
/* REG_PAC_DEL_GOOD_MPDU_COUNT_SELECT_USER_ID 0x1E0 */
#define B0_PAC_DEL_GOOD_MPDU_COUNT_SELECT_USER_ID_FIELD_GOOD_MPDU_COUNT_SELECT_USER_ID_MASK                       0x000003FF
//========================================
/* REG_PAC_DEL_GOOD_MPDU_COUNT_FOR_SELECTED_USER 0x1E4 */
#define B0_PAC_DEL_GOOD_MPDU_COUNT_FOR_SELECTED_USER_FIELD_GOOD_MPDU_COUNT_FOR_SELECTED_USER_MASK                 0x00000FFF
//========================================
/* REG_PAC_DEL_NEW_HW_SKIP_CHECK_FC 0x1E8 */
#define B0_PAC_DEL_NEW_HW_SKIP_CHECK_FC_FIELD_NEW_HW_SKIP_CHECK_FC_MASK                                           0x00000001
//========================================
/* REG_PAC_DEL_DESC_LIST_EMPTY_HIGH 0x1EC */
#define B0_PAC_DEL_DESC_LIST_EMPTY_HIGH_FIELD_DESC_LIST_EMPTY_HIGH_MASK                                           0x0000000F
//========================================
/* REG_PAC_DEL_DESC_LIST_EMPTY_LOW 0x1F0 */
#define B0_PAC_DEL_DESC_LIST_EMPTY_LOW_FIELD_DESC_LIST_EMPTY_LOW_MASK                                             0xFFFFFFFF
//========================================
/* REG_PAC_DEL_SPARE_ECO 0x1FC */
#define B0_PAC_DEL_SPARE_ECO_FIELD_SPARE_ECO_MASK                                                                 0x0000FFFF


#endif // _PAC_DEL_REGS_H_
