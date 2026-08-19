
/***********************************************************************************
File:				TimGenVapCfgRegs.h
Module:				timGenVapCfg
SOC Revision:		latest
Generated at:       2022-05-04 10:58:49
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _TIM_GEN_VAP_CFG_REGS_H_
#define _TIM_GEN_VAP_CFG_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define TIM_GEN_VAP_CFG_BASE_ADDRESS                             MEMORY_MAP_UNIT_1080_BASE_ADDRESS
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG0     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x800)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG1     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x804)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG2     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x808)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG3     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x80C)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG4     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x810)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG5     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x814)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG6     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x818)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG7     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x81C)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG8     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x820)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG9     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x824)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG10    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x828)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG11    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x82C)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG12    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x830)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG13    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x834)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG14    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x838)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG15    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x83C)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG16    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x840)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG17    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x844)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG18    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x848)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG19    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x84C)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG20    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x850)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG21    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x854)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG22    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x858)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG23    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x85C)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG24    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x860)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG25    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x864)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG26    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x868)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG27    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x86C)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG28    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x870)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG29    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x874)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG30    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x878)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG31    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x87C)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG32    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x880)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG33    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x884)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG34    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x888)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG35    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x88C)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG36    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x890)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG37    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x894)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG38    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x898)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG39    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x89C)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG40    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8A0)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG41    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8A4)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG42    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8A8)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG43    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8AC)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG44    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8B0)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG45    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8B4)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG46    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8B8)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG47    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8BC)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG48    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8C0)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG49    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8C4)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG50    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8C8)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG51    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8CC)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG52    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8D0)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG53    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8D4)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG54    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8D8)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG55    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8DC)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG56    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8E0)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG57    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8E4)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG58    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8E8)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG59    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8EC)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG60    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8F0)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG61    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8F4)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG62    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8F8)
#define	REG_TIM_GEN_VAP_CFG_VAP_CFG63    (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x8FC)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_TIM_GEN_VAP_CFG_VAP_CFG0 0x800 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg0_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG1 0x804 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg1_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG2 0x808 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg2_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG3 0x80C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg3_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG4 0x810 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg4_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG5 0x814 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg5_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG6 0x818 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg6_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG7 0x81C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg7_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG8 0x820 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg8_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG9 0x824 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg9_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG10 0x828 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg10_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG11 0x82C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg11_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG12 0x830 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg12_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG13 0x834 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg13_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG14 0x838 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg14_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG15 0x83C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg15_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG16 0x840 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg16_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG17 0x844 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg17_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG18 0x848 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg18_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG19 0x84C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg19_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG20 0x850 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg20_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG21 0x854 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg21_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG22 0x858 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg22_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG23 0x85C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg23_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG24 0x860 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg24_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG25 0x864 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg25_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG26 0x868 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg26_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG27 0x86C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg27_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG28 0x870 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg28_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG29 0x874 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg29_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG30 0x878 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg30_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG31 0x87C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg31_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG32 0x880 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg32_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG33 0x884 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg33_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG34 0x888 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg34_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG35 0x88C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg35_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG36 0x890 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg36_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG37 0x894 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg37_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG38 0x898 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg38_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG39 0x89C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg39_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG40 0x8A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg40_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG41 0x8A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg41_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG42 0x8A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg42_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG43 0x8AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg43_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG44 0x8B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg44_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG45 0x8B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg45_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG46 0x8B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg46_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG47 0x8BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg47_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG48 0x8C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg48_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG49 0x8C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg49_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG50 0x8C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg50_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG51 0x8CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg51_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG52 0x8D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg52_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG53 0x8D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg53_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG54 0x8D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg54_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG55 0x8DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg55_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG56 0x8E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg56_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG57 0x8E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg57_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG58 0x8E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg58_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG59 0x8EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg59_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG60 0x8F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg60_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG61 0x8F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg61_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG62 0x8F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg62_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG63 0x8FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 siblingVap : 6; //Sibling VAP  , reset value: 0x0, access type: RW
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 mBssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg63_u;



#endif // _TIM_GEN_VAP_CFG_REGS_H_
