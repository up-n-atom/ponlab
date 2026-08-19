
/***********************************************************************************
File:				B0MacGenriscRxRegs.h
Module:				b0MacGenriscRx
SOC Revision:		latest
Generated at:       2024-06-26 12:54:25
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_MAC_GENRISC_RX_REGS_H_
#define _B0_MAC_GENRISC_RX_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_MAC_GENRISC_RX_BASE_ADDRESS             MEMORY_MAP_UNIT_27_BASE_ADDRESS
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER0           (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x0)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER1           (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x4)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER2           (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x8)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER3           (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0xC)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER4           (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x10)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER5           (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x14)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER6           (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x18)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER7           (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x1C)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER8           (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x20)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER9           (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x24)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER10          (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x28)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER11          (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x2C)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER12          (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x30)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER13          (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x34)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER14          (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x38)
#define	REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER15          (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x3C)
#define	REG_B0_MAC_GENRISC_RX_STATUS_REGISTER              (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x40)
#define	REG_B0_MAC_GENRISC_RX_ERR_REG_0                    (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x44)
#define	REG_B0_MAC_GENRISC_RX_ERR_REG_1                    (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x48)
#define	REG_B0_MAC_GENRISC_RX_LAST_PC_EXECUTED             (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x4C)
#define	REG_B0_MAC_GENRISC_RX_ADD_OUT_ABORT                (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x54)
#define	REG_B0_MAC_GENRISC_RX_STOP_OP                      (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x60)
#define	REG_B0_MAC_GENRISC_RX_CONTINUE_OP                  (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x64)
#define	REG_B0_MAC_GENRISC_RX_SINGLE_STEP                  (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x68)
#define	REG_B0_MAC_GENRISC_RX_EXT_COMMAND                  (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x6C)
#define	REG_B0_MAC_GENRISC_RX_STEP_COMMAND                 (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x70)
#define	REG_B0_MAC_GENRISC_RX_BRKP_ADDRESS                 (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x74)
#define	REG_B0_MAC_GENRISC_RX_BRKP_ADDRESS_EN              (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x7C)
#define	REG_B0_MAC_GENRISC_RX_TEST_BUS_DATA                (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x80)
#define	REG_B0_MAC_GENRISC_RX_TEST_BUS_ENABLE              (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x84)
#define	REG_B0_MAC_GENRISC_RX_INTTEUPTS_SAMPLE             (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x88)
#define	REG_B0_MAC_GENRISC_RX_START_OP                     (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x90)
#define	REG_B0_MAC_GENRISC_RX_ABORT_CNT_LIMIT              (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x94)
#define	REG_B0_MAC_GENRISC_RX_CPU2GENRISC_DEBUG_MODE_EN    (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x98)
#define	REG_B0_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_ENABLE     (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0x9C)
#define	REG_B0_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_ENABLE     (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0xA0)
#define	REG_B0_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_SET        (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0xA4)
#define	REG_B0_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_CLR        (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0xA8)
#define	REG_B0_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_STATUS     (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0xAC)
#define	REG_B0_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_SET        (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0xB0)
#define	REG_B0_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_CLR        (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0xB4)
#define	REG_B0_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_STATUS     (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0xB8)
#define	REG_B0_MAC_GENRISC_RX_MIPS2GENRISC_IRQ_SET         (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0xBC)
#define	REG_B0_MAC_GENRISC_RX_MIPS2GENRISC_IRQ_CLR         (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0xC0)
#define	REG_B0_MAC_GENRISC_RX_MIPS2GENRISC_IRQ_STATUS      (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0xC4)
#define	REG_B0_MAC_GENRISC_RX_INT_VECTOR                   (B0_MAC_GENRISC_RX_BASE_ADDRESS + 0xC8)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER0 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister0 : 32; //Internal Register 0, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister0_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER1 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister1 : 32; //Internal Register 1, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister1_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER2 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister2 : 32; //Internal Register 2, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister2_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER3 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister3 : 32; //Internal Register 3, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister3_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER4 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister4 : 32; //Internal Register 4, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister4_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER5 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister5 : 32; //Internal Register 5, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister5_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER6 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister6 : 32; //Internal Register 6, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister6_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER7 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister7 : 32; //Internal Register 7, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister7_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER8 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister8 : 32; //Internal Register 8, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister8_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER9 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister9 : 32; //Internal Register 9, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister9_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER10 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister10 : 32; //Internal Register 10, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister10_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER11 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister11 : 32; //Internal Register 11, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister11_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER12 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister12 : 32; //Internal Register 12, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister12_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER13 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister13 : 32; //Internal Register 13, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister13_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER14 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister14 : 32; //Internal Register 14, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister14_u;

/*REG_B0_MAC_GENRISC_RX_INTERNAL_REGISTER15 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 internalRegister15 : 32; //Internal Register 15, reset value: 0x0, access type: RO
	} bitFields;
} RegB0MacGenriscRxInternalRegister15_u;

/*REG_B0_MAC_GENRISC_RX_STATUS_REGISTER 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 statusEqual : 1; //status equal bit, reset value: 0x0, access type: RO
		uint32 statusBig : 1; //status big bit, reset value: 0x0, access type: RO
		uint32 statusLittle : 1; //status little bit, reset value: 0x0, access type: RO
		uint32 statusNeg : 1; //status neg bit, reset value: 0x0, access type: RO
		uint32 statusCarry : 1; //status carry bit, reset value: 0x0, access type: RO
		uint32 divValid : 1; //Divider result valid, reset value: 0x1, access type: RO
		uint32 currInterrupt : 5; //Current interrupt, reset value: 0x0, access type: RO
		uint32 intActive : 1; //interrupt active, reset value: 0x0, access type: RO
		uint32 enabled : 1; //enabled, reset value: 0x0, access type: RO
		uint32 timerExpired : 1; //timer expired, reset value: 0x1, access type: RO
		uint32 haltWaitInt : 1; //halt wait intterrupt, reset value: 0x0, access type: RO
		uint32 intEn : 1; //interrupt enable, reset value: 0x0, access type: RO
		uint32 breakStall : 1; //break stall, reset value: 0x0, access type: RO
		uint32 stackError : 1; //stack error, reset value: 0x0, access type: RO
		uint32 notLegalOpcErr : 1; //not legal opcode error, reset value: 0x0, access type: RO
		uint32 abortError : 1; //abort error, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0MacGenriscRxStatusRegister_u;

/*REG_B0_MAC_GENRISC_RX_ERR_REG_0 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errReg0 : 16; //Error reg 0, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0MacGenriscRxErrReg0_u;

/*REG_B0_MAC_GENRISC_RX_ERR_REG_1 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errReg1 : 16; //Error reg 1, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0MacGenriscRxErrReg1_u;

/*REG_B0_MAC_GENRISC_RX_LAST_PC_EXECUTED 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lastExecuted : 16; //Last pc executed, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0MacGenriscRxLastPcExecuted_u;

/*REG_B0_MAC_GENRISC_RX_ADD_OUT_ABORT 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 addOutAbort : 28; //Address out abort, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB0MacGenriscRxAddOutAbort_u;

/*REG_B0_MAC_GENRISC_RX_STOP_OP 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stopOp : 1; //stop opcode, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0MacGenriscRxStopOp_u;

/*REG_B0_MAC_GENRISC_RX_CONTINUE_OP 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 continueOp : 1; //continue opcode, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0MacGenriscRxContinueOp_u;

/*REG_B0_MAC_GENRISC_RX_SINGLE_STEP 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 singleStep : 1; //single step, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0MacGenriscRxSingleStep_u;

/*REG_B0_MAC_GENRISC_RX_EXT_COMMAND 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 extCommand : 1; //ext command, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0MacGenriscRxExtCommand_u;

/*REG_B0_MAC_GENRISC_RX_STEP_COMMAND 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stepCommand : 32; //step command, reset value: 0x0, access type: RW
	} bitFields;
} RegB0MacGenriscRxStepCommand_u;

/*REG_B0_MAC_GENRISC_RX_BRKP_ADDRESS 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 brkpAddress1 : 16; //brkp address1, reset value: 0x0, access type: RW
		uint32 brkpAddress2 : 16; //brkp address2, reset value: 0x0, access type: RW
	} bitFields;
} RegB0MacGenriscRxBrkpAddress_u;

/*REG_B0_MAC_GENRISC_RX_BRKP_ADDRESS_EN 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 brkpAddress1En : 1; //brkp address1 enable, reset value: 0x0, access type: RW
		uint32 brkpAddress2En : 1; //brkp address2 enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0MacGenriscRxBrkpAddressEn_u;

/*REG_B0_MAC_GENRISC_RX_TEST_BUS_DATA 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testBusData : 24; //test bus data, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB0MacGenriscRxTestBusData_u;

/*REG_B0_MAC_GENRISC_RX_TEST_BUS_ENABLE 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 testBusEnable : 1; //test bus enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB0MacGenriscRxTestBusEnable_u;

/*REG_B0_MAC_GENRISC_RX_INTTEUPTS_SAMPLE 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intteuptsSample : 24; //intteupts sample, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegB0MacGenriscRxIntteuptsSample_u;

/*REG_B0_MAC_GENRISC_RX_START_OP 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 startOp : 1; //start opcode, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB0MacGenriscRxStartOp_u;

/*REG_B0_MAC_GENRISC_RX_ABORT_CNT_LIMIT 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 abortCntLimit : 16; //abort counter limit, reset value: 0x01ff, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0MacGenriscRxAbortCntLimit_u;

/*REG_B0_MAC_GENRISC_RX_CPU2GENRISC_DEBUG_MODE_EN 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lowerCpu2GenriscDebugModeEn : 1; //lower cpu2genrisc debug mode enable, reset value: 0x0, access type: RW
		uint32 upperCpu2GenriscDebugModeEn : 1; //Upper cpu2genrisc debug mode enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB0MacGenriscRxCpu2GenriscDebugModeEn_u;

/*REG_B0_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_ENABLE 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genriscUpperIrqEnable : 16; //GenRisc to Upper IRQ enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0MacGenriscRxGenriscUpperIrqEnable_u;

/*REG_B0_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_ENABLE 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genriscLowerIrqEnable : 16; //GenRisc to lower IRQ enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB0MacGenriscRxGenriscLowerIrqEnable_u;

/*REG_B0_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_SET 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genriscUpperIrqSet : 16; //GenRisc to Upper IRQ set, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0MacGenriscRxGenriscUpperIrqSet_u;

/*REG_B0_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_CLR 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genriscUpperIrqClr : 16; //GenRisc to Upper IRQ clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0MacGenriscRxGenriscUpperIrqClr_u;

/*REG_B0_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_STATUS 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genriscUpperIrqStatus : 16; //GenRisc to Upper IRQ status, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0MacGenriscRxGenriscUpperIrqStatus_u;

/*REG_B0_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_SET 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genriscLowerIrqSet : 16; //GenRisc to Lower IRQ set, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0MacGenriscRxGenriscLowerIrqSet_u;

/*REG_B0_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_CLR 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genriscLowerIrqClr : 16; //GenRisc to Lower IRQ clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0MacGenriscRxGenriscLowerIrqClr_u;

/*REG_B0_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_STATUS 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 genriscLowerIrqStatus : 16; //GenRisc to Lower IRQ status, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0MacGenriscRxGenriscLowerIrqStatus_u;

/*REG_B0_MAC_GENRISC_RX_MIPS2GENRISC_IRQ_SET 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mips2GenriscIrqSet : 2; //MIPS to GenRisc IRQ set, reset value: 0x0, access type: WO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0MacGenriscRxMips2GenriscIrqSet_u;

/*REG_B0_MAC_GENRISC_RX_MIPS2GENRISC_IRQ_CLR 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mips2GenriscIrqClr : 2; //MIPS to GenRisc IRQ clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0MacGenriscRxMips2GenriscIrqClr_u;

/*REG_B0_MAC_GENRISC_RX_MIPS2GENRISC_IRQ_STATUS 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mips2GenriscIrqStatus : 2; //MIPS to GenRisc IRQ status, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0MacGenriscRxMips2GenriscIrqStatus_u;

/*REG_B0_MAC_GENRISC_RX_INT_VECTOR 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 intVector : 20; //GenRisc IRQ vector, reset value: 0x0, access type: RO
		uint32 reserved0 : 12;
	} bitFields;
} RegB0MacGenriscRxIntVector_u;

//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER0 0x0 */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER0_FIELD_INTERNAL_REGISTER0_MASK                                        0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER1 0x4 */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER1_FIELD_INTERNAL_REGISTER1_MASK                                        0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER2 0x8 */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER2_FIELD_INTERNAL_REGISTER2_MASK                                        0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER3 0xC */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER3_FIELD_INTERNAL_REGISTER3_MASK                                        0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER4 0x10 */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER4_FIELD_INTERNAL_REGISTER4_MASK                                        0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER5 0x14 */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER5_FIELD_INTERNAL_REGISTER5_MASK                                        0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER6 0x18 */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER6_FIELD_INTERNAL_REGISTER6_MASK                                        0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER7 0x1C */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER7_FIELD_INTERNAL_REGISTER7_MASK                                        0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER8 0x20 */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER8_FIELD_INTERNAL_REGISTER8_MASK                                        0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER9 0x24 */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER9_FIELD_INTERNAL_REGISTER9_MASK                                        0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER10 0x28 */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER10_FIELD_INTERNAL_REGISTER10_MASK                                      0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER11 0x2C */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER11_FIELD_INTERNAL_REGISTER11_MASK                                      0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER12 0x30 */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER12_FIELD_INTERNAL_REGISTER12_MASK                                      0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER13 0x34 */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER13_FIELD_INTERNAL_REGISTER13_MASK                                      0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER14 0x38 */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER14_FIELD_INTERNAL_REGISTER14_MASK                                      0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_INTERNAL_REGISTER15 0x3C */
#define B0_MAC_GENRISC_RX_INTERNAL_REGISTER15_FIELD_INTERNAL_REGISTER15_MASK                                      0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_STATUS_REGISTER 0x40 */
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_STATUS_EQUAL_MASK                                                 0x00000001
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_STATUS_BIG_MASK                                                   0x00000002
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_STATUS_LITTLE_MASK                                                0x00000004
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_STATUS_NEG_MASK                                                   0x00000008
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_STATUS_CARRY_MASK                                                 0x00000010
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_DIV_VALID_MASK                                                    0x00000020
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_CURR_INTERRUPT_MASK                                               0x000007C0
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_INT_ACTIVE_MASK                                                   0x00000800
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_ENABLED_MASK                                                      0x00001000
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_TIMER_EXPIRED_MASK                                                0x00002000
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_HALT_WAIT_INT_MASK                                                0x00004000
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_INT_EN_MASK                                                       0x00008000
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_BREAK_STALL_MASK                                                  0x00010000
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_STACK_ERROR_MASK                                                  0x00020000
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_NOT_LEGAL_OPC_ERR_MASK                                            0x00040000
#define B0_MAC_GENRISC_RX_STATUS_REGISTER_FIELD_ABORT_ERROR_MASK                                                  0x00080000
//========================================
/* REG_MAC_GENRISC_RX_ERR_REG_0 0x44 */
#define B0_MAC_GENRISC_RX_ERR_REG_0_FIELD_ERR_REG_0_MASK                                                          0x0000FFFF
//========================================
/* REG_MAC_GENRISC_RX_ERR_REG_1 0x48 */
#define B0_MAC_GENRISC_RX_ERR_REG_1_FIELD_ERR_REG_1_MASK                                                          0x0000FFFF
//========================================
/* REG_MAC_GENRISC_RX_LAST_PC_EXECUTED 0x4C */
#define B0_MAC_GENRISC_RX_LAST_PC_EXECUTED_FIELD_LAST_EXECUTED_MASK                                               0x0000FFFF
//========================================
/* REG_MAC_GENRISC_RX_ADD_OUT_ABORT 0x54 */
#define B0_MAC_GENRISC_RX_ADD_OUT_ABORT_FIELD_ADD_OUT_ABORT_MASK                                                  0x0FFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_STOP_OP 0x60 */
#define B0_MAC_GENRISC_RX_STOP_OP_FIELD_STOP_OP_MASK                                                              0x00000001
//========================================
/* REG_MAC_GENRISC_RX_CONTINUE_OP 0x64 */
#define B0_MAC_GENRISC_RX_CONTINUE_OP_FIELD_CONTINUE_OP_MASK                                                      0x00000001
//========================================
/* REG_MAC_GENRISC_RX_SINGLE_STEP 0x68 */
#define B0_MAC_GENRISC_RX_SINGLE_STEP_FIELD_SINGLE_STEP_MASK                                                      0x00000001
//========================================
/* REG_MAC_GENRISC_RX_EXT_COMMAND 0x6C */
#define B0_MAC_GENRISC_RX_EXT_COMMAND_FIELD_EXT_COMMAND_MASK                                                      0x00000001
//========================================
/* REG_MAC_GENRISC_RX_STEP_COMMAND 0x70 */
#define B0_MAC_GENRISC_RX_STEP_COMMAND_FIELD_STEP_COMMAND_MASK                                                    0xFFFFFFFF
//========================================
/* REG_MAC_GENRISC_RX_BRKP_ADDRESS 0x74 */
#define B0_MAC_GENRISC_RX_BRKP_ADDRESS_FIELD_BRKP_ADDRESS1_MASK                                                   0x0000FFFF
#define B0_MAC_GENRISC_RX_BRKP_ADDRESS_FIELD_BRKP_ADDRESS2_MASK                                                   0xFFFF0000
//========================================
/* REG_MAC_GENRISC_RX_BRKP_ADDRESS_EN 0x7C */
#define B0_MAC_GENRISC_RX_BRKP_ADDRESS_EN_FIELD_BRKP_ADDRESS1_EN_MASK                                             0x00000001
#define B0_MAC_GENRISC_RX_BRKP_ADDRESS_EN_FIELD_BRKP_ADDRESS2_EN_MASK                                             0x00000002
//========================================
/* REG_MAC_GENRISC_RX_TEST_BUS_DATA 0x80 */
#define B0_MAC_GENRISC_RX_TEST_BUS_DATA_FIELD_TEST_BUS_DATA_MASK                                                  0x00FFFFFF
//========================================
/* REG_MAC_GENRISC_RX_TEST_BUS_ENABLE 0x84 */
#define B0_MAC_GENRISC_RX_TEST_BUS_ENABLE_FIELD_TEST_BUS_ENABLE_MASK                                              0x00000001
//========================================
/* REG_MAC_GENRISC_RX_INTTEUPTS_SAMPLE 0x88 */
#define B0_MAC_GENRISC_RX_INTTEUPTS_SAMPLE_FIELD_INTTEUPTS_SAMPLE_MASK                                            0x00FFFFFF
//========================================
/* REG_MAC_GENRISC_RX_START_OP 0x90 */
#define B0_MAC_GENRISC_RX_START_OP_FIELD_START_OP_MASK                                                            0x00000001
//========================================
/* REG_MAC_GENRISC_RX_ABORT_CNT_LIMIT 0x94 */
#define B0_MAC_GENRISC_RX_ABORT_CNT_LIMIT_FIELD_ABORT_CNT_LIMIT_MASK                                              0x0000FFFF
//========================================
/* REG_MAC_GENRISC_RX_CPU2GENRISC_DEBUG_MODE_EN 0x98 */
#define B0_MAC_GENRISC_RX_CPU2GENRISC_DEBUG_MODE_EN_FIELD_LOWER_CPU2GENRISC_DEBUG_MODE_EN_MASK                    0x00000001
#define B0_MAC_GENRISC_RX_CPU2GENRISC_DEBUG_MODE_EN_FIELD_UPPER_CPU2GENRISC_DEBUG_MODE_EN_MASK                    0x00000002
//========================================
/* REG_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_ENABLE 0x9C */
#define B0_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_ENABLE_FIELD_GENRISC_UPPER_IRQ_ENABLE_MASK                            0x0000FFFF
//========================================
/* REG_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_ENABLE 0xA0 */
#define B0_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_ENABLE_FIELD_GENRISC_LOWER_IRQ_ENABLE_MASK                            0x0000FFFF
//========================================
/* REG_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_SET 0xA4 */
#define B0_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_SET_FIELD_GENRISC_UPPER_IRQ_SET_MASK                                  0x0000FFFF
//========================================
/* REG_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_CLR 0xA8 */
#define B0_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_CLR_FIELD_GENRISC_UPPER_IRQ_CLR_MASK                                  0x0000FFFF
//========================================
/* REG_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_STATUS 0xAC */
#define B0_MAC_GENRISC_RX_GENRISC_UPPER_IRQ_STATUS_FIELD_GENRISC_UPPER_IRQ_STATUS_MASK                            0x0000FFFF
//========================================
/* REG_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_SET 0xB0 */
#define B0_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_SET_FIELD_GENRISC_LOWER_IRQ_SET_MASK                                  0x0000FFFF
//========================================
/* REG_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_CLR 0xB4 */
#define B0_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_CLR_FIELD_GENRISC_LOWER_IRQ_CLR_MASK                                  0x0000FFFF
//========================================
/* REG_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_STATUS 0xB8 */
#define B0_MAC_GENRISC_RX_GENRISC_LOWER_IRQ_STATUS_FIELD_GENRISC_LOWER_IRQ_STATUS_MASK                            0x0000FFFF
//========================================
/* REG_MAC_GENRISC_RX_MIPS2GENRISC_IRQ_SET 0xBC */
#define B0_MAC_GENRISC_RX_MIPS2GENRISC_IRQ_SET_FIELD_MIPS2GENRISC_IRQ_SET_MASK                                    0x00000003
//========================================
/* REG_MAC_GENRISC_RX_MIPS2GENRISC_IRQ_CLR 0xC0 */
#define B0_MAC_GENRISC_RX_MIPS2GENRISC_IRQ_CLR_FIELD_MIPS2GENRISC_IRQ_CLR_MASK                                    0x00000003
//========================================
/* REG_MAC_GENRISC_RX_MIPS2GENRISC_IRQ_STATUS 0xC4 */
#define B0_MAC_GENRISC_RX_MIPS2GENRISC_IRQ_STATUS_FIELD_MIPS2GENRISC_IRQ_STATUS_MASK                              0x00000003
//========================================
/* REG_MAC_GENRISC_RX_INT_VECTOR 0xC8 */
#define B0_MAC_GENRISC_RX_INT_VECTOR_FIELD_INT_VECTOR_MASK                                                        0x000FFFFF


#endif // _MAC_GENRISC_RX_REGS_H_
