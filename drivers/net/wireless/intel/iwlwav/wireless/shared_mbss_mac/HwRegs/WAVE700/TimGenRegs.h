
/***********************************************************************************
File:				TimGenRegs.h
Module:				timGen
SOC Revision:		latest
Generated at:       2022-05-04 10:58:48
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _TIM_GEN_REGS_H_
#define _TIM_GEN_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define TIM_GEN_BASE_ADDRESS                             MEMORY_MAP_UNIT_1080_BASE_ADDRESS
#define	REG_TIM_GEN_BAND0_TIM_GEN_CTRL        (TIM_GEN_BASE_ADDRESS + 0x0)
#define	REG_TIM_GEN_BAND0_TIM_GEN_DBG         (TIM_GEN_BASE_ADDRESS + 0x4)
#define	REG_TIM_GEN_BAND0_START_VAP_ID        (TIM_GEN_BASE_ADDRESS + 0xC)
#define	REG_TIM_GEN_BAND0_SM_BUSY             (TIM_GEN_BASE_ADDRESS + 0x10)
#define	REG_TIM_GEN_BAND0_VAP_ID              (TIM_GEN_BASE_ADDRESS + 0x14)
#define	REG_TIM_GEN_BAND0_B_VAR               (TIM_GEN_BASE_ADDRESS + 0x18)
#define	REG_TIM_GEN_BAND0_DTIM_PARAM          (TIM_GEN_BASE_ADDRESS + 0x1C)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP0     (TIM_GEN_BASE_ADDRESS + 0x20)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP1     (TIM_GEN_BASE_ADDRESS + 0x24)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP2     (TIM_GEN_BASE_ADDRESS + 0x28)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP3     (TIM_GEN_BASE_ADDRESS + 0x2C)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP4     (TIM_GEN_BASE_ADDRESS + 0x30)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP5     (TIM_GEN_BASE_ADDRESS + 0x34)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP6     (TIM_GEN_BASE_ADDRESS + 0x38)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP7     (TIM_GEN_BASE_ADDRESS + 0x3C)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP8     (TIM_GEN_BASE_ADDRESS + 0x40)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP9     (TIM_GEN_BASE_ADDRESS + 0x44)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP10    (TIM_GEN_BASE_ADDRESS + 0x48)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP11    (TIM_GEN_BASE_ADDRESS + 0x4C)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP12    (TIM_GEN_BASE_ADDRESS + 0x50)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP13    (TIM_GEN_BASE_ADDRESS + 0x54)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP14    (TIM_GEN_BASE_ADDRESS + 0x58)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP15    (TIM_GEN_BASE_ADDRESS + 0x5C)
#define	REG_TIM_GEN_BAND0_TIM_CASE            (TIM_GEN_BASE_ADDRESS + 0x60)
#define	REG_TIM_GEN_BAND1_TIM_GEN_CTRL        (TIM_GEN_BASE_ADDRESS + 0x80)
#define	REG_TIM_GEN_BAND1_TIM_GEN_DBG         (TIM_GEN_BASE_ADDRESS + 0x84)
#define	REG_TIM_GEN_BAND1_START_VAP_ID        (TIM_GEN_BASE_ADDRESS + 0x8C)
#define	REG_TIM_GEN_BAND1_SM_BUSY             (TIM_GEN_BASE_ADDRESS + 0x90)
#define	REG_TIM_GEN_BAND1_VAP_ID              (TIM_GEN_BASE_ADDRESS + 0x94)
#define	REG_TIM_GEN_BAND1_B_VAR               (TIM_GEN_BASE_ADDRESS + 0x98)
#define	REG_TIM_GEN_BAND1_DTIM_PARAM          (TIM_GEN_BASE_ADDRESS + 0x9C)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP0     (TIM_GEN_BASE_ADDRESS + 0xA0)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP1     (TIM_GEN_BASE_ADDRESS + 0xA4)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP2     (TIM_GEN_BASE_ADDRESS + 0xA8)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP3     (TIM_GEN_BASE_ADDRESS + 0xAC)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP4     (TIM_GEN_BASE_ADDRESS + 0xB0)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP5     (TIM_GEN_BASE_ADDRESS + 0xB4)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP6     (TIM_GEN_BASE_ADDRESS + 0xB8)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP7     (TIM_GEN_BASE_ADDRESS + 0xBC)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP8     (TIM_GEN_BASE_ADDRESS + 0xC0)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP9     (TIM_GEN_BASE_ADDRESS + 0xC4)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP10    (TIM_GEN_BASE_ADDRESS + 0xC8)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP11    (TIM_GEN_BASE_ADDRESS + 0xCC)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP12    (TIM_GEN_BASE_ADDRESS + 0xE0)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP13    (TIM_GEN_BASE_ADDRESS + 0xE4)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP14    (TIM_GEN_BASE_ADDRESS + 0xE8)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP15    (TIM_GEN_BASE_ADDRESS + 0xEC)
#define	REG_TIM_GEN_BAND1_TIM_CASE            (TIM_GEN_BASE_ADDRESS + 0xF0)
#define	REG_TIM_GEN_BAND2_TIM_GEN_CTRL        (TIM_GEN_BASE_ADDRESS + 0x100)
#define	REG_TIM_GEN_BAND2_TIM_GEN_DBG         (TIM_GEN_BASE_ADDRESS + 0x104)
#define	REG_TIM_GEN_BAND2_START_VAP_ID        (TIM_GEN_BASE_ADDRESS + 0x10C)
#define	REG_TIM_GEN_BAND2_SM_BUSY             (TIM_GEN_BASE_ADDRESS + 0x110)
#define	REG_TIM_GEN_BAND2_VAP_ID              (TIM_GEN_BASE_ADDRESS + 0x114)
#define	REG_TIM_GEN_BAND2_B_VAR               (TIM_GEN_BASE_ADDRESS + 0x118)
#define	REG_TIM_GEN_BAND2_DTIM_PARAM          (TIM_GEN_BASE_ADDRESS + 0x11C)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP0     (TIM_GEN_BASE_ADDRESS + 0x120)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP1     (TIM_GEN_BASE_ADDRESS + 0x124)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP2     (TIM_GEN_BASE_ADDRESS + 0x128)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP3     (TIM_GEN_BASE_ADDRESS + 0x12C)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP4     (TIM_GEN_BASE_ADDRESS + 0x130)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP5     (TIM_GEN_BASE_ADDRESS + 0x134)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP6     (TIM_GEN_BASE_ADDRESS + 0x138)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP7     (TIM_GEN_BASE_ADDRESS + 0x13C)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP8     (TIM_GEN_BASE_ADDRESS + 0x140)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP9     (TIM_GEN_BASE_ADDRESS + 0x144)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP10    (TIM_GEN_BASE_ADDRESS + 0x148)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP11    (TIM_GEN_BASE_ADDRESS + 0x14C)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP12    (TIM_GEN_BASE_ADDRESS + 0x150)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP13    (TIM_GEN_BASE_ADDRESS + 0x154)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP14    (TIM_GEN_BASE_ADDRESS + 0x158)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP15    (TIM_GEN_BASE_ADDRESS + 0x15C)
#define	REG_TIM_GEN_BAND2_TIM_CASE            (TIM_GEN_BASE_ADDRESS + 0x160)
#define	REG_TIM_GEN_DTIM0                     (TIM_GEN_BASE_ADDRESS + 0x200)
#define	REG_TIM_GEN_DTIM1                     (TIM_GEN_BASE_ADDRESS + 0x204)
#define	REG_TIM_GEN_VAP_BI0                   (TIM_GEN_BASE_ADDRESS + 0x208)
#define	REG_TIM_GEN_VAP_BI1                   (TIM_GEN_BASE_ADDRESS + 0x20C)
#define	REG_TIM_GEN_STA_BI0                   (TIM_GEN_BASE_ADDRESS + 0x210)
#define	REG_TIM_GEN_STA_BI1                   (TIM_GEN_BASE_ADDRESS + 0x214)
#define	REG_TIM_GEN_STA_BI2                   (TIM_GEN_BASE_ADDRESS + 0x218)
#define	REG_TIM_GEN_STA_BI3                   (TIM_GEN_BASE_ADDRESS + 0x21C)
#define	REG_TIM_GEN_STA_BI4                   (TIM_GEN_BASE_ADDRESS + 0x220)
#define	REG_TIM_GEN_STA_BI5                   (TIM_GEN_BASE_ADDRESS + 0x224)
#define	REG_TIM_GEN_STA_BI6                   (TIM_GEN_BASE_ADDRESS + 0x228)
#define	REG_TIM_GEN_STA_BI7                   (TIM_GEN_BASE_ADDRESS + 0x22C)
#define	REG_TIM_GEN_STA_BI8                   (TIM_GEN_BASE_ADDRESS + 0x230)
#define	REG_TIM_GEN_STA_BI9                   (TIM_GEN_BASE_ADDRESS + 0x234)
#define	REG_TIM_GEN_STA_BI10                  (TIM_GEN_BASE_ADDRESS + 0x238)
#define	REG_TIM_GEN_STA_BI11                  (TIM_GEN_BASE_ADDRESS + 0x23C)
#define	REG_TIM_GEN_STA_BI12                  (TIM_GEN_BASE_ADDRESS + 0x240)
#define	REG_TIM_GEN_STA_BI13                  (TIM_GEN_BASE_ADDRESS + 0x244)
#define	REG_TIM_GEN_STA_BI14                  (TIM_GEN_BASE_ADDRESS + 0x248)
#define	REG_TIM_GEN_STA_BI15                  (TIM_GEN_BASE_ADDRESS + 0x24C)
#define	REG_TIM_GEN_STA_PS0                   (TIM_GEN_BASE_ADDRESS + 0x250)
#define	REG_TIM_GEN_STA_PS1                   (TIM_GEN_BASE_ADDRESS + 0x254)
#define	REG_TIM_GEN_STA_PS2                   (TIM_GEN_BASE_ADDRESS + 0x258)
#define	REG_TIM_GEN_STA_PS3                   (TIM_GEN_BASE_ADDRESS + 0x25C)
#define	REG_TIM_GEN_STA_PS4                   (TIM_GEN_BASE_ADDRESS + 0x260)
#define	REG_TIM_GEN_STA_PS5                   (TIM_GEN_BASE_ADDRESS + 0x264)
#define	REG_TIM_GEN_STA_PS6                   (TIM_GEN_BASE_ADDRESS + 0x268)
#define	REG_TIM_GEN_STA_PS7                   (TIM_GEN_BASE_ADDRESS + 0x26C)
#define	REG_TIM_GEN_STA_PS8                   (TIM_GEN_BASE_ADDRESS + 0x270)
#define	REG_TIM_GEN_STA_PS9                   (TIM_GEN_BASE_ADDRESS + 0x274)
#define	REG_TIM_GEN_STA_PS10                  (TIM_GEN_BASE_ADDRESS + 0x278)
#define	REG_TIM_GEN_STA_PS11                  (TIM_GEN_BASE_ADDRESS + 0x27C)
#define	REG_TIM_GEN_STA_PS12                  (TIM_GEN_BASE_ADDRESS + 0x280)
#define	REG_TIM_GEN_STA_PS13                  (TIM_GEN_BASE_ADDRESS + 0x284)
#define	REG_TIM_GEN_STA_PS14                  (TIM_GEN_BASE_ADDRESS + 0x288)
#define	REG_TIM_GEN_STA_PS15                  (TIM_GEN_BASE_ADDRESS + 0x28C)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_TIM_GEN_BAND0_TIM_GEN_CTRL 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimGenEnable : 1; //Enable TIM GEN BAND0    , reset value: 0x0, access type: RW
		uint32 band0SegBZeroI : 1; //Indicates to set Segment B with value all 0, reset value: 0x0, access type: RW
		uint32 band0SegBDtimI : 1; //Indicates to ignore the DTIM of the reported AP MLDs, reset value: 0x0, access type: RW
		uint32 reserved0 : 13;
		uint32 band0MaxNumOfBssidsI : 6; //Used by FW to set the maximum number of members in multiple BSSID set. If M-BSSID is supported then possible values are 2, 4, 8, 16, 32; else the value is 1., reset value: 0x0, access type: RW
		uint32 reserved1 : 10;
	} bitFields;
} RegTimGenBand0TimGenCtrl_u;

/*REG_TIM_GEN_BAND0_TIM_GEN_DBG 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0InvalidStaInt : 1; //Error event - Access to non valid MLD DB entry , reset value: 0x0, access type: RO
		uint32 band0InvalidVapInt : 1; //Error event - Access to non valid VAP CFG table entry , reset value: 0x0, access type: RO
		uint32 band0FalseStartInt : 1; //Error event - Atempt to start during busy , reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 band0InvalidStaClr : 1; //Error event Clear (Write 1 to clear) - Access to non valid MLD DB entry , reset value: 0x0, access type: WO
		uint32 band0InvalidVapClr : 1; //Error event Clear (Write 1 to clear) - Access to non valid VAP CFG table entry , reset value: 0x0, access type: WO
		uint32 band0FalseStartClr : 1; //Error event Clear (Write 1 to clear) - Atempt to start during busy , reset value: 0x0, access type: WO
		uint32 reserved1 : 9;
		uint32 band0FsmState : 4; //FSM State, reset value: 0x0, access type: RO
		uint32 reserved2 : 12;
	} bitFields;
} RegTimGenBand0TimGenDbg_u;

/*REG_TIM_GEN_BAND0_START_VAP_ID 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0StartVapIdI : 6; //Can be used by Sender to trigger the SM. Start includes the VAP_ID (6b) of the transmitted BSSID , (WR transaction to this register starting TIM GEN FSM of corresponding BAND) , reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegTimGenBand0StartVapId_u;

/*REG_TIM_GEN_BAND0_SM_BUSY 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0SmBusyI : 1; //sm_busy indicates SM is in progress, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegTimGenBand0SmBusy_u;

/*REG_TIM_GEN_BAND0_VAP_ID 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0VapIdI : 6; //The VAP_ID outputted by the Selector, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegTimGenBand0VapId_u;

/*REG_TIM_GEN_BAND0_B_VAR 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0B0VarI : 9; //Intermediate variable B0, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 band0B1VarI : 9; //Intermediate variable B1, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 band0B2VarI : 9; //Intermediate variable B2, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
	} bitFields;
} RegTimGenBand0BVar_u;

/*REG_TIM_GEN_BAND0_DTIM_PARAM 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0DtimCountI : 8; //Reflects the DTIM Count  from selector, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
		uint32 band0DtimPeriodI : 8; //Reflects the DTIM Period from selector, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegTimGenBand0DtimParam_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP0 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap0I : 32; //The last generated TIM header (bytes 0-3), including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap0_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP1 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap1I : 32; //The last generated TIM header (byte 4) + TIM bitmap (bytes 0-2) concatenated , including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap1_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP2 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap2I : 32; //The last generated TIM bitmap. Result of SM (bytes 3-6), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap2_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP3 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap3I : 32; //The last generated TIM bitmap. Result of SM (bytes 7-10), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap3_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP4 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap4I : 32; //The last generated TIM bitmap. Result of SM (bytes 11-14), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap4_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP5 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap5I : 32; //The last generated TIM bitmap. Result of SM (bytes 15-18), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap5_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP6 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap6I : 32; //The last generated TIM bitmap. Result of SM (bytes 19-22), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap6_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP7 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap7I : 32; //The last generated TIM bitmap. Result of SM (bytes 23-26), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap7_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP8 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap8I : 32; //The last generated TIM bitmap. Result of SM (bytes 27-30), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap8_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP9 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap9I : 32; //The last generated TIM bitmap. Result of SM (bytes 31-34), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap9_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP10 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap10I : 32; //The last generated TIM bitmap. Result of SM (bytes 35-38), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap10_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP11 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap11I : 32; //The last generated TIM bitmap. Result of SM (bytes 39-42), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap11_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP12 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap12I : 32; //The last generated TIM bitmap. Result of SM (bytes 43-46), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap12_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP13 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap13I : 32; //The last generated TIM bitmap. Result of SM (bytes 47-50), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap13_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP14 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap14I : 32; //The last generated TIM bitmap. Result of SM (bytes 51-54), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap14_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP15 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap15I : 32; //The last generated TIM bitmap. Result of SM (byte 55), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap15_u;

/*REG_TIM_GEN_BAND0_TIM_CASE 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimCaseI : 3; //Result of SM (Table 8), used by Sender, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegTimGenBand0TimCase_u;

/*REG_TIM_GEN_BAND1_TIM_GEN_CTRL 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimGenEnable : 1; //Enable TIM GEN BAND1, reset value: 0x0, access type: RW
		uint32 band1SegBZeroI : 1; //Indicates to set Segment B with value all 0, reset value: 0x0, access type: RW
		uint32 band1SegBDtimI : 1; //Indicates to ignore the DTIM of the reported AP MLDs, reset value: 0x0, access type: RW
		uint32 reserved0 : 13;
		uint32 band1MaxNumOfBssidsI : 6; //Used by FW to set the maximum number of members in multiple BSSID set. If M-BSSID is supported then possible values are 2, 4, 8, 16, 32; else the value is 1., reset value: 0x0, access type: RW
		uint32 reserved1 : 10;
	} bitFields;
} RegTimGenBand1TimGenCtrl_u;

/*REG_TIM_GEN_BAND1_TIM_GEN_DBG 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1InvalidStaInt : 1; //Error event - Access to non valid MLD DB entry , reset value: 0x0, access type: RO
		uint32 band1InvalidVapInt : 1; //Error event - Access to non valid VAP CFG table entry , reset value: 0x0, access type: RO
		uint32 band1FalseStartInt : 1; //Error event - Atempt to start during busy , reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 band1InvalidStaClr : 1; //Error event Clear (Write 1 to clear) - Access to non valid MLD DB entry , reset value: 0x0, access type: WO
		uint32 band1InvalidVapClr : 1; //Error event Clear (Write 1 to clear) - Access to non valid VAP CFG table entry , reset value: 0x0, access type: WO
		uint32 band1FalseStartClr : 1; //Error event Clear (Write 1 to clear) - Atempt to start during busy , reset value: 0x0, access type: WO
		uint32 reserved1 : 9;
		uint32 band1FsmState : 4; //FSM State, reset value: 0x0, access type: RO
		uint32 reserved2 : 12;
	} bitFields;
} RegTimGenBand1TimGenDbg_u;

/*REG_TIM_GEN_BAND1_START_VAP_ID 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1StartVapIdI : 6; //Can be used by Sender to trigger the SM. Start includes the VAP_ID (6b) of the transmitted BSSID, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegTimGenBand1StartVapId_u;

/*REG_TIM_GEN_BAND1_SM_BUSY 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1SmBusyI : 1; //sm_busy indicates SM is in progress, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegTimGenBand1SmBusy_u;

/*REG_TIM_GEN_BAND1_VAP_ID 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1VapIdI : 6; //The VAP_ID outputted by the Selector, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegTimGenBand1VapId_u;

/*REG_TIM_GEN_BAND1_B_VAR 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1B0VarI : 9; //Intermediate variable B0, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 band1B1VarI : 9; //Intermediate variable B1, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 band1B2VarI : 9; //Intermediate variable B2, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
	} bitFields;
} RegTimGenBand1BVar_u;

/*REG_TIM_GEN_BAND1_DTIM_PARAM 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1DtimCountI : 8; //Reflects the DTIM Count  from selector, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
		uint32 band1DtimPeriodI : 8; //Reflects the DTIM Period from selector, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegTimGenBand1DtimParam_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP0 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap0I : 32; //The last generated TIM header (bytes 0-3), including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap0_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP1 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap1I : 32; //The last generated TIM header (byte 4) + TIM bitmap (bytes 0-2) concatenated , including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap1_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP2 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap2I : 32; //The last generated TIM bitmap. Result of SM (bytes 3-6), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap2_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP3 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap3I : 32; //The last generated TIM bitmap. Result of SM (bytes 7-10), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap3_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP4 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap4I : 32; //The last generated TIM bitmap. Result of SM (bytes 11-14), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap4_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP5 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap5I : 32; //The last generated TIM bitmap. Result of SM (bytes 15-18), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap5_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP6 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap6I : 32; //The last generated TIM bitmap. Result of SM (bytes 19-22), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap6_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP7 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap7I : 32; //The last generated TIM bitmap. Result of SM (bytes 23-26), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap7_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP8 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap8I : 32; //The last generated TIM bitmap. Result of SM (bytes 27-30), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap8_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP9 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap9I : 32; //The last generated TIM bitmap. Result of SM (bytes 31-34), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap9_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP10 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap10I : 32; //The last generated TIM bitmap. Result of SM (bytes 35-38), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap10_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP11 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap11I : 32; //The last generated TIM bitmap. Result of SM (bytes 39-42), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap11_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP12 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap12I : 32; //The last generated TIM bitmap. Result of SM (bytes 43-46), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap12_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP13 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap13I : 32; //The last generated TIM bitmap. Result of SM (bytes 47-50), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap13_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP14 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap14I : 32; //The last generated TIM bitmap. Result of SM (bytes 51-54), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap14_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP15 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap15I : 32; //The last generated TIM bitmap. Result of SM (byte 55), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap15_u;

/*REG_TIM_GEN_BAND1_TIM_CASE 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimCaseI : 3; //Result of SM (Table 8), used by Sender, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegTimGenBand1TimCase_u;

/*REG_TIM_GEN_BAND2_TIM_GEN_CTRL 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimGenEnable : 1; //Enable TIM GEN BAND2, reset value: 0x0, access type: RW
		uint32 band2SegBZeroI : 1; //Indicates to set Segment B with value all 0, reset value: 0x0, access type: RW
		uint32 band2SegBDtimI : 1; //Indicates to ignore the DTIM of the reported AP MLDs, reset value: 0x0, access type: RW
		uint32 reserved0 : 13;
		uint32 band2MaxNumOfBssidsI : 6; //Used by FW to set the maximum number of members in multiple BSSID set. If M-BSSID is supported then possible values are 2, 4, 8, 16, 32; else the value is 1., reset value: 0x0, access type: RW
		uint32 reserved1 : 10;
	} bitFields;
} RegTimGenBand2TimGenCtrl_u;

/*REG_TIM_GEN_BAND2_TIM_GEN_DBG 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2InvalidStaInt : 1; //Error event - Access to non valid MLD DB entry , reset value: 0x0, access type: RO
		uint32 band2InvalidVapInt : 1; //Error event - Access to non valid VAP CFG table entry , reset value: 0x0, access type: RO
		uint32 band2FalseStartInt : 1; //Error event - Atempt to start during busy , reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 band2InvalidStaClr : 1; //Error event Clear (Write 1 to clear) - Access to non valid MLD DB entry , reset value: 0x0, access type: WO
		uint32 band2InvalidVapClr : 1; //Error event Clear (Write 1 to clear) - Access to non valid VAP CFG table entry , reset value: 0x0, access type: WO
		uint32 band2FalseStartClr : 1; //Error event Clear (Write 1 to clear) - Atempt to start during busy , reset value: 0x0, access type: WO
		uint32 reserved1 : 9;
		uint32 band2FsmState : 4; //FSM State, reset value: 0x0, access type: RO
		uint32 reserved2 : 12;
	} bitFields;
} RegTimGenBand2TimGenDbg_u;

/*REG_TIM_GEN_BAND2_START_VAP_ID 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2StartVapIdI : 6; //Can be used by Sender to trigger the SM. Start includes the VAP_ID (6b) of the transmitted BSSID, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegTimGenBand2StartVapId_u;

/*REG_TIM_GEN_BAND2_SM_BUSY 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2SmBusyI : 1; //sm_busy indicates SM is in progress, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegTimGenBand2SmBusy_u;

/*REG_TIM_GEN_BAND2_VAP_ID 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2VapIdI : 6; //The VAP_ID outputted by the Selector, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegTimGenBand2VapId_u;

/*REG_TIM_GEN_BAND2_B_VAR 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2B0VarI : 9; //Intermediate variable B0, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 band2B1VarI : 9; //Intermediate variable B1, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 band2B2VarI : 9; //Intermediate variable B2, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
	} bitFields;
} RegTimGenBand2BVar_u;

/*REG_TIM_GEN_BAND2_DTIM_PARAM 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2DtimCountI : 8; //Reflects the DTIM Count  from selector, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
		uint32 band2DtimPeriodI : 8; //Reflects the DTIM Period from selector, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegTimGenBand2DtimParam_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP0 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap0I : 32; //The last generated TIM header (bytes 0-3), including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap0_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP1 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap1I : 32; //The last generated TIM header (byte 4) + TIM bitmap (bytes 0-2) concatenated , including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap1_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP2 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap2I : 32; //The last generated TIM bitmap. Result of SM (bytes 3-6), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap2_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP3 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap3I : 32; //The last generated TIM bitmap. Result of SM (bytes 7-10), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap3_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP4 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap4I : 32; //The last generated TIM bitmap. Result of SM (bytes 11-14), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap4_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP5 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap5I : 32; //The last generated TIM bitmap. Result of SM (bytes 15-18), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap5_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP6 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap6I : 32; //The last generated TIM bitmap. Result of SM (bytes 19-22), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap6_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP7 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap7I : 32; //The last generated TIM bitmap. Result of SM (bytes 23-26), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap7_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP8 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap8I : 32; //The last generated TIM bitmap. Result of SM (bytes 27-30), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap8_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP9 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap9I : 32; //The last generated TIM bitmap. Result of SM (bytes 31-34), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap9_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP10 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap10I : 32; //The last generated TIM bitmap. Result of SM (bytes 35-38), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap10_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP11 0x14C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap11I : 32; //The last generated TIM bitmap. Result of SM (bytes 39-42), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap11_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP12 0x150 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap12I : 32; //The last generated TIM bitmap. Result of SM (bytes 43-46), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap12_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP13 0x154 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap13I : 32; //The last generated TIM bitmap. Result of SM (bytes 47-50), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap13_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP14 0x158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap14I : 32; //The last generated TIM bitmap. Result of SM (bytes 51-54), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap14_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP15 0x15C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap15I : 32; //The last generated TIM bitmap. Result of SM (byte 55), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap15_u;

/*REG_TIM_GEN_BAND2_TIM_CASE 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimCaseI : 3; //Result of SM (Table 8), used by Sender, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegTimGenBand2TimCase_u;

/*REG_TIM_GEN_DTIM0 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dtim0I : 32; //Reflects the DTIM bitmap outputted by the Selectors (bits 31-0), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenDtim0_u;

/*REG_TIM_GEN_DTIM1 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dtim1I : 32; //Reflects the DTIM bitmap outputted by the Selectors (bits 63-32), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenDtim1_u;

/*REG_TIM_GEN_VAP_BI0 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vapBi0I : 32; //Reflects the VAP bit indication outputted by the Selectors (bits 31-0), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenVapBi0_u;

/*REG_TIM_GEN_VAP_BI1 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vapBi1I : 32; //Reflects the VAP bit indication outputted by the Selectors (bits 63-32), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenVapBi1_u;

/*REG_TIM_GEN_STA_BI0 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi0I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 31-0), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi0_u;

/*REG_TIM_GEN_STA_BI1 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi1I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 63-32), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi1_u;

/*REG_TIM_GEN_STA_BI2 0x218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi2I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 95-64), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi2_u;

/*REG_TIM_GEN_STA_BI3 0x21C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi3I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 127-96), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi3_u;

/*REG_TIM_GEN_STA_BI4 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi4I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 159-128), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi4_u;

/*REG_TIM_GEN_STA_BI5 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi5I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 191-160), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi5_u;

/*REG_TIM_GEN_STA_BI6 0x228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi6I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 223-192), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi6_u;

/*REG_TIM_GEN_STA_BI7 0x22C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi7I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 255-224), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi7_u;

/*REG_TIM_GEN_STA_BI8 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi8I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 287-256), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi8_u;

/*REG_TIM_GEN_STA_BI9 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi9I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 319-288), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi9_u;

/*REG_TIM_GEN_STA_BI10 0x238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi10I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 351-320), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi10_u;

/*REG_TIM_GEN_STA_BI11 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi11I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 383-352), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi11_u;

/*REG_TIM_GEN_STA_BI12 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi12I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 415-384), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi12_u;

/*REG_TIM_GEN_STA_BI13 0x244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi13I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 447-416), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi13_u;

/*REG_TIM_GEN_STA_BI14 0x248 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi14I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 479-448), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi14_u;

/*REG_TIM_GEN_STA_BI15 0x24C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staBi15I : 32; //Reflects the STA bit indication outputted by the Selectors (bits 511-480), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaBi15_u;

/*REG_TIM_GEN_STA_PS0 0x250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs0I : 32; //Reflects the STA power state outputted by the Selectors (bits 31-0), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs0_u;

/*REG_TIM_GEN_STA_PS1 0x254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs1I : 32; //Reflects the STA power state outputted by the Selectors (bits 63-32), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs1_u;

/*REG_TIM_GEN_STA_PS2 0x258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs2I : 32; //Reflects the STA power state outputted by the Selectors (bits 95-64), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs2_u;

/*REG_TIM_GEN_STA_PS3 0x25C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs3I : 32; //Reflects the STA power state outputted by the Selectors (bits 127-96), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs3_u;

/*REG_TIM_GEN_STA_PS4 0x260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs4I : 32; //Reflects the STA power state outputted by the Selectors (bits 159-128), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs4_u;

/*REG_TIM_GEN_STA_PS5 0x264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs5I : 32; //Reflects the STA power state outputted by the Selectors (bits 191-160), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs5_u;

/*REG_TIM_GEN_STA_PS6 0x268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs6I : 32; //Reflects the STA power state outputted by the Selectors (bits 223-192), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs6_u;

/*REG_TIM_GEN_STA_PS7 0x26C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs7I : 32; //Reflects the STA power state outputted by the Selectors (bits 255-224), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs7_u;

/*REG_TIM_GEN_STA_PS8 0x270 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs8I : 32; //Reflects the STA power state outputted by the Selectors (bits 287-256), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs8_u;

/*REG_TIM_GEN_STA_PS9 0x274 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs9I : 32; //Reflects the STA power state outputted by the Selectors (bits 319-288), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs9_u;

/*REG_TIM_GEN_STA_PS10 0x278 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs10I : 32; //Reflects the STA power state outputted by the Selectors (bits 351-320), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs10_u;

/*REG_TIM_GEN_STA_PS11 0x27C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs11I : 32; //Reflects the STA power state outputted by the Selectors (bits 383-352), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs11_u;

/*REG_TIM_GEN_STA_PS12 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs12I : 32; //Reflects the STA power state outputted by the Selectors (bits 415-384), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs12_u;

/*REG_TIM_GEN_STA_PS13 0x284 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs13I : 32; //Reflects the STA power state outputted by the Selectors (bits 447-416), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs13_u;

/*REG_TIM_GEN_STA_PS14 0x288 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs14I : 32; //Reflects the STA power state outputted by the Selectors (bits 479-448), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs14_u;

/*REG_TIM_GEN_STA_PS15 0x28C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs15I : 32; //Reflects the STA power state outputted by the Selectors (bits 511-480), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs15_u;



#endif // _TIM_GEN_REGS_H_
