
/***********************************************************************************
File:				B2QManagerCpu2Regs.h
Module:				b2QManagerCpu2
SOC Revision:		latest
Generated at:       2022-05-04 10:58:25
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_Q_MANAGER_CPU2_REGS_H_
#define _B2_Q_MANAGER_CPU2_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_Q_MANAGER_CPU2_BASE_ADDRESS                             MEMORY_MAP_UNIT_1222_BASE_ADDRESS
#define	REG_B2_Q_MANAGER_CPU2_CTL                               (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x0)
#define	REG_B2_Q_MANAGER_CPU2_PUSH_ADDR_TOP                     (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x4)
#define	REG_B2_Q_MANAGER_CPU2_PUSH_ADDR_BOTTOM                  (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x8)
#define	REG_B2_Q_MANAGER_CPU2_POP_ADDR_TOP                      (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0xC)
#define	REG_B2_Q_MANAGER_CPU2_POP_ADDR_BOTTOM                   (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x10)
#define	REG_B2_Q_MANAGER_CPU2_CLEAN_FIFO                        (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x14)
#define	REG_B2_Q_MANAGER_CPU2_FIFO_EN                           (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x18)
#define	REG_B2_Q_MANAGER_CPU2_INT_CTL                           (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x1C)
#define	REG_B2_Q_MANAGER_CPU2_INT_PUSH_ADDR_TOP                 (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x20)
#define	REG_B2_Q_MANAGER_CPU2_INT_PUSH_ADDR_BOTTOM              (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x24)
#define	REG_B2_Q_MANAGER_CPU2_INT_POP_ADDR_TOP                  (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x28)
#define	REG_B2_Q_MANAGER_CPU2_INT_POP_ADDR_BOTTOM               (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x2C)
#define	REG_B2_Q_MANAGER_CPU2_INT_STATUS                        (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x30)
#define	REG_B2_Q_MANAGER_CPU2_TX_LISTS_EMPTY_INT_EN0_31         (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x38)
#define	REG_B2_Q_MANAGER_CPU2_TX_LISTS_EMPTY_INT_EN32_63        (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x3C)
#define	REG_B2_Q_MANAGER_CPU2_TX_LISTS_NOT_EMPTY_INT_EN0_31     (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x40)
#define	REG_B2_Q_MANAGER_CPU2_TX_LISTS_NOT_EMPTY_INT_EN32_63    (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x44)
#define	REG_B2_Q_MANAGER_CPU2_RX_LISTS_EMPTY_INT_EN0_31         (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x48)
#define	REG_B2_Q_MANAGER_CPU2_RX_LISTS_EMPTY_INT_EN32_63        (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x4C)
#define	REG_B2_Q_MANAGER_CPU2_RX_LISTS_NOT_EMPTY_INT_EN0_31     (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x50)
#define	REG_B2_Q_MANAGER_CPU2_RX_LISTS_NOT_EMPTY_INT_EN32_63    (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x54)
#define	REG_B2_Q_MANAGER_CPU2_TX_DONE_INT                       (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x58)
#define	REG_B2_Q_MANAGER_CPU2_RX_DONE_INT                       (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x5C)
#define	REG_B2_Q_MANAGER_CPU2_LO_PRI_TX_READY_INT               (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x60)
#define	REG_B2_Q_MANAGER_CPU2_LO_PRI_RX_READY_INT               (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x64)
#define	REG_B2_Q_MANAGER_CPU2_HI_PRI_TX_READY_INT               (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x68)
#define	REG_B2_Q_MANAGER_CPU2_HI_PRI_RX_READY_INT               (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x6C)
#define	REG_B2_Q_MANAGER_CPU2_DESC_POOL_INT_TX                  (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x70)
#define	REG_B2_Q_MANAGER_CPU2_DESC_POOL_INT_RX                  (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x74)
#define	REG_B2_Q_MANAGER_CPU2_TX_INT_CLR0_31                    (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x78)
#define	REG_B2_Q_MANAGER_CPU2_TX_INT_CLR32_63                   (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x7C)
#define	REG_B2_Q_MANAGER_CPU2_RX_INT_CLR0_31                    (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x80)
#define	REG_B2_Q_MANAGER_CPU2_RX_INT_CLR32_63                   (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x84)
#define	REG_B2_Q_MANAGER_CPU2_DISABLE_NOT_EMPTY_INT_EN          (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x88)
#define	REG_B2_Q_MANAGER_CPU2_DISABLE_INT_CLR                   (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x8C)
#define	REG_B2_Q_MANAGER_CPU2_DISABLE_INT                       (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x90)
#define	REG_B2_Q_MANAGER_CPU2_FIFO_STATUS                       (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x94)
#define	REG_B2_Q_MANAGER_CPU2_ERR                               (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x98)
#define	REG_B2_Q_MANAGER_CPU2_ERR_CLR                           (B2_Q_MANAGER_CPU2_BASE_ADDRESS + 0x9C)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_Q_MANAGER_CPU2_CTL 0x0 */
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
		uint32 ctlReq : 4; //DLM Request: , 0b000: Push packet to tail , 0b001: Push packet to head , 0b010: Pop packet , 0b011: Return head packet (Peek) , 0b100: Push packet list to tail , 0b101: Push packet list to head , 0b110: Pop packet list , 0b111: Flush , , reset value: 0x0, access type: RW
	} bitFields;
} RegB2QManagerCpu2Ctl_u;

