
/***********************************************************************************
File:				QManagerCpu3Regs.h
Module:				qManagerCpu3
SOC Revision:		latest
Generated at:       2024-06-26 12:55:11
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _Q_MANAGER_CPU3_REGS_H_
#define _Q_MANAGER_CPU3_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define Q_MANAGER_CPU3_BASE_ADDRESS                      MEMORY_MAP_UNIT_36_BASE_ADDRESS
#define	REG_Q_MANAGER_CPU3_CTL                                (Q_MANAGER_CPU3_BASE_ADDRESS + 0x0)
#define	REG_Q_MANAGER_CPU3_PUSH_ADDR_TOP                      (Q_MANAGER_CPU3_BASE_ADDRESS + 0x4)
#define	REG_Q_MANAGER_CPU3_PUSH_ADDR_BOTTOM                   (Q_MANAGER_CPU3_BASE_ADDRESS + 0x8)
#define	REG_Q_MANAGER_CPU3_POP_ADDR_TOP                       (Q_MANAGER_CPU3_BASE_ADDRESS + 0xC)
#define	REG_Q_MANAGER_CPU3_POP_ADDR_BOTTOM                    (Q_MANAGER_CPU3_BASE_ADDRESS + 0x10)
#define	REG_Q_MANAGER_CPU3_CLEAN_FIFO                         (Q_MANAGER_CPU3_BASE_ADDRESS + 0x14)
#define	REG_Q_MANAGER_CPU3_FIFO_EN                            (Q_MANAGER_CPU3_BASE_ADDRESS + 0x18)
#define	REG_Q_MANAGER_CPU3_INT_CTL                            (Q_MANAGER_CPU3_BASE_ADDRESS + 0x1C)
#define	REG_Q_MANAGER_CPU3_INT_PUSH_ADDR_TOP                  (Q_MANAGER_CPU3_BASE_ADDRESS + 0x20)
#define	REG_Q_MANAGER_CPU3_INT_PUSH_ADDR_BOTTOM               (Q_MANAGER_CPU3_BASE_ADDRESS + 0x24)
#define	REG_Q_MANAGER_CPU3_INT_POP_ADDR_TOP                   (Q_MANAGER_CPU3_BASE_ADDRESS + 0x28)
#define	REG_Q_MANAGER_CPU3_INT_POP_ADDR_BOTTOM                (Q_MANAGER_CPU3_BASE_ADDRESS + 0x2C)
#define	REG_Q_MANAGER_CPU3_INT_STATUS                         (Q_MANAGER_CPU3_BASE_ADDRESS + 0x30)
#define	REG_Q_MANAGER_CPU3_TX_LISTS_EMPTY_INT_EN0_31          (Q_MANAGER_CPU3_BASE_ADDRESS + 0x38)
#define	REG_Q_MANAGER_CPU3_TX_LISTS_EMPTY_INT_EN32_63         (Q_MANAGER_CPU3_BASE_ADDRESS + 0x3C)
#define	REG_Q_MANAGER_CPU3_TX_LISTS_NOT_EMPTY_INT_EN0_31      (Q_MANAGER_CPU3_BASE_ADDRESS + 0x40)
#define	REG_Q_MANAGER_CPU3_TX_LISTS_NOT_EMPTY_INT_EN32_63     (Q_MANAGER_CPU3_BASE_ADDRESS + 0x44)
#define	REG_Q_MANAGER_CPU3_RX_LISTS_EMPTY_INT_EN0_31          (Q_MANAGER_CPU3_BASE_ADDRESS + 0x48)
#define	REG_Q_MANAGER_CPU3_RX_LISTS_EMPTY_INT_EN32_63         (Q_MANAGER_CPU3_BASE_ADDRESS + 0x4C)
#define	REG_Q_MANAGER_CPU3_RX_LISTS_NOT_EMPTY_INT_EN0_31      (Q_MANAGER_CPU3_BASE_ADDRESS + 0x50)
#define	REG_Q_MANAGER_CPU3_RX_LISTS_NOT_EMPTY_INT_EN32_63     (Q_MANAGER_CPU3_BASE_ADDRESS + 0x54)
#define	REG_Q_MANAGER_CPU3_TX_DONE_INT                        (Q_MANAGER_CPU3_BASE_ADDRESS + 0x58)
#define	REG_Q_MANAGER_CPU3_RX_DONE_INT                        (Q_MANAGER_CPU3_BASE_ADDRESS + 0x5C)
#define	REG_Q_MANAGER_CPU3_LO_PRI_TX_READY_INT                (Q_MANAGER_CPU3_BASE_ADDRESS + 0x60)
#define	REG_Q_MANAGER_CPU3_LO_PRI_RX_READY_INT                (Q_MANAGER_CPU3_BASE_ADDRESS + 0x64)
#define	REG_Q_MANAGER_CPU3_HI_PRI_TX_READY_INT                (Q_MANAGER_CPU3_BASE_ADDRESS + 0x68)
#define	REG_Q_MANAGER_CPU3_HI_PRI_RX_READY_INT                (Q_MANAGER_CPU3_BASE_ADDRESS + 0x6C)
#define	REG_Q_MANAGER_CPU3_DESC_POOL_INT_TX                   (Q_MANAGER_CPU3_BASE_ADDRESS + 0x70)
#define	REG_Q_MANAGER_CPU3_DESC_POOL_INT_RX                   (Q_MANAGER_CPU3_BASE_ADDRESS + 0x74)
#define	REG_Q_MANAGER_CPU3_TX_INT_CLR0_31                     (Q_MANAGER_CPU3_BASE_ADDRESS + 0x78)
#define	REG_Q_MANAGER_CPU3_TX_INT_CLR32_63                    (Q_MANAGER_CPU3_BASE_ADDRESS + 0x7C)
#define	REG_Q_MANAGER_CPU3_RX_INT_CLR0_31                     (Q_MANAGER_CPU3_BASE_ADDRESS + 0x80)
#define	REG_Q_MANAGER_CPU3_RX_INT_CLR32_63                    (Q_MANAGER_CPU3_BASE_ADDRESS + 0x84)
#define	REG_Q_MANAGER_CPU3_DISABLE_NOT_EMPTY_INT_EN           (Q_MANAGER_CPU3_BASE_ADDRESS + 0x88)
#define	REG_Q_MANAGER_CPU3_DISABLE_INT_CLR                    (Q_MANAGER_CPU3_BASE_ADDRESS + 0x8C)
#define	REG_Q_MANAGER_CPU3_DISABLE_INT                        (Q_MANAGER_CPU3_BASE_ADDRESS + 0x90)
#define	REG_Q_MANAGER_CPU3_FIFO_STATUS                        (Q_MANAGER_CPU3_BASE_ADDRESS + 0x94)
#define	REG_Q_MANAGER_CPU3_ERR                                (Q_MANAGER_CPU3_BASE_ADDRESS + 0x98)
#define	REG_Q_MANAGER_CPU3_ERR_CLR                            (Q_MANAGER_CPU3_BASE_ADDRESS + 0x9C)
#define	REG_Q_MANAGER_CPU3_Q_MANAGER_SPARE                    (Q_MANAGER_CPU3_BASE_ADDRESS + 0xC0)
#define	REG_Q_MANAGER_CPU3_TX_LISTS_EMPTY0_31                 (Q_MANAGER_CPU3_BASE_ADDRESS + 0xC4)
#define	REG_Q_MANAGER_CPU3_TX_LISTS_EMPTY32_63                (Q_MANAGER_CPU3_BASE_ADDRESS + 0xC8)
#define	REG_Q_MANAGER_CPU3_RX_LISTS_EMPTY0_31                 (Q_MANAGER_CPU3_BASE_ADDRESS + 0xCC)
#define	REG_Q_MANAGER_CPU3_RX_LISTS_EMPTY32_63                (Q_MANAGER_CPU3_BASE_ADDRESS + 0xD0)
#define	REG_Q_MANAGER_CPU3_DMA_LISTS_EMPTY0_31                (Q_MANAGER_CPU3_BASE_ADDRESS + 0xD4)
#define	REG_Q_MANAGER_CPU3_DMA_LISTS_EMPTY32_63               (Q_MANAGER_CPU3_BASE_ADDRESS + 0xD8)
#define	REG_Q_MANAGER_CPU3_DMA_LISTS_EMPTY64_81               (Q_MANAGER_CPU3_BASE_ADDRESS + 0xDC)
#define	REG_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY0_31      (Q_MANAGER_CPU3_BASE_ADDRESS + 0xE0)
#define	REG_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY32_63     (Q_MANAGER_CPU3_BASE_ADDRESS + 0xE4)
#define	REG_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY64_79     (Q_MANAGER_CPU3_BASE_ADDRESS + 0xE8)
#define	REG_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY0_31      (Q_MANAGER_CPU3_BASE_ADDRESS + 0xEC)
#define	REG_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY32_63     (Q_MANAGER_CPU3_BASE_ADDRESS + 0xF0)
#define	REG_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY64_79     (Q_MANAGER_CPU3_BASE_ADDRESS + 0xF4)
#define	REG_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY0_31      (Q_MANAGER_CPU3_BASE_ADDRESS + 0xF8)
#define	REG_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY32_63     (Q_MANAGER_CPU3_BASE_ADDRESS + 0xFC)
#define	REG_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY64_95     (Q_MANAGER_CPU3_BASE_ADDRESS + 0x100)
#define	REG_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY96_115    (Q_MANAGER_CPU3_BASE_ADDRESS + 0x104)
#define	REG_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY0_31      (Q_MANAGER_CPU3_BASE_ADDRESS + 0x108)
#define	REG_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY32_63     (Q_MANAGER_CPU3_BASE_ADDRESS + 0x10C)
#define	REG_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY64_95     (Q_MANAGER_CPU3_BASE_ADDRESS + 0x110)
#define	REG_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY96_115    (Q_MANAGER_CPU3_BASE_ADDRESS + 0x114)
#define	REG_Q_MANAGER_CPU3_AGER_EN                            (Q_MANAGER_CPU3_BASE_ADDRESS + 0x200)
#define	REG_Q_MANAGER_CPU3_AGER_CFG                           (Q_MANAGER_CPU3_BASE_ADDRESS + 0x204)
#define	REG_Q_MANAGER_CPU3_AGER_PD_TYPE_UMAC_CFG              (Q_MANAGER_CPU3_BASE_ADDRESS + 0x208)
#define	REG_Q_MANAGER_CPU3_AGER_LISTS_CFG                     (Q_MANAGER_CPU3_BASE_ADDRESS + 0x20C)
#define	REG_Q_MANAGER_CPU3_AGER_SINGLE_CHECK_CFG              (Q_MANAGER_CPU3_BASE_ADDRESS + 0x210)
#define	REG_Q_MANAGER_CPU3_AGER_SINGLE_CHECK_ACTIVATE         (Q_MANAGER_CPU3_BASE_ADDRESS + 0x214)
#define	REG_Q_MANAGER_CPU3_AGER_CHECK_STATUS                  (Q_MANAGER_CPU3_BASE_ADDRESS + 0x218)
#define	REG_Q_MANAGER_CPU3_AGER_CHECK_MOST_STATUS             (Q_MANAGER_CPU3_BASE_ADDRESS + 0x21C)
#define	REG_Q_MANAGER_CPU3_AGER_UMAC_DONE_COUNTER             (Q_MANAGER_CPU3_BASE_ADDRESS + 0x220)
#define	REG_Q_MANAGER_CPU3_AGER_LIB_DONE_COUNTER              (Q_MANAGER_CPU3_BASE_ADDRESS + 0x224)
#define	REG_Q_MANAGER_CPU3_AGER_TRY_FAIL_COUNTER              (Q_MANAGER_CPU3_BASE_ADDRESS + 0x228)
#define	REG_Q_MANAGER_CPU3_AGER_COUNTER_CLR                   (Q_MANAGER_CPU3_BASE_ADDRESS + 0x22C)
#define	REG_Q_MANAGER_CPU3_AGER_AC_LEAVE_CRITERIA             (Q_MANAGER_CPU3_BASE_ADDRESS + 0x230)
#define	REG_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA0           (Q_MANAGER_CPU3_BASE_ADDRESS + 0x234)
#define	REG_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA1           (Q_MANAGER_CPU3_BASE_ADDRESS + 0x238)
#define	REG_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA2           (Q_MANAGER_CPU3_BASE_ADDRESS + 0x23C)
#define	REG_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA3           (Q_MANAGER_CPU3_BASE_ADDRESS + 0x240)
#define	REG_Q_MANAGER_CPU3_AGER_SINGLE_PD_LIMIT               (Q_MANAGER_CPU3_BASE_ADDRESS + 0x244)
#define	REG_Q_MANAGER_CPU3_AGER_STA0_31_STATUS                (Q_MANAGER_CPU3_BASE_ADDRESS + 0x248)
#define	REG_Q_MANAGER_CPU3_AGER_STA32_63_STATUS               (Q_MANAGER_CPU3_BASE_ADDRESS + 0x24C)
#define	REG_Q_MANAGER_CPU3_AGER_STA64_95_STATUS               (Q_MANAGER_CPU3_BASE_ADDRESS + 0x250)
#define	REG_Q_MANAGER_CPU3_AGER_STA96_127_STATUS              (Q_MANAGER_CPU3_BASE_ADDRESS + 0x254)
#define	REG_Q_MANAGER_CPU3_AGER_STA128_159_STATUS             (Q_MANAGER_CPU3_BASE_ADDRESS + 0x258)
#define	REG_Q_MANAGER_CPU3_AGER_STA160_191_STATUS             (Q_MANAGER_CPU3_BASE_ADDRESS + 0x25C)
#define	REG_Q_MANAGER_CPU3_AGER_STA192_223_STATUS             (Q_MANAGER_CPU3_BASE_ADDRESS + 0x260)
#define	REG_Q_MANAGER_CPU3_AGER_STA224_255_STATUS             (Q_MANAGER_CPU3_BASE_ADDRESS + 0x264)
#define	REG_Q_MANAGER_CPU3_AGER_VAP_STATUS                    (Q_MANAGER_CPU3_BASE_ADDRESS + 0x268)
#define	REG_Q_MANAGER_CPU3_AGER_IRQ_CLR                       (Q_MANAGER_CPU3_BASE_ADDRESS + 0x26C)
#define	REG_Q_MANAGER_CPU3_AGER_TMP_LIST_CFG                  (Q_MANAGER_CPU3_BASE_ADDRESS + 0x270)
#define	REG_Q_MANAGER_CPU3_LOGGER_AGER_PRIORITY               (Q_MANAGER_CPU3_BASE_ADDRESS + 0x274)
#define	REG_Q_MANAGER_CPU3_LOGGER_AGER_CFG                    (Q_MANAGER_CPU3_BASE_ADDRESS + 0x278)
#define	REG_Q_MANAGER_CPU3_LOGGER_AGER_BUSY                   (Q_MANAGER_CPU3_BASE_ADDRESS + 0x27C)
#define	REG_Q_MANAGER_CPU3_AGER_STA_TTL                       (Q_MANAGER_CPU3_BASE_ADDRESS + 0x280)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_Q_MANAGER_CPU3_CTL 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ctlPrimaryAddr : 8; //Primary address, reset value: 0x0, access type: RW
		uint32 ctlSecondaryAddr : 3; //Secondary address, reset value: 0x0, access type: RW
		uint32 ctlMgmt : 1; //Mgmt TID. When set Mgmt TID will be accessed and secondary field will be ignored., reset value: 0x0, access type: RW
		uint32 ctlSetNull : 1; //Set Null to the "Next descriptor field" of the pushed PD, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 ctlDplIdx : 8; //Descriptor Pointer List index number. Values can be 0-31, reset value: 0x0, access type: RW
		uint32 ctlDlmIdx : 4; //DLM index number. Values can be 0-8: , 0x0: TX Data DLM , 0x1: TX Lists DLM , 0x2: RX Data DLM , 0x3: RX Lists DLM , 0x4: DMA Lists DLM , 0x5: RX MPDU Descriptor Band0 Lists DLM , 0x6: RX MPDU Descriptor Band1 Lists DLM , 0x7: TX MPDU Descriptor Band0 Lists DLM , 0x8: TX MPDU Descriptor Band1 Lists DLM ,  , , reset value: 0x0, access type: RW
		uint32 ctlReq : 4; //DLM Request: , 3---b000: Push packet to tail , 3---b001: Push packet to head , 3---b010: Pop packet , 3---b011: Return head packet (Peek) , 3---b100: Push packet list to tail , 3---b101: Push packet list to head , 3---b110: Pop packet list , 3---b111: Flush , , reset value: 0x0, access type: RW
	} bitFields;
} RegQManagerCpu3Ctl_u;

/*REG_Q_MANAGER_CPU3_PUSH_ADDR_TOP 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pushAddrTop : 24; //Top address pointer, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegQManagerCpu3PushAddrTop_u;

/*REG_Q_MANAGER_CPU3_PUSH_ADDR_BOTTOM 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pushAddrBottom : 24; //Top address pointer, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegQManagerCpu3PushAddrBottom_u;

/*REG_Q_MANAGER_CPU3_POP_ADDR_TOP 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 popAddrTop : 24; //Requested head address pointer., reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegQManagerCpu3PopAddrTop_u;

/*REG_Q_MANAGER_CPU3_POP_ADDR_BOTTOM 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 popAddrBottom : 24; //Requested tail address pointer., reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegQManagerCpu3PopAddrBottom_u;

/*REG_Q_MANAGER_CPU3_CLEAN_FIFO 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cleanFifo : 1; //Cleans FIFO, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegQManagerCpu3CleanFifo_u;

/*REG_Q_MANAGER_CPU3_FIFO_EN 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifoEn : 1; //Enables FIFO, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegQManagerCpu3FifoEn_u;

/*REG_Q_MANAGER_CPU3_INT_CTL 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intCtlPrimaryAddr : 8; //Primary address, reset value: 0x0, access type: RW
		uint32 intCtlSecondaryAddr : 3; //Secondary address, reset value: 0x0, access type: RW
		uint32 intCtlMgmt : 1; //Mgmt TID. When set Mgmt TID will be accessed and secondary field will be ignored., reset value: 0x0, access type: RW
		uint32 intCtlSetNull : 1; //Set Null to the "Next descriptor field" of the pushed PD, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 intCtlDplIdx : 8; //Descriptor Pointer List index number. Values can be 0-31, reset value: 0x0, access type: RW
		uint32 intCtlDlmIdx : 4; //DLM index number. Values can be 0-8: , 0x0: TX Data DLM , 0x1: TX Lists DLM , 0x2: RX Data DLM , 0x3: RX Lists DLM , 0x4: DMA Lists DLM , 0x5: RX MPDU Descriptor Band0 Lists DLM , 0x6: RX MPDU Descriptor Band1 Lists DLM , 0x7: TX MPDU Descriptor Band0 Lists DLM , 0x8: TX MPDU Descriptor Band1 Lists DLM ,  , , reset value: 0x0, access type: RW
		uint32 intCtlReq : 4; //DLM Request: , 3---b000: Push packet to tail , 3---b001: Push packet to head , 3---b010: Pop packet , 3---b011: Return head packet (Peek) , 3---b100: Push packet list to tail , 3---b101: Push packet list to head , 3---b110: Pop packet list , 3---b111: Flush , , reset value: 0x0, access type: RW
	} bitFields;
} RegQManagerCpu3IntCtl_u;

/*REG_Q_MANAGER_CPU3_INT_PUSH_ADDR_TOP 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intPushAddrTop : 24; //Top address pointer, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegQManagerCpu3IntPushAddrTop_u;

/*REG_Q_MANAGER_CPU3_INT_PUSH_ADDR_BOTTOM 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intPushAddrBottom : 24; //Top address pointer, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegQManagerCpu3IntPushAddrBottom_u;

/*REG_Q_MANAGER_CPU3_INT_POP_ADDR_TOP 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intPopAddrTop : 24; //Requested head address pointer., reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegQManagerCpu3IntPopAddrTop_u;

/*REG_Q_MANAGER_CPU3_INT_POP_ADDR_BOTTOM 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intPopAddrBottom : 24; //Requested tail address pointer., reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegQManagerCpu3IntPopAddrBottom_u;

/*REG_Q_MANAGER_CPU3_INT_STATUS 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDoneListsInt : 1; //Interrupt coming from the High priority TXPD Done lists, reset value: 0x0, access type: RO
		uint32 rxDoneListsInt : 1; //Interrupt coming from the RXPD Done lists, reset value: 0x0, access type: RO
		uint32 loPriTxReadyListsInt : 1; //Interrupt coming from the Low priority TX Ready lists, reset value: 0x0, access type: RO
		uint32 loPriRxReadyListsInt : 1; //Interrupt coming from the Low priority RX Ready lists, reset value: 0x0, access type: RO
		uint32 hiPriTxReadyListsInt : 1; //Interrupt coming from the High priority TX Ready lists, reset value: 0x0, access type: RO
		uint32 hiPriRxReadyListsInt : 1; //Interrupt coming from the High priority RX Ready lists, reset value: 0x0, access type: RO
		uint32 descPoolInt : 1; //Interrupt coming from the TX/RX pool lists, reset value: 0x0, access type: RO
		uint32 reserved0 : 25;
	} bitFields;
} RegQManagerCpu3IntStatus_u;

/*REG_Q_MANAGER_CPU3_TX_LISTS_EMPTY_INT_EN0_31 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsEmptyIntEn031 : 32; //Empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegQManagerCpu3TxListsEmptyIntEn031_u;

/*REG_Q_MANAGER_CPU3_TX_LISTS_EMPTY_INT_EN32_63 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsEmptyIntEn3263 : 32; //Empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegQManagerCpu3TxListsEmptyIntEn3263_u;

/*REG_Q_MANAGER_CPU3_TX_LISTS_NOT_EMPTY_INT_EN0_31 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsNotEmptyIntEn031 : 32; //Not empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegQManagerCpu3TxListsNotEmptyIntEn031_u;

/*REG_Q_MANAGER_CPU3_TX_LISTS_NOT_EMPTY_INT_EN32_63 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsNotEmptyIntEn3263 : 32; //Not empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegQManagerCpu3TxListsNotEmptyIntEn3263_u;

/*REG_Q_MANAGER_CPU3_RX_LISTS_EMPTY_INT_EN0_31 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsEmptyIntEn031 : 32; //Empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegQManagerCpu3RxListsEmptyIntEn031_u;

/*REG_Q_MANAGER_CPU3_RX_LISTS_EMPTY_INT_EN32_63 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsEmptyIntEn3263 : 32; //Empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegQManagerCpu3RxListsEmptyIntEn3263_u;

/*REG_Q_MANAGER_CPU3_RX_LISTS_NOT_EMPTY_INT_EN0_31 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsNotEmptyIntEn031 : 32; //Not empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegQManagerCpu3RxListsNotEmptyIntEn031_u;

/*REG_Q_MANAGER_CPU3_RX_LISTS_NOT_EMPTY_INT_EN32_63 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsNotEmptyIntEn3263 : 32; //Not empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegQManagerCpu3RxListsNotEmptyIntEn3263_u;

/*REG_Q_MANAGER_CPU3_TX_DONE_INT 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDoneListsEmptyInt : 16; //TX done lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 txDoneListsNotEmptyInt : 16; //TX done lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3TxDoneInt_u;

/*REG_Q_MANAGER_CPU3_RX_DONE_INT 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxDoneListsEmptyInt : 16; //RX done lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 rxDoneListsNotEmptyInt : 16; //RX done lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3RxDoneInt_u;

/*REG_Q_MANAGER_CPU3_LO_PRI_TX_READY_INT 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txLoPriReadyEmptyInt : 16; //TX Low priority ready lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 txLoPriReadyNotEmptyInt : 16; //TX Low priority ready lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3LoPriTxReadyInt_u;

/*REG_Q_MANAGER_CPU3_LO_PRI_RX_READY_INT 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxLoPriReadyEmptyInt : 16; //RX Low priority ready lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 rxLoPriReadyNotEmptyInt : 16; //RX Low priority ready lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3LoPriRxReadyInt_u;

/*REG_Q_MANAGER_CPU3_HI_PRI_TX_READY_INT 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txHiPriReadyEmptyInt : 16; //TX high priority ready lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 txHiPriReadyNotEmptyInt : 16; //TX high priority ready lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3HiPriTxReadyInt_u;

/*REG_Q_MANAGER_CPU3_HI_PRI_RX_READY_INT 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxHiPriReadyEmptyInt : 16; //RX high priority ready lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 rxHiPriReadyNotEmptyInt : 16; //RX high priority ready lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3HiPriRxReadyInt_u;

/*REG_Q_MANAGER_CPU3_DESC_POOL_INT_TX 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descTxPoolEmptyInt : 16; //Descriptor pool TX lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 descTxPoolNotEmptyInt : 16; //Descriptor pool TX lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3DescPoolIntTx_u;

/*REG_Q_MANAGER_CPU3_DESC_POOL_INT_RX 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descRxPoolEmptyInt : 16; //Descriptor pool RX lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 descRxPoolNotEmptyInt : 16; //Descriptor pool RX lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3DescPoolIntRx_u;

/*REG_Q_MANAGER_CPU3_TX_INT_CLR0_31 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDoneListsIntClr : 16; //Clear TX Done lists interrupt, reset value: 0x0, access type: WO
		uint32 loPriTxReadyListsIntClr : 16; //Clear Low priority TX Ready lists interrupt, reset value: 0x0, access type: WO
	} bitFields;
} RegQManagerCpu3TxIntClr031_u;

/*REG_Q_MANAGER_CPU3_TX_INT_CLR32_63 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hiPriTxReadyListsIntClr : 16; //Clear High priority TXPD Done lists interrupt, reset value: 0x0, access type: WO
		uint32 descPoolIntClrTx : 16; //Clear TX pool lists interrupt, reset value: 0x0, access type: WO
	} bitFields;
} RegQManagerCpu3TxIntClr3263_u;

/*REG_Q_MANAGER_CPU3_RX_INT_CLR0_31 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxDoneListsIntClr : 16; //Clear RX Done lists interrupt, reset value: 0x0, access type: WO
		uint32 loPriRxReadyListsIntClr : 16; //Clear Low priority RX ready lists interrupt, reset value: 0x0, access type: WO
	} bitFields;
} RegQManagerCpu3RxIntClr031_u;

/*REG_Q_MANAGER_CPU3_RX_INT_CLR32_63 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hiPriRxReadyListsIntClr : 16; //Clear High priority RX Ready lists interrupt, reset value: 0x0, access type: WO
		uint32 descPoolIntClrRx : 16; //Clear RX pool lists interrupt, reset value: 0x0, access type: WO
	} bitFields;
} RegQManagerCpu3RxIntClr3263_u;

/*REG_Q_MANAGER_CPU3_DISABLE_NOT_EMPTY_INT_EN 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disQueueNotEmptyIntEn : 16; //Not empty interrupt Enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegQManagerCpu3DisableNotEmptyIntEn_u;

/*REG_Q_MANAGER_CPU3_DISABLE_INT_CLR 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disQueueNotEmptyIntClr : 16; //Clear High priority TXPD Done lists interrupt, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegQManagerCpu3DisableIntClr_u;

/*REG_Q_MANAGER_CPU3_DISABLE_INT 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disQueueNotEmptyInt : 16; //Descriptor pool TX lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegQManagerCpu3DisableInt_u;

/*REG_Q_MANAGER_CPU3_FIFO_STATUS 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swFifoHfull : 1; //SW FIFO half full indication, reset value: 0x0, access type: RO
		uint32 swFifoFull : 1; //SW FIFO full indication, reset value: 0x0, access type: RO
		uint32 swFifoEmpty : 1; //SW FIFO empty indication, reset value: 0x1, access type: RO
		uint32 swFifoPushWhileFull : 1; //SW FIFO overflow - push when FIFO is full, reset value: 0x0, access type: RO
		uint32 swFifoPopWhileEmpty : 1; //SW FIFO underflow - pop when there is no data, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegQManagerCpu3FifoStatus_u;

/*REG_Q_MANAGER_CPU3_ERR 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDataErrPopDisabled : 1; //Pop to a disabled client, reset value: 0x0, access type: RO
		uint32 errFifo : 1; //FIFO underflow/overflow error, reset value: 0x0, access type: RO
		uint32 cpu0ErrDoubleBlockingReq : 1; //Pop while another pop takes place, reset value: 0x0, access type: RO
		uint32 cpu1ErrDoubleBlockingReq : 1; //no description, reset value: 0x0, access type: RO
		uint32 cpu2ErrDoubleBlockingReq : 1; //no description, reset value: 0x0, access type: RO
		uint32 cpu3ErrDoubleBlockingReq : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegQManagerCpu3Err_u;

/*REG_Q_MANAGER_CPU3_ERR_CLR 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errFifoClr : 1; //Clears FIFO error, reset value: 0x0, access type: WO
		uint32 errDoubleBlockingReqClr : 1; //Clears double pop error, reset value: 0x0, access type: WO
		uint32 errDoubleBlockingReqEn : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegQManagerCpu3ErrClr_u;

/*REG_Q_MANAGER_CPU3_Q_MANAGER_SPARE 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qManagerSpare : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegQManagerCpu3QManagerSpare_u;

/*REG_Q_MANAGER_CPU3_TX_LISTS_EMPTY0_31 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsDlmEmpty031 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3TxListsEmpty031_u;

/*REG_Q_MANAGER_CPU3_TX_LISTS_EMPTY32_63 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsDlmEmpty3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3TxListsEmpty3263_u;

/*REG_Q_MANAGER_CPU3_RX_LISTS_EMPTY0_31 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsDlmEmpty031 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3RxListsEmpty031_u;

/*REG_Q_MANAGER_CPU3_RX_LISTS_EMPTY32_63 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsDlmEmpty3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3RxListsEmpty3263_u;

/*REG_Q_MANAGER_CPU3_DMA_LISTS_EMPTY0_31 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dmaListsEmpty031 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3DmaListsEmpty031_u;

/*REG_Q_MANAGER_CPU3_DMA_LISTS_EMPTY32_63 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dmaListsEmpty3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3DmaListsEmpty3263_u;

/*REG_Q_MANAGER_CPU3_DMA_LISTS_EMPTY64_81 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dmaListsEmpty6481 : 18; //no description, reset value: 0x3FFFF, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegQManagerCpu3DmaListsEmpty6481_u;

/*REG_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY0_31 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxMpduDesc0ListsEmpty031 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3RxMpduDesc0ListsEmpty031_u;

/*REG_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY32_63 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxMpduDesc0ListsEmpty3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3RxMpduDesc0ListsEmpty3263_u;

/*REG_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY64_79 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxMpduDesc0ListsEmpty6479 : 16; //no description, reset value: 0xFFFF, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegQManagerCpu3RxMpduDesc0ListsEmpty6479_u;

/*REG_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY0_31 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxMpduDesc1ListsEmpty031 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3RxMpduDesc1ListsEmpty031_u;

/*REG_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY32_63 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxMpduDesc1ListsEmpty3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3RxMpduDesc1ListsEmpty3263_u;

/*REG_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY64_79 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxMpduDesc1ListsEmpty6479 : 16; //no description, reset value: 0xFFFF, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegQManagerCpu3RxMpduDesc1ListsEmpty6479_u;

/*REG_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY0_31 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc0ListsEmpty031 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3TxMpduDesc0ListsEmpty031_u;

/*REG_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY32_63 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc0ListsEmpty3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3TxMpduDesc0ListsEmpty3263_u;

/*REG_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY64_95 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc0ListsEmpty6495 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3TxMpduDesc0ListsEmpty6495_u;

/*REG_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY96_115 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc0ListsEmpty96115 : 20; //no description, reset value: 0xFFFFF, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegQManagerCpu3TxMpduDesc0ListsEmpty96115_u;

/*REG_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY0_31 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc1ListsEmpty031 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3TxMpduDesc1ListsEmpty031_u;

/*REG_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY32_63 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc1ListsEmpty3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3TxMpduDesc1ListsEmpty3263_u;

/*REG_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY64_95 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc1ListsEmpty6495 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegQManagerCpu3TxMpduDesc1ListsEmpty6495_u;

/*REG_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY96_115 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc1ListsEmpty96115 : 20; //no description, reset value: 0xFFFFF, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegQManagerCpu3TxMpduDesc1ListsEmpty96115_u;

/*REG_Q_MANAGER_CPU3_AGER_EN 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agerEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 agerSwHalt : 1; //no description, reset value: 0x0, access type: RW
		uint32 periodicCatEn : 4; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 agerPeriodStartStaVap : 9; //no description, reset value: 0x0, access type: RW
		uint32 reserved2 : 7;
	} bitFields;
} RegQManagerCpu3AgerEn_u;

/*REG_Q_MANAGER_CPU3_AGER_CFG 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agerPeriod : 7; //no description, reset value: 0x10, access type: RW
		uint32 reserved0 : 1;
		uint32 shramWaitState : 7; //no description, reset value: 0x1, access type: RW
		uint32 reserved1 : 1;
		uint32 clientWaitState : 8; //no description, reset value: 0x0, access type: RW
		uint32 retryDiscardLock : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 agerMgmtTidEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 ignoreTwtSp : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
	} bitFields;
} RegQManagerCpu3AgerCfg_u;

/*REG_Q_MANAGER_CPU3_AGER_PD_TYPE_UMAC_CFG 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pdTypeUmacSel : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegQManagerCpu3AgerPdTypeUmacCfg_u;

/*REG_Q_MANAGER_CPU3_AGER_LISTS_CFG 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 liberatorListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 umacListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegQManagerCpu3AgerListsCfg_u;

/*REG_Q_MANAGER_CPU3_AGER_SINGLE_CHECK_CFG 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 startStaIdx : 9; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 forceLock : 1; //no description, reset value: 0x0, access type: RW
		uint32 singleCatEn : 4; //no description, reset value: 0x1, access type: RW
		uint32 reserved1 : 11;
	} bitFields;
} RegQManagerCpu3AgerSingleCheckCfg_u;

/*REG_Q_MANAGER_CPU3_AGER_SINGLE_CHECK_ACTIVATE 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageSingleCheckWaitRelease : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegQManagerCpu3AgerSingleCheckActivate_u;

/*REG_Q_MANAGER_CPU3_AGER_CHECK_STATUS 0x218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 singleCheckInWait : 1; //no description, reset value: 0x0, access type: RO
		uint32 singleCheckDone : 1; //no description, reset value: 0x0, access type: RO
		uint32 backgroundCheckDone : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
		uint32 agerSm : 5; //no description, reset value: 0x0, access type: RO
		uint32 lastCheckedClient : 9; //no description, reset value: 0x11f, access type: RO
		uint32 reserved1 : 10;
	} bitFields;
} RegQManagerCpu3AgerCheckStatus_u;

/*REG_Q_MANAGER_CPU3_AGER_CHECK_MOST_STATUS 0x21C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastTidChecked : 4; //no description, reset value: 0x0, access type: RO
		uint32 lastStaChecked : 9; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 mostPdTid : 4; //no description, reset value: 0x0, access type: RO
		uint32 mostPdSta : 9; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 3;
	} bitFields;
} RegQManagerCpu3AgerCheckMostStatus_u;

/*REG_Q_MANAGER_CPU3_AGER_UMAC_DONE_COUNTER 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 umacCounter : 17; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegQManagerCpu3AgerUmacDoneCounter_u;

/*REG_Q_MANAGER_CPU3_AGER_LIB_DONE_COUNTER 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 libCounter : 17; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegQManagerCpu3AgerLibDoneCounter_u;

/*REG_Q_MANAGER_CPU3_AGER_TRY_FAIL_COUNTER 0x228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tryFailCounter : 17; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegQManagerCpu3AgerTryFailCounter_u;

/*REG_Q_MANAGER_CPU3_AGER_COUNTER_CLR 0x22C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 umacClear : 1; //no description, reset value: 0x0, access type: WO
		uint32 libClear : 1; //no description, reset value: 0x0, access type: WO
		uint32 tryFailClear : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegQManagerCpu3AgerCounterClr_u;

/*REG_Q_MANAGER_CPU3_AGER_AC_LEAVE_CRITERIA 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beCriteria : 7; //no description, reset value: 0x10, access type: RW
		uint32 reserved0 : 1;
		uint32 bkCriteria : 7; //no description, reset value: 0x10, access type: RW
		uint32 reserved1 : 1;
		uint32 viCriteria : 7; //no description, reset value: 0x10, access type: RW
		uint32 reserved2 : 1;
		uint32 voCriteria : 7; //no description, reset value: 0x10, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegQManagerCpu3AgerAcLeaveCriteria_u;

/*REG_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA0 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beThreshCriteria : 17; //no description, reset value: 0x100, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegQManagerCpu3AgerAcThreshCriteria0_u;

/*REG_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA1 0x238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bkThreshCriteria : 17; //no description, reset value: 0x100, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegQManagerCpu3AgerAcThreshCriteria1_u;

/*REG_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA2 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 viThreshCriteria : 17; //no description, reset value: 0x100, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegQManagerCpu3AgerAcThreshCriteria2_u;

/*REG_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA3 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 voThreshCriteria : 17; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegQManagerCpu3AgerAcThreshCriteria3_u;

/*REG_Q_MANAGER_CPU3_AGER_SINGLE_PD_LIMIT 0x244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 singlePdDiscardLimit : 17; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 singlePdDiscardLimitEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 11;
	} bitFields;
} RegQManagerCpu3AgerSinglePdLimit_u;

/*REG_Q_MANAGER_CPU3_AGER_STA0_31_STATUS 0x248 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta031Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3AgerSta031Status_u;

/*REG_Q_MANAGER_CPU3_AGER_STA32_63_STATUS 0x24C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta3263Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3AgerSta3263Status_u;

/*REG_Q_MANAGER_CPU3_AGER_STA64_95_STATUS 0x250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta6495Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3AgerSta6495Status_u;

/*REG_Q_MANAGER_CPU3_AGER_STA96_127_STATUS 0x254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta96127Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3AgerSta96127Status_u;

/*REG_Q_MANAGER_CPU3_AGER_STA128_159_STATUS 0x258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta128159Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3AgerSta128159Status_u;

/*REG_Q_MANAGER_CPU3_AGER_STA160_191_STATUS 0x25C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta160191Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3AgerSta160191Status_u;

/*REG_Q_MANAGER_CPU3_AGER_STA192_223_STATUS 0x260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta192223Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3AgerSta192223Status_u;

/*REG_Q_MANAGER_CPU3_AGER_STA224_255_STATUS 0x264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta224255Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3AgerSta224255Status_u;

/*REG_Q_MANAGER_CPU3_AGER_VAP_STATUS 0x268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckVapStatus : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3AgerVapStatus_u;

/*REG_Q_MANAGER_CPU3_AGER_IRQ_CLR 0x26C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agerDoneIrqClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegQManagerCpu3AgerIrqClr_u;

/*REG_Q_MANAGER_CPU3_AGER_TMP_LIST_CFG 0x270 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 umacTmpListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegQManagerCpu3AgerTmpListCfg_u;

/*REG_Q_MANAGER_CPU3_LOGGER_AGER_PRIORITY 0x274 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 24;
		uint32 agerLoggerPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
	} bitFields;
} RegQManagerCpu3LoggerAgerPriority_u;

/*REG_Q_MANAGER_CPU3_LOGGER_AGER_CFG 0x278 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 28;
		uint32 agerLoggerEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
	} bitFields;
} RegQManagerCpu3LoggerAgerCfg_u;

/*REG_Q_MANAGER_CPU3_LOGGER_AGER_BUSY 0x27C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 31;
		uint32 agerLoggerBusy : 1; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3LoggerAgerBusy_u;

/*REG_Q_MANAGER_CPU3_AGER_STA_TTL 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staTtlValueWr : 7; //TTL value for Write, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 staTtlValueRd : 7; //TTL value of Read, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 staTtlStaIdx : 9; //STA/VAP index: , 0-255: STA index , 256-287: VAP index, reset value: 0x0, access type: RW
		uint32 reserved2 : 3;
		uint32 staTtlWr : 1; //Write/Read indication, reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
		uint32 staTtlDone : 1; //Access done indication., reset value: 0x0, access type: RO
	} bitFields;
} RegQManagerCpu3AgerStaTtl_u;

//========================================
/* REG_Q_MANAGER_CPU3_CTL 0x0 */
#define B01_Q_MANAGER_CPU3_CTL_FIELD_CTL_PRIMARY_ADDR_MASK                                                        0x000000FF
#define B01_Q_MANAGER_CPU3_CTL_FIELD_CTL_SECONDARY_ADDR_MASK                                                      0x00000700
#define B01_Q_MANAGER_CPU3_CTL_FIELD_CTL_MGMT_MASK                                                                0x00000800
#define B01_Q_MANAGER_CPU3_CTL_FIELD_CTL_SET_NULL_MASK                                                            0x00001000
#define B01_Q_MANAGER_CPU3_CTL_FIELD_CTL_DPL_IDX_MASK                                                             0x00FF0000
#define B01_Q_MANAGER_CPU3_CTL_FIELD_CTL_DLM_IDX_MASK                                                             0x0F000000
#define B01_Q_MANAGER_CPU3_CTL_FIELD_CTL_REQ_MASK                                                                 0xF0000000
//========================================
/* REG_Q_MANAGER_CPU3_PUSH_ADDR_TOP 0x4 */
#define B01_Q_MANAGER_CPU3_PUSH_ADDR_TOP_FIELD_PUSH_ADDR_TOP_MASK                                                 0x00FFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_PUSH_ADDR_BOTTOM 0x8 */
#define B01_Q_MANAGER_CPU3_PUSH_ADDR_BOTTOM_FIELD_PUSH_ADDR_BOTTOM_MASK                                           0x00FFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_POP_ADDR_TOP 0xC */
#define B01_Q_MANAGER_CPU3_POP_ADDR_TOP_FIELD_POP_ADDR_TOP_MASK                                                   0x00FFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_POP_ADDR_BOTTOM 0x10 */
#define B01_Q_MANAGER_CPU3_POP_ADDR_BOTTOM_FIELD_POP_ADDR_BOTTOM_MASK                                             0x00FFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_CLEAN_FIFO 0x14 */
#define B01_Q_MANAGER_CPU3_CLEAN_FIFO_FIELD_CLEAN_FIFO_MASK                                                       0x00000001
//========================================
/* REG_Q_MANAGER_CPU3_FIFO_EN 0x18 */
#define B01_Q_MANAGER_CPU3_FIFO_EN_FIELD_FIFO_EN_MASK                                                             0x00000001
//========================================
/* REG_Q_MANAGER_CPU3_INT_CTL 0x1C */
#define B01_Q_MANAGER_CPU3_INT_CTL_FIELD_INT_CTL_PRIMARY_ADDR_MASK                                                0x000000FF
#define B01_Q_MANAGER_CPU3_INT_CTL_FIELD_INT_CTL_SECONDARY_ADDR_MASK                                              0x00000700
#define B01_Q_MANAGER_CPU3_INT_CTL_FIELD_INT_CTL_MGMT_MASK                                                        0x00000800
#define B01_Q_MANAGER_CPU3_INT_CTL_FIELD_INT_CTL_SET_NULL_MASK                                                    0x00001000
#define B01_Q_MANAGER_CPU3_INT_CTL_FIELD_INT_CTL_DPL_IDX_MASK                                                     0x00FF0000
#define B01_Q_MANAGER_CPU3_INT_CTL_FIELD_INT_CTL_DLM_IDX_MASK                                                     0x0F000000
#define B01_Q_MANAGER_CPU3_INT_CTL_FIELD_INT_CTL_REQ_MASK                                                         0xF0000000
//========================================
/* REG_Q_MANAGER_CPU3_INT_PUSH_ADDR_TOP 0x20 */
#define B01_Q_MANAGER_CPU3_INT_PUSH_ADDR_TOP_FIELD_INT_PUSH_ADDR_TOP_MASK                                         0x00FFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_INT_PUSH_ADDR_BOTTOM 0x24 */
#define B01_Q_MANAGER_CPU3_INT_PUSH_ADDR_BOTTOM_FIELD_INT_PUSH_ADDR_BOTTOM_MASK                                   0x00FFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_INT_POP_ADDR_TOP 0x28 */
#define B01_Q_MANAGER_CPU3_INT_POP_ADDR_TOP_FIELD_INT_POP_ADDR_TOP_MASK                                           0x00FFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_INT_POP_ADDR_BOTTOM 0x2C */
#define B01_Q_MANAGER_CPU3_INT_POP_ADDR_BOTTOM_FIELD_INT_POP_ADDR_BOTTOM_MASK                                     0x00FFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_INT_STATUS 0x30 */
#define B01_Q_MANAGER_CPU3_INT_STATUS_FIELD_TX_DONE_LISTS_INT_MASK                                                0x00000001
#define B01_Q_MANAGER_CPU3_INT_STATUS_FIELD_RX_DONE_LISTS_INT_MASK                                                0x00000002
#define B01_Q_MANAGER_CPU3_INT_STATUS_FIELD_LO_PRI_TX_READY_LISTS_INT_MASK                                        0x00000004
#define B01_Q_MANAGER_CPU3_INT_STATUS_FIELD_LO_PRI_RX_READY_LISTS_INT_MASK                                        0x00000008
#define B01_Q_MANAGER_CPU3_INT_STATUS_FIELD_HI_PRI_TX_READY_LISTS_INT_MASK                                        0x00000010
#define B01_Q_MANAGER_CPU3_INT_STATUS_FIELD_HI_PRI_RX_READY_LISTS_INT_MASK                                        0x00000020
#define B01_Q_MANAGER_CPU3_INT_STATUS_FIELD_DESC_POOL_INT_MASK                                                    0x00000040
//========================================
/* REG_Q_MANAGER_CPU3_TX_LISTS_EMPTY_INT_EN0_31 0x38 */
#define B01_Q_MANAGER_CPU3_TX_LISTS_EMPTY_INT_EN0_31_FIELD_TX_LISTS_EMPTY_INT_EN0_31_MASK                         0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_TX_LISTS_EMPTY_INT_EN32_63 0x3C */
#define B01_Q_MANAGER_CPU3_TX_LISTS_EMPTY_INT_EN32_63_FIELD_TX_LISTS_EMPTY_INT_EN32_63_MASK                       0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_TX_LISTS_NOT_EMPTY_INT_EN0_31 0x40 */
#define B01_Q_MANAGER_CPU3_TX_LISTS_NOT_EMPTY_INT_EN0_31_FIELD_TX_LISTS_NOT_EMPTY_INT_EN0_31_MASK                 0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_TX_LISTS_NOT_EMPTY_INT_EN32_63 0x44 */
#define B01_Q_MANAGER_CPU3_TX_LISTS_NOT_EMPTY_INT_EN32_63_FIELD_TX_LISTS_NOT_EMPTY_INT_EN32_63_MASK               0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_RX_LISTS_EMPTY_INT_EN0_31 0x48 */
#define B01_Q_MANAGER_CPU3_RX_LISTS_EMPTY_INT_EN0_31_FIELD_RX_LISTS_EMPTY_INT_EN0_31_MASK                         0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_RX_LISTS_EMPTY_INT_EN32_63 0x4C */
#define B01_Q_MANAGER_CPU3_RX_LISTS_EMPTY_INT_EN32_63_FIELD_RX_LISTS_EMPTY_INT_EN32_63_MASK                       0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_RX_LISTS_NOT_EMPTY_INT_EN0_31 0x50 */
#define B01_Q_MANAGER_CPU3_RX_LISTS_NOT_EMPTY_INT_EN0_31_FIELD_RX_LISTS_NOT_EMPTY_INT_EN0_31_MASK                 0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_RX_LISTS_NOT_EMPTY_INT_EN32_63 0x54 */
#define B01_Q_MANAGER_CPU3_RX_LISTS_NOT_EMPTY_INT_EN32_63_FIELD_RX_LISTS_NOT_EMPTY_INT_EN32_63_MASK               0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_TX_DONE_INT 0x58 */
#define B01_Q_MANAGER_CPU3_TX_DONE_INT_FIELD_TX_DONE_LISTS_EMPTY_INT_MASK                                         0x0000FFFF
#define B01_Q_MANAGER_CPU3_TX_DONE_INT_FIELD_TX_DONE_LISTS_NOT_EMPTY_INT_MASK                                     0xFFFF0000
//========================================
/* REG_Q_MANAGER_CPU3_RX_DONE_INT 0x5C */
#define B01_Q_MANAGER_CPU3_RX_DONE_INT_FIELD_RX_DONE_LISTS_EMPTY_INT_MASK                                         0x0000FFFF
#define B01_Q_MANAGER_CPU3_RX_DONE_INT_FIELD_RX_DONE_LISTS_NOT_EMPTY_INT_MASK                                     0xFFFF0000
//========================================
/* REG_Q_MANAGER_CPU3_LO_PRI_TX_READY_INT 0x60 */
#define B01_Q_MANAGER_CPU3_LO_PRI_TX_READY_INT_FIELD_TX_LO_PRI_READY_EMPTY_INT_MASK                               0x0000FFFF
#define B01_Q_MANAGER_CPU3_LO_PRI_TX_READY_INT_FIELD_TX_LO_PRI_READY_NOT_EMPTY_INT_MASK                           0xFFFF0000
//========================================
/* REG_Q_MANAGER_CPU3_LO_PRI_RX_READY_INT 0x64 */
#define B01_Q_MANAGER_CPU3_LO_PRI_RX_READY_INT_FIELD_RX_LO_PRI_READY_EMPTY_INT_MASK                               0x0000FFFF
#define B01_Q_MANAGER_CPU3_LO_PRI_RX_READY_INT_FIELD_RX_LO_PRI_READY_NOT_EMPTY_INT_MASK                           0xFFFF0000
//========================================
/* REG_Q_MANAGER_CPU3_HI_PRI_TX_READY_INT 0x68 */
#define B01_Q_MANAGER_CPU3_HI_PRI_TX_READY_INT_FIELD_TX_HI_PRI_READY_EMPTY_INT_MASK                               0x0000FFFF
#define B01_Q_MANAGER_CPU3_HI_PRI_TX_READY_INT_FIELD_TX_HI_PRI_READY_NOT_EMPTY_INT_MASK                           0xFFFF0000
//========================================
/* REG_Q_MANAGER_CPU3_HI_PRI_RX_READY_INT 0x6C */
#define B01_Q_MANAGER_CPU3_HI_PRI_RX_READY_INT_FIELD_RX_HI_PRI_READY_EMPTY_INT_MASK                               0x0000FFFF
#define B01_Q_MANAGER_CPU3_HI_PRI_RX_READY_INT_FIELD_RX_HI_PRI_READY_NOT_EMPTY_INT_MASK                           0xFFFF0000
//========================================
/* REG_Q_MANAGER_CPU3_DESC_POOL_INT_TX 0x70 */
#define B01_Q_MANAGER_CPU3_DESC_POOL_INT_TX_FIELD_DESC_TX_POOL_EMPTY_INT_MASK                                     0x0000FFFF
#define B01_Q_MANAGER_CPU3_DESC_POOL_INT_TX_FIELD_DESC_TX_POOL_NOT_EMPTY_INT_MASK                                 0xFFFF0000
//========================================
/* REG_Q_MANAGER_CPU3_DESC_POOL_INT_RX 0x74 */
#define B01_Q_MANAGER_CPU3_DESC_POOL_INT_RX_FIELD_DESC_RX_POOL_EMPTY_INT_MASK                                     0x0000FFFF
#define B01_Q_MANAGER_CPU3_DESC_POOL_INT_RX_FIELD_DESC_RX_POOL_NOT_EMPTY_INT_MASK                                 0xFFFF0000
//========================================
/* REG_Q_MANAGER_CPU3_TX_INT_CLR0_31 0x78 */
#define B01_Q_MANAGER_CPU3_TX_INT_CLR0_31_FIELD_TX_DONE_LISTS_INT_CLR_MASK                                        0x0000FFFF
#define B01_Q_MANAGER_CPU3_TX_INT_CLR0_31_FIELD_LO_PRI_TX_READY_LISTS_INT_CLR_MASK                                0xFFFF0000
//========================================
/* REG_Q_MANAGER_CPU3_TX_INT_CLR32_63 0x7C */
#define B01_Q_MANAGER_CPU3_TX_INT_CLR32_63_FIELD_HI_PRI_TX_READY_LISTS_INT_CLR_MASK                               0x0000FFFF
#define B01_Q_MANAGER_CPU3_TX_INT_CLR32_63_FIELD_DESC_POOL_INT_CLR_TX_MASK                                        0xFFFF0000
//========================================
/* REG_Q_MANAGER_CPU3_RX_INT_CLR0_31 0x80 */
#define B01_Q_MANAGER_CPU3_RX_INT_CLR0_31_FIELD_RX_DONE_LISTS_INT_CLR_MASK                                        0x0000FFFF
#define B01_Q_MANAGER_CPU3_RX_INT_CLR0_31_FIELD_LO_PRI_RX_READY_LISTS_INT_CLR_MASK                                0xFFFF0000
//========================================
/* REG_Q_MANAGER_CPU3_RX_INT_CLR32_63 0x84 */
#define B01_Q_MANAGER_CPU3_RX_INT_CLR32_63_FIELD_HI_PRI_RX_READY_LISTS_INT_CLR_MASK                               0x0000FFFF
#define B01_Q_MANAGER_CPU3_RX_INT_CLR32_63_FIELD_DESC_POOL_INT_CLR_RX_MASK                                        0xFFFF0000
//========================================
/* REG_Q_MANAGER_CPU3_DISABLE_NOT_EMPTY_INT_EN 0x88 */
#define B01_Q_MANAGER_CPU3_DISABLE_NOT_EMPTY_INT_EN_FIELD_DIS_QUEUE_NOT_EMPTY_INT_EN_MASK                         0x0000FFFF
//========================================
/* REG_Q_MANAGER_CPU3_DISABLE_INT_CLR 0x8C */
#define B01_Q_MANAGER_CPU3_DISABLE_INT_CLR_FIELD_DIS_QUEUE_NOT_EMPTY_INT_CLR_MASK                                 0x0000FFFF
//========================================
/* REG_Q_MANAGER_CPU3_DISABLE_INT 0x90 */
#define B01_Q_MANAGER_CPU3_DISABLE_INT_FIELD_DIS_QUEUE_NOT_EMPTY_INT_MASK                                         0x0000FFFF
//========================================
/* REG_Q_MANAGER_CPU3_FIFO_STATUS 0x94 */
#define B01_Q_MANAGER_CPU3_FIFO_STATUS_FIELD_SW_FIFO_HFULL_MASK                                                   0x00000001
#define B01_Q_MANAGER_CPU3_FIFO_STATUS_FIELD_SW_FIFO_FULL_MASK                                                    0x00000002
#define B01_Q_MANAGER_CPU3_FIFO_STATUS_FIELD_SW_FIFO_EMPTY_MASK                                                   0x00000004
#define B01_Q_MANAGER_CPU3_FIFO_STATUS_FIELD_SW_FIFO_PUSH_WHILE_FULL_MASK                                         0x00000008
#define B01_Q_MANAGER_CPU3_FIFO_STATUS_FIELD_SW_FIFO_POP_WHILE_EMPTY_MASK                                         0x00000010
//========================================
/* REG_Q_MANAGER_CPU3_ERR 0x98 */
#define B01_Q_MANAGER_CPU3_ERR_FIELD_TX_DATA_ERR_POP_DISABLED_MASK                                                0x00000001
#define B01_Q_MANAGER_CPU3_ERR_FIELD_ERR_FIFO_MASK                                                                0x00000002
#define B01_Q_MANAGER_CPU3_ERR_FIELD_CPU0_ERR_DOUBLE_BLOCKING_REQ_MASK                                            0x00000004
#define B01_Q_MANAGER_CPU3_ERR_FIELD_CPU1_ERR_DOUBLE_BLOCKING_REQ_MASK                                            0x00000008
#define B01_Q_MANAGER_CPU3_ERR_FIELD_CPU2_ERR_DOUBLE_BLOCKING_REQ_MASK                                            0x00000010
#define B01_Q_MANAGER_CPU3_ERR_FIELD_CPU3_ERR_DOUBLE_BLOCKING_REQ_MASK                                            0x00000020
//========================================
/* REG_Q_MANAGER_CPU3_ERR_CLR 0x9C */
#define B01_Q_MANAGER_CPU3_ERR_CLR_FIELD_ERR_FIFO_CLR_MASK                                                        0x00000001
#define B01_Q_MANAGER_CPU3_ERR_CLR_FIELD_ERR_DOUBLE_BLOCKING_REQ_CLR_MASK                                         0x00000002
#define B01_Q_MANAGER_CPU3_ERR_CLR_FIELD_ERR_DOUBLE_BLOCKING_REQ_EN_MASK                                          0x00000004
//========================================
/* REG_Q_MANAGER_CPU3_Q_MANAGER_SPARE 0xC0 */
#define B01_Q_MANAGER_CPU3_Q_MANAGER_SPARE_FIELD_Q_MANAGER_SPARE_MASK                                             0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_TX_LISTS_EMPTY0_31 0xC4 */
#define B01_Q_MANAGER_CPU3_TX_LISTS_EMPTY0_31_FIELD_TX_LISTS_DLM_EMPTY0_31_MASK                                   0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_TX_LISTS_EMPTY32_63 0xC8 */
#define B01_Q_MANAGER_CPU3_TX_LISTS_EMPTY32_63_FIELD_TX_LISTS_DLM_EMPTY32_63_MASK                                 0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_RX_LISTS_EMPTY0_31 0xCC */
#define B01_Q_MANAGER_CPU3_RX_LISTS_EMPTY0_31_FIELD_RX_LISTS_DLM_EMPTY0_31_MASK                                   0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_RX_LISTS_EMPTY32_63 0xD0 */
#define B01_Q_MANAGER_CPU3_RX_LISTS_EMPTY32_63_FIELD_RX_LISTS_DLM_EMPTY32_63_MASK                                 0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_DMA_LISTS_EMPTY0_31 0xD4 */
#define B01_Q_MANAGER_CPU3_DMA_LISTS_EMPTY0_31_FIELD_DMA_LISTS_EMPTY0_31_MASK                                     0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_DMA_LISTS_EMPTY32_63 0xD8 */
#define B01_Q_MANAGER_CPU3_DMA_LISTS_EMPTY32_63_FIELD_DMA_LISTS_EMPTY32_63_MASK                                   0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_DMA_LISTS_EMPTY64_81 0xDC */
#define B01_Q_MANAGER_CPU3_DMA_LISTS_EMPTY64_81_FIELD_DMA_LISTS_EMPTY64_81_MASK                                   0x0003FFFF
//========================================
/* REG_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY0_31 0xE0 */
#define B01_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY0_31_FIELD_RX_MPDU_DESC0_LISTS_EMPTY0_31_MASK                 0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY32_63 0xE4 */
#define B01_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY32_63_FIELD_RX_MPDU_DESC0_LISTS_EMPTY32_63_MASK               0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY64_79 0xE8 */
#define B01_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY64_79_FIELD_RX_MPDU_DESC0_LISTS_EMPTY64_79_MASK               0x0000FFFF
//========================================
/* REG_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY0_31 0xEC */
#define B01_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY0_31_FIELD_RX_MPDU_DESC1_LISTS_EMPTY0_31_MASK                 0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY32_63 0xF0 */
#define B01_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY32_63_FIELD_RX_MPDU_DESC1_LISTS_EMPTY32_63_MASK               0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY64_79 0xF4 */
#define B01_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY64_79_FIELD_RX_MPDU_DESC1_LISTS_EMPTY64_79_MASK               0x0000FFFF
//========================================
/* REG_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY0_31 0xF8 */
#define B01_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY0_31_FIELD_TX_MPDU_DESC0_LISTS_EMPTY0_31_MASK                 0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY32_63 0xFC */
#define B01_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY32_63_FIELD_TX_MPDU_DESC0_LISTS_EMPTY32_63_MASK               0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY64_95 0x100 */
#define B01_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY64_95_FIELD_TX_MPDU_DESC0_LISTS_EMPTY64_95_MASK               0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY96_115 0x104 */
#define B01_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY96_115_FIELD_TX_MPDU_DESC0_LISTS_EMPTY96_115_MASK             0x000FFFFF
//========================================
/* REG_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY0_31 0x108 */
#define B01_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY0_31_FIELD_TX_MPDU_DESC1_LISTS_EMPTY0_31_MASK                 0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY32_63 0x10C */
#define B01_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY32_63_FIELD_TX_MPDU_DESC1_LISTS_EMPTY32_63_MASK               0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY64_95 0x110 */
#define B01_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY64_95_FIELD_TX_MPDU_DESC1_LISTS_EMPTY64_95_MASK               0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY96_115 0x114 */
#define B01_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY96_115_FIELD_TX_MPDU_DESC1_LISTS_EMPTY96_115_MASK             0x000FFFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_EN 0x200 */
#define B01_Q_MANAGER_CPU3_AGER_EN_FIELD_AGER_EN_MASK                                                             0x00000001
#define B01_Q_MANAGER_CPU3_AGER_EN_FIELD_AGER_SW_HALT_MASK                                                        0x00000100
#define B01_Q_MANAGER_CPU3_AGER_EN_FIELD_PERIODIC_CAT_EN_MASK                                                     0x00001E00
#define B01_Q_MANAGER_CPU3_AGER_EN_FIELD_AGER_PERIOD_START_STA_VAP_MASK                                           0x01FF0000
//========================================
/* REG_Q_MANAGER_CPU3_AGER_CFG 0x204 */
#define B01_Q_MANAGER_CPU3_AGER_CFG_FIELD_AGER_PERIOD_MASK                                                        0x0000007F
#define B01_Q_MANAGER_CPU3_AGER_CFG_FIELD_SHRAM_WAIT_STATE_MASK                                                   0x00007F00
#define B01_Q_MANAGER_CPU3_AGER_CFG_FIELD_CLIENT_WAIT_STATE_MASK                                                  0x00FF0000
#define B01_Q_MANAGER_CPU3_AGER_CFG_FIELD_RETRY_DISCARD_LOCK_MASK                                                 0x01000000
#define B01_Q_MANAGER_CPU3_AGER_CFG_FIELD_AGER_MGMT_TID_EN_MASK                                                   0x10000000
#define B01_Q_MANAGER_CPU3_AGER_CFG_FIELD_IGNORE_TWT_SP_MASK                                                      0x20000000
//========================================
/* REG_Q_MANAGER_CPU3_AGER_PD_TYPE_UMAC_CFG 0x208 */
#define B01_Q_MANAGER_CPU3_AGER_PD_TYPE_UMAC_CFG_FIELD_PD_TYPE_UMAC_SEL_MASK                                      0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_LISTS_CFG 0x20C */
#define B01_Q_MANAGER_CPU3_AGER_LISTS_CFG_FIELD_LIBERATOR_LIST_IDX_MASK                                           0x0000007F
#define B01_Q_MANAGER_CPU3_AGER_LISTS_CFG_FIELD_UMAC_LIST_IDX_MASK                                                0x00007F00
//========================================
/* REG_Q_MANAGER_CPU3_AGER_SINGLE_CHECK_CFG 0x210 */
#define B01_Q_MANAGER_CPU3_AGER_SINGLE_CHECK_CFG_FIELD_START_STA_IDX_MASK                                         0x000001FF
#define B01_Q_MANAGER_CPU3_AGER_SINGLE_CHECK_CFG_FIELD_FORCE_LOCK_MASK                                            0x00010000
#define B01_Q_MANAGER_CPU3_AGER_SINGLE_CHECK_CFG_FIELD_SINGLE_CAT_EN_MASK                                         0x001E0000
//========================================
/* REG_Q_MANAGER_CPU3_AGER_SINGLE_CHECK_ACTIVATE 0x214 */
#define B01_Q_MANAGER_CPU3_AGER_SINGLE_CHECK_ACTIVATE_FIELD_AGE_SINGLE_CHECK_WAIT_RELEASE_MASK                    0x00000001
//========================================
/* REG_Q_MANAGER_CPU3_AGER_CHECK_STATUS 0x218 */
#define B01_Q_MANAGER_CPU3_AGER_CHECK_STATUS_FIELD_SINGLE_CHECK_IN_WAIT_MASK                                      0x00000001
#define B01_Q_MANAGER_CPU3_AGER_CHECK_STATUS_FIELD_SINGLE_CHECK_DONE_MASK                                         0x00000002
#define B01_Q_MANAGER_CPU3_AGER_CHECK_STATUS_FIELD_BACKGROUND_CHECK_DONE_MASK                                     0x00000004
#define B01_Q_MANAGER_CPU3_AGER_CHECK_STATUS_FIELD_AGER_SM_MASK                                                   0x00001F00
#define B01_Q_MANAGER_CPU3_AGER_CHECK_STATUS_FIELD_LAST_CHECKED_CLIENT_MASK                                       0x003FE000
//========================================
/* REG_Q_MANAGER_CPU3_AGER_CHECK_MOST_STATUS 0x21C */
#define B01_Q_MANAGER_CPU3_AGER_CHECK_MOST_STATUS_FIELD_LAST_TID_CHECKED_MASK                                     0x0000000F
#define B01_Q_MANAGER_CPU3_AGER_CHECK_MOST_STATUS_FIELD_LAST_STA_CHECKED_MASK                                     0x00001FF0
#define B01_Q_MANAGER_CPU3_AGER_CHECK_MOST_STATUS_FIELD_MOST_PD_TID_MASK                                          0x000F0000
#define B01_Q_MANAGER_CPU3_AGER_CHECK_MOST_STATUS_FIELD_MOST_PD_STA_MASK                                          0x1FF00000
//========================================
/* REG_Q_MANAGER_CPU3_AGER_UMAC_DONE_COUNTER 0x220 */
#define B01_Q_MANAGER_CPU3_AGER_UMAC_DONE_COUNTER_FIELD_UMAC_COUNTER_MASK                                         0x0001FFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_LIB_DONE_COUNTER 0x224 */
#define B01_Q_MANAGER_CPU3_AGER_LIB_DONE_COUNTER_FIELD_LIB_COUNTER_MASK                                           0x0001FFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_TRY_FAIL_COUNTER 0x228 */
#define B01_Q_MANAGER_CPU3_AGER_TRY_FAIL_COUNTER_FIELD_TRY_FAIL_COUNTER_MASK                                      0x0001FFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_COUNTER_CLR 0x22C */
#define B01_Q_MANAGER_CPU3_AGER_COUNTER_CLR_FIELD_UMAC_CLEAR_MASK                                                 0x00000001
#define B01_Q_MANAGER_CPU3_AGER_COUNTER_CLR_FIELD_LIB_CLEAR_MASK                                                  0x00000002
#define B01_Q_MANAGER_CPU3_AGER_COUNTER_CLR_FIELD_TRY_FAIL_CLEAR_MASK                                             0x00000004
//========================================
/* REG_Q_MANAGER_CPU3_AGER_AC_LEAVE_CRITERIA 0x230 */
#define B01_Q_MANAGER_CPU3_AGER_AC_LEAVE_CRITERIA_FIELD_BE_CRITERIA_MASK                                          0x0000007F
#define B01_Q_MANAGER_CPU3_AGER_AC_LEAVE_CRITERIA_FIELD_BK_CRITERIA_MASK                                          0x00007F00
#define B01_Q_MANAGER_CPU3_AGER_AC_LEAVE_CRITERIA_FIELD_VI_CRITERIA_MASK                                          0x007F0000
#define B01_Q_MANAGER_CPU3_AGER_AC_LEAVE_CRITERIA_FIELD_VO_CRITERIA_MASK                                          0x7F000000
//========================================
/* REG_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA0 0x234 */
#define B01_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA0_FIELD_BE_THRESH_CRITERIA_MASK                                 0x0001FFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA1 0x238 */
#define B01_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA1_FIELD_BK_THRESH_CRITERIA_MASK                                 0x0001FFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA2 0x23C */
#define B01_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA2_FIELD_VI_THRESH_CRITERIA_MASK                                 0x0001FFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA3 0x240 */
#define B01_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA3_FIELD_VO_THRESH_CRITERIA_MASK                                 0x0001FFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_SINGLE_PD_LIMIT 0x244 */
#define B01_Q_MANAGER_CPU3_AGER_SINGLE_PD_LIMIT_FIELD_SINGLE_PD_DISCARD_LIMIT_MASK                                0x0001FFFF
#define B01_Q_MANAGER_CPU3_AGER_SINGLE_PD_LIMIT_FIELD_SINGLE_PD_DISCARD_LIMIT_EN_MASK                             0x00100000
//========================================
/* REG_Q_MANAGER_CPU3_AGER_STA0_31_STATUS 0x248 */
#define B01_Q_MANAGER_CPU3_AGER_STA0_31_STATUS_FIELD_AGE_CHECK_STA0_31_STATUS_MASK                                0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_STA32_63_STATUS 0x24C */
#define B01_Q_MANAGER_CPU3_AGER_STA32_63_STATUS_FIELD_AGE_CHECK_STA32_63_STATUS_MASK                              0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_STA64_95_STATUS 0x250 */
#define B01_Q_MANAGER_CPU3_AGER_STA64_95_STATUS_FIELD_AGE_CHECK_STA64_95_STATUS_MASK                              0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_STA96_127_STATUS 0x254 */
#define B01_Q_MANAGER_CPU3_AGER_STA96_127_STATUS_FIELD_AGE_CHECK_STA96_127_STATUS_MASK                            0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_STA128_159_STATUS 0x258 */
#define B01_Q_MANAGER_CPU3_AGER_STA128_159_STATUS_FIELD_AGE_CHECK_STA128_159_STATUS_MASK                          0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_STA160_191_STATUS 0x25C */
#define B01_Q_MANAGER_CPU3_AGER_STA160_191_STATUS_FIELD_AGE_CHECK_STA160_191_STATUS_MASK                          0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_STA192_223_STATUS 0x260 */
#define B01_Q_MANAGER_CPU3_AGER_STA192_223_STATUS_FIELD_AGE_CHECK_STA192_223_STATUS_MASK                          0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_STA224_255_STATUS 0x264 */
#define B01_Q_MANAGER_CPU3_AGER_STA224_255_STATUS_FIELD_AGE_CHECK_STA224_255_STATUS_MASK                          0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_VAP_STATUS 0x268 */
#define B01_Q_MANAGER_CPU3_AGER_VAP_STATUS_FIELD_AGE_CHECK_VAP_STATUS_MASK                                        0xFFFFFFFF
//========================================
/* REG_Q_MANAGER_CPU3_AGER_IRQ_CLR 0x26C */
#define B01_Q_MANAGER_CPU3_AGER_IRQ_CLR_FIELD_AGER_DONE_IRQ_CLR_MASK                                              0x00000001
//========================================
/* REG_Q_MANAGER_CPU3_AGER_TMP_LIST_CFG 0x270 */
#define B01_Q_MANAGER_CPU3_AGER_TMP_LIST_CFG_FIELD_UMAC_TMP_LIST_IDX_MASK                                         0x0000007F
//========================================
/* REG_Q_MANAGER_CPU3_LOGGER_AGER_PRIORITY 0x274 */
#define B01_Q_MANAGER_CPU3_LOGGER_AGER_PRIORITY_FIELD_AGER_LOGGER_PRIORITY_MASK                                   0x03000000
//========================================
/* REG_Q_MANAGER_CPU3_LOGGER_AGER_CFG 0x278 */
#define B01_Q_MANAGER_CPU3_LOGGER_AGER_CFG_FIELD_AGER_LOGGER_EN_MASK                                              0x10000000
//========================================
/* REG_Q_MANAGER_CPU3_LOGGER_AGER_BUSY 0x27C */
#define B01_Q_MANAGER_CPU3_LOGGER_AGER_BUSY_FIELD_AGER_LOGGER_BUSY_MASK                                           0x80000000
//========================================
/* REG_Q_MANAGER_CPU3_AGER_STA_TTL 0x280 */
#define B01_Q_MANAGER_CPU3_AGER_STA_TTL_FIELD_STA_TTL_VALUE_WR_MASK                                               0x0000007F
#define B01_Q_MANAGER_CPU3_AGER_STA_TTL_FIELD_STA_TTL_VALUE_RD_MASK                                               0x00007F00
#define B01_Q_MANAGER_CPU3_AGER_STA_TTL_FIELD_STA_TTL_STA_IDX_MASK                                                0x01FF0000
#define B01_Q_MANAGER_CPU3_AGER_STA_TTL_FIELD_STA_TTL_WR_MASK                                                     0x10000000
#define B01_Q_MANAGER_CPU3_AGER_STA_TTL_FIELD_STA_TTL_DONE_MASK                                                   0x80000000


#endif // _Q_MANAGER_CPU3_REGS_H_
