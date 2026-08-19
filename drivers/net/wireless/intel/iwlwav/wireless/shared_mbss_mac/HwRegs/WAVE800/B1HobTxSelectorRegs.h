
/***********************************************************************************
File:				B1HobTxSelectorRegs.h
Module:				b1HobTxSelector
SOC Revision:		latest
Generated at:       2024-06-26 12:54:37
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B1_HOB_TX_SELECTOR_REGS_H_
#define _B1_HOB_TX_SELECTOR_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B1_HOB_TX_SELECTOR_BASE_ADDRESS            MEMORY_MAP_UNIT_10505_BASE_ADDRESS
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_CONTROL               (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x0)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP_CONTROL        (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x4)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_MPS_GROUP_MSB         (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x8)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP0               (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0xC)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP1               (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x10)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP2               (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x14)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP3               (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x18)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP4               (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x1C)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP5               (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x20)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP6               (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x24)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP7               (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x28)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP8               (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x2C)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP9               (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x30)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP10              (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x34)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_HOB_RECIPE_FIELDS0    (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x38)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_HOB_RECIPE_FIELDS1    (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x3C)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_HOB_DEBUG_FIELDS0     (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x40)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_READ_DTIM_VAP_IDX     (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x44)
#define	REG_B1_HOB_TX_SELECTOR_TIM_IE_READ_DTIM_PARAMS      (B1_HOB_TX_SELECTOR_BASE_ADDRESS + 0x48)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B1_HOB_TX_SELECTOR_TIM_IE_CONTROL 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 elementId : 8; //no description, reset value: 0x5, access type: RO
		uint32 length : 8; //no description, reset value: 0x0, access type: RO
		uint32 dtimCount : 8; //no description, reset value: 0x0, access type: RO
		uint32 dtimPeriod : 8; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobTxSelectorTimIeControl_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP_CONTROL 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timIeBitmapControl : 8; //bitmap_control, reset value: 0x0, access type: RO
		uint32 mpsGroup : 24; //MBSSID - MPS group field value. , SBSSID - 0 , reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobTxSelectorTimIeBitmapControl_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_MPS_GROUP_MSB 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mpsGroupMsbByte : 8; //MBSSID - MPS group field value. , SBSSID - 0 , reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB1HobTxSelectorTimIeMpsGroupMsb_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP0 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timIeBitmapB3B0 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobTxSelectorTimIeBitmap0_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP1 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timIeBitmapB7B4 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobTxSelectorTimIeBitmap1_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP2 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timIeBitmapB11B8 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobTxSelectorTimIeBitmap2_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP3 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timIeBitmapB15B12 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobTxSelectorTimIeBitmap3_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP4 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timIeBitmapB19B16 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobTxSelectorTimIeBitmap4_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP5 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timIeBitmapB23B20 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobTxSelectorTimIeBitmap5_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP6 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timIeBitmapB27B24 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobTxSelectorTimIeBitmap6_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP7 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timIeBitmapB31B28 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobTxSelectorTimIeBitmap7_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP8 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timIeBitmapB35B32 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobTxSelectorTimIeBitmap8_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP9 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timIeBitmapB39B36 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1HobTxSelectorTimIeBitmap9_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_BITMAP10 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timIeBitmapB40 : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB1HobTxSelectorTimIeBitmap10_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_HOB_RECIPE_FIELDS0 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 hobReadLength : 8; //no description, reset value: 0x0, access type: RO
		uint32 hobReadOffset : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1HobTxSelectorTimIeHobRecipeFields0_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_HOB_RECIPE_FIELDS1 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timIeBitmapControlLength : 3; //Length of bitmap_control read: , SBSSID - 1 , MBSSID - 1 + group size in bytes., reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1HobTxSelectorTimIeHobRecipeFields1_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_HOB_DEBUG_FIELDS0 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timEmpty : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 7;
		uint32 timReady : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
		uint32 timIeIsLegacy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 15;
	} bitFields;
} RegB1HobTxSelectorTimIeHobDebugFields0_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_READ_DTIM_VAP_IDX 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 deliaReadDtimVapIdx : 5; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB1HobTxSelectorTimIeReadDtimVapIdx_u;

/*REG_B1_HOB_TX_SELECTOR_TIM_IE_READ_DTIM_PARAMS 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 deliaReadDtimPeriod : 8; //no description, reset value: 0x0, access type: RO
		uint32 deliaReadDtimCount : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1HobTxSelectorTimIeReadDtimParams_u;

//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_CONTROL 0x0 */
#define B1_HOB_TX_SELECTOR_TIM_IE_CONTROL_FIELD_ELEMENT_ID_MASK                                                   0x000000FF
#define B1_HOB_TX_SELECTOR_TIM_IE_CONTROL_FIELD_LENGTH_MASK                                                       0x0000FF00
#define B1_HOB_TX_SELECTOR_TIM_IE_CONTROL_FIELD_DTIM_COUNT_MASK                                                   0x00FF0000
#define B1_HOB_TX_SELECTOR_TIM_IE_CONTROL_FIELD_DTIM_PERIOD_MASK                                                  0xFF000000
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_BITMAP_CONTROL 0x4 */
#define B1_HOB_TX_SELECTOR_TIM_IE_BITMAP_CONTROL_FIELD_TIM_IE_BITMAP_CONTROL_MASK                                 0x000000FF
#define B1_HOB_TX_SELECTOR_TIM_IE_BITMAP_CONTROL_FIELD_MPS_GROUP_MASK                                             0xFFFFFF00
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_MPS_GROUP_MSB 0x8 */
#define B1_HOB_TX_SELECTOR_TIM_IE_MPS_GROUP_MSB_FIELD_MPS_GROUP_MSB_BYTE_MASK                                     0x000000FF
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_BITMAP0 0xC */
#define B1_HOB_TX_SELECTOR_TIM_IE_BITMAP0_FIELD_TIM_IE_BITMAP_B3_B0_MASK                                          0xFFFFFFFF
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_BITMAP1 0x10 */
#define B1_HOB_TX_SELECTOR_TIM_IE_BITMAP1_FIELD_TIM_IE_BITMAP_B7_B4_MASK                                          0xFFFFFFFF
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_BITMAP2 0x14 */
#define B1_HOB_TX_SELECTOR_TIM_IE_BITMAP2_FIELD_TIM_IE_BITMAP_B11_B8_MASK                                         0xFFFFFFFF
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_BITMAP3 0x18 */
#define B1_HOB_TX_SELECTOR_TIM_IE_BITMAP3_FIELD_TIM_IE_BITMAP_B15_B12_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_BITMAP4 0x1C */
#define B1_HOB_TX_SELECTOR_TIM_IE_BITMAP4_FIELD_TIM_IE_BITMAP_B19_B16_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_BITMAP5 0x20 */
#define B1_HOB_TX_SELECTOR_TIM_IE_BITMAP5_FIELD_TIM_IE_BITMAP_B23_B20_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_BITMAP6 0x24 */
#define B1_HOB_TX_SELECTOR_TIM_IE_BITMAP6_FIELD_TIM_IE_BITMAP_B27_B24_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_BITMAP7 0x28 */
#define B1_HOB_TX_SELECTOR_TIM_IE_BITMAP7_FIELD_TIM_IE_BITMAP_B31_B28_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_BITMAP8 0x2C */
#define B1_HOB_TX_SELECTOR_TIM_IE_BITMAP8_FIELD_TIM_IE_BITMAP_B35_B32_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_BITMAP9 0x30 */
#define B1_HOB_TX_SELECTOR_TIM_IE_BITMAP9_FIELD_TIM_IE_BITMAP_B39_B36_MASK                                        0xFFFFFFFF
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_BITMAP10 0x34 */
#define B1_HOB_TX_SELECTOR_TIM_IE_BITMAP10_FIELD_TIM_IE_BITMAP_B40_MASK                                           0x000000FF
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_HOB_RECIPE_FIELDS0 0x38 */
#define B1_HOB_TX_SELECTOR_TIM_IE_HOB_RECIPE_FIELDS0_FIELD_HOB_READ_LENGTH_MASK                                   0x000000FF
#define B1_HOB_TX_SELECTOR_TIM_IE_HOB_RECIPE_FIELDS0_FIELD_HOB_READ_OFFSET_MASK                                   0x0000FF00
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_HOB_RECIPE_FIELDS1 0x3C */
#define B1_HOB_TX_SELECTOR_TIM_IE_HOB_RECIPE_FIELDS1_FIELD_TIM_IE_BITMAP_CONTROL_LENGTH_MASK                      0x00000007
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_HOB_DEBUG_FIELDS0 0x40 */
#define B1_HOB_TX_SELECTOR_TIM_IE_HOB_DEBUG_FIELDS0_FIELD_TIM_EMPTY_MASK                                          0x00000001
#define B1_HOB_TX_SELECTOR_TIM_IE_HOB_DEBUG_FIELDS0_FIELD_TIM_READY_MASK                                          0x00000100
#define B1_HOB_TX_SELECTOR_TIM_IE_HOB_DEBUG_FIELDS0_FIELD_TIM_IE_IS_LEGACY_MASK                                   0x00010000
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_READ_DTIM_VAP_IDX 0x44 */
#define B1_HOB_TX_SELECTOR_TIM_IE_READ_DTIM_VAP_IDX_FIELD_DELIA_READ_DTIM_VAP_IDX_MASK                            0x0000001F
//========================================
/* REG_HOB_TX_SELECTOR_TIM_IE_READ_DTIM_PARAMS 0x48 */
#define B1_HOB_TX_SELECTOR_TIM_IE_READ_DTIM_PARAMS_FIELD_DELIA_READ_DTIM_PERIOD_MASK                              0x000000FF
#define B1_HOB_TX_SELECTOR_TIM_IE_READ_DTIM_PARAMS_FIELD_DELIA_READ_DTIM_COUNT_MASK                               0x0000FF00


#endif // _HOB_TX_SELECTOR_REGS_H_
