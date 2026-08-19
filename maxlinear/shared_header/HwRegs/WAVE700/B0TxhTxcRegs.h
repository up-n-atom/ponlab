
/***********************************************************************************
File:				B0TxhTxcRegs.h
Module:				b0TxhTxc
SOC Revision:		latest
Generated at:       2022-05-04 10:57:31
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_TXH_TXC_REGS_H_
#define _B0_TXH_TXC_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_TXH_TXC_BASE_ADDRESS                             MEMORY_MAP_UNIT_53_BASE_ADDRESS
#define	REG_B0_TXH_TXC_TXH_TXC_ENABLE               (B0_TXH_TXC_BASE_ADDRESS + 0x0)
#define	REG_B0_TXH_TXC_TXH_TXC_CONTROL              (B0_TXH_TXC_BASE_ADDRESS + 0x4)
#define	REG_B0_TXH_TXC_SIFS_ELEMENT                 (B0_TXH_TXC_BASE_ADDRESS + 0x10)
#define	REG_B0_TXH_TXC_TXC_SENSITIVE_DEFAULT        (B0_TXH_TXC_BASE_ADDRESS + 0x20)
#define	REG_B0_TXH_TXC_RESP_TIMEOUT_VALUE_SET       (B0_TXH_TXC_BASE_ADDRESS + 0x24)
#define	REG_B0_TXH_TXC_RESP_TIMEOUT_RD              (B0_TXH_TXC_BASE_ADDRESS + 0x28)
#define	REG_B0_TXH_TXC_TXC_DEBUG                    (B0_TXH_TXC_BASE_ADDRESS + 0x2C)
#define	REG_B0_TXH_TXC_START_SLOT_AFTER_SIFS_CTR    (B0_TXH_TXC_BASE_ADDRESS + 0x30)
#define	REG_B0_TXH_TXC_START_SLOT_AFTER_SLOT_CTR    (B0_TXH_TXC_BASE_ADDRESS + 0x34)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_TXH_TXC_TXH_TXC_ENABLE 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhTxcEnable : 1; //TXH TXC enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxhTxcTxhTxcEnable_u;

/*REG_B0_TXH_TXC_TXH_TXC_CONTROL 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 abortEifsOnCcaBusyEn : 1; //0: Abort EIFS counting on new PHY-MAC RX session. , 1: Abort EIFS counting on new PHY-MAC RX session OR CCA primary busy., reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0TxhTxcTxhTxcControl_u;

/*REG_B0_TXH_TXC_SIFS_ELEMENT 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sifsElement : 8; //SIFS element, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB0TxhTxcSifsElement_u;

/*REG_B0_TXH_TXC_TXC_SENSITIVE_DEFAULT 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txcCcaSensitiveDefault : 1; //CCA sensitive default, reset value: 0x1, access type: RW
		uint32 txcRxNavSensitiveDefault : 1; //RX NAV sensitive default, reset value: 0x1, access type: RW
		uint32 txcTxNavSensitiveDefault : 1; //TX NAV sensitive default, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB0TxhTxcTxcSensitiveDefault_u;

/*REG_B0_TXH_TXC_RESP_TIMEOUT_VALUE_SET 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 respTimeoutValueSet : 13; //Response timeout set, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegB0TxhTxcRespTimeoutValueSet_u;

/*REG_B0_TXH_TXC_RESP_TIMEOUT_RD 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 respTimeoutRd : 13; //Response timeout read, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 respTimeoutEnRd : 1; //Response timeout enable read, reset value: 0x0, access type: RO
		uint32 reserved1 : 15;
	} bitFields;
} RegB0TxhTxcRespTimeoutRd_u;

/*REG_B0_TXH_TXC_TXC_DEBUG 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txcSm : 4; //TXC state machine, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB0TxhTxcTxcDebug_u;

/*REG_B0_TXH_TXC_START_SLOT_AFTER_SIFS_CTR 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 startSlotAfterSifsCtr : 16; //Counter of Start SLOT after SIFS events from TXC to TxH, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0TxhTxcStartSlotAfterSifsCtr_u;

/*REG_B0_TXH_TXC_START_SLOT_AFTER_SLOT_CTR 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 startSlotAfterSlotCtr : 24; //Counter of Start SLOT after SLOT events from TXC to TxH, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB0TxhTxcStartSlotAfterSlotCtr_u;



#endif // _B0_TXH_TXC_REGS_H_
