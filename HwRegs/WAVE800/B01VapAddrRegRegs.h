
/***********************************************************************************
File:				B01VapAddrRegRegs.h
Module:				b01VapAddrReg
SOC Revision:		latest
Generated at:       2024-06-26 12:54:23
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B01_VAP_ADDR_REG_REGS_H_
#define _B01_VAP_ADDR_REG_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B01_VAP_ADDR_REG_BASE_ADDRESS            MEMORY_MAP_UNIT_5_BASE_ADDRESS
#define	REG_B01_VAP_ADDR_REG_B0_VAP_IDX_EN              (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x0)
#define	REG_B01_VAP_ADDR_REG_B1_VAP_IDX_EN              (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x8)
#define	REG_B01_VAP_ADDR_REG_VAP0_ADDR1_REFERENCE_0     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x10)
#define	REG_B01_VAP_ADDR_REG_VAP0_ADDR1_REFERENCE_1     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x14)
#define	REG_B01_VAP_ADDR_REG_VAP1_ADDR1_REFERENCE_0     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x18)
#define	REG_B01_VAP_ADDR_REG_VAP1_ADDR1_REFERENCE_1     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x1C)
#define	REG_B01_VAP_ADDR_REG_VAP2_ADDR1_REFERENCE_0     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x20)
#define	REG_B01_VAP_ADDR_REG_VAP2_ADDR1_REFERENCE_1     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x24)
#define	REG_B01_VAP_ADDR_REG_VAP3_ADDR1_REFERENCE_0     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x28)
#define	REG_B01_VAP_ADDR_REG_VAP3_ADDR1_REFERENCE_1     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x2C)
#define	REG_B01_VAP_ADDR_REG_VAP4_ADDR1_REFERENCE_0     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x30)
#define	REG_B01_VAP_ADDR_REG_VAP4_ADDR1_REFERENCE_1     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x34)
#define	REG_B01_VAP_ADDR_REG_VAP5_ADDR1_REFERENCE_0     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x38)
#define	REG_B01_VAP_ADDR_REG_VAP5_ADDR1_REFERENCE_1     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x3C)
#define	REG_B01_VAP_ADDR_REG_VAP6_ADDR1_REFERENCE_0     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x40)
#define	REG_B01_VAP_ADDR_REG_VAP6_ADDR1_REFERENCE_1     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x44)
#define	REG_B01_VAP_ADDR_REG_VAP7_ADDR1_REFERENCE_0     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x48)
#define	REG_B01_VAP_ADDR_REG_VAP7_ADDR1_REFERENCE_1     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x4C)
#define	REG_B01_VAP_ADDR_REG_VAP8_ADDR1_REFERENCE_0     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x50)
#define	REG_B01_VAP_ADDR_REG_VAP8_ADDR1_REFERENCE_1     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x54)
#define	REG_B01_VAP_ADDR_REG_VAP9_ADDR1_REFERENCE_0     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x58)
#define	REG_B01_VAP_ADDR_REG_VAP9_ADDR1_REFERENCE_1     (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x5C)
#define	REG_B01_VAP_ADDR_REG_VAP10_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x60)
#define	REG_B01_VAP_ADDR_REG_VAP10_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x64)
#define	REG_B01_VAP_ADDR_REG_VAP11_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x68)
#define	REG_B01_VAP_ADDR_REG_VAP11_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x6C)
#define	REG_B01_VAP_ADDR_REG_VAP12_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x70)
#define	REG_B01_VAP_ADDR_REG_VAP12_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x74)
#define	REG_B01_VAP_ADDR_REG_VAP13_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x78)
#define	REG_B01_VAP_ADDR_REG_VAP13_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x7C)
#define	REG_B01_VAP_ADDR_REG_VAP14_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x80)
#define	REG_B01_VAP_ADDR_REG_VAP14_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x84)
#define	REG_B01_VAP_ADDR_REG_VAP15_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x88)
#define	REG_B01_VAP_ADDR_REG_VAP15_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x8C)
#define	REG_B01_VAP_ADDR_REG_VAP16_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x90)
#define	REG_B01_VAP_ADDR_REG_VAP16_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x94)
#define	REG_B01_VAP_ADDR_REG_VAP17_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x98)
#define	REG_B01_VAP_ADDR_REG_VAP17_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x9C)
#define	REG_B01_VAP_ADDR_REG_VAP18_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xA0)
#define	REG_B01_VAP_ADDR_REG_VAP18_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xA4)
#define	REG_B01_VAP_ADDR_REG_VAP19_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xA8)
#define	REG_B01_VAP_ADDR_REG_VAP19_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xAC)
#define	REG_B01_VAP_ADDR_REG_VAP20_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xB0)
#define	REG_B01_VAP_ADDR_REG_VAP20_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xB4)
#define	REG_B01_VAP_ADDR_REG_VAP21_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xB8)
#define	REG_B01_VAP_ADDR_REG_VAP21_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xBC)
#define	REG_B01_VAP_ADDR_REG_VAP22_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xC0)
#define	REG_B01_VAP_ADDR_REG_VAP22_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xC4)
#define	REG_B01_VAP_ADDR_REG_VAP23_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xC8)
#define	REG_B01_VAP_ADDR_REG_VAP23_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xCC)
#define	REG_B01_VAP_ADDR_REG_VAP24_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xD0)
#define	REG_B01_VAP_ADDR_REG_VAP24_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xD4)
#define	REG_B01_VAP_ADDR_REG_VAP25_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xD8)
#define	REG_B01_VAP_ADDR_REG_VAP25_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xDC)
#define	REG_B01_VAP_ADDR_REG_VAP26_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xE0)
#define	REG_B01_VAP_ADDR_REG_VAP26_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xE4)
#define	REG_B01_VAP_ADDR_REG_VAP27_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xE8)
#define	REG_B01_VAP_ADDR_REG_VAP27_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xEC)
#define	REG_B01_VAP_ADDR_REG_VAP28_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xF0)
#define	REG_B01_VAP_ADDR_REG_VAP28_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xF4)
#define	REG_B01_VAP_ADDR_REG_VAP29_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xF8)
#define	REG_B01_VAP_ADDR_REG_VAP29_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0xFC)
#define	REG_B01_VAP_ADDR_REG_VAP30_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x100)
#define	REG_B01_VAP_ADDR_REG_VAP30_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x104)
#define	REG_B01_VAP_ADDR_REG_VAP31_ADDR1_REFERENCE_0    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x108)
#define	REG_B01_VAP_ADDR_REG_VAP31_ADDR1_REFERENCE_1    (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x10C)
#define	REG_B01_VAP_ADDR_REG_B0_FORCE_VAP_IDX0          (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x1E0)
#define	REG_B01_VAP_ADDR_REG_B1_FORCE_VAP_IDX0          (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x1E4)
#define	REG_B01_VAP_ADDR_REG_ADDR1_TO_VAP_IDX_SM        (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x1F0)
#define	REG_B01_VAP_ADDR_REG_SPARE_REGISTERS            (B01_VAP_ADDR_REG_BASE_ADDRESS + 0x1FC)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B01_VAP_ADDR_REG_B0_VAP_IDX_EN 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b0VapIdxEn : 32; //VAP index enable for Band 0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegB0VapIdxEn_u;

/*REG_B01_VAP_ADDR_REG_B1_VAP_IDX_EN 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b1VapIdxEn : 32; //VAP index enable for Band 1, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegB1VapIdxEn_u;

/*REG_B01_VAP_ADDR_REG_VAP0_ADDR1_REFERENCE_0 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap0Addr1Reference0 : 32; //vap0 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap0Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP0_ADDR1_REFERENCE_1 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap0Addr1Reference1 : 16; //vap0 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap0Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP1_ADDR1_REFERENCE_0 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap1Addr1Reference0 : 32; //vap1 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap1Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP1_ADDR1_REFERENCE_1 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap1Addr1Reference1 : 16; //vap1 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap1Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP2_ADDR1_REFERENCE_0 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap2Addr1Reference0 : 32; //vap2 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap2Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP2_ADDR1_REFERENCE_1 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap2Addr1Reference1 : 16; //vap2 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap2Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP3_ADDR1_REFERENCE_0 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap3Addr1Reference0 : 32; //vap3 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap3Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP3_ADDR1_REFERENCE_1 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap3Addr1Reference1 : 16; //vap3 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap3Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP4_ADDR1_REFERENCE_0 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap4Addr1Reference0 : 32; //vap4 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap4Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP4_ADDR1_REFERENCE_1 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap4Addr1Reference1 : 16; //vap4 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap4Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP5_ADDR1_REFERENCE_0 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap5Addr1Reference0 : 32; //vap5 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap5Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP5_ADDR1_REFERENCE_1 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap5Addr1Reference1 : 16; //vap5 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap5Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP6_ADDR1_REFERENCE_0 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap6Addr1Reference0 : 32; //vap6 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap6Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP6_ADDR1_REFERENCE_1 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap6Addr1Reference1 : 16; //vap6 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap6Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP7_ADDR1_REFERENCE_0 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap7Addr1Reference0 : 32; //vap7 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap7Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP7_ADDR1_REFERENCE_1 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap7Addr1Reference1 : 16; //vap7 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap7Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP8_ADDR1_REFERENCE_0 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap8Addr1Reference0 : 32; //vap8 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap8Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP8_ADDR1_REFERENCE_1 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap8Addr1Reference1 : 16; //vap8 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap8Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP9_ADDR1_REFERENCE_0 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap9Addr1Reference0 : 32; //vap9 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap9Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP9_ADDR1_REFERENCE_1 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap9Addr1Reference1 : 16; //vap9 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap9Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP10_ADDR1_REFERENCE_0 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap10Addr1Reference0 : 32; //vap10 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap10Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP10_ADDR1_REFERENCE_1 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap10Addr1Reference1 : 16; //vap10 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap10Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP11_ADDR1_REFERENCE_0 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap11Addr1Reference0 : 32; //vap11 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap11Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP11_ADDR1_REFERENCE_1 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap11Addr1Reference1 : 16; //vap11 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap11Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP12_ADDR1_REFERENCE_0 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap12Addr1Reference0 : 32; //vap12 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap12Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP12_ADDR1_REFERENCE_1 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap12Addr1Reference1 : 16; //vap12 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap12Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP13_ADDR1_REFERENCE_0 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap13Addr1Reference0 : 32; //vap13 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap13Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP13_ADDR1_REFERENCE_1 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap13Addr1Reference1 : 16; //vap13 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap13Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP14_ADDR1_REFERENCE_0 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap14Addr1Reference0 : 32; //vap14 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap14Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP14_ADDR1_REFERENCE_1 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap14Addr1Reference1 : 16; //vap14 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap14Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP15_ADDR1_REFERENCE_0 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap15Addr1Reference0 : 32; //vap15 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap15Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP15_ADDR1_REFERENCE_1 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap15Addr1Reference1 : 16; //vap15 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap15Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP16_ADDR1_REFERENCE_0 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap16Addr1Reference0 : 32; //vap16 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap16Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP16_ADDR1_REFERENCE_1 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap16Addr1Reference1 : 16; //vap16 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap16Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP17_ADDR1_REFERENCE_0 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap17Addr1Reference0 : 32; //vap17 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap17Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP17_ADDR1_REFERENCE_1 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap17Addr1Reference1 : 16; //vap17 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap17Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP18_ADDR1_REFERENCE_0 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap18Addr1Reference0 : 32; //vap18 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap18Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP18_ADDR1_REFERENCE_1 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap18Addr1Reference1 : 16; //vap18 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap18Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP19_ADDR1_REFERENCE_0 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap19Addr1Reference0 : 32; //vap19 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap19Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP19_ADDR1_REFERENCE_1 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap19Addr1Reference1 : 16; //vap19 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap19Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP20_ADDR1_REFERENCE_0 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap20Addr1Reference0 : 32; //vap20 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap20Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP20_ADDR1_REFERENCE_1 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap20Addr1Reference1 : 16; //vap20 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap20Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP21_ADDR1_REFERENCE_0 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap21Addr1Reference0 : 32; //vap21 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap21Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP21_ADDR1_REFERENCE_1 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap21Addr1Reference1 : 16; //vap21 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap21Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP22_ADDR1_REFERENCE_0 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap22Addr1Reference0 : 32; //vap22 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap22Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP22_ADDR1_REFERENCE_1 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap22Addr1Reference1 : 16; //vap22 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap22Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP23_ADDR1_REFERENCE_0 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap23Addr1Reference0 : 32; //vap23 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap23Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP23_ADDR1_REFERENCE_1 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap23Addr1Reference1 : 16; //vap23 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap23Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP24_ADDR1_REFERENCE_0 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap24Addr1Reference0 : 32; //vap24 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap24Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP24_ADDR1_REFERENCE_1 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap24Addr1Reference1 : 16; //vap24 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap24Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP25_ADDR1_REFERENCE_0 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap25Addr1Reference0 : 32; //vap25 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap25Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP25_ADDR1_REFERENCE_1 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap25Addr1Reference1 : 16; //vap25 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap25Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP26_ADDR1_REFERENCE_0 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap26Addr1Reference0 : 32; //vap26 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap26Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP26_ADDR1_REFERENCE_1 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap26Addr1Reference1 : 16; //vap26 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap26Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP27_ADDR1_REFERENCE_0 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap27Addr1Reference0 : 32; //vap27 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap27Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP27_ADDR1_REFERENCE_1 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap27Addr1Reference1 : 16; //vap27 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap27Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP28_ADDR1_REFERENCE_0 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap28Addr1Reference0 : 32; //vap28 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap28Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP28_ADDR1_REFERENCE_1 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap28Addr1Reference1 : 16; //vap28 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap28Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP29_ADDR1_REFERENCE_0 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap29Addr1Reference0 : 32; //vap29 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap29Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP29_ADDR1_REFERENCE_1 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap29Addr1Reference1 : 16; //vap29 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap29Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP30_ADDR1_REFERENCE_0 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap30Addr1Reference0 : 32; //vap30 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap30Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP30_ADDR1_REFERENCE_1 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap30Addr1Reference1 : 16; //vap30 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap30Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_VAP31_ADDR1_REFERENCE_0 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap31Addr1Reference0 : 32; //vap31 address1 reference, bits 32:0, reset value: 0x0, access type: RW
	} bitFields;
} RegB01VapAddrRegVap31Addr1Reference0_u;

/*REG_B01_VAP_ADDR_REG_VAP31_ADDR1_REFERENCE_1 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap31Addr1Reference1 : 16; //vap31 address1 reference, bits 47:32, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegVap31Addr1Reference1_u;

/*REG_B01_VAP_ADDR_REG_B0_FORCE_VAP_IDX0 0x1E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b0ForceVapIdx0 : 1; //Force the VAP index result to be 0 for Band0, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB01VapAddrRegB0ForceVapIdx0_u;

/*REG_B01_VAP_ADDR_REG_B1_FORCE_VAP_IDX0 0x1E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 b1ForceVapIdx0 : 1; //Force the VAP index result to be 0 for Band1, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB01VapAddrRegB1ForceVapIdx0_u;

/*REG_B01_VAP_ADDR_REG_ADDR1_TO_VAP_IDX_SM 0x1F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 addr1ToVapIdxSm : 2; //ADDR1 to VAP index State machine, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB01VapAddrRegAddr1ToVapIdxSm_u;

/*REG_B01_VAP_ADDR_REG_SPARE_REGISTERS 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareRegisters : 16; //Spare registers, reset value: 0x8888, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB01VapAddrRegSpareRegisters_u;

//========================================
/* REG_VAP_ADDR_REG_B0_VAP_IDX_EN 0x0 */
#define B01_VAP_ADDR_REG_B0_VAP_IDX_EN_FIELD_B0_VAP_IDX_EN_MASK                                                   0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_B1_VAP_IDX_EN 0x8 */
#define B01_VAP_ADDR_REG_B1_VAP_IDX_EN_FIELD_B1_VAP_IDX_EN_MASK                                                   0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP0_ADDR1_REFERENCE_0 0x10 */
#define B01_VAP_ADDR_REG_VAP0_ADDR1_REFERENCE_0_FIELD_VAP0_ADDR1_REFERENCE_0_MASK                                 0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP0_ADDR1_REFERENCE_1 0x14 */
#define B01_VAP_ADDR_REG_VAP0_ADDR1_REFERENCE_1_FIELD_VAP0_ADDR1_REFERENCE_1_MASK                                 0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP1_ADDR1_REFERENCE_0 0x18 */
#define B01_VAP_ADDR_REG_VAP1_ADDR1_REFERENCE_0_FIELD_VAP1_ADDR1_REFERENCE_0_MASK                                 0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP1_ADDR1_REFERENCE_1 0x1C */
#define B01_VAP_ADDR_REG_VAP1_ADDR1_REFERENCE_1_FIELD_VAP1_ADDR1_REFERENCE_1_MASK                                 0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP2_ADDR1_REFERENCE_0 0x20 */
#define B01_VAP_ADDR_REG_VAP2_ADDR1_REFERENCE_0_FIELD_VAP2_ADDR1_REFERENCE_0_MASK                                 0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP2_ADDR1_REFERENCE_1 0x24 */
#define B01_VAP_ADDR_REG_VAP2_ADDR1_REFERENCE_1_FIELD_VAP2_ADDR1_REFERENCE_1_MASK                                 0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP3_ADDR1_REFERENCE_0 0x28 */
#define B01_VAP_ADDR_REG_VAP3_ADDR1_REFERENCE_0_FIELD_VAP3_ADDR1_REFERENCE_0_MASK                                 0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP3_ADDR1_REFERENCE_1 0x2C */
#define B01_VAP_ADDR_REG_VAP3_ADDR1_REFERENCE_1_FIELD_VAP3_ADDR1_REFERENCE_1_MASK                                 0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP4_ADDR1_REFERENCE_0 0x30 */
#define B01_VAP_ADDR_REG_VAP4_ADDR1_REFERENCE_0_FIELD_VAP4_ADDR1_REFERENCE_0_MASK                                 0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP4_ADDR1_REFERENCE_1 0x34 */
#define B01_VAP_ADDR_REG_VAP4_ADDR1_REFERENCE_1_FIELD_VAP4_ADDR1_REFERENCE_1_MASK                                 0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP5_ADDR1_REFERENCE_0 0x38 */
#define B01_VAP_ADDR_REG_VAP5_ADDR1_REFERENCE_0_FIELD_VAP5_ADDR1_REFERENCE_0_MASK                                 0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP5_ADDR1_REFERENCE_1 0x3C */
#define B01_VAP_ADDR_REG_VAP5_ADDR1_REFERENCE_1_FIELD_VAP5_ADDR1_REFERENCE_1_MASK                                 0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP6_ADDR1_REFERENCE_0 0x40 */
#define B01_VAP_ADDR_REG_VAP6_ADDR1_REFERENCE_0_FIELD_VAP6_ADDR1_REFERENCE_0_MASK                                 0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP6_ADDR1_REFERENCE_1 0x44 */
#define B01_VAP_ADDR_REG_VAP6_ADDR1_REFERENCE_1_FIELD_VAP6_ADDR1_REFERENCE_1_MASK                                 0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP7_ADDR1_REFERENCE_0 0x48 */
#define B01_VAP_ADDR_REG_VAP7_ADDR1_REFERENCE_0_FIELD_VAP7_ADDR1_REFERENCE_0_MASK                                 0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP7_ADDR1_REFERENCE_1 0x4C */
#define B01_VAP_ADDR_REG_VAP7_ADDR1_REFERENCE_1_FIELD_VAP7_ADDR1_REFERENCE_1_MASK                                 0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP8_ADDR1_REFERENCE_0 0x50 */
#define B01_VAP_ADDR_REG_VAP8_ADDR1_REFERENCE_0_FIELD_VAP8_ADDR1_REFERENCE_0_MASK                                 0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP8_ADDR1_REFERENCE_1 0x54 */
#define B01_VAP_ADDR_REG_VAP8_ADDR1_REFERENCE_1_FIELD_VAP8_ADDR1_REFERENCE_1_MASK                                 0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP9_ADDR1_REFERENCE_0 0x58 */
#define B01_VAP_ADDR_REG_VAP9_ADDR1_REFERENCE_0_FIELD_VAP9_ADDR1_REFERENCE_0_MASK                                 0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP9_ADDR1_REFERENCE_1 0x5C */
#define B01_VAP_ADDR_REG_VAP9_ADDR1_REFERENCE_1_FIELD_VAP9_ADDR1_REFERENCE_1_MASK                                 0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP10_ADDR1_REFERENCE_0 0x60 */
#define B01_VAP_ADDR_REG_VAP10_ADDR1_REFERENCE_0_FIELD_VAP10_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP10_ADDR1_REFERENCE_1 0x64 */
#define B01_VAP_ADDR_REG_VAP10_ADDR1_REFERENCE_1_FIELD_VAP10_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP11_ADDR1_REFERENCE_0 0x68 */
#define B01_VAP_ADDR_REG_VAP11_ADDR1_REFERENCE_0_FIELD_VAP11_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP11_ADDR1_REFERENCE_1 0x6C */
#define B01_VAP_ADDR_REG_VAP11_ADDR1_REFERENCE_1_FIELD_VAP11_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP12_ADDR1_REFERENCE_0 0x70 */
#define B01_VAP_ADDR_REG_VAP12_ADDR1_REFERENCE_0_FIELD_VAP12_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP12_ADDR1_REFERENCE_1 0x74 */
#define B01_VAP_ADDR_REG_VAP12_ADDR1_REFERENCE_1_FIELD_VAP12_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP13_ADDR1_REFERENCE_0 0x78 */
#define B01_VAP_ADDR_REG_VAP13_ADDR1_REFERENCE_0_FIELD_VAP13_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP13_ADDR1_REFERENCE_1 0x7C */
#define B01_VAP_ADDR_REG_VAP13_ADDR1_REFERENCE_1_FIELD_VAP13_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP14_ADDR1_REFERENCE_0 0x80 */
#define B01_VAP_ADDR_REG_VAP14_ADDR1_REFERENCE_0_FIELD_VAP14_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP14_ADDR1_REFERENCE_1 0x84 */
#define B01_VAP_ADDR_REG_VAP14_ADDR1_REFERENCE_1_FIELD_VAP14_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP15_ADDR1_REFERENCE_0 0x88 */
#define B01_VAP_ADDR_REG_VAP15_ADDR1_REFERENCE_0_FIELD_VAP15_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP15_ADDR1_REFERENCE_1 0x8C */
#define B01_VAP_ADDR_REG_VAP15_ADDR1_REFERENCE_1_FIELD_VAP15_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP16_ADDR1_REFERENCE_0 0x90 */
#define B01_VAP_ADDR_REG_VAP16_ADDR1_REFERENCE_0_FIELD_VAP16_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP16_ADDR1_REFERENCE_1 0x94 */
#define B01_VAP_ADDR_REG_VAP16_ADDR1_REFERENCE_1_FIELD_VAP16_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP17_ADDR1_REFERENCE_0 0x98 */
#define B01_VAP_ADDR_REG_VAP17_ADDR1_REFERENCE_0_FIELD_VAP17_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP17_ADDR1_REFERENCE_1 0x9C */
#define B01_VAP_ADDR_REG_VAP17_ADDR1_REFERENCE_1_FIELD_VAP17_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP18_ADDR1_REFERENCE_0 0xA0 */
#define B01_VAP_ADDR_REG_VAP18_ADDR1_REFERENCE_0_FIELD_VAP18_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP18_ADDR1_REFERENCE_1 0xA4 */
#define B01_VAP_ADDR_REG_VAP18_ADDR1_REFERENCE_1_FIELD_VAP18_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP19_ADDR1_REFERENCE_0 0xA8 */
#define B01_VAP_ADDR_REG_VAP19_ADDR1_REFERENCE_0_FIELD_VAP19_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP19_ADDR1_REFERENCE_1 0xAC */
#define B01_VAP_ADDR_REG_VAP19_ADDR1_REFERENCE_1_FIELD_VAP19_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP20_ADDR1_REFERENCE_0 0xB0 */
#define B01_VAP_ADDR_REG_VAP20_ADDR1_REFERENCE_0_FIELD_VAP20_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP20_ADDR1_REFERENCE_1 0xB4 */
#define B01_VAP_ADDR_REG_VAP20_ADDR1_REFERENCE_1_FIELD_VAP20_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP21_ADDR1_REFERENCE_0 0xB8 */
#define B01_VAP_ADDR_REG_VAP21_ADDR1_REFERENCE_0_FIELD_VAP21_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP21_ADDR1_REFERENCE_1 0xBC */
#define B01_VAP_ADDR_REG_VAP21_ADDR1_REFERENCE_1_FIELD_VAP21_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP22_ADDR1_REFERENCE_0 0xC0 */
#define B01_VAP_ADDR_REG_VAP22_ADDR1_REFERENCE_0_FIELD_VAP22_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP22_ADDR1_REFERENCE_1 0xC4 */
#define B01_VAP_ADDR_REG_VAP22_ADDR1_REFERENCE_1_FIELD_VAP22_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP23_ADDR1_REFERENCE_0 0xC8 */
#define B01_VAP_ADDR_REG_VAP23_ADDR1_REFERENCE_0_FIELD_VAP23_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP23_ADDR1_REFERENCE_1 0xCC */
#define B01_VAP_ADDR_REG_VAP23_ADDR1_REFERENCE_1_FIELD_VAP23_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP24_ADDR1_REFERENCE_0 0xD0 */
#define B01_VAP_ADDR_REG_VAP24_ADDR1_REFERENCE_0_FIELD_VAP24_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP24_ADDR1_REFERENCE_1 0xD4 */
#define B01_VAP_ADDR_REG_VAP24_ADDR1_REFERENCE_1_FIELD_VAP24_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP25_ADDR1_REFERENCE_0 0xD8 */
#define B01_VAP_ADDR_REG_VAP25_ADDR1_REFERENCE_0_FIELD_VAP25_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP25_ADDR1_REFERENCE_1 0xDC */
#define B01_VAP_ADDR_REG_VAP25_ADDR1_REFERENCE_1_FIELD_VAP25_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP26_ADDR1_REFERENCE_0 0xE0 */
#define B01_VAP_ADDR_REG_VAP26_ADDR1_REFERENCE_0_FIELD_VAP26_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP26_ADDR1_REFERENCE_1 0xE4 */
#define B01_VAP_ADDR_REG_VAP26_ADDR1_REFERENCE_1_FIELD_VAP26_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP27_ADDR1_REFERENCE_0 0xE8 */
#define B01_VAP_ADDR_REG_VAP27_ADDR1_REFERENCE_0_FIELD_VAP27_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP27_ADDR1_REFERENCE_1 0xEC */
#define B01_VAP_ADDR_REG_VAP27_ADDR1_REFERENCE_1_FIELD_VAP27_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP28_ADDR1_REFERENCE_0 0xF0 */
#define B01_VAP_ADDR_REG_VAP28_ADDR1_REFERENCE_0_FIELD_VAP28_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP28_ADDR1_REFERENCE_1 0xF4 */
#define B01_VAP_ADDR_REG_VAP28_ADDR1_REFERENCE_1_FIELD_VAP28_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP29_ADDR1_REFERENCE_0 0xF8 */
#define B01_VAP_ADDR_REG_VAP29_ADDR1_REFERENCE_0_FIELD_VAP29_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP29_ADDR1_REFERENCE_1 0xFC */
#define B01_VAP_ADDR_REG_VAP29_ADDR1_REFERENCE_1_FIELD_VAP29_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP30_ADDR1_REFERENCE_0 0x100 */
#define B01_VAP_ADDR_REG_VAP30_ADDR1_REFERENCE_0_FIELD_VAP30_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP30_ADDR1_REFERENCE_1 0x104 */
#define B01_VAP_ADDR_REG_VAP30_ADDR1_REFERENCE_1_FIELD_VAP30_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_VAP31_ADDR1_REFERENCE_0 0x108 */
#define B01_VAP_ADDR_REG_VAP31_ADDR1_REFERENCE_0_FIELD_VAP31_ADDR1_REFERENCE_0_MASK                               0xFFFFFFFF
//========================================
/* REG_VAP_ADDR_REG_VAP31_ADDR1_REFERENCE_1 0x10C */
#define B01_VAP_ADDR_REG_VAP31_ADDR1_REFERENCE_1_FIELD_VAP31_ADDR1_REFERENCE_1_MASK                               0x0000FFFF
//========================================
/* REG_VAP_ADDR_REG_B0_FORCE_VAP_IDX0 0x1E0 */
#define B01_VAP_ADDR_REG_B0_FORCE_VAP_IDX0_FIELD_B0_FORCE_VAP_IDX0_MASK                                           0x00000001
//========================================
/* REG_VAP_ADDR_REG_B1_FORCE_VAP_IDX0 0x1E4 */
#define B01_VAP_ADDR_REG_B1_FORCE_VAP_IDX0_FIELD_B1_FORCE_VAP_IDX0_MASK                                           0x00000001
//========================================
/* REG_VAP_ADDR_REG_ADDR1_TO_VAP_IDX_SM 0x1F0 */
#define B01_VAP_ADDR_REG_ADDR1_TO_VAP_IDX_SM_FIELD_ADDR1_TO_VAP_IDX_SM_MASK                                       0x00000003
//========================================
/* REG_VAP_ADDR_REG_SPARE_REGISTERS 0x1FC */
#define B01_VAP_ADDR_REG_SPARE_REGISTERS_FIELD_SPARE_REGISTERS_MASK                                               0x0000FFFF


#endif // _VAP_ADDR_REG_REGS_H_
