
/***********************************************************************************
File:				B0HobPacTimRegs.h
Module:				b0HobPacTim
SOC Revision:		latest
Generated at:       2022-05-04 10:57:08
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_HOB_PAC_TIM_REGS_H_
#define _B0_HOB_PAC_TIM_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_HOB_PAC_TIM_BASE_ADDRESS                             MEMORY_MAP_UNIT_502_BASE_ADDRESS
#define	REG_B0_HOB_PAC_TIM_LATCHED_TST_FIME_LOW     (B0_HOB_PAC_TIM_BASE_ADDRESS + 0x0)
#define	REG_B0_HOB_PAC_TIM_LATCHED_TST_FIME_HIGH    (B0_HOB_PAC_TIM_BASE_ADDRESS + 0x4)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_HOB_PAC_TIM_LATCHED_TST_FIME_LOW 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 latchedTstFimeLow : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0HobPacTimLatchedTstFimeLow_u;

/*REG_B0_HOB_PAC_TIM_LATCHED_TST_FIME_HIGH 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 latchedTstFimeHigh : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB0HobPacTimLatchedTstFimeHigh_u;



#endif // _B0_HOB_PAC_TIM_REGS_H_
