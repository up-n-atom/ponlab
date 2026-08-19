
/***********************************************************************************
File:				B2TxSelectorBitmapRegs.h
Module:				b2TxSelectorBitmap
SOC Revision:		latest
Generated at:       2022-05-04 10:58:34
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_TX_SELECTOR_BITMAP_REGS_H_
#define _B2_TX_SELECTOR_BITMAP_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_TX_SELECTOR_BITMAP_BASE_ADDRESS                             MEMORY_MAP_UNIT_20029_BASE_ADDRESS
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP0                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1A80)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP1                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1A84)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP2                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1A88)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP3                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1A8C)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP4                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1A90)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP5                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1A94)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP6                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1A98)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP7                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1A9C)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP8                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AA0)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP9                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AA4)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP10                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AA8)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP11                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AAC)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP12                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AB0)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP13                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AB4)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP14                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AB8)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP15                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1ABC)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP16                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AC0)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP17                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AC4)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP18                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AC8)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP19                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1ACC)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP20                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AD0)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP21                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AD4)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP22                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AD8)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP23                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1ADC)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP24                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AE0)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP25                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AE4)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP26                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AE8)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP27                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AEC)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP28                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AF0)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP29                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AF4)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP30                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AF8)
#define	REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP31                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1AFC)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP0                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B00)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP1                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B04)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP2                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B08)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP3                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B0C)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP4                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B10)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP5                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B14)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP6                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B18)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP7                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B1C)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP8                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B20)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP9                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B24)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP10                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B28)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP11                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B2C)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP12                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B30)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP13                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B34)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP14                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B38)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP15                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B3C)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP16                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B40)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP17                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B44)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP18                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B48)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP19                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B4C)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP20                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B50)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP21                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B54)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP22                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B58)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP23                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B5C)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP24                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B60)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP25                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B64)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP26                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B68)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP27                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B6C)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP28                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B70)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP29                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B74)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP30                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B78)
#define	REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP31                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B7C)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP0                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B80)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP1                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B84)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP2                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B88)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP3                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B8C)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP4                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B90)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP5                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B94)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP6                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B98)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP7                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1B9C)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP8                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BA0)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP9                                       (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BA4)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP10                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BA8)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP11                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BAC)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP12                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BB0)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP13                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BB4)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP14                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BB8)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP15                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BBC)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP16                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BC0)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP17                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BC4)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP18                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BC8)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP19                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BCC)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP20                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BD0)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP21                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BD4)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP22                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BD8)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP23                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BDC)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP24                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BE0)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP25                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BE4)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP26                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BE8)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP27                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BEC)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP28                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BF0)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP29                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BF4)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP30                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BF8)
#define	REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP31                                      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1BFC)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP0                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C00)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP1                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C04)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP2                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C08)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP3                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C0C)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP4                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C10)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP5                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C14)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP6                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C18)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP7                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C1C)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP8                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C20)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP9                                     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C24)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP10                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C28)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP11                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C2C)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP12                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C30)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP13                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C34)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP14                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C38)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP15                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C3C)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP16                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C40)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP17                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C44)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP18                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C48)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP19                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C4C)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP20                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C50)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP21                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C54)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP22                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C58)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP23                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C5C)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP24                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C60)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP25                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C64)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP26                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C68)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP27                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C6C)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP28                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C70)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP29                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C74)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP30                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C78)
#define	REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP31                                    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C7C)
#define	REG_B2_TX_SELECTOR_BITMAP_PLAN_LOCK_BITMAP_31_0                              (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C80)
#define	REG_B2_TX_SELECTOR_BITMAP_PLAN_LOCK_BITMAP_63_32                             (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C84)
#define	REG_B2_TX_SELECTOR_BITMAP_PLAN_LOCK_BITMAP_95_64                             (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C88)
#define	REG_B2_TX_SELECTOR_BITMAP_PLAN_LOCK_BITMAP_127_96                            (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C8C)
#define	REG_B2_TX_SELECTOR_BITMAP_PLAN_UL_HE_SELECTION_WITHOUT_DATA_BITMAP_31_0      (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C90)
#define	REG_B2_TX_SELECTOR_BITMAP_PLAN_UL_HE_SELECTION_WITHOUT_DATA_BITMAP_63_32     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C94)
#define	REG_B2_TX_SELECTOR_BITMAP_PLAN_UL_HE_SELECTION_WITHOUT_DATA_BITMAP_95_64     (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C98)
#define	REG_B2_TX_SELECTOR_BITMAP_PLAN_UL_HE_SELECTION_WITHOUT_DATA_BITMAP_127_96    (B2_TX_SELECTOR_BITMAP_BASE_ADDRESS + 0x1C9C)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP0 0x1A80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap0DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap0LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap0DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap0LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap0DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap0LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap0DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap0LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap0LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap0Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap0_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP1 0x1A84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap1DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap1LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap1DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap1LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap1DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap1LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap1DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap1LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap1LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap1Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap1_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP2 0x1A88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap2DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap2LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap2DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap2LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap2DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap2LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap2DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap2LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap2LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap2Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap2_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP3 0x1A8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap3DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap3LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap3DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap3LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap3DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap3LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap3DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap3LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap3LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap3Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap3_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP4 0x1A90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap4DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap4LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap4DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap4LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap4DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap4LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap4DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap4LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap4LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap4Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap4_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP5 0x1A94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap5DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap5LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap5DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap5LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap5DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap5LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap5DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap5LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap5LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap5Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap5_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP6 0x1A98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap6DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap6LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap6DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap6LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap6DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap6LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap6DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap6LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap6LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap6Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap6_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP7 0x1A9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap7DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap7LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap7DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap7LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap7DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap7LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap7DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap7LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap7LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap7Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap7_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP8 0x1AA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap8DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap8LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap8DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap8LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap8DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap8LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap8DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap8LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap8LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap8Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap8_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP9 0x1AA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap9DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap9LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap9DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap9LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap9DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap9LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap9DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap9LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap9LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap9Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap9_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP10 0x1AA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap10DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap10LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap10DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap10LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap10DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap10LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap10DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap10LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap10LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap10Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap10_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP11 0x1AAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap11DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap11LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap11DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap11LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap11DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap11LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap11DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap11LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap11LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap11Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap11_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP12 0x1AB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap12DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap12LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap12DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap12LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap12DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap12LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap12DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap12LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap12LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap12Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap12_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP13 0x1AB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap13DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap13LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap13DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap13LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap13DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap13LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap13DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap13LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap13LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap13Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap13_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP14 0x1AB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap14DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap14LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap14DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap14LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap14DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap14LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap14DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap14LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap14LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap14Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap14_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP15 0x1ABC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap15DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap15LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap15DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap15LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap15DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap15LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap15DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap15LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap15LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap15Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap15_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP16 0x1AC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap16DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap16LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap16DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap16LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap16DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap16LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap16DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap16LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap16LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap16Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap16_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP17 0x1AC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap17DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap17LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap17DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap17LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap17DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap17LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap17DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap17LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap17LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap17Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap17_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP18 0x1AC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap18DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap18LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap18DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap18LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap18DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap18LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap18DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap18LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap18LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap18Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap18_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP19 0x1ACC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap19DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap19LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap19DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap19LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap19DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap19LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap19DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap19LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap19LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap19Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap19_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP20 0x1AD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap20DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap20LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap20DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap20LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap20DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap20LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap20DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap20LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap20LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap20Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap20_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP21 0x1AD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap21DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap21LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap21DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap21LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap21DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap21LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap21DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap21LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap21LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap21Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap21_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP22 0x1AD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap22DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap22LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap22DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap22LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap22DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap22LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap22DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap22LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap22LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap22Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap22_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP23 0x1ADC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap23DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap23LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap23DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap23LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap23DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap23LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap23DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap23LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap23LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap23Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap23_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP24 0x1AE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap24DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap24LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap24DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap24LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap24DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap24LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap24DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap24LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap24LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap24Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap24_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP25 0x1AE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap25DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap25LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap25DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap25LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap25DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap25LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap25DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap25LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap25LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap25Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap25_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP26 0x1AE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap26DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap26LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap26DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap26LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap26DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap26LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap26DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap26LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap26LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap26Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap26_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP27 0x1AEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap27DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap27LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap27DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap27LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap27DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap27LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap27DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap27LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap27LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap27Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap27_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP28 0x1AF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap28DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap28LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap28DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap28LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap28DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap28LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap28DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap28LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap28LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap28Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap28_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP29 0x1AF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap29DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap29LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap29DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap29LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap29DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap29LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap29DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap29LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap29LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap29Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap29_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP30 0x1AF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap30DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap30LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap30DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap30LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap30DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap30LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap30DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap30LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap30LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap30Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap30_u;

/*REG_B2_TX_SELECTOR_BITMAP_GLOBAL_BITMAP31 0x1AFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalBitmap31DataAc0 : 1; //AC0 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap31LockAc0 : 1; //AC0 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap31DataAc1 : 1; //AC1 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap31LockAc1 : 1; //AC1 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap31DataAc2 : 1; //AC2 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap31LockAc2 : 1; //AC2 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap31DataAc3 : 1; //AC3 data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap31LockAc3 : 1; //AC3 lock indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap31LegacyPs : 1; //Legacy PS indication bit, reset value: 0x0, access type: RW
		uint32 globalBitmap31Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2TxSelectorBitmapGlobalBitmap31_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP0 0x1B00 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap0Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap0Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap0Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap0_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP1 0x1B04 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap1Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap1Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap1Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap1_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP2 0x1B08 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap2Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap2Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap2Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap2_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP3 0x1B0C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap3Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap3Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap3Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap3_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP4 0x1B10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap4Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap4Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap4Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap4_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP5 0x1B14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap5Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap5Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap5Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap5_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP6 0x1B18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap6Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap6Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap6Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap6_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP7 0x1B1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap7Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap7Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap7Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap7_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP8 0x1B20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap8Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap8Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap8Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap8_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP9 0x1B24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap9Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap9Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap9Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap9_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP10 0x1B28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap10Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap10Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap10Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap10_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP11 0x1B2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap11Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap11Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap11Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap11_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP12 0x1B30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap12Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap12Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap12Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap12_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP13 0x1B34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap13Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap13Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap13Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap13_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP14 0x1B38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap14Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap14Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap14Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap14_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP15 0x1B3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap15Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap15Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap15Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap15_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP16 0x1B40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap16Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap16Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap16Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap16_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP17 0x1B44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap17Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap17Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap17Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap17_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP18 0x1B48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap18Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap18Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap18Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap18_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP19 0x1B4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap19Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap19Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap19Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap19_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP20 0x1B50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap20Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap20Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap20Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap20_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP21 0x1B54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap21Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap21Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap21Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap21_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP22 0x1B58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap22Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap22Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap22Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap22_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP23 0x1B5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap23Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap23Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap23Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap23_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP24 0x1B60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap24Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap24Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap24Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap24_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP25 0x1B64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap25Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap25Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap25Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap25_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP26 0x1B68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap26Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap26Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap26Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap26_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP27 0x1B6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap27Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap27Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap27Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap27_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP28 0x1B70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap28Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap28Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap28Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap28_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP29 0x1B74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap29Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap29Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap29Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap29_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP30 0x1B78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap30Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap30Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap30Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap30_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPLP_BITMAP31 0x1B7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpBitmap31Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap31Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gplpBitmap31Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGplpBitmap31_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP0 0x1B80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap0Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap0Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap0Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap0_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP1 0x1B84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap1Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap1Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap1Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap1_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP2 0x1B88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap2Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap2Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap2Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap2_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP3 0x1B8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap3Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap3Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap3Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap3_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP4 0x1B90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap4Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap4Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap4Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap4_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP5 0x1B94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap5Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap5Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap5Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap5_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP6 0x1B98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap6Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap6Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap6Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap6_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP7 0x1B9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap7Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap7Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap7Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap7_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP8 0x1BA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap8Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap8Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap8Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap8_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP9 0x1BA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap9Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap9Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap9Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap9_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP10 0x1BA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap10Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap10Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap10Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap10_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP11 0x1BAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap11Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap11Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap11Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap11_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP12 0x1BB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap12Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap12Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap12Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap12_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP13 0x1BB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap13Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap13Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap13Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap13_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP14 0x1BB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap14Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap14Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap14Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap14_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP15 0x1BBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap15Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap15Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap15Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap15_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP16 0x1BC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap16Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap16Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap16Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap16_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP17 0x1BC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap17Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap17Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap17Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap17_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP18 0x1BC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap18Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap18Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap18Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap18_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP19 0x1BCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap19Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap19Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap19Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap19_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP20 0x1BD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap20Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap20Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap20Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap20_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP21 0x1BD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap21Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap21Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap21Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap21_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP22 0x1BD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap22Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap22Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap22Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap22_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP23 0x1BDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap23Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap23Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap23Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap23_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP24 0x1BE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap24Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap24Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap24Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap24_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP25 0x1BE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap25Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap25Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap25Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap25_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP26 0x1BE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap26Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap26Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap26Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap26_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP27 0x1BEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap27Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap27Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap27Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap27_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP28 0x1BF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap28Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap28Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap28Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap28_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP29 0x1BF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap29Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap29Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap29Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap29_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP30 0x1BF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap30Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap30Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap30Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap30_u;

/*REG_B2_TX_SELECTOR_BITMAP_GPHP_BITMAP31 0x1BFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpBitmap31Data : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap31Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 gphpBitmap31Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2TxSelectorBitmapGphpBitmap31_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP0 0x1C00 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap0DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap0Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap0Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap0Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap0_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP1 0x1C04 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap1DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap1Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap1Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap1Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap1_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP2 0x1C08 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap2DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap2Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap2Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap2Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap2_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP3 0x1C0C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap3DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap3Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap3Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap3Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap3_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP4 0x1C10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap4DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap4Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap4Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap4Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap4_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP5 0x1C14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap5DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap5Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap5Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap5Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap5_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP6 0x1C18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap6DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap6Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap6Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap6Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap6_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP7 0x1C1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap7DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap7Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap7Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap7Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap7_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP8 0x1C20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap8DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap8Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap8Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap8Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap8_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP9 0x1C24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap9DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap9Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap9Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap9Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap9_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP10 0x1C28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap10DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap10Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap10Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap10Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap10_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP11 0x1C2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap11DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap11Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap11Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap11Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap11_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP12 0x1C30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap12DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap12Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap12Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap12Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap12_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP13 0x1C34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap13DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap13Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap13Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap13Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap13_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP14 0x1C38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap14DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap14Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap14Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap14Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap14_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP15 0x1C3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap15DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap15Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap15Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap15Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap15_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP16 0x1C40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap16DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap16Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap16Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap16Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap16_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP17 0x1C44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap17DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap17Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap17Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap17Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap17_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP18 0x1C48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap18DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap18Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap18Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap18Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap18_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP19 0x1C4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap19DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap19Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap19Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap19Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap19_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP20 0x1C50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap20DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap20Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap20Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap20Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap20_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP21 0x1C54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap21DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap21Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap21Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap21Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap21_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP22 0x1C58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap22DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap22Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap22Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap22Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap22_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP23 0x1C5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap23DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap23Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap23Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap23Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap23_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP24 0x1C60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap24DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap24Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap24Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap24Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap24_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP25 0x1C64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap25DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap25Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap25Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap25Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap25_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP26 0x1C68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap26DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap26Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap26Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap26Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap26_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP27 0x1C6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap27DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap27Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap27Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap27Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap27_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP28 0x1C70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap28DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap28Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap28Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap28Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap28_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP29 0x1C74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap29DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap29Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap29Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap29Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap29_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP30 0x1C78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap30DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap30Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap30Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap30Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap30_u;

/*REG_B2_TX_SELECTOR_BITMAP_BEACON_BITMAP31 0x1C7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconBitmap31DataInQ : 1; //data_in_queue indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap31Lock : 1; //lock indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap31Request : 1; //PS request indication bit, reset value: 0x0, access type: RW
		uint32 beaconBitmap31Disable : 1; //disable indication bit, reset value: 0x1, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2TxSelectorBitmapBeaconBitmap31_u;

/*REG_B2_TX_SELECTOR_BITMAP_PLAN_LOCK_BITMAP_31_0 0x1C80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 planLockBitmap310 : 32; //plan_lock[31:0] indication bits, reset value: 0x0, access type: RW
	} bitFields;
} RegB2TxSelectorBitmapPlanLockBitmap310_u;

/*REG_B2_TX_SELECTOR_BITMAP_PLAN_LOCK_BITMAP_63_32 0x1C84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 planLockBitmap6332 : 32; //plan_lock[63:32] indication bits, reset value: 0x0, access type: RW
	} bitFields;
} RegB2TxSelectorBitmapPlanLockBitmap6332_u;

/*REG_B2_TX_SELECTOR_BITMAP_PLAN_LOCK_BITMAP_95_64 0x1C88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 planLockBitmap9564 : 32; //plan_lock[95:64] indication bits, reset value: 0x0, access type: RW
	} bitFields;
} RegB2TxSelectorBitmapPlanLockBitmap9564_u;

/*REG_B2_TX_SELECTOR_BITMAP_PLAN_LOCK_BITMAP_127_96 0x1C8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 planLockBitmap12796 : 32; //plan_lock[127:96] indication bits, reset value: 0x0, access type: RW
	} bitFields;
} RegB2TxSelectorBitmapPlanLockBitmap12796_u;

/*REG_B2_TX_SELECTOR_BITMAP_PLAN_UL_HE_SELECTION_WITHOUT_DATA_BITMAP_31_0 0x1C90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 planUlHeSelectionWithoutDataBitmap310 : 32; //plan_ul_he_selection_without_data[31:0] indication bits, reset value: 0x0, access type: RW
	} bitFields;
} RegB2TxSelectorBitmapPlanUlHeSelectionWithoutDataBitmap310_u;

/*REG_B2_TX_SELECTOR_BITMAP_PLAN_UL_HE_SELECTION_WITHOUT_DATA_BITMAP_63_32 0x1C94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 planUlHeSelectionWithoutDataBitmap6332 : 32; //plan_ul_he_selection_without_data[63:32] indication bits, reset value: 0x0, access type: RW
	} bitFields;
} RegB2TxSelectorBitmapPlanUlHeSelectionWithoutDataBitmap6332_u;

/*REG_B2_TX_SELECTOR_BITMAP_PLAN_UL_HE_SELECTION_WITHOUT_DATA_BITMAP_95_64 0x1C98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 planUlHeSelectionWithoutDataBitmap9564 : 32; //plan_ul_he_selection_without_data[95:64] indication bits, reset value: 0x0, access type: RW
	} bitFields;
} RegB2TxSelectorBitmapPlanUlHeSelectionWithoutDataBitmap9564_u;

/*REG_B2_TX_SELECTOR_BITMAP_PLAN_UL_HE_SELECTION_WITHOUT_DATA_BITMAP_127_96 0x1C9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 planUlHeSelectionWithoutDataBitmap12796 : 32; //plan_ul_he_selection_without_data[127:96] indication bits, reset value: 0x0, access type: RW
	} bitFields;
} RegB2TxSelectorBitmapPlanUlHeSelectionWithoutDataBitmap12796_u;



#endif // _B2_TX_SELECTOR_BITMAP_REGS_H_
