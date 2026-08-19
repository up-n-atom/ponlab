
/***********************************************************************************
File:				B01QManagerCpu3Regs.h
Module:				b01QManagerCpu3
SOC Revision:		latest
Generated at:       2022-05-04 10:56:59
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B01_Q_MANAGER_CPU3_REGS_H_
#define _B01_Q_MANAGER_CPU3_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B01_Q_MANAGER_CPU3_BASE_ADDRESS                             MEMORY_MAP_UNIT_36_BASE_ADDRESS
#define	REG_B01_Q_MANAGER_CPU3_CTL                                (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x0)
#define	REG_B01_Q_MANAGER_CPU3_PUSH_ADDR_TOP                      (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x4)
#define	REG_B01_Q_MANAGER_CPU3_PUSH_ADDR_BOTTOM                   (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x8)
#define	REG_B01_Q_MANAGER_CPU3_POP_ADDR_TOP                       (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xC)
#define	REG_B01_Q_MANAGER_CPU3_POP_ADDR_BOTTOM                    (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x10)
#define	REG_B01_Q_MANAGER_CPU3_CLEAN_FIFO                         (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x14)
#define	REG_B01_Q_MANAGER_CPU3_FIFO_EN                            (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x18)
#define	REG_B01_Q_MANAGER_CPU3_INT_CTL                            (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x1C)
#define	REG_B01_Q_MANAGER_CPU3_INT_PUSH_ADDR_TOP                  (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x20)
#define	REG_B01_Q_MANAGER_CPU3_INT_PUSH_ADDR_BOTTOM               (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x24)
#define	REG_B01_Q_MANAGER_CPU3_INT_POP_ADDR_TOP                   (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x28)
#define	REG_B01_Q_MANAGER_CPU3_INT_POP_ADDR_BOTTOM                (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x2C)
#define	REG_B01_Q_MANAGER_CPU3_INT_STATUS                         (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x30)
#define	REG_B01_Q_MANAGER_CPU3_TX_LISTS_EMPTY_INT_EN0_31          (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x38)
#define	REG_B01_Q_MANAGER_CPU3_TX_LISTS_EMPTY_INT_EN32_63         (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x3C)
#define	REG_B01_Q_MANAGER_CPU3_TX_LISTS_NOT_EMPTY_INT_EN0_31      (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x40)
#define	REG_B01_Q_MANAGER_CPU3_TX_LISTS_NOT_EMPTY_INT_EN32_63     (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x44)
#define	REG_B01_Q_MANAGER_CPU3_RX_LISTS_EMPTY_INT_EN0_31          (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x48)
#define	REG_B01_Q_MANAGER_CPU3_RX_LISTS_EMPTY_INT_EN32_63         (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x4C)
#define	REG_B01_Q_MANAGER_CPU3_RX_LISTS_NOT_EMPTY_INT_EN0_31      (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x50)
#define	REG_B01_Q_MANAGER_CPU3_RX_LISTS_NOT_EMPTY_INT_EN32_63     (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x54)
#define	REG_B01_Q_MANAGER_CPU3_TX_DONE_INT                        (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x58)
#define	REG_B01_Q_MANAGER_CPU3_RX_DONE_INT                        (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x5C)
#define	REG_B01_Q_MANAGER_CPU3_LO_PRI_TX_READY_INT                (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x60)
#define	REG_B01_Q_MANAGER_CPU3_LO_PRI_RX_READY_INT                (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x64)
#define	REG_B01_Q_MANAGER_CPU3_HI_PRI_TX_READY_INT                (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x68)
#define	REG_B01_Q_MANAGER_CPU3_HI_PRI_RX_READY_INT                (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x6C)
#define	REG_B01_Q_MANAGER_CPU3_DESC_POOL_INT_TX                   (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x70)
#define	REG_B01_Q_MANAGER_CPU3_DESC_POOL_INT_RX                   (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x74)
#define	REG_B01_Q_MANAGER_CPU3_TX_INT_CLR0_31                     (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x78)
#define	REG_B01_Q_MANAGER_CPU3_TX_INT_CLR32_63                    (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x7C)
#define	REG_B01_Q_MANAGER_CPU3_RX_INT_CLR0_31                     (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x80)
#define	REG_B01_Q_MANAGER_CPU3_RX_INT_CLR32_63                    (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x84)
#define	REG_B01_Q_MANAGER_CPU3_DISABLE_NOT_EMPTY_INT_EN           (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x88)
#define	REG_B01_Q_MANAGER_CPU3_DISABLE_INT_CLR                    (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x8C)
#define	REG_B01_Q_MANAGER_CPU3_DISABLE_INT                        (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x90)
#define	REG_B01_Q_MANAGER_CPU3_FIFO_STATUS                        (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x94)
#define	REG_B01_Q_MANAGER_CPU3_ERR                                (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x98)
#define	REG_B01_Q_MANAGER_CPU3_ERR_CLR                            (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x9C)
#define	REG_B01_Q_MANAGER_CPU3_Q_MANAGER_SPARE                    (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xC0)
#define	REG_B01_Q_MANAGER_CPU3_TX_LISTS_EMPTY0_31                 (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xC4)
#define	REG_B01_Q_MANAGER_CPU3_TX_LISTS_EMPTY32_63                (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xC8)
#define	REG_B01_Q_MANAGER_CPU3_RX_LISTS_EMPTY0_31                 (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xCC)
#define	REG_B01_Q_MANAGER_CPU3_RX_LISTS_EMPTY32_63                (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xD0)
#define	REG_B01_Q_MANAGER_CPU3_DMA_LISTS_EMPTY0_31                (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xD4)
#define	REG_B01_Q_MANAGER_CPU3_DMA_LISTS_EMPTY32_63               (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xD8)
#define	REG_B01_Q_MANAGER_CPU3_DMA_LISTS_EMPTY64_81               (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xDC)
#define	REG_B01_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY0_31      (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xE0)
#define	REG_B01_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY32_63     (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xE4)
#define	REG_B01_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY64_79     (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xE8)
#define	REG_B01_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY0_31      (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xEC)
#define	REG_B01_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY32_63     (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xF0)
#define	REG_B01_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY64_79     (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xF4)
#define	REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY0_31      (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xF8)
#define	REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY32_63     (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0xFC)
#define	REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY64_95     (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x100)
#define	REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY96_115    (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x104)
#define	REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY0_31      (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x108)
#define	REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY32_63     (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x10C)
#define	REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY64_95     (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x110)
#define	REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY96_115    (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x114)
#define	REG_B01_Q_MANAGER_CPU3_AGER_EN                            (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x200)
#define	REG_B01_Q_MANAGER_CPU3_AGER_CFG                           (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x204)
#define	REG_B01_Q_MANAGER_CPU3_AGER_PD_TYPE_UMAC_CFG              (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x208)
#define	REG_B01_Q_MANAGER_CPU3_AGER_LISTS_CFG                     (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x20C)
#define	REG_B01_Q_MANAGER_CPU3_AGER_SINGLE_CHECK_CFG              (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x210)
#define	REG_B01_Q_MANAGER_CPU3_AGER_SINGLE_CHECK_ACTIVATE         (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x214)
#define	REG_B01_Q_MANAGER_CPU3_AGER_CHECK_STATUS                  (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x218)
#define	REG_B01_Q_MANAGER_CPU3_AGER_CHECK_MOST_STATUS             (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x21C)
#define	REG_B01_Q_MANAGER_CPU3_AGER_UMAC_DONE_COUNTER             (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x220)
#define	REG_B01_Q_MANAGER_CPU3_AGER_LIB_DONE_COUNTER              (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x224)
#define	REG_B01_Q_MANAGER_CPU3_AGER_TRY_FAIL_COUNTER              (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x228)
#define	REG_B01_Q_MANAGER_CPU3_AGER_COUNTER_CLR                   (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x22C)
#define	REG_B01_Q_MANAGER_CPU3_AGER_AC_LEAVE_CRITERIA             (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x230)
#define	REG_B01_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA0           (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x234)
#define	REG_B01_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA1           (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x238)
#define	REG_B01_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA2           (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x23C)
#define	REG_B01_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA3           (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x240)
#define	REG_B01_Q_MANAGER_CPU3_AGER_SINGLE_PD_LIMIT               (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x244)
#define	REG_B01_Q_MANAGER_CPU3_AGER_STA0_31_STATUS                (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x248)
#define	REG_B01_Q_MANAGER_CPU3_AGER_STA32_63_STATUS               (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x24C)
#define	REG_B01_Q_MANAGER_CPU3_AGER_STA64_95_STATUS               (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x250)
#define	REG_B01_Q_MANAGER_CPU3_AGER_STA96_127_STATUS              (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x254)
#define	REG_B01_Q_MANAGER_CPU3_AGER_STA128_159_STATUS             (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x258)
#define	REG_B01_Q_MANAGER_CPU3_AGER_STA160_191_STATUS             (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x25C)
#define	REG_B01_Q_MANAGER_CPU3_AGER_STA192_223_STATUS             (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x260)
#define	REG_B01_Q_MANAGER_CPU3_AGER_STA224_255_STATUS             (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x264)
#define	REG_B01_Q_MANAGER_CPU3_AGER_VAP_STATUS                    (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x268)
#define	REG_B01_Q_MANAGER_CPU3_AGER_IRQ_CLR                       (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x26C)
#define	REG_B01_Q_MANAGER_CPU3_AGER_TMP_LIST_CFG                  (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x270)
#define	REG_B01_Q_MANAGER_CPU3_LOGGER_AGER_PRIORITY               (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x274)
#define	REG_B01_Q_MANAGER_CPU3_LOGGER_AGER_CFG                    (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x278)
#define	REG_B01_Q_MANAGER_CPU3_LOGGER_AGER_BUSY                   (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x27C)
#define	REG_B01_Q_MANAGER_CPU3_AGER_STA_TTL                       (B01_Q_MANAGER_CPU3_BASE_ADDRESS + 0x280)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B01_Q_MANAGER_CPU3_CTL 0x0 */
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
} RegB01QManagerCpu3Ctl_u;