/*REG_B2_Q_MANAGER_CPU2_PUSH_ADDR_TOP 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pushAddrTop : 24; //Top address pointer, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2QManagerCpu2PushAddrTop_u;

/*REG_B2_Q_MANAGER_CPU2_PUSH_ADDR_BOTTOM 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pushAddrBottom : 24; //Bottom address pointer, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2QManagerCpu2PushAddrBottom_u;

/*REG_B2_Q_MANAGER_CPU2_POP_ADDR_TOP 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 popAddrTop : 24; //Requested pop head address pointer., reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB2QManagerCpu2PopAddrTop_u;

/*REG_B2_Q_MANAGER_CPU2_POP_ADDR_BOTTOM 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 popAddrBottom : 24; //Requested pop tail address pointer., reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB2QManagerCpu2PopAddrBottom_u;

/*REG_B2_Q_MANAGER_CPU2_CLEAN_FIFO 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cleanFifo : 1; //Cleans FIFO, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2QManagerCpu2CleanFifo_u;

/*REG_B2_Q_MANAGER_CPU2_FIFO_EN 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifoEn : 1; //Enables FIFO, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2QManagerCpu2FifoEn_u;

/*REG_B2_Q_MANAGER_CPU2_INT_CTL 0x1C */
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
		uint32 intCtlDlmIdx : 4; //DLM index number. Values can be 0-8: , 0x0: TX Data DLM , 0x1: TX Lists DLM , 0x2: RX Data DLM , 0x3: RX Lists DLM , 0x4: DMA Lists DLM , 0x5: RX MPDU Descriptor Band0 Lists DLM , 0x6: RX MPDU Descriptor Band1 Lists DLM , 0x7: TX MPDU Descriptor Band0 Lists DLM , 0x8: TX MPDU Descriptor Band1 Lists DLM , , reset value: 0x0, access type: RW
		uint32 intCtlReq : 4; //DLM Request: , 0b000: Push packet to tail , 0b001: Push packet to head , 0b010: Pop packet , 0b011: Return head packet (Peek) , 0b100: Push packet list to tail , 0b101: Push packet list to head , 0b110: Pop packet list , 0b111: Flush , , reset value: 0x0, access type: RW
	} bitFields;
} RegB2QManagerCpu2IntCtl_u;

/*REG_B2_Q_MANAGER_CPU2_INT_PUSH_ADDR_TOP 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intPushAddrTop : 24; //Top address pointer, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2QManagerCpu2IntPushAddrTop_u;

/*REG_B2_Q_MANAGER_CPU2_INT_PUSH_ADDR_BOTTOM 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intPushAddrBottom : 24; //Bottom address pointer, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2QManagerCpu2IntPushAddrBottom_u;

/*REG_B2_Q_MANAGER_CPU2_INT_POP_ADDR_TOP 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intPopAddrTop : 24; //Requested pop head address pointer., reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB2QManagerCpu2IntPopAddrTop_u;

/*REG_B2_Q_MANAGER_CPU2_INT_POP_ADDR_BOTTOM 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intPopAddrBottom : 24; //Requested pop tail address pointer., reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB2QManagerCpu2IntPopAddrBottom_u;

/*REG_B2_Q_MANAGER_CPU2_INT_STATUS 0x30 */
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
} RegB2QManagerCpu2IntStatus_u;

