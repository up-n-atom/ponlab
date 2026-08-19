
/***********************************************************************************
File:				MacXbarStatusRegs.h
Module:				macXbarStatus
SOC Revision:		latest
Generated at:       2024-06-26 12:55:07
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _MAC_XBAR_STATUS_REGS_H_
#define _MAC_XBAR_STATUS_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define MAC_XBAR_STATUS_BASE_ADDRESS     MEMORY_MAP_UNIT_1122_BASE_ADDRESS
#define	REG_MAC_XBAR_STATUS_MAC_NOC_FAULTEN    (MAC_XBAR_STATUS_BASE_ADDRESS + 0x8)
#define	REG_MAC_XBAR_STATUS_MAC_NOC_ERRVLD     (MAC_XBAR_STATUS_BASE_ADDRESS + 0xC)
#define	REG_MAC_XBAR_STATUS_MAC_NOC_ERRCLR     (MAC_XBAR_STATUS_BASE_ADDRESS + 0x10)
#define	REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG0    (MAC_XBAR_STATUS_BASE_ADDRESS + 0x14)
#define	REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG1    (MAC_XBAR_STATUS_BASE_ADDRESS + 0x18)
#define	REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG3    (MAC_XBAR_STATUS_BASE_ADDRESS + 0x20)
#define	REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG4    (MAC_XBAR_STATUS_BASE_ADDRESS + 0x24)
#define	REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG5    (MAC_XBAR_STATUS_BASE_ADDRESS + 0x28)
#define	REG_MAC_XBAR_STATUS_MAC_NOC_STALLEN    (MAC_XBAR_STATUS_BASE_ADDRESS + 0x4C)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_MAC_XBAR_STATUS_MAC_NOC_FAULTEN 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 faulten : 1; //Set to 1 to enable output signal Fault. Fault asserted when ErrVld is 1., reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegMacXbarStatusMacNocFaulten_u;

/*REG_MAC_XBAR_STATUS_MAC_NOC_ERRVLD 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errvld : 1; //1 indicates an error has been logged, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegMacXbarStatusMacNocErrvld_u;

/*REG_MAC_XBAR_STATUS_MAC_NOC_ERRCLR 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errclr : 1; //Set to 1 to clear ErrVld. NOTE The written value is not stored in ErrVld., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegMacXbarStatusMacNocErrclr_u;

/*REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG0 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lock : 1; //Lock, reset value: 0x0, access type: RO
		uint32 opc : 4; //Opc, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 errcode : 3; //ErrCode, reset value: 0x0, access type: RO
		uint32 reserved1 : 5;
		uint32 len1 : 9; //Len1, reset value: 0x0, access type: RO
		uint32 reserved2 : 6;
		uint32 format : 1; //NTTP transport protocol version, reset value: 0x1, access type: RO
	} bitFields;
} RegMacXbarStatusMacNocErrlog0_u;

/*REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG1 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 routeid : 19; //Stores NTTP packet header field RouteId (LSBs) of the logged error, reset value: 0x0, access type: RO
		uint32 reserved0 : 13;
	} bitFields;
} RegMacXbarStatusMacNocErrlog1_u;

/*REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG3 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 addrLsb : 32; //Stores NTTP packet header field Addr (LSBs) of the logged error, reset value: 0x0, access type: RO
	} bitFields;
} RegMacXbarStatusMacNocErrlog3_u;

/*REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG4 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 addrMsb : 1; //Stores NTTP packet header field Addr (MSBs) of the logged error, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegMacXbarStatusMacNocErrlog4_u;

/*REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG5 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 userLsb : 4; //Stores NTTP packet header field User (LSBs) of the logged error, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegMacXbarStatusMacNocErrlog5_u;

/*REG_MAC_XBAR_STATUS_MAC_NOC_STALLEN 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 stallen : 1; //Set to 1 to enable stall mode behavior, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacXbarStatusMacNocStallen_u;

//========================================
/* REG_MAC_XBAR_STATUS_MAC_NOC_FAULTEN 0x8 */
#define MAC_XBAR_STATUS_MAC_NOC_FAULTEN_FIELD_FAULTEN_MASK                                                        0x00000001
//========================================
/* REG_MAC_XBAR_STATUS_MAC_NOC_ERRVLD 0xC */
#define MAC_XBAR_STATUS_MAC_NOC_ERRVLD_FIELD_ERRVLD_MASK                                                          0x00000001
//========================================
/* REG_MAC_XBAR_STATUS_MAC_NOC_ERRCLR 0x10 */
#define MAC_XBAR_STATUS_MAC_NOC_ERRCLR_FIELD_ERRCLR_MASK                                                          0x00000001
//========================================
/* REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG0 0x14 */
#define MAC_XBAR_STATUS_MAC_NOC_ERRLOG0_FIELD_LOCK_MASK                                                           0x00000001
#define MAC_XBAR_STATUS_MAC_NOC_ERRLOG0_FIELD_OPC_MASK                                                            0x0000001E
#define MAC_XBAR_STATUS_MAC_NOC_ERRLOG0_FIELD_ERRCODE_MASK                                                        0x00000700
#define MAC_XBAR_STATUS_MAC_NOC_ERRLOG0_FIELD_LEN1_MASK                                                           0x01FF0000
#define MAC_XBAR_STATUS_MAC_NOC_ERRLOG0_FIELD_FORMAT_MASK                                                         0x80000000
//========================================
/* REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG1 0x18 */
#define MAC_XBAR_STATUS_MAC_NOC_ERRLOG1_FIELD_ROUTEID_MASK                                                        0x0007FFFF
//========================================
/* REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG3 0x20 */
#define MAC_XBAR_STATUS_MAC_NOC_ERRLOG3_FIELD_ADDR_LSB_MASK                                                       0xFFFFFFFF
//========================================
/* REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG4 0x24 */
#define MAC_XBAR_STATUS_MAC_NOC_ERRLOG4_FIELD_ADDR_MSB_MASK                                                       0x00000001
//========================================
/* REG_MAC_XBAR_STATUS_MAC_NOC_ERRLOG5 0x28 */
#define MAC_XBAR_STATUS_MAC_NOC_ERRLOG5_FIELD_USER_LSB_MASK                                                       0x0000000F
//========================================
/* REG_MAC_XBAR_STATUS_MAC_NOC_STALLEN 0x4C */
#define MAC_XBAR_STATUS_MAC_NOC_STALLEN_FIELD_STALLEN_MASK                                                        0x00000001


#endif // _MAC_XBAR_STATUS_REGS_H_