/*REG_B01_Q_MANAGER_CPU3_PUSH_ADDR_TOP 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pushAddrTop : 24; //Top address pointer, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB01QManagerCpu3PushAddrTop_u;

/*REG_B01_Q_MANAGER_CPU3_PUSH_ADDR_BOTTOM 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pushAddrBottom : 24; //Top address pointer, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB01QManagerCpu3PushAddrBottom_u;

/*REG_B01_Q_MANAGER_CPU3_POP_ADDR_TOP 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 popAddrTop : 24; //Requested head address pointer., reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB01QManagerCpu3PopAddrTop_u;

/*REG_B01_Q_MANAGER_CPU3_POP_ADDR_BOTTOM 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 popAddrBottom : 24; //Requested tail address pointer., reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB01QManagerCpu3PopAddrBottom_u;

/*REG_B01_Q_MANAGER_CPU3_CLEAN_FIFO 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cleanFifo : 1; //Cleans FIFO, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB01QManagerCpu3CleanFifo_u;

/*REG_B01_Q_MANAGER_CPU3_FIFO_EN 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifoEn : 1; //Enables FIFO, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB01QManagerCpu3FifoEn_u;

/*REG_B01_Q_MANAGER_CPU3_INT_CTL 0x1C */
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
		uint32 intCtlReq : 4; //DLM Request: , 0b000: Push packet to tail , 0b001: Push packet to head , 0b010: Pop packet , 0b011: Return head packet (Peek) , 0b100: Push packet list to tail , 0b101: Push packet list to head , 0b110: Pop packet list , 0b111: Flush , , reset value: 0x0, access type: RW
	} bitFields;
} RegB01QManagerCpu3IntCtl_u;

