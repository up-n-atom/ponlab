
/***********************************************************************************
File:				BeaconReq0Regs.h
Module:				beaconReq0
SOC Revision:		latest
Generated at:       2024-06-26 12:55:04
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _BEACON_REQ_0_REGS_H_
#define _BEACON_REQ_0_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define BEACON_REQ_0_BASE_ADDRESS                    MEMORY_MAP_UNIT_1162_BASE_ADDRESS
#define	REG_BEACON_REQ_0_BEACON_GUARD_INTERVAL_REG      (BEACON_REQ_0_BASE_ADDRESS + 0x0)
#define	REG_BEACON_REQ_0_BEACON_ACTIVE_TIMEOUT_REG      (BEACON_REQ_0_BASE_ADDRESS + 0x4)
#define	REG_BEACON_REQ_0_BEACON_REQ_EN_BITMAP_REG       (BEACON_REQ_0_BASE_ADDRESS + 0x8)
#define	REG_BEACON_REQ_0_BEACON_ACTIVE_STATUS_REG       (BEACON_REQ_0_BASE_ADDRESS + 0xC)
#define	REG_BEACON_REQ_0_CLEAR_TIMER_STATE_REG          (BEACON_REQ_0_BASE_ADDRESS + 0x10)
#define	REG_BEACON_REQ_0_BEACON_ON_VALID_LEVEL_REG      (BEACON_REQ_0_BASE_ADDRESS + 0x14)
#define	REG_BEACON_REQ_0_BEACON_ON_VAP_IDX_REG          (BEACON_REQ_0_BASE_ADDRESS + 0x18)
#define	REG_BEACON_REQ_0_ACTIVE_TIMEOUT_EN_REG          (BEACON_REQ_0_BASE_ADDRESS + 0x1C)
#define	REG_BEACON_REQ_0_BEACON_REQ_ENABLE_REG          (BEACON_REQ_0_BASE_ADDRESS + 0x20)
#define	REG_BEACON_REQ_0_BEACON_REQ_DISABLE_REG         (BEACON_REQ_0_BASE_ADDRESS + 0x24)
#define	REG_BEACON_REQ_0_BEACON_ON_CLEAR_REG            (BEACON_REQ_0_BASE_ADDRESS + 0x28)
#define	REG_BEACON_REQ_0_BEACON_ACTIVE_CLEAR_REG        (BEACON_REQ_0_BASE_ADDRESS + 0x2C)
#define	REG_BEACON_REQ_0_BEACON_ON_CLEAR_DELAY_REG      (BEACON_REQ_0_BASE_ADDRESS + 0x30)
#define	REG_BEACON_REQ_0_CURRENT_VAP_REG                (BEACON_REQ_0_BASE_ADDRESS + 0x34)
#define	REG_BEACON_REQ_0_PUSH_TO_FULL_FIFO_REG          (BEACON_REQ_0_BASE_ADDRESS + 0x38)
#define	REG_BEACON_REQ_0_PUSH_TO_FULL_FIFO_CLEAR_REG    (BEACON_REQ_0_BASE_ADDRESS + 0x3C)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_BEACON_REQ_0_BEACON_GUARD_INTERVAL_REG 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconGuardInterval : 14; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegBeaconReq0BeaconGuardIntervalReg_u;

/*REG_BEACON_REQ_0_BEACON_ACTIVE_TIMEOUT_REG 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconActiveTimeout : 14; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegBeaconReq0BeaconActiveTimeoutReg_u;

/*REG_BEACON_REQ_0_BEACON_REQ_EN_BITMAP_REG 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconReqEnBitmap : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconReq0BeaconReqEnBitmapReg_u;

/*REG_BEACON_REQ_0_BEACON_ACTIVE_STATUS_REG 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconActiveStatus : 16; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconReq0BeaconActiveStatusReg_u;

/*REG_BEACON_REQ_0_CLEAR_TIMER_STATE_REG 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 clearTimerState : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegBeaconReq0ClearTimerStateReg_u;

/*REG_BEACON_REQ_0_BEACON_ON_VALID_LEVEL_REG 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconOnValidLevel : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegBeaconReq0BeaconOnValidLevelReg_u;

/*REG_BEACON_REQ_0_BEACON_ON_VAP_IDX_REG 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconOnVapIdx : 4; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegBeaconReq0BeaconOnVapIdxReg_u;

/*REG_BEACON_REQ_0_ACTIVE_TIMEOUT_EN_REG 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 activeTimeoutEn : 16; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconReq0ActiveTimeoutEnReg_u;

/*REG_BEACON_REQ_0_BEACON_REQ_ENABLE_REG 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconReqEnable : 4; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegBeaconReq0BeaconReqEnableReg_u;

/*REG_BEACON_REQ_0_BEACON_REQ_DISABLE_REG 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconReqDisable : 4; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegBeaconReq0BeaconReqDisableReg_u;

/*REG_BEACON_REQ_0_BEACON_ON_CLEAR_REG 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconOnClear : 4; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegBeaconReq0BeaconOnClearReg_u;

/*REG_BEACON_REQ_0_BEACON_ACTIVE_CLEAR_REG 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconActiveClear : 4; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegBeaconReq0BeaconActiveClearReg_u;

/*REG_BEACON_REQ_0_BEACON_ON_CLEAR_DELAY_REG 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconOnClearDelay : 5; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegBeaconReq0BeaconOnClearDelayReg_u;

/*REG_BEACON_REQ_0_CURRENT_VAP_REG 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 currentVap : 4; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegBeaconReq0CurrentVapReg_u;

/*REG_BEACON_REQ_0_PUSH_TO_FULL_FIFO_REG 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pushToFullFifo : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegBeaconReq0PushToFullFifoReg_u;

/*REG_BEACON_REQ_0_PUSH_TO_FULL_FIFO_CLEAR_REG 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pushToFullFifoClear : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegBeaconReq0PushToFullFifoClearReg_u;

//========================================
/* REG_BEACON_REQ_0_BEACON_GUARD_INTERVAL_REG 0x0 */
#define BEACON_REQ_0_BEACON_GUARD_INTERVAL_REG_FIELD_BEACON_GUARD_INTERVAL_MASK                                   0x00003FFF
//========================================
/* REG_BEACON_REQ_0_BEACON_ACTIVE_TIMEOUT_REG 0x4 */
#define BEACON_REQ_0_BEACON_ACTIVE_TIMEOUT_REG_FIELD_BEACON_ACTIVE_TIMEOUT_MASK                                   0x00003FFF
//========================================
/* REG_BEACON_REQ_0_BEACON_REQ_EN_BITMAP_REG 0x8 */
#define BEACON_REQ_0_BEACON_REQ_EN_BITMAP_REG_FIELD_BEACON_REQ_EN_BITMAP_MASK                                     0x0000FFFF
//========================================
/* REG_BEACON_REQ_0_BEACON_ACTIVE_STATUS_REG 0xC */
#define BEACON_REQ_0_BEACON_ACTIVE_STATUS_REG_FIELD_BEACON_ACTIVE_STATUS_MASK                                     0x0000FFFF
//========================================
/* REG_BEACON_REQ_0_CLEAR_TIMER_STATE_REG 0x10 */
#define BEACON_REQ_0_CLEAR_TIMER_STATE_REG_FIELD_CLEAR_TIMER_STATE_MASK                                           0x00000003
//========================================
/* REG_BEACON_REQ_0_BEACON_ON_VALID_LEVEL_REG 0x14 */
#define BEACON_REQ_0_BEACON_ON_VALID_LEVEL_REG_FIELD_BEACON_ON_VALID_LEVEL_MASK                                   0x00000001
//========================================
/* REG_BEACON_REQ_0_BEACON_ON_VAP_IDX_REG 0x18 */
#define BEACON_REQ_0_BEACON_ON_VAP_IDX_REG_FIELD_BEACON_ON_VAP_IDX_MASK                                           0x0000000F
//========================================
/* REG_BEACON_REQ_0_ACTIVE_TIMEOUT_EN_REG 0x1C */
#define BEACON_REQ_0_ACTIVE_TIMEOUT_EN_REG_FIELD_ACTIVE_TIMEOUT_EN_MASK                                           0x0000FFFF
//========================================
/* REG_BEACON_REQ_0_BEACON_REQ_ENABLE_REG 0x20 */
#define BEACON_REQ_0_BEACON_REQ_ENABLE_REG_FIELD_BEACON_REQ_ENABLE_MASK                                           0x0000000F
//========================================
/* REG_BEACON_REQ_0_BEACON_REQ_DISABLE_REG 0x24 */
#define BEACON_REQ_0_BEACON_REQ_DISABLE_REG_FIELD_BEACON_REQ_DISABLE_MASK                                         0x0000000F
//========================================
/* REG_BEACON_REQ_0_BEACON_ON_CLEAR_REG 0x28 */
#define BEACON_REQ_0_BEACON_ON_CLEAR_REG_FIELD_BEACON_ON_CLEAR_MASK                                               0x0000000F
//========================================
/* REG_BEACON_REQ_0_BEACON_ACTIVE_CLEAR_REG 0x2C */
#define BEACON_REQ_0_BEACON_ACTIVE_CLEAR_REG_FIELD_BEACON_ACTIVE_CLEAR_MASK                                       0x0000000F
//========================================
/* REG_BEACON_REQ_0_BEACON_ON_CLEAR_DELAY_REG 0x30 */
#define BEACON_REQ_0_BEACON_ON_CLEAR_DELAY_REG_FIELD_BEACON_ON_CLEAR_DELAY_MASK                                   0x0000001F
//========================================
/* REG_BEACON_REQ_0_CURRENT_VAP_REG 0x34 */
#define BEACON_REQ_0_CURRENT_VAP_REG_FIELD_CURRENT_VAP_MASK                                                       0x0000000F
//========================================
/* REG_BEACON_REQ_0_PUSH_TO_FULL_FIFO_REG 0x38 */
#define BEACON_REQ_0_PUSH_TO_FULL_FIFO_REG_FIELD_PUSH_TO_FULL_FIFO_MASK                                           0x00000001
//========================================
/* REG_BEACON_REQ_0_PUSH_TO_FULL_FIFO_CLEAR_REG 0x3C */
#define BEACON_REQ_0_PUSH_TO_FULL_FIFO_CLEAR_REG_FIELD_PUSH_TO_FULL_FIFO_CLEAR_MASK                               0x00000001


#endif // _BEACON_REQ_0_REGS_H_