/*REG_B2_Q_MANAGER_CPU2_TX_LISTS_EMPTY_INT_EN0_31 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsEmptyIntEn031 : 32; //Empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB2QManagerCpu2TxListsEmptyIntEn031_u;

/*REG_B2_Q_MANAGER_CPU2_TX_LISTS_EMPTY_INT_EN32_63 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsEmptyIntEn3263 : 32; //Empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB2QManagerCpu2TxListsEmptyIntEn3263_u;

/*REG_B2_Q_MANAGER_CPU2_TX_LISTS_NOT_EMPTY_INT_EN0_31 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsNotEmptyIntEn031 : 32; //Not empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB2QManagerCpu2TxListsNotEmptyIntEn031_u;

/*REG_B2_Q_MANAGER_CPU2_TX_LISTS_NOT_EMPTY_INT_EN32_63 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsNotEmptyIntEn3263 : 32; //Not empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB2QManagerCpu2TxListsNotEmptyIntEn3263_u;

/*REG_B2_Q_MANAGER_CPU2_RX_LISTS_EMPTY_INT_EN0_31 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsEmptyIntEn031 : 32; //Empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB2QManagerCpu2RxListsEmptyIntEn031_u;

/*REG_B2_Q_MANAGER_CPU2_RX_LISTS_EMPTY_INT_EN32_63 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsEmptyIntEn3263 : 32; //Empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB2QManagerCpu2RxListsEmptyIntEn3263_u;

/*REG_B2_Q_MANAGER_CPU2_RX_LISTS_NOT_EMPTY_INT_EN0_31 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsNotEmptyIntEn031 : 32; //Not empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB2QManagerCpu2RxListsNotEmptyIntEn031_u;

/*REG_B2_Q_MANAGER_CPU2_RX_LISTS_NOT_EMPTY_INT_EN32_63 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsNotEmptyIntEn3263 : 32; //Not empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB2QManagerCpu2RxListsNotEmptyIntEn3263_u;

/*REG_B2_Q_MANAGER_CPU2_TX_DONE_INT 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDoneListsEmptyInt : 16; //TX done lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 txDoneListsNotEmptyInt : 16; //TX done lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB2QManagerCpu2TxDoneInt_u;

/*REG_B2_Q_MANAGER_CPU2_RX_DONE_INT 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxDoneListsEmptyInt : 16; //RX done lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 rxDoneListsNotEmptyInt : 16; //RX done lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB2QManagerCpu2RxDoneInt_u;

/*REG_B2_Q_MANAGER_CPU2_LO_PRI_TX_READY_INT 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txLoPriReadyEmptyInt : 16; //TX Low priority ready lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 txLoPriReadyNotEmptyInt : 16; //TX Low priority ready lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB2QManagerCpu2LoPriTxReadyInt_u;

/*REG_B2_Q_MANAGER_CPU2_LO_PRI_RX_READY_INT 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxLoPriReadyEmptyInt : 16; //RX Low priority ready lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 rxLoPriReadyNotEmptyInt : 16; //RX Low priority ready lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB2QManagerCpu2LoPriRxReadyInt_u;

/*REG_B2_Q_MANAGER_CPU2_HI_PRI_TX_READY_INT 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txHiPriReadyEmptyInt : 16; //TX high priority ready lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 txHiPriReadyNotEmptyInt : 16; //TX high priority ready lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB2QManagerCpu2HiPriTxReadyInt_u;

/*REG_B2_Q_MANAGER_CPU2_HI_PRI_RX_READY_INT 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxHiPriReadyEmptyInt : 16; //RX high priority ready lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 rxHiPriReadyNotEmptyInt : 16; //RX high priority ready lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB2QManagerCpu2HiPriRxReadyInt_u;

/*REG_B2_Q_MANAGER_CPU2_DESC_POOL_INT_TX 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descTxPoolEmptyInt : 16; //Descriptor pool TX lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 descTxPoolNotEmptyInt : 16; //Descriptor pool TX lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB2QManagerCpu2DescPoolIntTx_u;

/*REG_B2_Q_MANAGER_CPU2_DESC_POOL_INT_RX 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descRxPoolEmptyInt : 16; //Descriptor pool RX lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 descRxPoolNotEmptyInt : 16; //Descriptor pool RX lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB2QManagerCpu2DescPoolIntRx_u;

/*REG_B2_Q_MANAGER_CPU2_TX_INT_CLR0_31 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDoneListsIntClr : 16; //Clear TX Done lists interrupt, reset value: 0x0, access type: WO
		uint32 loPriTxReadyListsIntClr : 16; //Clear Low priority TX Ready lists interrupt, reset value: 0x0, access type: WO
	} bitFields;
} RegB2QManagerCpu2TxIntClr031_u;

/*REG_B2_Q_MANAGER_CPU2_TX_INT_CLR32_63 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hiPriTxReadyListsIntClr : 16; //Clear High priority TXPD ready lists interrupt, reset value: 0x0, access type: WO
		uint32 descPoolIntClrTx : 16; //Clear TX pool lists interrupt, reset value: 0x0, access type: WO
	} bitFields;
} RegB2QManagerCpu2TxIntClr3263_u;

/*REG_B2_Q_MANAGER_CPU2_RX_INT_CLR0_31 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxDoneListsIntClr : 16; //Clear RX Done lists interrupt, reset value: 0x0, access type: WO
		uint32 loPriRxReadyListsIntClr : 16; //Clear Low priority RX Ready lists interrupt, reset value: 0x0, access type: WO
	} bitFields;
} RegB2QManagerCpu2RxIntClr031_u;

/*REG_B2_Q_MANAGER_CPU2_RX_INT_CLR32_63 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hiPriRxReadyListsIntClr : 16; //Clear High priority RX Ready lists interrupt, reset value: 0x0, access type: WO
		uint32 descPoolIntClrRx : 16; //Clear pool RX Ready lists interrupt, reset value: 0x0, access type: WO
	} bitFields;
} RegB2QManagerCpu2RxIntClr3263_u;

/*REG_B2_Q_MANAGER_CPU2_DISABLE_NOT_EMPTY_INT_EN 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disQueueNotEmptyIntEn : 16; //Not empty interrupt Enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2QManagerCpu2DisableNotEmptyIntEn_u;

/*REG_B2_Q_MANAGER_CPU2_DISABLE_INT_CLR 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disQueueNotEmptyIntClr : 16; //Clear High priority TXPD Done lists interrupt, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2QManagerCpu2DisableIntClr_u;

/*REG_B2_Q_MANAGER_CPU2_DISABLE_INT 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disQueueNotEmptyInt : 16; //Descriptor pool TX lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2QManagerCpu2DisableInt_u;

/*REG_B2_Q_MANAGER_CPU2_FIFO_STATUS 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swFifoHfull : 1; //SW FIFO half full indication, reset value: 0x0, access type: RO
		uint32 swFifoFull : 1; //SW FIFO full indication, reset value: 0x0, access type: RO
		uint32 swFifoEmpty : 1; //SW FIFO empty indication, reset value: 0x1, access type: RO
		uint32 swFifoPushWhileFull : 1; //SW FIFO overflow push when FIFO is full, reset value: 0x0, access type: RO
		uint32 swFifoPopWhileEmpty : 1; //SW FIFO underflow pop when there is no data, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB2QManagerCpu2FifoStatus_u;

/*REG_B2_Q_MANAGER_CPU2_ERR 0x98 */
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
} RegB2QManagerCpu2Err_u;

/*REG_B2_Q_MANAGER_CPU2_ERR_CLR 0x9C */
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
} RegB2QManagerCpu2ErrClr_u;



#endif // _B2_Q_MANAGER_CPU2_REGS_H_