/*REG_B01_Q_MANAGER_CPU3_INT_PUSH_ADDR_TOP 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intPushAddrTop : 24; //Top address pointer, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB01QManagerCpu3IntPushAddrTop_u;

/*REG_B01_Q_MANAGER_CPU3_INT_PUSH_ADDR_BOTTOM 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intPushAddrBottom : 24; //Top address pointer, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB01QManagerCpu3IntPushAddrBottom_u;

/*REG_B01_Q_MANAGER_CPU3_INT_POP_ADDR_TOP 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intPopAddrTop : 24; //Requested head address pointer., reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB01QManagerCpu3IntPopAddrTop_u;

/*REG_B01_Q_MANAGER_CPU3_INT_POP_ADDR_BOTTOM 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intPopAddrBottom : 24; //Requested tail address pointer., reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB01QManagerCpu3IntPopAddrBottom_u;

/*REG_B01_Q_MANAGER_CPU3_INT_STATUS 0x30 */
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
} RegB01QManagerCpu3IntStatus_u;

/*REG_B01_Q_MANAGER_CPU3_TX_LISTS_EMPTY_INT_EN0_31 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsEmptyIntEn031 : 32; //Empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB01QManagerCpu3TxListsEmptyIntEn031_u;

/*REG_B01_Q_MANAGER_CPU3_TX_LISTS_EMPTY_INT_EN32_63 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsEmptyIntEn3263 : 32; //Empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB01QManagerCpu3TxListsEmptyIntEn3263_u;

/*REG_B01_Q_MANAGER_CPU3_TX_LISTS_NOT_EMPTY_INT_EN0_31 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsNotEmptyIntEn031 : 32; //Not empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB01QManagerCpu3TxListsNotEmptyIntEn031_u;

/*REG_B01_Q_MANAGER_CPU3_TX_LISTS_NOT_EMPTY_INT_EN32_63 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsNotEmptyIntEn3263 : 32; //Not empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB01QManagerCpu3TxListsNotEmptyIntEn3263_u;

/*REG_B01_Q_MANAGER_CPU3_RX_LISTS_EMPTY_INT_EN0_31 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsEmptyIntEn031 : 32; //Empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB01QManagerCpu3RxListsEmptyIntEn031_u;

/*REG_B01_Q_MANAGER_CPU3_RX_LISTS_EMPTY_INT_EN32_63 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsEmptyIntEn3263 : 32; //Empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB01QManagerCpu3RxListsEmptyIntEn3263_u;

/*REG_B01_Q_MANAGER_CPU3_RX_LISTS_NOT_EMPTY_INT_EN0_31 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsNotEmptyIntEn031 : 32; //Not empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB01QManagerCpu3RxListsNotEmptyIntEn031_u;

/*REG_B01_Q_MANAGER_CPU3_RX_LISTS_NOT_EMPTY_INT_EN32_63 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsNotEmptyIntEn3263 : 32; //Not empty interrupt Enable, reset value: 0x0, access type: RW
	} bitFields;
} RegB01QManagerCpu3RxListsNotEmptyIntEn3263_u;

/*REG_B01_Q_MANAGER_CPU3_TX_DONE_INT 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDoneListsEmptyInt : 16; //TX done lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 txDoneListsNotEmptyInt : 16; //TX done lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3TxDoneInt_u;

/*REG_B01_Q_MANAGER_CPU3_RX_DONE_INT 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxDoneListsEmptyInt : 16; //RX done lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 rxDoneListsNotEmptyInt : 16; //RX done lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3RxDoneInt_u;

/*REG_B01_Q_MANAGER_CPU3_LO_PRI_TX_READY_INT 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txLoPriReadyEmptyInt : 16; //TX Low priority ready lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 txLoPriReadyNotEmptyInt : 16; //TX Low priority ready lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3LoPriTxReadyInt_u;

/*REG_B01_Q_MANAGER_CPU3_LO_PRI_RX_READY_INT 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxLoPriReadyEmptyInt : 16; //RX Low priority ready lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 rxLoPriReadyNotEmptyInt : 16; //RX Low priority ready lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3LoPriRxReadyInt_u;

/*REG_B01_Q_MANAGER_CPU3_HI_PRI_TX_READY_INT 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txHiPriReadyEmptyInt : 16; //TX high priority ready lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 txHiPriReadyNotEmptyInt : 16; //TX high priority ready lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3HiPriTxReadyInt_u;

/*REG_B01_Q_MANAGER_CPU3_HI_PRI_RX_READY_INT 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxHiPriReadyEmptyInt : 16; //RX high priority ready lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 rxHiPriReadyNotEmptyInt : 16; //RX high priority ready lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3HiPriRxReadyInt_u;

/*REG_B01_Q_MANAGER_CPU3_DESC_POOL_INT_TX 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descTxPoolEmptyInt : 16; //Descriptor pool TX lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 descTxPoolNotEmptyInt : 16; //Descriptor pool TX lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3DescPoolIntTx_u;

/*REG_B01_Q_MANAGER_CPU3_DESC_POOL_INT_RX 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 descRxPoolEmptyInt : 16; //Descriptor pool RX lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 descRxPoolNotEmptyInt : 16; //Descriptor pool RX lists not empty Interrupts, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3DescPoolIntRx_u;

/*REG_B01_Q_MANAGER_CPU3_TX_INT_CLR0_31 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txDoneListsIntClr : 16; //Clear TX Done lists interrupt, reset value: 0x0, access type: WO
		uint32 loPriTxReadyListsIntClr : 16; //Clear Low priority TX Ready lists interrupt, reset value: 0x0, access type: WO
	} bitFields;
} RegB01QManagerCpu3TxIntClr031_u;

/*REG_B01_Q_MANAGER_CPU3_TX_INT_CLR32_63 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hiPriTxReadyListsIntClr : 16; //Clear High priority TXPD Done lists interrupt, reset value: 0x0, access type: WO
		uint32 descPoolIntClrTx : 16; //Clear TX pool lists interrupt, reset value: 0x0, access type: WO
	} bitFields;
} RegB01QManagerCpu3TxIntClr3263_u;

/*REG_B01_Q_MANAGER_CPU3_RX_INT_CLR0_31 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxDoneListsIntClr : 16; //Clear RX Done lists interrupt, reset value: 0x0, access type: WO
		uint32 loPriRxReadyListsIntClr : 16; //Clear Low priority RX ready lists interrupt, reset value: 0x0, access type: WO
	} bitFields;
} RegB01QManagerCpu3RxIntClr031_u;

/*REG_B01_Q_MANAGER_CPU3_RX_INT_CLR32_63 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hiPriRxReadyListsIntClr : 16; //Clear High priority RX Ready lists interrupt, reset value: 0x0, access type: WO
		uint32 descPoolIntClrRx : 16; //Clear RX pool lists interrupt, reset value: 0x0, access type: WO
	} bitFields;
} RegB01QManagerCpu3RxIntClr3263_u;

/*REG_B01_Q_MANAGER_CPU3_DISABLE_NOT_EMPTY_INT_EN 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disQueueNotEmptyIntEn : 16; //Not empty interrupt Enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01QManagerCpu3DisableNotEmptyIntEn_u;

/*REG_B01_Q_MANAGER_CPU3_DISABLE_INT_CLR 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disQueueNotEmptyIntClr : 16; //Clear High priority TXPD Done lists interrupt, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegB01QManagerCpu3DisableIntClr_u;

/*REG_B01_Q_MANAGER_CPU3_DISABLE_INT 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disQueueNotEmptyInt : 16; //Descriptor pool TX lists empty Interrupts, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB01QManagerCpu3DisableInt_u;

/*REG_B01_Q_MANAGER_CPU3_FIFO_STATUS 0x94 */
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
} RegB01QManagerCpu3FifoStatus_u;

