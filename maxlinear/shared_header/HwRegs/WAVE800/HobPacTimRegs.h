
/***********************************************************************************
File:				HobPacTimRegs.h
Module:				hobPacTim
SOC Revision:		latest
Generated at:       2024-06-26 12:55:13
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _HOB_PAC_TIM_REGS_H_
#define _HOB_PAC_TIM_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define HOB_PAC_TIM_BASE_ADDRESS               MEMORY_MAP_UNIT_502_BASE_ADDRESS
#define	REG_HOB_PAC_TIM_LATCHED_TST_FIME_LOW     (HOB_PAC_TIM_BASE_ADDRESS + 0x0)
#define	REG_HOB_PAC_TIM_LATCHED_TST_FIME_HIGH    (HOB_PAC_TIM_BASE_ADDRESS + 0x4)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_HOB_PAC_TIM_LATCHED_TST_FIME_LOW 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 latchedTstFimeLow : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegHobPacTimLatchedTstFimeLow_u;

/*REG_HOB_PAC_TIM_LATCHED_TST_FIME_HIGH 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 latchedTstFimeHigh : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegHobPacTimLatchedTstFimeHigh_u;

//========================================
/* REG_HOB_PAC_TIM_LATCHED_TST_FIME_LOW 0x0 */
#define B0_HOB_PAC_TIM_LATCHED_TST_FIME_LOW_FIELD_LATCHED_TST_FIME_LOW_MASK                                       0xFFFFFFFF
//========================================
/* REG_HOB_PAC_TIM_LATCHED_TST_FIME_HIGH 0x4 */
#define B0_HOB_PAC_TIM_LATCHED_TST_FIME_HIGH_FIELD_LATCHED_TST_FIME_HIGH_MASK                                     0xFFFFFFFF


#endif // _HOB_PAC_TIM_REGS_H_
