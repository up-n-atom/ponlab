
/***********************************************************************************
File:				CpuXbarStatusRegs.h
Module:				cpuXbarStatus
SOC Revision:		latest
Generated at:       2022-05-04 10:58:41
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _CPU_XBAR_STATUS_REGS_H_
#define _CPU_XBAR_STATUS_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define CPU_XBAR_STATUS_BASE_ADDRESS                             MEMORY_MAP_UNIT_1115_BASE_ADDRESS
#define	REG_CPU_XBAR_STATUS_CPU_NOC_FAULTEN    (CPU_XBAR_STATUS_BASE_ADDRESS + 0x8)
#define	REG_CPU_XBAR_STATUS_CPU_NOC_ERRVLD     (CPU_XBAR_STATUS_BASE_ADDRESS + 0xC)
#define	REG_CPU_XBAR_STATUS_CPU_NOC_ERRCLR     (CPU_XBAR_STATUS_BASE_ADDRESS + 0x10)
#define	REG_CPU_XBAR_STATUS_CPU_NOC_ERRLOG0    (CPU_XBAR_STATUS_BASE_ADDRESS + 0x14)
#define	REG_CPU_XBAR_STATUS_CPU_NOC_ERRLOG1    (CPU_XBAR_STATUS_BASE_ADDRESS + 0x18)
#define	REG_CPU_XBAR_STATUS_CPU_NOC_ERRLOG3    (CPU_XBAR_STATUS_BASE_ADDRESS + 0x20)
#define	REG_CPU_XBAR_STATUS_CPU_NOC_ERRLOG4    (CPU_XBAR_STATUS_BASE_ADDRESS + 0x24)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_CPU_XBAR_STATUS_CPU_NOC_FAULTEN 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuNocFaulten : 1; //Set to 1 to enable output signal Fault. Fault asserted when ErrVld is 1., reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegCpuXbarStatusCpuNocFaulten_u;

/*REG_CPU_XBAR_STATUS_CPU_NOC_ERRVLD 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuNocErrvld : 1; //1 indicates an error has been logged, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegCpuXbarStatusCpuNocErrvld_u;

/*REG_CPU_XBAR_STATUS_CPU_NOC_ERRCLR 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cpuNocErrclr : 1; //Set to 1 to clear ErrVld. NOTE The written value is not stored in ErrVld., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegCpuXbarStatusCpuNocErrclr_u;

/*REG_CPU_XBAR_STATUS_CPU_NOC_ERRLOG0 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lock : 1; //no description, reset value: 0x0, access type: RO
		uint32 opc : 4; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 errcode : 3; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 5;
		uint32 len1 : 12; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 3;
		uint32 format : 1; //no description, reset value: 0x1, access type: RO
	} bitFields;
} RegCpuXbarStatusCpuNocErrlog0_u;

/*REG_CPU_XBAR_STATUS_CPU_NOC_ERRLOG1 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 routeid : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegCpuXbarStatusCpuNocErrlog1_u;

/*REG_CPU_XBAR_STATUS_CPU_NOC_ERRLOG3 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 addr : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegCpuXbarStatusCpuNocErrlog3_u;

/*REG_CPU_XBAR_STATUS_CPU_NOC_ERRLOG4 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 addrMsb : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegCpuXbarStatusCpuNocErrlog4_u;



#endif // _CPU_XBAR_STATUS_REGS_H_