/*REG_B01_Q_MANAGER_CPU3_ERR 0x98 */
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
} RegB01QManagerCpu3Err_u;

/*REG_B01_Q_MANAGER_CPU3_ERR_CLR 0x9C */
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
} RegB01QManagerCpu3ErrClr_u;

/*REG_B01_Q_MANAGER_CPU3_Q_MANAGER_SPARE 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qManagerSpare : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB01QManagerCpu3QManagerSpare_u;

/*REG_B01_Q_MANAGER_CPU3_TX_LISTS_EMPTY0_31 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsDlmEmpty031 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3TxListsEmpty031_u;

/*REG_B01_Q_MANAGER_CPU3_TX_LISTS_EMPTY32_63 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txListsDlmEmpty3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3TxListsEmpty3263_u;

/*REG_B01_Q_MANAGER_CPU3_RX_LISTS_EMPTY0_31 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsDlmEmpty031 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3RxListsEmpty031_u;

/*REG_B01_Q_MANAGER_CPU3_RX_LISTS_EMPTY32_63 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxListsDlmEmpty3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3RxListsEmpty3263_u;

/*REG_B01_Q_MANAGER_CPU3_DMA_LISTS_EMPTY0_31 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dmaListsEmpty031 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3DmaListsEmpty031_u;

/*REG_B01_Q_MANAGER_CPU3_DMA_LISTS_EMPTY32_63 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dmaListsEmpty3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3DmaListsEmpty3263_u;

/*REG_B01_Q_MANAGER_CPU3_DMA_LISTS_EMPTY64_81 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dmaListsEmpty6481 : 18; //no description, reset value: 0x3FFFF, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegB01QManagerCpu3DmaListsEmpty6481_u;

/*REG_B01_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY0_31 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxMpduDesc0ListsEmpty031 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3RxMpduDesc0ListsEmpty031_u;

/*REG_B01_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY32_63 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxMpduDesc0ListsEmpty3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3RxMpduDesc0ListsEmpty3263_u;

/*REG_B01_Q_MANAGER_CPU3_RX_MPDU_DESC0_LISTS_EMPTY64_79 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxMpduDesc0ListsEmpty6479 : 16; //no description, reset value: 0xFFFF, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB01QManagerCpu3RxMpduDesc0ListsEmpty6479_u;

/*REG_B01_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY0_31 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxMpduDesc1ListsEmpty031 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3RxMpduDesc1ListsEmpty031_u;

/*REG_B01_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY32_63 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxMpduDesc1ListsEmpty3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3RxMpduDesc1ListsEmpty3263_u;

/*REG_B01_Q_MANAGER_CPU3_RX_MPDU_DESC1_LISTS_EMPTY64_79 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxMpduDesc1ListsEmpty6479 : 16; //no description, reset value: 0xFFFF, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB01QManagerCpu3RxMpduDesc1ListsEmpty6479_u;

/*REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY0_31 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc0ListsEmpty031 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3TxMpduDesc0ListsEmpty031_u;

/*REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY32_63 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc0ListsEmpty3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3TxMpduDesc0ListsEmpty3263_u;

/*REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY64_95 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc0ListsEmpty6495 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3TxMpduDesc0ListsEmpty6495_u;

/*REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC0_LISTS_EMPTY96_115 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc0ListsEmpty96115 : 20; //no description, reset value: 0xFFFFF, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB01QManagerCpu3TxMpduDesc0ListsEmpty96115_u;

/*REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY0_31 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc1ListsEmpty031 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3TxMpduDesc1ListsEmpty031_u;

/*REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY32_63 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc1ListsEmpty3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3TxMpduDesc1ListsEmpty3263_u;

/*REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY64_95 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc1ListsEmpty6495 : 32; //no description, reset value: 0xFFFFFFFF, access type: RO
	} bitFields;
} RegB01QManagerCpu3TxMpduDesc1ListsEmpty6495_u;

/*REG_B01_Q_MANAGER_CPU3_TX_MPDU_DESC1_LISTS_EMPTY96_115 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txMpduDesc1ListsEmpty96115 : 20; //no description, reset value: 0xFFFFF, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB01QManagerCpu3TxMpduDesc1ListsEmpty96115_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_EN 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agerEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 agerSwHalt : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 23;
	} bitFields;
} RegB01QManagerCpu3AgerEn_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_CFG 0x204 */
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
		uint32 reserved3 : 3;
	} bitFields;
} RegB01QManagerCpu3AgerCfg_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_PD_TYPE_UMAC_CFG 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pdTypeUmacSel : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegB01QManagerCpu3AgerPdTypeUmacCfg_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_LISTS_CFG 0x20C */
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
} RegB01QManagerCpu3AgerListsCfg_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_SINGLE_CHECK_CFG 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 startStaIdx : 9; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 forceLock : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 15;
	} bitFields;
} RegB01QManagerCpu3AgerSingleCheckCfg_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_SINGLE_CHECK_ACTIVATE 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageSingleCheckWaitRelease : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB01QManagerCpu3AgerSingleCheckActivate_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_CHECK_STATUS 0x218 */
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
		uint32 reserved1 : 19;
	} bitFields;
} RegB01QManagerCpu3AgerCheckStatus_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_CHECK_MOST_STATUS 0x21C */
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
} RegB01QManagerCpu3AgerCheckMostStatus_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_UMAC_DONE_COUNTER 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 umacCounter : 17; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB01QManagerCpu3AgerUmacDoneCounter_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_LIB_DONE_COUNTER 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 libCounter : 17; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB01QManagerCpu3AgerLibDoneCounter_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_TRY_FAIL_COUNTER 0x228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tryFailCounter : 17; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB01QManagerCpu3AgerTryFailCounter_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_COUNTER_CLR 0x22C */
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
} RegB01QManagerCpu3AgerCounterClr_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_AC_LEAVE_CRITERIA 0x230 */
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
} RegB01QManagerCpu3AgerAcLeaveCriteria_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA0 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beThreshCriteria : 17; //no description, reset value: 0x100, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegB01QManagerCpu3AgerAcThreshCriteria0_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA1 0x238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bkThreshCriteria : 17; //no description, reset value: 0x100, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegB01QManagerCpu3AgerAcThreshCriteria1_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA2 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 viThreshCriteria : 17; //no description, reset value: 0x100, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegB01QManagerCpu3AgerAcThreshCriteria2_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_AC_THRESH_CRITERIA3 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 voThreshCriteria : 17; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegB01QManagerCpu3AgerAcThreshCriteria3_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_SINGLE_PD_LIMIT 0x244 */
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
} RegB01QManagerCpu3AgerSinglePdLimit_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_STA0_31_STATUS 0x248 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta031Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3AgerSta031Status_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_STA32_63_STATUS 0x24C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta3263Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3AgerSta3263Status_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_STA64_95_STATUS 0x250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta6495Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3AgerSta6495Status_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_STA96_127_STATUS 0x254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta96127Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3AgerSta96127Status_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_STA128_159_STATUS 0x258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta128159Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3AgerSta128159Status_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_STA160_191_STATUS 0x25C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta160191Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3AgerSta160191Status_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_STA192_223_STATUS 0x260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta192223Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3AgerSta192223Status_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_STA224_255_STATUS 0x264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckSta224255Status : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3AgerSta224255Status_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_VAP_STATUS 0x268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ageCheckVapStatus : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3AgerVapStatus_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_IRQ_CLR 0x26C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 agerDoneIrqClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB01QManagerCpu3AgerIrqClr_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_TMP_LIST_CFG 0x270 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 umacTmpListIdx : 7; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB01QManagerCpu3AgerTmpListCfg_u;

/*REG_B01_Q_MANAGER_CPU3_LOGGER_AGER_PRIORITY 0x274 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 24;
		uint32 agerLoggerPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
	} bitFields;
} RegB01QManagerCpu3LoggerAgerPriority_u;

/*REG_B01_Q_MANAGER_CPU3_LOGGER_AGER_CFG 0x278 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 28;
		uint32 agerLoggerEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
	} bitFields;
} RegB01QManagerCpu3LoggerAgerCfg_u;

/*REG_B01_Q_MANAGER_CPU3_LOGGER_AGER_BUSY 0x27C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 31;
		uint32 agerLoggerBusy : 1; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB01QManagerCpu3LoggerAgerBusy_u;

/*REG_B01_Q_MANAGER_CPU3_AGER_STA_TTL 0x280 */
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
} RegB01QManagerCpu3AgerStaTtl_u;



#endif // _B01_Q_MANAGER_CPU3_REGS_H_
