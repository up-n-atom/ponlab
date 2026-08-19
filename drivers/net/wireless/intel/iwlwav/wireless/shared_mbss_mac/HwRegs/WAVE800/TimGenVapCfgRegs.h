
/***********************************************************************************
File:				TimGenVapCfgRegs.h
Module:				timGenVapCfg
SOC Revision:		latest
Generated at:       2024-06-26 12:55:09
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _TIM_GEN_VAP_CFG_REGS_H_
#define _TIM_GEN_VAP_CFG_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define TIM_GEN_VAP_CFG_BASE_ADDRESS  MEMORY_MAP_UNIT_1080_BASE_ADDRESS
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
#define	REG_TIM_GEN_VAP_CFG_AP_MLD0      (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x900)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD1      (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x904)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD2      (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x908)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD3      (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x90C)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD4      (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x910)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD5      (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x914)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD6      (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x918)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD7      (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x91C)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD8      (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x920)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD9      (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x924)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD10     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x928)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD11     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x92C)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD12     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x930)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD13     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x934)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD14     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x938)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD15     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x93C)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD16     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x940)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD17     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x944)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD18     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x948)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD19     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x94C)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD20     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x950)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD21     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x954)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD22     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x958)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD23     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x95C)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD24     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x960)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD25     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x964)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD26     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x968)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD27     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x96C)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD28     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x970)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD29     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x974)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD30     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x978)
#define	REG_TIM_GEN_VAP_CFG_AP_MLD31     (TIM_GEN_VAP_CFG_BASE_ADDRESS + 0x97C)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_TIM_GEN_VAP_CFG_VAP_CFG0 0x800 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg0_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG1 0x804 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg1_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG2 0x808 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg2_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG3 0x80C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg3_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG4 0x810 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg4_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG5 0x814 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg5_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG6 0x818 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg6_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG7 0x81C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg7_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG8 0x820 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg8_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG9 0x824 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg9_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG10 0x828 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg10_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG11 0x82C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg11_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG12 0x830 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg12_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG13 0x834 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg13_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG14 0x838 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg14_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG15 0x83C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg15_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG16 0x840 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg16_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG17 0x844 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg17_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG18 0x848 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg18_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG19 0x84C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg19_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG20 0x850 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg20_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG21 0x854 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg21_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG22 0x858 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg22_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG23 0x85C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg23_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG24 0x860 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg24_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG25 0x864 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg25_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG26 0x868 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg26_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG27 0x86C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg27_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG28 0x870 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg28_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG29 0x874 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg29_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG30 0x878 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg30_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG31 0x87C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg31_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG32 0x880 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg32_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG33 0x884 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg33_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG34 0x888 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg34_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG35 0x88C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg35_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG36 0x890 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg36_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG37 0x894 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg37_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG38 0x898 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg38_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG39 0x89C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg39_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG40 0x8A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg40_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG41 0x8A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg41_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG42 0x8A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg42_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG43 0x8AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg43_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG44 0x8B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg44_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG45 0x8B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg45_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG46 0x8B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg46_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG47 0x8BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg47_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG48 0x8C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg48_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG49 0x8C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg49_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG50 0x8C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg50_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG51 0x8CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg51_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG52 0x8D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg52_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG53 0x8D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg53_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG54 0x8D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg54_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG55 0x8DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg55_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG56 0x8E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg56_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG57 0x8E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg57_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG58 0x8E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg58_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG59 0x8EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg59_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG60 0x8F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg60_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG61 0x8F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg61_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG62 0x8F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg62_u;

/*REG_TIM_GEN_VAP_CFG_VAP_CFG63 0x8FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldIdx : 5; //The ID of the AP MLD (relevant if is_mld=1)  , reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 isMld : 1; //is MLD , reset value: 0x0, access type: RW
		uint32 isMbssid : 1; //is M-BSSID, reset value: 0x0, access type: RW
		uint32 compressed : 1; //Compressed, reset value: 0x0, access type: RW
		uint32 validEntry : 1; //This entry is valid, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegTimGenVapCfgVapCfg63_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD0 0x900 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld0_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD1 0x904 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld1_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD2 0x908 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld2_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD3 0x90C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld3_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD4 0x910 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld4_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD5 0x914 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld5_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD6 0x918 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld6_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD7 0x91C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld7_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD8 0x920 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld8_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD9 0x924 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld9_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD10 0x928 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld10_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD11 0x92C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld11_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD12 0x930 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld12_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD13 0x934 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld13_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD14 0x938 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld14_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD15 0x93C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld15_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD16 0x940 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld16_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD17 0x944 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld17_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD18 0x948 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld18_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD19 0x94C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld19_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD20 0x950 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld20_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD21 0x954 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld21_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD22 0x958 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld22_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD23 0x95C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld23_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD24 0x960 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld24_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD25 0x964 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld25_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD26 0x968 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld26_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD27 0x96C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld27_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD28 0x970 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld28_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD29 0x974 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld29_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD30 0x978 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld30_u;

/*REG_TIM_GEN_VAP_CFG_AP_MLD31 0x97C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 isMbssid0 : 1; //---1--- indicates that vap_idx0 is member of M-BSSID set in band 0, reset value: 0x0, access type: RW
		uint32 isMbssid1 : 1; //---1--- indicates that vap_idx1 is member of M-BSSID set in band 1, reset value: 0x0, access type: RW
		uint32 isMbssid2 : 1; //---1--- indicates that vap_idx2 is member of M-BSSID set in band 2, reset value: 0x0, access type: RW
		uint32 vapIdx0 : 6; //Band 0 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx1 : 6; //Band 1 vap_idx, reset value: 0x0, access type: RW
		uint32 vapIdx2 : 6; //Band 2 vap_idx, reset value: 0x0, access type: RW
		uint32 linkBitmap : 3; //Link_bitmap[N]=1, N=0..2, indicates that the AP MLD has affiliated VAP on band N, with index vap_idxN, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTimGenVapCfgApMld31_u;

//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG0 0x800 */
#define TIM_GEN_VAP_CFG_VAP_CFG0_FIELD_MLD_IDX_MASK                                                               0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG0_FIELD_IS_MLD_MASK                                                                0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG0_FIELD_IS_MBSSID_MASK                                                             0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG0_FIELD_COMPRESSED_MASK                                                            0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG0_FIELD_VALID_ENTRY_MASK                                                           0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG1 0x804 */
#define TIM_GEN_VAP_CFG_VAP_CFG1_FIELD_MLD_IDX_MASK                                                               0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG1_FIELD_IS_MLD_MASK                                                                0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG1_FIELD_IS_MBSSID_MASK                                                             0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG1_FIELD_COMPRESSED_MASK                                                            0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG1_FIELD_VALID_ENTRY_MASK                                                           0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG2 0x808 */
#define TIM_GEN_VAP_CFG_VAP_CFG2_FIELD_MLD_IDX_MASK                                                               0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG2_FIELD_IS_MLD_MASK                                                                0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG2_FIELD_IS_MBSSID_MASK                                                             0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG2_FIELD_COMPRESSED_MASK                                                            0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG2_FIELD_VALID_ENTRY_MASK                                                           0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG3 0x80C */
#define TIM_GEN_VAP_CFG_VAP_CFG3_FIELD_MLD_IDX_MASK                                                               0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG3_FIELD_IS_MLD_MASK                                                                0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG3_FIELD_IS_MBSSID_MASK                                                             0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG3_FIELD_COMPRESSED_MASK                                                            0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG3_FIELD_VALID_ENTRY_MASK                                                           0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG4 0x810 */
#define TIM_GEN_VAP_CFG_VAP_CFG4_FIELD_MLD_IDX_MASK                                                               0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG4_FIELD_IS_MLD_MASK                                                                0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG4_FIELD_IS_MBSSID_MASK                                                             0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG4_FIELD_COMPRESSED_MASK                                                            0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG4_FIELD_VALID_ENTRY_MASK                                                           0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG5 0x814 */
#define TIM_GEN_VAP_CFG_VAP_CFG5_FIELD_MLD_IDX_MASK                                                               0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG5_FIELD_IS_MLD_MASK                                                                0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG5_FIELD_IS_MBSSID_MASK                                                             0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG5_FIELD_COMPRESSED_MASK                                                            0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG5_FIELD_VALID_ENTRY_MASK                                                           0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG6 0x818 */
#define TIM_GEN_VAP_CFG_VAP_CFG6_FIELD_MLD_IDX_MASK                                                               0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG6_FIELD_IS_MLD_MASK                                                                0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG6_FIELD_IS_MBSSID_MASK                                                             0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG6_FIELD_COMPRESSED_MASK                                                            0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG6_FIELD_VALID_ENTRY_MASK                                                           0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG7 0x81C */
#define TIM_GEN_VAP_CFG_VAP_CFG7_FIELD_MLD_IDX_MASK                                                               0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG7_FIELD_IS_MLD_MASK                                                                0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG7_FIELD_IS_MBSSID_MASK                                                             0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG7_FIELD_COMPRESSED_MASK                                                            0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG7_FIELD_VALID_ENTRY_MASK                                                           0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG8 0x820 */
#define TIM_GEN_VAP_CFG_VAP_CFG8_FIELD_MLD_IDX_MASK                                                               0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG8_FIELD_IS_MLD_MASK                                                                0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG8_FIELD_IS_MBSSID_MASK                                                             0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG8_FIELD_COMPRESSED_MASK                                                            0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG8_FIELD_VALID_ENTRY_MASK                                                           0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG9 0x824 */
#define TIM_GEN_VAP_CFG_VAP_CFG9_FIELD_MLD_IDX_MASK                                                               0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG9_FIELD_IS_MLD_MASK                                                                0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG9_FIELD_IS_MBSSID_MASK                                                             0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG9_FIELD_COMPRESSED_MASK                                                            0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG9_FIELD_VALID_ENTRY_MASK                                                           0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG10 0x828 */
#define TIM_GEN_VAP_CFG_VAP_CFG10_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG10_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG10_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG10_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG10_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG11 0x82C */
#define TIM_GEN_VAP_CFG_VAP_CFG11_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG11_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG11_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG11_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG11_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG12 0x830 */
#define TIM_GEN_VAP_CFG_VAP_CFG12_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG12_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG12_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG12_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG12_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG13 0x834 */
#define TIM_GEN_VAP_CFG_VAP_CFG13_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG13_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG13_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG13_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG13_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG14 0x838 */
#define TIM_GEN_VAP_CFG_VAP_CFG14_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG14_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG14_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG14_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG14_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG15 0x83C */
#define TIM_GEN_VAP_CFG_VAP_CFG15_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG15_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG15_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG15_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG15_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG16 0x840 */
#define TIM_GEN_VAP_CFG_VAP_CFG16_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG16_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG16_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG16_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG16_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG17 0x844 */
#define TIM_GEN_VAP_CFG_VAP_CFG17_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG17_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG17_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG17_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG17_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG18 0x848 */
#define TIM_GEN_VAP_CFG_VAP_CFG18_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG18_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG18_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG18_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG18_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG19 0x84C */
#define TIM_GEN_VAP_CFG_VAP_CFG19_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG19_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG19_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG19_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG19_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG20 0x850 */
#define TIM_GEN_VAP_CFG_VAP_CFG20_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG20_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG20_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG20_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG20_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG21 0x854 */
#define TIM_GEN_VAP_CFG_VAP_CFG21_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG21_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG21_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG21_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG21_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG22 0x858 */
#define TIM_GEN_VAP_CFG_VAP_CFG22_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG22_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG22_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG22_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG22_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG23 0x85C */
#define TIM_GEN_VAP_CFG_VAP_CFG23_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG23_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG23_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG23_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG23_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG24 0x860 */
#define TIM_GEN_VAP_CFG_VAP_CFG24_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG24_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG24_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG24_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG24_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG25 0x864 */
#define TIM_GEN_VAP_CFG_VAP_CFG25_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG25_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG25_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG25_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG25_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG26 0x868 */
#define TIM_GEN_VAP_CFG_VAP_CFG26_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG26_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG26_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG26_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG26_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG27 0x86C */
#define TIM_GEN_VAP_CFG_VAP_CFG27_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG27_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG27_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG27_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG27_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG28 0x870 */
#define TIM_GEN_VAP_CFG_VAP_CFG28_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG28_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG28_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG28_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG28_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG29 0x874 */
#define TIM_GEN_VAP_CFG_VAP_CFG29_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG29_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG29_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG29_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG29_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG30 0x878 */
#define TIM_GEN_VAP_CFG_VAP_CFG30_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG30_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG30_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG30_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG30_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG31 0x87C */
#define TIM_GEN_VAP_CFG_VAP_CFG31_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG31_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG31_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG31_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG31_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG32 0x880 */
#define TIM_GEN_VAP_CFG_VAP_CFG32_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG32_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG32_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG32_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG32_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG33 0x884 */
#define TIM_GEN_VAP_CFG_VAP_CFG33_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG33_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG33_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG33_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG33_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG34 0x888 */
#define TIM_GEN_VAP_CFG_VAP_CFG34_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG34_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG34_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG34_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG34_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG35 0x88C */
#define TIM_GEN_VAP_CFG_VAP_CFG35_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG35_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG35_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG35_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG35_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG36 0x890 */
#define TIM_GEN_VAP_CFG_VAP_CFG36_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG36_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG36_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG36_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG36_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG37 0x894 */
#define TIM_GEN_VAP_CFG_VAP_CFG37_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG37_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG37_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG37_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG37_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG38 0x898 */
#define TIM_GEN_VAP_CFG_VAP_CFG38_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG38_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG38_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG38_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG38_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG39 0x89C */
#define TIM_GEN_VAP_CFG_VAP_CFG39_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG39_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG39_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG39_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG39_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG40 0x8A0 */
#define TIM_GEN_VAP_CFG_VAP_CFG40_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG40_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG40_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG40_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG40_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG41 0x8A4 */
#define TIM_GEN_VAP_CFG_VAP_CFG41_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG41_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG41_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG41_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG41_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG42 0x8A8 */
#define TIM_GEN_VAP_CFG_VAP_CFG42_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG42_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG42_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG42_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG42_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG43 0x8AC */
#define TIM_GEN_VAP_CFG_VAP_CFG43_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG43_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG43_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG43_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG43_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG44 0x8B0 */
#define TIM_GEN_VAP_CFG_VAP_CFG44_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG44_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG44_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG44_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG44_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG45 0x8B4 */
#define TIM_GEN_VAP_CFG_VAP_CFG45_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG45_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG45_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG45_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG45_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG46 0x8B8 */
#define TIM_GEN_VAP_CFG_VAP_CFG46_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG46_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG46_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG46_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG46_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG47 0x8BC */
#define TIM_GEN_VAP_CFG_VAP_CFG47_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG47_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG47_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG47_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG47_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG48 0x8C0 */
#define TIM_GEN_VAP_CFG_VAP_CFG48_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG48_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG48_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG48_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG48_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG49 0x8C4 */
#define TIM_GEN_VAP_CFG_VAP_CFG49_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG49_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG49_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG49_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG49_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG50 0x8C8 */
#define TIM_GEN_VAP_CFG_VAP_CFG50_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG50_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG50_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG50_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG50_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG51 0x8CC */
#define TIM_GEN_VAP_CFG_VAP_CFG51_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG51_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG51_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG51_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG51_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG52 0x8D0 */
#define TIM_GEN_VAP_CFG_VAP_CFG52_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG52_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG52_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG52_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG52_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG53 0x8D4 */
#define TIM_GEN_VAP_CFG_VAP_CFG53_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG53_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG53_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG53_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG53_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG54 0x8D8 */
#define TIM_GEN_VAP_CFG_VAP_CFG54_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG54_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG54_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG54_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG54_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG55 0x8DC */
#define TIM_GEN_VAP_CFG_VAP_CFG55_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG55_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG55_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG55_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG55_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG56 0x8E0 */
#define TIM_GEN_VAP_CFG_VAP_CFG56_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG56_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG56_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG56_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG56_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG57 0x8E4 */
#define TIM_GEN_VAP_CFG_VAP_CFG57_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG57_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG57_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG57_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG57_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG58 0x8E8 */
#define TIM_GEN_VAP_CFG_VAP_CFG58_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG58_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG58_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG58_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG58_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG59 0x8EC */
#define TIM_GEN_VAP_CFG_VAP_CFG59_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG59_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG59_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG59_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG59_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG60 0x8F0 */
#define TIM_GEN_VAP_CFG_VAP_CFG60_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG60_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG60_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG60_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG60_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG61 0x8F4 */
#define TIM_GEN_VAP_CFG_VAP_CFG61_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG61_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG61_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG61_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG61_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG62 0x8F8 */
#define TIM_GEN_VAP_CFG_VAP_CFG62_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG62_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG62_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG62_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG62_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_VAP_CFG63 0x8FC */
#define TIM_GEN_VAP_CFG_VAP_CFG63_FIELD_MLD_IDX_MASK                                                              0x0000001F
#define TIM_GEN_VAP_CFG_VAP_CFG63_FIELD_IS_MLD_MASK                                                               0x00000040
#define TIM_GEN_VAP_CFG_VAP_CFG63_FIELD_IS_MBSSID_MASK                                                            0x00000080
#define TIM_GEN_VAP_CFG_VAP_CFG63_FIELD_COMPRESSED_MASK                                                           0x00000100
#define TIM_GEN_VAP_CFG_VAP_CFG63_FIELD_VALID_ENTRY_MASK                                                          0x00000200
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD0 0x900 */
#define TIM_GEN_VAP_CFG_AP_MLD0_FIELD_IS_MBSSID0_MASK                                                             0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD0_FIELD_IS_MBSSID1_MASK                                                             0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD0_FIELD_IS_MBSSID2_MASK                                                             0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD0_FIELD_VAP_IDX0_MASK                                                               0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD0_FIELD_VAP_IDX1_MASK                                                               0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD0_FIELD_VAP_IDX2_MASK                                                               0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD0_FIELD_LINK_BITMAP_MASK                                                            0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD1 0x904 */
#define TIM_GEN_VAP_CFG_AP_MLD1_FIELD_IS_MBSSID0_MASK                                                             0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD1_FIELD_IS_MBSSID1_MASK                                                             0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD1_FIELD_IS_MBSSID2_MASK                                                             0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD1_FIELD_VAP_IDX0_MASK                                                               0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD1_FIELD_VAP_IDX1_MASK                                                               0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD1_FIELD_VAP_IDX2_MASK                                                               0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD1_FIELD_LINK_BITMAP_MASK                                                            0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD2 0x908 */
#define TIM_GEN_VAP_CFG_AP_MLD2_FIELD_IS_MBSSID0_MASK                                                             0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD2_FIELD_IS_MBSSID1_MASK                                                             0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD2_FIELD_IS_MBSSID2_MASK                                                             0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD2_FIELD_VAP_IDX0_MASK                                                               0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD2_FIELD_VAP_IDX1_MASK                                                               0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD2_FIELD_VAP_IDX2_MASK                                                               0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD2_FIELD_LINK_BITMAP_MASK                                                            0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD3 0x90C */
#define TIM_GEN_VAP_CFG_AP_MLD3_FIELD_IS_MBSSID0_MASK                                                             0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD3_FIELD_IS_MBSSID1_MASK                                                             0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD3_FIELD_IS_MBSSID2_MASK                                                             0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD3_FIELD_VAP_IDX0_MASK                                                               0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD3_FIELD_VAP_IDX1_MASK                                                               0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD3_FIELD_VAP_IDX2_MASK                                                               0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD3_FIELD_LINK_BITMAP_MASK                                                            0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD4 0x910 */
#define TIM_GEN_VAP_CFG_AP_MLD4_FIELD_IS_MBSSID0_MASK                                                             0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD4_FIELD_IS_MBSSID1_MASK                                                             0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD4_FIELD_IS_MBSSID2_MASK                                                             0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD4_FIELD_VAP_IDX0_MASK                                                               0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD4_FIELD_VAP_IDX1_MASK                                                               0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD4_FIELD_VAP_IDX2_MASK                                                               0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD4_FIELD_LINK_BITMAP_MASK                                                            0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD5 0x914 */
#define TIM_GEN_VAP_CFG_AP_MLD5_FIELD_IS_MBSSID0_MASK                                                             0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD5_FIELD_IS_MBSSID1_MASK                                                             0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD5_FIELD_IS_MBSSID2_MASK                                                             0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD5_FIELD_VAP_IDX0_MASK                                                               0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD5_FIELD_VAP_IDX1_MASK                                                               0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD5_FIELD_VAP_IDX2_MASK                                                               0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD5_FIELD_LINK_BITMAP_MASK                                                            0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD6 0x918 */
#define TIM_GEN_VAP_CFG_AP_MLD6_FIELD_IS_MBSSID0_MASK                                                             0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD6_FIELD_IS_MBSSID1_MASK                                                             0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD6_FIELD_IS_MBSSID2_MASK                                                             0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD6_FIELD_VAP_IDX0_MASK                                                               0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD6_FIELD_VAP_IDX1_MASK                                                               0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD6_FIELD_VAP_IDX2_MASK                                                               0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD6_FIELD_LINK_BITMAP_MASK                                                            0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD7 0x91C */
#define TIM_GEN_VAP_CFG_AP_MLD7_FIELD_IS_MBSSID0_MASK                                                             0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD7_FIELD_IS_MBSSID1_MASK                                                             0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD7_FIELD_IS_MBSSID2_MASK                                                             0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD7_FIELD_VAP_IDX0_MASK                                                               0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD7_FIELD_VAP_IDX1_MASK                                                               0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD7_FIELD_VAP_IDX2_MASK                                                               0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD7_FIELD_LINK_BITMAP_MASK                                                            0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD8 0x920 */
#define TIM_GEN_VAP_CFG_AP_MLD8_FIELD_IS_MBSSID0_MASK                                                             0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD8_FIELD_IS_MBSSID1_MASK                                                             0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD8_FIELD_IS_MBSSID2_MASK                                                             0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD8_FIELD_VAP_IDX0_MASK                                                               0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD8_FIELD_VAP_IDX1_MASK                                                               0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD8_FIELD_VAP_IDX2_MASK                                                               0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD8_FIELD_LINK_BITMAP_MASK                                                            0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD9 0x924 */
#define TIM_GEN_VAP_CFG_AP_MLD9_FIELD_IS_MBSSID0_MASK                                                             0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD9_FIELD_IS_MBSSID1_MASK                                                             0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD9_FIELD_IS_MBSSID2_MASK                                                             0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD9_FIELD_VAP_IDX0_MASK                                                               0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD9_FIELD_VAP_IDX1_MASK                                                               0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD9_FIELD_VAP_IDX2_MASK                                                               0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD9_FIELD_LINK_BITMAP_MASK                                                            0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD10 0x928 */
#define TIM_GEN_VAP_CFG_AP_MLD10_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD10_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD10_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD10_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD10_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD10_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD10_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD11 0x92C */
#define TIM_GEN_VAP_CFG_AP_MLD11_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD11_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD11_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD11_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD11_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD11_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD11_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD12 0x930 */
#define TIM_GEN_VAP_CFG_AP_MLD12_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD12_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD12_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD12_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD12_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD12_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD12_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD13 0x934 */
#define TIM_GEN_VAP_CFG_AP_MLD13_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD13_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD13_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD13_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD13_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD13_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD13_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD14 0x938 */
#define TIM_GEN_VAP_CFG_AP_MLD14_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD14_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD14_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD14_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD14_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD14_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD14_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD15 0x93C */
#define TIM_GEN_VAP_CFG_AP_MLD15_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD15_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD15_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD15_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD15_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD15_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD15_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD16 0x940 */
#define TIM_GEN_VAP_CFG_AP_MLD16_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD16_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD16_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD16_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD16_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD16_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD16_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD17 0x944 */
#define TIM_GEN_VAP_CFG_AP_MLD17_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD17_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD17_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD17_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD17_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD17_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD17_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD18 0x948 */
#define TIM_GEN_VAP_CFG_AP_MLD18_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD18_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD18_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD18_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD18_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD18_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD18_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD19 0x94C */
#define TIM_GEN_VAP_CFG_AP_MLD19_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD19_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD19_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD19_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD19_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD19_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD19_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD20 0x950 */
#define TIM_GEN_VAP_CFG_AP_MLD20_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD20_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD20_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD20_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD20_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD20_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD20_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD21 0x954 */
#define TIM_GEN_VAP_CFG_AP_MLD21_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD21_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD21_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD21_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD21_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD21_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD21_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD22 0x958 */
#define TIM_GEN_VAP_CFG_AP_MLD22_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD22_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD22_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD22_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD22_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD22_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD22_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD23 0x95C */
#define TIM_GEN_VAP_CFG_AP_MLD23_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD23_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD23_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD23_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD23_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD23_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD23_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD24 0x960 */
#define TIM_GEN_VAP_CFG_AP_MLD24_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD24_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD24_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD24_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD24_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD24_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD24_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD25 0x964 */
#define TIM_GEN_VAP_CFG_AP_MLD25_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD25_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD25_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD25_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD25_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD25_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD25_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD26 0x968 */
#define TIM_GEN_VAP_CFG_AP_MLD26_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD26_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD26_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD26_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD26_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD26_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD26_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD27 0x96C */
#define TIM_GEN_VAP_CFG_AP_MLD27_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD27_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD27_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD27_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD27_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD27_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD27_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD28 0x970 */
#define TIM_GEN_VAP_CFG_AP_MLD28_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD28_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD28_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD28_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD28_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD28_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD28_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD29 0x974 */
#define TIM_GEN_VAP_CFG_AP_MLD29_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD29_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD29_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD29_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD29_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD29_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD29_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD30 0x978 */
#define TIM_GEN_VAP_CFG_AP_MLD30_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD30_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD30_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD30_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD30_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD30_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD30_FIELD_LINK_BITMAP_MASK                                                           0x00E00000
//========================================
/* REG_TIM_GEN_VAP_CFG_AP_MLD31 0x97C */
#define TIM_GEN_VAP_CFG_AP_MLD31_FIELD_IS_MBSSID0_MASK                                                            0x00000001
#define TIM_GEN_VAP_CFG_AP_MLD31_FIELD_IS_MBSSID1_MASK                                                            0x00000002
#define TIM_GEN_VAP_CFG_AP_MLD31_FIELD_IS_MBSSID2_MASK                                                            0x00000004
#define TIM_GEN_VAP_CFG_AP_MLD31_FIELD_VAP_IDX0_MASK                                                              0x000001F8
#define TIM_GEN_VAP_CFG_AP_MLD31_FIELD_VAP_IDX1_MASK                                                              0x00007E00
#define TIM_GEN_VAP_CFG_AP_MLD31_FIELD_VAP_IDX2_MASK                                                              0x001F8000
#define TIM_GEN_VAP_CFG_AP_MLD31_FIELD_LINK_BITMAP_MASK                                                           0x00E00000


#endif // _TIM_GEN_VAP_CFG_REGS_H_
